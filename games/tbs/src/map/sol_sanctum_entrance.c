/// @file
/// Sol Entrance and the first rooms of Sol Sanctum, two maps one overlay
/// serves, chosen by gSaveBlock.mapId in every entry point.
///
/// - Sol Entrance (MapId_SolEntrance), the mountainside before the
///   sanctum's door, reached from Vale (VALE_TO_SOL_ENTRANCE). Walking north
///   into the door opens it and takes the leader in
///   (EventScript_OpenDoorAndEnter); searching the other door finds it "tightly
///   locked". Retreat does not work here. The way down leads to MapId_Vale,
///   except between the eruption (FLAG_SOL_SANCTUM_ERUPTED) and the party
///   leaving Vale (FLAG_VALE_PARTY_LEFT), when it leads to MapId_ValeEruption.
///   After the eruption the ground rumbles now and then; once the party has
///   left Vale a man stands in the path and warns everyone away.
/// - Sol Sanctum (MapId_SolSanctumReliefs), the rooms inside the door, which
///   Isaac, Garet and Jenna explore with Kraden. Two minotaur reliefs each have
///   an empty eye socket, and a Small Jewel set in it opens the way on: the
///   first from this map's chest (FLAG_SOL_SANCTUM_FIRST_SMALL_JEWEL_SET), the
///   second from the chest in MapId_SolSanctumMaze
///   (FLAG_SOL_SANCTUM_SECOND_SMALL_JEWEL_SET). Kraden has a scene on the first
///   entry at the door (SolSanctumEntrance_IntroduceSanctum), in the first
///   relief room (SolSanctumEntrance_KradenSuspectsHiddenPassage) and in the
///   second (SolSanctumEntrance_KradenSeesStatuesOutOfReach). Two statues in
///   the first relief room and the row of six in the second can be pushed, or
///   moved with Move, a cell east or west. Its warps lead to
///   MapId_SolSanctumCorridor, MapId_SolSanctumMaze, Sol Entrance and
///   MapId_SolSanctumLunaSolRooms; the rest lead between its own rooms.
///
/// MapId_SolSanctumReliefs is one map, but each relief room has object event
/// template and event tables of its own:
/// SolSanctumEntrance_GetObjectEventTemplates and _GetEvents choose them by
/// the warp destination the party came in at, so a room's statues and their
/// events exist only while the party is in it. Each pushable statue's column
/// is kept in two map section flags, one for each side, which
/// SolSanctumEntrance_RestoreInteriorState reads to put it back on entry;
/// Map_ResetFlagsOnEnter clears them when the party enters another map
/// section, so the statues are back at their starts on the next visit.
/// Searching a relief while its Small Jewel is held asks for the field menu as
/// the search ends, so that the jewel can be used on it at once.
///
/// Once Isaac and Garet are back in Vale (FLAG_VALE_RETURNED_FROM_MT_ALEPH)
/// the door no longer opens: Sol Entrance's setup makes the three wall cells
/// across the door MB_SOL_ENTRANCE_LOCKED_DOOR, copied from the other door,
/// and the three MB_SOL_ENTRANCE_DOOR cells before it plain ground, so the
/// leader walks up to the door and stops, and searching it finds it "tightly
/// locked" (render probe sol-entrance).
///
/// Open questions: none.

#include "save_block.h"
#include "unit/field/field.h"
#include "unit/import.h"
#include "unit/map/sol_sanctum_entrance.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/field_services.h"
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
#include "script_layer.h"
#include "map_tables.h"
#include "field_service.h"
#include "sol_sanctum_entrance.h"
#include "map_ids.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "object_event.h"
#include "fixed_math.h"
#include "macros.h"
#include "songs.h"
#include "item_ids.h"

// Map section flags: Map_ResetFlagsOnEnter clears 0x300 to 0x3ff when the
// party enters another map section, and other map sections use the numbers for
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

// Map flags, cleared on every ordinary arrival:
// - FLAG_SOL_SANCTUM_IN_SECOND_RELIEF_ROOM: set on entering the second relief
//   room; the Move rows of the first relief room apply only while it is clear,
//   which it always is there: only the second room's warp destinations set it,
//   every ordinary warp destination into the first room clears it, and a return
//   after a battle comes back by the warp destination the party last came in
//   by, so the condition never stops a Move.

// A warp's flag word of -1 stands for no flag: GameFlag_GetFar reads it as
// flag 0xfff, which nothing sets, so the warp is always taken.
#define WARP_FLAG_NONE 0xffffffff

/// The metatile behaviors of the cells the Sol Sanctum entrance's coord, bg
/// and item events answer; behaviors 1 to 16 in both maps are the warps, in
/// the warp table's order.
enum SolEntranceMetatileBehavior {
  /// ★ Named from what the player sees: the door that slides open as the
  /// leader walks into it (EventScript_OpenDoorAndEnter)
  MB_SOL_ENTRANCE_DOOR = 2,
  /// ★ Named from the game: "The door is tightly locked."
  /// (MsgFieldDoorTightlyLocked)
  MB_SOL_ENTRANCE_LOCKED_DOOR = 10
};

/// The metatile behaviors of the cells the relief rooms' coord, bg and item
/// events answer.
enum SolSanctumReliefsMetatileBehavior {
  /// ★ Named from the game: "It's a relief of a minotaur."
  /// (MsgSolSanctumMinotaurReliefOneEye)
  MB_SOL_SANCTUM_RELIEFS_FIRST_RELIEF = 20,
  /// ★ Named from the game: "It's a relief of a minotaur."
  /// (MsgSolSanctumMinotaurReliefOneEye)
  MB_SOL_SANCTUM_RELIEFS_SECOND_RELIEF = 21,
  /// The first relief room's statues, pushed from any side.
  MB_SOL_SANCTUM_RELIEFS_FIRST_ROOM_STATUE = 22,
  /// The first relief room's statues, pushed east only.
  MB_SOL_SANCTUM_RELIEFS_FIRST_ROOM_STATUE_EAST = 23,
  /// The second relief room's row of statues.
  MB_SOL_SANCTUM_RELIEFS_ROW_STATUE = 24,
  /// The first relief room's statues, pushed west only.
  MB_SOL_SANCTUM_RELIEFS_FIRST_ROOM_STATUE_WEST = 25,
  /// ★ Named from the game: "checked the chest" (MsgVaultIsaacCheckedChest)
  MB_SOL_SANCTUM_RELIEFS_CHEST = 100
};

/// An item event's trigger bit that skips the `<actor> uses <item>.` message,
/// so the script shows its own (Field_UseItem).
#define SOL_SANCTUM_ITEM_OWN_MESSAGE 0x400

static void SolSanctumEntrance_ApplyEntryState(void);
static void SolSanctumEntrance_RestoreInteriorState(void);
static void SolSanctumEntrance_IntroduceSanctum(void);
static void SolSanctumEntrance_KradenSuspectsHiddenPassage(void);
static void SolSanctumEntrance_KradenSeesStatuesOutOfReach(void);
static void SolSanctumEntrance_RestoreRowStatueCells(s32 placeObjectEvents);
static void SolSanctumEntrance_UpdateEruptionRumbles(void);

s32 gSolSanctumEntrance_RumbleTimer __attribute__((section(".bss")));

void FieldEffect_InitializeSparkles();

/// Returns Sol Entrance's warp destinations (MapId_SolEntrance) or Sol
/// Sanctum's (MapId_SolSanctumReliefs).
const struct MapWarpDest *SolSanctumEntrance_GetWarpDests(void) {
  s16 mapId = gSaveBlock.mapId;

  if (mapId == (s32)&MapId_SolEntrance) {
    return gSolEntrance_WarpDests;
  }
  if (mapId == (s32)&MapId_SolSanctumReliefs) {
    return gSolSanctumReliefs_WarpDests;
  }
  return gSolSanctumEntrance_FallbackWarpDests;
}

const struct MapWarpRegion *SolSanctumEntrance_GetRegions(void) { return 0; }

const u32 *SolSanctumEntrance_GetWarps(void) {
  return gSolSanctumEntrance_Warps;
}

/// Selects the object events for the map and, in Sol Sanctum, the room. The
/// first relief room's warp destinations place Kraden and that room's eleven
/// statues, the second relief room's Kraden and the row of six; the rest of Sol
/// Sanctum places Kraden and one statue (LOCALID_STATUE), and Sol Entrance the
/// man who warns everyone away (LOCALID_WARNING_MAN).
const struct ObjectEventTemplate *
SolSanctumEntrance_GetObjectEventTemplates(void) {
  struct ObjectEventTemplate *table;
  s32 firstStatueRoomWarpDest =
      SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_FROM_CHEST_ROOM;

  if (gSaveBlock.mapId == (s32)&MapId_SolSanctumReliefs) {
    if (gSaveBlock.warpDest >= firstStatueRoomWarpDest) {
      if (gSaveBlock.warpDest >
          SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_FROM_STEPS_ROOM) {
        if (gSaveBlock.warpDest <=
            SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS) {
          return gSolSanctumReliefs_SecondReliefRoomObjectEventTemplates;
        }
      } else {
        return gSolSanctumReliefs_FirstReliefRoomObjectEventTemplates;
      }
    }
    table = gSolSanctumReliefs_ObjectEventTemplates;
    Map_AssignViewFlags(table);
    return table;
  }
  if (gSaveBlock.mapId == (s32)&MapId_SolEntrance) {
    return gSolEntrance_ObjectEventTemplates;
  }
  return gSolSanctumEntrance_FallbackObjectEventTemplates;
}

