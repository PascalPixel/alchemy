#include "types.h"
#include "scene_tables.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "call.h"
#include "text/msg_ids.h"
#include "edition.h"

void FieldEvent_Wait();
void Func_0200282c();
void Func_02002834();
void Func_02002874();
void Func_020028ac();
void Func_020028bc();
void Func_020028dc();
void Func_020028e4();
void Func_020028fc();
void Func_02002904();
void GameFlag_SetBit();
void Event_AskYesNoWithActorAnimation();
void Motion_SetModeAndWaitAnimation();
void Motion_SetVarCbAndRefresh();
void Actor_ArmCallback();
void Actor_WaitForMoveById();
void Actor_CommitPositionAndActivate();
void Actor_Stop();
void Actor_Jump();
void Actor_ResetAndSetPosition();
void Actor_SetPositionAndReset();
void Actor_SetSpeedParameters();
void Actor_StartRepeatedMotion();
struct Actor *Actor_GetById();
void Actor_WatchAndSetScript();
void Actor_SetModeById();
void Party_RemoveActiveOwner();
void FieldMessage_ShowAtActor();

s32 Func_02000284(void) {
  struct Actor *object;

#if (LANGUAGE == LANGUAGE_JAPANESE)
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x201;
#else
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x209;
#endif
  Actor_SetSpritePriority(11, 1);
  Actor_SetSpritePriority(14, 2);
  Actor_SetSpritePriority(10, 2);
  object = Actor_GetById(15);
  object->motionFlags = 0;
  object->terrainHeight = 0;
  object->y = 0;
  object = Actor_GetById(17);
  object->motionFlags = 0;
  object->terrainHeight = 0;
  object->y = 0;
  if (GameFlag_Test(0x966)) {
    Actor_SetPositionAndReset(10, 100, 112);
    Actor_GetById(10)->angle = FACING_SOUTHEAST + FACING_STEP;
    wait(1);
  }
  if (GameFlag_Test(0x969))
    Func_02002874(18, 0, 0);
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

void Func_0200031c(void) {
  struct Actor *object0;
  struct Actor *object1;
  s32 result2;
  struct Actor *object3;

  GameFlag_SetBit(0x965);
  Party_RemoveActiveOwner(7);
  Func_0200282c();
  Func_020028fc(0);
  Func_020028ac((s32)MsgLemuriaInteriors07);
  FieldEvent_Wait(20);
  Call3((void (*)())Actor_ArmCallback, 10, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_SetPositionAndReset(4, 112, 128);
  Call4((void (*)())Func_02002904, 12, 16, 0, FACING_NORTHWEST + FACING_STEP);
  Call4((void (*)())Func_02002904, 7, 16, -16, FACING_WEST);
  Actor_ArmCallback(4, FACING_NORTH, 0);
  Actor_WaitForMoveById(7);
  FieldEvent_Wait(20);
  Motion_SetModeAndWaitAnimation(7, 3);
  FieldEvent_Wait(10);
  Func_020028bc(7, 0, 10);
  Value3((s32 (*)())Func_020028dc, 10, 0x100, 30);
  Actor_ArmCallback(10, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_Jump(10, 6, 15);
  Actor_Jump(10, 6, 23);
  Func_020028bc(10, 0, 10);
  Motion_SetModeAndWaitAnimation(7, 3);
  FieldEvent_Wait(10);
  Func_020028bc(7, 0, 10);
  Value3((s32 (*)())Func_020028dc, 12, 0x100, 45);
  Call3((void (*)())Actor_ArmCallback, 12, FACING_WEST, 0);
  FieldEvent_Wait(20);
  Call3((void (*)())Func_020028bc, 0x200c, 0, 10);
  Actor_ArmCallback(4, FACING_EAST, 0);
  FieldEvent_Wait(30);
  Motion_SetModeAndWaitAnimation(4, 3);
  FieldEvent_Wait(20);
  Call3((void (*)())Actor_ArmCallback, 12, FACING_NORTHWEST + FACING_STEP, 0);
  Call3((void (*)())Actor_ArmCallback, 4, FACING_NORTHWEST, 0);
  FieldEvent_Wait(30);
  Call3((void (*)())Actor_ArmCallback, 10, FACING_SOUTHEAST, 0);
  FieldEvent_Wait(20);
  Value3((s32 (*)())Func_020028dc, 10, 0x108, 30);
  Func_020028bc(10, 0, 10);
  Call3((void (*)())Actor_ArmCallback, 12, FACING_WEST, 0);
  Actor_ArmCallback(4, FACING_EAST, 0);
  FieldEvent_Wait(40);
  Call3((void (*)())Actor_ArmCallback, 12, FACING_NORTHWEST + FACING_STEP, 0);
  Call3((void (*)())Actor_ArmCallback, 4, FACING_NORTHWEST, 0);
  FieldEvent_Wait(30);
  Actor_SetModeById(12, 3);
  Motion_SetModeAndWaitAnimation(4, 3);
  FieldEvent_Wait(30);
  Call3((void (*)())Actor_ArmCallback, 7, FACING_SOUTH, 0);
  FieldEvent_Wait(45);
  Call3((void (*)())Actor_ArmCallback, 7, FACING_WEST, 0);
  FieldEvent_Wait(20);
  Func_020028bc(7, 0, 10);
  Value3((s32 (*)())Func_020028dc, 10, 0x102, 40);
  Actor_ArmCallback(10, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Func_020028bc(10, 0, 10);
  Motion_SetVarCbAndRefresh(10, 2);
  FieldEvent_Wait(10);
  Actor_ArmCallback(10, FACING_SOUTHEAST + FACING_STEP, 0);
  FieldEvent_Wait(30);
  Value3((s32 (*)())Func_020028dc, 7, 0x100, 45);
  Call3((void (*)())Actor_ArmCallback, 7, FACING_NORTHEAST, 0);
  FieldEvent_Wait(35);
  Call3((void (*)())Actor_ArmCallback, 7, FACING_SOUTH, 0);
  FieldEvent_Wait(35);
  Call3((void (*)())Actor_ArmCallback, 7, FACING_NORTHEAST, 0);
  FieldEvent_Wait(35);
  Call3((void (*)())Actor_ArmCallback, 7, FACING_WEST, 0);
  FieldEvent_Wait(35);
  Func_020028bc(7, 0, 10);
  Motion_SetVarCbAndRefresh(10, 2);
  FieldEvent_Wait(20);
  Value3((s32 (*)())Func_020028dc, 10, 0x105, 60);
  Actor_CommitPositionAndActivate(7, 48, 0);
  FieldEvent_Wait(10);
  Call3((void (*)())Actor_ArmCallback, 7, FACING_NORTHEAST, 0);
  FieldEvent_Wait(30);
  Call3((void (*)())Actor_ArmCallback, 7, FACING_NORTHWEST, 0);
  FieldEvent_Wait(30);
  Call3((void (*)())Actor_ArmCallback, 7, FACING_NORTHEAST, 0);
  FieldEvent_Wait(30);
  Call3((void (*)())Actor_ArmCallback, 7, FACING_NORTHWEST, 0);
  FieldEvent_Wait(40);
  Actor_CommitPositionAndActivate(7, -24, 0);
  FieldEvent_Wait(10);
  Call3((void (*)())Actor_ArmCallback, 7, FACING_SOUTHEAST, 0);
  FieldEvent_Wait(30);
  Call3((void (*)())Actor_ArmCallback, 7, FACING_SOUTHWEST, 0);
  FieldEvent_Wait(30);
  Actor_ArmCallback(7, FACING_SOUTHEAST, 0);
  FieldEvent_Wait(30);
  Actor_ArmCallback(7, FACING_SOUTH, 0);
  FieldEvent_Wait(40);
  Value3((s32 (*)())Func_020028dc, 7, 0x101, 30);
  Func_020028bc(7, 0, 10);
  Value3((s32 (*)())Func_020028dc, 7, 0x102, 40);
  Func_020028bc(7, 0, 10);
  Value3((s32 (*)())Func_020028dc, 12, 0x100, 30);
  Call3((void (*)())Actor_ArmCallback, 4, FACING_NORTHEAST, 0);
  Call3((void (*)())Actor_ArmCallback, 12, FACING_NORTH + FACING_STEP, 0);
  FieldEvent_Wait(20);
  Call3((void (*)())Func_020028bc, 0x200c, 0, 10);
  Motion_SetVarCbAndRefresh(7, 2);
  FieldEvent_Wait(20);
  Actor_ArmCallback(7, FACING_SOUTHWEST, 0);
  FieldEvent_Wait(20);
  Value3((s32 (*)())Func_020028dc, 7, 0x109, 30);
  Func_020028bc(7, 0, 10);
  Actor_ArmCallback(10, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Motion_SetModeAndWaitAnimation(10, 4);
  Func_020028bc(10, 0, 10);
  Call3((void (*)())Actor_ArmCallback, 12, FACING_WEST, 0);
  Actor_ArmCallback(4, FACING_EAST, 0);
  FieldEvent_Wait(50);
  Call3((void (*)())Actor_ArmCallback, 12, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_ArmCallback(4, FACING_NORTHWEST, 0);
  FieldEvent_Wait(30);
  Call3((void (*)())Actor_ArmCallback, 7, FACING_WEST, 0);
  FieldEvent_Wait(20);
  Actor_StartRepeatedMotion(7, 3);
  Call2((void (*)())Func_020028e4, 7, 0x102);
  FieldEvent_Wait(50);
  Actor_CommitPositionAndActivate(7, -12, 0);
  FieldEvent_Wait(10);
  Value3((s32 (*)())Func_020028dc, 10, 0x109, 30);
  Func_020028bc(10, 0, 10);
  Motion_SetVarCbAndRefresh(7, 2);
  FieldEvent_Wait(20);
  Motion_SetModeAndWaitAnimation(10, 3);
  FieldEvent_Wait(10);
  Func_020028bc(10, 0, 10);
  Motion_SetModeAndWaitAnimation(7, 4);
  FieldEvent_Wait(20);
  Motion_SetModeAndWaitAnimation(10, 4);
  Func_020028bc(10, 0, 10);
  Actor_ArmCallback(10, FACING_NORTHWEST + FACING_STEP, 0);
  FieldEvent_Wait(30);
  Func_020028bc(10, 0, 20);
  Motion_SetModeAndWaitAnimation(10, 3);
  FieldEvent_Wait(10);
  Func_020028bc(10, 0, 10);
  FieldEvent_Wait(10);
  Actor_SetModeById(7, 9);
  FieldEvent_Wait(40);
  Call3((void (*)())Actor_ArmCallback, 4, FACING_NORTHEAST, 0);
  Actor_ArmCallback(12, FACING_NORTH + FACING_STEP, 0);
  FieldEvent_Wait(20);
  Func_020028bc(7, 0, 10);
  Actor_ArmCallback(10, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Func_020028bc(10, 0, 10);
  Actor_SetModeById(7, 1);
  FieldEvent_Wait(30);
  Actor_SetSpeedParameters(7, 0x4ccc, 0x2666);
  object0 = Actor_GetById(7);
  object0->actionFlags &= 254;
  Actor_CommitPositionAndActivate(7, 12, 0);
  FieldEvent_Wait(1);
  object1 = Actor_GetById(7);
  object1->actionFlags |= 1;
  FieldEvent_Wait(10);
  Actor_StartRepeatedMotion(7, 3);
  Func_020028e4(7, 0x102);
  FieldEvent_Wait(55);
  Actor_WatchAndSetScript(4, 7);
  Actor_WatchAndSetScript(12, 7);
  Actor_WatchAndSetScript(10, 7);
  Call3((void (*)())Actor_SetSpeedParameters, 7, 0x23333, 0x11999);
  Actor_CommitPositionAndActivate(7, -8, 88);
  Actor_Stop(4);
  Actor_Stop(12);
  Actor_Stop(10);
  Func_02002874(7, 0, 0);
  FieldEvent_Wait(30);
  Value3((s32 (*)())Func_020028dc, 12, 0x109, 30);
  Call3((void (*)())Actor_ArmCallback, 12, FACING_WEST, 0);
  Actor_ArmCallback(4, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Value2((s32 (*)())FieldMessage_ShowAtActor, 0x200c, 0);
  result2 = Value2((s32 (*)())Event_AskYesNoWithActorAnimation, 4, 0);
  if (result2 != 0)
    goto L_020089be;
  FieldEvent_Wait(20);
  Func_020028bc(0x200c, 0, 10);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
  goto L_020089e4;
L_020089be:
  FieldEvent_Wait(40);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
  Func_020028bc(0x200c, 0, 10);
L_020089e4:
  Motion_SetModeAndWaitAnimation(12, 3);
  FieldEvent_Wait(10);
  Motion_SetModeAndWaitAnimation(4, 3);
  FieldEvent_Wait(20);
  Call3((void (*)())Actor_SetSpeedParameters, 12, 0x13333, 0x9999);
  Actor_SetModeById(12, 2);
  object3 = Actor_GetById(gMainRoutineWork.currentOwner);
  if (object3 == 0)
    goto L_02008a36;
  Actor_ResetAndSetPosition(12, *(s16 *)((u8 *)object3 + 10),
                            *(s16 *)((u8 *)object3 + 18));
L_02008a36:
  Actor_WaitForMoveById(12);
  Func_02002874(12, 0, 0);
  Func_02002834();
  return;
}
