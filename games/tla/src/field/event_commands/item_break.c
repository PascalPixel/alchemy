#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "ram_buffer.h"
#include "field_event.h"

struct BattleFxScene {
  u8 unk00[16];
  void *volatile mainObject;
};

void FieldEffect_RunMove(void) {
  // Unused: gives the stack frame its 12 bytes.
  s32 work[3];
  struct BattleFxScene *scene;
  void *object;

  scene = Ram_HeapSlots->effectWork;
  object = scene->mainObject;

  BattleEffect_InitializeSharedScene();
  object = BattleEffect_StartItemBreak(object);
  BattleEffect_SnapScaleToFull(object);
  if (object != 0) {
    Actor_SetMode(object, 4);
    wait(30);
  }
  BattleEffect_PrepareBufferInterpolation();
  UpdateRisingParticleBurst(object);
}

#include "objdisp.h"
#include "actor.h"
#include "fixed_math.h"

extern const s32 gData_080f0e78[];

struct Actor *Actor_Spawn(s32 kind, s32 x, s32 y, s32 z);
void Motion_SetTargetPositionFromMagnitudeAngle(struct Actor *object,
                                                s32 magnitude, s32 angle);
void BattleEffect_UpdateItemBreakFragment(void);

#define SE_ITEM_BREAK 138

/// The Broken Seal's BattleEffect_StartItemBreak with this game's objects
/// 278 and 673: spawns the breaking item above object and eight fragments
/// that scatter from it.
void *BattleEffect_StartItemBreak(void *object) {
  struct Actor *source = object;
  struct Actor *parent;
  struct Actor *child;
  s32 angle;
  s32 radius;
  s32 speed;
  s32 fragmentIndex;
  u32 spread;
  u32 jitter;

  angle = (source->facing + 0x2000) & 0xc000;
  parent = Actor_Spawn(278, source->x, source->y + (16 << 16), source->z);
  if (parent == 0)
    return 0;
  parent->scaleY = 0x4000;
  parent->scaleX = 0x4000;
  parent->update = BattleEffect_UpdateItemBreakFragment;
  parent->speedLimit = 2 << 16;
  parent->acceleration = 2 << 16;
  parent->motionFlags = 0;
  Actor_SetMode(parent, 3);
  Motion_SetTargetPositionFromMagnitudeAngle(parent, 16 << 16, angle);

  for (fragmentIndex = 7; fragmentIndex >= 0; fragmentIndex--) {
    child = Actor_Spawn(673, source->x, source->y + (16 << 16), source->z);
    if (child != 0) {
      Actor_StartScriptFar((struct ActorView *)child, gData_080f0e78);
      speed = Random16() + Q16_ONE;
      child->acceleration = 1 << 16;
      child->speedLimit = speed;
      child->motionFlags = 2;
      child->gravity = 0x51e;
      spread = Random16();
      child->velocityY = spread - Random16();
      radius = Random16() * 24 + (8 << 16);
      jitter = Random16();
      Motion_SetTargetPositionFromMagnitudeAngle(
          child, radius, ((jitter - Random16()) >> 3) + source->facing);
    }
  }
  Sound_PlayFar(SE_ITEM_BREAK);
  return parent;
}

/// Raises the object's scale in sixteenths to full size and commits its
/// position.
void BattleEffect_SnapScaleToFull(struct Actor *object) {
  s32 next;
  s32 scale;

  if (object != NULL) {
    scale = object->scaleX;
    if (scale <= 0xffff) {
      do {
        next = scale + Q16_ONE / 16;
        scale = next;
      } while (next <= 0xffff);
      object->scaleX = next;
      object->scaleY = next;
    }
    Actor_CommitPosition(object);
  }
}

/// The object spins as it rises for 31 frames, shrinks, and bursts into 8
/// particles (object 673).
// As ☀️'s UpdateRisingParticleBurst.
void UpdateRisingParticleBurst(void *object) {
  struct Actor *src = object;
  struct Actor *child;
  s32 i;
  s32 dist;
  u32 spread;

  Sound_PlayFar(154);
  for (i = 30; i >= 0; i--) {
    src->y += 1 << 16;
    src->facing += 0x2000; // 1/8 turn
    src->scaleX += -Q16_ONE / 32;
    src->scaleY += -Q16_ONE / 32;
    wait(1);
  }
  for (i = 7; i >= 0; i--) {
    child = Actor_Spawn(673, src->x, src->y, src->z);
    if (child != 0) {
      s32 speed;
      // One 1.0 for both uses, set before the block's calls; a literal at
      // each use differs.
      s32 unit = 0x10000;

      Actor_StartScriptFar((struct ActorView *)child, gData_080f0e78);
      speed = Random16();
      child->acceleration = unit;
      child->speedLimit = speed + unit;
      child->motionFlags = 2;
      child->gravity = 0xa3d;
      spread = Random16();
      child->velocityY = spread - Random16();
      dist = Random16() * 24;
      dist += 8 << 16;
      Motion_SetTargetPositionFromMagnitudeAngle(child, dist, Random16());
    }
  }
  Sound_PlayFar(131);
  Actor_Destroy(src);
}

/// Burns 63 loop iterations and returns; callers pass an argument it ignores.
void BattleEffect_RunBusyWaitDelay(void) {
  s32 i = 63;

loop:
  i--;
  if (i >= 0)
    goto loop;
}

#include "main_routine_work.h"
#include "iwram_call.h"

/// The Broken Seal's FieldEffect_WatchLeaderDistance with this game's
/// offsets: counts the timer down and, once the leader is 60 pixels from the
/// centre or the timer has run out, requests the effect's end.
void Func_080dd754(void) {
  u8 *work;
  struct Actor *actor;
  s16 *timer;
  s32 dx;
  s32 dz;
  struct MainRoutineWork *party;

  work = Ram_HeapSlots->eventWork;
  party = &gMainRoutineWork;
  actor = ObjectTable_Get(party->currentOwner);
  if (*(s8 *)(work + 0xca5) != 0) {
    timer = (s16 *)(work + 0xca6);
    if (*timer != 0)
      (*timer)--;
  }

  dx = Iwram_MulQ16(*(s16 *)(work + 0xca8) - actor->x / Q16_ONE, 0xd105);
  dz = *(s16 *)(work + 0xcaa) - (actor->z - actor->y) / Q16_ONE;
  if (dx * dx + dz * dz >= 3600 || *(s16 *)(work + 0xca6) == 0) {
    u16 *slot = (u16 *)(work + 358);
    u32 request = 0x2090;

    *slot = request;
  }
}
