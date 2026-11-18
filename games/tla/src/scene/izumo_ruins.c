/// @file
/// The Izumo Ruins, under the stone circle of Izumo, in two rooms that share
/// this overlay (SceneId_IzumoRuinsFirstRoom, SceneId_IzumoRuinsSecondRoom).
/// Sand carries the party across the sandy floor (IzumoRuins_SinkIntoSand),
/// Frost freezes the puddles (IzumoRuins_FreezePuddle), Pound sinks wooden
/// stakes (IzumoRuins_PoundStake), Move sets the stone pillars in place
/// (IzumoRuins_FinishMovingPillar), grey pillars crumble under a second step
/// (shared/crumbling_pillar.h), and Parch dries the pool before the room of
/// the stone tablet (IzumoRuins_ParchThePool).
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
void Rubble_Spray(s32 mode);
void Rubble_Burst(void);
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

#include "shared/rubble.h"

s32 MapCollision_FindOrAddTag(s32 terrainId, s32 value, s32 height);

#include "shared/actor_set_cell_shape.h"

#include "shared/leader_set_cell_shape.h"

#include "../field/actor_set_sprite_priority.c"

#include "../field/field_effect_move.c"

#define EFFECT_SPAWN FieldEffect_Spawn
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SCRIPTS gFieldEffect_DurationScripts
#include "../field/field_effect_spawn.c"

#include "text/msg_ids.h"

struct Actor;
extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];
extern u32 gFieldEffect_LongScript[];

/// Set as Parch dries the pool before the room of the stone tablet.
#define FLAG_IZUMO_RUINS_POOL_PARCHED 0x941

/// Both rooms' first two actors: wooden stakes Pound sinks
/// (gIzumoRuins_FirstRoomStakes, gIzumoRuins_SecondRoomStakes).
enum { ACTOR_FIRST_STAKE = ACTOR_FIRST_PLACED, ACTOR_SECOND_STAKE };

/// The first room's other actors: the puddles Frost freezes, west and east of
/// its first two stakes, a third stake to the north, and the stone pillar Move
/// pushes.
enum {
  ACTOR_WEST_PUDDLE = ACTOR_FIRST_PLACED + 2,
  ACTOR_EAST_PUDDLE,
  ACTOR_THIRD_STAKE,
  ACTOR_FIRST_ROOM_PILLAR
};

/// The second room's six stone pillars Move pushes, in a ring round the stone
/// tablet at its north end.
enum {
  ACTOR_RING_NORTHWEST_PILLAR = ACTOR_FIRST_PLACED + 2,
  ACTOR_RING_NORTHEAST_PILLAR,
  ACTOR_RING_WEST_PILLAR,
  ACTOR_RING_SOUTHWEST_PILLAR,
  ACTOR_RING_SOUTHEAST_PILLAR,
  ACTOR_RING_EAST_PILLAR
};

/// Set while Pound has driven a stake into the ground: each room's first and
/// second, and the first room's third.
#define FLAG_FIRST_STAKE_POUNDED FLAG_SCENE_TEMP_00
#define FLAG_SECOND_STAKE_POUNDED FLAG_SCENE_TEMP_01
#define FLAG_THIRD_STAKE_POUNDED FLAG_SCENE_TEMP_02
/// Set once Frost has frozen a puddle of the first room.
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

