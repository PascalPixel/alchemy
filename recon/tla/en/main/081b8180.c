// 2026-10-25, with the Makefile's options (-mthumb-slow-loads): differ 8 of
// 119; a greedy device search found nothing lower.
// Draft, differ 12 of 119: draws a number as digit sprites into the slot
// machine's sprite table, right-aligned to end at x: each of digits places
// takes value's digit there, leading zeros skipped but the last digit always
// drawn, tiles 816 + 4 * digit in the given palette. Returns the next free
// sprite. The sprite words are volatile, as ☀️'s LuckyWheels_Update has its
// OAM shadow (FAKEMATCH(volatile) when adopted): loop.c then keeps the two
// offset givs (sprite * 8 + 196 in r7, + 200 in r10) and the work pointer
// and sprite count in stack slots, as the native does; as plain u32 words,
// 86 of 119 instructions differ in tla-en. Left: instruction order only, six
// single moves, each a stack store or load the native issues a place
// earlier (the sprite argument's spill, the y byte's slot, the sprite
// count's store) or a register copy (mov r9, r3). Tried: an empty asm at the
// top of the body (16), after the work load (12) and after sprite++ (12);
// the divide loop guarded by its own digits - i != 1 test (88, before the
// volatile words); six permuter minutes (12).
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
