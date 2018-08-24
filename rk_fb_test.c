#include <ctype.h>		// isprint
#include <stdlib.h>		// exit
#include <stdio.h>		// printf
#include <fcntl.h>		// open
#include <stdbool.h>	// bool
#include <sys/mman.h>	// mmap
#include <unistd.h>		// close
#include <string.h>		// strerror 
#include <errno.h>		// errno
#include <sys/ioctl.h>	// ioctl

#include <linux/fb.h>	// fb_var_XXX

#include "draw.h"

struct sBufferInformation gBufferInfo;

struct sOptionalCommand {
	bool isDumpScreen;			/* -d option */
    char **inputFiles;          /* -f option */
    FILE *outFile;
    const char *outFileName;    /* -o option */
};
 
static const char *optString = "df:vh?";

void display_usage( void )
{
    printf( "rk_fb_test - framebuffer demo." );
    /* ... */
    exit( EXIT_FAILURE );
}

void showScreenInfo(struct fb_fix_screeninfo finfo, struct fb_var_screeninfo vinfo)
{
	printf("/******** Screen Information ********/\n"
			"    id           = %s\n"
			"    xres         = %d px\n"
			"    yres         = %d px\n"
			"    xres_virtual = %d px\n"
			"    yres_virtual = %d px\n"
			"    xoffset      = %d px\n"
			"    yoffset      = %d px\n"
			"    bpp          = %d\n"
			"    r            = %2u:%u\n"
			"    g            = %2u:%u\n"
			"    b            = %2u:%u\n"
			"    format       = %d\n"
			"/************************************/\n",
			finfo.id,
			vinfo.xres,
			vinfo.yres,
			vinfo.xres_virtual,
			vinfo.yres_virtual,
			vinfo.xoffset,
			vinfo.yoffset,
			vinfo.bits_per_pixel,
			vinfo.red.offset,   vinfo.red.length,
			vinfo.green.offset, vinfo.green.length,
			vinfo.blue.offset,  vinfo.blue.length,
			vinfo.nonstd);
}


struct sOptionalCommand pareOptionalArg(int argc, char **argv)
{
    int opt = 0;
	struct sOptionalCommand command;

	opt = getopt( argc, argv, optString );
    while( opt != -1 ) {
        switch( opt ) {
            case 'd':
                command.isDumpScreen = true;
                break;
                 
            case 'h':   /* fall-through is intentional */
            case '?':
                display_usage();
                break;
                 
            default:
                /* You won't actually get here. */
                break;
        }
         
        opt = getopt( argc, argv, optString );
    }
    return command;
}

static size_t round_up_to_page_size(size_t size)
{
	size_t remainder;
	long pageSize;
	
	pageSize = sysconf(_SC_PAGESIZE);
	remainder = (size + (pageSize - 1)) & ~(pageSize - 1);
	
	return remainder;
}

int main(int argc, char ** argv)
{
    int fd = -1;
    int ret = -1;
	struct sOptionalCommand cmd;
    char fbNode[]={"/dev/graphics/fb0"};

	/* Pare arg */
	cmd = pareOptionalArg(argc, argv);

    fd = open(fbNode, O_RDWR);
    if (fd < 0) {
        printf("Error: Open failed with file:%s\n", fbNode);
        return -errno;
    }

	/* Get fb_fix_screeninfo */
	struct fb_fix_screeninfo fInfo;
	ret = ioctl(fd, FBIOGET_FSCREENINFO, &fInfo);
	if (ret < 0)
	{
		printf("Error: %d, Failed to get information of [fb_fix_screeninfo]\n", ret);
		return -errno;
	}

	/* Get fb_var_screeninfo */
    struct fb_var_screeninfo vInfo;
    ret = ioctl(fd, FBIOGET_VSCREENINFO, &vInfo);
    if (ret < 0) {
		printf("Error: %d, Failed to get information of [fb_var_screeninfo]\n", ret);
        return -errno;
    }

	/* Show the information of screen */
	if (cmd.isDumpScreen)
		showScreenInfo(fInfo, vInfo);

	/* Map the framebuffer */
	size_t fbSize = round_up_to_page_size(fInfo.line_length * vInfo.yres_virtual);
	void* vaddr = mmap(0, fbSize, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if (vaddr == MAP_FAILED)
	{
		printf("Error: Failed to mapping the framebuffer (%s)", (char *)strerror(errno));
		return -errno;
	}

//	printf("Buffer virtual address: 0x%p\n", vaddr);	
	/* Clear fb */
	memset(vaddr, 0, fbSize);

	gBufferInfo.xres = vInfo.xres;
	gBufferInfo.yres = vInfo.yres;
	gBufferInfo.bpp  = vInfo.bits_per_pixel; 	
	gBufferInfo.base = (char *)vaddr;
	drawInit(gBufferInfo);

	/* A-[31:24], B-[23:16], G-[15:8], R-[7:0] */
	unsigned int color = 0x000000ff;
	struct position start = { 0 , 0  };
	struct position end   = { vInfo.xres/2, vInfo.yres/2 };
	
	drawLine(start, end, color);

	munmap(vaddr, fbSize);
    close(fd);
	printf("Info: rk_fb test finish.\n");
	return 0;
}
