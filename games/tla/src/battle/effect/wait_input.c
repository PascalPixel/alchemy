#include "types.h"
#include "input.h"

void WaitFrames(s32 frames);

// Waits up to 120 frames, stopping early when a button is pressed.
void Event_WaitTwoSecondsOrPress(void) {
  u32 i;

  for (i = 0; i <= 119; i++) {
    WaitFrames(1);
    if (gInput.pressed != 0)
      break;
  }
}
