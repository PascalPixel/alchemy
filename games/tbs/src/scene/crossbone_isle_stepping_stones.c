/// @file
/// Five of the side rooms of Crossbone Isle Cave, the cave beneath Crossbone
/// Isle.
///
/// Each opens off one of the nine floors the cave descends through
/// (crossbone_isle.c) through two doors, east and west. The floor's east door
/// leads in at the room's `_EAST` entrance
/// (CROSSBONE_ISLE_STEPPING_STONES_FROM_CROSSBONE_ISLE_FIRST_FLOOR_EAST in the
/// first) and its west door at the `_WEST` one. The exits of the same names
/// lead back out.
///
/// Each is a puzzle with four chests, and all five take the area name
/// Crossbone Isle Cave. In the order the party meets them:
///
/// - SceneId_CrossboneIsleSteppingStones, off the first floor: raised squares
///   over a pit, with four stone pillars on them. Walking into a pillar jumps
///   Isaac across it where there is a square to land on, and otherwise pushes
///   it (CrossboneIsleSteppingStones_JumpOrPushPillar), so the pillars are
///   moved until a line of squares leads to each chest and to the far door.
///
/// - SceneId_CrossboneIsleIcePillars, off the second floor: columns lying
///   north to south and east to west, pushed by walking into them until the
///   way to the Mystery Blade's chest is clear.
///
/// - SceneId_CrossboneIsleFloodgate, off the fifth floor: a cave with a stream
///   and a floodgate across it, and logs to roll.
///
///   Searching the switch beside the floodgate
///   (CrossboneIsleSteppingStones_FlipWaterSwitch) closes it, so the water
///   backs up and the logs float higher, or opens it to drain them back down.
///   The water level lasts only while the party stays
///   (FLAG_CROSSBONE_ISLE_FLOODGATE_WATER_RAISED, kept through a battle).
///
/// - SceneId_CrossboneIsleStackedPillars, off the seventh floor, a cave of
///   cliffs, ladders and brown pits. Its square pillars are shifted with Move
///   and lifted onto ledges with Carry, and one can stand on another; each
///   settles after every push (CrossboneIsleSteppingStones_MovePillar). Pushed
///   off a cliff, a pillar drops out of play.
///
/// - SceneId_CrossboneIsleDragonStatue, off the eighth floor, below a stone
///   dragon: two logs, a small boulder and three puddles that Frost freezes
///   into pillars of ice.
///
///   The puddles are by the west ladder, by the east ladder, and in front of
///   the dragon under a white platform. Frost on that one lifts the platform
///   on a pillar of ice, and the dragon breathes fire on the puddle by the
///   east ladder, melting its ice. It freezes only if Isaac was not standing
///   in it.
///
///   Move pushes the small boulder below the logs east until it settles into
///   the floor two steps along, for good.
///
/// The other three side rooms are in crossbone_isle_keys.c, and the ninth
/// floor's in crossbone_isle.c.

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "macros.h"
#include "scene_ids.h"
#include "types.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/actor.h"
#include "field/event_commands.h"
#include "field/event_inline.h"
#include "field/field.h"
#include "field/field_effect.h"
#include "field/field_psynergy_state.h"
#include "field/field_sequence.h"
#include "field/ground_mask.h"
#include "field/map_grid_scroll.h"
#include "field/pushed_actor.h"
#include "field/scene_tables.h"
#include "field/script_operands.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "scene/field_effect_spawn.h"
#include "scene/import.h"
#include "sound/sound.h"
#include "sound/sound_driver_symbols.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "system/irq.h"
#include "system/main_routine_state.h"
#include "system/system.h"
#include "text/msg_ids.h"

extern s32 gCrossboneIsleSteppingStones_IceShineScript[];
extern u16 gCrossboneIsleSteppingStones_SwitchCloseSteps[];
extern u16 gCrossboneIsleSteppingStones_SwitchOpenSteps[];
extern u16 gCrossboneIsleSteppingStones_FloodgateCloseSteps[];
extern u16 gCrossboneIsleSteppingStones_FloodgateOpenSteps[];
extern u16 gCrossboneIsleSteppingStones_FloodgateFootSteps[];
extern struct SceneEntrance gScene_FallbackEntrances[];
extern struct SceneEntrance gCrossboneIsleSteppingStonesEntrances[];
extern struct SceneEntrance gCrossboneIsleIcePillarsEntrances[];
extern struct SceneEntrance gCrossboneIsleFloodgateEntrances[];
extern struct SceneEntrance gCrossboneIsleStackedPillarsEntrances[];
extern struct SceneEntrance gCrossboneIsleDragonStatueEntrances[];
extern u32 gCrossboneIsleSteppingStonesExits[];
extern struct ActorPlacement gCrossboneIsleSteppingStonesActorPlacements[];
extern struct ActorPlacement gCrossboneIsleIcePillarsActorPlacements[];
extern struct ActorPlacement gCrossboneIsleFloodgateActorPlacements[];
extern struct ActorPlacement gCrossboneIsleSteppingStonesActorPlacementsOther[];
extern struct ActorPlacement gCrossboneIsleDragonStatueActorPlacements[];
extern struct Trigger gCrossboneIsleSteppingStonesTriggersOther[];
extern struct Trigger gCrossboneIsleSteppingStonesTriggers[];
extern struct Trigger gCrossboneIsleIcePillarsTriggers[];
extern struct Trigger gCrossboneIsleFloodgateTriggers[];
extern struct Trigger gCrossboneIsleStackedPillarsTriggers[];
extern struct Trigger gCrossboneIsleDragonStatueTriggers[];

/// The placed actors, numbered in the order of each room's actor
/// placements (Scene_GetActorPlacements).
enum CrossboneIsleSteppingStonesActorId {
  /// The pillars of the first floor's side room (four, SPRITE_STONE_PILLAR),
  /// the second floor's side room (five) and the seventh floor's side room
  /// (four, SPRITE_SPIRAL_PILLAR), the only actors those rooms place.
  ACTOR_FIRST_PILLAR = ACTOR_FIRST_PLACED,
  /// The logs, SPRITE_LOG_NORTH_SOUTH and SPRITE_LOG_WEST_EAST as Kolima
  /// Forest's. There are eight in the floodgate room, where the seven
  /// after the first float on the raised water, and two in the dragon
  /// statue's room.
  ACTOR_FIRST_LOG = ACTOR_FIRST_PLACED,
  /// The dragon statue's room's boulder for Move, SPRITE_TURNING_ROCK, which
  /// sinks into place at SINKING_BOULDER_COLUMN.
  ACTOR_SINKING_BOULDER = ACTOR_FIRST_LOG + 2,
  /// The dragon statue's room's three puddles, SPRITE_PUDDLE, which Frost
  /// freezes into pillars of ice: by the west ladder, by the east ladder, and
  /// in front of the stone dragon.
  ACTOR_FIRST_PUDDLE,
  ACTOR_SECOND_PUDDLE,
  ACTOR_THIRD_PUDDLE,
  /// The white platform in the puddle in front of the stone dragon,
  /// SPRITE_WHITE_PLATFORM, which a pillar of ice lifts two steps as Frost
  /// freezes the puddle.
  ACTOR_RISING_PILLAR,
  /// The floodgate room's water, SPRITE_SPLASH, raised behind the closed
  /// floodgate.
  ACTOR_WATER = ACTOR_FIRST_LOG + 8
};

/// The rooms' chests, named for what each holds; each room numbers its own.
enum {
  TREASURE_SPOT_STEPPING_STONES_ANTIDOTE = TREASURE_SPOT_FIRST_ID,
  TREASURE_SPOT_STEPPING_STONES_NUT = TREASURE_SPOT_FIRST_ID + 1,
  TREASURE_SPOT_STEPPING_STONES_111_COINS = TREASURE_SPOT_FIRST_ID + 2,
  TREASURE_SPOT_STEPPING_STONES_HARD_NUT = TREASURE_SPOT_FIRST_ID + 3,
  TREASURE_SPOT_ICE_PILLARS_LUCKY_PEPPER = TREASURE_SPOT_FIRST_ID,
  TREASURE_SPOT_ICE_PILLARS_MYSTERY_BLADE = TREASURE_SPOT_FIRST_ID + 1,
  TREASURE_SPOT_ICE_PILLARS_222_COINS = TREASURE_SPOT_FIRST_ID + 2,
  TREASURE_SPOT_ICE_PILLARS_ELIXIR = TREASURE_SPOT_FIRST_ID + 3,
  TREASURE_SPOT_FLOODGATE_555_COINS = TREASURE_SPOT_FIRST_ID,
  TREASURE_SPOT_FLOODGATE_LUCKY_MEDAL = TREASURE_SPOT_FIRST_ID + 1,
  TREASURE_SPOT_FLOODGATE_POTION = TREASURE_SPOT_FIRST_ID + 2,
  TREASURE_SPOT_FLOODGATE_NINJA_GARB = TREASURE_SPOT_FIRST_ID + 3,
  TREASURE_SPOT_STACKED_PILLARS_777_COINS = TREASURE_SPOT_FIRST_ID,
  TREASURE_SPOT_STACKED_PILLARS_LUCKY_MEDAL = TREASURE_SPOT_FIRST_ID + 1,
  TREASURE_SPOT_STACKED_PILLARS_PSY_CRYSTAL = TREASURE_SPOT_FIRST_ID + 2,
  TREASURE_SPOT_STACKED_PILLARS_WICKED_MACE = TREASURE_SPOT_FIRST_ID + 3,
  TREASURE_SPOT_DRAGON_STATUE_888_COINS = TREASURE_SPOT_FIRST_ID,
  TREASURE_SPOT_DRAGON_STATUE_POTION = TREASURE_SPOT_FIRST_ID + 1,
  TREASURE_SPOT_DRAGON_STATUE_CLERICS_RING = TREASURE_SPOT_FIRST_ID + 2,
  TREASURE_SPOT_DRAGON_STATUE_SMOKE_BOMB = TREASURE_SPOT_FIRST_ID + 3
};

/// The floodgate room: the water has been raised.
#define FLAG_CROSSBONE_ISLE_FLOODGATE_WATER_RAISED FLAG_SCENE_TEMP_00
/// The seventh floor's side room: the first pillar has another pushed on top of
/// it, so Move and Carry leave it be. The four pillars' flags follow from this
/// one.
#define FLAG_CROSSBONE_ISLE_STACKED_PILLARS_FIRST_PILLAR_UNDER_ANOTHER         \
  FLAG_SCENE_TEMP_00
/// The dragon statue's room: the first puddle is frozen. The second's and
/// third's flags follow from this one.
#define FLAG_CROSSBONE_ISLE_DRAGON_STATUE_FIRST_PUDDLE_FROZEN FLAG_SCENE_TEMP_00
/// The dragon statue's room: the leader stood on the third puddle when Frost
/// began, so it stays unfrozen.
#define FLAG_CROSSBONE_ISLE_DRAGON_STATUE_LEADER_ON_THIRD_PUDDLE               \
  FLAG_SCENE_TEMP_03
/// The dragon statue's room: the boulder Move pushes has sunk into place at
/// SINKING_BOULDER_COLUMN.
#define FLAG_CROSSBONE_ISLE_DRAGON_STATUE_BOULDER_SUNK FLAG_SCENE_TEMP_04

/// The map column where the dragon statue's room's boulder sinks into place.
#define SINKING_BOULDER_COLUMN 16

/// The seventh floor's side room's own cell behaviors, on layer 2: where a
/// pillar pushed onto one falls down the cliff, and where it sinks into the
/// floor. Either way it then leaves play.
enum {
  CELL_BEHAVIOR_STACKED_PILLARS_DROP = 50,
  CELL_BEHAVIOR_STACKED_PILLARS_SINK = 51
};

