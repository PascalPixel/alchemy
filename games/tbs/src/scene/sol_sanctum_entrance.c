/// @file
/// Sol Entrance and the first rooms of Sol Sanctum, two scenes one overlay
/// serves, chosen by gGameState.scene in every entry point.
///
/// - Sol Entrance (SceneId_SolEntrance), the mountainside before the
///   sanctum's door, reached from Vale (VALE_TO_SOL_ENTRANCE). Walking north
///   into the door opens it and takes the leader in (Event_OpenDoorAndEnter);
///   searching the other door finds it "tightly locked". Retreat does not work
///   here. The way down leads to SceneId_Vale, except between the eruption
///   (FLAG_SOL_SANCTUM_ERUPTED) and the party leaving Vale
///   (FLAG_PARTY_LEFT_VALE), when it leads to SceneId_ValeEruption. After the
///   eruption the ground rumbles now and then; once the party has left Vale a
///   man stands in the path and warns everyone away.
/// - Sol Sanctum (SceneId_SolSanctumReliefs), the rooms inside the door, which
///   Isaac, Garet and Jenna explore with Kraden. Two minotaur reliefs each have
///   an empty eye socket, and a Small Jewel set in it opens the way on: the
///   first from this scene's chest (FLAG_SOL_SANCTUM_FIRST_GEM_PLACED), the
///   second from the chest in SceneId_SolSanctumMaze
///   (FLAG_SOL_SANCTUM_SECOND_GEM_PLACED). Kraden has a scene on the first
///   entry at the door (SolSanctumEntrance_IntroduceSanctum), in the first
///   relief room (SolSanctumEntrance_KradenSuspectsHiddenPassage) and in the
///   second (SolSanctumEntrance_KradenSeesStatuesOutOfReach). Two statues in
///   the first relief room and the row of six in the second can be pushed, or
///   moved with Move, a cell east or west. Its exits lead to
///   SceneId_SolSanctumCorridor, SceneId_SolSanctumMaze, Sol Entrance and
///   SceneId_SolSanctumLunaSolRooms; the rest lead between its own rooms.
///
/// SceneId_SolSanctumReliefs is one map, but each relief room has placement
/// and event tables of its own: SolSanctumEntrance_GetPlacements and _GetEvents
/// choose them by the entrance the party came in at, so a room's statues and
/// their events exist only while the party is in it. Each pushable statue's
/// column is kept in two place flags, one for each side, which
/// SolSanctumEntrance_RestoreInteriorState reads to put it back on entry;
/// Scene_ResetFlagsOnEnter clears them when the party enters another place
/// group, so the statues are back at their starts on the next visit. Searching
/// a relief while its Small Jewel is held asks for the field menu as the search
/// ends, so that the jewel can be used on it at once.
///
/// Open questions:
/// - XXX: whether FLAG_SCENE_TEMP_34 is ever set while the first relief room's
///   table is read, since the two rooms are entered separately; a return to
///   the first relief room after a battle (FLAG_KEEP_PARTY_POSITION) would
///   settle it.
/// - XXX: what the six cells before the door become once Isaac and Garet are
///   back in Vale (FLAG_VALE_RETURNED_FROM_MT_ALEPH); likely cells that no
///   longer open the door, which walking north into it at that point would
///   settle.
/// - XXX: SE_181, SE_182, SE_183 and SE_185 have no proven names; a recording
///   of the door opening, a jewel being set and a statue being pushed would
///   settle them.

#include "game_state.h"
#include "unit/field/field.h"
#include "unit/import.h"
#include "unit/scene/sol_sanctum_entrance.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
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
#include "field_event.h"
#include "field_scene.h"
#include "field_service.h"
#include "sol_sanctum_entrance.h"
#include "scene_ids.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "field_obj.h"
#include "fixed_math.h"
#include "macros.h"
#include "songs.h"
#include "item_ids.h"

// Place flags: Scene_ResetFlagsOnEnter clears 0x300 to 0x3ff when the party
// enters another place group, and other places use the numbers for their own.

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

// Scene flags, cleared on every ordinary entrance:
// - FLAG_SCENE_TEMP_34: set on entering the second relief room; the
//   Move rows of the first relief room apply only while it is clear (see the
//   open questions above).

// An exit's flag word of -1 stands for no flag: GameFlag_TestFar reads it as
// flag 0xfff, which nothing sets, so the exit is always taken.
#define EXIT_FLAG_NONE 0xffffffff

/// The collision codes of the cells the two scenes' touch, search and item
/// events answer; codes 1 to 16 are the exits, in the exit table's order.
enum SolSanctumEntranceCell {
  /// ★ Named from what the player sees: the door that slides open as the
  /// leader walks into it (Event_OpenDoorAndEnter)
  SOL_ENTRANCE_CELL_DOOR = 2,
  /// ★ Named from the game: "The door is tightly locked."
  /// (MsgFieldDoorTightlyLocked)
  SOL_ENTRANCE_CELL_LOCKED_DOOR = 10,
  /// ★ Named from the game: "It's a relief of a minotaur."
  /// (MsgSolSanctumMinotaurReliefOneEye)
  SOL_SANCTUM_CELL_FIRST_RELIEF = 20,
  /// ★ Named from the game: "It's a relief of a minotaur."
  /// (MsgSolSanctumMinotaurReliefOneEye)
  SOL_SANCTUM_CELL_SECOND_RELIEF = 21,
  /// The first relief room's statues, pushed from any side.
  SOL_SANCTUM_CELL_FIRST_ROOM_STATUE = 22,
  /// The first relief room's statues, pushed east only.
  SOL_SANCTUM_CELL_FIRST_ROOM_STATUE_EAST = 23,
  /// The second relief room's row of statues.
  SOL_SANCTUM_CELL_ROW_STATUE = 24,
  /// The first relief room's statues, pushed west only.
  SOL_SANCTUM_CELL_FIRST_ROOM_STATUE_WEST = 25,
  SOL_SANCTUM_CELL_CHEST = 100
};

/// An item event's trigger bit that skips the `<actor> uses <item>.` message,
/// so the script shows its own (Field_UseItem).
#define SOL_SANCTUM_ITEM_OWN_MESSAGE 0x400

static void SolSanctumEntrance_ApplyEntryState(void);
static void SolSanctumEntrance_RestoreInteriorState(void);
static void SolSanctumEntrance_IntroduceSanctum(void);
static void SolSanctumEntrance_KradenSuspectsHiddenPassage(void);
static void SolSanctumEntrance_KradenSeesStatuesOutOfReach(void);
static void SolSanctumEntrance_RestoreRowStatueCells(s32 placeActors);
static void SolSanctumEntrance_UpdateEruptionRumbles(void);

s32 gSolSanctumEntrance_RumbleTimer __attribute__((section(".bss")));

void InitializeSceneRecordBuffer();

/// Returns Sol Entrance's entrances (SceneId_SolEntrance) or Sol Sanctum's
/// (SceneId_SolSanctumReliefs).
const struct SceneEntrance *SolSanctumEntrance_GetEntrances(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_SolEntrance) {
    return gSolEntrance_Entrances;
  }
  if (scene == (s32)&SceneId_SolSanctumReliefs) {
    return gSolSanctumReliefs_Entrances;
  }
  return gSolSanctumEntrance_FallbackEntrances;
}

const struct SceneRegion *SolSanctumEntrance_GetRegions(void) { return 0; }

const u32 *SolSanctumEntrance_GetExits(void) {
  return gSolSanctumEntrance_Exits;
}

/// Selects the actors for the place and, in Sol Sanctum, the room. The first
/// relief room's entrances place Kraden and that room's eleven statues, the
/// second relief room's Kraden and the row of six; the rest of Sol Sanctum
/// places Kraden and one statue (ACTOR_STATUE), and Sol Entrance the man who
/// warns everyone away (ACTOR_WARNING_MAN).
const struct ScenePlacement *SolSanctumEntrance_GetPlacements(void) {
  struct ScenePlacement *table;
  s32 firstStatueRoomEntrance =
      SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_FROM_CHEST_ROOM;

  if (gGameState.scene == (s32)&SceneId_SolSanctumReliefs) {
    if (gGameState.entrance >= firstStatueRoomEntrance) {
      if (gGameState.entrance >
          SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_FROM_STEPS_ROOM) {
        if (gGameState.entrance <=
            SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS) {
          return gSolSanctumReliefs_SecondReliefRoomPlacements;
        }
      } else {
        return gSolSanctumReliefs_FirstReliefRoomPlacements;
      }
    }
    table = gSolSanctumReliefs_Placements;
    FieldScene_PrepareActors(table);
    return table;
  }
  if (gGameState.scene == (s32)&SceneId_SolEntrance) {
    return gSolEntrance_Placements;
  }
  return gSolSanctumEntrance_FallbackPlacements;
}

