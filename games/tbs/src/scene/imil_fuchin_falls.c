/// @file
/// Imil Falls Cave, north of Imil, and Fuchin Falls Cave, the waterfall
/// grotto of Fuchin Temple's test: two places in seven scenes.
///
/// - Imil Falls Cave (ImilFallsCave, scene 52), up the way north out of
///   Imil; its only exit, IMIL_FALLS_CAVE_TO_IMIL, leads back down
///   (IMIL_FROM_IMIL_FALLS_CAVE). Fever, a Mars Djinni, waits on its ice and
///   joins the party when spoken to, without a battle. Every normal entry
///   blocks Retreat and walks the leader up toward Fever, and stepping onto
///   the ice (collision code 10) slides the leader in the held direction,
///   turning toward it as he goes, until a blocked cell or one too high stops
///   him.
/// - Fuchin Falls Cave (FuchinFallsCaveEntrance to FuchinFallsCaveDragonStatue,
/// scenes 62 to
///   67), the waterfall grotto. Master Nyunpa of Fuchin Temple tells the
///   party they must survive its test before he will tell them how to pass
///   through Mogall Forest. The test ends at the chest of the Orb of Force
///   (FLAG_FUCHIN_FALLS_ORB_OF_FORCE_FOUND), and back at the temple Nyunpa
///   names the power they have gained Ki (fuchin_temple.c). The way in and
///   out is FUCHIN_FALLS_CAVE_ENTRANCE_TO_FUCHIN_TEMPLE_GROUNDS, to
///   FUCHIN_TEMPLE_GROUNDS_FROM_FUCHIN_FALLS_CAVE_ENTRANCE. The code numbers
///   the rooms by scene: the first to fourth rooms (FuchinFallsCaveEntrance to
///   FuchinFallsCaveArcticBlade), the room of the Ki inscription
///   (FuchinFallsCaveOrbOfForce) and the dragon statue's room
///   (FuchinFallsCaveDragonStatue).
///
///   The first four rooms are crossed on floating logs (sprites 0xec and 0xed,
///   the logs of Colosso's Lumber Water Crossing Area,
///   colosseum_azart_match.c). Walking into a log's trigger cell pushes it
///   along the water to its next stop, the leader moving with it
///   (SceneActor_PlacePairAtOffset); touching the cell plays the log's two
///   rolling animations. Each stop is a place flag, FLAG_PLACE_TEMP_01 to
///   FLAG_PLACE_TEMP_03 for the first room's logs, FLAG_PLACE_TEMP_04 to
///   FLAG_PLACE_TEMP_07 the second's, FLAG_PLACE_TEMP_08 to FLAG_PLACE_TEMP_12
///   the third's and FLAG_PLACE_TEMP_13 to FLAG_PLACE_TEMP_15 the fourth's, so
///   the logs are back where they started once the party leaves the cave's
///   scenes. After each move, and on entry, the room's rebuild function puts
///   every log at the cell its flags name and rewrites the cells of each log's
///   track to match. In the third room the logs block one another, so where a
///   log stops depends on where the others lie; in the second and third rooms
///   the leader hops across gaps a log's stop leaves.
///
///   Zephyr, a Jupiter Djinni, waits in the second room (scene 63) and fights
///   when spoken to. In the first room a chest holds a mimic, whose battle
///   its find value starts (type 5); the fourth room's chest holds an Arctic
///   Blade, and the Ki room's the Orb of Force. Arriving in the third room by
///   entrance 1 before the Dragon's Eye is taken (flag 0xf13) shows it, and
///   searching there takes it.
///
///   The dragon statue's room is dark until the eye is set: entering its
///   northern half (entrances 1 to 6: from the Ki room, from the fourth room
///   or up from the southern half) darkens the palette. Coming in from the Ki
///   room
///   (FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_ORB_OF_FORCE_EAST
///   and _WEST) or up by entrance 5 starts the fade overlay
///   (object/object/fade_overlay.c), which follows the leader except from
///   entrance 5, and triggers 15 and 16 start and stop its following. When
///   Reveal ends, the room goes dark again. The statue is `an eyeless dragon`;
///   using the Dragon's Eye on it flashes the room, blows dust from the cave
///   mouth as the room brightens, redraws the cells of the hidden path and uses
///   the eye up (FLAG_FUCHIN_FALLS_DRAGONS_EYE_SET). The first room's
///   inscription points the way: `The dragon's flame illuminates the path of
///   truth in darkness.`

#include "script.h"
#include "game_state.h"
#include "keystate.h"
#include "scene_ids.h"
#include "unit/import.h"
#include "unit/scene/imil_fuchin_falls.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
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
#include "field_event.h"
#include "field_scene.h"
#include "djinn_ids.h"
#include "call.h"
#include "battle_party.h"
#include "field_effect.h"
#include "scene.h"
#include "field_runtime.h"
#include "display_transition.h"
#include "fx_scene.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "objcmd.h"
#include "fixed_math.h"
#include "object_effect.h"
#include "palbuf.h"
#include "flags.h"
#include "songs.h"
#include "item_ids.h"

/// The placed actors, numbering each scene's placement rows from
/// ACTOR_FIRST_PLACED: Fever in Imil Falls Cave, the floating logs of Fuchin
/// Falls Cave's first four rooms, followed in the second room by Zephyr, and
/// the one sprite the dragon statue's room places.
enum ImilFuchinFallsActor {
  ACTOR_FEVER = ACTOR_FIRST_PLACED, ///< a Mars Djinni, in Imil Falls Cave
  /// The floating logs: three in the first, second and fourth rooms, four in
  /// the third.
  ACTOR_FIRST_LOG = ACTOR_FIRST_PLACED,
  ACTOR_SECOND_LOG,
  ACTOR_THIRD_LOG,
  ACTOR_FOURTH_LOG,
  /// Zephyr, a Jupiter Djinni, after the second room's three logs.
  ACTOR_ZEPHYR = ACTOR_FIRST_PLACED + 3,
  /// Sprite 0xdf in the dragon statue's room, the sprite of the third room's
  /// two foreground pieces (Fuchin_CreateForegroundActor); on entry
  /// ImilFuchinFalls_PrepareRoom stills it at floor height and draws it at
  /// sprite priority 1, as those are drawn. XXX: what it shows is not known; a
  /// render of the room would show it.
  ACTOR_DRAGON_ROOM_FOREGROUND = ACTOR_FIRST_PLACED
};

enum {
  IMIL_SLIDE_ANIMATION_STEP = 3 * 16 ///< three times normal animation speed
};

static void Fuchin_RebuildEntranceLogs(void);
static void Fuchin_RebuildZephyrRoomLogs(void);
static void Fuchin_RebuildDragonsEyeRoomLogs(void);
static void Fuchin_RebuildArcticBladeRoomLogs(void);
static void Imil_WalkTowardFever(void);
static void ImilFuchinFalls_PrepareRoom(void);
static void ImilFuchinFalls_RebuildLogRoom(void);
static void ImilFuchinFalls_HopBy(s32 dx, s32 dz);
static void ImilFuchinFalls_PlaceDragonsEye(void);
static void Imil_SlideActorAcrossIce(void);

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
  Djinn_JoinParty(ACTOR_FEVER, ELEMENT_MARS, 1);
}

/// Talking to Zephyr, a Jupiter Djinni, in Fuchin Falls Cave's second room
/// (scene 63) starts its battle.
static void Fuchin_FightZephyr(void) {
  Field_RequestDjinnBattle(ACTOR_ZEPHYR, DJINNI_ZEPHYR);
}

const struct SceneEntrance *ImilFuchinFalls_GetEntrances(void) {
  s16 sceneId = gGameState.scene;

  if (sceneId == (s32)&SceneId_ImilFallsCave) {
    return gImilFuchinFalls_ImilFallsCaveEntrances;
  }
  if (sceneId == (s32)&SceneId_FuchinFallsCaveEntrance) {
    return gImilFuchinFalls_FirstRoomEntrances;
  }
  if (sceneId == (s32)&SceneId_FuchinFallsCaveZephyr) {
    return gImilFuchinFalls_SecondRoomEntrances;
  }
  if (sceneId == (s32)&SceneId_FuchinFallsCaveDragonsEye) {
    return gImilFuchinFalls_ThirdRoomEntrances;
  }
  if (sceneId == (s32)&SceneId_FuchinFallsCaveArcticBlade) {
    return gImilFuchinFalls_FourthRoomEntrances;
  }
  if (sceneId == (s32)&SceneId_FuchinFallsCaveOrbOfForce) {
    return gImilFuchinFalls_KiRoomEntrances;
  }
  if (sceneId == (s32)&SceneId_FuchinFallsCaveDragonStatue) {
    return gImilFuchinFalls_DragonRoomEntrances;
  }
  return gImilFuchinFalls_NoEntrances;
}

const struct SceneRegion *ImilFuchinFalls_GetRegions(void) { return NULL; }

/// Returns the exit table every scene of the overlay shares.
const u32 *ImilFuchinFalls_GetExits(void) { return gImilFuchinFalls_Exits; }

const struct ScenePlacement *ImilFuchinFalls_GetPlacements(void) {
  s16 sceneId = gGameState.scene;

  if (sceneId == (s32)&SceneId_ImilFallsCave) {
    return gImilFuchinFalls_ImilFallsCavePlacements;
  }
  if (sceneId == (s32)&SceneId_FuchinFallsCaveEntrance) {
    return gImilFuchinFalls_FirstRoomPlacements;
  }
  if (sceneId == (s32)&SceneId_FuchinFallsCaveZephyr) {
    return gImilFuchinFalls_SecondRoomPlacements;
  }
  if (sceneId == (s32)&SceneId_FuchinFallsCaveDragonsEye) {
    return gImilFuchinFalls_ThirdRoomPlacements;
  }
  if (sceneId == (s32)&SceneId_FuchinFallsCaveArcticBlade) {
    return gImilFuchinFalls_FourthRoomPlacements;
  }
  if (sceneId == (s32)&SceneId_FuchinFallsCaveDragonStatue) {
    return gImilFuchinFalls_DragonRoomPlacements;
  }
  return gImilFuchinFalls_NoPlacements;
}

#include "common/object_pairmove.h"

/// Lets the log settle before rebuilding its room's collision cells.
static inline void Fuchin_FinishLogMove(void (*rebuildLogs)(void)) {
  Engine_AudioPlayCue(SE_289);
  Engine_TaskWait(2);
  rebuildLogs();
}

/// Fuchin Falls Cave, first room (scene 62): pushing the first log east
/// (trigger 11) floats it 224 pixels east (FLAG_PLACE_TEMP_01 set).
static void Fuchin_FirstRoom_MoveFirstLogEast(void) {
  Engine_AudioPlayCue(SE_241);
  SceneActor_PlacePairAtOffset(ACTOR_FIRST_LOG, 112, 0);
  SceneActor_PlacePairAtOffset(ACTOR_FIRST_LOG, 112, 0);
  Engine_AudioPlayCue(SE_289);
  Engine_GameFlagSet(FLAG_PLACE_TEMP_01);
  Engine_TaskWait(2);
  Fuchin_RebuildEntranceLogs();
}

/// Fuchin Falls Cave, first room (scene 62): pushing the first log west
/// (trigger 12) floats it 224 pixels back (FLAG_PLACE_TEMP_01 clear).
static void Fuchin_FirstRoom_MoveFirstLogWest(void) {
  s32 offset = 112;

  offset = -offset;
  Engine_AudioPlayCue(SE_241);
  SceneActor_PlacePairAtOffset(ACTOR_FIRST_LOG, offset, 0);
  SceneActor_PlacePairAtOffset(ACTOR_FIRST_LOG, offset, 0);
  Engine_AudioPlayCue(SE_289);
  Engine_GameFlagClear(FLAG_PLACE_TEMP_01);
  Engine_TaskWait(2);
  Fuchin_RebuildEntranceLogs();
}

/// Fuchin Falls Cave, first room (scene 62): pushing the second log south
/// (trigger 13) floats it 64 pixels south (FLAG_PLACE_TEMP_02 clear).
static void Fuchin_FirstRoom_MoveSecondLogSouth(void) {
  Engine_AudioPlayCue(SE_241);
  SceneActor_PlacePairAtOffset(ACTOR_SECOND_LOG, 0, 64);
  Engine_AudioPlayCue(SE_289);
  Engine_GameFlagClear(FLAG_PLACE_TEMP_02);
  Engine_TaskWait(2);
  Fuchin_RebuildEntranceLogs();
}

/// Fuchin Falls Cave, first room (scene 62): pushing the second log north
/// (trigger 14) floats it 64 pixels north (FLAG_PLACE_TEMP_02 set).
static void Fuchin_FirstRoom_MoveSecondLogNorth(void) {
  Engine_AudioPlayCue(SE_241);
  SceneActor_PlacePairAtOffset(ACTOR_SECOND_LOG, 0, -64);
  Engine_AudioPlayCue(SE_289);
  Engine_GameFlagSet(FLAG_PLACE_TEMP_02);
  Engine_TaskWait(2);
  Fuchin_RebuildEntranceLogs();
}

/// Fuchin Falls Cave, first room (scene 62): pushing the third log south
/// (trigger 15) floats it 64 pixels south (FLAG_PLACE_TEMP_03 clear).
static void Fuchin_FirstRoom_MoveThirdLogSouth(void) {
  Engine_AudioPlayCue(SE_241);
  SceneActor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, 64);
  Engine_AudioPlayCue(SE_289);
  Engine_GameFlagClear(FLAG_PLACE_TEMP_03);
  Engine_TaskWait(2);
  Fuchin_RebuildEntranceLogs();
}

/// Fuchin Falls Cave, first room (scene 62): pushing the third log north
/// (trigger 16) floats it 64 pixels north (FLAG_PLACE_TEMP_03 set).
static void Fuchin_FirstRoom_MoveThirdLogNorth(void) {
  Engine_AudioPlayCue(SE_241);
  SceneActor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, -64);
  Engine_AudioPlayCue(SE_289);
  Engine_GameFlagSet(FLAG_PLACE_TEMP_03);
  Engine_TaskWait(2);
  Fuchin_RebuildEntranceLogs();
}

/// Fuchin Falls Cave, second room (scene 63): pushing the first log east
/// (trigger 11), while FLAG_PLACE_TEMP_06 is clear, floats it 144 pixels east
/// (FLAG_PLACE_TEMP_04 set).
static void Fuchin_SecondRoom_MoveFirstLogToEastEnd(void) {
  Engine_AudioPlayCue(SE_241);
  SceneActor_PlacePairAtOffset(ACTOR_FIRST_LOG, 144, 0);
  Engine_AudioPlayCue(SE_289);
  Engine_GameFlagSet(FLAG_PLACE_TEMP_04);
  Engine_TaskWait(2);
  Fuchin_RebuildZephyrRoomLogs();
}

