/// @file
/// Scene code of overlay 0x6ab, in ROM order.

#include "types.h"
#include "main_routine_work.h"
#include "field_sprite.h"
#include "field_effect.h"
#include "event_inline.h"

void Actor_SelectAnimation(struct Actor *object, s32 mode);
extern void Actor_StartScript(struct FieldEffect *effect, const s32 *script);
extern const s32 *gData_02004ce0[];
/* VIEW(s363c): routine Actor_SetSpriteFlags as void Actor_SetSpriteFlags(struct
 * FieldEffect *effect, s32 flags); field/field.h has void
 * Actor_SetSpriteFlags(struct Actor *object, u32 flags). */
void Actor_SetSpriteFlags__s363c(struct FieldEffect *effect,
                                 s32 flags) __asm__("Actor_SetSpriteFlags");
void Func_0200363c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options) {
  struct FieldEffect *party =
      (struct FieldEffect *)Actor_GetById(gMainRoutineWork.currentOwner);
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
  Actor_StartScript(effect, gData_02004ce0[flags & EFFECT_SCRIPT_MASK]);
  effect->motionFlags = 0;
  Actor_SetSpriteFlags__s363c(effect, 0);
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

    script = gData_02004ce0[flags & EFFECT_SCRIPT_MASK];
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

// ---- scene/mars_lighthouse_star_room/s3804.c ----

/// Scene code of overlay 0x6ab, in ROM order.

#include "../shared/cracked_tile.h"
#include "event_work.h"
#include "field.h"
#include "heap_release.h"
#include "item_icon.h"
#include "module/scene/scene.h"

#define CELL_TABLE gData_02004cf4
#define CELL_SPRITE_DRAW CrackedTiles_Update
#define CELL_PUSH_SLOT_ENTRY Oam_AddSpriteAtDepth
#include "../../field/map/cell_sprite.c"

#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "iwram_call.h"
#include "event_inline.h"
#include "cell_target.h"

DEFINE_CRACKED_TILES_FIND(Func_02003930, gData_02004cf4, CrackedTiles_Update)

#include "scene_tables.h"

DEFINE_LEADER_CENTER_IN_CELL(Leader_CenterInCell)

/// Map_CopyCellsRect through this overlay's import.

/// Centers the leader on its cell, plays sound 161 and copies the cell below
/// the table's source cell over the leader's, keeping the source column's and
/// row's 64 bit; CrackedTiles_Step calls it the first time the leader steps on
/// one of the table's cells.
DEFINE_CRACKED_TILE_CRACK(Func_02003bfc, Leader_CenterInCell)

#include "io_reg.h"

DEFINE_CRACKED_TILE_FALL_EVENT(CrackedTile_FallEvent, Leader_CenterInCell)