/// The seventh floor's side room's cells that run events as Isaac walks: among
/// the pillars, at the ladders, on the lowest floor and where he leaves it.
enum {
  CELL_BEHAVIOR_STACKED_PILLARS_PILLARS = 30,
  CELL_BEHAVIOR_STACKED_PILLARS_LADDER = 31,
  CELL_BEHAVIOR_STACKED_PILLARS_LOWEST_FLOOR = 40,
  CELL_BEHAVIOR_STACKED_PILLARS_LEAVE_LOWEST_FLOOR = 41
};

/// SPRITE_TURNING_ROCK's round boulder (turning_rock.tsv's picture 1), as the
/// dragon statue's room's boulder is drawn.
#define TURNING_ROCK_ANIMATION_BOULDER 2

/// The cells beside the pillars, columns, logs and boulder of the first,
/// second, fifth and eighth floors' side rooms, from which walking into one
/// pushes it.
enum { CELL_BEHAVIOR_BESIDE_PUSHED_ACTOR = 10 };

/// The floodgate room's switch, searched to flip it.
enum { CELL_BEHAVIOR_FLOODGATE_SWITCH = 20 };

/// While the floodgate room's water changes level, the background maps
/// hold the stream in its other level twelve cell rows below where they show
/// it (MapGrid_DrawCellPanel's rows 19 and 31), and the top
/// WATER_DISSOLVE_LINES lines of the screen show it scrolled down to there,
/// line by line, more often each frame.
#define WATER_PANEL_SCROLL_Y (12 * MAPGRID_CELL_PIXELS)

/// The floodgate room's map: its layers 1 and 2 begin at the map's columns 32
/// and 64, and each water level is kept below the room as those two layers
/// side by side, the raised water from row 32 and the drained from row 64.
/// The stream the level changes in is the 16 by 5 cells from (10, 19).
enum {
  FLOODGATE_LAYER_1_COLUMN = 32,
  FLOODGATE_LAYER_2_COLUMN = 64,
  RAISED_WATER_ROW = 32,
  DRAINED_WATER_ROW = 64,
  STREAM_COLUMN = 10,
  STREAM_ROW = 19,
  /// Where the stream at its other level is drawn, WATER_PANEL_SCROLL_Y below.
  WATER_PANEL_ROW = STREAM_ROW + 12
};
#define WATER_DISSOLVE_LINES 48

/// A damped particle loses a PARTICLE_DRAG-th of its speed across the ground
/// each frame, and a PARTICLE_DRAG_Y-th of its speed up or down.
enum { PARTICLE_DRAG = 10, PARTICLE_DRAG_Y = 3 };

/// A copied map cell, also read in the two flag pairs the pillar redraw keeps.
union CrossboneIslePillarCell {
  struct MapGridCell cell;
  struct CrossboneIslePillarCellBits {
    u32 cellTiles : 12;
    u32 lowFlags : 2; ///< bits 13..12 of MapGridCell.cellTilesAndFlags
    u8 highFlags : 2; ///< bits 15..14, read by MapGrid_GetCellHighFlags
    u8 behavior;
    u8 shape;
  } bits;
};

STATIC_ASSERT(sizeof(union CrossboneIslePillarCell) ==
              sizeof(struct MapGridCell));
STATIC_ASSERT(offsetof(union CrossboneIslePillarCell, bits.behavior) == 2);
STATIC_ASSERT(offsetof(union CrossboneIslePillarCell, bits.shape) == 3);

#include "shared/field_effect_create.c"
#include "shared/field_effect_spawn.c"
#include "shared/pushed_actor.c"

#include "shared/map_grid_panel.c"

