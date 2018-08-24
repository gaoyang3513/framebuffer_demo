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
	xOff = end.x - start.x;
	yOff = end.y - start.y;
	k = yOff/xOff;
	e = -0.5;
	
	x = start.x;
	y = start.y; 

    for ( i = 0; i < yOff; i++ ) {
		drawPixel ( x, y, color ); 
        x = x + 1;
		e = e + k; 
        if (e >= 0) { 
			y = y + 1;
			e = e - 1;
		} 
    } 
} 
