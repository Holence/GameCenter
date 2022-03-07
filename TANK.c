#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <conio.h>
#include <time.h>
#include <mmsystem.h>
#include "mapeditor.c"
#include "MAP.c"
#include"function.h"
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
FILE *fp;
short tankcheat;
short AIrandomrate,AIlastingtime,enemyshootingrate;
int MAP[6][41][83]; 
short enemyleft,enemyalive,playerleft,resurrecting;
short end;
short x,y;
short *pstack;
short stack[10000];

typedef struct{
		short done;
		short directionback;
	}NODE;
	
typedef struct{
	short x,y;
	short direction;
	short exsist;
}BULLET;

typedef struct{
	short x,y;
	short color;
	short direction;
	short exsist;
	short startAI;
	long int interval;
	double time;
	int stop;//
	short stack[10000];//å­˜å‚¨AIè·¯çº¿
	short *p;
	NODE node[41][83];
	BULLET bullet;
//	short wait;
}TANK;
TANK player,enemy[5];

typedef struct{
	char tag[20];
	double time;
}RECORD;
RECORD list[1000],temp;









void record_addnew(){
	cursor(1);
	fp=fopen("record.dat","a+");
	fseek(fp,0,2);
	gotoxy(20,30);
	scanf("%s",temp.tag);
	temp.time=player.time;
	fwrite(&temp,sizeof(RECORD),1,fp);
	fclose(fp);
	cursor(0);
}
void record_rank(){
	fp=fopen("record.dat","r");
	int n,i=0,j,min;
	rewind(fp);
	while(!feof(fp)){
		fread(&(list[i++]),sizeof(RECORD),1,fp);
	}
	n=i-1;
	for(i=0;i<n;i++){
		min=i;
		for(j=i+1;j<n;j++){
			if(list[min].time>list[j].time) min=j;
		}
		temp=list[i];list[i]=list[min];list[min]=temp;
	}
	fclose(fp);
}

void record_list(){
	gotoxy(48,10);printf("TOP 20");
	gotoxy(0,15);
	for(j=0;j<20;j++) if(list[j].time!=0){printf("          %d.%-60s",j+1,list[j].tag);printf("%10.1fs\n",list[j].time);}
}

void mycls(){
	int i,j;
	for(i=0;i<60;i+=1)
				for(j=0;j<120;j+=10) {
					COORD coord;
					coord.X=j;//Xæ˜¯å±å¹•ä¸Šä»Žå·¦å¾€å³æ¨ªç€æ•°  
					coord.Y=i;//Yæ˜¯ä»Žä¸Šå¾€ä¸‹ç«–ç€æ•° 
					SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
					printf("          ");
				}
	return;
}

/*void SetSize(short x, short y){  // è®¾ç½®åŒæ ·çš„ç¼“å†²åŒºå’Œçª—å£å¤§å°ï¼Œä¸ä¼šå‡ºçŽ°æ»šåŠ¨æ¡
	HANDLE hOut=GetStdHandle(STD_OUTPUT_HANDLE);
   	COORD pos = {x, y};
   	SMALL_RECT rc = {0, 0, x-1, y-1};//å·¦ä¸Šå³ä¸‹
   	SetConsoleWindowInfo(hOut, TRUE, &rc);
   	SetConsoleScreenBufferSize(hOut, pos);
}*/



void colorfulprint(short x,short y,char a,int color){
	coord.X=x;
	coord.Y=y;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),color);
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
	printf("%c",a);
	return;
}


void clear(short x,short y){
	print(x,y,' ');
}

























void createnemybullet(TANK *q){
	switch(q->direction){
		case UP:{
			(*q).bullet.x=q->x;(*q).bullet.y=q->y-1;
			(*q).bullet.direction=UP;
			(*q).bullet.exsist=1;
			break;
		}
		case DOWN:{
			(*q).bullet.x=q->x;(*q).bullet.y=q->y+1;
			(*q).bullet.direction=DOWN;
			(*q).bullet.exsist=1;
			break;
		}
		case LEFT:{
			(*q).bullet.x=q->x-1;(*q).bullet.y=q->y;
			(*q).bullet.direction=LEFT;
			(*q).bullet.exsist=1;
			break;
		}
		case RIGHT:{
			(*q).bullet.x=q->x+1;(*q).bullet.y=q->y;
			(*q).bullet.direction=RIGHT;
			(*q).bullet.exsist=1;
			break;
		}
	}
}




void creatplayerbullet(){
	switch(player.direction){
		case UP:{
			player.bullet.x=player.x;player.bullet.y=player.y-1;
			player.bullet.direction=UP;
			player.bullet.exsist=1;
			break;
		}
		case DOWN:{
			player.bullet.x=player.x;player.bullet.y=player.y+1;
			player.bullet.direction=DOWN;
			player.bullet.exsist=1;
			break;
		}
		case LEFT:{
			player.bullet.x=player.x-1;player.bullet.y=player.y;
			player.bullet.direction=LEFT;
			player.bullet.exsist=1;
			break;
		}
		case RIGHT:{
			player.bullet.x=player.x+1;player.bullet.y=player.y;
			player.bullet.direction=RIGHT;
			player.bullet.exsist=1;
			break;
		}
	}
}



void clearenemywhole(TANK *q){
	switch(q->direction){
		case UP:{
			clear(q->x,q->y);
			clear(q->x-1,q->y);
			clear(q->x+1,q->y);
			clear(q->x,q->y-1);
			break;
		}
		case DOWN:{
			clear(q->x,q->y);
			clear(q->x-1,q->y);
			clear(q->x+1,q->y);
			clear(q->x,q->y+1);
			break;
		}
		case LEFT:{
			clear(q->x,q->y-1);
			clear(q->x,q->y);
			clear(q->x,q->y+1);
			clear(q->x-1,q->y);
			break;
		}
		case RIGHT:{
			clear(q->x,q->y-1);
			clear(q->x,q->y);
			clear(q->x,q->y+1);
			clear(q->x+1,q->y);
			break;
		}
	}
}













// void chishi(){//è°ƒè¯•ç”¨
// 	int a=40;int *p=stack;
// 	gotoxy(30,20);
// 	printf("%d",pstack-stack);
// 	gotoxy(30,10);
// 	while(a--) printf("%d",*(p++));
// 	system("pause");
// }



int checkaround(short n){
	if((MAP[n][y][x]==0||MAP[n][y][x]==1)&&(MAP[n][y][x-1]==0||MAP[n][y][x-1]==1)&&(MAP[n][y][x+1]==0||MAP[n][y][x+1]==1)&& (MAP[n][y-1][x]==0||MAP[n][y-1][x]==1)&&(MAP[n][y-1][x-1]==0||MAP[n][y-1][x-1]==1)&&(MAP[n][y-1][x+1]==0||MAP[n][y-1][x+1]==1)&&  (MAP[n][y+1][x]==0||MAP[n][y+1][x]==1)&&(MAP[n][y+1][x-1]==0||MAP[n][y+1][x-1]==1)&&(MAP[n][y+1][x+1]==0||MAP[n][y+1][x+1]==1)) return 1;
	else return 0;
}

void normalAImovedown(short n,TANK* q){//ä¼˜å…ˆå‘ä¸‹çš„æœç´¢
	
	if(checkaround(n)&&(*q).node[y+1][x].done==0){(*q).node[y][x].done=1;*pstack=DOWN;pstack++; y++;(*q).node[y][x].done=1;(*q).node[y][x].directionback=UP;return;}
	if(checkaround(n)&&(*q).node[y][x+1].done==0){(*q).node[y][x].done=1;*pstack=RIGHT;pstack++; x++;(*q).node[y][x].done=1;(*q).node[y][x].directionback=LEFT;return;}//right
	if(checkaround(n)&&(*q).node[y][x-1].done==0){(*q).node[y][x].done=1;*pstack=LEFT;pstack++; x--;(*q).node[y][x].done=1;(*q).node[y][x].directionback=RIGHT;return;}
	if(checkaround(n)&&(*q).node[y-1][x].done==0){(*q).node[y][x].done=1;*pstack=UP;pstack++; y--;(*q).node[y][x].done=1;(*q).node[y][x].directionback=DOWN;return;}
	//ÃŽÃžÃ‚Â·Â¿Ã‰Ã—ÃŸ
	else {
		switch((*q).node[y][x].directionback){
			case LEFT:{pstack--;    x--;break;}
			case UP:{pstack--;    y--;break;}
			case DOWN:{pstack--;    y++;break;}
			case RIGHT:{pstack--;    x++;break;}
		}
	}
}