/// Selects the events for the place and room, by the same entrances as
/// SolSanctumEntrance_GetPlacements.
const struct SceneEvent *SolSanctumEntrance_GetEvents(void) {
  s32 firstStatueRoomEntrance =
      SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_FROM_CHEST_ROOM;

  if (gGameState.scene == (s32)&SceneId_SolEntrance) {
    return gSolEntrance_Events;
  }
  if (gGameState.scene == (s32)&SceneId_SolSanctumReliefs) {
    if (gGameState.entrance >= firstStatueRoomEntrance) {
      if (gGameState.entrance >
          SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_FROM_STEPS_ROOM) {
        if (gGameState.entrance <=
            SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS) {
          return gSolSanctumReliefs_SecondReliefRoomEvents;
        }
      } else {
        return gSolSanctumReliefs_FirstReliefRoomEvents;
      }
    }
    return gSolSanctumReliefs_Events;
  }
  return gSolSanctumEntrance_FallbackEvents;
}

/// Sol Entrance: walking north into the sanctum's door slides it open, and
/// the leader walks in. Takes SOL_ENTRANCE_TO_SOL_SANCTUM_RELIEFS, into Sol
/// Sanctum at SOL_SANCTUM_RELIEFS_FROM_SOL_ENTRANCE.
static void Event_OpenDoorAndEnter(void) {
  s32 width;
  s32 height;

  Engine_EventBegin();
  // The door opens over three frames.
  width = 3;
  height = 2;
  Event_PlayCue(SE_181);
  Map_CopyCellsTo(16, 28, 21, 3, width, height);
  Engine_TaskWait(10);
  Map_CopyCellsTo(16, 30, 21, 3, width, height);
  Engine_TaskWait(10);
  Map_CopyCellsTo(16, 32, 21, 3, width, height);
  Engine_TaskWait(10);
  Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 2);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 120, 98);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -8);
  Engine_EventWait(10);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(SOL_ENTRANCE_TO_SOL_SANCTUM_RELIEFS);
  Engine_EventEnd();
}

/// Sol Sanctum: searching the first minotaur relief shows one eye sparkling, or
/// both once the Small Jewel is set: "The right eye is sparkling, but there's a
/// hole where the left eye should be." Once the first Small Jewel is found, and
/// until it is set, it requests the field menu (the event work's unk172), which
/// opens when the search ends so that the jewel can be used.
static void Event_InspectFirstMinotaurRelief(void) {

  Engine_EventBegin();

  if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_FIRST_GEM_PLACED) != 0) {
    Engine_MessageShowCentered(MsgSolSanctumMinotaurReliefBothEyes, 1);
  } else {
    Engine_MessageShowCentered(MsgSolSanctumMinotaurReliefOneEye, 1);
    if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_FIRST_SMALL_JEWEL_FOUND) != 0) {
      u16 *stateSlot = &gEventWork->unk172;
      u16 value = 1;
      *stateSlot = value;
    }
  }

  Engine_EventEnd();
}

/// The room shakes as stone shifts; the leader looks around and jumps.
static inline void SolSanctumEntrance_ReactToOpeningPassage(void) {
  Map_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  Engine_EventWait(20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Map_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 40);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 10);
  Engine_ActorJump(ACTOR_PARTY_LEADER, 4, 20);
  Engine_ActorJump(ACTOR_PARTY_LEADER, 6, 40);
  Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Engine_EventWait(40);
}

/// Sol Sanctum: using the Small Jewel on the first relief sets it in, and a
/// passage opens. `<leader> set the small gem into the empty socket.`, "You
/// hear the sound of shifting stone." Does nothing until the first Small Jewel
/// is found, or once it is set. Sets FLAG_SOL_SANCTUM_FIRST_GEM_PLACED and
/// FLAG_ITEM_USED_UP, which takes the jewel.
static void Event_SetFirstSmallJewel(void) {
  s32 message;
  s32 oneCell;
  s32 reliefWidth;

  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_FIRST_SMALL_JEWEL_FOUND) == 0) {
  } else {
    if (GameFlag_IsSet(FLAG_SOL_SANCTUM_FIRST_GEM_PLACED) != 0) {
    } else {
      Engine_EventBegin();
      Field_EndBusyAndReveal();
      // The eye sparkles, then the wall opens.
      oneCell = 1;
      Event_PlayCue(SE_182);
      Map_CopyCellsTo(0, 70, 30, 42, oneCell, oneCell);
      Engine_MapRedraw();
      Engine_EventWait(40);
      message = MsgSolSanctumSetSmallGem;
      Engine_MessageShowCentered(message, 1);
      Engine_EventWait(20);
      reliefWidth = 3;
      Event_PlayCue(SE_183);
      Map_CopyCellsTo(0, 29, 3, 1, reliefWidth, 2);
      Map_CopyCellAttributes(0, 29, 3, 2, reliefWidth, oneCell);
      Map_CopyCellsTo(1, 109, 4, 81, oneCell, oneCell);
      Engine_MapRedraw();
      SolSanctumEntrance_ReactToOpeningPassage();
      Engine_MessageShowCentered(message + 1, 1);
      GameFlag_Set(FLAG_ITEM_USED_UP);
      GameFlag_Set(FLAG_SOL_SANCTUM_FIRST_GEM_PLACED);
      Engine_EventEnd();
    }
  }
}

/// Sol Sanctum, the second relief room: searching the second minotaur
/// relief, as the first. Once the second Small Jewel is found,
/// and until it is set, it requests the field menu.
static void Event_InspectSecondMinotaurRelief(void) {

  struct EventWork *work;

  Engine_EventBegin();

  if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_SECOND_GEM_PLACED) != 0) {
    Engine_MessageShowCentered(MsgSolSanctumMinotaurReliefBothEyes, 1);
  } else if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_SECOND_SMALL_JEWEL_FOUND) !=
             0) {
    work = gEventWork;
    Engine_MessageShowCentered(MsgSolSanctumMinotaurReliefOneEye, 1);
    {

      u16 *stateSlot = &work->unk172;
      s32 one = 1;
      *stateSlot = one;
    }
  } else {
    Engine_MessageShowCentered(MsgSolSanctumMinotaurReliefOneEye, 1);
  }

  Engine_EventEnd();
}

/// Sol Sanctum, the second relief room: using the Small Jewel on the
/// second relief, as the first, and a doorway opens. Only once the
/// second Small Jewel is found and until it is set. Sets
/// FLAG_SOL_SANCTUM_SECOND_GEM_PLACED and FLAG_ITEM_USED_UP.
static void Event_SetSecondSmallJewel(void) {
  s32 message;

  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_SECOND_SMALL_JEWEL_FOUND) != 0 &&
      GameFlag_IsSet(FLAG_SOL_SANCTUM_SECOND_GEM_PLACED) == 0) {
    Engine_EventBegin();
    Field_EndBusyAndReveal();
    // The eye sparkles, then the doorway opens.
    Engine_AudioPlayCue(SE_182);
    Map_CopyCellsTo(0, 71, 100, 71, 1, 1);
    Engine_MapRedraw();
    Engine_EventWait(40);
    message = MsgSolSanctumSetSmallGem;
    Engine_MessageShowCentered(message, 1);
    Engine_EventWait(20);
    Engine_AudioPlayCue(SE_183);
    Map_CopyCellsTo(122, 20, 120, 30, 1, 2);
    Map_CopyCellAttributes(122, 20, 1, 2, 120, 30);
    Engine_MapRedraw();
    SolSanctumEntrance_ReactToOpeningPassage();
    Engine_MessageShowCentered(message + 1, 1);
    GameFlag_Set(FLAG_ITEM_USED_UP);
    GameFlag_Set(FLAG_SOL_SANCTUM_SECOND_GEM_PLACED);
    Engine_EventEnd();
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
    Engine_GameFlagSet(flagBase + 1);
  } else if (column == rightColumn) {
    Engine_GameFlagSet(flagBase);
  }
}

