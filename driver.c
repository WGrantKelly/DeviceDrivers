#include "library.h"
int player;
int color;
char enter = 'q';
Color col;
unsigned short *graphics;
char pressed;
int coordX;
int coordY;
int main()
{
	printf("%s", "Welcome to the worst game on the planet!");
	printf("%s", "\nMovement: up - w , right - d , left - a , down - s , quit - q");
	printf("%s", "\nPick a player: pixel - 1 , circle - 2 , rectangle - 3: ");
	scanf("%d", &player);
	printf("%s", "\nPick a color: red - 1 , green - 2 , blue - 3: ");
	scanf("%d", &color);
	coordX = 250;
	coordY = 250;
	switch(color)
	{
		case 1:
			col = getColor(0x000f,0x0000,0x0000);
		break;
		case 2:
			col = getColor(0x0000,0x001f,0x00000);
		break;
		case 3:
			col = getColor(0x0000,0x0000,0x000f);	
		break;
	}
	graphics = init_graphics();
	clear_screen();
	switch(player)
	{
		case 1:
			draw_pixel(graphics, coordX, coordY, col);
			do
			{
				pressed = getkey();
				if(pressed == 'w')
				{
					coordY-=25;
				}
				else if(pressed == 'a')
				{
					coordX-=25;
				}
				else if(pressed == 's')
				{
					coordY+=25;
				}
				else if(pressed == 'd')
				{
					coordX+=25;
				}
				clear_screen();
				draw_pixel(graphics, coordX, coordY, col);
				sleep_ms(10);
			}
			while(pressed!='q');
			
			break;
		case 2:
			draw_circle(graphics, coordX, coordY, 25, col);
			do
			{
				pressed = getkey();
				if(pressed == 'w')
				{
					coordY-=25;
				}
				else if(pressed == 'a')
				{
					coordX-=25;
				}
				else if(pressed == 's')
				{
					coordY+=25;
				}
				else if(pressed == 'd')
				{
					coordX+=25;
				}
				clear_screen();
				draw_circle(graphics, coordX, coordY, 25, col);
				sleep_ms(10);
			}
			while(pressed!='q');
			clear_screen();
			exit_graphics();
			break;
		break;
		case 3:
			draw_rect(graphics, coordX, coordY, 50, 25, col);
			do
			{
				pressed = getkey();
				if(pressed == 'w')
				{
					coordY-=25;
				}
				else if(pressed == 'a')
				{
					coordX-=25;
				}
				else if(pressed == 's')
				{
					coordY+=25;
				}
				else if(pressed == 'd')
				{
					coordX+=25;
				}
				clear_screen();
				draw_rect(graphics, coordX, coordY, 50, 25, col);
				sleep_ms(10);
			}
			while(pressed!='q');
			break;	
	}
	clear_screen();
	exit_graphics();
    return 0;
}
