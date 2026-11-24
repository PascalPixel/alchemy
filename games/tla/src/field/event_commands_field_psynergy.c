/// @file
/// Helpers the field Psynergy kinds share: map cells, spawned actors and
/// particles, and the field's tint.
// Part of event_commands.c.

#include "event_commands_internal.h"
#include "field/event_commands.h"
#include "field/field.h"
#include "field/field_effect.h"
#include "field/object.h"
#include "macros.h"

/// If the scene's sprite is active, stops its update callback
/// (FieldPsynergy_DrawCanvasSprite) and frees its VRAM cache entry.
void FieldPsynergy_ReleaseSceneSprite(void) {
  struct FieldEffectState *effectState = Ram_HeapSlots->fieldEffectState;

  if (effectState->spriteActive != 0) {
    effectState->spriteActive = 0;
    Task_Remove(FieldPsynergy_DrawCanvasSprite);
    VramCache_FreeEntry(effectState->spriteVramSlot);
  }
}

/// Returns the map cell at position x, z in the given layer, read from the
/// world map's blocks or the cell buffer by the event state's mode.
struct MapCell *MapGrid_GetCellAt(s32 x, s32 z, s32 layer) {
  struct FieldState *event = Ram_HeapSlots->fieldState;
  struct MapState *map = Ram_HeapSlots->mapGrid;
  struct MapCell *cells;

  if (event->mode == ACTOR_SYSTEM_WORLD_MAP) {
    cells = &(
        (struct MapCell *)Ram_MapBlocks)[((x / toFIXED(WORLD_MAP_CELL_PIXELS)) &
                                          (WORLD_MAP_VIEW_CELLS - 1)) +
                                         ((z / toFIXED(WORLD_MAP_CELL_PIXELS)) &
                                          (WORLD_MAP_VIEW_CELLS - 1)) *
                                             WORLD_MAP_VIEW_CELLS];
  } else {
    if (layer <= MAPGRID_LAYER_OVERHEAD)
      cells = map->layers[layer].cells;
    else
      cells = (struct MapCell *)Ram_MapCellBuffer;
    cells = &cells[x / toFIXED(MAPGRID_CELL_PIXELS) +
                   (z / toFIXED(MAPGRID_CELL_PIXELS)) * MAPGRID_SIZE];
  }
  return cells;
}

