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
/// - SceneId_CrossboneIsleSteppingStones, off the first floor: stepping stones
///   over a pit. Walking into a pillar jumps two cells across it where the
///   cells allow, and otherwise pushes it
///   (CrossboneIsleSteppingStones_JumpOrPushPillar).
///
/// - SceneId_CrossboneIsleIcePillars, off the second floor: pillars of ice to
///   push, standing and lying across a floor of spikes.
///
/// - SceneId_CrossboneIsleFloodgate, off the fifth floor: a cave with a stream
///   and a floodgate across it, and seven logs to roll.
///
///   The switch beside the floodgate
///   (CrossboneIsleSteppingStones_FlipWaterSwitch) closes it, so the water
///   backs up and floats the logs higher, or opens it to drain them back
///   down. The water level lasts only while the party stays
///   (FLAG_CROSSBONE_ISLE_FLOODGATE_WATER_RAISED, kept through a battle).
///
/// - SceneId_CrossboneIsleStackedPillars, off the seventh floor, a cave of
///   cliffs and ladders. It has four pillars that Move and Carry shift, a
///   ladder, and pillars to jump across. The four pillars settle and redraw
///   their footprints after every push
///   (CrossboneIsleSteppingStones_MovePillar).
///
/// - SceneId_CrossboneIsleDragonStatue, off the eighth floor, by a dragon
///   statue: three puddles that Frost freezes into pillars of ice.
///
///   Frost on the third makes the pillar in it rise and thaws a frozen second
///   puddle. The third freezes only if the leader was not standing on it.
///
///   A boulder moved with Move sinks into place at column 16.
///
/// The other three side rooms are in crossbone_isle_keys.c, and the ninth
/// floor's in crossbone_isle.c.

#include "script_operands.h"
#include "main_routine_work.h"
#include "map_grid_scroll.h"
#include "scene_ids.h"
#include "module/import.h"
#include "module/scene/crossbone_isle_stepping_stones.h"
#include "module/scene/shared/field_effect_spawn.h"
#include "module/field/event_commands.h"
#include "module/system/system.h"
#include "actor.h"
#include "field_effect.h"
#include "create_configured_actor.h"
#include "pushed_actor.h"
#include "io_reg.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "dma.h"
#include "text/msg_ids.h"
#include "event_inline.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "ground_mask.h"
#include "flags.h"
#include "sound_driver_symbols.h"

/// A chest searched while facing the given way.
#define EXAMINE_CHEST_FACING(facing)                                           \
  (EXAMINE_TRIGGER(EXAMINE_CHEST) | TRIGGER_FLAG_FACING | (facing))

/// The placed actors, numbered in the order of each room's actor
/// placements (Scene_GetActorPlacements).
enum CrossboneIsleSteppingStonesActorId {
  /// The pillars of CrossboneIsleSteppingStones (four, SPRITE_STONE_PILLAR),
  /// CrossboneIsleIcePillars (five) and CrossboneIsleStackedPillars (four,
  /// SPRITE_SPIRAL_PILLAR), the only actors those rooms place.
  ACTOR_FIRST_PILLAR = ACTOR_FIRST_PLACED,
  /// The logs, SPRITE_LOG_NORTH_SOUTH and SPRITE_LOG_WEST_EAST as Kolima
  /// Forest's. There are eight in CrossboneIsleFloodgate, where the seven
  /// after the first float on the raised water, and two in
  /// CrossboneIsleDragonStatue.
  ACTOR_FIRST_LOG = ACTOR_FIRST_PLACED,
  /// CrossboneIsleDragonStatue's boulder for Move, SPRITE_TURNING_ROCK, which
  /// sinks into place at column 16.
  ACTOR_SINKING_BOULDER = ACTOR_FIRST_LOG + 2,
  /// CrossboneIsleDragonStatue's three puddles, SPRITE_PUDDLE, which Frost
  /// freezes into pillars of ice.
  ACTOR_FIRST_PUDDLE,
  ACTOR_SECOND_PUDDLE,
  ACTOR_THIRD_PUDDLE,
  /// The pillar in CrossboneIsleDragonStatue's third puddle,
  /// SPRITE_WHITE_PLATFORM, which rises 32 pixels as Frost freezes the puddle.
  ACTOR_RISING_PILLAR,
  /// CrossboneIsleFloodgate's water, SPRITE_SPLASH, raised behind the closed
  /// floodgate.
  ACTOR_WATER = ACTOR_FIRST_LOG + 8
};

/// CrossboneIsleFloodgate: the water has been raised.
#define FLAG_CROSSBONE_ISLE_FLOODGATE_WATER_RAISED FLAG_SCENE_TEMP_00
/// CrossboneIsleStackedPillars: the first pillar has another pushed on top of
/// it, so Move and Carry leave it be. The four pillars' flags follow from this
/// one.
#define FLAG_CROSSBONE_ISLE_STACKED_PILLARS_FIRST_PILLAR_UNDER_ANOTHER         \
  FLAG_SCENE_TEMP_00