/// Sol Sanctum, the first relief room: Move on the west statue
/// (ACTOR_WEST_STATUE) while FLAG_SCENE_TEMP_34 is clear, and called by
/// Event_PushStatue there. The column it stops in is saved: sets
/// FLAG_SOL_SANCTUM_WEST_STATUE_AT_EAST or _AT_WEST, or clears both when it
/// stands in neither column.
static void Event_WestStatueMoved(void) {
  struct FieldActor *statue = Object_GetById(ACTOR_WEST_STATUE);
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
static void Event_EastStatueMoved(void) {
  struct FieldActor *statue = Object_GetById(ACTOR_EAST_STATUE);
  s32 column;

  if (statue == NULL) {
    return;
  }
  column = statue->x.fixed >> 20;
  GameFlag_Clear(FLAG_SOL_SANCTUM_EAST_STATUE_AT_EAST);
  GameFlag_Clear(FLAG_SOL_SANCTUM_EAST_STATUE_AT_WEST);
  if (column == 115) {
    Engine_GameFlagSet(FLAG_SOL_SANCTUM_EAST_STATUE_AT_EAST);
  } else if (column == 113) {
    Engine_GameFlagSet(FLAG_SOL_SANCTUM_EAST_STATUE_AT_WEST);
  }
}

/// Sol Sanctum, the second relief room: Move on the first statue of the
/// row (ACTOR_ROW_STATUE_1), and called by Event_PushStatue there. The column
/// it stops in is saved and the cells under all six are set again. Sets
/// FLAG_SOL_SANCTUM_ROW_STATUE_1_AT_EAST or _AT_WEST, or clears both.
static void Event_RowStatue1Moved(void) {
  struct FieldActor *statue = Object_GetById(ACTOR_ROW_STATUE_1);
  s32 column;

  if (statue == NULL) {
    return;
  }
  column = statue->x.fixed >> 20;
  SolSanctumEntrance_RecordStatueColumn(
      column, FLAG_SOL_SANCTUM_ROW_STATUE_1_AT_EAST, 99, 101);
  SolSanctumEntrance_RestoreRowStatueCells(0);
}

/// Sol Sanctum, the second relief room: as Event_RowStatue1Moved, for
/// the row's second statue (ACTOR_ROW_STATUE_2).
static void Event_RowStatue2Moved(void) {
  struct FieldActor *statue = Object_GetById(ACTOR_ROW_STATUE_2);
  s32 column;

  if (statue == NULL) {
    return;
  }
  column = statue->x.fixed >> 20;
  SolSanctumEntrance_RecordStatueColumn(
      column, FLAG_SOL_SANCTUM_ROW_STATUE_2_AT_EAST, 103, 105);
  SolSanctumEntrance_RestoreRowStatueCells(0);
}

/// Sol Sanctum, the second relief room: as Event_RowStatue1Moved, for
/// the row's third statue (ACTOR_ROW_STATUE_3).
static void Event_RowStatue3Moved(void) {
  struct FieldActor *statue = Object_GetById(ACTOR_ROW_STATUE_3);
  s32 column;

  if (statue == NULL) {
    return;
  }
  column = statue->x.fixed >> 20;
  SolSanctumEntrance_RecordStatueColumn(
      column, FLAG_SOL_SANCTUM_ROW_STATUE_3_AT_EAST, 107, 109);
  SolSanctumEntrance_RestoreRowStatueCells(0);
}

/// Sol Sanctum, the second relief room: as Event_RowStatue1Moved, for
/// the row's fourth statue (ACTOR_ROW_STATUE_4).
static void Event_RowStatue4Moved(void) {
  struct FieldActor *statue = Object_GetById(ACTOR_ROW_STATUE_4);
  s32 column;

  if (statue == NULL) {
    return;
  }
  column = statue->x.fixed >> 20;
  SolSanctumEntrance_RecordStatueColumn(
      column, FLAG_SOL_SANCTUM_ROW_STATUE_4_AT_EAST, 111, 113);
  SolSanctumEntrance_RestoreRowStatueCells(0);
}

/// Sol Sanctum, the second relief room: as Event_RowStatue1Moved, for
/// the row's fifth statue (ACTOR_ROW_STATUE_5).
static void Event_RowStatue5Moved(void) {
  struct FieldActor *statue = Object_GetById(ACTOR_ROW_STATUE_5);
  s32 column;

  if (statue == NULL) {
    return;
  }
  column = statue->x.fixed >> 20;
  SolSanctumEntrance_RecordStatueColumn(
      column, FLAG_SOL_SANCTUM_ROW_STATUE_5_AT_EAST, 115, 117);
  SolSanctumEntrance_RestoreRowStatueCells(0);
}

/// Sol Sanctum, the second relief room: as Event_RowStatue1Moved, for
/// the row's sixth statue (ACTOR_ROW_STATUE_6).
static void Event_RowStatue6Moved(void) {
  struct FieldActor *statue = Object_GetById(ACTOR_ROW_STATUE_6);
  s32 column;

  if (statue == NULL)
    return;
  column = statue->x.fixed >> 20;
  SolSanctumEntrance_RecordStatueColumn(
      column, FLAG_SOL_SANCTUM_ROW_STATUE_6_AT_EAST, 119, 121);
  SolSanctumEntrance_RestoreRowStatueCells(0);
}

#include "common/object_findcell.h"

/// Sol Sanctum, the two relief rooms: walking into a statue's cell
/// pushes it one cell, the leader behind it, if nothing is in the way, and its
/// column is saved through the room's Moved events.
static void Event_PushStatue(void) {
  struct FieldActor *leader;
  struct FieldActor *statue;
  s32 stoppedVelocity;
  s32 packedStep;
  u32 direction;
  struct FieldPosition target;
  struct FieldPosition *destination;

  // Find the statue one map cell ahead in the leader's facing direction.
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  direction = leader->facing >> 12;
  statue = SceneActor_FindActorAtCell(
      (leader->x.part.pixel + (gSolSanctumPushSteps[direction] >> 16)) >> 4,
      (leader->z.part.pixel + (s16)gSolSanctumPushSteps[direction]) >> 4);
  if (statue != NULL) {
    stoppedVelocity = 0;
    statue->terrainId = 2;
    destination = &target;
    packedStep = gSolSanctumPushSteps[direction];
    SET_POSITION(destination, statue->x.fixed + (packedStep & -0x10000),
                 statue->y.fixed, statue->z.fixed + (packedStep << 16));
    // Move only after the statue's proposed position passes collision.
    if (FieldObject_CheckMovementCollision(statue, destination) <= 0) {
      Object_SetMode(leader, ANIM_PUSH);
      Engine_TaskWait(15);
      Engine_AudioPlayCue(SE_185);
      statue->speed = PIXELS(1) / 5;
      statue->acceleration = PIXELS(1) / 5;
      Engine_ObjectSetPosition(statue, destination->x, destination->y,
                               destination->z);
      leader->speed = PIXELS(1) / 5;
      leader->acceleration = PIXELS(1) / 5;
      Engine_ObjectSetPosition(leader, destination->x, destination->y,
                               destination->z);
      FieldObject_WaitForMove(statue);
      statue->x.fixed = destination->x;
      statue->z.fixed = destination->z;
      statue->velocityX = stoppedVelocity;
      statue->velocityZ = stoppedVelocity;
      Object_SetMode(leader, ANIM_STAND);
      // Save the moved statue in the flag set belonging to this room.
      switch (gGameState.entrance) {
      case SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_FROM_CHEST_ROOM:
      case SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_FROM_STATUE_NOOK:
      case SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_FROM_STEPS_ROOM:
        Event_WestStatueMoved();
        Event_EastStatueMoved();
        break;
      case SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_MAZE_EAST:
      case SOL_SANCTUM_RELIEFS_SECOND_RELIEF_ROOM_FROM_ROUND_PASSAGE:
      case SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS:
        Event_RowStatue1Moved();
        Event_RowStatue2Moved();
        Event_RowStatue3Moved();
        Event_RowStatue4Moved();
        Event_RowStatue5Moved();
        Event_RowStatue6Moved();
        break;
      }
    }
  }
}

/// Sets up Sol Entrance or Sol Sanctum. Called by Field_RunScene once the
/// actors exist.
s32 SolSanctumEntrance_Initialize(void) {
  s32 scene = gGameState.scene;

  if (scene == (s32)&SceneId_SolEntrance) {
    SolSanctumEntrance_ApplyEntryState();
  } else if (scene == (s32)&SceneId_SolSanctumReliefs) {
    SolSanctumEntrance_RestoreInteriorState();
  }
  return 0;
}

/// Sets up Sol Entrance (SceneId_SolEntrance): Retreat is blocked
/// (FLAG_RETREAT_BLOCKED), and after the eruption
/// SolSanctumEntrance_UpdateEruptionRumbles shakes the ground now and then.
/// Once Isaac and Garet are back in Vale (FLAG_VALE_RETURNED_FROM_MT_ALEPH),
/// the six cells before the door take the collision of other cells (an open
/// question above); once the party has left Vale (FLAG_PARTY_LEFT_VALE), the
/// man who warns everyone away, placed off the map until then, stands in the
/// path and the three cells across it change.
static void SolSanctumEntrance_ApplyEntryState(void) {
  // Retreat does not work here.
  GameFlag_Set(FLAG_RETREAT_BLOCKED);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_ERUPTED) != 0) {
    gSolSanctumEntrance_RumbleTimer = 0;
    Task_AddCallback(SolSanctumEntrance_UpdateEruptionRumbles, 0xc80);
  }
  if (GameFlag_IsSet(FLAG_VALE_RETURNED_FROM_MT_ALEPH) != 0) {
    Map_CopyCellAttributes(5, 6, 1, 1, 6, 6);
    Map_CopyCellAttributes(5, 6, 1, 1, 7, 6);
    Map_CopyCellAttributes(5, 6, 1, 1, 8, 6);
    Map_CopyCellAttributes(0, 1, 3, 1, 6, 5);
  }
  if (GameFlag_IsSet(FLAG_PARTY_LEFT_VALE) != 0) {
    Actor_SetPosition(ACTOR_WARNING_MAN, PIXELS(120), PIXELS(232));
    Map_CopyCellAttributes(2, 10, 1, 1, 6, 14);
    Map_CopyCellAttributes(2, 10, 1, 1, 7, 14);
    Map_CopyCellAttributes(2, 10, 1, 1, 8, 14);
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
/// set, the six go back to their columns, FLAG_SCENE_TEMP_34 is set and the
/// second relief's doorway shows once it is open.
static void SolSanctumEntrance_RestoreInteriorState(void) {
  s32 entrance;
  gEventWork->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_ERUPTED) != 0) {
    FieldFx_SetQueuedSoundAndPlay(SE_141);
    Map_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
    InitializeSceneRecordBuffer();
  }

  entrance = gGameState.entrance;
  switch (entrance) {
  case SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_CORRIDOR_WEST:
  case SOL_SANCTUM_RELIEFS_CHEST_ROOM_FROM_FIRST_RELIEF_ROOM:
    if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_FIRST_GEM_PLACED) != 0) {
      Map_CopyCellsTo(1, 109, 4, 81, 1, 1);
      Map_CopyCellsTo(0, 70, 30, 42, 1, 1);
      Map_CopyCellsTo(0, 29, 3, 1, 3, 2);
      Map_CopyCellAttributes(0, 29, 3, 2, 3, 1);
      Engine_MapRedraw();
    }
    break;

  case SOL_SANCTUM_RELIEFS_STATUE_NOOK_FROM_FIRST_RELIEF_ROOM:
    Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_STATUE), 0);
    break;

  case SOL_SANCTUM_RELIEFS_FROM_SOL_ENTRANCE:
    gGameState.retreatScene = (s32)&SceneId_SolSanctumReliefs;
    gGameState.retreatEntrance = SOL_SANCTUM_RELIEFS_FROM_SOL_ENTRANCE;
    if (GameFlag_IsSet(FLAG_SOL_SANCTUM_ENTERED) == 0)
      SolSanctumEntrance_IntroduceSanctum();
    break;

  case SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_FROM_CHEST_ROOM:
  case SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_FROM_STATUE_NOOK:
  case SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_FROM_STEPS_ROOM:
    // Put back this room's statues after its introduction has played.
    Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_WEST_STATUE), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_EAST_STATUE), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_FIRST_FIXED_STATUE), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_FIRST_FIXED_STATUE + 1), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_FIRST_FIXED_STATUE + 2), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_FIRST_FIXED_STATUE + 3), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_FIRST_FIXED_STATUE + 4), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_FIRST_FIXED_STATUE + 5), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_FIRST_FIXED_STATUE + 6), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_FIRST_FIXED_STATUE + 7), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_FIRST_FIXED_STATUE + 8), 0);
    if (Engine_GameFlagIsSet(FLAG_SEARCHING_FOR_HIDDEN_PASSAGE) == 0)
      SolSanctumEntrance_KradenSuspectsHiddenPassage();
    if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_WEST_STATUE_AT_WEST) != 0)
      Actor_SetPosition(ACTOR_WEST_STATUE, PIXELS(1496), PIXELS(136));
    else if (GameFlag_IsSet(FLAG_SOL_SANCTUM_WEST_STATUE_AT_EAST) != 0)
      Actor_SetPosition(ACTOR_WEST_STATUE, PIXELS(1528), PIXELS(136));
    if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_EAST_STATUE_AT_WEST) != 0)
      Actor_SetPosition(ACTOR_EAST_STATUE, PIXELS(1816), PIXELS(136));
    else if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_EAST_STATUE_AT_EAST) != 0)
      Actor_SetPosition(ACTOR_EAST_STATUE, PIXELS(1848), PIXELS(136));
    break;

  case SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_MAZE_EAST:
  case SOL_SANCTUM_RELIEFS_SECOND_RELIEF_ROOM_FROM_ROUND_PASSAGE:
  case SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS:
    // Restore the row of statues and the second gem's doorway.
    Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_ROW_STATUE_1), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_ROW_STATUE_2), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_ROW_STATUE_3), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_ROW_STATUE_4), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_ROW_STATUE_5), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_ROW_STATUE_6), 0);
    if (Engine_GameFlagIsSet(FLAG_SOL_SANCTUM_MORE_STATUES_SEEN) == 0)
      SolSanctumEntrance_KradenSeesStatuesOutOfReach();
    SolSanctumEntrance_RestoreRowStatueCells(1);
    Engine_GameFlagSet(FLAG_SCENE_TEMP_34);
    if (GameFlag_IsSet(FLAG_SOL_SANCTUM_SECOND_GEM_PLACED) != 0) {
      Map_CopyCellsTo(0, 71, 100, 71, 1, 1);
      Map_CopyCellsTo(122, 20, 120, 30, 1, 2);
      Map_CopyCellAttributes(122, 20, 1, 2, 120, 30);
      Engine_MapRedraw();
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
/// set, which it sets; clears FLAG_SHOW_LOCATION_NAME.
static void SolSanctumEntrance_IntroduceSanctum(void) {
  struct FieldActor *leader;

  Engine_EventBegin();
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Engine_EventOpenScreen();
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(4);
  Camera_MoveTo(-1, -1, -1, 0);
  // Gather the followers around the leader for Kraden's introduction.
  Camera_SetSpeed(PIXELS(3) / 5, PIXELS(3) / 40);
  Camera_MoveTo(PIXELS(1224), -1, PIXELS(136), 1);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetPosition(ACTOR_KRADEN, leader->x.fixed, leader->z.fixed);
  }
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetPosition(JENNA, leader->x.fixed, leader->z.fixed);
  }
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetPosition(GARET, leader->x.fixed, leader->z.fixed);
  }
  Actor_SetSpeed(ACTOR_KRADEN, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_SetSpeed(JENNA, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_SetSpeed(GARET, PIXELS(3) / 5, PIXELS(3) / 10);
  Engine_ActorSetAnimation(GARET, ANIM_WALK);
  Engine_ActorSetAnimation(JENNA, ANIM_WALK);
  Engine_ActorSetAnimation(ACTOR_KRADEN, ANIM_WALK);
  Actor_SetDestinationOffset(GARET, -16, 0);
  Actor_SetDestinationOffset(JENNA, 16, 0);
  Actor_SetDestinationOffset(ACTOR_KRADEN, 0, -32);
  Engine_ActorWaitForMove(GARET);
  Engine_ActorSetAnimation(GARET, 0);
  Engine_ActorSetAnimation(JENNA, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(JENNA, FACING_NORTH, 0);
  Engine_ActorWaitForMove(ACTOR_KRADEN);
  Engine_ActorSetAnimation(ACTOR_KRADEN, ANIM_STAND);
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST + FACING_STEP, 40);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH + FACING_STEP, 40);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST + FACING_STEP, 20);
  Engine_ActorJump(ACTOR_KRADEN, 4, 20);
  // Kraden: "This is your first time here, right?" Yes: "Take a good look at
  // it." No: "I doubt that..."
  Engine_EventSetMessage(MsgSolSanctumKradenFirstTimeAtSol);
  Event_AskYesNo(SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 0);
  Engine_EventWait(20);
  Camera_MoveTo(PIXELS(1224), -1, PIXELS(148), 1);
  Engine_ActorSetAnimation(GARET, ANIM_WALK);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetDestination(GARET, leader->x.part.pixel, leader->z.part.pixel);
  }
  Engine_ActorSetAnimation(JENNA, ANIM_WALK);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetDestination(JENNA, leader->x.part.pixel, leader->z.part.pixel);
  }
  Engine_ActorSetAnimation(ACTOR_KRADEN, ANIM_WALK);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetDestination(ACTOR_KRADEN, leader->x.part.pixel,
                         leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(GARET);
  Actor_SetPosition(GARET, 0, 0);
  Actor_SetPosition(JENNA, 0, 0);
  Engine_ActorWaitForMove(ACTOR_KRADEN);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  Engine_ActorSetAnimation(GARET, ANIM_STAND);
  Engine_ActorSetAnimation(JENNA, ANIM_STAND);
  Engine_ActorSetAnimation(ACTOR_KRADEN, ANIM_STAND);
  GameFlag_Set(FLAG_SOL_SANCTUM_ENTERED);
  gEventWork->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  GameFlag_Clear(FLAG_SHOW_LOCATION_NAME);
  Engine_EventEnd();
}

