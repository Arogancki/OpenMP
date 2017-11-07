#include <stdio.h>
#include <math.h>
#include <iostream>
#include <omp.h>

#define THREADS 8

static unsigned char colors[1600][1600][3] = { { { 0 } } };
const int iXmaxG = 1600;
const int iYmaxG = 1600;

int threadColor[16][3] = {
	{255, 0, 0},
	{0, 255, 0},
	{0, 0, 255},
	{128, 128, 0},
	{0, 128, 128},
	{128, 0, 128,},
	{128, 32, 32},
	{32, 128, 32},
	{32, 32, 128},
	{64, 64, 172},
	{64, 172, 64},
	{172, 64, 64},
	{32, 0, 0},
	{0, 32, 0},
	{0, 0, 32},
	{16, 232, 16}
};

int main()
{
	// zmienne globalne
	FILE * fp;
	char *filename = "new.ppm";
	fp = fopen(filename, "wb");

	fprintf(fp, "P6\n %s\n %d\n %d\n %d\n", "# ", iXmaxG, iYmaxG, 255);

	// zmienne algorytmu
	int iX, iY;
	double Cx, Cy;
	const double CxMin = -2.5;
	const double CyMin = -2.0;
	double PixelWidth = (1.5 - CxMin) / iXmaxG;
	double PixelHeight = (2.0 - CyMin) / iYmaxG;
	double Zx, Zy, Zx2, Zy2; 
	int Iteration;
	const int IterationMax = 200;

	// 
	// default shared private
	// zmienne prywatne lub wspoldzielone
	// po wypisaniu pragmy domyslnie wszsyskie sa prywatne
	// mozemy zmienic na wspoldzielone dzieki-  default(shared)  (#pragma omp parallel for default(shared) private(i, j))
	// i zaprywacic kilka -  default(shared) private(i, j) (#pragma omp parallel for default(shared) private(i, j))
	// moge rowniez uzyc default(none) co zmusza mnie do sprecyzowania jaka jest kazda zmienna - dla czytelnosci i ni omyslnosci
	// default(none) shared(A, u, v, rows, columns) private(i, j)
	//
	//
	// schedule schedule(sposob_rozdzialu_iteracji, [chunk]) (#pragma omp for schedule(static, chunk_size)) (1 obowiazkowo 2 opcjonalnie)
	// okresla sposob rozdzielania iteracji
	// static - rozdzielone na zbiory o rozmiarach chunk - kiedy chunck nie okresloy dzieli sprawiedliwie
	// dynamic - watki biora chunk liczbe (lub 1) i po zrobieniu biora nastepna czesc chunck
	// guided - rozmiar iteracji zmienny w czasie - pozostale/watki ale jesli podam m to nie mniejszy niz m, ostatni moze mniejszy
	// runtime - opcja jest wybierana  w czasie pracy  ( np. setenv OMP_SCHEDULE "guided,4" )
	// auto - kompilator wybiera z tych 4
	//
	
	omp_set_dynamic(0); // Explicitly disable dynamic teams
	omp_set_num_threads(THREADS);
	
	#pragma omp parallel for 
	for (iY = 0; iY<iYmaxG; iY++)
	{
		int threadID = omp_get_thread_num();
		Cy = CyMin + iY*PixelHeight;
		if (fabs(Cy)< PixelHeight / 2) Cy = 0.0;
		for (iX = 0; iX<iXmaxG; iX++)
		{
			colors[iY][iX][0] = 32 + (192 * ( ((double)threadID)  / THREADS ) );
			colors[iY][iX][1] = 32 + (192 * ( ((double)threadID) / THREADS ) );
			colors[iY][iX][2] = 32 + (192 * ( ((double)threadID) / THREADS ) );
			
			Cx = CxMin + iX*PixelWidth;
			Zx = 0.0;
			Zy = 0.0;
			Zx2 = Zx*Zx;
			Zy2 = Zy*Zy;
			
			for (Iteration = 0; Iteration<IterationMax && ((Zx2 + Zy2)< ((double)4) ); Iteration++)
			{
				Zy = 2 * Zx*Zy + Cy;
				Zx = Zx2 - Zy2 + Cx;
				Zx2 = Zx*Zx;
				Zy2 = Zy*Zy;
			}
			
			if (Iteration == IterationMax)
			{
				colors[iY][iX][0] = threadColor[threadID % 16][0];
				colors[iY][iX][1]  = threadColor[threadID % 16][1];
				colors[iY][iX][2]  = threadColor[threadID % 16][2];
			}
		}
	}

	for (int i = 0; i<iYmaxG; i++)
		for (int j = 0; j<iXmaxG; j++)
			fwrite(colors[i][j], 1, 3, fp);
	fclose(fp);
	std::cout << "Wyeksportowano do pliku " << filename << ".\n";
}