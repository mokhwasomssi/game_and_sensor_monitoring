/**
 * @file   accel_monitor.c
 * @brief  1초 마다 가속도 값을 읽어서 text lcd에 출력
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

/* driver functions */
int textlcd_fd;
void driver_open();
void driver_close();

/* main functions */
void accel_print();


/* main start */
int main(int argc, char* argv[])
{
	// open  driver 
	driver_open();
	
	// 1초 마다 가속도 값을 읽어서 text lcd에 출력 - 5번 반복
	int i;
	for(i = 0; i < 5; i++)
	{
		accel_print();
		sleep(1);
	}
	
	// close driver
	driver_close();
	
	// 가속도 값을 출력한 후에 메뉴로 돌아감
	execl("./menu", "./menu", NULL);
	
	return 0;
}
/* main end */


/* driver functions */
void driver_open()
{	
	textlcd_fd = open(TEXTLCD_DRIVER_NAME,O_RDWR);
	if(textlcd_fd == -1) printf("/dev/peritextlcd open error \n");
}

void driver_close()
{
	close(textlcd_fd);
}

/* main functions */
void accel_print()
{
	stTextLCD  stlcd = {0};

	char accel_text[16] = "accelerometer";
	char accel_value[16];
	FILE* accel_fp;
	int bytesRead;

	accel_fp = popen("./readAcceltest.sh", "r");
	
	if(accel_fp == NULL)
	{
		perror("can't popen readAcceltest.sh");
		exit(0);
	}

	while(!feof(accel_fp))
	{
		bytesRead = fread(accel_value, 1, 16, accel_fp);
		accel_value[bytesRead-1] = 0;

	}

	pclose(accel_fp);

	// print
	printf("%s\n", accel_text);
	printf("%s\n", accel_value);

	// textlcd line 1
	stlcd.cmdData = CMD_DATA_WRITE_LINE_1;
	memcpy(stlcd.TextData[stlcd.cmdData - 1],accel_text,strlen(accel_text));
	stlcd.cmd = CMD_WRITE_STRING;
	write(textlcd_fd,&stlcd,sizeof(stTextLCD));

	// textlcd line 2
	stlcd.cmdData = CMD_DATA_WRITE_LINE_2;
	memcpy(stlcd.TextData[stlcd.cmdData - 1],accel_value,strlen(accel_value));
	stlcd.cmd = CMD_WRITE_STRING;
	write(textlcd_fd,&stlcd,sizeof(stTextLCD));
}
