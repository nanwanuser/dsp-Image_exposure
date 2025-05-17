/*
 * main.c - ͼ������ع⴦��������
 *
 * ���ߣ�[��骣�Nanwan��]
 * ���ڣ�2025-5-17
 *
 * ������������ʵ����ͼ������ع⴦���ܣ�֧������ʵ�ַ�ʽ��
 *      1. C����ʵ��
 *      2. Intrinsicָ���Ż�ʵ��
 *      3. ���Ի���Ż�ʵ��
 * ������Ϊ���ϿƼ���ѧDSP�γ������Ŀ��ʹ��TI C6748 DSP������
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "ImageDoubleExposure.h"  // ͼ������ع��㷨�ӿ�ͷ�ļ�
#include "bmp_head_struct.h"      // BMP�ļ���ʽ�ṹ����

// ȫ�ֱ�������
image_dblexp_create_t dblexp_create;  // �㷨���������ṹ
image_dblexp_frame_t dblexp_frame;    // �㷨����֡�ṹ
image_dblexp_state_t dblexp_state;    // �㷨״̬�ṹ

BITMAPFILEHEADER *p_bmpFileHead;      // BMP�ļ�ͷָ�� (14�ֽ�)
BITMAPINFOHEADER *p_bmpInfoHead;      // BMP��Ϣͷָ�� (40�ֽ�)
void *handle;                         // �㷨ʵ�����

/**
 * ������ͼ��ĺ���
 *
 * @param process_mode - ����ģʽ: 0-C���ԣ�1-Intrinsic��2-���Ի��
 */
