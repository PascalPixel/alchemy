/// @file
/// Helpers the field Psynergy kinds share: map cells, spawned objects and
/// particles, and the field's tint.
// Part of visual_attributes.c.

#include "../visual_attributes_internal.h"
#include "module/field/event_commands.h"
#include "module/field/field.h"
#include "module/field/field_effect.h"
#include "module/field/object.h"
#include "macros.h"

/// If the scene's sprite is active, stops its update callback
/// (BattleEffect_DrawSceneSprite) and frees its VRAM cache entry.
void FieldPsynergy_ReleaseSceneSprite(void) {
  struct DepthScene *scene = Ram_HeapSlots->effectWork;

  if (scene->active != 0) {
    scene->active = 0;
    Task_Remove(BattleEffect_DrawSceneSprite);
    VramCache_FreeEntry(scene->vramSlot);
  }
}

/// Returns the map cell at position x, z in the given layer, read from the
/// world map's blocks or the cell buffer by the event work's mode.
struct MapCell *MapGrid_GetCellAt(s32 x, s32 z, s32 layer) {
  u8 *event = Ram_HeapSlots->fieldWork;
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct MapCell *cells;

  if ((s8)event[394] == 3) {
    cells = &((struct MapCell *)Ram_MapBlocks)[((x / 0x200000) & 31) +
                                               (((z / 0x200000) & 31) << 5)];
  } else {
    if (layer <= 2)
      cells = map->layers[layer].cells;
    else
      cells = (struct MapCell *)Ram_MapCellBuffer;
    cells = &cells[x / 0x100000 + (z / 0x100000) * 128];
  }
  return cells;
}

/// Whether the Scoop can dig at (x, z): on the world map, sand; elsewhere not
/// on a Cyclone cell, water or raised ground, and then on sand, or where
/// attribute bit 1 is set (clear, when the event work's byte 0xd36 is set).
s32 MapGrid_CanScoop(s32 x, s32 z, s32 layer) {
  u8 *event = Ram_HeapSlots->fieldWork;

  if (*(s8 *)(event + 394) == 3)
    return MapGrid_IsSandCell(x, z, layer);
  if (MapGrid_GetCycloneCellKind(x, z, layer) != 0 ||
      MapGrid_IsWaterCell(x, z, layer) != 0 ||
      MapGrid_GetCellAttributeLowNibbleFar(layer, x, z) != 0)
    return 0;
  if (MapGrid_IsSandCell(x, z, layer) != 0)
    return 1;
  if (*(s8 *)(event + 0xd36) == 0)
    return MapGrid_GetCellAt(x, z, layer)->attributeB & 1;
  return ~MapGrid_GetCellAt(x, z, layer)->attributeB & 1;
}

/// Whether Cloak can hide the leader on the cell at (x, z): layer 2's
/// collision code 231.
s32 MapGrid_IsCloakCell(s32 x, s32 z) {
  return MapGrid_GetCellAt(x, z, 2)->collisionCode == 231;
}

/// Whether Hover holds up over the cell at (x, z): layer 2's collision code
/// 236.
s32 MapGrid_IsHoverCell(s32 x, s32 z) {
  return MapGrid_GetCellAt(x, z, 2)->collisionCode == 236;
}

/// Whether the leader can move as sand on the cell at (x, z): layer 2's
/// collision code 232, or on the world map collision 5 or 6; the event work's
/// byte 0xd37 turns the answer round.
s32 MapGrid_IsSandCell(s32 x, s32 z, s32 layer) {
  u8 *event = Ram_HeapSlots->fieldWork;
  s32 column = x;
  if (*(s8 *)(event + 394) == 3) {
    s32 position[3];

    SET_VECTOR(position, column, 0, z);
    column = (u32)(WorldMap_GetCollisionFar(position) - 5) <= 1;
  } else {
    s32 code = MapGrid_GetCellAt(column, z, 2)->collisionCode;

    code ^= 232;
    column = (u32)(-code | code) >> 31;
    column = 1 - column;
  }
  if (*(s8 *)(event + 0xd37) != 0)
    column = 1 - ((u32)(-column | column) >> 31);
  return column;
}

/// Whether the cell at (x, z) can be climbed (attribute bit 0x40), as the
/// climbing control (mode 2) and the Sand's climb test read it.
u8 MapGrid_IsClimbableCell(s32 x, s32 z, s32 layer) {
  return MapGrid_GetCellAt(x, z, layer)->attributeB & 0x40;
}

