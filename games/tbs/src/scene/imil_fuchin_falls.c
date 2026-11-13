/// @file
/// Imil Falls Cave, north of Imil, and Fuchin Falls Cave, the waterfall grotto
/// of Fuchin Temple's test: two areas in seven scenes.
///
/// - Imil Falls Cave (SceneId_ImilFallsCave), up the way north out of Imil. Its
///   only exit, IMIL_FALLS_CAVE_TO_IMIL, leads back down
///   (IMIL_FROM_IMIL_FALLS_CAVE).
///
///   Fever, a Mars Djinni, waits on its ice. It joins the party when spoken to,
///   without a battle.
///
///   Every normal entry blocks Retreat and walks the leader up toward Fever.
///   Stepping onto the ice (cell behavior 10) slides the leader in the held
///   direction. He turns toward it as he goes, until a blocked cell or one too
///   high stops him.
///
/// - Fuchin Falls Cave (SceneId_FuchinFallsCaveEntrance to
///   SceneId_FuchinFallsCaveDragonStatue), the waterfall grotto.
///
///   Master Nyunpa of Fuchin Temple tells the party they must survive its test
///   before he will tell them how to pass through Mogall Forest. The test ends
///   at the chest of the Orb of Force (FLAG_FUCHIN_FALLS_ORB_OF_FORCE_FOUND).
///   Back at the temple, Nyunpa names the power they have gained Ki
///   (fuchin_temple.c).
///
///   The way in and out is FUCHIN_FALLS_CAVE_ENTRANCE_TO_FUCHIN_TEMPLE_GROUNDS,
///   to FUCHIN_TEMPLE_GROUNDS_FROM_FUCHIN_FALLS_CAVE_ENTRANCE.
///
///   The code numbers the rooms by scene. The first to fourth rooms are
///   SceneId_FuchinFallsCaveEntrance to SceneId_FuchinFallsCaveArcticBlade.
///   Then come the room of the Ki inscription
///   (SceneId_FuchinFallsCaveOrbOfForce) and the dragon statue's room
///   (SceneId_FuchinFallsCaveDragonStatue).
///
/// The first four rooms are crossed on floating logs,
/// SPRITE_FLOATING_LOG_WEST_EAST and SPRITE_FLOATING_LOG_NORTH_SOUTH. They are
/// the logs of Colosso's Lumber Water Crossing Area (colosseum_azart_match.c).
///
/// Walking into a log's step trigger cell pushes it along the water to its next
/// stop, and the leader moves with it (Actor_PlacePairAtOffset). Touching the
/// cell plays the log's two rolling animations.
///
/// Each stop is an area temp flag, so the logs are back where they started once
/// the party leaves the cave's scenes:
///
/// - FLAG_FUCHIN_FALLS_ENTRANCE_FIRST_LOG_EAST to
///   FLAG_FUCHIN_FALLS_ENTRANCE_THIRD_LOG_NORTH for the first room's logs
/// - FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_EAST_END to
///   FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_THIRD_LOG_NORTH the second's
/// - FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_WEST_END to
///   FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_LOWER_STOP the third's
/// - FLAG_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_FIRST_LOG_SOUTH to
///   FLAG_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_THIRD_LOG_EAST the fourth's
///
/// After each move, and on entry, the room's rebuild function puts every log at
/// the cell its flags name. It rewrites the cells of each log's track to match.
///
/// In the first room (FuchinFallsCave_RebuildEntranceLogs) the track's cells
/// are first copied in from a copy below the room. That copy has each log's
/// step trigger cells (11 and 12, 13 and 14, 15 and 16) at both ends of its
/// run. Then the end each log's flag says it is not at is overwritten with a
/// wall cell of the water. Each cell of a track is then a wall, a way out or
/// the push that moves a log.
///
/// In the third room the logs block one another, so where a log stops depends
/// on where the others lie. In the second and third rooms the leader hops
/// across gaps a log's stop leaves.
///
/// Zephyr, a Jupiter Djinni, waits in the second room
/// (SceneId_FuchinFallsCaveZephyr) and fights when spoken to.
///
/// In the first room a chest holds a mimic, whose battle its find value starts
/// (FIND_MIMIC). The fourth room's chest holds an Arctic Blade, and the Ki
/// room's the Orb of Force.
///
/// Arriving in the third room by
/// FUCHIN_FALLS_CAVE_DRAGONS_EYE_CHAMBER_FROM_LOGS before the Dragon's Eye is
/// taken (FLAG_FUCHIN_FALLS_DRAGONS_EYE_FOUND) shows it, and searching there
/// takes it.
///
/// The dragon statue's room is dark until the eye is set. Entering its northern
/// half darkens the palette. That half is its first six entrances, up to
/// FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_FROM_SOUTH_HALF_EAST: from the Ki
/// room, from the fourth room or up from the southern half.
///
/// Coming in from the Ki room
/// (FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_ORB_OF_FORCE_EAST
/// and _WEST) or up by
/// FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_FROM_SOUTH_HALF_MIDDLE starts the
/// fade overlay (field/field_sequence/ground_mask.c). The overlay follows the
/// leader, except from
/// FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_FROM_SOUTH_HALF_MIDDLE. Step
/// triggers 15 and 16 start and stop its following. When Reveal ends, the room
/// goes dark again.
///
/// The statue is `an eyeless dragon`. Using the Dragon's Eye on it flashes the
/// room and blows dust from the cave mouth as the room brightens. It redraws
/// the cells of the hidden path and uses the eye up
/// (FLAG_FUCHIN_FALLS_DRAGONS_EYE_SET).
///
/// The first room's inscription points the way: `The dragon's flame illuminates
/// the path of truth in darkness.`

#include "script_operands.h"
#include "main_routine_work.h"
#include "input.h"
#include "scene_ids.h"
#include "module/import.h"
#include "module/scene/imil_fuchin_falls.h"
#include "module/field/field_sequence.h"
#include "module/system/main_routine.h"
#include "module/field/event_commands.h"
#include "module/field/field.h"
#include "module/game/game.h"
#include "battle.h"
#include "event_inline.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "types.h"
#include "edition.h"
#include "far_call.h"
#include "sound.h"
#include "system.h"
#include "imil_fuchin_falls.h"
#include "glyph.h"
#include "event.h"
#include "scene_tables.h"
#include "djinn_ids.h"
#include "character_ids.h"
#include "battle_side.h"
#include "field_effect.h"
#include "scene.h"
#include "display_transition.h"
#include "field_psynergy_work.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "actor_script.h"
#include "fixed_math.h"
#include "ground_mask.h"
#include "palette_buffers.h"
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
  ACTOR_FEVER = ACTOR_FIRST_PLACED,
  /// The floating logs: three in the first, second and fourth rooms, four in
  /// the third.
  ACTOR_FIRST_LOG = ACTOR_FIRST_PLACED,
  ACTOR_SECOND_LOG,
  ACTOR_THIRD_LOG,
  ACTOR_FOURTH_LOG,
  /// Zephyr, a Jupiter Djinni, after the second room's three logs.
  ACTOR_ZEPHYR = ACTOR_FIRST_PLACED + 3,
  /// A crate (SPRITE_CRATE) in the dragon statue's room, which the player never
  /// sees. It is the sprite of the third room's two foreground pieces
  /// (FuchinFallsCave_CreateForegroundActor). On entry
  /// ImilFuchinFalls_PrepareRoom stills it at floor height and draws it at
  /// sprite priority 1 in SPRITE_COLOR_HIDDEN, as those are drawn, so every
  /// pixel is transparent.
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
  s16 sceneId = gMainRoutineWork.next;

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
  s16 sceneId = gMainRoutineWork.next;

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

