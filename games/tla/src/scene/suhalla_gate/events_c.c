#include "local.h"

void Func_02000650(void) {
  Func_02000988();
  Func_02000a70(0);
  Func_020009d0_(8, PIXELS(472), PIXELS(480));
  FieldEvent_Wait(16);
  Actor_SetAngleToward(gMainRoutineWork.currentOwner, 8, 0);
  Func_02000a18(gMainRoutineWork.currentOwner, 0x100, 0);
  Camera_FollowActor(8, 1);
  Func_02000a30();
  Actor_Jump_(8, 4, 0);
  FieldEvent_Wait(16);
  Actor_Jump(8, 4, 0);
  Actor_SetPositionAndCommit_(8, 480, 472);
  FieldEvent_Wait(30);
  Actor_Jump(8, 2, 0);
  Actor_SetPositionAndCommit_(8, 496, 464);
  Actor_Jump(8, 4, 0);
  Actor_SetPositionAndCommit_(8, 488, 456);
  Actor_Jump(8, 4, 0);
  Actor_SetPositionAndCommit_(8, 496, 448);
  Func_020005e8();
}

void Func_0200071c(void) {
  s32 x, z;

  Func_02000988();
  Func_02000a70(0);
  Func_020009d0_(8, PIXELS(472), PIXELS(480));
  FieldEvent_Wait(16);
  Actor_SetAngleToward(gMainRoutineWork.currentOwner, 8, 0);
  x = 0x100;
  z = 0;
  /* FAKEMATCH: the native builds both arguments before loading the leader. */
  __asm__ volatile("" : "+r"(x), "+r"(z));
  Func_02000a18(gMainRoutineWork.currentOwner, x, z);
  FieldEvent_Wait(16);
  Actor_ResetAndSetPositionInMode2_(gMainRoutineWork.currentOwner, 488, 440);
  FieldEvent_Wait(16);
  Camera_FollowActor(8, 1);
  Func_02000a30();
  Actor_Jump_(8, 4, 0);
  FieldEvent_Wait(16);
  Actor_Jump(8, 4, 0);
  Actor_SetPositionAndCommit_(8, 488, 480);
  FieldEvent_Wait(30);
  Actor_Jump(8, 2, 0);
  Actor_SetPositionAndCommit_(8, 504, 472);
  Actor_Jump(8, 4, 0);
  Actor_SetPositionAndCommit_(8, 520, 464);
  Actor_Jump(8, 4, 0);
  Actor_SetPositionAndCommit_(8, 528, 456);
  Func_020005e8();
}

void Func_02000800(void) {
  struct Actor *leader;

  Func_02000988();
  Func_02000a70(0);
  Func_02000a00((s32)&MsgMyThatWas);
  Func_020009d0_(9, PIXELS(512), PIXELS(432));
  Func_020009d0_(gMainRoutineWork.currentOwner, PIXELS(528), PIXELS(432));
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Func_02000a08(9, 0, 10);
  Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 3);
  FieldEvent_Wait(30);
  Motion_SetVarCbAndRefresh(9, 2);
  FieldEvent_Wait(20);
  Func_02000a08(9, 0, 10);
  Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 3);
  FieldEvent_Wait(30);
  GameFlag_SetBit(0x896);
  Actor_SetModeById(9, 2);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  if (leader)
    Actor_ResetAndSetPosition(9, ((s16 *)&leader->x)[1],
                                     ((s16 *)&leader->z)[1]);
  Actor_WaitForMoveById(9);
  Func_020009d0(9, 0, 0);
  FieldEvent_Wait(10);
  Func_02000990();
}

void Func_020008cc(void) {
  struct Actor *leader;

  Func_02000988();
  Func_02000a70(0);
  Func_02000a00((s32)&MsgNotThatWay);
  Func_02000a78(16, -16, 0, 0);
  Actor_WaitForMoveById(16);
  Func_02000a08(0x1010, 0, 10);
  Actor_SetModeById(16, 2);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  if (leader)
    Actor_ResetAndSetPosition(16, ((s16 *)&leader->x)[1],
                                     ((s16 *)&leader->z)[1]);
  Actor_WaitForMoveById(16);
  Func_020009d0(16, 0, 0);
  FieldEvent_Wait(10);
  Actor_CommitPositionAndActivate(gMainRoutineWork.currentOwner, 16, 0);
  Func_02000990();
}
