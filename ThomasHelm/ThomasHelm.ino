/* Daniel Sarmiento
 * Code for my Thomas Daft Punk Visor
 * Pin 2 - TX on Bluetooth
 * Pin 3 - RX on Bluetooth
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
 * 12 Daft Punk
 */

#include "Const.h" 
#include <SoftwareSerial.h>
#include<FastLED.h>


#define DEBUG false

// Connected pins
#define latchPin 8  // Pin connected to ST_CP of 74HC595
#define clockPin 12 // Pin connected to SH_CP of 74HC595
#define dataPin 11  // Pin connected to DS of 74HC595
#define SPEED_UP 4  // x4 speed up and x1/4 speed down
#define NUM_LEDS 8

int option = 4;
int earCnt = 1;
long earTimer = 0;
boolean flag = true;


SoftwareSerial bluetooth = SoftwareSerial(2,3);
CRGB earLeds[NUM_LEDS];
CRGBPalette16 currentPalette = PartyColors_p;
TBlendType    currentBlending = LINEARBLEND;

// First time setup
void setup() 
{
  // Shift register
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  digitalWrite(latchPin, HIGH);
  bluetooth.begin(9600);

  
  FastLED.addLeds<WS2811, 5, GRB>(earLeds, NUM_LEDS).setCorrection( TypicalLEDStrip ); 
  FastLED.addLeds<WS2811, 7, GRB>(earLeds, NUM_LEDS).setCorrection( TypicalLEDStrip );

  #if DEBUG
  Serial.begin(9600);
  #endif
}

// For counter option
int cnt = 1000; 

// For marquee option
int steps = 0;
byte temp[5];
byte data[8][5];

char msg[256] = "HUMAN      "; // HELLO WORLD
char txt[6]   = "HUMAN";
int msgLen = 0;
int setDelay = 50;
static uint8_t colorIndex = 0;

void loop()
{
  
    colorIndex = colorIndex + 1; /* motion speed */
  // Check serial for new request
  if(bluetooth.available() > 0)
  {
    option = bluetooth.parseInt();
    setDelay = bluetooth.parseInt();
    
    #if DEBUG
    Serial.println(option);
    Serial.println(setDelay);
    #endif
    
    if(option == 1 || option == 7)
    {
      flag = true;
      delay(500);

        String message = bluetooth.readString();
        
        if(option == 1)
          message += "      ";
        
        message.toCharArray(msg, 256);
        msgLen = message.length();
        
        if(option == 7 && msgLen <= 6)
        {
          for(int i = 0; i < 6; i++)
            txt[i] = ' ';
          for(int i = (6 - msgLen)/2; i < msgLen + (6 - msgLen)/2; i++)
            txt[i] = msg[i - (6 - msgLen)/2];
        }

        if(message.length() == 0)
        {
          all(true, 250);
          all(false, 250);
          all(true, 250);
          option = 9;
        }
    }
  }

  switch(option){
    case 0:
      countDown(cnt);
      cnt--;
      if(cnt % 100 > 60)
        cnt = cnt - (cnt % 100) + 59;
      break;
    case 1:
      marquee(msg);
      break;
    case 2:
      heartBeat();
      break;
    case 3:
      heartBlink();
      break;
    case 4:
      roboCop(15);
      break;
    case 5:
      noise();
      break;
    case 6:
      eyes();
      break;
    case 7:
      text(txt, 50);
      break;
    case 8:
      all(true, 50);
      break;
    case 10:
      pacman();
      break;
    case 11:
      cyclops();
      break;
    case 12:
      daftPunk();
      break;
    case 9:
    default:
      all(false, 50);
      break;
  }
}


void daftPunk()
{
    text(" DAFT ", 750);
    all(false, 250);
    text(" PUNK ", 750);
    all(false, 250);
}

void countDown(int cnt)
{
  for(int i = 0; i < 8; i++)
    for(int j = 0; j < 5; j++)
      data[i][j] = 0x00;

  // Wrap over counter
  if(cnt > 9999)
    cnt = 0;  

  // Grab the digits
  int digits[4]; // Digits in time 01:23
  digits[0] = cnt / 1000;
  digits[1] = (cnt / 100) % 10;
  digits[2] = (cnt / 10) % 10;
  digits[3] = cnt % 10;

  // Go through the rows
  for(int i = 0; i < 8; i++)
  {
  temp[0] = 0;
      
  temp[0] = numbers[digits[0]][i] >> 1;
  data[i][0] = temp[0];
      
  temp[0] = 0;
  temp[0] = numbers[digits[0]][i] << 7;
  temp[0] |= numbers[digits[1]][i] >> 2;
  data[i][1] = temp[0];
      
  temp[0] = 0;
  temp[0] = numbers[digits[1]][i] << 6;
  temp[0] |= colon[i];
  temp[0] |= numbers[digits[2]][i] >> 6;
  data[i][2] = temp[0];
      
  temp[0] = 0;
  temp[0] = numbers[digits[2]][i] << 2;
  temp[0] |= numbers[digits[3]][i] >> 7;
  data[i][3] = temp[0];
      
  temp[0] = 0;
  temp[0] = numbers[digits[3]][i] << 1;
  data[i][4] = temp[0];
      
      
  }
  sendScreen(data, 50);
}


