#include "jupiter_room_7.h"

s32 Func_02000290(void) {
  s32 count = 0;

  if (GameFlag_Test(0xa10) || GameFlag_Test(0xa12) || GameFlag_Test(0xa14))
    count = 1;
  if (GameFlag_Test(0xa11) || GameFlag_Test(0xa13) || GameFlag_Test(0xa15))
    count++;
  return count;
}
