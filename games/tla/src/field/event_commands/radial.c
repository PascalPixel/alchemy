// ---- games/tla/src/field/event_commands/radial.c ----
#include "iwram_call.h"
#include "objdisp.h"
#include "system.h"
#include "fixed_math.h"
#include "types.h"
#include "motobj.h"
#include "scene_tables.h"
#include "field_effect.h"

extern const u8 gBattleEffect_SharedParticleScript[];

struct OrbitEffect {
  u8 unk00[6];
  u16 angle;
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[20];
  s32 scale;
  u8 unk2c[28];
  s32 speed;
  u8 unk4c[24];
  u16 timer;
  s16 pause;
  struct OrbitEffect *anchor;
  void (*update)(struct OrbitEffect *);
};

void Effect_AdvanceMotion(struct FieldEffect *effect);
void Func_080df8e0(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);

void BattleEffect_SpawnRadialParticleRing(struct MotionObject *origin) {
  struct EffectOptions options;
  s32 velocity[3];
  s32 sine;
  s32 angle;
  u32 i;

  options.palette = 0;
  options.update = Effect_AdvanceMotion;
  options.startScaleX = 0xCCCC;
  options.startScaleY = 0xCCCC;
  i = 0;
  do {
    angle = i << 12;
    velocity[0] = Trig_Cos(angle) * 3 / 2;
    velocity[1] = 0;
    sine = Trig_Sin(angle);
    velocity[2] = sine;
    Func_080df8e0(origin->x, origin->y, origin->z, velocity[0], velocity[1],
                  sine, 0x01090001, &options);
    i++;
  } while (i <= 16);
}

void BattleEffect_WanderAroundAnchor(struct OrbitEffect *effect) {
  s32 radius;
  s32 angle;
  s32 dx;
  s32 dz;

  radius = Random16() + 0x20000;
  angle = effect->angle;
  dx = Iwram_MulQ16(radius, Trig_Cos(angle));
  dz = Iwram_MulQ16(radius, Trig_Sin(angle));
  effect->x += dx;
  effect->z += dz;
  effect->angle += 0xfff0;
  if (effect->pause != 0) {
    effect->pause--;
    effect->angle += 0x800;
  } else if ((Random16() << 5) >> 16 == 0) {
    effect->pause = ((Random16() << 4) >> 16) + 8;
  }
  if (++effect->timer == 101)
    Actor_StartScriptFar((struct Actor *)effect,
                         (u32)gBattleEffect_SharedParticleScript);
}

/// Per-frame update of a Whirlwind particle: circles the anchor at 8 pixels,
/// a sixteenth of a turn a frame; after 121 frames it starts to wander
/// (BattleEffect_WanderAroundAnchor) with upward speed and gravity. As The
/// Broken Seal's FieldEffect_CircleAnchor.
void BattleEffect_CircleAnchor(struct OrbitEffect *effect) {
  struct OrbitEffect *anchor;
  s32 angle;
  s32 radius;
  s32 dx;
  s32 dz;

  anchor = effect->anchor;
  radius = 0x80000;
  angle = effect->angle;
  dx = Iwram_MulQ16(radius, Trig_Cos(angle));
  dz = Iwram_MulQ16(radius, Trig_Sin(angle));
  effect->x = anchor->x + dx;
  effect->z = anchor->z + dz;
  effect->angle += 0x800;
  if (++effect->timer == 121) {
    effect->update = BattleEffect_WanderAroundAnchor;
    effect->timer = 0;
    effect->pause = 0;
    effect->speed = 0x1999;
    effect->scale = 0x30000;
    effect->angle = Random16();
  }
}

#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "actor.h"

#define Actor_SetPositionAndResetMotionFar                                     \
  ((s32 (*)(void *, s32, s32, s32))Actor_SetPositionAndResetMotionFar)

/* VIEW(radial): routine FieldEffect_RunWhirlwind as s32 FieldEffect_RunWhirlwind(); the definition has void FieldEffect_RunWhirlwind(void). */
extern s32 FieldEffect_RunWhirlwind__radial() __asm__("FieldEffect_RunWhirlwind");

void BattleEffect_ShrinkObjectScaleUntilHalf(struct Actor *object) {
  s32 scale;

  scale = object->scaleX - 0x80;
  object->scaleY = scale;
  object->scaleX = scale;
  if (scale < 0x8000) {
    Actor_SetPositionAndResetMotionFar(object, 0, 0, 0);
    object->update = 0;
  }
}

void FieldEffect_RunWhirlwindOnTarget(void) { FieldEffect_RunWhirlwind__radial(); }
#undef Actor_SetPositionAndResetMotionFar

// ---- games/tla/src/field/event_commands/f0dfd00.c ----
#include "ram_buffer.h"
#include "field_psynergy_work.h"
#include "sprite.h"

struct EffectVector {
  s32 x;
  s32 y;
  s32 z;
};

// An orbiting mote: radial.c's OrbitEffect.
/* VIEW(f0dfd00): tag struct OrbitEffect as struct OrbitEffect__f0dfd00; radial.c has another definition. */
struct OrbitEffect__f0dfd00 {
  u8 unk00[6];
  u16 angle;
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[20];
  s32 scale;
  u8 unk2c[28];
  s32 speed;
  u8 unk4c[24];
  u16 timer;
  s16 pause;
  struct Actor *anchor;
  void (*update)(struct OrbitEffect__f0dfd00 *);
};

// ⚓️'s composite sprite, as far as this reads it.
struct OrbTargetSprite {
  u8 unk00[0x28];
  struct AnimationEntry *entries[4];
};

struct Actor *Actor_Spawn(s32 kind, s32 x, s32 y, s32 z);
void BattleEffect_InitializeSharedScene(void);
void Vector_AddPolarOffset(s32 magnitude, s32 angle, struct EffectVector *p);
void BattleEffect_ShrinkObjectScaleUntilHalf(struct Actor *object);
/* VIEW(f0dfd00): routine BattleEffect_CircleAnchor as void BattleEffect_CircleAnchor(struct OrbitEffect__f0dfd00 *mote); the definition has void BattleEffect_CircleAnchor(struct OrbitEffect *effect). */
extern void BattleEffect_CircleAnchor__f0dfd00(struct OrbitEffect__f0dfd00 *mote) __asm__("BattleEffect_CircleAnchor");
/* VIEW(f0dfd00): routine BattleEffect_WanderAroundAnchor as void BattleEffect_WanderAroundAnchor(struct OrbitEffect__f0dfd00 *mote); the definition has void BattleEffect_WanderAroundAnchor(struct OrbitEffect *effect). */
extern void BattleEffect_WanderAroundAnchor__f0dfd00(struct OrbitEffect__f0dfd00 *mote) __asm__("BattleEffect_WanderAroundAnchor");
/* VIEW(f0dfd00): routine BattleEffect_AgeAndCueAtEighty as void BattleEffect_AgeAndCueAtEighty(struct Actor *object); the definition has void BattleEffect_AgeAndCueAtEighty(struct TimedObject *object). */
extern void BattleEffect_AgeAndCueAtEighty__f0dfd00(struct Actor *object) __asm__("BattleEffect_AgeAndCueAtEighty");
/* VIEW(f0dfd00): routine BattleEffect_SetObjectUpdateMode as void BattleEffect_SetObjectUpdateMode(void *object, s32 mode); the definition has void BattleEffect_SetObjectUpdateMode(struct Actor *object, s32 mode). */
extern void BattleEffect_SetObjectUpdateMode__f0dfd00(void *object, s32 mode) __asm__("BattleEffect_SetObjectUpdateMode");
void BattleEffect_PrepareBufferInterpolation(void);
void Func_080db0b0(s32 objectId);
void Func_08020340(struct Actor *object);
void Actor_Destroy(struct Actor *object);

#define ORB_OBJECT 280
#define MOTE_OBJECT 308
#define SE_ORB_ARRIVES 207
#define SE_ORB_FADES 136

// The two targets ⚓️ treats apart, by their first animation.
#define TARGET_ANIMATION_A 372
#define TARGET_ANIMATION_B 373

enum OrbKind {
  ORB_PLAIN,    // the scene is enabled: the orb only comes and goes
  ORB_ORBITING, // motes circle the orb while the target shrinks
  ORB_WANDER,   // target 372: motes wander off, the target is handed on
  ORB_HANDOFF,  // target 373: the target is handed on
};

