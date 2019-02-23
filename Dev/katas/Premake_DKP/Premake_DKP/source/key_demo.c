//
// key_demo.c
//
// demonstating key states (and minor palette animation)
//
// (20030927 - 20060922, cearn)

#include "toolbox.h"
#include "input.h"

#include "gba_pic.h"
#include "gba_input.h"

#define BTN_PAL  5
#define CLR_UP   RGB15(27,27,29)

u32 keyIsPressed(u32 btn) {
	return btn & (u32) keysHeld();
}

u32 keyIsHeld(u32 btn) {
	return btn & (u32) keysDown();
}

u32 keyIsReleased(u32 btn) {
	return btn & (u32) keysUp();
}

int main()
{
	int ii;
	u32 btn;
	COLOR clr;
	int frame=0;

	memcpy(vid_mem, gba_picBitmap, gba_picBitmapLen);
	memcpy(pal_bg_mem, gba_picPal, gba_picPalLen);

	REG_DISPCNT= DCNT_MODE4 | DCNT_BG2 ;

	while(1)
	{
		vid_vsync();

		// slowing down polling to make the changes visible
		if ((frame & 7) == 0) {
			//key_poll();
			scanKeys();
			
		}
		// Iterate over all buttons and check their states
		// Depending on the state, change that buttons palette colour to the state colour
		// red for pressed, yellow for released, lime for held, and the default for any other state
		for(ii=0; ii<KI_MAX; ii++)
		{
			clr=0;
			btn= 1<<ii;
			if(keyIsPressed(btn))
				clr= CLR_RED;
			else if(keyIsReleased(btn))
				clr= CLR_YELLOW;
			else if(keyIsHeld(btn))
				clr= CLR_LIME;
			else
				clr= CLR_UP;
			pal_bg_mem[BTN_PAL+ii]= clr;
		}
		frame++;
	}

	return 0;
}