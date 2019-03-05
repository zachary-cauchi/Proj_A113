
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
		FadeToBlack(60);
	}

}

void splash_pulse_title()
{
	// Switch between fading to the original palette colours and fading to a different palette
	if ((frame & 45) == 0) {
		FadeToPalette(_pal_buff2, 60);
	} if ((frame & 90) == 0) {
		FadeToPalette(_pal_buff1, 60);
	}
}

void splash_pulse_gray()
{
	// Switch between fading to the original palette colours and fading to a greyscale palette
	if ((frame & 60) == 0) {
		FadeToGrayScale(4, 60);
	} if ((frame & 120) == 0) {
		FadeToPalette(_pal_buff1, 60);
	}
}