/// @file
/// The radial object: the radial effects, then in ROM order the tile copies,
/// the field Psynergy (parts under psynergy/), the actors' cells, the step
/// vectors and the world and World Map pictures.

/// @file
/// What every part of the radial object shares: its includes. radial.c
/// includes it before the parts, and each part includes it too.

// world_map.h first: battle.h names struct WorldMapWork, and a struct declared
// before its definition keeps a smaller alignment, which world_picture.c's
// copies of it show.
#include "world_map.h"
#include "iwram_call.h"
#include "objdisp.h"
#include "system.h"
#include "fixed_math.h"
#include "types.h"
#include "motobj.h"
#include "scene_tables.h"
#include "field_effect.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "actor.h"
#include "ram_buffer.h"
#include "field_psynergy_work.h"
#include "sprite.h"
#include "field_event.h"
#include "field_particle.h"
#include "actor_view.h"
#include "object_lookup.h"
#include "main_routine_work.h"
#include "effect_sprite.h"
#include "vector3.h"
#include "graphics/resource.h"
#include "system/task.h"
#include "system/heap_release.h"
#include "scene.h"
#include "frame.h"
#include "scene_ids.h"
#include "resource.h"
#include "vram_tab.h"
#include "arc_effect.h"
#include "heap_release.h"
#include "task.h"
#include "canvas_sprite.h"
#include "io_reg.h"
#include "map.h"

// The parts, in ROM order.
#include "module/field/event_commands.h"

extern const s32 gBattleEffect_SharedParticleScript[];

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

/* VIEW(radial): routine FieldEffect_AdvanceMotion as void
 * FieldEffect_AdvanceMotion(struct FieldEffect *effect);
 * module/field/field_effect.h has void FieldEffect_AdvanceMotion(union
 * FieldObject *object); not tried: FieldEffect_AdvanceMotion's bound
 * declaration was not found. */
extern void Effect_AdvanceMotion__radial(struct FieldEffect *effect) __asm__(
    "FieldEffect_AdvanceMotion");

void FieldEffect_SpawnRadialParticleRing(struct MotionObject *origin) {
  struct EffectOptions options;
  s32 velocity[3];
  s32 sine;
  s32 angle;
  u32 i;

  options.palette = 0;
  options.update = Effect_AdvanceMotion__radial;
  options.startScaleX = 0xCCCC;
  options.startScaleY = 0xCCCC;
  i = 0;
  do {
    angle = i << 12;
    velocity[0] = Trig_Cos(angle) * 3 / 2;
    velocity[1] = 0;
    sine = Trig_Sin(angle);
    velocity[2] = sine;
    FieldEffect_SpawnResident(origin->x, origin->y, origin->z, velocity[0],
                              velocity[1], sine, 0x01090001, &options);
    i++;
  } while (i <= 16);
}

void FieldEffect_WanderAroundAnchor(struct OrbitEffect *effect) {
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
/// (FieldEffect_WanderAroundAnchor) with upward speed and gravity. As The
/// Broken Seal's FieldEffect_CircleAnchor.
void FieldEffect_CircleAnchor(struct OrbitEffect *effect) {
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
    effect->update = FieldEffect_WanderAroundAnchor;
    effect->timer = 0;
    effect->pause = 0;
    effect->speed = 0x1999;
    effect->scale = 0x30000;
    effect->angle = Random16();
  }
}

#define Actor_SetPositionAndResetMotionFar                                     \
  ((s32 (*)(void *, s32, s32, s32))Actor_SetPositionAndResetMotionFar)

void FieldEffect_ShrinkActorScaleUntilHalf(struct Actor *object) {
  s32 scale;

  scale = object->scaleX - 0x80;
  object->scaleY = scale;
  object->scaleX = scale;
  if (scale < 0x8000) {
    Actor_SetPositionAndResetMotionFar(object, 0, 0, 0);
    object->update = 0;
  }
}

void FieldPsynergy_WhirlwindOnTarget(void) { FieldPsynergy_Whirlwind(); }
#undef Actor_SetPositionAndResetMotionFar
#include "module/field/field.h"
#include "module/field/object.h"
#include "module/field/radial.h"

// An orbiting mote: radial.c's OrbitEffect.

void Vector_AddPolarOffset(s32 radius, s32 angle, s32 *position);
/* VIEW(f0dfd00): routine FieldEffect_WanderAroundAnchor as void
 * FieldEffect_WanderAroundAnchor(struct OrbitEffect__f0dfd00 *mote); the
 * definition has void FieldEffect_WanderAroundAnchor(struct OrbitEffect
 * *effect); not tried: FieldEffect_WanderAroundAnchor's bound declaration was
 * not found. */
extern void BattleEffect_WanderAroundAnchor__f0dfd00(
    struct OrbitEffect *mote) __asm__("FieldEffect_WanderAroundAnchor");
/* VIEW(f0dfd00): routine FieldEffect_CircleAnchor as void
 * FieldEffect_CircleAnchor(struct OrbitEffect__f0dfd00 *mote); the definition
 * has void FieldEffect_CircleAnchor(struct OrbitEffect *effect); not tried:
 * FieldEffect_CircleAnchor's bound declaration was not found. */
extern void BattleEffect_CircleAnchor__f0dfd00(
    struct OrbitEffect *mote) __asm__("FieldEffect_CircleAnchor");
void BattleEffect_AgeAndCueAtEighty(struct TimedObject *object);

#define ORB_OBJECT 280
#define MOTE_OBJECT 308
#define SE_ORB_ARRIVES 207
#define SE_ORB_FADES 136

// The two targets ⚓️ treats apart, by their first animation.
#define TARGET_ANIMATION_A 372
#define TARGET_ANIMATION_B 373

// Animation 14: carry an orb from the caster to the scene's position over
// ten frames, growing it from a quarter size, then play out the kind.
void FieldPsynergy_Whirlwind(void) {
  struct BattleFxScene *scene = Ram_HeapSlots->effectWork;
  struct Actor *caster = scene->mainObject;
  struct Actor *target = scene->child;
  struct OrbTargetSprite *sprite = target->sprite;
  struct EffectVector position;
  struct EffectVector from;
  struct EffectVector to;
  struct Actor *orb;
  struct OrbitEffect *mote;
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

  FieldPsynergy_FadeToCastColor();
  Actor_SelectAnimationFar(orb, 2);
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
    Actor_SelectAnimationFar(orb, 1);
    wait(30);
    Sound_PlayFar(SE_ORB_FADES);
    Actor_WaitForTrackEndFar(orb);
    Actor_ReleaseFar(orb);
  } else if (kind == ORB_ORBITING) {
    // Sixteen motes, one every six frames, 4 pixels above the orb and 0.8
    // higher each, 3 to 8 pixels out, circle it while the target shrinks.
    wait(20);
    if (target != 0)
      target->update = (void (*)(void))FieldEffect_ShrinkActorScaleUntilHalf;
    for (i = 0; i <= 15; i++) {
      s32 distance;

      position.x = orb->x;
      position.y = orb->y + i * 0xcccc + (4 << 16);
      position.z = orb->z;
      distance = Random16() * 5 + (3 << 16);
      Vector_AddPolarOffset(distance, Random16(), &position);
      mote = (struct OrbitEffect *)Actor_Spawn(MOTE_OBJECT, position.x,
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
    Actor_SelectAnimationFar(orb, 1);
    wait(30);
    Sound_PlayFar(SE_ORB_FADES);
    Actor_WaitForTrackEndFar(orb);
    Actor_ReleaseFar(orb);
  } else if (kind == ORB_WANDER || kind == ORB_HANDOFF) {
    wait(5);
    Actor_SelectAnimationFar(orb, 1);
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
        mote = (struct OrbitEffect *)Actor_Spawn(MOTE_OBJECT, position.x,
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
    BattleEffect_SetObjectUpdateMode(scene->mainObject, 0);
    FieldPsynergy_FadeFromCastColor();
    ((struct OrbitEffect *)orb)->timer = 0;
    orb->update = (void (*)(void))BattleEffect_AgeAndCueAtEighty;
    HangingVine_Swing(scene->secondObjectId);
    Actor_WaitForTrackEndFar(orb);
    Actor_ReleaseFar(orb);
  }
  FieldPsynergy_FadeFromCastColor();
}
#undef ORB_OBJECT
#undef MOTE_OBJECT
#undef SE_ORB_ARRIVES
#undef SE_ORB_FADES
#undef TARGET_ANIMATION_A
#undef TARGET_ANIMATION_B

void BattleEffect_AgeAndCueAtEighty(struct TimedObject *object) {
  if (object->age == 80) {
    object->update = 0;
    Sound_PlayFar(136);
  }
  object->age++;
}

void FieldEffect_UpdateHaltParticle(struct FieldParticle *effect) {
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
    Vector_AddPolarOffset(Random16() * 30 + (40 << 16), (u16)angle, &pos);
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
    Vector_AddPolarOffset(4 << 16, Random16(), &pos);
    effect->targetX = pos.x;
    effect->targetZ = pos.z;
    effect->maxTurnStep = 0x1000;
    effect->limitTurnStep = 1;
    *st = *st + 1;
    return;
  }
  if (state == 3 && FieldParticle_HasPendingTarget(effect) == 0)
    FieldEffect_ClearOwnedSlot(effect);
}

/// Pauses the scene's child (byte 91) and runs the fallback transition.
void FieldPsynergy_HaltOnTarget(void) {
  (*(u8 **)((u8 *)Ram_HeapSlots->effectWork + 20))[91] = 1;
  FieldPsynergy_Halt();
}

void FieldPsynergy_Halt(void) {
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
  Actor_SelectAnimationFar(obj, 0);
  if (obj == 0)
    return;

  FieldPsynergy_FadeToCastColor();
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
  Actor_SelectAnimationFar(obj, 1);
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
    Vector_AddPolarOffset(4 << 16, Random16(), &pos);
    FieldParticle_Initialize(slot, 673, pos.x, pos.z);
    FieldParticle_SetCallback(slot, FieldEffect_UpdateHaltParticle);
    Animation_ApplyChildValuesToRecordFar(slot->object, 7);
    slot++;
  }

  pos.x = obj->x;
  pos.y = obj->y + (8 << 16);
  pos.z = obj->z;
  wait(8);
  Actor_ReleaseFar(obj);
  wait(4);
  wait(30);
  FieldPsynergy_FadeFromCastColor();
}

#define Actor_SetAnimationSpeedFar                                             \
  ((void (*)(void *, s32))Actor_SetAnimationSpeedFar)

void FieldPsynergy_UpdateHaltedActor(struct ActorView *object) {
  s8 *table = gBattleEffect_RandomChildValues;
  s32 index = Random16();
  Actor_SetSpriteColorModeFar(object, table[(u32)(index * 8) >> 16]);
}

void FieldPsynergy_HaltActor(s32 id) {
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
    object->update = FieldPsynergy_UpdateHaltedActor;
    object->value5b = 1;
    Actor_SetAnimationSpeedFar(object, 0);
  }
}

