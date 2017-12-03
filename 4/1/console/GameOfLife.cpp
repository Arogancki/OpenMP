#include <math.h>       /* floor */
#include <stdio.h>      /* printf, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <sstream>
#include <string>       // std::string
#include <iostream>     // std::cout
#include <sstream> 
#include "GameOfLife.h"
#include <omp.h>
#include <sstream>

void GameOfLife::ch(){
	
	#pragma omp parallel
	{
        #pragma omp single nowait 
		{ 
		printf("Starting calculation\n"); 
		}
		
		printf("rest\n");
		
	}
	
}

void GameOfLife::realConstructor(int _width, int _height, int _maxCycles, int _numberOfThreads)
{
	wholetime = 0.0;
	if (_width < horizon + 2)
		_width = horizon + 2;
	if (_height < horizon + 2)
		_height = horizon + 2;
	width = _width;
	height = _height;
	maxCycles = _maxCycles;
	int size = width*height;
	if (_numberOfThreads > size)	numberOfThreads = size;
	else if (_numberOfThreads>0)		numberOfThreads = _numberOfThreads;
	else							numberOfThreads = 1;
	int step = size / numberOfThreads;
	srand(time(NULL));
	for (int i = 0; i < size; i++)
	{
		field  *temp = new field;
		temp->actual = (rand()% 100) < density;
		temp->prev = (rand() % 100) < density;
		fields.push_back(*temp);
	}
	cout << std::endl << std::endl;
	cycle = -1;
	doStep();
}

GameOfLife::GameOfLife(int _width, int _height, int _maxCycles, int _numberOfThreads)
{
	srand(time(NULL));
	realConstructor(_width, _height, _maxCycles,  _numberOfThreads);
}

GameOfLife::GameOfLife(int _width, int _height)
{
	srand(time(NULL));
	realConstructor(_width, _height, 0, (rand() % ((_width*_height) - 1)) + 1);
}

void GameOfLife::Classic(int i, int sum){
	// living
	if (fields[i].prev){
		if (sum != 3 && sum != 2)
			fields[i].actual = false;
	}
	// dead
	else{
		if (sum == 3)
			fields[i].actual = true;
	}
}

bool GameOfLife::doStep(int number)
{
	bool _return = false;
	omp_set_dynamic(0); // Explicitly disable dynamic teams
	omp_set_num_threads(this->numberOfThreads);

	double timeStamp = omp_get_wtime();
	for (int j = 0; j<number; j++)
	{
		if (cycle++ < maxCycles && maxCycles>0)
		{
			_return = true;

			#pragma omp parallel for 
			for (int i = 0; i < this->fields.size(); ++i) 
				fields[i].prev = fields[i].actual;

			#pragma omp parallel for 
			for (int i = 0; i < this->fields.size(); ++i)
			{
				//sprawdzam po prev ale aktualizuje actual
				int x = i % this->width;
				int y = floor((double)i / (double)this->height);
				// calculate how many neighbours lives
				int sum = 0;
				for (int row = -horizon; row <= horizon; row++)
					for (int col = -horizon; col <= horizon; col++)
					{
						//dont calculate a current one
						if (row == 0 && col == 0)
							continue;
						int neiX = x + row;
						int neiY = y + col;
						if (neiX < 0)	neiX = width - 1;
						else if (neiX >= width)	neiX = 0;
						if (neiY < 0)	neiY = height - 1;
						else if (neiY >= height)	neiY = 0;
						if (fields[(neiX*width) + neiY].prev)
							sum++;
					}
				Classic(i, sum);
			}
		}
		else
			break;
	}
	wholetime += omp_get_wtime() - timeStamp;
	return _return;
}

bool GameOfLife::doStep()
{
	return doStep(1);
}

void GameOfLife::makeGif(int timeInterval, int pixelOnPixel)
{
	do
	{

	} while (doStep());
}

void GameOfLife::saveStates(int start, int end)
{
	for (int i=start;i<end;i++)
		fields[i].prev = fields[i].actual;
}

void GameOfLife::changeStates(int who)
{
		int x = 0;
		int y = 0;
		srand(time(NULL));
		int sumOfLiving[5] = {0,0,0,0,0};
		int g = (rand()%3)-1;
		x = x+g;
		if (x < 0)				x = width + g;
		else if (x >= width)	x = g - 1;
		g = (rand() % 3) - 1;
		y = y+g;
		if (y < 0)				y = height + g;
		else if (y >= height)	y = g - 1;
		for (int j = -horizon; j <= horizon; j++)
			for (int k = -horizon; k <= horizon; k++)
			{
				if (j || k)
				{
					int neighbor_x = x + j;
					int neighbor_y = y + k;
					if (neighbor_x < 0)				neighbor_x = width + j;
					else if (neighbor_x >= width)	neighbor_x = j - 1;
					if (neighbor_y < 0)				neighbor_y = height + k;
					else if (neighbor_y >= height)	neighbor_y = k - 1;
					if (0!=fields[(width*neighbor_y) + neighbor_x].prev)
					sumOfLiving[fields[(width*neighbor_y) + neighbor_x].prev]++;
				}
			}
		int max = -1;
		for (int i = 0; i < 5; i++)
			if (max < sumOfLiving[i]) max = i;
		if((max==0||max==who+1)&& fields[(width*y) + x].prev!=who+1)
		{
			fields[(width*y) + x].actual = who+1;
			
		}
}

GameOfLife::~GameOfLife()
{
	if (!fields.empty())
			fields.clear();
}

void GameOfLife::print()
{
	for (int i = 0; i < height+2; i++)
		cout << "o";
	for (int i = 0; i < height; i++)
	{
		cout << "\no";
		for (int j = 0; j < width; j++)
		{
			if (fields[(width*i) + j].prev==red)	cout << "r";
			else if(fields[(width*i) + j].prev==green)	cout << "g";
			else if(fields[(width*i) + j].prev==blue)	cout << "b";
			else if(fields[(width*i) + j].prev==black)	cout << "w";
			else	cout << " ";
		}
		cout << "o";
	}
	cout << "\n";
	for (int i = 0; i < height + 2; i++)
		cout << "o";
	cout << "\n";
}