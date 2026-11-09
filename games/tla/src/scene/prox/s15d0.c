#include "../../../../games/tla/src/scene/prox/local.h"
#include "actor.h"
#include "event_inline.h"

s32 Func_02004464();
s32 Func_0200446c();

void Func_020015d0(void) {
  struct Actor *object;

  Func_02004464();
  Event_ResolvePendingActions(0);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x20000, 0x10000);
  Actor_SetSpeed(20, 0x20000, 0x10000);
  object = Actor_Get(20);
  object->terrainId = 1;
  Actor_SetSpritePriority(20, 3);
  object = Actor_Get(gMainRoutineWork.currentOwner);
  object->actionFlags &= 0xfe;
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 0x198, 0x146);
  Event_Wait(1);
  object = Actor_Get(gMainRoutineWork.currentOwner);
  object->actionFlags |= 1;
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0xc000, 0);
  Sound_Play(0xa1);
  object = (struct Actor *)((s32)Func_080d4568());
  object->y += 0x20000;
  object = Actor_Get(20);
  object->actionFlags &= 0xfe;
  Actor_SetPositionAndReset(20, 0x198, 0xc8);
  Event_Wait(1);
  object = Actor_Get(20);
  object->actionFlags |= 1;
  Actor_SetHorizontalPositionWithTerrain(20, 0, 0);
  Sound_Play(0xb0);
  MapGrid_SetShake(0x10000, 0x20000, 0x10000);
  Event_Wait(30);
  MapGrid_SetShake(-1, -1, 0xe666);
  Sound_Play(0x7f);
  Event_Wait(30);
  Sound_Play(0x92);
  Event_Wait(20);
  Func_080d47b4(gMainRoutineWork.currentOwner, 0x105, 40);
  Actor_SetSpritePriority(20, 2);
  object = Actor_Get(20);
  object->priorityFlags |= 1;
  MapGrid_CopyCellsTo(0x48, 3, 0x45, 3, 3, 3);
  MapGrid_CopyCellsTo(0x30, 0x24, 0x45, 8, 3, 2);
  MapGrid_CopyCellAttributes(0x48, 0xd, 1, 3, 0x46, 0x14);
  GameFlag_SetBit(0xa2d);
  Func_0200446c();
}