// Animation 14: carry an orb from the caster to the scene's position over
// ten frames, growing it from a quarter size, then play out the kind.
void FieldEffect_RunWhirlwind(void) {
  struct BattleFxScene *scene = Ram_HeapSlots->effectWork;
  struct Actor *caster = scene->mainObject;
  struct Actor *target = scene->child;
  struct OrbTargetSprite *sprite = target->sprite;
  struct EffectVector position;
  struct EffectVector from;
  struct EffectVector to;
  struct Actor *orb;
  struct OrbitEffect__f0dfd00 *mote;
  s32 kind = ORB_PLAIN;
  s32 step;
  s32 i;
  s32 frames = 11;

  if (scene->childOption != 0)
    return;
  if (scene->enabled == 0) {
    s32 animation;

    kind = ORB_ORBITING;
    animation = sprite->entries[0]->animId;
    if (animation == TARGET_ANIMATION_A)
      kind = ORB_WANDER;
    if (animation == TARGET_ANIMATION_B)
      kind = ORB_HANDOFF;
  }

  from.x = caster->x;
  from.y = caster->y;
  from.z = caster->z;
  to.x = scene->x;
  to.y = scene->y - (4 << 16);
  to.z = scene->z;
  orb = Actor_Spawn(ORB_OBJECT, 0, 0, 0);
  if (orb == 0)
    return;

  BattleEffect_InitializeSharedScene();
  Actor_SetMode(orb, 2);
  step = 0;
  do {
    s32 scale;

    orb->x = from.x + (to.x - from.x) * step / 10;
    orb->y = from.y + (to.y - from.y) * step / 10;
    orb->z = from.z + (to.z - from.z) * step / 10;
    scale = step * 0x10ccc / 10 + 0x4000; // from 0.25 to 1.3
    orb->scaleX = scale;
    orb->scaleY = scale;
    wait(1);
    step++;
  } while (step < frames);
  orb->scaleX = 0x1b333; // 1.7 wide
  orb->scaleY = 0x14ccc; // 1.3 high
  Sound_PlayFar(SE_ORB_ARRIVES);

  if (kind == ORB_PLAIN) {
    Actor_SetMode(orb, 1);
    wait(30);
    Sound_PlayFar(SE_ORB_FADES);
    Func_08020340(orb);
    Actor_Destroy(orb);
  } else if (kind == ORB_ORBITING) {
    // Sixteen motes, one every six frames, 4 pixels above the orb and 0.8
    // higher each, 3 to 8 pixels out, circle it while the target shrinks.
    wait(20);
    if (target != 0)
      target->update = (void (*)(void))BattleEffect_ShrinkObjectScaleUntilHalf;
    for (i = 0; i <= 15; i++) {
      s32 distance;

      position.x = orb->x;
      position.y = orb->y + i * 0xcccc + (4 << 16);
      position.z = orb->z;
      distance = Random16() * 5 + (3 << 16);
      Vector_AddPolarOffset(distance, Random16(), &position);
      mote = (struct OrbitEffect__f0dfd00 *)Actor_Spawn(MOTE_OBJECT, position.x,
                                               position.y, position.z);
      if (mote != 0) {
        mote->update = BattleEffect_CircleAnchor__f0dfd00;
        mote->anchor = orb;
        mote->timer = 0;
        mote->pause = 0;
        mote->angle = Random16();
      }
      wait(6);
    }
    wait(20);
    wait(120);
    Actor_SetMode(orb, 1);
    wait(30);
    Sound_PlayFar(SE_ORB_FADES);
    Func_08020340(orb);
    Actor_Destroy(orb);
  } else if (kind == ORB_WANDER || kind == ORB_HANDOFF) {
    wait(5);
    Actor_SetMode(orb, 1);
    if (kind == ORB_WANDER) {
      // Sixteen motes that wander about the orb, the first pausing longest.
      for (i = 0; i <= 15; i++) {
        s32 distance;

        position.x = orb->x;
        position.y = orb->y + i * 0xcccc + (4 << 16);
        position.z = orb->z;
        distance = Random16() * 5;
        distance += 3 << 16;
        Vector_AddPolarOffset(distance, Random16(), &position);
        mote = (struct OrbitEffect__f0dfd00 *)Actor_Spawn(MOTE_OBJECT, position.x,
                                                 position.y, position.z);
        if (mote != 0) {
          mote->update = BattleEffect_WanderAroundAnchor__f0dfd00;
          mote->timer = 20 + i * -2;
          mote->pause = 0;
          mote->speed = 0x1999;
          mote->scale = 3 << 16;
          mote->angle = Random16();
        }
      }
    }
    BattleEffect_SetObjectUpdateMode__f0dfd00(scene->mainObject, 0);
    BattleEffect_PrepareBufferInterpolation();
    ((struct OrbitEffect__f0dfd00 *)orb)->timer = 0;
    orb->update = (void (*)(void))BattleEffect_AgeAndCueAtEighty__f0dfd00;
    Func_080db0b0(scene->secondObjectId);
    Func_08020340(orb);
    Actor_Destroy(orb);
  }
  BattleEffect_PrepareBufferInterpolation();
}
#undef ORB_OBJECT
#undef MOTE_OBJECT
#undef SE_ORB_ARRIVES
#undef SE_ORB_FADES
#undef TARGET_ANIMATION_A
#undef TARGET_ANIMATION_B

// ---- games/tla/src/field/event_commands/age_and_cue.c ----
#include "field_event.h"

struct TimedObject {
  u8 unk00[0x64];
  s16 age;
  u8 unk66[6];
  void *update;
};

void BattleEffect_AgeAndCueAtEighty(struct TimedObject *object) {
  if (object->age == 80) {
    object->update = 0;
    Sound_PlayFar(136);
  }
  object->age++;
}

#include "field_particle.h"


/* VIEW(age_and_cue): routine Vector_AddPolarOffset as void Vector_AddPolarOffset(s32 radius, s32 angle, s32 *position); f0dfd00 has void Vector_AddPolarOffset(s32 magnitude, s32 angle, struct EffectVector *p). */
extern void Vector_AddPolarOffset__age_and_cue(s32 radius, s32 angle, s32 *position) __asm__("Vector_AddPolarOffset");
void Camera_WorldToScreen(struct EffectVector *pos);
void BattleEffect_ClearOwnedSlot(struct FieldParticle *slot);

void Func_080e0020(struct FieldParticle *effect) {
  struct BattleFxScene *scene;
  struct EffectVector pos;
  s8 *st;
  s16 angle;
  s32 state;

  scene = Ram_HeapSlots->effectWork;
  st = &effect->state;
top:
  state = *st;
  if (state == 0) {
    pos.x = effect->originX;
    pos.z = effect->originZ;
    angle = Random16();
    Vector_AddPolarOffset__age_and_cue(Random16() * 30 + (40 << 16), (u16)angle, &pos);
    effect->targetX = pos.x;
    effect->targetZ = pos.z;
    effect->acceleration = 4 << 16;
    effect->maxSpeed = 4 << 16;
    effect->limitTurnStep = state;
    *st = *st + 1;
    return;
  }
  if (state == 1) {
    if (FieldParticle_HasPendingTarget(effect) != 0)
      return;
    *st = *st + 1;
    goto top;
  }
  if (state == 2) {
    pos.x = scene->x;
    pos.y = scene->y + (8 << 16);
    pos.z = scene->z;
    Camera_WorldToScreen(&pos);
    Vector_AddPolarOffset__age_and_cue(4 << 16, Random16(), &pos);
    effect->targetX = pos.x;
    effect->targetZ = pos.z;
    effect->maxTurnStep = 0x1000;
    effect->limitTurnStep = 1;
    *st = *st + 1;
    return;
  }
  if (state == 3 && FieldParticle_HasPendingTarget(effect) == 0)
    BattleEffect_ClearOwnedSlot(effect);
}

void Func_080e0134(void);

/// Pauses the scene's child (byte 91) and runs the fallback transition.
void BattleEffect_MarkChildAndRunFallbackTransition(void) {
  (*(u8 **)((u8 *)Ram_HeapSlots->effectWork + 20))[91] = 1;
  Func_080e0134();
}


// ⚓️'s effect work holds its slots 8 bytes earlier than ☀️'s.
struct FallbackScene {
  u8 unk00[0x50];
  struct FieldParticle slots[16];
};

void BattleEffect_InitializeSharedScene(void);
void BattleEffect_PrepareBufferInterpolation(void);
struct Actor *Actor_Spawn(s32 kind, s32 x, s32 y, s32 z);
void Actor_Destroy(struct Actor *object);
void FieldParticle_Initialize(struct FieldParticle *effect, s32 kind, s32 x,
                              s32 z);
#include "actor_view.h"

void Func_080e0134(void) {
  struct BattleFxScene *scene = Ram_HeapSlots->effectWork;
  struct Actor *user = scene->mainObject;
  struct Actor *obj;
  struct FieldParticle *slot;
  struct EffectVector pos;
  struct EffectVector from;
  struct EffectVector to;
  s32 step;
  s32 i;

  scene->y = user->y;
  obj = Actor_Spawn(309, 0, 0, 0);
  step = 0;
  Actor_SetMode(obj, 0);
  if (obj == 0)
    return;

  BattleEffect_InitializeSharedScene();
  from.x = user->x;
  from.y = user->y + (16 << 16);
  from.z = user->z;
  to.x = scene->x;
  to.y = scene->y + (8 << 16);
  to.z = scene->z;
  for (;;) {
    s32 steps = 11;
    s32 scale;

    obj->x = from.x + (to.x - from.x) * step / 10;
    obj->y = from.y + (to.y - from.y) * step / 10;
    obj->z = from.z + (to.z - from.z) * step / 10;
    scale = BattleEffect_Interpolate(Q16_ONE, 0x4000, step);
    obj->scaleX = scale;
    obj->scaleY = scale;
    wait(1);
    step++;
    if (step >= steps)
      break;
  }

  wait(5);
  Actor_SetMode(obj, 1);
  Sound_PlayFar(108);
  wait(10);
  Sound_PlayFar(108);
  wait(10);
  Sound_PlayFar(108);
  wait(10);
  Sound_PlayFar(109);

  slot = &((struct FallbackScene *)scene)->slots[0];
  for (i = 0; i < 16; i++) {
    pos.x = obj->x;
    pos.y = obj->y + (8 << 16);
    pos.z = obj->z;
    Camera_WorldToScreen(&pos);
    Vector_AddPolarOffset__age_and_cue(4 << 16, Random16(), &pos);
    FieldParticle_Initialize(slot, 673, pos.x, pos.z);
    FieldParticle_SetCallback(slot, Func_080e0020);
    Animation_ApplyChildValuesToRecordFar(slot->object, 7);
    slot++;
  }

  pos.x = obj->x;
  pos.y = obj->y + (8 << 16);
  pos.z = obj->z;
  wait(8);
  Actor_Destroy(obj);
  wait(4);
  wait(30);
  BattleEffect_PrepareBufferInterpolation();
}

#include "object_lookup.h"
#include "main_routine_work.h"

#define Actor_SetAnimationSpeedFar                                             \
  ((void (*)(void *, s32))Actor_SetAnimationSpeedFar)

void BattleEffect_SetRandomTableValueOnObject(struct ActorView *object) {
  s8 *table = gBattleEffect_RandomChildValues;
  s32 index = Random16();
  Animation_ApplyChildValuesFar(object, table[(u32)(index * 8) >> 16]);
}

void BattleEffect_PauseObject(s32 id) {
  struct ActorView *object;
  struct AnimationEntry *entry;

  object = ObjectTable_Get(id);
  if (object != NULL) {
    gMainRoutineWork.savedCallback = (u32)object->update;
    gMainRoutineWork.savedByte = 0;
    if (object->kind == 1) {
      entry = object->target.animation->entries[0];
      if (entry != NULL) {
        gMainRoutineWork.savedByte = entry->param;
      }
    }
    object->update = BattleEffect_SetRandomTableValueOnObject;
    object->value5b = 1;
    Actor_SetAnimationSpeedFar(object, 0);
  }
}

void BattleEffect_ResumeObject(s32 id) {
  struct ActorView *object = ObjectTable_Get(id);

  if (object != NULL) {
    if (object->update == BattleEffect_SetRandomTableValueOnObject) {
      object->update = (void *)gMainRoutineWork.savedCallback;
      gMainRoutineWork.savedCallback = 0;
      Animation_ApplyChildValuesFar(object, (s8)gMainRoutineWork.savedByte);
    }
    object->value5b = 0;
    Actor_SetAnimationSpeedFar(object, 16);
  }
}

// Battle effects: filter the effect's target object.

s32 BattleEffect_GetFlags();

