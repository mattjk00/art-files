/*
	bitmap.h
	Matthew Kleitz, 2021
*/
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "image.h"

typedef enum {
	BI_RGB,
	BI_RLE8,
	BI_RLE4
} bmp_compression;

typedef struct {
	// Header - 14 Bytes
	unsigned char signature[2];
	unsigned long file_size;
	//unsigned long reserved[4];
	unsigned long data_offset;

	// Info Header - 40 Bytes
	//const long size = 40;
	unsigned long width;
	unsigned long height;
	unsigned short planes;
	unsigned short bits_per_pixel;
	unsigned long compression;
	unsigned long image_size;
	unsigned long x_pixels_per_meter;
	unsigned long y_pixels_per_meter;
	unsigned long colors_used;
	unsigned long important_colors;

	// Color Table
	unsigned char* tbl_red;
	unsigned char* tbl_green;
	unsigned char* tbl_blue;
	bool has_color_table;
	
	// Pixel Data
	char* pixel_data;
} bitmap;

/// <summary>
/// Loads a Bitmap File.
/// </summary>
/// <param name="file_path">File path of the bitmap.</param>
/// <param name="bmp">Structure to load data into.</param>
void load_bitmap(const char* file_path, bitmap* bmp);

/// <summary>
/// Releases allocated bitmap memory.
/// </summary>
/// <param name="bmp"></param>
void free_bitmap(bitmap bmp);

void bitmap_from_image(vimage img, bitmap* bmp);
void image_from_bitmap(bitmap bmp, vimage* img);
void set_defaults_bitmap(bitmap* bmp);

/// <summary>
/// Saves a Bitmap file.
/// </summary>
/// <param name="file_path">File path to write to.</param>
/// <param name="bmp">Bitmap data to write.</param>
void save_bitmap(const char* file_path, bitmap* bmp);

/// <summary>
/// Prints bitmap information to the console.
/// </summary>
/// <param name="bmp"></param>
void print_bitmap_info(bitmap bmp);

/// <summary>
/// Takes a four char buffer and converts it into a long. Uses memcpy.
/// </summary>
/// <param name="l">Long pointer to write data into.</param>
/// <param name="bytes">Bytes that contain long data.</param>
/// <returns></returns>
int pack_in_long(unsigned long* l, char bytes[4]);

/// <summary>
/// Takes a two char buffer and converts it into a short. Uses memcpy.
/// </summary>
/// <param name="s">Short pointer to write data into.</param>
/// <param name="bytes">Bytes that contain short data.</param>
/// <returns></returns>
int pack_in_short(unsigned short* s, char bytes[2]);

/// <summary>
/// Takes a long and converts it into a 4 byte buffer. Little-endian.
/// </summary>
/// <param name="b">Buffer to write data.</param>
/// <param name="l">Long to write.</param>
/// <returns></returns>
int unpack_long(char* b, const long l);

/// <summary>
/// Takes a short and converts it into a 2 byte buffer. Little-endian.
/// </summary>
/// <param name="b">Buffer to write data.</param>
/// <param name="s">Short to write.</param>
/// <returns></returns>
int unpack_short(char* b, const short s);

