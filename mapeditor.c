#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <stdio.h>
#include <windows.h>
#include "file.h"
#include"function.h"

void print(short x, short y,char a);
int cursor_x = 1;
int cursor_y = 1;

void cursor(int n);

void color(int c);
void draw_map();
void home();
void clean_map();
void mapeditor();

void mapeditor() 
{
	system("mode con cols=120 lines=41");
	char ch;
	cursor (1);
	file_read();
	home();
	draw_map();
	home();
	gotoxy(1, 1);
	while (1) {
		home();
		gotoxy (cursor_x, cursor_y);
		if((GetAsyncKeyState(27)& 0x8000)) return;
		ch = getch();
		switch (ch) {//不是实时写文件，先保存在数组中
			case -32: switch (ch = getch()) {
							case 72: if (cursor_y != 1) cursor_y--; break;
							case 80: if (cursor_y != ROW-2) cursor_y++; break;
							case 75: if (cursor_x != 1) cursor_x--; else {cursor_x = COL-2; if (cursor_y != 1) cursor_y--;}break;
							case 77: if (cursor_x != COL-2) cursor_x++; else {cursor_x = 1; if (cursor_y != ROW-2) cursor_y++;}break;
						}break;
			case '1': custommap[cursor_y][cursor_x] = ch - '0'; color (12); printf("%c",176); break;
			case '4': custommap[cursor_y][cursor_x] = 1;if (cursor_x != COL-2) cursor_x++; color (12); printf("%c",176); break;
			case '2': custommap[cursor_y][cursor_x] = ch - '0'; color (15); printf("%c",177); break;
			case '5': custommap[cursor_y][cursor_x] = 2; if (cursor_x != COL-2) cursor_x++; color (15); printf("%c",177); break;
			case '0': custommap[cursor_y][cursor_x] = ch - '0'; printf (" "); break;
			case '.': custommap[cursor_y][cursor_x] = 0; if (cursor_x != COL-2) cursor_x++; printf (" "); break;
			case ' ': custommap[cursor_y][cursor_x] = 0; printf (" "); break;
			case 's': file_load(); system("cls"); gotoxy(COL/2, ROW/2); printf("success!!!"); Sleep(1000); draw_map(); break;
			case 'c': clean_map(); system("cls"); draw_map(); break;
			case 'r': file_read(); draw_map(); break;
		}
	}
	
	
}

void home()
{
	cursor(0);
	custommap [ROW-3][(COL-1)/2] = 1;
	custommap[38][45]=0;print(45,38,' ');
	custommap[38][37]=0;print(37,38,' ');
	gotoxy((COL-1)/2, ROW-3);
	printf ("@");
	cursor(1);
}

void clean_map()
{
	for (i = 1; i < ROW-1; i++)
	for (j = 1; j < COL-1; j++)
		custommap [i][j] = 0;
	home();
}

void draw_map()
{
	gotoxy(96, 10);
	printf ("C)clean");
	gotoxy(96, 15);
	printf ("S)save");
	gotoxy(96, 20);
	printf ("R)reload");
	gotoxy(96, 25);
	printf ("Esc)exit");
	gotoxy(96, 30); printf("Use 1,2,4,5,0,.,space");
	gotoxy(96, 35);printf("You can't cover");
	gotoxy(96, 40);printf("(37,38)&(41,38)&(45,38)");
	cursor (0);
	for (i = 0; i < ROW; i++) {
		for (j = 0; j < COL; j++) {
			switch (custommap[i][j]) {
				case 9: color (7); print(j,i,176); break;
				case 1: color (12); print(j,i,176);break;
				case 2: color (15); print(j,i,177); break;
				case 0: printf (" "); break;
			}
		}
	}
	cursor (1);
}