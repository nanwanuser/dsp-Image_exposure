/*
 * DoubleExposureCore.c
 *
 *  Created on: 2025-5-17
 */

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>

#include "DoubleExposureCore.h"
#include "RGB2RGB.h"

// 导入汇编函数声明
extern void process_pixels_dblexp_asm(
		unsigned char *SrcB1, unsigned char *SrcG1, unsigned char *SrcR1,
		unsigned char *SrcB2, unsigned char *SrcG2, unsigned char *SrcR2,
		unsigned char *DstB, unsigned char *DstG, unsigned char *DstR,
		int width);

/* C语言版本的处理函数 */
void process_pixels_c(c674x_dblexp_t *handle)
{
	c674x_dblexp_t *c674x_dblexp = handle;
	int i, j;
	int width = c674x_dblexp->width;
	int height = c674x_dblexp->height;

	for (i = 0; i < height; i++) {
		unsigned char *pSrc1RowB = c674x_dblexp->pImgSrc1B + i*width;
		unsigned char *pSrc1RowG = c674x_dblexp->pImgSrc1G + i*width;
		unsigned char *pSrc1RowR = c674x_dblexp->pImgSrc1R + i*width;

		unsigned char *pSrc2RowB = c674x_dblexp->pImgSrc2B + i*width;
		unsigned char *pSrc2RowG = c674x_dblexp->pImgSrc2G + i*width;
		unsigned char *pSrc2RowR = c674x_dblexp->pImgSrc2R + i*width;

		unsigned char *pDstRowB = c674x_dblexp->pImgDstB + i*width;
		unsigned char *pDstRowG = c674x_dblexp->pImgDstG + i*width;
		unsigned char *pDstRowR = c674x_dblexp->pImgDstR + i*width;

		for (j = 0; j < width; j++) {
			// 图像二次曝光：C=0.5×A+0.5×B
			pDstRowB[j] = (unsigned char)((pSrc1RowB[j] + pSrc2RowB[j]) / 2);
			pDstRowG[j] = (unsigned char)((pSrc1RowG[j] + pSrc2RowG[j]) / 2);
			pDstRowR[j] = (unsigned char)((pSrc1RowR[j] + pSrc2RowR[j]) / 2);
		}
	}
}