/// Fits the cells under a stake to its flag (kept in its timer). Sunk, the
/// overhead cell is a wall, the ground cell is no gap to jump, and the cell
/// sinks STAKE_HEIGHT_STEPS unless the stake's ground is the map's; standing,
/// the overhead cell is sand where the ground is sand and a wall otherwise,
/// the ground cell is a gap the leader jumps across, and the cell rises to the
/// stake's top. A sunk stake is drawn at sprite priority 3, a standing one at
/// 2.
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
/// -1: each shows its sunk or standing state by its flag.
void IzumoRuins_PlaceStakes(u16 *list) {
  struct Actor *actor;
  s32 flag;

  while (list[0] != (u16)-1) {
    actor = Actor_Get(list[0]);
    flag = (s16)list[2];
    Actor_SetSpriteFlags(actor, 0);
    Actor_SelectAnimation(actor, Event_GetFlag((u16)flag) + 1);
    actor->motionFlags = 0;
    actor->collisionFlags &= ~ACTOR_SOLID;
    actor->timer = flag;
    IzumoRuins_ShowStakeState(actor);
    list += 3;
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

/// Pound on a stake sinks it (Pound_SinkTarget, by the room's list); as it
/// lands (mode 1) its cells follow (IzumoRuins_ShowStakeState).
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

/// Frost on a puddle: it becomes an ice pillar, drawn by the cells under it.
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

/// Where the cell under the leader is sand (CELL_BEHAVIOR_SAND in its high
/// byte), the leader walks on the upper layer, in the sand.
void IzumoRuins_SinkIntoSand(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  s32 cell =
      MapCell_GetAttributes(MAPGRID_LAYER_OVERHEAD, FIELD_CELL(leader->x.fixed),
                            FIELD_CELL(leader->z.fixed));

  if (CELL_CODE(cell) == CELL_BEHAVIOR_SAND)
    leader->layer = 2;
}

/// Puts the leader on map layer 0.
void Leader_SetLayer(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);

  leader->layer = 0;
}

/// Move's cast on a pillar lifts it; in the second room the gap south of the
/// stone tablet is first made a wall.
void IzumoRuins_StartMovingPillar(void) {
  if (gMainRoutineWork.next == SCENE_IZUMO_RUINS_FIRST_ROOM) {
    Actor_LiftTargetBlock(gIzumoRuins_FirstRoomPillars);
  } else {
    /* FAKEMATCH(signature): the cell read through an s16 return type. */
    MapCell_SetAttributes(MAPGRID_LAYER_OVERHEAD, RING_SOUTH_GAP_COLUMN,
                          RING_SOUTH_GAP_ROW,
                          CELL_ATTRIBUTES(CELL_BEHAVIOR_WALL, 0) |
                              (((s16 (*)(u32, s32, s32))MapCell_GetAttributes)(
                                   MAPGRID_LAYER_OVERHEAD,
                                   RING_SOUTH_GAP_COLUMN, RING_SOUTH_GAP_ROW) &
                               CELL_ATTRIBUTE_BITS));
    Actor_LiftTargetBlock(gIzumoRuins_SecondRoomPillars);
  }
}

/// Move sets a pillar down; in the second room the gap south of the stone
/// tablet is open ground again.
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

/// Parch dries the pool before the room of the stone tablet, for good.
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

/// Sets up the room being entered: the window in, its movable and Pound's
/// pillars, the first room's puddles, and the second room's pool if parched.
/// Returns 0.
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
u32 gRubble_SprayScript[] = {
    SC_WAIT(33), SC_SET(SW_HOOK, 0), SC_POSITION(0, 0, 0), SC_WAIT(1),
    SC_RELEASE,
};

u32 gRubble_BurstScript[] = {
    SC_WAIT(44), SC_SET(SW_HOOK, 0), SC_POSITION(0, 0, 0), SC_WAIT(1),
    SC_RELEASE,
};

#include "shared/field_effect_duration_scripts.c"

/// The pillars Move can lift in each room, ended by -1.
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

/// Pound's stakes in each room (IzumoRuins_PlaceStakes): actor, unused, and
/// the flag set while it is sunk; ended by -1.
s16 gIzumoRuins_FirstRoomStakes[] = {
    ACTOR_FIRST_STAKE, 0, FLAG_FIRST_STAKE_POUNDED,
    ACTOR_SECOND_STAKE, 0, FLAG_SECOND_STAKE_POUNDED,
    ACTOR_THIRD_STAKE, 0, FLAG_THIRD_STAKE_POUNDED,
    -1,
};

s16 gIzumoRuins_SecondRoomStakes[] = {
    ACTOR_FIRST_STAKE, 0, FLAG_FIRST_STAKE_POUNDED,
    ACTOR_SECOND_STAKE, 0, FLAG_SECOND_STAKE_POUNDED,
    -1,
};

/// The first room's puddles Frost freezes: actor and the flag set once frozen,
/// ended by -1.
s16 gIzumoRuins_Puddles[] = {
    ACTOR_WEST_PUDDLE, FLAG_WEST_PUDDLE_FROZEN,
    ACTOR_EAST_PUDDLE, FLAG_EAST_PUDDLE_FROZEN,
    -1,
};

#include "shared/scene_entrances_default.c"

/// The top of the ladder up out of each room, which takes the leader out by
/// its exit.
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
     EVENT(Leader_SetLayer)},
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
    {PSYNERGY_PHASE_ON_CELL(ABILITY_PARCH, PSYNERGY_PHASE_ACTS), 50,
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
