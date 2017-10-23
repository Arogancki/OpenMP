#include <omp.h>
#include <iostream>
#include  "Matrix.h"

#define WIDTH 250
#define HEIGHT 250

using namespace std;

int main(String x[])
{
	double start,stop;
	// create 2 objects
	Matrix matrix1 = Matrix(HEIGHT,WIDTH);
	Matrix matrix2 = Matrix(HEIGHT,WIDTH);
	
	matrix1.randomizeCells();
	matrix2.randomizeCells();
	
	// series
	start = omp_get_wtime();
	//do the job
	(matrix1.multiplicate(matrix2))->~Matrix();
	
	stop = omp_get_wtime();
	cout << "Time for series: " << stop - start<<endl;

	// parallel
	start = omp_get_wtime();
	//do the job
	(matrix1.multiplicateParallel(matrix2))->~Matrix();
	
	stop = omp_get_wtime();
	cout << "Time for parallel: " << stop - start<<endl;
	return 0;
}
