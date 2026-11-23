/// @file
/// The Yampi Desert Cave, the hidden dungeon behind the desert's sand slide:
/// a Teleport circle leads on from the room the party comes in by, and in the
/// rooms beyond the party pushes logs, sinks a stake with Pound, bursts
/// pillars, slips under walls with Sand, Scoops up the Venus Djinni Crystal
/// racing through the sand, and meets Valukar, the guardian of fire.
#include "djinn_ids.h"
#include "fixed_math.h"
#include "flags.h"
#include "macros.h"
#include "scene_ids.h"
#include "sprite_ids.h"
#include "types.h"
#include "version.h"
#include "battle/battle.h"
#include "field/actor.h"
#include "field/actor_script.h"
#include "field/event_commands.h"
#include "field/event_inline.h"
#include "field/field.h"
#include "field/field_coordinate.h"
#include "field/field_event.h"
#include "field/field_sequence_event.h"
#include "field/map_grid.h"
#include "field/scene_tables.h"
#include "game/game.h"
#include "graphics/palette_buffers.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "scene/scene.h"
#include "sound/sound.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "system/main_routine_work.h"
#include "system/math_table.h"
#include "system/ram_buffer.h"
#include "system/system.h"
#include "system/task.h"
#include "text/msg_ids.h"

s32 MapGrid_CheckCollision(struct Actor *actor, struct FieldPosition *position);

#include "shared/tables.h"

/// Set once Burst has broken the pillar in the Water of Life chest's room.
#define FLAG_YAMPI_DESERT_CAVE_CHEST_ROOM_PILLAR_BURST FLAG_AREA_TEMP_00
/// Set once Burst has broken the pillar in Crystal's room.
#define FLAG_YAMPI_DESERT_CAVE_CRYSTAL_ROOM_PILLAR_BURST FLAG_AREA_TEMP_01
/// Set once Scoop has dug up the Mythril Silver in Valukar's room.
#define FLAG_YAMPI_DESERT_CAVE_MYTHRIL_SILVER_DUG_UP FLAG_AREA_TEMP_02
/// Set as Move leaves each pillar where it opens the way: Valukar's room's,
/// and Crystal's room's west and east pillars.
#define FLAG_YAMPI_DESERT_CAVE_GUARDIAN_PILLAR_MOVED FLAG_AREA_TEMP_03
#define FLAG_YAMPI_DESERT_CAVE_WEST_PILLAR_MOVED FLAG_AREA_TEMP_04
#define FLAG_YAMPI_DESERT_CAVE_EAST_PILLAR_MOVED FLAG_AREA_TEMP_05
/// Set once Pound has sunk the stake in Crystal's room.
#define FLAG_YAMPI_DESERT_CAVE_STAKE_POUNDED FLAG_SCENE_TEMP_06
/// Set while the sand ripple runs, the Djinni Crystal racing under it.
#define FLAG_YAMPI_DESERT_CAVE_RIPPLE_RUNNING FLAG_SCENE_TEMP_70
/// Set once Scoop has dug Crystal out of the sand.
#define FLAG_YAMPI_DESERT_CAVE_CRYSTAL_DUG_UP FLAG_SCENE_TEMP_71
/// The ripple's step count, the byte of flags from FLAG_SCENE_TEMP_78: each
/// RIPPLE_STEPS_PER_POINT steps it moves on one point of
/// gYampiDesertCave_RipplePath.
#define FLAG_BYTE_YAMPI_DESERT_CAVE_RIPPLE_STEP FLAG_SCENE_TEMP_78
/// The steps the ripple takes at each point of its path.
#define RIPPLE_STEPS_PER_POINT 32

/// The lava cave's battle backdrop, Valukar's battle's.
extern char ResourceId_LavaCaveBackdrop;
/// Set once Valukar, the guardian of fire, has been beaten; he is gone.
#define FLAG_VALUKAR_BEATEN 0x9cd

/// The Water of Life chest's room's actors
/// (SceneId_YampiDesertCaveWaterOfLifeChest), the rows of
/// gYampiDesertCave_Placements.
enum YampiDesertCaveChestRoomActorId {
  /// The three fallen columns pushed as logs (gPushKinds).
  ACTOR_CHEST_ROOM_FIRST_LOG = ACTOR_FIRST_PLACED,
  ACTOR_CHEST_ROOM_SECOND_LOG,
  ACTOR_CHEST_ROOM_THIRD_LOG,
  /// The pillar Burst breaks (SPRITE_BURST_PILLAR).
  ACTOR_CHEST_ROOM_BURST_PILLAR
};

/// The chest room's two Teleport circles, the north one where the party comes
/// in from the desert and the south one: the collision code of each circle's
/// cells, and the entrance on it. Each circle sends the party to the other.
enum YampiDesertCaveTeleportCircle {
  NORTH_CIRCLE_CELLS = 30,
  SOUTH_CIRCLE_CELLS = 31,
  NORTH_CIRCLE_ENTRANCE = 9,
  SOUTH_CIRCLE_ENTRANCE = 10
};

/// The cells the rooms' trigger tables answer, by the behaviour the maps give
/// them: round the logs, the Water of Life chest room's pillar Burst breaks,
/// the stretch of sand in Crystal's room the ripple races over (cells (46, 14)
/// on, which Burst and Scoop both answer on), before Valukar, and over the
/// buried Mythril Silver.
enum YampiDesertCaveCell {
  CELL_AROUND_LOG = 42,
  CELL_BURST_PILLAR = 33,
  CELL_RIPPLE_SAND = 33,
  CELL_BEFORE_VALUKAR = 52,
  CELL_MYTHRIL_SILVER = 100
};

/// Valukar's room's actors (SceneId_YampiDesertCaveGuardian).
enum YampiDesertCaveGuardianActorId {
  /// The stone pillar Move pushes (SPRITE_STONE_PILLAR).
  ACTOR_GUARDIAN_ROOM_PILLAR = ACTOR_FIRST_PLACED,
  /// Valukar, the guardian of fire: "I am the..." (MsgIAmThe).
  ACTOR_VALUKAR
};

/// Crystal's room's actors (SceneId_YampiDesertCaveCrystal).
enum YampiDesertCaveCrystalActorId {
  ACTOR_CRYSTAL_ROOM_FIRST_LOG = ACTOR_FIRST_PLACED,
  ACTOR_CRYSTAL_ROOM_SECOND_LOG,
  ACTOR_CRYSTAL_ROOM_THIRD_LOG,
  ACTOR_CRYSTAL_ROOM_FOURTH_LOG,
  /// The two stone pillars Move pushes, east and west.
  ACTOR_EAST_PILLAR,
  ACTOR_WEST_PILLAR,
  /// The stake Pound sinks.
  ACTOR_STAKE,
  /// The pillar Burst breaks.
  ACTOR_CRYSTAL_ROOM_BURST_PILLAR,
  /// The ripple of sand the Djinni Crystal races under
  /// (YampiDesertCave_WonderAtRipple).
  ACTOR_SAND_RIPPLE,
  /// Crystal, the Venus Djinni, dug out of the sand
  /// (YampiDesertCave_FightCrystal).
  ACTOR_CRYSTAL
};

/// The Mythril Silver buried in Valukar's room, its first scene item's
/// treasure spot.
enum { ACTOR_MYTHRIL_SILVER = FIELD_TREASURE_ACTOR };

extern u16 gPushKinds[];

void YampiDesertCave_StopLeaderAtLog(struct Actor *actor);

void Teleport_ArriveWhirling(void);

extern u16 gYampiDesertCave_CircleCycleSlots[];

void FieldEffect_StartBlankOverlay(s32 value);

void YampiDesertCave_FightCrystal(void);

void YampiDesertCave_WonderAtRipple(void);

/// Walking against one of the logs lying in the inner cave or in Crystal's
/// room pushes it, and it rolls on until it meets a wall or another log.
void YampiDesertCave_TryPushLog(void);

