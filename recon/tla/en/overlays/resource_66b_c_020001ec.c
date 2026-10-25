// Draft, differ 30 of 219: overlay 66b's copy of Effect_Spawn, the same listing
// as overlay 663's Func_0200015c, whose draft holds its notes.
#include "types.h"
#include "party_state.h"
#include "field_effect.h"

extern const s32 *gData_020019b4[];

struct FieldActor *Object_GetById(s32 id);
struct FieldActor *Engine_ObjectCreate(s32 type, s32 x, s32 y, s32 z);
void Func_020011b4(struct FieldActor *object, s32 mode);
void Func_020011bc(void *object, const s32 *script);
void ObjectDispatch_SetSingleChildFlags(struct FieldActor *object, s32 flags);
void Object_SetSpritePriority(struct FieldActor *object, s32 priority);
void Object_SetPartAttribute(struct FieldActor *object, s32 value);

void Func_020001ec(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags_,
                   const struct EffectOptions *options) {
  register u32 flags __asm__("r10") = flags_;
  struct FieldActor *party = Object_GetById(gPartyState.currentOwner);
  struct FieldEffect *effect;
  struct FieldSprite *sprite;
  const s32 *script;

  if ((flags & EFFECT_USE_TYPE) != 0 && options != 0)
    effect = (struct FieldEffect *)Engine_ObjectCreate(options->type, x, y, z);
  else
    effect = (struct FieldEffect *)Engine_ObjectCreate(285, x, y, z);
  if (effect == 0)
    return;

  sprite = effect->sprite;
  Func_020011b4((struct FieldActor *)effect, (flags + 1) & EFFECT_SCRIPT_MASK);
  Func_020011bc(effect, gData_020019b4[flags & EFFECT_SCRIPT_MASK]);
  effect->motionFlags = 0;
  ObjectDispatch_SetSingleChildFlags((struct FieldActor *)effect, 0);
  effect->update = BattleEffect_UpdateObjectMotionScaleAndLinkedAngle;
  effect->velocity.x = velocityX;
  effect->velocity.y = velocityY;
  effect->velocity.z = velocityZ;
  Object_SetSpritePriority((struct FieldActor *)effect,
                           party->sprite->priority);
  effect->scaleRateX = 0;
  effect->scaleRateY = 0;
  effect->spin = 0;

  if ((flags & 0xffff0000) == 0 || options == 0)
    return;
  if ((flags & EFFECT_USE_PALETTE) != 0)
    Object_SetPartAttribute((struct FieldActor *)effect, options->palette);
  if ((flags & EFFECT_USE_PRIORITY) != 0) {
    effect->priorityFlags &= ~1;
    sprite->priority = (u8)options->priority;
    Object_SetSpritePriority((struct FieldActor *)effect, options->priority);
  }
  if ((flags & EFFECT_USE_START_SCALE) != 0) {
    effect->scaleX = options->startScaleX;
    effect->scaleY = options->startScaleY;
  }
  if ((flags & EFFECT_SCALE_TO_TARGET) != 0) {
    s32 targetX;
    s32 count;

    script = gData_020019b4[flags & EFFECT_SCRIPT_MASK];
    targetX = options->targetScaleX;
    count = script[3];
    if ((flags & EFFECT_USE_START_SCALE) != 0) {
      effect->scaleRateX = (targetX - effect->scaleX) / count;
      effect->scaleRateY = (options->targetScaleY - effect->scaleY) / script[3];
    } else {
      effect->scaleRateX = (targetX - 0x10000) / count;
      effect->scaleRateY = (options->targetScaleY - 0x10000) / script[3];
    }
  }
  if ((flags & EFFECT_USE_SCRIPT) != 0) {
    Func_020011b4((struct FieldActor *)effect, 1);
    Func_020011bc(effect, options->script);
  }
  if ((flags & EFFECT_USE_ROTATION) != 0)
    sprite->rotation = options->rotation;
  if ((flags & EFFECT_USE_SPIN) != 0)
    effect->spin = options->spin;
  if ((flags & EFFECT_USE_UPDATE) != 0)
    effect->update = options->update;
}