void marquee(char msg[])
{
  if(flag)
  {
    flag = false;
    steps = 0;
    for(int i = 0; i < 8; i++)
      for(int j = 0; j < 5; j++)
        data[i][j] = 0x00;
    sendScreen(data, 50);
  }
  
  for(int i = 0; i < 8; i++){
    byte ch = 0;
    if(steps % 6 != 5){  
      int pos = 4 - (steps % 6);
      if(msg[(steps / 6) % msgLen] != ' '){
        ch = alphabet[msg[(steps / 6) % msgLen] - 'A'][i];
        ch >>= pos;
        ch &= 0x01;
      }else{
        ch = 0;
      }
    }
      
    temp[0] = data[i][0];
    temp[1] = data[i][1];
    temp[2] = data[i][2];
    temp[3] = data[i][3];
    temp[4] = data[i][4];
      
    data[i][0] <<= 1;
    data[i][1] <<= 1;
    data[i][2] <<= 1;
    data[i][3] <<= 1;
    data[i][4] <<= 1;
      
    data[i][4] |= ch;
    temp[4] >>= 7;
    data[i][3] |= temp[4];
    temp[3] >>= 7;
    data[i][2] |= temp[3];
    temp[2] >>= 7;
    data[i][1] |= temp[2];
    temp[1] >>= 7;
    data[i][0] |= temp[1];
      
  } 
  sendScreen(data, 150);
  steps++;  
}

void roboCop(int timerDelay)
{
  
  uint8_t robo[5] = {0xF0, 0x00, 0x00, 0x00, 0x00};
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
      data[j][0] = robo[0];
      data[j][1] = robo[1];
      data[j][2] = robo[2];
      data[j][3] = robo[3];
      data[j][4] = robo[4];
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
      data[j][0] = robo[0];
      data[j][1] = robo[1];
      data[j][2] = robo[2];
      data[j][3] = robo[3];
      data[j][4] = robo[4];
    }
    sendScreen(data, timerDelay);
  }
}

void heartBeat()
{
  uint8_t mask[5] = {0x00, 0x00, 0x00, 0x00, 0x00};
  for(int j = 0; j < 40; j++)
  {
    mask[j / 8] |= 1 << 7 - (j % 8);
    for(int i = 0; i < 8; i++)
    {
      data[i][0] = heartbeat[i][0] & mask[0];
      data[i][1] = heartbeat[i][1] & mask[1];
      data[i][2] = heartbeat[i][2] & mask[2];
      data[i][3] = heartbeat[i][3] & mask[3];
      data[i][4] = heartbeat[i][4] & mask[4];
    }
    sendScreen(data, 25);
  }
  
  for(int i = 0; i < 5; i++)
    mask[i] = 0x00;
    
  for(int j = 0; j < 40; j++)
  {
    mask[j / 8] |= 1 << 7 - (j % 8);
    for(int i = 0; i < 8; i++)
    {
      data[i][0] = heartbeat[i][0] & ~mask[0];
      data[i][1] = heartbeat[i][1] & ~mask[1];
      data[i][2] = heartbeat[i][2] & ~mask[2];
      data[i][3] = heartbeat[i][3] & ~mask[3];
      data[i][4] = heartbeat[i][4] & ~mask[4];
    }
    sendScreen(data, 25);
  }
}

