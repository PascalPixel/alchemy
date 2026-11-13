/// @file
/// Sol Entrance and the first rooms of Sol Sanctum, two scenes one overlay
/// serves, chosen by gMainRoutineWork.next in every entry point.
///
/// - Sol Entrance (SceneId_SolEntrance), the mountainside before the sanctum's
///   door, reached from Vale (VALE_TO_SOL_ENTRANCE). Walking north into the
///   door opens it and takes the leader in
///   (SolSanctumEntrance_OpenDoorAndEnter). Searching the other door finds it
///   "tightly locked". Retreat does not work here. The way down leads to
///   SceneId_Vale, except between the eruption (FLAG_SOL_SANCTUM_ERUPTED) and
///   the party leaving Vale (FLAG_VALE_PARTY_LEFT), when it leads to
///   SceneId_ValeEruption. After the eruption the ground rumbles now and then.
///   Once the party has left Vale a man stands in the path and warns everyone
///   away.
/// - Sol Sanctum (SceneId_SolSanctumReliefs), the rooms inside the door, which
///   Isaac, Garet and Jenna explore with Kraden. Two minotaur reliefs each have
///   an empty eye socket, and a Small Jewel set in it opens the way on. The
///   first jewel comes from this scene's chest
///   (FLAG_SOL_SANCTUM_FIRST_SMALL_JEWEL_SET), the second from the chest in
///   SceneId_SolSanctumMaze (FLAG_SOL_SANCTUM_SECOND_SMALL_JEWEL_SET). Kraden
///   has an event on the first entry at the door
///   (SolSanctumEntrance_IntroduceSanctum), in the first relief room
///   (SolSanctumEntrance_KradenSuspectsHiddenPassage) and in the second
///   (SolSanctumEntrance_KradenSeesStatuesOutOfReach). Two statues in the first
///   relief room and the row of six in the second can be pushed, or moved with
///   Move, a cell east or west. Its exits lead to SceneId_SolSanctumCorridor,
///   SceneId_SolSanctumMaze, Sol Entrance and SceneId_SolSanctumLunaSolRooms.
///   The rest lead between its own rooms.
///
/// SceneId_SolSanctumReliefs is one scene, but each relief room has actor
/// placement and trigger tables of its own.
/// Scene_GetActorPlacements and _GetTriggers choose them by the
/// entrance the party came in at, so a room's statues and their events exist
/// only while the party is in it.
///
/// A pushed statue stays where it was left for the rest of the visit. Each
/// pushable statue's column is kept in two area flags, one for each side, which
/// SolSanctumEntrance_RestoreInteriorState reads to put it back on entry.
/// Scene_Arrive clears them when the party enters another area, so the statues
/// are back at their starts on the next visit.
///
/// Searching a relief while its Small Jewel is held asks for the field menu as
/// the search ends, so that the jewel can be used on it at once.
///
/// Once Isaac and Garet are back in Vale (FLAG_VALE_RETURNED_FROM_MT_ALEPH) the
/// door no longer opens. The leader walks up to the door and stops, and
/// searching it finds it "tightly locked" (render probe sol-entrance). Sol
/// Entrance's setup does this with two changes. It makes the three wall cells
/// across the door CELL_BEHAVIOR_SOL_ENTRANCE_LOCKED_DOOR, copied from the
/// other door. And it makes the three CELL_BEHAVIOR_SOL_ENTRANCE_DOOR cells
/// before it plain ground.

#include "main_routine_work.h"
#include "module/import.h"
#include "types.h"
#include "scene_tables.h"

const struct SceneEntrance *Scene_GetEntrances(void);
const struct SceneExitRegion *Scene_GetExitRegions(void);
const u32 *Scene_GetExits(void);
const struct ActorPlacement *Scene_GetActorPlacements(void);
const struct Trigger *Scene_GetTriggers(void);
s32 Scene_Initialize(void);
extern struct SceneEntrance gSolSanctumEntrance_FallbackEntrances[];
extern struct SceneEntrance gSolEntrance_Entrances[];
extern struct SceneEntrance gSolSanctumReliefs_Entrances[];
extern u32 gSolSanctumEntrance_Exits[];
extern struct ActorPlacement gSolSanctumEntrance_FallbackActorPlacements[];
extern struct ActorPlacement gSolSanctumReliefs_ActorPlacements[];
extern struct ActorPlacement
    gSolSanctumReliefs_FirstReliefRoomActorPlacements[];
extern struct ActorPlacement
    gSolSanctumReliefs_SecondReliefRoomActorPlacements[];
extern struct ActorPlacement gSolEntrance_ActorPlacements[];
extern struct Trigger gSolSanctumEntrance_FallbackTriggers[];
extern struct Trigger gSolEntrance_Triggers[];
extern struct Trigger gSolSanctumReliefs_Triggers[];
extern struct Trigger gSolSanctumReliefs_FirstReliefRoomTriggers[];
extern struct Trigger gSolSanctumReliefs_SecondReliefRoomTriggers[];
extern s32 gSolSanctumPushSteps[];

#include "module/field/event_commands.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "event_inline.h"
#include "types.h"
#include "event.h"
#include "scene_tables.h"
#include "flags.h"

/// In Sol Sanctum (SceneId_SolSanctumReliefs), Kraden follows the party; Garet
/// and Jenna, placed before him, take their own party slots.
enum { ACTOR_KRADEN = ACTOR_FIRST_PLACED };

/// At Sol Entrance (SceneId_SolEntrance), the one placed actor is a man
/// with long brown hair and a beard in a long robe, who stands in the path once
/// the party has left Vale and warns everyone away while Mt. Aleph is active;
/// read with Mind Read, he is all alone in this dangerous place and asks how
/// the Great Healer can treat him like this.
enum { ACTOR_WARNING_MAN = ACTOR_FIRST_PLACED };

/// The statues, placed after Kraden. They share one sprite, which Kraden calls
/// statues ("More statues..."). Outside the two rooms below, Sol Sanctum places
/// one, in the statue nook off the first relief room
/// (SOL_SANCTUM_RELIEFS_STATUE_NOOK_FROM_FIRST_RELIEF_ROOM).
enum { ACTOR_STATUE = ACTOR_FIRST_PLACED + 1 };

/// The first relief room's eleven statues: the two that can be pushed,
/// then nine that cannot.
enum {
  ACTOR_WEST_STATUE = ACTOR_FIRST_PLACED + 1,
  ACTOR_EAST_STATUE,
  /// The first of the nine.
  ACTOR_FIRST_FIXED_STATUE
};

/// The second relief room's row of six, each of which can be pushed a cell
/// east or west; alike, they are numbered west to east, each starting 64 pixels
/// east of the one before (SolSanctumEntrance_RestoreInteriorState).
enum {
  ACTOR_ROW_STATUE_1 = ACTOR_FIRST_PLACED + 1,
  ACTOR_ROW_STATUE_2,
  ACTOR_ROW_STATUE_3,
  ACTOR_ROW_STATUE_4,
  ACTOR_ROW_STATUE_5,
  ACTOR_ROW_STATUE_6
};

extern s32 gSolSanctumEntrance_RumbleTimer;

#include "text/msg_ids.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "shared/walk_into_leader.h"

// Area flags: Scene_Arrive clears 0x300 to 0x3ff when the
// party enters another area, and other areas use the numbers for
// their own.

/// The column each statue that can be pushed stands in, east or west of its
/// start.
///
/// They are the first relief room's west and east statues, then the row of six
/// in the second relief room. Both clear means the start.
#define FLAG_SOL_SANCTUM_EAST_STATUE_AT_EAST FLAG_AREA_TEMP_00
#define FLAG_SOL_SANCTUM_EAST_STATUE_AT_WEST FLAG_AREA_TEMP_01
#define FLAG_SOL_SANCTUM_WEST_STATUE_AT_EAST FLAG_AREA_TEMP_02
#define FLAG_SOL_SANCTUM_WEST_STATUE_AT_WEST FLAG_AREA_TEMP_03
/// The second relief room's row of six statues, each two flags as above.
#define FLAG_SOL_SANCTUM_ROW_STATUE_1_AT_EAST FLAG_AREA_TEMP_10
#define FLAG_SOL_SANCTUM_ROW_STATUE_1_AT_WEST FLAG_AREA_TEMP_11
#define FLAG_SOL_SANCTUM_ROW_STATUE_2_AT_EAST FLAG_AREA_TEMP_12
#define FLAG_SOL_SANCTUM_ROW_STATUE_2_AT_WEST FLAG_AREA_TEMP_13
#define FLAG_SOL_SANCTUM_ROW_STATUE_3_AT_EAST FLAG_AREA_TEMP_14
#define FLAG_SOL_SANCTUM_ROW_STATUE_3_AT_WEST FLAG_AREA_TEMP_15
#define FLAG_SOL_SANCTUM_ROW_STATUE_4_AT_EAST FLAG_AREA_TEMP_16
#define FLAG_SOL_SANCTUM_ROW_STATUE_4_AT_WEST FLAG_AREA_TEMP_17
#define FLAG_SOL_SANCTUM_ROW_STATUE_5_AT_EAST FLAG_AREA_TEMP_18
#define FLAG_SOL_SANCTUM_ROW_STATUE_5_AT_WEST FLAG_AREA_TEMP_19
#define FLAG_SOL_SANCTUM_ROW_STATUE_6_AT_EAST FLAG_AREA_TEMP_1A
#define FLAG_SOL_SANCTUM_ROW_STATUE_6_AT_WEST FLAG_AREA_TEMP_1B

