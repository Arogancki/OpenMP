// Artur Ziemba 

#include "stdafx.h"
#include "GameOfLife.h"

#define Nframe 40
int timeInterval = (1000.0 / Nframe);

int main()
{
	GameOfLife sample(xxx/4, yyy/4, 200, 8);
	sample.makeGif("Game Of Life 2.gif", timeInterval, 4); 	return 0;
	do
	{
		system("cls");
		sample.print();
		std::this_thread::sleep_for(std::chrono::milliseconds(timeInterval));
	} while (sample.doStep());
	return 0;
}