/// Whether the cell at (x, z) is water: attribute bit 16, or on the world map
/// collision 2 to 4.
s32 MapGrid_IsWaterCell(s32 x, s32 z, s32 layer) {
  u8 *event = Ram_HeapSlots->fieldWork;

  if (*(s8 *)(event + 394) == 3) {
    s32 position[3];
    s32 collision;

    position[1] = 0;
    position[0] = x;
    position[2] = z;
    collision = WorldMap_GetCollisionFar(position);
    return (u32)(collision - 2) <= 2;
  }
  return (u8)(MapGrid_GetCellAt(x, z, layer)->attributeB & 16);
}

/// The Cyclone's kind of the cell at (x, z), from layer 2's collision code:
/// 1 for 233, 2 for 234 (a teleporter it carries the leader to), 3 for 235,
/// else 0. FieldPsynergy_GatherNearbyCells lists them.
s32 MapGrid_GetCycloneCellKind(s32 x, s32 z, s32 layer) {
  u8 code = MapGrid_GetCellAt(x, z, 2)->collisionCode;

  if (code == 233)
    return 1;
  if (code == 234)
    return 2;
  if (code == 235)
    return 3;
  return 0;
}

/// Returns the behaviour (collision code) of the map cell at x, z in layer.
u8 MapGrid_GetCellBehavior(s32 x, s32 z, s32 layer) {
  return MapGrid_GetCellAt(x, z, layer)->collisionCode;
}

/* VIEW(visual_attributes): routine MapGrid_SetCellBehavior as u8
 * MapGrid_SetCellBehavior(s32 x, s32 z, s32 layer, s32 value); field/event.h
 * has s32 MapGrid_SetCellBehavior(s32 x, s32 z, s32 layer, s32 behavior); tried
 * as the definition's u8 MapGrid_SetCellBehavior(s32 x, s32 z, s32 layer, s32
 * value), which the object already declares, it does not compile in tla-ja:
 * games/tla/include/field/event.h:159: conflicting types for
 * `MapGrid_SetCellBehavior'. */
u8 MapGrid_SetCellBehavior__visual_attributes(
    s32 x, s32 z, s32 layer, s32 value) __asm__("MapGrid_SetCellBehavior");
#define MapGrid_SetCellBehavior MapGrid_SetCellBehavior__visual_attributes
u8 MapGrid_SetCellBehavior(s32 x, s32 z, s32 layer, s32 value) {
  MapGrid_GetCellAt(x, z, layer)->collisionCode = value;
  return value;
}

/// Returns the attribute byte of the map cell at x, z in layer.
s32 MapGrid_GetAttributeBAt(s32 x, s32 z, s32 layer) {
  return MapGrid_GetCellAt(x, z, layer)->attributeB;
}

/// Sets the attribute byte of the map cell at x, z in layer.
u8 MapGrid_SetAttributeBAt(s32 x, s32 z, s32 layer, s32 value) {
  MapGrid_GetCellAt(x, z, layer)->attributeB = value;
  return value;
}

/// Sets bits in the attribute byte of the map cell at x, z in layer.
u8 MapGrid_SetAttributeBBitsAt(s32 x, s32 z, s32 layer, s32 bits) {
  s32 flags = MapGrid_GetAttributeBAt(x, z, layer);
  u8 *cell = (u8 *)MapGrid_GetCellAt(x, z, layer);

  flags |= bits;
  cell[3] = flags;
  return flags;
}

/// Clears bits in the attribute byte of the map cell at x, z in layer.
u8 MapGrid_ClearAttributeBBitsAt(s32 x, s32 z, s32 layer, s32 bits) {
  s32 flags = MapGrid_GetAttributeBAt(x, z, layer);
  u8 *cell = (u8 *)MapGrid_GetCellAt(x, z, layer);

  flags &= ~bits;
  cell[3] = flags;
  return flags;
}

/// Returns 1 when two upright cylinders overlap: their centres closer on the
/// ground than the sum of radius_a and radius_b, and their heights within it;
/// else -1.
s32 FieldPsynergy_CheckOverlap(s32 *a, s32 radius_a, s32 *b, s32 radius_b) {
  s32 dx = (*a++ - *b++) >> 16;
  s32 dy = (*a++ - *b++) >> 16;
  s32 dz = (*a - *b) >> 16;
  s32 radius = radius_a + radius_b;

  if (dx > 0x400000 || dz > 0x400000)
    return -1;
  if (dy > radius << 16)
    return -1;
  if (dx * dx + dz * dz < radius * radius)
    return 1;
  return -1;
}

