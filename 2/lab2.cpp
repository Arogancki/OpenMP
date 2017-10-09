#include  "Matrix.h"
#include <omp.h>
#include <iostream>

using namespace std;

int main()
{
	// series
	start = omp_get_wtime();
	//do the job
	
	stop = omp_get_wtime();
	cout << "Time for series: " << stop - start<<endl;

	// parallel
	start = omp_get_wtime();
	//do the job

	stop = omp_get_wtime();
	cout << "Time for parallel: " << stop - start<<endl;
	return 0;
}