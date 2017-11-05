
#include <stdio.h>
#include <vector>
#include <iostream>
#include <sstream> 

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
	double getMatrix(int, int);
	string getCell(int, int);
	string getRow(int);
	string getColumn(int);
	string getMatrix();
	Matrix* transpose(); // transpnowanie
	Matrix* add(Matrix); // dodawanie macierzy
	Matrix* sub(Matrix);// odejmowanie macierzy
	Matrix* multiplicate(Matrix); // mnozenie macierzy
	Matrix* multiplicateParallel(Matrix); // mnozenie macierzy
	Matrix* multiplicateDiff(Matrix); // mnozenie macierzy
	Matrix* multiplicateParallelDiff(Matrix); // mnozenie macierzy
	Matrix* Strassen(Matrix);
	Matrix* StrassenParallel(Matrix);
	int width, height;
	vector < vector < double > > matrix;
	void areEqual(Matrix) throw(string);
};