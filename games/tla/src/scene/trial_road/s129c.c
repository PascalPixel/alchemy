#include "types.h"
#include "field_effect.h"
#include "sequence_parent_work.h"

extern const s32 *gData_02007bc0[];

struct FieldActor *Actor_GetById(s32 id);
struct FieldEffect *Func_020057d8(s32 type, s32 x, s32 y, s32 z);
void Func_020057c8(struct FieldEffect *effect, s32 mode);
void Func_020057d0(struct FieldEffect *effect, const s32 *script);
void Actor_SetSingleChildFlags(struct FieldEffect *effect, s32 flags);
void Scene_SetActorSpritePriority(struct FieldEffect *effect, s32 priority);
void Actor_SetPartAttribute(struct FieldEffect *effect, s32 value);

void Func_0200129c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options) {
  struct FieldActor *party = Actor_GetById(gSequenceParentWork.currentOwner);
  struct FieldEffect *effect;
  struct FieldSprite *sprite;
  const s32 *script;

  if ((flags & EFFECT_USE_TYPE) != 0 && options != 0)
    effect = Func_020057d8(options->type, x, y, z);
  else
    effect = Func_020057d8(285, x, y, z);
  if (effect == 0)
    return;

  sprite = effect->sprite;
  Func_020057c8(effect, (flags + 1) & EFFECT_SCRIPT_MASK);
  Func_020057d0(effect, gData_02007bc0[flags & EFFECT_SCRIPT_MASK]);
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
  if ((flags & EFFECT_USE_START_SCALE) != 0) {
    effect->scaleX = options->startScaleX;
    effect->scaleY = options->startScaleY;
  }
  if ((flags & EFFECT_SCALE_TO_TARGET) != 0) {
    s32 target;
    s32 frames;

    script = gData_02007bc0[flags & EFFECT_SCRIPT_MASK];
    target = options->targetScaleX;
    frames = script[3];
    if ((flags & EFFECT_USE_START_SCALE) != 0) {
      effect->scaleRateX = (target - effect->scaleX) / frames;
      effect->scaleRateY = (options->targetScaleY - effect->scaleY) / script[3];
    } else {
      effect->scaleRateX = (target - 0x10000) / frames;
      effect->scaleRateY = (options->targetScaleY - 0x10000) / script[3];
    }
  }
  if ((flags & EFFECT_USE_SCRIPT) != 0) {
    Func_020057c8(effect, 1);
    Func_020057d0(effect, options->script);
  }
  if ((flags & EFFECT_USE_ROTATION) != 0)
    sprite->rotation = options->rotation;
  if ((flags & EFFECT_USE_SPIN) != 0)
    effect->spin = options->spin;
  if ((flags & EFFECT_USE_UPDATE) != 0)
    effect->update = options->update;
}
