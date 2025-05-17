/*
 * DoubleExposureCore.c - ͼ������ع��㷨����ʵ��
 *
 * ��������: 2025-5-17
 * ����: ���ļ�ʵ����ͼ������ع��㷨�ĺ��Ĺ��ܣ���������ʵ�ַ�ʽ��
 *       1. C����ʵ��
 *       2. Intrinsicָ���Ż�ʵ��
 *       3. ���Ի���Ż�ʵ��
 */

#include <stdlib.h>   /* ��׼�⺯�� */
#include <string.h>   /* �ַ��������� */
#include <time.h>     /* ʱ�Ӻ��� */
#include <stdio.h>    /* ��׼������� */

#include "DoubleExposureCore.h"  /* �����㷨ͷ�ļ� */
#include "RGB2RGB.h"             /* RGB��ʽת������ */

/**
 * �����ຯ������
 * �������Ի���Ż�ʵ�ֵ��ⲿ����
 */
extern void process_pixels_dblexp_asm(
        unsigned char *SrcB1, unsigned char *SrcG1, unsigned char *SrcR1,
        unsigned char *SrcB2, unsigned char *SrcG2, unsigned char *SrcR2,
        unsigned char *DstB, unsigned char *DstG, unsigned char *DstR,
        int width);

/**
 * process_pixels_c - C���԰汾ʵ�ֵ�ͼ������ع⴦����
 *
 * ʹ�ñ�׼C���������ش���ʵ������ͼ��0.5:0.5Ȩ�غϳ�
 *
 * @param handle: �㷨ʵ���ṹ��ָ��
 */
void process_pixels_c(c674x_dblexp_t *handle)
{
    c674x_dblexp_t *c674x_dblexp = handle;
    int i, j;
    int width = c674x_dblexp->width;
    int height = c674x_dblexp->height;

    /* ���д���ͼ�� */
    for (i = 0; i < height; i++) {
        /* ��ȡ��ǰ�е�ָ�� */
        unsigned char *pSrc1RowB = c674x_dblexp->pImgSrc1B + i*width;  /* ��һ��ͼ��Bͨ����ǰ�� */
        unsigned char *pSrc1RowG = c674x_dblexp->pImgSrc1G + i*width;  /* ��һ��ͼ��Gͨ����ǰ�� */
        unsigned char *pSrc1RowR = c674x_dblexp->pImgSrc1R + i*width;  /* ��һ��ͼ��Rͨ����ǰ�� */

        unsigned char *pSrc2RowB = c674x_dblexp->pImgSrc2B + i*width;  /* �ڶ���ͼ��Bͨ����ǰ�� */
        unsigned char *pSrc2RowG = c674x_dblexp->pImgSrc2G + i*width;  /* �ڶ���ͼ��Gͨ����ǰ�� */
        unsigned char *pSrc2RowR = c674x_dblexp->pImgSrc2R + i*width;  /* �ڶ���ͼ��Rͨ����ǰ�� */

        unsigned char *pDstRowB = c674x_dblexp->pImgDstB + i*width;    /* Ŀ��ͼ��Bͨ����ǰ�� */
        unsigned char *pDstRowG = c674x_dblexp->pImgDstG + i*width;    /* Ŀ��ͼ��Gͨ����ǰ�� */
        unsigned char *pDstRowR = c674x_dblexp->pImgDstR + i*width;    /* Ŀ��ͼ��Rͨ����ǰ�� */

        /* �����ش��� */
        for (j = 0; j < width; j++) {
            /* ͼ������ع⣺C=0.5��A+0.5��B��ͨ����ƽ��ֵʵ�� */
            pDstRowB[j] = (unsigned char)((pSrc1RowB[j] + pSrc2RowB[j]) / 2);  /* Bͨ��ƽ��ֵ */
            pDstRowG[j] = (unsigned char)((pSrc1RowG[j] + pSrc2RowG[j]) / 2);  /* Gͨ��ƽ��ֵ */
            pDstRowR[j] = (unsigned char)((pSrc1RowR[j] + pSrc2RowR[j]) / 2);  /* Rͨ��ƽ��ֵ */
        }
    }
}

