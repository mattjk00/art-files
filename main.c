#include "bitmap.h"
#include "image.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(void) {

	time_t t;
	srand((unsigned)time(&t));
	
	vimage image;
	alloc_image(&image, 1000, 1000);
	vfill(image, vGUNMETAL);
	for (int i = 0; i < 5; i++) {
		vdraw_rect(image, rand() % 900, rand() % 900, 25, 25, vCARMINE);
	}
	//vdraw_rect(image, 50, 50, 50, 50, vCLEMENTINE);
	bitmap bmp;
	bitmap_from_image(image, &bmp);
	save_bitmap("F:\\Secondary\\Matthew\\C\\art-files\\build\\Debug\\test.bmp", &bmp);

	free_image(image);
	free_bitmap(bmp);
	//load_bitmap("F:\\Secondary\\Matthew\\C\\art-files\\build\\Debug\\img.bmp", &bmp);
	//save_bitmap("F:\\Secondary\\Matthew\\C\\art-files\\build\\Debug\\a2.bmp", &bmp);
	
	
	return 0;
}
