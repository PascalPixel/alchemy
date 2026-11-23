/// @file
/// The Lemurian Ship below deck, at the East Indra Shore, in two scenes
/// (SceneId_LemurianShipCabins and SceneId_LemurianShipAquaHydra) whose rooms
/// are told apart by the entrance used.
///
/// Piers leads the party down from the main deck through the captain's cabin
/// into rooms full of Aqua Jellies. Each jelly beaten melts into a puddle that
/// Frost turns into a pillar of ice to jump across, and tall crates are pushed
/// between them: two jellies in the first room, six and a crate in the room of
/// the lift, six more and a crate among the pipes of the room after it. A jelly
/// by the lift takes fright and flees down it; beaten below, it leaves a puddle
/// that Frost freezes to hold the lift up. In the hold the last jelly calls the
/// others together into the Aqua Hydra, and once it is beaten the hold floods,
/// keeping its chest out of reach until Parch dries it on a later visit. In the
/// room of two pillars below, the left pillar already stands sunk in its
/// square; Douse fills the right one and Move pushes it onto the tile marked
/// with a drop of water, where it sinks too and opens the arched doorway to the
/// engine room. There Piers sets the Black Crystal in its pedestal and the ship
/// sails.
#include "system/task.h"
#include "graphics/palette_buffers.h"
#include "sprite_ids.h"
#include "shared/tables.h"
#include "field/field_work.h"
#include "flags.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "field/scene_tables.h"
#include "scene_ids.h"
#include "scene/lemurian_ship.h"
#include "scene/scene.h"
#include "system/main_routine_work.h"
#include "system/ram_buffer.h"
#include "field/field_sprite.h"
#include "field/event_inline.h"
#include "system/frame.h"
#include "version.h"
#include "macros.h"
#include "field/actor_script.h"
#include "item_ids.h"
#include "text/msg_ids.h"

/// The Lemurian Ship's room of two jellies, by the rows of
/// gLemurianShip_TwoJellyRoomPlacements.
enum LemurianShipTwoJellyRoomActorId {
  ACTOR_TWO_JELLY_ROOM_JELLY_A = ACTOR_FIRST_PLACED,
  ACTOR_TWO_JELLY_ROOM_JELLY_B,
  ACTOR_TWO_JELLY_ROOM_PUDDLE_A,
  ACTOR_TWO_JELLY_ROOM_PUDDLE_B
};

/// The Lemurian Ship's room of six jellies and the lift, by the rows of
/// gLemurianShip_SixJellyRoomPlacements.
enum LemurianShipSixJellyRoomActorId {
  ACTOR_SIX_JELLY_ROOM_CRATE = ACTOR_FIRST_PLACED,
  ACTOR_SIX_JELLY_ROOM_JELLY_A,
  ACTOR_SIX_JELLY_ROOM_JELLY_B,
  ACTOR_SIX_JELLY_ROOM_JELLY_C,
  ACTOR_SIX_JELLY_ROOM_JELLY_D,
  ACTOR_SIX_JELLY_ROOM_JELLY_E,
  ACTOR_SIX_JELLY_ROOM_JELLY_F,
  ACTOR_SIX_JELLY_ROOM_LIFT_PUDDLE,
  ACTOR_SIX_JELLY_ROOM_PUDDLE_A,
  ACTOR_SIX_JELLY_ROOM_PUDDLE_B,
  ACTOR_SIX_JELLY_ROOM_PUDDLE_C,
  ACTOR_SIX_JELLY_ROOM_PUDDLE_D,
  ACTOR_SIX_JELLY_ROOM_PUDDLE_E
};

/// The Lemurian Ship's room of the crate on the ledge, by the rows of
/// gLemurianShip_LedgeRoomPlacements.
enum LemurianShipLedgeRoomActorId { ACTOR_LEDGE_CRATE = ACTOR_FIRST_PLACED };

/// The Lemurian Ship's room of the pipes, by the rows of
/// gLemurianShip_PipeRoomPlacements.
enum LemurianShipPipeRoomActorId {
  ACTOR_PIPE_ROOM_CRATE = ACTOR_FIRST_PLACED,
  ACTOR_PIPE_ROOM_JELLY_A,
  ACTOR_PIPE_ROOM_JELLY_B,
  ACTOR_PIPE_ROOM_JELLY_C,
  ACTOR_PIPE_ROOM_JELLY_D,
  ACTOR_PIPE_ROOM_JELLY_E,
  ACTOR_PIPE_ROOM_JELLY_F, ///< Unused: placed in the room's corner
  ACTOR_PIPE_ROOM_JELLY_G,
  ACTOR_PIPE_ROOM_PUDDLE_A,
  ACTOR_PIPE_ROOM_PUDDLE_B,
  ACTOR_PIPE_ROOM_PUDDLE_C,
  ACTOR_PIPE_ROOM_PUDDLE_D,
  ACTOR_PIPE_ROOM_PUDDLE_E,
  ACTOR_PIPE_ROOM_PUDDLE_F, ///< Unused: never placed
  ACTOR_PIPE_ROOM_PUDDLE_G
};

/// The Lemurian Ship's hold, where the jellies gather into the Aqua Hydra, by
/// the rows of gLemurianShip_HoldPlacements; the party's rows go to their
/// slots.
enum LemurianShipHoldActorId {
  ACTOR_HOLD_JELLY_A = ACTOR_FIRST_PLACED,
  ACTOR_HOLD_JELLY_B,
  ACTOR_HOLD_JELLY_C,
  ACTOR_HOLD_JELLY_D,
  ACTOR_HOLD_JELLY_E,
  ACTOR_HOLD_JELLY_F
};

/// The Lemurian Ship's room of the two pillars Douse fills, by the rows of
/// gLemurianShip_PillarRoomPlacements.
enum LemurianShipPillarRoomActorId {
  ACTOR_RIGHT_PILLAR = ACTOR_FIRST_PLACED,
  ACTOR_LEFT_PILLAR,
  ACTOR_RIGHT_PILLAR_PUDDLE,
  ACTOR_LEFT_PILLAR_PUDDLE
};

/// The Lemurian Ship's engine room, by the rows of
/// gLemurianShip_EngineRoomPlacements; the party's rows go to their slots.
enum LemurianShipEngineRoomActorId {
  ACTOR_ENGINE_ROOM_KRADEN = ACTOR_FIRST_PLACED
};

/// Each Aqua Jelly by its number, and last the Aqua Hydra they become: the
/// offset of its flags from FLAG_JELLIES_BEATEN, FLAG_JELLY_MELTING,
/// FLAG_PUDDLE_FROZEN and FLAG_FIGHTING_JELLY.
enum LemurianShipJelly {
  JELLY_TWO_JELLY_ROOM_A,
  JELLY_TWO_JELLY_ROOM_B,
  JELLY_SIX_JELLY_ROOM_A, ///< the jelly by the lift
  JELLY_SIX_JELLY_ROOM_B,
  JELLY_SIX_JELLY_ROOM_C,
  JELLY_SIX_JELLY_ROOM_D,
  JELLY_SIX_JELLY_ROOM_E,
  JELLY_SIX_JELLY_ROOM_F,
  JELLY_PIPE_ROOM_A,
  JELLY_PIPE_ROOM_B,
  JELLY_PIPE_ROOM_C,
  JELLY_PIPE_ROOM_D,
  JELLY_PIPE_ROOM_E,
  JELLY_PIPE_ROOM_F, ///< placed in the room's corner and never fought
  JELLY_PIPE_ROOM_G,
  JELLY_AQUA_HYDRA,
  JELLY_COUNT
};

/// The battles below deck by their slot of ENCOUNTER_TABLE_AQUA_JELLIES: an
/// Aqua Jelly, or the Aqua Hydra.
enum LemurianShipEncounter { ENCOUNTER_AQUA_JELLY, ENCOUNTER_AQUA_HYDRA };

#define FLAG_JELLIES_BEATEN 0x930 ///< the first, by enum LemurianShipJelly
#define FLAG_AQUA_HYDRA_BEATEN (FLAG_JELLIES_BEATEN + JELLY_AQUA_HYDRA)
#define FLAG_PILLAR_WAY_OPEN 0x982     ///< both pillars sank, the way opened
#define FLAG_SHIP_ENGINE_RUNNING 0x983 ///< the Black Crystal is set
#define FLAG_HOLD_PARCHED 0x9e4        ///< Parch dried the flooded hold
#define FLAG_CRATE_DROPPED FLAG_SCENE_TEMP_00
#define FLAG_JELLY_FLED FLAG_SCENE_TEMP_01 ///< the frightened jelly ran
#define FLAG_RIGHT_PILLAR_SUNK FLAG_SCENE_TEMP_02
#define FLAG_LEFT_PILLAR_SUNK FLAG_SCENE_TEMP_03

/// The pillar room's raised triggers: a pillar's puddle has dried, and the
/// pillar rises.
enum LemurianShipRaisedTrigger {
  RAISED_RIGHT_PILLAR = 200,
  RAISED_LEFT_PILLAR
};
#define FLAG_RIGHT_PILLAR_FULL FLAG_SCENE_TEMP_04 ///< Douse filled it
#define FLAG_LEFT_PILLAR_FULL FLAG_SCENE_TEMP_05
#define FLAG_LIFT_LOWERED FLAG_SCENE_TEMP_07
/// The first, by enum LemurianShipJelly: Frost froze the jelly's puddle.
#define FLAG_PUDDLE_FROZEN FLAG_SCENE_TEMP_10
/// The pillar room's puddles share the room of two jellies' puddle rows
/// (gLemurianShip_TwoJellyRoomPuddles) and so its flags: Frost froze the
/// puddle on the pillar.
#define FLAG_RIGHT_PILLAR_PUDDLE_FROZEN                                        \
  (FLAG_PUDDLE_FROZEN + JELLY_TWO_JELLY_ROOM_A)
#define FLAG_LEFT_PILLAR_PUDDLE_FROZEN                                         \
  (FLAG_PUDDLE_FROZEN + JELLY_TWO_JELLY_ROOM_B)
/// The first, by enum LemurianShipJelly: the jelly was just beaten and melts on
/// arrival.
#define FLAG_JELLY_MELTING FLAG_SCENE_TEMP_20
/// The first, by enum LemurianShipJelly: the battle with the jelly is on.
#define FLAG_FIGHTING_JELLY FLAG_AREA_TEMP_00

/// The cabins' entrances from within the scene, by the room each opens into and
/// the room it leads from; scene_exits.h names those from the main deck and the
/// lower deck. The main deck's door leads into the room of the captain's
/// logbook, and four halls join the rooms: one off the room of two jellies, one
/// off the stair room, and two off the room of six jellies' west and east
/// doors.
enum LemurianShipCabinsEntrance {
  ENTRANCE_LOGBOOK_ROOM_FROM_TWO_JELLY_ROOM = 2,
  ENTRANCE_TWO_JELLY_ROOM_FROM_LOGBOOK_ROOM,
  ENTRANCE_TWO_JELLY_ROOM_FROM_STAIR_ROOM,
  ENTRANCE_TWO_JELLY_ROOM_FROM_TWO_JELLY_ROOM_HALL,
  ENTRANCE_STAIR_ROOM_FROM_TWO_JELLY_ROOM,
  ENTRANCE_STAIR_ROOM_FROM_STAIR_ROOM_HALL,
  ENTRANCE_SIX_JELLY_ROOM_WEST_HALL_FROM_SIX_JELLY_ROOM = 10,
  ENTRANCE_SIX_JELLY_ROOM_EAST_HALL_FROM_SIX_JELLY_ROOM,
  ENTRANCE_SIX_JELLY_ROOM_EAST_HALL_FROM_PIPE_ROOM,
  ENTRANCE_TWO_JELLY_ROOM_HALL_FROM_TWO_JELLY_ROOM,
  ENTRANCE_LEDGE_ROOM_FROM_SIX_JELLY_ROOM_WEST = 16,
  ENTRANCE_LEDGE_ROOM_FROM_SIX_JELLY_ROOM_EAST,
  ENTRANCE_LEDGE_ROOM_FROM_PIPE_ROOM,
  ENTRANCE_SIX_JELLY_ROOM_FROM_WEST_HALL = 25,
  ENTRANCE_SIX_JELLY_ROOM_FROM_EAST_HALL,
  ENTRANCE_SIX_JELLY_ROOM_FROM_LEDGE_ROOM_WEST,
  ENTRANCE_SIX_JELLY_ROOM_FROM_LEDGE_ROOM_EAST,
  ENTRANCE_PIPE_ROOM_FROM_SIX_JELLY_ROOM_EAST_HALL,
  ENTRANCE_PIPE_ROOM_FROM_LEDGE_ROOM
};

/// The lower deck's entrances from within the scene: the pillar room, the
/// corridor beyond it and the engine room.
enum LemurianShipLowerDeckEntrance {
  ENTRANCE_PILLAR_ROOM_FROM_CORRIDOR = 6,
  ENTRANCE_CORRIDOR_FROM_PILLAR_ROOM,
  ENTRANCE_CORRIDOR_FROM_ENGINE_ROOM,
  /// Until the engine runs; Piers sets the Black Crystal here.
  ENTRANCE_ENGINE_ROOM_FROM_CORRIDOR,
  /// Where the Aqua Hydra's battle returns the party, in the flooded hold.
  ENTRANCE_HOLD_AFTER_AQUA_HYDRA_BATTLE = 15,
  /// Once the engine runs: the engine room lit.
  ENTRANCE_RUNNING_ENGINE_ROOM_FROM_CORRIDOR = 19
};

extern struct SceneEntrance gLemurianShip_AquaHydraEntrances[];

u8 *Scene_GetEntrances(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_LEMURIAN_SHIP_CABINS)
    return gLemurianShip_CabinsEntrances;
  if (scene == SCENE_LEMURIAN_SHIP_AQUA_HYDRA)
    return (u8 *)gLemurianShip_AquaHydraEntrances;
  return gLemurianShip_Entrances;
}

u8 *Scene_GetExitRegions(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_LEMURIAN_SHIP_CABINS)
    return gLemurianShip_CabinsRegions;
  if (scene == SCENE_LEMURIAN_SHIP_AQUA_HYDRA)
    return gLemurianShip_AquaHydraRegions;
  return 0;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

void Event_Start(void);
void Event_End(void);
void FlagBlock_ApplyFlags(const void *script);
void Actor_SetSpriteFlags(struct Actor *actor, u32 value);
void FieldPsynergy_RunHandler(s32 type);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);

void FlagBlock_PlaceAll(const void *script);
void Event_StartScreenTransition(void);

void Actor_PlaceOnActor(s32 actor, s32 target);

/// The cabin doors (gLemurianShip_Doors).
#define CABIN_DOOR_COUNT 10
/// The lower deck's doors (gLemurianShip_LowerDoors); the hold's two doors
/// follow, as they open while it is flooded.
#define LOWER_DOOR_COUNT 4

void MapGrid_CenterOnFollowedPoint(void);
void Field_RequestBattleFromEncounterTable(s32 a, s32 b);
void Actor_LiftTargetBlock(const void *script);
void Actor_PlaceMovedBlock(void);

void LemurianShip_LeapIntoHydra(s32 id);
void LemurianShip_CheckPillarsOnDropTiles(void);
void LemurianShip_SetUpPuddle(s32 id);
void LemurianShip_SetUpLowPuddle(s32 id);

extern s32 gLemurianShip_JellyMelts[];
extern s32 gLemurianShip_PuddleSpreads[];
extern s32 gLemurianShip_HydraForms[];
extern s16 gLemurianShip_Crate[];
extern s16 gLemurianShip_TwoJellyRoomPuddles[];
extern s16 gLemurianShip_SixJellyRoomPuddles[];
extern s16 gLemurianShip_PipeRoomPuddles[];
extern s16 gLemurianShip_Pillars[];

void LemurianShip_JellyFlees(void);
void LemurianShip_SetBlackCrystal(void);
void Actor_StartScript(struct Actor *actor, const void *script);
void Actor_PlaceBlocks(const void *script);
extern u32 gLemurianShip_LightToOrb[];

extern struct ActorPlacement gLemurianShip_LedgeRoomPlacements[];
extern struct ActorPlacement gLemurianShip_PipeRoomPlacements[];

struct ActorPlacement *Scene_GetActorPlacements(void) {
  if (gMainRoutineWork.next == SCENE_LEMURIAN_SHIP_CABINS) {
    switch (gMainRoutineWork.entrance) {
    case ENTRANCE_TWO_JELLY_ROOM_FROM_LOGBOOK_ROOM:
    case ENTRANCE_TWO_JELLY_ROOM_FROM_STAIR_ROOM:
    case ENTRANCE_TWO_JELLY_ROOM_FROM_TWO_JELLY_ROOM_HALL:
      return gLemurianShip_TwoJellyRoomPlacements;
    case ENTRANCE_SIX_JELLY_ROOM_FROM_WEST_HALL:
    case ENTRANCE_SIX_JELLY_ROOM_FROM_EAST_HALL:
    case ENTRANCE_SIX_JELLY_ROOM_FROM_LEDGE_ROOM_WEST:
    case ENTRANCE_SIX_JELLY_ROOM_FROM_LEDGE_ROOM_EAST:
      return gLemurianShip_SixJellyRoomPlacements;
    case LEMURIAN_SHIP_CABINS_FROM_LEMURIAN_SHIP_AQUA_HYDRA_NORTHEAST_A:
    case ENTRANCE_LEDGE_ROOM_FROM_SIX_JELLY_ROOM_WEST:
    case ENTRANCE_LEDGE_ROOM_FROM_SIX_JELLY_ROOM_EAST:
    case ENTRANCE_LEDGE_ROOM_FROM_PIPE_ROOM:
      return gLemurianShip_LedgeRoomPlacements;
    case ENTRANCE_PIPE_ROOM_FROM_SIX_JELLY_ROOM_EAST_HALL:
    case ENTRANCE_PIPE_ROOM_FROM_LEDGE_ROOM:
      return gLemurianShip_PipeRoomPlacements;
    }
  } else if (gMainRoutineWork.next == SCENE_LEMURIAN_SHIP_AQUA_HYDRA) {
    switch (gMainRoutineWork.entrance) {
    case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_NORTHWEST_A:
    case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_NORTH_A:
    case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_WEST_A:
    case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_NORTHEAST_A:
    case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_SOUTHWEST:
    case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_SOUTH_A:
    case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_SOUTH_B:
    case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_SOUTH_C:
    case ENTRANCE_HOLD_AFTER_AQUA_HYDRA_BATTLE:
    case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_NORTHWEST_B:
    case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_NORTH_B:
    case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_WEST_B:
    case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_NORTHEAST_B:
      return gLemurianShip_HoldPlacements;
    case ENTRANCE_PILLAR_ROOM_FROM_CORRIDOR:
    case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_EAST:
      return gLemurianShip_PillarRoomPlacements;
    default:
      return gLemurianShip_EngineRoomPlacements;
    }
  }
  return gLemurianShip_Placements;
}