/// Fuchin Falls Cave, second room (scene 63): pushing the first log west
/// (trigger 12) floats it 144 pixels west (FLAG_PLACE_TEMP_04 clear,
/// FLAG_PLACE_TEMP_05 set).
static void Fuchin_SecondRoom_MoveFirstLogToWestEnd(void) {
  Engine_AudioPlayCue(SE_241);
  SceneActor_PlacePairAtOffset(ACTOR_FIRST_LOG, -144, 0);
  Engine_AudioPlayCue(SE_289);
  Engine_GameFlagClear(FLAG_PLACE_TEMP_04);
  Engine_GameFlagSet(FLAG_PLACE_TEMP_05);
  Engine_TaskWait(2);
  Fuchin_RebuildZephyrRoomLogs();
}

/// Fuchin Falls Cave, second room (scene 63): pushing the first log west
/// (trigger 11) floats it 14 pixels west (FLAG_PLACE_TEMP_05 set).
static void Fuchin_SecondRoom_NudgeFirstLogWest(void) {
  Engine_AudioPlayCue(SE_241);
  SceneActor_PlacePairAtOffset(ACTOR_FIRST_LOG, -14, 0);
  Engine_AudioPlayCue(SE_289);
  Engine_GameFlagSet(FLAG_PLACE_TEMP_05);
  Engine_TaskWait(2);
  Fuchin_RebuildZephyrRoomLogs();
}

/// Fuchin Falls Cave, second room (scene 63): pushing the first log east
/// (trigger 17) moves it one cell when the second log lies south
/// (FLAG_PLACE_TEMP_06; FLAG_PLACE_TEMP_05 clear), else 144 pixels
/// (FLAG_PLACE_TEMP_04 set).
static void Fuchin_SecondRoom_MoveFirstLogEast(void) {
  Engine_AudioPlayCue(SE_241);

  if (Engine_GameFlagIsSet(FLAG_PLACE_TEMP_06) != 0) {
    SceneActor_PlacePairAtOffset(ACTOR_FIRST_LOG, 16, 0);
    Engine_GameFlagClear(FLAG_PLACE_TEMP_05);
  } else {
    SceneActor_PlacePairAtOffset(ACTOR_FIRST_LOG, 144, 0);

    Engine_GameFlagSet(FLAG_PLACE_TEMP_04);
  }

  Fuchin_FinishLogMove(Fuchin_RebuildZephyrRoomLogs);
}

/// Fuchin Falls Cave, second room (scene 63): pushing the second log south
/// (trigger 13) floats it 64 pixels south (FLAG_PLACE_TEMP_06 set).
static void Fuchin_SecondRoom_MoveSecondLogSouth(void) {
  Engine_AudioPlayCue(SE_241);
  SceneActor_PlacePairAtOffset(ACTOR_SECOND_LOG, 0, 64);
  Engine_AudioPlayCue(SE_289);
  Engine_GameFlagSet(FLAG_PLACE_TEMP_06);
  Engine_TaskWait(2);
  Fuchin_RebuildZephyrRoomLogs();
}

/// Fuchin Falls Cave, second room (scene 63): pushing the second log north
/// (trigger 14) floats it 64 pixels north (FLAG_PLACE_TEMP_06 clear).
static void Fuchin_SecondRoom_MoveSecondLogNorth(void) {
  Engine_AudioPlayCue(SE_241);
  SceneActor_PlacePairAtOffset(ACTOR_SECOND_LOG, 0, -64);
  Engine_AudioPlayCue(SE_289);
  Engine_GameFlagClear(FLAG_PLACE_TEMP_06);
  Engine_TaskWait(2);
  Fuchin_RebuildZephyrRoomLogs();
}

/// Fuchin Falls Cave, second room (scene 63): pushing the third log south
/// (trigger 15) floats it 272 pixels south (FLAG_PLACE_TEMP_07 clear).
static void Fuchin_SecondRoom_MoveThirdLogSouth(void) {
  Engine_AudioPlayCue(SE_241);
  SceneActor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, 144);
  SceneActor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, 128);
  Engine_AudioPlayCue(SE_289);
  Engine_GameFlagClear(FLAG_PLACE_TEMP_07);
  Engine_TaskWait(2);
  Fuchin_RebuildZephyrRoomLogs();
}

/// Fuchin Falls Cave, second room (scene 63): pushing the third log north
/// (trigger 16) floats it 272 pixels north (FLAG_PLACE_TEMP_07 set).
static void Fuchin_SecondRoom_MoveThirdLogNorth(void) {
  s32 depth = -96;

  Engine_AudioPlayCue(SE_241);
  SceneActor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, depth);
  SceneActor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, depth);
  SceneActor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, -80);
  Engine_AudioPlayCue(SE_289);
  Engine_GameFlagSet(FLAG_PLACE_TEMP_07);
  Engine_TaskWait(2);
  Fuchin_RebuildZephyrRoomLogs();
}

/// Fuchin Falls Cave, third room (scene 64): pushing the first log west
/// (trigger 10): 48 pixels (FLAG_PLACE_TEMP_09 set) when the third log lies at
/// FLAG_PLACE_TEMP_0D or the fourth at FLAG_PLACE_TEMP_10, else 96
/// (FLAG_PLACE_TEMP_08 set).
static void Fuchin_ThirdRoom_MoveFirstLogWest(void) {
  Engine_AudioPlayCue(SE_241);

  if (Engine_GameFlagIsSet(FLAG_PLACE_TEMP_10) != 0 ||
      Engine_GameFlagIsSet(FLAG_PLACE_TEMP_0D) != 0) {
    SceneActor_PlacePairAtOffset(ACTOR_FIRST_LOG, -48, 0);
    Engine_GameFlagClear(FLAG_PLACE_TEMP_08);
    Engine_GameFlagSet(FLAG_PLACE_TEMP_09);
  } else {
    SceneActor_PlacePairAtOffset(ACTOR_FIRST_LOG, -96, 0);
    Engine_GameFlagSet(FLAG_PLACE_TEMP_08);
    Engine_GameFlagClear(FLAG_PLACE_TEMP_09);
  }

  Fuchin_FinishLogMove(Fuchin_RebuildDragonsEyeRoomLogs);
}

/// Fuchin Falls Cave, third room (scene 64): pushing the first log east
/// (trigger 11) floats it 48 pixels east (FLAG_PLACE_TEMP_08 and
/// FLAG_PLACE_TEMP_09 clear).
static void Fuchin_ThirdRoom_MoveFirstLogEastFromMiddle(void) {
  Engine_AudioPlayCue(SE_241);
  Engine_GameFlagClear(FLAG_PLACE_TEMP_08);
  Engine_GameFlagClear(FLAG_PLACE_TEMP_09);
  SceneActor_PlacePairAtOffset(ACTOR_FIRST_LOG, 48, 0);
  Fuchin_FinishLogMove(Fuchin_RebuildDragonsEyeRoomLogs);
}

/// Fuchin Falls Cave, third room (scene 64): pushing the first log west
/// (trigger 11), unless the third log lies at FLAG_PLACE_TEMP_0D or the fourth
/// at FLAG_PLACE_TEMP_10, floats it 48 pixels west (FLAG_PLACE_TEMP_08 set).
static void Fuchin_ThirdRoom_MoveFirstLogWestIfClear(void) {
  if (Engine_GameFlagIsSet(FLAG_PLACE_TEMP_10) != 0) {
    return;
  }
  if (Engine_GameFlagIsSet(FLAG_PLACE_TEMP_0D) != 0) {
    return;
  }

  Engine_AudioPlayCue(SE_241);

  Engine_GameFlagSet(FLAG_PLACE_TEMP_08);
  Engine_GameFlagClear(FLAG_PLACE_TEMP_09);

  SceneActor_PlacePairAtOffset(ACTOR_FIRST_LOG, -48, 0);
  Fuchin_FinishLogMove(Fuchin_RebuildDragonsEyeRoomLogs);
}

/// Fuchin Falls Cave, third room (scene 64): pushing the first log east
/// (trigger 12) floats it 96 pixels east (FLAG_PLACE_TEMP_08 and
/// FLAG_PLACE_TEMP_09 clear).
static void Fuchin_ThirdRoom_MoveFirstLogEastFromWestEnd(void) {
  Engine_AudioPlayCue(SE_241);
  Engine_GameFlagClear(FLAG_PLACE_TEMP_08);
  Engine_GameFlagClear(FLAG_PLACE_TEMP_09);
  SceneActor_PlacePairAtOffset(ACTOR_FIRST_LOG, 96, 0);
  Fuchin_FinishLogMove(Fuchin_RebuildDragonsEyeRoomLogs);
}

/// Fuchin Falls Cave, third room (scene 64): pushing the second log west
/// (trigger 14) floats it 32 pixels west (FLAG_PLACE_TEMP_0A set).
static void Fuchin_ThirdRoom_MoveSecondLogWest(void) {
  Engine_AudioPlayCue(SE_241);
  SceneActor_PlacePairAtOffset(ACTOR_SECOND_LOG, -32, 0);
  Engine_AudioPlayCue(SE_289);
  Engine_GameFlagSet(FLAG_PLACE_TEMP_0A);
  Engine_TaskWait(2);
  Fuchin_RebuildDragonsEyeRoomLogs();
}

/// Fuchin Falls Cave, third room (scene 64): pushing the second log east
/// (trigger 13) floats it 32 pixels east (FLAG_PLACE_TEMP_0A clear).
static void Fuchin_ThirdRoom_MoveSecondLogEast(void) {
  Engine_AudioPlayCue(SE_241);
  SceneActor_PlacePairAtOffset(ACTOR_SECOND_LOG, 32, 0);
  Engine_AudioPlayCue(SE_289);
  Engine_GameFlagClear(FLAG_PLACE_TEMP_0A);
  Engine_TaskWait(2);
  Fuchin_RebuildDragonsEyeRoomLogs();
}

/// Fuchin Falls Cave, third room (scene 64): pushing the third log north
/// (trigger 15): 64 pixels (FLAG_PLACE_TEMP_0C) when the first log lies at
/// FLAG_PLACE_TEMP_08, else 128 (FLAG_PLACE_TEMP_0B).
static void Fuchin_ThirdRoom_MoveThirdLogNorthFromSouthEnd(void) {
  Engine_AudioPlayCue(SE_241);
  if (Engine_GameFlagIsSet(FLAG_PLACE_TEMP_08) != 0) {
    SceneActor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, -64);
    Engine_GameFlagClear(FLAG_PLACE_TEMP_0B);
    Engine_GameFlagSet(FLAG_PLACE_TEMP_0C);
    Engine_GameFlagClear(FLAG_PLACE_TEMP_0D);
    Engine_GameFlagClear(FLAG_PLACE_TEMP_0E);
  } else {
    SceneActor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, -128);
    Engine_GameFlagSet(FLAG_PLACE_TEMP_0B);
    Engine_GameFlagClear(FLAG_PLACE_TEMP_0C);
    Engine_GameFlagClear(FLAG_PLACE_TEMP_0D);
    Engine_GameFlagClear(FLAG_PLACE_TEMP_0E);
  }
  Fuchin_FinishLogMove(Fuchin_RebuildDragonsEyeRoomLogs);
}

/// Fuchin Falls Cave, third room (scene 64): pushing the third log south
/// (trigger 16): one cell (FLAG_PLACE_TEMP_0C) when the first log lies at
/// FLAG_PLACE_TEMP_08 or the fourth at FLAG_PLACE_TEMP_10, 64 pixels
/// (FLAG_PLACE_TEMP_0D) when the fourth lies at FLAG_PLACE_TEMP_11, else 128
/// (its stops clear).
static void Fuchin_ThirdRoom_MoveThirdLogSouthFromNorthEnd(void) {
  Engine_AudioPlayCue(SE_241);

  if (Engine_GameFlagIsSet(FLAG_PLACE_TEMP_08) != 0) {
    SceneActor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, 16);
    Engine_GameFlagClear(FLAG_PLACE_TEMP_0B);
    Engine_GameFlagSet(FLAG_PLACE_TEMP_0C);
    Engine_GameFlagClear(FLAG_PLACE_TEMP_0D);
    Engine_GameFlagClear(FLAG_PLACE_TEMP_0E);
  } else if (Engine_GameFlagIsSet(FLAG_PLACE_TEMP_10) != 0) {
    SceneActor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, 16);
    Engine_GameFlagClear(FLAG_PLACE_TEMP_0B);
    Engine_GameFlagSet(FLAG_PLACE_TEMP_0C);
    Engine_GameFlagClear(FLAG_PLACE_TEMP_0D);
    Engine_GameFlagClear(FLAG_PLACE_TEMP_0E);
  } else if (Engine_GameFlagIsSet(FLAG_PLACE_TEMP_11) != 0) {
    SceneActor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, 64);
    Engine_GameFlagClear(FLAG_PLACE_TEMP_0B);
    Engine_GameFlagClear(FLAG_PLACE_TEMP_0C);
    Engine_GameFlagSet(FLAG_PLACE_TEMP_0D);
    Engine_GameFlagClear(FLAG_PLACE_TEMP_0E);
  } else {
    SceneActor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, 128);
    Engine_GameFlagClear(FLAG_PLACE_TEMP_0B);
    Engine_GameFlagClear(FLAG_PLACE_TEMP_0C);
    Engine_GameFlagClear(FLAG_PLACE_TEMP_0D);
    Engine_GameFlagClear(FLAG_PLACE_TEMP_0E);
  }
  Fuchin_FinishLogMove(Fuchin_RebuildDragonsEyeRoomLogs);
}

/// Fuchin Falls Cave, third room (scene 64): pushing the third log north
/// (trigger 17) moves it one cell (FLAG_PLACE_TEMP_0B).
static void Fuchin_ThirdRoom_NudgeThirdLogNorth(void) {
  Engine_AudioPlayCue(SE_241);
  SceneActor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, -16);
  Engine_AudioPlayCue(SE_289);
  Engine_GameFlagSet(FLAG_PLACE_TEMP_0B);
  Engine_GameFlagClear(FLAG_PLACE_TEMP_0C);
  Engine_GameFlagClear(FLAG_PLACE_TEMP_0D);
  Engine_GameFlagClear(FLAG_PLACE_TEMP_0E);
  Engine_TaskWait(2);
  Fuchin_RebuildDragonsEyeRoomLogs();
}

