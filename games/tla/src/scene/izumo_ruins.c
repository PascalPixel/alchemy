/// @file
/// The Izumo Ruins, the grey caves under the ring of giant stones in Izumo,
/// reached by the ladder Reveal shows in the ring's centre. They are two
/// scenes that share this overlay (SceneId_IzumoRuinsFirstRoom,
/// SceneId_IzumoRuinsSecondRoom), each of several rooms.
///
/// In the first room's south part, where the ladder comes down, two wooden
/// stakes stand between two puddles across the floor, with patches of sand
/// beside them: Sand takes Felix through the sand (IzumoRuins_SinkIntoSand),
/// Frost freezes the puddles into pillars of ice (IzumoRuins_FreezePuddle) and
/// Pound drives the stakes down (IzumoRuins_PoundStake). Its north part has a
/// third stake by a strip of sand and a stone pillar Move pulls along
/// (IzumoRuins_FinishMovingPillar), and grey pillars crumble under a second
/// step (shared/crumbling_pillar.h). The second scene, a floor down, holds the
/// Phantasmal Mail, two more stakes and a green pool: Parch dries the pool
/// (IzumoRuins_ParchThePool) and opens the way to the room of the stone tablet
/// that teaches Ulysses, which stands in a ring of six stone pillars.
#include "system/iwram_call.h"
#include "field/field_sequence.h"
#include "field/actor.h"
#include "system/main_routine_work.h"
#include "system/ram_buffer.h"
#include "types.h"
#include "field/event_inline.h"
#include "field/field.h"
#include "field/object.h"
#include "scene/scene.h"
#include "system/input.h"
#include "text/msg_ids.h"
#include "system/frame.h"
#include "field/scene_tables.h"
#include "sprite_ids.h"

void Pound_SinkTarget(u16 *list, s32 mode, s32 count);
void IzumoRuins_ShowStakeState(struct Actor *actor);
void Event_End(void);
void Actor_DisableById(s32 id);
void FlagBlock_ApplyFlags(const void *data);
void Actor_LiftTargetBlock(const void *data);
void Actor_PlaceMovedBlock(void);

extern s16 gIzumoRuins_FirstRoomPillars[];
extern s16 gIzumoRuins_SecondRoomPillars[];
extern s16 gIzumoRuins_FirstRoomStakes[];
extern s16 gIzumoRuins_SecondRoomStakes[];
extern s16 gIzumoRuins_Puddles[];
extern struct SceneRegion gIzumoRuins_FirstRoomRegions[];
extern struct SceneRegion gIzumoRuins_SecondRoomRegions[];
extern struct ActorPlacement gIzumoRuins_ActorPlacements[];
extern struct ActorPlacement gIzumoRuins_FirstRoomActorPlacements[];
extern struct ActorPlacement gIzumoRuins_SecondRoomActorPlacements[];
extern struct Trigger gIzumoRuins_FirstRoomTriggers[];
extern struct Trigger gIzumoRuins_SecondRoomTriggers[];

void IzumoRuins_PlaceStakes(u16 *list);
void Dust_Scatter(s32 mode);
void Dust_Burst(void);
void MapGrid_CenterOnFollowedPoint(void);
s32 Actor_GetCellIndex(struct Actor *actor);
void Actor_PlaceBlocks(const void *data);
void FieldEffect_StartBlankOverlay(s32 value);
void FlagBlock_PlaceAll(const void *data);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
#include "field/map_grid.h"
#include "shared/trial_road.h"
#include "system/system.h"
#include "field/field_sprite.h"
#include "system/field_effect.h"
#include "field/field_effect.h"
#include "flags.h"
#include "shared/tables.h"
#include "battle/battle.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "field/field_sequence_event.h"
#include "field/call.h"
#include "field/actor_script.h"

struct Actor;
s32 MapGrid_GetTerrainKind(s32 layer, s32 x, s32 z);
s32 Field_TryJumpForward(void);
struct Actor *Actor_FindAtCell(s32 *position, s32 flags);
void Leader_SetCellShapeAt(s32 level, s32 layer);

void Actor_SetCellOpen(s32 id, u32 kind, s32 flag, s32 layer);

#include "shared/crumbling_pillar.h"

#include "../field/field_effect_advance_motion_priority.c"

#include "shared/dust.h"

s32 MapCollision_FindOrAddTag(s32 terrainId, s32 value, s32 height);