/// Felix walks north into one of the cabins' wooden doors: it swings open, he
/// steps through, and the screen closes and opens on the room beyond.
///
/// The doors are gLemurianShip_Doors, found by the cell's step trigger, which
/// is also the exit taken; the first is the captain's cabin's, cell (4, 4).
/// Every placed actor stops moving while he goes.
void LemurianShip_OpenDoor(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *actor;
  u32 found;
  u32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  found = 0;
  for (i = ACTOR_FIRST_PLACED; i < FIELD_TREASURE_ACTOR; i++) {
    actor = ((struct Actor *)Actor_GetById(i));
    if (actor != NULL)
      actor->motionFlags = 0;
  }
  for (i = 0; i < CABIN_DOOR_COUNT; i++) {
    if (work->stepTrigger == gLemurianShip_Doors[i].exit)
      found = i;
  }
  Event_PlaySound(SE_DOOR_OPEN);
  Actor_SetSpritePriority(gMainRoutineWork.leaderActorId, 1);
  MapGrid_CopyCellsTo(0, 0, gLemurianShip_Doors[found].x,
                      gLemurianShip_Doors[found].z, 1, 2);
  MapGrid_AnimateCells(gLemurianShip_Doors[found].value,
                       gLemurianShip_Doors[found].x,
                       gLemurianShip_Doors[found].z + 60);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_WALK);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.5);
  {
    s32 owner = gMainRoutineWork.leaderActorId;

    // DEAD: found never reaches 12 among the ten doors; the ROM keeps the test.
    if (found == 12)
      Actor_SetDestinationOffset(owner, 0, -4);
    else
      Actor_CenterAndWalk(owner, 2, -4);
  }
  Event_Wait(4);
  Import_EventSetExitRequest(work->stepTrigger);
}

/// Felix walks north into one of the two doors at the top of the hold, cells
/// (4, 4) and (10, 4): it swings open and he steps through.
///
/// While the hold is flooded (after the Aqua Hydra, until Parch) the party
/// walks a copy of the hold drawn 19 rows further south, and the doors there,
/// cells (4, 23) and (10, 23), are the last rows of gLemurianShip_LowerDoors.
void LemurianShip_OpenLowerDoor(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *actor;
  u32 found;
  u32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  found = 0;
  for (i = ACTOR_FIRST_PLACED; i < FIELD_TREASURE_ACTOR; i++) {
    actor = ((struct Actor *)Actor_GetById(i));
    if (actor != NULL)
      actor->motionFlags = 0;
  }
  for (i = 0; i < LOWER_DOOR_COUNT; i++) {
    if (work->stepTrigger == gLemurianShip_LowerDoors[i].exit)
      found = i;
  }
  if (found < 2 && Event_GetFlag(FLAG_AQUA_HYDRA_BEATEN) &&
      Event_GetFlag(FLAG_HOLD_PARCHED) == 0)
    found += LOWER_DOOR_COUNT;
  Event_PlaySound(SE_DOOR_OPEN);
  Actor_SetSpritePriority(gMainRoutineWork.leaderActorId, 1);
  MapGrid_CopyCellsTo(0, 0, gLemurianShip_LowerDoors[found].x,
                      gLemurianShip_LowerDoors[found].z, 1, 2);
  MapGrid_AnimateCells(gLemurianShip_LowerDoors[found].value,
                       gLemurianShip_LowerDoors[found].x,
                       gLemurianShip_LowerDoors[found].z + 60);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_WALK);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.5);
  Actor_CenterOnColumn(gMainRoutineWork.leaderActorId, 2, -4);
  Event_Wait(4);
  Import_EventSetExitRequest(work->stepTrigger);
}

/// Once both pillars have sunk, Felix walks north into the arched doorway at
/// the top of the room of two pillars, cell (32, 6), and shrinks away into the
/// passage as the screen closes. Before that, walking into it does nothing.
void LemurianShip_WalkIntoPassage(void) {
  if (Event_GetFlag(FLAG_PILLAR_WAY_OPEN)) {
    struct FieldWork *work = Ram_HeapSlots->fieldWork;
    struct MainRoutineWork *party = &gMainRoutineWork;
    struct Actor *leader;
    FIXED x;
    s32 pixelX;

    leader = ((struct Actor *)Actor_GetById(party->leaderActorId));
    Event_Start();
    Event_ResolvePendingActions(0);
    FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
    Actor_SetAnimation(party->leaderActorId, ANIM_WALK);
    Actor_SetSpeed(party->leaderActorId, 0.5);
    x = leader->x.fixed;
    pixelX = FIXEDtoINT(x);
    Actor_SetMoveTarget(leader, INTtoFIXED(8 - pixelX % 16) + x,
                        leader->y.fixed, leader->z.fixed);
    FieldActor_WaitForMove(leader);
    leader->facing = FACING_NORTH;
    Actor_SetScript(party->leaderActorId, gLeader_WalkThroughDoorScript);
    wait(12);
    Event_PlaySound(SE_MAP_EXIT);
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Import_EventSetExitRequest(work->stepTrigger);
  }
}

extern struct Trigger gLemurianShip_LedgeRoomTriggers[];
extern struct Trigger gLemurianShip_HoldTriggers[];
extern struct Trigger gLemurianShip_PillarRoomTriggers[];

const struct Trigger *Scene_GetTriggers(void) {
  if (gMainRoutineWork.next == SCENE_LEMURIAN_SHIP_CABINS) {
    switch (gMainRoutineWork.entrance) {
    case ENTRANCE_TWO_JELLY_ROOM_FROM_LOGBOOK_ROOM:
    case ENTRANCE_TWO_JELLY_ROOM_FROM_STAIR_ROOM:
    case ENTRANCE_TWO_JELLY_ROOM_FROM_TWO_JELLY_ROOM_HALL:
      return gLemurianShip_TwoJellyRoomTriggers;
    case ENTRANCE_SIX_JELLY_ROOM_FROM_WEST_HALL:
    case ENTRANCE_SIX_JELLY_ROOM_FROM_EAST_HALL:
    case ENTRANCE_SIX_JELLY_ROOM_FROM_LEDGE_ROOM_WEST:
    case ENTRANCE_SIX_JELLY_ROOM_FROM_LEDGE_ROOM_EAST:
      return gLemurianShip_SixJellyRoomTriggers;
    case LEMURIAN_SHIP_CABINS_FROM_LEMURIAN_SHIP_AQUA_HYDRA_NORTHEAST_A:
    case ENTRANCE_LEDGE_ROOM_FROM_SIX_JELLY_ROOM_WEST:
    case ENTRANCE_LEDGE_ROOM_FROM_SIX_JELLY_ROOM_EAST:
    case ENTRANCE_LEDGE_ROOM_FROM_PIPE_ROOM:
      return gLemurianShip_LedgeRoomTriggers;
    case ENTRANCE_PIPE_ROOM_FROM_SIX_JELLY_ROOM_EAST_HALL:
    case ENTRANCE_PIPE_ROOM_FROM_LEDGE_ROOM:
      return gLemurianShip_PipeRoomTriggers;
    default:
      return gLemurianShip_CabinTriggers;
    }
  } else if (gMainRoutineWork.next == SCENE_LEMURIAN_SHIP_AQUA_HYDRA) {
    switch (gMainRoutineWork.entrance) {
    case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_NORTHWEST_A:
    case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_NORTH_A:
    case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_WEST_A:
    case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_NORTHEAST_A:
    case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_SOUTHWEST:
    case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_SOUTH_A:
    case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_SOUTH_B:
    case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_SOUTH_C:
    case ENTRANCE_HOLD_AFTER_AQUA_HYDRA_BATTLE:
    case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_NORTHWEST_B:
    case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_NORTH_B:
    case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_WEST_B:
    case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_NORTHEAST_B:
      return gLemurianShip_HoldTriggers;
    case ENTRANCE_PILLAR_ROOM_FROM_CORRIDOR:
    case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_EAST:
      return gLemurianShip_PillarRoomTriggers;
    default:
      return gLemurianShip_LowerDeckTriggers;
    }
  }
  return gLemurianShip_Triggers;
}

/// Felix checks the lever beside the lift in the room of six jellies, cell
/// (53, 3): before the engine runs, "There's no reaction... It doesn't seem to
/// be running."; once it runs, the lift goes down or comes back up with a
/// crunch, or does nothing while the frozen puddle holds it up.
void LemurianShip_ExamineLiftSwitch(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  if (Event_GetFlag(FLAG_SHIP_ENGINE_RUNNING)) {
    if (Event_GetFlag(FLAG_LIFT_LOWERED)) {
      Event_PlaySound(SE_CRUNCH);
      FieldActor_SetPositionAndResetMotion(
          ((struct Actor *)Actor_GetById(ACTOR_SIX_JELLY_ROOM_LIFT_PUDDLE)),
          toFIXED(824), -toFIXED(32), toFIXED(72));
      ((struct Actor *)Actor_GetById(ACTOR_SIX_JELLY_ROOM_LIFT_PUDDLE))
          ->groundHeight = -toFIXED(32);
      MapGrid_CopyCellsTo(72, 68, 50, 64, 3, 3);
      MapGrid_CopyCellAttributes(106, 16, 3, 2, 110, 4);
      MapGrid_CopyCellAttributes(50, 1, 3, 1, 50, 4);
      Event_ClearFlag(FLAG_LIFT_LOWERED);
    } else if (Event_GetFlag((FLAG_PUDDLE_FROZEN + JELLY_SIX_JELLY_ROOM_A))) {
      Event_PlaySound(SE_ITEM_REMOVE);
    } else {
      Event_PlaySound(SE_CRUNCH);
      Actor_SetPosition(ACTOR_SIX_JELLY_ROOM_LIFT_PUDDLE, 0, 0);
      MapGrid_CopyCellsTo(68, 68, 50, 64, 3, 3);
      MapGrid_CopyCellAttributes(102, 16, 3, 2, 110, 4);
      MapGrid_CopyCellAttributes(50, 0, 3, 1, 50, 4);
      Event_SetFlag(FLAG_LIFT_LOWERED);
    }
  } else {
    // "There's no reaction... It doesn't seem to be running."
    FieldMessage_Show((s32)MsgLemurianShipTheresNoReactionItDoesnt, 1);
  }
  Event_End();
}

/// Nothing visible happens as Felix starts to Move or push the tall crate in
/// the room of six jellies or the room of the pipes; the game takes the crate
/// up so that it can slide (`Actor_LiftTargetBlock`).
void LemurianShip_LiftCrate(void) {
  Actor_LiftTargetBlock(gLemurianShip_Crate);
}

/// Felix slides the tall crate one cell with Move or a push, in the room of six
/// jellies or the room of the pipes; it stays where it stops, so that the
/// frozen puddles and the crate make a row to jump along.
void LemurianShip_SetDownCrate(void) { Actor_PlaceMovedBlock(); }

/// Nothing visible happens as Felix starts to Move or push the tall crate on
/// the ledge by the stairs; the game takes the crate up so that it can slide.
void LemurianShip_LiftLedgeCrate(void) {
  Actor_LiftTargetBlock(gLemurianShip_Crate);
}

/// Felix slides the tall crate along the ledge by the stairs with Move or a
/// push. Once it reaches cell (16, 46), at the ledge's edge, it falls with a
/// thump to the floor below and stays there.
void LemurianShip_PushCrateOffLedge(void) {
  struct Actor *actor;
  s32 x;
  s32 moving;

  Actor_PlaceMovedBlock();
  actor = ((struct Actor *)Actor_GetById(ACTOR_LEDGE_CRATE));
  x = FIELD_CELL(actor->x.fixed);
  if (x == 16) {
    Event_Start();
    Event_ResolvePendingActions(0);
    MapGrid_CopyCellAttributes(16, 47, 1, 1, x, 46);
    wait(1);
    actor->motionFlags = 3;
    do {
      wait(1);
      moving = actor->velocityY;
    } while (moving != 0);
    Event_PlaySound(SE_THUMP);
    wait(10);
    actor->motionFlags = moving;
    Event_SetFlag(FLAG_CRATE_DROPPED);
    MapGrid_CopyCellAttributes(14, 46, 1, 1, 16, 46);
    Event_End();
  }
}

/// Felix casts Frost on the puddle a jelly left in the first room below deck:
/// it freezes into a pillar of ice he can jump onto, the way to the room's
/// chest.
void LemurianShip_FreezeTwoJellyRoomPuddle(void) {
  FlagBlock_ApplyFlags(gLemurianShip_TwoJellyRoomPuddles);
}

/// Felix casts Frost on a puddle a jelly left in the room of six jellies: it
/// freezes into a pillar of ice to jump along.
void LemurianShip_FreezeSixJellyRoomPuddle(void) {
  FlagBlock_ApplyFlags(gLemurianShip_SixJellyRoomPuddles);
}

/// Felix casts Frost on the puddle the frightened jelly left beside the lift in
/// the room of six jellies: it freezes into ice that keeps the lift up, so that
/// stepping onto it no longer drops it.
void LemurianShip_FreezeLiftPuddle(void) {
  MapGrid_CopyCellAttributes(110, 1, 3, 1, 110, 4);
  Event_SetFlag((FLAG_PUDDLE_FROZEN + JELLY_SIX_JELLY_ROOM_A));
}

/// Felix casts Frost on a puddle a jelly left in the room of the pipes: it
/// freezes into a pillar of ice to jump along.
void LemurianShip_FreezePipeRoomPuddle(void) {
  FlagBlock_ApplyFlags(gLemurianShip_PipeRoomPuddles);
}

/// Felix steps onto the grated lift at the top right of the room of six
/// jellies from the east, cell (52, 4), before the engine runs and before its
/// puddle is frozen: the lift shudders and sinks, Felix jumps back off, and it
/// comes back up with a crunch. While the jelly that fled down it is still
/// unbeaten, it rides up on the lift and hops off beside it.
void LemurianShip_StepOnLift(void) {
  struct Actor *leader;
  struct Actor *helper;
  s32 opened;

  if (!Event_GetFlag(FLAG_SHIP_ENGINE_RUNNING)) {
    leader = ((struct Actor *)Actor_GetById(gMainRoutineWork.leaderActorId));
    Event_Start();
    Event_ResolvePendingActions(0);
    Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.8);
    Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 840, 72);
    opened = Event_GetFlag((FLAG_JELLIES_BEATEN + JELLY_SIX_JELLY_ROOM_A));
    if (opened) {
      Event_PlaySound(SE_LEMURIAN_SHIP_LIFT);
      Actor_SetAttachedEffect(gMainRoutineWork.leaderActorId,
                              ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
      MapGrid_SetShake(0, 2, 1);
      Event_Wait(20);
      MapGrid_SetShake(0, 0, 0);
      MapGrid_CopyCellsTo(68, 68, 50, 64, 3, 3);
      leader->layer = 2;
      Event_Wait(20);
      Actor_Jump(gMainRoutineWork.leaderActorId, 4, 0);
      Actor_SetSpeed(gMainRoutineWork.leaderActorId, 1.6);
      Actor_MoveToAndWait(gMainRoutineWork.leaderActorId, 840, 88);
      Event_Wait(20);
      Event_PlaySound(SE_CRUNCH);
      MapGrid_CopyCellsTo(72, 68, 50, 64, 3, 3);
      Event_Wait(20);
      leader->layer = 1;
    } else {
      helper = ((struct Actor *)Actor_GetById(ACTOR_SIX_JELLY_ROOM_JELLY_A));
      Actor_SetSpritePriority(ACTOR_SIX_JELLY_ROOM_JELLY_A, 2);
      Actor_SetSpeed(ACTOR_SIX_JELLY_ROOM_JELLY_A, 2.4);
      ((struct Actor *)Actor_GetById(ACTOR_SIX_JELLY_ROOM_JELLY_A))
          ->steeringFlags &= ~ACTOR_FACES_MOTION;
      Actor_WalkToAndWait(ACTOR_SIX_JELLY_ROOM_JELLY_A, 824, 88);
      Event_Wait(1);
      ActorFlags_Set(
          &((struct Actor *)Actor_GetById(ACTOR_SIX_JELLY_ROOM_JELLY_A))
               ->steeringFlags,
          ACTOR_FACES_MOTION);
      Event_PlaySound(SE_LEMURIAN_SHIP_LIFT);
      Actor_SetAttachedEffect(gMainRoutineWork.leaderActorId,
                              ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
      MapGrid_SetShake(0, 2, 1);
      Event_Wait(20);
      MapGrid_SetShake(0, 0, 0);
      MapGrid_CopyCellsTo(68, 68, 50, 64, 3, 3);
      leader->layer = 2;
      Event_Wait(20);
      Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_SOUTHWEST, 20);
      Actor_Jump(gMainRoutineWork.leaderActorId, 4, 0);
      Actor_SetSpeed(gMainRoutineWork.leaderActorId, 1.6);
      Actor_MoveToAndWait(gMainRoutineWork.leaderActorId, 840, 88);
      Actor_FaceDirectionAndWait(gMainRoutineWork.leaderActorId, FACING_WEST);
      Actor_WatchAndSetScript(gMainRoutineWork.leaderActorId, 9);
      helper->gravity = toFIXED(1);
      helper->layer = 2;
      Actor_Jump(ACTOR_SIX_JELLY_ROOM_JELLY_A, 4, 0);
      Actor_MoveToAndWait(ACTOR_SIX_JELLY_ROOM_JELLY_A, 832, 74);
      Actor_Jump(ACTOR_SIX_JELLY_ROOM_JELLY_A, 4, 0);
      Actor_MoveToAndWait(ACTOR_SIX_JELLY_ROOM_JELLY_A, 856, 88);
      Actor_SetAttachedEffect(gMainRoutineWork.leaderActorId,
                              ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
      Event_Wait(20);
      Event_PlaySound(SE_CRUNCH);
      MapGrid_CopyCellsTo(72, 68, 50, 64, 3, 3);
      Event_Wait(20);
      Actor_Stop(gMainRoutineWork.leaderActorId);
      leader->layer = 1;
      helper->layer = 0;
      wait(1);
      Event_ClearFlag(FLAG_JELLY_FLED);
    }
    Event_End();
  }
}

