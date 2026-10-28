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
///   cells allow, and otherwise pushes it
///   (CrossboneIsleSteppingStones_JumpOrPushPillar).
/// - SceneId_CrossboneIsleIcePillars, off the second floor: pillars of ice to
///   push, standing and lying across a floor of spikes.
/// - SceneId_CrossboneIsleFloodgate, off the fifth floor: a cave with a stream
///   and a floodgate across it, and seven logs to roll. The switch beside the
///   floodgate (CrossboneIsleSteppingStones_FlipWaterSwitch) closes it, so the
///   water backs up and floats the logs higher, or opens it to drain them back
///   down; the water level lasts only while the party stays
///   (FLAG_CROSSBONE_ISLE_FLOODGATE_WATER_RAISED, kept through a battle).
/// - SceneId_CrossboneIsleStackedPillars, off the seventh floor, a cave of
/// cliffs
///   and ladders: four pillars that Move and Carry shift, and that settle and
///   redraw their footprints after every push
///   (CrossboneIsleSteppingStones_MovePillar), a ladder, and pillars to jump
///   across.
/// - SceneId_CrossboneIsleDragonStatue, off the eighth floor, by a dragon
/// statue:
///   three puddles that Frost freezes into pillars of ice. Frost on the third
///   makes the pillar in it rise and thaws a frozen second puddle, and the
///   third freezes only if the leader was not standing on it; a boulder moved
///   with Move sinks into place at column 16.
///
/// The other three side rooms are in crossbone_isle_keys.c, and the ninth
/// floor's in crossbone_isle.c.

#include "script_operands.h"
#include "adventure.h"
#include "map_grid_scroll.h"
#include "scene_ids.h"
#include "unit/import.h"
#include "unit/scene/crossbone_isle_stepping_stones.h"
#include "unit/scene/shared/effect_spawn.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/event_commands.h"
#include "unit/field/field.h"
#include "unit/runtime/runtime.h"
#include "actor.h"
#include "types.h"
#include "animspr.h"
#include "event_layer.h"
#include "scene_tables.h"
#include "field_effect.h"
#include "create_configured_actor.h"
#include "pushed_actor.h"
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
#include "event_inline.h"
#include "irq.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "actor_script.h"
#include "heap.h"
#include "scene.h"
#include "actor_effect.h"
#include "fixed_math.h"
#include "flags.h"
#include "songs.h"
#include "audio_engine_symbols.h"
#include "macros.h"

/// A chest searched while facing the given way.
#define EXAMINE_CHEST_FACING(facing)                                           \
  (EXAMINE_TRIGGER(EXAMINE_CHEST) | TRIGGER_FLAG_FACING | (facing))

