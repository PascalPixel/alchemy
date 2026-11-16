/// @file
/// Blaze, and the check whether an actor is blocked.
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

/// Blaze: a jet of flame sprays from the caster, with smoke, for up to 80
/// frames or until B is pressed: red, or violet, yellow or blue to match the
/// torch (SPRITE_MARS_TORCH_LIT) it is cast at. Each actor it reaches runs its
/// Blaze event, and one that is lit by it ends the jet, its actor id kept in
/// the field work. Cast at nothing, the nearest torch in reach is the target.
void FieldPsynergy_Blaze(void) {
  struct BlazeWork *work;
  struct FieldWork *event;
  struct FieldEffectWork *scene;
  struct Actor *caster;
  struct Actor *object;
  struct EffectSprite *sprite;
  union {
    struct Actor *object;
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
  look = 0;
  object = scene->child;
  if (Actor_GetSpriteIdFar(object) == SPRITE_MARS_TORCH_LIT) {
    kind = ACTOR_FIRST_LAYER(object)->colorMode;
    if (kind == 4)
      look = 1;
    if (kind == 1)
      look = 2;
    if (kind == 3)
      look = 3;
  }
  Field_StartColorTransition(8, gBlaze_TintColors[look]);
  work->mode = BLAZE_FLICKER;
  if (scene->enabled != 0) {
    work->origin[0] = scene->x;
    work->origin[1] = scene->y + toFIXED(16);
    work->origin[2] = scene->z;
    best = 9999;
    for (i = 0; i < FIELD_ACTOR_COUNT; i++) {
      cursor.object = event->actors[i];
      id = ACTOR_FIRST_LAYER(cursor.object)->animationId;
      if (cursor.object != NULL && cursor.object->script != NULL &&
          (id == 410 || id == 304 || id == 386 || id == 418)) {
        distance = Field_MeasurePsynergyReach(scene->fieldPsynergy,
                                              scene->firstActorId, i);
        if (distance != -1 && best > distance) {
          height = toFIXED((s8)Resource_GetMetadataRecordFarFar(
                               ACTOR_FIRST_LAYER(cursor.object)->animationId)
                               ->boxX);
          work->mode = BLAZE_LIGHT_TORCH;
          work->origin[0] = cursor.object->x.fixed;
          work->origin[1] = cursor.object->y.fixed + height;
          work->origin[2] = cursor.object->z.fixed;
          scene->secondActorId = i;
          scene->child = cursor.object;
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
    object = Actor_Spawn(SPRITE_BLAZE_FLAME, pos[0], pos[1], pos[2]);
    if (object == NULL)
      goto end;
    object->motionFlags = 0;
    object->scaleX = FIXED_ONE * 5 / 4;
    object->scaleY = FIXED_ONE * 5 / 4;
    Actor_SelectAnimationFar(object, 2);
    Sound_PlayFar(SE_SOL_SANCTUM_LIGHT_UP);
    wait(50);
    Actor_ReleaseFar(object);
    wait(10);
    goto end;
  }
  if (mode == BLAZE_FLICKER) {
    pos[0] = work->origin[0];
    pos[1] = work->origin[1];
    pos[2] = work->origin[2];
    object = Actor_Spawn(SPRITE_BLAZE_FLICKER, pos[0], pos[1], pos[2]);
    if (object == NULL)
      goto end;
    object->motionFlags = mode;
    Actor_SelectAnimationFar(object, 2);
    Sound_PlayFar(SE_SOL_SANCTUM_LIGHT_UP);
    wait(45);
    Actor_ReleaseFar(object);
    wait(10);
    goto end;
  }

  Sound_PlayFar(SE_RUSH);
  BattleEffect_SetObjectUpdateMode(scene->mainActor, 0);
  work->angle = scene->angle8;
  work->targetAngle = scene->angle8;
  switch (look) {
  case 0:
    resourceId = RESOURCE_RED_FLAME_BURST;
    break;
  case 1:
    resourceId = RESOURCE_VIOLET_FLAME_BURST;
    break;
  case 2:
    resourceId = RESOURCE_YELLOW_FLAME_BURST;
    break;
  case 3:
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
    sprite->depth = BattleEffect_GetObjectDepth(caster) + 10;
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
    sprite->depth = 250;
    sprite->oam.priority = 0;
    sprite->oam.fullColor = 1;
    sprite->oam.palette = 0;
    cursor.particle->timer = -1;
    cursor.particle++;
  }
  wait(1);
  work->emitting = 1;
  Task_Add(Blaze_UpdateSpray, TASK_PASS_VBLANK);
  for (k = 0; k < FIELD_ACTOR_COUNT; k++) {
    wait(1);
    if (work->hit != 0) {
      pending = Scene_FindPsynergyTrigger(PSYNERGY_TRIGGER(PSYNERGY_PHASE_HITS),
                                          scene->fieldPsynergy,
                                          (s16)(work->hitObject | 0x100));
      if (pending != NULL)
        Field_RunPsynergyEvent(pending, scene->firstActorId, work->hitObject);
      if (Scene_FindPsynergyTrigger(PSYNERGY_TRIGGER(PSYNERGY_PHASE_AFTER_END),
                                    scene->fieldPsynergy,
                                    (s16)(work->hitObject | 0x100)) != NULL) {
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
end:
  FieldPsynergy_FadeFromCastColor();
  HeapSlot_Release(HEAP_SLOT_BATTLE_EFFECT);
}

// Whether a script object stands where it may not.

/// The script objects (Ram_HeapSlots->actors).
#define SCRIPT_OBJECT_COUNT 64

/// The event work's mode in which the cell test is skipped.
#define EVENT_MODE_NO_CELL_TEST 3

s32 MapGrid_CheckCollisionAtFar(s32 x, s32 z, s32 layer, s32 height);

/// Says whether object is blocked: 1 when, outside event mode 3,
/// MapGrid_CheckCollisionAtFar reports its cell, ORed with 2 when it overlaps
/// any other collidable script object (each radius less 2 pixels).
s32 Actor_CheckBlocked(struct Actor *object) {
  s32 result;
  struct Actor *other;
  s32 i;

  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  result = 0;
  if (work->mode != EVENT_MODE_NO_CELL_TEST) {
    if (MapGrid_CheckCollisionAtFar(object->x.fixed, object->z.fixed, 0,
                                    object->groundHeight) != 0)
      result = 1;
  }
  other = Ram_HeapSlots->actors;
  for (i = SCRIPT_OBJECT_COUNT - 1; i >= 0; i--, other++) {
    if (other->script != NULL && (other->collisionFlags & ACTOR_SOLID) &&
        other != object) {
      if (Field_CheckSpheresOverlapFar(&object->x.fixed, object->radius - 2,
                                       &other->x.fixed, other->radius - 2) >= 0)
        result |= 2;
    }
  }
  return result;
}
