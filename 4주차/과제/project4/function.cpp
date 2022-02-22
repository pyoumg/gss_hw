#define _USE_MATH_DEFINES
#include <math.h>

double _f1(double x) {
	return x * x - 4 * x + 4 - log(x);
}

double _fp1(double x) {
	return 2 * x - 4 - 1 / x;
}

double _f2(double x) {
	return x+1-2.0*sin(M_PI*x);
}

double _fp2(double x) {
	return 1.0-2.0*M_PI*cos(M_PI * x);
}

double _f3(double x) {
	return pow(x, 4) - 11.0 * pow(x, 3) + 42.35 * pow(x, 2) - 66.55 * x + 35.1384;
}

double _fp3(double x) {
	return 4.0 * pow(x, 3) - 33.0 * pow(x, 2) + 42.35 * 2 * x-66.55;
}



double _f_sqrt(double x) {
	return x * x - 2.0;
}

double _fp_sqrt(double x) {
	return 2.0 * x;
}

double _f_vehicle(double x) {
	double l = 89.0, h = 49.0, D = 55.0, b = 11.5*M_PI/180;

	double A = l * sin(b);
	double B = l * cos(b);
	double C = (h + 0.5 * D) * sin(b) - 0.5 * D * tan(b);
	double E = (h + 0.5 * D) * cos(b) - 0.5 * D;
	
	x = M_PI /180* x;
	return A * sin( x) * cos( x) + B * sin( x) * sin( x) - C * cos( x) - E * sin( x);
}

double _fp_vehicle(double x) {
	double l = 89.0, h = 49.0, D = 55.0, b = 11.5 * M_PI/180;

	double A = l * sin(b);
	double B = l * cos(b);
	double C = (h + 0.5 * D) * sin(b) - 0.5 * D * tan(b);
	double E = (h + 0.5 * D) * cos(b) - 0.5 * D;
	x = M_PI/180 * x;
	double result = A * cos(x) * cos(x) - A * sin(x) * sin(x) + 2.0 * B * sin(x) * cos(x) + C * sin(x) - E * sin(x);
	result = result * M_PI/180;
	
	return result;
}

double _f_comp(double x) {
	return log(x)-1;
}

double _fp_comp(double x) {
	return 1/x;
}