/// The placed actors, numbered in the order of each room's actor
/// templates (Scene_GetActorTemplates).
enum CrossboneIsleSteppingStonesActorId {
  /// The pillars of CrossboneIsleSteppingStones (four, SPRITE_STONE_PILLAR),
  /// CrossboneIsleIcePillars (five) and CrossboneIsleStackedPillars (four,
  /// sprite 0x100), the only actors those rooms place.
  /// ★ Named from what the player sees: tan stone pillars over the pit,
  /// pillars of ice on the spikes, and grey carved pillars stacked two high
  /// (its render)
  ACTOR_FIRST_PILLAR = ACTOR_FIRST_PLACED,
  /// The logs, SPRITE_LOG_NORTH_SOUTH and SPRITE_LOG_WEST_EAST as Kolima
  /// Forest's: eight in
  /// CrossboneIsleFloodgate, where the seven after the first float on the
  /// raised water, and two in CrossboneIsleDragonStatue.
  /// ★ Named from what the player sees: logs lying on the cave floor (its
  /// render)
  ACTOR_FIRST_LOG = ACTOR_FIRST_PLACED,
  /// CrossboneIsleDragonStatue's boulder for Move, sprite 0x125, which sinks
  /// into place at column 16.
  /// ★ Named from what the player sees: a round grey boulder below the logs
  /// (its render)
  ACTOR_SINKING_BOULDER = ACTOR_FIRST_LOG + 2,
  /// CrossboneIsleDragonStatue's three puddles, SPRITE_PUDDLE, which Frost
  /// freezes into pillars of ice.
  /// ★ Named from what the player sees: a small blue puddle on the west ledge
  /// (its render)
  ACTOR_FIRST_PUDDLE,
  /// ★ Named from what the player sees: a small blue puddle on the middle
  /// ledge (its render)
  ACTOR_SECOND_PUDDLE,
  /// ★ Named from what the player sees: the puddle by the dragon statue,
  /// drawn frozen around its pillar (its render)
  ACTOR_THIRD_PUDDLE,
  /// The pillar in CrossboneIsleDragonStatue's third puddle, sprite 0x122,
  /// which rises 32 pixels as Frost freezes the puddle.
  /// ★ Named from what the player sees: a narrow dark pillar standing in the
  /// third puddle (its render)
  ACTOR_RISING_PILLAR,
  /// CrossboneIsleFloodgate's water, sprite 0xd6, raised behind the closed
  /// floodgate.
  /// ★ Named from what the player sees: the stream's green water, which the
  /// closed floodgate backs up (its render)
  ACTOR_WATER = ACTOR_FIRST_LOG + 8
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

#include "shared/effect_create.h"
#include "shared/effect_spawn.h"
#include "shared/staged_actor.h"

#include "shared/map_grid_panel.h"

/// Waits up to sixty frames for landing, then stops vertical movement on the
/// ground.
static void
CrossboneIsleSteppingStones_SettleActorOnGround(struct Actor *actor) {
  s32 remainingFrames = 60;

  for (;;) {
    if (remainingFrames == 0) {
      break;
    }
    WaitFrames(1);
    if ((u32)actor->y.fixed == (u32)actor->groundHeight) {
      break;
    }
    remainingFrames--;
  }

  actor->velocityY = 0;
  actor->targetY = ACTOR_NO_TARGET;
  actor->y.fixed = actor->groundHeight;
}

/// Waits until the actor reaches the ground or height limit, then stops
/// its descent.
static void
CrossboneIsleSteppingStones_StopAtGroundOrHeight(struct Actor *actor,
                                                 s32 limit) {
  s32 remainingFrames = 60;

  for (;;) {
    if (remainingFrames == 0) {
      break;
    }
    WaitFrames(1);
    if (actor->y.fixed <= actor->groundHeight) {
      break;
    }
    if (actor->y.fixed <= limit) {
      break;
    }
    remainingFrames--;
  }

  actor->velocityY = 0;
  actor->targetY = ACTOR_NO_TARGET;
}

/// Finds the actor's footprint dimensions, grid position, and map-layer
/// origin.
static s32 CrossboneIsleSteppingStones_QueryFootprint(
    s32 id, u32 *width, u32 *depth, struct PushedActorProbe *probe,
    s32 *layerOriginX, s32 *layerOriginZ) {
  struct MapGridScrollWork *grid = gMapGridWork[0];
  struct Actor *actor = Actor_GetById(id);
  u32 i;
  s32 coordinate;
  s32 farExtent;

  for (i = 0; i <= 5; i++) {
    if (actor->sprite->entries[0]->animationId ==
        gPushedActor_FootprintKinds[i]) {
      probe->footprintIndex = i;
      break;
    }
    probe->footprintIndex = 7;
  }
  if ((u32)probe->footprintIndex > 6) {
    return 0;
  }
  // Bounds are pixel offsets; dimensions and origins below are 16-pixel cells.
  probe->position.x = actor->x.fixed;
  probe->position.y = actor->y.fixed;
  probe->position.z = actor->z.fixed;
  coordinate = gPushedActor_FootprintBounds[probe->footprintIndex * 4 + 1];
  if (coordinate < 0) {
    coordinate = -coordinate;
  }
  farExtent = gPushedActor_FootprintBounds[probe->footprintIndex * 4 + 3];
  if (farExtent < 0) {
    farExtent = -farExtent;
  }
  *depth = (coordinate + farExtent) >> 4;
  coordinate = gPushedActor_FootprintBounds[probe->footprintIndex * 4];
  if (coordinate < 0) {
    coordinate = -coordinate;
  }
  farExtent = gPushedActor_FootprintBounds[probe->footprintIndex * 4 + 2];
  if (farExtent < 0) {
    farExtent = -farExtent;
  }
  *width = (coordinate + farExtent) >> 4;
  // Move from the actor's center to the footprint's top-left cell.
  probe->position.x +=
      gPushedActor_FootprintBounds[probe->footprintIndex * 4] * PIXELS(1);
  {
    s32 offsetZ =
        gPushedActor_FootprintBounds[probe->footprintIndex * 4 + 1] * PIXELS(1);

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
/// water, lying across or along the room as the log does, and puts its
/// actor under the leader.
static s32 CrossboneIsleSteppingStones_DrawRaisedFootprint(s32 actorId) {
  struct Actor *actor = Actor_GetById(actorId);
  u32 width, depth;
  s32 layerOriginX, layerOriginZ;
  struct PushedActorProbe probe;
  s32 cellX, cellZ;
  u8 mask;

  if (CrossboneIsleSteppingStones_QueryFootprint(
          actorId, &width, &depth, &probe, &layerOriginX, &layerOriginZ) == 0) {
    return 0;
  }

  cellX = probe.position.x;
  cellZ = probe.position.z;
  MapGrid_CopyCellAttributes(2, 2, width, depth, cellX, cellZ);

  Actor_SelectAnimation(actor, 4);
  mask = 2;
  actor->priorityFlags |= mask;

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
  CrossboneIsleSteppingStones_DrawRaisedFootprint(ACTOR_FIRST_LOG + 1);
  CrossboneIsleSteppingStones_DrawRaisedFootprint(ACTOR_FIRST_LOG + 2);
  CrossboneIsleSteppingStones_DrawRaisedFootprint(ACTOR_FIRST_LOG + 3);
  CrossboneIsleSteppingStones_DrawRaisedFootprint(ACTOR_FIRST_LOG + 4);
  CrossboneIsleSteppingStones_DrawRaisedFootprint(ACTOR_FIRST_LOG + 5);
  CrossboneIsleSteppingStones_DrawRaisedFootprint(ACTOR_FIRST_LOG + 6);
  CrossboneIsleSteppingStones_DrawRaisedFootprint(ACTOR_FIRST_LOG + 7);
}

/// Restores the ground metatiles and cell behaviors under the
/// actor's footprint.
static s32 CrossboneIsleSteppingStones_RestoreGroundFootprint(s32 actorId) {
  struct Actor *actor = Actor_GetById(actorId);
  u32 width, depth;
  s32 layerOriginX, layerOriginZ;
  struct PushedActorProbe probe;

  if (CrossboneIsleSteppingStones_QueryFootprint(
          actorId, &width, &depth, &probe, &layerOriginX, &layerOriginZ) == 0) {
    return 0;
  }

  {
    s32 sourceCellX = layerOriginX + probe.position.x;
    s32 sourceCellZ = layerOriginZ + probe.position.z;

    MapGrid_CopyCellAttributes(sourceCellX, sourceCellZ, width, depth,
                               probe.position.x, probe.position.z);
    PushedActor_FillGridAttributeRectangle(0, probe.position.x,
                                           probe.position.z, width, depth,
                                           CELL_BEHAVIOR_WALL);
  }

  Actor_SelectAnimation(actor, 1);
  actor->priorityFlags &= ~ACTOR_PRIORITY_UNDERFOOT;

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

/// Copies the cell's flags and cell behavior without replacing its
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

/// Finds the next terrain ledge and stops the falling actor at its
/// projected height.
static s32 CrossboneIsleSteppingStones_LowerActorToLedge(s32 id, s32 far) {
  struct Actor *actor = Actor_GetById(id);
  s32 found;
  struct Actor *temporaryActor;
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
  temporaryActor = NULL;
  Actor_SetSpriteFlags(actor, found);
  height = MapGrid_GetTerrainHeight(2, actor->x.fixed, actor->z.fixed);
  level = height / PIXELS(16);
  rows = level;
  if (level < 0) {
    rows = -level;
  }
  rows++;
  for (row = 0; row <= rows; row++) {
    top = MapGrid_GetTerrainHeight(actor->layer, actor->x.fixed,
                                   (row << 20) + actor->z.fixed);
    top /= PIXELS(16);
    if (level < top) {
      x = ((actor->x.fixed >> 20) << 20) + PIXELS(8);
      if (far == 0) {
        row += 2;
        z = ((row + (actor->z.fixed >> 20)) << 20) + PIXELS(2);
        y = top << 20;
        kind = 223;
      } else {
        row += 3;
        z = ((row + (actor->z.fixed >> 20)) << 20) - PIXELS(2);
        y = top << 20;
        kind = 253;
      }
      /* FAKEMATCH(one-pass): the block keeps found's constant after the height
       * arithmetic; as no block, 1 of 117 instructions differ in tbs-en. */
      do {
        temporaryActor = Actor_Prepare(x, y, z, kind);
        height = actor->z.fixed - z + y;
      } while (0);
      found = 1;
      break;
    }
  }
  CrossboneIsleSteppingStones_StopAtGroundOrHeight(actor, height);
  actor->x.fixed = 0;
  actor->y.fixed = 0;
  actor->z.fixed = 0;
  if (temporaryActor != 0) {
    Actor_Release(temporaryActor);
  }
  return found;
}

/// Shrinks the pillar and lowers its height for up to thirty-two frames.
static void CrossboneIsleSteppingStones_ShrinkPillar(s32 index) {
  struct Actor *actor = Actor_GetById(index);
  u32 elapsedFrames;

  actor->motionFlags = 0;

  elapsedFrames = 0;
  for (;;) {
    if (elapsedFrames > 31)
      return;
    WaitFrames(1);
    actor->scaleY += -Q16_ONE / 10;
    actor->y.fixed += -PIXELS(4) / 5;
    elapsedFrames++;
    if (actor->scaleY <= Q16_ONE / 10 - 1) {
      actor->scaleY = Q16_ONE / 10;
      return;
    }
  }
}

/// Unused: spawns a particle near the leader once every four frames; nothing
/// calls it.
UNUSED static void CrossboneIsleSteppingStones_SpawnUnusedLeaderParticle(void) {

  struct Actor *leader;
  s32 cycleFrame;
  s32 x;
  s32 z;
  s32 velocityY;
  s32 drawX;
  s32 drawZ;
  s32 drawY;
  struct EffectOptions *settings;
  struct EffectOptions options;

  leader = Actor_GetById(ACTOR_PARTY_LEADER);
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
  struct Actor *leader;
  u8 *flags;
  s32 savedMotionFlags;
  s32 jumpDirection;
#if !TBS_PATCH_1
  s32 step;
#endif
  struct FieldPosition *position;
  struct FieldPosition destination;

  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  flags = &leader->motionFlags;
  savedMotionFlags = *flags;
#if TBS_PATCH_1
  jumpDirection = (leader->facing + 0x2000) & 0xc000;
#endif
  if (gAdventure.movementMode != LEADER_MOVEMENT_WALK) {
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
  step = gPushedActor_DirectionSteps[jumpDirection];
  position->x = leader->x.fixed + (step & 0xffff0000);
  position->y = leader->y.fixed;
  step <<= 16;
  position->z = leader->z.fixed + step;
#endif
  // Require a free intervening cell before testing the two-cell landing.
  if (FieldActor_CheckMovementCollision(leader, position) != 1 &&
      PushedActor_FindAtTile(position, leader) == 0) {
#if TBS_PATCH_1
    position->x = (leader->x.fixed & -0x100000) + PIXELS(8);
    position->y = leader->y.fixed;
    position->z = (leader->z.fixed & -0x100000) + PIXELS(8);
    Vector_AddPolarOffset(PIXELS(32), jumpDirection, position);
#else
    step = gPushedActor_DirectionSteps[jumpDirection];
    position->x = leader->x.fixed + ((step & 0xffff0000) * 2);
    position->y = leader->y.fixed;
    step <<= 17;
    position->z = leader->z.fixed + step;
#endif
    if (PushedActor_FindAtTile(position, leader) == 0 &&
        FieldActor_CheckMovementCollision(leader, position) == 0) {
      Event_Start();
      Actor_SelectAnimation(leader, 6);
      WaitFrames(6);
      Sound_PlaySong(SE_JUMP);
      Actor_SelectAnimation(leader, 7);
      leader->speedLimit = PIXELS(3);
      leader->acceleration = PIXELS(2);
      leader->velocityY = PIXELS(4);
      *flags &= 126;
      Actor_SetSpriteFlags(leader, 0);
      Actor_SetPositionAndCommit(ACTOR_PARTY_LEADER, position->x >> 16,
                                 position->z >> 16);
      Actor_SelectAnimation(leader, 6);
      Actor_SetSpriteFlags(leader, 1);
      *flags = savedMotionFlags;
      Event_End();
      return 1;
    }
  }
  return 0;
}

/// Moves the particle while damping its velocity, growing it, and advancing
/// rotation.
static void
CrossboneIsleSteppingStones_UpdateDampedParticle(struct Actor *actor) {
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;

  ((struct FieldEffect *)actor)->x += ((struct FieldEffect *)actor)->velocity.x;
  ((struct FieldEffect *)actor)->y += ((struct FieldEffect *)actor)->velocity.y;
  ((struct FieldEffect *)actor)->z += ((struct FieldEffect *)actor)->velocity.z;
  velocityX = ((struct FieldEffect *)actor)->velocity.x;
  velocityY = ((struct FieldEffect *)actor)->velocity.y;
  velocityZ = ((struct FieldEffect *)actor)->velocity.z;
  ((struct FieldEffect *)actor)->velocity.x =
      velocityX - Math_Divide(velocityX, 10);
  ((struct FieldEffect *)actor)->velocity.y =
      velocityY - Math_Divide(velocityY, 3);
  ((struct FieldEffect *)actor)->velocity.z =
      velocityZ - Math_Divide(velocityZ, 10);
  ((struct FieldEffect *)actor)->scaleX +=
      ((struct FieldEffect *)actor)->scaleRateX;
  ((struct FieldEffect *)actor)->scaleY +=
      ((struct FieldEffect *)actor)->scaleRateY;
  ((struct FieldEffect *)actor)->sprite->rotation +=
      ((struct FieldEffect *)actor)->spin;
}

/// Sets the actor's sprite to the colour mode (enum SpriteColorMode) in
/// the low four bits of its timer.
static s32
CrossboneIsleSteppingStones_SetChildValueFromTimer(struct Actor *interpreter) {
  struct Actor *actor = interpreter;
  Actor_SetSpriteColor(actor, actor->timer & 15);
  return 0;
}

const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 sceneId = gAdventure.sceneId;

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

const struct ActorTemplate *Scene_GetActorTemplates(void) {
  s16 sceneId = gAdventure.sceneId;

  if (sceneId == (s32)&SceneId_CrossboneIsleSteppingStones) {
    return gCrossboneIsleSteppingStonesActorTemplates;
  }
  if (sceneId == (s32)&SceneId_CrossboneIsleIcePillars) {
    return gCrossboneIsleIcePillarsActorTemplates;
  }
  if (sceneId == (s32)&SceneId_CrossboneIsleFloodgate) {
    return gCrossboneIsleFloodgateActorTemplates;
  }
  if (sceneId == (s32)&SceneId_CrossboneIsleDragonStatue) {
    return gCrossboneIsleDragonStatueActorTemplates;
  }
  return gCrossboneIsleSteppingStonesActorTemplatesOther;
}

/// Walking into a pillar in CrossboneIsleSteppingStones: jump two cells across
/// it where the cells allow, otherwise push it.
static void CrossboneIsleSteppingStones_JumpOrPushPillar(void) {
  Event_Start();
  if (CrossboneIsleSteppingStones_TryJumpAcrossPillar() == 0)
    PushedActor_AdvancePair();
  Event_End();
}

/// Walking into a pillar in CrossboneIsleIcePillars, or a log in
/// CrossboneIsleFloodgate: push it if the cell beyond is clear.
static void CrossboneIsleSteppingStones_PushPillar(void) {
  struct PushedActorProbe out;

  Event_Start();
  if (PushedActor_FindClearPosition(&out) != 0)
    Actor_MoveAndRedraw(out);
  Event_End();
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
  Task_AddFrame(CrossboneIsleSteppingStones_PrepareWaterScroll,
                TASK_PASS_FRAME);
  WaitFrames(1);
  Irq_SetHandler(1, 0, CrossboneIsleSteppingStones_JitterBackgroundScroll);
  Sound_PlaySong(SE_WATER_LEVEL);
  sShakeChance = 0;
  do {
    WaitFrames(1);
  } while (++sShakeChance <= 100);
  // Commit the water tiles, then remove the scroll task and HBlank handler.
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
static void CrossboneIsleSteppingStones_FlipWaterSwitch(void) {
  s32 waterRaised;
  struct Actor *water;

  Event_Start();
  FieldCamera_SetSpeed(PIXELS(1), PIXELS(1) / 8);
  FieldCamera_MoveTo(PIXELS(281), -1, PIXELS(432), 1);
  FieldCamera_WaitForArrival();
  FieldMessage_Show(MsgFieldFlippedSwitch, 1);
  waterRaised = Event_GetFlag(FLAG_CROSSBONE_ISLE_FLOODGATE_WATER_RAISED);
  // Raise or drain the water before redrawing the logs' footprints.
  if (waterRaised == 0) {
    Event_PlaySong(SE_CRUNCH);
    MapGrid_AnimateCells(gCrossboneIsleSteppingStones_SwitchCloseSteps, 84, 24);
    Event_WaitFrames(30);
    Event_PlaySong(SE_LOG_FALL);
    Actor_SetSpritePriority(ACTOR_WATER, 1);
    Actor_GetById(ACTOR_WATER)->motionFlags = waterRaised;
    water = Actor_Get(ACTOR_WATER);
    water->y.fixed = -PIXELS(32);
    Actor_SetPosition(ACTOR_WATER, PIXELS(272), PIXELS(416));
    Import_ActorSetAnimation(ACTOR_WATER, 1);
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
    Event_PlaySong(SE_CRUNCH);
    MapGrid_AnimateCells(gCrossboneIsleSteppingStones_SwitchOpenSteps, 84, 24);
    Event_WaitFrames(30);
    Event_PlaySong(SE_WATER_DRAIN);
    Actor_GetById(ACTOR_WATER)->motionFlags = 0;
    water = Actor_Get(ACTOR_WATER);
    water->y.fixed = -PIXELS(32);
    Actor_SetPosition(ACTOR_WATER, PIXELS(272), PIXELS(436));
    Import_ActorSetAnimation(ACTOR_WATER, 2);
    MapGrid_CopyCellsTo(65, 45, 16, 27, 2, 4);
    MapGrid_AnimateCells(gCrossboneIsleSteppingStones_FloodgateOpenSteps, 80,
                         24);
    CrossboneIsleSteppingStones_AnimateWaterLevel();
    CrossboneIsleSteppingStones_RestoreGroundFootprint(ACTOR_FIRST_LOG + 1);
    CrossboneIsleSteppingStones_RestoreGroundFootprint(ACTOR_FIRST_LOG + 2);
    CrossboneIsleSteppingStones_RestoreGroundFootprint(ACTOR_FIRST_LOG + 3);
    CrossboneIsleSteppingStones_RestoreGroundFootprint(ACTOR_FIRST_LOG + 4);
    CrossboneIsleSteppingStones_RestoreGroundFootprint(ACTOR_FIRST_LOG + 5);
    CrossboneIsleSteppingStones_RestoreGroundFootprint(ACTOR_FIRST_LOG + 6);
    CrossboneIsleSteppingStones_RestoreGroundFootprint(ACTOR_FIRST_LOG + 7);
    MapGrid_CopyCellAttributes(24, 4, 1, 1, 24, 8);
    GameFlag_Clear(FLAG_CROSSBONE_ISLE_FLOODGATE_WATER_RAISED);
  }
  Event_End();
}

/// Matches a nearby actor's draw priority when it is behind the
/// comparison actor.
static s32
CrossboneIsleSteppingStones_SyncPriorityIfBehind(struct Actor *actor,
                                                 struct Actor *front) {
  s32 result = 0;

  if (front->x.fixed == actor->x.fixed && front->y.fixed == actor->y.fixed &&
      front->z.fixed == actor->z.fixed) {
    return result;
  }
  if (front->x.fixed - PIXELS(16) < actor->x.fixed &&
      actor->x.fixed < front->x.fixed + PIXELS(16) &&
      front->y.fixed / PIXELS(1) == actor->y.fixed / PIXELS(1) &&
      front->z.fixed > actor->z.fixed &&
      front->z.fixed - PIXELS(32) < actor->z.fixed) {
    if (actor->sprite->image.priority < front->sprite->image.priority) {
      actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
      actor->sprite->image.priority = front->sprite->image.priority;
      actor->sprite->shadow.priority = front->sprite->shadow.priority;
    }
    result = 1;
  }
  return result;
}

/// Matches the comparison actor's draw priority when it overlaps the
/// actor in front.
static s32
CrossboneIsleSteppingStones_SyncPriorityIfAhead(struct Actor *front,
                                                struct Actor *actor) {
  s32 result = 0;

  if (actor->x.fixed == front->x.fixed && actor->y.fixed == front->y.fixed &&
      actor->z.fixed == front->z.fixed) {
    return result;
  }
  if (front->x.fixed - PIXELS(16) < actor->x.fixed &&
      actor->x.fixed < front->x.fixed + PIXELS(16) &&
      actor->y.fixed / PIXELS(1) == front->y.fixed / PIXELS(1) &&
      front->z.fixed > actor->z.fixed &&
      front->z.fixed - PIXELS(32) < actor->z.fixed) {
    if (front->sprite->image.priority > actor->sprite->image.priority) {
      actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
      actor->sprite->image.priority = front->sprite->image.priority;
      actor->sprite->shadow.priority = front->sprite->shadow.priority;
    }
    result = 1;
  }
  return result;
}

// UB: calling this s32 function through a void update routine is incompatible.
// Plain void changes 320 to 316 bytes in tbs-en.
#ifdef UBFIX
/// Keeps each moving pillar's priority and collision flags consistent with the
/// leader.
static void
CrossboneIsleSteppingStones_UpdateActorPriority(struct Actor *updated) {
#else
/// Keeps each moving pillar's priority and collision flags consistent with the
/// leader.
static s32
CrossboneIsleSteppingStones_UpdateActorPriority(struct Actor *updated) {
#endif
  struct Actor *actor = (struct Actor *)updated;
  struct Actor *leader = Actor_GetById(ACTOR_PARTY_LEADER);
  s32 hit;
  u32 id;

  if (gFieldWork->castStarted == 1) {
    actor->sprite->image.priority = leader->sprite->image.priority;
    actor->collisionFlags |= ACTOR_SOLID;
#ifdef UBFIX
    return;
#else
    return 0;
#endif
  }
  hit = CrossboneIsleSteppingStones_SyncPriorityIfBehind(actor, leader);
  for (id = ACTOR_FIRST_PILLAR; id <= ACTOR_FIRST_PILLAR + 3; id++) {
    hit += CrossboneIsleSteppingStones_SyncPriorityIfBehind(actor,
                                                            Actor_GetById(id));
  }
  if (hit != 0) {
    for (id = ACTOR_FIRST_PILLAR; id <= ACTOR_FIRST_PILLAR + 3; id++) {
      CrossboneIsleSteppingStones_SyncPriorityIfAhead(actor, Actor_GetById(id));
    }
  }
  if (actor->y.fixed < leader->y.fixed) {
    actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    actor->collisionFlags &= ~ACTOR_SOLID;
    if (actor->sprite->image.priority < leader->sprite->image.priority) {
      actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
      actor->sprite->image.priority = leader->sprite->image.priority;
      actor->sprite->shadow.priority = leader->sprite->shadow.priority;
      hit = 1;
    }
  } else {
    actor->priorityFlags &= ~ACTOR_PRIORITY_UNDERFOOT;
    actor->collisionFlags |= ACTOR_SOLID;
  }
  if (hit == 0) {
    actor->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  }
#ifdef UBFIX
  return;
#else
  return 0;
#endif
}

/// Sorts the four pillars by height and depth, carrying their saved cell state
/// with them.
static void CrossboneIsleSteppingStones_SortPillarActors(void) {
  u32 i;
  u32 j;
  struct Actor *firstActor;
  struct Actor *secondActor;
  struct Actor savedActor;
#ifdef UBFIX
  struct PillarPlacement savedPlacement;
#else
  // UB: the original 12-byte scratch receives a 16-byte DMA transfer.
  // A complete placement array adds four native stack bytes.
  s32 savedPlacement[3];
#endif

  for (i = 0; i <= 2; i++) {
    firstActor = Actor_GetById(i + ACTOR_FIRST_PILLAR);
    for (j = i; j <= 3; j++) {
      secondActor = Actor_GetById(j + ACTOR_FIRST_PILLAR);
      if (firstActor->y.fixed <= secondActor->y.fixed &&
          firstActor->z.fixed < secondActor->z.fixed) {
        continue;
      }
      Dma_Set(secondActor, &savedActor,
              ((u32)(DMA_ENABLE | DMA_32BIT) << 16) |
                  sizeof(savedActor) / sizeof(u32),
              REG_DMA3);
      Dma_Wait(REG_DMA3);
      Dma_Set(firstActor, secondActor,
              ((u32)(DMA_ENABLE | DMA_32BIT) << 16) |
                  sizeof(savedActor) / sizeof(u32),
              REG_DMA3);
      Dma_Wait(REG_DMA3);
      Dma_Set(&savedActor, firstActor,
              ((u32)(DMA_ENABLE | DMA_32BIT) << 16) |
                  sizeof(savedActor) / sizeof(u32),
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
static void CrossboneIsleSteppingStones_MovePillar(void) {
  u32 id;
  struct Actor *actor;
  u8 *flags;
  u32 i;
  u32 k;
  struct Actor *other;
  union CrossboneIslePillarCell cell;

  for (id = ACTOR_FIRST_PILLAR; id <= ACTOR_FIRST_PILLAR + 3; id++) {
    actor = Actor_GetById(id);
    actor->layer = 2;
    i = id - ACTOR_FIRST_PILLAR;
    if ((actor->x.fixed >> 20) == sPillarSlots[i].x &&
        (actor->z.fixed >> 20) == sPillarSlots[i].z && actor->velocityY == 0) {
      continue;
    }
    Dma_Set(&actor->x, &sProbePosition,
            ((u32)(DMA_ENABLE | DMA_32BIT) << 16) |
                sizeof(sProbePosition) / sizeof(u32),
            REG_DMA3);
    Dma_Wait(REG_DMA3);
    if (FieldActor_CheckMovementCollision(actor, &sProbePosition) == -1) {
      actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    }
    flags = &actor->motionFlags;
    CrossboneIsleSteppingStones_SetCellAttributes(
        0, sPillarSlots[i].x, sPillarSlots[i].z, &sPillarSlots[i].cell);
    CrossboneIsleSteppingStones_SetCellAttributes(
        2, sPillarSlots[i].x, sPillarSlots[i].z, &sPillarSlots[i].cell);
    // Falling pillars either reach a ledge, shrink away, or settle on ground.
    if (*flags & 1) {
      if (MapGrid_GetCellBehaviorAt(2, actor->x.fixed, actor->z.fixed) == 50) {
        Sound_PlaySong(SE_STONE_GRIND);
        // Both u8 accesses are alias set 0, and the view is no struct reference
        // (MEM_IN_STRUCT_P); as field stores, the loop is reallocated (100 of
        // 265 instructions differ in tbs-en).
        *(u8 *)&actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
        CrossboneIsleSteppingStones_LowerActorToLedge(id, 1);
        *(u8 *)&actor->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
      } else if (MapGrid_GetCellBehaviorAt(2, actor->x.fixed, actor->z.fixed) ==
                 51) {
        CrossboneIsleSteppingStones_StopAtGroundOrHeight(actor, 0);
        Sound_PlaySong(SE_STONE_GRIND);
        actor->y.fixed = 0;
        actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
        CrossboneIsleSteppingStones_ShrinkPillar(id);
        actor->x.fixed = 0;
        actor->y.fixed = 0;
        actor->z.fixed = 0;
        actor->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
      } else {
        CrossboneIsleSteppingStones_SettleActorOnGround(actor);
      }
      *flags = 0;
    }
    // Save the new ground cell before applying the pillar's height attributes.
    CrossboneIsleSteppingStones_ReadMapGridCell(
        0, actor->x.fixed >> 20, actor->z.fixed >> 20, &sPillarSlots[i].cell);
    if (actor->y.fixed >= 0) {
      CrossboneIsleSteppingStones_ReadMapGridCell(
          0, 27, (actor->y.fixed >> 20) + 6, &cell);
      CrossboneIsleSteppingStones_SetCellAttributes(
          0, actor->x.fixed >> 20, actor->z.fixed >> 20, &cell);
      cell.bits.highFlags = sPillarSlots[i].cell.bits.highFlags;
      // x and z read by their offsets: through Actor's single/group
      // union, as actor->x.fixed, a load moves.
      CrossboneIsleSteppingStones_SetCellAttributes(
          2, *(s32 *)((u8 *)actor + offsetof(struct Actor, x)) >> 20,
          *(s32 *)((u8 *)actor + offsetof(struct Actor, z)) >> 20, &cell);
    }
    sPillarSlots[i].x = actor->x.fixed >> 20;
    sPillarSlots[i].y = actor->y.fixed >> 20;
    sPillarSlots[i].z = actor->z.fixed >> 20;
    // Mark pillars covered by this one, then restore their sort order.
    for (k = 0; k <= 3; k++) {
      if (k == i) {
        continue;
      }
      GameFlag_Clear(sPillarSlots[k].flag);
      other = Actor_GetById(k + ACTOR_FIRST_PILLAR);
      if ((actor->x.fixed >> 20) == (other->x.fixed >> 20) &&
          (actor->z.fixed >> 20) == (other->z.fixed >> 20) &&
          actor->y.fixed > other->y.fixed) {
        GameFlag_Set(sPillarSlots[k].flag);
      }
    }
  }
  CrossboneIsleSteppingStones_SortPillarActors();
}

/// Walking into a pillar in CrossboneIsleStackedPillars: jump across if
/// possible, otherwise push it, and the pillars settle.
static void CrossboneIsleSteppingStones_JumpOrPushStackedPillar(void) {
  Event_Start();
#if TBS_PATCH_3
  Actor_GetById(ACTOR_PARTY_LEADER)->y.fixed =
      MapGrid_GetTerrainHeight(Actor_GetById(ACTOR_PARTY_LEADER)->layer,
                               Actor_GetById(ACTOR_PARTY_LEADER)->x.fixed,
                               Actor_GetById(ACTOR_PARTY_LEADER)->z.fixed);
#endif
  if (CrossboneIsleSteppingStones_TryJumpAcrossPillar() == 0) {
    Actor_GetById(ACTOR_PARTY_LEADER)->motionFlags &= ~ACTOR_FOLLOWS_TERRAIN;
    Actor_GetById(ACTOR_PARTY_LEADER)->priorityFlags &=
        ~ACTOR_PRIORITY_AUTOMATIC;
    PushedActor_AdvancePair();
    CrossboneIsleSteppingStones_MovePillar();
    Actor_GetById(ACTOR_PARTY_LEADER)->motionFlags |= ACTOR_FOLLOWS_TERRAIN;
    Actor_GetById(ACTOR_PARTY_LEADER)->priorityFlags |=
        ACTOR_PRIORITY_AUTOMATIC;
  }
  Event_End();
}

/// Walking north at CrossboneIsleStackedPillars's ladder: climb up.
static void CrossboneIsleSteppingStones_ClimbUp(void) { Field_ClimbUp(); }

/// Walking south at CrossboneIsleStackedPillars's ladder: push the pillar
/// below, jump, or climb down.
static void CrossboneIsleSteppingStones_PushOrClimbDown(void) {
  struct Actor *leader = Actor_GetById(ACTOR_PARTY_LEADER);
  struct FieldPosition position;
  struct FieldPosition *p = &position;

  p->x = leader->x.fixed;
  p->y = leader->y.fixed;
  p->z = leader->z.fixed + PIXELS(16);
  if (PushedActor_FindAtTile(p, leader) != NULL) {
    CrossboneIsleSteppingStones_JumpOrPushStackedPillar();
  } else if (CrossboneIsleSteppingStones_TryJumpAcrossPillar() == 0) {
    Field_ClimbDown();
  }
}

/// Stepping on step trigger 40 in CrossboneIsleStackedPillars: the fade overlay
/// follows the leader, and the leader's rise counter starts.
static void CrossboneIsleSteppingStones_StartRising(void) {
  struct Actor *leader = Actor_GetById(ACTOR_PARTY_LEADER);
  ((struct FadeOverlayWork *)HEAP_SLOT_BLOCK(HEAP_SLOT_FADE_OVERLAY))->actor =
      leader;
  leader->riseCounter = 1;
}

/// Stepping on step trigger 41 in CrossboneIsleStackedPillars: the overlay lets
/// the leader go, and the rise counter stops.
static void CrossboneIsleSteppingStones_StopRising(void) {
  struct Actor *leader = Actor_GetById(ACTOR_PARTY_LEADER);

  ((struct FadeOverlayWork *)HEAP_SLOT_BLOCK(HEAP_SLOT_FADE_OVERLAY))->actor =
      NULL;
  leader->riseCounter = 0;
}

// UB: calling this s32 function through a void update routine is incompatible.
// Plain void changes 54 to 52 bytes in tbs-en.
#ifdef UBFIX
/// Keeps the frozen puddle's underfoot flag consistent with the leader's height
/// and depth.
static void
CrossboneIsleSteppingStones_UpdateFrozenPuddlePriority(struct Actor *updated) {
#else
/// Puts the frozen puddle under the leader when it is behind and below the
/// leader.
static s32
CrossboneIsleSteppingStones_UpdateFrozenPuddlePriority(struct Actor *updated) {
#endif
  struct Actor *actor = (struct Actor *)updated;
  struct Actor *ref = Actor_GetById(ACTOR_PARTY_LEADER);

  if (actor->z.fixed > ref->z.fixed) {
    actor->priorityFlags &= ~ACTOR_PRIORITY_UNDERFOOT;
    if (actor->y.fixed < ref->y.fixed)
      actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  }

#ifdef UBFIX
  return;
#else
  return 0;
#endif
}

/// Remembers the frozen puddle, installs its priority update routine, and
/// starts its flicker script.
static void CrossboneIsleSteppingStones_FreezePuddle(s32 id) {
  struct Actor *actor;

  actor = Actor_GetById(id);
  Event_Start();
#ifdef UBFIX
  actor->update = CrossboneIsleSteppingStones_UpdateFrozenPuddlePriority;
#else
  actor->update = (void (*)(
      struct Actor *))CrossboneIsleSteppingStones_UpdateFrozenPuddlePriority;
#endif
  MapGrid_CopyCellAttributes(20, 14, 1, 1, actor->x.fixed >> 20,
                             actor->z.fixed >> 20);
  GameFlag_Set(FLAG_CROSSBONE_ISLE_DRAGON_STATUE_FIRST_PUDDLE_FROZEN + id -
               ACTOR_FIRST_PUDDLE);
  Actor_SetScript(id, gCrossboneIsleSteppingStones_ActionTable);
  Event_End();
}

/// Frost on the first puddle in CrossboneIsleDragonStatue: it freezes into a
/// pillar of ice.
static void CrossboneIsleSteppingStones_FreezeFirstPuddle(void) {
  CrossboneIsleSteppingStones_FreezePuddle(ACTOR_FIRST_PUDDLE);
}

/// Frost on the second puddle in CrossboneIsleDragonStatue.
static void CrossboneIsleSteppingStones_FreezeSecondPuddle(void) {
  CrossboneIsleSteppingStones_FreezePuddle(ACTOR_SECOND_PUDDLE);
}

/// Frost begins on the third puddle in CrossboneIsleDragonStatue: note whether
/// the leader stands on it.
static void CrossboneIsleSteppingStones_FrostStartsOnThirdPuddle(void) {
  struct Actor *leader;
  struct Actor *puddle;
  u8 *effects;

  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  puddle = Actor_GetById(ACTOR_THIRD_PUDDLE);
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
/// shrinks back, and the pillar in it (ACTOR_RISING_PILLAR) rises 32 pixels.
static void CrossboneIsleSteppingStones_FrostRaisesPillar(void) {
  struct Actor *pillar;

  pillar = Actor_GetById(ACTOR_RISING_PILLAR);
  Actor_GetById(ACTOR_THIRD_PUDDLE)->scaleX = Q16_ONE;
  Actor_GetById(ACTOR_THIRD_PUDDLE)->scaleY = Q16_ONE;
  (Actor_GetById(ACTOR_THIRD_PUDDLE)->sprite)->image.priority = 2;
  pillar->sprite->image.priority = 2;
  pillar->acceleration = PIXELS(2) / 5;
  pillar->speedLimit = PIXELS(4) / 5;
  FieldActor_SetMoveTarget(pillar, pillar->x.fixed, PIXELS(32),
                           pillar->z.fixed);
  Actor_WaitForMoveById(ACTOR_RISING_PILLAR);
  MapGrid_CopyCellAttributes(20, 14, 1, 1, 22, 16);
}

/// Emits growing, rotating particles while Frost freezes the third puddle.
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

// UB: calling this s32 function through a void update routine is incompatible.
// Plain void changes 172 to 168 bytes in tbs-en.
#ifdef UBFIX
/// Emits a randomized particle from the thawing puddle every eight frames.
static void
CrossboneIsleSteppingStones_SpawnThawParticle(struct Actor *updated) {
#else
/// Emits a randomized particle from the thawing puddle every eight frames.
static s32
CrossboneIsleSteppingStones_SpawnThawParticle(struct Actor *updated) {
#endif
  struct Actor *actor = (struct Actor *)updated;
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

  x = actor->x.fixed + (((((u32)Import_RandomNext() * 17) >> 16) - 8) << 16);
  y = actor->y.fixed + ((((u32)Import_RandomNext() * 17) >> 16) << 16);
  z = actor->z.fixed + (((((u32)Import_RandomNext() * 17) >> 16) - 8) << 16);
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
static void CrossboneIsleSteppingStones_FreezeThirdPuddle(void) {
  s32 set;

  set = GameFlag_Get(FLAG_CROSSBONE_ISLE_DRAGON_STATUE_LEADER_ON_THIRD_PUDDLE);
  if (set == 0) {
    GameFlag_Set(FLAG_CROSSBONE_ISLE_DRAGON_STATUE_FIRST_PUDDLE_FROZEN + 2);
    Event_Start();
    FieldCamera_SetSpeedParameters(PIXELS(3) / 5, PIXELS(3) / 40);
    FieldCamera_MoveWithinMapBounds(PIXELS(312), -1, PIXELS(184), 1);
    FieldCamera_WaitForArrival();
    Event_WaitFrames(20);
    MapGrid_CopyCellsTo(73, 10, 60, 10, 1, 2);
    Event_WaitFrames(20);
    // The third puddle freezes while the previously frozen second puddle thaws.
    Task_AddFrame(CrossboneIsleSteppingStones_SpawnFrostParticle,
                  TASK_PASS_FRAME);
    Event_WaitFrames(40);
    if (Event_GetFlag(FLAG_CROSSBONE_ISLE_DRAGON_STATUE_FIRST_PUDDLE_FROZEN +
                      1) != 0) {
#ifdef UBFIX
      Actor_GetById(ACTOR_SECOND_PUDDLE)->update =
          CrossboneIsleSteppingStones_SpawnThawParticle;
#else
      Actor_GetById(ACTOR_SECOND_PUDDLE)->update = (void (*)(
          struct Actor *))CrossboneIsleSteppingStones_SpawnThawParticle;
#endif
      Import_ActorSetAnimation(ACTOR_SECOND_PUDDLE, 6);
      Actor_WaitForAnimationChange(ACTOR_SECOND_PUDDLE);
      Actor_GetById(ACTOR_SECOND_PUDDLE)->update =
          (void (*)(struct Actor *))set;
      MapGrid_CopyCellAttributes(17, 13, 1, 1, 18, 13);
      GameFlag_Clear(FLAG_CROSSBONE_ISLE_DRAGON_STATUE_FIRST_PUDDLE_FROZEN + 1);
      Actor_ConfigureSpriteColor(ACTOR_SECOND_PUDDLE, SPRITE_COLOR_NORMAL);
      Actor_SetMovementTypeById(ACTOR_SECOND_PUDDLE, MOVEMENT_TYPE_STAND);
    } else {
      Event_WaitFrames(60);
    }
    Task_Remove(CrossboneIsleSteppingStones_SpawnFrostParticle);
    Event_WaitFrames(20);
    MapGrid_CopyCellsTo(72, 10, 60, 10, 1, 2);
    Event_WaitFrames(20);
    Event_End();
  }
}

/// Move begins on CrossboneIsleDragonStatue's sinking boulder: its cell is
/// cleared.
static void CrossboneIsleSteppingStones_StartMovingSinkingPillar(void) {
  struct Actor *pillar = Actor_GetById(ACTOR_SINKING_BOULDER);
  s32 x;
  s32 z;

  Event_Start();

  x = pillar->x.fixed >> 20;
  z = pillar->z.fixed >> 20;

  PushedActor_FillGridAttributeRectangle(2, x, z, 1, 1, CELL_BEHAVIOR_NORMAL);
  Event_End();
}

/// Move on CrossboneIsleDragonStatue's sinking boulder, and after each push:
/// once it reaches column 16 it sinks two pixels into place for good.
static void CrossboneIsleSteppingStones_MoveSinkingPillar(void) {
  struct Actor *pillar;
  s32 alreadyLowered;

  pillar = Actor_GetById(ACTOR_SINKING_BOULDER);
  Event_Start();
  PushedActor_FillGridAttributeRectangle(2, (pillar->x.fixed >> 20),
                                         (pillar->z.fixed >> 20), 1, 1,
                                         CELL_BEHAVIOR_WALL);
  if ((pillar->x.fixed >> 20) == 16) {
    alreadyLowered =
        Event_GetFlag(FLAG_CROSSBONE_ISLE_DRAGON_STATUE_BOULDER_SUNK);
    if (alreadyLowered == 0) {
      Event_WaitFrames(10);
      Sound_PlaySong(SE_DOOR_CLOSE);
      pillar->motionFlags = alreadyLowered;
      pillar->groundHeight = -PIXELS(2);
      pillar->y.fixed = -PIXELS(2);
      GameFlag_Set(FLAG_CROSSBONE_ISLE_DRAGON_STATUE_BOULDER_SUNK);
    }
  }
  Event_End();
}

/// Walking into CrossboneIsleDragonStatue's sinking boulder: push it if the
/// cell beyond is clear.
static void CrossboneIsleSteppingStones_PushSinkingPillar(void) {
  struct PushedActorProbe probe;

  Event_Start();
  if (PushedActor_FindClearPosition(&probe) != 0) {
    Actor_MoveAndRedraw(probe);
  } else {
    CrossboneIsleSteppingStones_StartMovingSinkingPillar();
    PushedActor_AdvancePair();
    CrossboneIsleSteppingStones_MoveSinkingPillar();
  }
  Event_End();
}

const struct Trigger *Scene_GetTriggers(void) {
  s16 sceneId = gAdventure.sceneId;

  if (sceneId == (s32)&SceneId_CrossboneIsleSteppingStones) {
    return gCrossboneIsleSteppingStonesTriggers;
  }
  if (sceneId == (s32)&SceneId_CrossboneIsleIcePillars) {
    return gCrossboneIsleIcePillarsTriggers;
  }
  if (sceneId == (s32)&SceneId_CrossboneIsleFloodgate) {
    return gCrossboneIsleFloodgateTriggers;
  }
  if (sceneId == (s32)&SceneId_CrossboneIsleStackedPillars) {
    return gCrossboneIsleStackedPillarsTriggers;
  }
  if (sceneId == (s32)&SceneId_CrossboneIsleDragonStatue) {
    return gCrossboneIsleDragonStatueTriggers;
  }
  return gCrossboneIsleSteppingStonesTriggersOther;
}

/// Prepares the lowered actor's collision and draw priority below the
/// leader.
static void CrossboneIsleSteppingStones_PrepLoweredActor(s32 id) {
  struct Actor *actor = Actor_GetById(id);

  actor->collisionFlags &= ~ACTOR_SOLID;
  actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  actor->motionFlags = 0;
  Actor_SetSpriteFlags(actor, 0);
  actor->sprite->image.priority = 2;
}

/// Restores a previously frozen pillar's mode, map cell, priority update
/// routine, and script.
static void CrossboneIsleSteppingStones_RestoreFlaggedPillar(s32 actorId) {
  struct Actor *actor;

  actor = Actor_GetById(actorId);
  if (GameFlag_Get(FLAG_CROSSBONE_ISLE_DRAGON_STATUE_FIRST_PUDDLE_FROZEN +
                   actorId - ACTOR_FIRST_PUDDLE) != 0) {
    Actor_SelectAnimation(actor, 5);
#ifdef UBFIX
    actor->update = CrossboneIsleSteppingStones_UpdateFrozenPuddlePriority;
#else
    actor->update = (void (*)(
        struct Actor *))CrossboneIsleSteppingStones_UpdateFrozenPuddlePriority;
#endif
    MapGrid_CopyCellAttributes(20, 14, 1, 1, actor->x.fixed >> 20,
                               actor->z.fixed >> 20);
    Actor_SetScript(actorId, gCrossboneIsleSteppingStones_ActionTable);
  }
}

/// Uses layer 2 and installs the moving pillar's priority update routine.
static void CrossboneIsleSteppingStones_InitializePillarPriority(s32 id) {
  struct Actor *actor = Actor_GetById(id);

  actor->layer = 2;
  actor->motionFlags = 0;
#ifdef UBFIX
  actor->update = CrossboneIsleSteppingStones_UpdateActorPriority;
#else
  actor->update =
      (void (*)(struct Actor *))CrossboneIsleSteppingStones_UpdateActorPriority;
#endif
}

/// Matches the leader's draw priority to a nearby pillar in front.
static void CrossboneIsleSteppingStones_UpdateLeaderPriority(void) {
  struct Actor *hero = Actor_GetById(ACTOR_PARTY_LEADER);
  u32 selector;
  struct Actor *actor;

  for (selector = ACTOR_FIRST_PILLAR; selector <= ACTOR_FIRST_PILLAR + 3;
       selector++) {
    actor = Actor_GetById(selector);
    if (hero->y.fixed / PIXELS(1) == actor->y.fixed / PIXELS(1) &&
        hero->z.fixed <= actor->z.fixed - PIXELS(8) &&
        hero->z.fixed > actor->z.fixed - PIXELS(24)) {
      if (hero->x.fixed - PIXELS(16) <= actor->x.fixed &&
          actor->x.fixed < hero->x.fixed + PIXELS(16)) {
        Actor_SetSpritePriority(ACTOR_PARTY_LEADER,
                                actor->sprite->image.priority);
        return;
      }
    } else {
      Actor_GetById(ACTOR_PARTY_LEADER)->priorityFlags |=
          ACTOR_PRIORITY_AUTOMATIC;
    }
  }
}

/// Restores each room's pillar footprints, water state, and persistent puzzle
/// state.
s32 Scene_Initialize(void) {
  struct Actor *actor;
  u32 i;
  u32 id;
  s32 x;
  s32 z;

  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  if (gAdventure.sceneId == (s32)&SceneId_CrossboneIsleIcePillars) {
    Scene_RedrawActorFootprint(ACTOR_FIRST_PILLAR);
    Scene_RedrawActorFootprint(ACTOR_FIRST_PILLAR + 1);
    Scene_RedrawActorFootprint(ACTOR_FIRST_PILLAR + 2);
    Scene_RedrawActorFootprint(ACTOR_FIRST_PILLAR + 3);
    Scene_RedrawActorFootprint(ACTOR_FIRST_PILLAR + 4);
  } else if (gAdventure.sceneId == (s32)&SceneId_CrossboneIsleFloodgate) {
    // Rebuild the flooded room and float the logs again when the water stayed
    // raised through a battle.
    MapGrid_CopyCells(32, 0, 64, 32, 0, 64);
    Scene_RedrawActorFootprint(ACTOR_FIRST_LOG);
    Scene_RedrawActorFootprint(ACTOR_FIRST_LOG + 1);
    Scene_RedrawActorFootprint(ACTOR_FIRST_LOG + 2);
    Scene_RedrawActorFootprint(ACTOR_FIRST_LOG + 3);
    Scene_RedrawActorFootprint(ACTOR_FIRST_LOG + 4);
    Scene_RedrawActorFootprint(ACTOR_FIRST_LOG + 5);
    Scene_RedrawActorFootprint(ACTOR_FIRST_LOG + 6);
    Scene_RedrawActorFootprint(ACTOR_FIRST_LOG + 7);
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) != 0 &&
        GameFlag_Get(FLAG_CROSSBONE_ISLE_FLOODGATE_WATER_RAISED) != 0) {
      MapGrid_CopyCellsTo(79, 34, 84, 24, 1, 2);
      MapGrid_CopyCellsTo(0, 32, 32, 0, 32, 32);
      MapGrid_CopyCellsTo(32, 32, 64, 0, 32, 32);
      CrossboneIsleSteppingStones_DrawFloatingLogs();
      MapGrid_CopyCellAttributes(24, 3, 1, 1, 24, 8);
    }
  } else if (gAdventure.sceneId == (s32)&SceneId_CrossboneIsleStackedPillars) {
    // Restore the four movable pillars and the leader's riding overlay.
    Actor_CreateConfigured(PIXELS(584), 0, PIXELS(200), SPRITE_CRATE);
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
      Actor_GetById(ACTOR_PARTY_LEADER)->riseCounter = 1;
    }
    FieldEffect_StartFadeOverlay(0);
    if (Actor_GetById(ACTOR_PARTY_LEADER)->riseCounter == 0) {
      CrossboneIsleSteppingStones_StopRising();
    }
    CrossboneIsleSteppingStones_InitializePillarPriority(ACTOR_FIRST_PILLAR);
    CrossboneIsleSteppingStones_InitializePillarPriority(ACTOR_FIRST_PILLAR +
                                                         1);
    CrossboneIsleSteppingStones_InitializePillarPriority(ACTOR_FIRST_PILLAR +
                                                         2);
    CrossboneIsleSteppingStones_InitializePillarPriority(ACTOR_FIRST_PILLAR +
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
    CrossboneIsleSteppingStones_MovePillar();
    Event_WaitFrames(1);
    Task_AddFrame(CrossboneIsleSteppingStones_UpdateLeaderPriority,
                  TASK_PASS_FRAME);
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) != 0) {
      for (id = ACTOR_FIRST_PILLAR; id < ACTOR_FIRST_PILLAR + 4; id++) {
        actor = Actor_GetById(id);
        x = actor->x.fixed >> 20;
        if (x == 37) {
          z = actor->z.fixed >> 20;
          if (z == 9) {
            MapGrid_CopyCellAttributes(27, 8, 1, 1, x, z);
            break;
          }
        }
      }
    }
  } else if (gAdventure.sceneId == (s32)&SceneId_CrossboneIsleDragonStatue) {
    // Restore CrossboneIsleDragonStatue's moved pillar, frozen puddles, and
    // raised pillar pair.
    Import_ActorSetAnimation(ACTOR_SINKING_BOULDER, 2);
    Actor_ConfigureSpriteColor(ACTOR_SINKING_BOULDER, SPRITE_COLOR_GREY);
    Scene_RedrawActorFootprint(ACTOR_FIRST_LOG);
    Scene_RedrawActorFootprint(ACTOR_FIRST_LOG + 1);
#if TBS_PATCH_1
    Actor_GetById(ACTOR_FIRST_LOG)->motionFlags = 0;
    Actor_GetById(ACTOR_FIRST_LOG + 1)->motionFlags = 0;
#endif
    CrossboneIsleSteppingStones_MoveSinkingPillar();
    CrossboneIsleSteppingStones_PrepLoweredActor(ACTOR_FIRST_PUDDLE);
    CrossboneIsleSteppingStones_PrepLoweredActor(ACTOR_SECOND_PUDDLE);
    CrossboneIsleSteppingStones_PrepLoweredActor(ACTOR_THIRD_PUDDLE);
    CrossboneIsleSteppingStones_RestoreFlaggedPillar(ACTOR_FIRST_PUDDLE);
    CrossboneIsleSteppingStones_RestoreFlaggedPillar(ACTOR_SECOND_PUDDLE);
    CrossboneIsleSteppingStones_RestoreFlaggedPillar(ACTOR_THIRD_PUDDLE);
    actor = Actor_GetById(ACTOR_THIRD_PUDDLE);
    actor->update = 0;
    CrossboneIsleSteppingStones_PrepLoweredActor(ACTOR_RISING_PILLAR);
    Actor_GetById(ACTOR_RISING_PILLAR)->collisionFlags |= ACTOR_NOT_TARGETED;
    if (GameFlag_Get(FLAG_CROSSBONE_ISLE_DRAGON_STATUE_FIRST_PUDDLE_FROZEN +
                     2) == 0) {
      Actor_GetById(ACTOR_THIRD_PUDDLE)->scaleX = 3 * Q16_ONE / 2;
      Actor_GetById(ACTOR_THIRD_PUDDLE)->scaleY = 3 * Q16_ONE / 2;
      Actor_GetById(ACTOR_THIRD_PUDDLE)->sprite->image.priority = 3;
      Actor_GetById(ACTOR_RISING_PILLAR)->sprite->image.priority = 3;
      MapGrid_CopyCellAttributes(26, 12, 1, 1, 22, 16);
    }
  }
  return 0;
}

s32 gFieldEffect_ShortScript[] = {
    SC_WAIT(1),           SC_LOOP(22, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),          SC_RELEASE,
};

s32 gFieldEffect_MediumScript[] = {
    SC_WAIT(1),           SC_LOOP(44, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),          SC_RELEASE,
};

s32 gFieldEffect_LongScript[] = {
    SC_WAIT(1),           SC_LOOP(126, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),           SC_RELEASE,
};

/// Pixel step by direction: X in the upper halfword, Z in the lower.
#define DIRECTION_STEP(dx, dz) (((dx) << 16) | ((dz) & 0xffff))

s32 gPushedActor_DirectionSteps[] = {
    DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(16, 0),
};

// clang-format off
u32 gPushedActor_FootprintKinds[] = {
    207, 205, 228, 229, 298, 297,
};

s32 gPushedActor_FootprintBounds[] = {
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

/// The floodgate's switch flipping as CrossboneIsleSteppingStones_FlipWaterSwitch closes the
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
/// almost dry (CrossboneIsleSteppingStones_FlipWaterSwitch's render).
u16 gCrossboneIsleSteppingStones_FloodgateFootSteps[] = {
    MAPGRID_ANIMATION_STEP(75, 38, 2, 1, 4), MAPGRID_ANIMATION_STEP(77, 38, 2, 1, 6), MAPGRID_ANIMATION_STEP(79, 38, 2, 1, 8), MAPGRID_ANIMATION_STEP(65, 53, 2, 1, 1), MAPGRID_ANIMATION_STEPS_END,
};

struct SceneEntrance gCrossboneIsleSteppingStonesEntrancesOther[] = {
    { 0, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gCrossboneIsleSteppingStonesEntrances[] = {
    { CROSSBONE_ISLE_STEPPING_STONES_FROM_CROSSBONE_ISLE_FIRST_FLOOR_EAST, CONDITION_ALWAYS, 712, 0, 760, FACING_NORTH, 0, 560, 368, 992, 792, 0 },
    { CROSSBONE_ISLE_STEPPING_STONES_FROM_CROSSBONE_ISLE_FIRST_FLOOR_WEST, CONDITION_ALWAYS, 600, 0, 760, FACING_NORTH, 0, 560, 368, 992, 792, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gCrossboneIsleIcePillarsEntrances[] = {
    { CROSSBONE_ISLE_ICE_PILLARS_FROM_CROSSBONE_ISLE_SECOND_FLOOR_EAST, CONDITION_ALWAYS, 216, 0, 360, FACING_NORTH, 0, 32, 32, 384, 400, 0 },
    { CROSSBONE_ISLE_ICE_PILLARS_FROM_CROSSBONE_ISLE_SECOND_FLOOR_WEST, CONDITION_ALWAYS, 104, 0, 360, FACING_NORTH, 0, 32, 32, 384, 400, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gCrossboneIsleFloodgateEntrances[] = {
    { CROSSBONE_ISLE_FLOODGATE_FROM_CROSSBONE_ISLE_FIFTH_FLOOR_EAST, CONDITION_ALWAYS, 184, 0, 472, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { CROSSBONE_ISLE_FLOODGATE_FROM_CROSSBONE_ISLE_FIFTH_FLOOR_WEST, CONDITION_ALWAYS, 64, 0, 472, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gCrossboneIsleStackedPillarsEntrances[] = {
    { CROSSBONE_ISLE_STACKED_PILLARS_FROM_CROSSBONE_ISLE_SEVENTH_FLOOR_EAST, CONDITION_ALWAYS, 744, 0, 520, FACING_NORTH, 0, 440, 16, 960, 544, 0 },
    { CROSSBONE_ISLE_STACKED_PILLARS_FROM_CROSSBONE_ISLE_SEVENTH_FLOOR_WEST, CONDITION_ALWAYS, 632, 0, 520, FACING_NORTH, 0, 440, 16, 960, 544, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gCrossboneIsleDragonStatueEntrances[] = {
    { CROSSBONE_ISLE_DRAGON_STATUE_FROM_CROSSBONE_ISLE_EIGHTH_FLOOR_EAST, CONDITION_ALWAYS, 280, 0, 488, FACING_NORTH, 0, 16, 32, 464, 528, 0 },
    { CROSSBONE_ISLE_DRAGON_STATUE_FROM_CROSSBONE_ISLE_EIGHTH_FLOOR_WEST, CONDITION_ALWAYS, 168, 0, 488, FACING_NORTH, 0, 16, 32, 464, 528, 0 },
    { SCENE_TABLE_END },
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

/// Unused: an empty actor template table nothing returns.
struct ActorTemplate gCrossboneIsleSteppingStonesNoActorTemplates[] = {
    { SCENE_TABLE_END },
};

struct ActorTemplate gCrossboneIsleSteppingStonesActorTemplates[] = {
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(744), 0, PIXELS(488), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(792), 0, PIXELS(552), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(696), 0, PIXELS(616), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(776), 0, PIXELS(680), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SCENE_TABLE_END },
};

struct ActorTemplate gCrossboneIsleIcePillarsActorTemplates[] = {
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
    { SCENE_TABLE_END },
};

struct ActorTemplate gCrossboneIsleFloodgateActorTemplates[] = {
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
    { SCENE_TABLE_END },
};

struct ActorTemplate gCrossboneIsleSteppingStonesActorTemplatesOther[] = {
    { 0x100, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(648), 0, PIXELS(184), FACING_NORTH, TALK_FACING_KEEP, 0 },
    { 0x100, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0 },
    { 0x100, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(680), 0, PIXELS(392), FACING_NORTH, TALK_FACING_KEEP, 0 },
    { 0x100, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0 },
    { SCENE_TABLE_END },
};

struct ActorTemplate gCrossboneIsleDragonStatueActorTemplates[] = {
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
    { SCENE_TABLE_END },
};

struct Trigger gCrossboneIsleSteppingStonesTriggersOther[] = {
    { TRIGGERS_END },
};

struct Trigger gCrossboneIsleSteppingStonesTriggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, CROSSBONE_ISLE_STEPPING_STONES_TO_CROSSBONE_ISLE_FIRST_FLOOR_EAST },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, CROSSBONE_ISLE_STEPPING_STONES_TO_CROSSBONE_ISLE_FIRST_FLOOR_WEST },
    { TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, 10, CONDITION_ALWAYS, EVENT(CrossboneIsleSteppingStones_JumpOrPushPillar) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 100, FLAG_CROSSBONE_ISLE_STEPPING_STONES_ANTIDOTE, FIND_ITEM(ITEM_ANTIDOTE) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 101, FLAG_CROSSBONE_ISLE_STEPPING_STONES_NUT, FIND_ITEM(ITEM_NUT) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 102, FLAG_CROSSBONE_ISLE_STEPPING_STONES_111_COINS, FIND_COINS(111) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 103, FLAG_CROSSBONE_ISLE_STEPPING_STONES_HARD_NUT, FIND_ITEM(ITEM_HARD_NUT) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_PILLAR, CONDITION_ALWAYS, 0 },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_PILLAR + 1, CONDITION_ALWAYS, 0 },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_PILLAR + 2, CONDITION_ALWAYS, 0 },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_PILLAR + 3, CONDITION_ALWAYS, 0 },
    { TRIGGERS_END },
};

struct Trigger gCrossboneIsleIcePillarsTriggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, CROSSBONE_ISLE_ICE_PILLARS_TO_CROSSBONE_ISLE_SECOND_FLOOR_EAST },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, CROSSBONE_ISLE_ICE_PILLARS_TO_CROSSBONE_ISLE_SECOND_FLOOR_WEST },
    { TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, 10, CONDITION_ALWAYS, EVENT(CrossboneIsleSteppingStones_PushPillar) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 100, FLAG_CROSSBONE_ISLE_ICE_PILLARS_LUCKY_PEPPER, FIND_ITEM(ITEM_LUCKY_PEPPER) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 101, FLAG_CROSSBONE_ISLE_ICE_PILLARS_MYSTERY_BLADE, FIND_ITEM(ITEM_MYSTERY_BLADE) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 102, FLAG_CROSSBONE_ISLE_ICE_PILLARS_222_COINS, FIND_COINS(222) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 103, FLAG_CROSSBONE_ISLE_ICE_PILLARS_ELIXIR, FIND_ITEM(ITEM_ELIXIR) },
    { TRIGGERS_END },
};

struct Trigger gCrossboneIsleFloodgateTriggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, CROSSBONE_ISLE_FLOODGATE_TO_CROSSBONE_ISLE_FIFTH_FLOOR_EAST },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, CROSSBONE_ISLE_FLOODGATE_TO_CROSSBONE_ISLE_FIFTH_FLOOR_WEST },
    { TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, 10, CONDITION_ALWAYS, EVENT(CrossboneIsleSteppingStones_PushPillar) },
    { EXAMINE_CHEST_FACING(FACING_EAST), 100, FLAG_CROSSBONE_ISLE_FLOODGATE_555_COINS, FIND_COINS(555) },
    { EXAMINE_CHEST_FACING(FACING_NORTH), 100, FLAG_CROSSBONE_ISLE_FLOODGATE_555_COINS, FIND_COINS(555) },
    { EXAMINE_CHEST_FACING(FACING_EAST), 101, FLAG_CROSSBONE_ISLE_FLOODGATE_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { EXAMINE_CHEST_FACING(FACING_NORTHEAST), 101, FLAG_CROSSBONE_ISLE_FLOODGATE_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 102, FLAG_CROSSBONE_ISLE_FLOODGATE_POTION, FIND_ITEM(ITEM_POTION) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 103, FLAG_CROSSBONE_ISLE_FLOODGATE_NINJA_GARB, FIND_ITEM(ITEM_NINJA_GARB) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), 20, CONDITION_ALWAYS, EVENT(CrossboneIsleSteppingStones_FlipWaterSwitch) },
    { TRIGGERS_END },
};

struct Trigger gCrossboneIsleStackedPillarsTriggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, CROSSBONE_ISLE_STACKED_PILLARS_TO_CROSSBONE_ISLE_SEVENTH_FLOOR_EAST },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, CROSSBONE_ISLE_STACKED_PILLARS_TO_CROSSBONE_ISLE_SEVENTH_FLOOR_WEST },
    { TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, 30, CONDITION_ALWAYS, EVENT(CrossboneIsleSteppingStones_JumpOrPushStackedPillar) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 31, CONDITION_ALWAYS, EVENT(CrossboneIsleSteppingStones_ClimbUp) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 31, CONDITION_ALWAYS, EVENT(CrossboneIsleSteppingStones_PushOrClimbDown) },
    { TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, 31, CONDITION_ALWAYS, EVENT(CrossboneIsleSteppingStones_JumpOrPushStackedPillar) },
    { TRIGGER_STEP, 40, CONDITION_ALWAYS, EVENT(CrossboneIsleSteppingStones_StartRising) },
    { TRIGGER_STEP, 41, CONDITION_ALWAYS, EVENT(CrossboneIsleSteppingStones_StopRising) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 100, FLAG_CROSSBONE_ISLE_STACKED_PILLARS_777_COINS, FIND_COINS(777) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 101, FLAG_CROSSBONE_ISLE_STACKED_PILLARS_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { EXAMINE_CHEST_FACING(FACING_EAST), 102, FLAG_CROSSBONE_ISLE_STACKED_PILLARS_PSY_CRYSTAL, FIND_ITEM(ITEM_PSY_CRYSTAL) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 103, FLAG_CROSSBONE_ISLE_STACKED_PILLARS_WICKED_MACE, FIND_ITEM(ITEM_WICKED_MACE) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_PILLAR, FLAG_CROSSBONE_ISLE_STACKED_PILLARS_FIRST_PILLAR_UNDER_ANOTHER, EVENT(CrossboneIsleSteppingStones_MovePillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_PILLAR + 1, FLAG_CROSSBONE_ISLE_STACKED_PILLARS_FIRST_PILLAR_UNDER_ANOTHER + 1, EVENT(CrossboneIsleSteppingStones_MovePillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_PILLAR + 2, FLAG_CROSSBONE_ISLE_STACKED_PILLARS_FIRST_PILLAR_UNDER_ANOTHER + 2, EVENT(CrossboneIsleSteppingStones_MovePillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_PILLAR + 3, FLAG_CROSSBONE_ISLE_STACKED_PILLARS_FIRST_PILLAR_UNDER_ANOTHER + 3, EVENT(CrossboneIsleSteppingStones_MovePillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_FIRST_PILLAR, FLAG_CROSSBONE_ISLE_STACKED_PILLARS_FIRST_PILLAR_UNDER_ANOTHER, EVENT(CrossboneIsleSteppingStones_MovePillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_FIRST_PILLAR + 1, FLAG_CROSSBONE_ISLE_STACKED_PILLARS_FIRST_PILLAR_UNDER_ANOTHER + 1, EVENT(CrossboneIsleSteppingStones_MovePillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_FIRST_PILLAR + 2, FLAG_CROSSBONE_ISLE_STACKED_PILLARS_FIRST_PILLAR_UNDER_ANOTHER + 2, EVENT(CrossboneIsleSteppingStones_MovePillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_FIRST_PILLAR + 3, FLAG_CROSSBONE_ISLE_STACKED_PILLARS_FIRST_PILLAR_UNDER_ANOTHER + 3, EVENT(CrossboneIsleSteppingStones_MovePillar) },
    { TRIGGERS_END },
};
// clang-format on

struct Trigger gCrossboneIsleDragonStatueTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS,
     CROSSBONE_ISLE_DRAGON_STATUE_TO_CROSSBONE_ISLE_EIGHTH_FLOOR_EAST},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS,
     CROSSBONE_ISLE_DRAGON_STATUE_TO_CROSSBONE_ISLE_EIGHTH_FLOOR_WEST},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, 10, CONDITION_ALWAYS,
     EVENT(CrossboneIsleSteppingStones_PushSinkingPillar)},
    {EXAMINE_TRIGGER(EXAMINE_CHEST), 100,
     FLAG_CROSSBONE_ISLE_DRAGON_STATUE_888_COINS, FIND_COINS(888)},
    {EXAMINE_TRIGGER(EXAMINE_CHEST), 101,
     FLAG_CROSSBONE_ISLE_DRAGON_STATUE_POTION, FIND_ITEM(ITEM_POTION)},
    {EXAMINE_TRIGGER(EXAMINE_CHEST), 102,
     FLAG_CROSSBONE_ISLE_DRAGON_STATUE_CLERICS_RING,
     FIND_ITEM(ITEM_CLERICS_RING)},
    {EXAMINE_TRIGGER(EXAMINE_CHEST), 103,
     FLAG_CROSSBONE_ISLE_DRAGON_STATUE_SMOKE_BOMB, FIND_ITEM(ITEM_SMOKE_BOMB)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_SINKING_BOULDER, FLAG_CROSSBONE_ISLE_DRAGON_STATUE_BOULDER_SUNK,
     EVENT(CrossboneIsleSteppingStones_StartMovingSinkingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SINKING_BOULDER,
     FLAG_CROSSBONE_ISLE_DRAGON_STATUE_BOULDER_SUNK,
     EVENT(CrossboneIsleSteppingStones_MoveSinkingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_FIRST_PUDDLE,
     FLAG_CROSSBONE_ISLE_DRAGON_STATUE_FIRST_PUDDLE_FROZEN,
     EVENT(CrossboneIsleSteppingStones_FreezeFirstPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SECOND_PUDDLE,
     FLAG_CROSSBONE_ISLE_DRAGON_STATUE_FIRST_PUDDLE_FROZEN + 1,
     EVENT(CrossboneIsleSteppingStones_FreezeSecondPuddle)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_FROST, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_THIRD_PUDDLE,
     FLAG_CROSSBONE_ISLE_DRAGON_STATUE_FIRST_PUDDLE_FROZEN + 2,
     EVENT(CrossboneIsleSteppingStones_FrostStartsOnThirdPuddle)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_FROST, PSYNERGY_PHASE_ACTS),
     ACTOR_THIRD_PUDDLE,
     FLAG_CROSSBONE_ISLE_DRAGON_STATUE_FIRST_PUDDLE_FROZEN + 2,
     EVENT(CrossboneIsleSteppingStones_FrostRaisesPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_THIRD_PUDDLE,
     FLAG_CROSSBONE_ISLE_DRAGON_STATUE_FIRST_PUDDLE_FROZEN + 2,
     EVENT(CrossboneIsleSteppingStones_FreezeThirdPuddle)},
    {TRIGGERS_END},
};
