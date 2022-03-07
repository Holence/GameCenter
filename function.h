#ifndef _FUNCTION_
#define _FUNCTION_

COORD coord;
void color(int c)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

int randomize(int n){
	n++;
	static unsigned int seed=0;
	seed++;
	srand(time(NULL) + seed * seed);
	return rand()%n;
}

void cursor(int n)
{
	CONSOLE_CURSOR_INFO cursor;
	cursor.dwSize = 1;
	if (n)
		cursor.bVisible = 1;
	else 
		cursor.bVisible = 0;
	cursor.dwSize = 100;
	SetConsoleCursorInfo (GetStdHandle(STD_OUTPUT_HANDLE), &cursor);

} 

void gotoxy(short x,short y){
	coord.X=x;
	coord.Y=y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
}


void print(short x,short y,char a){
	coord.X=x;
	coord.Y=y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
	printf("%c",a);
	return;
}



#endif
