// Matches (tla-en, 2026-11-08, -mtune=gba), under -Os, with no device: the loop is one body (the
// affine index chosen by an if, one call) with i, tile and oam all stepped in the for header; as arms that
// each step oam, 15 differ; with tile also stepped in the arms, the two hoisted copies swap registers (17).
// No twin: the routine is the Gabomba statue clock (clock.c's ClockWork at gData_020023c4 + 0x288); no
// other scene or ☀️ has its five-digit counter loop.
// The entry: byte 5 is a two-bit mode field and its rest, as the gabomba_statue sibling s4710.c declares it;
// the copy loop counts up (the compiler reverses it); the scale is a plain s32 local stored into z = 0, x, y.
// Builds the five-digit counter's sprites: decodes three digit lists, copies each digit's picture into the
// tiles and loads them, builds two affine matrices and fills five OAM entries.
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
  u8 *now = work->tiles;
  u32 tile = gResourceTableEntries[work->block].offset >> 5;
  struct Sprite *oam = work->oam;
  s32 *value = &work->value;
  struct Effect effect;
  s32 plain, scaled, i;
  s32 scale;
  u32 *p;

  Func_02004f84(*value + 50, digit);
  Func_02004f84(*value + 60, work->old);
  Func_02004f84(*value, work->now);
  for (i = 0; i < 5; i++) {
    Iwram_CopyWords(now, old + (*digit << 6) - 0xc00, 64);
    now += 64;
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