int findpath(short n,TANK *q){//æ¯ä¸ªAIç®—çš„æ—¶å€™åŠ¨ç”¨å…¨å±€å˜é‡xyè®¡ç®—AIè·¯å¾„
	memset(stack,0,sizeof(stack));
	int t;
	pstack=stack;
	x=(*q).x;y=(*q).y;
	if(x<41) t=37;
	else t=45;
	while((*q).node[38][t].done!=1){
		normalAImovedown(n,q);
		if(pstack-stack<0) return -1; 
	}
	return pstack-stack;
}











int directioncheck(short n,TANK *q){//æ ¹æ®ä¸Šä¸‹å·¦å³çš„ç©ºæ ¼å­çš„å¤šå°‘é€‰æ‹©å‡ºç©ºæ ¼æœ€å¤šçš„æ–¹å‘
	int a[4]={1,1,1,1};//ä¸Šä¸‹å·¦å³
	int b[4]={1,1,1,1};
	int *pa=a,max,runlinma=0;
	int x=q->x,y=q->y;
	switch(q->direction){
		case UP:{
			a[0]=0;
			for(int i=2;i<4;i++){
				if(b[1]==1) {
					if(MAP[n][y+i][x]==0) a[1]++;
					else b[1]=0;
				}
				if(b[2]==1) {
					if(MAP[n][y][x-i]==0) a[2]++;
					else b[2]=0;
				}
				if(b[3]==1) {
					if(MAP[n][y][x+i]==0) a[3]++;
					else b[3]=0;
				}
			}
			max=*pa;
			pa++;
			for(int i=0;i<2;i++){
				if(*pa>max) {max=*pa;runlinma=pa-a;}
				pa++;
			}
			if(*pa>max) {max=*pa;runlinma=pa-a;}
			return runlinma;
		}
		case DOWN:{
			a[1]=0;
			for(int i=2;i<4;i++){
				if(b[0]==1) {
					if(MAP[n][y-i][x]==0) a[0]++;
					else b[0]=0;
				}
				if(b[2]==1) {
					if(MAP[n][y][x-i]==0) a[2]++;
					else b[2]=0;
				}
				if(b[3]==1) {
					if(MAP[n][y][x+i]==0) a[3]++;
					else b[3]=0;
				}
			}
			max=*pa;
			pa++;
			for(int i=0;i<2;i++){
				if(*pa>max) {max=*pa;runlinma=pa-a;}
				pa++;
			}
			if(*pa>max) {max=*pa;runlinma=pa-a;}
			return runlinma;
		}
		case LEFT:{
			a[2]=0;
			for(int i=2;i<4;i++){
				if(b[1]==1) {
					if(MAP[n][y+i][x]==0) a[1]++;
					else b[1]=0;
				}
				if(b[0]==1) {
					if(MAP[n][y-i][x]==0) a[0]++;
					else b[0]=0;
				}
				if(b[3]==1) {
					if(MAP[n][y][x+i]==0) a[3]++;
					else b[3]=0;
				}
			}
			max=*pa;
			pa++;
			for(int i=0;i<2;i++){
				if(*pa>max) {max=*pa;runlinma=pa-a;}
				pa++;
			}
			if(*pa>max) {max=*pa;runlinma=pa-a;}
			return runlinma;
		}
		case RIGHT:{
			a[3]=0;
			for(int i=2;i<4;i++){
				if(b[1]==1) {
					if(MAP[n][y+i][x]==0) a[1]++;
					else b[1]=0;
				}
				if(b[2]==1) {
					if(MAP[n][y][x-i]==0) a[2]++;
					else b[2]=0;
				}
				if(b[0]==1) {
					if(MAP[n][y][x+i]==0) a[0]++;
					else b[0]=0;
				}
			}
			max=*pa;
			pa++;
			for(int i=0;i<2;i++){
				if(*pa>max) {max=*pa;runlinma=pa-a;}
				pa++;
			}
			if(*pa>max) {max=*pa;runlinma=pa-a;}
			return runlinma;
		}
	}
}

int checkhitwall(int n,int x,int y,int i,int j){
	if(MAP[n][y+j][x+i]==1){
		if(i==0){
		if(MAP[n][y+j][x-1]==1) {clear(x-1,y+j);MAP[n][y+j][x-1]=0;}
		clear(x,y+j);MAP[n][y+j][x]=0;
		if(MAP[n][y+j][x+1]==1) {clear(x+1,y+j);MAP[n][y+j][x+1]=0;}
		}
		else{
			if(MAP[n][y+1][x+i]==1) {clear(x+i,y+1);MAP[n][y+1][x+i]=0;}
		clear(x+i,y);MAP[n][y][x+i]=0;
		if(MAP[n][y-1][x+i]==1) {clear(x+i,y-1);MAP[n][y-1][x+i]=0;}
		}
		return 1;
	}
	return 0;
}

int checkbarrier(int n,int x,int y,int i,int j){
	if(MAP[n][y+j][x+i]==2) {
		return 1;
	}
	return 0;
}

int checkhitenemy(int x,int y,int i,int j){//å…¨åˆ¤å®š
	void clearenemywhole();
	TANK *q=enemy;
	for(int o=0;o<5;o++,q++){
		if(q->exsist){
			switch(q->direction){
				case UP:{
					if((x+i==q->x&&y+j==q->y)||(x+i==q->x-1&&y+j==q->y)||(x+i==q->x+1&&y+j==q->y)||(x+i==q->x&&y+j==q->y-1)) {PlaySound("boom.wav", NULL, SND_FILENAME | SND_ASYNC);q->exsist=0;q->startAI=0;clear((*q).bullet.x,(*q).bullet.y);(*q).bullet.exsist=0;clearenemywhole(q);enemyalive--;return 1;}
				}
				case DOWN:{
					if((x+i==q->x&&y+j==q->y)||(x+i==q->x-1&&y+j==q->y)||(x+i==q->x+1&&y+j==q->y)||(x+i==q->x&&y+j==q->y+1)) {PlaySound("boom.wav", NULL, SND_FILENAME | SND_ASYNC);q->exsist=0;q->startAI=0;clear((*q).bullet.x,(*q).bullet.y);(*q).bullet.exsist=0;clearenemywhole(q);enemyalive--;return 1;}
				}
				case LEFT:{
					if((x+i==q->x&&y+j==q->y)||(x+i==q->x&&y+j==q->y+1)||(x+i==q->x&&y+j==q->y-1)||(x+i==q->x-1&&y+j==q->y)) {PlaySound("boom.wav", NULL, SND_FILENAME | SND_ASYNC);q->exsist=0;q->startAI=0;clear((*q).bullet.x,(*q).bullet.y);(*q).bullet.exsist=0;clearenemywhole(q);enemyalive--;return 1;}
				}
				case RIGHT:{
					if((x+i==q->x&&y+j==q->y)||(x+i==q->x&&y+j==q->y+1)||(x+i==q->x&&y+j==q->y-1)||(x+i==q->x+1&&y+j==q->y)) {PlaySound("boom.wav", NULL, SND_FILENAME | SND_ASYNC);q->exsist=0;q->startAI=0;clear((*q).bullet.x,(*q).bullet.y);(*q).bullet.exsist=0;clearenemywhole(q);enemyalive--;return 1;}
				}
			}
		}
	}
	return 0;
}

int checkhitplayer(int x,int y,int i,int j){//å…¨åˆ¤å®š
	void clearplayerwhole();
	if(resurrecting==0&&tankcheat==0){
		switch(player.direction){
			case UP:{
				if((x+i==player.x&&y+j==player.y)||(x+i==player.x-1&&y+j==player.y)||(x+i==player.x+1&&y+j==player.y)||(x+i==player.x&&y+j==player.y-1)) {PlaySound("boom.wav", NULL, SND_FILENAME | SND_ASYNC);clear(x,y);playerleft--; player.exsist=0;clearplayerwhole();return 1;}
			}
			case DOWN:{
				if((x+i==player.x&&y+j==player.y)||(x+i==player.x-1&&y+j==player.y)||(x+i==player.x+1&&y+j==player.y)||(x+i==player.x&&y+j==player.y+1)) {PlaySound("boom.wav", NULL, SND_FILENAME | SND_ASYNC);clear(x,y);playerleft--; player.exsist=0;clearplayerwhole();return 1;}
			}
			case LEFT:{
				if((x+i==player.x&&y+j==player.y)||(x+i==player.x&&y+j==player.y+1)||(x+i==player.x&&y+j==player.y-1)||(x+i==player.x-1&&y+j==player.y)) {PlaySound("boom.wav", NULL, SND_FILENAME | SND_ASYNC);clear(x,y);playerleft--; player.exsist=0;clearplayerwhole();return 1;}
			}
			case RIGHT:{
				if((x+i==player.x&&y+j==player.y)||(x+i==player.x&&y+j==player.y+1)||(x+i==player.x&&y+j==player.y-1)||(x+i==player.x+1&&y+j==player.y)) {PlaySound("boom.wav", NULL, SND_FILENAME | SND_ASYNC);clear(x,y);playerleft--; player.exsist=0;clearplayerwhole();return 1;}
			}
		}
	}
	return 0;
}

