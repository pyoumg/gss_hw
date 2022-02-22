#pragma once

#include "cuda_code.cuh"

#include <stdio.h>
#include<stdio.h>
#include<stdlib.h>
#include <math.h>
#include <Windows.h>
#include <time.h>
#include <assert.h>

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

#define Window 2
#define BLOCK_SIZE_X  8
#define  BLOCK_SIZE_Y  8
#define SHARED_MEM(x, y)		sharedBuffer[SMW * (y) + (x)]
__constant__ float constant_gaussian_kernel[ 25 ];

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	
//	Gaussian ���͸��� �ϴ� Ŀ��
//	shared memory�� ������� �ʴ´�
//	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

__global__ void Gaussian_kernel_no_shared(IN unsigned char *d_bitmaps, OUT unsigned char *d_Gaussian, long width, long height) {	
	int row = blockDim.y * blockIdx.y + threadIdx.y;
	int col = blockDim.x * blockIdx.x + threadIdx.x;
	int id = width * row + col;

	int r, c,kernel_idx=0;
	int r_idx, c_idx;
	char temp = 0;

	for (r = -2; r <= 2; r++) {
		r_idx = row + r;
		if (r_idx < 0) r_idx = 0;
		else if (r_idx > height - 1) r_idx = height - 1;
		for (c = -2; c <= 2; c++) {
			c_idx = col+ c;
			if (c_idx < 0) c_idx = 0;
			else if (c_idx > width - 1) c_idx = width - 1;
			temp += constant_gaussian_kernel[kernel_idx++] * d_bitmaps[r_idx *width+ c_idx];

		}
	}
	d_Gaussian[id] = temp;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	
//	Gaussian ���͸��� �ϴ� Ŀ��
//	shared memory�� ����Ѵ�.
//	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern __shared__ unsigned char sharedBuffer[];
__global__ void Gaussian_kernel_shared(INOUT unsigned char *d_bitmaps, OUT unsigned char *d_Gaussian, long width, long height) {
	int row = blockDim.y * blockIdx.y + threadIdx.y;
	int col = blockDim.x * blockIdx.x + threadIdx.x;
	int id = width * row + col;
	int SMW = blockDim.x + 4;//2*2



	int r, c, kernel_idx = 0;
	int2 input_idx;
	int idx = 0;
	int loc_column=threadIdx.x,loc_row=threadIdx.y;// index
	char temp = 0;

	int side_left=0, side_right = 0;
	

	SHARED_MEM(loc_column+2, loc_row+2 ) = d_bitmaps[id];


	if (blockIdx.x == 0 || blockIdx.x == gridDim.x - 1 || blockIdx.y == 0 || blockIdx.y == gridDim.y - 1) {
		if (loc_column < 2) {
			input_idx.x = (col - 2 < 0) ? 0 : col - 2;
			SHARED_MEM(loc_column, loc_row + 2) = d_bitmaps[row*width+input_idx.x];
			side_left = 1;
		}
		else if (loc_column >= blockDim.x - 2) {
			input_idx.x = (col + 2 >= width) ? width - 1 : col + 2;
			SHARED_MEM(loc_column + 2 * 2, loc_row + 2) = d_bitmaps[row * width + input_idx.x]; 
			side_right = 1;
		}

		if (loc_row < 2) {
			input_idx.y = (row - 2 < 0) ? 0 : row - 2;
			SHARED_MEM(loc_column + 2, loc_row) = d_bitmaps[input_idx.y * width + col]; 
			if (side_left == 1) {
				input_idx.x = (col - 2 < 0) ? 0 : col - 2;
				input_idx.y = (row - 2 < 0) ? 0 : row - 2;
				SHARED_MEM(loc_column, loc_row) = d_bitmaps[input_idx.y * width + input_idx.x]; 
			}
			if (side_right == 1) {
				input_idx.x = (col + 2 >= width) ? width - 1 : col + 2;
				input_idx.y = (row - 2 < 0) ? 0 : row - 2;
				SHARED_MEM(loc_column + 2 * 2, loc_row) = d_bitmaps[input_idx.y * width + input_idx.x]; 
			}
		}
		else if (loc_row >= blockDim.y - 2) {
			input_idx.y = (row + 2 >= height) ? height - 1 : row + 2;
			SHARED_MEM(loc_column + 2, loc_row + 2 * 2) = d_bitmaps[input_idx.y * width + col];
			if (side_left == 1) {
				input_idx.x = (col - 2 < 0) ? 0 : col - 2;
				input_idx.y = (row + 2 >= height) ? height - 1 : row + 2;
				SHARED_MEM(loc_column, loc_row + 2 * 2) = d_bitmaps[input_idx.y * width + input_idx.x]; 
			}
			if (side_right == 1) {
				input_idx.x = (col + 2 >= width) ? width - 1 : col + 2;
				input_idx.y = (row + 2 >= height) ? height - 1 : row + 2;
				SHARED_MEM(loc_column + 2 * 2, loc_row + 2 * 2) = d_bitmaps[input_idx.y * width + input_idx.x]; 
			}
		}
	}
	else {
		if (loc_column < 2) {
			SHARED_MEM(loc_column, loc_row + 2) = d_bitmaps[row * width + col - 2]; 
			side_left = 1;
		}
		else if (loc_column >= blockDim.x - 2) {
			SHARED_MEM(loc_column + 2 * 2, loc_row + 2) = d_bitmaps[row * width + col + 2]; 
			side_right = 1;
		}

		if (loc_row < 2) {
			SHARED_MEM(loc_column + 2, loc_row) = d_bitmaps[row * width + col - 2];
			if (side_left == 1)
				SHARED_MEM(loc_column, loc_row) = d_bitmaps[(row-2) * width + col - 2]; 
			if (side_right == 1)
				SHARED_MEM(loc_column + 2 * 2, loc_row) = d_bitmaps[(row - 2) * width + col + 2]; 
		}
		else if (loc_row >= blockDim.y - 2) {
			SHARED_MEM(loc_column + 2, loc_row + 2 * 2) = d_bitmaps[(row + 2) * width + col ]; 
			if (side_left == 1)
				SHARED_MEM(loc_column, loc_row + 2 * 2) = d_bitmaps[(row + 2) * width + col - 2]; 
			if (side_right == 1)
				SHARED_MEM(loc_column + 2 * 2, loc_row + 2 * 2) = d_bitmaps[(row + 2) * width + col + 2]; 
		}
	}



	__syncthreads();

	for (r = loc_row - 2; r <= loc_row + 2; r++) {
		for (c = loc_column - 2; c <= loc_column + 2; c++) {
			temp+=SHARED_MEM(c + 2, r + 2)* constant_gaussian_kernel[kernel_idx++];
		}
	}
	d_Gaussian[id] = temp;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	
//	Constant variable �� gaussian kernel�� �����ϴ� �Լ�
//	�Ŀ� gaussian filtering ���� ����Ѵ�.
//	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Set_Gaussain_Kernel(){
	float _1 = 1.0f / 256.0f;
	float _4 = _1 * 4;
	float _6 = _1 * 6;
	float _16 = _1 * 16;
	float _24 = _1 * 24;
	float _36 = _1 * 36;

	float *p_gaussian_kernel = new float[25];

	p_gaussian_kernel[0] = p_gaussian_kernel[4] = p_gaussian_kernel[20] = p_gaussian_kernel[24] = _1;
	p_gaussian_kernel[1] = p_gaussian_kernel[3] = p_gaussian_kernel[5] = p_gaussian_kernel[9]= _4;
	p_gaussian_kernel[15] = p_gaussian_kernel[19] = p_gaussian_kernel[21] = p_gaussian_kernel[23] = _4;
	p_gaussian_kernel[2] = p_gaussian_kernel[10] = p_gaussian_kernel[14] = p_gaussian_kernel[22] = _6;
	p_gaussian_kernel[6] = p_gaussian_kernel[8] = p_gaussian_kernel[16] = p_gaussian_kernel[18] = _16;
	p_gaussian_kernel[7] = p_gaussian_kernel[11] =p_gaussian_kernel[13] = p_gaussian_kernel[17] = _24;
	p_gaussian_kernel[12] = _36;

	cudaMemcpyToSymbol( constant_gaussian_kernel, p_gaussian_kernel, sizeof( float ) * 25 );

	delete[] p_gaussian_kernel;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	
//	Ŀ���� �����ϱ� �� �ʿ��� �ڷ�� �غ� �� Ŀ���� ������ ����̽��� ����
//	Shared_flag �Է� �� NO_SHARED �� SHARED �� �� ���� ��ũ�θ� ������
//	flag���� �´� Ŀ���� ����
//	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float Do_Gaussian_on_GPU(IN unsigned char *p_bitmaps, OUT unsigned char *p_Gaussian, long width, long height, int Shared_flag)
{
	Set_Gaussain_Kernel();
	CUDA_CALL(cudaSetDevice(0));
	unsigned int total_pixel = width * height;

	unsigned char *d_bitmaps, *d_Gaussian;
	size_t mem_size;

	mem_size = width * height * sizeof(unsigned char);
	CUDA_CALL(cudaMalloc(&d_bitmaps, mem_size));

	CUDA_CALL(cudaMemcpy(d_bitmaps, p_bitmaps, mem_size, cudaMemcpyHostToDevice));

	CUDA_CALL(cudaMalloc(&d_Gaussian, mem_size));

	dim3 blockDim(BLOCK_SIZE_X, BLOCK_SIZE_Y);
	
	int gridy = height / blockDim.y;
	if (height % blockDim.y != 0)
		gridy++;
	dim3 gridDim(width / blockDim.x, gridy);
	CHECK_TIME_INIT_GPU();
	CHECK_TIME_START_GPU();

	switch (Shared_flag)
	{
	case NO_SHARED:
		Gaussian_kernel_no_shared << <gridDim, blockDim >> > (d_bitmaps, d_Gaussian, width, height);
		break;
	case SHARED:
		Gaussian_kernel_shared << < gridDim, blockDim, sizeof(unsigned char) * (blockDim.x + 2 * Window) * (blockDim.y + 2 * Window)>> > (d_bitmaps, d_Gaussian, width, height);
		break;
	}

	CUDA_CALL(cudaDeviceSynchronize());
	CHECK_TIME_END_GPU(device_time);
	CHECK_TIME_DEST_GPU();

	CUDA_CALL(cudaMemcpy(p_Gaussian, d_Gaussian, mem_size, cudaMemcpyDeviceToHost));
	
	cudaFree(d_bitmaps);
	cudaFree(d_Gaussian);

	return device_time;
}