void YampiDesertCave_TeleportFromNorthCircle(void);

void YampiDesertCave_TeleportFromSouthCircle(void);

void YampiDesertCave_CastSand(void);
void YampiDesertCave_EndSand(void);

extern u32 gYampiDesertCave_Settle[];

struct Actor *Actor_FindAtCell(s32 *position, s32 mode);
/// The Teleport circles' shimmer (shared/teleport_shimmer.c).
u16 gPaletteCycleBright BSS_DATA;
u16 *gPaletteCycleSlots BSS_DATA;
u16 gPaletteCycleTimer BSS_DATA;
u16 gPaletteCycleDelay BSS_DATA ALIGNED(4);
u16 *gPaletteSlots BSS_DATA;
u16 *gPaletteColors BSS_DATA;
u16 *gPaletteCycleColors BSS_DATA;
u16 gPaletteCycleLength BSS_DATA;
u16 gPaletteCycleIndex BSS_DATA ALIGNED(4);
void Actor_DisableById(s32 id);

extern s8 gPushFootprints[][4];

/// Makes a log's cells solid where it lies: its footprint's cells take the
/// raised layers' attributes and block the ground; returns 0 for an actor that
/// is no log.
s32 YampiDesertCave_SetLogCells(s32 id) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct Actor *actor = Actor_GetById(id);
  s16 sprite = ((struct Sprite *)actor->sprite)->entries[0]->animationId;
  s16 i;
  u32 kind;
  s32 w, h, x, z;
  s32 az;
  s32 bx, bz;

  i = 0;
  if ((u16)sprite != gPushKinds[i]) {
    do {
      kind = (u16)NEXT_HALFWORD(i);
      i = kind;
    } while (kind < PUSH_KINDS && (u16)sprite != gPushKinds[kind]);
  }
  kind = (u16)i;
  if (kind == PUSH_KINDS)
    return 0;
  w = (ABS(gPushFootprints[kind][0]) + ABS(gPushFootprints[kind][2])) >> 4;
  // i is reused: the footprint's top here, the middle layer's x offset in
  // cells below.
  i = gPushFootprints[kind][1];
  h = (ABS(i) + ABS(gPushFootprints[kind][3])) >> 4;
  x = FIELD_CELL(actor->x.fixed + toFIXED(gPushFootprints[kind][0]));
  z = FIELD_CELL(actor->z.fixed + toFIXED(i));
  i = FIELD_CELL(map->layers[MAPGRID_LAYER_MIDDLE].offsetX);
  az = FIELD_CELL(map->layers[MAPGRID_LAYER_MIDDLE].offsetY);
  bx = map->layers[MAPGRID_LAYER_OVERHEAD].offsetX;
  bz = map->layers[MAPGRID_LAYER_OVERHEAD].offsetY;
  MapGrid_CopyCellAttributes(x, z, w, h, (u16)i + x, (u16)az + z);
  MapGrid_CopyCellAttributes((u16)FIELD_CELL(bx) + x, (u16)FIELD_CELL(bz) + z,
                             w, h, x + 64, z + 64);
  PushedActor_FillGridAttributeRectangle(MAPGRID_LAYER_GROUND, x, z, w, h,
                                         CELL_BEHAVIOR_WALL);
  PushedActor_FillGridAttributeRectangle(MAPGRID_LAYER_OVERHEAD, x, z, w, h,
                                         CELL_BEHAVIOR_WALL);
  return 1;
}

#include "../field/map_cell_fill.c"

extern s32 gPushLeaderSteps[];

/// Where a pushed log comes to rest, found before the leader pushes it.
struct LogSlide {
  FIXED x, y, z;  ///< the log's position at the end of the slide
  s32 kind;       ///< its row of gPushFootprints
  s32 slot;       ///< its actor id
  void *onPushed; ///< run as the log slides, if not NULL
};

/// Finds the log the leader faces and can push: its kind, its actor slot and
/// the direction, or NULL.
struct Actor *YampiDesertCave_FindPushedLog(s32 *direction, s32 *slot,
                                            s32 *kind);

/// What MapGrid_CheckCollision says of a step to a position: the system call
/// layer's inline, as The Broken Seal's system_call.h has it, which this
/// game's does not yet.
static inline s32 FieldActor_CheckMovementCollision(struct Actor *actor,
                                                    s32 *position) {
  return MapGrid_CheckCollision(actor, position);
}

/// Finds how far a pushed object can slide in the leader's direction, probing
/// its footprint cell by cell until MapGrid_CheckCollision reports a wall.
/// Returns 1 with the log's end position in slide, or 0 if it cannot move.
s32 YampiDesertCave_FindLogSlide(struct LogSlide *slide) {
  FIXED probe[3];
  s32 direction;
  struct Actor *actor;
  s32 kind, w, h, dx;
  FIXED dz;
  s32 count, i, j;
  u8 *layer;

  actor = YampiDesertCave_FindPushedLog(&direction, &slide->slot, &slide->kind);
  if (actor != NULL) {
    kind = slide->kind;
    w = (ABS(gPushFootprints[kind][0]) + ABS(gPushFootprints[kind][2])) >> 4;
    h = (ABS(gPushFootprints[kind][1]) + ABS(gPushFootprints[kind][3])) >> 4;
    dx = gPushLeaderSteps[direction] & FIELD_PIXEL_MASK;
    dz = toFIXED(gPushLeaderSteps[direction]);
    slide->onPushed = NULL;
    count = 0;
    layer = &actor->layer;
    *layer = MAPGRID_LAYER_OVERHEAD;
    slide->x = actor->x.fixed + dx;
    slide->z = actor->z.fixed + dz;
    probe[1] = actor->y.fixed;
    do {
      probe[2] = slide->z + toFIXED(gPushFootprints[kind][1]);
      for (j = 0; j < h; j++) {
        probe[0] = slide->x + toFIXED(gPushFootprints[kind][0]);
        for (i = 0; i < w; i++) {
          if (FieldActor_CheckMovementCollision(actor, probe) ==
              COLLISION_IMPASSABLE)
            goto blocked;
          probe[0] += toFIXED(MAPGRID_CELL_PIXELS);
        }
        probe[2] += toFIXED(MAPGRID_CELL_PIXELS);
      }
      slide->x += dx;
      count++;
      slide->z += dz;
    } while (1);
  blocked:
    *layer = MAPGRID_LAYER_GROUND;
    if (count == 0)
      return 0;
    slide->x = actor->x.fixed + dx * count;
    slide->y = actor->y.fixed;
    slide->z = actor->z.fixed + dz * count;
    return 1;
  } else {
    return 0;
  }
}

struct Actor *YampiDesertCave_FindPushedLog(s32 *direction, s32 *slot,
                                            s32 *kind) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct Actor **entry;
  struct Actor *actor;
  u32 i, k;
  s32 x, z, left, top, right, bottom;

  *direction = FACING_INDEX(leader->facing);
  i = ACTOR_FIRST_PLACED;
  entry = &work->actors[ACTOR_FIRST_PLACED];
  for (; i < FIELD_TREASURE_ACTOR; i++) {
    actor = *entry;
    for (k = 0; k < PUSH_KINDS; k++) {
      if (((struct Sprite *)actor->sprite)->entries[0]->animationId ==
          gPushKinds[k]) {
        x = (leader->x.part[0].integer +
             PUSH_STEP_X(gPushLeaderSteps[*direction])) >>
            4;
        z = (leader->z.part[0].integer +
             PUSH_STEP_Z(gPushLeaderSteps[*direction])) >>
            4;
        left = (actor->x.part[0].integer + gPushFootprints[k][0]) >> 4;
        top = (actor->z.part[0].integer + gPushFootprints[k][1]) >> 4;
        right = (actor->x.part[0].integer + gPushFootprints[k][2]) >> 4;
        bottom = (actor->z.part[0].integer + gPushFootprints[k][3]) >> 4;
        if (left <= x && x < right && top <= z && z < bottom) {
          *kind = k;
          if (k & 1) {
            if (left != FIELD_CELL(leader->x.fixed)) {
              *slot = i;
              return actor;
            }
          } else {
            if (top != FIELD_CELL(leader->z.fixed)) {
              *slot = i;
              return actor;
            }
          }
        }
      }
    }
    entry++;
  }
  return NULL;
}

