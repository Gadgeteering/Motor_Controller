//const byte ledPin = 13;
// PORTB 
//  NI,NU,HS_B,HS_A,LS_A,LS_B, NU,NU
#define motor_debug
const byte Low_Side_A = 13; // Black
const byte High_Side_A = 11; // White
const byte Low_Side_B = 12; //Red
const byte High_Side_B = 10; // Green 
const byte interruptPin = 3;
byte PORT;
volatile byte state = LOW;

unsigned long mark,space,loops,inp;
unsigned long period;
 int i;
unsigned int timer_us,last_timer_us,timer_ms,last_timer_ms;
bool flipflop;
bool hall_state;
bool last_hall_state;
bool Running;

void setup() {
  Serial.begin(57600);
  //PORTB = B110000;
  //DDRB =B00111100;
  
  pinMode(Low_Side_A, OUTPUT);
  pinMode(Low_Side_B, OUTPUT);
  pinMode(High_Side_A, OUTPUT);
  pinMode(High_Side_B, OUTPUT);
  
 All_Off();
 pinMode(interruptPin, INPUT_PULLUP);
 //attachInterrupt(digitalPinToInterrupt(interruptPin), Measure_Speed, CHANGE);
 //attachInterrupt(digitalPinToInterrupt(interruptPin), Commutate_B, FALLING);
 Serial.println("Start");
 
 last_hall_state=digitalRead(interruptPin);
 mark =100;
 space =900;
 Running = false;
 period =500000;
 last_timer_us=micros();
 last_timer_ms = millis();
}

void loop() {
  
  
//Test();
//int last_interrupt;
//i=digitalRead(interruptPin);
//Serial.println(i);
//  
//  last_interrupt = micros()()-timer; 
//  if (last_interrupt>1000)  {
//   Start_running();
//   }
//Commutate();
//All_Off();
if (Serial.available() >0){
  String buff = Serial.readStringUntil(13);
  inp=buff.charAt(0);
  Serial.println(buff);
  Serial.println(buff.length());
  Serial.println(inp);
  if (buff.length() > 0)
  { 
    Serial.print("Send:");
    Serial.println(inp);
    mark= (inp -48)*100;
    space= 1000-mark;
    Serial.print("Mark:");
    Serial.print(mark);Serial.print("Space:");Serial.println(space);
    Test(inp);
  }
}


}


void Measure_Speed()
{
  unsigned long delta_timer;
 //Serial.print("Measure Speed") ;
 timer_us = micros();
 timer_ms = millis();
 unsigned int delta_timer_ms = timer_ms-last_timer_ms;
 
 if (delta_timer_ms < 1){
 delta_timer = timer_us-last_timer_us;
 }
 else
 {
 delta_timer = (timer_ms-last_timer_ms)*1000;
 
 }
 last_timer_ms=timer_ms;
 last_timer_us=timer_us;
 Serial.println(delta_timer);
 //period=delta_timer;
 flipflop = !flipflop;
}

