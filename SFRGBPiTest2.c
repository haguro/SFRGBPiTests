/*
* SFRGBPiTest2.c
* 
* Draws a series of randomly coloured, dual pixel-based patterns,
* sequentially and 1 display at a time.
*
* Part of a "test suite" to help jumpstart/test/troubleshoot your
* Sparkfun's RGB LED matri(x|ces) connection with the
* Raspberry Pi using the SPI interface
*
* Assumes:
*   The LED Matrix backpack is running the 4096 colors capable
*   alternative firmware from https://github.com/fornellas/SFRGBLEDMatrix.
*
* Requires:
* 	The wiringPi library, installed. Download from
*   https://projects.drogon.net/raspberry-pi/wiringpi/
*/

#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <time.h>

int main (void)
{
	//Configure according to your setup:
	uint8_t channel = 0; 			// Raspberry Pi SPI channel
	uint32_t displays = 4;		// Number of RGB LED Matrices	
	uint32_t delaytime = 250; // The delay (in msec)  before drawing a new pattern

	uint32_t i = 0;
	uint32_t j = 0;
	uint32_t k = 0;
	uint32_t bufsize = 96*displays;
	uint8_t outbuffer [bufsize];
	uint8_t tempbuffer [bufsize];
	uint8_t c[30000];
	uint16_t c1, c2;

	printf ("Sparkfun RGB LED Matrix Raspberry Pi test #2\n");

	wiringPiSetup();
	// The alternative firmware should (and does when used with the Aurdino, support
  // an SPI connection with a DIV4 speed devider (4MHz). I haven't figured out yet
  // why I cannot get more than ~120kHz to work. Still, for a 4 displays setup,
  // that frequency is more than sufficient.
	if (wiringPiSPISetup (0,120000) < 0)
	{
		fprintf (stderr, "Unable to open SPI device 0: %s\n", strerror (errno));
		exit (1);
	}

	printf ("Building colour pool\n");
	for (j = 0 ;j < 30000;j++)
	{
		srand(time(NULL) + (j^2));
		c[j] = rand () % 256;
	}

	printf ("Starting\n");
	printf ("Buffer size: %d bytes\n", bufsize);
	printf ("Delay time is : %d milliseconds\n", delaytime);

	for (k = 0; k < 30000-3; k += 3)
	{
		// Print the colours being used (in both formats) in a user-friendly manner.
		c1 = (c[k] << 4) + (c[k+1] >> 4);
		c2 = ((c[k+1] << 8) + c[k+2]) & 0xfff; 
		printf ("Drawing colours: [\033[1mbytes:\033[0m \033[32m%02x %02x %02x \033[0m][\033[1mwords:\033[0m \033[33m%03x %03x\033[0m]\n", c[k], c[k+1], c[k+2], c1,c2);
		
		memset (outbuffer, 0, bufsize);		//Initialise the output buffer
		for (i = 0; i <= bufsize-3; i += 3) {
			outbuffer[i] = c[k];
			outbuffer[i+1] = c[k+1];
			outbuffer[i+2] = c[k+2];

			//Save the output buffer as wiringPiSPIDataRW clears it
			memcpy (tempbuffer, outbuffer, bufsize);
			
			wiringPiSPIDataRW (channel, outbuffer, bufsize);
			delayMicroseconds (297); //Minimum delay required by the backpack.

			memcpy (outbuffer, tempbuffer, bufsize);
		}

		delay (delaytime);
	}

	printf ("Done\n");
	return 0;
}