/// CrossboneIsleDragonStatue: the first puddle is frozen. The second's and
/// third's flags follow from this one.
#define FLAG_CROSSBONE_ISLE_DRAGON_STATUE_FIRST_PUDDLE_FROZEN FLAG_SCENE_TEMP_00
/// CrossboneIsleDragonStatue: the leader stood on the third puddle when Frost
/// began, so it stays unfrozen.
#define FLAG_CROSSBONE_ISLE_DRAGON_STATUE_LEADER_ON_THIRD_PUDDLE               \
  FLAG_SCENE_TEMP_03
/// CrossboneIsleDragonStatue: the boulder Move pushes has sunk into place at
/// column 16.
#define FLAG_CROSSBONE_ISLE_DRAGON_STATUE_BOULDER_SUNK FLAG_SCENE_TEMP_04

/// A copied map cell, also read in the two flag pairs the pillar redraw keeps.
union CrossboneIslePillarCell {
  struct MapGridCell cell;
  struct CrossboneIslePillarCellBits {
    u32 cellPicture : 12;
    u32 lowFlags : 2; ///< bits 13..12 of MapGridCell.cellPictureAndFlags
    u8 highFlags : 2; ///< bits 15..14, read by MapGrid_GetCellHighFlags
    u8 behavior;
    u8 shape;
  } bits;
};

STATIC_ASSERT(sizeof(union CrossboneIslePillarCell) ==
              sizeof(struct MapGridCell));
STATIC_ASSERT(offsetof(union CrossboneIslePillarCell, bits.behavior) == 2);
STATIC_ASSERT(offsetof(union CrossboneIslePillarCell, bits.shape) == 3);

#include "shared/field_effect_create.h"
#include "shared/field_effect_spawn.h"
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
    wait(1);
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

