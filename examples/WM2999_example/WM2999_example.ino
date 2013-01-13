/*
 * Copyright 2012 Alan Burlison, alan@bleaklow.com.  All rights reserved.
 * Use is subject to license terms.
 *
 * Demo of the WS2811 driver, driving 3 pixels.
 */

#include <WM2999.h>


WM2999<8> strip =  WM2999<8>(20);


void setup() {
        strip.SetPixelCount(20);
        
        strip.start();
        
        strip.Paint();
}


void loop() {

 
 colorWipe(Color(255, 0, 0), 100);
  colorWipe(Color(0, 255, 0), 150);
  colorWipe(Color(0, 0, 255), 200);
  rainbow(50);
  rainbowCycle(200);
   
       }



// Slightly different, this one makes the rainbow wheel equally distributed 
// along the chain
void rainbowCycle(uint8_t wait) {
  int i, j;
  
  for (j=0; j < 256 * 5; j++) {     // 5 cycles of all 25 colors in the wheel
    for (i=0; i < strip.GetPixelCount(); i++) {
      // tricky math! we use each pixel as a fraction of the full 96-color wheel
      // (thats the i / strip.numPixels() part)
      // Then add in j which makes the colors go around per pixel
      // the % 96 is to make the wheel cycle around
      strip.SetPixelColor(i, Wheel( ((i * 256 / strip.GetPixelCount()) + j) % 256) );
    }  
    strip.Paint();   // write all the pixels out
    delay(wait);
  }
}

// fill the dots one after the other with said color
// good for testing purposes
void colorWipe(uint32_t c, uint8_t wait) {
  int i;
  
  for (i=0; i < strip.GetPixelCount(); i++) {
      strip.SetPixelColor(i, c);
      strip.Paint();
      delay(wait);
  }
}

/* Helper functions */

// Create a 24 bit color value from R,G,B
uint32_t Color(byte r, byte g, byte b)
{
  uint32_t c;
  c = r;
  c <<= 8;
  c |= g;
  c <<= 8;
  c |= b;
  return c;
}

//Input a value 0 to 255 to get a color value.
//The colours are a transition r - g -b - back to r
uint32_t Wheel(byte WheelPos)
{
  if (WheelPos < 85) {
   return Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
   WheelPos -= 85;
   return Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170; 
   return Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}


void rainbow(uint8_t wait) {
  int i, j;
   
  for (j=0; j < 256; j++) {     // 3 cycles of all 256 colors in the wheel
    for (i=0; i < strip.GetPixelCount(); i++) {
      strip.SetPixelColor(i, Wheel( (i + j) % 255));
    }  
    strip.Paint();   // write all the pixels out
    delay(wait);
  }
}
