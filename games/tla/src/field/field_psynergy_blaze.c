/// @file
/// Blaze, and the check whether an actor stands where the map or another
/// actor blocks it.
// Part of field_psynergy.c.

#include "field_psynergy_internal.h"
#include "resource_ids.h"
#include "fixed_math.h"
#include "sounds.h"
#include "system/input.h"
#include "system/iwram_call.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "system/heap_release.h"

#include "sprite_ids.h"

extern const s32 gBlaze_TintColors[];

void Field_StartColorTransition(s32 frames, s32 color);

/// The first layer of an actor's sprite.
#define ACTOR_FIRST_LAYER(actor)                                               \
  (((struct Sprite *)(actor)->sprite)->entries[0])

/// The jet's longest spray, in frames; B ends it sooner.
#define BLAZE_SPRAY_FRAMES 80
/// How far in front of the caster the jet's drops are drawn.
#define BLAZE_DROP_DEPTH 10

/// The flame's colour, after the torch it is cast at: an index of
/// gBlaze_TintColors.
enum BlazeLook { BLAZE_RED, BLAZE_VIOLET, BLAZE_YELLOW, BLAZE_BLUE };

/// Blaze: a jet of flame sprays from the caster, with smoke, until B is pressed
/// or for BLAZE_SPRAY_FRAMES at most: red, or violet, yellow or blue to match
/// the torch (SPRITE_MARS_TORCH_LIT) it is cast at. Each actor it reaches runs
/// its Blaze event, and one that is lit by it ends the jet, its actor id kept
/// in the field work. Cast at nothing, the nearest fire in reach is the target:
/// a brazier, a lit candlestick, a lit Mars Lighthouse torch, or an actor
/// wearing SPRITE_EXCLAMATION_BUBBLE (the Gondowan Cliffs hideout's unseen
/// marker at its fire).
void FieldPsynergy_Blaze(void) {
  struct BlazeWork *work;
  struct FieldWork *event;
  struct FieldEffectWork *scene;
  struct Actor *caster;
  struct Actor *actor;
  struct EffectSprite *sprite;
  union {
    struct Actor *actor;
    struct FieldPsynergyParticle *particle;
  } cursor;
  void *pending;
  s32 pos[3];
  s32 kind;
  s32 look;
  s32 best;
  s32 distance;
  FIXED height;
  s32 id;
  s32 resourceId;
  s32 dropSlot;
  s32 puffSlot;
  s32 tile;
  s32 i;
  s32 j;
  s32 k;
  s32 mode;

  work = HeapSlot_Alloc(HEAP_SLOT_BATTLE_EFFECT, sizeof(struct BlazeWork));
  scene = Ram_HeapSlots->fieldEffectWork;
  event = Ram_HeapSlots->fieldWork;
  caster = scene->mainActor;
  look = BLAZE_RED;
  actor = scene->child;
  if (Actor_GetSpriteIdFar(actor) == SPRITE_MARS_TORCH_LIT) {
    kind = ACTOR_FIRST_LAYER(actor)->colorMode;
    if (kind == SPRITE_COLOR_GBR)
      look = BLAZE_VIOLET;
    if (kind == SPRITE_COLOR_GRB)
      look = BLAZE_YELLOW;
    if (kind == SPRITE_COLOR_BGR)
      look = BLAZE_BLUE;
  }
  Field_StartColorTransition(8, gBlaze_TintColors[look]);
  work->mode = BLAZE_FLICKER;
  if (scene->enabled != 0) {
    work->origin[0] = scene->x;
    work->origin[1] = scene->y + toFIXED(16);
    work->origin[2] = scene->z;
    best = BEYOND_REACH;
    for (i = 0; i < FIELD_ACTOR_COUNT; i++) {
      cursor.actor = event->actors[i];
      id = ACTOR_FIRST_LAYER(cursor.actor)->animationId;
      if (cursor.actor != NULL && cursor.actor->script != NULL &&
          (id == SPRITE_EXCLAMATION_BUBBLE || id == SPRITE_BRAZIER ||
           id == SPRITE_LIT_CANDLESTICK || id == SPRITE_MARS_TORCH_LIT)) {
        distance = Field_MeasurePsynergyReach(scene->fieldPsynergy,
                                              scene->firstActorId, i);
        if (distance != -1 && best > distance) {
          height = toFIXED((s8)Resource_GetMetadataRecordFarFar(
                               ACTOR_FIRST_LAYER(cursor.actor)->animationId)
                               ->boxX);
          work->mode = BLAZE_LIGHT_TORCH;
          work->origin[0] = cursor.actor->x.fixed;
          work->origin[1] = cursor.actor->y.fixed + height;
          work->origin[2] = cursor.actor->z.fixed;
          scene->secondActorId = i;
          scene->child = cursor.actor;
          best = distance;
        }
      }
    }
  } else {
    if (event->blazeLongSpray == 0)
      work->mode = BLAZE_SPRAY;
    else
      work->mode = BLAZE_LONG_SPRAY;
    if (scene->child != NULL)
      height = toFIXED((s8)Resource_GetMetadataRecordFarFar(
                           ACTOR_FIRST_LAYER(scene->child)->animationId)
                           ->boxX);
    else
      height = toFIXED(20);
    work->origin[0] = scene->x;
    work->origin[1] = scene->y + height;
    work->origin[2] = scene->z;
  }

  mode = work->mode;
  if (mode == BLAZE_LIGHT_TORCH) {
    pos[0] = work->origin[0];
    pos[1] = work->origin[1];
    pos[2] = work->origin[2];
    actor = Actor_Spawn(SPRITE_BLAZE_FLAME, pos[0], pos[1], pos[2]);
    if (actor != NULL) {
      actor->motionFlags = 0;
      actor->scaleX = FIXED_ONE * 5 / 4;
      actor->scaleY = FIXED_ONE * 5 / 4;
      Actor_SelectAnimationFar(actor, 2);
      Sound_PlayFar(SE_SOL_SANCTUM_LIGHT_UP);
      wait(50);
      Actor_ReleaseFar(actor);
      wait(10);
    }
  } else if (mode == BLAZE_FLICKER) {
    pos[0] = work->origin[0];
    pos[1] = work->origin[1];
    pos[2] = work->origin[2];
    actor = Actor_Spawn(SPRITE_BLAZE_FLICKER, pos[0], pos[1], pos[2]);
    if (actor != NULL) {
      actor->motionFlags = mode;
      Actor_SelectAnimationFar(actor, 2);
      Sound_PlayFar(SE_SOL_SANCTUM_LIGHT_UP);
      wait(45);
      Actor_ReleaseFar(actor);
      wait(10);
    }
  } else {

    Sound_PlayFar(SE_RUSH);
    BattleEffect_SetObjectUpdateMode(scene->mainActor, 0);
    work->angle = scene->angle8;
    work->targetAngle = scene->angle8;
    switch (look) {
    case BLAZE_RED:
      resourceId = RESOURCE_RED_FLAME_BURST;
      break;
    case BLAZE_VIOLET:
      resourceId = RESOURCE_VIOLET_FLAME_BURST;
      break;
    case BLAZE_YELLOW:
      resourceId = RESOURCE_YELLOW_FLAME_BURST;
      break;
    case BLAZE_BLUE:
      resourceId = RESOURCE_BLUE_FLAME_BURST;
      break;
    default:
      resourceId = RESOURCE_RED_FLAME_BURST;
      break;
    }
    Decompress_TaggedLz(Resource_GetTableEntry(resourceId), work);
    dropSlot = VramCache_FindFreeEntry();
    tile = VramCache_Load(dropSlot, 4096, work);
    work->dropTile = tile;
    cursor.particle = work->dropMotion;
    for (j = 0; j < BLAZE_DROPS; j++) {
      sprite = &work->drops[j];

      EffectSprite_Init(sprite, 15, 24, OBJ_SIZE_32X32, tile);
      sprite->depth = BattleEffect_GetObjectDepth(caster) + BLAZE_DROP_DEPTH;
      sprite->oam.priority = BattleEffect_GetObjectPriority(caster);
      sprite->oam.fullColor = 1;
      sprite->oam.palette = 0;
      cursor.particle->timer = -1;
      cursor.particle++;
    }
    Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_DUST_PUFF_A), work);
    puffSlot = VramCache_FindFreeEntry();
    tile = VramCache_Load(puffSlot, 2048, work);
    work->puffTile = tile;
    cursor.particle = work->puffMotion;
    for (j = 0; j < BLAZE_DROPS; j++) {
      sprite = &work->puffs[j];

      EffectSprite_Init(sprite, 8, 15, OBJ_SIZE_16X16, tile);
      sprite->scaleY = sprite->scaleX = FIXED_ONE * 3 / 4;
      sprite->depth = EFFECT_SPRITE_FRONT_DEPTH;
      sprite->oam.priority = 0;
      sprite->oam.fullColor = 1;
      sprite->oam.palette = 0;
      cursor.particle->timer = -1;
      cursor.particle++;
    }
    wait(1);
    work->emitting = 1;
    Task_Add(Blaze_UpdateSpray, TASK_PASS_FRAME);
    for (k = 0; k < BLAZE_SPRAY_FRAMES; k++) {
      wait(1);
      if (work->hit != 0) {
        pending = Scene_FindPsynergyTrigger(
            PSYNERGY_TRIGGER(PSYNERGY_PHASE_HITS), scene->fieldPsynergy,
            (s16)(work->hitObject | PSYNERGY_TARGET_ACTOR));
        if (pending != NULL)
          Field_RunPsynergyEvent(pending, scene->firstActorId, work->hitObject);
        if (Scene_FindPsynergyTrigger(
                PSYNERGY_TRIGGER(PSYNERGY_PHASE_AFTER_END),
                scene->fieldPsynergy,
                (s16)(work->hitObject | PSYNERGY_TARGET_ACTOR)) != NULL) {
          event->psynergyHitObject = work->hitObject;
          break;
        }
      }
      if (gInput.pressed & KEY_B)
        break;
    }
    Sound_PlayFar(SOUND_FADE_OUT_EXIT_SE);
    work->emitting = 0;
    for (k = 0; k < 60; k++) {
      cursor.particle = work->dropMotion;
      for (j = 0; j < BLAZE_DROPS && cursor.particle->timer == -1;
           j++, cursor.particle++)
        ;
      if (j == BLAZE_DROPS)
        break;
      wait(1);
    }
    Task_Remove(Blaze_UpdateSpray);
    VramCache_FreeEntry(dropSlot);
    VramCache_FreeEntry(puffSlot);
  }
  FieldPsynergy_FadeFromCastColor();
  HeapSlot_Release(HEAP_SLOT_BATTLE_EFFECT);
}