/* 使用Intrinsic指令优化的函数 */
void process_pixels_intrinsic(c674x_dblexp_t *handle)
{
	c674x_dblexp_t *c674x_dblexp = handle;
	int i, j;
	int width = c674x_dblexp->width;
	int height = c674x_dblexp->height;

	for (i = 0; i < height; i += 2) {
		unsigned char *restrict pSrc1RowB_0 = c674x_dblexp->pImgSrc1B + (i+0)*width;
		unsigned char *restrict pSrc1RowB_1 = c674x_dblexp->pImgSrc1B + (i+1)*width;
		unsigned char *restrict pSrc1RowG_0 = c674x_dblexp->pImgSrc1G + (i+0)*width;
		unsigned char *restrict pSrc1RowG_1 = c674x_dblexp->pImgSrc1G + (i+1)*width;
		unsigned char *restrict pSrc1RowR_0 = c674x_dblexp->pImgSrc1R + (i+0)*width;
		unsigned char *restrict pSrc1RowR_1 = c674x_dblexp->pImgSrc1R + (i+1)*width;

		unsigned char *restrict pSrc2RowB_0 = c674x_dblexp->pImgSrc2B + (i+0)*width;
		unsigned char *restrict pSrc2RowB_1 = c674x_dblexp->pImgSrc2B + (i+1)*width;
		unsigned char *restrict pSrc2RowG_0 = c674x_dblexp->pImgSrc2G + (i+0)*width;
		unsigned char *restrict pSrc2RowG_1 = c674x_dblexp->pImgSrc2G + (i+1)*width;
		unsigned char *restrict pSrc2RowR_0 = c674x_dblexp->pImgSrc2R + (i+0)*width;
		unsigned char *restrict pSrc2RowR_1 = c674x_dblexp->pImgSrc2R + (i+1)*width;

		unsigned char *restrict pDstRowB_0 = c674x_dblexp->pImgDstB + (i+0)*width;
		unsigned char *restrict pDstRowB_1 = c674x_dblexp->pImgDstB + (i+1)*width;
		unsigned char *restrict pDstRowG_0 = c674x_dblexp->pImgDstG + (i+0)*width;
		unsigned char *restrict pDstRowG_1 = c674x_dblexp->pImgDstG + (i+1)*width;
		unsigned char *restrict pDstRowR_0 = c674x_dblexp->pImgDstR + (i+0)*width;
		unsigned char *restrict pDstRowR_1 = c674x_dblexp->pImgDstR + (i+1)*width;

		#pragma MUST_ITERATE(192,192,192) // 图像宽度为192=768/4
		for (j = 0; j < width; j += 4) {
			// 读取4个像素点数据
			unsigned int src1B_0_3210 = _amem4_const(&pSrc1RowB_0[j]);
			unsigned int src1B_1_3210 = _amem4_const(&pSrc1RowB_1[j]);
			unsigned int src1G_0_3210 = _amem4_const(&pSrc1RowG_0[j]);
			unsigned int src1G_1_3210 = _amem4_const(&pSrc1RowG_1[j]);
			unsigned int src1R_0_3210 = _amem4_const(&pSrc1RowR_0[j]);
			unsigned int src1R_1_3210 = _amem4_const(&pSrc1RowR_1[j]);

			unsigned int src2B_0_3210 = _amem4_const(&pSrc2RowB_0[j]);
			unsigned int src2B_1_3210 = _amem4_const(&pSrc2RowB_1[j]);
			unsigned int src2G_0_3210 = _amem4_const(&pSrc2RowG_0[j]);
			unsigned int src2G_1_3210 = _amem4_const(&pSrc2RowG_1[j]);
			unsigned int src2R_0_3210 = _amem4_const(&pSrc2RowR_0[j]);
			unsigned int src2R_1_3210 = _amem4_const(&pSrc2RowR_1[j]);

			// 计算平均值 (a+b)/2 = (a+b)>>1
			// 使用标准数学计算替代_avg4
			unsigned int B_0_3210 = ((src1B_0_3210 & 0xFEFEFEFE) >> 1) + ((src2B_0_3210 & 0xFEFEFEFE) >> 1);
			unsigned int B_1_3210 = ((src1B_1_3210 & 0xFEFEFEFE) >> 1) + ((src2B_1_3210 & 0xFEFEFEFE) >> 1);
			unsigned int G_0_3210 = ((src1G_0_3210 & 0xFEFEFEFE) >> 1) + ((src2G_0_3210 & 0xFEFEFEFE) >> 1);
			unsigned int G_1_3210 = ((src1G_1_3210 & 0xFEFEFEFE) >> 1) + ((src2G_1_3210 & 0xFEFEFEFE) >> 1);
			unsigned int R_0_3210 = ((src1R_0_3210 & 0xFEFEFEFE) >> 1) + ((src2R_0_3210 & 0xFEFEFEFE) >> 1);
			unsigned int R_1_3210 = ((src1R_1_3210 & 0xFEFEFEFE) >> 1) + ((src2R_1_3210 & 0xFEFEFEFE) >> 1);

			// 写入4个像素点数据
			_amem4(&pDstRowB_0[j]) = B_0_3210;
			_amem4(&pDstRowB_1[j]) = B_1_3210;
			_amem4(&pDstRowG_0[j]) = G_0_3210;
			_amem4(&pDstRowG_1[j]) = G_1_3210;
			_amem4(&pDstRowR_0[j]) = R_0_3210;
			_amem4(&pDstRowR_1[j]) = R_1_3210;
		}
	}
}

