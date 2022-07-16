#pragma once
#include <random>

#define RED_PAIR 1
#define BLUE_PAIR 2
#define YELLOW_PAIR 3
#define GREEN_PAIR 4
#define PURPLE_PAIR 5
#define WHITE_PAIR 6

//Puyo
struct Puyo
{
	//Puyo Color
	int color;

	//Constructor
	Puyo(int colorNum) {
		//Choose Color with Random
		color = rand() % colorNum + 1;
	}
};

//PuyoPuyo
struct PuyoPuyo
{
	//Puyo x 2
	Puyo *mainPuyo;
	Puyo *subPuyo;

	int mainPuyoPositionX;
	int mainPuyoPositionY;

	int puyoDirection = 0;	//up, left, down, right

	//Constructor
	PuyoPuyo(int colorNum) {
		//Generate Puyos
		mainPuyo = new Puyo(colorNum);
		subPuyo = new Puyo(colorNum);

		mainPuyoPositionX = mainPuyoPositionY = -1;

		puyoDirection = 0;
	}
};