/// Felix steps onto the grated lift from its west side, cell (50, 4), before
/// the engine runs and before its puddle is frozen: as from the east, the lift
/// shudders and sinks, Felix jumps back off, and it comes back up with a
/// crunch.
void LemurianShip_StepOnLiftWestSide(void) {
  struct Actor *leader;

  if (!Event_GetFlag(FLAG_SHIP_ENGINE_RUNNING)) {
    leader = ((struct Actor *)Actor_GetById(gMainRoutineWork.leaderActorId));
    Event_Start();
    Event_ResolvePendingActions(0);
    Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.8);
    Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 812, 72);
    Event_PlaySound(SE_LEMURIAN_SHIP_LIFT);
    Actor_SetAttachedEffect(gMainRoutineWork.leaderActorId,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    MapGrid_SetShake(0, 2, 1);
    Event_Wait(20);
    MapGrid_SetShake(0, 0, 0);
    MapGrid_CopyCellsTo(68, 68, 50, 64, 3, 3);
    leader->layer = 2;
    Event_Wait(20);
    Actor_Jump(gMainRoutineWork.leaderActorId, 4, 0);
    Actor_SetSpeed(gMainRoutineWork.leaderActorId, 3.2);
    Actor_MoveToAndWait(gMainRoutineWork.leaderActorId, 812, 88);
    Event_Wait(20);
    Event_PlaySound(SE_CRUNCH);
    MapGrid_CopyCellsTo(72, 68, 50, 64, 3, 3);
    Event_Wait(20);
    leader->layer = 1;
  }
}

#define DEFINE_JELLY_BATTLE(name, firstJelly, firstActor)                      \
  void name(s32 id) {                                                          \
    Event_Start();                                                             \
    Event_ResolvePendingActions(0);                                            \
    Actor_FaceActorById(id, gMainRoutineWork.leaderActorId, 0);                \
    Actor_StartRepeatedMotion(id, 2);                                          \
    Actor_SetAttachedEffect(id,                                                \
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);  \
    Event_Wait(20);                                                            \
    gMainRoutineWork.battleType = BATTLE_TYPE_NO_ESCAPE;                       \
    Field_RequestBattleFromEncounterTable(ENCOUNTER_TABLE_AQUA_JELLIES,        \
                                          ENCOUNTER_AQUA_JELLY);               \
    GameFlag_Set(id + (FLAG_FIGHTING_JELLY + (firstJelly) - (firstActor)));    \
  }

/// Felix talks to an Aqua Jelly in the first room below deck: it turns to him
/// and quivers, and a battle starts that cannot be run from. Its
/// `FLAG_FIGHTING_JELLY` flag lets a win melt it into a puddle.
DEFINE_JELLY_BATTLE(LemurianShip_FightTwoJellyRoomJelly, JELLY_TWO_JELLY_ROOM_A,
                    ACTOR_TWO_JELLY_ROOM_JELLY_A)

/// Felix talks to an Aqua Jelly in the room of six jellies: it turns to him and
/// quivers, and a battle starts that cannot be run from. Its
/// `FLAG_FIGHTING_JELLY` flag lets a win melt it into a puddle.
DEFINE_JELLY_BATTLE(LemurianShip_FightSixJellyRoomJelly, JELLY_SIX_JELLY_ROOM_A,
                    ACTOR_SIX_JELLY_ROOM_JELLY_A)

/// Felix talks to an Aqua Jelly in the room of the pipes: it turns to him
/// and quivers, and a battle starts that cannot be run from. Its
/// `FLAG_FIGHTING_JELLY` flag lets a win melt it into a puddle.
DEFINE_JELLY_BATTLE(LemurianShip_FightPipeRoomJelly, JELLY_PIPE_ROOM_A,
                    ACTOR_PIPE_ROOM_JELLY_A)

/// A jelly in the hold leaps into the middle of it, where the Aqua Hydra
/// forms.
void LemurianShip_LeapIntoHydra(s32 id) {
  struct Actor *actor = ((struct Actor *)Actor_GetById(id));

  Actor_SetSpeed(id, 3.2);
  actor->gravity = toFIXED(1) / 2;
  actor->velocityY = toFIXED(8);
  Actor_SetDestination(id, 120, 120);
}

/// Felix talks to the jelly in the middle of the hold: the five around it
/// quiver, cry out and leap into it, and it swells into the Aqua Hydra, whose
/// battle cannot be run from.
void LemurianShip_FightAquaHydra(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_PlaySound(MUS_SILENCE);
  Actor_StartRepeatedMotion(ACTOR_HOLD_JELLY_B, 2);
  Actor_StartRepeatedMotion(ACTOR_HOLD_JELLY_C, 2);
  Actor_StartRepeatedMotion(ACTOR_HOLD_JELLY_D, 2);
  Actor_StartRepeatedMotion(ACTOR_HOLD_JELLY_E, 2);
  Actor_RunRepeatedMotion(ACTOR_HOLD_JELLY_F, 2);
  Event_Wait(20);
  Event_PlaySound(SE_BEAST_CRY);
  LemurianShip_LeapIntoHydra(ACTOR_HOLD_JELLY_B);
  LemurianShip_LeapIntoHydra(ACTOR_HOLD_JELLY_C);
  LemurianShip_LeapIntoHydra(ACTOR_HOLD_JELLY_D);
  LemurianShip_LeapIntoHydra(ACTOR_HOLD_JELLY_E);
  LemurianShip_LeapIntoHydra(ACTOR_HOLD_JELLY_F);
  Actor_WaitForMoveById(ACTOR_HOLD_JELLY_F);
  Actor_SetPosition(ACTOR_HOLD_JELLY_B, 0, 0);
  Actor_SetPosition(ACTOR_HOLD_JELLY_C, 0, 0);
  Actor_SetPosition(ACTOR_HOLD_JELLY_D, 0, 0);
  Actor_SetPosition(ACTOR_HOLD_JELLY_E, 0, 0);
  Actor_SetPosition(ACTOR_HOLD_JELLY_F, 0, 0);
  Event_Wait(20);
  Event_PlaySound(MUS_AQUA_HYDRA);
  Actor_RunScriptAndWait(ACTOR_HOLD_JELLY_A, gLemurianShip_HydraForms);

  gMainRoutineWork.battleType = BATTLE_TYPE_NO_ESCAPE;
  Event_SetAfterBattleDestination(SCENE_LEMURIAN_SHIP_AQUA_HYDRA,
                                  ENTRANCE_HOLD_AFTER_AQUA_HYDRA_BATTLE);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_TABLE_AQUA_JELLIES,
                                        ENCOUNTER_AQUA_HYDRA);
  Event_SetFlag((FLAG_FIGHTING_JELLY + JELLY_AQUA_HYDRA));
}

/// In the room of two pillars, a pillar full of water that stands on its tile
/// marked with a drop of water, cell (35, 9) for the right one and (29, 9) for
/// the left, sinks slowly into the floor with a rumble. Once both have sunk, a
/// thump opens the arched doorway to the engine room.
void LemurianShip_CheckPillarsOnDropTiles(void) {
  struct Actor *actor;
  struct Actor *leader;
  s32 column;
  s32 row;
  u32 frame;
  s32 done;

  done = Event_GetFlag(FLAG_RIGHT_PILLAR_SUNK);
  if (!done && Event_GetFlag(FLAG_RIGHT_PILLAR_FULL)) {
    actor = ((struct Actor *)Actor_GetById(ACTOR_RIGHT_PILLAR));
    column = FIELD_CELL(actor->x.fixed);
    if (column == 35) {
      row = FIELD_CELL(actor->z.fixed);
      if (row == 9) {
        leader =
            ((struct Actor *)Actor_GetById(gMainRoutineWork.leaderActorId));
        Event_Start();
        Event_ResolvePendingActions(0);
        Event_Wait(20);
        Event_PlaySound(SE_STONE_RUMBLE);
        MapGrid_CopyCellsTo(35, 68, 35, 69, 1, 1);
        MapGrid_CopyCellAttributes(35, 0, 1, 1, column, row);
        actor->motionFlags = done;
        Actor_SetSpritePriority(ACTOR_RIGHT_PILLAR, 3);
        if (actor->z.fixed > leader->z.fixed)
          Actor_SetSpritePriority(gMainRoutineWork.leaderActorId, 3);
        for (frame = 0; frame < 128; frame++) {
          actor->y.fixed -= toFIXED(1) / 8;
          wait(1);
        }
        Actor_SetAnimation(ACTOR_RIGHT_PILLAR, 3);
        Actor_SetPosition(ACTOR_RIGHT_PILLAR_PUDDLE, 568, 152);
        Event_Wait(20);
        Event_SetFlag(FLAG_RIGHT_PILLAR_SUNK);
        if (actor->z.fixed > leader->z.fixed) {
          Actor_SetSpritePriority(gMainRoutineWork.leaderActorId, 2);
          ((struct Actor *)Actor_GetById(gMainRoutineWork.leaderActorId))
              ->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
        }
        Event_End();
      }
    }
  }
  done = Event_GetFlag(FLAG_LEFT_PILLAR_SUNK);
  if (!done && Event_GetFlag(FLAG_LEFT_PILLAR_FULL)) {
    actor = ((struct Actor *)Actor_GetById(ACTOR_LEFT_PILLAR));
    column = FIELD_CELL(actor->x.fixed);
    if (column == 29) {
      row = FIELD_CELL(actor->z.fixed);
      if (row == 9) {
        leader =
            ((struct Actor *)Actor_GetById(gMainRoutineWork.leaderActorId));
        Event_Start();
        Event_ResolvePendingActions(0);
        Event_Wait(20);
        Event_PlaySound(SE_STONE_RUMBLE);
        MapGrid_CopyCellsTo(29, 68, 29, 69, 1, 1);
        MapGrid_CopyCellAttributes(35, 0, 1, 1, column, row);
        actor->motionFlags = done;
        Actor_SetSpritePriority(ACTOR_LEFT_PILLAR, 3);
        if (actor->z.fixed > leader->z.fixed)
          Actor_SetSpritePriority(gMainRoutineWork.leaderActorId, 3);
        for (frame = 0; frame < 128; frame++) {
          actor->y.fixed -= toFIXED(1) / 8;
          wait(1);
        }
        Actor_SetAnimation(ACTOR_LEFT_PILLAR, 3);
        Actor_SetPosition(ACTOR_LEFT_PILLAR_PUDDLE, 472, 152);
        Event_Wait(20);
        Event_SetFlag(FLAG_LEFT_PILLAR_SUNK);
        if (actor->z.fixed > leader->z.fixed) {
          Actor_SetSpritePriority(gMainRoutineWork.leaderActorId, 2);
          ((struct Actor *)Actor_GetById(gMainRoutineWork.leaderActorId))
              ->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
        }
        Event_End();
      }
    }
  }
  if (Event_GetFlag(FLAG_RIGHT_PILLAR_SUNK) &&
      Event_GetFlag(FLAG_LEFT_PILLAR_SUNK)) {
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_PlaySound(SE_THUMP);
    MapGrid_CopyCellsTo(4, 124, 31, 64, 3, 2);
    Event_Wait(20);
    Event_SetFlag(FLAG_PILLAR_WAY_OPEN);
    Event_End();
  }
}

/// Nothing visible happens as Felix starts to Move or push one of the two
/// pillars; the game takes it up so that it can slide.
void LemurianShip_LiftPillar(void) {
  Actor_LiftTargetBlock(gLemurianShip_Pillars);
}

/// Felix slides one of the two pillars a cell with Move or a push. If it is
/// full of water and stops on its tile marked with a drop of water, it sinks;
/// once both have sunk, the arched doorway opens.
void LemurianShip_SetDownPillar(void) {
  Actor_PlaceMovedBlock();
  LemurianShip_CheckPillarsOnDropTiles();
}

#define DEFINE_DOUSE_PILLAR(name, pillar, fullFlag)                            \
  void name(void) {                                                            \
    Sound_Play(SE_DRIP);                                                       \
    Actor_SetAnimationById(pillar, 2);                                         \
    GameFlag_Set(fullFlag);                                                    \
    LemurianShip_CheckPillarsOnDropTiles();                                    \
  }

/// Felix casts Douse on the hollow white pillar on the right of the room of two
/// pillars: rain falls into it and it fills with water, ready to sink once it
/// stands on the tile marked with a drop of water.
DEFINE_DOUSE_PILLAR(LemurianShip_DouseRightPillar, ACTOR_RIGHT_PILLAR,
                    FLAG_RIGHT_PILLAR_FULL)

/// Felix casts Douse on the left pillar of the room of two pillars, once it has
/// risen again: it fills with water, ready to sink on its tile.
DEFINE_DOUSE_PILLAR(LemurianShip_DouseLeftPillar, ACTOR_LEFT_PILLAR,
                    FLAG_LEFT_PILLAR_FULL)

/// Felix casts Frost on the water in the right pillar after it has sunk: the
/// water, unseen until now, shows as it freezes into a pillar of ice.
void LemurianShip_ShowRightPillarPuddle(void) {
  FieldActor_SetSpriteColor(
      ((struct Actor *)Actor_GetById(ACTOR_RIGHT_PILLAR_PUDDLE)), 0);
}

/// Felix casts Frost on the water in the sunk left pillar, cell (29, 9): the
/// water, unseen until now, shows as it freezes into a pillar of ice above the
/// sunk pillar.
void LemurianShip_ShowLeftPillarPuddle(void) {
  FieldActor_SetSpriteColor(
      ((struct Actor *)Actor_GetById(ACTOR_LEFT_PILLAR_PUDDLE)), 0);
}

#define DEFINE_RAISE_PILLAR(name, pillar, loweredFlag, fullFlag, column)       \
  void name(void) {                                                            \
    struct FieldWork *work = Ram_HeapSlots->fieldWork;                         \
    struct Actor *actor = ((struct Actor *)Actor_GetById(pillar));             \
    u32 frame;                                                                 \
                                                                               \
    Event_Start();                                                             \
    Event_ResolvePendingActions(0);                                            \
    work->raisedTrigger = 0;                                                   \
    GameFlag_Clear(loweredFlag);                                               \
    GameFlag_Clear(fullFlag);                                                  \
    Event_Wait(20);                                                            \
    Sound_Play(SE_SOL_SANCTUM_LIGHT_UP);                                       \
    for (frame = 0; frame <= 63; frame++) {                                    \
      actor->y.fixed += toFIXED(1) / 4;                                        \
      wait(1);                                                                 \
    }                                                                          \
    Actor_SetSpritePriority(pillar, 2);                                        \
    MapGrid_CopyCellPictureIndicesRect(4, 123, column, 69, 1, 1);              \
    MapGrid_CopyCellAttributes(36, 0, 1, 1, column, 9);                        \
    Sound_Play(SE_THUMP);                                                      \
    MapGrid_CopyCellsTo(7, 124, 31, 64, 3, 2);                                 \
    GameFlag_Clear(FLAG_PILLAR_WAY_OPEN);                                      \
    Event_Wait(20);                                                            \
    Event_End();                                                               \
  }

/// Felix casts Parch on the water standing in the sunk right pillar, not
/// frozen: it dries up in a white flash, and the pillar rises out of the floor
/// with a chime and a thump, and the arched doorway closes again. Parch takes
/// any unfrozen puddle it finds off the map (FieldPsynergy_Parch), and the
/// room's watch (LemurianShip_WatchPillarPuddles) raises the pillar once its
/// puddle is gone.
DEFINE_RAISE_PILLAR(LemurianShip_RaiseRightPillar, ACTOR_RIGHT_PILLAR,
                    FLAG_RIGHT_PILLAR_SUNK, FLAG_RIGHT_PILLAR_FULL, 35)

/// Felix casts Parch on the water standing in the sunk left pillar, not
/// frozen: it dries up in a white flash, and the pillar rises out of the floor
/// with a chime and a thump, and the arched doorway closes again. Parch takes
/// any unfrozen puddle it finds off the map (FieldPsynergy_Parch), and the
/// room's watch (LemurianShip_WatchPillarPuddles) raises the pillar once its
/// puddle is gone.
DEFINE_RAISE_PILLAR(LemurianShip_RaiseLeftPillar, ACTOR_LEFT_PILLAR,
                    FLAG_LEFT_PILLAR_SUNK, FLAG_LEFT_PILLAR_FULL, 29)

/// Felix casts Parch in the flooded hold: the water dries up, and the chest the
/// flood had kept out of reach, with its Mist Potion, can be opened.
///
/// The flooded hold is a copy drawn 304 pixels further south; Parch moves the
/// party and the chest back to the dry hold, which looks the same without the
/// water, and sets the camera's bounds to it.
void LemurianShip_ParchHold(void) {
  struct Actor *leader =
      ((struct Actor *)Actor_GetById(gMainRoutineWork.leaderActorId));
  struct Actor *actor;

  Event_Start();
  Event_ResolvePendingActions(0);
  Field_SetCameraBounds(toFIXED(8), toFIXED(16), toFIXED(248), toFIXED(256));
  Actor_SetPositionFX(gMainRoutineWork.leaderActorId, leader->x.fixed,
                      leader->z.fixed - toFIXED(304));
  actor = ((struct Actor *)Actor_GetById(FIELD_TREASURE_ACTOR));
  Actor_SetPositionFX(FIELD_TREASURE_ACTOR, actor->x.fixed,
                      actor->z.fixed - toFIXED(304));
  FieldActor_SetSpriteColor(
      ((struct Actor *)Actor_GetById(FIELD_TREASURE_ACTOR)), 0);
  wait(10);
  FieldCamera_FollowActor(gMainRoutineWork.leaderActorId, 0);
  wait(1);
  MapGrid_CopyCellAttributes(6, 0, 1, 2, 6, 4);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  Event_SetFlag(FLAG_HOLD_PARCHED);
  PaletteFade_SetTarget(PALETTE_TINT_NONE, 0);
  Event_End();
}

/// Nothing visible happens as Felix begins to cast Parch in the flooded hold;
/// the game settles any actions still pending before LemurianShip_ParchHold
/// dries it.
void LemurianShip_StartParch(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_ResolvePendingActions(0);
  Event_End();
}

#define DEFINE_SET_UP_PUDDLE(name, priority)                                   \
  void name(s32 id) {                                                          \
    struct Actor *actor = ((struct Actor *)Actor_GetById(id));                 \
                                                                               \
    ((struct Actor *)Actor_GetById(id))->priorityFlags |=                      \
        ACTOR_PRIORITY_IGNORES_RISES;                                          \
    Actor_SetSpritePriority(id, priority);                                     \
    actor->priorityFlags &= ~ACTOR_PRIORITY_UNDERFOOT;                         \
    actor->riseCounter = 1;                                                    \
  }

/// Keeps a frozen puddle flat on the floor, drawn at sprite priority 1.
DEFINE_SET_UP_PUDDLE(LemurianShip_SetUpPuddle, 1)

// From patch 1; in the Japanese release the pillar room's puddles also take
// priority 1.
#if TLA_PATCH_1
/// Keeps a frozen puddle flat on the floor, drawn at sprite priority 2.
DEFINE_SET_UP_PUDDLE(LemurianShip_SetUpLowPuddle, 2)
#endif