/// Sol Sanctum, the first time the party enters the first relief
/// room: Kraden looks round and recalls that Saturos and Menardi spoke of a
/// hidden passage leading deeper into the sanctum; a sanctum built in honour
/// of the sun ought to show it somewhere. Garet asks whether to help him, and
/// either way the four stay together. Called by
/// SolSanctumEntrance_RestoreInteriorState there until
/// FLAG_SEARCHING_FOR_HIDDEN_PASSAGE is set, which it sets; clears
/// FLAG_SHOW_LOCATION_NAME.
static void SolSanctumEntrance_KradenSuspectsHiddenPassage(void) {
  struct FieldActor *leader;

  Engine_EventBegin();
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();

  // Spread the party out while Kraden examines the room.
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != NULL)
    Engine_ActorSetPosition(ACTOR_KRADEN, leader->x.fixed, leader->z.fixed);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != NULL)
    Engine_ActorSetPosition(JENNA, leader->x.fixed, leader->z.fixed);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != NULL)
    Engine_ActorSetPosition(GARET, leader->x.fixed, leader->z.fixed);

  Actor_SetSpeed(ACTOR_KRADEN, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_SetSpeed(JENNA, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_SetSpeed(GARET, PIXELS(3) / 5, PIXELS(3) / 10);
  Engine_ActorSetAnimation(GARET, ANIM_WALK);
  Engine_ActorSetAnimation(JENNA, ANIM_WALK);
  Engine_ActorSetAnimation(ACTOR_KRADEN, ANIM_WALK);
  Actor_SetDestinationOffset(GARET, -16, 0);
  Actor_SetDestinationOffset(JENNA, 16, 0);
  Actor_SetDestinationOffset(ACTOR_KRADEN, 0, -16);
  Engine_ActorWaitForMove(ACTOR_KRADEN);
  Engine_ActorSetAnimation(ACTOR_KRADEN, ANIM_STAND);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 0);
  Engine_ActorSetAnimation(GARET, 0);
  Engine_ActorSetAnimation(JENNA, 0);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(JENNA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 30);
  Actor_FaceDirection(GARET, FACING_WEST, 0);
  Actor_FaceDirection(JENNA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 30);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(JENNA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH, 30);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(JENNA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 40);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
  Engine_EventWait(10);
  Engine_ActorSetAnimation(ACTOR_KRADEN, ANIM_WALK);
  Engine_ActorSetDestinationOffset(ACTOR_KRADEN, 0, -16);
  Engine_ActorWaitForMove(ACTOR_KRADEN);
  Engine_ActorSetAnimation(ACTOR_KRADEN, ANIM_STAND);
  Engine_EventWait(6);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 40);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(ACTOR_KRADEN, ANIM_WALK);
  Engine_ActorSetDestinationOffset(ACTOR_KRADEN, 0, -32);
  Engine_ActorWaitForMove(ACTOR_KRADEN);
  Engine_ActorSetAnimation(ACTOR_KRADEN, ANIM_STAND);

  // Survey both sides of the room before returning to Kraden.
  Camera_SetSpeed(PIXELS(2), PIXELS(1) / 4);
  Camera_MoveTo(PIXELS(1585), -1, PIXELS(150), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(10);
  Camera_SetSpeed(PIXELS(6) / 5, PIXELS(3) / 20);
  Camera_MoveTo(PIXELS(1621), -1, PIXELS(100), 1);
  Engine_CameraWaitForMove();
  Camera_MoveTo(PIXELS(1718), -1, PIXELS(100), 1);
  Engine_CameraWaitForMove();
  Engine_ActorSetAnimation(ACTOR_KRADEN, ANIM_STAND);
  Camera_MoveTo(PIXELS(1752), -1, PIXELS(150), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(40);
  Camera_SetSpeed(PIXELS(12) / 5, PIXELS(3) / 10);
  Camera_MoveTo(PIXELS(1668), -1, PIXELS(256), 1);
  Engine_CameraWaitForMove();
  Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
  Engine_EventWait(10);

  Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(JENNA, FACING_NORTHWEST, 10);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  // Garet: "What's wrong, Kraden?"
  Engine_EventSetMessage(MsgSolSanctumWrongKraden);
  Event_ShowMessageAndWait(GARET, 0, 10);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Engine_ActorStartRepeatedMotion(ACTOR_KRADEN, 2);
  // Kraden: "Those two said there was a hidden passage leading deeper into
  // the sanctum."
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0, 10);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_ActorStartRepeatedMotion(JENNA, 2);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(JENNA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
  Engine_EventWait(20);
  // Kraden: the sanctum always looked strange to him, not as he imagined.
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0, 10);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 5, 0);
  Engine_EventWait(40);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Engine_ActorRunRepeatedMotion(JENNA, 1);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(JENNA, FACING_NORTHWEST, 20);
  Engine_ActorRunRepeatedMotion(JENNA, 2);
  // Jenna: "What do you mean?"
  Event_ShowMessageAndWait(JENNA, 0, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
  // Kraden: a sanctum built in honour of the sun ought to show it somewhere.
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0, 10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST + FACING_STEP, 10);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0, 40);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 1);
  Engine_EventWait(10);
  Actor_SetAttachedEffect(ACTOR_KRADEN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH, 60);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
  Engine_EventWait(10);
  // Kraden: "If there is a hidden passage, then maybe it leads to..."
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0, 10);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH, 20);
  Engine_ActorJump(ACTOR_KRADEN, 2, 20);

  // Kraden hops: "Let's see if we can find it!"
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0, 40);
  Actor_FaceDirection(GARET, FACING_EAST, 20);
  // Garet: "I think he's serious! Should we help him?"
  Event_OpenMessage(GARET, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    // Yes: Garet says it is too dangerous to split up; they stay together.
    Engine_EventSetMessage(MsgSolSanctumDangerousSplitStay);
    Engine_ActorRunRepeatedMotion(GARET, 1);
    Engine_EventShowMessageAndWait(GARET, 0, 10);
  } else {
    // No: Jenna objects to turning back now; Isaac nods and Garet agrees to
    // help Kraden.
    Engine_EventSetMessage(MsgSolSanctumCantSeriouslyWant);
    Actor_FaceDirection(JENNA, FACING_WEST, 20);
    Event_ShowMessageAndWait(JENNA, 0, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
    Actor_FaceDirection(JENNA, FACING_NORTHWEST, 60);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
    Engine_ActorRunRepeatedMotion(GARET, 1);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
    Engine_EventWait(10);
    Actor_FaceDirection(GARET, FACING_EAST, 0);
    Actor_FaceDirection(JENNA, FACING_WEST, 30);
    Engine_ActorRunRepeatedMotion(GARET, 1);
    Engine_EventWait(10);
    Engine_EventShowMessageAndWait(GARET, 0, 10);
  }

  // Bring Kraden back and return the followers to the party.
  Actor_SetSpeed(ACTOR_KRADEN, PIXELS(3) / 5, PIXELS(3) / 10);
  Engine_ActorSetAnimation(ACTOR_KRADEN, ANIM_WALK);
  Engine_ActorSetDestinationOffset(ACTOR_KRADEN, 0, 48);
  Engine_ActorWaitForMove(ACTOR_KRADEN);
  Engine_ActorSetAnimation(ACTOR_KRADEN, ANIM_STAND);
  Engine_EventWait(6);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
  Engine_ActorFaceDirection(JENNA, FACING_NORTHWEST, 0);
  Engine_ActorWaitForMove(ACTOR_KRADEN);
  Engine_ActorSetAnimation(ACTOR_KRADEN, ANIM_STAND);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimation(JENNA, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(6);
  Engine_ActorSetAnimation(GARET, ANIM_WALK);

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != NULL)
    Engine_ActorSetDestination(GARET, leader->x.part.pixel,
                               leader->z.part.pixel);
  Engine_ActorSetAnimation(JENNA, ANIM_WALK);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != NULL)
    Engine_ActorSetDestination(JENNA, leader->x.part.pixel,
                               leader->z.part.pixel);
  Engine_ActorSetAnimation(ACTOR_KRADEN, ANIM_WALK);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != NULL)
    Engine_ActorSetDestination(ACTOR_KRADEN, leader->x.part.pixel,
                               leader->z.part.pixel);

  Engine_ActorWaitForMove(ACTOR_KRADEN);
  Actor_SetPosition(GARET, 0, 0);
  Actor_SetPosition(JENNA, 0, 0);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  Engine_ActorSetAnimation(ACTOR_KRADEN, ANIM_STAND);
  Engine_ActorSetAnimation(GARET, ANIM_STAND);
  Engine_ActorSetAnimation(JENNA, ANIM_STAND);
  GameFlag_Set(FLAG_SEARCHING_FOR_HIDDEN_PASSAGE);
  Engine_GameFlagClear(FLAG_SHOW_LOCATION_NAME);
  Engine_EventEnd();
}

