#include "my_solver.h"

extern float (*_sp_f)(float);
extern float (*_sp_fp)(float);

/*********************************************
  Newton-Rapson Method
**********************************************/
void sp_program1_1(FILE* fp) {
	float x0, x1;
	int n=0;

	if (fp == NULL)
		return;

	scanf("%f", &x0);

	double f = _sp_f(x0);
	double p = _sp_fp(x0);
	fprintf(fp, "*********************************************\n\
Newton-Rapson Method (single precision) \n\
*********************************************\n ");
	fprintf(fp, "n\t\txn1\t\t|f(xn1)|\n");
	fprintf(fp, "%d %.15e %.15e\n", n, x0, fabsf(f));

	for (n = 1; n < Nmax; n++) {
		x1 = x0 - f / p;
		fprintf(fp, "%d %.15e %.15e\n", n, x1, fabsf(_sp_f(x1)));
		if (fabsf(x1 - x0) < EPSILON || fabsf(_sp_f(x1)) < DELTA)
			break;
		x0 = x1;
		f = _sp_f(x0);
		p = _sp_fp(x0);
	}
	printf("Newton-Rapson Method (single precision): root= %.15e\n", x1);

}
