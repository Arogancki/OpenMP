
#include <stdio.h>
#include <vector>
#include <iostream>
#include <sstream> 

using namespace std;

class MatrixT
{
public:
	MatrixT(int, int);
	~MatrixT();
	void setCell(int, int, double);
	void addToAllCells(double); // ustawienie podanej komorki
	void randomizeCells();
	void addToCell(int,int,double);
	double fRand(double fMin, double fMax);
	void randomizeCell(int,int,double,double);
	void randomizeCells(double,double);
	void randomizeCells(int,int,double,double);
	double getMatrixT(int, int);
	string getCell(int, int);
	string getRow(int);
	string getColumn(int);
	string getMatrixT();
	MatrixT* transpose(); // transpnowanie
	MatrixT* add(MatrixT); // dodawanie macierzy
	MatrixT* sub(MatrixT);// odejmowanie macierzy
	MatrixT* multiplicate(MatrixT); // mnozenie macierzy
	MatrixT* multiplicateParallel(MatrixT); // mnozenie macierzy
	MatrixT* multiplicateDiff(MatrixT); // mnozenie macierzy
	MatrixT* multiplicateParallelDiff(MatrixT); // mnozenie macierzy
	MatrixT* Strassen(MatrixT);
	MatrixT* StrassenParallel(MatrixT);
	int width, height;
	double **matrix;
	void areEqual(MatrixT) throw(string);
};