#include "shared/actor_pair_move.h"

/// The first room's first log lies at the east end of its track.
#define FLAG_FUCHIN_FALLS_ENTRANCE_FIRST_LOG_EAST FLAG_AREA_TEMP_01
/// The first room's second log lies at the north end of its track.
#define FLAG_FUCHIN_FALLS_ENTRANCE_SECOND_LOG_NORTH FLAG_AREA_TEMP_02
/// The first room's third log lies at the north end of its track.
#define FLAG_FUCHIN_FALLS_ENTRANCE_THIRD_LOG_NORTH FLAG_AREA_TEMP_03
/// The second room's first log lies at the east end of its track.
#define FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_EAST_END FLAG_AREA_TEMP_04
/// The second room's first log lies at the west end of its track, a cell west
/// of where it starts, unless it has since been pushed to the east end.
#define FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_WEST_END FLAG_AREA_TEMP_05
/// The second room's second log lies at the south end of its track.
#define FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_SECOND_LOG_SOUTH FLAG_AREA_TEMP_06
/// The second room's third log lies at the north end of its track.
#define FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_THIRD_LOG_NORTH FLAG_AREA_TEMP_07
/// The third room's first log lies at the west end of its track.
#define FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_WEST_END FLAG_AREA_TEMP_08
/// The third room's first log lies at its middle stop, where the third or
/// fourth log blocked it.
#define FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_MIDDLE FLAG_AREA_TEMP_09
/// The third room's second log lies at the west end of its track.
#define FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_SECOND_LOG_WEST FLAG_AREA_TEMP_0A
/// The third room's third log lies at the north end of its track.
#define FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END FLAG_AREA_TEMP_0B
/// The third room's third log lies at its upper stop, a cell south of the north
/// end.
#define FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_UPPER_STOP                \
  FLAG_AREA_TEMP_0C
/// The third room's third log lies at its middle stop.
#define FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE FLAG_AREA_TEMP_0D
/// The third room's third log lies at its lower stop. No move sets it.
#define FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_LOWER_STOP                \
  FLAG_AREA_TEMP_0E
/// The third room's fourth log lies at the north end of its track.
#define FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_NORTH_END                \
  FLAG_AREA_TEMP_0F
/// The third room's fourth log lies at its upper stop, a cell south of the
/// north end.
#define FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_UPPER_STOP               \
  FLAG_AREA_TEMP_10
/// The third room's fourth log lies at its middle stop.
#define FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_MIDDLE FLAG_AREA_TEMP_11
/// The third room's fourth log lies at its lower stop. No move sets it.
#define FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_LOWER_STOP               \
  FLAG_AREA_TEMP_12
/// The fourth room's first log lies at the south end of its track.
#define FLAG_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_FIRST_LOG_SOUTH FLAG_AREA_TEMP_13
/// The fourth room's second log lies at the west end of its track.
#define FLAG_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_SECOND_LOG_WEST FLAG_AREA_TEMP_14
/// The fourth room's third log lies at the east end of its track.
#define FLAG_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_THIRD_LOG_EAST FLAG_AREA_TEMP_15

/// Lets the log settle before rebuilding its room's cell behaviors.
static inline void FuchinFallsCave_FinishLogMove(void (*rebuildLogs)(void)) {
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  wait(2);
  rebuildLogs();
}

/// In Fuchin Falls Cave's first room, pushing the first log east (step
/// trigger 11) floats it 224 pixels east and sets
/// FLAG_FUCHIN_FALLS_ENTRANCE_FIRST_LOG_EAST.
static void FuchinFallsCave_Entrance_MoveFirstLogEast(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_FIRST_LOG, 112, 0);
  Actor_PlacePairAtOffset(ACTOR_FIRST_LOG, 112, 0);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Set(FLAG_FUCHIN_FALLS_ENTRANCE_FIRST_LOG_EAST);
  wait(2);
  FuchinFallsCave_RebuildEntranceLogs();
}

/// In Fuchin Falls Cave's first room, pushing the first log west (step
/// trigger 12) floats it 224 pixels back and clears
/// FLAG_FUCHIN_FALLS_ENTRANCE_FIRST_LOG_EAST.
static void FuchinFallsCave_Entrance_MoveFirstLogWest(void) {
  s32 offset = 112;

  offset = -offset;
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_FIRST_LOG, offset, 0);
  Actor_PlacePairAtOffset(ACTOR_FIRST_LOG, offset, 0);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_ENTRANCE_FIRST_LOG_EAST);
  wait(2);
  FuchinFallsCave_RebuildEntranceLogs();
}

/// In Fuchin Falls Cave's first room, pushing the second log south (step
/// trigger 13) floats it 64 pixels south and clears
/// FLAG_FUCHIN_FALLS_ENTRANCE_SECOND_LOG_NORTH.
static void FuchinFallsCave_Entrance_MoveSecondLogSouth(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_SECOND_LOG, 0, 64);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_ENTRANCE_SECOND_LOG_NORTH);
  wait(2);
  FuchinFallsCave_RebuildEntranceLogs();
}

/// In Fuchin Falls Cave's first room, pushing the second log north (step
/// trigger 14) floats it 64 pixels north and sets
/// FLAG_FUCHIN_FALLS_ENTRANCE_SECOND_LOG_NORTH.
static void FuchinFallsCave_Entrance_MoveSecondLogNorth(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_SECOND_LOG, 0, -64);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Set(FLAG_FUCHIN_FALLS_ENTRANCE_SECOND_LOG_NORTH);
  wait(2);
  FuchinFallsCave_RebuildEntranceLogs();
}

/// In Fuchin Falls Cave's first room, pushing the third log south (step
/// trigger 15) floats it 64 pixels south and clears
/// FLAG_FUCHIN_FALLS_ENTRANCE_THIRD_LOG_NORTH.
static void FuchinFallsCave_Entrance_MoveThirdLogSouth(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, 64);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_ENTRANCE_THIRD_LOG_NORTH);
  wait(2);
  FuchinFallsCave_RebuildEntranceLogs();
}

/// In Fuchin Falls Cave's first room, pushing the third log north (step
/// trigger 16) floats it 64 pixels north and sets
/// FLAG_FUCHIN_FALLS_ENTRANCE_THIRD_LOG_NORTH.
static void FuchinFallsCave_Entrance_MoveThirdLogNorth(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, -64);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Set(FLAG_FUCHIN_FALLS_ENTRANCE_THIRD_LOG_NORTH);
  wait(2);
  FuchinFallsCave_RebuildEntranceLogs();
}

/// In Fuchin Falls Cave's second room, pushing the first log east (step
/// trigger 11) floats it 144 pixels east and sets
/// FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_EAST_END. It moves only while
/// FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_SECOND_LOG_SOUTH is clear.
static void FuchinFallsCave_ZephyrRoom_MoveFirstLogToEastEnd(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_FIRST_LOG, 144, 0);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Set(FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_EAST_END);
  wait(2);
  FuchinFallsCave_RebuildZephyrRoomLogs();
}

/// In Fuchin Falls Cave's second room, pushing the first log west (step
/// trigger 12) floats it 144 pixels west. It clears
/// FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_EAST_END and sets
/// FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_WEST_END.
static void FuchinFallsCave_ZephyrRoom_MoveFirstLogToWestEnd(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_FIRST_LOG, -144, 0);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_EAST_END);
  GameFlag_Set(FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_WEST_END);
  wait(2);
  FuchinFallsCave_RebuildZephyrRoomLogs();
}