int checkhitbullet(int direction,int i){//å¯¹æ’žæ¶ˆå¼¹ï¼Œå­å¼¹æ“¦å¼¹ä¹Ÿæ¶ˆå¼¹ï¼ˆçºµå‘æˆ–æ¨ªå‘è·ç¦»ä¸€æ ¼ï¼‰
	TANK *q=enemy;
	switch(direction){
		case UP:{
			for(int o=0;o<5;q++,o++){
				if((*q).bullet.exsist==1&&(*q).bullet.direction==DOWN){
					if(player.bullet.y+2*i<(*q).bullet.y&&abs(player.bullet.x-(*q).bullet.x)<4) {player.bullet.exsist=0;(*q).bullet.exsist=0;clear((*q).bullet.x,(*q).bullet.y);return 1;}
				}
			}
			break;
		}
		case DOWN:{
			for(int o=0;o<5;q++,o++){
				if((*q).bullet.exsist==1&&(*q).bullet.direction==UP){
					if(player.bullet.y+2*i>(*q).bullet.y&&abs(player.bullet.x-(*q).bullet.x)<4) {player.bullet.exsist=0;(*q).bullet.exsist=0;clear((*q).bullet.x,(*q).bullet.y);return 1;}
				}
			}
			break;
		}
		case LEFT:{
			for(int o=0;o<5;q++,o++){
				if((*q).bullet.exsist==1&&(*q).bullet.direction==RIGHT){
					if(player.bullet.x+2*i<(*q).bullet.x&&abs(player.bullet.y-(*q).bullet.y)<4) {player.bullet.exsist=0;(*q).bullet.exsist=0;clear((*q).bullet.x,(*q).bullet.y);return 1;}
				}
			}
			break;
		}
		case RIGHT:{
			for(int o=0;o<5;q++,o++){
				if((*q).bullet.exsist==1&&(*q).bullet.direction==LEFT){
					if(player.bullet.x+2*i>(*q).bullet.x&&abs(player.bullet.y-(*q).bullet.y)<4) {player.bullet.exsist=0;(*q).bullet.exsist=0;clear((*q).bullet.x,(*q).bullet.y);return 1;}
				}
			}
			break;
		}
	}
	return 0;
}

void bulletfly(int n){//æ¯å¸§æ¨ªå‘èµ°6æ ¼ï¼Œçºµå‘èµ°3ä¸ªã€‚
	//è¿™æ˜¯çŽ©å®¶çš„å­å¼¹
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),6);
		if(player.bullet.exsist){
			if(resurrecting==1) color(4);
			else color(6);
			int x=player.bullet.x,y=player.bullet.y;
			switch(player.bullet.direction){
				case UP:{
					if(y!=player.y-1) clear(x,y);//åœ¨æžªå£å¤„ä¸æ‰“å°,å…¶ä»–æ—¶å€™æ¶ˆé™¤å‰ä¸€æ¬¡çš„ç—•è¿¹
					int i;
					for(i=1;i<=3;i++){
						if(checkhitbullet(UP,-i)) break;
						if(checkhitenemy(x,y,0,-i)) {player.bullet.exsist=0;break;}
						if(checkhitwall(n,x,y,0,-i)) {player.bullet.exsist=0;break;}
						if(checkbarrier(n,x,y,0,-i)) {player.bullet.exsist=0;break;}
					}
						if(y-3<=0) {player.bullet.exsist=0;break;}
					if(player.bullet.exsist==1){player.bullet.y-=3;print(x,player.bullet.y,'o');}
					break;
				}
				case DOWN:{
					if(y!=player.y+1) clear(x,y);//åœ¨æžªå£å¤„ä¸æ‰“å°
					int i;
					for(i=1;i<=3;i++){
						if(checkhitbullet(DOWN,i)) break;
						if(checkhitenemy(x,y,0,i)) {player.bullet.exsist=0;break;}
						if(checkhitwall(n,x,y,0,i)) {player.bullet.exsist=0;break;}
						if(checkbarrier(n,x,y,0,i)) {player.bullet.exsist=0;break;}
					}
						if(y+3>=40) {player.bullet.exsist=0;break;}
					if(player.bullet.exsist==1){player.bullet.y+=3;print(x,player.bullet.y,'o');}
					break;
				}
				case LEFT:{
					if(x!=player.x-1) clear(x,y);//åœ¨æžªå£å¤„ä¸æ‰“å°
					int i;
					for(i=1;i<=6;i++){
						if(checkhitbullet(LEFT,-i)) break;
						if(checkhitenemy(x,y,-i,0)) {player.bullet.exsist=0;break;}
						if(checkhitwall(n,x,y,-i,0)) {player.bullet.exsist=0;break;}
						if(checkbarrier(n,x,y,-i,0)) {player.bullet.exsist=0;break;}
					}
						if(x-6<=0) {player.bullet.exsist=0;break;}
					if(player.bullet.exsist==1){player.bullet.x-=6;print(player.bullet.x,y,'o');}
					break;
				}
				case RIGHT:{
					if(x!=player.x+1) clear(x,y);//åœ¨æžªå£å¤„ä¸æ‰“å°
					int i;
					for(i=1;i<=6;i++){
						if(checkhitbullet(RIGHT,i)) break;
						if(checkhitenemy(x,y,i,0)) {player.bullet.exsist=0;break;}
						if(checkhitwall(n,x,y,i,0)) {player.bullet.exsist=0;break;}
						if(checkbarrier(n,x,y,i,0)) {player.bullet.exsist=0;break;}
					}
						if(x+6>=82) {player.bullet.exsist=0;break;}
					if(player.bullet.exsist==1){player.bullet.x+=6;print(player.bullet.x,y,'o');}
					break;
				}
				
			}
		}
		
	

	//AIçš„å­å¼¹
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),11);
	TANK *q;
	q=enemy;
	for(int i=0;i<5;i++,q++){//è¿™æ˜¯enemyçš„å­å¼¹
		if((*q).bullet.exsist){
					color(q->color);
					int x=(*q).bullet.x,y=(*q).bullet.y;
					switch((*q).bullet.direction){
						case UP:{
							if(y!=q->y-1) clear(x,y);//åœ¨æžªå£å¤„ä¸æ‰“å°
							int i;
							for(i=1;i<=3;i++){
								if(checkhitplayer(x,y,0,-i)) {(*q).bullet.exsist=0;break;}
								if(checkhitwall(n,x,y,0,-i)) {(*q).bullet.exsist=0;break;}
								if(checkbarrier(n,x,y,0,-i)) {(*q).bullet.exsist=0;break;}
							}
								if(y-3<=0) {(*q).bullet.exsist=0;break;}
							if((*q).bullet.exsist==1) {(*q).bullet.y-=3;print(x,(*q).bullet.y,'o');}
							break;
						}
						case DOWN:{
							if(y!=q->y+1) clear(x,y);//åœ¨æžªå£å¤„ä¸æ‰“å°
							int i;
							for(i=1;i<=3;i++){
								if(checkhitplayer(x,y,0,+i)) {(*q).bullet.exsist=0;break;}
								if(checkhitwall(n,x,y,0,i)) {(*q).bullet.exsist=0;break;}
								if(checkbarrier(n,x,y,0,i)) {(*q).bullet.exsist=0;break;}
							}
								if(y+3>=40) {(*q).bullet.exsist=0;break;}
							if((*q).bullet.exsist==1){(*q).bullet.y+=3;print(x,(*q).bullet.y,'o');}
							break;
						}
						case LEFT:{
							if(x!=q->x-1) clear(x,y);//åœ¨æžªå£å¤„ä¸æ‰“å°
							int i;
							for(i=1;i<=6;i++){
								if(checkhitplayer(x,y,-i,0)) {(*q).bullet.exsist=0;break;}
								if(checkhitwall(n,x,y,-i,0)) {(*q).bullet.exsist=0;break;}
								if(checkbarrier(n,x,y,-i,0)) {(*q).bullet.exsist=0;break;}
							}
								if(x-6<=0) {(*q).bullet.exsist=0;break;}
							if((*q).bullet.exsist==1){(*q).bullet.x-=6;print((*q).bullet.x,y,'o');}
							break;
						}
						case RIGHT:{
							if(x!=q->x+1) clear(x,y);//åœ¨æžªå£å¤„ä¸æ‰“å°
							int i;
							for(i=1;i<=6;i++){
								if(checkhitplayer(x,y,i,0)) {(*q).bullet.exsist=0;break;}
								if(checkhitwall(n,x,y,i,0)) {(*q).bullet.exsist=0;break;}
								if(checkbarrier(n,x,y,i,0)) {(*q).bullet.exsist=0;break;}
							}
								if(x+6>=82) {(*q).bullet.exsist=0;break;}
							if((*q).bullet.exsist==1){(*q).bullet.x+=6;print((*q).bullet.x,y,'o');}
							break;
						}
						
					}
		}
	}
		
}












