#include "FastLED.h"
#define NUM_LEDS 64  
CRGB leds[NUM_LEDS];
#define DATAPIN 5
#define SEGLEN 4

byte RAINBOW[8][3] = {
  {255,0,0},        /*Red*/
  {255,127,0},      /*Orange*/
  {255,255,0},      /*Yellow*/
  {0,255,0},        /*Green*/
  {0,255,255},      /*Cyan*/
  {0,0,255},        /*Blue*/
  {75,0,130},       /*Purple*/
  {143,0,255}       /*Violet*/
};

void setup()
{
  FastLED.addLeds<WS2811, DATAPIN, GRB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  Serial.begin(9600);
}

// *** REPLACE FROM HERE ***
/*Bottom to top: 
 * RED      0xFF0000 or CRGB::Crimson
 * ORANGE   0xFF7F00
 * YELLOW   0xFFFF00
 * GREEN    0x00FF00
 * CYAN     0x00FFFF
 * BLUE     0x0000FF
 * PURPLE   0x7F00FF
 * VIOLET   0xC78BFF
 * 
 * List of what I want:
 * 1. Stacking LEDs from top to bottom
 * 2. Rotating wheel of 1 or 2 colors CCW or CW
 * 3. Color wheel of 8 of the same colors following each other rotating 
 * 4. 8 of the same color are replaced/stacked over 1 color at a time
 * 5. 8 of the same color with a wheel of 3 colors of the same color rotating
 * 6. 1 bar bouncing back and forth, when it hits the bottom or top it changes color
*/
void loop() {
  //setAll(255,0,0);
  RainbowDrop(200, 500);
}

void RainbowDrop( int SpeedDelay, int ReturnDelay){
  setAll(0,0,0);
  for(int i = 0; i < (NUM_LEDS-SEGLEN+1)/2; i+=4) {
    for(int j = 0; j < SEGLEN; j++) {
      setPixel(i+j, RAINBOW[(i/4)%8][0], RAINBOW[(i/4)%8][1], RAINBOW[(i/4)%8][2]);
      setPixel(NUM_LEDS-i-j-1, RAINBOW[(i/4)%8][0], RAINBOW[(i/4)%8][1], RAINBOW[(i/4)%8][2]);  
    }
    showStrip();
    delay(SpeedDelay);
  }

   for(int i = NUM_LEDS/2; i > 0; i-=4) {
    for(int j = 0; j < SEGLEN; j++) {
      setPixel(i-j, 0, 0, 0); 
      setPixel(NUM_LEDS-i-j-1, 0, 0, 0);
    }
    showStrip();
    delay(SpeedDelay);
  }
  setAll(0,0,0);
  delay(ReturnDelay);
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
  for(int i = 0; i < NUM_LEDS; i++ ) {
    setPixel(i, red, green, blue); 
  }
  showStrip();
}
