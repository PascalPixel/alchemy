/// @file
/// Scene code of overlay 0x6b0, in ROM order.

#include "types.h"
#include "main_routine_work.h"
#include "field_sprite.h"
#include "field_effect.h"
#include "fixed_math.h"
#include "event_inline.h"
#include "actor.h"

#include "macros.h"
#include "field.h"
#include "module/scene/anemos_inner_sanctum.h"

void Func_020032e0(struct Actor *, s32);
void Actor_StartScript(struct Actor *object, const s32 *script);
extern const s32 *gData_02003840[];

void Func_02000480(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options) {
  struct FieldEffect *party = ((struct FieldEffect *)Actor_GetById(0));
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
  Actor_StartScript(effect, gData_02003840[flags & EFFECT_SCRIPT_MASK]);
  effect->motionFlags = 0;
  Actor_SetSingleChildFlags(effect, 0);
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
    FieldActor_SetSpriteColor(effect, options->palette);
  if ((flags & EFFECT_USE_PRIORITY) != 0) {
    effect->priorityFlags &= ~1;
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
    Actor_StartScript(effect, options->script);
  }
  if ((flags & EFFECT_USE_ROTATION) != 0)
    sprite->rotation = options->rotation;
  if ((flags & EFFECT_USE_SPIN) != 0)
    effect->spin = options->spin;
  if ((flags & EFFECT_USE_UPDATE) != 0)
    effect->update = options->update;
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

s32 Func_02000654(s32 a0) {
  Actor_SetSingleChildFlags(a0, 0);
  return 0;
}

s32 Func_02000660(struct Actor *object) {
  s32 radius;

  object->timer += object->unk66;
  SET_POSITION(object, object->bounce, object->gravity, 0x980000);
  FieldVector_AddPolarOffset(object->unk4c, object->timer, &object->x);
  radius = object->unk4c - 0x10000;
  object->unk4c = radius;
  if (radius)
    return 1;
  return 0;
}

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

extern const u8 gData_02003674[];

void Func_020006a4(struct Actor *source) {
  struct Actor *object =
      (FieldActor_Create(672, source->x, source->y, source->z));

  object->motionFlags = 0;
  FieldActor_SetSpriteColor(object, BYTE_AT(source, 98));
  Scene_SetActorSpritePriority(object, 2);
  Func_020032e0(object, 1);
  Actor_SetSingleChildFlags(object, 0);
  Actor_StartScript(object, gData_02003674);
  Actor_SetBlendMode(object, 1);
}

s32 Func_020006f8(struct Mover *mover) {
  if (mover->radius < mover->step)
    return 0;
  mover->angle += mover->spin >> 1;
  mover->x = mover->originX;
  mover->y = mover->height;
  mover->z = mover->originZ;
  // Read through a pointer of its own type, an access alchemy-gcc keeps apart
  // from the s32 stores before it: as mover->angle, the angle loads before
  // the radius (2 of 73 instructions differ).
  FieldVector_AddPolarOffset(mover->radius, *(s16 *)&mover->angle, &mover->x);
  Func_020006a4(mover);
  mover->radius -= mover->step >> 1;
  mover->angle += mover->spin >> 1;
  mover->x = mover->originX;
  mover->y = mover->height;
  mover->z = mover->originZ;
  FieldVector_AddPolarOffset(mover->radius, mover->angle, &mover->x);
  Func_020006a4(mover);
  mover->radius -= mover->step >> 1;
  mover->height -= mover->fall;
  return 1;
}

s32 Func_02000794(struct Actor *object) {
  struct SpawnParams params;
  s32 x;
  s32 y;

  params.kind = 2;
  params.part = *((u8 *)object + 98);
  Func_02000480(object->x, object->y, object->z, 0, 0x10000, 0, 0x30000,
                &params);
  x = (object->bounce >> 20) + 64;
  y = object->gravity >> 20;
  MapGrid_CopyCellsTo(x, y + 26, x, y, 1, 1);
  return 0;
}

#include "../../field/object/wait_until_idle.c"
