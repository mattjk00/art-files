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

void alloc_image(vimage* img, unsigned long width, unsigned long height);
void free_image(vimage img);
void vdraw(vimage img, unsigned long x, unsigned long y, vcolor c);
void vfill(vimage img, vcolor c);
void vdraw_rect(vimage img, unsigned long x, unsigned long y, unsigned long w, unsigned long h, vcolor c);

vcolor add_colors(vcolor a, vcolor b);
vcolor mult_colors(vcolor a, vcolor b);
vcolor div_colors(vcolor a, vcolor b);
vcolor sub_colors(vcolor a, vcolor b);

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
	size_t n = (size_t)(width * height);
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

void vdraw(vimage img, unsigned long x, unsigned long y, vcolor c) {
	y = img.height - y;
	img.pixels[y * img.width + x] = c;
	//img->pixels[5] = c;
}

void vfill(vimage img, vcolor c) {
	for (int i = 0; i < img.width * img.height; i++) {
		img.pixels[i] = c;
	}
}

void vdraw_rect(vimage img, unsigned long x, unsigned long y, unsigned long w, unsigned long h, vcolor c) {
	for (int i = y; i < y + h; i++) {
		for (int j = x; j < x + w; j++) {
			vdraw(img, j, i, c);
		}
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

vcolor add_colors(vcolor a, vcolor b) {
	return (vcolor) { a.r + b.r, a.g + b.g, a.b + b.b };
}

vcolor mult_colors(vcolor a, vcolor b) {
	return (vcolor) { a.r * b.r, a.g * b.g, a.b * b.b };
}

vcolor div_colors(vcolor a, vcolor b) {
	return (vcolor) { a.r/ b.r, a.g/ b.g, a.b/ b.b };
}

vcolor sub_colors(vcolor a, vcolor b) {
	return (vcolor) { a.r- b.r, a.g- b.g, a.b- b.b };
}

const vcolor vBLACK = { 0,0,0 };
const vcolor vWHITE = { 255, 255, 255 };
const vcolor vRED = { 255,0,0 };
const vcolor vBLUE = { 0,0,255 };
const vcolor vGREEN = { 0,255,0 };
const vcolor vGRAY = { 200, 200, 200 };
#ifndef BASIC_COLORS
const vcolor vFLAX = { 232, 219, 125 };
const vcolor vDARKPURPLE = { 35, 17, 35};
const vcolor vSTEELTEAL = { 85, 140, 140};
const vcolor vCARMINE = { 145, 23, 31 };
const vcolor vYELLOWGREEN = { 203, 232, 150 };
const vcolor vGUNMETAL = { 27, 36, 50 };
const vcolor vCOPPER = { 183, 109, 104 };
const vcolor vPEWTERBLUE = { 54, 85, 143 };
const vcolor vDARKSLATE = { 64, 55, 110 };
const vcolor vLEMON = { 247, 254, 114 };
const vcolor vMAYA = { 81, 187, 254 };
const vcolor vLIGHTGREEN = { 143, 247, 167 };
const vcolor vCOFFEE = { 67, 54, 51 };
const vcolor vALMOND = { 222, 203, 183 };
const vcolor vNIGHT = { 9, 12, 8 };
const vcolor vPINKLACE = { 252, 221, 242 };
const vcolor vCLEMENTINE = { 240, 113, 103 };
const vcolor vMINT = { 193, 255, 155 };
const vcolor vCREAM = { 245, 255, 198 };
const vcolor vLAVENDER = { 171, 135, 255};
const vcolor vORCHID = { 255, 172, 228 };
const vcolor vFLAME = { 207, 92, 54 };
#endif