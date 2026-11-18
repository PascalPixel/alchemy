/// @file
/// A heading picture shown in a window that opens from the centre, and wiped
/// away from the left. Nothing calls these but the far-call table.
// Part of field_sequence.c.

#include "window/window_tile.h"

/// A heading picture: 26 by 5 tiles of 16 colours, drawn from tile 16 of
/// character block 0 and shown in the window palette on BG0.
#define HEADING_COLUMNS 26
#define HEADING_ROWS 5
#define HEADING_FIRST_TILE 16
#define HEADING_TILES                                                          \
  ((u8 *)BG_CHAR_BLOCK(0) + HEADING_FIRST_TILE * TILE_SIZE_4BPP)
#define HEADING_TILE_BYTES (HEADING_COLUMNS * HEADING_ROWS * TILE_SIZE_4BPP)
/// A blank BG0 cell: tile 0 in the window palette.
#define HEADING_BLANK_CELL BG_TILE_PALETTE(WINDOW_PALETTE)
/// The frames the heading's window takes to open from the centre, and the
/// pixels it widens by on each side a frame.
#define HEADING_OPEN_FRAMES 24
#define HEADING_OPEN_STEP 5

/// Unused: only the far-call table refers to it. Draws heading picture
/// `heading` of the headings resource into a 26 by 5 cell window and opens
/// the window from the centre.
void Heading_Show(s32 heading) {
  u16 tile;
  s32 *headings = Resource_GetTableEntry(RESOURCE_HEADINGS);
  u8 *tiles;
  u16 *cell;
  u32 row, column;
  s32 left, right;

  tile = HEADING_FIRST_TILE;
  Event_WaitTransitionFrames();
  cell = (u16 *)BG_SCREEN_BLOCK(4);
  tiles = Heap_AllocEwram(HEADING_TILE_BYTES);
  headings += headings[heading] / 4;
  Decompress_ByteLz(headings, tiles);
  Dma_Set(tiles, HEADING_TILES, DMA_COPY_WORDS(HEADING_TILE_BYTES / 4),
          REG_DMA3);
  Heap_Release(tiles);
  REG_DISPCNT &= ~DISPCNT_BG0_ON;
  Dma_Fill32(HEADING_BLANK_CELL, cell, BG_SCREEN_SIZE);
  cell = (u16 *)BG_SCREEN_BLOCK(4) + 3 * BG_SCREEN_WIDTH;
  for (row = 0; row < HEADING_ROWS; row++) {
    cell += 2;
    for (column = 0; column < HEADING_COLUMNS; column++) {
      *cell++ = (u16)(tile++) | HEADING_BLANK_CELL;
    }
    cell += 4;
  }
  REG_DISPCNT &= ~(DISPCNT_WIN0_ON | DISPCNT_WIN1_ON | DISPCNT_OBJWIN_ON);
  REG_DISPCNT |= DISPCNT_WIN0_ON;
  REG_WININ = WININ_WIN0_ALL;
  REG_WINOUT = WINOUT_WIN01_BG1 | WINOUT_WIN01_BG2 | WINOUT_WIN01_BG3 |
               WINOUT_WIN01_OBJ | WINOUT_WIN01_CLR;
  REG_WIN0H = WIN_RANGE(DISPLAY_WIDTH / 2, DISPLAY_WIDTH / 2);
  REG_WIN0V = WIN_RANGE(24, 72);
  PaletteFade_SetSource(PALETTE_TINT_NONE, 0);
  PaletteFade_SetTarget(PALETTE_TINT_DIM_BRIGHT, 0);
  PaletteFade_StartInterpolation(24);
  REG_DISPCNT ^= DISPCNT_BG0_ON;
  left = DISPLAY_WIDTH / 2;
  right = DISPLAY_WIDTH / 2;
  for (column = 0; column < HEADING_OPEN_FRAMES; column++) {
    left -= HEADING_OPEN_STEP;
    right += HEADING_OPEN_STEP;
    REG_WIN0H = (left << 8) | right;
    wait(1);
  }
}

/// Waits up to two seconds, stopping early when a button is pressed.
void Event_WaitTwoSecondsOrPress(void) {
  u32 i;

  for (i = 0; i < 2 * FRAMES_PER_SECOND; i++) {
    wait(1);
    if (gInput.pressed != 0)
      break;
  }
}

/// Clears pixel `pixel` (its row times 8 plus its column) of the 4bpp tile at
/// (x, y) of the heading's picture, and the pixels left of it in its
/// halfword: a tile's row is two halfwords of four pixels each, the leftmost
/// in the low bits, so halfword pixel >> 2 loses its low (pixel & 3) + 1
/// pixels.
void Heading_WipeTilePixel(s32 x, s32 y, u32 pixel) {
  u16 *cell =
      (u16 *)(HEADING_TILES + (y * HEADING_COLUMNS + x) * TILE_SIZE_4BPP);

  cell += pixel >> 2;

  switch (pixel & 3) {
  case 0:
    *cell &= 0xfff0;
    break;
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

extern s16 gHeading_WipeSteps[];
/// gHeading_WipeSteps' last entry, after its last diagonal.
#define HEADING_WIPE_END (-9)

/// Unused: only the far-call table refers to it. Closes Heading_Show's
/// picture window: fades the palette out while wiping every one of the
/// picture's 26 by 5 tiles from its top left corner to its bottom right, a
/// diagonal every two frames (gHeading_WipeSteps: a tile's pixels a diagonal at
/// a time, each ended by a negative entry and the last by HEADING_WIPE_END),
/// then turns BG0 and the window off, clears the screen block, reloads the
/// window tiles and turns BG0 back on.
void Heading_Close(void) {
  s32 step = 0;
  u32 x;
  u32 y;
  volatile u32 fill;

  PaletteFade_SetTarget(PALETTE_TINT_NONE, 0);
  PaletteFade_StartInterpolation(24);
  for (;;) {
    for (; gHeading_WipeSteps[step] >= 0; step++) {
      for (x = 0; x < HEADING_COLUMNS; x++) {
        for (y = 0; y < HEADING_ROWS; y++)
          Heading_WipeTilePixel(x, y, gHeading_WipeSteps[step]);
      }
    }
    wait(2);
    if (gHeading_WipeSteps[step] == HEADING_WIPE_END)
      break;
    step++;
  }
  REG_DISPCNT ^= DISPCNT_BG0_ON | DISPCNT_WIN0_ON;
  Dma_Fill32Cell(fill, HEADING_BLANK_CELL, (void *)BG_SCREEN_BLOCK(4),
                 BG_SCREEN_SIZE);
  Dma_Set(Resource_GetTableEntry(RESOURCE_WINDOW_TILES),
          (void *)BG_CHAR_BLOCK(0), DMA_COPY_WORDS(2048), REG_DMA3);
  REG_DISPCNT ^= DISPCNT_BG0_ON;
}

#undef HEADING_COLUMNS
#undef HEADING_ROWS
#undef HEADING_FIRST_TILE
#undef HEADING_TILES
#undef HEADING_TILE_BYTES
#undef HEADING_BLANK_CELL
#undef HEADING_OPEN_FRAMES
#undef HEADING_OPEN_STEP
#undef HEADING_WIPE_END
