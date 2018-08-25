#include <stdlib.h> // ads
#include <stdio.h>	// printf
#include <string.h>	// memcpy

#include "draw.h"

struct sBufferInformation buffer;

int drawInit(struct sBufferInformation buf)
{
	memset(&buffer, 0, sizeof(buffer));
	memcpy(&buffer, &buf, sizeof(buf));
}

int drawPixel(unsigned int x, unsigned int y, unsigned int color)
{
	unsigned int offset;
	unsigned char *position;

	offset = (y*buffer.xres + x)*buffer.bpp/8;
	position = (unsigned char *)(buffer.base + offset);
/*
	printf("Draw a dor [%4d, %4d](0x%p) with color [0x%2.2x, 0x%2.2x, 0x%2.2x, 0x%2.2x]\n",
			x, y, position,
			(color >> 8*3) & 0xff, (color >> 8*2) & 0xff,
			(color >> 8*1) & 0xff, (color >> 8*0) & 0xff);
*/
	memcpy(position, &color, sizeof(color));
}

int drawLine(struct position start, struct position end, unsigned int color)
{
	unsigned int i, x, y, xOff, yOff;
	float k, e;
/*
	printf("Draw a line [%4d, %4d] - [%4d, %4d] with color [0x%-2x, 0x%-2x, 0x%-2x, 0x%-2x]\n",
			start.x, start.y, end.x, end.y,
			(color >> 8*3) & 0xff, (color >> 8*2) & 0xff,
			(color >> 8*1) & 0xff, (color >> 8*0) & 0xff);
*/
// void bresenham(int x0, int y0, int x1, int y1) {
	int dx = abs(end.x - start.x);
	int sx = start.x < end.x ? 1 : -1;
	int dy = abs(end.y - start.y), sy = start.y < end.y ? 1 : -1;
	int err = (dx > dy ? dx : -dy) / 2;

	while (start.x != end.x || start.y != end.y) {
		drawPixel ( start.x, start.y, color );
		if (err > -dx) { err -= dy; start.x += sx; }
		if (err <  dy) { err += dx; start.y += sy; }
	}
}

int drawRect(struct position upLeft, struct position lowRight,
			unsigned int color)
{
	unsigned int index, yOffset;

	if ( upLeft.x <= lowRight.x && upLeft.y<= lowRight.y ) {
		yOffset = lowRight.y - upLeft.y;
		for (index = 0; index < yOffset; index++ ) {
			upLeft.y = upLeft.y + 1;
			lowRight.y =  upLeft.y;
			drawLine(upLeft, lowRight, color);
			printf("Info: Draw line[%4d, %4d]-[%4d, %4d]\n",
						upLeft.x, upLeft.y,
						lowRight.x, lowRight.y);
		}
	}
	else {
		printf("Error: Invalible position upLeft[%4d, %4d], lowRight[%4d, %4d]\n",
					upLeft.x, upLeft.y,
					lowRight.x, lowRight.y);
	}
}
