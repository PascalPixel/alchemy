/// @file
/// Helpers the field Psynergy kinds share: map cells, spawned objects and
/// particles, and the field's tint.
// Part of event_commands.c.

#include "event_commands_internal.h"
#include "field/event_commands.h"
#include "field/field.h"
#include "field/field_effect.h"
#include "field/object.h"
#include "macros.h"

/// If the scene's sprite is active, stops its update callback
/// (BattleEffect_DrawSceneSprite) and frees its VRAM cache entry.
void FieldPsynergy_ReleaseSceneSprite(void) {
  struct FieldEffectWork *effectWork = Ram_HeapSlots->fieldEffectWork;

  if (effectWork->spriteActive != 0) {
    effectWork->spriteActive = 0;
    Task_Remove(BattleEffect_DrawSceneSprite);
    VramCache_FreeEntry(effectWork->spriteVramSlot);
  }
}

/// Returns the map cell at position x, z in the given layer, read from the
/// world map's blocks or the cell buffer by the event work's mode.
struct MapCell *MapGrid_GetCellAt(s32 x, s32 z, s32 layer) {
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct MapCell *cells;

  if (event->mode == ACTOR_SYSTEM_WORLD_MAP) {
    cells = &((struct MapCell *)Ram_MapBlocks)[((x / toFIXED(32)) & 31) +
                                               (((z / toFIXED(32)) & 31) << 5)];
  } else {
    if (layer <= 2)
      cells = map->layers[layer].cells;
    else
      cells = (struct MapCell *)Ram_MapCellBuffer;
    cells = &cells[x / toFIXED(16) + (z / toFIXED(16)) * 128];
  }
  return cells;
}

/// Whether the Scoop can dig at (x, z): on the world map, sand; elsewhere not
/// on a Cyclone cell, water or raised ground, and then on sand, or where
/// attribute bit 1 is set (clear, when FieldWork.sceneAttributeLow is set).
s32 MapGrid_CanScoop(s32 x, s32 z, s32 layer) {
  struct FieldWork *event = Ram_HeapSlots->fieldWork;

  if (event->mode == ACTOR_SYSTEM_WORLD_MAP)
    return MapGrid_IsSandCell(x, z, layer);
  if (MapGrid_GetCycloneCellKind(x, z, layer) != 0 ||
      MapGrid_IsWaterCell(x, z, layer) != 0 ||
      MapGrid_GetCellAttributeLowNibbleFar(layer, x, z) != 0)
    return 0;
  if (MapGrid_IsSandCell(x, z, layer) != 0)
    return 1;
  if (event->sceneAttributeLow == 0)
    return MapGrid_GetCellAt(x, z, layer)->attributeB & 1;
  return ~MapGrid_GetCellAt(x, z, layer)->attributeB & 1;
}

/// Whether Cloak can hide the leader on the cell at (x, z): the overhead
/// layer's collision code is CELL_BEHAVIOR_CLOAK, whatever layer is passed.
s32 MapGrid_IsCloakCell(s32 x, s32 z, s32 layer) {
  return MapGrid_GetCellAt(x, z, MAPGRID_LAYER_OVERHEAD)->collisionCode ==
         CELL_BEHAVIOR_CLOAK;
}

/// Whether Hover holds up over the cell at (x, z): the overhead layer's
/// collision code is CELL_BEHAVIOR_HOVER, whatever layer is passed.
s32 MapGrid_IsHoverCell(s32 x, s32 z, s32 layer) {
  return MapGrid_GetCellAt(x, z, MAPGRID_LAYER_OVERHEAD)->collisionCode ==
         CELL_BEHAVIOR_HOVER;
}

/// Whether the leader can move as sand on the cell at (x, z): the overhead
/// layer's CELL_BEHAVIOR_SAND, or on the world map collision
/// WORLD_MAP_COLLISION_WALKABLE_FIRST or shallow water;
/// FieldWork.sceneAttributeHigh turns the answer round.
s32 MapGrid_IsSandCell(s32 x, s32 z, s32 layer) {
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  s32 column = x;
  if (event->mode == ACTOR_SYSTEM_WORLD_MAP) {
    s32 position[3];

    SET_VECTOR(position, column, 0, z);
    column =
        (u32)(WorldMap_GetCollisionFar(position) -
              WORLD_MAP_COLLISION_WALKABLE_FIRST) <=
        WORLD_MAP_COLLISION_SHALLOW_WATER - WORLD_MAP_COLLISION_WALKABLE_FIRST;
  } else {
    s32 code =
        MapGrid_GetCellAt(column, z, MAPGRID_LAYER_OVERHEAD)->collisionCode;

    code ^= CELL_BEHAVIOR_SAND;
    column = NONZERO(code);
    column = 1 - column;
  }
  if (event->sceneAttributeHigh != 0)
    column = 1 - NONZERO(column);
  return column;
}

