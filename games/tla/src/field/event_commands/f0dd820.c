#include "types.h"
#include "system.h"
#include "ram_buffer.h"
#include "field_psynergy_work.h"
#include "actor.h"
#include "display_transition.h"
#include "fixed_math.h"

// The field event work's Reveal state (eventWork + 0xca4).
struct RevealEvent {
  u8 unk000[0xca4];
  s8 active;   // Reveal is showing
  u8 markers;  // the nearby markers are up
  s16 frames;  // how long it lasts
  s16 centreX; // pixels
  s16 centreY;
};

void BattleEffect_RunBusyWaitDelay(s32 value);
void BattleEffect_ApplyColorToSourceBuffer(u32 value, s32 enabled);
void BattleEffect_ApplyColorToTargetBuffer(u32 value, s32 enabled);
void BattleEffect_StartBufferInterpolation(s32 frames);
s32 Func_080ce31c(s32 kind);
void *Func_080ce458(u32 kind, s32 entry, s32 arg);
#include "event_work.h"
void Func_080cf554(void);
#include "task.h"
void Func_080dd754(void);

#define FIELD_FX_REVEAL 8
#define REVEAL_EVENT_START 0x50000005 // the Reveal event, stage 5
#define REVEAL_FRAMES 600
#define SE_REVEAL 131

// Reveal: open a circle of the transition on the caster, wash the screen
// plain, run the Reveal event that matches, then show the markers nearby
// and watch how far the leader walks.
void Func_080dd820(void) {
  struct BattleFxScene *scene = Ram_HeapSlots->effectWork;
  struct RevealEvent *event;
  struct ScreenTransitionState *transition;
  struct Actor *caster;
  void *pending;
  s32 frame;

  BattleEffect_RunBusyWaitDelay(6);
  event = Ram_HeapSlots->eventWork;
  ScreenTransition_InitializeBattleEffectState(FIELD_FX_REVEAL);
  transition = Ram_HeapSlots->screenTransition;
  caster = scene->mainObject;
  transition->centreX = caster->x;
  transition->centreY = caster->z - caster->y;
  BattleEffect_ApplyColorToSourceBuffer(0x10000, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10001, 1);
  BattleEffect_StartBufferInterpolation(1);
  event->active = 0;
  wait(1);

  pending = Func_080ce458(REVEAL_EVENT_START, FIELD_FX_REVEAL,
                          Func_080ce31c(FIELD_FX_REVEAL));
  if (pending != 0)
    Field_RunPsynergyEvent(pending, scene->firstObjectId,
                           scene->secondObjectId);
  // Leave the centre (in pixels) and the time in the event work, and put
  // up the markers nearby.
  // The cue and the active flag in a once-only block keep active's store
  // before the centre's reload; as plain statements, 4 of 137 instructions
  // differ in tla-en.
  do {
    Sound_PlayFar(SE_REVEAL);
    event->active = 1;
  } while (0);
  event->centreX = transition->centreX / Q16_ONE;
  event->centreY = transition->centreY / Q16_ONE;
  event->frames = REVEAL_FRAMES;
  event->markers = 1;
  Func_080cf554();
  frame = 0;
  do {
    wait(1);
    transition->value = frame;
    frame++;
  } while (frame <= 18);
  if ((s8)scene->mode == 0)
    Task_Add(Func_080dd754, 0x480);
}

void Func_080dd950(void) {
  struct BattleFxScene *scene = Ram_HeapSlots->effectWork;
  struct ScreenTransitionState *transition = Ram_HeapSlots->screenTransition;
  struct RevealEvent *event = Ram_HeapSlots->eventWork;
  void *pending;

  if (event->active != 0) {
    Sound_PlayFar(167);
    Task_Remove(Func_080dd754);
    event->active = 0;
    event->frames = 0;
    BattleEffect_RunBusyWaitDelay(0);
    BattleEffect_ApplyColorToTargetBuffer(0x10000, 1);
    BattleEffect_StartBufferInterpolation(1);
    BattleEffect_ApplyColorToSourceBuffer(0, 0);
    BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
    BattleEffect_StartBufferInterpolation(30);
    wait(1);
    pending = Func_080ce458(0x40000005, FIELD_FX_REVEAL,
                            Func_080ce31c(FIELD_FX_REVEAL));
    if (pending != NULL)
      Field_RunPsynergyEvent(pending, scene->firstObjectId,
                             scene->secondObjectId);
    if (scene->childOption == 0) {
      transition->nextMode = 0;
      transition->duration = 1;
      transition->step = 1;
      wait(10);
    }
  }
}