/// Returns no target (-1) when the current effect definition's flags are
/// 0xff, else id.
/// id: target object ID.
s32 BattleEffect_FilterObjectIdByFlags(s32 id) {
  if (BattleEffect_GetFlags() == 0xff)
    return -1;
  return id;
}

void Func_080e03cc(void);

/// Runs field effect 13 (Func_080e03cc) for the request dispatcher.
void BattleEffect_CallEffect13(void) { Func_080e03cc(); }

struct FxScene13 {
  u16 angle;
  u8 unk02[2];
  s32 x;
  s32 y;
  s32 z;
  struct Actor *mainObject;
  struct Actor *child;
  u8 unk18[8];
  s8 enabled;
};

struct FxPosition13 {
  s32 x;
  s32 y;
  s32 z;
};

static __inline__ s32 Interpolate(s32 from, s32 to, s32 step) {
  return from + (to - from) * step / 10;
}

void Func_080e03cc(void) {
  struct FxScene13 *scene = Ram_HeapSlots->effectWork;
  struct Actor *mainObject = scene->mainObject;
  struct Actor *secondary = scene->child;
  struct Actor *object;
  struct FxPosition13 spawn;
  struct FxPosition13 origin;
  struct FxPosition13 target;
  s32 step;
  s32 index;
  s32 steps = 11;

  origin.x = mainObject->x;
  origin.y = mainObject->y + 0x100000;
  origin.z = mainObject->z;
  if (scene->enabled) {
    target.x = mainObject->x;
    target.y = mainObject->y + 0x200000;
    target.z = mainObject->z;
    Vector_AddPolarOffset__age_and_cue(0x200000, scene->angle, &target.x);
  } else {
    target.x = scene->x;
    target.y = scene->y + 0x200000;
    target.z = scene->z;
  }
  spawn.x = scene->x;
  spawn.y = scene->y + 0x200000;
  spawn.z = scene->z;
  object = Actor_Spawn(278, spawn.x, spawn.y, spawn.z);
  if (object == 0)
    return;
  BattleEffect_InitializeSharedScene();
  Sound_PlayFar(138);
  object->facing = mainObject->facing;
  object->speedLimit = 0x14ccc;
  object->motionFlags = 0;
  Actor_SetMode(object, 5);
  Animation_ApplyChildValuesFar(object, 1);
  step = 0;
  do {
    s32 scale;

    object->x = origin.x + (target.x - origin.x) * step / 10;
    object->y = origin.y + (target.y - origin.y) * step / 10;
    object->z = origin.z + (target.z - origin.z) * step / 10;
    scale = Interpolate(0x4000, Q16_ONE, step);
    object->scaleX = scale;
    object->scaleY = scale;
    wait(1);
    step++;
  } while (step < steps);
  wait(10);
  Actor_SetMode(object, 6);
  wait(15);
  for (index = 0; index < 10; index++) {
    object->y -= 0x20000;
    wait(1);
  }
  Actor_SetMode(object, 5);
  Sound_PlayFar(132);
  if (secondary != 0)
    Actor_SetPositionAndResetMotionFar(secondary, -0x90000, secondary->y,
                                       -0x90000);
  wait(20);
  for (index = 0; index < 13; index++) {
    object->y += 0x18000;
    wait(1);
  }
  wait(10);
  Sound_PlayFar(114);
  step = 0;
  do {
    s32 scale;

    object->x = target.x + (origin.x - target.x) * step / 10;
    object->y = target.y + (origin.y - target.y) * step / 10;
    object->z = target.z + (origin.z - target.z) * step / 10;
    scale = Interpolate(Q16_ONE, 0x4000, step);
    object->scaleX = scale;
    object->scaleY = scale;
    wait(1);
    step++;
  } while (step < steps);
  Actor_Destroy(object);
  BattleEffect_PrepareBufferInterpolation();
}

#include "effect_sprite.h"
#include "vector3.h"

struct ScatterParticle {
  s32 x;
  s32 y;
  s32 z;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 timer;
};

struct ScatterWork {
  u8 tiles[0x1000];
  struct EffectSprite sprites[64];
  struct ScatterParticle particles[64];
  struct Actor *object;
  s16 phase;
  s16 timer;
  s16 spawnCount;
  s16 next;
  u16 tileA;
  u16 tileB;
  s16 flash;
  s16 hit;
};

struct ScatterEvent {
  u8 unk000[0xd31];
  s8 target;
  s8 targetY;
  s8 targetZ;
};

void EffectSprite_DrawAtWorldPosition(struct EffectSprite *sprite,
                                      struct ScatterParticle *p);
void Func_080d92a4(s32 target, struct Vector3 *position);

void Func_080e0618(void) {
  struct ScatterWork *work =
      (struct ScatterWork *)Ram_HeapSlots->battleFxWork[0];
  struct BattleFxScene *scene = Ram_HeapSlots->effectWork;
  struct ScatterEvent *event = Ram_HeapSlots->eventWork;
  struct Actor *object = work->object;
  struct Vector3 position;
  struct EffectSprite *sprite;
  struct ScatterParticle *particle;
  s32 i;

  work->flash = 0;
  for (i = 0; i < work->spawnCount; i++) {
    s32 distance;
    s32 speed;

    particle = &work->particles[work->next];
    particle->x = object->x;
    particle->y = object->y;
    particle->z = object->z;
    distance = Random16() * 12 + 0x40000;
    Vector_AddPolarOffset__age_and_cue(distance, Random16(), particle);
    particle->velocityX = 0;
    particle->velocityY = 0x20000 - Random16() * 2;
    particle->velocityZ = 0;
    speed = Random16() + 0x18000;
    Vector_AddPolarOffset__age_and_cue(speed, Random16(), &particle->velocityX);
    particle->timer = 0;
    work->next++;
    work->next &= 63;
  }

  for (i = 0, particle = work->particles; i <= 63; i++, particle++) {
    s32 timer = particle->timer;

    if (timer >= 0) {
      sprite = &work->sprites[i];
      if (i & 1)
        sprite->oam.index = work->tileA + (timer & 3) * 2;
      else
        sprite->oam.index = work->tileB + (timer & 3) * 2;
      EffectSprite_DrawAtWorldPosition(sprite, particle);
      BattleEffect_IntegrateVector3(particle, 63, -0x1400);
      if (++particle->timer == 32)
        particle->timer = -1;
    }
  }

  switch (work->phase) {
  case 0: {
    s32 a = Random16();
    s32 b = Random16();
    s32 c, d;

    position.x = scene->x + a * 3 - b * 3;
    c = Random16();
    d = Random16();
    position.y = scene->y + c * 3 - d * 3;
    position.z = scene->z;
    Actor_SetPositionAndResetMotionFar(object, position.x, position.y,
                                       position.z);
    if (work->timer == 20) {
      work->phase++;
      work->timer = -1;
    }
    break;
  }
  case 1:
    Actor_SetPositionAndResetMotionFar(object, scene->x, scene->y, scene->z);
    if (work->timer == 6) {
      work->phase++;
      work->timer = -1;
    }
    break;
  case 2:
    if (work->timer == 0)
      work->flash = 1;
    Func_080d92a4(event->target, &position);
    object->x = position.x;
    object->y = position.y - 0x40000;
    object->z = position.z;
    if (work->hit != 0) {
      work->phase++;
      work->timer = -1;
    }
    if (work->timer == 90) {
      work->phase++;
      work->timer = -1;
    }
    break;
  case 3:
    Actor_SetMode(object, 3);
    work->spawnCount = 3;
    object->y += work->timer << 17;
    if (work->timer == 10) {
      work->phase++;
      work->timer = -1;
    }
    break;
  case 4:
    object->scaleX = 0;
    work->spawnCount = 0;
    if (work->timer == 30)
      work->phase = 999;
    break;
  }
  work->timer++;
}

struct EffectSceneState {
  u8 unk000[0xd31];
  u8 valueD31;
  u8 valueD32;
  u8 valueD33;
};

struct EffectSceneWork {
  u8 unk00[0x20];
  u8 failed;
};

s32 Func_080d9a74(s32 value, s32 *first, s32 *second);
void Func_080e09c0(void);

/// Effect 17 on a target: asks Func_080d9a74 for two results; on failure
/// marks the effect failed, else stores the target and both in the event work.
/// Then runs Func_080e09c0.
void Func_080e0978(s32 value) {
  struct EffectSceneState *state = Ram_HeapSlots->eventWork;
  struct EffectSceneWork *work = Ram_HeapSlots->effectWork;
  s32 second;
  s32 first;

  if (Func_080d9a74(value, &first, &second)) {
    work->failed = 1;
  } else {
    state->valueD32 = value;
    // An empty once-only loop keeps the first result's load after the value's
    // store; without it, 4 of 34 instructions differ in tla-en.
    do {
    } while (0);
    state->valueD33 = first;
    state->valueD31 = second;
  }
  Func_080e09c0();
}

#include "graphics/resource.h"
#include "system/task.h"
#include "system/heap_release.h"

void Func_080dbed0(struct Actor *object, s32 x, s32 y, s32 z);
struct Actor *BattleEffect_SpawnScatterParticle(s32 x, s32 y, s32 z);
void Func_080d92d4(s32 target, s32 y, s32 z);

extern char ResourceId_SparkleFade;
extern char ResourceId_StarSparkleFade;

