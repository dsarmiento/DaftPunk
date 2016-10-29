#include "FastLED.h"
#define NUM_LEDS 64
CRGB leds[NUM_LEDS];
#define DATAPIN 5
#define SEGLEN 4

byte RAINBOW[8][3] = {
  {255, 0, 0},      /*Red*/
  {255, 127, 0},    /*Orange*/
  {255, 255, 0},    /*Yellow*/
  {0, 255, 0},      /*Green*/
  {0, 255, 255},    /*Cyan*/
  {0, 0, 255},      /*Blue*/
  {138, 43, 226},     /*Purple*/
  {128, 0, 125}     /*Violet*/
};

void setup()
{
  FastLED.addLeds<WS2811, DATAPIN, GRB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  Serial.begin(9600);
}

// *** REPLACE FROM HERE ***
/*Bottom to top:
   RED      0xFF0000 or CRGB::Crimson
   ORANGE   0xFF7F00
   YELLOW   0xFFFF00
   GREEN    0x00FF00
   CYAN     0x00FFFF
   BLUE     0x0000FF
   PURPLE   0x7F00FF
   VIOLET   0xC78BFF

   List of what I want:
   1. Stacking LEDs from top to bottom
   2. Rotating wheel of 1 or 2 colors CCW or CW
   3. Color wheel of 8 of the same colors following each other rotating
   4. 8 of the same color are replaced/stacked over 1 color at a time
   5. 8 of the same color with a wheel of 3 colors of the same color rotating
   6. 1 bar bouncing back and forth, when it hits the bottom or top it changes color
*/
void loop() {
  //Left led values goes from 0-31 (Top to Bot)
  //Right led values goes from 63-32 (Top to Bot)
    RainbowDrop(100, 200);
    RainbowWheel(100, 200);
    RainbowStack(100, 500);
    RainbowStackOver(100, 500);
    RainbowFlash(100, 200);
    RainbowRandom(100, 200, false);
    RainbowRandomSegment(100, 50, true);
  RainbowRandomSpeedUp( 100, 200);
CylonBounce(0xff, 0, 0, 4, 10, 50);
rainbowCycle(20);
 
}



void rainbowCycle(int SpeedDelay) {
  byte *c;
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< NUM_LEDS; i++) {
      c=Wheel(((i * 256 / NUM_LEDS) + j) & 255);
      setPixel(i, *c, *(c+1), *(c+2));
    }
    showStrip();
    delay(SpeedDelay);
  }
}

byte * Wheel(byte WheelPos) {
  static byte c[3];
  
  if(WheelPos < 85) {
   c[0]=WheelPos * 3;
   c[1]=255 - WheelPos * 3;
   c[2]=0;
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   c[0]=255 - WheelPos * 3;
   c[1]=0;
   c[2]=WheelPos * 3;
  } else {
   WheelPos -= 170;
   c[0]=0;
   c[1]=WheelPos * 3;
   c[2]=255 - WheelPos * 3;
  }

  return c;
}
void CylonBounce(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay){

  for(int i = 0; i < NUM_LEDS-EyeSize-2; i++) {
    setAll(0,0,0);
    setPixel(i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      setPixel(i+j, red, green, blue); 
    }
    setPixel(i+EyeSize+1, red/10, green/10, blue/10);
    showStrip();
    delay(SpeedDelay);
  }

  delay(ReturnDelay);

  for(int i = NUM_LEDS-EyeSize-2; i > 0; i--) {
    setAll(0,0,0);
    setPixel(i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      setPixel(i+j, red, green, blue); 
    }
    setPixel(i+EyeSize+1, red/10, green/10, blue/10);
    showStrip();
    delay(SpeedDelay);
  }
  
  delay(ReturnDelay);
}
void RainbowRandomSpeedUp(int SpeedDelay, int ReturnDelay)
{
  int k = 200;
  int color = random(8);
  while (k > 2)
  {
    setAll(RAINBOW[color][0], RAINBOW[color][1], RAINBOW[color][2]);
    delay(k);
    k -= k / 2;
  }

}

void RainbowRandomSegment( int SpeedDelay, int ReturnDelay, bool Solid) {
  //setAll(0, 0, 0);
  int color = 0;
  int i = random(8) * 4;
  //  for (int i = 0; i < 4*randnum; i += 4)
  //  {
  if (Solid)
    color = random(8);
  for (int j = 0; j < SEGLEN; j++)
  {
    if (!Solid)
      color = random(8);
    setPixel(i + j, RAINBOW[color][0], RAINBOW[color][1], RAINBOW[color][2]);
    setPixel(NUM_LEDS - i - j - 1, RAINBOW[color][0], RAINBOW[color][1], RAINBOW[color][2]);
  }
  showStrip();
  delay(ReturnDelay);
  //setAll(0,0,0);
  //}

}

void RainbowRandom( int SpeedDelay, int ReturnDelay, bool Solid) {
  setAll(0, 0, 0);
  int color = 0;
  for (int i = 0; i < NUM_LEDS / 2; i += 4) {
    if (Solid)
      color = random(8);
    for (int j = 0; j < SEGLEN; j++) {
      if (!Solid)
        color = random(8);
      setPixel(i + j, RAINBOW[color][0], RAINBOW[color][1], RAINBOW[color][2]);
      setPixel(NUM_LEDS - i - j - 1, RAINBOW[color][0], RAINBOW[color][1], RAINBOW[color][2]);
    }
  }
  showStrip();
  delay(ReturnDelay);
}