void clearplayerwhole(){
	switch(player.direction){
		case UP:{
			clear(player.x,player.y);
			clear(player.x-1,player.y);
			clear(player.x+1,player.y);
			clear(player.x,player.y-1);
			break;
		}
		case DOWN:{
			clear(player.x,player.y);
			clear(player.x-1,player.y);
			clear(player.x+1,player.y);
			clear(player.x,player.y+1);
			break;
		}
		case LEFT:{
			clear(player.x,player.y-1);
			clear(player.x,player.y);
			clear(player.x,player.y+1);
			clear(player.x-1,player.y);
			break;
		}
		case RIGHT:{
			clear(player.x,player.y-1);
			clear(player.x,player.y);
			clear(player.x,player.y+1);
			clear(player.x+1,player.y);
			break;
		}
	}
}

void clearenemy(TANK *q){
	switch(q->direction){
		case UP:{
			clear(q->x,q->y);
			clear(q->x+1,q->y);
			clear(q->x-1,q->y);
			clear(q->x,q->y+1);
			clear(q->x+1,q->y+1);
			clear(q->x-1,q->y+1);
			break;
		}
		case DOWN:{
			clear(q->x,q->y);
			clear(q->x+1,q->y);
			clear(q->x-1,q->y);
			clear(q->x,q->y-1);
			clear(q->x+1,q->y-1);
			clear(q->x-1,q->y-1);
			break;
		}
		case LEFT:{
			clear(q->x,q->y-1);
			clear(q->x,q->y);
			clear(q->x,q->y+1);
			clear(q->x+1,q->y-1);
			clear(q->x+1,q->y);
			clear(q->x+1,q->y+1);
			clear(q->x-1,q->y-1);
			clear(q->x-1,q->y);
			clear(q->x-1,q->y+1);
			break;
		}
		case RIGHT:{
			clear(q->x,q->y-1);
			clear(q->x,q->y);
			clear(q->x,q->y+1);
			clear(q->x+1,q->y-1);
			clear(q->x+1,q->y);
			clear(q->x+1,q->y+1);
			clear(q->x-1,q->y-1);
			clear(q->x-1,q->y);
			clear(q->x-1,q->y+1);
			break;
		}
	}
}

void printenemy(TANK *q){
	color(q->color);
	switch(q->direction){
		case UP:{
			print(q->x,q->y,'=');
			print(q->x+1,q->y,'*');
			print(q->x-1,q->y,'*');
			print(q->x,q->y-1,'!');
			break;
		}
		case DOWN:{
			print(q->x,q->y,'=');
			print(q->x+1,q->y,'*');
			print(q->x-1,q->y,'*');
			print(q->x,q->y+1,'i');
			break;
		}
		case LEFT:{
			print(q->x,q->y,'[');
			print(q->x,q->y-1,'*');
			print(q->x,q->y+1,'*');
			print(q->x-1,q->y,'<');
			break;
		}
		case RIGHT:{
			print(q->x,q->y,']');
			print(q->x,q->y-1,'*');
			print(q->x,q->y+1,'*');
			print(q->x+1,q->y,'>');
			break;	
		}
	}
}

void enemyshoot(TANK *q){//å¯¹å…¨ä½“æ•Œæœºè¿›è¡Œå°„å‡»è®¡ç®—
		if ((*q).bullet.exsist==0){ 
			switch(q->direction){
				case UP:{
					(*q).bullet.direction=UP;
					createnemybullet(q);
					return;
				}
				case LEFT:{
					(*q).bullet.direction=LEFT;
					createnemybullet(q);
					return;
				}
				case RIGHT:{
					(*q).bullet.direction=RIGHT;
					createnemybullet(q);
					return;
				}
				case DOWN:{
					(*q).bullet.direction=DOWN;
					createnemybullet(q);
					return;
				}
				
			}
			
		}
}



void AIenemymov(int n,TANK **q){//!!!!!!!!!!!!!!!!!!!!!!!!!!!!æ³¨æ„æ³¨æ„è‹¥è¦ä¿®æ”¹å¤–éƒ¨çš„å€¼ï¼Œåº”ä¼ å…¥åœ°å€ï¼Œè‹¥ä¼ å…¥çš„æ˜¯*qï¼Œé‚£*q->y++å°†æ— æ„ä¹‰ï¼ˆè‹¥åªæ˜¯å¼•ç”¨qçš„å±žæ€§ï¼Œé‚£å¯ä»¥ä¸ç”¨**qï¼‰
	int x=(*q)->x,y=(*q)->y;
	switch((*q)->direction){//æ ¹æ®æ–¹å‘ç§»åŠ¨
			case UP:{
					if(player.y-y<=3&&player.y-y>0&&abs(player.x-x)<2) return;
					if(MAP[n][y-2][x]!=0) return;
					clearenemy(*q);
					(*q)->y--;
					printenemy(*q);
					(*q)->direction=UP;
					break;
			
			}
			case DOWN:{
					if(y-player.y<=3&&y-player.y>0&&abs(player.x-x)<2) return;
					if(MAP[n][y+2][x]!=0) return;
					clearenemy(*q);
					(*q)->y++;
					printenemy(*q);
					(*q)->direction=DOWN;
					break;
			}
			case LEFT:{
	   			if(x-player.x<=3&&x-player.x>0&&abs(player.y-y)<2) return;
	   			if(MAP[n][y][x-2]!=0) return;
	    		clearenemy(*q);
	    		(*q)->x-=1;
				printenemy(*q);
				(*q)->direction=LEFT;
				break;
			}
			case RIGHT:{
				if(player.x-x<=3&&player.x-x>0&&abs(player.y-y)<2) return;
				if(MAP[n][y][x+2]!=0) return;
		   		clearenemy(*q);
		   		(*q)->x+=1;
				printenemy(*q);
				(*q)->direction=RIGHT;
	    		break;
			}
	}	
	((*q)->p)++;
}


