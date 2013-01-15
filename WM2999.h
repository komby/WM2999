// LED Pixel Driver for Holiday Time 20ct "Color Changing" lights
// Written By:  Greg Scull (komby) 1/2013
// All Use subject to license terms.
//the implementation of this method is a combination of others work.
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
//

#ifndef  WM2999_H
#define  WM2999_H

#include <Arduino.h>


#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#endif


typedef struct __attribute__ ((__packed__)) 
{
	uint8_t  r, g, b;
} rgb_color;


#if defined(__AVR_ATmega32U4__)
// ATmega32U4-based boards such as the Arduino Leonardo

const unsigned char pinBit[] =
{
	2, 3, 1, 0, 4, 6, 7, 6,  // Pins 0-7
	4, 5, 6, 7, 6, 7,        // Pins 8-13
	3, 1, 2, 0,              // Pins 14-17
	7, 6, 5, 4, 1, 0,        // Pins 18-23
	4, 7, 4, 5, 6, 6,        // Pins 24-29
};

const unsigned char pinAddr[] =
{
	_SFR_IO_ADDR(PORTD),
	_SFR_IO_ADDR(PORTD),
	_SFR_IO_ADDR(PORTD),
	_SFR_IO_ADDR(PORTD),
	_SFR_IO_ADDR(PORTD),
	_SFR_IO_ADDR(PORTC),
	_SFR_IO_ADDR(PORTD),
	_SFR_IO_ADDR(PORTE),

	_SFR_IO_ADDR(PORTB),
	_SFR_IO_ADDR(PORTB),
	_SFR_IO_ADDR(PORTB),
	_SFR_IO_ADDR(PORTB),
	_SFR_IO_ADDR(PORTD),
	_SFR_IO_ADDR(PORTC),

	_SFR_IO_ADDR(PORTB),
	_SFR_IO_ADDR(PORTB),
	_SFR_IO_ADDR(PORTB),
	_SFR_IO_ADDR(PORTB),

	_SFR_IO_ADDR(PORTF),
	_SFR_IO_ADDR(PORTF),
	_SFR_IO_ADDR(PORTF),
	_SFR_IO_ADDR(PORTF),
	_SFR_IO_ADDR(PORTF),
	_SFR_IO_ADDR(PORTF),

	_SFR_IO_ADDR(PORTD),
	_SFR_IO_ADDR(PORTD),
	_SFR_IO_ADDR(PORTB),
	_SFR_IO_ADDR(PORTB),
	_SFR_IO_ADDR(PORTB),
	_SFR_IO_ADDR(PORTD),    
};

#elif defined(__AVR__) && !defined(NUM_DIGITAL_PINS) || NUM_DIGITAL_PINS == 20
// ATmega168/328-based boards such as the Arduino Uno or Baby Orangutan B-328

const unsigned char pinBit[] =
{
	0, 1, 2, 3, 4, 5, 6, 7,  // PORTD
	0, 1, 2, 3, 4, 5,        // PORTB
	0, 1, 2, 3, 4, 5, 6,     // PORTC
};

const unsigned char pinAddr[] =
{
	_SFR_IO_ADDR(PORTD),
	_SFR_IO_ADDR(PORTD),
	_SFR_IO_ADDR(PORTD),
	_SFR_IO_ADDR(PORTD),
	_SFR_IO_ADDR(PORTD),
	_SFR_IO_ADDR(PORTD),
	_SFR_IO_ADDR(PORTD),
	_SFR_IO_ADDR(PORTD),
	_SFR_IO_ADDR(PORTB),
	_SFR_IO_ADDR(PORTB),
	_SFR_IO_ADDR(PORTB),
	_SFR_IO_ADDR(PORTB),
	_SFR_IO_ADDR(PORTB),
	_SFR_IO_ADDR(PORTB),
	_SFR_IO_ADDR(PORTC),
	_SFR_IO_ADDR(PORTC),
	_SFR_IO_ADDR(PORTC),
	_SFR_IO_ADDR(PORTC),
	_SFR_IO_ADDR(PORTC),
	_SFR_IO_ADDR(PORTC),
	_SFR_IO_ADDR(PORTC),
};

#elif defined(__AVR__) && NUM_DIGITAL_PINS == 70
// ATmega2560-based boards such as the Arduino Mega 2560

