#include "my_solver.h"

#define SOLNUMS 2
#define MATCOLS SOLNUMS
#define MATROWS SOLNUMS
#define TOLERANCE 0.0000001

void fcn3_3(int* n, double* x, double* fvec, double* fjac, int* ldfjac, int* iflag)
{
	// origin function F(x)
	if (*iflag == 1) {
		/********************************/
		//x:0 y:1
		fvec[0] = (sin(x[0] * x[1] + 6 / M_PI) + sqrt(x[0] * x[0] * x[1] * x[1] + 1)) / cos(x[0] - x[1]) + 2.8;
		fvec[1] = (x[0] * exp(x[0] * x[1] + 6 / M_PI) - sin(x[0] - x[1])) / sqrt(x[0] * x[0] * x[1] * x[1] + 1) - 1.66;


		/********************************/
	}
	// Jacobi matrix J(x)
	else if (*iflag == 2) {
		/********************************/

		fjac[0] = x[1] * cos(x[0] * x[1] + 6 / M_PI) + 2 * x[0] * x[1] * x[1] * pow(x[0] * x[0] * x[1] * x[1] + 1, -0.5) - 2.8 * sin(x[0] - x[1]);
		fjac[2] = x[0] * cos(x[0] * x[1] + 6 / M_PI) + 2 * x[1] * x[0] * x[0] * pow(x[0] * x[0] * x[1] * x[1] + 1, -0.5) + 2.8 * sin(x[0] - x[1]);
		fjac[1] = (x[0] * x[1] + 1) * exp(x[0] * x[1] + 6 / M_PI) - cos(x[0] - x[1]) - 1.66 * 2 * x[0] * x[1] * x[1] * pow(x[0] * x[0] * x[1] * x[1] + 1, -0.5);
		fjac[3] = x[0] * x[0] * exp(x[0] * x[1] + 6 / M_PI) + cos(x[0] - x[1]) - 1.66 * 2 * x[0] * x[0] * x[1] * pow(x[0] * x[0] * x[1] * x[1] + 1, -0.5);

		/********************************/
	}
}

void practice3_3(void) {
	int n = SOLNUMS;
	double x[SOLNUMS] = { 20.0,0.0 };	//need to initilize x0
	double fvec[SOLNUMS], fjac[MATCOLS * MATROWS];
	int ldfjac = SOLNUMS;
	double tol = TOLERANCE;
	int info;
	double wa[(SOLNUMS * (SOLNUMS + 13)) / 2];
	int lwa = (SOLNUMS * (SOLNUMS + 13)) / 2;

	FILE* fp_w = fopen("roots_found_3-3.txt", "w");
	if (fp_w == NULL) {
		printf("%s file open error...\n", "roots_found_3-3.txt");
		return;
	}

	/********************************/

	int temp = 1;//iflag

	//�ʱⰪ 1
	hybrj1_(fcn3_3, &n, x, fvec, fjac, &ldfjac, &tol, &info, wa, &lwa);

	fprintf(fp_w, "%lf %lf\n", x[0], x[1]);

	fprintf(fp_w, "***f(x)***\n");
	fcn3_3(&n, x, fvec, fjac, &ldfjac, &temp);
	fprintf(fp_w, "%lf %lf\n", fvec[0], fvec[1]);

	/********************************/

	fclose(fp_w);
}
