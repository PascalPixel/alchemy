/// @file
/// Animated map tiles, then actors: creating and releasing them, their
/// animations, and starting scripts.
// Part of field.c.

#include "../resource_slot_internal.h"
#include "flag_call.h"
#include "module/field/field.h"
#include "module/field/object.h"
#include "module/field/resource_slot.h"
#include "module/system/system.h"

void Animation_WaitForTrackEnd(struct AnimObject *object) {
  u8 frame = object->frame;
  struct AnimTrack *track = object->track;
  s32 i;

  if (object != 0) {
    i = 0;
    if (track->script[track->index] != 241) {
      do {
        wait(1);
        i++;
        if (i > 89)
          break;
        if (track->script[track->index] == 241)
          break;
      } while (object->frame == frame);
    }
  }
}

void SpriteDissolve_Step(u8 *object, u32 position) {
  u16 *destination;
  s32 count;
  u32 row;
  u8 *table;
  u32 highMask;
  u32 indexMask;
  u8 offsetMask;

  destination =
      (u16 *)(0x06010000 + gResourceTableEntries[object[0x10]].offset);
  count = (object[0x14] * object[0x15]) / 64;
  row = 0;

  if (row < (u32)count) {
    table = gSpriteDissolve_PixelOrder;
    highMask = 0xFF00;
    indexMask = 0x3F;
    offsetMask = 0x3E;

    for (; row < (u32)count; row++, destination += 32, position++) {
      if ((u32)(position - 0x40) <= 0x3F) {
        u8 entry;
        u16 *cell;
        u32 parity;

        entry = table[(position + row * 16) & indexMask];
        cell = (u16 *)((u8 *)destination + (entry & offsetMask));
        parity = 1;
        parity &= entry;
        if (parity != 0)
          *cell = *(u8 *)cell;
        else
          *cell &= highMask;
      }
    }
  }
}

void SpriteDissolve_Run(u8 *object) {
  u32 pos;

  pos = 0;
  do {
    SpriteDissolve_Step(object, pos);
    SpriteDissolve_Step(object, pos + 1);
    SpriteDissolve_Step(object, pos + 2);
    SpriteDissolve_Step(object, pos + 3);
    pos += 4;
    wait(1);
  } while (pos <= 0x7f);
}

void SpriteDissolve_RunMany(u8 **table, s32 cnt) {
  u8 **top;
  u8 **p;
  s32 n;
  u32 pos;
  u32 pos1;
  u32 pos2;
  u32 pos3;

  top = table;
  pos = 0;
  do {
    if (cnt > 0) {
      pos1 = pos + 1;
      p = top;
      pos2 = pos + 2;
      pos3 = pos + 3;
      n = cnt;
      do {
        SpriteDissolve_Step(*p, pos);
        SpriteDissolve_Step(*p, pos1);
        SpriteDissolve_Step(*p, pos2);
        n -= 1;
        SpriteDissolve_Step(*p++, pos3);
      } while (n != 0);
    }
    wait(1U);
    pos += 4;
  } while (pos <= 0x7FU);
}

s32 MapGrid_GetScreenRelativePosition(struct Thing *obj, s32 *out) {
  u8 *state = (u8 *)Ram_HeapSlots->mapWork;
  s32 *org = (s32 *)(state + 228);
  s32 a;
  s32 b;
  s32 x;
  s32 y;

  a = org[0] & 0xffff0000;
  b = org[1] & 0xffff0000;
  x = obj->field8 - a;
  y = obj->field16 - b;
  if ((u32)(x + 0x001fffff) <= 0x012ffffe && y > 0 && y < 0xe00000) {
    *out++ = x >> 16;
    *out = y >> 16;
    return 0;
  }
  *out++ = 0;
  *out = 0;
  return -1;
}

#include "system/task.h"

