// Differ 67 of 167 (tla-en, 2026-11-06), draft 170, no devices. From 104: the scale is
// stored straight into effect.x and effect.y copied from it (one extra ldrh, but no scale
// pseudo, which settles most of the entry's allocation: 75), then the entry's locals in
// the order digit, old, value, block, tile, now, oam (67, best of all 2520 orders with
// block before tile; the old scale local's best order was 100). A volatile scale (73, frame
// 24) and a 2 held in a local for i == 2 were permuter finds not kept.
// Left: the loop's hoisted copies, as below (i + 1 r5, tile + 2 r9, oam + 1 r11; the
// test reads i + 1), and a few entry rows.
// Earlier state, 104 of 167: the hoisted copies' registers and the entry's schedule, as below.
// Why the test reads i + 1: gcse's second cprop, after -Os hoisting, replaces i with
// the hoisted i + 1 in the loop's exit test, since both arms copy it into i; the
// listing tests i. Pinning i to r6 (measured 2026-11-06, not kept) stops that but
// also stops the hoist and the folding of the entry test: 114.
// Builds the five-digit counter's sprites: decodes three digit lists,
// copies each digit's picture into the tiles and loads them, builds two affine matrices and fills five OAM entries.
// The arms' increments hoist under -Os as the native's do and the frame slots agree (scaled, plain, tile at sp+0, 4, 8).
// Left: the hoisted copies' registers. Native: i + 1 in r11, oam + 1 in r9, tile + 2 in r5, tested as i after its copy back.
// Here the loop test reads i + 1 itself (cmp r5, #4), giving it 4 refs, so it ranks first and takes r5.
// Also the entry's order follows from those ranks. Tried: all 36 orders of the arms' increments, i < 5, do-while, goto.
// Also tried (6 November): one merged body (124); i++ in the header, tile and oam in the arms (110: the test then reads i
// and tile + 2, oam + 1 take r5, r9 as native, but tile itself takes r11 and i + 1 spills); other header and arm splits
// (108 to 116); oam++ as the call's argument (138); a second loop counter (126); s32 or u16 tile (110, 132); &work->value
// at each use (130); a dozen orders of the entry's declarations (104 to 130); (*digit - 48) * 64 (104).
#include "types.h"
#include "call.h"
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
  u8 *digit = work->digit;
  u8 *old = work->pixels;
  s32 *value = &work->value;
  s32 block = work->block;
  u32 tile = gResourceTableEntries[block].offset >> 5;
  u8 *now = work->tiles;
  struct Sprite *oam = work->oam;
  struct Effect effect;
  s32 plain, scaled, i;
  u32 *p;

  Func_02004f84(*value + 50, digit);
  Func_02004f84(*value + 60, work->old);
  Func_02004f84(*value, work->now);
  for (i = 4; i >= 0; i--) {
    Iwram_CopyWords(now, old + (*digit << 6) - 0xc00, 64);
    now += 64;
    digit++;
  }
  VramCache_Load(work->block, 320, work->tiles);
  effect.z = 0;
  effect.x = 0x200;
  effect.y = 0x200;
  plain = AffineMatrix_BuildForEffect(&effect);
  effect.x = Func_02005020(*value);
  effect.z = 0;
  effect.y = effect.x;
  scaled = AffineMatrix_BuildForEffect(&effect);
  old = work->old;
  now = work->now;
  for (i = 0; i <= 4;) {
    p = (u32 *)oam;
    *p++ = 0;
    *p++ = (i << 20) | 0x8000;
    *p = tile | 0x400;
    oam->b5 |= 3;
    if (*old++ != *now++ || i == 2) {
      oam->affine = scaled;
      Func_020055ec(oam, 236);
      tile += 2;
      oam++;
      i++;
    } else {
      oam->affine = plain;
      Func_020055ec(oam, 236);
      tile += 2;
      oam++;
      i++;
    }
  }
}
