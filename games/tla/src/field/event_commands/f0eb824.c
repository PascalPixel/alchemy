/// @file
/// The canvas sprite of the field Psynergy scenes.

#include "types.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"
#include "heap_release.h"
#include "task.h"
#include "canvas_sprite.h"

#define OBJ_TILES ((u8 *)0x06010000)
#define TILE_SIZE 32

/// Starts the canvas sprite of the field Psynergy scenes: its work, a cleared
/// 64x64 canvas copied into the sprite's tiles, and a map from each canvas
/// pixel (row by row) to its place in the tiles. Func_080eb6a0 draws the
/// sprite and Func_080eb594 uploads the canvas each frame;
/// BattleEffect_EndCanvasSprite ends it.
void Func_080eb824(void) {
  u8 *windowTiles = Ram_HeapSlots->windowTiles;
  struct CanvasSpriteWork *work =
      HeapSlot_Alloc(HEAP_SLOT_CANVAS_SPRITE, sizeof(*work));
  u8 *canvas = HeapSlot_Alloc(HEAP_SLOT_CANVAS, CANVAS_SIZE);
  u16 *map;
  s32 y;
  s32 x;

  do {
    Iwram_FillWords(work, sizeof(*work), 0);
  } while (0);
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
  map = HeapSlot_AllocEwram(HEAP_SLOT_CANVAS_MAP,
                            CANVAS_WIDTH * CANVAS_WIDTH * sizeof(u16));
  for (y = 0; y < CANVAS_WIDTH; y++) {
    for (x = 0; x < CANVAS_WIDTH; x++) {
      s32 row = y & 7;
      s32 column = x & 7;
      *map++ = ((((y >> 3) * 8 + (x >> 3)) * 8) + row) * 8 + column;
    }
  }

  Task_Add(Func_080eb6a0, TASK_PASS_VBLANK);
  Task_Add(Func_080eb594, TASK_PASS_FRAME - 20);
}
