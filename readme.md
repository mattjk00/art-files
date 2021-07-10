# Vino Art Files
art-files is a pure C library that provides a simple api for reading, writing, and creating image/audio files.

## Supported File Types
| Name | Bit Depths | Compression |
| ----- | ----- | ----- |
| BMP | 24 | BI_RGB |
| PNG | 🚧 | 🚧 |
| JPG | 🚧 | 🚧 |
| WAV | 🚧 | 🚧 |
| MP3 | 🚧 | 🚧 |
| AIF | 🚧 | 🚧 |

# Examples
# Load Bitmap
```c
#include "bitmap.h"

int main(void) {
	bitmap bmp;
	load_bitmap("example.bmp", &bmp);
	print_bitmap_info(&bmp);

	free_bitmap(bmp);

	return 0;
}
```

# Create Bitmap
```c
#include "bitmap.h"
#include "image.h"

int main(void) {
	bitmap bmp;
	vimage image;

	// Allocate memory for 1000x1000 image and fill the background with a solid color.
	alloc_image(&image, 1000, 1000);
	vfill(image, vGUNMETAL);

	// Create a Bitmap file from the pixel data and save it.
	bitmap_from_image(image, &bmp);
	save_bitmap("test.bmp", &bmp);

	// Free Resources
	free_image(image);
	free_bitmap(bmp);

	return 0;
}
```
vimage is a generic data type for manipulating pixel data. Any supported image file type can then be created from a vimage struct. vimage abstracts the image data from a file type so that
you can modify and generate images without worrying about a file format.

## Contributing
Please feel free to make pull requests. The library is still in its very early stages in is subject to change. Open up an issue if you have any questions or feature suggestions.
