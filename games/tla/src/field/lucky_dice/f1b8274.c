/// @file
/// A number drawn as large digit sprites.

#include "types.h"
#include "ram_buffer.h"

/// A sprite's attribute words. They are volatile, as The Broken Seal's
/// LuckyWheels_Update keeps its OAM shadow; as plain words, the two offset
/// counters, the multiply on each pass and the stack slots all differ.
struct ReelSprite {
  volatile u32 attr01;
  volatile u32 attr2;
};

struct ReelWork {
  u8 unk000[0xc4];
  struct ReelSprite obj[128];
};

/// Draws value as digits large digit sprites from x, 22 pixels apart, leading
/// zeros skipped, tiles 640 + 16 * digit in palette, as Func_081b8180 does.
/// Returns the next free sprite.
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
    work->obj[sprite].attr01 =
        (((x + i * 22 + 512) & 0x1ff) << 16) | ((y + 256) & 255) | 0x80000000;
    work->obj[sprite].attr2 = (digit * 16 + 640) | (palette << 12);
    sprite++;
  }
  return sprite;
}
