# game_and_sensor_monitoring
[임베디드 시스템] 전공과목 팀프로젝트 - 게임과 센서 모니터링

## 개발환경
- Ubuntu 14.04
- VirtualBox
- [eCube-EmbeddedAll-in-one](https://hancomnshop.co.kr/product/ecube-embeddedall-in-one/145/)

## 작동영상
- [링크](https://youtu.be/rA6nzgUYMHs)

## 게임과 센서 모니터링

### 메뉴

![image](https://user-images.githubusercontent.com/48342925/146694810-fa66fcd6-0723-44b5-a2ec-9194cf81dbc3.png)


### 게임1 - 폭탄 해체 게임
#### 게임설명
1. LED가 랜덤으로 점멸
2. 점멸 순서를 기억
3. 그 순서대로 버튼을 입력

#### Example
- Round2 
  1. LED3 1초 점멸 -> LED5 1초 점멸
  2. 버튼3 누르기 -> 버튼5 누르기
  3. TextLCD : congratulation, next level
- Round4
  1. LED1 -> LED6 -> LED3 -> LED5
  2. 버튼1 -> 버튼3 -> 버튼2 -> 버튼1
  3. TextLCD : boom...

### 게임2 - 운전 연습 게임
#### 게임설명
1. 키트를 핸들처럼 회전
2. 회전 방향에 따라 LCD에 화살표가 뜸

#### 구현방법
- 가속도 센서로 회전 방향과 각도를 계산
```c
	// 가속도를 이용해 x축 각도를 계산
	angle_x = atan(accel_y / accel_z) * (180 / 3.14);
	// 가속도를 이용해 y축 각도를 계산
	angle_y = atan(accel_x/ sqrt(accel_y*accel_y + accel_z*accel_z)) * (180 / 3.14);
```

### 센서 모니터링
1. 온도 센서 데이터를 1초마다 5번 출력
2. 자이로 센서 데이터를 1초마다 5번 출력
3. 가속도 센서 데이터를 1초마다 5번 출력

## 빌드 과정

### 1. project 폴더  
1. 소스코드  
	- menu.c  
	- Game1.c  
	- Game2.c  
	- temperature_monitor.c  
	- gyro_monitor.c  
	- accel_monitor.c  
	- textlcddrv.h  

2. 실행파일 (bitmap파일 부터 키트 제조사 제공)
	- menu
	- Game1
	- Game2
	- temperature_monitor
	- gyro_monitor
	- accel_monitor
	- bitmap
	- ledtest 
	- buttontest (기존파일 수정 후 컴파일)
	- fndtest (기존파일 수정 후 컴파일)
	- textlcdtest 
	- readGyrotest.sh  
	- readAcceltest.sh  

3. 디바이스 드라이버 (키트 제조사 제공)
	- leddrv.ko
	- buttondrv.ko
	- textlcddrv.ko
	- fnddrv.ko

4. 비트맵 이미지
	- start.bmp
	- stop.bmp
	- go.bmp
	- left.bmp
	- right.bmp
	- break.bmp
	- end.bmp

### 2. 우분투에서 소스 코드 컴파일
- arm-linux-gnueabihf-gcc –o menu menu.c
- arm-linux-gnueabihf-gcc –o Game1 Game1.c
- arm-linux-gnueabihf-gcc –o Game2 Game2.c –lm // math.h 인클루드 에러 방지
- arm-linux-gnueabihf-gcc –o temperature_monitor temperature_monitor.c
- arm-linux-gnueabihf-gcc –o gyro_monitor gyro_monitor.c
- arm-linux-gnueabihf-gcc –o accel_monitor accel_monitor.c

### 3. 디바이스 드라이버, 실행파일, 비트맵 이미지 타켓에 전송
- sudo minicom
	- zmodem

### 4. 디바이스 드라이버 적재

1. LED
	- sudo insmod leddrv.ko
	- sudo chmod 666 /dev/periled

2. FND
	- sudo insmod fnddrv.ko
	- sudo chmod 666 /dev/perifnd

3. 온도센서 spi dd
	- sudo chmod 666 /dev/spidev1.0 
 
4. text lcd
	- sudo insmod textlcddrv.ko
	- sudo chmod 666 /dev/peritextlcd

5. button
	- sudo insmod buttondrv.ko
	- lsmod

### 5. 입력 장치 확인
- cat /proc/bus/input/devices
	- event0 – st1232-touchscreen
	- event1 – FreescaleAccelerometer
	- event2 – FreescaleMagnetometer
	- event3 – FreescaleGyroscope
	- enent4 – ecube-button 

- 가속도와 자이로 센서가 인식되어야 하고 ecube-button은 envet4로 인식되어야 함

### 6. 실행
- sudo ./menu


