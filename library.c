#include "library.h"

//You can use typedef to make a color type, color_t, that is an unsigned 16-bit value. You can also make a macro or function to encode a color_t from three RGB values using bit shifting and masking to make a single 16-bit number.
//The machine that QEMU is configured to emulate is in a basic 640x480 mode with 16-bit color. This means that there are 640 pixels in the x direction (0 is leftmost and 639 is rightmost) and 480 pixels in the y direction (0 is topmost, 479 is bottommost). In 16-bit color, we store the intensity of the three primary colors of additive light, Red, Green, and Blue, all packed together into a single 16-bit number. Since 16 isn't evenly divisible by three (RGB), we devote the upper 5 bits to storing red intensity (0-31), the middle 6 bits to store green intensity (0-63), and the low order 5 bits to store blue intensity (0-31). 

struct fb_var_screeninfo resStruct;
struct fb_fix_screeninfo depStruct;


unsigned short *init_graphics()
{

	//open the graphics 
	///dev/fb0
	//Since it appears to be a file, it can be opened using the open() syscall.
	int fBuffer = open("/dev/fb0", O_RDWR);

	


	//To get the screen size and bits per pixels, we can use a special system call: ioctl. This system call is used to query and set parameters for almost any device connected to the system. You pass it a file descriptor and a particular number that represents the request you're making of that device. We will use two requests: FBIOGET_VSCREENINFO and FBIOGET_FSCREENINFO. The first will give back a struct fb_var_screeninfo that will give the virtual resolution. The second will give back a struct fb_fix_screeninfo from which we can determine the bit depth. The total size of the mmap()'ed file would be the yres_virtual field of the first struct multiplied by the line_length field of the second.

	int vRet = ioctl(fBuffer, FBIOGET_VSCREENINFO, &resStruct);
	int fRet = ioctl(fBuffer, FBIOGET_FSCREENINFO, &depStruct);
	int virt = (int)(resStruct.yres_virtual); 
	int dep = (int)(depStruct.line_length);

	//res.yres_virtual X dep.line_Length
	int bytes = virt * dep;
	

	//The mmap() system call takes a file descriptor from open (and a bunch of other parameters) and returns a void *, an address in our address space that now represents the contents of the file. That means that we can use pointer arithmetic or array subscripting to set each individual pixel, provided we know how many pixels there are and how many bytes of data is associated with each pixel.
	//must use the MAP_SHARED parameter to mmap() because other parts of the OS want to use the framebuffer too.

	unsigned short *bufAdr =(unsigned short *)mmap(NULL, bytes, PROT_WRITE, MAP_SHARED, fBuffer, 0);


	//use the ioctl system call to disable keypress echo (displaying the keys as you're typing on the screen automatically) and buffering the keypresses. The commands we will need are TCGETS and TCSETS. These will yield or take a struct termios that describes the current terminal settings. You will want to disable canonical mode by unsetting the ICANON bit and disabling ECHO by forcing that bit to zero as well. 
	
	flags_off();

	return bufAdr;
}
void exit_graphics()
{
	//close() and munmap() 


	//make an ioctl() to reenable key press echoing and buffering as described above.
	flags_on();
}
void clear_screen()
{
	//ANSI escape codes are a sequence of characters that are not meant to be displayed as text but rather interpreted as commands to the terminal. We can print the string "\033[2J" to tell the terminal to clear itself.
	const char msg[] = "\033[2J";
	write(STDOUT_FILENO, msg, sizeof(msg)-1);
}
char getkey()
{

	char in;
	//To make games, we probably want some sort of user input. We will use key press input and we can read a single character using the read() system call. However, read() is blocking and will cause our program to not draw unless the user has typed something. Instead, we want to know if there is a keypress at all, and if so, read it. select()
	fd_set descSet;
   	struct timeval timeVal;
   	int retVal;

   	FD_ZERO(&descSet);
   	FD_SET(0, &descSet);

   	timeVal.tv_sec = 5;
   	timeVal.tv_usec = 0;

   	retVal = select(STDIN_FILENO+1, &descSet, NULL, NULL, &timeVal);

   	if (retVal>0)
       	read(0, &in, sizeof(in));
    return in;
}
void sleep_ms(int ms)
{
	struct timespec time;
	time.tv_sec=0;
	time.tv_nsec = ms*1000000;
	//We will use the system call nanosleep() to make our program sleep between frames of graphics being drawn. From its name you can guess that it has nanosecond precision, but we don't need that level of granularity. We will instead sleep for a specified number of milliseconds and just multiply that by 1,000,000.
	//We do not need to worry about the call being interrupted and so the second parameter to nanosleep() can be NULL.
	nanosleep(&time, NULL);

}
void draw_pixel(unsigned short *bufAdr, int coordX, int coordY, Color col)
{
	//This is the main drawing code, where the work is actually done. We want to set the pixel at coordinate (x, y) to the specified color. You will use the given coordinates to scale the base address of the memory-mapped framebuffer using pointer arithmetic. The frame buffer will be stored in row-major order, meaning that the first row starts at offset 0 and then that is followed by the second row of pixels, and so on.
	int offset = (coordY * resStruct.xres_virtual) + coordX;
	bufAdr[offset] = col.builtColor;
}
void draw_rect(unsigned short *bufAdr, int coordX, int coordY, int width, int height, Color col)
{
	//Using draw_pixel, make a rectangle with corners (x1, y1), (x1+width,y1), (x1+width,y1+height), (x1, y1+height).
	int i;

	for(i = 0; i < width; i++)
	{ 
		draw_pixel(bufAdr, coordX++, coordY, col);
	}

	for(i = 0; i < height; i++)
	{ 
		draw_pixel(bufAdr, coordX, coordY++, col);
	}

	for(i = 0; i < width; i++)
	{ 
		draw_pixel(bufAdr, coordX--, coordY, col);
	}

	for(i = 0; i < height; i++)
	{ 
		draw_pixel(bufAdr, coordX, coordY--, col);
	}
}
void draw_circle(unsigned short *bufAdr, int coordX, int coordY, int radius, Color col)
{
	//Use the midpoint circle algorithm to draw a circle at (x, y) with radius r. 
	int x = radius-1;
    int y = 0;
    int dx = 1;
    int dy = 1;
    int err = dx - (radius << 1);

    while (x >= y)
    {
    	draw_pixel(bufAdr, coordX + x, coordY + y, col);
    	draw_pixel(bufAdr, coordX + y, coordY + x, col);
    	draw_pixel(bufAdr, coordX - y, coordY + x, col);
    	draw_pixel(bufAdr, coordX - x, coordY + y, col);
    	draw_pixel(bufAdr, coordX - x, coordY - y, col);
    	draw_pixel(bufAdr, coordX - y, coordY - x, col);
    	draw_pixel(bufAdr, coordX + y, coordY - x, col);
    	draw_pixel(bufAdr, coordX + x, coordY - y, col);

        if (err <= 0)
        {
            y++;
            err += dy;
            dy += 2;
        }
        
        if (err > 0)
        {
            x--;
            dx += 2;
            err += dx - (radius << 1);
        }
    }

}
Color getColor(unsigned short r, unsigned short g, unsigned short b)
{
	Color color;
	color.red = r<<11;
	color.green = g<<5;
	color.blue = b;
	color.builtColor = 0x0;
	color.builtColor = color.red | color.builtColor;
	color.builtColor = color.green | color.builtColor;
	color.builtColor = color.blue | color.builtColor;
	return color;

}
void flags_off()
{
  struct termios state;
  ioctl(0, TCGETS, (char *)&state);
  state.c_lflag &= ~ECHO;
  state.c_lflag &= ~ICANON;
  ioctl(0, TCSETS, (char *)&state);
}

void flags_on()
{
  struct termios state;
  ioctl(0, TCGETS, (char *)&state);
  state.c_lflag |= ECHO;
  state.c_lflag &= ~ICANON;
  ioctl(0, TCSETS, (char *)&state);
}




