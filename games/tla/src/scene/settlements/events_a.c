#include "local.h"

void Func_02000404(void) {
  s32 mode;

  Func_02002068();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetPosition(11, 0, 0);
  Event_SetFlag(0xf5d);
  Func_02002130(193, 3);
  mode = 0;
  Party_GiveItem(193, mode);
  Func_02002070();
}

void Func_0200043c(s32 actor) {
  s32 msg = (s32)&MsgSettlements01;

  FieldEvent_SetMessageId(msg);
  Event_OpenMessage(actor, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) == 0) {
    FieldEvent_Wait(10);
    FieldEvent_SetMessageId(msg + 1);
  } else {
    FieldEvent_Wait(20);
    FieldEvent_SetMessageId(msg + 2);
  }
  Event_ShowMessage(actor, 0);
}

void Func_02000488(void) { Window(5, 70, 9, 7, 5, 43, 1); }

void Func_020004a8(void) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;

  if (x == 5 && z == 17) {
    FieldActor_Jump(gMainRoutineWork.currentOwner, 2, 0);
    SetPosCommit(gMainRoutineWork.currentOwner, 88, 296);
  }
  Func_020021a0(5, 70, 9, 7, 5, 43, 1);
  MapGrid_CopyCellsTo(4, 25, 5, 17, 1, 1);
  MapGrid_CopyCellAttributes(4, 25, 1, 1, 5, 17);
  Event_SetFlag(0xa60);
}

void Func_0200052c(void) {
  s32 next = *(s16 *)((u8 *)Ram_HeapSlots->eventWork + 0x154);

  Actor_FaceDirection(4, FACING_NORTH, 0);
  FieldEvent_Wait(10);
  Actor_Get(4)->actionFlags &= 0xfe;
  CopyCells(96, 14, 72, 14, 2, 1);
  Actor_SetDestinationOffset(4, 0, 4);
  FieldActor_WaitForMoveById(4);
  Actor_SetSingleChildFlags(Actor_Get(4), 0);
  Actor_SetAnimation(4, 13);
  Actor_SetDestinationOffset(4, 0, 16);
  FieldActor_WaitForMoveById(4);
  Actor_SetAnimation(4, 10);
  FieldEvent_Wait(10);
  Event_PlaySound(123);
  FieldEvent_SetExitRequest(next);
  Func_02002070();
}

void Func_020005cc(void) {
  s32 id;

  Func_02002068();
  FieldEvent_ResolvePendingActions(0);
  CommitPos(4, 0, -14);
  id = 4;
  Actor_WalkByAndWait(id, 16, 0);
  Func_0200052c();
}

void Func_020005f4(void) {
  s32 id, x, z;

  Func_02002068();
  FieldEvent_ResolvePendingActions(0);
  CommitPos(4, -2, -6);
  x = 14;
  z = 8;
  id = 4;
  /* FAKEMATCH(barrier): the empty asm on the actor id, which then comes
   * between the distances and their negation; as Actor_WalkByAndWait(4, -x,
   * -z), 2 of 18 instructions differ in tla-en. */
  __asm__ volatile("" : "+r"(id));
  Actor_WalkByAndWait(id, -x, -z);
  Func_0200052c();
}

void Func_02000624(void) {
  s32 id, a;

  Func_02002068();
  FieldEvent_ResolvePendingActions(0);
  a = 192;
  id = 4;
  /* FAKEMATCH(barrier): the empty asm on the actor id, which then comes
   * between the angle byte and its shift; as Actor_FaceDirection(4, a << 8,
   * 0), 2 of 11 instructions differ in tla-en. */
  __asm__ volatile("" : "+r"(id));
  Actor_FaceDirection(id, a << 8, 0);
  Func_0200052c();
}

void Func_02000644(void) {
  CopyCells2(0, 3, 1, 1, 3, 15);
  Event_SetFlag(0x230);
}
