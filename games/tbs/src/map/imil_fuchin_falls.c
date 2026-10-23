/// @file
/// Imil Falls Cave, north of Imil, and Fuchin Falls Cave, the waterfall
/// grotto of Fuchin Temple's test: two map sections in seven maps.
///
/// - Imil Falls Cave (MapId_ImilFallsCave), up the way north out of
///   Imil; its only warp, IMIL_FALLS_CAVE_TO_IMIL, leads back down
///   (IMIL_FROM_IMIL_FALLS_CAVE). Fever, a Mars Djinni, waits on its ice and
///   joins the party when spoken to, without a battle. Every normal entry
///   blocks Retreat and walks the leader up toward Fever, and stepping onto
///   the ice (metatile behavior 10) slides the leader in the held direction,
///   turning toward it as he goes, until a blocked cell or one too high stops
///   him.
/// - Fuchin Falls Cave (MapId_FuchinFallsCaveEntrance to
///   MapId_FuchinFallsCaveDragonStatue), the waterfall grotto. Master Nyunpa
///   of Fuchin Temple tells the party they must survive its test before he will
///   tell them how to pass through Mogall Forest. The test ends at the chest of
///   the Orb of Force (FLAG_FUCHIN_FALLS_ORB_OF_FORCE_FOUND), and back at the
///   temple Nyunpa names the power they have gained Ki (fuchin_temple.c). The
///   way in and out is FUCHIN_FALLS_CAVE_ENTRANCE_TO_FUCHIN_TEMPLE_GROUNDS, to
///   FUCHIN_TEMPLE_GROUNDS_FROM_FUCHIN_FALLS_CAVE_ENTRANCE. The code numbers
///   the rooms by map: the first to fourth rooms
///   (MapId_FuchinFallsCaveEntrance to MapId_FuchinFallsCaveArcticBlade),
///   the room of the Ki inscription (MapId_FuchinFallsCaveOrbOfForce) and the
///   dragon statue's room (MapId_FuchinFallsCaveDragonStatue).
///
///   The first four rooms are crossed on floating logs
///   (SPRITE_FLOATING_LOG_WEST_EAST and SPRITE_FLOATING_LOG_NORTH_SOUTH, the
///   logs of Colosso's Lumber Water Crossing Area, colosseum_azart_match.c).
///   Walking into a log's coord event cell pushes it along the water to its
///   next stop, the leader moving with it (ObjectEvent_PlacePairAtOffset);
///   touching the cell plays the log's two rolling animations. Each stop is a
///   map-section temp flag, FLAG_MAPSEC_TEMP_01 to FLAG_MAPSEC_TEMP_03 for the
///   first room's logs, FLAG_MAPSEC_TEMP_04 to FLAG_MAPSEC_TEMP_07 the
///   second's, FLAG_MAPSEC_TEMP_08 to FLAG_MAPSEC_TEMP_12 the third's and
///   FLAG_MAPSEC_TEMP_13 to FLAG_MAPSEC_TEMP_15 the fourth's, so the logs are
///   back where they started once the party leaves the cave's maps. After
///   each move, and on entry, the room's rebuild function puts every log at the
///   cell its flags name and rewrites the cells of each log's track to match:
///   in the first room (FuchinFallsCave_RebuildEntranceLogs) the track's cells
///   are first copied in from a copy below the room that has each log's coord
///   event cells (11 and 12, 13 and 14, 15 and 16) at both ends of its run,
///   then the end each log's flag says it is not at is overwritten with a wall
///   cell of the water
///   (https://alchemy.pascalpixel.com/places/fuchin-falls-cave-entrance/log-rebuild.png:
///   the track's cells tinted by what each does, a wall, a way out or the
///   push that moves a log, entered with none, then one, two and three of
///   FLAG_MAPSEC_TEMP_01 to _03 set, the cells changed from the first
///   outlined). In the third room the
///   logs block one another, so where a log stops depends on where the others
///   lie; in the second and third rooms the leader hops across gaps a log's
///   stop leaves.
///
///   Zephyr, a Jupiter Djinni, waits in the second room
///   (MapId_FuchinFallsCaveZephyr) and fights when spoken to. In the first
///   room a chest holds a mimic, whose battle its find value starts
///   (FIND_MIMIC); the fourth room's chest holds an Arctic Blade, and the Ki
///   room's the Orb of Force. Arriving in the third room by
///   FUCHIN_FALLS_CAVE_DRAGONS_EYE_CHAMBER_FROM_LOGS before the Dragon's Eye
///   is taken (FLAG_FUCHIN_FALLS_DRAGONS_EYE_FOUND) shows it, and searching
///   there takes it.
///
///   The dragon statue's room is dark until the eye is set: entering its
///   northern half (its first six warp destinations, up to
///   FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_FROM_SOUTH_HALF_EAST: from the
///   Ki room, from the fourth room or up from the southern half) darkens the
///   palette. Coming in from the Ki room
///   (FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_ORB_OF_FORCE_EAST
///   and _WEST) or up by
///   FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_FROM_SOUTH_HALF_MIDDLE starts
///   the fade overlay (overworld/overworld/fade_overlay.c), which follows the
///   leader except from
///   FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_FROM_SOUTH_HALF_MIDDLE, and
///   coord events 15 and 16 start and stop its following. When Reveal ends, the
///   room goes dark again. The statue is `an eyeless dragon`; using the
///   Dragon's Eye on it flashes the room, blows dust from the cave mouth as the
///   room brightens, redraws the cells of the hidden path and uses the eye up
///   (FLAG_FUCHIN_FALLS_DRAGONS_EYE_SET). The first room's inscription points
///   the way: `The dragon's flame illuminates the path of truth in darkness.`
///
/// Open questions: none.

#include "action_script.h"
#include "save_block.h"
#include "keystate.h"
#include "map_ids.h"
#include "unit/import.h"
#include "unit/map/imil_fuchin_falls.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/field_services.h"
#include "unit/field/field.h"
#include "unit/game/game.h"
#include "battle.h"
#include "field_service.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "imil_fuchin_falls.h"
#include "types.h"
#include "glyph.h"
#include "script_layer.h"
#include "map_tables.h"
#include "djinn_ids.h"
#include "character_ids.h"
#include "call.h"
#include "battle_party.h"
#include "field_effect.h"
#include "map_header.h"
#include "display_transition.h"
#include "fx_work.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "objcmd.h"
#include "fixed_math.h"
#include "object_effect.h"
#include "palbuf.h"
#include "io_reg.h"
#include "flags.h"
#include "songs.h"
#include "audio_engine_symbols.h"
#include "item_ids.h"
#include "map_grid.h"

/// The placed object events, numbering each map's template rows from
/// LOCALID_FIRST_PLACED: Fever in Imil Falls Cave, the floating logs of Fuchin
/// Falls Cave's first four rooms, followed in the second room by Zephyr, and
/// the one sprite the dragon statue's room places.
enum ImilFuchinFallsLocalId {
  /// A Mars Djinni, in Imil Falls Cave.
  /// ★ Named from the game: "Fever" (MsgDjinnName)
  LOCALID_FEVER = LOCALID_FIRST_PLACED,
  /// The floating logs: three in the first, second and fourth rooms, four in
  /// the third.
  /// ★ Named from what the player sees: yellow logs floating on the cave's
  /// water (its render)
  LOCALID_FIRST_LOG = LOCALID_FIRST_PLACED,
  /// ★ Named from what the player sees: a floating log (its render)
  LOCALID_SECOND_LOG,
  /// ★ Named from what the player sees: a floating log (its render)
  LOCALID_THIRD_LOG,
  /// ★ Named from what the player sees: a floating log (its render)
  LOCALID_FOURTH_LOG,
  /// Zephyr, a Jupiter Djinni, after the second room's three logs.
  /// ★ Named from the game: "Zephyr" (MsgDjinnName)
  LOCALID_ZEPHYR = LOCALID_FIRST_PLACED + 3,
  /// A crate (SPRITE_CRATE) in the dragon statue's room, the sprite of the
  /// third room's two foreground pieces
  /// (FuchinFallsCave_CreateForegroundObjectEvent);
  /// on
  /// entry ImilFuchinFalls_PrepareRoom stills it at floor height and draws it
  /// at sprite priority 1 in SPRITE_COLOR_HIDDEN, as those are drawn, so
  /// every pixel is transparent and the player never sees it.
  LOCALID_BLANK_DRAGON_ROOM_CRATE = LOCALID_FIRST_PLACED
};

enum {
  IMIL_SLIDE_ANIMATION_STEP = 3 * 16 ///< three times normal animation speed
};

static void FuchinFallsCave_RebuildEntranceLogs(void);
static void FuchinFallsCave_RebuildZephyrRoomLogs(void);
static void FuchinFallsCave_RebuildDragonsEyeRoomLogs(void);
static void FuchinFallsCave_RebuildArcticBladeRoomLogs(void);
static void Imil_WalkTowardFever(void);
static void ImilFuchinFalls_PrepareRoom(void);
static void ImilFuchinFalls_RebuildLogRoom(void);
static void ImilFuchinFalls_HopBy(s32 dx, s32 dz);
static void ImilFuchinFalls_PlaceDragonsEye(void);
static void Imil_SlideObjectEventAcrossIce(void);

s32 gImilFuchinFalls_TrackLeader;

/// EffectOptions cut after its scales: the caller's frame holds only these 24
/// bytes.
struct CaveDustOptions {
  s32 priority;
  s32 palette;
  s32 startScaleX;
  s32 startScaleY;
  s32 targetScaleX;
  s32 targetScaleY;
};

STATIC_ASSERT(offsetof(struct CaveDustOptions, priority) ==
              offsetof(struct EffectOptions, priority));
STATIC_ASSERT(offsetof(struct CaveDustOptions, palette) ==
              offsetof(struct EffectOptions, palette));
STATIC_ASSERT(offsetof(struct CaveDustOptions, startScaleX) ==
              offsetof(struct EffectOptions, startScaleX));
STATIC_ASSERT(offsetof(struct CaveDustOptions, startScaleY) ==
              offsetof(struct EffectOptions, startScaleY));
STATIC_ASSERT(offsetof(struct CaveDustOptions, targetScaleX) ==
              offsetof(struct EffectOptions, targetScaleX));
STATIC_ASSERT(offsetof(struct CaveDustOptions, targetScaleY) ==
              offsetof(struct EffectOptions, targetScaleY));

/// Talking to Fever, a Mars Djinni, in Imil Falls Cave adds it to the party.
static void Imil_RecruitFever(void) {
  Djinn_JoinParty(LOCALID_FEVER, ELEMENT_MARS, DJINNI_INDEX(DJINNI_FEVER));
}

/// Talking to Zephyr, a Jupiter Djinni, in Fuchin Falls Cave's second room
/// (MapId_FuchinFallsCaveZephyr) starts its battle.
static void FuchinFallsCave_FightZephyr(void) {
  Field_RequestDjinnBattle(LOCALID_ZEPHYR, DJINNI_ZEPHYR);
}

const struct MapWarpDest *ImilFuchinFalls_GetWarpDests(void) {
  s16 mapId = gSaveBlock.mapId;

  if (mapId == (s32)&MapId_ImilFallsCave) {
    return gImilFuchinFalls_ImilFallsCaveWarpDests;
  }
  if (mapId == (s32)&MapId_FuchinFallsCaveEntrance) {
    return gImilFuchinFalls_EntranceWarpDests;
  }
  if (mapId == (s32)&MapId_FuchinFallsCaveZephyr) {
    return gImilFuchinFalls_ZephyrRoomWarpDests;
  }
  if (mapId == (s32)&MapId_FuchinFallsCaveDragonsEye) {
    return gImilFuchinFalls_DragonsEyeRoomWarpDests;
  }
  if (mapId == (s32)&MapId_FuchinFallsCaveArcticBlade) {
    return gImilFuchinFalls_ArcticBladeRoomWarpDests;
  }
  if (mapId == (s32)&MapId_FuchinFallsCaveOrbOfForce) {
    return gImilFuchinFalls_KiRoomWarpDests;
  }
  if (mapId == (s32)&MapId_FuchinFallsCaveDragonStatue) {
    return gImilFuchinFalls_DragonRoomWarpDests;
  }
  return gImilFuchinFalls_NoWarpDests;
}

const struct MapWarpRegion *ImilFuchinFalls_GetRegions(void) { return NULL; }

/// Returns the warp table every map of the overlay shares.
const u32 *ImilFuchinFalls_GetWarps(void) { return gImilFuchinFalls_Warps; }

const struct ObjectEventTemplate *
ImilFuchinFalls_GetObjectEventTemplates(void) {
  s16 mapId = gSaveBlock.mapId;

  if (mapId == (s32)&MapId_ImilFallsCave) {
    return gImilFuchinFalls_ImilFallsCaveObjectEventTemplates;
  }
  if (mapId == (s32)&MapId_FuchinFallsCaveEntrance) {
    return gImilFuchinFalls_EntranceObjectEventTemplates;
  }
  if (mapId == (s32)&MapId_FuchinFallsCaveZephyr) {
    return gImilFuchinFalls_ZephyrRoomObjectEventTemplates;
  }
  if (mapId == (s32)&MapId_FuchinFallsCaveDragonsEye) {
    return gImilFuchinFalls_DragonsEyeRoomObjectEventTemplates;
  }
  if (mapId == (s32)&MapId_FuchinFallsCaveArcticBlade) {
    return gImilFuchinFalls_ArcticBladeRoomObjectEventTemplates;
  }
  if (mapId == (s32)&MapId_FuchinFallsCaveDragonStatue) {
    return gImilFuchinFalls_DragonRoomObjectEventTemplates;
  }
  return gImilFuchinFalls_NoObjectEventTemplates;
}

#include "common/object_pairmove.h"

/// Lets the log settle before rebuilding its room's metatile behaviors.
static inline void FuchinFallsCave_FinishLogMove(void (*rebuildLogs)(void)) {
  Sound_PlaySong(SONG_FADE_OUT_ACTION_SE);
  WaitFrames(2);
  rebuildLogs();
}

/// Fuchin Falls Cave, first room: pushing the
/// first log east (coord event 11) floats it 224 pixels east
/// (FLAG_MAPSEC_TEMP_01 set).
static void FuchinFallsCave_Entrance_MoveFirstLogEast(void) {
  Sound_PlaySong(SE_WATER_MOVE);
  ObjectEvent_PlacePairAtOffset(LOCALID_FIRST_LOG, 112, 0);
  ObjectEvent_PlacePairAtOffset(LOCALID_FIRST_LOG, 112, 0);
  Sound_PlaySong(SONG_FADE_OUT_ACTION_SE);
  GameFlag_Set(FLAG_MAPSEC_TEMP_01);
  WaitFrames(2);
  FuchinFallsCave_RebuildEntranceLogs();
}

/// Fuchin Falls Cave, first room: pushing the
/// first log west (coord event 12) floats it 224 pixels back
/// (FLAG_MAPSEC_TEMP_01 clear).
static void FuchinFallsCave_Entrance_MoveFirstLogWest(void) {
  s32 offset = 112;

  offset = -offset;
  Sound_PlaySong(SE_WATER_MOVE);
  ObjectEvent_PlacePairAtOffset(LOCALID_FIRST_LOG, offset, 0);
  ObjectEvent_PlacePairAtOffset(LOCALID_FIRST_LOG, offset, 0);
  Sound_PlaySong(SONG_FADE_OUT_ACTION_SE);
  GameFlag_Clear(FLAG_MAPSEC_TEMP_01);
  WaitFrames(2);
  FuchinFallsCave_RebuildEntranceLogs();
}

/// Fuchin Falls Cave, first room: pushing the
/// second log south (coord event 13) floats it 64 pixels south
/// (FLAG_MAPSEC_TEMP_02 clear).
static void FuchinFallsCave_Entrance_MoveSecondLogSouth(void) {
  Sound_PlaySong(SE_WATER_MOVE);
  ObjectEvent_PlacePairAtOffset(LOCALID_SECOND_LOG, 0, 64);
  Sound_PlaySong(SONG_FADE_OUT_ACTION_SE);
  GameFlag_Clear(FLAG_MAPSEC_TEMP_02);
  WaitFrames(2);
  FuchinFallsCave_RebuildEntranceLogs();
}

/// Fuchin Falls Cave, first room: pushing the
/// second log north (coord event 14) floats it 64 pixels north
/// (FLAG_MAPSEC_TEMP_02 set).
static void FuchinFallsCave_Entrance_MoveSecondLogNorth(void) {
  Sound_PlaySong(SE_WATER_MOVE);
  ObjectEvent_PlacePairAtOffset(LOCALID_SECOND_LOG, 0, -64);
  Sound_PlaySong(SONG_FADE_OUT_ACTION_SE);
  GameFlag_Set(FLAG_MAPSEC_TEMP_02);
  WaitFrames(2);
  FuchinFallsCave_RebuildEntranceLogs();
}

/// Fuchin Falls Cave, first room: pushing the
/// third log south (coord event 15) floats it 64 pixels south
/// (FLAG_MAPSEC_TEMP_03 clear).
static void FuchinFallsCave_Entrance_MoveThirdLogSouth(void) {
  Sound_PlaySong(SE_WATER_MOVE);
  ObjectEvent_PlacePairAtOffset(LOCALID_THIRD_LOG, 0, 64);
  Sound_PlaySong(SONG_FADE_OUT_ACTION_SE);
  GameFlag_Clear(FLAG_MAPSEC_TEMP_03);
  WaitFrames(2);
  FuchinFallsCave_RebuildEntranceLogs();
}

/// Fuchin Falls Cave, first room: pushing the
/// third log north (coord event 16) floats it 64 pixels north
/// (FLAG_MAPSEC_TEMP_03 set).
static void FuchinFallsCave_Entrance_MoveThirdLogNorth(void) {
  Sound_PlaySong(SE_WATER_MOVE);
  ObjectEvent_PlacePairAtOffset(LOCALID_THIRD_LOG, 0, -64);
  Sound_PlaySong(SONG_FADE_OUT_ACTION_SE);
  GameFlag_Set(FLAG_MAPSEC_TEMP_03);
  WaitFrames(2);
  FuchinFallsCave_RebuildEntranceLogs();
}

