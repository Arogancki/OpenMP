#include <omp.h>
#include <iostream>
#include  "Matrix.h"

#define WIDTH 5
#define HEIGHT 5

using namespace std;

int main(int argc,  char** argv)
{
	// create 2 objects
	Matrix matrix1 = Matrix(HEIGHT,WIDTH);
	Matrix matrix2 = Matrix(HEIGHT,WIDTH);
	
	matrix1.randomizeCells();
	matrix2.randomizeCells();
	
	cout << "N:\n\n"<<((matrix1.multiplicate(matrix2))->getMatrix());
	cout << "S:\n\n"<<((matrix1.Strassen(matrix2))->getMatrix());
	
	return 0;
}
