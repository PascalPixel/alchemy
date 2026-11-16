/// @file
/// The effect spawner each field overlay keeps its own copy of, the twin of The
/// Broken Seal's EFFECT_SPAWN (scene/shared/field_effect_spawn.h) with default
/// type 285. The including file names its overlay's copy and the imports it
/// calls: EFFECT_SPAWN, EFFECT_CREATE, EFFECT_SET_MODE, EFFECT_SET_SCRIPT and
/// EFFECT_SCRIPTS. Copies that also define EFFECT_SPRITE_PRIORITY create the
/// actor as an event does (FieldActor_Create) and give the sprite the requested
/// priority themselves before Scene_SetActorSpritePriority, as overlays 663,
/// 664, 66b, 679 and 67b do.
#include "types.h"
#include "system/main_routine_work.h"
#include "field/field_sprite.h"
#include "system/field_effect.h"
#include "fixed_math.h"

extern struct Actor *EFFECT_CREATE(s32 type, s32 x, s32 y, s32 z);
extern const s32 *EFFECT_SCRIPTS[];
struct Actor *Actor_GetById(u32 objectId);
void Actor_SetSpriteFlags(struct Actor *actor, u32 value);

void EFFECT_SPAWN(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                  s32 velocityZ, u32 flags,
                  const struct EffectOptions *options) {
  struct Actor *party = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct FieldEffect *effect;
  struct FieldSprite *sprite;
  const s32 *script;
  u32 start;

#ifdef EFFECT_SPRITE_PRIORITY
  if ((flags & EFFECT_USE_TYPE) != 0 && options != 0)
    effect = (struct FieldEffect *)FieldActor_Create(options->type, x, y, z);
  else
    effect = (struct FieldEffect *)FieldActor_Create(285, x, y, z);
#else
  if ((flags & EFFECT_USE_TYPE) != 0 && options != 0)
    effect = (struct FieldEffect *)EFFECT_CREATE(options->type, x, y, z);
  else
    effect = (struct FieldEffect *)EFFECT_CREATE(285, x, y, z);
#endif
  if (effect == 0)
    return;

  sprite = effect->sprite;
  EFFECT_SET_MODE((void *)effect, (flags + 1) & EFFECT_SCRIPT_MASK);
  EFFECT_SET_SCRIPT((void *)effect, EFFECT_SCRIPTS[flags & EFFECT_SCRIPT_MASK]);
  effect->motionFlags = 0;
  Actor_SetSpriteFlags(effect, 0);
  effect->update = FieldEffect_UpdateMotionScaleAndSpin;
  effect->velocity.x = velocityX;
  effect->velocity.y = velocityY;
  effect->velocity.z = velocityZ;
  Scene_SetActorSpritePriority(effect,
                               ((struct FieldSprite *)party->sprite)->priority);
  effect->scaleRateX = 0;
  effect->scaleRateY = 0;
  effect->spin = 0;

  if ((flags & 0xffff0000) == 0 || options == 0)
    return;
  if ((flags & EFFECT_USE_PALETTE) != 0)
    Actor_SetSpriteColor(effect, options->palette);
  if ((flags & EFFECT_USE_PRIORITY) != 0) {
#ifdef EFFECT_SPRITE_PRIORITY
    effect->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
    // Read as its low byte: (u8)options->priority orders the loads apart.
    sprite->priority = *(u8 *)&options->priority;
#else
    effect->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
#endif
    Scene_SetActorSpritePriority(effect, options->priority);
  }
  start = flags & EFFECT_USE_START_SCALE;
  if (start != 0) {
    effect->scaleX = options->startScaleX;
    effect->scaleY = options->startScaleY;
  }
  if ((flags & EFFECT_SCALE_TO_TARGET) != 0) {
    FIXED targetX;
    s32 count;

    script = EFFECT_SCRIPTS[flags & EFFECT_SCRIPT_MASK];
    targetX = options->targetScaleX;
    count = script[3];
    if (start != 0) {
      effect->scaleRateX = (targetX - effect->scaleX) / count;
      effect->scaleRateY = (options->targetScaleY - effect->scaleY) / script[3];
    } else {
      effect->scaleRateX = (targetX - FIXED_ONE) / count;
      effect->scaleRateY = (options->targetScaleY - FIXED_ONE) / script[3];
    }
  }
  if ((flags & EFFECT_USE_SCRIPT) != 0) {
    EFFECT_SET_MODE((void *)effect, 1);
    EFFECT_SET_SCRIPT((void *)effect, options->script);
  }
  if ((flags & EFFECT_USE_ROTATION) != 0)
    sprite->rotation = options->rotation;
  if ((flags & EFFECT_USE_SPIN) != 0)
    effect->spin = options->spin;
  if ((flags & EFFECT_USE_UPDATE) != 0)
    effect->update = options->update;
}
