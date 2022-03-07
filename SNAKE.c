#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>//windows���ͷ�ļ�
#include <time.h>
#include <conio.h>//����̨�������ͷ�ļ�
#include <unistd.h>
#define SNAKESIZE 100//�ߵ�����������
#define MAPWIDTH 58 //���
#define MAPHEIGHT 26//�߶�
#include"function.h"
//ʳ�������
struct fd{
	int x;
	int y;
}food;
 
//�ߵ��������
struct sn{
	int speed;//���ƶ����ٶ�
	int len;//�ߵĳ���
	int x[SNAKESIZE];//��������ÿһ��С������x������
	int y[SNAKESIZE];//��������ÿһ��С������y������
}snake;

void drawsnake();//������װ����
int welcometogame();//���뼰������Ϸ������ 
void color(int c);//�ı�������ɫ 
void drawMap();//������Ϸ�߿� 
void endgame();//�������� 
int choose();//��Ϸ������ѡ��
void lostpicture();//ʧ�ܽ��� 
void explanation();//���� 
void framemaker();  //���ɱ߿� 
void tips();//��Ϸ�����Ե�˵�� 
void createFood();//�������ʳ�� 
int keyDown();//�������� 
int snakestate();//�ߵ�״̬ 
void HideCursor();//�������ع�� 
void gotoxy(short x, short y);//�ӿ���̨�ƶ����
void File_out();//���ļ��ж�ȡ��߷�
void File_in();//����߷�д���ļ� 
int key = 72;//��ʾ���ƶ��ķ���72Ϊ����"��"�����������
int changeFlag = 0;//�����ж����Ƿ�Ե���ʳ���һ������Ҫ���漰���Ƿ���������ƶ���Ч���Լ�����������Ч��
int score = 0;//��¼��ҵĵ÷�
int add=10;//��¼�Ե�һ��ʳ���õķ��� 
int highestscore=0; //��¼��߳ɼ� 
int endgamestate;//�ж��������ַ�ʽ������Ϸ 

void HideCursor()//�������ع�꺯��
{
	CONSOLE_CURSOR_INFO cursor;    
	cursor.bVisible = FALSE;    
	cursor.dwSize = sizeof(cursor);    
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);    
	SetConsoleCursorInfo(handle, &cursor);
}