/// Whether the cell at (x, z) can be climbed (MAP_CELL_CLIMBABLE), as the
/// climbing control (mode 2) and the Sand's climb test read it.
u8 MapGrid_IsClimbableCell(s32 x, s32 z, s32 layer) {
  return MapGrid_GetCellAt(x, z, layer)->attributeB & MAP_CELL_CLIMBABLE;
}

/// Whether the cell at (x, z) is water: MAP_CELL_WATER, or on the world map
/// collision 2 to 4.
s32 MapGrid_IsWaterCell(s32 x, s32 z, s32 layer) {
  struct FieldWork *event = Ram_HeapSlots->fieldWork;

  if (event->mode == ACTOR_SYSTEM_WORLD_MAP) {
    s32 position[3];
    s32 collision;

    position[1] = 0;
    position[0] = x;
    position[2] = z;
    collision = WorldMap_GetCollisionFar(position);
    return (u32)(collision - 2) <= 2;
  }
  return (u8)(MapGrid_GetCellAt(x, z, layer)->attributeB & MAP_CELL_WATER);
}

/// The Cyclone's kind of the cell at (x, z), from the overhead layer's
/// collision code: 1 for CELL_BEHAVIOR_CYCLONE, 2 for
/// CELL_BEHAVIOR_CYCLONE_CARRY, 3 for CELL_BEHAVIOR_CYCLONE_LIFT, else 0.
/// FieldPsynergy_GatherNearbyCells lists them.
s32 MapGrid_GetCycloneCellKind(s32 x, s32 z, s32 layer) {
  u8 code = MapGrid_GetCellAt(x, z, MAPGRID_LAYER_OVERHEAD)->collisionCode;

  if (code == CELL_BEHAVIOR_CYCLONE)
    return 1;
  if (code == CELL_BEHAVIOR_CYCLONE_CARRY)
    return 2;
  if (code == CELL_BEHAVIOR_CYCLONE_LIFT)
    return 3;
  return 0;
}

/// Returns the behaviour (collision code) of the map cell at x, z in layer.
u8 MapGrid_GetCellBehavior(s32 x, s32 z, s32 layer) {
  return MapGrid_GetCellAt(x, z, layer)->collisionCode;
}

/// Sets the cell behavior of the map cell at x, z in layer, and returns it.
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
  struct MapCell *cell = MapGrid_GetCellAt(x, z, layer);

  flags |= bits;
  cell->attributeB = flags;
  return flags;
}

/// Clears bits in the attribute byte of the map cell at x, z in layer.
u8 MapGrid_ClearAttributeBBitsAt(s32 x, s32 z, s32 layer, s32 bits) {
  s32 flags = MapGrid_GetAttributeBAt(x, z, layer);
  struct MapCell *cell = MapGrid_GetCellAt(x, z, layer);

  flags &= ~bits;
  cell->attributeB = flags;
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

  if (dx > toFIXED(64) || dz > toFIXED(64))
    return -1;
  if (dy > INTtoFIXED(radius))
    return -1;
  if (dx * dx + dz * dz < radius * radius)
    return 1;
  return -1;
}

/// Moves the object from 16 pixels above the effect's main object to (x, y,
/// z), growing it from 0.25 to 1.
void FieldPsynergy_GrowFromCaster(struct Actor *object, s32 x, s32 y, s32 z) {
  struct FieldEffectWork *scene = Ram_HeapSlots->fieldEffectWork;
  struct Actor *mainActor = scene->mainActor;
  FIXED start[3];
  s32 i = 0;

  if (scene->angle != TRIG_TURN * 3 / 4)
    Actor_SetSpritePriorityDirectFar(object, 0);
  start[0] = mainActor->x.fixed;
  start[1] = mainActor->y.fixed + toFIXED(16);
  start[2] = mainActor->z.fixed;
  do {
    SET_ACTOR_POSITION(object, start[0] + (x - start[0]) * i / 10,
                       start[1] + (y - start[1]) * i / 10,
                       start[2] + (z - start[2]) * i / 10);
    object->scaleY = object->scaleX =
        i * (FIXED_ONE * 3 / 4) / 10 + FIXED_ONE / 4;
    wait(1);
    i++;
  } while (i <= 10);
}

struct MapCell;

