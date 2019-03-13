#ifndef _graphics_controller_h_
#define _graphics_controller_h_

#include "gba_base.h"
#include "gba_types.h"


////////////////////////////////////
// storage space for palette data
////////////////////////////////////

extern u16 EWRAM_DATA PaletteBuffer[256];		/**< Palette buffer 1 */
extern u16 EWRAM_DATA PulsePaletteBuffer[256];	/**< Palette buffer 2 */

/**
 * A finite set of state definitions that the graphics_controller recognises.
 * @see graphics_controller.c
 */
typedef enum STATE {
	STATE_SPLASH_SCREEN = 0,	/**< The default state; The splash screen is rendered without interruption. */
	STATE_TRANSITIONING,		/**< This state represents a transition from any other state into the Finished Transition State. */
	STATE_FINISH_TRANSITION		/**< This state is the end of a transition effect done to migrate between splash screens. */
} STATE;

static STATE state;				/**< The state buffer recognised by the graphics controller */
unsigned int trans_frame_count;	/**< The frame count buffer used when in state 1 */

/**
 * Resets the state to zero;
 */
static inline graphics_StateReset() { state = STATE_SPLASH_SCREEN; }

/**
 *  Loads a new image to the specified location.
 * @param (header) A pointer to the image data.
 * @param (pal_p) The destination spot for the new palette.
 * @see graphics_controller.c
 */
void graphics_LoadImage(u8 *header, u16 *pal_p);

/**
 * Queue a transition effect sequence on the given image.
 * @param (data) The new image to be loaded.
 * @see graphics_controller.c
 */
void graphics_TransNewImage(u8 *data);

/**
 * Per-frame graphics_controller updater.
 * @see graphics_controller.c
 */
void graphics_Update();

#endif