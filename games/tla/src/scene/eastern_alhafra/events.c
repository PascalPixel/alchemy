#include "types.h"
#include "actor.h"
#include "field_sprite.h"
#include "sequence_parent_work.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "call.h"
#include "text/msg_ids.h"

void FieldEvent_Wait();
void Func_02004a94();
void Func_02004a9c();
void Func_02004b04();
void Func_02004b3c();
void Func_02004b4c();
void Func_02004b6c();
void Func_02004b74();
void Func_02004b84();
void Func_02004b94();
void Func_02004b9c();
void Func_02004be4();
void Func_02004bec();
void Camera_MoveWithinMapBounds();
void Motion_SetModeAndWaitAnimation();
void Motion_SetVarCbAndRefresh();
void Actor_ArmCallback();
void Actor_WaitForMoveById();
void Actor_CommitPositionAndActivate();
void Actor_Jump();
void Actor_ResetAndSetPosition();
void Actor_SetPositionAndReset();
void Actor_SetSpeedParameters();
void Actor_StartRepeatedMotion();
void Camera_FollowActor();
struct Actor *Actor_GetById();
void Actor_SetModeById();
void GameFlag_SetBit();
void GameFlag_Test();
void Func_02004b5c();
void Event_AskYesNoWithActorAnimation();
void FieldMessage_ShowAtActor();