#include "shared/actor_set_cell_shape.h"

#include "shared/leader_set_cell_shape.h"

#include "../field/actor_set_sprite_priority.c"

#include "../field/field_effect_move.c"

#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#include "../field/field_effect_spawn.c"

#include "text/msg_ids.h"

struct Actor;
extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];
extern u32 gFieldEffect_LongScript[];

/// Set as Parch dries the green pool on the way to the room of the stone
/// tablet.
#define FLAG_IZUMO_RUINS_POOL_PARCHED 0x941
/// The green pool's cells, which answer Parch.
enum { CELL_POOL = 50 };

/// Both scenes' first two actors: wooden stakes Pound drives down
/// (gIzumoRuins_FirstRoomStakes, gIzumoRuins_SecondRoomStakes), at cells
/// (10, 51) and (12, 51) in the first and (27, 34) and (25, 34) in the second.
enum { ACTOR_FIRST_STAKE = ACTOR_FIRST_PLACED, ACTOR_SECOND_STAKE };

/// The first scene's other actors: the puddles Frost freezes, west and east of
/// its first two stakes (cells (8, 51) and (14, 51)), a third stake in the
/// north part by the strip of sand (cell (43, 15)), and the stone pillar Move
/// pulls, at the north end (cell (32, 5)).
enum {
  ACTOR_WEST_PUDDLE = ACTOR_FIRST_PLACED + 2,
  ACTOR_EAST_PUDDLE,
  ACTOR_THIRD_STAKE,
  ACTOR_FIRST_ROOM_PILLAR
};

/// The second scene's six stone pillars Move pushes, in a ring round the stone
/// tablet in its room in the east.
enum {
  ACTOR_RING_NORTHWEST_PILLAR = ACTOR_FIRST_PLACED + 2,
  ACTOR_RING_NORTHEAST_PILLAR,
  ACTOR_RING_WEST_PILLAR,
  ACTOR_RING_SOUTHWEST_PILLAR,
  ACTOR_RING_SOUTHEAST_PILLAR,
  ACTOR_RING_EAST_PILLAR
};

/// Set while Pound has driven a stake into the ground: each scene's first and
/// second, and the first scene's third.
#define FLAG_FIRST_STAKE_POUNDED FLAG_SCENE_TEMP_00
#define FLAG_SECOND_STAKE_POUNDED FLAG_SCENE_TEMP_01
#define FLAG_THIRD_STAKE_POUNDED FLAG_SCENE_TEMP_02
/// Set once Frost has frozen a puddle of the first scene.
#define FLAG_WEST_PUDDLE_FROZEN FLAG_SCENE_TEMP_03
#define FLAG_EAST_PUDDLE_FROZEN FLAG_SCENE_TEMP_04

/// The cell between the ring's two south pillars, south of the stone tablet:
/// kept a wall while Move carries a pillar, so that none is set down there.
#define RING_SOUTH_GAP_COLUMN 50
#define RING_SOUTH_GAP_ROW 13

/// How far a standing stake's top rises over the ground under it, in height
/// steps of 8 pixels.
#define STAKE_HEIGHT_STEPS 4

#include "../field/map_cell_get.c"

#include "shared/map_cell_set.c"

void MapGrid_SetCellHeight(s32 x, s32 z, s32 layer, s32 height);