// Scene flags, cleared on every ordinary arrival:
// - FLAG_SOL_SANCTUM_IN_SECOND_RELIEF_ROOM: set on entering the second relief
//   room; the Move rows of the first relief room apply only while it is clear,
//   which it always is there: only the second room's entrances set it,
//   every ordinary entrance into the first room clears it, and a return
//   after a battle comes back by the entrance the party last came in
//   by, so the condition never stops a Move.

// An exit's flag word of -1 stands for no flag: GameFlag_GetFar reads it as
// flag 0xfff, which nothing sets, so the exit is always taken.
#define EXIT_FLAG_NONE 0xffffffff

/// The cell behaviors of the cells the Sol Sanctum entrance's coord, bg and
/// item triggers answer.
///
/// Behaviors 1 to 16 in both scenes are the exits, in the exit table's order.
enum SolEntranceCellBehavior {
  CELL_BEHAVIOR_SOL_ENTRANCE_DOOR = 2,
  CELL_BEHAVIOR_SOL_ENTRANCE_LOCKED_DOOR = 10
};

/// The cell behaviors of the cells the relief rooms' coord, bg and
/// item triggers answer.
enum SolSanctumReliefsCellBehavior {
  CELL_BEHAVIOR_SOL_SANCTUM_RELIEFS_FIRST_RELIEF = 20,
  CELL_BEHAVIOR_SOL_SANCTUM_RELIEFS_SECOND_RELIEF = 21,
  /// The first relief room's statues, pushed from any side.
  CELL_BEHAVIOR_SOL_SANCTUM_RELIEFS_FIRST_ROOM_STATUE = 22,
  /// The first relief room's statues, pushed east only.
  CELL_BEHAVIOR_SOL_SANCTUM_RELIEFS_FIRST_ROOM_STATUE_EAST = 23,
  /// The second relief room's row of statues.
  CELL_BEHAVIOR_SOL_SANCTUM_RELIEFS_ROW_STATUE = 24,
  /// The first relief room's statues, pushed west only.
  CELL_BEHAVIOR_SOL_SANCTUM_RELIEFS_FIRST_ROOM_STATUE_WEST = 25,
  CELL_BEHAVIOR_SOL_SANCTUM_RELIEFS_CHEST = 100
};

/// An item trigger's subject bit that skips the `<actor> uses <item>.` message,
/// so the event shows its own (Field_UseItem).
#define SOL_SANCTUM_ITEM_OWN_MESSAGE 0x400

static void SolSanctumEntrance_ApplyEntryState(void);
static void SolSanctumEntrance_RestoreInteriorState(void);
static void SolSanctumEntrance_IntroduceSanctum(void);
static void SolSanctumEntrance_KradenSuspectsHiddenPassage(void);
static void SolSanctumEntrance_KradenSeesStatuesOutOfReach(void);
static void SolSanctumEntrance_RestoreRowStatueCells(s32 placeActors);
static void SolSanctumEntrance_UpdateEruptionRumbles(void);

s32 gSolSanctumEntrance_RumbleTimer __attribute__((section(".bss")));

/// Returns Sol Entrance's entrances (SceneId_SolEntrance) or Sol
/// Sanctum's (SceneId_SolSanctumReliefs).
const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 sceneId = gMainRoutineWork.next;

  if (sceneId == SCENE_SOL_ENTRANCE) {
    return gSolEntrance_Entrances;
  }
  if (sceneId == SCENE_SOL_SANCTUM_RELIEFS) {
    return gSolSanctumReliefs_Entrances;
  }
  return gSolSanctumEntrance_FallbackEntrances;
}

const struct SceneExitRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSolSanctumEntrance_Exits; }

/// Selects the actors for the scene and, in Sol Sanctum, the room.
const struct ActorPlacement *Scene_GetActorPlacements(void) {
  struct ActorPlacement *table;
  s32 firstStatueRoomEntrance =
      SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_FROM_CHEST_ROOM;

  if (gMainRoutineWork.next == SCENE_SOL_SANCTUM_RELIEFS) {
    if (gMainRoutineWork.entrance >= firstStatueRoomEntrance) {
      if (gMainRoutineWork.entrance >
          SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_FROM_STEPS_ROOM) {
        if (gMainRoutineWork.entrance <=
            SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS) {
          return gSolSanctumReliefs_SecondReliefRoomActorPlacements;
        }
      } else {
        return gSolSanctumReliefs_FirstReliefRoomActorPlacements;
      }
    }
    table = gSolSanctumReliefs_ActorPlacements;
    Scene_AssignViewFlags(table);
    return table;
  }
  if (gMainRoutineWork.next == SCENE_SOL_ENTRANCE) {
    return gSolEntrance_ActorPlacements;
  }
  return gSolSanctumEntrance_FallbackActorPlacements;
}

/// Selects the events for the scene and room, by the same entrances as
/// Scene_GetActorPlacements.
const struct Trigger *Scene_GetTriggers(void) {
  s32 firstStatueRoomEntrance =
      SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_FROM_CHEST_ROOM;

  if (gMainRoutineWork.next == SCENE_SOL_ENTRANCE) {
    return gSolEntrance_Triggers;
  }
  if (gMainRoutineWork.next == SCENE_SOL_SANCTUM_RELIEFS) {
    if (gMainRoutineWork.entrance >= firstStatueRoomEntrance) {
      if (gMainRoutineWork.entrance >
          SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_FROM_STEPS_ROOM) {
        if (gMainRoutineWork.entrance <=
            SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS) {
          return gSolSanctumReliefs_SecondReliefRoomTriggers;
        }
      } else {
        return gSolSanctumReliefs_FirstReliefRoomTriggers;
      }
    }
    return gSolSanctumReliefs_Triggers;
  }
  return gSolSanctumEntrance_FallbackTriggers;
}

/// Sol Entrance: walking north into the sanctum's door slides it open, and the
/// leader walks in.
static void SolSanctumEntrance_OpenDoorAndEnter(void) {
  Event_Start();
  // The door opens over three frames.
  Sound_Play(SE_BOOM);
  MapGrid_CopyCellsTo(16, 28, 21, 3, 3, 2);
  wait(10);
  MapGrid_CopyCellsTo(16, 30, 21, 3, 3, 2);
  wait(10);
  MapGrid_CopyCellsTo(16, 32, 21, 3, 3, 2);
  wait(10);
  Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 2);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.6);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 120, 98);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -8);
  Event_Wait(10);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(SOL_ENTRANCE_TO_SOL_SANCTUM_RELIEFS);
  Event_End();
}

/// Sol Sanctum: searching the first minotaur relief shows one eye sparkling, or
/// both once the Small Jewel is set.
static void SolSanctumEntrance_InspectFirstMinotaurRelief(void) {
  Event_Start();

  if (GameFlag_Get(FLAG_SOL_SANCTUM_FIRST_SMALL_JEWEL_SET) != 0) {
    FieldMessage_Show(MsgSolSanctumMinotaurReliefBothEyes, 1);
  } else {
    FieldMessage_Show(MsgSolSanctumMinotaurReliefOneEye, 1);
    if (GameFlag_Get(FLAG_SOL_SANCTUM_FIRST_SMALL_JEWEL_FOUND) != 0) {
      gFieldWork->fieldMenu = 1;
    }
  }

  Event_End();
}

/// The room shakes as stone shifts, and the leader looks around and jumps.
static inline void SolSanctumEntrance_ReactToOpeningPassage(void) {
  MapGrid_SetShake(1, 1, 1);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  MapGrid_SetShake(2, 2, 1);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 40);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 10);
  Actor_Jump(ACTOR_PARTY_LEADER, 4, 20);
  Actor_Jump(ACTOR_PARTY_LEADER, 6, 40);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  Event_Wait(40);
}

/// Sol Sanctum: using the Small Jewel on the first relief sets it in, and a
/// passage opens.
static void SolSanctumEntrance_SetFirstSmallJewel(void) {
  s32 message;

  if (GameFlag_Get(FLAG_SOL_SANCTUM_FIRST_SMALL_JEWEL_FOUND) != 0) {
    if (Event_GetFlag(FLAG_SOL_SANCTUM_FIRST_SMALL_JEWEL_SET) == 0) {
      Event_Start();
      Field_EndBusyAndReveal();
      // The eye sparkles, then the wall opens.
      Sound_Play(SE_GEM_SET);
      MapGrid_CopyCellsTo(0, 70, 30, 42, 1, 1);
      MapGrid_CenterOnFollowedPoint();
      Event_Wait(40);
      message = MsgSolSanctumSetSmallGem;
      FieldMessage_Show(message, 1);
      Event_Wait(20);
      Sound_Play(SE_STONE_RUMBLE);
      MapGrid_CopyCellsTo(0, 29, 3, 1, 3, 2);
      MapGrid_CopyCellAttributes(0, 29, 3, 2, 3, 1);
      MapGrid_CopyCellsTo(1, 109, 4, 81, 1, 1);
      MapGrid_CenterOnFollowedPoint();
      SolSanctumEntrance_ReactToOpeningPassage();
      FieldMessage_Show(message + 1, 1);
      GameFlag_Set(FLAG_ITEM_USED_UP);
      GameFlag_Set(FLAG_SOL_SANCTUM_FIRST_SMALL_JEWEL_SET);
      Event_End();
    }
  }
}

