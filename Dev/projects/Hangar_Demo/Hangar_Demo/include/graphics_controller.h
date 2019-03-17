#ifndef _graphics_controller_h_
#define _graphics_controller_h_

#include "splash_scr.h"
#include "gba_base.h"
#include "gba_types.h"


////////////////////////////////////
// storage space for palette data
////////////////////////////////////

extern u16 EWRAM_DATA PaletteBuffer[256];		/**< Palette buffer 1 */
extern u16 EWRAM_DATA PulsePaletteBuffer[256];	/**< Palette buffer 2 */

/**
 * A struct representing a splash screen. This contains its image data and the effect to be applied.
 * WIP: Currently not implemented
 */
typedef struct SPLASH {
	u8 * image_data;
	u16 * main_pal;
	u16 * alt_pal;
	SPLASH_EFFECT effect;
	u32 duration;
	struct SPLASH * nextSplash;
} SPLASH;

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
 * Initialise a splash screen struct with the image data, splash effect, and duration provided.
 * The splash palettes are set to the palette buffers by default.
 */
SPLASH * graphics_CreateSplash(u8 * image, void(*splash_eff)(), u32 duration);

/**
 * Resets the state to zero;
 */
static inline void graphics_StateReset() { state = STATE_SPLASH_SCREEN; }

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