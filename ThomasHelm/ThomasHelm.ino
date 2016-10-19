/* Daniel Sarmiento
 * Code for my Thomas Daft Punk Visor
 * Current animations:
 * 0  Count up timer
 * 1  Marquee
 * 2  Heart beat
 * 3  Heart blink
 * 4  RoboCop
 * 5  White noise
 * 6  Blinking eyes
 * 7  Static text
 * 8  All on
 * 9  All off
 * 10 Pacman
 * 11 Cyclops
 */

#include "Const.h" 
#include <SoftwareSerial.h>

// Connected pins
#define latchPin 8  //Pin connected to ST_CP of 74HC595
#define clockPin 12 //Pin connected to SH_CP of 74HC595
#define dataPin 11  //Pin connected to DS of 74HC595

int option = 9;
boolean flag = true;

SoftwareSerial bluetooth = SoftwareSerial(2,3);

// First time setup
void setup() 
{
    // Shift register
    pinMode(latchPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    pinMode(dataPin, OUTPUT);
    digitalWrite(latchPin, HIGH);
    Serial.begin(9600);
    bluetooth.begin(115200);
    bluetooth.print("$$$");
    delay(100);
    bluetooth.println("U,9600,N");
    bluetooth.print("---");
    bluetooth.end();
    bluetooth.begin(9600);
    bluetooth.print("---");
}

// For counter option
int cnt = 0; 

// For marquee option
int steps = 0;
byte temp[5];
byte ch;
char msg[256] = "HUMAN      "; // HELLO WORLD
char txt[6]   = "HUMAN";
int msgLen = 0;

void loop()
{
  // Check serial for new request
  if(bluetooth.available() > 0)
  {
    option = bluetooth.parseInt();
    Serial.println(option);
    if(option == 1)
    {
      flag = true;
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
  else if(option == 2)
  {
    heartBeat();
  }
  else if(option == 3)
  {
    heartBlink();
  }
  else if(option == 4)
  {
    roboCop(15);
  }
  else if(option == 5)
  {
    noise();
  }
  else if(option == 6)
  {
    eyes();
  }
  else if(option == 7)
  {
    text(txt);
  }
  else if(option == 8)
  {
    all(true);
  }
  else if(option == 9)
  {
    all(false);
  }
  else if(option == 10)
  {
    pacman();
  }
  else if(option == 11)
  {
    cyclops();
  }
  else if(option == 12)
  {
    daftPunk();
  }
}

void daftPunk()
{
  long timer = millis();
  while(millis() < timer + 750)
    text(" DAFT ");
    
  timer = millis();
  while(millis() < timer + 250)
    all(false);
    
  timer = millis();
  while(millis() < timer + 750)
    text(" PUNK ");
    
  timer = millis();
  while(millis() < timer + 250)
    all(false);
}

void countUp(int cnt)
{
  uint8_t data[8][6];
  for(int i = 0; i < 8; i++)
    for(int j = 0; j < 6; j++)
      data[i][j] = 0x00;

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
  uint8_t data[8][6];
  if(flag)
  {
    flag = false;
    for(int i = 0; i < 8; i++)
      for(int j = 0; j < 6; j++)
        data[i][j] = 0x00;
  }
      
    for(int i = 0; i < 8; i++){
      ch = 0;
      if(steps % 6 != 5){  
        int pos = 4 - (steps % 6);
        if(msg[(steps/6)%msgLen] != ' '){
          ch = alphabet[msg[(steps/6)%msgLen] - 'A'][i];
          ch >>= pos;
          ch &= 0x01;
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

void roboCop(int timerDelay)
{
  uint8_t data[8][6];
  uint8_t robo[5] = {0xF0, 0x00, 0x00, 0x00, 0x00};
  uint8_t temp[5];

  for(int i = 0; i < 36; i++)
  {
    temp[0] = robo[0] & 0x01;
    temp[1] = robo[1] & 0x01;
    temp[2] = robo[2] & 0x01;
    temp[3] = robo[3] & 0x01;
    temp[4] = robo[4] & 0x01;
    
    robo[0] >>= 1;
    robo[1] >>= 1;
    robo[2] >>= 1;
    robo[3] >>= 1;
    robo[4] >>= 1;
    
    robo[1] |= temp[0] << 7;
    robo[2] |= temp[1] << 7;
    robo[3] |= temp[2] << 7;
    robo[4] |= temp[3] << 7;
    
    for(int j = 0; j < 8; j++)
    {
      data[j][0] = 1 << j;
      data[j][1] = robo[0];
      data[j][2] = robo[1];
      data[j][3] = robo[2];
      data[j][4] = robo[3];
      data[j][5] = robo[4];
    }
    sendScreen(data, timerDelay);
  }

  for(int i = 0; i < 36; i++)
  {
    temp[0] = robo[0] & 0x80;
    temp[1] = robo[1] & 0x80;
    temp[2] = robo[2] & 0x80;
    temp[3] = robo[3] & 0x80;
    temp[4] = robo[4] & 0x80;
    
    robo[0] <<= 1;
    robo[1] <<= 1;
    robo[2] <<= 1;
    robo[3] <<= 1;
    robo[4] <<= 1;
    
    robo[0] |= temp[1] >> 7;
    robo[1] |= temp[2] >> 7;
    robo[2] |= temp[3] >> 7;
    robo[3] |= temp[4] >> 7;
    
    for(int j = 0; j < 8; j++)
    {
      data[j][0] = 1 << j;
      data[j][1] = robo[0];
      data[j][2] = robo[1];
      data[j][3] = robo[2];
      data[j][4] = robo[3];
      data[j][5] = robo[4];
    }
    sendScreen(data, timerDelay);
  }
}

void heartBeat()
{
  uint8_t data[8][6];
  uint8_t mask[5] = {0x00, 0x00, 0x00, 0x00, 0x00};
  for(int j = 0; j < 40; j++)
  {
    mask[j / 8] |= 1 << 7 - (j % 8);
    for(int i = 0; i < 8; i++)
    {
      data[i][0] = 1 << i;
      data[i][1] = heartbeat[i][0] & mask[0];
      data[i][2] = heartbeat[i][1] & mask[1];
      data[i][3] = heartbeat[i][2] & mask[2];
      data[i][4] = heartbeat[i][3] & mask[3];
      data[i][5] = heartbeat[i][4] & mask[4];
    }
    sendScreen(data, 25);
  }
}

void heartBlink()
{
  uint8_t data[8][6];
  if((millis()/500) % 2 == 0)
  {
    for(int i = 0; i < 8; i++)
    {
      data[i][0] = 1 << i;
      if(i != 7)
      {
        data[i][1]  = heartEmpty[i][0];
        data[i][2]  = heartEmpty[i][1];
        data[i][2] |= heartEmpty[i][0] >> 7;
        data[i][3]  = heartEmpty[i][0] << 1;
        data[i][3] |= heartEmpty[i][1] >> 7;
        data[i][4]  = heartEmpty[i][1] << 1;
        data[i][4] |= heartEmpty[i][0] >> 6;
        data[i][5]  = heartEmpty[i][0] << 2;
        data[i][5] |= heartEmpty[i][1] >> 6;
      }
      else
      {
        data[i][1] = 0x00;
        data[i][2] = 0x00;
        data[i][3] = 0x00;
        data[i][4] = 0x00;
        data[i][5] = 0x00;
      }
    }
  }
  else
  {
    for(int i = 0; i < 8; i++)
    {
      data[i][0] = 1 << i;

      data[i][1]  = heartFull[i][0];
      data[i][2]  = heartFull[i][1];
      data[i][2] |= heartFull[i][0] >> 7;
      data[i][3]  = heartFull[i][0] << 1;
      data[i][3] |= heartFull[i][1] >> 7;
      data[i][4]  = heartFull[i][1] << 1;
      data[i][4] |= heartFull[i][0] >> 6;
      data[i][5]  = heartFull[i][0] << 2;
      data[i][5] |= heartFull[i][1] >> 6;
    }
  }
  sendScreen(data, 50);
}

void noise()
{
  uint8_t data[8][6];
  for(int i = 0; i < 8; i++)
  {
    data[i][0] = 1 << i;
    for(int j = 1; j < 6; j++)
    {
      data[i][j] = random(256);
    }
  }
  sendScreen(data, 50);
}

void eyes()
{
  uint8_t data[8][6];
  for(int k = 0; k < 4; k++)
  {
    for(int i = 0; i < 8; i++)
    {
      data[i][0] = 1 << i;
      data[i][1] = 0x00;
      data[i][2] = blinking[k][i];
      data[i][3] = 0x00;
      data[i][4] = blinking[k][i];
      data[i][5] = 0x00;
    }
    if(k == 0)
      sendScreen(data, random(5) * 1000);
    else
      sendScreen(data, 100);
  }
  for(int k = 3; k > 0; k--)
  {
    for(int i = 0; i < 8; i++)
    {
      data[i][0] = 1 << i;
      data[i][1] = 0x00;
      data[i][2] = blinking[k][i];
      data[i][3] = 0x00;
      data[i][4] = blinking[k][i];
      data[i][5] = 0x00;
    }
    sendScreen(data, 100);
  }
}

void text(char txt[6])
{
  uint8_t data[8][6];
  for(int i = 0; i < 8; i++)
  {
    data[i][0]  = 1 << i;
    if(msg[0] != ' ')
    {
      data[i][1]  = alphabet[txt[0] - 'A'][i];
    }
    else
    {
      data[i][1]  = 0x00;
    }
    if(msg[1] != ' ')
    {
      data[i][2]  = alphabet[txt[1] - 'A'][i] << 2;
    }
    else
    {
      data[i][2]  = 0x00;
    }
    if(msg[2] != ' ')
    {
      data[i][2] |= alphabet[txt[2] - 'A'][i] >> 4;
      data[i][3]  = alphabet[txt[2] - 'A'][i] << 4;
    }
    else
    {
      data[i][2] &= ~0x03;
      data[i][3]  = 0x00; 
    }
    if(msg[3] != ' ')
    {
      data[i][3] |= alphabet[txt[3] - 'A'][i] >> 2;
      data[i][4]  = alphabet[txt[3] - 'A'][i] << 6;
    }
    else
    {
      data[i][3] &= ~0x0F;
      data[i][4]  = 0x00;
    }
    if(msg[4] != ' ')
    {
      data[i][4] |= alphabet[txt[4] - 'A'][i];
    }
    else
    {
      data[i][4] &= ~0x3F;
    }
    if(msg[5] != ' ')
    {
      data[i][5]  = alphabet[txt[5] - 'A'][i] << 2;
    }
    else
    {
      data[i][5]  = 0x00;
    }
  }
  sendScreen(data, 50);
}

void all(boolean x)
{
  uint8_t data[8][6];
  for(int i = 0; i < 8; i++)
  {
    data[i][0] = 1 << i;
    for(int j = 0; j < 6; j++)
    {
      if(x)
        data[i][j] = 0xFF;
      else
        data[i][j] = 0x00;
    }
  }
  sendScreen(data, 50);
}

void pacman()
{
  uint8_t data[8][6];
  uint8_t temp;
  for(int i = 0; i < 48; i++)
  {
    int k = 0;
    switch(i % 6){
      case 0:
        k = 0;
        break;
      case 1:
      case 5:
        k = 1;
        break;
      case 2:
      case 4:
        k = 2;
        break;
      case 3:
        k = 3;
        break;
    }
    for(int j = 0; j < 8; j++)
    {
      data[j][0] = 1 << j;

      

      if(i < 8)
        data[j][1] = pacMan[k][j] << 7 - (i%8);
      else if(i < 16)
        data[j][1] = pacMan[k][j] >> i%8;
      else
        data[j][1] = 0x00;

      if(i >= 8 && i < 16)
        data[j][2] = pacMan[k][j] << 7 - (i%8);
      else if(i >= 16 && i < 24)
        data[j][2] = pacMan[k][j] >> i%8;
      else
        data[j][2] = 0x00;

      if(i >=16 && i < 24)
        data[j][3] = pacMan[k][j] << 7 - (i%8);
      else if(i >= 24 && i < 32)
        data[j][3] = pacMan[k][j] >> i%8;
      else
        data[j][3] = 0x00;

      if(i >= 24 && i < 32)
        data[j][4] = pacMan[k][j] << 7 - (i%8);
      else if(i >= 32 && i < 40)
        data[j][4] = pacMan[k][j] >> i%8;
      else
        data[j][4] = 0x00;

      if(i >= 32 && i < 40)
        data[j][5] = pacMan[k][j] << 7 - (i%8);
      else if(i >= 40)
        data[j][5] = pacMan[k][j] >> i%8;
      else
        data[j][5] = 0x00;

      if(j == 3 || j == 4)
      {
        for(int dot = 0; dot < 5; dot++)
        {
          if(dot >= (i / 8))
          {
            data[j][dot+1] |= 0x18;
          }
        }
      }

    }
    sendScreen(data, 100);
  }
}

void cyclops()
{
  uint8_t data[8][6];
  for(int i = 0; i < 8; i++)
  {
    data[i][0] = 1 << i;
    for(int j = 1; j < 6; j++)
    {
      if(i == 3 | i == 4)
        data[i][j] = 0xFF;
      else
        data[i][j] = 0x00;
    }
  }
  sendScreen(data, 50);
}

void sendScreen(uint8_t data[8][6], int timerDelay)
{
    long timer = millis();
    while(millis() < timer + timerDelay)
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