/// Fuchin Falls Cave, second room: pushing the
/// first log east (coord event 11), while FLAG_MAPSEC_TEMP_06 is clear, floats
/// it 144 pixels east (FLAG_MAPSEC_TEMP_04 set).
static void FuchinFallsCave_ZephyrRoom_MoveFirstLogToEastEnd(void) {
  Sound_PlaySong(SE_WATER_MOVE);
  ObjectEvent_PlacePairAtOffset(LOCALID_FIRST_LOG, 144, 0);
  Sound_PlaySong(SONG_FADE_OUT_ACTION_SE);
  GameFlag_Set(FLAG_MAPSEC_TEMP_04);
  WaitFrames(2);
  FuchinFallsCave_RebuildZephyrRoomLogs();
}

/// Fuchin Falls Cave, second room: pushing the
/// first log west (coord event 12) floats it 144 pixels west
/// (FLAG_MAPSEC_TEMP_04 clear, FLAG_MAPSEC_TEMP_05 set).
static void FuchinFallsCave_ZephyrRoom_MoveFirstLogToWestEnd(void) {
  Sound_PlaySong(SE_WATER_MOVE);
  ObjectEvent_PlacePairAtOffset(LOCALID_FIRST_LOG, -144, 0);
  Sound_PlaySong(SONG_FADE_OUT_ACTION_SE);
  GameFlag_Clear(FLAG_MAPSEC_TEMP_04);
  GameFlag_Set(FLAG_MAPSEC_TEMP_05);
  WaitFrames(2);
  FuchinFallsCave_RebuildZephyrRoomLogs();
}

/// Fuchin Falls Cave, second room: pushing the
/// first log west (coord event 11) floats it 14 pixels west
/// (FLAG_MAPSEC_TEMP_05 set).
static void FuchinFallsCave_ZephyrRoom_NudgeFirstLogWest(void) {
  Sound_PlaySong(SE_WATER_MOVE);
  ObjectEvent_PlacePairAtOffset(LOCALID_FIRST_LOG, -14, 0);
  Sound_PlaySong(SONG_FADE_OUT_ACTION_SE);
  GameFlag_Set(FLAG_MAPSEC_TEMP_05);
  WaitFrames(2);
  FuchinFallsCave_RebuildZephyrRoomLogs();
}

/// Fuchin Falls Cave, second room: pushing the
/// first log east (coord event 17) moves it one cell when the second log lies
/// south (FLAG_MAPSEC_TEMP_06; FLAG_MAPSEC_TEMP_05 clear), else 144 pixels
/// (FLAG_MAPSEC_TEMP_04 set).
static void FuchinFallsCave_ZephyrRoom_MoveFirstLogEast(void) {
  Sound_PlaySong(SE_WATER_MOVE);

  if (GameFlag_Get(FLAG_MAPSEC_TEMP_06) != 0) {
    ObjectEvent_PlacePairAtOffset(LOCALID_FIRST_LOG, 16, 0);
    GameFlag_Clear(FLAG_MAPSEC_TEMP_05);
  } else {
    ObjectEvent_PlacePairAtOffset(LOCALID_FIRST_LOG, 144, 0);

    GameFlag_Set(FLAG_MAPSEC_TEMP_04);
  }

  FuchinFallsCave_FinishLogMove(FuchinFallsCave_RebuildZephyrRoomLogs);
}

/// Fuchin Falls Cave, second room: pushing the
/// second log south (coord event 13) floats it 64 pixels south
/// (FLAG_MAPSEC_TEMP_06 set).
static void FuchinFallsCave_ZephyrRoom_MoveSecondLogSouth(void) {
  Sound_PlaySong(SE_WATER_MOVE);
  ObjectEvent_PlacePairAtOffset(LOCALID_SECOND_LOG, 0, 64);
  Sound_PlaySong(SONG_FADE_OUT_ACTION_SE);
  GameFlag_Set(FLAG_MAPSEC_TEMP_06);
  WaitFrames(2);
  FuchinFallsCave_RebuildZephyrRoomLogs();
}

/// Fuchin Falls Cave, second room: pushing the
/// second log north (coord event 14) floats it 64 pixels north
/// (FLAG_MAPSEC_TEMP_06 clear).
static void FuchinFallsCave_ZephyrRoom_MoveSecondLogNorth(void) {
  Sound_PlaySong(SE_WATER_MOVE);
  ObjectEvent_PlacePairAtOffset(LOCALID_SECOND_LOG, 0, -64);
  Sound_PlaySong(SONG_FADE_OUT_ACTION_SE);
  GameFlag_Clear(FLAG_MAPSEC_TEMP_06);
  WaitFrames(2);
  FuchinFallsCave_RebuildZephyrRoomLogs();
}

/// Fuchin Falls Cave, second room: pushing the
/// third log south (coord event 15) floats it 272 pixels south
/// (FLAG_MAPSEC_TEMP_07 clear).
static void FuchinFallsCave_ZephyrRoom_MoveThirdLogSouth(void) {
  Sound_PlaySong(SE_WATER_MOVE);
  ObjectEvent_PlacePairAtOffset(LOCALID_THIRD_LOG, 0, 144);
  ObjectEvent_PlacePairAtOffset(LOCALID_THIRD_LOG, 0, 128);
  Sound_PlaySong(SONG_FADE_OUT_ACTION_SE);
  GameFlag_Clear(FLAG_MAPSEC_TEMP_07);
  WaitFrames(2);
  FuchinFallsCave_RebuildZephyrRoomLogs();
}

/// Fuchin Falls Cave, second room: pushing the
/// third log north (coord event 16) floats it 272 pixels north
/// (FLAG_MAPSEC_TEMP_07 set).
static void FuchinFallsCave_ZephyrRoom_MoveThirdLogNorth(void) {
  s32 depth = -96;

  Sound_PlaySong(SE_WATER_MOVE);
  ObjectEvent_PlacePairAtOffset(LOCALID_THIRD_LOG, 0, depth);
  ObjectEvent_PlacePairAtOffset(LOCALID_THIRD_LOG, 0, depth);
  ObjectEvent_PlacePairAtOffset(LOCALID_THIRD_LOG, 0, -80);
  Sound_PlaySong(SONG_FADE_OUT_ACTION_SE);
  GameFlag_Set(FLAG_MAPSEC_TEMP_07);
  WaitFrames(2);
  FuchinFallsCave_RebuildZephyrRoomLogs();
}

/// Fuchin Falls Cave, third room: pushing
/// the first log west (coord event 10): 48 pixels (FLAG_MAPSEC_TEMP_09 set)
/// when the third log lies at FLAG_MAPSEC_TEMP_0D or the fourth at
/// FLAG_MAPSEC_TEMP_10, else 96 (FLAG_MAPSEC_TEMP_08 set).
static void FuchinFallsCave_DragonsEyeRoom_MoveFirstLogWest(void) {
  Sound_PlaySong(SE_WATER_MOVE);

  if (GameFlag_Get(FLAG_MAPSEC_TEMP_10) != 0 ||
      GameFlag_Get(FLAG_MAPSEC_TEMP_0D) != 0) {
    ObjectEvent_PlacePairAtOffset(LOCALID_FIRST_LOG, -48, 0);
    GameFlag_Clear(FLAG_MAPSEC_TEMP_08);
    GameFlag_Set(FLAG_MAPSEC_TEMP_09);
  } else {
    ObjectEvent_PlacePairAtOffset(LOCALID_FIRST_LOG, -96, 0);
    GameFlag_Set(FLAG_MAPSEC_TEMP_08);
    GameFlag_Clear(FLAG_MAPSEC_TEMP_09);
  }

  FuchinFallsCave_FinishLogMove(FuchinFallsCave_RebuildDragonsEyeRoomLogs);
}

/// Fuchin Falls Cave, third room: pushing
/// the first log east (coord event 11) floats it 48 pixels east
/// (FLAG_MAPSEC_TEMP_08 and FLAG_MAPSEC_TEMP_09 clear).
static void FuchinFallsCave_DragonsEyeRoom_MoveFirstLogEastFromMiddle(void) {
  Sound_PlaySong(SE_WATER_MOVE);
  GameFlag_Clear(FLAG_MAPSEC_TEMP_08);
  GameFlag_Clear(FLAG_MAPSEC_TEMP_09);
  ObjectEvent_PlacePairAtOffset(LOCALID_FIRST_LOG, 48, 0);
  FuchinFallsCave_FinishLogMove(FuchinFallsCave_RebuildDragonsEyeRoomLogs);
}

/// Fuchin Falls Cave, third room: pushing
/// the first log west (coord event 11), unless the third log lies at
/// FLAG_MAPSEC_TEMP_0D or the fourth at FLAG_MAPSEC_TEMP_10, floats it 48
/// pixels west (FLAG_MAPSEC_TEMP_08 set).
static void FuchinFallsCave_DragonsEyeRoom_MoveFirstLogWestIfClear(void) {
  if (GameFlag_Get(FLAG_MAPSEC_TEMP_10) != 0) {
    return;
  }
  if (GameFlag_Get(FLAG_MAPSEC_TEMP_0D) != 0) {
    return;
  }

  Sound_PlaySong(SE_WATER_MOVE);

  GameFlag_Set(FLAG_MAPSEC_TEMP_08);
  GameFlag_Clear(FLAG_MAPSEC_TEMP_09);

  ObjectEvent_PlacePairAtOffset(LOCALID_FIRST_LOG, -48, 0);
  FuchinFallsCave_FinishLogMove(FuchinFallsCave_RebuildDragonsEyeRoomLogs);
}

/// Fuchin Falls Cave, third room: pushing
/// the first log east (coord event 12) floats it 96 pixels east
/// (FLAG_MAPSEC_TEMP_08 and FLAG_MAPSEC_TEMP_09 clear).
static void FuchinFallsCave_DragonsEyeRoom_MoveFirstLogEastFromWestEnd(void) {
  Sound_PlaySong(SE_WATER_MOVE);
  GameFlag_Clear(FLAG_MAPSEC_TEMP_08);
  GameFlag_Clear(FLAG_MAPSEC_TEMP_09);
  ObjectEvent_PlacePairAtOffset(LOCALID_FIRST_LOG, 96, 0);
  FuchinFallsCave_FinishLogMove(FuchinFallsCave_RebuildDragonsEyeRoomLogs);
}

/// Fuchin Falls Cave, third room: pushing
/// the second log west (coord event 14) floats it 32 pixels west
/// (FLAG_MAPSEC_TEMP_0A set).
static void FuchinFallsCave_DragonsEyeRoom_MoveSecondLogWest(void) {
  Sound_PlaySong(SE_WATER_MOVE);
  ObjectEvent_PlacePairAtOffset(LOCALID_SECOND_LOG, -32, 0);
  Sound_PlaySong(SONG_FADE_OUT_ACTION_SE);
  GameFlag_Set(FLAG_MAPSEC_TEMP_0A);
  WaitFrames(2);
  FuchinFallsCave_RebuildDragonsEyeRoomLogs();
}

/// Fuchin Falls Cave, third room: pushing
/// the second log east (coord event 13) floats it 32 pixels east
/// (FLAG_MAPSEC_TEMP_0A clear).
static void FuchinFallsCave_DragonsEyeRoom_MoveSecondLogEast(void) {
  Sound_PlaySong(SE_WATER_MOVE);
  ObjectEvent_PlacePairAtOffset(LOCALID_SECOND_LOG, 32, 0);
  Sound_PlaySong(SONG_FADE_OUT_ACTION_SE);
  GameFlag_Clear(FLAG_MAPSEC_TEMP_0A);
  WaitFrames(2);
  FuchinFallsCave_RebuildDragonsEyeRoomLogs();
}

/// Fuchin Falls Cave, third room: pushing
/// the third log north (coord event 15): 64 pixels (FLAG_MAPSEC_TEMP_0C) when
/// the first log lies at FLAG_MAPSEC_TEMP_08, else 128 (FLAG_MAPSEC_TEMP_0B).
static void FuchinFallsCave_DragonsEyeRoom_MoveThirdLogNorthFromSouthEnd(void) {
  Sound_PlaySong(SE_WATER_MOVE);
  if (GameFlag_Get(FLAG_MAPSEC_TEMP_08) != 0) {
    ObjectEvent_PlacePairAtOffset(LOCALID_THIRD_LOG, 0, -64);
    GameFlag_Clear(FLAG_MAPSEC_TEMP_0B);
    GameFlag_Set(FLAG_MAPSEC_TEMP_0C);
    GameFlag_Clear(FLAG_MAPSEC_TEMP_0D);
    GameFlag_Clear(FLAG_MAPSEC_TEMP_0E);
  } else {
    ObjectEvent_PlacePairAtOffset(LOCALID_THIRD_LOG, 0, -128);
    GameFlag_Set(FLAG_MAPSEC_TEMP_0B);
    GameFlag_Clear(FLAG_MAPSEC_TEMP_0C);
    GameFlag_Clear(FLAG_MAPSEC_TEMP_0D);
    GameFlag_Clear(FLAG_MAPSEC_TEMP_0E);
  }
  FuchinFallsCave_FinishLogMove(FuchinFallsCave_RebuildDragonsEyeRoomLogs);
}

/// Fuchin Falls Cave, third room: pushing
/// the third log south (coord event 16): one cell (FLAG_MAPSEC_TEMP_0C) when
/// the first log lies at FLAG_MAPSEC_TEMP_08 or the fourth at
/// FLAG_MAPSEC_TEMP_10, 64 pixels (FLAG_MAPSEC_TEMP_0D) when the fourth lies at
/// FLAG_MAPSEC_TEMP_11, else 128 (its stops clear).
static void FuchinFallsCave_DragonsEyeRoom_MoveThirdLogSouthFromNorthEnd(void) {
  Sound_PlaySong(SE_WATER_MOVE);

  if (GameFlag_Get(FLAG_MAPSEC_TEMP_08) != 0) {
    ObjectEvent_PlacePairAtOffset(LOCALID_THIRD_LOG, 0, 16);
    GameFlag_Clear(FLAG_MAPSEC_TEMP_0B);
    GameFlag_Set(FLAG_MAPSEC_TEMP_0C);
    GameFlag_Clear(FLAG_MAPSEC_TEMP_0D);
    GameFlag_Clear(FLAG_MAPSEC_TEMP_0E);
  } else if (GameFlag_Get(FLAG_MAPSEC_TEMP_10) != 0) {
    ObjectEvent_PlacePairAtOffset(LOCALID_THIRD_LOG, 0, 16);
    GameFlag_Clear(FLAG_MAPSEC_TEMP_0B);
    GameFlag_Set(FLAG_MAPSEC_TEMP_0C);
    GameFlag_Clear(FLAG_MAPSEC_TEMP_0D);
    GameFlag_Clear(FLAG_MAPSEC_TEMP_0E);
  } else if (GameFlag_Get(FLAG_MAPSEC_TEMP_11) != 0) {
    ObjectEvent_PlacePairAtOffset(LOCALID_THIRD_LOG, 0, 64);
    GameFlag_Clear(FLAG_MAPSEC_TEMP_0B);
    GameFlag_Clear(FLAG_MAPSEC_TEMP_0C);
    GameFlag_Set(FLAG_MAPSEC_TEMP_0D);
    GameFlag_Clear(FLAG_MAPSEC_TEMP_0E);
  } else {
    ObjectEvent_PlacePairAtOffset(LOCALID_THIRD_LOG, 0, 128);
    GameFlag_Clear(FLAG_MAPSEC_TEMP_0B);
    GameFlag_Clear(FLAG_MAPSEC_TEMP_0C);
    GameFlag_Clear(FLAG_MAPSEC_TEMP_0D);
    GameFlag_Clear(FLAG_MAPSEC_TEMP_0E);
  }
  FuchinFallsCave_FinishLogMove(FuchinFallsCave_RebuildDragonsEyeRoomLogs);
}

/// Fuchin Falls Cave, third room: pushing
/// the third log north (coord event 17) moves it one cell
/// (FLAG_MAPSEC_TEMP_0B).
static void FuchinFallsCave_DragonsEyeRoom_NudgeThirdLogNorth(void) {
  Sound_PlaySong(SE_WATER_MOVE);
  ObjectEvent_PlacePairAtOffset(LOCALID_THIRD_LOG, 0, -16);
  Sound_PlaySong(SONG_FADE_OUT_ACTION_SE);
  GameFlag_Set(FLAG_MAPSEC_TEMP_0B);
  GameFlag_Clear(FLAG_MAPSEC_TEMP_0C);
  GameFlag_Clear(FLAG_MAPSEC_TEMP_0D);
  GameFlag_Clear(FLAG_MAPSEC_TEMP_0E);
  WaitFrames(2);
  FuchinFallsCave_RebuildDragonsEyeRoomLogs();
}

/// Fuchin Falls Cave, third room: pushing
/// the third log south (coord event 17), unless the first log lies at
/// FLAG_MAPSEC_TEMP_08: 48 pixels (FLAG_MAPSEC_TEMP_0D) when the fourth lies at
/// FLAG_MAPSEC_TEMP_11, 32 (FLAG_MAPSEC_TEMP_0C) when it lies at
/// FLAG_MAPSEC_TEMP_10, else 112 (its stops clear).
static void
FuchinFallsCave_DragonsEyeRoom_MoveThirdLogSouthFromUpperStop(void) {
  Sound_PlaySong(SE_WATER_MOVE);
  if (GameFlag_Get(FLAG_MAPSEC_TEMP_11) != 0) {
    ObjectEvent_PlacePairAtOffset(LOCALID_THIRD_LOG, 0, 48);
    GameFlag_Clear(FLAG_MAPSEC_TEMP_0B);

    GameFlag_Clear(FLAG_MAPSEC_TEMP_0C);
    GameFlag_Set(FLAG_MAPSEC_TEMP_0D);
    GameFlag_Clear(FLAG_MAPSEC_TEMP_0E);
  } else if (GameFlag_Get(FLAG_MAPSEC_TEMP_10) != 0) {
    ObjectEvent_PlacePairAtOffset(LOCALID_THIRD_LOG, 0, 32);
    GameFlag_Clear(FLAG_MAPSEC_TEMP_0B);
    GameFlag_Set(FLAG_MAPSEC_TEMP_0C);
    GameFlag_Clear(FLAG_MAPSEC_TEMP_0D);
    GameFlag_Clear(FLAG_MAPSEC_TEMP_0E);
  } else {
    ObjectEvent_PlacePairAtOffset(LOCALID_THIRD_LOG, 0, 112);
    GameFlag_Clear(FLAG_MAPSEC_TEMP_0B);
    GameFlag_Clear(FLAG_MAPSEC_TEMP_0C);
    GameFlag_Clear(FLAG_MAPSEC_TEMP_0D);
    GameFlag_Clear(FLAG_MAPSEC_TEMP_0E);
  }
  FuchinFallsCave_FinishLogMove(FuchinFallsCave_RebuildDragonsEyeRoomLogs);
}

