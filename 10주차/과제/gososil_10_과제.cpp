#include <stdio.h>
#include <string.h>
#include <random>
#include <time.h>

#include <math.h>
#include <time.h>
#include <Windows.h>

__int64 start, freq, end;
#define CHECK_TIME_START QueryPerformanceFrequency((LARGE_INTEGER*)&freq); QueryPerformanceCounter((LARGE_INTEGER*)&start)
#define CHECK_TIME_END(a) QueryPerformanceCounter((LARGE_INTEGER*)&end); a = (float)((float)(end - start) / (freq / 1000.0f))
float compute_time;
float compute_time1, compute_time2;


#define MATDIM 1024
#define HW1_N 100000
#define HW3_N 10000000
float* hw1_x, hw1_E, hw1_var1, hw1_var2;
float hw2_a, hw2_b, hw2_c, hw2_naive_ans[2], hw2_pre_ans[2];
float hw3_ans1_1, hw3_ans1_2, hw3_ans2_1, hw3_ans2_2;
float hw3_ans3_1, hw3_ans3_2,hw3_ans4_1,hw3_ans4_2;
float hw3_ans5_1, hw3_ans5_2;
#define foo(a, b) (((a)-(b)) / ((b)+1) /((a)+1))


/* hw1 */
void init_hw1(int fixed);
void hw1_calc_e();
void hw1_calc_var1();
void hw1_calc_var2();
/* hw2 */
void hw2_naive();
void hw2_safe();
float hw2_verify(float x);
/* hw3 */
void hw3_recursion(float i);
void hw3_recursion_2(void);
void hw3_avoid_divide(void);
void hw3_avoid_divide_2(void);
void hw3_macro(void);
void hw3_macro_2(void);
float foo_2(float, float);
void hw3_loop_inversion(void);
void hw3_loop_inversion_2(void);
void hw3_using_register(void);
void hw3_using_register_2(void);

void main(void)
{
	srand((unsigned)time(NULL));

	/* hw1 */
	puts("====== hw1 ======");
	init_hw1(0);
	hw1_calc_e();
	CHECK_TIME_START;
	hw1_calc_var1();
	CHECK_TIME_END(compute_time);
	compute_time1 = compute_time;
	printf("hw1_calc_var1 = %f ms\n", compute_time);
	printf("hw1_calc_var1 value = %.15f\n", hw1_var1);


	CHECK_TIME_START;
	hw1_calc_var2();
	CHECK_TIME_END(compute_time);
	compute_time2 = compute_time;
	printf("hw1_calc_var2 = %f ms\n", compute_time);
	printf("hw1_calc_var2 value = %.15f\n", hw1_var2);
	puts("");

	/* hw2 */
	puts("====== hw2 ======");
	printf("a, b, c : ");
	scanf("%f %f %f", &hw2_a, &hw2_b, &hw2_c);
	hw2_naive();
	hw2_safe();
	printf("naive result    : %.15f, %.15f\n", hw2_naive_ans[0], hw2_naive_ans[1]);
	printf("advanced result : %.15f, %.15f\n", hw2_pre_ans[0], hw2_pre_ans[1]);
	puts("");
	printf("Verifying naive ans    : %.15f, %.15f\n", hw2_verify(hw2_naive_ans[0]), hw2_verify(hw2_naive_ans[1]));
	printf("Verifying advanced ans : %.15f, %.15f\n", hw2_verify(hw2_pre_ans[0]), hw2_verify(hw2_pre_ans[1]));
	puts("");

	/* hw3 */
	puts("====== hw3 ======");

	//1
	hw3_ans1_1 = 0.0, hw3_ans1_2 = 0.0;
	CHECK_TIME_START;
	hw3_recursion(0.0);
	CHECK_TIME_END(compute_time);
	compute_time1 = compute_time;
	printf("hw3_recursion = %f ms\n", compute_time);
	printf("hw3_recursion value = %.15f\n", hw3_ans1_1);

	CHECK_TIME_START;
	hw3_recursion_2();
	CHECK_TIME_END(compute_time);
	compute_time2 = compute_time;
	printf("hw3_recursion_2 = %f ms\n", compute_time);
	printf("hw3_recursion_2 value = %.15f\n", hw3_ans1_2);
	puts("");

	//2
	CHECK_TIME_START;
	hw3_avoid_divide();
	CHECK_TIME_END(compute_time);
	compute_time1 = compute_time;
	printf("hw3_avoid_divide = %f ms\n", compute_time);
	printf("hw3_avoid_divide value = %.15f\n", hw3_ans2_1);

	CHECK_TIME_START;
	hw3_avoid_divide_2();
	CHECK_TIME_END(compute_time);
	compute_time2 = compute_time;
	printf("hw3_avoid_divide_2 = %f ms\n", compute_time);
	printf("hw3_avoid_divide_2 value = %.15f\n", hw3_ans2_2);
	puts("");

	//3
	CHECK_TIME_START;
	hw3_macro();
	CHECK_TIME_END(compute_time);
	compute_time1 = compute_time;
	printf("hw3_macro = %f ms\n", compute_time);
	printf("hw3_macro value = %.15f\n", hw3_ans3_1);

	CHECK_TIME_START;
	hw3_macro_2();
	CHECK_TIME_END(compute_time);
	compute_time2 = compute_time;
	printf("hw3_macro_2 = %f ms\n", compute_time);
	printf("hw3_macro_2 value = %.15f\n", hw3_ans3_2);
	puts("");

	//4
	CHECK_TIME_START;
	hw3_loop_inversion();
	CHECK_TIME_END(compute_time);
	compute_time1 = compute_time;
	printf("hw3_loop_inversion = %f ms\n", compute_time);
	printf("hw3_loop_inversion value = %.15f\n", hw3_ans4_1);

	CHECK_TIME_START;
	hw3_loop_inversion_2();
	CHECK_TIME_END(compute_time);
	compute_time2 = compute_time;
	printf("hw3_loop_inversion_2 = %f ms\n", compute_time);
	printf("hw3_loop_inversion_2 value = %.15f\n", hw3_ans4_2);
	puts("");
	
	
	//5
	CHECK_TIME_START;
	hw3_using_register();
	CHECK_TIME_END(compute_time);
	compute_time1 = compute_time;
	printf("hw3_using_register = %f ms\n", compute_time);
	printf("hw3_using_register value = %.15f\n", hw3_ans5_1);

	CHECK_TIME_START;
	hw3_using_register_2();
	CHECK_TIME_END(compute_time);
	compute_time2 = compute_time;
	printf("hw3_using_register_2 = %f ms\n", compute_time);
	printf("hw3_using_register_2 value = %.15f\n", hw3_ans5_2);
	puts("");

}