extern struct ActorPlacement gYampiDesertCave_Placements[];
extern struct ActorPlacement gYampiDesertCaveGuardianPlacements[];
extern struct ActorPlacement gYampiDesertCaveCrystalPlacements[];
extern u16 gYampiDesertCaveCrystalStakes[];
extern struct Trigger gYampiDesertCave_Triggers[];
extern struct Trigger gYampiDesertCaveGuardianTriggers[];
extern struct Trigger gYampiDesertCaveCrystalTriggers[];

s32 YampiDesertCave_FindLogSlide(struct LogSlide *slide);
/// The leader pushes a log to where YampiDesertCave_FindLogSlide stopped it,
/// and its cells move with it.
void YampiDesertCave_PushLog(struct LogSlide slide);
void Teleport_StartShimmer(void);
void Teleport_StopShimmer(void);
s32 List_CountEntries(u16 *list);
void PaletteCycle_Fade(void);
void Pound_SinkTarget(u16 *list, s32 mode, s32 count);
void Teleport_PlaySparks(void);

void YampiDesertCave_PushLog(struct LogSlide slide) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  s32 w, h;
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct Actor *actor = Actor_GetById(slide.slot);
  // BUG: the leader's facing is taken as a 16.16 number, whose whole part is
  // always 0 for this 16-bit angle, where its sixteenth (FACING_INDEX) was
  // meant: every push takes direction 0 (east), so the log rolls with
  // animation 2 whichever way it goes and the leader's step after it is none.
#ifdef BUGFIX
  s32 direction = FACING_INDEX(leader->facing);
#else
  s32 direction = FIXEDtoINT(leader->facing);
#endif
  s32 kind = slide.kind;
  s32 speed = leader->speedLimit;
  s32 acceleration = leader->acceleration;
  s32 cell[3];
  FIXED from[3];
  s32 ax, az, bx, bz;

  from[0] = actor->x.fixed;
  from[2] = actor->z.fixed;
  cell[0] = FIELD_CELL(actor->x.fixed + toFIXED(gPushFootprints[kind][0]));
  cell[2] = FIELD_CELL(actor->z.fixed + toFIXED(gPushFootprints[kind][1]));
  w = (ABS(gPushFootprints[kind][0]) + ABS(gPushFootprints[kind][2])) >> 4;
  h = (ABS(gPushFootprints[kind][1]) + ABS(gPushFootprints[kind][3])) >> 4;
  PushedActor_FillGridAttributeRectangle(MAPGRID_LAYER_GROUND, cell[0], cell[2],
                                         w, h, CELL_BEHAVIOR_NORMAL);
  Actor_SetSpeedAndAcceleration(gMainRoutineWork.leaderActorId, 0.5, 0.1);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_PUSH);
  wait(15);
  // The leader follows the log half its way.
  Actor_SetDestinationOffset(gMainRoutineWork.leaderActorId,
                             FIXEDtoINT(slide.x - from[0]) >> 1,
                             FIXEDtoINT(slide.z - from[2]) >> 1);
  Actor_GetById(gMainRoutineWork.leaderActorId)->update =
      YampiDesertCave_StopLeaderAtLog;
  wait(4);
  if (direction >= FACING_INDEX(FACING_SOUTHWEST) &&
      direction <= FACING_INDEX(FACING_NORTH_NORTHEAST))
    Actor_SelectAnimation(actor, 3);
  else
    Actor_SelectAnimation(actor, 2);
  Event_PlaySound(SE_HEAVY_MOVE);
  Actor_SetSpeedAndAcceleration(slide.slot, 0.5, 0.1);
  Actor_SetMoveTarget(actor, slide.x, slide.y, slide.z);
  Actor_WaitForMoveById(gMainRoutineWork.leaderActorId);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_WALK);
  Actor_SetSpeedAndAcceleration(gMainRoutineWork.leaderActorId, 0.3, 0.1);
  // BUG: the leader's step after the push reads both halves from the step's
  // z: x is its sign and z its half (half of PUSH_STEP_X and PUSH_STEP_Z was
  // meant). With direction 0 both are 0.
  {
    s32 step = gPushLeaderSteps[direction];

#ifdef BUGFIX
    Actor_SetDestinationOffset(gMainRoutineWork.leaderActorId,
                               PUSH_STEP_X(step) / 2, PUSH_STEP_Z(step) / 2);
#else
    Actor_SetDestinationOffset(gMainRoutineWork.leaderActorId,
                               toFIXED(step) >> 31,
                               FIXEDtoINT(toFIXED(step)) >> 1);
#endif
  }
  if (slide.onPushed != NULL)
    ((void (*)(void))slide.onPushed)();
  Actor_WaitForMoveById(gMainRoutineWork.leaderActorId);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_STAND);
  leader->update = NULL;
  leader->speedLimit = speed;
  leader->acceleration = acceleration;
  FieldActor_WaitForMove(actor);
  Event_PlaySound(SE_STOP_EXIT_SE);
  Event_PlaySound(SE_HEAVY_STOP);
  Actor_SetPositionAndResetMotion(actor, slide.x, actor->y.fixed, slide.z);
  Actor_SelectAnimation(actor, 1);
  slide.x = FIELD_CELL(slide.x + toFIXED(gPushFootprints[kind][0]));
  slide.z = FIELD_CELL(slide.z + toFIXED(gPushFootprints[kind][1]));
  ax = FIELD_CELL(map->layers[MAPGRID_LAYER_MIDDLE].offsetX);
  az = FIELD_CELL(map->layers[MAPGRID_LAYER_MIDDLE].offsetY);
  bx = FIELD_CELL(map->layers[MAPGRID_LAYER_OVERHEAD].offsetX);
  bz = FIELD_CELL(map->layers[MAPGRID_LAYER_OVERHEAD].offsetY);
  MapGrid_CopyCellAttributes(slide.x, slide.z, w, h, ax + slide.x,
                             az + slide.z);
  MapGrid_CopyCellAttributes(slide.x + bx, slide.z + bz, w, h, slide.x + 64,
                             slide.z + 64);
  PushedActor_FillGridAttributeRectangle(MAPGRID_LAYER_GROUND, slide.x, slide.z,
                                         w, h, CELL_BEHAVIOR_WALL);
  PushedActor_FillGridAttributeRectangle(MAPGRID_LAYER_OVERHEAD, slide.x,
                                         slide.z, w, h, CELL_BEHAVIOR_WALL);
  cell[0] = FIELD_CELL(from[0] + toFIXED(gPushFootprints[kind][0]));
  cell[2] = FIELD_CELL(from[2] + toFIXED(gPushFootprints[kind][1]));
  MapGrid_CopyCellAttributes(ax + cell[0], az + cell[2], w, h, cell[0],
                             cell[2]);
  MapGrid_CopyCellAttributes(cell[0] + 64, cell[2] + 64, w, h, cell[0] + bx,
                             cell[2] + bz);
  Event_NoOp();
}

