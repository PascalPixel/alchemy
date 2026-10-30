// Matches: differ 0 of 214 (tla-en, 2026-10-30); its literal pool's three
// words (gData_02003840, the update hook, 0xffff0000) agree with the
// listing's. ⚓️'s Effect_Spawn as overlay 6b0 links it after
// field/field_effect/move.c: games/tla/src/field/field_effect/spawn.c's
// EFFECT_SPRITE_PRIORITY form word for word, except that the party is actor 0
// (Actor_GetById(0)) where spawn.c reads gMainRoutineWork.currentOwner; it
// matches with or without spawn.c's Effect_Create wrapper, so it is written
// without. Adopting it through spawn.c needs only a way to name the party.
// Earlier: the draft read the velocity as velocityX..Z, which the header now
// spells velocity.x..z; with that fix its own form (the sprite priority set
// through a SpriteView in a one-pass block with the priority call) also
// measured differ 0, and 4 without the block.
#include "types.h"
#include "main_routine_work.h"
#include "field_sprite.h"
#include "field_effect.h"
#include "fixed_math.h"

extern struct FieldEffect *Actor_GetById(s32 id);
extern struct FieldActor *Import_ActorCreate(s32 type, s32 x, s32 y, s32 z);
extern void Func_020032e0(struct FieldEffect *effect, s32 mode);
extern void Func_020032e8(struct FieldEffect *effect, const s32 *script);
extern const s32 *gData_02003840[];
extern void Actor_SetSingleChildFlags(struct FieldEffect *effect, u32 flags);
extern void Scene_SetActorSpritePriority(struct FieldEffect *effect,
                                         s32 priority);
extern void Actor_SetPartAttribute(struct FieldEffect *effect, s32 attribute);

void Func_02000480(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options) {
  struct FieldEffect *party = Actor_GetById(0);
  struct FieldEffect *effect;
  struct FieldSprite *sprite;
  const s32 *script;
  u32 start;

  if ((flags & EFFECT_USE_TYPE) != 0 && options != 0)
    effect = (struct FieldEffect *)Import_ActorCreate(options->type, x, y, z);
  else
    effect = (struct FieldEffect *)Import_ActorCreate(285, x, y, z);
  if (effect == 0)
    return;

  sprite = effect->sprite;
  Func_020032e0(effect, (flags + 1) & EFFECT_SCRIPT_MASK);
  Func_020032e8(effect, gData_02003840[flags & EFFECT_SCRIPT_MASK]);
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
    /* FAKEMATCH(one-pass): the flag cleared in a once-only block; as a plain
     * statement, 4 of 214 instructions differ in tla-en. */
    do {
      effect->priorityFlags &= ~1;
    } while (0);
    // Read as its low byte: (u8)options->priority orders the loads apart.
    sprite->priority = *(u8 *)&options->priority;
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

    script = gData_02003840[flags & EFFECT_SCRIPT_MASK];
    targetX = options->targetScaleX;
    count = script[3];
    if (start != 0) {
      effect->scaleRateX = (targetX - effect->scaleX) / count;
      effect->scaleRateY = (options->targetScaleY - effect->scaleY) / script[3];
    } else {
      effect->scaleRateX = (targetX - Q16_ONE) / count;
      effect->scaleRateY = (options->targetScaleY - Q16_ONE) / script[3];
    }
  }
  if ((flags & EFFECT_USE_SCRIPT) != 0) {
    Func_020032e0(effect, 1);
    Func_020032e8(effect, options->script);
  }
  if ((flags & EFFECT_USE_ROTATION) != 0)
    sprite->rotation = options->rotation;
  if ((flags & EFFECT_USE_SPIN) != 0)
    effect->spin = options->spin;
  if ((flags & EFFECT_USE_UPDATE) != 0)
    effect->update = options->update;
}
