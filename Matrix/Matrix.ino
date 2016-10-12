/* Made numbers for the matrix
 * Starting clock code soon (make it loop through 00:00 - 99:99 in a short time span)
 * Data being sent to registers are 48 Bits or 6 Bytes, 1 Byte for row and 5 for columns
 * Data transmitted must be in the order of Col4, Col3, Col2, Col1, Col0, Rows (last shift is first register)
 * Data transmitted must be inverted for Col0-4 (since we need to drive the ones we want on LOW for GND
 * wrote everything as if it would be driven HIGH for my own sanity
 */

#include "Const.h" 

// Connected pins
#define latchPin 8  //Pin connected to ST_CP of 74HC595
#define clockPin 12 //Pin connected to SH_CP of 74HC595
#define dataPin 11  //Pin connected to DS of 74HC595

int option = 1;
byte data[8][6]; // 0 - Rows; 5 - Col4



// First time setup
void setup() 
{
    // Shift register
    pinMode(latchPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    pinMode(dataPin, OUTPUT);
    digitalWrite(latchPin, HIGH);
}

// For counter option
int cnt = 0; 

// For marquee option
int steps = 0;
byte temp[5];
byte ch;
char msg[256] = "HELLO WORLD"; // HELLO WORLD
int msgLen = 0;

void loop()
{
  // Check serial for new request
  if(Serial.available() > 0)
  {
    char in = Serial.read();
    option = (int)in - '0';
    if(option == 1)
    {
      Serial.print("OK");
      while(Serial.available() == 0);
      String message = Serial.readString();
      message.toCharArray(msg, 256);
      msgLen = message.length();
    }
    
  }
  
  if(option == 0)
  {
    countUp(cnt);
    cnt++;
  }
  else if(option == 1)
  {
    marquee(msg);
  }
}

void countUp(int cnt)
{
    // Wrap over counter
    if(cnt > 9999)
        cnt = 0;  

    // Grab the digits
    int digits[4]; // Digits in time 01:23
    digits[0] = cnt/1000;
    digits[1] = (cnt/100)%10;
    digits[2] = (cnt/10)%10;
    digits[3] = cnt%10;

    // Go through the rows
    for(int i = 0; i < 8; i++)
    {
      byte temp = 0;
      // Compute the row data
      data[i][0] = 1 << i;
      
      temp = numbers[digits[0]][i] >> 1;
      data[i][1] = temp;
      
      temp = 0;
      temp = numbers[digits[0]][i] << 7;
      temp |= numbers[digits[1]][i] >> 2;
      data[i][2] = temp;
      
      temp = 0;
      temp = numbers[digits[1]][i] << 6;
      temp |= colon[i];
      temp |= numbers[digits[2]][i] >> 6;
      data[i][3] = temp;
      
      temp = 0;
      temp = numbers[digits[2]][i] << 2;
      temp |= numbers[digits[3]][i] >> 7;
      data[i][4] = temp;
      
      temp = 0;
      temp = numbers[digits[3]][i] << 1;
      data[i][5] = temp;
      
      
    }
    sendScreen(data, 50);
}


void marquee(char msg[])
{
    for(int i = 0; i < 8; i++){
      ch = 0;
      if(steps % 6 != 5){  
        int pos = 4 - (steps % 6);
        if(msg[(steps/6)%msgLen] != ' '){
          ch = alphabet[msg[(steps/6)%msgLen] - 'A'][i];
          ch >>= pos;
          ch &= 1;
        }else{
          ch = 0;
        }
      }
      
      temp[0] = data[i][1];
      temp[1] = data[i][2];
      temp[2] = data[i][3];
      temp[3] = data[i][4];
      temp[4] = data[i][5];
      
      data[i][1] <<= 1;
      data[i][2] <<= 1;
      data[i][3] <<= 1;
      data[i][4] <<= 1;
      data[i][5] <<= 1;
      
      data[i][5] |= ch;
      temp[4] >>= 7;
      data[i][4] |= temp[4];
      temp[3] >>= 7;
      data[i][3] |= temp[3];
      temp[2] >>= 7;
      data[i][2] |= temp[2];
      temp[1] >>= 7;
      data[i][1] |= temp[1];
      data[i][0] = 1 << i;
      
    } 
    sendScreen(data, 150);
    steps++;  
}

void blink()
{
  
}


void sendScreen(uint8_t data[8][6], int delay)
{
    long timer = millis();
    while(millis() < timer + delay)
    {
        for(int i = 0; i < 8; i++)
        {
            digitalWrite(latchPin, LOW);
            shiftOut(dataPin, clockPin, LSBFIRST, ~data[i][5]);
            shiftOut(dataPin, clockPin, LSBFIRST, ~data[i][4]); 
            shiftOut(dataPin, clockPin, LSBFIRST, ~data[i][3]); 
            shiftOut(dataPin, clockPin, LSBFIRST, ~data[i][2]); 
            shiftOut(dataPin, clockPin, LSBFIRST, ~data[i][1]); 
            shiftOut(dataPin, clockPin, LSBFIRST, data[i][0]);  
            digitalWrite(latchPin, HIGH);
        }
    }  
}
