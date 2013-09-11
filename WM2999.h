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
// And a LOT of help from Andy L. @ DIYC for getting these pixels timing figured out and explaining it to us mortals 
// Also thanks to Zeph for posing the question and others for finding them for 5$ a set :)  
// You can read about that here and all the others hard work,  Ernie H.,KingofKYa, bigredsoftware .... THANKS EVERYONE! 
// http://doityourselfchristmas.com/forums/showthread.php?24162-Hackable-Cheap-Walmart-quot-Pixels-quot-an-exlectronic-puzzle
// 
//This driver is a bit banging approach to driving the pixels on a Atmega 328p but should work with any avr 16mhz chip. 
//

#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#ifndef  WM2999_H
#define  WM2999_H




//struct to store the pixel color information
//this is a packed struct such that the memory
//is contigous allowing for pointer iteration.
typedef struct __attribute__ ((__packed__)) 
{
	uint8_t  r, g, b;
} rgb_color;



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






//Template class to handle one string of pixels on a particular pin.
//the pin is meant to specify what you know as a pin on an arduino,  not a pin on the atmega chip
//IE Pin 8 would be pin0, portb - your code would use 8 , not 0.
class WM2999
{
public:	

		WM2999(uint8_t ppin): pin(ppin){
			this->externalDataPointerSet=false;
		}


	//cleanup from the alloc calls - thanks to adafruit driver for idea
	~WM2999(void)
	{
		if (pixels != NULL) {
			free(pixels);
		}

	}

	// setup the pins
	void start() {
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

		//if(n < numberOfPixels) { // Arrays are 0-indexed, thus NOT '<='
			uint8_t *p = &pixels[n * 3];
			// To keep the show() loop as simple & fast as possible, the
			// internal color representation is native to different pixel
			// types.  For compatibility with existing code, 'packed' RGB
			// values passed in or out are always 0xBBGGRR order.

			*p++ = c;         // Blue
			*p++ = c >>  8; // Green
			*p++ = c >> 16; // Red

		//}
	}


