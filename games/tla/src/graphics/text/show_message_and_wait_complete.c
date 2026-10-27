#include "types.h"

s32 UiWork_IsComplete(void);
s32 UiText_OpenMessageWindow(s32, s32, s32, s32);

s32 UiText_ShowMessageAndWaitComplete(s32 message, s32 x, s32 y) {
  s32 result;

  result = UiText_OpenMessageWindow(message, x, y, 1);
  while (UiWork_IsComplete() == 0)
    WaitFrames(1);
  return result;
}
