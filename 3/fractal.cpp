#include <stdio.h>
#include <math.h>
#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;
static unsigned char colors[1600][1600][3] = { { { 0 } } };
const int iXmaxG = 1600;
const int iYmaxG = 1600;
const int MaxColorComponentValue = 255;
char *comment = "# ";
int ilosc_watkow = 16;

int threadColor[8][3] = {
	{255, 0, 0},
	{0, 255, 0},
	{0, 0, 255},
	{128, 128, 0},
	{0, 128, 128},
	{128, 0, 128,},
	{32, 128, 32},
	{64, 0, 128}
};

void draw(int threads)
{
	/* screen ( integer) coordinate */
	int iX, iY;
	const int iXmax = iXmaxG;
	const int iYmax = iYmaxG;
	/* world ( double) coordinate = parameter plane*/
	double Cx, Cy;
	const double CxMin = -2.5;
	const double CxMax = 1.5;
	const double CyMin = -2.0;
	const double CyMax = 2.0;
	/* */
	double PixelWidth = (CxMax - CxMin) / iXmax;
	double PixelHeight = (CyMax - CyMin) / iYmax;
	/* color component ( R or G or B) is coded from 0 to 255 */
	/* it is 24 bit color RGB file */
	/* Z=Zx+Zy*i  ;   Z0 = 0 */
	double Zx, Zy;
	double Zx2, Zy2; /* Zx2=Zx*Zx;  Zy2=Zy*Zy  */
	int Iteration;
	const int IterationMax = 200;
	/* bail-out value , radius of circle ;  */
	const double EscapeRadius = 2;
	double ER2 = EscapeRadius*EscapeRadius;

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


	// najpier wypelnic kolorami szarosci zeby bylo tlo dobrze widoczne
	// potem podzielic kolowy miedzy watki - stowrzyc jakams tablice czy cos 
	
	omp_set_dynamic(0);     // Explicitly disable dynamic teams
	omp_set_num_threads(threads);
	
	#pragma omp parallel for 
	for (iY = 0; iY<iYmaxG; iY++)
	{
		int threadID = omp_get_thread_num();
	
		Cy = CyMin + iY*PixelHeight;
		if (fabs(Cy)< PixelHeight / 2) Cy = 0.0; /* Main antenna */
		for (iX = 0; iX<iXmaxG; iX++)
		{
			colors[iY][iX][0] = 32 + (192 * ( ((double)threadID)  / 8 ) );
			colors[iY][iX][1] = 32 + (192 * ( ((double)threadID) / 8 ) );
			colors[iY][iX][2] = 32 + (192 * ( ((double)threadID) / 8 ) );
			
			Cx = CxMin + iX*PixelWidth;
			/* initial value of orbit = critical point Z= 0 */
			Zx = 0.0;
			Zy = 0.0;
			Zx2 = Zx*Zx;
			Zy2 = Zy*Zy;
			/* */
			for (Iteration = 0; Iteration<IterationMax && ((Zx2 + Zy2)<ER2); Iteration++)
			{
				Zy = 2 * Zx*Zy + Cy;
				Zx = Zx2 - Zy2 + Cx;
				Zx2 = Zx*Zx;
				Zy2 = Zy*Zy;
			}
			/* compute  pixel color (24 bit = 3 bytes) */
			if (Iteration == IterationMax)
			{ /*  interior of Mandelbrot set = black */
				colors[iY][iX][0] = threadColor[threadID%8][0];
				colors[iY][iX][1]  = threadColor[threadID%8][1];
				colors[iY][iX][2]  = threadColor[threadID%8][2];
			}
		}
	}
}

int main()
{
	FILE * fp;
	char *filename = "new1.ppm";
	fp = fopen(filename, "wb");

	fprintf(fp, "P6\n %s\n %d\n %d\n %d\n", comment, iXmaxG, iYmaxG, MaxColorComponentValue);

	draw(4);

	for (int i = 0; i<iYmaxG; i++)
		for (int j = 0; j<iXmaxG; j++)
			fwrite(colors[i][j], 1, 3, fp);

	fclose(fp);
	std::cout << "Wyeksportowano do pliku " << filename << ".\n";
}