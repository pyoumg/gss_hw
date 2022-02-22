
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>
#include<stdio.h>
#include<stdlib.h>
#include <math.h>
#include <Windows.h>
#include <time.h>
#include <assert.h>




#define CUDA_CALL(x) { const cudaError_t a = (x); if(a != cudaSuccess) { printf("\nCuda Error: %s (err_num=%d) at line:%d\n", cudaGetErrorString(a), a, __LINE__); cudaDeviceReset(); assert(0);}}
typedef float TIMER_T;
#define USE_CPU_TIMER 1
#define USE_GPU_TIMER 1
#if USE_CPU_TIMER == 1
__int64 start, freq, end;
#define CHECK_TIME_START { QueryPerformanceFrequency((LARGE_INTEGER*)&freq); QueryPerformanceCounter((LARGE_INTEGER*)&start); }
#define CHECK_TIME_END(a) { QueryPerformanceCounter((LARGE_INTEGER*)&end); a = (float)((float)(end - start) / (freq / 1000.0f)); }
#else
#define CHECK_TIME_START
#define CHECK_TIME_END(a)
#endif

#if USE_GPU_TIMER == 1
cudaEvent_t cuda_timer_start, cuda_timer_stop;
#define CUDA_STREAM_0 (0)

// CUDA event 객체를 사용하여 커널 실행시간 측정
void create_device_timer()
{
	CUDA_CALL(cudaEventCreate(&cuda_timer_start));
	CUDA_CALL(cudaEventCreate(&cuda_timer_stop));
}

void destroy_device_timer()
{
	CUDA_CALL(cudaEventDestroy(cuda_timer_start));
	CUDA_CALL(cudaEventDestroy(cuda_timer_stop));
}

inline void start_device_timer()
{
	cudaEventRecord(cuda_timer_start, CUDA_STREAM_0);
}

inline TIMER_T stop_device_timer()
{
	TIMER_T ms;
	cudaEventRecord(cuda_timer_stop, CUDA_STREAM_0);
	cudaEventSynchronize(cuda_timer_stop);

	cudaEventElapsedTime(&ms, cuda_timer_start, cuda_timer_stop);
	return ms;
}

#define CHECK_TIME_INIT_GPU() { create_device_timer(); }
#define CHECK_TIME_START_GPU() { start_device_timer(); }
#define CHECK_TIME_END_GPU(a) { a = stop_device_timer(); }
#define CHECK_TIME_DEST_GPU() { destroy_device_timer(); }
#else
#define CHECK_TIME_INIT_GPU()
#define CHECK_TIME_START_GPU()
#define CHECK_TIME_END_GPU(a)
#define CHECK_TIME_DEST_GPU()
#endif

TIMER_T compute_time = 0;
TIMER_T device_time = 0;




#define N_EQUATIONS	(1<<20)
#define BLOCK_SIZE 256
float(*pX0), (*pX1);
float(*pX0_G), (*pX1_G);//gpu
float(*pFX0), (*pFX1);//cpu
float(*pFX0_G), (*pFX1_G);//gpu
float(*pA), (*pB), (*pC);


void init_data(void) {
	srand((unsigned)time(NULL));
	FILE* fp_a = fopen("A.bin", "wb"); //write binary
	FILE* fp_b = fopen("B.bin", "wb");
	FILE* fp_c = fopen("C.bin", "wb");

	int i, j;
	float x;

	for (i = 0; i < N_EQUATIONS; i++) {
		x = 2.0f * ((float)rand() / RAND_MAX) - 1.0f;
		fwrite(&x, sizeof(float), 1, fp_a);

	}

	for (i = 0; i < N_EQUATIONS; i++) {
		x = 1.0f * ((float)rand() / RAND_MAX)  +7.0f;
		fwrite(&x, sizeof(float), 1, fp_b);

	}
	for (i = 0; i < N_EQUATIONS; i++) {
		x = 2.0f * ((float)rand() / RAND_MAX) - 1.0f;
		fwrite(&x, sizeof(float), 1, fp_c);

	}

	fclose(fp_a);
	fclose(fp_b);
	fclose(fp_c);
	return;
}

