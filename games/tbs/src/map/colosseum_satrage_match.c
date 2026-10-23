/// @file
/// Colosseum Final 2 (MapId_ColosseumSatrageMatch), the site of the second
/// finals match, where Isaac, having won the first, races Satrage, the
/// fourth-ranked warrior, through five stages to the arena and fights him
/// there.
///
/// It works as Colosseum Final 1 does (colosseum_azart_match.c), and is reached
/// by the Colosseum's west passage. The stages run from east to west, through
/// the Mini-Maze, the shifting floor stage, the Log-Rolling Stage, The Wall and
/// the Board Walk. Isaac comes here three times, each at its own warp
/// destination, and ColosseumSatrageMatch_Initialize restores the stages from
/// their temporary flags (FLAG_OBSTACLE_KNOCKED_DOWN to FLAG_WALL_PILLAR_MOVED,
/// and the Mini-Maze pillars' columns from FLAG_MINI_MAZE_FIRST_PILLAR_COLUMN)
/// before setting up the visit its warp destination stands for:
///
/// - COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_WEST, the tour
///   (ColosseumSatrageMatch_IntroduceSecondFinals). The guide only greets the
///   party, since the first finals match's guide explained the rules; the five
///   stages' attendants describe their stages and offer a place for each
///   friend to cheer, and the guide then sends Isaac back to the west passage
///   (COLOSSEUM_SATRAGE_MATCH_TO_COLOSSEUM_SOUTH).
/// - COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_EAST, `Lend a Hand`: Garet, Ivan
///   and Mia in turn may use Psynergy on the stages. Move shifts the
///   Mini-Maze's stone pillars and the stone pillar by The Wall, Halt stops the
///   lifts' operator and with him the shifting floor, Frost freezes two targets
///   on the Log-Rolling Stage, Growth grows a plant up The Wall, and Force
///   knocks an obstacle down onto the Board Walk. After Mia,
///   COLOSSEUM_SATRAGE_MATCH_LEND_A_HAND_TO_RACE starts the race.
/// - COLOSSEUM_SATRAGE_MATCH_RACE_FROM_LEND_A_HAND, the race against Satrage,
///   who follows a recorded path, after the lettering `Match 2` and a count of
///   three. At the arena the first to arrive takes the Chain Mail and the
///   second the Cotton Shirt, and the battle is ENCOUNTER_ZONE_COLOSSO's third
///   or fourth group, the Satrage with or without the mail.
///
/// A win returns to COLOSSEUM_SATRAGE_MATCH_AFTER_MATCH_WON and on to the
/// Colosseum (COLOSSEUM_SATRAGE_MATCH_TO_COLOSSEUM_NORTH, arriving at
/// COLOSSEUM_FROM_COLOSSEUM_SATRAGE_MATCH_NORTH), which sets
/// FLAG_COLOSSO_SECOND_MATCH_WON; a loss returns to
/// COLOSSEUM_SATRAGE_MATCH_AFTER_MATCH_LOST, and
/// COLOSSEUM_SATRAGE_MATCH_TO_BABI_PALACE wakes the party in Babi's Palace from
/// what it takes for a dream.
///
/// The shifting floor's four lifts run on every visit until Halt stops them
/// (ColosseumSatrageMatch_UpdateLifts): every
/// COLOSSEUM_SATRAGE_MATCH_LIFT_CYCLE_FRAMES the phase flips, all four lifts
/// lined up across the water in one phase and two up and two down, by turns,
/// in the other. Each frame the task copies metatile behaviors into row 13 from
/// column 58 and into cell (58, 11): one pair of copies by phase once the
/// last lift has stopped, another while it moves
/// (https://alchemy.pascalpixel.com/places/colosseum-satrage-match/lifts.gif:
/// two cycles, the copied cells outlined and tinted by what each does, a wall,
/// a jump across or open ground, under the phase in words, the lifts lining
/// up or parting, and the frames to its next change).
///
/// Open questions: none.

#include "action_script.h"
#include "game_state.h"
#include "io_write_queue.h"
#include "map_ids.h"
#include "vram_tab.h"
#include "unit/import.h"
#include "unit/map/colosseum_satrage_match.h"
#include "unit/map/common/colosso_solo.h"
#include "unit/map/common/colosso_marker.h"
#include "unit/map/common/colosso_setup.h"
#include "unit/map/common/colosso_occupant.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/psynergy_effects.h"
#include "unit/field/field.h"
#include "unit/game/game.h"
#include "unit/window/window.h"
#include "unit/window/battle_command.h"
#include "unit/menu/menu.h"
#include "unit/sound/sound.h"
#include "unit/runtime/runtime.h"
#include "unit/map/common/colosso_item_icon.h"
#include "battle.h"
#include "keystate.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "fx_work.h"
#include "mem.h"
#include "resource.h"
#include "types.h"
#include "script_layer.h"
#include "map_tables.h"
#include "io_reg.h"
#include "callback_scheduler.h"
#include "resource_ids.h"
#include "field_service.h"
#include "text/msg_ids.h"
#include "battle_unit.h"
#include "ability_ids.h"
#include "edition.h"
#include "item_ids.h"
#include "objcmd.h"
#include "object_event.h"
#include "global_cells.h"
#include "heap_state.h"
#include "field_runtime.h"
#include "colosso_gauge.h"
#include "flags.h"
#include "songs.h"
#include "audio_engine_symbols.h"
#include "macros.h"

#define COLOSSEUM_SATRAGE_MATCH_LIFT_CYCLE_FRAMES 120
#define COLOSSEUM_SATRAGE_MATCH_LIFT_CROSSING_FRAME 75
#define COLOSSEUM_SATRAGE_MATCH_DEMONSTRATION_TIMEOUT_FRAMES 600
#define COLOSSEUM_SATRAGE_MATCH_LOG_TRAVEL_PIXELS 64

// UB: westward log travel passes -64, which cannot be shifted left as signed C.
// Both callers supply only -64, 0 or 64, so fixed-point multiplication fits.
#ifdef UBFIX
#define COLOSSEUM_SATRAGE_MATCH_OFFSET_FIXED(value) ((value) * 0x10000)
#else
#define COLOSSEUM_SATRAGE_MATCH_OFFSET_FIXED(value) ((value) << 16)
#endif

#if LANGUAGE == LANGUAGE_JAPANESE || LANGUAGE == LANGUAGE_ENGLISH
#define COLOSSEUM_SATRAGE_MATCH_ALTERNATE_PORTRAIT_PALETTE_OFFSET 64
#else
#define COLOSSEUM_SATRAGE_MATCH_ALTERNATE_PORTRAIT_PALETTE_OFFSET 160
#endif

#if LANGUAGE == LANGUAGE_JAPANESE
#define COLOSSEUM_SATRAGE_MATCH_INTRO_SCALE 256
#else
#define COLOSSEUM_SATRAGE_MATCH_INTRO_SCALE 384
#endif

#if LANGUAGE == LANGUAGE_FRENCH
#define COLOSSEUM_SATRAGE_MATCH_PULSE_FIRST_SCALE 512
#define COLOSSEUM_SATRAGE_MATCH_PULSE_HIGH_SCALE 512
#define COLOSSEUM_SATRAGE_MATCH_PULSE_LOW_SCALE 384
#elif LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_ITALIAN
#define COLOSSEUM_SATRAGE_MATCH_PULSE_FIRST_SCALE 384
#define COLOSSEUM_SATRAGE_MATCH_PULSE_HIGH_SCALE 384
#define COLOSSEUM_SATRAGE_MATCH_PULSE_LOW_SCALE 256
#else
#define COLOSSEUM_SATRAGE_MATCH_PULSE_FIRST_SCALE 512
#define COLOSSEUM_SATRAGE_MATCH_PULSE_HIGH_SCALE 384
#define COLOSSEUM_SATRAGE_MATCH_PULSE_LOW_SCALE 256
#endif

// UB: each of these calls a void function through an s32 function type, the
// call the bytes show; UBFIX calls it as declared. Each use is tagged.
#ifdef UBFIX
#define COLOSSEUM_SATRAGE_MATCH_SHOW_MESSAGE Engine_ScriptShowMessage
#define COLOSSEUM_SATRAGE_MATCH_MOVE_OBJ_EVENT_PAIR                            \
  ColosseumSatrageMatch_MovePusherAndLog
#define COLOSSEUM_SATRAGE_MATCH_START_MODE_TASK                                \
  ColosseumSatrageMatch_StartLetteringAnimation
#define COLOSSEUM_SATRAGE_MATCH_APPLY_MATCHING_SLOT_VALUE Map_GiveAndEquipItem
#else
#define COLOSSEUM_SATRAGE_MATCH_SHOW_MESSAGE                                   \
  ((s32 (*)())Engine_ScriptShowMessage)
#define COLOSSEUM_SATRAGE_MATCH_MOVE_OBJ_EVENT_PAIR                            \
  ((s32 (*)())ColosseumSatrageMatch_MovePusherAndLog)
#define COLOSSEUM_SATRAGE_MATCH_START_MODE_TASK                                \
  ((s32 (*)())ColosseumSatrageMatch_StartLetteringAnimation)
#define COLOSSEUM_SATRAGE_MATCH_APPLY_MATCHING_SLOT_VALUE                      \
  ((s32 (*)())Map_GiveAndEquipItem)
#endif

// Forward declarations.
static void ColosseumSatrageMatch_IntroduceSecondFinals(s32 attendantId);
static void ColosseumSatrageMatch_ShowRoundStartLettering(s32 mode);
static void ColosseumSatrageMatch_ShowBattleResultLettering(s32 result);
static void MapLocalState_EmptyHook(void);
static void ColosseumSatrageMatch_WaitForOpponentFinish(void);
static s32 ColosseumSatrageMatch_ApproachFinishAndTakeItem(s32 firstItem,
                                                           s32 secondItem);
static void ColosseumSatrageMatch_StartOpponentPath(s32 pathResource);
static void ColosseumSatrageMatch_PushAdjacentObstacle(void);

/// The second finals match's object events, numbered in the order of
/// gColosseumSatrageMatchObjectEventTemplates from LOCALID_FIRST_PLACED; the
/// rows of Isaac, Garet, Ivan and Mia take their own object events. By the
/// guide's and attendants' explanations, the Mini-Maze, the shifting floor
/// stage, the Log-Rolling Stage, The Wall and the Board Walk lead to the arena.
/// The stone pillars are drawn as the stone pillars Move pushes elsewhere.
enum ColosseumSatrageMatchLocalId {
  LOCALID_OPPONENT =
      LOCALID_FIRST_PLACED, ///< Satrage, racing Isaac to the arena
  /// The Board Walk's two stone pillars, on which Move does nothing.
  LOCALID_BOARD_WALK_WEST_PILLAR,
  LOCALID_BOARD_WALK_EAST_PILLAR,
  LOCALID_WALL_PILLAR, ///< the stone pillar by The Wall, pushed or moved with
                       ///< Move
  /// The Mini-Maze's three stone pillars, pushed or moved with Move.
  /// ColosseumSatrageMatch_Initialize stands them west to east, at columns 73,
  /// 76 and 79, until they have been moved.
  LOCALID_MINI_MAZE_FIRST_PILLAR,
  /// The Board Walk's three logs, in the room of its stone pillars
  /// (https://alchemy.pascalpixel.com/places/colosseum-satrage-match/map-1.png):
  /// the western and eastern logs roll, and the center one never does
  /// (ColosseumSatrageMatch_UnusedRollCenterLogNorth).
  LOCALID_ROLLING_LOG_WEST = LOCALID_MINI_MAZE_FIRST_PILLAR + 3,
  LOCALID_ROLLING_LOG_EAST,
  LOCALID_ROLLING_LOG_CENTER,
  LOCALID_FORCE_TARGET,
  LOCALID_GROWTH_PLANT,
  /// The Log-Rolling Stage's two Frost targets, drawn as the puddle Frost
  /// freezes in Lunpa (lunpa.h's SPRITE_PUDDLE).
  LOCALID_FROST_TARGET_WEST,
  LOCALID_FROST_TARGET_EAST,
  /// The shifting floor stage's four lifts, from west to east, which the
  /// lifts' operator runs.
  LOCALID_COLOSSEUM_SATRAGE_MATCH_FIRST_LIFT,
  LOCALID_COLOSSEUM_SATRAGE_MATCH_LAST_LIFT =
          LOCALID_COLOSSEUM_SATRAGE_MATCH_FIRST_LIFT +
      3,
  /// Four platforms in the lifts' sprite laid across the water channel at the
  /// Board Walk's south edge, drawn underfoot; nothing but
  /// ColosseumSatrageMatch_Initialize touches them.
  /// ★ Named from what the player sees: four white platforms across a water
  /// channel (its render)
  LOCALID_BOARD_WALK_FIRST_PLATFORM,
  /// Knocked down onto the Board Walk by Force on LOCALID_FORCE_TARGET.
  LOCALID_FALLING_OBSTACLE = LOCALID_BOARD_WALK_FIRST_PLATFORM + 4,
  /// "The operator of the lifts", as the shifting floor stage's attendant
  /// calls him: a young man with fair hair.
  LOCALID_LIFT_OPERATOR,
  LOCALID_RIDING_LOG_NORTH, ///< the Log-Rolling Stage's two logs
  LOCALID_RIDING_LOG_SOUTH,
  LOCALID_GUIDE, ///< a soldier who explains the second finals match
  /// The stages' attendants, young men with fair hair in white tunics.
  LOCALID_MINI_MAZE_ATTENDANT,
  LOCALID_SHIFTING_FLOOR_ATTENDANT,
  LOCALID_LOG_ROLLING_ATTENDANT,
  LOCALID_WALL_ATTENDANT,
  LOCALID_BOARD_WALK_ATTENDANT,
  LOCALID_PRIZE_A, ///< the arena's Chain Mail and Cotton Shirt
  LOCALID_PRIZE_B
};