/// Sol Sanctum, the second relief room: searching the second minotaur relief,
/// as the first.
static void SolSanctumEntrance_InspectSecondMinotaurRelief(void) {
  struct FieldWork *work;

  Event_Start();

  if (GameFlag_Get(FLAG_SOL_SANCTUM_SECOND_SMALL_JEWEL_SET) != 0) {
    FieldMessage_Show(MsgSolSanctumMinotaurReliefBothEyes, 1);
  } else if (GameFlag_Get(FLAG_SOL_SANCTUM_SECOND_SMALL_JEWEL_FOUND) != 0) {
    work = gFieldWork;
    FieldMessage_Show(MsgSolSanctumMinotaurReliefOneEye, 1);
    work->fieldMenu = 1;
  } else {
    FieldMessage_Show(MsgSolSanctumMinotaurReliefOneEye, 1);
  }

  Event_End();
}

/// Sol Sanctum, the second relief room: using the Small Jewel on the second
/// relief, as the first, and a doorway opens.
static void SolSanctumEntrance_SetSecondSmallJewel(void) {
  s32 message;

  if (GameFlag_Get(FLAG_SOL_SANCTUM_SECOND_SMALL_JEWEL_FOUND) != 0 &&
      Event_GetFlag(FLAG_SOL_SANCTUM_SECOND_SMALL_JEWEL_SET) == 0) {
    Event_Start();
    Field_EndBusyAndReveal();
    // The eye sparkles, then the doorway opens.
    Sound_Play(SE_GEM_SET);
    MapGrid_CopyCellsTo(0, 71, 100, 71, 1, 1);
    MapGrid_CenterOnFollowedPoint();
    Event_Wait(40);
    message = MsgSolSanctumSetSmallGem;
    FieldMessage_Show(message, 1);
    Event_Wait(20);
    Sound_Play(SE_STONE_RUMBLE);
    MapGrid_CopyCellsTo(122, 20, 120, 30, 1, 2);
    MapGrid_CopyCellAttributes(122, 20, 1, 2, 120, 30);
    MapGrid_CenterOnFollowedPoint();
    SolSanctumEntrance_ReactToOpeningPassage();
    FieldMessage_Show(message + 1, 1);
    GameFlag_Set(FLAG_ITEM_USED_UP);
    GameFlag_Set(FLAG_SOL_SANCTUM_SECOND_SMALL_JEWEL_SET);
    Event_End();
  }
}

/// Remembers which of a statue's two columns beside its start it stands in.
static inline void SolSanctumEntrance_RecordStatueColumn(s32 column,
                                                         s32 flagBase,
                                                         s32 leftColumn,
                                                         s32 rightColumn) {
  GameFlag_Clear(flagBase);
  GameFlag_Clear(flagBase + 1);
  if (column == leftColumn) {
    GameFlag_Set(flagBase + 1);
  } else if (column == rightColumn) {
    GameFlag_Set(flagBase);
  }
}

/// Sol Sanctum, the first relief room: Move on the west statue
/// (ACTOR_WEST_STATUE) while FLAG_SOL_SANCTUM_IN_SECOND_RELIEF_ROOM is clear,
/// and called by SolSanctumEntrance_PushStatue there.
static void SolSanctumEntrance_WestStatueMoved(void) {
  struct Actor *statue = Actor_Get(ACTOR_WEST_STATUE);
  s32 column;

  if (statue == NULL) {
    return;
  }
  column = FIELD_CELL(statue->x.fixed);
  SolSanctumEntrance_RecordStatueColumn(
      column, FLAG_SOL_SANCTUM_WEST_STATUE_AT_EAST, 93, 95);
}

/// Sol Sanctum, the first relief room: Move on the east statue
/// (ACTOR_EAST_STATUE), as the west one.
static void SolSanctumEntrance_EastStatueMoved(void) {
  struct Actor *statue = Actor_Get(ACTOR_EAST_STATUE);
  s32 column;

  if (statue == NULL) {
    return;
  }
  column = FIELD_CELL(statue->x.fixed);
  Event_ClearFlag(FLAG_SOL_SANCTUM_EAST_STATUE_AT_EAST);
  Event_ClearFlag(FLAG_SOL_SANCTUM_EAST_STATUE_AT_WEST);
  if (column == 115) {
    GameFlag_Set(FLAG_SOL_SANCTUM_EAST_STATUE_AT_EAST);
  } else if (column == 113) {
    GameFlag_Set(FLAG_SOL_SANCTUM_EAST_STATUE_AT_WEST);
  }
}

/// Sol Sanctum, the second relief room: Move on the first statue of the row
/// (ACTOR_ROW_STATUE_1), and called by SolSanctumEntrance_PushStatue there.
static void SolSanctumEntrance_RowStatue1Moved(void) {
  struct Actor *statue = Actor_Get(ACTOR_ROW_STATUE_1);
  s32 column;

  if (statue == NULL) {
    return;
  }
  column = FIELD_CELL(statue->x.fixed);
  SolSanctumEntrance_RecordStatueColumn(
      column, FLAG_SOL_SANCTUM_ROW_STATUE_1_AT_EAST, 99, 101);
  SolSanctumEntrance_RestoreRowStatueCells(0);
}

/// Sol Sanctum, the second relief room: as SolSanctumEntrance_RowStatue1Moved,
/// for the row's second statue (ACTOR_ROW_STATUE_2).
static void SolSanctumEntrance_RowStatue2Moved(void) {
  struct Actor *statue = Actor_Get(ACTOR_ROW_STATUE_2);
  s32 column;

  if (statue == NULL) {
    return;
  }
  column = FIELD_CELL(statue->x.fixed);
  SolSanctumEntrance_RecordStatueColumn(
      column, FLAG_SOL_SANCTUM_ROW_STATUE_2_AT_EAST, 103, 105);
  SolSanctumEntrance_RestoreRowStatueCells(0);
}

/// Sol Sanctum, the second relief room: as SolSanctumEntrance_RowStatue1Moved,
/// for the row's third statue (ACTOR_ROW_STATUE_3).
static void SolSanctumEntrance_RowStatue3Moved(void) {
  struct Actor *statue = Actor_Get(ACTOR_ROW_STATUE_3);
  s32 column;

  if (statue == NULL) {
    return;
  }
  column = FIELD_CELL(statue->x.fixed);
  SolSanctumEntrance_RecordStatueColumn(
      column, FLAG_SOL_SANCTUM_ROW_STATUE_3_AT_EAST, 107, 109);
  SolSanctumEntrance_RestoreRowStatueCells(0);
}

/// Sol Sanctum, the second relief room: as SolSanctumEntrance_RowStatue1Moved,
/// for the row's fourth statue (ACTOR_ROW_STATUE_4).
static void SolSanctumEntrance_RowStatue4Moved(void) {
  struct Actor *statue = Actor_Get(ACTOR_ROW_STATUE_4);
  s32 column;

  if (statue == NULL) {
    return;
  }
  column = FIELD_CELL(statue->x.fixed);
  SolSanctumEntrance_RecordStatueColumn(
      column, FLAG_SOL_SANCTUM_ROW_STATUE_4_AT_EAST, 111, 113);
  SolSanctumEntrance_RestoreRowStatueCells(0);
}

/// Sol Sanctum, the second relief room: as SolSanctumEntrance_RowStatue1Moved,
/// for the row's fifth statue (ACTOR_ROW_STATUE_5).
static void SolSanctumEntrance_RowStatue5Moved(void) {
  struct Actor *statue = Actor_Get(ACTOR_ROW_STATUE_5);
  s32 column;

  if (statue == NULL) {
    return;
  }
  column = FIELD_CELL(statue->x.fixed);
  SolSanctumEntrance_RecordStatueColumn(
      column, FLAG_SOL_SANCTUM_ROW_STATUE_5_AT_EAST, 115, 117);
  SolSanctumEntrance_RestoreRowStatueCells(0);
}

/// Sol Sanctum, the second relief room: as SolSanctumEntrance_RowStatue1Moved,
/// for the row's sixth statue (ACTOR_ROW_STATUE_6).
static void SolSanctumEntrance_RowStatue6Moved(void) {
  struct Actor *statue = Actor_Get(ACTOR_ROW_STATUE_6);
  s32 column;

  if (statue == NULL)
    return;
  column = FIELD_CELL(statue->x.fixed);
  SolSanctumEntrance_RecordStatueColumn(
      column, FLAG_SOL_SANCTUM_ROW_STATUE_6_AT_EAST, 119, 121);
  SolSanctumEntrance_RestoreRowStatueCells(0);
}

#include "shared/actor_find_at_cell.c"

/// The party has entered the second relief room, which turns off Move on the
/// first relief room's statues.
#define FLAG_SOL_SANCTUM_IN_SECOND_RELIEF_ROOM FLAG_SCENE_TEMP_34