/// Whether the Scoop can dig at (x, z): on the world map, sand; elsewhere not
/// on a Cyclone cell, water or raised ground, and then on sand, or on a
/// MAP_CELL_SCOOP cell (any other, where FieldState.sceneAttributeLow is set).
s32 MapGrid_CanScoop(s32 x, s32 z, s32 layer) {
  struct FieldState *event = Ram_HeapSlots->fieldState;

  if (event->mode == ACTOR_SYSTEM_WORLD_MAP)
    return MapGrid_IsSandCell(x, z, layer);
  if (MapGrid_GetCycloneCellKind(x, z, layer) != CYCLONE_CELL_NONE ||
      MapGrid_IsWaterCell(x, z, layer) != 0 ||
      MapGrid_GetTerrainKindAtFar(layer, x, z) != 0)
    return 0;
  if (MapGrid_IsSandCell(x, z, layer) != 0)
    return 1;
  if (event->sceneAttributeLow == 0)
    return MapGrid_GetCellAt(x, z, layer)->attributeB & MAP_CELL_SCOOP;
  return ~MapGrid_GetCellAt(x, z, layer)->attributeB & MAP_CELL_SCOOP;
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
/// FieldState.sceneAttributeHigh turns the answer round.
s32 MapGrid_IsSandCell(s32 x, s32 z, s32 layer) {
  struct FieldState *event = Ram_HeapSlots->fieldState;
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

/// Whether the cell at (x, z) can be climbed (MAP_CELL_CLIMBABLE): Sand starts
/// up a wall face only there (Actor_MoveAsSand) and ends its climb off one.
u8 MapGrid_IsClimbableCell(s32 x, s32 z, s32 layer) {
  return MapGrid_GetCellAt(x, z, layer)->attributeB & MAP_CELL_CLIMBABLE;
}

/// Whether the cell at (x, z) is water: MAP_CELL_WATER, or on the world map
/// collision WORLD_MAP_COLLISION_WATER_FIRST to WORLD_MAP_COLLISION_SEA_LAST.
s32 MapGrid_IsWaterCell(s32 x, s32 z, s32 layer) {
  struct FieldState *event = Ram_HeapSlots->fieldState;

  if (event->mode == ACTOR_SYSTEM_WORLD_MAP) {
    s32 position[3];
    s32 collision;

    position[1] = 0;
    position[0] = x;
    position[2] = z;
    collision = WorldMap_GetCollisionFar(position);
    return (u32)(collision - WORLD_MAP_COLLISION_WATER_FIRST) <=
           WORLD_MAP_COLLISION_SEA_LAST - WORLD_MAP_COLLISION_WATER_FIRST;
  }
  return (u8)(MapGrid_GetCellAt(x, z, layer)->attributeB & MAP_CELL_WATER);
}

/// The Cyclone's kind of the cell at (x, z), from the overhead layer's
/// collision code (enum CycloneCellKind).
/// FieldPsynergy_GatherNearbyCells lists them.
s32 MapGrid_GetCycloneCellKind(s32 x, s32 z, s32 layer) {
  u8 code = MapGrid_GetCellAt(x, z, MAPGRID_LAYER_OVERHEAD)->collisionCode;

  if (code == CELL_BEHAVIOR_CYCLONE)
    return CYCLONE_CELL_BURST;
  if (code == CELL_BEHAVIOR_CYCLONE_CARRY)
    return CYCLONE_CELL_CARRY;
  if (code == CELL_BEHAVIOR_CYCLONE_LIFT)
    return CYCLONE_CELL_LIFT;
  return CYCLONE_CELL_NONE;
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
s32 MapGrid_GetCellAttributesAt(s32 x, s32 z, s32 layer) {
  return MapGrid_GetCellAt(x, z, layer)->attributeB;
}

/// Sets the attribute byte of the map cell at x, z in layer.
u8 MapGrid_SetCellAttributesAt(s32 x, s32 z, s32 layer, s32 value) {
  MapGrid_GetCellAt(x, z, layer)->attributeB = value;
  return value;
}

/// Sets bits in the attribute byte of the map cell at x, z in layer.
u8 MapGrid_SetCellAttributeBitsAt(s32 x, s32 z, s32 layer, s32 bits) {
  s32 flags = MapGrid_GetCellAttributesAt(x, z, layer);
  struct MapCell *cell = MapGrid_GetCellAt(x, z, layer);

  flags |= bits;
  cell->attributeB = flags;
  return flags;
}

/// Clears bits in the attribute byte of the map cell at x, z in layer.
u8 MapGrid_ClearAttributeBBitsAt(s32 x, s32 z, s32 layer, s32 bits) {
  s32 flags = MapGrid_GetCellAttributesAt(x, z, layer);
  struct MapCell *cell = MapGrid_GetCellAt(x, z, layer);

  flags &= ~bits;
  cell->attributeB = flags;
  return flags;
}

/// Returns 1 when two upright cylinders overlap, their centres closer on the
/// ground than the sum of radiusA and radiusB; else -1.
s32 FieldPsynergy_CheckOverlap(s32 *a, s32 radiusA, s32 *b, s32 radiusB) {
  s32 dx = FIXEDtoINT(*a++ - *b++);
  s32 dy = FIXEDtoINT(*a++ - *b++);
  s32 dz = FIXEDtoINT(*a - *b);
  s32 radius = radiusA + radiusB;

  // The box and height bounds are 16.16 against whole-pixel differences, so
  // on a map they never reject a pair and the ground circle alone decides.
  if (dx > toFIXED(64) || dz > toFIXED(64))
    return -1;
  if (dy > INTtoFIXED(radius))
    return -1;
  if (dx * dx + dz * dz < radius * radius)
    return 1;
  return -1;
}

/// Moves the actor from 16 pixels above the caster to (x, y, z), growing it
/// from a quarter to full size.
void FieldPsynergy_GrowFromCaster(struct Actor *actor, s32 x, s32 y, s32 z) {
  struct FieldEffectState *scene = Ram_HeapSlots->fieldEffectState;
  struct Actor *mainActor = scene->caster;
  FIXED start[3];
  s32 i = 0;

  if (scene->angle != TRIG_TURN * 3 / 4)
    Actor_SetSpritePriorityDirectFar(actor, 0);
  start[0] = mainActor->x.fixed;
  start[1] = mainActor->y.fixed + toFIXED(16);
  start[2] = mainActor->z.fixed;
  for (; i <= GROW_STEPS; i++) {
    SET_ACTOR_POSITION(actor, start[0] + (x - start[0]) * i / GROW_STEPS,
                       start[1] + (y - start[1]) * i / GROW_STEPS,
                       start[2] + (z - start[2]) * i / GROW_STEPS);
    actor->scaleY = actor->scaleX =
        i * (FIXED_ONE * 3 / 4) / GROW_STEPS + FIXED_ONE / 4;
    wait(1);
  }
}

/// FieldPsynergy_GrowFromCaster backwards: moves the actor from (x, y, z) to
/// 16 pixels above the caster, shrinking it from full size to a quarter.
void FieldPsynergy_ShrinkToCaster(struct Actor *actor, s32 x, s32 y, s32 z) {
  struct Actor *mainActor =
      ((struct FieldEffectState *)Ram_HeapSlots->fieldEffectState)->caster;
  FIXED start[3];
  s32 i;

  i = 0;
  start[0] = mainActor->x.fixed;
  start[1] = mainActor->y.fixed + toFIXED(16);
  start[2] = mainActor->z.fixed;
  for (; i <= GROW_STEPS; i++) {
    SET_ACTOR_POSITION(actor, x + (start[0] - x) * i / GROW_STEPS,
                       y + (start[1] - y) * i / GROW_STEPS,
                       z + (start[2] - z) * i / GROW_STEPS);
    actor->scaleY = actor->scaleX =
        i * -(FIXED_ONE * 3 / 4) / GROW_STEPS + FIXED_ONE;
    wait(1);
  }
}

/// Spawns a sparkle at x, y, z that hops off in a random
/// direction 8 to 32 pixels away at 1 to 2 pixels a frame.
struct Actor *BattleEffect_SpawnScatterParticle(s32 x, s32 y, s32 z) {
  struct Actor *actor = Actor_Spawn(SPRITE_SPARKLE, x, y, z);
  s32 magnitude;

  if (actor != NULL) {
    Actor_StartScriptFar(actor, gFieldEffect_FragmentScript);
    actor->speedLimit = Random16() + FIXED_ONE;
    actor->acceleration = FIXED_ONE;
    actor->motionFlags = ACTOR_FALLS;
    actor->gravity = toFIXED(1) / 25;
    magnitude = Random16();
    magnitude -= Random16();
    actor->velocityY = magnitude;
    Actor_SetTargetPositionFromMagnitudeAngle(
        actor, Random16() * 24 + toFIXED(8), Random16());
  }
  return actor;
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
  struct FieldEffectState *scene = Ram_HeapSlots->fieldEffectState;
  struct Actor *actor;

  actor = Actor_CreateFar(kind, x, y, z);
  if (actor != NULL) {
    if (actor->spriteStorageKind == ACTOR_SPRITE_NONE) {
      Actor_ReleaseFar(actor);
      return NULL;
    }
    actor->groundHeight = scene->caster->groundHeight;
    actor->motionFlags = ACTOR_FLOATS;
    actor->priorityFlags = ACTOR_PRIORITY_OVERHEAD;
    ((struct Sprite *)actor->sprite)->image.priority = 0;
    Actor_SetSpriteFlagsFar(actor, 0);
    Actor_SelectAnimationFar(actor, 1);
  }
  return actor;
}

/// Pauses or resumes every actor but the Halted one and excluded, and sets
/// their animation speed, as Sand stills the field around the leader.
void Actor_SetOthersPaused(struct Actor *excluded, s32 paused,
                           s32 animationSpeed) {
  s16 *haltedId;
  struct Actor *actor;
  s32 actorId;

  actorId = 0;
  haltedId = &gMainRoutineState.haltedActor;
  do {
    actor = Actor_Lookup(actorId);
    if (actorId != *haltedId && actor != NULL && actor != excluded) {
      actor->paused = paused;
      Actor_SetAnimationSpeedFar(actor, animationSpeed);
    }
    actorId++;
    // BUG: one past the actor table: Actor_Lookup(FIELD_ACTOR_COUNT) reads the
    // step trigger and push cell requests as an actor.
#ifdef BUGFIX
  } while (actorId < FIELD_ACTOR_COUNT);
#else
  } while (actorId <= FIELD_ACTOR_COUNT);
#endif
}

void PaletteFade_SetTarget(s32 value, s32 mode);

/// The channels of those colours, three a colour, before the OBJ colours'.
#define BG_CHANNELS (PALETTE_FADE_BG_COLORS * 3)

/// Fades the palettes to color over frames frames, keeping the field's colours
/// for Field_StartPaletteTransition; FLAG_PSYNERGY_TINTED is set while it
/// holds.
void Field_StartColorTransition(s32 frames, s32 color) {
  struct FieldState *field = Ram_HeapSlots->fieldState;
  struct PaletteFade *state = Ram_HeapSlots->paletteFade;

  // The OBJ colours are kept always, the BG colours only while Reveal has not
  // greyed them.
  Dma_Set(&state->target[BG_CHANNELS], &field->savedFadeTarget[BG_CHANNELS],
          DMA_COPY_WORDS((PALETTE_FADE_CHANNELS - BG_CHANNELS) / 2), REG_DMA3);
  if (field->revealActive == 0)
    Dma_Set(state->target, field->savedFadeTarget,
            DMA_COPY_WORDS(BG_CHANNELS / 2), REG_DMA3);
  Dma_Set(state->target, state->current,
          DMA_COPY_WORDS(sizeof state->current / 4), REG_DMA3);
  PaletteFade_SetTarget(PALETTE_TINT_CUSTOM | color, PALETTES_BG);
  GameFlag_Set(FLAG_PSYNERGY_TINTED);
  PaletteFade_StartInterpolation(frames);
}

/// Fades the palettes back from Field_StartColorTransition's colour over frames
/// frames, the BG half to grey while Reveal is active; clears
/// FLAG_PSYNERGY_TINTED.
void Field_StartPaletteTransition(s32 frames) {
  struct FieldState *state = Ram_HeapSlots->fieldState;
  s32 savedPalettes = (s32)state->savedFadeTarget;

  PaletteFade_SetTarget(savedPalettes, PALETTES_OBJECTS);
  if (state->revealActive != 0)
    PaletteFade_SetTarget(PALETTE_TINT_GRAYSCALE, PALETTES_BG);
  else
    PaletteFade_SetTarget(savedPalettes, PALETTES_BG);
  GameFlag_Clear(FLAG_PSYNERGY_TINTED);
  PaletteFade_StartInterpolation(frames);
}

/// Tints the field for a Psynergy animation: a tint chosen by the frame
/// counter (so each cast looks a little different), fixed for some
/// animations, and forced by the tint flags; then fades towards it over eight
/// frames. The palettes are copied by Field_StartColorTransition.
void FieldPsynergy_FadeToCastColor(void) {
  struct FieldEffectState *scene = Ram_HeapSlots->fieldEffectState;
  s32 tint;
  s32 animation;

  tint = gFrameCount & 7;
  animation = scene->fieldPsynergy;
  if (animation == FIELD_PSYNERGY_POUND)
    tint = PSYNERGY_TINT_BLUE;
  if (animation == FIELD_PSYNERGY_BLAZE)
    tint = PSYNERGY_TINT_RED;
  if (animation == FIELD_PSYNERGY_SAND)
    tint = PSYNERGY_TINT_GREY;
  if (animation == FIELD_PSYNERGY_CYCLONE)
    tint = PSYNERGY_TINT_BLUE;
  if (animation == FIELD_PSYNERGY_SCOOP)
    tint = PSYNERGY_TINT_YELLOW;
  if (animation == FIELD_PSYNERGY_ARROW)
    tint = PSYNERGY_TINT_BLUE;
  if (animation == FIELD_PSYNERGY_PARCH)
    tint = PSYNERGY_TINT_GREY;
  if (animation == FIELD_PSYNERGY_TREMOR)
    tint = PSYNERGY_TINT_DARK_GREY;
  if (animation == FIELD_PSYNERGY_GRIND)
    tint = PSYNERGY_TINT_BLUE;
  if (animation == FIELD_PSYNERGY_BURST)
    tint = PSYNERGY_TINT_GREY;
  if (scene->fieldPsynergy == FIELD_PSYNERGY_TELEPORT)
    tint = PSYNERGY_TINT_DARK_GREY;

  if (GameFlag_Get(FLAG_PSYNERGY_TINT(PSYNERGY_TINT_BLUE)))
    tint = PSYNERGY_TINT_BLUE;
  if (GameFlag_Get(FLAG_PSYNERGY_TINT(PSYNERGY_TINT_GREEN)))
    tint = PSYNERGY_TINT_GREEN;
  if (GameFlag_Get(FLAG_PSYNERGY_TINT(PSYNERGY_TINT_RED)))
    tint = PSYNERGY_TINT_RED;
  if (GameFlag_Get(FLAG_PSYNERGY_TINT(PSYNERGY_TINT_CYAN)))
    tint = PSYNERGY_TINT_CYAN;
  if (GameFlag_Get(FLAG_PSYNERGY_TINT(PSYNERGY_TINT_MAGENTA)))
    tint = PSYNERGY_TINT_MAGENTA;
  if (GameFlag_Get(FLAG_PSYNERGY_TINT(PSYNERGY_TINT_YELLOW)))
    tint = PSYNERGY_TINT_YELLOW;
  if (GameFlag_Get(FLAG_PSYNERGY_TINT(PSYNERGY_TINT_DARK_GREY)))
    tint = PSYNERGY_TINT_DARK_GREY;
  if (GameFlag_Get(FLAG_PSYNERGY_TINT(PSYNERGY_TINT_GREY)))
    tint = PSYNERGY_TINT_GREY;
  Field_StartColorTransition(8, gFieldPsynergy_CastColors[tint]);
}
#undef Actor_SetAnimationSpeedFar
#undef BG_CHANNELS