/// The temporary flags that keep what the friends changed on the stages, for
/// the race (ColosseumSatrageMatch_Initialize restores them).
enum {
  /// ★ Named from the game: "This place is called the Board Walk."
  /// (MsgColossoPlaceCalledBoard)
  FLAG_OBSTACLE_KNOCKED_DOWN =
      FLAG_MAPSEC_TEMP_30, ///< by Force, on the Board Walk
  /// ★ Named from the game: "Bestows Frost when equipped" (MsgItemPlainName)
  FLAG_WEST_TARGET_FROZEN = FLAG_MAPSEC_TEMP_31, ///< by Frost
  /// ★ Named from the game: "Bestows Frost when equipped" (MsgItemPlainName)
  FLAG_EAST_TARGET_FROZEN = FLAG_MAPSEC_TEMP_32, ///< by Frost
  /// ★ Named from the game: "Growth" (MsgAbilityName)
  FLAG_PLANT_GROWN = FLAG_MAPSEC_TEMP_33, ///< by Growth, up The Wall
  /// ★ Named from the game: "Halt" (MsgAbilityName)
  FLAG_LIFTS_HALTED = FLAG_MAPSEC_TEMP_34, ///< by Halt on LOCALID_LIFT_OPERATOR
  /// The stone pillar by The Wall stands at column 36.
  /// ★ Named from the game: "The fans just call this The Wall."
  /// (MsgColossoFansJustCallWall)
  FLAG_WALL_PILLAR_MOVED = FLAG_MAPSEC_TEMP_35,
  /// The bytes of flags that keep the Mini-Maze's three pillars' columns.
  /// ★ Named from the game: "This stage is dubbed the Mini-Maze."
  /// (MsgColossoStageDubbedMini)
  FLAG_MINI_MAZE_FIRST_PILLAR_COLUMN = FLAG_MAPSEC_TEMP_40,
  /// ★ Named from the game: "This stage is dubbed the Mini-Maze."
  /// (MsgColossoStageDubbedMini)
  FLAG_MINI_MAZE_SECOND_PILLAR_COLUMN = FLAG_MAPSEC_TEMP_48,
  /// ★ Named from the game: "This stage is dubbed the Mini-Maze."
  /// (MsgColossoStageDubbedMini)
  FLAG_MINI_MAZE_THIRD_PILLAR_COLUMN = FLAG_MAPSEC_TEMP_50,
  /// Someone has reached the arena
  /// (ColosseumSatrageMatch_ApproachFinishAndTakeItem).
  /// ★ Named from the game: "You must get through several stages before you
  /// reach the arena." (MsgColossoWarriorsEnterFinalsWithoutAny)
  FLAG_ARENA_REACHED = FLAG_MAP_TEMP_11
};