const unsigned char pinBit[] =
{
	0, 1, 4, 5, 5, 3, 3, 4, 5, 6, 
	4, 5, 6, 7, 1, 0, 1, 0, 3, 2, 
	1, 0, 0, 1, 2, 3, 4, 5, 6, 7, 
	7, 6, 5, 4, 3, 2, 1, 0, 7, 2, 
	1, 0, 7, 6, 5, 4, 3, 2, 1, 0, 
	3, 2, 1, 0, 0, 1, 2, 3, 4, 5, 
	6, 7, 0, 1, 2, 3, 4, 5, 6, 7, 
};

const unsigned char pinAddr[] =
{
	_SFR_IO_ADDR(PORTE),
	_SFR_IO_ADDR(PORTE),
	_SFR_IO_ADDR(PORTE),
	_SFR_IO_ADDR(PORTE),
	_SFR_IO_ADDR(PORTG),
	_SFR_IO_ADDR(PORTE),
	_SFR_IO_ADDR(PORTH),
	_SFR_IO_ADDR(PORTH),
	_SFR_IO_ADDR(PORTH),
	_SFR_IO_ADDR(PORTH),
	_SFR_IO_ADDR(PORTB),
	_SFR_IO_ADDR(PORTB),
	_SFR_IO_ADDR(PORTB),
	_SFR_IO_ADDR(PORTB),
	_SFR_IO_ADDR(PORTJ),
	_SFR_IO_ADDR(PORTJ),
	_SFR_IO_ADDR(PORTH),
	_SFR_IO_ADDR(PORTH),
	_SFR_IO_ADDR(PORTD),
	_SFR_IO_ADDR(PORTD),
	_SFR_IO_ADDR(PORTD),
	_SFR_IO_ADDR(PORTD),
	_SFR_IO_ADDR(PORTA),
	_SFR_IO_ADDR(PORTA),
	_SFR_IO_ADDR(PORTA),
	_SFR_IO_ADDR(PORTA),
	_SFR_IO_ADDR(PORTA),
	_SFR_IO_ADDR(PORTA),
	_SFR_IO_ADDR(PORTA),
	_SFR_IO_ADDR(PORTA),
	_SFR_IO_ADDR(PORTC),
	_SFR_IO_ADDR(PORTC),
	_SFR_IO_ADDR(PORTC),
	_SFR_IO_ADDR(PORTC),
	_SFR_IO_ADDR(PORTC),
	_SFR_IO_ADDR(PORTC),
	_SFR_IO_ADDR(PORTC),
	_SFR_IO_ADDR(PORTC),
	_SFR_IO_ADDR(PORTD),
	_SFR_IO_ADDR(PORTG),
	_SFR_IO_ADDR(PORTG),
	_SFR_IO_ADDR(PORTG),
	_SFR_IO_ADDR(PORTL),
	_SFR_IO_ADDR(PORTL),
	_SFR_IO_ADDR(PORTL),
	_SFR_IO_ADDR(PORTL),
	_SFR_IO_ADDR(PORTL),
	_SFR_IO_ADDR(PORTL),
	_SFR_IO_ADDR(PORTL),
	_SFR_IO_ADDR(PORTL),
	_SFR_IO_ADDR(PORTB),
	_SFR_IO_ADDR(PORTB),
	_SFR_IO_ADDR(PORTB),
	_SFR_IO_ADDR(PORTB),
	_SFR_IO_ADDR(PORTF),
	_SFR_IO_ADDR(PORTF),
	_SFR_IO_ADDR(PORTF),
	_SFR_IO_ADDR(PORTF),
	_SFR_IO_ADDR(PORTF),
	_SFR_IO_ADDR(PORTF),
	_SFR_IO_ADDR(PORTF),
	_SFR_IO_ADDR(PORTF),
	_SFR_IO_ADDR(PORTK),
	_SFR_IO_ADDR(PORTK),
	_SFR_IO_ADDR(PORTK),
	_SFR_IO_ADDR(PORTK),
	_SFR_IO_ADDR(PORTK),
	_SFR_IO_ADDR(PORTK),
	_SFR_IO_ADDR(PORTK),
	_SFR_IO_ADDR(PORTK),
};

#endif



