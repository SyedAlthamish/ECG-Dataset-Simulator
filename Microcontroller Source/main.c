/*
 * ECG Simulator.c
 *
 * Created: 22-05-2024 10:45:31
 * Author : aathi
 */ 


//////Inclusions
//including necessary base libraries
#include <avr/io.h>
#include <string.h>
#include <avr/interrupt.h>
#define  F_CPU 8000000		//defines the necessary parameter for util/delay.h ---> parameter defines the speed of the MC in Hz
#include <util/delay.h>
//including necessary user-defined libraries
#include "UARTLib.h"
#include "ECGSimulatorHead.h"
#define BUFFER_SIZE 300	
#include "purebuffer.h"


//////Declarations and Definitions
unsigned char startcommchar;			//Var for startcomm_protocol
unsigned char samplingrate;				//Var for the sampling rate of the incoming dataset
unsigned char receiveddata;					//Var to store the received sample from lap into a buffer register	
#define TIMER1_TIME_PER_CYCLE 1.0f		//depicts the the time it takes to complete one osc of the timer clock in microseconds
#define SEC_2_MICROSEC 1000000.0f		//a conversion ratio for seconds to microseconds conversion
#define CHAR2UINT8_DIFF		0x30		//a differential conversion metric for converting char variable into uint8 ones. eg: char_3=0x33-->char_3-0x30=0x30=uint8_3


//////User-defined Functions for readability
void ConfigTimer(void){
	//////Configuring Interrupt for DACing the data
	sei();																	//sets the interrupt enable flag register to all 0xff, i.e. therefore enabling all types of interrupts to function
	TCCR1B |= 1<<WGM12;														// making the timer1 operate at mode12 where value of outputcompareregister of 1a will be the top value
	TIMSK |= 1<<OCIE1A;														// enabling the 1a timer's interrupt from the timer interrupt mask register
	OCR1A = (uint16_t)((1.0/samplingrate)*SEC_2_MICROSEC)/TIMER1_TIME_PER_CYCLE;		// giving a max value for the counter of timer to rise tothis gives the no of clock cycles for the counter to rise to corresponding to the sampling rate
}


//////Main
int main(void)
{
	//////Initialising UART Module with 9600 baud and others...
	uartinitialize(9600, 0, 8, 0, 2);
	
	
	//////Initializing the DACpins of the MC
	Init_Send2ADC();
	
	//////Verify Comms with PC ---> startcomm_protocol
	startcommchar=uartreceive();
	if (startcommchar != 'H'){
		// toggle the led
		uarttransmit('N');
	}else{
		samplingrate=uartreceive()-CHAR2UINT8_DIFF;			//receive the sampling rate and convert to uint8 compatabile
		//send confirmation message back to pc
		uarttransmit('R');							// R for ready
	}
	
	
	//////Configuring the Timer for DACing the data
	ConfigTimer();
	
	
	//////Starting the Timer Once for Regularly Sending Data at sampling rate 
	TCCR1B |= 1<<CS11;


	////////Main While loop
    while (1) 
    {	
		////Receive Data and store it into a fifo buffer
		receiveddata=uartreceive();
		purebuffer_write(receiveddata);	
    }
}

//////ISR for Sending the actual data via adc
ISR(TIMER1_COMPA_vect)
{
	uint8_t data = purebuffer_read();
	Send2ADC(data);
	if (data!=0 )
		uarttransmit(data);
}