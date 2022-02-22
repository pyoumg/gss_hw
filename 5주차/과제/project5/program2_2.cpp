#include "my_solver.h"
#include <time.h>
#include <math.h>
#include <stdlib.h>
#define MY_RAND_MAX 32767

static unsigned long int next = 2019;


void program2_2()
{
	FILE* fp_r, * fp_w;

	fp_r = fopen("pdf_table.txt", "r");
	fp_w = fopen("random_event_table.txt", "w");

	/***************************************************/

	double* pdf_x, * pdf_y, * random_x, * random_y, h;
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
		random_y[i] = bisection(pdf_x, pdf_y, h, random_x[i], n);
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
double get_integral(double u, double h, double x0, double* x, double* y) {//integral
	double result = 0.0, y_sum = 0.0;
	int i = 0;
	for (i = 0; x[i + 1] < x0; i++) {
		y_sum += y[i];
	}
	result = h / 2 * (y[0] + 2 * y_sum + y[i]);

	//나머지

	result += (y[i] + (y[i + 1] - y[i]) / (x[i + 1] - x[i]) * (x0 - x[i]) / 2) * (x0 - x[i]);

	return result - u;
}


int rand(void) {
	next = next * 1103515245 + 12345;
	return (unsigned int)(next / 65536) % (MY_RAND_MAX + 1);
}
void srand(unsigned int seed)
{
	next = seed;
}

double bisection(double* x, double* y, double h, double u, int num)
{
	double a0 = x[0], b0 = x[num - 1], x0, x1, temp;
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
	//printf("Bisection Method: root= %.15e %d\n", x1,n);
	return x1;
}


double reverse_exp(double x, double a) {
	return -log(1.0 - x) / a;
}
void program2_1_a() {
	int n;
	double a, b, c;
	double* rand_x, * rand_a, * rand_b, * rand_c, avg = 0.0, d_avg = 0.0;
	FILE* fp_w = fopen("hw_2-1.txt", "w");
	FILE* fp_w2 = fopen("hw_2-1-x.txt", "w");
	printf("input n: ");
	scanf("%d", &n);

	rand_x = (double*)malloc(sizeof(double) * n);

	rand_a = (double*)malloc(sizeof(double) * n);
	rand_b = (double*)malloc(sizeof(double) * n);
	rand_c = (double*)malloc(sizeof(double) * n);

	a = 0.5;
	b = 1.0;
	c = 5.0;

	double x_avg = 0.0;
	//0과 1 사이 난수 생성
	for (int i = 0; i < n; i++) {
		rand_x[i] = (double)(rand() + 1) / (MY_RAND_MAX + 2);
		x_avg += rand_x[i];
	}
	x_avg /= n;
	//printf("%lf\n", x_avg);

	//a
	fprintf(fp_w, "λ= %.15lf\n", a);
	fprintf(fp_w2, "λ= %.15lf\n", a);
	for (int i = 0; i < n; i++) {
		rand_a[i] = reverse_exp(rand_x[i], a);
		avg += rand_a[i];
		d_avg += (rand_a[i] * rand_a[i]);
		fprintf(fp_w2, "%.15lf\n", rand_a[i]);
	}
	avg = avg / n;
	d_avg = d_avg / n;
	fprintf(fp_w, "average: %.15lf\n", avg);
	fprintf(fp_w, "variance: %.15lf\n", d_avg - avg * avg);

	//b
	avg = 0.0, d_avg = 0.0;
	fprintf(fp_w, "λ= %.15lf\n", b);
	fprintf(fp_w2, "λ= %.15lf\n", b);
	for (int i = 0; i < n; i++) {
		rand_b[i] = reverse_exp(rand_x[i], b);
		avg += rand_b[i];
		d_avg += (rand_b[i] * rand_b[i]);
		fprintf(fp_w2, "%.15lf\n", rand_b[i]);
	}
	avg = avg / n;
	d_avg = d_avg / n;
	fprintf(fp_w, "average: %.15lf\n", avg);
	fprintf(fp_w, "variance: %.15lf\n", d_avg - avg * avg);


	//c
	avg = 0.0, d_avg = 0.0;
	fprintf(fp_w, "λ= %.15lf\n", c);
	fprintf(fp_w2, "λ= %.15lf\n", c);
	for (int i = 0; i < n; i++) {
		rand_c[i] = reverse_exp(rand_x[i], c);
		avg += rand_c[i];
		d_avg += (rand_c[i] * rand_c[i]);
		fprintf(fp_w2, "%.15lf\n", rand_c[i]);
	}
	avg = avg / n;
	d_avg = d_avg / n;
	fprintf(fp_w, "average: %.15lf\n", avg);
	fprintf(fp_w, "variance: %.15lf\n", d_avg - avg * avg);


	free(rand_x);
	free(rand_a);
	free(rand_b);
	free(rand_c);
	fclose(fp_w);
	fclose(fp_w2);
}
