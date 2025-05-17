/*
 * main.c - 图像二次曝光处理主程序
 *
 * 作者：[彭靖楠（Nanwan）]
 * 日期：2025-5-17
 *
 * 描述：本程序实现了图像二次曝光处理功能，支持三种实现方式：
 *      1. C语言实现
 *      2. Intrinsic指令优化实现
 *      3. 线性汇编优化实现
 * 本程序为西南科技大学DSP课程设计项目，使用TI C6748 DSP处理器
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "ImageDoubleExposure.h"  // 图像二次曝光算法接口头文件
#include "bmp_head_struct.h"      // BMP文件格式结构定义

// 全局变量声明
image_dblexp_create_t dblexp_create;  // 算法创建参数结构
image_dblexp_frame_t dblexp_frame;    // 算法处理帧结构
image_dblexp_state_t dblexp_state;    // 算法状态结构

BITMAPFILEHEADER *p_bmpFileHead;      // BMP文件头指针 (14字节)
BITMAPINFOHEADER *p_bmpInfoHead;      // BMP信息头指针 (40字节)
void *handle;                         // 算法实例句柄

/**
 * 处理单幅图像的函数
 *
 * @param process_mode - 处理模式: 0-C语言，1-Intrinsic，2-线性汇编
 */
void process_single_image(int process_mode) {
    int SizeImage;                    // 图像数据大小(字节)
    int width, height;                // 图像宽度和高度
    FILE *file_src1 = NULL;           // 第一幅源图像文件指针
    FILE *file_src2 = NULL;           // 第二幅源图像文件指针
    FILE *file_dst = NULL;            // 目标图像文件指针
    BYTE *p_src1 = NULL;              // 第一幅源图像数据指针
    BYTE *p_src2 = NULL;              // 第二幅源图像数据指针
    BYTE *p_dst = NULL;               // 目标图像数据指针
    clock_t begin, end;               // 时钟计数变量，用于性能测量
    char output_filename[100];        // 输出文件名缓冲区

    printf("--- Image Double Exposure Processing (Mode %d) ---\n", process_mode);

    // 打开第一幅输入图像文件
    file_src1 = fopen("..//bmps//ImageA.bmp", "rb");
    if (!file_src1) {
        printf("file_src1 open error!\n");
        return;
    }

    // 打开第二幅输入图像文件
    file_src2 = fopen("..//bmps//ImageB.bmp", "rb");
    if (!file_src2) {
        printf("file_src2 open error!\n");
        fclose(file_src1);
        return;
    }

    // 创建输出文件名称并打开输出文件
    sprintf(output_filename, "..//bmps//Result_Mode%d.bmp", process_mode);
    file_dst = fopen(output_filename, "wb");
    if (!file_dst) {
        printf("file_dst open error!\n");
        fclose(file_src1);
        fclose(file_src2);
        return;
    }

    // 为BMP文件头和信息头分配内存
    p_bmpFileHead = (BITMAPFILEHEADER *)malloc(14);
    p_bmpInfoHead = (BITMAPINFOHEADER *)malloc(40);

    // 读取第一幅图像的BMP文件头和信息头
    fread(p_bmpFileHead, 1, 14, file_src1);
    fread(p_bmpInfoHead, 1, 40, file_src1);

    // 检查第二幅图像的尺寸是否与第一幅图像一致
    BITMAPFILEHEADER tempFileHead;
    BITMAPINFOHEADER tempInfoHead;
    fread(&tempFileHead, 1, 14, file_src2);
    fread(&tempInfoHead, 1, 40, file_src2);

    // 如果两幅图像尺寸不一致，报错并退出
    if (tempInfoHead.biWidth != p_bmpInfoHead->biWidth ||
        tempInfoHead.biHeight != p_bmpInfoHead->biHeight) {
        printf("Error: The two images must have the same dimensions!\n");
        fclose(file_src1);
        fclose(file_src2);
        fclose(file_dst);
        free(p_bmpFileHead);
        free(p_bmpInfoHead);
        return;
    }

    // 将第二幅图像文件指针重置到文件开头，以便后续读取像素数据
    fseek(file_src2, 0, SEEK_SET);
    fread(&tempFileHead, 1, 14, file_src2);
    fread(&tempInfoHead, 1, 40, file_src2);

    // 获取图像尺寸信息，计算图像数据大小
    width = p_bmpInfoHead->biWidth;
    height = p_bmpInfoHead->biHeight;
    SizeImage = width * height * 3;  // 3字节/像素 (RGB)

    // 为图像数据分配内存
    p_src1 = (BYTE *)malloc(SizeImage);
    p_src2 = (BYTE *)malloc(SizeImage);
    p_dst = (BYTE *)malloc(SizeImage);

    // 检查内存分配是否成功
    if ((!p_src1) || (!p_src2) || (!p_dst)) {
        printf("Error in malloc for image data!\n");
        // 释放已分配的内存
        if (p_src1) free(p_src1);
        if (p_src2) free(p_src2);
        if (p_dst) free(p_dst);
        free(p_bmpFileHead);
        free(p_bmpInfoHead);
        // 关闭已打开的文件
        fclose(file_src1);
        fclose(file_src2);
        fclose(file_dst);
        return;
    }

    // 步骤1: 创建算法实例 ------------------------------------
    dblexp_create.width = width;
    dblexp_create.height = height;
    begin = clock();  // 开始计时
    image_double_exposure(NULL, DSP_ALG_CREATE, &dblexp_create, NULL);
    end = clock();    // 结束计时
    printf("DSP_ALG_CREATE time = %d\n", end - begin);
    handle = dblexp_create.handle;

    // 读取两幅图像的像素数据
    fread(p_src1, 1, SizeImage, file_src1);
    fread(p_src2, 1, SizeImage, file_src2);

    // 步骤2: 执行图像二次曝光处理 -----------------------------
    printf("\n--- Image Double Exposure Begin! ---\n");
    // 设置处理参数
    dblexp_frame.formatImage = IMAGE_FORMAT_RGB;
    dblexp_frame.pInImage1 = p_src1;
    dblexp_frame.pInImage2 = p_src2;
    dblexp_frame.pOutData = p_dst;
    dblexp_frame.processMode = process_mode;  // 选择处理模式: 0-C, 1-Intrinsic, 2-ASM

    // 调用图像二次曝光算法
    image_double_exposure(handle, DSP_ALG_PROCESS, &dblexp_frame, &dblexp_state);

    // 输出处理时间（性能测量）
    printf("Processing Time: %d clock cycles\n", dblexp_state.processTime);
    printf("\n--- Image Double Exposure End! ---\n");

    // 步骤3: 销毁算法实例并释放资源 ---------------------------
    image_double_exposure(handle, DSP_ALG_DESTROY, NULL, NULL);

    // 将处理结果写入输出文件
    fwrite(p_bmpFileHead, 1, 14, file_dst);  // 写入文件头
    fwrite(p_bmpInfoHead, 1, 40, file_dst);  // 写入信息头
    fwrite(p_dst, 1, SizeImage, file_dst);   // 写入像素数据

    // 关闭所有文件
    fclose(file_src1);
    fclose(file_src2);
    fclose(file_dst);

    // 释放所有分配的内存
    free(p_bmpFileHead);
    free(p_bmpInfoHead);
    free(p_src1);
    free(p_src2);
    free(p_dst);
}

/**
 * 主函数
 *
 * 依次使用三种不同的实现方式处理图像，并比较性能
 */
int main(void) {
    printf("\n========= DSP 图像处理应用系统 - 图像二次曝光 =========\n");
    printf("实现方式：C语言、Intrinsic指令和线性汇编\n\n");

    // 使用C语言实现处理图像
    process_single_image(0);

    // 使用Intrinsic指令优化实现处理图像
    process_single_image(1);

    // 使用线性汇编优化实现处理图像
    process_single_image(2);

    printf("\n============ 处理完成，请查看结果图像文件 ============\n");

    return 0;
}
