/// @file
/// Imil Falls Cave, north of Imil, and Fuchin Falls Cave, the waterfall grotto
/// of Fuchin Temple's test: two areas in seven scenes.
///
/// - Imil Falls Cave (SceneId_ImilFallsCave), up the way north out of Imil; its
///   only exit, IMIL_FALLS_CAVE_TO_IMIL, leads back down
///   (IMIL_FROM_IMIL_FALLS_CAVE). Fever, a Mars Djinni, waits on its ice and
///   joins the party when spoken to, without a battle. Every normal entry
///   blocks Retreat and walks the leader up toward Fever, and stepping onto the
///   ice (cell behavior 10) slides the leader in the held direction,
///   turning toward it as he goes, until a blocked cell or one too high stops
///   him.
/// - Fuchin Falls Cave (SceneId_FuchinFallsCaveEntrance to
///   SceneId_FuchinFallsCaveDragonStatue), the waterfall grotto. Master Nyunpa
///   of Fuchin Temple tells the party they must survive its test before he will
///   tell them how to pass through Mogall Forest. The test ends at the chest of
///   the Orb of Force (FLAG_FUCHIN_FALLS_ORB_OF_FORCE_FOUND), and back at the
///   temple Nyunpa names the power they have gained Ki (fuchin_temple.c). The
///   way in and out is FUCHIN_FALLS_CAVE_ENTRANCE_TO_FUCHIN_TEMPLE_GROUNDS, to
///   FUCHIN_TEMPLE_GROUNDS_FROM_FUCHIN_FALLS_CAVE_ENTRANCE. The code numbers
///   the rooms by scene: the first to fourth rooms
///   (SceneId_FuchinFallsCaveEntrance to SceneId_FuchinFallsCaveArcticBlade),
///   the room of the Ki inscription (SceneId_FuchinFallsCaveOrbOfForce) and the
///   dragon statue's room (SceneId_FuchinFallsCaveDragonStatue).
///
/// The first four rooms are crossed on floating logs
/// (SPRITE_FLOATING_LOG_WEST_EAST and SPRITE_FLOATING_LOG_NORTH_SOUTH, the logs
/// of Colosso's Lumber Water Crossing Area, colosseum_azart_match.c). Walking
/// into a log's step trigger cell pushes it along the water to its next stop,
/// the leader moving with it (Actor_PlacePairAtOffset); touching the cell
/// plays the log's two rolling animations. Each stop is an area temp
/// flag, FLAG_FUCHIN_FALLS_ENTRANCE_FIRST_LOG_EAST to
/// FLAG_FUCHIN_FALLS_ENTRANCE_THIRD_LOG_NORTH for the first room's logs,
/// FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_EAST_END to
/// FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_THIRD_LOG_NORTH the second's,
/// FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_WEST_END to
/// FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_LOWER_STOP the third's and
/// FLAG_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_FIRST_LOG_SOUTH to
/// FLAG_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_THIRD_LOG_EAST the fourth's, so the logs
/// are back where they started once the party leaves the cave's scenes. After
/// each move, and on entry, the room's rebuild function puts every log at the
/// cell its flags name and rewrites the cells of each log's track to match: in
/// the first room (FuchinFallsCave_RebuildEntranceLogs) the track's cells are
/// first copied in from a copy below the room that has each log's step trigger
/// cells (11 and 12, 13 and 14, 15 and 16) at both ends of its run, then the
/// end each log's flag says it is not at is overwritten with a wall cell of the
/// water. Each cell of a track is then a wall, a way out or the push that
/// moves a log. In the third room
/// the logs block one another, so where a log stops depends on where the others
/// lie; in the second and third rooms the leader hops across gaps a log's stop
/// leaves.
///
/// Zephyr, a Jupiter Djinni, waits in the second room
/// (SceneId_FuchinFallsCaveZephyr) and fights when spoken to. In the first room
/// a chest holds a mimic, whose battle its find value starts (FIND_MIMIC); the
/// fourth room's chest holds an Arctic Blade, and the Ki room's the Orb of
/// Force. Arriving in the third room by
/// FUCHIN_FALLS_CAVE_DRAGONS_EYE_CHAMBER_FROM_LOGS before the Dragon's Eye is
/// taken (FLAG_FUCHIN_FALLS_DRAGONS_EYE_FOUND) shows it, and searching there
/// takes it.
///
/// The dragon statue's room is dark until the eye is set: entering its northern
/// half (its first six entrances, up to
/// FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_FROM_SOUTH_HALF_EAST: from the Ki
/// room, from the fourth room or up from the southern half) darkens the
/// palette. Coming in from the Ki room
/// (FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_ORB_OF_FORCE_EAST
/// and _WEST) or up by
/// FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_FROM_SOUTH_HALF_MIDDLE starts the
/// fade overlay (overworld/overworld/fade_overlay.c), which follows the leader
/// except from
/// FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_FROM_SOUTH_HALF_MIDDLE, and
/// step triggers 15 and 16 start and stop its following. When Reveal ends, the
/// room goes dark again. The statue is `an eyeless dragon`; using the Dragon's
/// Eye on it flashes the room, blows dust from the cave mouth as the room
/// brightens, redraws the cells of the hidden path and uses the eye up
/// (FLAG_FUCHIN_FALLS_DRAGONS_EYE_SET). The first room's inscription points the
/// way: `The dragon's flame illuminates the path of truth in darkness.`

#include "script_operands.h"
#include "adventure.h"
#include "keystate.h"
#include "scene_ids.h"
#include "module/import.h"
#include "module/scene/imil_fuchin_falls.h"
#include "module/overworld/overworld.h"
#include "module/overworld/event_commands.h"
#include "module/field/field.h"
#include "module/game/game.h"
#include "battle.h"
#include "event_inline.h"
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
#include "event.h"
#include "scene_tables.h"
#include "djinn_ids.h"
#include "character_ids.h"
#include "call.h"
#include "battle_party.h"
#include "field_effect.h"
#include "scene.h"
#include "display_transition.h"
#include "fx_work.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "actor_script.h"
#include "fixed_math.h"
#include "actor_effect.h"
#include "palbuf.h"
#include "io_reg.h"
#include "flags.h"
#include "sounds.h"
#include "heap.h"
#include "sound_driver_symbols.h"
#include "item_ids.h"
#include "map_grid.h"

/// The placed actors, numbering each scene's placement rows from
/// ACTOR_FIRST_PLACED: Fever in Imil Falls Cave, the floating logs of Fuchin
/// Falls Cave's first four rooms, followed in the second room by Zephyr, and
/// the one sprite the dragon statue's room places.
enum ImilFuchinFallsActorId {
  /// A Mars Djinni, in Imil Falls Cave.
  /// ★ Named from the game: "Fever" (MsgDjinnName)
  ACTOR_FEVER = ACTOR_FIRST_PLACED,
  /// The floating logs: three in the first, second and fourth rooms, four in
  /// the third.
  /// ★ Named from what the player sees: yellow logs floating on the cave's
  /// water (its render)
  ACTOR_FIRST_LOG = ACTOR_FIRST_PLACED,
  /// ★ Named from what the player sees: a floating log (its render)
  ACTOR_SECOND_LOG,
  /// ★ Named from what the player sees: a floating log (its render)
  ACTOR_THIRD_LOG,
  /// ★ Named from what the player sees: a floating log (its render)
  ACTOR_FOURTH_LOG,
  /// Zephyr, a Jupiter Djinni, after the second room's three logs.
  /// ★ Named from the game: "Zephyr" (MsgDjinnName)
  ACTOR_ZEPHYR = ACTOR_FIRST_PLACED + 3,
  /// A crate (SPRITE_CRATE) in the dragon statue's room, the sprite of the
  /// third room's two foreground pieces
  /// (FuchinFallsCave_CreateForegroundActor);
  /// on
  /// entry ImilFuchinFalls_PrepareRoom stills it at floor height and draws it
  /// at sprite priority 1 in SPRITE_COLOR_HIDDEN, as those are drawn, so
  /// every pixel is transparent and the player never sees it.
  ACTOR_INVISIBLE_DRAGON_ROOM_CRATE = ACTOR_FIRST_PLACED
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
  Djinn_JoinParty(ACTOR_FEVER, ELEMENT_MARS, DJINNI_INDEX(DJINNI_FEVER));
}

/// Talking to Zephyr, a Jupiter Djinni, in Fuchin Falls Cave's second room
/// (SceneId_FuchinFallsCaveZephyr) starts its battle.
static void FuchinFallsCave_FightZephyr(void) {
  Field_RequestDjinnBattle(ACTOR_ZEPHYR, DJINNI_ZEPHYR);
}

const struct SceneEntrance *ImilFuchinFalls_GetEntrances(void) {
  s16 sceneId = gAdventure.sceneId;

  if (sceneId == (s32)&SceneId_ImilFallsCave) {
    return gImilFuchinFalls_ImilFallsCaveEntrances;
  }
  if (sceneId == (s32)&SceneId_FuchinFallsCaveEntrance) {
    return gImilFuchinFalls_EntranceEntrances;
  }
  if (sceneId == (s32)&SceneId_FuchinFallsCaveZephyr) {
    return gImilFuchinFalls_ZephyrRoomEntrances;
  }
  if (sceneId == (s32)&SceneId_FuchinFallsCaveDragonsEye) {
    return gImilFuchinFalls_DragonsEyeRoomEntrances;
  }
  if (sceneId == (s32)&SceneId_FuchinFallsCaveArcticBlade) {
    return gImilFuchinFalls_ArcticBladeRoomEntrances;
  }
  if (sceneId == (s32)&SceneId_FuchinFallsCaveOrbOfForce) {
    return gImilFuchinFalls_KiRoomEntrances;
  }
  if (sceneId == (s32)&SceneId_FuchinFallsCaveDragonStatue) {
    return gImilFuchinFalls_DragonRoomEntrances;
  }
  return gImilFuchinFalls_NoEntrances;
}

const struct SceneExitRegion *ImilFuchinFalls_GetRegions(void) { return NULL; }

/// Returns the exit table every scene of the overlay shares.
const u32 *ImilFuchinFalls_GetExits(void) { return gImilFuchinFalls_Exits; }

const struct ActorPlacement *ImilFuchinFalls_GetActorPlacements(void) {
  s16 sceneId = gAdventure.sceneId;

  if (sceneId == (s32)&SceneId_ImilFallsCave) {
    return gImilFuchinFalls_ImilFallsCaveActorPlacements;
  }
  if (sceneId == (s32)&SceneId_FuchinFallsCaveEntrance) {
    return gImilFuchinFalls_EntranceActorPlacements;
  }
  if (sceneId == (s32)&SceneId_FuchinFallsCaveZephyr) {
    return gImilFuchinFalls_ZephyrRoomActorPlacements;
  }
  if (sceneId == (s32)&SceneId_FuchinFallsCaveDragonsEye) {
    return gImilFuchinFalls_DragonsEyeRoomActorPlacements;
  }
  if (sceneId == (s32)&SceneId_FuchinFallsCaveArcticBlade) {
    return gImilFuchinFalls_ArcticBladeRoomActorPlacements;
  }
  if (sceneId == (s32)&SceneId_FuchinFallsCaveDragonStatue) {
    return gImilFuchinFalls_DragonRoomActorPlacements;
  }
  return gImilFuchinFalls_NoActorPlacements;
}

#include "shared/actor_pairmove.h"

/// The first room's first log lies at the east end of its track.
/// ★ Named from what the player sees: golden logs floating on the cave's pool,
/// which float along it when pushed (its render)
#define FLAG_FUCHIN_FALLS_ENTRANCE_FIRST_LOG_EAST FLAG_AREA_TEMP_01
/// The first room's second log lies at the north end of its track.
/// ★ Named from what the player sees: golden logs floating on the cave's pool,
/// which float along it when pushed (its render)
#define FLAG_FUCHIN_FALLS_ENTRANCE_SECOND_LOG_NORTH FLAG_AREA_TEMP_02
/// The first room's third log lies at the north end of its track.
/// ★ Named from what the player sees: golden logs floating on the cave's pool,
/// which float along it when pushed (its render)
#define FLAG_FUCHIN_FALLS_ENTRANCE_THIRD_LOG_NORTH FLAG_AREA_TEMP_03
/// The second room's first log lies at the east end of its track.
#define FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_EAST_END FLAG_AREA_TEMP_04
/// The second room's first log lies at the west end of its track, a cell west
/// of where it starts, unless it has since been pushed to the east end.
#define FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_WEST_END FLAG_AREA_TEMP_05
/// The second room's second log lies at the south end of its track.
/// ★ Named from what the player sees: golden logs floating on the cave's pool,
/// which float along it when pushed (its render)
#define FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_SECOND_LOG_SOUTH FLAG_AREA_TEMP_06
/// The second room's third log lies at the north end of its track.
/// ★ Named from what the player sees: golden logs floating on the cave's pool,
/// which float along it when pushed (its render)
#define FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_THIRD_LOG_NORTH FLAG_AREA_TEMP_07
/// The third room's first log lies at the west end of its track.
#define FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_WEST_END FLAG_AREA_TEMP_08
/// The third room's first log lies at its middle stop, where the third or
/// fourth log blocked it.
/// ★ Named from what the player sees: golden logs floating on the cave's pool,
/// which float along it when pushed (its render)
#define FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_MIDDLE FLAG_AREA_TEMP_09
/// The third room's second log lies at the west end of its track.
/// ★ Named from what the player sees: golden logs floating on the cave's pool,
/// which float along it when pushed (its render)
#define FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_SECOND_LOG_WEST FLAG_AREA_TEMP_0A
/// The third room's third log lies at the north end of its track.
#define FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END FLAG_AREA_TEMP_0B
/// The third room's third log lies at its upper stop, a cell south of the north
/// end.
/// ★ Named from what the player sees: golden logs floating on the cave's pool,
/// which float along it when pushed (its render)
#define FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_UPPER_STOP                \
  FLAG_AREA_TEMP_0C
