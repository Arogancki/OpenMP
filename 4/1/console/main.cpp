
#include "GameOfLife.h"

#define Nframe 40
int timeInterval = (1000.0 / Nframe);

int main()
{
	int THREADS = THRE;
	
	double s;
	s=0.0;
	int r;
	for (r=0; r<10; r++){
		GameOfLife sample(xxx/4, yyy/4, TTT, THREADS);
		sample.makeGif(timeInterval, 4); 	
		s += sample.wholetime;
	}
	
	double temp;
	temp =10.0;
	s = s / temp;
	
	std::stringstream strs;
	strs << "del /F /Q  w\\ | echo \"\" >>  w/" << s;

	system(strs.str().c_str());
	
	cout << "done: " << s <<std::endl;
	return 0;
}