/// The leader's update while pushing: it stops at a wall or another log.
void YampiDesertCave_StopLeaderAtLog(struct Actor *actor) {
  FIXED position[3];
  struct Actor *hit;
  u32 i;
  s32 angle = FACING_INDEX(actor->facing);

  // The facing rounded to the nearest of east, south, west and north: two
  // sixteenths on, whole quarters kept, and back from sixteenths to a facing.
  angle = ((angle + 2) & 0xc) << 12;
  position[0] = actor->x.fixed;
  position[1] = actor->y.fixed;
  position[2] = actor->z.fixed;
  Vector_AddPolarOffset(toFIXED(MAPGRID_CELL_PIXELS), angle, position);
  hit = Actor_FindAtCell(position, 1);
  if (hit != NULL) {
    for (i = 0; i < PUSH_KINDS; i++) {
      if (((struct Sprite *)hit->sprite)->entries[0]->animationId ==
          gPushKinds[i])
        return;
    }
    Actor_SetPositionAndResetMotion(actor, actor->x.fixed, actor->y.fixed,
                                    actor->z.fixed);
  }
  position[0] = actor->x.fixed;
  position[1] = actor->y.fixed;
  position[2] = actor->z.fixed;
  if (MapGrid_CheckCollision(actor, position) > 0)
    Actor_SetPositionAndResetMotion(actor, actor->x.fixed, actor->y.fixed,
                                    actor->z.fixed);
}

#include "shared/teleport_shimmer.c"

void Actor_SetCellOpen(s32 id, u32 kind, s32 flag, s32 layer);

#include "shared/pillar_place.h"

// Finds the current target's table row and shrinks it.
#include "shared/pound_sink_target.h"

#include "shared/actor_set_cell_open.h"

/// Talking to Crystal, the Venus Djinni Scoop has dug out of the sand, starts
/// its battle.
void YampiDesertCave_FightCrystal(void) {
  Event_SetFlag(FLAG_YAMPI_DESERT_CAVE_CRYSTAL_DUG_UP);
  Event_RequestDjinnBattle(ACTOR_CRYSTAL, DJINNI_CRYSTAL);
  return;
}

/// Talking to the ripple racing through the sand of Crystal's room only puts a
/// question mark over Felix's head.
void YampiDesertCave_WonderAtRipple(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldActor_ShowEmote(gMainRoutineWork.leaderActorId,
                       EMOTE_IN_FRONT | EMOTE_QUESTION, 32);
  Event_End();
  return;
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return NULL; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  if (gMainRoutineWork.next == SCENE_YAMPI_DESERT_CAVE_GUARDIAN)
    return gYampiDesertCaveGuardianPlacements;
  if (gMainRoutineWork.next == SCENE_YAMPI_DESERT_CAVE_CRYSTAL)
    return gYampiDesertCaveCrystalPlacements;
  return gYampiDesertCave_Placements;
}

void YampiDesertCave_TryPushLog(void) {
  struct LogSlide slide;

  if (YampiDesertCave_FindLogSlide(&slide))
    YampiDesertCave_PushLog(slide);
}

/// Teleport on one of the chest room's circles: the circle shimmers and the
/// party arrives on the other, by its entrance.
#define DEFINE_TELEPORT_FROM_CIRCLE(name, entrance)                            \
  void name(void) {                                                            \
    Teleport_StartShimmer();                                                   \
    Teleport_PlaySparks();                                                     \
    gMainRoutineWork.teleportState = 2;                                        \
    Event_SetDestination(SCENE_YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST,          \
                         entrance);                                            \
    Event_Wait(40);                                                            \
    Teleport_StopShimmer();                                                    \
  }

/// Teleport cast on the pale carved circle in the small round chamber at the
/// north of the cave's first room (cells (14, 7) to (17, 9)) makes the circle
/// shimmer and carries the party to the circle in the inner cave.
DEFINE_TELEPORT_FROM_CIRCLE(YampiDesertCave_TeleportFromNorthCircle,
                            SOUTH_CIRCLE_ENTRANCE)

/// Teleport cast on the carved circle in the inner cave (cells (21, 56) to
/// (24, 58)) makes it shimmer and carries the party back to the circle in the
/// small chamber of the cave's first room, near the Water of Life's chest.
DEFINE_TELEPORT_FROM_CIRCLE(YampiDesertCave_TeleportFromSouthCircle,
                            NORTH_CIRCLE_ENTRANCE)

/// Burst breaks a pillar for good (flag) as it acts (phase 3).
#define DEFINE_BURST_PILLAR(name, flag, actorId)                               \
  void name(s32 phase) {                                                       \
    if (!Event_GetFlag(flag) && phase == 3) {                                  \
      Actor_SetPosition(actorId, 0, 0);                                        \
      GameFlag_Set(flag);                                                      \
    }                                                                          \
  }

/// Burst on the chest room's pillar breaks it for good. The pillar is taken
/// away and `FLAG_YAMPI_DESERT_CAVE_CHEST_ROOM_PILLAR_BURST` is set.
DEFINE_BURST_PILLAR(YampiDesertCave_BurstChestRoomPillar,
                    FLAG_YAMPI_DESERT_CAVE_CHEST_ROOM_PILLAR_BURST,
                    ACTOR_CHEST_ROOM_BURST_PILLAR)

/// Burst on the pillar in Crystal's room breaks it for good. The pillar is
/// taken away and `FLAG_YAMPI_DESERT_CAVE_CRYSTAL_ROOM_PILLAR_BURST` is set.
DEFINE_BURST_PILLAR(YampiDesertCave_BurstCrystalRoomPillar,
                    FLAG_YAMPI_DESERT_CAVE_CRYSTAL_ROOM_PILLAR_BURST,
                    ACTOR_CRYSTAL_ROOM_BURST_PILLAR)

/// Move takes a pillar: the cells it stood on open, as the room has them
/// without it.
void YampiDesertCave_StartMovingPillar(s32 unused, s32 id) {
  if (gMainRoutineWork.next == SCENE_YAMPI_DESERT_CAVE_GUARDIAN) {
    s32 row = 12;

    MapGrid_CopyCellAttributes(98, 3, 3, 3, 97, row);
    MapGrid_CopyCellAttributes(33, 12, 1, 1, 34, row);
  }
  if (gMainRoutineWork.next == SCENE_YAMPI_DESERT_CAVE_CRYSTAL) {
    if (id == ACTOR_EAST_PILLAR) {
      MapGrid_CopyCellAttributes(102, 15, 1, 1, 103, 15);
      MapGrid_CopyCellAttributes(43, 14, 1, 1, 39, 16);
    }
    if (id == ACTOR_WEST_PILLAR) {
      struct Actor *actor = Actor_GetById(ACTOR_WEST_PILLAR);

      s32 column = FIELD_CELL(actor->x.fixed);
      s32 row = FIELD_CELL(actor->z.fixed);

      MapGrid_CopyCellAttributes(7, 10, 1, 1, column, row);
    }
  }
}

/// Move sets a pillar down: where it opens the way it stays for good, and the
/// cells under it close again.
void YampiDesertCave_FinishMovingPillar(s32 unused, s32 id) {
  struct Actor *actor = Actor_GetById(id);

  if (gMainRoutineWork.next == SCENE_YAMPI_DESERT_CAVE_GUARDIAN) {
    s32 row = FIELD_CELL(actor->z.fixed);

    if (row == 13) {
      actor->motionFlags = 0;
      Event_SetFlag(FLAG_YAMPI_DESERT_CAVE_GUARDIAN_PILLAR_MOVED);
      MapGrid_CopyCellAttributes(32, 13, 1, 1, 34, row);
      MapGrid_CopyCellAttributes(95, 3, 3, 3, 97, 12);
    } else {
      row = 12;
      MapGrid_CopyCellAttributes(92, 3, 3, 3, 97, row);
      MapGrid_CopyCellAttributes(32, 13, 1, 1, 34, row);
    }
  }
  if (gMainRoutineWork.next == SCENE_YAMPI_DESERT_CAVE_CRYSTAL) {
    if (id == ACTOR_EAST_PILLAR) {
      s32 column;

      MapGrid_CopyCellAttributes(103, 16, 1, 1, 103, 15);
      column = FIELD_CELL(actor->x.fixed);
      if (column == 38) {
        Event_SetFlag(FLAG_YAMPI_DESERT_CAVE_EAST_PILLAR_MOVED);
        MapGrid_CopyCellAttributes(38, 15, 1, 1, column, 16);
      } else {
        MapGrid_CopyCellAttributes(38, 15, 1, 1, 39, 16);
      }
    }
    if (id == ACTOR_WEST_PILLAR) {
      if (FIELD_CELL(actor->x.fixed) == 10 && FIELD_CELL(actor->z.fixed) == 11)
        Event_SetFlag(FLAG_YAMPI_DESERT_CAVE_WEST_PILLAR_MOVED);
      MapGrid_CopyCellAttributes(12, 11, 1, 1, FIELD_CELL(actor->x.fixed),
                                 FIELD_CELL(actor->z.fixed));
    }
  }
}

