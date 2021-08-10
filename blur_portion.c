#include <stdio.h>
#include <stdlib.h>
#include "multithreading.h"


/**
 * blur_portion - desc
 * @portion: portion
 */
void blur_portion(blur_portion_t const *portion)
{C99(
	const kernel_t *k = portion->kernel;
	const img_t *src = portion->img;
	img_t *dst = portion->img_blur;
	int x = portion->x, y = portion->y;
	int iw = portion->w, ih = portion->h;
	int kw = k->size, kh = k->size;
	int ix, iy, kx, ky, n, r = k->size/2;
	for (iy = y; iy < y + ih; iy++)
	for (ix = x; ix < x + iw; ix++)
	{
		float wgt, sum = 0, rgb[3] = {0};
		for (ky = -r; ky < kh - r; ky++)
		for (kx = -r; kx < kw - r; kx++)
		{
			if ((ix+kx >= 0 && (unsigned)(ix+kx) < src->w) &&
			    (iy+ky >= 0 && (unsigned)(iy+ky) < src->h))
			{
				sum += (wgt = k->matrix[ky+r][kx+r]);
				uint8_t *pxs = (uint8_t *)&GET_PIXEL(src, ix+kx, iy+ky);
				for (n = 0; n < 3; n++)
					rgb[n] += wgt * pxs[n];
			}
		}
		for (n = 0; n < 3; n++)
			rgb[n] /= sum, rgb[n] = (uint8_t)rgb[n];
		PUT_PIXEL(rgb, dst, ix, iy);
	}
);}
