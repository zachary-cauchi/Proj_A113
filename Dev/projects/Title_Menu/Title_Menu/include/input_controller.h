#ifndef _input_controller_h_
#define _input_controller_h_

#include "gba_types.h"
#include "gba_input.h"

u32 keyFrame;
u32 pressed;
u32 released;
u32 held;

void input_key_reads();

static inline bool input_key_pressed(KEYPAD_BITS key) { return key & pressed; }

static inline bool input_key_released(KEYPAD_BITS key) { return key & released; }

static inline bool input_key_held(KEYPAD_BITS key) { return key & held; }

#endif