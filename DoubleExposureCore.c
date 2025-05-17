/*
 * DoubleExposureCore.c - 图像二次曝光算法核心实现
 *
 * 创建日期: 2025-5-17
 * 描述: 本文件实现了图像二次曝光算法的核心功能，包括三种实现方式：
 *       1. C语言实现
 *       2. Intrinsic指令优化实现
 *       3. 线性汇编优化实现
 */

#include <stdlib.h>   /* 标准库函数 */
#include <string.h>   /* 字符串处理函数 */
#include <time.h>     /* 时钟函数 */
#include <stdio.h>    /* 标准输入输出 */

#include "DoubleExposureCore.h"  /* 核心算法头文件 */
#include "RGB2RGB.h"             /* RGB格式转换函数 */

/**
 * 导入汇编函数声明
 * 用于线性汇编优化实现的外部函数
 */
extern void process_pixels_dblexp_asm(
        unsigned char *SrcB1, unsigned char *SrcG1, unsigned char *SrcR1,
        unsigned char *SrcB2, unsigned char *SrcG2, unsigned char *SrcR2,
        unsigned char *DstB, unsigned char *DstG, unsigned char *DstR,
        int width);

/**
 * process_pixels_c - C语言版本实现的图像二次曝光处理函数
 *
 * 使用标准C语言逐像素处理实现两幅图像按0.5:0.5权重合成
 *
 * @param handle: 算法实例结构体指针
 */
void process_pixels_c(c674x_dblexp_t *handle)
{
    c674x_dblexp_t *c674x_dblexp = handle;
    int i, j;
    int width = c674x_dblexp->width;
    int height = c674x_dblexp->height;

    /* 逐行处理图像 */
    for (i = 0; i < height; i++) {
        /* 获取当前行的指针 */
        unsigned char *pSrc1RowB = c674x_dblexp->pImgSrc1B + i*width;  /* 第一幅图像B通道当前行 */
        unsigned char *pSrc1RowG = c674x_dblexp->pImgSrc1G + i*width;  /* 第一幅图像G通道当前行 */
        unsigned char *pSrc1RowR = c674x_dblexp->pImgSrc1R + i*width;  /* 第一幅图像R通道当前行 */

        unsigned char *pSrc2RowB = c674x_dblexp->pImgSrc2B + i*width;  /* 第二幅图像B通道当前行 */
        unsigned char *pSrc2RowG = c674x_dblexp->pImgSrc2G + i*width;  /* 第二幅图像G通道当前行 */
        unsigned char *pSrc2RowR = c674x_dblexp->pImgSrc2R + i*width;  /* 第二幅图像R通道当前行 */

        unsigned char *pDstRowB = c674x_dblexp->pImgDstB + i*width;    /* 目标图像B通道当前行 */
        unsigned char *pDstRowG = c674x_dblexp->pImgDstG + i*width;    /* 目标图像G通道当前行 */
        unsigned char *pDstRowR = c674x_dblexp->pImgDstR + i*width;    /* 目标图像R通道当前行 */

        /* 逐像素处理 */
        for (j = 0; j < width; j++) {
            /* 图像二次曝光：C=0.5×A+0.5×B，通过求平均值实现 */
            pDstRowB[j] = (unsigned char)((pSrc1RowB[j] + pSrc2RowB[j]) / 2);  /* B通道平均值 */
            pDstRowG[j] = (unsigned char)((pSrc1RowG[j] + pSrc2RowG[j]) / 2);  /* G通道平均值 */
            pDstRowR[j] = (unsigned char)((pSrc1RowR[j] + pSrc2RowR[j]) / 2);  /* R通道平均值 */
        }
    }
}

/**
 * process_pixels_intrinsic - 使用Intrinsic指令优化的图像二次曝光处理函数
 *
 * 使用DSP Intrinsic指令进行优化，每次处理4个像素，并采用位运算优化平均值计算
 *
 * @param handle: 算法实例结构体指针
 */