void init_Mat(void)
{
	srand((unsigned)time(NULL));
	FILE* fp_a = fopen("A.bin", "rb");
	FILE* fp_b = fopen("B.bin", "rb");
	FILE* fp_c = fopen("C.bin", "rb");


	pFX0 = new float[N_EQUATIONS];
	pFX1 = new float[N_EQUATIONS];
	pFX0_G = new float[N_EQUATIONS];
	pFX1_G = new float[N_EQUATIONS];

	pX0 = new float[N_EQUATIONS];
	pX1 = new float[N_EQUATIONS];
	pX0_G = new float[N_EQUATIONS];
	pX1_G = new float[N_EQUATIONS];

	pA = new float[N_EQUATIONS];
	pB = new float[N_EQUATIONS];
	pC = new float[N_EQUATIONS];


	fread(pA, sizeof(float), N_EQUATIONS, fp_a);
	fread(pB, sizeof(float), N_EQUATIONS, fp_b);
	fread(pC, sizeof(float), N_EQUATIONS, fp_c);

	fclose(fp_a);
	fclose(fp_b);
	fclose(fp_c);
}

void write_data() {
	FILE* fp_x0 = fopen("X0.bin", "wb");
	FILE* fp_x1 = fopen("X1.bin", "wb");
	FILE* fp_fx0 = fopen("FX0.bin", "wb");
	FILE* fp_fx1 = fopen("FX1.bin", "wb");

	for (int i = 0; i < N_EQUATIONS; i++) {
		fwrite(&pX0_G[i], sizeof(float), 1, fp_x0);
		fwrite(&pX1_G[i], sizeof(float), 1, fp_x1);
		fwrite(&pFX0_G[i], sizeof(float), 1, fp_fx0);
		fwrite(&pFX1_G[i], sizeof(float), 1, fp_fx1);
	}


	fclose(fp_x0);
	fclose(fp_x1);
	fclose(fp_fx0);
	fclose(fp_fx1);
}

void find_roots_CPU(float* A, float* B, float* C,
	float* X0, float* X1, float* FX0, float* FX1, int n) {
	int i;
	float a, b, c, d, x0, x1, tmp, temp;
	for (i = 0; i < n; i++) {
		a = A[i]; b = B[i]; c = C[i];
		d = sqrtf(b * b - 4.0f * a * c);
		tmp = 1.0f / (2.0f * a);
		X0[i] = x0 = (-b - d) * tmp;
		X1[i] = x1 = (-b + d) * tmp;

		if (x0 > x1) {
			temp = x0;
			x0 = x1;
			x1 = temp;
			X0[i] = x0, X1[i] = x1;
		}


		FX0[i] = (a * x0 + b) * x0 + c;
		FX1[i] = (a * x1 + b) * x1 + c;

		//printf("%f %f %f %f\n", X0[i], X1[i], FX0[i], FX1[i]);
	}
}

__global__ void find_roots_Kernel(float* A, float* B, float* C,
	float* X0, float* X1, float* FX0, float* FX1) {
	int id = blockDim.x * blockIdx.x + threadIdx.x;
	float a, b, c, d, x0, x1, tmp, temp;

	a = A[id]; b = B[id]; c = C[id];
	d = sqrtf(b * b - 4.0f * a * c);
	tmp = 1.0f / (2.0f * a);
	X0[id] = x0 = (-b - d) * tmp;
	X1[id] = x1 = (-b + d) * tmp;
	if (x0 > x1) {
		temp = x0;
		x0 = x1;
		x1 = temp;
		X0[id] = x0, X1[id] = x1;
	}
	FX0[id] = (a * x0 + b) * x0 + c;
	FX1[id] = (a * x1 + b) * x1 + c;

}


cudaError_t find_roots_GPU(float* A, float* B, float* C,
	float* X0, float* X1, float* FX0, float* FX1, int n);