/// Fits the cell under a stake to whether it stands or is driven down (its
/// flag, kept in its timer). Standing, its top is a step STAKE_HEIGHT_STEPS
/// above the floor that Felix jumps onto and across, and under Sand it blocks
/// the way unless the floor round it is sand. Driven down, the cell is floor
/// again, closed to Felix under the sand. A driven stake is drawn behind
/// whoever walks over it.
void IzumoRuins_ShowStakeState(struct Actor *actor) {
  s32 column = FIELD_CELL(actor->x.fixed);
  s32 row = FIELD_CELL(actor->z.fixed);
  s32 lower = MapCell_GetAttributes(MAPGRID_LAYER_GROUND, column, row);
  s32 upper = MapCell_GetAttributes(MAPGRID_LAYER_OVERHEAD, column, row);
  s32 height = CELL_HEIGHT_STEPS(
      Map_GetTerrainHeight(actor->layer, actor->x.fixed, actor->z.fixed));

  if (Event_GetFlag(actor->timer)) {
    upper |= CELL_ATTRIBUTES(CELL_BEHAVIOR_WALL, 0);
    lower &= ~MAP_CELL_JUMP;
    if (CELL_HEIGHT_STEPS(actor->groundHeight) != height)
      height -= STAKE_HEIGHT_STEPS;
    Scene_SetActorSpritePriority((struct Actor *)actor, 3);
  } else {
    if (CELL_CODE(lower) != CELL_BEHAVIOR_SAND)
      upper = CELL_ATTRIBUTES(CELL_BEHAVIOR_WALL, upper & CELL_ATTRIBUTE_BITS);
    else
      upper = CELL_ATTRIBUTES(CELL_BEHAVIOR_SAND, upper & CELL_ATTRIBUTE_BITS);
    lower |= MAP_CELL_JUMP;
    height += STAKE_HEIGHT_STEPS;
    Scene_SetActorSpritePriority((struct Actor *)actor, 2);
  }
  MapCell_SetAttributes(MAPGRID_LAYER_GROUND, column, row, lower);
  MapCell_SetAttributes(MAPGRID_LAYER_OVERHEAD, column, row, upper);
  MapGrid_SetCellHeight(FIELD_CELL_ORIGIN(column), FIELD_CELL_ORIGIN(row),
                        actor->layer, height);
}

/// Places Pound's stakes from a list of rows (actor, unused, flag), ended by
/// -1: each standing, or driven down if its flag is set.
void IzumoRuins_PlaceStakes(u16 *list) {
  struct Actor *actor;
  s32 flag;

  for (; list[0] != (u16)-1; list += 3) {
    actor = Actor_Get(list[0]);
    flag = (s16)list[2];
    Actor_SetSpriteFlags(actor, 0);
    Actor_SelectAnimation(actor, Event_GetFlag((u16)flag) + 1);
    actor->motionFlags = 0;
    actor->collisionFlags &= ~ACTOR_SOLID;
    actor->timer = flag;
    IzumoRuins_ShowStakeState(actor);
  }
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_IZUMO_RUINS_FIRST_ROOM)
    return gIzumoRuins_FirstRoomRegions;
  if (scene == SCENE_IZUMO_RUINS_SECOND_ROOM)
    return gIzumoRuins_SecondRoomRegions;
  return 0;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_IZUMO_RUINS_FIRST_ROOM)
    return gIzumoRuins_FirstRoomActorPlacements;
  if (scene == SCENE_IZUMO_RUINS_SECOND_ROOM)
    return gIzumoRuins_SecondRoomActorPlacements;
  return gIzumoRuins_ActorPlacements;
}

/// Pound on one of the ruins' wooden stakes: the giant fist hammers it down
/// flush with the floor, so that it is no longer a step to jump on. The stakes
/// are the pair between the puddles where the ladder comes down, the one by
/// the strip of sand in the first scene's north part, and the pair in the
/// second scene.
///
/// Pound_SinkTarget animates the blow from the scene's list of stakes; as the
/// stake lands (mode 1) IzumoRuins_ShowStakeState makes its cell floor.
void IzumoRuins_PoundStake(s32 mode, s32 target) {
  struct FieldEffectWork *work = Ram_HeapSlots->fieldEffectWork;
  struct MainRoutineWork *party = &gMainRoutineWork;
  s16 scene;

  scene = party->next;
  if (scene == SCENE_IZUMO_RUINS_FIRST_ROOM)
    Pound_SinkTarget(gIzumoRuins_FirstRoomStakes, mode, target);
  else
    Pound_SinkTarget(gIzumoRuins_SecondRoomStakes, mode, target);
  if (mode == 1)
    IzumoRuins_ShowStakeState(Actor_Get(work->secondActorId));
}

/// Frost on one of the two puddles either side of the stakes where the ladder
/// comes down (cells (8, 51) and (14, 51)) freezes it into a pillar of ice
/// that Felix can jump onto.
void IzumoRuins_FreezePuddle(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *actor;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FlagBlock_ApplyFlags(gIzumoRuins_Puddles);
  actor = work->movedBlock;
  actor->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  actor->priorityFlags &= ~ACTOR_PRIORITY_UNDERFOOT;
  Event_End();
}