/// Sol Sanctum, the first time the party enters the second relief
/// room: the camera pans along the row of statues, and Kraden: "More
/// statues... It doesn't look like we can reach them from here." He asks
/// Isaac to try his Psynergy. Called by SolSanctumEntrance_RestoreInteriorState
/// there until FLAG_SOL_SANCTUM_MORE_STATUES_SEEN is set, which it sets.
static void SolSanctumEntrance_KradenSeesStatuesOutOfReach(void) {
  struct FieldActor *leader;

  Engine_EventBegin();
  gEventWork->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(20);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetPosition(ACTOR_KRADEN, leader->x.fixed, leader->z.fixed);
  }
  Actor_SetSpeed(ACTOR_KRADEN, PIXELS(1), PIXELS(1) / 2);
  Engine_ActorSetAnimation(ACTOR_KRADEN, ANIM_WALK);
  Actor_SetDestinationOffset(ACTOR_KRADEN, 24, -10);
  Engine_ActorWaitForMove(ACTOR_KRADEN);
  Engine_ActorSetAnimation(ACTOR_KRADEN, ANIM_STAND);
  Engine_EventWait(6);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 40);
  Camera_SetSpeed(PIXELS(12) / 5, PIXELS(3) / 10);
  // Pan across the statues, then return to the party's position.
  Camera_MoveTo(PIXELS(1672), -1, PIXELS(524), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  Camera_SetSpeed(PIXELS(8) / 5, PIXELS(1) / 5);
  Camera_MoveTo(PIXELS(1880), -1, PIXELS(524), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  Camera_SetSpeed(PIXELS(16) / 5, PIXELS(2) / 5);
  Camera_MoveTo(PIXELS(1769), -1, PIXELS(548), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 2);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 30);
  // Kraden: "More statues... It doesn't look like we can reach them".
  Engine_EventSetMessage(MsgSolSanctumMoreStatuesOutOfReach);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 0, 10);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Engine_ActorRunRepeatedMotion(ACTOR_KRADEN, 1);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH + FACING_STEP, 20);
  // Kraden: "Perhaps Psynergy is the key..."
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 0, 10);
  Engine_ActorSetAnimation(ACTOR_KRADEN, ANIM_WALK);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetDestination(ACTOR_KRADEN, leader->x.part.pixel,
                         leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(ACTOR_KRADEN);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  GameFlag_Set(FLAG_SOL_SANCTUM_MORE_STATUES_SEEN);
  Engine_EventEnd();
}

