#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "task.h"
#include "effect_sprite.h"

// Kind 24's work (battle_fx_work[0]), as Func_080e9af4's draft has it.
struct GlowWork {
  u8 unk0000[0x1000];
  struct EffectSprite sprite;
  s32 scale;
  s16 resource;
  u16 tile;
  u16 unk1030;
  u16 phase;
  s16 fading;
};

s32 Party_GetLeaderActor(void);
struct Actor *Actor_GetById(s32 id);
void Actor_SetPositionAndResetMotionFar(struct Actor *object, s32 x, s32 y,
                                        s32 z);
void wait(s32 frames);
s32 Resource_ResetEntry(u32 entry);
void HeapSlot_Release(s32 slot);
void Func_080e9940(void);

void Func_080e9db4(void) {
  struct GlowWork *work = (struct GlowWork *)Ram_HeapSlots->battleFxWork[0];
  struct Actor *object = Actor_GetById(Party_GetLeaderActor());
  s32 i;

  Task_Remove(Func_080e9940);
  Actor_SetPositionAndResetMotionFar(object, object->x, object->y, object->z);
  object->motionFlags = 3;
  gMainRoutineWork.renderMode = 0;
  for (i = 0; i < 60; i++) {
    if (object->y <= object->terrainHeight)
      break;
    wait(1);
  }
  Sound_PlayFar(132);
  wait(1);
  for (i = 0; i < 20; i++) {
    if (object->y <= object->terrainHeight)
      break;
    wait(1);
  }
  wait(1);
  for (i = 0; i < 20; i++) {
    if (object->y <= object->terrainHeight)
      break;
    wait(1);
  }
  wait(1);
  object->bounce = 0;
  object->velocityY = 0;
  object->y = object->terrainHeight;
  object->gravity = 0x10000;
  Resource_ResetEntry(work->resource);
  HeapSlot_Release(92);
}
