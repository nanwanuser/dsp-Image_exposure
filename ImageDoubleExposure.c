/*
 * ImageDoubleExposure.c
 *
 *  Created on: 2025-5-17
 */

#include "DoubleExposureCore.h"

int
image_double_exposure(void *handle, int opt, void *param1, void *param2)
{
	switch (opt) {
	case DSP_ALG_PROCESS:
		return ljz_alg_process(
				(c674x_dblexp_t *)handle,
				(image_dblexp_frame_t *)param1,
				(image_dblexp_state_t *)param2);

	case DSP_ALG_CREATE:
		return ljz_alg_create((image_dblexp_create_t *)param1);

	case DSP_ALG_DESTROY:
		return ljz_alg_destroy((c674x_dblexp_t *) handle);

	default:
		return C674X_ERR_FAIL;
	}
}
