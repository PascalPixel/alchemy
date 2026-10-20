/// @file
/// Crossbone Isle's pillar rooms: pillars to push, jump across, Move and Carry,
/// the switch that raises the water in room 3, and the pools Frost freezes in
/// room 5.
#include "script.h"
#include "game_state.h"
#include "map_scroll.h"
#include "scene_ids.h"
#include "unit/import.h"
#include "unit/scene/crossbone_isle_pillar_rooms.h"
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

#define WATER_ACTOR 16
/// Room 3: the water has been raised.
#define WATER_RAISED_FLAG FLAG_SCENE_TEMP_00
// Room 4 keeps FLAG_SCENE_TEMP_00 to 03 for its four pillars' footprints.
// Room 5: FLAG_SCENE_TEMP_00 to 02 the three pools are frozen, 03 the leader
// stood on the third when Frost began, 04 the sinking pillar has sunk.
#define FIRST_FROZEN_POOL_FLAG FLAG_SCENE_TEMP_00
#define LEADER_ON_THIRD_POOL_FLAG FLAG_SCENE_TEMP_03
#define PILLAR_SUNK_FLAG FLAG_SCENE_TEMP_04

/// One map cell: its tile, layer, kind, height and event.
struct MapCellFields {
  u32 tile : 12;
  u32 layer : 2;
  u8 kind : 2;
  u8 height;
  u8 event;
};

#include "common/effect_create.h"
#include "common/effect_spawn.h"
#include "common/object_staged_actor.h"

#include "common/map_panel.h"

/// Waits up to sixty frames for landing, then stops vertical movement on the
/// ground.
static void
CrossboneIslePillarRooms_SettleActorOnGround(struct FieldActor *actor) {
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
CrossboneIslePillarRooms_StopAtGroundOrHeight(struct FieldActor *actor,
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
static s32
CrossboneIslePillarRooms_QueryFootprint(s32 id, u32 *width, u32 *depth,
                                        struct StagedActorProbe *probe,
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
  probe->positionX = actor->x.fixed;
  probe->positionY = actor->y.fixed;
  probe->positionZ = actor->z.fixed;
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
  probe->positionX +=
      gStagedActor_FootprintBounds[probe->footprintIndex * 4] * PIXELS(1);
  {
    s32 offsetZ =
        gStagedActor_FootprintBounds[probe->footprintIndex * 4 + 1] * PIXELS(1);

    coordinate = probe->positionZ;
    coordinate += offsetZ;
    coordinate >>= 20;
    probe->positionZ = coordinate;
  }
  probe->positionX >>= 20;
  *layerOriginX = map->layers[1].offsetX >> 20;
  *layerOriginZ = map->layers[1].offsetY >> 20;
  return 1;
}

/// Draws the raised pillar's footprint and puts its actor under the leader.
static s32 CrossboneIslePillarRooms_DrawRaisedFootprint(s32 actorId) {
  struct FieldActor *actor = Object_GetById(actorId);
  u32 width, depth;
  s32 layerOriginX, layerOriginZ;
  struct StagedActorProbe probe;
  s32 cellX, cellZ;
  u8 mask;

  if (CrossboneIslePillarRooms_QueryFootprint(
          actorId, &width, &depth, &probe, &layerOriginX, &layerOriginZ) == 0) {
    return 0;
  }

  cellX = probe.positionX;
  cellZ = probe.positionZ;
  Map_CopyCellAttributes(2, 2, width, depth, cellX, cellZ);

  Object_SetMode(actor, 4);
  mask = 2;
  actor->priorityFlags |= mask;

  if (width > depth) {
    Map_CopyCellsTo(70, 40, probe.positionX + 32, probe.positionZ + 2, width,
                    depth);
  } else {
    Map_CopyCellsTo(68, 40, probe.positionX + 32, probe.positionZ + 2, width,
                    depth);
  }

  return 1;
}

/// Redraws all seven pillars at the raised water level.
static inline void CrossboneIslePillarRooms_DrawRaisedPillars(void) {
  CrossboneIslePillarRooms_DrawRaisedFootprint(9);
  CrossboneIslePillarRooms_DrawRaisedFootprint(10);
  CrossboneIslePillarRooms_DrawRaisedFootprint(11);
  CrossboneIslePillarRooms_DrawRaisedFootprint(12);
  CrossboneIslePillarRooms_DrawRaisedFootprint(13);
  CrossboneIslePillarRooms_DrawRaisedFootprint(14);
  CrossboneIslePillarRooms_DrawRaisedFootprint(15);
}

/// Restores the ground tiles and collision attributes under the actor's
/// footprint.
static s32 CrossboneIslePillarRooms_RestoreGroundFootprint(s32 actorId) {
  struct FieldActor *actor = Object_GetById(actorId);
  u32 width, depth;
  s32 layerOriginX, layerOriginZ;
  struct StagedActorProbe probe;

  if (CrossboneIslePillarRooms_QueryFootprint(
          actorId, &width, &depth, &probe, &layerOriginX, &layerOriginZ) == 0) {
    return 0;
  }

  {
    s32 sourceCellX = layerOriginX + probe.positionX;
    s32 sourceCellZ = layerOriginZ + probe.positionZ;

    Map_CopyCellAttributes(sourceCellX, sourceCellZ, width, depth,
                           probe.positionX, probe.positionZ);
    StagedActor_FillGridAttributeRectangle(0, probe.positionX, probe.positionZ,
                                           width, depth, 255);
  }

  Object_SetMode(actor, 1);
  actor->priorityFlags &= ~ACTOR_PRIORITY_UNDERFOOT;

  return 1;
}

/// Reads one map cell through DMA, waiting until the transfer finishes.
static void CrossboneIslePillarRooms_ReadMapCell(s32 layer, s32 x, s32 y,
                                                 struct MapCellFields *dst) {
  struct MapState *map = gMapWork[0];

  if (map != 0) {
    u32 *cell = (u32 *)map->layers[layer].cells;

    cell += x + (y << 7);
    Dma_Set(cell, dst, 0x84000001, REG_DMA3);
    {
      volatile u32 *dma = REG_DMA3;

      while (dma[2] & 0x80000000)
        ;
    }
  }
}

/// Copies the cell's layer, kind, height, and event without replacing its tile.
static void CrossboneIslePillarRooms_SetCellAttributes(s32 layer, s32 x, s32 y,
                                                       struct MapCell *source) {
  struct MapCellFields *src = (struct MapCellFields *)source;
  struct MapState *map = gMapWork[0];

  if (map != 0) {
    struct MapCellFields *cell =
        (struct MapCellFields *)map->layers[layer].cells;

    cell += x + (y << 7);

    cell->layer = src->layer;
    cell->kind = src->kind;
    cell->height = src->height;
    cell->event = src->event;
  }
}

/// Finds the next terrain ledge and stops the falling actor at its projected
/// height.
static s32 CrossboneIslePillarRooms_LowerActorToLedge(s32 id, s32 far) {
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
      /* FAKEMATCH: a one-pass block around the object and height issues the
       * height arithmetic first; as no block, 2 instructions differ in
       * tbs-en. */
      do {
        temporaryActor = OverlayObject_PrepareObject(x, y, z, kind);
        height = actor->z.fixed - z + y;
      } while (0);
      found = 1;
      break;
    }
  }
  CrossboneIslePillarRooms_StopAtGroundOrHeight(actor, height);
  actor->x.fixed = 0;
  actor->y.fixed = 0;
  actor->z.fixed = 0;
  if (temporaryActor != 0) {
    Engine_ObjectDispatchRelease(temporaryActor);
  }
  return found;
}