/// Selects the events for the map and room, by the same warp destinations as
/// SolSanctumEntrance_GetObjectEventTemplates.
const struct MapEvent *SolSanctumEntrance_GetEvents(void) {
  s32 firstStatueRoomWarpDest =
      SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_FROM_CHEST_ROOM;

  if (gSaveBlock.mapId == (s32)&MapId_SolEntrance) {
    return gSolEntrance_Events;
  }
  if (gSaveBlock.mapId == (s32)&MapId_SolSanctumReliefs) {
    if (gSaveBlock.warpDest >= firstStatueRoomWarpDest) {
      if (gSaveBlock.warpDest >
          SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_FROM_STEPS_ROOM) {
        if (gSaveBlock.warpDest <=
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
static void EventScript_OpenDoorAndEnter(void) {
  s32 width;
  s32 height;

  FieldScript_Start();
  // The door opens over three frames.
  width = 3;
  height = 2;
  Script_PlaySong(SE_BOOM);
  MapGrid_CopyCellsTo(16, 28, 21, 3, width, height);
  WaitFrames(10);
  MapGrid_CopyCellsTo(16, 30, 21, 3, width, height);
  WaitFrames(10);
  MapGrid_CopyCellsTo(16, 32, 21, 3, width, height);
  WaitFrames(10);
  ObjectMotion_SetActionVariant(LOCALID_PARTY_LEADER, 2);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(3) / 5, PIXELS(3) / 10);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 120, 98);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_WALK);
  ObjectEvent_SetDestinationOffset(LOCALID_PARTY_LEADER, 0, -8);
  FieldScript_WaitFrames(10);
  FieldScript_FinishScreenTransition();
  FieldScript_WaitTransitionFrames();
  Import_ScriptRequestWarp(SOL_ENTRANCE_TO_SOL_SANCTUM_RELIEFS);
  FieldScript_End();
}

/// Sol Sanctum: searching the first minotaur relief shows one eye sparkling, or
/// both once the Small Jewel is set: "The right eye is sparkling, but there's a
/// hole where the left eye should be." Once the first Small Jewel is found, and
/// until it is set, the main menu opens as the search ends, so that the jewel
/// can be used.
static void EventScript_InspectFirstMinotaurRelief(void) {

  FieldScript_Start();

  if (GameFlag_Get(FLAG_SOL_SANCTUM_FIRST_SMALL_JEWEL_SET) != 0) {
    FieldMessage_Show(MsgSolSanctumMinotaurReliefBothEyes, 1);
  } else {
    FieldMessage_Show(MsgSolSanctumMinotaurReliefOneEye, 1);
    if (GameFlag_Get(FLAG_SOL_SANCTUM_FIRST_SMALL_JEWEL_FOUND) != 0) {
      u16 *menuRequest = &gFieldWork->menu;
      u16 open = 1;
      *menuRequest = open;
    }
  }

  FieldScript_End();
}

/// The room shakes as stone shifts; the leader looks around and jumps.
static inline void SolSanctumEntrance_ReactToOpeningPassage(void) {
  MapGrid_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
  FieldScript_WaitFrames(20);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  MapGrid_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 40);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_WEST, 20);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_EAST, 20);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 10);
  ObjectMotion_Launch(LOCALID_PARTY_LEADER, 4, 20);
  ObjectMotion_Launch(LOCALID_PARTY_LEADER, 6, 40);
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  FieldScript_WaitFrames(40);
}

/// Sol Sanctum: using the Small Jewel on the first relief sets it in, and a
/// passage opens. `<leader> set the small gem into the empty socket.`, "You
/// hear the sound of shifting stone." Does nothing until the first Small Jewel
/// is found, or once it is set. Sets FLAG_SOL_SANCTUM_FIRST_SMALL_JEWEL_SET and
/// FLAG_ITEM_USED_UP, which takes the jewel.
static void EventScript_SetFirstSmallJewel(void) {
  s32 message;
  s32 oneCell;
  s32 reliefWidth;

  if (GameFlag_Get(FLAG_SOL_SANCTUM_FIRST_SMALL_JEWEL_FOUND) == 0) {
  } else {
    if (Script_GetFlag(FLAG_SOL_SANCTUM_FIRST_SMALL_JEWEL_SET) != 0) {
    } else {
      FieldScript_Start();
      Field_EndBusyAndReveal();
      // The eye sparkles, then the wall opens.
      oneCell = 1;
      Script_PlaySong(SE_GEM_SET);
      MapGrid_CopyCellsTo(0, 70, 30, 42, oneCell, oneCell);
      MapGrid_CenterOnFollowedPoint();
      FieldScript_WaitFrames(40);
      message = MsgSolSanctumSetSmallGem;
      FieldMessage_Show(message, 1);
      FieldScript_WaitFrames(20);
      reliefWidth = 3;
      Script_PlaySong(SE_STONE_RUMBLE);
      MapGrid_CopyCellsTo(0, 29, 3, 1, reliefWidth, 2);
      MapGrid_CopyCellAttributes(0, 29, 3, 2, reliefWidth, oneCell);
      MapGrid_CopyCellsTo(1, 109, 4, 81, oneCell, oneCell);
      MapGrid_CenterOnFollowedPoint();
      SolSanctumEntrance_ReactToOpeningPassage();
      FieldMessage_Show(message + 1, 1);
      GameFlag_Set(FLAG_ITEM_USED_UP);
      GameFlag_Set(FLAG_SOL_SANCTUM_FIRST_SMALL_JEWEL_SET);
      FieldScript_End();
    }
  }
}

/// Sol Sanctum, the second relief room: searching the second minotaur
/// relief, as the first. Once the second Small Jewel is found,
/// and until it is set, it opens the main menu.
static void EventScript_InspectSecondMinotaurRelief(void) {

  struct FieldWork *work;

  FieldScript_Start();

  if (GameFlag_Get(FLAG_SOL_SANCTUM_SECOND_SMALL_JEWEL_SET) != 0) {
    FieldMessage_Show(MsgSolSanctumMinotaurReliefBothEyes, 1);
  } else if (GameFlag_Get(FLAG_SOL_SANCTUM_SECOND_SMALL_JEWEL_FOUND) != 0) {
    work = gFieldWork;
    FieldMessage_Show(MsgSolSanctumMinotaurReliefOneEye, 1);
    {

      u16 *menuRequest = &work->menu;
      s32 open = 1;
      *menuRequest = open;
    }
  } else {
    FieldMessage_Show(MsgSolSanctumMinotaurReliefOneEye, 1);
  }

  FieldScript_End();
}

