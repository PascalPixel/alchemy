#include "shrine_of_the_sea_god.h"

void Func_02002384(void) {
  u8 off = 0;

  Func_020039d8();
  Event_ResolvePendingActions(0);
  Func_080d4568()->motionFlags = off;
  Camera_MoveWithinMapBounds(480 << 16, 32 << 16, 340 << 16, 1);
  Func_02003ac8();
  Actor_SetAngleToward(15, gMainRoutineWork.currentOwner, 0);
  Func_080d47b4(15, 0x100, 40);
  Actor_SetSpritePriority(15, 1);
  Actor_GetById(15)->motionFlags = off;
  ShrineOfTheSeaGod_ObjSpeed(15, 0x19999, 0xcccc);
  ShrineOfTheSeaGod_ObjWalk(15, 500, 344);
  ShrineOfTheSeaGod_ObjWalk(15, 580, 344);
  Actor_WatchAndSetScript(15, gMainRoutineWork.currentOwner);
  Event_Wait(20);
  GameFlag_SetBit(0x874);
  Func_020039e0();
}

void Func_02002430(void) {
  u8 off = 0;

  Func_020039d8();
  Event_ResolvePendingActions(0);
  Actor_Stop(15);
  Func_080d4568()->motionFlags = off;
  Camera_MoveWithinMapBounds(564 << 16, 32 << 16, 344 << 16, 1);
  Func_02003ac8();
  Actor_SetAngleToward(15, gMainRoutineWork.currentOwner, 0);
  Func_080d47b4(15, 0x100, 40);
  Actor_SetSpritePriority(15, 1);
  Actor_GetById(15)->motionFlags = off;
  ShrineOfTheSeaGod_ObjSpeed(15, 0x19999, 0xcccc);
  ShrineOfTheSeaGod_ObjWalk(15, 616, 376);
  ShrineOfTheSeaGod_ObjWalk(15, 616, 460);
  Actor_SetHorizontalPositionWithTerrain(15, 0, 0);
  Event_Wait(20);
  GameFlag_SetBit(0x875);
  Func_020039e0();
}
