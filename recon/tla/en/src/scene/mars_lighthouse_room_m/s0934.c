#include "types.h"
#include "io_write_queue.h"
#include "call.h"

extern s16 Data_02003a7c;
extern s16 gData_02003a7e;
extern s16 gData_02003a80;
extern s16 gData_02003a82;

void Func_0200055c(void);
void Func_02003158(void);
void Func_02003178(s32 a, s32 b, s32 c, s32 d, s32 x, s32 z);
s32 Task_Add(void (*func)(void), s32 priority);

void Func_02000934(s32 entrance) {
  s32 cell = -1;
  s16 *blend;
  Func_02003158();
  blend = &Data_02003a7c;
  if (entrance == 8)
    cell = 20;
  if (entrance == 10)
    cell = 22;
  if (entrance == 12)
    cell = 24;
  if (entrance == 14)
    cell = 26;
  if (cell == -1) {
    *blend = 0;
    gData_02003a7e = 0;
    gData_02003a80 = cell;
    gData_02003a82 = cell;
  } else {
    s32 step;
    *blend = 15;
    gData_02003a7e = 16;
    step = (cell - 20) / 2;
    gData_02003a80 = step;
    gData_02003a82 = cell;
    if (step == 0)
      Call6((void (*)())Func_02003178, 0, 125, 3, 3, 17, 67);
    if (gData_02003a80 == 1)
      Call6((void (*)())Func_02003178, 0, 125, 3, 3, 8, 72);
    if (gData_02003a80 == 2)
      Call6((void (*)())Func_02003178, 0, 125, 3, 3, 17, 72);
    if (gData_02003a80 == 3)
      Call6((void (*)())Func_02003178, 0, 125, 3, 3, 8, 67);
  }
  IO_WRITE_QUEUE_PUSH(0x04000052, Data_02003a7c | (16 - Data_02003a7c) << 8,
                      0x20000);
  IO_WRITE_QUEUE_PUSH(0x04000050, 0x3f42, 0x20000);
  Task_Add(Func_0200055c, 0x480);
}
