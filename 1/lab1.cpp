#include <omp.h>
#include <iostream>

using namespace std;

#define HEIGHT 10000
#define WIDTH 10000

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

int main()
{
	double** p;
	int i = 0, j = 0;
	double start, stop;

	// series
	p = alloc(HEIGHT, WIDTH);

	start = omp_get_wtime();

	for (i = 0; i<HEIGHT; i++)
		for (j = 0; j<WIDTH; j++)
			p[i][j] = (((double)i) * 100) / 0.01;

	stop = omp_get_wtime();

	deAlloc(p, HEIGHT);

	cout << "Time for series: " << stop - start<<endl;

	// parallel
	p = alloc(HEIGHT, WIDTH);

	start = omp_get_wtime();

	#pragma omp parallel for
		for (i = 0; i<HEIGHT; i++)
			for (j = 0; j<WIDTH; j++)
				p[i][j] = (((double)i) * 100) / 0.01;

	stop = omp_get_wtime();
	deAlloc(p, HEIGHT);
	cout << "Time for parallel: " << stop - start<<endl;
	return 0;
}