#include "edition.h"
#include "actor_view.h"

struct BurstParticleVector {
  s32 values[3];
};

extern const s32 gData_080f0ee8[];

void Func_080ddbd8(void);
void Vector_AddPolarOffset(s32 radius, s32 angle, s32 *position);
struct Actor *Actor_Spawn(s32 kind, s32 x, s32 y, s32 z);
#include "objdisp.h"
void BattleEffect_PrepareBufferInterpolation(void);
// Far call to the target classifier Func_080237c8.
s32 Func_08020330(struct Actor *object);
void Func_080daecc(s32 objectId, s32 mode);

#define BURST_FALL_OBJECT 281 ///< the object that falls round the target

// The two kinds of target that do not sink into the ground.
#define TARGET_KIND_SCATTERS 372 ///< scatters five falling pieces instead
#define TARGET_KIND_STAYS 394

/// Animation 12 on a target: the burst of particles, then the target sinks
/// to the underfoot priority. A target of kind 372 instead throws five pieces
/// round the scene's position; one of kind 394 only plays the cue (sound 126).
void Func_080dda30(s32 unused) {
  struct BattleFxScene *scene = Ram_HeapSlots->effectWork;
  struct Actor *target = scene->child;
  s32 kind = 0;

  if (target == 0)
    return;

  Func_080ddbd8();
  if (Func_08020330(target) == TARGET_KIND_SCATTERS)
    kind = 1;
  if (Func_08020330(target) == TARGET_KIND_STAYS)
    kind = 2;

  if (kind == 0) {
    Actor_SetMode(target, 2);
    Actor_SetSingleChildFlagsFar(target, 0);
    target->collisionFlags = 0;
    target->priorityFlags |= 2;
    wait(10);
    Sound_PlayFar(126);
    wait(40);
  } else if (kind == 1) {
    struct BurstParticleVector position;
    struct BurstParticleVector *p = &position;
    s32 n = 4;

    do {
      struct Actor *piece;
      s32 distance;

      p->values[0] = scene->x;
      p->values[2] = scene->z;
      distance = Random16() * 6 + 0x40000;
      Vector_AddPolarOffset(distance, Random16(), p->values);
      p->values[1] = scene->y;
      piece = Actor_Spawn(BURST_FALL_OBJECT, p->values[0], p->values[1],
                          p->values[2]);
      if (piece != 0) {
        Actor_StartScriptFar(piece, gData_080f0ee8);
        piece->motionFlags = 2;
      }
      wait((((u32)Random16() * 2) >> 16) + 2);
      n--;
    } while (n >= 0);
    Func_080daecc(scene->secondObjectId, 1);
  } else {
    wait(10);
    Sound_PlayFar(126);
  }
  BattleEffect_PrepareBufferInterpolation();
}

void FieldEffect_RunGrowth(void) {
  u8 *state = Ram_HeapSlots->effectWork;
  struct BurstParticleVector position;
  struct BurstParticleVector *p;
  s32 entryCount;

  Func_080ddbd8();
  Sound_PlayFar(134);
  p = &position;
  entryCount = 4;
  do {
    void *object;
    s32 randomValue;

    p->values[0] = *(s32 *)(state + 4);
    p->values[2] = *(s32 *)(state + 12);
    randomValue = (Random16() * 6) + 0x40000;
    Vector_AddPolarOffset(randomValue, Random16(), p->values);
    p->values[1] = *(s32 *)(state + 8);
    object = Actor_Spawn(0x119, p->values[0], p->values[1], p->values[2]);
    if (object != 0) {
      Actor_StartScriptFar(object, gData_080f0ee8);
      *((u8 *)object + 0x55) = 2;
    }
    wait((((u32)Random16() * 2) >> 16) + 2);
    entryCount--;
  } while (entryCount >= 0);
  wait(30);
  BattleEffect_PrepareBufferInterpolation();
}

#include "object_effect_scripts.h"
#include "field_event.h"