/// Fuchin Falls Cave, third room (scene 64): pushing the third log south
/// (trigger 17), unless the first log lies at FLAG_PLACE_TEMP_08: 48 pixels
/// (FLAG_PLACE_TEMP_0D) when the fourth lies at FLAG_PLACE_TEMP_11, 32
/// (FLAG_PLACE_TEMP_0C) when it lies at FLAG_PLACE_TEMP_10, else 112 (its stops
/// clear).
static void Fuchin_ThirdRoom_MoveThirdLogSouthFromUpperStop(void) {
  Engine_AudioPlayCue(SE_241);
  if (Engine_GameFlagIsSet(FLAG_PLACE_TEMP_11) != 0) {
    SceneActor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, 48);
    Engine_GameFlagClear(FLAG_PLACE_TEMP_0B);

    Engine_GameFlagClear(FLAG_PLACE_TEMP_0C);
    Engine_GameFlagSet(FLAG_PLACE_TEMP_0D);
    Engine_GameFlagClear(FLAG_PLACE_TEMP_0E);
  } else if (Engine_GameFlagIsSet(FLAG_PLACE_TEMP_10) != 0) {
    SceneActor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, 32);
    Engine_GameFlagClear(FLAG_PLACE_TEMP_0B);
    Engine_GameFlagSet(FLAG_PLACE_TEMP_0C);
    Engine_GameFlagClear(FLAG_PLACE_TEMP_0D);
    Engine_GameFlagClear(FLAG_PLACE_TEMP_0E);
  } else {
    SceneActor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, 112);
    Engine_GameFlagClear(FLAG_PLACE_TEMP_0B);
    Engine_GameFlagClear(FLAG_PLACE_TEMP_0C);
    Engine_GameFlagClear(FLAG_PLACE_TEMP_0D);
    Engine_GameFlagClear(FLAG_PLACE_TEMP_0E);
  }
  Fuchin_FinishLogMove(Fuchin_RebuildDragonsEyeRoomLogs);
}

/// Fuchin Falls Cave, third room (scene 64): pushing the third log north
/// (trigger 18) floats it 64 pixels north (FLAG_PLACE_TEMP_0B).
static void Fuchin_ThirdRoom_MoveThirdLogNorthFromMiddle(void) {
  Engine_AudioPlayCue(SE_241);
  SceneActor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, -64);
  Engine_AudioPlayCue(SE_289);
  Engine_GameFlagSet(FLAG_PLACE_TEMP_0B);
  Engine_GameFlagClear(FLAG_PLACE_TEMP_0D);
  Engine_TaskWait(2);
  Fuchin_RebuildDragonsEyeRoomLogs();
}

/// Fuchin Falls Cave, third room (scene 64): pushing the third log south
/// (trigger 18), unless the fourth lies at FLAG_PLACE_TEMP_10, floats it 64
/// pixels south (its stops clear).
static void Fuchin_ThirdRoom_MoveThirdLogSouthFromMiddle(void) {
  Engine_AudioPlayCue(SE_241);
  SceneActor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, 64);
  Engine_AudioPlayCue(SE_289);
  Engine_GameFlagClear(FLAG_PLACE_TEMP_0B);
  Engine_GameFlagClear(FLAG_PLACE_TEMP_0C);
  Engine_GameFlagClear(FLAG_PLACE_TEMP_0D);
  Engine_GameFlagClear(FLAG_PLACE_TEMP_0E);
  Engine_TaskWait(2);
  Fuchin_RebuildDragonsEyeRoomLogs();
}

/// Fuchin Falls Cave, third room (scene 64): pushing the third log north
/// (trigger 19) floats it 80 pixels north (FLAG_PLACE_TEMP_0B).
static void Fuchin_ThirdRoom_MoveThirdLogNorthFromLowerStop(void) {
  Engine_AudioPlayCue(SE_241);
  SceneActor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, -80);
  Engine_AudioPlayCue(SE_289);
  Engine_GameFlagSet(FLAG_PLACE_TEMP_0B);
  Engine_GameFlagClear(FLAG_PLACE_TEMP_0C);
  Engine_GameFlagClear(FLAG_PLACE_TEMP_0D);
  Engine_GameFlagClear(FLAG_PLACE_TEMP_0E);
  Engine_TaskWait(2);
  Fuchin_RebuildDragonsEyeRoomLogs();
}

/// Fuchin Falls Cave, third room (scene 64): pushing the third log south
/// (trigger 19) floats it 64 pixels south (its stops clear).
static void Fuchin_ThirdRoom_MoveThirdLogSouthFromLowerStop(void) {
  Engine_AudioPlayCue(SE_241);
  SceneActor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, 64);
  Engine_AudioPlayCue(SE_289);
  Engine_GameFlagClear(FLAG_PLACE_TEMP_0B);
  Engine_GameFlagClear(FLAG_PLACE_TEMP_0C);
  Engine_GameFlagClear(FLAG_PLACE_TEMP_0D);
  Engine_GameFlagClear(FLAG_PLACE_TEMP_0E);
  Engine_TaskWait(2);
  Fuchin_RebuildDragonsEyeRoomLogs();
}

/// Fuchin Falls Cave, third room (scene 64): pushing the fourth log north
/// (trigger 20): 64 pixels (FLAG_PLACE_TEMP_11) when the first log lies at
/// FLAG_PLACE_TEMP_08 or the third at FLAG_PLACE_TEMP_0D, 112
/// (FLAG_PLACE_TEMP_10) when the third lies at FLAG_PLACE_TEMP_0C, else 128
/// (FLAG_PLACE_TEMP_0F).
static void Fuchin_ThirdRoom_MoveFourthLogNorth(void) {
  Engine_AudioPlayCue(SE_241);

  if (Engine_GameFlagIsSet(FLAG_PLACE_TEMP_08) != 0 ||
      Engine_GameFlagIsSet(FLAG_PLACE_TEMP_0D) != 0) {
    SceneActor_PlacePairAtOffset(ACTOR_FOURTH_LOG, 0, -64);
    Engine_GameFlagClear(FLAG_PLACE_TEMP_0F);
    Engine_GameFlagClear(FLAG_PLACE_TEMP_10);
    Engine_GameFlagSet(FLAG_PLACE_TEMP_11);
    Engine_GameFlagClear(FLAG_PLACE_TEMP_12);
  } else if (Engine_GameFlagIsSet(FLAG_PLACE_TEMP_0C) != 0) {
    SceneActor_PlacePairAtOffset(ACTOR_FOURTH_LOG, 0, -112);
    Engine_GameFlagClear(FLAG_PLACE_TEMP_0F);
    Engine_GameFlagSet(FLAG_PLACE_TEMP_10);
    Engine_GameFlagClear(FLAG_PLACE_TEMP_11);
    Engine_GameFlagClear(FLAG_PLACE_TEMP_12);
  } else {
    SceneActor_PlacePairAtOffset(ACTOR_FOURTH_LOG, 0, -128);
    Engine_GameFlagSet(FLAG_PLACE_TEMP_0F);
    Engine_GameFlagClear(FLAG_PLACE_TEMP_10);
    Engine_GameFlagClear(FLAG_PLACE_TEMP_11);
    Engine_GameFlagClear(FLAG_PLACE_TEMP_12);
  }
  Fuchin_FinishLogMove(Fuchin_RebuildDragonsEyeRoomLogs);
}

/// Clears all saved intermediate stops for the fourth log.
static void Fuchin_ClearFourthLogStops(void) {
  Engine_GameFlagClear(FLAG_PLACE_TEMP_0F);
  Engine_GameFlagClear(FLAG_PLACE_TEMP_10);
  Engine_GameFlagClear(FLAG_PLACE_TEMP_11);
  Engine_GameFlagClear(FLAG_PLACE_TEMP_12);
}

/// Fuchin Falls Cave, third room (scene 64): pushing the fourth log south
/// (trigger 21), unless the first log lies at FLAG_PLACE_TEMP_08, floats it 128
/// pixels south (its stops clear).
static void Fuchin_ThirdRoom_MoveFourthLogSouthFromNorthEnd(void) {
  Engine_AudioPlayCue(SE_241);
  SceneActor_PlacePairAtOffset(ACTOR_FOURTH_LOG, 0, 128);
  Fuchin_ClearFourthLogStops();
  Fuchin_FinishLogMove(Fuchin_RebuildDragonsEyeRoomLogs);
}

/// Fuchin Falls Cave, third room (scene 64): pushing the fourth log north
/// (trigger 22) moves it one cell (FLAG_PLACE_TEMP_0F).
static void Fuchin_ThirdRoom_NudgeFourthLogNorth(void) {
  Engine_AudioPlayCue(SE_241);
  SceneActor_PlacePairAtOffset(ACTOR_FOURTH_LOG, 0, -16);
  Engine_AudioPlayCue(SE_289);
  Engine_GameFlagSet(FLAG_PLACE_TEMP_0F);
  Engine_GameFlagClear(FLAG_PLACE_TEMP_10);
  Engine_GameFlagClear(FLAG_PLACE_TEMP_11);
  Engine_GameFlagClear(FLAG_PLACE_TEMP_12);
  Engine_TaskWait(2);
  Fuchin_RebuildDragonsEyeRoomLogs();
}

/// Fuchin Falls Cave, third room (scene 64): pushing the fourth log south
/// (trigger 22) floats it 112 pixels south (its stops clear).
static void Fuchin_ThirdRoom_MoveFourthLogSouthFromUpperStop(void) {
  Engine_AudioPlayCue(SE_241);
  SceneActor_PlacePairAtOffset(ACTOR_FOURTH_LOG, 0, 112);
  Engine_AudioPlayCue(SE_289);
  Fuchin_ClearFourthLogStops();
  Engine_TaskWait(2);
  Fuchin_RebuildDragonsEyeRoomLogs();
}

/// Fuchin Falls Cave, third room (scene 64): pushing the fourth log south
/// (trigger 23) floats it 64 pixels south (its stops clear).
static void Fuchin_ThirdRoom_MoveFourthLogSouthFromMiddle(void) {
  Engine_AudioPlayCue(SE_241);
  SceneActor_PlacePairAtOffset(ACTOR_FOURTH_LOG, 0, 64);
  Engine_AudioPlayCue(SE_289);
  Fuchin_ClearFourthLogStops();
  Engine_TaskWait(2);
  Fuchin_RebuildDragonsEyeRoomLogs();
}

/// Fuchin Falls Cave, third room (scene 64): walking north into trigger 24
/// floats the fourth log 80 pixels south (its stops clear). XXX: the table and
/// the move disagree on the direction; the map around trigger 24 would settle
/// which is meant.
static void Fuchin_ThirdRoom_MoveFourthLogSouthFromLowerStop(void) {
  Engine_AudioPlayCue(SE_241);
  SceneActor_PlacePairAtOffset(ACTOR_FOURTH_LOG, 0, 80);
  Engine_AudioPlayCue(SE_289);
  Fuchin_ClearFourthLogStops();
  Engine_TaskWait(2);
  Fuchin_RebuildDragonsEyeRoomLogs();
}

/// Fuchin Falls Cave, third room (scene 64): pushing the fourth log south
/// (trigger 24) floats it 48 pixels south (its stops clear).
static void Fuchin_ThirdRoom_MoveFourthLogSouthFromLastStop(void) {
  Engine_AudioPlayCue(SE_241);
  SceneActor_PlacePairAtOffset(ACTOR_FOURTH_LOG, 0, 48);
  Engine_AudioPlayCue(SE_289);
  Fuchin_ClearFourthLogStops();
  Engine_TaskWait(2);
  Fuchin_RebuildDragonsEyeRoomLogs();
}

/// Fuchin Falls Cave, fourth room (scene 65): pushing the first log south
/// (trigger 11) floats it 112 pixels south (FLAG_PLACE_TEMP_13 set).
static void Fuchin_FourthRoom_MoveFirstLogSouth(void) {
  Engine_AudioPlayCue(SE_241);
  SceneActor_PlacePairAtOffset(ACTOR_FIRST_LOG, 0, 112);
  Engine_AudioPlayCue(SE_289);
  Engine_GameFlagSet(FLAG_PLACE_TEMP_13);
  Engine_TaskWait(2);
  Fuchin_RebuildArcticBladeRoomLogs();
}

/// Fuchin Falls Cave, fourth room (scene 65): pushing the first log north
/// (trigger 12) floats it 112 pixels north (FLAG_PLACE_TEMP_13 clear).
static void Fuchin_FourthRoom_MoveFirstLogNorth(void) {
  Engine_AudioPlayCue(SE_241);
  SceneActor_PlacePairAtOffset(ACTOR_FIRST_LOG, 0, -112);
  Engine_AudioPlayCue(SE_289);
  Engine_GameFlagClear(FLAG_PLACE_TEMP_13);
  Engine_TaskWait(2);
  Fuchin_RebuildArcticBladeRoomLogs();
}

/// Fuchin Falls Cave, fourth room (scene 65): pushing the second log west
/// (trigger 13), while the first log lies north (FLAG_PLACE_TEMP_13 clear),
/// floats it 128 pixels west (FLAG_PLACE_TEMP_14 set).
static void Fuchin_FourthRoom_MoveSecondLogWest(void) {
  Engine_AudioPlayCue(SE_241);
  SceneActor_PlacePairAtOffset(ACTOR_SECOND_LOG, -128, 0);
  Engine_AudioPlayCue(SE_289);
  Engine_GameFlagSet(FLAG_PLACE_TEMP_14);
  Engine_TaskWait(2);
  Fuchin_RebuildArcticBladeRoomLogs();
}

/// Fuchin Falls Cave, fourth room (scene 65): pushing the second log east
/// (trigger 14) floats it 128 pixels east (FLAG_PLACE_TEMP_14 clear).
static void Fuchin_FourthRoom_MoveSecondLogEast(void) {
  Engine_AudioPlayCue(SE_241);
  SceneActor_PlacePairAtOffset(ACTOR_SECOND_LOG, 128, 0);
  Engine_AudioPlayCue(SE_289);
  Engine_GameFlagClear(FLAG_PLACE_TEMP_14);
  Engine_TaskWait(2);
  Fuchin_RebuildArcticBladeRoomLogs();
}

/// Fuchin Falls Cave, fourth room (scene 65): pushing the third log east
/// (trigger 15) floats it 160 pixels east (FLAG_PLACE_TEMP_15 set).
static void Fuchin_FourthRoom_MoveThirdLogEast(void) {
  Engine_AudioPlayCue(SE_241);
  SceneActor_PlacePairAtOffset(ACTOR_THIRD_LOG, 160, 0);
  Engine_AudioPlayCue(SE_289);
  Engine_GameFlagSet(FLAG_PLACE_TEMP_15);
  Engine_TaskWait(2);
  Fuchin_RebuildArcticBladeRoomLogs();
}

/// Fuchin Falls Cave, fourth room (scene 65): pushing the third log west
/// (trigger 16) floats it 160 pixels west (FLAG_PLACE_TEMP_15 clear).
static void Fuchin_FourthRoom_MoveThirdLogWest(void) {
  Engine_AudioPlayCue(SE_241);
  SceneActor_PlacePairAtOffset(ACTOR_THIRD_LOG, -160, 0);
  Engine_AudioPlayCue(SE_289);
  Engine_GameFlagClear(FLAG_PLACE_TEMP_15);
  Engine_TaskWait(2);
  Fuchin_RebuildArcticBladeRoomLogs();
}

