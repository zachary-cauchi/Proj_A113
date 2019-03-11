#include "graphics_controller.h"
#include "gba_video.h"
#include "splash_scr.h"

void graphics_LoadImage(u8 *header, u16 *pal_p) {
	DecodePCX(header, (u16 *)VRAM, pal_p);
}

void graphics_TransNewImage(u8 *data, u16 *pal_p) {
	// If in state transition mode, wait until 60 frames have passed
	if (state == 1) {
		if ((++transFrameCount % 60) == 0) {
			// Load the image and set the screen mode to use the 
			graphics_LoadImage(data, pal_p);
			splash_screen_eff = splash_pulse_gray;
			state = 2;
		}
	}
	else if (state == 0) {
		// If this is reached, begin transitioning to a new 
		splash_screen_eff = splash_fade_out;
		state = 1;
		transFrameCount = 0;
	}
}