/// Sets the party leader's walking speed and acceleration.
static inline void InitializeObjectEventZero(void) {
  Engine_ObjectEventSetSpeed(LOCALID_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
}

/// Sets the specified object event's walking speed and acceleration.
static inline void InitializeSelectedObjectEvent(s32 localId) {
  Engine_ObjectEventSetSpeed(localId, PIXELS(1), PIXELS(1) / 2);
}

const struct MapWarpDest *Map_GetWarpDests(void) {
  return gColosseumSatrageMatchWarpDests;
}

const struct MapWarpRegion *Map_GetWarpRegions(void) { return 0; }

/// Returns the packed stage and battle-result warps.
const u32 *Map_GetWarps(void) { return gColosseumSatrageMatchWarps; }

/// Returns Colosseum Final 2's warriors, obstacles and attendants.
const struct ObjectEventTemplate *Map_GetObjectEventTemplates(void) {
  return gColosseumSatrageMatchObjectEventTemplates;
}

/// Move on the Board Walk's two stone pillars does nothing.
static void EventScript_IgnoreMove(void) {}

/// Walking into a stone pillar pushes it. In Colosseum Final 2, walking into
/// one at coord event 21 pushes the obstacle immediately ahead of the selected
/// object event.
static void EventScript_PushPillar(void) {
  ColosseumSatrageMatch_PushAdjacentObstacle();
}

/// Move on the stone pillar by The Wall, and after each push: its cell is
/// updated, and FLAG_WALL_PILLAR_MOVED keeps whether it stands at column 36.
static void EventScript_UpdateMovedPillarCell(void) {
  struct ObjectEvent *obstacle;
  s32 column;
  s32 row;

  obstacle = ObjectEvent_GetById(LOCALID_WALL_PILLAR);
  if ((obstacle->x.fixed >> 20) == 36) {
    Engine_GameFlagSet(FLAG_WALL_PILLAR_MOVED);
    column = 0x23;
    row = 0x4D;
    MapGrid_CopyCellAttributes(0x23, 0x4E, 1, 1, column, row);
  } else {
    Engine_GameFlagClear(FLAG_WALL_PILLAR_MOVED);
    column = 0x23;
    row = 0x4D;
    MapGrid_CopyCellAttributes(0x22, 0x4D, 1, 1, column, row);
  }
}

/// Walking into the stone pillar by The Wall (coord events 20 and 26) pushes
/// it, then updates its cell.
static void EventScript_PushPillarAndUpdateCells(void) {
  ColosseumSatrageMatch_PushAdjacentObstacle();
  EventScript_UpdateMovedPillarCell();
}

/// Move on a stone pillar in the Mini-Maze, and after each push: the three
/// pillars' columns are kept (FLAG_MINI_MAZE_FIRST_PILLAR_COLUMN and the two
/// after it) and their cells rebuilt.
static void EventScript_SaveMovedPillarColumns(void) {

  struct ObjectEvent *obstacle;
  s32 column;

  MapGrid_CopyCellAttributes(100, 11, 12, 4, 14, 11);

  obstacle = ObjectEvent_GetById(LOCALID_MINI_MAZE_FIRST_PILLAR);
  column = obstacle->x.fixed >> 20;
  FieldGameFlag_SetByte(FLAG_MINI_MAZE_FIRST_PILLAR_COLUMN, column);
  MapGrid_CopyCellAttributes(71, 16, 1, 1, column, 16);

  obstacle = ObjectEvent_GetById(LOCALID_MINI_MAZE_FIRST_PILLAR + 1);
  column = obstacle->x.fixed >> 20;
  FieldGameFlag_SetByte(FLAG_MINI_MAZE_SECOND_PILLAR_COLUMN, column);
  MapGrid_CopyCellAttributes(71, 16, 1, 1, column, 16);

  obstacle = ObjectEvent_GetById(LOCALID_MINI_MAZE_FIRST_PILLAR + 2);
  column = obstacle->x.fixed >> 20;
  FieldGameFlag_SetByte(FLAG_MINI_MAZE_THIRD_PILLAR_COLUMN, column);
  MapGrid_CopyCellAttributes(71, 16, 1, 1, column, 16);
}

/// Force on the target over the Board Walk, before its effect plays: the effect
/// takes its child mode.
static void EventScript_PrepareForce(void) {
  u8 *effectWork = gFieldEffectWork;

  ((struct FieldEffectWork *)effectWork)->childMode = 1;
}

/// Force on the target over the Board Walk: the obstacle spins down into place
/// and the way past it opens (FLAG_OBSTACLE_KNOCKED_DOWN).
static void EventScript_KnockDownObstacle(void) {
  struct ObjectEvent *obstacle;
  s32 fallFrames;
  s32 rotationStep;
  s32 settleFrames;
  s32 settleRotationStep;
  struct Sprite *sprite;

  obstacle = ObjectEvent_GetById(LOCALID_FALLING_OBSTACLE);
  sprite = obstacle->sprite;
  GameFlag_Set(FLAG_OBSTACLE_KNOCKED_DOWN);
  obstacle->acceleration = 0x1999;
  obstacle->speed = 0x13333;
  Engine_AudioPlayCue(SE_TOPPLE);
  // Spin through the fall, then settle the obstacle at its final height.
  Object_SetPosition(obstacle, PIXELS(336), PIXELS(10), PIXELS(264));
  for (rotationStep = 0, fallFrames = 9; fallFrames >= 0; fallFrames--) {
    sprite->rotation -= rotationStep;
    Engine_TaskWait(1);
    rotationStep += 36;
  }
  Object_SetPosition(obstacle, PIXELS(330), -PIXELS(16), PIXELS(264));
  for (settleRotationStep = 0x168, settleFrames = 21; settleFrames >= 0;
       settleFrames--) {
    sprite->rotation -= settleRotationStep;
    Engine_TaskWait(1);
    settleRotationStep += 36;
  }
  Object_WaitForMove(obstacle);
  Engine_ScriptWait(2);
  Engine_AudioPlayCue(SE_240);
  {
    s32 rotation = 0;

    sprite->rotation = rotation;
  }
  Engine_ObjectEventSetAnimation(LOCALID_FALLING_OBSTACLE,
                                 ANIMATION_SHAKE_HEAD);
  obstacle->x.fixed = PIXELS(336);
  obstacle->y.fixed = -PIXELS(8);
  obstacle->z.fixed = PIXELS(264);
  obstacle->velocityY = 0;
  obstacle->velocityX = 0;
  // Keep both the stage and its saved metatile behavior strip in the new state.
  MapGrid_CopyCellAttributes(19, 16, 1, 1, 20, 16);
  MapGrid_CopyCellAttributes(20, 80, 1, 1, 21, 80);
}

/// Frost on the Log-Rolling Stage's western target: it freezes and its crossing
/// opens (FLAG_WEST_TARGET_FROZEN).
static void EventScript_FreezeWestTarget(void) {
  struct ObjectEvent *obstacle;

  Engine_GameFlagSet(FLAG_WEST_TARGET_FROZEN);
  obstacle = ObjectEvent_GetById(LOCALID_FROST_TARGET_WEST);
  obstacle->motionFlags = 0;
  {

    MapGrid_CopyCellAttributes(46, 17, 1, 1, 44, 17);
  }
}

/// Frost on the Log-Rolling Stage's eastern target: it freezes and its crossing
/// opens (FLAG_EAST_TARGET_FROZEN).
static void EventScript_FreezeEastTarget(void) {
  struct ObjectEvent *obstacle;

  Engine_GameFlagSet(FLAG_EAST_TARGET_FROZEN);
  obstacle = ObjectEvent_GetById(LOCALID_FROST_TARGET_EAST);
  obstacle->motionFlags = 0;
  {

    MapGrid_CopyCellAttributes(46, 17, 1, 1, 50, 17);
  }
}

/// Growth on the plant by The Wall: it grows into a strip that can be climbed
/// (FLAG_PLANT_GROWN).
static void EventScript_GrowPlant(void) {
  Engine_GameFlagSet(FLAG_PLANT_GROWN);
  {

    MapGrid_CopyCellAttributes(32, 37, 1, 4, 32, 77);
  }
}

/// The selected object event jumps down off the ledge by an obstacle. Walking
/// south into the obstacle at coord event 30 in Colosseum Final 2 plays the hop
/// the field plays at any ledge (FieldObjectEvent_JumpDownLedge) for the object
/// event touching it.
static void EventScript_JumpDownLedge(void) {
  FieldObjectEvent_JumpDownLedge(gGameState.leaderLocalId, 6, 0);
}

/// Walking west into the obstacle at coord event 30 in Colosseum Final 2 only
/// checks what is ahead.
static void EventScript_CheckAhead(void) { Engine_LeaderCheckAhead(); }

/// Moves a riding log and its pusher to aligned cell centers.
static void ColosseumSatrageMatch_MovePusherAndLog(s32 logId, s32 offsetX,
                                                   s32 offsetZ) {

  struct ObjectEvent *pusher;
  struct ObjectEvent *log;
  s32 destinationX;
  s32 destinationZ;

  pusher = ObjectEvent_GetById(gGameState.leaderLocalId);
  log = ObjectEvent_GetById(logId);
  Engine_ScriptStart();
  {
    destinationX =
        ((pusher->x.fixed + COLOSSEUM_SATRAGE_MATCH_OFFSET_FIXED(offsetX)) &
         0xFFF00000) +
        0x80000;
    destinationZ =
        ((pusher->z.fixed + COLOSSEUM_SATRAGE_MATCH_OFFSET_FIXED(offsetZ)) &
         0xFFF00000) +
        0x80000;

    pusher->speed = 0x10000;
    pusher->acceleration = 0x8000;
    Engine_ObjectSetPosition(pusher, destinationX, pusher->y.fixed,
                             destinationZ);
  }
  ObjectEvent_SetMode(pusher, 27);
  {
    destinationX =
        ((log->x.fixed + COLOSSEUM_SATRAGE_MATCH_OFFSET_FIXED(offsetX)) &
         0xFFF00000) +
        0x80000;
    destinationZ =
        ((log->z.fixed + COLOSSEUM_SATRAGE_MATCH_OFFSET_FIXED(offsetZ)) &
         0xFFF00000) +
        0x80000;

    log->speed = 0x10000;
    log->acceleration = 0x8000;
    Engine_ObjectSetPosition(log, destinationX, log->y.fixed, destinationZ);
  }
  if (offsetX < 0 || offsetZ < 0) {
    ObjectEvent_SetMode(log, 4);
  } else {
    ObjectEvent_SetMode(log, 3);
  }
  Engine_AudioPlayCue(SE_226);
  Object_WaitForMove(pusher);
  Engine_AudioPlayCue(SE_STOP_SLOT3);
  ObjectEvent_SetMode(log, 2);
  Engine_ScriptEnd();
}

/// Standing on one of the Log-Rolling Stage's logs (coord event 22): it carries
/// the rider across the water the way the player holds.
static void EventScript_RideLog(void) {
  struct ObjectEvent *pusher = ObjectEvent_GetById(gGameState.leaderLocalId);
  s32 column = pusher->x.fixed >> 20;
  s32 offsetX = 0;
  s32 logId = LOCALID_RIDING_LOG_NORTH;

  if (pusher->z.fixed >> 20 > 12) {
    logId = LOCALID_RIDING_LOG_SOUTH;
  }
  if (ObjectEvent_GetById(logId)->x.fixed >> 20 != column) {
    return;
  }
  if (column > 51) {
    if (gKeysHeld & KEY_LEFT) {
      offsetX = -COLOSSEUM_SATRAGE_MATCH_LOG_TRAVEL_PIXELS;
    }
  } else if (gKeysHeld & KEY_RIGHT) {
    offsetX = COLOSSEUM_SATRAGE_MATCH_LOG_TRAVEL_PIXELS;
  }
  if (offsetX != 0) {
    ColosseumSatrageMatch_MovePusherAndLog(logId, offsetX, 0);
    // Replace both log footprints after the moving pair has stopped.
    MapGrid_CopyCellAttributes(120, 10, 5, 6, 48, 10);
    column = ObjectEvent_GetById(LOCALID_RIDING_LOG_NORTH)->x.fixed >> 20;
    MapGrid_CopyCellAttributes(52, 28, 1, 3, column, 10);
    column = ObjectEvent_GetById(LOCALID_RIDING_LOG_SOUTH)->x.fixed >> 20;
    MapGrid_CopyCellAttributes(52, 28, 1, 3, column, 13);
  }
}

/// Rolls a log to half-cell coordinates while its pusher follows halfway.
static void ColosseumSatrageMatch_RollLogToCell(s32 logId, s32 halfCellX,
                                                s32 halfCellZ) {
  s32 pusher = gGameState.leaderLocalId;
  struct ObjectEvent *pusherObjectEvent = ObjectEvent_GetById(pusher);
  struct ObjectEvent *log = ObjectEvent_GetById(logId);
  s32 heading = (pusherObjectEvent->facing + 0x1000) & 0xe000;
  s32 alongX = (log->x.fixed >> 20) != halfCellX / 2;
  s32 offsetX;
  s32 offsetZ;

  halfCellX <<= 19;
  halfCellZ <<= 19;
  if (alongX) {
    offsetX = (halfCellX - log->x.fixed) / 2;
    offsetZ = 0;
  } else {
    offsetX = 0;
    offsetZ = (halfCellZ - log->z.fixed) / 2;
  }
  Engine_ScriptStart();
  Engine_ObjectEventSetAnimation(pusher, 8);
  Engine_ScriptWait(6);
  log->speed = 0x8000;
  log->acceleration = 0x3333;
  ObjectEvent_SetMode(log,
                      gColosseumSatrageMatch_RollLogScript[heading / 0x4000]);
  Engine_ObjectSetPosition(log, halfCellX, 0, halfCellZ);
  Engine_ScriptWait(6);
  Engine_ObjectEventSetAnimation(pusher, ANIMATION_WALK);
  // Follow halfway behind the rolling log while its finish effect runs.
  ObjectDispatch_StartCamera(
      (struct DispatchObject *)((struct EventWork *)AllocHeapSlotEwram(
                                    HEAP_SLOT_FIELD, sizeof(struct FieldWork)))
          ->camera,
      (s32)log);
  Engine_ObjectEventSetSpeed(pusher, PIXELS(1) / 2, PIXELS(1) / 5);
  ObjectEvent_SetMode(pusherObjectEvent, 2);
  Engine_ObjectSetPosition(pusherObjectEvent,
                           pusherObjectEvent->x.fixed + offsetX, 0,
                           pusherObjectEvent->z.fixed + offsetZ);
  Engine_AudioPlayCue(SE_HEAVY_MOVE);
  Object_WaitForMove(pusherObjectEvent);
  ObjectEvent_SetMode(pusherObjectEvent, 1);
  Object_WaitForMove(log);
  Engine_AudioPlayCue(SE_STOP_SLOT3);
  Engine_AudioPlayCue(SE_HEAVY_STOP);
  ObjectEvent_SetMode(log, 1);
  Engine_ScriptWait(15);
  Engine_ScriptEnd();
}

/// Rebuilds the metatile behaviors under the three rolling logs at their
/// positions.
static void ColosseumSatrageMatch_UpdateRollingLogCollision(void) {
  s32 cell;

  MapGrid_CopyCellAttributes(100, 11, 12, 4, 14, 11);

  cell = ObjectEvent_GetById(LOCALID_ROLLING_LOG_WEST)->x.fixed >> 20;
  MapGrid_CopyCellAttributes(13, 28, 1, 4, cell, 11);

  cell = ObjectEvent_GetById(LOCALID_ROLLING_LOG_EAST)->x.fixed >> 20;
  MapGrid_CopyCellAttributes(13, 28, 1, 4, cell, 11);

  cell = ObjectEvent_GetById(LOCALID_ROLLING_LOG_CENTER)->z.fixed >> 20;
  MapGrid_CopyCellAttributes(13, 28, 4, 1, 18, cell);
}

/// Walking west into the Board Walk's western log rolls it west. Walking into
/// the western log from the east (coord event 24) or the west (25), or into the
/// eastern one from the east (28) or the west (27), rolls it the way the leader
/// walks.
static void EventScript_RollWestLogWest(void) {
  ColosseumSatrageMatch_RollLogToCell(LOCALID_ROLLING_LOG_WEST, 29, 26);
  ColosseumSatrageMatch_UpdateRollingLogCollision();
}

/// Walking east into the Board Walk's western log (coord event 25) rolls it
/// east.
static void EventScript_RollWestLogEast(void) {
  ColosseumSatrageMatch_RollLogToCell(LOCALID_ROLLING_LOG_WEST, 33, 26);
  ColosseumSatrageMatch_UpdateRollingLogCollision();
}

/// Walking west into the Board Walk's eastern log (coord event 28) rolls it
/// west.
static void EventScript_RollEastLogWest(void) {
  ColosseumSatrageMatch_RollLogToCell(LOCALID_ROLLING_LOG_EAST, 45, 26);
  ColosseumSatrageMatch_UpdateRollingLogCollision();
}

/// Walking east into the Board Walk's eastern log (coord event 27) rolls it
/// east.
static void EventScript_RollEastLogEast(void) {
  ColosseumSatrageMatch_RollLogToCell(LOCALID_ROLLING_LOG_EAST, 49, 26);
  ColosseumSatrageMatch_UpdateRollingLogCollision();
}

/// Unused: no map event row calls it, so the Board Walk's center log never
/// rolls. Walking into it would have rolled it north, as the western and
/// eastern logs roll west and east (EventScript_RollWestLogWest and the rest).
static UNUSED void ColosseumSatrageMatch_UnusedRollCenterLogNorth(void) {
  ColosseumSatrageMatch_RollLogToCell(LOCALID_ROLLING_LOG_CENTER, 40, 23);
  ColosseumSatrageMatch_UpdateRollingLogCollision();
}

/// Unused: as ColosseumSatrageMatch_UnusedRollCenterLogNorth, rolling the
/// center log south.
static UNUSED void ColosseumSatrageMatch_UnusedRollCenterLogSouth(void) {
  ColosseumSatrageMatch_RollLogToCell(LOCALID_ROLLING_LOG_CENTER, 40, 25);
  ColosseumSatrageMatch_UpdateRollingLogCollision();
}

/// The shifting floor's callback: carries the selected object event on the
/// lifts and alternates their routes every two seconds, until Halt stops the
/// operator.
static void ColosseumSatrageMatch_UpdateLifts(void) {
  s32 liftId;
  struct ObjectEvent *rider;
  struct ObjectEvent *lift;
  s32 liftX;
  s32 riderX;
  s32 deltaX;
  s32 liftZ;
  s32 riderZ;
  s32 deltaZ;
  struct ObjectEvent *liftObjectEvent;

  rider = ObjectEvent_GetById(gGameState.leaderLocalId);
  // Carry a rider only while both horizontal distances are less than ten
  // pixels.
  for (liftId = LOCALID_COLOSSEUM_SATRAGE_MATCH_FIRST_LIFT;
       liftId <= LOCALID_COLOSSEUM_SATRAGE_MATCH_LAST_LIFT; liftId++) {
    lift = ObjectEvent_GetById(liftId);
    lift->paused = 0;
    liftX = lift->x.fixed;
    riderX = rider->x.fixed;
    deltaX = liftX - riderX;
    if (deltaX >= 0) {
      if (deltaX > 0x9ffff) {
        continue;
      }
    } else {
      riderX -= liftX;
      if (riderX > 0x9ffff) {
        continue;
      }
    }
    liftZ = lift->z.fixed;
    riderZ = rider->z.fixed;
    deltaZ = liftZ - riderZ;
    if (deltaZ >= 0) {
      if (deltaZ > 0x9ffff) {
        continue;
      }
    } else {
      riderZ -= liftZ;
      if (riderZ > 0x9ffff) {
        continue;
      }
    }
    if (GameFlag_IsSet(FLAG_FIELD_HANDLING_REQUESTS) != 0) {
      riderZ = lift->z.fixed;
    } else {
      riderZ = rider->z.fixed;
      deltaZ = lift->velocityZ;
      riderZ += deltaZ;
    }
    rider->z.fixed = riderZ;
  }
  // The final lift's destination selects the metatile behaviors for the row.
  if (gColosseumSatrageMatch_LiftTimer != 0 && lift->targetX == 0x80000000) {
    if (gColosseumSatrageMatch_LiftPhase == 0) {
      MapGrid_CopyCellAttributes(58, 28, 7, 1, 58, 13);
    } else {
      MapGrid_CopyCellAttributes(58, 10, 1, 1, 58, 11);
    }
  } else {
    MapGrid_CopyCellAttributes(57, 11, 1, 1, 58, 11);
    MapGrid_CopyCellAttributes(58, 14, 7, 1, 58, 13);
  }
  // Start the next alternating route when the two-second cycle wraps.
  if (gColosseumSatrageMatch_LiftTimer == 0) {
    gColosseumSatrageMatch_LiftPhase ^= 1;
    if (gColosseumSatrageMatch_LiftPhase != 0) {
      liftObjectEvent =
          ObjectEvent_GetById(LOCALID_COLOSSEUM_SATRAGE_MATCH_FIRST_LIFT);
      Object_SetPosition(liftObjectEvent, PIXELS(936), 0, PIXELS(184));
      liftObjectEvent =
          ObjectEvent_GetById(LOCALID_COLOSSEUM_SATRAGE_MATCH_FIRST_LIFT + 1);
      Object_SetPosition(liftObjectEvent, PIXELS(968), 0, PIXELS(248));
      liftObjectEvent =
          ObjectEvent_GetById(LOCALID_COLOSSEUM_SATRAGE_MATCH_FIRST_LIFT + 2);
      Object_SetPosition(liftObjectEvent, PIXELS(1000), 0, PIXELS(184));
      liftObjectEvent =
          ObjectEvent_GetById(LOCALID_COLOSSEUM_SATRAGE_MATCH_LAST_LIFT);
      Object_SetPosition(liftObjectEvent, PIXELS(1032), 0, PIXELS(248));
      Engine_ObjectEventSetAnimation(LOCALID_LIFT_OPERATOR, 11);
    } else {
      liftObjectEvent =
          ObjectEvent_GetById(LOCALID_COLOSSEUM_SATRAGE_MATCH_FIRST_LIFT);
      Object_SetPosition(liftObjectEvent, PIXELS(936), 0, PIXELS(216));
      liftObjectEvent =
          ObjectEvent_GetById(LOCALID_COLOSSEUM_SATRAGE_MATCH_FIRST_LIFT + 1);
      Object_SetPosition(liftObjectEvent, PIXELS(968), 0, PIXELS(216));
      liftObjectEvent =
          ObjectEvent_GetById(LOCALID_COLOSSEUM_SATRAGE_MATCH_FIRST_LIFT + 2);
      Object_SetPosition(liftObjectEvent, PIXELS(1000), 0, PIXELS(216));
      liftObjectEvent =
          ObjectEvent_GetById(LOCALID_COLOSSEUM_SATRAGE_MATCH_LAST_LIFT);
      Object_SetPosition(liftObjectEvent, PIXELS(1032), 0, PIXELS(216));
      Engine_ObjectEventSetAnimation(LOCALID_LIFT_OPERATOR, 10);
    }
  }
  gColosseumSatrageMatch_LiftTimer++;
  if ((u32)gColosseumSatrageMatch_LiftTimer >=
      COLOSSEUM_SATRAGE_MATCH_LIFT_CYCLE_FRAMES) {
    if (GameFlag_IsSet(FLAG_FIELD_HANDLING_REQUESTS) == 0) {
      gColosseumSatrageMatch_LiftTimer = 0;
    }
  }
}

/// Stops the moving-lift task and restores its starting row.
static void ColosseumSatrageMatch_ResetLifts(void) {
  gColosseumSatrageMatch_LiftTimer = 0;
  gColosseumSatrageMatch_LiftPhase = 0;
  Engine_TaskRemoveCallback(ColosseumSatrageMatch_UpdateLifts);
  ObjectEvent_SetPosition(LOCALID_COLOSSEUM_SATRAGE_MATCH_FIRST_LIFT,
                          PIXELS(936), PIXELS(216));
  ObjectEvent_SetPosition(LOCALID_COLOSSEUM_SATRAGE_MATCH_FIRST_LIFT + 1,
                          PIXELS(968), PIXELS(216));
  ObjectEvent_SetPosition(LOCALID_COLOSSEUM_SATRAGE_MATCH_FIRST_LIFT + 2,
                          PIXELS(1000), PIXELS(216));
  ObjectEvent_SetPosition(LOCALID_COLOSSEUM_SATRAGE_MATCH_LAST_LIFT,
                          PIXELS(1032), PIXELS(216));
  Engine_ObjectEventSetAnimation(LOCALID_LIFT_OPERATOR, 10);
}

/// Starts the moving-lift task and releases its pause flag.
static void ColosseumSatrageMatch_StartLifts(void) {
  FieldScheduler_AddCallback(ColosseumSatrageMatch_UpdateLifts,
                             TASK_PRIORITY_MAP + 5);
  Engine_GameFlagClear(FLAG_FIELD_HANDLING_REQUESTS);
}

/// Waits for the demonstration's crossing phase, with a ten-second timeout.
static void ColosseumSatrageMatch_WaitForLiftCrossingPhase(void) {
  s32 elapsedFrames;

  Engine_TaskWait(10);

  elapsedFrames = 0;
  while (gColosseumSatrageMatch_LiftPhase != 0 ||
         gColosseumSatrageMatch_LiftTimer !=
             COLOSSEUM_SATRAGE_MATCH_LIFT_CROSSING_FRAME) {
    Engine_TaskWait(1);
    elapsedFrames++;
    if (elapsedFrames >= COLOSSEUM_SATRAGE_MATCH_DEMONSTRATION_TIMEOUT_FRAMES) {
      return;
    }
  }
}

/// Halt on the shifting floor's lift operator: the lifts stop and leave a
/// crossing (FLAG_LIFTS_HALTED).
static void EventScript_HaltLifts(void) {
  FieldEffect_PauseObject(LOCALID_LIFT_OPERATOR);
  Engine_GameFlagSet(FLAG_LIFTS_HALTED);

  if (gColosseumSatrageMatch_LiftPhase != 0) {
    gColosseumSatrageMatch_LiftTimer = 0;
  }

  Engine_TaskWait(30);
  Engine_TaskWait(1);

  Engine_TaskRemoveCallback(ColosseumSatrageMatch_UpdateLifts);

  MapGrid_CopyCellAttributes(58, 28, 7, 1, 58, 13);
  MapGrid_CopyCellAttributes(57, 11, 1, 1, 58, 11);
}

/// Reaching Colosseum Final 2's arena: the leader takes a prize (the Chain Mail
/// if first, the Cotton Shirt if second), and he and Satrage face off and
/// fight; the battle's group follows who took the mail.
/// The arena's coord event row waits on FLAG_MAP_TEMP_10 being clear, which
/// nothing in the game sets (colosseum_azart_match.c's EventScript_ReachArena).
static void EventScript_ReachArena(void) {
  s32 opponentFinishedFirst;
  s32 refreshCount;
  s32 resultBase;

  resultBase = 2;
  MapLocalState_EmptyHook();
  Engine_ScriptStart();
  opponentFinishedFirst = ColosseumSatrageMatch_ApproachFinishAndTakeItem(
      ITEM_CHAIN_MAIL, ITEM_COTTON_SHIRT);
  ColosseumSatrageMatch_WaitForOpponentFinish();
  refreshCount = 9;
  do {
    ObjectEvent_RefreshSelectorById(LOCALID_OPPONENT);
    refreshCount = (refreshCount - 1);
  } while (refreshCount >= 0);
  // Bring the warriors together before selecting the battle's result bias.
  ObjectEvent_SetSpeed(LOCALID_OPPONENT, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkTo(LOCALID_OPPONENT, 88, 256);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 120, 256);
  Engine_ObjectEventSetAnimation(LOCALID_OPPONENT, ANIMATION_STAND);
  Engine_ObjectEventFaceEachOther(LOCALID_PARTY_LEADER, LOCALID_OPPONENT, 0);
  Engine_ScriptWait(10);
  Engine_ObjectEventSetAnimation(LOCALID_OPPONENT, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  Engine_ScriptWait(20);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(2), PIXELS(1));
  ObjectEvent_SetSpeed(LOCALID_OPPONENT, PIXELS(2), PIXELS(1));
  ObjectEvent_WalkTo(LOCALID_PARTY_LEADER, 112, 256);
  Engine_ObjectEventWalkToAndWait(LOCALID_OPPONENT, 96, 0x100);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_LEADER_STRIDE);
  Engine_ObjectEventSetAnimation(LOCALID_OPPONENT,
                                 WARRIOR_ANIMATION_FIGHTING_STANCE);
  Engine_ScriptWait(10);
  /* FAKEMATCH(temporary): resultBase, set to 2 at the top, keeps the + 1
   * unfolded; as 3 - opponentFinishedFirst, which shares r5's 3 with
   * Battle_SetType(BATTLE_TYPE_SCRIPTED), 3 of 106 instructions differ in
   * tbs-en. */
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_COLOSSO,
                                        resultBase - opponentFinishedFirst + 1);
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  FieldScript_SetNextDestination((s32)&MapId_ColosseumSatrageMatch,
                                 COLOSSEUM_SATRAGE_MATCH_AFTER_MATCH_WON);
  FieldScript_SetDefeatDestination((s32)&MapId_ColosseumSatrageMatch,
                                   COLOSSEUM_SATRAGE_MATCH_AFTER_MATCH_LOST);
  Engine_GameFlagSet(FLAG_KEEP_MUSIC);
}

