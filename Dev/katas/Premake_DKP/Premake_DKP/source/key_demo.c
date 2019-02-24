//
// key_demo.c
//
// demonstating key states (and minor palette animation)
//
// (20030927 - 20060922, cearn)

#include "gba_pic.h"

#include "toolbox.h"
#include "input.h"
#include "gba_base.h"
#include "gba_input.h"
#include "gba_video.h"

#define BTN_PAL  5
#define CLR_UP   RGB15(27,27,29)

int main()
{
	int ii;
	u32 btn;
	u32 pressed;
	u32 held;
	u32 released;
	COLOR clr;
	int frame=0;

	// Copy the background image and palette to their respective locations
	memcpy((COLOR*) VRAM, gba_picBitmap, gba_picBitmapLen);
	memcpy(BG_PALETTE, gba_picPal, gba_picPalLen);

	// Enable bitmap mode 4 and background 2
	SetMode(MODE_4 | BG2_ON);

	while(1)
	{

		vid_vsync();

		// slowing down polling to make the changes visible
		if ((frame & 7) == 0) {

			// Check for key updates and record the results to a buffer for later use
			scanKeys();
			pressed = keysDown();
			held = keysHeld();
			released = keysUp();

		}

		// Iterate over all buttons and check their states
		// Depending on the state, change that buttons palette colour to the state colour
		// red for pressed, yellow for released, lime for held, and the default for any other state
		for (ii = 0; ii < KI_MAX; ii++)
		{
			clr = 0;
			// Select the button to read by bit-shifting (see gba_input.h for reference)
			btn = 1 << ii;
			if (pressed & btn)
				clr = CLR_RED;
			else if (released & btn)
				clr = CLR_YELLOW;
			else if (held & btn)
				clr = CLR_LIME;
			else
				clr = CLR_UP;
			pal_bg_mem[BTN_PAL + ii] = clr;
		}

		frame++;
		
	}

	return 0;
}