/// As Sand melts Felix into the sand, the ways only a sunk Felix can take open
/// in the Water of Life chest's room and in Crystal's room.
void YampiDesertCave_CastSand(void) {
  Actor_GetById(gMainRoutineWork.leaderActorId)->layer = MAPGRID_LAYER_OVERHEAD;
  if (gMainRoutineWork.next == SCENE_YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST) {
    MapGrid_CopyCellAttributes(55, 71, 4, 1, 43, 70);
    MapGrid_CopyCellAttributes(43, 71, 4, 1, 55, 71);
  }
  if (gMainRoutineWork.next == SCENE_YAMPI_DESERT_CAVE_CRYSTAL) {
    MapGrid_CopyCellAttributes(110, 45, 5, 2, 90, 50);
    MapGrid_CopyCellAttributes(113, 47, 2, 1, 90, 52);
    MapGrid_CopyCellAttributes(107, 48, 3, 2, 90, 53);
    MapGrid_CopyCellAttributes(97, 58, 5, 2, 110, 45);
    MapGrid_CopyCellAttributes(97, 58, 2, 1, 113, 47);
    MapGrid_CopyCellAttributes(97, 58, 3, 2, 107, 48);
  }
}

/// As Sand fades and Felix rises out of the sand, those ways close again.
void YampiDesertCave_EndSand(void) {
  Actor_GetById(gMainRoutineWork.leaderActorId)->layer = MAPGRID_LAYER_GROUND;
  if (gMainRoutineWork.next == SCENE_YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST)
    MapGrid_CopyCellAttributes(43, 70, 4, 1, 55, 71);
  if (gMainRoutineWork.next == SCENE_YAMPI_DESERT_CAVE_CRYSTAL) {
    MapGrid_CopyCellAttributes(90, 50, 5, 2, 110, 45);
    MapGrid_CopyCellAttributes(90, 52, 2, 1, 113, 47);
    MapGrid_CopyCellAttributes(90, 53, 3, 2, 107, 48);
  }
}

/// Pound on the wooden stake in Crystal's room hammers it into the ground and
/// opens the way past it.
void YampiDesertCave_PoundStake(s32 mode, s32 count) {
  Pound_SinkTarget(gYampiDesertCaveCrystalStakes, mode, count);
  MapGrid_CopyCellAttributes(47, 12, 1, 1, 46, 12);
}

extern s16 gYampiDesertCave_RipplePath[];

/// Moves the ripple along its path, a row of gYampiDesertCave_RipplePath for
/// every RIPPLE_STEPS_PER_POINT steps of its counter, which counts on while
/// the player has control, whether the party walks or stands.
void YampiDesertCave_MoveSandRipple(void) {
  s32 step = FieldGameFlag_GetByte(FLAG_BYTE_YAMPI_DESERT_CAVE_RIPPLE_STEP);
  s32 index = (step / RIPPLE_STEPS_PER_POINT) * 2;
  s32 column = gYampiDesertCave_RipplePath[index];
  s32 row = gYampiDesertCave_RipplePath[index + 1];

  if (Actor_GetById(ACTOR_SAND_RIPPLE)->paused == 0) {
    Actor_SetPositionFX(ACTOR_SAND_RIPPLE, FIELD_HALF_CELL_ORIGIN(column),
                        FIELD_HALF_CELL_ORIGIN(row));
    if (Event_GetFlag(FLAG_FIELD_HANDLING_REQUESTS) == 0)
      step++;
    GameFlag_SetByte(FLAG_BYTE_YAMPI_DESERT_CAVE_RIPPLE_STEP, step);
  }
}

/// Scoop cast right beside the ripple racing through the sand of Crystal's
/// room stops it and digs up Crystal, the Venus Djinni, who drops to the
/// ground.
/// The ripple is stopped as Scoop acts (mode 1) and Crystal set down as it
/// ends (mode 2).
void YampiDesertCave_ScoopDjinni(s32 mode) {
  s32 index =
      (((s32)GameFlag_GetByte(FLAG_BYTE_YAMPI_DESERT_CAVE_RIPPLE_STEP)) /
       RIPPLE_STEPS_PER_POINT) *
      2;
  s16 column = gYampiDesertCave_RipplePath[index];
  s32 px = column * (MAPGRID_CELL_PIXELS / 2);
  s16 row = gYampiDesertCave_RipplePath[index + 1];
  s32 pz = row * (MAPGRID_CELL_PIXELS / 2);
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  // The Scoop's hand, where it digs: the ScoopWork's actor, which only
  // field_psynergy_scoop.c declares.
  struct Actor *scoopHand =
      *(struct Actor **)(Ram_HeapSlots->battleEffect + 0x1930);
  s32 x = 0, z = 0;

  if (scoopHand != NULL) {
    x = FIELD_CELL(scoopHand->x.fixed);
    z = FIELD_CELL(scoopHand->z.fixed);
  }
  if (px / MAPGRID_CELL_PIXELS != x || pz / MAPGRID_CELL_PIXELS != z)
    return;
  if (mode == 1) {
    Task_Remove(YampiDesertCave_MoveSandRipple);
    Actor_SetPosition(ACTOR_SAND_RIPPLE, 0, 0);
  }
  if (mode == 2) {
    struct Actor *crystal = Actor_GetById(ACTOR_CRYSTAL);

    Actor_SetPositionFX(ACTOR_CRYSTAL, FIELD_HALF_CELL_ORIGIN(column),
                        FIELD_HALF_CELL_ORIGIN(row));
    crystal->y.fixed = toFIXED(32);
    crystal->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    crystal->gravity = toFIXED(0.09);
    wait(50);
    {
      s16 *counter = &work->talk;
      s32 value = ACTOR_CRYSTAL;

      *counter = value;
    }
#if TLA_PATCH_1
    Event_ClearFlag(FLAG_YAMPI_DESERT_CAVE_RIPPLE_RUNNING);
#endif
  }
}

/// Stepping up to Valukar in his chamber (cells (11, 11) to (13, 11)), the
/// screen glows warm as he says "I am the guardian of fire. If you want my
/// power, prove yourself in my crucible of flame.", then he comes at Felix and
/// his battle begins, one the party cannot run from.
void YampiDesertCave_MeetGuardianOfFire(void) {
  struct Actor *leader;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_StartRepeatedMotion(ACTOR_VALUKAR, 2);
  Event_Wait(20);
  PaletteFade_ApplySource(PALETTE_TINT_NONE, 0);
  PaletteFade_SetTarget(PALETTE_TINT_WARM, 1);
  PaletteFade_StartInterpolation(60);
  Event_Wait(60);
  // "I am the guardian of fire. If you want my power, prove yourself in my
  // crucible of flame."
  EventMessage_ShowCentered((s32)MsgIAmThe, 0, 6);
  PaletteFade_SetTarget(PALETTE_TINT_NONE, 0);
  PaletteFade_StartInterpolation(60);
  Event_Wait(60);
  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  if (leader != 0)
    Actor_SetDestination(ACTOR_VALUKAR, leader->x.part[0].integer,
                         leader->z.part[0].integer);
  Event_Wait(20);
  gMainRoutineWork.pendingDjinni = IF_FLAG_SET(FLAG_VALUKAR_BEATEN);
  Battle_SetType(BATTLE_TYPE_NO_ESCAPE);
  gMainRoutineWork.battleBackground = (s32)&ResourceId_LavaCaveBackdrop;
  Field_RequestBattleFromEncounterTable(ENCOUNTER_TABLE_HIDDEN_BOSSES,
                                        HIDDEN_BOSS_VALUKAR);
}

