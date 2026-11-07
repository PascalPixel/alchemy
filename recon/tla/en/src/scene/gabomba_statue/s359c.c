#include "types.h"
#include "main_routine_work.h"

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void Func_02002394(void);
void Func_02005824(void);
void Func_0200583c(s32 index, s32 flag, s32 a, s32 b);

void Func_0200359c(void) {
  Func_02005824();
  Func_0200583c(0, 513, 31, 32);
  if (!GameFlag_Test(0x109)) {
    s32 entrance = (u16)gMainRoutineWork.entrance;

    if ((u16)(entrance - 7) <= 1)
      Func_02002394();
    if (gMainRoutineWork.entrance == 7)
      GameFlag_SetBit(0x204);
  }
}