void File_out()//������߳ɼ� 
{
	FILE *fp;
	fp=fopen("save.txt","a+");
	fscanf(fp,"%d",&highestscore);
	fclose(fp);
}
void File_in()//д����߳ɼ� 
{
	FILE *fp;
	fp=fopen("save.txt","w+");
	fprintf(fp,"%d",score);
	fclose(fp);
}
void framemaker(int x1,int x2, int y1, int y2)
{
	int i,j;
	for(j=y1;j<=y2;j++)
	{
		for(i=x1;i<=x2;i++)
		{
			gotoxy(i,j);
			if(j==y1||j==y2) printf("-");
			else if(i==x1||i==x2) printf("|");
		}
	}
}
void drawsnake()
{
	gotoxy (0,0);
	color(2);
	printf("                                                                                         \n");
	printf("                       __________       ___                                              \n");
	printf("                      /          \\     / \\ \\    |____      __\\__                     \n");
	printf("                     /  ________  \\   / ___ \\  _/ __     | |   /                       \n");
	printf("                     |  |      |__|     _/_   |_|  /    [|] |/                           \n");
	printf("                     |  |              | | |      /     _|_ \\__/                        \n");
	printf("                     \\  \\_______        / \\      |___/        ____                    \n");
	printf("                      \\         \\    ____ ____      ____   __ |  |  ___   ______       \n");
	printf("                       \\_______  \\   |  |/    \\    /    \\_/ / |  | /  /  /      \\   \n");
	printf("                               \\  \\  |    ___  \\  / ____   /  |  |/  /  /  ____  \\   \n");
	printf("                     __        |  |  |   /   \\  \\ | |  |  /   |     /  |  /____\\  |   \n");
	printf("                    \\  \\_______|  |  |  |    |  | | |__|  |   |     \\  |  ________/   \n");
	printf("                     \\            /  |  |    |  |  \\       \\  |  |\\  \\  \\  \\____  \n");
	printf("                      \\__________/   |__|    |__|   \\___/\\__\\ |__| \\__\\  \\______/ \n");	
}
void lostpicture()
{
	system("cls");
	color(11);
	framemaker(30,82,6,18);
}
int welcometogame()
{
	fflush(stdin);
	drawsnake();
	int n,i,j;
	gotoxy(48,16);
	color(11);
	printf("��ӭ����̰���ߵ����磡");
	color(14);
    framemaker(27,74,18,24);
	color(10);
	gotoxy(35,20);
	printf("1.��ʼ");
	gotoxy(55,20);
	printf("2.����");
	gotoxy(35,22);
	printf("3.����������");
	gotoxy(29,27);
	color(3);
	printf("��ѡ��[1 2 3]:[       \b\b\b\b\b\b]\b\b");        //\bΪ�˸�ʹ�ù�괦��[]�м�
	color(14);
    scanf("%d", &n);    		//����ѡ��
    switch (n)
    {
    	case 1:					//ѡ��ʼ��Ϸ
    		system("cls");
			drawMap();        //������ͼ
			key=72;
            while (1)
	           {
		         if (keyDown()) break;
		         if (snakestate()==0) break;
		         createFood();
		         Sleep(snake.speed);
		       }
		       endgame();
	             break;
               
    	case 2:					//ѡ����Ϸ˵��
        	explanation();
        	break;
    	case 3:					//ѡ���˳���Ϸ
        	return 1;     		//�˳���Ϸ
        	break;
        default:
			return 0;
    }
}
void endgame() 
{
	system("cls");
	if(endgamestate==1)
	{
		lostpicture();
		gotoxy(35,9);
		color(7);
		printf("ײ��ǽ ��Ϸ����"); 
	}
	else if (endgamestate==2)
	{
		lostpicture();
		gotoxy(35,9);
		color(7);
		printf("ҧ���Լ� ��Ϸ����");
	}
	else if(endgamestate==3)
	{
		lostpicture();
		gotoxy(35,9);
		color(7);
		printf("���Ѿ���������Ϸ");
	}
	gotoxy(43,12);
	color(14);
	printf("���ĵ÷���: %d",score);
	if(score>=highestscore)
	{
		color(10);
		gotoxy(33,16);
		printf("�¼�¼: %d",score);
		File_in();
	}
	else
	{
		color(10);
		gotoxy(33,16);
		printf("����߷ֻ��%d",highestscore-score);
	}
	if (choose())
    	return;
}
int choose()
{
	while (1) {
		int n;
		gotoxy(30,23);
		color(12);
		printf("����һ�� [1]");
		gotoxy(45,25);
		color(11);
		printf("ѡ��");
		gotoxy(50,25);
		printf ("    \b\b\b\b");
		scanf("%d", &n);
			
	    switch(n)
	    {
	    	case 1:
	    		system("cls");          //����
			    score=0;                //��������
			    return 0;
			    break;
	    }
	}
}
void explanation()
{ 
    int i,j;
	system("cls");
	color(15);
	gotoxy(44,3);
	color(8);
	printf("˵��������������أ�");
	color(2);
    framemaker(20,76,6,22);
    color(3);
    gotoxy(30,8);
    printf("1.ײǽ��ҧ���Լ�ʱ��Ϸ����");
	color(10);
	gotoxy(30,11);
	printf("2.�á�.��.��.���ֱ�����ߵ��ƶ�");
	color(14);
	gotoxy(30,14);
	printf("3.Esc���˳�");
	HideCursor();
	getch();     //����������������� 
	system("cls");
	drawsnake();
	return;
}
void tips()
{ 
    File_out();
    gotoxy(64,4);
    color(11);				
	printf("����߼�¼�%d",highestscore);	//��ӡ��߷�
	gotoxy(64,8);
	color(11);
	printf("��ǰ����: %d",score);
	color(15);
	gotoxy(73,11);
	printf("����"); 
	color(12);
	framemaker(60,13,80,25);
	color(3);
	gotoxy(64,14);
	printf("�Ե�һ��ʳ���10��");
	gotoxy(64,16);
	printf("ײǽ����ҧ���Լ�ʱ��Ϸ����");
	gotoxy(64,18);
	printf("�á� �� �� ���ֱ�����ߵ��ƶ�");
	gotoxy(64,20);
	printf("������Ϸ���У��ߵ��ƶ��ٶȻ�ӿ�"); 
	gotoxy(64,22);
	printf("Esc���˳���Ϸ");
}
void drawMap()
{
 for(int i=0;i<MAPWIDTH;i+=2)		//��ӡ���±߿�
    {
        gotoxy(i,0);
		color(6);			
        printf("��");
        gotoxy(i,26);
        printf("��");
    }
    for(int i=1;i<MAPHEIGHT;i++)		//��ӡ���ұ߿�
    {
        gotoxy(0,i);
        printf("��");                        
        gotoxy(56,i);
        printf("��");        
    }
	//������ɳ�ʼʳ��
	while (1)
	{
		srand((unsigned int)time(NULL));
		food.x = rand() % 52 + 2;
		food.y = rand() % (MAPHEIGHT - 2) + 1;
		if (food.x % 2 == 0) break;//ʹʳ��ĺ�����Ϊż������ֹʳ��ֻ������һ���������� 
	}
	//������Ƶ�ʳ������괦��ӡʳ��
	gotoxy(food.x, food.y);
	color(10);
	printf("��");
	//��ʼ���ߵ�����
	snake.len = 3;
	snake.speed = 200;
	//����Ļ�м�������ͷ
	snake.x[0] = MAPWIDTH / 2 + 1;//x����Ϊż��
	snake.y[0] = MAPHEIGHT / 2;
	//��ӡ��ͷ
	gotoxy(snake.x[0], snake.y[0]);
	color(14);
	printf("��");
	//���ɳ��Ե�����
	for (int i = 1; i < snake.len; i++)
	{
		//����Ĵ�ӡ�������겻�䣬������Ϊ��һ�����������ֵ+2
		snake.x[i] = snake.x[i - 1] + 2;
		snake.y[i] = snake.y[i - 1];
		gotoxy(snake.x[i], snake.y[i]);
		color(14);
		printf("��");
	}
	//��ӡ������󽫹���Ƶ���Ļ���Ϸ���������������һֱ��˸
	gotoxy(MAPWIDTH - 2, 0);
	HideCursor();//���ع�� 
	return;
}
int keyDown()
{
	tips();
	int pre_key = key;//��¼ǰһ�������ķ���
	if (_kbhit())//����û������˼����е�ĳ����
	{
		fflush(stdin);//��ջ��������ַ�
		//getch()��ȡ�������ʱ�򣬻᷵�����Σ���һ�ε��÷���0����224���ڶ��ε��÷��صĲ���ʵ��ֵ
		key = _getch();//��һ�ε��÷��صĲ���ʵ��ֵ
		key = _getch();//�ڶ��ε��÷���ʵ��ֵ
	}
	/*
	*���ƶ�ʱ���Ȳ�ȥ��β��һ��
	*changeFlagΪ0������ʱû�гԵ�ʳ����ÿ��һ����Ҫ��������β���Դ�Ӫ��һ���ƶ���Ч��
	*Ϊ1�����Ե���ʳ��Ͳ���Ҫ������β���Դ�Ӫ��һ������������Ч��
	*/
	if (changeFlag == 0)
	{
		gotoxy(snake.x[snake.len - 1], snake.y[snake.len - 1]);
		printf("  ");//����β������ո񼴲�ȥ��β
	}
 
	//���ߵ�ÿһ��������ǰ�ƶ�һ�ڣ���ͷ���⣩
	for (int i = snake.len - 1; i > 0; i--)
	{
		snake.x[i] = snake.x[i - 1];
		snake.y[i] = snake.y[i - 1];
	}
	//�ߵ�ǰ�ƶ��ķ����ܺ�ǰһ�εķ����෴�������������ߵ�ʱ����ֱ�Ӱ��Ҽ�������
	//�����ǰ�ƶ������ǰһ�η����෴�Ļ����ѵ�ǰ�ƶ��ķ����Ϊǰһ�εķ���
	if (pre_key == 72 && key == 80)  key = 72;
	if (pre_key == 80 && key == 72)  key = 80;
	if (pre_key == 75 && key == 77)  key = 75;
	if (pre_key == 77 && key == 75)  key = 77;
	/**
	*����̨���������������
	*"��"��72
	*"��"��80
	*"��"��75
	*"��"��77
	*/
 
	//�ж���ͷӦ�����ĸ������ƶ�
	switch (key)
	{
	case 75:
		snake.x[0] -= 2;//����
		break;
	case 77:
		snake.x[0] += 2;//����
		break;
	case 72:
		snake.y[0]--;//����
		break;
	case 80:
		snake.y[0]++;//����
		break;
	case 27://��esc����ֱ�ӵ���������
		 endgamestate=3;    
		return 1;
	}
	//��ӡ����ͷ
	gotoxy(snake.x[0], snake.y[0]);
	color(14);
	printf("��");
	gotoxy(MAPWIDTH - 2, 0);
	//����Ŀǰû�гԵ�ʳ�changFlagֵΪ0
	changeFlag = 0;
	return 0;
}
void createFood()
{
	if (snake.x[0] == food.x && snake.y[0] == food.y)//��ͷ����ʳ��
	{
		//��ͷ����ʳ�ＴΪҪ�Ե����ʳ���ˣ������Ҫ�ٴ�����һ��ʳ��
		while (1)
		{
			int flag = 1;
			srand((unsigned int)time(NULL));
			food.x = rand() % 52 + 2;
			food.y = rand() % (MAPHEIGHT - 2) + 1;
 
			//������ɵ�ʳ�ﲻ�����ߵ�������
			for (int i = 0; i < snake.len; i++)
			{
				if (snake.x[i] == food.x && snake.y[i] == food.y)
				{
					flag = 0;
					break;
				}
			}
			/*���ɵ�ʳ����������ż�������ͷ�����������żһ�£���Ϊһ���ַ�ռ�����ֽ�λ�ã�����һ�»ᵼ�³�ʳ���ʱ��ֻ�Ե�һ��
			��������ɵ�ʳ�ﲻ�ܺ�����Ϊ������Ҳ����������������������*/ 
			if (flag && food.x % 2 == 0)
				break;
		}
 
		//����ʳ��
		gotoxy(food.x, food.y);
		color(10);
		printf("��");
		snake.len++;//�Ե�ʳ������ȼ�1
		score=score+add;//ÿ��ʳ���10��
		snake.speed -= 5;//���ųԵ�ʳ��Խ��Խ�࣬�ٶȻ�Խ��Խ��
		changeFlag = 1;//����Ҫ����Ϊ�Ե���ʳ��Ͳ����ٲ�����β����һ�ڣ��Դ������������������Ч��
	}
	return;
}
int snakestate()
{
	//��ͷ�������±߽磬��Ϸ����
	if (snake.y[0] == 0 || snake.y[0] == MAPHEIGHT)
	    {	
	      endgamestate=1;
	      return 0;
		}	
	//��ͷ�������ұ߽磬��Ϸ����
	if (snake.x[0] == 0 || snake.x[0] == MAPWIDTH-2)
		{	
	       endgamestate=1;
	       return 0;
		}	
	//��ͷ����������Ϸ����
	for (int i = 1; i < snake.len; i++)
	{
		if (snake.x[i] == snake.x[0] && snake.y[i] == snake.y[0])
		{	
	       endgamestate=2;
	       return 0;
		}	
	}
	return 1;
}

void SNAKE()//������ 
{
	system("mode con cols=100 lines=30");
	SetConsoleOutputCP(936);
	while (1) {
    	if (welcometogame())
			break;	
	}
	getchar();
	fflush(stdin);
	return;
}



