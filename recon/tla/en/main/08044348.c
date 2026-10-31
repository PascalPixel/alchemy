// Near miss, differ 2 of 66 (tla-en, 2026-10-30): scheduler cost model. In the
// inner loop the listing advances source before reloading window->width; here
// the reload comes first. sched2 shows why: the reload depends on the two
// stores, each of latency 1 under -mthumb-slow-loads, so it is ready the cycle
// after the second store and its priority (4) outranks source++ (1); the
// listing's order follows if a store feeding a dependent load costs 2 cycles.
// An empty asm after source++ (col++ moved before it, vram assigned before map
// to keep the allocation) fixes the tail but puts source++ between the two
// stores (the output-dependence tie-break), still differ 2. No single empty
// asm, one-pass block, wrapping block or swap of neighbouring statements
// brings it closer, nor any pair of empty asm statements or one-pass blocks
// (device search); a 4-minute permuter run found nothing closer.
#include "types.h"
#include "ram_buffer.h"
#include "window.h"
#include "uiwindow.h"
#include "resource.h"
#include "heap_release.h"

void Window_DrawTilemapResource(struct Window *window,
                                  const void *resource) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowTiles;
  u16 *buffer = Heap_AllocEwram(0x300);
  s32 offset;
  u16 *map;
  u16 *vram;
  u16 *source;
  s32 row;
  s32 col;

  source = buffer;
  Resource_DecodeType01(resource, buffer);
  offset = (((s16)window->y << 5) + (s16)window->x) * 2;
  map = (u16 *)((u8 *)work->tilemap + offset);
  vram = (u16 *)(offset + 0x06002000);
  for (row = 0; row < window->height; row++) {
    for (col = 0; col < window->width; col++) {
      s16 tile = *source++;

      *vram++ = tile;
      *map++ = tile;
    }
    vram += 32 - window->width;
    map += 32 - window->width;
  }
  Heap_Release(buffer);
}
