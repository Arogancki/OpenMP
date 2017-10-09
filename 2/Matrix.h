#include <stdio.h>
#include <tchar.h>
#include <vector>
#include <iostream>
#include <sstream> 

using namespace std;

class Matrix
{
public:
	Matrix(int, int);
	Matrix(string);
	~Matrix();
	void setCell(int, int, double);
	void addToAllCells(double); // ustawienie podanej komorki
	string getCell(int, int);
	string getRow(int);
	string getColumn(int);
	string getMatrix();
	Matrix* transpose(); // transpnowanie
	Matrix* add(Matrix); // dodawanie macierzy
	Matrix* sub(Matrix);// odejmowanie macierzy
	Matrix* multiplicate(Matrix); // mnozenie macierzy
	void save(); // nazwa- hight x width + czas
private:
	int width, height;
	vector < vector < double > >  matrix;
	void areEqual(Matrix) throw(string);
};