/**
 * process_pixels_intrinsic - ʹ��Intrinsicָ���Ż���ͼ������ع⴦����
 *
 * ʹ��DSP Intrinsicָ������Ż���ÿ�δ���4�����أ�������λ�����Ż�ƽ��ֵ����
 *
 * @param handle: �㷨ʵ���ṹ��ָ��
 */
void process_pixels_intrinsic(c674x_dblexp_t *handle)
{
    c674x_dblexp_t *c674x_dblexp = handle;
    int i, j;
    int width = c674x_dblexp->width;
    int height = c674x_dblexp->height;

    /* ÿ�δ����������� */
    for (i = 0; i < height; i += 2) {
        /* ��ȡ��ǰ�к���һ�е�ָ�룬ʹ��restrict�ؼ����Ż��ڴ���� */
        unsigned char *restrict pSrc1RowB_0 = c674x_dblexp->pImgSrc1B + (i+0)*width;  /* ��һ��ͼ��Bͨ����ǰ�� */
        unsigned char *restrict pSrc1RowB_1 = c674x_dblexp->pImgSrc1B + (i+1)*width;  /* ��һ��ͼ��Bͨ����һ�� */
        unsigned char *restrict pSrc1RowG_0 = c674x_dblexp->pImgSrc1G + (i+0)*width;  /* ��һ��ͼ��Gͨ����ǰ�� */
        unsigned char *restrict pSrc1RowG_1 = c674x_dblexp->pImgSrc1G + (i+1)*width;  /* ��һ��ͼ��Gͨ����һ�� */
        unsigned char *restrict pSrc1RowR_0 = c674x_dblexp->pImgSrc1R + (i+0)*width;  /* ��һ��ͼ��Rͨ����ǰ�� */
        unsigned char *restrict pSrc1RowR_1 = c674x_dblexp->pImgSrc1R + (i+1)*width;  /* ��һ��ͼ��Rͨ����һ�� */

        unsigned char *restrict pSrc2RowB_0 = c674x_dblexp->pImgSrc2B + (i+0)*width;  /* �ڶ���ͼ��Bͨ����ǰ�� */
        unsigned char *restrict pSrc2RowB_1 = c674x_dblexp->pImgSrc2B + (i+1)*width;  /* �ڶ���ͼ��Bͨ����һ�� */
        unsigned char *restrict pSrc2RowG_0 = c674x_dblexp->pImgSrc2G + (i+0)*width;  /* �ڶ���ͼ��Gͨ����ǰ�� */
        unsigned char *restrict pSrc2RowG_1 = c674x_dblexp->pImgSrc2G + (i+1)*width;  /* �ڶ���ͼ��Gͨ����һ�� */
        unsigned char *restrict pSrc2RowR_0 = c674x_dblexp->pImgSrc2R + (i+0)*width;  /* �ڶ���ͼ��Rͨ����ǰ�� */
        unsigned char *restrict pSrc2RowR_1 = c674x_dblexp->pImgSrc2R + (i+1)*width;  /* �ڶ���ͼ��Rͨ����һ�� */

        unsigned char *restrict pDstRowB_0 = c674x_dblexp->pImgDstB + (i+0)*width;    /* Ŀ��ͼ��Bͨ����ǰ�� */
        unsigned char *restrict pDstRowB_1 = c674x_dblexp->pImgDstB + (i+1)*width;    /* Ŀ��ͼ��Bͨ����һ�� */
        unsigned char *restrict pDstRowG_0 = c674x_dblexp->pImgDstG + (i+0)*width;    /* Ŀ��ͼ��Gͨ����ǰ�� */
        unsigned char *restrict pDstRowG_1 = c674x_dblexp->pImgDstG + (i+1)*width;    /* Ŀ��ͼ��Gͨ����һ�� */
        unsigned char *restrict pDstRowR_0 = c674x_dblexp->pImgDstR + (i+0)*width;    /* Ŀ��ͼ��Rͨ����ǰ�� */
        unsigned char *restrict pDstRowR_1 = c674x_dblexp->pImgDstR + (i+1)*width;    /* Ŀ��ͼ��Rͨ����һ�� */

        /* ǿ��ѭ��չ����ÿ������4�����أ��Ż�ѭ��ִ��Ч�� */
        #pragma MUST_ITERATE(192,192,192) /* ͼ����Ϊ192=768/4 */
        for (j = 0; j < width; j += 4) {
            /* ��ȡ4�����ص����ݣ�ʹ��_amem4_const Intrinsic�����Ż��ڴ��ȡ */
            unsigned int src1B_0_3210 = _amem4_const(&pSrc1RowB_0[j]);  /* ��һ��ͼ��Bͨ����ǰ��4������ */
            unsigned int src1B_1_3210 = _amem4_const(&pSrc1RowB_1[j]);  /* ��һ��ͼ��Bͨ����һ��4������ */
            unsigned int src1G_0_3210 = _amem4_const(&pSrc1RowG_0[j]);  /* ��һ��ͼ��Gͨ����ǰ��4������ */
            unsigned int src1G_1_3210 = _amem4_const(&pSrc1RowG_1[j]);  /* ��һ��ͼ��Gͨ����һ��4������ */
            unsigned int src1R_0_3210 = _amem4_const(&pSrc1RowR_0[j]);  /* ��һ��ͼ��Rͨ����ǰ��4������ */
            unsigned int src1R_1_3210 = _amem4_const(&pSrc1RowR_1[j]);  /* ��һ��ͼ��Rͨ����һ��4������ */

            unsigned int src2B_0_3210 = _amem4_const(&pSrc2RowB_0[j]);  /* �ڶ���ͼ��Bͨ����ǰ��4������ */
            unsigned int src2B_1_3210 = _amem4_const(&pSrc2RowB_1[j]);  /* �ڶ���ͼ��Bͨ����һ��4������ */
            unsigned int src2G_0_3210 = _amem4_const(&pSrc2RowG_0[j]);  /* �ڶ���ͼ��Gͨ����ǰ��4������ */
            unsigned int src2G_1_3210 = _amem4_const(&pSrc2RowG_1[j]);  /* �ڶ���ͼ��Gͨ����һ��4������ */
            unsigned int src2R_0_3210 = _amem4_const(&pSrc2RowR_0[j]);  /* �ڶ���ͼ��Rͨ����ǰ��4������ */
            unsigned int src2R_1_3210 = _amem4_const(&pSrc2RowR_1[j]);  /* �ڶ���ͼ��Rͨ����һ��4������ */

            /* ����ƽ��ֵ (a+b)/2 = (a+b)>>1��ʹ�ñ�׼��ѧ�������_avg4 */
            /* ����0xFEFEFEFEȷ�������н�λӰ�� */
            unsigned int B_0_3210 = ((src1B_0_3210 & 0xFEFEFEFE) >> 1) + ((src2B_0_3210 & 0xFEFEFEFE) >> 1);
            unsigned int B_1_3210 = ((src1B_1_3210 & 0xFEFEFEFE) >> 1) + ((src2B_1_3210 & 0xFEFEFEFE) >> 1);
            unsigned int G_0_3210 = ((src1G_0_3210 & 0xFEFEFEFE) >> 1) + ((src2G_0_3210 & 0xFEFEFEFE) >> 1);
            unsigned int G_1_3210 = ((src1G_1_3210 & 0xFEFEFEFE) >> 1) + ((src2G_1_3210 & 0xFEFEFEFE) >> 1);
            unsigned int R_0_3210 = ((src1R_0_3210 & 0xFEFEFEFE) >> 1) + ((src2R_0_3210 & 0xFEFEFEFE) >> 1);
            unsigned int R_1_3210 = ((src1R_1_3210 & 0xFEFEFEFE) >> 1) + ((src2R_1_3210 & 0xFEFEFEFE) >> 1);

            /* д��4�����ص����ݣ�ʹ��_amem4 Intrinsic�����Ż��ڴ�д�� */
            _amem4(&pDstRowB_0[j]) = B_0_3210;  /* Ŀ��ͼ��Bͨ����ǰ��4������ */
            _amem4(&pDstRowB_1[j]) = B_1_3210;  /* Ŀ��ͼ��Bͨ����һ��4������ */
            _amem4(&pDstRowG_0[j]) = G_0_3210;  /* Ŀ��ͼ��Gͨ����ǰ��4������ */
            _amem4(&pDstRowG_1[j]) = G_1_3210;  /* Ŀ��ͼ��Gͨ����һ��4������ */
            _amem4(&pDstRowR_0[j]) = R_0_3210;  /* Ŀ��ͼ��Rͨ����ǰ��4������ */
            _amem4(&pDstRowR_1[j]) = R_1_3210;  /* Ŀ��ͼ��Rͨ����һ��4������ */
        }
    }
}

