/// @file
/// Sol Entrance and the first rooms of Sol Sanctum, two scenes one overlay
/// serves, chosen by gAdventure.sceneId in every entry point.
///
/// - Sol Entrance (SceneId_SolEntrance), the mountainside before the
///   sanctum's door, reached from Vale (VALE_TO_SOL_ENTRANCE). Walking north
///   into the door opens it and takes the leader in
///   (SolSanctumEntrance_OpenDoorAndEnter); searching the other door finds it
///   "tightly locked". Retreat does not work here. The way down leads to
///   SceneId_Vale, except between the eruption (FLAG_SOL_SANCTUM_ERUPTED) and
///   the party leaving Vale (FLAG_VALE_PARTY_LEFT), when it leads to
///   SceneId_ValeEruption. After the eruption the ground rumbles now and then;
///   once the party has left Vale a man stands in the path and warns everyone
///   away.
/// - Sol Sanctum (SceneId_SolSanctumReliefs), the rooms inside the door, which
///   Isaac, Garet and Jenna explore with Kraden. Two minotaur reliefs each have
///   an empty eye socket, and a Small Jewel set in it opens the way on: the
///   first from this scene's chest (FLAG_SOL_SANCTUM_FIRST_SMALL_JEWEL_SET),
///   the second from the chest in SceneId_SolSanctumMaze
///   (FLAG_SOL_SANCTUM_SECOND_SMALL_JEWEL_SET). Kraden has an event on the
///   first entry at the door (SolSanctumEntrance_IntroduceSanctum), in the
///   first relief room (SolSanctumEntrance_KradenSuspectsHiddenPassage) and in
///   the second (SolSanctumEntrance_KradenSeesStatuesOutOfReach). Two statues
///   in the first relief room and the row of six in the second can be pushed,
///   or moved with Move, a cell east or west. Its exits lead to
///   SceneId_SolSanctumCorridor, SceneId_SolSanctumMaze, Sol Entrance and
///   SceneId_SolSanctumLunaSolRooms; the rest lead between its own rooms.
///
/// SceneId_SolSanctumReliefs is one scene, but each relief room has actor
/// placement and trigger tables of its own:
/// SolSanctumEntrance_GetActorPlacements and _GetTriggers choose them by
/// the entrance the party came in at, so a room's statues and their
/// events exist only while the party is in it. Each pushable statue's column
/// is kept in two area flags, one for each side, which
/// SolSanctumEntrance_RestoreInteriorState reads to put it back on entry;
/// Scene_Arrive clears them when the party enters another
/// area, so the statues are back at their starts on the next visit.
/// Searching a relief while its Small Jewel is held asks for the field menu as
/// the search ends, so that the jewel can be used on it at once.
///
/// Once Isaac and Garet are back in Vale (FLAG_VALE_RETURNED_FROM_MT_ALEPH)
/// the door no longer opens: Sol Entrance's setup makes the three wall cells
/// across the door CELL_BEHAVIOR_SOL_ENTRANCE_LOCKED_DOOR, copied from the
/// other door, and the three CELL_BEHAVIOR_SOL_ENTRANCE_DOOR cells before it
/// plain ground, so the leader walks up to the door and stops, and searching it
/// finds it "tightly locked" (render probe sol-entrance).

#include "adventure.h"
#include "module/field/field.h"
#include "module/import.h"
#include "module/scene/sol_sanctum_entrance.h"
#include "module/overworld/overworld.h"
#include "module/overworld/event_commands.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "event.h"
#include "scene_tables.h"
#include "event_inline.h"
#include "sol_sanctum_entrance.h"
#include "scene_ids.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "actor.h"
#include "fixed_math.h"
#include "macros.h"
#include "sounds.h"
#include "item_ids.h"

// Area flags: Scene_Arrive clears 0x300 to 0x3ff when the
// party enters another area, and other areas use the numbers for
// their own.

/// The column each statue that can be pushed stands in, east or west of its
/// start: the first relief room's west and east statues, then the row
/// of six in the second relief room. Both clear means the start.
/// ★ Named from what the player sees: the stone statues of the first relief
/// room, two of which Move pushes a cell aside (its render)
#define FLAG_SOL_SANCTUM_EAST_STATUE_AT_EAST 0x300
/// ★ Named from what the player sees: the first relief room's east statue,
/// pushed a cell west (its render)
#define FLAG_SOL_SANCTUM_EAST_STATUE_AT_WEST 0x301
/// ★ Named from what the player sees: the first relief room's west statue,
/// pushed a cell east (its render)
#define FLAG_SOL_SANCTUM_WEST_STATUE_AT_EAST 0x302
/// ★ Named from what the player sees: the first relief room's west statue,
/// pushed a cell west (its render)
#define FLAG_SOL_SANCTUM_WEST_STATUE_AT_WEST 0x303
/// The second relief room's row of six statues, each two flags as above.
/// ★ Named from the game: "More statues..."
/// (MsgSolSanctumMoreStatuesOutOfReach)
#define FLAG_SOL_SANCTUM_ROW_STATUE_1_AT_EAST 0x310
/// ★ Named from the game: "More statues..."
/// (MsgSolSanctumMoreStatuesOutOfReach)
#define FLAG_SOL_SANCTUM_ROW_STATUE_1_AT_WEST 0x311
/// ★ Named from the game: "More statues..."
/// (MsgSolSanctumMoreStatuesOutOfReach)
#define FLAG_SOL_SANCTUM_ROW_STATUE_2_AT_EAST 0x312
/// ★ Named from the game: "More statues..."
/// (MsgSolSanctumMoreStatuesOutOfReach)
#define FLAG_SOL_SANCTUM_ROW_STATUE_2_AT_WEST 0x313
/// ★ Named from the game: "More statues..."
/// (MsgSolSanctumMoreStatuesOutOfReach)
#define FLAG_SOL_SANCTUM_ROW_STATUE_3_AT_EAST 0x314
/// ★ Named from the game: "More statues..."
/// (MsgSolSanctumMoreStatuesOutOfReach)
#define FLAG_SOL_SANCTUM_ROW_STATUE_3_AT_WEST 0x315
/// ★ Named from the game: "More statues..."
/// (MsgSolSanctumMoreStatuesOutOfReach)
#define FLAG_SOL_SANCTUM_ROW_STATUE_4_AT_EAST 0x316
/// ★ Named from the game: "More statues..."
/// (MsgSolSanctumMoreStatuesOutOfReach)
#define FLAG_SOL_SANCTUM_ROW_STATUE_4_AT_WEST 0x317
/// ★ Named from the game: "More statues..."
/// (MsgSolSanctumMoreStatuesOutOfReach)
#define FLAG_SOL_SANCTUM_ROW_STATUE_5_AT_EAST 0x318
/// ★ Named from the game: "More statues..."
/// (MsgSolSanctumMoreStatuesOutOfReach)
#define FLAG_SOL_SANCTUM_ROW_STATUE_5_AT_WEST 0x319
/// ★ Named from the game: "More statues..."
/// (MsgSolSanctumMoreStatuesOutOfReach)
#define FLAG_SOL_SANCTUM_ROW_STATUE_6_AT_EAST 0x31a
/// ★ Named from the game: "More statues..."
/// (MsgSolSanctumMoreStatuesOutOfReach)
#define FLAG_SOL_SANCTUM_ROW_STATUE_6_AT_WEST 0x31b

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

