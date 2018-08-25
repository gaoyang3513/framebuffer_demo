
struct sBufferInformation {
    unsigned int xres;     	/* visible resolution       */
    unsigned int yres;      
    unsigned char bpp;		/* pexel bits               */
    unsigned char * base;	/* buffer base address		*/
}; 

struct color {
	unsigned char b;    /* glye   */
	unsigned char g;	/* greedn */
	unsigned char r;	/* red    */
	unsigned char a;	/* alpha  */
};

struct position {
	unsigned int x;		/* x offset    */
	unsigned int y;		/* y offset    */
};

int drawInit(struct sBufferInformation buf);
int drawPixel(unsigned int x, unsigned int y, unsigned int color);
int drawLine(struct position start, struct position end, unsigned int color);
int drawRect(struct position upLeft, struct position lowRight,
			unsigned int color);