/// Nonzero when the map blocks an actor at (x, z) on a layer at a height.
s32 MapGrid_CheckCollisionAtFar(s32 x, s32 z, s32 layer, s32 height);

/// Says whether an actor stands where it may not: 1 when, outside the World
/// Map, MapGrid_CheckCollisionAtFar reports its cell, ORed with 2 when it
/// overlaps any other solid actor (each radius less 2 pixels).
s32 Actor_CheckBlocked(struct Actor *actor) {
  s32 result;
  struct Actor *other;
  s32 i;

  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  result = 0;
  if (work->mode != ACTOR_SYSTEM_WORLD_MAP) {
    if (MapGrid_CheckCollisionAtFar(actor->x.fixed, actor->z.fixed, 0,
                                    actor->groundHeight) != 0)
      result = 1;
  }
  other = Ram_HeapSlots->actors;
  for (i = 0; i < ACTOR_COUNT; i++, other++) {
    if (other->script != NULL && (other->collisionFlags & ACTOR_SOLID) &&
        other != actor) {
      if (Field_CheckSpheresOverlapFar(&actor->x.fixed, actor->radius - 2,
                                       &other->x.fixed, other->radius - 2) >= 0)
        result |= 2;
    }
  }
  return result;
}

#undef BLAZE_SPRAY_FRAMES
#undef BLAZE_DROP_DEPTH