/// Returns Colosseum Final 2's obstacle interactions and attendant dialogue.
const struct MapEvent *Map_GetEvents(void) {
  return gColosseumSatrageMatchEvents;
}

/// Back from the battle (COLOSSEUM_SATRAGE_MATCH_AFTER_MATCH_WON or _LOST):
/// Isaac and Satrage stand in the arena as the result is shown.
static void ColosseumSatrageMatch_StageBattleResult(s32 result) {

  Engine_ObjectEventDestroy(LOCALID_PRIZE_A);
  Engine_ObjectEventDestroy(LOCALID_PRIZE_B);
  Party_RestoreHpPp(1);
  Engine_ScriptStart();
  ObjectEvent_SetPosition(LOCALID_OPPONENT, PIXELS(88), PIXELS(256));
  ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, PIXELS(120), PIXELS(256));
  // BUG: 0x4000 is FACING_SOUTH, passed where ObjectEvent_FaceObjectEvent takes
  // a local id; ObjectEvent_GetByLocalId has no object event past 0xbf, so
  // neither turns.
#ifdef BUGFIX
  ObjectEvent_FaceDirection(LOCALID_OPPONENT, FACING_SOUTH, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 0);
#else
  ObjectEvent_FaceObjectEvent(LOCALID_OPPONENT, 0x4000, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, 0x4000, 0);
#endif
  if (result < 0) {
    Engine_ObjectEventSetAnimation(LOCALID_OPPONENT,
                                   WARRIOR_ANIMATION_ARMS_RAISED);
    Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER,
                                   ANIMATION_LEADER_CROUCH);
  } else {
    Engine_ObjectEventSetAnimation(LOCALID_OPPONENT,
                                   WARRIOR_ANIMATION_DEFEATED);
    Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER,
                                   ANIMATION_LEADER_ARMS_RAISED);
  }
  Engine_TaskWait(1);
  FieldCamera_MoveTo(PIXELS(104), 0, PIXELS(192), 0);
  ColosseumSatrageMatch_ShowBattleResultLettering(result);
  Engine_ScriptEnd();
}

/// Sets up Colosseum Final 2: restores obstacle progress, then by the arrival:
/// the race (COLOSSEUM_SATRAGE_MATCH_RACE_FROM_LEND_A_HAND), the friends' turns
/// to lend a hand (COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_EAST), the guide's
/// tour (COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_WEST), or the match won
/// (COLOSSEUM_SATRAGE_MATCH_AFTER_MATCH_WON) or lost
/// (COLOSSEUM_SATRAGE_MATCH_AFTER_MATCH_LOST).
s32 ColosseumSatrageMatch_Initialize(void) {
  struct ObjectEvent *objectEvent;
  s32 x;
  s32 obstacleId;
  s32 column;
  s32 liftId;
  s32 columnOffset;
  s32 row;
  s32 zero;

  gFieldWork->transition = MAP_TRANSITION(TRANSITION_FADE, 0);
  // Restore the metatile behavior templates before applying saved puzzle
  // progress.
  Engine_GameFlagSet(FLAG_RETREAT_BLOCKED);
  MapGrid_CopyCellAttributes(14, 11, 12, 4, 100, 11);
  MapGrid_CopyCellAttributes(48, 10, 5, 6, 120, 10);
  for (obstacleId = LOCALID_BOARD_WALK_FIRST_PLATFORM;
       obstacleId <= LOCALID_FALLING_OBSTACLE; obstacleId++) {
    objectEvent = ObjectEvent_GetById(obstacleId);
    ObjectEvent_SetMode(objectEvent, 4);
    objectEvent->motionFlags = 0;
    objectEvent->y.fixed = 0;
    objectEvent->priorityFlags = 2;
  }
  ObjectEvent_GetById(LOCALID_FORCE_TARGET)->priorityFlags = 2;
  if (Engine_GameFlagIsSet(FLAG_OBSTACLE_KNOCKED_DOWN)) {
    objectEvent = ObjectEvent_GetById(LOCALID_FALLING_OBSTACLE);
    objectEvent->x.fixed = PIXELS(336);
    objectEvent->y.fixed = -PIXELS(8);
    objectEvent->z.fixed = PIXELS(264);
    MapGrid_CopyCellAttributes(19, 16, 1, 1, 20, 16);
    MapGrid_CopyCellAttributes(20, 80, 1, 1, 21, 80);
  } else {
    objectEvent = ObjectEvent_GetById(LOCALID_FALLING_OBSTACLE);
    ObjectEvent_SetMode(objectEvent, 3);
    objectEvent->y.fixed = PIXELS(16);
  }
  ObjectEvent_GetById(LOCALID_WALL_PILLAR)->priorityFlags = 2;
  zero = 0;
  if (Engine_GameFlagIsSet(FLAG_WALL_PILLAR_MOVED)) {
    MapGrid_CopyCellAttributes(35, 78, 1, 1, 35, 77);
  }
  if (Engine_GameFlagIsSet(FLAG_PLANT_GROWN)) {
    Engine_ObjectEventSetAnimation(LOCALID_GROWTH_PLANT, ANIMATION_SHAKE_HEAD);
    MapGrid_CopyCellAttributes(32, 37, 1, 4, 32, 77);
  }
  if (Engine_GameFlagIsSet(FLAG_WEST_TARGET_FROZEN)) {
    ObjectEvent_GetById(LOCALID_FROST_TARGET_WEST)->motionFlags = zero;
    ObjectEvent_GetById(LOCALID_FROST_TARGET_WEST)->priorityFlags = 2;
    Engine_ObjectEventSetAnimation(LOCALID_FROST_TARGET_WEST, 5);
    MapGrid_CopyCellAttributes(46, 17, 1, 1, 44, 17);
  }
  if (Engine_GameFlagIsSet(FLAG_EAST_TARGET_FROZEN)) {
    ObjectEvent_GetById(LOCALID_FROST_TARGET_EAST)->motionFlags = zero;
    ObjectEvent_GetById(LOCALID_FROST_TARGET_EAST)->priorityFlags = 2;
    Engine_ObjectEventSetAnimation(LOCALID_FROST_TARGET_EAST, 5);
    MapGrid_CopyCellAttributes(46, 17, 1, 1, 50, 17);
  }
  objectEvent = ObjectEvent_GetById(LOCALID_RIDING_LOG_NORTH);
  column = objectEvent->x.fixed >> 20;
  objectEvent->motionFlags = zero;
  objectEvent->priorityFlags = 2;
  MapGrid_CopyCellAttributes(52, 28, 1, 3, column, 10);
  objectEvent = ObjectEvent_GetById(LOCALID_RIDING_LOG_SOUTH);
  column = objectEvent->x.fixed >> 20;
  objectEvent->motionFlags = zero;
  objectEvent->priorityFlags = 2;
  MapGrid_CopyCellAttributes(52, 28, 1, 3, column, 13);

  // Each Mini-Maze pillar returns to the column its map-section temp flag byte
  // keeps.
  x = FieldGameFlag_GetByte(FLAG_MINI_MAZE_FIRST_PILLAR_COLUMN);
  if (x == 0) {
    x = 73;
  }
  objectEvent = ObjectEvent_GetById(LOCALID_MINI_MAZE_FIRST_PILLAR);
  objectEvent->x.fixed = (x << 20) + 0x80000;
  objectEvent->motionFlags = zero;
  objectEvent->priorityFlags = 2;
  MapGrid_CopyCellAttributes(71, 16, 1, 1, x, 16);
  x = FieldGameFlag_GetByte(FLAG_MINI_MAZE_SECOND_PILLAR_COLUMN);
  if (x == 0) {
    x = 76;
  }
  objectEvent = ObjectEvent_GetById(LOCALID_MINI_MAZE_FIRST_PILLAR + 1);
  objectEvent->x.fixed = (x << 20) + 0x80000;
  objectEvent->motionFlags = zero;
  objectEvent->priorityFlags = 2;
  MapGrid_CopyCellAttributes(71, 16, 1, 1, x, 16);
  x = FieldGameFlag_GetByte(FLAG_MINI_MAZE_THIRD_PILLAR_COLUMN);
  if (x == 0) {
    x = 79;
  }
  objectEvent = ObjectEvent_GetById(LOCALID_MINI_MAZE_FIRST_PILLAR + 2);
  objectEvent->x.fixed = (x << 20) + 0x80000;
  objectEvent->motionFlags = zero;
  objectEvent->priorityFlags = 2;
  MapGrid_CopyCellAttributes(71, 16, 1, 1, x, 16);

  ColosseumSatrageMatch_UpdateRollingLogCollision();
  // A Halted controller leaves the lifts standing; otherwise resume the task.
  Engine_ObjectEventSetAnimation(LOCALID_LIFT_OPERATOR, 10);
  if (Engine_GameFlagIsSet(FLAG_LIFTS_HALTED)) {
    for (liftId = LOCALID_COLOSSEUM_SATRAGE_MATCH_FIRST_LIFT, row = 13,
        columnOffset = 58;
         liftId <= LOCALID_COLOSSEUM_SATRAGE_MATCH_LAST_LIFT;
         liftId++, columnOffset += 2) {
      objectEvent = ObjectEvent_GetById(liftId);
      objectEvent->priorityFlags = 2;
      ObjectEvent_SetMode(objectEvent, 4);
      MapGrid_CopyCellAttributes(56, 13, 1, 1, columnOffset, row);
    }
    Engine_ObjectEventSetAnimation(LOCALID_LIFT_OPERATOR, 10);
    FieldEffect_PauseObject(LOCALID_LIFT_OPERATOR);
  } else {
    for (liftId = LOCALID_COLOSSEUM_SATRAGE_MATCH_FIRST_LIFT;
         liftId <= LOCALID_COLOSSEUM_SATRAGE_MATCH_LAST_LIFT; liftId++) {
      objectEvent = ObjectEvent_GetById(liftId);
      objectEvent->priorityFlags = 2;
      ObjectEvent_SetMode(objectEvent, 4);
      objectEvent->speed = 0x8000;
      objectEvent->acceleration = 0x3333;
    }
    FieldScheduler_AddCallback(ColosseumSatrageMatch_UpdateLifts,
                               TASK_PRIORITY_MAP + 5);
    Task_SetFlags(ColosseumSatrageMatch_UpdateLifts, 1);
  }
  Engine_ObjectEventSetAnimation(LOCALID_OPPONENT,
                                 WARRIOR_ANIMATION_FIGHTING_STANCE);
  gGameState.movementMode = LEADER_MOVEMENT_WALK;
  Colosso_ShowItemIcon(LOCALID_PRIZE_B, ITEM_COTTON_SHIRT);
  Colosso_ShowItemIcon(LOCALID_PRIZE_A, ITEM_CHAIN_MAIL);
  Engine_ObjectEventSetChildValue(LOCALID_OPPONENT, 1);
  // The race, the friends' turns, the tour, and the battle's result.
  switch (gGameState.warpDest) {
  case COLOSSEUM_SATRAGE_MATCH_RACE_FROM_LEND_A_HAND:
    Map_BuildDescriptorAndInstallTask(LOCALID_PARTY_LEADER, LOCALID_OPPONENT, 5,
                                      PIXELS(104), PIXELS(256), LOCALID_PRIZE_A,
                                      LOCALID_PRIZE_B);
    MapGrid_CopyCellsLayered(127, 0, 1, 2, 79, 6);
    Engine_ObjectEventDestroy(LOCALID_GUIDE);
    Engine_ObjectEventDestroy(LOCALID_MINI_MAZE_ATTENDANT);
    Engine_ObjectEventDestroy(LOCALID_SHIFTING_FLOOR_ATTENDANT);
    Engine_ObjectEventDestroy(LOCALID_LOG_ROLLING_ATTENDANT);
    Engine_ObjectEventDestroy(LOCALID_WALL_ATTENDANT);
    Engine_ObjectEventDestroy(LOCALID_BOARD_WALK_ATTENDANT);
    if (!Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION)) {
      Engine_AudioPlayCue(SONG_FADE_OUT_BGM);
      Colosso_ControlCharacterAlone(ISAAC);
      Colosso_PlaceFriendsAtCheeringSpots();
      ColosseumSatrageMatch_ShowRoundStartLettering(2);
    }
    ObjectEvent_LinkObjectAndSetCallback(GARET, LOCALID_PARTY_LEADER);
    ObjectEvent_LinkObjectAndSetCallback(IVAN, LOCALID_PARTY_LEADER);
    ObjectEvent_LinkObjectAndSetCallback(MIA, LOCALID_PARTY_LEADER);
    ColosseumSatrageMatch_StartOpponentPath((s32)&ResourceId_OpponentPathB);
    break;
  case COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_EAST:
    FieldScheduler_AddCallback(Colosso_RaiseTakePlaceOnPsynergy,
                               TASK_PRIORITY_MAP);
    Engine_ObjectEventDestroy(LOCALID_PRIZE_A);
    Engine_ObjectEventDestroy(LOCALID_PRIZE_B);
    if (!Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION)) {
      Colosso_PlaceFriendsAtCheeringSpots();
      Colosso_ControlCharacterAlone(GARET);
      ColosseumSatrageMatch_ShowRoundStartLettering(0);
    }
    break;
  case COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_WEST:
    if (!Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION)) {
      ColosseumSatrageMatch_IntroduceSecondFinals(LOCALID_GUIDE);
      Colosso_ClearCheeringSpots();
    }
    break;
  case COLOSSEUM_SATRAGE_MATCH_AFTER_MATCH_WON:
    ColosseumSatrageMatch_StageBattleResult(2);
    Engine_ScriptRequestWarp(COLOSSEUM_SATRAGE_MATCH_TO_COLOSSEUM_NORTH);
    break;
  case COLOSSEUM_SATRAGE_MATCH_AFTER_MATCH_LOST:
    ColosseumSatrageMatch_StageBattleResult(-2);
    Engine_ScriptRequestWarp(COLOSSEUM_SATRAGE_MATCH_TO_BABI_PALACE);
    break;
  }
  return 0;
}

