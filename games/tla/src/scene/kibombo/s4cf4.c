#include "kibombo.h"
#include "scene_tables.h"

void Func_02005a4c(void);
void Actor_SetHorizontalPositionAndAngleWithTerrain(s32 actor, s32 x, s32 z,
                                                    s32 angle);
void BattleEffect_ApplyColorToTargetBuffer(s32 a, s32 b);
void BattleEffect_ApplyColorToSourceBuffer(s32 a, s32 b);
void BattleEffect_StartBufferInterpolation(s32 frames);

void Func_02004cf4(void) {
  struct KibomboEventWork *work;
  struct KibomboObject *leader;

  Func_02005a9c();
  Event_ResolvePendingActions(0);
  Kibombo_CamMove(-1, -1, -1, 0);
  Camera_MoveWithinMapBounds(PIXELS(384), -1, PIXELS(601), 0);
  wait(1);
  Actor_SetHorizontalPositionAndAngleWithTerrain(
      gMainRoutineWork.currentOwner, PIXELS(370), PIXELS(636), FACING_NORTH);
  Func_02005a4c();
  wait(1);
  ((struct KibomboEventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x100;
  BattleEffect_ApplyColorToSourceBuffer(0, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10002, 0);
  ((struct KibomboEventWork *)Ram_HeapSlots->eventWork)->field1b4 = 60;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(40);
  Kibombo_ObjArm(10, FACING_NORTHWEST + FACING_STEP, 20);
  Kibombo_ObjArm(10, FACING_NORTH + FACING_STEP, 20);
  Motion_SetModeAndWaitAnimation(10, 3);
  Actor_SetModeById(9, 3);
  Motion_SetModeAndWaitAnimation(8, 3);
  Kibombo_ObjArm(10, FACING_NORTHWEST + FACING_STEP, 20);
  Kibombo_ObjArm(10, FACING_NORTH + FACING_STEP, 20);
  Actor_GetById(10)->velocityY = PIXELS(3);
  Event_Wait(40);
  Actor_SetModeById(9, 3);
  Motion_SetModeAndWaitAnimation(8, 3);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  BattleEffect_StartBufferInterpolation(40);
  wait(60);
  Motion_SetModeAndWaitAnimation(10, 3);
  Event_SetMessageId((s32)MsgRightWellI);
  Event_ShowMessageAndWait(10, 0);
  Motion_SetModeAndWaitAnimation(9, 3);
  Event_ShowMessageAndWait(9, 0);
  Actor_ArmCallback(10, FACING_EAST, 0);
  Kibombo_ObjArm(gMainRoutineWork.currentOwner, FACING_WEST, 40);
  Kibombo_ObjArm(10, FACING_NORTH + FACING_STEP, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTH, 10);
  Actor_SetModeById(10, 3);
  Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 3);
  Actor_Jump(8, 4, 10);
  Kibombo_MessagePut(0x2008, 0);
  Actor_ArmCallbackAndRefresh(9, FACING_EAST);
  Func_080d47b4(8, 259, 20);
  Kibombo_MessagePut(0x2008, 0);
  Func_080d47b4(9, 256, 20);
  Event_ShowMessageAndWait(9, 0);
  Kibombo_ObjTurn(8, FACING_WEST);
  Actor_SetModeById(8, 4);
  Kibombo_MessagePut(0x2008, 0);
  Kibombo_ObjTurn(8, FACING_SOUTH + FACING_STEP);
  Kibombo_MessagePut(0x2008, 0);
  Actor_SetModeById(8, 4);
  Event_ShowMessageAndWait(0x2008, 0);
  Kibombo_ObjArm(9, FACING_NORTH + FACING_STEP, 0);
  Kibombo_ObjSpeed(8, 0x19999, 0xcccc);
  Kibombo_ObjWalk(8, 364, 576);
  Kibombo_ObjArm(8, FACING_NORTHWEST + FACING_STEP, 0);
  Func_080d47b4(9, 261, 80);
  Event_ShowMessageAndWait(9, 0);
  Kibombo_ObjTurn(9, FACING_SOUTHEAST + FACING_STEP);
  Event_ShowMessageAndWait(9, 0);
  Actor_SetModeById(9, 4);
  Event_ShowMessageAndWait(9, 0);
  Event_ShowMessageAndWait(9, 0);
  Motion_SetModeAndWaitAnimation(10, 3);
  Event_ShowMessageAndWait(10, 0);
  Actor_ArmCallbackAndRefresh(10, FACING_EAST);
  Event_ShowMessageAndWait(10, 0);
  Actor_ArmCallbackAndRefresh(gMainRoutineWork.currentOwner, FACING_WEST);
  Actor_SetModeById(10, 3);
  Event_ShowMessageAndWait(10, 0);
  Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 3);
  Kibombo_ObjSpeed(10, 0xcccc, 0x6666);
  Actor_SetModeById(10, 2);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  if (leader != 0)
    Actor_ResetAndSetPosition(10, ((s16 *)&leader->x)[1],
                              ((s16 *)&leader->z)[1]);
  Actor_WaitForMoveById(10);
  Actor_SetHorizontalPositionWithTerrain(10, 0, 0);
  work = Ram_HeapSlots->eventWork;
  work->field1b4 = 16;
  work->screenEffect = 0x209;
  GameFlag_SetBit(0x8ef);
  Func_02005aa4();
}
