/*
 * RGB2RGB.c
 *
 *  Created on: 2018-2-17
 *      Author: Lu1
 */

void pkRGB2plRGB(unsigned char *pSrcBGR,
		unsigned char *pDstB,
		unsigned char *pDstG,
		unsigned char *pDstR,
		int width, int height)
{
	int i,j;

	for (i=0; i<height; i++) {
		unsigned char *pDstRowB = pDstB + i*width; //
		unsigned char *pDstRowG = pDstG + i*width; //
		unsigned char *pDstRowR = pDstR + i*width; //
		unsigned char *pRowBGR  = pSrcBGR + i*width*3;//
		for (j=0; j<width; j++)	{
			pDstRowB[j] = pRowBGR[j*3+0]; //
			pDstRowG[j] = pRowBGR[j*3+1]; //
			pDstRowR[j] = pRowBGR[j*3+2]; //
		}
	}
}

void plRGB2pkRGB(unsigned char *pDstBGR,
		unsigned char *pSrcB,
		unsigned char *pSrcG,
		unsigned char *pSrcR,
		int width, int height)
{
	int i,j;

	for (i=0; i<height; i++)
	{
		unsigned char *pSrcRowB = pSrcB + i*width;
		unsigned char *pSrcRowG = pSrcG + i*width;
		unsigned char *pSrcRowR = pSrcR + i*width;
		unsigned char *pRowBGR = pDstBGR + i*width*3;
		for (j=0; j<width; j++)
		{
			pRowBGR[j*3+0] = pSrcRowB[j];
			pRowBGR[j*3+1] = pSrcRowG[j];
			pRowBGR[j*3+2] = pSrcRowR[j];
		}
	}
}
