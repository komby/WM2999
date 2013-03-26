// LED Pixel Driver for Holiday Time 20ct "color Changing" lights
// Written By:  Greg Scull (komby) 1/2013
// All Use subject to license terms.

#include <WM2999.h>
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

#define OUTPUT_PIN 8
WM2999 strip =  WM2999(OUTPUT_PIN);


void setup() {
  //strip   set to use Arduino digital 8 pin
  pinMode(OUTPUT_PIN, OUTPUT);
 
 //initialize to 20 pixels;
  strip.setPixelCount(20);
}

void loop() {

    int i, j;
	//shut em all off so we can start with a clean slate;
	for ( i=0;i<strip.getPixelCount();i++){
		strip.setPixelColor(i, 0,0,0);
	}

	strip.paint();
	delay(2000);

	//now lets turn them back on

  //lets do some chasing exercises for warmup    
  colorWipe(color(255, 0, 0), 50); //red   
  colorWipe(color(0, 255, 0), 50); //green
  colorWipe(color(0, 0, 255), 30); //blue

  //chase is iterating backwards from the colorwipe 
  chase(color(255, 255, 255), 30); //white
  chase(color(255, 90, 5), 10); 
  chase(color(20, 255, 5), 5); 
  chase(color(128, 123, 238), 5); 
  chase(color(128, 128, 128), 5); 
  chase(color(128, 0, 0), 5); 
  chase(color(255, 255, 5), 5); 
  
  //now lets get crazy and do both

  colorWipe(color(255, 5,34), 5); 
  chase(color(99, 90, 5), 1);   
  colorWipe(color(0, 2, 0), 5); 
  chase(color(3, 55, 5), 1); 
  colorWipe(color(0, 0, 33), 6); 
  chase(color(255, 90, 5), 10); 
   colorWipe(color(255, 0, 0), 5); 
  chase(color(255, 90, 5), 9);   
  colorWipe(color(0, 255, 0), 5); 
  chase(color(255, 90, 5), 1); 
  colorWipe(color(0, 0, 255), 3); 
  chase(color(255, 90, 5), 1); 
   colorWipe(color(255, 0, 3), 5); 
  chase(color(255, 90, 224), 1);   
  colorWipe(color(0, 255, 0), 5); 
  chase(color(255, 90, 0), 1); 
  colorWipe(color(0, 0, 255), 3); 
  chase(color(255, 90, 5), 1); 
   colorWipe(color(255, 45, 7), 5); 
  chase(color(255, 90, 5), 1);   
  colorWipe(color(0, 255, 0), 5); 
  chase(color(255, 90, 5), 1); 
  colorWipe(color(0, 0, 255), 3); 
  chase(color(255, 90, 5), 1); 
  

  //lets look at some pretty colors for a little bit
   rainbow(10);
   rainbowCycle(10);


}


//This method is from the adafruit example program,  its pretty 
//try not to touch any live wires while being mesmorized by the pretty colors
// Slightly different, this one makes the rainbow wheel equally distributed 
// along the chain
void rainbowCycle(uint8_t wait) {
  int i, j;
  for (j=0; j < 256 * 5; j++) {     // 5 cycles of all 25 colors in the wheel
    for (i=0; i < strip.getPixelCount(); i++) {
      // tricky math! we use each pixel as a fraction of the full 96-color wheel
      // (thats the i / strip.numPixels() part)
      // Then add in j which makes the colors go around per pixel
      // the % 96 is to make the wheel cycle around
      strip.setPixelColor(i, Wheel( ((i * 256 / strip.getPixelCount()) + j) % 256) );
    }  
    strip.paint();   // write all the pixels out
    delay(wait);
  }
}


// fill the dots one after the other with said color
// should chase from the controller box to the end of the string.
// good for testing purposes
void chase(uint32_t c, uint8_t wait) {
  int i;
   for (i=19; i >= 0; i--) {
    strip.setPixelColor(i, c);
	//paintand delay while in the loop to get the effect of chase
	strip.paint();
	//use a slow setting if you are getting dizy
	delay(wait);
  }
  
}

// this will "chase" from the very "last" pixel in the string
// back to the controller.
// fill the dots one after the other with said color
// good for testing purposes
void colorWipe(uint32_t c, uint8_t wait) {
  int i;
  // for (i=20; i >= 0; i--) {
  for (i=0; i < strip.getPixelCount(); i++) {
    strip.setPixelColor(i, c);

	strip.paint();
	delay(wait);
  }
}

/* Helper functions */

// Create a 24 bit color value from R,G,B
uint32_t color(byte r, byte g, byte b)
{
  //Note,  we are storing these as good old RGB NOT the pixel order BGR
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
    return color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } 
  else if (WheelPos < 170) {
    WheelPos -= 85;
    return color(255 - WheelPos * 3, 0, WheelPos * 3);
  } 
  else {
    WheelPos -= 170; 
    return color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}


void rainbow(uint8_t wait) {
  int i, j;

  for (j=0; j < 256; j++) {     // 3 cycles of all 256 colors in the wheel
    for (i=0; i < strip.getPixelCount(); i++) {
      strip.setPixelColor(i, Wheel( (i + j) % 255));
    }  
    strip.paint();   // write all the pixels out
    delay(20);
  }
}

