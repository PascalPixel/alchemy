// Near miss, differ 22 of 116 (2026-10-15, written from the listing; ☀️ has no
// twin). The command's 0 comes from the literal pool, as in the ROM (a
// halfword zero kept in a register). What remains: the pixel map loop gives the
// row r5 and its tile row r6 where the ROM has them the other way round (14
// rows); the fill routine's entry is loaded after the canvas pointer's copy;
// and the literal pool lands one call later. Measured, no better: every order
// of the four locals' declarations (22), the tile row and pixel row in locals
// (24), s16 map entries, the store apart from the increment (22), other
// formulas for the tile offset (22-53); permute 6 min, 2 jobs: nothing below
// 22.
#include "types.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"
#include "mem.h"
#include "callback_scheduler.h"
#include "canvas_sprite.h"

#define OBJ_TILES ((u8 *)0x06010000)
#define TILE_SIZE 32

// Starts the canvas sprite of the field Psynergy scenes: its work, a cleared
// 64x64 canvas copied into the sprite's tiles, and a map from each canvas
// pixel (row by row) to its place in the tiles. Func_080eb6a0 draws the
// sprite and Func_080eb594 uploads the canvas each frame; Func_080eb930 ends
// it.
void Func_080eb824(void) {
  u8 *windowTiles = Ram_HeapSlots->windowTiles;
  struct CanvasSpriteWork *work =
      Runtime_AllocateHeapBlock(HEAP_SLOT_CANVAS_SPRITE, sizeof(*work));
  u8 *canvas = Runtime_AllocateHeapBlock(HEAP_SLOT_CANVAS, CANVAS_SIZE);
  u16 *map;
  s32 y;
  s32 x;

  Iwram_FillWords(work, sizeof(*work), 0);
  work->tile = *(u16 *)(windowTiles + 0x1348);
  work->mode = CANVAS_SPRITE_16_COLORS;
  work->command = 0;
  work->x = 16;
  work->y = 0;
  work->scaleX = 0x10000;
  work->scaleY = 0x10000;
  work->priority = 1;
  work->depth = 250;
  Iwram_FillWords(canvas, CANVAS_SIZE, 0);
  Dma_Set(canvas, OBJ_TILES + work->tile * TILE_SIZE,
          0x84000000 | CANVAS_SIZE / 4, REG_DMA3);

  // Where each pixel of the canvas sits in its 8x8 tiles.
  map = Runtime_AllocateBlock(HEAP_SLOT_CANVAS_MAP,
                              CANVAS_WIDTH * CANVAS_WIDTH * sizeof(u16));
  for (y = 0; y < CANVAS_WIDTH; y++) {
    for (x = 0; x < CANVAS_WIDTH; x++)
      *map++ = ((((y >> 3) * 8 + (x >> 3)) * 8) + (y & 7)) * 8 + (x & 7);
  }

  Scheduler_AddOrUpdateCallback(Func_080eb6a0, 0x480);
  Scheduler_AddOrUpdateCallback(Func_080eb594, 0xc6c);
}