/// In Fuchin Falls Cave's second room, pushing the first log west (step
/// trigger 11) floats it 14 pixels west and sets
/// FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_WEST_END.
static void FuchinFallsCave_ZephyrRoom_NudgeFirstLogWest(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_FIRST_LOG, -14, 0);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Set(FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_WEST_END);
  wait(2);
  FuchinFallsCave_RebuildZephyrRoomLogs();
}

/// In Fuchin Falls Cave's second room, pushing the first log east (step
/// trigger 17) moves it one cell or 144 pixels.
///
/// When the second log lies south
/// (FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_SECOND_LOG_SOUTH) it moves one cell and
/// clears FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_WEST_END. Otherwise it moves
/// 144 pixels and sets FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_EAST_END.
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

/// In Fuchin Falls Cave's second room, pushing the second log south (step
/// trigger 13) floats it 64 pixels south and sets
/// FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_SECOND_LOG_SOUTH.
static void FuchinFallsCave_ZephyrRoom_MoveSecondLogSouth(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_SECOND_LOG, 0, 64);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Set(FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_SECOND_LOG_SOUTH);
  wait(2);
  FuchinFallsCave_RebuildZephyrRoomLogs();
}

/// In Fuchin Falls Cave's second room, pushing the second log north (step
/// trigger 14) floats it 64 pixels north and clears
/// FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_SECOND_LOG_SOUTH.
static void FuchinFallsCave_ZephyrRoom_MoveSecondLogNorth(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_SECOND_LOG, 0, -64);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_SECOND_LOG_SOUTH);
  wait(2);
  FuchinFallsCave_RebuildZephyrRoomLogs();
}

/// In Fuchin Falls Cave's second room, pushing the third log south (step
/// trigger 15) floats it 272 pixels south and clears
/// FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_THIRD_LOG_NORTH.
static void FuchinFallsCave_ZephyrRoom_MoveThirdLogSouth(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, 144);
  Actor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, 128);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_THIRD_LOG_NORTH);
  wait(2);
  FuchinFallsCave_RebuildZephyrRoomLogs();
}

/// In Fuchin Falls Cave's second room, pushing the third log north (step
/// trigger 16) floats it 272 pixels north and sets
/// FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_THIRD_LOG_NORTH.
static void FuchinFallsCave_ZephyrRoom_MoveThirdLogNorth(void) {
  s32 depth = -96;

  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, depth);
  Actor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, depth);
  Actor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, -80);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Set(FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_THIRD_LOG_NORTH);
  wait(2);
  FuchinFallsCave_RebuildZephyrRoomLogs();
}

/// In Fuchin Falls Cave's third room, pushing the first log west (step
/// trigger 10) moves it 48 pixels or 96.
///
/// It goes 48 pixels and sets
/// FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_MIDDLE when the third log lies
/// at FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE or the fourth at
/// FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_UPPER_STOP. Otherwise it goes
/// 96 and sets FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_WEST_END.
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

/// In Fuchin Falls Cave's third room, pushing the first log east (step
/// trigger 11) floats it 48 pixels east. It clears
/// FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_WEST_END and
/// FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_MIDDLE.
static void FuchinFallsCave_DragonsEyeRoom_MoveFirstLogEastFromMiddle(void) {
  Sound_Play(SE_WATER_MOVE);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_WEST_END);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_MIDDLE);
  Actor_PlacePairAtOffset(ACTOR_FIRST_LOG, 48, 0);
  FuchinFallsCave_FinishLogMove(FuchinFallsCave_RebuildDragonsEyeRoomLogs);
}

/// In Fuchin Falls Cave's third room, pushing the first log west (step
/// trigger 11) floats it 48 pixels west and sets
/// FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_WEST_END.
///
/// It does not move when the third log lies at
/// FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE or the fourth at
/// FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_UPPER_STOP.
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

/// In Fuchin Falls Cave's third room, pushing the first log east (step
/// trigger 12) floats it 96 pixels east. It clears
/// FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_WEST_END and
/// FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_MIDDLE.
static void FuchinFallsCave_DragonsEyeRoom_MoveFirstLogEastFromWestEnd(void) {
  Sound_Play(SE_WATER_MOVE);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_WEST_END);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_MIDDLE);
  Actor_PlacePairAtOffset(ACTOR_FIRST_LOG, 96, 0);
  FuchinFallsCave_FinishLogMove(FuchinFallsCave_RebuildDragonsEyeRoomLogs);
}

/// In Fuchin Falls Cave's third room, pushing the second log west (step
/// trigger 14) floats it 32 pixels west and sets
/// FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_SECOND_LOG_WEST.
static void FuchinFallsCave_DragonsEyeRoom_MoveSecondLogWest(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_SECOND_LOG, -32, 0);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Set(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_SECOND_LOG_WEST);
  wait(2);
  FuchinFallsCave_RebuildDragonsEyeRoomLogs();
}

/// In Fuchin Falls Cave's third room, pushing the second log east (step
/// trigger 13) floats it 32 pixels east and clears
/// FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_SECOND_LOG_WEST.
static void FuchinFallsCave_DragonsEyeRoom_MoveSecondLogEast(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_SECOND_LOG, 32, 0);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_SECOND_LOG_WEST);
  wait(2);
  FuchinFallsCave_RebuildDragonsEyeRoomLogs();
}

/// In Fuchin Falls Cave's third room, pushing the third log north (step
/// trigger 15) moves it 64 pixels or 128.
///
/// It goes 64 pixels, to
/// FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_UPPER_STOP, when the first log
/// lies at FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_WEST_END. Otherwise it
/// goes 128, to FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END.
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

/// In Fuchin Falls Cave's third room, pushing the third log south (step
/// trigger 16) moves it one cell, 64 pixels or 128.
///
/// - One cell, to FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_UPPER_STOP, when
///   the first log lies at
///   FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_WEST_END or the fourth at
///   FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_UPPER_STOP.
/// - 64 pixels, to FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE, when
///   the fourth lies at FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_MIDDLE.
/// - Otherwise 128, and its stops are cleared.
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

/// In Fuchin Falls Cave's third room, pushing the third log north (step
/// trigger 17) moves it one cell, to
/// FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END.
static void FuchinFallsCave_DragonsEyeRoom_NudgeThirdLogNorth(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, -16);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Set(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_UPPER_STOP);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_LOWER_STOP);
  wait(2);
  FuchinFallsCave_RebuildDragonsEyeRoomLogs();
}

/// In Fuchin Falls Cave's third room, pushing the third log south (step
/// trigger 17) moves it 48 pixels, 32 or 112.
///
/// It does not move when the first log lies at
/// FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_WEST_END. Otherwise:
///
/// - 48 pixels, to FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE, when
///   the fourth lies at FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_MIDDLE.
/// - 32, to FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_UPPER_STOP, when it
///   lies at FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_UPPER_STOP.
/// - Else 112, and its stops are cleared.
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

/// In Fuchin Falls Cave's third room, pushing the third log north (step
/// trigger 18) floats it 64 pixels north, to
/// FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END.
static void FuchinFallsCave_DragonsEyeRoom_MoveThirdLogNorthFromMiddle(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, -64);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Set(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE);
  wait(2);
  FuchinFallsCave_RebuildDragonsEyeRoomLogs();
}

/// In Fuchin Falls Cave's third room, pushing the third log south (step
/// trigger 18) floats it 64 pixels south and clears its stops. It does not move
/// when the fourth lies at
/// FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_UPPER_STOP.
static void FuchinFallsCave_DragonsEyeRoom_MoveThirdLogSouthFromMiddle(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, 64);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_UPPER_STOP);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_LOWER_STOP);
  wait(2);
  FuchinFallsCave_RebuildDragonsEyeRoomLogs();
}

