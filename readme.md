# Vino Art Files
art-files is a pure C library that provides a simple api for reading, writing, and creating image/audio files.

## Supported File Types
| Name | Bit Depths | Compression |
| ----- | ----- | ----- |
| Bitmap | 24 | BI_RGB |

## Example
```c
#include "bitmap.h"

int main(void) {
	bitmap bmp;
	load_bitmap("example.bmp", &bmp);
	print_bitmap_info(&bmp);

	return 0;
}
```

## Contributing
Please feel free to make pull requests. The library is still in its very early stages in is subject to change. Open up an issue if you have any questions or feature suggestions.
