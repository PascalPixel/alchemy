// Under -mthumb-slow-loads (2026-10-26): differ 6 of 218 in tla-en, instruction
// order only. No empty asm before any one statement lowers it.
// Near miss, differ 8 of 218 (2026-10-15): ☀️'s EFFECT_SPAWN
// (Effect_SpawnResident) with ⚓️'s calls (Object_GetById(0), Func_080200c0,
// default type 285, the gData_080f3978 scripts, Object_SetPartAttribute). Left:
// the listing stores scaleRateX and _y (0x30, 0x34) after the priority byte
// copy; here the scheduler lifts both stores above the party sprite's byte
// load, and a barrier after the copy swaps r9 and r11. One reordered pool load
// before it (gData_080f3978).
#include "types.h"
#include "field_effect.h"
#include "objdisp.h"

extern const s32 *gData_080f3978[];

struct FieldActor *Object_GetById(s32 id);
struct FieldEffect *Func_080200c0(s32 type, s32 x, s32 y, s32 z);
void Object_SetPartAttribute(struct FieldActor *object, s32 value);
void BattleEffect_UpdateObjectMotionScaleAndLinkedAngle(union FieldObject *object);

void Func_080df8e0(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options) {
  struct FieldActor *party = Object_GetById(0);
  struct FieldEffect *effect;
  struct FieldSprite *sprite;
  const s32 *script;

  if ((flags & EFFECT_USE_TYPE) != 0 && options != 0)
    effect = Func_080200c0(options->type, x, y, z);
  else
    effect = Func_080200c0(285, x, y, z);
  if (effect == 0)
    return;

  sprite = effect->sprite;
  Object_SetMode((struct FieldActor *)effect, (flags + 1) & EFFECT_SCRIPT_MASK);
  ObjectDispatch_InitializeFar((void *)effect,
                               gData_080f3978[flags & EFFECT_SCRIPT_MASK]);
  effect->motionFlags = 0;
  sprite->nextFrame = 0;
  effect->update = BattleEffect_UpdateObjectMotionScaleAndLinkedAngle;
  effect->velocity.x = velocityX;
  effect->velocity.y = velocityY;
  effect->velocity.z = velocityZ;
  sprite->priority = party->sprite->priority;
  effect->scaleRateX = 0;
  effect->scaleRateY = 0;
  effect->spin = 0;

  if ((flags & 0xffff0000) == 0 || options == 0)
    return;
  if ((flags & EFFECT_USE_PALETTE) != 0)
    Object_SetPartAttribute((struct FieldActor *)effect, options->palette);
  if ((flags & EFFECT_USE_PRIORITY) != 0) {
    effect->priorityFlags &= ~1;
    sprite->priority = options->priority;
  }
  if ((flags & EFFECT_USE_START_SCALE) != 0) {
    effect->scaleX = options->startScaleX;
    effect->scaleY = options->startScaleY;
  }
  if ((flags & EFFECT_SCALE_TO_TARGET) != 0) {
    script = gData_080f3978[flags & EFFECT_SCRIPT_MASK];
    if ((flags & EFFECT_USE_START_SCALE) != 0) {
      effect->scaleRateX = (options->targetScaleX - effect->scaleX) / script[3];
      effect->scaleRateY = (options->targetScaleY - effect->scaleY) / script[3];
    } else {
      effect->scaleRateX = (options->targetScaleX - 0x10000) / script[3];
      effect->scaleRateY = (options->targetScaleY - 0x10000) / script[3];
    }
  }
  if ((flags & EFFECT_USE_SCRIPT) != 0) {
    Object_SetMode((struct FieldActor *)effect, 1);
    ObjectDispatch_InitializeFar((void *)effect, options->script);
  }
  if ((flags & EFFECT_USE_ROTATION) != 0)
    sprite->rotation = options->rotation;
  if ((flags & EFFECT_USE_SPIN) != 0)
    effect->spin = options->spin;
  if ((flags & EFFECT_USE_UPDATE) != 0)
    effect->update = options->update;
}
