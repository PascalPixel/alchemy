/// @file
/// Five of the side rooms of Crossbone Isle Cave, the cave beneath Crossbone
/// Isle. Each opens off one of the nine floors the cave descends through
/// (crossbone_isle.c) through two doors, east and west: the floor's east door
/// leads in at the room's `_EAST` entrance
/// (CROSSBONE_ISLE_STEPPING_STONES_FROM_CROSSBONE_ISLE_FIRST_FLOOR_EAST in the
/// first) and its west door at the `_WEST` one, and the exits of the same names
/// lead back out. Each is a puzzle with four chests, and all five take the area
/// name Crossbone Isle Cave. In the order the party meets them:
///
/// - SceneId_CrossboneIsleSteppingStones, off the first floor: stepping stones
///   over a pit; walking into a pillar jumps two cells across it where the
///   cells allow, and otherwise pushes it (EventScript_JumpOrPushPillar).
/// - SceneId_CrossboneIsleIcePillars, off the second floor: pillars of ice to
///   push, standing and lying across a floor of spikes.
/// - SceneId_CrossboneIsleFloodgate, off the fifth floor: a cave with a stream
///   and a floodgate across it, and seven logs to roll. The switch beside the
///   floodgate (EventScript_FlipWaterSwitch) closes it, so the water backs up
///   and floats the logs higher, or opens it to drain them back down; the water
///   level lasts only while the party stays
///   (FLAG_CROSSBONE_ISLE_FLOODGATE_WATER_RAISED, kept through a battle).
/// - SceneId_CrossboneIsleStackedPillars, off the seventh floor, a cave of
/// cliffs
///   and ladders: four pillars that Move and Carry shift, and that settle and
///   redraw their footprints after every push (EventScript_MovePillar), a
///   ladder, and pillars to jump across.
/// - SceneId_CrossboneIsleDragonStatue, off the eighth floor, by a dragon
/// statue:
///   three puddles that Frost freezes into pillars of ice. Frost on the third
///   makes the pillar in it rise and thaws a frozen second puddle, and the
///   third freezes only if the leader was not standing on it; a boulder moved
///   with Move sinks into place at column 16.
///
/// The other three side rooms are in crossbone_isle_keys.c, and the ninth
/// floor's in crossbone_isle.c.

#include "action_script.h"
#include "save_block.h"
#include "map_grid_scroll.h"
#include "scene_ids.h"
#include "unit/import.h"
#include "unit/scene/crossbone_isle_stepping_stones.h"
#include "unit/scene/common/effect_spawn.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/field_services.h"
#include "unit/field/field.h"
#include "unit/runtime/runtime.h"
#include "object_event.h"
#include "types.h"
#include "animspr.h"
#include "script_layer.h"
#include "map_tables.h"
#include "field_effect.h"
#include "create_configured_overlay_object.h"
#include "pushed_object.h"
#include "ram_buffer.h"
#include "map_grid.h"
#include "io_reg.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "heap_slots.h"
#include "dma.h"
#include "edition.h"
#include "text/msg_ids.h"
#include "call.h"
#include "field_service.h"
#include "irq.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"
#include "heap.h"
#include "scene_header.h"
#include "object_effect.h"
#include "fixed_math.h"
#include "flags.h"
#include "songs.h"
#include "audio_engine_symbols.h"
#include "macros.h"

/// A chest searched while facing the given way.
#define BG_EVENT_CHEST_FACING(facing)                                          \
  (BG_EVENT(BG_EVENT_CHEST) | MAP_EVENT_FLAG_FACING | (facing))

/// The placed object events, numbered in the order of each room's object event
/// templates (Scene_GetObjectEventTemplates).
enum CrossboneIsleSteppingStonesLocalId {
  /// The pillars of CrossboneIsleSteppingStones (four, SPRITE_STONE_PILLAR),
  /// CrossboneIsleIcePillars (five) and CrossboneIsleStackedPillars (four,
  /// sprite 0x100), the only object events those rooms place.
  /// ★ Named from what the player sees: tan stone pillars over the pit,
  /// pillars of ice on the spikes, and grey carved pillars stacked two high
  /// (its render)
  LOCALID_FIRST_PILLAR = LOCALID_FIRST_PLACED,
  /// The logs, SPRITE_LOG_NORTH_SOUTH and SPRITE_LOG_WEST_EAST as Kolima
  /// Forest's: eight in
  /// CrossboneIsleFloodgate, where the seven after the first float on the
  /// raised water, and two in CrossboneIsleDragonStatue.
  /// ★ Named from what the player sees: logs lying on the cave floor (its
  /// render)
  LOCALID_FIRST_LOG = LOCALID_FIRST_PLACED,
  /// CrossboneIsleDragonStatue's boulder for Move, sprite 0x125, which sinks
  /// into place at column 16.
  /// ★ Named from what the player sees: a round grey boulder below the logs
  /// (its render)
  LOCALID_SINKING_BOULDER = LOCALID_FIRST_LOG + 2,
  /// CrossboneIsleDragonStatue's three puddles, SPRITE_PUDDLE, which Frost
  /// freezes into pillars of ice.
  /// ★ Named from what the player sees: a small blue puddle on the west ledge
  /// (its render)
  LOCALID_FIRST_PUDDLE,
  /// ★ Named from what the player sees: a small blue puddle on the middle
  /// ledge (its render)
  LOCALID_SECOND_PUDDLE,
  /// ★ Named from what the player sees: the puddle by the dragon statue,
  /// drawn frozen around its pillar (its render)
  LOCALID_THIRD_PUDDLE,
  /// The pillar in CrossboneIsleDragonStatue's third puddle, sprite 0x122,
  /// which rises 32 pixels as Frost freezes the puddle.
  /// ★ Named from what the player sees: a narrow dark pillar standing in the
  /// third puddle (its render)
  LOCALID_RISING_PILLAR,
  /// CrossboneIsleFloodgate's water, sprite 0xd6, raised behind the closed
  /// floodgate.
  /// ★ Named from what the player sees: the stream's green water, which the
  /// closed floodgate backs up (its render)
  LOCALID_WATER = LOCALID_FIRST_LOG + 8
};

/// CrossboneIsleFloodgate: the water has been raised.
/// ★ Named from what the player sees: the stream's water risen behind the
/// closed floodgate, floating the logs higher (its render)
#define FLAG_CROSSBONE_ISLE_FLOODGATE_WATER_RAISED FLAG_SCENE_TEMP_00
/// CrossboneIsleStackedPillars: the first pillar has another pushed on top of
/// it, so Move and Carry leave it be; the four pillars' flags follow from this
/// one.
/// ★ Named from what the player sees: two stacks of two grey stone pillars
/// among the ladders and cliffs (its render)
#define FLAG_CROSSBONE_ISLE_STACKED_PILLARS_FIRST_PILLAR_UNDER_ANOTHER         \
  FLAG_SCENE_TEMP_00
/// CrossboneIsleDragonStatue: the first puddle is frozen; the second's and
/// third's flags follow from this one.
/// ★ Named from what the player sees: three small blue puddles on the cave
/// floor (its render)
#define FLAG_CROSSBONE_ISLE_DRAGON_STATUE_FIRST_PUDDLE_FROZEN FLAG_SCENE_TEMP_00
/// CrossboneIsleDragonStatue: the leader stood on the third puddle when Frost
/// began, so it stays unfrozen.
/// ★ Named from what the player sees: the third puddle, by the dragon statue,
/// with a narrow dark pillar standing in it (its render)
#define FLAG_CROSSBONE_ISLE_DRAGON_STATUE_LEADER_ON_THIRD_PUDDLE               \
  FLAG_SCENE_TEMP_03
/// CrossboneIsleDragonStatue: the boulder Move pushes has sunk into place at
/// column 16.
/// ★ Named from what the player sees: a round grey boulder below the two logs
/// (its render)
#define FLAG_CROSSBONE_ISLE_DRAGON_STATUE_BOULDER_SUNK FLAG_SCENE_TEMP_04

/// A copied map cell, also read in the two flag pairs the pillar redraw keeps.
union CrossboneIslePillarCell {
  struct MapGridCell cell;
  struct CrossboneIslePillarCellBits {
    u32 metatile : 12;
    u32 lowFlags : 2; ///< bits 13..12 of MapGridCell.metatileAndFlags
    u8 highFlags : 2; ///< bits 15..14, read by MapGrid_GetCellHighFlags
    u8 behavior;
    u8 shape;
  } bits;
};

STATIC_ASSERT(sizeof(union CrossboneIslePillarCell) ==
              sizeof(struct MapGridCell));
STATIC_ASSERT(offsetof(union CrossboneIslePillarCell, bits.behavior) == 2);
STATIC_ASSERT(offsetof(union CrossboneIslePillarCell, bits.shape) == 3);

#include "common/effect_create.h"
#include "common/effect_spawn.h"
#include "common/object_staged_object_event.h"

#include "common/map_grid_panel.h"

/// Waits up to sixty frames for landing, then stops vertical movement on the
/// ground.
static void CrossboneIsleSteppingStones_SettleObjectEventOnGround(
    struct ObjectEvent *objectEvent) {
  s32 remainingFrames = 60;

  for (;;) {
    if (remainingFrames == 0) {
      break;
    }
    WaitFrames(1);
    if ((u32)objectEvent->y.fixed == (u32)objectEvent->groundHeight) {
      break;
    }
    remainingFrames--;
  }

  objectEvent->velocityY = 0;
  objectEvent->targetY = OBJ_EVENT_NO_TARGET;
  objectEvent->y.fixed = objectEvent->groundHeight;
}

/// Waits until the object event reaches the ground or height limit, then stops
/// its descent.
static void CrossboneIsleSteppingStones_StopAtGroundOrHeight(
    struct ObjectEvent *objectEvent, s32 limit) {
  s32 remainingFrames = 60;

  for (;;) {
    if (remainingFrames == 0) {
      break;
    }
    WaitFrames(1);
    if (objectEvent->y.fixed <= objectEvent->groundHeight) {
      break;
    }
    if (objectEvent->y.fixed <= limit) {
      break;
    }
    remainingFrames--;
  }

  objectEvent->velocityY = 0;
  objectEvent->targetY = OBJ_EVENT_NO_TARGET;
}

/// Finds the object event's footprint dimensions, grid position, and map-layer
/// origin.
static s32 CrossboneIsleSteppingStones_QueryFootprint(
    s32 id, u32 *width, u32 *depth, struct PushedObjectProbe *probe,
    s32 *layerOriginX, s32 *layerOriginZ) {
  struct MapGridScrollWork *grid = gMapGridWork[0];
  struct ObjectEvent *objectEvent = ObjectEvent_GetById(id);
  u32 i;
  s32 coordinate;
  s32 farExtent;

  for (i = 0; i <= 5; i++) {
    if (objectEvent->sprite->entries[0]->animationId ==
        gPushedObject_FootprintKinds[i]) {
      probe->footprintIndex = i;
      break;
    }
    probe->footprintIndex = 7;
  }
  if ((u32)probe->footprintIndex > 6) {
    return 0;
  }
  // Bounds are pixel offsets; dimensions and origins below are 16-pixel cells.
  probe->position.x = objectEvent->x.fixed;
  probe->position.y = objectEvent->y.fixed;
  probe->position.z = objectEvent->z.fixed;
  coordinate = gPushedObject_FootprintBounds[probe->footprintIndex * 4 + 1];
  if (coordinate < 0) {
    coordinate = -coordinate;
  }
  farExtent = gPushedObject_FootprintBounds[probe->footprintIndex * 4 + 3];
  if (farExtent < 0) {
    farExtent = -farExtent;
  }
  *depth = (coordinate + farExtent) >> 4;
  coordinate = gPushedObject_FootprintBounds[probe->footprintIndex * 4];
  if (coordinate < 0) {
    coordinate = -coordinate;
  }
  farExtent = gPushedObject_FootprintBounds[probe->footprintIndex * 4 + 2];
  if (farExtent < 0) {
    farExtent = -farExtent;
  }
  *width = (coordinate + farExtent) >> 4;
  // Move from the object event's center to the footprint's top-left cell.
  probe->position.x +=
      gPushedObject_FootprintBounds[probe->footprintIndex * 4] * PIXELS(1);
  {
    s32 offsetZ = gPushedObject_FootprintBounds[probe->footprintIndex * 4 + 1] *
                  PIXELS(1);

    coordinate = probe->position.z;
    coordinate += offsetZ;
    coordinate >>= 20;
    probe->position.z = coordinate;
  }
  probe->position.x >>= 20;
  *layerOriginX = grid->layers[1].offsetX >> 20;
  *layerOriginZ = grid->layers[1].offsetY >> 20;
  return 1;
}

/// Draws a log's footprint where it floats on CrossboneIsleFloodgate's raised
/// water, lying across or along the room as the log does, and puts its object
/// event under the leader.
static s32 CrossboneIsleSteppingStones_DrawRaisedFootprint(s32 localId) {
  struct ObjectEvent *objectEvent = ObjectEvent_GetById(localId);
  u32 width, depth;
  s32 layerOriginX, layerOriginZ;
  struct PushedObjectProbe probe;
  s32 cellX, cellZ;
  u8 mask;

  if (CrossboneIsleSteppingStones_QueryFootprint(
          localId, &width, &depth, &probe, &layerOriginX, &layerOriginZ) == 0) {
    return 0;
  }

  cellX = probe.position.x;
  cellZ = probe.position.z;
  MapGrid_CopyCellAttributes(2, 2, width, depth, cellX, cellZ);

  Object_SelectAnimation(objectEvent, 4);
  mask = 2;
  objectEvent->priorityFlags |= mask;

  if (width > depth) {
    MapGrid_CopyCellsTo(70, 40, probe.position.x + 32, probe.position.z + 2,
                        width, depth);
  } else {
    MapGrid_CopyCellsTo(68, 40, probe.position.x + 32, probe.position.z + 2,
                        width, depth);
  }

  return 1;
}