/// Fuchin Falls Cave, third room: pushing
/// the third log north (coord event 18) floats it 64 pixels north
/// (FLAG_MAPSEC_TEMP_0B).
static void FuchinFallsCave_DragonsEyeRoom_MoveThirdLogNorthFromMiddle(void) {
  Sound_PlaySong(SE_WATER_MOVE);
  ObjectEvent_PlacePairAtOffset(LOCALID_THIRD_LOG, 0, -64);
  Sound_PlaySong(SONG_FADE_OUT_ACTION_SE);
  GameFlag_Set(FLAG_MAPSEC_TEMP_0B);
  GameFlag_Clear(FLAG_MAPSEC_TEMP_0D);
  WaitFrames(2);
  FuchinFallsCave_RebuildDragonsEyeRoomLogs();
}

/// Fuchin Falls Cave, third room: pushing
/// the third log south (coord event 18), unless the fourth lies at
/// FLAG_MAPSEC_TEMP_10, floats it 64 pixels south (its stops clear).
static void FuchinFallsCave_DragonsEyeRoom_MoveThirdLogSouthFromMiddle(void) {
  Sound_PlaySong(SE_WATER_MOVE);
  ObjectEvent_PlacePairAtOffset(LOCALID_THIRD_LOG, 0, 64);
  Sound_PlaySong(SONG_FADE_OUT_ACTION_SE);
  GameFlag_Clear(FLAG_MAPSEC_TEMP_0B);
  GameFlag_Clear(FLAG_MAPSEC_TEMP_0C);
  GameFlag_Clear(FLAG_MAPSEC_TEMP_0D);
  GameFlag_Clear(FLAG_MAPSEC_TEMP_0E);
  WaitFrames(2);
  FuchinFallsCave_RebuildDragonsEyeRoomLogs();
}

/// Fuchin Falls Cave, third room: pushing
/// the third log north (coord event 19) floats it 80 pixels north
/// (FLAG_MAPSEC_TEMP_0B).
static void
FuchinFallsCave_DragonsEyeRoom_MoveThirdLogNorthFromLowerStop(void) {
  Sound_PlaySong(SE_WATER_MOVE);
  ObjectEvent_PlacePairAtOffset(LOCALID_THIRD_LOG, 0, -80);
  Sound_PlaySong(SONG_FADE_OUT_ACTION_SE);
  GameFlag_Set(FLAG_MAPSEC_TEMP_0B);
  GameFlag_Clear(FLAG_MAPSEC_TEMP_0C);
  GameFlag_Clear(FLAG_MAPSEC_TEMP_0D);
  GameFlag_Clear(FLAG_MAPSEC_TEMP_0E);
  WaitFrames(2);
  FuchinFallsCave_RebuildDragonsEyeRoomLogs();
}

/// Fuchin Falls Cave, third room: pushing
/// the third log south (coord event 19) floats it 64 pixels south (its stops
/// clear).
static void
FuchinFallsCave_DragonsEyeRoom_MoveThirdLogSouthFromLowerStop(void) {
  Sound_PlaySong(SE_WATER_MOVE);
  ObjectEvent_PlacePairAtOffset(LOCALID_THIRD_LOG, 0, 64);
  Sound_PlaySong(SONG_FADE_OUT_ACTION_SE);
  GameFlag_Clear(FLAG_MAPSEC_TEMP_0B);
  GameFlag_Clear(FLAG_MAPSEC_TEMP_0C);
  GameFlag_Clear(FLAG_MAPSEC_TEMP_0D);
  GameFlag_Clear(FLAG_MAPSEC_TEMP_0E);
  WaitFrames(2);
  FuchinFallsCave_RebuildDragonsEyeRoomLogs();
}

/// Fuchin Falls Cave, third room: pushing
/// the fourth log north (coord event 20): 64 pixels (FLAG_MAPSEC_TEMP_11) when
/// the first log lies at FLAG_MAPSEC_TEMP_08 or the third at
/// FLAG_MAPSEC_TEMP_0D, 112 (FLAG_MAPSEC_TEMP_10) when the third lies at
/// FLAG_MAPSEC_TEMP_0C, else 128 (FLAG_MAPSEC_TEMP_0F).
static void FuchinFallsCave_DragonsEyeRoom_MoveFourthLogNorth(void) {
  Sound_PlaySong(SE_WATER_MOVE);

  if (GameFlag_Get(FLAG_MAPSEC_TEMP_08) != 0 ||
      GameFlag_Get(FLAG_MAPSEC_TEMP_0D) != 0) {
    ObjectEvent_PlacePairAtOffset(LOCALID_FOURTH_LOG, 0, -64);
    GameFlag_Clear(FLAG_MAPSEC_TEMP_0F);
    GameFlag_Clear(FLAG_MAPSEC_TEMP_10);
    GameFlag_Set(FLAG_MAPSEC_TEMP_11);
    GameFlag_Clear(FLAG_MAPSEC_TEMP_12);
  } else if (GameFlag_Get(FLAG_MAPSEC_TEMP_0C) != 0) {
    ObjectEvent_PlacePairAtOffset(LOCALID_FOURTH_LOG, 0, -112);
    GameFlag_Clear(FLAG_MAPSEC_TEMP_0F);
    GameFlag_Set(FLAG_MAPSEC_TEMP_10);
    GameFlag_Clear(FLAG_MAPSEC_TEMP_11);
    GameFlag_Clear(FLAG_MAPSEC_TEMP_12);
  } else {
    ObjectEvent_PlacePairAtOffset(LOCALID_FOURTH_LOG, 0, -128);
    GameFlag_Set(FLAG_MAPSEC_TEMP_0F);
    GameFlag_Clear(FLAG_MAPSEC_TEMP_10);
    GameFlag_Clear(FLAG_MAPSEC_TEMP_11);
    GameFlag_Clear(FLAG_MAPSEC_TEMP_12);
  }
  FuchinFallsCave_FinishLogMove(FuchinFallsCave_RebuildDragonsEyeRoomLogs);
}

/// Clears all saved intermediate stops for the fourth log.
static void FuchinFallsCave_ClearFourthLogStops(void) {
  GameFlag_Clear(FLAG_MAPSEC_TEMP_0F);
  GameFlag_Clear(FLAG_MAPSEC_TEMP_10);
  GameFlag_Clear(FLAG_MAPSEC_TEMP_11);
  GameFlag_Clear(FLAG_MAPSEC_TEMP_12);
}

/// Fuchin Falls Cave, third room: pushing
/// the fourth log south (coord event 21), unless the first log lies at
/// FLAG_MAPSEC_TEMP_08, floats it 128 pixels south (its stops clear).
static void
FuchinFallsCave_DragonsEyeRoom_MoveFourthLogSouthFromNorthEnd(void) {
  Sound_PlaySong(SE_WATER_MOVE);
  ObjectEvent_PlacePairAtOffset(LOCALID_FOURTH_LOG, 0, 128);
  FuchinFallsCave_ClearFourthLogStops();
  FuchinFallsCave_FinishLogMove(FuchinFallsCave_RebuildDragonsEyeRoomLogs);
}

/// Fuchin Falls Cave, third room: pushing
/// the fourth log north (coord event 22) moves it one cell
/// (FLAG_MAPSEC_TEMP_0F).
static void FuchinFallsCave_DragonsEyeRoom_NudgeFourthLogNorth(void) {
  Sound_PlaySong(SE_WATER_MOVE);
  ObjectEvent_PlacePairAtOffset(LOCALID_FOURTH_LOG, 0, -16);
  Sound_PlaySong(SONG_FADE_OUT_ACTION_SE);
  GameFlag_Set(FLAG_MAPSEC_TEMP_0F);
  GameFlag_Clear(FLAG_MAPSEC_TEMP_10);
  GameFlag_Clear(FLAG_MAPSEC_TEMP_11);
  GameFlag_Clear(FLAG_MAPSEC_TEMP_12);
  WaitFrames(2);
  FuchinFallsCave_RebuildDragonsEyeRoomLogs();
}

/// Fuchin Falls Cave, third room: pushing
/// the fourth log south (coord event 22) floats it 112 pixels south (its stops
/// clear).
static void
FuchinFallsCave_DragonsEyeRoom_MoveFourthLogSouthFromUpperStop(void) {
  Sound_PlaySong(SE_WATER_MOVE);
  ObjectEvent_PlacePairAtOffset(LOCALID_FOURTH_LOG, 0, 112);
  Sound_PlaySong(SONG_FADE_OUT_ACTION_SE);
  FuchinFallsCave_ClearFourthLogStops();
  WaitFrames(2);
  FuchinFallsCave_RebuildDragonsEyeRoomLogs();
}

/// Fuchin Falls Cave, third room: pushing
/// the fourth log south (coord event 23) floats it 64 pixels south (its stops
/// clear).
static void FuchinFallsCave_DragonsEyeRoom_MoveFourthLogSouthFromMiddle(void) {
  Sound_PlaySong(SE_WATER_MOVE);
  ObjectEvent_PlacePairAtOffset(LOCALID_FOURTH_LOG, 0, 64);
  Sound_PlaySong(SONG_FADE_OUT_ACTION_SE);
  FuchinFallsCave_ClearFourthLogStops();
  WaitFrames(2);
  FuchinFallsCave_RebuildDragonsEyeRoomLogs();
}

/// Unused: walking north into coord event 24 in Fuchin Falls Cave's third room
/// would float the fourth log 80 pixels south (its stops clear). Coord event
/// 24's cells reach the room only from their copy at columns 10 to 12, row 43,
/// which FuchinFallsCave_RebuildDragonsEyeRoomLogs makes with the log at
/// FLAG_MAPSEC_TEMP_12, and no move sets that flag, so the room never has a
/// coord event 24 (in tbs-en it appears there only with the flag set by hand;
/// walking north on the log then carries the leader 80 pixels south, past the
/// log's south end, where the rebuild puts the log back). XXX: likely a slip
/// for a move north, because the third log's matching coord event 19 floats it
/// north.
static void
FuchinFallsCave_DragonsEyeRoom_UnusedMoveFourthLogSouthFromLowerStop(void) {
  Sound_PlaySong(SE_WATER_MOVE);
  ObjectEvent_PlacePairAtOffset(LOCALID_FOURTH_LOG, 0, 80);
  Sound_PlaySong(SONG_FADE_OUT_ACTION_SE);
  FuchinFallsCave_ClearFourthLogStops();
  WaitFrames(2);
  FuchinFallsCave_RebuildDragonsEyeRoomLogs();
}

/// Unused: pushing the fourth log south from coord event 24 in Fuchin Falls
/// Cave's third room would float it 48 pixels south (its stops clear), back to
/// its south end; the room never has a coord event 24, as
/// FuchinFallsCave_DragonsEyeRoom_UnusedMoveFourthLogSouthFromLowerStop says.
static void
FuchinFallsCave_DragonsEyeRoom_UnusedMoveFourthLogSouthFromLastStop(void) {
  Sound_PlaySong(SE_WATER_MOVE);
  ObjectEvent_PlacePairAtOffset(LOCALID_FOURTH_LOG, 0, 48);
  Sound_PlaySong(SONG_FADE_OUT_ACTION_SE);
  FuchinFallsCave_ClearFourthLogStops();
  WaitFrames(2);
  FuchinFallsCave_RebuildDragonsEyeRoomLogs();
}

/// Fuchin Falls Cave, fourth room: pushing
/// the first log south (coord event 11) floats it 112 pixels south
/// (FLAG_MAPSEC_TEMP_13 set).
static void FuchinFallsCave_ArcticBladeRoom_MoveFirstLogSouth(void) {
  Sound_PlaySong(SE_WATER_MOVE);
  ObjectEvent_PlacePairAtOffset(LOCALID_FIRST_LOG, 0, 112);
  Sound_PlaySong(SONG_FADE_OUT_ACTION_SE);
  GameFlag_Set(FLAG_MAPSEC_TEMP_13);
  WaitFrames(2);
  FuchinFallsCave_RebuildArcticBladeRoomLogs();
}

/// Fuchin Falls Cave, fourth room: pushing
/// the first log north (coord event 12) floats it 112 pixels north
/// (FLAG_MAPSEC_TEMP_13 clear).
static void FuchinFallsCave_ArcticBladeRoom_MoveFirstLogNorth(void) {
  Sound_PlaySong(SE_WATER_MOVE);
  ObjectEvent_PlacePairAtOffset(LOCALID_FIRST_LOG, 0, -112);
  Sound_PlaySong(SONG_FADE_OUT_ACTION_SE);
  GameFlag_Clear(FLAG_MAPSEC_TEMP_13);
  WaitFrames(2);
  FuchinFallsCave_RebuildArcticBladeRoomLogs();
}

/// Fuchin Falls Cave, fourth room: pushing
/// the second log west (coord event 13), while the first log lies north
/// (FLAG_MAPSEC_TEMP_13 clear), floats it 128 pixels west (FLAG_MAPSEC_TEMP_14
/// set).
static void FuchinFallsCave_ArcticBladeRoom_MoveSecondLogWest(void) {
  Sound_PlaySong(SE_WATER_MOVE);
  ObjectEvent_PlacePairAtOffset(LOCALID_SECOND_LOG, -128, 0);
  Sound_PlaySong(SONG_FADE_OUT_ACTION_SE);
  GameFlag_Set(FLAG_MAPSEC_TEMP_14);
  WaitFrames(2);
  FuchinFallsCave_RebuildArcticBladeRoomLogs();
}

/// Fuchin Falls Cave, fourth room: pushing
/// the second log east (coord event 14) floats it 128 pixels east
/// (FLAG_MAPSEC_TEMP_14 clear).
static void FuchinFallsCave_ArcticBladeRoom_MoveSecondLogEast(void) {
  Sound_PlaySong(SE_WATER_MOVE);
  ObjectEvent_PlacePairAtOffset(LOCALID_SECOND_LOG, 128, 0);
  Sound_PlaySong(SONG_FADE_OUT_ACTION_SE);
  GameFlag_Clear(FLAG_MAPSEC_TEMP_14);
  WaitFrames(2);
  FuchinFallsCave_RebuildArcticBladeRoomLogs();
}

/// Fuchin Falls Cave, fourth room: pushing
/// the third log east (coord event 15) floats it 160 pixels east
/// (FLAG_MAPSEC_TEMP_15 set).
static void FuchinFallsCave_ArcticBladeRoom_MoveThirdLogEast(void) {
  Sound_PlaySong(SE_WATER_MOVE);
  ObjectEvent_PlacePairAtOffset(LOCALID_THIRD_LOG, 160, 0);
  Sound_PlaySong(SONG_FADE_OUT_ACTION_SE);
  GameFlag_Set(FLAG_MAPSEC_TEMP_15);
  WaitFrames(2);
  FuchinFallsCave_RebuildArcticBladeRoomLogs();
}

/// Fuchin Falls Cave, fourth room: pushing
/// the third log west (coord event 16) floats it 160 pixels west
/// (FLAG_MAPSEC_TEMP_15 clear).
static void FuchinFallsCave_ArcticBladeRoom_MoveThirdLogWest(void) {
  Sound_PlaySong(SE_WATER_MOVE);
  ObjectEvent_PlacePairAtOffset(LOCALID_THIRD_LOG, -160, 0);
  Sound_PlaySong(SONG_FADE_OUT_ACTION_SE);
  GameFlag_Clear(FLAG_MAPSEC_TEMP_15);
  WaitFrames(2);
  FuchinFallsCave_RebuildArcticBladeRoomLogs();
}

/// Touching the first log's coord event cell in Fuchin Falls Cave plays its two
/// rolling animations.
static void FuchinFallsCave_AnimateFirstLog(void) {
  Engine_ObjectEventSetAnimation(LOCALID_FIRST_LOG, 1);
  Engine_ObjectEventSetAnimation(LOCALID_FIRST_LOG, 2);
}

/// Touching the second log's coord event cell in Fuchin Falls Cave plays its
/// two rolling animations.
static void FuchinFallsCave_AnimateSecondLog(void) {
  Engine_ObjectEventSetAnimation(LOCALID_SECOND_LOG, 1);
  Engine_ObjectEventSetAnimation(LOCALID_SECOND_LOG, 2);
}

/// Touching the third log's coord event cell in Fuchin Falls Cave plays its two
/// rolling animations.
static void FuchinFallsCave_AnimateThirdLog(void) {
  Engine_ObjectEventSetAnimation(LOCALID_THIRD_LOG, 1);
  Engine_ObjectEventSetAnimation(LOCALID_THIRD_LOG, 2);
}

/// Touching the fourth log's coord event cell in Fuchin Falls Cave (third
/// room) plays its two rolling animations.
static void FuchinFallsCave_AnimateFourthLog(void) {
  Engine_ObjectEventSetAnimation(LOCALID_FOURTH_LOG, 1);
  Engine_ObjectEventSetAnimation(LOCALID_FOURTH_LOG, 2);
}

/// Creates a stationary object event at sprite priority 1, drawn hidden
/// (SPRITE_COLOR_HIDDEN).
static struct ObjectEvent *
FuchinFallsCave_CreateForegroundObjectEvent(s32 x, s32 y, s32 z,
                                            s32 resourceId) {
  struct ObjectEvent *objectEvent = ObjectEvent_Create(resourceId, x, y, z);

  if (objectEvent != 0) {
    objectEvent->sprite->image.priority = 1;
    objectEvent->motionFlags = 0;
    ObjectEvent_SetSpriteFlags(objectEvent, 0);
    ObjectGroup_SetChildValue(objectEvent, SPRITE_COLOR_HIDDEN);
    objectEvent->priorityFlags |= OBJ_EVENT_PRIORITY_UNDERFOOT;
    return objectEvent;
  }
  return 0;
}

