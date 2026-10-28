// Draft, differ 12 of 207: overlay 6aa's effect spawner, ☀️'s EFFECT_SPAWN
// (scene/shared/effect_spawn.h) with the leader from gSequenceParentWork.currentOwner,
// Actor_SetSingleChildFlags for the sprite flags, the leader's sprite
// priority through Actor_SetSpritePriorityB, and the scale-to-target target
// and frame count read before the start-scale test (the ☀️ form: 40). Left:
// three pairs where the native schedules a load before a register copy and this
// form the copy first: the gSequenceParentWork load before z's copy to sl in the
// prologue, the sprite load before flags' copy (sched2 priority 60 against 61),
// and the script table address before flags' copy. Tried: an asm operand on the
// sprite (32); alchemy permute 6 minutes (no gain).
#include "types.h"
#include "field_effect.h"
#include "sequence_parent_work.h"

extern const s32 *gData_0200686c[];

struct FieldActor *Actor_GetById(s32 id);
struct FieldEffect *Func_02004bd8(s32 type, s32 x, s32 y, s32 z);
void Func_02004bc0(struct FieldEffect *effect, s32 mode);
void Func_02004bd0(struct FieldEffect *effect, const s32 *script);
void Actor_SetSingleChildFlags(struct FieldEffect *effect, s32 flags);
void Actor_SetSpritePriorityB(struct FieldEffect *effect, s32 priority);
void Actor_SetPartAttribute(struct FieldEffect *effect, s32 value);

void Func_02003b30(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options) {
  struct FieldActor *party = Actor_GetById(gSequenceParentWork.currentOwner);
  struct FieldEffect *effect;
  struct FieldSprite *sprite;
  const s32 *script;

  if ((flags & EFFECT_USE_TYPE) != 0 && options != 0)
    effect = Func_02004bd8(options->type, x, y, z);
  else
    effect = Func_02004bd8(285, x, y, z);
  if (effect == 0)
    return;

  sprite = effect->sprite;
  Func_02004bc0(effect, (flags + 1) & EFFECT_SCRIPT_MASK);
  Func_02004bd0(effect, gData_0200686c[flags & EFFECT_SCRIPT_MASK]);
  effect->motionFlags = 0;
  Actor_SetSingleChildFlags(effect, 0);
  effect->update = BattleEffect_UpdateObjectMotionScaleAndLinkedAngle;
  effect->velocityX = velocityX;
  effect->velocityY = velocityY;
  effect->velocityZ = velocityZ;
  Actor_SetSpritePriorityB(effect, party->sprite->priority);
  effect->scaleRateX = 0;
  effect->scaleRateY = 0;
  effect->spin = 0;

  if ((flags & 0xffff0000) == 0 || options == 0)
    return;
  if ((flags & EFFECT_USE_PALETTE) != 0)
    Actor_SetPartAttribute(effect, options->palette);
  if ((flags & EFFECT_USE_PRIORITY) != 0) {
    effect->priorityFlags &= ~1;
    Actor_SetSpritePriorityB(effect, options->priority);
  }
  if ((flags & EFFECT_USE_START_SCALE) != 0) {
    effect->scaleX = options->startScaleX;
    effect->scaleY = options->startScaleY;
  }
  if ((flags & EFFECT_SCALE_TO_TARGET) != 0) {
    s32 target;
    s32 frames;

    script = gData_0200686c[flags & EFFECT_SCRIPT_MASK];
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
    Func_02004bc0(effect, 1);
    Func_02004bd0(effect, options->script);
  }
  if ((flags & EFFECT_USE_ROTATION) != 0)
    sprite->rotation = options->rotation;
  if ((flags & EFFECT_USE_SPIN) != 0)
    effect->spin = options->spin;
  if ((flags & EFFECT_USE_UPDATE) != 0)
    effect->update = options->update;
}
