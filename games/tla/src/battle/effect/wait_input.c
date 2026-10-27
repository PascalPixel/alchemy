#include "types.h"
#include "input.h"

void WaitTicks(s32 ticks);

/// Waits up to 120 frames, stopping early when a button is pressed.
void Event_WaitTwoSecondsOrPress(void) {
  u32 i;

  for (i = 0; i <= 119; i++) {
    WaitTicks(1);
    if (gInput.pressed != 0)
      break;
  }
}