/// Sol Sanctum, the two relief rooms: walking into a statue's cell pushes it
/// one cell, the leader behind it, if nothing is in the way.
static void SolSanctumEntrance_PushStatue(void) {
  struct Actor *leader;
  struct Actor *statue;
  u32 direction;
  struct FieldPosition target;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  direction = leader->facing >> 12;
  statue = Actor_FindAtCell(
      (leader->x.part.pixel + (gSolSanctumPushSteps[direction] >> 16)) >> 4,
      (leader->z.part.pixel + (s16)gSolSanctumPushSteps[direction]) >> 4);
  if (statue != NULL) {
    statue->layer = 2;
    Actor_StepFrom(&target, statue, gSolSanctumPushSteps[direction]);
    // Move only after the statue's proposed position passes collision.
    if (FieldActor_CheckMovementCollision(statue, &target) <= 0) {
      Actor_SelectAnimation(leader, ANIM_PUSH);
      wait(15);
      Sound_Play(SE_PUSH);
      statue->speedLimit = px(1) / 5;
      statue->acceleration = px(1) / 5;
      FieldActor_SetMoveTargetFixed(statue, target.x, target.y, target.z);
      leader->speedLimit = px(1) / 5;
      leader->acceleration = px(1) / 5;
      FieldActor_SetMoveTargetFixed(leader, target.x, target.y, target.z);
      Actor_WaitForMove(statue);
      statue->x.fixed = target.x;
      statue->z.fixed = target.z;
      statue->velocityX = 0;
      statue->velocityZ = 0;
      Actor_SelectAnimation(leader, ANIM_STAND);
      switch (gMainRoutineWork.entrance) {
      case SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_FROM_CHEST_ROOM:
      case SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_FROM_STATUE_NOOK:
      case SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_FROM_STEPS_ROOM:
        SolSanctumEntrance_WestStatueMoved();
        SolSanctumEntrance_EastStatueMoved();
        break;
      case SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_MAZE_EAST:
      case SOL_SANCTUM_RELIEFS_SECOND_RELIEF_ROOM_FROM_ROUND_PASSAGE:
      case SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS:
        SolSanctumEntrance_RowStatue1Moved();
        SolSanctumEntrance_RowStatue2Moved();
        SolSanctumEntrance_RowStatue3Moved();
        SolSanctumEntrance_RowStatue4Moved();
        SolSanctumEntrance_RowStatue5Moved();
        SolSanctumEntrance_RowStatue6Moved();
        break;
      }
    }
  }
}

/// Sets up Sol Entrance or Sol Sanctum.
s32 Scene_Initialize(void) {
  s32 sceneId = gMainRoutineWork.next;

  if (sceneId == SCENE_SOL_ENTRANCE) {
    SolSanctumEntrance_ApplyEntryState();
  } else if (sceneId == SCENE_SOL_SANCTUM_RELIEFS) {
    SolSanctumEntrance_RestoreInteriorState();
  }
  return 0;
}

/// Sets up Sol Entrance (SceneId_SolEntrance).
static void SolSanctumEntrance_ApplyEntryState(void) {
  // Retreat does not work here.
  GameFlag_Set(FLAG_RETREAT_BLOCKED);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  if (GameFlag_Get(FLAG_SOL_SANCTUM_ERUPTED) != 0) {
    gSolSanctumEntrance_RumbleTimer = 0;
    Task_AddFrame(SolSanctumEntrance_UpdateEruptionRumbles, TASK_PASS_FRAME);
  }
  if (GameFlag_Get(FLAG_VALE_RETURNED_FROM_MT_ALEPH) != 0) {
    MapGrid_CopyCellAttributes(5, 6, 1, 1, 6, 6);
    MapGrid_CopyCellAttributes(5, 6, 1, 1, 7, 6);
    MapGrid_CopyCellAttributes(5, 6, 1, 1, 8, 6);
    MapGrid_CopyCellAttributes(0, 1, 3, 1, 6, 5);
  }
  if (GameFlag_Get(FLAG_VALE_PARTY_LEFT) != 0) {
    Actor_SetPosition(ACTOR_WARNING_MAN, 120, 232);
    MapGrid_CopyCellAttributes(2, 10, 1, 1, 6, 14);
    MapGrid_CopyCellAttributes(2, 10, 1, 1, 7, 14);
    MapGrid_CopyCellAttributes(2, 10, 1, 1, 8, 14);
  }
}

/// Sets up Sol Sanctum (SceneId_SolSanctumReliefs).
static void SolSanctumEntrance_RestoreInteriorState(void) {
  s32 entrance;
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  if (GameFlag_Get(FLAG_SOL_SANCTUM_ERUPTED) != 0) {
    Scene_QueueMusic(SE_RUMBLE);
    MapGrid_SetShake(1, 1, 1);
    FieldEffect_InitializeSparkles();
  }

  entrance = gMainRoutineWork.entrance;
  switch (entrance) {
  case SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_CORRIDOR_WEST:
  case SOL_SANCTUM_RELIEFS_CHEST_ROOM_FROM_FIRST_RELIEF_ROOM:
    // With the first small jewel set, an arched doorway stands open in the
    // north wall of this room, between its two lit sconces.
    if (GameFlag_Get(FLAG_SOL_SANCTUM_FIRST_SMALL_JEWEL_SET) != 0) {
      MapGrid_CopyCellsTo(1, 109, 4, 81, 1, 1);
      MapGrid_CopyCellsTo(0, 70, 30, 42, 1, 1);
      MapGrid_CopyCellsTo(0, 29, 3, 1, 3, 2);
      MapGrid_CopyCellAttributes(0, 29, 3, 2, 3, 1);
      MapGrid_CenterOnFollowedPoint();
    }
    break;

  case SOL_SANCTUM_RELIEFS_STATUE_NOOK_FROM_FIRST_RELIEF_ROOM:
    Actor_SetSpriteFlags(Actor_Get(ACTOR_STATUE), 0);
    break;

  case SOL_SANCTUM_RELIEFS_FROM_SOL_ENTRANCE:
    gMainRoutineWork.retreatScene = SCENE_SOL_SANCTUM_RELIEFS;
    gMainRoutineWork.retreatEntrance = SOL_SANCTUM_RELIEFS_FROM_SOL_ENTRANCE;
    if (GameFlag_Get(FLAG_SOL_SANCTUM_ENTERED) == 0)
      SolSanctumEntrance_IntroduceSanctum();
    break;

  case SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_FROM_CHEST_ROOM:
  case SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_FROM_STATUE_NOOK:
  case SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_FROM_STEPS_ROOM:
    Actor_SetSpriteFlags(Actor_Get(ACTOR_WEST_STATUE), 0);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_EAST_STATUE), 0);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_FIRST_FIXED_STATUE), 0);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_FIRST_FIXED_STATUE + 1), 0);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_FIRST_FIXED_STATUE + 2), 0);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_FIRST_FIXED_STATUE + 3), 0);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_FIRST_FIXED_STATUE + 4), 0);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_FIRST_FIXED_STATUE + 5), 0);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_FIRST_FIXED_STATUE + 6), 0);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_FIRST_FIXED_STATUE + 7), 0);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_FIRST_FIXED_STATUE + 8), 0);
    if (GameFlag_Get(FLAG_SEARCHING_FOR_HIDDEN_PASSAGE) == 0)
      SolSanctumEntrance_KradenSuspectsHiddenPassage();
    if (GameFlag_Get(FLAG_SOL_SANCTUM_WEST_STATUE_AT_WEST) != 0)
      Actor_SetPosition(ACTOR_WEST_STATUE, 1496, 136);
    else if (GameFlag_Get(FLAG_SOL_SANCTUM_WEST_STATUE_AT_EAST) != 0)
      Actor_SetPosition(ACTOR_WEST_STATUE, 1528, 136);
    if (GameFlag_Get(FLAG_SOL_SANCTUM_EAST_STATUE_AT_WEST) != 0)
      Actor_SetPosition(ACTOR_EAST_STATUE, 1816, 136);
    else if (GameFlag_Get(FLAG_SOL_SANCTUM_EAST_STATUE_AT_EAST) != 0)
      Actor_SetPosition(ACTOR_EAST_STATUE, 1848, 136);
    break;

  case SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_MAZE_EAST:
  case SOL_SANCTUM_RELIEFS_SECOND_RELIEF_ROOM_FROM_ROUND_PASSAGE:
  case SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS:
    Actor_SetSpriteFlags(Actor_Get(ACTOR_ROW_STATUE_1), 0);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_ROW_STATUE_2), 0);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_ROW_STATUE_3), 0);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_ROW_STATUE_4), 0);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_ROW_STATUE_5), 0);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_ROW_STATUE_6), 0);
    if (GameFlag_Get(FLAG_SOL_SANCTUM_MORE_STATUES_SEEN) == 0)
      SolSanctumEntrance_KradenSeesStatuesOutOfReach();
    SolSanctumEntrance_RestoreRowStatueCells(1);
    GameFlag_Set(FLAG_SOL_SANCTUM_IN_SECOND_RELIEF_ROOM);
    if (GameFlag_Get(FLAG_SOL_SANCTUM_SECOND_SMALL_JEWEL_SET) != 0) {
      MapGrid_CopyCellsTo(0, 71, 100, 71, 1, 1);
      MapGrid_CopyCellsTo(122, 20, 120, 30, 1, 2);
      MapGrid_CopyCellAttributes(122, 20, 1, 2, 120, 30);
      MapGrid_CenterOnFollowedPoint();
    }
    break;
  }
}