/// In Fuchin Falls Cave's third room, pushing the third log north (step
/// trigger 19) floats it 80 pixels north, to
/// FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END.
static void
FuchinFallsCave_DragonsEyeRoom_MoveThirdLogNorthFromLowerStop(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, -80);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Set(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_UPPER_STOP);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_LOWER_STOP);
  wait(2);
  FuchinFallsCave_RebuildDragonsEyeRoomLogs();
}

/// In Fuchin Falls Cave's third room, pushing the third log south (step
/// trigger 19) floats it 64 pixels south and clears its stops.
static void
FuchinFallsCave_DragonsEyeRoom_MoveThirdLogSouthFromLowerStop(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_THIRD_LOG, 0, 64);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_UPPER_STOP);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_LOWER_STOP);
  wait(2);
  FuchinFallsCave_RebuildDragonsEyeRoomLogs();
}

/// In Fuchin Falls Cave's third room, pushing the fourth log north (step
/// trigger 20) moves it 64 pixels, 112 or 128.
///
/// - 64 pixels, to FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_MIDDLE, when
///   the first log lies at
///   FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_WEST_END or the third at
///   FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE.
/// - 112, to FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_UPPER_STOP, when the
///   third lies at FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_UPPER_STOP.
/// - Else 128, to FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_NORTH_END.
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

/// In Fuchin Falls Cave's third room, pushing the fourth log south (step
/// trigger 21) floats it 128 pixels south and clears its stops. It does not
/// move when the first log lies at
/// FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_WEST_END.
static void
FuchinFallsCave_DragonsEyeRoom_MoveFourthLogSouthFromNorthEnd(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_FOURTH_LOG, 0, 128);
  FuchinFallsCave_ClearFourthLogStops();
  FuchinFallsCave_FinishLogMove(FuchinFallsCave_RebuildDragonsEyeRoomLogs);
}

/// In Fuchin Falls Cave's third room, pushing the fourth log north (step
/// trigger 22) moves it one cell, to
/// FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_NORTH_END.
static void FuchinFallsCave_DragonsEyeRoom_NudgeFourthLogNorth(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_FOURTH_LOG, 0, -16);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Set(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_NORTH_END);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_UPPER_STOP);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_MIDDLE);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_LOWER_STOP);
  wait(2);
  FuchinFallsCave_RebuildDragonsEyeRoomLogs();
}

/// In Fuchin Falls Cave's third room, pushing the fourth log south (step
/// trigger 22) floats it 112 pixels south and clears its stops.
static void
FuchinFallsCave_DragonsEyeRoom_MoveFourthLogSouthFromUpperStop(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_FOURTH_LOG, 0, 112);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  FuchinFallsCave_ClearFourthLogStops();
  wait(2);
  FuchinFallsCave_RebuildDragonsEyeRoomLogs();
}

/// In Fuchin Falls Cave's third room, pushing the fourth log south (step
/// trigger 23) floats it 64 pixels south and clears its stops.
static void FuchinFallsCave_DragonsEyeRoom_MoveFourthLogSouthFromMiddle(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_FOURTH_LOG, 0, 64);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  FuchinFallsCave_ClearFourthLogStops();
  wait(2);
  FuchinFallsCave_RebuildDragonsEyeRoomLogs();
}

// XXX (no evidence): whether walking north was meant to float the log north,
// as the third log's matching step trigger 19 does; the room never has a
// step trigger 24, so the game never shows which.
/// Unused: walking north into step trigger 24 in Fuchin Falls Cave's third room
/// would float the fourth log 80 pixels south (its stops clear).
///
/// Step trigger 24's cells reach the room only from their copy at columns 10 to
/// 12, row 43. FuchinFallsCave_RebuildDragonsEyeRoomLogs makes it with the log
/// at FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_LOWER_STOP, and no move
/// sets that flag. So the room never has a step trigger 24.
///
/// In tbs-en it appears there only with the flag set by hand. Walking north on
/// the log then carries the leader 80 pixels south, past the log's south end,
/// where the rebuild puts the log back.
static void
FuchinFallsCave_DragonsEyeRoom_UnusedMoveFourthLogSouthFromLowerStop(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_FOURTH_LOG, 0, 80);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  FuchinFallsCave_ClearFourthLogStops();
  wait(2);
  FuchinFallsCave_RebuildDragonsEyeRoomLogs();
}

/// Unused: pushing the fourth log south from step trigger 24 in Fuchin Falls
/// Cave's third room would float it 48 pixels south (its stops clear), back to
/// its south end. The room never has a step trigger 24, as
/// FuchinFallsCave_DragonsEyeRoom_UnusedMoveFourthLogSouthFromLowerStop says.
static void
FuchinFallsCave_DragonsEyeRoom_UnusedMoveFourthLogSouthFromLastStop(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_FOURTH_LOG, 0, 48);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  FuchinFallsCave_ClearFourthLogStops();
  wait(2);
  FuchinFallsCave_RebuildDragonsEyeRoomLogs();
}

/// In Fuchin Falls Cave's fourth room, pushing the first log south (step
/// trigger 11) floats it 112 pixels south and sets
/// FLAG_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_FIRST_LOG_SOUTH.
static void FuchinFallsCave_ArcticBladeRoom_MoveFirstLogSouth(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_FIRST_LOG, 0, 112);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Set(FLAG_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_FIRST_LOG_SOUTH);
  wait(2);
  FuchinFallsCave_RebuildArcticBladeRoomLogs();
}

/// In Fuchin Falls Cave's fourth room, pushing the first log north (step
/// trigger 12) floats it 112 pixels north and clears
/// FLAG_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_FIRST_LOG_SOUTH.
static void FuchinFallsCave_ArcticBladeRoom_MoveFirstLogNorth(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_FIRST_LOG, 0, -112);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_FIRST_LOG_SOUTH);
  wait(2);
  FuchinFallsCave_RebuildArcticBladeRoomLogs();
}

/// In Fuchin Falls Cave's fourth room, pushing the second log west (step
/// trigger 13) floats it 128 pixels west and sets
/// FLAG_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_SECOND_LOG_WEST. It moves only while the
/// first log lies north (FLAG_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_FIRST_LOG_SOUTH
/// clear).
static void FuchinFallsCave_ArcticBladeRoom_MoveSecondLogWest(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_SECOND_LOG, -128, 0);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Set(FLAG_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_SECOND_LOG_WEST);
  wait(2);
  FuchinFallsCave_RebuildArcticBladeRoomLogs();
}

/// In Fuchin Falls Cave's fourth room, pushing the second log east (step
/// trigger 14) floats it 128 pixels east and clears
/// FLAG_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_SECOND_LOG_WEST.
static void FuchinFallsCave_ArcticBladeRoom_MoveSecondLogEast(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_SECOND_LOG, 128, 0);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_SECOND_LOG_WEST);
  wait(2);
  FuchinFallsCave_RebuildArcticBladeRoomLogs();
}

/// In Fuchin Falls Cave's fourth room, pushing the third log east (step
/// trigger 15) floats it 160 pixels east and sets
/// FLAG_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_THIRD_LOG_EAST.
static void FuchinFallsCave_ArcticBladeRoom_MoveThirdLogEast(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_THIRD_LOG, 160, 0);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Set(FLAG_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_THIRD_LOG_EAST);
  wait(2);
  FuchinFallsCave_RebuildArcticBladeRoomLogs();
}

