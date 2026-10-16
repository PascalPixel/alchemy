// Near miss, differ 2 of 98 (2026-10-15, written from the listing; ☀️ has no
// twin): one reordered pair in the clear-after-upload case, where the native
// loads IwramFillWords' entry between the two halves of the size (movs r1,
// #128; ldr r3; adds r0, r4; lsls r1, r1, #5) and this finishes the size
// first. Measured, each still 2: the entry in a local pointer, the size in a
// local, an empty asm on the size, on the buffer, on the entry, and a
// one-pass block around the call. An asm on the entry with the size split by
// hand (128, then << 5) leaves only the buffer copy out of place (2). Case 2
// comes before case 1 in the source: its body comes first in the ROM
// (differ 18 the other way).
#include "types.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"
#include "canvas_sprite.h"

#define OBJ_TILES ((u8 *)0x06010000)
#define BG_VRAM ((void *)0x06000000)
#define TILE_SIZE 32

void ColorBuffer_MapToFadeStep1(u8 *buffer, s32 count);
void ColorBuffer_MapToFadeStep2(u8 *buffer, s32 count);
void ColorBuffer_MapToFadeStep3(u8 *buffer, s32 count);

// The canvas's frame callback: counts the frame and carries out the command
// the effect left for it, copying the canvas into the sprite's tiles (or to
// the start of BG VRAM), and clearing or fading it after the copy.
void Func_080eb594(void) {
  struct CanvasSpriteWork *work = Ram_HeapSlots->unkec[1];
  u8 *canvas = Ram_HeapSlots->battleFxWork[1];
  u8 *tiles = OBJ_TILES + work->tile * TILE_SIZE;

  work->frame++;
  switch (work->command) {
  case CANVAS_UPLOAD:
    Dma_Set(canvas, tiles, 0x84000000 | CANVAS_SIZE / 4, REG_DMA3);
    break;
  case CANVAS_UPLOAD_AND_CLEAR:
    Dma_Set(canvas, tiles, 0x84000000 | CANVAS_SIZE / 4, REG_DMA3);
    Iwram_FillWords(canvas, CANVAS_SIZE, 0);
    break;
  case CANVAS_UPLOAD_AND_FADE1:
    Dma_Set(canvas, tiles, 0x84000000 | CANVAS_SIZE / 4, REG_DMA3);
    ColorBuffer_MapToFadeStep1(canvas, CANVAS_SIZE);
    break;
  case CANVAS_UPLOAD_AND_FADE2:
    Dma_Set(canvas, tiles, 0x84000000 | CANVAS_SIZE / 4, REG_DMA3);
    ColorBuffer_MapToFadeStep2(canvas, CANVAS_SIZE);
    break;
  case CANVAS_UPLOAD_AND_FADE3:
    Dma_Set(canvas, tiles, 0x84000000 | CANVAS_SIZE / 4, REG_DMA3);
    ColorBuffer_MapToFadeStep3(canvas, CANVAS_SIZE);
    break;
  case CANVAS_UPLOAD_TO_BG:
    Dma_Set(canvas, BG_VRAM, 0x84000000 | CANVAS_SIZE * 2 / 4, REG_DMA3);
    break;
  }
  work->command = 0;
}