struct BurstParticleVisual {
  u8 reserved00[5];
  u8 flagsALow : 5;
  u8 flip : 1;
  u8 flagsAHigh : 2;
  u8 reserved06;
  u8 flagsBLow : 6;
  u8 flagsBHigh : 2;
  union {
    struct {
      u16 shape : 10;
      u16 unused : 6;
    } bits;
    struct {
      u8 lo;
      u8 highLow : 4;
      u8 palette : 4;
    } bytes;
  } attributes;
};

// ⚓️'s sprite keeps its second part 28 bytes in.
struct BurstParticleVisualGroup {
  struct BurstParticleVisual primary;
  u8 reserved0c[16];
  struct BurstParticleVisual child;
};

void BattleEffect_InitializeSharedScene(void);

void Func_080ddbd8(void) {
  struct BattleFxScene *work;
  struct Actor *user;
  s32 pos[3];
  s32 n;
  s32 *p;

  work = Ram_HeapSlots->effectWork;
  user = work->mainObject;
  BattleEffect_InitializeSharedScene();

  p = pos;
  for (n = 23; n >= 0; n--) {
    struct Actor *obj;
    struct BurstParticleVisual *visual;
    struct BurstParticleVisual *child;
    s32 dist;

    if (*(u16 *)&work->angle == 0x4000) {
      pos[0] = user->x;
      pos[1] = user->y + 0xa0000;
      pos[2] = user->z;
    } else if (*(u16 *)&work->angle == 0xc000) {
      pos[0] = user->x;
      pos[1] = user->y + 0x180000;
      pos[2] = user->z;
    } else {
      pos[0] = user->x;
      pos[1] = user->y + 0xa0000;
      pos[2] = user->z;
      Vector_AddPolarOffset(0xa0000, *(u16 *)&work->angle, pos);
    }

    obj = Actor_Spawn(672, pos[0], pos[1], pos[2]);
    visual = &((struct BurstParticleVisualGroup *)obj->sprite)->primary;
    child = &((struct BurstParticleVisualGroup *)obj->sprite)->child;
    child->flip = visual->flip;
    child->flagsBHigh = visual->flagsBHigh;
    child->flagsAHigh = visual->flagsAHigh;
    child->attributes.bits.shape = visual->attributes.bits.shape;
    child->attributes.bytes.palette = visual->attributes.bytes.palette;

    if (obj != 0) {
      ((s32 *)obj)[7] = 0xb333;
      ((s32 *)obj)[6] = 0xb333;
      obj->acceleration = 0x18000;
      obj->speedLimit = 0x18000;
      obj->motionFlags = 0;
      Animation_ApplyChildValuesFar(obj, 11);
      Actor_SetMode(obj, 7);
      Actor_StartScriptFar(obj, gBattleEffect_SharedParticleScript + 1);

      Actor_SetSingleChildFlagsFar(obj, 1);

      p[0] = work->x;
      p[1] = work->y;
      p[2] = work->z;
      if (*(u16 *)&work->angle == 0xc000)
        Vector_AddPolarOffset(0xe0000, *(u16 *)&work->angle, p);
      dist = Random16() * 6 + 0x40000;
      Vector_AddPolarOffset(dist, Random16(), p);
      Actor_SetTargetPosition(obj, p[0], p[1], p[2]);
    }
    Sound_PlayFar(131);
    wait(2);
  }
  wait(8);
}

struct BattleEffectScene {
  u8 reserved00[4];
  s32 x;
  s32 y;
  s32 z;
  void *mainObject;
  void *child;
};

struct EffectChild {
  u8 reserved00[12];
  s32 y;
  u8 reserved10[0x55 - 0x10];
  u8 flag;
  u8 reserved56[0x6c - 0x56];
  void (*callback)(void);
};

extern const s32 gData_080f0e60[];

struct Actor *BattleEffect_SpawnItemBreakMode3(s32 x, s32 y, s32 z, s32 angle);
void Motion_SetTargetPositionFromMagnitudeAngle(struct Actor *object,
                                                s32 magnitude, s32 angle);
void Actor_ApplyRandomChildValues(void);
void UpdateRisingParticleBurst(void *effect);

