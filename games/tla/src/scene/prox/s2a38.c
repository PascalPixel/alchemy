// Unless flag 0x109 is set, calls Func_02001c24 with 0 for entrance 6 and with
// 1 for entrances 7 and 8, then schedules Func_02002a08. The entrance is read
// unsigned first and tested signed against 6, as the native reads it twice.
#include "types.h"
#include "adventure.h"

s32 GameFlag_Test(s32 flag);
void Task_Add(void (*func)(void), s32 priority);
void Func_02001c24(s32 value);
void Func_020045dc(s32 value);
void Func_02002a08(void);

void Func_02002a38(void) {
  s32 priority;

  if (GameFlag_Test(0x109) == 0) {
    s32 entrance = (u16)gAdventure.entrance;

    if (gAdventure.entrance == 6)
      Func_02001c24(0);
    else if ((u32)((entrance - 7) << 16) <= 0x10000)
      Func_02001c24(1);
  }
  Func_020045dc(0);
  // The priority in a local is built before the task function's address (passed
  // directly, the address loads first).
  priority = 0x480;
  Task_Add(Func_02002a08, priority);
}
