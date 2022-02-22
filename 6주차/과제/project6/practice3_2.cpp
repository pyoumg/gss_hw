#include "my_solver.h"

#define SOLNUMS 4
#define MATCOLS SOLNUMS
#define MATROWS SOLNUMS
#define TOLERANCE 0.0000001

void fcn3_2(int* n, double* x, double* fvec, int* iflag)
{
	/********************************/
	//x:0 y:1 z:2 w:3
	fvec[0] = x[0] + 10 * x[1] + 9;
	fvec[1] = pow(5, 0.5) * (x[2] - x[3]) - 2 * pow(5, 0.5);
	fvec[2] = pow(x[1] - 2 * x[2], 2) - 9;
	fvec[3] = pow(10, 0.5) * pow(x[0] - x[3], 2) - 2 * pow(10, 0.5);

	/********************************/
}

void practice3_2(void)
{
	int n = SOLNUMS;
	double x[SOLNUMS] = { 0.9,-0.9,1.25,-1.25 };	//need to initilize x0
	double fvec[SOLNUMS];
	double tol = TOLERANCE;
	int info;
	double wa[(SOLNUMS * (3 * SOLNUMS + 13)) / 2];
	int lwa = (SOLNUMS * (3 * SOLNUMS + 13)) / 2;

	FILE* fp_w = fopen("roots_found_3-2.txt", "w");
	if (fp_w == NULL) {
		printf("%s file open error...\n", "roots_found_3-2.txt");
		return;
	}

	/********************************/


	int temp = 1;//iflag
	hybrd1_(fcn3_2, &n, x, fvec, &tol, &info, wa, &lwa);

	fprintf(fp_w, "info: %d \nx: %lf y: %lf z: %lf w: %lf\n", info, x[0], x[1], x[2], x[3]);
	fprintf(fp_w, "***f(x)***\n");
	fcn3_2(&n, x, fvec, &temp);
	fprintf(fp_w, "%lf %lf %lf %lf\n", fvec[0], fvec[1], fvec[2], fvec[3]);



	/********************************/

	fclose(fp_w);
}