#include "types.h"
#include "window.h"
#include "uiwindow.h"
#include "ram_buffer.h"

#define UI_WORK ((struct WindowWork *)Ram_HeapSlots->windowTiles)

s32 Message_Decode(s32 key, s32 mode);
s32 Text_GetMessageSize(s32 message, s32 *x, s32 *y, u32 *width,
                                 u32 *height);
struct Window *Window_Create(s32 x, s32 y, s32 width, s32 height,
                                 s32 attrs);
s32 Message_Start(struct Window *window, s32 entry, s32 x, s32 y,
                              u16 *bounds, s32 mode);
s32 Message_AllTyped(void);
s32 Window_IsClosed(struct Window *window);
void WaitFrames(s32 frames);

void Func_0803ce64(s32 message, s32 mode, s32 yOffset) {
  struct WindowWork *work;
  struct Window *window;
  s32 x;
  s32 y;
  s32 width;
  s32 height;
  s32 entry;

  work = UI_WORK;
  x = 8;
  y = 8;
  window = NULL;
  entry = Message_Decode(message, 1);
  if (work->entries[entry] != 0) {
    Text_GetMessageSize(message, &x, &y, (u32 *)&width,
                                 (u32 *)&height);
    x = (30 - width) >> 1;
    y = ((15 - height) >> 1) + yOffset;
    if (mode != 0)
      window = Window_Create(x, y, width, height, 0);
    else {
      window = Window_Create(x, y, 0, 0, 2);
      window->width = mode;
      window->height = mode;
    }
    if (Message_Start(window, entry, 0, 0, 0, 0) == 0)
      Window_Close(window, 1);
    else {
      while (Message_AllTyped() == 0)
        WaitFrames(1);
      if (mode != 0) {
        Window_Close(window, 0);
        while (Window_IsClosed(window) == 0)
          WaitFrames(1);
      } else
        Window_Close(window, 1);
      work->speakerVoiceSound = 0;
      work->speakerVoiceCountdown = 0;
    }
  }
}
