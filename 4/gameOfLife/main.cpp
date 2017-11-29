// Artur Ziemba 

#include "stdafx.h"
#include "GameOfLife.h"

#define Nframe 500
int timeInterval = (1000.0 / Nframe);

int main()
{
	GameOfLife sample(500, 500, 2000, 8);
	sample.makeGif("Game Of Life.gif", timeInterval, 8);	return 0;
	do
	{
		system("cls");
		sample.print();
		std::this_thread::sleep_for(std::chrono::milliseconds(timeInterval));
	} while (sample.doStep());
	return 0;
}