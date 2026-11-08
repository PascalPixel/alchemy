#include "types.h"
#include "io_reg.h"
#include "input.h"

/// Frames a key is held before it first repeats, and between later repeats.
#define INPUT_KEY_REPEAT_FIRST 19
#define INPUT_KEY_REPEAT_NEXT 6

/// Runs once a frame after VBlank, as The Broken Seal's twin does. When the
/// repeat timer has run out, the held keys repeat into gInput.repeat.
/// gInput.direction is the same keys with at most one D-pad direction: of
/// several held directions it keeps the one added last (previousDirection
/// remembers it), and with all four it keeps none. Last, the keys newly held
/// since the previous frame go to pressed, and previousHeld takes held.
void Input_UpdateKeyRepeatAndDirection(void) {
  u32 held;
  u32 keys;
  u32 axis = 0;
  u32 directionCount;
  u32 mask;
  s32 delay;

  held = gInput.held;
  /* FAKEMATCH(barrier): the form kept; as the plain read, 2 of 113
   * instructions differ in tla-en (the timer is loaded before held). */
  asm("");
  delay = gInput.repeatTimer;
  if (delay <= 0) {
    keys = held;
    gInput.repeat = held;
    if (delay == 0)
      gInput.repeatTimer = INPUT_KEY_REPEAT_NEXT;
    else
      gInput.repeatTimer = INPUT_KEY_REPEAT_FIRST;
  } else {
    keys = 0;
    gInput.repeat = 0;
  }

  if (keys != 0) {
    // Count the D-pad directions in the repeat
    directionCount = 0;
    if (keys & KEY_UP)
      directionCount = 1;
    if (keys & KEY_DOWN)
      directionCount++;
    if (keys & KEY_LEFT)
      directionCount++;
    if (keys & KEY_RIGHT)
      directionCount++;

    gInput.direction = keys;
    switch (directionCount) {
    default:
      gInput.previousDirection = KEYS_HORIZONTAL;
      /* FAKEMATCH(one-pass): the form kept; as plain C with no block, 2 of 113
       * instructions differ in tla-en (the direction is loaded before the
       * mask). */
      do {
      } while (0);
      mask = 0xffff & ~KEYS_DPAD;
      gInput.direction &= mask;
      break;
    case 0:
      gInput.previousDirection = KEYS_HORIZONTAL;
      break;
    case 1:
      gInput.previousDirection = keys & KEYS_DPAD;
      break;
    case 2:
      if ((gInput.previousDirection & gInput.direction) == 0)
        gInput.previousDirection = KEYS_HORIZONTAL;
      gInput.direction &= gInput.previousDirection ^ 0xffff;
      break;
    case 3:
      if (gInput.previousDirection & KEYS_HORIZONTAL)
        axis = KEYS_HORIZONTAL;
      if (gInput.previousDirection & KEYS_VERTICAL)
        axis = KEYS_VERTICAL;
      mask = 0xffff ^ axis;
      gInput.previousDirection = keys & mask;
      gInput.direction &= mask;
      break;
    }
  } else {
    gInput.direction = keys;
  }

  gInput.pressed = held & ~gInput.previousHeld;
  /* FAKEMATCH(barrier): the form kept; as plain C, 2 of 113 instructions
   * differ in tla-en (previousHeld is stored before pressed). */
  asm("");
  gInput.previousHeld = held;
}