void RainbowDrop( int SpeedDelay, int ReturnDelay) {
  setAll(0, 0, 0);
  for (int i = 0; i < NUM_LEDS / 2; i += 4) {
    for (int j = 0; j < SEGLEN; j++) {
      //Set left, starts 0-3 then 4-7...
      setPixel(i + j, RAINBOW[7 - (i / 4) % 8][0], RAINBOW[7 - (i / 4) % 8][1], RAINBOW[7 - (i / 4) % 8][2]);
      //Set right (NUM_LEDS-i-j-1) starts at 63
      setPixel(NUM_LEDS - i - j - 1, RAINBOW[7 - (i / 4) % 8][0], RAINBOW[7 - (i / 4) % 8][1], RAINBOW[7 - (i / 4) % 8][2]);
    }

    //Turn on
    showStrip();
    delay(SpeedDelay);
  }

  for (int i = (NUM_LEDS / 2); i > 0; i -= 4) {
    //left goes from 0 -> 31
    //right goes from 32 -> 63
    for (int j = 0; j < 4; j++) {
      setPixel(i - j - 1, 0, 0, 0);
      setPixel(NUM_LEDS - i + j, 0, 0, 0);
    }
    showStrip();
    delay(SpeedDelay);
  }
  setAll(0, 0, 0);
  delay(ReturnDelay);
}

void RainbowStack(int SpeedDelay, int ReturnDelay)
{
  int f = NUM_LEDS / 2;
  for (int k = 0; k < 8; k++)
  {
    for ( int i = 0; i < f; i += 4)
    {
      for (int j = 0; j < 4; j++)
      {
        setPixel(i + j, RAINBOW[k][0], RAINBOW[k][1], RAINBOW[k][2]);
        setPixel(NUM_LEDS - i - j - 1, RAINBOW[k][0], RAINBOW[k][1], RAINBOW[k][2]);
      }
      showStrip();
      delay(SpeedDelay);

      if (i + 4 < f)
      {
        for (int j = 0; j < 4; j++)
        {
          setPixel(i + j, 0, 0, 0);
          setPixel(NUM_LEDS - i - j - 1, 0, 0, 0);
        }
      }
    }
    SpeedDelay -= (SpeedDelay / 8);
    f -= 4;
  }
  delay(ReturnDelay);
  setAll(0, 0, 0);
  delay(ReturnDelay);
}

void RainbowStackOver(int SpeedDelay, int ReturnDelay)
{
  int f = NUM_LEDS / 2;
  for (int k = 0; k < 8; k++)
  {
    for ( int i = 0; i < f; i += 4)
    {
      for (int j = 0; j < 4; j++)
      {
        setPixel(i + j, RAINBOW[k][0], RAINBOW[k][1], RAINBOW[k][2]);
        setPixel(NUM_LEDS - i - j - 1, RAINBOW[k][0], RAINBOW[k][1], RAINBOW[k][2]);
      }
      showStrip();
      delay(SpeedDelay);
    }
    SpeedDelay -= (SpeedDelay / 8);
    f -= 4;
  }
  delay(ReturnDelay);
  setAll(0, 0, 0);
  delay(ReturnDelay);
}

void RainbowFlash(int SpeedDelay, int ReturnDelay)
{
  for (int i = 0; i < 8; i++)
  {
    setAll( RAINBOW[i][0], RAINBOW[i][1], RAINBOW[i][200]);
    delay(SpeedDelay);
  }
}

void RainbowWheel(int SpeedDelay, int ReturnDelay) {
  for (int k = 0; k < 8; k++)
  {
    for (int i = 0; i < NUM_LEDS / 2; i += 4)
    {
      for (int j = 0; j < SEGLEN; j++)
      {
        //Set left, starts 0-3 then 4-7...
        setPixel(i + j, RAINBOW[k][0], RAINBOW[k][1], RAINBOW[k][2]);
        //Set right (NUM_LEDS-i-j-1) starts at 63
        setPixel(NUM_LEDS - i - j - 1, RAINBOW[k][0], RAINBOW[k][1], RAINBOW[k][2]);
      }
      //Turn on
      showStrip();
      delay(SpeedDelay);
    }
  }
}
// *** REPLACE TO HERE ***

void showStrip() {
#ifdef ADAFRUIT_NEOPIXEL_H
  // NeoPixel
  strip.show();
#endif
#ifndef ADAFRUIT_NEOPIXEL_H
  // FastLED
  FastLED.show();
#endif
}

void setPixel(int Pixel, byte red, byte green, byte blue) {
#ifdef ADAFRUIT_NEOPIXEL_H
  // NeoPixel
  strip.setPixelColor(Pixel, strip.Color(red, green, blue));
#endif
#ifndef ADAFRUIT_NEOPIXEL_H
  // FastLED
  leds[Pixel].r = red;
  leds[Pixel].g = green;
  leds[Pixel].b = blue;
#endif
}

void setAll(byte red, byte green, byte blue) {
  for (int i = 0; i < NUM_LEDS; i++ ) {
    setPixel(i, red, green, blue);
  }
  showStrip();
}
