#include "types.h"
#include "ram_buffer.h"
#include "window.h"

s32 Message_Decode(s32, s32);
void Window_FitOnScreen(s32 idx, s32 *x, s32 *y, u32 *width, u32 *height,
                          s32 arg5, s32 flags);

void Text_MeasureMessage(s32 no, s32 *x, s32 *y) {
  Text_MeasureEntry(Message_Decode(no, 0), x, y, 0);
}

/// Builds the render entries of message no and fits its window on screen,
/// passing flags to Window_FitOnScreen; returns 0 when the message is empty.
#define DEFINE_GET_RESOURCE_DIMENSIONS(name, flags)                            \
  s32 name(s32 no, s32 *x, s32 *y, u32 *width, u32 *height) {                  \
    struct WindowWork *work;                                                 \
    s32 idx;                                                                   \
                                                                               \
    work = (struct WindowWork *)Ram_HeapSlots->windowTiles;                  \
    idx = Message_Decode(no, 0);                                    \
    if (work->entries[idx] == 0) {                                             \
      return 0;                                                                \
    }                                                                          \
    Window_FitOnScreen(idx, x, y, width, height, 0, flags);                  \
    return 1;                                                                  \
  }

DEFINE_GET_RESOURCE_DIMENSIONS(Text_GetMessageSize, 0)
DEFINE_GET_RESOURCE_DIMENSIONS(Text_GetMessageSizeAlt, 1)
