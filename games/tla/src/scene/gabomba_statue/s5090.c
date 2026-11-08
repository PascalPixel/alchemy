/// @file
/// Draws the clock's five-digit count: copies the pictures of the digits of
/// timer + 50 out of the digit sheet and loads them, builds a plain and a
/// scaled affine matrix, and fills five OAM entries, giving the scaled matrix
/// to the middle digit and to each digit where timer + 60 and timer differ.

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
  u8 b0, b1, b2, b3, b4;
  u8 mode : 2;
  u8 b5rest : 6;
  u8 b6;
  u8 b7a : 1;
  u8 affine : 5;
  u8 b7b : 2;
  u32 w8;
};

struct ClockWork {
  u8 *tiles;  ///< the digit sheet
  u8 *digits; ///< the five digit pictures, as loaded
  u8 old[8];
  u8 now[8];
  u8 digit[8];
  struct Sprite oam[8];
  s32 tileEntry;
  s32 timer;
};

extern u8 gData_020023c4[];
void Func_02004f84(s32 value, u8 *digits);
void VramCache_Load(s32 block, s32 size, void *pixels);
s32 AffineMatrix_BuildForEffect(void *matrix);
s32 Func_02005020(s32 value);
void Func_020055ec(void *oam, s32 priority);

void Func_02005090(void) {
  struct ClockWork *clock = (struct ClockWork *)(gData_020023c4 + 0x288);
  u8 *digit = clock->digit;
  u8 *old = clock->tiles;
  u8 *now = clock->digits;
  u32 tile = gResourceTableEntries[clock->tileEntry].offset >> 5;
  struct Sprite *oam = clock->oam;
  s32 *timer = &clock->timer;
  struct Effect effect;
  s32 plain, scaled, i;
  s32 scale;
  u32 *p;

  Func_02004f84(*timer + 50, digit);
  Func_02004f84(*timer + 60, clock->old);
  Func_02004f84(*timer, clock->now);
  // old and now walk the sheet and the pictures here, the digit lists below.
  for (i = 0; i < 5; i++) {
    Iwram_CopyWords(now, old + (*digit << 6) - 0xc00, 64);
    now += 64;
    digit++;
  }
  VramCache_Load(clock->tileEntry, 320, clock->digits);
  effect.z = 0;
  effect.x = 0x200;
  effect.y = 0x200;
  plain = AffineMatrix_BuildForEffect(&effect);
  scale = Func_02005020(*timer);
  effect.z = 0;
  effect.x = scale;
  effect.y = scale;
  scaled = AffineMatrix_BuildForEffect(&effect);
  old = clock->old;
  now = clock->now;
  for (i = 0; i <= 4; i++, tile += 2, oam++) {
    p = (u32 *)oam;
    *p++ = 0;
    *p++ = (i << 20) | 0x8000;
    *p = tile | 0x400;
    oam->mode = 3;
    if (*old++ != *now++ || i == 2)
      oam->affine = scaled;
    else
      oam->affine = plain;
    Func_020055ec(oam, 236);
  }
}