/// Sol Sanctum, the second relief room: using the Small Jewel on the
/// second relief, as the first, and a doorway opens. Only once the
/// second Small Jewel is found and until it is set. Sets
/// FLAG_SOL_SANCTUM_SECOND_SMALL_JEWEL_SET and FLAG_ITEM_USED_UP.
static void EventScript_SetSecondSmallJewel(void) {
  s32 message;

  if (GameFlag_Get(FLAG_SOL_SANCTUM_SECOND_SMALL_JEWEL_FOUND) != 0 &&
      Script_GetFlag(FLAG_SOL_SANCTUM_SECOND_SMALL_JEWEL_SET) == 0) {
    FieldScript_Start();
    Field_EndBusyAndReveal();
    // The eye sparkles, then the doorway opens.
    Sound_PlaySong(SE_GEM_SET);
    MapGrid_CopyCellsTo(0, 71, 100, 71, 1, 1);
    MapGrid_CenterOnFollowedPoint();
    FieldScript_WaitFrames(40);
    message = MsgSolSanctumSetSmallGem;
    FieldMessage_Show(message, 1);
    FieldScript_WaitFrames(20);
    Sound_PlaySong(SE_STONE_RUMBLE);
    MapGrid_CopyCellsTo(122, 20, 120, 30, 1, 2);
    MapGrid_CopyCellAttributes(122, 20, 1, 2, 120, 30);
    MapGrid_CenterOnFollowedPoint();
    SolSanctumEntrance_ReactToOpeningPassage();
    FieldMessage_Show(message + 1, 1);
    GameFlag_Set(FLAG_ITEM_USED_UP);
    GameFlag_Set(FLAG_SOL_SANCTUM_SECOND_SMALL_JEWEL_SET);
    FieldScript_End();
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
/// (LOCALID_WEST_STATUE) while FLAG_SOL_SANCTUM_IN_SECOND_RELIEF_ROOM is clear,
/// and called by EventScript_PushStatue there. The column it stops in is saved:
/// sets FLAG_SOL_SANCTUM_WEST_STATUE_AT_EAST or _AT_WEST, or clears both when
/// it stands in neither column.
static void EventScript_WestStatueMoved(void) {
  struct ObjectEvent *statue = ObjectEvent_GetById(LOCALID_WEST_STATUE);
  s32 column;

  if (statue == NULL) {
    return;
  }
  column = statue->x.fixed >> 20;
  SolSanctumEntrance_RecordStatueColumn(
      column, FLAG_SOL_SANCTUM_WEST_STATUE_AT_EAST, 93, 95);
}

/// Sol Sanctum, the first relief room: Move on the east statue
/// (LOCALID_EAST_STATUE), as the west one. Sets
/// FLAG_SOL_SANCTUM_EAST_STATUE_AT_EAST or _AT_WEST, or clears both.
static void EventScript_EastStatueMoved(void) {
  struct ObjectEvent *statue = ObjectEvent_GetById(LOCALID_EAST_STATUE);
  s32 column;

  if (statue == NULL) {
    return;
  }
  column = statue->x.fixed >> 20;
  Script_ClearFlag(FLAG_SOL_SANCTUM_EAST_STATUE_AT_EAST);
  Script_ClearFlag(FLAG_SOL_SANCTUM_EAST_STATUE_AT_WEST);
  if (column == 115) {
    GameFlag_Set(FLAG_SOL_SANCTUM_EAST_STATUE_AT_EAST);
  } else if (column == 113) {
    GameFlag_Set(FLAG_SOL_SANCTUM_EAST_STATUE_AT_WEST);
  }
}

/// Sol Sanctum, the second relief room: Move on the first statue of the
/// row (LOCALID_ROW_STATUE_1), and called by EventScript_PushStatue there. The
/// column it stops in is saved and the cells under all six are set again. Sets
/// FLAG_SOL_SANCTUM_ROW_STATUE_1_AT_EAST or _AT_WEST, or clears both.
static void EventScript_RowStatue1Moved(void) {
  struct ObjectEvent *statue = ObjectEvent_GetById(LOCALID_ROW_STATUE_1);
  s32 column;

  if (statue == NULL) {
    return;
  }
  column = statue->x.fixed >> 20;
  SolSanctumEntrance_RecordStatueColumn(
      column, FLAG_SOL_SANCTUM_ROW_STATUE_1_AT_EAST, 99, 101);
  SolSanctumEntrance_RestoreRowStatueCells(0);
}

/// Sol Sanctum, the second relief room: as EventScript_RowStatue1Moved, for
/// the row's second statue (LOCALID_ROW_STATUE_2).
static void EventScript_RowStatue2Moved(void) {
  struct ObjectEvent *statue = ObjectEvent_GetById(LOCALID_ROW_STATUE_2);
  s32 column;

  if (statue == NULL) {
    return;
  }
  column = statue->x.fixed >> 20;
  SolSanctumEntrance_RecordStatueColumn(
      column, FLAG_SOL_SANCTUM_ROW_STATUE_2_AT_EAST, 103, 105);
  SolSanctumEntrance_RestoreRowStatueCells(0);
}

/// Sol Sanctum, the second relief room: as EventScript_RowStatue1Moved, for
/// the row's third statue (LOCALID_ROW_STATUE_3).
static void EventScript_RowStatue3Moved(void) {
  struct ObjectEvent *statue = ObjectEvent_GetById(LOCALID_ROW_STATUE_3);
  s32 column;

  if (statue == NULL) {
    return;
  }
  column = statue->x.fixed >> 20;
  SolSanctumEntrance_RecordStatueColumn(
      column, FLAG_SOL_SANCTUM_ROW_STATUE_3_AT_EAST, 107, 109);
  SolSanctumEntrance_RestoreRowStatueCells(0);
}

/// Sol Sanctum, the second relief room: as EventScript_RowStatue1Moved, for
/// the row's fourth statue (LOCALID_ROW_STATUE_4).
static void EventScript_RowStatue4Moved(void) {
  struct ObjectEvent *statue = ObjectEvent_GetById(LOCALID_ROW_STATUE_4);
  s32 column;

  if (statue == NULL) {
    return;
  }
  column = statue->x.fixed >> 20;
  SolSanctumEntrance_RecordStatueColumn(
      column, FLAG_SOL_SANCTUM_ROW_STATUE_4_AT_EAST, 111, 113);
  SolSanctumEntrance_RestoreRowStatueCells(0);
}

/// Sol Sanctum, the second relief room: as EventScript_RowStatue1Moved, for
/// the row's fifth statue (LOCALID_ROW_STATUE_5).
static void EventScript_RowStatue5Moved(void) {
  struct ObjectEvent *statue = ObjectEvent_GetById(LOCALID_ROW_STATUE_5);
  s32 column;

  if (statue == NULL) {
    return;
  }
  column = statue->x.fixed >> 20;
  SolSanctumEntrance_RecordStatueColumn(
      column, FLAG_SOL_SANCTUM_ROW_STATUE_5_AT_EAST, 115, 117);
  SolSanctumEntrance_RestoreRowStatueCells(0);
}

/// Sol Sanctum, the second relief room: as EventScript_RowStatue1Moved, for
/// the row's sixth statue (LOCALID_ROW_STATUE_6).
static void EventScript_RowStatue6Moved(void) {
  struct ObjectEvent *statue = ObjectEvent_GetById(LOCALID_ROW_STATUE_6);
  s32 column;

  if (statue == NULL)
    return;
  column = statue->x.fixed >> 20;
  SolSanctumEntrance_RecordStatueColumn(
      column, FLAG_SOL_SANCTUM_ROW_STATUE_6_AT_EAST, 119, 121);
  SolSanctumEntrance_RestoreRowStatueCells(0);
}

#include "common/object_findcell.h"

/// The party has entered the second relief room, which turns off Move on the
/// first relief room's statues.
/// ★ Named from the game: "It's a relief of a minotaur."
/// (MsgSolSanctumMinotaurReliefOneEye)
#define FLAG_SOL_SANCTUM_IN_SECOND_RELIEF_ROOM FLAG_MAP_TEMP_34

/// Sol Sanctum, the two relief rooms: walking into a statue's cell
/// pushes it one cell, the leader behind it, if nothing is in the way, and its
/// column is saved through the room's Moved events.
static void EventScript_PushStatue(void) {
  struct ObjectEvent *leader;
  struct ObjectEvent *statue;
  s32 stoppedVelocity;
  s32 packedStep;
  u32 direction;
  struct FieldPosition target;
  struct FieldPosition *destination;

  // Find the statue one map cell ahead in the leader's facing direction.
  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  direction = leader->facing >> 12;
  statue = ObjectEvent_FindAtCell(
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
    if (FieldObject_CheckMovementCollision(statue, destination) <= 0) {
      Object_SelectAnimation(leader, ANIM_PUSH);
      WaitFrames(15);
      Sound_PlaySong(SE_PUSH);
      statue->speed = PIXELS(1) / 5;
      statue->acceleration = PIXELS(1) / 5;
      Object_SetPosition(statue, destination->x, destination->y,
                         destination->z);
      leader->speed = PIXELS(1) / 5;
      leader->acceleration = PIXELS(1) / 5;
      Object_SetPosition(leader, destination->x, destination->y,
                         destination->z);
      Object_WaitForMove(statue);
      statue->x.fixed = destination->x;
      statue->z.fixed = destination->z;
      statue->velocityX = stoppedVelocity;
      statue->velocityZ = stoppedVelocity;
      Object_SelectAnimation(leader, ANIM_STAND);
      // Save the moved statue in the flag set belonging to this room.
      switch (gSaveBlock.warpDest) {
      case SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_FROM_CHEST_ROOM:
      case SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_FROM_STATUE_NOOK:
      case SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_FROM_STEPS_ROOM:
        EventScript_WestStatueMoved();
        EventScript_EastStatueMoved();
        break;
      case SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_MAZE_EAST:
      case SOL_SANCTUM_RELIEFS_SECOND_RELIEF_ROOM_FROM_ROUND_PASSAGE:
      case SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS:
        EventScript_RowStatue1Moved();
        EventScript_RowStatue2Moved();
        EventScript_RowStatue3Moved();
        EventScript_RowStatue4Moved();
        EventScript_RowStatue5Moved();
        EventScript_RowStatue6Moved();
        break;
      }
    }
  }
}

/// Sets up Sol Entrance or Sol Sanctum. Called by Map_Run once the
/// object events exist.
s32 SolSanctumEntrance_Initialize(void) {
  s32 mapId = gSaveBlock.mapId;

  if (mapId == (s32)&MapId_SolEntrance) {
    SolSanctumEntrance_ApplyEntryState();
  } else if (mapId == (s32)&MapId_SolSanctumReliefs) {
    SolSanctumEntrance_RestoreInteriorState();
  }
  return 0;
}

/// Sets up Sol Entrance (MapId_SolEntrance): Retreat is blocked
/// (FLAG_RETREAT_BLOCKED), and after the eruption
/// SolSanctumEntrance_UpdateEruptionRumbles shakes the ground now and then.
/// Once Isaac and Garet are back in Vale (FLAG_VALE_RETURNED_FROM_MT_ALEPH),
/// the door is locked: its row becomes MB_SOL_ENTRANCE_LOCKED_DOOR and the
/// cells before it plain ground; once the party has left Vale
/// (FLAG_VALE_PARTY_LEFT), the
/// man who warns everyone away, placed off the map until then, stands in the
/// path and the three cells across it change.
static void SolSanctumEntrance_ApplyEntryState(void) {
  // Retreat does not work here.
  GameFlag_Set(FLAG_RETREAT_BLOCKED);
  gFieldWork->transition = MAP_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  if (GameFlag_Get(FLAG_SOL_SANCTUM_ERUPTED) != 0) {
    gSolSanctumEntrance_RumbleTimer = 0;
    Task_AddCallback(SolSanctumEntrance_UpdateEruptionRumbles, 0xc80);
  }
  if (GameFlag_Get(FLAG_VALE_RETURNED_FROM_MT_ALEPH) != 0) {
    MapGrid_CopyCellAttributes(5, 6, 1, 1, 6, 6);
    MapGrid_CopyCellAttributes(5, 6, 1, 1, 7, 6);
    MapGrid_CopyCellAttributes(5, 6, 1, 1, 8, 6);
    MapGrid_CopyCellAttributes(0, 1, 3, 1, 6, 5);
  }
  if (GameFlag_Get(FLAG_VALE_PARTY_LEFT) != 0) {
    ObjectEvent_SetPosition(LOCALID_WARNING_MAN, PIXELS(120), PIXELS(232));
    MapGrid_CopyCellAttributes(2, 10, 1, 1, 6, 14);
    MapGrid_CopyCellAttributes(2, 10, 1, 1, 7, 14);
    MapGrid_CopyCellAttributes(2, 10, 1, 1, 8, 14);
  }
}

/// Sets up Sol Sanctum (MapId_SolSanctumReliefs): after the eruption its rooms
/// rumble and shake. SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_CORRIDOR_WEST and
/// SOL_SANCTUM_RELIEFS_CHEST_ROOM_FROM_FIRST_RELIEF_ROOM show the first
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
  s32 warpDest;
  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  if (GameFlag_Get(FLAG_SOL_SANCTUM_ERUPTED) != 0) {
    Map_QueueMusic(SE_RUMBLE);
    MapGrid_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
    FieldEffect_InitializeSparkles();
  }

  warpDest = gSaveBlock.warpDest;
  switch (warpDest) {
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
    ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_STATUE), 0);
    break;

  case SOL_SANCTUM_RELIEFS_FROM_SOL_ENTRANCE:
    gSaveBlock.retreatMap = (s32)&MapId_SolSanctumReliefs;
    gSaveBlock.retreatWarpDest = SOL_SANCTUM_RELIEFS_FROM_SOL_ENTRANCE;
    if (GameFlag_Get(FLAG_SOL_SANCTUM_ENTERED) == 0)
      SolSanctumEntrance_IntroduceSanctum();
    break;

  case SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_FROM_CHEST_ROOM:
  case SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_FROM_STATUE_NOOK:
  case SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_FROM_STEPS_ROOM:
    // Put back this room's statues after its introduction has played.
    ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_WEST_STATUE), 0);
    ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_EAST_STATUE), 0);
    ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_FIRST_FIXED_STATUE),
                               0);
    ObjectEvent_SetSpriteFlags(
        ObjectEvent_GetById(LOCALID_FIRST_FIXED_STATUE + 1), 0);
    ObjectEvent_SetSpriteFlags(
        ObjectEvent_GetById(LOCALID_FIRST_FIXED_STATUE + 2), 0);
    ObjectEvent_SetSpriteFlags(
        ObjectEvent_GetById(LOCALID_FIRST_FIXED_STATUE + 3), 0);
    ObjectEvent_SetSpriteFlags(
        ObjectEvent_GetById(LOCALID_FIRST_FIXED_STATUE + 4), 0);
    ObjectEvent_SetSpriteFlags(
        ObjectEvent_GetById(LOCALID_FIRST_FIXED_STATUE + 5), 0);
    ObjectEvent_SetSpriteFlags(
        ObjectEvent_GetById(LOCALID_FIRST_FIXED_STATUE + 6), 0);
    ObjectEvent_SetSpriteFlags(
        ObjectEvent_GetById(LOCALID_FIRST_FIXED_STATUE + 7), 0);
    ObjectEvent_SetSpriteFlags(
        ObjectEvent_GetById(LOCALID_FIRST_FIXED_STATUE + 8), 0);
    if (GameFlag_Get(FLAG_SEARCHING_FOR_HIDDEN_PASSAGE) == 0)
      SolSanctumEntrance_KradenSuspectsHiddenPassage();
    if (GameFlag_Get(FLAG_SOL_SANCTUM_WEST_STATUE_AT_WEST) != 0)
      ObjectEvent_SetPosition(LOCALID_WEST_STATUE, PIXELS(1496), PIXELS(136));
    else if (GameFlag_Get(FLAG_SOL_SANCTUM_WEST_STATUE_AT_EAST) != 0)
      ObjectEvent_SetPosition(LOCALID_WEST_STATUE, PIXELS(1528), PIXELS(136));
    if (GameFlag_Get(FLAG_SOL_SANCTUM_EAST_STATUE_AT_WEST) != 0)
      ObjectEvent_SetPosition(LOCALID_EAST_STATUE, PIXELS(1816), PIXELS(136));
    else if (GameFlag_Get(FLAG_SOL_SANCTUM_EAST_STATUE_AT_EAST) != 0)
      ObjectEvent_SetPosition(LOCALID_EAST_STATUE, PIXELS(1848), PIXELS(136));
    break;

  case SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_MAZE_EAST:
  case SOL_SANCTUM_RELIEFS_SECOND_RELIEF_ROOM_FROM_ROUND_PASSAGE:
  case SOL_SANCTUM_RELIEFS_FROM_SOL_SANCTUM_LUNA_SOL_ROOMS:
    // Restore the row of statues and the second gem's doorway.
    ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_ROW_STATUE_1), 0);
    ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_ROW_STATUE_2), 0);
    ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_ROW_STATUE_3), 0);
    ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_ROW_STATUE_4), 0);
    ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_ROW_STATUE_5), 0);
    ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_ROW_STATUE_6), 0);
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
/// set, which it sets; clears FLAG_SHOW_LOCATION_NAME.
static void SolSanctumEntrance_IntroduceSanctum(void) {
  struct ObjectEvent *leader;

  FieldScript_Start();
  gFieldWork->transition = MAP_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  FieldScript_StartScreenTransition();
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, 0);
  FieldScript_WaitFrames(4);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  // Gather the followers around the leader for Kraden's introduction.
  FieldCamera_SetSpeed(PIXELS(3) / 5, PIXELS(3) / 40);
  FieldCamera_MoveTo(PIXELS(1224), -1, PIXELS(136), 1);
  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (leader != NULL) {
    ObjectEvent_SetPosition(LOCALID_KRADEN, leader->x.fixed, leader->z.fixed);
  }
  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (leader != NULL) {
    ObjectEvent_SetPosition(LOCALID_JENNA, leader->x.fixed, leader->z.fixed);
  }
  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (leader != NULL) {
    ObjectEvent_SetPosition(LOCALID_GARET, leader->x.fixed, leader->z.fixed);
  }
  ObjectEvent_SetSpeed(LOCALID_KRADEN, PIXELS(3) / 5, PIXELS(3) / 10);
  ObjectEvent_SetSpeed(LOCALID_JENNA, PIXELS(3) / 5, PIXELS(3) / 10);
  ObjectEvent_SetSpeed(LOCALID_GARET, PIXELS(3) / 5, PIXELS(3) / 10);
  Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_WALK);
  Import_ObjectEventSetAnimation(LOCALID_JENNA, ANIM_WALK);
  Import_ObjectEventSetAnimation(LOCALID_KRADEN, ANIM_WALK);
  ObjectEvent_SetDestinationOffset(LOCALID_GARET, -16, 0);
  ObjectEvent_SetDestinationOffset(LOCALID_JENNA, 16, 0);
  ObjectEvent_SetDestinationOffset(LOCALID_KRADEN, 0, -32);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_GARET);
  Import_ObjectEventSetAnimation(LOCALID_GARET, 0);
  Import_ObjectEventSetAnimation(LOCALID_JENNA, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_NORTH, 0);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_KRADEN);
  Import_ObjectEventSetAnimation(LOCALID_KRADEN, ANIM_STAND);
  FieldScript_WaitFrames(40);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_KRADEN, 2);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_SOUTHEAST + FACING_STEP, 40);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_SOUTH + FACING_STEP, 40);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_SOUTHEAST + FACING_STEP, 20);
  ObjectMotion_Launch(LOCALID_KRADEN, 4, 20);
  // Kraden: "This is your first time here, right?" Yes: "Take a good look at
  // it." No: "I doubt that..."
  FieldScript_SetMessageId(MsgSolSanctumKradenFirstTimeAtSol);
  Script_AskYesNo(SPEAKER_WINDOW_ABOVE | LOCALID_KRADEN, 0);
  FieldScript_WaitFrames(20);
  FieldCamera_MoveTo(PIXELS(1224), -1, PIXELS(148), 1);
  Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_WALK);
  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (leader != NULL) {
    ObjectEvent_SetDestination(LOCALID_GARET, leader->x.part.pixel,
                               leader->z.part.pixel);
  }
  Import_ObjectEventSetAnimation(LOCALID_JENNA, ANIM_WALK);
  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (leader != NULL) {
    ObjectEvent_SetDestination(LOCALID_JENNA, leader->x.part.pixel,
                               leader->z.part.pixel);
  }
  Import_ObjectEventSetAnimation(LOCALID_KRADEN, ANIM_WALK);
  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (leader != NULL) {
    ObjectEvent_SetDestination(LOCALID_KRADEN, leader->x.part.pixel,
                               leader->z.part.pixel);
  }
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_GARET);
  ObjectEvent_SetPosition(LOCALID_GARET, 0, 0);
  ObjectEvent_SetPosition(LOCALID_JENNA, 0, 0);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_KRADEN);
  ObjectEvent_SetPosition(LOCALID_KRADEN, 0, 0);
  Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_STAND);
  Import_ObjectEventSetAnimation(LOCALID_JENNA, ANIM_STAND);
  Import_ObjectEventSetAnimation(LOCALID_KRADEN, ANIM_STAND);
  GameFlag_Set(FLAG_SOL_SANCTUM_ENTERED);
  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  GameFlag_Clear(FLAG_SHOW_LOCATION_NAME);
  FieldScript_End();
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
  struct ObjectEvent *leader;

  FieldScript_Start();
  FieldScript_StartScreenTransition();
  FieldScript_WaitTransitionFrames();

  // Spread the party out while Kraden examines the room.
  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (leader != NULL)
    Import_ObjectEventSetPosition(LOCALID_KRADEN, leader->x.fixed,
                                  leader->z.fixed);
  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (leader != NULL)
    Import_ObjectEventSetPosition(LOCALID_JENNA, leader->x.fixed,
                                  leader->z.fixed);
  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (leader != NULL)
    Import_ObjectEventSetPosition(LOCALID_GARET, leader->x.fixed,
                                  leader->z.fixed);

  ObjectEvent_SetSpeed(LOCALID_KRADEN, PIXELS(3) / 5, PIXELS(3) / 10);
  ObjectEvent_SetSpeed(LOCALID_JENNA, PIXELS(3) / 5, PIXELS(3) / 10);
  ObjectEvent_SetSpeed(LOCALID_GARET, PIXELS(3) / 5, PIXELS(3) / 10);
  Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_WALK);
  Import_ObjectEventSetAnimation(LOCALID_JENNA, ANIM_WALK);
  Import_ObjectEventSetAnimation(LOCALID_KRADEN, ANIM_WALK);
  ObjectEvent_SetDestinationOffset(LOCALID_GARET, -16, 0);
  ObjectEvent_SetDestinationOffset(LOCALID_JENNA, 16, 0);
  ObjectEvent_SetDestinationOffset(LOCALID_KRADEN, 0, -16);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_KRADEN);
  Import_ObjectEventSetAnimation(LOCALID_KRADEN, ANIM_STAND);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, 0);
  Import_ObjectEventSetAnimation(LOCALID_GARET, 0);
  Import_ObjectEventSetAnimation(LOCALID_JENNA, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTHEAST, 0);
  ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_NORTHWEST, 0);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_NORTH, 30);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_WEST, 0);
  ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_EAST, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 0);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_WEST, 30);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTH, 0);
  ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_WEST, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_EAST, 0);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_SOUTH, 30);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTHEAST, 0);
  ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_NORTHWEST, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_NORTH, 40);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_KRADEN, 2);
  FieldScript_WaitFrames(10);
  Import_ObjectEventSetAnimation(LOCALID_KRADEN, ANIM_WALK);
  ObjectMotion_OffsetPositionAndResetMotion(LOCALID_KRADEN, 0, -16);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_KRADEN);
  Import_ObjectEventSetAnimation(LOCALID_KRADEN, ANIM_STAND);
  FieldScript_WaitFrames(6);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_WEST, 20);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_EAST, 20);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_NORTH, 40);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_KRADEN, 2);
  FieldScript_WaitFrames(20);
  Import_ObjectEventSetAnimation(LOCALID_KRADEN, ANIM_WALK);
  ObjectMotion_OffsetPositionAndResetMotion(LOCALID_KRADEN, 0, -32);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_KRADEN);
  Import_ObjectEventSetAnimation(LOCALID_KRADEN, ANIM_STAND);

  // Survey both sides of the room before returning to Kraden.
  FieldCamera_SetSpeed(PIXELS(2), PIXELS(1) / 4);
  FieldCamera_MoveTo(PIXELS(1585), -1, PIXELS(150), 1);
  FieldCamera_WaitForArrival();
  FieldScript_WaitFrames(10);
  FieldCamera_SetSpeed(PIXELS(6) / 5, PIXELS(3) / 20);
  FieldCamera_MoveTo(PIXELS(1621), -1, PIXELS(100), 1);
  FieldCamera_WaitForArrival();
  FieldCamera_MoveTo(PIXELS(1718), -1, PIXELS(100), 1);
  FieldCamera_WaitForArrival();
  Import_ObjectEventSetAnimation(LOCALID_KRADEN, ANIM_STAND);
  FieldCamera_MoveTo(PIXELS(1752), -1, PIXELS(150), 1);
  FieldCamera_WaitForArrival();
  FieldScript_WaitFrames(40);
  FieldCamera_SetSpeed(PIXELS(12) / 5, PIXELS(3) / 10);
  FieldCamera_MoveTo(PIXELS(1668), -1, PIXELS(256), 1);
  FieldCamera_WaitForArrival();
  ObjectMotion_SetAnimationAndWait(LOCALID_KRADEN, ANIM_NOD);
  FieldScript_WaitFrames(10);

  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTHEAST, 0);
  ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_NORTHWEST, 10);
  ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  // Garet: "What's wrong, Kraden?"
  FieldScript_SetMessageId(MsgSolSanctumWrongKraden);
  Script_ShowMessageAndWait(LOCALID_GARET, 0, 10);
  ObjectEvent_ShowEmote(LOCALID_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  ObjectMotion_SetVariantCallback(LOCALID_KRADEN, 2);
  // Kraden: "Those two said there was a hidden passage leading deeper into
  // the sanctum."
  Script_ShowMessageAndWait(LOCALID_KRADEN, 0, 10);
  ObjectMotion_SetVariantCallback(LOCALID_PARTY_LEADER, 2);
  ObjectMotion_SetVariantCallback(LOCALID_GARET, 2);
  ObjectMotion_SetVariantCallback(LOCALID_JENNA, 2);
  ObjectEvent_SetAttachedEffect(LOCALID_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT |
                                                          ATTACHED_EFFECT_ONCE);
  ObjectEvent_SetAttachedEffect(LOCALID_GARET, ATTACHED_EFFECT_IN_FRONT |
                                                   ATTACHED_EFFECT_ONCE);
  ObjectEvent_SetAttachedEffect(LOCALID_JENNA, ATTACHED_EFFECT_IN_FRONT |
                                                   ATTACHED_EFFECT_ONCE);
  FieldScript_WaitFrames(40);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_KRADEN, 2);
  FieldScript_WaitFrames(20);
  // Kraden: the sanctum always looked strange to him, not as he imagined.
  Script_ShowMessage(LOCALID_KRADEN, 0);
  ObjectMotion_SetAnimationAndWait(LOCALID_KRADEN, ANIM_SHAKE_HEAD);
  Script_ShowMessageAndWait(LOCALID_KRADEN, 0, 10);
  ObjectEvent_LinkPair(LOCALID_PARTY_LEADER, 5, 0);
  FieldScript_WaitFrames(40);
  ObjectMotion_SetVariantCallback(LOCALID_PARTY_LEADER, 1);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_JENNA, 1);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_NORTHWEST, 20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_JENNA, 2);
  // Jenna: "What do you mean?"
  Script_ShowMessageAndWait(LOCALID_JENNA, 0, 10);
  ObjectMotion_SetAnimationAndWait(LOCALID_KRADEN, ANIM_SHAKE_HEAD);
  // Kraden: a sanctum built in honour of the sun ought to show it somewhere.
  Script_ShowMessageAndWait(LOCALID_KRADEN, 0, 10);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_SOUTHEAST + FACING_STEP, 10);
  Script_ShowMessageAndWait(LOCALID_KRADEN, 0, 40);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_NORTH, 20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_KRADEN, 1);
  FieldScript_WaitFrames(10);
  ObjectEvent_SetAttachedEffect(LOCALID_KRADEN, ATTACHED_EFFECT_IN_FRONT |
                                                    ATTACHED_EFFECT_ONCE);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_WEST, 20);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_EAST, 20);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_NORTH, 60);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_KRADEN, 2);
  FieldScript_WaitFrames(10);
  // Kraden: "If there is a hidden passage, then maybe it leads to..."
  Script_ShowMessageAndWait(LOCALID_KRADEN, 0, 10);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_SOUTH, 20);
  ObjectMotion_Launch(LOCALID_KRADEN, 2, 20);

  // Kraden hops: "Let's see if we can find it!"
  Script_ShowMessageAndWait(LOCALID_KRADEN, 0, 40);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_EAST, 20);
  // Garet: "I think he's serious! Should we help him?"
  Script_OpenMessage(LOCALID_GARET, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
    // Yes: Garet says it is too dangerous to split up; they stay together.
    FieldScript_SetMessageId(MsgSolSanctumDangerousSplitStay);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 1);
    FieldScript_ShowMessageThenWait(LOCALID_GARET, 0, 10);
  } else {
    // No: Jenna objects to turning back now; Isaac nods and Garet agrees to
    // help Kraden.
    FieldScript_SetMessageId(MsgSolSanctumCantSeriouslyWant);
    ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_WEST, 20);
    Script_ShowMessageAndWait(LOCALID_JENNA, 0, 10);
    ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
    ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTHEAST, 0);
    ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_NORTHWEST, 60);
    ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE,
                          40);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 1);
    FieldScript_WaitFrames(10);
    ObjectMotion_SetAnimationAndWait(LOCALID_GARET, ANIM_NOD);
    FieldScript_WaitFrames(10);
    ObjectEvent_FaceDirection(LOCALID_GARET, FACING_EAST, 0);
    ObjectEvent_FaceDirection(LOCALID_JENNA, FACING_WEST, 30);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 1);
    FieldScript_WaitFrames(10);
    FieldScript_ShowMessageThenWait(LOCALID_GARET, 0, 10);
  }

  // Bring Kraden back and return the followers to the party.
  ObjectEvent_SetSpeed(LOCALID_KRADEN, PIXELS(3) / 5, PIXELS(3) / 10);
  Import_ObjectEventSetAnimation(LOCALID_KRADEN, ANIM_WALK);
  ObjectMotion_OffsetPositionAndResetMotion(LOCALID_KRADEN, 0, 48);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_KRADEN);
  Import_ObjectEventSetAnimation(LOCALID_KRADEN, ANIM_STAND);
  FieldScript_WaitFrames(6);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTHEAST, 0);
  ObjectMotion_ArmCallback(LOCALID_JENNA, FACING_NORTHWEST, 0);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_KRADEN);
  Import_ObjectEventSetAnimation(LOCALID_KRADEN, ANIM_STAND);
  FieldScript_WaitFrames(20);
  Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_NOD);
  Import_ObjectEventSetAnimation(LOCALID_JENNA, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD);
  FieldScript_WaitFrames(6);
  Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_WALK);

  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (leader != NULL)
    ObjectMotion_ResetAndSetPosition(LOCALID_GARET, leader->x.part.pixel,
                                     leader->z.part.pixel);
  Import_ObjectEventSetAnimation(LOCALID_JENNA, ANIM_WALK);
  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (leader != NULL)
    ObjectMotion_ResetAndSetPosition(LOCALID_JENNA, leader->x.part.pixel,
                                     leader->z.part.pixel);
  Import_ObjectEventSetAnimation(LOCALID_KRADEN, ANIM_WALK);
  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (leader != NULL)
    ObjectMotion_ResetAndSetPosition(LOCALID_KRADEN, leader->x.part.pixel,
                                     leader->z.part.pixel);

  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_KRADEN);
  ObjectEvent_SetPosition(LOCALID_GARET, 0, 0);
  ObjectEvent_SetPosition(LOCALID_JENNA, 0, 0);
  ObjectEvent_SetPosition(LOCALID_KRADEN, 0, 0);
  Import_ObjectEventSetAnimation(LOCALID_KRADEN, ANIM_STAND);
  Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_STAND);
  Import_ObjectEventSetAnimation(LOCALID_JENNA, ANIM_STAND);
  GameFlag_Set(FLAG_SEARCHING_FOR_HIDDEN_PASSAGE);
  GameFlag_Clear(FLAG_SHOW_LOCATION_NAME);
  FieldScript_End();
}

