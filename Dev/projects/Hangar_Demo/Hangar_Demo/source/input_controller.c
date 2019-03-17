#include "input_controller.h"

#include "gba_input.h"

void input_key_reads() {
	if ((keyFrame & 7) == 0) {
		// Check for key updates and record the results to a buffer for later use
		scanKeys();
		pressed = keysDown();
		held = keysHeld();
		released = keysUp();
	}
}