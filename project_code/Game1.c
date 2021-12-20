#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <pthread.h>

#define INPUT_DEVICE_LIST "/dev/input/event"
#define  SYS_PATH	"S: Sysfs="
#define  BUTTON_NAME	"ecube-button"
#define  LINE_FEED	0x0A
#define  MAX_BUFF	200


pthread_t tid;
pthread_mutex_t lock;
int round_num, status,Timer_stat;
int LED_SET[20];

void Setfnd(int num){
    char str[40];
    sprintf(str,"./fndtest c %d",num);
    system(str);
}

void textLCD(int line, char* str){
    char Str_input[40];
    sprintf(Str_input,"./textlcdtest %d '%s'",line,str);
    system(Str_input);
}

void rand_Set(int round_num){
    int i, rand_num;
    for(i=0; i<=round_num; i++){
        rand_num=rand();
        LED_SET[i]=rand_num-(rand_num/6)+1;
    }
}

void SetLED(int round_num){
    int i, j, mod[2];
    float _delay;
    char str[40];
    srand(time(NULL));
    _delay = (round_num<3)? 1000000 :
              (round_num<5)? 800000 : 500000;
    system("./ledtest 0xC0");
    usleep(300000);
    system("./ledtest 0x00");
    usleep(500000);
    system("./ledtest 0xC0");
    Setfnd(300);
    system("./ledtest 0x00");
    usleep(500000);

    for(i=0; i<=round_num-1; i++){
        char ledpoint[2];
        LED_SET[i]=(rand()%6)+1;
        printf("LED_SET : %d\n",LED_SET[i]);
        mod[0] = (1<<(LED_SET[i]-1))%16;
        mod[1] = (((1<<(LED_SET[i]-1))-mod[0])/16)%16;
        if(mod[0]>10) mod[0]=mod[0]-10 + 65;
        else mod[0]=mod[0] + 48;
        if(mod[1]>10) mod[1]=mod[1]-10 + 65;
        else mod[1]=mod[1] + 48;
        ledpoint[0] = mod[0];
        ledpoint[1] = mod[1];
        sprintf(str,"./ledtest %c%c",ledpoint[1],ledpoint[0]);
        system(str);
        usleep(_delay);
        system("./ledtest 0");
    }
}

void* Timer(void *arg){
    sleep(5);
    if(status==0) {
        pthread_mutex_lock(&lock);
        round_num=1;
        pthread_mutex_unlock(&lock);
        status=0;
        Timer_stat=0;
    }
    else{
        Timer_stat=1;
        status=0;
    }
    return NULL;
}

int Comp(int BUTTON_INPUT[round_num],int LED_SET[round_num]){
    int count, i;
    count=0;
    for(i=0; i<round_num;i++){
        printf("BTTN : %d\n", BUTTON_INPUT[i]);
        printf("LEDS : %d\n", LED_SET[i]);
    }
    for(i=0; i<round_num; i++){
        if(BUTTON_INPUT[i]==LED_SET[i]) count ++;
    }
    if(count==round_num) return 1;
    else return 0;
}

int main(void){
    int BUTTON_INPUT[20];
    Timer_stat=1;
    round_num=1;
    int    fp, key_set, i;
	char 	inputfileName[MAX_BUFF+1];
	int		readSize;
	struct input_event  stEvent;

    sprintf(inputfileName,"%s%d",INPUT_DEVICE_LIST,4);
	fp = open(inputfileName, O_RDONLY);
    while(1){
        char roundMSG[10];
        sprintf(roundMSG,"ROUND : %d",round_num);
        textLCD(1,roundMSG);
        key_set = 1;
        SetLED(round_num);
        Setfnd(300);
        //pthread_create(&tid,NULL,&Timer,NULL);
        while(key_set<=round_num){
            readSize = read(fp, &stEvent , sizeof(stEvent));
            if (readSize != sizeof(stEvent)){
                continue;
            }
            if (stEvent.type == EV_KEY){
                switch(stEvent.code){
                    case KEY_HOME:
                    BUTTON_INPUT[key_set-1] = 1;
                    break;
                    case KEY_SEARCH:
                    BUTTON_INPUT[key_set-1] = 3;
                    break;
                    case KEY_BACK:
                    BUTTON_INPUT[key_set-1] = 2;
                    break;
                    case KEY_MENU:
                    BUTTON_INPUT[key_set-1] = 4;
                    break;
                    case KEY_VOLUMEUP:
                    BUTTON_INPUT[key_set-1] = 5;
                    break;
                    case KEY_VOLUMEDOWN:
                    BUTTON_INPUT[key_set-1] = 6;
                    break;
                }
                if ( stEvent.value ){
                        key_set ++;
                        printf("button input : %d\n", BUTTON_INPUT[key_set-1]);     
                }
            }
            else{}
        }
        if(Comp(BUTTON_INPUT,LED_SET)){
            status=1;
            textLCD(2,"CONGRATUATION!");
            usleep(500000);
            textLCD(2,"NEXT LEVEL");
            round_num ++;
            
        }
        else{
            char roundMSG[10];
            textLCD(2,"BOOM...");
            usleep(2000000);
            textLCD(1,"");
            textLCD(2,"");
            execl("./menu", "./menu", NULL);
            return 0;
        }
        //pthread_join(tid,NULL);
        if(round_num==10) 
        {
            execl("./menu", "./menu", NULL);
            return 0;
        }
    }
	close(fp);		
    return 0;
}