/// Redraws CrossboneIsleFloodgate's seven logs floating on the raised water.
static inline void CrossboneIsleSteppingStones_DrawFloatingLogs(void) {
  CrossboneIsleSteppingStones_DrawRaisedFootprint(LOCALID_FIRST_LOG + 1);
  CrossboneIsleSteppingStones_DrawRaisedFootprint(LOCALID_FIRST_LOG + 2);
  CrossboneIsleSteppingStones_DrawRaisedFootprint(LOCALID_FIRST_LOG + 3);
  CrossboneIsleSteppingStones_DrawRaisedFootprint(LOCALID_FIRST_LOG + 4);
  CrossboneIsleSteppingStones_DrawRaisedFootprint(LOCALID_FIRST_LOG + 5);
  CrossboneIsleSteppingStones_DrawRaisedFootprint(LOCALID_FIRST_LOG + 6);
  CrossboneIsleSteppingStones_DrawRaisedFootprint(LOCALID_FIRST_LOG + 7);
}

/// Restores the ground metatiles and metatile behaviors under the object
/// event's footprint.
static s32 CrossboneIsleSteppingStones_RestoreGroundFootprint(s32 localId) {
  struct ObjectEvent *objectEvent = ObjectEvent_GetById(localId);
  u32 width, depth;
  s32 layerOriginX, layerOriginZ;
  struct PushedObjectProbe probe;

  if (CrossboneIsleSteppingStones_QueryFootprint(
          localId, &width, &depth, &probe, &layerOriginX, &layerOriginZ) == 0) {
    return 0;
  }

  {
    s32 sourceCellX = layerOriginX + probe.position.x;
    s32 sourceCellZ = layerOriginZ + probe.position.z;

    MapGrid_CopyCellAttributes(sourceCellX, sourceCellZ, width, depth,
                               probe.position.x, probe.position.z);
    PushedObject_FillGridAttributeRectangle(
        0, probe.position.x, probe.position.z, width, depth, MB_WALL);
  }

  Object_SelectAnimation(objectEvent, 1);
  objectEvent->priorityFlags &= ~OBJ_EVENT_PRIORITY_UNDERFOOT;

  return 1;
}

/// Reads one map cell through DMA, waiting until the transfer finishes.
static void CrossboneIsleSteppingStones_ReadMapGridCell(
    s32 layer, s32 x, s32 y, union CrossboneIslePillarCell *dst) {
  struct MapGridScrollWork *grid = gMapGridWork[0];

  if (grid != 0) {
    struct MapGridCell *cell = grid->layers[layer].cells;

    cell += x + (y << 7);
    Dma_Set(cell, dst,
            ((u32)(DMA_ENABLE | DMA_32BIT) << 16) | sizeof(*cell) / sizeof(u32),
            REG_DMA3);
    {
      volatile u32 *dma = REG_DMA3;

      while (dma[2] & ((u32)DMA_ENABLE << 16))
        ;
    }
  }
}

/// Copies the cell's flags and metatile behavior without replacing its
/// metatile.
static void CrossboneIsleSteppingStones_SetCellAttributes(
    s32 layer, s32 x, s32 y, const union CrossboneIslePillarCell *source) {
  struct MapGridScrollWork *grid = gMapGridWork[0];

  if (grid != 0) {
    // UB: the native bitfield view aliases MapGridCell storage. Byte writes
    // keep its fields separate without that alias (tbs-en: 88 to 84 bytes).
#ifdef UBFIX
    u8 *cell = (u8 *)grid->layers[layer].cells;

    cell += sizeof(struct MapGridCell) * (x + (y << 7));
    cell[1] = (source->bits.lowFlags << 4) | (cell[1] & ~0x30);
    cell[1] = (source->bits.highFlags << 6) | (cell[1] & ~0xc0);
    cell[2] = source->cell.behavior;
    cell[3] = source->cell.shape;
#else
    struct CrossboneIslePillarCellBits *cell =
        (struct CrossboneIslePillarCellBits *)grid->layers[layer].cells;

    cell += x + (y << 7);
    cell->lowFlags = source->bits.lowFlags;
    cell->highFlags = source->bits.highFlags;
    cell->behavior = source->bits.behavior;
    cell->shape = source->bits.shape;
#endif
  }
}

/// Finds the next terrain ledge and stops the falling object event at its
/// projected height.
static s32 CrossboneIsleSteppingStones_LowerObjectEventToLedge(s32 id,
                                                               s32 far) {
  struct ObjectEvent *objectEvent = ObjectEvent_GetById(id);
  s32 found;
  struct ObjectEvent *temporaryObjectEvent;
  s32 height;
  s32 level;
  s32 top;
  u32 row;
  u32 rows;
  s32 z;
  s32 y;
  s32 x;
  s32 kind;

  found = 0;
  temporaryObjectEvent = NULL;
  ObjectEvent_SetSpriteFlags(objectEvent, found);
  height =
      MapGrid_GetTerrainHeight(2, objectEvent->x.fixed, objectEvent->z.fixed);
  level = height / PIXELS(16);
  rows = level;
  if (level < 0) {
    rows = -level;
  }
  rows++;
  for (row = 0; row <= rows; row++) {
    top = MapGrid_GetTerrainHeight(objectEvent->layer, objectEvent->x.fixed,
                                   (row << 20) + objectEvent->z.fixed);
    top /= PIXELS(16);
    if (level < top) {
      x = ((objectEvent->x.fixed >> 20) << 20) + PIXELS(8);
      if (far == 0) {
        row += 2;
        z = ((row + (objectEvent->z.fixed >> 20)) << 20) + PIXELS(2);
        y = top << 20;
        kind = 223;
      } else {
        row += 3;
        z = ((row + (objectEvent->z.fixed >> 20)) << 20) - PIXELS(2);
        y = top << 20;
        kind = 253;
      }
      /* FAKEMATCH(one-pass): the block keeps found's constant after the height
       * arithmetic; as no block, 1 of 117 instructions differ in tbs-en. */
      do {
        temporaryObjectEvent = ObjectEvent_PrepareObject(x, y, z, kind);
        height = objectEvent->z.fixed - z + y;
      } while (0);
      found = 1;
      break;
    }
  }
  CrossboneIsleSteppingStones_StopAtGroundOrHeight(objectEvent, height);
  objectEvent->x.fixed = 0;
  objectEvent->y.fixed = 0;
  objectEvent->z.fixed = 0;
  if (temporaryObjectEvent != 0) {
    ObjectEvent_Release(temporaryObjectEvent);
  }
  return found;
}

/// Shrinks the pillar and lowers its height for up to thirty-two frames.
static void CrossboneIsleSteppingStones_ShrinkPillar(s32 index) {
  struct ObjectEvent *objectEvent = ObjectEvent_GetById(index);
  u32 elapsedFrames;

  objectEvent->motionFlags = 0;

  elapsedFrames = 0;
  for (;;) {
    if (elapsedFrames > 31)
      return;
    WaitFrames(1);
    objectEvent->scaleY += -Q16_ONE / 10;
    objectEvent->y.fixed += -PIXELS(4) / 5;
    elapsedFrames++;
    if (objectEvent->scaleY <= Q16_ONE / 10 - 1) {
      objectEvent->scaleY = Q16_ONE / 10;
      return;
    }
  }
}

/// Unused: spawns a particle near the leader once every four frames; nothing
/// calls it.
UNUSED static void CrossboneIsleSteppingStones_SpawnUnusedLeaderParticle(void) {

  struct ObjectEvent *leader;
  s32 cycleFrame;
  s32 x;
  s32 z;
  s32 velocityY;
  s32 drawX;
  s32 drawZ;
  s32 drawY;
  struct EffectOptions *settings;
  struct EffectOptions options;

  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  cycleFrame = gFrameCount & 3;
  if (cycleFrame == 0) {
    settings = &options;
    settings->palette = 10;
    settings->startScaleX = 7 * Q16_ONE / 10;
    settings->startScaleY = 7 * Q16_ONE / 10;
    drawX = Import_RandomNext();
    x = leader->x.fixed + ((((u32)((drawX << 4) + drawX) >> 16) - 8) << 16);
    drawZ = Import_RandomNext();
    z = leader->z.fixed + ((((u32)((drawZ << 4) + drawZ) >> 16) - 8) << 16);
    drawY = Import_RandomNext();
    velocityY = Math_Divide(
        (((u32)((drawY << 2) + drawY) >> 16) << 16) + PIXELS(3), 10);
    FieldEffect_Spawn(x, leader->y.fixed, z, 0, velocityY, cycleFrame,
                      EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE |
                          EFFECT_SCRIPT_MEDIUM,
                      settings);
  }
}

/// Jumps two cells forward when both the intervening and landing cells permit
/// it.
static s32 CrossboneIsleSteppingStones_TryJumpAcrossPillar(void) {
  struct ObjectEvent *leader;
  u8 *flags;
  s32 savedMotionFlags;
  s32 jumpDirection;
#if !TBS_PATCH_1
  s32 step;
#endif
  struct FieldPosition *position;
  struct FieldPosition destination;

  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  flags = &leader->motionFlags;
  savedMotionFlags = *flags;
#if TBS_PATCH_1
  jumpDirection = (leader->facing + 0x2000) & 0xc000;
#endif
  if (gSaveBlock.movementMode != LEADER_MOVEMENT_WALK) {
    return 0;
  }
  position = &destination;
#if TBS_PATCH_1
  position->x = (leader->x.fixed & -0x100000) + PIXELS(8);
  position->y = leader->y.fixed;
  position->z = (leader->z.fixed & -0x100000) + PIXELS(8);
  Vector_AddPolarOffset(PIXELS(16), jumpDirection, position);
#else
  jumpDirection = leader->facing >> 12;
  step = gPushedObject_DirectionSteps[jumpDirection];
  position->x = leader->x.fixed + (step & 0xffff0000);
  position->y = leader->y.fixed;
  step <<= 16;
  position->z = leader->z.fixed + step;
#endif
  // Require a free intervening cell before testing the two-cell landing.
  if (FieldObject_CheckMovementCollision(leader, position) != 1 &&
      PushedObject_FindAtTile(position, leader) == 0) {
#if TBS_PATCH_1
    position->x = (leader->x.fixed & -0x100000) + PIXELS(8);
    position->y = leader->y.fixed;
    position->z = (leader->z.fixed & -0x100000) + PIXELS(8);
    Vector_AddPolarOffset(PIXELS(32), jumpDirection, position);
#else
    step = gPushedObject_DirectionSteps[jumpDirection];
    position->x = leader->x.fixed + ((step & 0xffff0000) * 2);
    position->y = leader->y.fixed;
    step <<= 17;
    position->z = leader->z.fixed + step;
#endif
    if (PushedObject_FindAtTile(position, leader) == 0 &&
        FieldObject_CheckMovementCollision(leader, position) == 0) {
      FieldScript_Start();
      Object_SelectAnimation(leader, 6);
      WaitFrames(6);
      Sound_PlaySong(SE_JUMP);
      Object_SelectAnimation(leader, 7);
      leader->speed = PIXELS(3);
      leader->acceleration = PIXELS(2);
      leader->velocityY = PIXELS(4);
      *flags &= 126;
      ObjectEvent_SetSpriteFlags(leader, 0);
      ObjectMotion_SetPositionAndCommit(LOCALID_PARTY_LEADER, position->x >> 16,
                                        position->z >> 16);
      Object_SelectAnimation(leader, 6);
      ObjectEvent_SetSpriteFlags(leader, 1);
      *flags = savedMotionFlags;
      FieldScript_End();
      return 1;
    }
  }
  return 0;
}

/// Moves the particle while damping its velocity, growing it, and advancing
/// rotation.
static void CrossboneIsleSteppingStones_UpdateDampedParticle(
    union ObjectEventOrEffect *fieldObject) {
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;

  fieldObject->effect.x += fieldObject->effect.velocity.x;
  fieldObject->effect.y += fieldObject->effect.velocity.y;
  fieldObject->effect.z += fieldObject->effect.velocity.z;
  velocityX = fieldObject->effect.velocity.x;
  velocityY = fieldObject->effect.velocity.y;
  velocityZ = fieldObject->effect.velocity.z;
  fieldObject->effect.velocity.x = velocityX - Math_Divide(velocityX, 10);
  fieldObject->effect.velocity.y = velocityY - Math_Divide(velocityY, 3);
  fieldObject->effect.velocity.z = velocityZ - Math_Divide(velocityZ, 10);
  fieldObject->effect.scaleX += fieldObject->effect.scaleRateX;
  fieldObject->effect.scaleY += fieldObject->effect.scaleRateY;
  fieldObject->effect.sprite->rotation += fieldObject->effect.spin;
}

/// Sets the object event's sprite to the colour mode (enum SpriteColorMode) in
/// the low four bits of its timer.
static s32
CrossboneIsleSteppingStones_SetChildValueFromTimer(struct Object *interpreter) {
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)interpreter;
  ObjectEvent_SetSpriteColor(objectEvent, objectEvent->timer & 15);
  return 0;
}

