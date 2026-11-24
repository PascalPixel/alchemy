/// @file
/// The main image's copy of the overlays' effect spawner
/// (field_effect_spawn.c), an object of its own between the event commands:
/// Carry's ring of dust (FieldEffect_SpawnRadialParticleRing) spawns through
/// it.

#include "types.h"
#include "fixed_math.h"
#include "sprite_ids.h"
#include "system/field_effect.h"
#include "system/main_routine_state.h"
#include "field/field_sprite.h"
#include "field/objdisp.h"
#include "field/scene_tables.h"
#include "gamepak/far_call/field.h"
#include "field/sprite.h"

extern const s32 *gFieldEffect_DurationScripts[];

struct Actor *Actor_GetById(u32 objectId);
void Actor_SetSpriteColor(struct Actor *actor, s32 attribute);

/// Places a small effect at (x, y, z) moving at a velocity: a dust cloud, or
/// options' type when EFFECT_USE_TYPE asks for it, its script one of
/// gFieldEffect_DurationScripts and its options chosen by the EFFECT_USE_
/// flags.
void FieldEffect_SpawnResident(s32 x, s32 y, s32 z, s32 velocityX,
                               s32 velocityY, s32 velocityZ, u32 flags,
                               const struct EffectOptions *options) {
  // UB: actor 0 is Isaac's, on stage only while he leads; with Felix leading
  // Actor_GetById returns NULL, and the sprite priority below is read through
  // it. UBFIX takes the leader's, as the overlays' copy does.
#ifdef UBFIX
  struct Actor *party = Actor_GetById(gMainRoutineState.leaderActorId);
#else
  struct Actor *party = Actor_GetById(ACTOR_ISAAC);
#endif
  struct FieldEffect *effect;
  struct FieldSprite *sprite;
  const s32 *script;

  if ((flags & EFFECT_USE_TYPE) != 0 && options != 0)
    effect = (struct FieldEffect *)Actor_CreateFar(options->type, x, y, z);
  else
    effect =
        (struct FieldEffect *)Actor_CreateFar(EFFECT_DEFAULT_TYPE, x, y, z);
  if (effect == 0)
    return;

  sprite = effect->sprite;
  Actor_SelectAnimationFar((struct Actor *)effect,
                           (flags + 1) & EFFECT_SCRIPT_MASK);
  Actor_StartScriptFar(
      (void *)effect, gFieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK]);
  effect->motionFlags = 0;
  sprite->flags = 0;
  effect->update = FieldEffect_UpdateMotionScaleAndSpin;
  effect->velocity.x = velocityX;
  effect->velocity.y = velocityY;
  effect->velocity.z = velocityZ;
  sprite->priority = ((struct Sprite *)party->sprite)->image.priority;
  effect->scaleRateX = 0;
  effect->scaleRateY = 0;
  effect->spin = 0;

  if ((flags & EFFECT_OPTION_MASK) == 0 || options == 0)
    return;
  if ((flags & EFFECT_USE_PALETTE) != 0)
    Actor_SetSpriteColor((struct Actor *)effect, options->palette);
  if ((flags & EFFECT_USE_PRIORITY) != 0) {
    effect->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
    sprite->priority = options->priority;
  }
  if ((flags & EFFECT_USE_START_SCALE) != 0) {
    effect->scaleX = options->startScaleX;
    effect->scaleY = options->startScaleY;
  }
  if ((flags & EFFECT_SCALE_TO_TARGET) != 0) {
    script = gFieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK];
    if ((flags & EFFECT_USE_START_SCALE) != 0) {
      effect->scaleRateX = (options->targetScaleX - effect->scaleX) /
                           script[EFFECT_SCRIPT_REPEAT_COUNT];
      effect->scaleRateY = (options->targetScaleY - effect->scaleY) /
                           script[EFFECT_SCRIPT_REPEAT_COUNT];
    } else {
      effect->scaleRateX = (options->targetScaleX - FIXED_ONE) /
                           script[EFFECT_SCRIPT_REPEAT_COUNT];
      effect->scaleRateY = (options->targetScaleY - FIXED_ONE) /
                           script[EFFECT_SCRIPT_REPEAT_COUNT];
    }
  }
  if ((flags & EFFECT_USE_SCRIPT) != 0) {
    Actor_SelectAnimationFar((struct Actor *)effect, 1);
    Actor_StartScriptFar((void *)effect, options->script);
  }
  if ((flags & EFFECT_USE_ROTATION) != 0)
    sprite->rotation = options->rotation;
  if ((flags & EFFECT_USE_SPIN) != 0)
    effect->spin = options->spin;
  if ((flags & EFFECT_USE_UPDATE) != 0)
    effect->update = options->update;
}