/// The third room's third log lies at its middle stop.
/// ★ Named from what the player sees: golden logs floating on the cave's pool,
/// which float along it when pushed (its render)
#define FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE FLAG_AREA_TEMP_0D
/// The third room's third log lies at its lower stop; no move sets it.
/// ★ Named from what the player sees: golden logs floating on the cave's pool,
/// which float along it when pushed (its render)
#define FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_LOWER_STOP                \
  FLAG_AREA_TEMP_0E
/// The third room's fourth log lies at the north end of its track.
#define FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_NORTH_END                \
  FLAG_AREA_TEMP_0F
/// The third room's fourth log lies at its upper stop, a cell south of the
/// north end.
/// ★ Named from what the player sees: golden logs floating on the cave's pool,
/// which float along it when pushed (its render)
#define FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_UPPER_STOP               \
  FLAG_AREA_TEMP_10
/// The third room's fourth log lies at its middle stop.
/// ★ Named from what the player sees: golden logs floating on the cave's pool,
/// which float along it when pushed (its render)
#define FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_MIDDLE FLAG_AREA_TEMP_11
/// The third room's fourth log lies at its lower stop; no move sets it.
/// ★ Named from what the player sees: golden logs floating on the cave's pool,
/// which float along it when pushed (its render)
#define FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_LOWER_STOP               \
  FLAG_AREA_TEMP_12
/// The fourth room's first log lies at the south end of its track.
/// ★ Named from what the player sees: golden logs floating on the cave's pool,
/// which float along it when pushed (its render)
#define FLAG_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_FIRST_LOG_SOUTH FLAG_AREA_TEMP_13
/// The fourth room's second log lies at the west end of its track.
/// ★ Named from what the player sees: golden logs floating on the cave's pool,
/// which float along it when pushed (its render)
#define FLAG_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_SECOND_LOG_WEST FLAG_AREA_TEMP_14
/// The fourth room's third log lies at the east end of its track.
/// ★ Named from what the player sees: golden logs floating on the cave's pool,
/// which float along it when pushed (its render)
#define FLAG_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_THIRD_LOG_EAST FLAG_AREA_TEMP_15

/// Lets the log settle before rebuilding its room's cell behaviors.
static inline void FuchinFallsCave_FinishLogMove(void (*rebuildLogs)(void)) {
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  WaitFrames(2);
  rebuildLogs();
}

/// Fuchin Falls Cave, first room: pushing the first log east (step trigger 11)
/// floats it 224 pixels east (FLAG_FUCHIN_FALLS_ENTRANCE_FIRST_LOG_EAST set).
static void FuchinFallsCave_Entrance_MoveFirstLogEast(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_FIRST_LOG, 112, 0);
  Actor_PlacePairAtOffset(ACTOR_FIRST_LOG, 112, 0);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Set(FLAG_FUCHIN_FALLS_ENTRANCE_FIRST_LOG_EAST);
  WaitFrames(2);
  FuchinFallsCave_RebuildEntranceLogs();
}

/// Fuchin Falls Cave, first room: pushing the first log west (step trigger 12)
/// floats it 224 pixels back (FLAG_FUCHIN_FALLS_ENTRANCE_FIRST_LOG_EAST clear).
static void FuchinFallsCave_Entrance_MoveFirstLogWest(void) {
  s32 offset = 112;

  offset = -offset;
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_FIRST_LOG, offset, 0);
  Actor_PlacePairAtOffset(ACTOR_FIRST_LOG, offset, 0);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_ENTRANCE_FIRST_LOG_EAST);
  WaitFrames(2);
  FuchinFallsCave_RebuildEntranceLogs();
}

/// Fuchin Falls Cave, first room: pushing the second log south (step trigger
/// 13) floats it 64 pixels south (FLAG_FUCHIN_FALLS_ENTRANCE_SECOND_LOG_NORTH
/// clear).
static void FuchinFallsCave_Entrance_MoveSecondLogSouth(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_SECOND_LOG, 0, 64);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_ENTRANCE_SECOND_LOG_NORTH);
  WaitFrames(2);
  FuchinFallsCave_RebuildEntranceLogs();
}

/// Fuchin Falls Cave, first room: pushing the second log north (step trigger
/// 14) floats it 64 pixels north (FLAG_FUCHIN_FALLS_ENTRANCE_SECOND_LOG_NORTH
/// set).
static void FuchinFallsCave_Entrance_MoveSecondLogNorth(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_SECOND_LOG, 0, -64);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Set(FLAG_FUCHIN_FALLS_ENTRANCE_SECOND_LOG_NORTH);
  WaitFrames(2);
  FuchinFallsCave_RebuildEntranceLogs();
}

/// Fuchin Falls Cave, first room: pushing the third log south (step trigger 15)
/// floats it 64 pixels south (FLAG_FUCHIN_FALLS_ENTRANCE_THIRD_LOG_NORTH
/// clear).
static void FuchinFallsCave_Entrance_MoveThirdLogSouth(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, 64);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_ENTRANCE_THIRD_LOG_NORTH);
  WaitFrames(2);
  FuchinFallsCave_RebuildEntranceLogs();
}

/// Fuchin Falls Cave, first room: pushing the third log north (step trigger 16)
/// floats it 64 pixels north (FLAG_FUCHIN_FALLS_ENTRANCE_THIRD_LOG_NORTH set).
static void FuchinFallsCave_Entrance_MoveThirdLogNorth(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, -64);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Set(FLAG_FUCHIN_FALLS_ENTRANCE_THIRD_LOG_NORTH);
  WaitFrames(2);
  FuchinFallsCave_RebuildEntranceLogs();
}

/// Fuchin Falls Cave, second room: pushing the first log east (step trigger
/// 11), while FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_SECOND_LOG_SOUTH is clear, floats
/// it 144 pixels east (FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_EAST_END set).
static void FuchinFallsCave_ZephyrRoom_MoveFirstLogToEastEnd(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_FIRST_LOG, 144, 0);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Set(FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_EAST_END);
  WaitFrames(2);
  FuchinFallsCave_RebuildZephyrRoomLogs();
}

/// Fuchin Falls Cave, second room: pushing the first log west (step trigger 12)
/// floats it 144 pixels west (FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_EAST_END
/// clear, FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_WEST_END set).
static void FuchinFallsCave_ZephyrRoom_MoveFirstLogToWestEnd(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_FIRST_LOG, -144, 0);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_EAST_END);
  GameFlag_Set(FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_WEST_END);
  WaitFrames(2);
  FuchinFallsCave_RebuildZephyrRoomLogs();
}

/// Fuchin Falls Cave, second room: pushing the first log west (step trigger 11)
/// floats it 14 pixels west (FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_WEST_END
/// set).
static void FuchinFallsCave_ZephyrRoom_NudgeFirstLogWest(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_FIRST_LOG, -14, 0);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Set(FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_WEST_END);
  WaitFrames(2);
  FuchinFallsCave_RebuildZephyrRoomLogs();
}

/// Fuchin Falls Cave, second room: pushing the first log east (step trigger 17)
/// moves it one cell when the second log lies south
/// (FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_SECOND_LOG_SOUTH;
/// FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_WEST_END clear), else 144 pixels
/// (FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_EAST_END set).
static void FuchinFallsCave_ZephyrRoom_MoveFirstLogEast(void) {
  Sound_Play(SE_WATER_MOVE);

  if (GameFlag_Get(FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_SECOND_LOG_SOUTH) != 0) {
    Actor_PlacePairAtOffset(ACTOR_FIRST_LOG, 16, 0);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_WEST_END);
  } else {
    Actor_PlacePairAtOffset(ACTOR_FIRST_LOG, 144, 0);

    GameFlag_Set(FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_EAST_END);
  }

  FuchinFallsCave_FinishLogMove(FuchinFallsCave_RebuildZephyrRoomLogs);
}

/// Fuchin Falls Cave, second room: pushing the second log south (step trigger
/// 13) floats it 64 pixels south
/// (FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_SECOND_LOG_SOUTH set).
static void FuchinFallsCave_ZephyrRoom_MoveSecondLogSouth(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_SECOND_LOG, 0, 64);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Set(FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_SECOND_LOG_SOUTH);
  WaitFrames(2);
  FuchinFallsCave_RebuildZephyrRoomLogs();
}

/// Fuchin Falls Cave, second room: pushing the second log north (step trigger
/// 14) floats it 64 pixels north
/// (FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_SECOND_LOG_SOUTH clear).
static void FuchinFallsCave_ZephyrRoom_MoveSecondLogNorth(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_SECOND_LOG, 0, -64);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_SECOND_LOG_SOUTH);
  WaitFrames(2);
  FuchinFallsCave_RebuildZephyrRoomLogs();
}

/// Fuchin Falls Cave, second room: pushing the third log south (step trigger
/// 15) floats it 272 pixels south
/// (FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_THIRD_LOG_NORTH clear).
static void FuchinFallsCave_ZephyrRoom_MoveThirdLogSouth(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, 144);
  Actor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, 128);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_THIRD_LOG_NORTH);
  WaitFrames(2);
  FuchinFallsCave_RebuildZephyrRoomLogs();
}

/// Fuchin Falls Cave, second room: pushing the third log north (step trigger
/// 16) floats it 272 pixels north
/// (FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_THIRD_LOG_NORTH set).
static void FuchinFallsCave_ZephyrRoom_MoveThirdLogNorth(void) {
  s32 depth = -96;

  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, depth);
  Actor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, depth);
  Actor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, -80);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Set(FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_THIRD_LOG_NORTH);
  WaitFrames(2);
  FuchinFallsCave_RebuildZephyrRoomLogs();
}

/// Fuchin Falls Cave, third room: pushing the first log west (step trigger 10):
/// 48 pixels (FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_MIDDLE set) when the
/// third log lies at FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE or the
/// fourth at FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_UPPER_STOP, else 96
/// (FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_WEST_END set).
static void FuchinFallsCave_DragonsEyeRoom_MoveFirstLogWest(void) {
  Sound_Play(SE_WATER_MOVE);

  if (GameFlag_Get(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_UPPER_STOP) !=
          0 ||
      GameFlag_Get(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE) != 0) {
    Actor_PlacePairAtOffset(ACTOR_FIRST_LOG, -48, 0);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_WEST_END);
    GameFlag_Set(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_MIDDLE);
  } else {
    Actor_PlacePairAtOffset(ACTOR_FIRST_LOG, -96, 0);
    GameFlag_Set(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_WEST_END);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_MIDDLE);
  }

  FuchinFallsCave_FinishLogMove(FuchinFallsCave_RebuildDragonsEyeRoomLogs);
}

/// Fuchin Falls Cave, third room: pushing the first log east (step trigger 11)
/// floats it 48 pixels east
/// (FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_WEST_END and
/// FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_MIDDLE clear).
static void FuchinFallsCave_DragonsEyeRoom_MoveFirstLogEastFromMiddle(void) {
  Sound_Play(SE_WATER_MOVE);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_WEST_END);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_MIDDLE);
  Actor_PlacePairAtOffset(ACTOR_FIRST_LOG, 48, 0);
  FuchinFallsCave_FinishLogMove(FuchinFallsCave_RebuildDragonsEyeRoomLogs);
}

/// Fuchin Falls Cave, third room: pushing the first log west (step trigger 11),
/// unless the third log lies at
/// FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE or the fourth at
/// FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_UPPER_STOP, floats it 48
/// pixels west (FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_WEST_END set).
static void FuchinFallsCave_DragonsEyeRoom_MoveFirstLogWestIfClear(void) {
  if (GameFlag_Get(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_UPPER_STOP) !=
      0) {
    return;
  }
  if (GameFlag_Get(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE) != 0) {
    return;
  }

  Sound_Play(SE_WATER_MOVE);

  GameFlag_Set(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_WEST_END);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_MIDDLE);

  Actor_PlacePairAtOffset(ACTOR_FIRST_LOG, -48, 0);
  FuchinFallsCave_FinishLogMove(FuchinFallsCave_RebuildDragonsEyeRoomLogs);
}

/// Fuchin Falls Cave, third room: pushing the first log east (step trigger 12)
/// floats it 96 pixels east
/// (FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_WEST_END and
/// FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_MIDDLE clear).
static void FuchinFallsCave_DragonsEyeRoom_MoveFirstLogEastFromWestEnd(void) {
  Sound_Play(SE_WATER_MOVE);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_WEST_END);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_MIDDLE);
  Actor_PlacePairAtOffset(ACTOR_FIRST_LOG, 96, 0);
  FuchinFallsCave_FinishLogMove(FuchinFallsCave_RebuildDragonsEyeRoomLogs);
}