const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 sceneId = gSaveBlock.sceneId;

  if (sceneId == (s32)&SceneId_CrossboneIsleSteppingStones) {
    return gCrossboneIsleSteppingStonesEntrances;
  }
  if (sceneId == (s32)&SceneId_CrossboneIsleIcePillars) {
    return gCrossboneIsleIcePillarsEntrances;
  }
  if (sceneId == (s32)&SceneId_CrossboneIsleFloodgate) {
    return gCrossboneIsleFloodgateEntrances;
  }
  if (sceneId == (s32)&SceneId_CrossboneIsleStackedPillars) {
    return gCrossboneIsleStackedPillarsEntrances;
  }
  if (sceneId == (s32)&SceneId_CrossboneIsleDragonStatue) {
    return gCrossboneIsleDragonStatueEntrances;
  }
  return gCrossboneIsleSteppingStonesEntrancesOther;
}

const struct SceneExitRegion *Scene_GetExitRegions(void) { return 0; }

/// Returns the exits the five rooms share.
const u32 *Scene_GetExits(void) { return gCrossboneIsleSteppingStonesExits; }

const struct ObjectEventTemplate *Scene_GetObjectEventTemplates(void) {
  s16 sceneId = gSaveBlock.sceneId;

  if (sceneId == (s32)&SceneId_CrossboneIsleSteppingStones) {
    return gCrossboneIsleSteppingStonesObjectEventTemplates;
  }
  if (sceneId == (s32)&SceneId_CrossboneIsleIcePillars) {
    return gCrossboneIsleIcePillarsObjectEventTemplates;
  }
  if (sceneId == (s32)&SceneId_CrossboneIsleFloodgate) {
    return gCrossboneIsleFloodgateObjectEventTemplates;
  }
  if (sceneId == (s32)&SceneId_CrossboneIsleDragonStatue) {
    return gCrossboneIsleDragonStatueObjectEventTemplates;
  }
  return gCrossboneIsleSteppingStonesObjectEventTemplatesOther;
}

/// Walking into a pillar in CrossboneIsleSteppingStones: jump two cells across
/// it where the cells allow, otherwise push it.
static void EventScript_JumpOrPushPillar(void) {
  FieldScript_Start();
  if (CrossboneIsleSteppingStones_TryJumpAcrossPillar() == 0)
    PushedObject_AdvancePair();
  FieldScript_End();
}

/// Walking into a pillar in CrossboneIsleIcePillars, or a log in
/// CrossboneIsleFloodgate: push it if the cell beyond is clear.
static void EventScript_PushPillar(void) {
  struct PushedObjectProbe out;

  FieldScript_Start();
  if (PushedObject_FindClearPosition(&out) != 0)
    ObjectEvent_MoveAndRedraw(out);
  FieldScript_End();
}

/// A pillar slot's place and ground cell, without its flag; it holds one slot
/// while two are swapped.
struct PillarPlacement {
  s32 x; ///< 16-pixel map cells
  s32 y; ///< 16-pixel map cells
  s32 z; ///< 16-pixel map cells
  union CrossboneIslePillarCell cell;
};

/// Where one of CrossboneIsleStackedPillars's pillars stands, the ground cell
/// saved under it and its footprint flag.
struct PillarSlot {
  s32 x; ///< 16-pixel map cells
  s32 y; ///< 16-pixel map cells
  s32 z; ///< 16-pixel map cells
  union CrossboneIslePillarCell cell;
  s32 flag;
};

static struct PillarSlot sPillarSlots[4];
static struct FieldPosition sProbePosition;
static u8 sShakenScroll[12];
static s32 sShakeChance;

/// Randomly substitutes the shifted background scroll during the water
/// transition.
static void CrossboneIsleSteppingStones_JitterBackgroundScroll(void) {
  u32 line = REG_VCOUNT;
  u32 *src = &((u32 *)gBgScroll)[1];
  volatile u32 *reg = (volatile u32 *)&REG_BG1HOFS;

  if (line == 227 || line <= 46) {
    if ((u32)(Import_RandomNext() * 100) >> 16 < sShakeChance) {
      src = ((u32 *)sShakenScroll);
    }
  }
  *reg = *src++;
  reg = (volatile u32 *)&REG_BG2HOFS;
  *reg++ = *src++;
  *reg = *src;
}

/// Waits until the DMA channel clears its enable bit.
static __inline__ void Dma_Wait(volatile u32 *dma) {
  while (dma[2] & ((u32)DMA_ENABLE << 16))
    ;
}

/// Copies the three background scroll pairs and shifts their vertical offsets
/// by 192 pixels.
static void CrossboneIsleSteppingStones_PrepareWaterScroll(void) {
  u32 *destination;
  const u32 *source;
  u16 *coordinates;

  source = (const u32 *)&((u32 *)gBgScroll)[1];
  destination = (u32 *)sShakenScroll;
  *destination++ = *source++;
  *destination++ = *source++;
  *destination = *source;
  coordinates = (u16 *)sShakenScroll;
  coordinates[1] += 0xc0;
  coordinates[3] += 0xc0;
  coordinates[5] += 0xc0;
}

/// Reveals the alternate water tiles through a gradually randomized scroll
/// transition.
static void CrossboneIsleSteppingStones_AnimateWaterLevel(void) {
  s32 waterRaised;

  waterRaised = GameFlag_Get(FLAG_CROSSBONE_ISLE_FLOODGATE_WATER_RAISED);
  if (waterRaised == 0) {
    MapGrid_DrawCellPanel(10, 19, 16, 5, waterRaised, 10, 31);
    MapGrid_DrawCellPanel(10, 51, 16, 5, 1, 10, 31);
    MapGrid_DrawCellPanel(42, 51, 16, 5, 2, 10, 31);
  } else {
    MapGrid_DrawCellPanel(10, 19, 16, 5, 0, 10, 31);
    MapGrid_DrawCellPanel(10, 83, 16, 5, 1, 10, 31);
    MapGrid_DrawCellPanel(42, 83, 16, 5, 2, 10, 31);
  }
  // Increase the chance of showing the alternate scroll on each scanline.
  sShakeChance = 0;
  FieldScheduler_AddCallback(CrossboneIsleSteppingStones_PrepareWaterScroll,
                             TASK_PRIORITY_MAP);
  WaitFrames(1);
  Irq_SetHandler(1, 0, CrossboneIsleSteppingStones_JitterBackgroundScroll);
  Sound_PlaySong(SE_WATER_LEVEL);
  sShakeChance = 0;
  do {
    WaitFrames(1);
  } while (++sShakeChance <= 100);
  // Commit the water tiles, then remove both scroll callbacks.
  Sound_PlaySong(SONG_FADE_OUT_ACTION_SE);
  if (GameFlag_Get(FLAG_CROSSBONE_ISLE_FLOODGATE_WATER_RAISED) == 0) {
    MapGrid_CopyCellsTo(0, 32, 32, 0, 32, 32);
    MapGrid_CopyCellsTo(32, 32, 64, 0, 32, 32);
  } else {
    MapGrid_CopyCellsTo(0, 64, 32, 0, 32, 32);
    MapGrid_CopyCellsTo(32, 64, 64, 0, 32, 32);
  }
  WaitFrames(1);
  Irq_SetHandler(1, 0, 0);
  WaitFrames(1);
  Task_Remove(CrossboneIsleSteppingStones_PrepareWaterScroll);
  MapGrid_CenterOnFollowedPoint();
  WaitFrames(30);
}

/// Searching the switch beside CrossboneIsleFloodgate's floodgate flips it.
/// "Isaac flipped the switch." The floodgate closes and the water backs up
/// behind it, floating the logs higher, or it opens and the water drains away
/// and the logs settle on the floor.
static void EventScript_FlipWaterSwitch(void) {
  s32 waterRaised;
  struct ObjectEvent *water;

  FieldScript_Start();
  FieldCamera_SetSpeed(PIXELS(1), PIXELS(1) / 8);
  FieldCamera_MoveTo(PIXELS(281), -1, PIXELS(432), 1);
  FieldCamera_WaitForArrival();
  FieldMessage_Show(MsgFieldFlippedSwitch, 1);
  waterRaised = Script_GetFlag(FLAG_CROSSBONE_ISLE_FLOODGATE_WATER_RAISED);
  // Raise or drain the water before redrawing the logs' footprints.
  if (waterRaised == 0) {
    Script_PlaySong(SE_CRUNCH);
    MapGrid_AnimateCells(gCrossboneIsleSteppingStones_SwitchCloseSteps, 84, 24);
    FieldScript_WaitFrames(30);
    Script_PlaySong(SE_LOG_FALL);
    ObjectMotion_SetActionVariant(LOCALID_WATER, 1);
    ObjectEvent_GetById(LOCALID_WATER)->motionFlags = waterRaised;
    water = ObjectEvent_Get(LOCALID_WATER);
    water->y.fixed = -PIXELS(32);
    ObjectEvent_SetPosition(LOCALID_WATER, PIXELS(272), PIXELS(416));
    Import_ObjectEventSetAnimation(LOCALID_WATER, 1);
    MapGrid_AnimateCells(gCrossboneIsleSteppingStones_FloodgateCloseSteps, 80,
                         24);
    MapGrid_AnimateCells(gCrossboneIsleSteppingStones_FloodgateFootSteps, 80,
                         28);
    MapGrid_CopyCellsTo(65, 40, 16, 27, 2, 4);
    CrossboneIsleSteppingStones_AnimateWaterLevel();
    CrossboneIsleSteppingStones_DrawFloatingLogs();
    MapGrid_CopyCellAttributes(24, 3, 1, 1, 24, 8);
    GameFlag_Set(FLAG_CROSSBONE_ISLE_FLOODGATE_WATER_RAISED);
  } else {
    Script_PlaySong(SE_CRUNCH);
    MapGrid_AnimateCells(gCrossboneIsleSteppingStones_SwitchOpenSteps, 84, 24);
    FieldScript_WaitFrames(30);
    Script_PlaySong(SE_WATER_DRAIN);
    ObjectEvent_GetById(LOCALID_WATER)->motionFlags = 0;
    water = ObjectEvent_Get(LOCALID_WATER);
    water->y.fixed = -PIXELS(32);
    ObjectEvent_SetPosition(LOCALID_WATER, PIXELS(272), PIXELS(436));
    Import_ObjectEventSetAnimation(LOCALID_WATER, 2);
    MapGrid_CopyCellsTo(65, 45, 16, 27, 2, 4);
    MapGrid_AnimateCells(gCrossboneIsleSteppingStones_FloodgateOpenSteps, 80,
                         24);
    CrossboneIsleSteppingStones_AnimateWaterLevel();
    CrossboneIsleSteppingStones_RestoreGroundFootprint(LOCALID_FIRST_LOG + 1);
    CrossboneIsleSteppingStones_RestoreGroundFootprint(LOCALID_FIRST_LOG + 2);
    CrossboneIsleSteppingStones_RestoreGroundFootprint(LOCALID_FIRST_LOG + 3);
    CrossboneIsleSteppingStones_RestoreGroundFootprint(LOCALID_FIRST_LOG + 4);
    CrossboneIsleSteppingStones_RestoreGroundFootprint(LOCALID_FIRST_LOG + 5);
    CrossboneIsleSteppingStones_RestoreGroundFootprint(LOCALID_FIRST_LOG + 6);
    CrossboneIsleSteppingStones_RestoreGroundFootprint(LOCALID_FIRST_LOG + 7);
    MapGrid_CopyCellAttributes(24, 4, 1, 1, 24, 8);
    GameFlag_Clear(FLAG_CROSSBONE_ISLE_FLOODGATE_WATER_RAISED);
  }
  FieldScript_End();
}

/// Matches a nearby object event's draw priority when it is behind the
/// comparison object event.
static s32 CrossboneIsleSteppingStones_SyncPriorityIfBehind(
    struct ObjectEvent *objectEvent, struct ObjectEvent *front) {
  s32 result = 0;

  if (front->x.fixed == objectEvent->x.fixed &&
      front->y.fixed == objectEvent->y.fixed &&
      front->z.fixed == objectEvent->z.fixed) {
    return result;
  }
  if (front->x.fixed - PIXELS(16) < objectEvent->x.fixed &&
      objectEvent->x.fixed < front->x.fixed + PIXELS(16) &&
      front->y.fixed / PIXELS(1) == objectEvent->y.fixed / PIXELS(1) &&
      front->z.fixed > objectEvent->z.fixed &&
      front->z.fixed - PIXELS(32) < objectEvent->z.fixed) {
    if (objectEvent->sprite->image.priority < front->sprite->image.priority) {
      objectEvent->priorityFlags &= ~OBJ_EVENT_PRIORITY_AUTOMATIC;
      objectEvent->sprite->image.priority = front->sprite->image.priority;
      objectEvent->sprite->shadow.priority = front->sprite->shadow.priority;
    }
    result = 1;
  }
  return result;
}