/// FieldPsynergy_GrowFromCaster backwards: moves the object from (x, y, z) to
/// 16 pixels above the effect's main object, shrinking it from 1 to 0.25.
void FieldPsynergy_ShrinkToCaster(struct Actor *object, s32 x, s32 y, s32 z) {
  struct Actor *mainActor =
      ((struct FieldEffectWork *)Ram_HeapSlots->fieldEffectWork)->mainActor;
  FIXED start[3];
  s32 i;

  i = 0;
  start[0] = mainActor->x.fixed;
  start[1] = mainActor->y.fixed + toFIXED(16);
  start[2] = mainActor->z.fixed;
  do {
    SET_ACTOR_POSITION(object, x + (start[0] - x) * i / 10,
                       y + (start[1] - y) * i / 10,
                       z + (start[2] - z) * i / 10);
    object->scaleY = object->scaleX = i * -(FIXED_ONE * 3 / 4) / 10 + FIXED_ONE;
    wait(1);
    i++;
  } while (i <= 10);
}

/// Spawns a sparkle at x, y, z that hops off in a random
/// direction 8 to 32 pixels away at 1 to 2 pixels a frame.
struct Actor *BattleEffect_SpawnScatterParticle(s32 x, s32 y, s32 z) {
  struct Actor *object = Actor_Spawn(SPRITE_SPARKLE, x, y, z);
  s32 magnitude;

  if (object != NULL) {
    Actor_StartScriptFar(object, gFieldEffect_FragmentScript);
    object->speedLimit = Random16() + FIXED_ONE;
    object->acceleration = FIXED_ONE;
    object->motionFlags = ACTOR_FALLS;
    object->gravity = toFIXED(1) / 25;
    magnitude = Random16();
    magnitude -= Random16();
    object->velocityY = magnitude;
    Actor_SetTargetPositionFromMagnitudeAngle(
        object, Random16() * 24 + toFIXED(8), Random16());
  }
  return object;
}

/// Counts the free blocks of the sprite VRAM cache.
s32 ResourceTable_CountFreeBlocks(void) {
  u8 *marker = gVramCacheBlockOwners;
  s32 freeCount;
  s32 remaining;

  freeCount = 0;
  remaining = VRAM_BLOCK_COUNT;
  do {
    if (*marker++ == VRAM_BLOCK_OWNER_FREE) {
      freeCount++;
    }
    remaining--;
  } while (remaining != 0);
  return freeCount;
}

/// Makes sourceSprite share sharedSprite's VRAM block, releasing its own; with
/// no sharedSprite it keeps its block.
void *Sprite_ShareVramCacheEntry(void *sourceSprite, void *sharedSprite) {
  struct Sprite *shared;
  struct Sprite *sprite;

  sprite = sourceSprite;
  shared = NULL;
  if (sprite != NULL) {
    if (sharedSprite == NULL) {
      sprite->retainCacheEntry = 1;
    } else {
      VramCache_FreeEntry(sprite->slot);
      sprite->slot = ((struct Sprite *)sharedSprite)->slot;
      sprite->retainCacheEntry = 1;
      sprite = sharedSprite;
    }
    shared = sprite;
  }
  return shared;
}

#define Actor_SetAnimationSpeedFar                                             \
  ((void (*)(struct Actor *, s32))Actor_SetAnimationSpeedFar)

/// Creates an actor for a field Psynergy, at the caster's ground height, with
/// sprite priority 0 and animation 1.
struct Actor *Actor_Spawn(s32 kind, s32 x, s32 y, s32 z) {
  struct FieldEffectWork *scene = Ram_HeapSlots->fieldEffectWork;
  struct Actor *object;

  object = Actor_CreateFar(kind, x, y, z);
  if (object != NULL) {
    if (object->spriteStorageKind == ACTOR_SPRITE_NONE) {
      Actor_ReleaseFar(object);
      return NULL;
    }
    object->groundHeight = scene->mainActor->groundHeight;
    object->motionFlags = ACTOR_FLOATS;
    object->priorityFlags = ACTOR_PRIORITY_OVERHEAD;
    ((struct Sprite *)object->sprite)->image.priority = 0;
    Actor_SetSpriteFlagsFar(object, 0);
    Actor_SelectAnimationFar(object, 1);
  }
  return object;
}

/// Sets the movement state and animation timer steps of every actor but the
/// player's and except.
void Actor_SetOthersPaused(struct Actor *excludedObject, s32 groupMode,
                           s32 action) {
  s16 *activeObjectId;
  struct Actor *object;
  s32 objectId;

  objectId = 0;
  activeObjectId = &gMainRoutineWork.haltedActor;
  do {
    object = Actor_Lookup(objectId);
    if (objectId != *activeObjectId && object != NULL &&
        object != excludedObject) {
      object->paused = groupMode;
      Actor_SetAnimationSpeedFar(object, action);
    }
    objectId++;
    // BUG: one past the actor table: Actor_Lookup(FIELD_ACTOR_COUNT) reads the
    // step trigger and push cell requests as an actor.
#ifdef BUGFIX
  } while (objectId < FIELD_ACTOR_COUNT);
#else
  } while (objectId <= FIELD_ACTOR_COUNT);