/**
 * process_pixels_linear - ʹ�����Ի���Ż���ͼ������ع⴦����
 *
 * �����ⲿ���Ի�ຯ�����д���ʵ����������Ż�
 *
 * @param handle: �㷨ʵ���ṹ��ָ��
 */
void process_pixels_linear(c674x_dblexp_t *handle)
{
    c674x_dblexp_t *c674x_dblexp = handle;
    int width = c674x_dblexp->width;
    int height = c674x_dblexp->height;
    int i;

    /* ������ָ����� */
    unsigned char *restrict pSrc1RowB_0;
    unsigned char *restrict pSrc1RowG_0;
    unsigned char *restrict pSrc1RowR_0;
    unsigned char *restrict pSrc2RowB_0;
    unsigned char *restrict pSrc2RowG_0;
    unsigned char *restrict pSrc2RowR_0;
    unsigned char *restrict pDstRowB_0;
    unsigned char *restrict pDstRowG_0;
    unsigned char *restrict pDstRowR_0;

    /* ÿ�δ����������� */
    for (i = 0; i < height; i += 2) {
        /* ��ȡ��ǰ�е�ָ�� */
        pSrc1RowB_0 = c674x_dblexp->pImgSrc1B + (i+0)*width;  /* ��һ��ͼ��Bͨ����ǰ�� */
        pSrc1RowG_0 = c674x_dblexp->pImgSrc1G + (i+0)*width;  /* ��һ��ͼ��Gͨ����ǰ�� */
        pSrc1RowR_0 = c674x_dblexp->pImgSrc1R + (i+0)*width;  /* ��һ��ͼ��Rͨ����ǰ�� */

        pSrc2RowB_0 = c674x_dblexp->pImgSrc2B + (i+0)*width;  /* �ڶ���ͼ��Bͨ����ǰ�� */
        pSrc2RowG_0 = c674x_dblexp->pImgSrc2G + (i+0)*width;  /* �ڶ���ͼ��Gͨ����ǰ�� */
        pSrc2RowR_0 = c674x_dblexp->pImgSrc2R + (i+0)*width;  /* �ڶ���ͼ��Rͨ����ǰ�� */

        pDstRowB_0 = c674x_dblexp->pImgDstB + (i+0)*width;    /* Ŀ��ͼ��Bͨ����ǰ�� */
        pDstRowG_0 = c674x_dblexp->pImgDstG + (i+0)*width;    /* Ŀ��ͼ��Gͨ����ǰ�� */
        pDstRowR_0 = c674x_dblexp->pImgDstR + (i+0)*width;    /* Ŀ��ͼ��Rͨ����ǰ�� */

        /* �������Ի�ຯ����ֻ����10������ */
        process_pixels_dblexp_asm(
            pSrc1RowB_0, pSrc1RowG_0, pSrc1RowR_0,  /* ��һ��ͼ��RGBͨ�� */
            pSrc2RowB_0, pSrc2RowG_0, pSrc2RowR_0,  /* �ڶ���ͼ��RGBͨ�� */
            pDstRowB_0, pDstRowG_0, pDstRowR_0,     /* Ŀ��ͼ��RGBͨ�� */
            width);                                  /* ͼ���� */
    }
}

