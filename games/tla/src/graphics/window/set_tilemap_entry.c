#include "types.h"
#include "ram_buffer.h"
#include "window.h"

enum {
  TILEMAP_ENTRY_PLAIN,
  TILEMAP_ENTRY_NONE,
  TILEMAP_ENTRY_PALETTE_14,
  TILEMAP_ENTRY_PALETTE_15,
  TILEMAP_ENTRY_PALETTE_1
};

#define TILEMAP_WIDTH 32
#define TILEMAP_ENTRIES (TILEMAP_WIDTH * 20)

void UiWindow_SetTilemapEntry(struct UiWindow *window, s32 value, s32 x, s32 y,
                              u32 mode) {
  u16 *map = ((struct UiRenderWork *)Ram_HeapSlots->windowTiles)->tilemap[0];
  s32 palette;
  s32 index;

  y += 1;
  x += 1;
  if ((u32)y > (u32)(window->height - 1))
    return;
  if ((u32)x > (u32)(window->width - 1))
    return;

  switch (mode) {
  case TILEMAP_ENTRY_PALETTE_15:
    palette = 0xf000;
    break;
  case TILEMAP_ENTRY_PALETTE_14:
    palette = 0xe000;
    break;
  case TILEMAP_ENTRY_PALETTE_1:
    palette = 0x1000;
    break;
  default:
    palette = 0;
    break;
  }

  /* FAKEMATCH: byte-offset stores preserve both strh base/offset register
   * pairs. */
  switch (mode) {
  case TILEMAP_ENTRY_NONE:
    return;
  case TILEMAP_ENTRY_PALETTE_14:
  case TILEMAP_ENTRY_PALETTE_15:
  case TILEMAP_ENTRY_PALETTE_1:
    index = ((s16)window->y + y) * TILEMAP_WIDTH + ((s16)window->x + x);
    if ((u32)index >= TILEMAP_ENTRIES)
      return;
    *(u16 *)((u8 *)map + (index << 1)) = palette | value;
    return;
  case TILEMAP_ENTRY_PLAIN:
  default:
    index = ((s16)window->y + y) * TILEMAP_WIDTH + ((s16)window->x + x);
    if ((u32)index >= TILEMAP_ENTRIES)
      return;
    *(u16 *)((u8 *)map + (index << 1)) = value;
    return;
  }
}
