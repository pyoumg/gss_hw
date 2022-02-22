#include "my_solver.h"

extern float (*_sp_f)(float);
extern float (*_sp_fp)(float);

/*********************************************
  Secant Method
**********************************************/
void sp_program1_2(FILE* fp) {
	float x0, x1, temp;
	int n;

	if (fp == NULL)
		return;
	scanf("%f %f", &x0, &x1);

	float fx0 = _sp_f(x0);
	float fx1 = _sp_f(x1);
	fprintf(fp, "*********************************************\n\
Secant Method (single precision)\n\
**********************************************\n ");
	fprintf(fp, "n\t\txn1\t\t|f(xn1)|\n");
	for (n = 0; n < Nmax; n++) {
		temp = x1 - fx1 * (x1 - x0) / (fx1 - fx0);
		fprintf(fp, "%d %.15e %.15e\n", n, x1, fabsf(fx1));
		if (fabsf(x1 - x0) < EPSILON || fabsf(fx1) < DELTA)
			break;
		x0 = x1;
		x1 = temp;
		fx0 = _sp_f(x0);
		fx1 = _sp_f(x1);


	}
	printf("Secant Method (single precision): root= %.15e\n", x1);
}
