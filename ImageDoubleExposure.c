/*
 * ImageDoubleExposure.c - 图像二次曝光算法接口实现
 *
 * 创建日期: 2025-5-17
 * 描述: 本文件实现了图像二次曝光算法的接口函数
 */

#include "DoubleExposureCore.h"  /* 包含核心算法头文件 */

/**
 * image_double_exposure - 图像二次曝光算法接口函数
 *
 * 根据操作代码分别调用创建、处理或销毁函数
 *
 * @param handle: 算法实例句柄
 * @param opt: 操作代码（CREATE/DESTROY/PROCESS）
 * @param param1: 参数1指针，根据操作代码不同指向不同结构
 * @param param2: 参数2指针，根据操作代码不同指向不同结构
 * @return: =0表示成功，<0表示失败
 */
int image_double_exposure(void *handle, int opt, void *param1, void *param2)
{
    switch (opt) {
    case DSP_ALG_PROCESS:  /* 处理图像操作 */
        return Nanwan_alg_process(
                (c674x_dblexp_t *)handle,            /* 类型转换为内部算法结构 */
                (image_dblexp_frame_t *)param1,      /* 类型转换为处理帧结构 */
                (image_dblexp_state_t *)param2);     /* 类型转换为状态结构 */

    case DSP_ALG_CREATE:   /* 创建算法实例操作 */
        return Nanwan_alg_create((image_dblexp_create_t *)param1);  /* 类型转换为创建结构 */

    case DSP_ALG_DESTROY:  /* 销毁算法实例操作 */
        return Nanwan_alg_destroy((c674x_dblexp_t *) handle);  /* 类型转换为内部算法结构 */

    default:               /* 无效操作代码 */
        return C674X_ERR_FAIL;  /* 返回一般性错误 */
    }
}
