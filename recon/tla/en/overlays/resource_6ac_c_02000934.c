// Draft, differ 78 of 159: rewritten from the listing. Sets room 1's blend for
// its entrance (none, or 15 and 16 with a step of (cell - 20) / 2), changes the
// cells the step selects, queues BLDALPHA and BLDCNT writes and schedules
// Func_0200055c. Left: the native compares the step global with 1, 2 and 3 as
// words (ldrsh, cmp) where this narrows each test to a halfword compare (ldrh,
// shifts), and it reads Data_02003a7c's address once before the entrance tests;
// a word local reloaded before each test or a reversed compare do not help.
#include "types.h"
#include "io_write_queue.h"

extern s16 Data_02003a7c;
extern s16 gData_02003a7e;
extern s16 gData_02003a80;
extern s16 gData_02003a82;

void Func_0200055c(void);
void Func_02003158(void);
void Func_02003178(s32 a, s32 b, s32 c, s32 d, s32 x, s32 z);
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);

void Func_02000934(s32 entrance) {
  s32 cell = -1;

  Func_02003158();
  if (entrance == 8)
    cell = 20;
  if (entrance == 10)
    cell = 22;
  if (entrance == 12)
    cell = 24;
  if (entrance == 14)
    cell = 26;
  if (cell == -1) {
    Data_02003a7c = 0;
    gData_02003a7e = 0;
    gData_02003a80 = cell;
    gData_02003a82 = cell;
  } else {
    s32 step;

    Data_02003a7c = 15;
    gData_02003a7e = 16;
    step = (cell - 20) / 2;
    gData_02003a80 = step;
    gData_02003a82 = cell;
    if (step == 0)
      Func_02003178(0, 125, 3, 3, 17, 67);
    if (gData_02003a80 == 1)
      Func_02003178(0, 125, 3, 3, 8, 72);
    if (gData_02003a80 == 2)
      Func_02003178(0, 125, 3, 3, 17, 72);
    if (gData_02003a80 == 3)
      Func_02003178(0, 125, 3, 3, 8, 67);
  }
  IO_WRITE_QUEUE_PUSH(0x04000052, Data_02003a7c | (16 - Data_02003a7c) << 8,
                      0x20000);
  IO_WRITE_QUEUE_PUSH(0x04000050, 0x3f42, 0x20000);
  Scheduler_AddOrUpdateCallback(Func_0200055c, 0x480);
}