const struct MapEvent *ImilFuchinFalls_GetEvents(void) {
  s16 mapId = gSaveBlock.mapId;

  if (mapId == (s32)&MapId_ImilFallsCave) {
    return gImilFuchinFalls_ImilFallsCaveEvents;
  }
  if (mapId == (s32)&MapId_FuchinFallsCaveEntrance) {
    return gImilFuchinFalls_EntranceEvents;
  }
  if (mapId == (s32)&MapId_FuchinFallsCaveZephyr) {
    return gImilFuchinFalls_ZephyrRoomEvents;
  }
  if (mapId == (s32)&MapId_FuchinFallsCaveDragonsEye) {
    return gImilFuchinFalls_DragonsEyeRoomEvents;
  }
  if (mapId == (s32)&MapId_FuchinFallsCaveArcticBlade) {
    return gImilFuchinFalls_ArcticBladeRoomEvents;
  }
  if (mapId == (s32)&MapId_FuchinFallsCaveOrbOfForce) {
    return gImilFuchinFalls_KiRoomEvents;
  }
  if (mapId == (s32)&MapId_FuchinFallsCaveDragonStatue) {
    return gImilFuchinFalls_DragonRoomEvents;
  }
  return gImilFuchinFalls_NoEvents;
}

/// Walking into the way out of Imil Falls Cave (coord event 1): faces the
/// leader south and leaves for Imil.
static void Imil_LeaveFallsCave(void) {
  struct ObjectEvent *leader;

  FieldScript_Start();
  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  leader->facing = FACING_SOUTH;
  Sound_PlaySong(SE_MAP_EXIT);
  FieldScript_FinishScreenTransition();
  FieldScript_WaitTransitionFrames();
  Engine_ScriptRequestWarp(IMIL_FALLS_CAVE_TO_IMIL);
}

/// Stepping onto the ice in Imil Falls Cave (metatile behavior 10) slides
/// the selected object event in the held direction.
static void Imil_MoveAcrossIce(void) { Imil_SlideObjectEventAcrossIce(); }

/// Centers a log on its cell and gives it underfoot priority.
static void FuchinFallsCave_PlaceLogAtCell(s32 id, s32 column, s32 row) {
  struct ObjectEvent *objectEvent = ObjectEvent_GetById(id);

  if (objectEvent != NULL) {
    ObjectMotion_SetActionVariant(id, 3);
    objectEvent->layer = 2;
    objectEvent->priorityFlags |= OBJ_EVENT_PRIORITY_UNDERFOOT;
    objectEvent->x.fixed = PIXELS(column * 16) + PIXELS(8);
    objectEvent->z.fixed = PIXELS(row * 16) + PIXELS(8);
  }
}

/// Puts the logs of Fuchin Falls Cave's first room
/// (MapId_FuchinFallsCaveEntrance) at the cells their flags name, and
/// rewrites the cells of their tracks to match.
static void FuchinFallsCave_RebuildEntranceLogs(void) {
  {
    s32 column = 8;
    s32 row = 29;
    MapGrid_CopyCellAttributes(8, 42, 15, 5, column, row);
  }

  if (GameFlag_Get(FLAG_MAPSEC_TEMP_01) != 0) {
    FuchinFallsCave_PlaceLogAtCell(LOCALID_FIRST_LOG, 22, 31);
    {
      s32 column = 8;
      s32 row = 30;
      MapGrid_CopyCellAttributes(9, 30, 1, 3, column, row);
    }
  } else {
    FuchinFallsCave_PlaceLogAtCell(LOCALID_FIRST_LOG, 8, 31);
    {
      s32 column = 22;
      s32 row = 30;
      MapGrid_CopyCellAttributes(9, 30, 1, 3, column, row);
    }
  }

  if (GameFlag_Get(FLAG_MAPSEC_TEMP_02) != 0) {
    FuchinFallsCave_PlaceLogAtCell(LOCALID_SECOND_LOG, 12, 29);
    {
      s32 column = 11;
      s32 row = 33;
      MapGrid_CopyCellAttributes(14, 33, 3, 1, column, row);
    }
  } else {
    FuchinFallsCave_PlaceLogAtCell(LOCALID_SECOND_LOG, 12, 33);
    {
      s32 column = 11;
      s32 row = 29;
      MapGrid_CopyCellAttributes(14, 29, 3, 1, column, row);
    }
  }

  if (GameFlag_Get(FLAG_MAPSEC_TEMP_03) != 0) {
    FuchinFallsCave_PlaceLogAtCell(LOCALID_THIRD_LOG, 18, 29);
    {
      s32 column = 17;
      s32 row = 33;
      MapGrid_CopyCellAttributes(14, 33, 3, 1, column, row);
    }
  } else {
    FuchinFallsCave_PlaceLogAtCell(LOCALID_THIRD_LOG, 18, 33);
    {
      s32 column = 17;
      s32 row = 29;
      MapGrid_CopyCellAttributes(14, 29, 3, 1, column, row);
    }
  }
}

/// Puts the logs of Fuchin Falls Cave's second room where their flags say. The
/// flags name cells in MapId_FuchinFallsCaveZephyr, and the cells of the
/// logs' tracks are rewritten to match.
static void FuchinFallsCave_RebuildZephyrRoomLogs(void) {
  {
    s32 column = 12;
    s32 row = 8;
    MapGrid_CopyCellAttributes(0, 28, 10, 18, column, row);
  }

  if (GameFlag_Get(FLAG_MAPSEC_TEMP_04) != 0) {
    FuchinFallsCave_PlaceLogAtCell(LOCALID_FIRST_LOG, 21, 20);
    {
      s32 column = 13;
      s32 row = 19;
      MapGrid_CopyCellAttributes(20, 19, 1, 3, column, row);
    }
  } else {
    FuchinFallsCave_PlaceLogAtCell(LOCALID_FIRST_LOG, 13, 20);
    {
      s32 column = 21;
      s32 row = 19;
      MapGrid_CopyCellAttributes(20, 19, 1, 3, column, row);
    }
  }

  if (GameFlag_Get(FLAG_MAPSEC_TEMP_05) != 0) {
    FuchinFallsCave_PlaceLogAtCell(LOCALID_FIRST_LOG, 12, 20);
    {
      s32 column = 12;
      s32 row = 19;
      MapGrid_CopyCellAttributes(5, 19, 1, 3, column, row);
    }
    {
      s32 column = 13;
      s32 row = 19;
      MapGrid_CopyCellAttributes(20, 19, 1, 3, column, row);
    }
    if (GameFlag_Get(FLAG_MAPSEC_TEMP_04) != 0) {
      FuchinFallsCave_PlaceLogAtCell(LOCALID_FIRST_LOG, 21, 20);
      {
        s32 column = 13;
        s32 row = 19;
        MapGrid_CopyCellAttributes(20, 19, 1, 3, column, row);
      }
      {
        s32 column = 12;
        s32 row = 19;
        MapGrid_CopyCellAttributes(20, 19, 1, 3, column, row);
      }
    }
  }

  if (GameFlag_Get(FLAG_MAPSEC_TEMP_06) != 0) {
    FuchinFallsCave_PlaceLogAtCell(LOCALID_SECOND_LOG, 15, 21);
    {
      s32 column = 14;
      s32 row = 17;
      MapGrid_CopyCellAttributes(14, 18, 3, 1, column, row);
    }
  } else {
    FuchinFallsCave_PlaceLogAtCell(LOCALID_SECOND_LOG, 15, 17);
    {
      s32 column = 14;
      s32 row = 21;
      MapGrid_CopyCellAttributes(14, 18, 3, 1, column, row);
    }
  }

  if (GameFlag_Get(FLAG_MAPSEC_TEMP_07) != 0) {
    FuchinFallsCave_PlaceLogAtCell(LOCALID_THIRD_LOG, 19, 8);
    {
      s32 column = 18;
      s32 row = 25;
      MapGrid_CopyCellAttributes(14, 18, 3, 1, column, row);
    }
  } else {
    FuchinFallsCave_PlaceLogAtCell(LOCALID_THIRD_LOG, 19, 25);
    {
      s32 column = 18;
      s32 row = 8;
      MapGrid_CopyCellAttributes(14, 18, 3, 1, column, row);
    }
  }
}

/// Puts the four logs of Fuchin Falls Cave's third room where their flags say.
/// The flags name cells in MapId_FuchinFallsCaveDragonsEye, and the cells of
/// the logs' tracks are rewritten to match.
static void FuchinFallsCave_RebuildDragonsEyeRoomLogs(void) {
  {
    MapGrid_CopyCellAttributes(12, 3, 9, 16, 12, 21);
  }

  if (GameFlag_Get(FLAG_MAPSEC_TEMP_08) != 0) {
    FuchinFallsCave_PlaceLogAtCell(LOCALID_FIRST_LOG, 14, 25);
    {
      MapGrid_CopyCellAttributes(16, 24, 1, 3, 20, 24);
    }
  } else if (GameFlag_Get(FLAG_MAPSEC_TEMP_09) != 0) {
    FuchinFallsCave_PlaceLogAtCell(LOCALID_FIRST_LOG, 17, 25);
    {
      s32 row = 24;
      MapGrid_CopyCellAttributes(18, 24, 1, 3, 20, row);
      MapGrid_CopyCellAttributes(18, 24, 1, 3, 14, row);
      MapGrid_CopyCellAttributes(8, 41, 1, 3, 17, row);
    }
  } else {
    FuchinFallsCave_PlaceLogAtCell(LOCALID_FIRST_LOG, 20, 25);
    {
      MapGrid_CopyCellAttributes(16, 24, 1, 3, 14, 24);
    }
  }

  if (GameFlag_Get(FLAG_MAPSEC_TEMP_0A) != 0) {
    FuchinFallsCave_PlaceLogAtCell(LOCALID_SECOND_LOG, 13, 35);
    {
      MapGrid_CopyCellAttributes(14, 34, 1, 3, 15, 34);
    }
  } else {
    FuchinFallsCave_PlaceLogAtCell(LOCALID_SECOND_LOG, 15, 35);
    {
      MapGrid_CopyCellAttributes(14, 34, 1, 3, 13, 34);
    }
  }

  if (GameFlag_Get(FLAG_MAPSEC_TEMP_0B) != 0) {
    FuchinFallsCave_PlaceLogAtCell(LOCALID_THIRD_LOG, 15, 22);
    {
      s32 column = 14;
      MapGrid_CopyCellAttributes(14, 29, 3, 1, column, 30);
      MapGrid_CopyCellAttributes(5, 41, 3, 1, column, 22);
    }
  } else if (GameFlag_Get(FLAG_MAPSEC_TEMP_0C) != 0) {
    FuchinFallsCave_PlaceLogAtCell(LOCALID_THIRD_LOG, 15, 23);
    {
      s32 column = 14;
      MapGrid_CopyCellAttributes(5, 42, 3, 1, column, 23);
      MapGrid_CopyCellAttributes(14, 29, 3, 1, column, 30);
      MapGrid_CopyCellAttributes(10, 44, 3, 1, column, 21);
    }
  } else if (GameFlag_Get(FLAG_MAPSEC_TEMP_0D) != 0) {
    FuchinFallsCave_PlaceLogAtCell(LOCALID_THIRD_LOG, 15, 26);
    {
      s32 column = 14;
      MapGrid_CopyCellAttributes(14, 29, 3, 1, column, 22);
      MapGrid_CopyCellAttributes(5, 43, 3, 1, column, 26);
      MapGrid_CopyCellAttributes(14, 29, 3, 1, column, 30);
    }
  } else if (GameFlag_Get(FLAG_MAPSEC_TEMP_0E) != 0) {
    FuchinFallsCave_PlaceLogAtCell(LOCALID_THIRD_LOG, 15, 27);
    {
      s32 column = 14;
      MapGrid_CopyCellAttributes(14, 29, 3, 1, column, 22);
      MapGrid_CopyCellAttributes(14, 29, 3, 1, column, 30);
      MapGrid_CopyCellAttributes(5, 44, 3, 1, column, 27);
    }
  } else {
    FuchinFallsCave_PlaceLogAtCell(LOCALID_THIRD_LOG, 15, 30);
  }

  if (GameFlag_Get(FLAG_MAPSEC_TEMP_0F) != 0) {
    FuchinFallsCave_PlaceLogAtCell(LOCALID_FOURTH_LOG, 15, 23);
    {
      s32 column = 14;
      MapGrid_CopyCellAttributes(14, 29, 3, 1, column, 31);
      MapGrid_CopyCellAttributes(10, 40, 3, 1, column, 23);
    }
  } else if (GameFlag_Get(FLAG_MAPSEC_TEMP_10) != 0) {
    FuchinFallsCave_PlaceLogAtCell(LOCALID_FOURTH_LOG, 15, 24);
    {
      s32 column = 14;
      MapGrid_CopyCellAttributes(14, 29, 3, 1, column, 31);
      MapGrid_CopyCellAttributes(10, 41, 3, 1, column, 24);
    }
  } else if (GameFlag_Get(FLAG_MAPSEC_TEMP_11) != 0) {
    FuchinFallsCave_PlaceLogAtCell(LOCALID_FOURTH_LOG, 15, 27);
    {
      s32 column = 14;
      MapGrid_CopyCellAttributes(14, 29, 3, 1, column, 31);
      MapGrid_CopyCellAttributes(10, 42, 3, 1, column, 27);
    }
  } else if (GameFlag_Get(FLAG_MAPSEC_TEMP_12) != 0) {
    // Unused: no move sets FLAG_MAPSEC_TEMP_12, so the log never lies here and
    // the room never gets coord event 24.
    FuchinFallsCave_PlaceLogAtCell(LOCALID_FOURTH_LOG, 15, 28);
    {
      s32 column = 14;
      MapGrid_CopyCellAttributes(14, 29, 3, 1, column, 31);
      MapGrid_CopyCellAttributes(10, 43, 3, 1, column, 28);
    }
  } else {
    FuchinFallsCave_PlaceLogAtCell(LOCALID_FOURTH_LOG, 15, 31);
  }
}

/// Puts the logs of Fuchin Falls Cave's fourth room
/// (MapId_FuchinFallsCaveArcticBlade), in both of its halves, at the cells
/// their flags name.
static void FuchinFallsCave_RebuildArcticBladeRoomLogs(void) {
  if (((u32)((u16)gSaveBlock.warpDest - 1) << 16) <= 0x10000) {
    {
      s32 column = 14;
      s32 row = 10;
      MapGrid_CopyCellAttributes(22, 20, 9, 8, column, row);
    }
  } else {
    {
      s32 column = 7;
      s32 row = 45;
      MapGrid_CopyCellAttributes(20, 45, 11, 4, column, row);
    }
  }

  if (GameFlag_Get(FLAG_MAPSEC_TEMP_13) != 0) {
    FuchinFallsCave_PlaceLogAtCell(LOCALID_FIRST_LOG, 20, 17);
    {
      s32 column = 19;
      s32 row = 10;
      MapGrid_CopyCellAttributes(19, 11, 3, 1, column, row);
    }
  } else {
    FuchinFallsCave_PlaceLogAtCell(LOCALID_FIRST_LOG, 20, 10);
    {
      s32 column = 19;
      s32 row = 17;
      MapGrid_CopyCellAttributes(19, 11, 3, 1, column, row);
    }
  }

  if (GameFlag_Get(FLAG_MAPSEC_TEMP_14) != 0) {
    FuchinFallsCave_PlaceLogAtCell(LOCALID_SECOND_LOG, 14, 16);
    {
      s32 column = 22;
      s32 row = 15;
      MapGrid_CopyCellAttributes(16, 15, 1, 3, column, row);
    }
  } else {
    FuchinFallsCave_PlaceLogAtCell(LOCALID_SECOND_LOG, 22, 16);
    {
      s32 column = 14;
      s32 row = 15;
      MapGrid_CopyCellAttributes(16, 15, 1, 3, column, row);
    }
  }

  if (GameFlag_Get(FLAG_MAPSEC_TEMP_15) != 0) {
    FuchinFallsCave_PlaceLogAtCell(LOCALID_THIRD_LOG, 17, 46);
    {
      s32 column = 7;
      s32 row = 45;
      MapGrid_CopyCellAttributes(15, 15, 1, 3, column, row);
    }
  } else {
    FuchinFallsCave_PlaceLogAtCell(LOCALID_THIRD_LOG, 7, 46);
    {
      s32 column = 17;
      s32 row = 45;
      MapGrid_CopyCellAttributes(15, 15, 1, 3, column, row);
    }
  }
}

/// The fade overlay follows the leader through the dragon statue room. Touching
/// coord event 15 in Fuchin Falls Cave's dragon statue room makes it follow,
/// once it has been started there.
static void FuchinFallsCave_FollowLeaderWithLight(void) {
  if (gImilFuchinFalls_TrackLeader != 0) {
    struct FadeOverlayWork *work = *(gHeapSlots + 36);

    work->objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  }
}

/// Touching coord event 16 in the dragon statue room stops the fade overlay
/// following the leader.
static void FuchinFallsCave_StopLightFollowingLeader(void) {
  if (gImilFuchinFalls_TrackLeader != 0) {
    struct FadeOverlayWork *work = *(gHeapSlots + 36);

    work->objectEvent = NULL;
  }
}

/// Sets up the map as the party enters. Every normal entry into Imil Falls
/// Cave blocks Retreat and walks the leader toward Fever; otherwise each room
/// of Fuchin Falls Cave restores its logs, the Dragon's Eye and its lighting.
s32 ImilFuchinFalls_Initialize(void) {
  gFieldWork->transition = MAP_TRANSITION(TRANSITION_WINDOW, 4);
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0 &&
      gSaveBlock.mapId == (s32)&MapId_ImilFallsCave) {
    GameFlag_Set(FLAG_RETREAT_BLOCKED);
    Imil_WalkTowardFever();
  } else {
    ImilFuchinFalls_PrepareRoom();
  }
  return 0;
}

