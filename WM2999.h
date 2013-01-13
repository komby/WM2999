//
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
// This has sure been fun :)
//

#ifndef  WM2999_H
#define  WM2999_H

#include <Arduino.h>


#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>




	typedef struct rgb_color
	{
		unsigned char r, g, b;
	} rgb_color;




	//base class for pixels - this is overkill for today but hey its cpp lets inherit something :)
	class PixelBase {
	public: 
		void virtual Paint(rgb_color *, unsigned int count) = 0;
		void virtual Paint(void) = 0;
		uint32_t virtual GetPixelCount( void ) = 0;
	    void virtual SetPixelCount( uint32_t ) = 0;
		void virtual start( void ) = 0;
		uint32_t virtual GetPixelColor(uint16_t n) = 0;
		//n is the index of previous color
	//	uint32_t virtual SetPixelColor(uint16_t n) = 0;
		void virtual SetPixelColor(uint16_t n, uint32_t c)  = 0;
		void virtual SetPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b) = 0;
	};


	template<unsigned char pin> class WM2999: public PixelBase
	{



	public:
		WM2999(void);
		WM2999(uint16_t pNumPixels);
		~WM2999(void);
		//see cpp file for declartions of instance methods

		//setup the pins for this instance
		void virtual start();
		//update the entire pixel string
		void virtual Paint(rgb_color *, unsigned int count);
		void virtual Paint(void);

		//helper to get how many pixels are in this string
		uint32_t virtual GetPixelCount(void) ;
		//setup
		void virtual SetPixelCount( uint32_t pCount );
		uint32_t virtual GetPixelColor(uint16_t n);
		void virtual SetPixelColor(uint16_t n, uint32_t c);
		//uint32_t virtual SetPixelColor(uint32_t c);
		//void virtual SetPixelColor(uint16_t n, uint32_t c);
		void virtual SetPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b);

	

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
		void alloc( uint16_t  ) ;
	};


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

	#endif