#include "bitmap.h"
#include "image.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(void) {

	bitmap bmp;
	vimage image;
	alloc_image(&image, 480, 480);
	vfill(&image, vBLUE);
	for (int i = 1; i < 460; i++) {
		vdraw(&image, i, 100 + sinf(i) * 3, vRED);
		vdraw(&image, i, 200 + sinf(i)*3, vGREEN);
	}
	bitmap_from_image(image, &bmp);
	free_image(image);
	print_bitmap_info(&bmp);
	save_bitmap("F:\\Secondary\\Matthew\\C\\art-files\\build\\Debug\\test.bmp", &bmp);

	//load_bitmap("F:\\Secondary\\Matthew\\C\\art-files\\build\\Debug\\img.bmp", &bmp);
	//save_bitmap("F:\\Secondary\\Matthew\\C\\art-files\\build\\Debug\\a2.bmp", &bmp);

	free_bitmap(bmp);
	return 0;
}