/// The cell behaviors of the cells the Sol Sanctum entrance's coord, bg
/// and item triggers answer; behaviors 1 to 16 in both scenes are the exits, in
/// the exit table's order.
enum SolEntranceCellBehavior {
  /// ★ Named from what the player sees: the door that slides open as the
  /// leader walks into it (SolSanctumEntrance_OpenDoorAndEnter)
  CELL_BEHAVIOR_SOL_ENTRANCE_DOOR = 2,
  /// ★ Named from the game: "The door is tightly locked."
  /// (MsgFieldDoorTightlyLocked)
  CELL_BEHAVIOR_SOL_ENTRANCE_LOCKED_DOOR = 10
};

/// The cell behaviors of the cells the relief rooms' coord, bg and
/// item triggers answer.
enum SolSanctumReliefsCellBehavior {
  /// ★ Named from the game: "It's a relief of a minotaur."
  /// (MsgSolSanctumMinotaurReliefOneEye)
  CELL_BEHAVIOR_SOL_SANCTUM_RELIEFS_FIRST_RELIEF = 20,
  /// ★ Named from the game: "It's a relief of a minotaur."
  /// (MsgSolSanctumMinotaurReliefOneEye)
  CELL_BEHAVIOR_SOL_SANCTUM_RELIEFS_SECOND_RELIEF = 21,
  /// The first relief room's statues, pushed from any side.
  CELL_BEHAVIOR_SOL_SANCTUM_RELIEFS_FIRST_ROOM_STATUE = 22,
  /// The first relief room's statues, pushed east only.
  CELL_BEHAVIOR_SOL_SANCTUM_RELIEFS_FIRST_ROOM_STATUE_EAST = 23,
  /// The second relief room's row of statues.
  CELL_BEHAVIOR_SOL_SANCTUM_RELIEFS_ROW_STATUE = 24,
  /// The first relief room's statues, pushed west only.
  CELL_BEHAVIOR_SOL_SANCTUM_RELIEFS_FIRST_ROOM_STATUE_WEST = 25,
  /// ★ Named from the game: "checked the chest" (MsgVaultIsaacCheckedChest)
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

void FieldEffect_InitializeSparkles();

/// Returns Sol Entrance's entrances (SceneId_SolEntrance) or Sol
/// Sanctum's (SceneId_SolSanctumReliefs).
const struct SceneEntrance *SolSanctumEntrance_GetEntrances(void) {
  s16 sceneId = gAdventure.sceneId;

  if (sceneId == (s32)&SceneId_SolEntrance) {
    return gSolEntrance_Entrances;
  }
  if (sceneId == (s32)&SceneId_SolSanctumReliefs) {
    return gSolSanctumReliefs_Entrances;
  }
  return gSolSanctumEntrance_FallbackEntrances;
}

const struct SceneExitRegion *SolSanctumEntrance_GetRegions(void) { return 0; }

const u32 *SolSanctumEntrance_GetExits(void) {
  return gSolSanctumEntrance_Exits;
}

/// Selects the actors for the scene and, in Sol Sanctum, the room. The
/// first relief room's entrances place Kraden and that room's eleven
/// statues, the second relief room's Kraden and the row of six; the rest of Sol
/// Sanctum places Kraden and one statue (ACTOR_STATUE), and Sol Entrance the
/// man who warns everyone away (ACTOR_WARNING_MAN).
const struct ActorPlacement *SolSanctumEntrance_GetActorPlacements(void) {
  struct ActorPlacement *table;
  s32 firstStatueRoomEntrance =
      SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_FROM_CHEST_ROOM;

  if (gAdventure.sceneId == (s32)&SceneId_SolSanctumReliefs) {
    if (gAdventure.entrance >= firstStatueRoomEntrance) {
      if (gAdventure.entrance >
          SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_FROM_STEPS_ROOM) {
        if (gAdventure.entrance <=
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
  if (gAdventure.sceneId == (s32)&SceneId_SolEntrance) {
    return gSolEntrance_ActorPlacements;
  }
  return gSolSanctumEntrance_FallbackActorPlacements;
}

/// Selects the events for the scene and room, by the same entrances as
/// SolSanctumEntrance_GetActorPlacements.
const struct Trigger *SolSanctumEntrance_GetTriggers(void) {
  s32 firstStatueRoomEntrance =
      SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_FROM_CHEST_ROOM;

  if (gAdventure.sceneId == (s32)&SceneId_SolEntrance) {
    return gSolEntrance_Triggers;
  }
  if (gAdventure.sceneId == (s32)&SceneId_SolSanctumReliefs) {
    if (gAdventure.entrance >= firstStatueRoomEntrance) {
      if (gAdventure.entrance >
          SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_FROM_STEPS_ROOM) {
        if (gAdventure.entrance <=
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

/// Sol Entrance: walking north into the sanctum's door slides it open, and
/// the leader walks in. Takes SOL_ENTRANCE_TO_SOL_SANCTUM_RELIEFS, into Sol
/// Sanctum at SOL_SANCTUM_RELIEFS_FROM_SOL_ENTRANCE.
static void SolSanctumEntrance_OpenDoorAndEnter(void) {
  s32 width;
  s32 height;

  Event_Start();
  // The door opens over three frames.
  width = 3;
  height = 2;
  Event_PlaySound(SE_BOOM);
  MapGrid_CopyCellsTo(16, 28, 21, 3, width, height);
  wait(10);
  MapGrid_CopyCellsTo(16, 30, 21, 3, width, height);
  wait(10);
  MapGrid_CopyCellsTo(16, 32, 21, 3, width, height);
  wait(10);
  Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 2);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 120, 98);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -8);
  Event_Wait(10);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(SOL_ENTRANCE_TO_SOL_SANCTUM_RELIEFS);
  Event_End();
}

/// Sol Sanctum: searching the first minotaur relief shows one eye sparkling, or
/// both once the Small Jewel is set: "The right eye is sparkling, but there's a
/// hole where the left eye should be." Once the first Small Jewel is found, and
/// until it is set, the File Screen opens as the search ends, so that the jewel
/// can be used.
static void SolSanctumEntrance_InspectFirstMinotaurRelief(void) {

  Event_Start();

  if (GameFlag_Get(FLAG_SOL_SANCTUM_FIRST_SMALL_JEWEL_SET) != 0) {
    FieldMessage_Show(MsgSolSanctumMinotaurReliefBothEyes, 1);
  } else {
    FieldMessage_Show(MsgSolSanctumMinotaurReliefOneEye, 1);
    if (GameFlag_Get(FLAG_SOL_SANCTUM_FIRST_SMALL_JEWEL_FOUND) != 0) {
      u16 *menuRequest = &gFieldWork->fieldMenu;
      u16 open = 1;
      *menuRequest = open;
    }
  }

  Event_End();
}

/// The room shakes as stone shifts; the leader looks around and jumps.
static inline void SolSanctumEntrance_ReactToOpeningPassage(void) {
  MapGrid_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  MapGrid_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 40);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 10);
  Actor_Jump(ACTOR_PARTY_LEADER, 4, 20);
  Actor_Jump(ACTOR_PARTY_LEADER, 6, 40);
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Event_Wait(40);
}

/// Sol Sanctum: using the Small Jewel on the first relief sets it in, and a
/// passage opens. `<leader> set the small gem into the empty socket.`, "You
/// hear the sound of shifting stone." Does nothing until the first Small Jewel
/// is found, or once it is set. Sets FLAG_SOL_SANCTUM_FIRST_SMALL_JEWEL_SET and
/// FLAG_ITEM_USED_UP, which takes the jewel.
static void SolSanctumEntrance_SetFirstSmallJewel(void) {
  s32 message;
  s32 oneCell;
  s32 reliefWidth;

  if (GameFlag_Get(FLAG_SOL_SANCTUM_FIRST_SMALL_JEWEL_FOUND) == 0) {
  } else {
    if (Event_GetFlag(FLAG_SOL_SANCTUM_FIRST_SMALL_JEWEL_SET) != 0) {
    } else {
      Event_Start();
      Field_EndBusyAndReveal();
      // The eye sparkles, then the wall opens.
      oneCell = 1;
      Event_PlaySound(SE_GEM_SET);
      MapGrid_CopyCellsTo(0, 70, 30, 42, oneCell, oneCell);
      MapGrid_CenterOnFollowedPoint();
      Event_Wait(40);
      message = MsgSolSanctumSetSmallGem;
      FieldMessage_Show(message, 1);
      Event_Wait(20);
      reliefWidth = 3;
      Event_PlaySound(SE_STONE_RUMBLE);
      MapGrid_CopyCellsTo(0, 29, 3, 1, reliefWidth, 2);
      MapGrid_CopyCellAttributes(0, 29, 3, 2, reliefWidth, oneCell);
      MapGrid_CopyCellsTo(1, 109, 4, 81, oneCell, oneCell);
      MapGrid_CenterOnFollowedPoint();
      SolSanctumEntrance_ReactToOpeningPassage();
      FieldMessage_Show(message + 1, 1);
      GameFlag_Set(FLAG_ITEM_USED_UP);
      GameFlag_Set(FLAG_SOL_SANCTUM_FIRST_SMALL_JEWEL_SET);
      Event_End();
    }
  }
}

/// Sol Sanctum, the second relief room: searching the second minotaur
/// relief, as the first. Once the second Small Jewel is found,
/// and until it is set, it opens the File Screen.
static void SolSanctumEntrance_InspectSecondMinotaurRelief(void) {

  struct FieldWork *work;

  Event_Start();

  if (GameFlag_Get(FLAG_SOL_SANCTUM_SECOND_SMALL_JEWEL_SET) != 0) {
    FieldMessage_Show(MsgSolSanctumMinotaurReliefBothEyes, 1);
  } else if (GameFlag_Get(FLAG_SOL_SANCTUM_SECOND_SMALL_JEWEL_FOUND) != 0) {
    work = gFieldWork;
    FieldMessage_Show(MsgSolSanctumMinotaurReliefOneEye, 1);
    {

      u16 *menuRequest = &work->fieldMenu;
      s32 open = 1;
      *menuRequest = open;
    }
  } else {
    FieldMessage_Show(MsgSolSanctumMinotaurReliefOneEye, 1);
  }

  Event_End();
}

/// Sol Sanctum, the second relief room: using the Small Jewel on the
/// second relief, as the first, and a doorway opens. Only once the
/// second Small Jewel is found and until it is set. Sets
/// FLAG_SOL_SANCTUM_SECOND_SMALL_JEWEL_SET and FLAG_ITEM_USED_UP.
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
/// and called by SolSanctumEntrance_PushStatue there. The column it stops in is
/// saved: sets FLAG_SOL_SANCTUM_WEST_STATUE_AT_EAST or _AT_WEST, or clears both
/// when it stands in neither column.
static void SolSanctumEntrance_WestStatueMoved(void) {
  struct Actor *statue = Actor_GetById(ACTOR_WEST_STATUE);
  s32 column;

  if (statue == NULL) {
    return;
  }
  column = statue->x.fixed >> 20;
  SolSanctumEntrance_RecordStatueColumn(
      column, FLAG_SOL_SANCTUM_WEST_STATUE_AT_EAST, 93, 95);
}

/// Sol Sanctum, the first relief room: Move on the east statue
/// (ACTOR_EAST_STATUE), as the west one. Sets
/// FLAG_SOL_SANCTUM_EAST_STATUE_AT_EAST or _AT_WEST, or clears both.
static void SolSanctumEntrance_EastStatueMoved(void) {
  struct Actor *statue = Actor_GetById(ACTOR_EAST_STATUE);
  s32 column;

  if (statue == NULL) {
    return;
  }
  column = statue->x.fixed >> 20;
  Event_ClearFlag(FLAG_SOL_SANCTUM_EAST_STATUE_AT_EAST);
  Event_ClearFlag(FLAG_SOL_SANCTUM_EAST_STATUE_AT_WEST);
  if (column == 115) {
    GameFlag_Set(FLAG_SOL_SANCTUM_EAST_STATUE_AT_EAST);
  } else if (column == 113) {
    GameFlag_Set(FLAG_SOL_SANCTUM_EAST_STATUE_AT_WEST);
  }
}

/// Sol Sanctum, the second relief room: Move on the first statue of the
/// row (ACTOR_ROW_STATUE_1), and called by SolSanctumEntrance_PushStatue there.
/// The column it stops in is saved and the cells under all six are set again.
/// Sets FLAG_SOL_SANCTUM_ROW_STATUE_1_AT_EAST or _AT_WEST, or clears both.
static void SolSanctumEntrance_RowStatue1Moved(void) {
  struct Actor *statue = Actor_GetById(ACTOR_ROW_STATUE_1);
  s32 column;

  if (statue == NULL) {
    return;
  }
  column = statue->x.fixed >> 20;
  SolSanctumEntrance_RecordStatueColumn(
      column, FLAG_SOL_SANCTUM_ROW_STATUE_1_AT_EAST, 99, 101);
  SolSanctumEntrance_RestoreRowStatueCells(0);
}

/// Sol Sanctum, the second relief room: as SolSanctumEntrance_RowStatue1Moved,
/// for the row's second statue (ACTOR_ROW_STATUE_2).
static void SolSanctumEntrance_RowStatue2Moved(void) {
  struct Actor *statue = Actor_GetById(ACTOR_ROW_STATUE_2);
  s32 column;

  if (statue == NULL) {
    return;
  }
  column = statue->x.fixed >> 20;
  SolSanctumEntrance_RecordStatueColumn(
      column, FLAG_SOL_SANCTUM_ROW_STATUE_2_AT_EAST, 103, 105);
  SolSanctumEntrance_RestoreRowStatueCells(0);
}

/// Sol Sanctum, the second relief room: as SolSanctumEntrance_RowStatue1Moved,
/// for the row's third statue (ACTOR_ROW_STATUE_3).
static void SolSanctumEntrance_RowStatue3Moved(void) {
  struct Actor *statue = Actor_GetById(ACTOR_ROW_STATUE_3);
  s32 column;

  if (statue == NULL) {
    return;
  }
  column = statue->x.fixed >> 20;
  SolSanctumEntrance_RecordStatueColumn(
      column, FLAG_SOL_SANCTUM_ROW_STATUE_3_AT_EAST, 107, 109);
  SolSanctumEntrance_RestoreRowStatueCells(0);
}

/// Sol Sanctum, the second relief room: as SolSanctumEntrance_RowStatue1Moved,
/// for the row's fourth statue (ACTOR_ROW_STATUE_4).
static void SolSanctumEntrance_RowStatue4Moved(void) {
  struct Actor *statue = Actor_GetById(ACTOR_ROW_STATUE_4);
  s32 column;

  if (statue == NULL) {
    return;
  }
  column = statue->x.fixed >> 20;
  SolSanctumEntrance_RecordStatueColumn(
      column, FLAG_SOL_SANCTUM_ROW_STATUE_4_AT_EAST, 111, 113);
  SolSanctumEntrance_RestoreRowStatueCells(0);
}

/// Sol Sanctum, the second relief room: as SolSanctumEntrance_RowStatue1Moved,
/// for the row's fifth statue (ACTOR_ROW_STATUE_5).
static void SolSanctumEntrance_RowStatue5Moved(void) {
  struct Actor *statue = Actor_GetById(ACTOR_ROW_STATUE_5);
  s32 column;

  if (statue == NULL) {
    return;
  }
  column = statue->x.fixed >> 20;
  SolSanctumEntrance_RecordStatueColumn(
      column, FLAG_SOL_SANCTUM_ROW_STATUE_5_AT_EAST, 115, 117);
  SolSanctumEntrance_RestoreRowStatueCells(0);
}

/// Sol Sanctum, the second relief room: as SolSanctumEntrance_RowStatue1Moved,
/// for the row's sixth statue (ACTOR_ROW_STATUE_6).
static void SolSanctumEntrance_RowStatue6Moved(void) {
  struct Actor *statue = Actor_GetById(ACTOR_ROW_STATUE_6);
  s32 column;

  if (statue == NULL)
    return;
  column = statue->x.fixed >> 20;
  SolSanctumEntrance_RecordStatueColumn(
      column, FLAG_SOL_SANCTUM_ROW_STATUE_6_AT_EAST, 119, 121);
  SolSanctumEntrance_RestoreRowStatueCells(0);
}

#include "shared/actor_findcell.h"

/// The party has entered the second relief room, which turns off Move on the
/// first relief room's statues.
/// ★ Named from the game: "It's a relief of a minotaur."
/// (MsgSolSanctumMinotaurReliefOneEye)
#define FLAG_SOL_SANCTUM_IN_SECOND_RELIEF_ROOM FLAG_SCENE_TEMP_34

/// Sol Sanctum, the two relief rooms: walking into a statue's cell
/// pushes it one cell, the leader behind it, if nothing is in the way, and its
/// column is saved through the room's Moved events.
static void SolSanctumEntrance_PushStatue(void) {
  struct Actor *leader;
  struct Actor *statue;
  s32 stoppedVelocity;
  s32 packedStep;
  u32 direction;
  struct FieldPosition target;
  struct FieldPosition *destination;

  // Find the statue one map cell ahead in the leader's facing direction.
  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  direction = leader->facing >> 12;
  statue = Actor_FindAtCell(
      (leader->x.part.pixel + (gSolSanctumPushSteps[direction] >> 16)) >> 4,
      (leader->z.part.pixel + (s16)gSolSanctumPushSteps[direction]) >> 4);
  if (statue != NULL) {
    stoppedVelocity = 0;
    statue->layer = 2;
    destination = &target;
    packedStep = gSolSanctumPushSteps[direction];
    SET_POSITION(destination, statue->x.fixed + (packedStep & -0x10000),
                 statue->y.fixed, statue->z.fixed + (packedStep << 16));
    // Move only after the statue's proposed position passes collision.
    if (FieldActor_CheckMovementCollision(statue, destination) <= 0) {
      Actor_SelectAnimation(leader, ANIM_PUSH);
      wait(15);
      Sound_Play(SE_PUSH);
      statue->speedLimit = PIXELS(1) / 5;
      statue->acceleration = PIXELS(1) / 5;
      FieldActor_SetMoveTarget(statue, destination->x, destination->y,
                               destination->z);
      leader->speedLimit = PIXELS(1) / 5;
      leader->acceleration = PIXELS(1) / 5;
      FieldActor_SetMoveTarget(leader, destination->x, destination->y,
                               destination->z);
      Actor_WaitForMove(statue);
      statue->x.fixed = destination->x;
      statue->z.fixed = destination->z;
      statue->velocityX = stoppedVelocity;
      statue->velocityZ = stoppedVelocity;
      Actor_SelectAnimation(leader, ANIM_STAND);
      // Save the moved statue in the flag set belonging to this room.
      switch (gAdventure.entrance) {
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

/// Sets up Sol Entrance or Sol Sanctum. Called by SceneSequence_Run once the
/// actors exist.
s32 SolSanctumEntrance_Initialize(void) {
  s32 sceneId = gAdventure.sceneId;

  if (sceneId == (s32)&SceneId_SolEntrance) {
    SolSanctumEntrance_ApplyEntryState();
  } else if (sceneId == (s32)&SceneId_SolSanctumReliefs) {
    SolSanctumEntrance_RestoreInteriorState();
  }
  return 0;
}

/// Sets up Sol Entrance (SceneId_SolEntrance): Retreat is blocked
/// (FLAG_RETREAT_BLOCKED), and after the eruption
/// SolSanctumEntrance_UpdateEruptionRumbles shakes the ground now and then.
/// Once Isaac and Garet are back in Vale (FLAG_VALE_RETURNED_FROM_MT_ALEPH),
/// the door is locked: its row becomes CELL_BEHAVIOR_SOL_ENTRANCE_LOCKED_DOOR
/// and the cells before it plain ground; once the party has left Vale
/// (FLAG_VALE_PARTY_LEFT), the
/// man who warns everyone away, placed off the map until then, stands in the
/// path and the three cells across it change.
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
    Actor_SetPosition(ACTOR_WARNING_MAN, PIXELS(120), PIXELS(232));
    MapGrid_CopyCellAttributes(2, 10, 1, 1, 6, 14);
    MapGrid_CopyCellAttributes(2, 10, 1, 1, 7, 14);
    MapGrid_CopyCellAttributes(2, 10, 1, 1, 8, 14);
  }
}

/// Sets up Sol Sanctum (SceneId_SolSanctumReliefs): after the eruption its
/// rooms rumble and shake. SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_CORRIDOR_WEST
/// and SOL_SANCTUM_RELIEFS_CHEST_ROOM_FROM_FIRST_RELIEF_ROOM show the first
/// relief's passage once it is open; SOL_SANCTUM_RELIEFS_FROM_SOL_ENTRANCE, the
/// way in, makes itself the Retreat point and plays Kraden's welcome until
/// FLAG_SOL_SANCTUM_ENTERED is set. In the first relief room Kraden's talk of
/// the hidden passage plays until FLAG_SEARCHING_FOR_HIDDEN_PASSAGE is set, and
/// the two statues go back to their saved columns; in the second relief room
/// Kraden sees the row of statues until FLAG_SOL_SANCTUM_MORE_STATUES_SEEN is
/// set, the six go back to their columns,
/// FLAG_SOL_SANCTUM_IN_SECOND_RELIEF_ROOM is set and the second relief's
/// doorway shows once it is open.
static void SolSanctumEntrance_RestoreInteriorState(void) {
  s32 entrance;
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  if (GameFlag_Get(FLAG_SOL_SANCTUM_ERUPTED) != 0) {
    Scene_QueueMusic(SE_RUMBLE);
    MapGrid_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
    FieldEffect_InitializeSparkles();
  }

  entrance = gAdventure.entrance;
  switch (entrance) {
  case SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_CORRIDOR_WEST:
  case SOL_SANCTUM_RELIEFS_CHEST_ROOM_FROM_FIRST_RELIEF_ROOM:
    if (GameFlag_Get(FLAG_SOL_SANCTUM_FIRST_SMALL_JEWEL_SET) != 0) {
      MapGrid_CopyCellsTo(1, 109, 4, 81, 1, 1);
      MapGrid_CopyCellsTo(0, 70, 30, 42, 1, 1);
      MapGrid_CopyCellsTo(0, 29, 3, 1, 3, 2);
      MapGrid_CopyCellAttributes(0, 29, 3, 2, 3, 1);
      MapGrid_CenterOnFollowedPoint();
    }
    break;

  case SOL_SANCTUM_RELIEFS_STATUE_NOOK_FROM_FIRST_RELIEF_ROOM:
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_STATUE), 0);
    break;

  case SOL_SANCTUM_RELIEFS_FROM_SOL_ENTRANCE:
    gAdventure.retreatScene = (s32)&SceneId_SolSanctumReliefs;
    gAdventure.retreatEntrance = SOL_SANCTUM_RELIEFS_FROM_SOL_ENTRANCE;
    if (GameFlag_Get(FLAG_SOL_SANCTUM_ENTERED) == 0)
      SolSanctumEntrance_IntroduceSanctum();
    break;

  case SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_FROM_CHEST_ROOM:
  case SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_FROM_STATUE_NOOK:
  case SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_FROM_STEPS_ROOM:
    // Put back this room's statues after its introduction has played.
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_WEST_STATUE), 0);
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_EAST_STATUE), 0);
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_FIRST_FIXED_STATUE), 0);
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_FIRST_FIXED_STATUE + 1), 0);
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_FIRST_FIXED_STATUE + 2), 0);
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_FIRST_FIXED_STATUE + 3), 0);
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_FIRST_FIXED_STATUE + 4), 0);
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_FIRST_FIXED_STATUE + 5), 0);
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_FIRST_FIXED_STATUE + 6), 0);
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_FIRST_FIXED_STATUE + 7), 0);
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_FIRST_FIXED_STATUE + 8), 0);
    if (GameFlag_Get(FLAG_SEARCHING_FOR_HIDDEN_PASSAGE) == 0)
      SolSanctumEntrance_KradenSuspectsHiddenPassage();
    if (GameFlag_Get(FLAG_SOL_SANCTUM_WEST_STATUE_AT_WEST) != 0)
      Actor_SetPosition(ACTOR_WEST_STATUE, PIXELS(1496), PIXELS(136));
    else if (GameFlag_Get(FLAG_SOL_SANCTUM_WEST_STATUE_AT_EAST) != 0)
      Actor_SetPosition(ACTOR_WEST_STATUE, PIXELS(1528), PIXELS(136));
    if (GameFlag_Get(FLAG_SOL_SANCTUM_EAST_STATUE_AT_WEST) != 0)
      Actor_SetPosition(ACTOR_EAST_STATUE, PIXELS(1816), PIXELS(136));
    else if (GameFlag_Get(FLAG_SOL_SANCTUM_EAST_STATUE_AT_EAST) != 0)
      Actor_SetPosition(ACTOR_EAST_STATUE, PIXELS(1848), PIXELS(136));
    break;

  case SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_MAZE_EAST:
  case SOL_SANCTUM_RELIEFS_SECOND_RELIEF_ROOM_FROM_ROUND_PASSAGE:
  case SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS:
    // Restore the row of statues and the second gem's doorway.
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_ROW_STATUE_1), 0);
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_ROW_STATUE_2), 0);
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_ROW_STATUE_3), 0);
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_ROW_STATUE_4), 0);
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_ROW_STATUE_5), 0);
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_ROW_STATUE_6), 0);
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

