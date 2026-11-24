/// @file
/// The way into Sol Sanctum: the mountainside before its door, and the first
/// rooms inside, where Isaac, Garet and Jenna follow Kraden on the morning the
/// story begins. Two scenes share this overlay, and every entry point chooses
/// by gMainRoutineState.next.
///
/// Sol Entrance (SceneId_SolEntrance) is the paved terrace at the top of the
/// path from Vale, with the sanctum's carved door in the cliff. Walking north
/// into the door (cells (6, 6) to (8, 6)) slides it open and takes the party in
/// (SolSanctumEntrance_OpenDoorAndEnter). Retreat does not work here. Until
/// the eruption the path leads down to Vale; during it, to Vale with the
/// villagers gathered (SceneId_ValeEruption) until the party has left. After
/// the eruption the ground rumbles now and then. Once Isaac and Garet are back
/// from the mountain the door no longer opens: Isaac walks up to it and stops,
/// and searching it finds it "tightly locked". Once the party has left Vale, a
/// bearded man in a long robe stands in the path and warns everyone away.
///
/// Sol Sanctum (SceneId_SolSanctumReliefs) is the run of rooms behind the
/// door, in the order the party meets them:
/// - The entrance hall, where Kraden welcomes them on the first visit
///   (SolSanctumEntrance_IntroduceSanctum).
/// - Two rooms of stepping stones over water, where Isaac jumps one-cell gaps,
///   joined by a corridor (SceneId_SolSanctumCorridor). In the north of the
///   second lie the chest with the first Small Jewel (cell (17, 3)) and, at
///   cell (30, 2), a minotaur relief with an empty eye socket. Setting the
///   jewel in it opens an arched doorway at the end of the west path, between
///   two lit sconces (cells (3, 1) to (5, 2)).
/// - The statue hall, the hall of eleven statues on a checkered floor that
///   the first relief opens. Kraden searches it for the hidden passage
///   Saturos and Menardi spoke of
///   (SolSanctumEntrance_KradenSuspectsHiddenPassage). Its westmost and
///   eastmost statues stand in front of doorways, cells (94, 7) and (114, 7),
///   and can be pushed or moved aside: the west one hides a small room with a
///   statue that will not budge, the east one the steps up to the maze
///   (SceneId_SolSanctumMaze) and its chest with the second Small Jewel.
/// - The second relief room, at the maze's far end, where six statues stand in
///   alcoves across a channel of water. Kraden sees them out of reach
///   (SolSanctumEntrance_KradenSeesStatuesOutOfReach); Move shifts them. The
///   westmost hides the second minotaur relief, the next a doorway into a
///   winding passage that leads nowhere. Once a statue is moved, Isaac can jump
///   to its alcove from the stepping stone in the water before it, and push it
///   by hand. Setting the second jewel opens a way behind the eastmost statue,
///   on to the rooms of Sol and Luna (SceneId_SolSanctumLunaSolRooms).
///
/// Each relief room has actor placement and trigger tables of its own.
/// Scene_GetActorPlacements and _GetTriggers choose them by the entrance the
/// party came in at, so a room's statues and their events exist only while the
/// party is in it.
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
/// The locked door is two changes in Sol Entrance's setup, once
/// FLAG_VALE_RETURNED_FROM_MT_ALEPH is set. It makes the three
/// CELL_BEHAVIOR_SOL_ENTRANCE_DOOR cells before the door plain ground, and the
/// three wall cells across the door CELL_BEHAVIOR_SOL_ENTRANCE_LOCKED_DOOR,
/// copied from a spare row in the cliff at the map's top left, cells (0, 1) to
/// (2, 1), which the player never reaches.

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "macros.h"
#include "types.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/event.h"
#include "field/event_commands.h"
#include "field/event_inline.h"
#include "field/field.h"
#include "field/pushed_actor.h"
#include "field/scene_tables.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "scene/import.h"
#include "scene/walk_into_leader.h"
#include "sound/sound.h"
#include "system/main_routine_state.h"
#include "system/system.h"
#include "text/msg_ids.h"

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
extern s32 gPushedActor_DirectionSteps[];

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
/// one, in the statue nook off the statue hall
/// (SOL_SANCTUM_RELIEFS_STATUE_NOOK_FROM_STATUE_HALL).
enum { ACTOR_STATUE = ACTOR_FIRST_PLACED + 1 };

/// The statue hall's eleven statues: the two that can be pushed,
/// then nine that cannot.
enum {
  ACTOR_WEST_STATUE = ACTOR_FIRST_PLACED + 1,
  ACTOR_EAST_STATUE,
  /// The first of the nine.
  ACTOR_FIRST_FIXED_STATUE
};

/// The second relief room's row of six, each of which can be pushed a cell
/// east or west; alike, they are lettered west to east, each starting 64 pixels
/// east of the one before (SolSanctumEntrance_RestoreInteriorState).
enum {
  ACTOR_ROW_STATUE_A = ACTOR_FIRST_PLACED + 1,
  ACTOR_ROW_STATUE_B,
  ACTOR_ROW_STATUE_C,
  ACTOR_ROW_STATUE_D,
  ACTOR_ROW_STATUE_E,
  ACTOR_ROW_STATUE_F
};

// Area flags: Scene_Arrive clears 0x300 to 0x3ff when the
// party enters another area, and other areas use the numbers for
// their own.

