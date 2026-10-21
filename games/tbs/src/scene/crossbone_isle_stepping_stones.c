/// @file
/// Five of the side rooms of Crossbone Isle Cave, the cave beneath Crossbone
/// Isle. Each opens off one of the nine floors the cave descends through
/// (crossbone_isle.c) through two doors, east and west: the floor's
/// east door leads in at the room's `_EAST` entrance
/// (CROSSBONE_ISLE_STEPPING_STONES_FROM_CROSSBONE_ISLE_FIRST_FLOOR_EAST in the
/// first) and its west door at the `_WEST` one, and the exits of the same names
/// lead back out. Each is a puzzle with four chests, and all five take the area
/// name Crossbone Isle Cave. In the order the party meets them:
///
/// - SceneId_CrossboneIsleSteppingStones, off the first floor: stepping
///   stones over a pit; walking into a pillar jumps two cells across it where
///   the cells allow, and otherwise pushes it (Event_JumpOrPushPillar).
/// - SceneId_CrossboneIsleIcePillars, off the second floor:
///   pillars of ice to push, standing and lying across a floor of spikes.
/// - SceneId_CrossboneIsleFloodgate, off the fifth floor: a cave
///   with a stream and a floodgate across it, and seven logs to roll. The
///   switch beside the floodgate (Event_FlipWaterSwitch) closes it, so the
///   water backs up and floats the logs higher, or opens it to drain them
///   back down; the water level lasts only while the party stays
///   (FLAG_SCENE_TEMP_00, kept through a battle).
/// - SceneId_CrossboneIsleStackedPillars, off the seventh floor, a cave of
///   cliffs and ladders: four pillars that Move and Carry shift, and that
///   settle and redraw their footprints after every push (Event_MovePillar), a
///   ladder, and pillars to jump across.
/// - SceneId_CrossboneIsleDragonStatue, off the eighth floor, by a dragon
///   statue: three puddles that Frost freezes into pillars of ice. Frost on
///   the third makes the pillar in it rise and thaws a frozen second puddle,
///   and the third freezes only if the leader was not standing on it; a boulder
///   moved with Move sinks into place at column 16.
///
/// The other three side rooms are in crossbone_isle_keys.c, and the
/// ninth floor's in crossbone_isle.c.
#include "script.h"
#include "game_state.h"
#include "map_scroll.h"
#include "scene_ids.h"
#include "unit/import.h"
#include "unit/scene/crossbone_isle_stepping_stones.h"
#include "unit/scene/common/effect_spawn.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "unit/runtime/runtime.h"
#include "field_obj.h"
#include "types.h"
#include "animspr.h"
#include "field_event.h"
#include "field_scene.h"
#include "field_effect.h"
#include "create_configured_overlay_object.h"
#include "staged_actor.h"
#include "ram_buffer.h"
#include "map.h"
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
#include "global_cells.h"
#include "dma.h"
#include "edition.h"
#include "text/msg_ids.h"
#include "call.h"
#include "field_service.h"
#include "irq.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"
#include "heap_state.h"
#include "field_runtime.h"
#include "object_effect.h"
#include "fixed_math.h"
#include "flags.h"
#include "songs.h"
#include "event.h"
#include "macros.h"

