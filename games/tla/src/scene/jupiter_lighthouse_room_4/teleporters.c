/// @file
/// The room of grey blocks and locked doors in Jupiter Lighthouse: its copy of
/// the effect spawner, and the cracked tiles, found, cracked and fallen
/// through.

#include "../shared/teleporter.h"
#include "types.h"
#include "main_routine_work.h"
#include "field_sprite.h"
#include "field_effect.h"
#include "fixed_math.h"
#include "event_inline.h"
#include "heap_release.h"
#include "item_icon.h"
#include "module/scene/scene.h"
#include "io_reg.h"
#include "field.h"
#include "event_work.h"

#include "field/cell_target.h"
struct CellTarget;
void Leader_CenterInCell(void);
extern struct CellTarget gTeleporters;
void Teleporter_Uncover(struct CellTarget *table,
                        struct CellTargetEntry *entry);

extern const s32 *gFieldEffect_LifeScripts[];

void Actor_SelectAnimation(struct Actor *object, s32 mode);
extern void Actor_StartScript(struct FieldEffect *effect, const s32 *script);

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options) {
  struct FieldEffect *party =
      ((struct FieldEffect *)Actor_GetById(gMainRoutineWork.currentOwner));
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
  Actor_SelectAnimation(effect, (flags + 1) & EFFECT_SCRIPT_MASK);
  Actor_StartScript(effect,
                    gFieldEffect_LifeScripts[flags & EFFECT_SCRIPT_MASK]);
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
    FieldActor_SetSpriteColor(effect, options->palette);
  if ((flags & EFFECT_USE_PRIORITY) != 0) {
    effect->priorityFlags &= ~1;
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

    script = gFieldEffect_LifeScripts[flags & EFFECT_SCRIPT_MASK];
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
    Actor_SelectAnimation(effect, 1);
    Actor_StartScript(effect, options->script);
  }
  if ((flags & EFFECT_USE_ROTATION) != 0)
    sprite->rotation = options->rotation;
  if ((flags & EFFECT_USE_SPIN) != 0)
    effect->spin = options->spin;
  if ((flags & EFFECT_USE_UPDATE) != 0)
    effect->update = options->update;
}

// The cells Teleporters_Step watches, and the cell it copies over the one the
// leader steps on.
#define CELL_TABLE gTeleporters
#define CELL_SPRITE_DRAW Teleporters_Update
#define CELL_PUSH_SLOT_ENTRY Import_OamAddSpriteAtDepthClamped
#include "../../field/map/cell_sprite.c"

#include "ram_buffer.h"
#include "actor.h"
#include "iwram_call.h"
#include "field_event.h"
#include "scene_tables.h"
#include "cell_target.h"

DEFINE_TELEPORTERS_FIND(Teleporters_Find, gTeleporters, Teleporters_Update)

void Leader_CenterInCell(void) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  s32 x = object->x;
  s32 z = object->z;

  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x40000, 0x20000);

  x >>= 20;
  z >>= 20;
  Actor_MoveToAndWaitById(gMainRoutineWork.currentOwner, (x << 4) + 8,
                          (z << 4) + 8);
  System_Wait(1);
  FieldActor_SetPositionAndResetMotion(object, (x << 20) + PIXELS(8), object->y,
                                       (z << 20) + PIXELS(8));
  FieldEvent_Wait(4);
  ((struct Actor *)FieldCamera_GetActor())->y += 0xfff80000;
}

/// Centers the leader on its cell, plays sound 161 and copies the cell below
/// the table's source cell over the leader's, keeping the source column's and
/// row's 64 bit; Teleporters_Step calls it the first time the leader steps on
/// one of the table's cells.
DEFINE_TELEPORTER_UNCOVER(Teleporter_Uncover, Leader_CenterInCell)

/* VIEW(s1888): tag struct CellTarget as struct CellTarget__s1888; cell_target.h
 * has another definition; tried with the definition the rest of the object has,
 * Teleporter_Sink moves: 2 of 131 instructions in tla-ja. */
struct CellTarget__s1888 {
  s16 firstFlag;
  s16 flagByte;
  s16 kind;
  s16 x;
  s16 z;
  s16 count;
  s16 current;
  s16 shown;
  s16 tile;
};

#include "io_write.h"

DEFINE_TELEPORTER_SINK(Teleporter_Sink, CellTarget__s1888, Leader_CenterInCell)
