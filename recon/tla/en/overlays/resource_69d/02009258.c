#include "types.h"
#include "party_state.h"
#include "field_effect.h"

extern const s32 *const gData_0200270c[];

struct FieldActor *Object_GetById(s32 id);
struct FieldActor *Func_02001dc0(s32 type, s32 x, s32 y, s32 z);
void Func_02001da8(struct FieldActor *object, s32 mode);
void Func_02001db8(struct FieldActor *object, const s32 *script);
void ObjectDispatch_SetSingleChildFlags(struct FieldActor *object, s32 flags);
void Object_SetSpritePriority(struct FieldActor *object, s32 priority);
void Object_SetPartAttribute(struct FieldActor *object, s32 value);

static inline void Effect_SetScript(struct FieldActor *object,
                                    const s32 *script) {
  /* FAKEMATCH: as in ☀️'s spawner, the inline script boundary keeps the table
   * offset live until scale setup. */
  Func_02001db8(object, script);
}

void Func_02001258(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options) {
  struct FieldActor *party = Object_GetById(gPartyState.currentOwner);
  struct FieldEffect *effect;
  struct FieldSprite *sprite;
  const s32 *script;
  u32 tableOffset;

  if ((flags & EFFECT_USE_TYPE) != 0 && options != 0)
    effect = (struct FieldEffect *)Func_02001dc0(options->type, x, y, z);
  else
    effect = (struct FieldEffect *)Func_02001dc0(285, x, y, z);
  if (effect == 0)
    return;

  sprite = effect->sprite;
  Func_02001da8((struct FieldActor *)effect, (flags + 1) & EFFECT_SCRIPT_MASK);
  tableOffset = (flags & EFFECT_SCRIPT_MASK) << 2;
  Effect_SetScript((struct FieldActor *)effect,
                   gData_0200270c[tableOffset >> 2]);
  effect->motionFlags = 0;
  ObjectDispatch_SetSingleChildFlags((struct FieldActor *)effect, 0);
  effect->update = BattleFx_UpdateObjectMotionScaleAndLinkedAngle;
  effect->velocityX = velocityX;
  effect->velocityY = velocityY;
  effect->velocityZ = velocityZ;
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
    Object_SetSpritePriority((struct FieldActor *)effect, options->priority);
  }
  if ((flags & EFFECT_USE_START_SCALE) != 0) {
    effect->scaleX = options->startScaleX;
    effect->scaleY = options->startScaleY;
  }
  if ((flags & EFFECT_SCALE_TO_TARGET) != 0) {
    s32 target;
    s32 steps;

    script = gData_0200270c[tableOffset >> 2];
    target = options->targetScaleX;
    steps = script[3];
    if ((flags & EFFECT_USE_START_SCALE) != 0) {
      effect->scaleRateX = (target - effect->scaleX) / steps;
      effect->scaleRateY = (options->targetScaleY - effect->scaleY) / script[3];
    } else {
      effect->scaleRateX = (target - 0x10000) / steps;
      effect->scaleRateY = (options->targetScaleY - 0x10000) / script[3];
    }
  }
  if ((flags & EFFECT_USE_SCRIPT) != 0) {
    Func_02001da8((struct FieldActor *)effect, 1);
    Func_02001db8((struct FieldActor *)effect, options->script);
  }
  if ((flags & EFFECT_USE_ROTATION) != 0)
    sprite->rotation = options->rotation;
  if ((flags & EFFECT_USE_SPIN) != 0)
    effect->spin = options->spin;
  if ((flags & EFFECT_USE_UPDATE) != 0)
    effect->update = options->update;
}
