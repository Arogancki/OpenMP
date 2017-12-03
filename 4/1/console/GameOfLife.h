#include <vector>
#include <iostream>
#include <sstream>
using std::cout;

// to dzielimy na 4
// 40 400 2000
#define xxx 2000
#define yyy xxx

//ilocs iteracji gry
// 10 200 1000
#define TTT 1000

// 1 4 6 8
#define THRE 8


#define density 40// of procent

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

class GameOfLife
{
public:
	GameOfLife(int width, int height, int maxCycles,int numberOfThreads);
	GameOfLife(int width, int height);
	~GameOfLife(); 
	void print();
	bool doStep(int number);
	bool doStep();
	void ch();
	void makeGif(int timeInterval, int pixelOnPixel);
	void reset();
	void checkThreads();
	double wholetime;
private:
	//rulls
	void Classic(int, int);
	void realConstructor(int width, int height, int maxCycles, int numberOfThreads);
	std::vector<field> fields;
	int width, height,cycle,maxCycles;
	int numberOfThreads;
	void saveStates(int start, int end);
	void changeStates(int who);
	void reset2(int,int);
};