/// Shows Fever, in Imil Falls Cave, and walks the leader toward it.
static void Imil_WalkTowardFever(void) {
  struct ObjectEvent *objectEvent;

  FieldScript_Start();
  objectEvent = ObjectEvent_GetById(LOCALID_FEVER);
  ObjectEvent_SetSpriteFlags(objectEvent, 0);
  FieldScript_StartScreenTransition();
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(2), PIXELS(1) / 10);
  ObjectEvent_MoveToAndWait(LOCALID_PARTY_LEADER, 264, 196);
  FieldScript_End();
}

/// Sets up a room of Fuchin Falls Cave for the warp destination it is entered
/// by: its logs, the Dragon's Eye and two foreground sprites in the third
/// room, and the darkness and the fade overlay in the dragon statue's room.
static void ImilFuchinFalls_PrepareRoom(void) {
  struct ObjectEvent *objectEvent;
  /* FAKEMATCH(temporary): one variable holds the zeros and then the slot word,
   * so case 5 stores a fresh 0 and gSaveBlock stays in r6; as literal zeros
   * and the slot read in place, 51 of 144 instructions differ in tbs-en. */
  s32 value;

  ImilFuchinFalls_RebuildLogRoom();
  if (gSaveBlock.mapId == (s32)&MapId_FuchinFallsCaveDragonsEye) {
    if (!GameFlag_Get(FLAG_FUCHIN_FALLS_DRAGONS_EYE_FOUND) &&
        gSaveBlock.warpDest ==
            FUCHIN_FALLS_CAVE_DRAGONS_EYE_CHAMBER_FROM_LOGS) {
      ImilFuchinFalls_PlaceDragonsEye();
    }
    if ((u16)(gSaveBlock.warpDest - 2) <= 3) {
      FuchinFallsCave_CreateForegroundObjectEvent(PIXELS(156), 0, PIXELS(452),
                                                  SPRITE_CRATE);
      FuchinFallsCave_CreateForegroundObjectEvent(PIXELS(188), 0, PIXELS(452),
                                                  SPRITE_CRATE);
    }
  } else if (gSaveBlock.mapId == (s32)&MapId_FuchinFallsCaveDragonStatue) {
    value = 0;
    objectEvent = ObjectEvent_Get(LOCALID_BLANK_DRAGON_ROOM_CRATE);
    gImilFuchinFalls_TrackLeader = value;
    objectEvent->motionFlags = value;
    objectEvent->y.fixed = value;
    ObjectMotion_SetActionVariant(LOCALID_BLANK_DRAGON_ROOM_CRATE, 1);
    ObjectEvent_SetChildValue(LOCALID_BLANK_DRAGON_ROOM_CRATE,
                              SPRITE_COLOR_HIDDEN);
    switch (gSaveBlock.warpDest) {
    case FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_ORB_OF_FORCE_EAST:
    case FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_ORB_OF_FORCE_WEST:
      FieldEffect_StartFadeOverlay(0);
      gImilFuchinFalls_TrackLeader = 1;
      break;
    case FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_FROM_SOUTH_HALF_MIDDLE:
      FieldEffect_StartFadeOverlay(0);
      gImilFuchinFalls_TrackLeader = 1;
      value = *(s32 *)(gHeapSlots + 36);
      ((struct FadeOverlayWork *)value)->objectEvent = NULL;
      break;
    }
    if (gSaveBlock.warpDest <=
        FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_FROM_SOUTH_HALF_EAST) {
      if (GameFlag_Get(FLAG_FUCHIN_FALLS_DRAGONS_EYE_SET)) {
        MapGrid_CopyCellsTo(30, 57, 19, 57, 1, 1);
        MapGrid_CopyCellsTo(30, 8, 12, 8, 8, 7);
      } else {
        gFieldWork->transition = MAP_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
        PaletteFade_ApplySource(PALETTE_TINT_CUSTOM | RGB(8, 8, 12), 1);
        PaletteFade_ApplyTarget(PALETTE_TINT_CUSTOM | RGB(8, 8, 12), 1);
        PaletteFade_StartInterpolation(1);
        WaitFrames(1);
      }
    }
  }
}

/// Restores the metatile behavior templates for the current log room of Fuchin
/// Falls Cave.
static void ImilFuchinFalls_RebuildLogRoom(void) {
  if (gSaveBlock.mapId == (s32)&MapId_FuchinFallsCaveEntrance) {
    MapGrid_CopyCellAttributes(8, 29, 15, 5, 8, 42);
    FuchinFallsCave_RebuildEntranceLogs();
  } else if (gSaveBlock.mapId == (s32)&MapId_FuchinFallsCaveZephyr) {
    MapGrid_CopyCellAttributes(12, 8, 10, 18, 0, 28);
    FuchinFallsCave_RebuildZephyrRoomLogs();
  } else if (gSaveBlock.mapId == (s32)&MapId_FuchinFallsCaveDragonsEye &&
             gSaveBlock.warpDest !=
                 FUCHIN_FALLS_CAVE_DRAGONS_EYE_CHAMBER_FROM_LOGS) {
    MapGrid_CopyCellAttributes(12, 21, 9, 16, 12, 3);
    FuchinFallsCave_RebuildDragonsEyeRoomLogs();
  } else if (gSaveBlock.mapId == (s32)&MapId_FuchinFallsCaveArcticBlade) {
    if (gSaveBlock.warpDest ==
            FUCHIN_FALLS_CAVE_ARCTIC_BLADE_FROM_FUCHIN_FALLS_CAVE_DRAGON_STATUE ||
        gSaveBlock.warpDest ==
            FUCHIN_FALLS_CAVE_ARCTIC_BLADE_NORTH_HALF_FROM_SOUTH_HALF) {
      MapGrid_CopyCellAttributes(14, 10, 9, 8, 22, 20);
    } else {
      MapGrid_CopyCellAttributes(7, 45, 11, 4, 20, 45);
    }
    FuchinFallsCave_RebuildArcticBladeRoomLogs();
  }
}

/// Hops across a gap in Fuchin Falls Cave: west or north in the second room
/// (coord events 13, 14 and 17), south in the third (coord event 25) when its
/// first log lies at FLAG_MAPSEC_TEMP_09.
static void ImilFuchinFalls_HopOnTrigger(void) {
  s32 trigger = gFieldWork->coordEvent;

  if (gSaveBlock.mapId == (s32)&MapId_FuchinFallsCaveZephyr) {
    if (trigger == 17) {
      ImilFuchinFalls_HopBy(0, -32);
    } else {
      ImilFuchinFalls_HopBy(-32, 0);
    }
  }
  if (gSaveBlock.mapId == (s32)&MapId_FuchinFallsCaveDragonsEye &&
      trigger == 25 && GameFlag_Get(FLAG_MAPSEC_TEMP_09)) {
    ImilFuchinFalls_HopBy(0, 32);
  }
}

/// Hops the leader one gap north, in Fuchin Falls Cave's third room (coord
/// events 11 and 12).
static void FuchinFallsCave_HopNorth(void) { ImilFuchinFalls_HopBy(0, -32); }

/// Hops the leader one gap west, in Fuchin Falls Cave's third room (coord event
/// 23).
static void FuchinFallsCave_HopWest(void) { ImilFuchinFalls_HopBy(-32, 0); }

/// Jumps the leader by a pixel offset while using the hopping animations.
static void ImilFuchinFalls_HopBy(s32 dx, s32 dz) {
  FieldScript_Start();
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(5) / 2, PIXELS(5) / 4);
  ObjectMotion_OffsetPositionAndResetMotion(LOCALID_PARTY_LEADER, dx, dz);
  ObjectMotion_Launch(LOCALID_PARTY_LEADER, 4, 0);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_LEADER_LEAP);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_PARTY_LEADER);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_LEADER_SPRING);
  FieldScript_End();
}

/// Shows the Dragon's Eye in Fuchin Falls Cave's third room, as its item icon.
static void ImilFuchinFalls_PlaceDragonsEye(void) {
  struct ObjectEvent *objectEvent;
  struct Sprite *sprite;
  GlyphTransfer *glyph;
  u8 clear = 0;

  objectEvent =
      ObjectEvent_Create(SPRITE_BLANK, PIXELS(248), PIXELS(8), PIXELS(152));
  if (objectEvent != NULL) {
    sprite = objectEvent->sprite;
    sprite->flags = clear;
    sprite->count = clear;
    sprite->image.fullColor = 0;
    sprite->image.palette = 0;
    objectEvent->motionFlags = clear;
    objectEvent->retainSpriteCache = 1;
    glyph = AllocHeapSlot(17, sizeof(*glyph));
    Icon_DrawItem(ITEM_DRAGONS_EYE);
    VramCache_Load(sprite->slot, 128, glyph->tiles);
    FreeHeapSlot(17);
    gImilFuchinFalls_DragonsEye = objectEvent;
  }
}

/// Searching where the Dragon's Eye lies, in Fuchin Falls Cave's third room
/// (bg event 30), gives it to the party (FLAG_FUCHIN_FALLS_DRAGONS_EYE_FOUND).
static void FuchinFallsCave_CollectDragonsEye(void) {
  FieldScript_Start();

  if (gImilFuchinFalls_DragonsEye != NULL) {
    ItemDisplay_ShowPicture(gImilFuchinFalls_DragonsEye, 3);
  }

  PartyInventory_GiveItem(ITEM_DRAGONS_EYE, 0);
  GameFlag_Set(FLAG_FUCHIN_FALLS_DRAGONS_EYE_FOUND);

  if (gImilFuchinFalls_DragonsEye != NULL) {
    ObjectEvent_Release(gImilFuchinFalls_DragonsEye);
  }

  FieldScript_End();
}

/// Applies the dust velocity and scale change for one frame.
static void FuchinFallsCave_UpdateDustMotion(union FieldObject *raw) {
  struct FieldEffect *object = &raw->effect;

  object->x += object->velocity.x;
  object->y += object->velocity.y;
  object->z += object->velocity.z;
  object->scaleX += object->scaleRateX;
  object->scaleY += object->scaleRateY;
}

/// Hides the dust (SPRITE_COLOR_HIDDEN) when its object script finishes
/// looping.
static s32
FuchinFallsCave_SetDustPalette(struct ObjectScriptContext *interpreter) {
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)interpreter;
  ObjectGroup_SetChildValue(objectEvent, SPRITE_COLOR_HIDDEN);
  return 0;
}

/// Creates scripted cave dust with the requested velocity, palette and scale.
void ImilFuchinFalls_SpawnDust(s32 x, s32 y, s32 z, s32 velocityX,
                               s32 velocityY, s32 velocityZ, u32 flags,
                               const struct EffectOptions *extra) {
  struct FieldEffect *effect;
  struct Sprite *sprite;
  const s32 *script;
  const s32 *scripts[3] = {
      gImilFuchinFalls_ShortDustScript,
      gImilFuchinFalls_MediumDustScript,
      gImilFuchinFalls_LongDustScript,
  };

  effect = (struct FieldEffect *)Object_CreateField(SPRITE_DUST_PUFF, x, y, z);
  if (effect == 0)
    return;
  sprite = effect->sprite;
  ObjectDispatch_SelectAnimation((struct ObjectEvent *)effect,
                                 (flags + 1) & EFFECT_SCRIPT_MASK);
  ObjectEvent_SetScript((struct ObjectEvent *)effect,
                        scripts[flags & EFFECT_SCRIPT_MASK]);
  effect->motionFlags = 0;
  sprite->flags = 0;
  effect->update = FuchinFallsCave_UpdateDustMotion;
  effect->velocity.x = velocityX;
  effect->velocity.y = velocityY;
  effect->velocity.z = velocityZ;
  effect->scaleRateX = 0;
  effect->scaleRateY = 0;
  sprite->image.priority = 1;
  if ((flags & 0xffff0000) == 0 || extra == 0)
    return;
  if (flags & EFFECT_USE_PALETTE)
    ObjectGroup_SetChildValue((struct ObjectEvent *)effect, extra->palette);
  if (flags & EFFECT_USE_PRIORITY) {
    effect->priorityFlags &= ~OBJ_EVENT_PRIORITY_AUTOMATIC;
    sprite->image.priority = extra->priority;
  }
  if (flags & EFFECT_USE_START_SCALE) {
    effect->scaleX = extra->startScaleX;
    effect->scaleY = extra->startScaleY;
  }
  if (flags & EFFECT_SCALE_TO_TARGET) {
    script = scripts[flags & EFFECT_SCRIPT_MASK];
    if (flags & EFFECT_USE_START_SCALE) {
      effect->scaleRateX = (extra->targetScaleX - effect->scaleX) / script[3];
      effect->scaleRateY = (extra->targetScaleY - effect->scaleY) / script[3];
    } else {
      effect->scaleRateX = (extra->targetScaleX - Q16_ONE) / script[3];
      effect->scaleRateY = (extra->targetScaleY - Q16_ONE) / script[3];
    }
  }
}

/// Emits cave-mouth dust every fourth frame while the dragon awakens.
static void ImilFuchinFalls_BlowCaveMouthDust(void) {
  struct CaveDustOptions options;
  s32 framePhase;
  s32 velocityX;
  s32 velocityY;

  framePhase = gFrameCount & 3;
  if (framePhase != 0)
    return;
  options.palette = 10;
  options.startScaleX = Q16_ONE / 2;
  options.startScaleY = Q16_ONE / 2;
  options.targetScaleX = 9 * Q16_ONE / 5;
  options.targetScaleY = 9 * Q16_ONE / 5;
  if ((gFrameCount & 7) == 0)
    Sound_PlaySong(SE_136);
  velocityX = -PIXELS(1) - ((((u32)Engine_RandomNext() << 1) >> 16) << 16);
  velocityY = -((((u32)Engine_RandomNext() * 3) >> 16) * (PIXELS(1) / 5));
  ImilFuchinFalls_SpawnDust(
      PIXELS(308), PIXELS(64), PIXELS(222), velocityX, velocityY, framePhase,
      1 | EFFECT_USE_PALETTE | EFFECT_USE_START_SCALE | EFFECT_SCALE_TO_TARGET,
      (const struct EffectOptions *)&options);
}

/// Using the Dragon's Eye on the dragon statue in Fuchin Falls Cave (trigger
/// 20), until it is set: inserts the eye, lights the room and reveals its
/// hidden path.
static void FuchinFallsCave_LightDragonWithEye(void) {
  u32 pass;

  // Flash the room as the eye opens the dragon's path.
  Sound_PlaySong(MUS_SILENCE);
  Sound_PlaySong(SE_GEM_SET);
  FieldScript_Start();
  Field_EndBusyAndReveal();

  for (pass = 0; pass < 4; pass++) {
    PaletteFade_SetTarget(PALETTE_TINT_CUSTOM | RGB(24, 24, 16), 1);
    PaletteFade_StartInterpolation(1);
    WaitFrames(2);
    if (pass == 0) {
      MapGrid_CopyCellsTo(30, 8, 12, 8, 8, 7);
      MapGrid_CopyCellsTo(30, 57, 19, 57, 1, 1);
    }
    PaletteFade_SetTarget(PALETTE_TINT_CUSTOM | RGB(8, 8, 12), 1);
    PaletteFade_StartInterpolation(1);
    WaitFrames(2);
  }

  WaitFrames(30);

  // Blow dust from the cave mouth while the room brightens.
  FieldScheduler_AddCallback(ImilFuchinFalls_BlowCaveMouthDust,
                             TASK_PRIORITY_MAP);
  WaitFrames(40);
  PaletteFade_SetTarget(PALETTE_TINT_CUSTOM | RGB(16, 4, 4), 1);
  PaletteFade_StartInterpolation(40);
  WaitFrames(80);
  Task_Remove(ImilFuchinFalls_BlowCaveMouthDust);
  WaitFrames(20);

  // Settle the lighting, consume the eye and remember the revealed path.
  PaletteFade_SetTarget(PALETTE_TINT_NONE, 1);
  PaletteFade_StartInterpolation(80);

  WaitFrames(80);

  GameFlag_Set(FLAG_FUCHIN_FALLS_DRAGONS_EYE_SET);
  PartyInventory_Discard(ITEM_DRAGONS_EYE);
  Map_PlayMusic();

  FieldScript_End();
}

/// Searching the inscription in Fuchin Falls Cave's first room (bg event 1):
/// `The dragon's flame illuminates the path of truth in darkness.`
static void FuchinFallsCave_ReadDragonFlameInscription(void) {
  FieldScript_Start();
  FieldMessage_Show(MsgFuchinDragonFlameShowsPath, 1);
  FieldScript_End();
}

/// Searching the inscription in Fuchin Falls Cave's Ki room (bg event 7): `The
/// secret of Ki shall be revealed to the disciples of truth.`
static void FuchinFallsCave_ReadKiInscription(void) {
  FieldScript_Start();
  FieldMessage_Show(MsgFuchinSecretKiRevealed, 1);
  FieldScript_End();
}

/// Searching the inscription in Fuchin Falls Cave's dragon statue room
/// (bg event 19): `Rays of light give birth to shadows, revealing the way.`
static void FuchinFallsCave_ReadShadowInscription(void) {
  FieldScript_Start();
  FieldMessage_Show(MsgFuchinLightRevealsShadows, 1);
  FieldScript_End();
}

/// Searching the dragon statue in Fuchin Falls Cave (bg event 20): its burning
/// red eyes once set, else its empty sockets; with the Dragon's Eye carried
/// it then opens the main menu as the search ends, so that the eye can be used.
static void FuchinFallsCave_SearchDragonStatue(void) {
  FieldScript_Start();

  if (GameFlag_Get(FLAG_FUCHIN_FALLS_DRAGONS_EYE_SET) != 0) {
    FieldMessage_Show(MsgFuchinDragonRedEyes, 1);
  } else {
    FieldMessage_Show(MsgFuchinEyelessDragon, 1);
    if (PartyInventory_FindMember(ITEM_DRAGONS_EYE) != -1) {
      gFieldWork->menu = 1;
    }
  }
  FieldScript_End();
}