/// Casting Sand while standing on one of the ruins' sand patches takes Felix
/// into the sand: the game moves him onto the map's sand layer, where he can
/// travel only through sand cells, under the stakes and past what blocks the
/// floor above. Cast off the sand, nothing changes.
void IzumoRuins_SinkIntoSand(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  s32 cell =
      MapCell_GetAttributes(MAPGRID_LAYER_OVERHEAD, FIELD_CELL(leader->x.fixed),
                            FIELD_CELL(leader->z.fixed));

  if (CELL_CODE(cell) == CELL_BEHAVIOR_SAND)
    leader->layer = 2;
}

/// As Sand wears off, Felix comes up out of the sand onto the ruins' floor:
/// the game puts him back on the map's ground layer.
void IzumoRuins_RiseOutOfSand(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);

  leader->layer = 0;
}

/// Move (or a push) takes hold of a stone pillar: the one at the north end of
/// the first scene, or one of the six round the stone tablet. Round the
/// tablet, the cell south of it between the ring's two south pillars is first
/// closed, so that no pillar can be set down in the way to it.
void IzumoRuins_StartMovingPillar(void) {
  if (gMainRoutineWork.next == SCENE_IZUMO_RUINS_FIRST_ROOM) {
    Actor_LiftTargetBlock(gIzumoRuins_FirstRoomPillars);
  } else {
    s32 cell = MapCell_GetAttributes(MAPGRID_LAYER_OVERHEAD,
                                     RING_SOUTH_GAP_COLUMN, RING_SOUTH_GAP_ROW);

    cell = CELL_ATTRIBUTES(CELL_BEHAVIOR_WALL, cell & CELL_ATTRIBUTE_BITS);
    MapCell_SetAttributes(MAPGRID_LAYER_OVERHEAD, RING_SOUTH_GAP_COLUMN,
                          RING_SOUTH_GAP_ROW, cell);
    Actor_LiftTargetBlock(gIzumoRuins_SecondRoomPillars);
  }
}

/// Move sets a stone pillar down; round the stone tablet the way in from the
/// south is open floor again.
void IzumoRuins_FinishMovingPillar(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_PlaceMovedBlock();
  if (gMainRoutineWork.next == SCENE_IZUMO_RUINS_SECOND_ROOM)
    MapCell_SetAttributes(
        MAPGRID_LAYER_OVERHEAD, RING_SOUTH_GAP_COLUMN, RING_SOUTH_GAP_ROW,
        MapCell_GetAttributes(MAPGRID_LAYER_OVERHEAD, RING_SOUTH_GAP_COLUMN,
                              RING_SOUTH_GAP_ROW) &
            CELL_ATTRIBUTE_BITS);
  Event_End();
}

/// Parch cast at the green pool in the west of the second scene (cells (8, 14)
/// to (14, 21)) dries it for good: the water is gone, and its bed is floor
/// that leads on towards the room of the stone tablet.
void IzumoRuins_ParchThePool(void) {
  Event_Start();
  MapGrid_CopyCellAttributes(7, 2, 8, 7, 7, 12);
  MapGrid_CopyCellsTo(7, 66, 7, 76, 8, 9);
  Event_SetFlag(FLAG_IZUMO_RUINS_POOL_PARCHED);
  Event_End();
}

const struct Trigger *Scene_GetTriggers(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_IZUMO_RUINS_FIRST_ROOM)
    return gIzumoRuins_FirstRoomTriggers;
  return gIzumoRuins_SecondRoomTriggers;
}

