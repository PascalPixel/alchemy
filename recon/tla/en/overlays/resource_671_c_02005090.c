// Draft, differ 195 of 167: builds the five-digit counter's sprites (the work at
// gData_020023c4 + 0x288): decodes three digit lists (Func_02004f84), copies
// each digit's 64-byte picture into the sprite tiles and loads them into
// VRAM, builds two affine matrices (full size, and Func_02005020's scale),
// then fills five OAM entries 16 pixels apart, each using the scaled matrix
// where its digit changed or for the middle one (Func_020055ec draws them).
// Left: a rough reading: the native keeps the work pointers in r8 to r11, the matrices on the stack and the digit cursors in r7 and r10, and builds the first matrix's words differently; the alignment is poor.
#include "types.h"
#include "iwram_call.h"
#include "graphics/vram_tab.h"

extern u8 gData_020023c4[];
void Func_02004f84(s32 value, u8 *digits);
void VramCache_Load(s32 block, s32 size, void *pixels);
s32 AffineMatrix_BuildForEffect(u16 *matrix);
s32 Func_02005020(s32 value);
void Func_020055ec(u32 *oam, s32 priority);

void Func_02005090(void) {
  u8 *work = gData_020023c4 + 0x288;
  u8 *digit = work + 24;
  u8 *pixels = *(u8 **)work;
  u8 *tiles = *(u8 **)(work + 4);
  u32 *oam = (u32 *)(work + 32);
  s32 block = *(s32 *)(gData_020023c4 + 0x308);
  u32 tile = gResourceTableEntries[block].offset >> 5;
  u8 *old = gData_020023c4 + 0x290, *now = gData_020023c4 + 0x298;
  u16 matrix[4];
  s32 plain, scaled, i, affine, scale;

  Func_02004f84(*(s32 *)(work + 132) + 50, digit);
  Func_02004f84(*(s32 *)(work + 132) + 60, old);
  Func_02004f84(*(s32 *)(work + 132), now);
  for (i = 4; i >= 0; i--) {
    Iwram_CopyWords(tiles, pixels + (*digit << 6) - 0xc00, 64);
    tiles += 64;
    digit++;
  }
  VramCache_Load(block, 320, *(u8 **)(work + 4));
  *(u32 *)&matrix[2] &= 0xffff0000;
  *(u32 *)&matrix[0] =
      (((*(u32 *)&matrix[0] & 0xffff0000) | 0x200) & 0xffff) | 0x2000000;
  plain = AffineMatrix_BuildForEffect(matrix);
  scale = Func_02005020(*(s32 *)(work + 132));
  matrix[2] = 0;
  matrix[0] = scale;
  matrix[1] = scale;
  scaled = AffineMatrix_BuildForEffect(matrix);
  for (i = 0; i <= 4; i++) {
    oam[0] = 0;
    oam[1] = (i << 20) | 0x8000;
    oam[2] = tile | 0x400;
    ((u8 *)oam)[5] |= 3;
    if (*old != *now || i == 2)
      affine = scaled;
    else
      affine = plain;
    old++;
    now++;
    ((u8 *)oam)[7] = (((u8 *)oam)[7] & -63) | ((affine & 31) << 1);
    Func_020055ec(oam, 236);
    tile += 2;
    oam += 3;
  }
}