/// Touching the first log's trigger cell in Fuchin Falls Cave plays its two
/// rolling animations.
static void Fuchin_AnimateFirstLog(void) {
  Engine_ActorSetAnimation(ACTOR_FIRST_LOG, 1);
  Engine_ActorSetAnimation(ACTOR_FIRST_LOG, 2);
}

/// Touching the second log's trigger cell in Fuchin Falls Cave plays its two
/// rolling animations.
static void Fuchin_AnimateSecondLog(void) {
  Engine_ActorSetAnimation(ACTOR_SECOND_LOG, 1);
  Engine_ActorSetAnimation(ACTOR_SECOND_LOG, 2);
}

/// Touching the third log's trigger cell in Fuchin Falls Cave plays its two
/// rolling animations.
static void Fuchin_AnimateThirdLog(void) {
  Engine_ActorSetAnimation(ACTOR_THIRD_LOG, 1);
  Engine_ActorSetAnimation(ACTOR_THIRD_LOG, 2);
}

/// Touching the fourth log's trigger cell in Fuchin Falls Cave (third room)
/// plays its two rolling animations.
static void Fuchin_AnimateFourthLog(void) {
  Engine_ActorSetAnimation(ACTOR_FOURTH_LOG, 1);
  Engine_ActorSetAnimation(ACTOR_FOURTH_LOG, 2);
}

/// Creates a stationary foreground actor with its palette and priority set.
static struct FieldActor *Fuchin_CreateForegroundActor(s32 x, s32 y, s32 z,
                                                       s32 resourceId) {
  struct FieldActor *actor = Engine_ObjectCreate(resourceId, x, y, z);

  if (actor != 0) {
    actor->sprite->image.priority = 1;
    actor->motionFlags = 0;
    Engine_ActorSetSpriteFlags(actor, 0);
    ObjectGroup_SetChildValue(actor, 15);
    actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    return actor;
  }
  return 0;
}

const struct SceneEvent *ImilFuchinFalls_GetEvents(void) {
  s16 sceneId = gGameState.scene;

  if (sceneId == (s32)&SceneId_ImilFallsCave) {
    return gImilFuchinFalls_ImilFallsCaveEvents;
  }
  if (sceneId == (s32)&SceneId_FuchinFallsCaveEntrance) {
    return gImilFuchinFalls_FirstRoomEvents;
  }
  if (sceneId == (s32)&SceneId_FuchinFallsCaveZephyr) {
    return gImilFuchinFalls_SecondRoomEvents;
  }
  if (sceneId == (s32)&SceneId_FuchinFallsCaveDragonsEye) {
    return gImilFuchinFalls_ThirdRoomEvents;
  }
  if (sceneId == (s32)&SceneId_FuchinFallsCaveArcticBlade) {
    return gImilFuchinFalls_FourthRoomEvents;
  }
  if (sceneId == (s32)&SceneId_FuchinFallsCaveOrbOfForce) {
    return gImilFuchinFalls_KiRoomEvents;
  }
  if (sceneId == (s32)&SceneId_FuchinFallsCaveDragonStatue) {
    return gImilFuchinFalls_DragonRoomEvents;
  }
  return gImilFuchinFalls_NoEvents;
}

/// Walking into the way out of Imil Falls Cave (trigger 1): faces the leader
/// south and leaves for Imil.
static void Imil_LeaveFallsCave(void) {
  struct FieldActor *leader;

  Engine_EventBegin();
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  leader->facing = FACING_SOUTH;
  Engine_AudioPlayCue(SE_MAP_EXIT);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventRequestExit(IMIL_FALLS_CAVE_TO_IMIL);
}

/// Stepping onto the ice in Imil Falls Cave (collision code 10) slides the
/// selected actor in the held direction.
static void Imil_MoveAcrossIce(void) { Imil_SlideActorAcrossIce(); }

/// Centers a log on its cell and gives it underfoot priority.
static void Fuchin_PlaceLogAtCell(s32 id, s32 column, s32 row) {
  struct FieldActor *actor = Object_GetById(id);

  if (actor != NULL) {
    Engine_ActorSetSpritePriority(id, 3);
    actor->terrainId = 2;
    actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    actor->x.fixed = PIXELS(column * 16) + PIXELS(8);
    actor->z.fixed = PIXELS(row * 16) + PIXELS(8);
  }
}

/// Puts the logs of Fuchin Falls Cave's first room (scene 62) at the cells
/// their flags name, and rewrites the cells of their tracks to match.
static void Fuchin_RebuildEntranceLogs(void) {
  {
    s32 column = 8;
    s32 row = 29;
    Map_CopyCellAttributes(8, 42, 15, 5, column, row);
  }

  if (Engine_GameFlagIsSet(FLAG_PLACE_TEMP_01) != 0) {
    Fuchin_PlaceLogAtCell(ACTOR_FIRST_LOG, 22, 31);
    {
      s32 column = 8;
      s32 row = 30;
      Map_CopyCellAttributes(9, 30, 1, 3, column, row);
    }
  } else {
    Fuchin_PlaceLogAtCell(ACTOR_FIRST_LOG, 8, 31);
    {
      s32 column = 22;
      s32 row = 30;
      Map_CopyCellAttributes(9, 30, 1, 3, column, row);
    }
  }

  if (Engine_GameFlagIsSet(FLAG_PLACE_TEMP_02) != 0) {
    Fuchin_PlaceLogAtCell(ACTOR_SECOND_LOG, 12, 29);
    {
      s32 column = 11;
      s32 row = 33;
      Map_CopyCellAttributes(14, 33, 3, 1, column, row);
    }
  } else {
    Fuchin_PlaceLogAtCell(ACTOR_SECOND_LOG, 12, 33);
    {
      s32 column = 11;
      s32 row = 29;
      Map_CopyCellAttributes(14, 29, 3, 1, column, row);
    }
  }

  if (Engine_GameFlagIsSet(FLAG_PLACE_TEMP_03) != 0) {
    Fuchin_PlaceLogAtCell(ACTOR_THIRD_LOG, 18, 29);
    {
      s32 column = 17;
      s32 row = 33;
      Map_CopyCellAttributes(14, 33, 3, 1, column, row);
    }
  } else {
    Fuchin_PlaceLogAtCell(ACTOR_THIRD_LOG, 18, 33);
    {
      s32 column = 17;
      s32 row = 29;
      Map_CopyCellAttributes(14, 29, 3, 1, column, row);
    }
  }
}

/// Puts the logs of Fuchin Falls Cave's second room (scene 63) at the cells
/// their flags name, and rewrites the cells of their tracks to match.
static void Fuchin_RebuildZephyrRoomLogs(void) {
  {
    s32 column = 12;
    s32 row = 8;
    Map_CopyCellAttributes(0, 28, 10, 18, column, row);
  }

  if (Engine_GameFlagIsSet(FLAG_PLACE_TEMP_04) != 0) {
    Fuchin_PlaceLogAtCell(ACTOR_FIRST_LOG, 21, 20);
    {
      s32 column = 13;
      s32 row = 19;
      Map_CopyCellAttributes(20, 19, 1, 3, column, row);
    }
  } else {
    Fuchin_PlaceLogAtCell(ACTOR_FIRST_LOG, 13, 20);
    {
      s32 column = 21;
      s32 row = 19;
      Map_CopyCellAttributes(20, 19, 1, 3, column, row);
    }
  }

  if (Engine_GameFlagIsSet(FLAG_PLACE_TEMP_05) != 0) {
    Fuchin_PlaceLogAtCell(ACTOR_FIRST_LOG, 12, 20);
    {
      s32 column = 12;
      s32 row = 19;
      Map_CopyCellAttributes(5, 19, 1, 3, column, row);
    }
    {
      s32 column = 13;
      s32 row = 19;
      Map_CopyCellAttributes(20, 19, 1, 3, column, row);
    }
    if (Engine_GameFlagIsSet(FLAG_PLACE_TEMP_04) != 0) {
      Fuchin_PlaceLogAtCell(ACTOR_FIRST_LOG, 21, 20);
      {
        s32 column = 13;
        s32 row = 19;
        Map_CopyCellAttributes(20, 19, 1, 3, column, row);
      }
      {
        s32 column = 12;
        s32 row = 19;
        Map_CopyCellAttributes(20, 19, 1, 3, column, row);
      }
    }
  }

  if (Engine_GameFlagIsSet(FLAG_PLACE_TEMP_06) != 0) {
    Fuchin_PlaceLogAtCell(ACTOR_SECOND_LOG, 15, 21);
    {
      s32 column = 14;
      s32 row = 17;
      Map_CopyCellAttributes(14, 18, 3, 1, column, row);
    }
  } else {
    Fuchin_PlaceLogAtCell(ACTOR_SECOND_LOG, 15, 17);
    {
      s32 column = 14;
      s32 row = 21;
      Map_CopyCellAttributes(14, 18, 3, 1, column, row);
    }
  }

  if (Engine_GameFlagIsSet(FLAG_PLACE_TEMP_07) != 0) {
    Fuchin_PlaceLogAtCell(ACTOR_THIRD_LOG, 19, 8);
    {
      s32 column = 18;
      s32 row = 25;
      Map_CopyCellAttributes(14, 18, 3, 1, column, row);
    }
  } else {
    Fuchin_PlaceLogAtCell(ACTOR_THIRD_LOG, 19, 25);
    {
      s32 column = 18;
      s32 row = 8;
      Map_CopyCellAttributes(14, 18, 3, 1, column, row);
    }
  }
}

/// Puts the four logs of Fuchin Falls Cave's third room (scene 64) at the cells
/// their flags name, and rewrites the cells of their tracks to match.
static void Fuchin_RebuildDragonsEyeRoomLogs(void) {
  {
    Map_CopyCellAttributes(12, 3, 9, 16, 12, 21);
  }

  if (Engine_GameFlagIsSet(FLAG_PLACE_TEMP_08) != 0) {
    Fuchin_PlaceLogAtCell(ACTOR_FIRST_LOG, 14, 25);
    {
      Map_CopyCellAttributes(16, 24, 1, 3, 20, 24);
    }
  } else if (Engine_GameFlagIsSet(FLAG_PLACE_TEMP_09) != 0) {
    Fuchin_PlaceLogAtCell(ACTOR_FIRST_LOG, 17, 25);
    {
      s32 row = 24;
      Map_CopyCellAttributes(18, 24, 1, 3, 20, row);
      Map_CopyCellAttributes(18, 24, 1, 3, 14, row);
      Map_CopyCellAttributes(8, 41, 1, 3, 17, row);
    }
  } else {
    Fuchin_PlaceLogAtCell(ACTOR_FIRST_LOG, 20, 25);
    {
      Map_CopyCellAttributes(16, 24, 1, 3, 14, 24);
    }
  }

  if (Engine_GameFlagIsSet(FLAG_PLACE_TEMP_0A) != 0) {
    Fuchin_PlaceLogAtCell(ACTOR_SECOND_LOG, 13, 35);
    {
      Map_CopyCellAttributes(14, 34, 1, 3, 15, 34);
    }
  } else {
    Fuchin_PlaceLogAtCell(ACTOR_SECOND_LOG, 15, 35);
    {
      Map_CopyCellAttributes(14, 34, 1, 3, 13, 34);
    }
  }

  if (Engine_GameFlagIsSet(FLAG_PLACE_TEMP_0B) != 0) {
    Fuchin_PlaceLogAtCell(ACTOR_THIRD_LOG, 15, 22);
    {
      s32 column = 14;
      Map_CopyCellAttributes(14, 29, 3, 1, column, 30);
      Map_CopyCellAttributes(5, 41, 3, 1, column, 22);
    }
  } else if (Engine_GameFlagIsSet(FLAG_PLACE_TEMP_0C) != 0) {
    Fuchin_PlaceLogAtCell(ACTOR_THIRD_LOG, 15, 23);
    {
      s32 column = 14;
      Map_CopyCellAttributes(5, 42, 3, 1, column, 23);
      Map_CopyCellAttributes(14, 29, 3, 1, column, 30);
      Map_CopyCellAttributes(10, 44, 3, 1, column, 21);
    }
  } else if (Engine_GameFlagIsSet(FLAG_PLACE_TEMP_0D) != 0) {
    Fuchin_PlaceLogAtCell(ACTOR_THIRD_LOG, 15, 26);
    {
      s32 column = 14;
      Map_CopyCellAttributes(14, 29, 3, 1, column, 22);
      Map_CopyCellAttributes(5, 43, 3, 1, column, 26);
      Map_CopyCellAttributes(14, 29, 3, 1, column, 30);
    }
  } else if (Engine_GameFlagIsSet(FLAG_PLACE_TEMP_0E) != 0) {
    Fuchin_PlaceLogAtCell(ACTOR_THIRD_LOG, 15, 27);
    {
      s32 column = 14;
      Map_CopyCellAttributes(14, 29, 3, 1, column, 22);
      Map_CopyCellAttributes(14, 29, 3, 1, column, 30);
      Map_CopyCellAttributes(5, 44, 3, 1, column, 27);
    }
  } else {
    Fuchin_PlaceLogAtCell(ACTOR_THIRD_LOG, 15, 30);
  }

  if (Engine_GameFlagIsSet(FLAG_PLACE_TEMP_0F) != 0) {
    Fuchin_PlaceLogAtCell(ACTOR_FOURTH_LOG, 15, 23);
    {
      s32 column = 14;
      Map_CopyCellAttributes(14, 29, 3, 1, column, 31);
      Map_CopyCellAttributes(10, 40, 3, 1, column, 23);
    }
  } else if (Engine_GameFlagIsSet(FLAG_PLACE_TEMP_10) != 0) {
    Fuchin_PlaceLogAtCell(ACTOR_FOURTH_LOG, 15, 24);
    {
      s32 column = 14;
      Map_CopyCellAttributes(14, 29, 3, 1, column, 31);
      Map_CopyCellAttributes(10, 41, 3, 1, column, 24);
    }
  } else if (Engine_GameFlagIsSet(FLAG_PLACE_TEMP_11) != 0) {
    Fuchin_PlaceLogAtCell(ACTOR_FOURTH_LOG, 15, 27);
    {
      s32 column = 14;
      Map_CopyCellAttributes(14, 29, 3, 1, column, 31);
      Map_CopyCellAttributes(10, 42, 3, 1, column, 27);
    }
  } else if (Engine_GameFlagIsSet(FLAG_PLACE_TEMP_12) != 0) {
    Fuchin_PlaceLogAtCell(ACTOR_FOURTH_LOG, 15, 28);
    {
      s32 column = 14;
      Map_CopyCellAttributes(14, 29, 3, 1, column, 31);
      Map_CopyCellAttributes(10, 43, 3, 1, column, 28);
    }
  } else {
    Fuchin_PlaceLogAtCell(ACTOR_FOURTH_LOG, 15, 31);
  }
}

