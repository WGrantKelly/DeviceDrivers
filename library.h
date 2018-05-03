#include <stdio.h>
#include <sgtty.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <termios.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
typedef struct Color{
	unsigned short red;
   	unsigned short green;
   	unsigned short blue;
   	unsigned short builtColor;
}Color;
unsigned short *init_graphics();
void exit_graphics();
void clear_screen();
char getkey();
void sleep_ms(int ms);
void draw_pixel(unsigned short *bufAdr, int coordX, int coordY, Color col);
void draw_rect(unsigned short *bufAdr, int coordX, int coordY, int width, int height, Color col);
void draw_circle(unsigned short *bufAdr, int coordX, int coordY, int radius, Color col);
Color getColor(unsigned short r, unsigned short g, unsigned short b);
void flags_off();
void flags_on();