void process_single_image(int process_mode) {
    int SizeImage;                    // ͼ�����ݴ�С(�ֽ�)
    int width, height;                // ͼ���Ⱥ͸߶�
    FILE *file_src1 = NULL;           // ��һ��Դͼ���ļ�ָ��
    FILE *file_src2 = NULL;           // �ڶ���Դͼ���ļ�ָ��
    FILE *file_dst = NULL;            // Ŀ��ͼ���ļ�ָ��
    BYTE *p_src1 = NULL;              // ��һ��Դͼ������ָ��
    BYTE *p_src2 = NULL;              // �ڶ���Դͼ������ָ��
    BYTE *p_dst = NULL;               // Ŀ��ͼ������ָ��
    clock_t begin, end;               // ʱ�Ӽ����������������ܲ���
    char output_filename[100];        // ����ļ���������

    printf("--- Image Double Exposure Processing (Mode %d) ---\n", process_mode);

    // �򿪵�һ������ͼ���ļ�
    file_src1 = fopen("..//bmps//ImageA.bmp", "rb");
    if (!file_src1) {
        printf("file_src1 open error!\n");
        return;
    }

    // �򿪵ڶ�������ͼ���ļ�
    file_src2 = fopen("..//bmps//ImageB.bmp", "rb");
    if (!file_src2) {
        printf("file_src2 open error!\n");
        fclose(file_src1);
        return;
    }

    // ��������ļ����Ʋ�������ļ�
    sprintf(output_filename, "..//bmps//Result_Mode%d.bmp", process_mode);
    file_dst = fopen(output_filename, "wb");
    if (!file_dst) {
        printf("file_dst open error!\n");
        fclose(file_src1);
        fclose(file_src2);
        return;
    }

    // ΪBMP�ļ�ͷ����Ϣͷ�����ڴ�
    p_bmpFileHead = (BITMAPFILEHEADER *)malloc(14);
    p_bmpInfoHead = (BITMAPINFOHEADER *)malloc(40);

    // ��ȡ��һ��ͼ���BMP�ļ�ͷ����Ϣͷ
    fread(p_bmpFileHead, 1, 14, file_src1);
    fread(p_bmpInfoHead, 1, 40, file_src1);

    // ���ڶ���ͼ��ĳߴ��Ƿ����һ��ͼ��һ��
    BITMAPFILEHEADER tempFileHead;
    BITMAPINFOHEADER tempInfoHead;
    fread(&tempFileHead, 1, 14, file_src2);
    fread(&tempInfoHead, 1, 40, file_src2);

    // �������ͼ��ߴ粻һ�£������˳�
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

    // ���ڶ���ͼ���ļ�ָ�����õ��ļ���ͷ���Ա������ȡ��������
    fseek(file_src2, 0, SEEK_SET);
    fread(&tempFileHead, 1, 14, file_src2);
    fread(&tempInfoHead, 1, 40, file_src2);

    // ��ȡͼ��ߴ���Ϣ������ͼ�����ݴ�С
    width = p_bmpInfoHead->biWidth;
    height = p_bmpInfoHead->biHeight;
    SizeImage = width * height * 3;  // 3�ֽ�/���� (RGB)

    // Ϊͼ�����ݷ����ڴ�
    p_src1 = (BYTE *)malloc(SizeImage);
    p_src2 = (BYTE *)malloc(SizeImage);
    p_dst = (BYTE *)malloc(SizeImage);

    // ����ڴ�����Ƿ�ɹ�
    if ((!p_src1) || (!p_src2) || (!p_dst)) {
        printf("Error in malloc for image data!\n");
        // �ͷ��ѷ�����ڴ�
        if (p_src1) free(p_src1);
        if (p_src2) free(p_src2);
        if (p_dst) free(p_dst);
        free(p_bmpFileHead);
        free(p_bmpInfoHead);
        // �ر��Ѵ򿪵��ļ�
        fclose(file_src1);
        fclose(file_src2);
        fclose(file_dst);
        return;
    }

    // ����1: �����㷨ʵ�� ------------------------------------
    dblexp_create.width = width;
    dblexp_create.height = height;
    begin = clock();  // ��ʼ��ʱ
    image_double_exposure(NULL, DSP_ALG_CREATE, &dblexp_create, NULL);
    end = clock();    // ������ʱ
    printf("DSP_ALG_CREATE time = %d\n", end - begin);
    handle = dblexp_create.handle;

    // ��ȡ����ͼ�����������
    fread(p_src1, 1, SizeImage, file_src1);
    fread(p_src2, 1, SizeImage, file_src2);

    // ����2: ִ��ͼ������ع⴦�� -----------------------------
    printf("\n--- Image Double Exposure Begin! ---\n");
    // ���ô������
    dblexp_frame.formatImage = IMAGE_FORMAT_RGB;
    dblexp_frame.pInImage1 = p_src1;
    dblexp_frame.pInImage2 = p_src2;
    dblexp_frame.pOutData = p_dst;
    dblexp_frame.processMode = process_mode;  // ѡ����ģʽ: 0-C, 1-Intrinsic, 2-ASM

    // ����ͼ������ع��㷨
    image_double_exposure(handle, DSP_ALG_PROCESS, &dblexp_frame, &dblexp_state);

    // �������ʱ�䣨���ܲ�����
    printf("Processing Time: %d clock cycles\n", dblexp_state.processTime);
    printf("\n--- Image Double Exposure End! ---\n");

    // ����3: �����㷨ʵ�����ͷ���Դ ---------------------------
    image_double_exposure(handle, DSP_ALG_DESTROY, NULL, NULL);

    // ��������д������ļ�
    fwrite(p_bmpFileHead, 1, 14, file_dst);  // д���ļ�ͷ
    fwrite(p_bmpInfoHead, 1, 40, file_dst);  // д����Ϣͷ
    fwrite(p_dst, 1, SizeImage, file_dst);   // д����������

    // �ر������ļ�
    fclose(file_src1);
    fclose(file_src2);
    fclose(file_dst);

    // �ͷ����з�����ڴ�
    free(p_bmpFileHead);
    free(p_bmpInfoHead);
    free(p_src1);
    free(p_src2);
    free(p_dst);
}

/**
 * ������
 *
 * ����ʹ�����ֲ�ͬ��ʵ�ַ�ʽ����ͼ�񣬲��Ƚ�����
 */
int main(void) {
    printf("\n========= DSP ͼ����Ӧ��ϵͳ - ͼ������ع� =========\n");
    printf("ʵ�ַ�ʽ��C���ԡ�Intrinsicָ������Ի��\n\n");

    // ʹ��C����ʵ�ִ���ͼ��
    process_single_image(0);

    // ʹ��Intrinsicָ���Ż�ʵ�ִ���ͼ��
    process_single_image(1);

    // ʹ�����Ի���Ż�ʵ�ִ���ͼ��
    process_single_image(2);

    printf("\n============ ������ɣ���鿴���ͼ���ļ� ============\n");

    return 0;
}
