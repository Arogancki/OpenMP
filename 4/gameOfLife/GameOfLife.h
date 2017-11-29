#pragma once
#include <vector>
#include <thread>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <stdlib.h>
#include <iostream>
#include <mutex>

namespace std{
	class mutex;
}

using std::cout;

#define xxx 400
#define yyy 400

#define density 25// of procent

#define fre 0
#define red 1
#define blue 2
#define green 3
#define black 4

#define horizon 1

struct field
{
	int actual;
	int prev;
};
struct GifWriter
{
	FILE* f;
	uint8_t* oldImage;
	bool firstFrame;
};
struct Threads
{
	std::thread *thread;
	int startx, starty;
};

class GameOfLife
{
public:
	GameOfLife(int width, int height, int maxCycles,int numberOfThreads);
	GameOfLife(int width, int height);
	~GameOfLife(); 
	void print();
	bool doStep(int number);
	bool doStep();
	void makeGif(char *name, int timeInterval, int pixelOnPixel);
	void reset();
private:
	//rulls
	void Classic(int, int);
	GifWriter Gif;
	uint8_t ***pixels;
	void realConstructor(int width, int height, int maxCycles, int numberOfThreads);
	void reserveMemoryForPixels(int pixelOnPixel);
	void freeMemoryForPixels(int pixelOnPixel);
	void updatePixels(int pixelOnPixel);
	std::vector<Threads> threads;
	std::vector<field> fields;
	int width, height,cycle,maxCycles;
	int numberOfThreads;
	void saveStates(int start, int end);
	void changeStates(int who);
	void reset2(int,int);
};