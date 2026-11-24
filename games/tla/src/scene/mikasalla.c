/// @file
/// Mikasalla, the village in the far south of Osenia, and the tunnels under
/// it, in two scenes that share this overlay (`SceneId_Mikasalla`, the village,
/// and `SceneId_MikasallaNorth`, the tunnels).
///
/// The inn is the northeast door, the general store the northwest one, and the
/// Sanctum the wide door in the west cliff. Once the party has crossed the
/// desert, Scoop in the sheep pen in the southwest corner digs up a ladder down
/// to the tunnels. Parch dries up the tunnels' water, and their far end comes
/// out in the cliff behind the inn, where Spark the Mars Djinni joins the party
/// without a fight.

#include "flags.h"
#include "sprite_ids.h"
#include "shared/tables.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "field/scene_tables.h"
#include "system/main_routine_state.h"
#include "scene_ids.h"
#include "field/field_sequence_event.h"
#include "system/ram_buffer.h"
#include "field/actor.h"
#include "field/event_inline.h"
#include "field/actor_script.h"
#include "field/sprite.h"
#include "field/map_grid.h"
#include "djinn_ids.h"
#include "text/msg_ids.h"

/// Set once Scoop has dug up the ladder down to the tunnels by the sheep; the
/// village redraws it on every visit.
#define FLAG_MIKASALLA_LADDER_DUG_UP 0x85b
/// Set once Parch has dried up the water in the tunnels, for this visit.
#define FLAG_MIKASALLA_TUNNELS_PARCHED FLAG_SCENE_TEMP_00

/// The ways between the tunnels' cave and the chamber above it, by the way out
/// behind the inn, each an exit and the entrance that lands there: the top of
/// the cave's ladder, and the hole in the chamber's floor.
enum { MIKASALLA_NORTH_CAVE_LADDER = 2, MIKASALLA_NORTH_CHAMBER_HOLE = 3 };

/// The cells round a ladder top that climb Felix down it, by the way he walks
/// in. The dug ladder in the sheep pen has the first three, round its top at
/// cell (5, 40); the hole in the tunnels' chamber floor has the last two, at
/// cells (25, 7) to (27, 7).
enum {
  CELL_LADDER_TOP_FROM_NORTH = 30,
  CELL_LADDER_TOP_FROM_WEST,
  CELL_LADDER_TOP_FROM_EAST,
  CELL_LADDER_TOP_FROM_SOUTH,
  CELL_LADDER_TOP_FROM_SOUTH_BESIDE
};

/// The patch of ground in the sheep pen that Scoop digs, cell (5, 41).
#define CELL_BURIED_LADDER 50
/// The tunnels' water, which Parch dries.
#define CELL_TUNNEL_WATER 60

extern struct SceneRegion gMikasalla_TunnelRegions[];
extern struct ActorPlacement gMikasalla_VillagePlacements[];
extern struct ActorPlacement gMikasalla_TunnelPlacements[];

extern struct Trigger gMikasalla_TunnelTriggers[];

extern struct Trigger gMikasalla_VillageTriggers[];
extern u16 gMikasalla_DoorOpening[];
extern u16 gMikasalla_ThirdDoorOpening[];
extern u16 gMikasalla_WideDoorOpening[];
extern s32 gMikasalla_ChickenPaces[];
extern s32 gMikasalla_SheepPaces[];

/// The village's actors, numbering the rows of `gMikasalla_VillagePlacements`.
enum MikasallaActorId {
  ACTOR_VILLAGER_NO_VISITORS = ACTOR_FIRST_PLACED,
  /// Mikasalla_AskLookingForSomeone's villager.
  ACTOR_VILLAGER_LOOKING_FOR_SOMEONE,
  ACTOR_VILLAGER_AFTER_EARTHQUAKE,
  ACTOR_VILLAGER_DEEPER_INTO_OSENIA,
  ACTOR_VILLAGER_AIRS_ROCK,
  ACTOR_VILLAGER_TWO_WAYS,
  ACTOR_VILLAGER_TUNDARIA,
  ACTOR_VILLAGER_GAROH,
  ACTOR_CHICKEN,
  ACTOR_SHEEP,
  ACTOR_SPARK
};

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) {
  s16 scene = gMainRoutineState.next;

  if (scene == SCENE_MIKASALLA_NORTH)
    return gMikasalla_TunnelRegions;
  return 0;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s16 scene = gMainRoutineState.next;

  if (scene == SCENE_MIKASALLA)
    return gMikasalla_VillagePlacements;
  return gMikasalla_TunnelPlacements;
}

