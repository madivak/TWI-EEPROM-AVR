/*

A sample program to test the Extrenal EEPROM interfacing
routines. The program fills the whole EEPROM with 7 and 
then reads the whole EEPROM memory to see if all of them
contains 7.

This helps in quick testing of you hardware /software before
using these routines in your own programs.

The target for this program is ATmega8, ATmega16, or ATmega32
running at 16MHz. If you use slower crystal the program will
simply run slow but without any problems.

This program also makes use of eXtreme Electronics 16x2 LCD
Interfacing routines. See the related page for more info

Author : Avinash Gupta
Date : 16 March 2009
Mail : me@avinashgupta.com
Web : www.eXtremeElectronics.co.in

NOTE: IF YOU USE THIS LIBRARY IN YOUR PROGRAMS AND FINDS 
IT USEFUL PLEASE MENTION US IN CREDIT AND RECOMEND OTHERS.


*/

#include <avr/io.h>

#define F_CPU 1000000UL
#include "util/delay.h"

#include <string.h>
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <ctype.h> //for identifying digits and alphabets

#include "24c64.h"
#include "USART.h"
#include "lcd.h"

#define FOSC 2000000UL // Clock Speed
#define BAUD 9600
#define MYUBRR (((FOSC / (BAUD * 16UL))) - 1)

static FILE uart0_output = FDEV_SETUP_STREAM(USART0_Transmit, NULL, _FDEV_SETUP_WRITE);

uint8_t memory[100];
uint8_t MADMAN[] ="111.11.111.111:1111";

/**********************************

A simple delay routine to wait 
between messages given to user
so that he/she gets time to read them

***********************************/
void Wait()
{
	uint8_t i;

	for(i=0;i<100;i++)
		_delay_loop_2(0);
}

void main()
{
	
	USART0_Init(MYUBRR);
	stdout = &uart0_output;
	
	_delay_ms(3000);
	//Variables
	uint8_t failed;
	unsigned int address;

		printf("r\nStarting EEPROM write process....\r\n");
		_delay_ms(3000);

	//Init EEPROM
	EEOpen();
	_delay_loop_2(0);
	printf("Successfully opened EEPROM write process....\r\n");
	_delay_ms(1000);
	printf("Writing to EEPROM....\r\n");
	Wait();
	
	//Write MADMAN in EEPROM
	failed=0;
	for(address=0;address<20;address++)
	{
		if(EEWriteByte(address,MADMAN[address])==0)
		{
			//Write Failed
			printf("Write to EEPROM failed!!\r\n");
			failed=1;
			Wait();
			break;
		}
		printf(".");
	}

	

	if(!failed)
	printf("Done writing to EEPROM\r\n");

	Wait();

	printf("Verifying EEPROM contents...\r\n");

	//write MADMAN characters in EEPROM
	failed=0;
	uint8_t  w;
//	int X=0, M=0;
	for(address=0;address<100;address++)
	{
// 		if(EEReadByte(address)!=7)
// 		{
// 			//Failed !
// 			printf("Reading 7 from EEPROM failed!!\r\n");
// 			failed=1;
// 			Wait();
// 			break;
// 		}
		w = EEReadByte(address);
		memory[address]=w;

	}

	printf("Done Reading from EEPROM!!\r\n");
	if(!failed)
	{
		//We have Done it !!!
		printf("/r/nWrite Success !\r\n Here is the data:\r\n");
		for(int i=0;i<100;i++) {printf("%c" ,memory[i]);}

	}
	
	while(1);

}


