
#include "splash_scr.h"

unsigned int frame;

u16 *_pal_buff1;
u16 *_pal_buff2;

void setPal1(u16 *pal)
{
	_pal_buff1 = pal;
}

void setPal2(u16 *pal)
{
	_pal_buff2 = pal;
}

void update()
{
	frame++;
}

// Create a screen 'fade out' effect by first fading the palette to gray, then black
void splash_fade_out()
{
	if ((frame & 60) == 0) {
		dprintf("Fading to black\n");
		FadeToBlack(60);
	}

}

void splash_pulse_title()
{
	// Switch between fading to the original palette colours and fading to a different palette
	if ((frame & 45) == 0) {
		dprintf("Fading to palette 2\n");
		FadeToPalette(_pal_buff2, 60);
	} if ((frame & 90) == 0) {
		dprintf("Fading to palette 1\n");
		FadeToPalette(_pal_buff1, 60);
	}
}

void splash_pulse_gray()
{
	// Switch between fading to the original palette colours and fading to a greyscale palette
	if ((frame & 60) == 0) {
		dprintf("Fading to grayscale palette\n");
		FadeToGrayScale(4, 60);
	} if ((frame & 120) == 0) {
		dprintf("Fading back to palette 1\n");
		FadeToPalette(_pal_buff1, 60);
	}
}