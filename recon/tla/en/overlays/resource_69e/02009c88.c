#include "types.h"
#include "field_effect.h"
#include "field_sprite.h"
#include "sequence_parent_work.h"

extern const s32 *gData_020033f4[];

struct FieldActor *Actor_GetById(s32 id);
struct FieldEffect *Func_02002828(s32 type, s32 x, s32 y, s32 z);
void Func_02002810(struct FieldEffect *effect, s32 mode);
void Func_02002820(struct FieldEffect *effect, const s32 *script);
void Actor_SetSingleChildFlags(struct FieldEffect *effect, s32 flags);
void Scene_SetActorSpritePriority(struct FieldEffect *effect, s32 priority);
void Actor_SetPartAttribute(struct FieldEffect *effect, s32 value);

enum {
  EFFECT_DEFAULT_TYPE = 285,
  EFFECT_SCALE_ONE = 0x10000,
  EFFECT_OPTION_MASK = 0xffff0000,
  EFFECT_SCRIPT_REPEAT_COUNT = 3
};

void Func_02001c88(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options) {
  struct FieldActor *party = Actor_GetById(gSequenceParentWork.currentOwner);
  struct FieldEffect *effect;
  struct FieldSprite *sprite;
  const s32 *script;
  s32 target;
  s32 count;

  if ((flags & EFFECT_USE_TYPE) != 0 && options != 0)
    effect = Func_02002828(options->type, x, y, z);
  else
    effect = Func_02002828(EFFECT_DEFAULT_TYPE, x, y, z);
  if (effect == 0)
    return;

  sprite = effect->sprite;
  Func_02002810(effect, (flags + 1) & EFFECT_SCRIPT_MASK);
  Func_02002820(effect, gData_020033f4[flags & EFFECT_SCRIPT_MASK]);
  effect->motionFlags = 0;
  Actor_SetSingleChildFlags(effect, 0);
  effect->update = BattleEffect_UpdateObjectMotionScaleAndLinkedAngle;
  effect->velocityX = velocityX;
  effect->velocityY = velocityY;
  effect->velocityZ = velocityZ;
  Scene_SetActorSpritePriority(effect, party->sprite->priority);
  effect->scaleRateX = 0;
  effect->scaleRateY = 0;
  effect->spin = 0;

  if ((flags & EFFECT_OPTION_MASK) == 0 || options == 0)
    return;
  if ((flags & EFFECT_USE_PALETTE) != 0)
    Actor_SetPartAttribute(effect, options->palette);
  if ((flags & EFFECT_USE_PRIORITY) != 0) {
    effect->priorityFlags &= ~1;
    Scene_SetActorSpritePriority(effect, options->priority);
  }
  if ((flags & EFFECT_USE_START_SCALE) != 0) {
    effect->scaleX = options->startScaleX;
    effect->scaleY = options->startScaleY;
  }
  if ((flags & EFFECT_SCALE_TO_TARGET) != 0) {
    script = gData_020033f4[flags & EFFECT_SCRIPT_MASK];
    target = options->targetScaleX;
    count = script[EFFECT_SCRIPT_REPEAT_COUNT];
    if ((flags & EFFECT_USE_START_SCALE) != 0) {
      effect->scaleRateX = (target - effect->scaleX) / count;
      effect->scaleRateY = (options->targetScaleY - effect->scaleY) /
                           script[EFFECT_SCRIPT_REPEAT_COUNT];
    } else {
      effect->scaleRateX = (target - EFFECT_SCALE_ONE) / count;
      effect->scaleRateY = (options->targetScaleY - EFFECT_SCALE_ONE) /
                           script[EFFECT_SCRIPT_REPEAT_COUNT];
    }
  }
  if ((flags & EFFECT_USE_SCRIPT) != 0) {
    Func_02002810(effect, 1);
    Func_02002820(effect, options->script);
  }
  if ((flags & EFFECT_USE_ROTATION) != 0)
    sprite->rotation = options->rotation;
  if ((flags & EFFECT_USE_SPIN) != 0)
    effect->spin = options->spin;
  if ((flags & EFFECT_USE_UPDATE) != 0)
    effect->update = options->update;
}