void Func_0200112c(void) {
  struct Actor *object0;
  struct Actor *object1;
  struct Actor *object2;

  ((void (*)())Func_02004a94)();
  Call1((void (*)())Func_02004be4, 0);
  Call1((void (*)())Func_02004b3c, (s32)MsgItSIt);
  Call2((void (*)())Func_02004b84, 0x40000, 0x8000);
  Call3((void (*)())Func_02004b4c, 22, 0, 10);
  Call3((void (*)())Actor_SetPositionAndReset, 4, 232, 0x188);
  Call4((void (*)())Func_02004bec, 26, 0, -16, 0x2000);
  Call4((void (*)())Func_02004bec, 5, 0, 0, 0x2000);
  Call4((void (*)())Func_02004bec, 6, 0, 16, 0x2000);
  Call3((void (*)())Actor_CommitPositionAndActivate, 4, 16, 0);
  Call3((void (*)())Actor_ArmCallback, 4, 0x2000, 0);
  Call1((void (*)())FieldEvent_Wait, 30);
  Value2((s32 (*)())Func_02004b9c, 22, 1);
  ((void (*)())Func_02004b94)();
  Call1((void (*)())FieldEvent_Wait, 10);
  Value2((s32 (*)())Motion_SetVarCbAndRefresh, 22, 2);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Func_02004b4c, 22, 0, 10);
  Value2((s32 (*)())Func_02004b9c, 23, 1);
  ((void (*)())Func_02004b94)();
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Actor_Jump, 23, 6, 15);
  Call3((void (*)())Actor_Jump, 23, 6, 23);
  Call3((void (*)())Func_02004b4c, 23, 0, 10);
  Call3((void (*)())Actor_ArmCallback, 22, 0x2000, 0);
  Call3((void (*)())Actor_ArmCallback, 24, 0x2000, 0);
  Call3((void (*)())Actor_ArmCallback, 25, 0x5000, 0);
  Call1((void (*)())FieldEvent_Wait, 20);
  Value2((s32 (*)())Func_02004b9c, 11, 1);
  ((void (*)())Func_02004b94)();
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Func_02004b4c, 23, 0, 10);
  Value2((s32 (*)())Func_02004b9c, 24, 1);
  ((void (*)())Func_02004b94)();
  Call1((void (*)())FieldEvent_Wait, 10);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 24, 4);
  Call3((void (*)())Func_02004b4c, 24, 0, 10);
  Call3((void (*)())Actor_ArmCallback, 24, 0xe000, 0);
  Call1((void (*)())FieldEvent_Wait, 20);
  Value2((s32 (*)())Func_02004b9c, 25, 1);
  ((void (*)())Func_02004b94)();
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Actor_ArmCallback, 25, 0xe000, 0);
  Call1((void (*)())FieldEvent_Wait, 30);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 25, 3);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Func_02004b4c, 25, 0, 10);
  Call3((void (*)())Actor_ArmCallback, 22, 0, 0);
  Call3((void (*)())Actor_ArmCallback, 23, 0xd000, 0);
  Call1((void (*)())FieldEvent_Wait, 20);
  Value2((s32 (*)())Func_02004b9c, 14, 1);
  ((void (*)())Func_02004b94)();
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Func_02004b4c, 25, 0, 10);
  Value2((s32 (*)())Func_02004b9c, 22, 1);
  ((void (*)())Func_02004b94)();
  Call1((void (*)())FieldEvent_Wait, 10);
  Value3((s32 (*)())Func_02004b6c, 22, 0x107, 40);
  Call3((void (*)())Func_02004b4c, 22, 0, 10);
  Call4((void (*)())Camera_MoveWithinMapBounds, 0x1e80000, 0, 0x1a00000, 1);
  ((void (*)())Func_02004b94)();
  Call1((void (*)())FieldEvent_Wait, 20);
  Value3((s32 (*)())Func_02004b6c, 23, 0x102, 50);
  Call3((void (*)())Actor_ArmCallback, 23, 0, 0);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call3((void (*)())Func_02004b4c, 23, 0, 10);
  Call3((void (*)())Actor_ArmCallback, 24, 0, 0);
  Call3((void (*)())Actor_ArmCallback, 25, 0x2000, 0);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call2((void (*)())Actor_StartRepeatedMotion, 25, 3);
  Call2((void (*)())Func_02004b74, 25, 0x102);
  Call1((void (*)())FieldEvent_Wait, 40);
  Call3((void (*)())Func_02004b4c, 25, 0, 10);
  Call4((void (*)())Camera_MoveWithinMapBounds, 0x1880000, 0x100000, 0x1a00000,
        1);
  ((void (*)())Func_02004b94)();
  Call1((void (*)())FieldEvent_Wait, 20);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 24, 4);
  Call3((void (*)())Func_02004b4c, 24, 0, 10);
  Call3((void (*)())Actor_ArmCallback, 23, 0x8000, 0);
  Call1((void (*)())FieldEvent_Wait, 30);
  Value3((s32 (*)())Func_02004b6c, 22, 0x105, 0);
  Value3((s32 (*)())Func_02004b6c, 23, 0x105, 0);
  Value3((s32 (*)())Func_02004b6c, 25, 0x105, 50);
  Call1((void (*)())FieldEvent_Wait, 20);
  Value2((s32 (*)())Motion_SetVarCbAndRefresh, 25, 2);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Actor_ArmCallback, 25, 0x5000, 0);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call3((void (*)())Func_02004b4c, 25, 0, 20);
  Call2((void (*)())Actor_SetModeById, 22, 3);
  Call2((void (*)())Actor_SetModeById, 23, 3);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 24, 3);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call3((void (*)())Actor_ArmCallback, 22, 0, 0);
  Call3((void (*)())Actor_ArmCallback, 23, 0x5000, 0);
  Call3((void (*)())Actor_ArmCallback, 24, 0xc000, 0);
  Call3((void (*)())Actor_ArmCallback, 25, 0xe000, 0);
  Call1((void (*)())FieldEvent_Wait, 20);
  Value2((s32 (*)())Func_02004b9c, gSequenceParentWork.currentOwner, 1);
  ((void (*)())Func_02004b94)();
  Call1((void (*)())FieldEvent_Wait, 20);
  Call3((void (*)())Actor_SetSpeedParameters, 5, 0x13333, 0x9999);
  Call2((void (*)())Actor_SetModeById, 5, 2);
  object0 = Actor_GetById(gSequenceParentWork.currentOwner);
  if (object0 == 0)
    goto L_020094c6;
  Value3((s32 (*)())Actor_ResetAndSetPosition, 5,
         *(s16 *)((u8 *)object0 + 10), *(s16 *)((u8 *)object0 + 18));
