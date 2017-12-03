#include "stdafx.h"
#include <sstream>
#include <string>       // std::string
#include <iostream>     // std::cout
#include <sstream> 
#include "GameOfLife.h"
#include "gif.h"
#include <omp.h>
#include <sstream>

void GameOfLife::ch()
{
	omp_set_dynamic(0); // Explicitly disable dynamic teams
	omp_set_num_threads(4);

	omp_lock_t lock;
	omp_init_lock(&lock);
	#pragma omp parallel sections
	{
		#pragma omp section
		{
			#pragma omp parallel 
			{
				cout << "1 hello world!";
			}
		}
		#pragma omp section
		{
			#pragma omp parallel 
			{
				omp_set_lock(&lock);
				cout << "2 hello world!";
				omp_unset_lock(&lock);
			}
		}
	}
}

void GameOfLife::realConstructor(int _width, int _height, int _maxCycles, int _numberOfThreads)
{
	if (_width < horizon + 2)
		_width = horizon + 2;
	if (_height < horizon + 2)
		_height = horizon + 2;
	wholetime = 0.0;
	pixels = NULL;
	width = _width;
	height = _height;
	maxCycles = _maxCycles;
	int size = width*height;
	if (_numberOfThreads > size)	numberOfThreads = size;
	else if (_numberOfThreads>0)		numberOfThreads = _numberOfThreads;
	else							numberOfThreads = 1;
	int step = size / numberOfThreads;
	srand(time(NULL) + ((21 * 13) % 43) + ((7) % 13) + reinterpret_cast<int>(&step));
	for (int i = 0; i < size; i++)
	{
		field  *temp = new field;
		temp->actual = 0;
		bool czy = ((rand() % 100) < density);
		if (czy)
		{
			if (rand() %2){
				temp->actual = -1;
			}
			else
				temp->actual = 1;
		}
		temp->prev = temp->actual;
		omp_init_lock(&temp->lock);
		fields.push_back(*temp);
	}
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

void GameOfLife::Reverse(int i, int sum) {
	// living
	if (fields[i].prev) {
		if (sum >= 6)
			fields[i].actual = false;
	}
	// dead
	else {
		if (sum == 2)
			fields[i].actual = -1;
	}
}

void GameOfLife::Classic(int i, int sum) {
	// living
	if (fields[i].prev) {
		if (sum != 3 && sum != 2)
			fields[i].actual = false;
	}
	// dead
	else {
		if (sum == 3)
			fields[i].actual = 1;
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
			for (int i = 0; i < this->fields.size(); ++i) {
				fields[i].prev = fields[i].actual;
				fields[i].actual = 0;
			}

			#pragma omp parallel sections 
			{
				#pragma omp section
				{
					#pragma omp parallel for 
					for (int i = 0; i < this->fields.size(); ++i)
					{
						int x = i % this->width;
						int y = floor((double)i / (double)this->height);
						int sum = 0;
						for (int row = -horizon; row <= horizon; row++)
							for (int col = -horizon; col <= horizon; col++)
						{
								if (row == 0 && col == 0) {  continue; }
								int neiX = x + row;
								int neiY = y + col;
								if (neiX < 0)	neiX = width - 1;
								else if (neiX >= width)	neiX = 0;
								if (neiY < 0)	neiY = height - 1;
								else if (neiY >= height)	neiY = 0;
								if (fields[(neiX*width) + neiY].prev)
									sum++;
							}
						if (!fields[i].actual) {
							omp_set_lock(&fields[i].lock);
							Classic(i, sum);
							omp_unset_lock(&fields[i].lock);
						}
					}
				}
				#pragma omp section
				{
					#pragma omp parallel for 
					for (int i = this->fields.size()-1; i >= 0; --i)
					{
						int x = i % this->width;
						int y = floor((double)i / (double)this->height);
						int sum = 0;
						for (int row = -horizon; row <= horizon; row++)
							for (int col = -horizon; col <= horizon; col++)
							{
								if (row == 0 && col == 0) { continue; }
								int neiX = x + row;
								int neiY = y + col;
								if (neiX < 0)	neiX = width - 1;
								else if (neiX >= width)	neiX = 0;
								if (neiY < 0)	neiY = height - 1;
								else if (neiY >= height)	neiY = 0;
								if (fields[(neiX*width) + neiY].prev)
									sum++;
							}
						if (!fields[i].actual){
							omp_set_lock(&fields[i].lock);
							Reverse(i, sum);
							omp_unset_lock(&fields[i].lock);
						}
					}
				}
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

void GameOfLife::reserveMemoryForPixels(int pixelOnPixel)
{
	pixels = new uint8_t**[height*pixelOnPixel];
	//pixels= (uint8_t***)malloc(height*pixelOnPixel * sizeof(uint8_t**));
	for (int i = 0; i < height*pixelOnPixel; i++)
	{
		pixels[i] = new uint8_t*[width*pixelOnPixel];
		//pixels[i] = (uint8_t**)malloc(width*pixelOnPixel * sizeof(uint8_t*));
		for (int j = 0; j < width*pixelOnPixel; j++)
			pixels[i][j] = new uint8_t[4];
			//pixels[i][j]=(uint8_t*)malloc(4 * sizeof(uint8_t));
	}
}

void GameOfLife::freeMemoryForPixels(int pixelOnPixel)
{
	for (int i = 0; i < height*pixelOnPixel; i+=pixelOnPixel)
	{
		for (int j = 0; j < width*pixelOnPixel; j++)
			delete [] pixels[i][j];
		delete [] pixels[i];
	}
	delete [] pixels;
	pixels = NULL;
}

void GameOfLife::updatePixels(int pixelOnPixel)
{
	uint8_t bbb[4] = { 255,0,0,1 };
	uint8_t ccc[4] = { 0,0,255,1 };
	uint8_t white[4] = { 255,255,255,1 };

	int l;
	for (int i = 0; i < height; i++)
	{
		l = 0;
		for (int j = 0; j<width; j++)
		{
			if (fields[(width*i) + j].prev == 1)
				for (int k = 0; k < pixelOnPixel; k++)
				{
					for (int m = 0; m < 4; m++)
						pixels[(i*pixelOnPixel) + l][(j*pixelOnPixel) + k][m] = bbb[m];
				}
			else if (fields[(width*i) + j].prev == -1)
				for (int k = 0; k < pixelOnPixel; k++)
				{
					for (int m = 0; m < 4; m++)
						pixels[(i*pixelOnPixel) + l][(j*pixelOnPixel) + k][m] = ccc[m];
				}
			else
				for (int k = 0; k < pixelOnPixel; k++)
				{
					for (int m = 0; m < 4; m++)
						pixels[(i*pixelOnPixel) + l][(j*pixelOnPixel) + k][m] = white[m];
				}
		}
		for (l = 1; l < pixelOnPixel; l++)
			pixels[(i*pixelOnPixel) + l] = pixels[(i*pixelOnPixel)];
	}
}

void GameOfLife::makeGif(char *name, int timeInterval, int pixelOnPixel)
{
	cout << "Start making gif file\n";
	reserveMemoryForPixels(pixelOnPixel);
	GifBegin(&Gif, name, width*pixelOnPixel, height*pixelOnPixel, timeInterval, 8, 0);

	uint8_t tab[xxx][yyy][4];
	do
	{
		updatePixels(pixelOnPixel);
		for (int i = 0; i<width*pixelOnPixel; i++)
			for (int j = 0; j<height*pixelOnPixel; j++)
			{
				for (int m = 0; m < 4; m++)
					tab[i][j][m] = pixels[i][j][m];
			}

		GifWriteFrame(&Gif, **tab, width*pixelOnPixel,height*pixelOnPixel, timeInterval, 8, 0);
	} while (doStep());
	GifEnd(&Gif);
	freeMemoryForPixels(pixelOnPixel);
}

GameOfLife::~GameOfLife()
{
	for (int i = 0; i < this->fields.size(); ++i)
		omp_destroy_lock(&fields[i].lock);
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