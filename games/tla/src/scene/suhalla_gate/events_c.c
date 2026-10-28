#include "local.h"

void Func_02000650(void) {
  Func_02000988();
  Func_02000a70(0);
  Func_020009d0_(8, 0x1d80000, 0x1e00000);
  FieldEvent_WaitFrames(16);
  ObjectMotion_SetAngleToward(gAdventure.currentOwner, 8, 0);
  Func_02000a18(gAdventure.currentOwner, 0x100, 0);
  Camera_FollowActor(8, 1);
  Func_02000a30();
  ObjectMotion_Launch_(8, 4, 0);
  FieldEvent_WaitFrames(16);
  ObjectMotion_Launch(8, 4, 0);
  ObjectMotion_SetPositionAndCommit_(8, 480, 472);
  FieldEvent_WaitFrames(30);
  ObjectMotion_Launch(8, 2, 0);
  ObjectMotion_SetPositionAndCommit_(8, 496, 464);
  ObjectMotion_Launch(8, 4, 0);
  ObjectMotion_SetPositionAndCommit_(8, 488, 456);
  ObjectMotion_Launch(8, 4, 0);
  ObjectMotion_SetPositionAndCommit_(8, 496, 448);
  Func_020005e8();
}

void Func_0200071c(void) {
  s32 x, z;

  Func_02000988();
  Func_02000a70(0);
  Func_020009d0_(8, 0x1d80000, 0x1e00000);
  FieldEvent_WaitFrames(16);
  ObjectMotion_SetAngleToward(gAdventure.currentOwner, 8, 0);
  x = 0x100;
  z = 0;
  /* FAKEMATCH: the native builds both arguments before loading the leader. */
  __asm__ volatile("" : "+r"(x), "+r"(z));
  Func_02000a18(gAdventure.currentOwner, x, z);
  FieldEvent_WaitFrames(16);
  ObjectMotion_ResetAndSetPositionInMode2_(gAdventure.currentOwner, 488, 440);
  FieldEvent_WaitFrames(16);
  Camera_FollowActor(8, 1);
  Func_02000a30();
  ObjectMotion_Launch_(8, 4, 0);
  FieldEvent_WaitFrames(16);
  ObjectMotion_Launch(8, 4, 0);
  ObjectMotion_SetPositionAndCommit_(8, 488, 480);
  FieldEvent_WaitFrames(30);
  ObjectMotion_Launch(8, 2, 0);
  ObjectMotion_SetPositionAndCommit_(8, 504, 472);
  ObjectMotion_Launch(8, 4, 0);
  ObjectMotion_SetPositionAndCommit_(8, 520, 464);
  ObjectMotion_Launch(8, 4, 0);
  ObjectMotion_SetPositionAndCommit_(8, 528, 456);
  Func_020005e8();
}

void Func_02000800(void) {
  struct ObjectRuntime *leader;

  Func_02000988();
  Func_02000a70(0);
  Func_02000a00((s32)&MsgMyThatWas);
  Func_020009d0_(9, 0x2000000, 0x1b00000);
  Func_020009d0_(gAdventure.currentOwner, 0x2100000, 0x1b00000);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Func_02000a08(9, 0, 10);
  Motion_SetModeAndWaitAnimation(gAdventure.currentOwner, 3);
  FieldEvent_WaitFrames(30);
  Motion_SetVarCbAndRefresh(9, 2);
  FieldEvent_WaitFrames(20);
  Func_02000a08(9, 0, 10);
  Motion_SetModeAndWaitAnimation(gAdventure.currentOwner, 3);
  FieldEvent_WaitFrames(30);
  GameFlag_SetBit(0x896);
  Object_SetModeById(9, 2);
  leader = Object_GetById(gAdventure.currentOwner);
  if (leader)
    ObjectMotion_ResetAndSetPosition(9, ((s16 *)&leader->x)[1],
                                     ((s16 *)&leader->z)[1]);
  ObjectMotion_CommitCurrentPositionAndActivate(9);
  Func_020009d0(9, 0, 0);
  FieldEvent_WaitFrames(10);
  Func_02000990();
}

void Func_020008cc(void) {
  struct ObjectRuntime *leader;

  Func_02000988();
  Func_02000a70(0);
  Func_02000a00((s32)&MsgNotThatWay);
  Func_02000a78(16, -16, 0, 0);
  ObjectMotion_CommitCurrentPositionAndActivate(16);
  Func_02000a08(0x1010, 0, 10);
  Object_SetModeById(16, 2);
  leader = Object_GetById(gAdventure.currentOwner);
  if (leader)
    ObjectMotion_ResetAndSetPosition(16, ((s16 *)&leader->x)[1],
                                     ((s16 *)&leader->z)[1]);
  ObjectMotion_CommitCurrentPositionAndActivate(16);
  Func_020009d0(16, 0, 0);
  FieldEvent_WaitFrames(10);
  ObjectMotion_CommitPositionAndActivate(gAdventure.currentOwner, 16, 0);
  Func_02000990();
}
