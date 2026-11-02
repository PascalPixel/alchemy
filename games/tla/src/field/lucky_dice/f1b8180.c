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
  /* FAKEMATCH(volatile): the sprite words volatile, as The Broken Seal's
   * twin has them; as plain u32 members, 84 of 119 instructions differ in
   * tla-en. */
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
    work->obj[sprite].attr01 =
        (((x + i * 10 + 522) & 0x1ff) << 16) | ((y + 256) & 255) | 0x40000000;
    work->obj[sprite].attr2 = (digit * 4 + 816) | (palette << 12);
    sprite++;
  }
  return sprite;
}
