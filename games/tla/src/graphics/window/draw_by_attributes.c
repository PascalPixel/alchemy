#include "types.h"
#include "window.h"
#include "uiwindow.h"
#include "ram_buffer.h"
#include "iwram_call.h"

#define UI_WORK ((struct UiRenderWork *)Ram_HeapSlots->windowTiles)

void UiWindow_DrawFrame(s32 x, s32 y, s32 width, s32 height);
void Func_0803a2b0(s32 x, s32 y, s32 width, s32 height, s32 mode);

static inline void FillWords(void *destination, s32 size, u32 value) {
  Iwram_FillWordsEntry(destination, size, value);
}

/// Draws a window as its flags ask: with flag 8 its inside is filled (with
/// colour 4 under flag 0x20, which also draws the frame, else cleared), and
/// without it only the frame is drawn. Clears the window's duration.
void UiWork_DrawByAttributes(struct UiWindow *window) {
  u32 flags;
  s32 x;
  s32 y;
  u32 width;
  u32 height;
  struct UiRenderWork *work;

  work = UI_WORK;
  x = (s16)window->x;
  y = (s16)window->y;
  width = window->width;
  height = window->height;
  flags = window->flags;
  window->duration = 0;
  if (8 & flags) {
    if (0x20 & flags) {
      UiWindow_DrawFrame(x, y, width, height);
      FillWords((void *)0x06002500, 0xF00, 0x44444444);
    } else {
      FillWords((void *)0x06002500, 0xF00, 0);
    }
    Func_0803a2b0(x, y, width, height, 0);
  } else {
    UiWindow_DrawFrame(x, y, width, height);
  }
  work->dirty = 1;
}
