/*
	image.h
	Matthew Kleitz, 2021
*/
#pragma once
typedef enum {
	RGB,
	BGR
} color_order;

typedef struct {
	char r;
	char g;
	char b;
} vcolor;

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