/// Waits up to sixty frames for landing, then stops vertical movement on the
/// ground.
static void
CrossboneIsleSteppingStones_SettleActorOnGround(struct Actor *actor) {
  s32 remainingFrames = 60;

  for (;;) {
    if (remainingFrames == 0) {
      break;
    }
    wait(1);
    if (actor->y.fixed == actor->groundHeight) {
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
    wait(1);
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

/// Finds the cells the actor covers: their width and depth, the top-left one,
/// and the cell where the map grid's layer 1 starts.
///
/// The actor's edges are its row of gPushedActor_Rectangles. An actor whose
/// animation is none of the six kinds gets PUSHED_ACTOR_NO_RECTANGLE, past
/// them, and is refused.
static s32 CrossboneIsleSteppingStones_QueryCoveredCells(
    s32 id, u32 *width, u32 *depth, struct PushedActorProbe *probe,
    s32 *layerOriginX, s32 *layerOriginZ) {
  struct MapGridScrollState *grid = gMapGridState[0];
  struct Actor *actor = Actor_Get(id);
  u32 i;
  s32 coordinate;
  s32 farExtent;

  for (i = 0; i <= 5; i++) {
    if (actor->sprite->entries[0]->animationId ==
        gPushedActor_RectangleKinds[i]) {
      probe->rectangleIndex = i;
      break;
    }
    probe->rectangleIndex = PUSHED_ACTOR_NO_RECTANGLE;
  }
  if (probe->rectangleIndex < 0 || probe->rectangleIndex > 6) {
    return 0;
  }
  // Bounds are pixel offsets; dimensions and origins below are 16-pixel cells.
  probe->position.x = actor->x.fixed;
  probe->position.y = actor->y.fixed;
  probe->position.z = actor->z.fixed;
  coordinate = gPushedActor_Rectangles[probe->rectangleIndex].z0;
  if (coordinate < 0) {
    coordinate = -coordinate;
  }
  farExtent = gPushedActor_Rectangles[probe->rectangleIndex].z1;
  if (farExtent < 0) {
    farExtent = -farExtent;
  }
  *depth = (coordinate + farExtent) >> 4;
  coordinate = gPushedActor_Rectangles[probe->rectangleIndex].x0;
  if (coordinate < 0) {
    coordinate = -coordinate;
  }
  farExtent = gPushedActor_Rectangles[probe->rectangleIndex].x1;
  if (farExtent < 0) {
    farExtent = -farExtent;
  }
  *width = (coordinate + farExtent) >> 4;
  // Move from the actor's center to the top-left cell it covers.
  probe->position.x +=
      INTtoFIXED(gPushedActor_Rectangles[probe->rectangleIndex].x0);
  {
    s32 offsetZ = INTtoFIXED(gPushedActor_Rectangles[probe->rectangleIndex].z0);

    coordinate = probe->position.z;
    coordinate += offsetZ;
    coordinate = FIELD_CELL(coordinate);
    probe->position.z = coordinate;
  }
  probe->position.x = FIELD_CELL(probe->position.x);
  *layerOriginX = FIELD_CELL(grid->layers[1].offsetX);
  *layerOriginZ = FIELD_CELL(grid->layers[1].offsetY);
  return 1;
}

/// Floats one of the floodgate room's logs on the raised water: it bobs
/// under the leader's feet, and its cells are redrawn as a floating log lying
/// across or along the room as it does.
static s32 CrossboneIsleSteppingStones_FloatLog(s32 actorId) {
  struct Actor *actor = Actor_Get(actorId);
  u32 width, depth;
  s32 layerOriginX, layerOriginZ;
  struct PushedActorProbe probe;
  s32 cellX, cellZ;

  if (CrossboneIsleSteppingStones_QueryCoveredCells(
          actorId, &width, &depth, &probe, &layerOriginX, &layerOriginZ) == 0) {
    return 0;
  }

  cellX = probe.position.x;
  cellZ = probe.position.z;
  MapGrid_CopyCellAttributes(2, 2, width, depth, cellX, cellZ);

  Actor_SelectAnimation(actor, LOG_ANIMATION_FLOAT);
  actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;

  if (width > depth) {
    MapGrid_CopyCellsTo(70, 40, probe.position.x + 32, probe.position.z + 2,
                        width, depth);
  } else {
    MapGrid_CopyCellsTo(68, 40, probe.position.x + 32, probe.position.z + 2,
                        width, depth);
  }

  return 1;
}

/// Floats the floodgate room's seven logs on the raised water.
static inline void CrossboneIsleSteppingStones_FloatLogs(void) {
  CrossboneIsleSteppingStones_FloatLog(ACTOR_FIRST_LOG + 1);
  CrossboneIsleSteppingStones_FloatLog(ACTOR_FIRST_LOG + 2);
  CrossboneIsleSteppingStones_FloatLog(ACTOR_FIRST_LOG + 3);
  CrossboneIsleSteppingStones_FloatLog(ACTOR_FIRST_LOG + 4);
  CrossboneIsleSteppingStones_FloatLog(ACTOR_FIRST_LOG + 5);
  CrossboneIsleSteppingStones_FloatLog(ACTOR_FIRST_LOG + 6);
  CrossboneIsleSteppingStones_FloatLog(ACTOR_FIRST_LOG + 7);
}

/// Strands one of the floodgate room's logs on the floor as the water
/// drains: the cells under it are restored from the map and walled off, and it
/// lies still.
static s32 CrossboneIsleSteppingStones_StrandLog(s32 actorId) {
  struct Actor *actor = Actor_Get(actorId);
  u32 width, depth;
  s32 layerOriginX, layerOriginZ;
  struct PushedActorProbe probe;

  if (CrossboneIsleSteppingStones_QueryCoveredCells(
          actorId, &width, &depth, &probe, &layerOriginX, &layerOriginZ) == 0) {
    return 0;
  }

  {
    s32 sourceCellX = layerOriginX + probe.position.x;
    s32 sourceCellZ = layerOriginZ + probe.position.z;

    MapGrid_CopyCellAttributes(sourceCellX, sourceCellZ, width, depth,
                               probe.position.x, probe.position.z);
    MapGrid_FillCollisionRectangle(0, probe.position.x, probe.position.z, width,
                                   depth, CELL_BEHAVIOR_WALL);
  }

  Actor_SelectAnimation(actor, LOG_ANIMATION_AT_REST);
  actor->priorityFlags &= ~ACTOR_PRIORITY_UNDERFOOT;

  return 1;
}

/// Waits until the DMA channel clears its enable bit.
static inline void Dma_Wait(volatile u32 *dma) {
  while (dma[2] & DMA_BUSY)
    ;
}

/// Reads one map cell through DMA, waiting until the transfer finishes.
static void CrossboneIsleSteppingStones_ReadMapGridCell(
    s32 layer, s32 x, s32 y, union CrossboneIslePillarCell *dst) {
  struct MapGridScrollState *grid = gMapGridState[0];

  if (grid != NULL) {
    struct MapGridCell *cell = grid->layers[layer].cells;

    cell += x + (y << MAPGRID_ROW_SHIFT);
    Dma_Set(cell, dst, DMA_COPY_WORDS(sizeof(*cell) / sizeof(u32)), REG_DMA3);
    Dma_Wait(REG_DMA3);
  }
}

/// Copies the cell's flags and cell behavior without replacing its
/// cell tiles.
static void CrossboneIsleSteppingStones_SetCellAttributes(
    s32 layer, s32 x, s32 y, const union CrossboneIslePillarCell *source) {
  struct MapGridScrollState *grid = gMapGridState[0];

  if (grid != NULL) {
    struct CrossboneIslePillarCellBits *cell =
        (struct CrossboneIslePillarCellBits *)grid->layers[layer].cells;

    cell += x + (y << MAPGRID_ROW_SHIFT);
    cell->lowFlags = source->bits.lowFlags;
    cell->highFlags = source->bits.highFlags;
    cell->behavior = source->bits.behavior;
    cell->shape = source->bits.shape;
  }
}

/// Lets the actor fall down the cliff south of it to the next ledge, then
/// takes it out of play, to the map's origin. Returns whether a ledge was
/// found.
static s32 CrossboneIsleSteppingStones_LowerActorToLedge(s32 id, s32 far) {
  struct Actor *actor = Actor_Get(id);
  s32 found;
  struct Actor *temporaryActor;
  FIXED height;
  s32 level;
  s32 top;
  u32 row;
  u32 rows;
  s32 x;
  s32 y;
  FIXED z;

  found = 0;
  temporaryActor = NULL;
  Actor_SetSpriteFlags(actor, 0);
  height = MapGrid_GetTerrainHeight(2, actor->x.fixed, actor->z.fixed);
  level = height / toFIXED(16);
  rows = level;
  if (level < 0) {
    rows = -level;
  }
  rows++;
  for (row = 0; row <= rows; row++) {
    top = MapGrid_GetTerrainHeight(actor->layer, actor->x.fixed,
                                   FIELD_CELL_ORIGIN(row) + actor->z.fixed);
    top /= toFIXED(16);
    if (level < top) {
      x = FIELD_CELL_CENTER(FIELD_CELL(actor->x.fixed));
      // A black, see-through crate or capped pillar stands at the ledge
      // until the actor has stopped.
      if (far == 0) {
        row += 2;
        z = FIELD_CELL_ORIGIN(row + FIELD_CELL(actor->z.fixed)) + toFIXED(2);
        y = FIELD_CELL_ORIGIN(top);
        temporaryActor = Actor_PrepareFX(x, y, z, SPRITE_CRATE);
        height = actor->z.fixed - z + y;
      } else {
        row += 3;
        z = FIELD_CELL_ORIGIN(row + FIELD_CELL(actor->z.fixed)) - toFIXED(2);
        y = FIELD_CELL_ORIGIN(top);
        temporaryActor = Actor_PrepareFX(x, y, z, SPRITE_CAPPED_PILLAR);
        height = actor->z.fixed - z + y;
      }
      found = 1;
      break;
    }
  }
  CrossboneIsleSteppingStones_StopAtGroundOrHeight(actor, height);
  actor->x.fixed = 0;
  actor->y.fixed = 0;
  actor->z.fixed = 0;
  if (temporaryActor != NULL) {
    Actor_Release(temporaryActor);
  }
  return found;
}

/// Sinks the pillar into the floor, a tenth of its height and 0.8 pixels a
/// frame, until a tenth of it is left (at most 32 frames).
static void CrossboneIsleSteppingStones_SinkPillar(s32 index) {
  struct Actor *actor = Actor_Get(index);
  u32 elapsedFrames;

  actor->motionFlags = 0;

  elapsedFrames = 0;
  for (;;) {
    if (elapsedFrames > 31)
      return;
    wait(1);
    actor->scaleY -= FIXED_ONE / 10;
    actor->y.fixed -= toFIXED(4) / 5;
    elapsedFrames++;
    if (actor->scaleY <= FIXED_ONE / 10 - 1) {
      actor->scaleY = FIXED_ONE / 10;
      return;
    }
  }
}

/// Unused: spawns a particle near the leader once every four frames; nothing
/// calls it.
UNUSED static void CrossboneIsleSteppingStones_SpawnUnusedLeaderParticle(void) {
  struct Actor *leader;
  s32 cycleFrame;
  FIXED x;
  FIXED z;
  s32 velocityY;
  struct EffectOptions options;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  cycleFrame = gFrameCount & 3;
  if (cycleFrame == 0) {
    options.palette = SPRITE_COLOR_RED_TINT;
    options.startScaleX = 7 * FIXED_ONE / 10;
    options.startScaleY = 7 * FIXED_ONE / 10;
    x = leader->x.fixed + INTtoFIXED(Random_Below(17) - 8);
    z = leader->z.fixed + INTtoFIXED(Random_Below(17) - 8);
    velocityY = Math_Divide(INTtoFIXED(Random_Below(5)) + toFIXED(3), 10);
    FieldEffect_Spawn(x, leader->y.fixed, z, 0, velocityY, cycleFrame,
                      EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE |
                          EFFECT_SCRIPT_MEDIUM,
                      &options);
  }
}

/// Jumps two cells forward when both the intervening and landing cells permit
/// it.
static s32 CrossboneIsleSteppingStones_TryJumpAcrossPillar(void) {
  struct Actor *leader;
  s32 savedMotionFlags;
  s32 jumpDirection;
#if !TBS_PATCH_1
  s32 step;
#endif
  struct FieldPosition destination;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  savedMotionFlags = leader->motionFlags;
#if TBS_PATCH_1
  jumpDirection = FACING_NEAREST_CARDINAL(leader->facing);
#endif
  if (gMainRoutineState.movementMode != LEADER_MOVEMENT_WALK) {
    return 0;
  }
#if TBS_PATCH_1
  destination.x = (leader->x.fixed & FIELD_CELL_MASK) + toFIXED(8);
  destination.y = leader->y.fixed;
  destination.z = (leader->z.fixed & FIELD_CELL_MASK) + toFIXED(8);
  Vector_AddPolarOffset(toFIXED(16), jumpDirection, &destination);
#else
  jumpDirection = FACING_INDEX(leader->facing);
  Actor_StepFrom(&destination, leader,
                 gPushedActor_DirectionSteps[jumpDirection]);
#endif
  if (FieldActor_CheckMovementCollision(leader, &destination) != 1 &&
      PushedActor_FindAtTile(&destination, leader) == 0) {
#if TBS_PATCH_1
    destination.x = (leader->x.fixed & FIELD_CELL_MASK) + toFIXED(8);
    destination.y = leader->y.fixed;
    destination.z = (leader->z.fixed & FIELD_CELL_MASK) + toFIXED(8);
    Vector_AddPolarOffset(toFIXED(32), jumpDirection, &destination);
#else
    // Two steps on.
    step = gPushedActor_DirectionSteps[jumpDirection];
    destination.x = leader->x.fixed + (DIRECTION_STEP_X_FIXED(step) * 2);
    destination.y = leader->y.fixed;
    step = DIRECTION_STEP_Z_FIXED(step) * 2;
    destination.z = leader->z.fixed + step;
#endif
    if (PushedActor_FindAtTile(&destination, leader) == 0 &&
        FieldActor_CheckMovementCollision(leader, &destination) == 0) {
      Event_Start();
      Actor_SelectAnimation(leader, ANIM_LEADER_SPRING);
      wait(6);
      Sound_Play(SE_JUMP);
      Actor_SelectAnimation(leader, ANIM_LEADER_LEAP);
      leader->speedLimit = toFIXED(3);
      leader->acceleration = toFIXED(2);
      leader->velocityY = toFIXED(4);
      leader->motionFlags &= ACTOR_JUMP_KEEPS_MOTION;
      Actor_SetSpriteFlags(leader, 0);
      Actor_MoveToAndWait(ACTOR_PARTY_LEADER, FIXEDtoINT(destination.x),
                          FIXEDtoINT(destination.z));
      Actor_SelectAnimation(leader, ANIM_LEADER_SPRING);
      Actor_SetSpriteFlags(leader, SPRITE_FLAG_SHADOW);
      leader->motionFlags = savedMotionFlags;
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
  struct FieldEffect *effect = (struct FieldEffect *)actor;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;

  effect->x += effect->velocity.x;
  effect->y += effect->velocity.y;
  effect->z += effect->velocity.z;
  velocityX = effect->velocity.x;
  velocityY = effect->velocity.y;
  velocityZ = effect->velocity.z;
  effect->velocity.x = velocityX - Math_Divide(velocityX, PARTICLE_DRAG);
  effect->velocity.y = velocityY - Math_Divide(velocityY, PARTICLE_DRAG_Y);
  effect->velocity.z = velocityZ - Math_Divide(velocityZ, PARTICLE_DRAG);
  effect->scaleX += effect->scaleRateX;
  effect->scaleY += effect->scaleRateY;
  effect->sprite->rotation += effect->spin;
}

/// Sets the actor's sprite to the colour mode (enum SpriteColorMode) in the
/// low four bits of its timer.
static s32
CrossboneIsleSteppingStones_SetSpriteColorFromTimer(struct Actor *actor) {
  Actor_SetSpriteColor(actor, actor->timer & 15);
  return 0;
}

const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 sceneId = gMainRoutineState.next;

  if (sceneId == SCENE_CROSSBONE_ISLE_STEPPING_STONES) {
    return gCrossboneIsleSteppingStonesEntrances;
  }
  if (sceneId == SCENE_CROSSBONE_ISLE_ICE_PILLARS) {
    return gCrossboneIsleIcePillarsEntrances;
  }
  if (sceneId == SCENE_CROSSBONE_ISLE_FLOODGATE) {
    return gCrossboneIsleFloodgateEntrances;
  }
  if (sceneId == SCENE_CROSSBONE_ISLE_STACKED_PILLARS) {
    return gCrossboneIsleStackedPillarsEntrances;
  }
  if (sceneId == SCENE_CROSSBONE_ISLE_DRAGON_STATUE) {
    return gCrossboneIsleDragonStatueEntrances;
  }
  return gScene_FallbackEntrances;
}

const struct SceneExitRegion *Scene_GetExitRegions(void) { return 0; }

/// Returns the exits the five rooms share.
const u32 *Scene_GetExits(void) { return gCrossboneIsleSteppingStonesExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s16 sceneId = gMainRoutineState.next;

  if (sceneId == SCENE_CROSSBONE_ISLE_STEPPING_STONES) {
    return gCrossboneIsleSteppingStonesActorPlacements;
  }
  if (sceneId == SCENE_CROSSBONE_ISLE_ICE_PILLARS) {
    return gCrossboneIsleIcePillarsActorPlacements;
  }
  if (sceneId == SCENE_CROSSBONE_ISLE_FLOODGATE) {
    return gCrossboneIsleFloodgateActorPlacements;
  }
  if (sceneId == SCENE_CROSSBONE_ISLE_DRAGON_STATUE) {
    return gCrossboneIsleDragonStatueActorPlacements;
  }
  return gCrossboneIsleSteppingStonesActorPlacementsOther;
}

/// Isaac walks into one of the stone pillars on the raised squares of the
/// first floor's side room: he jumps across it if there is a square beyond to
/// land on, and otherwise pushes it a step.
static void CrossboneIsleSteppingStones_JumpOrPushPillar(void) {
  Event_Start();
  if (CrossboneIsleSteppingStones_TryJumpAcrossPillar() == 0)
    PushedActor_AdvancePair();
  Event_End();
}

/// Isaac walks into one of the lying columns of the second floor's side room,
/// or a log in the fifth floor's room of the floodgate, and pushes it if the
/// way beyond is clear.
static void CrossboneIsleSteppingStones_PushPillar(void) {
  struct PushedActorProbe out;

  Event_Start();
  if (PushedActor_FindClearPosition(&out) != 0)
    PushedActor_Roll(out);
  Event_End();
}

/// A pillar slot's place and ground cell, without its flag. It holds one slot
/// while two are swapped.
struct PillarPlacement {
  s32 x; ///< 16-pixel map cells
  s32 y; ///< 16-pixel map cells
  s32 z; ///< 16-pixel map cells
  union CrossboneIslePillarCell cell;
};

/// Where one of the seventh floor's side room's pillars stands, the ground cell
/// saved under it and its flag of standing under another pillar.
struct PillarSlot {
  s32 x; ///< 16-pixel map cells
  s32 y; ///< 16-pixel map cells
  s32 z; ///< 16-pixel map cells
  union CrossboneIslePillarCell cell;
  s32 flag;
};

/// The side room's four pillars.
static struct PillarSlot sPillarSlots[4] ALIGNED(16);
/// Where a moved pillar stands, tested against the map's collision.
static struct FieldPosition sProbePosition;
/// BG1 to BG3's scroll moved down to the stream's other level
/// (CrossboneIsleSteppingStones_PrepareWaterScroll).
static struct BgScroll sOtherWaterScroll[3];
/// The percent chance that a line of the top WATER_DISSOLVE_LINES shows the
/// other water level: 0 when the water starts to change, then a point higher
/// each frame until every line shows it.
static s32 sOtherWaterChance;

/// HBlank handler while the water changes level: the next line, if one of the
/// top WATER_DISSOLVE_LINES, shows the other water level with
/// sOtherWaterChance, else the backgrounds' own scroll.
static void CrossboneIsleSteppingStones_JitterBackgroundScroll(void) {
  u32 line = REG_VCOUNT;
  u32 *src = &((u32 *)gBgScroll)[1];
  volatile u32 *reg = (volatile u32 *)&REG_BG1HOFS;

  if (line == DISPLAY_TOTAL_LINES - 1 || line <= WATER_DISSOLVE_LINES - 2) {
    if (Random_Below(100) < sOtherWaterChance) {
      src = ((u32 *)sOtherWaterScroll);
    }
  }
  *reg = *src++;
  reg = (volatile u32 *)&REG_BG2HOFS;
  *reg++ = *src++;
  *reg = *src;
}

/// Copies BG1 to BG3's scroll and moves it down WATER_PANEL_SCROLL_Y, to the
/// stream in its other level. Task, each frame while the water changes level.
static void CrossboneIsleSteppingStones_PrepareWaterScroll(void) {
  u32 *destination;
  const u32 *source;
  struct BgScroll *other;

  source = (const u32 *)&((u32 *)gBgScroll)[1];
  destination = (u32 *)sOtherWaterScroll;
  *destination++ = *source++;
  *destination++ = *source++;
  *destination = *source;
  other = sOtherWaterScroll;
  other[0].y += WATER_PANEL_SCROLL_Y;
  other[1].y += WATER_PANEL_SCROLL_Y;
  other[2].y += WATER_PANEL_SCROLL_Y;
}

/// The floodgate room's water changes level: over 100 frames, more and
/// more of the top lines show the stream at its other level, then the map
/// takes it.
static void CrossboneIsleSteppingStones_AnimateWaterLevel(void) {
  s32 waterRaised;

  waterRaised = GameFlag_Get(FLAG_CROSSBONE_ISLE_FLOODGATE_WATER_RAISED);
  if (waterRaised == 0) {
    MapGrid_DrawCellPanel(STREAM_COLUMN, STREAM_ROW, 16, 5, waterRaised,
                          STREAM_COLUMN, WATER_PANEL_ROW);
    MapGrid_DrawCellPanel(STREAM_COLUMN, RAISED_WATER_ROW + STREAM_ROW, 16, 5,
                          1, STREAM_COLUMN, WATER_PANEL_ROW);
    MapGrid_DrawCellPanel(32 + STREAM_COLUMN, RAISED_WATER_ROW + STREAM_ROW, 16,
                          5, 2, STREAM_COLUMN, WATER_PANEL_ROW);
  } else {
    MapGrid_DrawCellPanel(STREAM_COLUMN, STREAM_ROW, 16, 5, 0, STREAM_COLUMN,
                          WATER_PANEL_ROW);
    MapGrid_DrawCellPanel(STREAM_COLUMN, DRAINED_WATER_ROW + STREAM_ROW, 16, 5,
                          1, STREAM_COLUMN, WATER_PANEL_ROW);
    MapGrid_DrawCellPanel(32 + STREAM_COLUMN, DRAINED_WATER_ROW + STREAM_ROW,
                          16, 5, 2, STREAM_COLUMN, WATER_PANEL_ROW);
  }
  sOtherWaterChance = 0;
  Task_AddFrame(CrossboneIsleSteppingStones_PrepareWaterScroll,
                TASK_PASS_FRAME);
  wait(1);
  Irq_SetHandler(IRQ_HBLANK, 0,
                 CrossboneIsleSteppingStones_JitterBackgroundScroll);
  Sound_Play(SE_WATER_LEVEL);
  for (sOtherWaterChance = 0; sOtherWaterChance <= 100; sOtherWaterChance++) {
    wait(1);
  }
  // The map takes the new level for good.
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  if (GameFlag_Get(FLAG_CROSSBONE_ISLE_FLOODGATE_WATER_RAISED) == 0) {
    MapGrid_CopyCellsTo(0, RAISED_WATER_ROW, FLOODGATE_LAYER_1_COLUMN, 0, 32,
                        32);
    MapGrid_CopyCellsTo(32, RAISED_WATER_ROW, FLOODGATE_LAYER_2_COLUMN, 0, 32,
                        32);
  } else {
    MapGrid_CopyCellsTo(0, DRAINED_WATER_ROW, FLOODGATE_LAYER_1_COLUMN, 0, 32,
                        32);
    MapGrid_CopyCellsTo(32, DRAINED_WATER_ROW, FLOODGATE_LAYER_2_COLUMN, 0, 32,
                        32);
  }
  wait(1);
  Irq_SetHandler(IRQ_HBLANK, 0, 0);
  wait(1);
  Task_Remove(CrossboneIsleSteppingStones_PrepareWaterScroll);
  MapGrid_CenterOnFollowedPoint();
  wait(30);
}

/// Isaac searches the switch beside the floodgate in the fifth floor's side
/// room: "Isaac flipped the switch." With the gate open it shuts, spray bursts
/// at its foot, the water rises behind it and the logs float up; with it shut,
/// it opens, the water drains and the logs settle on the stream bed.
static void CrossboneIsleSteppingStones_FlipWaterSwitch(void) {
  s32 waterRaised;

  Event_Start();
  FieldCamera_SetSpeed(1);
  FieldCamera_MoveTo(281, CAMERA_KEEP, 432, 1);
  FieldCamera_WaitForMove();
  FieldMessage_Show(MsgFieldFlippedSwitch, 1);
  waterRaised = Event_GetFlag(FLAG_CROSSBONE_ISLE_FLOODGATE_WATER_RAISED);
  if (waterRaised == 0) {
    Sound_Play(SE_CRUNCH);
    MapGrid_AnimateCells(gCrossboneIsleSteppingStones_SwitchCloseSteps, 84, 24);
    Event_Wait(30);
    Sound_Play(SE_LOG_FALL);
    Actor_SetSpritePriority(ACTOR_WATER, 1);
    Actor_Get(ACTOR_WATER)->motionFlags = waterRaised;
    Actor_Get(ACTOR_WATER)->y.fixed = -toFIXED(32);
    Actor_SetPosition(ACTOR_WATER, 272, 416);
    Actor_SetAnimation(ACTOR_WATER, SPLASH_ANIMATION_BURST);
    MapGrid_AnimateCells(gCrossboneIsleSteppingStones_FloodgateCloseSteps, 80,
                         24);
    MapGrid_AnimateCells(gCrossboneIsleSteppingStones_FloodgateFootSteps, 80,
                         28);
    MapGrid_CopyCellsTo(65, 40, 16, 27, 2, 4);
    CrossboneIsleSteppingStones_AnimateWaterLevel();
    CrossboneIsleSteppingStones_FloatLogs();
    MapGrid_CopyCellAttributes(24, 3, 1, 1, 24, 8);
    GameFlag_Set(FLAG_CROSSBONE_ISLE_FLOODGATE_WATER_RAISED);
  } else {
    Sound_Play(SE_CRUNCH);
    MapGrid_AnimateCells(gCrossboneIsleSteppingStones_SwitchOpenSteps, 84, 24);
    Event_Wait(30);
    Sound_Play(SE_WATER_DRAIN);
    Actor_Get(ACTOR_WATER)->motionFlags = 0;
    Actor_Get(ACTOR_WATER)->y.fixed = -toFIXED(32);
    Actor_SetPosition(ACTOR_WATER, 272, 436);
    Actor_SetAnimation(ACTOR_WATER, SPLASH_ANIMATION_CHURN);
    MapGrid_CopyCellsTo(65, 45, 16, 27, 2, 4);
    MapGrid_AnimateCells(gCrossboneIsleSteppingStones_FloodgateOpenSteps, 80,
                         24);
    CrossboneIsleSteppingStones_AnimateWaterLevel();
    CrossboneIsleSteppingStones_StrandLog(ACTOR_FIRST_LOG + 1);
    CrossboneIsleSteppingStones_StrandLog(ACTOR_FIRST_LOG + 2);
    CrossboneIsleSteppingStones_StrandLog(ACTOR_FIRST_LOG + 3);
    CrossboneIsleSteppingStones_StrandLog(ACTOR_FIRST_LOG + 4);
    CrossboneIsleSteppingStones_StrandLog(ACTOR_FIRST_LOG + 5);
    CrossboneIsleSteppingStones_StrandLog(ACTOR_FIRST_LOG + 6);
    CrossboneIsleSteppingStones_StrandLog(ACTOR_FIRST_LOG + 7);
    MapGrid_CopyCellAttributes(24, 4, 1, 1, 24, 8);
    GameFlag_Clear(FLAG_CROSSBONE_ISLE_FLOODGATE_WATER_RAISED);
  }
  Event_End();
}

/// When actor stands just behind front, up to two cells north of it, within a
/// cell east or west and at the same height, it is drawn no nearer the viewer
/// than front: it takes front's sprite priority (the background layers it is
/// drawn over) if its own is nearer. Returns whether it stands there.
static s32
CrossboneIsleSteppingStones_SyncPriorityIfBehind(struct Actor *actor,
                                                 struct Actor *front) {
  s32 result = 0;

  if (front->x.fixed == actor->x.fixed && front->y.fixed == actor->y.fixed &&
      front->z.fixed == actor->z.fixed) {
    return result;
  }
  if (front->x.fixed - toFIXED(16) < actor->x.fixed &&
      actor->x.fixed < front->x.fixed + toFIXED(16) &&
      front->y.fixed / toFIXED(1) == actor->y.fixed / toFIXED(1) &&
      front->z.fixed > actor->z.fixed &&
      front->z.fixed - toFIXED(32) < actor->z.fixed) {
    if (actor->sprite->image.priority < front->sprite->image.priority) {
      actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
      actor->sprite->image.priority = front->sprite->image.priority;
      actor->sprite->shadow.priority = front->sprite->shadow.priority;
    }
    result = 1;
  }
  return result;
}

/// The same test from the front: when actor stands just behind front, it takes
/// front's sprite priority if its own is nearer the viewer. Returns whether it
/// stands there.
static s32
CrossboneIsleSteppingStones_SyncPriorityIfAhead(struct Actor *front,
                                                struct Actor *actor) {
  s32 result = 0;

  if (actor->x.fixed == front->x.fixed && actor->y.fixed == front->y.fixed &&
      actor->z.fixed == front->z.fixed) {
    return result;
  }
  if (front->x.fixed - toFIXED(16) < actor->x.fixed &&
      actor->x.fixed < front->x.fixed + toFIXED(16) &&
      actor->y.fixed / toFIXED(1) == front->y.fixed / toFIXED(1) &&
      front->z.fixed > actor->z.fixed &&
      front->z.fixed - toFIXED(32) < actor->z.fixed) {
    if (front->sprite->image.priority > actor->sprite->image.priority) {
      actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
      actor->sprite->image.priority = front->sprite->image.priority;
      actor->sprite->shadow.priority = front->sprite->shadow.priority;
    }
    result = 1;
  }
  return result;
}

/// A stacked pillar's update routine, each frame. While Psynergy is cast it is
/// solid and drawn on Isaac's layers. Otherwise a pillar just behind Isaac or
/// another pillar is drawn no nearer than it, and then the pillars just behind
/// it no nearer than it in turn; a pillar whose top is below Isaac's feet is
/// walked over, drawn under him and not solid. A pillar none of this touches
/// goes back to the priority its place gives it.
// XXX: what the player would see without it; a recording of Isaac walking
// round the stacked pillars, with this routine and with it returning at once,
// would settle it.
static UNREAD_RESULT
CrossboneIsleSteppingStones_UpdateActorPriority(struct Actor *actor) {
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);
  s32 hit;
  u32 id;

  if (gFieldState->castStarted == 1) {
    actor->sprite->image.priority = leader->sprite->image.priority;
    actor->collisionFlags |= ACTOR_SOLID;
    RETURN_UNREAD(0);
  }
  hit = CrossboneIsleSteppingStones_SyncPriorityIfBehind(actor, leader);
  for (id = ACTOR_FIRST_PILLAR; id <= ACTOR_FIRST_PILLAR + 3; id++) {
    hit +=
        CrossboneIsleSteppingStones_SyncPriorityIfBehind(actor, Actor_Get(id));
  }
  if (hit != 0) {
    for (id = ACTOR_FIRST_PILLAR; id <= ACTOR_FIRST_PILLAR + 3; id++) {
      CrossboneIsleSteppingStones_SyncPriorityIfAhead(actor, Actor_Get(id));
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
  RETURN_UNREAD(0);
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
  s32 savedPlacement[3];
#endif

  for (i = 0; i <= 2; i++) {
    firstActor = Actor_Get(i + ACTOR_FIRST_PILLAR);
    for (j = i; j <= 3; j++) {
      secondActor = Actor_Get(j + ACTOR_FIRST_PILLAR);
      if (firstActor->y.fixed <= secondActor->y.fixed &&
          firstActor->z.fixed < secondActor->z.fixed) {
        continue;
      }
      Dma_Set(secondActor, &savedActor,
              DMA_COPY_WORDS(sizeof(savedActor) / sizeof(u32)), REG_DMA3);
      Dma_Wait(REG_DMA3);
      Dma_Set(firstActor, secondActor,
              DMA_COPY_WORDS(sizeof(savedActor) / sizeof(u32)), REG_DMA3);
      Dma_Wait(REG_DMA3);
      Dma_Set(&savedActor, firstActor,
              DMA_COPY_WORDS(sizeof(savedActor) / sizeof(u32)), REG_DMA3);
      Dma_Wait(REG_DMA3);
      Dma_Set(&sPillarSlots[j], &savedPlacement,
              DMA_COPY_WORDS(sizeof(struct PillarPlacement) / sizeof(u32)),
              REG_DMA3);
      Dma_Wait(REG_DMA3);
      Dma_Set(&sPillarSlots[i], &sPillarSlots[j],
              DMA_COPY_WORDS(sizeof(struct PillarPlacement) / sizeof(u32)),
              REG_DMA3);
      Dma_Wait(REG_DMA3);
      Dma_Set(&savedPlacement, &sPillarSlots[i],
              DMA_COPY_WORDS(sizeof(struct PillarPlacement) / sizeof(u32)),
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

/// Isaac casts Move or Carry on one of the square pillars of the seventh
/// floor's side room, and after each push there: each pillar settles where it
/// now stands, on the floor, on a ledge or on another pillar, or drops off a
/// cliff edge and is gone.
///
/// The cells under each pillar are saved and redrawn as it moves, so the
/// pillar tops can be walked and jumped on.
static void CrossboneIsleSteppingStones_MovePillar(void) {
  u32 id;
  struct Actor *actor;
  u32 i;
  u32 k;
  struct Actor *other;
  union CrossboneIslePillarCell cell;

  for (id = ACTOR_FIRST_PILLAR; id <= ACTOR_FIRST_PILLAR + 3; id++) {
    actor = Actor_Get(id);
    actor->layer = 2;
    i = id - ACTOR_FIRST_PILLAR;
    if (FIELD_CELL(actor->x.fixed) == sPillarSlots[i].x &&
        (FIELD_CELL(actor->z.fixed)) == sPillarSlots[i].z &&
        actor->velocityY == 0) {
      continue;
    }
    Dma_Set(&actor->x, &sProbePosition,
            DMA_COPY_WORDS(sizeof(sProbePosition) / sizeof(u32)), REG_DMA3);
    Dma_Wait(REG_DMA3);
    if (FieldActor_CheckMovementCollision(actor, &sProbePosition) == -1) {
      actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    }
    CrossboneIsleSteppingStones_SetCellAttributes(
        0, sPillarSlots[i].x, sPillarSlots[i].z, &sPillarSlots[i].cell);
    CrossboneIsleSteppingStones_SetCellAttributes(
        2, sPillarSlots[i].x, sPillarSlots[i].z, &sPillarSlots[i].cell);
    // Falling pillars either reach a ledge, shrink away, or settle on ground.
    if (actor->motionFlags & ACTOR_FOLLOWS_TERRAIN) {
      if (MapGrid_GetCellBehaviorAt(2, actor->x.fixed, actor->z.fixed) ==
          CELL_BEHAVIOR_STACKED_PILLARS_DROP) {
        Sound_Play(SE_STONE_GRIND);
        actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
        CrossboneIsleSteppingStones_LowerActorToLedge(id, 1);
        actor->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
      } else if (MapGrid_GetCellBehaviorAt(2, actor->x.fixed, actor->z.fixed) ==
                 CELL_BEHAVIOR_STACKED_PILLARS_SINK) {
        CrossboneIsleSteppingStones_StopAtGroundOrHeight(actor, 0);
        Sound_Play(SE_STONE_GRIND);
        actor->y.fixed = 0;
        actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
        CrossboneIsleSteppingStones_SinkPillar(id);
        actor->x.fixed = 0;
        actor->y.fixed = 0;
        actor->z.fixed = 0;
        actor->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
      } else {
        CrossboneIsleSteppingStones_SettleActorOnGround(actor);
      }
      actor->motionFlags = 0;
    }
    CrossboneIsleSteppingStones_ReadMapGridCell(0, FIELD_CELL(actor->x.fixed),
                                                FIELD_CELL(actor->z.fixed),
                                                &sPillarSlots[i].cell);
    if (actor->y.fixed >= 0) {
      CrossboneIsleSteppingStones_ReadMapGridCell(
          0, 27, FIELD_CELL(actor->y.fixed) + 6, &cell);
      CrossboneIsleSteppingStones_SetCellAttributes(
          0, FIELD_CELL(actor->x.fixed), FIELD_CELL(actor->z.fixed), &cell);
      cell.bits.highFlags = sPillarSlots[i].cell.bits.highFlags;
      CrossboneIsleSteppingStones_SetCellAttributes(
          2, FIELD_CELL(actor->x.fixed), FIELD_CELL(actor->z.fixed), &cell);
    }
    sPillarSlots[i].x = FIELD_CELL(actor->x.fixed);
    sPillarSlots[i].y = FIELD_CELL(actor->y.fixed);
    sPillarSlots[i].z = FIELD_CELL(actor->z.fixed);
    for (k = 0; k <= 3; k++) {
      if (k == i) {
        continue;
      }
      GameFlag_Clear(sPillarSlots[k].flag);
      other = Actor_Get(k + ACTOR_FIRST_PILLAR);
      if (FIELD_CELL(actor->x.fixed) == FIELD_CELL(other->x.fixed) &&
          (FIELD_CELL(actor->z.fixed)) == FIELD_CELL(other->z.fixed) &&
          actor->y.fixed > other->y.fixed) {
        GameFlag_Set(sPillarSlots[k].flag);
      }
    }
  }
  CrossboneIsleSteppingStones_SortPillarActors();
}

/// Isaac walks into a square pillar in the seventh floor's side room: he jumps
/// across it if there is somewhere to land, and otherwise pushes it, and the
/// pillars settle.
static void CrossboneIsleSteppingStones_JumpOrPushStackedPillar(void) {
  Event_Start();
#if TBS_PATCH_3
  Actor_Get(ACTOR_PARTY_LEADER)->y.fixed =
      MapGrid_GetTerrainHeight(Actor_Get(ACTOR_PARTY_LEADER)->layer,
                               Actor_Get(ACTOR_PARTY_LEADER)->x.fixed,
                               Actor_Get(ACTOR_PARTY_LEADER)->z.fixed);
#endif
  if (CrossboneIsleSteppingStones_TryJumpAcrossPillar() == 0) {
    Actor_Get(ACTOR_PARTY_LEADER)->motionFlags &= ~ACTOR_FOLLOWS_TERRAIN;
    Actor_Get(ACTOR_PARTY_LEADER)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
    PushedActor_AdvancePair();
    CrossboneIsleSteppingStones_MovePillar();
    Actor_Get(ACTOR_PARTY_LEADER)->motionFlags |= ACTOR_FOLLOWS_TERRAIN;
    Actor_Get(ACTOR_PARTY_LEADER)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  }
  Event_End();
}

/// Isaac walks north into a ladder in the seventh floor's side room and climbs
/// up it.
static void CrossboneIsleSteppingStones_ClimbUp(void) { Field_ClimbUp(); }

/// Isaac walks south at the top of a ladder in the seventh floor's side room:
/// he pushes a pillar standing below him, jumps across one, or climbs down.
static void CrossboneIsleSteppingStones_PushOrClimbDown(void) {
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);
  struct FieldPosition position;

  position.x = leader->x.fixed;
  position.y = leader->y.fixed;
  position.z = leader->z.fixed + toFIXED(16);
  if (PushedActor_FindAtTile(&position, leader) != NULL) {
    CrossboneIsleSteppingStones_JumpOrPushStackedPillar();
  } else if (CrossboneIsleSteppingStones_TryJumpAcrossPillar() == 0) {
    Field_ClimbDown();
  }
}

/// Nothing visible happens when Isaac walks onto the lowest floor of the
/// seventh floor's side room, among the brown pits, for example cell (54, 15)
/// in its north east. The game starts the ground mask on him and sets his
/// `sceneCounter`. From then on, walking on the brown floor along the raised
/// pink ground south of it, he shows only from the waist up: the ground mask
/// covers the rest in the colour of the raised ground, as if he stood behind
/// it.
static void CrossboneIsleSteppingStones_StartRising(void) {
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);
  gGroundMaskState->actor = leader;
  leader->sceneCounter = 1;
}

/// Nothing visible happens when Isaac leaves the lowest floor of the seventh
/// floor's side room at cell (33, 27), below the west ladders, or cell
/// (54, 14) in the north east. The ground mask lets him go and his
/// `sceneCounter` is cleared.
static void CrossboneIsleSteppingStones_StopRising(void) {
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);

  gGroundMaskState->actor = NULL;
  leader->sceneCounter = 0;
}

/// Puts the frozen puddle under the leader when it is behind and below the
/// leader.
static UNREAD_RESULT
CrossboneIsleSteppingStones_UpdateFrozenPuddlePriority(struct Actor *actor) {
  struct Actor *ref = Actor_Get(ACTOR_PARTY_LEADER);

  if (actor->z.fixed > ref->z.fixed) {
    actor->priorityFlags &= ~ACTOR_PRIORITY_UNDERFOOT;
    if (actor->y.fixed < ref->y.fixed)
      actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  }

  RETURN_UNREAD(0);
}

/// A puddle in the dragon statue's room has frozen into a pillar of ice: it
/// shines, and its cell becomes a pillar top Isaac can jump onto
/// (gCrossboneIsleSteppingStones_IceShineScript).
static void CrossboneIsleSteppingStones_FreezePuddle(s32 id) {
  struct Actor *actor;

  actor = Actor_Get(id);
  Event_Start();
#ifdef UBFIX
  actor->update = CrossboneIsleSteppingStones_UpdateFrozenPuddlePriority;
#else
  actor->update = (void (*)(
      struct Actor *))CrossboneIsleSteppingStones_UpdateFrozenPuddlePriority;
#endif
  MapGrid_CopyCellAttributes(20, 14, 1, 1, FIELD_CELL(actor->x.fixed),
                             FIELD_CELL(actor->z.fixed));
  GameFlag_Set(FLAG_CROSSBONE_ISLE_DRAGON_STATUE_FIRST_PUDDLE_FROZEN + id -
               ACTOR_FIRST_PUDDLE);
  Actor_SetScript(id, gCrossboneIsleSteppingStones_IceShineScript);
  Event_End();
}

/// Isaac casts Frost on the puddle by the west ladder of the dragon statue's
/// room, cell (8, 13): it freezes into a pillar of ice.
static void CrossboneIsleSteppingStones_FreezeFirstPuddle(void) {
  CrossboneIsleSteppingStones_FreezePuddle(ACTOR_FIRST_PUDDLE);
}

/// Isaac casts Frost on the puddle by the east ladder of the dragon statue's
/// room, cell (18, 13): it freezes into a pillar of ice, until the dragon's
/// fire melts it.
static void CrossboneIsleSteppingStones_FreezeSecondPuddle(void) {
  CrossboneIsleSteppingStones_FreezePuddle(ACTOR_SECOND_PUDDLE);
}

/// Isaac starts casting Frost on the puddle under the white platform in front
/// of the stone dragon. Nothing visible happens yet; the game notes whether he
/// is standing in the puddle, which then stays unfrozen.
static void CrossboneIsleSteppingStones_FrostStartsOnThirdPuddle(void) {
  struct Actor *leader;
  struct Actor *puddle;
  struct FieldEffectState *effectState;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  puddle = Actor_Get(ACTOR_THIRD_PUDDLE);
  effectState = gFieldEffectState;
  if (FIELD_CELL(puddle->x.fixed) == FIELD_CELL(leader->x.fixed) &&
      FIELD_CELL(puddle->z.fixed) == FIELD_CELL(leader->z.fixed)) {
    GameFlag_Set(FLAG_CROSSBONE_ISLE_DRAGON_STATUE_LEADER_ON_THIRD_PUDDLE);
    effectState->leaveTarget = 1;
  } else {
    GameFlag_Clear(FLAG_CROSSBONE_ISLE_DRAGON_STATUE_LEADER_ON_THIRD_PUDDLE);
  }
}

/// As Frost takes hold of the puddle in front of the stone dragon, a pillar of
/// ice grows in it and lifts the white platform two steps up.
static void CrossboneIsleSteppingStones_FrostRaisesPillar(void) {
  struct Actor *pillar;

  pillar = Actor_Get(ACTOR_RISING_PILLAR);
  Actor_Get(ACTOR_THIRD_PUDDLE)->scaleX = FIXED_ONE;
  Actor_Get(ACTOR_THIRD_PUDDLE)->scaleY = FIXED_ONE;
  (Actor_Get(ACTOR_THIRD_PUDDLE)->sprite)->image.priority = 2;
  pillar->sprite->image.priority = 2;
  pillar->acceleration = toFIXED(2) / 5;
  pillar->speedLimit = toFIXED(4) / 5;
  FieldActor_SetMoveTargetFX(pillar, pillar->x.fixed, toFIXED(32),
                             pillar->z.fixed);
  Actor_WaitForMoveById(ACTOR_RISING_PILLAR);
  MapGrid_CopyCellAttributes(20, 14, 1, 1, 22, 16);
}

/// The stone dragon's fire, each frame while it breathes: flames burst from
/// beside it towards the puddle by the east ladder, with a whoosh.
static void CrossboneIsleSteppingStones_SpawnDragonFire(void) {
  struct EffectOptions options;
  s32 velocityX;
  FIXED velocityY;
  u32 angle;

  if ((gFrameCount & 2) != 0)
    return;
  if ((gFrameCount & 7) == 0)
    Sound_Play(SE_WHOOSH);

  options.palette = SPRITE_COLOR_RED_TINT;
  options.startScaleX = FIXED_ONE / 2;
  options.startScaleY = FIXED_ONE / 2;
  options.targetScaleX = 8 * FIXED_ONE / 5;
  options.targetScaleY = 8 * FIXED_ONE / 5;
  // A random angle, of which options.rotation keeps the top four bits: a
  // random sixteenth of a turn.
  angle = Random_Next() & 0x0ffff000;
  options.rotation = angle;
  options.update = CrossboneIsleSteppingStones_UpdateDampedParticle;

  velocityX = -(INTtoFIXED(Random_Below(5)) + toFIXED(6));
  velocityX /= 2;
  velocityY = -(INTtoFIXED(Random_Below(5)) + toFIXED(5));

  FieldEffect_Spawn(
      toFIXED(324), toFIXED(48), toFIXED(228), velocityX, velocityY, 0,
      EFFECT_USE_UPDATE | EFFECT_USE_ROTATION | EFFECT_USE_START_SCALE |
          EFFECT_SCALE_TO_TARGET | EFFECT_USE_PALETTE,
      &options);
}

/// Steam rises from the melting ice by the east ladder: a white puff every
/// eight frames.
static UNREAD_RESULT
CrossboneIsleSteppingStones_SpawnMeltParticle(struct Actor *actor) {
  struct EffectOptions options;
  u32 phase = gFrameCount & 7;
  FIXED x;
  FIXED y;
  FIXED z;
  s32 velocityY;

  if (phase != 0)
    RETURN_UNREAD(0);

  options.palette = SPRITE_COLOR_WHITE;
  options.startScaleX = 7 * FIXED_ONE / 10;
  options.startScaleY = 7 * FIXED_ONE / 10;

  x = actor->x.fixed + INTtoFIXED(Random_Below(17) - 8);
  y = actor->y.fixed + INTtoFIXED(Random_Below(17));
  z = actor->z.fixed + INTtoFIXED(Random_Below(17) - 8);
  velocityY = Math_Divide(INTtoFIXED(Random_Below(5)) + toFIXED(3), 10);

  FieldEffect_Spawn(x, y, z, 0, velocityY, phase,
                    EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE |
                        EFFECT_SCRIPT_MEDIUM,
                    &options);
  RETURN_UNREAD(0);
}

/// Isaac's Frost freezes the puddle in front of the stone dragon, unless he
/// stood in it. The view moves up to the dragon, which breathes fire at the
/// puddle by the east ladder; if that puddle is frozen, its ice melts away in
/// steam, so it can be frozen again.
static void CrossboneIsleSteppingStones_FreezeThirdPuddle(void) {
  s32 set;

  set = GameFlag_Get(FLAG_CROSSBONE_ISLE_DRAGON_STATUE_LEADER_ON_THIRD_PUDDLE);
  if (set == 0) {
    GameFlag_Set(FLAG_CROSSBONE_ISLE_DRAGON_STATUE_FIRST_PUDDLE_FROZEN + 2);
    Event_Start();
    FieldCamera_SetSpeed(0.6);
    FieldCamera_MoveTo(312, CAMERA_KEEP, 184, 1);
    FieldCamera_WaitForMove();
    Event_Wait(20);
    MapGrid_CopyCellsTo(73, 10, 60, 10, 1, 2);
    Event_Wait(20);
    // The third puddle freezes while the frozen second puddle melts.
    Task_AddFrame(CrossboneIsleSteppingStones_SpawnDragonFire, TASK_PASS_FRAME);
    Event_Wait(40);
    if (Event_GetFlag(FLAG_CROSSBONE_ISLE_DRAGON_STATUE_FIRST_PUDDLE_FROZEN +
                      1) != 0) {
#ifdef UBFIX
      Actor_Get(ACTOR_SECOND_PUDDLE)->update =
          CrossboneIsleSteppingStones_SpawnMeltParticle;
#else
      Actor_Get(ACTOR_SECOND_PUDDLE)->update = (void (*)(
          struct Actor *))CrossboneIsleSteppingStones_SpawnMeltParticle;
#endif
      Actor_SetAnimation(ACTOR_SECOND_PUDDLE, PUDDLE_ANIMATION_MELT);
      Actor_WaitForAnimationChange(ACTOR_SECOND_PUDDLE);
      Actor_Get(ACTOR_SECOND_PUDDLE)->update = (void (*)(struct Actor *))set;
      MapGrid_CopyCellAttributes(17, 13, 1, 1, 18, 13);
      GameFlag_Clear(FLAG_CROSSBONE_ISLE_DRAGON_STATUE_FIRST_PUDDLE_FROZEN + 1);
      Actor_SetSpriteColorById(ACTOR_SECOND_PUDDLE, SPRITE_COLOR_NORMAL);
      Actor_SetMovementTypeById(ACTOR_SECOND_PUDDLE, MOVEMENT_TYPE_STAND);
    } else {
      Event_Wait(60);
    }
    Task_Remove(CrossboneIsleSteppingStones_SpawnDragonFire);
    Event_Wait(20);
    MapGrid_CopyCellsTo(72, 10, 60, 10, 1, 2);
    Event_Wait(20);
    Event_End();
  }
}

/// Nothing visible happens as Isaac starts casting Move on the small boulder
/// below the logs in the dragon statue's room; the game clears the boulder's
/// cell so that it can move.
static void CrossboneIsleSteppingStones_StartMovingSinkingBoulder(void) {
  struct Actor *boulder = Actor_Get(ACTOR_SINKING_BOULDER);
  s32 x;
  s32 z;

  Event_Start();

  x = FIELD_CELL(boulder->x.fixed);
  z = FIELD_CELL(boulder->z.fixed);

  MapGrid_FillCollisionRectangle(2, x, z, 1, 1, CELL_BEHAVIOR_NORMAL);
  Event_End();
}

/// Isaac moves the small boulder below the logs in the dragon statue's room a
/// step, with Move or by walking into it. Two steps east of where it starts,
/// in column SINKING_BOULDER_COLUMN, it settles into the floor with a thud and
/// stays there for good.
static void CrossboneIsleSteppingStones_MoveSinkingBoulder(void) {
  struct Actor *boulder;
  s32 alreadyLowered;

  boulder = Actor_Get(ACTOR_SINKING_BOULDER);
  Event_Start();
  MapGrid_FillCollisionRectangle(2, FIELD_CELL(boulder->x.fixed),
                                 (FIELD_CELL(boulder->z.fixed)), 1, 1,
                                 CELL_BEHAVIOR_WALL);
  if (FIELD_CELL(boulder->x.fixed) == SINKING_BOULDER_COLUMN) {
    alreadyLowered =
        Event_GetFlag(FLAG_CROSSBONE_ISLE_DRAGON_STATUE_BOULDER_SUNK);
    if (alreadyLowered == 0) {
      Event_Wait(10);
      Sound_Play(SE_DOOR_CLOSE);
      boulder->motionFlags = alreadyLowered;
      boulder->groundHeight = -toFIXED(2);
      boulder->y.fixed = -toFIXED(2);
      GameFlag_Set(FLAG_CROSSBONE_ISLE_DRAGON_STATUE_BOULDER_SUNK);
    }
  }
  Event_End();
}

/// Isaac walks into the small boulder below the logs in the dragon statue's
/// room and pushes it a step if the way beyond is clear.
static void CrossboneIsleSteppingStones_PushSinkingBoulder(void) {
  struct PushedActorProbe probe;

  Event_Start();
  if (PushedActor_FindClearPosition(&probe) != 0) {
    PushedActor_Roll(probe);
  } else {
    CrossboneIsleSteppingStones_StartMovingSinkingBoulder();
    PushedActor_AdvancePair();
    CrossboneIsleSteppingStones_MoveSinkingBoulder();
  }
  Event_End();
}

const struct Trigger *Scene_GetTriggers(void) {
  s16 sceneId = gMainRoutineState.next;

  if (sceneId == SCENE_CROSSBONE_ISLE_STEPPING_STONES) {
    return gCrossboneIsleSteppingStonesTriggers;
  }
  if (sceneId == SCENE_CROSSBONE_ISLE_ICE_PILLARS) {
    return gCrossboneIsleIcePillarsTriggers;
  }
  if (sceneId == SCENE_CROSSBONE_ISLE_FLOODGATE) {
    return gCrossboneIsleFloodgateTriggers;
  }
  if (sceneId == SCENE_CROSSBONE_ISLE_STACKED_PILLARS) {
    return gCrossboneIsleStackedPillarsTriggers;
  }
  if (sceneId == SCENE_CROSSBONE_ISLE_DRAGON_STATUE) {
    return gCrossboneIsleDragonStatueTriggers;
  }
  return gCrossboneIsleSteppingStonesTriggersOther;
}

/// Lays an actor in the floor below the leader: not solid, drawn under his
/// feet at priority 2, and without a shadow.
static void CrossboneIsleSteppingStones_SetBelowLeader(s32 id) {
  struct Actor *actor = Actor_Get(id);

  actor->collisionFlags &= ~ACTOR_SOLID;
  actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  actor->motionFlags = 0;
  Actor_SetSpriteFlags(actor, 0);
  actor->sprite->image.priority = 2;
}

/// A puddle Frost has frozen stands as its pillar of ice again, with its cell,
/// its priority routine and its shine.
static void CrossboneIsleSteppingStones_RestoreFrozenPuddle(s32 actorId) {
  struct Actor *actor;

  actor = Actor_Get(actorId);
  if (GameFlag_Get(FLAG_CROSSBONE_ISLE_DRAGON_STATUE_FIRST_PUDDLE_FROZEN +
                   actorId - ACTOR_FIRST_PUDDLE) != 0) {
    Actor_SelectAnimation(actor, PUDDLE_ANIMATION_PILLAR);
#ifdef UBFIX
    actor->update = CrossboneIsleSteppingStones_UpdateFrozenPuddlePriority;
#else
    actor->update = (void (*)(
        struct Actor *))CrossboneIsleSteppingStones_UpdateFrozenPuddlePriority;
#endif
    MapGrid_CopyCellAttributes(20, 14, 1, 1, FIELD_CELL(actor->x.fixed),
                               FIELD_CELL(actor->z.fixed));
    Actor_SetScript(actorId, gCrossboneIsleSteppingStones_IceShineScript);
  }
}

/// Uses layer 2 and installs the moving pillar's priority update routine.
static void CrossboneIsleSteppingStones_InitializePillarPriority(s32 id) {
  struct Actor *actor = Actor_Get(id);

  actor->layer = 2;
  actor->motionFlags = 0;
  actor->update =
      (void (*)(struct Actor *))CrossboneIsleSteppingStones_UpdateActorPriority;
}

/// Matches the leader's draw priority to a nearby pillar in front.
static void CrossboneIsleSteppingStones_UpdateLeaderPriority(void) {
  struct Actor *hero = Actor_Get(ACTOR_PARTY_LEADER);
  u32 selector;
  struct Actor *actor;

  for (selector = ACTOR_FIRST_PILLAR; selector <= ACTOR_FIRST_PILLAR + 3;
       selector++) {
    actor = Actor_Get(selector);
    if (hero->y.fixed / toFIXED(1) == actor->y.fixed / toFIXED(1) &&
        hero->z.fixed <= actor->z.fixed - toFIXED(8) &&
        hero->z.fixed > actor->z.fixed - toFIXED(24)) {
      if (hero->x.fixed - toFIXED(16) <= actor->x.fixed &&
          actor->x.fixed < hero->x.fixed + toFIXED(16)) {
        Actor_SetSpritePriority(ACTOR_PARTY_LEADER,
                                actor->sprite->image.priority);
        return;
      }
    } else {
      Actor_Get(ACTOR_PARTY_LEADER)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
    }
  }
}

/// Sets up the room: the cells under its pillars and logs, and the
/// floodgate's water and the puzzles as a battle left them.
s32 Scene_Initialize(void) {
  struct Actor *actor;
  u32 i;
  u32 id;
  s32 x;
  s32 z;

  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  if (gMainRoutineState.next == SCENE_CROSSBONE_ISLE_ICE_PILLARS) {
    PushedActor_BlockFootprint(ACTOR_FIRST_PILLAR);
    PushedActor_BlockFootprint(ACTOR_FIRST_PILLAR + 1);
    PushedActor_BlockFootprint(ACTOR_FIRST_PILLAR + 2);
    PushedActor_BlockFootprint(ACTOR_FIRST_PILLAR + 3);
    PushedActor_BlockFootprint(ACTOR_FIRST_PILLAR + 4);
  } else if (gMainRoutineState.next == SCENE_CROSSBONE_ISLE_FLOODGATE) {
    MapGrid_CopyCells(32, 0, 64, 32, 0, 64);
    PushedActor_BlockFootprint(ACTOR_FIRST_LOG);
    PushedActor_BlockFootprint(ACTOR_FIRST_LOG + 1);
    PushedActor_BlockFootprint(ACTOR_FIRST_LOG + 2);
    PushedActor_BlockFootprint(ACTOR_FIRST_LOG + 3);
    PushedActor_BlockFootprint(ACTOR_FIRST_LOG + 4);
    PushedActor_BlockFootprint(ACTOR_FIRST_LOG + 5);
    PushedActor_BlockFootprint(ACTOR_FIRST_LOG + 6);
    PushedActor_BlockFootprint(ACTOR_FIRST_LOG + 7);
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) != 0 &&
        GameFlag_Get(FLAG_CROSSBONE_ISLE_FLOODGATE_WATER_RAISED) != 0) {
      MapGrid_CopyCellsTo(79, 34, 84, 24, 1, 2);
      MapGrid_CopyCellsTo(0, RAISED_WATER_ROW, FLOODGATE_LAYER_1_COLUMN, 0, 32,
                          32);
      MapGrid_CopyCellsTo(32, RAISED_WATER_ROW, FLOODGATE_LAYER_2_COLUMN, 0, 32,
                          32);
      CrossboneIsleSteppingStones_FloatLogs();
      MapGrid_CopyCellAttributes(24, 3, 1, 1, 24, 8);
    }
  } else if (gMainRoutineState.next == SCENE_CROSSBONE_ISLE_STACKED_PILLARS) {
    Actor_CreateConfigured(584, 0, 200, SPRITE_CRATE);
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
      Actor_Get(ACTOR_PARTY_LEADER)->sceneCounter = 1;
    }
    FieldEffect_StartGroundMask(0);
    if (Actor_Get(ACTOR_PARTY_LEADER)->sceneCounter == 0) {
      CrossboneIsleSteppingStones_StopRising();
    }
    CrossboneIsleSteppingStones_InitializePillarPriority(ACTOR_FIRST_PILLAR);
    CrossboneIsleSteppingStones_InitializePillarPriority(ACTOR_FIRST_PILLAR +
                                                         1);
    CrossboneIsleSteppingStones_InitializePillarPriority(ACTOR_FIRST_PILLAR +
                                                         2);
    CrossboneIsleSteppingStones_InitializePillarPriority(ACTOR_FIRST_PILLAR +
                                                         3);
    for (i = 0; i < 4; i++) {
      sPillarSlots[i].x = 0;
      sPillarSlots[i].y = 0;
      sPillarSlots[i].z = 0;
      sPillarSlots[i].flag =
          i + FLAG_CROSSBONE_ISLE_STACKED_PILLARS_FIRST_PILLAR_UNDER_ANOTHER;
    }
    CrossboneIsleSteppingStones_MovePillar();
    Event_Wait(1);
    Task_AddFrame(CrossboneIsleSteppingStones_UpdateLeaderPriority,
                  TASK_PASS_FRAME);
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) != 0) {
      for (id = ACTOR_FIRST_PILLAR; id < ACTOR_FIRST_PILLAR + 4; id++) {
        actor = Actor_Get(id);
        x = FIELD_CELL(actor->x.fixed);
        if (x == 37) {
          z = FIELD_CELL(actor->z.fixed);
          if (z == 9) {
            MapGrid_CopyCellAttributes(27, 8, 1, 1, x, z);
            break;
          }
        }
      }
    }
  } else if (gMainRoutineState.next == SCENE_CROSSBONE_ISLE_DRAGON_STATUE) {
    Actor_SetAnimation(ACTOR_SINKING_BOULDER, TURNING_ROCK_ANIMATION_BOULDER);
    Actor_SetSpriteColorById(ACTOR_SINKING_BOULDER, SPRITE_COLOR_GREY);
    PushedActor_BlockFootprint(ACTOR_FIRST_LOG);
    PushedActor_BlockFootprint(ACTOR_FIRST_LOG + 1);
#if TBS_PATCH_1
    Actor_Get(ACTOR_FIRST_LOG)->motionFlags = 0;
    Actor_Get(ACTOR_FIRST_LOG + 1)->motionFlags = 0;
#endif
    CrossboneIsleSteppingStones_MoveSinkingBoulder();
    CrossboneIsleSteppingStones_SetBelowLeader(ACTOR_FIRST_PUDDLE);
    CrossboneIsleSteppingStones_SetBelowLeader(ACTOR_SECOND_PUDDLE);
    CrossboneIsleSteppingStones_SetBelowLeader(ACTOR_THIRD_PUDDLE);
    CrossboneIsleSteppingStones_RestoreFrozenPuddle(ACTOR_FIRST_PUDDLE);
    CrossboneIsleSteppingStones_RestoreFrozenPuddle(ACTOR_SECOND_PUDDLE);
    CrossboneIsleSteppingStones_RestoreFrozenPuddle(ACTOR_THIRD_PUDDLE);
    Actor_Get(ACTOR_THIRD_PUDDLE)->update = 0;
    CrossboneIsleSteppingStones_SetBelowLeader(ACTOR_RISING_PILLAR);
    Actor_Get(ACTOR_RISING_PILLAR)->collisionFlags |= ACTOR_NOT_TARGETED;
    if (GameFlag_Get(FLAG_CROSSBONE_ISLE_DRAGON_STATUE_FIRST_PUDDLE_FROZEN +
                     2) == 0) {
      Actor_Get(ACTOR_THIRD_PUDDLE)->scaleX = 3 * FIXED_ONE / 2;
      Actor_Get(ACTOR_THIRD_PUDDLE)->scaleY = 3 * FIXED_ONE / 2;
      Actor_Get(ACTOR_THIRD_PUDDLE)->sprite->image.priority = 3;
      Actor_Get(ACTOR_RISING_PILLAR)->sprite->image.priority = 3;
      MapGrid_CopyCellAttributes(26, 12, 1, 1, 22, 16);
    }
  }
  return 0;
}

#include "shared/field_effect_duration_scripts.c"

#include "shared/pushed_actor_tables.c"
// clang-format off

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

/// The shine on a pillar of ice Frost has made of a puddle in the dragon
/// statue's room.
///
/// The timer carries a colour mode to
/// CrossboneIsleSteppingStones_SetSpriteColorFromTimer, with green and blue
/// swapped for two frames in every eight.
s32 gCrossboneIsleSteppingStones_IceShineScript[] = {
    SC_SET(SW_TIMER, SPRITE_COLOR_RBG),
    SC_CALL(CrossboneIsleSteppingStones_SetSpriteColorFromTimer),
    SC_WAIT(2),
    SC_SET(SW_TIMER, SPRITE_COLOR_NORMAL),
    SC_CALL(CrossboneIsleSteppingStones_SetSpriteColorFromTimer),
    SC_WAIT(6),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/// The floodgate's switch flipping as
/// CrossboneIsleSteppingStones_FlipWaterSwitch closes the gate, and flipping
/// back as it opens it.
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

/// The two cells at the floodgate's foot as it shuts.
///
/// They show three pictures of white spray splashing up and falling, then the
/// stream bed below the gate, left almost dry
/// (CrossboneIsleSteppingStones_FlipWaterSwitch's render).
u16 gCrossboneIsleSteppingStones_FloodgateFootSteps[] = {
    MAPGRID_ANIMATION_STEP(75, 38, 2, 1, 4), MAPGRID_ANIMATION_STEP(77, 38, 2, 1, 6), MAPGRID_ANIMATION_STEP(79, 38, 2, 1, 8), MAPGRID_ANIMATION_STEP(65, 53, 2, 1, 1), MAPGRID_ANIMATION_STEPS_END,
};

#include "shared/fallback_entrances.c"

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

/// Unused: an empty actor placement table nothing returns.
struct ActorPlacement gCrossboneIsleSteppingStonesNoActorPlacements[] = {
    { SCENE_TABLE_END },
};

struct ActorPlacement gCrossboneIsleSteppingStonesActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        744, 0, 488, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        792, 0, 552, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        696, 0, 616, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        776, 0, 680, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gCrossboneIsleIcePillarsActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        144, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        256, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        224, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        280, 0, 160, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        184, 0, 240, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gCrossboneIsleFloodgateActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        144, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        232, 0, 256, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        208, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        232, 0, 176, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        272, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        352, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPLASH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gCrossboneIsleSteppingStonesActorPlacementsOther[] = {
    ACTOR_PLACEMENT(SPRITE_SPIRAL_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        648, 0, 184, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPIRAL_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPIRAL_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        680, 0, 392, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPIRAL_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gCrossboneIsleDragonStatueActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        208, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        224, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TURNING_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        232, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        136, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        296, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        360, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        360, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

struct Trigger gCrossboneIsleSteppingStonesTriggersOther[] = {
    { TRIGGER_TABLE_END },
};

struct Trigger gCrossboneIsleSteppingStonesTriggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, CROSSBONE_ISLE_STEPPING_STONES_TO_CROSSBONE_ISLE_FIRST_FLOOR_EAST },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, CROSSBONE_ISLE_STEPPING_STONES_TO_CROSSBONE_ISLE_FIRST_FLOOR_WEST },
    { TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, CELL_BEHAVIOR_BESIDE_PUSHED_ACTOR, CONDITION_ALWAYS, EVENT(CrossboneIsleSteppingStones_JumpOrPushPillar) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), TREASURE_SPOT_STEPPING_STONES_ANTIDOTE, FLAG_CROSSBONE_ISLE_STEPPING_STONES_ANTIDOTE, FIND_ITEM(ITEM_ANTIDOTE) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), TREASURE_SPOT_STEPPING_STONES_NUT, FLAG_CROSSBONE_ISLE_STEPPING_STONES_NUT, FIND_ITEM(ITEM_NUT) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), TREASURE_SPOT_STEPPING_STONES_111_COINS, FLAG_CROSSBONE_ISLE_STEPPING_STONES_111_COINS, FIND_COINS(111) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), TREASURE_SPOT_STEPPING_STONES_HARD_NUT, FLAG_CROSSBONE_ISLE_STEPPING_STONES_HARD_NUT, FIND_ITEM(ITEM_HARD_NUT) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_PILLAR, CONDITION_ALWAYS, 0 },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_PILLAR + 1, CONDITION_ALWAYS, 0 },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_PILLAR + 2, CONDITION_ALWAYS, 0 },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_PILLAR + 3, CONDITION_ALWAYS, 0 },
    { TRIGGER_TABLE_END },
};

