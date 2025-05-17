/*
 * RGB2RGB.h
 *
 *  Created on: 2018-2-17
 *      Author: Lu1
 */

#ifndef RGB2RGB_H_
#define RGB2RGB_H_

/**packed BGR to planar B, G and R**/
void pkRGB2plRGB(unsigned char *pSrcBGR,
		unsigned char *pDstB,
		unsigned char *pDstG,
		unsigned char *pDstR,
		int width, int height);

/**planar B, G and R to packed BGR**/
void plRGB2pkRGB(unsigned char *pDstBGR,
		unsigned char *pSrcB,
		unsigned char *pSrcG,
		unsigned char *pSrcR,
		int width, int height);

#endif /* RGB2RGB_H_ */
