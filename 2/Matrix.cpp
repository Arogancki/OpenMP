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

void Matrix::areEqual(Matrix second) throw(string)
{
	if (this->height != second.height || this->width != second.width)
		throw "Multiplication exception: Incorrect sizes.";
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
				newMatrix->addToCell(count + 1, count2 + 1, this->matrix[count][count3] * second.matrix[count3][count2]);
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
				newMatrix->addToCell(count + 1, count2 + 1, this->matrix[count][count3] * second.matrix[count3][count2]);
	return(newMatrix);
}

//function for strassem
void StrassenMatrix_Sub(int n, Matrix *X, Matrix *Y, Matrix *Z) {
     for(int i=0; i<n; i++) {
        for(int j=0; j<n; j++) {
			Z->setCell(i+1,j+1, X->matrix[i][j] - Y->matrix[i][j]);   
        }        
     }     
}

void StrassenMatrix_Add(int n, Matrix *X, Matrix *Y, Matrix *Z) {
     for(int i=0; i<n; i++) {
        for(int j=0; j<n; j++) {
			Z->setCell(i+1,j+1, X->matrix[i][j] + Y->matrix[i][j]);   
        }        
     }     
}

void StrassenMatrix_Multiply(Matrix *X, Matrix *Y, Matrix *Z) {
     for(int i=0; i<2; i++) {
        for(int j=0; j<2; j++) { 
           for(int t=0; t<2; t++) {
			   Z->addToCell(i+1, j+1, X->matrix[i][t] * Y->matrix[t][j]);
           }  
        }        
     }
}

void StrassenMatrix_SubParallel(int n, Matrix *X, Matrix *Y, Matrix *Z) {
	#pragma omp parallel for collapse(2)
     for(int i=0; i<n; i++) {
        for(int j=0; j<n; j++) {
			Z->setCell(i+1,j+1, X->matrix[i][j] - Y->matrix[i][j]);   
        }        
     }     
}

void StrassenMatrix_AddParallel(int n, Matrix *X, Matrix *Y, Matrix *Z) {
	#pragma omp parallel for collapse(2)
     for(int i=0; i<n; i++) {
        for(int j=0; j<n; j++) {
			Z->setCell(i+1,j+1, X->matrix[i][j] + Y->matrix[i][j]);   
        }        
     }     
}

void StrassenMatrix_MultiplyParallel(Matrix *X, Matrix *Y, Matrix *Z) {
	#pragma omp parallel for collapse(3)
     for(int i=0; i<2; i++) {
        for(int j=0; j<2; j++) { 
           for(int t=0; t<2; t++) {
			   Z->addToCell(i+1, j+1, X->matrix[i][t] * Y->matrix[t][j]);
           }  
        }        
     }
}