/// In Fuchin Falls Cave's fourth room, pushing the third log west (step
/// trigger 16) floats it 160 pixels west and clears
/// FLAG_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_THIRD_LOG_EAST.
static void FuchinFallsCave_ArcticBladeRoom_MoveThirdLogWest(void) {
  Sound_Play(SE_WATER_MOVE);
  Actor_PlacePairAtOffset(ACTOR_THIRD_LOG, -160, 0);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  GameFlag_Clear(FLAG_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_THIRD_LOG_EAST);
  wait(2);
  FuchinFallsCave_RebuildArcticBladeRoomLogs();
}

/// Touching the first log's step trigger cell in Fuchin Falls Cave plays its
/// two rolling animations.
static void FuchinFallsCave_AnimateFirstLog(void) {
  Actor_SetAnimation(ACTOR_FIRST_LOG, 1);
  Actor_SetAnimation(ACTOR_FIRST_LOG, 2);
}

/// Touching the second log's step trigger cell in Fuchin Falls Cave plays its
/// two rolling animations.
static void FuchinFallsCave_AnimateSecondLog(void) {
  Actor_SetAnimation(ACTOR_SECOND_LOG, 1);
  Actor_SetAnimation(ACTOR_SECOND_LOG, 2);
}

/// Touching the third log's step trigger cell in Fuchin Falls Cave plays its
/// two rolling animations.
static void FuchinFallsCave_AnimateThirdLog(void) {
  Actor_SetAnimation(ACTOR_THIRD_LOG, 1);
  Actor_SetAnimation(ACTOR_THIRD_LOG, 2);
}

/// Touching the fourth log's step trigger cell in Fuchin Falls Cave (third
/// room) plays its two rolling animations.
static void FuchinFallsCave_AnimateFourthLog(void) {
  Actor_SetAnimation(ACTOR_FOURTH_LOG, 1);
  Actor_SetAnimation(ACTOR_FOURTH_LOG, 2);
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
  s16 sceneId = gMainRoutineWork.next;

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

/// Walking into the way out of Imil Falls Cave (step trigger 1) faces the
/// leader south and leaves for Imil.
static void Imil_LeaveFallsCave(void) {
  struct Actor *leader;

  Event_Start();
  leader = Actor_Get(ACTOR_PARTY_LEADER);
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
  struct Actor *actor = Actor_Get(id);

  if (actor != NULL) {
    Actor_SetSpritePriority(id, 3);
    actor->layer = 2;
    actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    actor->x.fixed = px(column * 16) + px(8);
    actor->z.fixed = px(row * 16) + px(8);
  }
}

/// Puts the logs of Fuchin Falls Cave's first room
/// (SceneId_FuchinFallsCaveEntrance) at the cells their flags name, and
/// rewrites the cells of their tracks to match.
static void FuchinFallsCave_RebuildEntranceLogs(void) {
  MapGrid_CopyCellAttributes(8, 42, 15, 5, 8, 29);

  if (GameFlag_Get(FLAG_FUCHIN_FALLS_ENTRANCE_FIRST_LOG_EAST) != 0) {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_FIRST_LOG, 22, 31);
    MapGrid_CopyCellAttributes(9, 30, 1, 3, 8, 30);
  } else {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_FIRST_LOG, 8, 31);
    MapGrid_CopyCellAttributes(9, 30, 1, 3, 22, 30);
  }

  if (GameFlag_Get(FLAG_FUCHIN_FALLS_ENTRANCE_SECOND_LOG_NORTH) != 0) {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_SECOND_LOG, 12, 29);
    MapGrid_CopyCellAttributes(14, 33, 3, 1, 11, 33);
  } else {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_SECOND_LOG, 12, 33);
    MapGrid_CopyCellAttributes(14, 29, 3, 1, 11, 29);
  }

  if (GameFlag_Get(FLAG_FUCHIN_FALLS_ENTRANCE_THIRD_LOG_NORTH) != 0) {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_THIRD_LOG, 18, 29);
    MapGrid_CopyCellAttributes(14, 33, 3, 1, 17, 33);
  } else {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_THIRD_LOG, 18, 33);
    MapGrid_CopyCellAttributes(14, 29, 3, 1, 17, 29);
  }
}

/// Puts the logs of Fuchin Falls Cave's second room where their flags say. The
/// flags name cells in SceneId_FuchinFallsCaveZephyr, and the cells of the
/// logs' tracks are rewritten to match.
///
/// As in the third room, a stop without its log takes three walls in a row.
/// They run from (20, 19) for a log lying north to south, from (14, 18) for one
/// lying west to east.
static void FuchinFallsCave_RebuildZephyrRoomLogs(void) {
  MapGrid_CopyCellAttributes(0, 28, 10, 18, 12, 8);

  if (GameFlag_Get(FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_EAST_END) != 0) {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_FIRST_LOG, 21, 20);
    MapGrid_CopyCellAttributes(20, 19, 1, 3, 13, 19);
  } else {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_FIRST_LOG, 13, 20);
    MapGrid_CopyCellAttributes(20, 19, 1, 3, 21, 19);
  }

  if (GameFlag_Get(FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_WEST_END) != 0) {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_FIRST_LOG, 12, 20);
    MapGrid_CopyCellAttributes(5, 19, 1, 3, 12, 19);
    MapGrid_CopyCellAttributes(20, 19, 1, 3, 13, 19);
    if (GameFlag_Get(FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_FIRST_LOG_EAST_END) != 0) {
      FuchinFallsCave_PlaceLogAtCell(ACTOR_FIRST_LOG, 21, 20);
      MapGrid_CopyCellAttributes(20, 19, 1, 3, 13, 19);
      MapGrid_CopyCellAttributes(20, 19, 1, 3, 12, 19);
    }
  }

  if (GameFlag_Get(FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_SECOND_LOG_SOUTH) != 0) {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_SECOND_LOG, 15, 21);
    MapGrid_CopyCellAttributes(14, 18, 3, 1, 14, 17);
  } else {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_SECOND_LOG, 15, 17);
    MapGrid_CopyCellAttributes(14, 18, 3, 1, 14, 21);
  }

  if (GameFlag_Get(FLAG_FUCHIN_FALLS_ZEPHYR_ROOM_THIRD_LOG_NORTH) != 0) {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_THIRD_LOG, 19, 8);
    MapGrid_CopyCellAttributes(14, 18, 3, 1, 18, 25);
  } else {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_THIRD_LOG, 19, 25);
    MapGrid_CopyCellAttributes(14, 18, 3, 1, 18, 8);
  }
}