/// Sol Sanctum, the first time the party enters the second relief
/// room: the camera pans along the row of statues, and Kraden: "More
/// statues... It doesn't look like we can reach them from here." He asks
/// Isaac to try his Psynergy. Called by SolSanctumEntrance_RestoreInteriorState
/// there until FLAG_SOL_SANCTUM_MORE_STATUES_SEEN is set, which it sets.
static void SolSanctumEntrance_KradenSeesStatuesOutOfReach(void) {
  struct ObjectEvent *leader;

  FieldScript_Start();
  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  FieldScript_StartScreenTransition();
  FieldScript_WaitTransitionFrames();
  FieldScript_WaitFrames(20);
  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (leader != NULL) {
    ObjectEvent_SetPosition(LOCALID_KRADEN, leader->x.fixed, leader->z.fixed);
  }
  ObjectEvent_SetSpeed(LOCALID_KRADEN, PIXELS(1), PIXELS(1) / 2);
  Import_ObjectEventSetAnimation(LOCALID_KRADEN, ANIM_WALK);
  ObjectEvent_SetDestinationOffset(LOCALID_KRADEN, 24, -10);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_KRADEN);
  Import_ObjectEventSetAnimation(LOCALID_KRADEN, ANIM_STAND);
  FieldScript_WaitFrames(6);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_NORTHWEST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 40);
  FieldCamera_SetSpeed(PIXELS(12) / 5, PIXELS(3) / 10);
  // Pan across the statues, then return to the party's position.
  FieldCamera_MoveTo(PIXELS(1672), -1, PIXELS(524), 1);
  FieldCamera_WaitForArrival();
  FieldScript_WaitFrames(20);
  FieldCamera_SetSpeed(PIXELS(8) / 5, PIXELS(1) / 5);
  FieldCamera_MoveTo(PIXELS(1880), -1, PIXELS(524), 1);
  FieldCamera_WaitForArrival();
  FieldScript_WaitFrames(20);
  FieldCamera_SetSpeed(PIXELS(16) / 5, PIXELS(2) / 5);
  FieldCamera_MoveTo(PIXELS(1769), -1, PIXELS(548), 1);
  FieldCamera_WaitForArrival();
  FieldScript_WaitFrames(20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_KRADEN, 2);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_EAST, 30);
  // Kraden: "More statues... It doesn't look like we can reach them".
  FieldScript_SetMessageId(MsgSolSanctumMoreStatuesOutOfReach);
  Script_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | LOCALID_KRADEN, 0, 10);
  ObjectEvent_ShowEmote(LOCALID_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_KRADEN, 1);
  ObjectEvent_FaceDirection(LOCALID_KRADEN, FACING_SOUTH + FACING_STEP, 20);
  // Kraden: "Perhaps Psynergy is the key..."
  Script_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | LOCALID_KRADEN, 0, 10);
  Import_ObjectEventSetAnimation(LOCALID_KRADEN, ANIM_WALK);
  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (leader != NULL) {
    ObjectEvent_SetDestination(LOCALID_KRADEN, leader->x.part.pixel,
                               leader->z.part.pixel);
  }
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_KRADEN);
  ObjectEvent_SetPosition(LOCALID_KRADEN, 0, 0);
  GameFlag_Set(FLAG_SOL_SANCTUM_MORE_STATUES_SEEN);
  FieldScript_End();
}

