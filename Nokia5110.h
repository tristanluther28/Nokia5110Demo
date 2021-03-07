  
//-----------------------------------------------------------------------------
//
//  Nokia5110.h
//
//  Swallowtail Nokia 5110/PCD8544 Firmware
//  Nokia 5110/PCD8544 Firmware
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

#define DC PB4
#define RST PB5
#define BIT_SET(byte, bit) (byte & (1<<bit))

/******************** Includes ***************************/

#include <avr/io.h>
#include <string.h>
#include "SPI.h"
#include "ASCII.h"

/******************* Globals *****************************/


/******************** Functions **************************/
//Reset the display
void Nokia5110_Reset(){
	PORTB &= ~(1<<RST);
	_delay_ms(100);
	PORTB |= (1<<RST);
}

//Clear the display
void Nokia5110_Clear(){
	SPI_Enable();
	PORTB |= (1<<DC);
	//Traverse the entire area of the screen
	for (int i=0; i<=503; i++)
	{
		SPI_Transfer(0x00);
	}
	PORTB &= ~(1<<DC);
	SPI_Disable();
}

//Fill the display
void Nokia5110_Fill(){
	SPI_Enable();
	PORTB |= (1<<DC);
	//Traverse the entire area of the screen
	for (int i=0; i<=503; i++)
	{
		SPI_Transfer(0xFF);
	}
	PORTB &= ~(1<<DC);
	SPI_Disable();
}

//Send a command to the display
void Nokia5110_Command(char byte){
	//Set DC to zero to indicate a command
	PORTB &= ~(1<<DC);
	//Enable the SPI Line
	SPI_Enable();
	//Transfer the byte of data over
	SPI_Transfer(byte);
	//Set the DC pin to high to indicate data operation
	PORTB |= (1<<DC);
	SPI_Disable();
}

//Set the column and row location of the cursor
void Nokia5110_SetXY(char x, char y){
	x = (x % 84);
	y = (y % (48/9 + 1));
	Nokia5110_Command(0x80 | x);
	Nokia5110_Command(0x40 | y);
}

//Inverts the current screen
void Nokia5110_Invert(){
	Nokia5110_Command(0x0D);
}

//Noninverts the screen
void Nokia5110_Noninvert(){ 
	Nokia5110_Command(0x0C);
}

//Send ASCII data to the display
void Nokia5110_Data(char* data){
	//Set the DC Pin for Data Operation
	PORTB |= (1<<DC);
	//Enable Communication
	SPI_Enable();
	int length = strlen(data);
	for (int i=0; i<length; i++)
	{
		for (int j=0; j<5; j++)
		{
			//Send the data
			SPI_Transfer(pgm_read_byte(&(ASCII[data[i] - 0x20][j])));
		}
		SPI_Transfer(0x00);
	}
	SPI_Disable();
}

//Send image data to the screen
void Nokia5110_Image (const unsigned char *image_data){
	SPI_Enable();
	PORTB |= (1<<DC);
	//Traverse the entire area of the screen
	for (int i=0; i<=503; i++)
	{
		SPI_Transfer(pgm_read_byte(&(image_data[i])));
	}
	PORTB &= ~(1<<DC);
	SPI_Disable();
}

//Initialize the SPI Communication and clear the screen
void Nokia5110_init(){
	//Initialize the output
	DDRB |= (1<<DC)|(1<<RST); //DC | RST
	//Set the default values for outputs to zero and inputs to have pull-up resistors
	PORTB |= (0<<DC)|(0<<RST); //DC | RST
	SPI_init(); //Init SPI Communications
	Nokia5110_Reset(); //Hardware Reset
	Nokia5110_Command(0x21);  // Command set in addition mode
	Nokia5110_Command(0xC0);  // Set the voltage by sending C0 means VOP = 5V
	Nokia5110_Command(0x07);  // Set the temp. coefficient to 3
	Nokia5110_Command(0x13);  // Set value of Voltage Bias System
	Nokia5110_Command(0x20);  // Command set in basic mode
	Nokia5110_Command(0x0C);  // Display result in normal mode
	return; //Return to call point
}


/******************** Interrupt Service Routines *********/
