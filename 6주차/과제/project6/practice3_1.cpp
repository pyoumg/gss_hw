#include "my_solver.h"

#define SOLNUMS 4
#define MATCOLS SOLNUMS
#define MATROWS SOLNUMS
#define TOLERANCE 0.0000001
double p[SOLNUMS][SOLNUMS];
double t[SOLNUMS];
double tr[SOLNUMS];
double c;
void fcn3_1(int* n, double* x, double* fvec, double* fjac, int* ldfjac, int* iflag)
{
	// origin function F(x)
	if (*iflag == 1) {
		/********************************/

		fvec[0] = pow(x[0] - p[0][0], 2) + pow(x[1] - p[0][1], 2) + pow(x[2] - p[0][2], 2) - pow(c * (tr[0] + x[3] - t[0]), 2);
		fvec[1] = pow(x[0] - p[1][0], 2) + pow(x[1] - p[1][1], 2) + pow(x[2] - p[1][2], 2) - pow(c * (tr[1] + x[3] - t[1]), 2);
		fvec[2] = pow(x[0] - p[2][0], 2) + pow(x[1] - p[2][1], 2) + pow(x[2] - p[2][2], 2) - pow(c * (tr[2] + x[3] - t[2]), 2);
		fvec[3] = pow(x[0] - p[3][0], 2) + pow(x[1] - p[3][1], 2) + pow(x[2] - p[3][2], 2) - pow(c * (tr[3] + x[3] - t[3]), 2);

		/********************************/
	}
	// Jacobi matrix J(x)
	else if (*iflag == 2) {
		/********************************/


		fjac[0] = 2 * (x[0] - p[0][0]);     fjac[4] = 2 * (x[1] - p[0][1]);     fjac[8] = 2 * (x[2] - p[0][2]);		fjac[12] = -2 * c * c * (tr[0] + x[3] - t[0]);
		fjac[1] = 2 * (x[0] - p[1][0]);     fjac[5] = 2 * (x[1] - p[1][1]);     fjac[9] = 2 * (x[2] - p[1][2]);		fjac[13] = -2 * c * c * (tr[1] + x[3] - t[1]);
		fjac[2] = 2 * (x[0] - p[2][0]);     fjac[6] = 2 * (x[1] - p[2][1]);		fjac[10] = 2 * (x[2] - p[2][2]);	fjac[14] = -2 * c * c * (tr[2] + x[3] - t[2]);
		fjac[3] = 2 * (x[0] - p[3][0]);		fjac[7] = 2 * (x[1] - p[3][1]);		fjac[11] = 2 * (x[2] - p[3][2]);	fjac[15] = -2 * c * c * (tr[3] + x[3] - t[3]);


		/********************************/
	}
}
void fcn3_1_hybrid(int* n, double* x, double* fvec, int* iflag) {


	fvec[0] = pow(x[0] - p[0][0], 2) + pow(x[1] - p[0][1], 2) + pow(x[2] - p[0][2], 2) - pow(c * (tr[0] + x[3] - t[0]), 2);
	fvec[1] = pow(x[0] - p[1][0], 2) + pow(x[1] - p[1][1], 2) + pow(x[2] - p[1][2], 2) - pow(c * (tr[1] + x[3] - t[1]), 2);
	fvec[2] = pow(x[0] - p[2][0], 2) + pow(x[1] - p[2][1], 2) + pow(x[2] - p[2][2], 2) - pow(c * (tr[2] + x[3] - t[2]), 2);
	fvec[3] = pow(x[0] - p[3][0], 2) + pow(x[1] - p[3][1], 2) + pow(x[2] - p[3][2], 2) - pow(c * (tr[3] + x[3] - t[3]), 2);


}
void practice3_1(void) {
	int n = SOLNUMS;
	double x[SOLNUMS] = { 0.0, };	//need to initilize x0
	double fvec[SOLNUMS], fjac[MATCOLS * MATROWS];
	int ldfjac = SOLNUMS;
	double tol = TOLERANCE;
	int info;
	double wa[(SOLNUMS * (SOLNUMS + 13)) / 2];
	int lwa = (SOLNUMS * (SOLNUMS + 13)) / 2;
	double p_t[SOLNUMS][SOLNUMS];
	double b;
	int temp = 1;//iflag
	char str_r[40];
	char str_w[40];
	char str_w2[40];
	for (int i = 0; i < 3; i++) {

		sprintf(str_r, "GPS_signal_%d.txt", i);
		sprintf(str_w, "GPS_position_3-1_%d.txt", i);
		FILE* fp_r = fopen(str_r, "r");
		if (fp_r == NULL) {
			printf("%s file open error...\n", str_r);
			return;
		}
		FILE* fp_w = fopen(str_w, "w");
		if (fp_w == NULL) {
			printf("%s file open error...\n", str_w);
			return;
		}

		/********************************/

		fscanf(fp_r, "%lf %lf", &c, &b);
		for (int i = 0; i < SOLNUMS; i++) {
			for (int j = 0; j < SOLNUMS - 1; j++) {
				fscanf(fp_r, "%lf", &p[i][j]);

			}
			fscanf(fp_r, "%lf", &t[i]);
			fscanf(fp_r, "%lf", &tr[i]);

		}
		printf("input x : ");
		for (int i = 0; i < SOLNUMS; i++)
			scanf("%lf", &x[i]);
		x[3] = b;



		hybrj1_(fcn3_1, &n, x, fvec, fjac, &ldfjac, &tol, &info, wa, &lwa);

		fprintf(fp_w, "%lf %lf %lf %lf\n", x[0], x[1], x[2], x[3]);

		fprintf(fp_w, "***f(x)***\n");
		fcn3_1(&n, x, fvec, fjac, &ldfjac, &temp);

		fprintf(fp_w, "%lf %lf %lf %lf\n", fvec[0], fvec[1], fvec[2], fvec[3]);


		/********************************/

		fclose(fp_w);
		fclose(fp_r);
	}


	//hybrid
	double wa2[(SOLNUMS * (3 * SOLNUMS + 13)) / 2];
	int lwa2 = (SOLNUMS * (3 * SOLNUMS + 13)) / 2;


	for (int i = 0; i < 3; i++) {

		sprintf(str_r, "GPS_signal_%d.txt", i);
		sprintf(str_w2, "GPS_position_3-2_%d.txt", i);
		FILE* fp_r = fopen(str_r, "r");
		if (fp_r == NULL) {
			printf("%s file open error...\n", str_r);
			return;
		}
		FILE* fp_w = fopen(str_w2, "w");
		if (fp_w == NULL) {
			printf("%s file open error...\n", str_w2);
			return;
		}

		fscanf(fp_r, "%lf %lf", &c, &b);
		for (int i = 0; i < SOLNUMS; i++) {
			for (int j = 0; j < SOLNUMS - 1; j++) {
				fscanf(fp_r, "%lf", &p[i][j]);

			}
			fscanf(fp_r, "%lf", &t[i]);
			fscanf(fp_r, "%lf", &tr[i]);

		}
		printf("input x : ");
		for (int i = 0; i < SOLNUMS; i++)
			scanf("%lf", &x[i]);
		x[3] = b;

		hybrd1_(fcn3_1_hybrid, &n, x, fvec, &tol, &info, wa2, &lwa2);
		fprintf(fp_w, "info: %d x: %lf %lf %lf %lf\n", info, x[0], x[1], x[2], x[3]);
		fprintf(fp_w, "***f(x)***\n");
		fcn3_1_hybrid(&n, x, fvec, &temp);
		fprintf(fp_w, "%lf %lf %lf %lf\n", fvec[0], fvec[1], fvec[2], fvec[3]);


		fclose(fp_w);
		fclose(fp_r);
	}

}