void TilesetAnim_Pause(s32 channel);

void Mikasalla_ClimbDownLadder(void);

extern u8 MsgFieldAreYouLookingForSomeone[];

extern struct MapGridCellAnimation gMikasalla_Doors[];

/// Felix walks north into one of the village's doors: the inn's, the northeast
/// door at cell (20, 32); the general store's, to the northwest at (13, 36);
/// the house of the old man who climbs Air's Rock, to the east at (22, 40);
/// the explorer's family's, to the southwest at (11, 43); or the Sanctum's wide
/// door in the west cliff at (5, 31). Everyone stops, the door opens,
/// and he walks in.
///
/// Each door's cell carries the number of the exit it leaves by, and
/// `gMikasalla_Doors` holds its opening, drawn over its cells. The Sanctum's
/// door is entered without the walk.
void Mikasalla_EnterDoor(void) {
  struct FieldState *state = Ram_HeapSlots->fieldState;
  u32 id;
  s16 *exit;
  s32 index;

  Event_Start();
  Event_EndLastingPsynergy(0);
  for (id = ACTOR_FIRST_PLACED; id < FIELD_TREASURE_ACTOR; id++) {
    struct Actor *actor = Actor_GetById(id);

    if (actor != NULL)
      actor->motionFlags = 0;
  }
  exit = &state->stepTrigger;
  index = *exit;
  Event_PlaySound(SE_DOOR_OPEN);
  index--;
  MapGrid_AnimateCells(gMikasalla_Doors[index].steps, gMikasalla_Doors[index].x,
                       gMikasalla_Doors[index].y);
  Actor_SetSpeed(gMainRoutineState.leaderActorId, 0.5);
  Actor_GetById(gMainRoutineState.leaderActorId)->motionFlags = 0;

  Actor_SetAnimation(gMainRoutineState.leaderActorId, ANIM_WALK);
  if (index != MIKASALLA_TO_MIKASALLA_ROOMS_SANCTUM - 1) {
    Actor_CenterAndWalk(gMainRoutineState.leaderActorId, 2, -8);
    Event_Wait(10);
  }
  Import_EventSetExitRequest(*exit);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

/// Felix talks to Spark, the Mars Djinni on the path behind the inn, and Spark
/// joins the party without a fight.
void Mikasalla_MeetSpark(void) {
  Djinn_JoinParty(ACTOR_SPARK, DJINNI_SPARK / DJINN_PER_ELEMENT,
                  DJINNI_INDEX(DJINNI_SPARK));
}

const struct Trigger *Scene_GetTriggers(void) {
  s16 scene = gMainRoutineState.next;

  if (scene == SCENE_MIKASALLA_NORTH)
    return gMikasalla_TunnelTriggers;
  return gMikasalla_VillageTriggers;
}

/// The woman in a white dress south of the inn asks Felix whether he is looking
/// for someone; either way, she says strangers are rare here.
void Mikasalla_AskLookingForSomeone(void) {
  s32 msg;

  Event_Start();
  Event_EndLastingPsynergy(0);
  msg = (s32)&MsgFieldAreYouLookingForSomeone;
  Event_SetMessageId(msg);
  // "Are you looking for someone?"
  // Yes: "Well, if you're looking for someone... I'd say the best place to
  // start would be... somewhere else." No: "Didn't think so... We don't get a
  // lot of strangers coming here."
  Event_OpenMessage(ACTOR_VILLAGER_LOOKING_FOR_SOMEONE, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(msg + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessage(ACTOR_VILLAGER_LOOKING_FOR_SOMEONE, 0);
  Event_End();
}

/// Felix casts Scoop in the sheep pen in the village's southwest corner, cell
/// (5, 41), and digs up a ladder down to the tunnels under the village. It
/// stays dug on later visits.
void Mikasalla_DigUpLadder(void) {
  MapGrid_CopyCellsTo(5, 49, 5, 41, 1, 1);
  MapGrid_CopyCellAttributes(4, 48, 3, 2, 4, 40);
  Event_SetFlag(FLAG_MIKASALLA_LADDER_DUG_UP);
}

/// Dries up the water in the tunnels: its tile animations stop and dry ground
/// is drawn over its pools.
static inline void Mikasalla_DryTunnels(void) {
  TilesetAnim_Pause(0);
  TilesetAnim_Pause(1);
  TilesetAnim_Pause(2);
  MapGrid_CopyCellsTo(56, 46, 30, 46, 16, 16);
  MapGrid_CopyCellsTo(56, 174, 30, 174, 16, 16);
  MapGrid_CopyCells(28, 23, 16, 16, 15, 23);
  MapGrid_CopyCells(28, 90, 2, 2, 21, 88);
  MapGrid_CopyCells(28, 90, 4, 4, 17, 89);
  MapGrid_CopyCells(28, 90, 2, 1, 17, 91);
  MapGrid_CopyCells(28, 90, 2, 2, 19, 91);
  MapGrid_CopyCells(28, 92, 2, 2, 15, 92);
  MapGrid_CopyCells(30, 92, 2, 2, 17, 92);
}

/// Felix casts Parch on the water in the tunnels under the village: the screen
/// flashes, and the water is gone, leaving bare ground to walk across. It stays
/// dry until the party leaves, and is dried again on a return.
void Mikasalla_CastParch(void) {
  if (Event_GetFlag(FLAG_MIKASALLA_TUNNELS_PARCHED) == 0) {
    Mikasalla_DryTunnels();
    Event_SetFlag(FLAG_MIKASALLA_TUNNELS_PARCHED);
  }
}

s32 Scene_Initialize(void) {
  gFieldState->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  return 0;
}

s32 Scene_PrepareMap(void) {
  struct MainRoutineState *party;

  Actor_GetById(ACTOR_CHICKEN)->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  party = &gMainRoutineState;
  Actor_GetById(party->leaderActorId)->priorityFlags |=
      ACTOR_PRIORITY_IGNORES_RISES;
  if (party->next == SCENE_MIKASALLA_NORTH) {
    if (Event_GetFlag(FLAG_MIKASALLA_TUNNELS_PARCHED))
      Mikasalla_DryTunnels();
  } else if (Event_GetFlag(FLAG_MIKASALLA_LADDER_DUG_UP)) {
    Mikasalla_DigUpLadder();
  }
  return 0;
}

/// Felix, at a ladder top, turns north, steps onto the rungs and climbs down
/// out of sight: down the dug ladder from the sheep pen to the tunnels, or
/// down the hole in the chamber's floor to the cave below.
void Mikasalla_ClimbDownLadder(void) {
  s32 exit;
  s32 drop;

  if (gMainRoutineState.next == SCENE_MIKASALLA) {
    exit = MIKASALLA_TO_MIKASALLA_NORTH_SOUTH;
    drop = 17;
  } else {
    exit = MIKASALLA_NORTH_CHAMBER_HOLE;
    drop = 9;
  }
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  Event_Wait(10);
  Actor_GetById(ACTOR_FELIX)->steeringFlags &= ~ACTOR_FACES_MOTION;
  // XXX: what do the two cells copied to (72, 14) show, in both the village
  // and the tunnels? A recording of each climb down, with the grid before and
  // after, would settle it.
  MapGrid_CopyCellsTo(96, 14, 72, 14, 2, 1);
  Actor_SetDestinationOffset(ACTOR_FELIX, 0, 4);
  Actor_WaitForMoveById(ACTOR_FELIX);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_FELIX), 0);
  Actor_SetAnimation(ACTOR_FELIX, ANIM_LEADER_CLIMB_DOWN_FROM_TOP);
  Actor_SetDestinationOffset(ACTOR_FELIX, 0, drop);
  Actor_WaitForMoveById(ACTOR_FELIX);
  Actor_SetAnimation(ACTOR_FELIX, ANIM_LEADER_CLIMB);
  Event_Wait(10);
  Event_PlaySound(SE_MAP_EXIT);
  Import_EventSetExitRequest(exit);
  Event_End();
}

/// Felix walks east into the dug ladder in the sheep pen from its west side,
/// cell (4, 41): he steps up and across onto the rungs and climbs down into
/// the tunnels.
void Mikasalla_StepOntoLadderFacingEast(void) {
  Event_Start();
  Event_EndLastingPsynergy(0);
  Actor_WalkByAndWait(ACTOR_FELIX, 0, -14);
  Actor_WalkByAndWait(ACTOR_FELIX, 16, 0);
  Mikasalla_ClimbDownLadder();
}

/// Felix walks west into the dug ladder in the sheep pen from its east side,
/// cell (6, 41): he steps up and across onto the rungs and climbs down into
/// the tunnels.
void Mikasalla_StepOntoLadderFacingWest(void) {
  Event_Start();
  Event_EndLastingPsynergy(0);
  Actor_WalkByAndWait(ACTOR_FELIX, -2, -6);
  Actor_WalkByAndWait(ACTOR_FELIX, -14, -8);
  Mikasalla_ClimbDownLadder();
}

/// Felix walks south onto the top of the dug ladder in the sheep pen, cell
/// (5, 40): he turns round and climbs down into the tunnels.
void Mikasalla_TurnAndClimbDownLadder(void) {
  Event_Start();
  Event_EndLastingPsynergy(0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  Mikasalla_ClimbDownLadder();
}

/// Felix walks north into the hole in the floor of the small chamber at the
/// tunnels' north end, cell (26, 7): he steps onto its ladder from whichever
/// side he stands and climbs down into the cave below.
void Mikasalla_StepOntoLadderFacingNorth(void) {
  s32 x = Actor_GetById(ACTOR_FELIX)->x.part[0].integer;

  Event_Start();
  Event_EndLastingPsynergy(0);
  if (x % 16 <= 7) {
    Actor_WalkByAndWait(ACTOR_FELIX, -4, -4);
    Actor_WalkByAndWait(ACTOR_FELIX, -4, -8);
  } else {
    Actor_WalkByAndWait(ACTOR_FELIX, 4, -4);
    Actor_WalkByAndWait(ACTOR_FELIX, 4, -8);
  }
  Actor_WalkToAndWait(ACTOR_FELIX, 424, 99);
  Mikasalla_ClimbDownLadder();
}

/// Felix walks north into the hole in the floor of the small chamber at the
/// tunnels' north end, beside its ladder, cell (25, 7) or (27, 7): he steps
/// onto the ladder and climbs down into the cave below.
void Mikasalla_StepOntoLadderAhead(void) {
  Event_Start();
  Event_EndLastingPsynergy(0);
  Actor_WalkByAndWait(ACTOR_FELIX, 0, -4);
  Actor_WalkToAndWait(ACTOR_FELIX, 424, 99);
  Mikasalla_ClimbDownLadder();
}

/// The doors' openings: most doors, the east house's door with cells of its
/// own, and the Sanctum's wide door, two cells across.
u16 gMikasalla_DoorOpening[] = {
    MAPGRID_ANIMATION_STEP(81, 50, 1, 2, 6),
    MAPGRID_ANIMATION_STEP(80, 50, 1, 2, 6),
    MAPGRID_ANIMATION_STEPS_END,
};
u16 gMikasalla_ThirdDoorOpening[] = {
    MAPGRID_ANIMATION_STEP(79, 50, 1, 2, 6),
    MAPGRID_ANIMATION_STEP(78, 50, 1, 2, 6),
    MAPGRID_ANIMATION_STEPS_END,
};
u16 gMikasalla_WideDoorOpening[] = {
    MAPGRID_ANIMATION_STEP(78, 52, 2, 2, 6),
    MAPGRID_ANIMATION_STEP(78, 53, 2, 2, 6),
    MAPGRID_ANIMATION_STEPS_END,
};

/// The village's doors, by exit number from the first (Mikasalla_EnterDoor):
/// each opening and the cell it is drawn at.
struct MapGridCellAnimation gMikasalla_Doors[] = {
    {gMikasalla_DoorOpening, 84, 29},      {gMikasalla_DoorOpening, 77, 34},
    {gMikasalla_ThirdDoorOpening, 86, 38}, {gMikasalla_DoorOpening, 75, 41},
    {gMikasalla_WideDoorOpening, 69, 27},
};

// clang-format off
struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 456, 0, 584, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

/// The tunnels' ways out at the tops of their ladders: up to the sheep pen,
/// and up from the cave to the chamber.
struct SceneRegion gMikasalla_TunnelRegions[] = {
    {68, 28, 404, 76, 36, 412, CONDITION_ALWAYS, MIKASALLA_NORTH_TO_MIKASALLA_SOUTH},
    {244, 28, 100, 252, 36, 108, CONDITION_ALWAYS, MIKASALLA_NORTH_CAVE_LADDER},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(Mikasalla),
    SCENE_EXIT_UNLESS(MIKASALLA_TO_MIKASALLA_ROOMS_HOUSES_NORTHEAST, MikasallaRoomsHouses, MIKASALLA_ROOMS_HOUSES_FROM_MIKASALLA_NORTHEAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MIKASALLA_TO_MIKASALLA_ROOMS_HOUSES_NORTHWEST, MikasallaRoomsHouses, MIKASALLA_ROOMS_HOUSES_FROM_MIKASALLA_NORTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MIKASALLA_TO_MIKASALLA_ROOMS_HOUSES_EAST, MikasallaRoomsHouses, MIKASALLA_ROOMS_HOUSES_FROM_MIKASALLA_EAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MIKASALLA_TO_MIKASALLA_ROOMS_HOUSES_SOUTHWEST, MikasallaRoomsHouses, MIKASALLA_ROOMS_HOUSES_FROM_MIKASALLA_SOUTHWEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MIKASALLA_TO_MIKASALLA_ROOMS_SANCTUM, MikasallaRoomsSanctum, MIKASALLA_ROOMS_SANCTUM_FROM_MIKASALLA, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MIKASALLA_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_MIKASALLA, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, Mikasalla, 9, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(8, Mikasalla, 10, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(9, Mikasalla, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(10, Mikasalla, 8, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MIKASALLA_TO_MIKASALLA_NORTH_NORTH, MikasallaNorth, MIKASALLA_NORTH_FROM_MIKASALLA_NORTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MIKASALLA_TO_MIKASALLA_NORTH_SOUTH, MikasallaNorth, MIKASALLA_NORTH_FROM_MIKASALLA_SOUTH, EXIT_FLAG_NONE),
    SCENE_EXITS(MikasallaNorth),
    SCENE_EXIT_UNLESS(MIKASALLA_NORTH_TO_MIKASALLA_SOUTH, Mikasalla, MIKASALLA_FROM_MIKASALLA_NORTH_SOUTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MIKASALLA_NORTH_CAVE_LADDER, MikasallaNorth, MIKASALLA_NORTH_CHAMBER_HOLE, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MIKASALLA_NORTH_CHAMBER_HOLE, MikasallaNorth, MIKASALLA_NORTH_CAVE_LADDER, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(MIKASALLA_NORTH_TO_MIKASALLA_NORTH, Mikasalla, MIKASALLA_FROM_MIKASALLA_NORTH_NORTH, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};
// clang-format on

/// One squash of an animal in the pen as it turns: it flattens to three
/// quarters of its width and springs back over ten frames.
#define PEN_SQUASH                                                             \
  SC_ADD(SW_SCALE_X, -FIXED_ONE / 4), SC_ADD(SW_SCALE_Y, FIXED_ONE / 8),       \
      SC_WAIT(2), SC_ADD(SW_SCALE_X, FIXED_ONE / 8),                           \
      SC_ADD(SW_SCALE_Y, -FIXED_ONE / 16), SC_WAIT(2),                         \
      SC_ADD(SW_SCALE_X, FIXED_ONE / 16), SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32), \
      SC_WAIT(2), SC_ADD(SW_SCALE_X, FIXED_ONE / 16),                          \
      SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32), SC_WAIT(4)

/// An animal at one end of its pace stands a moment, turns round and squashes
/// three times.
#define PEN_TURN_ROUND                                                         \
  SC_ANIMATION(ANIM_STAND), SC_WAIT(20), SC_ADD(SW_FACING, -DEGtoANG(180)),    \
      PEN_SQUASH, PEN_SQUASH, PEN_SQUASH

/// The chicken paces between x 72 and 120 on its row, turning at each end.
s32 gMikasalla_ChickenPaces[] = {
    SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_SOLID),
    SC_ACCELERATION(0.2),
    SC_SPEED(0.6),
    SC_LABEL(1),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(120, 0, 648),
    SC_SYNC,
    PEN_TURN_ROUND,
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(72, 0, 648),
    SC_SYNC,
    PEN_TURN_ROUND,
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
};

/// The sheep paces its pen more slowly, between x 88 and 120.
s32 gMikasalla_SheepPaces[] = {
    SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_SOLID),
    SC_ACCELERATION(0.2),
    SC_SPEED(0.3),
    SC_LABEL(1),
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(88, 0, 680),
    SC_SYNC,
    PEN_TURN_ROUND,
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(120, 0, 680),
    SC_SYNC,
    PEN_TURN_ROUND,
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
};
// clang-format off

/// The village (`enum MikasallaActorId`).
struct ActorPlacement gMikasalla_VillagePlacements[] = {
    ACTOR_PLACEMENT(SPRITE_MAN_IN_GREEN_VEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 552, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_WHITE_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 600, FACING_SOUTH, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_MAN_IN_ORANGE_VEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 504, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_AUBURN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 264, 0, 680, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_BLUE_COAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 664, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_IN_GREEN_SHAWL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 680, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_IN_BLUE_SHIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 632, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_BLONDE_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 616, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_CHICKEN, CONDITION_ALWAYS, (s32)gMikasalla_ChickenPaces, 72, 0, 648, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SHEEP, CONDITION_ALWAYS, (s32)gMikasalla_SheepPaces, 120, 0, 680, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, FLAG_DJINNI(DJINNI_SPARK), MOVEMENT_TYPE_STAND, 312, 0, 168, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

/// The tunnels under the village.
struct ActorPlacement gMikasalla_TunnelPlacements[] = {
    {SCENE_TABLE_END},
};
// clang-format on

// clang-format off
/// The village's doors, talk, Mind Read, the dug ladder, and Spark.
struct Trigger gMikasalla_VillageTriggers[] = {
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), MIKASALLA_TO_MIKASALLA_ROOMS_HOUSES_NORTHEAST, CONDITION_ALWAYS, EVENT(Mikasalla_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), MIKASALLA_TO_MIKASALLA_ROOMS_HOUSES_NORTHWEST, CONDITION_ALWAYS, EVENT(Mikasalla_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), MIKASALLA_TO_MIKASALLA_ROOMS_HOUSES_EAST, CONDITION_ALWAYS, EVENT(Mikasalla_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), MIKASALLA_TO_MIKASALLA_ROOMS_HOUSES_SOUTHWEST, CONDITION_ALWAYS, EVENT(Mikasalla_EnterDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), MIKASALLA_TO_MIKASALLA_ROOMS_SANCTUM, CONDITION_ALWAYS, EVENT(Mikasalla_EnterDoor)},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_LADDER_TOP_FROM_NORTH, CONDITION_ALWAYS, EVENT(Mikasalla_TurnAndClimbDownLadder)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_LADDER_TOP_FROM_WEST, CONDITION_ALWAYS, EVENT(Mikasalla_StepOntoLadderFacingEast)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_LADDER_TOP_FROM_EAST, CONDITION_ALWAYS, EVENT(Mikasalla_StepOntoLadderFacingWest)},
    {TRIGGER_TALK, ACTOR_VILLAGER_NO_VISITORS, CONDITION_ALWAYS, MSGNO(MsgFieldItSBeenALong)},
    {TRIGGER_TALK, ACTOR_VILLAGER_LOOKING_FOR_SOMEONE, CONDITION_ALWAYS, EVENT(Mikasalla_AskLookingForSomeone)},
    {TRIGGER_TALK, ACTOR_VILLAGER_AFTER_EARTHQUAKE, CONDITION_ALWAYS, MSGNO(MsgFieldIWentOutToInvestigate)},
    {TRIGGER_TALK, ACTOR_VILLAGER_DEEPER_INTO_OSENIA, CONDITION_ALWAYS, MSGNO(MsgFieldIfYouReGoingDeeper)},
    {TRIGGER_TALK, ACTOR_VILLAGER_AIRS_ROCK, CONDITION_ALWAYS, MSGNO(MsgFieldInTheHeartOfOsenia)},
    {TRIGGER_TALK, ACTOR_VILLAGER_TWO_WAYS, CONDITION_ALWAYS, MSGNO(MsgFieldIfYouWantToGo)},
    {TRIGGER_TALK, ACTOR_VILLAGER_TUNDARIA, CONDITION_ALWAYS, MSGNO(MsgFieldIfYouLookAcrossThe)},
    {TRIGGER_TALK, ACTOR_VILLAGER_GAROH, CONDITION_ALWAYS, MSGNO(MsgFieldATownCalledGarohLies)},
    {TRIGGER_TALK, ACTOR_CHICKEN, CONDITION_ALWAYS, MSGNO(MsgFieldBukBukBuk)},
    {TRIGGER_TALK, ACTOR_SHEEP, CONDITION_ALWAYS, MSGNO(MsgFieldBahBaaaaaaa)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_NO_VISITORS, CONDITION_ALWAYS, MSGNO(MsgFieldWhyWouldAnyoneTravelAll)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_LOOKING_FOR_SOMEONE, CONDITION_ALWAYS, MSGNO(MsgFieldILikeThisTimeOf)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_AFTER_EARTHQUAKE, CONDITION_ALWAYS, MSGNO(MsgFieldOseniaWasBigAlreadyBut)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_DEEPER_INTO_OSENIA, CONDITION_ALWAYS, MSGNO(MsgFieldNoWayIDLeave)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_AIRS_ROCK, CONDITION_ALWAYS, MSGNO(MsgFieldAirSRockHasStood)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_TWO_WAYS, CONDITION_ALWAYS, MSGNO(MsgFieldItSWeirdThatYou)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_TUNDARIA, CONDITION_ALWAYS, MSGNO(MsgFieldItSBitterColdIn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_GAROH, CONDITION_ALWAYS, MSGNO(MsgFieldThePeopleInGarohAre)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHICKEN, CONDITION_ALWAYS, MSGNO(MsgFieldTheGroundHereIsSo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SHEEP, CONDITION_ALWAYS, MSGNO(MsgFieldOhYeahMmmIJust)},
    {TRIGGER_TALK, ACTOR_SPARK, CONDITION_ALWAYS, EVENT(Mikasalla_MeetSpark)},
    {EXAMINE_TRIGGER(EXAMINE_WELL), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgFieldItSCompletelyFullOf))},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), CELL_BURIED_LADDER, CONDITION_ALWAYS, EVENT(Mikasalla_DigUpLadder)},
    {TRIGGER_TABLE_END},
};
// clang-format on

