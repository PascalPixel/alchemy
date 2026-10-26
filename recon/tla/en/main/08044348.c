// Draft, differ 2 of 66 in tla-en under -mthumb-slow-loads (2026-10-26). No
// single empty asm, one-pass block, wrapping block or swap of neighbouring
// statements brings it closer, nor any pair of empty asm statements or
// one-pass blocks (device search).
#include "types.h"
#include "ram_buffer.h"
#include "window.h"
#include "uiwindow.h"
#include "resource.h"
#include "mem.h"

void UiWindow_DrawTilemapResource(struct UiWindow *window,
                                  const void *resource) {
  struct UiRenderWork *work = (struct UiRenderWork *)Ram_HeapSlots->windowTiles;
  u16 *buffer = Runtime_BumpAllocateAlternatePool(0x300);
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
  MemFree(buffer);
}
