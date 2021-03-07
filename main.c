//-----------------------------------------------------------------------------
//
//  main.c
//
//  Swallowtail Nokia 5110 Demo Firmware
//  AVR (ATtiny26) Nokia 5110 Demo Firmware
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

#ifndef F_CPU
#define F_CPU 1000000UL //Set clock speed to 1MHz
#endif

#define BIT_SET(byte, bit) (byte & (1<<bit))

/******************** Includes ***************************/

#include <avr/io.h>
#include <util/delay.h>
#include "Nokia5110.h"
#include "Logo.h"

/******************* Globals *****************************/

//Add volatile keyword so the compiler won't optimize these variables out if only used in ISR


/******************** Functions **************************/


/******************** Interrupt Service Routines *********/


/******************** Main *******************************/
int main(void)
{
	/* Initialize the I/O Registers */
	/*				-I/O Map-
	 *	
	 */

	/* Initialize the Nokia 5110 */
	Nokia5110_init();
	
    /* State machine loop */
    while (1) 
    {
		//Set the cursor start location and write to the screen
		Nokia5110_Noninvert();
		Nokia5110_Clear();
		Nokia5110_SetXY(0x00, 0x00);
		Nokia5110_Image(LOGO);
		_delay_ms(3000);
		Nokia5110_Clear();
		Nokia5110_SetXY(0x0A, 0x02);
		Nokia5110_Data("Swallowtail");
		Nokia5110_SetXY(0x0A, 0x03);
		Nokia5110_Data("Electronics");
		Nokia5110_Invert();
		_delay_ms(3000);
    }
}