/// Sol Sanctum, the second relief room: sets the cells the six statues
/// start on, one collision while a statue stands there and another once it has
/// been pushed to either side, and, when placeActors is nonzero (on entry),
/// moves the statues to their saved columns.
static void SolSanctumEntrance_RestoreRowStatueCells(s32 placeActors) {
  // Clear all six destinations before restoring occupied cells.
  Map_CopyCellAttributes(122, 20, 1, 1, 100, 32);
  Map_CopyCellAttributes(122, 20, 1, 1, 104, 32);
  Map_CopyCellAttributes(122, 20, 1, 1, 108, 32);
  Map_CopyCellAttributes(122, 20, 1, 1, 112, 32);
  Map_CopyCellAttributes(122, 20, 1, 1, 116, 32);
  Map_CopyCellAttributes(122, 20, 1, 1, 120, 32);
  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_ROW_STATUE_1_AT_WEST) != 0) {
    Map_CopyCellAttributes(121, 20, 1, 1, 100, 32);
    if (placeActors != 0) {
      Actor_SetPosition(ACTOR_ROW_STATUE_1, PIXELS(1592), PIXELS(520));
    }
  } else {
    if (GameFlag_IsSet(FLAG_SOL_SANCTUM_ROW_STATUE_1_AT_EAST) != 0) {
      Map_CopyCellAttributes(121, 20, 1, 1, 100, 32);
      if (placeActors != 0) {
        Actor_SetPosition(ACTOR_ROW_STATUE_1, PIXELS(1624), PIXELS(520));
      }
    }
  }
  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_ROW_STATUE_2_AT_WEST) != 0) {
    Map_CopyCellAttributes(121, 20, 1, 1, 104, 32);
    if (placeActors != 0) {
      Actor_SetPosition(ACTOR_ROW_STATUE_2, PIXELS(1656), PIXELS(520));
    }
  } else {
    if (GameFlag_IsSet(FLAG_SOL_SANCTUM_ROW_STATUE_2_AT_EAST) != 0) {
      Map_CopyCellAttributes(121, 20, 1, 1, 104, 32);
      if (placeActors != 0) {
        Actor_SetPosition(ACTOR_ROW_STATUE_2, PIXELS(1688), PIXELS(520));
      }
    }
  }
  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_ROW_STATUE_3_AT_WEST) != 0) {
    Map_CopyCellAttributes(121, 20, 1, 1, 108, 32);
    if (placeActors != 0) {
      Actor_SetPosition(ACTOR_ROW_STATUE_3, PIXELS(1720), PIXELS(520));
    }
  } else {
    if (GameFlag_IsSet(FLAG_SOL_SANCTUM_ROW_STATUE_3_AT_EAST) != 0) {
      Map_CopyCellAttributes(121, 20, 1, 1, 108, 32);
      if (placeActors != 0) {
        Actor_SetPosition(ACTOR_ROW_STATUE_3, PIXELS(1752), PIXELS(520));
      }
    }
  }
  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_ROW_STATUE_4_AT_WEST) != 0) {
    Map_CopyCellAttributes(121, 20, 1, 1, 112, 32);
    if (placeActors != 0) {
      Actor_SetPosition(ACTOR_ROW_STATUE_4, PIXELS(1784), PIXELS(520));
    }
  } else {
    if (GameFlag_IsSet(FLAG_SOL_SANCTUM_ROW_STATUE_4_AT_EAST) != 0) {
      Map_CopyCellAttributes(121, 20, 1, 1, 112, 32);
      if (placeActors != 0) {
        Actor_SetPosition(ACTOR_ROW_STATUE_4, PIXELS(1816), PIXELS(520));
      }
    }
  }
  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_ROW_STATUE_5_AT_WEST) != 0) {
    Map_CopyCellAttributes(121, 20, 1, 1, 116, 32);
    if (placeActors != 0) {
      Actor_SetPosition(ACTOR_ROW_STATUE_5, PIXELS(1848), PIXELS(520));
    }
  } else {
    if (GameFlag_IsSet(FLAG_SOL_SANCTUM_ROW_STATUE_5_AT_EAST) != 0) {
      Map_CopyCellAttributes(121, 20, 1, 1, 116, 32);
      if (placeActors != 0) {
        Actor_SetPosition(ACTOR_ROW_STATUE_5, PIXELS(1880), PIXELS(520));
      }
    }
  }
  if (GameFlag_IsSet(FLAG_SOL_SANCTUM_ROW_STATUE_6_AT_WEST) != 0) {
    Map_CopyCellAttributes(121, 20, 1, 1, 120, 32);
    if (placeActors != 0) {
      Actor_SetPosition(ACTOR_ROW_STATUE_6, PIXELS(1912), PIXELS(520));
    }
  } else {
    if (GameFlag_IsSet(FLAG_SOL_SANCTUM_ROW_STATUE_6_AT_EAST) != 0) {
      Map_CopyCellAttributes(121, 20, 1, 1, 120, 32);
      if (placeActors != 0) {
        Actor_SetPosition(ACTOR_ROW_STATUE_6, PIXELS(1944), PIXELS(520));
      }
    }
  }
}