/// Fuchin Falls Cave, third room: pushing the second log west (step trigger 14)
/// floats it 32 pixels west (FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_SECOND_LOG_WEST
/// set).
static void FuchinFallsCave_DragonsEyeRoom_MoveSecondLogWest(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_SECOND_LOG, -32, 0);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Set(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_SECOND_LOG_WEST);
  WaitFrames(2);
  FuchinFallsCave_RebuildDragonsEyeRoomLogs();
}

/// Fuchin Falls Cave, third room: pushing the second log east (step trigger 13)
/// floats it 32 pixels east (FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_SECOND_LOG_WEST
/// clear).
static void FuchinFallsCave_DragonsEyeRoom_MoveSecondLogEast(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_SECOND_LOG, 32, 0);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_SECOND_LOG_WEST);
  WaitFrames(2);
  FuchinFallsCave_RebuildDragonsEyeRoomLogs();
}

/// Fuchin Falls Cave, third room: pushing the third log north (step trigger
/// 15): 64 pixels (FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_UPPER_STOP)
/// when the first log lies at
/// FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_WEST_END, else 128
/// (FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END).
static void FuchinFallsCave_DragonsEyeRoom_MoveThirdLogNorthFromSouthEnd(void) {
  Sound_Play(SE_WATER_MOVE);
  if (GameFlag_Get(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_WEST_END) !=
      0) {
    Actor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, -64);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END);
    GameFlag_Set(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_UPPER_STOP);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_LOWER_STOP);
  } else {
    Actor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, -128);
    GameFlag_Set(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_UPPER_STOP);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_LOWER_STOP);
  }
  FuchinFallsCave_FinishLogMove(FuchinFallsCave_RebuildDragonsEyeRoomLogs);
}

/// Fuchin Falls Cave, third room: pushing the third log south (step trigger
/// 16): one cell (FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_UPPER_STOP) when
/// the first log lies at FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_WEST_END
/// or the fourth at FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_UPPER_STOP,
/// 64 pixels (FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE) when the
/// fourth lies at FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_MIDDLE, else
/// 128 (its stops clear).
static void FuchinFallsCave_DragonsEyeRoom_MoveThirdLogSouthFromNorthEnd(void) {
  Sound_Play(SE_WATER_MOVE);

  if (GameFlag_Get(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_WEST_END) !=
      0) {
    Actor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, 16);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END);
    GameFlag_Set(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_UPPER_STOP);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_LOWER_STOP);
  } else if (GameFlag_Get(
                 FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_UPPER_STOP) !=
             0) {
    Actor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, 16);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END);
    GameFlag_Set(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_UPPER_STOP);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_LOWER_STOP);
  } else if (GameFlag_Get(
                 FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_MIDDLE) != 0) {
    Actor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, 64);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_UPPER_STOP);
    GameFlag_Set(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_LOWER_STOP);
  } else {
    Actor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, 128);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_UPPER_STOP);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_LOWER_STOP);
  }
  FuchinFallsCave_FinishLogMove(FuchinFallsCave_RebuildDragonsEyeRoomLogs);
}

/// Fuchin Falls Cave, third room: pushing the third log north (step trigger 17)
/// moves it one cell (FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END).
static void FuchinFallsCave_DragonsEyeRoom_NudgeThirdLogNorth(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, -16);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Set(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_UPPER_STOP);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_LOWER_STOP);
  WaitFrames(2);
  FuchinFallsCave_RebuildDragonsEyeRoomLogs();
}

/// Fuchin Falls Cave, third room: pushing the third log south (step trigger
/// 17), unless the first log lies at
/// FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_WEST_END: 48 pixels
/// (FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE) when the fourth lies
/// at FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_MIDDLE, 32
/// (FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_UPPER_STOP) when it lies at
/// FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_UPPER_STOP, else 112 (its
/// stops clear).
static void
FuchinFallsCave_DragonsEyeRoom_MoveThirdLogSouthFromUpperStop(void) {
  Sound_Play(SE_WATER_MOVE);
  if (GameFlag_Get(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_MIDDLE) != 0) {
    Actor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, 48);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END);

    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_UPPER_STOP);
    GameFlag_Set(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_LOWER_STOP);
  } else if (GameFlag_Get(
                 FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_UPPER_STOP) !=
             0) {
    Actor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, 32);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END);
    GameFlag_Set(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_UPPER_STOP);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_LOWER_STOP);
  } else {
    Actor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, 112);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_UPPER_STOP);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_LOWER_STOP);
  }
  FuchinFallsCave_FinishLogMove(FuchinFallsCave_RebuildDragonsEyeRoomLogs);
}

/// Fuchin Falls Cave, third room: pushing the third log north (step trigger 18)
/// floats it 64 pixels north
/// (FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END).
static void FuchinFallsCave_DragonsEyeRoom_MoveThirdLogNorthFromMiddle(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, -64);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Set(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE);
  WaitFrames(2);
  FuchinFallsCave_RebuildDragonsEyeRoomLogs();
}

/// Fuchin Falls Cave, third room: pushing the third log south (step trigger
/// 18), unless the fourth lies at
/// FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_UPPER_STOP, floats it 64
/// pixels south (its stops clear).
static void FuchinFallsCave_DragonsEyeRoom_MoveThirdLogSouthFromMiddle(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, 64);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_UPPER_STOP);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_LOWER_STOP);
  WaitFrames(2);
  FuchinFallsCave_RebuildDragonsEyeRoomLogs();
}

/// Fuchin Falls Cave, third room: pushing the third log north (step trigger 19)
/// floats it 80 pixels north
/// (FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END).
static void
FuchinFallsCave_DragonsEyeRoom_MoveThirdLogNorthFromLowerStop(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, -80);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Set(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_UPPER_STOP);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_LOWER_STOP);
  WaitFrames(2);
  FuchinFallsCave_RebuildDragonsEyeRoomLogs();
}

/// Fuchin Falls Cave, third room: pushing
/// the third log south (step trigger 19) floats it 64 pixels south (its stops
/// clear).
static void
FuchinFallsCave_DragonsEyeRoom_MoveThirdLogSouthFromLowerStop(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, 64);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_UPPER_STOP);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_LOWER_STOP);
  WaitFrames(2);
  FuchinFallsCave_RebuildDragonsEyeRoomLogs();
}

/// Fuchin Falls Cave, third room: pushing the fourth log north (step trigger
/// 20): 64 pixels (FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_MIDDLE) when
/// the first log lies at FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_WEST_END
/// or the third at FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE, 112
/// (FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_UPPER_STOP) when the third
/// lies at FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_UPPER_STOP, else 128
/// (FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_NORTH_END).
static void FuchinFallsCave_DragonsEyeRoom_MoveFourthLogNorth(void) {
  Sound_Play(SE_WATER_MOVE);

  if (GameFlag_Get(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_WEST_END) !=
          0 ||
      GameFlag_Get(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE) != 0) {
    Actor_PlacePairAtOffset(ACTOR_FOURTH_LOG, 0, -64);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_NORTH_END);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_UPPER_STOP);
    GameFlag_Set(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_MIDDLE);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_LOWER_STOP);
  } else if (GameFlag_Get(
                 FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_UPPER_STOP) !=
             0) {
    Actor_PlacePairAtOffset(ACTOR_FOURTH_LOG, 0, -112);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_NORTH_END);
    GameFlag_Set(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_UPPER_STOP);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_MIDDLE);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_LOWER_STOP);
  } else {
    Actor_PlacePairAtOffset(ACTOR_FOURTH_LOG, 0, -128);
    GameFlag_Set(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_NORTH_END);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_UPPER_STOP);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_MIDDLE);
    GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_LOWER_STOP);
  }
  FuchinFallsCave_FinishLogMove(FuchinFallsCave_RebuildDragonsEyeRoomLogs);
}

/// Clears all saved intermediate stops for the fourth log.
static void FuchinFallsCave_ClearFourthLogStops(void) {
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_NORTH_END);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_UPPER_STOP);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_MIDDLE);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_LOWER_STOP);
}

/// Fuchin Falls Cave, third room: pushing the fourth log south (step trigger
/// 21), unless the first log lies at
/// FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_WEST_END, floats it 128 pixels
/// south (its stops clear).
static void
FuchinFallsCave_DragonsEyeRoom_MoveFourthLogSouthFromNorthEnd(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_FOURTH_LOG, 0, 128);
  FuchinFallsCave_ClearFourthLogStops();
  FuchinFallsCave_FinishLogMove(FuchinFallsCave_RebuildDragonsEyeRoomLogs);
}

/// Fuchin Falls Cave, third room: pushing the fourth log north (step trigger
/// 22) moves it one cell
/// (FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_NORTH_END).
static void FuchinFallsCave_DragonsEyeRoom_NudgeFourthLogNorth(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_FOURTH_LOG, 0, -16);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Set(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_NORTH_END);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_UPPER_STOP);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_MIDDLE);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_LOWER_STOP);
  WaitFrames(2);
  FuchinFallsCave_RebuildDragonsEyeRoomLogs();
}

/// Fuchin Falls Cave, third room: pushing
/// the fourth log south (step trigger 22) floats it 112 pixels south (its stops
/// clear).
static void
FuchinFallsCave_DragonsEyeRoom_MoveFourthLogSouthFromUpperStop(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_FOURTH_LOG, 0, 112);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  FuchinFallsCave_ClearFourthLogStops();
  WaitFrames(2);
  FuchinFallsCave_RebuildDragonsEyeRoomLogs();
}

/// Fuchin Falls Cave, third room: pushing
/// the fourth log south (step trigger 23) floats it 64 pixels south (its stops
/// clear).
static void FuchinFallsCave_DragonsEyeRoom_MoveFourthLogSouthFromMiddle(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_FOURTH_LOG, 0, 64);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  FuchinFallsCave_ClearFourthLogStops();
  WaitFrames(2);
  FuchinFallsCave_RebuildDragonsEyeRoomLogs();
}

// XXX (no evidence): whether walking north was meant to float the log north,
// as the third log's matching step trigger 19 does; the room never has a
// step trigger 24, so the game never shows which.
/// Unused: walking north into step trigger 24 in Fuchin Falls Cave's third room
/// would float the fourth log 80 pixels south (its stops clear). Step trigger
/// 24's cells reach the room only from their copy at columns 10 to 12, row 43,
/// which FuchinFallsCave_RebuildDragonsEyeRoomLogs makes with the log at
/// FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_LOWER_STOP, and no move sets
/// that flag, so the room never has a step trigger 24 (in tbs-en it appears
/// there only with the flag set by hand; walking north on the log then carries
/// the leader 80 pixels south, past the log's south end, where the rebuild puts
/// the log back).
static void
FuchinFallsCave_DragonsEyeRoom_UnusedMoveFourthLogSouthFromLowerStop(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_FOURTH_LOG, 0, 80);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  FuchinFallsCave_ClearFourthLogStops();
  WaitFrames(2);
  FuchinFallsCave_RebuildDragonsEyeRoomLogs();
}

/// Unused: pushing the fourth log south from step trigger 24 in Fuchin Falls
/// Cave's third room would float it 48 pixels south (its stops clear), back to
/// its south end; the room never has a step trigger 24, as
/// FuchinFallsCave_DragonsEyeRoom_UnusedMoveFourthLogSouthFromLowerStop says.
static void
FuchinFallsCave_DragonsEyeRoom_UnusedMoveFourthLogSouthFromLastStop(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_FOURTH_LOG, 0, 48);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  FuchinFallsCave_ClearFourthLogStops();
  WaitFrames(2);
  FuchinFallsCave_RebuildDragonsEyeRoomLogs();
}

/// Fuchin Falls Cave, fourth room: pushing the first log south (step trigger
/// 11) floats it 112 pixels south
/// (FLAG_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_FIRST_LOG_SOUTH set).
static void FuchinFallsCave_ArcticBladeRoom_MoveFirstLogSouth(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_FIRST_LOG, 0, 112);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Set(FLAG_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_FIRST_LOG_SOUTH);
  WaitFrames(2);
  FuchinFallsCave_RebuildArcticBladeRoomLogs();
}

/// Fuchin Falls Cave, fourth room: pushing the first log north (step trigger
/// 12) floats it 112 pixels north
/// (FLAG_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_FIRST_LOG_SOUTH clear).
static void FuchinFallsCave_ArcticBladeRoom_MoveFirstLogNorth(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_FIRST_LOG, 0, -112);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_FIRST_LOG_SOUTH);
  WaitFrames(2);
  FuchinFallsCave_RebuildArcticBladeRoomLogs();
}

/// Fuchin Falls Cave, fourth room: pushing the second log west (step trigger
/// 13), while the first log lies north
/// (FLAG_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_FIRST_LOG_SOUTH clear), floats it 128
/// pixels west (FLAG_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_SECOND_LOG_WEST set).
static void FuchinFallsCave_ArcticBladeRoom_MoveSecondLogWest(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_SECOND_LOG, -128, 0);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Set(FLAG_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_SECOND_LOG_WEST);
  WaitFrames(2);
  FuchinFallsCave_RebuildArcticBladeRoomLogs();
}

/// Fuchin Falls Cave, fourth room: pushing the second log east (step trigger
/// 14) floats it 128 pixels east
/// (FLAG_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_SECOND_LOG_WEST clear).
static void FuchinFallsCave_ArcticBladeRoom_MoveSecondLogEast(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_SECOND_LOG, 128, 0);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_SECOND_LOG_WEST);
  WaitFrames(2);
  FuchinFallsCave_RebuildArcticBladeRoomLogs();
}

