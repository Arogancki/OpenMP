
#include <stdio.h>
#include <tchar.h>
#include <vector>
#include <iostream>
#include <sstream> 
#include <list>
#include <deque>
#include <array>

using namespace std;

class Matrix
{
public:
	Matrix(int, int);
	~Matrix();
	void setCell(int, int, double);
	void addToAllCells(double); // ustawienie podanej komorki
	void randomizeCells();
	void addToCell(int,int,double);
	double fRand(double fMin, double fMax);
	void randomizeCell(int,int,double,double);
	void randomizeCells(double,double);
	void randomizeCells(int,int,double,double);
	string getCell(int, int);
	string getRow(int);
	string getColumn(int);
	string getMatrix();
	Matrix* transpose(); // transpnowanie
	Matrix* add(Matrix); // dodawanie macierzy
	Matrix* sub(Matrix);// odejmowanie macierzy
	Matrix* multiplicate(Matrix); // mnozenie macierzy
	Matrix* multiplicateParallel(Matrix); // mnozenie macierzy
	Matrix* Strassen(Matrix);
	Matrix* StrassenParallel(Matrix);
	int width, height;
	double** matrix;
	void areEqual(Matrix) throw(string);
};