/// Sets up the map animation work for map mode: its 92-byte work and an 8 KB
/// buffer, both cleared by DMA from one zero word, and the animation callbacks
/// (mode 4 runs its own; modes 3 and 4 take ObjectSystem_UpdateCameraFixed at
/// 0x480, the others ActorSystem_PlaceSprites).
// No ☀️ twin found; ☀️'s field.c starts its map animations the same way.
void ActorSystem_Initialize(s32 mode) {
  struct MapAnimationWork *work;
  void *buffer;
  // The fill word Dma_Fill32Cell reads its zero from, one for both fills.
  volatile u32 zero;

  work = HeapSlot_AllocEwram(24, sizeof(*work));
  buffer = HeapSlot_AllocEwram(20, 0x2000);
  ActorSystem_Configure(mode);
  Dma_Fill32Cell(zero, 0, buffer, 0x2000);
  Dma_Fill32Cell(zero, 0, work, sizeof(*work));
  if (mode == 4) {
    Task_Add(Actor_UpdateAllMotion, TASK_PASS_VBLANK + 10);
  } else {
    Task_Add(Actor_UpdateAllThumb, TASK_PASS_VBLANK + 11);
    Task_Add(Actor_MoveWithCarriers, TASK_PASS_VBLANK + 10);
  }
  if (mode == 3 || mode == 4)
    Task_Add(ObjectSystem_UpdateCameraFixed, TASK_PASS_VBLANK);
  else
    Task_Add(ActorSystem_PlaceSprites, TASK_PASS_VBLANK);
  work->unk06 = 15;
  work->unk07 = 0;
}

/// Two empty functions follow; nothing in the ROM refers to either.
void ActorSystem_ReservedNoOpA(void) {}

void ActorSystem_ReservedNoOpB(void) {}

// Objects: create and release field objects, and render them each frame.

#include "field/objdisp.h"
#include "actor.h"
struct ActorSpriteView;
#define Animation_ApplyChildArgument Animation_ApplyChildArgument
struct ActorSpriteView;
#define Animation_ApplyChildValue Animation_ApplyChildValue
struct ActorSpriteView;
#define InitializeAnimationObjects InitializeAnimationObjects
struct ActorSpriteView;
#define ResourceMetadata_Register ResourceMetadata_Register
#include "actor_view.h"
#undef ResourceMetadata_Register
#undef InitializeAnimationObjects
#undef Animation_ApplyChildValue
#undef Animation_ApplyChildArgument
#include "system/ram_code.h"
#include "battle/battle_system.h"
#include "system/transform.h"
#include "macros.h"

/// Returns an unused object (one without a script) from the 64 slots, or 0
/// if none is free.
void *Actor_FindFree(void) {
  struct Actor *obj = Ram_HeapSlots->scriptObjects;
  void *ret = 0;
  s32 n = 0;

  while (n <= 63) {
    if (obj->script == 0) {
      ret = obj;
      break;
    }
    n++;
    obj++;
  }
  return ret;
}

/// Releases the object's graphics and zeroes the object to free it.
/// obj: object.
void Actor_Release(struct Actor *obj) {
  volatile u32 zero;
  volatile u32 *src;
  s32 n;
  struct ResourceObjectWork **child;

  if (obj) {
    switch (obj->animationKind & 15) {
    case 1: // One graphic.
      Sprite_Destroy(
          (struct ResourceObjectWork *)((struct ActorSpriteView *)obj->sprite));
      break;
    case 2: // Up to four graphics.
      child = (struct ResourceObjectWork **)((struct ActorSpriteView **)
                                                 obj->sprite);
      n = 3;
      do {
        struct ResourceObjectWork *res = *child++;
        if (res)
          Sprite_Destroy(res);
      } while (--n >= 0);
      break;
    }
    src = &zero;
    *src = 0;
    Dma_Set(src, obj,
            (DMA_ENABLE | DMA_32BIT | DMA_SRC_FIXED) << 16 |
                (sizeof(struct Actor) / 4),
            REG_DMA3);
  }
}

struct Actor *Actor_Create(s32 id, s32 x, s32 y, s32 z) {
  struct Actor *object;
  void *sprite;
  s32 kind;
  void **entry;
  volatile u32 zero;