/// Fuchin Falls Cave, fourth room: pushing the third log east (step trigger 15)
/// floats it 160 pixels east
/// (FLAG_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_THIRD_LOG_EAST set).
static void FuchinFallsCave_ArcticBladeRoom_MoveThirdLogEast(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_THIRD_LOG, 160, 0);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Set(FLAG_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_THIRD_LOG_EAST);
  WaitFrames(2);
  FuchinFallsCave_RebuildArcticBladeRoomLogs();
}

/// Fuchin Falls Cave, fourth room: pushing the third log west (step trigger 16)
/// floats it 160 pixels west
/// (FLAG_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_THIRD_LOG_EAST clear).
static void FuchinFallsCave_ArcticBladeRoom_MoveThirdLogWest(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_THIRD_LOG, -160, 0);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_THIRD_LOG_EAST);
  WaitFrames(2);
  FuchinFallsCave_RebuildArcticBladeRoomLogs();
}

/// Touching the first log's step trigger cell in Fuchin Falls Cave plays its
/// two rolling animations.
static void FuchinFallsCave_AnimateFirstLog(void) {
  Import_ActorSetAnimation(ACTOR_FIRST_LOG, 1);
  Import_ActorSetAnimation(ACTOR_FIRST_LOG, 2);
}

/// Touching the second log's step trigger cell in Fuchin Falls Cave plays its
/// two rolling animations.
static void FuchinFallsCave_AnimateSecondLog(void) {
  Import_ActorSetAnimation(ACTOR_SECOND_LOG, 1);
  Import_ActorSetAnimation(ACTOR_SECOND_LOG, 2);
}

/// Touching the third log's step trigger cell in Fuchin Falls Cave plays its
/// two rolling animations.
static void FuchinFallsCave_AnimateThirdLog(void) {
  Import_ActorSetAnimation(ACTOR_THIRD_LOG, 1);
  Import_ActorSetAnimation(ACTOR_THIRD_LOG, 2);
}

/// Touching the fourth log's step trigger cell in Fuchin Falls Cave (third
/// room) plays its two rolling animations.
static void FuchinFallsCave_AnimateFourthLog(void) {
  Import_ActorSetAnimation(ACTOR_FOURTH_LOG, 1);
  Import_ActorSetAnimation(ACTOR_FOURTH_LOG, 2);
}

/// Creates a stationary actor at sprite priority 1, drawn hidden
/// (SPRITE_COLOR_HIDDEN).
static struct Actor *FuchinFallsCave_CreateForegroundActor(s32 x, s32 y, s32 z,
                                                           s32 resourceId) {
  struct Actor *actor = FieldActor_Create(resourceId, x, y, z);

  if (actor != 0) {
    actor->sprite->image.priority = 1;
    actor->motionFlags = 0;
    Actor_SetSpriteFlags(actor, 0);
    Actor_SetSpriteColor(actor, SPRITE_COLOR_HIDDEN);
    actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    return actor;
  }
  return 0;
}

const struct Trigger *ImilFuchinFalls_GetTriggers(void) {
  s16 sceneId = gAdventure.sceneId;

  if (sceneId == (s32)&SceneId_ImilFallsCave) {
    return gImilFuchinFalls_ImilFallsCaveTriggers;
  }
  if (sceneId == (s32)&SceneId_FuchinFallsCaveEntrance) {
    return gImilFuchinFalls_EntranceTriggers;
  }
  if (sceneId == (s32)&SceneId_FuchinFallsCaveZephyr) {
    return gImilFuchinFalls_ZephyrRoomTriggers;
  }
  if (sceneId == (s32)&SceneId_FuchinFallsCaveDragonsEye) {
    return gImilFuchinFalls_DragonsEyeRoomTriggers;
  }
  if (sceneId == (s32)&SceneId_FuchinFallsCaveArcticBlade) {
    return gImilFuchinFalls_ArcticBladeRoomTriggers;
  }
  if (sceneId == (s32)&SceneId_FuchinFallsCaveOrbOfForce) {
    return gImilFuchinFalls_KiRoomTriggers;
  }
  if (sceneId == (s32)&SceneId_FuchinFallsCaveDragonStatue) {
    return gImilFuchinFalls_DragonRoomTriggers;
  }
  return gImilFuchinFalls_NoTriggers;
}

/// Walking into the way out of Imil Falls Cave (step trigger 1): faces the
/// leader south and leaves for Imil.
static void Imil_LeaveFallsCave(void) {
  struct Actor *leader;

  Event_Start();
  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  leader->facing = FACING_SOUTH;
  Sound_Play(SE_MAP_EXIT);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(IMIL_FALLS_CAVE_TO_IMIL);
}

/// Stepping onto the ice in Imil Falls Cave (cell behavior 10) slides
/// the selected actor in the held direction.
static void Imil_MoveAcrossIce(void) { Imil_SlideActorAcrossIce(); }

/// Centers a log on its cell and gives it underfoot priority.
static void FuchinFallsCave_PlaceLogAtCell(s32 id, s32 column, s32 row) {
  struct Actor *actor = Actor_GetById(id);

  if (actor != NULL) {
    Actor_SetSpritePriority(id, 3);
    actor->layer = 2;
    actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    actor->x.fixed = PIXELS(column * 16) + PIXELS(8);
    actor->z.fixed = PIXELS(row * 16) + PIXELS(8);
  }
}

/// Puts the logs of Fuchin Falls Cave's first room
/// (SceneId_FuchinFallsCaveEntrance) at the cells their flags name, and
/// rewrites the cells of their tracks to match.
static void FuchinFallsCave_RebuildEntranceLogs(void) {
  {
    s32 column = 8;
    s32 row = 29;
    MapGrid_CopyCellAttributes(8, 42, 15, 5, column, row);
  }

  if (GameFlag_Get(FLAG_FUCHIN_FALLS_ENTRANCE_FIRST_LOG_EAST) != 0) {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_FIRST_LOG, 22, 31);
    {
      s32 column = 8;
      s32 row = 30;
      MapGrid_CopyCellAttributes(9, 30, 1, 3, column, row);
    }
  } else {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_FIRST_LOG, 8, 31);
    {
      s32 column = 22;
      s32 row = 30;
      MapGrid_CopyCellAttributes(9, 30, 1, 3, column, row);
    }
  }

  if (GameFlag_Get(FLAG_FUCHIN_FALLS_ENTRANCE_SECOND_LOG_NORTH) != 0) {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_SECOND_LOG, 12, 29);
    {
      s32 column = 11;
      s32 row = 33;
      MapGrid_CopyCellAttributes(14, 33, 3, 1, column, row);
    }
  } else {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_SECOND_LOG, 12, 33);
    {
      s32 column = 11;
      s32 row = 29;
      MapGrid_CopyCellAttributes(14, 29, 3, 1, column, row);
    }
  }

  if (GameFlag_Get(FLAG_FUCHIN_FALLS_ENTRANCE_THIRD_LOG_NORTH) != 0) {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_THIRD_LOG, 18, 29);
    {
      s32 column = 17;
      s32 row = 33;
      MapGrid_CopyCellAttributes(14, 33, 3, 1, column, row);
    }
  } else {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_THIRD_LOG, 18, 33);
    {
      s32 column = 17;
      s32 row = 29;
      MapGrid_CopyCellAttributes(14, 29, 3, 1, column, row);
    }
  }
}

/// Puts the logs of Fuchin Falls Cave's second room where their flags say. The
/// flags name cells in SceneId_FuchinFallsCaveZephyr, and the cells of the
/// logs' tracks are rewritten to match.
static void FuchinFallsCave_RebuildZephyrRoomLogs(void) {
  {
    s32 column = 12;
    s32 row = 8;
    MapGrid_CopyCellAttributes(0, 28, 10, 18, column, row);
  }

  if (GameFlag_Get(FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_EAST_END) != 0) {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_FIRST_LOG, 21, 20);
    {
      s32 column = 13;
      s32 row = 19;
      MapGrid_CopyCellAttributes(20, 19, 1, 3, column, row);
    }
  } else {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_FIRST_LOG, 13, 20);
    {
      s32 column = 21;
      s32 row = 19;
      MapGrid_CopyCellAttributes(20, 19, 1, 3, column, row);
    }
  }

  if (GameFlag_Get(FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_WEST_END) != 0) {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_FIRST_LOG, 12, 20);
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
    if (GameFlag_Get(FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_EAST_END) != 0) {
      FuchinFallsCave_PlaceLogAtCell(ACTOR_FIRST_LOG, 21, 20);
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

  if (GameFlag_Get(FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_SECOND_LOG_SOUTH) != 0) {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_SECOND_LOG, 15, 21);
    {
      s32 column = 14;
      s32 row = 17;
      MapGrid_CopyCellAttributes(14, 18, 3, 1, column, row);
    }
  } else {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_SECOND_LOG, 15, 17);
    {
      s32 column = 14;
      s32 row = 21;
      MapGrid_CopyCellAttributes(14, 18, 3, 1, column, row);
    }
  }

  if (GameFlag_Get(FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_THIRD_LOG_NORTH) != 0) {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_THIRD_LOG, 19, 8);
    {
      s32 column = 18;
      s32 row = 25;
      MapGrid_CopyCellAttributes(14, 18, 3, 1, column, row);
    }
  } else {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_THIRD_LOG, 19, 25);
    {
      s32 column = 18;
      s32 row = 8;
      MapGrid_CopyCellAttributes(14, 18, 3, 1, column, row);
    }
  }
}

/// Puts the four logs of Fuchin Falls Cave's third room where their flags say.
/// The flags name cells in SceneId_FuchinFallsCaveDragonsEye, and the cells of
/// the logs' tracks are rewritten to match.
static void FuchinFallsCave_RebuildDragonsEyeRoomLogs(void) {
  {
    MapGrid_CopyCellAttributes(12, 3, 9, 16, 12, 21);
  }

  if (GameFlag_Get(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_WEST_END) !=
      0) {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_FIRST_LOG, 14, 25);
    {
      MapGrid_CopyCellAttributes(16, 24, 1, 3, 20, 24);
    }
  } else if (GameFlag_Get(
                 FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_MIDDLE) != 0) {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_FIRST_LOG, 17, 25);
    {
      s32 row = 24;
      MapGrid_CopyCellAttributes(18, 24, 1, 3, 20, row);
      MapGrid_CopyCellAttributes(18, 24, 1, 3, 14, row);
      MapGrid_CopyCellAttributes(8, 41, 1, 3, 17, row);
    }
  } else {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_FIRST_LOG, 20, 25);
    {
      MapGrid_CopyCellAttributes(16, 24, 1, 3, 14, 24);
    }
  }

  if (GameFlag_Get(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_SECOND_LOG_WEST) != 0) {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_SECOND_LOG, 13, 35);
    {
      MapGrid_CopyCellAttributes(14, 34, 1, 3, 15, 34);
    }
  } else {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_SECOND_LOG, 15, 35);
    {
      MapGrid_CopyCellAttributes(14, 34, 1, 3, 13, 34);
    }
  }

  if (GameFlag_Get(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END) !=
      0) {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_THIRD_LOG, 15, 22);
    {
      s32 column = 14;
      MapGrid_CopyCellAttributes(14, 29, 3, 1, column, 30);
      MapGrid_CopyCellAttributes(5, 41, 3, 1, column, 22);
    }
  } else if (GameFlag_Get(
                 FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_UPPER_STOP) !=
             0) {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_THIRD_LOG, 15, 23);
    {
      s32 column = 14;
      MapGrid_CopyCellAttributes(5, 42, 3, 1, column, 23);
      MapGrid_CopyCellAttributes(14, 29, 3, 1, column, 30);
      MapGrid_CopyCellAttributes(10, 44, 3, 1, column, 21);
    }
  } else if (GameFlag_Get(
                 FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE) != 0) {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_THIRD_LOG, 15, 26);
    {
      s32 column = 14;
      MapGrid_CopyCellAttributes(14, 29, 3, 1, column, 22);
      MapGrid_CopyCellAttributes(5, 43, 3, 1, column, 26);
      MapGrid_CopyCellAttributes(14, 29, 3, 1, column, 30);
    }
  } else if (GameFlag_Get(
                 FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_LOWER_STOP) !=
             0) {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_THIRD_LOG, 15, 27);
    {
      s32 column = 14;
      MapGrid_CopyCellAttributes(14, 29, 3, 1, column, 22);
      MapGrid_CopyCellAttributes(14, 29, 3, 1, column, 30);
      MapGrid_CopyCellAttributes(5, 44, 3, 1, column, 27);
    }
  } else {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_THIRD_LOG, 15, 30);
  }

  if (GameFlag_Get(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_NORTH_END) !=
      0) {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_FOURTH_LOG, 15, 23);
    {
      s32 column = 14;
      MapGrid_CopyCellAttributes(14, 29, 3, 1, column, 31);
      MapGrid_CopyCellAttributes(10, 40, 3, 1, column, 23);
    }
  } else if (GameFlag_Get(
                 FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_UPPER_STOP) !=
             0) {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_FOURTH_LOG, 15, 24);
    {
      s32 column = 14;
      MapGrid_CopyCellAttributes(14, 29, 3, 1, column, 31);
      MapGrid_CopyCellAttributes(10, 41, 3, 1, column, 24);
    }
  } else if (GameFlag_Get(
                 FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_MIDDLE) != 0) {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_FOURTH_LOG, 15, 27);
    {
      s32 column = 14;
      MapGrid_CopyCellAttributes(14, 29, 3, 1, column, 31);
      MapGrid_CopyCellAttributes(10, 42, 3, 1, column, 27);
    }
  } else if (GameFlag_Get(
                 FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_LOWER_STOP) !=
             0) {
    // Unused: no move sets
    // FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_LOWER_STOP, so the log
    // never lies here and the room never gets step trigger 24.
    FuchinFallsCave_PlaceLogAtCell(ACTOR_FOURTH_LOG, 15, 28);
    {
      s32 column = 14;
      MapGrid_CopyCellAttributes(14, 29, 3, 1, column, 31);
      MapGrid_CopyCellAttributes(10, 43, 3, 1, column, 28);
    }
  } else {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_FOURTH_LOG, 15, 31);
  }
}

