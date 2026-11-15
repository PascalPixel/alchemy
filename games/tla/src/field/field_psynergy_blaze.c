/// @file
/// Blaze, and the check whether an actor is blocked.
// Part of field_psynergy.c.

#include "field_psynergy_internal.h"
#include "fixed_math.h"
#include "sounds.h"
#include "system/input.h"
#include "system/iwram_call.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "system/heap_release.h"

struct ObjectMetadata {
  u8 unk00[8];
  s8 height;
};

extern const s32 Data_080f0f90[];
extern char ResourceId_RedFlameBurst, ResourceId_VioletFlameBurst,
    ResourceId_YellowFlameBurst, ResourceId_BlueFlameBurst,
    ResourceId_DustPuffA;

void Field_StartColorTransition(s32 frames, s32 color);
/* VIEW(psynergy): routine Resource_GetMetadataRecordFarFar as struct
 * ObjectMetadata *Resource_GetMetadataRecordFarFar(s32 id);
 * system/sprite_metadata.h has struct AnimationMetadata
 * *Resource_GetMetadataRecordFarFar(u32 number); tried as the rest of the
 * object declares it, FieldPsynergy_Blaze moves: 4 of 609 instructions in
 * tla-ja. */
struct ObjectMetadata *Resource_GetMetadataRecordFarFar__psynergy(
    s32 id) __asm__("Resource_GetMetadataRecordFarFar");
#define Resource_GetMetadataRecordFarFar                                       \
  Resource_GetMetadataRecordFarFar__psynergy

#define OBJECT_DEFINITION(object)                                              \
  (((struct ObjectAnimation *)(object)->sprite)->definition)