/// Matches the comparison object event's draw priority when it overlaps the
/// object event in front.
static s32 CrossboneIsleSteppingStones_SyncPriorityIfAhead(
    struct ObjectEvent *front, struct ObjectEvent *objectEvent) {
  s32 result = 0;

  if (objectEvent->x.fixed == front->x.fixed &&
      objectEvent->y.fixed == front->y.fixed &&
      objectEvent->z.fixed == front->z.fixed) {
    return result;
  }
  if (front->x.fixed - PIXELS(16) < objectEvent->x.fixed &&
      objectEvent->x.fixed < front->x.fixed + PIXELS(16) &&
      objectEvent->y.fixed / PIXELS(1) == front->y.fixed / PIXELS(1) &&
      front->z.fixed > objectEvent->z.fixed &&
      front->z.fixed - PIXELS(32) < objectEvent->z.fixed) {
    if (front->sprite->image.priority > objectEvent->sprite->image.priority) {
      objectEvent->priorityFlags &= ~OBJ_EVENT_PRIORITY_AUTOMATIC;
      objectEvent->sprite->image.priority = front->sprite->image.priority;
      objectEvent->sprite->shadow.priority = front->sprite->shadow.priority;
    }
    result = 1;
  }
  return result;
}

// UB: calling this s32 function through a void update hook is incompatible.
// Plain void changes 320 to 316 bytes in tbs-en.
#ifdef UBFIX
/// Keeps each moving pillar's priority and collision flags consistent with the
/// leader.
static void CrossboneIsleSteppingStones_UpdateObjectEventPriority(
    union ObjectEventOrEffect *fieldObject) {
#else
/// Keeps each moving pillar's priority and collision flags consistent with the
/// leader.
static s32 CrossboneIsleSteppingStones_UpdateObjectEventPriority(
    union ObjectEventOrEffect *fieldObject) {
#endif
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)fieldObject;
  struct ObjectEvent *leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  s32 hit;
  u32 id;

  if (gFieldWork->castStarted == 1) {
    objectEvent->sprite->image.priority = leader->sprite->image.priority;
    objectEvent->collisionFlags |= OBJ_EVENT_SOLID;
#ifdef UBFIX
    return;
#else
    return 0;
#endif
  }
  hit = CrossboneIsleSteppingStones_SyncPriorityIfBehind(objectEvent, leader);
  for (id = LOCALID_FIRST_PILLAR; id <= LOCALID_FIRST_PILLAR + 3; id++) {
    hit += CrossboneIsleSteppingStones_SyncPriorityIfBehind(
        objectEvent, ObjectEvent_GetById(id));
  }
  if (hit != 0) {
    for (id = LOCALID_FIRST_PILLAR; id <= LOCALID_FIRST_PILLAR + 3; id++) {
      CrossboneIsleSteppingStones_SyncPriorityIfAhead(objectEvent,
                                                      ObjectEvent_GetById(id));
    }
  }
  if (objectEvent->y.fixed < leader->y.fixed) {
    objectEvent->priorityFlags |= OBJ_EVENT_PRIORITY_UNDERFOOT;
    objectEvent->collisionFlags &= ~OBJ_EVENT_SOLID;
    if (objectEvent->sprite->image.priority < leader->sprite->image.priority) {
      objectEvent->priorityFlags &= ~OBJ_EVENT_PRIORITY_AUTOMATIC;
      objectEvent->sprite->image.priority = leader->sprite->image.priority;
      objectEvent->sprite->shadow.priority = leader->sprite->shadow.priority;
      hit = 1;
    }
  } else {
    objectEvent->priorityFlags &= ~OBJ_EVENT_PRIORITY_UNDERFOOT;
    objectEvent->collisionFlags |= OBJ_EVENT_SOLID;
  }
  if (hit == 0) {
    objectEvent->priorityFlags |= OBJ_EVENT_PRIORITY_AUTOMATIC;
  }
#ifdef UBFIX
  return;
#else
  return 0;
#endif
}

/// Sorts the four pillars by height and depth, carrying their saved cell state
/// with them.
static void CrossboneIsleSteppingStones_SortPillarObjectEvents(void) {
  u32 i;
  u32 j;
  struct ObjectEvent *firstObjectEvent;
  struct ObjectEvent *secondObjectEvent;
  struct ObjectEvent savedObjectEvent;
#ifdef UBFIX
  struct PillarPlacement savedPlacement;
#else
  // UB: the original 12-byte scratch receives a 16-byte DMA transfer.
  // A complete placement array adds four native stack bytes.
  s32 savedPlacement[3];
#endif

  for (i = 0; i <= 2; i++) {
    firstObjectEvent = ObjectEvent_GetById(i + LOCALID_FIRST_PILLAR);
    for (j = i; j <= 3; j++) {
      secondObjectEvent = ObjectEvent_GetById(j + LOCALID_FIRST_PILLAR);
      if (firstObjectEvent->y.fixed <= secondObjectEvent->y.fixed &&
          firstObjectEvent->z.fixed < secondObjectEvent->z.fixed) {
        continue;
      }
      Dma_Set(secondObjectEvent, &savedObjectEvent,
              ((u32)(DMA_ENABLE | DMA_32BIT) << 16) |
                  sizeof(savedObjectEvent) / sizeof(u32),
              REG_DMA3);
      Dma_Wait(REG_DMA3);
      Dma_Set(firstObjectEvent, secondObjectEvent,
              ((u32)(DMA_ENABLE | DMA_32BIT) << 16) |
                  sizeof(savedObjectEvent) / sizeof(u32),
              REG_DMA3);
      Dma_Wait(REG_DMA3);
      Dma_Set(&savedObjectEvent, firstObjectEvent,
              ((u32)(DMA_ENABLE | DMA_32BIT) << 16) |
                  sizeof(savedObjectEvent) / sizeof(u32),
              REG_DMA3);
      Dma_Wait(REG_DMA3);
      Dma_Set(&sPillarSlots[j], &savedPlacement,
              ((u32)(DMA_ENABLE | DMA_32BIT) << 16) |
                  sizeof(struct PillarPlacement) / sizeof(u32),
              REG_DMA3);
      Dma_Wait(REG_DMA3);
      Dma_Set(&sPillarSlots[i], &sPillarSlots[j],
              ((u32)(DMA_ENABLE | DMA_32BIT) << 16) |
                  sizeof(struct PillarPlacement) / sizeof(u32),
              REG_DMA3);
      Dma_Wait(REG_DMA3);
      Dma_Set(&savedPlacement, &sPillarSlots[i],
              ((u32)(DMA_ENABLE | DMA_32BIT) << 16) |
                  sizeof(struct PillarPlacement) / sizeof(u32),
              REG_DMA3);
      Dma_Wait(REG_DMA3);
      if (GameFlag_Get(sPillarSlots[i].flag) &&
          !GameFlag_Get(sPillarSlots[j].flag)) {
        GameFlag_Clear(sPillarSlots[i].flag);
        GameFlag_Set(sPillarSlots[j].flag);
      } else if (!GameFlag_Get(sPillarSlots[i].flag) &&
                 GameFlag_Get(sPillarSlots[j].flag)) {
        GameFlag_Set(sPillarSlots[i].flag);
        GameFlag_Clear(sPillarSlots[j].flag);
      }
    }
  }
}

/// Move or Carry on one of CrossboneIsleStackedPillars's pillars, and after
/// each push there: the pillars settle, and their footprints follow them.
static void EventScript_MovePillar(void) {
  u32 id;
  struct ObjectEvent *objectEvent;
  u8 *flags;
  u32 i;
  u32 k;
  struct ObjectEvent *other;
  union CrossboneIslePillarCell cell;

  for (id = LOCALID_FIRST_PILLAR; id <= LOCALID_FIRST_PILLAR + 3; id++) {
    objectEvent = ObjectEvent_GetById(id);
    objectEvent->layer = 2;
    i = id - LOCALID_FIRST_PILLAR;
    if ((objectEvent->x.fixed >> 20) == sPillarSlots[i].x &&
        (objectEvent->z.fixed >> 20) == sPillarSlots[i].z &&
        objectEvent->velocityY == 0) {
      continue;
    }
    Dma_Set(&objectEvent->x, &sProbePosition,
            ((u32)(DMA_ENABLE | DMA_32BIT) << 16) |
                sizeof(sProbePosition) / sizeof(u32),
            REG_DMA3);
    Dma_Wait(REG_DMA3);
    if (FieldObject_CheckMovementCollision(objectEvent, &sProbePosition) ==
        -1) {
      objectEvent->motionFlags = OBJ_EVENT_FOLLOWS_TERRAIN | OBJ_EVENT_FALLS;
    }
    flags = &objectEvent->motionFlags;
    CrossboneIsleSteppingStones_SetCellAttributes(
        0, sPillarSlots[i].x, sPillarSlots[i].z, &sPillarSlots[i].cell);
    CrossboneIsleSteppingStones_SetCellAttributes(
        2, sPillarSlots[i].x, sPillarSlots[i].z, &sPillarSlots[i].cell);
    // Falling pillars either reach a ledge, shrink away, or settle on ground.
    if (*flags & 1) {
      if (MapGrid_GetMetatileBehaviorAt(2, objectEvent->x.fixed,
                                        objectEvent->z.fixed) == 50) {
        Sound_PlaySong(SE_STONE_GRIND);
        // Both u8 accesses are alias set 0, and the view is no struct reference
        // (MEM_IN_STRUCT_P); as field stores, the loop is reallocated (100 of
        // 265 instructions differ in tbs-en).
        *(u8 *)&objectEvent->priorityFlags &= ~OBJ_EVENT_PRIORITY_AUTOMATIC;
        CrossboneIsleSteppingStones_LowerObjectEventToLedge(id, 1);
        *(u8 *)&objectEvent->priorityFlags |= OBJ_EVENT_PRIORITY_AUTOMATIC;
      } else if (MapGrid_GetMetatileBehaviorAt(2, objectEvent->x.fixed,
                                               objectEvent->z.fixed) == 51) {
        CrossboneIsleSteppingStones_StopAtGroundOrHeight(objectEvent, 0);
        Sound_PlaySong(SE_STONE_GRIND);
        objectEvent->y.fixed = 0;
        objectEvent->priorityFlags &= ~OBJ_EVENT_PRIORITY_AUTOMATIC;
        CrossboneIsleSteppingStones_ShrinkPillar(id);
        objectEvent->x.fixed = 0;
        objectEvent->y.fixed = 0;
        objectEvent->z.fixed = 0;
        objectEvent->priorityFlags |= OBJ_EVENT_PRIORITY_AUTOMATIC;
      } else {
        CrossboneIsleSteppingStones_SettleObjectEventOnGround(objectEvent);
      }
      *flags = 0;
    }
    // Save the new ground cell before applying the pillar's height attributes.
    CrossboneIsleSteppingStones_ReadMapGridCell(0, objectEvent->x.fixed >> 20,
                                                objectEvent->z.fixed >> 20,
                                                &sPillarSlots[i].cell);
    if (objectEvent->y.fixed >= 0) {
      CrossboneIsleSteppingStones_ReadMapGridCell(
          0, 27, (objectEvent->y.fixed >> 20) + 6, &cell);
      CrossboneIsleSteppingStones_SetCellAttributes(
          0, objectEvent->x.fixed >> 20, objectEvent->z.fixed >> 20, &cell);
      cell.bits.highFlags = sPillarSlots[i].cell.bits.highFlags;
      // x and z read by their offsets: through ObjectEvent's single/group
      // union, as objectEvent->x.fixed, a load moves.
      CrossboneIsleSteppingStones_SetCellAttributes(
          2,
          *(s32 *)((u8 *)objectEvent + offsetof(struct ObjectEvent, x)) >> 20,
          *(s32 *)((u8 *)objectEvent + offsetof(struct ObjectEvent, z)) >> 20,
          &cell);
    }
    sPillarSlots[i].x = objectEvent->x.fixed >> 20;
    sPillarSlots[i].y = objectEvent->y.fixed >> 20;
    sPillarSlots[i].z = objectEvent->z.fixed >> 20;
    // Mark pillars covered by this one, then restore their sort order.
    for (k = 0; k <= 3; k++) {
      if (k == i) {
        continue;
      }
      GameFlag_Clear(sPillarSlots[k].flag);
      other = ObjectEvent_GetById(k + LOCALID_FIRST_PILLAR);
      if ((objectEvent->x.fixed >> 20) == (other->x.fixed >> 20) &&
          (objectEvent->z.fixed >> 20) == (other->z.fixed >> 20) &&
          objectEvent->y.fixed > other->y.fixed) {
        GameFlag_Set(sPillarSlots[k].flag);
      }
    }
  }
  CrossboneIsleSteppingStones_SortPillarObjectEvents();
}

/// Walking into a pillar in CrossboneIsleStackedPillars: jump across if
/// possible, otherwise push it, and the pillars settle.
static void EventScript_JumpOrPushStackedPillar(void) {
  FieldScript_Start();
#if TBS_PATCH_3
  ObjectEvent_GetById(LOCALID_PARTY_LEADER)->y.fixed = MapGrid_GetTerrainHeight(
      ObjectEvent_GetById(LOCALID_PARTY_LEADER)->layer,
      ObjectEvent_GetById(LOCALID_PARTY_LEADER)->x.fixed,
      ObjectEvent_GetById(LOCALID_PARTY_LEADER)->z.fixed);
#endif
  if (CrossboneIsleSteppingStones_TryJumpAcrossPillar() == 0) {
    ObjectEvent_GetById(LOCALID_PARTY_LEADER)->motionFlags &=
        ~OBJ_EVENT_FOLLOWS_TERRAIN;
    ObjectEvent_GetById(LOCALID_PARTY_LEADER)->priorityFlags &=
        ~OBJ_EVENT_PRIORITY_AUTOMATIC;
    PushedObject_AdvancePair();
    EventScript_MovePillar();
    ObjectEvent_GetById(LOCALID_PARTY_LEADER)->motionFlags |=
        OBJ_EVENT_FOLLOWS_TERRAIN;
    ObjectEvent_GetById(LOCALID_PARTY_LEADER)->priorityFlags |=
        OBJ_EVENT_PRIORITY_AUTOMATIC;
  }
  FieldScript_End();
}