void enemymov(int n){
	TANK *q=enemy;
	for(int i=0;i<5;i++,q++){
		if(q->exsist==1){
			int cx=q->x;int cy=q->y;//ä¸‹é¢å°†å¤šæ¬¡å¼•ç”¨å¦å…‹çš„åæ ‡ï¼Œæ•…ç”¨x,yè®¡ç®—å‡ºï¼Œæå‡æ•ˆçŽ‡
			// MAP[n][cy][cx]=0;//ä½“æœ¯åˆ¤å®šå…ˆå½’é›¶ï¼Œå‡½æ•°ç»“å°¾å¤„ä¼šèµ‹å€¼çš„ã€‚
			if(randomize(100)<enemyshootingrate) enemyshoot(q);


					if(randomize(100)<AIrandomrate&&q->startAI==0){//ä¸€å®šæ¦‚çŽ‡å¦å…‹å˜å¾—æ™ºèƒ½å¯»è·¯
							q->interval=clock();//ç¬¬ä¸€æ¬¡è¿›æ¥å¼€å§‹è®¡æ—¶
							memset(q->node,0,sizeof(q->node));
							memset(q->stack,0,sizeof(q->stack));
							if((q->stop=findpath(n,q))!=-1) {
								q->startAI=1;
								pstack=stack;
								q->p=q->stack;
								for(int i=0;i<q->stop;i++){
									*(q->p)=*pstack;
									(q->p)++;
									pstack++;
								}
								q->p=q->stack;
							};
							
					}
					if(q->startAI==1){
						if(clock()-q->interval<AIlastingtime){//æ¯«ç§’å†…å¼€å¯æ™ºèƒ½å¯»è·¯æ¨¡å¼
							// gotoxy(q->x,q->y);printf("i am runing" );
							q->direction=*(q->p);
							AIenemymov(n,&q);
							continue;
						}
						else q->startAI=0;
					}
					switch(q->direction){//ç¢°å£è½¬å‘
								case UP:{
									if(MAP[n][cy-2][cx]!=0||MAP[n][cy-2][cx-1]!=0||MAP[n][cy-2][cx+1]!=0) {if(randomize(100)<20) q->direction=directioncheck(n,q);else q->direction=randomize(3);}
									// if(rand()%3==0) q->wait=1;
									break;
								}
								case DOWN:{
									if(MAP[n][cy+2][cx]!=0||MAP[n][cy+2][cx-1]!=0||MAP[n][cy+2][cx+1]!=0) {if(randomize(100)<20) q->direction=directioncheck(n,q);else q->direction=randomize(3);}
									// if(rand()%3==0) q->wait=1;
									break;
								}
								case LEFT:{
									if(MAP[n][cy][cx-2]!=0||MAP[n][cy-1][cx-2]!=0||MAP[n][cy+1][cx-2]!=0) {if(randomize(100)<20) q->direction=directioncheck(n,q);else q->direction=randomize(3);}
									// if(rand()%3==0) q->wait=1;
									break;
								}
								case RIGHT:{
									if(MAP[n][cy][cx+2]!=0||MAP[n][cy-1][cx+2]!=0||MAP[n][cy+1][cx+2]!=0) {if(randomize(100)<20) q->direction=directioncheck(n,q);else q->direction=randomize(3);}
									// if(rand()%3==0) q->wait=1;
									break;
								}
					}
					switch(q->direction){//æ ¹æ®æ–¹å‘ç§»åŠ¨
							case UP:{
								if(q->direction!=UP){
									clearenemy(q);
									printenemy(q);
									q->direction=UP;
								}
								else{
									if(cy==2) break;//è¾¹ç•Œåˆ¤å®š
									if(MAP[n][cy-2][cx]==1) break;
									if(MAP[n][cy-2][cx-1]==1) break;
									if(MAP[n][cy-2][cx+1]==1) break;
									if(MAP[n][cy-2][cx]==2) break;
									if(MAP[n][cy-2][cx-1]==2) break;
									if(MAP[n][cy-2][cx+1]==2) break;

									if(player.y-q->y<=3&&player.y-q->y>0&&abs(player.x-q->x)<2) break;

									clearenemy(q);
									q->y--;
									printenemy(q);
									q->direction=UP;
								}
								break;
							}
							case DOWN:{
								
								if(q->direction!=DOWN){
									clearenemy(q);
									printenemy(q);
									q->direction=DOWN;
								}
								else{
									if(cy==38) break;
									if(MAP[n][cy+2][cx-1]==1) break;
									if(MAP[n][cy+2][cx]==1) break;
									if(MAP[n][cy+2][cx+1]==1) break;
									if(MAP[n][cy+2][cx-1]==2) break;
									if(MAP[n][cy+2][cx]==2) break;
									if(MAP[n][cy+2][cx+1]==2) break;

									if(q->y-player.y<=3&&q->y-player.y>0&&abs(player.x-q->x)<2) break;
									
									clearenemy(q);
									q->y++;
									printenemy(q);
									q->direction=DOWN;
								}
								break;
							}
							case LEFT:{
			    	    		if(q->direction!=LEFT){
			    	    			clearenemy(q);
			    	    			printenemy(q);
			    	    			q->direction=LEFT;
			    	    		}
			    	    		else{
			    	    			if(cx==2) break;
			    	    			if(cx==3){
			    	    				if(MAP[n][cy][cx-2]==2||MAP[n][cy+1][cx-2]==2||MAP[n][cy-1][cx-2]==2||MAP[n][cy][cx-2]==1||MAP[n][cy+1][cx-2]==1||MAP[n][cy-1][cx-2]==1) break;
			    	    				else{
			    	    					clearenemy(q);
			    	    				q->x-=1;
										printenemy(q);
										q->direction=LEFT;
										break;
			    	    				}
			    	    			}
			    	    			if(MAP[n][cy][cx-2]==2||MAP[n][cy+1][cx-2]==2||MAP[n][cy-1][cx-2]==2||MAP[n][cy][cx-2]==1||MAP[n][cy+1][cx-2]==1||MAP[n][cy-1][cx-2]==1) break;

			    	    			if(q->x-player.x<=3&&q->x-player.x>0&&abs(player.y-q->y)<2) break;
									
			    	    			if(MAP[n][cy][cx-3]==2||MAP[n][cy+1][cx-3]==2||MAP[n][cy-1][cx-3]==2||MAP[n][cy][cx-3]==1||MAP[n][cy+1][cx-3]==1||MAP[n][cy-1][cx-3]==1) {
			    	    				clearenemy(q);
			    	    				q->x-=1;
										printenemy(q);
										q->direction=LEFT;
			    	    			}
			    	    			else{
			    	    				clearenemy(q);
			    	    				q->x-=2;
										printenemy(q);
										q->direction=LEFT;
									}
			    	    		}
			    	    		break;
							}
							case RIGHT:{
								if(q->direction!=RIGHT){
			    	    			clearenemy(q);
			    	    			printenemy(q);
			    	    			q->direction=RIGHT;
			    	    		}
			    	    		else{
			    	    			if(cx==80) break;
			    	    			if(cx==79){
			    	    				if(MAP[n][cy][cx+2]==2||MAP[n][cy+1][cx+2]==2||MAP[n][cy-1][cx+2]==2||MAP[n][cy][cx+2]==1||MAP[n][cy+1][cx+2]==1||MAP[n][cy-1][cx+2]==1) break;
			    	    				else{
			    	    					clearenemy(q);
					        			q->x+=1;
										printenemy(q);
										q->direction=RIGHT;
										break;
			    	    				}
			    	    			}
									if(MAP[n][cy][cx+2]==2||MAP[n][cy+1][cx+2]==2||MAP[n][cy-1][cx+2]==2||MAP[n][cy][cx+2]==1||MAP[n][cy+1][cx+2]==1||MAP[n][cy-1][cx+2]==1) break;

									if(player.x-q->x<=3&&player.x-q->x>0&&abs(player.y-q->y)<2) break;
									
					        		if(MAP[n][cy][cx+3]==2||MAP[n][cy+1][cx+3]==2||MAP[n][cy-1][cx+3]==2||MAP[n][cy][cx+3]==1||MAP[n][cy+1][cx+3]==1||MAP[n][cy-1][cx+3]==1) {
					        			clearenemy(q);
					        			q->x+=1;
										printenemy(q);
										q->direction=RIGHT;
					        		}
					        		else{
					        			clearenemy(q);
					        			q->x+=2;
										printenemy(q);
										q->direction=RIGHT;
									}
			    	    		}
			    	    		break;
							}
					}		
		}

	}
}

		
	





void createnemy(int n){
	TANK *q=enemy;
	while(q->exsist==1) {q++;}
		int b;enemyalive++;
		while(1){
			b=2+randomize(80);
			if(MAP[n][3][b-2]==0&&MAP[n][3][b-1]==0&&MAP[n][3][b]==0&&MAP[n][3][b+1]==0&&MAP[n][3][b+2]==0) break;
		}
		memset(q->node,0,sizeof(q->node));
		memset(q->stack,0,sizeof(q->stack));
		q->x=b;
		q->y=2;
		q->startAI=0;
		q->exsist=1;
		q->direction=UP;
		q->color=1+randomize(2)+randomize(1)*8;
		printenemy(q);
	
}


void clearplayer(int c){
	switch(c){
		case UP:{
			clear(player.x,player.y);
			clear(player.x+1,player.y);
			clear(player.x-1,player.y);
			clear(player.x,player.y+1);
			clear(player.x+1,player.y+1);
			clear(player.x-1,player.y+1);
			break;
		}
		case DOWN:{
			clear(player.x,player.y);
			clear(player.x+1,player.y);
			clear(player.x-1,player.y);
			clear(player.x,player.y-1);
			clear(player.x+1,player.y-1);
			clear(player.x-1,player.y-1);
			break;
		}
		case LEFT:{
			clear(player.x,player.y-1);
			clear(player.x,player.y);
			clear(player.x,player.y+1);
			clear(player.x+1,player.y-1);
			clear(player.x+1,player.y);
			clear(player.x+1,player.y+1);
			clear(player.x-1,player.y-1);
			clear(player.x-1,player.y);
			clear(player.x-1,player.y+1);
			break;
		}
		case RIGHT:{
			clear(player.x,player.y-1);
			clear(player.x,player.y);
			clear(player.x,player.y+1);
			clear(player.x+1,player.y-1);
			clear(player.x+1,player.y);
			clear(player.x+1,player.y+1);
			clear(player.x-1,player.y-1);
			clear(player.x-1,player.y);
			clear(player.x-1,player.y+1);
			break;
		}
	}
}

void printplayer(int c){
	if(resurrecting==1) color(4);
	else color(6);
	switch(c){
		case UP:{
			print(player.x,player.y,'M');
			print(player.x+1,player.y,'O');
			print(player.x-1,player.y,'O');
			print(player.x,player.y-1,'I');
			break;
		}
		case DOWN:{
			print(player.x,player.y,'W');
			print(player.x+1,player.y,'O');
			print(player.x-1,player.y,'O');
			print(player.x,player.y+1,'I');
			break;
		}
		case LEFT:{
			print(player.x,player.y,'<');
			print(player.x,player.y-1,'O');
			print(player.x,player.y+1,'O');
			print(player.x-1,player.y,'=');
			break;
		}
		case RIGHT:{
			print(player.x,player.y,'>');
			print(player.x,player.y-1,'O');
			print(player.x,player.y+1,'O');
			print(player.x+1,player.y,'=');
			break;	
		}
	}
}