/// Sol Sanctum, the first time the party comes in through the door: Kraden,
/// Garet and Jenna step out of the leader, and Kraden asks whether this is the
/// party's first time here. "This is the entrance to Sol Sanctum." Yes: "Take a
/// good look at it." No: "I doubt that..." Called by
/// SolSanctumEntrance_RestoreInteriorState at
/// SOL_SANCTUM_RELIEFS_FROM_SOL_ENTRANCE until FLAG_SOL_SANCTUM_ENTERED is
/// set, which it sets; clears FLAG_SHOW_AREA_NAME.
static void SolSanctumEntrance_IntroduceSanctum(void) {
  struct Actor *leader;

  Event_Start();
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Event_StartScreenTransition();
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STILL);
  Event_Wait(4);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  // Gather the followers around the leader for Kraden's introduction.
  FieldCamera_SetSpeed(PIXELS(3) / 5, PIXELS(3) / 40);
  FieldCamera_MoveTo(PIXELS(1224), -1, PIXELS(136), 1);
  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetPosition(ACTOR_KRADEN, leader->x.fixed, leader->z.fixed);
  }
  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetPosition(ACTOR_JENNA, leader->x.fixed, leader->z.fixed);
  }
  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetPosition(ACTOR_GARET, leader->x.fixed, leader->z.fixed);
  }
  Actor_SetSpeed(ACTOR_KRADEN, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_SetSpeed(ACTOR_JENNA, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(3) / 5, PIXELS(3) / 10);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_WALK);
  Import_ActorSetAnimation(ACTOR_JENNA, ANIM_WALK);
  Import_ActorSetAnimation(ACTOR_KRADEN, ANIM_WALK);
  Actor_SetDestinationOffset(ACTOR_GARET, -16, 0);
  Actor_SetDestinationOffset(ACTOR_JENNA, 16, 0);
  Actor_SetDestinationOffset(ACTOR_KRADEN, 0, -32);
  Actor_WaitForMoveById(ACTOR_GARET);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_STILL);
  Import_ActorSetAnimation(ACTOR_JENNA, ANIM_STILL);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
  Actor_WaitForMoveById(ACTOR_KRADEN);
  Import_ActorSetAnimation(ACTOR_KRADEN, ANIM_STAND);
  Event_Wait(40);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST + FACING_STEP, 40);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH + FACING_STEP, 40);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST + FACING_STEP, 20);
  Actor_Jump(ACTOR_KRADEN, 4, 20);
  // Kraden: "This is your first time here, right?" Yes: "Take a good look at
  // it." No: "I doubt that..."
  Event_SetMessageId(MsgSolSanctumKradenFirstTimeAtSol);
  Event_AskYesNo(SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 0);
  Event_Wait(20);
  FieldCamera_MoveTo(PIXELS(1224), -1, PIXELS(148), 1);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_WALK);
  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetDestination(ACTOR_GARET, leader->x.part.pixel,
                         leader->z.part.pixel);
  }
  Import_ActorSetAnimation(ACTOR_JENNA, ANIM_WALK);
  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetDestination(ACTOR_JENNA, leader->x.part.pixel,
                         leader->z.part.pixel);
  }
  Import_ActorSetAnimation(ACTOR_KRADEN, ANIM_WALK);
  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetDestination(ACTOR_KRADEN, leader->x.part.pixel,
                         leader->z.part.pixel);
  }
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_SetPosition(ACTOR_GARET, 0, 0);
  Actor_SetPosition(ACTOR_JENNA, 0, 0);
  Actor_WaitForMoveById(ACTOR_KRADEN);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_STAND);
  Import_ActorSetAnimation(ACTOR_JENNA, ANIM_STAND);
  Import_ActorSetAnimation(ACTOR_KRADEN, ANIM_STAND);
  GameFlag_Set(FLAG_SOL_SANCTUM_ENTERED);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  GameFlag_Clear(FLAG_SHOW_AREA_NAME);
  Event_End();
}

