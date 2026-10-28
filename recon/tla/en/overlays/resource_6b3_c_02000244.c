// Draft, differ 19 of 102: centres a message and hands it to Func_02000158. x
// and y stay in their stack slots for Text_GetMessageSize; GCSE copies
// their loads for the closing call into each branch, so an empty asm with a
// memory clobber before the call keeps them as one load each, as the native has
// (without it: 64; with x and y volatile their addresses take registers: 100).
// Left, register allocation: the native gives the message r8, the wait r10 and
// the mode r9 and stores the zeros of x and y from r4, where this gives the
// mode r8, the message r10 and the wait r9 and stores the zeros from r3; a
// pinned mode (29) or another place for its zero (30) is worse, and permute
// found nothing in 8 minutes.

// Draft of overlay 6b3 Func_02000244: centres a message and hands it to
// Func_02000158. The logic is right; the listing keeps x and y in their stack
// slots (it stores y in every branch and loads both once before the call),
// where this draft forwards y as a constant and reloads x in each branch, and
// the listing builds x's and y's zero in r4 apart from the mode's zero in r3.
// Tried: integer-cast addresses, "y = x = 0", and the four values as one
// struct (the stores then stay, but through a base register: 75 differ).

#include "types.h"

extern s32 gData_02000a28;

void Text_GetMessageSize();
void Func_02000158();

void Func_02000244(s32 message, s32 position, s32 frames) {
  s32 x;
  s32 y;
  s32 width;
  s32 height;
  s32 mode = 0;
  s32 left;
  s32 line = 0;
  s32 half;
  s32 wide;

  x = 0;
  y = 0;
  wide = message & 0x8000;
  Text_GetMessageSize(gData_02000a28, &x, &y, &width, &height);
  if (width <= 24) {
    if (!wide)
      x = (25 - width) / 2 + 5;
    else
      x = (30 - width) / 2;
    left = x - 5;
  } else {
    x = (30 - width) / 2;
    left = x;
  }
  half = (height - 1) / 2;
  if (position == 0) {
    y = 3;
    line = half + 1;
  } else if (position == 1) {
    y = 14 - half;
    line = y + half - 2;
  } else if (position == 2) {
    if (half == 3) {
      y = 13;
      line = 15;
      mode = 2;
    } else {
      y = 16;
      line = 15;
    }
  }
  __asm__("" : : : "memory");
  Func_02000158(message, x, y, left, line, frames, mode);
}