void FieldPsynergy_ResumeHaltedActor(s32 id) {
  struct ActorView *object = ObjectTable_Get(id);

  if (object != NULL) {
    if (object->update == FieldPsynergy_UpdateHaltedActor) {
      object->update = (void *)gMainRoutineWork.savedCallback;
      gMainRoutineWork.savedCallback = 0;
      Actor_SetSpriteColorModeFar(object, (s8)gMainRoutineWork.savedByte);
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
s32 FieldPsynergy_FilterHaltTarget(s32 id) {
  if (BattleEffect_GetFlags() == 0xff)
    return -1;
  return id;
}

/// Runs field effect 13 (FieldPsynergy_Catch) for the request dispatcher.
void FieldPsynergy_CatchOnTarget(void) { FieldPsynergy_Catch(); }

static __inline__ s32 Interpolate(s32 from, s32 to, s32 step) {
  return from + (to - from) * step / 10;
}

void FieldPsynergy_Catch(void) {
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
    Vector_AddPolarOffset(0x200000, scene->angle, &target.x);
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
  FieldPsynergy_FadeToCastColor();
  Sound_PlayFar(138);
  object->facing = mainObject->facing;
  object->speedLimit = 0x14ccc;
  object->motionFlags = 0;
  Actor_SelectAnimationFar(object, 5);
  Actor_SetSpriteColorModeFar(object, 1);
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
  Actor_SelectAnimationFar(object, 6);
  wait(15);
  for (index = 0; index < 10; index++) {
    object->y -= 0x20000;
    wait(1);
  }
  Actor_SelectAnimationFar(object, 5);
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
  Actor_ReleaseFar(object);
  FieldPsynergy_FadeFromCastColor();
}

#define EffectSprite_DrawAtWorldPosition EffectSprite_DrawAtWorldPosition
void Rope_GetEndNodePosition(s32 target, struct Vector3 *position);

/// Task of the Lash: its scattering particles along the rope.
void FieldPsynergy_UpdateLash(void) {
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
    Vector_AddPolarOffset(distance, Random16(), particle);
    particle->velocityX = 0;
    particle->velocityY = 0x20000 - Random16() * 2;
    particle->velocityZ = 0;
    speed = Random16() + 0x18000;
    Vector_AddPolarOffset(speed, Random16(), &particle->velocityX);
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
    Rope_GetEndNodePosition(event->target, &position);
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
    Actor_SelectAnimationFar(object, 3);
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

/* VIEW(radial): tag struct EffectSceneWork as struct EffectSceneWork__radial;
 * module/field/event_commands.h has another definition; tried with the
 * definition the rest of the object has, it does not compile in tla-ja:
 * games/tla/src/field/event_commands/radial.c:760: structure has no member
 * named `failed'. */
struct EffectSceneWork__radial {
  u8 unk00[0x20];
  u8 failed;
};

/// Effect 17 on a target: asks Rope_FindById for two results; on failure
/// marks the effect failed, else stores the target and both in the event work.
/// Then runs FieldPsynergy_Lash.
void FieldPsynergy_LashOnTarget(s32 value) {
  struct EffectSceneState *state = Ram_HeapSlots->eventWork;
  struct EffectSceneWork__radial *work = Ram_HeapSlots->effectWork;
  s32 second;
  s32 first;

  if (Rope_FindById(value, &first, &second)) {
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
  FieldPsynergy_Lash();
}

extern char ResourceId_StarSparkleFade;

void FieldPsynergy_Lash(void) {
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
  FieldPsynergy_FadeToCastColor();
  object = Actor_Spawn(363, 0, 0, 0);
  if (object != 0) {
    object->facing = caster->facing;
    object->motionFlags = 0;
    if (caster->priorityFlags & 0x20)
      object->priorityFlags |= 0x20;
    Actor_SelectAnimationFar(object, 0);
    Sound_PlayFar(138);
    Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_SparkleFade),
                        work);
    slotA = VramCache_FindFreeEntry();
    tileA = VramCache_Load(slotA, 0x100, work);
    work->tileA = tileA;
    Decompress_TaggedLz(
        Resource_GetTableEntry((s32)&ResourceId_StarSparkleFade), work);
    slotB = VramCache_FindFreeEntry();
    work->tileB = VramCache_Load(slotB, 0x100, work);

    particle = work->particles;
    sprite = work->sprites;
    for (i = 63; i >= 0; i--) {
      EffectSprite_Init(sprite, 4, 4, 0, tileA);
      sprite->depth = 250;
      sprite->oam.priority = 0;
      sprite->oam.palette256 = 1;
      sprite->oam.palette = 0;
      particle->x = object->x;
      particle->y = object->y;
      particle->z = object->z;
      Vector_AddPolarOffset(0xc0000, Random16(), particle);
      particle->timer = -1;
      sprite++;
      particle++;
    }

    FieldPsynergy_GrowFromCaster(object, scene->x, scene->y + 0x200000,
                                 scene->z);
    Actor_SelectAnimationFar(object, 1);
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
      Task_Add(FieldPsynergy_UpdateLash, 0x480);
      while (work->phase != 999) {
        if (work->flash == 1) {
          Sound_PlayFar(0x200);
          Rope_Lash(event->target, event->targetY, event->targetZ);
          work->hit = 1;
          Sound_PlayFar(395);
        }
        wait(1);
      }
      Task_Remove(FieldPsynergy_UpdateLash);
    }
    VramCache_FreeEntry(slotA);
    VramCache_FreeEntry(slotB);
    Actor_ReleaseFar(object);
  }
  FieldPsynergy_FadeFromCastColor();
  HeapSlot_Release(92);
}

void FieldEffect_UpdateSpinAngle(struct Actor *object) {
  u32 step;

  step = object->timer * 0x50;
  object->facing += step + 0x1000;
  if (step < 0x1000)
    object->timer++;
}

/// Per-frame update: spins 1/8 turn a frame as it shrinks, rising a pixel a
/// frame; released once its scale falls below 0x3000.
// As ☀️'s FieldEffect_UpdateDescendingObject.
void FieldEffect_UpdateRetreatCaster(struct Actor *obj) {
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

void FieldEffect_UpdateRadialLaunch(struct FieldParticle *slot) {
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
    Vector_AddPolarOffset(
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
      FieldEffect_ClearOwnedSlot(slot);
  }
}
#include "event.h"
#include "event_work.h"
#include "scene_call.h"

#define SLOT_MAX 24

struct SpinScene {
  u8 unk00[0x10];
  struct SpinObject *mainObject;
  u8 unk14[4];
  s16 firstObjectId;
  u8 unk1a[0x36];
  struct FieldParticle slots[SLOT_MAX];
};

/* VIEW(launch): routine Actor_SetSpriteColorModeFar as void
 * Actor_SetSpriteColorModeFar(void *object, s32 value); actor_view.h has void
 * Actor_SetSpriteColorModeFar(struct Actor *, u32); not tried:
 * Actor_SetSpriteColorModeFar's bound declaration was not found. */
extern void Animation_ApplyChildValuesFar__launch(
    void *object, s32 value) __asm__("Actor_SetSpriteColorModeFar");
/* VIEW(launch): routine Actor_SetSpriteFlagsFar as void
 * Actor_SetSpriteFlagsFar(void *object, s32 flags); actor_view.h has void
 * Actor_SetSpriteFlagsFar(struct Actor *object, u32 flags); not tried:
 * Actor_SetSpriteFlagsFar's bound declaration was not found. */
extern void Actor_SetSingleChildFlagsFar__launch(
    void *object, s32 flags) __asm__("Actor_SetSpriteFlagsFar");
void Actor_ApplyRandomChildValues(struct SpinObject *owner);
/* VIEW(launch): routine FieldEffect_UpdateSpinAngle as void
 * FieldEffect_UpdateSpinAngle(struct SpinObject *object); the definition has
 * void FieldEffect_UpdateSpinAngle(struct Actor *object); not tried:
 * FieldEffect_UpdateSpinAngle's bound declaration was not found. */
extern void BattleEffect_AdvanceAngleWithRamp__launch(
    struct SpinObject *object) __asm__("FieldEffect_UpdateSpinAngle");
/* VIEW(launch): routine FieldEffect_UpdateRetreatCaster as void
 * FieldEffect_UpdateRetreatCaster(struct SpinObject *obj); the definition has
 * void FieldEffect_UpdateRetreatCaster(struct Actor *obj); not tried:
 * FieldEffect_UpdateRetreatCaster's bound declaration was not found. */
extern void BattleEffect_UpdateDescendingObject__launch(
    struct SpinObject *obj) __asm__("FieldEffect_UpdateRetreatCaster");

void FieldPsynergy_Retreat(void) {
  struct SpinScene *scene = Ram_HeapSlots->effectWork;
  struct SpinObject *obj = scene->mainObject;
  struct SpinVector pos;
  struct FieldParticle *slot;
  struct FieldParticle *slots;
  u8 last;
  u32 i;

  Event_Start();
  FieldCamera_MoveWithinMapBounds(-1, -1, -1, 0);
  FieldPsynergy_FadeToCastColor();
  wait(10);
  Actor_ArmCallback(scene->firstObjectId, 0x4000, 0);
  wait(30);
  obj->update = Actor_ApplyRandomChildValues;
  Sound_PlayFar(131);
  Actor_SelectAnimationFar(obj, 28);
  wait(40);

  Sound_PlayFar(220);
  Animation_ApplyChildValuesFar__launch(obj, 0);
  Actor_SelectAnimationFar(obj, 3);
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
  Camera_WorldToScreen(&pos);
  i = 0;
  slot = scene->slots;
  slots = slot;
  do {
    s32 scale;

    FieldParticle_Initialize(slot, 0x2a0, pos.x, pos.z);
    FieldParticle_SetCallback(slot, FieldEffect_UpdateRadialLaunch);
    FieldParticle_SetActorMode(slot, 7);
    Animation_ApplyChildValuesToRecordFar(slot->object, (Random16() * 7) >> 16);
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
  FieldPsynergy_FadeFromCastColor();
  obj->fall = 0;
  obj->y = obj->floor;
  wait(10);
}

void FieldEffect_UpdateDescendingParticlePositiveArc(struct Actor *particle) {
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
    Actor_ReleaseFar(particle);
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

void FieldEffect_UpdateDescendingParticleNegativeArc(struct Actor *particle) {
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
    Actor_ReleaseFar(particle);
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

/* VIEW(radial): routine Actor_CreateFar as struct ArcObject
 * *Actor_CreateFar(s32 kind, s32 x, s32 y, s32 z); gamepak/far_call/field.h has
 * struct Actor *Actor_CreateFar(s32 kind, s32 x, s32 y, s32 z); tried as the
 * rest of the object declares it, it does not compile in tla-ja (make modern):
 * games/tla/src/field/event_commands/radial.c:79:13: note:
 * 'Effect_AdvanceMotion__radial' declared here. */
struct ArcObject *Actor_CreateFar__radial(s32 kind, s32 x, s32 y,
                                          s32 z) __asm__("Actor_CreateFar");
#define Actor_CreateFar Actor_CreateFar__radial

void FieldEffect_SpawnDescendingArcParticles(struct ArcObject *source) {
  u8 *work = Ram_HeapSlots->effectWork;
  struct ArcObject *leader = *(struct ArcObject **)(work + 16);
  struct ArcObject *objects[2];
  struct ArcObject *object;
  struct ArcSprite *sprite;
  s32 i;
  u8 zero;

  for (i = 0; i <= 1; i++) {
    object = Actor_CreateFar(271, source->x, source->y + 0x400000, source->z);
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
    VramCache_FreeEntry(sprite->resource);
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
      (void *)FieldEffect_UpdateDescendingParticleNegativeArc;
  objects[0]->sprite->attr.byte.variant = 0;
  objects[1]->callback =
      (void *)FieldEffect_UpdateDescendingParticlePositiveArc;
  objects[1]->sprite->attr.byte.variant = leader->sprite->attr.byte.variant;
}
#include "flag_call.h"
#include "gamepak/far_call/window.h"
#include "text/msg_ids.h"

/// The caster during Avoid (FieldPsynergy_Avoid): sheds a pair of falling
/// arcs every 5 frames (7 in scene Ov64a), and spins in phase 1.
// As ☀️'s function of the same name.
void FieldEffect_UpdateAvoidCaster(struct ArcEffectObject *caster) {
  s16 phase;
  s32 elapsedFrames;
  struct MainRoutineWork *party;

  phase = caster->phase;
  elapsedFrames = (s16)caster->step++;
  party = &gMainRoutineWork;

  if (party->map == (s32)&SceneId_Ov64a) {
    if (elapsedFrames % 7 == 0)
      FieldEffect_SpawnDescendingArcParticles(caster);
  } else if (elapsedFrames % 5 == 0) {
    FieldEffect_SpawnDescendingArcParticles(caster);
  }

  if (phase == 1)
    caster->angle += 0xc00;
}

void FieldPsynergy_ShowAvoidWoreOff(void) {
  gMainRoutineWork.avoidDistance = 0;
  WorldMap_Freeze();
  if (gMainRoutineWork.avoidByItem == 0) {
    MessageArgument_PushFar(150, 4);
    FieldMessage_ShowFar((s32)&MsgAbilityWoreOff, 1);
  } else {
    MessageArgument_PushFar(236, 2);
    FieldMessage_ShowFar((s32)&MsgItemWoreOff, 1);
  }
  WorldMap_ReloadPendingAreaGraphics();
}

/// Psynergy effect 16: the user spins, rains arc particles, then flashes.
void FieldPsynergy_Avoid(void) {
  struct ArcScene *scene;
  struct ArcEffectObject *object;
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
  slot = VramCache_FindFreeEntry();
  scene->arcTileSlot = slot;
  // tiles holds the picture's address, loaded between the slot's two shifts;
  // as the address in the call, 2 of 156 instructions differ in tla-en.
  tiles = gFieldEffect_ArcSparkTiles;
  VramCache_Load((s16)slot, 0x100, tiles);
  gMainRoutineWork.avoidDistance = 0x09600000;
  gMainRoutineWork.avoidByItem = GameFlag_Get(0x145);
  Animation_ApplyChildValuesFar__launch(object, 0);
  object->callback = FieldEffect_UpdateAvoidCaster;
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

  VramCache_FreeEntry(scene->arcTileSlot);
  FieldMessage_ShowFar((s32)&MsgMonstersAttackLess, 1);
}

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
    Actor_SetSpriteColorModeFar(object, 7);
  } else {
    Actor_SetSpriteColorModeFar(object, 0);
  }
  if ((gFrameCount & 15) == 0) {
    BattleEffect_SpawnScaledArcObjects(object);
  }
}

void BattleEffect_CycleObjectAndSpawnArcs(struct Actor *object) {
  if (gFrameCount & 1)
    Actor_SetSpriteColorModeFar(object, (gFrameCount >> 1) % 6);
  if ((gFrameCount & 15) == 0)
    BattleEffect_SpawnScaledArcObjects(object);
}

void BattleEffect_SetObjectUpdateMode(struct Actor *object, s32 mode) {
  if (mode == 0) {
    object->update = 0;
    Actor_SetSpriteColorModeFar(object, 0);
  } else if (mode == 1) {
    object->update = (void (*)(void))BattleEffect_FlickerObjectAndSpawnArcs;
  } else {
    object->update = (void (*)(void))BattleEffect_CycleObjectAndSpawnArcs;
  }
}

// The arc effect's next pair of scaled sprites.

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
    EffectSprite_Init(sprite, 16, 16, 0x80004000, tiles);
    sprite->oam.palette256 = 0;
    sprite->oam.palette = 0;
    sprite->oam.priority = BattleEffect_GetObjectPriority(target);
  }
  work->age[group] = 0;
  work->target = target;
  work->group ^= 1;
}

/* VIEW(f0e150c): tag struct ArcSprite as struct ArcSprite__f0e150c; radial.c
 * has another definition; tried with the definition the rest of the object has,
 * it does not compile in tla-ja:
 * games/tla/src/field/event_commands/radial.c:1374: structure has no member
 * named `x'. */
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

void BattleEffect_UpdateArcEffect(void) {
  struct ArcWork *work = Ram_HeapSlots->block62;
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
      Camera_WorldToScreen(position);
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
        EffectSprite_Draw(sprite);
      }
      work->age[group]++;
    }
  }
}

void BattleEffect_StartArcEffect(void) {
  struct ArcEffectWork *work = HeapSlot_Alloc(248, sizeof(*work));

  work->resource = VramCache_FindFreeEntry();
  VramCache_Load(work->resource, 0x100, gBattleEffect_ArcTiles);
  work->group = 0;
  work->age[0] = 999;
  work->age[1] = 999;
  work->target = 0;
  Task_Add((s32)BattleEffect_UpdateArcEffect, TASK_PASS_VBLANK);
}

void BattleEffect_EndArcEffect(void) {
  struct ArcEffectWork *work = Ram_HeapSlots->block62;

  VramCache_FreeEntry(work->resource);
  Task_Remove((u32)BattleEffect_UpdateArcEffect);
  HeapSlot_Release(248);
}

#define BOW_NEXT_PHASE(work) ((work)->phase++, (work)->timer = -1)

/// Task of field Psynergy 29 (its name is question marks): the bow and the
/// flaming arrow.
void FieldPsynergy_UpdateUnnamed29(void) {
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
      Vector_AddPolarOffset(0x2666, scene->angle + 0x8000, pos);
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
      Vector_AddPolarOffset(0x50000, scene->angle, pos);
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
    if (MapGrid_CheckCollisionFar(scene->mainObject, pos) > 0) {
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
          FieldPsynergy_CheckOverlap(&motion->x, 2, &object->x,
                                     object->radius - 2) >= 0)
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
    Vector_AddPolarOffset(0x20000, scene->angle, pos);
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
  Camera_WorldToScreen(pos);
  sprite->x = pos[0];
  sprite->y = pos[2];
  EffectSprite_Draw(sprite);
  motion = &work->arrowMotion;
  pos[0] = motion->x;
  pos[1] = motion->y;
  pos[2] = motion->z;
  Camera_WorldToScreen(pos);
  sprite = &work->arrow;
  sprite->x = pos[0];
  sprite->y = pos[2];
  EffectSprite_Draw(sprite);
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
  Vector_AddPolarOffset(-0x240000, scene->angle, pos);
  Camera_WorldToScreen(pos);
  canvas->x = (s16)(pos[0] >> 16);
  canvas->y = (s16)(pos[2] >> 16);
  canvas->command = CANVAS_UPLOAD_AND_FADE3;
  work->timer++;
}

/// Effect 29 cast on a target: runs the effect. Nothing calls it; the
/// targeted dispatch has no case for animation 29.
void BattleEffect_CallEffect29(void) { FieldPsynergy_Unnamed29(); }

// One moving point: a spark on the canvas (x and y only, counting delay) or
// a sprite's place in the world.

extern char ResourceId_BowDraw, ResourceId_FlamingArrow;

void FieldPsynergy_Unnamed29(void) {
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

  FieldPsynergy_FadeToCastColor();
  // The spark pointer and the sprite pointer are one variable.
  sprite = (struct EffectSprite *)work->sparks;
  for (i = 0; i < 32; i++) {
    ((struct BowMotion *)sprite)->delay = -i;
    sprite = (struct EffectSprite *)((struct BowMotion *)sprite + 1);
  }

  work->tiles = HeapSlot_Alloc(96, 0x1800);
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_BowDraw),
                      work->tiles);
  bowSlot = VramCache_FindFreeEntry();
  tile = VramCache_Load(bowSlot, 0x1800, work->tiles);
  work->bowTile = tile;
  sprite = &work->bow;
  EffectSprite_Init(sprite, 16, 16, 0x80000000, tile);
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
  Vector_AddPolarOffset(0x80000, scene->angle, pos);
  motion->x = pos[0];
  motion->y = 0x100000;
  motion->z = pos[2];

  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_FlamingArrow),
                      work->tiles);
  arrowSlot = VramCache_FindFreeEntry();
  tile = VramCache_Load(arrowSlot, 0x100, work->tiles);
  sprite = &work->arrow;
  EffectSprite_Init(sprite, 12, 8, 0x40000000, tile);
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
  Vector_AddPolarOffset(0x120000, scene->angle, pos);
  motion->x = pos[0];
  motion->y = 0x100000;
  motion->z = pos[2];
  HeapSlot_Release(96);

  BattleEffect_StartCanvasSprite();
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
  Vector_AddPolarOffset(0x500000, scene->angle, work->target);

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
  Task_Add(FieldPsynergy_UpdateUnnamed29, TASK_PASS_VBLANK);
  while (work->phase != 999)
    wait(1);
  Task_Remove(FieldPsynergy_UpdateUnnamed29);
  if (work->done != 0 && work->result != -1)
    event[0xd30] = work->result;
  VramCache_FreeEntry(arrowSlot);
  VramCache_FreeEntry(bowSlot);
  FieldPsynergy_FadeFromCastColor();
  BattleEffect_EndCanvasSprite();
  HeapSlot_Release(92);
}

