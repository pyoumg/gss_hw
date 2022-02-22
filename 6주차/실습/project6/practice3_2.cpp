#include "my_solver.h"

void practice3_2() {
	char readfile[256];
	char writefile[256];

	int NCOEF, DEGREE;

	double temp, a, b;
	double* poly;
	double* zeror, * zeroi;
	long int fail;
	int i, j;
	double resultr, resulti;
	double multr = 0.0f, multi = 0.0f;
	for (int t = 1; t <= 6; t++) {

		sprintf(readfile, "polynomial_3-2_%d.txt", t);
		sprintf(writefile, "roots_3-2_%d.txt", t);

		FILE* fp_r = fopen(readfile, "r");
		if (fp_r == NULL) {
			printf("%s file open error...\n", readfile);
			return;
		}

		FILE* fp_w = fopen(writefile, "w");
		if (fp_w == NULL) {
			printf("%s file open error...\n", writefile);
			return;
		}

		fscanf(fp_r, "%d", &DEGREE);
		NCOEF = DEGREE + 1;

		poly = (double*)malloc(sizeof(double) * NCOEF);
		zeror = (double*)malloc(sizeof(double) * DEGREE);
		zeroi = (double*)malloc(sizeof(double) * DEGREE);

		/********************************/

		for (i = 0; i < NCOEF; i++) {
			fscanf(fp_r, "%lf", &poly[i]);
		}


		rpoly_(poly, &DEGREE, zeror, zeroi, &fail);
		if (fail) {
			fprintf(fp_w, "fail\n");
			continue;
		}

		for (i = 0; i < DEGREE; i++)
			fprintf(fp_w, "%10f ", zeror[i]);
		fprintf(fp_w, "\n");
		for (i = 0; i < DEGREE; i++)
			fprintf(fp_w, "%10f ", zeroi[i]);
		fprintf(fp_w, "\n");

		fprintf(fp_w, "***** |f(x*)| *****\n");

		for (j = 0; j < DEGREE; j++) {
			resultr = poly[DEGREE], resulti = 0.0f;
			multr = zeror[j], multi = zeroi[j];
			for (i = 0; i < DEGREE; i++) {
				resultr += poly[DEGREE - i - 1] * multr;
				resulti += poly[DEGREE - i - 1] * multi;
				a = multr, b = multi;
				multr = a * zeror[j] - b * zeroi[j];
				multi = a * zeroi[j] + zeror[j] * b;
			}
			temp = sqrt(resultr * resultr + resulti * resulti);
			fprintf(fp_w, "%lf\n", temp);
		}

		/********************************/


		free(zeroi);
		free(zeror);
		free(poly);

		if (fp_r != NULL) fclose(fp_r);
		if (fp_w != NULL) fclose(fp_w);
	}
}