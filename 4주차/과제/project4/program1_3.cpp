#include "my_solver.h"

extern double (*_f)(double);
extern double (*_fp)(double);

/*********************************************
  Bisection Method -- HOMEWORK
**********************************************/
void program1_3(FILE *fp)
{
	double a0, b0, x0, x1 , temp;
	int n=0;
	scanf("%lf %lf", &a0, &b0);

	fprintf(fp, "*********************************************\n\
Bisection Method\n\
**********************************************\n ");
	fprintf(fp, "n\t\txn1\t\t|f(xn1)|\n");
	x0 = (a0 + b0) / 2.0;
	fprintf(fp, "%d %.15e %.15e\n", n, x0, fabs(_f(x0)));
	for (n = 1; n < Nmax; n++) {
		
		if (_f(x0) * _f(b0) < 0){
			x1 = (x0 + b0) / 2.0;
			a0 = x0;
		}
		else {
			x1 = (x0 + a0) / 2.0;
			b0 = x0;
		}
		fprintf(fp, "%d %.15e %.15e\n", n, x1, fabs(_f(x1)));
		if (fabs(x1 - x0) < EPSILON || fabs(_f(x1)) < DELTA)
			break;
		x0 = x1;
	}
	printf("Bisection Method: root= %.15e\n", x1);
}