/// Moves the object from 16 pixels above the effect's main object to (x, y,
/// z), growing it from 0.25 to 1.
void FieldPsynergy_GrowFromCaster(struct Actor *object, s32 x, s32 y, s32 z) {
  struct MoveScene *scene = Ram_HeapSlots->effectWork;
  struct Actor *mainObject = scene->mainObject;
  s32 start[3];
  s32 i = 0;

  if (scene->mode != 0xc000)
    Actor_SetSpritePriorityDirectFar(object, 0);
  start[0] = mainObject->x;
  start[1] = mainObject->y + 0x100000;
  start[2] = mainObject->z;
  do {
    SET_POSITION(object, start[0] + (x - start[0]) * i / 10,
                 start[1] + (y - start[1]) * i / 10,
                 start[2] + (z - start[2]) * i / 10);
    object->scaleY = object->scaleX = i * (Q16_ONE * 3 / 4) / 10 + Q16_ONE / 4;
    wait(1);
    i++;
  } while (i <= 10);
}

struct MapCell;

/// FieldPsynergy_GrowFromCaster backwards: moves the object from (x, y, z) to
/// 16 pixels above the effect's main object, shrinking it from 1 to 0.25.
void FieldPsynergy_ShrinkToCaster(struct Actor *object, s32 x, s32 y, s32 z) {
  struct Actor *mainObject =
      ((struct MoveScene *)Ram_HeapSlots->effectWork)->mainObject;
  s32 start[3];
  s32 i;

  i = 0;
  start[0] = mainObject->x;
  start[1] = mainObject->y + PIXELS(16);
  start[2] = mainObject->z;
  do {
    SET_POSITION(object, x + (start[0] - x) * i / 10,
                 y + (start[1] - y) * i / 10, z + (start[2] - z) * i / 10);
    object->scaleY = object->scaleX = i * -(Q16_ONE * 3 / 4) / 10 + Q16_ONE;
    wait(1);
    i++;
  } while (i <= 10);
}

/// Spawns a particle (kind 0x2a1) at x, y, z that hops off in a random
/// direction 8 to 32 pixels away at 1 to 2 pixels a frame.
struct Actor *BattleEffect_SpawnScatterParticle(s32 x, s32 y, s32 z) {
  struct Actor *object = Actor_Spawn(0x2a1, x, y, z);
  s32 magnitude;

  if (object != NULL) {
    Actor_StartScriptFar(object, gFieldEffect_FragmentScript);
    object->speedLimit = Random16() + Q16_ONE;
    object->acceleration = Q16_ONE;
    object->motionFlags = 2;
    object->gravity = 0xa3d;
    magnitude = Random16();
    magnitude -= Random16();
    object->velocityY = magnitude;
    Actor_SetTargetPositionFromMagnitudeAngle(object, Random16() * 24 + 0x80000,
                                              Random16());
  }
  return object;
}

struct ResourceSprite {
  u8 unk00[0x10];
  u8 vramBlock;
  u8 flags;
};

extern u8 gResourceBlockOwners[];

s32 ResourceTable_CountFreeBlocks(void) {
  u8 *marker = gResourceBlockOwners;
  s32 freeCount;
  s32 remaining;

  freeCount = 0;
  remaining = 0x200;
  do {
    if (*marker++ == 0xff) {
      freeCount++;
    }
    remaining--;
  } while (remaining != 0);
  return freeCount;
}

void *Sprite_ShareVramCacheEntry(void *src, void *alt) {
  void *ret;
  struct ResourceSprite *obj;

  obj = src;
  ret = NULL;
  if (obj != NULL) {
    if (alt == NULL) {
      *((u8 *)obj + 0x11) = (u8)(*((u8 *)obj + 0x11) | 1);
    } else {
      VramCache_FreeEntry(obj->vramBlock);
      obj->vramBlock = ((struct ResourceSprite *)alt)->vramBlock;
      *((u8 *)obj + 0x11) = (u8)(*((u8 *)obj + 0x11) | 1);
      obj = alt;
    }
    ret = obj;
  }
  return ret;
}

#define Actor_SetAnimationSpeedFar                                             \
  ((void (*)(struct Actor *, s32))Actor_SetAnimationSpeedFar)

struct Actor *Actor_Spawn(s32 kind, s32 x, s32 y, s32 z) {
  struct SpawnScene *scene = Ram_HeapSlots->effectWork;
  struct Actor *object;
  u8 *part;
  s32 mask;

  object = Actor_CreateFar(kind, x, y, z);
  if (object != NULL) {
    if (object->animationKind == 0) {
      Actor_ReleaseFar(object);
      return NULL;
    }
    object->terrainHeight = scene->mainObject->terrainHeight;
    object->motionFlags = 4;
    object->priorityFlags = 4;
    mask = ~0xc;
    part = object->sprite;
    part[9] &= mask;
    Actor_SetSpriteFlagsFar(object, 0);
    Actor_SelectAnimationFar(object, 1);
  }
  return object;
}

