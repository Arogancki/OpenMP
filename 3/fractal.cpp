#include <stdio.h>
#include <math.h>
#include <iostream>
#include <thread>
#include <vector>

using namespace std;
static unsigned char colors[1600][1600][3] = { { { 0 } } };
const int iXmaxG = 1600;
const int iYmaxG = 1600;
const int MaxColorComponentValue = 255;
char *comment = "# ";
int ilosc_watkow = 16;

void draw()
{
	static unsigned char color[3];
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
	for (iY = 0; iY<iYmaxG; iY++)
	{
		Cy = CyMin + iY*PixelHeight;
		if (fabs(Cy)< PixelHeight / 2) Cy = 0.0; /* Main antenna */
		for (iX = 0; iX<iXmaxG; iX++)
		{
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
			};
			/* compute  pixel color (24 bit = 3 bytes) */
			if (Iteration == IterationMax)
			{ /*  interior of Mandelbrot set = black */
				color[0] = 0;
				color[1] = 0;
				color[2] = 0;
			}
			else
			{ /* exterior of Mandelbrot set = white */
				color[0] = 255; /* Red*/
				color[1] = 255;  /* Green */
				color[2] = 255;/* Blue */
			};
			/*write color to the file*/
			//fwrite(color,1,3,fp);
			colors[iY][iX][0] = color[0];
			colors[iY][iX][1] = color[1];
			colors[iY][iX][2] = color[2];
		}

	}
}

int main()
{
	FILE * fp;
	char *filename = "new1.ppm";
	fp = fopen(filename, "wb");

	fprintf(fp, "P6\n %s\n %d\n %d\n %d\n", comment, iXmaxG, iYmaxG, MaxColorComponentValue);

	draw();

	for (int i = 0; i<iYmaxG; i++)
		for (int j = 0; j<iXmaxG; j++)
			fwrite(colors[i][j], 1, 3, fp);

	fclose(fp);
	std::cout << "Wyeksportowano do pliku " << filename << ".\n";
}