void StrassenX(int n, Matrix *A, Matrix *B, Matrix *C) {
	Matrix *A11 = new Matrix(A->height, A->width);
	Matrix *A12 = new Matrix(A->height, A->width);
	Matrix *A21 = new Matrix(A->height, A->width);
	Matrix *A22 = new Matrix(A->height, A->width);
	
	Matrix *B11 = new Matrix(A->height, A->width);
	Matrix *B12 =new Matrix(A->height, A->width);
	Matrix *B21 = new Matrix(A->height, A->width);
	Matrix *B22 = new Matrix(A->height, A->width);
	
	Matrix *C11 = new Matrix(A->height, A->width);
	Matrix *C12 = new Matrix(A->height, A->width);
	Matrix *C21 = new Matrix(A->height, A->width);
	Matrix *C22 = new Matrix(A->height, A->width);
	
	Matrix *M1 = new Matrix(A->height, A->width);
	Matrix *M2 = new Matrix(A->height, A->width);
	Matrix *M3 = new Matrix(A->height, A->width);
	Matrix *M4 = new Matrix(A->height, A->width);
	Matrix *M5 = new Matrix(A->height, A->width);
	Matrix *M6 = new Matrix(A->height, A->width);
	Matrix *M7 = new Matrix(A->height, A->width);
	
	Matrix *AA = new Matrix(A->height, A->width);
	Matrix *BB = new Matrix(A->height, A->width);
	
	if (n == 2){
		StrassenMatrix_Multiply(A, B, C);
	} else {
		for(int i=0; i<n/2; i++) {
           for(int j=0; j<n/2; j++) {
			  A11->setCell(i+1,j+1, A->matrix[i][j]);
			  A12->setCell(i+1,j+1, A->matrix[i][j+n/2]);
			  A21->setCell(i+1,j+1, A->matrix[i+n/2][j]);
			  A22->setCell(i+1,j+1, A->matrix[i+n/2][j+n/2]);
			  
			  B11->setCell(i+1,j+1, B->matrix[i][j]);
			  B12->setCell(i+1,j+1, B->matrix[i][j+n/2]);
			  B21->setCell(i+1,j+1, B->matrix[i+n/2][j]);
			  B22->setCell(i+1,j+1, B->matrix[i+n/2][j+n/2]); 
           }        
        }
		
		//Calculate M1 = (A0 + A3) × (B0 + B3)
        StrassenMatrix_Add(n/2, A11, A22, AA);
        StrassenMatrix_Add(n/2, B11, B22, BB);
        StrassenX(n/2, AA, BB, M1);
		
		//Calculate M2 = (A2 + A3) × B0
        StrassenMatrix_Add(n/2, A21, A22, AA);
        StrassenX(n/2, AA, B11, M2);
        
        //Calculate M3 = A0 × (B1 - B3)
        StrassenMatrix_Sub(n/2, B12, B22, BB);
        StrassenX(n/2, A11, BB, M3);
        
        //Calculate M4 = A3 × (B2 - B0)
        StrassenMatrix_Sub(n/2, B21, B11, BB);
        StrassenX(n/2, A22, BB, M4);
        
        //Calculate M5 = (A0 + A1) × B3
        StrassenMatrix_Add(n/2, A11, A12, AA);
        StrassenX(n/2, AA, B22, M5);
        
        //Calculate M6 = (A2 - A0) × (B0 + B1)
        StrassenMatrix_Sub(n/2, A21, A11, AA);
        StrassenMatrix_Add(n/2, B11, B12, BB);
        StrassenX(n/2, AA, BB, M6);
        
        //Calculate M7 = (A1 - A3) × (B2 + B3)
        StrassenMatrix_Sub(n/2, A12, A22, AA);
        StrassenMatrix_Add(n/2, B21, B22, BB);
        StrassenX(n/2, AA, BB, M7);
        
        //Calculate C0 = M1 + M4 - M5 + M7
        StrassenMatrix_Add(n/2, M1, M4, AA);
        StrassenMatrix_Sub(n/2, M7, M5, BB);
        StrassenMatrix_Add(n/2, AA, BB, C11);
        
        //Calculate C1 = M3 + M5
        StrassenMatrix_Add(n/2, M3, M5, C12);
        
        //Calculate C2 = M2 + M4
        StrassenMatrix_Add(n/2, M2, M4, C21);
        
        //Calculate C3 = M1 - M2 + M3 + M6
        StrassenMatrix_Sub(n/2, M1, M2, AA);
        StrassenMatrix_Add(n/2, M3, M6, BB);
        StrassenMatrix_Add(n/2, AA, BB, C22);
        
        //Set the result to C[][N]
        for(int i=0; i<n/2; i++) {
           for(int j=0; j<n/2; j++) {
			  C->setCell(i+1,j+1, C11->matrix[i][j]);
			  C->setCell(i+1,(j+1)+n/2, C12->matrix[i][j]);
			  C->setCell((i+1)+n/2,(j+1), C21->matrix[i][j]);
			  C->setCell((i+1)+n/2,(j+1)+n/2, C22->matrix[i][j]);  
           }        
        }
		
	}
}

