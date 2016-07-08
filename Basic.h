#include<iostream>
#include<vector>
#include<windows.h>
#include<conio.h>
#define BACKCOLOR 15
#define SNAKE1COLOR 15
#define SNAKE2COLOR 7
#define FOODCOLOR 10
using namespace std;
void gotoxy(int x,int y)//位置函数
{
	COORD pos;
	pos.X=2*x;
	pos.Y=y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),pos);
}
void color(int a)//颜色函数
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),a);
}

class coordinate{
public:
	int x;
	int y;
	void setCoordinate(int xx,int yy){
		x=xx;
		y=yy;
	}
	coordinate(int xx=0,int yy=0):x(xx),y(yy){}
	friend int operator == (coordinate&c1,coordinate&c2){
		if(c1.x==c2.x&&c1.y==c2.y)return 1;
		else return 0;
	}
};

class board{
public:
	int **all;
	int Radius;  //盘的边长
	board(int r);
	~board();
	friend ostream& operator << (ostream&, board&);
	virtual void addFood()=0;
};
board::board(int r=15){
	int i,j;
	Radius=r;
	all=new int*[Radius+2];
	for(i=0;i<=Radius+1;i++){
		*(all+i)=new int [Radius+2];
		if(i==0||i==Radius+1){
			for(j=0;j<Radius+2;j++)
				*(*(all+i)+j)=0;
		}
		if(0<i&&i<Radius+1){
			for(j=0;j<Radius+2;j++){
				if(j==0)*(*(all+i)+j)=0;
				if(j==Radius+1)
					*(*(all+i)+j)=0;
				if(0<j&&j<Radius+1)
					*(*(all+i)+j)=1;
			}
		}
	}
}
board::~board(){
	for(int i=0;i<Radius+2;i++)
		delete [] *(all+i);
	delete [] all;
}
ostream& operator<<(ostream&output, board&game){
	int i,j;
	for(i=0;i<game.Radius+2;i++){
		for(j=0;j<game.Radius+2;j++){
			color(BACKCOLOR);
			if(*(*(game.all+i)+j))output<<"■";
			else output<<"□";
		}
		output<<std::endl;
	}
	return output;
}

class snake{
public:
	char direction;
	vector<coordinate>  body;
	int length;
	int score;
	snake();
	~snake();
	void move(int);
	int eatFood(int,int,int);
	void setDirection(char d){
		if(d=='w'||d=='s'||d=='a'||d=='d')
			direction=d;
		else{
			switch(d){
			case'i':direction='w';break;
			case'k':direction='s';break;
			case'j':direction='a';break;
			case'l':direction='d';break;
			}}}
	int judge(int);  
};
snake::snake(){
	length=2;
	score=0;
	coordinate temp1(5,6),temp2(5,5);
	body.push_back(temp2);
	body.push_back(temp1);
	direction='d';
}
snake::~snake(){
	body.~vector<coordinate>();
}
void snake::move(int type=0){       //带一个参数是为了双人模式中可以让不同的蛇颜色不同
	coordinate head(body[length-1]);
	switch(direction){
	case 'w': //upward
		body[length-1].x--;break;
	case 's': //downward
		body[length-1].x++;break;
	case 'a': //leftward
		body[length-1].y--;break;
	case 'd': //rightward
		body[length-1].y++;break;
	default:
		break;
	}
	coordinate temp(body[0]);
	for(int i=0;i<length-1;i++)
		body[i]=body[i+1];
	body[length-2]=head;
	gotoxy(body[length-1].y,body[length-1].x);
	if(type)color(SNAKE2COLOR);
	else color(SNAKE1COLOR);
	std::cout<<"★";
	color(BACKCOLOR);
	if(temp==body[length-1]);     //先判断之前的蛇尾是不是现在的蛇头，如果不是，就把之前蛇尾处的输出改为"■"
	else{
		gotoxy(temp.y,temp.x);
		std::cout<<"■";
	}
}
int snake::eatFood(int xx,int yy,int type=0){ //吃食返回1，没吃返回0；
	coordinate head(body[length-1]);
	if(head.x==xx&head.y==yy){
		length++;
		score++;
		coordinate  temp;
		switch(direction){
		case 'w': //upward
			temp.setCoordinate(head.x-1,head.y);break;
		case 's': //downward
			temp.setCoordinate(head.x+1,head.y);break;
		case 'a': //leftward
			temp.setCoordinate(head.x,head.y-1);break;
		case 'd': //rightward
			temp.setCoordinate(head.x,head.y+1);break;
		}
		body.push_back(temp);
		gotoxy(temp.y,temp.x);
		if(type)color(SNAKE2COLOR);
		else color(SNAKE1COLOR);
		std::cout<<"★";
		color(BACKCOLOR);
		return 1;
	}
	return 0;
}
int snake::judge(int Radius){
	int i=0;
	coordinate head(body[length-1]);
	if(head.x==0||head.x==Radius+1||head.y==0||head.y==Radius+1)return 1;
	for(i=0;i<length-1;i++)
		if(head.x==body[i].x&&head.y==body[i].y)return 1;
	return 0;
}

