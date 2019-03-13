#ifndef _splash_scr_h_
#define _splash_scr_h_

#include <gba_types.h>
#include "mappy.h"
#include "fade.h"

void(*splash_screen_eff)();		/**< Pointer to the current splash screen effect to render. */

void splash_set_pal1(u16 *pal); /**< Sets the primary palette buffer for the image to be drawn with. */
void splash_set_pal2(u16 *pal); /**< Sets the secondary palette buffer for the image to be drawn with. */

/**
 * Per-frame updater method for the splash screen controller.
 */
void splash_Update();

/////////////////////////////////////
// Splash screen effects
/////////////////////////////////////

void splash_FadeOut();			/**< Generates a fade-to-black effect. */
void splash_PulseGray();		/**< Generates a pulsing gray effect. */
void splash_Pulse_Title();		/**< Generates a pulsing effect, alternating between two palettes. */

#endif