/// The column each statue that can be pushed stands in, east or west of its
/// start.
///
/// They are the statue hall's west and east statues, then the row of six
/// in the second relief room. Both clear means the start.
#define FLAG_SOL_SANCTUM_EAST_STATUE_AT_EAST FLAG_AREA_TEMP_00
#define FLAG_SOL_SANCTUM_EAST_STATUE_AT_WEST FLAG_AREA_TEMP_01
#define FLAG_SOL_SANCTUM_WEST_STATUE_AT_EAST FLAG_AREA_TEMP_02
#define FLAG_SOL_SANCTUM_WEST_STATUE_AT_WEST FLAG_AREA_TEMP_03
/// The second relief room's row of six statues, each two flags as above.
#define FLAG_SOL_SANCTUM_ROW_STATUE_A_AT_EAST FLAG_AREA_TEMP_10
#define FLAG_SOL_SANCTUM_ROW_STATUE_A_AT_WEST FLAG_AREA_TEMP_11
#define FLAG_SOL_SANCTUM_ROW_STATUE_B_AT_EAST FLAG_AREA_TEMP_12
#define FLAG_SOL_SANCTUM_ROW_STATUE_B_AT_WEST FLAG_AREA_TEMP_13
#define FLAG_SOL_SANCTUM_ROW_STATUE_C_AT_EAST FLAG_AREA_TEMP_14
#define FLAG_SOL_SANCTUM_ROW_STATUE_C_AT_WEST FLAG_AREA_TEMP_15
#define FLAG_SOL_SANCTUM_ROW_STATUE_D_AT_EAST FLAG_AREA_TEMP_16
#define FLAG_SOL_SANCTUM_ROW_STATUE_D_AT_WEST FLAG_AREA_TEMP_17
#define FLAG_SOL_SANCTUM_ROW_STATUE_E_AT_EAST FLAG_AREA_TEMP_18
#define FLAG_SOL_SANCTUM_ROW_STATUE_E_AT_WEST FLAG_AREA_TEMP_19
#define FLAG_SOL_SANCTUM_ROW_STATUE_F_AT_EAST FLAG_AREA_TEMP_1A
#define FLAG_SOL_SANCTUM_ROW_STATUE_F_AT_WEST FLAG_AREA_TEMP_1B

// Scene flags, cleared on every ordinary arrival:
// - FLAG_SOL_SANCTUM_IN_SECOND_RELIEF_ROOM: set on entering the second relief
//   room; the Move rows of the statue hall apply only while it is clear,
//   which it always is there: only the second room's entrances set it,
//   every ordinary entrance into the first room clears it, and a return
//   after a battle comes back by the entrance the party last came in
//   by, so the condition never stops a Move.

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
  /// The statue hall's statues, pushed from any side.
  CELL_BEHAVIOR_SOL_SANCTUM_RELIEFS_FIRST_ROOM_STATUE = 22,
  /// The statue hall's statues, pushed east only.
  CELL_BEHAVIOR_SOL_SANCTUM_RELIEFS_FIRST_ROOM_STATUE_EAST = 23,
  /// The second relief room's row of statues.
  CELL_BEHAVIOR_SOL_SANCTUM_RELIEFS_ROW_STATUE = 24,
  /// The statue hall's statues, pushed west only.
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

s32 gSolSanctumEntrance_RumbleTimer BSS_DATA;