/// Puts the four logs of Fuchin Falls Cave's third room where their flags say.
/// The flags name cells in SceneId_FuchinFallsCaveDragonsEye, and the cells of
/// the logs' tracks are rewritten to match.
///
/// The rewriting uses two kinds of cell kept in spare corners of the map. Where
/// a log lies, its three cells take step triggers (rows 41 to 44), which roll
/// it when stepped on. A stop it is not at takes three walls in a row, from
/// (14, 29) for a log lying west to east.
static void FuchinFallsCave_RebuildDragonsEyeRoomLogs(void) {
  MapGrid_CopyCellAttributes(12, 3, 9, 16, 12, 21);

  if (GameFlag_Get(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_WEST_END) !=
      0) {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_FIRST_LOG, 14, 25);
    MapGrid_CopyCellAttributes(16, 24, 1, 3, 20, 24);
  } else if (GameFlag_Get(
                 FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_MIDDLE) != 0) {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_FIRST_LOG, 17, 25);
    MapGrid_CopyCellAttributes(18, 24, 1, 3, 20, 24);
    MapGrid_CopyCellAttributes(18, 24, 1, 3, 14, 24);
    MapGrid_CopyCellAttributes(8, 41, 1, 3, 17, 24);
  } else {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_FIRST_LOG, 20, 25);
    MapGrid_CopyCellAttributes(16, 24, 1, 3, 14, 24);
  }

  if (GameFlag_Get(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_SECOND_LOG_WEST) != 0) {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_SECOND_LOG, 13, 35);
    MapGrid_CopyCellAttributes(14, 34, 1, 3, 15, 34);
  } else {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_SECOND_LOG, 15, 35);
    MapGrid_CopyCellAttributes(14, 34, 1, 3, 13, 34);
  }

  if (GameFlag_Get(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_NORTH_END) !=
      0) {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_THIRD_LOG, 15, 22);
    MapGrid_CopyCellAttributes(14, 29, 3, 1, 14, 30);
    MapGrid_CopyCellAttributes(5, 41, 3, 1, 14, 22);
  } else if (GameFlag_Get(
                 FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_UPPER_STOP) !=
             0) {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_THIRD_LOG, 15, 23);
    MapGrid_CopyCellAttributes(5, 42, 3, 1, 14, 23);
    MapGrid_CopyCellAttributes(14, 29, 3, 1, 14, 30);
    MapGrid_CopyCellAttributes(10, 44, 3, 1, 14, 21);
  } else if (GameFlag_Get(
                 FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_MIDDLE) != 0) {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_THIRD_LOG, 15, 26);
    MapGrid_CopyCellAttributes(14, 29, 3, 1, 14, 22);
    MapGrid_CopyCellAttributes(5, 43, 3, 1, 14, 26);
    MapGrid_CopyCellAttributes(14, 29, 3, 1, 14, 30);
  } else if (GameFlag_Get(
                 FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_THIRD_LOG_LOWER_STOP) !=
             0) {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_THIRD_LOG, 15, 27);
    MapGrid_CopyCellAttributes(14, 29, 3, 1, 14, 22);
    MapGrid_CopyCellAttributes(14, 29, 3, 1, 14, 30);
    MapGrid_CopyCellAttributes(5, 44, 3, 1, 14, 27);
  } else {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_THIRD_LOG, 15, 30);
  }

  if (GameFlag_Get(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_NORTH_END) !=
      0) {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_FOURTH_LOG, 15, 23);
    MapGrid_CopyCellAttributes(14, 29, 3, 1, 14, 31);
    MapGrid_CopyCellAttributes(10, 40, 3, 1, 14, 23);
  } else if (GameFlag_Get(
                 FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_UPPER_STOP) !=
             0) {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_FOURTH_LOG, 15, 24);
    MapGrid_CopyCellAttributes(14, 29, 3, 1, 14, 31);
    MapGrid_CopyCellAttributes(10, 41, 3, 1, 14, 24);
  } else if (GameFlag_Get(
                 FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_MIDDLE) != 0) {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_FOURTH_LOG, 15, 27);
    MapGrid_CopyCellAttributes(14, 29, 3, 1, 14, 31);
    MapGrid_CopyCellAttributes(10, 42, 3, 1, 14, 27);
  } else if (GameFlag_Get(
                 FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_LOWER_STOP) !=
             0) {
    // Unused: no move sets
    // FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FOURTH_LOG_LOWER_STOP, so the log
    // never lies here and the room never gets step trigger 24.
    FuchinFallsCave_PlaceLogAtCell(ACTOR_FOURTH_LOG, 15, 28);
    MapGrid_CopyCellAttributes(14, 29, 3, 1, 14, 31);
    MapGrid_CopyCellAttributes(10, 43, 3, 1, 14, 28);
  } else {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_FOURTH_LOG, 15, 31);
  }
}

/// Puts the logs of Fuchin Falls Cave's fourth room
/// (SceneId_FuchinFallsCaveArcticBlade), in both of its halves, at the cells
/// their flags name.
static void FuchinFallsCave_RebuildArcticBladeRoomLogs(void) {
  if (((u32)((u16)gMainRoutineWork.entrance - 1) << 16) <= 1 << 16) {
    MapGrid_CopyCellAttributes(22, 20, 9, 8, 14, 10);
  } else {
    MapGrid_CopyCellAttributes(20, 45, 11, 4, 7, 45);
  }

  if (GameFlag_Get(FLAG_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_FIRST_LOG_SOUTH) != 0) {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_FIRST_LOG, 20, 17);
    MapGrid_CopyCellAttributes(19, 11, 3, 1, 19, 10);
  } else {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_FIRST_LOG, 20, 10);
    MapGrid_CopyCellAttributes(19, 11, 3, 1, 19, 17);
  }

  if (GameFlag_Get(FLAG_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_SECOND_LOG_WEST) != 0) {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_SECOND_LOG, 14, 16);
    MapGrid_CopyCellAttributes(16, 15, 1, 3, 22, 15);
  } else {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_SECOND_LOG, 22, 16);
    MapGrid_CopyCellAttributes(16, 15, 1, 3, 14, 15);
  }

  if (GameFlag_Get(FLAG_FUCHIN_FALLS_ARCTIC_BLADE_ROOM_THIRD_LOG_EAST) != 0) {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_THIRD_LOG, 17, 46);
    MapGrid_CopyCellAttributes(15, 15, 1, 3, 7, 45);
  } else {
    FuchinFallsCave_PlaceLogAtCell(ACTOR_THIRD_LOG, 7, 46);
    MapGrid_CopyCellAttributes(15, 15, 1, 3, 17, 45);
  }
}

/// The fade overlay follows the leader through the dragon statue room. Touching
/// step trigger 15 in Fuchin Falls Cave's dragon statue room makes it follow,
/// once it has been started there.
static void FuchinFallsCave_FollowLeaderWithLight(void) {
  if (gImilFuchinFalls_TrackLeader != 0) {
    struct FadeOverlayWork *work = *(gHeapSlots + 36);

    work->actor = Actor_Get(ACTOR_PARTY_LEADER);
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
/// Cave blocks Retreat and walks the leader toward Fever. In Fuchin Falls Cave,
/// each room restores its logs, the Dragon's Eye and its lighting.
s32 ImilFuchinFalls_Initialize(void) {
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0 &&
      gMainRoutineWork.next == (s32)&SceneId_ImilFallsCave) {
    GameFlag_Set(FLAG_RETREAT_BLOCKED);
    Imil_WalkTowardFever();
  } else {
    ImilFuchinFalls_PrepareRoom();
  }
  return 0;
}

/// Shows Fever, in Imil Falls Cave, and walks the leader toward it.
static void Imil_WalkTowardFever(void) {

  Event_Start();
  Actor_SetSpriteFlags(Actor_Get(ACTOR_FEVER), 0);
  Event_StartScreenTransition();
  Actor_SetSpeedAndAcceleration(ACTOR_PARTY_LEADER, 2, 0.1);
  Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 264, 196);
  Event_End();
}

