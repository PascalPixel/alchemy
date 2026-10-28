#include "izumo.h"
#include "flag_call.h"
#include "scene_tables.h"

void Func_02000038(void) { Func_02000de8(gData_02000e10); }

void Func_02000048(void) {
  struct FieldActor *actor = Actor_GetById(12);
  s32 x = actor->x >> 20;
  s32 z = actor->z >> 20;

  /* FAKEMATCH: the flag commands go through flag_call.h's inlines, which build
   * each flag id again, and the block copies through call.h's Call6; as direct
   * calls, 19 instructions differ in tla-en. */
  Func_02000df0();
  Func_02000e08(132);
  if (x == 20 && z == 9) {
    if (Flag_IsSet(0x301)) {
      Call6((void (*)())Func_02000ce0, 36, 12, 22, 8, 3, 3);
      Call6((void (*)())Func_02000ce8, 36, 12, 3, 3, 22, 8);
    }
    if (!Flag_IsSet(0x924)) {
      Func_02000d00();
      Func_02000dd0(0);
      Camera_FollowActor(22, 1);
      Func_02000da0();
      Func_02000d88(22, 257, 0);
      FieldEvent_Wait(60);
      Actor_ArmCallback(22, FACING_WEST, 0);
      FieldEvent_Wait(60);
      Func_02000d88(22, 256, 0);
      FieldEvent_Wait(60);
      Func_02000d60(MsgTheMisalignedStone);
      Func_02000d70(22, 0);
      FieldEvent_Wait(30);
      Func_02000d88(22, 262, 0);
      FieldEvent_Wait(30);
      Func_02000d70(22, 0);
      Actor_ArmCallback(22, FACING_SOUTH, 0);
      Func_02000d08();
      Flag_Set(0x924);
    }
    Flag_Set(0x250);
  } else if (x == 19 && z == 9) {
    Flag_Clear(0x250);
    if (Flag_IsSet(0x301)) {
      Call6((void (*)())Func_02000ce0, 36, 8, 22, 8, 3, 3);
      Call6((void (*)())Func_02000ce8, 36, 8, 3, 3, 22, 8);
    }
  }
}