void Commutate (){
  loops= period/(mark+space);
#ifdef motor_debug
  Serial.print("period:");
  Serial.println(period);
#endif
#ifdef motor_debug
  Serial.print("Loops:");
  Serial.println(loops);
#endif

  timer_ms = millis();
  unsigned long delta_timer = timer_ms-last_timer_ms;
#ifdef motor_debug
  Serial.print("delta:");
  Serial.println(delta_timer);
#endif
  if (delta_timer > 500) 
  {period = 500000;
  flipflop = !flipflop;
#ifdef motor_debug
  Serial.print("flip:");
  
#endif
  }
  //Serial.print("delay");
  
//  }
  //last_hall_state = hall_state;
  //Serial.print("Pin:");
  //Serial.println(hall_state);
  //if (i == 0)  flipflop = !flipflop;
  
  //Serial.print("flipflop:");
  //Serial.println(flipflop);
  if (flipflop == true)
  {Commutate_B(); } else {Commutate_A(); }
 
  
}
void Commutate_A() {
#ifdef motor_debug
  Serial.println("Com_A Low B Hig");
#endif
  //Off First
  digitalWrite(Low_Side_B, HIGH);//OFF
  digitalWrite(High_Side_A, HIGH);//OFF
   digitalWrite(High_Side_B, LOW);//On
  for (int cycles = 0; cycles < loops; cycles ++)
  {
  digitalWrite(Low_Side_A, LOW);// On
  delayMicroseconds(mark);
  digitalWrite(Low_Side_A, HIGH);// On
  delayMicroseconds(space);
  }
  All_Off();
 
  
}
void Commutate_B(){
#ifdef motor_debug
  Serial.println("Com_A High B Low");
#endif
  //Off First
  digitalWrite(Low_Side_A, HIGH);// Off
  digitalWrite(High_Side_B, HIGH);//OFF
  //On
  
  digitalWrite(High_Side_A, LOW);//On
  for (int cycles = 0; cycles < loops; cycles ++)
  {
  digitalWrite(Low_Side_B, LOW);//On
  delayMicroseconds(mark);  
  digitalWrite(Low_Side_B, HIGH);//off
  delayMicroseconds(space);
  }
  All_Off();
}

void All_Off ()
{
 // Serial.println("Off");
  digitalWrite(Low_Side_A, HIGH);// Off
  digitalWrite(Low_Side_B, HIGH);//Off
  digitalWrite(High_Side_A, HIGH);//Off
  digitalWrite(High_Side_B, HIGH);//Off
  
  
}

void Test (int sw)
{
  int Delay =2000;
Serial.println("Before");
Serial.println(sw);
sw= sw -48 ;
Serial.println(sw);
switch (sw) {
  

  case 1:
    digitalWrite(Low_Side_A, HIGH);// Off
    digitalWrite(Low_Side_B, HIGH);//On
    digitalWrite(High_Side_A, LOW);//OFF
    digitalWrite(High_Side_B, HIGH);//OFF
    Serial.println("High_Side_A On");
    break;//delay(Delay);
  case 2:
    digitalWrite(Low_Side_A, HIGH);// Off
    digitalWrite(Low_Side_B, HIGH);//On
    digitalWrite(High_Side_A, HIGH);//OFF
    digitalWrite(High_Side_B, LOW);//OFF
    Serial.println("High_Side_B On");
    break;//delay(Delay);
  case 3:
    digitalWrite(Low_Side_A, LOW);// Onn
    digitalWrite(Low_Side_B, HIGH);//Off
    digitalWrite(High_Side_A, HIGH);//Off
    digitalWrite(High_Side_B, HIGH);//Off
    Serial.println("Low_Side_A On");
    break;//delay(Delay);
  case 4:
    digitalWrite(Low_Side_A, HIGH);// Off
    digitalWrite(Low_Side_B, LOW);//On
    digitalWrite(High_Side_A, HIGH);//OFF
    digitalWrite(High_Side_B, HIGH);//OFF
    Serial.println("Low_Side_B On");
    break;//delay(Delay);
//  digitalWrite(Low_Side_A, HIGH);// On
//  digitalWrite(Low_Side_B, LOW);//Off
//  digitalWrite(High_Side_A, HIGH);//Off
//  digitalWrite(High_Side_B, HIGH);//Off
//  Serial.println("Low A");
//  delay(Delay);
//  digitalWrite(Low_Side_A, LOW);// Off
//  digitalWrite(Low_Side_B, LOW);//Off
//  digitalWrite(High_Side_A, LOW);//Off
//  digitalWrite(High_Side_B, HIGH);//Off
//  Serial.println("High A");
//  delay(Delay);
//  digitalWrite(Low_Side_A, LOW);// Off
//  digitalWrite(Low_Side_B, LOW);//Off
//  digitalWrite(High_Side_A, HIGH);//Off
//  digitalWrite(High_Side_B, LOW);//Off
// Serial.println("High B");
//  delay(Delay*3);
  default:
  All_Off();
  Serial.print("All Off");
  break;
}
}
