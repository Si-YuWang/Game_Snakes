#pragma once
#include"Basic.h"
extern int game();
using namespace std;
void main(){
	snakeBoard Game(20);
	int i;
	Game.display();
	/*while(1){
		if(kbhit())
			Game.setDirection(getche());
		if(Game.eatFood(Game.food.x,Game.food.y))Game.addFood();
		else Game.move();
		system("cls");
		Game.display();
		Sleep(100);
	}*/
	gotoxy(11,1);
	cout<<"ha";
	/*cout<<"Game over!"<<endl;
	cout<<"Please enter q to drop or c to continue:"<<endl;*/
	//game();
	system("pause");
}