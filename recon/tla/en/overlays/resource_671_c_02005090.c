// Draft, differ 149 of 167, skeleton gap 73 (tla-en, 2026-11-01)
// Builds the five-digit counter's sprites (the work at gData_020023c4 + 0x288):
// decodes three digit lists (Func_02004f84), copies each digit's 64-byte
// picture into the sprite tiles and loads them into VRAM, builds two affine
// matrices (full size, and Func_02005020's scale), then fills five OAM entries
// 16 pixels apart, each using the scaled matrix where its digit changed or for
// the middle one (Func_020055ec draws them).
// Left: the second loop. The native computes i + 1, oam + 12, tile + 2 and reads
// byte 7 at the end of the block before the branch on the digits, and copies
// them back after the call (r11, r9 and r5 to r6, r8 and sp+8); the end of that
// block is where gcse inserts, but plain PRE does not move code from both arms,
// and alchemy-gcc only hoists under optimize_size, so the source of this shape is
// still open. No plain form tried gets it: stores in both arms, whole tails in
// both arms, and old[i] and now[i] for the pointers all stay at a gap of 73 or
// worse.
// Measured with forced temporaries, best differ 130, gap 48, so left out (S2):
// nextI, nextOam and nextTile set before the branch; `s32 b7 = byte 7` before it,
// which keeps the pooled -63 (a u8 local folds it to 193, an s8 local gives
// ldrb but then sign-extends); both arms in full (`a = scaled; a &= 31; y = -63 &
// b7; a <<= 1; y |= a; byte 7 = y`), which stops IF-CASE-2 lifting the plain load
// and lets cross-jumping leave the native's one-load arms. It also used: goto for
// both loops, which keeps IwramCopyWords and the mask in the body; separate j
// for the first loop; locals in the listing's order (digit, tile, pixels, tiles,
// value, oam); i = 0 before the first matrix, which puts tile on the stack and
// value in r11 with the native's add; old and now set before the second matrix
// (old crossing one call goes to r4). What then remains: digit and nextOam want
// r9, pixels and old r10 (digit outranks old, and r10 comes first), and the first
// block's order.
// Settled: the effect is a 16-bit bitfield struct, written z, x, y; block, old
// and now read through work reach the pool entries; the entry's first three
// words go through a post-incremented pointer.
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