void process_pixels_intrinsic(c674x_dblexp_t *handle)
{
    c674x_dblexp_t *c674x_dblexp = handle;
    int i, j;
    int width = c674x_dblexp->width;
    int height = c674x_dblexp->height;

    /* 每次处理两行数据 */
    for (i = 0; i < height; i += 2) {
        /* 获取当前行和下一行的指针，使用restrict关键字优化内存访问 */
        unsigned char *restrict pSrc1RowB_0 = c674x_dblexp->pImgSrc1B + (i+0)*width;  /* 第一幅图像B通道当前行 */
        unsigned char *restrict pSrc1RowB_1 = c674x_dblexp->pImgSrc1B + (i+1)*width;  /* 第一幅图像B通道下一行 */
        unsigned char *restrict pSrc1RowG_0 = c674x_dblexp->pImgSrc1G + (i+0)*width;  /* 第一幅图像G通道当前行 */
        unsigned char *restrict pSrc1RowG_1 = c674x_dblexp->pImgSrc1G + (i+1)*width;  /* 第一幅图像G通道下一行 */
        unsigned char *restrict pSrc1RowR_0 = c674x_dblexp->pImgSrc1R + (i+0)*width;  /* 第一幅图像R通道当前行 */
        unsigned char *restrict pSrc1RowR_1 = c674x_dblexp->pImgSrc1R + (i+1)*width;  /* 第一幅图像R通道下一行 */

        unsigned char *restrict pSrc2RowB_0 = c674x_dblexp->pImgSrc2B + (i+0)*width;  /* 第二幅图像B通道当前行 */
        unsigned char *restrict pSrc2RowB_1 = c674x_dblexp->pImgSrc2B + (i+1)*width;  /* 第二幅图像B通道下一行 */
        unsigned char *restrict pSrc2RowG_0 = c674x_dblexp->pImgSrc2G + (i+0)*width;  /* 第二幅图像G通道当前行 */
        unsigned char *restrict pSrc2RowG_1 = c674x_dblexp->pImgSrc2G + (i+1)*width;  /* 第二幅图像G通道下一行 */
        unsigned char *restrict pSrc2RowR_0 = c674x_dblexp->pImgSrc2R + (i+0)*width;  /* 第二幅图像R通道当前行 */
        unsigned char *restrict pSrc2RowR_1 = c674x_dblexp->pImgSrc2R + (i+1)*width;  /* 第二幅图像R通道下一行 */

        unsigned char *restrict pDstRowB_0 = c674x_dblexp->pImgDstB + (i+0)*width;    /* 目标图像B通道当前行 */
        unsigned char *restrict pDstRowB_1 = c674x_dblexp->pImgDstB + (i+1)*width;    /* 目标图像B通道下一行 */
        unsigned char *restrict pDstRowG_0 = c674x_dblexp->pImgDstG + (i+0)*width;    /* 目标图像G通道当前行 */
        unsigned char *restrict pDstRowG_1 = c674x_dblexp->pImgDstG + (i+1)*width;    /* 目标图像G通道下一行 */
        unsigned char *restrict pDstRowR_0 = c674x_dblexp->pImgDstR + (i+0)*width;    /* 目标图像R通道当前行 */
        unsigned char *restrict pDstRowR_1 = c674x_dblexp->pImgDstR + (i+1)*width;    /* 目标图像R通道下一行 */

        /* 强制循环展开，每次跳过4个像素，优化循环执行效率 */
        #pragma MUST_ITERATE(192,192,192) /* 图像宽度为192=768/4 */
        for (j = 0; j < width; j += 4) {
            /* 读取4个像素点数据，使用_amem4_const Intrinsic函数优化内存读取 */
            unsigned int src1B_0_3210 = _amem4_const(&pSrc1RowB_0[j]);  /* 第一幅图像B通道当前行4个像素 */
            unsigned int src1B_1_3210 = _amem4_const(&pSrc1RowB_1[j]);  /* 第一幅图像B通道下一行4个像素 */
            unsigned int src1G_0_3210 = _amem4_const(&pSrc1RowG_0[j]);  /* 第一幅图像G通道当前行4个像素 */
            unsigned int src1G_1_3210 = _amem4_const(&pSrc1RowG_1[j]);  /* 第一幅图像G通道下一行4个像素 */
            unsigned int src1R_0_3210 = _amem4_const(&pSrc1RowR_0[j]);  /* 第一幅图像R通道当前行4个像素 */
            unsigned int src1R_1_3210 = _amem4_const(&pSrc1RowR_1[j]);  /* 第一幅图像R通道下一行4个像素 */

            unsigned int src2B_0_3210 = _amem4_const(&pSrc2RowB_0[j]);  /* 第二幅图像B通道当前行4个像素 */
            unsigned int src2B_1_3210 = _amem4_const(&pSrc2RowB_1[j]);  /* 第二幅图像B通道下一行4个像素 */
            unsigned int src2G_0_3210 = _amem4_const(&pSrc2RowG_0[j]);  /* 第二幅图像G通道当前行4个像素 */
            unsigned int src2G_1_3210 = _amem4_const(&pSrc2RowG_1[j]);  /* 第二幅图像G通道下一行4个像素 */
            unsigned int src2R_0_3210 = _amem4_const(&pSrc2RowR_0[j]);  /* 第二幅图像R通道当前行4个像素 */
            unsigned int src2R_1_3210 = _amem4_const(&pSrc2RowR_1[j]);  /* 第二幅图像R通道下一行4个像素 */

            /* 计算平均值 (a+b)/2 = (a+b)>>1，使用标准数学计算替代_avg4 */
            /* 掩码0xFEFEFEFE确保不会有进位影响 */
            unsigned int B_0_3210 = ((src1B_0_3210 & 0xFEFEFEFE) >> 1) + ((src2B_0_3210 & 0xFEFEFEFE) >> 1);
            unsigned int B_1_3210 = ((src1B_1_3210 & 0xFEFEFEFE) >> 1) + ((src2B_1_3210 & 0xFEFEFEFE) >> 1);
            unsigned int G_0_3210 = ((src1G_0_3210 & 0xFEFEFEFE) >> 1) + ((src2G_0_3210 & 0xFEFEFEFE) >> 1);
            unsigned int G_1_3210 = ((src1G_1_3210 & 0xFEFEFEFE) >> 1) + ((src2G_1_3210 & 0xFEFEFEFE) >> 1);
            unsigned int R_0_3210 = ((src1R_0_3210 & 0xFEFEFEFE) >> 1) + ((src2R_0_3210 & 0xFEFEFEFE) >> 1);
            unsigned int R_1_3210 = ((src1R_1_3210 & 0xFEFEFEFE) >> 1) + ((src2R_1_3210 & 0xFEFEFEFE) >> 1);

            /* 写入4个像素点数据，使用_amem4 Intrinsic函数优化内存写入 */
            _amem4(&pDstRowB_0[j]) = B_0_3210;  /* 目标图像B通道当前行4个像素 */
            _amem4(&pDstRowB_1[j]) = B_1_3210;  /* 目标图像B通道下一行4个像素 */
            _amem4(&pDstRowG_0[j]) = G_0_3210;  /* 目标图像G通道当前行4个像素 */
            _amem4(&pDstRowG_1[j]) = G_1_3210;  /* 目标图像G通道下一行4个像素 */
            _amem4(&pDstRowR_0[j]) = R_0_3210;  /* 目标图像R通道当前行4个像素 */
            _amem4(&pDstRowR_1[j]) = R_1_3210;  /* 目标图像R通道下一行4个像素 */
        }
    }
}

