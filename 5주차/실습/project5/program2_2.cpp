#include "my_solver.h"
#include <time.h>
#include <math.h>
#include <stdlib.h>
#define MY_RAND_MAX 32767

int rand(void);
void srand(unsigned int seed);
double get_integral(double u, double h, double x0, double* x, double* y);

double bisection(double* x, double* y, double h, double u, int n); 
static unsigned long int next = 2019;


void program2_2()
{
	FILE* fp_r, * fp_w;

	fp_r = fopen("pdf_table.txt", "r");
	fp_w = fopen("random_event_table.txt", "w");

	/***************************************************/

	double* pdf_x, * pdf_y,*random_x,*random_y, h;
	int n, nr;
	fscanf(fp_r, "%d %lf", &n, &h);
	pdf_x = (double*)malloc(sizeof(double) * n);
	pdf_y = (double*)malloc(sizeof(double) * n);

	for (int i = 0; i < n; i++) {
		fscanf(fp_r, "%lf %lf", &pdf_x[i], &pdf_y[i]);
	}



	printf("input nr : ");
	scanf("%d", &nr);
	random_x = (double*)malloc(sizeof(double) * nr);
	random_y = (double*)malloc(sizeof(double) * nr);
	for (int i = 0; i < nr; i++) {
		random_x[i] = (double)(rand() + 1) / (MY_RAND_MAX + 2);
		//printf("random x %lf\n", random_x[i]);
	}

	
	fprintf(fp_w, "%d\n", nr);
	for (int i = 0; i < nr; i++) {
		random_y[i]=bisection(pdf_x, pdf_y, h, random_x[i],n);
		fprintf(fp_w, "%.15lf\n", random_y[i]);
	}


	free(pdf_x);
	free(pdf_y);
	free(random_x);
	free(random_y);

	/***************************************************/

	if (fp_r != NULL) fclose(fp_r);
	if (fp_w != NULL) fclose(fp_w);
}

//Fx-U
double get_integral(double u, double h, double x0,double* x, double* y ) {//integral
	double result = 0.0, y_sum = 0.0;
	int i = 0;
	for (i = 0; x[i + 1] < x0; i++) {
		y_sum += y[i];
	}
	result = h / 2 * (y[0] + 2 * y_sum + y[i]);

	//³ª¸ÓÁö

	result+=(y[i] + (y[i + 1] - y[i]) / (x[i + 1] - x[i]) * (x0 - x[i]) / 2)* (x0 - x[i]);

	return result-u;
}


int rand(void) {
	next = next * 1103515245 + 12345;
	return (unsigned int)(next / 65536) % (MY_RAND_MAX + 1);
}
void srand(unsigned int seed)
{
	next = seed;
}

double bisection( double* x, double* y, double h,double u,int num)
{
	double a0=x[0], b0=x[num-1], x0, x1, temp;
	int n = 0;


	x0 = (a0 + b0) / 2.0;
	for (n = 1; n < Nmax; n++) {

		if (get_integral(u,h,x0,x,y) * get_integral(u,h,b0,x,y) < 0) {
			x1 = (x0 + b0) / 2.0;
			a0 = x0;
		}
		else {
			x1 = (x0 + a0) / 2.0;
			b0 = x0;
		}
		if (fabs(x1 - x0) < EPSILON || fabs(get_integral(u, h, x1, x, y)) < DELTA)
			break;
		x0 = x1;
	}
	//printf("Bisection Method: root= %.15e %d\n", x1,n);
	return x1;
}