/// Keeps the room of six jellies' puddles flat on the floor.
void LemurianShip_KeepSixJellyRoomPuddles(void) {
  ((struct Actor *)Actor_GetById(ACTOR_SIX_JELLY_ROOM_LIFT_PUDDLE))
      ->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
  Actor_SetSpritePriority(ACTOR_SIX_JELLY_ROOM_LIFT_PUDDLE, 3);
  ((struct Actor *)Actor_GetById(ACTOR_SIX_JELLY_ROOM_LIFT_PUDDLE))
      ->priorityFlags &= ~ACTOR_PRIORITY_UNDERFOOT;
  LemurianShip_SetUpPuddle(ACTOR_SIX_JELLY_ROOM_PUDDLE_A);
  LemurianShip_SetUpPuddle(ACTOR_SIX_JELLY_ROOM_PUDDLE_B);
  LemurianShip_SetUpPuddle(ACTOR_SIX_JELLY_ROOM_PUDDLE_C);
  LemurianShip_SetUpPuddle(ACTOR_SIX_JELLY_ROOM_PUDDLE_D);
  LemurianShip_SetUpPuddle(ACTOR_SIX_JELLY_ROOM_PUDDLE_E);
}

/// Keeps the room of the pipes' puddles flat on the floor.
void LemurianShip_KeepPipeRoomPuddles(void) {
  LemurianShip_SetUpPuddle(ACTOR_PIPE_ROOM_PUDDLE_A);
  LemurianShip_SetUpPuddle(ACTOR_PIPE_ROOM_PUDDLE_B);
  LemurianShip_SetUpPuddle(ACTOR_PIPE_ROOM_PUDDLE_C);
  LemurianShip_SetUpPuddle(ACTOR_PIPE_ROOM_PUDDLE_D);
  LemurianShip_SetUpPuddle(ACTOR_PIPE_ROOM_PUDDLE_E);
  LemurianShip_SetUpPuddle(ACTOR_PIPE_ROOM_PUDDLE_G);
}

/// Keeps the pillar room's two puddles flat on the floor.
void LemurianShip_KeepPillarRoomPuddles(void) {
#if !TLA_PATCH_1
  LemurianShip_SetUpPuddle(ACTOR_RIGHT_PILLAR_PUDDLE);
  LemurianShip_SetUpPuddle(ACTOR_LEFT_PILLAR_PUDDLE);
#else
  LemurianShip_SetUpLowPuddle(ACTOR_RIGHT_PILLAR_PUDDLE);
  LemurianShip_SetUpLowPuddle(ACTOR_LEFT_PILLAR_PUDDLE);
#endif
}

/// A task: in the flooded hold, draws the water rippling around Felix as he
/// wades.
void LemurianShip_RippleAroundLeader(void) {
  struct Actor *leader =
      ((struct Actor *)Actor_GetById(gMainRoutineWork.leaderActorId));
  s32 x = FIELD_CELL(leader->x.fixed);
  s32 z = FIELD_CELL(leader->z.fixed);

  MapGrid_CopyCellsTo(74, 105, 4, 85, 8, 6);
  if (z == 25) {
    if (x == 4)
      MapGrid_CopyCellsTo(18, 126, 4, 85, 1, 2);
  } else if (z == 26) {
    if (x == 7)
      MapGrid_CopyCellsTo(18, 126, 7, 86, 1, 1);
  } else if (z == 27) {
    if (x == 4)
      MapGrid_CopyCellsTo(18, 126, 4, 87, 1, 2);
    else if (x == 6)
      MapGrid_CopyCellsTo(18, 126, 6, 87, 1, 2);
    else if (x == 7)
      MapGrid_CopyCellsTo(18, 126, 7, 87, 1, 2);
    else if (x == 9)
      MapGrid_CopyCellsTo(18, 126, 9, 87, 1, 2);
  } else if (z == 29) {
    if (x == 6)
      MapGrid_CopyCellsTo(18, 126, 6, 89, 1, 2);
  }
}

/// A task: once the water on a sunk pillar is gone (its puddle is no longer
/// placed), the pillar rises again.
void LemurianShip_WatchPillarPuddles(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *actor;

  if (Event_GetFlag(FLAG_RIGHT_PILLAR_SUNK)) {
    actor = ((struct Actor *)Actor_GetById(ACTOR_RIGHT_PILLAR_PUDDLE));
    if ((actor->x.fixed | actor->z.fixed) == 0) {
      work->raisedTrigger = RAISED_RIGHT_PILLAR;
      Actor_SetAnimation(ACTOR_RIGHT_PILLAR, 1);
    }
  }
  if (Event_GetFlag(FLAG_LEFT_PILLAR_SUNK)) {
    actor = ((struct Actor *)Actor_GetById(ACTOR_LEFT_PILLAR_PUDDLE));
    if ((actor->x.fixed | actor->z.fixed) == 0) {
      work->raisedTrigger = RAISED_LEFT_PILLAR;
      Actor_SetAnimation(ACTOR_LEFT_PILLAR, 1);
    }
  }
}

/// A task: as Piers sets the Black Crystal, glints of light drift in from
/// around the pedestal toward it.
void LemurianShip_DrawLightToOrb(void) {
  s32 position[3];
  struct Actor *actor;

  SET_VECTOR(position, toFIXED(808), toFIXED(8), toFIXED(140));
  Vector_AddPolarOffset(toFIXED(32), Random16(), position);
  actor = ((struct Actor *)Import_ActorCreate(SPRITE_GLINT, position[0],
                                              position[1], position[2]));
  if (actor != NULL) {
    actor->motionFlags = 0;
    actor->speedLimit = toFIXED(2);
    actor->acceleration = toFIXED(1) / 2;
    Actor_SetSpriteFlags(actor, 0);
    Actor_SelectAnimation(actor, 0);
    FieldActor_SetMoveTarget(actor, 808, 8, 140);
    Actor_StartScript(actor, gLemurianShip_LightToOrb);
  }
}

/// Felix walks into the engine room for the first time: Piers steps up to the
/// pedestal, "How did all these monsters get on board?", and sets the Black
/// Crystal in it. Light gathers, the engine starts, and the ship sails out to
/// sea with every monster aboard gone.
void LemurianShip_SetBlackCrystal(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgLemurianShipHowDidAllTheseMonsters);
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.8);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 808, 208);
  FieldCamera_SetSpeed(0.8);
  FieldCamera_MoveTo(808, CAMERA_KEEP, 176, 1);
  Actor_PlaceOnActor(ACTOR_PIERS, gMainRoutineWork.leaderActorId);
  Actor_PlaceOnActor(ACTOR_SHEBA, gMainRoutineWork.leaderActorId);
  Actor_PlaceOnActor(ACTOR_JENNA, gMainRoutineWork.leaderActorId);
  Actor_PlaceOnActor(ACTOR_ENGINE_ROOM_KRADEN, gMainRoutineWork.leaderActorId);
  wait(1);
  Actor_SetSpeed(ACTOR_PIERS, 0.8);
  Actor_SetSpeed(ACTOR_SHEBA, 0.8);
  Actor_SetSpeed(ACTOR_JENNA, 0.8);
  Actor_SetSpeed(ACTOR_ENGINE_ROOM_KRADEN, 0.8);
  Actor_SetScript(ACTOR_SHEBA, gLemurianShip_ShebaStepsUp);
  Actor_SetScript(ACTOR_JENNA, gLemurianShip_JennaStepsUp);
  Actor_SetScript(ACTOR_ENGINE_ROOM_KRADEN, gLemurianShip_KradenStepsUp);
  Actor_RunScriptAndWait(ACTOR_PIERS, gLemurianShip_PiersToPedestal);
  Event_Wait(20);
  // "How did all these monsters get on board? I could swear I locked the..."
  Event_ShowMessage(ACTOR_PIERS, 0);
  Actor_FaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTH);
  // "It doesn't matter. Once we get this ship moving, I'm sure they'll..."
  Event_ShowMessage(ACTOR_PIERS, 0);
  Actor_SetAnimation(ACTOR_PIERS, ANIM_NOD);
  // "OK, now, I must set the black orb in this pedestal."
  Event_ShowMessage(ACTOR_PIERS, 0);
  Actor_SetAnimationAndWait(gMainRoutineWork.leaderActorId, ANIM_NOD);
  Actor_WalkToAndWait(ACTOR_PIERS, 808, 148);
  Actor_RunRepeatedMotion(ACTOR_PIERS, 2);
  Actor_SetSpritePriority(ACTOR_PIERS, 1);
  Event_PlaySound(SE_SHIMMER);
  Task_Add(LemurianShip_DrawLightToOrb, TASK_PASS_FRAME);
  Event_Wait(60);
  Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
  Event_Wait(20);
  Task_Remove(LemurianShip_DrawLightToOrb);
  Event_PlaySound(SE_BLACK_CRYSTAL_SET);
  Field_SetCameraBounds(toFIXED(688), 0, toFIXED(928), toFIXED(544));
  FieldCamera_MoveTo(808, CAMERA_KEEP, 448, 0);
  Actor_SetPosition(gMainRoutineWork.leaderActorId, 808, 480);
  Actor_SetPosition(ACTOR_PIERS, 808, 420);
  Actor_SetPosition(ACTOR_SHEBA, 784, 468);
  Actor_SetPosition(ACTOR_JENNA, 832, 468);
  Actor_SetPosition(ACTOR_ENGINE_ROOM_KRADEN, 792, 488);
  MapGrid_CenterOnFollowedPoint();
  Event_Wait(20);
  Event_PlaySound(SE_GLITTER);
  Event_Wait(80);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  PartyInventory_Remove(ITEM_BLACK_CRYSTAL);
  Event_SetFlag(FLAG_SHIP_ENGINE_RUNNING);
  Event_SetFlag(FLAG_JELLIES_BEATEN);
  Event_SetFlag((FLAG_JELLIES_BEATEN + JELLY_TWO_JELLY_ROOM_B));
  Event_SetFlag((FLAG_JELLIES_BEATEN + JELLY_SIX_JELLY_ROOM_B));
  Event_SetFlag((FLAG_JELLIES_BEATEN + JELLY_SIX_JELLY_ROOM_C));
  Event_SetFlag((FLAG_JELLIES_BEATEN + JELLY_SIX_JELLY_ROOM_D));
  Event_SetFlag((FLAG_JELLIES_BEATEN + JELLY_SIX_JELLY_ROOM_E));
  Event_SetFlag((FLAG_JELLIES_BEATEN + JELLY_SIX_JELLY_ROOM_F));
  Event_SetFlag((FLAG_JELLIES_BEATEN + JELLY_PIPE_ROOM_A));
  Event_SetFlag((FLAG_JELLIES_BEATEN + JELLY_PIPE_ROOM_B));
  Event_SetFlag((FLAG_JELLIES_BEATEN + JELLY_PIPE_ROOM_C));
  Event_SetFlag((FLAG_JELLIES_BEATEN + JELLY_PIPE_ROOM_D));
  Event_SetFlag((FLAG_JELLIES_BEATEN + JELLY_PIPE_ROOM_E));
  Event_SetFlag((FLAG_JELLIES_BEATEN + JELLY_PIPE_ROOM_G));
  Event_SetFlag((FLAG_JELLIES_BEATEN + JELLY_SIX_JELLY_ROOM_A));
  Event_SetFlag(FLAG_AQUA_HYDRA_BEATEN);
  gMainRoutineWork.ownerOverride = 1;
  Import_EventSetExitRequest(LEMURIAN_SHIP_AQUA_HYDRA_TO_WORLD_MAP);
}

