// Draft, differ 14 of 68: the twin's order of reads with x and y read signed
// (the native loads them with ldrsh) and the fill through a forwarding inline
// (the native loads the routine's address first, as through ☀️'s IWRAM call
// layer). The native reads x, height, the work, flags and y before it clears
// duration and reads width last, with x in r5 and width in r7; read in that
// order here x and width swap registers (differ 24). Measured: typed s16 and
// u16 locals (differ 22 to 87), x and y first, three minutes of permuter
// (differ 22).
#include "types.h"
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

void UiWork_DrawByAttributes(struct UiWindow *window) {
  u32 flags;
  u32 value;
  s32 x;
  s32 y;
  u32 width;
  u32 height;
  struct UiRenderWork *work;

  work = UI_WORK;
  value = window->height;
  flags = window->flags;
  height = value;
  value = 0;
  window->duration = value;
  x = (s16)window->x;
  y = (s16)window->y;
  width = window->width;
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