/// Walking north at CrossboneIsleStackedPillars's ladder: climb up.
static void EventScript_ClimbUp(void) { Field_ClimbUp(); }

/// Walking south at CrossboneIsleStackedPillars's ladder: push the pillar
/// below, jump, or climb down.
static void EventScript_PushOrClimbDown(void) {
  struct ObjectEvent *leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  struct FieldPosition position;
  struct FieldPosition *p = &position;

  p->x = leader->x.fixed;
  p->y = leader->y.fixed;
  p->z = leader->z.fixed + PIXELS(16);
  if (PushedObject_FindAtTile(p, leader) != NULL) {
    EventScript_JumpOrPushStackedPillar();
  } else if (CrossboneIsleSteppingStones_TryJumpAcrossPillar() == 0) {
    Field_ClimbDown();
  }
}

/// Stepping on coord event 40 in CrossboneIsleStackedPillars: the fade overlay
/// follows the leader, and the leader's rise counter starts.
static void EventScript_StartRising(void) {
  struct ObjectEvent *leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  ((struct FadeOverlayWork *)HEAP_SLOT_BLOCK(HEAP_SLOT_FADE_OVERLAY))
      ->objectEvent = leader;
  leader->riseCounter = 1;
}

/// Stepping on coord event 41 in CrossboneIsleStackedPillars: the overlay lets
/// the leader go, and the rise counter stops.
static void EventScript_StopRising(void) {
  struct ObjectEvent *leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);

  ((struct FadeOverlayWork *)HEAP_SLOT_BLOCK(HEAP_SLOT_FADE_OVERLAY))
      ->objectEvent = NULL;
  leader->riseCounter = 0;
}

// UB: calling this s32 function through a void update hook is incompatible.
// Plain void changes 54 to 52 bytes in tbs-en.
#ifdef UBFIX
/// Keeps the frozen puddle's underfoot flag consistent with the leader's height
/// and depth.
static void CrossboneIsleSteppingStones_UpdateFrozenPuddlePriority(
    union ObjectEventOrEffect *fieldObject) {
#else
/// Puts the frozen puddle under the leader when it is behind and below the
/// leader.
static s32 CrossboneIsleSteppingStones_UpdateFrozenPuddlePriority(
    union ObjectEventOrEffect *fieldObject) {
#endif
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)fieldObject;
  struct ObjectEvent *ref = ObjectEvent_GetById(LOCALID_PARTY_LEADER);

  if (objectEvent->z.fixed > ref->z.fixed) {
    objectEvent->priorityFlags &= ~OBJ_EVENT_PRIORITY_UNDERFOOT;
    if (objectEvent->y.fixed < ref->y.fixed)
      objectEvent->priorityFlags |= OBJ_EVENT_PRIORITY_UNDERFOOT;
  }

#ifdef UBFIX
  return;
#else
  return 0;
#endif
}

/// Remembers the frozen puddle, installs its priority callback, and starts its
/// flicker script.
static void CrossboneIsleSteppingStones_FreezePuddle(s32 id) {
  struct ObjectEvent *objectEvent;

  objectEvent = ObjectEvent_GetById(id);
  FieldScript_Start();
#ifdef UBFIX
  objectEvent->update = CrossboneIsleSteppingStones_UpdateFrozenPuddlePriority;
#else
  objectEvent->update = (void (*)(union ObjectEventOrEffect *))
      CrossboneIsleSteppingStones_UpdateFrozenPuddlePriority;
#endif
  MapGrid_CopyCellAttributes(20, 14, 1, 1, objectEvent->x.fixed >> 20,
                             objectEvent->z.fixed >> 20);
  GameFlag_Set(FLAG_CROSSBONE_ISLE_DRAGON_STATUE_FIRST_PUDDLE_FROZEN + id -
               LOCALID_FIRST_PUDDLE);
  ObjectEvent_SetActionScript(id, gCrossboneIsleSteppingStones_ActionTable);
  FieldScript_End();
}

/// Frost on the first puddle in CrossboneIsleDragonStatue: it freezes into a
/// pillar of ice.
static void EventScript_FreezeFirstPuddle(void) {
  CrossboneIsleSteppingStones_FreezePuddle(LOCALID_FIRST_PUDDLE);
}

/// Frost on the second puddle in CrossboneIsleDragonStatue.
static void EventScript_FreezeSecondPuddle(void) {
  CrossboneIsleSteppingStones_FreezePuddle(LOCALID_SECOND_PUDDLE);
}

/// Frost begins on the third puddle in CrossboneIsleDragonStatue: note whether
/// the leader stands on it.
static void EventScript_FrostStartsOnThirdPuddle(void) {
  struct ObjectEvent *leader;
  struct ObjectEvent *puddle;
  u8 *effects;

  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  puddle = ObjectEvent_GetById(LOCALID_THIRD_PUDDLE);
  effects = gFieldEffectWork;
  if (puddle->x.fixed >> 20 == leader->x.fixed >> 20 &&
      puddle->z.fixed >> 20 == leader->z.fixed >> 20) {
    GameFlag_Set(FLAG_CROSSBONE_ISLE_DRAGON_STATUE_LEADER_ON_THIRD_PUDDLE);
    effects[53] = 1;
  } else {
    GameFlag_Clear(FLAG_CROSSBONE_ISLE_DRAGON_STATUE_LEADER_ON_THIRD_PUDDLE);
  }
}

/// Later in the same Frost in CrossboneIsleDragonStatue: the third puddle
/// shrinks back, and the pillar in it (LOCALID_RISING_PILLAR) rises 32 pixels.
static void EventScript_FrostRaisesPillar(void) {
  struct ObjectEvent *pillar;

  pillar = ObjectEvent_GetById(LOCALID_RISING_PILLAR);
  ObjectEvent_GetById(LOCALID_THIRD_PUDDLE)->scaleX = Q16_ONE;
  ObjectEvent_GetById(LOCALID_THIRD_PUDDLE)->scaleY = Q16_ONE;
  (ObjectEvent_GetById(LOCALID_THIRD_PUDDLE)->sprite)->image.priority = 2;
  pillar->sprite->image.priority = 2;
  pillar->acceleration = PIXELS(2) / 5;
  pillar->speed = PIXELS(4) / 5;
  Object_SetPosition(pillar, pillar->x.fixed, PIXELS(32), pillar->z.fixed);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_RISING_PILLAR);
  MapGrid_CopyCellAttributes(20, 14, 1, 1, 22, 16);
}

/// Emits growing, rotating particles during the third puddle's Frost sequence.
static void CrossboneIsleSteppingStones_SpawnFrostParticle(void) {
  struct EffectOptions options;
  struct EffectOptions *settings;
  s32 velocityX;
  s32 velocityY;
  u32 draw;
  u32 mask;

  if ((gFrameCount & 2) != 0)
    return;
  if ((gFrameCount & 7) == 0)
    Sound_PlaySong(SE_WHOOSH);

  settings = &options;
  settings->palette = 10;
  settings->startScaleX = Q16_ONE / 2;
  settings->startScaleY = Q16_ONE / 2;
  settings->targetScaleX = 8 * Q16_ONE / 5;
  settings->targetScaleY = 8 * Q16_ONE / 5;
  draw = Import_RandomNext();
  mask = 0x0ffff000;
  mask &= draw;
  settings->rotation = mask;
  settings->update = CrossboneIsleSteppingStones_UpdateDampedParticle;

  draw = Import_RandomNext();
  velocityX = -(((draw * 5) >> 16) * PIXELS(1) + PIXELS(6));
  velocityX /= 2;
  draw = Import_RandomNext();
  velocityY = -(((draw * 5) >> 16) * PIXELS(1) + PIXELS(5));

  FieldEffect_Spawn(
      PIXELS(324), PIXELS(48), PIXELS(228), velocityX, velocityY, 0,
      EFFECT_USE_UPDATE | EFFECT_USE_ROTATION | EFFECT_USE_START_SCALE |
          EFFECT_SCALE_TO_TARGET | EFFECT_USE_PALETTE,
      settings);
}

// UB: calling this s32 function through a void update hook is incompatible.
// Plain void changes 172 to 168 bytes in tbs-en.
#ifdef UBFIX
/// Emits a randomized particle from the thawing puddle every eight frames.
static void CrossboneIsleSteppingStones_SpawnThawParticle(
    union ObjectEventOrEffect *fieldObject) {
#else
/// Emits a randomized particle from the thawing puddle every eight frames.
static s32 CrossboneIsleSteppingStones_SpawnThawParticle(
    union ObjectEventOrEffect *fieldObject) {
#endif
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)fieldObject;
  struct EffectOptions options;
  struct EffectOptions *settings;
  u32 phase = gFrameCount & 7;
  s32 x;
  s32 y;
  s32 z;
  s32 scale;

  if (phase != 0)
#ifdef UBFIX
    return;
#else
    return 0;
#endif

  settings = &options;
  settings->palette = 7;
  settings->startScaleX = 7 * Q16_ONE / 10;
  settings->startScaleY = 7 * Q16_ONE / 10;

  x = objectEvent->x.fixed +
      (((((u32)Import_RandomNext() * 17) >> 16) - 8) << 16);
  y = objectEvent->y.fixed + ((((u32)Import_RandomNext() * 17) >> 16) << 16);
  z = objectEvent->z.fixed +
      (((((u32)Import_RandomNext() * 17) >> 16) - 8) << 16);
  scale = Math_Divide(
      (((u32)Import_RandomNext() * 5) >> 16) * PIXELS(1) + PIXELS(3), 10);

  FieldEffect_Spawn(x, y, z, 0, scale, phase,
                    EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE |
                        EFFECT_SCRIPT_MEDIUM,
                    settings);
#ifdef UBFIX
  return;
#else
  return 0;
#endif
}

/// Frost ends on the third puddle in CrossboneIsleDragonStatue: unless the
/// leader stood on it, it freezes, and a frozen second puddle thaws back to
/// water.
static void EventScript_FreezeThirdPuddle(void) {
  s32 set;

  set = GameFlag_Get(FLAG_CROSSBONE_ISLE_DRAGON_STATUE_LEADER_ON_THIRD_PUDDLE);
  if (set == 0) {
    GameFlag_Set(FLAG_CROSSBONE_ISLE_DRAGON_STATUE_FIRST_PUDDLE_FROZEN + 2);
    FieldScript_Start();
    FieldCamera_SetSpeedParameters(PIXELS(3) / 5, PIXELS(3) / 40);
    FieldCamera_MoveWithinMapBounds(PIXELS(312), -1, PIXELS(184), 1);
    FieldCamera_WaitForArrival();
    FieldScript_WaitFrames(20);
    MapGrid_CopyCellsTo(73, 10, 60, 10, 1, 2);
    FieldScript_WaitFrames(20);
    // The third puddle freezes while the previously frozen second puddle thaws.
    FieldScheduler_AddCallback(CrossboneIsleSteppingStones_SpawnFrostParticle,
                               TASK_PRIORITY_MAP);
    FieldScript_WaitFrames(40);
    if (Script_GetFlag(FLAG_CROSSBONE_ISLE_DRAGON_STATUE_FIRST_PUDDLE_FROZEN +
                       1) != 0) {
#ifdef UBFIX
      ObjectEvent_GetById(LOCALID_SECOND_PUDDLE)->update =
          CrossboneIsleSteppingStones_SpawnThawParticle;
#else
      ObjectEvent_GetById(LOCALID_SECOND_PUDDLE)->update =
          (void (*)(union ObjectEventOrEffect *))
              CrossboneIsleSteppingStones_SpawnThawParticle;
#endif
      Import_ObjectEventSetAnimation(LOCALID_SECOND_PUDDLE, 6);
      ObjectMotion_WaitForAnimationChange(LOCALID_SECOND_PUDDLE);
      ObjectEvent_GetById(LOCALID_SECOND_PUDDLE)->update =
          (void (*)(union ObjectEventOrEffect *))set;
      MapGrid_CopyCellAttributes(17, 13, 1, 1, 18, 13);
      GameFlag_Clear(FLAG_CROSSBONE_ISLE_DRAGON_STATUE_FIRST_PUDDLE_FROZEN + 1);
      ObjectEvent_ConfigureSpriteColor(LOCALID_SECOND_PUDDLE,
                                       SPRITE_COLOR_NORMAL);
      ObjectEvent_SetBuiltinAction(LOCALID_SECOND_PUDDLE,
                                   OBJ_EVENT_ACTION_IDLE);
    } else {
      FieldScript_WaitFrames(60);
    }
    Task_Remove(CrossboneIsleSteppingStones_SpawnFrostParticle);
    FieldScript_WaitFrames(20);
    MapGrid_CopyCellsTo(72, 10, 60, 10, 1, 2);
    FieldScript_WaitFrames(20);
    FieldScript_End();
  }
}

