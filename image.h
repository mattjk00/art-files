/*
	image.h
	Matthew Kleitz, 2021
*/
#pragma once
#include <stdlib.h>


typedef enum {
	RGB,
	BGR
} color_order;

typedef struct {
	char r;
	char g;
	char b;
} vcolor;

typedef struct {
	vcolor* pixels;
	unsigned long width;
	unsigned long height;
} vimage;

const vcolor vBLACK = { 0,0,0 };
const vcolor vRED = { 255,0,0 };
const vcolor vBLUE = { 0,0,255 };
const vcolor vGREEN = { 0,255,0 };

void alloc_image(vimage* img, unsigned long width, unsigned long height);
void free_image(vimage img);
void vdraw(vimage* img, unsigned long x, unsigned long y, vcolor c);
void vfill(vimage* img, vcolor c);

/// <summary>
/// Writes color data to a buffer
/// </summary>
/// <param name="buf">Buffer to write to</param>
/// <param name="c">Color to write</param>
/// <param name="order">Order to write color in. RGB or BGR</param>
void pack_color(char* buf, vcolor c, color_order order);

/// <summary>
/// Helper method for creating a color structure
/// </summary>
/// <param name="r">Red</param>
/// <param name="g">Green</param>
/// <param name="b">Blue</param>
/// <returns></returns>
vcolor new_color(char r, char g, char b);

void alloc_image(vimage* img, unsigned long width, unsigned long height) {
	size_t n = width * height;
	vcolor* px = (vcolor*)malloc(n* sizeof (vcolor));
	if (px == NULL) {
		perror("Failed to allocate memory for image.");
		return;
	}
	img->width = width;
	img->height = height;
	img->pixels = px;
}

void free_image(vimage img) {
	free(img.pixels);
}

void vdraw(vimage* img, unsigned long x, unsigned long y, vcolor c) {
	y = img->height - y;
	img->pixels[y * img->width + x] = c;
	//img->pixels[5] = c;
}

void vfill(vimage* img, vcolor c) {
	for (int i = 0; i < img->width * img->height; i++) {
		img->pixels[i] = c;
	}
}

void pack_color(char* buf, vcolor c, color_order order) {
	buf[1] = c.g;
	if (order == RGB) {
		buf[0] = c.r;
		buf[2] = c.b;
	}
	else if (order == BGR) {
		buf[0] = c.b;
		buf[2] = c.r;
	}	
}

vcolor new_color(char r, char g, char b) {
	return (vcolor){ r, g, b };
}