//base class for pixels - this is overkill for today but hey its cpp lets inherit something :)
class PixelBase {
public: 
	//	void virtual Paint(rgb_color *, unsigned int count) = 0;
	void virtual Paint(uint8_t * colors , unsigned int count) = 0;
	void virtual Paint(void) = 0;
	uint32_t virtual GetPixelCount( void ) = 0;
	void virtual SetPixelCount( uint32_t ) = 0;
	void virtual Start( void ) = 0;
	uint32_t virtual GetPixelColor(uint16_t n) = 0;
	//n is the index of previous color
	//	uint32_t virtual SetPixelColor(uint16_t n) = 0;
	void virtual SetPixelColor(uint16_t n, uint32_t c)  = 0;
	void virtual SetPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b) = 0;
};



//This template class is intended for the pin to represent the pin on the Arduino 
//hopefully we can use this to have multiple instances of the driver running on one chip.
//i am woefully ignorant of what the implications are regarding the register usage if there are 
//multiples ...  enlighten me ... please
template<unsigned int pin> class WM2999 : public PixelBase
{
public:	

	//cleanup from the alloc calls - thanks to adafruit driver for idea
	~WM2999(void)
	{
		if (pixels != NULL) {
			free(pixels);
		}

	}

	// setup the pins
	void Start() {
		pinMode(pin, OUTPUT);
	}


	/// this method may be helpful ... its from teh adafruit library.  
	//currently im storing the pixel info in rgb order,  since these pixels are crazy
	//and use BGR  it would be more efficient ( but less sane ) to store them in that order 
	//so that we can simply operate on the 24bit string ... this could allow us to not use
	//three registers in the assembly operation ( i think )
	// 
	// Set pixel color from 'packed' 32-bit RGB value:
	void  setPixelColor(uint16_t n, uint32_t c) {

		if(n < numberOfPixels) { // Arrays are 0-indexed, thus NOT '<='
			uint8_t *p = &pixels[n * 3];
			// To keep the show() loop as simple & fast as possible, the
			// internal color representation is native to different pixel
			// types.  For compatibility with existing code, 'packed' RGB
			// values passed in or out are always 0xBBGGRR order.

			*p++ = c;         // Blue
			*p++ = c >>  8; // Green
			*p++ = c >> 16; // Red

		}
	}


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
	void Paint(void)
	{
 Serial.println ("default paint: ");
		Paint(pixels, numberOfPixels);	
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
	void Paint(uint8_t *  colors, unsigned int count)
	{
   //Serial.print ("2arg paint: ");
		Paint( colors, 20,0, PORTB);
	}
	void Paint(uint8_t * colors, unsigned int count, uint8_t ppin, uint8_t port){



                //here it is,  the dirty work.  truth be told i have no idea if this works.  I tried to test the timings 
                //my only way is using the pickit2 in logic analyzer mode.  It was really helpful,  but i think its off a little bit.
                //
                for ( int i= 0; i<count ; i++)
		{
			asm volatile (
                               " in r26, __SREG__     ; timing-critical, so no interrupts\n" 
                                "cli  ; global interrupts disable\n" 
				"    rcall write_byte%=\n"
				"    rcall write_byte%=\n"
				"    rcall write_byte%=\n"
				"    rjmp string_end%=\n"
				"write_byte%=:\n"
                                 "    ld __tmp_reg__, Z+   ; get next 8 bits\n"   
				"    rcall write_bit%=\n"
				"    rcall write_bit%=\n"
				"    rcall write_bit%=\n"
				"    rcall write_bit%=\n"
				"    rcall write_bit%=\n"
				"    rcall write_bit%=\n"
				"    rcall write_bit%=\n"
				"    rcall write_bit%=\n"
				"    ret\n"
				"write_bit%=:\n"
				"    cbi  %[port], %[pin] \n"      // hi -> lo sarting low state delay 2us
				"    ldi  r18, 8         ;	delay for 2us\n" 
				"13:  dec  r18		 ;\n" 
				"    brne 13b            ;\n"
				"    nop		 ;end delay\n"
				"    rol __tmp_reg__     ; push bit to eval into carry \n"
				"    brcc 15f             ; jump past the stretching of a low state for a 1 bit when it is a 0 \n "
				"    ldi  r18, 34        ;	delay for 6us\n" 
				"14:  dec  r18		 ;\n" 
				"    brne 14b            ;\n"
				"    nop		 ;end delay\n"
				"15:  sbi %[port], %[pin]\n" 
				"    ldi  r18, 45        ;delay for 6us\n" 
				"16:  dec  r18		 ;\n" 
				"    brne 16b            ;\n"
				"    nop		 ;end delay\n"
				"    ret\n"
				"string_end%=:\n "
                                "    out __SREG__, r26    ; reenable interrupts\n" 
				: \
				: [colors] "z" (colors),
				[count] "w" (count),
				[port] "I" (_SFR_IO_ADDR(PORTB)), 
				[pin] "I" (0) 
                                :"r18", "r26","cc", "memory"  );
			delayMicroseconds(590);
		}

		//asm();          // Re-enable interrupts now that we are done.  Hopefully this works!
		digitalWrite(pin, LOW);
		delayMicroseconds(38);         // Hold the line low for 24 microseconds to send the reset signal.
//digitalWrite(pin, HIGH);

	


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
	void SetPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b) {
		if(n < numberOfPixels) { // Arrays are 0-indexed, thus NOT '<='
			uint8_t *p = &pixels[n * 3];
			*p++ = b;
 // Serial.print("b:");         
                         //Serial.println(*p++);
			*p++ = g;
  //Serial.print("g:");         
                       //  Serial.println(*p++);
			*p = r;
  //Serial.print("r:");         
                        // Serial.println(*p);
 
		}
else 
  Serial.print("error in void SetPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b)");  

	}

	// Set pixel color from 'packed' 32-bit RGB value:
	void SetPixelColor(uint16_t n, uint32_t c) {
		if(n < numberOfPixels) { // Arrays are 0-indexed, thus NOT '<='
			uint8_t *p = &pixels[n * 3];
                        *p++ = c;         // Blue
                        // Serial.print("b:");         
                        // Serial.println(*p);
                        *p++ = c >>  8; // Green
		         ////Serial.print("g:");         
                         //Serial.println(*p);
                	*p = c >> 16; // Red
		        // Serial.print("r:");         
                        // Serial.println(*p);
                			
  
		}
else

 Serial.print("error in void SetPixelColor(uint16_t n, uint8_t raqsdasd, uint8_t g, uint8_t b)");  

                    
	}


	// Query color from previously-set pixel (returns packed 32-bit RGB value)
	uint32_t GetPixelColor(uint16_t n) {
		if(n < numberOfPixels) {
			uint16_t ofs = n * 3;
			// To keep the show() loop as simple & fast as possible, the
			// internal color representation is native to different pixel
			// types.  For compatibility with existing code, 'packed' RGB
			// values passed in or out are always 0xRRGGBB order.
			return (true) ?
				((uint32_t)pixels[ofs] << 16) | ((uint16_t) pixels[ofs + 1] <<  8) | pixels[ofs + 2] :
			(pixels[ofs] <<  8) | ((uint32_t)pixels[ofs + 1] << 16) | pixels[ofs + 2];
		}
		return 0;
	}
	//definition of helper method to return pixel count
	uint32_t GetPixelCount(void) {

		return numberOfPixels;

	}
	//definition of helper method to return pixel count
	void SetPixelCount(  uint32_t pCount) {
		alloc(pCount);

	}

	void alloc(uint16_t n) {
     

  		if (pixels != NULL) {
			free(pixels);
		}
		numberOfPixels = ((pixels = (uint8_t *)calloc(n, 3)) != NULL) ? n : 0;
       
        uint8_t *temp = pixels;
        

        for (int i=0;i<numberOfPixels;i++)
        {
                 
       *(temp++) = 0;
        // Serial.print("b:");         
        // Serial.println(*temp);
         *(temp++)=0;
        // Serial.print("g:");                    
        // Serial.println(*temp);
         *(temp++)=0;
       //Serial.print("r:"); 
        // Serial.println(*temp);
       

                    
        
          //Serial.println(&temp);
        }
 
}

private:
	//number of pixels
	uint16_t numberOfPixels;

	//pointer to each color value (24 bits 3 bytes each)
	uint8_t * pixels;

	//pin on arduino
	//unsigned char pin;

	//DataPort register
	volatile uint8_t * dataport;   
 
	// allocate memory for pixels
	//example for pre-allocation of pixel space from ADAFRUIT WS2801 example

};

