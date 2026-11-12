#include "types.h"
#include "sprite.h"
#include "ram_buffer.h"

struct AnimationEntry *AnimationObject_AllocateEmpty(void);
s32 Animation_ApplyChildArgumentFar(void *, s32);

struct EffectSprite {
  u8 unk00[0x0c];
  s32 unk0c;
  u8 unk10;
  u8 unk11_0 : 1;
  u8 unk11_1 : 7;
  u16 unk12;
  u8 kind;
  u8 unk15;
  u8 unk16;
  u8 unk17;
  u8 unk18[2];
  u8 unk1a;
  u8 unk1b;
  u8 unk1c[0x0c];
  struct AnimationEntry *entry;
  u8 unk2c[0x0c];
};

// Takes the first free effect sprite of the 64 with a new animation entry.
struct EffectSprite *Func_0815b3b0(s32 kind, s32 value) {
  struct EffectSprite *sprite = Ram_HeapSlots->unk0c[1];
  struct EffectSprite *found = NULL;
  s32 i = 0;

  while (i <= 63) {
    if (sprite->kind == 0) {
      found = sprite;
      break;
    }
    i++;
    sprite++;
  }
  sprite->entry = AnimationObject_AllocateEmpty();
  sprite->kind = kind;
  sprite->unk15 = value;
  sprite->unk0c = 0x10000;
  sprite->unk11_0 = 0;
  sprite->unk17 = 0;
  sprite->unk16 = 0;
  sprite->unk1b = 1;
  sprite->unk1a = 0;
  sprite->unk12 = 0;
  Animation_ApplyChildArgumentFar(sprite, 0);
  return found;
}

/// Fills BG palette colours 1 to 63 with a grey ramp, each level i / 2.
void Graphics_LoadGreyRamp(void) {
  s32 i = 0;
  u16 *color = (u16 *)0x05000002;

next:
  {
    s32 level = i / 2;

    i++;
    *color = level << 10 | level << 5 | level;
  }
  color++;
  if (i != 63)
    goto next;
}

// Shaded circles of growing radius, drawn into a half-width byte map.

#include "fixed_math.h"
#include "iwram_call.h"

/// Fills a size by size/2 byte map with circles of growing radius, each
/// shaded by the cosine of its radius (0 to 63).
void Func_0815b434(u8 *map, s32 size, s32 scale) {
  s32 half = size / 2;
  s32 center = half - 1;
  s32 last = size - 1;
  s32 radius;

  Iwram_ClearWords(map, half * size);
  for (radius = 0; radius != last; radius++) {
    s32 x = radius;
    s32 d = radius;
    s32 y = 0;
    s32 shade;
    s32 col;

    shade = (Trig_Cos((radius << 14) / last) * 63) >> 16;
    shade = shade * scale / 0x10000;

    if (shade > 63)
      shade = 63;
    while (x >= y) {
      col = center - x / 2;
      map[half * (last - y) + col] = shade;
      col = center - y / 2;
      map[half * (last - x) + col] = shade;
      d = d - y * 2 - 1;
      if (d < 0) {
        d = d + x * 2 - 2;
        x--;
      }
      y++;
    }
  }
}

// Shaded circles of growing radius, mirrored into all four quarters of a
// byte map.

/// Func_0815b434 for a size by size map: each circle is drawn in all four
/// quarters, mirrored about the centre row and column.
void Func_0815b510(u8 *map, s32 size, s32 scale) {
  s32 center = size / 2 - 1;
  s32 last = size - 1;
  s32 radius;

  Iwram_ClearWords(map, size * size * 2);
  for (radius = 0; radius != last; radius++) {
    s32 x = radius;
    s32 d = radius;
    s32 y = 0;
    s32 shade;
    s32 col;
    s32 row;

    shade = (Trig_Cos((radius << 14) / last) * 63) >> 16;
    shade = shade * scale / 0x10000;
    if (shade > 63)
      shade = 63;
    while (x >= y) {
      col = center - x / 2;
      row = last - y;
      map[row * size + col] = shade;
      map[(size * 2 - row - 1) * size + col] = shade;
      map[row * size + (size - col) - 1] = shade;
      map[(size * 2 - row - 1) * size + (size - col) - 1] = shade;
      col = center - y / 2;
      row = last - x;
      map[row * size + col] = shade;
      map[(size * 2 - row - 1) * size + col] = shade;
      map[row * size + (size - col) - 1] = shade;
      map[(size * 2 - row - 1) * size + (size - col) - 1] = shade;
      d = d - y * 2 - 1;
      if (d < 0) {
        d = d + x * 2 - 2;
        x--;
      }
      y++;
    }
  }
}

#include "io_reg.h"

static __inline__ s32 CopyWords_(void *destination, const void *source,
                                 s32 size) {
  return Iwram_CopyWords(destination, source, size);
}

// Mirrors the first 64 background colours, keeping the backdrop colour.
void Func_0815b634(void) {
  u16 saved[64];
  u16 flipped[64];
  s32 i;

  /* FAKEMATCH(wrapper): both copies through CopyWords_, so each builds its
   * array's address anew; as direct Iwram_CopyWords calls, 19 of 39
   * instructions differ in tla-en. */
  CopyWords_(saved, (void *)BG_PLTT, 128);
  for (i = 1; i != 64; i++)
    flipped[i] = saved[64 - i];
  flipped[0] = saved[0];
  CopyWords_((void *)BG_PLTT, flipped, 128);
  REG_BLDALPHA = 0x810;
}