void Func_080e09c0(void) {
  struct ScatterWork *work = HeapSlot_Alloc(92, 0x2114);
  struct BattleFxScene *scene = Ram_HeapSlots->effectWork;
  struct ScatterEvent *event = Ram_HeapSlots->eventWork;
  struct Actor *caster = scene->mainObject;
  struct Actor *object;
  struct EffectSprite *sprite;
  struct ScatterParticle *particle;
  s32 slotA;
  s32 slotB;
  s32 tileA;
  s32 i;

  if (scene->child != 0) {
    scene->y += 0x40000;
    scene->z += 0x40000;
  }
  BattleEffect_InitializeSharedScene();
  object = Actor_Spawn(363, 0, 0, 0);
  if (object != 0) {
    object->facing = caster->facing;
    object->motionFlags = 0;
    if (caster->priorityFlags & 0x20)
      object->priorityFlags |= 0x20;
    Actor_SetMode(object, 0);
    Sound_PlayFar(138);
    Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_SparkleFade),
                          work);
    slotA = Resource_FindFreeEntry();
    tileA = VramCache_Load(slotA, 0x100, work);
    work->tileA = tileA;
    Resource_DecodeType01(
        Resource_GetTableEntry((s32)&ResourceId_StarSparkleFade), work);
    slotB = Resource_FindFreeEntry();
    work->tileB = VramCache_Load(slotB, 0x100, work);

    particle = work->particles;
    sprite = work->sprites;
    for (i = 63; i >= 0; i--) {
      Func_080eaf98(sprite, 4, 4, 0, tileA);
      sprite->depth = 250;
      sprite->oam.priority = 0;
      sprite->oam.palette256 = 1;
      sprite->oam.palette = 0;
      particle->x = object->x;
      particle->y = object->y;
      particle->z = object->z;
      Vector_AddPolarOffset__age_and_cue(0xc0000, Random16(), particle);
      particle->timer = -1;
      sprite++;
      particle++;
    }

    Func_080dbed0(object, scene->x, scene->y + 0x200000, scene->z);
    Actor_SetMode(object, 1);
    wait(10);
    for (i = 15; i >= 0; i--) {
      object->y += -0x20000;
      wait(1);
    }
    Sound_PlayFar(132);
    if (scene->enabled != 0) {
      for (i = 15; i >= 0; i--)
        BattleEffect_SpawnScatterParticle(object->x, object->y, object->z);
      wait(1);
    } else {
      wait(6);
      work->object = object;
      work->phase = 0;
      work->timer = 0;
      work->next = 0;
      work->spawnCount = 1;
      work->hit = 0;
      Task_Add(Func_080e0618, 0x480);
      while (work->phase != 999) {
        if (work->flash == 1) {
          Sound_PlayFar(0x200);
          Func_080d92d4(event->target, event->targetY, event->targetZ);
          work->hit = 1;
          Sound_PlayFar(395);
        }
        wait(1);
      }
      Task_Remove(Func_080e0618);
    }
    Resource_ResetEntry(slotA);
    Resource_ResetEntry(slotB);
    Actor_Destroy(object);
  }
  BattleEffect_PrepareBufferInterpolation();
  HeapSlot_Release(92);
}

#include "scene.h"

void BattleEffect_AdvanceAngleWithRamp(struct Actor *object) {
  u32 step;

  step = object->timer * 0x50;
  object->facing += step + 0x1000;
  if (step < 0x1000)
    object->timer++;
}

/// Per-frame update: spins 1/8 turn a frame as it shrinks, rising a pixel a
/// frame; released once its scale falls below 0x3000.
// As ☀️'s FieldEffect_UpdateDescendingObject.
void BattleEffect_UpdateDescendingObject(struct Actor *obj) {
  u32 scale;
  u32 scaleX;
  s32 step = -0x280;

  obj->scaleY += step;
  scaleX = obj->scaleX;
  scale = scaleX + step;
  obj->facing += 0x2000;
  obj->y += 1 << 16;
  obj->scaleX = scale;
  if ((s32)scale < 0x3000) {
    obj->animationKind = 0;
  }
}

#include "frame.h"

struct LaunchVector {
  s32 x;
  s32 y;
  s32 z;
};

struct LaunchObject {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
};

struct LaunchScene {
  u8 unk00[0x10];
  struct LaunchObject *mainObject;
};

void Func_080e0cec(struct FieldParticle *slot) {
  struct LaunchVector pos;
  struct LaunchObject *src;
  s32 state;
  u32 r;
  struct LaunchVector *arg;
  s32 lift;

  src = ((struct LaunchScene *)Ram_HeapSlots->effectWork)->mainObject;
  state = slot->state;

  if (state == 0) {
    pos.x = src->x;
    arg = &pos;
    pos.y = src->y;
    pos.z = src->z;
    Camera_WorldToScreen(arg);

    slot->x = slot->originX = pos.x;
    slot->z = slot->originZ = pos.z + (8 << 16);

    r = Random16();
    Vector_AddPolarOffset__age_and_cue(
        120 << 16, ((r << 13) >> 16) - ((Random16() << 13) >> 16) + 0xc000,
        &pos);

    slot->targetX = pos.x;
    slot->targetZ = pos.z;
    slot->acceleration = 5 << 16;
    slot->maxSpeed = 5 << 16;
    slot->limitTurnStep = state;
    slot->state++;

    if ((gFrameCount & 2) != 0)
      Sound_PlayFar(246);
  } else if (state == 1) {
    if (FieldParticle_HasPendingTarget(slot) == 0)
      slot->state--;
  } else if (state == 2) {
    if (FieldParticle_HasPendingTarget(slot) == 0)
      BattleEffect_ClearOwnedSlot(slot);
  }
}
#undef Actor_SetAnimationSpeedFar

// ---- games/tla/src/field/event_commands/launch.c ----
#define SLOT_MAX 24

struct SpinObject {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  s32 floor;
  s32 fall;
  u8 unk1c[0x1c];
  s32 targetX;
  u8 unk3c[0x19];
  u8 motionFlags;
  u8 unk56[0xe];
  s16 timer;
  u8 unk66[6];
  void (*update)(struct SpinObject *);
};

struct SpinScene {
  u8 unk00[0x10];
  struct SpinObject *mainObject;
  u8 unk14[4];
  s16 firstObjectId;
  u8 unk1a[0x36];
  struct FieldParticle slots[SLOT_MAX];
};

struct SpinVector {
  s32 x;
  s32 y;
  s32 z;
};

void Event_Start(void);
void Camera_MoveWithinMapBounds(s32, s32, s32, s32);
void BattleEffect_InitializeSharedScene(void);
void wait(s32 frames);
void Actor_ArmCallback(s32 actor, s32 angle, s32 frames);
void Sound_PlayFar(s32 cue);
/* VIEW(launch): routine Animation_ApplyChildValuesFar as void Animation_ApplyChildValuesFar(void *object, s32 value); actor_view.h has void Animation_ApplyChildValuesFar(struct Actor *, u32). */
extern void Animation_ApplyChildValuesFar__launch(void *object, s32 value) __asm__("Animation_ApplyChildValuesFar");
/* VIEW(launch): routine Actor_SetSingleChildFlagsFar as void Actor_SetSingleChildFlagsFar(void *object, s32 flags); actor_view.h has void Actor_SetSingleChildFlagsFar(struct Actor *object, u32 flags). */
extern void Actor_SetSingleChildFlagsFar__launch(void *object, s32 flags) __asm__("Actor_SetSingleChildFlagsFar");
/* VIEW(launch): routine Camera_WorldToScreen as void Camera_WorldToScreen(struct SpinVector *position); age_and_cue has void Camera_WorldToScreen(struct EffectVector *pos). */
extern void Camera_WorldToScreen__launch(struct SpinVector *position) __asm__("Camera_WorldToScreen");
void FieldParticle_Initialize(struct FieldParticle *slot, s32 kind, s32 x,
                              s32 z);
/* VIEW(launch): routine Animation_ApplyChildValuesToRecordFar as void Animation_ApplyChildValuesToRecordFar(void *record, u32 value); actor_view.h has void Animation_ApplyChildValuesToRecordFar(struct ActorSpriteView *, u32). */
extern void Animation_ApplyChildValuesToRecordFar__launch(void *record, u32 value) __asm__("Animation_ApplyChildValuesToRecordFar");
u32 Random16(void);
void BattleEffect_PrepareBufferInterpolation(void);
void Actor_ApplyRandomChildValues(struct SpinObject *owner);
/* VIEW(launch): routine BattleEffect_AdvanceAngleWithRamp as void BattleEffect_AdvanceAngleWithRamp(struct SpinObject *object); the definition has void BattleEffect_AdvanceAngleWithRamp(struct Actor *object). */
extern void BattleEffect_AdvanceAngleWithRamp__launch(struct SpinObject *object) __asm__("BattleEffect_AdvanceAngleWithRamp");
/* VIEW(launch): routine BattleEffect_UpdateDescendingObject as void BattleEffect_UpdateDescendingObject(struct SpinObject *obj); the definition has void BattleEffect_UpdateDescendingObject(struct Actor *obj). */
extern void BattleEffect_UpdateDescendingObject__launch(struct SpinObject *obj) __asm__("BattleEffect_UpdateDescendingObject");
void Func_080e0cec(struct FieldParticle *slot);

void FieldEffect_RunRetreat(void) {
  struct SpinScene *scene = Ram_HeapSlots->effectWork;
  struct SpinObject *obj = scene->mainObject;
  struct SpinVector pos;
  struct FieldParticle *slot;
  struct FieldParticle *slots;
  u8 last;
  u32 i;

  Event_Start();
  Camera_MoveWithinMapBounds(-1, -1, -1, 0);
  BattleEffect_InitializeSharedScene();
  wait(10);
  Actor_ArmCallback(scene->firstObjectId, 0x4000, 0);
  wait(30);
  obj->update = Actor_ApplyRandomChildValues;
  Sound_PlayFar(131);
  Actor_SetMode(obj, 28);
  wait(40);

  Sound_PlayFar(220);
  Animation_ApplyChildValuesFar__launch(obj, 0);
  Actor_SetMode(obj, 3);
  obj->update = BattleEffect_AdvanceAngleWithRamp__launch;
  obj->timer = 0;
  wait(70);

  Actor_SetSingleChildFlagsFar__launch(obj, 0);
  obj->motionFlags = 0;
  obj->update = BattleEffect_UpdateDescendingObject__launch;
  obj->targetX = EFFECT_NO_TARGET;
  pos.x = obj->x;
  pos.y = obj->y;
  pos.z = obj->z;
  Camera_WorldToScreen__launch(&pos);
  i = 0;
  slot = scene->slots;
  slots = slot;
  do {
    s32 scale;

    FieldParticle_Initialize(slot, 0x2a0, pos.x, pos.z);
    FieldParticle_SetCallback(slot, Func_080e0cec);
    FieldParticle_SetActorMode(slot, 7);
    Animation_ApplyChildValuesToRecordFar__launch(slot->object, (Random16() * 7) >> 16);
    scale = (Random16() >> 1) + 0x13333;
    slot->scaleY = scale;
    slot->scaleX = scale;
    wait(1);
    i++;
    slot++;
  } while (i < SLOT_MAX);
  wait(70);

  i = 0;
  last = 2;
  slot = slots;
  do {
    if (slot->active != 0)
      slot->state = last;
    i++;
    slot++;
  } while (i < SLOT_MAX);
  wait(40);
  BattleEffect_PrepareBufferInterpolation();
  obj->fall = 0;
  obj->y = obj->floor;
  wait(10);
}

#include "scene_ids.h"

void Actor_Destroy(struct Actor *object);