/// Looks at 21 map cells around the effect owner, at the tile offsets of
/// Data_080f0f64 (signed x, z byte pairs), on the owner's layer. Each cell of
/// kind 1, 2 or 3 that Field_MeasurePsynergyHeight accepts for the effect's id
/// is listed as a tile in the event work's point list at 0xcc0, which ends with
/// -1, -1; a kind-1 cell also stores its kind in the heap slot 0x5c work's byte
/// 0x1b34, and the last kind-2 and kind-3 cells are kept in pixels, centred on
/// their tile, at 0xd3c and 0xd40.
void FieldPsynergy_GatherNearbyCells(void) {
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
    kind = MapGrid_GetCycloneCellKind(x, z, owner->terrainId);
    // position[1] is never set: only x and z reach Field_MeasurePsynergyHeight.
    position[0] = x;
    position[2] = z;
    if (kind == 1 && Field_MeasurePsynergyHeight(*(s16 *)(effect + 30), owner,
                                                 position) != -1) {
      points[count * 2] = x / 0x100000;
      points[count * 2 + 1] = z / 0x100000;
      count++;
      work[0x1b34] = kind;
    }
    if (kind == 2 && Field_MeasurePsynergyHeight(*(s16 *)(effect + 30), owner,
                                                 position) != -1) {
      event->unk_d3c = x >> 16;
      event->unk_d3e = z >> 16;
      event->unk_d3c = (event->unk_d3c & 0xfff0) + 8;
      event->unk_d3e = (event->unk_d3e & 0xfff0) + 8;
      points[count * 2] = x / 0x100000;
      points[count * 2 + 1] = z / 0x100000;
      count++;
    }
    if (kind == 3 && Field_MeasurePsynergyHeight(*(s16 *)(effect + 30), owner,
                                                 position) != -1) {
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

s32 MapGrid_GetCellBehavior(s32 x, s32 z, s32 layer);

// Only for the compiler: see the FAKEMATCH tag in
// FieldPsynergy_RunTouchedEvents.
static inline s32 Index(s32 i) { return i; }

/// Resolves each cell in the event work's touched list to an event id at the
/// owner's layer, sorts the distinct ids and runs each one's event for the
/// effect's kind.
void FieldPsynergy_RunTouchedEvents(void) {
  struct TouchWork *work = Ram_HeapSlots->effectWork;
  struct TouchCell *cells =
      (struct TouchCell *)((u8 *)Ram_HeapSlots->eventWork + 0xcc0);
  struct TouchOwner *owner = work->owner;
  s32 count = 0;
  s16 ids[24];
  s32 i, j;

  for (i = 0; cells[i].x != -1; i++) {
    s16 id = MapGrid_GetCellBehavior(cells[i].x << 20, cells[i].z << 20,
                                     owner->layer);

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
    s32 event =
        Scene_FindPsynergyTrigger(0x50000005, work->kind, ids[Index(i)]);

    if (event != 0)
      Field_RunPsynergyEvent(event, work->argument0, work->argument1);
  }
}

/* VIEW(f0e150c): tag struct MapCell as struct MapCell__f0e150c; map.h has
 * another definition; tried with the definition the rest of the object has, it
 * does not compile in tla-ja: games/tla/src/field/event_commands/radial.c:1978:
 * structure has no member named `unk02'. */
struct MapCell__f0e150c {
  u8 unk00[2];
  u8 unk02;
};

extern struct TouchCopies gData_02001000;

struct MapCell__f0e150c *MapGrid_GetCellAt(s32 x, s32 z, s32 layer);

void TileCopies_CopyTouchedCells(s32 toX, s32 toZ, s32 width, s32 height,
                                 s32 originX, s16 originZ, s32 layerIndex) {
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

      MapGrid_CopyCellPictureCellsRectFar(toX + x, toZ + z, 1, 1, originX + x,
                                          originZ + z);
      MapGrid_GetCellAt(cell[i].x << 20, cell[i].z << 20, 2)->unk02 = 0;
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

/* VIEW(f0e2718): variable gData_02001000 as extern u8 gData_02001000[]; f0e150c
 * has extern struct TouchCopies gData_02001000; tried as the rest of the object
 * declares it, it does not compile in tla-ja:
 * games/tla/src/field/event_commands/radial.c:2009: incompatible types in
 * initialization. */
extern u8 gData_02001000__f0e2718[] __asm__("gData_02001000");
/* VIEW(f0e2718): routine MapGrid_GetCellAt as struct MapCell
 * *MapGrid_GetCellAt(s32 x, s32 z, s32 layer); f0e150c has struct
 * MapCell__f0e150c *MapGrid_GetCellAt(s32 x, s32 z, s32 layer); tried as the
 * definition's struct MapCell *MapGrid_GetCellAt(s32 x, s32 z, s32 layer), it
 * does not compile in tla-ja: games/tla/src/field/event_commands/radial.c:2002:
 * conflicting types for `MapGrid_GetCellAt'. */
extern struct MapCell *
MapGrid_GetCellAt__f0e2718(s32 x, s32 z,
                           s32 layer) __asm__("MapGrid_GetCellAt");

/// For each entry of the event work's list at 0x174c, calls
/// MapGrid_CopyCellPictureCellsRectFar with its bytes and clears the collision
/// code of the cell at its position.
void TileCopies_Restore(void) {
  u8 *base = gData_02001000__f0e2718;
  s32 i;

  for (i = 0; i < (s8)gData_02001000__f0e2718[0x194c]; i++) {
    u8 *entry = base + 0x174c + i * 8;

    MapGrid_CopyCellPictureCellsRectFar(entry[0], entry[1], 1, 1, entry[2],
                                        entry[3]);
    MapGrid_GetCellAt__f0e2718(entry[4] << 20, entry[5] << 20, 2)
        ->collisionCode = 0;
  }
}

#undef Actor_CreateFar
#undef EffectSprite_DrawAtWorldPosition
// ---- field/event_commands/tile_copies.c ----

/// The Cyclone's helpers: copying the cells it touched, its arcs and its
/// burst, and an object jumping to the leader.

#include "module/system/system.h"
#include "gamepak/far_call/field.h"
#include "field/field.h"

/// For each cell the Cyclone touched (the event work's point list from
/// FieldPsynergy_GatherNearbyCells) inside the width by height rectangle at
/// (x, z) of the layer, copies the matching cell picture from the rectangle at
/// (toX, toZ) over it, opens it (layer 2's collision 0) and records the copy
/// in gData_02001000, up to 64. Trial Road, Gaia Rock and the Gabomba
/// catacombs call it after a Cyclone.
void Cyclone_CopyTouchedCells(s32 x, s32 z, s16 layerIndex, s16 width,
                              s16 height, s32 toX, s32 toZ) {
  u8 *event = Ram_HeapSlots->eventWork;
  struct Actor *owner = Actor_GetById(Party_GetLeaderActor());
  struct TouchLayer *layer =
      ((struct TouchMap *)Ram_HeapSlots->mapWork)->layers;
  struct TouchCell *cell = (struct TouchCell *)(event + 0xcc0);
  s32 lift;
  s32 scrollX = layer->x >> 20;
  s32 scrollZ = layer->z >> 20;
  s32 layerX, layerZ;
  s32 left, top, i;

  if (layerIndex <= 2)
    layer += layerIndex;
  layerX = layer->x >> 20;
  layerZ = layer->z >> 20;
  left = x - layerX;
  top = z - layerZ;
  lift = -(owner->terrainHeight >> 20);

  /* FAKEMATCH(signature): gData_02001000 typed as 1491 TouchCells before the
   * copies, which it does not hold, so the cell's z is read after the last
   * copy's address is built; as u8 unk0000[0x174c], 2 of 169 instructions
   * differ in tla-en. */
  for (i = 0; cell[i].x != -1; i++) {
    s32 cz, cx, cellX, cellZ;

    cx = cell[i].x;
    cellX = cx - scrollX;
    cz = cell[i].z;
    cellZ = cz + lift - scrollZ;

    if (cellX >= left && cellX < left + width && cellZ >= top &&
        cellZ < top + height) {
      s32 dx = cellX - left;
      s32 dz = cellZ - top;
      s32 n;

      MapGrid_CopyCellPictureCellsRectFar(toX + dx, toZ + dz, 1, 1, x + dx,
                                          z + dz);
      MapGrid_GetCellAt(cx << 20, cz << 20, 2)->unk02 = 0;
      n = gData_02001000.count;
      if (n < 64) {
        gData_02001000.copies[n].toX = toX + dx;
        gData_02001000.copies[n].toZ = toZ + dz;
        gData_02001000.copies[n].fromX = x + dx;
        gData_02001000.copies[n].fromZ = z + dz;
        gData_02001000.copies[n].cellX = cell[i].x;
        gData_02001000.copies[n].cellZ = cell[i].z;
        gData_02001000.count++;
      }
    }
  }
}

/// An object (sprite) pops up at the cell (x, z), grows, turns to the leader,
/// who raises its arms, hops twice and flies into the leader in an arc. Trial
/// Road, Gaia Rock, the Gabomba catacombs, Jupiter Lighthouse and Taopo Swamp
/// call it.
// XXX: what the object is (an item the Cyclone uncovers?); a render at Trial
// Road would settle it.
void FieldItem_JumpToLeader(s32 sprite, s32 x, s32 z) {
  s32 ownerId = Party_GetLeaderActor();
  struct Actor *owner = Actor_GetById(ownerId);
  struct Actor *object = Actor_CreateFar(sprite, x << 16, 0, z << 16);
  s32 from[3];
  s32 to[3];
  s32 i;
  s32 steps;

  if (object == NULL)
    return;
  Actor_SetSpriteFlagsFar(object, 0);
  object->gravity = 0x5999;
  object->velocityY = 0x50000;
  object->scaleY = object->scaleX = 0x6666;
  do {
    wait(1);
    object->scaleY = object->scaleX = object->scaleX + 0x3d7;
    object->facing += 0x1000;
  } while (object->scaleX <= 0xffff);
  object->scaleY = object->scaleX = 0x10000;
  Actor_SelectAnimationFar(owner, 1);
  Actor_TurnPairToFaceEachOther(object, owner);
  owner->timer = owner->facing;
  Event_Wait(10);
  Actor_StartRepeatedMotion(ownerId, 3);
  FieldEffect_SetAttached(ownerId, 258);
  Event_Wait(32);
  object->gravity = 0x10000;
  object->velocityY = 0x60000;
  Sound_PlayFar(152);
  Event_Wait(15);
  object->velocityY = 0x60000;
  Sound_PlayFar(152);
  Event_Wait(23);
  Sound_PlayFar(146);

  from[0] = object->x;
  from[1] = object->y;
  from[2] = object->z;
  to[0] = owner->x;
  to[1] = owner->y;
  to[2] = owner->z;
  steps = 20;
  for (i = 0; i < steps; i++) {
    object->x = from[0] + i * (to[0] - from[0]) / 20;
    object->y =
        from[1] + i * (to[1] - from[1]) / 20 + Trig_Sin((i << 15) / 20) * 40;
    object->z = from[2] + i * (to[2] - from[2]) / 20;
    wait(1);
  }
}

/// Plots 30 pixels of an arc at radius (at most 31) from the middle of a 64 by
/// 64 picture, from angle on, shading from colour 225 to 239; offsets maps
/// each point to its byte. The Cyclone draws its swirl with it.
void Cyclone_PlotArc(s32 radius, s32 angle, u16 *offsets, u8 *pixels) {
  s32 i;

  if (radius > 31)
    radius = 31;
  for (i = 0; i <= 29; i++) {
    s32 x = ((radius * Trig_Cos(angle)) >> 16) + 32;
    s32 y = ((radius * Trig_Sin(angle)) >> 16) + 32;
    s32 value = i / 2 + 225;

    pixels[offsets[y * 64 + x]] = value;
    angle += 0x170;
  }
}

void ParticleBurst_Start(void) {
  struct BurstWork *work = (struct BurstWork *)Ram_HeapSlots->battleFxWork[0];
  u8 *effect = Ram_HeapSlots->effectWork;
  u8 *event = Ram_HeapSlots->eventWork;
  struct Actor *owner = *(struct Actor **)(effect + 16);
  struct BurstCell *cell = (struct BurstCell *)(event + 0xcc0);
  s32 dx = (cell->x << 20) - owner->x + 0x80000;
  s32 dz = (cell->z << 20) - owner->z + 0x80000;
  s32 heading = (u16)ArcTan2(dz, dx);
  struct EffectSprite *sprite;
  struct BurstParticle *particle;
  s32 resource;
  s32 vram;
  s32 i;

  work->frame = 0;
  work->active = 1;
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_Leaves), work);
  resource = VramCache_FindFreeEntry();
  vram = VramCache_Load(resource, 384, work);
  work->resource = resource;
  work->vram = vram;

  for (i = 0, particle = work->particles; i < 96; i++, particle++) {
    sprite = &work->sprites[i];
    EffectSprite_Init(sprite, 4, 4, 0, vram);
    sprite->depth = 250;
    sprite->oam.priority = 0;
    sprite->oam.palette256 = 1;
    sprite->oam.palette = 0;
    particle->x = owner->x;
    particle->y = owner->y;
    particle->z = owner->z;
    Vector_AddPolarOffset(Random16() * 8 + 0x80000, Random16(), &particle->x);
    particle->heading = heading;
    particle->speed = Random16() + 0x10000;
    particle->spin = (Random16() >> 6) + 384;
    if (i & 1)
      particle->spin = -particle->spin;
    particle->delay = -i / 2;
  }
  Sound_PlayFar(220);
  Task_Add(ParticleBurst_Update, TASK_PASS_VBLANK);
}

// ---- field/event_commands/psynergy.c ----

/// The field Psynergy The Lost Age adds, from Cyclone to Hover, each part named
/// for the Psynergy it runs (its kind in FieldPsynergy_Run), and the effects
/// they share. psynergy_internal.h holds the includes every part shares; the
/// parts under psynergy/ follow in ROM order.

#include "psynergy_internal.h"

// The parts, in ROM order.
#include "psynergy/cyclone.c"
#include "psynergy/blaze.c"
#include "psynergy/sand_sinking.c"
#include "psynergy/growing_particle.c"
#include "psynergy/sand.c"
#include "psynergy/burst.c"
#include "psynergy/sinking_object.c"
#include "psynergy/pound.c"
#include "psynergy/growing_sprite.c"
#include "psynergy/scoop.c"
#include "psynergy/unnamed_28.c"
#include "psynergy/tremor.c"
#include "psynergy/grind.c"
#include "psynergy/hover.c"
#include "psynergy/mote_trail.c"
#include "psynergy/collision_tags.c"

#undef BurstWork
#undef Camera_WorldToScreen
#undef EffectSprite_DrawAtWorldPosition
#undef ObjectAnimation
#undef Resource_GetMetadataRecordFarFar
#undef BattleEffect_DropObjectAtAnchor
#undef SparkParticle
#undef ObjectSprite
#undef Mote
#undef Actor_StartScriptFar

// ---- field/map/actor_cells.c ----

/// Actors standing as blocks in the map: their cells tagged and the tags
/// cleared, the actor at a position, and effect sprites filled and queued.

/* VIEW(f0eabd0): tag struct SwitchCell as struct SwitchCell__f0eabd0; field.h
 * has another definition; tried with the definition the rest of the object has,
 * it does not compile in tla-ja:
 * games/tla/src/field/event_commands/radial.c:9007: structure has no member
 * named `flags'. */
struct SwitchCell__f0eabd0 {
  u8 unk00[2];
  u8 block;
  u8 flags;
};

extern u8 gMapShapeGrid[];

struct Actor *Actor_GetById(u32 objectId);

/// Makes each listed actor (ids, ended by 0xffff) a block in the map: its
/// cell's shape is tagged 32 pixels above the ground, so the party can stand
/// on it, its cell is marked raised (the old mark kept in the actor) and layer
/// 2 blocks it. A leader below the new ground is lifted onto it. Scenes call it
/// as they start, for the blocks Move pushes.
void Actor_PlaceBlocks(u16 *actors) {
  struct RestoreMap *map = (struct RestoreMap *)Ram_HeapSlots->mapWork;
  struct SwitchCell__f0eabd0 *blocks = map->layers[2].cells;
  u8 *grid = gMapShapeGrid;
  struct Actor *leader;
  s32 height;

  for (; *actors != 0xffff; actors++) {
    struct Actor *object = Actor_GetById(*actors);
    u32 key =
        MapGrid_GetTerrainKindFar(object->terrainId, object->x, object->z);
    s32 level =
        MapGrid_GetTerrainHeightFar(object->terrainId, object->x, object->z) >>
        19;
    s32 slot = Actor_GetCellIndex(object);
    struct SwitchCell__f0eabd0 *cells = map->layers[object->terrainId].cells;
    // The row's address as an integer sum, index first: &grid[row] adds the
    // grid first.
    u8 *shapes =
        (u8 *)((((s32)cells - (s32)Ram_MapCellBuffer) >> 2) + (s32)grid);
    struct SwitchCell__f0eabd0 *cell;
    s32 tag;
    u8 flags;

    object->motionFlags = 0;
    object->collisionFlags = 0;
    Actor_SetSpriteFlagsFar(object, 0);
    shapes += slot;
    tag = MapCollision_FindOrAddTag(object->terrainId, key, level + 4);
    cell = &cells[slot];
    flags = cell->flags;
    object->unk4c = flags;
    cell->flags = flags | 0x80;
    *shapes = tag;
    // Reused for the block in layer 2's table.
    cell = &blocks[slot];
    cell->block = 0xff;
  }
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  height = MapGrid_GetTerrainHeightFar(leader->terrainId, leader->x, leader->z);
  if (leader->y < height)
    leader->y = leader->terrainHeight = height;
}

// Only so the shape store has an alias set of its own; the grid holds bytes.
struct ShapeCell {
  u8 shape;
};

struct Actor *Leader_FindActorAhead(void);

/// Takes the block about to move out of the map: the field effect's target
/// while one runs, else the actor ahead of the leader, if it is one of the
/// listed actors. Its cell goes back to the ground and its old mark, and the
/// event work remembers it for Actor_PlaceMovedBlock.
void Actor_LiftTargetBlock(u16 *actors) {
  u8 *event = Ram_HeapSlots->eventWork;
  u8 *effect = Ram_HeapSlots->effectWork;
  struct Actor *target;

  if (effect != NULL && *(s16 *)(event + 358) != 0)
    target = *(struct Actor **)(effect + 20);
  else
    target = Leader_FindActorAhead();
  *(struct Actor **)(event + 0xcbc) = NULL;

  for (; *actors != 0xffff; actors++) {
    if (target == Actor_GetById(*actors)) {
      struct RestoreMap *map = (struct RestoreMap *)Ram_HeapSlots->mapWork;
      s32 zero = 0;
      u8 *grid = gMapShapeGrid;
      struct SwitchCell__f0eabd0 *cells = map->layers[target->terrainId].cells;
      struct SwitchCell__f0eabd0 *blocks = map->layers[2].cells;
      u8 *shapes = &grid[((s32)cells - (s32)Ram_MapCellBuffer) >> 2];
      u32 key =
          MapGrid_GetTerrainKindFar(target->terrainId, target->x, target->z);
      s32 level =
          MapGrid_GetTerrainHeightFar(target->terrainId, target->x, target->z);
      s32 slot = Actor_GetCellIndex(target);
      s32 tag;

      shapes += slot;
      tag =
          MapCollision_FindOrAddTag(target->terrainId, key, (level >> 19) - 4);
      cells += slot;
      blocks += slot;
      ((struct ShapeCell *)shapes)->shape = tag;
      cells->flags = target->unk4c;
      blocks->block = zero;
      *(struct Actor **)(event + 0xcbc) = target;
    }
  }
}

#include "stddef.h"

// Only for the compiler: see the FAKEMATCH tag in Actor_PlaceMovedBlock.
static inline s32 ActorX(struct Actor *actor) { return actor->x; }

/// Puts the block Actor_LiftTargetBlock remembered back into the map at the
/// cell it has moved to, as Actor_PlaceBlocks places each.
void Actor_PlaceMovedBlock(void) {
  struct Actor *target =
      *(struct Actor **)((u8 *)Ram_HeapSlots->eventWork + 0xcbc);

  if (target != NULL) {
    struct RestoreMap *map = (struct RestoreMap *)Ram_HeapSlots->mapWork;
    /* FAKEMATCH(temporary): the cell table read split into its byte offset
     * and the read, with the grid's address read between them; as
     * cells = map->layers[target->terrainId].cells, 35 of 84 instructions
     * differ in tla-en. */
    s32 offset = offsetof(struct RestoreMap, layers[target->terrainId].cells);
    u8 *grid = gMapShapeGrid;
    struct SwitchCell__f0eabd0 *cells =
        *(struct SwitchCell__f0eabd0 **)((u8 *)map + offset);
    struct SwitchCell__f0eabd0 *blocks = map->layers[2].cells;
    s32 index = ((s32)cells - (s32)Ram_MapCellBuffer) >> 2;
    u8 *shapes;
    u32 key;
    s32 level;
    s32 slot;
    s32 tag;
    u8 flags;

    /* FAKEMATCH(barrier): an empty asm on the grid index that claims to read
     * blocks, giving layer 2's table load a fourth dependent, so sched2 takes
     * it before the grid's pool load; without it, 2 of 84 instructions differ
     * in tla-en. */
    asm("" : "+l"(index) : "l"(blocks));
    shapes = &grid[index];
    key = MapGrid_GetTerrainKindFar(target->terrainId, target->x, target->z);
    /* FAKEMATCH(wrapper): x read through ActorX for the height call; as
     * target->x, 12 of 84 instructions differ in tla-en. */
    level = MapGrid_GetTerrainHeightFar(target->terrainId, ActorX(target),
                                        target->z);
    slot = Actor_GetCellIndex(target);
    shapes += slot;
    tag = MapCollision_FindOrAddTag(target->terrainId, key, (level >> 19) + 4);
    ((struct ShapeCell *)shapes)->shape = tag;
    cells += slot;
    flags = cells->flags;
    target->unk4c = flags;
    cells->flags = flags | 0x80;
    blocks += slot;
    blocks->block = 0xff;
  }
}

struct Actor *Actor_FindAtCell(s32 *position, s32 check_y);

/// Returns the actor one cell, or failing that two cells, ahead of the leader
/// at the leader's height, or NULL.
struct Actor *Leader_FindActorAhead(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 position[3];
  s32 angle = ((leader->facing >> 12) + 2) & 12;
  struct Actor *found;

  position[0] = leader->x;
  position[1] = leader->y;
  position[2] = leader->z;
  Vector_AddPolarOffset(0x100000, angle << 12, position);
  found = Actor_FindAtCell(position, 1);
  if (found != NULL)
    return found;
  position[0] = leader->x;
  position[1] = leader->y;
  position[2] = leader->z;
  Vector_AddPolarOffset(0x200000, angle << 12, position);
  found = Actor_FindAtCell(position, 1);
  if (found != NULL)
    return found;
  return NULL;
}

/// Returns the first of event objects 8 to 63 on position's cell, at its
/// height too when checkY is set, or NULL.
struct Actor *Actor_FindAtCell(s32 *position, s32 checkY) {
  u32 i;

  for (i = 8; i < 64; i++) {
    struct EventWork *event = Ram_HeapSlots->eventWork;
    struct Actor *object = event->objects[i];

    if (checkY) {
      if (position[0] >> 20 == object->x >> 20 &&
          position[1] / Q16_ONE == object->y / Q16_ONE &&
          position[2] >> 20 == object->z >> 20)
        return object;
    } else {
      if (position[0] >> 20 == object->x >> 20 &&
          position[2] >> 20 == object->z >> 20)
        return object;
    }
  }
  return NULL;
}

/// The width and height of each OAM shape and size, in pixels.
extern const u8 gData_080f3bc4[][2];

/// Fills an effect sprite: OAM attributes 0 and 1 as given (no affine matrix,
/// 256 colours), the tiles, unscaled and unrotated at depth 255, and the
/// picture's size from its shape, pivoted at (pivotX, pivotY).
void EffectSprite_Init(struct EffectSprite *sprite, s32 pivotX, s32 pivotY,
                       u32 attributes, s32 tile) {
  u32 *words;
  s32 shape;

  if (sprite == NULL)
    return;
  Iwram_FillWordsEntry(sprite, sizeof(*sprite), 0);
  sprite->scaleX = Q16_ONE;
  sprite->scaleY = Q16_ONE;
  sprite->depth = 255;
  sprite->pivotX = pivotX;
  sprite->pivotY = pivotY;
  shape = ((attributes >> 30) | (attributes >> 12)) & 15;
  sprite->width = gData_080f3bc4[shape][0];
  sprite->height = gData_080f3bc4[shape][1];
  words = (u32 *)&sprite->oam;
  *words++ = attributes;
  *words = 0;
  // Through the OAM's address: as sprite->oam.palette256, 2 of 57 differ.
  (&sprite->oam)->palette256 = 1;
  sprite->oam.index = tile;
}

#include <affine.h>

#define Q16_ONE 0x10000

/// Draws a field effect's sprite. An unscaled, unrotated sprite needs no
/// affine matrix; any other takes one, and one scaled past 1 is drawn in a
/// double-size box. The picture's pivot is turned and scaled with it, so the
/// pivot stays on the sprite's position. A sprite wholly off screen is not
/// queued; the rest join the OAM bucket of their depth.
void EffectSprite_Draw(struct EffectSprite *sprite) {
  s32 affineMode;
  s32 matrix;
  s32 x = sprite->x;
  s32 y = sprite->y;
  s32 scaleX = sprite->scaleX;
  s32 scaleY = sprite->scaleY;
  s32 width = sprite->width;
  s32 height = sprite->height;
  s32 left;
  s32 top;
  s32 screenX;
  s32 screenY;
  s32 magnitude;
  struct AffineTransform transform;

  if (sprite->depth == EFFECT_SPRITE_HIDDEN || scaleX == 0 || scaleY == 0)
    return;

  // The affine matrix, unless the sprite is drawn as it is.
  if (scaleX == Q16_ONE && scaleY == scaleX && sprite->angle == 0) {
    matrix = 0;
    affineMode = 0;
  } else {
    transform.angle = sprite->angle;
    transform.scaleX = scaleX >> 8;
    transform.scaleY = scaleY >> 8;
    matrix = OamMatrix_AllocFromTransform(&transform);
    affineMode = 1;
  }

  // Grown past its own size, it needs the double-size box.
  magnitude = scaleX;
  if (magnitude < 0)
    magnitude = -magnitude;
  if (magnitude > Q16_ONE ||
      (magnitude = scaleY < 0 ? -scaleY : scaleY, magnitude > Q16_ONE)) {
    affineMode = 3;
    width *= 2;
    height *= 2;
  }

  // Where the pivot sits in the drawn box, from the box's top left.
  if (sprite->angle == 0) {
    if (scaleX == Q16_ONE && scaleY == scaleX) {
      left = sprite->pivotX - (sprite->width >> 1) + width / 2;
      top = sprite->pivotY - (sprite->height >> 1) + height / 2;
    } else {
      left =
          ((scaleX * (sprite->pivotX - (sprite->width >> 1)) + 0xffff) >> 16) +
          width / 2;
      top =
          ((scaleY * (sprite->pivotY - (sprite->height >> 1)) + 0xffff) >> 16) +
          height / 2;
    }
  } else {
    s32 sin = Trig_Sin(sprite->angle);
    s32 cos = Trig_Cos(sprite->angle);
    s32 halfWidth = sprite->width * scaleX / 2;
    s32 halfHeight = sprite->height * scaleY / 2;
    s32 cornerX = Iwram_MulQ16(cos, halfWidth) - Iwram_MulQ16(sin, halfWidth);
    s32 cornerY = Iwram_MulQ16(cos, halfHeight) + Iwram_MulQ16(sin, halfHeight);
    s32 pivotX = scaleX * sprite->pivotX;
    s32 pivotY = sprite->pivotY * scaleY;
    s32 turnedX = Iwram_MulQ16(cos, pivotX) - Iwram_MulQ16(sin, pivotY);
    s32 turnedY = Iwram_MulQ16(cos, pivotY) + Iwram_MulQ16(sin, pivotX);

    left = width / 2 + ((turnedX - cornerX) >> 16);
    top = height / 2 + ((turnedY - cornerY) >> 16);
  }

  // The box on screen; skip it when none of it shows.
  screenX = (x >> 16) - left;
  screenY = (y >> 16) - top;
  if (screenX + width < 0 || screenX > 239 || screenY + height <= 0 ||
      screenY > 159)
    return;

  sprite->oam.x = screenX;
  sprite->oam.y = screenY;
  sprite->oam.affineMode = affineMode;
  sprite->oam.matrix = matrix;
  Oam_AddSpriteAtDepthClamped(sprite, sprite->depth);
}

struct ScreenSprite {
  u8 unk00[0xc];
  s32 x;
  s32 y;
  u8 unk14[0x14];
};

/// Places sprite at the screen position of world position and draws it.
void EffectSprite_DrawAtWorldPosition(struct ScreenSprite *sprite,
                                      s32 *position) {
  s32 screen[3];

  if (sprite != 0) {
    s32 *p = screen;
    s32 *arg;

    p[0] = position[0];
    arg = p;
    p[1] = position[1];
    p[2] = position[2];
    Camera_WorldToScreen(arg);
    sprite->x = screen[0];
    sprite->y = screen[2];
    EffectSprite_Draw(sprite);
  }
}

// ---- field/event_commands/step_vector.c ----

void BattleEffect_StepVector2(s32 *vector, s32 damping, s32 gravity) {
  BattleEffect_IntegrateVector2(vector, damping, gravity);
}

void BattleEffect_StepVector3(s32 *vector, s32 damping, s32 gravity) {
  BattleEffect_IntegrateVector3(vector, damping, gravity);
}

void BattleEffect_PlotHalfCanvasPixel(s32 x, s32 y, s32 color) {
  u16 *map = Ram_HeapSlots->canvasMap;
  u8 *canvas;

  canvas = Ram_HeapSlots->battleFxWork[1];
  canvas[map[(y << 6) + x]] = color;
}

void BattleEffect_PlotCanvasPixel(s32 x, s32 y, s32 color) {
  u16 *map = Ram_HeapSlots->canvasMap;
  u8 *canvas = Ram_HeapSlots->battleFxWork[1];

  if (x > 63) {
    canvas += 0x1000;
    x -= 64;
  }
  canvas[map[(y << 6) + x]] = color;
}

/// Draws a line of half canvas pixels from x0, y0 to x1, y1, Bresenham's
/// way: one pixel per step along the longer axis.
void BattleEffect_DrawHalfCanvasLine(s32 x0, s32 y0, s32 x1, s32 y1,
                                     s32 color) {
  s32 dx = x1 - x0;
  s32 dy;
  s32 sx;
  s32 sy;
  s32 err = 0;
  s32 x;
  s32 y;
  s32 i;

  if (dx < 0) {
    sx = -1;
    dx = -dx;
  } else {
    sx = 1;
  }
  dy = y1 - y0;
  if (dy < 0) {
    sy = -1;
    dy = -dy;
  } else {
    sy = 1;
  }
  x = x0;
  y = y0;
  if (dx > dy) {
    for (i = 0; i < dx; i++) {
      BattleEffect_PlotHalfCanvasPixel(x, y, color);
      err += dy;
      if (err >= dx) {
        err -= dx;
        y += sy;
      }
      x += sx;
    }
  } else {
    for (i = 0; i < dy; i++) {
      BattleEffect_PlotHalfCanvasPixel(x, y, color);
      err += dx;
      if (err >= dy) {
        err -= dy;
        x += sx;
      }
      y += sy;
    }
  }
}

/// Draws a circle of the given radius round (cx, cy) on the half canvas, by
/// the midpoint method: each step plots the eight symmetric points.
void BattleEffect_DrawHalfCanvasCircle(s32 cx, s32 cy, s32 radius, s32 color) {
  s32 x = radius;
  s32 y = 0;

  while (x >= y) {
    BattleEffect_PlotHalfCanvasPixel(cx + x, cy + y, color);
    BattleEffect_PlotHalfCanvasPixel(cx + x, cy - y, color);
    BattleEffect_PlotHalfCanvasPixel(cx - x, cy + y, color);
    BattleEffect_PlotHalfCanvasPixel(cx - x, cy - y, color);
    BattleEffect_PlotHalfCanvasPixel(cx + y, cy + x, color);
    BattleEffect_PlotHalfCanvasPixel(cx + y, cy - x, color);
    BattleEffect_PlotHalfCanvasPixel(cx - y, cy + x, color);
    BattleEffect_PlotHalfCanvasPixel(cx - y, cy - x, color);
    radius = radius - y * 2 + 1;
    if (radius < 0) {
      s32 next = x - 1;

      radius += next * 2;
      x = next;
    }
    y++;
  }
}

/// Copies a width by height block of colour indices onto the battle canvas
/// through its cell map at x, y, skipping zeros; nothing is drawn when the
/// block starts off the map or its far edge passes cell 63.
void BattleEffect_DrawCanvasBlock(const u8 *src, s32 x, s32 y, s32 width,
                                  s32 height) {
  u16 *map = Ram_HeapSlots->canvasMap;
  u8 *canvas = Ram_HeapSlots->battleFxWork[1];
  s32 row;
  s32 n;

  if (x < 0 || x + width > 63 || y < 0 || y + height > 63)
    return;
  for (row = 0; row < height; row++) {
    for (n = 0; n < width; n++) {
      if (*src != 0)
        canvas[map[((y + row) << 6) + x + n]] = *src;
      src++;
    }
  }
}

void ColorBuffer_MapToFadeStep1(u8 *buffer, s32 count) {
  if (count > 0) {
    const u8 *table = gColorBuffer_FadeStep1Map;

    do {
      if (*buffer != 0)
        *buffer = table[*buffer & 31];
      count--;
      buffer++;
    } while (count != 0);
  }
}

void ColorBuffer_MapToFadeStep2(u8 *buffer, s32 count) {
  if (count > 0) {
    const u8 *table = gColorBuffer_FadeStep2Map;

    do {
      if (*buffer != 0)
        *buffer = table[*buffer & 31];
      count--;
      buffer++;
    } while (count != 0);
  }
}

void ColorBuffer_MapToFadeStep3(u8 *buffer, s32 count) {
  if (count > 0) {
    const u8 *table = gColorBuffer_FadeStep3Map;

    do {
      if (*buffer != 0)
        *buffer = table[*buffer & 31];
      count--;
      buffer++;
    } while (count != 0);
  }
}

#include "dma.h"

#define OBJ_TILES ((u8 *)0x06010000)
#define BG_VRAM ((void *)0x06000000)
#define TILE_SIZE 32

/// The canvas in words, as one 32-bit DMA.
#define CANVAS_DMA DMA_COPY_WORDS(CANVAS_SIZE / 4)

/// The canvas's frame callback: counts the frame and carries out the command
/// the effect left for it, copying the canvas into the sprite's tiles (or to
/// the start of BG VRAM), and clearing or fading it after the copy. Case 2
/// comes before case 1: its body comes first in the ROM.
void BattleEffect_UploadCanvas(void) {
  struct CanvasSpriteWork *work = Ram_HeapSlots->unkec[1];
  u8 *canvas = Ram_HeapSlots->battleFxWork[1];
  u8 *tiles = OBJ_TILES + work->tile * TILE_SIZE;

  work->frame++;
  switch (work->command) {
  case CANVAS_UPLOAD:
    Dma_Set(canvas, tiles, CANVAS_DMA, REG_DMA3);
    break;
  case CANVAS_UPLOAD_AND_CLEAR:
    Dma_Set(canvas, tiles, CANVAS_DMA, REG_DMA3);
    {
      s32 size = 128;
      /* FAKEMATCH(pin): the canvas pinned to the r0 it is passed in, so its
       * copy there comes before the size's shift; as a plain u8 *buffer, 2
       * of 98 instructions differ in tla-en. */
      register u8 *buffer asm("r0");
      s32 (*fill)(void *, s32, u32);

      fill = Iwram_FillWordsEntry;
      buffer = canvas;
      fill(buffer, size << 5, 0);
    }
    break;
  case CANVAS_UPLOAD_AND_FADE1:
    Dma_Set(canvas, tiles, CANVAS_DMA, REG_DMA3);
    ColorBuffer_MapToFadeStep1(canvas, CANVAS_SIZE);
    break;
  case CANVAS_UPLOAD_AND_FADE2:
    Dma_Set(canvas, tiles, CANVAS_DMA, REG_DMA3);
    ColorBuffer_MapToFadeStep2(canvas, CANVAS_SIZE);
    break;
  case CANVAS_UPLOAD_AND_FADE3:
    Dma_Set(canvas, tiles, CANVAS_DMA, REG_DMA3);
    ColorBuffer_MapToFadeStep3(canvas, CANVAS_SIZE);
    break;
  case CANVAS_UPLOAD_TO_BG:
    Dma_Set(canvas, BG_VRAM, DMA_COPY_WORDS(CANVAS_SIZE * 2 / 4), REG_DMA3);
    break;
  }
  work->command = 0;
}

// OAM attributes 0 and 1 of a 64x64 square sprite.
#define OAM_SQUARE_64X64 0xc0000000

// The canvas sprite's frame callback: places the sprite at the work's
// position (whole pixels), scale and angle, and draws it. A sprite scaled to
// nothing is not drawn.
void BattleEffect_DrawCanvasSprite(void) {
  struct CanvasSpriteWork *work = Ram_HeapSlots->unkec[1];
  s32 x = work->x << 16;
  s32 y = work->y << 16;
  s32 scaleX = work->scaleX;
  s32 scaleY = work->scaleY;

  if (scaleX != 0 && scaleY != 0) {
    if (work->mode == CANVAS_SPRITE_16_COLORS) {
      EffectSprite_Init(&work->sprite, 32, 32, OAM_SQUARE_64X64, work->tile);
      work->sprite.oam.palette256 = 0;
      work->sprite.oam.palette = 14;
      work->sprite.oam.priority = work->priority;
      work->sprite.depth = work->depth;
      work->sprite.x = x;
      work->sprite.y = y;
      work->sprite.scaleX = scaleX;
      work->sprite.scaleY = scaleY;
      work->sprite.angle = work->angle;
      EffectSprite_Draw(&work->sprite);
    }
    if (work->mode == CANVAS_SPRITE_256_COLORS) {
      EffectSprite_Init(&work->sprite, 32, 32, OAM_SQUARE_64X64, work->tile);
      work->sprite.oam.palette256 = 1;
      work->sprite.oam.palette = 0;
      work->sprite.oam.priority = work->priority;
      work->sprite.depth = work->depth;
      work->sprite.x = x;
      work->sprite.y = y;
      work->sprite.scaleX = scaleX;
      work->sprite.scaleY = scaleY;
      work->sprite.angle = work->angle;
      EffectSprite_Draw(&work->sprite);
    }
    if (work->mode == CANVAS_SPRITE_KEPT) {
      work->sprite.oam.priority = work->priority;
      work->sprite.depth = work->depth;
      work->sprite.x = x;
      work->sprite.y = y;
      work->sprite.scaleX = scaleX;
      work->sprite.scaleY = scaleY;
      work->sprite.angle = work->angle;
      EffectSprite_Draw(&work->sprite);
    }
  }
  work->drawnFrames++;
}

// The canvas sprite of the field Psynergy scenes.

#include "window.h"

/// Starts the canvas sprite of the field Psynergy scenes: its work, a cleared
/// 64x64 canvas copied into the sprite's tiles, and a map from each canvas
/// pixel (row by row) to its place in the tiles. BattleEffect_DrawCanvasSprite
/// draws the sprite and BattleEffect_UploadCanvas uploads the canvas each
/// frame; BattleEffect_EndCanvasSprite ends it.
void BattleEffect_StartCanvasSprite(void) {
  struct WindowWork *windowWork =
      (struct WindowWork *)Ram_HeapSlots->windowTiles;
  struct CanvasSpriteWork *work =
      HeapSlot_Alloc(HEAP_SLOT_CANVAS_SPRITE, sizeof(*work));
  u8 *canvas = HeapSlot_Alloc(HEAP_SLOT_CANVAS, CANVAS_SIZE);
  u16 *map;
  s32 y;
  s32 x;

  do {
    Iwram_FillWordsEntry(work, sizeof(*work), 0);
  } while (0);
  work->tile = windowWork->glyphTiles;
  work->mode = CANVAS_SPRITE_16_COLORS;
  work->command = 0;
  work->x = 16;
  work->y = 0;
  work->scaleX = 0x10000;
  work->scaleY = 0x10000;
  work->priority = 1;
  work->depth = 250;
  Iwram_FillWordsEntry(canvas, CANVAS_SIZE, 0);
  Dma_Set(canvas, OBJ_TILES + work->tile * TILE_SIZE,
          0x84000000 | CANVAS_SIZE / 4, REG_DMA3);

  // Where each pixel of the canvas sits in its 8x8 tiles.
  map = HeapSlot_AllocEwram(HEAP_SLOT_CANVAS_MAP,
                            CANVAS_WIDTH * CANVAS_WIDTH * sizeof(u16));
  for (y = 0; y < CANVAS_WIDTH; y++) {
    for (x = 0; x < CANVAS_WIDTH; x++) {
      s32 row = y & 7;
      s32 column = x & 7;
      *map++ = ((((y >> 3) * 8 + (x >> 3)) * 8) + row) * 8 + column;
    }
  }

  Task_Add(BattleEffect_DrawCanvasSprite, TASK_PASS_VBLANK);
  Task_Add(BattleEffect_UploadCanvas, TASK_PASS_FRAME - 20);
}

/// Ends the canvas sprite: stops its draw and upload callbacks, waits a
/// frame and frees its sprite work, canvas and pixel map.
void BattleEffect_EndCanvasSprite(void) {
  Task_Remove(BattleEffect_DrawCanvasSprite);
  Task_Remove(BattleEffect_UploadCanvas);
  wait(1);
  HeapSlot_Release(180);
  HeapSlot_Release(96);
  HeapSlot_Release(240);
}

// One spark: where it circles round, its angle and radius, and its age in
// frames (-1: unused).
struct Spark {
  s32 centre[3];
  s32 angle;
  s32 unk10;
  s32 radius;
  s32 age;
};

// The heap block of the effect: the picture, then the sparks and their
// sprites.
/* VIEW(step_vector): tag struct SparkWork as struct SparkWork__step_vector;
 * field/event_commands/psynergy has another definition; tried with the
 * definition the rest of the object has, it does not compile in tla-ja:
 * games/tla/src/field/event_commands/radial.c:9718: redefinition of `struct
 * SparkWork'. */
#define SparkWork SparkWork__step_vector
struct SparkWork {
  u8 picture[0x400];
  struct Spark sparks[64];
  struct EffectSprite sprites[64];
};

#define HEAP_SLOT_SPARKS 92
#define SPARK_LIFE 50
#define PHASE_DONE 999

/// An object vanishes in a whirl of sparks: 64 sprites of picture 0x1ea circle
/// out from it, each spark rising and closing in for 50 frames. Ten frames in
/// the sparks stop coming and the object spins and widens from nothing back to
/// full width over 64 frames, when its sprite and its child flags come back;
/// 74 frames later it ends.
void Actor_VanishInSparks(s32 objectId) {
  struct SparkWork *work =
      HeapSlot_Alloc(HEAP_SLOT_SPARKS, sizeof(struct SparkWork));
  s32 phase;
  s32 entry;
  s32 tiles;
  s32 spawning;
  struct Actor *object = ObjectTable_Get(objectId);
  s32 depth = BattleEffect_GetObjectDepth(object);
  s32 angle = (u16)object->facing;
  struct Spark *spark;
  struct EffectSprite *sprite;
  s32 i;
  s32 next;
  s32 frame;
  s32 position[3];

  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_SparkleFade),
                      work);
  entry = VramCache_FindFreeEntry();
  tiles = VramCache_Load(entry, sizeof(work->picture), work);

  i = 0;
  spark = work->sparks;
  for (; i < 64; i++, spark++) {
    sprite = &work->sprites[i];
    EffectSprite_Init(sprite, 4, 4, 0, tiles + (i & 15) * 2);
    sprite->oam.palette256 = 1;
    sprite->oam.palette = 0;
    sprite->depth = BattleEffect_GetObjectDepth(object);
    sprite->oam.priority = BattleEffect_GetObjectPriority(object);
    spark->age = -1;
  }
  object->scaleX = 0;
  object->scaleY = 0x10000;
  Actor_SetSpriteFlagsFar(object, 0);
  spawning = 1;
  phase = 0;
  next = 0;
  frame = 0;
  do {
    if (spawning != 0) {
      for (i = spawning; i != 0; i--) {
        s32 sparkAngle = Random16();
        s32 radius = Random16() * 6 + 0xc0000;

        spark = &work->sparks[next];

        spark->age = 0;
        spark->centre[0] = object->x;
        spark->centre[1] = object->y + 0x140000;
        spark->centre[2] = object->z;
        Vector_AddPolarOffset(Random16(), sparkAngle, spark->centre);
        next++;
        spark->angle = sparkAngle;
        spark->radius = radius;
        next %= 64;
      }
    }
    spark = work->sparks;
    sprite = work->sprites;
    i = 63;
    for (; i >= 0; i--, sprite++, spark++) {
      if (spark->age >= 0) {
        struct EffectSprite *drawn = sprite;
        /* FAKEMATCH(barrier): sprite copied to drawn through an empty asm,
           which keeps the two in registers of their own; as sprite itself (or
           the plain copy), 83 of 325 instructions differ in tla-en. */
        asm("" : "=r"(drawn) : "0"(drawn));

        if ((u32)spark->age < SPARK_LIFE) {
          s32 *pos = position;
          s32 sparkAngle = spark->angle;
          s32 radius = spark->radius;

          pos[0] = spark->centre[0];
          pos[1] = spark->centre[1];
          pos[2] = spark->centre[2];
          Vector_AddPolarOffset(radius, sparkAngle, pos);
          if (pos[2] > object->z)
            sprite->depth = depth + 2;
          else
            sprite->depth = depth - 2;
          Camera_WorldToScreen(pos);
          drawn->x = pos[0];
          drawn->y = pos[2];
          drawn->oam.index = tiles + (spark->age & 3) * 2;
          EffectSprite_Draw(drawn);
          spark->angle += 0x800;
          spark->radius -= 0x3333;
          spark->centre[1] -= 0x4000;
        }
        if (++spark->age == SPARK_LIFE)
          spark->age = -1;
      }
    }
    switch (phase) {
    case 0:
      if (frame == 10) {
        phase = 1;
        frame = -1;
      }
      break;
    case 1:
      if (frame < 64) {
        object->facing -= 0x1000;
        object->scaleX = frame << 10;
        if (object->scaleX > 0x10000)
          object->scaleX = 0x10000;
      }
      if (frame == 64) {
        object->facing = angle;
        Actor_SetSpriteFlagsFar(object, 1);
      }
      if (frame == 10)
        spawning = 0;
      if (frame == 74)
        phase = PHASE_DONE;
      break;
    }
    wait(1);
    frame++;
  } while (phase != PHASE_DONE);
  VramCache_FreeEntry(entry);
  HeapSlot_Release(HEAP_SLOT_SPARKS);
}