	//Convienence method, updates based on member variables.
	void paint(void)
	{
printf("wm2999::paint %d - %d %d\n", numberOfPixels,pixels[0], pixels[1]);
		paint(pixels, numberOfPixels);
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
	/* void paint(uint8_t *  colors, unsigned int count)
	{
		//TODO:  Refactor to use the specified pin bit
		//THIS IS CURRENTLY HARD CODED TO PIN0 on PORTB or ( digital pin 8)
		paint( colors, numberOfPixels,pinBit[pin],pinAddr[pin]);
	} */



	//This method has the main functionality for updating the pixel string with new color information.
	//the colors pointer is the pointer to the first pixels bits,  since memory is packed we will iterate by incrementing the pointer by 3
	//count - the number of pixels in the string.
	//ppin - the pin to output on
	//port - the port the pin is located on

	void  __attribute__((always_inline)) paint(uint8_t * colors, unsigned int count){
				//move to the red byte
				colors = colors + 2;
		
				//Reset the line so that the first output of "Lo" will be interpreted.
				//TODO - refactor based on timer idea as specified at the end of the loop.
				//       Hopefully that will speed up the pixel timing.
                digitalWrite(pin, HIGH);
               
                //Begin the loop of updating the string of pixels with new color information
			    //This code was written in avr assembler so that the timings could be made to be
				//more precise than the delayMicroseconds allows for.
                for ( int i= 0; i<count ; i++)
				{
					asm volatile (
					"    in r26, __SREG__      ; timing-critical, so no interrupts\n" 
                    "    cli                   ; global interrupts disable\n" 
					"    ld __tmp_reg__, Z\n" // get 8 bits from memory and increment the pointer\n"   					  					
					"    rcall write_byte%=\n" // call write_byte for blue bits
					"    ld __tmp_reg__, -Z\n" // get 8 bits from memory and increment the pointer\n"   
					"    rcall write_byte%=\n" // call write_byte for the green bits
					"    ld __tmp_reg__, -Z\n" // get 8 bits from memory and increment the pointer\n"   
					"    rcall write_byte%=\n" // call write_byte for the red bits 
					"    rjmp string_end%=\n"  // go to the end of the routine
					"write_byte%=:\n"
					
					"    rcall write_bit%=\n"  // The next 8 calls handle outputting 8 bits of color information
					"    rcall write_bit%=\n"
					"    rcall write_bit%=\n"
					"    rcall write_bit%=\n"
					"    rcall write_bit%=\n"
					"    rcall write_bit%=\n"
					"    rcall write_bit%=\n"
					"    rcall write_bit%=\n"
					"    ret\n"                      // return from call for write byte
					"write_bit%=:\n"                 // write_bit routine to handle the output of a single bit
					"    cbi  %[port], %[pin] ;\n"   // set the pin to low ( works even if its already low )
					"    ldi  r18, 10         ;\n"   // The next few lines are a loop to cause a delay for 2 microseconds 
					"13:  dec  r18		      ;\n"   // decrement the counter
					"    brne 13b             ;\n"   // if we are done killing time break out of the loop
					"    nop                  ;\n"   // one extra nop just for good measure
					"    rol __tmp_reg__      ;\n"   // rotate the next bit into the carry so we can check if its a 0 or 1
					"    brcc 15f             ;\n"   // if its a 0 we will skip the next delay 
					"    ldi  r18, 35         ;\n"   // must be a 1 so we need to add another 7 us onto the low state
					"14:  dec  r18		      ;\n"   // check if we are done waiting ( this kills time, its all it does )  
					"    brne 14b             ;\n"   // if we are done waiting for the 1 bit low state we can move on to the high
					"    nop				  ;\n"   // one more clock cycle of delay
					"15:  sbi %[port], %[pin] ;\n"   // change the pin from lo -> hi
					"    ldi  r18, 45         ;\n"   // set a counter for the delay loop,  this time we are delaying 9us 
					"16:  dec  r18		      ;\n"   // check if we are done waiting
					"    brne 16b             ;\n"   // if we are done we can exit this function
					"    nop				  ;\n"   // add one more clock cycle.
					"    ret\n"                      // return from the write_bit call
					"string_end%=:\n "               // we are all done writing out this pixels worth of bits
					"    out __SREG__, r26    ;\n"	 // reenable interrupts, this should probally be happening outside the loop for efficiency, and safety, but its working....
					: \
					: [colors] "z" (colors),
					[port] "I" _SFR_IO_ADDR(PORTC),
					[pin] "I" (0)
									:"r18", "r26","cc", "memory"  );
				
					//we finished one pixel,  the line is still high,  add the 600 microsecond pause,  
					delayMicroseconds(585);  // 585 takes into account the timing from the loop and other stuff 
			
					//increment the pointer to the next pixel 
					colors=colors+3;
				}


				digitalWrite(pin, LOW);// Holding the Line low for 2ms.  Change made with Andy's findings on 1/28/13 that 600 was not necessary.
				
				delay(2);         // TODO change from a delay of 2 ms to a timer.
								  // when this function returns there will likely be time needed to
						          // fetch from input data.  The timer could be checked at the beginning of the 
						          // paint call to finish the 2ms delay.  If a timer worked,  it should allow the updates to run faster.
				
				////Setting back to high.  This may not be needed still need to test this. 
				//// not needed as there will be a high at the beginning of the 
				digitalWrite(pin, HIGH);
				
	}


	// Set pixel color from separate 8-bit R, G, B components:
	// n is the pixel index (0-19) - its a bit backwards so,  pixel 0 is the 
	// last pixel in the string.  
	// r, g, b params are 8 bit RGB value
	// 
	//  For example:  RGB r=255,g=255, b=255
	//  in binary :  11111111 11111111 11111111
	//                RGB r=128, g=128, b=128
	//               10000000 10000000 10000000
	// 
	//  the most significant bit in each byte control the color being on
	//   so:  10000000  is less intense than 10000001
	//
	//  TODO research more about color mixing and support for colors.
	void setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b) {
		if(n < numberOfPixels) { // Arrays are 0-indexed, thus NOT '<='
			uint8_t *p = &pixels[n * 3];
			*p++ = b;
			*p++ = g;
			*p = r;
	  	}
	}