/// Returns Sol Entrance's entrances (SceneId_SolEntrance) or Sol
/// Sanctum's (SceneId_SolSanctumReliefs).
const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 sceneId = gMainRoutineState.next;

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
  s32 firstStatueRoomEntrance = SOL_SANCTUM_RELIEFS_STATUE_HALL_FROM_CHEST_ROOM;

  if (gMainRoutineState.next == SCENE_SOL_SANCTUM_RELIEFS) {
    if (gMainRoutineState.entrance >= firstStatueRoomEntrance) {
      if (gMainRoutineState.entrance >
          SOL_SANCTUM_RELIEFS_STATUE_HALL_FROM_STEPS_ROOM) {
        if (gMainRoutineState.entrance <=
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
  if (gMainRoutineState.next == SCENE_SOL_ENTRANCE) {
    return gSolEntrance_ActorPlacements;
  }
  return gSolSanctumEntrance_FallbackActorPlacements;
}

/// Selects the events for the scene and room, by the same entrances as
/// Scene_GetActorPlacements.
const struct Trigger *Scene_GetTriggers(void) {
  s32 firstStatueRoomEntrance = SOL_SANCTUM_RELIEFS_STATUE_HALL_FROM_CHEST_ROOM;

  if (gMainRoutineState.next == SCENE_SOL_ENTRANCE) {
    return gSolEntrance_Triggers;
  }
  if (gMainRoutineState.next == SCENE_SOL_SANCTUM_RELIEFS) {
    if (gMainRoutineState.entrance >= firstStatueRoomEntrance) {
      if (gMainRoutineState.entrance >
          SOL_SANCTUM_RELIEFS_STATUE_HALL_FROM_STEPS_ROOM) {
        if (gMainRoutineState.entrance <=
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

/// Isaac walks north into Sol Sanctum's door at the top of the terrace (cells
/// (6, 6) to (8, 6)); with a boom the stone door slides open, and he walks in
/// as the screen fades.
///
/// The door is drawn open in three steps ten frames apart, each copying the
/// next picture of it from a spare part of the map.
static void SolSanctumEntrance_OpenDoorAndEnter(void) {
  Event_Start();
  // The door opens in three steps.
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

/// Isaac searches the minotaur relief at the north of the second room of
/// stepping stones (cell (30, 2)): "The right eye is sparkling, but there's a
/// hole where the left eye should be." Once the Small Jewel is set, both eyes
/// sparkle.
///
/// While Isaac holds the jewel, the field menu opens as the message ends, so
/// that he can use it on the relief at once.
static void SolSanctumEntrance_InspectFirstMinotaurRelief(void) {
  Event_Start();

  if (GameFlag_Get(FLAG_SOL_SANCTUM_FIRST_SMALL_JEWEL_SET) != 0) {
    // "It's the minotaur relief, with both eyes sparkling brightly."
    FieldMessage_Show(MsgSolSanctumMinotaurReliefBothEyes, 1);
  } else {
    // "It's a relief of a minotaur..."
    FieldMessage_Show(MsgSolSanctumMinotaurReliefOneEye, 1);
    if (GameFlag_Get(FLAG_SOL_SANCTUM_FIRST_SMALL_JEWEL_FOUND) != 0) {
      gFieldState->fieldMenu = 1;
    }
  }

  Event_End();
}

/// The room shakes as stone shifts, and the leader looks round, startled, and
/// jumps.
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

/// Isaac uses the Small Jewel on the first minotaur relief (cell (30, 2)) and
/// sets it into the empty socket. "You hear the sound of shifting stone": the
/// room shakes, Isaac looks round, startled, and an arched doorway opens at
/// the end of the room's west path, between two lit sconces.
///
/// The jewel is used up. The doorway is cells (3, 1) to (5, 2), copied in with
/// their heights from a spare part of the map; Scene_Initialize redraws it on
/// later visits.
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
      // "Isaac set the small gem into the empty socket."
      FieldMessage_Show(message, 1);
      Event_Wait(20);
      Sound_Play(SE_STONE_RUMBLE);
      MapGrid_CopyCellsTo(0, 29, 3, 1, 3, 2);
      MapGrid_CopyCellAttributes(0, 29, 3, 2, 3, 1);
      MapGrid_CopyCellsTo(1, 109, 4, 81, 1, 1);
      MapGrid_CenterOnFollowedPoint();
      SolSanctumEntrance_ReactToOpeningPassage();
      // "You hear the sound of shifting stone."
      FieldMessage_Show(message + 1, 1);
      GameFlag_Set(FLAG_ITEM_USED_UP);
      GameFlag_Set(FLAG_SOL_SANCTUM_FIRST_SMALL_JEWEL_SET);
      Event_End();
    }
  }
}

/// Isaac searches the second minotaur relief, uncovered behind the westmost of
/// the six statues across the water (cell (100, 31)). It too has one eye
/// sparkling and a hole for the other, until the second Small Jewel is set.
static void SolSanctumEntrance_InspectSecondMinotaurRelief(void) {
  struct FieldState *state;

  Event_Start();

  if (GameFlag_Get(FLAG_SOL_SANCTUM_SECOND_SMALL_JEWEL_SET) != 0) {
    // "It's the minotaur relief, with both eyes sparkling brightly."
    FieldMessage_Show(MsgSolSanctumMinotaurReliefBothEyes, 1);
  } else if (GameFlag_Get(FLAG_SOL_SANCTUM_SECOND_SMALL_JEWEL_FOUND) != 0) {
    state = gFieldState;
    // "It's a relief of a minotaur..."
    FieldMessage_Show(MsgSolSanctumMinotaurReliefOneEye, 1);
    state->fieldMenu = 1;
  } else {
    // "It's a relief of a minotaur..."
    FieldMessage_Show(MsgSolSanctumMinotaurReliefOneEye, 1);
  }

  Event_End();
}

/// Isaac uses the second Small Jewel on the second minotaur relief (cell
/// (100, 31)) and sets it. Stone shifts, the room shakes, and a doorway opens
/// in the wall behind the eastmost of the six statues (cells (120, 30) to
/// (120, 31)), the way on to the rooms of Sol and Luna.
///
/// The eastmost statue still stands in front of the doorway until it is moved
/// aside.
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
    // "Isaac set the small gem into the empty socket."
    FieldMessage_Show(message, 1);
    Event_Wait(20);
    Sound_Play(SE_STONE_RUMBLE);
    MapGrid_CopyCellsTo(122, 20, 120, 30, 1, 2);
    MapGrid_CopyCellAttributes(122, 20, 1, 2, 120, 30);
    MapGrid_CenterOnFollowedPoint();
    SolSanctumEntrance_ReactToOpeningPassage();
    // "You hear the sound of shifting stone."
    FieldMessage_Show(message + 1, 1);
    GameFlag_Set(FLAG_ITEM_USED_UP);
    GameFlag_Set(FLAG_SOL_SANCTUM_SECOND_SMALL_JEWEL_SET);
    Event_End();
  }
}

/// Notes which side of its starting column a moved statue now stands on, so
/// it stays there for the rest of the visit: flagBase (its _AT_EAST flag) for
/// rightColumn, the flag after it (_AT_WEST) for leftColumn, neither back at
/// its start.
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

/// Isaac shifts the westmost statue of the checkered statue hall a cell east or
/// west, with Move or by pushing it, and uncovers the dark doorway behind it
/// (cell (94, 7)) to a small room whose statue will not budge. Nothing more
/// happens on screen.
///
/// The Move row runs this after Move has shifted the statue, and
/// SolSanctumEntrance_PushStatue after a push. The statue starts in column 94;
/// columns 93 and 95 set its two flags.
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

/// Isaac shifts the eastmost statue of the checkered statue hall a cell east or
/// west, with Move or by pushing it, and uncovers the doorway behind it (cell
/// (114, 7)) to the steps up to the maze. As for the west statue, the game
/// notes which side it now stands on.
///
/// The statue starts in column 114; columns 113 and 115 set its two flags.
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

/// Isaac uses Move on the westmost of the six statues across the water (cell
/// (100, 32)) and shifts it a cell east or west, uncovering the second minotaur
/// relief in the wall behind it.
///
/// SolSanctumEntrance_PushStatue runs this too, after a push by hand.
/// SolSanctumEntrance_RestoreRowStatueCells then lets Isaac jump into the cell
/// the statue left, from the stepping stone before it.
static void SolSanctumEntrance_RowStatueAMoved(void) {
  struct Actor *statue = Actor_Get(ACTOR_ROW_STATUE_A);
  s32 column;

  if (statue == NULL) {
    return;
  }
  column = FIELD_CELL(statue->x.fixed);
  SolSanctumEntrance_RecordStatueColumn(
      column, FLAG_SOL_SANCTUM_ROW_STATUE_A_AT_EAST, 99, 101);
  SolSanctumEntrance_RestoreRowStatueCells(0);
}

/// Isaac uses Move on the second of the six statues from the west (cell
/// (104, 32)) and shifts it aside, uncovering a dark doorway behind it into a
/// winding passage that leads nowhere.
static void SolSanctumEntrance_RowStatueBMoved(void) {
  struct Actor *statue = Actor_Get(ACTOR_ROW_STATUE_B);
  s32 column;

  if (statue == NULL) {
    return;
  }
  column = FIELD_CELL(statue->x.fixed);
  SolSanctumEntrance_RecordStatueColumn(
      column, FLAG_SOL_SANCTUM_ROW_STATUE_B_AT_EAST, 103, 105);
  SolSanctumEntrance_RestoreRowStatueCells(0);
}

/// Isaac uses Move on the third of the six statues from the west (cell
/// (108, 32)) and shifts it aside; only plain wall stands behind it.
static void SolSanctumEntrance_RowStatueCMoved(void) {
  struct Actor *statue = Actor_Get(ACTOR_ROW_STATUE_C);
  s32 column;

  if (statue == NULL) {
    return;
  }
  column = FIELD_CELL(statue->x.fixed);
  SolSanctumEntrance_RecordStatueColumn(
      column, FLAG_SOL_SANCTUM_ROW_STATUE_C_AT_EAST, 107, 109);
  SolSanctumEntrance_RestoreRowStatueCells(0);
}

/// Isaac uses Move on the fourth of the six statues from the west (cell
/// (112, 32)) and shifts it aside; only plain wall stands behind it.
static void SolSanctumEntrance_RowStatueDMoved(void) {
  struct Actor *statue = Actor_Get(ACTOR_ROW_STATUE_D);
  s32 column;

  if (statue == NULL) {
    return;
  }
  column = FIELD_CELL(statue->x.fixed);
  SolSanctumEntrance_RecordStatueColumn(
      column, FLAG_SOL_SANCTUM_ROW_STATUE_D_AT_EAST, 111, 113);
  SolSanctumEntrance_RestoreRowStatueCells(0);
}

/// Isaac uses Move on the fifth of the six statues from the west (cell
/// (116, 32)) and shifts it aside; only plain wall stands behind it.
static void SolSanctumEntrance_RowStatueEMoved(void) {
  struct Actor *statue = Actor_Get(ACTOR_ROW_STATUE_E);
  s32 column;

  if (statue == NULL) {
    return;
  }
  column = FIELD_CELL(statue->x.fixed);
  SolSanctumEntrance_RecordStatueColumn(
      column, FLAG_SOL_SANCTUM_ROW_STATUE_E_AT_EAST, 115, 117);
  SolSanctumEntrance_RestoreRowStatueCells(0);
}

/// Isaac uses Move on the eastmost of the six statues (cell (120, 32)) and
/// shifts it aside. Behind it is plain wall until the second Small Jewel is
/// set in the relief, then the doorway on to the rooms of Sol and Luna.
static void SolSanctumEntrance_RowStatueFMoved(void) {
  struct Actor *statue = Actor_Get(ACTOR_ROW_STATUE_F);
  s32 column;

  if (statue == NULL)
    return;
  column = FIELD_CELL(statue->x.fixed);
  SolSanctumEntrance_RecordStatueColumn(
      column, FLAG_SOL_SANCTUM_ROW_STATUE_F_AT_EAST, 119, 121);
  SolSanctumEntrance_RestoreRowStatueCells(0);
}

#include "shared/actor_find_at_cell.c"

/// The party has entered the second relief room, which turns off Move on the
/// statue hall's statues.
#define FLAG_SOL_SANCTUM_IN_SECOND_RELIEF_ROOM FLAG_SCENE_TEMP_34

/// Isaac walks into one of the statues that can be pushed and shoves it one
/// cell, following behind it, if nothing is in the way: the westmost and
/// eastmost statues of the checkered hall (cells (94, 8) and (114, 8)), which
/// a dotted line on the floor marks, or a statue of the row across the water,
/// once he has jumped into its alcove.
///
/// Each statue's cells and the cells beside it carry the push behaviours; the
/// statue pushed is whichever stands one step ahead of the leader. After the
/// push the room's Moved routines note where every statue now stands.
static void SolSanctumEntrance_PushStatue(void) {
  struct Actor *leader;
  struct Actor *statue;
  u32 direction;
  struct FieldPosition target;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  direction = FACING_INDEX(leader->facing);
  statue =
      Actor_FindAtCell((leader->x.part[0].integer +
                        (FIXEDtoINT(gPushedActor_DirectionSteps[direction]))) >>
                           4,
                       (leader->z.part[0].integer +
                        (s16)gPushedActor_DirectionSteps[direction]) >>
                           4);
  if (statue != NULL) {
    statue->layer = 2;
    Actor_StepFrom(&target, statue, gPushedActor_DirectionSteps[direction]);
    // Move only after the statue's proposed position passes collision.
    if (FieldActor_CheckMovementCollision(statue, &target) <= 0) {
      Actor_SelectAnimation(leader, ANIM_PUSH);
      wait(15);
      Sound_Play(SE_PUSH);
      statue->speedLimit = toFIXED(1) / 5;
      statue->acceleration = toFIXED(1) / 5;
      FieldActor_SetMoveTargetFX(statue, target.x, target.y, target.z);
      leader->speedLimit = toFIXED(1) / 5;
      leader->acceleration = toFIXED(1) / 5;
      FieldActor_SetMoveTargetFX(leader, target.x, target.y, target.z);
      Actor_WaitForMove(statue);
      statue->x.fixed = target.x;
      statue->z.fixed = target.z;
      statue->velocityX = 0;
      statue->velocityZ = 0;
      Actor_SelectAnimation(leader, ANIM_STAND);
      switch (gMainRoutineState.entrance) {
      case SOL_SANCTUM_RELIEFS_STATUE_HALL_FROM_CHEST_ROOM:
      case SOL_SANCTUM_RELIEFS_STATUE_HALL_FROM_STATUE_NOOK:
      case SOL_SANCTUM_RELIEFS_STATUE_HALL_FROM_STEPS_ROOM:
        SolSanctumEntrance_WestStatueMoved();
        SolSanctumEntrance_EastStatueMoved();
        break;
      case SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_MAZE_EAST:
      case SOL_SANCTUM_RELIEFS_SECOND_RELIEF_ROOM_FROM_ROUND_PASSAGE:
      case SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS:
        SolSanctumEntrance_RowStatueAMoved();
        SolSanctumEntrance_RowStatueBMoved();
        SolSanctumEntrance_RowStatueCMoved();
        SolSanctumEntrance_RowStatueDMoved();
        SolSanctumEntrance_RowStatueEMoved();
        SolSanctumEntrance_RowStatueFMoved();
        break;
      }
    }
  }
}

/// Sets up Sol Entrance or Sol Sanctum.
s32 Scene_Initialize(void) {
  s32 sceneId = gMainRoutineState.next;

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
  gFieldState->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
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
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  if (GameFlag_Get(FLAG_SOL_SANCTUM_ERUPTED) != 0) {
    Scene_QueueMusic(SE_RUMBLE);
    MapGrid_SetShake(1, 1, 1);
    FieldEffect_InitializeSparkles();
  }

  entrance = gMainRoutineState.entrance;
  switch (entrance) {
  case SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_CORRIDOR_WEST:
  case SOL_SANCTUM_RELIEFS_CHEST_ROOM_FROM_STATUE_HALL:
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

  case SOL_SANCTUM_RELIEFS_STATUE_NOOK_FROM_STATUE_HALL:
    Actor_SetSpriteFlags(Actor_Get(ACTOR_STATUE), 0);
    break;

  case SOL_SANCTUM_RELIEFS_FROM_SOL_ENTRANCE:
    gMainRoutineState.retreatScene = SCENE_SOL_SANCTUM_RELIEFS;
    gMainRoutineState.retreatEntrance = SOL_SANCTUM_RELIEFS_FROM_SOL_ENTRANCE;
    if (GameFlag_Get(FLAG_SOL_SANCTUM_ENTERED) == 0)
      SolSanctumEntrance_IntroduceSanctum();
    break;

  case SOL_SANCTUM_RELIEFS_STATUE_HALL_FROM_CHEST_ROOM:
  case SOL_SANCTUM_RELIEFS_STATUE_HALL_FROM_STATUE_NOOK:
  case SOL_SANCTUM_RELIEFS_STATUE_HALL_FROM_STEPS_ROOM:
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
    Actor_SetSpriteFlags(Actor_Get(ACTOR_ROW_STATUE_A), 0);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_ROW_STATUE_B), 0);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_ROW_STATUE_C), 0);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_ROW_STATUE_D), 0);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_ROW_STATUE_E), 0);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_ROW_STATUE_F), 0);
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