  kind = id / 4096;
  id &= 0xfff;
  object = Actor_FindFree();
  if (object != NULL) {
    object->radius = 16;
    switch (kind) {
    case 0:
      sprite = Sprite_Create(id);
      if (sprite != NULL) {
        object->animationKind = 1;
        object->sprite = sprite;
        object->radius = Resource_GetMetadataRecordFar(id)->boxY >> 1;
      } else {
        object->animationKind = 0;
      }
      break;
    case 2:
      entry = (void **)((u32 *)Ram_HeapSlots->objectSystem +
                        Ram_HeapSlots->objectSystem->count++ + 2);
      object->animationKind = kind;
      object->sprite = entry;
      zero = 0;
      Dma_Set(&zero, entry, 0x85000004, REG_DMA3);
      sprite = Sprite_Create(id);
      if (sprite != NULL) {
        object->radius = Resource_GetMetadataRecordFar(id)->boxY >> 1;
        *entry = sprite;
        entry++;
      }
      sprite = Sprite_Create(id + 1);
      if (sprite != NULL)
        *entry = sprite;
      break;
    }
  }
  if (object != NULL) {
    Actor_SetPositionAndResetMotion(object, x, y, z);
    object->script = (s32 *)gActorScript_EndScript;
    object->speedLimit = 0x20000;
    object->step = 0;
    object->scaleX = 0x10000;
    object->scaleY = 0x10000;
    object->acceleration = 0x10000;
    object->motionFlags = 3;
    object->gravity = 0x10000;
    object->bounce = 0x4000;
    object->collisionFlags = 0;
    object->actionFlags = 1;
    object->facing = 0x4000;
    object->unk4c = 0;
    object->timer = x / 65536;
    object->unk66 = z / 65536;
    *(s32 *)&object->unk70[12] = 0;
    *(s32 *)&object->unk70[0] = 0;
    *(s32 *)&object->unk70[4] = 0;
    *(s32 *)&object->unk70[8] = 0;
  }
  return object;
}

/// Points an object at its script and clears its script state.
void Actor_StartScript(struct Actor *object, const s32 *script) {
  if (object != 0) {
    object->step = 0;
    object->script = script;
    object->movementState = 0;
    object->unk5c[1] = 0;
    object->unk57 = 0;
  }
}

void Actor_SelectAnimation(struct Actor *object, s32 argument) {
  struct ActorSpriteView **items;
  struct ActorSpriteView *item;
  s32 count;

  if (object != 0) {
    switch (object->animationKind & 0xf) {
    case 1:
      Animation_ApplyChildArgument(((struct ActorSpriteView *)object->sprite),
                                   argument);
      break;
    case 2:
      items = ((struct ActorSpriteView **)object->sprite);
      count = 3;
      do {
        item = *items++;
        if (item != 0)
          Animation_ApplyChildArgument(item, argument);
        count--;
      } while (count >= 0);
      break;
    }
  }
}

void Actor_SetAnimationSpeed(struct Actor *object, s32 value) {
  s32 count;
  struct ActorSpriteView *child;
  struct ActorSpriteView **children;

  if (object != 0) {
    switch (object->animationKind & 0xf) {
    case 1:
      Animation_ApplyChildValue(((struct ActorSpriteView *)object->sprite),
                                value);
      return;
    case 2:
      children = ((struct ActorSpriteView **)object->sprite);
      count = 3;
      do {
        child = *children++;
        if (child != 0)
          Animation_ApplyChildValue(child, value);
        count--;
      } while (count >= 0);
      break;
    }
  }
}

void Actor_SetAnimationAndSpeed(struct Actor *object, s32 argument, s32 value) {
  struct ActorSpriteView **items;
  struct ActorSpriteView *item;
  s32 count;

  if (object != 0) {
    switch (object->animationKind & 0xf) {
    case 1:
      Animation_ApplyChildArgument(((struct ActorSpriteView *)object->sprite),
                                   argument);
      Animation_ApplyChildValue(((struct ActorSpriteView *)object->sprite),
                                value);
      break;
    case 2:
      items = ((struct ActorSpriteView **)object->sprite);
      for (count = 3; count >= 0; count--) {
        item = *items++;
        if (item != 0) {
          Animation_ApplyChildArgument(item, argument);
          Animation_ApplyChildValue(item, value);
        }
      }
      break;
    }
  }
}

void Actor_SetSpriteRotation(struct Actor *object, u32 value) {
  if (object != 0 && (object->animationKind & 0xf) == 1)
    ((struct ActorSpriteView *)object->sprite)->value12 = value;
}

void Actor_SetFirstSpriteAnimation(struct Actor *object, s32 index) {
  struct ActorSpriteView *child;

  if (object != NULL && (object->animationKind & 0xf) == 1) {
    child = ((struct ActorSpriteView *)object->sprite);
    if (index >= 0) {
      child->entries[0]->animId = index;
      InitializeAnimationObjects(child);
    }
  }
}