/* 使用线性汇编优化的函数 */
void process_pixels_linear(c674x_dblexp_t *handle)
{
	c674x_dblexp_t *c674x_dblexp = handle;
	int width = c674x_dblexp->width;
	int height = c674x_dblexp->height;
	int i;
	unsigned char *restrict pSrc1RowB_0;
	unsigned char *restrict pSrc1RowG_0;
	unsigned char *restrict pSrc1RowR_0;
	unsigned char *restrict pSrc2RowB_0;
	unsigned char *restrict pSrc2RowG_0;
	unsigned char *restrict pSrc2RowR_0;
	unsigned char *restrict pDstRowB_0;
	unsigned char *restrict pDstRowG_0;
	unsigned char *restrict pDstRowR_0;

	for (i = 0; i < height; i += 2) {
		pSrc1RowB_0 = c674x_dblexp->pImgSrc1B + (i+0)*width;
		pSrc1RowG_0 = c674x_dblexp->pImgSrc1G + (i+0)*width;
		pSrc1RowR_0 = c674x_dblexp->pImgSrc1R + (i+0)*width;

		pSrc2RowB_0 = c674x_dblexp->pImgSrc2B + (i+0)*width;
		pSrc2RowG_0 = c674x_dblexp->pImgSrc2G + (i+0)*width;
		pSrc2RowR_0 = c674x_dblexp->pImgSrc2R + (i+0)*width;

		pDstRowB_0 = c674x_dblexp->pImgDstB + (i+0)*width;
		pDstRowG_0 = c674x_dblexp->pImgDstG + (i+0)*width;
		pDstRowR_0 = c674x_dblexp->pImgDstR + (i+0)*width;

		// 调用线性汇编函数，已修改为只接收10个参数
		process_pixels_dblexp_asm(
			pSrc1RowB_0, pSrc1RowG_0, pSrc1RowR_0,
			pSrc2RowB_0, pSrc2RowG_0, pSrc2RowR_0,
			pDstRowB_0, pDstRowG_0, pDstRowR_0,
			width);
	}
}

int ljz_alg_process(c674x_dblexp_t *handle,
				image_dblexp_frame_t *param1,
				image_dblexp_state_t *param2)
{
	c674x_dblexp_t *c674x_dblexp = handle;
	int width = c674x_dblexp->width;
	int height = c674x_dblexp->height;
	clock_t begin, end;
	int process_mode = param1->processMode;

	/*1. Convert packed BGR to planar B, G and R for image 1 */
	pkRGB2plRGB(param1->pInImage1,
			c674x_dblexp->pImgSrc1B, c674x_dblexp->pImgSrc1G, c674x_dblexp->pImgSrc1R,
			width, height);

	/*2. Convert packed BGR to planar B, G and R for image 2 */
	pkRGB2plRGB(param1->pInImage2,
			c674x_dblexp->pImgSrc2B, c674x_dblexp->pImgSrc2G, c674x_dblexp->pImgSrc2R,
			width, height);

	begin = clock();
	/*3. Process the pixels - double exposure */
	switch (process_mode) {
		case 0:  // C 语言版本
			process_pixels_c(c674x_dblexp);
			break;
		case 1:  // Intrinsic 指令优化
			process_pixels_intrinsic(c674x_dblexp);
			break;
		case 2:  // 线性汇编优化
			process_pixels_linear(c674x_dblexp);
			break;
		default:
			process_pixels_c(c674x_dblexp);
			break;
	}
	end = clock();
	printf("Double exposure processing time = %d clock cycles (Mode %d)\n",
		end - begin, process_mode);

	/*4. Convert planar B, G and R to packed BGR for output */
	plRGB2pkRGB(param1->pOutData,
			c674x_dblexp->pImgDstB, c674x_dblexp->pImgDstG, c674x_dblexp->pImgDstR,
			width, height);

	param2->width = width;
	param2->height = height;
	param2->processTime = end - begin;
	param2->processMode = process_mode;

	return 0;
}


/*
 * 创建算法实例，根据宽和高申请内存
 */