/// Sol Sanctum, the second relief room: sets the cells the six statues
/// start on, one metatile behavior while a statue stands there and another once
/// it has been pushed to either side, and, when placeObjectEvents is nonzero
/// (on entry), moves the statues to their saved columns.
static void SolSanctumEntrance_RestoreRowStatueCells(s32 placeObjectEvents) {
  // Clear all six destinations before restoring occupied cells.
  MapGrid_CopyCellAttributes(122, 20, 1, 1, 100, 32);
  MapGrid_CopyCellAttributes(122, 20, 1, 1, 104, 32);
  MapGrid_CopyCellAttributes(122, 20, 1, 1, 108, 32);
  MapGrid_CopyCellAttributes(122, 20, 1, 1, 112, 32);
  MapGrid_CopyCellAttributes(122, 20, 1, 1, 116, 32);
  MapGrid_CopyCellAttributes(122, 20, 1, 1, 120, 32);
  if (GameFlag_Get(FLAG_SOL_SANCTUM_ROW_STATUE_1_AT_WEST) != 0) {
    MapGrid_CopyCellAttributes(121, 20, 1, 1, 100, 32);
    if (placeObjectEvents != 0) {
      ObjectEvent_SetPosition(LOCALID_ROW_STATUE_1, PIXELS(1592), PIXELS(520));
    }
  } else {
    if (GameFlag_Get(FLAG_SOL_SANCTUM_ROW_STATUE_1_AT_EAST) != 0) {
      MapGrid_CopyCellAttributes(121, 20, 1, 1, 100, 32);
      if (placeObjectEvents != 0) {
        ObjectEvent_SetPosition(LOCALID_ROW_STATUE_1, PIXELS(1624),
                                PIXELS(520));
      }
    }
  }
  if (GameFlag_Get(FLAG_SOL_SANCTUM_ROW_STATUE_2_AT_WEST) != 0) {
    MapGrid_CopyCellAttributes(121, 20, 1, 1, 104, 32);
    if (placeObjectEvents != 0) {
      ObjectEvent_SetPosition(LOCALID_ROW_STATUE_2, PIXELS(1656), PIXELS(520));
    }
  } else {
    if (GameFlag_Get(FLAG_SOL_SANCTUM_ROW_STATUE_2_AT_EAST) != 0) {
      MapGrid_CopyCellAttributes(121, 20, 1, 1, 104, 32);
      if (placeObjectEvents != 0) {
        ObjectEvent_SetPosition(LOCALID_ROW_STATUE_2, PIXELS(1688),
                                PIXELS(520));
      }
    }
  }
  if (GameFlag_Get(FLAG_SOL_SANCTUM_ROW_STATUE_3_AT_WEST) != 0) {
    MapGrid_CopyCellAttributes(121, 20, 1, 1, 108, 32);
    if (placeObjectEvents != 0) {
      ObjectEvent_SetPosition(LOCALID_ROW_STATUE_3, PIXELS(1720), PIXELS(520));
    }
  } else {
    if (GameFlag_Get(FLAG_SOL_SANCTUM_ROW_STATUE_3_AT_EAST) != 0) {
      MapGrid_CopyCellAttributes(121, 20, 1, 1, 108, 32);
      if (placeObjectEvents != 0) {
        ObjectEvent_SetPosition(LOCALID_ROW_STATUE_3, PIXELS(1752),
                                PIXELS(520));
      }
    }
  }
  if (GameFlag_Get(FLAG_SOL_SANCTUM_ROW_STATUE_4_AT_WEST) != 0) {
    MapGrid_CopyCellAttributes(121, 20, 1, 1, 112, 32);
    if (placeObjectEvents != 0) {
      ObjectEvent_SetPosition(LOCALID_ROW_STATUE_4, PIXELS(1784), PIXELS(520));
    }
  } else {
    if (GameFlag_Get(FLAG_SOL_SANCTUM_ROW_STATUE_4_AT_EAST) != 0) {
      MapGrid_CopyCellAttributes(121, 20, 1, 1, 112, 32);
      if (placeObjectEvents != 0) {
        ObjectEvent_SetPosition(LOCALID_ROW_STATUE_4, PIXELS(1816),
                                PIXELS(520));
      }
    }
  }
  if (GameFlag_Get(FLAG_SOL_SANCTUM_ROW_STATUE_5_AT_WEST) != 0) {
    MapGrid_CopyCellAttributes(121, 20, 1, 1, 116, 32);
    if (placeObjectEvents != 0) {
      ObjectEvent_SetPosition(LOCALID_ROW_STATUE_5, PIXELS(1848), PIXELS(520));
    }
  } else {
    if (GameFlag_Get(FLAG_SOL_SANCTUM_ROW_STATUE_5_AT_EAST) != 0) {
      MapGrid_CopyCellAttributes(121, 20, 1, 1, 116, 32);
      if (placeObjectEvents != 0) {
        ObjectEvent_SetPosition(LOCALID_ROW_STATUE_5, PIXELS(1880),
                                PIXELS(520));
      }
    }
  }
  if (GameFlag_Get(FLAG_SOL_SANCTUM_ROW_STATUE_6_AT_WEST) != 0) {
    MapGrid_CopyCellAttributes(121, 20, 1, 1, 120, 32);
    if (placeObjectEvents != 0) {
      ObjectEvent_SetPosition(LOCALID_ROW_STATUE_6, PIXELS(1912), PIXELS(520));
    }
  } else {
    if (GameFlag_Get(FLAG_SOL_SANCTUM_ROW_STATUE_6_AT_EAST) != 0) {
      MapGrid_CopyCellAttributes(121, 20, 1, 1, 120, 32);
      if (placeObjectEvents != 0) {
        ObjectEvent_SetPosition(LOCALID_ROW_STATUE_6, PIXELS(1944),
                                PIXELS(520));
      }
    }
  }
}