/// Sol Sanctum, the first time the party enters the first relief
/// room: Kraden looks round and recalls that Saturos and Menardi spoke of a
/// hidden passage leading deeper into the sanctum; a sanctum built in honour
/// of the sun ought to show it somewhere. Garet asks whether to help him, and
/// either way the four stay together. Called by
/// SolSanctumEntrance_RestoreInteriorState there until
/// FLAG_SEARCHING_FOR_HIDDEN_PASSAGE is set, which it sets; clears
/// FLAG_SHOW_AREA_NAME.
static void SolSanctumEntrance_KradenSuspectsHiddenPassage(void) {
  struct Actor *leader;

  Event_Start();
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();

  // Spread the party out while Kraden examines the room.
  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (leader != NULL)
    Import_ActorSetPosition(ACTOR_KRADEN, leader->x.fixed, leader->z.fixed);
  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (leader != NULL)
    Import_ActorSetPosition(ACTOR_JENNA, leader->x.fixed, leader->z.fixed);
  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (leader != NULL)
    Import_ActorSetPosition(ACTOR_GARET, leader->x.fixed, leader->z.fixed);

  Actor_SetSpeed(ACTOR_KRADEN, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_SetSpeed(ACTOR_JENNA, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_SetSpeed(ACTOR_GARET, PIXELS(3) / 5, PIXELS(3) / 10);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_WALK);
  Import_ActorSetAnimation(ACTOR_JENNA, ANIM_WALK);
  Import_ActorSetAnimation(ACTOR_KRADEN, ANIM_WALK);
  Actor_SetDestinationOffset(ACTOR_GARET, -16, 0);
  Actor_SetDestinationOffset(ACTOR_JENNA, 16, 0);
  Actor_SetDestinationOffset(ACTOR_KRADEN, 0, -16);
  Actor_WaitForMoveById(ACTOR_KRADEN);
  Import_ActorSetAnimation(ACTOR_KRADEN, ANIM_STAND);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STILL);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_STILL);
  Import_ActorSetAnimation(ACTOR_JENNA, ANIM_STILL);
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
  Import_ActorSetAnimation(ACTOR_KRADEN, ANIM_WALK);
  Actor_OffsetPositionAndResetMotion(ACTOR_KRADEN, 0, -16);
  Actor_WaitForMoveById(ACTOR_KRADEN);
  Import_ActorSetAnimation(ACTOR_KRADEN, ANIM_STAND);
  Event_Wait(6);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 40);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  Event_Wait(20);
  Import_ActorSetAnimation(ACTOR_KRADEN, ANIM_WALK);
  Actor_OffsetPositionAndResetMotion(ACTOR_KRADEN, 0, -32);
  Actor_WaitForMoveById(ACTOR_KRADEN);
  Import_ActorSetAnimation(ACTOR_KRADEN, ANIM_STAND);

  // Survey both sides of the room before returning to Kraden.
  FieldCamera_SetSpeed(PIXELS(2), PIXELS(1) / 4);
  FieldCamera_MoveTo(PIXELS(1585), -1, PIXELS(150), 1);
  FieldCamera_WaitForArrival();
  Event_Wait(10);
  FieldCamera_SetSpeed(PIXELS(6) / 5, PIXELS(3) / 20);
  FieldCamera_MoveTo(PIXELS(1621), -1, PIXELS(100), 1);
  FieldCamera_WaitForArrival();
  FieldCamera_MoveTo(PIXELS(1718), -1, PIXELS(100), 1);
  FieldCamera_WaitForArrival();
  Import_ActorSetAnimation(ACTOR_KRADEN, ANIM_STAND);
  FieldCamera_MoveTo(PIXELS(1752), -1, PIXELS(150), 1);
  FieldCamera_WaitForArrival();
  Event_Wait(40);
  FieldCamera_SetSpeed(PIXELS(12) / 5, PIXELS(3) / 10);
  FieldCamera_MoveTo(PIXELS(1668), -1, PIXELS(256), 1);
  FieldCamera_WaitForArrival();
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Event_Wait(10);

  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 10);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  // Garet: "What's wrong, Kraden?"
  Event_SetMessageId(MsgSolSanctumWrongKraden);
  FieldEvent_ShowMessageThenWait(ACTOR_GARET, 0, 10);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Actor_StartRepeatedMotion(ACTOR_KRADEN, 2);
  // Kraden: "Those two said there was a hidden passage leading deeper into
  // the sanctum."
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
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
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  Actor_TurnToFaceEachOther(ACTOR_PARTY_LEADER, 5, 0);
  Event_Wait(40);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 1);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 20);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  // Jenna: "What do you mean?"
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  // Kraden: a sanctum built in honour of the sun ought to show it somewhere.
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST + FACING_STEP, 10);
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 40);
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
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH, 20);
  Actor_Jump(ACTOR_KRADEN, 2, 20);

  // Kraden hops: "Let's see if we can find it!"
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 40);
  Actor_FaceDirection(ACTOR_GARET, FACING_EAST, 20);
  // Garet: "I think he's serious! Should we help him?"
  Event_OpenMessage(ACTOR_GARET, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
    // Yes: Garet says it is too dangerous to split up; they stay together.
    Event_SetMessageId(MsgSolSanctumDangerousSplitStay);
    Actor_RunRepeatedMotion(ACTOR_GARET, 1);
    Event_ShowMessageThenWait(ACTOR_GARET, 0, 10);
  } else {
    // No: Jenna objects to turning back now; Isaac nods and Garet agrees to
    // help Kraden.
    Event_SetMessageId(MsgSolSanctumCantSeriouslyWant);
    Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 20);
    FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
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
    Event_ShowMessageThenWait(ACTOR_GARET, 0, 10);
  }

  // Bring Kraden back and return the followers to the party.
  Actor_SetSpeed(ACTOR_KRADEN, PIXELS(3) / 5, PIXELS(3) / 10);
  Import_ActorSetAnimation(ACTOR_KRADEN, ANIM_WALK);
  Actor_OffsetPositionAndResetMotion(ACTOR_KRADEN, 0, 48);
  Actor_WaitForMoveById(ACTOR_KRADEN);
  Import_ActorSetAnimation(ACTOR_KRADEN, ANIM_STAND);
  Event_Wait(6);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
  Actor_ArmCallback(ACTOR_JENNA, FACING_NORTHWEST, 0);
  Actor_WaitForMoveById(ACTOR_KRADEN);
  Import_ActorSetAnimation(ACTOR_KRADEN, ANIM_STAND);
  Event_Wait(20);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(6);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_WALK);

  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (leader != NULL)
    Actor_ResetAndSetPosition(ACTOR_GARET, leader->x.part.pixel,
                              leader->z.part.pixel);
  Import_ActorSetAnimation(ACTOR_JENNA, ANIM_WALK);
  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (leader != NULL)
    Actor_ResetAndSetPosition(ACTOR_JENNA, leader->x.part.pixel,
                              leader->z.part.pixel);
  Import_ActorSetAnimation(ACTOR_KRADEN, ANIM_WALK);
  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (leader != NULL)
    Actor_ResetAndSetPosition(ACTOR_KRADEN, leader->x.part.pixel,
                              leader->z.part.pixel);

  Actor_WaitForMoveById(ACTOR_KRADEN);
  Actor_SetPosition(ACTOR_GARET, 0, 0);
  Actor_SetPosition(ACTOR_JENNA, 0, 0);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  Import_ActorSetAnimation(ACTOR_KRADEN, ANIM_STAND);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_STAND);
  Import_ActorSetAnimation(ACTOR_JENNA, ANIM_STAND);
  GameFlag_Set(FLAG_SEARCHING_FOR_HIDDEN_PASSAGE);
  GameFlag_Clear(FLAG_SHOW_AREA_NAME);
  Event_End();
}

