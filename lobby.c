#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include "TANK.c"
#include "STG.c"
#include "SNAKE.c"
#include "menumap.c"
void main(){
	/*1
	2   3*/
	int i,j,flag=1;int egg=0;
	cursor(0);
	
	system("mode con cols=83 lines=41");
		while(1){
			if(randomize(1000)==0) {
				mciSendString(TEXT("open awei.mp3 alias awei"),NULL,0,NULL);
				mciSendString(TEXT("play awei"),NULL,0,NULL);
				Sleep(4000);
				mciSendString(TEXT("close awei"),NULL,0,NULL);
			}
			SetConsoleOutputCP(437);
			if(GetAsyncKeyState(VK_DOWN)&0x8000&&(flag==3||flag==2)) {Sleep(300);egg++;	}
			if(GetAsyncKeyState(VK_UP)&0x8000&&(flag==2||flag==3)) flag=1; 
			if(GetAsyncKeyState(VK_DOWN)&0x8000&&flag==1) flag=2;
			if(GetAsyncKeyState(VK_LEFT)&0x8000&&flag==3) flag=2;
			if(GetAsyncKeyState(VK_LEFT)&0x8000&&flag==1) flag=2;
			if(GetAsyncKeyState(VK_RIGHT)&0x8000&&flag==2) flag=3;
			if(GetAsyncKeyState(VK_RIGHT)&0x8000&&flag==1) flag=3;
				
			if(egg==2){
				mycls();
				while(1){
					int i=0;
					
					SetConsoleOutputCP(936);
					color ((randomize(100)+(i++))%16);
					gotoxy(20,10);printf("制作人员"); color ((randomize(100)+(i++))%16);
					gotoxy(15,15);printf("马心远 Blanka_vulpo");color ((randomize(100)+(i++))%16);
					gotoxy(15,20);printf("吴昊伦");color ((randomize(100)+(i++))%16);
					gotoxy(15,25);printf("朱天立");color ((randomize(100)+(i++))%16);
					gotoxy(15,30);printf("NPCMACI");color ((randomize(100)+(i++))%16);
					gotoxy(50,10);printf("特别鸣谢");color ((randomize(100)+(i++))%16);
					gotoxy(45,15);printf("皇牌测试师 WXH");color ((randomize(100)+(i++))%16);
					gotoxy(45,20);printf("测试员01");
					gotoxy(45,25);printf("教头");
					Sleep(100);
					if(GetAsyncKeyState(VK_UP)&0x8000) break;
				}
				mycls();
				egg=0;
			}
			if(flag==1) color(14);
			else color(8);
			for(i=0;i<22;i++){
				for(j=0;j<83;j++) if(menumap[i][j]==1) print(j,i,177);
			}
			gotoxy(30,8);printf("I WANNA BE THE TANKMAN!");
			
			
			if(flag==2) color(14);
			else color(8);
			for(i=22;i<41;i++){
				for(j=0;j<41;j++) if(menumap[i][j]==1) print(j,i,177);
			}
			gotoxy(10,32);printf("I WANNA BE THE PILOT");
			
			if(flag==3) color(14);
			else color(8);
			for(i=22;i<41;i++){
			 	for(j=41;j<83;j++) if(menumap[i][j]==1) print(j,i,177);
			}
			gotoxy(60,32);printf("Snake");
			gotoxy(41,22);
			if(flag==1&&GetAsyncKeyState(VK_RETURN)&0x8000){
				getchar();
				fflush(stdin);
				Sleep(300);mycls(); GAMETANK();Sleep(300);mycls();SetConsoleOutputCP(936);system("mode con cols=83 lines=41");
			}
			if(flag==2&&GetAsyncKeyState(VK_RETURN)&0x8000){
				getchar();
				fflush(stdin);
				Sleep(300);mycls(); STG();Sleep(300);mycls();SetConsoleOutputCP(936);system("mode con cols=83 lines=41");
			}
			if(flag==3&&GetAsyncKeyState(VK_RETURN)&0x8000){
				getchar();
				fflush(stdin);
				Sleep(300);mycls();SNAKE();Sleep(300);mycls();SetConsoleOutputCP(936);system("mode con cols=83 lines=41");
			}
				
			
			
				
			
		}
	
		
} 