/// Scoop at the west end of a passage in the part of the inner cave below
/// Valukar's chamber (cell (5, 38)) digs up the Mythril Silver: "Felix got
/// some Mythril Silver."
void YampiDesertCave_DigUpMythrilSilver(s32 mode) {
  Event_DigUpTreasure(mode, ACTOR_MYTHRIL_SILVER,
                      FLAG_YAMPI_DESERT_CAVE_MYTHRIL_SILVER_DUG_UP);
}

const struct Trigger *Scene_GetTriggers(void) {
  if (gMainRoutineWork.next == SCENE_YAMPI_DESERT_CAVE_GUARDIAN)
    return gYampiDesertCaveGuardianTriggers;
  if (gMainRoutineWork.next == SCENE_YAMPI_DESERT_CAVE_CRYSTAL)
    return gYampiDesertCaveCrystalTriggers;
  return gYampiDesertCave_Triggers;
}

extern s16 gYampiDesertCave_CircleSlots[];
extern u16 gTeleportCircle_Colors[];
extern u16 gTeleportCircle_ShimmerColors[];

void Actor_SetWideSprite(s32 id, s32 wide);

/// Sets up Yampi Desert Cave's three rooms as each one loads: the first room's
/// palette cycle and Retreat destination, the third room's background
/// priorities, and in each room the objects its flags place.
s32 Scene_Initialize(void) {
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  FieldEffect_StartBlankOverlay(0);
  if (gMainRoutineWork.next == SCENE_YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST) {
    Palette_SetColorsFade(gYampiDesertCave_CircleSlots, gTeleportCircle_Colors,
                          gTeleportCircle_ShimmerColors,
                          gYampiDesertCave_CircleCycleSlots);
    Actor_DisableById(ACTOR_CHEST_ROOM_FIRST_LOG);
    Actor_DisableById(ACTOR_CHEST_ROOM_SECOND_LOG);
    Actor_DisableById(ACTOR_CHEST_ROOM_THIRD_LOG);
    YampiDesertCave_SetLogCells(ACTOR_CHEST_ROOM_FIRST_LOG);
    YampiDesertCave_SetLogCells(ACTOR_CHEST_ROOM_SECOND_LOG);
    YampiDesertCave_SetLogCells(ACTOR_CHEST_ROOM_THIRD_LOG);
    if (Event_GetFlag(FLAG_YAMPI_DESERT_CAVE_CHEST_ROOM_PILLAR_BURST)) {
      Actor_SetPosition(ACTOR_CHEST_ROOM_BURST_PILLAR, 0, 0);
      MapGrid_CopyCellAttributes(32, 13, 1, 1, 34, 13);
    }
    if (gMainRoutineWork.entrance ==
        YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST_FROM_YAMPI_DESERT_TRAINERS_WHIP_CHEST) {
      if (Event_GetFlag(FLAG_YAMPI_DESERT_ENTERED_BY_GUARDIAN_RING_CHEST)) {
        gMainRoutineWork.retreatScene = SCENE_YAMPI_DESERT_GUARDIAN_RING_CHEST;
        gMainRoutineWork.retreatEntrance =
            YAMPI_DESERT_GUARDIAN_RING_CHEST_FROM_WORLD_MAP;
      } else if (Event_GetFlag(
                     FLAG_YAMPI_DESERT_ENTERED_BY_TRAINERS_WHIP_CHEST)) {
        gMainRoutineWork.retreatScene = SCENE_YAMPI_DESERT_TRAINERS_WHIP_CHEST;
        gMainRoutineWork.retreatEntrance =
            YAMPI_DESERT_TRAINERS_WHIP_CHEST_FROM_WORLD_MAP;
      } else if (Event_GetFlag(
                     FLAG_YAMPI_DESERT_ENTERED_BY_HIDDEN_PSYNERGY_STONE)) {
        gMainRoutineWork.retreatScene =
            SCENE_YAMPI_DESERT_HIDDEN_PSYNERGY_STONE;
        gMainRoutineWork.retreatEntrance =
            YAMPI_DESERT_HIDDEN_PSYNERGY_STONE_FROM_WORLD_MAP;
      }
    }
  }
  if (gMainRoutineWork.next == SCENE_YAMPI_DESERT_CAVE_GUARDIAN) {
    if (Event_GetFlag(FLAG_YAMPI_DESERT_CAVE_GUARDIAN_PILLAR_MOVED)) {
      Actor_SetPosition(ACTOR_GUARDIAN_ROOM_PILLAR, 552, 216);
      MapGrid_CopyCellAttributes(32, 13, 1, 1, 34, 13);
      MapGrid_CopyCellAttributes(95, 3, 3, 3, 98, 13);
    } else {
      MapGrid_CopyCellAttributes(32, 13, 1, 1, 34, 12);
    }
    if (!Event_GetFlag(FLAG_YAMPI_DESERT_CAVE_MYTHRIL_SILVER_DUG_UP))
      Actor_SetWideSprite(ACTOR_MYTHRIL_SILVER, 1);
  }
  if (gMainRoutineWork.next == SCENE_YAMPI_DESERT_CAVE_CRYSTAL) {
    if ((u16)(gMainRoutineWork.entrance - 3) <= 2) {
      REG_BG1CNT &= ~BGCNT_PRIORITY(3);
      REG_BG1CNT |= BGCNT_PRIORITY(1);
      REG_BG2CNT &= ~BGCNT_PRIORITY(3);
      REG_BG2CNT |= BGCNT_PRIORITY(3);
      REG_BG3CNT &= ~BGCNT_PRIORITY(3);
      REG_BG3CNT |= BGCNT_PRIORITY(3);
    }
    Actor_DisableById(ACTOR_CRYSTAL_ROOM_FIRST_LOG);
    Actor_DisableById(ACTOR_CRYSTAL_ROOM_SECOND_LOG);
    Actor_DisableById(ACTOR_CRYSTAL_ROOM_THIRD_LOG);
    Actor_DisableById(ACTOR_CRYSTAL_ROOM_FOURTH_LOG);
    YampiDesertCave_SetLogCells(ACTOR_CRYSTAL_ROOM_FIRST_LOG);
    YampiDesertCave_SetLogCells(ACTOR_CRYSTAL_ROOM_SECOND_LOG);
    YampiDesertCave_SetLogCells(ACTOR_CRYSTAL_ROOM_THIRD_LOG);
    YampiDesertCave_SetLogCells(ACTOR_CRYSTAL_ROOM_FOURTH_LOG);
    Pillar_Place(gYampiDesertCaveCrystalStakes);
    if (!Event_GetFlag(FLAG_YAMPI_DESERT_CAVE_STAKE_POUNDED))
      MapGrid_CopyCellAttributes(48, 12, 1, 1, 46, 12);
    if (Event_GetFlag(FLAG_YAMPI_DESERT_CAVE_CRYSTAL_ROOM_PILLAR_BURST))
      Actor_SetPosition(ACTOR_CRYSTAL_ROOM_BURST_PILLAR, 0, 0);
    if (Event_GetFlag(FLAG_YAMPI_DESERT_CAVE_WEST_PILLAR_MOVED)) {
      Actor_SetPosition(ACTOR_WEST_PILLAR, 168, 184);
      MapGrid_CopyCellAttributes(12, 11, 1, 1, 10, 11);
    } else {
#if TLA_PATCH_1
      struct Actor *actor = Actor_GetById(ACTOR_WEST_PILLAR);

      MapGrid_CopyCellAttributes(12, 11, 1, 1, FIELD_CELL(actor->x.fixed),
                                 FIELD_CELL(actor->z.fixed));
#else
      MapGrid_CopyCellAttributes(12, 11, 1, 1, 8, 10);
#endif
    }
    if (Event_GetFlag(FLAG_YAMPI_DESERT_CAVE_EAST_PILLAR_MOVED)) {
      Actor_SetPosition(ACTOR_EAST_PILLAR, 616, 264);
      MapGrid_CopyCellAttributes(38, 15, 1, 1, 38, 16);
    } else {
      MapGrid_CopyCellAttributes(38, 15, 1, 1, 39, 16);
    }
    if (!Event_GetFlag(FLAG_DJINNI(DJINNI_CRYSTAL)) &&
        !Event_GetFlag(FLAG_DJINNI_HIDING(DJINNI_CRYSTAL))) {
      s32 step = FieldGameFlag_GetByte(FLAG_BYTE_YAMPI_DESERT_CAVE_RIPPLE_STEP);
      s32 placed = Event_GetFlag(FLAG_YAMPI_DESERT_CAVE_CRYSTAL_DUG_UP);

      if (placed) {
        step = step / RIPPLE_STEPS_PER_POINT * 2;
        Actor_SetPosition(ACTOR_SAND_RIPPLE, 0, 0);
        Actor_SetPositionFX(
            ACTOR_CRYSTAL,
            FIELD_HALF_CELL_ORIGIN(gYampiDesertCave_RipplePath[step]),
            FIELD_HALF_CELL_ORIGIN(gYampiDesertCave_RipplePath[step + 1]));
      } else {
        if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION))
          GameFlag_SetByte(FLAG_BYTE_YAMPI_DESERT_CAVE_RIPPLE_STEP, 0);
        Task_Add(YampiDesertCave_MoveSandRipple, TASK_PASS_FRAME);
        Event_SetFlag(FLAG_YAMPI_DESERT_CAVE_RIPPLE_RUNNING);
        Actor_SetSpritePriority(ACTOR_SAND_RIPPLE, 3);
        Actor_GetById(ACTOR_SAND_RIPPLE)->collisionFlags = 0;
      }
    }
  }
  FieldEffect_StartBlankOverlay(0);
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

