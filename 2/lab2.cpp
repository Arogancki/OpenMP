#include <omp.h>
#include <iostream>
#include  "Matrix.h"
#include <fstream>

#define WIDTH 250
#define HEIGHT 250

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
	cout<<"1."<<endl;
	start = omp_get_wtime();
	(matrix1.multiplicate(matrix2))->~Matrix();
	stop = omp_get_wtime();
	
	temp = stop - start;
	cout<<"2."<<endl;
	start = omp_get_wtime();
	(matrix1.Strassen(matrix2))->~Matrix();
	stop = omp_get_wtime();
	results << ",iterative,strassen\n" <<"1," << temp << "," << stop - start << endl;
	
	for (int i=2; i<=8; i++){
		cout<<"1."<<i<<endl;
		
		omp_set_dynamic(0); 
		omp_set_num_threads(i);
		start = omp_get_wtime();
		(matrix1.multiplicateParallel(matrix2))->~Matrix();
		stop = omp_get_wtime();
		temp = stop - start;
		cout<<"2."<<i<<endl;
		start = omp_get_wtime();
		(matrix1.StrassenParallel(matrix2))->~Matrix();
		stop = omp_get_wtime();
		results << i << ","<< temp << "," << stop - start << endl;
	}
	
	// save results to file
	std::ofstream outFile;
	outFile.open("results.csv");
	outFile << results.rdbuf();
	
	outFile.flush();
	outFile.close();
	
	return 0;
}
