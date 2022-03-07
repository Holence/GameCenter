#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include<time.h>
#include"function.h"
#define  WIDTH 101
#define  HEIGHT 50
#define PI 3.1415926
#define BULLETAMOUNT 300
int stage,stgend,boom,boomleft;
short cheat;
short stage2flag;
typedef struct {
	double x;
	double y;
	int slow_move;
	int shoot;
}PLAYER;
PLAYER STGplayer;
int STGplayer_left,res;
struct {
	double x;
	double y;
	int exist;
}STGplayerbullet[50];

typedef struct {
	double x1,x2;
	double y1,y2;
	int blood;
}ENEMY;
ENEMY STGenemy;

struct bullet {
	double x;
	double y;
	double inia;
	double vector_x;
	double vector_y;
	double v;
	double delt;
	int p;
	int exist;
	short color;
}STGenemy_bullet[BULLETAMOUNT];










void get_kybhit()
{
	void STGplayer_move(int i);
	static int i = 0;
	i++;
	if (i%3 != 0) {//不要每帧都读取按键，可以控制自机移动速度，可以做出斜向45度移动
		return;	
	}
	if(GetAsyncKeyState(VK_LCONTROL)&0x8000) STGplayer.slow_move = 1;
	else STGplayer.slow_move = 0;
	if(GetAsyncKeyState(VK_UP)&0x8000) STGplayer_move(1);
	if(GetAsyncKeyState(VK_DOWN)&0x8000) STGplayer_move(2);
	if(GetAsyncKeyState(90)&0x8000) STGplayer.shoot = 1; else STGplayer.shoot = 0;
	if (STGplayer.slow_move == 1 && i%3 == 2) return;
	if(GetAsyncKeyState(VK_RIGHT)&0x8000) STGplayer_move(4);
	if(GetAsyncKeyState(VK_LEFT)&0x8000) STGplayer_move(3);
	return;
}




void print_STGplayer(){
	int x=STGplayer.x,y=STGplayer.y;
	print(x-1,y,'O');
	print(x,y,'M');
	print(x+1,y,'O');
	print(x,y-1,'I');
}

void clear_STGplayer(){
	int x=STGplayer.x,y=STGplayer.y;
	print(x-1,y,' ');
	print(x,y,' ');
	print(x+1,y,' ');
	print(x,y-1,' ');
}

void createSTGplayerbullet(){
	int i=0;
	for(i=0;i<50;i++){
		if(STGplayerbullet[i].exist==0&&STGplayer.y>5) {STGplayerbullet[i].exist=1;STGplayerbullet[i].x=STGplayer.x;STGplayerbullet[i].y=STGplayer.y-2;return;}
	}
}

void print_STGplayerbullet(){
	int i;
	for(i=0;i<50;i++){
		if(STGplayerbullet[i].exist==1) {
			print(STGplayerbullet[i].x,STGplayerbullet[i].y,' ');
			STGplayerbullet[i].y-=4;
			print(STGplayerbullet[i].x,STGplayerbullet[i].y,'*');
		}
	}
}

void STGplayer_move(int dir)
{
	clear_STGplayer();
	switch(dir){
		case 1: if (STGplayer.y <= 1) break; STGplayer.y--; break;
		case 2: if (STGplayer.y>= HEIGHT-1) break; STGplayer.y++;break;
		case 3: if (STGplayer.x < 3)  break; if (STGplayer.slow_move) STGplayer.x--; else STGplayer.x -= 2; break;
		case 4: if (STGplayer.x > WIDTH-4)  break; if (STGplayer.slow_move) STGplayer.x++;else STGplayer.x+=2 ;break;
	}
}

void eliminateSTGplayerbullet(){
	int i=0;
	for(;i<50;i++){
		if(STGplayerbullet[i].exist==1&&STGplayerbullet[i].y<=4) {print(STGplayerbullet[i].x,STGplayerbullet[i].y,' ');STGplayerbullet[i].exist=0;}
	}
}

void shoot_judge()
{
	if(STGplayer.shoot) createSTGplayerbullet();
}

void checkhitSTGenemy(){
	int k=1;
	for(int i=0;i<50;i++){
		if(STGplayerbullet[i].exist==1&&STGplayerbullet[i].x<STGenemy.x2&&STGplayerbullet[i].x>STGenemy.x1&&STGplayerbullet[i].y>STGenemy.y1&&STGplayerbullet[i].y<STGenemy.y2) {
			if(STGplayer.slow_move==1) k=2;
			if(STGplayer.y-STGenemy.y2<10) STGenemy.blood-=k*4;
			else STGenemy.blood-=k*1;
			print(STGplayerbullet[i].x,STGplayerbullet[i].y,' ');
			STGplayerbullet[i].exist=0;
		}
	}
}

