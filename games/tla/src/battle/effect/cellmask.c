#include "types.h"

// Clears the left (corner & 3) + 1 pixels of halfword corner >> 2 in the
// 4bpp tile at (x, y) of a 26-tile-wide grid at VRAM 0x06000200.
void Tile_ClearLeftPixels(s32 x, s32 y, u32 corner) {
  u16 *cell = (u16 *)(0x06000200 + ((y * 26 + x) << 5));

  cell += corner >> 2;

  switch (corner & 3) {
  case 0: {
    u32 value = *cell;
    u32 mask;

    /* FAKEMATCH: ⚓️ reads the cell before it builds the mask; the plain
     * statement starts the mask first (one pair of instructions swapped). */
    __asm__ volatile("");
    mask = 0xfff0;
    mask &= value;
    *cell = mask;
  } break;
  case 1:
    *cell &= 0xff00;
    break;
  case 2:
    *cell &= 0xf000;
    break;
  case 3:
    *cell &= 0;
    break;
  }
}
