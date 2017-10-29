#include "Matrix.h"
#include <iostream>
#include <sstream> 
#include <fstream>
#include <ctime>
#include <vector>
#include <cstdlib>

using namespace std;

static bool alreadyDidSrand = false;

Matrix::Matrix(int height = 0, int width = 0)
{
	this->width = width;
	this->height = height;
	vector < double > temp;
	for (int count = 0; count < height; count++)
		temp.push_back(0);
	for (int count = 0; count < width; count++)
		matrix.push_back(temp);
}

Matrix::~Matrix()
{
	matrix.clear();
}

void Matrix::setCell(int height, int width, double newValue)
{
	if (this->height>0 && height <= this->height && this->width>0 && width <= this->width)
		matrix[height - 1][width - 1] = newValue;
}

void Matrix::randomizeCells(){
	this->randomizeCells(0.01d, 1000.0d);
}

void Matrix::randomizeCells(double fMin, double fMax){
	for (int i=1; i<=height; i++)
		for (int j=1; j<=height; j++)
			 randomizeCell(i,j,fMin,fMax);
}

void Matrix::randomizeCell(int x, int y, double fMin, double fMax){
	setCell(x,y,fRand(fMin, fMax));
}

double Matrix::fRand(double fMin, double fMax)
{
	if (alreadyDidSrand){
		srand( time( NULL ) );
		alreadyDidSrand=true;
	}
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

void Matrix::addToAllCells(double value)
{
	for (int count = 0; count < height; count++)
		for (int count2 = 0; count2 < width; count2++)
			matrix[count][count2] += value;
}

string Matrix::getCell(int height, int width)
{
	stringstream value;
	value << matrix[height - 1][width - 1];
	return(value.str());
}

string Matrix::getRow(int height)
{
	stringstream value;
	for (int count = 0; count < width; count++)
		value << matrix[height - 1][count] << " ";
	return(value.str());
}

string Matrix::getColumn(int width)
{
	stringstream value;
	for (int count = 0; count < height; count++)
		value << matrix[width - 1][count] << " ";
	return(value.str());
}

string Matrix::getMatrix()
{
	stringstream value;
	for (int count = 0; count < height; count++)
		value << getRow(count + 1) << endl;
	return(value.str());
}

Matrix* Matrix::transpose()
{
	Matrix *newmatrix = new Matrix(width, height);
	for (int count = 0; count < width; count++)
		for (int count2 = 0; count2 < height; count2++)
			newmatrix->setCell(count + 1, count2 + 1, matrix[count2][count]);
	return(newmatrix);
}

Matrix* Matrix::add(Matrix second)
{
	Matrix *newmatrix = new Matrix(width, height);
	for (int count = 0; count < width; count++)
		for (int count2 = 0; count2 < height; count2++)
			newmatrix->setCell(count + 1, count2 + 1, matrix[count2][count] + second.matrix[count2][count]);
	return(newmatrix);
}

Matrix* Matrix::sub(Matrix second)
{
	Matrix *newmatrix = new Matrix(width, height);
	for (int count = 0; count < width; count++)
		for (int count2 = 0; count2 < height; count2++)
			newmatrix->setCell(count + 1, count2 + 1, matrix[count2][count] - second.matrix[count2][count]);
	return(newmatrix);
}

void Matrix::addToCell(int x, int y, double num){
	if (x>0 && y>0 && x<=this->height && y<=this->width)
		this->matrix[x-1][y-1] += num;
}

Matrix* Matrix::multiplicate(Matrix second)
{
	Matrix *newMatrix = new Matrix(height, second.width);
	areEqual(second);
	for (int count = 0; count < height; count++)
		for (int count2 = 0; count2 < second.width; count2++)
			for (int count3 = 0; count3 < width; count3++)
				addToCell(count + 1, count2 + 1, this->matrix[count][count3] * second.matrix[count3][count2]);
	return(newMatrix);
}

Matrix* Matrix::multiplicateParallel(Matrix second)
{
	Matrix *newMatrix = new Matrix(height, second.width);
	areEqual(second);
	#pragma omp parallel for collapse(3)
	for (int count = 0; count < height; count++)
		for (int count2 = 0; count2 < second.width; count2++)
			for (int count3 = 0; count3 < width; count3++)
				addToCell(count + 1, count2 + 1, this->matrix[count][count3] * second.matrix[count3][count2]);
	return(newMatrix);
}

Matrix* Matrix::Strassen(Matrix second)
{
	Matrix *newMatrix = new Matrix(height, second.width);
	areEqual(second);
	
}

Matrix* Matrix::StrassenParallel(Matrix second)
{
	Matrix *newMatrix = new Matrix(height, second.width);
	areEqual(second);
}


void Matrix::areEqual(Matrix second) throw(string)
{
	if (this->height != second.height || this->width != second.width)
		throw "Multiplication exception: Incorrect sizes.";
}