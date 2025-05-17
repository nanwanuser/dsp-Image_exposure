/*
 * DoubleExposureCore.h - 图像二次曝光算法核心头文件
 *
 * 创建日期: 2025-5-17
 * 描述: 本文件定义了图像二次曝光处理的内部数据结构和核心函数
 */

#ifndef DOUBLEEXPOSURECORE_H_
#define DOUBLEEXPOSURECORE_H_

#include "ImageDoubleExposure.h"  /* 包含接口头文件 */

/*----------------------------------------------------------------------------
 * c674x_dblexp_t 结构体定义
 *
 * 此结构用于算法内部实现，存储处理过程中的图像数据和参数
 *--------------------------------------------------------------------------*/
typedef struct c674x_dblexp_t{
    /* 第一幅输入图像的各颜色通道平面数据 */
    unsigned char *pImgSrc1R;  /* 第一幅源图像R通道, 宽×高 */
    unsigned char *pImgSrc1G;  /* 第一幅源图像G通道, 宽×高 */
    unsigned char *pImgSrc1B;  /* 第一幅源图像B通道, 宽×高 */

    /* 第二幅输入图像的各颜色通道平面数据 */
    unsigned char *pImgSrc2R;  /* 第二幅源图像R通道, 宽×高 */
    unsigned char *pImgSrc2G;  /* 第二幅源图像G通道, 宽×高 */
    unsigned char *pImgSrc2B;  /* 第二幅源图像B通道, 宽×高 */

    /* 输出图像的各颜色通道平面数据 */
    unsigned char *pImgDstR;   /* 目标图像R通道, 宽×高 */
    unsigned char *pImgDstG;   /* 目标图像G通道, 宽×高 */
    unsigned char *pImgDstB;   /* 目标图像B通道, 宽×高 */

    /* 图像尺寸参数 */
    int width;                 /* 图像宽度（像素） */
    int height;                /* 图像高度（像素） */
} c674x_dblexp_t;

/*----------------------------------------------------------------------------
 * 描述: 处理输入图像并输出状态
 * 参数:
 *      -handle: 算法结构体 c674x_dblexp_t
 *      -param1: 帧结构体 image_dblexp_frame_t，包含输入输出图像数据和处理模式
 *      -param2: 状态结构体 image_dblexp_state_t，包含处理结果状态信息
 * 返回值:
 *       0: 成功; <0: 失败
 * --------------------------------------------------------------------------*/
int Nanwan_alg_process(c674x_dblexp_t *handle,
        image_dblexp_frame_t *param1,
        image_dblexp_state_t *param2);

/*----------------------------------------------------------------------------
 * 描述: 创建算法实例并分配内存
 * 参数:
 *      -param1: 创建结构体 image_dblexp_create_t，包含图像尺寸信息
 * 返回值:
 *       0: 成功; <0: 失败
 * --------------------------------------------------------------------------*/
int Nanwan_alg_create(image_dblexp_create_t *param1);

/*----------------------------------------------------------------------------
 * 描述: 销毁算法实例并释放内存
 * 参数:
 *      -handle: 算法结构体 c674x_dblexp_t
 * 返回值:
 *       0: 成功; <0: 失败
 * --------------------------------------------------------------------------*/
int Nanwan_alg_destroy(c674x_dblexp_t *handle);

#endif /* DOUBLEEXPOSURECORE_H_ */
