// Draft, differ 12 of 207: overlay 69f's copy of the function drafted in
// resource_6aa_c_02003b30.c (overlay 6aa's Func_02003b30, differ 12 there), whose
// header says what it is, what still differs and what was tried.
#include "types.h"
#include "field_effect.h"
#include "party_state.h"

extern const s32 *gData_020023d8[];

struct FieldActor *Object_GetById(s32 id);
struct FieldEffect *Func_02001acc(s32 type, s32 x, s32 y, s32 z);
void Func_02001ab4(struct FieldEffect *effect, s32 mode);
void Func_02001ac4(struct FieldEffect *effect, const s32 *script);
void ObjectDispatch_SetSingleChildFlags(struct FieldEffect *effect, s32 flags);
void Object_SetSpritePriority(struct FieldEffect *effect, s32 priority);
void Object_SetPartAttribute(struct FieldEffect *effect, s32 value);

void Func_02000f5c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options) {
  struct FieldActor *party = Object_GetById(gPartyState.currentOwner);
  struct FieldEffect *effect;
  struct FieldSprite *sprite;
  const s32 *script;

  if ((flags & EFFECT_USE_TYPE) != 0 && options != 0)
    effect = Func_02001acc(options->type, x, y, z);
  else
    effect = Func_02001acc(285, x, y, z);
  if (effect == 0)
    return;

  sprite = effect->sprite;
  Func_02001ab4(effect, (flags + 1) & EFFECT_SCRIPT_MASK);
  Func_02001ac4(effect, gData_020023d8[flags & EFFECT_SCRIPT_MASK]);
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
  if ((flags & EFFECT_USE_START_SCALE) != 0) {
    effect->scaleX = options->startScaleX;
    effect->scaleY = options->startScaleY;
  }
  if ((flags & EFFECT_SCALE_TO_TARGET) != 0) {
    s32 target;
    s32 frames;

    script = gData_020023d8[flags & EFFECT_SCRIPT_MASK];
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
    Func_02001ab4(effect, 1);
    Func_02001ac4(effect, options->script);
  }
  if ((flags & EFFECT_USE_ROTATION) != 0)
    sprite->rotation = options->rotation;
  if ((flags & EFFECT_USE_SPIN) != 0)
    effect->spin = options->spin;
  if ((flags & EFFECT_USE_UPDATE) != 0)
    effect->update = options->update;
}
