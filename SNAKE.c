#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>//windows编程头文件
#include <time.h>
#include <conio.h>//控制台输入输出头文件
#include <unistd.h>
#define SNAKESIZE 100//蛇的身体最大节数
#define MAPWIDTH 58 //宽度
#define MAPHEIGHT 26//高度
#include"function.h"
//食物的坐标
struct fd{
	int x;
	int y;
}food;
 
//蛇的相关属性
struct sn{
	int speed;//蛇移动的速度
	int len;//蛇的长度
	int x[SNAKESIZE];//组成蛇身的每一个小方块中x的坐标
	int y[SNAKESIZE];//组成蛇身的每一个小方块中y的坐标
}snake;

void drawsnake();//主界面装饰用
int welcometogame();//进入及返回游戏界面用 
void color(int c);//改变文字颜色 
void drawMap();//绘制游戏边框 
void endgame();//结束界面 
int choose();//游戏结束后选择
void lostpicture();//失败界面 
void explanation();//帮助 
void framemaker();  //生成边框 
void tips();//游戏界面旁的说明 
void createFood();//随机生成食物 
int keyDown();//案件操作 
int snakestate();//蛇的状态 
void HideCursor();//用于隐藏光标 
void gotoxy(short x, short y);//从控制台移动光标
void File_out();//在文件中读取最高分
void File_in();//将最高分写进文件 
int key = 72;//表示蛇移动的方向，72为按下"↑"所代表的数字
int changeFlag = 0;//用来判断蛇是否吃掉了食物，这一步很重要，涉及到是否会有蛇身移动的效果以及蛇身增长的效果
int score = 0;//记录玩家的得分
int add=10;//记录吃掉一个食物获得的分数 
int highestscore=0; //记录最高成绩 
int endgamestate;//判断是以哪种方式结束游戏 

void HideCursor()//定义隐藏光标函数
{
	CONSOLE_CURSOR_INFO cursor;    
	cursor.bVisible = FALSE;    
	cursor.dwSize = sizeof(cursor);    
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);    
	SetConsoleCursorInfo(handle, &cursor);
}