/// Sol Sanctum, the first time the party comes in through the door: Kraden
/// welcomes it.
static void SolSanctumEntrance_IntroduceSanctum(void) {
  struct Actor *leader;

  Event_Start();
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Event_StartScreenTransition();
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STILL);
  Event_Wait(4);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  FieldCamera_SetSpeed(0.6);
  FieldCamera_MoveTo(1224, CAMERA_KEEP, 136, 1);
  Scene_PlaceAtLeader(ACTOR_KRADEN);
  Scene_PlaceAtLeader(ACTOR_JENNA);
  Scene_PlaceAtLeader(ACTOR_GARET);
  Actor_SetSpeed(ACTOR_KRADEN, 0.6);
  Actor_SetSpeed(ACTOR_JENNA, 0.6);
  Actor_SetSpeed(ACTOR_GARET, 0.6);
  Actor_SetAnimation(ACTOR_GARET, ANIM_WALK);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_WALK);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_WALK);
  Actor_SetDestinationOffset(ACTOR_GARET, -16, 0);
  Actor_SetDestinationOffset(ACTOR_JENNA, 16, 0);
  Actor_SetDestinationOffset(ACTOR_KRADEN, 0, -32);
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_SetAnimation(ACTOR_GARET, ANIM_STILL);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_STILL);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  Actor_WaitForMoveById(ACTOR_KRADEN);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_STAND);
  Event_Wait(40);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 40);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 40);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 20);
  Actor_Jump(ACTOR_KRADEN, 4, 20);
  // Kraden: "This is your first time here, right?" Yes: "Take a good look at
  // it." No: "I doubt that..."
  Event_SetMessageId(MsgSolSanctumKradenFirstTimeAtSol);
  // "This is the entrance to Sol Sanctum. This is your first time..."
  // Yes: "It's an important relic of our past. Take a good look at it."
  // No: "I doubt that... Few ever get the chance to see this sanctum..."
  Event_AskYesNo(SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 0);
  Event_Wait(20);
  FieldCamera_MoveTo(1224, CAMERA_KEEP, 148, 1);
  Actor_SetAnimation(ACTOR_GARET, ANIM_WALK);
  Scene_WalkTowardLeader(ACTOR_GARET);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_WALK);
  Scene_WalkTowardLeader(ACTOR_JENNA);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_WALK);
  Scene_WalkTowardLeader(ACTOR_KRADEN);
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_SetPosition(ACTOR_GARET, 0, 0);
  Actor_SetPosition(ACTOR_JENNA, 0, 0);
  Actor_WaitForMoveById(ACTOR_KRADEN);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  Actor_SetAnimation(ACTOR_GARET, ANIM_STAND);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_STAND);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_STAND);
  GameFlag_Set(FLAG_SOL_SANCTUM_ENTERED);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  GameFlag_Clear(FLAG_SHOW_AREA_NAME);
  Event_End();
}

/// Sol Sanctum, the first time the party enters the first relief room: Kraden
/// suspects a hidden passage.
static void SolSanctumEntrance_KradenSuspectsHiddenPassage(void) {
  struct Actor *leader;

  Event_Start();
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL)
    Actor_SetPositionFixed(ACTOR_KRADEN, leader->x.fixed, leader->z.fixed);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL)
    Actor_SetPositionFixed(ACTOR_JENNA, leader->x.fixed, leader->z.fixed);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL)
    Actor_SetPositionFixed(ACTOR_GARET, leader->x.fixed, leader->z.fixed);

  Actor_SetSpeed(ACTOR_KRADEN, 0.6);
  Actor_SetSpeed(ACTOR_JENNA, 0.6);
  Actor_SetSpeed(ACTOR_GARET, 0.6);
  Actor_SetAnimation(ACTOR_GARET, ANIM_WALK);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_WALK);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_WALK);
  Actor_SetDestinationOffset(ACTOR_GARET, -16, 0);
  Actor_SetDestinationOffset(ACTOR_JENNA, 16, 0);
  Actor_SetDestinationOffset(ACTOR_KRADEN, 0, -16);
  Actor_WaitForMoveById(ACTOR_KRADEN);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_STAND);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STILL);
  Actor_SetAnimation(ACTOR_GARET, ANIM_STILL);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_STILL);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 30);
  Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 30);
  Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH, 30);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 40);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  Event_Wait(10);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_WALK);
  Actor_SetDestinationOffset(ACTOR_KRADEN, 0, -16);
  Actor_WaitForMoveById(ACTOR_KRADEN);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_STAND);
  Event_Wait(6);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 40);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  Event_Wait(20);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_WALK);
  Actor_SetDestinationOffset(ACTOR_KRADEN, 0, -32);
  Actor_WaitForMoveById(ACTOR_KRADEN);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_STAND);

  // Survey both sides of the room before returning to Kraden.
  FieldCamera_SetSpeed(2);
  FieldCamera_MoveTo(1585, CAMERA_KEEP, 150, 1);
  FieldCamera_WaitForMove();
  Event_Wait(10);
  FieldCamera_SetSpeed(1.2);
  FieldCamera_MoveTo(1621, CAMERA_KEEP, 100, 1);
  FieldCamera_WaitForMove();
  FieldCamera_MoveTo(1718, CAMERA_KEEP, 100, 1);
  FieldCamera_WaitForMove();
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_STAND);
  FieldCamera_MoveTo(1752, CAMERA_KEEP, 150, 1);
  FieldCamera_WaitForMove();
  Event_Wait(40);
  FieldCamera_SetSpeed(2.4);
  FieldCamera_MoveTo(1668, CAMERA_KEEP, 256, 1);
  FieldCamera_WaitForMove();
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Event_Wait(10);

  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 10);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  Event_SetMessageId(MsgSolSanctumWrongKraden);
  // "What's wrong, Kraden?"
  Event_ShowMessageThenWait(ACTOR_GARET, 0, 10);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Actor_StartRepeatedMotion(ACTOR_KRADEN, 2);
  // Kraden: "Those two said there was a hidden passage leading deeper into
  // the sanctum."
  Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  Actor_StartRepeatedMotion(ACTOR_JENNA, 2);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_JENNA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  Event_Wait(20);
  // Kraden: the sanctum always looked strange to him, not as he imagined.
  // "I've come here many times, and I've always thought it..."
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  // "I had always imagined Sol Sanctum would look different, somehow."
  Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  Actor_FaceEachOther(ACTOR_PARTY_LEADER, 5, 0);
  Event_Wait(40);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 1);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 20);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  // Jenna: "What do you mean?"
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  // Kraden: a sanctum built in honour of the sun ought to show it somewhere.
  // "Sol Sanctum was built in honor of the sun. There ought to be..."
  Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 10);
  // "I was always puzzled why there wasn't."
  Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 40);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 20);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 1);
  Event_Wait(10);
  Actor_SetAttachedEffect(ACTOR_KRADEN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 60);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  Event_Wait(10);
  // Kraden: "If there is a hidden passage, then maybe it leads to..."
  Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH, 20);
  Actor_Jump(ACTOR_KRADEN, 2, 20);

  // Kraden hops: "Let's see if we can find it!"
  Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 40);
  Actor_FaceDirection(ACTOR_GARET, FACING_EAST, 20);
  // Garet: "I think he's serious! Should we help him?"
  Event_OpenMessage(ACTOR_GARET, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
    // Yes: Garet says it is too dangerous to split up; they stay together.
    Event_SetMessageId(MsgSolSanctumDangerousSplitStay);
    Actor_RunRepeatedMotion(ACTOR_GARET, 1);
    // "But it's too dangerous for us to split up. We should stay..."
    Event_ShowMessageThenWait(ACTOR_GARET, 0, 10);
  } else {
    // No: Jenna objects to turning back now; Isaac nods and Garet agrees to
    // help Kraden.
    Event_SetMessageId(MsgSolSanctumCantSeriouslyWant);
    Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 20);
    // "You can't seriously want to turn back after we've come this far."
    Event_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
    Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 60);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
    Actor_RunRepeatedMotion(ACTOR_GARET, 1);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_GARET, FACING_EAST, 0);
    Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 30);
    Actor_RunRepeatedMotion(ACTOR_GARET, 1);
    Event_Wait(10);
    // "Let's help Kraden. We should stick together, after all."
    Event_ShowMessageThenWait(ACTOR_GARET, 0, 10);
  }

  Actor_SetSpeed(ACTOR_KRADEN, 0.6);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_WALK);
  Actor_SetDestinationOffset(ACTOR_KRADEN, 0, 48);
  Actor_WaitForMoveById(ACTOR_KRADEN);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_STAND);
  Event_Wait(6);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
  Actor_WaitForMoveById(ACTOR_KRADEN);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_STAND);
  Event_Wait(20);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(6);
  Actor_SetAnimation(ACTOR_GARET, ANIM_WALK);

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL)
    Actor_SetDestination(ACTOR_GARET, leader->x.part.pixel,
                         leader->z.part.pixel);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_WALK);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL)
    Actor_SetDestination(ACTOR_JENNA, leader->x.part.pixel,
                         leader->z.part.pixel);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_WALK);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL)
    Actor_SetDestination(ACTOR_KRADEN, leader->x.part.pixel,
                         leader->z.part.pixel);

  Actor_WaitForMoveById(ACTOR_KRADEN);
  Actor_SetPosition(ACTOR_GARET, 0, 0);
  Actor_SetPosition(ACTOR_JENNA, 0, 0);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_STAND);
  Actor_SetAnimation(ACTOR_GARET, ANIM_STAND);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_STAND);
  GameFlag_Set(FLAG_SEARCHING_FOR_HIDDEN_PASSAGE);
  GameFlag_Clear(FLAG_SHOW_AREA_NAME);
  Event_End();
}

