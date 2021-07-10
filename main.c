#include "bitmap.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
	//printf("Hello");

	bitmap bmp;// = malloc(sizeof(bmp));
	load_bitmap("F:\\Secondary\\Matthew\\C\\art-files\\build\\Debug\\img.bmp", &bmp);
	//print_bitmap_info(&bmp);
	//printf("\n\n%d", bmp.pixel_data);
	save_bitmap("F:\\Secondary\\Matthew\\C\\art-files\\build\\Debug\\a2.bmp", &bmp);

	free_bitmap(&bmp);


	return 0;
}