/// Sol Entrance: searching the other door . "The door is
/// tightly locked."
static void Event_DoorTightlyLocked(void) {
  Engine_EventBegin();
  Engine_MessageShowCentered(MsgFieldDoorTightlyLocked, 1);
  Engine_EventEnd();
}

/// Sol Entrance after the eruption: the ground rumbles now and then, and each
/// shake fades. Run each frame as a callback that
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
      Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
    }
  } else {
    randomValue = Engine_RandomNext();
    if (((u32)(((randomValue << 4) - randomValue) << 3) >> 16) == 0) {
      Event_PlayCue(SE_ITEM_BREAK);
      Map_SetShake(PIXELS(1), PIXELS(2), Q16_ONE);
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
    SCENE_EXIT_IF(SOL_ENTRANCE_TO_VALE, Vale, VALE_FROM_SOL_ENTRANCE,
                  FLAG_SOL_SANCTUM_ERUPTED),
    SCENE_EXIT_IF(SOL_ENTRANCE_TO_VALE, ValeEruption,
                  VALE_ERUPTION_FROM_SOL_ENTRANCE, FLAG_PARTY_LEFT_VALE),
    SCENE_EXIT_IF(SOL_ENTRANCE_TO_VALE, Vale, VALE_FROM_SOL_ENTRANCE,
                  EXIT_FLAG_NONE),
    SCENE_EXIT_IF(SOL_ENTRANCE_TO_SOL_SANCTUM_RELIEFS, SolSanctumReliefs,
                  SOL_SANCTUM_RELIEFS_FROM_SOL_ENTRANCE, EXIT_FLAG_NONE),
    SCENE_EXITS(SolSanctumReliefs),
    SCENE_EXIT_IF(
        SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_CORRIDOR_WEST, SolSanctumCorridor,
        SOL_SANCTUM_CORRIDOR_FROM_SOL_SANCTUM_RELIEFS_WEST, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(
        SOL_SANCTUM_RELIEFS_CHEST_ROOM_TO_FIRST_RELIEF_ROOM, SolSanctumReliefs,
        SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_FROM_CHEST_ROOM, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(
        SOL_SANCTUM_RELIEFS_STATUE_NOOK_TO_FIRST_RELIEF_ROOM, SolSanctumReliefs,
        SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_FROM_STATUE_NOOK, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(SOL_SANCTUM_RELIEFS_STEPPING_STONE_ROOM_TO_ENTRANCE_ROOM,
                  SolSanctumReliefs,
                  SOL_SANCTUM_RELIEFS_ENTRANCE_ROOM_FROM_STEPPING_STONE_ROOM,
                  EXIT_FLAG_NONE),
    SCENE_EXIT_IF(
        SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_CORRIDOR_EAST, SolSanctumCorridor,
        SOL_SANCTUM_CORRIDOR_FROM_SOL_SANCTUM_RELIEFS_EAST, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(
        SOL_SANCTUM_RELIEFS_STEPS_ROOM_TO_FIRST_RELIEF_ROOM, SolSanctumReliefs,
        SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_FROM_STEPS_ROOM, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_MAZE_WEST, SolSanctumMaze,
                  SOL_SANCTUM_MAZE_FROM_SOL_SANCTUM_RELIEFS_SOUTH,
                  EXIT_FLAG_NONE),
    SCENE_EXIT_IF(SOL_SANCTUM_RELIEFS_TO_SOL_ENTRANCE, SolEntrance,
                  SOL_ENTRANCE_FROM_SOL_SANCTUM_RELIEFS, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(SOL_SANCTUM_RELIEFS_ENTRANCE_ROOM_TO_STEPPING_STONE_ROOM,
                  SolSanctumReliefs,
                  SOL_SANCTUM_RELIEFS_STEPPING_STONE_ROOM_FROM_ENTRANCE_ROOM,
                  EXIT_FLAG_NONE),
    SCENE_EXIT_IF(SOL_SANCTUM_RELIEFS_ROUND_PASSAGE_TO_SECOND_RELIEF_ROOM,
                  SolSanctumReliefs,
                  SOL_SANCTUM_RELIEFS_SECOND_RELIEF_ROOM_FROM_ROUND_PASSAGE,
                  EXIT_FLAG_NONE),
    SCENE_EXIT_IF(
        SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_TO_CHEST_ROOM, SolSanctumReliefs,
        SOL_SANCTUM_RELIEFS_CHEST_ROOM_FROM_FIRST_RELIEF_ROOM, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(
        SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_TO_STATUE_NOOK, SolSanctumReliefs,
        SOL_SANCTUM_RELIEFS_STATUE_NOOK_FROM_FIRST_RELIEF_ROOM, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(
        SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_TO_STEPS_ROOM, SolSanctumReliefs,
        SOL_SANCTUM_RELIEFS_STEPS_ROOM_FROM_FIRST_RELIEF_ROOM, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_MAZE_EAST, SolSanctumMaze,
                  SOL_SANCTUM_MAZE_FROM_SOL_SANCTUM_RELIEFS_NORTH,
                  EXIT_FLAG_NONE),
    SCENE_EXIT_IF(SOL_SANCTUM_RELIEFS_SECOND_RELIEF_ROOM_TO_ROUND_PASSAGE,
                  SolSanctumReliefs,
                  SOL_SANCTUM_RELIEFS_ROUND_PASSAGE_FROM_SECOND_RELIEF_ROOM,
                  EXIT_FLAG_NONE),
    SCENE_EXIT_IF(SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_LUNA_SOL_ROOMS,
                  SolSanctumLunaSolRooms,
                  SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_RELIEFS,
                  EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

struct ScenePlacement gSolSanctumEntrance_FallbackPlacements[] = {
    {SCENE_TABLE_END},
};

// clang-format off
struct ScenePlacement gSolSanctumReliefs_Placements[] = {
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { JENNA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { SPRITE_KRADEN, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { SPRITE_STONE_STATUE, 0x0, ACTOR_STAND,
        PIXELS(744), 0, PIXELS(56), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gSolSanctumReliefs_FirstReliefRoomPlacements[] = {
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { JENNA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { SPRITE_KRADEN, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1512), 0, PIXELS(136), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1832), 0, PIXELS(136), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1560), 0, PIXELS(136), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1784), 0, PIXELS(136), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1640), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1640), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1640), 0, PIXELS(136), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1672), 0, PIXELS(88), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1704), 0, PIXELS(136), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1704), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1704), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SCENE_TABLE_END },
};

struct ScenePlacement gSolSanctumReliefs_SecondReliefRoomPlacements[] = {
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { JENNA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { SPRITE_KRADEN, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1608), 0, PIXELS(520), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1672), 0, PIXELS(520), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1736), 0, PIXELS(520), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1800), 0, PIXELS(520), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1864), 0, PIXELS(520), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1928), 0, PIXELS(520), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SCENE_TABLE_END },
};