L_020094c6:
  Call1((void (*)())Actor_WaitForMoveById, 5);
  Call3((void (*)())Func_02004b04, 5, 0, 0);
  Call3((void (*)())Actor_SetSpeedParameters, 6, 0x13333, 0x9999);
  Call2((void (*)())Actor_SetModeById, 6, 2);
  object1 = Actor_GetById(gSequenceParentWork.currentOwner);
  if (object1 == 0)
    goto L_02009504;
  Value3((s32 (*)())Actor_ResetAndSetPosition, 6,
         *(s16 *)((u8 *)object1 + 10), *(s16 *)((u8 *)object1 + 18));
L_02009504:
  Call1((void (*)())Actor_WaitForMoveById, 6);
  Call3((void (*)())Func_02004b04, 6, 0, 0);
  Call3((void (*)())Actor_SetSpeedParameters, 26, 0x13333, 0x9999);
  Call2((void (*)())Actor_SetModeById, 26, 2);
  object2 = Actor_GetById(gSequenceParentWork.currentOwner);
  if (object2 == 0)
    goto L_02009550;
  Value3((s32 (*)())Actor_ResetAndSetPosition, 26,
         *(s16 *)((u8 *)object2 + 10), *(s16 *)((u8 *)object2 + 18));
L_02009550:
  Call1((void (*)())Actor_WaitForMoveById, 26);
  Call3((void (*)())Func_02004b04, 26, 0, 0);
  Call2((void (*)())Camera_FollowActor, gSequenceParentWork.currentOwner, 1);
  ((void (*)())Func_02004b94)();
  ((void (*)())Func_02004a9c)();
  return;
}

void Func_02001574(void) {
  s32 result0;
  s32 result1;
  s32 result2;
  s32 result3;
  struct Actor *object4;
  struct Actor *object5;
  struct Actor *object6;
  struct Actor *object7;

  result0 = Value1((s32 (*)())GameFlag_Test, 0x8ab);
  if (result0 != 0)
    goto L_02009586;
  goto L_020097dc;
L_02009586:
  result1 = Value1((s32 (*)())GameFlag_Test, 0x8a7);
  if (result1 == 0)
    goto L_02009596;
  goto L_020097dc;
L_02009596:
  ((void (*)())Func_02004a94)();
  Call1((void (*)())Func_02004be4, 0);
  Call1((void (*)())Func_02004b3c, (s32)MsgYouDoPlan);
  Call1((void (*)())GameFlag_SetBit, 0x8ac);
  Call3((void (*)())Actor_SetPositionAndReset, 4, 0x108, 0x1c0);
  Call4((void (*)())Func_02004bec, 26, 0, -16, 0x4000);
  Call4((void (*)())Func_02004bec, 5, 16, 0, 0x8000);
  Call4((void (*)())Func_02004bec, 6, 16, -16, 0x6000);
  result2 = Value1((s32 (*)())GameFlag_Test, 7);
  if (result2 == 0)
    goto L_02009604;
  Call4((void (*)())Func_02004bec, 7, 8, 16, 0xc000);
L_02009604:
  Call1((void (*)())Actor_WaitForMoveById, 6);
  Call3((void (*)())Actor_ArmCallback, 4, 0, 0);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 26, 3);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Func_02004b4c, 26, 0, 10);
  Call3((void (*)())Actor_ArmCallback, 4, 0xc000, 0);
  Call1((void (*)())FieldEvent_Wait, 20);
  Value3((s32 (*)())Func_02004b6c, 6, 0x102, 30);
  Call3((void (*)())Func_02004b4c, 6, 0, 10);
  Call3((void (*)())Actor_ArmCallback, 5, 0xc000, 0);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Func_02004b4c, 5, 0, 10);
  Call3((void (*)())Actor_ArmCallback, 5, 0x8000, 0);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Func_02004b4c, 5, 0, 10);
  Call3((void (*)())Actor_ArmCallback, 4, 0, 0);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 4, 3);
  Call1((void (*)())FieldEvent_Wait, 30);
  Value3((s32 (*)())Func_02004b6c, 26, 0x106, 30);
  Call3((void (*)())Func_02004b4c, 26, 0, 10);
  result3 = Value1((s32 (*)())GameFlag_Test, 7);
  if (result3 == 0)
    goto L_02009716;
  Call3((void (*)())Actor_SetSpeedParameters, 7, 0x13333, 0x9999);
  Call2((void (*)())Actor_SetModeById, 7, 2);
  object4 = Actor_GetById(gSequenceParentWork.currentOwner);
  if (object4 == 0)
    goto L_02009706;
  Value3((s32 (*)())Actor_ResetAndSetPosition, 7,
         *(s16 *)((u8 *)object4 + 10), *(s16 *)((u8 *)object4 + 18));