void FieldEffect_RunLiftOnTarget(void) {
  struct BattleEffectScene *scene;
  void *mainObject;
  void *child;
  void *anchors[2];
  s32 position[3];
  s32 x, y, z;
  s32 index;
  s32 startY;

  scene = Ram_HeapSlots->effectWork;
  child = scene->child;
  mainObject = scene->mainObject;
  if (child == 0)
    return;

  BattleEffect_InitializeSharedScene();
  *(void **)((u8 *)mainObject + 0x68) = child;
  Actor_StartScriptFar(mainObject, gData_080f0e60);

  x = scene->x;
  position[0] = x;
  y = scene->y + 0x100000;
  position[1] = y;
  z = scene->z;
  position[2] = z;
  anchors[0] = BattleEffect_SpawnItemBreakMode3(x + 0x200000, y, z, 0x8000);
  anchors[1] = BattleEffect_SpawnItemBreakMode3(position[0] - 0x200000,
                                                position[1], position[2], 0);

  wait(15);
  for (index = 0; index < 2; index++) {
    void *anchor = anchors[index];
    if (anchor != 0)
      Motion_SetTargetPositionFromMagnitudeAngle(anchor, 0xe0000,
                                                 *(u16 *)((u8 *)anchor + 6));
  }

  Actor_CommitPosition(anchors[0]);
  ((struct EffectChild *)child)->callback = Actor_ApplyRandomChildValues;
  Sound_PlayFar(130);
  ((struct EffectChild *)child)->flag = 4;

  startY = ((struct EffectChild *)child)->y;
  if (anchors[0] != 0 && anchors[1] != 0 && startY <= startY + 0x200000) {
    do {
      *(s32 *)((u8 *)anchors[0] + 0xc) += 0x4000;
      *(s32 *)((u8 *)anchors[1] + 0xc) += 0x4000;
      ((struct EffectChild *)child)->y += 0x4000;
      wait(1);
    } while (((struct EffectChild *)child)->y <= startY + 0x200000);
  }

  UpdateRisingParticleBurst(anchors[0]);
  UpdateRisingParticleBurst(anchors[1]);
  BattleEffect_PrepareBufferInterpolation();
}

struct ScatterPosition {
  s32 x;
  s32 y;
  s32 z;
};

extern const s32 gData_080f0e78[];

void Actor_Destroy(struct Actor *object);

void Func_080ddea8(void) {
  struct BattleFxScene *work;
  struct Actor *obj;
  struct Actor *pair[2];
  struct ScatterPosition pos;
  struct Actor **walk;
  s32 n;
  s32 dist;

  work = Ram_HeapSlots->effectWork;
  BattleEffect_InitializeSharedScene();
  pos.x = work->x;
  pos.y = work->y + 0x100000;
  pos.z = work->z;
  pair[0] =
      BattleEffect_SpawnItemBreakMode3(pos.x + 0x200000, pos.y, pos.z, 0x8000);
  pair[1] = BattleEffect_SpawnItemBreakMode3(pos.x - 0x200000, pos.y, pos.z, 0);

  wait(15);
  walk = pair;
  for (n = 0; n < 2; n++) {
    obj = *walk++;
    if (obj != 0)
      Motion_SetTargetPositionFromMagnitudeAngle(obj, 0x180000, obj->facing);
  }

  Actor_CommitPosition(pair[0]);
  Sound_PlayFar(134);
  for (n = 0; n < 24; n++) {
    pos.x = work->x;
    pos.y = work->y + 0x100000;
    pos.z = work->z;
    obj = Actor_Spawn(673, pos.x, pos.y, pos.z);
    if (obj != 0) {
      Actor_StartScriptFar(obj, gData_080f0e78);
      obj->speedLimit = Random16() + 0x20000;
      obj->acceleration = 0x20000;
      obj->motionFlags = 0;
      dist = Random16() * 24 + 0x80000;
      Motion_SetTargetPositionFromMagnitudeAngle(obj, dist, Random16());
    }
  }

  Actor_Destroy(pair[0]);
  Actor_Destroy(pair[1]);
  BattleEffect_PrepareBufferInterpolation();
}

#include "field_sprite.h"

/// Spawns object 278 and grows it from a quarter to full size, an eighth of a
/// turn a frame, then faces it to angle.
struct Actor *BattleEffect_SpawnItemBreakMode3(s32 x, s32 y, s32 z, s32 angle) {
  struct Actor *object;
  s32 scale;