/// The tour of Colosseum Final 2 (COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_WEST):
/// the guide welcomes the party to the second finals match's stages and leaves
/// them to their attendants, then Garet, Ivan and Mia walk back into Isaac.
static void ColosseumSatrageMatch_IntroduceSecondFinals(s32 attendantId) {
  s32 attendantZ;
  s32 attendantX;
  struct ObjectEvent *objectEvent;

  objectEvent = ObjectEvent_GetById(attendantId);
  attendantX = objectEvent->x.part.pixel;
  attendantZ = objectEvent->z.part.pixel;
  Engine_ScriptStart();
  ObjectEvent_SetSpeed(attendantId, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetSpeed(GARET, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetSpeed(IVAN, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetSpeed(MIA, PIXELS(1), PIXELS(1) / 2);
  Engine_ObjectEventSetPosition(LOCALID_PARTY_LEADER, (attendantX << 16),
                                ((attendantZ << 16) - 0x300000));
  Engine_ObjectEventSetPosition(GARET, (attendantX << 16) - 0x100000,
                                (attendantZ << 16) - 0x280000);
  Engine_ObjectEventSetPosition(IVAN, (attendantX << 16) + 0x100000,
                                (attendantZ << 16) - 0x280000);
  Engine_ObjectEventSetPosition(MIA, (attendantX << 16),
                                ((attendantZ << 16) - 0x200000));
  Engine_ObjectEventSetPosition(attendantId, (attendantX << 16),
                                ((attendantZ << 16) - 0x500000));
  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  objectEvent->facing = FACING_NORTH;
  Engine_CameraFollowObjectEvent(LOCALID_PARTY_LEADER, 0);
  Engine_ScriptOpenScreen();
  Engine_ScriptWaitForScreen();
  Engine_ScriptSetMessage(MsgColossoSiteSecondFinals);
  Engine_ObjectEventSetAnimationAndWait(attendantId, ANIMATION_NOD);
  Engine_ScriptShowMessage(attendantId, 0);
  Engine_ObjectEventRunRepeatedMotion(attendantId, 2);
  Engine_ScriptShowMessage(attendantId, 0);
  Engine_ObjectEventRunRepeatedMotion(attendantId, 2);
  Engine_ScriptShowMessage(attendantId, 0);
  Engine_ObjectEventRunRepeatedMotion(attendantId, 2);
  Engine_ScriptShowMessage(attendantId, 0);
  // Gather the companions into Isaac before the attendant returns to his post.
  Engine_ObjectEventSetAnimation(MIA, ANIMATION_NOD);
  Engine_ObjectEventSetAnimation(GARET, ANIMATION_NOD);
  Engine_ObjectEventSetAnimation(IVAN, ANIMATION_NOD);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  Engine_ScriptWait(6);
  Engine_ObjectEventSetAnimation(GARET, ANIMATION_WALK);
  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (objectEvent != 0) {
    Engine_ObjectEventSetDestination(GARET, objectEvent->x.part.pixel,
                                     objectEvent->z.part.pixel);
  }
  Engine_ObjectEventSetAnimation(IVAN, ANIMATION_WALK);
  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (objectEvent != 0) {
    Engine_ObjectEventSetDestination(IVAN, objectEvent->x.part.pixel,
                                     objectEvent->z.part.pixel);
  }
  Engine_ObjectEventSetAnimation(MIA, ANIMATION_WALK);
  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (objectEvent != 0) {
    Engine_ObjectEventSetDestination(MIA, objectEvent->x.part.pixel,
                                     objectEvent->z.part.pixel);
  }
  Engine_ObjectEventWalkToAndWait(attendantId, (attendantX - 16),
                                  (attendantZ - 64));
  Engine_ObjectEventSetPosition(GARET, 0, 0);
  Engine_ObjectEventSetPosition(IVAN, 0, 0);
  Engine_ObjectEventSetPosition(MIA, 0, 0);
  Engine_ObjectEventWalkToAndWait(attendantId, (attendantX - 16),
                                  (attendantZ - 16));
  Engine_ObjectEventWalkToAndWait(attendantId, attendantX, attendantZ);
  Engine_ObjectEventFaceDirection(attendantId, FACING_NORTH, 10);
  Engine_ScriptEnd();
}

/// The Mini-Maze's attendant, stage 1 of Colosseum Final 2: the first time,
/// Isaac appears and runs the maze; then the objective, and a place for a
/// friend to cheer. With a friend alone on the stages
/// (COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_EAST) he asks whether they are done
/// cheering instead.
static void EventScript_TalkToMiniMazeAttendant(s32 attendant) {
  s32 result;
  struct ObjectEvent *objectEvent;

  if (gGameState.warpDest == COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_EAST) {
    Colosso_AskDoneCheering(attendant);
  } else {
    Engine_ScriptStart();
    result = Colosso_GreetAtStage(attendant, 1);
    if (result == COLOSSO_GUIDE_DESCRIBE) {
      Engine_ScriptSetMessage(MsgColossoStageDubbedMini);
      Engine_CameraSetSpeed(0x30000, 0x6000);
      Engine_CameraMoveTo(PIXELS(1224), -1, PIXELS(184), 1);
      Engine_CameraWaitForMove();
      Engine_ScriptShowMessage(attendant, 0);
      Colosso_FadeInWarrior(LOCALID_PARTY_LEADER, 1272, 168);
      ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(3) / 2, PIXELS(3) / 4);
      Colosso_WalkObjectEventToAndStand(LOCALID_PARTY_LEADER, 1288, 184);
      Colosso_WalkObjectEventToAndStand(LOCALID_PARTY_LEADER, 1288, 216);
      Colosso_WalkObjectEventToAndStand(LOCALID_PARTY_LEADER, 1224, 216);
      Engine_ScriptShowMessage(attendant, 0);
      Colosso_WalkObjectEventToAndStand(LOCALID_PARTY_LEADER, 1224, 248);
      Colosso_WalkObjectEventToAndStand(LOCALID_PARTY_LEADER, 1192, 248);
      Engine_ScriptWait(3);
      objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
      objectEvent->velocityY = 0x40000;
      Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER,
                                     ANIMATION_LEADER_ARMS_RAISED);
      Engine_ObjectEventSetAttachedEffect(LOCALID_PARTY_LEADER,
                                          ATTACHED_EFFECT_IN_FRONT |
                                              ATTACHED_EFFECT_ONCE);
      Engine_ScriptWait(30);
      Engine_ScriptShowMessage(attendant, 0);
      Colosso_RestoreWarrior(LOCALID_PARTY_LEADER);
      Engine_CameraFollowObjectEvent(LOCALID_PARTY_LEADER, 0);
      Colosso_RepeatStageNumber(attendant, 1);
    } else if (result == COLOSSO_GUIDE_OFFER_CHEERING) {
      Engine_ScriptSetMessage(MsgColossoMainThingStage);
      Engine_ScriptShowMessage(attendant, 0);
    }
    Colosso_OfferCheeringSpot(result, attendant, 1);
    Engine_ScriptEnd();
  }
}

/// The shifting floor stage's attendant, stage 2 of Colosseum Final 2: the
/// first time, Isaac appears and rides the lifts; then, that the lifts'
/// operator will cheer for Isaac, and a place for a friend to cheer.
static void EventScript_TalkToShiftingFloorAttendant(s32 attendant) {
  s32 result;

  if (gGameState.warpDest == COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_EAST) {
    Colosso_AskDoneCheering(attendant);
  } else {
    Engine_ScriptStart();
    result = Colosso_GreetAtStage(attendant, 2);
    if (result == COLOSSO_GUIDE_DESCRIBE) {
      Engine_ScriptSetMessage(MsgColossoShiftingFloorStage);
      ColosseumSatrageMatch_ResetLifts();
      FieldCamera_SetSpeed(0x30000, 0x6000);
      FieldCamera_MoveTo(PIXELS(984), -1, PIXELS(232), 1);
      Engine_CameraWaitForMove();
      /* FAKEMATCH(wrapper): called through a cast to the s32 call the bytes
       * show, which sets r1 before r0; as a direct call, 1 of 104
       * instructions differ in tbs-en. */
      COLOSSEUM_SATRAGE_MATCH_SHOW_MESSAGE(attendant, 0);
      ColosseumSatrageMatch_StartLifts();
      Script_ShowMessage(attendant, 0);
      Colosso_FadeInWarrior(LOCALID_PARTY_LEADER, 1080, 264);
      Engine_ScriptWait(15);
      ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(3) / 2, PIXELS(3) / 4);
      Colosso_WalkObjectEventToAndStand(LOCALID_PARTY_LEADER, 1080, 216);
      Colosso_WalkObjectEventToAndStand(LOCALID_PARTY_LEADER, 1064, 216);
      ColosseumSatrageMatch_WaitForLiftCrossingPhase();
      Engine_LeaderCheckAhead();
      FieldCamera_MoveTo(-1, -1, -1, 0);
      Script_ShowMessage(attendant, 0);
      Colosso_RestoreWarrior(LOCALID_PARTY_LEADER);
      Engine_CameraFollowObjectEvent(LOCALID_PARTY_LEADER, 0);
      Colosso_RepeatStageNumber(attendant, 2);
    } else {
      if (result == COLOSSO_GUIDE_OFFER_CHEERING) {
        Engine_ScriptSetMessage(MsgColossoOperatorLiftsWillCheerFor);
        Script_ShowMessage(attendant, 0);
      }
    }
    Colosso_OfferCheeringSpot(result, attendant, 2);
    Engine_ScriptEnd();
  }
}

/// The Log-Rolling Stage's attendant, stage 3 of Colosseum Final 2: the first
/// time, Isaac appears and rides a log across; then the objective, and a place
/// for a friend to cheer.
static void EventScript_TalkToLogRollingAttendant(s32 attendant) {
  s32 result;

  if (gGameState.warpDest == COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_EAST) {
    Colosso_AskDoneCheering(attendant);
  } else {
    Engine_ScriptStart();
    result = Colosso_GreetAtStage(attendant, 3);
    if (result == COLOSSO_GUIDE_DESCRIBE) {
      Engine_ScriptSetMessage(MsgColossoLogRollingStage);
      FieldCamera_SetSpeed(0x30000, 0x6000);
      FieldCamera_MoveTo(PIXELS(752), -1, PIXELS(192), 1);
      Engine_CameraWaitForMove();
      Engine_ScriptWait(60);
      FieldCamera_SetSpeed(0x10000, 0x2000);
      FieldCamera_MoveTo(PIXELS(752), -1, PIXELS(224), 1);
      Engine_CameraWaitForMove();
      Script_ShowMessage(attendant, 0);
      Colosso_FadeInWarrior(LOCALID_PARTY_LEADER, 856, 264);
      Engine_ScriptWait(10);
      ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(3) / 2, PIXELS(3) / 4);
      Colosso_WalkObjectEventToAndStand(LOCALID_PARTY_LEADER, 856, 264);
      Colosso_WalkObjectEventToAndStand(LOCALID_PARTY_LEADER, 856, 232);
      Script_ShowMessage(attendant, 0);
      Colosso_WalkObjectEventToAndStand(LOCALID_PARTY_LEADER, 840, 232);
      Engine_ScriptWait(10);
      /* FAKEMATCH(wrapper): called through a cast to the s32 call the bytes
       * show, which sets r0 last; as a direct call, 1 of 163 instructions
       * differ in tbs-en. */
      COLOSSEUM_SATRAGE_MATCH_MOVE_OBJ_EVENT_PAIR(LOCALID_RIDING_LOG_SOUTH, -64,
                                                  0);
      FieldCamera_MoveTo(PIXELS(752), -1, PIXELS(216), 1);
      Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIMATION_STAND);
      Engine_ScriptWait(10);
      ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
      ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 760, 232);
      Engine_ScriptWait(10);
      ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 30);
      Script_ShowMessage(attendant, 0);
      Colosso_RestoreWarrior(LOCALID_PARTY_LEADER);
      Engine_CameraFollowObjectEvent(LOCALID_PARTY_LEADER, 0);
      ObjectEvent_SetPosition(LOCALID_RIDING_LOG_SOUTH, PIXELS(840),
                              PIXELS(232));
      Colosso_RepeatStageNumber(attendant, 3);
    } else if (result == COLOSSO_GUIDE_OFFER_CHEERING) {
      Engine_ScriptSetMessage(MsgColossoHereYourObjectiveRideLogs);
      Script_ShowMessage(attendant, 0);
    }
    Colosso_OfferCheeringSpot(result, attendant, 3);
    Engine_ScriptEnd();
  }
}