/// Puts the logs of Fuchin Falls Cave's fourth room
/// (SceneId_FuchinFallsCaveArcticBlade), in both of its halves, at the cells
/// their flags name.
static void FuchinFallsCave_RebuildArcticBladeRoomLogs(void) {
  if (((u32)((u16)gAdventure.entrance - 1) << 16) <= 0x10000) {
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

  if (GameFlag_Get(FLAG_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_FIRST_LOG_SOUTH) != 0) {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_FIRST_LOG, 20, 17);
    {
      s32 column = 19;
      s32 row = 10;
      MapGrid_CopyCellAttributes(19, 11, 3, 1, column, row);
    }
  } else {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_FIRST_LOG, 20, 10);
    {
      s32 column = 19;
      s32 row = 17;
      MapGrid_CopyCellAttributes(19, 11, 3, 1, column, row);
    }
  }

  if (GameFlag_Get(FLAG_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_SECOND_LOG_WEST) != 0) {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_SECOND_LOG, 14, 16);
    {
      s32 column = 22;
      s32 row = 15;
      MapGrid_CopyCellAttributes(16, 15, 1, 3, column, row);
    }
  } else {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_SECOND_LOG, 22, 16);
    {
      s32 column = 14;
      s32 row = 15;
      MapGrid_CopyCellAttributes(16, 15, 1, 3, column, row);
    }
  }

  if (GameFlag_Get(FLAG_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_THIRD_LOG_EAST) != 0) {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_THIRD_LOG, 17, 46);
    {
      s32 column = 7;
      s32 row = 45;
      MapGrid_CopyCellAttributes(15, 15, 1, 3, column, row);
    }
  } else {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_THIRD_LOG, 7, 46);
    {
      s32 column = 17;
      s32 row = 45;
      MapGrid_CopyCellAttributes(15, 15, 1, 3, column, row);
    }
  }
}

/// The fade overlay follows the leader through the dragon statue room. Touching
/// step trigger 15 in Fuchin Falls Cave's dragon statue room makes it follow,
/// once it has been started there.
static void FuchinFallsCave_FollowLeaderWithLight(void) {
  if (gImilFuchinFalls_TrackLeader != 0) {
    struct FadeOverlayWork *work = *(gHeapSlots + 36);

    work->actor = Actor_GetById(ACTOR_PARTY_LEADER);
  }
}

/// Touching step trigger 16 in the dragon statue room stops the fade overlay
/// following the leader.
static void FuchinFallsCave_StopLightFollowingLeader(void) {
  if (gImilFuchinFalls_TrackLeader != 0) {
    struct FadeOverlayWork *work = *(gHeapSlots + 36);

    work->actor = NULL;
  }
}

/// Sets up the scene as the party enters. Every normal entry into Imil Falls
/// Cave blocks Retreat and walks the leader toward Fever; otherwise each room
/// of Fuchin Falls Cave restores its logs, the Dragon's Eye and its lighting.
s32 ImilFuchinFalls_Initialize(void) {
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0 &&
      gAdventure.sceneId == (s32)&SceneId_ImilFallsCave) {
    GameFlag_Set(FLAG_RETREAT_BLOCKED);
    Imil_WalkTowardFever();
  } else {
    ImilFuchinFalls_PrepareRoom();
  }
  return 0;
}

/// Shows Fever, in Imil Falls Cave, and walks the leader toward it.
static void Imil_WalkTowardFever(void) {
  struct Actor *actor;

  Event_Start();
  actor = Actor_GetById(ACTOR_FEVER);
  Actor_SetSpriteFlags(actor, 0);
  Event_StartScreenTransition();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(2), PIXELS(1) / 10);
  Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 264, 196);
  Event_End();
}

/// Sets up a room of Fuchin Falls Cave for the entrance it is entered
/// by: its logs, the Dragon's Eye and two foreground sprites in the third
/// room, and the darkness and the fade overlay in the dragon statue's room.
static void ImilFuchinFalls_PrepareRoom(void) {
  struct Actor *actor;
  /* FAKEMATCH(temporary): one variable holds the zeros and then the slot word,
   * so case 5 stores a fresh 0 and gAdventure stays in r6; as literal
   * zeros and the slot read in place, 51 of 144 instructions differ in tbs-en.
   */
  s32 value;

  ImilFuchinFalls_RebuildLogRoom();
  if (gAdventure.sceneId == (s32)&SceneId_FuchinFallsCaveDragonsEye) {
    if (!GameFlag_Get(FLAG_FUCHIN_FALLS_DRAGONS_EYE_FOUND) &&
        gAdventure.entrance ==
            FUCHIN_FALLS_CAVE_DRAGONS_EYE_CHAMBER_FROM_LOGS) {
      ImilFuchinFalls_PlaceDragonsEye();
    }
    if ((u16)(gAdventure.entrance - 2) <= 3) {
      FuchinFallsCave_CreateForegroundActor(PIXELS(156), 0, PIXELS(452),
                                            SPRITE_CRATE);
      FuchinFallsCave_CreateForegroundActor(PIXELS(188), 0, PIXELS(452),
                                            SPRITE_CRATE);
    }
  } else if (gAdventure.sceneId == (s32)&SceneId_FuchinFallsCaveDragonStatue) {
    value = 0;
    actor = Actor_Get(ACTOR_INVISIBLE_DRAGON_ROOM_CRATE);
    gImilFuchinFalls_TrackLeader = value;
    actor->motionFlags = value;
    actor->y.fixed = value;
    Actor_SetSpritePriority(ACTOR_INVISIBLE_DRAGON_ROOM_CRATE, 1);
    Actor_SetSpriteColorById(ACTOR_INVISIBLE_DRAGON_ROOM_CRATE,
                             SPRITE_COLOR_HIDDEN);
    switch (gAdventure.entrance) {
    case FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_ORB_OF_FORCE_EAST:
    case FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_ORB_OF_FORCE_WEST:
      FieldEffect_StartFadeOverlay(0);
      gImilFuchinFalls_TrackLeader = 1;
      break;
    case FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_FROM_SOUTH_HALF_MIDDLE:
      FieldEffect_StartFadeOverlay(0);
      gImilFuchinFalls_TrackLeader = 1;
      value = *(s32 *)(gHeapSlots + 36);
      ((struct FadeOverlayWork *)value)->actor = NULL;
      break;
    }
    if (gAdventure.entrance <=
        FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_FROM_SOUTH_HALF_EAST) {
      if (GameFlag_Get(FLAG_FUCHIN_FALLS_DRAGONS_EYE_SET)) {
        MapGrid_CopyCellsTo(30, 57, 19, 57, 1, 1);
        MapGrid_CopyCellsTo(30, 8, 12, 8, 8, 7);
      } else {
        gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
        PaletteFade_ApplySource(PALETTE_TINT_CUSTOM | RGB(8, 8, 12),
                                PALETTES_BG);
        PaletteFade_ApplyTarget(PALETTE_TINT_CUSTOM | RGB(8, 8, 12),
                                PALETTES_BG);
        PaletteFade_StartInterpolation(1);
        WaitFrames(1);
      }
    }
  }
}

/// Restores the cell behavior templates for the current log room of Fuchin
/// Falls Cave.
static void ImilFuchinFalls_RebuildLogRoom(void) {
  if (gAdventure.sceneId == (s32)&SceneId_FuchinFallsCaveEntrance) {
    MapGrid_CopyCellAttributes(8, 29, 15, 5, 8, 42);
    FuchinFallsCave_RebuildEntranceLogs();
  } else if (gAdventure.sceneId == (s32)&SceneId_FuchinFallsCaveZephyr) {
    MapGrid_CopyCellAttributes(12, 8, 10, 18, 0, 28);
    FuchinFallsCave_RebuildZephyrRoomLogs();
  } else if (gAdventure.sceneId == (s32)&SceneId_FuchinFallsCaveDragonsEye &&
             gAdventure.entrance !=
                 FUCHIN_FALLS_CAVE_DRAGONS_EYE_CHAMBER_FROM_LOGS) {
    MapGrid_CopyCellAttributes(12, 21, 9, 16, 12, 3);
    FuchinFallsCave_RebuildDragonsEyeRoomLogs();
  } else if (gAdventure.sceneId == (s32)&SceneId_FuchinFallsCaveArcticBlade) {
    if (gAdventure.entrance ==
            FUCHIN_FALLS_CAVE_ARCTIC_BLADE_FROM_FUCHIN_FALLS_CAVE_DRAGON_STATUE ||
        gAdventure.entrance ==
            FUCHIN_FALLS_CAVE_ARCTIC_BLADE_NORTH_HALF_FROM_SOUTH_HALF) {
      MapGrid_CopyCellAttributes(14, 10, 9, 8, 22, 20);
    } else {
      MapGrid_CopyCellAttributes(7, 45, 11, 4, 20, 45);
    }
    FuchinFallsCave_RebuildArcticBladeRoomLogs();
  }
}

/// Hops across a gap in Fuchin Falls Cave: west or north in the second room
/// (step triggers 13, 14 and 17), south in the third (step trigger 25) when its
/// first log lies at FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_MIDDLE.
static void ImilFuchinFalls_HopOnTrigger(void) {
  s32 trigger = gFieldWork->stepTrigger;

  if (gAdventure.sceneId == (s32)&SceneId_FuchinFallsCaveZephyr) {
    if (trigger == 17) {
      ImilFuchinFalls_HopBy(0, -32);
    } else {
      ImilFuchinFalls_HopBy(-32, 0);
    }
  }
  if (gAdventure.sceneId == (s32)&SceneId_FuchinFallsCaveDragonsEye &&
      trigger == 25 &&
      GameFlag_Get(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_MIDDLE)) {
    ImilFuchinFalls_HopBy(0, 32);
  }
}

/// Hops the leader one gap north, in Fuchin Falls Cave's third room (
/// step triggers 11 and 12).
static void FuchinFallsCave_HopNorth(void) { ImilFuchinFalls_HopBy(0, -32); }

/// Hops the leader one gap west, in Fuchin Falls Cave's third room (step
/// trigger 23).
static void FuchinFallsCave_HopWest(void) { ImilFuchinFalls_HopBy(-32, 0); }

/// Jumps the leader by a pixel offset while using the hopping animations.
static void ImilFuchinFalls_HopBy(s32 dx, s32 dz) {
  Event_Start();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(5) / 2, PIXELS(5) / 4);
  Actor_OffsetPositionAndResetMotion(ACTOR_PARTY_LEADER, dx, dz);
  Actor_Jump(ACTOR_PARTY_LEADER, 4, 0);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_LEAP);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_SPRING);
  Event_End();
}

/// Shows the Dragon's Eye in Fuchin Falls Cave's third room, as its item icon.
static void ImilFuchinFalls_PlaceDragonsEye(void) {
  struct Actor *actor;
  struct Sprite *sprite;
  GlyphTransfer *glyph;
  u8 clear = 0;

  actor = FieldActor_Create(SPRITE_BLANK, PIXELS(248), PIXELS(8), PIXELS(152));
  if (actor != NULL) {
    sprite = actor->sprite;
    sprite->flags = clear;
    sprite->count = clear;
    sprite->image.fullColor = 0;
    sprite->image.palette = 0;
    actor->motionFlags = clear;
    actor->retainSpriteCache = 1;
    glyph = HeapSlot_Alloc(HEAP_SLOT_GLYPH_TRANSFER, sizeof(*glyph));
    Icon_DrawItem(ITEM_DRAGONS_EYE);
    VramCache_Load(sprite->slot, 128, glyph->tiles);
    HeapSlot_Release(HEAP_SLOT_GLYPH_TRANSFER);
    gImilFuchinFalls_DragonsEye = actor;
  }
}

/// Searching where the Dragon's Eye lies, in Fuchin Falls Cave's third room
/// (examine trigger 30), gives it to the party
/// (FLAG_FUCHIN_FALLS_DRAGONS_EYE_FOUND).
static void FuchinFallsCave_CollectDragonsEye(void) {
  Event_Start();

  if (gImilFuchinFalls_DragonsEye != NULL) {
    ItemDisplay_ShowPicture(gImilFuchinFalls_DragonsEye, 3);
  }

  PartyInventory_GiveItem(ITEM_DRAGONS_EYE, 0);
  GameFlag_Set(FLAG_FUCHIN_FALLS_DRAGONS_EYE_FOUND);

  if (gImilFuchinFalls_DragonsEye != NULL) {
    Actor_Release(gImilFuchinFalls_DragonsEye);
  }

  Event_End();
}

