// Draft, differ 114 of 105: draws a number as large digit sprites into the
// slot machine's sprite table, as Func_081b8180 does: digits places from x,
// 22 pixels apart, leading zeros skipped, tiles 640 + 16 * digit in the given
// palette. Returns the next free sprite. Left: other: the native multiplies
// i by 22 in each pass (muls) where loop.c here strength-reduces x + i * 22
// into a register giv, keeps the work pointer in a stack slot and addresses
// the two words with two offset givs (+196 in r7, +200 in r10), and keeps
// the sprite count in r11; here one pointer giv carries both words, so the
// frame (12 bytes, not 20) and every register differ. Control flow and the
// divisions match.
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

s32 Func_081b8274(s32 sprite, s32 x, s32 y, s32 value, s32 palette,
                  s32 digits) {
  struct ReelWork *work;
  s32 started;
  s32 digit;
  s32 i;
  s32 k;

  work = (struct ReelWork *)Ram_HeapSlots->canvasMap;
  started = 0;
  for (i = 0; i != digits; i++) {
    digit = value;
    for (k = 0; k != digits - i - 1; k++)
      digit /= 10;
    digit %= 10;
    if (digit != 0)
      started = 1;
    else if (!started)
      continue;
    work->obj[sprite].attr01 = (((x + i * 22 + 512) & 0x1ff) << 16) |
                               ((y + 256) & 255) | 0x80000000;
    work->obj[sprite].attr2 = (digit * 16 + 640) | (palette << 12);
    sprite++;
  }
  return sprite;
}