/**
 * process_pixels_linear - 使用线性汇编优化的图像二次曝光处理函数
 *
 * 调用外部线性汇编函数进行处理，实现最高性能优化
 *
 * @param handle: 算法实例结构体指针
 */
void process_pixels_linear(c674x_dblexp_t *handle)
{
    c674x_dblexp_t *c674x_dblexp = handle;
    int width = c674x_dblexp->width;
    int height = c674x_dblexp->height;
    int i;

    /* 声明行指针变量 */
    unsigned char *restrict pSrc1RowB_0;
    unsigned char *restrict pSrc1RowG_0;
    unsigned char *restrict pSrc1RowR_0;
    unsigned char *restrict pSrc2RowB_0;
    unsigned char *restrict pSrc2RowG_0;
    unsigned char *restrict pSrc2RowR_0;
    unsigned char *restrict pDstRowB_0;
    unsigned char *restrict pDstRowG_0;
    unsigned char *restrict pDstRowR_0;

    /* 每次处理两行数据 */
    for (i = 0; i < height; i += 2) {
        /* 获取当前行的指针 */
        pSrc1RowB_0 = c674x_dblexp->pImgSrc1B + (i+0)*width;  /* 第一幅图像B通道当前行 */
        pSrc1RowG_0 = c674x_dblexp->pImgSrc1G + (i+0)*width;  /* 第一幅图像G通道当前行 */
        pSrc1RowR_0 = c674x_dblexp->pImgSrc1R + (i+0)*width;  /* 第一幅图像R通道当前行 */

        pSrc2RowB_0 = c674x_dblexp->pImgSrc2B + (i+0)*width;  /* 第二幅图像B通道当前行 */
        pSrc2RowG_0 = c674x_dblexp->pImgSrc2G + (i+0)*width;  /* 第二幅图像G通道当前行 */
        pSrc2RowR_0 = c674x_dblexp->pImgSrc2R + (i+0)*width;  /* 第二幅图像R通道当前行 */

        pDstRowB_0 = c674x_dblexp->pImgDstB + (i+0)*width;    /* 目标图像B通道当前行 */
        pDstRowG_0 = c674x_dblexp->pImgDstG + (i+0)*width;    /* 目标图像G通道当前行 */
        pDstRowR_0 = c674x_dblexp->pImgDstR + (i+0)*width;    /* 目标图像R通道当前行 */

        /* 调用线性汇编函数，只接收10个参数 */
        process_pixels_dblexp_asm(
            pSrc1RowB_0, pSrc1RowG_0, pSrc1RowR_0,  /* 第一幅图像RGB通道 */
            pSrc2RowB_0, pSrc2RowG_0, pSrc2RowR_0,  /* 第二幅图像RGB通道 */
            pDstRowB_0, pDstRowG_0, pDstRowR_0,     /* 目标图像RGB通道 */
            width);                                  /* 图像宽度 */
    }
}