/// Sets up the scene being entered: the window in, its stone pillars and
/// stakes, the first scene's puddles, and the second scene's pool dried if
/// Parch has dried it. Returns 0.
s32 Scene_Initialize(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *actor;
  s32 height;

  work->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  Actor_Get(gMainRoutineWork.leaderActorId)->priorityFlags |=
      ACTOR_PRIORITY_IGNORES_RISES;
  FieldEffect_StartBlankOverlay(0);
  if (gMainRoutineWork.next == SCENE_IZUMO_RUINS_FIRST_ROOM) {
    Actor_PlaceBlocks(gIzumoRuins_FirstRoomPillars);
    IzumoRuins_PlaceStakes((u16 *)gIzumoRuins_FirstRoomStakes);
    FlagBlock_PlaceAll(gIzumoRuins_Puddles);
  } else {
    Actor_Get(ACTOR_RING_NORTHWEST_PILLAR)->priorityFlags |=
        ACTOR_PRIORITY_IGNORES_RISES;
    Actor_Get(ACTOR_RING_NORTHEAST_PILLAR)->priorityFlags |=
        ACTOR_PRIORITY_IGNORES_RISES;
    Actor_PlaceBlocks(gIzumoRuins_SecondRoomPillars);
    IzumoRuins_PlaceStakes((u16 *)gIzumoRuins_SecondRoomStakes);
    if (Event_GetFlag(FLAG_IZUMO_RUINS_POOL_PARCHED)) {
      MapGrid_CopyCellAttributes(7, 2, 8, 7, 7, 12);
      MapGrid_CopyCellsTo(7, 66, 7, 76, 8, 9);
      MapGrid_CenterOnFollowedPoint();
      System_Wait(1);
      if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION)) {
        actor = Actor_Get(gMainRoutineWork.leaderActorId);
        height = Map_GetTerrainHeight(actor->layer, actor->x.fixed,
                                      actor->z.fixed - actor->y.fixed);
        actor->groundHeight = height;
        actor->y.fixed = height;
        EventCamera_FollowActor(gMainRoutineWork.leaderActorId, 0);
      }
    }
  }
  return 0;
}

/// Nothing to prepare before the map is built. Returns 0.
s32 Scene_PrepareMap(void) { return 0; }

// clang-format off
#include "shared/dust_scatter_script.c"

#include "shared/dust_burst_script.c"

#include "shared/field_effect_duration_scripts.c"

/// The stone pillars Move can push in each scene, ended by -1.
s16 gIzumoRuins_FirstRoomPillars[] = {
    ACTOR_FIRST_ROOM_PILLAR, -1,
};
s16 gIzumoRuins_SecondRoomPillars[] = {
    ACTOR_RING_NORTHWEST_PILLAR, ACTOR_RING_NORTHEAST_PILLAR,
    ACTOR_RING_WEST_PILLAR, ACTOR_RING_SOUTHWEST_PILLAR,
    ACTOR_RING_SOUTHEAST_PILLAR, ACTOR_RING_EAST_PILLAR,
    -1, 0,
};

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

/// Pound's stakes in each scene (IzumoRuins_PlaceStakes): actor, unused, and
/// the flag set while it is driven down; ended by -1.
s16 gIzumoRuins_FirstRoomStakes[] = {
    ACTOR_FIRST_STAKE, 0, FLAG_FIRST_STAKE_POUNDED,
    ACTOR_SECOND_STAKE, 0, FLAG_SECOND_STAKE_POUNDED,
    ACTOR_THIRD_STAKE, 0, FLAG_THIRD_STAKE_POUNDED,
    -1,
};

/// The second room's two stakes that Pound sinks: each stake, its kind and the
/// flag set once it is sunk, ended by -1.
s16 gIzumoRuins_SecondRoomStakes[] = {
    ACTOR_FIRST_STAKE, 0, FLAG_FIRST_STAKE_POUNDED,
    ACTOR_SECOND_STAKE, 0, FLAG_SECOND_STAKE_POUNDED,
    -1,
};

/// The first scene's puddles Frost freezes: actor and the flag set once frozen,
/// ended by -1.
s16 gIzumoRuins_Puddles[] = {
    ACTOR_WEST_PUDDLE, FLAG_WEST_PUDDLE_FROZEN,
    ACTOR_EAST_PUDDLE, FLAG_EAST_PUDDLE_FROZEN,
    -1,
};

#include "shared/scene_entrances_default.c"

/// The top of the ladder up out of each scene, which takes Felix out by its
/// exit: up to Izumo from the first, up to the first from the second.
struct SceneRegion gIzumoRuins_FirstRoomRegions[] = {
    {208, 44, 752, 224, 60, 768, CONDITION_ALWAYS,
     IZUMO_RUINS_FIRST_ROOM_TO_IZUMO},
    {SCENE_TABLE_END},
};