void playermov(int n){//æ’žå¢™åˆ¤å®šåœ¨å¤´çš„å‰æ–¹2æ ¼å¤„ï¼ï¼ï¼
	if (GetAsyncKeyState(VK_UP)& 0x8000)
		{

			if(player.direction!=UP){
				clearplayer(UP);
				printplayer(UP);
				player.direction=UP;
			}
			else{
				if(player.y==2) return;
				if(MAP[n][player.y-2][player.x]==1) return;
				if(MAP[n][player.y-2][player.x-1]==1) return;
				if(MAP[n][player.y-2][player.x+1]==1) return;
				if(MAP[n][player.y-2][player.x]==2) return;
				if(MAP[n][player.y-2][player.x-1]==2) return;
				if(MAP[n][player.y-2][player.x+1]==2) return;
				clearplayer(UP);
				player.y--;
				printplayer(UP);
				player.direction=UP;
			}
			
	}
	else if (GetAsyncKeyState(VK_DOWN)& 0x8000)  
		{

			if(player.direction!=DOWN){
				clearplayer(DOWN);
				printplayer(DOWN);
				player.direction=DOWN;
			}
			else{
				if(player.y==38) return;
				if(MAP[n][player.y+2][player.x-1]==1) return;
				if(MAP[n][player.y+2][player.x]==1) return;
				if(MAP[n][player.y+2][player.x+1]==1) return;
				if(MAP[n][player.y+2][player.x-1]==2) return;
				if(MAP[n][player.y+2][player.x]==2) return;
				if(MAP[n][player.y+2][player.x+1]==2) return;
				clearplayer(DOWN);
				player.y++;
				printplayer(DOWN);
				player.direction=DOWN;
			}
	}
	else if (GetAsyncKeyState(VK_LEFT)& 0x8000)
        {
      
        	int i=2;
        	if(GetAsyncKeyState(VK_LCONTROL)& 0x8000) i=1;
        	if(player.direction!=LEFT){
        		clearplayer(LEFT);
        		printplayer(LEFT);
        		player.direction=LEFT;
        	}
        	else{
        		if(player.x==2) return;//å·¦å³æ–¹å‘å¥‡å¶åˆ¤å®šæœ‰é—®é¢˜
        		if(player.x==3) {//
        			if(MAP[n][player.y][player.x-2]==2||MAP[n][player.y-1][player.x-2]==2||MAP[n][player.y+1][player.x-2]==2||MAP[n][player.y][player.x-2]==1||MAP[n][player.y-1][player.x-2]==1||MAP[n][player.y+1][player.x-2]==1) return;
        			else {
        				clearplayer(LEFT);
        			player.x-=1;
        			printplayer(LEFT);
        			player.direction=LEFT;
        			return;
        			}
        		}
        		if(MAP[n][player.y][player.x-2]==2||MAP[n][player.y-1][player.x-2]==2||MAP[n][player.y+1][player.x-2]==2||MAP[n][player.y][player.x-2]==1||MAP[n][player.y-1][player.x-2]==1||MAP[n][player.y+1][player.x-2]==1) return;//åœ¨å·¦2æ ¼æœ‰é˜»ç¢
        		if(MAP[n][player.y][player.x-3]==2||MAP[n][player.y-1][player.x-3]==2||MAP[n][player.y+1][player.x-3]==2||MAP[n][player.y][player.x-3]==1||MAP[n][player.y-1][player.x-3]==1||MAP[n][player.y+1][player.x-3]==1) {//åœ¨å·¦ä¸‰æ ¼æœ‰é˜»ç¢ï¼Œåªèµ°ä¸€æ ¼
        			clearplayer(LEFT);
        			player.x-=1;
					printplayer(LEFT);
					player.direction=LEFT;
        		}
        		else{
        			clearplayer(LEFT);
        			player.x-=i;
					printplayer(LEFT);
					player.direction=LEFT;
				}
        	}
        	
       }
	else if (GetAsyncKeyState(VK_RIGHT)& 0x8000)  
		{
			int i=2;
			if(GetAsyncKeyState(VK_LCONTROL)& 0x8000) i=1;
			if(player.direction!=RIGHT){
        		clearplayer(RIGHT);
        		printplayer(RIGHT);
        		player.direction=RIGHT;
        	}
        	else{
        		if(player.x==80) return;
        		if(player.x==79){
        			if(MAP[n][player.y][player.x+2]==2||MAP[n][player.y+1][player.x+2]==2||MAP[n][player.y-1][player.x+2]==2||MAP[n][player.y][player.x+2]==1||MAP[n][player.y+1][player.x+2]==1||MAP[n][player.y-1][player.x+2]==1) return;
        			else{
        				clearplayer(RIGHT);
	        		player.x+=1;
					printplayer(RIGHT);
					player.direction=RIGHT;
					return;
        			}
        		}
				if(MAP[n][player.y][player.x+2]==2||MAP[n][player.y+1][player.x+2]==2||MAP[n][player.y-1][player.x+2]==2||MAP[n][player.y][player.x+2]==1||MAP[n][player.y+1][player.x+2]==1||MAP[n][player.y-1][player.x+2]==1) return;
	        	if(MAP[n][player.y][player.x+3]==2||MAP[n][player.y+1][player.x+3]==2||MAP[n][player.y-1][player.x+3]==2||MAP[n][player.y][player.x+3]==1||MAP[n][player.y+1][player.x+3]==1||MAP[n][player.y-1][player.x+3]==1) {
	        		clearplayer(RIGHT);
	        		player.x+=1;
					printplayer(RIGHT);
					player.direction=RIGHT;
	        	}
	        	else{
	        		clearplayer(RIGHT);
	        		player.x+=i;
					printplayer(RIGHT);
					player.direction=RIGHT;
				}
        	}
	} 


}

void playershoot(){
	if (player.bullet.exsist==0&&(GetAsyncKeyState(90)& 0x8000)){
		if(player.startAI==0){//播放发射音效的不能过快（播放音乐的函数不能叠加音乐），否则会很难听
			player.interval=clock();
			PlaySound("fire.wav", NULL, SND_FILENAME | SND_ASYNC);
			player.startAI=1;
		}
		switch(player.direction){
			case UP:{
				creatplayerbullet();
				break;
			}
			case LEFT:{
				creatplayerbullet();
				break;
			}
			case RIGHT:{
				creatplayerbullet();
				break;
			}
			case DOWN:{
				creatplayerbullet();
				break;
			}
			
		}
		
	}
	else return;
}

void creatplayer(){
	player.x=36;
	player.y=38;
	player.exsist=1;
	player.direction=UP;
	printplayer(UP);
}



void renewbullet(){
	player.bullet.exsist=0;
	TANK *q=enemy;
	for(int i=0;i<5;i++,q++){
		(*q).bullet.exsist=0;
	}
}


void renewenemy(){
	enemyleft=12;enemyalive=0;
	TANK *q=enemy;
	for(int i=0;i<5;i++,q++){
		memset(q->node,0,sizeof(q->node));
		memset(q->stack,0,sizeof(q->stack));
		q->exsist=0;
	}
}



void renewMAP(){//endingåŽçš„åˆå§‹åŒ–
	int o,i,j;
	for(o=0;o<5;o++){
		for(i=0;i<41;i++){
			for(j=0;j<83;j++) MAP[o][i][j]=map[o][i][j];
		}
	}
	for(i=0;i<41;i++){
			for(j=0;j<83;j++) MAP[5][i][j]=custommap[i][j];
		}
}



void creatside(){
	int i;
	color(7);
	for (i = 83; i < 103; i++) print(i,0,176);
	for(i=83;i<103;i++) print(i,40,176);
	for (i=0;i<41;i++) print(102,i,176);
}


void creatmap(int n){
	int i,j;
	int *p=MAP[n][0];
	for(i=0;i<41;i++){
		for(j=0;j<83;j++){
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),12);
			if(*p==1) print(j,i,176);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
			if(*p==2) print(j,i,177);
			p++;
		}
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),7);
	for(i=0;i<40;i++) print(0,i,176);
	for(i=0;i<40;i++) print(82,i,176);	
	for(i=0;i<83;i++) print(i,0,176);
	for(i=0;i<83;i++) print(i,40,176);
	print(41,38,'@');
}