/// Move begins on CrossboneIsleDragonStatue's sinking boulder: its cell is
/// cleared.
static void EventScript_StartMovingSinkingPillar(void) {
  struct ObjectEvent *pillar = ObjectEvent_GetById(LOCALID_SINKING_BOULDER);
  s32 x;
  s32 z;

  FieldScript_Start();

  x = pillar->x.fixed >> 20;
  z = pillar->z.fixed >> 20;

  PushedObject_FillGridAttributeRectangle(2, x, z, 1, 1, MB_NORMAL);
  FieldScript_End();
}

/// Move on CrossboneIsleDragonStatue's sinking boulder, and after each push:
/// once it reaches column 16 it sinks two pixels into place for good.
static void EventScript_MoveSinkingPillar(void) {
  struct ObjectEvent *pillar;
  s32 alreadyLowered;

  pillar = ObjectEvent_GetById(LOCALID_SINKING_BOULDER);
  FieldScript_Start();
  PushedObject_FillGridAttributeRectangle(
      2, (pillar->x.fixed >> 20), (pillar->z.fixed >> 20), 1, 1, MB_WALL);
  if ((pillar->x.fixed >> 20) == 16) {
    alreadyLowered =
        Script_GetFlag(FLAG_CROSSBONE_ISLE_DRAGON_STATUE_BOULDER_SUNK);
    if (alreadyLowered == 0) {
      FieldScript_WaitFrames(10);
      Sound_PlaySong(SE_DOOR_CLOSE);
      pillar->motionFlags = alreadyLowered;
      pillar->groundHeight = -PIXELS(2);
      pillar->y.fixed = -PIXELS(2);
      GameFlag_Set(FLAG_CROSSBONE_ISLE_DRAGON_STATUE_BOULDER_SUNK);
    }
  }
  FieldScript_End();
}

/// Walking into CrossboneIsleDragonStatue's sinking boulder: push it if the
/// cell beyond is clear.
static void EventScript_PushSinkingPillar(void) {
  struct PushedObjectProbe probe;

  FieldScript_Start();
  if (PushedObject_FindClearPosition(&probe) != 0) {
    ObjectEvent_MoveAndRedraw(probe);
  } else {
    EventScript_StartMovingSinkingPillar();
    PushedObject_AdvancePair();
    EventScript_MoveSinkingPillar();
  }
  FieldScript_End();
}

const struct MapEvent *Scene_GetEvents(void) {
  s16 sceneId = gSaveBlock.sceneId;

  if (sceneId == (s32)&SceneId_CrossboneIsleSteppingStones) {
    return gCrossboneIsleSteppingStonesEvents;
  }
  if (sceneId == (s32)&SceneId_CrossboneIsleIcePillars) {
    return gCrossboneIsleIcePillarsEvents;
  }
  if (sceneId == (s32)&SceneId_CrossboneIsleFloodgate) {
    return gCrossboneIsleFloodgateEvents;
  }
  if (sceneId == (s32)&SceneId_CrossboneIsleStackedPillars) {
    return gCrossboneIsleStackedPillarsEvents;
  }
  if (sceneId == (s32)&SceneId_CrossboneIsleDragonStatue) {
    return gCrossboneIsleDragonStatueEvents;
  }
  return gCrossboneIsleSteppingStonesEventsOther;
}

/// Prepares the lowered object event's collision and draw priority below the
/// leader.
static void CrossboneIsleSteppingStones_PrepLoweredObjectEvent(s32 id) {
  struct ObjectEvent *objectEvent = ObjectEvent_GetById(id);

  objectEvent->collisionFlags &= ~OBJ_EVENT_SOLID;
  objectEvent->priorityFlags |= OBJ_EVENT_PRIORITY_UNDERFOOT;
  objectEvent->motionFlags = 0;
  ObjectEvent_SetSpriteFlags(objectEvent, 0);
  objectEvent->sprite->image.priority = 2;
}

/// Restores a previously frozen pillar's mode, map cell, priority hook, and
/// script.
static void CrossboneIsleSteppingStones_RestoreFlaggedPillar(s32 localId) {
  struct ObjectEvent *objectEvent;

  objectEvent = ObjectEvent_GetById(localId);
  if (GameFlag_Get(FLAG_CROSSBONE_ISLE_DRAGON_STATUE_FIRST_PUDDLE_FROZEN +
                   localId - LOCALID_FIRST_PUDDLE) != 0) {
    Object_SelectAnimation(objectEvent, 5);
#ifdef UBFIX
    objectEvent->update =
        CrossboneIsleSteppingStones_UpdateFrozenPuddlePriority;
#else
    objectEvent->update = (void (*)(union ObjectEventOrEffect *))
        CrossboneIsleSteppingStones_UpdateFrozenPuddlePriority;
#endif
    MapGrid_CopyCellAttributes(20, 14, 1, 1, objectEvent->x.fixed >> 20,
                               objectEvent->z.fixed >> 20);
    ObjectEvent_SetActionScript(localId,
                                gCrossboneIsleSteppingStones_ActionTable);
  }
}

/// Uses layer 2 and installs the moving pillar's priority callback.
static void CrossboneIsleSteppingStones_InitializePillarPriority(s32 id) {
  struct ObjectEvent *objectEvent = ObjectEvent_GetById(id);

  objectEvent->layer = 2;
  objectEvent->motionFlags = 0;
#ifdef UBFIX
  objectEvent->update = CrossboneIsleSteppingStones_UpdateObjectEventPriority;
#else
  objectEvent->update = (void (*)(union ObjectEventOrEffect *))
      CrossboneIsleSteppingStones_UpdateObjectEventPriority;
#endif
}

/// Matches the leader's draw priority to a nearby pillar in front.
static void CrossboneIsleSteppingStones_UpdateLeaderPriority(void) {
  struct ObjectEvent *hero = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  u32 selector;
  struct ObjectEvent *objectEvent;

  for (selector = LOCALID_FIRST_PILLAR; selector <= LOCALID_FIRST_PILLAR + 3;
       selector++) {
    objectEvent = ObjectEvent_GetById(selector);
    if (hero->y.fixed / PIXELS(1) == objectEvent->y.fixed / PIXELS(1) &&
        hero->z.fixed <= objectEvent->z.fixed - PIXELS(8) &&
        hero->z.fixed > objectEvent->z.fixed - PIXELS(24)) {
      if (hero->x.fixed - PIXELS(16) <= objectEvent->x.fixed &&
          objectEvent->x.fixed < hero->x.fixed + PIXELS(16)) {
        ObjectMotion_SetActionVariant(LOCALID_PARTY_LEADER,
                                      objectEvent->sprite->image.priority);
        return;
      }
    } else {
      ObjectEvent_GetById(LOCALID_PARTY_LEADER)->priorityFlags |=
          OBJ_EVENT_PRIORITY_AUTOMATIC;
    }
  }
}

/// Restores each room's pillar footprints, water state, and persistent puzzle
/// state.
s32 Scene_Initialize(void) {
  struct ObjectEvent *objectEvent;
  u32 i;
  u32 id;
  s32 x;
  s32 z;

  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  if (gSaveBlock.sceneId == (s32)&SceneId_CrossboneIsleIcePillars) {
    Scene_RedrawObjectEventFootprint(LOCALID_FIRST_PILLAR);
    Scene_RedrawObjectEventFootprint(LOCALID_FIRST_PILLAR + 1);
    Scene_RedrawObjectEventFootprint(LOCALID_FIRST_PILLAR + 2);
    Scene_RedrawObjectEventFootprint(LOCALID_FIRST_PILLAR + 3);
    Scene_RedrawObjectEventFootprint(LOCALID_FIRST_PILLAR + 4);
  } else if (gSaveBlock.sceneId == (s32)&SceneId_CrossboneIsleFloodgate) {
    // Rebuild the flooded room and float the logs again when the water stayed
    // raised through a battle.
    MapGrid_CopyCells(32, 0, 64, 32, 0, 64);
    Scene_RedrawObjectEventFootprint(LOCALID_FIRST_LOG);
    Scene_RedrawObjectEventFootprint(LOCALID_FIRST_LOG + 1);
    Scene_RedrawObjectEventFootprint(LOCALID_FIRST_LOG + 2);
    Scene_RedrawObjectEventFootprint(LOCALID_FIRST_LOG + 3);
    Scene_RedrawObjectEventFootprint(LOCALID_FIRST_LOG + 4);
    Scene_RedrawObjectEventFootprint(LOCALID_FIRST_LOG + 5);
    Scene_RedrawObjectEventFootprint(LOCALID_FIRST_LOG + 6);
    Scene_RedrawObjectEventFootprint(LOCALID_FIRST_LOG + 7);
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) != 0 &&
        GameFlag_Get(FLAG_CROSSBONE_ISLE_FLOODGATE_WATER_RAISED) != 0) {
      MapGrid_CopyCellsTo(79, 34, 84, 24, 1, 2);
      MapGrid_CopyCellsTo(0, 32, 32, 0, 32, 32);
      MapGrid_CopyCellsTo(32, 32, 64, 0, 32, 32);
      CrossboneIsleSteppingStones_DrawFloatingLogs();
      MapGrid_CopyCellAttributes(24, 3, 1, 1, 24, 8);
    }
  } else if (gSaveBlock.sceneId == (s32)&SceneId_CrossboneIsleStackedPillars) {
    // Restore the four movable pillars and the leader's riding overlay.
    ObjectEvent_CreateConfiguredObject(PIXELS(584), 0, PIXELS(200),
                                       SPRITE_CRATE);
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
      ObjectEvent_GetById(LOCALID_PARTY_LEADER)->riseCounter = 1;
    }
    FieldEffect_StartFadeOverlay(0);
    if (ObjectEvent_GetById(LOCALID_PARTY_LEADER)->riseCounter == 0) {
      EventScript_StopRising();
    }
    CrossboneIsleSteppingStones_InitializePillarPriority(LOCALID_FIRST_PILLAR);
    CrossboneIsleSteppingStones_InitializePillarPriority(LOCALID_FIRST_PILLAR +
                                                         1);
    CrossboneIsleSteppingStones_InitializePillarPriority(LOCALID_FIRST_PILLAR +
                                                         2);
    CrossboneIsleSteppingStones_InitializePillarPriority(LOCALID_FIRST_PILLAR +
                                                         3);
    {
      s32 *entry = (s32 *)sPillarSlots;

      for (i = 0; i < 4; i++) {
        entry[0] = 0;
        entry[1] = 0;
        entry[2] = 0;
        entry[4] =
            i + FLAG_CROSSBONE_ISLE_STACKED_PILLARS_FIRST_PILLAR_UNDER_ANOTHER;
        entry += 5;
      }
    }
    EventScript_MovePillar();
    FieldScript_WaitFrames(1);
    FieldScheduler_AddCallback(CrossboneIsleSteppingStones_UpdateLeaderPriority,
                               TASK_PRIORITY_MAP);
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) != 0) {
      for (id = LOCALID_FIRST_PILLAR; id < LOCALID_FIRST_PILLAR + 4; id++) {
        objectEvent = ObjectEvent_GetById(id);
        x = objectEvent->x.fixed >> 20;
        if (x == 37) {
          z = objectEvent->z.fixed >> 20;
          if (z == 9) {
            MapGrid_CopyCellAttributes(27, 8, 1, 1, x, z);
            break;
          }
        }
      }
    }
  } else if (gSaveBlock.sceneId == (s32)&SceneId_CrossboneIsleDragonStatue) {
    // Restore CrossboneIsleDragonStatue's moved pillar, frozen puddles, and
    // raised pillar pair.
    Import_ObjectEventSetAnimation(LOCALID_SINKING_BOULDER, 2);
    ObjectEvent_ConfigureSpriteColor(LOCALID_SINKING_BOULDER,
                                     SPRITE_COLOR_GREY);
    Scene_RedrawObjectEventFootprint(LOCALID_FIRST_LOG);
    Scene_RedrawObjectEventFootprint(LOCALID_FIRST_LOG + 1);
#if TBS_PATCH_1
    ObjectEvent_GetById(LOCALID_FIRST_LOG)->motionFlags = 0;
    ObjectEvent_GetById(LOCALID_FIRST_LOG + 1)->motionFlags = 0;
#endif
    EventScript_MoveSinkingPillar();
    CrossboneIsleSteppingStones_PrepLoweredObjectEvent(LOCALID_FIRST_PUDDLE);
    CrossboneIsleSteppingStones_PrepLoweredObjectEvent(LOCALID_SECOND_PUDDLE);
    CrossboneIsleSteppingStones_PrepLoweredObjectEvent(LOCALID_THIRD_PUDDLE);
    CrossboneIsleSteppingStones_RestoreFlaggedPillar(LOCALID_FIRST_PUDDLE);
    CrossboneIsleSteppingStones_RestoreFlaggedPillar(LOCALID_SECOND_PUDDLE);
    CrossboneIsleSteppingStones_RestoreFlaggedPillar(LOCALID_THIRD_PUDDLE);
    objectEvent = ObjectEvent_GetById(LOCALID_THIRD_PUDDLE);
    objectEvent->update = 0;
    CrossboneIsleSteppingStones_PrepLoweredObjectEvent(LOCALID_RISING_PILLAR);
    ObjectEvent_GetById(LOCALID_RISING_PILLAR)->collisionFlags |=
        OBJ_EVENT_NOT_TARGETED;
    if (GameFlag_Get(FLAG_CROSSBONE_ISLE_DRAGON_STATUE_FIRST_PUDDLE_FROZEN +
                     2) == 0) {
      ObjectEvent_GetById(LOCALID_THIRD_PUDDLE)->scaleX = 3 * Q16_ONE / 2;
      ObjectEvent_GetById(LOCALID_THIRD_PUDDLE)->scaleY = 3 * Q16_ONE / 2;
      ObjectEvent_GetById(LOCALID_THIRD_PUDDLE)->sprite->image.priority = 3;
      ObjectEvent_GetById(LOCALID_RISING_PILLAR)->sprite->image.priority = 3;
      MapGrid_CopyCellAttributes(26, 12, 1, 1, 22, 16);
    }
  }
  return 0;
}