/// The dragon statue room goes dark again when Reveal ends. That happens in
/// Fuchin Falls Cave only before the eye is set.
static void FuchinFallsCave_EndRevealLighting(void) {
  struct FieldEffectWork *effectWork;
  struct ScreenTransitionState *state;

  if (gSaveBlock.warpDest <=
      FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_FROM_SOUTH_HALF_EAST) {
    effectWork = *(gHeapSlots + 56);
    state = *(gHeapSlots + 31);
    effectWork->childOption = 1;
    state->nextMode = 0;
    state->duration = 1;
    state->step = 1;
    PaletteFade_SetSource(0, 1);
    PaletteFade_SetTarget(PALETTE_TINT_CUSTOM | RGB(8, 8, 12), 1);
    PaletteFade_StartInterpolation(16);
    WaitFrames(16);
  }
}

/// Turns the object event sliding on Imil Falls Cave's ice gradually toward the
/// held direction.
static void Imil_TurnSkaterTowardHeldDirection(union FieldObject *fieldObject) {
  struct ObjectEvent *objectEvent = &fieldObject->objectEvent;
  s32 timer;
  s32 direction;
  u16 facing;

  timer = objectEvent->timer;
  if (timer != 0) {
    objectEvent->timer--;
    return;
  }
  objectEvent->steeringFlags = 0;
  direction = gImilFuchinFalls_KeyHeadings[(gKeysHeld >> 4) & 15];
  if (direction == -1) {
    ObjectDispatch_SelectAnimation(objectEvent, 9);
    return;
  }
  facing = objectEvent->facing;
  direction = (s16)(direction - facing);
  if (direction > 0x1000)
    direction = 0x1000;
  if (direction < -0x1000)
    direction = -0x1000;
  objectEvent->facing = facing + direction;
  ObjectDispatch_SelectAnimation(objectEvent, 2);
  ObjectDispatch_SetAnimationSpeed((struct DispatchObject *)objectEvent,
                                   IMIL_SLIDE_ANIMATION_STEP);
}

/// Slides along Imil Falls Cave's ice until terrain or a metatile behavior
/// boundary ends the move.
static void Imil_SlideObjectEventAcrossIce(void) {
  struct ObjectEvent *objectEvent;
  struct FieldPosition probe;
  s32 heading;
  s32 startBehavior;
  s32 nextBehavior;
  s32 z;
  s32 x;

  objectEvent = ObjectEvent_GetByLocalId(gSaveBlock.leaderLocalId);

  for (;;) {
    heading = gImilFuchinFalls_SlideHeadings[(gKeysHeld >> 4) & 15];
    if (((u32)heading << 16) == 0xffff0000) {
      return;
    }

    FieldScript_Start();

    // Start at the cell center and reject a blocked or raised next cell.
    probe.x = (objectEvent->x.fixed & 0xfff00000) + PIXELS(8);
    probe.y = objectEvent->y.fixed;
    probe.z = (objectEvent->z.fixed & 0xfff00000) + PIXELS(8);
    z = probe.z;
    x = probe.x;
    startBehavior = MapGrid_GetMetatileBehaviorAt(objectEvent->layer, x, z);
    FieldVector_AddPolarOffset(PIXELS(16), heading, &probe);

    nextBehavior =
        MapGrid_GetMetatileBehaviorAt(objectEvent->layer, probe.x, probe.z);
    if (nextBehavior == MB_WALL ||
        MapGrid_GetTerrainHeight(objectEvent->layer, probe.x, probe.z) -
                objectEvent->y.fixed >
            PIXELS(8)) {
      objectEvent->facing = heading;
      goto tail;
    }

    probe.x = x;
    probe.z = z;
    objectEvent->speed = PIXELS(2);
    objectEvent->acceleration = PIXELS(1) / 10;
    objectEvent->timer = 0;
    Object_SetPosition(objectEvent, x, objectEvent->y.fixed, z);

    ObjectDispatch_SelectAnimation(objectEvent, 2);
    ObjectDispatch_SetAnimationSpeed((struct DispatchObject *)objectEvent,
                                     IMIL_SLIDE_ANIMATION_STEP);
    Object_WaitForMove(objectEvent);
    objectEvent->update = Imil_TurnSkaterTowardHeldDirection;

    // Keep sliding through cells with the same metatile behavior.
    for (;;) {
      FieldVector_AddPolarOffset(PIXELS(16), heading, &probe);
      nextBehavior =
          MapGrid_GetMetatileBehaviorAt(objectEvent->layer, probe.x, probe.z);
      if (nextBehavior == MB_WALL) {
        break;
      }
      if (MapGrid_GetTerrainHeight(objectEvent->layer, probe.x, probe.z) -
              objectEvent->y.fixed >
          PIXELS(8)) {
        break;
      }
      x = probe.x;
      z = probe.z;
      objectEvent->speed = PIXELS(2);
      objectEvent->acceleration = PIXELS(1) / 10;
      Object_SetPosition(objectEvent, probe.x, probe.y, probe.z);
      Object_WaitForMove(objectEvent);
      if (nextBehavior != startBehavior) {
        goto blocked;
      }
    }

    objectEvent->speed = PIXELS(2);
    objectEvent->acceleration = PIXELS(1);
    Object_SetPosition(objectEvent, x, objectEvent->y.fixed, z);
    Object_WaitForMove(objectEvent);
    WaitFrames(2);
  }

blocked:
  // A metatile behavior change ends the slide and restores normal steering.
  objectEvent->update = NULL;
  objectEvent->steeringFlags |= OBJ_EVENT_FACES_MOTION;

  objectEvent->acceleration = PIXELS(1) / 4;

tail:
  WaitFrames(10);
  FieldScript_End();
}

// clang-format off
s16 gImilFuchinFalls_KeyHeadings[] = {
    0xffff, FACING_EAST, FACING_WEST, 0xffff, FACING_NORTH, FACING_NORTHEAST, FACING_NORTHWEST, FACING_NORTH,
    FACING_SOUTH, FACING_SOUTHEAST, FACING_SOUTHWEST, FACING_SOUTH, 0xffff, FACING_EAST, FACING_WEST, 0xffff,
    0xffff, FACING_EAST, FACING_WEST, 0xffff, FACING_NORTH, 0xffff, 0xffff, FACING_NORTH,
    FACING_SOUTH, 0xffff, 0xffff, FACING_SOUTH, 0xffff, FACING_EAST, FACING_WEST, 0xffff,
};

s16 gImilFuchinFalls_SlideHeadings[] = {
    0xffff, FACING_EAST, FACING_WEST, 0xffff, FACING_NORTH, FACING_EAST, FACING_WEST, FACING_NORTH,
    FACING_SOUTH, FACING_EAST, FACING_WEST, FACING_SOUTH, 0xffff, FACING_EAST, FACING_WEST, 0xffff,
    FACING_EAST, FACING_EAST,
};

struct ObjectEvent *gImilFuchinFalls_DragonsEye = NULL;