void BattleEffect_UpdateDescendingParticlePositiveArc(struct Actor *particle) {
  struct Actor *source;
  s32 y;
  s32 limit;
  s32 scale;
  struct MainRoutineWork *party = &gMainRoutineWork;

  limit = particle->terrainHeight + 0xA0000;
  source = particle->watchedObject;
  if (party->map == (s32)&SceneId_Ov64a)
    limit = particle->terrainHeight + 0x40000;

  y = particle->y;
  if (y <= limit) {
    Actor_Destroy(particle);
    return;
  }

  scale = particle->scaleX + 0xC00;
  if (scale > Q16_ONE)
    scale = Q16_ONE;
  particle->scaleX = scale;
  particle->scaleY = scale;
  particle->x = source->x;
  particle->y = y - 0x20000;
  particle->z = source->z + ((0x10000 - scale) * 5) + 0x90000;
}

void BattleEffect_UpdateDescendingParticleNegativeArc(struct Actor *particle) {
  struct Actor *source;
  s32 y;
  s32 limit;
  s32 scale;
  struct MainRoutineWork *party = &gMainRoutineWork;

  limit = particle->terrainHeight + 0xA0000;
  source = particle->watchedObject;
  if (party->map == (s32)&SceneId_Ov64a)
    limit = particle->terrainHeight + 0x40000;

  y = particle->y;
  if (y <= limit) {
    Actor_Destroy(particle);
    return;
  }

  scale = particle->scaleX + 0xC00;
  if (scale > Q16_ONE)
    scale = Q16_ONE;
  particle->scaleX = scale;
  particle->scaleY = -scale;
  particle->x = source->x;
  particle->y = y - 0x20000;
  particle->z = source->z - ((0x10000 - scale) * 5) + 0x100000;
}

#include "resource.h"

struct ArcPicture {
  u8 padding00[22];
  u8 visible;
};

struct ArcSprite {
  u8 padding00[5];
  u8 flags05_0 : 5;
  u8 flags05_5 : 1;
  u8 layer : 2;
  u8 padding06;
  u8 flags07_0 : 6;
  u8 size : 2;
  union {
    struct {
      u16 tile : 10;
      u16 flags08_10 : 6;
    } half;
    struct {
      u8 tileLow;
      u8 flags09_0 : 2;
      u8 variant : 2;
      u8 flags09_4 : 4;
    } byte;
  } attr;
  u8 padding0c[4];
  u8 resource;
  u8 active : 1;
  u8 flags11_1 : 7;
  u8 padding12[8];
  u8 animation;
  u8 padding1b[13];
  struct ArcPicture *picture;
};

struct ArcObject {
  u8 padding00[8];
  s32 x;
  s32 y;
  s32 z;
  s32 unk14;
  s32 scaleX;
  s32 scaleY;
  u8 padding20[0x30];
  struct ArcSprite *sprite;
  u8 padding54;
  u8 unk55;
  u8 padding56[0x0e];
  u16 unk64;
  u8 padding66[2];
  struct ArcObject *source;
  void *callback;
};

#include "vram_tab.h"

struct ArcObject *Func_080200c0(s32 kind, s32 x, s32 y, s32 z);
void Animation_ApplyChildArgumentFar(struct ArcSprite *sprite, s32 animation);

void Func_080e1024(struct ArcObject *source) {
  u8 *work = Ram_HeapSlots->effectWork;
  struct ArcObject *leader = *(struct ArcObject **)(work + 16);
  struct ArcObject *objects[2];
  struct ArcObject *object;
  struct ArcSprite *sprite;
  s32 i;
  u8 zero;

  for (i = 0; i <= 1; i++) {
    object = Func_080200c0(271, source->x, source->y + 0x400000, source->z);
    objects[i] = object;
    if (object == NULL)
      continue;
    object->unk14 = source->unk14;
    sprite = object->sprite;
    object->unk55 = 0;
    object->unk64 = 0;
    zero = 0;
    object->source = source;
    object->scaleY = 0x6666;
    object->scaleX = 0x6666;
    if (sprite == NULL)
      continue;
    Animation_ApplyChildArgumentFar(sprite, 0);
    sprite->animation = zero;
    Resource_ResetEntry(sprite->resource);
    sprite->resource = *(u16 *)(work + 0x710);
    sprite->active = 1;
    sprite->attr.half.tile =
        gResourceTableEntries[sprite->resource].offset >> 5;
    sprite->flags05_5 = 0;
    sprite->layer = 1;
    sprite->size = 2;
    sprite->picture->visible = zero;
  }
  objects[0]->callback =
      (void *)BattleEffect_UpdateDescendingParticleNegativeArc;
  objects[0]->sprite->attr.byte.variant = 0;
  objects[1]->callback =
      (void *)BattleEffect_UpdateDescendingParticlePositiveArc;
  objects[1]->sprite->attr.byte.variant = leader->sprite->attr.byte.variant;
}

struct ArcEntry {
  u8 unk00[5];
  u8 param;
};

struct ArcGroup {
  u8 unk00[25];
  u8 dirty;
  u8 flags;
  u8 unk1b[13];
  struct ArcEntry *entries[1];
};

/* VIEW(launch): tag struct ArcEffectObject as struct ArcEffectObject__launch; field_psynergy_work.h has another definition. */
struct ArcEffectObject__launch {
  u8 unk00[6];
  u16 angle;
  u8 unk08[0x48];
  struct ArcGroup *records;
  u8 unk54[0x10];
  s16 phase;
  u16 step;
  u8 unk68[4];
  void (*callback)(struct ArcEffectObject__launch *);
};

extern char MsgAbilityWoreOff;
extern char MsgItemWoreOff;

void Func_080cb82c(void);
void Func_080cb8a4(void);
void MessageArgument_PushFar(s32 value, s32 slot);
void FieldMessage_ShowFar(s32 message, s32 flags);

/// The caster during Avoid (BattleEffect_RunEffect16): sheds a pair of falling
/// arcs every 5 frames (7 in scene Ov64a), and spins in phase 1.
// As ☀️'s function of the same name.
void BattleEffect_UpdatePairedArcSpawner(struct ArcEffectObject__launch *caster) {
  s16 phase;
  s32 elapsedFrames;
  struct MainRoutineWork *party;

  phase = caster->phase;
  elapsedFrames = (s16)caster->step++;
  party = &gMainRoutineWork;

  if (party->map == (s32)&SceneId_Ov64a) {
    if (elapsedFrames % 7 == 0)
      Func_080e1024(caster);
  } else if (elapsedFrames % 5 == 0) {
    Func_080e1024(caster);
  }

  if (phase == 1)
    caster->angle += 0xc00;
}

void FieldEvent_ShowStatusMessage(void) {
  gMainRoutineWork.avoidDistance = 0;
  Func_080cb82c();
  if (gMainRoutineWork.avoidByItem == 0) {
    MessageArgument_PushFar(150, 4);
    FieldMessage_ShowFar((s32)&MsgAbilityWoreOff, 1);
  } else {
    MessageArgument_PushFar(236, 2);
    FieldMessage_ShowFar((s32)&MsgItemWoreOff, 1);
  }
  Func_080cb8a4();
}

struct ArcScene {
  u8 unk00[0x10];
  struct ArcEffectObject__launch *mainObject;
  u8 unk14[0x6fc];
  s16 arcTileSlot;
};

extern const u8 gData_080ed90c[];
extern char MsgMonstersAttackLess;

s32 GameFlag_Test(s32 flag);

/// Psynergy effect 16: the user spins, rains arc particles, then flashes.
void BattleEffect_RunEffect16(void) {
  struct ArcScene *scene;
  struct ArcEffectObject__launch *object;
  struct ArcGroup *group;
  struct ArcEntry *entry;
  const u8 *tiles;
  u32 angle;
  s32 flags;
  s32 slot;
  s32 count;
  s32 phase;

  scene = Ram_HeapSlots->effectWork;
  object = scene->mainObject;
  group = object->records;
  entry = group->entries[0];
  angle = object->angle;
  flags = group->flags;
  slot = Resource_FindFreeEntry();
  scene->arcTileSlot = slot;
  // tiles holds the picture's address, loaded between the slot's two shifts;
  // as the address in the call, 2 of 156 instructions differ in tla-en.
  tiles = gData_080ed90c;
  VramCache_Load((s16)slot, 0x100, tiles);
  gMainRoutineWork.avoidDistance = 0x09600000;
  gMainRoutineWork.avoidByItem = GameFlag_Test(0x145);
  Animation_ApplyChildValuesFar__launch(object, 0);
  object->callback = BattleEffect_UpdatePairedArcSpawner;
  object->phase = 0;
  object->step = 0;
  Sound_PlayFar(140);
  wait(15);
  object->phase = 1;
  wait(10);
  for (count = 0; count < 20; count++) {
    entry->param = 7;
    group->dirty = 1;
    wait(2);
    entry->param = 0;
    group->dirty = 1;
    group->flags = 1;
    wait(3);
  }
  object->callback = NULL;
  object->angle = angle;

  // For 70 frames, set and clear flags bit 2 on a five-frame cycle.
  for (count = 0; count < 70; count++) {
    phase = count % 5;
    if (phase == 0) {
      group->dirty = 1;
      group->flags = flags | 2;
    } else if (phase == 2) {
      group->dirty = 1;
      group->flags = flags;
      entry->param = 0;
    }
    if (count == 15)
      Sound_PlayFar(174);
    wait(1);
  }
  {
    /* FAKEMATCH(temporary): the flags restored through a pointer of their
     * own, which sets the param's 0 after the flags' copy; as group->flags =
     * flags, 2 of 156 instructions differ in tla-en. */
    u8 *restore = &group->flags;

    *restore = flags;
  }
  group->dirty = 1;
  entry->param = 0;
  // An empty once-only loop keeps the wait's argument after the param store;
  // without it, 2 of 156 instructions differ in tla-en.
  do {
  } while (0);
  wait(1);

  Resource_ResetEntry(scene->arcTileSlot);
  FieldMessage_ShowFar((s32)&MsgMonstersAttackLess, 1);
}
#undef SLOT_MAX

// ---- games/tla/src/field/event_commands/flicker_object.c ----
#include "arc_effect.h"

void BattleEffect_WaitForArcGroups(void) {
  struct ArcEffectWork *work = Ram_HeapSlots->block62;
  s32 i = 0;

  while (work->age[0] <= 31 || work->age[1] <= 31) {
    wait(1);
    i++;
    if (i > 89)
      return;
  }
}

