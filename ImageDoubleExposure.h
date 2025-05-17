/*
 * ImageDoubleExposure.h - 图像二次曝光算法接口头文件
 *
 * 创建日期: 2025-5-17
 * 描述: 本文件定义了图像二次曝光处理的API接口、数据结构和常量
 */

#ifndef IMAGEDOUBLEEXPOSURE_H_
#define IMAGEDOUBLEEXPOSURE_H_

/* 错误代码定义 */
#define C674X_ERR_MEMORY    -1      /* 内存分配错误 */
#define C674X_ERR_FAIL      -2      /* 一般性故障 */
#define C674X_ERR_FORMAT    -3      /* 文件格式错误 */

/* 图像格式定义 */
#define IMAGE_FORMAT_RGB  (1<<0)    /* RGB格式图像 */
#define IMAGE_FORMAT_YUV  (1<<1)    /* YUV格式图像 */

/*----------------------------------------------------------------------------
 * image_dblexp_create_t 结构体定义
 *
 * 此结构在创建算法实例时作为param1参数传递（操作代码DSP_ALG_CREATE）
 *--------------------------------------------------------------------------*/
typedef struct {
    int version;                 /* [输入] 算法版本号 */
    int width;                   /* [输入] 图像宽度，像素单位 */
    int height;                  /* [输入] 图像高度，像素单位 */

    void *handle;                /* [输出] 算法实例句柄 */
} image_dblexp_create_t;

/*----------------------------------------------------------------------------
 * image_dblexp_frame_t 结构体定义
 *
 * 此结构在处理图像时作为param1参数传递（操作代码DSP_ALG_PROCESS）
 *--------------------------------------------------------------------------*/
typedef struct {
    int formatImage;             /* [输入] 图像格式：RGB或YUV */
    unsigned char *pInImage1;    /* [输入] 第一幅输入图像数据指针（打包格式） */
    unsigned char *pInImage2;    /* [输入] 第二幅输入图像数据指针（打包格式） */
    unsigned char *pOutData;     /* [输出] 输出图像数据指针（打包格式） */
    int length;                  /* [输出] 结果数据长度 */
    int processMode;             /* [输入] 处理模式: 0-C语言, 1-Intrinsic, 2-线性汇编 */
} image_dblexp_frame_t;

/*----------------------------------------------------------------------------
 * image_dblexp_state_t 结构体定义
 *
 * 此结构在处理图像时作为param2参数传递（操作代码DSP_ALG_PROCESS）
 *--------------------------------------------------------------------------*/
typedef struct {
    int width;                  /* [输出] 图像宽度，像素单位 */
    int height;                 /* [输出] 图像高度，像素单位 */
    int processTime;            /* [输出] 处理时间（时钟周期数） */
    int processMode;            /* [输出] 使用的处理模式 */
} image_dblexp_state_t;

/* 操作代码定义 */
#define DSP_ALG_CREATE  0 /* 创建算法实例; 成功返回0 */
#define DSP_ALG_DESTROY 1 /* 销毁算法实例; 成功返回0 */
#define DSP_ALG_PROCESS 2 /* 执行图像二次曝光处理 */

/*----------------------------------------------------------------------------
 * 描述:   图像二次曝光算法接口函数
 * 参数:
 *      -handle: 算法实例句柄
 *      -opt   : 操作代码，可以是CREATE/DESTROY/PROCESS之一
 *      -param1: 参数1指针，根据操作代码不同指向不同结构
 *      -param2: 参数2指针，根据操作代码不同指向不同结构
 * 返回值:
 *      =0: 成功
 *      <0: 失败
 * --------------------------------------------------------------------------*/
int image_double_exposure(void *handle, int opt, void *param1, void *param2);


#endif /* IMAGEDOUBLEEXPOSURE_H_ */