/// Shrinks the pillar and lowers its height for up to thirty-two frames.
static void CrossboneIslePillarRooms_ShrinkPillar(s32 index) {
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

/// Spawns the unused particle near the leader once every four frames.
UNUSED static void CrossboneIslePillarRooms_SpawnUnusedLeaderParticle(void) {

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
static s32 CrossboneIslePillarRooms_TryJumpAcrossPillar(void) {
  struct FieldActor *leader;
  u8 *flags;
  s32 savedMotionFlags;
  s32 jumpDirection;
#if !TBS_PATCH_1
  s32 step;
#endif
  s32 *position;
  s32 destination[3];

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  flags = &leader->motionFlags;
  savedMotionFlags = *flags;
#if TBS_PATCH_1
  jumpDirection = (leader->facing + 0x2000) & 0xc000;
#endif
  if (gGameState.movementMode != 0) {
    return 0;
  }
  position = destination;
#if TBS_PATCH_1
  SET_VECTOR(position, (leader->x.fixed & -0x100000) + PIXELS(8),
             leader->y.fixed, (leader->z.fixed & -0x100000) + PIXELS(8));
  Vector_AddPolarOffset(PIXELS(16), jumpDirection, position);
#else
  jumpDirection = leader->facing >> 12;
  step = gStagedActor_DirectionSteps[jumpDirection];
  position[0] = leader->x.fixed + (step & 0xffff0000);
  position[1] = leader->y.fixed;
  step <<= 16;
  position[2] = leader->z.fixed + step;
#endif
  // Require a free intervening cell before testing the two-cell landing.
  if (FieldObject_CheckMovementCollision(leader, position) != 1 &&
      StagedActor_FindAtTile(position, (struct StagedActor *)leader) == 0) {
#if TBS_PATCH_1
    SET_VECTOR(position, (leader->x.fixed & -0x100000) + PIXELS(8),
               leader->y.fixed, (leader->z.fixed & -0x100000) + PIXELS(8));
    Vector_AddPolarOffset(PIXELS(32), jumpDirection, position);
#else
    step = gStagedActor_DirectionSteps[jumpDirection];
    position[0] = leader->x.fixed + ((step & 0xffff0000) * 2);
    position[1] = leader->y.fixed;
    step <<= 17;
    position[2] = leader->z.fixed + step;
#endif
    if (StagedActor_FindAtTile(position, (struct StagedActor *)leader) == 0 &&
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
      Engine_ActorMoveToAndWait(ACTOR_PARTY_LEADER, ((s16 *)position)[1],
                                ((s16 *)position)[5]);
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
CrossboneIslePillarRooms_UpdateDampedParticle(union FieldObject *object) {
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
static s32 CrossboneIslePillarRooms_SetChildValueFromTimer(
    struct ScriptInterpreter *interpreter) {
  struct FieldActor *actor = (struct FieldActor *)interpreter;
  ObjectGroup_SetChildValue(actor, actor->timer & 15);
  return 0;
}

const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_CrossboneIslePillarRooms1) {
    return gCrossboneIslePillarRoomsEntrances1;
  }
  if (scene == (s32)&SceneId_CrossboneIslePillarRooms2) {
    return gCrossboneIslePillarRoomsEntrances2;
  }
  if (scene == (s32)&SceneId_CrossboneIslePillarRooms3) {
    return gCrossboneIslePillarRoomsEntrances3;
  }
  if (scene == (s32)&SceneId_CrossboneIslePillarRooms4) {
    return gCrossboneIslePillarRoomsEntrances4;
  }
  if (scene == (s32)&SceneId_CrossboneIslePillarRooms5) {
    return gCrossboneIslePillarRoomsEntrances5;
  }
  return gCrossboneIslePillarRoomsEntrancesOther;
}

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

/// Returns the shared exits for the pillar rooms.
const u32 *Scene_GetExits(void) { return gCrossboneIslePillarRoomsExits; }

const struct ScenePlacement *Scene_GetPlacements(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_CrossboneIslePillarRooms1) {
    return gCrossboneIslePillarRoomsPlacements1;
  }
  if (scene == (s32)&SceneId_CrossboneIslePillarRooms2) {
    return gCrossboneIslePillarRoomsPlacements2;
  }
  if (scene == (s32)&SceneId_CrossboneIslePillarRooms3) {
    return gCrossboneIslePillarRoomsPlacements3;
  }
  if (scene == (s32)&SceneId_CrossboneIslePillarRooms5) {
    return gCrossboneIslePillarRoomsPlacements5;
  }
  return gCrossboneIslePillarRoomsPlacementsOther;
}

/// Walking into a pillar in room 1: jump two cells across it where the cells
/// allow, otherwise push it.
static void Event_JumpOrPushPillar(void) {
  EV_BEGIN();
  if (CrossboneIslePillarRooms_TryJumpAcrossPillar() == 0)
    StagedActor_AdvancePair();
  EV_END();
}

/// Walking into a pillar in rooms 2 and 3: push it if the cell beyond is clear.
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
  struct MapCellFields cell;
};

/// Where one of room 4's pillars stands, the ground cell saved under it and its
/// footprint flag.
struct PillarSlot {
  s32 x; ///< 16-pixel map cells
  s32 y; ///< 16-pixel map cells
  s32 z; ///< 16-pixel map cells
  struct MapCellFields cell;
  s32 flag;
};

static struct PillarSlot sPillarSlots[4];
static s32 sProbePosition[3];
static u8 sShakenScroll[12];
static s32 sShakeChance;

/// Randomly substitutes the shifted background scroll during the water
/// transition.
static void CrossboneIslePillarRooms_JitterBackgroundScroll(void) {
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

#define DMA3 ((volatile u32 *)0x040000d4)

/// Waits until the DMA channel clears its enable bit.
static __inline__ void Dma_Wait(volatile u32 *dma) {
  while (dma[2] & 0x80000000)
    ;
}

/// Copies the three background scroll pairs and shifts their vertical offsets
/// by 192 pixels.
static void CrossboneIslePillarRooms_PrepareWaterScroll(void) {
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
static void CrossboneIslePillarRooms_AnimateWaterLevel(void) {
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
  FieldScheduler_AddCallback(CrossboneIslePillarRooms_PrepareWaterScroll,
                             0xc80);
  Engine_TaskWait(1);
  Runtime_SetIrqHandler(1, 0, CrossboneIslePillarRooms_JitterBackgroundScroll);
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
  Engine_TaskRemoveCallback(CrossboneIslePillarRooms_PrepareWaterScroll);
  Engine_MapRedraw();
  Engine_TaskWait(30);
}

/// Searching the switch in room 3 flips it. "Isaac flipped the switch." The
/// water rises, or drains, and the pillars stand higher or lower with it.
static void Event_FlipWaterSwitch(void) {
  s32 waterRaised;
  struct FieldActor *water;

  EV_BEGIN();
  Camera_SetSpeed(PIXELS(1), PIXELS(1) / 8);
  Camera_MoveTo(PIXELS(281), -1, PIXELS(432), 1);
  Engine_CameraWaitForMove();
  Engine_MessageShowCentered(MsgFieldFlippedSwitch, 1);
  waterRaised = FLAG_CHECK(WATER_RAISED_FLAG);
  // Raise or drain the water before updating the pillar collision footprints.
  if (waterRaised == 0) {
    SE(SE_232);
    Map_AnimateCells(gCrossboneIslePillarRooms_ShiftSteps1, 84, 24);
    WAIT(30);
    SE(SE_240);
    Engine_ActorSetSpritePriority(WATER_ACTOR, 1);
    Object_GetById(WATER_ACTOR)->motionFlags = waterRaised;
    water = Actor_Get(WATER_ACTOR);
    water->y.fixed = -PIXELS(32);
    OBJ_POS_SET(WATER_ACTOR, 272, 416);
    OBJ_MODE(WATER_ACTOR, 1);
    Map_AnimateCells(gCrossboneIslePillarRooms_ShiftSteps3, 80, 24);
    Map_AnimateCells(gCrossboneIslePillarRooms_ShiftSteps5, 80, 28);
    Map_CopyCellsTo(65, 40, 16, 27, 2, 4);
    CrossboneIslePillarRooms_AnimateWaterLevel();
    CrossboneIslePillarRooms_DrawRaisedPillars();
    Map_CopyCellAttributes(24, 3, 1, 1, 24, 8);
    FLAG_SET(WATER_RAISED_FLAG);
  } else {
    SE(SE_232);
    Map_AnimateCells(gCrossboneIslePillarRooms_ShiftSteps2, 84, 24);
    WAIT(30);
    SE(SE_230);
    Object_GetById(WATER_ACTOR)->motionFlags = 0;
    water = Actor_Get(WATER_ACTOR);
    water->y.fixed = -PIXELS(32);
    OBJ_POS_SET(WATER_ACTOR, 272, 436);
    OBJ_MODE(WATER_ACTOR, 2);
    Map_CopyCellsTo(65, 45, 16, 27, 2, 4);
    Map_AnimateCells(gCrossboneIslePillarRooms_ShiftSteps4, 80, 24);
    CrossboneIslePillarRooms_AnimateWaterLevel();
    CrossboneIslePillarRooms_RestoreGroundFootprint(9);
    CrossboneIslePillarRooms_RestoreGroundFootprint(10);
    CrossboneIslePillarRooms_RestoreGroundFootprint(11);
    CrossboneIslePillarRooms_RestoreGroundFootprint(12);
    CrossboneIslePillarRooms_RestoreGroundFootprint(13);
    CrossboneIslePillarRooms_RestoreGroundFootprint(14);
    CrossboneIslePillarRooms_RestoreGroundFootprint(15);
    Map_CopyCellAttributes(24, 4, 1, 1, 24, 8);
    GameFlag_Clear(WATER_RAISED_FLAG);
  }
  EV_END();
}

/// Matches a nearby actor's draw priority when it is behind the comparison
/// actor.
static s32
CrossboneIslePillarRooms_SyncPriorityIfBehind(struct FieldActor *actor,
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
CrossboneIslePillarRooms_SyncPriorityIfAhead(struct FieldActor *front,
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
static void
CrossboneIslePillarRooms_UpdateActorPriority(union FieldObject *fieldObject) {
#else
/// Keeps each moving pillar's priority and collision flags consistent with the
/// leader.
static s32
CrossboneIslePillarRooms_UpdateActorPriority(union FieldObject *fieldObject) {
#endif
  struct FieldActor *actor = (struct FieldActor *)fieldObject;
  struct FieldActor *leader = Object_GetById(0);
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
  hit = CrossboneIslePillarRooms_SyncPriorityIfBehind(actor, leader);
  for (id = 8; id <= 11; id++) {
    hit += CrossboneIslePillarRooms_SyncPriorityIfBehind(actor,
                                                         Object_GetById(id));
  }
  if (hit != 0) {
    for (id = 8; id <= 11; id++) {
      CrossboneIslePillarRooms_SyncPriorityIfAhead(actor, Object_GetById(id));
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
static void CrossboneIslePillarRooms_SortPillarActors(void) {
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
    firstActor = Object_GetById(i + 8);
    for (j = i; j <= 3; j++) {
      secondActor = Object_GetById(j + 8);
      if (firstActor->y.fixed <= secondActor->y.fixed &&
          firstActor->z.fixed < secondActor->z.fixed) {
        continue;
      }
      Dma_Set(secondActor, &savedActor, 0x8400001c, DMA3);
      Dma_Wait(DMA3);
      Dma_Set(firstActor, secondActor, 0x8400001c, DMA3);
      Dma_Wait(DMA3);
      Dma_Set(&savedActor, firstActor, 0x8400001c, DMA3);
      Dma_Wait(DMA3);
      Dma_Set(&sPillarSlots[j], &savedPlacement, 0x84000004, DMA3);
      Dma_Wait(DMA3);
      Dma_Set(&sPillarSlots[i], &sPillarSlots[j], 0x84000004, DMA3);
      Dma_Wait(DMA3);
      Dma_Set(&savedPlacement, &sPillarSlots[i], 0x84000004, DMA3);
      Dma_Wait(DMA3);
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

/// One of room 4's pillars, seen as a field actor while it moves and settles.
struct PillarActor {
  u8 unk00[8];
  s32 x; ///< 16.16 fixed-point pixels
  s32 y; ///< 16.16 fixed-point pixels
  s32 z; ///< 16.16 fixed-point pixels
  u8 unk14[14];
  u8 terrainId;
  u8 priorityFlags;
  s32 velocityX;
  s32 velocityY;
  u8 unk2c[0x29];
  u8 motionFlags;
};

/// Move or Carry on one of room 4's pillars, and after each push there: the
/// pillars settle, and their footprints follow them.
static void Event_MovePillar(void) {
  u32 id;
  struct PillarActor *actor;
  u8 *flags;
  u32 i;
  u32 k;
  struct PillarActor *other;
  struct MapCellFields cell;

  for (id = 8; id <= 11; id++) {
    actor = (struct PillarActor *)Object_GetById(id);
    actor->terrainId = 2;
    i = id - 8;
    if ((actor->x >> 20) == sPillarSlots[i].x &&
        (actor->z >> 20) == sPillarSlots[i].z && actor->velocityY == 0) {
      continue;
    }
    Dma_Set(&actor->x, sProbePosition, 0x84000003, DMA3);
    Dma_Wait(DMA3);
    if (FieldObject_CheckMovementCollision(actor, sProbePosition) == -1) {
      actor->motionFlags = 3;
    }
    flags = &actor->motionFlags;
    CrossboneIslePillarRooms_SetCellAttributes(
        0, sPillarSlots[i].x, sPillarSlots[i].z,
        (struct MapCell *)&sPillarSlots[i].cell);
    CrossboneIslePillarRooms_SetCellAttributes(
        2, sPillarSlots[i].x, sPillarSlots[i].z,
        (struct MapCell *)&sPillarSlots[i].cell);
    // Falling pillars either reach a ledge, shrink away, or settle on ground.
    if (*flags & 1) {
      if (GetMapCellCollision(2, actor->x, actor->z) == 50) {
        Engine_AudioPlayCue(SE_189);
        // Both u8 accesses are alias set 0, and the view is no struct reference
        // (MEM_IN_STRUCT_P).
        /* FAKEMATCH: the u8 view keeps these flag updates out of the record's
         * alias set; as field stores, the loop is reallocated and 217
         * instructions differ in tbs-en. */
        *(u8 *)&actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
        CrossboneIslePillarRooms_LowerActorToLedge(id, 1);
        *(u8 *)&actor->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
      } else if (GetMapCellCollision(2, actor->x, actor->z) == 51) {
        CrossboneIslePillarRooms_StopAtGroundOrHeight(
            (struct FieldActor *)actor, 0);
        Engine_AudioPlayCue(SE_189);
        actor->y = 0;
        actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
        CrossboneIslePillarRooms_ShrinkPillar(id);
        SET_POSITION(actor, 0, 0, 0);
        actor->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
      } else {
        CrossboneIslePillarRooms_SettleActorOnGround(
            (struct FieldActor *)actor);
      }
      *flags = 0;
    }
    // Save the new ground cell before applying the pillar's height attributes.
    CrossboneIslePillarRooms_ReadMapCell(0, actor->x >> 20, actor->z >> 20,
                                         &sPillarSlots[i].cell);
    if (actor->y >= 0) {
      CrossboneIslePillarRooms_ReadMapCell(0, 27, (actor->y >> 20) + 6, &cell);
      CrossboneIslePillarRooms_SetCellAttributes(
          0, actor->x >> 20, actor->z >> 20, (struct MapCell *)&cell);
      cell.kind = sPillarSlots[i].cell.kind;
      CrossboneIslePillarRooms_SetCellAttributes(
          2, actor->x >> 20, actor->z >> 20, (struct MapCell *)&cell);
    }
    sPillarSlots[i].x = actor->x >> 20;
    sPillarSlots[i].y = actor->y >> 20;
    sPillarSlots[i].z = actor->z >> 20;
    // Mark pillars covered by this one, then restore their sort order.
    for (k = 0; k <= 3; k++) {
      if (k == i) {
        continue;
      }
      Engine_GameFlagClear(sPillarSlots[k].flag);
      other = (struct PillarActor *)Object_GetById(k + 8);
      if ((actor->x >> 20) == (other->x >> 20) &&
          (actor->z >> 20) == (other->z >> 20) && actor->y > other->y) {
        Engine_GameFlagSet(sPillarSlots[k].flag);
      }
    }
  }
  CrossboneIslePillarRooms_SortPillarActors();
}

/// Walking into a pillar in room 4: jump across if possible, otherwise push
/// it, and the pillars settle.
static void Event_JumpOrPushRoom4Pillar(void) {
  EV_BEGIN();
#if TBS_PATCH_3
  Object_GetById(0)->y.fixed = Map_GetTerrainHeight(
      Object_GetById(0)->terrainId, Object_GetById(0)->x.fixed,
      Object_GetById(0)->z.fixed);
#endif
  if (CrossboneIslePillarRooms_TryJumpAcrossPillar() == 0) {
    Object_GetById(0)->motionFlags &= 254;
    Object_GetById(0)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
    StagedActor_AdvancePair();
    Event_MovePillar();
    Object_GetById(ACTOR_PARTY_LEADER)->motionFlags |= ACTOR_FOLLOWS_TERRAIN;
    Object_GetById(ACTOR_PARTY_LEADER)->priorityFlags |=
        ACTOR_PRIORITY_AUTOMATIC;
  }
  EV_END();
}

/// Walking north at room 4's ladder: climb up.
static void Event_ClimbUp(void) { Field_ClimbUp(); }

/// Walking south at room 4's ladder: push the pillar below, jump, or climb
/// down.
static void Event_PushOrClimbDown(void) {
  struct FieldActor *leader = Object_GetById(0);
  s32 pos[3];
  s32 *p = pos;

  p[0] = leader->x.fixed;
  p[1] = leader->y.fixed;
  p[2] = leader->z.fixed + PIXELS(16);
  if (StagedActor_FindAtTile(p, leader) != NULL) {
    Event_JumpOrPushRoom4Pillar();
  } else if (CrossboneIslePillarRooms_TryJumpAcrossPillar() == 0) {
    Field_ClimbDown();
  }
}

/// Stepping on trigger 40 in room 4: the fade overlay follows the leader, and
/// the leader's rise counter starts.
static void Event_StartRising(void) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  ((struct FadeOverlayWork *)WORK_SLOT(36))->actor = leader;
  leader->riseCounter = 1;
}

/// Stepping on trigger 41 in room 4: the overlay lets the leader go, and the
/// rise counter stops.
static void Event_StopRising(void) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);

  ((struct FadeOverlayWork *)WORK_SLOT(36))->actor = NULL;
  leader->riseCounter = 0;
}

// UB: calling this s32 function through a void update hook is incompatible.
// Plain void changes 54 to 52 bytes in tbs-en.
#ifdef UBFIX
/// Keeps the frozen pool's underfoot flag consistent with the leader's height
/// and depth.
static void CrossboneIslePillarRooms_UpdateFrozenPoolPriority(
    union FieldObject *fieldObject) {
#else
/// Puts the frozen pool under the leader when it is behind and below the
/// leader.
static s32 CrossboneIslePillarRooms_UpdateFrozenPoolPriority(
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

/// Remembers the frozen pool, installs its priority callback, and starts its
/// flicker script.
static void CrossboneIslePillarRooms_FreezePool(s32 id) {
  struct FieldActor *actor;

  actor = Object_GetById(id);
  EV_BEGIN();
#ifdef UBFIX
  actor->update = CrossboneIslePillarRooms_UpdateFrozenPoolPriority;
#else
  actor->update = (void (*)(
      union FieldObject *))CrossboneIslePillarRooms_UpdateFrozenPoolPriority;
#endif
  Map_CopyCellAttributes(20, 14, 1, 1, actor->x.fixed >> 20,
                         actor->z.fixed >> 20);
  FLAG_SET(FIRST_FROZEN_POOL_FLAG + id - 11);
  Actor_SetActionScript(id, gCrossboneIslePillarRooms_ActionTable);
  EV_END();
}

/// Frost on the first pool in room 5: it freezes into a pillar of ice.
static void Event_FreezeFirstPool(void) {
  CrossboneIslePillarRooms_FreezePool(11);
}

/// Frost on the second pool in room 5.
static void Event_FreezeSecondPool(void) {
  CrossboneIslePillarRooms_FreezePool(12);
}

/// Frost begins on the third pool: note whether the leader stands on it.
static void Event_FrostBeginsOnThirdPool(void) {
  struct FieldActor *leader;
  struct FieldActor *pillar;
  u8 *effects;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  pillar = Object_GetById(13);
  effects = gEffectWork;
  if (pillar->x.fixed >> 20 == leader->x.fixed >> 20 &&
      pillar->z.fixed >> 20 == leader->z.fixed >> 20) {
    Engine_GameFlagSet(LEADER_ON_THIRD_POOL_FLAG);
    effects[53] = 1;
  } else {
    Engine_GameFlagClear(LEADER_ON_THIRD_POOL_FLAG);
  }
}

/// Later in the same Frost: the third pool shrinks back, and the pillar beside
/// it (actor 14) rises 32 pixels.
static void Event_FrostRaisesPillar(void) {
  struct FieldActor *pillar;

  pillar = Object_GetById(14);
  Object_GetById(13)->scaleX = Q16_ONE;
  Object_GetById(13)->scaleY = Q16_ONE;
  (Object_GetById(13)->sprite)->image.priority = 2;
  pillar->sprite->image.priority = 2;
  pillar->acceleration = PIXELS(2) / 5;
  pillar->speed = PIXELS(4) / 5;
  FieldObject_SetPosition(pillar, pillar->x.fixed, PIXELS(32), pillar->z.fixed);
  Engine_ActorWaitForMove(14);
  Map_CopyCellAttributes(20, 14, 1, 1, 22, 16);
}

/// Emits growing, rotating particles during the third pool's Frost sequence.
static void CrossboneIslePillarRooms_SpawnFrostParticle(void) {
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
  settings->update = CrossboneIslePillarRooms_UpdateDampedParticle;

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
/// Emits a randomized particle from the thawing pool every eight frames.
static void
CrossboneIslePillarRooms_SpawnThawParticle(union FieldObject *fieldObject) {
#else
/// Emits a randomized particle from the thawing pool every eight frames.
static s32
CrossboneIslePillarRooms_SpawnThawParticle(union FieldObject *fieldObject) {
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

/// Frost ends on the third pool: unless the leader stood on it, it freezes,
/// and a frozen second pool thaws back to water.
static void Event_FreezeThirdPool(void) {
  s32 set;

  set = Engine_GameFlagIsSet(LEADER_ON_THIRD_POOL_FLAG);
  if (set == 0) {
    Engine_GameFlagSet(FIRST_FROZEN_POOL_FLAG + 2);
    EV_BEGIN();
    Engine_CameraSetSpeed(PIXELS(3) / 5, PIXELS(3) / 40);
    Engine_CameraMoveTo(PIXELS(312), -1, PIXELS(184), 1);
    Engine_CameraWaitForMove();
    WAIT(20);
    Map_CopyCellsTo(73, 10, 60, 10, 1, 2);
    WAIT(20);
    // The third pool freezes while the previously frozen second pool thaws.
    FieldScheduler_AddCallback(CrossboneIslePillarRooms_SpawnFrostParticle,
                               0xc80);
    WAIT(40);
    if (FLAG_CHECK(FIRST_FROZEN_POOL_FLAG + 1) != 0) {
#ifdef UBFIX
      Object_GetById(12)->update = CrossboneIslePillarRooms_SpawnThawParticle;
#else
      Object_GetById(12)->update = (void (*)(
          union FieldObject *))CrossboneIslePillarRooms_SpawnThawParticle;
#endif
      OBJ_MODE(12, 6);
      ObjectMotion_WaitForAnimationChange(12);
      Object_GetById(12)->update = (void (*)(union FieldObject *))set;
      Map_CopyCellAttributes(17, 13, 1, 1, 18, 13);
      Engine_GameFlagClear(FIRST_FROZEN_POOL_FLAG + 1);
      Engine_ActorSetChildValue(12, 0);
      Actor_SetBuiltinAction(12, ACTOR_ACTION_IDLE);
    } else {
      WAIT(60);
    }
    Engine_TaskRemoveCallback(CrossboneIslePillarRooms_SpawnFrostParticle);
    WAIT(20);
    Map_CopyCellsTo(72, 10, 60, 10, 1, 2);
    WAIT(20);
    EV_END();
  }
}

/// Move begins on room 5's sinking pillar: its cell is cleared.
static void Event_StartMovingSinkingPillar(void) {
  struct FieldActor *pillar = Object_GetById(10);
  s32 x;
  s32 z;

  EV_BEGIN();

  x = pillar->x.fixed >> 20;
  z = pillar->z.fixed >> 20;

  StagedActor_FillGridAttributeRectangle(2, x, z, 1, 1, 0);
  EV_END();
}

/// Move on room 5's sinking pillar, and after each push: once it reaches
/// column 16 it sinks two pixels into place for good.
static void Event_MoveSinkingPillar(void) {
  struct FieldActor *pillar;
  s32 alreadyLowered;

  pillar = Object_GetById(10);
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

/// Walking into room 5's sinking pillar: push it if the cell beyond is clear.
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

  if (scene == (s32)&SceneId_CrossboneIslePillarRooms1) {
    return gCrossboneIslePillarRoomsEvents1;
  }
  if (scene == (s32)&SceneId_CrossboneIslePillarRooms2) {
    return gCrossboneIslePillarRoomsEvents2;
  }
  if (scene == (s32)&SceneId_CrossboneIslePillarRooms3) {
    return gCrossboneIslePillarRoomsEvents3;
  }
  if (scene == (s32)&SceneId_CrossboneIslePillarRooms4) {
    return gCrossboneIslePillarRoomsEvents4;
  }
  if (scene == (s32)&SceneId_CrossboneIslePillarRooms5) {
    return gCrossboneIslePillarRoomsEvents5;
  }
  return gCrossboneIslePillarRoomsEventsOther;
}

/// Prepares the lowered actor's collision and draw priority below the leader.
static void CrossboneIslePillarRooms_PrepLoweredActor(s32 id) {
  struct FieldActor *actor = Object_GetById(id);

  actor->collisionFlags &= 254;
  actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  actor->motionFlags = 0;
  Engine_ActorSetSpriteFlags(actor, 0);
  actor->sprite->image.priority = 2;
}

/// Restores a previously frozen pillar's mode, map cell, priority hook, and
/// script.
static void CrossboneIslePillarRooms_RestoreFlaggedPillar(s32 actorId) {
  struct FieldActor *actor;

  actor = Object_GetById(actorId);
  if (FLAG_CHECK(FIRST_FROZEN_POOL_FLAG + actorId - 11) != 0) {
    Object_SetMode(actor, 5);
#ifdef UBFIX
    actor->update = CrossboneIslePillarRooms_UpdateFrozenPoolPriority;
#else
    actor->update = (void (*)(
        union FieldObject *))CrossboneIslePillarRooms_UpdateFrozenPoolPriority;
#endif
    Map_CopyCellAttributes(20, 14, 1, 1, actor->x.fixed >> 20,
                           actor->z.fixed >> 20);
    Actor_SetActionScript(actorId, gCrossboneIslePillarRooms_ActionTable);
  }
}

/// Uses terrain layer 2 and installs the moving pillar's priority callback.
static void CrossboneIslePillarRooms_InitializePillarPriority(s32 id) {
  struct FieldActor *actor = Object_GetById(id);

  actor->terrainId = 2;
  actor->motionFlags = 0;
#ifdef UBFIX
  actor->update = CrossboneIslePillarRooms_UpdateActorPriority;
#else
  actor->update = (void (*)(
      union FieldObject *))CrossboneIslePillarRooms_UpdateActorPriority;
#endif
}

/// Matches the leader's draw priority to a nearby pillar in front.
static void CrossboneIslePillarRooms_UpdateLeaderPriority(void) {
  struct FieldActor *hero = Object_GetById(ACTOR_PARTY_LEADER);
  u32 selector;
  struct FieldActor *actor;

  for (selector = 8; selector <= 11; selector++) {
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
  if (gGameState.scene == (s32)&SceneId_CrossboneIslePillarRooms2) {
    FieldScene_RedrawActorFootprint(8);
    FieldScene_RedrawActorFootprint(9);
    FieldScene_RedrawActorFootprint(10);
    FieldScene_RedrawActorFootprint(11);
    FieldScene_RedrawActorFootprint(12);
  } else if (gGameState.scene == (s32)&SceneId_CrossboneIslePillarRooms3) {
    // Rebuild the water room and restore raised pillars when its flag persists.
    Map_CopyCells(32, 0, 64, 32, 0, 64);
    FieldScene_RedrawActorFootprint(8);
    FieldScene_RedrawActorFootprint(9);
    FieldScene_RedrawActorFootprint(10);
    FieldScene_RedrawActorFootprint(11);
    FieldScene_RedrawActorFootprint(12);
    FieldScene_RedrawActorFootprint(13);
    FieldScene_RedrawActorFootprint(14);
    FieldScene_RedrawActorFootprint(15);
    if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) != 0 &&
        Engine_GameFlagIsSet(WATER_RAISED_FLAG) != 0) {
      Map_CopyCellsTo(79, 34, 84, 24, 1, 2);
      Map_CopyCellsTo(0, 32, 32, 0, 32, 32);
      Map_CopyCellsTo(32, 32, 64, 0, 32, 32);
      CrossboneIslePillarRooms_DrawRaisedPillars();
      Map_CopyCellAttributes(24, 3, 1, 1, 24, 8);
    }
  } else if (gGameState.scene == (s32)&SceneId_CrossboneIslePillarRooms4) {
    // Restore the four movable pillars and the leader's riding overlay.
    OverlayObject_CreateConfiguredObject(PIXELS(584), 0, PIXELS(200), 223);
    if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
      Object_GetById(ACTOR_PARTY_LEADER)->riseCounter = 1;
    }
    BattleFx_StartFadeOverlay(0);
    if (Object_GetById(ACTOR_PARTY_LEADER)->riseCounter == 0) {
      Event_StopRising();
    }
    CrossboneIslePillarRooms_InitializePillarPriority(8);
    CrossboneIslePillarRooms_InitializePillarPriority(9);
    CrossboneIslePillarRooms_InitializePillarPriority(10);
    CrossboneIslePillarRooms_InitializePillarPriority(11);
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
    FieldScheduler_AddCallback(CrossboneIslePillarRooms_UpdateLeaderPriority,
                               TASK_PRIORITY_SCENE);
    if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) != 0) {
      for (id = 8; id < 12; id++) {
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
  } else if (gGameState.scene == (s32)&SceneId_CrossboneIslePillarRooms5) {
    // Restore room 5's moved pillar, frozen pools, and raised pillar pair.
    OBJ_MODE(10, 2);
    Engine_ActorSetChildValue(10, 6);
    FieldScene_RedrawActorFootprint(8);
    FieldScene_RedrawActorFootprint(9);
#if TBS_PATCH_1
    Object_GetById(8)->motionFlags = 0;
    Object_GetById(9)->motionFlags = 0;
#endif
    Event_MoveSinkingPillar();
    CrossboneIslePillarRooms_PrepLoweredActor(11);
    CrossboneIslePillarRooms_PrepLoweredActor(12);
    CrossboneIslePillarRooms_PrepLoweredActor(13);
    CrossboneIslePillarRooms_RestoreFlaggedPillar(11);
    CrossboneIslePillarRooms_RestoreFlaggedPillar(12);
    CrossboneIslePillarRooms_RestoreFlaggedPillar(13);
    actor = Object_GetById(13);
    actor->update = 0;
    CrossboneIslePillarRooms_PrepLoweredActor(14);
    Object_GetById(14)->collisionFlags |= 8;
    if (Engine_GameFlagIsSet(FIRST_FROZEN_POOL_FLAG + 2) == 0) {
      Object_GetById(13)->scaleX = 3 * Q16_ONE / 2;
      Object_GetById(13)->scaleY = 3 * Q16_ONE / 2;
      Object_GetById(13)->sprite->image.priority = 3;
      Object_GetById(14)->sprite->image.priority = 3;
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

s32 gCrossboneIslePillarRooms_ActionTable[] = {
    SC_SET(0x23, 2),
    SC_CALL(CrossboneIslePillarRooms_SetChildValueFromTimer),
    SC_WAIT(2),
    SC_SET(0x23, 0),
    SC_CALL(CrossboneIslePillarRooms_SetChildValueFromTimer),
    SC_WAIT(6),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

u16 gCrossboneIslePillarRooms_ShiftSteps1[] = {
    MAP_CELL_STEP(76, 34, 1, 2, 4), MAP_CELL_STEP(77, 34, 1, 2, 4), MAP_CELL_STEP(78, 34, 1, 2, 4), MAP_CELL_STEP(79, 34, 1, 2, 4), MAP_CELL_STEPS_END,
};

u16 gCrossboneIslePillarRooms_ShiftSteps2[] = {
    MAP_CELL_STEP(79, 34, 1, 2, 4), MAP_CELL_STEP(78, 34, 1, 2, 4), MAP_CELL_STEP(77, 34, 1, 2, 4), MAP_CELL_STEP(76, 34, 1, 2, 4), MAP_CELL_STEPS_END,
};

u16 gCrossboneIslePillarRooms_ShiftSteps3[] = {
    MAP_CELL_STEP(67, 34, 2, 5, 6), MAP_CELL_STEP(69, 34, 2, 5, 6), MAP_CELL_STEP(71, 34, 2, 5, 6), MAP_CELL_STEP(73, 34, 2, 5, 6), MAP_CELL_STEPS_END,
};

u16 gCrossboneIslePillarRooms_ShiftSteps4[] = {
    MAP_CELL_STEP(71, 50, 2, 5, 6), MAP_CELL_STEP(69, 50, 2, 5, 6), MAP_CELL_STEP(67, 50, 2, 5, 6), MAP_CELL_STEP(65, 50, 2, 5, 6), MAP_CELL_STEPS_END,
};

u16 gCrossboneIslePillarRooms_ShiftSteps5[] = {
    MAP_CELL_STEP(75, 38, 2, 1, 4), MAP_CELL_STEP(77, 38, 2, 1, 6), MAP_CELL_STEP(79, 38, 2, 1, 8), MAP_CELL_STEP(65, 53, 2, 1, 1), MAP_CELL_STEPS_END,
};

struct SceneEntrance gCrossboneIslePillarRoomsEntrancesOther[] = {
    { 0, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gCrossboneIslePillarRoomsEntrances1[] = {
    { 1, CONDITION_ALWAYS, 712, 0, 760, FACING_NORTH, 0, 560, 368, 992, 792, 0 },
    { 2, CONDITION_ALWAYS, 600, 0, 760, FACING_NORTH, 0, 560, 368, 992, 792, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gCrossboneIslePillarRoomsEntrances2[] = {
    { 1, CONDITION_ALWAYS, 216, 0, 360, FACING_NORTH, 0, 32, 32, 384, 400, 0 },
    { 2, CONDITION_ALWAYS, 104, 0, 360, FACING_NORTH, 0, 32, 32, 384, 400, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gCrossboneIslePillarRoomsEntrances3[] = {
    { 1, CONDITION_ALWAYS, 184, 0, 472, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { 2, CONDITION_ALWAYS, 64, 0, 472, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gCrossboneIslePillarRoomsEntrances4[] = {
    { 1, CONDITION_ALWAYS, 744, 0, 520, FACING_NORTH, 0, 440, 16, 960, 544, 0 },
    { 2, CONDITION_ALWAYS, 632, 0, 520, FACING_NORTH, 0, 440, 16, 960, 544, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gCrossboneIslePillarRoomsEntrances5[] = {
    { 1, CONDITION_ALWAYS, 280, 0, 488, FACING_NORTH, 0, 16, 32, 464, 528, 0 },
    { 2, CONDITION_ALWAYS, 168, 0, 488, FACING_NORTH, 0, 16, 32, 464, 528, 0 },
    { SCENE_TABLE_END },
};

u32 gCrossboneIslePillarRoomsExits[] = {
    SCENE_EXITS(115),
    SCENE_EXIT(1, 126, 3),
    SCENE_EXIT(2, 126, 4),
    SCENE_EXIT(0, 116, 0),
    SCENE_EXIT(1, 127, 3),
    SCENE_EXIT(2, 127, 4),
    SCENE_EXIT(0, 119, 0),
    SCENE_EXIT(1, 130, 3),
    SCENE_EXIT(2, 130, 4),
    SCENE_EXIT(0, 121, 0),
    SCENE_EXIT(1, 132, 3),
    SCENE_EXIT(2, 132, 4),
    SCENE_EXIT(0, 122, 0),
    SCENE_EXIT(1, 133, 3),
    SCENE_EXIT(2, 133, 4),
    SCENE_EXITS_END,
};

struct ScenePlacement gCrossboneIslePillarRoomsNoPlacements[] = {
    { SCENE_TABLE_END },
};

struct ScenePlacement gCrossboneIslePillarRoomsPlacements1[] = {
    { 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(744), 0, PIXELS(488), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(792), 0, PIXELS(552), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(696), 0, PIXELS(616), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(776), 0, PIXELS(680), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gCrossboneIslePillarRoomsPlacements2[] = {
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

struct ScenePlacement gCrossboneIslePillarRoomsPlacements3[] = {
    { 0xcd, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xcf, CONDITION_ALWAYS, 7,
        PIXELS(144), 0, PIXELS(296), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xcd, CONDITION_ALWAYS, 7,
        PIXELS(232), 0, PIXELS(256), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xcf, CONDITION_ALWAYS, 7,
        PIXELS(208), 0, PIXELS(216), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xcd, CONDITION_ALWAYS, 7,
        PIXELS(232), 0, PIXELS(176), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xcd, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xcf, CONDITION_ALWAYS, 7,
        PIXELS(272), 0, PIXELS(168), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xcf, CONDITION_ALWAYS, 7,
        PIXELS(352), 0, PIXELS(168), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xd6, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gCrossboneIslePillarRoomsPlacementsOther[] = {
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

struct ScenePlacement gCrossboneIslePillarRoomsPlacements5[] = {
    { 0xcf, CONDITION_ALWAYS, 7,
        PIXELS(208), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xcf, CONDITION_ALWAYS, 7,
        PIXELS(224), 0, PIXELS(280), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x125, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(232), 0, PIXELS(296), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xe3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(136), 0, PIXELS(216), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xe3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(296), 0, PIXELS(216), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xe3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(360), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x122, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(360), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gCrossboneIslePillarRoomsEventsOther[] = {
    { SCENE_EVENTS_END },
};

struct SceneEvent gCrossboneIslePillarRoomsEvents1[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_TOUCH | EVENT_TOUCH_WALKING, 10, CONDITION_ALWAYS, EVENT_SCRIPT(Event_JumpOrPushPillar) },
    { SEARCH(SEARCH_CHEST), 100, 0xec2, FIND_ITEM(ITEM_ANTIDOTE) },
    { SEARCH(SEARCH_CHEST), 101, 0xec3, FIND_ITEM(ITEM_NUT) },
    { SEARCH(SEARCH_CHEST), 102, 0xec4, FIND_COINS(111) },
    { SEARCH(SEARCH_CHEST), 103, 0xec5, FIND_ITEM(ITEM_HARD_NUT) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, CONDITION_ALWAYS, 0 },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 9, CONDITION_ALWAYS, 0 },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 10, CONDITION_ALWAYS, 0 },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 11, CONDITION_ALWAYS, 0 },
    { SCENE_EVENTS_END },
};

struct SceneEvent gCrossboneIslePillarRoomsEvents2[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_TOUCH | EVENT_TOUCH_WALKING, 10, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushPillar) },
    { SEARCH(SEARCH_CHEST), 100, 0xec6, FIND_ITEM(ITEM_LUCKY_PEPPER) },
    { SEARCH(SEARCH_CHEST), 101, 0xec7, FIND_ITEM(ITEM_MYSTERY_BLADE) },
    { SEARCH(SEARCH_CHEST), 102, 0xec8, FIND_COINS(222) },
    { SEARCH(SEARCH_CHEST), 103, 0xec9, FIND_ITEM(ITEM_ELIXIR) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gCrossboneIslePillarRoomsEvents3[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
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

struct SceneEvent gCrossboneIslePillarRoomsEvents4[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_TOUCH | EVENT_TOUCH_WALKING, 30, CONDITION_ALWAYS, EVENT_SCRIPT(Event_JumpOrPushRoom4Pillar) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 31, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ClimbUp) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 31, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushOrClimbDown) },
    { EVENT_TOUCH | EVENT_TOUCH_WALKING, 31, CONDITION_ALWAYS, EVENT_SCRIPT(Event_JumpOrPushRoom4Pillar) },
    { EVENT_TOUCH, 40, CONDITION_ALWAYS, EVENT_SCRIPT(Event_StartRising) },
    { EVENT_TOUCH, 41, CONDITION_ALWAYS, EVENT_SCRIPT(Event_StopRising) },
    { SEARCH(SEARCH_CHEST), 100, 0xeda, FIND_COINS(777) },
    { SEARCH(SEARCH_CHEST), 101, 0xedb, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { 0x413, 102, 0xedc, FIND_ITEM(ITEM_PSY_CRYSTAL) },
    { SEARCH(SEARCH_CHEST), 103, 0xedd, FIND_ITEM(ITEM_WICKED_MACE) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, FLAG_SCENE_TEMP_00, EVENT_SCRIPT(Event_MovePillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 9, FLAG_SCENE_TEMP_01, EVENT_SCRIPT(Event_MovePillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 10, FLAG_SCENE_TEMP_02, EVENT_SCRIPT(Event_MovePillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 11, FLAG_SCENE_TEMP_03, EVENT_SCRIPT(Event_MovePillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_CARRY), 8, FLAG_SCENE_TEMP_00, EVENT_SCRIPT(Event_MovePillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_CARRY), 9, FLAG_SCENE_TEMP_01, EVENT_SCRIPT(Event_MovePillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_CARRY), 10, FLAG_SCENE_TEMP_02, EVENT_SCRIPT(Event_MovePillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_CARRY), 11, FLAG_SCENE_TEMP_03, EVENT_SCRIPT(Event_MovePillar) },
    { SCENE_EVENTS_END },
};
// clang-format on

struct SceneEvent gCrossboneIslePillarRoomsEvents5[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
    {EVENT_EXIT, 2, CONDITION_ALWAYS, 2},
    {EVENT_TOUCH | EVENT_TOUCH_WALKING, 10, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_PushSinkingPillar)},
    {SEARCH(SEARCH_CHEST), 100, 0xede, FIND_COINS(888)},
    {SEARCH(SEARCH_CHEST), 101, 0xedf, FIND_ITEM(ITEM_POTION)},
    {SEARCH(SEARCH_CHEST), 102, 0xee0, FIND_ITEM(ITEM_CLERICS_RING)},
    {SEARCH(SEARCH_CHEST), 103, 0xee1, FIND_ITEM(ITEM_SMOKE_BOMB)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, 1), 10, PILLAR_SUNK_FLAG,
     EVENT_SCRIPT(Event_StartMovingSinkingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 10, PILLAR_SUNK_FLAG,
     EVENT_SCRIPT(Event_MoveSinkingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 11, FIRST_FROZEN_POOL_FLAG,
     EVENT_SCRIPT(Event_FreezeFirstPool)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 12, FIRST_FROZEN_POOL_FLAG + 1,
     EVENT_SCRIPT(Event_FreezeSecondPool)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_FROST, 1), 13, FIRST_FROZEN_POOL_FLAG + 2,
     EVENT_SCRIPT(Event_FrostBeginsOnThirdPool)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_FROST, 5), 13, FIRST_FROZEN_POOL_FLAG + 2,
     EVENT_SCRIPT(Event_FrostRaisesPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 13, FIRST_FROZEN_POOL_FLAG + 2,
     EVENT_SCRIPT(Event_FreezeThirdPool)},
    {SCENE_EVENTS_END},
};