void init_hw1(int fixed)
{
	float* ptr;
	hw1_x = (float*)malloc(sizeof(float) * HW1_N);

	if (fixed)
	{
		float tmp = HW1_N;
		for (int i = 0; i < HW1_N; i++)
		{
			if (i & 1) tmp -= 0.0001;
			hw1_x[i] = tmp;
		}
	}
	else
	{
		srand((unsigned)time(NULL));
		ptr = hw1_x;
		for (int i = 0; i < HW1_N; i++)
			*ptr++ = ((float)rand() / (float)RAND_MAX) * 2;
	}
}
void hw1_calc_e()
{
	hw1_E = 0.0f;
	for (int i = 0; i < HW1_N; i++) {
		hw1_E += hw1_x[i];
	}
	hw1_E = hw1_E / HW1_N;
}

void hw1_calc_var1()
{
	hw1_var1 = 0.0f;
	float a = 0.0f, b = 0.0f;
	for (int i = 0; i < HW1_N; i++) {
		a += hw1_x[i] * hw1_x[i];
		b += hw1_x[i];
	}
	hw1_var1 = (float)(HW1_N * a - b * b) / (HW1_N * (HW1_N - 1));
	hw1_var1 = sqrt(hw1_var1);
}
void hw1_calc_var2()
{
	hw1_var2 = 0.0f;
	for (int i = 0; i < HW1_N; i++) {
		hw1_var2 += (hw1_x[i] - hw1_E) * (hw1_x[i] - hw1_E);
	}
	hw1_var2 = hw1_var2 / (HW1_N - 1);

	hw1_var2 = sqrt(hw1_var2);
}

