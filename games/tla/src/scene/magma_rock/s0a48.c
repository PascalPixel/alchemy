/// @file
/// Rebuilds the room's opened passages on entry and sets the flag of the cell the leader stands on.
#include "iwram_call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"
#include "types.h"
#include "scene_ids.h"
#include "event_inline.h"


extern u8 gData_02003d98[];
extern u8 gData_02003dac[];
extern u8 gData_02003dd2[];
extern u8 gData_02003de4[];
extern u8 gData_02003de8[];
void Func_02003aec();
void Func_02003c1c();
void Func_020006cc();
s32 Func_02003af4();

s32 Func_02000a48(void) {
  struct Actor *leader = *(struct Actor **)((u8 *)Ram_HeapSlots->eventWork + 3260);
  s32 x, z;

  Func_02003aec();
  FieldEvent_ResolvePendingActions(0);
  Func_02003c1c();
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov691Room1) {
    MapGrid_CopyCellAttributes(98, 21, 4, 3, 78, 21);
    MapGrid_CopyCellAttributes(98, 27, 4, 3, 78, 24);
    Func_020006cc(gData_02003d98);
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov691Room2) {
    if (gMainRoutineWork.entrance <= 3) {
      Func_020006cc(gData_02003dac);
    } else if (gMainRoutineWork.entrance <= 8) {
      Func_020006cc(gData_02003dd2);
    } else if (gMainRoutineWork.entrance <= 12) {
      /* FAKEMATCH(wrapper): forwarded inline, so that its stack arguments build in
       * order; as a direct call, 8 of 187 instructions differ in tla-en. */
      MapGrid_CopyCellAttributes(79, 37, 2, 1, 79, 36);
      Func_020006cc(gData_02003de4);
    } else {
      MapGrid_CopyCellAttributes(103, 39, 1, 1, 103, 38);
      MapGrid_CopyCellAttributes(99, 42, 5, 1, 99, 38);
      Func_020006cc(gData_02003de8);
    }
  }
  x = leader->x >> 20;
  z = leader->z >> 20;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov691Room1) {
    if (x == 15 && z == 25)
      Event_SetFlag(0x8d6);
    else if (x == 16 && z == 22)
      Event_SetFlag(0x8d5);
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov691Room2) {
    if (gMainRoutineWork.entrance <= 3) {
      if (x == 27 && z == 33)
        Event_SetFlag(0x8d7);
    } else if (gMainRoutineWork.entrance <= 8) {
      if (x == 55 && z == 35)
        Event_SetFlag(0x8d8);
    } else if (gMainRoutineWork.entrance <= 12) {
      if (x == 16 && z == 36)
        Event_SetFlag(0x8d9);
    } else if (x == 40 && z == 37) {
      Event_SetFlag(0x8da);
    } else if (x == 35 && z == 38) {
      Event_SetFlag(0x8db);
    }
  }
  return Func_02003af4();
}