int main()
{
	int n_elements;

	srand((unsigned int)time(NULL));
	n_elements = N_EQUATIONS;

	printf("*** Data size : %d\n\n", n_elements);

	init_data();
	init_Mat();
	CHECK_TIME_START;
	find_roots_CPU(pA, pB, pC, pX0, pX1, pFX0, pFX1, n_elements);
	CHECK_TIME_END(compute_time);

	printf("***CPU X0[8] = %f Time taken = %.6fms\n",pX0[8],compute_time);


	cudaError_t cudaStatus = find_roots_GPU(pA, pB, pC, pX0_G, pX1_G, pFX0_G, pFX1_G, n_elements);

	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "find_roots_GPU failed!");
		return 1;
	}
	printf("***GPU X0[8] = %f Time taken = %.6fms\n", pX0_G[8], device_time);


	// cudaDeviceReset must be called before exiting in order for profiling and
	// tracing tools such as Nsight and Visual Profiler to show complete traces.
	cudaStatus = cudaDeviceReset();
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaDeviceReset failed!");
		return 1;
	}
	write_data();
	return 0;
}
cudaError_t find_roots_GPU(float* A, float* B, float* C,
	float* X0, float* X1, float* FX0, float* FX1, int n) {
	float * pa, * pb, * pc, * x0, * x1, * fx0, * fx1;

	float size = n * sizeof(float);

	cudaError_t cudaStatus;
	// Choose which GPU to run on, change this on a multi-GPU system.
	cudaStatus = cudaSetDevice(0);
	if (cudaStatus != cudaSuccess) {
		fprintf(stderr, "cudaSetDevice failed!  Do you have a CUDA-capable GPU installed?");
		goto Error;
	}/////////////  if(cu.....  ==CUDA_CALL

	cudaDeviceProp deviceProp;
	CUDA_CALL(cudaGetDeviceProperties(&deviceProp, 0));

		CUDA_CALL(cudaMalloc(&pa, size))
			CUDA_CALL(cudaMemcpy(pa, A, size, cudaMemcpyHostToDevice))
		CUDA_CALL(cudaMalloc(&pb, size))
			CUDA_CALL(cudaMemcpy(pb, B, size, cudaMemcpyHostToDevice))
		CUDA_CALL(cudaMalloc(&pc, size))
			CUDA_CALL(cudaMemcpy(pc, C, size, cudaMemcpyHostToDevice))

		CUDA_CALL(cudaMalloc(&x0, size))
		CUDA_CALL(cudaMalloc(&x1, size))
		CUDA_CALL(cudaMalloc(&fx0, size))
		CUDA_CALL(cudaMalloc(&fx1, size))

		

		// Assume that width and height are multiples of BLOCK SIZE.
		dim3 dimBlock(BLOCK_SIZE,1);
	dim3 dimGrid(n / dimBlock.x,1);
	CHECK_TIME_INIT_GPU();
	CHECK_TIME_START_GPU();
	find_roots_Kernel << < dimGrid, dimBlock >> > (pa, pb, pc, x0, x1, fx0, fx1);
	CHECK_TIME_END_GPU(device_time);
	CHECK_TIME_DEST_GPU();
	CUDA_CALL(cudaGetLastError())

	// cudaDeviceSynchronize waits for the kernel to finish, and returns
	// any errors encountered during the launch.
	CUDA_CALL(cudaDeviceSynchronize())

		CUDA_CALL(cudaMemcpy(X0, x0, size, cudaMemcpyDeviceToHost))
		CUDA_CALL(cudaMemcpy(X1, x1, size, cudaMemcpyDeviceToHost))
		CUDA_CALL(cudaMemcpy(FX0, fx0, size, cudaMemcpyDeviceToHost))
		CUDA_CALL(cudaMemcpy(FX1, fx1, size, cudaMemcpyDeviceToHost))

		Error:
	cudaFree(pa);
	cudaFree(pb);
	cudaFree(pc);
	cudaFree(x0);
	cudaFree(x1);
	cudaFree(fx0);
	cudaFree(fx1);
	return cudaStatus;
}
