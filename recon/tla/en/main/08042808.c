// Near miss, differ 4 of 170 (tla-en, 2026-10-31): two reordered pairs, both
// sched2 priority. The native copies the y argument (adds r5, r2, #0) before
// it loads the work's mode byte (ldrb r3, [r3, #5]); here the ldrb is ready a
// cycle after the store of `filled` (dependence cost 1) and outranks the copy
// (priority 4 against 1). The native stores the row's result before it builds
// the row step 1 (movs r2, #1); here the constant outranks the leaf store
// (priority 2 against 1). Both native orders would follow if a store cost two
// cycles (untested). The permuter (16,604 candidates) found nothing closer.
// Earlier: differ 6 under -mthumb-slow-loads (2026-10-26), where no single
// empty asm, one-pass block, wrapping block or swap of neighbouring
// statements brought it closer (device search).
// Twin: The Broken Seal's PartyStatus_DrawBars, the window's x and y read as
// signed halfwords.

#include "types.h"
#include "ram_buffer.h"
#include "window.h"
#include "dma.h"
#include "io_reg.h"
#include "render_input.h"

#define TILE_ROW_FROM_BOTTOM(tile, up)                                         \
  ((u32 *)((tile) * 32 - (up) * 4 + 0x0600001c))

s32 Palette_GetBasicObjColors(void);

s32 Func_08042808(struct RenderInput *window, s32 x, s32 y, s32 value) {
  s32 i;
  struct WindowWork *canvas =
      (struct WindowWork *)Ram_HeapSlots->windowTiles;
  s32 filled = value;
  s32 row;
  s32 col;
  u32 id;
  u32 light;
  u32 dark;
  u32 pixels;
  u32 result;

  if (canvas->menuState == 0) {
    Dma_Set((const void *)Palette_GetBasicObjColors(),
            (void *)&BG_PLTT_COLOR(14, 0), 0x80000010, REG_DMA3);
    BG_PLTT_COLOR(14, 14) = BG_PLTT_COLOR(15, 4);
  }
  x += (s16)window->x;
  y += (s16)window->y;
  for (i = 0; i < 5; i++, x++) {
    id = canvas->tilemap[0][y * 32 + x];
    light = 0x22222222;
    dark = 0xcccccccc;
    id &= 0x3ff;
    if (value > 7) {
      light = 0x88888888;
      dark = 0xdddddddd;
    } else if (value >= 0) {
      light <<= value * 4;
      light |= 0x88888888U >> (32 - value * 4);
      dark <<= value * 4;
      dark |= 0xddddddddU >> (32 - value * 4);
    }
    for (row = 0; row <= (filled != 0 ? 2 : 0); row++) {
      pixels = *TILE_ROW_FROM_BOTTOM(id, row);
      result = 0;
      for (col = 0; col <= 7; col++) {
        u32 color = pixels & 15;

        if (color == 14)
          result |= light & (15U << (col * 4));
        else if (color == 1)
          result |= dark & (15U << (col * 4));
        else
          result |= color << (col * 4);
        pixels >>= 4;
      }
      *TILE_ROW_FROM_BOTTOM(id, row) = result;
    }
    value -= 8;
  }
}
