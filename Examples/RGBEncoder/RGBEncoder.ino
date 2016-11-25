int encoderPin1 = 2;
int encoderPin2 = 3;
int R = 9;
int G = 10;
int B = 11;

volatile int lastEncoded = 0;
volatile long encoderValue = 0;

long lastencoderValue = 0;

int lastMSB = 0;
int lastLSB = 0;

void setup() {
  Serial.begin (9600);

  pinMode(encoderPin1, INPUT); 
  pinMode(encoderPin2, INPUT);
  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);
  analogWrite(R, 255);
  analogWrite(G, 255);
  analogWrite(B, 255);

  digitalWrite(encoderPin1, HIGH); //turn pullup resistor on
  digitalWrite(encoderPin2, HIGH); //turn pullup resistor on

  //call updateEncoder() when any high/low changed seen
  //on interrupt 0 (pin 2), or interrupt 1 (pin 3) 
  attachInterrupt(0, updateEncoder, CHANGE); 
  attachInterrupt(1, updateEncoder, CHANGE);

}

void loop(){ 
  //Do stuff here

  if((encoderValue/4)%3 == 0)
  {
    analogWrite(R, 0);
    analogWrite(G, 255);
    analogWrite(B, 255);
  }
  if((encoderValue/4)%3 == 1)
  {
    analogWrite(R, 255);
    analogWrite(G, 0);
    analogWrite(B, 255);   
  }
  if((encoderValue/4)%3 == 2)
  {
    analogWrite(R, 255);
    analogWrite(G, 255);
    analogWrite(B, 0);
  }
  //Serial.println(encoderValue/4);
}


void updateEncoder(){
  int MSB = digitalRead(encoderPin1); //MSB = most significant bit
  int LSB = digitalRead(encoderPin2); //LSB = least significant bit

  int encoded = (MSB << 1) |LSB; //converting the 2 pin value to single number
  int sum  = (lastEncoded << 2) | encoded; //adding it to the previous encoded value

  if(sum == 13 || sum == 4 || sum == 2 || sum == 11) encoderValue ++;
  if(sum == 14 || sum == 7 || sum == 1 || sum == 8 ) encoderValue --;

  lastEncoded = encoded; //store this value for next time
}