/// Applies the dust velocity and scale change for one frame.
static void FuchinFallsCave_UpdateDustMotion(struct Actor *raw) {
  struct FieldEffect *object = ((struct FieldEffect *)raw);

  object->x += object->velocity.x;
  object->y += object->velocity.y;
  object->z += object->velocity.z;
  object->scaleX += object->scaleRateX;
  object->scaleY += object->scaleRateY;
}

/// Hides the dust (SPRITE_COLOR_HIDDEN) when its actor script finishes
/// looping.
static s32 FuchinFallsCave_SetDustPalette(struct Actor *interpreter) {
  struct Actor *actor = interpreter;
  Actor_SetSpriteColor(actor, SPRITE_COLOR_HIDDEN);
  return 0;
}

/// Creates scripted cave dust with the requested velocity, palette and scale.
void ImilFuchinFalls_SpawnDust(s32 x, s32 y, s32 z, s32 velocityX,
                               s32 velocityY, s32 velocityZ, u32 flags,
                               const struct EffectOptions *extra) {
  struct FieldEffect *effect;
  struct Sprite *sprite;
  const s32 *script;
  // A local array: it compiles to an anonymous read-only constant copied into
  // the frame, which lands where the object's old .data table was; a struct
  // copy of a named table compiles to the same code.
  const s32 *scripts[3] = {
      gImilFuchinFalls_ShortDustScript,
      gImilFuchinFalls_MediumDustScript,
      gImilFuchinFalls_LongDustScript,
  };

  effect = (struct FieldEffect *)Actor_Create(SPRITE_DUST_PUFF, x, y, z);
  if (effect == 0)
    return;
  sprite = effect->sprite;
  Actor_SelectAnimation(effect, (flags + 1) & EFFECT_SCRIPT_MASK);
  Actor_StartScript((struct Actor *)effect,
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
    Actor_SetSpriteColor((struct Actor *)effect, extra->palette);
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
    Sound_Play(SE_WHOOSH);
  velocityX = -PIXELS(1) - ((((u32)Import_RandomNext() << 1) >> 16) << 16);
  velocityY = -((((u32)Import_RandomNext() * 3) >> 16) * (PIXELS(1) / 5));
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
  Sound_Play(MUS_SILENCE);
  Sound_Play(SE_GEM_SET);
  Event_Start();
  Field_EndBusyAndReveal();

  for (pass = 0; pass < 4; pass++) {
    PaletteFade_SetTarget(PALETTE_TINT_CUSTOM | RGB(24, 24, 16), PALETTES_BG);
    PaletteFade_StartInterpolation(1);
    WaitFrames(2);
    if (pass == 0) {
      MapGrid_CopyCellsTo(30, 8, 12, 8, 8, 7);
      MapGrid_CopyCellsTo(30, 57, 19, 57, 1, 1);
    }
    PaletteFade_SetTarget(PALETTE_TINT_CUSTOM | RGB(8, 8, 12), PALETTES_BG);
    PaletteFade_StartInterpolation(1);
    WaitFrames(2);
  }

  WaitFrames(30);

  // Blow dust from the cave mouth while the room brightens.
  Task_AddFrame(ImilFuchinFalls_BlowCaveMouthDust, TASK_PASS_FRAME);
  WaitFrames(40);
  PaletteFade_SetTarget(PALETTE_TINT_CUSTOM | RGB(16, 4, 4), PALETTES_BG);
  PaletteFade_StartInterpolation(40);
  WaitFrames(80);
  Task_Remove(ImilFuchinFalls_BlowCaveMouthDust);
  WaitFrames(20);

  // Settle the lighting, consume the eye and remember the revealed path.
  PaletteFade_SetTarget(PALETTE_TINT_NONE, PALETTES_BG);
  PaletteFade_StartInterpolation(80);

  WaitFrames(80);

  GameFlag_Set(FLAG_FUCHIN_FALLS_DRAGONS_EYE_SET);
  PartyInventory_Discard(ITEM_DRAGONS_EYE);
  Scene_PlayMusic();

  Event_End();
}

/// Searching the inscription in Fuchin Falls Cave's first room (examine trigger
/// 1): `The dragon's flame illuminates the path of truth in darkness.`
static void FuchinFallsCave_ReadDragonFlameInscription(void) {
  Event_Start();
  FieldMessage_Show(MsgFuchinDragonFlameShowsPath, 1);
  Event_End();
}

/// Searching the inscription in Fuchin Falls Cave's Ki room (examine trigger
/// 7): `The secret of Ki shall be revealed to the disciples of truth.`
static void FuchinFallsCave_ReadKiInscription(void) {
  Event_Start();
  FieldMessage_Show(MsgFuchinSecretKiRevealed, 1);
  Event_End();
}

/// Searching the inscription in Fuchin Falls Cave's dragon statue room
/// (examine trigger 19): `Rays of light give birth to shadows, revealing the
/// way.`
static void FuchinFallsCave_ReadShadowInscription(void) {
  Event_Start();
  FieldMessage_Show(MsgFuchinLightRevealsShadows, 1);
  Event_End();
}

/// Searching the dragon statue in Fuchin Falls Cave (examine trigger 20): its
/// burning red eyes once set, else its empty sockets; with the Dragon's Eye
/// carried it then opens the File Screen as the search ends, so that the eye
/// can be used.
static void FuchinFallsCave_SearchDragonStatue(void) {
  Event_Start();

  if (GameFlag_Get(FLAG_FUCHIN_FALLS_DRAGONS_EYE_SET) != 0) {
    FieldMessage_Show(MsgFuchinDragonRedEyes, 1);
  } else {
    FieldMessage_Show(MsgFuchinEyelessDragon, 1);
    if (PartyInventory_FindMember(ITEM_DRAGONS_EYE) != -1) {
      gFieldWork->fieldMenu = 1;
    }
  }
  Event_End();
}

/// The dragon statue room goes dark again when Reveal ends. That happens in
/// Fuchin Falls Cave only before the eye is set.
static void FuchinFallsCave_EndRevealLighting(void) {
  struct FieldEffectWork *effectWork;
  struct ScreenTransitionState *state;

  if (gAdventure.entrance <=
      FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_FROM_SOUTH_HALF_EAST) {
    effectWork = *(gHeapSlots + 56);
    state = *(gHeapSlots + 31);
    effectWork->childOption = 1;
    state->nextMode = 0;
    state->duration = 1;
    state->step = 1;
    PaletteFade_SetSource(RGB(0, 0, 0), PALETTES_BG);
    PaletteFade_SetTarget(PALETTE_TINT_CUSTOM | RGB(8, 8, 12), PALETTES_BG);
    PaletteFade_StartInterpolation(16);
    WaitFrames(16);
  }
}

/// Turns the actor sliding on Imil Falls Cave's ice gradually toward the
/// held direction.
static void Imil_TurnSkaterTowardHeldDirection(struct Actor *updated) {
  struct Actor *actor = updated;
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
    Actor_SelectAnimation(actor, 9);
    return;
  }
  facing = actor->facing;
  direction = (s16)(direction - facing);
  if (direction > 0x1000)
    direction = 0x1000;
  if (direction < -0x1000)
    direction = -0x1000;
  actor->facing = facing + direction;
  Actor_SelectAnimation(actor, 2);
  Actor_SetAnimationSpeed(actor, IMIL_SLIDE_ANIMATION_STEP);
}

