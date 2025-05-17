/*
 * ImageDoubleExposure.h - ͼ������ع��㷨�ӿ�ͷ�ļ�
 *
 * ��������: 2025-5-17
 * ����: ���ļ�������ͼ������ع⴦���API�ӿڡ����ݽṹ�ͳ���
 */

#ifndef IMAGEDOUBLEEXPOSURE_H_
#define IMAGEDOUBLEEXPOSURE_H_

/* ������붨�� */
#define C674X_ERR_MEMORY    -1      /* �ڴ������� */
#define C674X_ERR_FAIL      -2      /* һ���Թ��� */
#define C674X_ERR_FORMAT    -3      /* �ļ���ʽ���� */

/* ͼ���ʽ���� */
#define IMAGE_FORMAT_RGB  (1<<0)    /* RGB��ʽͼ�� */
#define IMAGE_FORMAT_YUV  (1<<1)    /* YUV��ʽͼ�� */

/*----------------------------------------------------------------------------
 * image_dblexp_create_t �ṹ�嶨��
 *
 * �˽ṹ�ڴ����㷨ʵ��ʱ��Ϊparam1�������ݣ���������DSP_ALG_CREATE��
 *--------------------------------------------------------------------------*/
typedef struct {
    int version;                 /* [����] �㷨�汾�� */
    int width;                   /* [����] ͼ���ȣ����ص�λ */
    int height;                  /* [����] ͼ��߶ȣ����ص�λ */

    void *handle;                /* [���] �㷨ʵ����� */
} image_dblexp_create_t;

/*----------------------------------------------------------------------------
 * image_dblexp_frame_t �ṹ�嶨��
 *
 * �˽ṹ�ڴ���ͼ��ʱ��Ϊparam1�������ݣ���������DSP_ALG_PROCESS��
 *--------------------------------------------------------------------------*/
typedef struct {
    int formatImage;             /* [����] ͼ���ʽ��RGB��YUV */
    unsigned char *pInImage1;    /* [����] ��һ������ͼ������ָ�루�����ʽ�� */
    unsigned char *pInImage2;    /* [����] �ڶ�������ͼ������ָ�루�����ʽ�� */
    unsigned char *pOutData;     /* [���] ���ͼ������ָ�루�����ʽ�� */
    int length;                  /* [���] ������ݳ��� */
    int processMode;             /* [����] ����ģʽ: 0-C����, 1-Intrinsic, 2-���Ի�� */
} image_dblexp_frame_t;

/*----------------------------------------------------------------------------
 * image_dblexp_state_t �ṹ�嶨��
 *
 * �˽ṹ�ڴ���ͼ��ʱ��Ϊparam2�������ݣ���������DSP_ALG_PROCESS��
 *--------------------------------------------------------------------------*/
typedef struct {
    int width;                  /* [���] ͼ���ȣ����ص�λ */
    int height;                 /* [���] ͼ��߶ȣ����ص�λ */
    int processTime;            /* [���] ����ʱ�䣨ʱ���������� */
    int processMode;            /* [���] ʹ�õĴ���ģʽ */
} image_dblexp_state_t;

/* �������붨�� */
#define DSP_ALG_CREATE  0 /* �����㷨ʵ��; �ɹ�����0 */
#define DSP_ALG_DESTROY 1 /* �����㷨ʵ��; �ɹ�����0 */
#define DSP_ALG_PROCESS 2 /* ִ��ͼ������ع⴦�� */

/*----------------------------------------------------------------------------
 * ����:   ͼ������ع��㷨�ӿں���
 * ����:
 *      -handle: �㷨ʵ�����
 *      -opt   : �������룬������CREATE/DESTROY/PROCESS֮һ
 *      -param1: ����1ָ�룬���ݲ������벻ָͬ��ͬ�ṹ
 *      -param2: ����2ָ�룬���ݲ������벻ָͬ��ͬ�ṹ
 * ����ֵ:
 *      =0: �ɹ�
 *      <0: ʧ��
 * --------------------------------------------------------------------------*/
int image_double_exposure(void *handle, int opt, void *param1, void *param2);


#endif /* IMAGEDOUBLEEXPOSURE_H_ */
