#ifndef ECGSimulatorHead
#define ECGSimulatorHead

void Init_Send2ADC()
{
	DDRC|=0xff;
	DDRD|=(1<<7)|(1<<6);
}
void Send2ADC(uint8_t value)
{
	PORTC = value & 0x3f;
	PORTD = value & 0xC0;
}

#endif