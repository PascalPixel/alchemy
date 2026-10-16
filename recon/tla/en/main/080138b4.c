#include "input.h"

void Input_UpdateKeyRepeatAndDirection(void) {
  u32 held;
  u32 repeat;
  u32 count;
  u32 mask = 0;
  s32 timer;

  held = gInput.held;
  timer = gInput.repeatTimer;
  if (timer <= 0) {
    repeat = held;
    gInput.repeat = held;
    if (timer == 0)
      gInput.repeatTimer = 6;
    else
      gInput.repeatTimer = 19;
  } else {
    repeat = 0;
    gInput.repeat = 0;
  }
  if (repeat != 0) {
    count = 0;
    if (repeat & 0x40)
      count = 1;
    if (repeat & 0x80)
      count++;
    if (repeat & 0x20)
      count++;
    if (repeat & 0x10)
      count++;
    gInput.direction = repeat;
    switch (count) {
    case 0:
      gInput.previousDirection = 0x30;
      break;
    case 1:
      gInput.previousDirection = repeat & 0xf0;
      break;
    case 2:
      if ((gInput.previousDirection & gInput.direction) == 0)
        gInput.previousDirection = 0x30;
      gInput.direction &= gInput.previousDirection ^ 0xffff;
      break;
    case 3:
      if (gInput.previousDirection & 0x30)
        mask = 0x30;
      if (gInput.previousDirection & 0xc0)
        mask = 0xc0;
      gInput.previousDirection = repeat & (mask ^ 0xffff);
      gInput.direction &= mask ^ 0xffff;
      break;
    default:
      gInput.previousDirection = 0x30;
      gInput.direction &= 0xff0f;
      break;
    }
  } else {
    gInput.direction = repeat;
  }
  gInput.pressed = held & ~gInput.previousHeld;
  gInput.previousHeld = held;
}