/// Back from a battle with an Aqua Jelly, the jelly melts away where it stood
/// and a puddle spreads in its place.
void LemurianShip_MeltJellyIntoPuddle(s32 actorId, s32 id) {
  struct Actor *actor = ((struct Actor *)Actor_GetById(id));

  Event_Start();
  Event_ResolvePendingActions(0);
  actor->scaleX = FIXED_ONE / 16;
  actor->scaleY = FIXED_ONE / 16;
  wait(1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(20);
  Event_PlaySound(SE_CURSE);
  Actor_SetScript(actorId, gLemurianShip_JellyMelts);
  Actor_RunScriptAndWait(id, gLemurianShip_PuddleSpreads);
  Event_Wait(20);
  Event_End();
}

/// Felix comes into the room of six jellies while the jelly by the lift is
/// unbeaten: it takes fright, jumps and flees down the lift.
void LemurianShip_ScareJellyOnArrival(void) {
  if (Event_GetFlag((FLAG_JELLIES_BEATEN + JELLY_SIX_JELLY_ROOM_A)) == 0) {
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Event_Wait(20);
    LemurianShip_JellyFlees();
    Event_End();
  }
}

/// Felix steps near the jelly by the lift, cell (54, 5), while it is unbeaten
/// and back up: it takes fright and flees down the lift again.
void LemurianShip_ScareJelly(void) {
  if (Event_GetFlag((FLAG_JELLIES_BEATEN + JELLY_SIX_JELLY_ROOM_A)) == 0) {
    Event_Start();
    Event_ResolvePendingActions(0);
    LemurianShip_JellyFlees();
    Event_End();
  }
}

/// The jelly by the lift startles, jumps and flees down the lift.
void LemurianShip_JellyFlees(void) {
  struct Actor *actor =
      ((struct Actor *)Actor_GetById(ACTOR_SIX_JELLY_ROOM_JELLY_A));

  FieldActor_ShowEmote(ACTOR_SIX_JELLY_ROOM_JELLY_A,
                       EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_RunRepeatedMotion(ACTOR_SIX_JELLY_ROOM_JELLY_A, 2);
  actor->gravity = toFIXED(1) / 2;
  Actor_SetSpritePriority(ACTOR_SIX_JELLY_ROOM_JELLY_A, 1);
  Actor_SetSpeed(ACTOR_SIX_JELLY_ROOM_JELLY_A, 0.8);
  Actor_Jump(ACTOR_SIX_JELLY_ROOM_JELLY_A, 6, 0);
  Actor_MoveToAndWait(ACTOR_SIX_JELLY_ROOM_JELLY_A, 832, 88);
  Event_Wait(20);
  Actor_SetSpeed(ACTOR_SIX_JELLY_ROOM_JELLY_A, 1.6);
  Actor_SetSpritePriority(ACTOR_SIX_JELLY_ROOM_JELLY_A, 3);
  Actor_MoveToAndWait(ACTOR_SIX_JELLY_ROOM_JELLY_A, 824, 72);
  Actor_FaceDirection(ACTOR_SIX_JELLY_ROOM_JELLY_A, FACING_SOUTH_SOUTHEAST, 40);
  Event_SetFlag(FLAG_JELLY_FLED);
}

/// Back from beating the Aqua Hydra, the party stands in the flooded hold; a
/// sound plays and the game waits two seconds before Felix can move.
void LemurianShip_FloodHold(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_PlaySound(SE_SCATTER);
  Event_Wait(120);
  Event_End();
}

void LemurianShip_SetUpTwoJellyRoom(void);

s32 Scene_Initialize(void) {
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  if (Event_GetFlag(FLAG_SHIP_AT_ATTEKA_INLET))
    Event_SetFlag(FLAG_RETREAT_BLOCKED);
  if (gMainRoutineWork.next == SCENE_LEMURIAN_SHIP_CABINS) {
    switch (gMainRoutineWork.entrance) {
    case ENTRANCE_TWO_JELLY_ROOM_FROM_LOGBOOK_ROOM:
    case ENTRANCE_TWO_JELLY_ROOM_FROM_STAIR_ROOM:
    case ENTRANCE_TWO_JELLY_ROOM_FROM_TWO_JELLY_ROOM_HALL:
      LemurianShip_SetUpTwoJellyRoom();
      break;
    case ENTRANCE_STAIR_ROOM_FROM_TWO_JELLY_ROOM:
    case ENTRANCE_STAIR_ROOM_FROM_STAIR_ROOM_HALL:
    case LEMURIAN_SHIP_CABINS_FROM_LEMURIAN_SHIP_AQUA_HYDRA_NORTHWEST_A:
      LemurianShip_SetUpStairRoom();
      break;
    case ENTRANCE_SIX_JELLY_ROOM_FROM_WEST_HALL:
    case ENTRANCE_SIX_JELLY_ROOM_FROM_EAST_HALL:
    case ENTRANCE_SIX_JELLY_ROOM_FROM_LEDGE_ROOM_WEST:
    case ENTRANCE_SIX_JELLY_ROOM_FROM_LEDGE_ROOM_EAST:
      LemurianShip_SetUpSixJellyRoom();
      break;
    case LEMURIAN_SHIP_CABINS_FROM_LEMURIAN_SHIP_AQUA_HYDRA_NORTHEAST_A:
    case ENTRANCE_LEDGE_ROOM_FROM_SIX_JELLY_ROOM_WEST:
    case ENTRANCE_LEDGE_ROOM_FROM_SIX_JELLY_ROOM_EAST:
    case ENTRANCE_LEDGE_ROOM_FROM_PIPE_ROOM:
      LemurianShip_SetUpLedgeRoom();
      break;
    case ENTRANCE_PIPE_ROOM_FROM_SIX_JELLY_ROOM_EAST_HALL:
    case ENTRANCE_PIPE_ROOM_FROM_LEDGE_ROOM:
      LemurianShip_SetUpPipeRoom();
      break;
    }
  } else if (gMainRoutineWork.next == SCENE_LEMURIAN_SHIP_AQUA_HYDRA) {
    switch (gMainRoutineWork.entrance) {
    case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_NORTHWEST_A:
    case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_NORTH_A:
    case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_WEST_A:
    case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_NORTHEAST_A:
    case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_SOUTHWEST:
    case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_SOUTH_A:
    case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_SOUTH_B:
    case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_SOUTH_C:
    case ENTRANCE_HOLD_AFTER_AQUA_HYDRA_BATTLE:
    case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_NORTHWEST_B:
    case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_NORTH_B:
    case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_WEST_B:
    case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_NORTHEAST_B:
      LemurianShip_SetUpHold();
      break;
    case ENTRANCE_PILLAR_ROOM_FROM_CORRIDOR:
    case LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_EAST:
      LemurianShip_SetUpPillarRoom();
      break;
    case ENTRANCE_ENGINE_ROOM_FROM_CORRIDOR:
      LemurianShip_SetUpEngineRoom();
      break;
    }
  }
  return 0;
}

s32 Scene_PrepareMap(void) {
  u32 i;

  for (i = 0; i < JELLY_COUNT; i++) {
    if (Event_GetFlag(FLAG_FIGHTING_JELLY + i)) {
      struct MainRoutineWork *work = &gMainRoutineWork;

      if (work->sequenceResult == 1) {
        Event_SetFlag(FLAG_JELLIES_BEATEN + i);
        Event_SetFlag(FLAG_JELLY_MELTING + i);
      }
    }
    Event_ClearFlag(FLAG_FIGHTING_JELLY + i);
  }
  return 0;
}

/// The first room below deck: its two jellies, or the puddles they left, and a
/// jelly just beaten melting into its puddle.
void LemurianShip_SetUpTwoJellyRoom(void) {
  s32 flag;

  if (Event_GetFlag(FLAG_JELLIES_BEATEN)) {
    flag = Event_GetFlag(FLAG_JELLY_MELTING);
    if (flag != 0) {
      MapGrid_CopyCellAttributes(0, 25, 1, 3, 6, 25);
      Actor_SetPositionAndResetMotion(
          ((struct Actor *)Actor_GetById(ACTOR_TWO_JELLY_ROOM_PUDDLE_A)),
          toFIXED(104), 0, toFIXED(424));
      ((struct Actor *)Actor_GetById(ACTOR_TWO_JELLY_ROOM_PUDDLE_A))
          ->groundHeight = 0;
    } else {
      Actor_SetPosition(ACTOR_TWO_JELLY_ROOM_JELLY_A, 0, 0);
      MapGrid_CopyCellAttributes(0, 25, 1, 3, 6, 25);
      FieldActor_SetPositionAndResetMotion(
          ((struct Actor *)Actor_GetById(ACTOR_TWO_JELLY_ROOM_PUDDLE_A)),
          toFIXED(104), 0, toFIXED(424));
      ((struct Actor *)Actor_GetById(ACTOR_TWO_JELLY_ROOM_PUDDLE_A))
          ->groundHeight = 0;
    }
    wait(1);
  }
  if (Event_GetFlag((FLAG_JELLIES_BEATEN + JELLY_TWO_JELLY_ROOM_B))) {
    if (Event_GetFlag((FLAG_JELLY_MELTING + JELLY_TWO_JELLY_ROOM_B)))
      Actor_SetSpritePriority(ACTOR_TWO_JELLY_ROOM_JELLY_B, 1);
    else
      Actor_SetPosition(ACTOR_TWO_JELLY_ROOM_JELLY_B, 0, 0);
    FieldActor_SetPositionAndResetMotion(
        ((struct Actor *)Actor_GetById(ACTOR_TWO_JELLY_ROOM_PUDDLE_B)),
        toFIXED(104), -toFIXED(32), toFIXED(536));
    ((struct Actor *)Actor_GetById(ACTOR_TWO_JELLY_ROOM_PUDDLE_B))
        ->groundHeight = -toFIXED(32);
    Actor_SetSpritePriority(ACTOR_TWO_JELLY_ROOM_PUDDLE_B, 1);
    wait(1);
  }
  FlagBlock_PlaceAll(gLemurianShip_TwoJellyRoomPuddles);
  if (Event_GetFlag(FLAG_JELLY_MELTING)) {
    LemurianShip_MeltJellyIntoPuddle(ACTOR_TWO_JELLY_ROOM_JELLY_A,
                                     ACTOR_TWO_JELLY_ROOM_PUDDLE_A);
    Event_ClearFlag(FLAG_JELLY_MELTING);
  } else if (Event_GetFlag((FLAG_JELLY_MELTING + JELLY_TWO_JELLY_ROOM_B))) {
    LemurianShip_MeltJellyIntoPuddle(ACTOR_TWO_JELLY_ROOM_JELLY_B,
                                     ACTOR_TWO_JELLY_ROOM_PUDDLE_B);
    Event_ClearFlag((FLAG_JELLY_MELTING + JELLY_TWO_JELLY_ROOM_B));
  }
}

/// Sets a jelly up on the room of six jellies' lower floor, 32 pixels down.
void LemurianShip_SetUpLowerFloorJelly(s32 id) {
  struct Actor *actor = ((struct Actor *)Actor_GetById(id));

  ((struct Actor *)Actor_GetById(id))->priorityFlags |=
      ACTOR_PRIORITY_IGNORES_RISES;
  Actor_SetSpritePriority(id, 1);
  actor->motionFlags = 0;
  actor->y.fixed = -toFIXED(32);
  actor->groundHeight = -toFIXED(32);
}

/// Places a puddle on the lower floor, where its jelly sat.
#define PLACE_ON_LOWER_FLOOR(id, x, z)                                         \
  {                                                                            \
    FieldActor_SetPositionAndResetMotion(((struct Actor *)Actor_GetById(id)),  \
                                         x, -toFIXED(32), z);                  \
    ((struct Actor *)Actor_GetById(id))->groundHeight = -toFIXED(32);          \
  }

/// The room of six jellies: its crate, its jellies, and the puddles of those
/// beaten.
void LemurianShip_SetUpSixJellyRoom(void) {
  ((struct Actor *)Actor_GetById(gMainRoutineWork.leaderActorId))
      ->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
  Actor_PlaceBlocks(gLemurianShip_Crate);
  ((struct Actor *)Actor_GetById(ACTOR_SIX_JELLY_ROOM_CRATE))->priorityFlags |=
      ACTOR_PRIORITY_IGNORES_RISES;
  Actor_SetSpritePriority(ACTOR_SIX_JELLY_ROOM_CRATE, 1);
  ((struct Actor *)Actor_GetById(ACTOR_SIX_JELLY_ROOM_JELLY_A))
      ->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
  if (Event_GetFlag((FLAG_JELLIES_BEATEN + JELLY_SIX_JELLY_ROOM_A))) {
    struct Actor *actor =
        ((struct Actor *)Actor_GetById(ACTOR_SIX_JELLY_ROOM_LIFT_PUDDLE));

    actor->motionFlags = 0;
    FieldActor_SetPositionAndResetMotion(
        ((struct Actor *)Actor_GetById(ACTOR_SIX_JELLY_ROOM_LIFT_PUDDLE)),
        toFIXED(824), -toFIXED(32), toFIXED(72));
    actor->groundHeight = -toFIXED(32);
    if (Event_GetFlag((FLAG_PUDDLE_FROZEN + JELLY_SIX_JELLY_ROOM_A)))
      MapGrid_CopyCellAttributes(110, 1, 3, 1, 110, 4);
    else
      MapGrid_CopyCellAttributes(110, 0, 3, 1, 110, 4);
    if (!Event_GetFlag((FLAG_JELLY_MELTING + JELLY_SIX_JELLY_ROOM_A)))
      Actor_SetPosition(ACTOR_SIX_JELLY_ROOM_JELLY_A, 0, 0);
  }
  if (Event_GetFlag(FLAG_SHIP_ENGINE_RUNNING)) {
    if (Event_GetFlag(FLAG_LIFT_LOWERED)) {
      Actor_SetPosition(ACTOR_SIX_JELLY_ROOM_LIFT_PUDDLE, 0, 0);
      MapGrid_CopyCellsTo(68, 68, 50, 64, 3, 3);
      MapGrid_CopyCellAttributes(102, 16, 3, 2, 110, 4);
      MapGrid_CopyCellAttributes(50, 0, 3, 1, 50, 4);
    } else {
      PLACE_ON_LOWER_FLOOR(ACTOR_SIX_JELLY_ROOM_LIFT_PUDDLE, toFIXED(824),
                           toFIXED(72))
      MapGrid_CopyCellsTo(72, 68, 50, 64, 3, 3);
      MapGrid_CopyCellAttributes(106, 16, 3, 2, 110, 4);
      MapGrid_CopyCellAttributes(50, 1, 3, 1, 50, 4);
      if (Event_GetFlag((FLAG_PUDDLE_FROZEN + JELLY_SIX_JELLY_ROOM_A)))
        MapGrid_CopyCellAttributes(110, 1, 3, 1, 110, 4);
      else
        MapGrid_CopyCellAttributes(110, 0, 3, 1, 110, 4);
    }
  }
  if (Event_GetFlag((FLAG_JELLIES_BEATEN + JELLY_SIX_JELLY_ROOM_B))) {
    MapGrid_CopyCellAttributes(42, 0, 1, 1, 46, 5);
    MapGrid_CopyCellAttributes(43, 0, 1, 1, 45, 5);
    MapGrid_CopyCellAttributes(43, 0, 1, 1, 47, 5);
    PLACE_ON_LOWER_FLOOR(ACTOR_SIX_JELLY_ROOM_PUDDLE_A, toFIXED(744),
                         toFIXED(88))
    if (!Event_GetFlag((FLAG_JELLY_MELTING + JELLY_SIX_JELLY_ROOM_B)))
      Actor_SetPosition(ACTOR_SIX_JELLY_ROOM_JELLY_B, 0, 0);
  } else {
    LemurianShip_SetUpLowerFloorJelly(ACTOR_SIX_JELLY_ROOM_JELLY_B);
  }
  if (Event_GetFlag((FLAG_JELLIES_BEATEN + JELLY_SIX_JELLY_ROOM_C))) {
    struct Actor *block =
        ((struct Actor *)Actor_GetById(ACTOR_SIX_JELLY_ROOM_CRATE));

    MapGrid_CopyCellAttributes(42, 0, 1, 1, 47, 9);
    MapGrid_CopyCellAttributes(43, 0, 1, 1, 46, 9);
    MapGrid_CopyCellAttributes(43, 0, 1, 1, 48, 9);
    MapGrid_CopyCellAttributes(43, 0, 1, 1, 47, 10);
    MapGrid_CopyCellAttributes(43, 0, 1, 1, 47, 8);
    PLACE_ON_LOWER_FLOOR(ACTOR_SIX_JELLY_ROOM_PUDDLE_B, toFIXED(760),
                         toFIXED(152))
    if (FIELD_CELL(block->x.fixed) == 47 && FIELD_CELL(block->z.fixed) == 8)
      Actor_PlaceBlocks(gLemurianShip_Crate);
    if (!Event_GetFlag((FLAG_JELLY_MELTING + JELLY_SIX_JELLY_ROOM_C)))
      Actor_SetPosition(ACTOR_SIX_JELLY_ROOM_JELLY_C, 0, 0);
  } else {
    LemurianShip_SetUpLowerFloorJelly(ACTOR_SIX_JELLY_ROOM_JELLY_C);
  }
  if (Event_GetFlag((FLAG_JELLIES_BEATEN + JELLY_SIX_JELLY_ROOM_D))) {
    struct Actor *block =
        ((struct Actor *)Actor_GetById(ACTOR_SIX_JELLY_ROOM_CRATE));
    s32 x = FIELD_CELL(block->x.fixed);
    s32 z = FIELD_CELL(block->z.fixed);

    MapGrid_CopyCellAttributes(42, 0, 1, 1, 49, 7);
    MapGrid_CopyCellAttributes(43, 0, 1, 1, 49, 6);
    MapGrid_CopyCellAttributes(43, 0, 1, 1, 50, 7);
    MapGrid_CopyCellAttributes(43, 0, 1, 1, 48, 7);
    MapGrid_CopyCellAttributes(43, 0, 1, 1, 49, 8);
    PLACE_ON_LOWER_FLOOR(ACTOR_SIX_JELLY_ROOM_PUDDLE_C, toFIXED(792),
                         toFIXED(120))
    if ((x == 48 && z == 7) || (x == 49 && z == 8))
      Actor_PlaceBlocks(gLemurianShip_Crate);
    if (!Event_GetFlag((FLAG_JELLY_MELTING + JELLY_SIX_JELLY_ROOM_D)))
      Actor_SetPosition(ACTOR_SIX_JELLY_ROOM_JELLY_D, 0, 0);
  } else {
    LemurianShip_SetUpLowerFloorJelly(ACTOR_SIX_JELLY_ROOM_JELLY_D);
  }
  if (Event_GetFlag((FLAG_JELLIES_BEATEN + JELLY_SIX_JELLY_ROOM_E))) {
    MapGrid_CopyCellAttributes(42, 0, 1, 1, 51, 7);
    PLACE_ON_LOWER_FLOOR(ACTOR_SIX_JELLY_ROOM_PUDDLE_D, toFIXED(824),
                         toFIXED(120))
    if (!Event_GetFlag((FLAG_JELLY_MELTING + JELLY_SIX_JELLY_ROOM_E)))
      Actor_SetPosition(ACTOR_SIX_JELLY_ROOM_JELLY_E, 0, 0);
  } else {
    LemurianShip_SetUpLowerFloorJelly(ACTOR_SIX_JELLY_ROOM_JELLY_E);
  }
  if (Event_GetFlag((FLAG_JELLIES_BEATEN + JELLY_SIX_JELLY_ROOM_F))) {
    MapGrid_CopyCellAttributes(42, 0, 1, 1, 51, 9);
    MapGrid_CopyCellAttributes(43, 0, 1, 1, 51, 8);
    MapGrid_CopyCellAttributes(43, 0, 1, 1, 50, 9);
    MapGrid_CopyCellAttributes(43, 0, 1, 1, 52, 9);
    MapGrid_CopyCellAttributes(43, 0, 1, 1, 51, 10);
    PLACE_ON_LOWER_FLOOR(ACTOR_SIX_JELLY_ROOM_PUDDLE_E, toFIXED(824),
                         toFIXED(152))
    if (!Event_GetFlag((FLAG_JELLY_MELTING + JELLY_SIX_JELLY_ROOM_F)))
      Actor_SetPosition(ACTOR_SIX_JELLY_ROOM_JELLY_F, 0, 0);
  } else {
    LemurianShip_SetUpLowerFloorJelly(ACTOR_SIX_JELLY_ROOM_JELLY_F);
  }
  FlagBlock_PlaceAll(gLemurianShip_SixJellyRoomPuddles);
  Task_Add(LemurianShip_KeepSixJellyRoomPuddles, TASK_PASS_FRAME);
  if (Event_GetFlag((FLAG_JELLY_MELTING + JELLY_SIX_JELLY_ROOM_A))) {
    LemurianShip_MeltJellyIntoPuddle(ACTOR_SIX_JELLY_ROOM_JELLY_A,
                                     ACTOR_SIX_JELLY_ROOM_LIFT_PUDDLE);
    Event_ClearFlag((FLAG_JELLY_MELTING + JELLY_SIX_JELLY_ROOM_A));
  } else if (Event_GetFlag((FLAG_JELLY_MELTING + JELLY_SIX_JELLY_ROOM_B))) {
    LemurianShip_MeltJellyIntoPuddle(ACTOR_SIX_JELLY_ROOM_JELLY_B,
                                     ACTOR_SIX_JELLY_ROOM_PUDDLE_A);
    Event_ClearFlag((FLAG_JELLY_MELTING + JELLY_SIX_JELLY_ROOM_B));
  } else if (Event_GetFlag((FLAG_JELLY_MELTING + JELLY_SIX_JELLY_ROOM_C))) {
    LemurianShip_MeltJellyIntoPuddle(ACTOR_SIX_JELLY_ROOM_JELLY_C,
                                     ACTOR_SIX_JELLY_ROOM_PUDDLE_B);
    Event_ClearFlag((FLAG_JELLY_MELTING + JELLY_SIX_JELLY_ROOM_C));
  } else if (Event_GetFlag((FLAG_JELLY_MELTING + JELLY_SIX_JELLY_ROOM_D))) {
    LemurianShip_MeltJellyIntoPuddle(ACTOR_SIX_JELLY_ROOM_JELLY_D,
                                     ACTOR_SIX_JELLY_ROOM_PUDDLE_C);
    Event_ClearFlag((FLAG_JELLY_MELTING + JELLY_SIX_JELLY_ROOM_D));
  } else if (Event_GetFlag((FLAG_JELLY_MELTING + JELLY_SIX_JELLY_ROOM_E))) {
    LemurianShip_MeltJellyIntoPuddle(ACTOR_SIX_JELLY_ROOM_JELLY_E,
                                     ACTOR_SIX_JELLY_ROOM_PUDDLE_D);
    Event_ClearFlag((FLAG_JELLY_MELTING + JELLY_SIX_JELLY_ROOM_E));
  } else if (Event_GetFlag((FLAG_JELLY_MELTING + JELLY_SIX_JELLY_ROOM_F))) {
    LemurianShip_MeltJellyIntoPuddle(ACTOR_SIX_JELLY_ROOM_JELLY_F,
                                     ACTOR_SIX_JELLY_ROOM_PUDDLE_E);
    Event_ClearFlag((FLAG_JELLY_MELTING + JELLY_SIX_JELLY_ROOM_F));
  }
  if (gMainRoutineWork.entrance == ENTRANCE_SIX_JELLY_ROOM_FROM_EAST_HALL &&
      !Event_GetFlag(FLAG_KEEP_PARTY_POSITION))
    LemurianShip_ScareJellyOnArrival();
}

/// A task: draws the ledge crate at the leader's sprite priority.
void LemurianShip_MatchCratePriority(void) {
  struct Actor *leader =
      ((struct Actor *)Actor_GetById(gMainRoutineWork.leaderActorId));

  Actor_SetSpritePriority(ACTOR_LEDGE_CRATE,
                          ((struct FieldSprite *)leader->sprite)->priority);
}

/// The room of the crate on the ledge.
void LemurianShip_SetUpLedgeRoom(void) {
  // XXX (no evidence): whether flag 0x8 is a slip for FLAG_CRATE_DROPPED, which
  // the drop sets; no code of the game sets flag 0x8, and nothing in reach
  // shows which flag was meant.
  if (Event_GetFlag(0x8))
    MapGrid_CopyCellAttributes(14, 46, 1, 1, 16, 46);
  Actor_PlaceBlocks(gLemurianShip_Crate);
  Task_Add(LemurianShip_MatchCratePriority, TASK_PASS_FRAME);
}

/// Sets a jelly up on the floor.
void LemurianShip_SetUpJelly(s32 id) {
  struct Actor *actor = ((struct Actor *)Actor_GetById(id));

  ((struct Actor *)Actor_GetById(id))->priorityFlags |=
      ACTOR_PRIORITY_IGNORES_RISES;
  Actor_SetSpritePriority(id, 1);
  actor->motionFlags = 0;
  actor->y.fixed = 0;
  actor->groundHeight = 0;
}

/// The room of the pipes: each beaten one leaves a puddle and opens its
/// cell.
void LemurianShip_SetUpPipeRoom(void) {
  ((struct Actor *)Actor_GetById(gMainRoutineWork.leaderActorId))
      ->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
  Actor_PlaceBlocks(gLemurianShip_Crate);
  ((struct Actor *)Actor_GetById(ACTOR_PIPE_ROOM_CRATE))->priorityFlags |=
      ACTOR_PRIORITY_IGNORES_RISES;
  Actor_SetSpritePriority(ACTOR_PIPE_ROOM_CRATE, 1);
  if (Event_GetFlag((FLAG_JELLIES_BEATEN + JELLY_PIPE_ROOM_A))) {
    MapGrid_CopyCellAttributes(44, 0, 1, 1, 48, 32);
    Actor_SetPositionAndResetMotion(
        ((struct Actor *)Actor_GetById(ACTOR_PIPE_ROOM_PUDDLE_A)), toFIXED(776),
        0, toFIXED(520));
    ((struct Actor *)Actor_GetById(ACTOR_PIPE_ROOM_PUDDLE_A))->groundHeight = 0;
    if (!Event_GetFlag((FLAG_JELLY_MELTING + JELLY_PIPE_ROOM_A)))
      Actor_SetPosition(ACTOR_PIPE_ROOM_JELLY_A, 0, 0);
  } else {
    LemurianShip_SetUpJelly(ACTOR_PIPE_ROOM_JELLY_A);
  }
  if (Event_GetFlag((FLAG_JELLIES_BEATEN + JELLY_PIPE_ROOM_B))) {
    MapGrid_CopyCellAttributes(44, 0, 1, 1, 53, 32);
    Actor_SetPositionAndResetMotion(
        ((struct Actor *)Actor_GetById(ACTOR_PIPE_ROOM_PUDDLE_B)), toFIXED(856),
        0, toFIXED(520));
    ((struct Actor *)Actor_GetById(ACTOR_PIPE_ROOM_PUDDLE_B))->groundHeight = 0;
    if (!Event_GetFlag((FLAG_JELLY_MELTING + JELLY_PIPE_ROOM_B)))
      Actor_SetPosition(ACTOR_PIPE_ROOM_JELLY_B, 0, 0);
  } else {
    LemurianShip_SetUpJelly(ACTOR_PIPE_ROOM_JELLY_B);
  }
  if (Event_GetFlag((FLAG_JELLIES_BEATEN + JELLY_PIPE_ROOM_C))) {
    MapGrid_CopyCellAttributes(44, 0, 1, 1, 43, 37);
    Actor_SetPositionAndResetMotion(
        ((struct Actor *)Actor_GetById(ACTOR_PIPE_ROOM_PUDDLE_C)), toFIXED(696),
        0, toFIXED(600));
    ((struct Actor *)Actor_GetById(ACTOR_PIPE_ROOM_PUDDLE_C))->groundHeight = 0;
    if (!Event_GetFlag((FLAG_JELLY_MELTING + JELLY_PIPE_ROOM_C)))
      Actor_SetPosition(ACTOR_PIPE_ROOM_JELLY_C, 0, 0);
  } else {
    LemurianShip_SetUpJelly(ACTOR_PIPE_ROOM_JELLY_C);
  }
  if (Event_GetFlag((FLAG_JELLIES_BEATEN + JELLY_PIPE_ROOM_D))) {
    MapGrid_CopyCellAttributes(44, 0, 1, 1, 48, 37);
    Actor_SetPositionAndResetMotion(
        ((struct Actor *)Actor_GetById(ACTOR_PIPE_ROOM_PUDDLE_D)), toFIXED(776),
        0, toFIXED(600));
    ((struct Actor *)Actor_GetById(ACTOR_PIPE_ROOM_PUDDLE_D))->groundHeight = 0;
    if (!Event_GetFlag((FLAG_JELLY_MELTING + JELLY_PIPE_ROOM_D)))
      Actor_SetPosition(ACTOR_PIPE_ROOM_JELLY_D, 0, 0);
  } else {
    LemurianShip_SetUpJelly(ACTOR_PIPE_ROOM_JELLY_D);
  }
  if (Event_GetFlag((FLAG_JELLIES_BEATEN + JELLY_PIPE_ROOM_E))) {
    MapGrid_CopyCellAttributes(44, 0, 1, 1, 50, 37);
    Actor_SetPositionAndResetMotion(
        ((struct Actor *)Actor_GetById(ACTOR_PIPE_ROOM_PUDDLE_E)), toFIXED(808),
        0, toFIXED(600));
    ((struct Actor *)Actor_GetById(ACTOR_PIPE_ROOM_PUDDLE_E))->groundHeight = 0;
    if (!Event_GetFlag((FLAG_JELLY_MELTING + JELLY_PIPE_ROOM_E)))
      Actor_SetPosition(ACTOR_PIPE_ROOM_JELLY_E, 0, 0);
  } else {
    LemurianShip_SetUpJelly(ACTOR_PIPE_ROOM_JELLY_E);
  }
  if (Event_GetFlag((FLAG_JELLIES_BEATEN + JELLY_PIPE_ROOM_G))) {
    MapGrid_CopyCellAttributes(44, 0, 1, 1, 48, 43);
    Actor_SetPositionAndResetMotion(
        ((struct Actor *)Actor_GetById(ACTOR_PIPE_ROOM_PUDDLE_G)), toFIXED(776),
        0, toFIXED(696));
    ((struct Actor *)Actor_GetById(ACTOR_PIPE_ROOM_PUDDLE_G))->groundHeight = 0;
    if (!Event_GetFlag((FLAG_JELLY_MELTING + JELLY_PIPE_ROOM_G)))
      Actor_SetPosition(ACTOR_PIPE_ROOM_JELLY_G, 0, 0);
  } else {
    LemurianShip_SetUpJelly(ACTOR_PIPE_ROOM_JELLY_G);
  }
  FlagBlock_PlaceAll(gLemurianShip_PipeRoomPuddles);
  FieldTask_AddFrame(LemurianShip_KeepPipeRoomPuddles, TASK_PASS_FRAME);
  if (Event_GetFlag((FLAG_JELLY_MELTING + JELLY_PIPE_ROOM_A))) {
    LemurianShip_MeltJellyIntoPuddle(ACTOR_PIPE_ROOM_JELLY_A,
                                     ACTOR_PIPE_ROOM_PUDDLE_A);
    Event_ClearFlag((FLAG_JELLY_MELTING + JELLY_PIPE_ROOM_A));
  } else if (Event_GetFlag((FLAG_JELLY_MELTING + JELLY_PIPE_ROOM_B))) {
    LemurianShip_MeltJellyIntoPuddle(ACTOR_PIPE_ROOM_JELLY_B,
                                     ACTOR_PIPE_ROOM_PUDDLE_B);
    Event_ClearFlag((FLAG_JELLY_MELTING + JELLY_PIPE_ROOM_B));
  } else if (Event_GetFlag((FLAG_JELLY_MELTING + JELLY_PIPE_ROOM_C))) {
    LemurianShip_MeltJellyIntoPuddle(ACTOR_PIPE_ROOM_JELLY_C,
                                     ACTOR_PIPE_ROOM_PUDDLE_C);
    Event_ClearFlag((FLAG_JELLY_MELTING + JELLY_PIPE_ROOM_C));
  } else if (Event_GetFlag((FLAG_JELLY_MELTING + JELLY_PIPE_ROOM_D))) {
    LemurianShip_MeltJellyIntoPuddle(ACTOR_PIPE_ROOM_JELLY_D,
                                     ACTOR_PIPE_ROOM_PUDDLE_D);
    Event_ClearFlag((FLAG_JELLY_MELTING + JELLY_PIPE_ROOM_D));
  } else if (Event_GetFlag((FLAG_JELLY_MELTING + JELLY_PIPE_ROOM_E))) {
    LemurianShip_MeltJellyIntoPuddle(ACTOR_PIPE_ROOM_JELLY_E,
                                     ACTOR_PIPE_ROOM_PUDDLE_E);
    Event_ClearFlag((FLAG_JELLY_MELTING + JELLY_PIPE_ROOM_E));
  } else if (Event_GetFlag((FLAG_JELLY_MELTING + JELLY_PIPE_ROOM_G))) {
    LemurianShip_MeltJellyIntoPuddle(ACTOR_PIPE_ROOM_JELLY_G,
                                     ACTOR_PIPE_ROOM_PUDDLE_G);
    Event_ClearFlag((FLAG_JELLY_MELTING + JELLY_PIPE_ROOM_G));
  }
}

/// The stair room: once the engine runs, its stairs are open.
void LemurianShip_SetUpStairRoom(void) {
  if (Event_GetFlag(FLAG_SHIP_ENGINE_RUNNING)) {
    MapGrid_CopyCellsTo(17, 125, 27, 63, 3, 3);
    MapGrid_CopyCellAttributes(25, 0, 5, 3, 25, 4);
  }
}

/// The hold of the Aqua Hydra: once it is beaten the hold is flooded, and once
/// Parch has dried it the chest lies on dry floor.
void LemurianShip_SetUpHold(void) {
  struct Actor *actor;

  ((struct Actor *)Actor_GetById(gMainRoutineWork.leaderActorId))
      ->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
  Task_Add(LemurianShip_RippleAroundLeader, TASK_PASS_FRAME);
  if (Event_GetFlag(FLAG_AQUA_HYDRA_BEATEN)) {
    actor = ((struct Actor *)Actor_GetById(FIELD_TREASURE_ACTOR));
    actor->motionFlags = 0;
    Actor_SetPositionAndResetMotion(
        ((struct Actor *)Actor_GetById(FIELD_TREASURE_ACTOR)), toFIXED(136),
        -toFIXED(32), toFIXED(392));
    actor->groundHeight = -toFIXED(16);
    Actor_SetSpritePriority(FIELD_TREASURE_ACTOR, 3);
    FieldActor_SetSpriteColor(
        ((struct Actor *)Actor_GetById(FIELD_TREASURE_ACTOR)), 1);
    Actor_SetPosition(ACTOR_HOLD_JELLY_A, 0, 0);
    Actor_SetPosition(ACTOR_HOLD_JELLY_B, 0, 0);
    Actor_SetPosition(ACTOR_HOLD_JELLY_C, 0, 0);
    Actor_SetPosition(ACTOR_HOLD_JELLY_D, 0, 0);
    Actor_SetPosition(ACTOR_HOLD_JELLY_E, 0, 0);
    Actor_SetPosition(ACTOR_HOLD_JELLY_F, 0, 0);
    wait(1);
  }
  if (Event_GetFlag(FLAG_HOLD_PARCHED)) {
    struct Actor *marker =
        ((struct Actor *)Actor_GetById(FIELD_TREASURE_ACTOR));

    Actor_SetPositionFX(FIELD_TREASURE_ACTOR, marker->x.fixed,
                        marker->z.fixed - toFIXED(304));
    FieldActor_SetSpriteColor(
        ((struct Actor *)Actor_GetById(FIELD_TREASURE_ACTOR)), 0);
    MapGrid_CopyCellAttributes(6, 0, 1, 2, 6, 4);
    MapGrid_SetCameraBounds(toFIXED(8), toFIXED(16), toFIXED(248),
                            toFIXED(256));
    FieldCamera_FollowActor(gMainRoutineWork.leaderActorId, 0);
    MapGrid_CenterOnFollowedPoint();
    wait(1);
  }
  if (Event_GetFlag((FLAG_JELLY_MELTING + JELLY_AQUA_HYDRA))) {
    LemurianShip_FloodHold();
    Event_ClearFlag((FLAG_JELLY_MELTING + JELLY_AQUA_HYDRA));
  }
}

/// The room of the two pillars Douse fills, as the player left them.
void LemurianShip_SetUpPillarRoom(void) {
  struct Actor *actor;

  ((struct Actor *)Actor_GetById(gMainRoutineWork.leaderActorId))
      ->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
  Actor_PlaceBlocks(gLemurianShip_Pillars);
  if (Event_GetFlag(FLAG_PILLAR_WAY_OPEN)) {
    MapGrid_CopyCellsTo(4, 124, 31, 64, 3, 2);
    Event_SetFlag(FLAG_RIGHT_PILLAR_SUNK);
    Event_SetFlag(FLAG_LEFT_PILLAR_SUNK);
  }
  Actor_SetSpriteFlags(((struct Actor *)Actor_GetById(ACTOR_RIGHT_PILLAR)), 0);
  if (Event_GetFlag(FLAG_RIGHT_PILLAR_SUNK)) {
    actor = ((struct Actor *)Actor_GetById(ACTOR_RIGHT_PILLAR));
    MapGrid_CopyCellsTo(35, 68, 35, 69, 1, 1);
    MapGrid_CopyCellAttributes(35, 0, 1, 1, 35, 9);
    actor->motionFlags = 0;
    FieldActor_SetPositionAndResetMotion(actor, toFIXED(568), -toFIXED(16),
                                         toFIXED(152));
    Actor_SetSpritePriority(ACTOR_RIGHT_PILLAR, 3);
    Event_SetFlag(FLAG_RIGHT_PILLAR_FULL);
    Actor_SetPosition(ACTOR_RIGHT_PILLAR_PUDDLE, 568, 152);
  }
  if (Event_GetFlag(FLAG_RIGHT_PILLAR_FULL)) {
    if (Event_GetFlag(FLAG_RIGHT_PILLAR_SUNK))
      Actor_SetAnimation(ACTOR_RIGHT_PILLAR, 3);
    else
      Actor_SetAnimation(ACTOR_RIGHT_PILLAR, 2);
  }
  Actor_SetSpriteFlags(((struct Actor *)Actor_GetById(ACTOR_LEFT_PILLAR)), 0);
  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION))
    Event_SetFlag(FLAG_LEFT_PILLAR_SUNK);
  if (Event_GetFlag(FLAG_LEFT_PILLAR_SUNK)) {
    actor = ((struct Actor *)Actor_GetById(ACTOR_LEFT_PILLAR));
    MapGrid_CopyCellsTo(29, 68, 29, 69, 1, 1);
    MapGrid_CopyCellAttributes(35, 0, 1, 1, 29, 9);
    actor->motionFlags = 0;
    actor->y.fixed = -toFIXED(16);
    Actor_SetSpritePriority(ACTOR_LEFT_PILLAR, 3);
    Event_SetFlag(FLAG_LEFT_PILLAR_FULL);
    Actor_SetPosition(ACTOR_LEFT_PILLAR_PUDDLE, 472, 152);
  }
  if (Event_GetFlag(FLAG_LEFT_PILLAR_FULL)) {
    if (Event_GetFlag(FLAG_LEFT_PILLAR_SUNK))
      Actor_SetAnimation(ACTOR_LEFT_PILLAR, 3);
    else
      Actor_SetAnimation(ACTOR_LEFT_PILLAR, 2);
  }
  wait(1);
  FlagBlock_PlaceAll(gLemurianShip_TwoJellyRoomPuddles);
  FieldTask_AddFrame(LemurianShip_KeepPillarRoomPuddles, TASK_PASS_FRAME);
  if (!Event_GetFlag(FLAG_RIGHT_PILLAR_PUDDLE_FROZEN))
    FieldActor_SetSpriteColor(
        ((struct Actor *)Actor_GetById(ACTOR_RIGHT_PILLAR_PUDDLE)),
        SPRITE_COLOR_HIDDEN);
  if (!Event_GetFlag(FLAG_LEFT_PILLAR_PUDDLE_FROZEN))
    FieldActor_SetSpriteColor(
        ((struct Actor *)Actor_GetById(ACTOR_LEFT_PILLAR_PUDDLE)),
        SPRITE_COLOR_HIDDEN);
  Task_Add(LemurianShip_WatchPillarPuddles, TASK_PASS_FRAME);
}