void ini(int i){//æ¯ä¸€å…³çš„åˆå§‹åŒ–
	player.direction=UP;
	playerleft = 5;
	renewbullet();
	renewenemy();
	creatside();
	creatmap(i);
	creatplayer();
	mciSendString(TEXT("open begin.mp3 alias mysong"),NULL,0,NULL);
	mciSendString(TEXT("play mySong"),NULL,0,NULL);
}


int judge(int i){
	
	if (playerleft == 0) {return 0;}
	if(MAP[i][38][41]==0&&tankcheat==0) {PlaySound("boom.wav", NULL, SND_FILENAME | SND_ASYNC);return 0;}
	return 1;
}

int checkfxxked(){
	if(tankcheat==0&&resurrecting==0){
		TANK *q=enemy;
		for(i=0;i<5;i++,q++) {
			if(q->exsist==1&&player.x==q->x&&player.y==q->y) {
				PlaySound("boom.wav", NULL, SND_FILENAME | SND_ASYNC);
				clear(x,y);
				playerleft--; 
				player.exsist=0;
				clearplayerwhole();
				return 1;
			}
		}
		return 0;
	}
}



void level(int i,int dif){
	resurrecting=0;
	color(14);
	i=i-1;
	ini(i);
	gotoxy(85,3);
	switch(dif){
		case 1: printf("Easy");break;
		case 2: printf("Normal");break;
		case 3: printf("Hard");break;
		case 4: printf("Lunatic");break;
	}
	int flag=1;//第一次进来不要判定敌机剩余量，不要关闭音乐
	int begin;//无敌时间计时器
	while(1){
		if (GetAsyncKeyState( 0x1B )& 0x8000){ end=-1;mciSendString(TEXT("close mySong"),NULL,0,NULL);return;} //按esc退出当前游戏
		checkfxxked();//体术判定

		if(judge(i)==0) {end=0;mciSendString(TEXT("close mySong"),NULL,0,NULL);return;}//玩家是否死亡，家是否炸了，游戏结束
		else if(flag==0&&enemyalive<=0) {mciSendString(TEXT("close mySong"),NULL,0,NULL);return;}//所有敌机被击落。跳到下一关

		if(enemyleft>0&&enemyalive<5) {enemyleft--;createnemy(i);flag=0;}//敌机不足5个时生成至5个

		if(player.exsist==0&&playerleft > 0) {creatplayer();begin=clock();resurrecting=1;printplayer(player.direction);}//玩家死后仍有残机的话游戏继续,且进入无敌时间
		if(resurrecting==1&&clock()-begin>5000) {resurrecting=0;printplayer(player.direction);}//5秒内无敌,5秒后结束无敌


		gotoxy(85,5);color(14);printf("Enemyleft:  \b\b%d",enemyleft+enemyalive);
		gotoxy(85,8);printf("Playerleft:%d\n",playerleft);
		enemymov(i);
		playermov(i);


		playershoot();

		bulletfly(i);



		if(player.startAI==1){//借用一下，发射音效要用，
			if(clock()-player.interval<500); 
			else player.startAI=0;
		}
		Sleep(90);
		player.time+=((float)90/CLK_TCK);
	}
	return;
}

void game(int dif){
	renewMAP();
	int i=1;
	playerleft = 5;
	player.time=0;
	while(end!=0){
		level(i,dif);
		mycls();
		i++;
		if(end==-1) {mycls();return;}
		if(i==7)  {mycls();return;}//成功打完6关
	}
	return;
}

void ending(){//1 good ending 0 bad ending
	switch(end){
		case 0:{
			system("cls");color(14);gotoxy(20,20);printf("GAME OVER");Sleep(2000);end=1;system("cls");return;
		}
		case 1:{
			system("cls");color(14);gotoxy(20,20);printf("YOU ARE THE VICTORIOR");Sleep(5000);
			system("cls");gotoxy(20,20);printf("Input your name below");
			record_addnew();record_rank();
			mycls();
			return;
		}
		case -1:return;
	}
}

