// LED Pixel Driver for Holiday Time 20ct "Color Changing" lights
// Written By:  Greg Scull (komby) 1/2013
// All Use subject to license terms.
//
// the implementation of this method is a combination of others work.
//
// the bit banging was a combination of the work from Alan B and Pololu's drivers for different pixel strings.
// Alan Burlison : http://sourceforge.net/p/abavr/lib
// Pololu : https://github.com/pololu/pololu-led-strip-arduino
//
// The Class/CPP ideals as well as some other ideas were taken from the AdaFruit and their work for the WS2801 pixel driver.
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
// This has sure been fun :)
//


#include "WM2999.h"



 template<unsigned char pin>WM2999<pin>::WM2999(void)
{
}

template<unsigned char pin> WM2999<pin>::WM2999(uint16_t pNumPixels )
{
	alloc(pNumPixels);
}


//cleanup from the alloc calls - thanks to adafruit driver for idea
 template<unsigned char pin>WM2999<pin>::~WM2999(void)
{
	 if (pixels != NULL) {
		 free(pixels);
	}

}


//example for pre-allocation of pixel space from ADAFRUIT WS2801 example
template<unsigned char pin> void WM2999<pin>::alloc(uint16_t n) {
  numberOfPixels = ((pixels = (uint8_t *)calloc(n, 3)) != NULL) ? n : 0;
}


// setup the pins
template<unsigned char pin> void WM2999<pin>::start() {
    pinMode(pin, OUTPUT);
    pinMode(pin , OUTPUT);
	
  }
  



  /// this method may be helpful ... its from teh adafruit library.  
//currently im storing the pixel info in rgb order,  since these pixels are crazy
//and use BGR  it would be more efficient ( but less sane ) to store them in that order 
//so that we can simply operate on the 24bit string ... this could allow us to not use
//three registers in the assembly operation ( i think )
// 
//  I kept the pixel order as RGB as I was hoping to make the base classes pixel independent - that doesnt really matter though does it... sleep now.
//
//// Set pixel color from separate 8-bit R, G, B components:
//void Adafruit_WS2801::setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b) {
//  if(n < numLEDs) { // Arrays are 0-indexed, thus NOT '<='
//    uint8_t *p = &pixels[n * 3];
//    // See notes later regarding color order
//    if(rgb_order == WS2801_RGB) {
//      *p++ = r;
//      *p++ = g;
//    } else {
//      *p++ = g;
//      *p++ = r;
//    }
//    *p++ = b;
//  }
//}
	template<unsigned char pin> void WM2999<pin>::Paint()
{

	this.Paint(this.pixels, this.numberOfPixels);	
}
// Function definition for the paint operation.  This is a templated class so the pin is dynamic to the instance.
// note that since this is using registers directly  r18,r19,r20 will definately get clobbered,  I am unsure how
// the use of multiple instances of this class will work if used at the same time.
// the implementation of this method is a combination of others work.
// Alan Burlison : http://sourceforge.net/p/abavr/lib
// AdaFruit and their work for the WS2801 pixel driver : 
// Pololu : https://github.com/pololu/pololu-led-strip-arduino
// And a LOT of help from Andy L. @ DIYC for getting these pixels timing figured out and explaining it to us mortals 
// Also thanks to Zeph for posing the question and others for finding them for 5$ a set :)  Glad i got 20 of them now !
//
	template<unsigned char pin> void WM2999<pin>::Paint(rgb_color * colors, unsigned int count)
{
	//set pin low to start
    digitalWrite(pin, LOW);
    pinMode(pin, OUTPUT);

	//shut off the interrupts so that pulses are predictable
	  asm volatile ("cli  ; global interrupts disable\n" );
	


	while(count--)
    {
		asm volatile( 
	"    ld r18, Z+           ; load in red byte\n" \
	"    ld r19, Z+           ; load in green byte\n" \
	"    ld r20, Z+           ; load in blue byte\n" \
	"    ld __tmp_reg__, r20  ; first output blue\n"   \
	"	 rcall write_byte%=	  ; \n"\
	"    ld __tmp_reg__, r19  ; output green\n"   \
    "    rcall write_byte%=	  ; \n"\
	"    ld __tmp_reg__, r18  ; output red\n"   \
	"	 rcall write_byte%=	  ; \n"\
	"	 rcall string_end%=   ; \n"\
	"write_byte%=:		      ; output the color byte\n"\
	"    rcall write_bit%=    ; \n"\
	"    rcall write_bit%=    ; \n"\
	"    rcall write_bit%=    ; \n"\
	"    rcall write_bit%=    ; \n"\
	"    rcall write_bit%=    ; \n"\
	"    rcall write_bit%=    ; \n"\
	"    rcall write_bit%=    ; \n"\
	"    rcall write_bit%=    ; \n"\
	"    ret                  ; \n"\
	"write_bit%=:             ; \n"\
	"2:  cbi  %[port], %[pin] ; hi -> lo sarting low state delay 2us \n" \
	"    ldi  r20, 9          ;	\n" \
    "3:  dec  r20			  ; \n" \
    "    brne 3b              ; \n"\
    "    rol __tmp_reg__      ; push bit to eval into carry \n"\
	"    brcs 5               ; jump past the stretching of a low state for a 1 bit when it is a 0 \n "\
	"    ldi  r20, 34         ;	delay for 6us when 1\n" \
    "4:  dec  r20	          ; \n" \
    "    brne 4b              ; \n"\
    "    nop			      ; end delay\n"\
	"5:  sbi  %[port], %[pin] ; pin lo -> hi  \n" \
	"    ldi  r20, 45         ;	delay for 9us both 0 & 1\n" \
    "6:  dec  r20			  ; \n" \
    "    brne 6b              ; \n"\
    "    nop			      ; end delay\n"\
	"    ret                  ; \n" \
	"string_end%=:            ; \n" 
	    : "=b" (colors)
        : "0" (colors),         // %a0 points to the next color to display
          "I" (pinAddr[pin]),   // %2 is the port register (e.g. PORTC)
          "I" (pinBit[pin])     // %3 is the pin number (0-8)
		  );
	}
	  asm("sei\nnop\n");          // Re-enable interrupts now that we are done.
     delayMicroseconds(38);  // Hold the line low for 24 microseconds to send the reset signal.
}