void checkhitSTGplayer(){
	void boomcls();
	if(cheat==0){
		if(res==0){
			for(int i=0;i<BULLETAMOUNT;i++){
				if(fabs(STGenemy_bullet[i].x - STGplayer.x) < 0.6 && fabs(STGenemy_bullet[i].y-STGplayer.y) < 0.6) {
					res=1;
					PlaySound("biu.wav", NULL, SND_FILENAME | SND_ASYNC);
					boomleft=3;
					if((--STGplayer_left)<0) stgend=0;
					print(STGplayer.x,STGplayer.y,' ');STGenemy.blood-=20;boomcls();STGplayer.x=50;STGplayer.y=48;
					return;
				}
			
			}
		}
	}
}

void checkSTGenemyblood(){
	if(STGenemy.blood<=0) 
		{system("cls");stgend=1;}
}




















void createSTGenemy(){
	STGenemy.x1=30;STGenemy.x2=86;STGenemy.y1=5;STGenemy.y2=15;
	STGenemy.blood=10000;
}

void printSTGenemy(int stage){
	color(15);	
	if(stage==1&&randomize(20)==0){
		// print(STGenemy.x1,STGenemy.y1,' ');print(STGenemy.x2,STGenemy.y2,' ');
	gotoxy(STGenemy.x1,(STGenemy.y1+STGenemy.y2)/2);printf("                                                           ");
		int i=randomize(3),j=randomize(2)-1;
		if(STGenemy.x2+j*i<WIDTH-1&&STGenemy.x1+j*i>0) {STGenemy.x1+=j*i;STGenemy.x2+=j*i;}
		if(STGenemy.y2+j*i<30&&STGenemy.y1+j*i>0) {STGenemy.y1+=j*i;STGenemy.y2+=j*i;}
	}
	// print(STGenemy.x1,STGenemy.y1,'A');print(STGenemy.x2,STGenemy.y2,'A');
	gotoxy(STGenemy.x1,(STGenemy.y1+STGenemy.y2)/2);printf("PlaySound(\"hitwall1.wav\", NULL, SND_FILENAME | SND_ASYNC)");
}

void eliminateSTGenemybullet()
{
	int i;
	for (i = 0; i < BULLETAMOUNT; i++) {
		if (STGenemy_bullet[i].x >= WIDTH-1 || STGenemy_bullet[i].x <= 0 || STGenemy_bullet[i].y >= HEIGHT || STGenemy_bullet[i].y <= 1)
			STGenemy_bullet[i].exist = 0;
	}	
}

void print_STGenemy_bullet()
{
	int i;
	for(i=0;i<BULLETAMOUNT;i++){
		if(STGenemy_bullet[i].exist==1) {

			color(STGenemy_bullet[i].color);print(STGenemy_bullet[i].x,STGenemy_bullet[i].y,'O');
		}
	}	
}

void bullet_move()//每帧通过加上子弹移动向量来移动
{
	int i; 
	for (i = 0; i < BULLETAMOUNT; i++) {
		if (STGenemy_bullet[i].exist == 1) {
			STGenemy_bullet[i].x += STGenemy_bullet[i].vector_x*2;
			STGenemy_bullet[i].y += STGenemy_bullet[i].vector_y;
		}
	}
}

void clearSTGenemybullet()
{
	int i; 
	for (i = 0; i < BULLETAMOUNT; i++) {
		if (STGenemy_bullet[i].exist == 1) print (STGenemy_bullet[i].x, STGenemy_bullet[i].y, ' ');
	}
}

void createboli(double x,double y,double firsta,double v)
{
	static double k=0;
	k++;
	double a=(firsta+k*k*0.04)/180*PI;
	if((int)k%4==0) {
		a=a+PI;
		int i;
		for (i = 0; i < BULLETAMOUNT; i++) {
			if(STGenemy_bullet[i].exist==0){
				STGenemy_bullet[i].color=15;
				STGenemy_bullet[i].exist=1;
				STGenemy_bullet[i].x = x;
				STGenemy_bullet[i].y = y;
				STGenemy_bullet[i].vector_x=cos(a)*v;
				STGenemy_bullet[i].vector_y=-sin(a)*v;
				break;
			}
		}
	}
	else if((int)k%4==2){
		int i;
		for (i = 0; i < BULLETAMOUNT; i++) {
			if(STGenemy_bullet[i].exist==0){
				STGenemy_bullet[i].color=15;
				STGenemy_bullet[i].exist=1;
				STGenemy_bullet[i].x = x;
				STGenemy_bullet[i].y = y;
				STGenemy_bullet[i].vector_x=cos(a)*v;
				STGenemy_bullet[i].vector_y=-sin(a)*v;
				break;
			}
		}
	}
}


void boli(){
	createboli((STGenemy.x1+STGenemy.x2)/2,(STGenemy.y1+STGenemy.y2)/2,0,0.4);
}