/// Sol Sanctum, the first time the party enters the second relief
/// room: the camera pans along the row of statues, and Kraden: "More
/// statues... It doesn't look like we can reach them from here." He asks
/// Isaac to try his Psynergy. Called by SolSanctumEntrance_RestoreInteriorState
/// there until FLAG_SOL_SANCTUM_MORE_STATUES_SEEN is set, which it sets.
static void SolSanctumEntrance_KradenSeesStatuesOutOfReach(void) {
  struct Actor *leader;

  Event_Start();
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(20);
  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetPosition(ACTOR_KRADEN, leader->x.fixed, leader->z.fixed);
  }
  Actor_SetSpeed(ACTOR_KRADEN, PIXELS(1), PIXELS(1) / 2);
  Import_ActorSetAnimation(ACTOR_KRADEN, ANIM_WALK);
  Actor_SetDestinationOffset(ACTOR_KRADEN, 24, -10);
  Actor_WaitForMoveById(ACTOR_KRADEN);
  Import_ActorSetAnimation(ACTOR_KRADEN, ANIM_STAND);
  Event_Wait(6);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 40);
  FieldCamera_SetSpeed(PIXELS(12) / 5, PIXELS(3) / 10);
  // Pan across the statues, then return to the party's position.
  FieldCamera_MoveTo(PIXELS(1672), -1, PIXELS(524), 1);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  FieldCamera_SetSpeed(PIXELS(8) / 5, PIXELS(1) / 5);
  FieldCamera_MoveTo(PIXELS(1880), -1, PIXELS(524), 1);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  FieldCamera_SetSpeed(PIXELS(16) / 5, PIXELS(2) / 5);
  FieldCamera_MoveTo(PIXELS(1769), -1, PIXELS(548), 1);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 30);
  // Kraden: "More statues... It doesn't look like we can reach them".
  Event_SetMessageId(MsgSolSanctumMoreStatuesOutOfReach);
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 0, 10);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 1);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH + FACING_STEP, 20);
  // Kraden: "Perhaps Psynergy is the key..."
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 0, 10);
  Import_ActorSetAnimation(ACTOR_KRADEN, ANIM_WALK);
  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetDestination(ACTOR_KRADEN, leader->x.part.pixel,
                         leader->z.part.pixel);
  }
  Actor_WaitForMoveById(ACTOR_KRADEN);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  GameFlag_Set(FLAG_SOL_SANCTUM_MORE_STATUES_SEEN);
  Event_End();
}