#include "macros.h"

void FieldParticle_Update(struct FieldParticle *effect) {
  if (effect->active != 0) {
    effect->age++;
    if (effect->callbackDelay != 0)
      effect->callbackDelay--;
    else if (effect->callback != 0)
      effect->callback(effect);
    if (effect->active != 0) {
      if (effect->updateMotion != 0)
        FieldParticle_UpdateMotion(effect);
      if (effect->render != 0)
        FieldEffect_DrawScaledActor(effect);
    }
  }
}

void FieldEffect_DrawScaledActor(struct FieldParticle *object) {
  struct ScaledSprite *sprite;
  s32 offset;
  s32 scale[2];
  s32 position[4];

  sprite = object->object;
  offset = 0;
  if (object->flags & 4)
    offset = 0x1fc0000 - object->z;
  scale[0] = Iwram_MulQ16(object->scaleX, sprite->scale);
  scale[1] = Iwram_MulQ16(object->scaleY, sprite->scale);
  SET_VECTOR(position, object->x, offset, object->z + offset);
  position[3] = 0;
  if (object->x > -0x200000 && object->x < 0x1100000 && object->z > -0x200000 &&
      object->z < 0xe00000)
    Render_ApplyProjectedPlacementFar(sprite, position, scale, 0);
}

void FieldParticle_UpdateMotion(struct FieldParticle *effect) {
  s32 dx;
  s32 dz;
  s32 distance;
  s32 ix;
  s32 iz;
  s32 speed;
  s16 angle;
  s32 heading;
  s32 facing;
  s16 turn;

  if (effect->targetX == EFFECT_NO_TARGET)
    return;
  dx = effect->targetX - effect->x;
  dz = effect->targetZ - effect->z;
  if (effect->stopAtTarget != 0) {
    ix = dx / Q16_ONE;
    iz = dz / Q16_ONE;
    distance = Iwram_Sqrt(ix * ix + iz * iz) << 16;
    if (distance < 0x800000)
      distance = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz));
    if (distance <= 0x80000) {
      FieldParticle_SetPosition(effect, effect->targetX, effect->targetZ);
      return;
    }
  }
  angle = ArcTan2(dz, dx);
  if (effect->limitTurnStep != 0) {
    heading = effect->heading;
    turn = angle - heading;
    if ((turn >= 0 ? turn : -turn) >= effect->maxTurnStep) {
      if (turn < 0) {
        if (-turn > effect->maxTurnStep)
          turn = -effect->maxTurnStep;
      } else if (turn > effect->maxTurnStep) {
        turn = effect->maxTurnStep;
      }
      angle = turn + heading;
    }
  }
  facing = (u16)angle;
  effect->heading = facing;
  speed = effect->speed + effect->acceleration;
  if (speed > effect->maxSpeed)
    speed = effect->maxSpeed;
  angle = (s16)facing;
  effect->speed = speed;
  effect->x += Iwram_MulQ16(Trig_Cos(angle), speed);
  effect->z += Iwram_MulQ16(Trig_Sin(angle), speed);
}

