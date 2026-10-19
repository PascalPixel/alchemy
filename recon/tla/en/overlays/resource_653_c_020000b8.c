// Draft, differ 12 of 207: ⚓️'s Effect_Spawn, the twin of ☀️'s EFFECT_SPAWN
// (scene/common/effect_spawn.h) with default type 285 and calls where ☀️ stores
// (ObjectDispatch_SetSingleChildFlags, Object_SetSpritePriority,
// Object_SetPartAttribute). The same function is in 26 overlays (653 654 655
// 65f 661 67f 68f 690 694 698 69a-6a5 6a8 6ab 6ad 6b4), each calling its own
// import stubs and script table. Left: instruction order only; the listing
// loads gPartyState's address before moving z into r10, and loads the sprite
// before copying flags for the mode. Loading the target scale and step count
// before the branch took it from 40 to 12; five minutes of alchemy permute
// found nothing better (27). The velocity stores follow field_effect.h's
// struct Vector3 (the draft no longer compiled). Tried 2026-10-19: velocityX
// pinned to r11 and z to r10 behind an empty asm barrier, as Func_02000cbc of
// overlay 64c matches (18: the other parameter copies all move ahead of the
// pool load).
#include "types.h"
#include "party_state.h"
#include "field_sprite.h"
#include "field_effect.h"

extern struct FieldEffect *Object_GetById(s32 id);
extern struct FieldEffect *Func_02005140(s32 type, s32 x, s32 y, s32 z);
extern void Func_02005130(struct FieldEffect *effect, s32 mode);
extern void Func_02005138(struct FieldEffect *effect, const s32 *script);
extern const s32 *gData_02005508[];
extern void ObjectDispatch_SetSingleChildFlags(struct FieldEffect *effect,
                                               u32 flags);
extern void Object_SetSpritePriority(struct FieldEffect *effect, s32 priority);
extern void Object_SetPartAttribute(struct FieldEffect *effect, s32 attribute);

void Func_020000b8(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options) {
  struct FieldEffect *party = Object_GetById(gPartyState.currentOwner);
  struct FieldEffect *effect;
  struct FieldSprite *sprite;
  const s32 *script;
  u32 start;

  if ((flags & EFFECT_USE_TYPE) != 0 && options != 0)
    effect = Func_02005140(options->type, x, y, z);
  else
    effect = Func_02005140(285, x, y, z);
  if (effect == 0)
    return;

  sprite = effect->sprite;
  Func_02005130(effect, (flags + 1) & EFFECT_SCRIPT_MASK);
  Func_02005138(effect, gData_02005508[flags & EFFECT_SCRIPT_MASK]);
  effect->motionFlags = 0;
  ObjectDispatch_SetSingleChildFlags(effect, 0);
  effect->update = BattleFx_UpdateObjectMotionScaleAndLinkedAngle;
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

    script = gData_02005508[flags & EFFECT_SCRIPT_MASK];
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
    Func_02005130(effect, 1);
    Func_02005138(effect, options->script);
  }
  if ((flags & EFFECT_USE_ROTATION) != 0)
    sprite->rotation = options->rotation;
  if ((flags & EFFECT_USE_SPIN) != 0)
    effect->spin = options->spin;
  if ((flags & EFFECT_USE_UPDATE) != 0)
    effect->update = options->update;
}
