/*
 * bmp_head_struct.h
 *
 *  Created on: 2018-2-17
 *      Author: Lu1
 */

#ifndef BMP_HEAD_STRUCT_H_
#define BMP_HEAD_STRUCT_H_

typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef int LONG;
typedef unsigned char BYTE;

/*BMP�ļ�ͷ*/
typedef struct tagBITMAPFILEHEADER{
	WORD  bfType;
	DWORD bfSize;
	WORD  bfReserved1;
	WORD  bfReserved2;
	DWORD bfOffBits;
} BITMAPFILEHEADER;

/*BMP�ļ���Ϣͷ*/
typedef struct tagBITMAPINFOHEADER {
	DWORD biSize;
	LONG biWidth;
	LONG biHeight;
	WORD biPlanes;
	WORD biBitCount;
	DWORD biCompression;
	DWORD biSizeImage;
	LONG biXPelsPerMeter;
	LONG biYPelsPerMeter;
	DWORD biClrUsed;
	DWORD biClrImportant;
} BITMAPINFOHEADER;

typedef struct RGB_QUAD
{
	BYTE rgbBlue; /* ��ɫǿ�� */
	BYTE rgbGreen; /* ��ɫǿ�� */
	BYTE rgbRed; /* ��ɫǿ�� */
	BYTE rgbReversed; /* ����ֵ */
} RGBQUAD;

#endif /* BMP_HEAD_STRUCT_H_ */