struct ScenePlacement gSolEntrance_Placements[] = {
    { SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gSolSanctumEntrance_FallbackEvents[] = {
    { SCENE_EVENTS_END },
};

struct SceneEvent gSolEntrance_Events[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, SOL_ENTRANCE_TO_VALE },
    { TOUCH_WALKING_INTO(FACING_NORTH), SOL_ENTRANCE_CELL_DOOR, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OpenDoorAndEnter) },
    { SEARCH(SEARCH_UNNAMED), SOL_ENTRANCE_CELL_LOCKED_DOOR, CONDITION_ALWAYS, EVENT_SCRIPT(Event_DoorTightlyLocked) },
    { EVENT_TALK, ACTOR_WARNING_MAN, CONDITION_ALWAYS, MSGNO(MsgValeItIsVeryDangerousToGo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WARNING_MAN, CONDITION_ALWAYS, MSGNO(MsgValeIAmAllAloneInThis) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gSolSanctumReliefs_Events[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_CORRIDOR_WEST },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_CHEST_ROOM_TO_FIRST_RELIEF_ROOM },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_STATUE_NOOK_TO_FIRST_RELIEF_ROOM },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_STEPPING_STONE_ROOM_TO_ENTRANCE_ROOM },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_CORRIDOR_EAST },
    { EVENT_EXIT, 6, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_STEPS_ROOM_TO_FIRST_RELIEF_ROOM },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED, 7, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_MAZE_WEST },
    { EVENT_EXIT, 8, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_ENTRANCE },
    { EVENT_EXIT, 9, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_ENTRANCE_ROOM_TO_STEPPING_STONE_ROOM },
    { EVENT_EXIT, 10, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_ROUND_PASSAGE_TO_SECOND_RELIEF_ROOM },
    { EVENT_EXIT, 11, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_TO_CHEST_ROOM },
    { EVENT_EXIT, 12, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_TO_STATUE_NOOK },
    { EVENT_EXIT, 13, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_TO_STEPS_ROOM },
    { EVENT_EXIT, 14, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_MAZE_EAST },
    { EVENT_EXIT, 15, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_SECOND_RELIEF_ROOM_TO_ROUND_PASSAGE },
    { EVENT_EXIT, 16, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_LUNA_SOL_ROOMS },
    { SEARCH(SEARCH_CHEST), SOL_SANCTUM_CELL_CHEST, FLAG_SOL_SANCTUM_FIRST_SMALL_JEWEL_FOUND, FIND_ITEM(ITEM_SMALL_JEWEL) },
    { USE_ITEM(ITEM_SMALL_JEWEL), SOL_SANCTUM_ITEM_OWN_MESSAGE | SOL_SANCTUM_CELL_FIRST_RELIEF, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SetFirstSmallJewel) },
    { SEARCH(SEARCH_UNNAMED), SOL_SANCTUM_CELL_FIRST_RELIEF, CONDITION_ALWAYS, EVENT_SCRIPT(Event_InspectFirstMinotaurRelief) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gSolSanctumReliefs_FirstReliefRoomEvents[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_CORRIDOR_WEST },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_CHEST_ROOM_TO_FIRST_RELIEF_ROOM },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_STATUE_NOOK_TO_FIRST_RELIEF_ROOM },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_STEPPING_STONE_ROOM_TO_ENTRANCE_ROOM },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_CORRIDOR_EAST },
    { EVENT_EXIT, 6, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_STEPS_ROOM_TO_FIRST_RELIEF_ROOM },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED, 7, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_MAZE_WEST },
    { EVENT_EXIT, 8, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_ENTRANCE },
    { EVENT_EXIT, 9, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_ENTRANCE_ROOM_TO_STEPPING_STONE_ROOM },
    { EVENT_EXIT, 10, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_ROUND_PASSAGE_TO_SECOND_RELIEF_ROOM },
    { EVENT_EXIT, 11, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_TO_CHEST_ROOM },
    { EVENT_EXIT, 12, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_TO_STATUE_NOOK },
    { EVENT_EXIT, 13, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_TO_STEPS_ROOM },
    { EVENT_EXIT, 14, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_MAZE_EAST },
    { EVENT_EXIT, 15, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_SECOND_RELIEF_ROOM_TO_ROUND_PASSAGE },
    { EVENT_EXIT, 16, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_LUNA_SOL_ROOMS },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_WEST_STATUE, FLAG_SCENE_TEMP_34, EVENT_SCRIPT(Event_WestStatueMoved) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_EAST_STATUE, FLAG_SCENE_TEMP_34, EVENT_SCRIPT(Event_EastStatueMoved) },
    { TOUCH_WALKING, SOL_SANCTUM_CELL_FIRST_ROOM_STATUE, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushStatue) },
    { TOUCH_WALKING_INTO(FACING_EAST), SOL_SANCTUM_CELL_FIRST_ROOM_STATUE_EAST, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushStatue) },
    { TOUCH_WALKING_INTO(FACING_WEST), SOL_SANCTUM_CELL_FIRST_ROOM_STATUE_WEST, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushStatue) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gSolSanctumReliefs_SecondReliefRoomEvents[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_CORRIDOR_WEST },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_CHEST_ROOM_TO_FIRST_RELIEF_ROOM },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_STATUE_NOOK_TO_FIRST_RELIEF_ROOM },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_STEPPING_STONE_ROOM_TO_ENTRANCE_ROOM },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_CORRIDOR_EAST },
    { EVENT_EXIT, 6, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_STEPS_ROOM_TO_FIRST_RELIEF_ROOM },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED, 7, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_MAZE_WEST },
    { EVENT_EXIT, 8, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_ENTRANCE },
    { EVENT_EXIT, 9, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_ENTRANCE_ROOM_TO_STEPPING_STONE_ROOM },
    { EVENT_EXIT, 10, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_ROUND_PASSAGE_TO_SECOND_RELIEF_ROOM },
    { EVENT_EXIT, 11, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_TO_CHEST_ROOM },
    { EVENT_EXIT, 12, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_TO_STATUE_NOOK },
    { EVENT_EXIT, 13, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_TO_STEPS_ROOM },
    { EVENT_EXIT, 14, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_MAZE_EAST },
    { EVENT_EXIT, 15, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_SECOND_RELIEF_ROOM_TO_ROUND_PASSAGE },
    { EVENT_EXIT, 16, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_LUNA_SOL_ROOMS },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ROW_STATUE_1, CONDITION_ALWAYS, EVENT_SCRIPT(Event_RowStatue1Moved) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ROW_STATUE_2, CONDITION_ALWAYS, EVENT_SCRIPT(Event_RowStatue2Moved) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ROW_STATUE_3, CONDITION_ALWAYS, EVENT_SCRIPT(Event_RowStatue3Moved) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ROW_STATUE_4, CONDITION_ALWAYS, EVENT_SCRIPT(Event_RowStatue4Moved) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ROW_STATUE_5, CONDITION_ALWAYS, EVENT_SCRIPT(Event_RowStatue5Moved) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ROW_STATUE_6, CONDITION_ALWAYS, EVENT_SCRIPT(Event_RowStatue6Moved) },
    { TOUCH_WALKING, SOL_SANCTUM_CELL_ROW_STATUE, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushStatue) },
    { USE_ITEM(ITEM_SMALL_JEWEL), SOL_SANCTUM_ITEM_OWN_MESSAGE | SOL_SANCTUM_CELL_SECOND_RELIEF, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SetSecondSmallJewel) },
    { SEARCH(SEARCH_UNNAMED), SOL_SANCTUM_CELL_SECOND_RELIEF, CONDITION_ALWAYS, EVENT_SCRIPT(Event_InspectSecondMinotaurRelief) },
    { SCENE_EVENTS_END },
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
