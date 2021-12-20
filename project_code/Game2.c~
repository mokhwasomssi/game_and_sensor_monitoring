/**
 * @file   Game2.c
 * @brief  운전 연습 게임
 */


/* include */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
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
void driving_print();
int accel_read(float* x, float* y, float* z);
void angle_read(float accel_x, float accel_y, float accel_z, double* angle_x, double* angle_y);


/* main start */
int main(int argc , char **argv)
{
	float a_x, a_y, a_z;
	double angle_x, angle_y;

	// open  driver 
	driver_open();

	// textlcd에 driving practice 출력
	driving_print();

	// 운전 연습 시작
	system("./bitmap start.bmp");
	sleep(2);
	
	while(1)
	{
		// 가속도를 읽고 각도를 계산함
		accel_read(&a_x, &a_y, &a_z);
		angle_read(a_x, a_y, a_z, &angle_x, &angle_y);
		
		// 각도에 따라 화면을 바꿔줌
		if(angle_x < -10) // 좌회전
			system("./bitmap left.bmp"); 
		else if(angle_x > 10) // 우회전
			system("./bitmap right.bmp");
		else if(angle_y < -10) // 직진
			system("./bitmap go.bmp");
		else if(angle_y > 10) // 후진(브레이크)
		{
			// 브레이크 시에 운전 연습 종료
			system("./bitmap break.bmp");
			sleep(2);
			system("./bitmap end.bmp");
			sleep(2);
			driver_close();	// close driver
			execl("./menu", "./menu", NULL);
		}
		else
			system("./bitmap stop.bmp"); // 정지
		
		usleep(100000); // 0.1초 주기로 반복
	}

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
void driving_print()
{
	stTextLCD  stlcd = {0};

	char driving_text[16] = "driving practice";

	// textlcd line 1
	stlcd.cmdData = CMD_DATA_WRITE_LINE_1;
	memcpy(stlcd.TextData[stlcd.cmdData - 1],driving_text, strlen(driving_text));
	stlcd.cmd = CMD_WRITE_STRING;
	write(textlcd_fd,&stlcd,sizeof(stTextLCD));

	// textlcd line 2
	stlcd.cmdData = CMD_DATA_WRITE_LINE_2;
	memcpy(stlcd.TextData[stlcd.cmdData - 1],driving_text,strlen(driving_text));
	stlcd.cmd = CMD_WRITE_STRING;
	write(textlcd_fd,&stlcd,sizeof(stTextLCD));
}

int accel_read(float* x, float* y, float* z)
{
	char accel_value[20];
	FILE* accel_fp;
	int bytesRead;

	char accel_x[10], accel_y[10], accel_z[10];
	int temp_x, temp_y, temp_z;

	// popen함수를 이용해 가속도 출력 값을 받음
	accel_fp = popen("./readAcceltest.sh", "r");
	
	if(accel_fp == NULL)
	{
		perror("can't popen readAcceltest.sh");
		return 0;
	}

	while(!feof(accel_fp))
	{
		bytesRead = fread(accel_value, 1, 20, accel_fp);
		accel_value[bytesRead-1] = '\0';
	}

	pclose(accel_fp);
	printf("%s\n", accel_value);
	
	int i = 0, j = 0;

	// 문자열에서 가속도 값 추출
	for(j = 0; accel_value[i] != ','; i++, j++)
		accel_x[j] = accel_value[i];
	accel_x[j] = '\0';
	i++;

	for(j = 0; accel_value[i] != ','; i++, j++)
		accel_y[j] = accel_value[i];
	accel_y[j] = '\0';
	i++;

	for(j = 0; accel_value[i] != '\0'; i++, j++)
		accel_z[j] = accel_value[i];
	accel_z[j] = '\0';

	// 문자열에서 정수로 변환
	temp_x = atoi(accel_x);
	temp_y = atoi(accel_y);
	temp_z = atoi(accel_z);
	
	// 미가공 데이터 출력
	printf("raw data : %d,%d,%d\n", temp_x, temp_y, temp_z);

	// 중력가속도 단위로 변환
	*x = (float)((float)temp_x / 16384);
	*y = (float)((float)temp_y / 16384);
	*z = (float)((float)temp_z / 16384);

	// 변환한 데이터 출력
	printf("accel data : %f,%f,%f\n", *x, *y, *z);

	return 1;
}

void angle_read(float accel_x, float accel_y, float accel_z, double* angle_x, double* angle_y)
{
	// 가속도를 이용해 x축 각도를 계산
	*angle_x = atan(accel_y / accel_z) * (180 / 3.14);
	// 가속도를 이용해 y축 각도를 계산
	*angle_y = atan(accel_x/ sqrt(accel_y*accel_y + accel_z*accel_z)) * (180 / 3.14);

	// 계산한 각도 출력
	printf("angle : %lf %lf\n", *angle_x, *angle_y);
}