/// Puts the logs of Fuchin Falls Cave's fourth room (scene 65), in both of its
/// halves, at the cells their flags name.
static void Fuchin_RebuildArcticBladeRoomLogs(void) {
  if (((u32)((u16)gGameState.entrance - 1) << 16) <= 0x10000) {
    {
      s32 column = 14;
      s32 row = 10;
      Map_CopyCellAttributes(22, 20, 9, 8, column, row);
    }
  } else {
    {
      s32 column = 7;
      s32 row = 45;
      Map_CopyCellAttributes(20, 45, 11, 4, column, row);
    }
  }

  if (Engine_GameFlagIsSet(FLAG_PLACE_TEMP_13) != 0) {
    Fuchin_PlaceLogAtCell(ACTOR_FIRST_LOG, 20, 17);
    {
      s32 column = 19;
      s32 row = 10;
      Map_CopyCellAttributes(19, 11, 3, 1, column, row);
    }
  } else {
    Fuchin_PlaceLogAtCell(ACTOR_FIRST_LOG, 20, 10);
    {
      s32 column = 19;
      s32 row = 17;
      Map_CopyCellAttributes(19, 11, 3, 1, column, row);
    }
  }

  if (Engine_GameFlagIsSet(FLAG_PLACE_TEMP_14) != 0) {
    Fuchin_PlaceLogAtCell(ACTOR_SECOND_LOG, 14, 16);
    {
      s32 column = 22;
      s32 row = 15;
      Map_CopyCellAttributes(16, 15, 1, 3, column, row);
    }
  } else {
    Fuchin_PlaceLogAtCell(ACTOR_SECOND_LOG, 22, 16);
    {
      s32 column = 14;
      s32 row = 15;
      Map_CopyCellAttributes(16, 15, 1, 3, column, row);
    }
  }

  if (Engine_GameFlagIsSet(FLAG_PLACE_TEMP_15) != 0) {
    Fuchin_PlaceLogAtCell(ACTOR_THIRD_LOG, 17, 46);
    {
      s32 column = 7;
      s32 row = 45;
      Map_CopyCellAttributes(15, 15, 1, 3, column, row);
    }
  } else {
    Fuchin_PlaceLogAtCell(ACTOR_THIRD_LOG, 7, 46);
    {
      s32 column = 17;
      s32 row = 45;
      Map_CopyCellAttributes(15, 15, 1, 3, column, row);
    }
  }
}

/// Touching trigger 15 in Fuchin Falls Cave's dragon statue room makes the fade
/// overlay follow the leader, once it has been started there.
static void Fuchin_FollowLeaderWithLight(void) {
  if (gImilFuchinFalls_TrackLeader != 0) {
    struct FadeOverlayWork *work = *(gWorkSlot + 36);

    work->actor = Object_GetById(ACTOR_PARTY_LEADER);
  }
}

/// Touching trigger 16 in the dragon statue room stops the fade overlay
/// following the leader.
static void Fuchin_StopLightFollowingLeader(void) {
  if (gImilFuchinFalls_TrackLeader != 0) {
    struct FadeOverlayWork *work = *(gWorkSlot + 36);

    work->actor = NULL;
  }
}

/// Sets up the scene as the party enters. Every normal entry into Imil Falls
/// Cave blocks Retreat and walks the leader toward Fever; otherwise each room
/// of Fuchin Falls Cave restores its logs, the Dragon's Eye and its lighting.
s32 ImilFuchinFalls_Initialize(void) {
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) == 0 &&
      gGameState.scene == (s32)&SceneId_ImilFallsCave) {
    Engine_GameFlagSet(FLAG_RETREAT_BLOCKED);
    Imil_WalkTowardFever();
  } else {
    ImilFuchinFalls_PrepareRoom();
  }
  return 0;
}

/// Shows Fever, in Imil Falls Cave, and walks the leader toward it.
static void Imil_WalkTowardFever(void) {
  struct FieldActor *actor;

  Engine_EventBegin();
  actor = Object_GetById(ACTOR_FEVER);
  Engine_ActorSetSpriteFlags(actor, 0);
  Engine_EventOpenScreen();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(2), PIXELS(1) / 10);
  Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 264, 196);
  Engine_EventEnd();
}

/// Sets up a room of Fuchin Falls Cave for its entrance: its logs, the Dragon's
/// Eye and two foreground sprites in the third room, and the darkness and the
/// fade overlay in the dragon statue's room.
static void ImilFuchinFalls_PrepareRoom(void) {
  struct FieldActor *actor;
  /* FAKEMATCH(temporary): one variable holds the zeros and then the slot word,
   * so case 5 stores a fresh 0 and gGameState stays in r6; as literal zeros
   * and the slot read in place, 51 of 144 instructions differ in tbs-en. */
  s32 value;

  ImilFuchinFalls_RebuildLogRoom();
  if (gGameState.scene == (s32)&SceneId_FuchinFallsCaveDragonsEye) {
    if (!GameFlag_IsSet(0xf13) && gGameState.entrance == 1) {
      ImilFuchinFalls_PlaceDragonsEye();
    }
    if ((u16)(gGameState.entrance - 2) <= 3) {
      Fuchin_CreateForegroundActor(PIXELS(156), 0, PIXELS(452), 223);
      Fuchin_CreateForegroundActor(PIXELS(188), 0, PIXELS(452), 223);
    }
  } else if (gGameState.scene == (s32)&SceneId_FuchinFallsCaveDragonStatue) {
    value = 0;
    actor = Actor_Get(ACTOR_DRAGON_ROOM_FOREGROUND);
    gImilFuchinFalls_TrackLeader = value;
    actor->motionFlags = value;
    actor->y.fixed = value;
    Engine_ActorSetSpritePriority(ACTOR_DRAGON_ROOM_FOREGROUND, 1);
    Actor_SetChildValue(ACTOR_DRAGON_ROOM_FOREGROUND, 15);
    switch (gGameState.entrance) {
    case FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_ORB_OF_FORCE_EAST:
    case FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_ORB_OF_FORCE_WEST:
      BattleFx_StartFadeOverlay(0);
      gImilFuchinFalls_TrackLeader = 1;
      break;
    case 5:
      BattleFx_StartFadeOverlay(0);
      gImilFuchinFalls_TrackLeader = 1;
      value = *(s32 *)(gWorkSlot + 36);
      ((struct FadeOverlayWork *)value)->actor = NULL;
      break;
    }
    if (gGameState.entrance <= 6) {
      if (GameFlag_IsSet(FLAG_FUCHIN_FALLS_DRAGONS_EYE_SET)) {
        Map_CopyCellsTo(30, 57, 19, 57, 1, 1);
        Map_CopyCellsTo(30, 8, 12, 8, 8, 7);
      } else {
        gEventWork->startTransition =
            SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
        ColorBuffer_ApplySource(0x203108, 1);
        ColorBuffer_ApplyTarget(0x203108, 1);
        Engine_ColorBufferInterpolate(1);
        Engine_TaskWait(1);
      }
    }
  }
}

/// Restores the collision templates for the current log room of Fuchin Falls
/// Cave.
static void ImilFuchinFalls_RebuildLogRoom(void) {
  if (gGameState.scene == (s32)&SceneId_FuchinFallsCaveEntrance) {
    Map_CopyCellAttributes(8, 29, 15, 5, 8, 42);
    Fuchin_RebuildEntranceLogs();
  } else if (gGameState.scene == (s32)&SceneId_FuchinFallsCaveZephyr) {
    Map_CopyCellAttributes(12, 8, 10, 18, 0, 28);
    Fuchin_RebuildZephyrRoomLogs();
  } else if (gGameState.scene == (s32)&SceneId_FuchinFallsCaveDragonsEye &&
             gGameState.entrance != 1) {
    Map_CopyCellAttributes(12, 21, 9, 16, 12, 3);
    Fuchin_RebuildDragonsEyeRoomLogs();
  } else if (gGameState.scene == (s32)&SceneId_FuchinFallsCaveArcticBlade) {
    if (gGameState.entrance ==
            FUCHIN_FALLS_CAVE_ARCTIC_BLADE_FROM_FUCHIN_FALLS_CAVE_DRAGON_STATUE ||
        gGameState.entrance == 2) {
      Map_CopyCellAttributes(14, 10, 9, 8, 22, 20);
    } else {
      Map_CopyCellAttributes(7, 45, 11, 4, 20, 45);
    }
    Fuchin_RebuildArcticBladeRoomLogs();
  }
}

/// Hops across a gap in Fuchin Falls Cave: west or north in the second room
/// (triggers 13, 14 and 17), south in the third (trigger 25) when its first
/// log lies at FLAG_PLACE_TEMP_09.
static void ImilFuchinFalls_HopOnTrigger(void) {
  s32 trigger = gEventWork->touchedTrigger;

  if (gGameState.scene == (s32)&SceneId_FuchinFallsCaveZephyr) {
    if (trigger == 17) {
      ImilFuchinFalls_HopBy(0, -32);
    } else {
      ImilFuchinFalls_HopBy(-32, 0);
    }
  }
  if (gGameState.scene == (s32)&SceneId_FuchinFallsCaveDragonsEye &&
      trigger == 25 && Engine_GameFlagIsSet(FLAG_PLACE_TEMP_09)) {
    ImilFuchinFalls_HopBy(0, 32);
  }
}

/// Hops the leader one gap north, in Fuchin Falls Cave's third room (triggers
/// 11 and 12).
static void Fuchin_HopNorth(void) { ImilFuchinFalls_HopBy(0, -32); }

/// Hops the leader one gap west, in Fuchin Falls Cave's third room (trigger
/// 23).
static void Fuchin_HopWest(void) { ImilFuchinFalls_HopBy(-32, 0); }

/// Jumps the leader by a pixel offset while using the hopping animations.
static void ImilFuchinFalls_HopBy(s32 dx, s32 dz) {
  Engine_EventBegin();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(5) / 2, PIXELS(5) / 4);
  Engine_ActorSetDestinationOffset(ACTOR_PARTY_LEADER, dx, dz);
  Engine_ActorJump(ACTOR_PARTY_LEADER, 4, 0);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 7);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 6);
  Engine_EventEnd();
}

/// Shows the Dragon's Eye in Fuchin Falls Cave's third room, as its item icon.
static void ImilFuchinFalls_PlaceDragonsEye(void) {
  struct FieldActor *actor;
  struct AnimationObject *sprite;
  GlyphTransfer *glyph;
  u8 clear = 0;

  actor = Object_Create(22, PIXELS(248), PIXELS(8), PIXELS(152));
  if (actor != NULL) {
    sprite = actor->sprite;
    sprite->flags = clear;
    sprite->count = clear;
    sprite->image.fullColor = 0;
    sprite->image.palette = 0;
    actor->motionFlags = clear;
    actor->retainSpriteCache = 1;
    glyph = Engine_HeapAllocate(17, sizeof(*glyph));
    Engine_ItemLoadIcon(ITEM_DRAGONS_EYE);
    Engine_VramLoad(sprite->slot, 128, glyph->tiles);
    Engine_HeapRelease(17);
    gImilFuchinFalls_DragonsEye = actor;
  }
}

/// Searching where the Dragon's Eye lies, in Fuchin Falls Cave's third room
/// (trigger 30), gives it to the party (flag 0xf13).
static void Fuchin_CollectDragonsEye(void) {
  Engine_EventBegin();

  if (gImilFuchinFalls_DragonsEye != NULL) {
    Engine_RunRisingObjectSequence(gImilFuchinFalls_DragonsEye, 3);
  }

  Engine_PartyGiveItem(ITEM_DRAGONS_EYE, 0);
  Engine_GameFlagSet(0xF13);

  if (gImilFuchinFalls_DragonsEye != NULL) {
    Engine_ObjectDispatchRelease(gImilFuchinFalls_DragonsEye);
  }

  Engine_EventEnd();
}

/// Applies the dust velocity and scale change for one frame.
static void Fuchin_UpdateDustMotion(union FieldObject *raw) {
  struct FieldEffect *object = &raw->effect;

  object->x += object->velocity.x;
  object->y += object->velocity.y;
  object->z += object->velocity.z;
  object->scaleX += object->scaleRateX;
  object->scaleY += object->scaleRateY;
}

/// Selects the dust palette when its object script finishes looping.
static s32 Fuchin_SetDustPalette(struct ScriptInterpreter *interpreter) {
  struct FieldActor *actor = (struct FieldActor *)interpreter;
  ObjectGroup_SetChildValue(actor, 15);
  return 0;
}