void load_bitmap(const char* file_path, bitmap* bmp) {
	FILE* fp;
	char hbuf[54];
	
	fp = fopen(file_path, "r");
	if (fp == NULL) {
		printf("Unable to open file! %s", file_path);
		return;
	}
	fgets(hbuf, 54, fp);
	
	// Reading the header
	bmp->signature[0] = hbuf[0];
	bmp->signature[1] = hbuf[1];
	if (strcmp(bmp->signature, "BM") == 0) {
		printf("Not a valid bitmap!");
		return;
	}

	pack_in_long(&bmp->file_size, (char[4]) { hbuf[2], hbuf[3], hbuf[4], hbuf[5] });
	//printf("Found bitmap. Size: %d bytes", bmp->file_size);
	pack_in_long(&bmp->data_offset, (char[4]) { hbuf[10], hbuf[11], hbuf[12], hbuf[13] });

	// Reading the Info Header
	pack_in_long(&bmp->width, (char[4]) { hbuf[18], hbuf[19], hbuf[20], hbuf[21] });
	pack_in_long(&bmp->height, (char[4]) { hbuf[22], hbuf[23], hbuf[24], hbuf[25] });
	pack_in_short(&bmp->planes, (char[2]) { hbuf[26], hbuf[27] });
	pack_in_short(&bmp->bits_per_pixel, (char[2]) { hbuf[28], hbuf[29] });
	pack_in_long(&bmp->compression, (char[4]) { hbuf[30], hbuf[31], hbuf[32], hbuf[33] });
	pack_in_long(&bmp->image_size, (char[4]) { hbuf[34], hbuf[35], hbuf[36], hbuf[37] });

	pack_in_long(&bmp->x_pixels_per_meter, (char[4]) { hbuf[38], hbuf[39], hbuf[40], hbuf[41] });
	pack_in_long(&bmp->y_pixels_per_meter, (char[4]) { hbuf[42], hbuf[43], hbuf[44], hbuf[45] });
	pack_in_long(&bmp->colors_used, (char[4]) { hbuf[46], hbuf[47], hbuf[48], hbuf[49] });
	pack_in_long(&bmp->important_colors, (char[4]) { hbuf[50], hbuf[51], hbuf[52], hbuf[53] });

	bmp->has_color_table = (bmp->bits_per_pixel < 8);
	if (bmp->has_color_table) {
		// Do Stuff
	}
	else {
		bmp->tbl_blue = NULL;
		bmp->tbl_green = NULL;
		bmp->tbl_red = NULL;
	}
	
	// Allocate memory for the pixel data.
	long n = bmp->image_size;
	char* idata = malloc(n);
	if (idata == NULL) {
		printf("Memory Allocation Failed.");
		return;
	}

	// Read pixel data
	fseek(fp, bmp->data_offset, SEEK_SET);
	size_t len_read = fread(idata, sizeof(char), n, fp);
	bmp->pixel_data = idata;

	fclose(fp);
}

void save_bitmap(const char* file_path, bitmap* bmp) {
	FILE* fp;
	char lbuf[4];
	char sbuf[2];
	size_t bw = 0;

	fp = fopen(file_path, "wb");
	
	// WRITE HEADER
	char sig[2] = "BM";
	bw += fwrite(sig, sizeof(char), sizeof(sig), fp);
	
	unpack_long(lbuf, bmp->file_size);
	bw += fwrite(lbuf, sizeof(char), sizeof(lbuf), fp);


	unpack_long(lbuf, 0); // reserved 0.
	bw += fwrite(lbuf, sizeof(char), sizeof(lbuf), fp);
	
	unpack_long(lbuf, bmp->data_offset);
	bw += fwrite(lbuf, sizeof(char), sizeof(lbuf), fp);

	// WRITE INFO HEADER
	unpack_long(lbuf, 40);
	bw += fwrite(lbuf, sizeof(char), sizeof(lbuf), fp);

	unpack_long(lbuf, bmp->width);
	bw += fwrite(lbuf, sizeof(char), sizeof(lbuf), fp);

	unpack_long(lbuf, bmp->height);
	bw += fwrite(lbuf, sizeof(char), sizeof(lbuf), fp);

	unpack_short(sbuf, bmp->planes);
	bw += fwrite(sbuf, sizeof(char), sizeof(sbuf), fp);

	unpack_short(sbuf, bmp->bits_per_pixel);
	bw += fwrite(sbuf, sizeof(char), sizeof(sbuf), fp);

	unpack_long(lbuf, bmp->compression);
	bw += fwrite(lbuf, sizeof(char), sizeof(lbuf), fp);

	unpack_long(lbuf, bmp->image_size);
	bw += fwrite(lbuf, sizeof(char), sizeof(lbuf), fp);

	unpack_long(lbuf, bmp->x_pixels_per_meter);
	bw += fwrite(lbuf, sizeof(char), sizeof(lbuf), fp);

	unpack_long(lbuf, bmp->y_pixels_per_meter);
	bw += fwrite(lbuf, sizeof(char), sizeof(lbuf), fp);

	unpack_long(lbuf, bmp->colors_used);
	bw += fwrite(lbuf, sizeof(char), sizeof(lbuf), fp);

	unpack_long(lbuf, bmp->important_colors);
	bw += fwrite(lbuf, sizeof(char), sizeof(lbuf), fp);

	if (bmp->has_color_table) {
		// DO STUFF
	}

	bw += fwrite(bmp->pixel_data, sizeof(char), bmp->image_size, fp);
	int a = 0;
	int b = 0;
	fclose(fp);
}

