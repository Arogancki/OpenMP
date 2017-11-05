
#include <omp.h>
#include <iostream>
#include  "Matrix.h"
#include  "MatrixT.h"
#include <fstream>

#define WIDTH1 50
#define HEIGHT1 50

#define WIDTH2 250
#define HEIGHT2 250

#define WIDTH3 500
#define HEIGHT3 500

#define t 10

using namespace std;

int main(int argc,  char** argv)
{
	double start, stop, temp;
	std::stringstream results;
	
	// create 2 objects
	Matrix matrix1 = Matrix(HEIGHT1,WIDTH1);
	Matrix matrix2 = Matrix(HEIGHT1,WIDTH1);
	
	matrix1.randomizeCells();
	matrix2.randomizeCells();
	
	// create 2 objects
	MatrixT matrix21 = MatrixT(HEIGHT1,WIDTH1);
	MatrixT matrix22 = MatrixT(HEIGHT1,WIDTH1);
	
	matrix21.randomizeCells();
	matrix22.randomizeCells();
	
	// vectory normalnie
	temp = 0.0000;
	for (int i=0; i<t;i++){
		start = omp_get_wtime();
		(matrix1.multiplicate(matrix2));
	stop = omp_get_wtime();
	temp+=stop-start;
	}
	results << ",vector k-w\n" <<"1," << temp/t << endl;
	for (int i=2; i<=8; i++){
		omp_set_dynamic(0); 
		omp_set_num_threads(i);
		temp = 0.000;
		for (int k=0; k<t;k++){
				start = omp_get_wtime();
				(matrix1.multiplicate(matrix2));
				stop = omp_get_wtime();
				temp+=stop-start;
		}
		results << i <<"," << temp/t << endl;
	}
	results<<endl;
	
	// vectory na odwrot
	temp = 0.0000;
	for (int i=0; i<t;i++){
		start = omp_get_wtime();
		(matrix1.multiplicateDiff(matrix2));
	stop = omp_get_wtime();
	temp+=stop-start;
	}
	results << ",vector w-k\n" <<"1," << temp/t << endl;
	for (int i=2; i<=8; i++){
		omp_set_dynamic(0); 
		omp_set_num_threads(i);
		temp = 0.000;
		for (int k=0; k<t;k++){
				start = omp_get_wtime();
				(matrix1.multiplicateDiff(matrix2));
				stop = omp_get_wtime();
				temp+=stop-start;
		}
		results << i <<"," << temp/t << endl;
	}
	results<<endl;
	
	
	// pointery normalnie
	temp = 0.0000;
	for (int i=0; i<t;i++){
		start = omp_get_wtime();
		(matrix21.multiplicate(matrix22));
	stop = omp_get_wtime();
	temp+=stop-start;
	}
	results << ",pointer k-w\n" <<"1," << temp/t << endl;
	for (int i=2; i<=8; i++){
		omp_set_dynamic(0); 
		omp_set_num_threads(i);
		temp = 0.000;
		for (int k=0; k<t;k++){
				start = omp_get_wtime();
				(matrix21.multiplicate(matrix22));
				stop = omp_get_wtime();
				temp+=stop-start;
		}
		results << i <<"," << temp/t << endl;
	}
	results<<endl;
	
	// pointery na odwrot
	temp = 0.0000;
	for (int i=0; i<t;i++){
		start = omp_get_wtime();
		(matrix21.multiplicateDiff(matrix22));
	stop = omp_get_wtime();
	temp+=stop-start;
	}
	results << ",poin w-k\n" <<"1," << temp/t << endl;
	for (int i=2; i<=8; i++){
		omp_set_dynamic(0); 
		omp_set_num_threads(i);
		temp = 0.000;
		for (int k=0; k<t;k++){
				start = omp_get_wtime();
				(matrix21.multiplicateDiff(matrix22));
				stop = omp_get_wtime();
				temp+=stop-start;
		}
		results << i <<"," << temp/t << endl;
	}
	results<<endl;
	
	// save results to file
	std::ofstream outFile;
	outFile.open("results1.csv");
	outFile << results.rdbuf();
	
	outFile.flush();
	outFile.close();
	
	return 0;
}