/// Sol Entrance: searching the other door . "The door is
/// tightly locked."
static void EventScript_DoorTightlyLocked(void) {
  FieldScript_Start();
  FieldMessage_Show(MsgFieldDoorTightlyLocked, 1);
  FieldScript_End();
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
      MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
    }
  } else {
    randomValue = Import_RandomNext();
    if (((u32)(((randomValue << 4) - randomValue) << 3) >> 16) == 0) {
      Script_PlaySong(SE_ITEM_BREAK);
      MapGrid_SetShake(PIXELS(1), PIXELS(2), Q16_ONE);
      *timer = 80;
    }
  }
}

struct MapWarpDest gSolSanctumEntrance_FallbackWarpDests[] = {
    {WARP_DEST_DEFAULT, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {MAP_TABLE_END},
};

struct MapWarpDest gSolEntrance_WarpDests[] = {
    {WARP_DEST_DEFAULT, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {SOL_ENTRANCE_FROM_VALE, CONDITION_ALWAYS, 80, 0, 288, FACING_NORTH, 0, -1,
     -1, -1, -1, 0},
    {SOL_ENTRANCE_FROM_SOL_SANCTUM_RELIEFS, CONDITION_ALWAYS, 120, 0, 112,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {MAP_TABLE_END},
};

struct MapWarpDest gSolSanctumReliefs_WarpDests[] = {
    {WARP_DEST_DEFAULT, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1,
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
    {MAP_TABLE_END},
};

// Sol Entrance (MapId_SolEntrance), then Sol Sanctum
// (MapId_SolSanctumReliefs). Sol Entrance's way
// down leads to MapId_Vale until the eruption, then to Vale during the
// eruption (MapId_ValeEruption) until the party has left Vale.
u32 gSolSanctumEntrance_Warps[] = {
    MAP_WARPS(SolEntrance),
    MAP_WARP_IF(SOL_ENTRANCE_TO_VALE, Vale, VALE_FROM_SOL_ENTRANCE,
                FLAG_SOL_SANCTUM_ERUPTED),
    MAP_WARP_IF(SOL_ENTRANCE_TO_VALE, ValeEruption,
                VALE_ERUPTION_FROM_SOL_ENTRANCE, FLAG_VALE_PARTY_LEFT),
    MAP_WARP_IF(SOL_ENTRANCE_TO_VALE, Vale, VALE_FROM_SOL_ENTRANCE,
                WARP_FLAG_NONE),
    MAP_WARP_IF(SOL_ENTRANCE_TO_SOL_SANCTUM_RELIEFS, SolSanctumReliefs,
                SOL_SANCTUM_RELIEFS_FROM_SOL_ENTRANCE, WARP_FLAG_NONE),
    MAP_WARPS(SolSanctumReliefs),
    MAP_WARP_IF(
        SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_CORRIDOR_WEST, SolSanctumCorridor,
        SOL_SANCTUM_CORRIDOR_FROM_SOL_SANCTUM_RELIEFS_WEST, WARP_FLAG_NONE),
    MAP_WARP_IF(
        SOL_SANCTUM_RELIEFS_CHEST_ROOM_TO_FIRST_RELIEF_ROOM, SolSanctumReliefs,
        SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_FROM_CHEST_ROOM, WARP_FLAG_NONE),
    MAP_WARP_IF(
        SOL_SANCTUM_RELIEFS_STATUE_NOOK_TO_FIRST_RELIEF_ROOM, SolSanctumReliefs,
        SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_FROM_STATUE_NOOK, WARP_FLAG_NONE),
    MAP_WARP_IF(SOL_SANCTUM_RELIEFS_STEPPING_STONE_ROOM_TO_ENTRANCE_ROOM,
                SolSanctumReliefs,
                SOL_SANCTUM_RELIEFS_ENTRANCE_ROOM_FROM_STEPPING_STONE_ROOM,
                WARP_FLAG_NONE),
    MAP_WARP_IF(
        SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_CORRIDOR_EAST, SolSanctumCorridor,
        SOL_SANCTUM_CORRIDOR_FROM_SOL_SANCTUM_RELIEFS_EAST, WARP_FLAG_NONE),
    MAP_WARP_IF(
        SOL_SANCTUM_RELIEFS_STEPS_ROOM_TO_FIRST_RELIEF_ROOM, SolSanctumReliefs,
        SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_FROM_STEPS_ROOM, WARP_FLAG_NONE),
    MAP_WARP_IF(SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_MAZE_WEST, SolSanctumMaze,
                SOL_SANCTUM_MAZE_FROM_SOL_SANCTUM_RELIEFS_SOUTH,
                WARP_FLAG_NONE),
    MAP_WARP_IF(SOL_SANCTUM_RELIEFS_TO_SOL_ENTRANCE, SolEntrance,
                SOL_ENTRANCE_FROM_SOL_SANCTUM_RELIEFS, WARP_FLAG_NONE),
    MAP_WARP_IF(SOL_SANCTUM_RELIEFS_ENTRANCE_ROOM_TO_STEPPING_STONE_ROOM,
                SolSanctumReliefs,
                SOL_SANCTUM_RELIEFS_STEPPING_STONE_ROOM_FROM_ENTRANCE_ROOM,
                WARP_FLAG_NONE),
    MAP_WARP_IF(SOL_SANCTUM_RELIEFS_ROUND_PASSAGE_TO_SECOND_RELIEF_ROOM,
                SolSanctumReliefs,
                SOL_SANCTUM_RELIEFS_SECOND_RELIEF_ROOM_FROM_ROUND_PASSAGE,
                WARP_FLAG_NONE),
    MAP_WARP_IF(
        SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_TO_CHEST_ROOM, SolSanctumReliefs,
        SOL_SANCTUM_RELIEFS_CHEST_ROOM_FROM_FIRST_RELIEF_ROOM, WARP_FLAG_NONE),
    MAP_WARP_IF(
        SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_TO_STATUE_NOOK, SolSanctumReliefs,
        SOL_SANCTUM_RELIEFS_STATUE_NOOK_FROM_FIRST_RELIEF_ROOM, WARP_FLAG_NONE),
    MAP_WARP_IF(
        SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_TO_STEPS_ROOM, SolSanctumReliefs,
        SOL_SANCTUM_RELIEFS_STEPS_ROOM_FROM_FIRST_RELIEF_ROOM, WARP_FLAG_NONE),
    MAP_WARP_IF(SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_MAZE_EAST, SolSanctumMaze,
                SOL_SANCTUM_MAZE_FROM_SOL_SANCTUM_RELIEFS_NORTH,
                WARP_FLAG_NONE),
    MAP_WARP_IF(SOL_SANCTUM_RELIEFS_SECOND_RELIEF_ROOM_TO_ROUND_PASSAGE,
                SolSanctumReliefs,
                SOL_SANCTUM_RELIEFS_ROUND_PASSAGE_FROM_SECOND_RELIEF_ROOM,
                WARP_FLAG_NONE),
    MAP_WARP_IF(SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_LUNA_SOL_ROOMS,
                SolSanctumLunaSolRooms,
                SOL_SANCTUM_LUNA_SOL_ROOMS_FROM_SOL_SANCTUM_RELIEFS,
                WARP_FLAG_NONE),
    MAP_WARPS_END,
};

struct ObjectEventTemplate gSolSanctumEntrance_FallbackObjectEventTemplates[] =
    {
        {MAP_TABLE_END},
};

// clang-format off
struct ObjectEventTemplate gSolSanctumReliefs_ObjectEventTemplates[] = {
    { LOCALID_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0 },
    { LOCALID_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0 },
    { SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0 },
    { SPRITE_STONE_STATUE, 0x0, MOVEMENT_TYPE_STAND,
        PIXELS(744), 0, PIXELS(56), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gSolSanctumReliefs_FirstReliefRoomObjectEventTemplates[] = {
    { LOCALID_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0 },
    { LOCALID_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0 },
    { SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0 },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1512), 0, PIXELS(136), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1832), 0, PIXELS(136), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1560), 0, PIXELS(136), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1784), 0, PIXELS(136), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1640), 0, PIXELS(264), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1640), 0, PIXELS(200), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1640), 0, PIXELS(136), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1672), 0, PIXELS(88), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1704), 0, PIXELS(136), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1704), 0, PIXELS(200), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1704), 0, PIXELS(264), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gSolSanctumReliefs_SecondReliefRoomObjectEventTemplates[] = {
    { LOCALID_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0 },
    { LOCALID_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0 },
    { SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0 },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1608), 0, PIXELS(520), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1672), 0, PIXELS(520), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1736), 0, PIXELS(520), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1800), 0, PIXELS(520), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1864), 0, PIXELS(520), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1928), 0, PIXELS(520), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gSolEntrance_ObjectEventTemplates[] = {
    { SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { MAP_TABLE_END },
};

struct MapEvent gSolSanctumEntrance_FallbackEvents[] = {
    { MAP_EVENTS_END },
};

struct MapEvent gSolEntrance_Events[] = {
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, SOL_ENTRANCE_TO_VALE },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), MB_SOL_ENTRANCE_DOOR, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_OpenDoorAndEnter) },
    { BG_EVENT(BG_EVENT_SILENT), MB_SOL_ENTRANCE_LOCKED_DOOR, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_DoorTightlyLocked) },
    { MAP_EVENT_TALK, LOCALID_WARNING_MAN, CONDITION_ALWAYS, MSGNO(MsgValeItIsVeryDangerousToGo) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WARNING_MAN, CONDITION_ALWAYS, MSGNO(MsgValeIAmAllAloneInThis) },
    { MAP_EVENTS_END },
};

