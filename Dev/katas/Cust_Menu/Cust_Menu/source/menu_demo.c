//---------------------------------------------------------------------------------
// GBA PCXView sample code for devkitARM - http://www.devkit.tk
// Suitable for a splash screen for a game
//---------------------------------------------------------------------------------
#include "gba_video.h"
#include "gba_systemcalls.h"
#include "gba_input.h"
#include "gba_interrupt.h"
#include "mappy.h"
#include "pcx.h"
#include "fade.h"

//---------------------------------------------------------------------------------
// header for binary data generated by bin2o macro in makefile
//---------------------------------------------------------------------------------
#include "title_pcx.h"
#include "title_pulse_pcx.h"

#include "splash_scr.h"

//---------------------------------------------------------------------------------
// storage space for palette data
//---------------------------------------------------------------------------------
u16 PaletteBuffer[256];
u16 PulsePaletteBuffer[256];

// System variables
u32 keyFrame;
u32 pressed;
u32 released;
u32 held;

// Method declarations
void (*screen_eff)();
void key_reads();
void handleControls();

//---------------------------------------------------------------------------------
// Program entry point
//---------------------------------------------------------------------------------
int main(void)
//---------------------------------------------------------------------------------
{
	// Set up the interrupt handlers
	irqInit();
	// Enable Vblank Interrupt to allow VblankIntrWait
	irqEnable(IRQ_VBLANK);

	SetMode(MODE_4 | BG2_ON);		// screen mode & background to display

	// Load all necessary image data
	DecodePCX(title_pcx, (u16*)VRAM, PaletteBuffer);
	DecodePCX(title_pulse_pcx, (u16*)VRAM, PulsePaletteBuffer);

	screen_eff = splash_pulse_gray;	// Set the initial screen mode
	setPal1(&PaletteBuffer);		// Load the first palette
	setPal2(&PulsePaletteBuffer);	// Load the second palette

	dprintf("Initialised application with palettes %p, %p\n", &PaletteBuffer, &PulsePaletteBuffer);

	while (1)
	{
		VBlankIntrWait();
		update();
		// Process key inputs
		key_reads();
		handleControls();
		(*screen_eff)(); // Perform title screen effect
	}
}

void key_reads() {
	if ((keyFrame & 7) == 0) {
		// Check for key updates and record the results to a buffer for later use
		scanKeys();
		pressed = keysDown();
		held = keysHeld();
		released = keysUp();
	}
}

void handleControls() {
	if (pressed & KEY_UP && screen_eff != splash_pulse_gray)			screen_eff = splash_pulse_gray;
	else if (pressed & KEY_RIGHT && screen_eff != splash_pulse_title)	screen_eff = splash_pulse_title;
	else if (pressed & KEY_DOWN && screen_eff != splash_fade_out)		screen_eff = splash_fade_out;
}