  Sound_PlayFar(138);
  object = Actor_Spawn(278, x, y, z);
  if (object != NULL) {
    object->scaleX = object->scaleY = 0x4000;
    object->acceleration = object->speedLimit = 0x30000;
    ((struct FieldSprite *)object->sprite)->priority = 0;
    Actor_SetMode(object, 3);
    scale = object->scaleX;
    if (scale < Q16_ONE) {
      do {
        scale += 0x800;
        object->scaleY = scale;
        object->scaleX = scale;
        object->facing += 0x2000;
        wait(1);
        scale = object->scaleX;
      } while (scale <= 0xffff);
    }
    object->facing = angle;
  }
  return object;
}

struct EffectPosition {
  s32 x;
  s32 y;
  s32 z;
};

struct EffectTarget {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
};

struct EffectWork {
  u16 angle;
  u8 unk02[14];
  struct EffectTarget *target;
};

struct RadialBurstEffect {
  u8 unk00[12];
  s32 x;
  s32 z;
  s32 sourceX;
  s32 sourceZ;
  u8 unk1c[4];
  s32 velocityX;
  s32 velocityZ;
  u8 unk28[10];
  u16 scale;
  u8 unk34[12];
  s8 state;
  u8 unk41;
  u8 enabled;
};

void Camera_WorldToScreen(struct EffectPosition *value);
void BattleEffect_ClearOwnedSlot(struct RadialBurstEffect *effect);

void Func_080de060(struct RadialBurstEffect *effect) {
  struct EffectWork *work = Ram_HeapSlots->effectWork;
  struct EffectPosition value;
  s32 state;

again:
  state = effect->state;
  if (state == 0) {
    value.x = effect->sourceX;
    value.z = effect->sourceZ;
    Vector_AddPolarOffset(0x190000, (u16)Random16(), &value);
    effect->x = value.x;
    effect->z = value.z;
    effect->velocityZ = 0x30000;
    effect->velocityX = 0x30000;
    effect->enabled = 0;
    effect->state++;
    return;
  }

  if (state == 1) {
    if (FieldParticle_HasPendingTarget(effect) == 0) {
      effect->state++;
      goto again;
    }
    return;
  }

  if (state == 2) {
    struct EffectTarget *target = work->target;

    value.x = target->x;
    value.y = target->y + 0x100000;
    value.z = target->z;
    Vector_AddPolarOffset(0x80000, work->angle, &value);
    Camera_WorldToScreen(&value);
    Vector_AddPolarOffset(0x40000, Random16(), &value);
    effect->x = value.x;
    effect->z = value.z;
    effect->scale = 0x800;
    effect->enabled = 1;
    effect->state++;
    return;
  }

  if (state == 3 && FieldParticle_HasPendingTarget(effect) == 0)
    BattleEffect_ClearOwnedSlot(effect);
}

struct ImpactVector {
  s32 x;
  s32 y;
  s32 z;
};

struct ParticleTimer {
  u8 unk00[0x5e];
  u16 timer;
};

s32 Func_080de154(struct Actor *src) {
  struct ImpactVector pos;
  struct Actor *obj;
  struct Actor *frag;
  s32 i;

  Sound_PlayFar(134);
  pos.x = src->x;
  pos.y = src->y;
  pos.z = src->z;
  obj = Actor_Spawn(671, pos.x, pos.y - (32 << 16), pos.z);
  if (obj != NULL) {
    obj->motionFlags = 0;
    ((struct ParticleTimer *)obj)->timer = 20;
    Actor_StartScriptFar(obj, gBattleEffect_SharedParticleScript);
  }

  for (i = 0; i < 12; i++) {
    frag = Actor_Spawn(673, pos.x, pos.y, pos.z);
    if (frag != NULL) {
      Actor_StartScriptFar(frag, gData_080f0e78);
      frag->speedLimit = Random16() + (1 << 16);
      frag->acceleration = 1 << 16;
      frag->motionFlags = 0;
      Motion_SetTargetPositionFromMagnitudeAngle(
          frag, Random16() * 24 + (8 << 16), Random16());
    }
  }
  return 0;
}

struct BattleEffectArgument;

void Func_080de21c(struct BattleEffectArgument *effect);

/// Runs field Psynergy animation 4; the target is unused.
void FieldEffect_RunForceOnTarget(struct BattleEffectArgument *effect) {
  Func_080de21c(effect);
}
