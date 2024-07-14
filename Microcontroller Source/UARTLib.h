#ifndef UARTLib
#define UARTLib

#define even 0
#define odd 1

#define f_cpu 8000000ul
#include <math.h>
#include <avr/io.h>
uint16_t a=0;

void uarttransmit(unsigned char data)
{
	//wait until the transmitter is ready
	while (! (UCSRA & (1 << UDRE)) );

	//get that data outa here!
	UDR = data;
}

unsigned char uartreceive(void)
{
	//while (! (UCSRA & (1 << RXC)) )
	//{
		//uarttransmit('n');
		//a++;
		//if (a>200) 
		//{
			//a=0;
			//return 3 ;
		//}
	//}
	//return UDR;
	/* Wait for data to be received */
	while ( !(UCSRA & (1<<RXC)) )
		{;}
	/* Get and return received data from buffer */
	//uarttransmit('b');
	return UDR;
}


void uartinitialize(int baud, char asyncdoublespeed, char datasizeinbits, char parityevenorodd, char stopbits)
{
	uint16_t ubbrvalue =lrint(f_cpu/(16l * baud)-1);
	
	if (asyncdoublespeed ==1) UCSRA =(1<< U2X	);
	
	UBRRH=(unsigned char)(ubbrvalue >> 8);
	UBRRL=(unsigned char)(ubbrvalue);
	UCSRB|=(1<<RXEN)|(1<<TXEN);
	
	//if (stopbits==2) ucsrc =1<<usbs;
	//if (parityevenorodd ==even) ucsrc |= (1<<upm1);
	//if (parityevenorodd ==odd) ucsrc |= (3<<upm0);
	//if (datasizeinbits ==6) ucsrc |= (1<< ucsz0);
	//if (datasizeinbits ==7) ucsrc |= (2<< ucsz0);
	//if (datasizeinbits ==8) ucsrc |= (3<< ucsz0);
	//if (datasizeinbits ==9) ucsrc |= (7<< ucsz0);
	
	if (stopbits == 2) UCSRC = (1<<URSEL)|1 << USBS;
	if (parityevenorodd == even) UCSRC |= (1<<URSEL)|(1 << UPM1);
	if (parityevenorodd == odd) UCSRC |= (1<<URSEL)|(3 << UPM0);
	if (datasizeinbits == 6) UCSRC |= (1<<URSEL)|(1 << UCSZ0);
	if (datasizeinbits == 7) UCSRC |= (1<<URSEL)|(2 << UCSZ0);
	if (datasizeinbits == 8) UCSRC |= (1<<URSEL)|(3 << UCSZ0);
	if (datasizeinbits == 9) UCSRC |= (1<<URSEL)|(7 << UCSZ0);

}




//#define FOSC 8000000 // Clock Speed
//#define BAUD 9600
//#define MYUBRR 51  //FOSC/16/BAUD-1
//
//void USART_Init( unsigned int ubrr)
//{
	///*Set baud rate */
	////UBRRH &= ~(1<<URSEL) 
	//UBRRH = (unsigned char)(ubrr>>8);
	//UBRRL = (unsigned char)ubrr;
	///* Enable receiver and transmitter */
	//UCSRB = (1<<RXEN)|(1<<TXEN);
	///* Set frame format: 8data, 2stop bit */
	//UCSRC = (1<<URSEL)|(1<<USBS)|(3<<UCSZ0);
//}
//
//void USART_Transmit( unsigned char data )
//{
	///* Wait for empty transmit buffer */
	//while ( !( UCSRA & (1<<UDRE)) )
	//;
	///* Put data into buffer, sends the data */
	//UDR = data;
//}
//
#endif