/// The Wall's attendant, stage 4 of Colosseum Final 2: the first time, Isaac
/// appears and climbs it; then the objective, and a place for a friend to
/// cheer.
static void EventScript_TalkToWallAttendant(s32 attendant) {
  s32 z;
  struct ObjectEvent *objectEvent;
  s32 result;

  if (gGameState.warpDest == COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_EAST) {
    Colosso_AskDoneCheering(attendant);
    return;
  }
  Engine_ScriptStart();
  result = Colosso_GreetAtStage(attendant, 4);
  if (result == COLOSSO_GUIDE_DESCRIBE) {
    Engine_ScriptSetMessage(MsgColossoFansJustCallWall);
    Engine_CameraSetSpeed(0x30000, 0x6000);
    FieldCamera_MoveTo(PIXELS(536), -1, PIXELS(240), 1);
    Engine_CameraWaitForMove();
    Engine_ScriptWait(45);
    FieldCamera_SetSpeed(0x10000, 0x2000);
    FieldCamera_MoveTo(PIXELS(536), -1, PIXELS(192), 1);
    z = 264;
    Engine_CameraWaitForMove();
    Engine_ScriptShowMessage(attendant, 0);
    Colosso_FadeInWarrior(LOCALID_PARTY_LEADER, 632, z);
    ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
    ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 616, z);
    ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 20);
    Field_ClimbUp();
    FieldCamera_SetSpeed(0x4000, 0x800);
    FieldCamera_MoveTo(PIXELS(536), -1, PIXELS(160), 1);
    ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
    Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER,
                                   ANIMATION_LEADER_CLIMB);
    objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
    Engine_ObjectSetPosition(objectEvent, objectEvent->x.fixed,
                             objectEvent->y.fixed + 0x400000,
                             objectEvent->z.fixed);
    Engine_ObjectEventWaitForMove(LOCALID_PARTY_LEADER);
    Field_ClimbUp();
    FieldCamera_MoveTo(-1, -1, -1, 0);
    Script_ShowMessage(attendant, 0);
    ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(3) / 2, PIXELS(3) / 4);
    Colosso_WalkObjectEventToAndStand(LOCALID_PARTY_LEADER, 488, 248);
    ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 20);
    FieldObjectEvent_JumpDownLedge(LOCALID_PARTY_LEADER, 6, 0);
    Engine_CameraMoveTo(PIXELS(536), -1, PIXELS(160), 1);
    Engine_ScriptShowMessage(attendant, 0);
    Colosso_RestoreWarrior(LOCALID_PARTY_LEADER);
    Engine_CameraFollowObjectEvent(LOCALID_PARTY_LEADER, 0);
    Colosso_RepeatStageNumber(attendant, 4);
  } else if (result == COLOSSO_GUIDE_OFFER_CHEERING) {
    Engine_ScriptSetMessage(MsgColossoScalingWallQuickly);
    Engine_ScriptShowMessage(attendant, 0);
  }
  Colosso_OfferCheeringSpot(result, attendant, 4);
  Engine_ScriptEnd();
}

/// Moves a demonstration obstacle to pixel coordinates while its pusher
/// follows.
static void ColosseumSatrageMatch_PushObstacleToCell(s32 obstacleId, s32 column,
                                                     s32 row) {
  s32 pusher = gGameState.leaderLocalId;
  struct ObjectEvent *pusherObjectEvent = ObjectEvent_GetById(pusher);
  struct ObjectEvent *obstacle = ObjectEvent_GetById(obstacleId);
  s32 alongX = (obstacle->x.fixed >> 20) != column / 2;
  s32 offsetX;
  s32 offsetZ;

  column <<= 16;
  row <<= 16;
  if (alongX) {
    offsetX = (column - obstacle->x.fixed) / 2;
    offsetZ = 0;
  } else {
    offsetX = 0;
    offsetZ = (row - obstacle->z.fixed) / 2;
  }
  Engine_ObjectEventSetAnimation(pusher, 8);
  Engine_ScriptWait(6);
  obstacle->speed = 0x8000;
  obstacle->acceleration = 0x3333;
  Engine_AudioPlayCue(SE_HEAVY_MOVE);
  ObjectEvent_SetMode(obstacle, 3);
  Engine_ObjectSetPosition(obstacle, column, 0, row);
  Engine_ScriptWait(6);
  Engine_ObjectEventSetAnimation(pusher, ANIMATION_WALK);
  Engine_ObjectEventSetSpeed(pusher, PIXELS(1) / 2, PIXELS(1) / 5);
  ObjectEvent_SetMode(pusherObjectEvent, 2);
  Engine_ObjectSetPosition(pusherObjectEvent,
                           pusherObjectEvent->x.fixed + offsetX, 0,
                           pusherObjectEvent->z.fixed + offsetZ);
  Object_WaitForMove(pusherObjectEvent);
  ObjectEvent_SetMode(pusherObjectEvent, 1);
  Object_WaitForMove(obstacle);
  ObjectEvent_SetMode(obstacle, 1);
  Engine_AudioPlayCue(SE_STOP_SLOT3);
  Engine_AudioPlayCue(SE_HEAVY_STOP);
  Engine_ScriptWait(15);
}

/// The Board Walk's attendant, stage 5 of Colosseum Final 2: the first time,
/// Isaac appears and rolls a log into a path; then the objective, and a place
/// for a friend to cheer.
static void EventScript_TalkToBoardWalkAttendant(s32 attendant) {
  s32 result;

  if (gGameState.warpDest == COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_EAST) {
    Colosso_AskDoneCheering(attendant);
    return;
  }
  Engine_ScriptStart();
  result = Colosso_GreetAtStage(attendant, 5);
  if (result == COLOSSO_GUIDE_DESCRIBE) {
    Engine_ScriptSetMessage(MsgColossoPlaceCalledBoard);
    Engine_CameraSetSpeed(0x20000, 0x4000);
    FieldCamera_MoveTo(PIXELS(328), -1, PIXELS(264), 1);
    Engine_CameraWaitForMove();
    Engine_ScriptWait(30);
    FieldCamera_SetSpeed(0x18000, 0x3000);
    FieldCamera_MoveTo(PIXELS(312), -1, PIXELS(176), 1);
    Engine_CameraWaitForMove();
    Engine_ScriptShowMessage(attendant, 0);
    Colosso_FadeInWarrior(LOCALID_PARTY_LEADER, 408, 264);
    ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(3) / 2, PIXELS(3) / 4);
    Colosso_WalkObjectEventToAndStand(LOCALID_PARTY_LEADER, 408, 216);
    ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_WEST, 10);
    Engine_ScriptShowMessage(attendant, 0);
    ColosseumSatrageMatch_PushObstacleToCell(LOCALID_ROLLING_LOG_EAST, 360,
                                             208);
    Engine_ObjectEventShowEmote(LOCALID_PARTY_LEADER,
                                EMOTE_IN_FRONT | EMOTE_SMILE, 45);
    ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(3) / 2, PIXELS(3) / 4);
    Colosso_WalkObjectEventToAndStand(LOCALID_PARTY_LEADER, 376, 216);
    Colosso_WalkObjectEventToAndStand(LOCALID_PARTY_LEADER, 376, 248);
    Colosso_WalkObjectEventToAndStand(LOCALID_PARTY_LEADER, 312, 248);
    Engine_ScriptShowMessage(attendant, 0);
    Colosso_RestoreWarrior(LOCALID_PARTY_LEADER);
    Engine_CameraFollowObjectEvent(LOCALID_PARTY_LEADER, 0);
    ObjectEvent_SetPosition(LOCALID_ROLLING_LOG_EAST, PIXELS(392), PIXELS(208));
    Colosso_RepeatStageNumber(attendant, 5);
  } else if (result == COLOSSO_GUIDE_OFFER_CHEERING) {
    Engine_ScriptSetMessage(MsgColossoFocusRollingLogs);
    Engine_ScriptShowMessage(attendant, 0);
  }
  Colosso_OfferCheeringSpot(result, attendant, 5);
  Engine_ScriptEnd();
}

#include "common/colosso_solo.h"
#include "common/item_equip.h"
#include "common/colosso_task.h"

/// Chooses the lettering animation script and resets its task state.
static void ColosseumSatrageMatch_StartLetteringAnimation(u32 mode,
                                                          u32 tileVariant) {
  s32 script;

  gColosso_LetteringMode = mode;
  gColosso_LetteringParameter = (tileVariant << 4);

  {
    s32 priority = TASK_PRIORITY_MAP;
    FieldScheduler_AddCallback(Colosso_UpdateLettering, priority);
  }

  script = (s32)gColosseumSatrageMatch_LetteringTitle;
  if (mode == COLOSSO_SHAPE_DIGIT) {
    script = (s32)gColosseumSatrageMatch_LetteringDigit;
  }
  if (mode == COLOSSO_SHAPE_GO) {
    script = (s32)gColosseumSatrageMatch_LetteringGo;
  }
  if (mode == COLOSSO_SHAPE_RESULT) {
    if (tileVariant != 0) {
      script = (s32)gColosseumSatrageMatch_LetteringLose;
    } else {
      script = (s32)gColosseumSatrageMatch_LetteringWon;
    }
  }

  gColosso_LetteringTimer = 0;
  gColosso_LetteringScript = (s16 *)script;
  gColosso_ModeMoveTarget = 0;
  gColosso_ModeMoveDuration = 0;
  gColosso_LetteringPosition = 0;
}

/// The lettering a visit opens with: `Lend a Hand` for the friends' turns
/// (mode 0), or for the race `Match 2`, then 3, 2, 1 and `Go!`.
static void ColosseumSatrageMatch_ShowRoundStartLettering(s32 mode) {
  if (mode == 0) {
    Engine_ScriptStart();
    Engine_ScriptOpenScreen();
    Engine_ScriptWaitForScreen();
    Engine_ScriptWait(30);
    Engine_AudioPlayCue(MUS_LEVEL_UP);
    Colosso_LoadLettering(COLOSSO_PICTURE_LEND_A_HAND);
    ColosseumSatrageMatch_StartLetteringAnimation(COLOSSO_SHAPE_TITLE, 0);
    Engine_ScriptWait(120);
    Engine_ScriptEnd();
    return;
  }

  Engine_AudioPlayCue(SE_247);
  Engine_ScriptStart();
  Engine_ScriptOpenScreen();
  Engine_ScriptWaitForScreen();
  {
    s16 *openingScript = gColosseumSatrageMatch_LetteringTitle;
    *(s16 *)((u8 *)openingScript + 30) = (mode * 60);
  }
  Engine_ScriptWait(30);
  Engine_AudioPlayCue(mode + 0x5a);
  Colosso_LoadLettering(mode);
  ColosseumSatrageMatch_StartLetteringAnimation(COLOSSO_SHAPE_TITLE, 0);
  Engine_ScriptWait(120);

  // Finish the opening fanfare before the final lettering pulses and start
  // signal.
  while (Sound_GetBgmRestoreDelay() != 0) {
    Engine_TaskWait(1);
  }

  Engine_AudioPlayCue(SONG_FADE_OUT_ACTION_SE);
  Colosso_LoadLettering(COLOSSO_PICTURE_COUNT_3_2);
  ColosseumSatrageMatch_StartLetteringAnimation(COLOSSO_SHAPE_DIGIT, 0);
  Engine_AudioPlayCue(SE_COLOSSO_COUNTDOWN);
  Engine_ScriptWait(60);
  ColosseumSatrageMatch_StartLetteringAnimation(COLOSSO_SHAPE_DIGIT, 1);
  Engine_AudioPlayCue(SE_COLOSSO_COUNTDOWN);
  Engine_ScriptWait(60);
  Colosso_LoadLettering(COLOSSO_PICTURE_COUNT_1);
  ColosseumSatrageMatch_StartLetteringAnimation(COLOSSO_SHAPE_DIGIT, 0);
  Engine_AudioPlayCue(SE_COLOSSO_COUNTDOWN);
  Engine_ScriptWait(60);
  Colosso_LoadLettering(COLOSSO_PICTURE_GO);
  ColosseumSatrageMatch_StartLetteringAnimation(COLOSSO_SHAPE_GO, 0);
  Engine_AudioPlayCue(SE_COLOSSO_GO);
  Map_PlayMusic();
  Engine_ScriptEnd();
  Engine_GameFlagSet(FLAG_IN_TOWN);
}

/// The result: `Won` or `Lose` in large lettering, and the loser's reaction.
static void ColosseumSatrageMatch_ShowBattleResultLettering(s32 result) {

  s32 reactingLocalId;

  Engine_AudioPlayCue(SE_247);
  Engine_ScriptOpenScreen();
  Engine_ScriptWaitForScreen();
  gColosseumSatrageMatch_LetteringWon[13] = result * 60;
  STORE_CONST(gColosseumSatrageMatch_LetteringLose[13]) =
      (result < 0 ? -result : result) * 60;
  if (result < 0) {
    Engine_ScriptWait(30);
    Engine_AudioPlayCue(MUS_086);
    Colosso_LoadLettering(COLOSSO_PICTURE_WON_LOSE_FOR_LOSS);
    /* FAKEMATCH(wrapper): called through a cast to the s32 call the bytes
     * show, which sets r1 before r0; as a direct call, 1 of 72 instructions
     * differ in tbs-en. */
    COLOSSEUM_SATRAGE_MATCH_START_MODE_TASK(3, 1);
    Engine_ScriptWait(-result * 60 + 60);
    reactingLocalId = 0;
  } else {
    Engine_ScriptWait(30);
    Engine_AudioPlayCue(result + 90);
    Colosso_LoadLettering(COLOSSO_PICTURE_WON_LOSE);
    /* FAKEMATCH(wrapper): the same cast as the first arm, which sets r1 before
     * r0; as a direct call, 1 of 72 instructions differ in tbs-en. */
    COLOSSEUM_SATRAGE_MATCH_START_MODE_TASK(3, 0);
    Engine_ScriptWait(result * 60 + 60);
    reactingLocalId = 8;
  }
  Engine_ObjectEventShowEmote(reactingLocalId, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
                              0);
  while (Sound_GetBgmRestoreDelay() != 0) {
    Engine_TaskWait(1);
  }
  Engine_AudioPlayCue(MUS_SILENCE);
  Engine_ScriptWait(30);
  Engine_AudioPlayCue(SONG_FADE_OUT_ACTION_SE);
  Engine_ScriptCloseScreen();
  Engine_ScriptWaitForScreen();
}