/// Sol Sanctum, the second relief room: sets the cells the six statues
/// start on, one cell behavior while a statue stands there and another once
/// it has been pushed to either side, and, when placeActors is nonzero
/// (on entry), moves the statues to their saved columns.
static void SolSanctumEntrance_RestoreRowStatueCells(s32 placeActors) {
  // Clear all six destinations before restoring occupied cells.
  MapGrid_CopyCellAttributes(122, 20, 1, 1, 100, 32);
  MapGrid_CopyCellAttributes(122, 20, 1, 1, 104, 32);
  MapGrid_CopyCellAttributes(122, 20, 1, 1, 108, 32);
  MapGrid_CopyCellAttributes(122, 20, 1, 1, 112, 32);
  MapGrid_CopyCellAttributes(122, 20, 1, 1, 116, 32);
  MapGrid_CopyCellAttributes(122, 20, 1, 1, 120, 32);
  if (GameFlag_Get(FLAG_SOL_SANCTUM_ROW_STATUE_1_AT_WEST) != 0) {
    MapGrid_CopyCellAttributes(121, 20, 1, 1, 100, 32);
    if (placeActors != 0) {
      Actor_SetPosition(ACTOR_ROW_STATUE_1, PIXELS(1592), PIXELS(520));
    }
  } else {
    if (GameFlag_Get(FLAG_SOL_SANCTUM_ROW_STATUE_1_AT_EAST) != 0) {
      MapGrid_CopyCellAttributes(121, 20, 1, 1, 100, 32);
      if (placeActors != 0) {
        Actor_SetPosition(ACTOR_ROW_STATUE_1, PIXELS(1624), PIXELS(520));
      }
    }
  }
  if (GameFlag_Get(FLAG_SOL_SANCTUM_ROW_STATUE_2_AT_WEST) != 0) {
    MapGrid_CopyCellAttributes(121, 20, 1, 1, 104, 32);
    if (placeActors != 0) {
      Actor_SetPosition(ACTOR_ROW_STATUE_2, PIXELS(1656), PIXELS(520));
    }
  } else {
    if (GameFlag_Get(FLAG_SOL_SANCTUM_ROW_STATUE_2_AT_EAST) != 0) {
      MapGrid_CopyCellAttributes(121, 20, 1, 1, 104, 32);
      if (placeActors != 0) {
        Actor_SetPosition(ACTOR_ROW_STATUE_2, PIXELS(1688), PIXELS(520));
      }
    }
  }
  if (GameFlag_Get(FLAG_SOL_SANCTUM_ROW_STATUE_3_AT_WEST) != 0) {
    MapGrid_CopyCellAttributes(121, 20, 1, 1, 108, 32);
    if (placeActors != 0) {
      Actor_SetPosition(ACTOR_ROW_STATUE_3, PIXELS(1720), PIXELS(520));
    }
  } else {
    if (GameFlag_Get(FLAG_SOL_SANCTUM_ROW_STATUE_3_AT_EAST) != 0) {
      MapGrid_CopyCellAttributes(121, 20, 1, 1, 108, 32);
      if (placeActors != 0) {
        Actor_SetPosition(ACTOR_ROW_STATUE_3, PIXELS(1752), PIXELS(520));
      }
    }
  }
  if (GameFlag_Get(FLAG_SOL_SANCTUM_ROW_STATUE_4_AT_WEST) != 0) {
    MapGrid_CopyCellAttributes(121, 20, 1, 1, 112, 32);
    if (placeActors != 0) {
      Actor_SetPosition(ACTOR_ROW_STATUE_4, PIXELS(1784), PIXELS(520));
    }
  } else {
    if (GameFlag_Get(FLAG_SOL_SANCTUM_ROW_STATUE_4_AT_EAST) != 0) {
      MapGrid_CopyCellAttributes(121, 20, 1, 1, 112, 32);
      if (placeActors != 0) {
        Actor_SetPosition(ACTOR_ROW_STATUE_4, PIXELS(1816), PIXELS(520));
      }
    }
  }
  if (GameFlag_Get(FLAG_SOL_SANCTUM_ROW_STATUE_5_AT_WEST) != 0) {
    MapGrid_CopyCellAttributes(121, 20, 1, 1, 116, 32);
    if (placeActors != 0) {
      Actor_SetPosition(ACTOR_ROW_STATUE_5, PIXELS(1848), PIXELS(520));
    }
  } else {
    if (GameFlag_Get(FLAG_SOL_SANCTUM_ROW_STATUE_5_AT_EAST) != 0) {
      MapGrid_CopyCellAttributes(121, 20, 1, 1, 116, 32);
      if (placeActors != 0) {
        Actor_SetPosition(ACTOR_ROW_STATUE_5, PIXELS(1880), PIXELS(520));
      }
    }
  }
  if (GameFlag_Get(FLAG_SOL_SANCTUM_ROW_STATUE_6_AT_WEST) != 0) {
    MapGrid_CopyCellAttributes(121, 20, 1, 1, 120, 32);
    if (placeActors != 0) {
      Actor_SetPosition(ACTOR_ROW_STATUE_6, PIXELS(1912), PIXELS(520));
    }
  } else {
    if (GameFlag_Get(FLAG_SOL_SANCTUM_ROW_STATUE_6_AT_EAST) != 0) {
      MapGrid_CopyCellAttributes(121, 20, 1, 1, 120, 32);
      if (placeActors != 0) {
        Actor_SetPosition(ACTOR_ROW_STATUE_6, PIXELS(1944), PIXELS(520));
      }
    }
  }
}

