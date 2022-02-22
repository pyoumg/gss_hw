#ifndef __MY_SOLVER_H__
#define __MY_SOLVER_H__

#include <stdio.h>
#include <Windows.h>

#define CHECK_TIME_START QueryPerformanceFrequency((_LARGE_INTEGER*) &freq); QueryPerformanceCounter((_LARGE_INTEGER*)&start);
#define CHECK_TIME_END(a) QueryPerformanceCounter((_LARGE_INTEGER*)&end); a = (float)((float) (end - start)/freq);

#define DELTA 0.000000000001
#define Nmax 100
#define EPSILON 0.00000000001

void program2_1();
void program2_2();

int rand(void);
void srand(unsigned int seed);
double get_integral(double u, double h, double x0, double* x, double* y);

double bisection(double* x, double* y, double h, double u, int n);
// HOMEWORK
void program2_2_a(void);
void program2_2_b(void);
void program2_3(void);
void program2_1_a();
void program2_2_a();
void program2_2_b(); 
void program2_3_test(char* ,char*);
#endif  // __MY_SOLVER_H__
