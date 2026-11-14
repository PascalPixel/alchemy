/// @file
/// The effect spawner each field overlay keeps its own copy of, the twin of The
/// Broken Seal's EFFECT_SPAWN (scene/shared/field_effect_spawn.h) with default
/// type 285. The including file names its overlay's copy and the imports it
/// calls: EFFECT_SPAWN, EFFECT_CREATE, EFFECT_SET_MODE, EFFECT_SET_SCRIPT and
/// EFFECT_SCRIPTS. Copies that also define EFFECT_SPRITE_PRIORITY give the
/// sprite the requested priority themselves before
/// Scene_SetActorSpritePriority, as overlays 663, 664, 66b, 679 and 67b do.
#include "types.h"
#include "system/main_routine_work.h"
#include "field_sprite.h"
#include "system/field_effect.h"
#include "fixed_math.h"

// The spawner sees its callees through its own views, as every copy did on
// its own: the including scene's declarations of the same names do not
// reach it, whatever they say.
#define EFFECT_NAME(name) EFFECT_NAME_STRING(name)
#define EFFECT_NAME_STRING(name) #name

/* VIEW(spawn): routine Actor_GetById as struct FieldEffect *Actor_GetById(s32
 * id); field/event.h has struct Actor *Actor_GetById(u32 objectId); tried as
 * the definition's struct Actor *Actor_GetById(u32 objectId), which the object
 * already declares, it does not compile in tla-ja (garoh.c) (make modern):
 * games/tla/src/field/field_effect/spawn.c:42:41: note: expected 'struct Actor
 * *' but argument is of type 'struct FieldEffect *'. */
extern struct FieldEffect *
Actor_GetById__spawn(s32 id) __asm__("Actor_GetById");
extern struct Actor *EFFECT_CREATE(s32 type, s32 x, s32 y, s32 z);
#ifdef EFFECT_SPRITE_PRIORITY
static inline struct FieldEffect *Effect_CreateActor(s32 type, s32 x, s32 y,
                                                     s32 z) {
  return (struct FieldEffect *)EFFECT_CREATE(type, x, y, z);
}
#endif
/* VIEW(spawn): routine EFFECT_SET_MODE as void EFFECT_SET_MODE(struct
 * FieldEffect *effect, s32 mode); module/field/field.h has void
 * Actor_SelectAnimation(struct Actor *object, s32 argument); not tried:
 * EFFECT_SET_MODE's bound declaration was not found. */
extern void SetMode__spawn(struct FieldEffect *effect,
                           s32 mode) __asm__(EFFECT_NAME(EFFECT_SET_MODE));
/* VIEW(spawn): routine EFFECT_SET_SCRIPT as void EFFECT_SET_SCRIPT(struct
 * FieldEffect *effect, const s32 *script); actor_view.h has void
 * Actor_StartScript(struct Actor *, const s32 *); not tried:
 * EFFECT_SET_SCRIPT's bound declaration was not found. */
extern void
SetScript__spawn(struct FieldEffect *effect,
                 const s32 *script) __asm__(EFFECT_NAME(EFFECT_SET_SCRIPT));
extern const s32 *EFFECT_SCRIPTS[];
void Actor_SetSpriteFlags(struct Actor *object, u32 value);

void EFFECT_SPAWN(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                  s32 velocityZ, u32 flags,
                  const struct EffectOptions *options) {
  struct FieldEffect *party =
      Actor_GetById__spawn(gMainRoutineWork.currentOwner);
  struct FieldEffect *effect;
  struct FieldSprite *sprite;
  const s32 *script;
  u32 start;

#ifdef EFFECT_SPRITE_PRIORITY
  /* FAKEMATCH(wrapper): the create call forwarded through Effect_CreateActor.
   */
  if ((flags & EFFECT_USE_TYPE) != 0 && options != 0)
    effect = Effect_CreateActor(options->type, x, y, z);
  else
    effect = Effect_CreateActor(285, x, y, z);
#else
  if ((flags & EFFECT_USE_TYPE) != 0 && options != 0)
    effect = (struct FieldEffect *)EFFECT_CREATE(options->type, x, y, z);
  else
    effect = (struct FieldEffect *)EFFECT_CREATE(285, x, y, z);
#endif
  if (effect == 0)
    return;

  sprite = effect->sprite;
  SetMode__spawn(effect, (flags + 1) & EFFECT_SCRIPT_MASK);
  SetScript__spawn(effect, EFFECT_SCRIPTS[flags & EFFECT_SCRIPT_MASK]);
  effect->motionFlags = 0;
  Actor_SetSpriteFlags(effect, 0);
  effect->update = FieldEffect_UpdateMotionScaleAndSpin;
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
    Actor_SetSpriteColor(effect, options->palette);
  if ((flags & EFFECT_USE_PRIORITY) != 0) {
#ifdef EFFECT_SPRITE_PRIORITY
    effect->priorityFlags &= ~1;
    // Read as its low byte: (u8)options->priority orders the loads apart.
    sprite->priority = *(u8 *)&options->priority;
#else
    effect->priorityFlags &= ~1;
#endif
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

    script = EFFECT_SCRIPTS[flags & EFFECT_SCRIPT_MASK];
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
    SetMode__spawn(effect, 1);
    SetScript__spawn(effect, options->script);
  }
  if ((flags & EFFECT_USE_ROTATION) != 0)
    sprite->rotation = options->rotation;
  if ((flags & EFFECT_USE_SPIN) != 0)
    effect->spin = options->spin;
  if ((flags & EFFECT_USE_UPDATE) != 0)
    effect->update = options->update;
}

#undef EFFECT_NAME
#undef EFFECT_NAME_STRING
