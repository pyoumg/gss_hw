#include "my_solver.h"
#include <time.h>
#include <math.h>
#include <stdlib.h>
#define MY_RAND_MAX 32767

static unsigned long int next = 2019;


double bisection2(double* x, double* y, double h, double u, int num)
{
	double a0 = x[0], b0 = x[num - 1], x0, x1;
	int n = 0;


	x0 = (a0 + b0) / 2.0;
	for (n = 1; n < Nmax; n++) {

		if (get_integral(u, h, x0, x, y) * get_integral(u, h, b0, x, y) < 0) {
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
	return x1;
}
double secant2(double* x, double* y, double h, double u, int num) {

	int n = 0;
	double  temp;
	double a0 = x[0], b0 = x[num - 1], x0, x1;

	x0 = (a0 + b0) / 2.0;
	//bisection
	for (n = 0; n < 2; n++) {
		if (get_integral(u, h, x0, x, y) * get_integral(u, h, b0, x, y) < 0) {
			x1 = (x0 + b0) / 2.0;
			a0 = x0;
		}
		else {
			x1 = (x0 + a0) / 2.0;
			b0 = x0;
		}
		if (fabs(x1 - x0) < EPSILON || fabs(get_integral(u, h, x1, x, y)) < DELTA)
			return x1;
		x0 = x1;
	}

	x0 = a0, x1 = b0;
	double fx0 = get_integral(u, h, x0, x, y);
	double fx1 = get_integral(u, h, x1, x, y);
	for (n; n < Nmax; n++) {
		temp = x1 - fx1 * (x1 - x0) / (fx1 - fx0);
		if (fabs(x1 - x0) < EPSILON || fabs(fx1) < DELTA)
			break;
		x0 = x1;
		x1 = temp;
		fx0 = get_integral(u, h, x0, x, y);
		fx1 = get_integral(u, h, x1, x, y);
	}
	return x1;

}

// HOMEWORK
void program2_3()
{
	FILE* fp_pdf, * fp_rand, * fp_his;
	int n, n_rand, idx, his[100] = { 0, };
	double h, * pdf_x, * pdf_y, * rand_x;
	double max = 0.0;
	fp_pdf = fopen("pdf_table.txt", "r");
	fp_rand = fopen("random_event_table.txt", "r");
	fp_his = fopen("histogram.txt", "w");


	//read pdf

	fscanf(fp_pdf, "%d %lf", &n, &h);
	pdf_x = (double*)malloc(sizeof(double) * n);
	pdf_y = (double*)malloc(sizeof(double) * n);
	for (int i = 0; i < n; i++) {
		fscanf(fp_pdf, "%lf %lf", &pdf_x[i], &pdf_y[i]);
		if (pdf_y[i] > max)
			max = pdf_y[i];
	}

	fprintf(fp_his, "pdf x y\n");
	for (int i = 0; i < n; i++) {
		fprintf(fp_his, "%lf %lf\n", pdf_x[i], pdf_y[i]);
	}


	//read rand

	fscanf(fp_rand, "%d", &n_rand);
	rand_x = (double*)malloc(sizeof(double) * n_rand);

	for (int i = 0; i < n_rand; i++) {
		fscanf(fp_rand, "%lf", &rand_x[i]);
	}
	for (int i = 0; i < n_rand; i++) {
		idx = (int)(rand_x[i] * 100.0);
		his[idx]++;
	}
	fprintf(fp_his, "histogram\n");
	for (int i = 0; i < 100; i++) {
		fprintf(fp_his, "[%.2lf,%.2lf) %d\n", (double)i / 100, (double)(i + 1) / 100, his[i]);
	}



	free(pdf_x);
	free(pdf_y);
	free(rand_x);
	fclose(fp_pdf);
	fclose(fp_rand);
	fclose(fp_his);

}
void program2_3_test(char* rand,char* histo)
{
	FILE* fp_pdf, * fp_rand, * fp_his;
	int n, n_rand, idx, his[100] = { 0, };
	double h, * pdf_x, * pdf_y, * rand_x;
	double max = 0.0;
	fp_pdf = fopen("pdf_table.txt", "r");
	fp_rand = fopen(rand, "r");
	fp_his = fopen(histo, "w");


	//read pdf

	fscanf(fp_pdf, "%d %lf", &n, &h);
	pdf_x = (double*)malloc(sizeof(double) * n);
	pdf_y = (double*)malloc(sizeof(double) * n);
	for (int i = 0; i < n; i++) {
		fscanf(fp_pdf, "%lf %lf", &pdf_x[i], &pdf_y[i]);
		if (pdf_y[i] > max)
			max = pdf_y[i];
	}

	fprintf(fp_his, "pdf x y\n");
	for (int i = 0; i < n; i++) {
		fprintf(fp_his, "%lf %lf\n", pdf_x[i], pdf_y[i]);
	}


	//read rand

	fscanf(fp_rand, "%d", &n_rand);
	rand_x = (double*)malloc(sizeof(double) * n_rand);

	for (int i = 0; i < n_rand; i++) {
		fscanf(fp_rand, "%lf", &rand_x[i]);
	}
	for (int i = 0; i < n_rand; i++) {
		idx = (int)(rand_x[i] * 100.0);
		his[idx]++;
	}
	fprintf(fp_his, "histogram\n");
	for (int i = 0; i < 100; i++) {
		fprintf(fp_his, "[%.2lf,%.2lf) %d\n", (double)i / 100, (double)(i + 1) / 100, his[i]);
	}



	free(pdf_x);
	free(pdf_y);
	free(rand_x);
	fclose(fp_pdf);
	fclose(fp_rand);
	fclose(fp_his);

}
// HOMEWORK
void program2_2_a()
{
	__int64 start, freq, end;
	float resultTime = 0;



	// something to do...
	FILE* fp_r, * fp_w;

	fp_r = fopen("pdf_table.txt", "r");
	fp_w = fopen("random_event_table_bisection.txt", "w");

	/***************************************************/

	double* pdf_x, * pdf_y, * random_x, * random_y, h;
	int n, nr;
	fscanf(fp_r, "%d %lf", &n, &h);
	pdf_x = (double*)malloc(sizeof(double) * n);
	pdf_y = (double*)malloc(sizeof(double) * n);

	for (int i = 0; i < n; i++) {
		fscanf(fp_r, "%lf %lf", &pdf_x[i], &pdf_y[i]);
	}
	CHECK_TIME_START;
	nr = 1000;//시간 측정을 위해 값 할당 

	random_x = (double*)malloc(sizeof(double) * nr);
	random_y = (double*)malloc(sizeof(double) * nr);
	for (int i = 0; i < nr; i++) {
		random_x[i] = (double)(rand() + 1) / (MY_RAND_MAX + 2);
	}


	for (int i = 0; i < nr; i++) {
		random_y[i] = bisection2(pdf_x, pdf_y, h, random_x[i], n);
	}

	CHECK_TIME_END(resultTime);
	fprintf(fp_w, "%d\n", nr);
	for (int i = 0; i < nr; i++) {
		fprintf(fp_w, "%.15lf\n", random_y[i]);
	}
	free(pdf_x);
	free(pdf_y);
	free(random_x);
	free(random_y);

	/***************************************************/

	if (fp_r != NULL) fclose(fp_r);
	if (fp_w != NULL) fclose(fp_w);



	printf("The program2_2_a run time is %f(ms)..\n", resultTime * 1000.0);
}

void program2_2_b()
{
	__int64 start, freq, end;
	float resultTime = 0;



	// something to do...
	FILE* fp_r, * fp_w;

	fp_r = fopen("pdf_table.txt", "r");
	fp_w = fopen("random_event_table_secant.txt", "w");

	/***************************************************/

	double* pdf_x, * pdf_y, * random_x, * random_y, h;
	int n, nr;
	fscanf(fp_r, "%d %lf", &n, &h);
	pdf_x = (double*)malloc(sizeof(double) * n);
	pdf_y = (double*)malloc(sizeof(double) * n);

	for (int i = 0; i < n; i++) {
		fscanf(fp_r, "%lf %lf", &pdf_x[i], &pdf_y[i]);
	}
	CHECK_TIME_START;
	nr = 1000;//시간 측정을 위해 값 할당 

	random_x = (double*)malloc(sizeof(double) * nr);
	random_y = (double*)malloc(sizeof(double) * nr);
	for (int i = 0; i < nr; i++) {
		random_x[i] = (double)(rand() + 1) / (MY_RAND_MAX + 2);
	}



	for (int i = 0; i < nr; i++) {
		random_y[i] = secant2(pdf_x, pdf_y, h, random_x[i], n);
	}
	CHECK_TIME_END(resultTime);

	fprintf(fp_w, "%d\n", nr);
	for (int i = 0; i < nr; i++) {
		fprintf(fp_w, "%.15lf\n", random_y[i]);
	}
	free(pdf_x);
	free(pdf_y);
	free(random_x);
	free(random_y);

	/***************************************************/

	if (fp_r != NULL) fclose(fp_r);
	if (fp_w != NULL) fclose(fp_w);



	printf("The program2_2_b run time is %f(ms)..\n", resultTime * 1000.0);
}

