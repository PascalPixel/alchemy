#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "flag_call.h"

void Func_02001bcc(s32 owner);
void Func_02001b4c(s32 id, s32 x, s32 z);
void Func_02000060(s32 animate);

s32 Func_0200020c(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x204;
  if (!Flag_IsSet(0x109) && gMainRoutineWork.entrance == 6)
    Func_02001bcc(gMainRoutineWork.currentOwner);
  if (Flag_IsSet(0xa01)) {
    Func_02001b4c(10, 0x1180000, 0x2180000);
    Func_02000060(0);
  }
  if (Flag_IsSet(0x200))
    Func_02000060(0);
  return 0;
}