L_02009706:
  Call1((void (*)())Actor_WaitForMoveById, 7);
  Call3((void (*)())Func_02004b04, 7, 0, 0);
L_02009716:
  Call3((void (*)())Actor_SetSpeedParameters, 5, 0x13333, 0x9999);
  Call2((void (*)())Actor_SetModeById, 5, 2);
  object5 = Actor_GetById(gSequenceParentWork.currentOwner);
  if (object5 == 0)
    goto L_0200974c;
  Value3((s32 (*)())Actor_ResetAndSetPosition, 5,
         *(s16 *)((u8 *)object5 + 10), *(s16 *)((u8 *)object5 + 18));
L_0200974c:
  Call1((void (*)())Actor_WaitForMoveById, 5);
  Call3((void (*)())Func_02004b04, 5, 0, 0);
  Call3((void (*)())Actor_SetSpeedParameters, 6, 0x13333, 0x9999);
  Call2((void (*)())Actor_SetModeById, 6, 2);
  object6 = Actor_GetById(gSequenceParentWork.currentOwner);
  if (object6 == 0)
    goto L_0200978a;
  Value3((s32 (*)())Actor_ResetAndSetPosition, 6,
         *(s16 *)((u8 *)object6 + 10), *(s16 *)((u8 *)object6 + 18));
L_0200978a:
  Call1((void (*)())Actor_WaitForMoveById, 6);
  Call3((void (*)())Func_02004b04, 6, 0, 0);
  Call3((void (*)())Actor_SetSpeedParameters, 26, 0x13333, 0x9999);
  Call2((void (*)())Actor_SetModeById, 26, 2);
  object7 = Actor_GetById(gSequenceParentWork.currentOwner);
  if (object7 == 0)
    goto L_020097c8;
  Value3((s32 (*)())Actor_ResetAndSetPosition, 26,
         *(s16 *)((u8 *)object7 + 10), *(s16 *)((u8 *)object7 + 18));
L_020097c8:
  Call1((void (*)())Actor_WaitForMoveById, 26);
  Call3((void (*)())Func_02004b04, 26, 0, 0);
  ((void (*)())Func_02004a9c)();
L_020097dc:
  return;
}

void Func_020017ec(void) {
  s32 result0;
  s32 result1;
  s32 result2;
  s32 result3;
  struct Actor *object4;
  struct Actor *object5;
  struct Actor *object6;
  struct Actor *object7;

  Value0((s32 (*)())Func_02004a94);
  Value1((s32 (*)())Func_02004be4, 0);
  Value1((s32 (*)())Func_02004b3c, (s32)MsgAreYouFinished);
  Call3((void (*)())Actor_SetPositionAndReset, 4, 232, 0x188);
  Call4((void (*)())Func_02004bec, 26, 0, -16, 0x4000);
  Call4((void (*)())Func_02004bec, 5, 16, 0, 0x8000);
  Value4((s32 (*)())Func_02004bec, 6, 16, -16, 0x6000);
  result0 = Value1((s32 (*)())GameFlag_Test, 7);
  if (result0 == 0)
    goto L_02009850;
  Value4((s32 (*)())Func_02004bec, 7, 8, 16, 0xc000);
L_02009850:
  Value1((s32 (*)())Actor_WaitForMoveById, 6);
  Value3((s32 (*)())Actor_ArmCallback, 4, 0xc000, 0);
  Value1((s32 (*)())FieldEvent_Wait, 20);
  Call2((void (*)())Motion_SetVarCbAndRefresh, 26, 2);
  Value1((s32 (*)())FieldEvent_Wait, 10);
  Value2((s32 (*)())FieldMessage_ShowAtActor, 26, 0);
  result1 = Value2((s32 (*)())Event_AskYesNoWithActorAnimation, 4, 0);
  if (result1 != 0)
    goto L_020098ea;
  Call1((void (*)())FieldEvent_Wait, 20);
  Value3((s32 (*)())Func_02004b6c, 6, 0x100, 30);
  Call3((void (*)())Func_02004b4c, 6, 0, 10);
  Call3((void (*)())Actor_ArmCallback, 4, 0xe000, 0);
  Call3((void (*)())Actor_ArmCallback, 5, 0xc000, 0);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 5, 4);
  Call3((void (*)())Func_02004b4c, 5, 0, 10);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 2;
  goto L_02009938;