/// The first time the party comes in through the door, Kraden steps up in the
/// entrance hall, looks round and asks if this is their first time here; the
/// answer gets "Take a good look at it" or "I doubt that...".
static void SolSanctumEntrance_IntroduceSanctum(void) {
  Event_Start();
  gFieldState->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
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
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  GameFlag_Clear(FLAG_SHOW_AREA_NAME);
  Event_End();
}

/// The first time the party enters the checkered statue hall, Kraden looks
/// round both sides of it, then tells the others that Saturos and Menardi
/// spoke of a hidden passage leading deeper in, and that a sanctum built in
/// honour of the sun ought to show it somewhere. "Let's see if we can find
/// it!" Garet asks whether they should help him, and whatever Isaac answers,
/// the three agree to stay together and help.
static void SolSanctumEntrance_KradenSuspectsHiddenPassage(void) {
  struct Actor *leader;

  Event_Start();
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL)
    Actor_SetPositionFX(ACTOR_KRADEN, leader->x.fixed, leader->z.fixed);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL)
    Actor_SetPositionFX(ACTOR_JENNA, leader->x.fixed, leader->z.fixed);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL)
    Actor_SetPositionFX(ACTOR_GARET, leader->x.fixed, leader->z.fixed);

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
  // Garet: "I think he's serious! Should we help him, Isaac?"
  // Yes: Garet, "But it's too dangerous for us to split up. We should stay
  // together."
  // No: Jenna, "You can't seriously want to turn back after we've come this
  // far."
  Event_OpenMessage(ACTOR_GARET, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
    Event_SetMessageId(MsgSolSanctumDangerousSplitStay);
    Actor_RunRepeatedMotion(ACTOR_GARET, 1);
    Event_ShowMessageThenWait(ACTOR_GARET, 0, 10);
  } else {
    // Isaac winces, and Garet nods and agrees to help Kraden.
    Event_SetMessageId(MsgSolSanctumCantSeriouslyWant);
    Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 20);
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
    Actor_SetDestination(ACTOR_GARET, leader->x.part[0].integer,
                         leader->z.part[0].integer);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_WALK);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL)
    Actor_SetDestination(ACTOR_JENNA, leader->x.part[0].integer,
                         leader->z.part[0].integer);
  Actor_SetAnimation(ACTOR_KRADEN, ANIM_WALK);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL)
    Actor_SetDestination(ACTOR_KRADEN, leader->x.part[0].integer,
                         leader->z.part[0].integer);

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