/**
 * Nanwan_alg_process - ��������ͼ��������״̬
 *
 * ʵ��ͼ������ع���Ĵ������̣����ݴ���ģʽѡ��ͬ��ʵ�ַ�ʽ
 *
 * @param handle: �㷨�ṹ��
 * @param param1: ֡�ṹ�壬�����������ͼ������
 * @param param2: ״̬�ṹ�壬���ش�����״̬
 * @return: 0��ʾ�ɹ���<0��ʾʧ��
 */
int Nanwan_alg_process(c674x_dblexp_t *handle,
                image_dblexp_frame_t *param1,
                image_dblexp_state_t *param2)
{
    c674x_dblexp_t *c674x_dblexp = handle;
    int width = c674x_dblexp->width;
    int height = c674x_dblexp->height;
    clock_t begin, end;             /* ʱ�Ӽ����������������ܲ��� */
    int process_mode = param1->processMode;  /* ��ȡ����ģʽ */

    /*1. ����һ��ͼ��Ĵ��BGR��ʽת��Ϊƽ��B��G��R��ʽ */
    pkRGB2plRGB(param1->pInImage1,
            c674x_dblexp->pImgSrc1B, c674x_dblexp->pImgSrc1G, c674x_dblexp->pImgSrc1R,
            width, height);

    /*2. ���ڶ���ͼ��Ĵ��BGR��ʽת��Ϊƽ��B��G��R��ʽ */
    pkRGB2plRGB(param1->pInImage2,
            c674x_dblexp->pImgSrc2B, c674x_dblexp->pImgSrc2G, c674x_dblexp->pImgSrc2R,
            width, height);

    /* ��ʼ��ʱ */
    begin = clock();

    /*3. ���ݴ���ģʽѡ��ͬʵ�ַ�ʽ����ͼ������ع⴦�� */
    switch (process_mode) {
        case 0:  /* C���԰汾 */
            process_pixels_c(c674x_dblexp);
            break;
        case 1:  /* Intrinsicָ���Ż� */
            process_pixels_intrinsic(c674x_dblexp);
            break;
        case 2:  /* ���Ի���Ż� */
            process_pixels_linear(c674x_dblexp);
            break;
        default:  /* Ĭ��ʹ��C���԰汾 */
            process_pixels_c(c674x_dblexp);
            break;
    }

    /* ������ʱ�����㴦��ʱ�� */
    end = clock();
    printf("Double exposure processing time = %d clock cycles (Mode %d)\n",
        end - begin, process_mode);

    /*4. ��ƽ��B��G��R��ʽת��Ϊ���BGR��ʽ��� */
    plRGB2pkRGB(param1->pOutData,
            c674x_dblexp->pImgDstB, c674x_dblexp->pImgDstG, c674x_dblexp->pImgDstR,
            width, height);

    /* �������״̬��Ϣ */
    param2->width = width;
    param2->height = height;
    param2->processTime = end - begin;  /* ��¼����ʱ�� */
    param2->processMode = process_mode; /* ��¼����ģʽ */

    return 0;  /* ���سɹ� */
}