/// Sets up a room of Fuchin Falls Cave for the entrance it is entered by. That
/// covers its logs, the Dragon's Eye and two foreground sprites in the third
/// room, and the darkness and the fade overlay in the dragon statue's room.
static void ImilFuchinFalls_PrepareRoom(void) {
  struct Actor *actor;
  struct FadeOverlayWork *work;

  ImilFuchinFalls_RebuildLogRoom();
  if (gMainRoutineWork.next == (s32)&SceneId_FuchinFallsCaveDragonsEye) {
    if (!GameFlag_Get(FLAG_FUCHIN_FALLS_DRAGONS_EYE_FOUND) &&
        gMainRoutineWork.entrance ==
            FUCHIN_FALLS_CAVE_DRAGONS_EYE_CHAMBER_FROM_LOGS) {
      ImilFuchinFalls_PlaceDragonsEye();
    }
    if ((u16)(gMainRoutineWork.entrance - 2) <= 3) {
      FuchinFallsCave_CreateForegroundActor(px(156), 0, px(452), SPRITE_CRATE);
      FuchinFallsCave_CreateForegroundActor(px(188), 0, px(452), SPRITE_CRATE);
    }
  } else if (gMainRoutineWork.next ==
             (s32)&SceneId_FuchinFallsCaveDragonStatue) {
    // Default: NULL unless the arrival from the south half's middle takes the
    // fade overlay.
    work = NULL;
    actor = Actor_Get(ACTOR_INVISIBLE_DRAGON_ROOM_CRATE);
    gImilFuchinFalls_TrackLeader = 0;
    actor->motionFlags = 0;
    actor->y.fixed = 0;
    Actor_SetSpritePriority(ACTOR_INVISIBLE_DRAGON_ROOM_CRATE, 1);
    Actor_SetSpriteColorById(ACTOR_INVISIBLE_DRAGON_ROOM_CRATE,
                             SPRITE_COLOR_HIDDEN);
    switch (gMainRoutineWork.entrance) {
    case FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_ORB_OF_FORCE_EAST:
    case FUCHIN_FALLS_CAVE_DRAGON_STATUE_FROM_FUCHIN_FALLS_CAVE_ORB_OF_FORCE_WEST:
      FieldEffect_StartFadeOverlay(0);
      gImilFuchinFalls_TrackLeader = 1;
      break;
    case FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_FROM_SOUTH_HALF_MIDDLE:
      FieldEffect_StartFadeOverlay(0);
      gImilFuchinFalls_TrackLeader = 1;
      work = HEAP_SLOT_BLOCK(HEAP_SLOT_FADE_OVERLAY);
      work->actor = NULL;
      break;
    }
    if (gMainRoutineWork.entrance <=
        FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_FROM_SOUTH_HALF_EAST) {
      // The cells FuchinFallsCave_LightDragonWithEye copies as it sets the
      // flag, redrawn on arrival.
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
        wait(1);
      }
    }
  }
}

/// Restores the cell behavior templates for the current log room of Fuchin
/// Falls Cave.
static void ImilFuchinFalls_RebuildLogRoom(void) {
  if (gMainRoutineWork.next == (s32)&SceneId_FuchinFallsCaveEntrance) {
    MapGrid_CopyCellAttributes(8, 29, 15, 5, 8, 42);
    FuchinFallsCave_RebuildEntranceLogs();
  } else if (gMainRoutineWork.next == (s32)&SceneId_FuchinFallsCaveZephyr) {
    MapGrid_CopyCellAttributes(12, 8, 10, 18, 0, 28);
    FuchinFallsCave_RebuildZephyrRoomLogs();
  } else if (gMainRoutineWork.next == (s32)&SceneId_FuchinFallsCaveDragonsEye &&
             gMainRoutineWork.entrance !=
                 FUCHIN_FALLS_CAVE_DRAGONS_EYE_CHAMBER_FROM_LOGS) {
    MapGrid_CopyCellAttributes(12, 21, 9, 16, 12, 3);
    FuchinFallsCave_RebuildDragonsEyeRoomLogs();
  } else if (gMainRoutineWork.next ==
             (s32)&SceneId_FuchinFallsCaveArcticBlade) {
    if (gMainRoutineWork.entrance ==
            FUCHIN_FALLS_CAVE_ARCTIC_BLADE_FROM_FUCHIN_FALLS_CAVE_DRAGON_STATUE ||
        gMainRoutineWork.entrance ==
            FUCHIN_FALLS_CAVE_ARCTIC_BLADE_NORTH_HALF_FROM_SOUTH_HALF) {
      MapGrid_CopyCellAttributes(14, 10, 9, 8, 22, 20);
    } else {
      MapGrid_CopyCellAttributes(7, 45, 11, 4, 20, 45);
    }
    FuchinFallsCave_RebuildArcticBladeRoomLogs();
  }
}

/// Hops across a gap in Fuchin Falls Cave. The hop is west or north in the
/// second room (step triggers 13, 14 and 17). In the third it is south (step
/// trigger 25), when its first log lies at
/// FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_MIDDLE.
static void ImilFuchinFalls_HopOnTrigger(void) {
  s32 trigger = gFieldWork->stepTrigger;

  if (gMainRoutineWork.next == (s32)&SceneId_FuchinFallsCaveZephyr) {
    if (trigger == 17) {
      ImilFuchinFalls_HopBy(0, -32);
    } else {
      ImilFuchinFalls_HopBy(-32, 0);
    }
  }
  if (gMainRoutineWork.next == (s32)&SceneId_FuchinFallsCaveDragonsEye &&
      trigger == 25 &&
      GameFlag_Get(FLAG_FUCHIN_FALLS_DRAGONS_EYE_ROOM_FIRST_LOG_MIDDLE)) {
    ImilFuchinFalls_HopBy(0, 32);
  }
}

/// Hops the leader one gap north, in Fuchin Falls Cave's third room (step
/// triggers 11 and 12).
static void FuchinFallsCave_HopNorth(void) { ImilFuchinFalls_HopBy(0, -32); }

/// Hops the leader one gap west, in Fuchin Falls Cave's third room (step
/// trigger 23).
static void FuchinFallsCave_HopWest(void) { ImilFuchinFalls_HopBy(-32, 0); }

/// Jumps the leader by a pixel offset while using the hopping animations.
static void ImilFuchinFalls_HopBy(s32 dx, s32 dz) {
  Event_Start();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 2.5);
  Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, dx, dz);
  Actor_Jump(ACTOR_PARTY_LEADER, 4, 0);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_LEAP);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_SPRING);
  Event_End();
}

/// Shows the Dragon's Eye in Fuchin Falls Cave's third room, as its item icon.
static void ImilFuchinFalls_PlaceDragonsEye(void) {
  struct Actor *actor;
  struct Sprite *sprite;
  GlyphTransfer *glyph;
  u8 clear = 0;

  actor = FieldActor_Create(SPRITE_BLANK, px(248), px(8), px(152));
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

  Party_GiveItem(ITEM_DRAGONS_EYE, 0);
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
static s32 FuchinFallsCave_SetDustPalette(struct Actor *actor) {
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
  velocityX = -px(1) - ((Random_Below(2)) << 16);
  velocityY = -((Random_Below(3)) * (px(1) / 5));
  ImilFuchinFalls_SpawnDust(
      px(308), px(64), px(222), velocityX, velocityY, framePhase,
      1 | EFFECT_USE_PALETTE | EFFECT_USE_START_SCALE | EFFECT_SCALE_TO_TARGET,
      (const struct EffectOptions *)&options);
}

/// Using the Dragon's Eye on the dragon statue in Fuchin Falls Cave
/// (trigger 20) inserts the eye, lights the room and reveals its hidden path.
/// It does so only until the eye is set.
static void FuchinFallsCave_LightDragonWithEye(void) {
  u32 pass;

  // Flash the room as the eye opens the dragon's path.
  Sound_Play(MUS_SILENCE);
  Sound_Play(SE_GEM_SET);
  Event_Start();
  Field_EndBusyAndReveal();

  for (pass = 0; pass < 4; pass++) {
    PaletteFade_ApplyTarget(PALETTE_TINT_CUSTOM | RGB(24, 24, 16), PALETTES_BG);
    PaletteFade_StartInterpolation(1);
    wait(2);
    if (pass == 0) {
      MapGrid_CopyCellsTo(30, 8, 12, 8, 8, 7);
      MapGrid_CopyCellsTo(30, 57, 19, 57, 1, 1);
    }
    PaletteFade_ApplyTarget(PALETTE_TINT_CUSTOM | RGB(8, 8, 12), PALETTES_BG);
    PaletteFade_StartInterpolation(1);
    wait(2);
  }

  wait(30);

  // Blow dust from the cave mouth while the room brightens.
  Task_AddFrame(ImilFuchinFalls_BlowCaveMouthDust, TASK_PASS_FRAME);
  wait(40);
  PaletteFade_ApplyTarget(PALETTE_TINT_CUSTOM | RGB(16, 4, 4), PALETTES_BG);
  PaletteFade_StartInterpolation(40);
  wait(80);
  Task_Remove(ImilFuchinFalls_BlowCaveMouthDust);
  wait(20);

  // Settle the lighting, consume the eye and remember the revealed path.
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_BG);
  PaletteFade_StartInterpolation(80);

  wait(80);

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

/// Searching the dragon statue in Fuchin Falls Cave (examine trigger 20)
/// describes its eyes. They are its burning red eyes once set, else its empty
/// sockets.
///
/// With the Dragon's Eye carried, it then opens the File Screen as the search
/// ends, so that the eye can be used.
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

  if (gMainRoutineWork.entrance <=
      FUCHIN_FALLS_CAVE_DRAGON_STATUE_NORTH_HALF_FROM_SOUTH_HALF_EAST) {
    effectWork = *(gHeapSlots + 56);
    state = *(gHeapSlots + 31);
    effectWork->childOption = 1;
    state->nextMode = 0;
    state->duration = 1;
    state->step = 1;
    PaletteFade_ApplySource(RGB(0, 0, 0), PALETTES_BG);
    PaletteFade_ApplyTarget(PALETTE_TINT_CUSTOM | RGB(8, 8, 12), PALETTES_BG);
    PaletteFade_StartInterpolation(16);
    wait(16);
  }
}