#include "common/colosso_marker.h"
#include "common/colosso_item_icon.h"

/// Unused: writes eight uppercase hexadecimal digits followed by a terminator;
/// nothing calls it.
static UNUSED void String_WriteU32AsHex(u8 *destination, u32 value) {
  s32 digit;

  destination += 8;
  *destination = 0;
  destination--;
  for (digit = 7; digit >= 0; digit--) {
    *destination = gHexDigits[value & 15];
    value >>= 4;
    destination--;
  }
}

/// Retains the empty hook called before the finish sequence.
static void MapLocalState_EmptyHook(void) {}

/// Unused: marks the opponent's path recorder finished
/// (PATH_RECORDER_FINISHED); nothing calls it.
static UNUSED void ColosseumSatrageMatch_MarkOpponentFinished(void) {
  ((struct PathRecorder *)gMapLocalState)->mode = PATH_RECORDER_FINISHED;
}

static void ColosseumSatrageMatch_WaitForOpponentFinish(void) {
  s16 *mode = &((struct PathRecorder *)gMapLocalState)->mode;

  while (*mode != 9) {
    Engine_TaskWait(1);
  }
}

/// Occasionally spawns a falling particle around the linked finish effect.
static void
ColosseumSatrageMatch_SpawnRandomMapEffect(union FieldObject *fieldObject) {
  struct ObjectEvent *source = (struct ObjectEvent *)fieldObject;
  s32 position[3];
  u32 chance;

  if (source->velocityY >= -255 && source->velocityY <= 255) {
    source->motionFlags = 0;
  }
  chance = Engine_RandomNext();
  if (chance * 100 >> 16 <= 9) {
    struct ObjectEvent *particle;
    s32 radius;
    s32 angle;

    SET_VECTOR(position, source->x.fixed, source->y.fixed, source->z.fixed);
    radius = Engine_RandomNext();
    angle = Engine_RandomNext();
    Vector_AddPolarOffset(radius << 4, angle, position);
    {
      s32 x = position[0];
      s32 y = position[1];
      s32 z = position[2];

      particle = Engine_ObjectCreate(SPRITE_SPARKLE, x, y, z);
    }
    if (particle != 0) {
      particle->motionFlags = 0;
      Engine_ObjectEventSetSpriteFlags(particle, 0);
      Engine_ObjectSetScript(particle, gColosseumSatrageMatch_SparkScript);
      ObjectEvent_SetMode(particle, 1);
      ObjectEvent_SetMode(particle, 0);
    }
  }
}

/// Raises the linked finish effect, starts its script and clears the link.
static s32 ColosseumSatrageMatch_RaiseLinkedMapEffect(
    struct ObjectScriptContext *interpreter) {
  struct ObjectEvent *source = (struct ObjectEvent *)interpreter;
  struct ObjectEvent *effect = ObjectEvent_GetById(source->timer);

  Engine_ObjectSetPosition(effect, source->x.fixed,
                           source->y.fixed + PIXELS(36), source->z.fixed);
  effect->motionFlags = 0;
  Engine_ObjectSetScript(effect, gColosseumSatrageMatch_RaiseScript);
  Engine_AudioPlayCue(MUS_TREASURE_FOUND);
  source->timer = 0;
  return 0;
}

/// Approaches the finish item on the unused side and shows the item message.
static s32 ColosseumSatrageMatch_ApproachFinishAndTakeItem(s32 firstItem,
                                                           s32 secondItem) {
  struct ColossoGaugeWork *gauge = HEAP_SLOT_BLOCK(HEAP_SLOT_COLOSSO_RACE);
  u8 *gameState;
  struct ObjectEvent *warrior;
  s32 finishAlreadyReached;
  s32 x;
  s32 z;
  u16 *itemLocalId;
  s16 *timer;

  finishAlreadyReached = Engine_GameFlagIsSet(FLAG_ARENA_REACHED);

  gameState = (u8 *)&gGameState;
  warrior = ObjectEvent_GetById(((struct GameState *)gameState)->leaderLocalId);

  if (gauge->finishX < warrior->x.fixed) {
    x = gauge->finishX + PIXELS(12);
  } else {
    x = gauge->finishX - PIXELS(12);
  }

  if (finishAlreadyReached != 0) {
    z = gauge->finishZ + PIXELS(16);
    itemLocalId = &gauge->finishItemLocalIdB;
  } else {
    z = gauge->finishZ - PIXELS(16);
    itemLocalId = &gauge->finishItemLocalIdA;
  }

  timer = (s16 *)&warrior->timer;
  *timer = *itemLocalId;
  warrior->acceleration = PIXELS(1) / 4;
  warrior->speed = PIXELS(1);

  Engine_ObjectSetPosition(warrior, x, 0, z);
  Engine_GameFlagSet(FLAG_ARENA_REACHED);
  Engine_ObjectSetScript(warrior, gColosseumSatrageMatch_ApproachScript);

  while (*timer != 0) {
    Engine_TaskWait(1);
  }

  // Assign the item on the side this warrior reached first.
  if (finishAlreadyReached == 0) {
    /* FAKEMATCH(wrapper): called through a cast to the s32 call the bytes
     * show, which sets r1 before r0; as a direct call, 1 of 110 instructions
     * differ in tbs-en. */
    COLOSSEUM_SATRAGE_MATCH_APPLY_MATCHING_SLOT_VALUE(0, firstItem);
    MessageArgument_Push(firstItem, 2);
  } else {
    /* FAKEMATCH(wrapper): the same cast as the first arm, which sets r1 before
     * r0; as a direct call, 1 of 110 instructions differ in tbs-en. */
    COLOSSEUM_SATRAGE_MATCH_APPLY_MATCHING_SLOT_VALUE(0, secondItem);
    MessageArgument_Push(secondItem, 2);
  }

  gameState = (u8 *)&gGameState;
  MessageArgument_Push(((struct GameState *)gameState)->leaderLocalId, 1);
  Engine_MessageShowCentered(MsgColossoIsaacGotItem, 3);
  ObjectDispatch_WaitForValue16(warrior);

  return finishAlreadyReached;
}

#include "common/colosso_path_opponent.h"

/// Allocates the race work, records both warriors and installs the gauge
/// task.
void Map_BuildDescriptorAndInstallTask(s32 leaderId, s32 opponentId,
                                       s32 segments, s32 finishX, s32 finishZ,
                                       s32 firstItem, s32 secondItem) {
  struct ColossoRaceWork *work;
  struct ObjectEvent *leader;
  struct ObjectEvent *opponent;
  void *graphics;
  s32 vramSlot;

  work = AllocHeapSlotEwram(HEAP_SLOT_COLOSSO_RACE,
                            sizeof(struct ColossoRaceWork));
  graphics = AllocEwram(512);

  work->gauge.playerLocalId = leaderId;
  work->gauge.opponentLocalId = opponentId;
  work->gauge.finishItemLocalIdA = firstItem;
  work->gauge.finishItemLocalIdB = secondItem;
  work->gauge.segments = segments;
  work->gauge.finishX = finishX;
  work->gauge.finishZ = finishZ;

  leader = ObjectEvent_GetById(leaderId);
  opponent = ObjectEvent_GetById(opponentId);

  // Mirror the recorded opponent start when this is a fresh arrival at the
  // race.
  if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
    opponent->x.fixed = (finishX << 1) - leader->x.fixed;
    opponent->z.fixed = leader->z.fixed;
  }

  work->gauge.level = 0;
  work->gauge.hold = 0;

  // Upload the gauge graphics and keep only its VRAM slot after freeing
  // scratch.
  FieldResource_DecodeType01(gColosso_GaugeGraphics, graphics);

  vramSlot = VramCache_FindFreeEntry();
  work->gauge.vramBlock = vramSlot;
  Engine_VramLoad((s16)vramSlot, 512, graphics);

  FieldScheduler_AddCallback(Colosso_DrawGauge, TASK_PRIORITY_MAP - 10);

  Runtime_BumpFree(graphics);
}

/// Decodes the opponent's recorded path and starts its playback task.
static void ColosseumSatrageMatch_StartOpponentPath(s32 pathResource) {
  struct ColossoRaceWork *work = HEAP_SLOT_BLOCK(HEAP_SLOT_COLOSSO_RACE);
  struct PathRecorder *recorder = (struct PathRecorder *)gMapLocalState;

  FieldResource_DecodeType01(Resource_GetTableEntry(pathResource), work->path);
  if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
    recorder->mode = PATH_RECORDER_PLAY;
    recorder->mirror = 1;
    recorder->localId = work->gauge.opponentLocalId;
    recorder->still = 0;
    recorder->pathIndex = 0;
  }
  FieldScheduler_AddCallback(Colosso_UpdatePathOpponent, TASK_PRIORITY_MAP + 5);
}

#include "common/colosso_map_state.h"
#include "common/object_findslot.h"

/// Pushes the adjacent obstacle only when the destination and space above are
/// clear.
static void ColosseumSatrageMatch_PushAdjacentObstacle(void) {

  struct ObjectEvent *pusher;
  struct ObjectEvent *obstacle;
  struct ObjectEvent *blocker;
  u32 packedOffset;
  u32 direction;
  struct FieldPosition destination;
  s32 zero;
  s32 localId;

  localId = gGameState.leaderLocalId;
  pusher = ObjectEvent_GetById(localId);

  direction = pusher->facing >> 12;

  packedOffset = gColosseumSatrageMatch_DirectionSteps[direction];
  destination.x = pusher->x.fixed + (packedOffset & 0xffff0000);
  destination.y = pusher->y.fixed;
  packedOffset <<= 16;
  destination.z = pusher->z.fixed + packedOffset;

  obstacle = MapData_FindSlotAtPosition(&destination, pusher);
  if (obstacle == 0) {
    return;
  }

  // Require a free destination cell and free space immediately above the
  // pillar.
  packedOffset = gColosseumSatrageMatch_DirectionSteps[direction];
  destination.x = obstacle->x.fixed + (packedOffset & 0xffff0000);
  destination.y = obstacle->y.fixed;
  packedOffset <<= 16;
  destination.z = obstacle->z.fixed + packedOffset;

  blocker = MapData_FindSlotAtPosition(&destination, obstacle);
  if (blocker != 0 && (blocker->collisionFlags & 1) != 0) {
    return;
  }

  destination.x = obstacle->x.fixed;
  destination.y = obstacle->y.fixed + 0x100000;
  destination.z = obstacle->z.fixed;

  blocker = MapData_FindSlotAtPosition(&destination, obstacle);
  if (blocker != 0 && (blocker->collisionFlags & 1) != 0) {
    return;
  }

  obstacle->layer = 2;
  zero = 0;

  packedOffset = gColosseumSatrageMatch_DirectionSteps[direction];
  destination.x = obstacle->x.fixed + (packedOffset & 0xffff0000);
  destination.y = obstacle->y.fixed;
  packedOffset <<= 16;
  destination.z = obstacle->z.fixed + packedOffset;

  if (FieldObject_CheckMovementCollision(obstacle, &destination) > 0) {
    return;
  }

  // Start the push only after all occupancy and terrain checks have passed.
  ObjectEvent_SetMode(pusher, 8);
  Engine_TaskWait(15);

  obstacle->speed = 0x3333;
  obstacle->acceleration = 0x3333;
  Engine_ObjectSetPosition(obstacle, destination.x, destination.y,
                           destination.z);

  pusher->speed = 0x3333;
  pusher->acceleration = 0x3333;
  Engine_ObjectSetPosition(pusher, destination.x, destination.y, destination.z);

  Engine_AudioPlayCue(SE_238);
  Object_WaitForMove(obstacle);
  Engine_AudioPlayCue(SE_STOP_SLOT3);

  obstacle->x.fixed = destination.x;
  obstacle->z.fixed = destination.z;
  obstacle->velocityX = zero;
  obstacle->velocityZ = zero;

  ObjectEvent_SetMode(pusher, 1);
}

#include "common/colosso_occupant.h"

const u8 gColosseumSatrageMatch_RollLogScript[] = {2, 2, 3, 3};

// clang-format off
const u8 gColosso_LetteringPaletteOffsets[] = {
    0, 32, 32, 32, 96, 64, 64,
    COLOSSEUM_SATRAGE_MATCH_ALTERNATE_PORTRAIT_PALETTE_OFFSET, 128, 0,
};

const s16 gColosseumSatrageMatch_LetteringDigit[] = {
    0x1000, 512, 1, 0x2000, 0, 1, 0x1000, 256,
    6, 0x2000, 16, 6, 0x7fff, 30, 0x2000, 0,
    30, 0x7fff, 30, -1,
};

const s16 gColosseumSatrageMatch_LetteringLose[] = {
    0x1000, 128, 1, 0x2000, 0, 1, 0x1000, 256,
    6, 0x2000, 16, 6, 0x7fff, 60, 0x1000, 384,
    6, 0x7fff, 6, 0x1000, 240, 6, 0x7fff, 6,
    0x1000, 368, 6, 0x7fff, 6, 0x1000, 224, 6,
    0x7fff, 6, 0x2000, 0, 30, 0x1000, 352, 6,
    0x7fff, 6, 0x1000, 208, 6, 0x7fff, 6, 0x1000,
    336, 6, 0x7fff, 6, 0x1000, 192, 6, 0x7fff,
    6, 0x1000, 320, 6, 0x7fff, 6, -1,
};

char gHexDigits[20] = "0123456789ABCDEF";

