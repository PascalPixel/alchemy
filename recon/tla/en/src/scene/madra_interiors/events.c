#include "types.h"
#include "actor.h"
#include "field_sprite.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "call.h"
#include "text/msg_ids.h"
#include "scene_tables.h"

void FieldEvent_Wait();
void Func_02001d50();
void Func_02001d58();
void Func_02001db8();
void Func_02001e18();
void Func_02001e28();
void Func_02001e40();
void Func_02001e48();
void Func_02001e50();
void Func_02001eb8();
void Func_02001ec8();
void Func_02001ef8();
void GameFlag_SetBit();
void GameFlag_Test();
void Event_AskYesNoWithActorAnimation();
void Motion_SetModeAndWaitAnimation();
void Motion_SetVarCbAndRefresh();
void Actor_ArmCallback();
void Actor_WaitForMoveById();
void Actor_Jump();
void Actor_OffsetPositionAndReset();
void Actor_ResetAndSetPosition();
void Actor_SetAngleToward();
void Actor_SetPositionAndReset();
void Actor_SetSpeedParameters();
void Actor_StartRepeatedMotion();
struct Actor *Actor_GetById();
void Actor_TurnToFaceEachOther();
void Actor_SetModeById();
void FieldMessage_ShowAtActor();
void Func_02001910(void);
void Func_02001a20(void);
void FieldEvent_Wait();
void Func_02001d50();
void Func_02001d58();
void Func_02001e18();
void Func_02001e28();
void Func_02001e40();
void Func_02001e48();
void Func_02001eb8();
void Func_02001ec0();
void GameFlag_SetBit();
void Motion_SetModeAndWaitAnimation();
void Motion_SetVarCbAndRefresh();
void Actor_ArmCallback();
void Actor_WaitForMoveById();
void Actor_Stop();
void Actor_SetMovementTypeById();
void Actor_ResetAndSetPositionInMode2();
void Actor_SetAngleToward();
void Actor_SetPositionAndReset();
void Actor_SetSpeedParameters();
struct Actor *Actor_GetById();
void Actor_WatchAndSetScript();
void Actor_TurnToFaceEachOther();
void Actor_RefreshSelectorById();
void Actor_SetModeById();
void PartyInventory_Remove();
extern u32 gData_02001f18[];
extern u32 gData_02001f4c[];
extern u32 gData_02001f90[];
extern u32 gData_02001fc4[];
extern u32 gData_02002054[];
extern u32 gData_020020e4[];

