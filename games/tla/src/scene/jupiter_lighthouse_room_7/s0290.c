#include "jupiter_lighthouse_room_7.h"
#include "event_inline.h"

s32 Func_02000290(void) {
  s32 count = 0;

  if (Event_GetFlag(0xa10) || Event_GetFlag(0xa12) || Event_GetFlag(0xa14))
    count = 1;
  if (Event_GetFlag(0xa11) || Event_GetFlag(0xa13) || Event_GetFlag(0xa15))
    count++;
  return count;
}
