
//////1. func to have have a uchar hardcore array for storing data

//giving a default buffer size for operation if not already specified
#ifndef BUFFER_SIZE
#define BUFFER_SIZE 50
# warning "undefined BUFFER_SIZE. Default set as 50"
#endif

//initializing the buffer, head and tails.
unsigned char databuffer[BUFFER_SIZE];
uint8_t head=0;
uint8_t tail=0;


//////2. func to store data in it

void purebuffer_write(unsigned char sample){
	databuffer[head]=sample;
	head++;
	if (head ==  BUFFER_SIZE) head=0;
}

//////3. func to retrieve data from it

unsigned char purebuffer_read(){
	if (tail==BUFFER_SIZE) tail=0;
	//if (tail==head) return 8;
	return databuffer[tail++];
}
