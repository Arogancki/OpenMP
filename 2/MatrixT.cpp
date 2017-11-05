#include <omp.h>
#include "MatrixT.h"
#include <iostream>
#include <sstream> 
#include <fstream>
#include <ctime>
#include <vector>
#include <cstdlib>


using namespace std;

static bool alreadyDidSrand = false;

MatrixT::MatrixT(int height = 0, int width = 0)
{
	this->width = width;
	this->height = height;
	matrix = new double*[height];
	for (int i=0; i<height; i++){
		matrix[i] = new double[width];
		for (int j=0; j<width; j++){
			matrix[i][j] = 0;
		}
	}
}

MatrixT::~MatrixT()
{
	
}

void MatrixT::setCell(int height, int width, double newValue)
{
	if (this->height>0 && height <= this->height && this->width>0 && width <= this->width)
		matrix[height - 1][width - 1] = newValue;
}

void MatrixT::randomizeCells(){
	this->randomizeCells(0.01, 1000.0);
}

void MatrixT::randomizeCells(double fMin, double fMax){
	for (int i=1; i<=height; i++)
		for (int j=1; j<=height; j++)
			 randomizeCell(i,j,fMin,fMax);
}

void MatrixT::randomizeCell(int x, int y, double fMin, double fMax){
	setCell(x,y,fRand(fMin, fMax));
}

