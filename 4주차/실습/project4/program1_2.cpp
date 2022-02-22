#include "my_solver.h"

extern double (*_f)(double);
extern double (*_fp)(double);

/*********************************************
  Secant Method
**********************************************/
void program1_2(FILE* fp) {
	double x0, x1, temp;
	int n;

	if (fp == NULL)
		return;
	scanf("%lf %lf", &x0,&x1);

	double fx0 = _f(x0);
	double fx1 = _f(x1);
	fprintf(fp, "*********************************************\n\
Secant Method\n\
**********************************************\n ");
	fprintf(fp, "n\t\txn1\t\t|f(xn1)|\n");
	for (n = 0; n < Nmax; n++) {
		temp = x1 - fx1*(x1-x0)/(fx1-fx0);
		fprintf(fp, "%d %.15e %.15e\n", n, x1, fabs(fx1));
		if (fabs(x1 - x0) < EPSILON || fabs(fx1) < DELTA)
			break;
		x0 = x1;
		x1 = temp;
		fx0 = _f(x0);
		fx1 = _f(x1);
		
		
	}
	printf("Secant Method: root= %.15e\n", x1);
}
