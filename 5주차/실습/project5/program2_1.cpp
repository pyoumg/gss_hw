#include "my_solver.h"
#include <stdlib.h>
double get_integral(int x0, int xn,  double* y, double h) {
	double result;
	double y_sum = 0.0;
	for (int i = x0 + 1; i < xn; i++) {
		y_sum += y[i];
	}
	result = h/2 * (y[x0] + 2 * y_sum + y[xn]);
	return result;
}
void program2_1()
{
	FILE* fp_r, *fp_w;
	__int64 start, freq, end;
	float resultTime = 0;

	fp_r = fopen("sampling_table.txt", "r");
	if (fp_r == NULL) {
		printf("input file not found....\n");
		exit(0);
	}

	fp_w = fopen("pdf_table.txt", "w");

	/***************************************************/


	int n;
	double h;
	//pdf 생성 코드
	fscanf(fp_r, "%d %lf", &n, &h);
	double* x, * y;
	double* pdf_x, * pdf_y;
	double integral = 0.0;
	double y_sum = 0.0;
	x = (double*)malloc(sizeof(double) * n);
	y = (double*)malloc(sizeof(double) * n);
	pdf_x = (double*)malloc(sizeof(double) * n);
	pdf_y = (double*)malloc(sizeof(double) * n);

	for (int i = 0; i < n; i++) {
		fscanf(fp_r, "%lf %lf", &x[i], &y[i]);
	}


	//x정규화
	pdf_x[0] = 0.0;
	for (int i = 1; i < n; i++) {
		pdf_x[i] = (x[i]-x[0])/(x[n-1]-x[0]);
	}
	h = 1.0 / (n-1);
	//적분계산
	for (int i = 1; i < n - 1; i++) {
		y_sum += y[i];
	}

	integral = h / 2 * (y[0] + 2 * y_sum + y[n - 1]);
	
	//y 정규화

	for (int i = 0; i < n; i++) {
		pdf_y[i] = y[i] / integral;
	}
	printf("*** Integrating the pdf from 0.0 to 1.0 = %.6lf\n", get_integral(0, n - 1, pdf_y, h));
	printf("*** Integrating the pdf from 0.0 to 0.25 = %.6lf\n", get_integral(0, (int)(0.25 / h),pdf_y, h));
	printf("*** Integrating the pdf from 0.25 to 0.5 = %.6lf\n", get_integral((int)(0.25 / h), (int)(0.5 / h),  pdf_y, h));
	printf("*** Integrating the pdf from 0.5 to 0.75 = %.6lf\n", get_integral((int)(0.5 / h), (int)(0.75 / h),  pdf_y, h));
	printf("*** Integrating the pdf from 0.75 to 1.0 = %.6lf\n", get_integral((int)(0.75 / h), n - 1, pdf_y, h));


	fprintf(fp_w, "%d %.6lf\n", n, h);
	for (int i = 0; i < n; i++) {
		fprintf(fp_w, "%.6lf %.6lf\n", pdf_x[i], pdf_y[i]);
	}

	free(x);
	free(y);
	free(pdf_x);
	free(pdf_y);


	/***************************************************/

	if (fp_r != NULL) fclose(fp_r);
	if (fp_w != NULL) fclose(fp_w);
}