void Actor_RegisterSpriteMetadata(struct Actor *object, s32 value) {
  if (object != 0 && (object->animationKind & 0xf) == 1) {
    struct ActorSpriteView *child = ((struct ActorSpriteView *)object->sprite);
    if (value >= 0)
      ResourceMetadata_Register(child, value);
  }
}

#include "system/main_routine_work.h"

/// Starts an actor on the script that approaches its watched actor
/// (SC_APPROACH_WATCHED), watching watched.
void Actor_StartApproachWatched(struct Actor *actor, struct Actor *watched) {
  Actor_StartScript(actor, gActor_ApproachWatchedScript);
  actor->watchedObject = watched;
}

/// Starts the dispatch script for gMainRoutineWork's render mode, waiting a
/// frame first in mode 9.
void Actor_StartRenderModeScript(struct Actor *object) {
  const s32 *script;
  struct MainRoutineWork *party = &gMainRoutineWork;

  switch (party->renderMode) {
  case 0:
    script = gActor_PlayerScript;
    break;
  case 1:
    script = gActor_WorldMapScript;
    break;
  case 2:
    script = gActor_KeysScript;
    break;
  case 3:
    script = gActor_RopeScript;
    break;
  case 4:
    script = gActor_TightropeScript;
    break;
  case 5:
    script = gActor_SandScript;
    break;
  case 6:
    script = gActor_SailOnMapScript;
    break;
  case 7:
    script = gActor_SailScript;
    break;
  case 8:
    script = gActor_FlyScript;
    break;
  case 9:
    wait(1);
    script = gActor_SandOnWorldMapScript;
    break;
  case 10:
    script = gActor_HoverScript;
    break;
  case 11:
    script = gActor_WalkingScript;
    break;
  case 12:
    script = gActorScript_EndScript;
    break;
  default:
    script = gActor_KeyCheckScript;
    break;
  }
  Actor_StartScript(object, script);
}

void Actor_StartIdle(struct Actor *object) {
  Actor_StartScript(object, gActorScript_EndScript);
}

void Actor_StartCamera(struct Actor *object, struct Actor *watched) {
  Actor_StartScript(object, gActor_CameraScript);
  if (watched != NULL) {
    object->acceleration = 0x8000;
    object->speedLimit = 0x40000;
    object->watchedObject = watched;
    object->timer = 0;
  }
}

void Actor_WaitForCommandEnd(struct Actor *object) {
  s32 cnt = 0;

  if (object->script != 0 &&
      ((const s32 *)object->script)[object->step] != 17) {
    do {
      wait(1);
      cnt++;
      if (cnt > 599)
        break;
    } while (object->script != 0 &&
             ((const s32 *)object->script)[object->step] != 17);
  }
}

void Actor_SetSpriteFlags(struct Actor *object, u32 value) {
  if (object != NULL && (object->animationKind & 0xf) == 1)
    ((struct ActorSpriteView *)object->sprite)->value1a = value;
}

void Actor_SetBlendMode(struct Actor *object, u32 value) {
  if (object != NULL && object->animationKind == 1)
    ((struct ActorSpriteView *)object->sprite)->stateFlags = value;
}

void Animation_SetDisplayFlag(struct Actor *object, u32 value) {
  if (object != NULL && object->animationKind == 1)
    ((struct ActorSpriteView *)object->sprite)->displayFlag = value;
}

#include "field_sprite.h"

void Actor_SetSpriteColorMode(struct Actor *object, u32 value) {
  if (object != NULL && object->animationKind == 1)
    Animation_ApplyChildValuesToRecord(
        ((struct ActorSpriteView *)object->sprite), value);
}

void Actor_SetSpritePriorityDirect(struct Actor *object, s32 priority) {
  struct FieldSprite *sprite;

  if (object == NULL || object->animationKind != 1) {
    return;
  }
  sprite = ((struct FieldSprite *)object->sprite);
  sprite->priority = priority;
  sprite->partPriority = priority;
  object->priorityFlags &= ~1;
}

#include "graphics/render_input.h"
#include "fixed_math.h"