/// The engine room: until the engine runs, Piers sets the Black Crystal as the
/// party arrives.
void LemurianShip_SetUpEngineRoom(void) {
  if (Event_GetFlag(FLAG_SHIP_ENGINE_RUNNING) == 0)
    LemurianShip_SetBlackCrystal();
}

// clang-format off
#include "shared/leader_walk_through_door_script.c"

/// A beaten jelly melting away.
s32 gLemurianShip_JellyMelts[] = {
    23, 9, -512, 23, 10, -1024,
    0, 1, 13, 60, 0, 3,
    0, 1, 17,
};

/// The puddle spreading where the jelly melted.
s32 gLemurianShip_PuddleSpreads[] = {
    SC_ADD(SW_SCALE_X, FIXED_ONE / 64),
    SC_ADD(SW_SCALE_Y, FIXED_ONE / 64),
    SC_WAIT(1),
    SC_LOOP(60, SC_TOP),
    SC_END,
};

/// The Aqua Hydra forming out of the gathered jellies, growing in pulses.
s32 gLemurianShip_HydraForms[] = {
    SC_ADD(SW_SCALE_X, FIXED_ONE * 3 / 16),
    SC_ADD(SW_SCALE_Y, FIXED_ONE * 3 / 16),
    SC_WAIT(4),
    SC_ADD(SW_SCALE_X, -FIXED_ONE * 3 / 32),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE * 3 / 32),
    SC_WAIT(4),
    SC_LOOP(8, SC_TOP),
    SC_END,
    SC_ADD(SW_SCALE_X, -FIXED_ONE / 128),
    SC_ADD(SW_SCALE_Y, -FIXED_ONE / 128),
    SC_WAIT(1),
    SC_LOOP(160, SC_TOP),
    SC_END,
};

/// Piers walking up to the Black Crystal's pedestal.
u32 gLemurianShip_PiersToPedestal[] =                                                                 
      {                                                                        
          SC_ANIMATION(2), SC_MOVE(808, 0, 168), SC_SYNC, SC_ANIMATION(1),   
          SC_END,                                                              
  };

/// Sheba stepping up beside Piers.
u32 gLemurianShip_ShebaStepsUp[] =                                                                 
      {                                                                        
          SC_ANIMATION(2),   SC_MOVE(784, 0, 196),                     
          SC_SYNC,     SC_ANIMATION(1),                                              
          SC_WAIT(10), SC_SET(SW_FACING, FACING_NORTHEAST),                    
          SC_WAIT(10), SC_END,                                                 
  };

/// Jenna stepping up beside Piers.
u32 gLemurianShip_JennaStepsUp[] =                                                                 
      {                                                                        
          SC_ANIMATION(2),   SC_MOVE(832, 0, 196),                     
          SC_SYNC,     SC_ANIMATION(1),                                              
          SC_WAIT(10), SC_SET(SW_FACING, FACING_NORTHWEST),                    
          SC_WAIT(10), SC_END,                                                 
  };

/// Kraden stepping up beside Piers.
u32 gLemurianShip_KradenStepsUp[] = {                                                               
      SC_ANIMATION(2),   SC_MOVE(792, 0, 216),                         
      SC_SYNC,     SC_ANIMATION(1),                                                  
      SC_WAIT(10), SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),              
      SC_WAIT(10), SC_END,                                                     
  };

/// The crate Move and a push take up, by actor id.
s16 gLemurianShip_Crate[] = {
    ACTOR_FIRST_PLACED, -1,
};