/**
 * Nanwan_alg_process - 处理输入图像并输出结果状态
 *
 * 实现图像二次曝光核心处理流程，根据处理模式选择不同的实现方式
 *
 * @param handle: 算法结构体
 * @param param1: 帧结构体，包含输入输出图像数据
 * @param param2: 状态结构体，返回处理结果状态
 * @return: 0表示成功，<0表示失败
 */
int Nanwan_alg_process(c674x_dblexp_t *handle,
                image_dblexp_frame_t *param1,
                image_dblexp_state_t *param2)
{
    c674x_dblexp_t *c674x_dblexp = handle;
    int width = c674x_dblexp->width;
    int height = c674x_dblexp->height;
    clock_t begin, end;             /* 时钟计数变量，用于性能测量 */
    int process_mode = param1->processMode;  /* 获取处理模式 */

    /*1. 将第一幅图像的打包BGR格式转换为平面B、G、R格式 */
    pkRGB2plRGB(param1->pInImage1,
            c674x_dblexp->pImgSrc1B, c674x_dblexp->pImgSrc1G, c674x_dblexp->pImgSrc1R,
            width, height);

    /*2. 将第二幅图像的打包BGR格式转换为平面B、G、R格式 */
    pkRGB2plRGB(param1->pInImage2,
            c674x_dblexp->pImgSrc2B, c674x_dblexp->pImgSrc2G, c674x_dblexp->pImgSrc2R,
            width, height);

    /* 开始计时 */
    begin = clock();

    /*3. 根据处理模式选择不同实现方式进行图像二次曝光处理 */
    switch (process_mode) {
        case 0:  /* C语言版本 */
            process_pixels_c(c674x_dblexp);
            break;
        case 1:  /* Intrinsic指令优化 */
            process_pixels_intrinsic(c674x_dblexp);
            break;
        case 2:  /* 线性汇编优化 */
            process_pixels_linear(c674x_dblexp);
            break;
        default:  /* 默认使用C语言版本 */
            process_pixels_c(c674x_dblexp);
            break;
    }

    /* 结束计时并计算处理时间 */
    end = clock();
    printf("Double exposure processing time = %d clock cycles (Mode %d)\n",
        end - begin, process_mode);

    /*4. 将平面B、G、R格式转换为打包BGR格式输出 */
    plRGB2pkRGB(param1->pOutData,
            c674x_dblexp->pImgDstB, c674x_dblexp->pImgDstG, c674x_dblexp->pImgDstR,
            width, height);

    /* 设置输出状态信息 */
    param2->width = width;
    param2->height = height;
    param2->processTime = end - begin;  /* 记录处理时间 */
    param2->processMode = process_mode; /* 记录处理模式 */

    return 0;  /* 返回成功 */
}