void createxiyiji(double x,double y,double a,double v,int p){
	static int k=0;
	if((k++)%20==0){
		a=a/180*PI;
		int i;
		for (i = 0; i < BULLETAMOUNT; i++) {
			if(STGenemy_bullet[i].exist==0){
				if(p==1) STGenemy_bullet[i].color=14;
				else STGenemy_bullet[i].color=11;
				STGenemy_bullet[i].exist=1;
				STGenemy_bullet[i].x = x;
				STGenemy_bullet[i].y = y;
				STGenemy_bullet[i].inia=p*a;
				STGenemy_bullet[i].v=v;
				STGenemy_bullet[i].delt=p*3.5;
				STGenemy_bullet[i].p=p;
				STGenemy_bullet[i].vector_x=cos(a)*v;
				STGenemy_bullet[i].vector_y=-sin(a)*v;
				break;
			}
		}	
	}
	
}

void xiyijigunqilai(){
	int i; 
	for (i = 0; i < BULLETAMOUNT; i++) {
		if(STGenemy_bullet[i].exist==1){
			if(STGenemy_bullet[i].p==1) {
				STGenemy_bullet[i].delt-=0.035;
				STGenemy_bullet[i].inia+=STGenemy_bullet[i].delt/180*PI;
				STGenemy_bullet[i].vector_x=cos(STGenemy_bullet[i].inia)*STGenemy_bullet[i].v;
				STGenemy_bullet[i].vector_y=-sin(STGenemy_bullet[i].inia)*STGenemy_bullet[i].v;
			}
			else {
				
				STGenemy_bullet[i].delt-=-0.01;
				STGenemy_bullet[i].inia+=STGenemy_bullet[i].delt/180*PI;
				STGenemy_bullet[i].vector_x=cos(STGenemy_bullet[i].inia)*STGenemy_bullet[i].v;
				STGenemy_bullet[i].vector_y=-sin(STGenemy_bullet[i].inia)*STGenemy_bullet[i].v;
			}
		}
	}
}


void xiyiji(){
	static double i=0;static short flag=1;
	i=i+flag*1;
	int j;
	for(j=0;j<=10;j++){//不知道为什么j<10只有一支弹幕，但这样j<=10就会有一支是两倍密度的了
		createxiyiji(50,20,flag*(i+j*36),0.45,flag);
	}
	for(int k=0;k<=5;k++){//同理
		createxiyiji(50,20,flag*(-i+k*72),1,-flag);
	}
	xiyijigunqilai();
	if(flag==1&&i>360) {flag=-1;i=0;}
	else if(flag==-1&&i<=-360) {flag=1;i=0;}
	
}

void boomcls(){
	// SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x80);
	
	int x1,x2,y1,y2;int i,j;
	x1=x2=STGplayer.x;y1=y2=STGplayer.y;
	do{
		if((x1-1)>=0) x1--;
		if((x2+1)<WIDTH) x2++;
		if((y1-1)>=0) y1--;
		if((y2+1)<HEIGHT) y2++;
		for(i=x1,j=y1;i<=x2;i++) {print(i,j,' ');}
		for(i=x1,j=y2;i<=x2;i++) {print(i,j,' ');}
		for(i=y1,j=x1;i<=y2;i++) {print(j,i,' ');}
		for(i=y1,j=x2;i<=y2;i++) {print(j,i,' ');}
			
	}while(x1>1||x2<WIDTH-1||y1>1||y2<HEIGHT-1);//
	memset(STGenemy_bullet,0,sizeof(STGenemy_bullet));
	// SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);

}


void renewSTGbullet(){
	memset(STGenemy_bullet,0,sizeof(STGenemy_bullet));
	memset(STGplayerbullet,0,sizeof(STGplayerbullet));
}

















/*???*/
void judge_process()
{

	static long int time;static short starttiming=0;//复活无敌的时间
	if(starttiming==0&&res==1) {
		time=clock();starttiming=1;
	}
	if(res==1&&clock()-time>2000){//复活2秒内无敌
		res=0;starttiming=0;
	}

	get_kybhit();
	shoot_judge();
	bullet_move();
	checkhitSTGplayer();
	checkhitSTGenemy();
	checkSTGenemyblood();

	static short throwboom=0;static long int boomtime;//允许扔雷的间隔时间
	if(GetAsyncKeyState(88)&0x8000){
		if(throwboom==1&&clock()-boomtime<2000) return;
		if(throwboom==1&&clock()-boomtime>2000) throwboom=0;
		if(throwboom==0&&boomleft>0) {
			if(fabs(STGplayer.y-(STGenemy.y1+STGenemy.y2)/2)<10) STGenemy.blood-=200;//扔雷伤害
			else STGenemy.blood-=100;//扔雷伤害
			boomleft--;boom=1;throwboom=1;boomtime=clock();
		}
	}

}