/// Reports an unfinished target only for effects that stop on arrival.
/// A plain != adds two bytes to the English function.
u32 FieldParticle_HasPendingTarget(struct FieldParticle *effect) {
  u32 value;

  if (effect->stopAtTarget == 0) {
    return 0;
  }
  value = (u32)effect->targetX ^ (u32)EFFECT_NO_TARGET;
  return ((0u - value) | value) >> 31;
}

#include "field_sprite.h"

void FieldParticle_SetPosition(struct FieldParticle *effect, s32 x, s32 z) {
  effect->targetX = EFFECT_NO_TARGET;
  effect->targetZ = EFFECT_NO_TARGET;
  effect->x = x;
  effect->z = z;
  effect->speed = 0;
}

void FieldParticle_SetActorMode(struct FieldParticle *effect, s32 mode) {
  Animation_ApplyChildArgumentFar(effect->object, mode);
}

void FieldParticle_SetCallback(struct FieldParticle *effect,
                               EffectCallback callback) {
  effect->callback = callback;
  effect->callbackDelay = 0;
  effect->age = 0;
  effect->state = 0;
}

/// Starts slot effect as a particle of resource object kind at (x, z): clears
/// it, creates its sprite in front, and sets it moving at up to 2 pixels a
/// frame toward its target at full size, turning a limited step at a time and
/// stopping on arrival, with a random value and z lifting the sprite.
void FieldParticle_Initialize(struct FieldParticle *effect, s32 kind, s32 x,
                              s32 z) {
  s32 unit;
  struct FieldSprite *object;
  volatile u32 zero;

  Dma_Fill32Cell(zero, 0, effect, sizeof *effect);
  object = (struct FieldSprite *)Sprite_CreateFar(kind);
  effect->object = object;
  if (object != NULL)
    object->priority = 0;
  FieldParticle_SetPosition(effect, x, z);
  unit = 0x10000;
  effect->maxSpeed = 0x20000;
  effect->acceleration = effect->scaleY = effect->scaleX = unit;
  effect->originX = x;
  effect->originZ = z;
  // The sprite flags (nextFrame), through a byte pointer: as a field of
  // struct FieldSprite the origin stores move after it.
  ((s8 *)effect->object)[26] = 0;
  effect->stopAtTarget = 1;
  effect->limitTurnStep = 1;
  effect->updateMotion = 1;
  effect->render = 1;
  effect->active = 1;
  effect->randomValue = Random16();
  effect->flags = 4;
  FieldParticle_SetActorMode(effect, 1);
}

