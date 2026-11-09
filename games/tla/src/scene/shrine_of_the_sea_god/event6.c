#include "shrine_of_the_sea_god.h"
#include "text/msg_ids.h"
#include "scene_tables.h"

void Func_020019dc(void) {
  ShrineOfTheSeaGod_CopyRect(42, 1, 15, 14, 1, 2);
  ShrineOfTheSeaGod_BlendStart(0, 0, 1, 1, 15, 14);
}

void Func_02001a0c(void) {
  ShrineOfTheSeaGod_CopyRect(15, 9, 15, 14, 1, 2);
  ShrineOfTheSeaGod_BlendStart(14, 14, 1, 1, 15, 14);
}

void Func_02001a3c(void) {
  struct ShrineOfTheSeaGodObject *leader;

  Func_020039d8();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgFieldRopeThrownAgain);
  Event_ShowMessageAndWait(0x200c, 0);
  ShrineOfTheSeaGod_ObjSpeed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  ShrineOfTheSeaGod_ObjSpeed(14, 0xcccc, 0x6666);
  ShrineOfTheSeaGod_ObjWalk(gMainRoutineWork.currentOwner, 272, 336);
  ShrineOfTheSeaGod_ObjTurn(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
  Actor_SetHorizontalPositionWithTerrain(14, 272 << 16, 336 << 16);
  wait(1);
  ShrineOfTheSeaGod_ObjWalk(14, 256, 336);
  Actor_ArmCallback(14, 0, 0);
  ShrineOfTheSeaGod_ObjTurn(gMainRoutineWork.currentOwner, FACING_WEST, 40);
  ShrineOfTheSeaGod_ObjTurn(14, 0xd000, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
  ShrineOfTheSeaGod_CamMove(-1, -1, -1, 0);
  ShrineOfTheSeaGod_CamSpeed(0x4cccc, 0x9999);
  ShrineOfTheSeaGod_CamMove(288 << 16, -1, 126 << 16, 1);
  Func_02003ac8();
  Actor_Jump(13, 4, 20);
  ShrineOfTheSeaGod_MessagePut(0x800d, 0);
  Motion_SetModeAndWaitAnimation(12, 3);
  Event_Wait(20);
  Motion_SetVarCbAndRefresh(12, 2);
  Actor_Jump(12, 6, 80);
  ShrineOfTheSeaGod_MessagePut(0x800d, 0);
  Actor_SetModeById(12, 4);
  ShrineOfTheSeaGod_MessagePut(0x400c, 0);
  ShrineOfTheSeaGod_ObjTurn(13, 0x3000, 0);
  ShrineOfTheSeaGod_Jump(13, 265, 20);
  ShrineOfTheSeaGod_MessagePut(0x800d, 0);
  ShrineOfTheSeaGod_MessagePut(0x400c, 0);
  Actor_ArmCallback(13, 0, 0);
  Func_080d489c(13, 258);
  Event_Wait(20);
  Event_ShowMessageAndWait(0x800d, 0);
  Motion_SetModeAndWaitAnimation(12, 3);
  Event_ShowMessageAndWait(0x400c, 0);
  Camera_SetSpeedParameters(0x33333, 0x6666);
  ShrineOfTheSeaGod_CamMove(420 << 16, -1, 200 << 16, 1);
  ShrineOfTheSeaGod_ObjSpeed(12, 0x19999, 0xcccc);
  ShrineOfTheSeaGod_ObjWalk(12, 346, 186);
  ShrineOfTheSeaGod_ObjWalk(12, 374, 192);
  ShrineOfTheSeaGod_ObjWalk(12, 392, 224);
  ShrineOfTheSeaGod_ObjWalk(12, 394, 246);
  ShrineOfTheSeaGod_ObjWalk(12, 420, 268);
  ShrineOfTheSeaGod_ObjWalk(12, 456, 268);
  ShrineOfTheSeaGod_ObjWalk(12, 488, 240);
  ShrineOfTheSeaGod_ObjWalk(12, 488, 226);
  Actor_SetHorizontalPositionWithTerrain(12, 0, 0);
  Camera_SetSpeedParameters(0x4cccc, 0x9999);
  Camera_MoveWithinMapBounds(264 << 16, -1, 320 << 16, 1);
  Func_02003ac8();
  Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_WEST, 0);
  Actor_ArmCallback(14, 0, 20);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 3);
  Motion_SetModeAndWaitAnimation(14, 3);
  Actor_SetModeById(14, 2);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  if (leader != NULL)
    Actor_ResetAndSetPosition(14, ((s16 *)&leader->x)[1],
                                     ((s16 *)&leader->z)[1]);
  Actor_WaitForMoveById(14);
  Actor_SetHorizontalPositionWithTerrain(14, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(13, 0, 0);
  GameFlag_SetBit(0x872);
  Func_020039e0();
}