/* VIEW(resource_slot): tag struct IconSprite as struct
 * IconSprite__resource_slot; item_icon.h has another definition; tried with the
 * definition the rest of the object has, it does not compile in tla-ja:
 * games/tla/src/field/field.c:2172: dereferencing pointer to incomplete type.
 */
struct IconSprite__resource_slot {
  u8 unk00[4];
  struct RenderOam oam;
  s32 scale;
  u8 vramSlot;
  u8 unk11[3];
  u8 width;
  u8 height;
  u8 unk16;
  s8 unk17;
  u8 unk18;
  u8 unk19;
  u8 unk1a;
  u8 unk1b;
};

/// Shows icon id on its owner's 16 by 16 sprite: Icon_DrawItemFar prepares the
/// icon in heap slot 68, whose tiles at +0x400 go to the sprite's VRAM cache
/// slot, and the sprite is reset to scale 1.0, 16-colour palette 0.
// No ☀️ twin found.
void Actor_SetItemIcon(struct IconOwner *owner, s32 id) {
  struct IconSprite__resource_slot *sprite;
  u8 *buffer;
  s32 tile;

  if (owner == 0)
    return;
  owner->unk5c = 1;
  owner->iconId = id;
  sprite = owner->sprite;
  if (sprite == 0)
    return;
  buffer = HeapSlot_Alloc(68, 0x608);
  Icon_DrawItemFar(id);
  tile = VramCache_Load(sprite->vramSlot, 128, buffer + 0x400);
  HeapSlot_Release(68);
  sprite->unk1b = 0;
  sprite->unk19 = 0;
  sprite->width = 16;
  sprite->height = 16;
  sprite->unk16 = 0;
  sprite->unk17 = 4;
  sprite->scale = Q16_ONE;
  sprite->oam.index = tile;
  sprite->oam.palette256 = 0;
  sprite->oam.palette = 0;
}

/// The sprite id an actor is drawn with (the first halfword of its sprite's
/// definition), or 0 for none.
s32 Actor_GetSpriteId(u8 *object) {
  u8 *track = *(u8 **)(object + 0x50);

  if (object != NULL && track != NULL && track != (u8 *)-0x28)
    return **(s16 **)(track + 0x28);
  return 0;
}

/// Animation_WaitForTrackEnd on an actor's sprite.
void Actor_WaitForTrackEnd(u8 *object) {
  if (object != NULL)
    Animation_WaitForTrackEnd(*(void **)(object + 0x50));
}

void ActorSystem_SuspendProjectionAndShowSprites(void) {
  u16 dispcnt;

  Task_SuspendAll(ActorSystem_PlaceSprites);
  Task_SuspendAll(ObjectSystem_UpdateCameraFixed);
  BattleEffect_ApplyColorToTargetBufferFar(0x10000, 1);
  PaletteFade_StartInterpolationFar(1);
  wait(1);
  dispcnt = REG_DISPCNT;
  REG_DISPCNT = (dispcnt & 0xf1ff) | DISPCNT_OBJ_ON;
}

void ActorSystem_ResumeProjectionAndHideLayers(void) {
  u16 dispcnt;

  Task_ResumeAll(ActorSystem_PlaceSprites);
  Task_ResumeAll(ObjectSystem_UpdateCameraFixed);
  dispcnt = REG_DISPCNT;
  REG_DISPCNT = dispcnt & 0xe1ff;
}

s32 Actor_ReturnTrue(void) { return 1; }

#include "map.h"
#include "system/iwram_call.h"
#include "field/sprite.h"
#include "field/field.h"

#define Render_ApplyProjectedPlacementAtDepth                                  \
  Render_ApplyProjectedPlacementAtDepth

#define BASE_PRIORITY(map) (((struct MapPriorityWork *)(map))->basePriority)
// The priority field (bits 15..14) of a cell's word.
#define CELL_PRIORITY(cells, x, z)                                             \
  (((cells)[(((z) >> 20) << 7) + ((x) >> 20)] << 16) >> 30)
// The layer field (bits 13..12) of a cell's word: the layer plus one.
#define CELL_LAYER(cells, x, z)                                                \
  (((cells)[(((z) >> 20) << 7) + ((x) >> 20)] << 18) >> 30)