void BattleEffect_FlickerObjectAndSpawnArcs(struct Actor *object) {
  if ((gFrameCount & 2) != 0) {
    Animation_ApplyChildValuesFar(object, 7);
  } else {
    Animation_ApplyChildValuesFar(object, 0);
  }
  if ((gFrameCount & 15) == 0) {
    BattleEffect_SpawnScaledArcObjects(object);
  }
}

void BattleEffect_CycleObjectAndSpawnArcs(struct Actor *object) {
  if (gFrameCount & 1)
    Animation_ApplyChildValuesFar(object, (gFrameCount >> 1) % 6);
  if ((gFrameCount & 15) == 0)
    BattleEffect_SpawnScaledArcObjects(object);
}

void BattleEffect_SetObjectUpdateMode(struct Actor *object, s32 mode) {
  if (mode == 0) {
    object->update = 0;
    Animation_ApplyChildValuesFar(object, 0);
  } else if (mode == 1) {
    object->update = (void (*)(void))BattleEffect_FlickerObjectAndSpawnArcs;
  } else {
    object->update = (void (*)(void))BattleEffect_CycleObjectAndSpawnArcs;
  }
}

// The arc effect's next pair of scaled sprites.


struct ArcWork {
  struct EffectSprite sprites[2][2];
  s16 age[2];
  s16 resource;
  s16 group;
  struct Actor *target;
};

s32 BattleEffect_GetObjectPriority(struct Actor *object);

/// Sets up the current group's two arc sprites from the effect's tiles, at the
/// target's priority, then restarts that group's age, records the target and
/// switches to the other group.
void BattleEffect_SpawnScaledArcObjects(struct Actor *target) {
  struct ArcWork *work = Ram_HeapSlots->block62;
  s32 tiles = gResourceTableEntries[work->resource].offset >> 5;
  s32 group = work->group;
  struct EffectSprite *sprite;
  s32 i;

  for (i = 0; i < 2; i++) {
    sprite = &work->sprites[group][i];
    Func_080eaf98(sprite, 16, 16, 0x80004000, tiles);
    sprite->oam.palette256 = 0;
    sprite->oam.palette = 0;
    sprite->oam.priority = BattleEffect_GetObjectPriority(target);
  }
  work->age[group] = 0;
  work->target = target;
  work->group ^= 1;
}

// ---- games/tla/src/field/event_commands/f0e150c.c ----
/* VIEW(f0e150c): tag struct ArcSprite as struct ArcSprite__f0e150c; radial.c has another definition. */
struct ArcSprite__f0e150c {
  u8 unk00[12];
  s32 x;
  s32 y;
  s32 scaleX;
  s32 scaleY;
  u8 unk1c[2];
  s16 depth;
  u8 unk20[8];
};

/* VIEW(f0e150c): tag struct ArcWork as struct ArcWork__f0e150c; radial.c has another definition. */
struct ArcWork__f0e150c {
  struct ArcSprite__f0e150c sprites[2][2];
  s16 age[2];
  s16 resource;
  s16 group;
  struct Actor *target;
};

/* VIEW(f0e150c): routine Camera_WorldToScreen as void Camera_WorldToScreen(s32 *position); launch has void Camera_WorldToScreen(struct SpinVector *position). */
extern void Camera_WorldToScreen__f0e150c(s32 *position) __asm__("Camera_WorldToScreen");
s32 BattleEffect_GetObjectDepth(struct Actor *object);

void BattleEffect_UpdateArcEffect(void) {
  struct ArcWork__f0e150c *work = Ram_HeapSlots->block62;
  struct Actor *target = work->target;
  s32 group;
  s32 position[3];
  s32 i;

  for (group = 0; group < 2; group++) {
    s32 scale = Trig_Sin(work->age[group] << 10);

    if (work->age[group] < 0)
      continue;
    if (work->age[group] < 32) {
      struct ArcSprite__f0e150c *sprite;
      s32 x, y;

      position[0] = target->x;
      position[1] = target->y + (work->age[group] << 16);
      position[2] = target->z;
      Camera_WorldToScreen__f0e150c(position);
      x = position[0];
      y = position[2];
      for (i = 0; i < 2; i++) {
        sprite = &work->sprites[group][i];
        sprite->x = x;
        sprite->y = y;
        sprite->scaleX = scale;
        sprite->scaleY = scale;
        if (i == 0) {
          sprite->depth = BattleEffect_GetObjectDepth(work->target) - 1;
        } else {
          sprite->depth = BattleEffect_GetObjectDepth(work->target) + 1;
          sprite->y += 0xffff0000;
          sprite->scaleY = -sprite->scaleY;
        }
        Func_080eb01c(sprite);
      }
      work->age[group]++;
    }
  }
}

#include "heap_release.h"
#include "task.h"

void BattleEffect_StartArcEffect(void) {
  struct ArcEffectWork *work = HeapSlot_Alloc(248, sizeof(*work));

  work->resource = Resource_FindFreeEntry();
  VramCache_Load(work->resource, 0x100, gBattleEffect_ArcTiles);
  work->group = 0;
  work->age[0] = 999;
  work->age[1] = 999;
  work->target = 0;
  Task_Add((s32)BattleEffect_UpdateArcEffect, TASK_PASS_VBLANK);
}

void BattleEffect_EndArcEffect(void) {
  struct ArcEffectWork *work = Ram_HeapSlots->block62;

  Resource_ResetEntry(work->resource);
  Task_Remove((u32)BattleEffect_UpdateArcEffect);
  HeapSlot_Release(248);
}

#include "canvas_sprite.h"

// One moving point: a spark on the canvas (x and y only, counting delay) or
// a sprite's place in the world.
struct BowMotion {
  s32 x;
  s32 y;
  s32 z;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 delay;
};

struct BowWork {
  u8 *tiles;
  struct BowMotion sparks[32];
  struct BowMotion arrowMotion;
  struct EffectSprite arrow;
  struct BowMotion bowMotion;
  struct EffectSprite bow;
  s32 start[3];
  s32 target[3];
  s16 phase;
  s16 timer;
  u16 bowTile;
  s16 frame;
  s8 bowGrowing;
  s8 bowShrinking;
  s8 sparking;
  s8 sparksRepeat;
  s16 result;
  s8 done;
  s8 arrowMoving;
};

struct BowScene {
  u16 angleFraction;
  u16 angle;
  s32 x;
  s32 y;
  s32 z;
  struct Actor *mainObject;
};

struct BowEvent {
  u8 unk00[20];
  struct Actor *objects[80];
};

extern const s32 Data_080f0f34[8];
extern const s32 Data_080f0f54[4];
extern const u8 Data_080eda0c[];

/* VIEW(f0e150c): routine Vector_AddPolarOffset as void Vector_AddPolarOffset(s32 distance, s32 angle, s32 *position); f0dfd00 has void Vector_AddPolarOffset(s32 magnitude, s32 angle, struct EffectVector *p). */
extern void Vector_AddPolarOffset__f0e150c(s32 distance, s32 angle, s32 *position) __asm__("Vector_AddPolarOffset");
s32 Func_08020210(struct Actor *object, s32 *position);
s32 Func_080dbe80(s32 *a, s32 radiusA, s32 *b, s32 radiusB);
void BattleEffect_DrawCanvasBlock(const u8 *src, s32 x, s32 y, s32 width,
                                  s32 height);

#define BOW_NEXT_PHASE(work) ((work)->phase++, (work)->timer = -1)

