#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "multithreading.h"

#ifndef NUMTHREADS
#define NUMTHREADS 16
#endif

/**
 * _thread_entry - entry point for blurring portions
 * @arg: generic pointer to a blur_portion_t
 * Return: generic pointer
 */
void *_thread_entry(void *arg)
{
	blur_portion((const blur_portion_t *)arg);
	return (NULL);
}

/**
 * blur_image - blur image by apportioning vertical strips
 * @img_blur: img_t pointer to blurring copy
 * @img: const img_t pointer to original
 * @kernel: kernel_t pointer
 */
void blur_image(img_t *img_blur, img_t const *img, kernel_t const *kernel)
{C99(
	pthread_t thr[NUMTHREADS];
	blur_portion_t ptn[NUMTHREADS];
	int i, x, r = img->w % NUMTHREADS;
	for (i = 0; i < NUMTHREADS; i++)
		ptn[i] = (blur_portion_t){img, img_blur, 0, 0, img->w / NUMTHREADS, img->h, kernel};

	for (i = x = 0; i < NUMTHREADS; i++)
	{
		ptn[i].x = x;
		x += (ptn[i].w += (r-i > 0));
		pthread_create(&thr[i], NULL, &_thread_entry, &ptn[i]);
	}
	for (i = 0; i < NUMTHREADS; i++)
		pthread_join(thr[i], NULL);
	DBG(printf("Threads: %d\n", NUMTHREADS));
);}