struct MapWarpDest gImilFuchinFalls_NoWarpDests[] = {
    { 0, CONDITION_ALWAYS, 256, 0, 600, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { MAP_TABLE_END },
};

struct MapWarpDest gImilFuchinFalls_ImilFallsCaveWarpDests[] = {
    { 0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { IMIL_FALLS_CAVE_FROM_IMIL, CONDITION_ALWAYS, 264, 0, 289, FACING_NORTH, 0, 135, 62, 376, 328, 0 },
    { MAP_TABLE_END },
};

struct MapWarpDest gImilFuchinFalls_EntranceWarpDests[] = {
    { 0, CONDITION_ALWAYS, 248, 0, 600, FACING_SOUTH, 0, 64, 64, 416, 656, 0 },
    { FUCHIN_FALLS_CAVE_ENTRANCE_FROM_FUCHIN_TEMPLE_GROUNDS, CONDITION_ALWAYS, 248, 0, 632, FACING_NORTH, 0, 64, 368, 416, 656, 0 },
    { FUCHIN_FALLS_CAVE_ENTRANCE_FROM_FUCHIN_FALLS_CAVE_ZEPHYR, CONDITION_ALWAYS, 376, 0, 488, FACING_SOUTH, 0, 64, 368, 416, 656, 0 },
    { FUCHIN_FALLS_CAVE_ENTRANCE_FROM_FUCHIN_FALLS_CAVE_ARCTIC_BLADE, CONDITION_ALWAYS, 120, 0, 488, FACING_SOUTH, 0, 64, 368, 416, 656, 0 },
    { FUCHIN_FALLS_CAVE_ENTRANCE_SOUTH_HALF_FROM_NORTH_HALF, CONDITION_ALWAYS, 232, 0, 408, FACING_SOUTH, 0, 64, 368, 416, 656, 0 },
    { FUCHIN_FALLS_CAVE_ENTRANCE_NORTH_HALF_FROM_SOUTH_HALF, CONDITION_ALWAYS, 232, 0, 344, FACING_NORTH, 0, 64, 48, 416, 376, 0 },
    { FUCHIN_FALLS_CAVE_ENTRANCE_FROM_FUCHIN_FALLS_CAVE_DRAGONS_EYE, CONDITION_ALWAYS, 376, 0, 152, FACING_NORTH, 0, 64, 48, 416, 376, 0 },
    { FUCHIN_FALLS_CAVE_ENTRANCE_FROM_FUCHIN_FALLS_CAVE_DRAGON_STATUE_WEST, CONDITION_ALWAYS, 120, 0, 200, FACING_SOUTH, 0, 64, 48, 416, 376, 0 },
    { FUCHIN_FALLS_CAVE_ENTRANCE_FROM_FUCHIN_FALLS_CAVE_DRAGON_STATUE_EAST, CONDITION_ALWAYS, 248, 0, 104, FACING_SOUTH, 0, 64, 48, 416, 376, 0 },
    { MAP_TABLE_END },
};

struct MapWarpDest gImilFuchinFalls_ZephyrRoomWarpDests[] = {
    { FUCHIN_FALLS_CAVE_ZEPHYR_FROM_FUCHIN_FALLS_CAVE_DRAGONS_EYE, CONDITION_ALWAYS, 248, 0, 120, FACING_SOUTH, 0, 96, 64, 416, 480, 0 },
    { FUCHIN_FALLS_CAVE_ZEPHYR_FROM_FUCHIN_FALLS_CAVE_ENTRANCE, CONDITION_ALWAYS, 152, 0, 200, FACING_NORTH, 0, 96, 64, 416, 480, 0 },
    { MAP_TABLE_END },
};

struct MapWarpDest gImilFuchinFalls_DragonsEyeRoomWarpDests[] = {
    { FUCHIN_FALLS_CAVE_DRAGONS_EYE_CHAMBER_FROM_LOGS, CONDITION_ALWAYS, 248, 0, 104, FACING_SOUTH, 0, 128, 64, 368, 256, 0 },
    { FUCHIN_FALLS_CAVE_DRAGONS_EYE_FROM_FUCHIN_FALLS_CAVE_DRAGON_STATUE, CONDITION_ALWAYS, 184, 0, 328, FACING_SOUTH, 0, 96, 256, 416, 656, 0 },
    { FUCHIN_FALLS_CAVE_DRAGONS_EYE_LOGS_FROM_CHAMBER, CONDITION_ALWAYS, 152, 0, 432, FACING_SOUTH, 0, 96, 256, 416, 656, 0 },
    { FUCHIN_FALLS_CAVE_DRAGONS_EYE_FROM_FUCHIN_FALLS_CAVE_ENTRANCE, CONDITION_ALWAYS, 184, 0, 552, FACING_SOUTH, 0, 96, 256, 416, 656, 0 },
    { FUCHIN_FALLS_CAVE_DRAGONS_EYE_FROM_FUCHIN_FALLS_CAVE_ZEPHYR, CONDITION_ALWAYS, 280, 0, 616, FACING_NORTH, 0, 96, 256, 416, 656, 0 },
    { MAP_TABLE_END },
};

struct MapWarpDest gImilFuchinFalls_ArcticBladeRoomWarpDests[] = {
    { FUCHIN_FALLS_CAVE_ARCTIC_BLADE_FROM_FUCHIN_FALLS_CAVE_DRAGON_STATUE, CONDITION_ALWAYS, 328, 0, 72, FACING_SOUTH, 0, 64, 16, 416, 416, 0 },
    { FUCHIN_FALLS_CAVE_ARCTIC_BLADE_NORTH_HALF_FROM_SOUTH_HALF, CONDITION_ALWAYS, 248, 0, 392, FACING_NORTH, 0, 64, 16, 416, 416, 0 },
    { FUCHIN_FALLS_CAVE_ARCTIC_BLADE_SOUTH_HALF_FROM_NORTH_HALF, CONDITION_ALWAYS, 248, 0, 472, FACING_SOUTH, 0, 64, 416, 416, 800, 0 },
    { FUCHIN_FALLS_CAVE_ARCTIC_BLADE_FROM_FUCHIN_FALLS_CAVE_ENTRANCE, CONDITION_ALWAYS, 344, 0, 632, FACING_NORTH, 0, 64, 416, 416, 800, 0 },
    { MAP_TABLE_END },
};

struct MapWarpDest gImilFuchinFalls_KiRoomWarpDests[] = {
    { FUCHIN_FALLS_CAVE_ORB_OF_FORCE_NORTH_HALF_FROM_SOUTH_HALF, CONDITION_ALWAYS, 264, 0, 232, FACING_NORTH, 0, 128, 16, 384, 256, 0 },
    { FUCHIN_FALLS_CAVE_ORB_OF_FORCE_SOUTH_HALF_FROM_NORTH_HALF, CONDITION_ALWAYS, 248, 0, 472, FACING_SOUTH, 0, 64, 256, 416, 720, 0 },
    { FUCHIN_FALLS_CAVE_ORB_OF_FORCE_FROM_FUCHIN_FALLS_CAVE_DRAGON_STATUE_WEST, CONDITION_ALWAYS, 216, 0, 664, FACING_NORTH, 0, 64, 256, 416, 720, 0 },
    { FUCHIN_FALLS_CAVE_ORB_OF_FORCE_FROM_FUCHIN_FALLS_CAVE_DRAGON_STATUE_EAST, CONDITION_ALWAYS, 280, 0, 664, FACING_NORTH, 0, 64, 256, 416, 720, 0 },
    { MAP_TABLE_END },
};

struct MapWarpDest gImilFuchinFalls_DragonRoomWarpDests[] = {
    { FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_ORB_OF_FORCE_EAST, CONDITION_ALWAYS, 280, 0, 120, FACING_SOUTH, 0, 64, 0, 416, 344, 0 },
    { FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_ORB_OF_FORCE_WEST, CONDITION_ALWAYS, 216, 0, 120, FACING_SOUTH, 0, 64, 0, 416, 344, 0 },
    { FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_ARCTIC_BLADE, CONDITION_ALWAYS, 104, 0, 280, FACING_SOUTH, 0, 64, 0, 416, 344, 0 },
    { FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_FROM_SOUTH_HALF_WEST, CONDITION_ALWAYS, 200, 0, 344, FACING_NORTH, 0, 64, 16, 416, 344, 0 },
    { FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_FROM_SOUTH_HALF_MIDDLE, CONDITION_ALWAYS, 248, 0, 312, FACING_NORTH, 0, 64, 16, 416, 344, 0 },
    { FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_FROM_SOUTH_HALF_EAST, CONDITION_ALWAYS, 296, 0, 344, FACING_NORTH, 0, 64, 16, 416, 344, 0 },
    { FUCHIN_FALLS_CAVE_DRAGON_STATUE_SOUTH_HALF_FROM_NORTH_HALF_WEST, CONDITION_ALWAYS, 200, 0, 440, FACING_SOUTH, 0, 32, 344, 432, 736, 0 },
    { FUCHIN_FALLS_CAVE_DRAGON_STATUE_SOUTH_HALF_FROM_NORTH_HALF_MIDDLE, CONDITION_ALWAYS, 248, 0, 440, FACING_SOUTH, 0, 32, 344, 432, 736, 0 },
    { FUCHIN_FALLS_CAVE_DRAGON_STATUE_SOUTH_HALF_FROM_NORTH_HALF_EAST, CONDITION_ALWAYS, 296, 0, 424, FACING_SOUTH, 0, 32, 344, 432, 736, 0 },
    { FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_DRAGONS_EYE, CONDITION_ALWAYS, 376, 0, 472, FACING_SOUTH, 0, 32, 344, 432, 736, 0 },
    { FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_ENTRANCE_EAST, CONDITION_ALWAYS, 248, 0, 536, FACING_NORTH, 0, 32, 344, 432, 736, 0 },
    { FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_ENTRANCE_WEST, CONDITION_ALWAYS, 120, 0, 696, FACING_SOUTH, 0, 32, 344, 432, 752, 0 },
    { MAP_TABLE_END },
};

u32 gImilFuchinFalls_Warps[] = {
    MAP_WARPS(ImilFallsCave),
    MAP_WARP(IMIL_FALLS_CAVE_TO_IMIL, Imil, IMIL_FROM_IMIL_FALLS_CAVE),
    MAP_WARPS(FuchinFallsCaveEntrance),
    MAP_WARP(FUCHIN_FALLS_CAVE_ENTRANCE_TO_FUCHIN_TEMPLE_GROUNDS, FuchinTempleGrounds, FUCHIN_TEMPLE_GROUNDS_FROM_FUCHIN_FALLS_CAVE_ENTRANCE),
    MAP_WARP(FUCHIN_FALLS_CAVE_ENTRANCE_TO_FUCHIN_FALLS_CAVE_ZEPHYR, FuchinFallsCaveZephyr, FUCHIN_FALLS_CAVE_ZEPHYR_FROM_FUCHIN_FALLS_CAVE_ENTRANCE),
    MAP_WARP(FUCHIN_FALLS_CAVE_ENTRANCE_TO_FUCHIN_FALLS_CAVE_ARCTIC_BLADE, FuchinFallsCaveArcticBlade, FUCHIN_FALLS_CAVE_ARCTIC_BLADE_FROM_FUCHIN_FALLS_CAVE_ENTRANCE),
    MAP_WARP(FUCHIN_FALLS_CAVE_ENTRANCE_SOUTH_HALF_TO_NORTH_HALF, FuchinFallsCaveEntrance, FUCHIN_FALLS_CAVE_ENTRANCE_NORTH_HALF_FROM_SOUTH_HALF),
    MAP_WARP(FUCHIN_FALLS_CAVE_ENTRANCE_NORTH_HALF_TO_SOUTH_HALF, FuchinFallsCaveEntrance, FUCHIN_FALLS_CAVE_ENTRANCE_SOUTH_HALF_FROM_NORTH_HALF),
    MAP_WARP(FUCHIN_FALLS_CAVE_ENTRANCE_TO_FUCHIN_FALLS_CAVE_DRAGONS_EYE, FuchinFallsCaveDragonsEye, FUCHIN_FALLS_CAVE_DRAGONS_EYE_FROM_FUCHIN_FALLS_CAVE_ENTRANCE),
    MAP_WARP(FUCHIN_FALLS_CAVE_ENTRANCE_TO_FUCHIN_FALLS_CAVE_DRAGON_STATUE_WEST, FuchinFallsCaveDragonStatue, FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_ENTRANCE_WEST),
    MAP_WARP(FUCHIN_FALLS_CAVE_ENTRANCE_TO_FUCHIN_FALLS_CAVE_DRAGON_STATUE_EAST, FuchinFallsCaveDragonStatue, FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_ENTRANCE_EAST),
    MAP_WARPS(FuchinFallsCaveZephyr),
    MAP_WARP(FUCHIN_FALLS_CAVE_ZEPHYR_TO_FUCHIN_FALLS_CAVE_DRAGONS_EYE, FuchinFallsCaveDragonsEye, FUCHIN_FALLS_CAVE_DRAGONS_EYE_FROM_FUCHIN_FALLS_CAVE_ZEPHYR),
    MAP_WARP(FUCHIN_FALLS_CAVE_ZEPHYR_TO_FUCHIN_FALLS_CAVE_ENTRANCE, FuchinFallsCaveEntrance, FUCHIN_FALLS_CAVE_ENTRANCE_FROM_FUCHIN_FALLS_CAVE_ZEPHYR),
    MAP_WARPS(FuchinFallsCaveDragonsEye),
    MAP_WARP(FUCHIN_FALLS_CAVE_DRAGONS_EYE_CHAMBER_TO_LOGS, FuchinFallsCaveDragonsEye, FUCHIN_FALLS_CAVE_DRAGONS_EYE_LOGS_FROM_CHAMBER),
    MAP_WARP(FUCHIN_FALLS_CAVE_DRAGONS_EYE_TO_FUCHIN_FALLS_CAVE_DRAGON_STATUE, FuchinFallsCaveDragonStatue, FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_DRAGONS_EYE),
    MAP_WARP(FUCHIN_FALLS_CAVE_DRAGONS_EYE_LOGS_TO_CHAMBER, FuchinFallsCaveDragonsEye, FUCHIN_FALLS_CAVE_DRAGONS_EYE_CHAMBER_FROM_LOGS),
    MAP_WARP(FUCHIN_FALLS_CAVE_DRAGONS_EYE_TO_FUCHIN_FALLS_CAVE_ENTRANCE, FuchinFallsCaveEntrance, FUCHIN_FALLS_CAVE_ENTRANCE_FROM_FUCHIN_FALLS_CAVE_DRAGONS_EYE),
    MAP_WARP(FUCHIN_FALLS_CAVE_DRAGONS_EYE_TO_FUCHIN_FALLS_CAVE_ZEPHYR, FuchinFallsCaveZephyr, FUCHIN_FALLS_CAVE_ZEPHYR_FROM_FUCHIN_FALLS_CAVE_DRAGONS_EYE),
    MAP_WARPS(FuchinFallsCaveArcticBlade),
    MAP_WARP(FUCHIN_FALLS_CAVE_ARCTIC_BLADE_TO_FUCHIN_FALLS_CAVE_DRAGON_STATUE, FuchinFallsCaveDragonStatue, FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_ARCTIC_BLADE),
    MAP_WARP(FUCHIN_FALLS_CAVE_ARCTIC_BLADE_NORTH_HALF_TO_SOUTH_HALF, FuchinFallsCaveArcticBlade, FUCHIN_FALLS_CAVE_ARCTIC_BLADE_SOUTH_HALF_FROM_NORTH_HALF),
    MAP_WARP(FUCHIN_FALLS_CAVE_ARCTIC_BLADE_SOUTH_HALF_TO_NORTH_HALF, FuchinFallsCaveArcticBlade, FUCHIN_FALLS_CAVE_ARCTIC_BLADE_NORTH_HALF_FROM_SOUTH_HALF),
    MAP_WARP(FUCHIN_FALLS_CAVE_ARCTIC_BLADE_TO_FUCHIN_FALLS_CAVE_ENTRANCE, FuchinFallsCaveEntrance, FUCHIN_FALLS_CAVE_ENTRANCE_FROM_FUCHIN_FALLS_CAVE_ARCTIC_BLADE),
    MAP_WARPS(FuchinFallsCaveOrbOfForce),
    MAP_WARP(FUCHIN_FALLS_CAVE_ORB_OF_FORCE_NORTH_HALF_TO_SOUTH_HALF, FuchinFallsCaveOrbOfForce, FUCHIN_FALLS_CAVE_ORB_OF_FORCE_SOUTH_HALF_FROM_NORTH_HALF),
    MAP_WARP(FUCHIN_FALLS_CAVE_ORB_OF_FORCE_SOUTH_HALF_TO_NORTH_HALF, FuchinFallsCaveOrbOfForce, FUCHIN_FALLS_CAVE_ORB_OF_FORCE_NORTH_HALF_FROM_SOUTH_HALF),
    MAP_WARP(FUCHIN_FALLS_CAVE_ORB_OF_FORCE_TO_FUCHIN_FALLS_CAVE_DRAGON_STATUE_WEST, FuchinFallsCaveDragonStatue, FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_ORB_OF_FORCE_WEST),
    MAP_WARP(FUCHIN_FALLS_CAVE_ORB_OF_FORCE_TO_FUCHIN_FALLS_CAVE_DRAGON_STATUE_EAST, FuchinFallsCaveDragonStatue, FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_ORB_OF_FORCE_EAST),
    MAP_WARPS(FuchinFallsCaveDragonStatue),
    MAP_WARP(FUCHIN_FALLS_CAVE_DRAGON_STATUE_TO_FUCHIN_FALLS_CAVE_ORB_OF_FORCE_WEST, FuchinFallsCaveOrbOfForce, FUCHIN_FALLS_CAVE_ORB_OF_FORCE_FROM_FUCHIN_FALLS_CAVE_DRAGON_STATUE_WEST),
    MAP_WARP(FUCHIN_FALLS_CAVE_DRAGON_STATUE_TO_FUCHIN_FALLS_CAVE_ORB_OF_FORCE_EAST, FuchinFallsCaveOrbOfForce, FUCHIN_FALLS_CAVE_ORB_OF_FORCE_FROM_FUCHIN_FALLS_CAVE_DRAGON_STATUE_EAST),
    MAP_WARP(FUCHIN_FALLS_CAVE_DRAGON_STATUE_TO_FUCHIN_FALLS_CAVE_ARCTIC_BLADE, FuchinFallsCaveArcticBlade, FUCHIN_FALLS_CAVE_ARCTIC_BLADE_FROM_FUCHIN_FALLS_CAVE_DRAGON_STATUE),
    MAP_WARP(FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_TO_SOUTH_HALF_WEST, FuchinFallsCaveDragonStatue, FUCHIN_FALLS_CAVE_DRAGON_STATUE_SOUTH_HALF_FROM_NORTH_HALF_WEST),
    MAP_WARP(FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_TO_SOUTH_HALF_MIDDLE, FuchinFallsCaveDragonStatue, FUCHIN_FALLS_CAVE_DRAGON_STATUE_SOUTH_HALF_FROM_NORTH_HALF_MIDDLE),
    MAP_WARP(FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_TO_SOUTH_HALF_EAST, FuchinFallsCaveDragonStatue, FUCHIN_FALLS_CAVE_DRAGON_STATUE_SOUTH_HALF_FROM_NORTH_HALF_EAST),
    MAP_WARP(FUCHIN_FALLS_CAVE_DRAGON_STATUE_SOUTH_HALF_TO_NORTH_HALF_WEST, FuchinFallsCaveDragonStatue, FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_FROM_SOUTH_HALF_WEST),
    MAP_WARP(FUCHIN_FALLS_CAVE_DRAGON_STATUE_SOUTH_HALF_TO_NORTH_HALF_MIDDLE, FuchinFallsCaveDragonStatue, FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_FROM_SOUTH_HALF_MIDDLE),
    MAP_WARP(FUCHIN_FALLS_CAVE_DRAGON_STATUE_SOUTH_HALF_TO_NORTH_HALF_EAST, FuchinFallsCaveDragonStatue, FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_FROM_SOUTH_HALF_EAST),
    MAP_WARP(FUCHIN_FALLS_CAVE_DRAGON_STATUE_TO_FUCHIN_FALLS_CAVE_DRAGONS_EYE, FuchinFallsCaveDragonsEye, FUCHIN_FALLS_CAVE_DRAGONS_EYE_FROM_FUCHIN_FALLS_CAVE_DRAGON_STATUE),
    MAP_WARP(FUCHIN_FALLS_CAVE_DRAGON_STATUE_TO_FUCHIN_FALLS_CAVE_ENTRANCE_EAST, FuchinFallsCaveEntrance, FUCHIN_FALLS_CAVE_ENTRANCE_FROM_FUCHIN_FALLS_CAVE_DRAGON_STATUE_EAST),
    MAP_WARP(FUCHIN_FALLS_CAVE_DRAGON_STATUE_TO_FUCHIN_FALLS_CAVE_ENTRANCE_WEST, FuchinFallsCaveEntrance, FUCHIN_FALLS_CAVE_ENTRANCE_FROM_FUCHIN_FALLS_CAVE_DRAGON_STATUE_WEST),
    MAP_WARPS_END,
};

struct ObjectEventTemplate gImilFuchinFalls_NoObjectEventTemplates[] = {
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gImilFuchinFalls_ImilFallsCaveObjectEventTemplates[] = {
    { MARS_DJINNI, FLAG_DJINNI(DJINNI_FEVER), MOVEMENT_TYPE_STAND,
        PIXELS(263), 0, PIXELS(204), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gImilFuchinFalls_EntranceObjectEventTemplates[] = {
    { SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(136), 0, PIXELS(504), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_FLOATING_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(200), 0, PIXELS(536), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_FLOATING_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(296), 0, PIXELS(536), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gImilFuchinFalls_ZephyrRoomObjectEventTemplates[] = {
    { SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(216), 0, PIXELS(328), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_FLOATING_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(248), 0, PIXELS(280), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_FLOATING_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(312), 0, PIXELS(408), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { JUPITER_DJINNI, FLAG_DJINNI(DJINNI_ZEPHYR), MOVEMENT_TYPE_STAND,
        PIXELS(344), 0, PIXELS(280), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gImilFuchinFalls_DragonsEyeRoomObjectEventTemplates[] = {
    { SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(328), 0, PIXELS(408), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(248), 0, PIXELS(568), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_FLOATING_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(248), 0, PIXELS(488), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_FLOATING_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(248), 0, PIXELS(504), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gImilFuchinFalls_ArcticBladeRoomObjectEventTemplates[] = {
    { SPRITE_FLOATING_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(328), 0, PIXELS(168), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(360), 0, PIXELS(264), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(120), 0, PIXELS(744), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gImilFuchinFalls_DragonRoomObjectEventTemplates[] = {
    { SPRITE_CRATE, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(312), 0, PIXELS(128), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

struct MapEvent gImilFuchinFalls_NoEvents[] = {
    { MAP_EVENTS_END },
};

struct MapEvent gImilFuchinFalls_ImilFallsCaveEvents[] = {
    { MAP_EVENT_COORD, 1, CONDITION_ALWAYS, EVENT_SCRIPT(Imil_LeaveFallsCave) },
    { MAP_EVENT_COORD, 10, CONDITION_ALWAYS, EVENT_SCRIPT(Imil_MoveAcrossIce) },
    { MAP_EVENT_TALK, LOCALID_FEVER, CONDITION_ALWAYS, EVENT_SCRIPT(Imil_RecruitFever) },
    { MAP_EVENTS_END },
};

struct MapEvent gImilFuchinFalls_EntranceEvents[] = {
    { COORD_EVENT_WALKING_INTO(FACING_EAST), 11, CONDITION_ALWAYS, EVENT_SCRIPT(FuchinFallsCave_Entrance_MoveFirstLogEast) },
    { COORD_EVENT_WALKING_INTO(FACING_WEST), 12, CONDITION_ALWAYS, EVENT_SCRIPT(FuchinFallsCave_Entrance_MoveFirstLogWest) },
    { MAP_EVENT_COORD, 11, CONDITION_ALWAYS, EVENT_SCRIPT(FuchinFallsCave_AnimateFirstLog) },
    { MAP_EVENT_COORD, 12, CONDITION_ALWAYS, EVENT_SCRIPT(FuchinFallsCave_AnimateFirstLog) },
    { COORD_EVENT_WALKING_INTO(FACING_SOUTH), 13, CONDITION_ALWAYS, EVENT_SCRIPT(FuchinFallsCave_Entrance_MoveSecondLogSouth) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 14, CONDITION_ALWAYS, EVENT_SCRIPT(FuchinFallsCave_Entrance_MoveSecondLogNorth) },
    { MAP_EVENT_COORD, 13, CONDITION_ALWAYS, EVENT_SCRIPT(FuchinFallsCave_AnimateSecondLog) },
    { MAP_EVENT_COORD, 14, CONDITION_ALWAYS, EVENT_SCRIPT(FuchinFallsCave_AnimateSecondLog) },
    { COORD_EVENT_WALKING_INTO(FACING_SOUTH), 15, CONDITION_ALWAYS, EVENT_SCRIPT(FuchinFallsCave_Entrance_MoveThirdLogSouth) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 16, CONDITION_ALWAYS, EVENT_SCRIPT(FuchinFallsCave_Entrance_MoveThirdLogNorth) },
    { MAP_EVENT_COORD, 15, CONDITION_ALWAYS, EVENT_SCRIPT(FuchinFallsCave_AnimateThirdLog) },
    { MAP_EVENT_COORD, 16, CONDITION_ALWAYS, EVENT_SCRIPT(FuchinFallsCave_AnimateThirdLog) },
    { MAP_EVENT_WARP, 21, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_ENTRANCE_TO_FUCHIN_TEMPLE_GROUNDS },
    { MAP_EVENT_WARP, 22, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_ENTRANCE_TO_FUCHIN_FALLS_CAVE_ZEPHYR },
    { MAP_EVENT_WARP, 23, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_ENTRANCE_TO_FUCHIN_FALLS_CAVE_ARCTIC_BLADE },
    { MAP_EVENT_WARP, 24, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_ENTRANCE_SOUTH_HALF_TO_NORTH_HALF },
    { MAP_EVENT_WARP, 25, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_ENTRANCE_NORTH_HALF_TO_SOUTH_HALF },
    { MAP_EVENT_WARP, 26, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_ENTRANCE_TO_FUCHIN_FALLS_CAVE_DRAGONS_EYE },
    { MAP_EVENT_WARP | WARP_STYLE_STAIRS_UP, 27, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_ENTRANCE_TO_FUCHIN_FALLS_CAVE_DRAGON_STATUE_WEST },
    { MAP_EVENT_WARP, 28, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_ENTRANCE_TO_FUCHIN_FALLS_CAVE_DRAGON_STATUE_EAST },
    { BG_EVENT(BG_EVENT_SILENT), 1, CONDITION_ALWAYS, EVENT_SCRIPT(FuchinFallsCave_ReadDragonFlameInscription) },
    { BG_EVENT(BG_EVENT_CHEST), 100, 0xef1, FIND_MIMIC | 1 },
    { MAP_EVENTS_END },
};

struct MapEvent gImilFuchinFalls_ZephyrRoomEvents[] = {
    { COORD_EVENT_WALKING_INTO(FACING_EAST), 11, FLAG_MAPSEC_TEMP_06, EVENT_SCRIPT(FuchinFallsCave_ZephyrRoom_MoveFirstLogToEastEnd) },
    { COORD_EVENT_WALKING_INTO(FACING_WEST), 11, CONDITION_ALWAYS, EVENT_SCRIPT(FuchinFallsCave_ZephyrRoom_NudgeFirstLogWest) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 17, CONDITION_ALWAYS, EVENT_SCRIPT(ImilFuchinFalls_HopOnTrigger) },
    { COORD_EVENT_WALKING_INTO(FACING_EAST), 17, CONDITION_ALWAYS, EVENT_SCRIPT(FuchinFallsCave_ZephyrRoom_MoveFirstLogEast) },
    { COORD_EVENT_WALKING_INTO(FACING_WEST), 12, CONDITION_ALWAYS, EVENT_SCRIPT(FuchinFallsCave_ZephyrRoom_MoveFirstLogToWestEnd) },
    { MAP_EVENT_COORD, 11, CONDITION_ALWAYS, EVENT_SCRIPT(FuchinFallsCave_AnimateFirstLog) },
    { MAP_EVENT_COORD, 12, CONDITION_ALWAYS, EVENT_SCRIPT(FuchinFallsCave_AnimateFirstLog) },
    { MAP_EVENT_COORD, 17, CONDITION_ALWAYS, EVENT_SCRIPT(FuchinFallsCave_AnimateFirstLog) },
    { COORD_EVENT_WALKING_INTO(FACING_WEST), 13, CONDITION_ALWAYS, EVENT_SCRIPT(ImilFuchinFalls_HopOnTrigger) },
    { COORD_EVENT_WALKING_INTO(FACING_WEST), 14, FLAG_MAPSEC_TEMP_04, EVENT_SCRIPT(ImilFuchinFalls_HopOnTrigger) },
    { COORD_EVENT_WALKING_INTO(FACING_SOUTH), 13, CONDITION_ALWAYS, EVENT_SCRIPT(FuchinFallsCave_ZephyrRoom_MoveSecondLogSouth) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 14, CONDITION_ALWAYS, EVENT_SCRIPT(FuchinFallsCave_ZephyrRoom_MoveSecondLogNorth) },
    { MAP_EVENT_COORD, 13, CONDITION_ALWAYS, EVENT_SCRIPT(FuchinFallsCave_AnimateSecondLog) },
    { MAP_EVENT_COORD, 14, CONDITION_ALWAYS, EVENT_SCRIPT(FuchinFallsCave_AnimateSecondLog) },
    { COORD_EVENT_WALKING_INTO(FACING_SOUTH), 15, CONDITION_ALWAYS, EVENT_SCRIPT(FuchinFallsCave_ZephyrRoom_MoveThirdLogSouth) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 16, CONDITION_ALWAYS, EVENT_SCRIPT(FuchinFallsCave_ZephyrRoom_MoveThirdLogNorth) },
    { MAP_EVENT_COORD, 15, CONDITION_ALWAYS, EVENT_SCRIPT(FuchinFallsCave_AnimateThirdLog) },
    { MAP_EVENT_COORD, 16, CONDITION_ALWAYS, EVENT_SCRIPT(FuchinFallsCave_AnimateThirdLog) },
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_ZEPHYR_TO_FUCHIN_FALLS_CAVE_DRAGONS_EYE },
    { MAP_EVENT_WARP, 2, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_ZEPHYR_TO_FUCHIN_FALLS_CAVE_ENTRANCE },
    { MAP_EVENT_TALK, LOCALID_ZEPHYR, CONDITION_ALWAYS, EVENT_SCRIPT(FuchinFallsCave_FightZephyr) },
    { MAP_EVENTS_END },
};

struct MapEvent gImilFuchinFalls_DragonsEyeRoomEvents[] = {
    { COORD_EVENT_WALKING_INTO(FACING_WEST), 10, CONDITION_ALWAYS, EVENT_SCRIPT(FuchinFallsCave_DragonsEyeRoom_MoveFirstLogWest) },
    { COORD_EVENT_WALKING_INTO(FACING_EAST), 11, CONDITION_ALWAYS, EVENT_SCRIPT(FuchinFallsCave_DragonsEyeRoom_MoveFirstLogEastFromMiddle) },
    { COORD_EVENT_WALKING_INTO(FACING_WEST), 11, CONDITION_ALWAYS, EVENT_SCRIPT(FuchinFallsCave_DragonsEyeRoom_MoveFirstLogWestIfClear) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 11, CONDITION_ALWAYS, EVENT_SCRIPT(FuchinFallsCave_HopNorth) },
    { COORD_EVENT_WALKING_INTO(FACING_EAST), 12, CONDITION_ALWAYS, EVENT_SCRIPT(FuchinFallsCave_DragonsEyeRoom_MoveFirstLogEastFromWestEnd) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 12, IF_FLAG_SET(FLAG_MAPSEC_TEMP_0B), EVENT_SCRIPT(FuchinFallsCave_HopNorth) },
    { MAP_EVENT_COORD, 10, CONDITION_ALWAYS, EVENT_SCRIPT(FuchinFallsCave_AnimateFirstLog) },
    { MAP_EVENT_COORD, 11, CONDITION_ALWAYS, EVENT_SCRIPT(FuchinFallsCave_AnimateFirstLog) },
    { MAP_EVENT_COORD, 12, CONDITION_ALWAYS, EVENT_SCRIPT(FuchinFallsCave_AnimateFirstLog) },
    { COORD_EVENT_WALKING_INTO(FACING_WEST), 14, CONDITION_ALWAYS, EVENT_SCRIPT(FuchinFallsCave_DragonsEyeRoom_MoveSecondLogWest) },
    { COORD_EVENT_WALKING_INTO(FACING_EAST), 13, CONDITION_ALWAYS, EVENT_SCRIPT(FuchinFallsCave_DragonsEyeRoom_MoveSecondLogEast) },
    { MAP_EVENT_COORD, 13, CONDITION_ALWAYS, EVENT_SCRIPT(FuchinFallsCave_AnimateSecondLog) },
    { MAP_EVENT_COORD, 14, CONDITION_ALWAYS, EVENT_SCRIPT(FuchinFallsCave_AnimateSecondLog) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 15, CONDITION_ALWAYS, EVENT_SCRIPT(FuchinFallsCave_DragonsEyeRoom_MoveThirdLogNorthFromSouthEnd) },
    { COORD_EVENT_WALKING_INTO(FACING_SOUTH), 16, CONDITION_ALWAYS, EVENT_SCRIPT(FuchinFallsCave_DragonsEyeRoom_MoveThirdLogSouthFromNorthEnd) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 17, CONDITION_ALWAYS, EVENT_SCRIPT(FuchinFallsCave_DragonsEyeRoom_NudgeThirdLogNorth) },
    { COORD_EVENT_WALKING_INTO(FACING_SOUTH), 17, FLAG_MAPSEC_TEMP_08, EVENT_SCRIPT(FuchinFallsCave_DragonsEyeRoom_MoveThirdLogSouthFromUpperStop) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 18, CONDITION_ALWAYS, EVENT_SCRIPT(FuchinFallsCave_DragonsEyeRoom_MoveThirdLogNorthFromMiddle) },
    { COORD_EVENT_WALKING_INTO(FACING_SOUTH), 18, FLAG_MAPSEC_TEMP_10, EVENT_SCRIPT(FuchinFallsCave_DragonsEyeRoom_MoveThirdLogSouthFromMiddle) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 19, CONDITION_ALWAYS, EVENT_SCRIPT(FuchinFallsCave_DragonsEyeRoom_MoveThirdLogNorthFromLowerStop) },
    { COORD_EVENT_WALKING_INTO(FACING_SOUTH), 19, CONDITION_ALWAYS, EVENT_SCRIPT(FuchinFallsCave_DragonsEyeRoom_MoveThirdLogSouthFromLowerStop) },
    { MAP_EVENT_COORD, 15, CONDITION_ALWAYS, EVENT_SCRIPT(FuchinFallsCave_AnimateThirdLog) },
    { MAP_EVENT_COORD, 16, CONDITION_ALWAYS, EVENT_SCRIPT(FuchinFallsCave_AnimateThirdLog) },
    { MAP_EVENT_COORD, 17, CONDITION_ALWAYS, EVENT_SCRIPT(FuchinFallsCave_AnimateThirdLog) },
    { MAP_EVENT_COORD, 18, CONDITION_ALWAYS, EVENT_SCRIPT(FuchinFallsCave_AnimateThirdLog) },
    { MAP_EVENT_COORD, 19, CONDITION_ALWAYS, EVENT_SCRIPT(FuchinFallsCave_AnimateThirdLog) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 20, CONDITION_ALWAYS, EVENT_SCRIPT(FuchinFallsCave_DragonsEyeRoom_MoveFourthLogNorth) },
    { COORD_EVENT_WALKING_INTO(FACING_SOUTH), 21, FLAG_MAPSEC_TEMP_08, EVENT_SCRIPT(FuchinFallsCave_DragonsEyeRoom_MoveFourthLogSouthFromNorthEnd) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 22, CONDITION_ALWAYS, EVENT_SCRIPT(FuchinFallsCave_DragonsEyeRoom_NudgeFourthLogNorth) },
    { COORD_EVENT_WALKING_INTO(FACING_SOUTH), 22, CONDITION_ALWAYS, EVENT_SCRIPT(FuchinFallsCave_DragonsEyeRoom_MoveFourthLogSouthFromUpperStop) },
    { COORD_EVENT_WALKING_INTO(FACING_WEST), 23, CONDITION_ALWAYS, EVENT_SCRIPT(FuchinFallsCave_HopWest) },
    { COORD_EVENT_WALKING_INTO(FACING_SOUTH), 23, CONDITION_ALWAYS, EVENT_SCRIPT(FuchinFallsCave_DragonsEyeRoom_MoveFourthLogSouthFromMiddle) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 24, CONDITION_ALWAYS, EVENT_SCRIPT(FuchinFallsCave_DragonsEyeRoom_UnusedMoveFourthLogSouthFromLowerStop) },
    { COORD_EVENT_WALKING_INTO(FACING_SOUTH), 24, CONDITION_ALWAYS, EVENT_SCRIPT(FuchinFallsCave_DragonsEyeRoom_UnusedMoveFourthLogSouthFromLastStop) },
    { MAP_EVENT_COORD, 20, CONDITION_ALWAYS, EVENT_SCRIPT(FuchinFallsCave_AnimateFourthLog) },
    { MAP_EVENT_COORD, 21, CONDITION_ALWAYS, EVENT_SCRIPT(FuchinFallsCave_AnimateFourthLog) },
    { MAP_EVENT_COORD, 22, CONDITION_ALWAYS, EVENT_SCRIPT(FuchinFallsCave_AnimateFourthLog) },
    { MAP_EVENT_COORD, 23, CONDITION_ALWAYS, EVENT_SCRIPT(FuchinFallsCave_AnimateFourthLog) },
    { MAP_EVENT_COORD, 24, CONDITION_ALWAYS, EVENT_SCRIPT(FuchinFallsCave_AnimateFourthLog) },
    { COORD_EVENT_WALKING_INTO(FACING_SOUTH), 25, CONDITION_ALWAYS, EVENT_SCRIPT(ImilFuchinFalls_HopOnTrigger) },
    { MAP_EVENT_WARP | WARP_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_DRAGONS_EYE_CHAMBER_TO_LOGS },
    { MAP_EVENT_WARP | WARP_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_DRAGONS_EYE_TO_FUCHIN_FALLS_CAVE_DRAGON_STATUE },
    { MAP_EVENT_WARP | WARP_STYLE_STAIRS_DOWN, 3, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_DRAGONS_EYE_LOGS_TO_CHAMBER },
    { MAP_EVENT_WARP, 4, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_DRAGONS_EYE_TO_FUCHIN_FALLS_CAVE_ENTRANCE },
    { MAP_EVENT_WARP, 5, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_DRAGONS_EYE_TO_FUCHIN_FALLS_CAVE_ZEPHYR },
    { BG_EVENT(BG_EVENT_SILENT), 30, FLAG_FUCHIN_FALLS_DRAGONS_EYE_FOUND, EVENT_SCRIPT(FuchinFallsCave_CollectDragonsEye) },
    { MAP_EVENTS_END },
};
// clang-format on

struct MapEvent gImilFuchinFalls_ArcticBladeRoomEvents[] = {
    {COORD_EVENT_WALKING_INTO(FACING_SOUTH), 11, CONDITION_ALWAYS,
     EVENT_SCRIPT(FuchinFallsCave_ArcticBladeRoom_MoveFirstLogSouth)},
    {COORD_EVENT_WALKING_INTO(FACING_NORTH), 12, CONDITION_ALWAYS,
     EVENT_SCRIPT(FuchinFallsCave_ArcticBladeRoom_MoveFirstLogNorth)},
    {MAP_EVENT_COORD, 11, CONDITION_ALWAYS,
     EVENT_SCRIPT(FuchinFallsCave_AnimateFirstLog)},
    {MAP_EVENT_COORD, 12, CONDITION_ALWAYS,
     EVENT_SCRIPT(FuchinFallsCave_AnimateFirstLog)},
    {COORD_EVENT_WALKING_INTO(FACING_WEST), 13, FLAG_MAPSEC_TEMP_13,
     EVENT_SCRIPT(FuchinFallsCave_ArcticBladeRoom_MoveSecondLogWest)},
    {COORD_EVENT_WALKING_INTO(FACING_EAST), 14, CONDITION_ALWAYS,
     EVENT_SCRIPT(FuchinFallsCave_ArcticBladeRoom_MoveSecondLogEast)},
    {MAP_EVENT_COORD, 13, CONDITION_ALWAYS,
     EVENT_SCRIPT(FuchinFallsCave_AnimateSecondLog)},
    {MAP_EVENT_COORD, 14, CONDITION_ALWAYS,
     EVENT_SCRIPT(FuchinFallsCave_AnimateSecondLog)},
    {COORD_EVENT_WALKING_INTO(FACING_EAST), 15, CONDITION_ALWAYS,
     EVENT_SCRIPT(FuchinFallsCave_ArcticBladeRoom_MoveThirdLogEast)},
    {COORD_EVENT_WALKING_INTO(FACING_WEST), 16, CONDITION_ALWAYS,
     EVENT_SCRIPT(FuchinFallsCave_ArcticBladeRoom_MoveThirdLogWest)},
    {MAP_EVENT_COORD, 15, CONDITION_ALWAYS,
     EVENT_SCRIPT(FuchinFallsCave_AnimateThirdLog)},
    {MAP_EVENT_COORD, 16, CONDITION_ALWAYS,
     EVENT_SCRIPT(FuchinFallsCave_AnimateThirdLog)},
    {MAP_EVENT_WARP | WARP_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_ARCTIC_BLADE_TO_FUCHIN_FALLS_CAVE_DRAGON_STATUE},
    {MAP_EVENT_WARP, 2, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_ARCTIC_BLADE_NORTH_HALF_TO_SOUTH_HALF},
    {MAP_EVENT_WARP, 3, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_ARCTIC_BLADE_SOUTH_HALF_TO_NORTH_HALF},
    {MAP_EVENT_WARP, 4, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_ARCTIC_BLADE_TO_FUCHIN_FALLS_CAVE_ENTRANCE},
    {BG_EVENT(BG_EVENT_CHEST), 100, 0xf6b, FIND_ITEM(ITEM_ARCTIC_BLADE)},
    {MAP_EVENTS_END},
};

struct MapEvent gImilFuchinFalls_KiRoomEvents[] = {
    {MAP_EVENT_WARP, 1, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_ORB_OF_FORCE_NORTH_HALF_TO_SOUTH_HALF},
    {MAP_EVENT_WARP, 2, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_ORB_OF_FORCE_SOUTH_HALF_TO_NORTH_HALF},
    {MAP_EVENT_WARP, 3, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_ORB_OF_FORCE_TO_FUCHIN_FALLS_CAVE_DRAGON_STATUE_WEST},
    {MAP_EVENT_WARP, 4, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_ORB_OF_FORCE_TO_FUCHIN_FALLS_CAVE_DRAGON_STATUE_EAST},
    {BG_EVENT(BG_EVENT_SILENT), 7, CONDITION_ALWAYS,
     EVENT_SCRIPT(FuchinFallsCave_ReadKiInscription)},
    {BG_EVENT(BG_EVENT_CHEST), 100, FLAG_FUCHIN_FALLS_ORB_OF_FORCE_FOUND,
     FIND_ITEM(ITEM_ORB_OF_FORCE)},
    {MAP_EVENTS_END},
};

struct MapEvent gImilFuchinFalls_DragonRoomEvents[] = {
    {MAP_EVENT_WARP, 1, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_DRAGON_STATUE_TO_FUCHIN_FALLS_CAVE_ORB_OF_FORCE_WEST},
    {MAP_EVENT_WARP, 2, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_DRAGON_STATUE_TO_FUCHIN_FALLS_CAVE_ORB_OF_FORCE_EAST},
    {MAP_EVENT_WARP | WARP_STYLE_STAIRS_DOWN, 3, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_DRAGON_STATUE_TO_FUCHIN_FALLS_CAVE_ARCTIC_BLADE},
    {MAP_EVENT_WARP, 4, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_TO_SOUTH_HALF_WEST},
    {MAP_EVENT_WARP, 5, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_TO_SOUTH_HALF_MIDDLE},
    {MAP_EVENT_WARP, 6, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_TO_SOUTH_HALF_EAST},
    {MAP_EVENT_WARP, 7, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_DRAGON_STATUE_SOUTH_HALF_TO_NORTH_HALF_WEST},
    {MAP_EVENT_WARP, 8, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_DRAGON_STATUE_SOUTH_HALF_TO_NORTH_HALF_MIDDLE},
    {MAP_EVENT_WARP, 9, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_DRAGON_STATUE_SOUTH_HALF_TO_NORTH_HALF_EAST},
    {MAP_EVENT_WARP | WARP_STYLE_STAIRS_DOWN, 10, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_DRAGON_STATUE_TO_FUCHIN_FALLS_CAVE_DRAGONS_EYE},
    {MAP_EVENT_WARP, 11, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_DRAGON_STATUE_TO_FUCHIN_FALLS_CAVE_ENTRANCE_EAST},
    {MAP_EVENT_WARP | WARP_STYLE_STAIRS_DOWN, 12, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_DRAGON_STATUE_TO_FUCHIN_FALLS_CAVE_ENTRANCE_WEST},
    {MAP_EVENT_COORD, 15, CONDITION_ALWAYS,
     EVENT_SCRIPT(FuchinFallsCave_FollowLeaderWithLight)},
    {MAP_EVENT_COORD, 16, CONDITION_ALWAYS,
     EVENT_SCRIPT(FuchinFallsCave_StopLightFollowingLeader)},
    {BG_EVENT(BG_EVENT_SILENT), 19, CONDITION_ALWAYS,
     EVENT_SCRIPT(FuchinFallsCave_ReadShadowInscription)},
    {USE_ITEM(ITEM_DRAGONS_EYE), 20, FLAG_FUCHIN_FALLS_DRAGONS_EYE_SET,
     EVENT_SCRIPT(FuchinFallsCave_LightDragonWithEye)},
    {BG_EVENT(BG_EVENT_SILENT), 20, CONDITION_ALWAYS,
     EVENT_SCRIPT(FuchinFallsCave_SearchDragonStatue)},
    {PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ENDS), 0,
     FLAG_FUCHIN_FALLS_DRAGONS_EYE_SET,
     EVENT_SCRIPT(FuchinFallsCave_EndRevealLighting)},
    {MAP_EVENTS_END},
};

s32 gImilFuchinFalls_ShortDustScript[] = {
    SC_WAIT(1), SC_LOOP(22, SC_TOP),  SC_CALL(FuchinFallsCave_SetDustPalette),
    SC_WAIT(1), SC_POSITION(0, 0, 0), SC_WAIT(1),
    SC_FREE,
};

s32 gImilFuchinFalls_MediumDustScript[] = {
    SC_WAIT(1), SC_LOOP(44, SC_TOP),  SC_CALL(FuchinFallsCave_SetDustPalette),
    SC_WAIT(1), SC_POSITION(0, 0, 0), SC_WAIT(1),
    SC_FREE,
};

s32 gImilFuchinFalls_LongDustScript[] = {
    SC_WAIT(1), SC_LOOP(126, SC_TOP), SC_CALL(FuchinFallsCave_SetDustPalette),
    SC_WAIT(1), SC_POSITION(0, 0, 0), SC_WAIT(1),
    SC_FREE,
};
