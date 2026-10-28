#include "types.h"
#include "system.h"
#include "actor.h"
#include "fx_scene.h"
#include "ram_buffer.h"
#include "macros.h"

struct ResourceEntry {
  u8 unk00[0x10];
  u8 vramBlock;
};

void FieldEffect_RunPly(void);
void BattleEffect_StopMainActor();
void BattleEffect_InitializeSharedScene(void);
void BattleEffect_PrepareBufferInterpolation(void);
struct Actor *Actor_Spawn(s32 kind, s32 x, s32 y, s32 z);
struct ResourceEntry *Sprite_ShareVramCacheEntry(void *sprite,
                                                  struct ResourceEntry *last);
void Animation_ApplyChildValuesFar(struct Actor *object, u32 value);
void Motion_SetTargetPositionFromMagnitudeAngle(struct Actor *object,
                                                s32 magnitude, s32 angle);
void Actor_Destroy(struct Actor *object);
s32 Resource_ResetEntry(u32 entry);
void BattleEffect_UpdateShrinkingOrbitObject(struct Actor *particle);
void Func_080de8d0(struct Actor *object);

/// Effect 3 cast on a target: runs effect 3, then stops the caster's hook.
void FieldEffect_RunPlyOnTarget(void) {
  FieldEffect_RunPly();
  BattleEffect_StopMainActor();
}

/// Eight particles spiral in on the main object, one a frame; then a centre
/// object flashes for 60 frames, emits sparks and flies off.
void FieldEffect_RunPly(void) {
  struct BattleFxScene *scene = Ram_HeapSlots->effectWork;
  struct Actor *target = scene->mainObject;
  struct Actor *object;
  struct ResourceEntry *last;
  u8 marker;
  s32 i;

  BattleEffect_InitializeSharedScene();

  last = 0;
  for (i = 0; i < 8; i++) {
    object = Actor_Spawn(296, target->x, target->y + 0x200000, target->z);
    if (object != 0) {
      object->scaleY = 0xb333;
      object->scaleX = 0xb333;
      object->update = (void (*)(void))BattleEffect_UpdateShrinkingOrbitObject;
      object->action = 120;
      object->unk66 = i << 13;
      object->motionFlags = 4;
      last = Sprite_ShareVramCacheEntry(object->animation, last);
    }
    wait(1);
  }

  marker = last->vramBlock;
  Sound_PlayFar(130);
  wait(110);

  target = Actor_Spawn(296, 0, 0, 0);
  object = target;
  if (target != 0) {
    target->scaleY = 0xb333;
    target->scaleX = 0xb333;
    SET_POSITION(target, scene->x, scene->y + 0x100000, scene->z);
    target->motionFlags = 4;
    Animation_ApplyChildValuesFar(target, 7);
  }

  Sound_PlayFar(131);
  wait(12);
  if (target != 0) {
    for (i = 0; i < 30; i++) {
      /* FAKEMATCH: the native keeps object as a second register copy of the
       * centre object (r5 beside r6); this redundant reassignment stops gcse
       * copy-propagating object = target away, as in ☀️'s FieldEffect_RunPly. */
      target = object;
      if (i & 3)
        Animation_ApplyChildValuesFar(object, 9);
      else
        Animation_ApplyChildValuesFar(object, 10);
      wait(2);
    }
  }

  Animation_ApplyChildValuesFar(object, 0);
  Sound_PlayFar(84);
  if (object != 0) {
    target->update = (void (*)(void))Func_080de8d0;
    target->action = 0;
    if (scene->enabled != 0)
      wait(128);
    else
      wait(192);
  }

  if (target != 0) {
    target->action = -1;
    target->speedLimit = 0x50000;
    target->acceleration = 0x6666;
    target->actionFlags = 0;
    Motion_SetTargetPositionFromMagnitudeAngle(target, 0xc00000, 0xe800);
    Actor_CommitPosition(target);
    Actor_Destroy(target);
  }
  if (marker != 0x60)
    Resource_ResetEntry(marker);
  if (scene->finishCallback != 0)
    scene->finishCallback();
  BattleEffect_PrepareBufferInterpolation();
}