s32 gFieldEffect_ShortScript[] = {
    SC_WAIT(1),           SC_LOOP(22, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gFieldEffect_MediumScript[] = {
    SC_WAIT(1),           SC_LOOP(44, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gFieldEffect_LongScript[] = {
    SC_WAIT(1),           SC_LOOP(126, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),           SC_FREE,
};

/// Pixel step by direction: X in the upper halfword, Z in the lower.
#define DIRECTION_STEP(dx, dz) (((dx) << 16) | ((dz) & 0xffff))

s32 gPushedObject_DirectionSteps[] = {
    DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(16, 0),
};

// clang-format off
u32 gPushedObject_FootprintKinds[] = {
    207, 205, 228, 229, 298, 297,
};

s32 gPushedObject_FootprintBounds[] = {
    -32, -8, 32, 8,
    -8, -32, 8, 32,
    -32, -16, 32, 0,
    -8, -32, 8, 32,
    -32, -8, 32, 8,
    -8, -32, 8, 32,
};

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

/// The glint of a puddle Frost has frozen in CrossboneIsleDragonStatue: the
/// timer carries a colour mode to
/// CrossboneIsleSteppingStones_SetChildValueFromTimer, green and blue swapped
/// for two frames in every eight.
s32 gCrossboneIsleSteppingStones_ActionTable[] = {
    SC_SET(SW_TIMER, SPRITE_COLOR_RBG),
    SC_CALL(CrossboneIsleSteppingStones_SetChildValueFromTimer),
    SC_WAIT(2),
    SC_SET(SW_TIMER, SPRITE_COLOR_NORMAL),
    SC_CALL(CrossboneIsleSteppingStones_SetChildValueFromTimer),
    SC_WAIT(6),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// The floodgate's switch flipping as EventScript_FlipWaterSwitch closes the
/// gate, and flipping back as it opens it.
u16 gCrossboneIsleSteppingStones_SwitchCloseSteps[] = {
    MAPGRID_ANIMATION_STEP(76, 34, 1, 2, 4), MAPGRID_ANIMATION_STEP(77, 34, 1, 2, 4), MAPGRID_ANIMATION_STEP(78, 34, 1, 2, 4), MAPGRID_ANIMATION_STEP(79, 34, 1, 2, 4), MAPGRID_ANIMATION_STEPS_END,
};

u16 gCrossboneIsleSteppingStones_SwitchOpenSteps[] = {
    MAPGRID_ANIMATION_STEP(79, 34, 1, 2, 4), MAPGRID_ANIMATION_STEP(78, 34, 1, 2, 4), MAPGRID_ANIMATION_STEP(77, 34, 1, 2, 4), MAPGRID_ANIMATION_STEP(76, 34, 1, 2, 4), MAPGRID_ANIMATION_STEPS_END,
};

/// The floodgate closing, and opening again.
u16 gCrossboneIsleSteppingStones_FloodgateCloseSteps[] = {
    MAPGRID_ANIMATION_STEP(67, 34, 2, 5, 6), MAPGRID_ANIMATION_STEP(69, 34, 2, 5, 6), MAPGRID_ANIMATION_STEP(71, 34, 2, 5, 6), MAPGRID_ANIMATION_STEP(73, 34, 2, 5, 6), MAPGRID_ANIMATION_STEPS_END,
};

u16 gCrossboneIsleSteppingStones_FloodgateOpenSteps[] = {
    MAPGRID_ANIMATION_STEP(71, 50, 2, 5, 6), MAPGRID_ANIMATION_STEP(69, 50, 2, 5, 6), MAPGRID_ANIMATION_STEP(67, 50, 2, 5, 6), MAPGRID_ANIMATION_STEP(65, 50, 2, 5, 6), MAPGRID_ANIMATION_STEPS_END,
};

/// The two cells at the floodgate's foot as it shuts: three pictures of white
/// spray splashing up and falling, then the stream bed below the gate, left
/// almost dry (EventScript_FlipWaterSwitch's render).
u16 gCrossboneIsleSteppingStones_FloodgateFootSteps[] = {
    MAPGRID_ANIMATION_STEP(75, 38, 2, 1, 4), MAPGRID_ANIMATION_STEP(77, 38, 2, 1, 6), MAPGRID_ANIMATION_STEP(79, 38, 2, 1, 8), MAPGRID_ANIMATION_STEP(65, 53, 2, 1, 1), MAPGRID_ANIMATION_STEPS_END,
};

struct SceneEntrance gCrossboneIsleSteppingStonesEntrancesOther[] = {
    { 0, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { MAP_TABLE_END },
};

struct SceneEntrance gCrossboneIsleSteppingStonesEntrances[] = {
    { CROSSBONE_ISLE_STEPPING_STONES_FROM_CROSSBONE_ISLE_FIRST_FLOOR_EAST, CONDITION_ALWAYS, 712, 0, 760, FACING_NORTH, 0, 560, 368, 992, 792, 0 },
    { CROSSBONE_ISLE_STEPPING_STONES_FROM_CROSSBONE_ISLE_FIRST_FLOOR_WEST, CONDITION_ALWAYS, 600, 0, 760, FACING_NORTH, 0, 560, 368, 992, 792, 0 },
    { MAP_TABLE_END },
};

struct SceneEntrance gCrossboneIsleIcePillarsEntrances[] = {
    { CROSSBONE_ISLE_ICE_PILLARS_FROM_CROSSBONE_ISLE_SECOND_FLOOR_EAST, CONDITION_ALWAYS, 216, 0, 360, FACING_NORTH, 0, 32, 32, 384, 400, 0 },
    { CROSSBONE_ISLE_ICE_PILLARS_FROM_CROSSBONE_ISLE_SECOND_FLOOR_WEST, CONDITION_ALWAYS, 104, 0, 360, FACING_NORTH, 0, 32, 32, 384, 400, 0 },
    { MAP_TABLE_END },
};

struct SceneEntrance gCrossboneIsleFloodgateEntrances[] = {
    { CROSSBONE_ISLE_FLOODGATE_FROM_CROSSBONE_ISLE_FIFTH_FLOOR_EAST, CONDITION_ALWAYS, 184, 0, 472, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { CROSSBONE_ISLE_FLOODGATE_FROM_CROSSBONE_ISLE_FIFTH_FLOOR_WEST, CONDITION_ALWAYS, 64, 0, 472, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { MAP_TABLE_END },
};

struct SceneEntrance gCrossboneIsleStackedPillarsEntrances[] = {
    { CROSSBONE_ISLE_STACKED_PILLARS_FROM_CROSSBONE_ISLE_SEVENTH_FLOOR_EAST, CONDITION_ALWAYS, 744, 0, 520, FACING_NORTH, 0, 440, 16, 960, 544, 0 },
    { CROSSBONE_ISLE_STACKED_PILLARS_FROM_CROSSBONE_ISLE_SEVENTH_FLOOR_WEST, CONDITION_ALWAYS, 632, 0, 520, FACING_NORTH, 0, 440, 16, 960, 544, 0 },
    { MAP_TABLE_END },
};

struct SceneEntrance gCrossboneIsleDragonStatueEntrances[] = {
    { CROSSBONE_ISLE_DRAGON_STATUE_FROM_CROSSBONE_ISLE_EIGHTH_FLOOR_EAST, CONDITION_ALWAYS, 280, 0, 488, FACING_NORTH, 0, 16, 32, 464, 528, 0 },
    { CROSSBONE_ISLE_DRAGON_STATUE_FROM_CROSSBONE_ISLE_EIGHTH_FLOOR_WEST, CONDITION_ALWAYS, 168, 0, 488, FACING_NORTH, 0, 16, 32, 464, 528, 0 },
    { MAP_TABLE_END },
};

u32 gCrossboneIsleSteppingStonesExits[] = {
    SCENE_EXITS(CrossboneIsleSteppingStones),
    SCENE_EXIT(CROSSBONE_ISLE_STEPPING_STONES_TO_CROSSBONE_ISLE_FIRST_FLOOR_EAST, CrossboneIsleFirstFloor, CROSSBONE_ISLE_FIRST_FLOOR_FROM_CROSSBONE_ISLE_STEPPING_STONES_EAST),
    SCENE_EXIT(CROSSBONE_ISLE_STEPPING_STONES_TO_CROSSBONE_ISLE_FIRST_FLOOR_WEST, CrossboneIsleFirstFloor, CROSSBONE_ISLE_FIRST_FLOOR_FROM_CROSSBONE_ISLE_STEPPING_STONES_WEST),
    SCENE_EXITS(CrossboneIsleIcePillars),
    SCENE_EXIT(CROSSBONE_ISLE_ICE_PILLARS_TO_CROSSBONE_ISLE_SECOND_FLOOR_EAST, CrossboneIsleSecondFloor, CROSSBONE_ISLE_SECOND_FLOOR_FROM_CROSSBONE_ISLE_ICE_PILLARS_EAST),
    SCENE_EXIT(CROSSBONE_ISLE_ICE_PILLARS_TO_CROSSBONE_ISLE_SECOND_FLOOR_WEST, CrossboneIsleSecondFloor, CROSSBONE_ISLE_SECOND_FLOOR_FROM_CROSSBONE_ISLE_ICE_PILLARS_WEST),
    SCENE_EXITS(CrossboneIsleFloodgate),
    SCENE_EXIT(CROSSBONE_ISLE_FLOODGATE_TO_CROSSBONE_ISLE_FIFTH_FLOOR_EAST, CrossboneIsleFifthFloor, CROSSBONE_ISLE_FIFTH_FLOOR_FROM_CROSSBONE_ISLE_FLOODGATE_EAST),
    SCENE_EXIT(CROSSBONE_ISLE_FLOODGATE_TO_CROSSBONE_ISLE_FIFTH_FLOOR_WEST, CrossboneIsleFifthFloor, CROSSBONE_ISLE_FIFTH_FLOOR_FROM_CROSSBONE_ISLE_FLOODGATE_WEST),
    SCENE_EXITS(CrossboneIsleStackedPillars),
    SCENE_EXIT(CROSSBONE_ISLE_STACKED_PILLARS_TO_CROSSBONE_ISLE_SEVENTH_FLOOR_EAST, CrossboneIsleSeventhFloor, CROSSBONE_ISLE_SEVENTH_FLOOR_FROM_CROSSBONE_ISLE_STACKED_PILLARS_EAST),
    SCENE_EXIT(CROSSBONE_ISLE_STACKED_PILLARS_TO_CROSSBONE_ISLE_SEVENTH_FLOOR_WEST, CrossboneIsleSeventhFloor, CROSSBONE_ISLE_SEVENTH_FLOOR_FROM_CROSSBONE_ISLE_STACKED_PILLARS_WEST),
    SCENE_EXITS(CrossboneIsleDragonStatue),
    SCENE_EXIT(CROSSBONE_ISLE_DRAGON_STATUE_TO_CROSSBONE_ISLE_EIGHTH_FLOOR_EAST, CrossboneIsleEighthFloor, CROSSBONE_ISLE_EIGHTH_FLOOR_FROM_CROSSBONE_ISLE_DRAGON_STATUE_EAST),
    SCENE_EXIT(CROSSBONE_ISLE_DRAGON_STATUE_TO_CROSSBONE_ISLE_EIGHTH_FLOOR_WEST, CrossboneIsleEighthFloor, CROSSBONE_ISLE_EIGHTH_FLOOR_FROM_CROSSBONE_ISLE_DRAGON_STATUE_WEST),
    SCENE_EXITS_END,
};

/// Unused: an empty object event template table nothing returns.
struct ObjectEventTemplate gCrossboneIsleSteppingStonesNoObjectEventTemplates[] = {
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gCrossboneIsleSteppingStonesObjectEventTemplates[] = {
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(744), 0, PIXELS(488), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(792), 0, PIXELS(552), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(696), 0, PIXELS(616), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(776), 0, PIXELS(680), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gCrossboneIsleIcePillarsObjectEventTemplates[] = {
    { 0x12a, CONDITION_ALWAYS, 7,
        PIXELS(144), 0, PIXELS(200), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { 0x12a, CONDITION_ALWAYS, 7,
        PIXELS(256), 0, PIXELS(200), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { 0x12a, CONDITION_ALWAYS, 7,
        PIXELS(224), 0, PIXELS(264), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { 0x129, CONDITION_ALWAYS, 7,
        PIXELS(280), 0, PIXELS(160), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { 0x129, CONDITION_ALWAYS, 7,
        PIXELS(184), 0, PIXELS(240), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gCrossboneIsleFloodgateObjectEventTemplates[] = {
    { SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(144), 0, PIXELS(296), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(232), 0, PIXELS(256), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(208), 0, PIXELS(216), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(232), 0, PIXELS(176), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(272), 0, PIXELS(168), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(352), 0, PIXELS(168), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { 0xd6, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gCrossboneIsleSteppingStonesObjectEventTemplatesOther[] = {
    { 0x100, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(648), 0, PIXELS(184), FACING_NORTH, TALK_FACING_KEEP, 0 },
    { 0x100, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0 },
    { 0x100, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(680), 0, PIXELS(392), FACING_NORTH, TALK_FACING_KEEP, 0 },
    { 0x100, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gCrossboneIsleDragonStatueObjectEventTemplates[] = {
    { SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(208), 0, PIXELS(264), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(224), 0, PIXELS(280), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { 0x125, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(232), 0, PIXELS(296), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(136), 0, PIXELS(216), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(296), 0, PIXELS(216), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(360), 0, PIXELS(264), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { 0x122, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(360), 0, PIXELS(264), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

struct MapEvent gCrossboneIsleSteppingStonesEventsOther[] = {
    { MAP_EVENTS_END },
};

struct MapEvent gCrossboneIsleSteppingStonesEvents[] = {
    { MAP_EVENT_EXIT, 1, CONDITION_ALWAYS, CROSSBONE_ISLE_STEPPING_STONES_TO_CROSSBONE_ISLE_FIRST_FLOOR_EAST },
    { MAP_EVENT_EXIT, 2, CONDITION_ALWAYS, CROSSBONE_ISLE_STEPPING_STONES_TO_CROSSBONE_ISLE_FIRST_FLOOR_WEST },
    { MAP_EVENT_COORD | COORD_EVENT_FLAG_WALKING, 10, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_JumpOrPushPillar) },
    { BG_EVENT(BG_EVENT_CHEST), 100, FLAG_CROSSBONE_ISLE_STEPPING_STONES_ANTIDOTE, FIND_ITEM(ITEM_ANTIDOTE) },
    { BG_EVENT(BG_EVENT_CHEST), 101, FLAG_CROSSBONE_ISLE_STEPPING_STONES_NUT, FIND_ITEM(ITEM_NUT) },
    { BG_EVENT(BG_EVENT_CHEST), 102, FLAG_CROSSBONE_ISLE_STEPPING_STONES_111_COINS, FIND_COINS(111) },
    { BG_EVENT(BG_EVENT_CHEST), 103, FLAG_CROSSBONE_ISLE_STEPPING_STONES_HARD_NUT, FIND_ITEM(ITEM_HARD_NUT) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_FIRST_PILLAR, CONDITION_ALWAYS, 0 },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_FIRST_PILLAR + 1, CONDITION_ALWAYS, 0 },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_FIRST_PILLAR + 2, CONDITION_ALWAYS, 0 },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_FIRST_PILLAR + 3, CONDITION_ALWAYS, 0 },
    { MAP_EVENTS_END },
};

struct MapEvent gCrossboneIsleIcePillarsEvents[] = {
    { MAP_EVENT_EXIT, 1, CONDITION_ALWAYS, CROSSBONE_ISLE_ICE_PILLARS_TO_CROSSBONE_ISLE_SECOND_FLOOR_EAST },
    { MAP_EVENT_EXIT, 2, CONDITION_ALWAYS, CROSSBONE_ISLE_ICE_PILLARS_TO_CROSSBONE_ISLE_SECOND_FLOOR_WEST },
    { MAP_EVENT_COORD | COORD_EVENT_FLAG_WALKING, 10, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_PushPillar) },
    { BG_EVENT(BG_EVENT_CHEST), 100, FLAG_CROSSBONE_ISLE_ICE_PILLARS_LUCKY_PEPPER, FIND_ITEM(ITEM_LUCKY_PEPPER) },
    { BG_EVENT(BG_EVENT_CHEST), 101, FLAG_CROSSBONE_ISLE_ICE_PILLARS_MYSTERY_BLADE, FIND_ITEM(ITEM_MYSTERY_BLADE) },
    { BG_EVENT(BG_EVENT_CHEST), 102, FLAG_CROSSBONE_ISLE_ICE_PILLARS_222_COINS, FIND_COINS(222) },
    { BG_EVENT(BG_EVENT_CHEST), 103, FLAG_CROSSBONE_ISLE_ICE_PILLARS_ELIXIR, FIND_ITEM(ITEM_ELIXIR) },
    { MAP_EVENTS_END },
};

struct MapEvent gCrossboneIsleFloodgateEvents[] = {
    { MAP_EVENT_EXIT, 1, CONDITION_ALWAYS, CROSSBONE_ISLE_FLOODGATE_TO_CROSSBONE_ISLE_FIFTH_FLOOR_EAST },
    { MAP_EVENT_EXIT, 2, CONDITION_ALWAYS, CROSSBONE_ISLE_FLOODGATE_TO_CROSSBONE_ISLE_FIFTH_FLOOR_WEST },
    { MAP_EVENT_COORD | COORD_EVENT_FLAG_WALKING, 10, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_PushPillar) },
    { BG_EVENT_CHEST_FACING(FACING_EAST), 100, FLAG_CROSSBONE_ISLE_FLOODGATE_555_COINS, FIND_COINS(555) },
    { BG_EVENT_CHEST_FACING(FACING_NORTH), 100, FLAG_CROSSBONE_ISLE_FLOODGATE_555_COINS, FIND_COINS(555) },
    { BG_EVENT_CHEST_FACING(FACING_EAST), 101, FLAG_CROSSBONE_ISLE_FLOODGATE_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { BG_EVENT_CHEST_FACING(FACING_NORTHEAST), 101, FLAG_CROSSBONE_ISLE_FLOODGATE_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { BG_EVENT(BG_EVENT_CHEST), 102, FLAG_CROSSBONE_ISLE_FLOODGATE_POTION, FIND_ITEM(ITEM_POTION) },
    { BG_EVENT(BG_EVENT_CHEST), 103, FLAG_CROSSBONE_ISLE_FLOODGATE_NINJA_GARB, FIND_ITEM(ITEM_NINJA_GARB) },
    { BG_EVENT(BG_EVENT_SILENT), 20, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_FlipWaterSwitch) },
    { MAP_EVENTS_END },
};

struct MapEvent gCrossboneIsleStackedPillarsEvents[] = {
    { MAP_EVENT_EXIT, 1, CONDITION_ALWAYS, CROSSBONE_ISLE_STACKED_PILLARS_TO_CROSSBONE_ISLE_SEVENTH_FLOOR_EAST },
    { MAP_EVENT_EXIT, 2, CONDITION_ALWAYS, CROSSBONE_ISLE_STACKED_PILLARS_TO_CROSSBONE_ISLE_SEVENTH_FLOOR_WEST },
    { MAP_EVENT_COORD | COORD_EVENT_FLAG_WALKING, 30, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_JumpOrPushStackedPillar) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 31, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ClimbUp) },
    { COORD_EVENT_WALKING_INTO(FACING_SOUTH), 31, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_PushOrClimbDown) },
    { MAP_EVENT_COORD | COORD_EVENT_FLAG_WALKING, 31, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_JumpOrPushStackedPillar) },
    { MAP_EVENT_COORD, 40, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_StartRising) },
    { MAP_EVENT_COORD, 41, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_StopRising) },
    { BG_EVENT(BG_EVENT_CHEST), 100, FLAG_CROSSBONE_ISLE_STACKED_PILLARS_777_COINS, FIND_COINS(777) },
    { BG_EVENT(BG_EVENT_CHEST), 101, FLAG_CROSSBONE_ISLE_STACKED_PILLARS_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { BG_EVENT_CHEST_FACING(FACING_EAST), 102, FLAG_CROSSBONE_ISLE_STACKED_PILLARS_PSY_CRYSTAL, FIND_ITEM(ITEM_PSY_CRYSTAL) },
    { BG_EVENT(BG_EVENT_CHEST), 103, FLAG_CROSSBONE_ISLE_STACKED_PILLARS_WICKED_MACE, FIND_ITEM(ITEM_WICKED_MACE) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_FIRST_PILLAR, FLAG_CROSSBONE_ISLE_STACKED_PILLARS_FIRST_PILLAR_UNDER_ANOTHER, EVENT_SCRIPT(EventScript_MovePillar) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_FIRST_PILLAR + 1, FLAG_CROSSBONE_ISLE_STACKED_PILLARS_FIRST_PILLAR_UNDER_ANOTHER + 1, EVENT_SCRIPT(EventScript_MovePillar) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_FIRST_PILLAR + 2, FLAG_CROSSBONE_ISLE_STACKED_PILLARS_FIRST_PILLAR_UNDER_ANOTHER + 2, EVENT_SCRIPT(EventScript_MovePillar) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_FIRST_PILLAR + 3, FLAG_CROSSBONE_ISLE_STACKED_PILLARS_FIRST_PILLAR_UNDER_ANOTHER + 3, EVENT_SCRIPT(EventScript_MovePillar) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_CARRY), LOCALID_FIRST_PILLAR, FLAG_CROSSBONE_ISLE_STACKED_PILLARS_FIRST_PILLAR_UNDER_ANOTHER, EVENT_SCRIPT(EventScript_MovePillar) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_CARRY), LOCALID_FIRST_PILLAR + 1, FLAG_CROSSBONE_ISLE_STACKED_PILLARS_FIRST_PILLAR_UNDER_ANOTHER + 1, EVENT_SCRIPT(EventScript_MovePillar) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_CARRY), LOCALID_FIRST_PILLAR + 2, FLAG_CROSSBONE_ISLE_STACKED_PILLARS_FIRST_PILLAR_UNDER_ANOTHER + 2, EVENT_SCRIPT(EventScript_MovePillar) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_CARRY), LOCALID_FIRST_PILLAR + 3, FLAG_CROSSBONE_ISLE_STACKED_PILLARS_FIRST_PILLAR_UNDER_ANOTHER + 3, EVENT_SCRIPT(EventScript_MovePillar) },
    { MAP_EVENTS_END },
};
// clang-format on

struct MapEvent gCrossboneIsleDragonStatueEvents[] = {
    {MAP_EVENT_EXIT, 1, CONDITION_ALWAYS,
     CROSSBONE_ISLE_DRAGON_STATUE_TO_CROSSBONE_ISLE_EIGHTH_FLOOR_EAST},
    {MAP_EVENT_EXIT, 2, CONDITION_ALWAYS,
     CROSSBONE_ISLE_DRAGON_STATUE_TO_CROSSBONE_ISLE_EIGHTH_FLOOR_WEST},
    {MAP_EVENT_COORD | COORD_EVENT_FLAG_WALKING, 10, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_PushSinkingPillar)},
    {BG_EVENT(BG_EVENT_CHEST), 100, FLAG_CROSSBONE_ISLE_DRAGON_STATUE_888_COINS,
     FIND_COINS(888)},
    {BG_EVENT(BG_EVENT_CHEST), 101, FLAG_CROSSBONE_ISLE_DRAGON_STATUE_POTION,
     FIND_ITEM(ITEM_POTION)},
    {BG_EVENT(BG_EVENT_CHEST), 102,
     FLAG_CROSSBONE_ISLE_DRAGON_STATUE_CLERICS_RING,
     FIND_ITEM(ITEM_CLERICS_RING)},
    {BG_EVENT(BG_EVENT_CHEST), 103,
     FLAG_CROSSBONE_ISLE_DRAGON_STATUE_SMOKE_BOMB, FIND_ITEM(ITEM_SMOKE_BOMB)},
    {PSYNERGY_PHASE_ON_OBJ_EVENT(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     LOCALID_SINKING_BOULDER, FLAG_CROSSBONE_ISLE_DRAGON_STATUE_BOULDER_SUNK,
     EVENT_SCRIPT(EventScript_StartMovingSinkingPillar)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_SINKING_BOULDER,
     FLAG_CROSSBONE_ISLE_DRAGON_STATUE_BOULDER_SUNK,
     EVENT_SCRIPT(EventScript_MoveSinkingPillar)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_FROST), LOCALID_FIRST_PUDDLE,
     FLAG_CROSSBONE_ISLE_DRAGON_STATUE_FIRST_PUDDLE_FROZEN,
     EVENT_SCRIPT(EventScript_FreezeFirstPuddle)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_FROST), LOCALID_SECOND_PUDDLE,
     FLAG_CROSSBONE_ISLE_DRAGON_STATUE_FIRST_PUDDLE_FROZEN + 1,
     EVENT_SCRIPT(EventScript_FreezeSecondPuddle)},
    {PSYNERGY_PHASE_ON_OBJ_EVENT(ABILITY_FROST, PSYNERGY_PHASE_BEFORE_CAST),
     LOCALID_THIRD_PUDDLE,
     FLAG_CROSSBONE_ISLE_DRAGON_STATUE_FIRST_PUDDLE_FROZEN + 2,
     EVENT_SCRIPT(EventScript_FrostStartsOnThirdPuddle)},
    {PSYNERGY_PHASE_ON_OBJ_EVENT(ABILITY_FROST, PSYNERGY_PHASE_EFFECT_ACTS),
     LOCALID_THIRD_PUDDLE,
     FLAG_CROSSBONE_ISLE_DRAGON_STATUE_FIRST_PUDDLE_FROZEN + 2,
     EVENT_SCRIPT(EventScript_FrostRaisesPillar)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_FROST), LOCALID_THIRD_PUDDLE,
     FLAG_CROSSBONE_ISLE_DRAGON_STATUE_FIRST_PUDDLE_FROZEN + 2,
     EVENT_SCRIPT(EventScript_FreezeThirdPuddle)},
    {MAP_EVENTS_END},
};
