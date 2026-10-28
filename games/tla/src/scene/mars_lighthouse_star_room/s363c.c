#include "types.h"
#include "sequence_parent_work.h"
#include "field_sprite.h"
#include "field_effect.h"

extern struct FieldEffect *Actor_GetById(s32 id);
extern void Func_020041cc(struct FieldEffect *effect, s32 mode);
extern void Func_020041dc(struct FieldEffect *effect, const s32 *script);
extern const s32 *gData_02004ce0[];
extern void Actor_SetSingleChildFlags(struct FieldEffect *effect,
                                               u32 flags);
extern void Scene_SetActorSpritePriority(struct FieldEffect *effect, s32 priority);
extern void Actor_SetPartAttribute(struct FieldEffect *effect, s32 attribute);
extern void
BattleEffect_UpdateObjectMotionScaleAndLinkedAngle(union FieldObject *object);
void Func_0200363c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options) {
  struct FieldEffect *party = Actor_GetById(gSequenceParentWork.currentOwner);
  struct FieldEffect *effect;
  struct FieldSprite *sprite;
  const s32 *script;
  u32 start;

  if ((flags & EFFECT_USE_TYPE) != 0 && options != 0)
    effect = (struct FieldEffect *)Func_020041e4(options->type, x, y, z);
  else
    effect = (struct FieldEffect *)Func_020041e4(285, x, y, z);
  if (effect == 0)
    return;

  sprite = effect->sprite;
  Func_020041cc(effect, (flags + 1) & EFFECT_SCRIPT_MASK);
  Func_020041dc(effect, gData_02004ce0[flags & EFFECT_SCRIPT_MASK]);
  effect->motionFlags = 0;
  Actor_SetSingleChildFlags(effect, 0);
  effect->update = BattleEffect_UpdateObjectMotionScaleAndLinkedAngle;
  effect->velocity.x = velocityX;
  effect->velocity.y = velocityY;
  effect->velocity.z = velocityZ;
  Scene_SetActorSpritePriority(effect, party->sprite->priority);
  effect->scaleRateX = 0;
  effect->scaleRateY = 0;
  effect->spin = 0;

  if ((flags & 0xffff0000) == 0 || options == 0)
    return;
  if ((flags & EFFECT_USE_PALETTE) != 0)
    Actor_SetPartAttribute(effect, options->palette);
  if ((flags & EFFECT_USE_PRIORITY) != 0) {
    effect->priorityFlags &= ~1;
    Scene_SetActorSpritePriority(effect, options->priority);
  }
  start = flags & EFFECT_USE_START_SCALE;
  if (start != 0) {
    effect->scaleX = options->startScaleX;
    effect->scaleY = options->startScaleY;
  }
  if ((flags & EFFECT_SCALE_TO_TARGET) != 0) {
    s32 targetX;
    s32 count;

    script = gData_02004ce0[flags & EFFECT_SCRIPT_MASK];
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
    Func_020041cc(effect, 1);
    Func_020041dc(effect, options->script);
  }
  if ((flags & EFFECT_USE_ROTATION) != 0)
    sprite->rotation = options->rotation;
  if ((flags & EFFECT_USE_SPIN) != 0)
    effect->spin = options->spin;
  if ((flags & EFFECT_USE_UPDATE) != 0)
    effect->update = options->update;
}
