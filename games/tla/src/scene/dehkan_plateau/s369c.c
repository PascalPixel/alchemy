#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

void Func_02001804(void);
void Func_02001fdc(void);
void Func_0200353c(void);
void Func_02003764(void);

void Func_0200369c(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x204;
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
  Func_02001fdc();
  if (GameFlag_Test(0x86c) != 0) {
    Map_CopyCellPictureIndicesRect(3, 8, 30, 33, 1, 1);
    Map_CopyCellPictureIndicesRect(67, 3, 94, 28, 1, 5);
    Map_CopyCellPictureIndicesRect(3, 69, 30, 93, 1, 4);
  }
  {
    s32 entrance = *(u16 *)&gMainRoutineWork.entrance;

    if ((u16)(entrance - 11) <= 28) {
      Func_02001804();
      entrance <<= 16;
      entrance >>= 16;
      entrance += 789;
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
