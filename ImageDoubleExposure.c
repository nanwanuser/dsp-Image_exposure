/*
 * ImageDoubleExposure.c - ͼ������ع��㷨�ӿ�ʵ��
 *
 * ��������: 2025-5-17
 * ����: ���ļ�ʵ����ͼ������ع��㷨�Ľӿں���
 */

#include "DoubleExposureCore.h"  /* ���������㷨ͷ�ļ� */

/**
 * image_double_exposure - ͼ������ع��㷨�ӿں���
 *
 * ���ݲ�������ֱ���ô�������������ٺ���
 *
 * @param handle: �㷨ʵ�����
 * @param opt: �������루CREATE/DESTROY/PROCESS��
 * @param param1: ����1ָ�룬���ݲ������벻ָͬ��ͬ�ṹ
 * @param param2: ����2ָ�룬���ݲ������벻ָͬ��ͬ�ṹ
 * @return: =0��ʾ�ɹ���<0��ʾʧ��
 */
int image_double_exposure(void *handle, int opt, void *param1, void *param2)
{
    switch (opt) {
    case DSP_ALG_PROCESS:  /* ����ͼ����� */
        return Nanwan_alg_process(
                (c674x_dblexp_t *)handle,            /* ����ת��Ϊ�ڲ��㷨�ṹ */
                (image_dblexp_frame_t *)param1,      /* ����ת��Ϊ����֡�ṹ */
                (image_dblexp_state_t *)param2);     /* ����ת��Ϊ״̬�ṹ */

    case DSP_ALG_CREATE:   /* �����㷨ʵ������ */
        return Nanwan_alg_create((image_dblexp_create_t *)param1);  /* ����ת��Ϊ�����ṹ */

    case DSP_ALG_DESTROY:  /* �����㷨ʵ������ */
        return Nanwan_alg_destroy((c674x_dblexp_t *) handle);  /* ����ת��Ϊ�ڲ��㷨�ṹ */

    default:               /* ��Ч�������� */
        return C674X_ERR_FAIL;  /* ����һ���Դ��� */
    }
}
