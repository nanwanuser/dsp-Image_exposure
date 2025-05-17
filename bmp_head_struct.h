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

/*BMP文件头*/
typedef struct tagBITMAPFILEHEADER{
	WORD  bfType;
	DWORD bfSize;
	WORD  bfReserved1;
	WORD  bfReserved2;
	DWORD bfOffBits;
} BITMAPFILEHEADER;

/*BMP文件信息头*/
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
	BYTE rgbBlue; /* 蓝色强度 */
	BYTE rgbGreen; /* 绿色强度 */
	BYTE rgbRed; /* 红色强度 */
	BYTE rgbReversed; /* 保留值 */
} RGBQUAD;

#endif /* BMP_HEAD_STRUCT_H_ */