/// Creates scripted cave dust with the requested velocity, palette and scale.
void ImilFuchinFalls_SpawnDust(s32 x, s32 y, s32 z, s32 velocityX,
                               s32 velocityY, s32 velocityZ, u32 flags,
                               const struct EffectOptions *extra) {
  struct FieldEffect *effect;
  struct AnimationObject *sprite;
  const s32 *script;
  const s32 *scripts[3] = {
      gImilFuchinFalls_DustScript0,
      gImilFuchinFalls_DustScript1,
      gImilFuchinFalls_DustScript2,
  };

  effect = (struct FieldEffect *)Engine_ObjectCreate(222, x, y, z);
  if (effect == 0)
    return;
  sprite = effect->sprite;
  Object_SetMode((struct FieldActor *)effect, (flags + 1) & EFFECT_SCRIPT_MASK);
  Engine_ObjectSetScript((struct FieldActor *)effect,
                         scripts[flags & EFFECT_SCRIPT_MASK]);
  effect->motionFlags = 0;
  sprite->flags = 0;
  effect->update = Fuchin_UpdateDustMotion;
  effect->velocity.x = velocityX;
  effect->velocity.y = velocityY;
  effect->velocity.z = velocityZ;
  effect->scaleRateX = 0;
  effect->scaleRateY = 0;
  sprite->image.priority = 1;
  if ((flags & 0xffff0000) == 0 || extra == 0)
    return;
  if (flags & EFFECT_USE_PALETTE)
    ObjectGroup_SetChildValue((struct FieldActor *)effect, extra->palette);
  if (flags & EFFECT_USE_PRIORITY) {
    effect->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
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
    Engine_AudioPlayCue(SE_136);
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
static void Fuchin_LightDragonWithEye(void) {
  u32 pass;

  // Flash the room as the eye opens the dragon's path.
  Engine_AudioPlayCue(MUS_019);
  Engine_AudioPlayCue(SE_182);
  Engine_EventBegin();
  Battle_ResetEffectCounter();

  for (pass = 0; pass < 4; pass++) {
    Engine_ColorBufferApplyTarget(0x204318, 1);
    Engine_ColorBufferInterpolate(1);
    Engine_TaskWait(2);
    if (pass == 0) {
      Map_CopyCellsTo(30, 8, 12, 8, 8, 7);
      Map_CopyCellsTo(30, 57, 19, 57, 1, 1);
    }
    Engine_ColorBufferApplyTarget(0x203108, 1);
    Engine_ColorBufferInterpolate(1);
    Engine_TaskWait(2);
  }

  Engine_TaskWait(30);

  // Blow dust from the cave mouth while the room brightens.
  FieldScheduler_AddCallback(ImilFuchinFalls_BlowCaveMouthDust,
                             TASK_PRIORITY_SCENE);
  Engine_TaskWait(40);
  Engine_ColorBufferApplyTarget(0x201090, 1);
  Engine_ColorBufferInterpolate(40);
  Engine_TaskWait(80);
  Engine_TaskRemoveCallback(ImilFuchinFalls_BlowCaveMouthDust);
  Engine_TaskWait(20);

  // Settle the lighting, consume the eye and remember the revealed path.
  Engine_ColorBufferApplyTarget(PALETTE_TRANSFORM_PLAIN, 1);
  Engine_ColorBufferInterpolate(80);

  Engine_TaskWait(80);

  Engine_GameFlagSet(FLAG_FUCHIN_FALLS_DRAGONS_EYE_SET);
  PartyInventory_Discard(ITEM_DRAGONS_EYE);
  Audio_PlayCueFromEventWork();

  Engine_EventEnd();
}

/// Searching the inscription in Fuchin Falls Cave's first room (trigger 1):
/// `The dragon's flame illuminates the path of truth in darkness.`
static void Fuchin_ReadDragonFlameInscription(void) {
  Engine_EventBegin();
  Engine_MessageShowCentered(MsgFuchinDragonFlameShowsPath, 1);
  Engine_EventEnd();
}

/// Searching the inscription in Fuchin Falls Cave's Ki room (trigger 7): `The
/// secret of Ki shall be revealed to the disciples of truth.`
static void Fuchin_ReadKiInscription(void) {
  Engine_EventBegin();
  Engine_MessageShowCentered(MsgFuchinSecretKiRevealed, 1);
  Engine_EventEnd();
}

/// Searching the inscription in Fuchin Falls Cave's dragon statue room
/// (trigger 19): `Rays of light give birth to shadows, revealing the way.`
static void Fuchin_ReadShadowInscription(void) {
  Engine_EventBegin();
  Engine_MessageShowCentered(MsgFuchinLightRevealsShadows, 1);
  Engine_EventEnd();
}

/// Searching the dragon statue in Fuchin Falls Cave (trigger 20): its burning
/// red eyes once set, else its empty sockets, and with the Dragon's Eye carried
/// the offer to set it.
static void Fuchin_SearchDragonStatue(void) {
  Engine_EventBegin();

  if (Engine_GameFlagIsSet(FLAG_FUCHIN_FALLS_DRAGONS_EYE_SET) != 0) {
    Engine_MessageShowCentered(MsgFuchinDragonRedEyes, 1);
  } else {
    Engine_MessageShowCentered(MsgFuchinEyelessDragon, 1);
    if (PartyInventory_FindOwner(ITEM_DRAGONS_EYE) != -1) {
      gEventWork->unk172 = 1;
    }
  }
  Engine_EventEnd();
}

/// When Reveal ends in Fuchin Falls Cave's dragon statue room, before the eye
/// is set, the room goes dark again.
static void Fuchin_EndRevealLighting(void) {
  struct BattleFxScene *scene;
  struct DisplayTransitionState *transition;

  if (gGameState.entrance <= 6) {
    scene = *(gWorkSlot + 56);
    transition = *(gWorkSlot + 31);
    scene->childOption = 1;
    transition->nextMode = 0;
    transition->duration = 1;
    transition->step = 1;
    Engine_ColorBufferApplySource(0, 1);
    Engine_ColorBufferApplyTarget(0x203108, 1);
    Engine_ColorBufferInterpolate(16);
    Engine_TaskWait(16);
  }
}

/// Turns the actor sliding on Imil Falls Cave's ice gradually toward the held
/// direction.
static void Imil_TurnSkaterTowardHeldDirection(union FieldObject *object) {
  struct FieldActor *actor = &object->actor;
  s32 timer;
  s32 direction;
  u16 facing;

  timer = actor->timer;
  if (timer != 0) {
    actor->timer--;
    return;
  }
  actor->steeringFlags = 0;
  direction = gImilFuchinFalls_KeyHeadings[(gKeysHeld >> 4) & 15];
  if (direction == -1) {
    Object_SetMode(actor, 9);
    return;
  }
  facing = actor->facing;
  direction = (s16)(direction - facing);
  if (direction > 0x1000)
    direction = 0x1000;
  if (direction < -0x1000)
    direction = -0x1000;
  actor->facing = facing + direction;
  Object_SetMode(actor, 2);
  ObjectDispatch_SetAnimationSpeed((struct DispatchObject *)actor,
                                   IMIL_SLIDE_ANIMATION_STEP);
}

/// Slides along Imil Falls Cave's ice until terrain or a collision boundary
/// ends the move.
static void Imil_SlideActorAcrossIce(void) {
  struct FieldActor *actor;
  struct FieldPosition probe;
  s32 heading;
  s32 startCollision;
  s32 nextCollision;
  s32 z;
  s32 x;

  actor = Engine_ActorLookup(gGameState.selectedActor);

  for (;;) {
    heading = gImilFuchinFalls_SlideHeadings[(gKeysHeld >> 4) & 15];
    if (((u32)heading << 16) == 0xffff0000) {
      return;
    }

    Engine_EventBegin();

    // Start at the cell center and reject a blocked or raised next cell.
    probe.x = (actor->x.fixed & 0xfff00000) + PIXELS(8);
    probe.y = actor->y.fixed;
    probe.z = (actor->z.fixed & 0xfff00000) + PIXELS(8);
    z = probe.z;
    x = probe.x;
    startCollision = GetMapCellCollision(actor->terrainId, x, z);
    FieldVector_AddPolarOffset(PIXELS(16), heading, &probe);

    nextCollision = GetMapCellCollision(actor->terrainId, probe.x, probe.z);
    if (nextCollision == 255 ||
        Map_GetTerrainHeight(actor->terrainId, probe.x, probe.z) -
                actor->y.fixed >
            PIXELS(8)) {
      actor->facing = heading;
      goto tail;
    }

    probe.x = x;
    probe.z = z;
    actor->speed = PIXELS(2);
    actor->acceleration = PIXELS(1) / 10;
    actor->timer = 0;
    Engine_ObjectSetPosition(actor, x, actor->y.fixed, z);

    Object_SetMode(actor, 2);
    ObjectDispatch_SetAnimationSpeed((struct DispatchObject *)actor,
                                     IMIL_SLIDE_ANIMATION_STEP);
    FieldObject_WaitForMove(actor);
    actor->update = Imil_TurnSkaterTowardHeldDirection;

    // Keep sliding through cells with the same collision kind.
    for (;;) {
      FieldVector_AddPolarOffset(PIXELS(16), heading, &probe);
      nextCollision = GetMapCellCollision(actor->terrainId, probe.x, probe.z);
      if (nextCollision == 255) {
        break;
      }
      if (Map_GetTerrainHeight(actor->terrainId, probe.x, probe.z) -
              actor->y.fixed >
          PIXELS(8)) {
        break;
      }
      x = probe.x;
      z = probe.z;
      actor->speed = PIXELS(2);
      actor->acceleration = PIXELS(1) / 10;
      Engine_ObjectSetPosition(actor, probe.x, probe.y, probe.z);
      FieldObject_WaitForMove(actor);
      if (nextCollision != startCollision) {
        goto blocked;
      }
    }

    actor->speed = PIXELS(2);
    actor->acceleration = PIXELS(1);
    Engine_ObjectSetPosition(actor, x, actor->y.fixed, z);
    FieldObject_WaitForMove(actor);
    Engine_TaskWait(2);
  }

blocked:
  // A collision-kind change ends the slide and restores normal steering.
  actor->update = NULL;
  actor->steeringFlags |= ACTOR_FACES_MOTION;

  actor->acceleration = PIXELS(1) / 4;

tail:
  Engine_TaskWait(10);
  Engine_EventEnd();
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

struct FieldActor *gImilFuchinFalls_DragonsEye = NULL;

struct SceneEntrance gImilFuchinFalls_NoEntrances[] = {
    { 0, CONDITION_ALWAYS, 256, 0, 600, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gImilFuchinFalls_ImilFallsCaveEntrances[] = {
    { 0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { IMIL_FALLS_CAVE_FROM_IMIL, CONDITION_ALWAYS, 264, 0, 289, FACING_NORTH, 0, 135, 62, 376, 328, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gImilFuchinFalls_FirstRoomEntrances[] = {
    { 0, CONDITION_ALWAYS, 248, 0, 600, FACING_SOUTH, 0, 64, 64, 416, 656, 0 },
    { FUCHIN_FALLS_CAVE_ENTRANCE_FROM_FUCHIN_TEMPLE_GROUNDS, CONDITION_ALWAYS, 248, 0, 632, FACING_NORTH, 0, 64, 368, 416, 656, 0 },
    { FUCHIN_FALLS_CAVE_ENTRANCE_FROM_FUCHIN_FALLS_CAVE_ZEPHYR, CONDITION_ALWAYS, 376, 0, 488, FACING_SOUTH, 0, 64, 368, 416, 656, 0 },
    { FUCHIN_FALLS_CAVE_ENTRANCE_FROM_FUCHIN_FALLS_CAVE_ARCTIC_BLADE, CONDITION_ALWAYS, 120, 0, 488, FACING_SOUTH, 0, 64, 368, 416, 656, 0 },
    { 4, CONDITION_ALWAYS, 232, 0, 408, FACING_SOUTH, 0, 64, 368, 416, 656, 0 },
    { 5, CONDITION_ALWAYS, 232, 0, 344, FACING_NORTH, 0, 64, 48, 416, 376, 0 },
    { FUCHIN_FALLS_CAVE_ENTRANCE_FROM_FUCHIN_FALLS_CAVE_DRAGONS_EYE, CONDITION_ALWAYS, 376, 0, 152, FACING_NORTH, 0, 64, 48, 416, 376, 0 },
    { FUCHIN_FALLS_CAVE_ENTRANCE_FROM_FUCHIN_FALLS_CAVE_DRAGON_STATUE_WEST, CONDITION_ALWAYS, 120, 0, 200, FACING_SOUTH, 0, 64, 48, 416, 376, 0 },
    { FUCHIN_FALLS_CAVE_ENTRANCE_FROM_FUCHIN_FALLS_CAVE_DRAGON_STATUE_EAST, CONDITION_ALWAYS, 248, 0, 104, FACING_SOUTH, 0, 64, 48, 416, 376, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gImilFuchinFalls_SecondRoomEntrances[] = {
    { FUCHIN_FALLS_CAVE_ZEPHYR_FROM_FUCHIN_FALLS_CAVE_DRAGONS_EYE, CONDITION_ALWAYS, 248, 0, 120, FACING_SOUTH, 0, 96, 64, 416, 480, 0 },
    { FUCHIN_FALLS_CAVE_ZEPHYR_FROM_FUCHIN_FALLS_CAVE_ENTRANCE, CONDITION_ALWAYS, 152, 0, 200, FACING_NORTH, 0, 96, 64, 416, 480, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gImilFuchinFalls_ThirdRoomEntrances[] = {
    { 1, CONDITION_ALWAYS, 248, 0, 104, FACING_SOUTH, 0, 128, 64, 368, 256, 0 },
    { FUCHIN_FALLS_CAVE_DRAGONS_EYE_FROM_FUCHIN_FALLS_CAVE_DRAGON_STATUE, CONDITION_ALWAYS, 184, 0, 328, FACING_SOUTH, 0, 96, 256, 416, 656, 0 },
    { 3, CONDITION_ALWAYS, 152, 0, 432, FACING_SOUTH, 0, 96, 256, 416, 656, 0 },
    { FUCHIN_FALLS_CAVE_DRAGONS_EYE_FROM_FUCHIN_FALLS_CAVE_ENTRANCE, CONDITION_ALWAYS, 184, 0, 552, FACING_SOUTH, 0, 96, 256, 416, 656, 0 },
    { FUCHIN_FALLS_CAVE_DRAGONS_EYE_FROM_FUCHIN_FALLS_CAVE_ZEPHYR, CONDITION_ALWAYS, 280, 0, 616, FACING_NORTH, 0, 96, 256, 416, 656, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gImilFuchinFalls_FourthRoomEntrances[] = {
    { FUCHIN_FALLS_CAVE_ARCTIC_BLADE_FROM_FUCHIN_FALLS_CAVE_DRAGON_STATUE, CONDITION_ALWAYS, 328, 0, 72, FACING_SOUTH, 0, 64, 16, 416, 416, 0 },
    { 2, CONDITION_ALWAYS, 248, 0, 392, FACING_NORTH, 0, 64, 16, 416, 416, 0 },
    { 3, CONDITION_ALWAYS, 248, 0, 472, FACING_SOUTH, 0, 64, 416, 416, 800, 0 },
    { FUCHIN_FALLS_CAVE_ARCTIC_BLADE_FROM_FUCHIN_FALLS_CAVE_ENTRANCE, CONDITION_ALWAYS, 344, 0, 632, FACING_NORTH, 0, 64, 416, 416, 800, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gImilFuchinFalls_KiRoomEntrances[] = {
    { 1, CONDITION_ALWAYS, 264, 0, 232, FACING_NORTH, 0, 128, 16, 384, 256, 0 },
    { 2, CONDITION_ALWAYS, 248, 0, 472, FACING_SOUTH, 0, 64, 256, 416, 720, 0 },
    { FUCHIN_FALLS_CAVE_ORB_OF_FORCE_FROM_FUCHIN_FALLS_CAVE_DRAGON_STATUE_WEST, CONDITION_ALWAYS, 216, 0, 664, FACING_NORTH, 0, 64, 256, 416, 720, 0 },
    { FUCHIN_FALLS_CAVE_ORB_OF_FORCE_FROM_FUCHIN_FALLS_CAVE_DRAGON_STATUE_EAST, CONDITION_ALWAYS, 280, 0, 664, FACING_NORTH, 0, 64, 256, 416, 720, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gImilFuchinFalls_DragonRoomEntrances[] = {
    { FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_ORB_OF_FORCE_EAST, CONDITION_ALWAYS, 280, 0, 120, FACING_SOUTH, 0, 64, 0, 416, 344, 0 },
    { FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_ORB_OF_FORCE_WEST, CONDITION_ALWAYS, 216, 0, 120, FACING_SOUTH, 0, 64, 0, 416, 344, 0 },
    { FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_ARCTIC_BLADE, CONDITION_ALWAYS, 104, 0, 280, FACING_SOUTH, 0, 64, 0, 416, 344, 0 },
    { 4, CONDITION_ALWAYS, 200, 0, 344, FACING_NORTH, 0, 64, 16, 416, 344, 0 },
    { 5, CONDITION_ALWAYS, 248, 0, 312, FACING_NORTH, 0, 64, 16, 416, 344, 0 },
    { 6, CONDITION_ALWAYS, 296, 0, 344, FACING_NORTH, 0, 64, 16, 416, 344, 0 },
    { 7, CONDITION_ALWAYS, 200, 0, 440, FACING_SOUTH, 0, 32, 344, 432, 736, 0 },
    { 8, CONDITION_ALWAYS, 248, 0, 440, FACING_SOUTH, 0, 32, 344, 432, 736, 0 },
    { 9, CONDITION_ALWAYS, 296, 0, 424, FACING_SOUTH, 0, 32, 344, 432, 736, 0 },
    { FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_DRAGONS_EYE, CONDITION_ALWAYS, 376, 0, 472, FACING_SOUTH, 0, 32, 344, 432, 736, 0 },
    { FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_ENTRANCE_EAST, CONDITION_ALWAYS, 248, 0, 536, FACING_NORTH, 0, 32, 344, 432, 736, 0 },
    { FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_ENTRANCE_WEST, CONDITION_ALWAYS, 120, 0, 696, FACING_SOUTH, 0, 32, 344, 432, 752, 0 },
    { SCENE_TABLE_END },
};

u32 gImilFuchinFalls_Exits[] = {
    SCENE_EXITS(ImilFallsCave),
    SCENE_EXIT(IMIL_FALLS_CAVE_TO_IMIL, Imil, IMIL_FROM_IMIL_FALLS_CAVE),
    SCENE_EXITS(FuchinFallsCaveEntrance),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_ENTRANCE_TO_FUCHIN_TEMPLE_GROUNDS, FuchinTempleGrounds, FUCHIN_TEMPLE_GROUNDS_FROM_FUCHIN_FALLS_CAVE_ENTRANCE),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_ENTRANCE_TO_FUCHIN_FALLS_CAVE_ZEPHYR, FuchinFallsCaveZephyr, FUCHIN_FALLS_CAVE_ZEPHYR_FROM_FUCHIN_FALLS_CAVE_ENTRANCE),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_ENTRANCE_TO_FUCHIN_FALLS_CAVE_ARCTIC_BLADE, FuchinFallsCaveArcticBlade, FUCHIN_FALLS_CAVE_ARCTIC_BLADE_FROM_FUCHIN_FALLS_CAVE_ENTRANCE),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_ENTRANCE_SOUTH_HALF_TO_NORTH_HALF, FuchinFallsCaveEntrance, FUCHIN_FALLS_CAVE_ENTRANCE_NORTH_HALF_FROM_SOUTH_HALF),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_ENTRANCE_NORTH_HALF_TO_SOUTH_HALF, FuchinFallsCaveEntrance, FUCHIN_FALLS_CAVE_ENTRANCE_SOUTH_HALF_FROM_NORTH_HALF),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_ENTRANCE_TO_FUCHIN_FALLS_CAVE_DRAGONS_EYE, FuchinFallsCaveDragonsEye, FUCHIN_FALLS_CAVE_DRAGONS_EYE_FROM_FUCHIN_FALLS_CAVE_ENTRANCE),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_ENTRANCE_TO_FUCHIN_FALLS_CAVE_DRAGON_STATUE_WEST, FuchinFallsCaveDragonStatue, FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_ENTRANCE_WEST),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_ENTRANCE_TO_FUCHIN_FALLS_CAVE_DRAGON_STATUE_EAST, FuchinFallsCaveDragonStatue, FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_ENTRANCE_EAST),
    SCENE_EXITS(FuchinFallsCaveZephyr),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_ZEPHYR_TO_FUCHIN_FALLS_CAVE_DRAGONS_EYE, FuchinFallsCaveDragonsEye, FUCHIN_FALLS_CAVE_DRAGONS_EYE_FROM_FUCHIN_FALLS_CAVE_ZEPHYR),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_ZEPHYR_TO_FUCHIN_FALLS_CAVE_ENTRANCE, FuchinFallsCaveEntrance, FUCHIN_FALLS_CAVE_ENTRANCE_FROM_FUCHIN_FALLS_CAVE_ZEPHYR),
    SCENE_EXITS(FuchinFallsCaveDragonsEye),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_DRAGONS_EYE_PEDESTAL_TO_LOGS, FuchinFallsCaveDragonsEye, FUCHIN_FALLS_CAVE_DRAGONS_EYE_LOGS_FROM_PEDESTAL),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_DRAGONS_EYE_TO_FUCHIN_FALLS_CAVE_DRAGON_STATUE, FuchinFallsCaveDragonStatue, FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_DRAGONS_EYE),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_DRAGONS_EYE_LOGS_TO_PEDESTAL, FuchinFallsCaveDragonsEye, FUCHIN_FALLS_CAVE_DRAGONS_EYE_PEDESTAL_FROM_LOGS),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_DRAGONS_EYE_TO_FUCHIN_FALLS_CAVE_ENTRANCE, FuchinFallsCaveEntrance, FUCHIN_FALLS_CAVE_ENTRANCE_FROM_FUCHIN_FALLS_CAVE_DRAGONS_EYE),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_DRAGONS_EYE_TO_FUCHIN_FALLS_CAVE_ZEPHYR, FuchinFallsCaveZephyr, FUCHIN_FALLS_CAVE_ZEPHYR_FROM_FUCHIN_FALLS_CAVE_DRAGONS_EYE),
    SCENE_EXITS(FuchinFallsCaveArcticBlade),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_ARCTIC_BLADE_TO_FUCHIN_FALLS_CAVE_DRAGON_STATUE, FuchinFallsCaveDragonStatue, FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_ARCTIC_BLADE),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_ARCTIC_BLADE_NORTH_HALF_TO_SOUTH_HALF, FuchinFallsCaveArcticBlade, FUCHIN_FALLS_CAVE_ARCTIC_BLADE_SOUTH_HALF_FROM_NORTH_HALF),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_ARCTIC_BLADE_SOUTH_HALF_TO_NORTH_HALF, FuchinFallsCaveArcticBlade, FUCHIN_FALLS_CAVE_ARCTIC_BLADE_NORTH_HALF_FROM_SOUTH_HALF),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_ARCTIC_BLADE_TO_FUCHIN_FALLS_CAVE_ENTRANCE, FuchinFallsCaveEntrance, FUCHIN_FALLS_CAVE_ENTRANCE_FROM_FUCHIN_FALLS_CAVE_ARCTIC_BLADE),
    SCENE_EXITS(FuchinFallsCaveOrbOfForce),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_ORB_OF_FORCE_NORTH_HALF_TO_SOUTH_HALF, FuchinFallsCaveOrbOfForce, FUCHIN_FALLS_CAVE_ORB_OF_FORCE_SOUTH_HALF_FROM_NORTH_HALF),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_ORB_OF_FORCE_SOUTH_HALF_TO_NORTH_HALF, FuchinFallsCaveOrbOfForce, FUCHIN_FALLS_CAVE_ORB_OF_FORCE_NORTH_HALF_FROM_SOUTH_HALF),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_ORB_OF_FORCE_TO_FUCHIN_FALLS_CAVE_DRAGON_STATUE_WEST, FuchinFallsCaveDragonStatue, FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_ORB_OF_FORCE_WEST),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_ORB_OF_FORCE_TO_FUCHIN_FALLS_CAVE_DRAGON_STATUE_EAST, FuchinFallsCaveDragonStatue, FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_ORB_OF_FORCE_EAST),
    SCENE_EXITS(FuchinFallsCaveDragonStatue),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_DRAGON_STATUE_TO_FUCHIN_FALLS_CAVE_ORB_OF_FORCE_WEST, FuchinFallsCaveOrbOfForce, FUCHIN_FALLS_CAVE_ORB_OF_FORCE_FROM_FUCHIN_FALLS_CAVE_DRAGON_STATUE_WEST),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_DRAGON_STATUE_TO_FUCHIN_FALLS_CAVE_ORB_OF_FORCE_EAST, FuchinFallsCaveOrbOfForce, FUCHIN_FALLS_CAVE_ORB_OF_FORCE_FROM_FUCHIN_FALLS_CAVE_DRAGON_STATUE_EAST),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_DRAGON_STATUE_TO_FUCHIN_FALLS_CAVE_ARCTIC_BLADE, FuchinFallsCaveArcticBlade, FUCHIN_FALLS_CAVE_ARCTIC_BLADE_FROM_FUCHIN_FALLS_CAVE_DRAGON_STATUE),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_TO_SOUTH_HALF_WEST, FuchinFallsCaveDragonStatue, FUCHIN_FALLS_CAVE_DRAGON_STATUE_SOUTH_HALF_FROM_NORTH_HALF_WEST),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_TO_SOUTH_HALF_MIDDLE, FuchinFallsCaveDragonStatue, FUCHIN_FALLS_CAVE_DRAGON_STATUE_SOUTH_HALF_FROM_NORTH_HALF_MIDDLE),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_TO_SOUTH_HALF_EAST, FuchinFallsCaveDragonStatue, FUCHIN_FALLS_CAVE_DRAGON_STATUE_SOUTH_HALF_FROM_NORTH_HALF_EAST),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_DRAGON_STATUE_SOUTH_HALF_TO_NORTH_HALF_WEST, FuchinFallsCaveDragonStatue, FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_FROM_SOUTH_HALF_WEST),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_DRAGON_STATUE_SOUTH_HALF_TO_NORTH_HALF_MIDDLE, FuchinFallsCaveDragonStatue, FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_FROM_SOUTH_HALF_MIDDLE),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_DRAGON_STATUE_SOUTH_HALF_TO_NORTH_HALF_EAST, FuchinFallsCaveDragonStatue, FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_FROM_SOUTH_HALF_EAST),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_DRAGON_STATUE_TO_FUCHIN_FALLS_CAVE_DRAGONS_EYE, FuchinFallsCaveDragonsEye, FUCHIN_FALLS_CAVE_DRAGONS_EYE_FROM_FUCHIN_FALLS_CAVE_DRAGON_STATUE),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_DRAGON_STATUE_TO_FUCHIN_FALLS_CAVE_ENTRANCE_EAST, FuchinFallsCaveEntrance, FUCHIN_FALLS_CAVE_ENTRANCE_FROM_FUCHIN_FALLS_CAVE_DRAGON_STATUE_EAST),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_DRAGON_STATUE_TO_FUCHIN_FALLS_CAVE_ENTRANCE_WEST, FuchinFallsCaveEntrance, FUCHIN_FALLS_CAVE_ENTRANCE_FROM_FUCHIN_FALLS_CAVE_DRAGON_STATUE_WEST),
    SCENE_EXITS_END,
};