/// Sol Entrance: searching the other door . "The door is
/// tightly locked."
static void SolSanctumEntrance_DoorTightlyLocked(void) {
  Event_Start();
  FieldMessage_Show(MsgFieldDoorTightlyLocked, 1);
  Event_End();
}

/// Sol Entrance after the eruption: the ground rumbles now and then, and each
/// shake fades. Run each frame as a task that
/// SolSanctumEntrance_ApplyEntryState adds once FLAG_SOL_SANCTUM_ERUPTED is
/// set.
static void SolSanctumEntrance_UpdateEruptionRumbles(void) {
  s32 randomValue;
  s32 *timer;
  s32 remainingFrames;

  timer = &gSolSanctumEntrance_RumbleTimer;
  if (*timer != 0) {
    remainingFrames = (*timer - 1);
    *timer = (*timer - 1);
    if (remainingFrames == 40) {
      MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
    }
  } else {
    randomValue = Import_RandomNext();
    if (((u32)(((randomValue << 4) - randomValue) << 3) >> 16) == 0) {
      Event_PlaySound(SE_ITEM_BREAK);
      MapGrid_SetShake(PIXELS(1), PIXELS(2), Q16_ONE);
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
    { ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0 },
    { ACTOR_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0 },
    { SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0 },
    { SPRITE_STONE_STATUE, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(744), 0, PIXELS(56), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SCENE_TABLE_END },
};

struct ActorPlacement gSolSanctumReliefs_FirstReliefRoomActorPlacements[] = {
    { ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0 },
    { ACTOR_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0 },
    { SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0 },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1512), 0, PIXELS(136), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1832), 0, PIXELS(136), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1560), 0, PIXELS(136), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1784), 0, PIXELS(136), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1640), 0, PIXELS(264), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1640), 0, PIXELS(200), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1640), 0, PIXELS(136), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1672), 0, PIXELS(88), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1704), 0, PIXELS(136), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1704), 0, PIXELS(200), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1704), 0, PIXELS(264), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SCENE_TABLE_END },
};