#undef DEFINE_TELEPORT_FROM_CIRCLE
#undef DEFINE_BURST_PILLAR

void YampiDesertCave_MeetGuardianOfFire(void);

// clang-format off
#include "shared/push_kinds.c"

#include "shared/push_leader_steps.c"

/// The ripple's path, a column and a row in half cells (8 pixels) for each
/// thirty-two steps: it shows at four places in turn, and between them lies at
/// the origin, out of sight.
s16 gYampiDesertCave_RipplePath[] = {
    101, 39, 0, 0, 97, 35, 0, 0,
    99, 37, 0, 0, 95, 33, 0, 0,
};

/// The background palette entries the Teleport circles are drawn in.
s16 gYampiDesertCave_CircleSlots[] = {
    9, 10, 11, 12, 13, 14, 15, -1,
};

#include "shared/teleport_circle_colors.c"

/// The shimmer's steps: for each, the palette entries its colours go to.
u16 gYampiDesertCave_CircleCycleSlots[] = {
    14, 9,  10, 11, 12, 13,
    13, 14, 9,  10, 11, 12,
    12, 13, 14, 9,  10, 11,
    11, 12, 13, 14, 9,  10,
    10, 11, 12, 13, 14, 9,
    9,  10, 11, 12, 13, 14,
};

#include "shared/scene_entrances_default.c"

u32 gSceneExits[] = {
    SCENE_EXITS(YampiDesertCaveWaterOfLifeChest),
    SCENE_EXIT(YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST_TO_YAMPI_DESERT_TRAINERS_WHIP_CHEST, YampiDesertTrainersWhipChest, YAMPI_DESERT_TRAINERS_WHIP_CHEST_FROM_YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST),
    SCENE_EXIT(YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST_TO_YAMPI_DESERT_CAVE_CRYSTAL_SOUTH, YampiDesertCaveCrystal, YAMPI_DESERT_CAVE_CRYSTAL_FROM_YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST_SOUTH),
    SCENE_EXIT(3, YampiDesertCaveWaterOfLifeChest, 6),
    SCENE_EXIT(4, YampiDesertCaveWaterOfLifeChest, 5),
    SCENE_EXIT(5, YampiDesertCaveWaterOfLifeChest, 4),
    SCENE_EXIT(6, YampiDesertCaveWaterOfLifeChest, 3),
    SCENE_EXIT(YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST_TO_YAMPI_DESERT_CAVE_CRYSTAL_NORTH, YampiDesertCaveCrystal, YAMPI_DESERT_CAVE_CRYSTAL_FROM_YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST_NORTH),
    SCENE_EXIT(YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST_TO_YAMPI_DESERT_CAVE_GUARDIAN, YampiDesertCaveGuardian, YAMPI_DESERT_CAVE_GUARDIAN_FROM_YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST),
    SCENE_EXITS(YampiDesertCaveGuardian),
    SCENE_EXIT(YAMPI_DESERT_CAVE_GUARDIAN_TO_YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST, YampiDesertCaveWaterOfLifeChest, YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST_FROM_YAMPI_DESERT_CAVE_GUARDIAN),
    SCENE_EXIT(2, YampiDesertCaveGuardian, 17),
    SCENE_EXIT(YAMPI_DESERT_CAVE_GUARDIAN_TO_YAMPI_DESERT_CAVE_CRYSTAL_NORTHWEST, YampiDesertCaveCrystal, YAMPI_DESERT_CAVE_CRYSTAL_FROM_YAMPI_DESERT_CAVE_GUARDIAN_NORTHWEST),
    SCENE_EXIT(YAMPI_DESERT_CAVE_GUARDIAN_TO_YAMPI_DESERT_CAVE_CRYSTAL_SOUTH, YampiDesertCaveCrystal, YAMPI_DESERT_CAVE_CRYSTAL_FROM_YAMPI_DESERT_CAVE_GUARDIAN_SOUTH),
    SCENE_EXIT(YAMPI_DESERT_CAVE_GUARDIAN_TO_YAMPI_DESERT_CAVE_CRYSTAL_SOUTHEAST, YampiDesertCaveCrystal, YAMPI_DESERT_CAVE_CRYSTAL_FROM_YAMPI_DESERT_CAVE_GUARDIAN_SOUTHEAST),
    SCENE_EXIT(17, YampiDesertCaveGuardian, 2),
    SCENE_EXITS(YampiDesertCaveCrystal),
    SCENE_EXIT(YAMPI_DESERT_CAVE_CRYSTAL_TO_YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST_SOUTH, YampiDesertCaveWaterOfLifeChest, YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST_FROM_YAMPI_DESERT_CAVE_CRYSTAL_SOUTH),
    SCENE_EXIT(2, YampiDesertCaveCrystal, 3),
    SCENE_EXIT(3, YampiDesertCaveCrystal, 2),
    SCENE_EXIT(4, YampiDesertCaveCrystal, 7),
    SCENE_EXIT(5, YampiDesertCaveCrystal, 6),
    SCENE_EXIT(6, YampiDesertCaveCrystal, 5),
    SCENE_EXIT(7, YampiDesertCaveCrystal, 4),
    SCENE_EXIT(YAMPI_DESERT_CAVE_CRYSTAL_TO_YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST_NORTH, YampiDesertCaveWaterOfLifeChest, YAMPI_DESERT_CAVE_WATER_OF_LIFE_CHEST_FROM_YAMPI_DESERT_CAVE_CRYSTAL_NORTH),
    SCENE_EXIT(9, YampiDesertCaveCrystal, 10),
    SCENE_EXIT(10, YampiDesertCaveCrystal, 9),
    SCENE_EXIT(YAMPI_DESERT_CAVE_CRYSTAL_TO_YAMPI_DESERT_CAVE_GUARDIAN_SOUTHEAST, YampiDesertCaveGuardian, YAMPI_DESERT_CAVE_GUARDIAN_FROM_YAMPI_DESERT_CAVE_CRYSTAL_SOUTHEAST),
    SCENE_EXIT(YAMPI_DESERT_CAVE_CRYSTAL_TO_YAMPI_DESERT_CAVE_GUARDIAN_SOUTH, YampiDesertCaveGuardian, YAMPI_DESERT_CAVE_GUARDIAN_FROM_YAMPI_DESERT_CAVE_CRYSTAL_SOUTH),
    SCENE_EXIT(YAMPI_DESERT_CAVE_CRYSTAL_TO_YAMPI_DESERT_CAVE_GUARDIAN_NORTHWEST, YampiDesertCaveGuardian, YAMPI_DESERT_CAVE_GUARDIAN_FROM_YAMPI_DESERT_CAVE_CRYSTAL_NORTHWEST),
    SCENE_EXITS_END,
};

