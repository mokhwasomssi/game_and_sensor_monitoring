/**
 * @file   menu.c
 * @brief  게임1, 게임2, 센서모니터링에 접근하기 위한 메뉴
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
#define INPUT_DEVICE_LIST	"/dev/input/event4" // 숫자유의

/* driver functions */
int textlcd_fd, button_fd;
void driver_open();
void driver_close();

/* main functions */
void menu_print();
void close_print();
int button_read();


/* main start */
int main(int argc , char **argv)
{
	// open  driver 
	driver_open();

	// menu
	menu_print();
	sleep(1);

	while(1)
	{
		menu_print();

		switch (button_read())
		{
			case 0: // 게임1
			printf("button : 0\n");
			execl("./Game1", "./Game1", NULL);
			break;
			case 1: // 게임2
			printf("button : 1\n");
			execl("./Game2", "./Game2", NULL);
			break;			
			case 2: // 온도 모니터링
			printf("button : 2\n");
			execl("./temperature_monitor", "./temperature_monitor", NULL);
			break;		
			case 3: // 자이로 모니터링
			printf("button : 3\n");
			execl("./gyro_monitor", "./gyro_monitor", NULL);
			break;
			case 4: // 가속도 모니터링
			printf("button : 4\n");
			execl("./accel_monitor", "./accel_monitor", NULL);
			break;	
			case 5: // 종료
			printf("button : 5\n");
			close_print();
			return 0;
			break;
		}
	}

	// close driver
	driver_close();
	
	return 0;
}
/* main end */


/* main functions */
void menu_print()
{
	stTextLCD  stlcd = {0};

	char menu_text[16] = "menu";
	char click_text[16] = "push button";

	// line 1
	stlcd.cmdData = CMD_DATA_WRITE_LINE_1;
	memcpy(stlcd.TextData[stlcd.cmdData - 1],menu_text,strlen(menu_text));
	stlcd.cmd = CMD_WRITE_STRING;
	write(textlcd_fd,&stlcd,sizeof(stTextLCD));

	// line 2
	stlcd.cmdData = CMD_DATA_WRITE_LINE_2;
	memcpy(stlcd.TextData[stlcd.cmdData - 1],click_text,strlen(click_text));
	stlcd.cmd = CMD_WRITE_STRING;
	write(textlcd_fd,&stlcd,sizeof(stTextLCD));
}

void close_print()
{
	stTextLCD  stlcd = {0};

	char close_text[16] = "good bye!";
	char click_text[16] = "good bye!";

	// line 1
	stlcd.cmdData = CMD_DATA_WRITE_LINE_1;
	memcpy(stlcd.TextData[stlcd.cmdData - 1],close_text,strlen(close_text));
	stlcd.cmd = CMD_WRITE_STRING;
	write(textlcd_fd,&stlcd,sizeof(stTextLCD));

	// line 2
	stlcd.cmdData = CMD_DATA_WRITE_LINE_2;
	memcpy(stlcd.TextData[stlcd.cmdData - 1],close_text,strlen(close_text));
	stlcd.cmd = CMD_WRITE_STRING;
	write(textlcd_fd,&stlcd,sizeof(stTextLCD));
}

int button_read()
{
	struct input_event  stEvent;
	int readSize;

	readSize = read(button_fd, &stEvent , sizeof(stEvent));
	if(readSize != sizeof(stEvent))	return -1;

	if (stEvent.type == EV_KEY && stEvent.value)
	{
		switch(stEvent.code)
		{
			case KEY_HOME:
			return 0;
			break;
			case KEY_BACK:
			return 1;
			break;
			case KEY_SEARCH:
			return 2;
			break;
			case KEY_MENU:
			return 3;
			break;
			case KEY_VOLUMEUP:
			return 4;
			break;
			case KEY_VOLUMEDOWN:
			return 5;
			break;
		}
	}
	else
	{ 
		return -2;
	}
}

/* driver functions */
void driver_open()
{	
	textlcd_fd = open(TEXTLCD_DRIVER_NAME,O_RDWR);
	if(textlcd_fd == -1) printf("/dev/peritextlcd open error \n");

	button_fd = open(INPUT_DEVICE_LIST, O_RDONLY);
	if(button_fd == -1) printf("/dev/input/event4 read error \n");
}

void driver_close()
{
	close(textlcd_fd);
	close(button_fd);
}
