
#include "splash_scr.h"

static unsigned int _frame;	/**< The internal frame counter to use when rendering frames. */

u16 *_pal_buff1;
u16 *_pal_buff2;

void splash_set_pal1(u16 *pal)
{
	_pal_buff1 = pal;
}

void splash_set_pal2(u16 *pal)
{
	_pal_buff2 = pal;
}

void splash_Update()
{
	_frame++;
	(*splash_screen_eff)(); // Perform title screen effect
}

// Create a screen 'fade out' effect by first fading the palette to gray, then black
void splash_FadeOut()
{
	if ((_frame % 60) == 0) {
		dprintf("Fading to black\n");
		FadeToBlack(60);
	}

}

void splash_Pulse_Title()
{
	// Switch between fading to the original palette colours and fading to a different palette
	if ((_frame % 90) == 45) {
		dprintf("Fading to palette 2\n");
		FadeToPalette(_pal_buff2, 60);
	} if ((_frame % 90) == 0) {
		dprintf("Fading to palette 1\n");
		FadeToPalette(_pal_buff1, 60);
	}
}

void splash_PulseGray()
{
	// Switch between fading to the original palette colours and fading to a greyscale palette
	if ((_frame % 60) == 0) {
		dprintf("Fading to grayscale palette\n");
		FadeToGrayScale(4, 60);
	} if ((_frame % 60) == 30) {
		dprintf("Fading back to palette 1\n");
		FadeToPalette(_pal_buff1, 60);
	}
}