void File_out()//读入最高成绩 
{
	FILE *fp;
	fp=fopen("save.txt","a+");
	fscanf(fp,"%d",&highestscore);
	fclose(fp);
}
void File_in()//写入最高成绩 
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
	printf("欢迎来到贪吃蛇的世界！");
	color(14);
    framemaker(27,74,18,24);
	color(10);
	gotoxy(35,20);
	printf("1.开始");
	gotoxy(55,20);
	printf("2.帮助");
	gotoxy(35,22);
	printf("3.返回主界面");
	gotoxy(29,27);
	color(3);
	printf("请选择[1 2 3]:[       \b\b\b\b\b\b]\b\b");        //\b为退格，使得光标处于[]中间
	color(14);
    scanf("%d", &n);    		//输入选项
    switch (n)
    {
    	case 1:					//选择开始游戏
    		system("cls");
			drawMap();        //创建地图
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
               
    	case 2:					//选择游戏说明
        	explanation();
        	break;
    	case 3:					//选择退出游戏
        	return 1;     		//退出游戏
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
		printf("撞到墙 游戏结束"); 
	}
	else if (endgamestate==2)
	{
		lostpicture();
		gotoxy(35,9);
		color(7);
		printf("咬到自己 游戏结束");
	}
	else if(endgamestate==3)
	{
		lostpicture();
		gotoxy(35,9);
		color(7);
		printf("你已经结束了游戏");
	}
	gotoxy(43,12);
	color(14);
	printf("您的得分是: %d",score);
	if(score>=highestscore)
	{
		color(10);
		gotoxy(33,16);
		printf("新纪录: %d",score);
		File_in();
	}
	else
	{
		color(10);
		gotoxy(33,16);
		printf("离最高分还差：%d",highestscore-score);
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
		printf("重玩一局 [1]");
		gotoxy(45,25);
		color(11);
		printf("选择：");
		gotoxy(50,25);
		printf ("    \b\b\b\b");
		scanf("%d", &n);
			
	    switch(n)
	    {
	    	case 1:
	    		system("cls");          //清屏
			    score=0;                //分数归零
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
	printf("说明（按任意键返回）");
	color(2);
    framemaker(20,76,6,22);
    color(3);
    gotoxy(30,8);
    printf("1.撞墙或咬到自己时游戏结束");
	color(10);
	gotoxy(30,11);
	printf("2.用↑.↓.←.→分别控制蛇的移动");
	color(14);
	gotoxy(30,14);
	printf("3.Esc键退出");
	HideCursor();
	getch();     //按任意键返回主界面 
	system("cls");
	drawsnake();
	return;
}
void tips()
{ 
    File_out();
    gotoxy(64,4);
    color(11);				
	printf("☆最高记录☆：%d",highestscore);	//打印最高分
	gotoxy(64,8);
	color(11);
	printf("当前分数: %d",score);
	color(15);
	gotoxy(73,11);
	printf("帮助"); 
	color(12);
	framemaker(60,13,80,25);
	color(3);
	gotoxy(64,14);
	printf("吃掉一个食物得10分");
	gotoxy(64,16);
	printf("撞墙或者咬到自己时游戏结束");
	gotoxy(64,18);
	printf("用↑ ↓ ← →分别控制蛇的移动");
	gotoxy(64,20);
	printf("随着游戏进行，蛇的移动速度会加快"); 
	gotoxy(64,22);
	printf("Esc键退出游戏");
}
void drawMap()
{
 for(int i=0;i<MAPWIDTH;i+=2)		//打印上下边框
    {
        gotoxy(i,0);
		color(6);			
        printf("□");
        gotoxy(i,26);
        printf("□");
    }
    for(int i=1;i<MAPHEIGHT;i++)		//打印左右边框
    {
        gotoxy(0,i);
        printf("□");                        
        gotoxy(56,i);
        printf("□");        
    }
	//随机生成初始食物
	while (1)
	{
		srand((unsigned int)time(NULL));
		food.x = rand() % 52 + 2;
		food.y = rand() % (MAPHEIGHT - 2) + 1;
		if (food.x % 2 == 0) break;//使食物的横坐标为偶数，防止食物只被吃了一半的情况发生 
	}
	//将光标移到食物的坐标处打印食物
	gotoxy(food.x, food.y);
	color(10);
	printf("★");
	//初始化蛇的属性
	snake.len = 3;
	snake.speed = 200;
	//在屏幕中间生成蛇头
	snake.x[0] = MAPWIDTH / 2 + 1;//x坐标为偶数
	snake.y[0] = MAPHEIGHT / 2;
	//打印蛇头
	gotoxy(snake.x[0], snake.y[0]);
	color(14);
	printf("■");
	//生成初试的蛇身
	for (int i = 1; i < snake.len; i++)
	{
		//蛇身的打印，纵坐标不变，横坐标为上一节蛇身的坐标值+2
		snake.x[i] = snake.x[i - 1] + 2;
		snake.y[i] = snake.y[i - 1];
		gotoxy(snake.x[i], snake.y[i]);
		color(14);
		printf("■");
	}
	//打印完蛇身后将光标移到屏幕最上方，避免光标在蛇身处一直闪烁
	gotoxy(MAPWIDTH - 2, 0);
	HideCursor();//隐藏光标 
	return;
}
int keyDown()
{
	tips();
	int pre_key = key;//记录前一个按键的方向
	if (_kbhit())//如果用户按下了键盘中的某个键
	{
		fflush(stdin);//清空缓冲区的字符
		//getch()读取方向键的时候，会返回两次，第一次调用返回0或者224，第二次调用返回的才是实际值
		key = _getch();//第一次调用返回的不是实际值
		key = _getch();//第二次调用返回实际值
	}
	/*
	*蛇移动时候先擦去蛇尾的一节
	*changeFlag为0表明此时没有吃到食物，因此每走一步就要擦除掉蛇尾，以此营造一个移动的效果
	*为1表明吃到了食物，就不需要擦除蛇尾，以此营造一个蛇身增长的效果
	*/
	if (changeFlag == 0)
	{
		gotoxy(snake.x[snake.len - 1], snake.y[snake.len - 1]);
		printf("  ");//在蛇尾处输出空格即擦去蛇尾
	}
 
	//将蛇的每一节依次向前移动一节（蛇头除外）
	for (int i = snake.len - 1; i > 0; i--)
	{
		snake.x[i] = snake.x[i - 1];
		snake.y[i] = snake.y[i - 1];
	}
	//蛇当前移动的方向不能和前一次的方向相反，比如蛇往左走的时候不能直接按右键往右走
	//如果当前移动方向和前一次方向相反的话，把当前移动的方向改为前一次的方向
	if (pre_key == 72 && key == 80)  key = 72;
	if (pre_key == 80 && key == 72)  key = 80;
	if (pre_key == 75 && key == 77)  key = 75;
	if (pre_key == 77 && key == 75)  key = 77;
	/**
	*控制台按键所代表的数字
	*"↑"：72
	*"↓"：80
	*"←"：75
	*"→"：77
	*/
 
	//判断蛇头应该往哪个方向移动
	switch (key)
	{
	case 75:
		snake.x[0] -= 2;//往左
		break;
	case 77:
		snake.x[0] += 2;//往右
		break;
	case 72:
		snake.y[0]--;//往上
		break;
	case 80:
		snake.y[0]++;//往下
		break;
	case 27://按esc键，直接到结束界面
		 endgamestate=3;    
		return 1;
	}
	//打印出蛇头
	gotoxy(snake.x[0], snake.y[0]);
	color(14);
	printf("■");
	gotoxy(MAPWIDTH - 2, 0);
	//由于目前没有吃到食物，changFlag值为0
	changeFlag = 0;
	return 0;
}
void createFood()
{
	if (snake.x[0] == food.x && snake.y[0] == food.y)//蛇头碰到食物
	{
		//蛇头碰到食物即为要吃掉这个食物了，因此需要再次生成一个食物
		while (1)
		{
			int flag = 1;
			srand((unsigned int)time(NULL));
			food.x = rand() % 52 + 2;
			food.y = rand() % (MAPHEIGHT - 2) + 1;
 
			//随机生成的食物不能在蛇的身体上
			for (int i = 0; i < snake.len; i++)
			{
				if (snake.x[i] == food.x && snake.y[i] == food.y)
				{
					flag = 0;
					break;
				}
			}
			/*生成的食物横坐标的奇偶必须和蛇头所在坐标的奇偶一致，因为一个字符占两个字节位置，若不一致会导致吃食物的时候只吃到一半
			故随机生成的食物不能横坐标为奇数，也不能在蛇身，否则重新生成*/ 
			if (flag && food.x % 2 == 0)
				break;
		}
 
		//绘制食物
		gotoxy(food.x, food.y);
		color(10);
		printf("★");
		snake.len++;//吃到食物，蛇身长度加1
		score=score+add;//每个食物得10分
		snake.speed -= 5;//随着吃的食物越来越多，速度会越来越快
		changeFlag = 1;//很重要，因为吃到了食物，就不用再擦除蛇尾的那一节，以此来造成蛇身体增长的效果
	}
	return;
}
int snakestate()
{
	//蛇头碰到上下边界，游戏结束
	if (snake.y[0] == 0 || snake.y[0] == MAPHEIGHT)
	    {	
	      endgamestate=1;
	      return 0;
		}	
	//蛇头碰到左右边界，游戏结束
	if (snake.x[0] == 0 || snake.x[0] == MAPWIDTH-2)
		{	
	       endgamestate=1;
	       return 0;
		}	
	//蛇头碰到蛇身，游戏结束
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

void SNAKE()//主函数 
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