void FieldEffect_ClearOwnedSlot(struct FieldParticle *slot) {
  if (slot->object)
    Sprite_DestroyFar((struct ResourceObjectWork *)slot->object);
  Dma_Fill32(0, slot, sizeof *slot);
}

#undef SparkWork
#undef BG_VRAM

// ---- field/event_commands/world_picture.c ----

/// The world map screen: its markers and cursor (The Broken Seal's
/// world_picture.c), and where the leader stands on it.

#include "field/world_map.h"

void WorldPicture_Initialize(void) {
  u32 *markerTiles;
  struct WorldMapWork *work = Ram_WorldMapWork;
  u32 *oam;
  s32 screenX;
  s32 screenY;
  s32 tileAttributes;
  u32 i;
  u32 tile;
  u32 cursorAttributes;
  s32 right;
  s32 bottom;
  s32 left;

  markerTiles = Heap_AllocEwram(32);
  oam = work->markers;
  Actor_GetById(Party_GetLeaderActor());
  screenX = 0;
  screenY = 0;
  work->vramBlock = VramCache_FindFreeEntry();
  work->unk644 = 0;
  work->unk648 = 0;
  Dma_Fill32(0, markerTiles, 0x80);
  markerTiles[0] = 255;
  markerTiles[1] = 0x1ff;
  markerTiles[2] = 272;
  markerTiles[8] = 68;
  markerTiles[9] = 324;
  markerTiles[10] = 272;
  markerTiles[16] = 119;
  markerTiles[17] = 0x177;
  markerTiles[18] = 272;
  markerTiles[24] = 0xff0;
  markerTiles[25] = 0xffff;
  markerTiles[26] = 0x1ffff;
  markerTiles[27] = 0x11ff0;
  markerTiles[28] = 0x1100;
  tileAttributes = VramCache_Load(work->vramBlock, 0x80, markerTiles) | 0x400;
  for (i = 0; i < 128; i++) {
    *oam++ = 0;
    *oam++ = 0;
    *oam++ = tileAttributes;
  }
  Heap_Release(markerTiles);
  if (GameFlag_Get(284)) {
    work->x = 264 << 16;
    work->z = 16 << 16;
  } else {
    WorldPicture_GetLeaderPoint(&screenX, &screenY);
    work->x = screenX << 16;
    work->z = screenY << 16;
  }
  screenX = work->x - (120 << 16);
  screenY = work->z - (80 << 16);
  left = 0;
  right = 272 << 16;
  bottom = 224 << 16;
  if (screenX < left)
    screenX = left;
  if (screenX > right)
    screenX = right;
  if (screenY < left)
    screenY = left;
  if (screenY > bottom)
    screenY = bottom;
  Ram_WorldMapCursorStart[0] = work->x;
  Ram_WorldMapCursorStart[1] = work->z;
  work->windowSlot = Resource_LoadFixedBlockBIntoFreeSlotFar();
  tile = gResourceTableEntries[work->windowSlot].offset;
  work->window = Window_CreateFar(0, 0, 0, 0, 2);
  work->shown = -1;
  work->speed = 0x10000;
  cursorAttributes = 0x40000000;
  *oam++ = 0;
  *oam++ = cursorAttributes;
  cursorAttributes = 0x400;
  cursorAttributes |= tile >> 5;
  *oam = cursorAttributes;
  *Ram_WorldMapCursor = &work->x;
  Ram_WorldMapScroll[0] = screenX;
  Ram_WorldMapScroll[1] = screenY;
}

void WorldPicture_Release(void) {
  VramCache_FreeEntry(gWorldMapWork.vramBlock);
  VramCache_FreeEntry(gWorldMapWork.windowSlot);
  Window_CloseFar((u32)gWorldMapWork.window, 2);
}

/// Unused: nothing calls it. Puts the leader at the world map position of
/// picture point (x, z), the inverse of WorldPicture_GetLeaderPoint's scale,
/// and the camera on it.
void WorldPicture_PlaceLeaderAt(s32 x, s32 z) {
  s32 owner = Party_GetLeaderActor();
  s32 *camera = *(s32 **)Ram_HeapSlots->mapWork;
  struct Actor *object = ObjectTable_Get(owner);

  if (object != NULL) {
    x >>= 16;
    x = ((x << 14) + 0x434000) / 853;
    z >>= 16;
    object->x = x << 16;
    object->z = (((z << 14) + 0x1c0000) / 640) << 16;
    camera[0] = object->x;
    camera[2] = object->z;
  }
}

#include "version.h"

/// The leader's point on the world map picture: its world map position scaled
/// by 853 and 640 in 16384, a far corner of the map folded in (flag 286 set
/// while the leader is there); off the world map, the picture's fixed point
/// (264, 16).
void WorldPicture_GetLeaderPoint(s32 *screenX, s32 *screenY) {
  s32 leader = Party_GetLeaderActor();
  u8 *state = Ram_HeapSlots->eventWork;
  u8 *object;
  s32 x;
  s32 z;

  GameFlag_Clear(286);
  if (*(s8 *)(state + 394) == 3) {
    object = ObjectTable_Get(leader);
    if (object == 0)
      return;
    x = *(s16 *)(object + 10);
    z = *(s16 *)(object + 18);
    if (z < 1832) {
      if (x < 12000) {
        GameFlag_Set(286);
        // From patch 1 this case is offset from the leader's position; the
        // Japanese release puts it at one fixed point.
#if TLA_PATCH_1
        x += 65;
        z += 1966;
#else
        x = 8872;
        z = 3320;
#endif
      } else {
        x -= 627;
        z += 6084;
      }
    }
    *screenX = 853 * x / 16384 - 269;
    *screenY = z * 640 / 16384 - 112;
  } else {
    *screenX = 264;
    *screenY = 16;
  }
}

/// Returns entry[1]'s high nibble when gData_0202a642 is set, else its low
/// nibble.
u32 Func_080ec298(u8 *entry) {
  if (gData_0202a642 != 0)
    return entry[1] >> 4;
  return entry[1] & 15;
}

// ---- field/event_commands/world_map_picture.c ----

/// The world map screen's picture: drawing it, scrolling it with the cursor,
/// its area markers, and zooming the view out while L is held.

#include "text/msg_ids.h"

// The world map screen's work, laid out as field/world_map.h has it.

// The place's position on the world map screen.
#define PLACE_SCREEN_X(place) ((place)->x * 853 / 16384 - 269)
#define PLACE_SCREEN_Y(place) ((place)->z * 640 / 16384 - 112)

/// R (step 1) and L (step -1) on the world map screen: the cursor jumps to the
/// next or the previous marked place round from the one nearest it, and its
/// point on the picture is written to screenX and screenY.
void WorldPicture_StepToPlace(s32 *screenX, s32 *screenY, s32 step) {
  struct WorldMapPlace *places;
  struct WorldMapPlace *place;
  struct WorldMapPlace *next;
  s32 best;
  s32 bestDistance;
  s32 cursorX;
  s32 cursorY;
  s32 count;
  s32 wraps;
  s32 flag;
  u32 state;
  struct WorldMapWork *work = Ram_WorldMapWork;

  Party_GetLeaderActor();
  places = FlagEntries_GetTable();
  bestDistance = 0x4000000;
  best = -1;
  cursorX = work->x >> 16;
  cursorY = work->z >> 16;
  if (work->unk644 != 0) {
    cursorX = work->unk644 >> 16;
    cursorY = work->unk648 >> 16;
  }
  for (count = 0; count < 128; count++) {

    place = &places[count];
    if (place->flag == 0)
      break;
    flag = place->flag + 0x180;
    state = Func_080ec298(place);

    if (state != 0 && state != 15 && GameFlag_Get(flag)) {
      s32 sx = PLACE_SCREEN_X(place);
      s32 sy = PLACE_SCREEN_Y(place);
      s32 dx = sx - cursorX;
      s32 dy = sy - cursorY;
      s32 distance = dx * dx + dy * dy;

      if (bestDistance > distance) {
        bestDistance = distance;
        best = count;
      }
    }
  }
  if (best == -1) {
    WorldPicture_GetLeaderPoint(screenX, screenY);
    return;
  }
  place = &places[best];
  if (bestDistance <= 4) {
    wraps = 0;
    do {
      best += step;
      if (best < 0) {
        best = count - 1;
        wraps++;
      } else {
        next = &places[best];
        if (next->flag == 0) {
          best = 0;
          wraps++;
        } else {
          flag = next->flag + 0x180;
          state = Func_080ec298(next);

          if (state != 0 && state != 15 && GameFlag_Get(flag)) {
            place = next;
            break;
          }
        }
      }
    } while (wraps <= 1);
  }
  *screenX = PLACE_SCREEN_X(place);
  *screenY = PLACE_SCREEN_Y(place);
}

// Moves a list of window sprites.

#include "graphics/render_input.h"

/// Moves every window sprite of a list by dx, dy: the output's position and its
/// queued sprite's OAM x and y.
void WindowSpriteList_Move(struct WindowSpriteList *list, s32 dx, s32 dy) {
  struct WindowSprite *output;
  struct QueuedSprite *sprite;

  if (list == NULL)
    return;
  output = list->head;
  while (output != NULL) {
    output->x += dx;
    output->y += dy;
    sprite = &output->sprite;
    output = output->next;
    sprite->oam.x += dx;
    sprite->oam.y += dy;
  }
}
#undef PLACE_SCREEN_X
#undef PLACE_SCREEN_Y

#include "input.h"
#include "io_write_queue.h"
#include "edition.h"

// The scrolled name's left edge, in pixels left of its place.
#if LANGUAGE == LANGUAGE_GERMAN
#define WORLD_MAP_NAME_LEFT 3
#else
#define WORLD_MAP_NAME_LEFT 1
#endif

