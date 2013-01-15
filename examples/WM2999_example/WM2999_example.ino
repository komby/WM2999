// LED Pixel Driver for Holiday Time 20ct "Color Changing" lights
// Written By:  Greg Scull (komby) 1/2013
// All Use subject to license terms.

#include "WM2999.h"
//#include <Arduino.h>

//
//#include <avr/io.h>
//#include <avr/interrupt.h>
//#include <util/delay.h>

//
//the implementation of this sketch  is a combination of  a lot of others work.
// and  atini bit of mine 
//
// the bit banging was a combination of the work from Alan B and Pololu's drivers for different pixel strings.
// Alan Burlison : http://sourceforge.net/p/abavr/lib
// Pololu : https://github.com/pololu/pololu-led-strip-arduino
//
// The Class/c++ ideas as well as some other ideas were taken from the AdaFruit and their work for the WS2801 pixel driver. ( and the pololu template for pin config ).
// https://github.com/adafruit/Adafruit-WS2801-Library
//
// This driver is a bit banging approach to driving the pixels on a Atmega 328pu (uno) chip. 
// I have not tested on anything but an uno on pin 8 .. YMMV
//
// And a LOT of help from Andy L. @ DIYC for getting these pixels timing figured out and explaining it to us mortals 
// Also thanks to Zeph for posing the question and others for finding them for 5$ a set :)  
// You can read about that here and all the others hard work,  Ernie H.,KingofKYa, bigredsoftware .... THANKS EVERYONE! 
// http://doityourselfchristmas.com/forums/showthread.php?24162-Hackable-Cheap-Walmart-quot-Pixels-quot-an-exlectronic-puzzle
// 
//


WM2999<8> strip =  WM2999<8>();


void setup() {
     //strip 
     pinMode(8, OUTPUT);
//digitalWrite(8, LOW);
    
Serial.begin(115200);
        strip.SetPixelCount(20);
        
        strip.Start();
        
       // strip.Paint();
}


void loop() {

 int i;
  
  for (i=0; i < 20; i++) {
      strip.SetPixelColor(i, 0, 0, 255);
      
     
  }
  strip.Paint();
  delay(2000);
   for (i=0; i < 20; i++) {
      strip.SetPixelColor(i, 0, 255, 0);
      
     
  }
  strip.Paint();
  delay(2000); 
     for (i=0; i < 20; i++) {
      strip.SetPixelColor(i, 255, 0, 0);
      
     
  }
  strip.Paint();
  delay(2000); 
//colorWipe(Color(255, 255, 255), 1000); //white
//  colorWipe(Color(0, 0, 255), 100); //red
//   
//   colorWipe(Color(0, 255, 0), 10000); //green
// colorWipe(Color(0, 0, 255), 10000); //blue
// colorWipe(Color(128, 128, 128), 100);
//  rainbow(100);
//  rainbowCycle(200);
 
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
    delay(4000);
  }
}

// fill the dots one after the other with said color
// good for testing purposes
void colorWipe(uint32_t c, uint8_t wait) {
  int i;
  
  for (i=0; i < strip.GetPixelCount(); i++) {
      strip.SetPixelColor(i, c);
      
     
  }
  strip.Paint();
  delay(wait);
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
  Serial.print( "uint32color returned : "); Serial.print(c);
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
    delay(3000);
  }
}
