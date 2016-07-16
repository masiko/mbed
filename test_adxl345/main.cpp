#include "mbed.h"
#include <stdio.h>

Serial pc(USBTX, USBRX);
SPI spi(p5, p6, p7); // mosi, miso, sclk
DigitalOut cs(p8);
LocalFileSystem local("local");

int main() {
	int devid;
	unsigned short 
		x0,x1,
	    y0,y1,
	    z0,z1 = 0;
	short
		x,y,z = 0;
//	FILE *fp = fopen("/local/acc.txt","w");
    pc.baud(115200);
    pc.format(8,Serial::None,1);
	// Chip must be deselected
    cs = 1;
	// Setup the spi for 8 bit data, high steady state clock,
	// second edge capture, with a 1MHz clock rate
	spi.format(8,3);
	spi.frequency(3200);
						  
	// Select the device by seting chip select low
    cs = 0;
						   
    // Send 0x31, spi-4line-mode
	int add = (0x00 & 0x3F) | 0x80;
    spi.write(add);
					    
    // Read Device-ID = E5
    devid = spi.write(0x00);
														 
    // Deselect the device
    cs = 1;

	pc.printf("Device ID = 0x%X\n", devid);

	//Set Power ON
	cs = 0;
	add = (0x2D & 0x3F) | 0x00;
	spi.write(add);
	spi.write(0x08);
	cs = 1;
	//Set FULL RES mode & +-16g Range
	cs = 0;
	add = (0x31 & 0x3F) | 0x80;
	spi.write(add);
	spi.write(0x0B);
	cs = 1;
	for(;;) {
	//	pc.printf("spi disable");
		wait_ms(1);
		cs = 0;
		add = (0x32 & 0x3F) | 0x80;
		spi.write(add);
		x0 = spi.write(0x00);
		cs = 1;
		wait_us(1);
		cs = 0;
		add = (0x33 & 0x3F) | 0x80;
		spi.write(add);
		x1 = spi.write(0x00);
		x = (x1<<8)|x0 ;
		//pc.printf("%X,%X\n", x0,x1);
		cs = 1;

		wait_us(1);
		cs = 0;
		add = (0x34 & 0x3F) | 0x80;
		spi.write(add);
		y0 = spi.write(0x00);
		cs = 1;
		wait_us(1);
		cs = 0;
		add = (0x35 & 0x3F) | 0x80;
		spi.write(add);
		y1 = spi.write(0x00);
		y = (y1<<8)|y0 ;
		cs = 1;
		
		wait_us(1);
		cs = 0;
		add = (0x36 & 0x3F) | 0x80;
		spi.write(add);
		z0 = spi.write(0x00);
		cs = 1;
		wait_us(1);
		cs = 0;
		add = (0x37 & 0x3F) | 0x80;
		spi.write(add);
		z1 = spi.write(0x00);
		z = (z1<<8)|z0 ;
		//pc.printf("%X,%X,", z0,z1);
		cs = 1;
		
		wait_ms(1);
		pc.printf("%d,%d,%d,\n",x,y,z);
		//fprintf(fp, "%d, %d, %d/",x,y,z);
		if(pc.readable()) break;
		wait_ms(20);
	}
//		fclose(fp);
		return 1;
}
