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


#define MATDIM 1024
double* pMatA, * pMatB, * pMatC;
void MultiplySquareMatrices_1(double* pDestMatrix, double* pLeftMatrix, double* pRightMatrix, int MatSize);
void MultiplySquareMatrices_2(double* pDestMatrix, double* pLeftMatrix, double* pRightMatrix, int MatSize);
void MultiplySquareMatrices_3(double* pDestMatrix, double* pLeftMatrix, double* pRightMatrix, int MatSize);
void MultiplySquareMatrices_4(double* pDestMatrix, double* pLeftMatrix, double* pRightMatrix, int MatSize);

void init_MatMat(void);

void main(void)
{
	init_MatMat();

	CHECK_TIME_START;
	MultiplySquareMatrices_1(pMatC, pMatA, pMatB, MATDIM);
	//Sleep(500);
	CHECK_TIME_END(compute_time);
	printf("MultiplySquareMatrices_1 : %f ms\n", compute_time);

	CHECK_TIME_START;
	MultiplySquareMatrices_2(pMatC, pMatA, pMatB, MATDIM);
	//Sleep(500);
	CHECK_TIME_END(compute_time);
	printf("MultiplySquareMatrices_2 = %f ms\n", compute_time);

	CHECK_TIME_START;
	MultiplySquareMatrices_3(pMatC, pMatA, pMatB, MATDIM);
	//Sleep(500);
	CHECK_TIME_END(compute_time);
	printf("MultiplySquareMatrices_3 = %f ms\n", compute_time);

	CHECK_TIME_START;
	MultiplySquareMatrices_4(pMatC, pMatA, pMatB, MATDIM);
	//Sleep(500);
	CHECK_TIME_END(compute_time);
	printf("MultiplySquareMatrices_4 = %f ms\n", compute_time);
}

void MultiplySquareMatrices_1(double* pDestMatrix, double* pLeftMatrix, double* pRightMatrix, int MatSize)
{
	int i, j, k;
	for (i = 0; i < MatSize; i++) {
		for (j = 0; j < MatSize; j++) {
			pDestMatrix[i * MatSize + j] = 0;
			for (k = 0; k < MatSize; k++) {
				pDestMatrix[i * MatSize + j] += pLeftMatrix[i * MatSize + k] * pRightMatrix[k * MatSize + j];
			}
		}
	}

}

//메모리 접근의 응집성 높이기
void MultiplySquareMatrices_2(double* pDestMatrix, double* pLeftMatrix, double* pRightMatrix, int MatSize)
{
	int i, j, k;
	double* pMatB_t = (double*)malloc(sizeof(double) * MatSize * MatSize);
	for (i = 0; i < MatSize; i++) {
		for (j = 0; j < MatSize; j++) {
			pMatB_t[j * MatSize + i] = pRightMatrix[i * MatSize + j];
		}
	}


	for (i = 0; i < MatSize; i++) {
		for (j = 0; j < MatSize; j++) {
			pDestMatrix[i * MatSize + j] = 0;
			for (k = 0; k < MatSize; k++) {
				pDestMatrix[i * MatSize + j] += pLeftMatrix[i * MatSize + k] * pMatB_t[j * MatSize + k];
			}
		}
	}

	free(pMatB_t);
}
//LU: MATDIM 이 1024니까 2의 n제곱으로 풀어야함: 2^2
//가장 안쪽의 for문 풀기 (강의자료)
void MultiplySquareMatrices_3(double* pDestMatrix, double* pLeftMatrix, double* pRightMatrix, int MatSize)
{
	int i, j, k;
	double* pMatB_t = (double*)malloc(sizeof(double) * MatSize * MatSize);
	for (i = 0; i < MatSize; i++) {
		for (j = 0; j < MatSize; j++) {
			pMatB_t[j * MatSize + i] = pRightMatrix[i * MatSize + j];
		}
	}


	for (i = 0; i < MatSize; i++) {
		for (j = 0; j < MatSize; j++) {
			pDestMatrix[i * MatSize + j] = 0;
			for (k = 0; k < MatSize ; k += 4) {
				pDestMatrix[i * MatSize + j] += pLeftMatrix[i * MatSize + k] * pMatB_t[j * MatSize + k];
				pDestMatrix[i * MatSize + j] += pLeftMatrix[i * MatSize + k + 1] * pMatB_t[j * MatSize + k + 1];
				pDestMatrix[i * MatSize + j] += pLeftMatrix[i * MatSize + k + 2] * pMatB_t[j * MatSize + k + 2];
				pDestMatrix[i * MatSize + j] += pLeftMatrix[i * MatSize + k + 3] * pMatB_t[j * MatSize + k + 3];
			}
		}
	}

	free(pMatB_t);
}
//LU: 2^3
void MultiplySquareMatrices_4(double* pDestMatrix, double* pLeftMatrix, double* pRightMatrix, int MatSize)
{
	int i, j, k;
	double* pMatB_t = (double*)malloc(sizeof(double) * MatSize * MatSize);
	for (i = 0; i < MatSize; i++) {
		for (j = 0; j < MatSize; j++) {
			pMatB_t[j * MatSize + i] = pRightMatrix[i * MatSize + j];
		}
	}


	for (i = 0; i < MatSize; i++) {
		for (j = 0; j < MatSize; j++) {
			pDestMatrix[i * MatSize + j] = 0;
			for (k = 0; k < MatSize ; k += 8) {
				pDestMatrix[i * MatSize + j] += pLeftMatrix[i * MatSize + k] * pMatB_t[j * MatSize + k];
				pDestMatrix[i * MatSize + j] += pLeftMatrix[i * MatSize + k + 1] * pMatB_t[j * MatSize + k + 1];
				pDestMatrix[i * MatSize + j] += pLeftMatrix[i * MatSize + k + 2] * pMatB_t[j * MatSize + k + 2];
				pDestMatrix[i * MatSize + j] += pLeftMatrix[i * MatSize + k + 3] * pMatB_t[j * MatSize + k + 3];
				pDestMatrix[i * MatSize + j] += pLeftMatrix[i * MatSize + k + 4] * pMatB_t[j * MatSize + k + 4];
				pDestMatrix[i * MatSize + j] += pLeftMatrix[i * MatSize + k + 5] * pMatB_t[j * MatSize + k + 5];
				pDestMatrix[i * MatSize + j] += pLeftMatrix[i * MatSize + k + 6] * pMatB_t[j * MatSize + k + 6];
				pDestMatrix[i * MatSize + j] += pLeftMatrix[i * MatSize + k + 7] * pMatB_t[j * MatSize + k + 7];
			}
		}
	}

	free(pMatB_t);
}

void init_MatMat(void)
{
	double* ptr;
	pMatA = (double*)malloc(sizeof(double) * MATDIM * MATDIM);
	pMatB = (double*)malloc(sizeof(double) * MATDIM * MATDIM);
	pMatC = (double*)malloc(sizeof(double) * MATDIM * MATDIM);
	srand((unsigned)time(NULL));
	ptr = pMatA;
	for (int i = 0; i < MATDIM * MATDIM; i++)
		*ptr++ = (double)rand() / ((double)RAND_MAX);
	ptr = pMatB;
	for (int i = 0; i < MATDIM * MATDIM; i++)
		*ptr++ = (double)rand() / ((double)RAND_MAX);
}
