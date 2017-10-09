#include "Matrix.h"
#include <iostream>
#include <sstream> 
#include <fstream>
#include <ctime>
#include <vector>

using namespace std;

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

void Matrix::save()
{
	time_t t = time(0);
	struct tm now;
	localtime_s(&now, &t);
	stringstream location;
	location << height << "x" << width << "t=" << now.tm_hour << "-" << now.tm_min << "-" << now.tm_sec << ".txt";
	ifstream inputFile(location.str());
	if (inputFile.is_open())
	{
		cout << "File already exist,  saving as - \"x" << location.str() << "\"\n";
		stringstream nextLocation;
		nextLocation << "x" << location.str();
		location.str("");
		location << nextLocation.str();
	}
	ofstream outputFile(location.str());
	outputFile << height << ";" << width << endl;
	outputFile << getMatrix();
	cout << "Matrix saved " << height << "x" << width << " in file " << location.str() << endl;
}

Matrix::Matrix(string location)
{
	ifstream inputFile(location);
	if (!inputFile.is_open())
	{
		cout << "Error during reading: " << location << "!\n";
		return;
	}
	string data;
	getline(inputFile, data);
	stringstream HEIGHT, WIDTH;
	int count = 0;
	while (';' != data[count])
	{
		HEIGHT << data[count];
		count++;
	}
	count++;
	while (NULL != data[count])
	{
		WIDTH << data[count];
		count++;
	}
	// usuniecie dotychczasowej macierzy 
	matrix.clear();
	// ustawienie nowych width oraz height
	WIDTH >> width;
	HEIGHT >> height;
	// ustawienie nowej macierzy wypelnionej 0
	vector < double > temp;
	for (int count = 0; count < height; count++)
		temp.push_back(0);
	for (int count2 = 0; count2 < width; count2++)
		matrix.push_back(temp);
	// wrzucanie kolejnych wartosci do macierzy
	int w, h = 1;
	getline(inputFile, data);
	while (data[0] != NULL)
	{
		w = 1;
		count = 0;
		while (data[count] != NULL)
		{
			stringstream value;
			value.str("");
			while (data[count] != ' ')
			{
				value << data[count];
				count++;
			}
			double temp;
			value >> temp;
			setCell(h, w, temp);
			w++;
			count++;
		}
		h++;
		getline(inputFile, data);
	}
	cout << "File loaded: " << location << endl;
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

Matrix* Matrix::multiplicate(Matrix second)
{
	Matrix *newMatrix = new Matrix(width, height);
	areEqual(second);
	for (int count = 0; count < width; count++)
		for (int count2 = 0; count2 < width; count2++)
		{
			int suma = 0;
			for (int count3 = 0; count < width; count3++)
				suma += this->matrix[count + 1][count3 + 1] * second.matrix[count3 + 1][count + 1];
			newMatrix->setCell(count + 1, count2 + 1, suma);
		}
	return(newMatrix);
}


void Matrix::areEqual(Matrix second) throw(string)
{
	if (this->height != second.height || this->width != second.width)
		throw "Multiplication exception: Incorrect sizes.";
}