struct ActorPlacement gSolSanctumReliefs_SecondReliefRoomActorPlacements[] = {
    { ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0 },
    { ACTOR_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0 },
    { SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0 },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1608), 0, PIXELS(520), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1672), 0, PIXELS(520), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1736), 0, PIXELS(520), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1800), 0, PIXELS(520), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1864), 0, PIXELS(520), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1928), 0, PIXELS(520), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SCENE_TABLE_END },
};

struct ActorPlacement gSolEntrance_ActorPlacements[] = {
    { SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct Trigger gSolSanctumEntrance_FallbackTriggers[] = {
    { TRIGGERS_END },
};

struct Trigger gSolEntrance_Triggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, SOL_ENTRANCE_TO_VALE },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_SOL_ENTRANCE_DOOR, CONDITION_ALWAYS, EVENT(SolSanctumEntrance_OpenDoorAndEnter) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_SOL_ENTRANCE_LOCKED_DOOR, CONDITION_ALWAYS, EVENT(SolSanctumEntrance_DoorTightlyLocked) },
    { TRIGGER_TALK, ACTOR_WARNING_MAN, CONDITION_ALWAYS, MSGNO(MsgValeItIsVeryDangerousToGo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WARNING_MAN, CONDITION_ALWAYS, MSGNO(MsgValeIAmAllAloneInThis) },
    { TRIGGERS_END },
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
    { TRIGGERS_END },
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
    { TRIGGERS_END },
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
    { TRIGGERS_END },
};
// clang-format on

/// Packs a push step: X in the upper halfword, Z in the lower (pixels).
#define PUSH_STEP(x, z) (((x) << 16) | ((z) & 0xffff))

s32 gSolSanctumPushSteps[] = {
    PUSH_STEP(16, 0),  PUSH_STEP(16, 0),  PUSH_STEP(16, 0),  PUSH_STEP(0, 16),
    PUSH_STEP(0, 16),  PUSH_STEP(0, 16),  PUSH_STEP(0, 16),  PUSH_STEP(-16, 0),
    PUSH_STEP(-16, 0), PUSH_STEP(-16, 0), PUSH_STEP(-16, 0), PUSH_STEP(0, -16),
    PUSH_STEP(0, -16), PUSH_STEP(0, -16), PUSH_STEP(0, -16), PUSH_STEP(16, 0),
};