/// The placed actors, numbered in the order of each room's placement table
/// (Scene_GetPlacements).
enum CrossboneIsleSteppingStonesActor {
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
#define WATER_RAISED_FLAG FLAG_SCENE_TEMP_00
// CrossboneIsleStackedPillars keeps FLAG_SCENE_TEMP_00 to 03 for its four
// pillars' footprints.
// CrossboneIsleDragonStatue: FLAG_SCENE_TEMP_00 to 02 the three puddles are
// frozen, 03 the leader stood on the third when Frost began, 04 the sinking
// pillar has sunk.
#define FIRST_FROZEN_PUDDLE_FLAG FLAG_SCENE_TEMP_00
#define LEADER_ON_THIRD_PUDDLE_FLAG FLAG_SCENE_TEMP_03
#define PILLAR_SUNK_FLAG FLAG_SCENE_TEMP_04

/// A copied map cell, also read in the two flag pairs the pillar redraw keeps.
union CrossboneIslePillarCell {
  struct MapCell cell;
  struct CrossboneIslePillarCellBits {
    u32 metatile : 12;
    u32 lowFlags : 2; ///< bits 13..12 of MapCell.metatileAndFlags
    u8 highFlags : 2; ///< bits 15..14, read by Map_GetCellHighFlags
    u8 collisionCode;
    u8 attributeB;
  } bits;
};

STATIC_ASSERT(sizeof(union CrossboneIslePillarCell) == sizeof(struct MapCell));
STATIC_ASSERT(offsetof(union CrossboneIslePillarCell, bits.collisionCode) == 2);
STATIC_ASSERT(offsetof(union CrossboneIslePillarCell, bits.attributeB) == 3);

#include "common/effect_create.h"
#include "common/effect_spawn.h"
#include "common/object_staged_actor.h"

#include "common/map_panel.h"

/// Waits up to sixty frames for landing, then stops vertical movement on the
/// ground.
static void
CrossboneIsleSteppingStones_SettleActorOnGround(struct FieldActor *actor) {
  s32 remainingFrames = 60;

  for (;;) {
    if (remainingFrames == 0) {
      break;
    }
    Engine_TaskWait(1);
    if ((u32)actor->y.fixed == (u32)actor->terrainHeight) {
      break;
    }
    remainingFrames--;
  }

  actor->velocityY = 0;
  actor->targetY = ACTOR_NO_TARGET;
  actor->y.fixed = actor->terrainHeight;
}

/// Waits until the actor reaches the ground or height limit, then stops its
/// descent.
static void
CrossboneIsleSteppingStones_StopAtGroundOrHeight(struct FieldActor *actor,
                                                 s32 limit) {
  s32 remainingFrames = 60;

  for (;;) {
    if (remainingFrames == 0) {
      break;
    }
    Engine_TaskWait(1);
    if (actor->y.fixed <= actor->terrainHeight) {
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

/// Finds the actor's footprint dimensions, grid position, and map-layer origin.
static s32 CrossboneIsleSteppingStones_QueryFootprint(
    s32 id, u32 *width, u32 *depth, struct StagedActorProbe *probe,
    s32 *layerOriginX, s32 *layerOriginZ) {
  struct MapScrollWork *map = gMapWork[0];
  struct FieldActor *actor = Object_GetById(id);
  u32 i;
  s32 coordinate;
  s32 farExtent;

  for (i = 0; i <= 5; i++) {
    if (actor->sprite->entries[0]->animId == gStagedActor_FootprintKinds[i]) {
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
  coordinate = gStagedActor_FootprintBounds[probe->footprintIndex * 4 + 1];
  if (coordinate < 0) {
    coordinate = -coordinate;
  }
  farExtent = gStagedActor_FootprintBounds[probe->footprintIndex * 4 + 3];
  if (farExtent < 0) {
    farExtent = -farExtent;
  }
  *depth = (coordinate + farExtent) >> 4;
  coordinate = gStagedActor_FootprintBounds[probe->footprintIndex * 4];
  if (coordinate < 0) {
    coordinate = -coordinate;
  }
  farExtent = gStagedActor_FootprintBounds[probe->footprintIndex * 4 + 2];
  if (farExtent < 0) {
    farExtent = -farExtent;
  }
  *width = (coordinate + farExtent) >> 4;
  // Move from the actor's center to the footprint's top-left cell.
  probe->position.x +=
      gStagedActor_FootprintBounds[probe->footprintIndex * 4] * PIXELS(1);
  {
    s32 offsetZ =
        gStagedActor_FootprintBounds[probe->footprintIndex * 4 + 1] * PIXELS(1);

    coordinate = probe->position.z;
    coordinate += offsetZ;
    coordinate >>= 20;
    probe->position.z = coordinate;
  }
  probe->position.x >>= 20;
  *layerOriginX = map->layers[1].offsetX >> 20;
  *layerOriginZ = map->layers[1].offsetY >> 20;
  return 1;
}

/// Draws a log's footprint where it floats on CrossboneIsleFloodgate's raised
/// water, lying across or along the room as the log does, and puts its actor
/// under the leader.
static s32 CrossboneIsleSteppingStones_DrawRaisedFootprint(s32 actorId) {
  struct FieldActor *actor = Object_GetById(actorId);
  u32 width, depth;
  s32 layerOriginX, layerOriginZ;
  struct StagedActorProbe probe;
  s32 cellX, cellZ;
  u8 mask;

  if (CrossboneIsleSteppingStones_QueryFootprint(
          actorId, &width, &depth, &probe, &layerOriginX, &layerOriginZ) == 0) {
    return 0;
  }

  cellX = probe.position.x;
  cellZ = probe.position.z;
  Map_CopyCellAttributes(2, 2, width, depth, cellX, cellZ);

  Object_SetMode(actor, 4);
  mask = 2;
  actor->priorityFlags |= mask;

  if (width > depth) {
    Map_CopyCellsTo(70, 40, probe.position.x + 32, probe.position.z + 2, width,
                    depth);
  } else {
    Map_CopyCellsTo(68, 40, probe.position.x + 32, probe.position.z + 2, width,
                    depth);
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

/// Restores the ground tiles and collision attributes under the actor's
/// footprint.
static s32 CrossboneIsleSteppingStones_RestoreGroundFootprint(s32 actorId) {
  struct FieldActor *actor = Object_GetById(actorId);
  u32 width, depth;
  s32 layerOriginX, layerOriginZ;
  struct StagedActorProbe probe;

  if (CrossboneIsleSteppingStones_QueryFootprint(
          actorId, &width, &depth, &probe, &layerOriginX, &layerOriginZ) == 0) {
    return 0;
  }

  {
    s32 sourceCellX = layerOriginX + probe.position.x;
    s32 sourceCellZ = layerOriginZ + probe.position.z;

    Map_CopyCellAttributes(sourceCellX, sourceCellZ, width, depth,
                           probe.position.x, probe.position.z);
    StagedActor_FillGridAttributeRectangle(0, probe.position.x,
                                           probe.position.z, width, depth, 255);
  }

  Object_SetMode(actor, 1);
  actor->priorityFlags &= ~ACTOR_PRIORITY_UNDERFOOT;

  return 1;
}

/// Reads one map cell through DMA, waiting until the transfer finishes.
static void
CrossboneIsleSteppingStones_ReadMapCell(s32 layer, s32 x, s32 y,
                                        union CrossboneIslePillarCell *dst) {
  struct MapScrollWork *map = gMapWork[0];

  if (map != 0) {
    struct MapCell *cell = map->layers[layer].cells;

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

/// Copies the cell's flags and collision attributes without replacing its tile.
static void CrossboneIsleSteppingStones_SetCellAttributes(
    s32 layer, s32 x, s32 y, const union CrossboneIslePillarCell *source) {
  struct MapScrollWork *map = gMapWork[0];

  if (map != 0) {
    // UB: the native bitfield view aliases MapCell storage. Byte writes keep
    // its fields separate without that alias (tbs-en: 88 to 84 bytes).
#ifdef UBFIX
    u8 *cell = (u8 *)map->layers[layer].cells;

    cell += sizeof(struct MapCell) * (x + (y << 7));
    cell[1] = (source->bits.lowFlags << 4) | (cell[1] & ~0x30);
    cell[1] = (source->bits.highFlags << 6) | (cell[1] & ~0xc0);
    cell[2] = source->cell.collisionCode;
    cell[3] = source->cell.attributeB;
#else
    struct CrossboneIslePillarCellBits *cell =
        (struct CrossboneIslePillarCellBits *)map->layers[layer].cells;

    cell += x + (y << 7);
    cell->lowFlags = source->bits.lowFlags;
    cell->highFlags = source->bits.highFlags;
    cell->collisionCode = source->bits.collisionCode;
    cell->attributeB = source->bits.attributeB;
#endif
  }
}

/// Finds the next terrain ledge and stops the falling actor at its projected
/// height.
static s32 CrossboneIsleSteppingStones_LowerActorToLedge(s32 id, s32 far) {
  struct FieldActor *actor = Object_GetById(id);
  s32 found;
  struct FieldActor *temporaryActor;
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
  Engine_ActorSetSpriteFlags(actor, found);
  height = Map_GetTerrainHeight(2, actor->x.fixed, actor->z.fixed);
  level = height / PIXELS(16);
  rows = level;
  if (level < 0) {
    rows = -level;
  }
  rows++;
  for (row = 0; row <= rows; row++) {
    top = Map_GetTerrainHeight(actor->terrainId, actor->x.fixed,
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
        temporaryActor = OverlayObject_PrepareObject(x, y, z, kind);
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
    Engine_ObjectDispatchRelease(temporaryActor);
  }
  return found;
}

/// Shrinks the pillar and lowers its height for up to thirty-two frames.
static void CrossboneIsleSteppingStones_ShrinkPillar(s32 index) {
  struct FieldActor *actor = Object_GetById(index);
  u32 elapsedFrames;

  actor->motionFlags = 0;

  elapsedFrames = 0;
  for (;;) {
    if (elapsedFrames > 31)
      return;
    Engine_TaskWait(1);
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

  struct FieldActor *leader;
  s32 cycleFrame;
  s32 x;
  s32 z;
  s32 velocityY;
  s32 drawX;
  s32 drawZ;
  s32 drawY;
  struct EffectOptions *settings;
  struct EffectOptions options;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  cycleFrame = gFrameCount & 3;
  if (cycleFrame == 0) {
    settings = &options;
    settings->palette = 10;
    settings->startScaleX = 7 * Q16_ONE / 10;
    settings->startScaleY = 7 * Q16_ONE / 10;
    drawX = Engine_RandomNext();
    x = leader->x.fixed + ((((u32)((drawX << 4) + drawX) >> 16) - 8) << 16);
    drawZ = Engine_RandomNext();
    z = leader->z.fixed + ((((u32)((drawZ << 4) + drawZ) >> 16) - 8) << 16);
    drawY = Engine_RandomNext();
    velocityY = Math_Divide(
        (((u32)((drawY << 2) + drawY) >> 16) << 16) + PIXELS(3), 10);
    Effect_Spawn(x, leader->y.fixed, z, 0, velocityY, cycleFrame,
                 EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE | 1, settings);
  }
}

/// Jumps two cells forward when both the intervening and landing cells permit
/// it.
static s32 CrossboneIsleSteppingStones_TryJumpAcrossPillar(void) {
  struct FieldActor *leader;
  u8 *flags;
  s32 savedMotionFlags;
  s32 jumpDirection;
#if !TBS_PATCH_1
  s32 step;
#endif
  struct FieldPosition *position;
  struct FieldPosition destination;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  flags = &leader->motionFlags;
  savedMotionFlags = *flags;
#if TBS_PATCH_1
  jumpDirection = (leader->facing + 0x2000) & 0xc000;
#endif
  if (gGameState.movementMode != 0) {
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
  step = gStagedActor_DirectionSteps[jumpDirection];
  position->x = leader->x.fixed + (step & 0xffff0000);
  position->y = leader->y.fixed;
  step <<= 16;
  position->z = leader->z.fixed + step;
#endif
  // Require a free intervening cell before testing the two-cell landing.
  if (FieldObject_CheckMovementCollision(leader, position) != 1 &&
      StagedActor_FindAtTile(position, leader) == 0) {
#if TBS_PATCH_1
    position->x = (leader->x.fixed & -0x100000) + PIXELS(8);
    position->y = leader->y.fixed;
    position->z = (leader->z.fixed & -0x100000) + PIXELS(8);
    Vector_AddPolarOffset(PIXELS(32), jumpDirection, position);
#else
    step = gStagedActor_DirectionSteps[jumpDirection];
    position->x = leader->x.fixed + ((step & 0xffff0000) * 2);
    position->y = leader->y.fixed;
    step <<= 17;
    position->z = leader->z.fixed + step;
#endif
    if (StagedActor_FindAtTile(position, leader) == 0 &&
        FieldObject_CheckMovementCollision(leader, position) == 0) {
      EV_BEGIN();
      Object_SetMode(leader, 6);
      Engine_TaskWait(6);
      Engine_AudioPlayCue(SE_JUMP);
      Object_SetMode(leader, 7);
      leader->speed = PIXELS(3);
      leader->acceleration = PIXELS(2);
      leader->velocityY = PIXELS(4);
      *flags &= 126;
      Engine_ActorSetSpriteFlags(leader, 0);
      Engine_ActorMoveToAndWait(ACTOR_PARTY_LEADER, position->x >> 16,
                                position->z >> 16);
      Object_SetMode(leader, 6);
      Engine_ActorSetSpriteFlags(leader, 1);
      *flags = savedMotionFlags;
      EV_END();
      return 1;
    }
  }
  return 0;
}

/// Moves the particle while damping its velocity, growing it, and advancing
/// rotation.
static void
CrossboneIsleSteppingStones_UpdateDampedParticle(union FieldObject *object) {
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;

  object->effect.x += object->effect.velocity.x;
  object->effect.y += object->effect.velocity.y;
  object->effect.z += object->effect.velocity.z;
  velocityX = object->effect.velocity.x;
  velocityY = object->effect.velocity.y;
  velocityZ = object->effect.velocity.z;
  object->effect.velocity.x = velocityX - Math_Divide(velocityX, 10);
  object->effect.velocity.y = velocityY - Math_Divide(velocityY, 3);
  object->effect.velocity.z = velocityZ - Math_Divide(velocityZ, 10);
  object->effect.scaleX += object->effect.scaleRateX;
  object->effect.scaleY += object->effect.scaleRateY;
  object->effect.sprite->rotation += object->effect.spin;
}

/// Applies the actor timer's low four bits to its child sprite value.
static s32 CrossboneIsleSteppingStones_SetChildValueFromTimer(
    struct ScriptInterpreter *interpreter) {
  struct FieldActor *actor = (struct FieldActor *)interpreter;
  ObjectGroup_SetChildValue(actor, actor->timer & 15);
  return 0;
}

const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_CrossboneIsleSteppingStones) {
    return gCrossboneIsleSteppingStonesEntrances;
  }
  if (scene == (s32)&SceneId_CrossboneIsleIcePillars) {
    return gCrossboneIsleIcePillarsEntrances;
  }
  if (scene == (s32)&SceneId_CrossboneIsleFloodgate) {
    return gCrossboneIsleFloodgateEntrances;
  }
  if (scene == (s32)&SceneId_CrossboneIsleStackedPillars) {
    return gCrossboneIsleStackedPillarsEntrances;
  }
  if (scene == (s32)&SceneId_CrossboneIsleDragonStatue) {
    return gCrossboneIsleDragonStatueEntrances;
  }
  return gCrossboneIsleSteppingStonesEntrancesOther;
}

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

/// Returns the exits the five rooms share.
const u32 *Scene_GetExits(void) { return gCrossboneIsleSteppingStonesExits; }

const struct ScenePlacement *Scene_GetPlacements(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_CrossboneIsleSteppingStones) {
    return gCrossboneIsleSteppingStonesPlacements;
  }
  if (scene == (s32)&SceneId_CrossboneIsleIcePillars) {
    return gCrossboneIsleIcePillarsPlacements;
  }
  if (scene == (s32)&SceneId_CrossboneIsleFloodgate) {
    return gCrossboneIsleFloodgatePlacements;
  }
  if (scene == (s32)&SceneId_CrossboneIsleDragonStatue) {
    return gCrossboneIsleDragonStatuePlacements;
  }
  return gCrossboneIsleSteppingStonesPlacementsOther;
}

/// Walking into a pillar in CrossboneIsleSteppingStones: jump two cells across
/// it where the cells allow, otherwise push it.
static void Event_JumpOrPushPillar(void) {
  EV_BEGIN();
  if (CrossboneIsleSteppingStones_TryJumpAcrossPillar() == 0)
    StagedActor_AdvancePair();
  EV_END();
}

/// Walking into a pillar in CrossboneIsleIcePillars, or a log in
/// CrossboneIsleFloodgate: push it if the cell beyond is clear.
static void Event_PushPillar(void) {
  struct StagedActorProbe out;

  EV_BEGIN();
  if (StagedActor_FindClearPosition(&out) != 0)
    SceneActor_MoveAndRedraw(out);
  EV_END();
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
  volatile u32 *reg = (volatile u32 *)0x04000014;

  if (line == 227 || line <= 46) {
    if ((u32)(Engine_RandomNext() * 100) >> 16 < sShakeChance) {
      src = ((u32 *)sShakenScroll);
    }
  }
  *reg = *src++;
  reg = (volatile u32 *)0x04000018;
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

  waterRaised = Engine_GameFlagIsSet(WATER_RAISED_FLAG);
  if (waterRaised == 0) {
    Map_DrawCellPanel(10, 19, 16, 5, waterRaised, 10, 31);
    Map_DrawCellPanel(10, 51, 16, 5, 1, 10, 31);
    Map_DrawCellPanel(42, 51, 16, 5, 2, 10, 31);
  } else {
    Map_DrawCellPanel(10, 19, 16, 5, 0, 10, 31);
    Map_DrawCellPanel(10, 83, 16, 5, 1, 10, 31);
    Map_DrawCellPanel(42, 83, 16, 5, 2, 10, 31);
  }
  // Increase the chance of showing the alternate scroll on each scanline.
  sShakeChance = 0;
  FieldScheduler_AddCallback(CrossboneIsleSteppingStones_PrepareWaterScroll,
                             0xc80);
  Engine_TaskWait(1);
  Runtime_SetIrqHandler(1, 0,
                        CrossboneIsleSteppingStones_JitterBackgroundScroll);
  Engine_AudioPlayCue(SE_231);
  sShakeChance = 0;
  do {
    Engine_TaskWait(1);
  } while (++sShakeChance <= 100);
  // Commit the water tiles, then remove both scroll callbacks.
  Engine_AudioPlayCue(SE_289);
  if (Engine_GameFlagIsSet(WATER_RAISED_FLAG) == 0) {
    Map_CopyCellsTo(0, 32, 32, 0, 32, 32);
    Map_CopyCellsTo(32, 32, 64, 0, 32, 32);
  } else {
    Map_CopyCellsTo(0, 64, 32, 0, 32, 32);
    Map_CopyCellsTo(32, 64, 64, 0, 32, 32);
  }
  Engine_TaskWait(1);
  Runtime_SetIrqHandler(1, 0, 0);
  Engine_TaskWait(1);
  Engine_TaskRemoveCallback(CrossboneIsleSteppingStones_PrepareWaterScroll);
  Engine_MapRedraw();
  Engine_TaskWait(30);
}

/// Searching the switch beside CrossboneIsleFloodgate's floodgate flips it.
/// "Isaac flipped the switch." The floodgate closes and the water backs up
/// behind it, floating the logs higher, or it opens and the water drains away
/// and the logs settle on the floor.
static void Event_FlipWaterSwitch(void) {
  s32 waterRaised;
  struct FieldActor *water;

  EV_BEGIN();
  Camera_SetSpeed(PIXELS(1), PIXELS(1) / 8);
  Camera_MoveTo(PIXELS(281), -1, PIXELS(432), 1);
  Engine_CameraWaitForMove();
  Engine_MessageShowCentered(MsgFieldFlippedSwitch, 1);
  waterRaised = FLAG_CHECK(WATER_RAISED_FLAG);
  // Raise or drain the water before redrawing the logs' footprints.
  if (waterRaised == 0) {
    SE(SE_232);
    Map_AnimateCells(gCrossboneIsleSteppingStones_ShiftSteps1, 84, 24);
    WAIT(30);
    SE(SE_240);
    Engine_ActorSetSpritePriority(ACTOR_WATER, 1);
    Object_GetById(ACTOR_WATER)->motionFlags = waterRaised;
    water = Actor_Get(ACTOR_WATER);
    water->y.fixed = -PIXELS(32);
    OBJ_POS_SET(ACTOR_WATER, 272, 416);
    OBJ_MODE(ACTOR_WATER, 1);
    Map_AnimateCells(gCrossboneIsleSteppingStones_ShiftSteps3, 80, 24);
    Map_AnimateCells(gCrossboneIsleSteppingStones_ShiftSteps5, 80, 28);
    Map_CopyCellsTo(65, 40, 16, 27, 2, 4);
    CrossboneIsleSteppingStones_AnimateWaterLevel();
    CrossboneIsleSteppingStones_DrawFloatingLogs();
    Map_CopyCellAttributes(24, 3, 1, 1, 24, 8);
    FLAG_SET(WATER_RAISED_FLAG);
  } else {
    SE(SE_232);
    Map_AnimateCells(gCrossboneIsleSteppingStones_ShiftSteps2, 84, 24);
    WAIT(30);
    SE(SE_230);
    Object_GetById(ACTOR_WATER)->motionFlags = 0;
    water = Actor_Get(ACTOR_WATER);
    water->y.fixed = -PIXELS(32);
    OBJ_POS_SET(ACTOR_WATER, 272, 436);
    OBJ_MODE(ACTOR_WATER, 2);
    Map_CopyCellsTo(65, 45, 16, 27, 2, 4);
    Map_AnimateCells(gCrossboneIsleSteppingStones_ShiftSteps4, 80, 24);
    CrossboneIsleSteppingStones_AnimateWaterLevel();
    CrossboneIsleSteppingStones_RestoreGroundFootprint(ACTOR_FIRST_LOG + 1);
    CrossboneIsleSteppingStones_RestoreGroundFootprint(ACTOR_FIRST_LOG + 2);
    CrossboneIsleSteppingStones_RestoreGroundFootprint(ACTOR_FIRST_LOG + 3);
    CrossboneIsleSteppingStones_RestoreGroundFootprint(ACTOR_FIRST_LOG + 4);
    CrossboneIsleSteppingStones_RestoreGroundFootprint(ACTOR_FIRST_LOG + 5);
    CrossboneIsleSteppingStones_RestoreGroundFootprint(ACTOR_FIRST_LOG + 6);
    CrossboneIsleSteppingStones_RestoreGroundFootprint(ACTOR_FIRST_LOG + 7);
    Map_CopyCellAttributes(24, 4, 1, 1, 24, 8);
    GameFlag_Clear(WATER_RAISED_FLAG);
  }
  EV_END();
}

/// Matches a nearby actor's draw priority when it is behind the comparison
/// actor.
static s32
CrossboneIsleSteppingStones_SyncPriorityIfBehind(struct FieldActor *actor,
                                                 struct FieldActor *front) {
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

/// Matches the comparison actor's draw priority when it overlaps the actor in
/// front.
static s32
CrossboneIsleSteppingStones_SyncPriorityIfAhead(struct FieldActor *front,
                                                struct FieldActor *actor) {
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

// UB: calling this s32 function through a void update hook is incompatible.
// Plain void changes 320 to 316 bytes in tbs-en.
#ifdef UBFIX
/// Keeps each moving pillar's priority and collision flags consistent with the
/// leader.
static void CrossboneIsleSteppingStones_UpdateActorPriority(
    union FieldObject *fieldObject) {
#else
/// Keeps each moving pillar's priority and collision flags consistent with the
/// leader.
static s32 CrossboneIsleSteppingStones_UpdateActorPriority(
    union FieldObject *fieldObject) {
#endif
  struct FieldActor *actor = (struct FieldActor *)fieldObject;
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  s32 hit;
  u32 id;

  if (((struct FieldWork *)gEventWork)->castStarted == 1) {
    actor->sprite->image.priority = leader->sprite->image.priority;
    actor->collisionFlags |= 1;
#ifdef UBFIX
    return;
#else
    return 0;
#endif
  }
  hit = CrossboneIsleSteppingStones_SyncPriorityIfBehind(actor, leader);
  for (id = ACTOR_FIRST_PILLAR; id <= ACTOR_FIRST_PILLAR + 3; id++) {
    hit += CrossboneIsleSteppingStones_SyncPriorityIfBehind(actor,
                                                            Object_GetById(id));
  }
  if (hit != 0) {
    for (id = ACTOR_FIRST_PILLAR; id <= ACTOR_FIRST_PILLAR + 3; id++) {
      CrossboneIsleSteppingStones_SyncPriorityIfAhead(actor,
                                                      Object_GetById(id));
    }
  }
  if (actor->y.fixed < leader->y.fixed) {
    actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    actor->collisionFlags &= 254;
    if (actor->sprite->image.priority < leader->sprite->image.priority) {
      actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
      actor->sprite->image.priority = leader->sprite->image.priority;
      actor->sprite->shadow.priority = leader->sprite->shadow.priority;
      hit = 1;
    }
  } else {
    actor->priorityFlags &= ~ACTOR_PRIORITY_UNDERFOOT;
    actor->collisionFlags |= 1;
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
  struct FieldActor *firstActor;
  struct FieldActor *secondActor;
  struct FieldActor savedActor;
#ifdef UBFIX
  struct PillarPlacement savedPlacement;
#else
  // UB: the original 12-byte scratch receives a 16-byte DMA transfer.
  // A complete placement record adds four native stack bytes.
  s32 savedPlacement[3];
#endif

  for (i = 0; i <= 2; i++) {
    firstActor = Object_GetById(i + ACTOR_FIRST_PILLAR);
    for (j = i; j <= 3; j++) {
      secondActor = Object_GetById(j + ACTOR_FIRST_PILLAR);
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
      if (Engine_GameFlagIsSet(sPillarSlots[i].flag) &&
          !Engine_GameFlagIsSet(sPillarSlots[j].flag)) {
        Engine_GameFlagClear(sPillarSlots[i].flag);
        Engine_GameFlagSet(sPillarSlots[j].flag);
      } else if (!Engine_GameFlagIsSet(sPillarSlots[i].flag) &&
                 Engine_GameFlagIsSet(sPillarSlots[j].flag)) {
        Engine_GameFlagSet(sPillarSlots[i].flag);
        Engine_GameFlagClear(sPillarSlots[j].flag);
      }
    }
  }
}

/// Move or Carry on one of CrossboneIsleStackedPillars's pillars, and after
/// each push there: the pillars settle, and their footprints follow them.
static void Event_MovePillar(void) {
  u32 id;
  struct FieldActor *actor;
  u8 *flags;
  u32 i;
  u32 k;
  struct FieldActor *other;
  union CrossboneIslePillarCell cell;

  for (id = ACTOR_FIRST_PILLAR; id <= ACTOR_FIRST_PILLAR + 3; id++) {
    actor = Object_GetById(id);
    actor->terrainId = 2;
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
    if (FieldObject_CheckMovementCollision(actor, &sProbePosition) == -1) {
      actor->motionFlags = 3;
    }
    flags = &actor->motionFlags;
    CrossboneIsleSteppingStones_SetCellAttributes(
        0, sPillarSlots[i].x, sPillarSlots[i].z, &sPillarSlots[i].cell);
    CrossboneIsleSteppingStones_SetCellAttributes(
        2, sPillarSlots[i].x, sPillarSlots[i].z, &sPillarSlots[i].cell);
    // Falling pillars either reach a ledge, shrink away, or settle on ground.
    if (*flags & 1) {
      if (GetMapCellCollision(2, actor->x.fixed, actor->z.fixed) == 50) {
        Engine_AudioPlayCue(SE_189);
        // Both u8 accesses are alias set 0, and the view is no struct reference
        // (MEM_IN_STRUCT_P); as field stores, the loop is reallocated.
        *(u8 *)&actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
        CrossboneIsleSteppingStones_LowerActorToLedge(id, 1);
        *(u8 *)&actor->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
      } else if (GetMapCellCollision(2, actor->x.fixed, actor->z.fixed) == 51) {
        CrossboneIsleSteppingStones_StopAtGroundOrHeight(actor, 0);
        Engine_AudioPlayCue(SE_189);
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
    CrossboneIsleSteppingStones_ReadMapCell(
        0, actor->x.fixed >> 20, actor->z.fixed >> 20, &sPillarSlots[i].cell);
    if (actor->y.fixed >= 0) {
      CrossboneIsleSteppingStones_ReadMapCell(0, 27, (actor->y.fixed >> 20) + 6,
                                              &cell);
      CrossboneIsleSteppingStones_SetCellAttributes(
          0, actor->x.fixed >> 20, actor->z.fixed >> 20, &cell);
      cell.bits.highFlags = sPillarSlots[i].cell.bits.highFlags;
      CrossboneIsleSteppingStones_SetCellAttributes(
          2, *(s32 *)((u8 *)actor + offsetof(struct FieldActor, x)) >> 20,
          *(s32 *)((u8 *)actor + offsetof(struct FieldActor, z)) >> 20, &cell);
    }
    sPillarSlots[i].x = actor->x.fixed >> 20;
    sPillarSlots[i].y = actor->y.fixed >> 20;
    sPillarSlots[i].z = actor->z.fixed >> 20;
    // Mark pillars covered by this one, then restore their sort order.
    for (k = 0; k <= 3; k++) {
      if (k == i) {
        continue;
      }
      Engine_GameFlagClear(sPillarSlots[k].flag);
      other = Object_GetById(k + ACTOR_FIRST_PILLAR);
      if ((actor->x.fixed >> 20) == (other->x.fixed >> 20) &&
          (actor->z.fixed >> 20) == (other->z.fixed >> 20) &&
          actor->y.fixed > other->y.fixed) {
        Engine_GameFlagSet(sPillarSlots[k].flag);
      }
    }
  }
  CrossboneIsleSteppingStones_SortPillarActors();
}

/// Walking into a pillar in CrossboneIsleStackedPillars: jump across if
/// possible, otherwise push it, and the pillars settle.
static void Event_JumpOrPushStackedPillar(void) {
  EV_BEGIN();
#if TBS_PATCH_3
  Object_GetById(ACTOR_PARTY_LEADER)->y.fixed =
      Map_GetTerrainHeight(Object_GetById(ACTOR_PARTY_LEADER)->terrainId,
                           Object_GetById(ACTOR_PARTY_LEADER)->x.fixed,
                           Object_GetById(ACTOR_PARTY_LEADER)->z.fixed);
#endif
  if (CrossboneIsleSteppingStones_TryJumpAcrossPillar() == 0) {
    Object_GetById(ACTOR_PARTY_LEADER)->motionFlags &= 254;
    Object_GetById(ACTOR_PARTY_LEADER)->priorityFlags &=
        ~ACTOR_PRIORITY_AUTOMATIC;
    StagedActor_AdvancePair();
    Event_MovePillar();
    Object_GetById(ACTOR_PARTY_LEADER)->motionFlags |= ACTOR_FOLLOWS_TERRAIN;
    Object_GetById(ACTOR_PARTY_LEADER)->priorityFlags |=
        ACTOR_PRIORITY_AUTOMATIC;
  }
  EV_END();
}

/// Walking north at CrossboneIsleStackedPillars's ladder: climb up.
static void Event_ClimbUp(void) { Field_ClimbUp(); }

/// Walking south at CrossboneIsleStackedPillars's ladder: push the pillar
/// below, jump, or climb down.
static void Event_PushOrClimbDown(void) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  struct FieldPosition pos;
  struct FieldPosition *p = &pos;

  p->x = leader->x.fixed;
  p->y = leader->y.fixed;
  p->z = leader->z.fixed + PIXELS(16);
  if (StagedActor_FindAtTile(p, leader) != NULL) {
    Event_JumpOrPushStackedPillar();
  } else if (CrossboneIsleSteppingStones_TryJumpAcrossPillar() == 0) {
    Field_ClimbDown();
  }
}

/// Stepping on trigger 40 in CrossboneIsleStackedPillars: the fade overlay
/// follows the leader, and the leader's rise counter starts.
static void Event_StartRising(void) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  ((struct FadeOverlayWork *)WORK_SLOT(36))->actor = leader;
  leader->riseCounter = 1;
}

/// Stepping on trigger 41 in CrossboneIsleStackedPillars: the overlay lets the
/// leader go, and the rise counter stops.
static void Event_StopRising(void) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);

  ((struct FadeOverlayWork *)WORK_SLOT(36))->actor = NULL;
  leader->riseCounter = 0;
}

// UB: calling this s32 function through a void update hook is incompatible.
// Plain void changes 54 to 52 bytes in tbs-en.
#ifdef UBFIX
/// Keeps the frozen puddle's underfoot flag consistent with the leader's height
/// and depth.
static void CrossboneIsleSteppingStones_UpdateFrozenPuddlePriority(
    union FieldObject *fieldObject) {
#else
/// Puts the frozen puddle under the leader when it is behind and below the
/// leader.
static s32 CrossboneIsleSteppingStones_UpdateFrozenPuddlePriority(
    union FieldObject *fieldObject) {
#endif
  struct FieldActor *actor = (struct FieldActor *)fieldObject;
  struct FieldActor *ref = Object_GetById(ACTOR_PARTY_LEADER);

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

/// Remembers the frozen puddle, installs its priority callback, and starts its
/// flicker script.
static void CrossboneIsleSteppingStones_FreezePuddle(s32 id) {
  struct FieldActor *actor;

  actor = Object_GetById(id);
  EV_BEGIN();
#ifdef UBFIX
  actor->update = CrossboneIsleSteppingStones_UpdateFrozenPuddlePriority;
#else
  actor->update = (void (*)(union FieldObject *))
      CrossboneIsleSteppingStones_UpdateFrozenPuddlePriority;
#endif
  Map_CopyCellAttributes(20, 14, 1, 1, actor->x.fixed >> 20,
                         actor->z.fixed >> 20);
  FLAG_SET(FIRST_FROZEN_PUDDLE_FLAG + id - ACTOR_FIRST_PUDDLE);
  Actor_SetActionScript(id, gCrossboneIsleSteppingStones_ActionTable);
  EV_END();
}

/// Frost on the first puddle in CrossboneIsleDragonStatue: it freezes into a
/// pillar of ice.
static void Event_FreezeFirstPuddle(void) {
  CrossboneIsleSteppingStones_FreezePuddle(ACTOR_FIRST_PUDDLE);
}

/// Frost on the second puddle in CrossboneIsleDragonStatue.
static void Event_FreezeSecondPuddle(void) {
  CrossboneIsleSteppingStones_FreezePuddle(ACTOR_SECOND_PUDDLE);
}

/// Frost begins on the third puddle in CrossboneIsleDragonStatue: note whether
/// the leader stands on it.
static void Event_FrostBeginsOnThirdPuddle(void) {
  struct FieldActor *leader;
  struct FieldActor *puddle;
  u8 *effects;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  puddle = Object_GetById(ACTOR_THIRD_PUDDLE);
  effects = gEffectWork;
  if (puddle->x.fixed >> 20 == leader->x.fixed >> 20 &&
      puddle->z.fixed >> 20 == leader->z.fixed >> 20) {
    Engine_GameFlagSet(LEADER_ON_THIRD_PUDDLE_FLAG);
    effects[53] = 1;
  } else {
    Engine_GameFlagClear(LEADER_ON_THIRD_PUDDLE_FLAG);
  }
}

/// Later in the same Frost in CrossboneIsleDragonStatue: the third puddle
/// shrinks back, and the pillar in it (ACTOR_RISING_PILLAR) rises 32 pixels.
static void Event_FrostRaisesPillar(void) {
  struct FieldActor *pillar;

  pillar = Object_GetById(ACTOR_RISING_PILLAR);
  Object_GetById(ACTOR_THIRD_PUDDLE)->scaleX = Q16_ONE;
  Object_GetById(ACTOR_THIRD_PUDDLE)->scaleY = Q16_ONE;
  (Object_GetById(ACTOR_THIRD_PUDDLE)->sprite)->image.priority = 2;
  pillar->sprite->image.priority = 2;
  pillar->acceleration = PIXELS(2) / 5;
  pillar->speed = PIXELS(4) / 5;
  FieldObject_SetPosition(pillar, pillar->x.fixed, PIXELS(32), pillar->z.fixed);
  Engine_ActorWaitForMove(ACTOR_RISING_PILLAR);
  Map_CopyCellAttributes(20, 14, 1, 1, 22, 16);
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
    Engine_AudioPlayCue(SE_136);

  settings = &options;
  settings->palette = 10;
  settings->startScaleX = Q16_ONE / 2;
  settings->startScaleY = Q16_ONE / 2;
  settings->targetScaleX = 8 * Q16_ONE / 5;
  settings->targetScaleY = 8 * Q16_ONE / 5;
  draw = Engine_RandomNext();
  mask = 0x0ffff000;
  mask &= draw;
  settings->rotation = mask;
  settings->update = CrossboneIsleSteppingStones_UpdateDampedParticle;

  draw = Engine_RandomNext();
  velocityX = -(((draw * 5) >> 16) * PIXELS(1) + PIXELS(6));
  velocityX /= 2;
  draw = Engine_RandomNext();
  velocityY = -(((draw * 5) >> 16) * PIXELS(1) + PIXELS(5));

  Effect_Spawn(PIXELS(324), PIXELS(48), PIXELS(228), velocityX, velocityY, 0,
               EFFECT_USE_UPDATE | EFFECT_USE_ROTATION |
                   EFFECT_USE_START_SCALE | EFFECT_SCALE_TO_TARGET |
                   EFFECT_USE_PALETTE,
               settings);
}

// UB: calling this s32 function through a void update hook is incompatible.
// Plain void changes 172 to 168 bytes in tbs-en.
#ifdef UBFIX
/// Emits a randomized particle from the thawing puddle every eight frames.
static void
CrossboneIsleSteppingStones_SpawnThawParticle(union FieldObject *fieldObject) {
#else
/// Emits a randomized particle from the thawing puddle every eight frames.
static s32
CrossboneIsleSteppingStones_SpawnThawParticle(union FieldObject *fieldObject) {
#endif
  struct FieldActor *actor = (struct FieldActor *)fieldObject;
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

  x = actor->x.fixed + (((((u32)Engine_RandomNext() * 17) >> 16) - 8) << 16);
  y = actor->y.fixed + ((((u32)Engine_RandomNext() * 17) >> 16) << 16);
  z = actor->z.fixed + (((((u32)Engine_RandomNext() * 17) >> 16) - 8) << 16);
  scale = Math_Divide(
      (((u32)Engine_RandomNext() * 5) >> 16) * PIXELS(1) + PIXELS(3), 10);

  Effect_Spawn(x, y, z, 0, scale, phase,
               EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE | 1, settings);
#ifdef UBFIX
  return;
#else
  return 0;
#endif
}

/// Frost ends on the third puddle in CrossboneIsleDragonStatue: unless the
/// leader stood on it, it freezes, and a frozen second puddle thaws back to
/// water.
static void Event_FreezeThirdPuddle(void) {
  s32 set;

  set = Engine_GameFlagIsSet(LEADER_ON_THIRD_PUDDLE_FLAG);
  if (set == 0) {
    Engine_GameFlagSet(FIRST_FROZEN_PUDDLE_FLAG + 2);
    EV_BEGIN();
    Engine_CameraSetSpeed(PIXELS(3) / 5, PIXELS(3) / 40);
    Engine_CameraMoveTo(PIXELS(312), -1, PIXELS(184), 1);
    Engine_CameraWaitForMove();
    WAIT(20);
    Map_CopyCellsTo(73, 10, 60, 10, 1, 2);
    WAIT(20);
    // The third puddle freezes while the previously frozen second puddle thaws.
    FieldScheduler_AddCallback(CrossboneIsleSteppingStones_SpawnFrostParticle,
                               0xc80);
    WAIT(40);
    if (FLAG_CHECK(FIRST_FROZEN_PUDDLE_FLAG + 1) != 0) {
#ifdef UBFIX
      Object_GetById(ACTOR_SECOND_PUDDLE)->update =
          CrossboneIsleSteppingStones_SpawnThawParticle;
#else
      Object_GetById(ACTOR_SECOND_PUDDLE)->update = (void (*)(
          union FieldObject *))CrossboneIsleSteppingStones_SpawnThawParticle;
#endif
      OBJ_MODE(ACTOR_SECOND_PUDDLE, 6);
      ObjectMotion_WaitForAnimationChange(ACTOR_SECOND_PUDDLE);
      Object_GetById(ACTOR_SECOND_PUDDLE)->update =
          (void (*)(union FieldObject *))set;
      Map_CopyCellAttributes(17, 13, 1, 1, 18, 13);
      Engine_GameFlagClear(FIRST_FROZEN_PUDDLE_FLAG + 1);
      Engine_ActorSetChildValue(ACTOR_SECOND_PUDDLE, 0);
      Actor_SetBuiltinAction(ACTOR_SECOND_PUDDLE, ACTOR_ACTION_IDLE);
    } else {
      WAIT(60);
    }
    Engine_TaskRemoveCallback(CrossboneIsleSteppingStones_SpawnFrostParticle);
    WAIT(20);
    Map_CopyCellsTo(72, 10, 60, 10, 1, 2);
    WAIT(20);
    EV_END();
  }
}

/// Move begins on CrossboneIsleDragonStatue's sinking boulder: its cell is
/// cleared.
static void Event_StartMovingSinkingPillar(void) {
  struct FieldActor *pillar = Object_GetById(ACTOR_SINKING_BOULDER);
  s32 x;
  s32 z;

  EV_BEGIN();

  x = pillar->x.fixed >> 20;
  z = pillar->z.fixed >> 20;

  StagedActor_FillGridAttributeRectangle(2, x, z, 1, 1, 0);
  EV_END();
}

/// Move on CrossboneIsleDragonStatue's sinking boulder, and after each push:
/// once it reaches column 16 it sinks two pixels into place for good.
static void Event_MoveSinkingPillar(void) {
  struct FieldActor *pillar;
  s32 alreadyLowered;

  pillar = Object_GetById(ACTOR_SINKING_BOULDER);
  EV_BEGIN();
  StagedActor_FillGridAttributeRectangle(2, (pillar->x.fixed >> 20),
                                         (pillar->z.fixed >> 20), 1, 1, 255);
  if ((pillar->x.fixed >> 20) == 16) {
    alreadyLowered = FLAG_CHECK(PILLAR_SUNK_FLAG);
    if (alreadyLowered == 0) {
      WAIT(10);
      Engine_AudioPlayCue(SE_159);
      pillar->motionFlags = alreadyLowered;
      pillar->terrainHeight = -PIXELS(2);
      pillar->y.fixed = -PIXELS(2);
      Engine_GameFlagSet(PILLAR_SUNK_FLAG);
    }
  }
  EV_END();
}

/// Walking into CrossboneIsleDragonStatue's sinking boulder: push it if the
/// cell beyond is clear.
static void Event_PushSinkingPillar(void) {
  struct StagedActorProbe probe;

  EV_BEGIN();
  if (StagedActor_FindClearPosition(&probe) != 0) {
    SceneActor_MoveAndRedraw(probe);
  } else {
    Event_StartMovingSinkingPillar();
    StagedActor_AdvancePair();
    Event_MoveSinkingPillar();
  }
  EV_END();
}

const struct SceneEvent *Scene_GetEvents(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_CrossboneIsleSteppingStones) {
    return gCrossboneIsleSteppingStonesEvents;
  }
  if (scene == (s32)&SceneId_CrossboneIsleIcePillars) {
    return gCrossboneIsleIcePillarsEvents;
  }
  if (scene == (s32)&SceneId_CrossboneIsleFloodgate) {
    return gCrossboneIsleFloodgateEvents;
  }
  if (scene == (s32)&SceneId_CrossboneIsleStackedPillars) {
    return gCrossboneIsleStackedPillarsEvents;
  }
  if (scene == (s32)&SceneId_CrossboneIsleDragonStatue) {
    return gCrossboneIsleDragonStatueEvents;
  }
  return gCrossboneIsleSteppingStonesEventsOther;
}

/// Prepares the lowered actor's collision and draw priority below the leader.
static void CrossboneIsleSteppingStones_PrepLoweredActor(s32 id) {
  struct FieldActor *actor = Object_GetById(id);

  actor->collisionFlags &= 254;
  actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  actor->motionFlags = 0;
  Engine_ActorSetSpriteFlags(actor, 0);
  actor->sprite->image.priority = 2;
}

/// Restores a previously frozen pillar's mode, map cell, priority hook, and
/// script.
static void CrossboneIsleSteppingStones_RestoreFlaggedPillar(s32 actorId) {
  struct FieldActor *actor;

  actor = Object_GetById(actorId);
  if (FLAG_CHECK(FIRST_FROZEN_PUDDLE_FLAG + actorId - ACTOR_FIRST_PUDDLE) !=
      0) {
    Object_SetMode(actor, 5);
#ifdef UBFIX
    actor->update = CrossboneIsleSteppingStones_UpdateFrozenPuddlePriority;
#else
    actor->update = (void (*)(union FieldObject *))
        CrossboneIsleSteppingStones_UpdateFrozenPuddlePriority;
#endif
    Map_CopyCellAttributes(20, 14, 1, 1, actor->x.fixed >> 20,
                           actor->z.fixed >> 20);
    Actor_SetActionScript(actorId, gCrossboneIsleSteppingStones_ActionTable);
  }
}

/// Uses terrain layer 2 and installs the moving pillar's priority callback.
static void CrossboneIsleSteppingStones_InitializePillarPriority(s32 id) {
  struct FieldActor *actor = Object_GetById(id);

  actor->terrainId = 2;
  actor->motionFlags = 0;
#ifdef UBFIX
  actor->update = CrossboneIsleSteppingStones_UpdateActorPriority;
#else
  actor->update = (void (*)(
      union FieldObject *))CrossboneIsleSteppingStones_UpdateActorPriority;
#endif
}

/// Matches the leader's draw priority to a nearby pillar in front.
static void CrossboneIsleSteppingStones_UpdateLeaderPriority(void) {
  struct FieldActor *hero = Object_GetById(ACTOR_PARTY_LEADER);
  u32 selector;
  struct FieldActor *actor;

  for (selector = ACTOR_FIRST_PILLAR; selector <= ACTOR_FIRST_PILLAR + 3;
       selector++) {
    actor = Object_GetById(selector);
    if (hero->y.fixed / PIXELS(1) == actor->y.fixed / PIXELS(1) &&
        hero->z.fixed <= actor->z.fixed - PIXELS(8) &&
        hero->z.fixed > actor->z.fixed - PIXELS(24)) {
      if (hero->x.fixed - PIXELS(16) <= actor->x.fixed &&
          actor->x.fixed < hero->x.fixed + PIXELS(16)) {
        Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER,
                                      actor->sprite->image.priority);
        return;
      }
    } else {
      Object_GetById(ACTOR_PARTY_LEADER)->priorityFlags |=
          ACTOR_PRIORITY_AUTOMATIC;
    }
  }
}

/// Restores each room's pillar footprints, water state, and persistent puzzle
/// state.
s32 Scene_Initialize(void) {
  struct FieldActor *actor;
  u32 i;
  u32 id;
  s32 x;
  s32 z;

  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  if (gGameState.scene == (s32)&SceneId_CrossboneIsleIcePillars) {
    FieldScene_RedrawActorFootprint(ACTOR_FIRST_PILLAR);
    FieldScene_RedrawActorFootprint(ACTOR_FIRST_PILLAR + 1);
    FieldScene_RedrawActorFootprint(ACTOR_FIRST_PILLAR + 2);
    FieldScene_RedrawActorFootprint(ACTOR_FIRST_PILLAR + 3);
    FieldScene_RedrawActorFootprint(ACTOR_FIRST_PILLAR + 4);
  } else if (gGameState.scene == (s32)&SceneId_CrossboneIsleFloodgate) {
    // Rebuild the flooded room and float the logs again when the water stayed
    // raised through a battle.
    Map_CopyCells(32, 0, 64, 32, 0, 64);
    FieldScene_RedrawActorFootprint(ACTOR_FIRST_LOG);
    FieldScene_RedrawActorFootprint(ACTOR_FIRST_LOG + 1);
    FieldScene_RedrawActorFootprint(ACTOR_FIRST_LOG + 2);
    FieldScene_RedrawActorFootprint(ACTOR_FIRST_LOG + 3);
    FieldScene_RedrawActorFootprint(ACTOR_FIRST_LOG + 4);
    FieldScene_RedrawActorFootprint(ACTOR_FIRST_LOG + 5);
    FieldScene_RedrawActorFootprint(ACTOR_FIRST_LOG + 6);
    FieldScene_RedrawActorFootprint(ACTOR_FIRST_LOG + 7);
    if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) != 0 &&
        Engine_GameFlagIsSet(WATER_RAISED_FLAG) != 0) {
      Map_CopyCellsTo(79, 34, 84, 24, 1, 2);
      Map_CopyCellsTo(0, 32, 32, 0, 32, 32);
      Map_CopyCellsTo(32, 32, 64, 0, 32, 32);
      CrossboneIsleSteppingStones_DrawFloatingLogs();
      Map_CopyCellAttributes(24, 3, 1, 1, 24, 8);
    }
  } else if (gGameState.scene == (s32)&SceneId_CrossboneIsleStackedPillars) {
    // Restore the four movable pillars and the leader's riding overlay.
    OverlayObject_CreateConfiguredObject(PIXELS(584), 0, PIXELS(200), 223);
    if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
      Object_GetById(ACTOR_PARTY_LEADER)->riseCounter = 1;
    }
    BattleFx_StartFadeOverlay(0);
    if (Object_GetById(ACTOR_PARTY_LEADER)->riseCounter == 0) {
      Event_StopRising();
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
        entry[4] = i + FLAG_SCENE_TEMP_00;
        entry += 5;
      }
    }
    Event_MovePillar();
    WAIT(1);
    FieldScheduler_AddCallback(CrossboneIsleSteppingStones_UpdateLeaderPriority,
                               TASK_PRIORITY_SCENE);
    if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) != 0) {
      for (id = ACTOR_FIRST_PILLAR; id < ACTOR_FIRST_PILLAR + 4; id++) {
        actor = Object_GetById(id);
        x = actor->x.fixed >> 20;
        if (x == 37) {
          z = actor->z.fixed >> 20;
          if (z == 9) {
            Map_CopyCellAttributes(27, 8, 1, 1, x, z);
            break;
          }
        }
      }
    }
  } else if (gGameState.scene == (s32)&SceneId_CrossboneIsleDragonStatue) {
    // Restore CrossboneIsleDragonStatue's moved pillar, frozen puddles, and
    // raised pillar pair.
    OBJ_MODE(ACTOR_SINKING_BOULDER, 2);
    Engine_ActorSetChildValue(ACTOR_SINKING_BOULDER, 6);
    FieldScene_RedrawActorFootprint(ACTOR_FIRST_LOG);
    FieldScene_RedrawActorFootprint(ACTOR_FIRST_LOG + 1);
#if TBS_PATCH_1
    Object_GetById(ACTOR_FIRST_LOG)->motionFlags = 0;
    Object_GetById(ACTOR_FIRST_LOG + 1)->motionFlags = 0;
#endif
    Event_MoveSinkingPillar();
    CrossboneIsleSteppingStones_PrepLoweredActor(ACTOR_FIRST_PUDDLE);
    CrossboneIsleSteppingStones_PrepLoweredActor(ACTOR_SECOND_PUDDLE);
    CrossboneIsleSteppingStones_PrepLoweredActor(ACTOR_THIRD_PUDDLE);
    CrossboneIsleSteppingStones_RestoreFlaggedPillar(ACTOR_FIRST_PUDDLE);
    CrossboneIsleSteppingStones_RestoreFlaggedPillar(ACTOR_SECOND_PUDDLE);
    CrossboneIsleSteppingStones_RestoreFlaggedPillar(ACTOR_THIRD_PUDDLE);
    actor = Object_GetById(ACTOR_THIRD_PUDDLE);
    actor->update = 0;
    CrossboneIsleSteppingStones_PrepLoweredActor(ACTOR_RISING_PILLAR);
    Object_GetById(ACTOR_RISING_PILLAR)->collisionFlags |= 8;
    if (Engine_GameFlagIsSet(FIRST_FROZEN_PUDDLE_FLAG + 2) == 0) {
      Object_GetById(ACTOR_THIRD_PUDDLE)->scaleX = 3 * Q16_ONE / 2;
      Object_GetById(ACTOR_THIRD_PUDDLE)->scaleY = 3 * Q16_ONE / 2;
      Object_GetById(ACTOR_THIRD_PUDDLE)->sprite->image.priority = 3;
      Object_GetById(ACTOR_RISING_PILLAR)->sprite->image.priority = 3;
      Map_CopyCellAttributes(26, 12, 1, 1, 22, 16);
    }
  }
  return 0;
}

s32 gEffectScript0[] = {
    SC_WAIT(1),      SC_LOOP(22, SC_TOP), SC_WAIT(1),
    SC_POS(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gEffectScript1[] = {
    SC_WAIT(1),      SC_LOOP(44, SC_TOP), SC_WAIT(1),
    SC_POS(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gEffectScript2[] = {
    SC_WAIT(1),      SC_LOOP(126, SC_TOP), SC_WAIT(1),
    SC_POS(0, 0, 0), SC_WAIT(1),           SC_FREE,
};

/// Pixel step by direction: X in the upper halfword, Z in the lower.
#define DIRECTION_STEP(dx, dz) (((dx) << 16) | ((dz) & 0xffff))

s32 gStagedActor_DirectionSteps[] = {
    DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(16, 0),
};

// clang-format off
u32 gStagedActor_FootprintKinds[] = {
    207, 205, 228, 229, 298, 297,
};

s32 gStagedActor_FootprintBounds[] = {
    -32, -8, 32, 8,
    -8, -32, 8, 32,
    -32, -16, 32, 0,
    -8, -32, 8, 32,
    -32, -8, 32, 8,
    -8, -32, 8, 32,
};

const s32 *gEffectScripts[] = {
    gEffectScript0,
    gEffectScript1,
    gEffectScript2,
};

s32 gCrossboneIsleSteppingStones_ActionTable[] = {
    SC_SET(0x23, 2),
    SC_CALL(CrossboneIsleSteppingStones_SetChildValueFromTimer),
    SC_WAIT(2),
    SC_SET(0x23, 0),
    SC_CALL(CrossboneIsleSteppingStones_SetChildValueFromTimer),
    SC_WAIT(6),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

u16 gCrossboneIsleSteppingStones_ShiftSteps1[] = {
    MAP_CELL_STEP(76, 34, 1, 2, 4), MAP_CELL_STEP(77, 34, 1, 2, 4), MAP_CELL_STEP(78, 34, 1, 2, 4), MAP_CELL_STEP(79, 34, 1, 2, 4), MAP_CELL_STEPS_END,
};

u16 gCrossboneIsleSteppingStones_ShiftSteps2[] = {
    MAP_CELL_STEP(79, 34, 1, 2, 4), MAP_CELL_STEP(78, 34, 1, 2, 4), MAP_CELL_STEP(77, 34, 1, 2, 4), MAP_CELL_STEP(76, 34, 1, 2, 4), MAP_CELL_STEPS_END,
};

u16 gCrossboneIsleSteppingStones_ShiftSteps3[] = {
    MAP_CELL_STEP(67, 34, 2, 5, 6), MAP_CELL_STEP(69, 34, 2, 5, 6), MAP_CELL_STEP(71, 34, 2, 5, 6), MAP_CELL_STEP(73, 34, 2, 5, 6), MAP_CELL_STEPS_END,
};

u16 gCrossboneIsleSteppingStones_ShiftSteps4[] = {
    MAP_CELL_STEP(71, 50, 2, 5, 6), MAP_CELL_STEP(69, 50, 2, 5, 6), MAP_CELL_STEP(67, 50, 2, 5, 6), MAP_CELL_STEP(65, 50, 2, 5, 6), MAP_CELL_STEPS_END,
};

u16 gCrossboneIsleSteppingStones_ShiftSteps5[] = {
    MAP_CELL_STEP(75, 38, 2, 1, 4), MAP_CELL_STEP(77, 38, 2, 1, 6), MAP_CELL_STEP(79, 38, 2, 1, 8), MAP_CELL_STEP(65, 53, 2, 1, 1), MAP_CELL_STEPS_END,
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

/// Unused: an empty placement table nothing returns.
struct ScenePlacement gCrossboneIsleSteppingStonesNoPlacements[] = {
    { SCENE_TABLE_END },
};

struct ScenePlacement gCrossboneIsleSteppingStonesPlacements[] = {
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(744), 0, PIXELS(488), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(792), 0, PIXELS(552), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(696), 0, PIXELS(616), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(776), 0, PIXELS(680), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gCrossboneIsleIcePillarsPlacements[] = {
    { 0x12a, CONDITION_ALWAYS, 7,
        PIXELS(144), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x12a, CONDITION_ALWAYS, 7,
        PIXELS(256), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x12a, CONDITION_ALWAYS, 7,
        PIXELS(224), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x129, CONDITION_ALWAYS, 7,
        PIXELS(280), 0, PIXELS(160), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x129, CONDITION_ALWAYS, 7,
        PIXELS(184), 0, PIXELS(240), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gCrossboneIsleFloodgatePlacements[] = {
    { SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, 7,
        PIXELS(144), 0, PIXELS(296), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7,
        PIXELS(232), 0, PIXELS(256), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, 7,
        PIXELS(208), 0, PIXELS(216), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7,
        PIXELS(232), 0, PIXELS(176), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, 7,
        PIXELS(272), 0, PIXELS(168), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, 7,
        PIXELS(352), 0, PIXELS(168), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xd6, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gCrossboneIsleSteppingStonesPlacementsOther[] = {
    { 0x100, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(648), 0, PIXELS(184), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { 0x100, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
    { 0x100, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(680), 0, PIXELS(392), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { 0x100, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gCrossboneIsleDragonStatuePlacements[] = {
    { SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, 7,
        PIXELS(208), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, 7,
        PIXELS(224), 0, PIXELS(280), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x125, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(232), 0, PIXELS(296), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(136), 0, PIXELS(216), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(296), 0, PIXELS(216), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(360), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x122, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(360), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gCrossboneIsleSteppingStonesEventsOther[] = {
    { SCENE_EVENTS_END },
};

struct SceneEvent gCrossboneIsleSteppingStonesEvents[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, CROSSBONE_ISLE_STEPPING_STONES_TO_CROSSBONE_ISLE_FIRST_FLOOR_EAST },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, CROSSBONE_ISLE_STEPPING_STONES_TO_CROSSBONE_ISLE_FIRST_FLOOR_WEST },
    { EVENT_TOUCH | EVENT_TOUCH_WALKING, 10, CONDITION_ALWAYS, EVENT_SCRIPT(Event_JumpOrPushPillar) },
    { SEARCH(SEARCH_CHEST), 100, 0xec2, FIND_ITEM(ITEM_ANTIDOTE) },
    { SEARCH(SEARCH_CHEST), 101, 0xec3, FIND_ITEM(ITEM_NUT) },
    { SEARCH(SEARCH_CHEST), 102, 0xec4, FIND_COINS(111) },
    { SEARCH(SEARCH_CHEST), 103, 0xec5, FIND_ITEM(ITEM_HARD_NUT) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_PILLAR, CONDITION_ALWAYS, 0 },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_PILLAR + 1, CONDITION_ALWAYS, 0 },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_PILLAR + 2, CONDITION_ALWAYS, 0 },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_PILLAR + 3, CONDITION_ALWAYS, 0 },
    { SCENE_EVENTS_END },
};

struct SceneEvent gCrossboneIsleIcePillarsEvents[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, CROSSBONE_ISLE_ICE_PILLARS_TO_CROSSBONE_ISLE_SECOND_FLOOR_EAST },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, CROSSBONE_ISLE_ICE_PILLARS_TO_CROSSBONE_ISLE_SECOND_FLOOR_WEST },
    { EVENT_TOUCH | EVENT_TOUCH_WALKING, 10, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushPillar) },
    { SEARCH(SEARCH_CHEST), 100, 0xec6, FIND_ITEM(ITEM_LUCKY_PEPPER) },
    { SEARCH(SEARCH_CHEST), 101, 0xec7, FIND_ITEM(ITEM_MYSTERY_BLADE) },
    { SEARCH(SEARCH_CHEST), 102, 0xec8, FIND_COINS(222) },
    { SEARCH(SEARCH_CHEST), 103, 0xec9, FIND_ITEM(ITEM_ELIXIR) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gCrossboneIsleFloodgateEvents[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, CROSSBONE_ISLE_FLOODGATE_TO_CROSSBONE_ISLE_FIFTH_FLOOR_EAST },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, CROSSBONE_ISLE_FLOODGATE_TO_CROSSBONE_ISLE_FIFTH_FLOOR_WEST },
    { EVENT_TOUCH | EVENT_TOUCH_WALKING, 10, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushPillar) },
    { 0x413, 100, 0xed2, FIND_COINS(555) },
    { 0xc413, 100, 0xed2, FIND_COINS(555) },
    { 0x413, 101, 0xed3, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { 0xe413, 101, 0xed3, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { SEARCH(SEARCH_CHEST), 102, 0xed4, FIND_ITEM(ITEM_POTION) },
    { SEARCH(SEARCH_CHEST), 103, 0xed5, FIND_ITEM(ITEM_NINJA_GARB) },
    { SEARCH(SEARCH_UNNAMED), 20, CONDITION_ALWAYS, EVENT_SCRIPT(Event_FlipWaterSwitch) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gCrossboneIsleStackedPillarsEvents[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, CROSSBONE_ISLE_STACKED_PILLARS_TO_CROSSBONE_ISLE_SEVENTH_FLOOR_EAST },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, CROSSBONE_ISLE_STACKED_PILLARS_TO_CROSSBONE_ISLE_SEVENTH_FLOOR_WEST },
    { EVENT_TOUCH | EVENT_TOUCH_WALKING, 30, CONDITION_ALWAYS, EVENT_SCRIPT(Event_JumpOrPushStackedPillar) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 31, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ClimbUp) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 31, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushOrClimbDown) },
    { EVENT_TOUCH | EVENT_TOUCH_WALKING, 31, CONDITION_ALWAYS, EVENT_SCRIPT(Event_JumpOrPushStackedPillar) },
    { EVENT_TOUCH, 40, CONDITION_ALWAYS, EVENT_SCRIPT(Event_StartRising) },
    { EVENT_TOUCH, 41, CONDITION_ALWAYS, EVENT_SCRIPT(Event_StopRising) },
    { SEARCH(SEARCH_CHEST), 100, 0xeda, FIND_COINS(777) },
    { SEARCH(SEARCH_CHEST), 101, 0xedb, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { 0x413, 102, 0xedc, FIND_ITEM(ITEM_PSY_CRYSTAL) },
    { SEARCH(SEARCH_CHEST), 103, 0xedd, FIND_ITEM(ITEM_WICKED_MACE) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_PILLAR, FLAG_SCENE_TEMP_00, EVENT_SCRIPT(Event_MovePillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_PILLAR + 1, FLAG_SCENE_TEMP_01, EVENT_SCRIPT(Event_MovePillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_PILLAR + 2, FLAG_SCENE_TEMP_02, EVENT_SCRIPT(Event_MovePillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_PILLAR + 3, FLAG_SCENE_TEMP_03, EVENT_SCRIPT(Event_MovePillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_FIRST_PILLAR, FLAG_SCENE_TEMP_00, EVENT_SCRIPT(Event_MovePillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_FIRST_PILLAR + 1, FLAG_SCENE_TEMP_01, EVENT_SCRIPT(Event_MovePillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_FIRST_PILLAR + 2, FLAG_SCENE_TEMP_02, EVENT_SCRIPT(Event_MovePillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_FIRST_PILLAR + 3, FLAG_SCENE_TEMP_03, EVENT_SCRIPT(Event_MovePillar) },
    { SCENE_EVENTS_END },
};
// clang-format on

struct SceneEvent gCrossboneIsleDragonStatueEvents[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS,
     CROSSBONE_ISLE_DRAGON_STATUE_TO_CROSSBONE_ISLE_EIGHTH_FLOOR_EAST},
    {EVENT_EXIT, 2, CONDITION_ALWAYS,
     CROSSBONE_ISLE_DRAGON_STATUE_TO_CROSSBONE_ISLE_EIGHTH_FLOOR_WEST},
    {EVENT_TOUCH | EVENT_TOUCH_WALKING, 10, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_PushSinkingPillar)},
    {SEARCH(SEARCH_CHEST), 100, 0xede, FIND_COINS(888)},
    {SEARCH(SEARCH_CHEST), 101, 0xedf, FIND_ITEM(ITEM_POTION)},
    {SEARCH(SEARCH_CHEST), 102, 0xee0, FIND_ITEM(ITEM_CLERICS_RING)},
    {SEARCH(SEARCH_CHEST), 103, 0xee1, FIND_ITEM(ITEM_SMOKE_BOMB)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_SINKING_BOULDER, PILLAR_SUNK_FLAG,
     EVENT_SCRIPT(Event_StartMovingSinkingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SINKING_BOULDER, PILLAR_SUNK_FLAG,
     EVENT_SCRIPT(Event_MoveSinkingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_FIRST_PUDDLE,
     FIRST_FROZEN_PUDDLE_FLAG, EVENT_SCRIPT(Event_FreezeFirstPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SECOND_PUDDLE,
     FIRST_FROZEN_PUDDLE_FLAG + 1, EVENT_SCRIPT(Event_FreezeSecondPuddle)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_FROST, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_THIRD_PUDDLE, FIRST_FROZEN_PUDDLE_FLAG + 2,
     EVENT_SCRIPT(Event_FrostBeginsOnThirdPuddle)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_FROST, PSYNERGY_PHASE_EFFECT_ACTS),
     ACTOR_THIRD_PUDDLE, FIRST_FROZEN_PUDDLE_FLAG + 2,
     EVENT_SCRIPT(Event_FrostRaisesPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_THIRD_PUDDLE,
     FIRST_FROZEN_PUDDLE_FLAG + 2, EVENT_SCRIPT(Event_FreezeThirdPuddle)},
    {SCENE_EVENTS_END},
};