void ActorSystem_PlaceSprites(void) {
  // Unused: gives the stack frame its 36 bytes, as The Broken Seal's.
  struct CameraPoint unusedPoints[3];
  struct SpritePlacement point;
  struct SpriteScale scale;
  s32 objectIndex;
  struct MapWork *map;
  s32 *view;
  s32 viewX;
  s32 viewZ;
  struct ObjectSystemWork *system;
  struct Actor *object;
  u32 kind;
  s32 dx;
  s32 dz;
  s32 originY;
  struct PlacedSprite *sprite;
  s32 screenTop;
  u8 *priorityFlags;
  s32 layer;
  u8 *terrainId;
  u32 *cells;
  s32 feetZ;
  s32 objectHeight;
  s32 probeX;
  s32 priority;
  s32 leftPriority;
  s32 middlePriority;
  s32 rightPriority;
  s32 height;
  s32 rise;
  u32 layerPlusOne;
  s8 *basePriority;

  map = Ram_HeapSlots->mapWork;
  view = &map->viewX;
  viewX = view[0] & 0xffff0000;
  viewZ = view[1] & 0xffff0000;
  system = Ram_HeapSlots->objectSystem;
  originY = map->origin[1];
  // The decoder's copy in a once-only block, as in
  // ObjectSystem_UpdateCameraFixed, keeps the first call's arguments after the
  // followed point's height; as a plain call, 6 of 606 instructions differ in
  // tla-en.
  do {
    Dma_Set(
        Render_DecodePictureCode,
        HeapSlot_Alloc(80, (s32)Render_DecodePictureBlockSize),
        DMA_COPY_WORDS(
            (u32)(Render_DecodePictureCopyEnd - Render_DecodePictureCode) >> 2),
        REG_DMA3);
  } while (0);
  {
    // The zero is set before the objects are read.
    u32 zero = 0;

    object = Ram_HeapSlots->scriptObjects;
    system->used = zero;
  }
  for (objectIndex = 0; objectIndex < 64; objectIndex++, object++) {
    if (object->script == NULL)
      continue;
    if (object->x != 0 || object->z != 0) {
      kind = object->animationKind & 15;
      if (kind == 0)
        continue;
      if (kind != 1)
        continue;
      if ((u16)system->suspended != 0 && !(object->unk5c[0] & 1)) {
        struct PlacedSprite *frozen = object->sprite;

        VramCache_ReleaseVram(frozen->slot);
        frozen->dirty = kind;
        continue;
      }
      sprite = object->sprite;
      dx = object->x - viewX;
      dz = object->z - viewZ;
      screenTop = dz - object->y;
      if (dx > -0x200000 && dx < 0x1100000 && screenTop > -0x200000 &&
          screenTop < 0xe00000 &&
          (priorityFlags = &object->priorityFlags, !(*priorityFlags & 16))) {
        terrainId = &object->terrainId;
        layer = *terrainId & 3;
        cells = (u32 *)map->layers[layer].cells;
        if (*priorityFlags & 1) {
          probeX = object->x - 0x70000;
          feetZ = object->z + 0x20000;
          objectHeight = object->terrainHeight >> 16;
          priority = 0;
          leftPriority = 0;
          middlePriority = 0;
          if (!(*priorityFlags & 32)) {
            /* FAKEMATCH(temporary): the base priority read through a
             * function-scope pointer, which gives the two spills at sp+12 and
             * sp+16 their order; as priority = BASE_PRIORITY(map), 8 of 606
             * instructions differ in tla-en. */
            basePriority = &BASE_PRIORITY(map);
            priority = *basePriority;
            if (MapGrid_GetTerrainHeight(layer, probeX, feetZ) ==
                object->terrainHeight) {
              leftPriority = CELL_PRIORITY(cells, probeX, feetZ);
              height =
                  MapGrid_GetTerrainHeight(layer, probeX, feetZ + 0x100000) >>
                  16;
              rise =
                  (MapGrid_GetTerrainHeight(layer, probeX, feetZ + 0x200000) >>
                   16) -
                  16;
              if (rise > height)
                height = rise;
              if (height > objectHeight)
                priority = BASE_PRIORITY(map) + 1;
            }
            probeX += 0xd0000;
            if (MapGrid_GetTerrainHeight(layer, probeX, feetZ) ==
                object->terrainHeight) {
              middlePriority = CELL_PRIORITY(cells, probeX, feetZ);
              height =
                  MapGrid_GetTerrainHeight(layer, probeX, feetZ + 0x100000) >>
                  16;
              rise =
                  (MapGrid_GetTerrainHeight(layer, probeX, feetZ + 0x200000) >>
                   16) -
                  16;
              if (rise > height)
                height = rise;
              if (height > objectHeight)
                priority = BASE_PRIORITY(map) + 1;
            }
            probeX -= 0x60000;
            rightPriority = CELL_PRIORITY(cells, probeX, feetZ);
            height =
                MapGrid_GetTerrainHeight(layer, probeX, feetZ + 0x100000) >> 16;
            rise = (MapGrid_GetTerrainHeight(layer, probeX, feetZ + 0x200000) >>
                    16) -
                   16;
            if (rise > height)
              height = rise;
            if (height > objectHeight)
              priority = BASE_PRIORITY(map) + 1;
          } else {
            if (object->terrainHeight ==
                MapGrid_GetTerrainHeight(layer, probeX, feetZ))
              leftPriority = CELL_PRIORITY(cells, probeX, feetZ);
            probeX += 0xd0000;
            if (object->terrainHeight ==
                MapGrid_GetTerrainHeight(layer, probeX, feetZ))
              middlePriority = CELL_PRIORITY(cells, probeX, feetZ);
            probeX -= 0x60000;
            rightPriority = CELL_PRIORITY(cells, probeX, feetZ);
          }
          // An odd priority is preferred, left first.
          if (leftPriority != 0 || middlePriority != 0 || rightPriority != 0) {
            priority = leftPriority;
            if (!(priority & 1)) {
              if (middlePriority != 0)
                priority = middlePriority;
              if (!(priority & 1) && rightPriority != 0)
                priority = rightPriority;
            }
          }
          if (priority != 0 && object->animationKind == 1) {
            struct PlacedSprite *placed = object->sprite;

            if ((u32)priority > 3)
              priority = 3;
            placed->priority = priority;
            placed->shadowPriority = priority;
          }
        }
        layerPlusOne = CELL_LAYER(cells, object->x, object->z);
        if (layerPlusOne != 0)
          *terrainId = layerPlusOne - 1;
        scale.x = Iwram_MulQ16(object->scaleX, sprite->scale);
        scale.y = Iwram_MulQ16(object->scaleY, sprite->scale);
        point.x = dx;
        point.height = object->y - originY;
        point.y = dz - originY;
        point.groundHeight = object->terrainHeight - originY;
        if (*priorityFlags & 2) {
          point.height += 0xfec00000;
          point.y += 0xfec00000;
          point.groundHeight += 0xfec00000;
        }
        if (*priorityFlags & 4) {
          point.height += 0x1400000;
          point.y += 0x1400000;
          point.groundHeight += 0x1400000;
        }
        if (*priorityFlags & 8)
          Render_ApplyProjectedPlacementAtDepth(sprite, &point, scale.values,
                                                object->facing,
                                                ((u8 *)object)[0x63]);
        else
          Render_ApplyProjectedPlacement(sprite, SpritePlacement_Values(&point),
                                         scale.values, object->facing);
        continue;
      }
      if (object->unk5c[0] & 2) {
        Sprite_ComposePicture(sprite, object->facing);
      } else if (!(object->unk5c[0] & 1) && !(sprite->retainCacheEntry & 1)) {
        VramCache_ReleaseVram(sprite->slot);
        sprite->dirty = 1;
      }
    } else {
      // At (0, 0): not placed; only the sprite's VRAM is given up.
      kind = object->animationKind & 15;
      if (kind == 1) {
        struct PlacedSprite *frozen = object->sprite;

        if (object->unk5c[0] == 0 && !(frozen->retainCacheEntry & 1)) {
          VramCache_ReleaseVram(frozen->slot);
          frozen->dirty = kind;
        }
      }
    }
  }
  HeapSlot_Release(80);
}

