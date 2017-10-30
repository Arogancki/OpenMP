#include <omp.h>
#include <iostream>
#include  "Matrix.h"
#include <fstream>

#define WIDTH 10
#define HEIGHT 10

using namespace std;

int main(int argc,  char** argv)
{
	double start,stop;
	std::stringstream results;
	
	// create 2 objects
	Matrix matrix1 = Matrix(HEIGHT,WIDTH);
	Matrix matrix2 = Matrix(HEIGHT,WIDTH);
	
	matrix1.randomizeCells();
	matrix2.randomizeCells();
	
	start = omp_get_wtime();
	(matrix1.multiplicate(matrix2))->~Matrix();
	stop = omp_get_wtime();
	cout << "Time for series(normaln) : " << stop - start <<endl;
	results << "1,N," << stop - start << endl;
	
	start = omp_get_wtime();
	(matrix1.Strassen(matrix2))->~Matrix();
	stop = omp_get_wtime();
	cout << "Time for series(strassen): " << stop - start<<endl;
	results << "1,S," << stop - start << endl;
	
	for (int i=2; i<=8; i++){
		omp_set_dynamic(0); 
		omp_set_num_threads(i);
		start = omp_get_wtime();
		(matrix1.multiplicateParallel(matrix2))->~Matrix();
		stop = omp_get_wtime();
		cout << "Time for " << i << " threads: (normal)" << stop - start<<endl;
		results << i << ",N," << stop - start << endl;
		
		start = omp_get_wtime();
		(matrix1.StrassenParallel(matrix2))->~Matrix();
		stop = omp_get_wtime();
		cout << "Time for " << i << " threads: (strassen)" << stop - start<<endl;
		results << i << ",S," << stop - start << endl;
	}
	
	// save results to file
	std::ofstream outFile;
	outFile.open("results.csv");
	outFile << results.rdbuf();
	
	return 0;
}
