/**
 * @file   temperature_monitor.c
 * @brief  1초 마다 온도센서 값을 읽어서 text lcd에 출력
 */

/* include */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/spi/spidev.h>
#include "textlcddrv.h"
#include <linux/input.h>
#include <sys/mman.h>

/* define driver name */
#define TEXTLCD_DRIVER_NAME	"/dev/peritextlcd"
#define SPI_DRIVER_NAME 	"/dev/spidev1.0"

/* spi functions for temperature sensor */
char gbuf[10];
int spi_init(char filename[40]);
char * spi_read_lm74(int file);

/* driver functions */
int textlcd_fd, temp_spi_fd;
void driver_open();
void driver_close();

/* temperature functions */
int temperature_read();
void temperature_print();


/* main start */
int main(int argc , char **argv)
{
	// open  driver 
	driver_open();

	// 1초 마다 온도센서 값을 읽어서 text lcd에 출력 - 5번 반복
	int i;
	for(i = 0; i < 5; i++)
	{
		temperature_print();
		sleep(1);
	}

	// close driver
	driver_close();

	// 온도센서 값을 출력한 후에 메뉴로 돌아감
	execl("./menu", "./menu", NULL);

	return 0;
}
/* main end*/


/* driver functions */
void driver_open()
{	
	textlcd_fd = open(TEXTLCD_DRIVER_NAME,O_RDWR);
	if(textlcd_fd == -1) printf("/dev/peritextlcd open error \n");

	temp_spi_fd = spi_init(SPI_DRIVER_NAME);
}

void driver_close()
{
	close(textlcd_fd);
	close(temp_spi_fd);
}

/* temperature functions */
int temperature_read()
{	
	char* temp_buffer;
	int temp_value;

	temp_buffer = (char *)spi_read_lm74(temp_spi_fd); // read raw data

	temp_value = (temp_buffer[0] << 8) | temp_buffer[1]; // parsing
	if(temp_value < 0)
		temp_value = (int)(temp_value^(-1));
	temp_value >>= 7;

	//printf("%d \n", temp_value); // print
	
	return temp_value;
}

void temperature_print()
{
	stTextLCD  stlcd = {0};

	int current_temp;
	current_temp = temperature_read(); // read temperature

	char temp_text[16] = "temperature ";
	char temp_val[16];

	sprintf(temp_val, "%d", current_temp);
	strcat(temp_text, temp_val);

	printf("%s\n", temp_text);

	// line 1
	stlcd.cmdData = CMD_DATA_WRITE_LINE_1;
	memcpy(stlcd.TextData[stlcd.cmdData - 1],temp_text,strlen(temp_text));
	stlcd.cmd = CMD_WRITE_STRING;
	write(textlcd_fd,&stlcd,sizeof(stTextLCD));

	// line 2
	stlcd.cmdData = CMD_DATA_WRITE_LINE_2;
	memcpy(stlcd.TextData[stlcd.cmdData - 1],temp_text,strlen(temp_text));
	stlcd.cmd = CMD_WRITE_STRING;
	write(textlcd_fd,&stlcd,sizeof(stTextLCD));

	// 1sec delay;
	sleep(1);
}


/* spi functions for temperature sensor */
int spi_init(char filename[40])
{
	int file;
	__u8    mode, lsb, bits;
	//__u32 speed=2500000;
	__u32 speed=20000;

	if ((file = open(filename,O_RDWR)) < 0)
	{
		printf("Failed to open the bus.");
		/* ERROR HANDLING; you can check errno to see what went wrong */
		exit(1);
	}

	//possible modes: mode |= SPI_LOOP; mode |= SPI_CPHA; mode |= SPI_CPOL; mode |= SPI_LSB_FIRST; mode |= SPI_CS_HIGH; mode |= SPI_3WIRE; mode |= SPI_NO_CS; mode |= SPI_READY;
	mode |= SPI_3WIRE ; 

	if (ioctl(file, SPI_IOC_RD_MODE, &mode) < 0)
	{
		perror("SPI rd_mode");
		return 0;
	}
	if (ioctl(file, SPI_IOC_RD_LSB_FIRST, &lsb) < 0)
	{
		perror("SPI rd_lsb_fist");
		return 0;
	}
	if (ioctl(file, SPI_IOC_RD_BITS_PER_WORD, &bits) < 0) 
	{
		perror("SPI bits_per_word");
		return 0;
	}
 
	printf("%s: spi mode %d, %d bits %sper word, %d Hz max\n",filename, mode, bits, lsb ? "(lsb first) " : "", speed); 
    	return file;
}

char * spi_read_lm74(int file)
{
	int len;

	memset(gbuf, 0, sizeof( gbuf));
	len = read(file, gbuf, 2);
	if (len !=2)
	{
		perror("read error");
		return NULL;
	}

	return gbuf;
}