struct ScenePlacement gImilFuchinFalls_NoPlacements[] = {
    { SCENE_TABLE_END },
};

struct ScenePlacement gImilFuchinFalls_ImilFallsCavePlacements[] = {
    { 0x5c, 0x59, ACTOR_STAND,
        PIXELS(263), 0, PIXELS(204), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gImilFuchinFalls_FirstRoomPlacements[] = {
    { 0xed, CONDITION_ALWAYS, 7,
        PIXELS(136), 0, PIXELS(504), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xec, CONDITION_ALWAYS, 7,
        PIXELS(200), 0, PIXELS(536), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xec, CONDITION_ALWAYS, 7,
        PIXELS(296), 0, PIXELS(536), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gImilFuchinFalls_SecondRoomPlacements[] = {
    { 0xed, CONDITION_ALWAYS, 7,
        PIXELS(216), 0, PIXELS(328), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xec, CONDITION_ALWAYS, 7,
        PIXELS(248), 0, PIXELS(280), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xec, CONDITION_ALWAYS, 7,
        PIXELS(312), 0, PIXELS(408), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x5d, 0x6e, ACTOR_STAND,
        PIXELS(344), 0, PIXELS(280), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gImilFuchinFalls_ThirdRoomPlacements[] = {
    { 0xed, CONDITION_ALWAYS, 7,
        PIXELS(328), 0, PIXELS(408), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xed, CONDITION_ALWAYS, 7,
        PIXELS(248), 0, PIXELS(568), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xec, CONDITION_ALWAYS, 7,
        PIXELS(248), 0, PIXELS(488), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xec, CONDITION_ALWAYS, 7,
        PIXELS(248), 0, PIXELS(504), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gImilFuchinFalls_FourthRoomPlacements[] = {
    { 0xec, CONDITION_ALWAYS, 7,
        PIXELS(328), 0, PIXELS(168), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xed, CONDITION_ALWAYS, 7,
        PIXELS(360), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xed, CONDITION_ALWAYS, 7,
        PIXELS(120), 0, PIXELS(744), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gImilFuchinFalls_DragonRoomPlacements[] = {
    { 0xdf, CONDITION_ALWAYS, 7,
        PIXELS(312), 0, PIXELS(128), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gImilFuchinFalls_NoEvents[] = {
    { SCENE_EVENTS_END },
};

struct SceneEvent gImilFuchinFalls_ImilFallsCaveEvents[] = {
    { EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(Imil_LeaveFallsCave) },
    { EVENT_TOUCH, 10, CONDITION_ALWAYS, EVENT_SCRIPT(Imil_MoveAcrossIce) },
    { EVENT_TALK, ACTOR_FEVER, CONDITION_ALWAYS, EVENT_SCRIPT(Imil_RecruitFever) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gImilFuchinFalls_FirstRoomEvents[] = {
    { TOUCH_WALKING_INTO(FACING_EAST), 11, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_FirstRoom_MoveFirstLogEast) },
    { TOUCH_WALKING_INTO(FACING_WEST), 12, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_FirstRoom_MoveFirstLogWest) },
    { EVENT_TOUCH, 11, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_AnimateFirstLog) },
    { EVENT_TOUCH, 12, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_AnimateFirstLog) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 13, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_FirstRoom_MoveSecondLogSouth) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 14, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_FirstRoom_MoveSecondLogNorth) },
    { EVENT_TOUCH, 13, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_AnimateSecondLog) },
    { EVENT_TOUCH, 14, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_AnimateSecondLog) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 15, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_FirstRoom_MoveThirdLogSouth) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 16, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_FirstRoom_MoveThirdLogNorth) },
    { EVENT_TOUCH, 15, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_AnimateThirdLog) },
    { EVENT_TOUCH, 16, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_AnimateThirdLog) },
    { EVENT_EXIT, 21, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_ENTRANCE_TO_FUCHIN_TEMPLE_GROUNDS },
    { EVENT_EXIT, 22, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_ENTRANCE_TO_FUCHIN_FALLS_CAVE_ZEPHYR },
    { EVENT_EXIT, 23, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_ENTRANCE_TO_FUCHIN_FALLS_CAVE_ARCTIC_BLADE },
    { EVENT_EXIT, 24, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_ENTRANCE_SOUTH_HALF_TO_NORTH_HALF },
    { EVENT_EXIT, 25, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_ENTRANCE_NORTH_HALF_TO_SOUTH_HALF },
    { EVENT_EXIT, 26, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_ENTRANCE_TO_FUCHIN_FALLS_CAVE_DRAGONS_EYE },
    { 0x21, 27, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_ENTRANCE_TO_FUCHIN_FALLS_CAVE_DRAGON_STATUE_WEST },
    { EVENT_EXIT, 28, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_ENTRANCE_TO_FUCHIN_FALLS_CAVE_DRAGON_STATUE_EAST },
    { SEARCH(SEARCH_UNNAMED), 1, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_ReadDragonFlameInscription) },
    { SEARCH(SEARCH_CHEST), 100, 0xef1, 0x500001 },
    { SCENE_EVENTS_END },
};

struct SceneEvent gImilFuchinFalls_SecondRoomEvents[] = {
    { TOUCH_WALKING_INTO(FACING_EAST), 11, FLAG_PLACE_TEMP_06, EVENT_SCRIPT(Fuchin_SecondRoom_MoveFirstLogToEastEnd) },
    { TOUCH_WALKING_INTO(FACING_WEST), 11, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_SecondRoom_NudgeFirstLogWest) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 17, CONDITION_ALWAYS, EVENT_SCRIPT(ImilFuchinFalls_HopOnTrigger) },
    { TOUCH_WALKING_INTO(FACING_EAST), 17, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_SecondRoom_MoveFirstLogEast) },
    { TOUCH_WALKING_INTO(FACING_WEST), 12, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_SecondRoom_MoveFirstLogToWestEnd) },
    { EVENT_TOUCH, 11, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_AnimateFirstLog) },
    { EVENT_TOUCH, 12, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_AnimateFirstLog) },
    { EVENT_TOUCH, 17, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_AnimateFirstLog) },
    { TOUCH_WALKING_INTO(FACING_WEST), 13, CONDITION_ALWAYS, EVENT_SCRIPT(ImilFuchinFalls_HopOnTrigger) },
    { TOUCH_WALKING_INTO(FACING_WEST), 14, FLAG_PLACE_TEMP_04, EVENT_SCRIPT(ImilFuchinFalls_HopOnTrigger) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 13, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_SecondRoom_MoveSecondLogSouth) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 14, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_SecondRoom_MoveSecondLogNorth) },
    { EVENT_TOUCH, 13, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_AnimateSecondLog) },
    { EVENT_TOUCH, 14, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_AnimateSecondLog) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 15, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_SecondRoom_MoveThirdLogSouth) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 16, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_SecondRoom_MoveThirdLogNorth) },
    { EVENT_TOUCH, 15, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_AnimateThirdLog) },
    { EVENT_TOUCH, 16, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_AnimateThirdLog) },
    { EVENT_EXIT, 1, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_ZEPHYR_TO_FUCHIN_FALLS_CAVE_DRAGONS_EYE },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_ZEPHYR_TO_FUCHIN_FALLS_CAVE_ENTRANCE },
    { EVENT_TALK, ACTOR_ZEPHYR, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_FightZephyr) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gImilFuchinFalls_ThirdRoomEvents[] = {
    { TOUCH_WALKING_INTO(FACING_WEST), 10, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_ThirdRoom_MoveFirstLogWest) },
    { TOUCH_WALKING_INTO(FACING_EAST), 11, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_ThirdRoom_MoveFirstLogEastFromMiddle) },
    { TOUCH_WALKING_INTO(FACING_WEST), 11, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_ThirdRoom_MoveFirstLogWestIfClear) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 11, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_HopNorth) },
    { TOUCH_WALKING_INTO(FACING_EAST), 12, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_ThirdRoom_MoveFirstLogEastFromWestEnd) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 12, IF_FLAG_SET(FLAG_PLACE_TEMP_0B), EVENT_SCRIPT(Fuchin_HopNorth) },
    { EVENT_TOUCH, 10, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_AnimateFirstLog) },
    { EVENT_TOUCH, 11, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_AnimateFirstLog) },
    { EVENT_TOUCH, 12, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_AnimateFirstLog) },
    { TOUCH_WALKING_INTO(FACING_WEST), 14, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_ThirdRoom_MoveSecondLogWest) },
    { TOUCH_WALKING_INTO(FACING_EAST), 13, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_ThirdRoom_MoveSecondLogEast) },
    { EVENT_TOUCH, 13, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_AnimateSecondLog) },
    { EVENT_TOUCH, 14, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_AnimateSecondLog) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 15, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_ThirdRoom_MoveThirdLogNorthFromSouthEnd) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 16, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_ThirdRoom_MoveThirdLogSouthFromNorthEnd) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 17, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_ThirdRoom_NudgeThirdLogNorth) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 17, FLAG_PLACE_TEMP_08, EVENT_SCRIPT(Fuchin_ThirdRoom_MoveThirdLogSouthFromUpperStop) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 18, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_ThirdRoom_MoveThirdLogNorthFromMiddle) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 18, FLAG_PLACE_TEMP_10, EVENT_SCRIPT(Fuchin_ThirdRoom_MoveThirdLogSouthFromMiddle) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 19, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_ThirdRoom_MoveThirdLogNorthFromLowerStop) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 19, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_ThirdRoom_MoveThirdLogSouthFromLowerStop) },
    { EVENT_TOUCH, 15, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_AnimateThirdLog) },
    { EVENT_TOUCH, 16, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_AnimateThirdLog) },
    { EVENT_TOUCH, 17, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_AnimateThirdLog) },
    { EVENT_TOUCH, 18, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_AnimateThirdLog) },
    { EVENT_TOUCH, 19, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_AnimateThirdLog) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 20, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_ThirdRoom_MoveFourthLogNorth) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 21, FLAG_PLACE_TEMP_08, EVENT_SCRIPT(Fuchin_ThirdRoom_MoveFourthLogSouthFromNorthEnd) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 22, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_ThirdRoom_NudgeFourthLogNorth) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 22, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_ThirdRoom_MoveFourthLogSouthFromUpperStop) },
    { TOUCH_WALKING_INTO(FACING_WEST), 23, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_HopWest) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 23, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_ThirdRoom_MoveFourthLogSouthFromMiddle) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 24, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_ThirdRoom_MoveFourthLogSouthFromLowerStop) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 24, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_ThirdRoom_MoveFourthLogSouthFromLastStop) },
    { EVENT_TOUCH, 20, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_AnimateFourthLog) },
    { EVENT_TOUCH, 21, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_AnimateFourthLog) },
    { EVENT_TOUCH, 22, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_AnimateFourthLog) },
    { EVENT_TOUCH, 23, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_AnimateFourthLog) },
    { EVENT_TOUCH, 24, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_AnimateFourthLog) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 25, CONDITION_ALWAYS, EVENT_SCRIPT(ImilFuchinFalls_HopOnTrigger) },
    { 0x21, 1, CONDITION_ALWAYS, 1 },
    { 0x21, 2, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_DRAGONS_EYE_TO_FUCHIN_FALLS_CAVE_DRAGON_STATUE },
    { 0x31, 3, CONDITION_ALWAYS, 3 },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_DRAGONS_EYE_TO_FUCHIN_FALLS_CAVE_ENTRANCE },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_DRAGONS_EYE_TO_FUCHIN_FALLS_CAVE_ZEPHYR },
    { SEARCH(SEARCH_UNNAMED), 30, 0xf13, EVENT_SCRIPT(Fuchin_CollectDragonsEye) },
    { SCENE_EVENTS_END },
};
// clang-format on

