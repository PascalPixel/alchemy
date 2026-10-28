// Draft, differ 2 of 214: ⚓️'s Effect_Spawn as overlay 6b0 links it after
// field/shared/effect/move.c, the twin of ☀️'s EFFECT_SPAWN with party actor 0,
// default type 285 and the sprite priority both written and set. Left: after
// the mode call the native loads the script table's address before it copies
// the flags from sl (ldr r2, =gData_02003840; mov r3, sl); the draft copies
// first. sched2 ranks the copy one above the table's reload (61 against 60),
// and the table's pseudo, shared with the scale-to-target block by cse and
// again by cse2, is never allocated. Measured: a script local (11), a table
// pinned to r2 (8), one-pass blocks around the mode or script call (2 or 4), a
// barrier after the table (32), index forms (2 or 34); alchemy permute 7
// minutes found nothing below 2. The sprite view with an s32 member makes the
// priority store alias the options read, as the native's two reads need; the
// one-pass block around it is alchemy permute's (6 to 2).
#include "types.h"
#include "field_effect.h"
#include "field_sprite.h"

struct SpriteView {
  u8 unk00[8];
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unk0a[10];
  s32 unk14;
};

extern const s32 *gData_02003840[];

struct FieldActor *Object_GetById(s32 id);
struct FieldActor *Engine_ObjectCreate(s32 type, s32 x, s32 y, s32 z);
void Func_020032e0(struct FieldActor *object, s32 mode);
void Func_020032e8(struct FieldActor *object, const void *script);
void ObjectDispatch_SetSingleChildFlags(struct FieldActor *object, u32 value);
void Object_SetPartAttribute(struct FieldActor *object, s32 attribute);
void Object_SetSpritePriority(struct FieldActor *object, s32 priority);

void Func_02000480(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options) {
  struct FieldActor *party = Object_GetById(0);
  struct FieldEffect *effect;
  struct FieldSprite *sprite;
  const s32 *script;
  u32 start;

  if ((flags & EFFECT_USE_TYPE) != 0 && options != 0)
    effect = (struct FieldEffect *)Engine_ObjectCreate(options->type, x, y, z);
  else
    effect = (struct FieldEffect *)Engine_ObjectCreate(285, x, y, z);
  if (effect == 0)
    return;

  sprite = effect->sprite;
  Func_020032e0((struct FieldActor *)effect, (flags + 1) & EFFECT_SCRIPT_MASK);
  Func_020032e8((struct FieldActor *)effect,
                gData_02003840[flags & EFFECT_SCRIPT_MASK]);
  effect->motionFlags = 0;
  ObjectDispatch_SetSingleChildFlags((struct FieldActor *)effect, 0);
  effect->update = BattleEffect_UpdateObjectMotionScaleAndLinkedAngle;
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
    do {
      ((struct SpriteView *)sprite)->priority = options->priority;
      Object_SetSpritePriority((struct FieldActor *)effect, options->priority);
    } while (0);
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
      effect->scaleRateX = (targetX - 0x10000) / count;
      effect->scaleRateY = (options->targetScaleY - 0x10000) / script[3];
    }
  }
  if ((flags & EFFECT_USE_SCRIPT) != 0) {
    Func_020032e0((struct FieldActor *)effect, 1);
    Func_020032e8((struct FieldActor *)effect, options->script);
  }
  if ((flags & EFFECT_USE_ROTATION) != 0)
    sprite->rotation = options->rotation;
  if ((flags & EFFECT_USE_SPIN) != 0)
    effect->spin = options->spin;
  if ((flags & EFFECT_USE_UPDATE) != 0)
    effect->update = options->update;
}