s32 gColosseumSatrageMatch_SparkScript[] = {
    SC_ADD(SW_Y, -0x4000),
    SC_WAIT(1),
    SC_LOOP(60, SC_TOP),
    SC_FREE,
};

s32 gColosseumSatrageMatch_RaiseScript[] = {
    SC_SET(SW_HOOK, (s32)ColosseumSatrageMatch_SpawnRandomMapEffect),
    SC_LABEL(1),
    SC_WAIT(10),
    SC_SET(SW_SCALE_X, 0x16000),
    SC_SET(SW_SCALE_Y, 0x16000),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 0x18000),
    SC_SET(SW_SCALE_Y, 0x18000),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 0x12000),
    SC_SET(SW_SCALE_Y, 0x12000),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 0x10000),
    SC_SET(SW_SCALE_Y, 0x10000),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 0x16000),
    SC_SET(SW_SCALE_Y, 0x16000),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 0x18000),
    SC_SET(SW_SCALE_Y, 0x18000),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 0x12000),
    SC_SET(SW_SCALE_Y, 0x12000),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 0x10000),
    SC_SET(SW_SCALE_Y, 0x10000),
    SC_WAIT(2),
    SC_WAIT(50),
    SC_LOOP(2, SC_TO(1)),
    SC_POSITION(0, 0, 0),
    SC_SET(SW_HOOK, 0),
    SC_END,
};

#define DIRECTION_STEP(x, z) ((u32)(x) << 16 | (u16)(z))

u32 gColosseumSatrageMatch_DirectionSteps[] = {
    DIRECTION_STEP(16, 0),
    DIRECTION_STEP(16, 0),
    DIRECTION_STEP(16, 0),
    DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),
    DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16),
    DIRECTION_STEP(16, 0),
};

struct MapWarpDest gColosseumSatrageMatchWarpDests[] = {
    { COLOSSEUM_SATRAGE_MATCH_RACE_FROM_LEND_A_HAND, CONDITION_ALWAYS, 1272, 0, 168, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_EAST, CONDITION_ALWAYS, 1224, 0, 488, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { 10, CONDITION_ALWAYS, 384, 0, 264, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { MAP_TABLE_END },
};

u32 gColosseumSatrageMatchWarps[] = {
    MAP_WARPS(ColosseumSatrageMatch),
    MAP_WARP(COLOSSEUM_SATRAGE_MATCH_LEND_A_HAND_TO_RACE, ColosseumSatrageMatch, COLOSSEUM_SATRAGE_MATCH_RACE_FROM_LEND_A_HAND),
    MAP_WARP(COLOSSEUM_SATRAGE_MATCH_TO_COLOSSEUM_SOUTH, Colosseum, COLOSSEUM_FROM_COLOSSEUM_SATRAGE_MATCH_SOUTH),
    MAP_WARP(COLOSSEUM_SATRAGE_MATCH_TO_COLOSSEUM_NORTH, Colosseum, COLOSSEUM_FROM_COLOSSEUM_SATRAGE_MATCH_NORTH),
    MAP_WARP(COLOSSEUM_SATRAGE_MATCH_TO_BABI_PALACE, BabiPalace, BABI_PALACE_FROM_COLOSSEUM_AZART_MATCH),
    MAP_WARPS_END,
};

struct ObjectEventTemplate gColosseumSatrageMatchObjectEventTemplates[] = {
    { 0x0, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1272), 0, PIXELS(168), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0 },
    { IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0 },
    { MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0 },
    { SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(264), 0, PIXELS(168), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(376), 0, PIXELS(168), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(568), 0, PIXELS(264), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1272), 0, PIXELS(264), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1224), 0, PIXELS(264), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1176), 0, PIXELS(264), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7,
        PIXELS(264), 0, PIXELS(208), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7,
        PIXELS(392), 0, PIXELS(208), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { 0x103, CONDITION_ALWAYS, 7,
        PIXELS(360), 0, PIXELS(264), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(520), 0, PIXELS(258), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(712), 0, PIXELS(280), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(808), 0, PIXELS(280), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { 0x122, CONDITION_ALWAYS, 7,
        PIXELS(936), 0, PIXELS(216), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { 0x122, CONDITION_ALWAYS, 7,
        PIXELS(968), 0, PIXELS(216), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { 0x122, CONDITION_ALWAYS, 7,
        PIXELS(1000), 0, PIXELS(216), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { 0x122, CONDITION_ALWAYS, 7,
        PIXELS(1032), 0, PIXELS(216), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { 0x122, CONDITION_ALWAYS, 7,
        PIXELS(248), 0, PIXELS(264), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { 0x122, CONDITION_ALWAYS, 7,
        PIXELS(264), 0, PIXELS(264), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { 0x122, CONDITION_ALWAYS, 7,
        PIXELS(296), 0, PIXELS(264), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { 0x122, CONDITION_ALWAYS, 7,
        PIXELS(312), 0, PIXELS(264), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { 0x103, CONDITION_ALWAYS, 7,
        PIXELS(360), 0, PIXELS(264), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1072), 0, PIXELS(280), FACING_WEST, TALK_FACING_KEEP, 0 },
    { SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7,
        PIXELS(840), 0, PIXELS(184), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7,
        PIXELS(840), 0, PIXELS(232), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1224), 0, PIXELS(488), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(1208), 0, PIXELS(376), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(984), 0, PIXELS(376), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(760), 0, PIXELS(376), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(536), 0, PIXELS(376), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(312), 0, PIXELS(376), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(104), 0, PIXELS(240), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(104), 0, PIXELS(272), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

s32 gColosseumSatrageMatch_LiftPhase = 0;

s32 gColosseumSatrageMatch_LiftTimer = 0;

struct MapEvent gColosseumSatrageMatchEvents[] = {
    { COORD_EVENT_WALKING_INTO(FACING_SOUTH), 30, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_JumpDownLedge) },
    { COORD_EVENT_WALKING_INTO(FACING_WEST), 30, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_CheckAhead) },
    { COORD_EVENT_WALKING_INTO(FACING_WEST), 28, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_RollEastLogWest) },
    { COORD_EVENT_WALKING_INTO(FACING_EAST), 27, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_RollEastLogEast) },
    { COORD_EVENT_WALKING_INTO(FACING_WEST), 24, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_RollWestLogWest) },
    { COORD_EVENT_WALKING_INTO(FACING_EAST), 25, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_RollWestLogEast) },
    { COORD_EVENT_WALKING, 22, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_RideLog) },
    { COORD_EVENT_WALKING, 21, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_PushPillar) },
    { COORD_EVENT_WALKING, 20, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_PushPillarAndUpdateCells) },
    { COORD_EVENT_WALKING, 26, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_PushPillarAndUpdateCells) },
    { MAP_EVENT_COORD, 50, FLAG_MAP_TEMP_10, EVENT_SCRIPT(EventScript_ReachArena) },
    { BG_EVENT(BG_EVENT_CHEST), 101, FLAG_MAPSEC_TEMP_38, FIND_ITEM(ITEM_NUT) },
    { BG_EVENT(BG_EVENT_CHEST) | MAP_EVENT_FLAG_FACING | FACING_WEST, 102, FLAG_MAPSEC_TEMP_39, FIND_ITEM(ITEM_SMOKE_BOMB) },
    { BG_EVENT(BG_EVENT_CHEST), 104, FLAG_MAPSEC_TEMP_3B, FIND_ITEM(ITEM_NUT) },
    { PSYNERGY_PHASE_ON_OBJ_EVENT(ABILITY_FROST, PSYNERGY_PHASE_EFFECT_ACTS), LOCALID_FROST_TARGET_WEST, FLAG_WEST_TARGET_FROZEN, EVENT_SCRIPT(EventScript_FreezeWestTarget) },
    { PSYNERGY_PHASE_ON_OBJ_EVENT(ABILITY_FROST, PSYNERGY_PHASE_EFFECT_ACTS), LOCALID_FROST_TARGET_EAST, FLAG_EAST_TARGET_FROZEN, EVENT_SCRIPT(EventScript_FreezeEastTarget) },
    { PSYNERGY_PHASE_ON_OBJ_EVENT(ABILITY_FORCE, PSYNERGY_PHASE_BEFORE_CAST), LOCALID_FORCE_TARGET, FLAG_OBSTACLE_KNOCKED_DOWN, EVENT_SCRIPT(EventScript_PrepareForce) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_FORCE), LOCALID_FORCE_TARGET, FLAG_OBSTACLE_KNOCKED_DOWN, EVENT_SCRIPT(EventScript_KnockDownObstacle) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_GROWTH), LOCALID_GROWTH_PLANT, FLAG_PLANT_GROWN, EVENT_SCRIPT(EventScript_GrowPlant) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_BOARD_WALK_WEST_PILLAR, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_IgnoreMove) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_BOARD_WALK_EAST_PILLAR, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_IgnoreMove) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_WALL_PILLAR, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_UpdateMovedPillarCell) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_MINI_MAZE_FIRST_PILLAR, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SaveMovedPillarColumns) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_MINI_MAZE_FIRST_PILLAR + 1, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SaveMovedPillarColumns) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_MINI_MAZE_FIRST_PILLAR + 2, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SaveMovedPillarColumns) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_HALT), LOCALID_LIFT_OPERATOR, FLAG_LIFTS_HALTED, EVENT_SCRIPT(EventScript_HaltLifts) },
    { MAP_EVENT_TALK, LOCALID_GUIDE, CONDITION_ALWAYS, EVENT_SCRIPT(Colosso_AskReadyToRace) },
    { MAP_EVENT_TALK, LOCALID_MINI_MAZE_ATTENDANT, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToMiniMazeAttendant) },
    { MAP_EVENT_TALK, LOCALID_SHIFTING_FLOOR_ATTENDANT, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToShiftingFloorAttendant) },
    { MAP_EVENT_TALK, LOCALID_LOG_ROLLING_ATTENDANT, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToLogRollingAttendant) },
    { MAP_EVENT_TALK, LOCALID_WALL_ATTENDANT, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToWallAttendant) },
    { MAP_EVENT_TALK, LOCALID_BOARD_WALK_ATTENDANT, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToBoardWalkAttendant) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_GUIDE, CONDITION_ALWAYS, MSGNO(MsgColossoRiverThisYearsFinalsAreIncredibleThis) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_MINI_MAZE_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoWallICantBelieveAnybodyEverGets) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SHIFTING_FLOOR_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoWallHowDoTheyMakeThatFloor) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_LOG_ROLLING_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoWallImAmazedPeopleCanActuallyCross) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WALL_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoWallBoyThatBitOverThereMust) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BOARD_WALK_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoWallIHeardBabiGotTheLog) },
    { MAP_EVENT_RAISED, 99, CONDITION_ALWAYS, EVENT_SCRIPT(Colosso_TakePlaceForMatch) },
    { MAP_EVENTS_END },
};

s16 gColosso_LetteringSlot = -1;

s16 gColosseumSatrageMatch_LetteringTitle[] = {
    0x4000, -188, 2048, 0x1000, COLOSSEUM_SATRAGE_MATCH_INTRO_SCALE, 1, 0x2000, 16,
    1, 0x7fff, 14, 0x3000, 0, 20, 0x7fff, 60,
    0x3000, 2048, 20, 0x7fff, 60, -1,
};

s16 gColosseumSatrageMatch_LetteringGo[] = {
    0x1000, COLOSSEUM_SATRAGE_MATCH_PULSE_FIRST_SCALE, 1, 0x2000, 0, 1, 0x1000, COLOSSEUM_SATRAGE_MATCH_PULSE_LOW_SCALE,
    6, 0x2000, 16, 6, 0x7fff, 6, 0x1000, COLOSSEUM_SATRAGE_MATCH_PULSE_HIGH_SCALE,
    6, 0x7fff, 6, 0x1000, COLOSSEUM_SATRAGE_MATCH_PULSE_LOW_SCALE, 6, 0x7fff, 6,
    0x1000, COLOSSEUM_SATRAGE_MATCH_PULSE_HIGH_SCALE, 6, 0x7fff, 6, 0x1000, COLOSSEUM_SATRAGE_MATCH_PULSE_LOW_SCALE, 6,
    0x7fff, 6, 0x2000, 0, 30, 0x1000, COLOSSEUM_SATRAGE_MATCH_PULSE_HIGH_SCALE, 6,
    0x7fff, 6, 0x1000, COLOSSEUM_SATRAGE_MATCH_PULSE_LOW_SCALE, 6, 0x7fff, 6, 0x1000,
    COLOSSEUM_SATRAGE_MATCH_PULSE_HIGH_SCALE, 6, 0x7fff, 6, 0x1000, COLOSSEUM_SATRAGE_MATCH_PULSE_LOW_SCALE, 6, 0x7fff,
    6, 0x1000, COLOSSEUM_SATRAGE_MATCH_PULSE_HIGH_SCALE, 6, 0x7fff, 6, -1,

};

s16 gColosseumSatrageMatch_LetteringWon[] = {
    0x1000, 128, 1, 0x2000, 0, 1, 0x1000, 256,
    6, 0x2000, 16, 6, 0x7fff, 60, 0x1000, 384,
    6, 0x7fff, 6, 0x1000, 272, 6, 0x7fff, 6,
    0x1000, 400, 6, 0x7fff, 6, 0x1000, 288, 6,
    0x7fff, 6, 0x2000, 0, 30, 0x1000, 416, 6,
    0x7fff, 6, 0x1000, 304, 6, 0x7fff, 6, 0x1000,
    432, 6, 0x7fff, 6, 0x1000, 320, 6, 0x7fff,
    6, 0x1000, 448, 6, 0x7fff, 6, -1,
};
// clang-format on

s16 gColosso_MarkerSlot = -1;

s32 gColosso_OpponentFinishScript[] = {
    SC_ANIMATION(2),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(6),
    SC_CALL(ColosseumSatrageMatch_RaiseLinkedMapEffect),
    SC_WAIT(3),
    SC_ANIMATION(10),
    SC_WAIT(150),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_ANIMATION(1),
    SC_END,
};

s32 gColosseumSatrageMatch_ApproachScript[] = {
    SC_ANIMATION(2),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(6),
    SC_CALL(ColosseumSatrageMatch_RaiseLinkedMapEffect),
    SC_WAIT(3),
    SC_ANIMATION(28),
    SC_WAIT(150),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_ANIMATION(1),
    SC_END,
};