struct SceneEvent gImilFuchinFalls_FourthRoomEvents[] = {
    {TOUCH_WALKING_INTO(FACING_SOUTH), 11, CONDITION_ALWAYS,
     EVENT_SCRIPT(Fuchin_FourthRoom_MoveFirstLogSouth)},
    {TOUCH_WALKING_INTO(FACING_NORTH), 12, CONDITION_ALWAYS,
     EVENT_SCRIPT(Fuchin_FourthRoom_MoveFirstLogNorth)},
    {EVENT_TOUCH, 11, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_AnimateFirstLog)},
    {EVENT_TOUCH, 12, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_AnimateFirstLog)},
    {TOUCH_WALKING_INTO(FACING_WEST), 13, FLAG_PLACE_TEMP_13,
     EVENT_SCRIPT(Fuchin_FourthRoom_MoveSecondLogWest)},
    {TOUCH_WALKING_INTO(FACING_EAST), 14, CONDITION_ALWAYS,
     EVENT_SCRIPT(Fuchin_FourthRoom_MoveSecondLogEast)},
    {EVENT_TOUCH, 13, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_AnimateSecondLog)},
    {EVENT_TOUCH, 14, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_AnimateSecondLog)},
    {TOUCH_WALKING_INTO(FACING_EAST), 15, CONDITION_ALWAYS,
     EVENT_SCRIPT(Fuchin_FourthRoom_MoveThirdLogEast)},
    {TOUCH_WALKING_INTO(FACING_WEST), 16, CONDITION_ALWAYS,
     EVENT_SCRIPT(Fuchin_FourthRoom_MoveThirdLogWest)},
    {EVENT_TOUCH, 15, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_AnimateThirdLog)},
    {EVENT_TOUCH, 16, CONDITION_ALWAYS, EVENT_SCRIPT(Fuchin_AnimateThirdLog)},
    {0x21, 1, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_ARCTIC_BLADE_TO_FUCHIN_FALLS_CAVE_DRAGON_STATUE},
    {EVENT_EXIT, 2, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_ARCTIC_BLADE_NORTH_HALF_TO_SOUTH_HALF},
    {EVENT_EXIT, 3, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_ARCTIC_BLADE_SOUTH_HALF_TO_NORTH_HALF},
    {EVENT_EXIT, 4, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_ARCTIC_BLADE_TO_FUCHIN_FALLS_CAVE_ENTRANCE},
    {SEARCH(SEARCH_CHEST), 100, 0xf6b, FIND_ITEM(ITEM_ARCTIC_BLADE)},
    {SCENE_EVENTS_END},
};

struct SceneEvent gImilFuchinFalls_KiRoomEvents[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_ORB_OF_FORCE_NORTH_HALF_TO_SOUTH_HALF},
    {EVENT_EXIT, 2, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_ORB_OF_FORCE_SOUTH_HALF_TO_NORTH_HALF},
    {EVENT_EXIT, 3, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_ORB_OF_FORCE_TO_FUCHIN_FALLS_CAVE_DRAGON_STATUE_WEST},
    {EVENT_EXIT, 4, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_ORB_OF_FORCE_TO_FUCHIN_FALLS_CAVE_DRAGON_STATUE_EAST},
    {SEARCH(SEARCH_UNNAMED), 7, CONDITION_ALWAYS,
     EVENT_SCRIPT(Fuchin_ReadKiInscription)},
    {SEARCH(SEARCH_CHEST), 100, FLAG_FUCHIN_FALLS_ORB_OF_FORCE_FOUND,
     FIND_ITEM(ITEM_ORB_OF_FORCE)},
    {SCENE_EVENTS_END},
};

struct SceneEvent gImilFuchinFalls_DragonRoomEvents[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_DRAGON_STATUE_TO_FUCHIN_FALLS_CAVE_ORB_OF_FORCE_WEST},
    {EVENT_EXIT, 2, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_DRAGON_STATUE_TO_FUCHIN_FALLS_CAVE_ORB_OF_FORCE_EAST},
    {0x31, 3, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_DRAGON_STATUE_TO_FUCHIN_FALLS_CAVE_ARCTIC_BLADE},
    {EVENT_EXIT, 4, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_TO_SOUTH_HALF_WEST},
    {EVENT_EXIT, 5, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_TO_SOUTH_HALF_MIDDLE},
    {EVENT_EXIT, 6, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_TO_SOUTH_HALF_EAST},
    {EVENT_EXIT, 7, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_DRAGON_STATUE_SOUTH_HALF_TO_NORTH_HALF_WEST},
    {EVENT_EXIT, 8, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_DRAGON_STATUE_SOUTH_HALF_TO_NORTH_HALF_MIDDLE},
    {EVENT_EXIT, 9, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_DRAGON_STATUE_SOUTH_HALF_TO_NORTH_HALF_EAST},
    {0x31, 10, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_DRAGON_STATUE_TO_FUCHIN_FALLS_CAVE_DRAGONS_EYE},
    {EVENT_EXIT, 11, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_DRAGON_STATUE_TO_FUCHIN_FALLS_CAVE_ENTRANCE_EAST},
    {0x31, 12, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_DRAGON_STATUE_TO_FUCHIN_FALLS_CAVE_ENTRANCE_WEST},
    {EVENT_TOUCH, 15, CONDITION_ALWAYS,
     EVENT_SCRIPT(Fuchin_FollowLeaderWithLight)},
    {EVENT_TOUCH, 16, CONDITION_ALWAYS,
     EVENT_SCRIPT(Fuchin_StopLightFollowingLeader)},
    {SEARCH(SEARCH_UNNAMED), 19, CONDITION_ALWAYS,
     EVENT_SCRIPT(Fuchin_ReadShadowInscription)},
    {USE_ITEM(ITEM_DRAGONS_EYE), 20, FLAG_FUCHIN_FALLS_DRAGONS_EYE_SET,
     EVENT_SCRIPT(Fuchin_LightDragonWithEye)},
    {SEARCH(SEARCH_UNNAMED), 20, CONDITION_ALWAYS,
     EVENT_SCRIPT(Fuchin_SearchDragonStatue)},
    {PSYNERGY_IN_AREA(ABILITY_REVEAL, 4), 0, FLAG_FUCHIN_FALLS_DRAGONS_EYE_SET,
     EVENT_SCRIPT(Fuchin_EndRevealLighting)},
    {SCENE_EVENTS_END},
};

s32 gImilFuchinFalls_DustScript0[] = {
    SC_WAIT(1), SC_LOOP(22, SC_TOP), SC_CALL(Fuchin_SetDustPalette),
    SC_WAIT(1), SC_POS(0, 0, 0),     SC_WAIT(1),
    SC_FREE,
};

s32 gImilFuchinFalls_DustScript1[] = {
    SC_WAIT(1), SC_LOOP(44, SC_TOP), SC_CALL(Fuchin_SetDustPalette),
    SC_WAIT(1), SC_POS(0, 0, 0),     SC_WAIT(1),
    SC_FREE,
};

s32 gImilFuchinFalls_DustScript2[] = {
    SC_WAIT(1), SC_LOOP(126, SC_TOP), SC_CALL(Fuchin_SetDustPalette),
    SC_WAIT(1), SC_POS(0, 0, 0),      SC_WAIT(1),
    SC_FREE,
};
