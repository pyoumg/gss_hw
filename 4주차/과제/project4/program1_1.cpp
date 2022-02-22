#include "my_solver.h"

extern double (*_f)(double);
extern double (*_fp)(double);

/*********************************************
  Newton-Rapson Method
**********************************************/
void program1_1(FILE* fp) {
	double x0, x1=0.0;
	int n=0;

	if (fp == NULL)
		return;
	scanf("%lf", &x0);

	double f =  _f(x0);
	double p = _fp(x0);
	fprintf(fp, "*********************************************\n\
Newton-Rapson Method\n\
*********************************************\n ");
	fprintf(fp, "n\t\txn1\t\t|f(xn1)|\n");
	fprintf(fp, "%d %.15e %.15e\n", n, x0, fabs(f));

	for (n = 1; n < Nmax; n++) {
		x1 = x0 - f / p;
		fprintf(fp, "%d %.15e %.15e\n", n, x1, fabs(_f(x1)));
		if (fabs(x1 - x0) < EPSILON || fabs(_f(x1)) < DELTA)
			break;
		x0 = x1;
		f = _f(x0);
		p = _fp(x0);
	}
	printf("Newton-Rapson Method: root= %.15e\n", x1);
}