/// The room of two jellies' puddles, by actor id and flag.
s16 gLemurianShip_TwoJellyRoomPuddles[] = {
    ACTOR_TWO_JELLY_ROOM_PUDDLE_A, FLAG_PUDDLE_FROZEN + JELLY_TWO_JELLY_ROOM_A,
    ACTOR_TWO_JELLY_ROOM_PUDDLE_B, FLAG_PUDDLE_FROZEN + JELLY_TWO_JELLY_ROOM_B,
    -1,
};

/// The room of six jellies' puddles, by actor id and flag.
s16 gLemurianShip_SixJellyRoomPuddles[] = {
    ACTOR_SIX_JELLY_ROOM_PUDDLE_A, FLAG_PUDDLE_FROZEN + JELLY_SIX_JELLY_ROOM_B,
    ACTOR_SIX_JELLY_ROOM_PUDDLE_B, FLAG_PUDDLE_FROZEN + JELLY_SIX_JELLY_ROOM_C,
    ACTOR_SIX_JELLY_ROOM_PUDDLE_C, FLAG_PUDDLE_FROZEN + JELLY_SIX_JELLY_ROOM_D,
    ACTOR_SIX_JELLY_ROOM_PUDDLE_D, FLAG_PUDDLE_FROZEN + JELLY_SIX_JELLY_ROOM_E,
    ACTOR_SIX_JELLY_ROOM_PUDDLE_E, FLAG_PUDDLE_FROZEN + JELLY_SIX_JELLY_ROOM_F,
    -1,
};

/// The room of the pipes' puddles, by actor id and flag.
s16 gLemurianShip_PipeRoomPuddles[] = {
    ACTOR_PIPE_ROOM_PUDDLE_A, FLAG_PUDDLE_FROZEN + JELLY_PIPE_ROOM_A,
    ACTOR_PIPE_ROOM_PUDDLE_B, FLAG_PUDDLE_FROZEN + JELLY_PIPE_ROOM_B,
    ACTOR_PIPE_ROOM_PUDDLE_C, FLAG_PUDDLE_FROZEN + JELLY_PIPE_ROOM_C,
    ACTOR_PIPE_ROOM_PUDDLE_D, FLAG_PUDDLE_FROZEN + JELLY_PIPE_ROOM_D,
    ACTOR_PIPE_ROOM_PUDDLE_E, FLAG_PUDDLE_FROZEN + JELLY_PIPE_ROOM_E,
    ACTOR_PIPE_ROOM_PUDDLE_G, FLAG_PUDDLE_FROZEN + JELLY_PIPE_ROOM_G,
    -1,
};

/// The two pillars Move and a push take up, by actor id.
s16 gLemurianShip_Pillars[] = {
    ACTOR_RIGHT_PILLAR, ACTOR_LEFT_PILLAR, -1,
};

/// Two halfwords of a scene table row the header declares as words, the first
/// in the low half.
#define HALFWORDS(low, high) ((s32)(((u32)(high) << 16) | (u16)(low)))
/// A struct SceneEntrance row as six words: the entrance, at (x, y, z) facing
/// facing, its camera box from (left, top) to (right, bottom).
#define ENTRANCE_WORDS(entrance, x, y, z, facing, left, top, right, bottom)    \
  HALFWORDS(entrance, CONDITION_ALWAYS), HALFWORDS(x, y),                      \
      HALFWORDS(z, facing), HALFWORDS(0, left), HALFWORDS(top, right),         \
      HALFWORDS(bottom, 0)
/// The SCENE_TABLE_END row of an entrance table, as six words.
#define ENTRANCE_WORDS_END HALFWORDS(SCENE_TABLE_END, 0), 0, 0, 0, 0, 0
/// A struct SceneRegion row as four words: the box from (minX, minY, minZ) to
/// (maxX, maxY, maxZ) that leaves by exit.
#define REGION_WORDS(minX, minY, minZ, maxX, maxY, maxZ, exit)                 \
  HALFWORDS(minX, minY), HALFWORDS(minZ, maxX), HALFWORDS(maxY, maxZ),         \
      HALFWORDS(CONDITION_ALWAYS, exit)
/// The SCENE_TABLE_END row of a region table, as four words.
#define REGION_WORDS_END HALFWORDS(SCENE_TABLE_END, 0), 0, 0, 0

/// The entrances of neither scene.
s32 gLemurianShip_Entrances[] = {
    ENTRANCE_WORDS(0, 172, 0, 149, FACING_SOUTH, -1, -1, -1, -1),
    ENTRANCE_WORDS_END,
};

/// The cabins' entrances.
u32 gLemurianShip_CabinsEntrances[] = {
    ENTRANCE_WORDS(LEMURIAN_SHIP_CABINS_FROM_LEMURIAN_SHIP_AQUA_HYDRA_NORTH_A, 406, 0, 376, FACING_NORTH, 336, 264, 616, 424),
    ENTRANCE_WORDS(ENTRANCE_SIX_JELLY_ROOM_WEST_HALL_FROM_SIX_JELLY_ROOM, 550, 0, 377, FACING_NORTH, 336, 264, 616, 424),
    ENTRANCE_WORDS(ENTRANCE_SIX_JELLY_ROOM_EAST_HALL_FROM_SIX_JELLY_ROOM, 566, 0, 889, FACING_NORTH, 488, 776, 728, 936),
    ENTRANCE_WORDS(ENTRANCE_SIX_JELLY_ROOM_EAST_HALL_FROM_PIPE_ROOM, 648, 0, 894, FACING_NORTH, 488, 776, 728, 936),
    ENTRANCE_WORDS(ENTRANCE_TWO_JELLY_ROOM_HALL_FROM_TWO_JELLY_ROOM, 440, 0, 216, FACING_EAST, 384, 128, 624, 288),
    ENTRANCE_WORDS(LEMURIAN_SHIP_CABINS_FROM_LEMURIAN_SHIP_AQUA_HYDRA_WEST_A, 567, 0, 215, FACING_SOUTH, 384, 128, 624, 288),
    ENTRANCE_WORDS_END,
};

/// The lower deck's entrances.
struct SceneEntrance gLemurianShip_AquaHydraEntrances[] = {
    {0, CONDITION_ALWAYS, 72, 0, 72, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_NORTHWEST_A, CONDITION_ALWAYS, 72, 0, 72, FACING_SOUTH, 0, 8, 16, 248, 256, 0},
    {LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_NORTH_A, CONDITION_ALWAYS, 168, 0, 72, FACING_SOUTH, 0, 8, 16, 248, 256, 0},
    {LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_WEST_A, CONDITION_ALWAYS, 72, 0, 216, FACING_NORTH, 0, 8, 16, 248, 256, 0},
    {LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_NORTHEAST_A, CONDITION_ALWAYS, 184, 0, 216, FACING_NORTH, 0, 8, 16, 248, 256, 0},
    {LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_SOUTHWEST, CONDITION_ALWAYS, 72, 0, 376, FACING_SOUTH, 0, 8, 320, 248, 560, 0},
    {LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_SOUTH_A, CONDITION_ALWAYS, 168, 0, 376, FACING_SOUTH, 0, 8, 320, 248, 560, 0},
    {LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_SOUTH_B, CONDITION_ALWAYS, 72, 0, 512, FACING_NORTH, 0, 8, 320, 248, 560, 0},
    {LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_SOUTH_C, CONDITION_ALWAYS, 184, 0, 512, FACING_NORTH, 0, 8, 320, 248, 560, 0},
    {ENTRANCE_HOLD_AFTER_AQUA_HYDRA_BATTLE, CONDITION_ALWAYS, 120, 0, 440, FACING_NORTH, 0, 8, 320, 248, 560, 0},
    {SCENE_TABLE_END},
};

/// The cabins' exit regions.
u32 gLemurianShip_CabinsRegions[] = {
    REGION_WORDS(466, -20, 580, 474, -12, 588,
                 LEMURIAN_SHIP_CABINS_TO_LEMURIAN_SHIP_AQUA_HYDRA_EAST_A),
    REGION_WORDS_END,
};

/// The lower deck's exit regions.
u32 gLemurianShip_AquaHydraRegions[] = {
    REGION_WORDS(512, 16, 190, 528, 32, 206,
                 LEMURIAN_SHIP_AQUA_HYDRA_TO_LEMURIAN_SHIP_CABINS_EAST),
    REGION_WORDS_END,
};

u32 gSceneExits[] = {
    SCENE_EXITS(LemurianShipCabins),
    SCENE_EXIT_UNLESS(LEMURIAN_SHIP_CABINS_TO_LEMURIAN_SHIP_MAIN_DECK, LemurianShipMainDeck, LEMURIAN_SHIP_MAIN_DECK_FROM_LEMURIAN_SHIP_CABINS, FLAG_LEMURIA_REACHED),
    SCENE_EXIT_UNLESS(LEMURIAN_SHIP_CABINS_TO_LEMURIAN_SHIP_MAIN_DECK, LemuriaDock, LEMURIA_DOCK_FROM_LEMURIAN_SHIP_CABINS, FLAG_SHIP_AT_ATTEKA_INLET),
    SCENE_EXIT_UNLESS(LEMURIAN_SHIP_CABINS_TO_LEMURIAN_SHIP_MAIN_DECK, AttekaInlet, ATTEKA_INLET_FROM_WORLD_MAP_WEST, EXIT_FLAG_NONE),
    SCENE_EXIT(2, LemurianShipCabins, ENTRANCE_TWO_JELLY_ROOM_FROM_LOGBOOK_ROOM),
    SCENE_EXIT(3, LemurianShipCabins, ENTRANCE_LOGBOOK_ROOM_FROM_TWO_JELLY_ROOM),
    SCENE_EXIT(4, LemurianShipCabins, ENTRANCE_STAIR_ROOM_FROM_TWO_JELLY_ROOM),
    SCENE_EXIT(5, LemurianShipCabins, ENTRANCE_TWO_JELLY_ROOM_HALL_FROM_TWO_JELLY_ROOM),
    SCENE_EXIT(6, LemurianShipCabins, ENTRANCE_TWO_JELLY_ROOM_FROM_STAIR_ROOM),
    SCENE_EXIT(7, LemurianShipCabins, LEMURIAN_SHIP_CABINS_FROM_LEMURIAN_SHIP_AQUA_HYDRA_EAST_A),
    SCENE_EXIT_UNLESS(LEMURIAN_SHIP_CABINS_TO_LEMURIAN_SHIP_AQUA_HYDRA_NORTHWEST_A, LemurianShipAquaHydra, LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_NORTHWEST_A, FLAG_AQUA_HYDRA_BEATEN),
    SCENE_EXIT_UNLESS(LEMURIAN_SHIP_CABINS_TO_LEMURIAN_SHIP_AQUA_HYDRA_NORTHWEST_A, LemurianShipAquaHydra, LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_SOUTHWEST, FLAG_HOLD_PARCHED),
    SCENE_EXIT_UNLESS(LEMURIAN_SHIP_CABINS_TO_LEMURIAN_SHIP_AQUA_HYDRA_NORTHWEST_A, LemurianShipAquaHydra, LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_NORTHWEST_B, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(LEMURIAN_SHIP_CABINS_TO_LEMURIAN_SHIP_AQUA_HYDRA_NORTH_A, LemurianShipAquaHydra, LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_NORTH_A, FLAG_AQUA_HYDRA_BEATEN),
    SCENE_EXIT_UNLESS(LEMURIAN_SHIP_CABINS_TO_LEMURIAN_SHIP_AQUA_HYDRA_NORTH_A, LemurianShipAquaHydra, LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_SOUTH_A, FLAG_HOLD_PARCHED),
    SCENE_EXIT_UNLESS(LEMURIAN_SHIP_CABINS_TO_LEMURIAN_SHIP_AQUA_HYDRA_NORTH_A, LemurianShipAquaHydra, LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_NORTH_B, EXIT_FLAG_NONE),
    SCENE_EXIT(10, LemurianShipCabins, ENTRANCE_SIX_JELLY_ROOM_FROM_WEST_HALL),
    SCENE_EXIT(11, LemurianShipCabins, ENTRANCE_SIX_JELLY_ROOM_FROM_EAST_HALL),
    SCENE_EXIT(12, LemurianShipCabins, ENTRANCE_PIPE_ROOM_FROM_SIX_JELLY_ROOM_EAST_HALL),
    SCENE_EXIT(13, LemurianShipCabins, ENTRANCE_TWO_JELLY_ROOM_FROM_TWO_JELLY_ROOM_HALL),
    SCENE_EXIT_UNLESS(LEMURIAN_SHIP_CABINS_TO_LEMURIAN_SHIP_AQUA_HYDRA_WEST_A, LemurianShipAquaHydra, LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_WEST_A, FLAG_AQUA_HYDRA_BEATEN),
    SCENE_EXIT_UNLESS(LEMURIAN_SHIP_CABINS_TO_LEMURIAN_SHIP_AQUA_HYDRA_WEST_A, LemurianShipAquaHydra, LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_SOUTH_B, FLAG_HOLD_PARCHED),
    SCENE_EXIT_UNLESS(LEMURIAN_SHIP_CABINS_TO_LEMURIAN_SHIP_AQUA_HYDRA_WEST_A, LemurianShipAquaHydra, LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_WEST_B, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(LEMURIAN_SHIP_CABINS_TO_LEMURIAN_SHIP_AQUA_HYDRA_NORTHEAST_A, LemurianShipAquaHydra, LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_NORTHEAST_A, FLAG_AQUA_HYDRA_BEATEN),
    SCENE_EXIT_UNLESS(LEMURIAN_SHIP_CABINS_TO_LEMURIAN_SHIP_AQUA_HYDRA_NORTHEAST_A, LemurianShipAquaHydra, LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_SOUTH_C, FLAG_HOLD_PARCHED),
    SCENE_EXIT_UNLESS(LEMURIAN_SHIP_CABINS_TO_LEMURIAN_SHIP_AQUA_HYDRA_NORTHEAST_A, LemurianShipAquaHydra, LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_NORTHEAST_B, EXIT_FLAG_NONE),
    SCENE_EXIT(16, LemurianShipCabins, ENTRANCE_SIX_JELLY_ROOM_FROM_LEDGE_ROOM_WEST),
    SCENE_EXIT(17, LemurianShipCabins, ENTRANCE_SIX_JELLY_ROOM_FROM_LEDGE_ROOM_EAST),
    SCENE_EXIT(18, LemurianShipCabins, ENTRANCE_PIPE_ROOM_FROM_LEDGE_ROOM),
    SCENE_EXIT(LEMURIAN_SHIP_CABINS_TO_LEMURIAN_SHIP_AQUA_HYDRA_EAST_A, LemurianShipAquaHydra, LEMURIAN_SHIP_AQUA_HYDRA_FROM_LEMURIAN_SHIP_CABINS_EAST),
    SCENE_EXIT(20, LemurianShipCabins, ENTRANCE_STAIR_ROOM_FROM_STAIR_ROOM_HALL),
    SCENE_EXIT(25, LemurianShipCabins, ENTRANCE_SIX_JELLY_ROOM_WEST_HALL_FROM_SIX_JELLY_ROOM),
    SCENE_EXIT(26, LemurianShipCabins, ENTRANCE_SIX_JELLY_ROOM_EAST_HALL_FROM_SIX_JELLY_ROOM),
    SCENE_EXIT(27, LemurianShipCabins, ENTRANCE_LEDGE_ROOM_FROM_SIX_JELLY_ROOM_WEST),
    SCENE_EXIT(28, LemurianShipCabins, ENTRANCE_LEDGE_ROOM_FROM_SIX_JELLY_ROOM_EAST),
    SCENE_EXIT(29, LemurianShipCabins, ENTRANCE_SIX_JELLY_ROOM_EAST_HALL_FROM_PIPE_ROOM),
    SCENE_EXIT(30, LemurianShipCabins, ENTRANCE_LEDGE_ROOM_FROM_PIPE_ROOM),
    SCENE_EXITS(LemurianShipAquaHydra),
    SCENE_EXIT(LEMURIAN_SHIP_AQUA_HYDRA_TO_LEMURIAN_SHIP_CABINS_NORTHWEST_A, LemurianShipCabins, LEMURIAN_SHIP_CABINS_FROM_LEMURIAN_SHIP_AQUA_HYDRA_NORTHWEST_A),
    SCENE_EXIT(LEMURIAN_SHIP_AQUA_HYDRA_TO_LEMURIAN_SHIP_CABINS_NORTH_A, LemurianShipCabins, LEMURIAN_SHIP_CABINS_FROM_LEMURIAN_SHIP_AQUA_HYDRA_NORTH_A),
    SCENE_EXIT(LEMURIAN_SHIP_AQUA_HYDRA_TO_LEMURIAN_SHIP_CABINS_WEST_A, LemurianShipCabins, LEMURIAN_SHIP_CABINS_FROM_LEMURIAN_SHIP_AQUA_HYDRA_WEST_A),
    SCENE_EXIT(LEMURIAN_SHIP_AQUA_HYDRA_TO_LEMURIAN_SHIP_CABINS_NORTHEAST_A, LemurianShipCabins, LEMURIAN_SHIP_CABINS_FROM_LEMURIAN_SHIP_AQUA_HYDRA_NORTHEAST_A),
    SCENE_EXIT(6, LemurianShipAquaHydra, ENTRANCE_CORRIDOR_FROM_PILLAR_ROOM),
    SCENE_EXIT(7, LemurianShipAquaHydra, ENTRANCE_PILLAR_ROOM_FROM_CORRIDOR),
    SCENE_EXIT_UNLESS(8, LemurianShipAquaHydra, ENTRANCE_ENGINE_ROOM_FROM_CORRIDOR, FLAG_SHIP_ENGINE_RUNNING),
    SCENE_EXIT_UNLESS(8, LemurianShipAquaHydra, ENTRANCE_RUNNING_ENGINE_ROOM_FROM_CORRIDOR, EXIT_FLAG_NONE),
    SCENE_EXIT(9, LemurianShipAquaHydra, ENTRANCE_CORRIDOR_FROM_ENGINE_ROOM),
    SCENE_EXIT(LEMURIAN_SHIP_AQUA_HYDRA_TO_LEMURIAN_SHIP_CABINS_EAST, LemurianShipCabins, LEMURIAN_SHIP_CABINS_FROM_LEMURIAN_SHIP_AQUA_HYDRA_EAST_B),
    SCENE_EXIT(LEMURIAN_SHIP_AQUA_HYDRA_TO_WORLD_MAP, WorldMap, WORLD_MAP_SAIL_TO_DECK),
    SCENE_EXITS_END,
};

/// The actors of neither scene: none.
struct ActorPlacement gLemurianShip_Placements[] = {
    {SCENE_TABLE_END},
};

/// The room of two jellies' actors, by enum LemurianShipTwoJellyRoomActorId.
struct ActorPlacement gLemurianShip_TwoJellyRoomPlacements[] = {                                             
      {SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, 1, toFIXED(104), 0, toFIXED(424), FACING_SOUTH, 0,   
       0},                                                                     
      {SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, 1, toFIXED(104), 0, toFIXED(536), FACING_SOUTH, 0,   
       0},                                                                     
      ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),               
      ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),               
      {SCENE_TABLE_END},                                                       
  };

