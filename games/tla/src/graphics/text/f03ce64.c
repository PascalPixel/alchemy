#include "types.h"
#include "window.h"
#include "uiwindow.h"
#include "ram_buffer.h"

#define UI_WORK ((struct UiRenderWork *)Ram_HeapSlots->windowTiles)

s32 UiText_BuildRenderEntries(s32 key, s32 mode);
s32 UiText_GetResourceDimensions(s32 message, s32 *x, s32 *y, u32 *width,
                                 u32 *height);
struct UiWindow *UiWindow_Create(s32 x, s32 y, s32 width, s32 height,
                                 s32 attrs);
s32 UiText_QueueRenderEntries(struct UiWindow *window, s32 entry, s32 x, s32 y,
                              u16 *bounds, s32 mode);
s32 UiWork_IsComplete(void);
s32 UiWork_IsIdle(struct UiWindow *window);
void WaitFrames(s32 frames);

void Func_0803ce64(s32 message, s32 mode, s32 yOffset) {
  struct UiRenderWork *work;
  struct UiWindow *window;
  s32 x;
  s32 y;
  s32 width;
  s32 height;
  s32 entry;

  work = UI_WORK;
  x = 8;
  y = 8;
  window = NULL;
  entry = UiText_BuildRenderEntries(message, 1);
  if (work->entries[entry] != 0) {
    UiText_GetResourceDimensions(message, &x, &y, (u32 *)&width,
                                 (u32 *)&height);
    x = (30 - width) >> 1;
    y = ((15 - height) >> 1) + yOffset;
    if (mode != 0)
      window = UiWindow_Create(x, y, width, height, 0);
    else {
      window = UiWindow_Create(x, y, 0, 0, 2);
      window->width = mode;
      window->height = mode;
    }
    if (UiText_QueueRenderEntries(window, entry, 0, 0, 0, 0) == 0)
      UiWork_Finalize(window, 1);
    else {
      while (UiWork_IsComplete() == 0)
        WaitFrames(1);
      if (mode != 0) {
        UiWork_Finalize(window, 0);
        while (UiWork_IsIdle(window) == 0)
          WaitFrames(1);
      } else
        UiWork_Finalize(window, 1);
      work->speakerVoiceSound = 0;
      work->speakerVoiceCountdown = 0;
    }
  }
}
