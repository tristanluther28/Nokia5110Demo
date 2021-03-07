  
//-----------------------------------------------------------------------------
//
//  SPI_ATtiny26.h
//
//  Swallowtail SPI (USI) Firmware
//  AVR (ATtiny26) SPI (USI) Firmware
//
//  Copyright (c) 2021 Swallowtail Electronics
//
//  Permission is hereby granted, free of charge, to any person obtaining a
//  copy of this software and associated documentation files (the "Software"),
//  to deal in the Software without restriction, including without limitation
//  the rights to use, copy, modify, merge, publish, distribute, sub-license,
//  and/or sell copies of the Software, and to permit persons to whom the
//  Software is furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
//  DEALINGS IN THE SOFTWARE.
//
//  Web:    http://tristanluther.com
//  Email:  tristanluther28@gmail.com
//
//-----------------------------------------------------------------------------

/******************** Macros *****************************/

#define MOSI PB1
#define MISO PB0
#define SCK PB2
#define SS PB3
#define DDR_SPI DDRB
#define PORT_SPI PORTB
#define BIT_SET(byte, bit) (byte & (1<<bit))

/******************** Includes ***************************/

#include <avr/io.h>

/******************* Globals *****************************/


/******************** Functions **************************/

//Initialize the USI on the ATtiny26 for Three-Wire Operation
void SPI_init(){
	//Initialize the output for SPI Master Mode
	DDR_SPI |= (1<<MOSI)|(0<<MISO)|(1<<SCK)|(1<<SS); //MOSI | MISO | SCK | SS
	//Set the default values for outputs to zero and inputs to have pull-up resistors
	PORT_SPI |= (0<<MOSI)|(1<<MISO)|(0<<SCK)|(0<<SS); //MOSI | MISO | SCK | SS
	//USI Control Register, Set to Three-Wire Operation, Use the software clock strobe, Toggle the clock on PB2 when in master mode (it will be)
	USICR |= (0<<USIWM1) | (1<<USIWM0) | (0<<USICS1) | (0<<USICS0) | (1<<USICLK) | (1<<USITC) | (1<<USIOIE);
	return; //Return to call point
}

//Enable the slave select line
void SPI_Enable(){
	//Enable SS Line
	PORT_SPI &= ~(1<<SS);
}

//Enable the slave select line
void SPI_Disable(){
	//Disable SS Line
	PORT_SPI |= (1<<SS);
}

//Reads the value from the ADC (Battery level)
uint8_t SPI_Transfer(char byte){
	//USI Data Register
	USIDR = byte;
	USISR |= (1<<USIOIF); //Clear sent data to receive new data
	//Wait for transmission to complete
	do {
		USICR = (1<<USIWM0)|(1<<USICS1)|(1<<USICLK)|(1<<USITC);
	} while (!(USISR & (1<<USIOIF)));
	byte = USIDR; //Return received data
	//Return the result
	return byte;
}

/******************** Interrupt Service Routines *********/
