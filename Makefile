#
#draw.o : draw.c
#	arm-linux-gnueabi-gcc -c draw.c
#
#rk_fb_test.o: rk_fb_test.c draw.h
#	arm-linux-gnueabi-gcc -c rk_fb_test.c
#
#rk_fb_test : draw.o rk_fb_test.o
#	arm-linux-gnueabi-gcc draw.o rk_fb_test.o -o rk_fb_test
#
#.PHONY: clean
#
#clean :
#	rm -f *.o rk_fb_test


# Mode 1
#FB_Test: draw.c rk_fb_test.c
#	arm-linux-gnueabi-gcc draw.c rk_fb_test.c -o FB_Test
#

# Mode 2
cc = arm-linux-gnueabi-gcc
target = FB_Demo
source = draw.c rk_fb_test.c

$(target): $(source)
	$(cc) $(source) -static -o $(target)

.PHONY: clean

clean :
	rm -f *.o $(target)