#endif
}

void PaletteFade_SetTarget(s32 value, s32 mode);

/// Fades the palettes to color over frames frames, keeping the field's colours
/// for Field_StartPaletteTransition; FLAG_PSYNERGY_TINTED is set while it
/// holds.
void Field_StartColorTransition(s32 frames, s32 color) {
  struct FieldWork *field = Ram_HeapSlots->fieldWork;
  struct PaletteFade *work = Ram_HeapSlots->paletteFade;

  Dma_Set(&work->target[672], &field->savedFadeTarget[672], DMA_COPY_WORDS(336),
          REG_DMA3);
  if (field->revealActive == 0)
    Dma_Set(work->target, field->savedFadeTarget, DMA_COPY_WORDS(336),
            REG_DMA3);
  Dma_Set(work->target, work->current, DMA_COPY_WORDS(sizeof work->current / 4),
          REG_DMA3);
  PaletteFade_SetTarget(PALETTE_TINT_CUSTOM | color, 1);
  GameFlag_Set(FLAG_PSYNERGY_TINTED);
  PaletteFade_StartInterpolation(frames);
}

/// Fades the palettes back from Field_StartColorTransition's colour over frames
/// frames, the BG half to grey while Reveal is active; clears
/// FLAG_PSYNERGY_TINTED.
void Field_StartPaletteTransition(s32 frames) {
  struct FieldWork *field = Ram_HeapSlots->fieldWork;
  s32 savedPalettes = (s32)field->savedFadeTarget;

  PaletteFade_SetTarget(savedPalettes, 2);
  if (field->revealActive != 0)
    PaletteFade_SetTarget(PALETTE_TINT_GRAYSCALE, 1);
  else
    PaletteFade_SetTarget(savedPalettes, 1);
  GameFlag_Clear(FLAG_PSYNERGY_TINTED);
  PaletteFade_StartInterpolation(frames);
}

/// Tints the field for a Psynergy animation: a tint chosen by the frame
/// counter (so each cast looks a little different), fixed for some
/// animations, and forced by the tint flags; then fades towards it over eight
/// frames. The palettes are copied by Field_StartColorTransition.
void FieldPsynergy_FadeToCastColor(void) {
  struct FieldEffectWork *scene = Ram_HeapSlots->fieldEffectWork;
  s32 tint;
  s32 animation;

  tint = gFrameCount & 7;
  animation = scene->fieldPsynergy;
  if (animation == FIELD_PSYNERGY_POUND)
    tint = 0;
  if (animation == FIELD_PSYNERGY_BLAZE)
    tint = 2;
  if (animation == FIELD_PSYNERGY_SAND)
    tint = 7;
  if (animation == FIELD_PSYNERGY_CYCLONE)
    tint = 0;
  if (animation == FIELD_PSYNERGY_SCOOP)
    tint = 5;
  if (animation == FIELD_PSYNERGY_ARROW)
    tint = 0;
  if (animation == FIELD_PSYNERGY_PARCH)
    tint = 7;
  if (animation == FIELD_PSYNERGY_TREMOR)
    tint = 6;
  if (animation == FIELD_PSYNERGY_GRIND)
    tint = 0;
  if (animation == FIELD_PSYNERGY_BURST)
    tint = 7;
  if (scene->fieldPsynergy == FIELD_PSYNERGY_TELEPORT)
    tint = 6;

  if (GameFlag_Get(FLAG_PSYNERGY_TINT(0)))
    tint = 0;
  if (GameFlag_Get(FLAG_PSYNERGY_TINT(1)))
    tint = 1;
  if (GameFlag_Get(FLAG_PSYNERGY_TINT(2)))
    tint = 2;
  if (GameFlag_Get(FLAG_PSYNERGY_TINT(3)))
    tint = 3;
  if (GameFlag_Get(FLAG_PSYNERGY_TINT(4)))
    tint = 4;
  if (GameFlag_Get(FLAG_PSYNERGY_TINT(5)))
    tint = 5;
  if (GameFlag_Get(FLAG_PSYNERGY_TINT(6)))
    tint = 6;
  if (GameFlag_Get(FLAG_PSYNERGY_TINT(7)))
    tint = 7;
  Field_StartColorTransition(8, gFieldEffect_TargetColors[tint]);
}
#undef Actor_SetAnimationSpeedFar