int ljz_alg_create(image_dblexp_create_t *param1)
{
	c674x_dblexp_t *c674x_dblexp;
	int width = param1->width;
	int height = param1->height;

	c674x_dblexp = (c674x_dblexp_t *)malloc(sizeof(c674x_dblexp_t));

	if (c674x_dblexp == NULL)
		goto alloc_memory_err;

	memset(c674x_dblexp, 0, sizeof(c674x_dblexp_t));

	// Allocate memory for source image 1
	c674x_dblexp->pImgSrc1B = (unsigned char *)malloc(width*height);
	c674x_dblexp->pImgSrc1G = (unsigned char *)malloc(width*height);
	c674x_dblexp->pImgSrc1R = (unsigned char *)malloc(width*height);
	if (c674x_dblexp->pImgSrc1B == NULL ||
		c674x_dblexp->pImgSrc1G == NULL ||
		c674x_dblexp->pImgSrc1R == NULL)
		goto alloc_memory_err;

	// Allocate memory for source image 2
	c674x_dblexp->pImgSrc2B = (unsigned char *)malloc(width*height);
	c674x_dblexp->pImgSrc2G = (unsigned char *)malloc(width*height);
	c674x_dblexp->pImgSrc2R = (unsigned char *)malloc(width*height);
	if (c674x_dblexp->pImgSrc2B == NULL ||
		c674x_dblexp->pImgSrc2G == NULL ||
		c674x_dblexp->pImgSrc2R == NULL)
		goto alloc_memory_err;

	// Allocate memory for destination image
	c674x_dblexp->pImgDstB = (unsigned char *)malloc(width*height);
	c674x_dblexp->pImgDstG = (unsigned char *)malloc(width*height);
	c674x_dblexp->pImgDstR = (unsigned char *)malloc(width*height);
	if (c674x_dblexp->pImgDstB == NULL ||
		c674x_dblexp->pImgDstG == NULL ||
		c674x_dblexp->pImgDstR == NULL)
		goto alloc_memory_err;

	c674x_dblexp->width = width;
	c674x_dblexp->height = height;

	param1->handle = c674x_dblexp;
	return 0;

alloc_memory_err:
	if (c674x_dblexp) {
		if (c674x_dblexp->pImgDstR) free(c674x_dblexp->pImgDstR);
		if (c674x_dblexp->pImgDstG) free(c674x_dblexp->pImgDstG);
		if (c674x_dblexp->pImgDstB) free(c674x_dblexp->pImgDstB);

		if (c674x_dblexp->pImgSrc2R) free(c674x_dblexp->pImgSrc2R);
		if (c674x_dblexp->pImgSrc2G) free(c674x_dblexp->pImgSrc2G);
		if (c674x_dblexp->pImgSrc2B) free(c674x_dblexp->pImgSrc2B);

		if (c674x_dblexp->pImgSrc1R) free(c674x_dblexp->pImgSrc1R);
		if (c674x_dblexp->pImgSrc1G) free(c674x_dblexp->pImgSrc1G);
		if (c674x_dblexp->pImgSrc1B) free(c674x_dblexp->pImgSrc1B);

		free(c674x_dblexp);
	}
	return C674X_ERR_MEMORY;
}

/*
 * 释放内存，销毁算法实例
 */
int ljz_alg_destroy(c674x_dblexp_t *handle)
{
	c674x_dblexp_t *c674x_dblexp = handle;

	free(c674x_dblexp->pImgDstR);
	free(c674x_dblexp->pImgDstG);
	free(c674x_dblexp->pImgDstB);

	free(c674x_dblexp->pImgSrc2R);
	free(c674x_dblexp->pImgSrc2G);
	free(c674x_dblexp->pImgSrc2B);

	free(c674x_dblexp->pImgSrc1R);
	free(c674x_dblexp->pImgSrc1G);
	free(c674x_dblexp->pImgSrc1B);

	free(c674x_dblexp);
	return 0;
}