void Actor_SetOthersPaused(struct Actor *excludedObject, s32 groupMode,
                           s32 action) {
  s16 *activeObjectId;
  struct Actor *object;
  s32 objectId;

  objectId = 0;
  activeObjectId = (s16 *)gPlayerObjectId;
  do {
    object = ObjectTable_Get(objectId);
    if (objectId != *activeObjectId && object != NULL &&
        object != excludedObject) {
      object->movementState = groupMode;
      Actor_SetAnimationSpeedFar(object, action);
    }
    objectId++;
  } while (objectId <= 0x50);
}

void BattleEffect_ApplyColorToTargetBuffer(s32 value, s32 mode);

void Field_StartColorTransition(s32 frames, s32 color) {
  u8 *state = Ram_HeapSlots->fieldWork;
  struct BattleEffectBuffers *buffers = Ram_HeapSlots->block32;

  Dma_Set(buffers->target + 0x2a0, state + 0x762, DMA_COPY_WORDS(0x150),
          REG_DMA3);
  if (*(s8 *)(state + 0xca4) == 0)
    Dma_Set(buffers->target, state + 0x222, DMA_COPY_WORDS(0x150), REG_DMA3);
  Dma_Set(buffers->target, buffers->current, DMA_COPY_WORDS(0x2a0), REG_DMA3);
  BattleEffect_ApplyColorToTargetBuffer(0x200000 | color, 1);
  GameFlag_Set(0x108);
  PaletteFade_StartInterpolation(frames);
}

void Field_StartPaletteTransition(s32 frames) {
  u8 *state = Ram_HeapSlots->fieldWork;
  s32 colors = (s32)(state + 0x222);

  BattleEffect_ApplyColorToTargetBuffer(colors, 2);
  if (*(s8 *)(state + 0xca4) != 0)
    BattleEffect_ApplyColorToTargetBuffer(0x10001, 1);
  else
    BattleEffect_ApplyColorToTargetBuffer(colors, 1);
  GameFlag_Clear(0x108);
  PaletteFade_StartInterpolation(frames);
}

/// Flags 0x148-0x14f each force one tint, the last set one winning.
#define FLAG_TINT_FIRST 0x148

/// Tints the field for a Psynergy animation: a tint chosen by the frame
/// counter (so each cast looks a little different), fixed for some
/// animations, and forced by the tint flags; then fades towards it over eight
/// frames. The palettes are copied by Field_StartColorTransition.
// ☀️'s function of the same name also copies the palettes.
void FieldPsynergy_FadeToCastColor(void) {
  struct BattleFxScene *scene = Ram_HeapSlots->effectWork;
  s32 tint;
  s32 animation;
  s32 raw;

  tint = gFrameCount & 7;
  raw = *(u16 *)&scene->fieldKind;
  /* FAKEMATCH(barrier): raw passed through an empty asm. */
  asm("" : "+r"(raw));
  animation = scene->fieldKind;
  if (animation == 18)
    tint = 0;
  if (animation == 26)
    tint = 2;
  if (animation == 23)
    tint = 7;
  if (animation == 25)
    tint = 0;
  if (animation == 20)
    tint = 5;
  if (animation == 29)
    tint = 0;
  if (animation == 22)
    tint = 7;
  if (animation == 19)
    tint = 6;
  if (animation == 27)
    tint = 0;
  if (animation == 21)
    tint = 7;
  if ((s16)raw == 30)
    tint = 6;

  if (GameFlag_Get(FLAG_TINT_FIRST + 0))
    tint = 0;
  if (GameFlag_Get(FLAG_TINT_FIRST + 1))
    tint = 1;
  if (GameFlag_Get(FLAG_TINT_FIRST + 2))
    tint = 2;
  if (GameFlag_Get(FLAG_TINT_FIRST + 3))
    tint = 3;
  if (GameFlag_Get(FLAG_TINT_FIRST + 4))
    tint = 4;
  if (GameFlag_Get(FLAG_TINT_FIRST + 5))
    tint = 5;
  if (GameFlag_Get(FLAG_TINT_FIRST + 6))
    tint = 6;
  if (GameFlag_Get(FLAG_TINT_FIRST + 7))
    tint = 7;
  Field_StartColorTransition(8, gFieldEffect_TargetColors[tint]);
}
#undef Actor_SetAnimationSpeedFar
#undef FLAG_TINT_FIRST