L_020098ea:
  ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 2;
  Call1((void (*)())FieldEvent_Wait, 40);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 6, 3);
  Call3((void (*)())Actor_ArmCallback, 4, 0xe000, 0);
  Value1((s32 (*)())FieldEvent_Wait, 20);
  ((void (*)())Func_02004b4c)(6, 0, 10);
  ((void (*)())Motion_SetModeAndWaitAnimation)(5, 4);
  Value3((s32 (*)())Func_02004b4c, 5, 0, 10);
L_02009938:
  result2 = ((s32 (*)())GameFlag_Test)(7);
  if (result2 == 0)
    goto L_0200996c;
  Call3((void (*)())Actor_ArmCallback, 4, 0xc000, 0);
  Call3((void (*)())Actor_ArmCallback, 5, 0xa000, 0);
  Value2((s32 (*)())Func_02004b5c, 6, 0x8000);
  Value1((s32 (*)())FieldEvent_Wait, 10);
  goto L_02009a2e;
L_0200996c:
  ((s32 (*)())Func_02004b6c)(26, 0x102, 30);
  ((void (*)())Func_02004b4c)(26, 0, 10);
  Call3((void (*)())Actor_ArmCallback, 4, 0xc000, 0);
  Call3((void (*)())Actor_ArmCallback, 5, 0xa000, 0);
  Value3((s32 (*)())Actor_ArmCallback, 6, 0x8000, 0);
  Value1((s32 (*)())FieldEvent_Wait, 30);
  ((s32 (*)())Func_02004b6c)(26, 0x101, 30);
  ((void (*)())Motion_SetModeAndWaitAnimation)(26, 4);
  ((void (*)())Func_02004b4c)(26, 0, 10);
  ((s32 (*)())Func_02004b6c)(26, 0x109, 30);
  ((void (*)())Func_02004b4c)(26, 0, 10);
  Call3((void (*)())Actor_ArmCallback, 5, 0xc000, 0);
  Value3((s32 (*)())Actor_ArmCallback, 6, 0x4000, 0);
  Value1((s32 (*)())FieldEvent_Wait, 40);
  Call3((void (*)())Actor_ArmCallback, 5, 0xa000, 0);
  Value3((s32 (*)())Actor_ArmCallback, 6, 0x8000, 0);
  Value1((s32 (*)())FieldEvent_Wait, 10);
  ((void (*)())Motion_SetModeAndWaitAnimation)(26, 4);
  Value3((s32 (*)())Func_02004b4c, 26, 0, 10);
