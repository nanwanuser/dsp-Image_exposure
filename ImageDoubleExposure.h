/*
 * ImageDoubleExposure.h
 *
 *  Created on: 2025-5-17
 */

#ifndef IMAGEDOUBLEEXPOSURE_H_
#define IMAGEDOUBLEEXPOSURE_H_

#define C674X_ERR_MEMORY	-1		/* memory allocation error */
#define C674X_ERR_FAIL		-2		/* general fault */
#define C674X_ERR_FORMAT	-3		/* file format error */

#define IMAGE_FORMAT_RGB  (1<<0)
#define IMAGE_FORMAT_YUV  (1<<1)

/*----------------------------------------------------------------------------
 * image_dblexp_create_t structure definition
 *
 * This structure is passed as param1 during an instance creation (operation
 * DSP_ALG_CREATE)
 *--------------------------------------------------------------------------*/
typedef struct {
	int version;                 /* [in] algorithm version */
	int width;                   /* [in] frame width, pixel units */
	int height;                  /* [in] frame height, pixel units */

	void *handle;                /* [out] algorithm instance handle */
} image_dblexp_create_t;

/*----------------------------------------------------------------------------
 * image_dblexp_frame_t structure definition
 *
 * This structure is passed as param1 during processing an image (operation
 * DSP_ALG_PROCESS)
 *--------------------------------------------------------------------------*/
typedef struct {
	int formatImage;             /* [in] RGB or YUV */
	unsigned char *pInImage1;    /* [in] first input image packed pointer */
	unsigned char *pInImage2;    /* [in] second input image packed pointer */
	unsigned char *pOutData;     /* [out] packed pointer */
	int length;                  /* [out] length of result */
	int processMode;             /* [in] processing mode: 0-C, 1-Intrinsic, 2-Linear ASM */
} image_dblexp_frame_t;

/*----------------------------------------------------------------------------
 * image_dblexp_state_t structure definition
 *
 * This structure is passed as param2 during processing an image (operation
 * DSP_ALG_PROCESS)
 *--------------------------------------------------------------------------*/
typedef struct {
	int width;                  /* [out] frame width, pixel units */
	int height;                 /* [out] frame height, pixel units */
	int processTime;            /* [out] processing time in clock cycles */
	int processMode;            /* [out] processing mode used */
} image_dblexp_state_t;

#define DSP_ALG_CREATE  0 /* create algorithm instance; returns 0 on success */
#define DSP_ALG_DESTROY 1 /* destroy algorithm instance; returns 0 on success */
#define DSP_ALG_PROCESS 2 /* process double exposure */
/*----------------------------------------------------------------------------
 * Description:   image double exposure algorithm interface
 * Parameters:
 * 		-handle: algorithm instance handle
 *		-opt   : operation one of CREATE/DESTROY/PROCESS
 *		-param1: parameter 1 pointer
 *		-param2: parameter 2 pointer
 * Return value:
 * 	    =0: successful
 * 	    <0: failed
 * --------------------------------------------------------------------------*/
int image_double_exposure(void *handle, int opt, void *param1, void *param2);


#endif /* IMAGEDOUBLEEXPOSURE_H_ */