/// Slides along Imil Falls Cave's ice until terrain or a cell behavior
/// boundary ends the move.
static void Imil_SlideActorAcrossIce(void) {
  struct Actor *actor;
  struct FieldPosition probe;
  s32 heading;
  s32 startBehavior;
  s32 nextBehavior;
  s32 z;
  s32 x;

  actor = Actor_Lookup(gAdventure.leaderActorId);

  for (;;) {
    heading = gImilFuchinFalls_SlideHeadings[(gKeysHeld >> 4) & 15];
    if (((u32)heading << 16) == 0xffff0000) {
      return;
    }

    Event_Start();

    // Start at the cell center and reject a blocked or raised next cell.
    probe.x = (actor->x.fixed & 0xfff00000) + PIXELS(8);
    probe.y = actor->y.fixed;
    probe.z = (actor->z.fixed & 0xfff00000) + PIXELS(8);
    z = probe.z;
    x = probe.x;
    startBehavior = MapGrid_GetCellBehaviorAt(actor->layer, x, z);
    FieldVector_AddPolarOffset(PIXELS(16), heading, &probe);

    nextBehavior = MapGrid_GetCellBehaviorAt(actor->layer, probe.x, probe.z);
    if (nextBehavior == CELL_BEHAVIOR_WALL ||
        MapGrid_GetTerrainHeight(actor->layer, probe.x, probe.z) -
                actor->y.fixed >
            PIXELS(8)) {
      actor->facing = heading;
      goto tail;
    }

    probe.x = x;
    probe.z = z;
    actor->speedLimit = PIXELS(2);
    actor->acceleration = PIXELS(1) / 10;
    actor->timer = 0;
    FieldActor_SetMoveTarget(actor, x, actor->y.fixed, z);

    Actor_SelectAnimation(actor, 2);
    Actor_SetAnimationSpeed(actor, IMIL_SLIDE_ANIMATION_STEP);
    Actor_WaitForMove(actor);
    actor->update = Imil_TurnSkaterTowardHeldDirection;

    // Keep sliding through cells with the same cell behavior.
    for (;;) {
      FieldVector_AddPolarOffset(PIXELS(16), heading, &probe);
      nextBehavior = MapGrid_GetCellBehaviorAt(actor->layer, probe.x, probe.z);
      if (nextBehavior == CELL_BEHAVIOR_WALL) {
        break;
      }
      if (MapGrid_GetTerrainHeight(actor->layer, probe.x, probe.z) -
              actor->y.fixed >
          PIXELS(8)) {
        break;
      }
      x = probe.x;
      z = probe.z;
      actor->speedLimit = PIXELS(2);
      actor->acceleration = PIXELS(1) / 10;
      FieldActor_SetMoveTarget(actor, probe.x, probe.y, probe.z);
      Actor_WaitForMove(actor);
      if (nextBehavior != startBehavior) {
        goto blocked;
      }
    }

    actor->speedLimit = PIXELS(2);
    actor->acceleration = PIXELS(1);
    FieldActor_SetMoveTarget(actor, x, actor->y.fixed, z);
    Actor_WaitForMove(actor);
    WaitFrames(2);
  }

blocked:
  // A cell behavior change ends the slide and restores normal steering.
  actor->update = NULL;
  actor->steeringFlags |= ACTOR_FACES_MOTION;

  actor->acceleration = PIXELS(1) / 4;

tail:
  WaitFrames(10);
  Event_End();
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

struct Actor *gImilFuchinFalls_DragonsEye = NULL;

struct SceneEntrance gImilFuchinFalls_NoEntrances[] = {
    { 0, CONDITION_ALWAYS, 256, 0, 600, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gImilFuchinFalls_ImilFallsCaveEntrances[] = {
    { 0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { IMIL_FALLS_CAVE_FROM_IMIL, CONDITION_ALWAYS, 264, 0, 289, FACING_NORTH, 0, 135, 62, 376, 328, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gImilFuchinFalls_EntranceEntrances[] = {
    { 0, CONDITION_ALWAYS, 248, 0, 600, FACING_SOUTH, 0, 64, 64, 416, 656, 0 },
    { FUCHIN_FALLS_CAVE_ENTRANCE_FROM_FUCHIN_TEMPLE_GROUNDS, CONDITION_ALWAYS, 248, 0, 632, FACING_NORTH, 0, 64, 368, 416, 656, 0 },
    { FUCHIN_FALLS_CAVE_ENTRANCE_FROM_FUCHIN_FALLS_CAVE_ZEPHYR, CONDITION_ALWAYS, 376, 0, 488, FACING_SOUTH, 0, 64, 368, 416, 656, 0 },
    { FUCHIN_FALLS_CAVE_ENTRANCE_FROM_FUCHIN_FALLS_CAVE_ARCTIC_BLADE, CONDITION_ALWAYS, 120, 0, 488, FACING_SOUTH, 0, 64, 368, 416, 656, 0 },
    { FUCHIN_FALLS_CAVE_ENTRANCE_SOUTH_HALF_FROM_NORTH_HALF, CONDITION_ALWAYS, 232, 0, 408, FACING_SOUTH, 0, 64, 368, 416, 656, 0 },
    { FUCHIN_FALLS_CAVE_ENTRANCE_NORTH_HALF_FROM_SOUTH_HALF, CONDITION_ALWAYS, 232, 0, 344, FACING_NORTH, 0, 64, 48, 416, 376, 0 },
    { FUCHIN_FALLS_CAVE_ENTRANCE_FROM_FUCHIN_FALLS_CAVE_DRAGONS_EYE, CONDITION_ALWAYS, 376, 0, 152, FACING_NORTH, 0, 64, 48, 416, 376, 0 },
    { FUCHIN_FALLS_CAVE_ENTRANCE_FROM_FUCHIN_FALLS_CAVE_DRAGON_STATUE_WEST, CONDITION_ALWAYS, 120, 0, 200, FACING_SOUTH, 0, 64, 48, 416, 376, 0 },
    { FUCHIN_FALLS_CAVE_ENTRANCE_FROM_FUCHIN_FALLS_CAVE_DRAGON_STATUE_EAST, CONDITION_ALWAYS, 248, 0, 104, FACING_SOUTH, 0, 64, 48, 416, 376, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gImilFuchinFalls_ZephyrRoomEntrances[] = {
    { FUCHIN_FALLS_CAVE_ZEPHYR_FROM_FUCHIN_FALLS_CAVE_DRAGONS_EYE, CONDITION_ALWAYS, 248, 0, 120, FACING_SOUTH, 0, 96, 64, 416, 480, 0 },
    { FUCHIN_FALLS_CAVE_ZEPHYR_FROM_FUCHIN_FALLS_CAVE_ENTRANCE, CONDITION_ALWAYS, 152, 0, 200, FACING_NORTH, 0, 96, 64, 416, 480, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gImilFuchinFalls_DragonsEyeRoomEntrances[] = {
    { FUCHIN_FALLS_CAVE_DRAGONS_EYE_CHAMBER_FROM_LOGS, CONDITION_ALWAYS, 248, 0, 104, FACING_SOUTH, 0, 128, 64, 368, 256, 0 },
    { FUCHIN_FALLS_CAVE_DRAGONS_EYE_FROM_FUCHIN_FALLS_CAVE_DRAGON_STATUE, CONDITION_ALWAYS, 184, 0, 328, FACING_SOUTH, 0, 96, 256, 416, 656, 0 },
    { FUCHIN_FALLS_CAVE_DRAGONS_EYE_LOGS_FROM_CHAMBER, CONDITION_ALWAYS, 152, 0, 432, FACING_SOUTH, 0, 96, 256, 416, 656, 0 },
    { FUCHIN_FALLS_CAVE_DRAGONS_EYE_FROM_FUCHIN_FALLS_CAVE_ENTRANCE, CONDITION_ALWAYS, 184, 0, 552, FACING_SOUTH, 0, 96, 256, 416, 656, 0 },
    { FUCHIN_FALLS_CAVE_DRAGONS_EYE_FROM_FUCHIN_FALLS_CAVE_ZEPHYR, CONDITION_ALWAYS, 280, 0, 616, FACING_NORTH, 0, 96, 256, 416, 656, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gImilFuchinFalls_ArcticBladeRoomEntrances[] = {
    { FUCHIN_FALLS_CAVE_ARCTIC_BLADE_FROM_FUCHIN_FALLS_CAVE_DRAGON_STATUE, CONDITION_ALWAYS, 328, 0, 72, FACING_SOUTH, 0, 64, 16, 416, 416, 0 },
    { FUCHIN_FALLS_CAVE_ARCTIC_BLADE_NORTH_HALF_FROM_SOUTH_HALF, CONDITION_ALWAYS, 248, 0, 392, FACING_NORTH, 0, 64, 16, 416, 416, 0 },
    { FUCHIN_FALLS_CAVE_ARCTIC_BLADE_SOUTH_HALF_FROM_NORTH_HALF, CONDITION_ALWAYS, 248, 0, 472, FACING_SOUTH, 0, 64, 416, 416, 800, 0 },
    { FUCHIN_FALLS_CAVE_ARCTIC_BLADE_FROM_FUCHIN_FALLS_CAVE_ENTRANCE, CONDITION_ALWAYS, 344, 0, 632, FACING_NORTH, 0, 64, 416, 416, 800, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gImilFuchinFalls_KiRoomEntrances[] = {
    { FUCHIN_FALLS_CAVE_ORB_OF_FORCE_NORTH_HALF_FROM_SOUTH_HALF, CONDITION_ALWAYS, 264, 0, 232, FACING_NORTH, 0, 128, 16, 384, 256, 0 },
    { FUCHIN_FALLS_CAVE_ORB_OF_FORCE_SOUTH_HALF_FROM_NORTH_HALF, CONDITION_ALWAYS, 248, 0, 472, FACING_SOUTH, 0, 64, 256, 416, 720, 0 },
    { FUCHIN_FALLS_CAVE_ORB_OF_FORCE_FROM_FUCHIN_FALLS_CAVE_DRAGON_STATUE_WEST, CONDITION_ALWAYS, 216, 0, 664, FACING_NORTH, 0, 64, 256, 416, 720, 0 },
    { FUCHIN_FALLS_CAVE_ORB_OF_FORCE_FROM_FUCHIN_FALLS_CAVE_DRAGON_STATUE_EAST, CONDITION_ALWAYS, 280, 0, 664, FACING_NORTH, 0, 64, 256, 416, 720, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gImilFuchinFalls_DragonRoomEntrances[] = {
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
    SCENE_EXIT(FUCHIN_FALLS_CAVE_DRAGONS_EYE_CHAMBER_TO_LOGS, FuchinFallsCaveDragonsEye, FUCHIN_FALLS_CAVE_DRAGONS_EYE_LOGS_FROM_CHAMBER),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_DRAGONS_EYE_TO_FUCHIN_FALLS_CAVE_DRAGON_STATUE, FuchinFallsCaveDragonStatue, FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_DRAGONS_EYE),
    SCENE_EXIT(FUCHIN_FALLS_CAVE_DRAGONS_EYE_LOGS_TO_CHAMBER, FuchinFallsCaveDragonsEye, FUCHIN_FALLS_CAVE_DRAGONS_EYE_CHAMBER_FROM_LOGS),
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

struct ActorPlacement gImilFuchinFalls_NoActorPlacements[] = {
    { SCENE_TABLE_END },
};

struct ActorPlacement gImilFuchinFalls_ImilFallsCaveActorPlacements[] = {
    { MARS_DJINNI, FLAG_DJINNI(DJINNI_FEVER), MOVEMENT_TYPE_STAND,
        PIXELS(263), 0, PIXELS(204), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ActorPlacement gImilFuchinFalls_EntranceActorPlacements[] = {
    { SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(136), 0, PIXELS(504), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_FLOATING_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(200), 0, PIXELS(536), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_FLOATING_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(296), 0, PIXELS(536), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SCENE_TABLE_END },
};

struct ActorPlacement gImilFuchinFalls_ZephyrRoomActorPlacements[] = {
    { SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(216), 0, PIXELS(328), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_FLOATING_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(248), 0, PIXELS(280), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_FLOATING_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(312), 0, PIXELS(408), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { JUPITER_DJINNI, FLAG_DJINNI(DJINNI_ZEPHYR), MOVEMENT_TYPE_STAND,
        PIXELS(344), 0, PIXELS(280), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ActorPlacement gImilFuchinFalls_DragonsEyeRoomActorPlacements[] = {
    { SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(328), 0, PIXELS(408), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(248), 0, PIXELS(568), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_FLOATING_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(248), 0, PIXELS(488), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_FLOATING_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(248), 0, PIXELS(504), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SCENE_TABLE_END },
};

struct ActorPlacement gImilFuchinFalls_ArcticBladeRoomActorPlacements[] = {
    { SPRITE_FLOATING_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(328), 0, PIXELS(168), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(360), 0, PIXELS(264), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(120), 0, PIXELS(744), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SCENE_TABLE_END },
};

struct ActorPlacement gImilFuchinFalls_DragonRoomActorPlacements[] = {
    { SPRITE_CRATE, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(312), 0, PIXELS(128), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SCENE_TABLE_END },
};

struct Trigger gImilFuchinFalls_NoTriggers[] = {
    { TRIGGERS_END },
};

struct Trigger gImilFuchinFalls_ImilFallsCaveTriggers[] = {
    { TRIGGER_STEP, 1, CONDITION_ALWAYS, EVENT(Imil_LeaveFallsCave) },
    { TRIGGER_STEP, 10, CONDITION_ALWAYS, EVENT(Imil_MoveAcrossIce) },
    { TRIGGER_TALK, ACTOR_FEVER, CONDITION_ALWAYS, EVENT(Imil_RecruitFever) },
    { TRIGGERS_END },
};

struct Trigger gImilFuchinFalls_EntranceTriggers[] = {
    { STEP_TRIGGER_WALKING_INTO(FACING_EAST), 11, CONDITION_ALWAYS, EVENT(FuchinFallsCave_Entrance_MoveFirstLogEast) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), 12, CONDITION_ALWAYS, EVENT(FuchinFallsCave_Entrance_MoveFirstLogWest) },
    { TRIGGER_STEP, 11, CONDITION_ALWAYS, EVENT(FuchinFallsCave_AnimateFirstLog) },
    { TRIGGER_STEP, 12, CONDITION_ALWAYS, EVENT(FuchinFallsCave_AnimateFirstLog) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 13, CONDITION_ALWAYS, EVENT(FuchinFallsCave_Entrance_MoveSecondLogSouth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 14, CONDITION_ALWAYS, EVENT(FuchinFallsCave_Entrance_MoveSecondLogNorth) },
    { TRIGGER_STEP, 13, CONDITION_ALWAYS, EVENT(FuchinFallsCave_AnimateSecondLog) },
    { TRIGGER_STEP, 14, CONDITION_ALWAYS, EVENT(FuchinFallsCave_AnimateSecondLog) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 15, CONDITION_ALWAYS, EVENT(FuchinFallsCave_Entrance_MoveThirdLogSouth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 16, CONDITION_ALWAYS, EVENT(FuchinFallsCave_Entrance_MoveThirdLogNorth) },
    { TRIGGER_STEP, 15, CONDITION_ALWAYS, EVENT(FuchinFallsCave_AnimateThirdLog) },
    { TRIGGER_STEP, 16, CONDITION_ALWAYS, EVENT(FuchinFallsCave_AnimateThirdLog) },
    { TRIGGER_EXIT, 21, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_ENTRANCE_TO_FUCHIN_TEMPLE_GROUNDS },
    { TRIGGER_EXIT, 22, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_ENTRANCE_TO_FUCHIN_FALLS_CAVE_ZEPHYR },
    { TRIGGER_EXIT, 23, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_ENTRANCE_TO_FUCHIN_FALLS_CAVE_ARCTIC_BLADE },
    { TRIGGER_EXIT, 24, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_ENTRANCE_SOUTH_HALF_TO_NORTH_HALF },
    { TRIGGER_EXIT, 25, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_ENTRANCE_NORTH_HALF_TO_SOUTH_HALF },
    { TRIGGER_EXIT, 26, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_ENTRANCE_TO_FUCHIN_FALLS_CAVE_DRAGONS_EYE },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 27, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_ENTRANCE_TO_FUCHIN_FALLS_CAVE_DRAGON_STATUE_WEST },
    { TRIGGER_EXIT, 28, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_ENTRANCE_TO_FUCHIN_FALLS_CAVE_DRAGON_STATUE_EAST },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), 1, CONDITION_ALWAYS, EVENT(FuchinFallsCave_ReadDragonFlameInscription) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 100, 0xef1, FIND_MIMIC | 1 },
    { TRIGGERS_END },
};

struct Trigger gImilFuchinFalls_ZephyrRoomTriggers[] = {
    { STEP_TRIGGER_WALKING_INTO(FACING_EAST), 11, FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_SECOND_LOG_SOUTH, EVENT(FuchinFallsCave_ZephyrRoom_MoveFirstLogToEastEnd) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), 11, CONDITION_ALWAYS, EVENT(FuchinFallsCave_ZephyrRoom_NudgeFirstLogWest) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 17, CONDITION_ALWAYS, EVENT(ImilFuchinFalls_HopOnTrigger) },
    { STEP_TRIGGER_WALKING_INTO(FACING_EAST), 17, CONDITION_ALWAYS, EVENT(FuchinFallsCave_ZephyrRoom_MoveFirstLogEast) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), 12, CONDITION_ALWAYS, EVENT(FuchinFallsCave_ZephyrRoom_MoveFirstLogToWestEnd) },
    { TRIGGER_STEP, 11, CONDITION_ALWAYS, EVENT(FuchinFallsCave_AnimateFirstLog) },
    { TRIGGER_STEP, 12, CONDITION_ALWAYS, EVENT(FuchinFallsCave_AnimateFirstLog) },
    { TRIGGER_STEP, 17, CONDITION_ALWAYS, EVENT(FuchinFallsCave_AnimateFirstLog) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), 13, CONDITION_ALWAYS, EVENT(ImilFuchinFalls_HopOnTrigger) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), 14, FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_EAST_END, EVENT(ImilFuchinFalls_HopOnTrigger) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 13, CONDITION_ALWAYS, EVENT(FuchinFallsCave_ZephyrRoom_MoveSecondLogSouth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 14, CONDITION_ALWAYS, EVENT(FuchinFallsCave_ZephyrRoom_MoveSecondLogNorth) },
    { TRIGGER_STEP, 13, CONDITION_ALWAYS, EVENT(FuchinFallsCave_AnimateSecondLog) },
    { TRIGGER_STEP, 14, CONDITION_ALWAYS, EVENT(FuchinFallsCave_AnimateSecondLog) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 15, CONDITION_ALWAYS, EVENT(FuchinFallsCave_ZephyrRoom_MoveThirdLogSouth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 16, CONDITION_ALWAYS, EVENT(FuchinFallsCave_ZephyrRoom_MoveThirdLogNorth) },
    { TRIGGER_STEP, 15, CONDITION_ALWAYS, EVENT(FuchinFallsCave_AnimateThirdLog) },
    { TRIGGER_STEP, 16, CONDITION_ALWAYS, EVENT(FuchinFallsCave_AnimateThirdLog) },
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_ZEPHYR_TO_FUCHIN_FALLS_CAVE_DRAGONS_EYE },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_ZEPHYR_TO_FUCHIN_FALLS_CAVE_ENTRANCE },
    { TRIGGER_TALK, ACTOR_ZEPHYR, CONDITION_ALWAYS, EVENT(FuchinFallsCave_FightZephyr) },
    { TRIGGERS_END },
};

struct Trigger gImilFuchinFalls_DragonsEyeRoomTriggers[] = {
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), 10, CONDITION_ALWAYS, EVENT(FuchinFallsCave_DragonsEyeRoom_MoveFirstLogWest) },
    { STEP_TRIGGER_WALKING_INTO(FACING_EAST), 11, CONDITION_ALWAYS, EVENT(FuchinFallsCave_DragonsEyeRoom_MoveFirstLogEastFromMiddle) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), 11, CONDITION_ALWAYS, EVENT(FuchinFallsCave_DragonsEyeRoom_MoveFirstLogWestIfClear) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 11, CONDITION_ALWAYS, EVENT(FuchinFallsCave_HopNorth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_EAST), 12, CONDITION_ALWAYS, EVENT(FuchinFallsCave_DragonsEyeRoom_MoveFirstLogEastFromWestEnd) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 12, IF_FLAG_SET(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END), EVENT(FuchinFallsCave_HopNorth) },
    { TRIGGER_STEP, 10, CONDITION_ALWAYS, EVENT(FuchinFallsCave_AnimateFirstLog) },
    { TRIGGER_STEP, 11, CONDITION_ALWAYS, EVENT(FuchinFallsCave_AnimateFirstLog) },
    { TRIGGER_STEP, 12, CONDITION_ALWAYS, EVENT(FuchinFallsCave_AnimateFirstLog) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), 14, CONDITION_ALWAYS, EVENT(FuchinFallsCave_DragonsEyeRoom_MoveSecondLogWest) },
    { STEP_TRIGGER_WALKING_INTO(FACING_EAST), 13, CONDITION_ALWAYS, EVENT(FuchinFallsCave_DragonsEyeRoom_MoveSecondLogEast) },
    { TRIGGER_STEP, 13, CONDITION_ALWAYS, EVENT(FuchinFallsCave_AnimateSecondLog) },
    { TRIGGER_STEP, 14, CONDITION_ALWAYS, EVENT(FuchinFallsCave_AnimateSecondLog) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 15, CONDITION_ALWAYS, EVENT(FuchinFallsCave_DragonsEyeRoom_MoveThirdLogNorthFromSouthEnd) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 16, CONDITION_ALWAYS, EVENT(FuchinFallsCave_DragonsEyeRoom_MoveThirdLogSouthFromNorthEnd) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 17, CONDITION_ALWAYS, EVENT(FuchinFallsCave_DragonsEyeRoom_NudgeThirdLogNorth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 17, FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_WEST_END, EVENT(FuchinFallsCave_DragonsEyeRoom_MoveThirdLogSouthFromUpperStop) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 18, CONDITION_ALWAYS, EVENT(FuchinFallsCave_DragonsEyeRoom_MoveThirdLogNorthFromMiddle) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 18, FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_UPPER_STOP, EVENT(FuchinFallsCave_DragonsEyeRoom_MoveThirdLogSouthFromMiddle) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 19, CONDITION_ALWAYS, EVENT(FuchinFallsCave_DragonsEyeRoom_MoveThirdLogNorthFromLowerStop) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 19, CONDITION_ALWAYS, EVENT(FuchinFallsCave_DragonsEyeRoom_MoveThirdLogSouthFromLowerStop) },
    { TRIGGER_STEP, 15, CONDITION_ALWAYS, EVENT(FuchinFallsCave_AnimateThirdLog) },
    { TRIGGER_STEP, 16, CONDITION_ALWAYS, EVENT(FuchinFallsCave_AnimateThirdLog) },
    { TRIGGER_STEP, 17, CONDITION_ALWAYS, EVENT(FuchinFallsCave_AnimateThirdLog) },
    { TRIGGER_STEP, 18, CONDITION_ALWAYS, EVENT(FuchinFallsCave_AnimateThirdLog) },
    { TRIGGER_STEP, 19, CONDITION_ALWAYS, EVENT(FuchinFallsCave_AnimateThirdLog) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 20, CONDITION_ALWAYS, EVENT(FuchinFallsCave_DragonsEyeRoom_MoveFourthLogNorth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 21, FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_WEST_END, EVENT(FuchinFallsCave_DragonsEyeRoom_MoveFourthLogSouthFromNorthEnd) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 22, CONDITION_ALWAYS, EVENT(FuchinFallsCave_DragonsEyeRoom_NudgeFourthLogNorth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 22, CONDITION_ALWAYS, EVENT(FuchinFallsCave_DragonsEyeRoom_MoveFourthLogSouthFromUpperStop) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), 23, CONDITION_ALWAYS, EVENT(FuchinFallsCave_HopWest) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 23, CONDITION_ALWAYS, EVENT(FuchinFallsCave_DragonsEyeRoom_MoveFourthLogSouthFromMiddle) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 24, CONDITION_ALWAYS, EVENT(FuchinFallsCave_DragonsEyeRoom_UnusedMoveFourthLogSouthFromLowerStop) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 24, CONDITION_ALWAYS, EVENT(FuchinFallsCave_DragonsEyeRoom_UnusedMoveFourthLogSouthFromLastStop) },
    { TRIGGER_STEP, 20, CONDITION_ALWAYS, EVENT(FuchinFallsCave_AnimateFourthLog) },
    { TRIGGER_STEP, 21, CONDITION_ALWAYS, EVENT(FuchinFallsCave_AnimateFourthLog) },
    { TRIGGER_STEP, 22, CONDITION_ALWAYS, EVENT(FuchinFallsCave_AnimateFourthLog) },
    { TRIGGER_STEP, 23, CONDITION_ALWAYS, EVENT(FuchinFallsCave_AnimateFourthLog) },
    { TRIGGER_STEP, 24, CONDITION_ALWAYS, EVENT(FuchinFallsCave_AnimateFourthLog) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 25, CONDITION_ALWAYS, EVENT(ImilFuchinFalls_HopOnTrigger) },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_DRAGONS_EYE_CHAMBER_TO_LOGS },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_DRAGONS_EYE_TO_FUCHIN_FALLS_CAVE_DRAGON_STATUE },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 3, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_DRAGONS_EYE_LOGS_TO_CHAMBER },
    { TRIGGER_EXIT, 4, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_DRAGONS_EYE_TO_FUCHIN_FALLS_CAVE_ENTRANCE },
    { TRIGGER_EXIT, 5, CONDITION_ALWAYS, FUCHIN_FALLS_CAVE_DRAGONS_EYE_TO_FUCHIN_FALLS_CAVE_ZEPHYR },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), 30, FLAG_FUCHIN_FALLS_DRAGONS_EYE_FOUND, EVENT(FuchinFallsCave_CollectDragonsEye) },
    { TRIGGERS_END },
};
// clang-format on

struct Trigger gImilFuchinFalls_ArcticBladeRoomTriggers[] = {
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 11, CONDITION_ALWAYS,
     EVENT(FuchinFallsCave_ArcticBladeRoom_MoveFirstLogSouth)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 12, CONDITION_ALWAYS,
     EVENT(FuchinFallsCave_ArcticBladeRoom_MoveFirstLogNorth)},
    {TRIGGER_STEP, 11, CONDITION_ALWAYS,
     EVENT(FuchinFallsCave_AnimateFirstLog)},
    {TRIGGER_STEP, 12, CONDITION_ALWAYS,
     EVENT(FuchinFallsCave_AnimateFirstLog)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 13,
     FLAG_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_FIRST_LOG_SOUTH,
     EVENT(FuchinFallsCave_ArcticBladeRoom_MoveSecondLogWest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 14, CONDITION_ALWAYS,
     EVENT(FuchinFallsCave_ArcticBladeRoom_MoveSecondLogEast)},
    {TRIGGER_STEP, 13, CONDITION_ALWAYS,
     EVENT(FuchinFallsCave_AnimateSecondLog)},
    {TRIGGER_STEP, 14, CONDITION_ALWAYS,
     EVENT(FuchinFallsCave_AnimateSecondLog)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 15, CONDITION_ALWAYS,
     EVENT(FuchinFallsCave_ArcticBladeRoom_MoveThirdLogEast)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 16, CONDITION_ALWAYS,
     EVENT(FuchinFallsCave_ArcticBladeRoom_MoveThirdLogWest)},
    {TRIGGER_STEP, 15, CONDITION_ALWAYS,
     EVENT(FuchinFallsCave_AnimateThirdLog)},
    {TRIGGER_STEP, 16, CONDITION_ALWAYS,
     EVENT(FuchinFallsCave_AnimateThirdLog)},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_ARCTIC_BLADE_TO_FUCHIN_FALLS_CAVE_DRAGON_STATUE},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_ARCTIC_BLADE_NORTH_HALF_TO_SOUTH_HALF},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_ARCTIC_BLADE_SOUTH_HALF_TO_NORTH_HALF},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_ARCTIC_BLADE_TO_FUCHIN_FALLS_CAVE_ENTRANCE},
    {EXAMINE_TRIGGER(EXAMINE_CHEST), 100, 0xf6b, FIND_ITEM(ITEM_ARCTIC_BLADE)},
    {TRIGGERS_END},
};

struct Trigger gImilFuchinFalls_KiRoomTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_ORB_OF_FORCE_NORTH_HALF_TO_SOUTH_HALF},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_ORB_OF_FORCE_SOUTH_HALF_TO_NORTH_HALF},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_ORB_OF_FORCE_TO_FUCHIN_FALLS_CAVE_DRAGON_STATUE_WEST},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_ORB_OF_FORCE_TO_FUCHIN_FALLS_CAVE_DRAGON_STATUE_EAST},
    {EXAMINE_TRIGGER(EXAMINE_SILENT), 7, CONDITION_ALWAYS,
     EVENT(FuchinFallsCave_ReadKiInscription)},
    {EXAMINE_TRIGGER(EXAMINE_CHEST), 100, FLAG_FUCHIN_FALLS_ORB_OF_FORCE_FOUND,
     FIND_ITEM(ITEM_ORB_OF_FORCE)},
    {TRIGGERS_END},
};

