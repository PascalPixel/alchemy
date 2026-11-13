/// @file
/// FieldEffect_SpawnResident, an object of its own between the event commands.

#include "types.h"
#include "field_effect.h"
#include "objdisp.h"
#include "gamepak/far_call/field.h"

/// The effect's sprite as the projection draft lays it out: FieldSprite with
/// bytes 0x0c to 0x0f its 16.16 scale.
struct LayeredSprite {
  u8 unk00[8];
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unk0a[2];
  s32 scale;
  u8 unk10[2];
  u16 rotation;
  u8 unk14[6];
  u8 nextFrame;
};

extern const s32 *gFieldEffect_DurationScripts[];

struct Actor *Actor_GetById(u32 objectId);
void Actor_SetSpriteColor(struct Actor *object, s32 attribute);

void FieldEffect_SpawnResident(s32 x, s32 y, s32 z, s32 velocityX,
                               s32 velocityY, s32 velocityZ, u32 flags,
                               const struct EffectOptions *options) {
  struct Actor *party = Actor_GetById(0);
  struct FieldEffect *effect;
  struct LayeredSprite *sprite;
  const s32 *script;

  if ((flags & EFFECT_USE_TYPE) != 0 && options != 0)
    effect = Actor_CreateFar(options->type, x, y, z);
  else
    effect = Actor_CreateFar(285, x, y, z);
  if (effect == 0)
    return;

  sprite = (struct LayeredSprite *)effect->sprite;
  Actor_SelectAnimationFar((struct Actor *)effect, (flags + 1) & EFFECT_SCRIPT_MASK);
  Actor_StartScriptFar(
      (void *)effect, gFieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK]);
  effect->motionFlags = 0;
  sprite->nextFrame = 0;
  effect->update = FieldEffect_UpdateMotionScaleAndSpin;
  effect->velocity.x = velocityX;
  effect->velocity.y = velocityY;
  effect->velocity.z = velocityZ;
  sprite->priority = ((struct FieldSprite *)party->sprite)->priority;
  effect->scaleRateX = 0;
  effect->scaleRateY = 0;
  effect->spin = 0;

  if ((flags & 0xffff0000) == 0 || options == 0)
    return;
  if ((flags & EFFECT_USE_PALETTE) != 0)
    Actor_SetSpriteColor((struct Actor *)effect, options->palette);
  if ((flags & EFFECT_USE_PRIORITY) != 0) {
    effect->priorityFlags &= ~1;
    sprite->priority = options->priority;
  }
  if ((flags & EFFECT_USE_START_SCALE) != 0) {
    effect->scaleX = options->startScaleX;
    effect->scaleY = options->startScaleY;
  }
  if ((flags & EFFECT_SCALE_TO_TARGET) != 0) {
    script = gFieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK];
    if ((flags & EFFECT_USE_START_SCALE) != 0) {
      effect->scaleRateX = (options->targetScaleX - effect->scaleX) / script[3];
      effect->scaleRateY = (options->targetScaleY - effect->scaleY) / script[3];
    } else {
      effect->scaleRateX = (options->targetScaleX - 0x10000) / script[3];
      effect->scaleRateY = (options->targetScaleY - 0x10000) / script[3];
    }
  }
  if ((flags & EFFECT_USE_SCRIPT) != 0) {
    Actor_SelectAnimationFar((struct Actor *)effect, 1);
    Actor_StartScriptFar((void *)effect, options->script);
  }
  if ((flags & EFFECT_USE_ROTATION) != 0)
    sprite->rotation = options->rotation;
  if ((flags & EFFECT_USE_SPIN) != 0)
    effect->spin = options->spin;
  if ((flags & EFFECT_USE_UPDATE) != 0)
    effect->update = options->update;
}
