/// @file
/// Scene code of overlay 0x6a4, in ROM order, from s1e78's offset up to the
/// next object the overlay links.

#include "../shared/routines.h"
#include "types.h"
#include "field_effect.h"
#include "main_routine_work.h"
#include "event_inline.h"
#include "heap_release.h"
#include "item_icon.h"
#include "module/scene/scene.h"

extern const s32 *gData_02003330[];

void Actor_StartScript(struct FieldEffect *effect, const s32 *script);
void Actor_SetSingleChildFlags(struct FieldEffect *effect, s32 flags);

void Func_02001e78(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
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
  Func_020029f0(effect, (flags + 1) & EFFECT_SCRIPT_MASK);
  Actor_StartScript(effect, gData_02003330[flags & EFFECT_SCRIPT_MASK]);
  effect->motionFlags = 0;
  Actor_SetSingleChildFlags(effect, 0);
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
    Actor_SetSpriteColor(effect, options->palette);
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

    script = gData_02003330[flags & EFFECT_SCRIPT_MASK];
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
    Func_020029f0(effect, 1);
    Actor_StartScript(effect, options->script);
  }
  if ((flags & EFFECT_USE_ROTATION) != 0)
    sprite->rotation = options->rotation;
  if ((flags & EFFECT_USE_SPIN) != 0)
    effect->spin = options->spin;
  if ((flags & EFFECT_USE_UPDATE) != 0)
    effect->update = options->update;
}

#define CELL_TABLE gData_02003488
#define CELL_SPRITE_DRAW Func_02002040
#define CELL_PUSH_SLOT_ENTRY Func_020029c0
#include "../../field/map/cell_sprite.c"

#include "ram_buffer.h"
#include "actor.h"
#include "iwram_call.h"
#include "cell_target.h"

DEFINE_FUNC_02001250(Func_0200216c, gData_02003488, Func_02002040)

#include "jupiter_lighthouse_aerie.h"
#include "scene_tables.h"

DEFINE_FUNC_020013E4(Func_020023b0)
#include "../shared/cell_with_bit.h"

/// Centers the leader on its cell, plays sound 161 and copies the cell below
/// the table's source cell over the leader's, keeping the source column's and
/// row's 64 bit; Func_020027d8 calls it the first time the leader steps on
/// one of the table's cells.
void Func_02002438(struct CellTarget *table, struct CellTargetEntry *entry) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s16 sourceX;
  s32 leaderX = leader->x;
  s32 x;
  s32 z;

  /* FAKEMATCH(barrier): passes the source column through an asm reading the
   * leader's x, so the x load ties the 64 mask's copy and wins on order; as
   * (leader->x >> 20) | (s16)(table->x & 64), 2 of 49 instructions differ in
   * tla-en. */
  asm("" : "=r"(sourceX) : "0"(table->x), "r"(leaderX));
  x = (leaderX >> 20) | (s16)(sourceX & 64);
  z = CellTarget_CellWithBit(leader->z, table->z & 64);
  Func_020023b0();
  Event_PlaySound(161);
  MapGrid_CopyCells(table->x, table->z + 1, 1, 1, x, z);
  FieldEvent_Wait(12);
}
