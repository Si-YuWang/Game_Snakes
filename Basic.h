#include<iostream>
#include<windows.h>
#include<conio.h>
#include<deque>
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



void stop(){}



class coordinate{
public:
	int x;
	int y;
	void setCoordinate(int xx,int yy){
		x=xx;
		y=yy;
	}
};
class board{
protected:
	int Radius;  //盘的边长
	int **all;
public:
	board(int r);
	~board();
	void display();
};
board::board(int r=13){
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
void board::display(){
	int i,j;
	for(i=0;i<Radius+2;i++){
		for(j=0;j<Radius+2;j++){
			if(*(*(all+i)+j))std::cout<<"■";
			else std::cout<<"□";
		}
		std::cout<<std::endl;
	}
}


class snake{
protected:
	int length;
	int score;
	char direction;
	coordinate *body;
public:
	snake();
	~snake();
	void move();
	int eatFood(int,int);
	void setDirection(char d){
		direction=d;
	}
};
snake::snake(){
	length=2;
	score=0;
	body=new coordinate [length];
	body->setCoordinate(5,6);
	(body+1)->setCoordinate(5,5);
	direction='d';
}
snake::~snake(){
	delete body;
}
void snake::move(){  
	coordinate temp;
	temp.setCoordinate(body->x,body->y);
	switch(direction){
	case 'w': //upward
		body->x--;break;
	case 's': //downward
		body->x++;break;
	case 'a': //leftward
		body->y--;break;
	case 'd': //rightward
		body->y++;break;
	default:
		break;
	}
	for(int i=length-1;i>=2;i--)
		*(body+i)=*(body+i-1);
	*(body+1)=temp;
}
int snake::eatFood(int xx,int yy){ //吃食返回1，没吃返回0；
	if(body->x==xx&&body->y==yy){
		length++;
		score++;
		coordinate* temp=new coordinate [length];
		switch(direction){
		case 'w': //upward
			temp->x=body->x-1;
			temp->y=body->y;break;
		case 's': //downward
			temp->x=body->x+1;
			temp->y=body->y;break;
		case 'a': //leftward
			temp->y=body->y-1;
			temp->x=body->x;break;
		case 'd': //rightward
			temp->y=body->y+1;
			temp->x=body->x;break;
		default:
			stop();break;
		}
		for(int i=1;i<length;i++){
			(temp+i)->x=(body+i-1)->x;
			(temp+i)->y=(body+i-1)->y;
		}
		delete body;
		body=new coordinate[length];
		for(int i=0;i<length;i++){
			(body+i)->x=(temp+i)->x;
			(body+i)->y=(temp+i)->y;
		}
		delete temp;
		return 1;
	}
	return 0;
}

class snakeBoard:public snake,public board{
public:
	coordinate food;
	snakeBoard(int r);  
	int checkSnake(int i,int j); //检查一个点是不是在蛇身上,是则返回1，否则返回0
	void display();  //输出整个board
	int judge();  //判读游戏是否结束,是则返回1，否则返回0
	void addFood(); //产生新的事物
};
void snakeBoard::addFood(){
	food.x=rand()%Radius+1;
	food.y=rand()%Radius+1;
}
snakeBoard::snakeBoard(int r=13):snake(),board(r){
	addFood();
}
int snakeBoard::checkSnake(int i,int j){   
	int num;
	for(num=0;num<length;num++)
		if((body+num)->x==i&&(body+num)->y==j)return 1;
	return 0;
}
void snakeBoard::display(){
	int i,j;
	for(i=0;i<Radius+2;i++){
		for(j=0;j<Radius+2;j++){
			if(*(*(all+i)+j)&&checkSnake(i,j))std::cout<<"★";
			else{
				if(food.x==i&&food.y==j)std::cout<<"●";
				else{
					if(*(*(all+i)+j))std::cout<<"■";
					else std::cout<<"□";
				}
			}
		}
		std::cout<<std::endl;
	}
}
int snakeBoard::judge(){
	if(body->x==0||body->x==Radius+1||body->y==0||body->y==Radius+1)return 1;
	if(checkSnake(body->x,body->y))return 1;
	return 0;
}