/* VIEW(f0ec4d4): tag struct WorldMapWork as struct WorldMapWork__f0ec4d4;
 * f0ec2b8.c has another definition; tried with the definition the rest of the
 * object has, it does not compile in tla-ja:
 * games/tla/src/field/event_commands/radial.c:10444: structure has no member
 * named `scrollX'. */
struct WorldMapWork__f0ec4d4 {
  u16 vramBlock;
  u16 windowSlot;
  s32 x;
  s32 z;
  u8 unk0c[6];
  s16 shown;
  u8 unk14[4];
  s32 speed;
  void *window;
  struct WorldMapMarker markers[129];
  u8 unk62c[4];
  s32 scrollX;
  s32 scrollZ;
};

/* VIEW(f0ec4d4): tag struct WorldMapPlace as struct WorldMapPlace__f0ec4d4;
 * f0ec2b8.c has another definition; tried with the definition the rest of the
 * object has, it does not compile in tla-ja:
 * games/tla/src/field/event_commands/radial.c:10517: structure has no member
 * named `name'. */
struct WorldMapPlace__f0ec4d4 {
  u8 flag;
  u8 value;
  s16 unk02;
  s16 x;
  s16 z;
  u8 unk08[4];
  s16 name;
  s16 nameEntrance;
  u8 unk10[4];
};

#define WORK Ram_WorldMapWork
#define NAME_BOX_X Ram_WorldMapNameBox[0]
#define NAME_BOX_Y Ram_WorldMapNameBox[1]
#define NAME_BOX_WIDTH Ram_WorldMapNameBox[2]
#define NAME_BOX_HEIGHT Ram_WorldMapNameBox[3]

void Text_DrawMessageFar(s32 message, s32 window, s32 x, s32 y);

#define WORLD_MAP_HIGH(value) (((s16 *)&(value))[1])
#define WORLD_MAP_SCALE(value) ((value) * 300 / 640)

void WorldPicture_UpdateMarkers(void) {
  struct WorldMapWork__f0ec4d4 *work;
  struct WorldMapMarker *marker;
  s32 tileBase;
  s32 blend;
  s32 nearest;
  s32 nearestDistance;
  s32 nearestX;
  s32 nearestY;
  s32 name;
  s32 nearestName;
  s32 nearestEntrance;
  s32 entrance;
  struct WorldMapPlace__f0ec4d4 *places;
  s32 scrollX;
  s32 scrollY;
  struct WorldMapPlace__f0ec4d4 *place;
  s32 width;
  s32 height;
  s32 placeX;
  s32 placeY;
  s32 pos[3];
  s32 x;
  s32 y;
  s32 dx;
  s32 dz;
  s32 distance;
  s32 step;
  s32 cursorX;
  s32 cursorY;
  u16 angle;
  s32 tile;
  s32 always;
  s32 flag;
  s32 i;

  Party_GetLeaderActor();
  work = WORK;
  marker = work->markers;
  tileBase = gResourceTableEntries[work->vramBlock].offset >> 5;
  nearest = -1;
  nearestDistance = 64;
  nearestX = 0;
  nearestY = 0;
  places = FlagEntries_GetTable();
  scrollX = work->scrollX & 0xffff0000;
  scrollY = work->scrollZ & 0xffff0000;
  blend = gWorldMap_MarkerBlendCycle[(gFrameCount >> 1) & 31];

  if (Data_0202a644 != 0) {
    x = Data_0202a644;
    y = Data_0202a648;
    dx = (x - work->x) / Q16_ONE;
    dz = (y - work->z) / Q16_ONE;
    distance = Iwram_Sqrt(dx * dx + dz * dz) << 16;
    dx = x - work->x;
    dz = y - work->z;
    if (distance < Q16_ONE * 64)
      distance = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz));
    step = distance / 4;
    if (step > Q16_ONE * 8)
      step = Q16_ONE * 8;
    if (distance < Q16_ONE / 2) {
      work->x = x;
      work->z = y;
      Data_0202a644 = 0;
      Data_0202a648 = 0;
    } else {
      if (distance > step) {
        dx = Iwram_MulQ16(Iwram_RatioMulQ14(distance, dx), step);
        dz = Iwram_MulQ16(Iwram_RatioMulQ14(distance, dz), step);
      }
      work->x += dx;
      work->z += dz;
    }
  } else {
    angle = gWorldMap_CursorDirectionAngles[(gInput.held >> 4) & 15];
    if (angle != 0xffff) {
      pos[0] = work->x;
      pos[1] = 0;
      pos[2] = work->z;
      Vector_AddPolarOffset(work->speed, angle, pos);
      if (pos[0] < Q16_ONE * 16)
        pos[0] = Q16_ONE * 16;
      if (pos[0] > Q16_ONE * 511)
        pos[0] = Q16_ONE * 511;
      if (pos[2] < 0)
        pos[2] = 0;
      if (pos[2] > Q16_ONE * 365)
        pos[2] = Q16_ONE * 365;
      work->x = pos[0];
      work->z = pos[2];
      Ram_WorldMapCursorStart[0] = pos[0];
      Ram_WorldMapCursorStart[1] = pos[2];
      if (work->speed < Q16_ONE * 6)
        work->speed += Q16_ONE / 8;
    }
  }

  cursorX = WORLD_MAP_HIGH(work->x);
  cursorY = WORLD_MAP_HIGH(work->z);
  place = places;
  i = 0;
  do {
    if (place->flag == 0) {
      if (gData_0202a642 != 0)
        break;
      place = &places[128];
      i = 128;
      name = -1;
      tile = 0;
      always = 0;
      WorldPicture_GetLeaderPoint(&placeX, &placeY);
    } else {
      name = place->name;
      entrance = place->nameEntrance;
      flag = place->flag + 0x180;
      tile = Func_080ec298(place);
      if (tile == 0)
        continue;
      if (tile == 15) {
        if (!GameFlag_Get(0x8de))
          continue;
        if (gMainRoutineWork.ownerOverride != 0)
          continue;
        name = -2;
        tile = 2;
        always = 0;
        placeX = (gMainRoutineWork.savedX >> 16) * 853 / 16384 - 269;
        placeY = (gMainRoutineWork.savedZ >> 16) * 640 / 16384 - 112;
      } else {
        if (!GameFlag_Get(flag))
          continue;
        always = 1;
        placeX = place->x * 853 / 16384 - 269;
        placeY = place->z * 640 / 16384 - 112;
      }
    }
    marker->objectMode = always;
    marker->tile = tileBase + tile;
    if (Data_0202a640 == 0) {
      x = placeX - (scrollX >> 16) - 1;
      y = placeY - (scrollY >> 16) - 1;
      if ((u32)(x + 4) > 247)
        continue;
      if (y < -4)
        continue;
      if (y > 163)
        continue;
      marker->x = x & 0x1ff;
      marker->y = y;
    } else {
      marker->x = WORLD_MAP_SCALE(placeX) - 1;
      marker->y = WORLD_MAP_SCALE(placeY) - 1;
    }
    width = placeX - cursorX;
    height = placeY - cursorY;
    if (width * width + height * height < nearestDistance) {
      nearest = i;
      nearestName = name;
      nearestEntrance = entrance;
      nearestDistance = width * width + height * height;
      nearestX = placeX;
      nearestY = placeY;
    }
    if (always != 0 || (gFrameCount & 15) <= 7)
      Oam_AddSpriteAtDepthClamped(marker++, 246);
  } while (++i, place++, i < 128);

  if (gInput.held == 0) {
    if (nearest != -1) {
      if (nearestDistance <= 2) {
        work->x = nearestX << 16;
        work->z = nearestY << 16;
        work->speed = Q16_ONE;
      } else {
        angle = ArcTan2(nearestY - cursorY, nearestX - cursorX);
        pos[0] = work->x;
        pos[1] = 0;
        pos[2] = work->z;
        Vector_AddPolarOffset(work->speed, (u16)angle, pos);
        work->x = pos[0];
        work->z = pos[2];
      }
      cursorX = WORLD_MAP_HIGH(work->x);
      cursorY = WORLD_MAP_HIGH(work->z);
    } else {
      work->speed = Q16_ONE;
    }
  }

  if (nearest != -1 && (gFrameCount & 15) <= 7) {
    marker->objectMode = 0;
    marker->tile = tileBase + 3;
    if (Data_0202a640 == 0) {
      marker->x = nearestX - (scrollX >> 16) - 2;
      marker->y = nearestY - (scrollY >> 16) - 2;
    } else {
      marker->x = WORLD_MAP_SCALE(nearestX) - 2;
      marker->y = WORLD_MAP_SCALE(nearestY) - 2;
    }
    Oam_AddSpriteAtDepthClamped(marker, 246);
  }

  marker = &Data_0202a620;
  if (Data_0202a640 == 0) {
    marker->affineIndex = 0;
    marker->x = cursorX - (scrollX >> 16) - 17;
    marker->y = cursorY - (scrollY >> 16) + 1;
  } else {
    marker->x = WORLD_MAP_SCALE(cursorX) - 17;
    marker->y = WORLD_MAP_SCALE(cursorY) + 1;
  }
  Oam_AddSpriteAtDepthClamped(marker, 246);

  if (work->shown != nearest) {
    // The Japanese release prepares the window and draws the new name in the
    // same frame; the later ones draw it 6 frames after preparing the window.
#if TLA_PATCH_1
    if (Data_0202a654[0] == 0) {
      WindowSprite_PrepareForRedrawFar(work->window);
      Data_0202a654[0] = 1;
      Data_0202a656[0] = 6;
    } else if (--Data_0202a656[0] <= 0) {
      Data_0202a654[0] = 0;
      work->shown = nearest;
#else
    {
      work->shown = nearest;
      WindowSprite_PrepareForRedrawFar(work->window);
#endif
      if (nearest != -1) {
        if (nearestName == -1)
          nearestName = (s32)MsgPresentLocation;
        else if (nearestName == -2)
          nearestName = (s32)MsgBoat;
        else
          nearestName = Area_GetNameIndex(nearestName, nearestEntrance) +
                        (s32)MsgDebugAreaName;
        Text_MeasureMessageFar(nearestName, &width, &height);
        if (Data_0202a640 == 0) {
          cursorX = nearestX - WORLD_MAP_NAME_LEFT;
          cursorY = nearestY - 11;
          cursorX -= scrollX >> 16;
          cursorY -= scrollY >> 16;
        } else {
          cursorX = WORLD_MAP_SCALE(nearestX) - 1;
          cursorY = WORLD_MAP_SCALE(nearestY) - 11;
        }
        if (cursorX + width > 239) {
          cursorX = 232 - width;
          cursorY += -10;
        }
        if (cursorY < 0)
          cursorY = 0;
        Text_DrawMessageFar(nearestName, work->window, cursorX, cursorY);
        {
          // One local carries width, then height, into the name box.
          s32 value = width;
          NAME_BOX_WIDTH = value;
          value = height;
          NAME_BOX_HEIGHT = value;
        }
        NAME_BOX_X = cursorX;
        NAME_BOX_Y = cursorY;
      }
    }
  } else if (nearest != -1) {
    width = NAME_BOX_WIDTH;
    height = NAME_BOX_HEIGHT;
    if (Data_0202a640 == 0) {
      cursorX = nearestX - WORLD_MAP_NAME_LEFT;
      cursorY = nearestY - 11;
      cursorX -= scrollX >> 16;
      cursorY -= scrollY >> 16;
    } else {
      cursorX = WORLD_MAP_SCALE(nearestX) - 1;
      cursorY = WORLD_MAP_SCALE(nearestY) - 11;
    }
    if (cursorX + width > 239) {
      cursorX = 232 - width;
      cursorY += -10;
    }
    if (cursorY < 0)
      cursorY = 0;
    width = cursorX - NAME_BOX_X;
    /* FAKEMATCH(temporary): the name box's y read through a pointer to a
       struct with an s32 member standing in for its own address, so its
       load may alias width's store to the stack and stays after it; as
       NAME_BOX_Y, 10 of 1044 instructions differ in tla-en. */
    height = cursorY - ((struct WorldMapAliasedHalfword *)&NAME_BOX_Y)->value;
    WindowSpriteList_Move(work->window, width, height);
    NAME_BOX_X = cursorX;
    NAME_BOX_Y = cursorY;
  }

  IO_WRITE_QUEUE_PUSH(0x04000050, 0x3f00, 0x20000);
  IO_WRITE_QUEUE_PUSH(0x04000052, ((16 - blend) << 8) | blend, 0x20000);
  IO_WRITE_QUEUE_PUSH(0x04000054, 0, 0x20000);
}
#undef WORLD_MAP_NAME_LEFT
#undef WORK
#undef NAME_BOX_X
#undef NAME_BOX_Y
#undef NAME_BOX_WIDTH
#undef NAME_BOX_HEIGHT
#undef WORLD_MAP_HIGH
#undef WORLD_MAP_SCALE

#include "dma.h"
#include "resource.h"
#include "bg_scroll.h"
#include "field/world_map.h"

extern char ResourceId_Data023, ResourceId_Data024;

/// Draws the world map picture on BG1, scrolled to the cursor in gWorldMapWork
/// (resource 36, or 35 at the origin when the halfword at 0x640 is set): the
/// palette, then 21 rows of 31 tiles from the decoded rows, wrapping the 24-row
/// buffer and the 32-column screen.
void WorldMapPicture_Draw(void) {
  struct WorldMapWork *work = &gWorldMapWork;
  u8 *buffer;
  s32 resource = (s32)&ResourceId_Data024;
  s32 *cursor = work->cursor;
  s32 x = *cursor++ - (120 << 16);
  s32 z = *cursor - (80 << 16);
  u32 *graphics;
  u32 column;
  u32 firstRow;
  u32 row;
  s32 bufferRow;
  s32 unwrapped;
  u32 i;
  s32 right;
  s32 bottom;
  s32 tile;

  if (work->centered != 0) {
    resource = (s32)&ResourceId_Data023;
    x = 0;
    z = 0;
  }
  graphics = Resource_GetTableEntry(resource);
  Dma_Set((u8 *)graphics + graphics[0], (void *)0x05000000, 0x84000078,
          (volatile u32 *)0x040000d4);
  *(u16 *)0x05000000 = 0;
  buffer = Ram_MapCellBuffer;
  right = 272 << 16;
  bottom = 224 << 16;
  if (x < 0)
    x = 0;
  if (x > right)
    x = right;
  if (z < 0)
    z = 0;
  if (z > bottom)
    z = bottom;
  gBgScroll[1].x = x >> 16;
  gBgScroll[1].y = z >> 16;
  x = (u32)x >> 19;
  column = x;
  firstRow = (u32)z >> 19;
  for (row = 0; row <= 20; row++) {
    bufferRow = firstRow + row;
    unwrapped = bufferRow;
    /* FAKEMATCH(barrier): an empty asm reading x, which keeps it live over the
       row's start; without it, 118 of 149 instructions differ in tla-en. */
    asm("" : : "r"(x));
    if (bufferRow > 23)
      bufferRow -= 24;
    // z is reused for the map cell's index; a local of its own differs.
    z = (column & 31) + (unwrapped << 5);
    tile = (z + 128) % 768 + 256;
    Decompress_TaggedLz((u8 *)graphics + graphics[unwrapped + 1],
                        buffer + (bufferRow << 12));
    // x is reused for the cell pointer; a local of its own differs.
    x = bufferRow << 6;
    x += column;
    {
      s32 to;

      i = 0;
      to = 0x06000000;
      x <<= 6;
      x += (s32)buffer;
      /* FAKEMATCH(barrier): VRAM's address through an empty asm that also
         reads i, which builds it once in r7 ahead of the loop; without it, 10
         of 149 instructions differ in tla-en. */
      asm("" : "=r"(to) : "0"(to), "r"(i));
      for (; i <= 30; i++) {
        Dma_Set((void *)x, (void *)((tile << 6) + to), 0x84000010,
                (volatile u32 *)0x040000d4);
        x += 64;
        tile++;
        if (i + column == 31)
          tile -= 32;
      }
    }
  }
}