/// A pushed pillar or stake stands still where it was placed.
u32 gYampiDesertCave_Settle[] = {
    SC_SET(SW_MOTION, 0),
    SC_END,
};

/// The Water of Life chest's room: three logs and the pillar Burst breaks.
struct ActorPlacement gYampiDesertCave_Placements[] = {
    ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 888, 0, 336, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 832, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 912, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BURST_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 712, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};
// clang-format on

/// Valukar's room: the pillar Move pushes, and Valukar until he is beaten.
struct ActorPlacement gYampiDesertCaveGuardianPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS,
                    (s32)gYampiDesertCave_Settle, 552, 0, 200, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_VALUKAR, FLAG_VALUKAR_BEATEN, MOVEMENT_TYPE_STAND,
                    200, 0, 152, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/// Crystal's room, in YampiDesertCaveCrystalActorId order.
struct ActorPlacement gYampiDesertCaveCrystalPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_NORTH_SOUTH, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_CLEAR_COLLISION, 712, 0, 752, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_WEST_EAST, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_CLEAR_COLLISION, 752, 0, 744, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_NORTH_SOUTH, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_CLEAR_COLLISION, 728, 0, 784, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_WEST_EAST, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_CLEAR_COLLISION, 800, 0, 760, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS,
                    (s32)gYampiDesertCave_Settle, 632, 0, 264, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS,
                    (s32)gYampiDesertCave_Settle, 136, 0, 168, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS,
                    (s32)gYampiDesertCave_Settle, 744, 0, 200, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BURST_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    264, 0, 632, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SAND_RIPPLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_VENUS_DJINNI, FLAG_DJINNI(DJINNI_CRYSTAL),
                    MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH,
                    TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

/// The stake Pound sinks in Crystal's room: its actor, its kind and the flag
/// set once it is sunk, as Pound_SinkTarget reads them.
u16 gYampiDesertCaveCrystalStakes[] ALIGNED(4) = {
    ACTOR_STAKE,
    0,
    FLAG_YAMPI_DESERT_CAVE_STAKE_POUNDED,
    ACTOR_LIST_END,
};

/// The Water of Life chest's room: its exits, the logs, the pillar, the two
/// Teleport circles and Sand.
struct Trigger gYampiDesertCave_Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {STEP_TRIGGER_WALKING, CELL_AROUND_LOG, CONDITION_ALWAYS,
     EVENT(YampiDesertCave_TryPushLog)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
     CELL_BURST_PILLAR, FLAG_YAMPI_DESERT_CAVE_CHEST_ROOM_PILLAR_BURST,
     EVENT(YampiDesertCave_BurstChestRoomPillar)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_TELEPORT, PSYNERGY_PHASE_AFTER_CAST),
     NORTH_CIRCLE_CELLS, CONDITION_ALWAYS,
     EVENT(YampiDesertCave_TeleportFromNorthCircle)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_TELEPORT, PSYNERGY_PHASE_AFTER_CAST),
     SOUTH_CIRCLE_CELLS, CONDITION_ALWAYS,
     EVENT(YampiDesertCave_TeleportFromSouthCircle)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0,
     CONDITION_ALWAYS, EVENT(YampiDesertCave_CastSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS,
     EVENT(YampiDesertCave_EndSand)},
    {TRIGGER_TABLE_END},
};

/// Valukar's room: its exits, Valukar's step, the pillar, Sand and Scoop on
/// the buried Mythril Silver.
struct Trigger gYampiDesertCaveGuardianTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, 15},
    {TRIGGER_EXIT, 16, CONDITION_ALWAYS, 16},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 17, CONDITION_ALWAYS, 17},
    {TRIGGER_STEP, CELL_BEFORE_VALUKAR, FLAG_VALUKAR_BEATEN,
     EVENT(YampiDesertCave_MeetGuardianOfFire)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_GUARDIAN_ROOM_PILLAR, FLAG_YAMPI_DESERT_CAVE_GUARDIAN_PILLAR_MOVED,
     EVENT(YampiDesertCave_StartMovingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_GUARDIAN_ROOM_PILLAR,
     FLAG_YAMPI_DESERT_CAVE_GUARDIAN_PILLAR_MOVED,
     EVENT(YampiDesertCave_FinishMovingPillar)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0,
     CONDITION_ALWAYS, EVENT(YampiDesertCave_CastSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS,
     EVENT(YampiDesertCave_EndSand)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS),
     CELL_MYTHRIL_SILVER, FLAG_YAMPI_DESERT_CAVE_MYTHRIL_SILVER_DUG_UP,
     EVENT(YampiDesertCave_DigUpMythrilSilver)},
    {TRIGGER_TABLE_END},
};

/// Crystal's room: its exits, the ripple and Crystal, the logs, the pillars,
/// the stake, Sand and Scoop on the ripple.
struct Trigger gYampiDesertCaveCrystalTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_TALK, ACTOR_SAND_RIPPLE, CONDITION_ALWAYS,
     EVENT(YampiDesertCave_WonderAtRipple)},
    {TRIGGER_TALK, ACTOR_CRYSTAL, CONDITION_ALWAYS,
     EVENT(YampiDesertCave_FightCrystal)},
    {STEP_TRIGGER_WALKING, CELL_AROUND_LOG, CONDITION_ALWAYS,
     EVENT(YampiDesertCave_TryPushLog)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS),
     CELL_RIPPLE_SAND, FLAG_YAMPI_DESERT_CAVE_CRYSTAL_ROOM_PILLAR_BURST,
     EVENT(YampiDesertCave_BurstCrystalRoomPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_EAST_PILLAR, FLAG_YAMPI_DESERT_CAVE_EAST_PILLAR_MOVED,
     EVENT(YampiDesertCave_StartMovingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_EAST_PILLAR,
     FLAG_YAMPI_DESERT_CAVE_EAST_PILLAR_MOVED,
     EVENT(YampiDesertCave_FinishMovingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_WEST_PILLAR, FLAG_YAMPI_DESERT_CAVE_WEST_PILLAR_MOVED,
     EVENT(YampiDesertCave_StartMovingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_WEST_PILLAR,
     FLAG_YAMPI_DESERT_CAVE_WEST_PILLAR_MOVED,
     EVENT(YampiDesertCave_FinishMovingPillar)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS,
     EVENT(YampiDesertCave_StartMovingPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS,
     EVENT(YampiDesertCave_FinishMovingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_STAKE,
     FLAG_YAMPI_DESERT_CAVE_STAKE_POUNDED, EVENT(YampiDesertCave_PoundStake)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0,
     CONDITION_ALWAYS, EVENT(YampiDesertCave_CastSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS,
     EVENT(YampiDesertCave_EndSand)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS),
     CELL_RIPPLE_SAND, IF_FLAG_SET(FLAG_YAMPI_DESERT_CAVE_RIPPLE_RUNNING),
     EVENT(YampiDesertCave_ScoopDjinni)},
    {TRIGGER_TABLE_END},
};