	// Set pixel color from 'packed' 32-bit RGB value:
	// Value will be in format Blue, Green, Red and 
	// the color C only 24 of the 32 bits are used.
	//void SetPixelColor(uint16_t n, uint32_t c) {
		//if(n < numberOfPixels) { // Arrays are 0-indexed, thus NOT '<='
		//	uint8_t *p = &pixels[n * 3];
            //            *p++ = c;         // Blue
            //            *p++ = c >>  8;   // Green
             //   		*p = c >> 16;     // Red
		        
		//}
	//}


	// Query color from previously-set pixel (returns packed 32-bit RGB value)
	// TODO:  Test this function,  Unsure if it is working correctly.
	//		  
	uint32_t getPixelColor(uint16_t n) {
		if(n < numberOfPixels) {
			uint16_t ofs = n * 3;
			// To keep the show() loop as simple & fast as possible, the
			// internal color representation is native to different pixel
			// types.  For compatibility with existing code, 'packed' RGB
			// values passed in or out are always 0xRRGGBB order.
			return 
				((uint32_t)pixels[ofs] << 16) | ((uint16_t) pixels[ofs + 1] <<  8) | pixels[ofs + 2];
		}
		return 0;
	}

	//Accessor method to expose the number of pixels in the string
	uint32_t getPixelCount(void) {
		return numberOfPixels;

	}

	//Mutator to set the number of pixels in the string
	void setPixelCount(  uint32_t pCount) {
		if(!this->externalDataPointerSet)
			alloc(pCount);
			else
	    this->numberOfPixels=pCount;

	}


	// Handle memory allocation for the entire strings worth of data.
	// This allocation will be a continuous "packed" section of memory
	// The pixels pointer in this class is the base address of that memory location
	void alloc(uint16_t n) {
     
		// If we already have some memory allocated, free it before getting more.
  		if (pixels != NULL) {
			free(pixels);
		}
		// If something goes wrong with the allocation 
		// the pixel string will be set to length 0
		numberOfPixels = ((pixels = (uint8_t *)calloc(n, 3)) != NULL) ? n : 0;
       
       }
        
		/**
		 * Helper to initialize the string (make sure its working)
		 * This may also be used for debugging...
		 *
		 * @Param c - the 24 bit RGB color value - this is the integer representation of a color
		 * @param wait - the duration to wait after the color is set.
		 */

		void colorWipe(uint32_t c, uint8_t wait) {
			int i;

			for (i=0; i < this->getPixelCount(); i++) {
				this->setPixelColor(i, c);
				this->paint();
				delay(wait);
			}

		}

		/* Helper functions */

		/**
		 * Create a 24 bit color value from R,G,B
		 *
		 * @param r - red 0-255 value
		 * @param g - green 0-255 value
		 * @param b - blue 0-255 value
		 *
		 * @return the color in a 24 bit pattern
		 */
		uint32_t color(byte r, byte g, byte b)
		{
			uint32_t c;
			c = b;
			c <<= 8;
			c |= g;
			c <<= 8;
			c |= r;
			return c;
		}




protected:
	uint16_t numberOfPixels;   //number of pixels
	uint8_t * pixels;		   //Pointer to the base address of the pixel colors in memory
							   //if alloc has been called this will be initialized to be pointing
							   //at pixel 0, green bits.  Note ( pixel 0 is the last pixel in the string Not the first!)
	uint8_t pin;
	//DataPort register
	volatile uint8_t * dataport;   
	
	bool externalDataPointerSet;
 
};
#endif /* WM2999_H_ */