void free_bitmap(bitmap bmp) {
	free(bmp.pixel_data);
	free(bmp.tbl_blue);
	free(bmp.tbl_green);
	free(bmp.tbl_red);
}

int pack_in_long(unsigned long* l, char bytes[4]) {
	return memcpy(l, bytes, sizeof(long));
}

int pack_in_short(unsigned short* s, char bytes[2]) {
	return memcpy(s, bytes, sizeof(short));
}

int unpack_long(char* b, const long l) {
	b[0] = l & 0xFF;
	b[1] = (l >> 8) & 0xFF;
	b[2] = (l >> 16) & 0xFF;
	b[3] = (l >> 24) & 0xFF;
	return 0;
}

int unpack_short(char* b, const short s) {
	b[0] = s & 0xFF;
	b[1] = (s >> 8) & 0xFF;
	return 0;
}

void print_bitmap_info(bitmap bmp) {
	printf("\n---BMP INFO------\n");
	printf("File Size: %d bytes\n", bmp.file_size);
	printf("%dpx * %dpx\n", bmp.width, bmp.height);
	printf("Bit Depth: %d\n", bmp.bits_per_pixel);
	printf("Compression: %d\n", bmp.compression);
	printf("---END INFO------\n");
}

void set_defaults_bitmap(bitmap* bmp) {
	bmp->bits_per_pixel = 24;
	bmp->data_offset = 54;
	bmp->planes = 1;
	bmp->compression = 0;
	bmp->x_pixels_per_meter = 1;
	bmp->y_pixels_per_meter = 1;
	bmp->colors_used = 0;
	bmp->important_colors = 0;
	bmp->tbl_blue = NULL;
	bmp->tbl_green = NULL;
	bmp->tbl_red = NULL;
}

void bitmap_from_image(vimage img, bitmap* bmp) {
	set_defaults_bitmap(bmp);
	bmp->width = img.width;
	bmp->height = img.height;
	int padding = img.width % 4;
	int n = (img.width + padding) * img.height;
	int imgsize = n * 3;
	bmp->pixel_data = malloc(imgsize);
	char pixbuf[3];
	for (int y = 0; y < img.height; y++) {
		int offset = ( y * ((img.width * 3) + padding));
		for (int i = 0; i < img.width * 3; i += 3) {
			pack_color(&pixbuf, img.pixels[(offset + i) / 3], BGR);
			bmp->pixel_data[offset + i] = pixbuf[0];
			bmp->pixel_data[offset + i + 1] = pixbuf[1];
			bmp->pixel_data[offset + i + 2] = pixbuf[2];
		}
		for (int p = 0; p < padding; p++) {
			bmp->pixel_data[offset + img.width*3 + p] = 0;
		}
	}
	
	bmp->image_size = imgsize;
	bmp->file_size = imgsize + bmp->data_offset;
}

void image_from_bitmap(bitmap bmp, vimage* img) {
	alloc_image(img, bmp.width, bmp.height);

	int offset = img->width % 4;
	int pxp = 0;
	int n = bmp.width * bmp.height;

	/*char buf[3];
	int xscan = 0;
	for (int i = 0; i < n*3; i++) {
		/*if (i != 0 && xscan >= bmp.width) {
			printf("Reached end of scanline.");
			xscan = 0;
		}
		if (xscan <= bmp.width && i % 3 == 0) {
			img->pixels[pxp] = new_color(bmp.pixel_data[i+2], bmp.pixel_data[i+1], bmp.pixel_data[i]);
			pxp++;
		}
		xscan++;
		if (xscan == bmp.width + offset) {
			xscan = 0;
		}
	}*/

	for (int y = 0; y < bmp.height; y++) {
		for (int x = 0; x < ( bmp.width * 3); x += 3) {
			img->pixels[pxp] = new_color(
				bmp.pixel_data[y * bmp.width*3 + x+2],
				bmp.pixel_data[y * bmp.width*3 + x + 1],
				bmp.pixel_data[y * bmp.width*3 + x]
			);
			pxp++;
		}
	}
}