// The fixed camera: each frame it places every object's sprites, facing them
// by the angle from the eye to the target. The Lost Age places each object in
// a function of its own, the body of ☀️'s ObjectSystem_UpdateCameraFixed
// loop.

s32 ActorCamera_ReturnTrue(void) { return 1; }

/// Places an object's sprite (kind 1, unless flag 0x10 hides it) or each of its
/// group's four sprites (kind 2), turned by angle. While battle commands are
/// chosen (flag 363) the sprites are drawn at 1.0 scale.
void Actor_PlaceSprites(struct PlacedObject *object, s32 angle) {
  const s32 *scale = object->scale;
  s32 *point;
  void *sprite;
  void **sprites;
  s32 remaining;

  if (GameFlag_Get(363))
    scale = gData_0802eb90;
  switch (object->kind & 15) {
  case 1:
    point = object->position;
    sprite = object->target;
    if (!(object->flags & 0x10))
      Render_PlaceProjectedSprite(sprite, point, scale, object->angle + angle,
                                  object->layer);
    break;
  case 2:
    sprites = object->target;
    for (remaining = 3; remaining >= 0; remaining--) {
      sprite = *sprites++;
      if (sprite != NULL)
        Render_PlaceProjectedSprite(sprite, object->position, scale,
                                    object->angle + angle, object->layer);
    }
    break;
  case 0:
    break;
  }
}