class snakeBoard:public snake,public board{
public:
	coordinate food;
	snakeBoard(int r);  
	int checkSnake(int i,int j); //检查一个点是不是在蛇身上,是则返回1，否则返回0
	//	int judge();  //判读游戏是否结束,是则返回1，否则返回0
	void addFood(); //产生新的食物
	void initDisplay();
};
void snakeBoard::addFood(){
	food.x=rand()%Radius+1;
	food.y=rand()%Radius+1;
	while(checkSnake(food.x,food.y)){  //只要生成的新食物还在蛇身上，就重新生成
		food.x=rand()%Radius+1;
		food.y=rand()%Radius+1;
	}
	gotoxy(food.y,food.x);
	color(FOODCOLOR);
	std::cout<<"●";
	color(BACKCOLOR);
	gotoxy(0,Radius+4);
}
snakeBoard::snakeBoard(int r=15):snake(),board(r){
	food.x=rand()%Radius+1;
	food.y=rand()%Radius+1;
}
int snakeBoard::checkSnake(int i,int j){   
	int num;
	for(num=0;num<length;num++)
		if(body[num].x==i&&body[num].y==j)return 1;
	return 0;
}
void snakeBoard::initDisplay(){
	cout<<*this;
	color(BACKCOLOR);
	gotoxy(Radius+7,5);
	std::cout<<"Score:"<<score;
	gotoxy(Radius+7,6);
	std::cout<<"Length:"<<length;
	color(SNAKE1COLOR);
	gotoxy(body[0].y,body[0].x);
	std::cout<<"★";
	gotoxy(body[1].y,body[1].x);
	std::cout<<"★";
	gotoxy(food.y,food.x);
	color(FOODCOLOR);
	std::cout<<"●";
	color(BACKCOLOR);
	gotoxy(0,Radius+3);

}
/*int snakeBoard::judge(){
if(body->x==0||body->x==Radius+1||body->y==0||body->y==Radius+1)return 1;
for(int i=1;i<length;i++)
if(body->x==(body+i)->x&&body->y==(body+i)->y)return 1;
return 0;
}*/

class Mode2Board:public board{
public:
	snake player1;
	snake player2;
	coordinate food;
	Mode2Board(int r);
	int checkSnakes(int x,int y);
	void initDisplay();
	int judge();
	void addFood();
};
Mode2Board::Mode2Board(int r=15):board(r){
	player2.body[0].setCoordinate(Radius-5,5);
	player2.body[1].setCoordinate(Radius-5,6);
	food.x=rand()%Radius+1;
	food.y=rand()%Radius+1;
}
int Mode2Board::checkSnakes(int x,int y){
	int num;
	for(num=0;num<player1.length;num++)
		if(player1.body[num].x==x&&player1.body[num].y==y)return 1;
	for(num=0;num<player2.length;num++)
		if(player2.body[num].x==x&&player2.body[num].y==y)return 1;
	return 0;
}
void Mode2Board::addFood(){
	food.x=rand()%Radius+1;
	food.y=rand()%Radius+1;
	while(checkSnakes(food.x,food.y)){  //只要生成的新食物还在蛇身上，就重新生成
		food.x=rand()%Radius+1;
		food.y=rand()%Radius+1;
	}
	gotoxy(food.y,food.x);
	color(FOODCOLOR);
	std::cout<<"●";
	color(BACKCOLOR);
	gotoxy(0,Radius+4);
}
int Mode2Board::judge(){   //检查游戏是否结束以及哪个玩家获胜，未结束返回0，player1获胜返回1，player2获胜返回2.
	if(player1.judge(Radius)) return 2;
	if(player2.judge(Radius))return 1;
	int i;
	for(i=0;i<player1.length-1;i++)
		if(player1.body[player1.length-1].x==player1.body[i].x&&player1.body[player1.length-1].y==player1.body[i].y)return 2;
	for(i=0;i<player2.length-1;i++)
		if(player2.body[player2.length-1].x==player2.body[i].x&&player2.body[player2.length-1].y==player2.body[i].y)return 1;
	for(i=0;i<player1.length-1;i++)
		if(player2.body[player2.length-1].x==player1.body[i].x&&player2.body[player2.length-1].y==player1.body[i].y)return 1;
	for(i=0;i<player2.length-1;i++)
		if(player1.body[player1.length-1].x==player2.body[i].x&&player1.body[player1.length-1].y==player2.body[i].y)return 2;
	if(player1.body[player1.length-1].x==player2.body[player2.length-1].x&&player1.body[player1.length-1].y==player2.body[player2.length-1].y){
		if(player1.score==player2.score)return 3;
		if(player1.score>player2.score)return 1;
		if(player1.score<player2.score)return 2;}
	return 0;
}
void Mode2Board::initDisplay(){
	cout<<*this;
	gotoxy(Radius+7,5);
	color(BACKCOLOR);
	std::cout<<"Player1:";
	gotoxy(Radius+7,6);
	std::cout<<"Score:"<<player1.score;
	gotoxy(Radius+7,7);
	std::cout<<"Length:"<<player1.length;
	gotoxy(Radius+7,9);
	std::cout<<"Player2:";
	gotoxy(Radius+7,10);
	std::cout<<"Score:"<<player2.score;
	gotoxy(Radius+7,11);
	std::cout<<"Length:"<<player2.length;
	color(SNAKE1COLOR);
	gotoxy(player1.body[1].y,player1.body[1].x);
	std::cout<<"★";
	gotoxy(player1.body[0].y,player1.body[0].x);
	std::cout<<"★";
	gotoxy(player2.body[0].y,player2.body[0].x);
	color(SNAKE2COLOR);
	std::cout<<"★";
	gotoxy(player2.body[1].y,player2.body[1].x);
	std::cout<<"★";
	gotoxy(food.y,food.x);
	color(FOODCOLOR);
	std::cout<<"●";
	color(BACKCOLOR);
	gotoxy(0,Radius+3);
}