struct Trigger gCrossboneIsleIcePillarsTriggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, CROSSBONE_ISLE_ICE_PILLARS_TO_CROSSBONE_ISLE_SECOND_FLOOR_EAST },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, CROSSBONE_ISLE_ICE_PILLARS_TO_CROSSBONE_ISLE_SECOND_FLOOR_WEST },
    { TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, CELL_BEHAVIOR_BESIDE_PUSHED_ACTOR, CONDITION_ALWAYS, EVENT(CrossboneIsleSteppingStones_PushPillar) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), TREASURE_SPOT_ICE_PILLARS_LUCKY_PEPPER, FLAG_CROSSBONE_ISLE_ICE_PILLARS_LUCKY_PEPPER, FIND_ITEM(ITEM_LUCKY_PEPPER) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), TREASURE_SPOT_ICE_PILLARS_MYSTERY_BLADE, FLAG_CROSSBONE_ISLE_ICE_PILLARS_MYSTERY_BLADE, FIND_ITEM(ITEM_MYSTERY_BLADE) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), TREASURE_SPOT_ICE_PILLARS_222_COINS, FLAG_CROSSBONE_ISLE_ICE_PILLARS_222_COINS, FIND_COINS(222) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), TREASURE_SPOT_ICE_PILLARS_ELIXIR, FLAG_CROSSBONE_ISLE_ICE_PILLARS_ELIXIR, FIND_ITEM(ITEM_ELIXIR) },
    { TRIGGER_TABLE_END },
};

