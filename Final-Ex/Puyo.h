#pragma once
#include <random>

//Puyo Colors
enum Color {
	Red,
	Blue,
	Yellow,
	Green,
	Purple
};

//Puyo
struct Puyo
{
	//Puyo Color
	Color color;

	//Constructor
	Puyo(int colorNum) {
		//Choose Color with Random
		switch (rand() % colorNum)
		{
		case 0:
			color = Red;
			break;

		case 1:
			color = Blue;
			break;

		case 2:
			color = Yellow;
			break;

		case 3:
			color = Green;
			break;

		case 4:
			color = Purple;
			break;

		default:
			color = Red;
			break;
		}
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
