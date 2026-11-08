#include "types.h"
#include "ram_buffer.h"
#include "window.h"
#include "uiwindow.h"
#include "resource.h"
#include "heap_release.h"

void Func_08044348(struct Window *window, const void *resource) {
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
    for (col = 0; col < window->width;) {
      s16 tile = *source;

      col++;
      /* FAKEMATCH(one-pass): the two stores in a block run once; as the two
       * stores alone, 2 of 66 instructions differ in tla-en. */
      do {
        *vram = tile;
        *map = tile;
      } while (0);
      source++;
      vram++;
      map++;
    }
    vram += 32 - window->width;
    map += 32 - window->width;
  }
  Heap_Release(buffer);
}
