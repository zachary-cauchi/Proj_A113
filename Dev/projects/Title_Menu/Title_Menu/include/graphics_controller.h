#ifndef _graphics_controller_h_
#define _graphics_controller_h_

#include "gba_types.h"

// Controls the 'menu state'
// 0 = splash screen mode
// 1 = transition to title mode
// 2 = title mode
unsigned int state;
// Control variable for handling transition to title mode
unsigned int transFrameCount;

void graphics_LoadImage(u8 *header, u16 *pal_p);

void graphics_TransNewImage(u8 *data, u16 *pal_p);

#endif