/// Finds the actor's footprint dimensions, grid position, and map-layer
/// origin.
static s32 CrossboneIsleSteppingStones_QueryFootprint(
    s32 id, u32 *width, u32 *depth, struct PushedActorProbe *probe,
    s32 *layerOriginX, s32 *layerOriginZ) {
  struct MapGridScrollWork *grid = gMapGridWork[0];
  struct Actor *actor = Actor_Get(id);
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
      px(gPushedActor_FootprintBounds[probe->footprintIndex * 4]);
  {
    s32 offsetZ =
        px(gPushedActor_FootprintBounds[probe->footprintIndex * 4 + 1]);

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
  struct Actor *actor = Actor_Get(actorId);
  u32 width, depth;
  s32 layerOriginX, layerOriginZ;
  struct PushedActorProbe probe;
  s32 cellX, cellZ;
  u8 mask = 2;

  if (CrossboneIsleSteppingStones_QueryFootprint(
          actorId, &width, &depth, &probe, &layerOriginX, &layerOriginZ) == 0) {
    return 0;
  }

  cellX = probe.position.x;
  cellZ = probe.position.z;
  MapGrid_CopyCellAttributes(2, 2, width, depth, cellX, cellZ);

  Actor_SelectAnimation(actor, 4);
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

/// Restores the ground cell pictures and cell behaviors under the
/// actor's footprint.
static s32 CrossboneIsleSteppingStones_RestoreGroundFootprint(s32 actorId) {
  struct Actor *actor = Actor_Get(actorId);
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

  if (grid != NULL) {
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
/// cell picture.
static void CrossboneIsleSteppingStones_SetCellAttributes(
    s32 layer, s32 x, s32 y, const union CrossboneIslePillarCell *source) {
  struct MapGridScrollWork *grid = gMapGridWork[0];

  if (grid != NULL) {
    struct CrossboneIslePillarCellBits *cell =
        (struct CrossboneIslePillarCellBits *)grid->layers[layer].cells;

    cell += x + (y << 7);
    cell->lowFlags = source->bits.lowFlags;
    cell->highFlags = source->bits.highFlags;
    cell->behavior = source->bits.behavior;
    cell->shape = source->bits.shape;
  }
}

/// Finds the next terrain ledge and stops the falling actor at its
/// projected height.
static s32 CrossboneIsleSteppingStones_LowerActorToLedge(s32 id, s32 far) {
  struct Actor *actor = Actor_Get(id);
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
  Actor_SetSpriteFlags(actor, 0);
  height = MapGrid_GetTerrainHeight(2, actor->x.fixed, actor->z.fixed);
  level = height / px(16);
  rows = level;
  if (level < 0) {
    rows = -level;
  }
  rows++;
  for (row = 0; row <= rows; row++) {
    top = MapGrid_GetTerrainHeight(actor->layer, actor->x.fixed,
                                   (row << 20) + actor->z.fixed);
    top /= px(16);
    if (level < top) {
      x = (FIELD_CELL(actor->x.fixed) << 20) + px(8);
      if (far == 0) {
        row += 2;
        z = ((row + FIELD_CELL(actor->z.fixed)) << 20) + px(2);
        y = top << 20;
        kind = 223;
      } else {
        row += 3;
        z = ((row + FIELD_CELL(actor->z.fixed)) << 20) - px(2);
        y = top << 20;
        kind = 253;
      }
      temporaryActor = Actor_PrepareFixed(x, y, z, kind);
      height = actor->z.fixed - z + y;
      /* FAKEMATCH(dead): an empty once-only loop; without it, 2 of 117
       * instructions differ in tbs-en. */
      do {
      } while (0);
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

/// Shrinks the pillar and lowers its height for up to thirty-two frames.
static void CrossboneIsleSteppingStones_ShrinkPillar(s32 index) {
  struct Actor *actor = Actor_Get(index);
  u32 elapsedFrames;

  actor->motionFlags = 0;

  elapsedFrames = 0;
  for (;;) {
    if (elapsedFrames > 31)
      return;
    wait(1);
    actor->scaleY -= Q16_ONE / 10;
    actor->y.fixed -= px(4) / 5;
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
  struct EffectOptions options;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  cycleFrame = gFrameCount & 3;
  if (cycleFrame == 0) {
    options.palette = 10;
    options.startScaleX = 7 * Q16_ONE / 10;
    options.startScaleY = 7 * Q16_ONE / 10;
    drawX = Random_Next();
    x = leader->x.fixed + px(((u32)((drawX << 4) + drawX) >> 16) - 8);
    drawZ = Random_Next();
    z = leader->z.fixed + px(((u32)((drawZ << 4) + drawZ) >> 16) - 8);
    drawY = Random_Next();
    velocityY =
        Math_Divide((((u32)((drawY << 2) + drawY) >> 16) << 16) + px(3), 10);
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
  u8 *flags;
  s32 savedMotionFlags;
  s32 jumpDirection;
#if !TBS_PATCH_1
  s32 step;
#endif
  struct FieldPosition destination;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  flags = &leader->motionFlags;
  savedMotionFlags = *flags;
#if TBS_PATCH_1
  jumpDirection = (leader->facing + TRIG_TURN / 8) & 0xc000;
#endif
  if (gMainRoutineWork.movementMode != LEADER_MOVEMENT_WALK) {
    return 0;
  }
#if TBS_PATCH_1
  destination.x = (leader->x.fixed & FIELD_CELL_MASK) + px(8);
  destination.y = leader->y.fixed;
  destination.z = (leader->z.fixed & FIELD_CELL_MASK) + px(8);
  Vector_AddPolarOffset(px(16), jumpDirection, &destination);
#else
  jumpDirection = leader->facing >> 12;
  Actor_StepFrom(&destination, leader,
                 gPushedActor_DirectionSteps[jumpDirection]);
#endif
  if (FieldActor_CheckMovementCollision(leader, &destination) != 1 &&
      PushedActor_FindAtTile(&destination, leader) == 0) {
#if TBS_PATCH_1
    destination.x = (leader->x.fixed & FIELD_CELL_MASK) + px(8);
    destination.y = leader->y.fixed;
    destination.z = (leader->z.fixed & FIELD_CELL_MASK) + px(8);
    Vector_AddPolarOffset(px(32), jumpDirection, &destination);
#else
    step = gPushedActor_DirectionSteps[jumpDirection];
    destination.x = leader->x.fixed + ((step & 0xffff0000) * 2);
    destination.y = leader->y.fixed;
    step <<= 17;
    destination.z = leader->z.fixed + step;
#endif
    if (PushedActor_FindAtTile(&destination, leader) == 0 &&
        FieldActor_CheckMovementCollision(leader, &destination) == 0) {
      Event_Start();
      Actor_SelectAnimation(leader, ANIM_LEADER_SPRING);
      wait(6);
      Sound_Play(SE_JUMP);
      Actor_SelectAnimation(leader, ANIM_LEADER_LEAP);
      leader->speedLimit = px(3);
      leader->acceleration = px(2);
      leader->velocityY = px(4);
      *flags &= 126;
      Actor_SetSpriteFlags(leader, 0);
      Actor_MoveToAndWait(ACTOR_PARTY_LEADER, destination.x >> 16,
                          destination.z >> 16);
      Actor_SelectAnimation(leader, ANIM_LEADER_SPRING);
      Actor_SetSpriteFlags(leader, SPRITE_FLAG_SHADOW);
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
  effect->velocity.x = velocityX - Math_Divide(velocityX, 10);
  effect->velocity.y = velocityY - Math_Divide(velocityY, 3);
  effect->velocity.z = velocityZ - Math_Divide(velocityZ, 10);
  effect->scaleX += effect->scaleRateX;
  effect->scaleY += effect->scaleRateY;
  effect->sprite->rotation += effect->spin;
}

/// Sets the actor's sprite to the colour mode (enum SpriteColorMode) in
/// the low four bits of its timer.
static s32
CrossboneIsleSteppingStones_SetChildValueFromTimer(struct Actor *actor) {
  Actor_SetSpriteColor(actor, actor->timer & 15);
  return 0;
}

const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 sceneId = gMainRoutineWork.next;

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

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s16 sceneId = gMainRoutineWork.next;

  if (sceneId == (s32)&SceneId_CrossboneIsleSteppingStones) {
    return gCrossboneIsleSteppingStonesActorPlacements;
  }
  if (sceneId == (s32)&SceneId_CrossboneIsleIcePillars) {
    return gCrossboneIsleIcePillarsActorPlacements;
  }
  if (sceneId == (s32)&SceneId_CrossboneIsleFloodgate) {
    return gCrossboneIsleFloodgateActorPlacements;
  }
  if (sceneId == (s32)&SceneId_CrossboneIsleDragonStatue) {
    return gCrossboneIsleDragonStatueActorPlacements;
  }
  return gCrossboneIsleSteppingStonesActorPlacementsOther;
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

/// A pillar slot's place and ground cell, without its flag. It holds one slot
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
    if (Random_Below(100) < sShakeChance) {
      src = ((u32 *)sShakenScroll);
    }
  }
  *reg = *src++;
  reg = (volatile u32 *)&REG_BG2HOFS;
  *reg++ = *src++;
  *reg = *src;
}

/// Waits until the DMA channel clears its enable bit.
static inline void Dma_Wait(volatile u32 *dma) {
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
  coordinates[1] += 192;
  coordinates[3] += 192;
  coordinates[5] += 192;
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
  wait(1);
  Irq_SetHandler(1, 0, CrossboneIsleSteppingStones_JitterBackgroundScroll);
  Sound_Play(SE_WATER_LEVEL);
  sShakeChance = 0;
  do {
    wait(1);
  } while (++sShakeChance <= 100);
  // Commit the water tiles, then remove the scroll task and HBlank handler.
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  if (GameFlag_Get(FLAG_CROSSBONE_ISLE_FLOODGATE_WATER_RAISED) == 0) {
    MapGrid_CopyCellsTo(0, 32, 32, 0, 32, 32);
    MapGrid_CopyCellsTo(32, 32, 64, 0, 32, 32);
  } else {
    MapGrid_CopyCellsTo(0, 64, 32, 0, 32, 32);
    MapGrid_CopyCellsTo(32, 64, 64, 0, 32, 32);
  }
  wait(1);
  Irq_SetHandler(1, 0, 0);
  wait(1);
  Task_Remove(CrossboneIsleSteppingStones_PrepareWaterScroll);
  MapGrid_CenterOnFollowedPoint();
  wait(30);
}

/// Searching the switch beside CrossboneIsleFloodgate's floodgate flips it.
static void CrossboneIsleSteppingStones_FlipWaterSwitch(void) {
  s32 waterRaised;
  struct Actor *water;

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
    water = Actor_Get(ACTOR_WATER);
    water->y.fixed = -px(32);
    Actor_SetPosition(ACTOR_WATER, 272, 416);
    Actor_SetAnimation(ACTOR_WATER, SPLASH_ANIMATION_BURST);
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
    Sound_Play(SE_CRUNCH);
    MapGrid_AnimateCells(gCrossboneIsleSteppingStones_SwitchOpenSteps, 84, 24);
    Event_Wait(30);
    Sound_Play(SE_WATER_DRAIN);
    Actor_Get(ACTOR_WATER)->motionFlags = 0;
    water = Actor_Get(ACTOR_WATER);
    water->y.fixed = -px(32);
    Actor_SetPosition(ACTOR_WATER, 272, 436);
    Actor_SetAnimation(ACTOR_WATER, SPLASH_ANIMATION_CHURN);
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
  if (front->x.fixed - px(16) < actor->x.fixed &&
      actor->x.fixed < front->x.fixed + px(16) &&
      front->y.fixed / px(1) == actor->y.fixed / px(1) &&
      front->z.fixed > actor->z.fixed &&
      front->z.fixed - px(32) < actor->z.fixed) {
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
  if (front->x.fixed - px(16) < actor->x.fixed &&
      actor->x.fixed < front->x.fixed + px(16) &&
      actor->y.fixed / px(1) == front->y.fixed / px(1) &&
      front->z.fixed > actor->z.fixed &&
      front->z.fixed - px(32) < actor->z.fixed) {
    if (front->sprite->image.priority > actor->sprite->image.priority) {
      actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
      actor->sprite->image.priority = front->sprite->image.priority;
      actor->sprite->shadow.priority = front->sprite->shadow.priority;
    }
    result = 1;
  }
  return result;
}

/// Keeps each moving pillar's priority and collision flags consistent with the
/// leader.
static UNREAD_RESULT
CrossboneIsleSteppingStones_UpdateActorPriority(struct Actor *actor) {
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);
  s32 hit;
  u32 id;

  if (gFieldWork->castStarted == 1) {
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
    actor = Actor_Get(id);
    actor->layer = 2;
    i = id - ACTOR_FIRST_PILLAR;
    if (FIELD_CELL(actor->x.fixed) == sPillarSlots[i].x &&
        (FIELD_CELL(actor->z.fixed)) == sPillarSlots[i].z &&
        actor->velocityY == 0) {
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
        Sound_Play(SE_STONE_GRIND);
        /* FAKEMATCH(temporary): the priority flags changed as a plain byte; as
         * the field, 100 of 265 instructions differ in tbs-en. */
        *(u8 *)&actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
        CrossboneIsleSteppingStones_LowerActorToLedge(id, 1);
        *(u8 *)&actor->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
      } else if (MapGrid_GetCellBehaviorAt(2, actor->x.fixed, actor->z.fixed) ==
                 51) {
        CrossboneIsleSteppingStones_StopAtGroundOrHeight(actor, 0);
        Sound_Play(SE_STONE_GRIND);
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
    CrossboneIsleSteppingStones_ReadMapGridCell(0, FIELD_CELL(actor->x.fixed),
                                                FIELD_CELL(actor->z.fixed),
                                                &sPillarSlots[i].cell);
    if (actor->y.fixed >= 0) {
      CrossboneIsleSteppingStones_ReadMapGridCell(
          0, 27, FIELD_CELL(actor->y.fixed) + 6, &cell);
      CrossboneIsleSteppingStones_SetCellAttributes(
          0, FIELD_CELL(actor->x.fixed), FIELD_CELL(actor->z.fixed), &cell);
      cell.bits.highFlags = sPillarSlots[i].cell.bits.highFlags;
      /* FAKEMATCH(temporary): x and z read as plain words outside their unions;
       * as actor->x.fixed and z.fixed, 3 of 265 instructions differ in tbs-en.
       */
      CrossboneIsleSteppingStones_SetCellAttributes(
          2, FIELD_CELL(*(s32 *)&actor->x), FIELD_CELL(*(s32 *)&actor->z),
          &cell);
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

/// Walking into a pillar in CrossboneIsleStackedPillars: jump across if
/// possible, otherwise push it, and the pillars settle.
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

/// Walking north at CrossboneIsleStackedPillars's ladder: climb up.
static void CrossboneIsleSteppingStones_ClimbUp(void) { Field_ClimbUp(); }

/// Walking south at CrossboneIsleStackedPillars's ladder: push the pillar
/// below, jump, or climb down.
static void CrossboneIsleSteppingStones_PushOrClimbDown(void) {
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);
  struct FieldPosition position;

  position.x = leader->x.fixed;
  position.y = leader->y.fixed;
  position.z = leader->z.fixed + px(16);
  if (PushedActor_FindAtTile(&position, leader) != NULL) {
    CrossboneIsleSteppingStones_JumpOrPushStackedPillar();
  } else if (CrossboneIsleSteppingStones_TryJumpAcrossPillar() == 0) {
    Field_ClimbDown();
  }
}

/// Stepping on step trigger 40 in CrossboneIsleStackedPillars: the fade overlay
/// follows the leader, and the leader's rise counter starts.
static void CrossboneIsleSteppingStones_StartRising(void) {
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);
  ((struct FadeOverlayWork *)HEAP_SLOT_BLOCK(HEAP_SLOT_FADE_OVERLAY))->actor =
      leader;
  leader->riseCounter = 1;
}

/// Stepping on step trigger 41 in CrossboneIsleStackedPillars: the overlay lets
/// the leader go, and the rise counter stops.
static void CrossboneIsleSteppingStones_StopRising(void) {
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);

  ((struct FadeOverlayWork *)HEAP_SLOT_BLOCK(HEAP_SLOT_FADE_OVERLAY))->actor =
      NULL;
  leader->riseCounter = 0;
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

/// Remembers the frozen puddle, installs its priority update routine, and
/// starts its flicker script.
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

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  puddle = Actor_Get(ACTOR_THIRD_PUDDLE);
  effects = gFieldEffectWork;
  if (FIELD_CELL(puddle->x.fixed) == FIELD_CELL(leader->x.fixed) &&
      puddle->z.fixed >> 20 == FIELD_CELL(leader->z.fixed)) {
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

  pillar = Actor_Get(ACTOR_RISING_PILLAR);
  Actor_Get(ACTOR_THIRD_PUDDLE)->scaleX = Q16_ONE;
  Actor_Get(ACTOR_THIRD_PUDDLE)->scaleY = Q16_ONE;
  (Actor_Get(ACTOR_THIRD_PUDDLE)->sprite)->image.priority = 2;
  pillar->sprite->image.priority = 2;
  pillar->acceleration = px(2) / 5;
  pillar->speedLimit = px(4) / 5;
  FieldActor_SetMoveTargetFixed(pillar, pillar->x.fixed, px(32),
                                pillar->z.fixed);
  Actor_WaitForMoveById(ACTOR_RISING_PILLAR);
  MapGrid_CopyCellAttributes(20, 14, 1, 1, 22, 16);
}

/// Emits growing, rotating particles while Frost freezes the third puddle.
static void CrossboneIsleSteppingStones_SpawnFrostParticle(void) {
  struct EffectOptions options;
  s32 velocityX;
  s32 velocityY;
  u32 draw;
  u32 mask;

  if ((gFrameCount & 2) != 0)
    return;
  if ((gFrameCount & 7) == 0)
    Sound_Play(SE_WHOOSH);

  options.palette = 10;
  options.startScaleX = Q16_ONE / 2;
  options.startScaleY = Q16_ONE / 2;
  options.targetScaleX = 8 * Q16_ONE / 5;
  options.targetScaleY = 8 * Q16_ONE / 5;
  draw = Random_Next();
  mask = 0x0ffff000;
  mask &= draw;
  options.rotation = mask;
  options.update = CrossboneIsleSteppingStones_UpdateDampedParticle;

  draw = Random_Next();
  velocityX = -(px((draw * 5) >> 16) + px(6));
  velocityX /= 2;
  draw = Random_Next();
  velocityY = -(px((draw * 5) >> 16) + px(5));

  FieldEffect_Spawn(px(324), px(48), px(228), velocityX, velocityY, 0,
                    EFFECT_USE_UPDATE | EFFECT_USE_ROTATION |
                        EFFECT_USE_START_SCALE | EFFECT_SCALE_TO_TARGET |
                        EFFECT_USE_PALETTE,
                    &options);
}

/// Emits a randomized particle from the thawing puddle every eight frames.
static UNREAD_RESULT
CrossboneIsleSteppingStones_SpawnThawParticle(struct Actor *actor) {
  struct EffectOptions options;
  u32 phase = gFrameCount & 7;
  s32 x;
  s32 y;
  s32 z;
  s32 scale;

  if (phase != 0)
    RETURN_UNREAD(0);

  options.palette = 7;
  options.startScaleX = 7 * Q16_ONE / 10;
  options.startScaleY = 7 * Q16_ONE / 10;

  x = actor->x.fixed + (((Random_Below(17)) - 8) << 16);
  y = actor->y.fixed + ((Random_Below(17)) << 16);
  z = actor->z.fixed + (((Random_Below(17)) - 8) << 16);
  scale = Math_Divide(px(Random_Below(5)) + px(3), 10);

  FieldEffect_Spawn(x, y, z, 0, scale, phase,
                    EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE |
                        EFFECT_SCRIPT_MEDIUM,
                    &options);
  RETURN_UNREAD(0);
}

/// Frost ends on the third puddle in CrossboneIsleDragonStatue: it freezes,
/// unless the leader stood on it.
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
    // The third puddle freezes while the previously frozen second puddle thaws.
    Task_AddFrame(CrossboneIsleSteppingStones_SpawnFrostParticle,
                  TASK_PASS_FRAME);
    Event_Wait(40);
    if (Event_GetFlag(FLAG_CROSSBONE_ISLE_DRAGON_STATUE_FIRST_PUDDLE_FROZEN +
                      1) != 0) {
#ifdef UBFIX
      Actor_Get(ACTOR_SECOND_PUDDLE)->update =
          CrossboneIsleSteppingStones_SpawnThawParticle;
#else
      Actor_Get(ACTOR_SECOND_PUDDLE)->update = (void (*)(
          struct Actor *))CrossboneIsleSteppingStones_SpawnThawParticle;
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
    Task_Remove(CrossboneIsleSteppingStones_SpawnFrostParticle);
    Event_Wait(20);
    MapGrid_CopyCellsTo(72, 10, 60, 10, 1, 2);
    Event_Wait(20);
    Event_End();
  }
}

/// Move begins on CrossboneIsleDragonStatue's sinking boulder: its cell is
/// cleared.
static void CrossboneIsleSteppingStones_StartMovingSinkingPillar(void) {
  struct Actor *pillar = Actor_Get(ACTOR_SINKING_BOULDER);
  s32 x;
  s32 z;

  Event_Start();

  x = FIELD_CELL(pillar->x.fixed);
  z = FIELD_CELL(pillar->z.fixed);

  PushedActor_FillGridAttributeRectangle(2, x, z, 1, 1, CELL_BEHAVIOR_NORMAL);
  Event_End();
}

/// Move on CrossboneIsleDragonStatue's sinking boulder, and after each push:
/// once it reaches column 16 it sinks two pixels into place for good.
static void CrossboneIsleSteppingStones_MoveSinkingPillar(void) {
  struct Actor *pillar;
  s32 alreadyLowered;

  pillar = Actor_Get(ACTOR_SINKING_BOULDER);
  Event_Start();
  PushedActor_FillGridAttributeRectangle(2, FIELD_CELL(pillar->x.fixed),
                                         (FIELD_CELL(pillar->z.fixed)), 1, 1,
                                         CELL_BEHAVIOR_WALL);
  if (FIELD_CELL(pillar->x.fixed) == 16) {
    alreadyLowered =
        Event_GetFlag(FLAG_CROSSBONE_ISLE_DRAGON_STATUE_BOULDER_SUNK);
    if (alreadyLowered == 0) {
      Event_Wait(10);
      Sound_Play(SE_DOOR_CLOSE);
      pillar->motionFlags = alreadyLowered;
      pillar->groundHeight = -px(2);
      pillar->y.fixed = -px(2);
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
  s16 sceneId = gMainRoutineWork.next;

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
  struct Actor *actor = Actor_Get(id);

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

  actor = Actor_Get(actorId);
  if (GameFlag_Get(FLAG_CROSSBONE_ISLE_DRAGON_STATUE_FIRST_PUDDLE_FROZEN +
                   actorId - ACTOR_FIRST_PUDDLE) != 0) {
    Actor_SelectAnimation(actor, 5);
#ifdef UBFIX
    actor->update = CrossboneIsleSteppingStones_UpdateFrozenPuddlePriority;
#else
    actor->update = (void (*)(
        struct Actor *))CrossboneIsleSteppingStones_UpdateFrozenPuddlePriority;
#endif
    MapGrid_CopyCellAttributes(20, 14, 1, 1, FIELD_CELL(actor->x.fixed),
                               FIELD_CELL(actor->z.fixed));
    Actor_SetScript(actorId, gCrossboneIsleSteppingStones_ActionTable);
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
    if (hero->y.fixed / px(1) == actor->y.fixed / px(1) &&
        hero->z.fixed <= actor->z.fixed - px(8) &&
        hero->z.fixed > actor->z.fixed - px(24)) {
      if (hero->x.fixed - px(16) <= actor->x.fixed &&
          actor->x.fixed < hero->x.fixed + px(16)) {
        Actor_SetSpritePriority(ACTOR_PARTY_LEADER,
                                actor->sprite->image.priority);
        return;
      }
    } else {
      Actor_Get(ACTOR_PARTY_LEADER)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
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
  if (gMainRoutineWork.next == (s32)&SceneId_CrossboneIsleIcePillars) {
    Scene_RedrawActorFootprint(ACTOR_FIRST_PILLAR);
    Scene_RedrawActorFootprint(ACTOR_FIRST_PILLAR + 1);
    Scene_RedrawActorFootprint(ACTOR_FIRST_PILLAR + 2);
    Scene_RedrawActorFootprint(ACTOR_FIRST_PILLAR + 3);
    Scene_RedrawActorFootprint(ACTOR_FIRST_PILLAR + 4);
  } else if (gMainRoutineWork.next == (s32)&SceneId_CrossboneIsleFloodgate) {
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
  } else if (gMainRoutineWork.next ==
             (s32)&SceneId_CrossboneIsleStackedPillars) {
    Actor_CreateConfigured(584, 0, 200, SPRITE_CRATE);
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
      Actor_Get(ACTOR_PARTY_LEADER)->riseCounter = 1;
    }
    FieldEffect_StartFadeOverlay(0);
    if (Actor_Get(ACTOR_PARTY_LEADER)->riseCounter == 0) {
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
  } else if (gMainRoutineWork.next == (s32)&SceneId_CrossboneIsleDragonStatue) {
    Actor_SetAnimation(ACTOR_SINKING_BOULDER, 2);
    Actor_SetSpriteColorById(ACTOR_SINKING_BOULDER, SPRITE_COLOR_GREY);
    Scene_RedrawActorFootprint(ACTOR_FIRST_LOG);
    Scene_RedrawActorFootprint(ACTOR_FIRST_LOG + 1);
#if TBS_PATCH_1
    Actor_Get(ACTOR_FIRST_LOG)->motionFlags = 0;
    Actor_Get(ACTOR_FIRST_LOG + 1)->motionFlags = 0;
#endif
    CrossboneIsleSteppingStones_MoveSinkingPillar();
    CrossboneIsleSteppingStones_PrepLoweredActor(ACTOR_FIRST_PUDDLE);
    CrossboneIsleSteppingStones_PrepLoweredActor(ACTOR_SECOND_PUDDLE);
    CrossboneIsleSteppingStones_PrepLoweredActor(ACTOR_THIRD_PUDDLE);
    CrossboneIsleSteppingStones_RestoreFlaggedPillar(ACTOR_FIRST_PUDDLE);
    CrossboneIsleSteppingStones_RestoreFlaggedPillar(ACTOR_SECOND_PUDDLE);
    CrossboneIsleSteppingStones_RestoreFlaggedPillar(ACTOR_THIRD_PUDDLE);
    Actor_Get(ACTOR_THIRD_PUDDLE)->update = 0;
    CrossboneIsleSteppingStones_PrepLoweredActor(ACTOR_RISING_PILLAR);
    Actor_Get(ACTOR_RISING_PILLAR)->collisionFlags |= ACTOR_NOT_TARGETED;
    if (GameFlag_Get(FLAG_CROSSBONE_ISLE_DRAGON_STATUE_FIRST_PUDDLE_FROZEN +
                     2) == 0) {
      Actor_Get(ACTOR_THIRD_PUDDLE)->scaleX = 3 * Q16_ONE / 2;
      Actor_Get(ACTOR_THIRD_PUDDLE)->scaleY = 3 * Q16_ONE / 2;
      Actor_Get(ACTOR_THIRD_PUDDLE)->sprite->image.priority = 3;
      Actor_Get(ACTOR_RISING_PILLAR)->sprite->image.priority = 3;
      MapGrid_CopyCellAttributes(26, 12, 1, 1, 22, 16);
    }
  }
  return 0;
}

#include "shared/field_effect_duration_scripts.h"

#include "shared/pushed_actor_tables.h"
// clang-format off

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

/// The glint of a puddle Frost has frozen in CrossboneIsleDragonStatue.
///
/// The timer carries a colour mode to
/// CrossboneIsleSteppingStones_SetChildValueFromTimer, with green and blue
/// swapped for two frames in every eight.
s32 gCrossboneIsleSteppingStones_ActionTable[] = {
    SC_SET(SW_TIMER, SPRITE_COLOR_RBG),
    SC_CALL(CrossboneIsleSteppingStones_SetChildValueFromTimer),
    SC_WAIT(2),
    SC_SET(SW_TIMER, SPRITE_COLOR_NORMAL),
    SC_CALL(CrossboneIsleSteppingStones_SetChildValueFromTimer),
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
    ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_WEST_EAST, CONDITION_ALWAYS, 7,
        144, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_WEST_EAST, CONDITION_ALWAYS, 7,
        256, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_WEST_EAST, CONDITION_ALWAYS, 7,
        224, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_NORTH_SOUTH, CONDITION_ALWAYS, 7,
        280, 0, 160, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_NORTH_SOUTH, CONDITION_ALWAYS, 7,
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
    ACTOR_PLACEMENT(SPRITE_SPLASH, CONDITION_ALWAYS, 7,
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
    { TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, 10, CONDITION_ALWAYS, EVENT(CrossboneIsleSteppingStones_JumpOrPushPillar) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 100, FLAG_CROSSBONE_ISLE_STEPPING_STONES_ANTIDOTE, FIND_ITEM(ITEM_ANTIDOTE) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 101, FLAG_CROSSBONE_ISLE_STEPPING_STONES_NUT, FIND_ITEM(ITEM_NUT) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 102, FLAG_CROSSBONE_ISLE_STEPPING_STONES_111_COINS, FIND_COINS(111) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 103, FLAG_CROSSBONE_ISLE_STEPPING_STONES_HARD_NUT, FIND_ITEM(ITEM_HARD_NUT) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_PILLAR, CONDITION_ALWAYS, 0 },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_PILLAR + 1, CONDITION_ALWAYS, 0 },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_PILLAR + 2, CONDITION_ALWAYS, 0 },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_PILLAR + 3, CONDITION_ALWAYS, 0 },
    { TRIGGER_TABLE_END },
};

struct Trigger gCrossboneIsleIcePillarsTriggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, CROSSBONE_ISLE_ICE_PILLARS_TO_CROSSBONE_ISLE_SECOND_FLOOR_EAST },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, CROSSBONE_ISLE_ICE_PILLARS_TO_CROSSBONE_ISLE_SECOND_FLOOR_WEST },
    { TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, 10, CONDITION_ALWAYS, EVENT(CrossboneIsleSteppingStones_PushPillar) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 100, FLAG_CROSSBONE_ISLE_ICE_PILLARS_LUCKY_PEPPER, FIND_ITEM(ITEM_LUCKY_PEPPER) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 101, FLAG_CROSSBONE_ISLE_ICE_PILLARS_MYSTERY_BLADE, FIND_ITEM(ITEM_MYSTERY_BLADE) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 102, FLAG_CROSSBONE_ISLE_ICE_PILLARS_222_COINS, FIND_COINS(222) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 103, FLAG_CROSSBONE_ISLE_ICE_PILLARS_ELIXIR, FIND_ITEM(ITEM_ELIXIR) },
    { TRIGGER_TABLE_END },
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
    { TRIGGER_TABLE_END },
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
    { TRIGGER_TABLE_END },
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
    {TRIGGER_TABLE_END},
};
