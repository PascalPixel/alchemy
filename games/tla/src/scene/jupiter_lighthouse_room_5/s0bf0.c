#include "types.h"
#include "main_routine_work.h"
#include "actor.h"
#include "flag_call.h"
#include "scene_tables.h"

struct Actor *Actor_GetById(s32 id);
void Scene_SetArrivalFlags(s32 unused);
void Func_02000d80(s32 mode);
void Func_02000e04(s32 id);
void Func_02002a28(s32 x, s32 z, s32 a, s32 b);
void Func_020028f8(s32 id, s32 x, s32 z);

/// Scene setup: the arrival flags and palette, the leader's priority bit 0x40,
/// and the actors flags 0xa00 and 0xa09 bring in.
s32 Func_02000bf0(void) {
  Scene_SetArrivalFlags(0);
  Func_02000d80(0);
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 0x40;
  if (Flag_IsSet(0xa00))
    Func_02002a28(0xd00000, 0x2a00000, 2, 236);
  if (Flag_IsSet(0xa00)) {
    Func_02000e04(10);
    Func_02000e04(12);
    Func_02000e04(13);
    Func_02000e04(14);
    Func_02000e04(15);
    Func_02000e04(16);
  }
  if (Flag_IsSet(0xa09)) {
    Func_020028f8(11, PIXELS(216), PIXELS(552));
    Func_02000e04(11);
  }
  return 0;
}