struct Trigger gCrossboneIsleFloodgateTriggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, CROSSBONE_ISLE_FLOODGATE_TO_CROSSBONE_ISLE_FIFTH_FLOOR_EAST },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, CROSSBONE_ISLE_FLOODGATE_TO_CROSSBONE_ISLE_FIFTH_FLOOR_WEST },
    { TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, CELL_BEHAVIOR_BESIDE_PUSHED_ACTOR, CONDITION_ALWAYS, EVENT(CrossboneIsleSteppingStones_PushPillar) },
    { EXAMINE_CHEST_FACING(FACING_EAST), TREASURE_SPOT_FLOODGATE_555_COINS, FLAG_CROSSBONE_ISLE_FLOODGATE_555_COINS, FIND_COINS(555) },
    { EXAMINE_CHEST_FACING(FACING_NORTH), TREASURE_SPOT_FLOODGATE_555_COINS, FLAG_CROSSBONE_ISLE_FLOODGATE_555_COINS, FIND_COINS(555) },
    { EXAMINE_CHEST_FACING(FACING_EAST), TREASURE_SPOT_FLOODGATE_LUCKY_MEDAL, FLAG_CROSSBONE_ISLE_FLOODGATE_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { EXAMINE_CHEST_FACING(FACING_NORTHEAST), TREASURE_SPOT_FLOODGATE_LUCKY_MEDAL, FLAG_CROSSBONE_ISLE_FLOODGATE_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), TREASURE_SPOT_FLOODGATE_POTION, FLAG_CROSSBONE_ISLE_FLOODGATE_POTION, FIND_ITEM(ITEM_POTION) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), TREASURE_SPOT_FLOODGATE_NINJA_GARB, FLAG_CROSSBONE_ISLE_FLOODGATE_NINJA_GARB, FIND_ITEM(ITEM_NINJA_GARB) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_FLOODGATE_SWITCH, CONDITION_ALWAYS, EVENT(CrossboneIsleSteppingStones_FlipWaterSwitch) },
    { TRIGGER_TABLE_END },
};

