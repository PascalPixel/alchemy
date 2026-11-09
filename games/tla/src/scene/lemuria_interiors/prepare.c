#include "types.h"
#include "scene_tables.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "text/msg_ids.h"
#include "version.h"
#include "event_inline.h"

void Func_0200282c();
void Func_02002834();
void Party_RemoveActiveOwner();

s32 Func_02000284(void) {
  struct Actor *object;

#if !TLA_PATCH_1
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
    Actor_SetHorizontalPositionWithTerrain(18, 0, 0);
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
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgLemuriaInteriors07);
  Event_Wait(20);
  Actor_FaceDirection(10, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_SetPositionAndReset(4, 112, 128);
  FieldActor_LaunchFromLeader(12, 16, 0, FACING_NORTHWEST + FACING_STEP);
  FieldActor_LaunchFromLeader(7, 16, -16, FACING_WEST);
  Actor_ArmCallback(4, FACING_NORTH, 0);
  Actor_WaitForMoveById(7);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(7, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(7, 0, 10);
  Actor_ShowEmote(10, 0x100, 30);
  Actor_ArmCallback(10, FACING_EAST, 0);
  Event_Wait(20);
  Actor_Jump(10, 6, 15);
  Actor_Jump(10, 6, 23);
  Event_ShowMessageThenWait(10, 0, 10);
  Motion_SetModeAndWaitAnimation(7, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(7, 0, 10);
  Actor_ShowEmote(12, 0x100, 45);
  Actor_FaceDirection(12, FACING_WEST, 0);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(0x200c, 0, 10);
  Actor_ArmCallback(4, FACING_EAST, 0);
  Event_Wait(30);
  Motion_SetModeAndWaitAnimation(4, 3);
  Event_Wait(20);
  Actor_FaceDirection(12, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(4, FACING_NORTHWEST, 0);
  Event_Wait(30);
  Actor_FaceDirection(10, FACING_SOUTHEAST, 0);
  Event_Wait(20);
  Actor_ShowEmote(10, 0x108, 30);
  Event_ShowMessageThenWait(10, 0, 10);
  Actor_FaceDirection(12, FACING_WEST, 0);
  Actor_ArmCallback(4, FACING_EAST, 0);
  Event_Wait(40);
  Actor_FaceDirection(12, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(4, FACING_NORTHWEST, 0);
  Event_Wait(30);
  Actor_SetModeById(12, 3);
  Motion_SetModeAndWaitAnimation(4, 3);
  Event_Wait(30);
  Actor_FaceDirection(7, FACING_SOUTH, 0);
  Event_Wait(45);
  Actor_FaceDirection(7, FACING_WEST, 0);
  Event_Wait(20);
  Event_ShowMessageThenWait(7, 0, 10);
  Actor_ShowEmote(10, 0x102, 40);
  Actor_ArmCallback(10, FACING_EAST, 0);
  Event_Wait(20);
  Event_ShowMessageThenWait(10, 0, 10);
  Motion_SetVarCbAndRefresh(10, 2);
  Event_Wait(10);
  Actor_ArmCallback(10, FACING_SOUTHEAST + FACING_STEP, 0);
  Event_Wait(30);
  Actor_ShowEmote(7, 0x100, 45);
  Actor_FaceDirection(7, FACING_NORTHEAST, 0);
  Event_Wait(35);
  Actor_FaceDirection(7, FACING_SOUTH, 0);
  Event_Wait(35);
  Actor_FaceDirection(7, FACING_NORTHEAST, 0);
  Event_Wait(35);
  Actor_FaceDirection(7, FACING_WEST, 0);
  Event_Wait(35);
  Event_ShowMessageThenWait(7, 0, 10);
  Motion_SetVarCbAndRefresh(10, 2);
  Event_Wait(20);
  Actor_ShowEmote(10, 0x105, 60);
  Actor_CommitPositionAndActivate(7, 48, 0);
  Event_Wait(10);
  Actor_FaceDirection(7, FACING_NORTHEAST, 0);
  Event_Wait(30);
  Actor_FaceDirection(7, FACING_NORTHWEST, 0);
  Event_Wait(30);
  Actor_FaceDirection(7, FACING_NORTHEAST, 0);
  Event_Wait(30);
  Actor_FaceDirection(7, FACING_NORTHWEST, 0);
  Event_Wait(40);
  Actor_CommitPositionAndActivate(7, -24, 0);
  Event_Wait(10);
  Actor_FaceDirection(7, FACING_SOUTHEAST, 0);
  Event_Wait(30);
  Actor_FaceDirection(7, FACING_SOUTHWEST, 0);
  Event_Wait(30);
  Actor_ArmCallback(7, FACING_SOUTHEAST, 0);
  Event_Wait(30);
  Actor_ArmCallback(7, FACING_SOUTH, 0);
  Event_Wait(40);
  Actor_ShowEmote(7, 0x101, 30);
  Event_ShowMessageThenWait(7, 0, 10);
  Actor_ShowEmote(7, 0x102, 40);
  Event_ShowMessageThenWait(7, 0, 10);
  Actor_ShowEmote(12, 0x100, 30);
  Actor_FaceDirection(4, FACING_NORTHEAST, 0);
  Actor_FaceDirection(12, FACING_NORTH + FACING_STEP, 0);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(0x200c, 0, 10);
  Motion_SetVarCbAndRefresh(7, 2);
  Event_Wait(20);
  Actor_ArmCallback(7, FACING_SOUTHWEST, 0);
  Event_Wait(20);
  Actor_ShowEmote(7, 0x109, 30);
  Event_ShowMessageThenWait(7, 0, 10);
  Actor_ArmCallback(10, FACING_EAST, 0);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(10, 4);
  Event_ShowMessageThenWait(10, 0, 10);
  Actor_FaceDirection(12, FACING_WEST, 0);
  Actor_ArmCallback(4, FACING_EAST, 0);
  Event_Wait(50);
  Actor_FaceDirection(12, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_ArmCallback(4, FACING_NORTHWEST, 0);
  Event_Wait(30);
  Actor_FaceDirection(7, FACING_WEST, 0);
  Event_Wait(20);
  Actor_StartRepeatedMotion(7, 3);
  Actor_SetAttachedEffect(7, 0x102);
  Event_Wait(50);
  Actor_CommitPositionAndActivate(7, -12, 0);
  Event_Wait(10);
  Actor_ShowEmote(10, 0x109, 30);
  Event_ShowMessageThenWait(10, 0, 10);
  Motion_SetVarCbAndRefresh(7, 2);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(10, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(10, 0, 10);
  Motion_SetModeAndWaitAnimation(7, 4);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(10, 4);
  Event_ShowMessageThenWait(10, 0, 10);
  Actor_ArmCallback(10, FACING_NORTHWEST + FACING_STEP, 0);
  Event_Wait(30);
  Event_ShowMessageThenWait(10, 0, 20);
  Motion_SetModeAndWaitAnimation(10, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(10, 0, 10);
  Event_Wait(10);
  Actor_SetModeById(7, 9);
  Event_Wait(40);
  Actor_FaceDirection(4, FACING_NORTHEAST, 0);
  Actor_ArmCallback(12, FACING_NORTH + FACING_STEP, 0);
  Event_Wait(20);
  Event_ShowMessageThenWait(7, 0, 10);
  Actor_ArmCallback(10, FACING_EAST, 0);
  Event_Wait(20);
  Event_ShowMessageThenWait(10, 0, 10);
  Actor_SetModeById(7, 1);
  Event_Wait(30);
  Actor_SetSpeedParameters(7, 0x4ccc, 0x2666);
  object0 = Actor_GetById(7);
  object0->actionFlags &= 254;
  Actor_CommitPositionAndActivate(7, 12, 0);
  Event_Wait(1);
  object1 = Actor_GetById(7);
  object1->actionFlags |= 1;
  Event_Wait(10);
  Actor_StartRepeatedMotion(7, 3);
  Func_080d489c(7, 0x102);
  Event_Wait(55);
  Actor_WatchAndSetScript(4, 7);
  Actor_WatchAndSetScript(12, 7);
  Actor_WatchAndSetScript(10, 7);
  Actor_SetSpeed(7, 0x23333, 0x11999);
  Actor_CommitPositionAndActivate(7, -8, 88);
  Actor_Stop(4);
  Actor_Stop(12);
  Actor_Stop(10);
  Actor_SetHorizontalPositionWithTerrain(7, 0, 0);
  Event_Wait(30);
  Actor_ShowEmote(12, 0x109, 30);
  Actor_FaceDirection(12, FACING_WEST, 0);
  Actor_ArmCallback(4, FACING_EAST, 0);
  Event_Wait(20);
  Event_OpenMessage(0x200c, 0);
  result2 = FieldEvent_AskYesNoWithActorAnimation(4, 0);
  if (result2 != 0)
    goto L_020089be;
  Event_Wait(20);
  Event_ShowMessageThenWait(0x200c, 0, 10);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
  goto L_020089e4;
L_020089be:
  Event_Wait(40);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
  Event_ShowMessageThenWait(0x200c, 0, 10);
L_020089e4:
  Motion_SetModeAndWaitAnimation(12, 3);
  Event_Wait(10);
  Motion_SetModeAndWaitAnimation(4, 3);
  Event_Wait(20);
  Actor_SetSpeed(12, 0x13333, 0x9999);
  Actor_SetModeById(12, 2);
  object3 = Actor_GetById(gMainRoutineWork.currentOwner);
  if (object3 == 0)
    goto L_02008a36;
  Actor_ResetAndSetPosition(12, *(s16 *)((u8 *)object3 + 10),
                            *(s16 *)((u8 *)object3 + 18));
L_02008a36:
  Actor_WaitForMoveById(12);
  Actor_SetHorizontalPositionWithTerrain(12, 0, 0);
  Func_02002834();
  return;
}
