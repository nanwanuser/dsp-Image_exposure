/*
 * DoubleExposureCore.h
 *
 *  Created on: 2025-5-17
 */

#ifndef DOUBLEEXPOSURECORE_H_
#define DOUBLEEXPOSURECORE_H_

#include "ImageDoubleExposure.h"

/*----------------------------------------------------------------------------
 * c674x_dblexp_t struct definition
 *
 * This struct is used for internal algorithm
 *--------------------------------------------------------------------------*/
typedef struct c674x_dblexp_t{
	unsigned char *pImgSrc1R;  /* source image 1 R, w*h */
	unsigned char *pImgSrc1G;  /* source image 1 G, w*h */
	unsigned char *pImgSrc1B;  /* source image 1 B, w*h */

	unsigned char *pImgSrc2R;  /* source image 2 R, w*h */
	unsigned char *pImgSrc2G;  /* source image 2 G, w*h */
	unsigned char *pImgSrc2B;  /* source image 2 B, w*h */

	unsigned char *pImgDstR;   /* destination image R, w*h */
	unsigned char *pImgDstG;   /* destination image G, w*h */
	unsigned char *pImgDstB;   /* destination image B, w*h */

	int width;                 /* image width */
	int height;                /* image height */
} c674x_dblexp_t;

/*----------------------------------------------------------------------------
 * Description  : process the input frames, and output state
 * Parameters:
 * 		-handle: algorithm struct c674x_dblexp_t
 * 		-param1: frame struct image_dblexp_frame_t
 * 		-param2: state struct image_dblexp_state_t
 * Return value:
 * 	     0: successful; <0: failed
 * --------------------------------------------------------------------------*/
int ljz_alg_process(c674x_dblexp_t *handle,
		image_dblexp_frame_t *param1,
		image_dblexp_state_t *param2);

/*----------------------------------------------------------------------------
 * Description:   create algorithm instance, and allocate memories
 * Parameters:
 * 		-param1: create struct image_dblexp_create_t
 * Return value:
 * 	     0: successful; <0: failed
 * --------------------------------------------------------------------------*/
int ljz_alg_create(image_dblexp_create_t *param1);

/*----------------------------------------------------------------------------
 * Description:   destroy algorithm, and free memories
 * Parameters:
 * 		-handle:  algorithm struct c674x_dblexp_t
 * Return value:
 * 	     0: successful; <0: failed
 * --------------------------------------------------------------------------*/
int ljz_alg_destroy(c674x_dblexp_t *handle);

#endif /* DOUBLEEXPOSURECORE_H_ */