/* hw2 */
void hw2_naive()
{
	hw2_naive_ans[0] = (-hw2_b + sqrt(hw2_b * hw2_b - 4 * hw2_a * hw2_c)) / (2 * hw2_a);
	hw2_naive_ans[1] = (-hw2_b - sqrt(hw2_b * hw2_b - 4 * hw2_a * hw2_c)) / (2 * hw2_a);
}
void hw2_safe()
{
	//printf("%lf\n", sqrt(hw2_b * hw2_b - 4 * hw2_a * hw2_c));
	if (hw2_b > 0) {
		hw2_pre_ans[0] = -2 * hw2_c / (hw2_b + sqrt(hw2_b * hw2_b - 4 * hw2_a * hw2_c));
		hw2_pre_ans[1] = (-hw2_b - sqrt(double(hw2_b * hw2_b - 4 * hw2_a * hw2_c))) / (2 * hw2_a);
	}
	else {
		hw2_pre_ans[0] = (-hw2_b + sqrt(double(hw2_b * hw2_b - 4 * hw2_a * hw2_c))) / (2 * hw2_a); 
		hw2_pre_ans[1] = -2 * hw2_c / (hw2_b - sqrt(hw2_b * hw2_b - 4 * hw2_a * hw2_c));
	}
}
float hw2_verify(float x)
{
	return hw2_a * x * x + hw2_b * x + hw2_c;
}

//hw3_1
void hw3_recursion(float i)
{
	if (i < 1000) {
		hw3_ans1_1++;
		hw3_recursion(hw3_ans1_1);
	}
}

void hw3_recursion_2(void)
{
	for (hw3_ans1_2 = 0.0f; hw3_ans1_2 < 1000;) {
		hw3_ans1_2++;
	}

}

//hw3_2

void hw3_avoid_divide(void)
{
	float a = 100.0f, b = 15.0f, c = 7.0f, d = 13.243f, e = 3.1f;
	hw3_ans2_1 = 0.0f;

	for (int i = 0; i < HW3_N; i++) {

		hw3_ans2_1 += a / b / c / d / e;
	}

}

void hw3_avoid_divide_2(void)
{
	float a = 100.0f, b = 15.0f, c = 7.0f, d = 13.243f, e = 3.1f;
	hw3_ans2_2 = 0.0f;

	b = 1.0 / b;
	c = 1.0 / c;
	d = 1.0 / d;
	e = 1.0 / e;
	for (int i = 0; i < HW3_N; i++) {

		hw3_ans2_2 += a * b * c * d * e;
	}

}

//hw3_3 : 

void hw3_macro(void) {
	hw3_ans3_1 = 0.0f;
	float a = 5.332f;
	float b = 0.435f;
	float temp;
	for (int i = 0; i < HW3_N; i++) {
		
		hw3_ans3_1 += foo_2(a,b);
	}
}

void hw3_macro_2(void) {
	hw3_ans3_2 = 0.0f;
	float a = 5.332f;
	float b = 0.435f;
	float temp;
	for (int i = 0; i < HW3_N; i++) {
		temp= foo(a, b);
		hw3_ans3_2 += temp;
	}
}

float foo_2(float a, float b) {
	float temp = a - b;
	temp /= (b + 1);
	temp /= (a + 1.0f);
	return temp;
}

//hw3_4 
void hw3_loop_inversion(void) {
	float a = 100.0f, b = 15.0f, c = 7.0f, d = 13.243f, e = 3.1f;
	hw3_ans4_1 = 0.0f;
	int i = 0;
	while (i < HW3_N) {
		hw3_ans4_1 += a / b / c / d / e;
		i++;
	} 
}

void hw3_loop_inversion_2(void) {
	float a = 100.0f, b = 15.0f, c = 7.0f, d = 13.243f, e = 3.1f;
	hw3_ans4_2 = 0.0f;

	int i =0;
	do {
		hw3_ans4_2 += a / b / c / d / e;
		i++;
	} while (i < HW3_N);
	
}

//hw3_5
void hw3_using_register(void) {
	hw3_ans5_1 = 0.0f;
	float a = 5.332f;
	float b = 0.435f;
	float temp;
	for (int i = 0; i < HW3_N; i++) {
		temp = a - b;
		temp /= (b + 1);
		temp /= (a + 1.0f);
		hw3_ans5_1 += temp;
	}
}

void hw3_using_register_2(void) {
	hw3_ans5_2 = 0.0f;
	register float a = 5.332f;
	register float b = 0.435f;
	register float temp;
	register int i;
	for (i = 0; i < HW3_N; i++) {
		temp = a - b;
		temp /= (b + 1);
		temp /= (a + 1.0f);
		hw3_ans5_2 += temp;
	}
}