#include "io_reg.h"

/// Set while the world map shows its alternative picture, which does not
/// scroll.
extern s16 Data_0202a640;

#define BG_VRAM 0x06000000

/// Draws one row of the world map picture on BG1, from a column to the right
/// edge of the map (at most 31 tiles; Japanese always draws 32), for
/// scrolling: decodes the picture's row into the cell buffer, which holds 24
/// rows and wraps, and copies its cells into the 32-column screen from tile
/// 256, wrapping at column 32.
void WorldMapPicture_DrawRow(u32 column, s32 row) {
  s32 resource = (s32)&ResourceId_Data024;
  u32 *graphics;
  s32 unwrapped;
  s32 cell;
  s32 tile;
  u32 i;

  if (Data_0202a640 != 0)
    resource = (s32)&ResourceId_Data023;
  graphics = Resource_GetTableEntry(resource);
  unwrapped = row;
  if (row > 23)
    row -= 24;
  cell = (row << 6) + column;
  tile = (column & 31) + (row << 5);
  tile = (tile + 128) % 768 + 256;
  Decompress_TaggedLz((u8 *)graphics + graphics[unwrapped + 1],
                      Ram_MapCellBuffer + (row << 12));
  // From patch 1 the copy stops at the map's last column instead of running
  // past it.
#if TLA_PATCH_1
  for (i = 0; i <= 30 && column + i <= 63; i++) {
#else
  for (i = 0; i < 32; i++) {
#endif
    Dma_Set(Ram_MapCellBuffer + (cell << 6), (void *)(BG_VRAM + (tile << 6)),
            DMA_COPY_WORDS(16), REG_DMA3);
    cell++;
    tile++;
    if (i + column == 31)
      tile -= 32;
  }
}

static inline s32 WorldMap_AddColumn(s32 column, s32 cell) {
  /* FAKEMATCH(wrapper): the column added through an inline, so the sum
   * keeps the column as its first operand; as column + cell, 2 of 51
   * instructions differ in tla-en. */
  return column + cell;
}

/// Draws one column of the world map picture on BG1, 21 rows down from a
/// row, for scrolling: copies the column's cells from the decoded rows in the
/// cell buffer, which wraps after 24 rows, into the 32-column screen from
/// tile 256; the column twin of WorldMapPicture_DrawRow.
void WorldMapPicture_DrawColumn(s32 column, s32 row) {
  u32 i;

  for (i = 0; i <= 20; i++) {
    s32 line = row + i;
    s32 cell = line;
    s32 tile;

    if (line > 23)
      cell = line - 24;
    tile = ((column & 31) + (line << 5) + 128) % 768;
    cell <<= 6;
    cell = WorldMap_AddColumn(column, cell);
    cell <<= 6;
    tile <<= 6;
    // line is reused for the screen's address; as (u8 *)BG_VRAM + (256 << 6) +
    // tile in the call, 6 of 51 instructions differ in tla-en.
    line = BG_VRAM + (256 << 6);
    Dma_Set(Ram_MapCellBuffer + cell, (u8 *)line + tile, DMA_COPY_WORDS(16),
            REG_DMA3);
  }
}

/// The world map cursor's position (x, then y, 16.16 pixels).
extern s32 *Data_0202a62c;

#define SCROLL_X Ram_WorldMapScroll[0]
#define SCROLL_Y Ram_WorldMapScroll[1]

/// The world map screen's scroll follows the cursor. The target puts the
/// cursor 120 by 80 pixels in (clamped to the map); unless the map is held the
/// scroll moves a quarter of the way there each frame, at most 8 pixels, and
/// stops when nearer than a quarter pixel. Each time the scroll crosses an
/// 8-pixel column or row, the new column (WorldMapPicture_DrawColumn) or row
/// (WorldMapPicture_DrawRow) of the picture is drawn; BG1 scrolls with it. All
/// values are 16.16 pixels.
void WorldMapPicture_UpdateScroll(void) {
  s32 *cursor = Data_0202a62c;
  s32 x = *cursor++ - (120 << 16);
  s32 y = *cursor - (80 << 16);
  s32 dx;
  s32 dy;
  s32 distance;
  s32 step;
  u32 column;
  u32 row;
  s32 limit;
  s32 bottom = 0xe00000;

  if (x < 0)
    x = 0;
  limit = 0x1100000;
  if (x > limit)
    x = limit;
  if (y < 0)
    y = 0;
  if (y > bottom)
    y = bottom;
  if (Data_0202a640 == 0) {
    dx = (x - SCROLL_X) / Q16_ONE;
    dy = (y - SCROLL_Y) / Q16_ONE;
    distance = Iwram_Sqrt(dx * dx + dy * dy) << 16;
    dx = x - SCROLL_X;
    dy = y - SCROLL_Y;
    // Near the target the whole-pixel distance is too coarse.
    if (distance < 0x400000)
      distance = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dy, dy));
    step = distance / 4;
    if (step > 0x80000)
      step = 0x80000;
    if (distance >= 0x4000) {
      if (distance > step) {
        dx = Iwram_MulQ16(Iwram_RatioMulQ14(distance, dx), step);
        dy = Iwram_MulQ16(Iwram_RatioMulQ14(distance, dy), step);
      }
      x = SCROLL_X + dx;
      y = SCROLL_Y + dy;
      if (x < 0)
        x = 0;
      if (y < 0)
        y = 0;
      if (x > limit)
        x = limit;
      if (y > limit)
        y = limit;
    }
    column = (u32)x >> 19;
    row = (u32)y >> 19;
    if ((SCROLL_X ^ x) & 0x80000) {
      if (SCROLL_X < x)
        WorldMapPicture_DrawColumn(column + 30, row);
      else
        WorldMapPicture_DrawColumn(column, row);
    }
    if ((SCROLL_Y ^ y) & 0x80000) {
      if (SCROLL_Y < y)
        WorldMapPicture_DrawRow(column, row + 20);
      else
        WorldMapPicture_DrawRow(column, row);
    }
    gBgScroll[1].x = x >> 16;
    gBgScroll[1].y = y >> 16;
  }
  SCROLL_X = x;
  SCROLL_Y = y;
}

#include "io_write.h"
#include "scene_ids.h"
#include "event_work.h"
#include "actor.h"
#include "map.h"
#include "field/sprite.h"
#include "flag_call.h"
#include "text/msg_ids.h"

extern s32 Data_0202a644;
extern s32 Data_0202a648;
extern s32 Data_0202a64c;
extern s32 Data_0202a650;
extern s32 Data_0202a630;
extern s32 Data_0202a634;

void ObjectTable_Snapshot(void);
void Task_ResetAll(void);
void wait(s32 frames);
void WorldPicture_Initialize(void);
void WorldPicture_UpdateMarkers(void);
void WorldPicture_StepToPlace(s32 *screenX, s32 *screenY, s32 step);
void WorldMap_SetReturnForScene(s32 scene, s32 entrance);
void WorldPicture_Release(void);
void WorldMap_LoadAreaGraphicsFar(void);

s32 WorldPicture_Show(s32 mode) {
  s32 savedDelay;
  u8 savedFlags[16];
  s32 screenX;
  s32 screenY;
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct EventWork *event = Ram_HeapSlots->eventWork;
  struct ObjectSystemWork *objects = Ram_HeapSlots->objectSystem;
  s16 blend;
  s32 messageShown = 0;
  s32 zoomed = 0;
  s32 gliding = 0;
  struct WorldMapWork *world = &gWorldMapWork;
  struct MapAnimation *layer;
  const struct FlagEntry *place;
  u32 *tiles;
  u32 tile;
  s32 row;
  s32 column;
  s32 i;
  struct InputState *input;
  struct InputState *keys;
  s32 *wayX;
  s32 *wayZ;
  s32 *cursorX;
  s32 *cursorZ;
  s32 *scrollX;
  s32 *scrollZ;
  s32 zero;
  s32 limitX;
  s32 limitZ;
  s32 value;
  struct WorldMapChoice *choice = &Data_02000500;
  struct MainRoutineWork *routine = &gMainRoutineWork;

  world->mode = mode;
  if (mode != 0 || event->controlMode != 3)
    ObjectTable_Snapshot();
  if (event->controlMode == 3) {
    savedDelay = event->delay;
    event->delay = 6;
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    layer = map->anim;
    for (i = 0; i < 16; i++) {
      savedFlags[i] = layer->paused;
      layer->paused = 1;
      layer++;
    }
    objects->suspended = 1;
    wait(1);
    ((struct MapViewWork *)map)->picturePending = 1;
    do {
      // A once-only block: as a plain statement, 16 of 548 instructions differ.
      blend = REG_BLDALPHA;
    } while (0);
  } else {
    do {
      // A once-only block: as a plain statement, 281 of 548 instructions
      // differ.
      event->viewCenter->movementState = 1;
    } while (0);
    savedDelay = event->delay;
    event->delay = 6;
    event->screenEffect = 0;
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Task_ResetAll();
    WindowSystem_InitFieldFar();
    objects->suspended = 1;
    wait(1);
    blend = REG_BLDALPHA;
    tiles = (u32 *)0x06003000;
    tile = 0x01810180;
    for (row = 0; row < 20; row++)
      for (column = 0; column < 16; column++) {
        *tiles++ = tile;
        tile += 0x20002;
      }
    tile = 0x01010100;
    for (row = 0; row < 24; row++)
      for (column = 0; column < 16; column++) {
        *tiles++ = tile;
        tile += 0x20002;
      }
    tile = 0x01010100;
    for (row = 0; row < 4; row++)
      for (column = 0; column < 16; column++) {
        *tiles++ = tile;
        tile += 0x20002;
      }
    map->origin = 0;
    gBgScroll[1].x = 0;
    gBgScroll[1].y = 0;
    Io_Write16(0x682, &REG_BG1CNT);
    Io_Write16(0x1340, &REG_DISPCNT);
  }
  // Default: no pointer yet.
  wayX = NULL;
  cursorX = NULL;
  cursorZ = NULL;
  scrollX = NULL;
  scrollZ = NULL;
  WorldPicture_Initialize();
  for (;;) {
    Data_0202a640 = zoomed;
    WorldMapPicture_Draw();
    wait(1);
    Io_Write16(0x8682, &REG_BG1CNT);
    Io_Write16(0x1340, &REG_DISPCNT);
    if ((GameFlag_Get(284) || GameFlag_Get(286)) && !messageShown) {
      FieldMessage_ShowFar((s32)MsgNotOnMap, 1);
      messageShown = 1;
    }
    keys = &gInput;
    wayX = &Data_0202a64c;
    cursorX = &Data_0202a644;
    cursorZ = &Data_0202a648;
    for (;;) {
      WorldMapPicture_UpdateScroll();
      WorldPicture_UpdateMarkers();
      input = keys;
      wait(1);
      if ((input->repeat & 8) && gliding == 0) {
        WorldPicture_GetLeaderPoint(&screenX, &screenY);
        *cursorX = screenX << 16;
        // DEAD: input set again to the &gInput it already holds; the ROM keeps
        // the reload.
        input = &gInput;
        *cursorZ = screenY << 16;
        gliding = 1;
      }
      if (!(keys->held & 8) && gliding == 1) {
        *cursorX = *wayX;
        // DEAD: input set again to the &gInput it already holds; the ROM keeps
        // the reload.
        input = &gInput;
        *cursorZ = Data_0202a650;
        gliding = 0;
      }
      if (keys->repeat & 0x100) {
        WorldPicture_StepToPlace(&screenX, &screenY, 1);
        *cursorX = screenX << 16;
        // wayZ only ever holds &Data_0202a650, set where it is used.
        wayZ = &Data_0202a650;
        *cursorZ = screenY << 16;
        *wayX = *cursorX;
        *wayZ = *cursorZ;
      }
      if (input->repeat & 0x200) {
        WorldPicture_StepToPlace(&screenX, &screenY, -1);
        *cursorX = screenX << 16;
        wayZ = &Data_0202a650;
        *cursorZ = screenY << 16;
        *wayX = *cursorX;
        *wayZ = *cursorZ;
      }
      if (input->repeat & 4) {
        world->shown = -1;
        WindowSprite_PrepareForRedrawFar(world->window);
        Io_Write16(64, &REG_DISPCNT);
        zoomed ^= 1;
        if (*cursorX != 0) {
          world->x = *cursorX;
          world->z = *cursorZ;
          zero = 0;
          limitX = 0x1100000;
          limitZ = 0xe00000;
          scrollX = &Data_0202a630;
          *scrollX = world->x - 0x780000;
          scrollZ = &Data_0202a634;
          *scrollZ = world->z - 0x500000;
          value = *scrollX;
          if (value < 0) {
            *scrollX = zero;
            value = *scrollX;
          }
          if (value > limitX)
            *scrollX = limitX;
          value = *scrollZ;
          if (value < 0) {
            *scrollZ = zero;
            value = *scrollZ;
          }
          if (value > limitZ)
            *scrollZ = limitZ;
        }
        break;
      }
      if (input->repeat & 1) {
        if (mode == 0)
          goto done;
        if (world->shown == -1)
          continue;
        place = FlagEntries_GetTable();
        place += world->shown;
        routine->unk2be = (s32)&SceneId_WorldMap;
        choice->flag = place->flag;
        WorldMap_SetReturnForScene(place->scene, place->entrance);
        goto done;
      }
      if (input->repeat & 2) {
        if (mode != 0) {
          routine->unk2be = 0xffff;
          {
            // The -1 in a local of its own; as choice->flag = -1, 40 of 548
            // instructions differ in tla-en.
            s32 none = -1;

            choice->flag = none;
          }
        }
        goto done;
      }
    }
  }
done:
  WorldPicture_Release();
  Io_Write16(64, &REG_DISPCNT);
  if (mode != 0 || event->controlMode != 3) {
    event->countdown = 999;
    GameFlag_Set(265);
  } else {
    WorldMap_LoadAreaGraphicsFar();
    IO_WRITE_QUEUE_PUSH(0x04000052, (u16)blend, 0x20000);
    layer = map->anim;
    for (i = 0; i < 16; i++) {
      layer->paused = savedFlags[i];
      layer++;
    }
    objects->suspended = 0;
    gBgScroll[1].x = 0;
    gBgScroll[1].y = 0;
    ((struct MapViewWork *)map)->picturePending = 0;
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    event->delay = savedDelay;
    event->viewCenter->movementState = 0;
  }
  return 0;
}

extern const struct FlagEntry gData_080f1140[];

void WorldMap_StartZoom(s32 scale, s32 frames);

/// On the world map, zooms the view out to eight thirteenths while L is held,
/// then back, six frames each way. FieldSequence_Run runs it for event -889.
void WorldMap_ZoomOutWhileLHeld(void) {
  struct MainRoutineWork *party;

  if (*(s8 *)((u8 *)Ram_HeapSlots->eventWork + 394) == 3) {
    Leader_StopMoving();
    WorldMap_StartZoom(Q16_ONE * 8 / 13, 6);
    while (gInput.held & KEY_L)
      wait(1);
    party = &gMainRoutineWork;

    if (party->ownerOverride != 0)
      WorldMap_StartZoom(Q16_ONE, 6);
    else
      WorldMap_StartZoom(Q16_ONE, 6);
  }
}

/// Returns the flag entry table, 20-byte entries ended by a zero flag.
const struct FlagEntry *FlagEntries_GetTable(void) { return gData_080f1140; }
#undef BG_VRAM
#undef SCROLL_X
#undef SCROLL_Y
