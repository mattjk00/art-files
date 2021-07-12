/*
	vinoimage.h
	Matthew Kleitz, 2021
	Library for the .ivn file format.
*/
#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

/// <summary>
/// FILE STRUCTURE for *.ivn Vino Image files!
/// ---
/// HEADER
/// ---
/// Signature | 2 bytes | 'VN'
/// TableSize | 4 bytes | Size of Color Table
/// ImageSize | 4 bytes | Size of Image Data
/// Compression | 1 bytes | Compression Mode
/// Height | 4 bytes
/// Width | 4 bytes
/// TableAddressSize | 2^n bytes, Where n equals ceil(log2(TableSize)) | Size of addresses to table elements. 
/// ---
/// TABLE
/// ---
/// Table | 3 bytes * TableSize | Color data
/// </summary>

typedef struct {
	// Header - 19 bytes
	unsigned long table_size;
	unsigned long image_size;
	unsigned char compression;
	unsigned long address_size;
	unsigned long height;
	unsigned long width;

	unsigned char* table;
	unsigned char* data;
} vinoimage;

void load_vino_image(const char* file_path, vinoimage* img) {
	FILE* fp;
	char hbuf[20];

	fp = fopen(file_path, "r");
	if (fp == NULL) {
		printf("Unable to open file! %s", file_path);
		return;
	}
	fgets(hbuf, 54, fp);

	// Reading the header
	char sig[2] = { hbuf[0], hbuf[1] };
	if (strcmp(sig, "VN") == 0) {
		printf("Not a valid vino image file!");
		return;
	}
}