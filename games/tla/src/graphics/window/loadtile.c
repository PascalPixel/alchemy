#include "types.h"
#include "dma.h"
#include "resource.h"

struct Rect_0803cba8 {
  u8 unk00[8];
  u16 width;
  u16 height;
  s16 x;
  s16 y;
  u8 unk10[2];
  u16 state;
};

extern char ResourceId_WindowBorderFlash;

void Func_0803a1c0(s32 x, s32 y, s32 width, s32 height);

void Func_0803cba8(struct Rect_0803cba8 **window, s32 offset) {
  u8 *source =
      (u8 *)Resource_GetTableEntry((s32)&ResourceId_WindowBorderFlash) + offset;
  struct Rect_0803cba8 *rect = *window;
  s32 x = rect->x;
  s32 y = rect->y;
  s32 width = rect->width;
  s32 height = rect->height;

  Dma_Set(source, (void *)0x06000100, DMA_COPY_WORDS(64), REG_DMA3);
  Func_0803a1c0(x, y, width, height);
}

struct Window_0803cbf8 {
  struct Rect_0803cba8 *rect;
  u8 unk04[16];
  u16 timer;
};

void Window_DrawBorder(s32 x, s32 y, s32 width, s32 height);

void Func_0803cbf8(struct Window_0803cbf8 *window) {
  if (window->rect->state == 4) {
    if (window->timer == 10)
      Func_0803cba8(&window->rect, 0x300);
    else if (window->timer == 9)
      Func_0803cba8(&window->rect, 0x200);
    else if (window->timer == 8)
      Func_0803cba8(&window->rect, 0x100);
    else if (window->timer == 6)
      Func_0803cba8(&window->rect, 0x200);
    else if (window->timer == 4)
      Func_0803cba8(&window->rect, 0x300);
    if (window->timer == 2) {
      struct Rect_0803cba8 *rect = window->rect;

      Window_DrawBorder(rect->x, rect->y, rect->width, rect->height);
    }
    window->timer--;
    if (window->timer == 0) {
      Dma_Set(Resource_GetTableEntry((s32)&ResourceId_WindowBorderFlash),
              (void *)0x06000100, 0x84000040, REG_DMA3);
      window->rect->state = 0;
    }
  }
}