void Func_080e1678(void) {
  struct BowScene *scene = Ram_HeapSlots->effectWork;
  struct BowWork *work = (struct BowWork *)Ram_HeapSlots->battleFxWork[0];
  struct CanvasSpriteWork *canvas = Ram_HeapSlots->unkec[1];
  struct BowEvent *event = Ram_HeapSlots->eventWork;
  struct Actor *object;
  struct BowMotion *motion;
  struct EffectSprite *sprite;
  s32 pos[3];
  s32 objectIndex;
  s32 i;
  s32 dx;
  s32 dz;

again:
  switch (work->phase) {
  hit:
    work->done = 1;
    work->result = objectIndex;
    BOW_NEXT_PHASE(work);
    goto again;
  case 0:
    if (work->timer == 0) {
      work->sparking = 0;
      work->sparksRepeat = 1;
      work->arrowMoving = 0;
      work->bowGrowing = 1;
      work->frame = 999;
    } else if (work->bowGrowing == 0) {
      work->frame = 0;
      BOW_NEXT_PHASE(work);
    }
    break;
  case 1:
    if (work->timer == 0) {
      sprite = &work->arrow;
      sprite->scaleX = sprite->scaleY = 0x10000;
      motion = &work->arrowMotion;
      pos[0] = pos[1] = pos[2] = 0;
      Vector_AddPolarOffset__f0e150c(0x2666, scene->angle + 0x8000, pos);
      motion->velocityX = pos[0];
      motion->velocityY = pos[1];
      motion->velocityZ = pos[2];
      work->arrowMoving = 1;
    }
    if (work->timer == 20)
      work->sparking = 1;
    if (work->timer == 56) {
      motion = &work->arrowMotion;
      pos[0] = pos[1] = pos[2] = 0;
      Vector_AddPolarOffset__f0e150c(0x50000, scene->angle, pos);
      motion->velocityX = pos[0];
      motion->velocityY = pos[1];
      motion->velocityZ = pos[2];
      BOW_NEXT_PHASE(work);
    }
    break;
  case 2:
    motion = &work->arrowMotion;
    if (work->timer == 90) {
      BOW_NEXT_PHASE(work);
      break;
    }
    dx = work->target[0] - work->start[0];
    if (dx < 0)
      dx = work->start[0] - work->target[0];
    dz = motion->x - work->start[0];
    if (dz < 0)
      dz = work->start[0] - motion->x;
    if (dx < dz) {
      BOW_NEXT_PHASE(work);
      goto again;
    }
    dx = work->target[2] - work->start[2];
    if (dx < 0)
      dx = work->start[2] - work->target[2];
    dz = motion->z - work->start[2];
    if (dz < 0)
      dz = work->start[2] - motion->z;
    if (dx < dz) {
      BOW_NEXT_PHASE(work);
      goto again;
    }
    pos[0] = motion->x;
    pos[1] = motion->y;
    pos[2] = motion->z;
    if (Func_08020210(scene->mainObject, pos) > 0) {
      work->done = 1;
      work->result = -1;
      work->phase++;
      work->timer = -1;
      goto again;
    }
    for (objectIndex = 0; objectIndex < 80; objectIndex++) {
      object = event->objects[objectIndex];
      if (object != NULL && object->script != NULL &&
          (object->collisionFlags & 1) && object != scene->mainObject &&
          Func_080dbe80(&motion->x, 2, &object->x, object->radius - 2) >= 0)
        goto hit;
    }
    break;
  case 3:
    if (work->done != 0) {
      work->arrowMoving = 0;
    } else {
      work->sparksRepeat = 0;
      work->bowShrinking = 1;
    }
    motion = &work->arrowMotion;
    pos[0] = pos[1] = pos[2] = 0;
    Vector_AddPolarOffset__f0e150c(0x20000, scene->angle, pos);
    motion->velocityX = pos[0];
    motion->velocityY = pos[1];
    motion->velocityZ = pos[2];
    BOW_NEXT_PHASE(work);
    break;
  case 4:
    sprite = &work->arrow;
    if (work->done != 0) {
      if (work->timer < 32) {
        s32 wobble[8];
        s32 fade[4];
        s32 w;

        memcpy(wobble, Data_080f0f34, sizeof(wobble));
        memcpy(fade, Data_080f0f54, sizeof(fade));
        w = wobble[work->timer & 7];
        sprite->angle = scene->angle + w * fade[work->timer / 8] / 0x10000;
      }
      if (work->timer == 32)
        work->bowShrinking = 1;
      if (work->timer == 50) {
        sprite->scaleX = sprite->scaleY = 0;
        work->arrowMoving = 1;
        work->sparksRepeat = 0;
        BOW_NEXT_PHASE(work);
      }
    } else {
      sprite->scaleX += -0x3000;
      sprite->scaleY = sprite->scaleX;
      if (sprite->scaleX < 0) {
        sprite->scaleX = sprite->scaleY = 0;
        BOW_NEXT_PHASE(work);
      }
    }
    break;
  case 5:
    if (work->bowShrinking == 0)
      work->phase = 999;
    break;
  }

  if (work->bowGrowing != 0) {
    sprite = &work->bow;
    sprite->scaleX += 0x400;
    sprite->scaleY = sprite->scaleX;
    sprite->angle += 0x2000;
    if (sprite->scaleX >= 0x10000) {
      sprite->scaleX = sprite->scaleY = 0x10000;
      sprite->angle = scene->angle;
      work->bowGrowing = 0;
    }
  }
  if (work->bowShrinking != 0) {
    sprite = &work->bow;
    sprite->scaleX += -0x400;
    sprite->scaleY = sprite->scaleX;
    sprite->angle += 0x2000;
    if (sprite->scaleX <= 0x6665) {
      sprite->scaleX = sprite->scaleY = 0;
      work->bowShrinking = 0;
    }
  }

  // The bow's drawing pose, by frame: drawn back, then loosed and recovered.
  if (work->frame != 999) {
    work->frame++;
    sprite = &work->bow;
    switch (work->frame) {
    case 10:
      sprite->oam.index = work->bowTile + 32;
      break;
    case 20:
      sprite->oam.index = work->bowTile + 64;
      break;
    case 54:
      sprite->oam.index = work->bowTile + 32;
      break;
    case 62:
      sprite->oam.index = work->bowTile + 96;
      break;
    case 65:
      sprite->oam.index = work->bowTile + 128;
      break;
    case 68:
      sprite->oam.index = work->bowTile + 160;
      break;
    case 58:
    case 71:
      sprite->oam.index = work->bowTile;
      break;
    case 72:
      work->frame = 999;
      break;
    }
  }

  motion = &work->bowMotion;
  pos[0] = motion->x;
  pos[1] = motion->y;
  pos[2] = motion->z;
  sprite = &work->bow;
  Camera_WorldToScreen__f0e150c(pos);
  sprite->x = pos[0];
  sprite->y = pos[2];
  Func_080eb01c(sprite);
  motion = &work->arrowMotion;
  pos[0] = motion->x;
  pos[1] = motion->y;
  pos[2] = motion->z;
  Camera_WorldToScreen__f0e150c(pos);
  sprite = &work->arrow;
  sprite->x = pos[0];
  sprite->y = pos[2];
  Func_080eb01c(sprite);
  if (work->arrowMoving != 0)
    BattleEffect_IntegrateVector3(&motion->x, 64, 0);

  if (work->sparking != 0) {
    motion = work->sparks;
    for (i = 9; i >= 0; i--) {
      if (motion->delay == 0) {
        s32 speed;
        s32 angle;

        speed = (Random16() & 255) + 256;
        angle = (Random16() & 0x1fff) + 0x7000;
        motion->x = 0x3c0000;
        motion->y = 0x1c0000;
        motion->velocityX = Trig_Cos(angle) * speed >> 6;
        motion->velocityY = Trig_Sin(angle) * speed >> 6;
      }
      if (motion->delay > 0) {
        s32 x = (s16)(motion->x >> 16);

        BattleEffect_DrawCanvasBlock(Data_080eda0c + (motion->delay & 1) * 64,
                                     x, (s16)(motion->y >> 16), 8, 8);
        BattleEffect_IntegrateVector2(&motion->x, 60, 0);
        /* FAKEMATCH(barrier): an empty asm after the step, which keeps the
         * picture's address add after the y's load for the draw; without it,
         * 2 of 923 instructions differ in tla-en. */
        asm("");
      }
      motion->delay++;
      if (work->sparksRepeat == 1 && motion->delay == 10)
        motion->delay = 0;
      motion++;
    }
  }

  motion = &work->arrowMotion;
  pos[0] = motion->x;
  pos[1] = motion->y;
  pos[2] = motion->z;
  Vector_AddPolarOffset__f0e150c(-0x240000, scene->angle, pos);
  Camera_WorldToScreen__f0e150c(pos);
  canvas->x = (s16)(pos[0] >> 16);
  canvas->y = (s16)(pos[2] >> 16);
  canvas->command = CANVAS_UPLOAD_AND_FADE3;
  work->timer++;
}

void Func_080e1f2c(void);

/// Effect 29 cast on a target: runs the effect. Nothing calls it; the
/// targeted dispatch has no case for animation 29.
void BattleEffect_CallEffect29(void) { Func_080e1f2c(); }

#include "io_reg.h"

// One moving point: a spark on the canvas (x and y only, counting delay) or
// a sprite's place in the world.

extern char ResourceId_BowDraw, ResourceId_FlamingArrow;

s32 BattleEffect_GetObjectPriority(struct Actor *object);

void Func_080e1f2c(void) {
  struct BowWork *work = HeapSlot_Alloc(92, sizeof(struct BowWork));
  struct BowScene *scene = Ram_HeapSlots->effectWork;
  u8 *event = Ram_HeapSlots->eventWork;
  struct CanvasSpriteWork *canvas;
  struct EffectSprite *sprite;
  struct BowMotion *motion;
  s32 pos[3];
  s32 arrowSlot;
  s32 bowSlot;
  s32 tile;
  u16 *palette;
  s32 blue;
  s32 shade;
  s32 i;

  BattleEffect_InitializeSharedScene();
  // The spark pointer and the sprite pointer are one variable.
  sprite = (struct EffectSprite *)work->sparks;
  for (i = 0; i < 32; i++) {
    ((struct BowMotion *)sprite)->delay = -i;
    sprite = (struct EffectSprite *)((struct BowMotion *)sprite + 1);
  }

  work->tiles = HeapSlot_Alloc(96, 0x1800);
  Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_BowDraw),
                        work->tiles);
  bowSlot = Resource_FindFreeEntry();
  tile = VramCache_Load(bowSlot, 0x1800, work->tiles);
  work->bowTile = tile;
  sprite = &work->bow;
  Func_080eaf98(sprite, 16, 16, 0x80000000, tile);
  sprite->oam.palette256 = 1;
  sprite->oam.palette = 0;
  sprite->oam.priority = BattleEffect_GetObjectPriority(scene->mainObject);
  // volatile keeps the depth store before the scale stores.
  *(volatile s16 *)&sprite->depth =
      BattleEffect_GetObjectDepth(scene->mainObject) + 1;
  *(volatile s32 *)&sprite->scaleX = 0x6666;
  *(volatile s32 *)&sprite->scaleY = 0x6666;
  sprite->angle = scene->angle;
  pos[0] = scene->mainObject->x;
  pos[2] = scene->mainObject->z;
  motion = &work->bowMotion;
  Vector_AddPolarOffset__f0e150c(0x80000, scene->angle, pos);
  motion->x = pos[0];
  motion->y = 0x100000;
  motion->z = pos[2];

  Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_FlamingArrow),
                        work->tiles);
  arrowSlot = Resource_FindFreeEntry();
  tile = VramCache_Load(arrowSlot, 0x100, work->tiles);
  sprite = &work->arrow;
  Func_080eaf98(sprite, 12, 8, 0x40000000, tile);
  sprite->oam.palette256 = 1;
  sprite->oam.palette = 0;
  sprite->oam.priority = BattleEffect_GetObjectPriority(scene->mainObject);
  *(volatile s16 *)&sprite->depth =
      BattleEffect_GetObjectDepth(scene->mainObject) + 2;
  *(volatile s32 *)&sprite->scaleX = 0;
  *(volatile s32 *)&sprite->scaleY = 0;
  sprite->angle = scene->angle;
  pos[0] = scene->mainObject->x;
  pos[2] = scene->mainObject->z;
  motion = &work->arrowMotion;
  Vector_AddPolarOffset__f0e150c(0x120000, scene->angle, pos);
  motion->x = pos[0];
  motion->y = 0x100000;
  motion->z = pos[2];
  HeapSlot_Release(96);

  Func_080eb824();
  canvas = Ram_HeapSlots->unkec[1];
  canvas->priority = BattleEffect_GetObjectPriority(scene->mainObject);
  canvas->depth = BattleEffect_GetObjectDepth(scene->mainObject) + 3;
  canvas->mode = CANVAS_SPRITE_256_COLORS;
  canvas->command = 0;
  canvas->angle = scene->angle;
  work->start[0] = scene->mainObject->x;
  work->start[1] = scene->mainObject->y;
  work->start[2] = scene->mainObject->z;
  blue = 31 << 10;
  work->target[0] = scene->mainObject->x;
  work->target[1] = scene->mainObject->y;
  work->target[2] = scene->mainObject->z;
  Vector_AddPolarOffset__f0e150c(0x500000, scene->angle, work->target);

  palette = (u16 *)&OBJ_PLTT_COLOR(14, 0);
  for (i = 0; i < 16; i++) {
    shade = i + 16;
    // A once-only block: the store stays ahead of the loop's increments.
    do {
      *palette = (shade << 5 | blue) | shade;
    } while (0);
    palette++;
  }

  Sound_PlayFar(130);
  wait(1);
  work->bowGrowing = 0;
  work->bowShrinking = 0;
  work->done = 0;
  work->result = -1;
  work->phase = 0;
  work->timer = 0;
  Task_Add(Func_080e1678, TASK_PASS_VBLANK);
  while (work->phase != 999)
    wait(1);
  Task_Remove(Func_080e1678);
  if (work->done != 0 && work->result != -1)
    event[0xd30] = work->result;
  Resource_ResetEntry(arrowSlot);
  Resource_ResetEntry(bowSlot);
  BattleEffect_PrepareBufferInterpolation();
  BattleEffect_EndCanvasSprite();
  HeapSlot_Release(92);
}

