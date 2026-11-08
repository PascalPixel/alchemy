#include "types.h"
#include "window.h"
#include "ram_buffer.h"
#include "dma.h"
#include "io_reg.h"

void Render_SetTilemapFlagRect(const struct Window *object, s32 x, s32 y,
                               s32 width, s32 height, u32 field) {
  struct WindowWork *work = (struct WindowWork *)Ram_HeapSlots->windowTiles;

  x += (s16)object->x + 1;
  y += (s16)object->y + 1;
  field <<= 12;
  if (x < 0) {
    width += x;
    x = 0;
  }
  if (x + width > 29) {
    width = 30 - x;
  }
  if (y < 0) {
    height += y;
    y = 0;
  }
  if (y + height > 29) {
    height = 20 - y;
  }
  if (width > 0 && height > 0) {
    do {
      u16 *pixel = &work->tilemap[y][x];
      s32 remaining = width;
      while (remaining != 0) {
        u32 value = *pixel;
        value &= 0xFFFFEFFF;
        value |= field;
        remaining--;
        *pixel = value;
        pixel++;
      }
      height--;
      y++;
    } while (height != 0);
    work->dirty = 1;
  }
}

void Palette_CopyObjectBankToBackground14(void) {
  Dma_Set((const void *)OBJ_PLTT, (void *)&BG_PLTT_COLOR(14, 0),
          DMA_COPY_HALFWORDS(16), REG_DMA3);
  Dma_Set((const void *)&BG_PLTT_COLOR(15, 4), (void *)&BG_PLTT_COLOR(14, 14),
          DMA_COPY_HALFWORDS(1), REG_DMA3);
}