/// The first time the party comes into the second relief room, the view pans
/// along the six statues across the water and Kraden says, "More statues...
/// It doesn't look like we can reach them," then, "Perhaps Psynergy is the
/// key..."
static void SolSanctumEntrance_KradenSeesStatuesOutOfReach(void) {
  Event_Start();
  gFieldState->transition =
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

/// Sets the ground under the six statues across the water. Each statue's start
/// cell is a wall while it stands there. Once it has been moved, the cell is
/// one Isaac can jump to (CELL_BEHAVIOR_JUMP_FORWARD) from the stepping stone
/// in the water two rows south, so he can get into the alcove. With
/// placeActors, the moved statues are also put back where they were left.
static void SolSanctumEntrance_RestoreRowStatueCells(s32 placeActors) {
  MapGrid_CopyCellAttributes(122, 20, 1, 1, 100, 32);
  MapGrid_CopyCellAttributes(122, 20, 1, 1, 104, 32);
  MapGrid_CopyCellAttributes(122, 20, 1, 1, 108, 32);
  MapGrid_CopyCellAttributes(122, 20, 1, 1, 112, 32);
  MapGrid_CopyCellAttributes(122, 20, 1, 1, 116, 32);
  MapGrid_CopyCellAttributes(122, 20, 1, 1, 120, 32);
  if (GameFlag_Get(FLAG_SOL_SANCTUM_ROW_STATUE_A_AT_WEST) != 0) {
    MapGrid_CopyCellAttributes(121, 20, 1, 1, 100, 32);
    if (placeActors != 0) {
      Actor_SetPosition(ACTOR_ROW_STATUE_A, 1592, 520);
    }
  } else {
    if (GameFlag_Get(FLAG_SOL_SANCTUM_ROW_STATUE_A_AT_EAST) != 0) {
      MapGrid_CopyCellAttributes(121, 20, 1, 1, 100, 32);
      if (placeActors != 0) {
        Actor_SetPosition(ACTOR_ROW_STATUE_A, 1624, 520);
      }
    }
  }
  if (GameFlag_Get(FLAG_SOL_SANCTUM_ROW_STATUE_B_AT_WEST) != 0) {
    MapGrid_CopyCellAttributes(121, 20, 1, 1, 104, 32);
    if (placeActors != 0) {
      Actor_SetPosition(ACTOR_ROW_STATUE_B, 1656, 520);
    }
  } else {
    if (GameFlag_Get(FLAG_SOL_SANCTUM_ROW_STATUE_B_AT_EAST) != 0) {
      MapGrid_CopyCellAttributes(121, 20, 1, 1, 104, 32);
      if (placeActors != 0) {
        Actor_SetPosition(ACTOR_ROW_STATUE_B, 1688, 520);
      }
    }
  }
  if (GameFlag_Get(FLAG_SOL_SANCTUM_ROW_STATUE_C_AT_WEST) != 0) {
    MapGrid_CopyCellAttributes(121, 20, 1, 1, 108, 32);
    if (placeActors != 0) {
      Actor_SetPosition(ACTOR_ROW_STATUE_C, 1720, 520);
    }
  } else {
    if (GameFlag_Get(FLAG_SOL_SANCTUM_ROW_STATUE_C_AT_EAST) != 0) {
      MapGrid_CopyCellAttributes(121, 20, 1, 1, 108, 32);
      if (placeActors != 0) {
        Actor_SetPosition(ACTOR_ROW_STATUE_C, 1752, 520);
      }
    }
  }
  if (GameFlag_Get(FLAG_SOL_SANCTUM_ROW_STATUE_D_AT_WEST) != 0) {
    MapGrid_CopyCellAttributes(121, 20, 1, 1, 112, 32);
    if (placeActors != 0) {
      Actor_SetPosition(ACTOR_ROW_STATUE_D, 1784, 520);
    }
  } else {
    if (GameFlag_Get(FLAG_SOL_SANCTUM_ROW_STATUE_D_AT_EAST) != 0) {
      MapGrid_CopyCellAttributes(121, 20, 1, 1, 112, 32);
      if (placeActors != 0) {
        Actor_SetPosition(ACTOR_ROW_STATUE_D, 1816, 520);
      }
    }
  }
  if (GameFlag_Get(FLAG_SOL_SANCTUM_ROW_STATUE_E_AT_WEST) != 0) {
    MapGrid_CopyCellAttributes(121, 20, 1, 1, 116, 32);
    if (placeActors != 0) {
      Actor_SetPosition(ACTOR_ROW_STATUE_E, 1848, 520);
    }
  } else {
    if (GameFlag_Get(FLAG_SOL_SANCTUM_ROW_STATUE_E_AT_EAST) != 0) {
      MapGrid_CopyCellAttributes(121, 20, 1, 1, 116, 32);
      if (placeActors != 0) {
        Actor_SetPosition(ACTOR_ROW_STATUE_E, 1880, 520);
      }
    }
  }
  if (GameFlag_Get(FLAG_SOL_SANCTUM_ROW_STATUE_F_AT_WEST) != 0) {
    MapGrid_CopyCellAttributes(121, 20, 1, 1, 120, 32);
    if (placeActors != 0) {
      Actor_SetPosition(ACTOR_ROW_STATUE_F, 1912, 520);
    }
  } else {
    if (GameFlag_Get(FLAG_SOL_SANCTUM_ROW_STATUE_F_AT_EAST) != 0) {
      MapGrid_CopyCellAttributes(121, 20, 1, 1, 120, 32);
      if (placeActors != 0) {
        Actor_SetPosition(ACTOR_ROW_STATUE_F, 1944, 520);
      }
    }
  }
}

/// Isaac searches Sol Sanctum's door once he and Garet are back from the
/// mountain: "The door is tightly locked."
///
/// The search answers the wall cells across the door, (6, 5) to (8, 5), which
/// Sol Entrance's setup gives this behaviour only then.
static void SolSanctumEntrance_DoorTightlyLocked(void) {
  Event_Start();
  // "The door is tightly locked."
  FieldMessage_Show(MsgFieldDoorTightlyLocked, 1);
  Event_End();
}

/// After the eruption the terrace before the door rumbles and shakes every few
/// seconds, at random, each shake dying away.
///
/// A shake lasts 80 frames and starts to fade after 40. Between shakes, each
/// frame has one chance in 120 of starting the next.
static void SolSanctumEntrance_UpdateEruptionRumbles(void) {
  if (gSolSanctumEntrance_RumbleTimer != 0) {
    if (--gSolSanctumEntrance_RumbleTimer == 40) {
      MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
    }
  } else {
    if (Random_Below(120) == 0) {
      Sound_Play(SE_ITEM_BREAK);
      MapGrid_SetShake(1, 2, 1);
      gSolSanctumEntrance_RumbleTimer = 80;
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
    {SOL_SANCTUM_RELIEFS_CHEST_ROOM_FROM_STATUE_HALL, CONDITION_ALWAYS, 72, 0,
     56, FACING_SOUTH, 0, 5, 0, 555, 510, 0},
    {SOL_SANCTUM_RELIEFS_STATUE_NOOK_FROM_STATUE_HALL, CONDITION_ALWAYS, 744, 0,
     152, FACING_NORTH, 0, 620, 0, 860, 195, 0},
    {SOL_SANCTUM_RELIEFS_STEPPING_STONE_ROOM_FROM_ENTRANCE_ROOM,
     CONDITION_ALWAYS, 696, 0, 568, FACING_NORTH, 0, 575, 265, 815, 610, 0},
    {SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_CORRIDOR_EAST, CONDITION_ALWAYS, 696,
     0, 328, FACING_SOUTH, 0, 575, 265, 815, 610, 0},
    {SOL_SANCTUM_RELIEFS_STEPS_ROOM_FROM_STATUE_HALL, CONDITION_ALWAYS, 984, 0,
     152, FACING_NORTH, 0, 860, 0, 1100, 195, 0},
    {SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_MAZE_WEST, CONDITION_ALWAYS, 984, 0,
     56, FACING_SOUTH, 0, 860, 0, 1100, 195, 0},
    {SOL_SANCTUM_RELIEFS_FROM_SOL_ENTRANCE, CONDITION_ALWAYS, 1224, 0, 184,
     FACING_NORTH, 0, 1100, 20, 1340, 220, 0},
    {SOL_SANCTUM_RELIEFS_ENTRANCE_ROOM_FROM_STEPPING_STONE_ROOM,
     CONDITION_ALWAYS, 1224, 0, 104, FACING_SOUTH, 0, 1100, 20, 1340, 220, 0},
    {SOL_SANCTUM_RELIEFS_ROUND_PASSAGE_FROM_SECOND_RELIEF_ROOM,
     CONDITION_ALWAYS, 1320, 0, 456, FACING_NORTH, 0, 905, 285, 1365, 490, 0},
    {SOL_SANCTUM_RELIEFS_STATUE_HALL_FROM_CHEST_ROOM, CONDITION_ALWAYS, 1672, 0,
     280, FACING_NORTH, 0, 1455, 0, 1890, 310, 0},
    {SOL_SANCTUM_RELIEFS_STATUE_HALL_FROM_STATUE_NOOK, CONDITION_ALWAYS, 1512,
     0, 136, FACING_SOUTH, 0, 1455, 0, 1890, 310, 0},
    {SOL_SANCTUM_RELIEFS_STATUE_HALL_FROM_STEPS_ROOM, CONDITION_ALWAYS, 1832, 0,
     136, FACING_SOUTH, 0, 1455, 0, 1890, 310, 0},
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
        SOL_SANCTUM_RELIEFS_CHEST_ROOM_TO_STATUE_HALL, SolSanctumReliefs,
        SOL_SANCTUM_RELIEFS_STATUE_HALL_FROM_CHEST_ROOM, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(
        SOL_SANCTUM_RELIEFS_STATUE_NOOK_TO_STATUE_HALL, SolSanctumReliefs,
        SOL_SANCTUM_RELIEFS_STATUE_HALL_FROM_STATUE_NOOK, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(
        SOL_SANCTUM_RELIEFS_STEPPING_STONE_ROOM_TO_ENTRANCE_ROOM,
        SolSanctumReliefs,
        SOL_SANCTUM_RELIEFS_ENTRANCE_ROOM_FROM_STEPPING_STONE_ROOM,
        EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(
        SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_CORRIDOR_EAST, SolSanctumCorridor,
        SOL_SANCTUM_CORRIDOR_FROM_SOL_SANCTUM_RELIEFS_EAST, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(
        SOL_SANCTUM_RELIEFS_STEPS_ROOM_TO_STATUE_HALL, SolSanctumReliefs,
        SOL_SANCTUM_RELIEFS_STATUE_HALL_FROM_STEPS_ROOM, EXIT_FLAG_NONE),
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
        SOL_SANCTUM_RELIEFS_STATUE_HALL_TO_CHEST_ROOM, SolSanctumReliefs,
        SOL_SANCTUM_RELIEFS_CHEST_ROOM_FROM_STATUE_HALL, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(
        SOL_SANCTUM_RELIEFS_STATUE_HALL_TO_STATUE_NOOK, SolSanctumReliefs,
        SOL_SANCTUM_RELIEFS_STATUE_NOOK_FROM_STATUE_HALL, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(
        SOL_SANCTUM_RELIEFS_STATUE_HALL_TO_STEPS_ROOM, SolSanctumReliefs,
        SOL_SANCTUM_RELIEFS_STEPS_ROOM_FROM_STATUE_HALL, EXIT_FLAG_NONE),
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
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_CHEST_ROOM_TO_STATUE_HALL },
    { TRIGGER_EXIT, 3, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_STATUE_NOOK_TO_STATUE_HALL },
    { TRIGGER_EXIT, 4, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_STEPPING_STONE_ROOM_TO_ENTRANCE_ROOM },
    { TRIGGER_EXIT, 5, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_CORRIDOR_EAST },
    { TRIGGER_EXIT, 6, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_STEPS_ROOM_TO_STATUE_HALL },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 7, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_MAZE_WEST },
    { TRIGGER_EXIT, 8, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_ENTRANCE },
    { TRIGGER_EXIT, 9, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_ENTRANCE_ROOM_TO_STEPPING_STONE_ROOM },
    { TRIGGER_EXIT, 10, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_ROUND_PASSAGE_TO_SECOND_RELIEF_ROOM },
    { TRIGGER_EXIT, 11, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_STATUE_HALL_TO_CHEST_ROOM },
    { TRIGGER_EXIT, 12, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_STATUE_HALL_TO_STATUE_NOOK },
    { TRIGGER_EXIT, 13, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_STATUE_HALL_TO_STEPS_ROOM },
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
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_CHEST_ROOM_TO_STATUE_HALL },
    { TRIGGER_EXIT, 3, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_STATUE_NOOK_TO_STATUE_HALL },
    { TRIGGER_EXIT, 4, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_STEPPING_STONE_ROOM_TO_ENTRANCE_ROOM },
    { TRIGGER_EXIT, 5, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_CORRIDOR_EAST },
    { TRIGGER_EXIT, 6, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_STEPS_ROOM_TO_STATUE_HALL },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 7, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_MAZE_WEST },
    { TRIGGER_EXIT, 8, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_ENTRANCE },
    { TRIGGER_EXIT, 9, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_ENTRANCE_ROOM_TO_STEPPING_STONE_ROOM },
    { TRIGGER_EXIT, 10, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_ROUND_PASSAGE_TO_SECOND_RELIEF_ROOM },
    { TRIGGER_EXIT, 11, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_STATUE_HALL_TO_CHEST_ROOM },
    { TRIGGER_EXIT, 12, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_STATUE_HALL_TO_STATUE_NOOK },
    { TRIGGER_EXIT, 13, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_STATUE_HALL_TO_STEPS_ROOM },
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
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_CHEST_ROOM_TO_STATUE_HALL },
    { TRIGGER_EXIT, 3, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_STATUE_NOOK_TO_STATUE_HALL },
    { TRIGGER_EXIT, 4, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_STEPPING_STONE_ROOM_TO_ENTRANCE_ROOM },
    { TRIGGER_EXIT, 5, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_CORRIDOR_EAST },
    { TRIGGER_EXIT, 6, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_STEPS_ROOM_TO_STATUE_HALL },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 7, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_MAZE_WEST },
    { TRIGGER_EXIT, 8, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_ENTRANCE },
    { TRIGGER_EXIT, 9, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_ENTRANCE_ROOM_TO_STEPPING_STONE_ROOM },
    { TRIGGER_EXIT, 10, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_ROUND_PASSAGE_TO_SECOND_RELIEF_ROOM },
    { TRIGGER_EXIT, 11, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_STATUE_HALL_TO_CHEST_ROOM },
    { TRIGGER_EXIT, 12, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_STATUE_HALL_TO_STATUE_NOOK },
    { TRIGGER_EXIT, 13, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_STATUE_HALL_TO_STEPS_ROOM },
    { TRIGGER_EXIT, 14, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_MAZE_EAST },
    { TRIGGER_EXIT, 15, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_SECOND_RELIEF_ROOM_TO_ROUND_PASSAGE },
    { TRIGGER_EXIT, 16, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_LUNA_SOL_ROOMS },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ROW_STATUE_A, CONDITION_ALWAYS, EVENT(SolSanctumEntrance_RowStatueAMoved) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ROW_STATUE_B, CONDITION_ALWAYS, EVENT(SolSanctumEntrance_RowStatueBMoved) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ROW_STATUE_C, CONDITION_ALWAYS, EVENT(SolSanctumEntrance_RowStatueCMoved) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ROW_STATUE_D, CONDITION_ALWAYS, EVENT(SolSanctumEntrance_RowStatueDMoved) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ROW_STATUE_E, CONDITION_ALWAYS, EVENT(SolSanctumEntrance_RowStatueEMoved) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ROW_STATUE_F, CONDITION_ALWAYS, EVENT(SolSanctumEntrance_RowStatueFMoved) },
    { STEP_TRIGGER_WALKING, CELL_BEHAVIOR_SOL_SANCTUM_RELIEFS_ROW_STATUE, CONDITION_ALWAYS, EVENT(SolSanctumEntrance_PushStatue) },
    { USE_ITEM(ITEM_SMALL_JEWEL), SOL_SANCTUM_ITEM_OWN_MESSAGE | CELL_BEHAVIOR_SOL_SANCTUM_RELIEFS_SECOND_RELIEF, CONDITION_ALWAYS, EVENT(SolSanctumEntrance_SetSecondSmallJewel) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_SOL_SANCTUM_RELIEFS_SECOND_RELIEF, CONDITION_ALWAYS, EVENT(SolSanctumEntrance_InspectSecondMinotaurRelief) },
    { TRIGGER_TABLE_END },
};
// clang-format on

#include "shared/pushed_actor_direction_steps.c"
