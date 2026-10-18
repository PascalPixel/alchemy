// Draft, differ 86 of 119: draws a number as digit sprites into the slot
// machine's sprite table, right-aligned to end at x: each of digits places
// takes value's digit there, leading zeros skipped but the last digit always
// drawn, tiles 816 + 4 * digit in the given palette. Returns the next free
// sprite. Left: register choice and addressing: the native keeps the work
// pointer and the sprite count in stack slots and addresses the two words
// with two offset givs (sprite * 8 + 196 in r7, + 200 in r10, each str
// [work, offset]); here loop.c folds work into one pointer giv (r6) stored
// at +0 and +4, the count stays in r11, the frame is 16 bytes not 24, and
// the remaining registers rotate. Control flow and the divisions match.
// Tried: the divide loop guarded by its own digits - i != 1 test (differ 88).
#include "types.h"
#include "ram_buffer.h"

struct ReelSprite {
  u32 attr01;
  u32 attr2;
};

struct ReelWork {
  u8 unk000[0xc4];
  struct ReelSprite obj[128];
};

s32 Func_081b8180(s32 sprite, s32 x, s32 y, s32 value, s32 palette,
                  s32 digits) {
  struct ReelWork *work;
  s32 started;
  s32 digit;
  s32 i;
  s32 k;

  work = (struct ReelWork *)Ram_HeapSlots->canvasMap;
  started = 0;
  x -= digits * 10;
  for (i = 0; i != digits; i++) {
    digit = value;
    for (k = 0; k != digits - i - 1; k++)
      digit /= 10;
    digit %= 10;
    if (digit != 0)
      started = 1;
    else if (!started && i != digits - 1)
      continue;
    work->obj[sprite].attr01 = (((x + i * 10 + 522) & 0x1ff) << 16) |
                               ((y + 256) & 255) | 0x40000000;
    work->obj[sprite].attr2 = (digit * 4 + 816) | (palette << 12);
    sprite++;
  }
  return sprite;
}
