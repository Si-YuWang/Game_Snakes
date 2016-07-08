#pragma once
#include<fstream>
#include"Basic.h"
extern int game();
using namespace std;
int record(int score){
	fstream file;
	file.open("E:\score.txt",ios::in|ios::app);
	if(!file){
		cout<<"Error, can not read previous highest score!"<<endl;
		return 0;}
	int highestScore;
	file>>highestScore;
	if(highestScore<score){
		cout<<"Congratulatiobs! You've broke the record!"<<endl;
		highestScore=score;}
	else cout<<"What a pity! You didn't break the record. Continue to progress!"<<endl;
	file.close();
	file.open("E:\score.txt",ios::out);
	file<<highestScore;
	file.close();
	return 1;
}
void Help(){
	cout<<"    Press w,s,a,d( player1) or i,k,j,l( player2) to change the moving direction to upward, downward, leftward and rightward."<<endl;
	cout<<"    In one player mode, as you getting longer and longer, you speed will get faster and faster unless you break some limitation. Once you touch the wall, or your head touch your body, you'ill die."<<endl;
	cout<<"    In two players mode, the speed you move is constant. Once a player's head touch the other one's body, the wall or his own body, he would die and the other player will win the game. If two players'heads meet, the one whose score is higher win, but if two players get the same score, they a get a draw."<<endl;
	cout<<"    Don't intend to backward, you will die because your head will touch your body!"<<endl;
	cout<<"Tips: Change the keyboard to English input mode in advance and enlarge the cmd window in case you can not see the whole board!"<<endl;
	cout<<"May you have fun!"<<endl;
	cout<<"Enter any key to quit"<<endl;
	char quit;
	cin>>quit;
}
void record(){
	fstream file;
	file.open("E:\score.txt",ios::app|ios::in);
	int num;
	file>>num;
	cout<<"The record is: "<<num<<endl;
	file.close();
	cout<<"Enter any key to quit"<<endl;
	char quit;
	cin>>quit;
}
void onePlayerMode(int size){
	system("cls");
	snakeBoard Game(size);
	Game.initDisplay();
	int score=0;
	while(1){
		if(kbhit()){
			char ch=getche();
			if(ch=='w'||ch=='s'||ch=='a'||ch=='d')
				Game.setDirection(ch);
			else {
				cout<<"Pausing......";
				cout<<"Enter any to restart.";
				char conti;
				cin>>conti;
				gotoxy(0,Game.Radius+3);
				printf("                                             \n");
				gotoxy(0,Game.Radius+3);
			}
		}
		if(Game.eatFood(Game.food.x,Game.food.y)){
			if(Game.length==Game.Radius*Game.Radius){
				cout<<"You win!"<<endl;
				break;
			}
			gotoxy(Game.Radius+7,5);
			cout<<"Score:"<<Game.score;
			gotoxy(Game.Radius+7,6);
			cout<<"Length:"<<Game.length;
			Game.addFood();
		}
		else Game.move();
		gotoxy(0,Game.Radius+3);
		Sleep(abs(300-2*Game.score)); //控制运动速度
		if(Game.judge()){
			score=Game.score;
			break;
		}
	}
	cout<<"Game over!"<<endl;
	record(score);
	cout<<"Enter anything to quit."<<endl;
	char hehe[5];
	cin>>hehe;
}
void twoPlayersMode(int size){
	Mode2Board Game(size);
	system("cls");
	Game.initDisplay();
	while(1){
		if(kbhit()){
			char ch=getche();
			if(ch=='w'||ch=='s'||ch=='a'||ch=='d')
				Game.player1.setDirection(ch);
			else{
				if(ch=='i'||ch=='k'||ch=='j'||ch=='l')
					Game.player2.setDirection(ch);
				else {
					cout<<"Pausing......";
					cout<<"Enter any to restart.";
					char conti;
					cin>>conti;
					gotoxy(0,Game.Radius+3);
					printf("                                             \n");
					gotoxy(0,Game.Radius+3);
				}}}
		if(Game.player1.eatFood(Game.food.x,Game.food.y)){
			gotoxy(Game.Radius+7,6);
			cout<<"Score:"<<Game.player1.score;
			gotoxy(Game.Radius+7,7);
			cout<<"Length:"<<Game.player1.length;
			Game.addFood();
		}
		else{
			if(Game.player2.eatFood(Game.food.x,Game.food.y,1)){
				gotoxy(Game.Radius+7,10);
				cout<<"Score:"<<Game.player2.score;
				gotoxy(Game.Radius+7,11);
				cout<<"Length:"<<Game.player2.length;
				Game.addFood();
			}
			else{
				Game.player1.move();
				Game.player2.move(1);
			}}
		gotoxy(0,Game.Radius+3);
		Sleep(200); //控制运动速度
		if(Game.judge()==1){
			cout<<"Player1 win!"<<endl;
			break;}
		if(Game.judge()==2){
			cout<<"Player2 win!"<<endl;
			break;}
		if(Game.judge()==3){
			cout<<"You two get a draw."<<endl;
			break;}
	}
	cout<<"Enter anything to quit."<<endl;
	char hehe[5];
	cin>>hehe;
}
void main(){
	color(15);
	while(1){
		system("cls");
		cout<<"Please select the function:"<<endl;
		cout<<"   1.start game"<<endl;
		cout<<"   2.help"<<endl;
		cout<<"   3.The highest single score"<<endl;
		cout<<"   4.Withdraw"<<endl;
		int select;
		cin>>select;
		switch(select){
		case(1):
			system("cls");
			cout<<"Please select the size of the board( between 13 and 22):";
			int size;
			cin>>size;
			while(size< 13||size>22){
				cout<<"Not suitable! select again!"<<endl;
				cin>>size;}
			cout<<"Please select a game mode:"<<endl;
			cout<<"1 player              2 player"<<endl;
			int key;
			cin>>key;
			if(key==1)onePlayerMode(size);
			if(key==2)twoPlayersMode(size);
			else cout<<"Error!"<<endl;
			break;
		case 2:Help();break;
		case 3:record();break;
		case 4:
			cout<<"Thank you! Goodbye!"<<endl;
			system("pause");
			exit(0);
		default:
			cout<<"Error! Drop out 3s later"<<endl;
			Sleep(3000);
			exit(0);
		}
	}
}