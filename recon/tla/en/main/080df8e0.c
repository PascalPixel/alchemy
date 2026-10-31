// Near miss, differ 2 of 218 (tla-en, 2026-10-30): one scheduler tie. Under
// EFFECT_USE_PRIORITY the listing copies the sprite pointer (mov r3, r8)
// before loading options->priority; here the load goes first. After the
// priorityFlags store both are ready at sched2 priority 7, and the load wins
// rank_for_schedule's class test (its dependence on that store costs 1; the
// copy's on r3 is an anti-dependence). Its address is r7, the Thumb hard frame
// pointer, so -mthumb-slow-loads never charges it a cycle, even as a plain
// load. No form of the block changes the order: the two statements swapped, a
// temporary for the priority, (u8) or *(u8 *)& reads, u8 bitfields, a one-pass
// block or an empty asm on either side; the permuter (4 minutes, 16,895
// candidates) found nothing better.
// The sprite is typed as the projection draft lays it out (recon/tla
// LayeredSprite), bytes 0x0c to 0x0f its s32 16.16 scale. That int member
// lets the sprite's priority byte alias the scaleRateX and _y stores, which
// then stay after the byte copy as in the listing; as struct FieldSprite,
// whose header names those bytes unk0a[6], differ 6 (both stores lifted above
// the party sprite's byte load). FieldSprite itself read so (unk0a[2], then
// s32 scale) measures the same 2 with this body unchanged; that header change
// needs checking against every matched user of FieldSprite.
// Earlier: ☀️'s EFFECT_SPAWN (Effect_SpawnResident) with ⚓️'s calls
// (Actor_GetById(0), Func_080200c0, default type 285, the gData_080f3978
// scripts, Actor_SetPartAttribute); a barrier after the priority copy swaps r9
// and r11.
#include "types.h"
#include "field_effect.h"
#include "objdisp.h"

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

extern const s32 *gData_080f3978[];

struct FieldActor *Actor_GetById(s32 id);
struct FieldEffect *Func_080200c0(s32 type, s32 x, s32 y, s32 z);
void Actor_SetPartAttribute(struct FieldActor *object, s32 value);
void BattleEffect_UpdateObjectMotionScaleAndLinkedAngle(union FieldObject *object);

void Func_080df8e0(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options) {
  struct FieldActor *party = Actor_GetById(0);
  struct FieldEffect *effect;
  struct LayeredSprite *sprite;
  const s32 *script;

  if ((flags & EFFECT_USE_TYPE) != 0 && options != 0)
    effect = Func_080200c0(options->type, x, y, z);
  else
    effect = Func_080200c0(285, x, y, z);
  if (effect == 0)
    return;

  sprite = (struct LayeredSprite *)effect->sprite;
  Actor_SetMode((struct FieldActor *)effect, (flags + 1) & EFFECT_SCRIPT_MASK);
  Actor_StartScriptFar((void *)effect,
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
    Actor_SetPartAttribute((struct FieldActor *)effect, options->palette);
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
    Actor_SetMode((struct FieldActor *)effect, 1);
    Actor_StartScriptFar((void *)effect, options->script);
  }
  if ((flags & EFFECT_USE_ROTATION) != 0)
    sprite->rotation = options->rotation;
  if ((flags & EFFECT_USE_SPIN) != 0)
    effect->spin = options->spin;
  if ((flags & EFFECT_USE_UPDATE) != 0)
    effect->update = options->update;
}
