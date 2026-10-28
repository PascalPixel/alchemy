#include "types.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"
#include "canvas_sprite.h"

#define OBJ_TILES ((u8 *)0x06010000)
#define BG_VRAM ((void *)0x06000000)
#define TILE_SIZE 32

/// The canvas in words, as one 32-bit DMA.
#define CANVAS_DMA DMA_COPY_WORDS(CANVAS_SIZE / 4)

void ColorBuffer_MapToFadeStep1(u8 *buffer, s32 count);
void ColorBuffer_MapToFadeStep2(u8 *buffer, s32 count);
void ColorBuffer_MapToFadeStep3(u8 *buffer, s32 count);

/// The canvas's frame callback: counts the frame and carries out the command
/// the effect left for it, copying the canvas into the sprite's tiles (or to
/// the start of BG VRAM), and clearing or fading it after the copy. Case 2
/// comes before case 1: its body comes first in the ROM.
void Func_080eb594(void) {
  struct CanvasSpriteWork *work = Ram_HeapSlots->unkec[1];
  u8 *canvas = Ram_HeapSlots->battleFxWork[1];
  u8 *tiles = OBJ_TILES + work->tile * TILE_SIZE;

  work->frame++;
  switch (work->command) {
  case CANVAS_UPLOAD:
    Dma_Set(canvas, tiles, CANVAS_DMA, REG_DMA3);
    break;
  case CANVAS_UPLOAD_AND_CLEAR:
    Dma_Set(canvas, tiles, CANVAS_DMA, REG_DMA3);
    {
      s32 size = 128;
      /* FAKEMATCH(register): the canvas pinned to the r0 it is passed in, so
       * its copy comes after the entry's load; as Iwram_FillWords(canvas,
       * CANVAS_SIZE, 0), 2 of 98 instructions differ in tla-en. */
      register u8 *buffer asm("r0");
      s32 (*fill)(void *, s32, u32);

      fill = Iwram_FillWords;
      /* FAKEMATCH(barrier): the size's first half (128) and IwramFillWords'
       * entry are loaded before the canvas is copied; as Iwram_FillWords(
       * canvas, CANVAS_SIZE, 0), 2 of 98 instructions differ in tla-en. */
      asm volatile("" : "+l"(size), "+l"(fill));
      buffer = canvas;
      /* FAKEMATCH(barrier): the size's shift comes after the canvas's copy;
       * as Iwram_FillWords(canvas, CANVAS_SIZE, 0), 2 of 98 instructions
       * differ in tla-en. */
      asm("" : "+l"(size) : "l"(fill), "l"(buffer));
      fill(buffer, size << 5, 0);
    }
    break;
  case CANVAS_UPLOAD_AND_FADE1:
    Dma_Set(canvas, tiles, CANVAS_DMA, REG_DMA3);
    ColorBuffer_MapToFadeStep1(canvas, CANVAS_SIZE);
    break;
  case CANVAS_UPLOAD_AND_FADE2:
    Dma_Set(canvas, tiles, CANVAS_DMA, REG_DMA3);
    ColorBuffer_MapToFadeStep2(canvas, CANVAS_SIZE);
    break;
  case CANVAS_UPLOAD_AND_FADE3:
    Dma_Set(canvas, tiles, CANVAS_DMA, REG_DMA3);
    ColorBuffer_MapToFadeStep3(canvas, CANVAS_SIZE);
    break;
  case CANVAS_UPLOAD_TO_BG:
    Dma_Set(canvas, BG_VRAM, DMA_COPY_WORDS(CANVAS_SIZE * 2 / 4), REG_DMA3);
    break;
  }
  work->command = 0;
}
