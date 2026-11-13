#include "types.h"
#include "field_effect.h"
#include "main_routine_work.h"
#include "event_inline.h"

extern const s32 *gData_02007bc0[];

/* VIEW(s129c): routine Func_020057c8 as void Func_020057c8(struct FieldEffect *effect, s32 mode); games/tla/src/scene/trial_road/trial_road.h has void Func_020057c8 ( struct Actor * , s32 ). */
extern void Func_020057c8__s129c(struct FieldEffect *effect, s32 mode) __asm__("Func_020057c8");
void Actor_StartScript(struct FieldEffect *effect, const s32 *script);
/* VIEW(s129c): routine Actor_SetSingleChildFlags as void Actor_SetSingleChildFlags(struct FieldEffect *effect, s32 flags); field.h has void Actor_SetSingleChildFlags(struct Actor *object, u32 flags). */
extern void Actor_SetSingleChildFlags__s129c(struct FieldEffect *effect, s32 flags) __asm__("Actor_SetSingleChildFlags");

void Func_0200129c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options) {
  struct Actor *party = Actor_GetById(gMainRoutineWork.currentOwner);
  struct FieldEffect *effect;
  struct FieldSprite *sprite;
  const s32 *script;

  if ((flags & EFFECT_USE_TYPE) != 0 && options != 0)
    effect = ((struct FieldEffect *)Import_ActorCreate(options->type, x, y, z));
  else
    effect = ((struct FieldEffect *)Import_ActorCreate(285, x, y, z));
  if (effect == 0)
    return;

  sprite = effect->sprite;
  Func_020057c8__s129c(effect, (flags + 1) & EFFECT_SCRIPT_MASK);
  Actor_StartScript(effect, gData_02007bc0[flags & EFFECT_SCRIPT_MASK]);
  effect->motionFlags = 0;
  Actor_SetSingleChildFlags__s129c(effect, 0);
  effect->update = BattleEffect_UpdateObjectMotionScaleAndLinkedAngle;
  effect->velocity.x = velocityX;
  effect->velocity.y = velocityY;
  effect->velocity.z = velocityZ;
  Scene_SetActorSpritePriority(effect,
                               ((struct FieldSprite *)party->sprite)->priority);
  effect->scaleRateX = 0;
  effect->scaleRateY = 0;
  effect->spin = 0;

  if ((flags & 0xffff0000) == 0 || options == 0)
    return;
  if ((flags & EFFECT_USE_PALETTE) != 0)
    Actor_SetPartAttribute(effect, options->palette);
  if ((flags & EFFECT_USE_PRIORITY) != 0) {
    effect->priorityFlags &= ~1;
    Scene_SetActorSpritePriority(effect, options->priority);
  }
  if ((flags & EFFECT_USE_START_SCALE) != 0) {
    effect->scaleX = options->startScaleX;
    effect->scaleY = options->startScaleY;
  }
  if ((flags & EFFECT_SCALE_TO_TARGET) != 0) {
    s32 target;
    s32 frames;

    script = gData_02007bc0[flags & EFFECT_SCRIPT_MASK];
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
    Func_020057c8__s129c(effect, 1);
    Actor_StartScript(effect, options->script);
  }
  if ((flags & EFFECT_USE_ROTATION) != 0)
    sprite->rotation = options->rotation;
  if ((flags & EFFECT_USE_SPIN) != 0)
    effect->spin = options->spin;
  if ((flags & EFFECT_USE_UPDATE) != 0)
    effect->update = options->update;
}

#include "../../field/field_effect/advance_motion_priority.c"

#include "system.h"
#include "actor.h"
#include "frame.h"

void Func_020014e4(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct EffectOptions options;
  s32 velocity[3];

  if (gMainRoutineWork.renderMode == 4)
    return;
  leader->z += 0x10000;
  if ((gFrameCount & 7) != 0)
    return;
  if (leader->velocityZ == 0 && leader->velocityX == 0)
    return;
  options.update = Effect_AdvanceMotionAtLeaderPriority;
  options.startScaleX = 0xcccc;
  options.startScaleY = 0xcccc;
  velocity[1] = 0;
  velocity[2] = 0x10000;
  velocity[0] = 0;
  velocity[0] += ((Random16Far() << 17) >> 16) - 0x10000;
  velocity[2] += ((Random16Far() << 16) >> 16) - 0x8000;
  Func_0200129c(leader->x, leader->y, leader->z, velocity[0], velocity[1],
                velocity[2], 0x1000001, &options);
}
