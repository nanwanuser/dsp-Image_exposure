#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "ImageDoubleExposure.h"
#include "bmp_head_struct.h"

image_dblexp_create_t dblexp_create;
image_dblexp_frame_t dblexp_frame;
image_dblexp_state_t dblexp_state;

BITMAPFILEHEADER *p_bmpFileHead; // 14 B
BITMAPINFOHEADER *p_bmpInfoHead; // 40 B
void *handle;

// ������ͼ��
void process_single_image(int process_mode) {
    int SizeImage;
    int width, height;
    FILE *file_src1 = NULL;
    FILE *file_src2 = NULL;
    FILE *file_dst = NULL;
    BYTE *p_src1 = NULL, *p_src2 = NULL, *p_dst = NULL;
    clock_t begin, end;
    char output_filename[100];

    printf("--- Image Double Exposure Processing (Mode %d) ---\n", process_mode);

    // �򿪵�һ������ͼ��
    file_src1 = fopen("..//bmps//ImageA.bmp", "rb");
    if (!file_src1) {
        printf("file_src1 open error!\n");
        return;
    }

    // �򿪵ڶ�������ͼ��
    file_src2 = fopen("..//bmps//ImageB.bmp", "rb");
    if (!file_src2) {
        printf("file_src2 open error!\n");
        fclose(file_src1);
        return;
    }

    // ��������ļ�����
    sprintf(output_filename, "..//bmps//Result_Mode%d.bmp", process_mode);
    file_dst = fopen(output_filename, "wb");
    if (!file_dst) {
        printf("file_dst open error!\n");
        fclose(file_src1);
        fclose(file_src2);
        return;
    }

    p_bmpFileHead = (BITMAPFILEHEADER *)malloc(14);
    p_bmpInfoHead = (BITMAPINFOHEADER *)malloc(40);

    // ��ȡBMP�ļ�ͷ
    fread(p_bmpFileHead, 1, 14, file_src1);
    fread(p_bmpInfoHead, 1, 40, file_src1);

    // ���ڶ���ͼ��ĳߴ��Ƿ�һ�£�Ӧ�������ļ�ͷֱ�Ӷ�ȡ��Ϣͷ��
    BITMAPFILEHEADER tempFileHead;
    BITMAPINFOHEADER tempInfoHead;
    fread(&tempFileHead, 1, 14, file_src2);
    fread(&tempInfoHead, 1, 40, file_src2);

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

    // �����ļ���ͷ�Ա������ȡ
    fseek(file_src2, 0, SEEK_SET);
    fread(&tempFileHead, 1, 14, file_src2);
    fread(&tempInfoHead, 1, 40, file_src2);

    width = p_bmpInfoHead->biWidth;
    height = p_bmpInfoHead->biHeight;
    SizeImage = width * height * 3;
    p_src1 = (BYTE *)malloc(SizeImage);
    p_src2 = (BYTE *)malloc(SizeImage);
    p_dst = (BYTE *)malloc(SizeImage);
    if ((!p_src1) || (!p_src2) || (!p_dst)) {
        printf("Error in malloc for image data!\n");
        if (p_src1) free(p_src1);
        if (p_src2) free(p_src2);
        if (p_dst) free(p_dst);
        free(p_bmpFileHead);
        free(p_bmpInfoHead);
        fclose(file_src1);
        fclose(file_src2);
        fclose(file_dst);
        return;
    }

    // 1.-----------------------------------------------------------------create
    dblexp_create.width = width;
    dblexp_create.height = height;
    begin = clock();
    image_double_exposure(NULL, DSP_ALG_CREATE, &dblexp_create, NULL);
    end = clock();
    printf("DSP_ALG_CREATE time = %d\n", end - begin);
    handle = dblexp_create.handle;

    // ��ȡ����ͼ�����������
    fread(p_src1, 1, SizeImage, file_src1);
    fread(p_src2, 1, SizeImage, file_src2);

    // 2.-----------------------------------------------------------------double exposure
    printf("\n--- Image Double Exposure Begin! ---\n");
    dblexp_frame.formatImage = IMAGE_FORMAT_RGB;
    dblexp_frame.pInImage1 = p_src1;
    dblexp_frame.pInImage2 = p_src2;
    dblexp_frame.pOutData = p_dst;
    dblexp_frame.processMode = process_mode; // ѡ����ģʽ

    image_double_exposure(handle, DSP_ALG_PROCESS, &dblexp_frame, &dblexp_state);

    printf("Processing Time: %d clock cycles\n", dblexp_state.processTime);
    printf("\n--- Image Double Exposure End! ---\n");

    // 3.-----------------------------------------------------------------destroy
    image_double_exposure(handle, DSP_ALG_DESTROY, NULL, NULL);

    // д������ļ�
    fwrite(p_bmpFileHead, 1, 14, file_dst);
    fwrite(p_bmpInfoHead, 1, 40, file_dst);
    fwrite(p_dst, 1, SizeImage, file_dst);

    fclose(file_src1);
    fclose(file_src2);
    fclose(file_dst);

    free(p_bmpFileHead);
    free(p_bmpInfoHead);
    free(p_src1);
    free(p_src2);
    free(p_dst);
}

/*
 * main.c
 */
int main(void) {
    printf("\n========= DSP ͼ����Ӧ��ϵͳ - ͼ������ع� =========\n");
    printf("ʵ�ַ�ʽ��C���ԡ�Intrinsicָ������Ի��\n\n");

    // ʹ��C����ʵ��
    process_single_image(0);

    // ʹ��Intrinsicָ��ʵ��
    process_single_image(1);

    // ʹ�����Ի��ʵ��
    process_single_image(2);

    printf("\n============ ������ɣ���鿴���ͼ���ļ� ============\n");

    return 0;
}