/// Blaze: a jet of flame sprays from the caster, with smoke, for up to 80
/// frames or until B is pressed: red, or violet, yellow or blue to match the
/// torch (sprite 418) it is cast at. Each actor it reaches runs its Blaze
/// event, and one that is lit by it ends the jet, its actor id kept in the
/// field work. Cast at nothing, the nearest torch in reach is the target.
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
  s32 height;
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
  if (Actor_GetSpriteIdFar(object) == 418) {
    kind = ((u8 *)OBJECT_DEFINITION(object))[5];
    if (kind == 4)
      look = 1;
    if (kind == 1)
      look = 2;
    if (kind == 3)
      look = 3;
  }
  Field_StartColorTransition(8, Data_080f0f90[look]);
  work->mode = 0;
  if (scene->enabled != 0) {
    work->origin[0] = scene->x;
    work->origin[1] = scene->y + 0x100000;
    work->origin[2] = scene->z;
    best = 9999;
    for (i = 0; i < 80; i++) {
      cursor.object = event->actors[i];
      id = OBJECT_DEFINITION(cursor.object)[0];
      if (cursor.object != NULL && cursor.object->script != NULL &&
          (id == 410 || id == 304 || id == 386 || id == 418)) {
        distance = Field_MeasurePsynergyReach(scene->fieldPsynergy,
                                              scene->firstActorId, i);
        if (distance != -1 && best > distance) {
          height = Resource_GetMetadataRecordFarFar(
                       OBJECT_DEFINITION(cursor.object)[0])
                       ->height
                   << 16;
          work->mode = 1;
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
    if (event->unkd38 == 0)
      work->mode = 2;
    else
      work->mode = 3;
    if (scene->child != NULL)
      height =
          Resource_GetMetadataRecordFarFar(OBJECT_DEFINITION(scene->child)[0])
              ->height
          << 16;
    else
      height = 0x140000;
    work->origin[0] = scene->x;
    work->origin[1] = scene->y + height;
    work->origin[2] = scene->z;
  }

  mode = work->mode;
  if (mode == 1) {
    pos[0] = work->origin[0];
    pos[1] = work->origin[1];
    pos[2] = work->origin[2];
    object = Actor_Spawn(366, pos[0], pos[1], pos[2]);
    if (object == NULL)
      goto end;
    ((u8 *)object)[85] = 0;
    object->scaleX = Q16_ONE * 5 / 4;
    object->scaleY = Q16_ONE * 5 / 4;
    Actor_SelectAnimationFar(object, 2);
    Sound_PlayFar(SE_SOL_SANCTUM_LIGHT_UP);
    wait(50);
    Actor_ReleaseFar(object);
    wait(10);
    goto end;
  }
  if (mode == 0) {
    pos[0] = work->origin[0];
    pos[1] = work->origin[1];
    pos[2] = work->origin[2];
    object = Actor_Spawn(367, pos[0], pos[1], pos[2]);
    if (object == NULL)
      goto end;
    ((u8 *)object)[85] = mode;
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
    resourceId = (s32)&ResourceId_RedFlameBurst;
    break;
  case 1:
    resourceId = (s32)&ResourceId_VioletFlameBurst;
    break;
  case 2:
    resourceId = (s32)&ResourceId_YellowFlameBurst;
    break;
  case 3:
    resourceId = (s32)&ResourceId_BlueFlameBurst;
    break;
  default:
    resourceId = (s32)&ResourceId_RedFlameBurst;
    break;
  }
  Decompress_TaggedLz(Resource_GetTableEntry(resourceId), work);
  dropSlot = VramCache_FindFreeEntry();
  tile = VramCache_Load(dropSlot, 0x1000, work);
  work->dropTile = tile;
  cursor.particle = work->dropMotion;
  for (j = 0; j < 16; j++) {
    sprite = &work->drops[j];

    EffectSprite_Init(sprite, 15, 24, 0x80000000, tile);
    sprite->depth = BattleEffect_GetObjectDepth(caster) + 10;
    sprite->oam.priority = BattleEffect_GetObjectPriority(caster);
    sprite->oam.palette256 = 1;
    sprite->oam.palette = 0;
    cursor.particle->timer = -1;
    cursor.particle++;
  }
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_DustPuffA), work);
  puffSlot = VramCache_FindFreeEntry();
  tile = VramCache_Load(puffSlot, 0x800, work);
  work->puffTile = tile;
  cursor.particle = work->puffMotion;
  for (j = 0; j < 16; j++) {
    sprite = &work->puffs[j];

    EffectSprite_Init(sprite, 8, 15, 0x40000000, tile);
    sprite->scaleY = sprite->scaleX = Q16_ONE * 3 / 4;
    sprite->depth = 250;
    sprite->oam.priority = 0;
    sprite->oam.palette256 = 1;
    sprite->oam.palette = 0;
    cursor.particle->timer = -1;
    cursor.particle++;
  }
  wait(1);
  work->emitting = 1;
  Task_Add(Blaze_UpdateSpray, 0x480);
  for (k = 0; k < 80; k++) {
    wait(1);
    if (work->hit != 0) {
      pending = Scene_FindPsynergyTrigger(0x60000005, scene->fieldPsynergy,
                                          (s16)(work->hitObject | 0x100));
      if (pending != NULL)
        Field_RunPsynergyEvent(pending, scene->firstActorId, work->hitObject);
      if (Scene_FindPsynergyTrigger(0x20000005, scene->fieldPsynergy,
                                    (s16)(work->hitObject | 0x100)) != NULL) {
        event->unkd30 = work->hitObject;
        break;
      }
    }
    if (gInput.pressed & KEY_B)
      break;
  }
  Sound_PlayFar(SOUND_FADE_OUT_SE5);
  work->emitting = 0;
  for (k = 0; k < 60; k++) {
    cursor.particle = work->dropMotion;
    for (j = 0; j < 16 && cursor.particle->timer == -1; j++, cursor.particle++)
      ;
    if (j == 16)
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
    if (other->script != NULL && (other->collisionFlags & 1) &&
        other != object) {
      if (Field_CheckSpheresOverlapFar(&object->x.fixed, object->radius - 2,
                                       &other->x.fixed, other->radius - 2) >= 0)
        result |= 2;
    }
  }
  return result;
}