void Func_02001008(void) {
  struct Actor *object;

  GameFlag_SetBit(0x98a);
  GameFlag_SetBit(0x310);
  PartyInventory_Remove(0x1c0);
  Func_02001d50();
  Func_02001eb8(0);
  Call3((void (*)())Actor_SetSpeedParameters, 11, 0x6666, 0x3333);
  Call3((void (*)())Actor_ResetAndSetPositionInMode2, 11, 410, 109);
  object = Actor_GetById(4);
  if (object->z > 0x5f0000)
    Actor_SetPositionAndReset(4, object->x / 0x10000, 95);
  Call3((void (*)())Actor_SetPositionAndReset, 4, 362, 95);
  Call2((void (*)())Func_02001e40, 4, FACING_SOUTH);
  Actor_WaitForMoveById(11);
  Func_02001e18((s32)MsgMadraInteriors110);
  Actor_SetAngleToward(10, 4, 0);
  FieldEvent_Wait(20);
  Value3((s32 (*)())Func_02001e48, 10, 0x102, 30);
  Func_02001e28(10, 0, 10);
  Value3((s32 (*)())Func_02001e48, 11, 0x100, 3);
  Call3((void (*)())Actor_SetPositionAndReset, 11, 378, 109);
  Actor_ArmCallback(11, FACING_NORTHWEST, 0);
  FieldEvent_Wait(20);
  Func_02001e28(11, 0, 10);
  Actor_TurnToFaceEachOther(10, 11, 0);
  FieldEvent_Wait(20);
  Actor_SetAngleToward(10, 4, 0);
  FieldEvent_Wait(20);
  Value3((s32 (*)())Func_02001e48, 10, 0x108, 30);
  Func_02001e28(10, 0, 10);
  Motion_SetModeAndWaitAnimation(4, 3);
  FieldEvent_Wait(20);
  Value3((s32 (*)())Func_02001e48, 11, 0x102, 30);
  Func_02001e28(11, 0, 10);
  Actor_SetAngleToward(10, 11, 0);
  FieldEvent_Wait(30);
  Motion_SetModeAndWaitAnimation(10, 3);
  FieldEvent_Wait(30);
  Value3((s32 (*)())Func_02001e48, 10, 0x101, 30);
  Func_02001e28(10, 0, 10);
  Motion_SetVarCbAndRefresh(11, 2);
  FieldEvent_Wait(20);
  Func_02001e28(11, 0, 10);
  Call2((void (*)())Func_02001e40, 11, FACING_SOUTH);
  FieldEvent_Wait(10);
  Func_02001e40(11, FACING_NORTH);
  FieldEvent_Wait(10);
  Call2((void (*)())Func_02001e40, 11, FACING_WEST);
  FieldEvent_Wait(10);
  Func_02001e48(11, 0x106, 30);
  Actor_Stop(25);
  Call3((void (*)())Actor_SetSpeedParameters, 10, 0x8000, 0x4000);
  Call3((void (*)())Actor_SetSpeedParameters, 11, 0x8000, 0x4000);
  Call3((void (*)())Actor_SetSpeedParameters, 25, 0x8000, 0x4000);
  Actor_SetMovementTypeById(11, (s32)gData_02001f18);
  Actor_SetMovementTypeById(4, (s32)gData_02001f4c);
  FieldEvent_Wait(60);
  Actor_SetMovementTypeById(10, (s32)gData_02001f90);
  Actor_RefreshSelectorById(10);
  FieldEvent_Wait(120);
  Actor_SetMovementTypeById(11, (s32)gData_02001fc4);
  Actor_WatchAndSetScript(4, 11);
  FieldEvent_Wait(60);
  Actor_SetMovementTypeById(10, (s32)gData_02002054);
  Actor_SetMovementTypeById(25, (s32)gData_020020e4);
  Actor_RefreshSelectorById(10);
  Actor_RefreshSelectorById(11);
  Actor_RefreshSelectorById(25);
  Motion_SetVarCbAndRefresh(11, 2);
  FieldEvent_Wait(20);
  Func_02001e28(11, 0, 10);
  Func_02001ec0(25, 2, 10);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
  Actor_SetModeById(4, 1);
  Actor_ArmCallback(10, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  Motion_SetModeAndWaitAnimation(10, 3);
  FieldEvent_Wait(20);
  Actor_ArmCallback(4, FACING_EAST, 0);
  Func_02001e28(10, 0, 10);
  Motion_SetModeAndWaitAnimation(11, 3);
  FieldEvent_Wait(20);
  Func_02001e28(11, 0, 10);
  Func_02001d58();
}

void Func_02001310(void) {
  struct Actor *object0;

  GameFlag_SetBit(0x210);
  Func_02001d50();
  Func_02001eb8(0);
  object0 = Actor_GetById(4);
  if (object0->z > 0x5f0000)
    Actor_SetPositionAndReset(4, object0->x / 0x10000, 95);
  Call3((void (*)())Actor_SetPositionAndReset, 4, 362, 95);
  Func_02001e40(4, FACING_SOUTH);
  Func_02001e18((s32)MsgMadraInteriors110);
  Actor_SetAngleToward(10, 4, 0);
  FieldEvent_Wait(30);
  Value3((s32 (*)())Func_02001e48, 10, 0x102, 30);
  Func_02001e28(10, 0, 5);
  Value3((s32 (*)())Func_02001e48, 11, 0x100, 3);
  Actor_SetAngleToward(11, 4, 0);
  FieldEvent_Wait(20);
  Func_02001e28(11, 0, 5);
  Func_02001e18((s32)MsgMadraInteriors115);
  Func_02001e48(10, 0x100, 30);
  Func_02001e40(10, FACING_EAST);
  FieldEvent_Wait(10);
  Func_02001e28(10, 0, 5);
  Actor_TurnToFaceEachOther(10, 11, 0);
  FieldEvent_Wait(20);
  Actor_SetAngleToward(10, 4, 0);
  FieldEvent_Wait(20);
  Func_02001e48(10, 0x105, 0);
  FieldEvent_Wait(20);
  Func_02001e28(10, 0, 5);
  Actor_SetAngleToward(11, 4, 0);
  FieldEvent_Wait(10);
  Motion_SetVarCbAndRefresh(11, 2);
  FieldEvent_Wait(5);
  Func_02001e28(11, 0, 5);
  Func_02001d58();
  return;
}

void Func_02001450(void) {
  s32 result0;
  s32 result1;
  struct Actor *object2;
  struct Actor *object3;
  struct Actor *object4;
  struct Actor *object5;

  Func_02001d50();
  Func_02001eb8(0);
  Func_02001e18((s32)MsgMadraInteriors126);
  Value3((s32 (*)())Func_02001e48, 28, 0x100, 30);
  Actor_SetAngleToward(28, 4, 0);
  Func_02001e28(28, 0, 10);
  Actor_SetAngleToward(15, 4, 0);
  Call3((void (*)())Actor_ArmCallback, 17, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_SetAngleToward(18, 4, 0);
  Actor_SetAngleToward(14, 4, 0);
  Actor_SetAngleToward(15, 4, 0);
  Actor_SetAngleToward(16, 4, 0);
  FieldEvent_Wait(20);
  Call3((void (*)())Actor_SetPositionAndReset, 4, 701, 93);
  Call3((void (*)())Actor_OffsetPositionAndReset, 4, -32, 0);
  Call4((void (*)())Func_02001ec8, 5, -8, 32, FACING_WEST);
  Call4((void (*)())Func_02001ec8, 31, -16, 0, FACING_SOUTH + FACING_STEP);
  Call4((void (*)())Func_02001ec8, 6, -8, 16, FACING_SOUTHWEST);
  Call4((void (*)())Func_02001ec8, 7, -24, 16, FACING_SOUTHWEST);
  FieldEvent_Wait(30);
  Value3((s32 (*)())Func_02001e48, 15, 0x100, 30);
  Func_02001e28(15, 0, 10);
  Func_02001e48(27, 0x100, 30);
  FieldEvent_Wait(20);
  Actor_ArmCallback(27, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Motion_SetVarCbAndRefresh(27, 2);
  FieldEvent_Wait(20);
  Func_02001e28(27, 0, 10);
  Motion_SetModeAndWaitAnimation(7, 3);
  FieldEvent_Wait(20);
  Value3((s32 (*)())Func_02001e48, 27, 0x109, 30);
  Func_02001e28(27, 0, 10);
  Actor_TurnToFaceEachOther(7, 4, 0);
  FieldEvent_Wait(30);
  Call3((void (*)())Actor_ArmCallback, 7, FACING_WEST, 0);
  Call3((void (*)())Actor_ArmCallback, 4, FACING_SOUTHWEST, 0);
  FieldEvent_Wait(20);
  Func_02001e28(7, 0, 10);
  Actor_Jump(18, 6, 20);
  FieldEvent_Wait(20);
  Func_02001e28(18, 0, 10);
  Call2((void (*)())Func_02001e40, 7, FACING_SOUTHEAST + FACING_STEP);
  Motion_SetModeAndWaitAnimation(7, 3);
  FieldEvent_Wait(20);
  Func_02001e28(7, 0, 10);
  Value3((s32 (*)())Func_02001e48, 28, 0x102, 30);
  Value2((s32 (*)())FieldMessage_ShowAtActor, 28, 0);
  result0 = Value2((s32 (*)())Event_AskYesNoWithActorAnimation, 4, 0);
  if (result0 != 0)
    goto L_02009690;
  Value3((s32 (*)())Func_02001e48, 27, 0x108, 30);
  Func_02001e28(27, 0, 10);
  Call2((void (*)())Func_02001e40, 7, FACING_SOUTHWEST);
  Motion_SetModeAndWaitAnimation(7, 3);
  FieldEvent_Wait(20);
  Func_02001e28(7, 0, 10);
  Value3((s32 (*)())Func_02001e48, 15, 0x108, 30);
  Func_02001e28(15, 0, 10);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 3;
  goto L_02009700;
L_02009690:
  Value3((s32 (*)())Func_02001e48, 15, 0x102, 30);
  Func_02001ef8();
  ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 3;
  Func_02001e28(27, 0, 10);
  Call2((void (*)())Func_02001e40, 7, FACING_SOUTHWEST);
  Motion_SetModeAndWaitAnimation(7, 4);
  FieldEvent_Wait(20);
  Func_02001e28(7, 0, 10);
  Actor_StartRepeatedMotion(15, 3);
  Func_02001e50(15, 0x102);
  FieldEvent_Wait(50);
  Func_02001e28(15, 0, 10);
L_02009700:
  Actor_SetModeById(7, 3);
  Actor_SetModeById(6, 3);
  Actor_SetModeById(31, 3);
  Actor_SetModeById(5, 3);
  Motion_SetModeAndWaitAnimation(4, 3);
  FieldEvent_Wait(20);
  Motion_SetModeAndWaitAnimation(31, 3);
  FieldEvent_Wait(20);
  Func_02001e28(31, 0, 10);
  result1 = Value1((s32 (*)())GameFlag_Test, 0x981);
  if (result1 == 0)
    goto L_02009760;
  Func_02001910();
  goto L_02009764;
L_02009760:
  Func_02001a20();
L_02009764:
  Call3((void (*)())Actor_ArmCallback, 7, FACING_SOUTH, 0);
  Func_02001e40(4, FACING_SOUTH);
  FieldEvent_Wait(20);
  Actor_SetModeById(7, 3);
  Motion_SetModeAndWaitAnimation(4, 3);
  FieldEvent_Wait(40);
  Call3((void (*)())Actor_ArmCallback, 5, FACING_NORTHWEST, 0);
  Call3((void (*)())Actor_ArmCallback, 6, FACING_WEST, 0);
  Call3((void (*)())Actor_ArmCallback, 31, FACING_SOUTHWEST, 0);
  Call3((void (*)())Actor_ArmCallback, 4, FACING_SOUTHEAST, 0);
  Actor_ArmCallback(7, FACING_EAST, 0);
  FieldEvent_Wait(40);
  Actor_SetModeById(5, 3);
  Actor_SetModeById(6, 3);
  Actor_SetModeById(31, 3);
  Actor_SetModeById(7, 3);
  Motion_SetModeAndWaitAnimation(4, 3);
  FieldEvent_Wait(20);
  Call3((void (*)())Actor_SetSpeedParameters, 31, 0x13333, 0x9999);
  Actor_SetModeById(31, 2);
  object2 = Actor_GetById(4);
  if (object2 == 0)
    goto L_02009832;
  Actor_ResetAndSetPosition(31, *(s16 *)((u8 *)object2 + 10),
                            *(s16 *)((u8 *)object2 + 18));
L_02009832:
  Actor_WaitForMoveById(31);
  Func_02001db8(31, 0, 0);
  Call3((void (*)())Actor_SetSpeedParameters, 6, 0x13333, 0x9999);
  Actor_SetModeById(6, 2);
  object3 = Actor_GetById(4);
  if (object3 == 0)
    goto L_02009870;
  Actor_ResetAndSetPosition(6, *(s16 *)((u8 *)object3 + 10),
                            *(s16 *)((u8 *)object3 + 18));
L_02009870:
  Actor_WaitForMoveById(6);
  Func_02001db8(6, 0, 0);
  Call3((void (*)())Actor_SetSpeedParameters, 7, 0x13333, 0x9999);
  Actor_SetModeById(7, 2);
  object4 = Actor_GetById(4);
  if (object4 == 0)
    goto L_020098ae;
  Actor_ResetAndSetPosition(7, *(s16 *)((u8 *)object4 + 10),
                            *(s16 *)((u8 *)object4 + 18));
L_020098ae:
  Actor_WaitForMoveById(7);
  Func_02001db8(7, 0, 0);
  Call3((void (*)())Actor_SetSpeedParameters, 5, 0x13333, 0x9999);
  Actor_SetModeById(5, 2);
  object5 = Actor_GetById(4);
  if (object5 == 0)
    goto L_020098ec;
  Actor_ResetAndSetPosition(5, *(s16 *)((u8 *)object5 + 10),
                            *(s16 *)((u8 *)object5 + 18));
L_020098ec:
  Actor_WaitForMoveById(5);
  Func_02001db8(5, 0, 0);
  GameFlag_SetBit(0x989);
  Func_02001d58();
  return;
}

void Func_02001910(void) {
  Func_02001e18((s32)MsgMadraInteriors127);
  Func_02001e28(28, 0, 10);
  Actor_TurnToFaceEachOther(4, 31, 0);
  FieldEvent_Wait(40);
  Call3((void (*)())Actor_ArmCallback, 4, FACING_SOUTHWEST, 0);
  Actor_ArmCallback(31, FACING_SOUTH + FACING_STEP, 0);
  FieldEvent_Wait(20);
  Motion_SetModeAndWaitAnimation(4, 3);
  FieldEvent_Wait(20);
  Value3((s32 (*)())Func_02001e48, 15, 0x100, 30);
  Func_02001e28(15, 0, 10);
  Motion_SetModeAndWaitAnimation(7, 3);
  FieldEvent_Wait(20);
  Func_02001e28(7, 0, 10);
  Value3((s32 (*)())Func_02001e48, 27, 0x102, 30);
  Func_02001e28(27, 0, 10);
  Motion_SetModeAndWaitAnimation(31, 3);
  FieldEvent_Wait(20);
  Func_02001e28(31, 0, 10);
  Value3((s32 (*)())Func_02001e48, 5, 0x108, 30);
  Func_02001e28(5, 0, 10);
  Motion_SetModeAndWaitAnimation(17, 3);
  FieldEvent_Wait(20);
  Func_02001e28(17, 0, 10);
  Call2((void (*)())Func_02001e40, 5, FACING_SOUTH);
  Motion_SetModeAndWaitAnimation(5, 3);
  FieldEvent_Wait(20);
  Motion_SetModeAndWaitAnimation(18, 3);
  FieldEvent_Wait(20);
  Func_02001e28(18, 0, 10);
  return;
}

void Func_02001a20(void) {
  Func_02001e18((s32)MsgMadraInteriors128);
  Motion_SetModeAndWaitAnimation(17, 3);
  FieldEvent_Wait(20);
  Func_02001e28(17, 0, 10);
  Call2((void (*)())Func_02001e40, 5, FACING_SOUTH);
  Motion_SetModeAndWaitAnimation(5, 4);
  FieldEvent_Wait(20);
  Func_02001e28(5, 0, 10);
  Actor_SetAngleToward(6, 5, 0);
  FieldEvent_Wait(20);
  Func_02001e28(6, 0, 10);
  Call2((void (*)())Func_02001e40, 5, FACING_NORTH);
  Motion_SetModeAndWaitAnimation(5, 3);
  FieldEvent_Wait(20);
  Value3((s32 (*)())Func_02001e48, 17, 0x101, 30);
  Actor_SetAngleToward(7, 17, 0);
  Value3((s32 (*)())Func_02001e48, 7, 0x108, 30);
  Func_02001e28(7, 0, 10);
  Value3((s32 (*)())Func_02001e48, 31, 0x102, 30);
  Actor_SetAngleToward(31, 7, 0);
  Func_02001e28(31, 0, 10);
  Actor_SetAngleToward(7, 31, 0);
  FieldEvent_Wait(60);
  Motion_SetModeAndWaitAnimation(7, 3);
  FieldEvent_Wait(20);
  Func_02001e28(7, 0, 10);
  Actor_SetAngleToward(4, 7, 0);
  Actor_SetAngleToward(6, 7, 0);
  Actor_SetAngleToward(5, 7, 0);
  FieldEvent_Wait(20);
  Value3((s32 (*)())Func_02001e48, 6, 0x101, 30);
  Func_02001e28(6, 0, 10);
  Actor_SetAngleToward(7, 6, 0);
  FieldEvent_Wait(20);
  Func_02001e28(7, 0, 10);
  Value3((s32 (*)())Func_02001e48, 4, 0x101, 30);
  Actor_SetAngleToward(7, 4, 0);
  FieldEvent_Wait(20);
  Func_02001e28(7, 0, 10);
  Motion_SetModeAndWaitAnimation(7, 3);
  FieldEvent_Wait(20);
  Func_02001e28(7, 0, 10);
  Value3((s32 (*)())Func_02001e48, 5, 0x109, 30);
  Func_02001e28(5, 0, 10);
  Motion_SetModeAndWaitAnimation(7, 4);
  FieldEvent_Wait(20);
  Call2((void (*)())Func_02001e40, 7, FACING_SOUTHEAST);
  Func_02001e28(7, 0, 10);
  Motion_SetModeAndWaitAnimation(5, 3);
  FieldEvent_Wait(20);
  Actor_SetAngleToward(7, 31, 0);
  FieldEvent_Wait(20);
  Func_02001e28(7, 0, 10);
  Motion_SetModeAndWaitAnimation(31, 3);
  FieldEvent_Wait(20);
  Actor_SetAngleToward(7, 4, 0);
  FieldEvent_Wait(20);
  Func_02001e28(7, 0, 10);
  GameFlag_SetBit(0x981);
  Value3((s32 (*)())Func_02001e48, 15, 0x100, 30);
  Func_02001e28(15, 0, 10);
  Call2((void (*)())Func_02001e40, 7, FACING_SOUTHWEST);
  Motion_SetModeAndWaitAnimation(7, 3);
  FieldEvent_Wait(20);
  Func_02001e28(7, 0, 10);
  Call3((void (*)())Actor_ArmCallback, 4, FACING_SOUTHWEST, 0);
  Value3((s32 (*)())Func_02001e48, 27, 0x102, 30);
  Func_02001e28(27, 0, 10);
  Motion_SetModeAndWaitAnimation(31, 3);
  FieldEvent_Wait(20);
  Func_02001e28(31, 0, 10);
  Value3((s32 (*)())Func_02001e48, 5, 0x108, 30);
  Call3((void (*)())Actor_ArmCallback, 5, FACING_WEST, 0);
  Func_02001e28(5, 0, 10);
  Motion_SetModeAndWaitAnimation(17, 3);
  FieldEvent_Wait(20);
  Func_02001e28(17, 0, 10);
  Call2((void (*)())Func_02001e40, 5, FACING_SOUTH);
  Motion_SetModeAndWaitAnimation(5, 3);
  FieldEvent_Wait(20);
  Motion_SetModeAndWaitAnimation(18, 3);
  FieldEvent_Wait(20);
  Func_02001e28(18, 0, 10);
  return;
}