/// Sol Sanctum, the first time the party enters the second relief room: Kraden
/// sees a row of statues out of reach.
static void SolSanctumEntrance_KradenSeesStatuesOutOfReach(void) {
  Event_Start();
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(20);
  Scene_PlaceAtLeader(ACTOR_KRADEN);
  Actor_SetSpeed(ACTOR_KRADEN, 1);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_WALK);
  Actor_SetDestinationOffset(ACTOR_KRADEN, 24, -10);
  Actor_WaitForMoveById(ACTOR_KRADEN);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_STAND);
  Event_Wait(6);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 40);
  FieldCamera_SetSpeed(2.4);
  FieldCamera_MoveTo(1672, CAMERA_KEEP, 524, 1);
  FieldCamera_WaitForMove();
  Event_Wait(20);
  FieldCamera_SetSpeed(1.6);
  FieldCamera_MoveTo(1880, CAMERA_KEEP, 524, 1);
  FieldCamera_WaitForMove();
  Event_Wait(20);
  FieldCamera_SetSpeed(3.2);
  FieldCamera_MoveTo(1769, CAMERA_KEEP, 548, 1);
  FieldCamera_WaitForMove();
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 30);
  // Kraden: "More statues... It doesn't look like we can reach them".
  Event_SetMessageId(MsgSolSanctumMoreStatuesOutOfReach);
  // "More statues... Hmm... It doesn't look like we can reach..."
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 0, 10);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 1);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 20);
  // Kraden: "Perhaps Psynergy is the key..."
  Event_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 0, 10);
  Scene_WalkIntoLeader(ACTOR_KRADEN);
  GameFlag_Set(FLAG_SOL_SANCTUM_MORE_STATUES_SEEN);
  Event_End();
}

/// Sol Sanctum, the second relief room: sets the cells the six statues start
/// on.
static void SolSanctumEntrance_RestoreRowStatueCells(s32 placeActors) {
  MapGrid_CopyCellAttributes(122, 20, 1, 1, 100, 32);
  MapGrid_CopyCellAttributes(122, 20, 1, 1, 104, 32);
  MapGrid_CopyCellAttributes(122, 20, 1, 1, 108, 32);
  MapGrid_CopyCellAttributes(122, 20, 1, 1, 112, 32);
  MapGrid_CopyCellAttributes(122, 20, 1, 1, 116, 32);
  MapGrid_CopyCellAttributes(122, 20, 1, 1, 120, 32);
  if (GameFlag_Get(FLAG_SOL_SANCTUM_ROW_STATUE_1_AT_WEST) != 0) {
    MapGrid_CopyCellAttributes(121, 20, 1, 1, 100, 32);
    if (placeActors != 0) {
      Actor_SetPosition(ACTOR_ROW_STATUE_1, 1592, 520);
    }
  } else {
    if (GameFlag_Get(FLAG_SOL_SANCTUM_ROW_STATUE_1_AT_EAST) != 0) {
      MapGrid_CopyCellAttributes(121, 20, 1, 1, 100, 32);
      if (placeActors != 0) {
        Actor_SetPosition(ACTOR_ROW_STATUE_1, 1624, 520);
      }
    }
  }
  if (GameFlag_Get(FLAG_SOL_SANCTUM_ROW_STATUE_2_AT_WEST) != 0) {
    MapGrid_CopyCellAttributes(121, 20, 1, 1, 104, 32);
    if (placeActors != 0) {
      Actor_SetPosition(ACTOR_ROW_STATUE_2, 1656, 520);
    }
  } else {
    if (GameFlag_Get(FLAG_SOL_SANCTUM_ROW_STATUE_2_AT_EAST) != 0) {
      MapGrid_CopyCellAttributes(121, 20, 1, 1, 104, 32);
      if (placeActors != 0) {
        Actor_SetPosition(ACTOR_ROW_STATUE_2, 1688, 520);
      }
    }
  }
  if (GameFlag_Get(FLAG_SOL_SANCTUM_ROW_STATUE_3_AT_WEST) != 0) {
    MapGrid_CopyCellAttributes(121, 20, 1, 1, 108, 32);
    if (placeActors != 0) {
      Actor_SetPosition(ACTOR_ROW_STATUE_3, 1720, 520);
    }
  } else {
    if (GameFlag_Get(FLAG_SOL_SANCTUM_ROW_STATUE_3_AT_EAST) != 0) {
      MapGrid_CopyCellAttributes(121, 20, 1, 1, 108, 32);
      if (placeActors != 0) {
        Actor_SetPosition(ACTOR_ROW_STATUE_3, 1752, 520);
      }
    }
  }
  if (GameFlag_Get(FLAG_SOL_SANCTUM_ROW_STATUE_4_AT_WEST) != 0) {
    MapGrid_CopyCellAttributes(121, 20, 1, 1, 112, 32);
    if (placeActors != 0) {
      Actor_SetPosition(ACTOR_ROW_STATUE_4, 1784, 520);
    }
  } else {
    if (GameFlag_Get(FLAG_SOL_SANCTUM_ROW_STATUE_4_AT_EAST) != 0) {
      MapGrid_CopyCellAttributes(121, 20, 1, 1, 112, 32);
      if (placeActors != 0) {
        Actor_SetPosition(ACTOR_ROW_STATUE_4, 1816, 520);
      }
    }
  }
  if (GameFlag_Get(FLAG_SOL_SANCTUM_ROW_STATUE_5_AT_WEST) != 0) {
    MapGrid_CopyCellAttributes(121, 20, 1, 1, 116, 32);
    if (placeActors != 0) {
      Actor_SetPosition(ACTOR_ROW_STATUE_5, 1848, 520);
    }
  } else {
    if (GameFlag_Get(FLAG_SOL_SANCTUM_ROW_STATUE_5_AT_EAST) != 0) {
      MapGrid_CopyCellAttributes(121, 20, 1, 1, 116, 32);
      if (placeActors != 0) {
        Actor_SetPosition(ACTOR_ROW_STATUE_5, 1880, 520);
      }
    }
  }
  if (GameFlag_Get(FLAG_SOL_SANCTUM_ROW_STATUE_6_AT_WEST) != 0) {
    MapGrid_CopyCellAttributes(121, 20, 1, 1, 120, 32);
    if (placeActors != 0) {
      Actor_SetPosition(ACTOR_ROW_STATUE_6, 1912, 520);
    }
  } else {
    if (GameFlag_Get(FLAG_SOL_SANCTUM_ROW_STATUE_6_AT_EAST) != 0) {
      MapGrid_CopyCellAttributes(121, 20, 1, 1, 120, 32);
      if (placeActors != 0) {
        Actor_SetPosition(ACTOR_ROW_STATUE_6, 1944, 520);
      }
    }
  }
}

/// Sol Entrance: searching the other door. "The door is tightly locked."
static void SolSanctumEntrance_DoorTightlyLocked(void) {
  Event_Start();
  FieldMessage_Show(MsgFieldDoorTightlyLocked, 1);
  Event_End();
}

/// Sol Entrance after the eruption: the ground rumbles now and then, and each
/// shake fades.
static void SolSanctumEntrance_UpdateEruptionRumbles(void) {
  s32 randomValue;
  s32 *timer;
  s32 remainingFrames;

  timer = &gSolSanctumEntrance_RumbleTimer;
  if (*timer != 0) {
    remainingFrames = (*timer - 1);
    *timer = (*timer - 1);
    if (remainingFrames == 40) {
      MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
    }
  } else {
    randomValue = Random_Next();
    if (((u32)(((randomValue << 4) - randomValue) << 3) >> 16) == 0) {
      Sound_Play(SE_ITEM_BREAK);
      MapGrid_SetShake(1, 2, 1);
      *timer = 80;
    }
  }
}

