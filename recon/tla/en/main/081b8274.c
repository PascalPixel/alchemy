// Draft, differ 10 of 105: draws a number as large digit sprites into the
// slot machine's sprite table, as Func_081b8180 does: digits places from x,
// 22 pixels apart, leading zeros skipped, tiles 640 + 16 * digit in the given
// palette. Returns the next free sprite. The sprite words are volatile, as
// ☀️'s ReelGame_RunFrame has its OAM shadow (FAKEMATCH(volatile) when
// adopted), which gives the native's two offset givs, the per-pass muls and
// the stack slots; as plain u32 words, 114 of 105 instructions differ in
// tla-en. Left: instruction order only, five single moves, each a stack
// store or load the native issues a place earlier (the x argument's spill,
// the y byte's slot, x's reload before the muls) or the constant 1's movs.
// Tried: x + i * 22 written i * 22 + x (12) or x + 512 + i * 22 (10); six
// permuter minutes (10).
#include "types.h"
#include "ram_buffer.h"

struct ReelSprite {
  volatile u32 attr01;
  volatile u32 attr2;
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
