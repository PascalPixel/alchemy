#include "types.h"
#include "field_effect.h"
#include "objdisp.h"
#include "scene_tables.h"
#include "fixed_math.h"

// Twenty-four maps, and the main image's Psynergy effects (whirlwind.c, with
// EFFECT_SPAWN_RESIDENT): FieldEffect_Spawn, which places a small effect moving
// at a velocity, its script one of gFieldEffect_DurationScripts and its options
// chosen by the EFFECT_USE_ flags.

/// Integrates linear motion, scale changes and rotation once per effect frame.
void FieldEffect_UpdateMotionScaleAndSpin(union ActorOrEffect *fieldObject) {
  fieldObject->effect.x += fieldObject->effect.velocity.x;
  fieldObject->effect.y += fieldObject->effect.velocity.y;
  fieldObject->effect.z += fieldObject->effect.velocity.z;
  fieldObject->effect.scaleX += fieldObject->effect.scaleRateX;
  fieldObject->effect.scaleY += fieldObject->effect.scaleRateY;
  fieldObject->effect.sprite->rotation += fieldObject->effect.spin;
}

// Compile the effect-spawning body once for the resident far-call version
// and once for each overlay's direct-call version.

extern const s32 *gFieldEffect_DurationScripts[];

enum {
  EFFECT_DEFAULT_TYPE = 222,
  EFFECT_OPTION_MASK = 0xffff0000,
  EFFECT_SCRIPT_REPEAT_COUNT = 3
};

/// Starts with linear motion and the leader's draw order; options can override
/// it.
static inline void FieldEffect_InitializeMotion(struct FieldEffect *effect,
                                                struct Sprite *sprite,
                                                struct Actor *leader,
                                                s32 velocityX, s32 velocityY,
                                                s32 velocityZ) {
  effect->velocity.x = velocityX;
  effect->velocity.y = velocityY;
  effect->velocity.z = velocityZ;
  // The sprite slot is read by its offset, the plain-C form of leader->sprite
  // this site matches with (scene/README.md): as the field, the pointer's
  // reload moves after the velocity.z store.
  sprite->image.priority =
      (*(struct Sprite **)((u8 *)leader + offsetof(struct Actor, sprite)))
          ->image.priority;
  effect->scaleRateX = 0;
  effect->scaleRateY = 0;
  effect->spin = 0;
}

#define EFFECT_SPAWN(name, create, set_mode, set_script)                       \
  void name(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY, s32 velocityZ,  \
            u32 flags, const struct EffectOptions *options) {                  \
    struct Actor *party = Actor_Get(ACTOR_PARTY_LEADER);                       \
    struct FieldEffect *effect;                                                \
    struct Sprite *sprite;                                                     \
    const s32 *script;                                                         \
                                                                               \
    if ((flags & EFFECT_USE_TYPE) != 0 && options != 0)                        \
      effect = (struct FieldEffect *)create(options->type, x, y, z);           \
    else                                                                       \
      effect = (struct FieldEffect *)create(EFFECT_DEFAULT_TYPE, x, y, z);     \
    if (effect == 0)                                                           \
      return;                                                                  \
                                                                               \
    sprite = effect->sprite;                                                   \
    set_mode(effect, (flags + 1) & EFFECT_SCRIPT_MASK);                        \
    set_script((void *)effect,                                                 \
               gFieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK]);      \
    effect->motionFlags = 0;                                                   \
    sprite->flags = 0;                                                         \
    effect->update = FieldEffect_UpdateMotionScaleAndSpin;                     \
    FieldEffect_InitializeMotion(effect, sprite, party, velocityX, velocityY,  \
                                 velocityZ);                                   \
                                                                               \
    if ((flags & EFFECT_OPTION_MASK) == 0 || options == 0)                     \
      return;                                                                  \
    if ((flags & EFFECT_USE_PALETTE) != 0)                                     \
      Actor_SetSpriteColor((struct Actor *)effect, options->palette);          \
    if ((flags & EFFECT_USE_PRIORITY) != 0) {                                  \
      effect->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;                      \
      sprite->image.priority = options->priority;                              \
    }                                                                          \
    if ((flags & EFFECT_USE_START_SCALE) != 0) {                               \
      effect->scaleX = options->startScaleX;                                   \
      effect->scaleY = options->startScaleY;                                   \
    }                                                                          \
    if ((flags & EFFECT_SCALE_TO_TARGET) != 0) {                               \
      script = gFieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK];       \
      if ((flags & EFFECT_USE_START_SCALE) != 0) {                             \
        effect->scaleRateX = (options->targetScaleX - effect->scaleX) /        \
                             script[EFFECT_SCRIPT_REPEAT_COUNT];               \
        effect->scaleRateY = (options->targetScaleY - effect->scaleY) /        \
                             script[EFFECT_SCRIPT_REPEAT_COUNT];               \
      } else {                                                                 \
        effect->scaleRateX = (options->targetScaleX - Q16_ONE) /               \
                             script[EFFECT_SCRIPT_REPEAT_COUNT];               \
        effect->scaleRateY = (options->targetScaleY - Q16_ONE) /               \
                             script[EFFECT_SCRIPT_REPEAT_COUNT];               \
      }                                                                        \
    }                                                                          \
    if ((flags & EFFECT_USE_SCRIPT) != 0) {                                    \
      set_mode(effect, 1);                                                     \
      set_script((void *)effect, options->script);                             \
    }                                                                          \
    if ((flags & EFFECT_USE_ROTATION) != 0)                                    \
      sprite->rotation = options->rotation;                                    \
    if ((flags & EFFECT_USE_SPIN) != 0)                                        \
      effect->spin = options->spin;                                            \
    if ((flags & EFFECT_USE_UPDATE) != 0)                                      \
      effect->update = options->update;                                        \
  }

#ifdef EFFECT_SPAWN_RESIDENT
/// The resident version uses far calls to the engine.
EFFECT_SPAWN(FieldEffect_SpawnResident, Actor_CreateFar,
             Actor_SelectAnimationFar, Actor_StartScriptFar)
#else
/// Each overlay includes its own copy of this version.
EFFECT_SPAWN(FieldEffect_Spawn, FieldActor_Create, Actor_SelectAnimation,
             Actor_StartScript)
#endif