/// The tunnels' exits, the hole in the chamber's floor, and the water Parch
/// dries.
// The rows for a ladder top entered from the north, west or east answer no
// cell of the tunnels' map: only the village's dug ladder has those.
struct Trigger gMikasalla_TunnelTriggers[] = {
    {TRIGGER_EXIT, MIKASALLA_NORTH_CHAMBER_HOLE, CONDITION_ALWAYS,
     MIKASALLA_NORTH_CHAMBER_HOLE},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_LADDER_TOP_FROM_NORTH,
     CONDITION_ALWAYS, EVENT(Mikasalla_TurnAndClimbDownLadder)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_LADDER_TOP_FROM_WEST,
     CONDITION_ALWAYS, EVENT(Mikasalla_StepOntoLadderFacingEast)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_LADDER_TOP_FROM_EAST,
     CONDITION_ALWAYS, EVENT(Mikasalla_StepOntoLadderFacingWest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_LADDER_TOP_FROM_SOUTH,
     CONDITION_ALWAYS, EVENT(Mikasalla_StepOntoLadderFacingNorth)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_LADDER_TOP_FROM_SOUTH_BESIDE,
     CONDITION_ALWAYS, EVENT(Mikasalla_StepOntoLadderAhead)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_PARCH, PSYNERGY_PHASE_ACTS),
     CELL_TUNNEL_WATER, FLAG_MIKASALLA_TUNNELS_PARCHED,
     EVENT(Mikasalla_CastParch)},
    {TRIGGER_TABLE_END},
};
