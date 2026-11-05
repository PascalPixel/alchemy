// Draft, differ 123 of 167, skeleton gap 63 (tla-en, 2026-11-01)
// Builds the five-digit counter's sprites (the work at gData_020023c4 + 0x288):
// decodes three digit lists (Func_02004f84), copies each digit's 64-byte
// picture into the sprite tiles and loads them into VRAM, builds two affine
// matrices (full size, and Func_02005020's scale), then fills five OAM entries
// 16 pixels apart, each using the scaled matrix where its digit changed or for
// the middle one (Func_020055ec draws them).
// Left: everything comes from one shape in the second loop. The native computes
// i + 1, oam + 12, tile + 2 and reads byte 7 at the end of the block before the
// branch on the digits, and copies them back after the call (r11, r9 and r5 to
// r6, r8 and sp+8). Those three extra live values are why the native spills
// tile, plain and scaled (frame 20) and gives value r11: across the first calls
// eight values want seven callee-saved registers, and in plain C the loop needs
// few enough that tile keeps a register and value (a constant equivalent) loses,
// so it stays a pool reload and the frame stays 8. That is the wrong registers
// and pool order of the entry block too; none of it has its own cause.
// Why it stays: gcse's PRE only moves a partial redundancy, and these have none
// (LCM leaves them where they are, in the join or in both arms); moving what both
// arms compute above the branch is code hoisting, which gcse runs only under
// optimize_size, so no plain C found reaches it. That is an options question, out
// of scope here (K3). IF-CASE-2 lifts one insn at most (BRANCH_COST 1).
// Tried without gain, gap 63 or worse: stores in both arms (differ 183), a ternary
// (130), old[i] and now[i] (134), work->old[i] (144), goto for the second loop
// (154), a separate counter for the first loop (138), old and now set before the
// second matrix (134), every order of the increments and of the three stores,
// u8 affine, affine in an inner block, s32 tile, other local orders, unused
// scale removed (140). i = 0 before the first matrix, as the listing's order
// suggests, is 128 here: it lets reload reuse r6 for the second z store.
// Measured with forced temporaries, best differ 130, gap 48, so left out (S2):
// nextI, nextOam and nextTile set before the branch and `s32 b7` read there.
// Settled: a goto for the first loop keeps IwramCopyWords and -0xc00 in the body
// (a `for` loses 22); all 1440 orders of the six initial locals and early or late
// i were measured, this one is the best; the effect is a 16-bit bitfield struct;
// block, old and now read through work reach the pool entries; the entry's first
// three words go through a post-incremented pointer.
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
  u8 *digit = work->digit;
  u8 *tiles = work->tiles;
  u8 *pixels = work->pixels;
  u32 tile = gResourceTableEntries[work->block].offset >> 5;
  struct Sprite *oam = work->oam;
  struct Effect effect;
  s32 plain, scaled, i, affine, scale;
  u32 *p;
  u8 *old, *now;

  Func_02004f84(*value + 50, digit);
  Func_02004f84(*value + 60, work->old);
  Func_02004f84(*value, work->now);
  i = 4;
loop1:
  Iwram_CopyWords(tiles, pixels + (*digit << 6) - 0xc00, 64);
  i--;
  tiles += 64;
  digit++;
  if (i >= 0)
    goto loop1;
  VramCache_Load(work->block, 320, work->tiles);
  effect.z = 0;
  effect.x = 0x200;
  effect.y = 0x200;
  plain = AffineMatrix_BuildForEffect(&effect);
  effect.z = 0;
  effect.x = scale;
  scale = Func_02005020(*value);
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