struct Trigger gCrossboneIsleStackedPillarsTriggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, CROSSBONE_ISLE_STACKED_PILLARS_TO_CROSSBONE_ISLE_SEVENTH_FLOOR_EAST },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, CROSSBONE_ISLE_STACKED_PILLARS_TO_CROSSBONE_ISLE_SEVENTH_FLOOR_WEST },
    { TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, CELL_BEHAVIOR_STACKED_PILLARS_PILLARS, CONDITION_ALWAYS, EVENT(CrossboneIsleSteppingStones_JumpOrPushStackedPillar) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_STACKED_PILLARS_LADDER, CONDITION_ALWAYS, EVENT(CrossboneIsleSteppingStones_ClimbUp) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_STACKED_PILLARS_LADDER, CONDITION_ALWAYS, EVENT(CrossboneIsleSteppingStones_PushOrClimbDown) },
    { TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, CELL_BEHAVIOR_STACKED_PILLARS_LADDER, CONDITION_ALWAYS, EVENT(CrossboneIsleSteppingStones_JumpOrPushStackedPillar) },
    { TRIGGER_STEP, CELL_BEHAVIOR_STACKED_PILLARS_LOWEST_FLOOR, CONDITION_ALWAYS, EVENT(CrossboneIsleSteppingStones_StartRising) },
    { TRIGGER_STEP, CELL_BEHAVIOR_STACKED_PILLARS_LEAVE_LOWEST_FLOOR, CONDITION_ALWAYS, EVENT(CrossboneIsleSteppingStones_StopRising) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), TREASURE_SPOT_STACKED_PILLARS_777_COINS, FLAG_CROSSBONE_ISLE_STACKED_PILLARS_777_COINS, FIND_COINS(777) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), TREASURE_SPOT_STACKED_PILLARS_LUCKY_MEDAL, FLAG_CROSSBONE_ISLE_STACKED_PILLARS_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { EXAMINE_CHEST_FACING(FACING_EAST), TREASURE_SPOT_STACKED_PILLARS_PSY_CRYSTAL, FLAG_CROSSBONE_ISLE_STACKED_PILLARS_PSY_CRYSTAL, FIND_ITEM(ITEM_PSY_CRYSTAL) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), TREASURE_SPOT_STACKED_PILLARS_WICKED_MACE, FLAG_CROSSBONE_ISLE_STACKED_PILLARS_WICKED_MACE, FIND_ITEM(ITEM_WICKED_MACE) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_PILLAR, FLAG_CROSSBONE_ISLE_STACKED_PILLARS_FIRST_PILLAR_UNDER_ANOTHER, EVENT(CrossboneIsleSteppingStones_MovePillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_PILLAR + 1, FLAG_CROSSBONE_ISLE_STACKED_PILLARS_FIRST_PILLAR_UNDER_ANOTHER + 1, EVENT(CrossboneIsleSteppingStones_MovePillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_PILLAR + 2, FLAG_CROSSBONE_ISLE_STACKED_PILLARS_FIRST_PILLAR_UNDER_ANOTHER + 2, EVENT(CrossboneIsleSteppingStones_MovePillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_PILLAR + 3, FLAG_CROSSBONE_ISLE_STACKED_PILLARS_FIRST_PILLAR_UNDER_ANOTHER + 3, EVENT(CrossboneIsleSteppingStones_MovePillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_FIRST_PILLAR, FLAG_CROSSBONE_ISLE_STACKED_PILLARS_FIRST_PILLAR_UNDER_ANOTHER, EVENT(CrossboneIsleSteppingStones_MovePillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_FIRST_PILLAR + 1, FLAG_CROSSBONE_ISLE_STACKED_PILLARS_FIRST_PILLAR_UNDER_ANOTHER + 1, EVENT(CrossboneIsleSteppingStones_MovePillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_FIRST_PILLAR + 2, FLAG_CROSSBONE_ISLE_STACKED_PILLARS_FIRST_PILLAR_UNDER_ANOTHER + 2, EVENT(CrossboneIsleSteppingStones_MovePillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_FIRST_PILLAR + 3, FLAG_CROSSBONE_ISLE_STACKED_PILLARS_FIRST_PILLAR_UNDER_ANOTHER + 3, EVENT(CrossboneIsleSteppingStones_MovePillar) },
    { TRIGGER_TABLE_END },
};
// clang-format on

struct Trigger gCrossboneIsleDragonStatueTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS,
     CROSSBONE_ISLE_DRAGON_STATUE_TO_CROSSBONE_ISLE_EIGHTH_FLOOR_EAST},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS,
     CROSSBONE_ISLE_DRAGON_STATUE_TO_CROSSBONE_ISLE_EIGHTH_FLOOR_WEST},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING,
     CELL_BEHAVIOR_BESIDE_PUSHED_ACTOR, CONDITION_ALWAYS,
     EVENT(CrossboneIsleSteppingStones_PushSinkingBoulder)},
    {EXAMINE_TRIGGER(EXAMINE_CHEST), TREASURE_SPOT_DRAGON_STATUE_888_COINS,
     FLAG_CROSSBONE_ISLE_DRAGON_STATUE_888_COINS, FIND_COINS(888)},
    {EXAMINE_TRIGGER(EXAMINE_CHEST), TREASURE_SPOT_DRAGON_STATUE_POTION,
     FLAG_CROSSBONE_ISLE_DRAGON_STATUE_POTION, FIND_ITEM(ITEM_POTION)},
    {EXAMINE_TRIGGER(EXAMINE_CHEST), TREASURE_SPOT_DRAGON_STATUE_CLERICS_RING,
     FLAG_CROSSBONE_ISLE_DRAGON_STATUE_CLERICS_RING,
     FIND_ITEM(ITEM_CLERICS_RING)},
    {EXAMINE_TRIGGER(EXAMINE_CHEST), TREASURE_SPOT_DRAGON_STATUE_SMOKE_BOMB,
     FLAG_CROSSBONE_ISLE_DRAGON_STATUE_SMOKE_BOMB, FIND_ITEM(ITEM_SMOKE_BOMB)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_SINKING_BOULDER, FLAG_CROSSBONE_ISLE_DRAGON_STATUE_BOULDER_SUNK,
     EVENT(CrossboneIsleSteppingStones_StartMovingSinkingBoulder)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SINKING_BOULDER,
     FLAG_CROSSBONE_ISLE_DRAGON_STATUE_BOULDER_SUNK,
     EVENT(CrossboneIsleSteppingStones_MoveSinkingBoulder)},
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
    {TRIGGER_TABLE_END},
};
