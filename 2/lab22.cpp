#include "stdafx.h"
#include <omp.h>
#include <iostream>
#include  "Matrix.h"
#include <fstream>

#define WIDTH 4
#define HEIGHT 4

#define t 10

using namespace std;

int main(int argc,  char** argv)
{
	double start, stop, temp;
	std::stringstream results;
	
	// create 2 objects
	Matrix matrix1 = Matrix(HEIGHT,WIDTH);
	Matrix matrix2 = Matrix(HEIGHT,WIDTH);
	
	matrix1.randomizeCells();
	matrix2.randomizeCells();
	
	temp = 0.0000;
	
	for (int i=0; i<t;i++){
		cout<<"i";
		start = omp_get_wtime();
		(matrix1.multiplicate(matrix2));
	stop = omp_get_wtime();
	temp+=stop-start;
	}
	
	results << ",iterative\n" <<"1," << temp/t << endl;
	
	for (int i=2; i<=8; i++){
		omp_set_dynamic(0); 
		omp_set_num_threads(i);
		temp = 0.000;
	
	for (int i=0; i<t;i++){
			start = omp_get_wtime();
			(matrix1.multiplicate(matrix2));
			stop = omp_get_wtime();
			temp+=stop-start;
	}
		
		results << i <<"," << temp/t << endl;
	}
	
	// save results to file
	std::ofstream outFile;
	outFile.open("results.csv");
	outFile << results.rdbuf();
	
	outFile.flush();
	outFile.close();
	
	return 0;
}