void StrassenXParallel(int n, Matrix *A, Matrix *B, Matrix *C) {
	Matrix *A11 = new Matrix(A->height, A->width);
	Matrix *A12 = new Matrix(A->height, A->width);
	Matrix *A21 = new Matrix(A->height, A->width);
	Matrix *A22 = new Matrix(A->height, A->width);
	
	Matrix *B11 = new Matrix(A->height, A->width);
	Matrix *B12 =new Matrix(A->height, A->width);
	Matrix *B21 = new Matrix(A->height, A->width);
	Matrix *B22 = new Matrix(A->height, A->width);
	
	Matrix *C11 = new Matrix(A->height, A->width);
	Matrix *C12 = new Matrix(A->height, A->width);
	Matrix *C21 = new Matrix(A->height, A->width);
	Matrix *C22 = new Matrix(A->height, A->width);
	
	Matrix *M1 = new Matrix(A->height, A->width);
	Matrix *M2 = new Matrix(A->height, A->width);
	Matrix *M3 = new Matrix(A->height, A->width);
	Matrix *M4 = new Matrix(A->height, A->width);
	Matrix *M5 = new Matrix(A->height, A->width);
	Matrix *M6 = new Matrix(A->height, A->width);
	Matrix *M7 = new Matrix(A->height, A->width);
	
	Matrix *AA = new Matrix(A->height, A->width);
	Matrix *BB = new Matrix(A->height, A->width);
	
	if (n == 2){
		StrassenMatrix_MultiplyParallel(A, B, C);
	} else {
		for(int i=0; i<n/2; i++) {
           for(int j=0; j<n/2; j++) {
			  A11->setCell(i+1,j+1, A->matrix[i][j]);
			  A12->setCell(i+1,j+1, A->matrix[i][j+n/2]);
			  A21->setCell(i+1,j+1, A->matrix[i+n/2][j]);
			  A22->setCell(i+1,j+1, A->matrix[i+n/2][j+n/2]);
			  
			  B11->setCell(i+1,j+1, B->matrix[i][j]);
			  B12->setCell(i+1,j+1, B->matrix[i][j+n/2]);
			  B21->setCell(i+1,j+1, B->matrix[i+n/2][j]);
			  B22->setCell(i+1,j+1, B->matrix[i+n/2][j+n/2]); 
           }        
        }
		
		//Calculate M1 = (A0 + A3) × (B0 + B3)
        StrassenMatrix_AddParallel(n/2, A11, A22, AA);
        StrassenMatrix_AddParallel(n/2, B11, B22, BB);
        StrassenXParallel(n/2, AA, BB, M1);
		
		//Calculate M2 = (A2 + A3) × B0
        StrassenMatrix_AddParallel(n/2, A21, A22, AA);
        StrassenXParallel(n/2, AA, B11, M2);
        
        //Calculate M3 = A0 × (B1 - B3)
        StrassenMatrix_SubParallel(n/2, B12, B22, BB);
        StrassenXParallel(n/2, A11, BB, M3);
        
        //Calculate M4 = A3 × (B2 - B0)
        StrassenMatrix_SubParallel(n/2, B21, B11, BB);
        StrassenXParallel(n/2, A22, BB, M4);
        
        //Calculate M5 = (A0 + A1) × B3
        StrassenMatrix_AddParallel(n/2, A11, A12, AA);
        StrassenXParallel(n/2, AA, B22, M5);
        
        //Calculate M6 = (A2 - A0) × (B0 + B1)
        StrassenMatrix_SubParallel(n/2, A21, A11, AA);
        StrassenMatrix_AddParallel(n/2, B11, B12, BB);
        StrassenXParallel(n/2, AA, BB, M6);
        
        //Calculate M7 = (A1 - A3) × (B2 + B3)
        StrassenMatrix_SubParallel(n/2, A12, A22, AA);
        StrassenMatrix_AddParallel(n/2, B21, B22, BB);
        StrassenXParallel(n/2, AA, BB, M7);
        
        //Calculate C0 = M1 + M4 - M5 + M7
        StrassenMatrix_AddParallel(n/2, M1, M4, AA);
        StrassenMatrix_SubParallel(n/2, M7, M5, BB);
        StrassenMatrix_AddParallel(n/2, AA, BB, C11);
        
        //Calculate C1 = M3 + M5
        StrassenMatrix_AddParallel(n/2, M3, M5, C12);
        
        //Calculate C2 = M2 + M4
        StrassenMatrix_AddParallel(n/2, M2, M4, C21);
        
        //Calculate C3 = M1 - M2 + M3 + M6
        StrassenMatrix_SubParallel(n/2, M1, M2, AA);
        StrassenMatrix_AddParallel(n/2, M3, M6, BB);
        StrassenMatrix_AddParallel(n/2, AA, BB, C22);
        
        //Set the result to C[][N]
        for(int i=0; i<n/2; i++) {
           for(int j=0; j<n/2; j++) {
			  C->setCell(i+1,j+1, C11->matrix[i][j]);
			  C->setCell(i+1,(j+1)+n/2, C12->matrix[i][j]);
			  C->setCell((i+1)+n/2,(j+1), C21->matrix[i][j]);
			  C->setCell((i+1)+n/2,(j+1)+n/2, C22->matrix[i][j]);  
           }        
        }
		
	}
}

Matrix* Matrix::Strassen(Matrix second)
{
	Matrix *newMatrix = new Matrix(height, second.width);
	StrassenX(newMatrix->height, this, &second, newMatrix);
	return(newMatrix);
}

Matrix* Matrix::StrassenParallel(Matrix second)
{
	Matrix *newMatrix = new Matrix(height, second.width);
	StrassenXParallel(newMatrix->height, this, &second, newMatrix);
	return(newMatrix);
}