/**
 * Nanwan_alg_create - �����㷨ʵ�������ݿ�͸������ڴ�
 *
 * @param param1: ���������ṹ�壬����ͼ��ߴ���Ϣ
 * @return: 0��ʾ�ɹ���<0��ʾʧ��
 */
int Nanwan_alg_create(image_dblexp_create_t *param1)
{
    c674x_dblexp_t *c674x_dblexp;
    int width = param1->width;    /* ��ȡͼ���� */
    int height = param1->height;  /* ��ȡͼ��߶� */

    /* Ϊ�㷨�ṹ������ڴ� */
    c674x_dblexp = (c674x_dblexp_t *)malloc(sizeof(c674x_dblexp_t));
    if (c674x_dblexp == NULL)
        goto alloc_memory_err;

    /* ��ʼ���㷨�ṹ���ڴ� */
    memset(c674x_dblexp, 0, sizeof(c674x_dblexp_t));

    /* Ϊ��һ��Դͼ���RGBͨ�������ڴ� */
    c674x_dblexp->pImgSrc1B = (unsigned char *)malloc(width*height);
    c674x_dblexp->pImgSrc1G = (unsigned char *)malloc(width*height);
    c674x_dblexp->pImgSrc1R = (unsigned char *)malloc(width*height);
    if (c674x_dblexp->pImgSrc1B == NULL ||
        c674x_dblexp->pImgSrc1G == NULL ||
        c674x_dblexp->pImgSrc1R == NULL)
        goto alloc_memory_err;

    /* Ϊ�ڶ���Դͼ���RGBͨ�������ڴ� */
    c674x_dblexp->pImgSrc2B = (unsigned char *)malloc(width*height);
    c674x_dblexp->pImgSrc2G = (unsigned char *)malloc(width*height);
    c674x_dblexp->pImgSrc2R = (unsigned char *)malloc(width*height);
    if (c674x_dblexp->pImgSrc2B == NULL ||
        c674x_dblexp->pImgSrc2G == NULL ||
        c674x_dblexp->pImgSrc2R == NULL)
        goto alloc_memory_err;

    /* ΪĿ��ͼ���RGBͨ�������ڴ� */
    c674x_dblexp->pImgDstB = (unsigned char *)malloc(width*height);
    c674x_dblexp->pImgDstG = (unsigned char *)malloc(width*height);
    c674x_dblexp->pImgDstR = (unsigned char *)malloc(width*height);
    if (c674x_dblexp->pImgDstB == NULL ||
        c674x_dblexp->pImgDstG == NULL ||
        c674x_dblexp->pImgDstR == NULL)
        goto alloc_memory_err;

    /* ����ͼ��ߴ� */
    c674x_dblexp->width = width;
    c674x_dblexp->height = height;

    /* ���������������سɹ� */
    param1->handle = c674x_dblexp;
    return 0;

alloc_memory_err:
    /* �������ͷ��ѷ�����ڴ� */
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
    return C674X_ERR_MEMORY;  /* �����ڴ������� */
}