/// Turns the actor sliding on Imil Falls Cave's ice gradually toward the
/// held direction.
static void Imil_TurnSkaterTowardHeldDirection(struct Actor *actor) {
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
    Actor_SelectAnimation(actor, ANIM_LEADER_LOOK_AROUND);
    return;
  }
  facing = actor->facing;
  direction = (s16)(direction - facing);
  if (direction > FACING_STEP)
    direction = FACING_STEP;
  if (direction < -FACING_STEP)
    direction = -FACING_STEP;
  actor->facing = facing + direction;
  Actor_SelectAnimation(actor, ANIM_WALK);
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

  actor = Actor_Lookup(gMainRoutineWork.leaderActorId);

  for (;;) {
    heading = gImilFuchinFalls_SlideHeadings[(gKeysHeld >> 4) & 15];
    if (((u32)heading << 16) == 0xffff0000) {
      return;
    }

    Event_Start();

    // Start at the cell center and reject a blocked or raised next cell.
    probe.x = (actor->x.fixed & FIELD_CELL_MASK) + px(8);
    probe.y = actor->y.fixed;
    probe.z = (actor->z.fixed & FIELD_CELL_MASK) + px(8);
    z = probe.z;
    x = probe.x;
    startBehavior = MapGrid_GetCellBehaviorAt(actor->layer, x, z);
    FieldVector_AddPolarOffset(16, heading, &probe);

    nextBehavior = MapGrid_GetCellBehaviorAt(actor->layer, probe.x, probe.z);
    if (nextBehavior == CELL_BEHAVIOR_WALL ||
        MapGrid_GetTerrainHeight(actor->layer, probe.x, probe.z) -
                actor->y.fixed >
            px(8)) {
      actor->facing = heading;
      goto tail;
    }

    probe.x = x;
    probe.z = z;
    actor->speedLimit = px(2);
    actor->acceleration = px(1) / 10;
    actor->timer = 0;
    FieldActor_SetMoveTargetFixed(actor, x, actor->y.fixed, z);

    Actor_SelectAnimation(actor, ANIM_WALK);
    Actor_SetAnimationSpeed(actor, IMIL_SLIDE_ANIMATION_STEP);
    Actor_WaitForMove(actor);
    actor->update = Imil_TurnSkaterTowardHeldDirection;

    // Keep sliding through cells with the same cell behavior.
    for (;;) {
      FieldVector_AddPolarOffset(16, heading, &probe);
      nextBehavior = MapGrid_GetCellBehaviorAt(actor->layer, probe.x, probe.z);
      if (nextBehavior == CELL_BEHAVIOR_WALL) {
        break;
      }
      if (MapGrid_GetTerrainHeight(actor->layer, probe.x, probe.z) -
              actor->y.fixed >
          px(8)) {
        break;
      }
      x = probe.x;
      z = probe.z;
      actor->speedLimit = px(2);
      actor->acceleration = px(1) / 10;
      FieldActor_SetMoveTargetFixed(actor, probe.x, probe.y, probe.z);
      Actor_WaitForMove(actor);
      if (nextBehavior != startBehavior) {
        goto blocked;
      }
    }

    actor->speedLimit = px(2);
    actor->acceleration = px(1);
    FieldActor_SetMoveTargetFixed(actor, x, actor->y.fixed, z);
    Actor_WaitForMove(actor);
    wait(2);
  }

blocked:
  // A cell behavior change ends the slide and restores normal steering.
  actor->update = NULL;
  actor->steeringFlags |= ACTOR_FACES_MOTION;

  actor->acceleration = px(1) / 4;

tail:
  wait(10);
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
    ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, FLAG_DJINNI(DJINNI_FEVER), MOVEMENT_TYPE_STAND,
        263, 0, 204, FACING_SOUTH, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gImilFuchinFalls_EntranceActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        136, 0, 504, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        200, 0, 536, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        296, 0, 536, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gImilFuchinFalls_ZephyrRoomActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        216, 0, 328, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        248, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        312, 0, 408, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JUPITER_DJINNI, FLAG_DJINNI(DJINNI_ZEPHYR), MOVEMENT_TYPE_STAND,
        344, 0, 280, FACING_SOUTH, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gImilFuchinFalls_DragonsEyeRoomActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        328, 0, 408, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        248, 0, 568, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        248, 0, 488, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        248, 0, 504, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gImilFuchinFalls_ArcticBladeRoomActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        328, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        360, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        120, 0, 744, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gImilFuchinFalls_DragonRoomActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_CRATE, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        312, 0, 128, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

struct Trigger gImilFuchinFalls_NoTriggers[] = {
    { TRIGGER_TABLE_END },
};

struct Trigger gImilFuchinFalls_ImilFallsCaveTriggers[] = {
    { TRIGGER_STEP, 1, CONDITION_ALWAYS, EVENT(Imil_LeaveFallsCave) },
    { TRIGGER_STEP, 10, CONDITION_ALWAYS, EVENT(Imil_MoveAcrossIce) },
    { TRIGGER_TALK, ACTOR_FEVER, CONDITION_ALWAYS, EVENT(Imil_RecruitFever) },
    { TRIGGER_TABLE_END },
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
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 100, FLAG_FUCHIN_FALLS_MIMIC_BEATEN, FIND_MIMIC | 1 },
    { TRIGGER_TABLE_END },
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
    { TRIGGER_TABLE_END },
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
    { TRIGGER_TABLE_END },
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
    {EXAMINE_TRIGGER(EXAMINE_CHEST), 100, FLAG_FUCHIN_FALLS_ARCTIC_BLADE,
     FIND_ITEM(ITEM_ARCTIC_BLADE)},
    {TRIGGER_TABLE_END},
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
    {TRIGGER_TABLE_END},
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
    {TRIGGER_TABLE_END},
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
