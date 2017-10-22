#include <omp.h>
#include <iostream>

using namespace std;

int main()
{
	#pragma omp parallel 
		cout << "Hello world"<<"\n";
		
	return 0;
}