struct PointWork {
  u8 unk_0[0xcc0];
  s16 points[62];
  u16 unk_d3c;
  u16 unk_d3e;
  u16 unk_d40;
  u16 unk_d42;
};

extern const s8 Data_080f0f64[];

s32 Func_080dbda8(s32 x, s32 z, s32 layer);
s32 Func_080cda84(s32 id, struct Actor *object, s32 *position);

/// Looks at 21 map cells around the effect owner, at the tile offsets of
/// Data_080f0f64 (signed x, z byte pairs), on the owner's layer. Each cell of
/// kind 1, 2 or 3 that Func_080cda84 accepts for the effect's id is listed as a
/// tile in the event work's point list at 0xcc0, which ends with -1, -1; a
/// kind-1 cell also stores its kind in the heap slot 0x5c work's byte 0x1b34,
/// and the last kind-2 and kind-3 cells are kept in pixels, centred on their
/// tile, at 0xd3c and 0xd40.
void Func_080e2264(void) {
  u8 *effect = Ram_HeapSlots->effectWork;
  struct PointWork *event = Ram_HeapSlots->eventWork;
  u8 *work = Ram_HeapSlots->battleFxWork[0];
  s16 *points = event->points;
  struct Actor *owner = *(struct Actor **)(effect + 16);
  s32 position[3];
  s32 x, z;
  s32 kind;
  u32 i;
  s32 count = 0;

  work[0x1b34] = count;
  event->unk_d3c = -1;
  event->unk_d3e = -1;
  event->unk_d40 = -1;
  event->unk_d42 = -1;
  for (i = 0; i <= 20; i++) {
    x = owner->x + (Data_080f0f64[i * 2] << 20);
    z = owner->z + (Data_080f0f64[i * 2 + 1] << 20);
    kind = Func_080dbda8(x, z, owner->terrainId);
    // position[1] is never set: only x and z reach Func_080cda84.
    position[0] = x;
    position[2] = z;
    if (kind == 1 &&
        Func_080cda84(*(s16 *)(effect + 30), owner, position) != -1) {
      points[count * 2] = x / 0x100000;
      points[count * 2 + 1] = z / 0x100000;
      count++;
      work[0x1b34] = kind;
    }
    if (kind == 2 &&
        Func_080cda84(*(s16 *)(effect + 30), owner, position) != -1) {
      event->unk_d3c = x >> 16;
      event->unk_d3e = z >> 16;
      event->unk_d3c = (event->unk_d3c & 0xfff0) + 8;
      event->unk_d3e = (event->unk_d3e & 0xfff0) + 8;
      points[count * 2] = x / 0x100000;
      points[count * 2 + 1] = z / 0x100000;
      count++;
    }
    if (kind == 3 &&
        Func_080cda84(*(s16 *)(effect + 30), owner, position) != -1) {
      event->unk_d40 = x >> 16;
      event->unk_d42 = z >> 16;
      event->unk_d40 = (event->unk_d40 & 0xfff0) + 8;
      event->unk_d42 = (event->unk_d42 & 0xfff0) + 8;
      points[count * 2] = x / 0x100000;
      points[count * 2 + 1] = z / 0x100000;
      count++;
    }
  }
  points[count * 2] = -1;
  points[count * 2 + 1] = -1;
}

// Runs the field events an effect touched.

struct TouchOwner {
  u8 unk00[34];
  u8 layer;
};

struct TouchWork {
  u8 unk00[16];
  struct TouchOwner *owner;
  u8 unk14[4];
  s16 argument0;
  s16 argument1;
  u8 unk1c[2];
  s16 kind;
};

struct TouchCell {
  s16 x;
  s16 z;
};

s32 Func_080dbdc8(s32 x, s32 z, s32 layer);
/* VIEW(f0e150c): routine Field_RunPsynergyEvent as void Field_RunPsynergyEvent(s32 event, s32 argument0, s32 argument1); event_work.h has s32 Field_RunPsynergyEvent(struct PendingEvent *event, s32 mode, s32 target). */
extern void Field_RunPsynergyEvent__f0e150c(s32 event, s32 argument0, s32 argument1) __asm__("Field_RunPsynergyEvent");

// Only for the compiler: see the FAKEMATCH tag in Func_080e249c.
static inline s32 Index(s32 i) { return i; }

/// Resolves each cell in the event work's touched list to an event id at the
/// owner's layer, sorts the distinct ids and runs each one's event for the
/// effect's kind.
void Func_080e249c(void) {
  struct TouchWork *work = Ram_HeapSlots->effectWork;
  struct TouchCell *cells =
      (struct TouchCell *)((u8 *)Ram_HeapSlots->eventWork + 0xcc0);
  struct TouchOwner *owner = work->owner;
  s32 count = 0;
  s16 ids[24];
  s32 i, j;

  for (i = 0; cells[i].x != -1; i++) {
    s16 id = Func_080dbdc8(cells[i].x << 20, cells[i].z << 20, owner->layer);

    for (j = 0; j < count; j++) {
      if (ids[j] == id)
        break;
    }
    if (j == count)
      ids[count++] = id;
  }
  ids[count] = -1;

  if (count > 1) {
    for (i = 0; i < count - 1; i++) {
      for (j = i + 1; j < count; j++) {
        if (ids[i] > ids[j]) {
          s16 swap = ids[i];

          ids[i] = ids[j];
          ids[j] = swap;
        }
      }
    }
  }

  for (i = 0; ids[i] != -1; i++) {
    /* FAKEMATCH(wrapper): ids[Index(i)], through an inline that returns i,
     * so the id is read before the kind; as ids[i], 18 of 160 instructions
     * differ in tla-en. */
    s32 event = Func_080ce458(0x50000005, work->kind, ids[Index(i)]);

    if (event != 0)
      Field_RunPsynergyEvent__f0e150c(event, work->argument0, work->argument1);
  }
}

struct TouchLayer {
  u8 unk00[8];
  s32 x;
  s32 z;
  u8 unk10[40];
};

struct TouchMap {
  u8 unk000[264];
  struct TouchLayer layers[1];
};

struct TouchCopy {
  u8 toX;
  u8 toZ;
  u8 fromX;
  u8 fromZ;
  u8 cellX;
  u8 cellZ;
  u8 unk06[2];
};

/* VIEW(f0e150c): tag struct MapCell as struct MapCell__f0e150c; map.h has another definition. */
struct MapCell__f0e150c {
  u8 unk00[2];
  u8 unk02;
};

struct TouchCopies {
  struct TouchCell unk0000[1491];
  struct TouchCopy copies[64];
  s8 count;
};

extern struct TouchCopies gData_02001000;

s32 Party_GetLeaderActor(void);
struct Actor *Actor_GetById(u32 objectId);
void Func_080201f0(s32 toX, s32 toZ, s32 width, s32 height, s32 fromX,
                   s32 fromZ);
struct MapCell__f0e150c *Func_080dbb78(s32 x, s32 z, s32 layer);

void Func_080e25e8(s32 toX, s32 toZ, s32 width, s32 height, s32 originX,
                   s16 originZ, s32 layerIndex) {
  u8 *event = Ram_HeapSlots->eventWork;
  struct Actor *owner = Actor_GetById(Party_GetLeaderActor());
  struct TouchCell *cell = (struct TouchCell *)(event + 0xcc0);
  s32 ownerHeight = owner->terrainHeight >> 20;
  struct TouchMap *map = (struct TouchMap *)Ram_HeapSlots->mapWork;
  struct TouchLayer *layer = &map->layers[layerIndex];
  s32 i;

  for (i = 0; cell[i].x != -1; i++) {
    s32 x = cell[i].x - originX;
    s32 z = cell[i].z - originZ - ownerHeight;

    x += layer->x >> 20;
    z += layer->z >> 20;

    if (x >= 0 && x < width && z >= 0 && z < height) {
      s32 n;

      Func_080201f0(toX + x, toZ + z, 1, 1, originX + x, originZ + z);
      Func_080dbb78(cell[i].x << 20, cell[i].z << 20, 2)->unk02 = 0;
      n = gData_02001000.count;
      if (n < 64) {
        gData_02001000.copies[n].toX = toX + x;
        gData_02001000.copies[n].toZ = toZ + z;
        gData_02001000.copies[n].fromX = originX + x;
        gData_02001000.copies[n].fromZ = originZ + z;
        gData_02001000.copies[n].cellX = cell[i].x;
        gData_02001000.copies[n].cellZ = cell[i].z;
        gData_02001000.count++;
      }
    }
  }
}
#undef BOW_NEXT_PHASE

// ---- games/tla/src/field/event_commands/f0e2718.c ----
// Places the event work's listed blocks.

#include "map.h"

/* VIEW(f0e2718): variable gData_02001000 as extern u8 gData_02001000[]; f0e150c has extern struct TouchCopies gData_02001000. */
extern u8 gData_02001000__f0e2718[] __asm__("gData_02001000");
void Func_080201f0(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
/* VIEW(f0e2718): routine Func_080dbb78 as struct MapCell *Func_080dbb78(s32 x, s32 z, s32 layer); f0e150c has struct MapCell__f0e150c *Func_080dbb78(s32 x, s32 z, s32 layer). */
extern struct MapCell *Func_080dbb78__f0e2718(s32 x, s32 z, s32 layer) __asm__("Func_080dbb78");

/// For each entry of the event work's list at 0x174c, calls Func_080201f0 with
/// its bytes and clears the collision code of the cell at its position.
void Func_080e2718(void) {
  u8 *base = gData_02001000__f0e2718;
  s32 i;

  for (i = 0; i < (s8)gData_02001000__f0e2718[0x194c]; i++) {
    u8 *entry = base + 0x174c + i * 8;

    Func_080201f0(entry[0], entry[1], 1, 1, entry[2], entry[3]);
    Func_080dbb78__f0e2718(entry[4] << 20, entry[5] << 20, 2)->collisionCode = 0;
  }
}