/// Each frame, with the fixed camera: places every active object, from the
/// 64th down. While battle commands are chosen (flag 363) it uses the fixed
/// view matrix and turns the objects by an eighth of a turn less.
void ObjectSystem_UpdateCameraFixed(void) {
  struct FixedCamera *camera = Ram_HeapSlots->cameraWork;
  struct ObjectSystemWork *system = Ram_HeapSlots->objectSystem;
  struct CameraPoint *eye;
  struct CameraPoint *target;
  struct PlacedObject *object;
  s32 angle;
  s32 i;
  // Unused: gives the stack frame its 12 bytes.
  struct CameraPoint unused;

  // The decoder's copy in a once-only block makes the stack frame before the
  // first call's arguments; as a plain call, 6 of 93 instructions differ in
  // tla-en.
  do {
    Dma_Set(
        Render_DecodePictureCode,
        HeapSlot_Alloc(80, (s32)Render_DecodePictureBlockSize),
        DMA_COPY_WORDS(
            (u32)(Render_DecodePictureCopyEnd - Render_DecodePictureCode) >> 2),
        REG_DMA3);
  } while (0);
  eye = &camera->eye;
  target = &camera->target;
  if (camera->eyeOverride != NULL)
    eye = camera->eyeOverride;
  if (camera->targetOverride != NULL)
    target = camera->targetOverride;
  angle = (s16)ArcTan2((eye->x - target->x) >> 16, (eye->z - target->z) >> 16);
  system->used = 0;
  if (GameFlag_Get(363)) {
    Transform_Reset();
    angle += 0xffffe000;
    Iwram_TransformMatrixEntry(gCamera_FixedViewMatrix);
    Transform_ApplyLookAt((s32 *)eye, (s32 *)target);
  } else {
    SceneTransform_RewindStack();
    Transform_SetLookAt((s32 *)eye, (s32 *)target);
  }
  object = Ram_HeapSlots->scriptObjects;
  object += 63;
  for (i = 63; i >= 0; i--, object--) {
    if (object->active != 0)
      Actor_PlaceSprites(object, angle);
  }
  HeapSlot_Release(80);
}

// Object scripts: move the instruction cursor and control byte54.

#include "field/script.h"

/// Jumps to the script the next word points to and returns 1 to continue
/// with its first command.
s32 Script_JumpToNextOperandPointer(struct ScriptInterpreter *sc) {
  s32 zero;

  // Jump to the script pointed to by the following word.
  sc->script = (const s32 *)sc->script[sc->cursor + 1];
  zero = 0;
  sc->cursor = zero;
  return 1;
}

/// Clears byte54 and advances to the next command.
s32 ActorScript_HideSprites(struct ScriptInterpreter *sc) {
  sc->byte54 = 0;
  sc->cursor++;
  return 1;
}

/// Sets byte54 to 1 and advances to the next command.
s32 ActorScript_ShowSprite(struct ScriptInterpreter *sc) {
  sc->byte54 = 1;
  sc->cursor++;
  return 1;
}

/// Waits up to 600 frames for a field object's movement target to be unset.
void Actor_WaitForMove(void *fieldObject) {
  s32 waitedFrames;

  waitedFrames = 0;
  while (waitedFrames < 600 && Actor_IsTargetUnset(fieldObject) == 0) {
    wait(1);
    waitedFrames++;
  }
}
