#include "types.h"

s32 Message_AllTyped(void);
s32 Message_OpenWindow(s32, s32, s32, s32);

s32 Message_ShowAndWait(s32 message, s32 x, s32 y) {
  s32 result;

  result = Message_OpenWindow(message, x, y, 1);
  while (Message_AllTyped() == 0)
    WaitFrames(1);
  return result;
}