//// Set pixel color from separate 8-bit R, G, B components using x,y coordinate system:
//void WM2999<char>::SetPixelColor(uint16_t x, uint16_t y, uint8_t r, uint8_t g, uint8_t b) {
//  boolean evenRow = ((y % 2) == 0);
//  // calculate x offset first
//  uint16_t offset = x % width;
//  if (!evenRow) {
//    offset = (width-1) - offset;
//  }
//  // add y offset
//  offset += y * width;
//  setPixelColor(offset, r, g, b);
//}

// Set pixel color from separate 8-bit R, G, B components:
template<unsigned char pin> void WM2999<pin>::SetPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b) {
  if(n < numberOfPixels) { // Arrays are 0-indexed, thus NOT '<='
    uint8_t *p = &pixels[n * 3];
   
      *p++ = r;
      *p++ = g;
 
    
    *p++ = b;
  }
}

// Set pixel color from 'packed' 32-bit RGB value:
template<unsigned char pin> void WM2999<pin>::SetPixelColor(uint16_t n, uint32_t c) {
  if(n < numberOfPixels) { // Arrays are 0-indexed, thus NOT '<='
    uint8_t *p = &pixels[n * 3];
    // To keep the show() loop as simple & fast as possible, the
    // internal color representation is native to different pixel
    // types.  For compatibility with existing code, 'packed' RGB
    // values passed in or out are always 0xRRGGBB order.
   
      *p++ = c >> 16; // Red
      *p++ = c >>  8; // Green
    *p++ = c;         // Blue
  }
}


// Query color from previously-set pixel (returns packed 32-bit RGB value)
template<unsigned char pin>  uint32_t WM2999<pin>::GetPixelColor(uint16_t n) {
  if(n < this.numberOfPixels) {
    uint16_t ofs = n * 3;
    // To keep the show() loop as simple & fast as possible, the
    // internal color representation is native to different pixel
    // types.  For compatibility with existing code, 'packed' RGB
    // values passed in or out are always 0xRRGGBB order.
    return (true) ?
      ((uint32_t)pixels[ofs] << 16) | ((uint16_t) pixels[ofs + 1] <<  8) | pixels[ofs + 2] :
      (pixels[ofs] <<  8) | ((uint32_t)pixels[ofs + 1] << 16) | pixels[ofs + 2];
  }
}
  //definition of helper method to return pixel count
  template<unsigned char pin> uint32_t WM2999<pin>::GetPixelCount(void) {
	  return this.numberOfPixels;
  
  }
    //definition of helper method to return pixel count
template<unsigned char pin> void  WM2999<pin>::SetPixelCount(  uint32_t pCount) {
	   this.numberOfPixels = pCount;
  
  }