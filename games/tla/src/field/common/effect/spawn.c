/// @file
/// The effect spawner each field overlay keeps its own copy of, the twin of The
/// Broken Seal's EFFECT_SPAWN (map/common/effect_spawn.h) with default type
/// 285. The including file names its overlay's copy and the imports it calls:
/// EFFECT_SPAWN, EFFECT_CREATE, EFFECT_SET_MODE, EFFECT_SET_SCRIPT and
/// EFFECT_SCRIPTS.
#include "types.h"
#include "party_state.h"
#include "field_sprite.h"
#include "field_effect.h"

extern struct FieldEffect *Object_GetById(s32 id);
extern struct FieldEffect *EFFECT_CREATE(s32 type, s32 x, s32 y, s32 z);
extern void EFFECT_SET_MODE(struct FieldEffect *effect, s32 mode);
extern void EFFECT_SET_SCRIPT(struct FieldEffect *effect, const s32 *script);
extern const s32 *EFFECT_SCRIPTS[];
extern void ObjectDispatch_SetSingleChildFlags(struct FieldEffect *effect,
                                               u32 flags);
extern void Object_SetSpritePriority(struct FieldEffect *effect, s32 priority);
extern void Object_SetPartAttribute(struct FieldEffect *effect, s32 attribute);

void EFFECT_SPAWN(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                  s32 velocityZ, u32 flags,
                  const struct EffectOptions *options) {
  struct FieldEffect *party = Object_GetById(gPartyState.currentOwner);
  struct FieldEffect *effect;
  struct FieldSprite *sprite;
  const s32 *script;
  u32 start;

  if ((flags & EFFECT_USE_TYPE) != 0 && options != 0)
    effect = EFFECT_CREATE(options->type, x, y, z);
  else
    effect = EFFECT_CREATE(285, x, y, z);
  if (effect == 0)
    return;

  sprite = effect->sprite;
  EFFECT_SET_MODE(effect, (flags + 1) & EFFECT_SCRIPT_MASK);
  EFFECT_SET_SCRIPT(effect, EFFECT_SCRIPTS[flags & EFFECT_SCRIPT_MASK]);
  effect->motionFlags = 0;
  ObjectDispatch_SetSingleChildFlags(effect, 0);
  effect->update = BattleEffect_UpdateObjectMotionScaleAndLinkedAngle;
  effect->velocity.x = velocityX;
  effect->velocity.y = velocityY;
  effect->velocity.z = velocityZ;
  Object_SetSpritePriority(effect, party->sprite->priority);
  effect->scaleRateX = 0;
  effect->scaleRateY = 0;
  effect->spin = 0;

  if ((flags & 0xffff0000) == 0 || options == 0)
    return;
  if ((flags & EFFECT_USE_PALETTE) != 0)
    Object_SetPartAttribute(effect, options->palette);
  if ((flags & EFFECT_USE_PRIORITY) != 0) {
    effect->priorityFlags &= ~1;
    Object_SetSpritePriority(effect, options->priority);
  }
  start = flags & EFFECT_USE_START_SCALE;
  if (start != 0) {
    effect->scaleX = options->startScaleX;
    effect->scaleY = options->startScaleY;
  }
  if ((flags & EFFECT_SCALE_TO_TARGET) != 0) {
    s32 targetX;
    s32 count;

    script = EFFECT_SCRIPTS[flags & EFFECT_SCRIPT_MASK];
    targetX = options->targetScaleX;
    count = script[3];
    if (start != 0) {
      effect->scaleRateX = (targetX - effect->scaleX) / count;
      effect->scaleRateY = (options->targetScaleY - effect->scaleY) / script[3];
    } else {
      effect->scaleRateX = (targetX - 0x10000) / count;
      effect->scaleRateY = (options->targetScaleY - 0x10000) / script[3];
    }
  }
  if ((flags & EFFECT_USE_SCRIPT) != 0) {
    EFFECT_SET_MODE(effect, 1);
    EFFECT_SET_SCRIPT(effect, options->script);
  }
  if ((flags & EFFECT_USE_ROTATION) != 0)
    sprite->rotation = options->rotation;
  if ((flags & EFFECT_USE_SPIN) != 0)
    effect->spin = options->spin;
  if ((flags & EFFECT_USE_UPDATE) != 0)
    effect->update = options->update;
}
