#include "scene_tables.h"
#include "local.h"

void Func_02002834(s32 id);
void Func_020025a8(s32 id);
void BattleEffect_StartBufferInterpolation(s32 frames);
void Func_0802b450(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Camera_SetSpeedParameters(s32 a, s32 b);
void Func_02002a10(void);
void Func_02000b10(struct Actor *object);
void Func_02000b58(struct Actor *object);
void Func_02001d98(struct Actor *object);

/// Sets flag 0xa69 after four rounds of a random colour value, runs the
/// blend scripts, then moves the camera to object 15, starts its update and
/// plays cue 623 six times.
void Func_02001224(s32 unused, s32 id) {
  struct Actor *object = Actor_GetById(id);
  s32 i;

  Func_02002998();
  Event_ResolvePendingActions(0);
  object->update = (void (*)(void))Func_02000b10;
  wait(30);
  Func_02002834(8);
  Sound_Play(220);
  for (i = 3; i >= 0; i--) {
    BattleEffect_ApplyColorToTargetBuffer(Random16Far() & 0x7fff, 0);
    BattleEffect_StartBufferInterpolation(15);
    wait(10);
    BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
    BattleEffect_StartBufferInterpolation(10);
    wait(10);
  }
  BattleEffect_ApplyColorToTargetBuffer(0x7fff, 0);
  BattleEffect_StartBufferInterpolation(20);
  Event_Wait(20);
  GameFlag_SetBit(0xa69);
  Map_CopyCellAttributeRect(15, 48, 1, 1, 16, 48);
  Map_CopyCellAttributeRect(17, 50, 1, 1, 16, 50);
  object->update = (void (*)(void))Func_02000b58;
  Func_020025a8(8);
  Func_0802b450(64, 68, 5, 13, 14, 94);
  Map_CopyCellAttributeRect(64, 68, 5, 8, 14, 34);
  Event_Wait(30);
  Func_02002a20_(0x10000, 0);
  BattleEffect_StartBufferInterpolation(30);
  Event_Wait(15);
  Camera_SetSpeedParameters(0x10000, 0x2000);
  Camera_MoveWithinMapBounds(PIXELS(248), -PIXELS(32), PIXELS(760), 1);
  Func_02002a10();
  Actor_SetHorizontalPositionWithTerrain(15, PIXELS(264), PIXELS(792));
  Actor_SetSingleChildFlags(Actor_GetById(15), 0);
  Actor_GetById(15)->collisionFlags = 0;
  Actor_GetById(15)->priorityFlags |= 2;
  Actor_GetById(15)->update = (void (*)(void))Func_02001d98;
  Event_Wait(30);
  for (i = 5; i >= 0; i--) {
    Sound_Play(623);
    wait(10);
  }
  Event_Wait(30);
  Func_020029a0();
}