void heartBlink()
{
  cnt++;
  if(cnt % 2 == 0)
  {
    for(int i = 0; i < 8; i++)
    {
      data[i][0]  = heartEmpty[i][0];
      data[i][1]  = heartEmpty[i][1];
      data[i][1] |= heartEmpty[i][0] >> 7;
      data[i][2]  = heartEmpty[i][0] << 1;
      data[i][2] |= heartEmpty[i][1] >> 7;
      data[i][3]  = heartEmpty[i][1] << 1;
      data[i][3] |= heartEmpty[i][0] >> 6;
      data[i][4]  = heartEmpty[i][0] << 2;
      data[i][4] |= heartEmpty[i][1] >> 6;
    }
  }
  else
  {
    for(int i = 0; i < 8; i++)
    {
      data[i][0]  = heartFull[i][0];
      data[i][1]  = heartFull[i][1];
      data[i][1] |= heartFull[i][0] >> 7;
      data[i][2]  = heartFull[i][0] << 1;
      data[i][2] |= heartFull[i][1] >> 7;
      data[i][3]  = heartFull[i][1] << 1;
      data[i][3] |= heartFull[i][0] >> 6;
      data[i][4]  = heartFull[i][0] << 2;
      data[i][4] |= heartFull[i][1] >> 6;
    }
  }
  sendScreen(data, 500);
}

void noise()
{
  for(int i = 0; i < 8; i++)
    for(int j = 0; j < 5; j++)
      data[i][j] = random(256);
  sendScreen(data, 50);
}

void eyes()
{
  if(random(15) == 0)
  {
    for(int k = 0; k < 4; k++)
    {
      for(int i = 0; i < 8; i++)
      {
        data[i][0] = 0x00;
        data[i][1] = blinking[k][i];
        data[i][2] = 0x00;
        data[i][3] = blinking[0][i];
        data[i][4] = 0x00;
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
        data[i][0] = 0x00;
        data[i][1] = blinking[k][i];
        data[i][2] = 0x00;
        data[i][3] = blinking[0][i];
        data[i][4] = 0x00;
      }
      sendScreen(data, 100);
    }
  }
  else
  {
    for(int k = 0; k < 4; k++)
    {
      for(int i = 0; i < 8; i++)
      {
        data[i][0] = 0x00;
        data[i][1] = blinking[k][i];
        data[i][2] = 0x00;
        data[i][3] = blinking[k][i];
        data[i][4] = 0x00;
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
        data[i][0] = 0x00;
        data[i][1] = blinking[k][i];
        data[i][2] = 0x00;
        data[i][3] = blinking[k][i];
        data[i][4] = 0x00;
      }
      sendScreen(data, 100);
    }
  }
  

}

void text(char txt[6], int delay)
{
  if(msgLen % 2 == 0){
    for(int i = 0; i < 8; i++)
    {
      if(txt[0] != ' ')
      {
        data[i][0]  = alphabet[txt[0] - 'A'][i];
      }
      else
      {
        data[i][0]  = 0x00;
      }
      if(txt[1] != ' ')
      {
        data[i][1]  = alphabet[txt[1] - 'A'][i] << 2;
      }
      else
      {
        data[i][1]  = 0x00;
      }
      if(txt[2] != ' ')
      {
        data[i][1] |= alphabet[txt[2] - 'A'][i] >> 4;
        data[i][2]  = alphabet[txt[2] - 'A'][i] << 4;
      }
      else
      {
        data[i][1] &= ~0x03;
        data[i][2]  =  0x00; 
      }
      if(txt[3] != ' ')
      {
        data[i][2] |= alphabet[txt[3] - 'A'][i] >> 2;
        data[i][3]  = alphabet[txt[3] - 'A'][i] << 6;
      }
      else
      {
        data[i][2] &= ~0x0F;
        data[i][3]  =  0x00;
      }
      if(txt[4] != ' ')
      {
        data[i][3] |= alphabet[txt[4] - 'A'][i];
      }
      else
      {
        data[i][3] &= ~0x3F;
      }
      if(txt[5] != ' ')
      {
        data[i][4]  = alphabet[txt[5] - 'A'][i] << 2;
      }
      else
      {
        data[i][4]  = 0x00;
      }
    }
  }
  else{
    for(int i = 0; i < 8; i++)
    {
      if(txt[0] != ' ')
      {
        data[i][0] = alphabet[txt[0] - 'A'][i] >> 3;
        data[i][1] = alphabet[txt[0] - 'A'][i] << 5;
      }
      else
      {
        data[i][0] = 0x00;
        data[i][1] = 0x00;
      }
      if(txt[1] != ' ')
      {
        data[i][1] |= alphabet[txt[1] - 'A'][i] >> 1;
        data[i][2]  = alphabet[txt[1] - 'A'][i] << 7;
      }
      else
      {
        data[i][1] &= ~0x1F;
        data[i][2]  =  0x00;
      }
      if(txt[2] != ' ')
      {
        data[i][2] |= alphabet[txt[2] - 'A'][i] << 1;
      }
      else
      {
        data[i][2] &= ~0x7E;
      }
      if(txt[3] != ' ')
      {
        data[i][2] |= alphabet[txt[3] - 'A'][i] >> 5;
        data[i][3]  = alphabet[txt[3] - 'A'][i] << 3;
      }
      else
      {
        data[i][2] &= ~0x01;
        data[i][3]  =  0x00;
      }
      if(txt[4] != ' ')
      {
        data[i][3] |= alphabet[txt[4] - 'A'][i] >> 3;
        data[i][4]  = alphabet[txt[4] - 'A'][i] << 5;
      }
      else
      {
        data[i][3] &= ~0x07;
        data[i][4]  = 0x00;
      }
    }
  }
  
  sendScreen(data, delay);
}

