
#include <16F84.h>
#fuses HS,NOWDT,NOPROTECT,PUT

#use fast_io(B)

#use delay(clock=10000000)

#define BOARD_DATA PIN_B0
#define BOARD_LATCH PIN_B1
#define BOARD_CLOCK PIN_B2
#define BOARD_BLANK PIN_B3
#define BOARD_CS0 PIN_B4
#define BOARD_CS1 PIN_B5
#define BOARD_CS2 PIN_B6
#define BOARD_DS PIN_B7

char currentline = 0;
char buffer [8][2];

void set_test()
{
	buffer[0][0] = 0b00001111; 	buffer[0][1] = 0b00001111; 
	buffer[1][0] = 0b10101010; 	buffer[1][1] = 0b10101010; 
	buffer[2][0] = 0b10000000; 	buffer[2][1] = 0b00001000; 
	buffer[3][0] = 0b00110011; 	buffer[3][1] = 0b00110011; 
	buffer[4][0] = 0b01010101; 	buffer[4][1] = 0b01010101; 
	buffer[5][0] = 0b11110000; 	buffer[5][1] = 0b11110000; 
	buffer[6][0] = 0b11001100; 	buffer[6][1] = 0b11001100; 
	buffer[7][0] = 0b11011011; 	buffer[7][1] = 0b11101011; 
}

void main (void)
{
	int i;
	int dsel = 0;
	char bu1;
	char bu2;
	char rotation = 0;
	int16 rotation_delay = 300;

	set_tris_b(0b00000000);
	output_b(0x00);


	set_test();
	//output_high(BOARD_BLANK);

	while (1)
	{

		for (currentline = 0; currentline < 8; currentline++)
		{

			// DISPLAY OFF
			output_high(BOARD_BLANK);
	
			// SET COMMON
			output_bit(BOARD_CS0,(currentline & 0x1));
			output_bit(BOARD_CS1,(currentline & 0x2) >> 1);
			output_bit(BOARD_CS2,(currentline & 0x4) >> 2);
	
			// SHIFT OUT CURRENT ROW DATA
			
			bu1 = buffer[currentline][0];
			bu2 = buffer[currentline][1];

			if (rotation)
			{
				for (i=0;i<rotation;i++)
				{
					rotate_left(&bu1,1);
					rotate_left(&bu2,1);
				}
			}

			for (i=0;i<8;i++)
			{
				
				output_bit(BOARD_DATA,bu1&0x1);
				//output_low(BOARD_DATA);
				output_high(BOARD_CLOCK);
				output_low(BOARD_CLOCK);
				bu1 >>= 1;
			}
	
			for (i=0;i<8;i++)
			{
				output_bit(BOARD_DATA,bu2&0x1);
				//output_low(BOARD_DATA);
				output_high(BOARD_CLOCK);
				output_low(BOARD_CLOCK);			
				bu2 >>= 1;
			}
	
			// LATCH
			output_high(BOARD_LATCH);
			output_low(BOARD_LATCH);
	
			output_low(BOARD_BLANK);
			output_low(BOARD_DS);

			delay_us(100);
			
		}

		if (rotation_delay == 0)
		{
			if (rotation == 7) rotation = 0;
			else rotation++;
			rotation_delay = 300;
		} else rotation_delay--;

		


	}
}