struct MapEvent gSolSanctumReliefs_Events[] = {
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_CORRIDOR_WEST },
    { MAP_EVENT_WARP, 2, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_CHEST_ROOM_TO_FIRST_RELIEF_ROOM },
    { MAP_EVENT_WARP, 3, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_STATUE_NOOK_TO_FIRST_RELIEF_ROOM },
    { MAP_EVENT_WARP, 4, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_STEPPING_STONE_ROOM_TO_ENTRANCE_ROOM },
    { MAP_EVENT_WARP, 5, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_CORRIDOR_EAST },
    { MAP_EVENT_WARP, 6, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_STEPS_ROOM_TO_FIRST_RELIEF_ROOM },
    { MAP_EVENT_WARP | WARP_STYLE_STAIRS_UP, 7, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_MAZE_WEST },
    { MAP_EVENT_WARP, 8, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_ENTRANCE },
    { MAP_EVENT_WARP, 9, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_ENTRANCE_ROOM_TO_STEPPING_STONE_ROOM },
    { MAP_EVENT_WARP, 10, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_ROUND_PASSAGE_TO_SECOND_RELIEF_ROOM },
    { MAP_EVENT_WARP, 11, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_TO_CHEST_ROOM },
    { MAP_EVENT_WARP, 12, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_TO_STATUE_NOOK },
    { MAP_EVENT_WARP, 13, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_TO_STEPS_ROOM },
    { MAP_EVENT_WARP, 14, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_MAZE_EAST },
    { MAP_EVENT_WARP, 15, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_SECOND_RELIEF_ROOM_TO_ROUND_PASSAGE },
    { MAP_EVENT_WARP, 16, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_LUNA_SOL_ROOMS },
    { BG_EVENT(BG_EVENT_CHEST), MB_SOL_SANCTUM_RELIEFS_CHEST, FLAG_SOL_SANCTUM_FIRST_SMALL_JEWEL_FOUND, FIND_ITEM(ITEM_SMALL_JEWEL) },
    { USE_ITEM(ITEM_SMALL_JEWEL), SOL_SANCTUM_ITEM_OWN_MESSAGE | MB_SOL_SANCTUM_RELIEFS_FIRST_RELIEF, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SetFirstSmallJewel) },
    { BG_EVENT(BG_EVENT_SILENT), MB_SOL_SANCTUM_RELIEFS_FIRST_RELIEF, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_InspectFirstMinotaurRelief) },
    { MAP_EVENTS_END },
};