struct SceneRegion gIzumoRuins_SecondRoomRegions[] = {
    {400, 16, 352, 416, 32, 368, CONDITION_ALWAYS,
     IZUMO_RUINS_SECOND_ROOM_TO_IZUMO_RUINS_FIRST_ROOM},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(IzumoRuinsFirstRoom),
    SCENE_EXIT_UNLESS(1, Izumo, 10, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, IzumoRuinsFirstRoom, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, IzumoRuinsFirstRoom, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, IzumoRuinsSecondRoom, 1, EXIT_FLAG_NONE),
    SCENE_EXITS(IzumoRuinsSecondRoom),
    SCENE_EXIT_UNLESS(1, IzumoRuinsFirstRoom, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, IzumoRuinsSecondRoom, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, IzumoRuinsSecondRoom, 2, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

struct ActorPlacement gIzumoRuins_ActorPlacements[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gIzumoRuins_FirstRoomActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 824, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 824, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 824, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 824, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 696, 0, 248, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 88, FACING_NORTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gIzumoRuins_SecondRoomActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 552, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 552, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 792, 0, 152, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 824, 0, 152, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 760, 0, 184, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 792, 0, 216, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 824, 0, 216, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 856, 0, 184, FACING_NORTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};
// clang-format on

struct Trigger gIzumoRuins_FirstRoomTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS),
     ACTOR_FIRST_STAKE, FLAG_FIRST_STAKE_POUNDED, EVENT(IzumoRuins_PoundStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS),
     ACTOR_SECOND_STAKE, FLAG_SECOND_STAKE_POUNDED,
     EVENT(IzumoRuins_PoundStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS),
     ACTOR_THIRD_STAKE, FLAG_THIRD_STAKE_POUNDED, EVENT(IzumoRuins_PoundStake)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_WEST_PUDDLE,
     FLAG_WEST_PUDDLE_FROZEN, EVENT(IzumoRuins_FreezePuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_EAST_PUDDLE,
     FLAG_EAST_PUDDLE_FROZEN, EVENT(IzumoRuins_FreezePuddle)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0,
     CONDITION_ALWAYS, EVENT(IzumoRuins_SinkIntoSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS,
     EVENT(IzumoRuins_RiseOutOfSand)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_FIRST_ROOM_PILLAR, CONDITION_ALWAYS,
     EVENT(IzumoRuins_StartMovingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_ROOM_PILLAR, CONDITION_ALWAYS,
     EVENT(IzumoRuins_FinishMovingPillar)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(IzumoRuins_StartMovingPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(IzumoRuins_FinishMovingPillar)},
    {TRIGGER_TABLE_END},
};

struct Trigger gIzumoRuins_SecondRoomTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS),
     ACTOR_FIRST_STAKE, FLAG_FIRST_STAKE_POUNDED, EVENT(IzumoRuins_PoundStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS),
     ACTOR_SECOND_STAKE, FLAG_SECOND_STAKE_POUNDED,
     EVENT(IzumoRuins_PoundStake)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_PARCH, PSYNERGY_PHASE_ACTS), CELL_POOL,
     CONDITION_ALWAYS, EVENT(IzumoRuins_ParchThePool)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_RING_NORTHWEST_PILLAR, CONDITION_ALWAYS,
     EVENT(IzumoRuins_StartMovingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_RING_NORTHWEST_PILLAR,
     CONDITION_ALWAYS, EVENT(IzumoRuins_FinishMovingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_RING_NORTHEAST_PILLAR, CONDITION_ALWAYS,
     EVENT(IzumoRuins_StartMovingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_RING_NORTHEAST_PILLAR,
     CONDITION_ALWAYS, EVENT(IzumoRuins_FinishMovingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_RING_WEST_PILLAR, CONDITION_ALWAYS,
     EVENT(IzumoRuins_StartMovingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_RING_WEST_PILLAR, CONDITION_ALWAYS,
     EVENT(IzumoRuins_FinishMovingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_RING_SOUTHWEST_PILLAR, CONDITION_ALWAYS,
     EVENT(IzumoRuins_StartMovingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_RING_SOUTHWEST_PILLAR,
     CONDITION_ALWAYS, EVENT(IzumoRuins_FinishMovingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_RING_SOUTHEAST_PILLAR, CONDITION_ALWAYS,
     EVENT(IzumoRuins_StartMovingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_RING_SOUTHEAST_PILLAR,
     CONDITION_ALWAYS, EVENT(IzumoRuins_FinishMovingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
     ACTOR_RING_EAST_PILLAR, CONDITION_ALWAYS,
     EVENT(IzumoRuins_StartMovingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_RING_EAST_PILLAR, CONDITION_ALWAYS,
     EVENT(IzumoRuins_FinishMovingPillar)},
    {TRIGGER_TABLE_END},
};
