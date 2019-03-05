#ifndef _splash_scr_h_
#define _splash_scr_h_

#include <gba_types.h>

#include "fade.h"

void(*screen_eff)();

void setPal1(u16 *pal);
void setPal2(u16 *pal);

void splash_fade_out();
void splash_pulse_gray();
void splash_pulse_title();

#endif