struct SceneEntrance gSolSanctumEntrance_FallbackEntrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gSolEntrance_Entrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {SOL_ENTRANCE_FROM_VALE, CONDITION_ALWAYS, 80, 0, 288, FACING_NORTH, 0, -1,
     -1, -1, -1, 0},
    {SOL_ENTRANCE_FROM_SOL_SANCTUM_RELIEFS, CONDITION_ALWAYS, 120, 0, 112,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gSolSanctumReliefs_Entrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_CORRIDOR_WEST, CONDITION_ALWAYS, 280,
     0, 472, FACING_NORTH, 0, 5, 0, 555, 510, 0},
    {SOL_SANCTUM_RELIEFS_CHEST_ROOM_FROM_FIRST_RELIEF_ROOM, CONDITION_ALWAYS,
     72, 0, 56, FACING_SOUTH, 0, 5, 0, 555, 510, 0},
    {SOL_SANCTUM_RELIEFS_STATUE_NOOK_FROM_FIRST_RELIEF_ROOM, CONDITION_ALWAYS,
     744, 0, 152, FACING_NORTH, 0, 620, 0, 860, 195, 0},
    {SOL_SANCTUM_RELIEFS_STEPPING_STONE_ROOM_FROM_ENTRANCE_ROOM,
     CONDITION_ALWAYS, 696, 0, 568, FACING_NORTH, 0, 575, 265, 815, 610, 0},
    {SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_CORRIDOR_EAST, CONDITION_ALWAYS, 696,
     0, 328, FACING_SOUTH, 0, 575, 265, 815, 610, 0},
    {SOL_SANCTUM_RELIEFS_STEPS_ROOM_FROM_FIRST_RELIEF_ROOM, CONDITION_ALWAYS,
     984, 0, 152, FACING_NORTH, 0, 860, 0, 1100, 195, 0},
    {SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_MAZE_WEST, CONDITION_ALWAYS, 984, 0,
     56, FACING_SOUTH, 0, 860, 0, 1100, 195, 0},
    {SOL_SANCTUM_RELIEFS_FROM_SOL_ENTRANCE, CONDITION_ALWAYS, 1224, 0, 184,
     FACING_NORTH, 0, 1100, 20, 1340, 220, 0},
    {SOL_SANCTUM_RELIEFS_ENTRANCE_ROOM_FROM_STEPPING_STONE_ROOM,
     CONDITION_ALWAYS, 1224, 0, 104, FACING_SOUTH, 0, 1100, 20, 1340, 220, 0},
    {SOL_SANCTUM_RELIEFS_ROUND_PASSAGE_FROM_SECOND_RELIEF_ROOM,
     CONDITION_ALWAYS, 1320, 0, 456, FACING_NORTH, 0, 905, 285, 1365, 490, 0},
    {SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_FROM_CHEST_ROOM, CONDITION_ALWAYS,
     1672, 0, 280, FACING_NORTH, 0, 1455, 0, 1890, 310, 0},
    {SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_FROM_STATUE_NOOK, CONDITION_ALWAYS,
     1512, 0, 136, FACING_SOUTH, 0, 1455, 0, 1890, 310, 0},
    {SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_FROM_STEPS_ROOM, CONDITION_ALWAYS,
     1832, 0, 136, FACING_SOUTH, 0, 1455, 0, 1890, 310, 0},
    {SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_MAZE_EAST, CONDITION_ALWAYS, 1768, 0,
     568, FACING_NORTH, 0, 1540, 440, 1995, 600, 0},
    {SOL_SANCTUM_RELIEFS_SECOND_RELIEF_ROOM_FROM_ROUND_PASSAGE,
     CONDITION_ALWAYS, 1672, 0, 520, FACING_SOUTH, 0, 1540, 440, 1995, 600, 0},
    {SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS, CONDITION_ALWAYS,
     1928, 0, 520, FACING_SOUTH, 0, 1540, 440, 1995, 600, 0},
    {SCENE_TABLE_END},
};

