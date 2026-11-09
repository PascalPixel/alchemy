#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "flag_call.h"
#include "scene_tables.h"
#include "event_inline.h"

void Func_02001bcc(s32 owner);
void Func_02000060(s32 animate);

s32 Func_0200020c(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x204;
  if (!Flag_IsSet(0x109) && gMainRoutineWork.entrance == 6)
    Func_02001bcc(gMainRoutineWork.currentOwner);
  if (Flag_IsSet(0xa01)) {
    Actor_SetHorizontalPositionWithTerrain(10, PIXELS(280), PIXELS(536));
    Func_02000060(0);
  }
  if (Flag_IsSet(0x200))
    Func_02000060(0);
  return 0;
}