void menu(){
	while(1){
		gotoxy(41,20);
		color(14);printf("I WANNA BE THE TANKMAN!!\n1.Start Game\n2.Stage Practice\n3.Manual\n4.Record\n5.Map Editor\n6.Exit");
		gotoxy(41,36);printf("Made by Blanka_vulpo");
		gotoxy(41,37);printf("I want to sleep......");
		if(tankcheat==1) {gotoxy(43,40);printf("CHEAT MODE    \b\b\bON");}
		else {gotoxy(43,40);printf("CHEAT MODE    \b\b\bOFF");}


		if(GetAsyncKeyState(VK_SPACE)&0x8000) {
			if(tankcheat==0){tankcheat=1;}
			else {tankcheat=0;}
			Sleep(100);
		}
		if ((GetAsyncKeyState('1')& 0x8000)||(GetAsyncKeyState(97)& 0x8000)){
			system("cls");
			Sleep(200);
			while(1){
				end=1;
				gotoxy(0,20);
				color(14);printf("Select Difficulty\n1.Easy\n2.Normal\n3.Hard\n4.Lunatic");
				if((GetAsyncKeyState(27)& 0x8000)) {mycls();break;}
				if((GetAsyncKeyState('1')& 0x8000)||(GetAsyncKeyState(97)& 0x8000)){
					enemyshootingrate=30;
					AIrandomrate=3;
					AIlastingtime=2000;
					system("cls");
					game(1);
					ending(end);
				}
				if((GetAsyncKeyState('2')& 0x8000)||(GetAsyncKeyState(98)& 0x8000)){
					enemyshootingrate=45;
					AIrandomrate=8;
					AIlastingtime=2000;
					system("cls");
					game(2);
					ending(end);
					
				}
				if((GetAsyncKeyState('3')& 0x8000)||(GetAsyncKeyState(99)& 0x8000)){
					enemyshootingrate=65;
					AIrandomrate=12;
					AIlastingtime=2000;
					system("cls");
					game(3);
					ending(end);
					
				}
				if((GetAsyncKeyState('4')& 0x8000)||(GetAsyncKeyState(100)& 0x8000)){
					enemyshootingrate=85;
					AIrandomrate=16;
					AIlastingtime=2000;
					system("cls");
					game(4);
					ending(end);
					
				}
				
			}
		}
		if ((GetAsyncKeyState('2')& 0x8000)||(GetAsyncKeyState(98)& 0x8000)) {
			system("cls");
			Sleep(200);
			gotoxy(0,20);
			color(14);printf("Select Stage\n1.1\n2.2\n3.3\n4.4\n5.5\n6.custom");
			while(1){
				//hhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh
				if((GetAsyncKeyState(27)& 0x8000))  {mycls();break;}
				if((GetAsyncKeyState('1')& 0x8000)||(GetAsyncKeyState(97)& 0x8000)) {
					mycls();
					Sleep(200);
					while(1){
						renewMAP();
						end=1;
						gotoxy(0,20);
						color(14);printf("Select Difficulty\n1.Easy\n2.Normal\n3.Hard\n4.Lunatic");
						if((GetAsyncKeyState('1')& 0x8000)||(GetAsyncKeyState(97)& 0x8000)){
							enemyshootingrate=30;
							AIrandomrate=3;
							AIlastingtime=2000;
							system("cls");
							level(1,1);mycls();
						}
						if((GetAsyncKeyState('2')& 0x8000)||(GetAsyncKeyState(98)& 0x8000)){
							enemyshootingrate=45;
							AIrandomrate=8;
							AIlastingtime=2000;
							system("cls");
							level(1,2);mycls();
							
						}
						if((GetAsyncKeyState('3')& 0x8000)||(GetAsyncKeyState(99)& 0x8000)){
							enemyshootingrate=65;
							AIrandomrate=12;
							AIlastingtime=2000;
							system("cls");
							level(1,3);mycls();
							
						}
						if((GetAsyncKeyState('4')& 0x8000)||(GetAsyncKeyState(100)& 0x8000)){
							enemyshootingrate=85;
							AIrandomrate=16;
							AIlastingtime=2000;
							system("cls");
							level(1,4);mycls();
							
						}
						if((GetAsyncKeyState(27)& 0x8000)) {mycls();break;}
					}
				}
				if((GetAsyncKeyState('2')& 0x8000)||(GetAsyncKeyState(98)& 0x8000)) {
					mycls();
					Sleep(200);
					while(1){
						renewMAP();
						end=1;
						gotoxy(0,20);
						color(14);printf("Select Difficulty\n1.Easy\n2.Normal\n3.Hard\n4.Lunatic");
						if((GetAsyncKeyState('1')& 0x8000)||(GetAsyncKeyState(97)& 0x8000)){
							enemyshootingrate=30;
							AIrandomrate=3;
							AIlastingtime=2000;
							system("cls");
							level(2,1);mycls();							
						}
						if((GetAsyncKeyState('2')& 0x8000)||(GetAsyncKeyState(98)& 0x8000)){
							enemyshootingrate=45;
							AIrandomrate=8;
							AIlastingtime=2000;
							system("cls");
							level(2,2);mycls();
							
						}
						if((GetAsyncKeyState('3')& 0x8000)||(GetAsyncKeyState(99)& 0x8000)){
							enemyshootingrate=65;
							AIrandomrate=12;
							AIlastingtime=2000;
							system("cls");
							level(2,3);mycls();
							
						}
						if((GetAsyncKeyState('4')& 0x8000)||(GetAsyncKeyState(100)& 0x8000)){
							enemyshootingrate=85;
							AIrandomrate=16;
							AIlastingtime=2000;
							system("cls");
							level(2,4);mycls();
							
						}
						if((GetAsyncKeyState(27)& 0x8000)) {mycls();break;}
					}
				}
				if((GetAsyncKeyState('3')& 0x8000)||(GetAsyncKeyState(99)& 0x8000)) {
					mycls();
					Sleep(200);
					while(1){
						renewMAP();
						end=1;
						gotoxy(0,20);
						color(14);printf("Select Difficulty\n1.Easy\n2.Normal\n3.Hard\n4.Lunatic");
						if((GetAsyncKeyState('1')& 0x8000)||(GetAsyncKeyState(97)& 0x8000)){
							enemyshootingrate=30;
							AIrandomrate=3;
							AIlastingtime=2000;
							system("cls");
							level(3,1);mycls();
							
						}
						if((GetAsyncKeyState('2')& 0x8000)||(GetAsyncKeyState(98)& 0x8000)){
							enemyshootingrate=45;
							AIrandomrate=8;
							AIlastingtime=2000;
							system("cls");
							level(3,2);mycls();
							
						}
						if((GetAsyncKeyState('3')& 0x8000)||(GetAsyncKeyState(99)& 0x8000)){
							enemyshootingrate=65;
							AIrandomrate=12;
							AIlastingtime=2000;
							system("cls");
							level(3,3);mycls();
							
						}
						if((GetAsyncKeyState('4')& 0x8000)||(GetAsyncKeyState(100)& 0x8000)){
							enemyshootingrate=85;
							AIrandomrate=16;
							AIlastingtime=2000;
							system("cls");
							level(3,4);mycls();
							
						}
						if((GetAsyncKeyState(27)& 0x8000)) {mycls();break;}
					}
				}
				if((GetAsyncKeyState('4')& 0x8000)||(GetAsyncKeyState(100)& 0x8000)) {
					mycls();
					Sleep(200);
					while(1){
						renewMAP();
						end=1;
						gotoxy(0,20);
						color(14);printf("Select Difficulty\n1.Easy\n2.Normal\n3.Hard\n4.Lunatic");
						if((GetAsyncKeyState('1')& 0x8000)||(GetAsyncKeyState(97)& 0x8000)){
							enemyshootingrate=30;
							AIrandomrate=3;
							AIlastingtime=2000;
							system("cls");
							level(4,1);mycls();
							
						}
						if((GetAsyncKeyState('2')& 0x8000)||(GetAsyncKeyState(98)& 0x8000)){
							enemyshootingrate=45;
							AIrandomrate=8;
							AIlastingtime=2000;
							system("cls");
							level(4,2);mycls();
							
						}
						if((GetAsyncKeyState('3')& 0x8000)||(GetAsyncKeyState(99)& 0x8000)){
							enemyshootingrate=65;
							AIrandomrate=12;
							AIlastingtime=2000;
							system("cls");
							level(4,3);mycls();
							
						}
						if((GetAsyncKeyState('4')& 0x8000)||(GetAsyncKeyState(100)& 0x8000)){
							enemyshootingrate=85;
							AIrandomrate=16;
							AIlastingtime=2000;
							system("cls");
							level(4,4);mycls();
							
						}
						if((GetAsyncKeyState(27)& 0x8000)) {mycls();break;}
					}
				}
				if((GetAsyncKeyState('5')& 0x8000)||(GetAsyncKeyState(101)& 0x8000)) {
					mycls();
					Sleep(200);
					while(1){
						renewMAP();
						end=1;
						gotoxy(0,20);
						color(14);printf("Select Difficulty\n1.Easy\n2.Normal\n3.Hard\n4.Lunatic");
						if((GetAsyncKeyState('1')& 0x8000)||(GetAsyncKeyState(97)& 0x8000)){
							enemyshootingrate=30;
							AIrandomrate=3;
							AIlastingtime=2000;
							system("cls");
							level(5,1);mycls();
							
						}
						if((GetAsyncKeyState('2')& 0x8000)||(GetAsyncKeyState(98)& 0x8000)){
							enemyshootingrate=45;
							AIrandomrate=8;
							AIlastingtime=2000;
							system("cls");
							level(5,2);mycls();
							
						}
						if((GetAsyncKeyState('3')& 0x8000)||(GetAsyncKeyState(99)& 0x8000)){
							enemyshootingrate=65;
							AIrandomrate=12;
							AIlastingtime=2000;
							system("cls");
							level(5,3);mycls();
							
						}
						if((GetAsyncKeyState('4')& 0x8000)||(GetAsyncKeyState(100)& 0x8000)){
							enemyshootingrate=85;
							AIrandomrate=16;
							AIlastingtime=2000;
							system("cls");
							level(5,4);mycls();
							
						}
						if((GetAsyncKeyState(27)& 0x8000)) {mycls();break;}
					}
				}
				if((GetAsyncKeyState('6')& 0x8000)||(GetAsyncKeyState(102)& 0x8000)) {
					mycls();
					Sleep(200);
					while(1){
						renewMAP();
						end=1;
						gotoxy(0,20);
						color(14);printf("Select Difficulty\n1.Easy\n2.Normal\n3.Hard\n4.Lunatic");
						if((GetAsyncKeyState('1')& 0x8000)||(GetAsyncKeyState(97)& 0x8000)){
							enemyshootingrate=30;
							AIrandomrate=3;
							AIlastingtime=2000;
							system("cls");
							level(6,1);mycls();
							
						}
						if((GetAsyncKeyState('2')& 0x8000)||(GetAsyncKeyState(98)& 0x8000)){
							enemyshootingrate=45;
							AIrandomrate=8;
							AIlastingtime=2000;
							system("cls");
							level(6,2);mycls();
							
						}
						if((GetAsyncKeyState('3')& 0x8000)||(GetAsyncKeyState(99)& 0x8000)){
							enemyshootingrate=65;
							AIrandomrate=12;
							AIlastingtime=2000;
							system("cls");
							level(6,3);mycls();
							
						}
						if((GetAsyncKeyState('4')& 0x8000)||(GetAsyncKeyState(100)& 0x8000)){
							enemyshootingrate=85;
							AIrandomrate=16;
							AIlastingtime=2000;
							system("cls");
							level(6,4);mycls();
							
						}
						if((GetAsyncKeyState(27)& 0x8000)) {mycls();break;}
					}
				}
			}
		}
		if((GetAsyncKeyState('3')& 0x8000)||(GetAsyncKeyState(99)& 0x8000)) {
				mycls();
				gotoxy(0,20);
				color(14);printf("You can shoot using Z and move slower using left control\nUse ESC to go back\nYou have 5s invincible time after your death\n\nUse Mapeditor to creat Your own Map.\nYour custom map is added to the 6th map\nIF YOU THINK THE GAME IS TOO HARD\nYOU CAN PRESS SPACE IN THE MAIN MENU TO TURN ON THE CHEAT MODE. \n\nYou could send me your custommap to my emailbox at 1427457987@qq.com");
			while(1){
				if((GetAsyncKeyState(27)& 0x8000)) {mycls();break;}
			}
		}
		if((GetAsyncKeyState('4')& 0x8000)||(GetAsyncKeyState(100)& 0x8000)){
				mycls();
				color(14);
				record_list();
			while(1){
				
				if((GetAsyncKeyState(27)& 0x8000)) {mycls();break;}
			}
		}
		if((GetAsyncKeyState('5')& 0x8000)||(GetAsyncKeyState(101)& 0x8000)){
			Sleep(200);
			fflush(stdin);
			mapeditor();
			cursor(0);
			mycls();
			system("mode con cols=103 lines=41");
		}
		if((GetAsyncKeyState('6')& 0x8000)||(GetAsyncKeyState(102)& 0x8000)) return;
	}
}









void GAMETANK(){

	SetConsoleOutputCP(437);
	cursor(0);
	file_read();
	if(!(fp=fopen("record.dat","a+"))){
		printf("Can't open record.dat!\n");
		exit(0);
	}
	fclose(fp);
	record_rank();
	tankcheat=0;
	system("mode con cols=103 lines=41");
	mycls();
	menu();
	return;
}