// Sol Entrance (SceneId_SolEntrance), then Sol Sanctum
// (SceneId_SolSanctumReliefs). Sol Entrance's way
// down leads to SceneId_Vale until the eruption, then to Vale during the
// eruption (SceneId_ValeEruption) until the party has left Vale.
u32 gSolSanctumEntrance_Exits[] = {
    SCENE_EXITS(SolEntrance),
    SCENE_EXIT_UNLESS(SOL_ENTRANCE_TO_VALE, Vale, VALE_FROM_SOL_ENTRANCE,
                      FLAG_SOL_SANCTUM_ERUPTED),
    SCENE_EXIT_UNLESS(SOL_ENTRANCE_TO_VALE, ValeEruption,
                      VALE_ERUPTION_FROM_SOL_ENTRANCE, FLAG_VALE_PARTY_LEFT),
    SCENE_EXIT_UNLESS(SOL_ENTRANCE_TO_VALE, Vale, VALE_FROM_SOL_ENTRANCE,
                      EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(SOL_ENTRANCE_TO_SOL_SANCTUM_RELIEFS, SolSanctumReliefs,
                      SOL_SANCTUM_RELIEFS_FROM_SOL_ENTRANCE, EXIT_FLAG_NONE),
    SCENE_EXITS(SolSanctumReliefs),
    SCENE_EXIT_UNLESS(
        SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_CORRIDOR_WEST, SolSanctumCorridor,
        SOL_SANCTUM_CORRIDOR_FROM_SOL_SANCTUM_RELIEFS_WEST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(
        SOL_SANCTUM_RELIEFS_CHEST_ROOM_TO_FIRST_RELIEF_ROOM, SolSanctumReliefs,
        SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_FROM_CHEST_ROOM, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(
        SOL_SANCTUM_RELIEFS_STATUE_NOOK_TO_FIRST_RELIEF_ROOM, SolSanctumReliefs,
        SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_FROM_STATUE_NOOK, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(
        SOL_SANCTUM_RELIEFS_STEPPING_STONE_ROOM_TO_ENTRANCE_ROOM,
        SolSanctumReliefs,
        SOL_SANCTUM_RELIEFS_ENTRANCE_ROOM_FROM_STEPPING_STONE_ROOM,
        EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(
        SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_CORRIDOR_EAST, SolSanctumCorridor,
        SOL_SANCTUM_CORRIDOR_FROM_SOL_SANCTUM_RELIEFS_EAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(
        SOL_SANCTUM_RELIEFS_STEPS_ROOM_TO_FIRST_RELIEF_ROOM, SolSanctumReliefs,
        SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_FROM_STEPS_ROOM, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(
        SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_MAZE_WEST, SolSanctumMaze,
        SOL_SANCTUM_MAZE_FROM_SOL_SANCTUM_RELIEFS_SOUTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(SOL_SANCTUM_RELIEFS_TO_SOL_ENTRANCE, SolEntrance,
                      SOL_ENTRANCE_FROM_SOL_SANCTUM_RELIEFS, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(
        SOL_SANCTUM_RELIEFS_ENTRANCE_ROOM_TO_STEPPING_STONE_ROOM,
        SolSanctumReliefs,
        SOL_SANCTUM_RELIEFS_STEPPING_STONE_ROOM_FROM_ENTRANCE_ROOM,
        EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(SOL_SANCTUM_RELIEFS_ROUND_PASSAGE_TO_SECOND_RELIEF_ROOM,
                      SolSanctumReliefs,
                      SOL_SANCTUM_RELIEFS_SECOND_RELIEF_ROOM_FROM_ROUND_PASSAGE,
                      EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(
        SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_TO_CHEST_ROOM, SolSanctumReliefs,
        SOL_SANCTUM_RELIEFS_CHEST_ROOM_FROM_FIRST_RELIEF_ROOM, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(
        SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_TO_STATUE_NOOK, SolSanctumReliefs,
        SOL_SANCTUM_RELIEFS_STATUE_NOOK_FROM_FIRST_RELIEF_ROOM, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(
        SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_TO_STEPS_ROOM, SolSanctumReliefs,
        SOL_SANCTUM_RELIEFS_STEPS_ROOM_FROM_FIRST_RELIEF_ROOM, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(
        SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_MAZE_EAST, SolSanctumMaze,
        SOL_SANCTUM_MAZE_FROM_SOL_SANCTUM_RELIEFS_NORTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(SOL_SANCTUM_RELIEFS_SECOND_RELIEF_ROOM_TO_ROUND_PASSAGE,
                      SolSanctumReliefs,
                      SOL_SANCTUM_RELIEFS_ROUND_PASSAGE_FROM_SECOND_RELIEF_ROOM,
                      EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_LUNA_SOL_ROOMS,
                      SolSanctumLunaSolRooms,
                      SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_RELIEFS,
                      EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

struct ActorPlacement gSolSanctumEntrance_FallbackActorPlacements[] = {
    {SCENE_TABLE_END},
};

// clang-format off
struct ActorPlacement gSolSanctumReliefs_ActorPlacements[] = {
    ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        744, 0, 56, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gSolSanctumReliefs_FirstReliefRoomActorPlacements[] = {
    ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        1512, 0, 136, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        1832, 0, 136, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        1560, 0, 136, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        1784, 0, 136, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        1640, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        1640, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        1640, 0, 136, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        1672, 0, 88, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        1704, 0, 136, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        1704, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        1704, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    { SCENE_TABLE_END },
};

struct ActorPlacement gSolSanctumReliefs_SecondReliefRoomActorPlacements[] = {
    ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        1608, 0, 520, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        1672, 0, 520, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        1736, 0, 520, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        1800, 0, 520, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        1864, 0, 520, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        1928, 0, 520, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    { SCENE_TABLE_END },
};

struct ActorPlacement gSolEntrance_ActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};

struct Trigger gSolSanctumEntrance_FallbackTriggers[] = {
    { TRIGGER_TABLE_END },
};

struct Trigger gSolEntrance_Triggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, SOL_ENTRANCE_TO_VALE },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_SOL_ENTRANCE_DOOR, CONDITION_ALWAYS, EVENT(SolSanctumEntrance_OpenDoorAndEnter) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_SOL_ENTRANCE_LOCKED_DOOR, CONDITION_ALWAYS, EVENT(SolSanctumEntrance_DoorTightlyLocked) },
    { TRIGGER_TALK, ACTOR_WARNING_MAN, CONDITION_ALWAYS, MSGNO(MsgValeItIsVeryDangerousToGo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WARNING_MAN, CONDITION_ALWAYS, MSGNO(MsgValeIAmAllAloneInThis) },
    { TRIGGER_TABLE_END },
};

struct Trigger gSolSanctumReliefs_Triggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_CORRIDOR_WEST },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_CHEST_ROOM_TO_FIRST_RELIEF_ROOM },
    { TRIGGER_EXIT, 3, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_STATUE_NOOK_TO_FIRST_RELIEF_ROOM },
    { TRIGGER_EXIT, 4, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_STEPPING_STONE_ROOM_TO_ENTRANCE_ROOM },
    { TRIGGER_EXIT, 5, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_CORRIDOR_EAST },
    { TRIGGER_EXIT, 6, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_STEPS_ROOM_TO_FIRST_RELIEF_ROOM },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 7, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_MAZE_WEST },
    { TRIGGER_EXIT, 8, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_ENTRANCE },
    { TRIGGER_EXIT, 9, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_ENTRANCE_ROOM_TO_STEPPING_STONE_ROOM },
    { TRIGGER_EXIT, 10, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_ROUND_PASSAGE_TO_SECOND_RELIEF_ROOM },
    { TRIGGER_EXIT, 11, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_TO_CHEST_ROOM },
    { TRIGGER_EXIT, 12, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_TO_STATUE_NOOK },
    { TRIGGER_EXIT, 13, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_TO_STEPS_ROOM },
    { TRIGGER_EXIT, 14, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_MAZE_EAST },
    { TRIGGER_EXIT, 15, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_SECOND_RELIEF_ROOM_TO_ROUND_PASSAGE },
    { TRIGGER_EXIT, 16, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_LUNA_SOL_ROOMS },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), CELL_BEHAVIOR_SOL_SANCTUM_RELIEFS_CHEST, FLAG_SOL_SANCTUM_FIRST_SMALL_JEWEL_FOUND, FIND_ITEM(ITEM_SMALL_JEWEL) },
    { USE_ITEM(ITEM_SMALL_JEWEL), SOL_SANCTUM_ITEM_OWN_MESSAGE | CELL_BEHAVIOR_SOL_SANCTUM_RELIEFS_FIRST_RELIEF, CONDITION_ALWAYS, EVENT(SolSanctumEntrance_SetFirstSmallJewel) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_SOL_SANCTUM_RELIEFS_FIRST_RELIEF, CONDITION_ALWAYS, EVENT(SolSanctumEntrance_InspectFirstMinotaurRelief) },
    { TRIGGER_TABLE_END },
};

struct Trigger gSolSanctumReliefs_FirstReliefRoomTriggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_CORRIDOR_WEST },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_CHEST_ROOM_TO_FIRST_RELIEF_ROOM },
    { TRIGGER_EXIT, 3, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_STATUE_NOOK_TO_FIRST_RELIEF_ROOM },
    { TRIGGER_EXIT, 4, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_STEPPING_STONE_ROOM_TO_ENTRANCE_ROOM },
    { TRIGGER_EXIT, 5, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_CORRIDOR_EAST },
    { TRIGGER_EXIT, 6, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_STEPS_ROOM_TO_FIRST_RELIEF_ROOM },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 7, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_MAZE_WEST },
    { TRIGGER_EXIT, 8, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_ENTRANCE },
    { TRIGGER_EXIT, 9, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_ENTRANCE_ROOM_TO_STEPPING_STONE_ROOM },
    { TRIGGER_EXIT, 10, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_ROUND_PASSAGE_TO_SECOND_RELIEF_ROOM },
    { TRIGGER_EXIT, 11, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_TO_CHEST_ROOM },
    { TRIGGER_EXIT, 12, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_TO_STATUE_NOOK },
    { TRIGGER_EXIT, 13, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_TO_STEPS_ROOM },
    { TRIGGER_EXIT, 14, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_MAZE_EAST },
    { TRIGGER_EXIT, 15, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_SECOND_RELIEF_ROOM_TO_ROUND_PASSAGE },
    { TRIGGER_EXIT, 16, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_LUNA_SOL_ROOMS },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_WEST_STATUE, FLAG_SOL_SANCTUM_IN_SECOND_RELIEF_ROOM, EVENT(SolSanctumEntrance_WestStatueMoved) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_EAST_STATUE, FLAG_SOL_SANCTUM_IN_SECOND_RELIEF_ROOM, EVENT(SolSanctumEntrance_EastStatueMoved) },
    { STEP_TRIGGER_WALKING, CELL_BEHAVIOR_SOL_SANCTUM_RELIEFS_FIRST_ROOM_STATUE, CONDITION_ALWAYS, EVENT(SolSanctumEntrance_PushStatue) },
    { STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_SOL_SANCTUM_RELIEFS_FIRST_ROOM_STATUE_EAST, CONDITION_ALWAYS, EVENT(SolSanctumEntrance_PushStatue) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_SOL_SANCTUM_RELIEFS_FIRST_ROOM_STATUE_WEST, CONDITION_ALWAYS, EVENT(SolSanctumEntrance_PushStatue) },
    { TRIGGER_TABLE_END },
};

struct Trigger gSolSanctumReliefs_SecondReliefRoomTriggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_CORRIDOR_WEST },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_CHEST_ROOM_TO_FIRST_RELIEF_ROOM },
    { TRIGGER_EXIT, 3, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_STATUE_NOOK_TO_FIRST_RELIEF_ROOM },
    { TRIGGER_EXIT, 4, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_STEPPING_STONE_ROOM_TO_ENTRANCE_ROOM },
    { TRIGGER_EXIT, 5, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_CORRIDOR_EAST },
    { TRIGGER_EXIT, 6, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_STEPS_ROOM_TO_FIRST_RELIEF_ROOM },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 7, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_MAZE_WEST },
    { TRIGGER_EXIT, 8, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_ENTRANCE },
    { TRIGGER_EXIT, 9, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_ENTRANCE_ROOM_TO_STEPPING_STONE_ROOM },
    { TRIGGER_EXIT, 10, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_ROUND_PASSAGE_TO_SECOND_RELIEF_ROOM },
    { TRIGGER_EXIT, 11, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_TO_CHEST_ROOM },
    { TRIGGER_EXIT, 12, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_TO_STATUE_NOOK },
    { TRIGGER_EXIT, 13, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_TO_STEPS_ROOM },
    { TRIGGER_EXIT, 14, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_MAZE_EAST },
    { TRIGGER_EXIT, 15, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_SECOND_RELIEF_ROOM_TO_ROUND_PASSAGE },
    { TRIGGER_EXIT, 16, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_LUNA_SOL_ROOMS },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ROW_STATUE_1, CONDITION_ALWAYS, EVENT(SolSanctumEntrance_RowStatue1Moved) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ROW_STATUE_2, CONDITION_ALWAYS, EVENT(SolSanctumEntrance_RowStatue2Moved) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ROW_STATUE_3, CONDITION_ALWAYS, EVENT(SolSanctumEntrance_RowStatue3Moved) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ROW_STATUE_4, CONDITION_ALWAYS, EVENT(SolSanctumEntrance_RowStatue4Moved) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ROW_STATUE_5, CONDITION_ALWAYS, EVENT(SolSanctumEntrance_RowStatue5Moved) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ROW_STATUE_6, CONDITION_ALWAYS, EVENT(SolSanctumEntrance_RowStatue6Moved) },
    { STEP_TRIGGER_WALKING, CELL_BEHAVIOR_SOL_SANCTUM_RELIEFS_ROW_STATUE, CONDITION_ALWAYS, EVENT(SolSanctumEntrance_PushStatue) },
    { USE_ITEM(ITEM_SMALL_JEWEL), SOL_SANCTUM_ITEM_OWN_MESSAGE | CELL_BEHAVIOR_SOL_SANCTUM_RELIEFS_SECOND_RELIEF, CONDITION_ALWAYS, EVENT(SolSanctumEntrance_SetSecondSmallJewel) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_SOL_SANCTUM_RELIEFS_SECOND_RELIEF, CONDITION_ALWAYS, EVENT(SolSanctumEntrance_InspectSecondMinotaurRelief) },
    { TRIGGER_TABLE_END },
};
// clang-format on

/// Packs a push step, with X in the upper halfword and Z in the lower (pixels).
#define PUSH_STEP(x, z) (((x) << 16) | ((z) & 0xffff))

s32 gSolSanctumPushSteps[] = {
    PUSH_STEP(16, 0),  PUSH_STEP(16, 0),  PUSH_STEP(16, 0),  PUSH_STEP(0, 16),
    PUSH_STEP(0, 16),  PUSH_STEP(0, 16),  PUSH_STEP(0, 16),  PUSH_STEP(-16, 0),
    PUSH_STEP(-16, 0), PUSH_STEP(-16, 0), PUSH_STEP(-16, 0), PUSH_STEP(0, -16),
    PUSH_STEP(0, -16), PUSH_STEP(0, -16), PUSH_STEP(0, -16), PUSH_STEP(16, 0),
};