/**
 * Nanwan_alg_destroy - �ͷ��ڴ棬�����㷨ʵ��
 *
 * @param handle: �㷨ʵ���ṹ��ָ��
 * @return: 0��ʾ�ɹ���<0��ʾʧ��
 */
int Nanwan_alg_destroy(c674x_dblexp_t *handle)
{
    c674x_dblexp_t *c674x_dblexp = handle;

    /* �ͷ����з�����ڴ� */
    free(c674x_dblexp->pImgDstR);  /* �ͷ�Ŀ��ͼ��Rͨ���ڴ� */
    free(c674x_dblexp->pImgDstG);  /* �ͷ�Ŀ��ͼ��Gͨ���ڴ� */
    free(c674x_dblexp->pImgDstB);  /* �ͷ�Ŀ��ͼ��Bͨ���ڴ� */

    free(c674x_dblexp->pImgSrc2R);  /* �ͷŵڶ���Դͼ��Rͨ���ڴ� */
    free(c674x_dblexp->pImgSrc2G);  /* �ͷŵڶ���Դͼ��Gͨ���ڴ� */
    free(c674x_dblexp->pImgSrc2B);  /* �ͷŵڶ���Դͼ��Bͨ���ڴ� */

    free(c674x_dblexp->pImgSrc1R);  /* �ͷŵ�һ��Դͼ��Rͨ���ڴ� */
    free(c674x_dblexp->pImgSrc1G);  /* �ͷŵ�һ��Դͼ��Gͨ���ڴ� */
    free(c674x_dblexp->pImgSrc1B);  /* �ͷŵ�һ��Դͼ��Bͨ���ڴ� */

    free(c674x_dblexp);  /* �ͷ��㷨�ṹ���ڴ� */
    return 0;  /* ���سɹ� */
}
