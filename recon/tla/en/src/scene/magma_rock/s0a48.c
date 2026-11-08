/// @file
/// Rebuilds the room's opened passages on entry and sets the flag of the cell the leader stands on.
#include "iwram_call.h"
#include "call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"
#include "types.h"
#include "scene_ids.h"


extern u8 gData_02003d98[];
extern u8 gData_02003dac[];
extern u8 gData_02003dd2[];
extern u8 gData_02003de4[];
extern u8 gData_02003de8[];
void Func_02003aec();
s32 Func_02003bcc();
void Func_02003c1c();
void Func_02003aa4();
void Func_020006cc();
void GameFlag_SetBit();
s32 Func_02003af4();

s32 Func_02000a48(void) {
  struct Actor *leader = *(struct Actor **)((u8 *)Ram_HeapSlots->eventWork + SPLIT_CONSTANT(0xc00, 188));
  s32 x, z;

  Func_02003aec();
  Func_02003bcc(0);
  Func_02003c1c();
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov691Room1) {
    Func_02003aa4(98, 21, 4, 3, 78, 21);
    Func_02003aa4(98, 27, 4, 3, 78, 24);
    Func_020006cc(gData_02003d98);
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov691Room2) {
    if (gMainRoutineWork.entrance <= 3) {
      Func_020006cc(gData_02003dac);
    } else if (gMainRoutineWork.entrance <= 8) {
      Func_020006cc(gData_02003dd2);
    } else if (gMainRoutineWork.entrance <= 12) {
      /* FAKEMATCH(wrapper): forwarded inline, so that its stack arguments build in
       * order; as a direct call, 8 of 187 instructions differ in tla-en. */
      Call6((void (*)())Func_02003aa4, 79, 37, 2, 1, 79, 36);
      Func_020006cc(gData_02003de4);
    } else {
      Func_02003aa4(103, 39, 1, 1, 103, 38);
      Func_02003aa4(99, 42, 5, 1, 99, 38);
      Func_020006cc(gData_02003de8);
    }
  }
  x = leader->x >> 20;
  z = leader->z >> 20;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov691Room1) {
    if (x == 15 && z == 25)
      GameFlag_SetBit(SPLIT_CONSTANT(0x800, 214));
    else if (x == 16 && z == 22)
      GameFlag_SetBit(SPLIT_CONSTANT(0x800, 213));
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov691Room2) {
    if (gMainRoutineWork.entrance <= 3) {
      if (x == 27 && z == 33)
        GameFlag_SetBit(SPLIT_CONSTANT(0x7d8, 255));
    } else if (gMainRoutineWork.entrance <= 8) {
      if (x == 55 && z == 35)
        GameFlag_SetBit(SPLIT_CONSTANT(0x800, 216));
    } else if (gMainRoutineWork.entrance <= 12) {
      if (x == 16 && z == 36)
        GameFlag_SetBit(SPLIT_CONSTANT(0x800, 217));
    } else if (x == 40 && z == 37) {
      GameFlag_SetBit(SPLIT_CONSTANT(0x800, 218));
    } else if (x == 35 && z == 38) {
      GameFlag_SetBit(SPLIT_CONSTANT(0x800, 219));
    }
  }
  return Func_02003af4();
}