double MatrixT::fRand(double fMin, double fMax)
{
	if (alreadyDidSrand){
		srand( time( NULL ) );
		alreadyDidSrand=true;
	}
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

void MatrixT::addToAllCells(double value)
{
	for (int count = 0; count < height; count++)
		for (int count2 = 0; count2 < width; count2++)
			matrix[count][count2] += value;
}

string MatrixT::getCell(int height, int width)
{
	stringstream value;
	value << matrix[height - 1][width - 1];
	return(value.str());
}

string MatrixT::getRow(int height)
{
	stringstream value;
	for (int count = 0; count < width; count++)
		value << matrix[height - 1][count] << " ";
	return(value.str());
}

string MatrixT::getColumn(int width)
{
	stringstream value;
	for (int count = 0; count < height; count++)
		value << matrix[width - 1][count] << " ";
	return(value.str());
}

string MatrixT::getMatrixT()
{
	stringstream value;
	for (int count = 0; count < height; count++)
		value << getRow(count + 1) << endl;
	return(value.str());
}

void MatrixT::areEqual(MatrixT second) throw(string)
{
	if (this->height != second.height || this->width != second.width)
		throw "Multiplication exception: Incorrect sizes.";
}

MatrixT* MatrixT::transpose()
{
	MatrixT *newmatrix = new MatrixT(width, height);
	for (int count = 0; count < width; count++)
		for (int count2 = 0; count2 < height; count2++)
			newmatrix->setCell(count + 1, count2 + 1, matrix[count2][count]);
	return(newmatrix);
}

MatrixT* MatrixT::add(MatrixT second)
{
	MatrixT *newmatrix = new MatrixT(width, height);
	for (int count = 0; count < width; count++)
		for (int count2 = 0; count2 < height; count2++)
			newmatrix->setCell(count + 1, count2 + 1, matrix[count2][count] + second.matrix[count2][count]);
	return(newmatrix);
}

MatrixT* MatrixT::sub(MatrixT second)
{
	MatrixT *newmatrix = new MatrixT(width, height);
	for (int count = 0; count < width; count++)
		for (int count2 = 0; count2 < height; count2++)
			newmatrix->setCell(count2 + 1, count + 1, matrix[count][count2] - second.matrix[count][count2]);
	return(newmatrix);
}

void MatrixT::addToCell(int x, int y, double num){
	if (x>0 && y>0 && x<=this->height && y<=this->width)
		this->matrix[x-1][y-1] += num;
}

MatrixT* MatrixT::multiplicate(MatrixT second)
{
	MatrixT *newMatrixT = new MatrixT(height, width);
	
	//normal workflow
	for (int count = 0; count < height; count++)
		for (int count2 = 0; count2 < second.width; count2++)
			for (int count3 = 0; count3 < width; count3++){
				newMatrixT->addToCell(count + 1, count2 + 1, this->getMatrixT(count,count3) * second.getMatrixT(count3,count2));
			}
	return(newMatrixT);
}

MatrixT* MatrixT::multiplicateDiff(MatrixT second)
{
	MatrixT *newMatrixT = new MatrixT(height, width);
	
	//normal workflow
	for (int count = 0; count < height; count++)
		for (int count2 = 0; count2 < second.width; count2++)
			for (int count3 = 0; count3 < width; count3++)
				newMatrixT->matrix[count2 + 1][count + 1] += this->matrix[count2][count3] * second.matrix[count3][count];
	return(newMatrixT);
}

double MatrixT::getMatrixT(int x, int y){
	try {
		return matrix[x][y];
	}
	catch (char *e) {
		return 0;
	}
}

MatrixT* MatrixT::multiplicateParallel(MatrixT second)
{
	MatrixT *newMatrixT = new MatrixT(height, second.width);
	
	//normal workflow
	#pragma omp parallel for collapse(3)
	for (int count = 0; count < height; count++)
		for (int count2 = 0; count2 < second.width; count2++)
			for (int count3 = 0; count3 < width; count3++)
				newMatrixT->matrix[count + 1][count2 + 1] += this->matrix[count][count3] * second.matrix[count3][count2];
	return(newMatrixT);
}

MatrixT* MatrixT::multiplicateParallelDiff(MatrixT second)
{
	MatrixT *newMatrixT = new MatrixT(height, second.width);
	
	//normal workflow
	#pragma omp parallel for collapse(3)
	for (int count = 0; count < height; count++)
		for (int count2 = 0; count2 < second.width; count2++)
			for (int count3 = 0; count3 < width; count3++)
				newMatrixT->matrix[count2 + 1][count + 1] += this->matrix[count2][count3] * second.matrix[count3][count];
	return(newMatrixT);
}

//function for strassem
void StrassenMatrixT_Sub(int n, MatrixT *X, MatrixT *Y, MatrixT *Z) {
     for(int i=0; i<n; i++) {
        for(int j=0; j<n; j++) {
			Z->setCell(i+1,j+1, X->matrix[i][j] - Y->matrix[i][j]);   
        }        
     }     
}

void StrassenMatrixT_Add(int n, MatrixT *X, MatrixT *Y, MatrixT *Z) {
     for(int i=0; i<n; i++) {
        for(int j=0; j<n; j++) {
			Z->setCell(i+1,j+1, X->matrix[i][j] + Y->matrix[i][j]);   
        }        
     }     
}

void StrassenMatrixT_Multiply(MatrixT *X, MatrixT *Y, MatrixT *Z) {
     for(int i=0; i<2; i++) {
        for(int j=0; j<2; j++) { 
           for(int t=0; t<2; t++) {
			   if (t == 0){
					Z->setCell(i+1, j+1, X->matrix[i][t] * Y->matrix[t][j]);
			   }
			   else
				    Z->addToCell(i+1, j+1, X->matrix[i][t] * Y->matrix[t][j]);
           }  
        }        
     }
}

void StrassenMatrixT_SubParallel(int n, MatrixT *X, MatrixT *Y, MatrixT *Z) {
	#pragma omp parallel for collapse(2)
     for(int i=0; i<n; i++) {
        for(int j=0; j<n; j++) {
			Z->setCell(i+1,j+1, X->matrix[i][j] - Y->matrix[i][j]);   
        }        
     }     
}

void StrassenMatrixT_AddParallel(int n, MatrixT *X, MatrixT *Y, MatrixT *Z) {
	#pragma omp parallel for collapse(2)
     for(int i=0; i<n; i++) {
        for(int j=0; j<n; j++) {
			Z->setCell(i+1,j+1, X->matrix[i][j] + Y->matrix[i][j]);   
        }        
     }     
}

void StrassenMatrixT_MultiplyParallel(MatrixT *X, MatrixT *Y, MatrixT *Z) {
	#pragma omp parallel for collapse(3)
     for(int i=0; i<2; i++) {
        for(int j=0; j<2; j++) { 
           for(int t=0; t<2; t++) {
			   if (t == 0){
					Z->setCell(i+1, j+1, X->matrix[i][t] * Y->matrix[t][j]);
			   }
			   else
				    Z->addToCell(i+1, j+1, X->matrix[i][t] * Y->matrix[t][j]);
           }  
        }        
     }
}

void StrassenX(int n, MatrixT *A, MatrixT *B, MatrixT *C) {
	if (n <= 2){
		StrassenMatrixT_Multiply(A, B, C);
	} else {
		
		MatrixT *A11 = new MatrixT(A->height, A->width);
		MatrixT *A12 = new MatrixT(A->height, A->width);
		MatrixT *A21 = new MatrixT(A->height, A->width);
		MatrixT *A22 = new MatrixT(A->height, A->width);
		
		MatrixT *B11 = new MatrixT(A->height, A->width);
		MatrixT *B12 =new MatrixT(A->height, A->width);
		MatrixT *B21 = new MatrixT(A->height, A->width);
		MatrixT *B22 = new MatrixT(A->height, A->width);
		
		MatrixT *C11 = new MatrixT(A->height, A->width);
		MatrixT *C12 = new MatrixT(A->height, A->width);
		MatrixT *C21 = new MatrixT(A->height, A->width);
		MatrixT *C22 = new MatrixT(A->height, A->width);
		
		MatrixT *M1 = new MatrixT(A->height, A->width);
		MatrixT *M2 = new MatrixT(A->height, A->width);
		MatrixT *M3 = new MatrixT(A->height, A->width);
		MatrixT *M4 = new MatrixT(A->height, A->width);
		MatrixT *M5 = new MatrixT(A->height, A->width);
		MatrixT *M6 = new MatrixT(A->height, A->width);
		MatrixT *M7 = new MatrixT(A->height, A->width);
		
		MatrixT *AA = new MatrixT(A->height, A->width);
		MatrixT *BB = new MatrixT(A->height, A->width);
		
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
        StrassenMatrixT_Add(n/2, A11, A22, AA);
        StrassenMatrixT_Add(n/2, B11, B22, BB);
        StrassenX(n/2, AA, BB, M1);
		
		//Calculate M2 = (A2 + A3) × B0
        StrassenMatrixT_Add(n/2, A21, A22, AA);
        StrassenX(n/2, AA, B11, M2);
        
        //Calculate M3 = A0 × (B1 - B3)
        StrassenMatrixT_Sub(n/2, B12, B22, BB);
        StrassenX(n/2, A11, BB, M3);
        
        //Calculate M4 = A3 × (B2 - B0)
        StrassenMatrixT_Sub(n/2, B21, B11, BB);
        StrassenX(n/2, A22, BB, M4);
        
        //Calculate M5 = (A0 + A1) × B3
        StrassenMatrixT_Add(n/2, A11, A12, AA);
        StrassenX(n/2, AA, B22, M5);
        
        //Calculate M6 = (A2 - A0) × (B0 + B1)
        StrassenMatrixT_Sub(n/2, A21, A11, AA);
        StrassenMatrixT_Add(n/2, B11, B12, BB);
        StrassenX(n/2, AA, BB, M6);
        
        //Calculate M7 = (A1 - A3) × (B2 + B3)
        StrassenMatrixT_Sub(n/2, A12, A22, AA);
        StrassenMatrixT_Add(n/2, B21, B22, BB);
        StrassenX(n/2, AA, BB, M7);
        
        //Calculate C0 = M1 + M4 - M5 + M7
        StrassenMatrixT_Add(n/2, M1, M4, AA);
        StrassenMatrixT_Sub(n/2, M7, M5, BB);
        StrassenMatrixT_Add(n/2, AA, BB, C11);
        
        //Calculate C1 = M3 + M5
        StrassenMatrixT_Add(n/2, M3, M5, C12);
        
        //Calculate C2 = M2 + M4
        StrassenMatrixT_Add(n/2, M2, M4, C21);
        
        //Calculate C3 = M1 - M2 + M3 + M6
        StrassenMatrixT_Sub(n/2, M1, M2, AA);
        StrassenMatrixT_Add(n/2, M3, M6, BB);
        StrassenMatrixT_Add(n/2, AA, BB, C22);
        
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

void StrassenXParallel(int n, MatrixT *A, MatrixT *B, MatrixT *C) {
		
	if (n <= 2){
		StrassenMatrixT_MultiplyParallel(A, B, C);
	} else {
		
		MatrixT *A11 = new MatrixT(A->height, A->width);
		MatrixT *A12 = new MatrixT(A->height, A->width);
		MatrixT *A21 = new MatrixT(A->height, A->width);
		MatrixT *A22 = new MatrixT(A->height, A->width);
		
		MatrixT *B11 = new MatrixT(A->height, A->width);
		MatrixT *B12 =new MatrixT(A->height, A->width);
		MatrixT *B21 = new MatrixT(A->height, A->width);
		MatrixT *B22 = new MatrixT(A->height, A->width);
		
		MatrixT *C11 = new MatrixT(A->height, A->width);
		MatrixT *C12 = new MatrixT(A->height, A->width);
		MatrixT *C21 = new MatrixT(A->height, A->width);
		MatrixT *C22 = new MatrixT(A->height, A->width);
		
		MatrixT *M1 = new MatrixT(A->height, A->width);
		MatrixT *M2 = new MatrixT(A->height, A->width);
		MatrixT *M3 = new MatrixT(A->height, A->width);
		MatrixT *M4 = new MatrixT(A->height, A->width);
		MatrixT *M5 = new MatrixT(A->height, A->width);
		MatrixT *M6 = new MatrixT(A->height, A->width);
		MatrixT *M7 = new MatrixT(A->height, A->width);
		
		MatrixT *AA = new MatrixT(A->height, A->width);
		MatrixT *BB = new MatrixT(A->height, A->width);
		
		#pragma omp parallel for collapse(2)
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
        StrassenMatrixT_AddParallel(n/2, A11, A22, AA);
        StrassenMatrixT_AddParallel(n/2, B11, B22, BB);
        StrassenXParallel(n/2, AA, BB, M1);
		
		//Calculate M2 = (A2 + A3) × B0
        StrassenMatrixT_AddParallel(n/2, A21, A22, AA);
        StrassenXParallel(n/2, AA, B11, M2);
        
        //Calculate M3 = A0 × (B1 - B3)
        StrassenMatrixT_SubParallel(n/2, B12, B22, BB);
        StrassenXParallel(n/2, A11, BB, M3);
        
        //Calculate M4 = A3 × (B2 - B0)
        StrassenMatrixT_SubParallel(n/2, B21, B11, BB);
        StrassenXParallel(n/2, A22, BB, M4);
        
        //Calculate M5 = (A0 + A1) × B3
        StrassenMatrixT_AddParallel(n/2, A11, A12, AA);
        StrassenXParallel(n/2, AA, B22, M5);
        
        //Calculate M6 = (A2 - A0) × (B0 + B1)
        StrassenMatrixT_SubParallel(n/2, A21, A11, AA);
        StrassenMatrixT_AddParallel(n/2, B11, B12, BB);
        StrassenXParallel(n/2, AA, BB, M6);
        
        //Calculate M7 = (A1 - A3) × (B2 + B3)
        StrassenMatrixT_SubParallel(n/2, A12, A22, AA);
        StrassenMatrixT_AddParallel(n/2, B21, B22, BB);
        StrassenXParallel(n/2, AA, BB, M7);
        
        //Calculate C0 = M1 + M4 - M5 + M7
        StrassenMatrixT_AddParallel(n/2, M1, M4, AA);
        StrassenMatrixT_SubParallel(n/2, M7, M5, BB);
        StrassenMatrixT_AddParallel(n/2, AA, BB, C11);
        
        //Calculate C1 = M3 + M5
        StrassenMatrixT_AddParallel(n/2, M3, M5, C12);
        
        //Calculate C2 = M2 + M4
        StrassenMatrixT_AddParallel(n/2, M2, M4, C21);
        
        //Calculate C3 = M1 - M2 + M3 + M6
        StrassenMatrixT_SubParallel(n/2, M1, M2, AA);
        StrassenMatrixT_AddParallel(n/2, M3, M6, BB);
        StrassenMatrixT_AddParallel(n/2, AA, BB, C22);
        
        //Set the result to C[][N]
		#pragma omp parallel for collapse(2)
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

MatrixT* MatrixT::Strassen(MatrixT second)
{
	MatrixT *newMatrixT = new MatrixT(height, second.width);
	StrassenX(newMatrixT->height, this, &second, newMatrixT);
	return(newMatrixT);
}

MatrixT* MatrixT::StrassenParallel(MatrixT second)
{
	MatrixT *newMatrixT = new MatrixT(height, second.width);
	StrassenXParallel(newMatrixT->height, this, &second, newMatrixT);
	return(newMatrixT);
}