struct MapEvent gSolSanctumReliefs_FirstReliefRoomEvents[] = {
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_CORRIDOR_WEST },
    { MAP_EVENT_WARP, 2, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_CHEST_ROOM_TO_FIRST_RELIEF_ROOM },
    { MAP_EVENT_WARP, 3, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_STATUE_NOOK_TO_FIRST_RELIEF_ROOM },
    { MAP_EVENT_WARP, 4, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_STEPPING_STONE_ROOM_TO_ENTRANCE_ROOM },
    { MAP_EVENT_WARP, 5, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_CORRIDOR_EAST },
    { MAP_EVENT_WARP, 6, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_STEPS_ROOM_TO_FIRST_RELIEF_ROOM },
    { MAP_EVENT_WARP | WARP_STYLE_STAIRS_UP, 7, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_MAZE_WEST },
    { MAP_EVENT_WARP, 8, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_ENTRANCE },
    { MAP_EVENT_WARP, 9, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_ENTRANCE_ROOM_TO_STEPPING_STONE_ROOM },
    { MAP_EVENT_WARP, 10, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_ROUND_PASSAGE_TO_SECOND_RELIEF_ROOM },
    { MAP_EVENT_WARP, 11, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_TO_CHEST_ROOM },
    { MAP_EVENT_WARP, 12, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_TO_STATUE_NOOK },
    { MAP_EVENT_WARP, 13, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_TO_STEPS_ROOM },
    { MAP_EVENT_WARP, 14, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_MAZE_EAST },
    { MAP_EVENT_WARP, 15, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_SECOND_RELIEF_ROOM_TO_ROUND_PASSAGE },
    { MAP_EVENT_WARP, 16, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_LUNA_SOL_ROOMS },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_WEST_STATUE, FLAG_SOL_SANCTUM_IN_SECOND_RELIEF_ROOM, EVENT_SCRIPT(EventScript_WestStatueMoved) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_EAST_STATUE, FLAG_SOL_SANCTUM_IN_SECOND_RELIEF_ROOM, EVENT_SCRIPT(EventScript_EastStatueMoved) },
    { COORD_EVENT_WALKING, MB_SOL_SANCTUM_RELIEFS_FIRST_ROOM_STATUE, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_PushStatue) },
    { COORD_EVENT_WALKING_INTO(FACING_EAST), MB_SOL_SANCTUM_RELIEFS_FIRST_ROOM_STATUE_EAST, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_PushStatue) },
    { COORD_EVENT_WALKING_INTO(FACING_WEST), MB_SOL_SANCTUM_RELIEFS_FIRST_ROOM_STATUE_WEST, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_PushStatue) },
    { MAP_EVENTS_END },
};

struct MapEvent gSolSanctumReliefs_SecondReliefRoomEvents[] = {
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_CORRIDOR_WEST },
    { MAP_EVENT_WARP, 2, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_CHEST_ROOM_TO_FIRST_RELIEF_ROOM },
    { MAP_EVENT_WARP, 3, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_STATUE_NOOK_TO_FIRST_RELIEF_ROOM },
    { MAP_EVENT_WARP, 4, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_STEPPING_STONE_ROOM_TO_ENTRANCE_ROOM },
    { MAP_EVENT_WARP, 5, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_CORRIDOR_EAST },
    { MAP_EVENT_WARP, 6, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_STEPS_ROOM_TO_FIRST_RELIEF_ROOM },
    { MAP_EVENT_WARP | WARP_STYLE_STAIRS_UP, 7, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_MAZE_WEST },
    { MAP_EVENT_WARP, 8, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_ENTRANCE },
    { MAP_EVENT_WARP, 9, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_ENTRANCE_ROOM_TO_STEPPING_STONE_ROOM },
    { MAP_EVENT_WARP, 10, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_ROUND_PASSAGE_TO_SECOND_RELIEF_ROOM },
    { MAP_EVENT_WARP, 11, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_TO_CHEST_ROOM },
    { MAP_EVENT_WARP, 12, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_TO_STATUE_NOOK },
    { MAP_EVENT_WARP, 13, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_FIRST_RELIEF_ROOM_TO_STEPS_ROOM },
    { MAP_EVENT_WARP, 14, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_MAZE_EAST },
    { MAP_EVENT_WARP, 15, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_SECOND_RELIEF_ROOM_TO_ROUND_PASSAGE },
    { MAP_EVENT_WARP, 16, CONDITION_ALWAYS, SOL_SANCTUM_RELIEFS_TO_SOL_SANCTUM_LUNA_SOL_ROOMS },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_ROW_STATUE_1, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_RowStatue1Moved) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_ROW_STATUE_2, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_RowStatue2Moved) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_ROW_STATUE_3, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_RowStatue3Moved) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_ROW_STATUE_4, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_RowStatue4Moved) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_ROW_STATUE_5, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_RowStatue5Moved) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_ROW_STATUE_6, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_RowStatue6Moved) },
    { COORD_EVENT_WALKING, MB_SOL_SANCTUM_RELIEFS_ROW_STATUE, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_PushStatue) },
    { USE_ITEM(ITEM_SMALL_JEWEL), SOL_SANCTUM_ITEM_OWN_MESSAGE | MB_SOL_SANCTUM_RELIEFS_SECOND_RELIEF, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SetSecondSmallJewel) },
    { BG_EVENT(BG_EVENT_SILENT), MB_SOL_SANCTUM_RELIEFS_SECOND_RELIEF, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_InspectSecondMinotaurRelief) },
    { MAP_EVENTS_END },
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
