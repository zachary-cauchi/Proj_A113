#include "graphics_controller.h"
#include "gba_video.h"
#include "splash_scr.h"

u16 EWRAM_DATA PaletteBuffer[256];
u16 EWRAM_DATA PulsePaletteBuffer[256];

static u8* new_data_p;

void graphics_LoadImage(u8 *header, u16 *pal_p) {
	DecodePCX(header, (u16 *)VRAM, pal_p);
}

void graphics_TransNewImage(u8 *data) {
	if (state == 0) {
		// If this is reached, begin transitioning to a new image
		new_data_p = data;
		splash_screen_eff = splash_FadeOut;
		state = STATE_TRANSITIONING;
		trans_frame_count = 0;
	}
}

static void graphics_State1Update() {
	if ((++trans_frame_count % 60) == 0) {
		// Load the image and set the screen mode to use the
		graphics_LoadImage(new_data_p, &PaletteBuffer);
		splash_screen_eff = splash_PulseGray;
		state = STATE_FINISH_TRANSITION;
	}
}

static void graphics_TransitionUpdate() {
	// If in state transition mode, wait until 60 frames have passed
	if (state == STATE_TRANSITIONING) {
		graphics_State1Update();
	}
}

void graphics_Update() {
	splash_Update();
	graphics_TransitionUpdate();	// If in transition mode, update transition
}