struct Trigger gImilFuchinFalls_DragonRoomTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_DRAGON_STATUE_TO_FUCHIN_FALLS_CAVE_ORB_OF_FORCE_WEST},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_DRAGON_STATUE_TO_FUCHIN_FALLS_CAVE_ORB_OF_FORCE_EAST},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 3, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_DRAGON_STATUE_TO_FUCHIN_FALLS_CAVE_ARCTIC_BLADE},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_TO_SOUTH_HALF_WEST},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_TO_SOUTH_HALF_MIDDLE},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_TO_SOUTH_HALF_EAST},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_DRAGON_STATUE_SOUTH_HALF_TO_NORTH_HALF_WEST},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_DRAGON_STATUE_SOUTH_HALF_TO_NORTH_HALF_MIDDLE},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_DRAGON_STATUE_SOUTH_HALF_TO_NORTH_HALF_EAST},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 10, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_DRAGON_STATUE_TO_FUCHIN_FALLS_CAVE_DRAGONS_EYE},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_DRAGON_STATUE_TO_FUCHIN_FALLS_CAVE_ENTRANCE_EAST},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 12, CONDITION_ALWAYS,
     FUCHIN_FALLS_CAVE_DRAGON_STATUE_TO_FUCHIN_FALLS_CAVE_ENTRANCE_WEST},
    {TRIGGER_STEP, 15, CONDITION_ALWAYS,
     EVENT(FuchinFallsCave_FollowLeaderWithLight)},
    {TRIGGER_STEP, 16, CONDITION_ALWAYS,
     EVENT(FuchinFallsCave_StopLightFollowingLeader)},
    {EXAMINE_TRIGGER(EXAMINE_SILENT), 19, CONDITION_ALWAYS,
     EVENT(FuchinFallsCave_ReadShadowInscription)},
    {USE_ITEM(ITEM_DRAGONS_EYE), 20, FLAG_FUCHIN_FALLS_DRAGONS_EYE_SET,
     EVENT(FuchinFallsCave_LightDragonWithEye)},
    {EXAMINE_TRIGGER(EXAMINE_SILENT), 20, CONDITION_ALWAYS,
     EVENT(FuchinFallsCave_SearchDragonStatue)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
     FLAG_FUCHIN_FALLS_DRAGONS_EYE_SET,
     EVENT(FuchinFallsCave_EndRevealLighting)},
    {TRIGGERS_END},
};

s32 gImilFuchinFalls_ShortDustScript[] = {
    SC_WAIT(1), SC_LOOP(22, SC_TOP),  SC_CALL(FuchinFallsCave_SetDustPalette),
    SC_WAIT(1), SC_POSITION(0, 0, 0), SC_WAIT(1),
    SC_RELEASE,
};

s32 gImilFuchinFalls_MediumDustScript[] = {
    SC_WAIT(1), SC_LOOP(44, SC_TOP),  SC_CALL(FuchinFallsCave_SetDustPalette),
    SC_WAIT(1), SC_POSITION(0, 0, 0), SC_WAIT(1),
    SC_RELEASE,
};

s32 gImilFuchinFalls_LongDustScript[] = {
    SC_WAIT(1), SC_LOOP(126, SC_TOP), SC_CALL(FuchinFallsCave_SetDustPalette),
    SC_WAIT(1), SC_POSITION(0, 0, 0), SC_WAIT(1),
    SC_RELEASE,
};