/**
 * Nanwan_alg_create - 创建算法实例，根据宽和高申请内存
 *
 * @param param1: 创建参数结构体，包含图像尺寸信息
 * @return: 0表示成功，<0表示失败
 */
int Nanwan_alg_create(image_dblexp_create_t *param1)
{
    c674x_dblexp_t *c674x_dblexp;
    int width = param1->width;    /* 获取图像宽度 */
    int height = param1->height;  /* 获取图像高度 */

    /* 为算法结构体分配内存 */
    c674x_dblexp = (c674x_dblexp_t *)malloc(sizeof(c674x_dblexp_t));
    if (c674x_dblexp == NULL)
        goto alloc_memory_err;

    /* 初始化算法结构体内存 */
    memset(c674x_dblexp, 0, sizeof(c674x_dblexp_t));

    /* 为第一幅源图像的RGB通道分配内存 */
    c674x_dblexp->pImgSrc1B = (unsigned char *)malloc(width*height);
    c674x_dblexp->pImgSrc1G = (unsigned char *)malloc(width*height);
    c674x_dblexp->pImgSrc1R = (unsigned char *)malloc(width*height);
    if (c674x_dblexp->pImgSrc1B == NULL ||
        c674x_dblexp->pImgSrc1G == NULL ||
        c674x_dblexp->pImgSrc1R == NULL)
        goto alloc_memory_err;

    /* 为第二幅源图像的RGB通道分配内存 */
    c674x_dblexp->pImgSrc2B = (unsigned char *)malloc(width*height);
    c674x_dblexp->pImgSrc2G = (unsigned char *)malloc(width*height);
    c674x_dblexp->pImgSrc2R = (unsigned char *)malloc(width*height);
    if (c674x_dblexp->pImgSrc2B == NULL ||
        c674x_dblexp->pImgSrc2G == NULL ||
        c674x_dblexp->pImgSrc2R == NULL)
        goto alloc_memory_err;

    /* 为目标图像的RGB通道分配内存 */
    c674x_dblexp->pImgDstB = (unsigned char *)malloc(width*height);
    c674x_dblexp->pImgDstG = (unsigned char *)malloc(width*height);
    c674x_dblexp->pImgDstR = (unsigned char *)malloc(width*height);
    if (c674x_dblexp->pImgDstB == NULL ||
        c674x_dblexp->pImgDstG == NULL ||
        c674x_dblexp->pImgDstR == NULL)
        goto alloc_memory_err;

    /* 设置图像尺寸 */
    c674x_dblexp->width = width;
    c674x_dblexp->height = height;

    /* 设置输出句柄并返回成功 */
    param1->handle = c674x_dblexp;
    return 0;

alloc_memory_err:
    /* 错误处理：释放已分配的内存 */
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
    return C674X_ERR_MEMORY;  /* 返回内存分配错误 */
}

/**
 * Nanwan_alg_destroy - 释放内存，销毁算法实例
 *
 * @param handle: 算法实例结构体指针
 * @return: 0表示成功，<0表示失败
 */
int Nanwan_alg_destroy(c674x_dblexp_t *handle)
{
    c674x_dblexp_t *c674x_dblexp = handle;

    /* 释放所有分配的内存 */
    free(c674x_dblexp->pImgDstR);  /* 释放目标图像R通道内存 */
    free(c674x_dblexp->pImgDstG);  /* 释放目标图像G通道内存 */
    free(c674x_dblexp->pImgDstB);  /* 释放目标图像B通道内存 */

    free(c674x_dblexp->pImgSrc2R);  /* 释放第二幅源图像R通道内存 */
    free(c674x_dblexp->pImgSrc2G);  /* 释放第二幅源图像G通道内存 */
    free(c674x_dblexp->pImgSrc2B);  /* 释放第二幅源图像B通道内存 */

    free(c674x_dblexp->pImgSrc1R);  /* 释放第一幅源图像R通道内存 */
    free(c674x_dblexp->pImgSrc1G);  /* 释放第一幅源图像G通道内存 */
    free(c674x_dblexp->pImgSrc1B);  /* 释放第一幅源图像B通道内存 */

    free(c674x_dblexp);  /* 释放算法结构体内存 */
    return 0;  /* 返回成功 */
}
