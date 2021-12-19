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

###

