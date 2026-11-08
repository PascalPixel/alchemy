#include "types.h"
#include "call.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
struct Actor *Actor_GetById(s32 id);
void Func_02001804(void);
void Func_02001fdc(void);
void Func_0200353c(void);
void Func_02003764(void);
void Func_02003be0(s32 srcX, s32 srcY, s32 destX, s32 destY, s32 width,
                   s32 height);

void Func_0200369c(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x204;
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
  Func_02001fdc();
  if (GameFlag_Test(SPLIT_CONSTANT(0x86c)) != 0) {
    Func_02003be0(3, 8, 30, 33, 1, 1);
    Func_02003be0(67, 3, 94, 28, 1, 5);
    Func_02003be0(3, 69, 30, 93, 1, 4);
  }
  {
    s32 entrance = *(u16 *)&gMainRoutineWork.entrance;

    if ((u16)(entrance - 11) <= 28) {
      Func_02001804();
      entrance <<= 16;
      entrance >>= 16;
      entrance += SPLIT_CONSTANT(0x315);
      GameFlag_SetBit(entrance);
    }
  }
  Func_0200353c();
  if (GameFlag_Test(265) == 0) {
    if (gMainRoutineWork.entrance > 10) {
      if (gMainRoutineWork.entrance <= 39)
        Func_02003764();
    }
  }
}
