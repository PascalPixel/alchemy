// Draft, differ 149 of 167, skeleton gap 73 (tla-en, 2026-11-01)
// Builds the five-digit counter's sprites (the work at gData_020023c4 + 0x288):
// decodes three digit lists (Func_02004f84), copies each digit's 64-byte
// picture into the sprite tiles and loads them into VRAM, builds two affine
// matrices (full size, and Func_02005020's scale), then fills five OAM entries
// 16 pixels apart, each using the scaled matrix where its digit changed or for
// the middle one (Func_020055ec draws them).
// Left: the second loop's shape and the allocation that follows from it. The
// native computes i + 1, oam + 12, tile + 2 and reads byte 7 of the entry before
// the branch on the digits, keeps them in r11, r9, r5 and r1, and copies them
// back after the call: separate next-value locals reproduce that shape (gap
// still 73, with the same allocation trouble) but are forced temporaries, so not used;
// a form that gets it in plain C is what to look for. Two-armed stores do not
// hoist. With that pressure missing, `value` loses its register (native r11)
// and is reloaded from the pool, tile takes a register where the native keeps it
// at sp+8, pixels (native r10) goes to the stack, and the copy routine's address
// is hoisted out of the first loop. Settled: the effect is a 16-bit bitfield
// struct, written z, x, y; block, old and now read through work reach the pool
// entries; the entry's first three words go through a post-incremented pointer.
#include "types.h"
#include "iwram_call.h"
#include "graphics/vram_tab.h"

struct Effect {
  u32 x : 16;
  u32 y : 16;
  u32 z : 16;
};

struct Sprite {
  u8 b0, b1, b2, b3, b4, b5, b6;
  u8 b7a : 1;
  u8 affine : 5;
  u8 b7b : 2;
  u32 w8;
};

struct Work {
  u8 *pixels;
  u8 *tiles;
  u8 old[8];
  u8 now[8];
  u8 digit[8];
  struct Sprite oam[8];
  s32 block;
  s32 value;
};

extern u8 gData_020023c4[];
void Func_02004f84(s32 value, u8 *digits);
void VramCache_Load(s32 block, s32 size, void *pixels);
s32 AffineMatrix_BuildForEffect(void *matrix);
s32 Func_02005020(s32 value);
void Func_020055ec(void *oam, s32 priority);

void Func_02005090(void) {
  struct Work *work = (struct Work *)(gData_020023c4 + 0x288);
  s32 *value = &work->value;
  u8 *tiles = work->tiles;
  struct Sprite *oam = work->oam;
  u8 *digit = work->digit;
  u32 tile = gResourceTableEntries[work->block].offset >> 5;
  u8 *pixels = work->pixels;
  struct Effect effect;
  s32 plain, scaled, i, affine, scale;
  u32 *p;
  u8 *old, *now;

  Func_02004f84(*value + 50, digit);
  Func_02004f84(*value + 60, work->old);
  Func_02004f84(*value, work->now);
  for (i = 4; i >= 0; i--) {
    Iwram_CopyWords(tiles, pixels + (*digit << 6) - 0xc00, 64);
    tiles += 64;
    digit++;
  }
  VramCache_Load(work->block, 320, work->tiles);
  effect.z = 0;
  effect.x = 0x200;
  effect.y = 0x200;
  plain = AffineMatrix_BuildForEffect(&effect);
  scale = Func_02005020(*value);
  effect.z = 0;
  effect.x = scale;
  effect.y = scale;
  scaled = AffineMatrix_BuildForEffect(&effect);
  old = work->old;
  now = work->now;
  for (i = 0; i <= 4; i++) {
    p = (u32 *)oam;
    *p++ = 0;
    *p++ = (i << 20) | 0x8000;
    *p = tile | 0x400;
    oam->b5 |= 3;
    if (*old++ != *now++ || i == 2)
      affine = scaled;
    else
      affine = plain;
    oam->affine = affine;
    Func_020055ec(oam, 236);
    tile += 2;
    oam++;
  }
}
