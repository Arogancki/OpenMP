#include <omp.h>
#include <iostream>
#include <math.h>

using namespace std;

#define HEIGHT 2500
#define WIDTH 2500

double** alloc(int sizeX, int sizeY) {
	double **p = new double*[sizeX];
	int i;
	for (i = 0; i<sizeX; i++) {
		p[i] = new double[sizeY];
	}
	return p;
}

void deAlloc(double** p, int sizeX) {
	int i;
	for (i = 0; i<sizeX; i++) {
		delete p[i];
	}
	delete[] p;
}

double doSth(int i, int j){
	return (sin(((double)i)*0.01)+cos(((double)j)/0.01) + 0.1)*0.14;
}

int main()
{
	double** p;
	int i = 0, j = 0;
	double start, stop;

	p = alloc(HEIGHT, WIDTH);
	
	// series
	start = omp_get_wtime();

	for (i = 0; i<HEIGHT; i++)
		for (j = 0; j<WIDTH; j++)
			p[i][j] = doSth(i, j);

	stop = omp_get_wtime();

	cout << "Time for series: " << stop - start<<endl;

	for(int k=2; k<9; k++){
	// parallel
	omp_set_dynamic(0); 
	omp_set_num_threads(k);
	start = omp_get_wtime();
	#pragma omp parallel for
		for (i = 0; i<HEIGHT; i++)
			for (j = 0; j<WIDTH; j++)
				p[i][j] = doSth(i, j);
	stop = omp_get_wtime();
	cout << "Time for parallel: "<<k<< " "<< stop - start<<endl;
}
	
	deAlloc(p, HEIGHT);
	return 0;
}