/*draw_process*/
void draw_process()
{
	static int k;
	k++;
	print_STGenemy_bullet();
	if(boom==1){
		res=1;
		boomcls();
		system("cls");
		boom=0;
	}
	printSTGenemy(stage);
	if(res==1) color(15);
	else color(1);
	print_STGplayer();
	print_STGplayerbullet();
	if(k%50==0){
		gotoxy(0,0);
		for(short i=0;i<100;i++) printf(" ");
		gotoxy(0,0);
		for(short i=0;i<STGenemy.blood/100;i++)printf("#");
	}
	//printf("ENEMY BLOOD       \b\b\b\b\b\b%d", STGenemy.blood);
	gotoxy(0,2);printf("PLAYER LEFT   \b\b%d",STGplayer_left );
	gotoxy(0,4);printf("BOOM LEFT  \b%d",boomleft );
}



/*game_play*/
int game_play()
{
	if(GetAsyncKeyState(27)&0x8000) return 0;
	if(stgend==0||stgend==1) return 0;
	clearSTGenemybullet();
	
	
	if(STGenemy.blood>5000) {stage=1;boli();}
	else {
		if(stage2flag==1) {STGenemy.x1=22;STGenemy.x2=78;STGenemy.y1=15;STGenemy.y2=25;xiyiji();}
		else {mciSendString(TEXT("close stage1"),NULL,0,NULL);stage=2;memset(STGenemy_bullet,0,sizeof(STGenemy_bullet));system("cls");xiyiji();mciSendString(TEXT("play stage2 repeat"),NULL,0,NULL);	stage2flag=1;}
	}
	judge_process();


	eliminateSTGenemybullet();
	eliminateSTGplayerbullet();



	draw_process();
	return 1;
}



void startgame()
{
	renewSTGbullet();
	stgend=-1;stage2flag=0;
	mciSendString(TEXT("open Necrofantasia.mp3 alias stage1"),NULL,0,NULL);
	mciSendString(TEXT("open Akahachi.mp3 alias stage2"),NULL,0,NULL);
	STGplayer_left=2;boomleft=3;
	STGplayer.x=50;STGplayer.y=48;
	createSTGenemy();
	mciSendString(TEXT("play stage1 repeat"),NULL,0,NULL);	
	res=0;
	while(1) {
		if(game_play()) {Sleep(2);continue;}
		else break;
	}
	mciSendString(TEXT("close stage1"),NULL,0,NULL);
	mciSendString(TEXT("close stage2"),NULL,0,NULL);
	switch(stgend){
		case 1:color(15);system("cls");gotoxy(40,25);printf("YOU ARE THE VICTORIOR\n");Sleep(5000);return;
		case 0:color(15);system("cls");gotoxy(40,25);printf("GAME OVER\n");Sleep(2000);stgend=-1;return;
	}
	return;
}

void STG(){
	cheat=0;
		system("mode con cols=101 lines=50");
		cursor(0);
		while(1){
			gotoxy(41,20);
			color(14);printf("I WANNA BE THE PILOT!!\n1.Start Game\n2.Manual\n3.Exit");
			gotoxy(41,38);printf("Made by Blanka_vulpo");
			gotoxy(41,39);printf("I want to sleep......");
			if(cheat==1) {gotoxy(41,47);printf("CHEAT MODE    \b\b\bON");}
			else {gotoxy(41,47);printf("CHEAT MODE    \b\b\bOFF");}
			if(GetAsyncKeyState(VK_SPACE)&0x8000) {
				if(cheat==0){cheat=1;}
				else {cheat=0;}
				Sleep(100);
			}
			if(GetAsyncKeyState('1')&0x8000||GetAsyncKeyState(97)&0x8000){
				system("cls");
				startgame();
				system("cls");
				continue;
			}
			if(GetAsyncKeyState('2')&0x8000||GetAsyncKeyState(98)&0x8000){
				system("cls");
				while(1){
					color(14);gotoxy(0,20);
					printf("Press z to shoot. Press x to throw a boom. Press left control to enter slow mode.\n");
					printf("You have 3 lives, each life you have 3 boom.\n");
					printf("Boom can decrease the BOSS's blood.\n");
					printf("If you get closer to the BOSS, your damage to the BOSS is MUCH MORE increased(so dose the boom).\n");
					printf("After you throw a boom or after your death, you have 2s invincible time.\n");
					printf("Press ESC to go back.\n");
					printf("If you think it's too hard to play the game, you can press space in the main menu to cheat a bit.\n");
					if(GetAsyncKeyState(27)&0x8000) break;
				}
				system("cls");
				continue;
			}
			if(GetAsyncKeyState('3')&0x8000||GetAsyncKeyState(99)&0x8000) return;
		}
		return;
		
}
