#include "tundaria_tower.h"

extern const u8 gData_02003370[];

void BattleEffect_StartBufferInterpolation(s32 frames);
void Sound_Play(s32 cue);
void Func_02002f90(struct Actor *object, const void *script);
void Event_Wait(s32 frames);
void Actor_WaitForMoveById(s32 actor);
void Actor_WatchAndSetScript(s32 actor, s32 target);

/// Actor 8 is placed and given its script while the screen flashes, and three
/// map cells change.
void Func_02000964(void) {
  struct Actor *object = Actor_GetById(8);

  Func_02003048();
  Event_ResolvePendingActions(0);
  TundariaTower_FadeFrom(0x10000, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x7fff, 0);
  BattleEffect_StartBufferInterpolation(2);
  Sound_Play(132);
  TundariaTower_SetWork(0x50000, 0x70000, 0x10000);
  TundariaTower_SetWork(-1, -1, 0xe666);
  Event_Wait(2);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  BattleEffect_StartBufferInterpolation(2);
  object->actionFlags = 0;
  object->update = 0;
  TundariaTower_ObjPlace(8, 424, 536);
  Func_02002f90(object, gData_02003370);
  Actor_WaitForMoveById(8);
  GameFlag_SetBit(0x200);
  Actor_WatchAndSetScript(8, gMainRoutineWork.currentOwner);
  MapCell_SetAttributes(0, 21, 33, 0x3200);
  MapCell_SetAttributes(0, 22, 33, 0x3200);
  MapCell_SetAttributes(0, 26, 33, 0xff00);
  Func_02003050();
}
