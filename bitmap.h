/*
	bitmap.h
	Matthew Kleitz, 2021
*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

enum bmp_compression {
	BI_RGB,
	BI_RLE8,
	BI_RLE4
};

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

void load_bitmap(const char* file_path, bitmap* bmp);
void free_bitmap(bitmap* bmp);
void save_bitmap(const char* file_path, bitmap* bmp);
void print_bitmap_info(bitmap* bmp);
int pack_in_long(unsigned long* l, char bytes[4]);
int pack_in_short(unsigned short* s, char bytes[2]);
int unpack_long(char* b, const long l);
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
	//fgets(ibuf, 40, (FILE*)fp);
	
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
	
	fpos_t pos;
	fgetpos(fp, &pos);
	

	fseek(fp, bmp->data_offset, SEEK_SET);
	long n = bmp->image_size;

	char* idata = malloc(n);
	
	if (idata == NULL) {
		printf("Memory Allocation Failed.");
		return;
	}

	size_t len_read = fread(idata, sizeof(char), n, fp);

	
	bmp->pixel_data = idata;
	//
	fclose(fp);
	//free(idata);
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

void free_bitmap(bitmap* bmp) {
	free(bmp->pixel_data);
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

void print_bitmap_info(bitmap* bmp) {
	printf("\n---BMP INFO------\n");
	printf("File Size: %d bytes\n", bmp->file_size);
	printf("%dpx * %dpx\n", bmp->width, bmp->height);
	printf("Bit Depth: %d\n", bmp->bits_per_pixel);
	printf("Compression: %d\n", bmp->compression);
	printf("---END INFO------\n");
}