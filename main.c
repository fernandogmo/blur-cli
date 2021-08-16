#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include "multithreading.h"
#pragma GCC diagnostic ignored "-Wunused-result"

/**
 * load_kernel - Load convolution kernel from a file
 *
 * @kernel: Pointer to the kernel structure to fill in
 * @file:   Path to the file to parse
 */
void load_kernel(kernel_t *kernel, char const *file)
{
	FILE *f;
	size_t i, j;

	f = fopen(file, "r");
	fscanf(f, "%lu\n", &kernel->size);
	printf("Kernel size -> %lu\n", kernel->size);
	kernel->matrix = malloc(kernel->size * sizeof(float *));
	for (i = 0; i < kernel->size; i++)
	{
		kernel->matrix[i] = malloc(kernel->size * sizeof(float));
		for (j = 0; j < kernel->size; j++)
			fscanf(f, "%f", &kernel->matrix[i][j]);
	}
	fclose(f);
}

/**
 * load_image - Load an image from a PPM file
 *
 * @img:  Pointer to the image structure to fill in
 * @file: Path to the file to parse
 */
void load_image(img_t *img, char const *file)
{
	FILE *f;
	size_t i;

	f = fopen(file, "r");
	fscanf(f, "P6\n %lu %lu 255\n", &img->w, &img->h);
	printf("Image size -> %lu * %lu\n", img->w, img->h);
	img->pixels = malloc(img->w * img->h * sizeof(*img->pixels));

	for (i = 0; i < img->w * img->h; i++)
		fscanf(f, "%c%c%c", &img->pixels[i].r, &img->pixels[i].g, &img->pixels[i].b);
	fclose(f);
}

/**
 * img_copy - Copy an image structure
 *
 * @dest: Pointer to the image structure to fill in
 * @src:  Pointer to the image structure to copy
 */
void img_copy(img_t *dest, img_t const *src)
{
	size_t nb_pixels = src->w * src->h;

	dest->w = src->w;
	dest->h = src->h;
	dest->pixels = malloc(nb_pixels * sizeof(pixel_t));
	memcpy(dest->pixels, src->pixels, nb_pixels * sizeof(pixel_t));
}

/**
 * write_image - Write an image into a PPM file
 *
 * @img:  Pointer to the image structure to write into a file
 * @file: Path to the file to write the image into
 */
void write_image(img_t const *img, char const *file)
{
	FILE *f;
	size_t i;

	f = fopen(file, "w");
	fprintf(f, "P6\n %lu %lu 255\n", img->w, img->h);
	for (i = 0; i < img->w * img->h; i++)
		fprintf(f, "%c%c%c", img->pixels[i].r, img->pixels[i].g, img->pixels[i].b);
	fclose(f);
}

/**
 * main - Entry point
 *
 * @ac: Arguments counter
 * @av: Arguments vector
 *
 * Return: EXIT_SUCCESS upon success, error code upon failure
 */
int main(int ac, char **av)
{
	img_t img;
	img_t img_blur;
	kernel_t kernel;
	size_t i;

	if (ac < 3)
	{
		printf("Usage: %s image.ppm kernel.knl\n", av[0]);
		return (EXIT_FAILURE);
	}

	load_image(&img, av[1]);
	img_copy(&img_blur, &img);
	load_kernel(&kernel, av[2]);

	/* Execute blur */
	blur_image(&img_blur, &img, &kernel);

	write_image(&img_blur, "output.ppm");

	/* Cleanup */
	free(img.pixels);
	free(img_blur.pixels);
	for (i = 0; i < kernel.size; i++)
		free(kernel.matrix[i]);
	free(kernel.matrix);

	return (EXIT_SUCCESS);
}