L_02009a2e:
  Value1((s32 (*)())Func_02004b3c, (s32)MsgSoIThink);
  Value2((s32 (*)())Motion_SetModeAndWaitAnimation, 26, 3);
  Value1((s32 (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Func_02004b4c, 26, 0, 10);
  Call2((void (*)())Actor_SetModeById, 7, 3);
  Call2((void (*)())Actor_SetModeById, 5, 3);
  Call2((void (*)())Actor_SetModeById, 6, 3);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 4, 3);
  Call1((void (*)())FieldEvent_Wait, 10);
  result3 = Value1((s32 (*)())GameFlag_Test, 7);
  if (result3 == 0)
    goto L_02009ac2;
  Call3((void (*)())Actor_SetSpeedParameters, 7, 0x13333, 0x9999);
  Call2((void (*)())Actor_SetModeById, 7, 2);
  object4 = Actor_GetById(gSequenceParentWork.currentOwner);
  if (object4 == 0)
    goto L_02009ab2;
  Value3((s32 (*)())Actor_ResetAndSetPosition, 7,
         *(s16 *)((u8 *)object4 + 10), *(s16 *)((u8 *)object4 + 18));
L_02009ab2:
  Call1((void (*)())Actor_WaitForMoveById, 7);
  Call3((void (*)())Func_02004b04, 7, 0, 0);
L_02009ac2:
  Call3((void (*)())Actor_SetSpeedParameters, 5, 0x13333, 0x9999);
  Call2((void (*)())Actor_SetModeById, 5, 2);
  object5 = Actor_GetById(gSequenceParentWork.currentOwner);
  if (object5 == 0)
    goto L_02009af8;
  Value3((s32 (*)())Actor_ResetAndSetPosition, 5,
         *(s16 *)((u8 *)object5 + 10), *(s16 *)((u8 *)object5 + 18));
L_02009af8:
  Call1((void (*)())Actor_WaitForMoveById, 5);
  Call3((void (*)())Func_02004b04, 5, 0, 0);
  Call3((void (*)())Actor_SetSpeedParameters, 6, 0x13333, 0x9999);
  Call2((void (*)())Actor_SetModeById, 6, 2);
  object6 = Actor_GetById(gSequenceParentWork.currentOwner);
  if (object6 == 0)
    goto L_02009b36;
  Value3((s32 (*)())Actor_ResetAndSetPosition, 6,
         *(s16 *)((u8 *)object6 + 10), *(s16 *)((u8 *)object6 + 18));
L_02009b36:
  Call1((void (*)())Actor_WaitForMoveById, 6);
  Call3((void (*)())Func_02004b04, 6, 0, 0);
  Call3((void (*)())Actor_SetSpeedParameters, 26, 0x13333, 0x9999);
  Call2((void (*)())Actor_SetModeById, 26, 2);
  object7 = Actor_GetById(gSequenceParentWork.currentOwner);
  if (object7 == 0)
    goto L_02009b74;
  Value3((s32 (*)())Actor_ResetAndSetPosition, 26,
         *(s16 *)((u8 *)object7 + 10), *(s16 *)((u8 *)object7 + 18));
L_02009b74:
  Call1((void (*)())Actor_WaitForMoveById, 26);
  Call3((void (*)())Func_02004b04, 26, 0, 0);
  ((void (*)())Func_02004a9c)();
  return;
}

struct SpawnObject {
  u8 unk00[0x50];
  struct FieldSprite *sprite;
  u8 unk54;
  u8 motionFlags;
  u8 unk56[8];
  u16 unk5e;
};

extern const u8 gData_020056d4[];

struct SpawnObject *Func_02004a1c(s32 type, s32 x, s32 y, s32 z);
void Func_02004a0c(struct SpawnObject *object, s32 mode);
void Func_02004a14(struct SpawnObject *object, const void *script);
void Func_02004a74(struct SpawnObject *object, s32 priority);

/// Spawns object type 0x1e8 at x, y, z with its sprite turned to value and
/// its timer set.
void Func_02001b9c(s32 x, s32 y, s32 z, s16 value, s32 timer) {
  struct SpawnObject *object;
  struct FieldSprite *sprite;

  object = Func_02004a1c(0x1e8, x, y, z);
  if (object != 0) {
    sprite = object->sprite;
    Func_02004a0c(object, 1);
    Func_02004a14(object, gData_020056d4);
    Func_02004a74(object, 1);
    object->motionFlags = 0;
    {
      /* FAKEMATCH: the 64-bit copy loads the timer argument before the pooled
       * zero for nextFrame; stored directly, 2 instructions differ in
       * tla-en. */
      s64 wide = timer;

      object->unk5e = wide;
    }
    sprite->nextFrame = 0;
    sprite->rotation = value;
  }
}