/// The room of six jellies' actors, by enum LemurianShipSixJellyRoomActorId.
struct ActorPlacement gLemurianShip_SixJellyRoomPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_TALL_CRATE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 760, 0, 120, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 856, 0, 88, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 744, 0, 88, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 760, 0, 152, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 792, 0, 120, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 824, 0, 120, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 824, 0, 152, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

/// The ledge room's actors, by enum LemurianShipLedgeRoomActorId.
struct ActorPlacement gLemurianShip_LedgeRoomPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_TALL_CRATE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 744, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

/// The room of the pipes' actors, by enum
/// LemurianShipPipeRoomActorId.
struct ActorPlacement gLemurianShip_PipeRoomPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_TALL_CRATE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 824, 0, 552, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 776, 0, 520, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 856, 0, 520, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 696, 0, 600, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 776, 0, 600, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 808, 0, 600, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 776, 0, 696, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

/// The hold's actors, by enum LemurianShipHoldActorId.
struct ActorPlacement gLemurianShip_HoldPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 120, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 56, 0, 88, FACING_SOUTH, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 120, FACING_SOUTH, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 152, FACING_SOUTH, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 88, FACING_SOUTH, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 152, FACING_SOUTH, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_PIERS, FLAG_IN_PARTY(ACTOR_ISAAC), MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/// The pillar room's actors, by enum LemurianShipPillarRoomActorId.
struct ActorPlacement gLemurianShip_PillarRoomPlacements[] = {                                             
      {SPRITE_SINKING_PILLAR, CONDITION_ALWAYS, 1, toFIXED(584), 0, toFIXED(168), FACING_SOUTH, 2,   
       0},                                                                     
      {SPRITE_SINKING_PILLAR, CONDITION_ALWAYS, 1, toFIXED(472), 0, toFIXED(152), FACING_SOUTH, 2,   
       0},                                                                     
      ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),               
      ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),               
      {SCENE_TABLE_END},                                                       
  };

/// The engine room's actors, by enum LemurianShipEngineRoomActorId.
struct ActorPlacement gLemurianShip_EngineRoomPlacements[] = {                                             
      ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),                
      ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),                
      ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),                
      ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),                
      {SCENE_TABLE_END},                                                       
  };

/// The steps of a cabin door opening.
s16 gLemurianShip_DoorOpening[] = {
    0, 126, 1, 2, 2, 1, 126, 1,
    2, 4, 2, 126, 1, 2, 4, -1,
};

/// The steps of the first cabin door opening.
s16 gLemurianShip_FirstDoorOpening[] = {
    10, 126, 1, 2, 4, 11, 126, 1,
    2, 4, -1, 4, 126, 2, 2, 2,
    -1, 0,
};

/// The cabin doors, by the step trigger that opens each.
struct LemurianShipWarp gLemurianShip_Doors[CABIN_DOOR_COUNT] = {
    {2, (s32)gLemurianShip_FirstDoorOpening, 4, 3},
    {14, (s32)gLemurianShip_DoorOpening, 35, 11},
    {15, (s32)gLemurianShip_DoorOpening, 6, 45},
    {16, (s32)gLemurianShip_DoorOpening, 11, 45},
    {17, (s32)gLemurianShip_DoorOpening, 21, 45},
    {18, (s32)gLemurianShip_DoorOpening, 26, 46},
    {20, (s32)gLemurianShip_DoorOpening, 29, 28},
    {25, (s32)gLemurianShip_DoorOpening, 48, 2},
    {26, (s32)gLemurianShip_DoorOpening, 54, 2},
    {29, (s32)gLemurianShip_DoorOpening, 41, 29},
};

/// The lower deck's doors, by the step trigger that opens each, then the
/// hold's two doors while it is flooded.
struct LemurianShipWarp gLemurianShip_LowerDoors[LOWER_DOOR_COUNT + 2] = {
    {1, (s32)gLemurianShip_DoorOpening, 4, 2},
    {2, (s32)gLemurianShip_DoorOpening, 10, 2},
    {6, (s32)gLemurianShip_DoorOpening, 31, 3},
    {8, (s32)gLemurianShip_DoorOpening, 41, 51},
    {11, (s32)gLemurianShip_DoorOpening, 4, 21},
    {12, (s32)gLemurianShip_DoorOpening, 10, 21},
};

/// The triggers of neither scene: none.
struct Trigger gLemurianShip_Triggers[] = {
    {TRIGGER_TABLE_END},
};

/// The cabins' triggers.
struct Trigger gLemurianShip_CabinTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT(LemurianShip_OpenDoor)},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 14, CONDITION_ALWAYS, EVENT(LemurianShip_OpenDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 20, CONDITION_ALWAYS, EVENT(LemurianShip_OpenDoor)},
    {EXAMINE_TRIGGER(EXAMINE_BOOKCASE), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemurianShipItLooksLikeTheresCaptains))},
    {EXAMINE_TRIGGER(EXAMINE_WARDROBE), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemurianShipItsPleasantlyCool))},
    {TRIGGER_TABLE_END},
};

/// The room of two jellies' triggers.
struct Trigger gLemurianShip_TwoJellyRoomTriggers[] = {
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_TALK, ACTOR_TWO_JELLY_ROOM_JELLY_A, CONDITION_ALWAYS, EVENT(LemurianShip_FightTwoJellyRoomJelly)},
    {TRIGGER_TALK, ACTOR_TWO_JELLY_ROOM_JELLY_B, CONDITION_ALWAYS, EVENT(LemurianShip_FightTwoJellyRoomJelly)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_TWO_JELLY_ROOM_PUDDLE_A, FLAG_PUDDLE_FROZEN + JELLY_TWO_JELLY_ROOM_A, EVENT(LemurianShip_FreezeTwoJellyRoomPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_TWO_JELLY_ROOM_PUDDLE_B, FLAG_PUDDLE_FROZEN + JELLY_TWO_JELLY_ROOM_B, EVENT(LemurianShip_FreezeTwoJellyRoomPuddle)},
    {TRIGGER_TABLE_END},
};

/// The room of six jellies' triggers.
struct Trigger gLemurianShip_SixJellyRoomTriggers[] = {
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 25, CONDITION_ALWAYS, EVENT(LemurianShip_OpenDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 26, CONDITION_ALWAYS, EVENT(LemurianShip_OpenDoor)},
    {TRIGGER_EXIT, 27, CONDITION_ALWAYS, 27},
    {TRIGGER_EXIT, 28, CONDITION_ALWAYS, 28},
    {TRIGGER_TALK, ACTOR_SIX_JELLY_ROOM_JELLY_A, CONDITION_ALWAYS, EVENT(LemurianShip_FightSixJellyRoomJelly)},
    {TRIGGER_TALK, ACTOR_SIX_JELLY_ROOM_JELLY_B, CONDITION_ALWAYS, EVENT(LemurianShip_FightSixJellyRoomJelly)},
    {TRIGGER_TALK, ACTOR_SIX_JELLY_ROOM_JELLY_C, CONDITION_ALWAYS, EVENT(LemurianShip_FightSixJellyRoomJelly)},
    {TRIGGER_TALK, ACTOR_SIX_JELLY_ROOM_JELLY_D, CONDITION_ALWAYS, EVENT(LemurianShip_FightSixJellyRoomJelly)},
    {TRIGGER_TALK, ACTOR_SIX_JELLY_ROOM_JELLY_E, CONDITION_ALWAYS, EVENT(LemurianShip_FightSixJellyRoomJelly)},
    {TRIGGER_TALK, ACTOR_SIX_JELLY_ROOM_JELLY_F, CONDITION_ALWAYS, EVENT(LemurianShip_FightSixJellyRoomJelly)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SIX_JELLY_ROOM_LIFT_PUDDLE, (FLAG_PUDDLE_FROZEN + JELLY_SIX_JELLY_ROOM_A), EVENT(LemurianShip_FreezeLiftPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SIX_JELLY_ROOM_PUDDLE_A, (FLAG_PUDDLE_FROZEN + JELLY_SIX_JELLY_ROOM_B), EVENT(LemurianShip_FreezeSixJellyRoomPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SIX_JELLY_ROOM_PUDDLE_B, (FLAG_PUDDLE_FROZEN + JELLY_SIX_JELLY_ROOM_C), EVENT(LemurianShip_FreezeSixJellyRoomPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SIX_JELLY_ROOM_PUDDLE_C, (FLAG_PUDDLE_FROZEN + JELLY_SIX_JELLY_ROOM_D), EVENT(LemurianShip_FreezeSixJellyRoomPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SIX_JELLY_ROOM_PUDDLE_D, (FLAG_PUDDLE_FROZEN + JELLY_SIX_JELLY_ROOM_E), EVENT(LemurianShip_FreezeSixJellyRoomPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SIX_JELLY_ROOM_PUDDLE_E, (FLAG_PUDDLE_FROZEN + JELLY_SIX_JELLY_ROOM_F), EVENT(LemurianShip_FreezeSixJellyRoomPuddle)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 8, CONDITION_ALWAYS, EVENT(LemurianShip_LiftCrate)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SIX_JELLY_ROOM_CRATE, CONDITION_ALWAYS, EVENT(LemurianShip_SetDownCrate)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(LemurianShip_LiftCrate)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(LemurianShip_SetDownCrate)},
    {TRIGGER_EXAMINE, 40, CONDITION_ALWAYS, EVENT(LemurianShip_ExamineLiftSwitch)},
    {TRIGGER_STEP, 41, (FLAG_PUDDLE_FROZEN + JELLY_SIX_JELLY_ROOM_A), EVENT(LemurianShip_StepOnLift)},
    {TRIGGER_STEP, 43, (FLAG_PUDDLE_FROZEN + JELLY_SIX_JELLY_ROOM_A), EVENT(LemurianShip_StepOnLiftWestSide)},
    {TRIGGER_STEP, 42, FLAG_JELLY_FLED, EVENT(LemurianShip_ScareJelly)},
    {TRIGGER_TABLE_END},
};

/// The ledge room's triggers.
struct Trigger gLemurianShip_LedgeRoomTriggers[] = {
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 15, CONDITION_ALWAYS, EVENT(LemurianShip_OpenDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 16, CONDITION_ALWAYS, EVENT(LemurianShip_OpenDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 17, CONDITION_ALWAYS, EVENT(LemurianShip_OpenDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 18, CONDITION_ALWAYS, EVENT(LemurianShip_OpenDoor)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 8, CONDITION_ALWAYS, EVENT(LemurianShip_LiftLedgeCrate)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_LEDGE_CRATE, CONDITION_ALWAYS, EVENT(LemurianShip_PushCrateOffLedge)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(LemurianShip_LiftLedgeCrate)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(LemurianShip_PushCrateOffLedge)},
    {TRIGGER_TABLE_END},
};

/// The room of the pipes' triggers.
struct Trigger gLemurianShip_PipeRoomTriggers[] = {
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 29, CONDITION_ALWAYS, EVENT(LemurianShip_OpenDoor)},
    {TRIGGER_EXIT, 30, CONDITION_ALWAYS, 30},
    {TALK_TRIGGER_FACING(FACING_EAST), ACTOR_PIPE_ROOM_JELLY_A, CONDITION_ALWAYS, EVENT(LemurianShip_FightPipeRoomJelly)},
    {TALK_TRIGGER_FACING(FACING_SOUTH), ACTOR_PIPE_ROOM_JELLY_A, CONDITION_ALWAYS, EVENT(LemurianShip_FightPipeRoomJelly)},
    {TALK_TRIGGER_FACING(FACING_WEST), ACTOR_PIPE_ROOM_JELLY_B, CONDITION_ALWAYS, EVENT(LemurianShip_FightPipeRoomJelly)},
    {TALK_TRIGGER_FACING(FACING_SOUTH), ACTOR_PIPE_ROOM_JELLY_B, CONDITION_ALWAYS, EVENT(LemurianShip_FightPipeRoomJelly)},
    {TALK_TRIGGER_FACING(FACING_NORTH), ACTOR_PIPE_ROOM_JELLY_B, CONDITION_ALWAYS, EVENT(LemurianShip_FightPipeRoomJelly)},
    {TRIGGER_TALK, ACTOR_PIPE_ROOM_JELLY_C, CONDITION_ALWAYS, EVENT(LemurianShip_FightPipeRoomJelly)},
    {TRIGGER_TALK, ACTOR_PIPE_ROOM_JELLY_D, CONDITION_ALWAYS, EVENT(LemurianShip_FightPipeRoomJelly)},
    {TALK_TRIGGER_FACING(FACING_NORTH), ACTOR_PIPE_ROOM_JELLY_E, CONDITION_ALWAYS, EVENT(LemurianShip_FightPipeRoomJelly)},
    {TALK_TRIGGER_FACING(FACING_EAST), ACTOR_PIPE_ROOM_JELLY_G, CONDITION_ALWAYS, EVENT(LemurianShip_FightPipeRoomJelly)},
    {TALK_TRIGGER_FACING(FACING_WEST), ACTOR_PIPE_ROOM_JELLY_G, CONDITION_ALWAYS, EVENT(LemurianShip_FightPipeRoomJelly)},
    {TALK_TRIGGER_FACING(FACING_NORTH), ACTOR_PIPE_ROOM_JELLY_G, CONDITION_ALWAYS, EVENT(LemurianShip_FightPipeRoomJelly)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PIPE_ROOM_PUDDLE_A, (FLAG_PUDDLE_FROZEN + JELLY_PIPE_ROOM_A), EVENT(LemurianShip_FreezePipeRoomPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PIPE_ROOM_PUDDLE_B, (FLAG_PUDDLE_FROZEN + JELLY_PIPE_ROOM_B), EVENT(LemurianShip_FreezePipeRoomPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PIPE_ROOM_PUDDLE_C, (FLAG_PUDDLE_FROZEN + JELLY_PIPE_ROOM_C), EVENT(LemurianShip_FreezePipeRoomPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PIPE_ROOM_PUDDLE_D, (FLAG_PUDDLE_FROZEN + JELLY_PIPE_ROOM_D), EVENT(LemurianShip_FreezePipeRoomPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PIPE_ROOM_PUDDLE_E, (FLAG_PUDDLE_FROZEN + JELLY_PIPE_ROOM_E), EVENT(LemurianShip_FreezePipeRoomPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PIPE_ROOM_PUDDLE_G, (FLAG_PUDDLE_FROZEN + JELLY_PIPE_ROOM_G), EVENT(LemurianShip_FreezePipeRoomPuddle)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 8, CONDITION_ALWAYS, EVENT(LemurianShip_LiftCrate)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PIPE_ROOM_CRATE, CONDITION_ALWAYS, EVENT(LemurianShip_SetDownCrate)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(LemurianShip_LiftCrate)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(LemurianShip_SetDownCrate)},
    {TRIGGER_TABLE_END},
};

/// The hold's triggers.
struct Trigger gLemurianShip_HoldTriggers[] = {
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT(LemurianShip_OpenLowerDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT(LemurianShip_OpenLowerDoor)},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_TALK, ACTOR_HOLD_JELLY_A, CONDITION_ALWAYS, EVENT(LemurianShip_FightAquaHydra)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_PARCH, PSYNERGY_PHASE_FIRST), 255, CONDITION_ALWAYS, EVENT(LemurianShip_StartParch)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_PARCH, PSYNERGY_PHASE_ACTS), 255, CONDITION_ALWAYS, EVENT(LemurianShip_ParchHold)},
    {TRIGGER_TABLE_END},
};

/// The pillar room's triggers.
struct Trigger gLemurianShip_PillarRoomTriggers[] = {
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT(LemurianShip_WalkIntoPassage)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_RIGHT_PILLAR, FLAG_RIGHT_PILLAR_SUNK, EVENT(LemurianShip_LiftPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_RIGHT_PILLAR, FLAG_RIGHT_PILLAR_SUNK, EVENT(LemurianShip_SetDownPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_LEFT_PILLAR, FLAG_LEFT_PILLAR_SUNK, EVENT(LemurianShip_LiftPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_LEFT_PILLAR, FLAG_LEFT_PILLAR_SUNK, EVENT(LemurianShip_SetDownPillar)},
    {TRIGGER_PUSH, 0, FLAG_PILLAR_WAY_OPEN, EVENT(LemurianShip_LiftPillar)},
    {TRIGGER_PUSHED, 0, FLAG_PILLAR_WAY_OPEN, EVENT(LemurianShip_SetDownPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_RIGHT_PILLAR, FLAG_RIGHT_PILLAR_FULL, EVENT(LemurianShip_DouseRightPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_LEFT_PILLAR, FLAG_LEFT_PILLAR_FULL, EVENT(LemurianShip_DouseLeftPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_FROST, PSYNERGY_PHASE_ACTS), ACTOR_RIGHT_PILLAR_PUDDLE, FLAG_RIGHT_PILLAR_PUDDLE_FROZEN, EVENT(LemurianShip_ShowRightPillarPuddle)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_FROST, PSYNERGY_PHASE_ACTS), ACTOR_LEFT_PILLAR_PUDDLE, FLAG_LEFT_PILLAR_PUDDLE_FROZEN, EVENT(LemurianShip_ShowLeftPillarPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_RIGHT_PILLAR_PUDDLE, FLAG_RIGHT_PILLAR_PUDDLE_FROZEN, EVENT(LemurianShip_FreezeTwoJellyRoomPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_LEFT_PILLAR_PUDDLE, FLAG_LEFT_PILLAR_PUDDLE_FROZEN, EVENT(LemurianShip_FreezeTwoJellyRoomPuddle)},
    {TRIGGER_RAISED, RAISED_RIGHT_PILLAR, CONDITION_ALWAYS, EVENT(LemurianShip_RaiseRightPillar)},
    {TRIGGER_RAISED, RAISED_LEFT_PILLAR, CONDITION_ALWAYS, EVENT(LemurianShip_RaiseLeftPillar)},
    {TRIGGER_TABLE_END},
};

/// The lower deck's triggers.
struct Trigger gLemurianShip_LowerDeckTriggers[] = {
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS, EVENT(LemurianShip_WalkIntoPassage)},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_TABLE_END},
};

/// A glint fading as it reaches the orb.
u32 gLemurianShip_LightToOrb[] = {
    0, 10, 1, 3, 0, 1,
    38,
};
// clang-format on
#undef PLACE_ON_LOWER_FLOOR
#undef HALFWORDS
#undef ENTRANCE_WORDS
#undef ENTRANCE_WORDS_END
#undef REGION_WORDS
#undef REGION_WORDS_END