void all(boolean x, uint8_t delay)
{
  for(int i = 0; i < 8; i++)
  {
    for(int j = 0; j < 5; j++)
    {
      if(x)
        data[i][j] = 0xFF;
      else
        data[i][j] = 0x00;
    }
  }
  sendScreen(data, delay);
}

void pacman()
{
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
      if(i < 8)
        data[j][0] = pacMan[k][j] << 7 - (i % 8);
      else if(i < 16)
        data[j][0] = pacMan[k][j] >> i % 8;
      else
        data[j][0] = 0x00;

      if(i >= 8 && i < 16)
        data[j][1] = pacMan[k][j] << 7 - (i % 8);
      else if(i >= 16 && i < 24)
        data[j][1] = pacMan[k][j] >> i % 8;
      else
        data[j][1] = 0x00;

      if(i >=16 && i < 24)
        data[j][2] = pacMan[k][j] << 7 - (i % 8);
      else if(i >= 24 && i < 32)
        data[j][2] = pacMan[k][j] >> i % 8;
      else
        data[j][2] = 0x00;

      if(i >= 24 && i < 32)
        data[j][3] = pacMan[k][j] << 7 - (i % 8);
      else if(i >= 32 && i < 40)
        data[j][3] = pacMan[k][j] >> i % 8;
      else
        data[j][3] = 0x00;

      if(i >= 32 && i < 40)
        data[j][4] = pacMan[k][j] << 7 - (i % 8);
      else if(i >= 40)
        data[j][4] = pacMan[k][j] >> i % 8;
      else
        data[j][4] = 0x00;

      if(j == 3 || j == 4)
        for(int dot = 0; dot < 5; dot++)
          if(dot >= (i / 8) || dot < (i / 8) - 1)
            data[j][dot] |= 0x18;

    }
    sendScreen(data, 50);
  }
}

void cyclops()
{
  for(int i = 0; i < 8; i++)
  {
    for(int j = 0; j < 5; j++)
    {
      if(i == 3 | i == 4)
        data[i][j] = 0xFF;
      else
        data[i][j] = 0x00;
    }
  }
  sendScreen(data, 50);
}

void sendScreen(uint8_t data[8][5], int timerDelay)
{
  long timer = millis();
  #if DEBUG
  Serial.print(timerDelay);
  Serial.print("\t");
  #endif
  if(setDelay <  50)
  {
    timerDelay = map(setDelay, 0, 49, timerDelay*SPEED_UP, timerDelay);
  }
  else
  {
    timerDelay = map(setDelay, 50, 100, timerDelay, timerDelay/SPEED_UP);
    if(timerDelay < 5)
      timerDelay = 5;
  }
  
  #if DEBUG
  Serial.print(setDelay);
  Serial.print("\t");
  Serial.println(timerDelay);
  #endif
  
  while(millis() < timer + timerDelay)
  {
    
    for(int i = 0; i < 8; i++)
    {
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, LSBFIRST, ~data[i][4]);
      shiftOut(dataPin, clockPin, LSBFIRST, ~data[i][3]); 
      shiftOut(dataPin, clockPin, LSBFIRST, ~data[i][2]); 
      shiftOut(dataPin, clockPin, LSBFIRST, ~data[i][1]); 
      shiftOut(dataPin, clockPin, LSBFIRST, ~data[i][0]); 
      shiftOut(dataPin, clockPin, MSBFIRST, 1 << i);  
      digitalWrite(latchPin, HIGH);
    
      if(millis() > earTimer + 10)
      {
        earTimer = millis();
        earLeds[i] = ColorFromPalette( currentPalette, colorIndex, 255, currentBlending);
        colorIndex += 3;
        FastLED.show();
      }
    }
  }  
}

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
    uint8_t brightness = 255;
    for( int i = 0; i < NUM_LEDS; i++) {
        
    }
}

