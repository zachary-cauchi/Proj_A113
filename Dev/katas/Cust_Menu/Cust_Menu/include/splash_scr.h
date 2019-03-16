#ifndef _splash_scr_h_
#define _splash_scr_h_

#include <gba_types.h>
#include "mappy.h"
#include "fade.h"

// Selected effect to be used
void(*splash_screen_eff)();

void splash_setPal1(u16 *pal);
void splash_setPal2(u16 *pal);

void splash_update();
void splash_fade_out();
void splash_pulse_gray();
void splash_pulse_title();

#endif