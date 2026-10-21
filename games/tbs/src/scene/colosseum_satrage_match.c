/// @file
/// Colosseum Final 2 (SceneId_ColosseumSatrageMatch), the site of the second
/// finals match, where Isaac, having won the first, races Satrage, the
/// fourth-ranked warrior, through five stages to the arena and fights him
/// there.
///
/// It works as Colosseum Final 1 does (colosseum_azart_match.c), and is reached
/// by the Colosseum's west passage. The stages run from east to west, through
/// the Mini-Maze, the shifting floor stage, the Log-Rolling Stage, The Wall and
/// the Board Walk. Isaac comes here three times, each at its own entrance, and
/// ColosseumSatrageMatch_Initialize restores the stages from the group's flags
/// (FLAG_PLACE_TEMP_30 to FLAG_PLACE_TEMP_35, and the Mini-Maze pillars'
/// columns in the bytes at FLAG_PLACE_TEMP_40, _48 and _50) before setting up
/// the visit its entrance stands for:
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

#include "script.h"
#include "game_state.h"
#include "io_write_queue.h"
#include "scene_ids.h"
#include "vram_tab.h"
#include "unit/import.h"
#include "unit/scene/colosseum_satrage_match.h"
#include "unit/scene/common/colosso_solo.h"
#include "unit/scene/common/colosso_marker.h"
#include "unit/scene/common/colosso_setup.h"
#include "unit/scene/common/colosso_occupant.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "unit/game/game.h"
#include "unit/window/window.h"
#include "unit/window/battle_command.h"
#include "unit/menu/menu.h"
#include "unit/sound/sound.h"
#include "unit/runtime/runtime.h"
#include "unit/scene/common/colosso_item_icon.h"
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
#include "fx_scene.h"
#include "mem.h"
#include "resource.h"
#include "types.h"
#include "field_event.h"
#include "field_scene.h"
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
#include "field_obj.h"
#include "global_cells.h"
#include "heap_state.h"
#include "field_runtime.h"
#include "colosso_gauge.h"
#include "flags.h"
#include "songs.h"
#include "macros.h"

#define COLOSSEUM_SATRAGE_MATCH_PLATFORM_CYCLE_FRAMES 120
#define COLOSSEUM_SATRAGE_MATCH_PLATFORM_CROSSING_FRAME 75
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
#define COLOSSEUM_SATRAGE_MATCH_SHOW_MESSAGE Engine_EventShowMessage
#define COLOSSEUM_SATRAGE_MATCH_MOVE_ACTOR_PAIR                                \
  ColosseumSatrageMatch_MovePusherAndLog
#define COLOSSEUM_SATRAGE_MATCH_START_MODE_TASK                                \
  ColosseumSatrageMatch_StartLetteringAnimation
#define COLOSSEUM_SATRAGE_MATCH_APPLY_MATCHING_SLOT_VALUE Scene_GiveAndEquipItem
#else
#define COLOSSEUM_SATRAGE_MATCH_SHOW_MESSAGE                                   \
  ((s32 (*)())Engine_EventShowMessage)
#define COLOSSEUM_SATRAGE_MATCH_MOVE_ACTOR_PAIR                                \
  ((s32 (*)())ColosseumSatrageMatch_MovePusherAndLog)
#define COLOSSEUM_SATRAGE_MATCH_START_MODE_TASK                                \
  ((s32 (*)())ColosseumSatrageMatch_StartLetteringAnimation)
#define COLOSSEUM_SATRAGE_MATCH_APPLY_MATCHING_SLOT_VALUE                      \
  ((s32 (*)())Scene_GiveAndEquipItem)
#endif

// Forward declarations.
static void ColosseumSatrageMatch_IntroduceSecondFinals(s32 attendantId);
static void ColosseumSatrageMatch_ShowRoundStartLettering(s32 mode);
static void ColosseumSatrageMatch_ShowBattleResultLettering(s32 result);
static void SceneState_EmptyHook(void);
static void ColosseumSatrageMatch_WaitForRivalFinish(void);
static s32 ColosseumSatrageMatch_ApproachFinishAndTakeItem(s32 firstItem,
                                                           s32 secondItem);
static void ColosseumSatrageMatch_StartRivalPath(s32 pathResource);
static void ColosseumSatrageMatch_PushAdjacentObstacle(void);

/// The second finals match's actors, numbered in the order of
/// gColosseumSatrageMatchPlacements from ACTOR_FIRST_PLACED; the rows of Isaac,
/// Garet, Ivan and Mia take their own actors. By the guide's and attendants'
/// explanations, the Mini-Maze, the shifting floor stage, the Log-Rolling
/// Stage, The Wall and the Board Walk lead to the arena. The stone pillars are
/// drawn as the stone pillars Move pushes elsewhere.
enum ColosseumSatrageMatchActor {
  OPPONENT = ACTOR_FIRST_PLACED, ///< Satrage, racing Isaac to the arena
  /// The Board Walk's two stone pillars, on which Move does nothing.
  BOARD_WALK_WEST_PILLAR,
  BOARD_WALK_EAST_PILLAR,
  WALL_PILLAR, ///< the stone pillar by The Wall, pushed or moved with Move
  /// The Mini-Maze's three stone pillars, pushed or moved with Move.
  /// ColosseumSatrageMatch_Initialize stands them west to east, at columns 73,
  /// 76 and 79, until they have been moved.
  MINI_MAZE_FIRST_PILLAR,
  ROLLING_LOG_WEST = MINI_MAZE_FIRST_PILLAR + 3, ///< the Board Walk's logs
  ROLLING_LOG_EAST,
  ROLLING_LOG_CENTER,
  FORCE_TARGET,
  GROWTH_PLANT,
  /// The Log-Rolling Stage's two Frost targets, drawn as the puddle Frost
  /// freezes in Lunpa (lunpa.h's SPRITE_PUDDLE).
  FROST_TARGET_WEST,
  FROST_TARGET_EAST,
  /// The shifting floor stage's four lifts, from west to east, which the
  /// lifts' operator runs.
  COLOSSEUM_SATRAGE_MATCH_FIRST_PLATFORM,
  COLOSSEUM_SATRAGE_MATCH_LAST_PLATFORM =
          COLOSSEUM_SATRAGE_MATCH_FIRST_PLATFORM +
      3,
  /// Four pieces in the lifts' sprite, lying on the Board Walk; nothing but
  /// ColosseumSatrageMatch_Initialize touches them.
  BOARD_WALK_FIRST_PLATFORM,
  /// Knocked down onto the Board Walk by Force on FORCE_TARGET.
  FALLING_OBSTACLE = BOARD_WALK_FIRST_PLATFORM + 4,
  /// "The operator of the lifts", as the shifting floor stage's attendant
  /// calls him: a young man with fair hair.
  LIFT_OPERATOR,
  RIDING_LOG_NORTH, ///< the Log-Rolling Stage's two logs
  RIDING_LOG_SOUTH,
  GUIDE, ///< a soldier who explains the second finals match
  /// The stages' attendants, young men with fair hair in white tunics.
  MINI_MAZE_ATTENDANT,
  SHIFTING_FLOOR_ATTENDANT,
  LOG_ROLLING_ATTENDANT,
  WALL_ATTENDANT,
  BOARD_WALK_ATTENDANT,
  PRIZE_A, ///< the arena's Chain Mail and Cotton Shirt
  PRIZE_B
};

/// Sets the party leader's walking speed and acceleration.
static inline void InitializeActorZero(void) {
  Engine_ActorSetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
}

/// Sets the specified actor's walking speed and acceleration.
static inline void InitializeSelectedActor(s32 actorId) {
  Engine_ActorSetSpeed(actorId, PIXELS(1), PIXELS(1) / 2);
}

const struct SceneEntrance *Scene_GetEntrances(void) {
  return gColosseumSatrageMatchEntrances;
}

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

/// Returns the packed stage and battle-result exits.
const u32 *Scene_GetExits(void) { return gColosseumSatrageMatchExits; }

/// Returns Colosseum Final 2's competitors, obstacles and attendants.
const struct ScenePlacement *Scene_GetPlacements(void) {
  return gColosseumSatrageMatchPlacements;
}

/// Move on the Board Walk's two stone pillars does nothing.
static void Event_IgnoreMove(void) {}

/// Walking into a stone pillar in Colosseum Final 2 (trigger 21) pushes the
/// obstacle immediately ahead of the selected actor.
static void Event_PushPillar(void) {
  ColosseumSatrageMatch_PushAdjacentObstacle();
}

/// Move on the stone pillar by The Wall, and after each push: its cell is
/// updated, and FLAG_PLACE_TEMP_35 keeps whether it stands at column 36.
static void Event_UpdateMovedPillarCell(void) {
  struct FieldActor *obstacle;
  s32 column;
  s32 row;

  obstacle = Object_GetById(WALL_PILLAR);
  if ((obstacle->x.fixed >> 20) == 36) {
    Engine_GameFlagSet(FLAG_PLACE_TEMP_35);
    column = 0x23;
    row = 0x4D;
    Map_CopyCellAttributes(0x23, 0x4E, 1, 1, column, row);
  } else {
    Engine_GameFlagClear(FLAG_PLACE_TEMP_35);
    column = 0x23;
    row = 0x4D;
    Map_CopyCellAttributes(0x22, 0x4D, 1, 1, column, row);
  }
}

/// Walking into the stone pillar by The Wall (triggers 20 and 26) pushes it,
/// then updates its cell.
static void Event_PushPillarAndUpdateCells(void) {
  ColosseumSatrageMatch_PushAdjacentObstacle();
  Event_UpdateMovedPillarCell();
}

/// Move on a stone pillar in the Mini-Maze, and after each push: the three
/// pillars' columns are kept (the bytes at FLAG_PLACE_TEMP_40, _48 and _50)
/// and their cells rebuilt.
static void Event_SaveMovedPillarColumns(void) {

  struct FieldActor *obstacle;
  s32 column;

  Map_CopyCellAttributes(100, 11, 12, 4, 14, 11);

  obstacle = Object_GetById(MINI_MAZE_FIRST_PILLAR);
  column = obstacle->x.fixed >> 20;
  FieldGameFlag_SetByte(FLAG_PLACE_TEMP_40, column);
  Map_CopyCellAttributes(71, 16, 1, 1, column, 16);

  obstacle = Object_GetById(MINI_MAZE_FIRST_PILLAR + 1);
  column = obstacle->x.fixed >> 20;
  FieldGameFlag_SetByte(FLAG_PLACE_TEMP_48, column);
  Map_CopyCellAttributes(71, 16, 1, 1, column, 16);

  obstacle = Object_GetById(MINI_MAZE_FIRST_PILLAR + 2);
  column = obstacle->x.fixed >> 20;
  FieldGameFlag_SetByte(FLAG_PLACE_TEMP_50, column);
  Map_CopyCellAttributes(71, 16, 1, 1, column, 16);
}

/// Force on the target over the Board Walk, before its effect plays: the effect
/// takes its child mode.
static void Event_PrepareForce(void) {
  u8 *effectWork = gEffectWork;

  ((struct BattleFxScene *)effectWork)->childMode = 1;
}

/// Force on the target over the Board Walk: the obstacle spins down into place
/// and the way past it opens (FLAG_PLACE_TEMP_30).
static void Event_KnockDownObstacle(void) {
  struct FieldActor *obstacle;
  s32 fallFrames;
  s32 rotationStep;
  s32 settleFrames;
  s32 settleRotationStep;
  struct AnimationObject *sprite;

  obstacle = Object_GetById(FALLING_OBSTACLE);
  sprite = obstacle->sprite;
  GameFlag_Set(FLAG_PLACE_TEMP_30);
  obstacle->acceleration = 0x1999;
  obstacle->speed = 0x13333;
  Engine_AudioPlayCue(SE_227);
  // Spin through the fall, then settle the obstacle at its final height.
  FieldObject_SetPosition(obstacle, PIXELS(336), PIXELS(10), PIXELS(264));
  for (rotationStep = 0, fallFrames = 9; fallFrames >= 0; fallFrames--) {
    sprite->rotation -= rotationStep;
    Engine_TaskWait(1);
    rotationStep += 36;
  }
  FieldObject_SetPosition(obstacle, PIXELS(330), -PIXELS(16), PIXELS(264));
  for (settleRotationStep = 0x168, settleFrames = 21; settleFrames >= 0;
       settleFrames--) {
    sprite->rotation -= settleRotationStep;
    Engine_TaskWait(1);
    settleRotationStep += 36;
  }
  FieldObject_WaitForMove(obstacle);
  Engine_EventWait(2);
  Engine_AudioPlayCue(SE_240);
  {
    s32 rotation = 0;

    sprite->rotation = rotation;
  }
  Engine_ActorSetAnimation(FALLING_OBSTACLE, 4);
  obstacle->x.fixed = PIXELS(336);
  obstacle->y.fixed = -PIXELS(8);
  obstacle->z.fixed = PIXELS(264);
  obstacle->velocityY = 0;
  obstacle->velocityX = 0;
  // Keep both the stage and its saved collision strip in the new state.
  Map_CopyCellAttributes(19, 16, 1, 1, 20, 16);
  Map_CopyCellAttributes(20, 80, 1, 1, 21, 80);
}

/// Frost on the Log-Rolling Stage's western target: it freezes and its crossing
/// opens (FLAG_PLACE_TEMP_31).
static void Event_FreezeWestTarget(void) {
  struct FieldActor *obstacle;

  Engine_GameFlagSet(FLAG_PLACE_TEMP_31);
  obstacle = Object_GetById(FROST_TARGET_WEST);
  obstacle->motionFlags = 0;
  {

    Map_CopyCellAttributes(46, 17, 1, 1, 44, 17);
  }
}

/// Frost on the Log-Rolling Stage's eastern target: it freezes and its crossing
/// opens (FLAG_PLACE_TEMP_32).
static void Event_FreezeEastTarget(void) {
  struct FieldActor *obstacle;

  Engine_GameFlagSet(FLAG_PLACE_TEMP_32);
  obstacle = Object_GetById(FROST_TARGET_EAST);
  obstacle->motionFlags = 0;
  {

    Map_CopyCellAttributes(46, 17, 1, 1, 50, 17);
  }
}

/// Growth on the plant by The Wall: it grows into a strip that can be climbed
/// (FLAG_PLACE_TEMP_33).
static void Event_GrowPlant(void) {
  Engine_GameFlagSet(FLAG_PLACE_TEMP_33);
  {

    Map_CopyCellAttributes(32, 37, 1, 4, 32, 77);
  }
}

/// Walking south into the obstacle at trigger 30 in Colosseum Final 2 runs the
/// rising-object sequence for the actor touching it.
static void Event_RunRisingSequence(void) {
  FieldBattleFx_RunRisingObjectSequence(gGameState.selectedActor, 6, 0);
}

/// Walking west into the obstacle at trigger 30 in Colosseum Final 2 only
/// checks what is ahead.
static void Event_CheckAhead(void) { Engine_LeaderCheckAhead(); }

/// Moves a riding log and its pusher to aligned cell centers.
static void ColosseumSatrageMatch_MovePusherAndLog(s32 logId, s32 offsetX,
                                                   s32 offsetZ) {

  struct FieldActor *pusher;
  struct FieldActor *log;
  s32 destinationX;
  s32 destinationZ;

  pusher = Object_GetById(gGameState.selectedActor);
  log = Object_GetById(logId);
  Engine_EventBegin();
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
  Object_SetMode(pusher, 27);
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
    Object_SetMode(log, 4);
  } else {
    Object_SetMode(log, 3);
  }
  Engine_AudioPlayCue(SE_226);
  FieldObject_WaitForMove(pusher);
  Engine_AudioPlayCue(SE_288);
  Object_SetMode(log, 2);
  Engine_EventEnd();
}

/// Standing on one of the Log-Rolling Stage's logs (trigger 22): it carries the
/// rider across the water the way the player holds.
static void Event_RideLog(void) {
  struct FieldActor *pusher = Object_GetById(gGameState.selectedActor);
  s32 column = pusher->x.fixed >> 20;
  s32 offsetX = 0;
  s32 logId = RIDING_LOG_NORTH;

  if (pusher->z.fixed >> 20 > 12) {
    logId = RIDING_LOG_SOUTH;
  }
  if (Object_GetById(logId)->x.fixed >> 20 != column) {
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
    Map_CopyCellAttributes(120, 10, 5, 6, 48, 10);
    column = Object_GetById(RIDING_LOG_NORTH)->x.fixed >> 20;
    Map_CopyCellAttributes(52, 28, 1, 3, column, 10);
    column = Object_GetById(RIDING_LOG_SOUTH)->x.fixed >> 20;
    Map_CopyCellAttributes(52, 28, 1, 3, column, 13);
  }
}

/// Rolls a log to half-cell coordinates while its pusher follows halfway.
static void ColosseumSatrageMatch_RollLogToCell(s32 logId, s32 halfCellX,
                                                s32 halfCellZ) {
  s32 pusher = gGameState.selectedActor;
  struct FieldActor *pusherActor = Object_GetById(pusher);
  struct FieldActor *log = Object_GetById(logId);
  s32 heading = (pusherActor->facing + 0x1000) & 0xe000;
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
  Engine_EventBegin();
  Engine_ActorSetAnimation(pusher, 8);
  Engine_EventWait(6);
  log->speed = 0x8000;
  log->acceleration = 0x3333;
  Object_SetMode(log, gColosseumSatrageMatch_RollLogScript[heading / 0x4000]);
  Engine_ObjectSetPosition(log, halfCellX, 0, halfCellZ);
  Engine_EventWait(6);
  Engine_ActorSetAnimation(pusher, 2);
  // Follow halfway behind the rolling log while its finish effect runs.
  ObjectDispatch_StartCamera(
      (struct DispatchObject *)((struct EventWork *)Runtime_AllocateBlock(
                                    HEAP_SLOT_EVENT, sizeof(struct FieldWork)))
          ->viewCenter,
      (s32)log);
  Engine_ActorSetSpeed(pusher, 0x8000, 0x3333);
  Object_SetMode(pusherActor, 2);
  Engine_ObjectSetPosition(pusherActor, pusherActor->x.fixed + offsetX, 0,
                           pusherActor->z.fixed + offsetZ);
  Engine_AudioPlayCue(SE_239);
  FieldObject_WaitForMove(pusherActor);
  Object_SetMode(pusherActor, 1);
  FieldObject_WaitForMove(log);
  Engine_AudioPlayCue(SE_288);
  Engine_AudioPlayCue(SE_213);
  Object_SetMode(log, 1);
  Engine_EventWait(15);
  Engine_EventEnd();
}

/// Rebuilds the three rolling logs' collision rectangles at their positions.
static void ColosseumSatrageMatch_UpdateRollingLogCollision(void) {
  s32 cell;

  Map_CopyCellAttributes(100, 11, 12, 4, 14, 11);

  cell = Object_GetById(ROLLING_LOG_WEST)->x.fixed >> 20;
  Map_CopyCellAttributes(13, 28, 1, 4, cell, 11);

  cell = Object_GetById(ROLLING_LOG_EAST)->x.fixed >> 20;
  Map_CopyCellAttributes(13, 28, 1, 4, cell, 11);

  cell = Object_GetById(ROLLING_LOG_CENTER)->z.fixed >> 20;
  Map_CopyCellAttributes(13, 28, 4, 1, 18, cell);
}

/// The Board Walk's logs: walking into the western one from the east (trigger
/// 24) or the west (25), or into the eastern one from the east (28) or the west
/// (27), rolls it the way the leader walks.
static void Event_RollWestLogWest(void) {
  ColosseumSatrageMatch_RollLogToCell(ROLLING_LOG_WEST, 29, 26);
  ColosseumSatrageMatch_UpdateRollingLogCollision();
}

static void Event_RollWestLogEast(void) {
  ColosseumSatrageMatch_RollLogToCell(ROLLING_LOG_WEST, 33, 26);
  ColosseumSatrageMatch_UpdateRollingLogCollision();
}

static void Event_RollEastLogWest(void) {
  ColosseumSatrageMatch_RollLogToCell(ROLLING_LOG_EAST, 45, 26);
  ColosseumSatrageMatch_UpdateRollingLogCollision();
}

static void Event_RollEastLogEast(void) {
  ColosseumSatrageMatch_RollLogToCell(ROLLING_LOG_EAST, 49, 26);
  ColosseumSatrageMatch_UpdateRollingLogCollision();
}

/// Unused: rolls the center log north and rebuilds the log collision
/// rectangles; nothing calls it.
static UNUSED void ColosseumSatrageMatch_RollCenterLogNorth(void) {
  ColosseumSatrageMatch_RollLogToCell(ROLLING_LOG_CENTER, 40, 23);
  ColosseumSatrageMatch_UpdateRollingLogCollision();
}

/// Unused: rolls the center log south and rebuilds the log collision
/// rectangles; nothing calls it.
static UNUSED void ColosseumSatrageMatch_RollCenterLogSouth(void) {
  ColosseumSatrageMatch_RollLogToCell(ROLLING_LOG_CENTER, 40, 25);
  ColosseumSatrageMatch_UpdateRollingLogCollision();
}

/// The shifting floor's callback: carries the selected actor on the lifts and
/// alternates their routes every two seconds, until Halt stops the operator.
static void ColosseumSatrageMatch_UpdateMovingPlatforms(void) {
  s32 platformId;
  struct FieldActor *rider;
  struct FieldActor *platform;
  s32 platformX;
  s32 riderX;
  s32 deltaX;
  s32 platformZ;
  s32 riderZ;
  s32 deltaZ;
  struct FieldActor *platformActor;

  rider = Object_GetById(gGameState.selectedActor);
  // Carry a rider only while both horizontal distances are less than ten
  // pixels.
  for (platformId = COLOSSEUM_SATRAGE_MATCH_FIRST_PLATFORM;
       platformId <= COLOSSEUM_SATRAGE_MATCH_LAST_PLATFORM; platformId++) {
    platform = Object_GetById(platformId);
    platform->paused = 0;
    platformX = platform->x.fixed;
    riderX = rider->x.fixed;
    deltaX = platformX - riderX;
    if (deltaX >= 0) {
      if (deltaX > 0x9ffff) {
        continue;
      }
    } else {
      riderX -= platformX;
      if (riderX > 0x9ffff) {
        continue;
      }
    }
    platformZ = platform->z.fixed;
    riderZ = rider->z.fixed;
    deltaZ = platformZ - riderZ;
    if (deltaZ >= 0) {
      if (deltaZ > 0x9ffff) {
        continue;
      }
    } else {
      riderZ -= platformZ;
      if (riderZ > 0x9ffff) {
        continue;
      }
    }
    if (GameFlag_IsSet(FLAG_FIELD_HANDLING_REQUESTS) != 0) {
      riderZ = platform->z.fixed;
    } else {
      riderZ = rider->z.fixed;
      deltaZ = platform->velocityZ;
      riderZ += deltaZ;
    }
    rider->z.fixed = riderZ;
  }
  // The final platform's destination selects the collision tiles for the row.
  if (gColosseumSatrageMatch_LiftTimer != 0 &&
      platform->targetX == 0x80000000) {
    if (gColosseumSatrageMatch_LiftPhase == 0) {
      Map_CopyCellAttributes(58, 28, 7, 1, 58, 13);
    } else {
      Map_CopyCellAttributes(58, 10, 1, 1, 58, 11);
    }
  } else {
    Map_CopyCellAttributes(57, 11, 1, 1, 58, 11);
    Map_CopyCellAttributes(58, 14, 7, 1, 58, 13);
  }
  // Start the next alternating route when the two-second cycle wraps.
  if (gColosseumSatrageMatch_LiftTimer == 0) {
    gColosseumSatrageMatch_LiftPhase ^= 1;
    if (gColosseumSatrageMatch_LiftPhase != 0) {
      platformActor = Object_GetById(COLOSSEUM_SATRAGE_MATCH_FIRST_PLATFORM);
      FieldObject_SetPosition(platformActor, PIXELS(936), 0, PIXELS(184));
      platformActor =
          Object_GetById(COLOSSEUM_SATRAGE_MATCH_FIRST_PLATFORM + 1);
      FieldObject_SetPosition(platformActor, PIXELS(968), 0, PIXELS(248));
      platformActor =
          Object_GetById(COLOSSEUM_SATRAGE_MATCH_FIRST_PLATFORM + 2);
      FieldObject_SetPosition(platformActor, PIXELS(1000), 0, PIXELS(184));
      platformActor = Object_GetById(COLOSSEUM_SATRAGE_MATCH_LAST_PLATFORM);
      FieldObject_SetPosition(platformActor, PIXELS(1032), 0, PIXELS(248));
      Engine_ActorSetAnimation(LIFT_OPERATOR, 11);
    } else {
      platformActor = Object_GetById(COLOSSEUM_SATRAGE_MATCH_FIRST_PLATFORM);
      FieldObject_SetPosition(platformActor, PIXELS(936), 0, PIXELS(216));
      platformActor =
          Object_GetById(COLOSSEUM_SATRAGE_MATCH_FIRST_PLATFORM + 1);
      FieldObject_SetPosition(platformActor, PIXELS(968), 0, PIXELS(216));
      platformActor =
          Object_GetById(COLOSSEUM_SATRAGE_MATCH_FIRST_PLATFORM + 2);
      FieldObject_SetPosition(platformActor, PIXELS(1000), 0, PIXELS(216));
      platformActor = Object_GetById(COLOSSEUM_SATRAGE_MATCH_LAST_PLATFORM);
      FieldObject_SetPosition(platformActor, PIXELS(1032), 0, PIXELS(216));
      Engine_ActorSetAnimation(LIFT_OPERATOR, 10);
    }
  }
  gColosseumSatrageMatch_LiftTimer++;
  if ((u32)gColosseumSatrageMatch_LiftTimer >=
      COLOSSEUM_SATRAGE_MATCH_PLATFORM_CYCLE_FRAMES) {
    if (GameFlag_IsSet(FLAG_FIELD_HANDLING_REQUESTS) == 0) {
      gColosseumSatrageMatch_LiftTimer = 0;
    }
  }
}

/// Stops the moving-platform task and restores its starting row.
static void ColosseumSatrageMatch_ResetMovingPlatforms(void) {
  gColosseumSatrageMatch_LiftTimer = 0;
  gColosseumSatrageMatch_LiftPhase = 0;
  Engine_TaskRemoveCallback(ColosseumSatrageMatch_UpdateMovingPlatforms);
  Actor_SetPosition(COLOSSEUM_SATRAGE_MATCH_FIRST_PLATFORM, PIXELS(936),
                    PIXELS(216));
  Actor_SetPosition(COLOSSEUM_SATRAGE_MATCH_FIRST_PLATFORM + 1, PIXELS(968),
                    PIXELS(216));
  Actor_SetPosition(COLOSSEUM_SATRAGE_MATCH_FIRST_PLATFORM + 2, PIXELS(1000),
                    PIXELS(216));
  Actor_SetPosition(COLOSSEUM_SATRAGE_MATCH_LAST_PLATFORM, PIXELS(1032),
                    PIXELS(216));
  Engine_ActorSetAnimation(LIFT_OPERATOR, 10);
}

/// Starts the moving-platform task and releases its pause flag.
static void ColosseumSatrageMatch_StartMovingPlatforms(void) {
  FieldScheduler_AddCallback(ColosseumSatrageMatch_UpdateMovingPlatforms,
                             0xC85);
  Engine_GameFlagClear(FLAG_FIELD_HANDLING_REQUESTS);
}

/// Waits for the demonstration's crossing phase, with a ten-second timeout.
static void ColosseumSatrageMatch_WaitForPlatformCrossingPhase(void) {
  s32 elapsedFrames;

  Engine_TaskWait(10);

  elapsedFrames = 0;
  while (gColosseumSatrageMatch_LiftPhase != 0 ||
         gColosseumSatrageMatch_LiftTimer !=
             COLOSSEUM_SATRAGE_MATCH_PLATFORM_CROSSING_FRAME) {
    Engine_TaskWait(1);
    elapsedFrames++;
    if (elapsedFrames >= COLOSSEUM_SATRAGE_MATCH_DEMONSTRATION_TIMEOUT_FRAMES) {
      return;
    }
  }
}

/// Halt on the shifting floor's lift operator: the lifts stop and leave a
/// crossing (FLAG_PLACE_TEMP_34).
static void Event_HaltLifts(void) {
  FieldFx_PauseObject(LIFT_OPERATOR);
  Engine_GameFlagSet(FLAG_PLACE_TEMP_34);

  if (gColosseumSatrageMatch_LiftPhase != 0) {
    gColosseumSatrageMatch_LiftTimer = 0;
  }

  Engine_TaskWait(30);
  Engine_TaskWait(1);

  Engine_TaskRemoveCallback(ColosseumSatrageMatch_UpdateMovingPlatforms);

  Map_CopyCellAttributes(58, 28, 7, 1, 58, 13);
  Map_CopyCellAttributes(57, 11, 1, 1, 58, 11);
}

/// Reaching Colosseum Final 2's arena: the leader takes a prize (the Chain Mail
/// if first, the Cotton Shirt if second), and he and Satrage face off and
/// fight; the battle's group follows who took the mail.
/// FLAG_SCENE_TEMP_11 says the arena was already reached; the arena's touch
/// works while FLAG_SCENE_TEMP_10 is clear.
static void Event_ReachArena(void) {
  s32 rivalFinishedFirst;
  s32 refreshCount;
  s32 resultBase;

  resultBase = 2;
  SceneState_EmptyHook();
  Engine_EventBegin();
  rivalFinishedFirst = ColosseumSatrageMatch_ApproachFinishAndTakeItem(77, 89);
  ColosseumSatrageMatch_WaitForRivalFinish();
  refreshCount = 9;
  do {
    Object_RefreshSelectorById(OPPONENT);
    refreshCount = (refreshCount - 1);
  } while (refreshCount >= 0);
  // Bring the competitors together before selecting the battle's result bias.
  Actor_SetSpeed(OPPONENT, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkTo(OPPONENT, 88, 256);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 120, 256);
  Engine_ActorSetAnimation(OPPONENT, 1);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, OPPONENT, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimation(OPPONENT, 3);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
  Engine_EventWait(20);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(2), PIXELS(1));
  Actor_SetSpeed(OPPONENT, PIXELS(2), PIXELS(1));
  Actor_WalkTo(ACTOR_PARTY_LEADER, 112, 256);
  Engine_ActorWalkToAndWait(OPPONENT, 96, 0x100);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 16);
  Engine_ActorSetAnimation(OPPONENT, 9);
  Engine_EventWait(10);
  /* FAKEMATCH(temporary): resultBase, set to 2 at the top, keeps the + 1
   * unfolded; as 3 - rivalFinishedFirst, which shares r5's 3 with
   * Battle_SetStart(BATTLE_START_SCENE), 3 of 106 instructions differ in
   * tbs-en. */
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_COLOSSO,
                                        resultBase - rivalFinishedFirst + 1);
  Battle_SetStart(BATTLE_START_SCENE);
  Event_SetNextDestination((s32)&SceneId_ColosseumSatrageMatch, 4);
  Event_SetDefeatDestination((s32)&SceneId_ColosseumSatrageMatch, 5);
  Engine_GameFlagSet(FLAG_KEEP_MUSIC);
}

/// Returns Colosseum Final 2's obstacle interactions and attendant dialogue.
const struct SceneEvent *Scene_GetEvents(void) {
  return gColosseumSatrageMatchEvents;
}

/// Back from the battle (COLOSSEUM_SATRAGE_MATCH_AFTER_MATCH_WON or _LOST):
/// Isaac and Satrage stand in the arena as the result is shown.
static void ColosseumSatrageMatch_StageBattleResult(s32 result) {

  Engine_ActorDestroy(PRIZE_A);
  Engine_ActorDestroy(PRIZE_B);
  Party_RestoreHpPp(1);
  Engine_EventBegin();
  Actor_SetPosition(OPPONENT, PIXELS(88), PIXELS(256));
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(120), PIXELS(256));
  Actor_FaceActor(OPPONENT, 0x4000, 0);
  Actor_FaceActor(ACTOR_PARTY_LEADER, 0x4000, 0);
  if (result < 0) {
    Engine_ActorSetAnimation(OPPONENT, 10);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 35);
  } else {
    Engine_ActorSetAnimation(OPPONENT, 8);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 28);
  }
  Engine_TaskWait(1);
  Camera_MoveTo(PIXELS(104), 0, PIXELS(192), 0);
  ColosseumSatrageMatch_ShowBattleResultLettering(result);
  Engine_EventEnd();
}

/// Sets up Colosseum Final 2: restores obstacle progress, then by the arrival:
/// the race (COLOSSEUM_SATRAGE_MATCH_RACE_FROM_LEND_A_HAND), the friends' turns
/// to lend a hand (COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_EAST), the guide's
/// tour (COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_WEST), or the match won
/// (COLOSSEUM_SATRAGE_MATCH_AFTER_MATCH_WON) or lost
/// (COLOSSEUM_SATRAGE_MATCH_AFTER_MATCH_LOST).
s32 ColosseumSatrageMatch_Initialize(void) {
  struct FieldActor *actor;
  s32 x;
  s32 obstacleId;
  s32 column;
  s32 platformId;
  s32 columnOffset;
  s32 row;
  s32 zero;

  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_FADE, 0);
  // Restore the collision templates before applying saved puzzle progress.
  Engine_GameFlagSet(FLAG_RETREAT_BLOCKED);
  Map_CopyCellAttributes(14, 11, 12, 4, 100, 11);
  Map_CopyCellAttributes(48, 10, 5, 6, 120, 10);
  for (obstacleId = BOARD_WALK_FIRST_PLATFORM; obstacleId <= FALLING_OBSTACLE;
       obstacleId++) {
    actor = Object_GetById(obstacleId);
    Object_SetMode(actor, 4);
    actor->motionFlags = 0;
    actor->y.fixed = 0;
    actor->priorityFlags = 2;
  }
  Object_GetById(FORCE_TARGET)->priorityFlags = 2;
  if (Engine_GameFlagIsSet(FLAG_PLACE_TEMP_30)) {
    actor = Object_GetById(FALLING_OBSTACLE);
    actor->x.fixed = PIXELS(336);
    actor->y.fixed = -PIXELS(8);
    actor->z.fixed = PIXELS(264);
    Map_CopyCellAttributes(19, 16, 1, 1, 20, 16);
    Map_CopyCellAttributes(20, 80, 1, 1, 21, 80);
  } else {
    actor = Object_GetById(FALLING_OBSTACLE);
    Object_SetMode(actor, 3);
    actor->y.fixed = PIXELS(16);
  }
  Object_GetById(WALL_PILLAR)->priorityFlags = 2;
  zero = 0;
  if (Engine_GameFlagIsSet(FLAG_PLACE_TEMP_35)) {
    Map_CopyCellAttributes(35, 78, 1, 1, 35, 77);
  }
  if (Engine_GameFlagIsSet(FLAG_PLACE_TEMP_33)) {
    Engine_ActorSetAnimation(GROWTH_PLANT, 4);
    Map_CopyCellAttributes(32, 37, 1, 4, 32, 77);
  }
  if (Engine_GameFlagIsSet(FLAG_PLACE_TEMP_31)) {
    Object_GetById(FROST_TARGET_WEST)->motionFlags = zero;
    Object_GetById(FROST_TARGET_WEST)->priorityFlags = 2;
    Engine_ActorSetAnimation(FROST_TARGET_WEST, 5);
    Map_CopyCellAttributes(46, 17, 1, 1, 44, 17);
  }
  if (Engine_GameFlagIsSet(FLAG_PLACE_TEMP_32)) {
    Object_GetById(FROST_TARGET_EAST)->motionFlags = zero;
    Object_GetById(FROST_TARGET_EAST)->priorityFlags = 2;
    Engine_ActorSetAnimation(FROST_TARGET_EAST, 5);
    Map_CopyCellAttributes(46, 17, 1, 1, 50, 17);
  }
  actor = Object_GetById(RIDING_LOG_NORTH);
  column = actor->x.fixed >> 20;
  actor->motionFlags = zero;
  actor->priorityFlags = 2;
  Map_CopyCellAttributes(52, 28, 1, 3, column, 10);
  actor = Object_GetById(RIDING_LOG_SOUTH);
  column = actor->x.fixed >> 20;
  actor->motionFlags = zero;
  actor->priorityFlags = 2;
  Map_CopyCellAttributes(52, 28, 1, 3, column, 13);

  // Each Mini-Maze pillar returns to the column its place flag byte keeps.
  x = FieldGameFlag_GetByte(FLAG_PLACE_TEMP_40);
  if (x == 0) {
    x = 73;
  }
  actor = Object_GetById(MINI_MAZE_FIRST_PILLAR);
  actor->x.fixed = (x << 20) + 0x80000;
  actor->motionFlags = zero;
  actor->priorityFlags = 2;
  Map_CopyCellAttributes(71, 16, 1, 1, x, 16);
  x = FieldGameFlag_GetByte(FLAG_PLACE_TEMP_48);
  if (x == 0) {
    x = 76;
  }
  actor = Object_GetById(MINI_MAZE_FIRST_PILLAR + 1);
  actor->x.fixed = (x << 20) + 0x80000;
  actor->motionFlags = zero;
  actor->priorityFlags = 2;
  Map_CopyCellAttributes(71, 16, 1, 1, x, 16);
  x = FieldGameFlag_GetByte(FLAG_PLACE_TEMP_50);
  if (x == 0) {
    x = 79;
  }
  actor = Object_GetById(MINI_MAZE_FIRST_PILLAR + 2);
  actor->x.fixed = (x << 20) + 0x80000;
  actor->motionFlags = zero;
  actor->priorityFlags = 2;
  Map_CopyCellAttributes(71, 16, 1, 1, x, 16);

  ColosseumSatrageMatch_UpdateRollingLogCollision();
  // A Halted controller leaves stationary platforms; otherwise resume the task.
  Engine_ActorSetAnimation(LIFT_OPERATOR, 10);
  if (Engine_GameFlagIsSet(FLAG_PLACE_TEMP_34)) {
    for (platformId = COLOSSEUM_SATRAGE_MATCH_FIRST_PLATFORM, row = 13,
        columnOffset = 58;
         platformId <= COLOSSEUM_SATRAGE_MATCH_LAST_PLATFORM;
         platformId++, columnOffset += 2) {
      actor = Object_GetById(platformId);
      actor->priorityFlags = 2;
      Object_SetMode(actor, 4);
      Map_CopyCellAttributes(56, 13, 1, 1, columnOffset, row);
    }
    Engine_ActorSetAnimation(LIFT_OPERATOR, 10);
    FieldFx_PauseObject(LIFT_OPERATOR);
  } else {
    for (platformId = COLOSSEUM_SATRAGE_MATCH_FIRST_PLATFORM;
         platformId <= COLOSSEUM_SATRAGE_MATCH_LAST_PLATFORM; platformId++) {
      actor = Object_GetById(platformId);
      actor->priorityFlags = 2;
      Object_SetMode(actor, 4);
      actor->speed = 0x8000;
      actor->acceleration = 0x3333;
    }
    FieldScheduler_AddCallback(ColosseumSatrageMatch_UpdateMovingPlatforms,
                               0xc85);
    Scheduler_SetTaskFlags(ColosseumSatrageMatch_UpdateMovingPlatforms, 1);
  }
  Engine_ActorSetAnimation(OPPONENT, 9);
  gGameState.movementMode = LEADER_MOVEMENT_WALK;
  Colosso_ShowItemIcon(PRIZE_B, 89);
  Colosso_ShowItemIcon(PRIZE_A, 77);
  Engine_ActorSetChildValue(OPPONENT, 1);
  // The race, the friends' turns, the tour, and the battle's result.
  switch (gGameState.entrance) {
  case COLOSSEUM_SATRAGE_MATCH_RACE_FROM_LEND_A_HAND:
    FieldScene_BuildDescriptorAndInstallTask(ACTOR_PARTY_LEADER, OPPONENT, 5,
                                             PIXELS(104), PIXELS(256), PRIZE_A,
                                             PRIZE_B);
    Map_CopyCellsLayered(127, 0, 1, 2, 79, 6);
    Engine_ActorDestroy(GUIDE);
    Engine_ActorDestroy(MINI_MAZE_ATTENDANT);
    Engine_ActorDestroy(SHIFTING_FLOOR_ATTENDANT);
    Engine_ActorDestroy(LOG_ROLLING_ATTENDANT);
    Engine_ActorDestroy(WALL_ATTENDANT);
    Engine_ActorDestroy(BOARD_WALK_ATTENDANT);
    if (!Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION)) {
      Engine_AudioPlayCue(MUS_017);
      Colosso_ControlCharacterAlone(ISAAC);
      Colosso_PlaceFriendsAtCheeringSpots();
      ColosseumSatrageMatch_ShowRoundStartLettering(2);
    }
    Object_LinkObjectAndSetCallback(GARET, ACTOR_PARTY_LEADER);
    Object_LinkObjectAndSetCallback(IVAN, ACTOR_PARTY_LEADER);
    Object_LinkObjectAndSetCallback(MIA, ACTOR_PARTY_LEADER);
    ColosseumSatrageMatch_StartRivalPath((s32)&ResourceId_RivalPathB);
    break;
  case COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_EAST:
    FieldScheduler_AddCallback(Colosso_RaiseTakePlaceOnPsynergy, 0xc80);
    Engine_ActorDestroy(PRIZE_A);
    Engine_ActorDestroy(PRIZE_B);
    if (!Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION)) {
      Colosso_PlaceFriendsAtCheeringSpots();
      Colosso_ControlCharacterAlone(GARET);
      ColosseumSatrageMatch_ShowRoundStartLettering(0);
    }
    break;
  case COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_WEST:
    if (!Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION)) {
      ColosseumSatrageMatch_IntroduceSecondFinals(GUIDE);
      Colosso_ClearCheeringSpots();
    }
    break;
  case COLOSSEUM_SATRAGE_MATCH_AFTER_MATCH_WON:
    ColosseumSatrageMatch_StageBattleResult(2);
    Engine_EventRequestExit(COLOSSEUM_SATRAGE_MATCH_TO_COLOSSEUM_NORTH);
    break;
  case COLOSSEUM_SATRAGE_MATCH_AFTER_MATCH_LOST:
    ColosseumSatrageMatch_StageBattleResult(-2);
    Engine_EventRequestExit(COLOSSEUM_SATRAGE_MATCH_TO_BABI_PALACE);
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
  struct FieldActor *actor;

  actor = Object_GetById(attendantId);
  attendantX = actor->x.part.pixel;
  attendantZ = actor->z.part.pixel;
  Engine_EventBegin();
  Actor_SetSpeed(attendantId, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(GARET, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(IVAN, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(MIA, PIXELS(1), PIXELS(1) / 2);
  Engine_ActorSetPosition(ACTOR_PARTY_LEADER, (attendantX << 16),
                          ((attendantZ << 16) - 0x300000));
  Engine_ActorSetPosition(GARET, (attendantX << 16) - 0x100000,
                          (attendantZ << 16) - 0x280000);
  Engine_ActorSetPosition(IVAN, (attendantX << 16) + 0x100000,
                          (attendantZ << 16) - 0x280000);
  Engine_ActorSetPosition(MIA, (attendantX << 16),
                          ((attendantZ << 16) - 0x200000));
  Engine_ActorSetPosition(attendantId, (attendantX << 16),
                          ((attendantZ << 16) - 0x500000));
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  actor->facing = FACING_NORTH;
  Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventSetMessage(MsgColossoSiteSecondFinals);
  Engine_ActorSetAnimationAndWait(attendantId, 3);
  Engine_EventShowMessage(attendantId, 0);
  Engine_ActorRunRepeatedMotion(attendantId, 2);
  Engine_EventShowMessage(attendantId, 0);
  Engine_ActorRunRepeatedMotion(attendantId, 2);
  Engine_EventShowMessage(attendantId, 0);
  Engine_ActorRunRepeatedMotion(attendantId, 2);
  Engine_EventShowMessage(attendantId, 0);
  // Gather the companions into Isaac before the attendant returns to his post.
  Engine_ActorSetAnimation(MIA, 3);
  Engine_ActorSetAnimation(GARET, 3);
  Engine_ActorSetAnimation(IVAN, 3);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
  Engine_EventWait(6);
  Engine_ActorSetAnimation(GARET, 2);
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Engine_ActorSetDestination(GARET, actor->x.part.pixel, actor->z.part.pixel);
  }
  Engine_ActorSetAnimation(IVAN, 2);
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Engine_ActorSetDestination(IVAN, actor->x.part.pixel, actor->z.part.pixel);
  }
  Engine_ActorSetAnimation(MIA, 2);
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Engine_ActorSetDestination(MIA, actor->x.part.pixel, actor->z.part.pixel);
  }
  Engine_ActorWalkToAndWait(attendantId, (attendantX - 16), (attendantZ - 64));
  Engine_ActorSetPosition(GARET, 0, 0);
  Engine_ActorSetPosition(IVAN, 0, 0);
  Engine_ActorSetPosition(MIA, 0, 0);
  Engine_ActorWalkToAndWait(attendantId, (attendantX - 16), (attendantZ - 16));
  Engine_ActorWalkToAndWait(attendantId, attendantX, attendantZ);
  Engine_ActorFaceDirection(attendantId, FACING_NORTH, 10);
  Engine_EventEnd();
}

/// The Mini-Maze's attendant, stage 1 of Colosseum Final 2: the first time,
/// Isaac appears and runs the maze; then the objective, and a place for a
/// friend to cheer. With a friend alone on the stages
/// (COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_EAST) he asks whether they are done
/// cheering instead.
static void Event_TalkToMiniMazeAttendant(s32 attendant) {
  s32 result;
  struct FieldActor *actor;

  if (gGameState.entrance == COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_EAST) {
    Colosso_AskDoneCheering(attendant);
  } else {
    Engine_EventBegin();
    result = Colosso_GreetAtStage(attendant, 1);
    if (result == COLOSSO_GUIDE_DESCRIBE) {
      Engine_EventSetMessage(MsgColossoStageDubbedMini);
      Engine_CameraSetSpeed(0x30000, 0x6000);
      Engine_CameraMoveTo(PIXELS(1224), -1, PIXELS(184), 1);
      Engine_CameraWaitForMove();
      Engine_EventShowMessage(attendant, 0);
      Colosso_FadeInCompetitor(ACTOR_PARTY_LEADER, 1272, 168);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 2, PIXELS(3) / 4);
      Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 1288, 184);
      Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 1288, 216);
      Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 1224, 216);
      Engine_EventShowMessage(attendant, 0);
      Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 1224, 248);
      Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 1192, 248);
      Engine_EventWait(3);
      actor = Object_GetById(ACTOR_PARTY_LEADER);
      actor->velocityY = 0x40000;
      Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 28);
      Engine_ActorSetAttachedEffect(
          ACTOR_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
      Engine_EventWait(30);
      Engine_EventShowMessage(attendant, 0);
      Colosso_RestoreCompetitor(ACTOR_PARTY_LEADER);
      Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
      Colosso_RepeatStageNumber(attendant, 1);
    } else if (result == COLOSSO_GUIDE_OFFER_CHEERING) {
      Engine_EventSetMessage(MsgColossoMainThingStage);
      Engine_EventShowMessage(attendant, 0);
    }
    Colosso_OfferCheeringSpot(result, attendant, 1);
    Engine_EventEnd();
  }
}

/// The shifting floor stage's attendant, stage 2 of Colosseum Final 2: the
/// first time, Isaac appears and rides the lifts; then, that the lifts'
/// operator will cheer for Isaac, and a place for a friend to cheer.
static void Event_TalkToShiftingFloorAttendant(s32 attendant) {
  s32 result;

  if (gGameState.entrance == COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_EAST) {
    Colosso_AskDoneCheering(attendant);
  } else {
    Engine_EventBegin();
    result = Colosso_GreetAtStage(attendant, 2);
    if (result == COLOSSO_GUIDE_DESCRIBE) {
      Engine_EventSetMessage(MsgColossoShiftingFloorStage);
      ColosseumSatrageMatch_ResetMovingPlatforms();
      Camera_SetSpeed(0x30000, 0x6000);
      Camera_MoveTo(PIXELS(984), -1, PIXELS(232), 1);
      Engine_CameraWaitForMove();
      /* FAKEMATCH(wrapper): called through a cast to the s32 call the bytes
       * show, which sets r1 before r0; as a direct call, 1 of 104
       * instructions differ in tbs-en. */
      COLOSSEUM_SATRAGE_MATCH_SHOW_MESSAGE(attendant, 0);
      ColosseumSatrageMatch_StartMovingPlatforms();
      Event_ShowMessage(attendant, 0);
      Colosso_FadeInCompetitor(ACTOR_PARTY_LEADER, 1080, 264);
      Engine_EventWait(15);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 2, PIXELS(3) / 4);
      Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 1080, 216);
      Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 1064, 216);
      ColosseumSatrageMatch_WaitForPlatformCrossingPhase();
      Engine_LeaderCheckAhead();
      Camera_MoveTo(-1, -1, -1, 0);
      Event_ShowMessage(attendant, 0);
      Colosso_RestoreCompetitor(ACTOR_PARTY_LEADER);
      Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
      Colosso_RepeatStageNumber(attendant, 2);
    } else {
      if (result == COLOSSO_GUIDE_OFFER_CHEERING) {
        Engine_EventSetMessage(MsgColossoOperatorLiftsWillCheerFor);
        Event_ShowMessage(attendant, 0);
      }
    }
    Colosso_OfferCheeringSpot(result, attendant, 2);
    Engine_EventEnd();
  }
}

/// The Log-Rolling Stage's attendant, stage 3 of Colosseum Final 2: the first
/// time, Isaac appears and rides a log across; then the objective, and a place
/// for a friend to cheer.
static void Event_TalkToLogRollingAttendant(s32 attendant) {
  s32 result;

  if (gGameState.entrance == COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_EAST) {
    Colosso_AskDoneCheering(attendant);
  } else {
    Engine_EventBegin();
    result = Colosso_GreetAtStage(attendant, 3);
    if (result == COLOSSO_GUIDE_DESCRIBE) {
      Engine_EventSetMessage(MsgColossoLogRollingStage);
      Camera_SetSpeed(0x30000, 0x6000);
      Camera_MoveTo(PIXELS(752), -1, PIXELS(192), 1);
      Engine_CameraWaitForMove();
      Engine_EventWait(60);
      Camera_SetSpeed(0x10000, 0x2000);
      Camera_MoveTo(PIXELS(752), -1, PIXELS(224), 1);
      Engine_CameraWaitForMove();
      Event_ShowMessage(attendant, 0);
      Colosso_FadeInCompetitor(ACTOR_PARTY_LEADER, 856, 264);
      Engine_EventWait(10);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 2, PIXELS(3) / 4);
      Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 856, 264);
      Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 856, 232);
      Event_ShowMessage(attendant, 0);
      Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 840, 232);
      Engine_EventWait(10);
      /* FAKEMATCH(wrapper): called through a cast to the s32 call the bytes
       * show, which sets r0 last; as a direct call, 1 of 163 instructions
       * differ in tbs-en. */
      COLOSSEUM_SATRAGE_MATCH_MOVE_ACTOR_PAIR(RIDING_LOG_SOUTH, -64, 0);
      Camera_MoveTo(PIXELS(752), -1, PIXELS(216), 1);
      Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
      Engine_EventWait(10);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 760, 232);
      Engine_EventWait(10);
      Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 30);
      Event_ShowMessage(attendant, 0);
      Colosso_RestoreCompetitor(ACTOR_PARTY_LEADER);
      Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
      Actor_SetPosition(RIDING_LOG_SOUTH, PIXELS(840), PIXELS(232));
      Colosso_RepeatStageNumber(attendant, 3);
    } else if (result == COLOSSO_GUIDE_OFFER_CHEERING) {
      Engine_EventSetMessage(MsgColossoHereYourObjectiveRideLogs);
      Event_ShowMessage(attendant, 0);
    }
    Colosso_OfferCheeringSpot(result, attendant, 3);
    Engine_EventEnd();
  }
}

/// The Wall's attendant, stage 4 of Colosseum Final 2: the first time, Isaac
/// appears and climbs it; then the objective, and a place for a friend to
/// cheer.
static void Event_TalkToWallAttendant(s32 attendant) {
  s32 z;
  struct FieldActor *actor;
  s32 result;

  if (gGameState.entrance == COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_EAST) {
    Colosso_AskDoneCheering(attendant);
    return;
  }
  Engine_EventBegin();
  result = Colosso_GreetAtStage(attendant, 4);
  if (result == COLOSSO_GUIDE_DESCRIBE) {
    Engine_EventSetMessage(MsgColossoFansJustCallWall);
    Engine_CameraSetSpeed(0x30000, 0x6000);
    Camera_MoveTo(PIXELS(536), -1, PIXELS(240), 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(45);
    Camera_SetSpeed(0x10000, 0x2000);
    Camera_MoveTo(PIXELS(536), -1, PIXELS(192), 1);
    z = 264;
    Engine_CameraWaitForMove();
    Engine_EventShowMessage(attendant, 0);
    Colosso_FadeInCompetitor(ACTOR_PARTY_LEADER, 632, z);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 616, z);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
    Field_ClimbUp();
    Camera_SetSpeed(0x4000, 0x800);
    Camera_MoveTo(PIXELS(536), -1, PIXELS(160), 1);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 10);
    actor = Object_GetById(ACTOR_PARTY_LEADER);
    Engine_ObjectSetPosition(actor, actor->x.fixed, actor->y.fixed + 0x400000,
                             actor->z.fixed);
    Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
    Field_ClimbUp();
    Camera_MoveTo(-1, -1, -1, 0);
    Event_ShowMessage(attendant, 0);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 2, PIXELS(3) / 4);
    Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 488, 248);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 20);
    FieldBattleFx_RunRisingObjectSequence(ACTOR_PARTY_LEADER, 6, 0);
    Engine_CameraMoveTo(PIXELS(536), -1, PIXELS(160), 1);
    Engine_EventShowMessage(attendant, 0);
    Colosso_RestoreCompetitor(ACTOR_PARTY_LEADER);
    Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
    Colosso_RepeatStageNumber(attendant, 4);
  } else if (result == COLOSSO_GUIDE_OFFER_CHEERING) {
    Engine_EventSetMessage(MsgColossoScalingWallQuickly);
    Engine_EventShowMessage(attendant, 0);
  }
  Colosso_OfferCheeringSpot(result, attendant, 4);
  Engine_EventEnd();
}

/// Moves a demonstration obstacle to pixel coordinates while its pusher
/// follows.
static void ColosseumSatrageMatch_PushObstacleToCell(s32 obstacleId, s32 column,
                                                     s32 row) {
  s32 pusher = gGameState.selectedActor;
  struct FieldActor *pusherActor = Object_GetById(pusher);
  struct FieldActor *obstacle = Object_GetById(obstacleId);
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
  Engine_ActorSetAnimation(pusher, 8);
  Engine_EventWait(6);
  obstacle->speed = 0x8000;
  obstacle->acceleration = 0x3333;
  Engine_AudioPlayCue(SE_239);
  Object_SetMode(obstacle, 3);
  Engine_ObjectSetPosition(obstacle, column, 0, row);
  Engine_EventWait(6);
  Engine_ActorSetAnimation(pusher, 2);
  Engine_ActorSetSpeed(pusher, 0x8000, 0x3333);
  Object_SetMode(pusherActor, 2);
  Engine_ObjectSetPosition(pusherActor, pusherActor->x.fixed + offsetX, 0,
                           pusherActor->z.fixed + offsetZ);
  FieldObject_WaitForMove(pusherActor);
  Object_SetMode(pusherActor, 1);
  FieldObject_WaitForMove(obstacle);
  Object_SetMode(obstacle, 1);
  Engine_AudioPlayCue(SE_288);
  Engine_AudioPlayCue(SE_213);
  Engine_EventWait(15);
}

/// The Board Walk's attendant, stage 5 of Colosseum Final 2: the first time,
/// Isaac appears and rolls a log into a path; then the objective, and a place
/// for a friend to cheer.
static void Event_TalkToBoardWalkAttendant(s32 attendant) {
  s32 result;

  if (gGameState.entrance == COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_EAST) {
    Colosso_AskDoneCheering(attendant);
    return;
  }
  Engine_EventBegin();
  result = Colosso_GreetAtStage(attendant, 5);
  if (result == COLOSSO_GUIDE_DESCRIBE) {
    Engine_EventSetMessage(MsgColossoPlaceCalledBoard);
    Engine_CameraSetSpeed(0x20000, 0x4000);
    Camera_MoveTo(PIXELS(328), -1, PIXELS(264), 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(30);
    Camera_SetSpeed(0x18000, 0x3000);
    Camera_MoveTo(PIXELS(312), -1, PIXELS(176), 1);
    Engine_CameraWaitForMove();
    Engine_EventShowMessage(attendant, 0);
    Colosso_FadeInCompetitor(ACTOR_PARTY_LEADER, 408, 264);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 2, PIXELS(3) / 4);
    Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 408, 216);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 10);
    Engine_EventShowMessage(attendant, 0);
    ColosseumSatrageMatch_PushObstacleToCell(ROLLING_LOG_EAST, 360, 208);
    Engine_ActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 8, 45);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 2, PIXELS(3) / 4);
    Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 376, 216);
    Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 376, 248);
    Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 312, 248);
    Engine_EventShowMessage(attendant, 0);
    Colosso_RestoreCompetitor(ACTOR_PARTY_LEADER);
    Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
    Actor_SetPosition(ROLLING_LOG_EAST, PIXELS(392), PIXELS(208));
    Colosso_RepeatStageNumber(attendant, 5);
  } else if (result == COLOSSO_GUIDE_OFFER_CHEERING) {
    Engine_EventSetMessage(MsgColossoFocusRollingLogs);
    Engine_EventShowMessage(attendant, 0);
  }
  Colosso_OfferCheeringSpot(result, attendant, 5);
  Engine_EventEnd();
}

#include "common/colosso_solo.h"
#include "common/item_equip.h"
#include "common/colosso_task.h"

/// Chooses the lettering animation script and resets its task state.
static void ColosseumSatrageMatch_StartLetteringAnimation(u32 mode,
                                                          u32 tileVariant) {
  s32 script;

  gColosso_LetteringMode = mode;
  gColosso_LetteringParam = (tileVariant << 4);

  {
    s32 priority = 0xc80;
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
    Engine_EventBegin();
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(30);
    Engine_AudioPlayCue(MUS_089);
    Colosso_LoadLettering(COLOSSO_PICTURE_LEND_A_HAND);
    ColosseumSatrageMatch_StartLetteringAnimation(COLOSSO_SHAPE_TITLE, 0);
    Engine_EventWait(120);
    Engine_EventEnd();
    return;
  }

  Engine_AudioPlayCue(SE_247);
  Engine_EventBegin();
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  {
    s16 *openingScript = gColosseumSatrageMatch_LetteringTitle;
    *(s16 *)((u8 *)openingScript + 30) = (mode * 60);
  }
  Engine_EventWait(30);
  Engine_AudioPlayCue(mode + 0x5a);
  Colosso_LoadLettering(mode);
  ColosseumSatrageMatch_StartLetteringAnimation(COLOSSO_SHAPE_TITLE, 0);
  Engine_EventWait(120);

  // Finish the opening cue before the final lettering pulses and start signal.
  while (AudioCommand_GetStateByte() != 0) {
    Engine_TaskWait(1);
  }

  Engine_AudioPlayCue(SE_289);
  Colosso_LoadLettering(COLOSSO_PICTURE_COUNT_3_2);
  ColosseumSatrageMatch_StartLetteringAnimation(COLOSSO_SHAPE_DIGIT, 0);
  Engine_AudioPlayCue(SE_236);
  Engine_EventWait(60);
  ColosseumSatrageMatch_StartLetteringAnimation(COLOSSO_SHAPE_DIGIT, 1);
  Engine_AudioPlayCue(SE_236);
  Engine_EventWait(60);
  Colosso_LoadLettering(COLOSSO_PICTURE_COUNT_1);
  ColosseumSatrageMatch_StartLetteringAnimation(COLOSSO_SHAPE_DIGIT, 0);
  Engine_AudioPlayCue(SE_236);
  Engine_EventWait(60);
  Colosso_LoadLettering(COLOSSO_PICTURE_GO);
  ColosseumSatrageMatch_StartLetteringAnimation(COLOSSO_SHAPE_GO, 0);
  Engine_AudioPlayCue(SE_237);
  Audio_PlayCueFromEventWork();
  Engine_EventEnd();
  Engine_GameFlagSet(FLAG_IN_TOWN);
}

/// The result: `Won` or `Lose` in large lettering, and the loser's reaction.
static void ColosseumSatrageMatch_ShowBattleResultLettering(s32 result) {

  s32 reactingActor;

  Engine_AudioPlayCue(SE_247);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  gColosseumSatrageMatch_LetteringWon[13] = result * 60;
  STORE_CONST(gColosseumSatrageMatch_LetteringLose[13]) =
      (result < 0 ? -result : result) * 60;
  if (result < 0) {
    Engine_EventWait(30);
    Engine_AudioPlayCue(MUS_086);
    Colosso_LoadLettering(COLOSSO_PICTURE_WON_LOSE_FOR_LOSS);
    /* FAKEMATCH(wrapper): called through a cast to the s32 call the bytes
     * show, which sets r1 before r0; as a direct call, 1 of 72 instructions
     * differ in tbs-en. */
    COLOSSEUM_SATRAGE_MATCH_START_MODE_TASK(3, 1);
    Engine_EventWait(-result * 60 + 60);
    reactingActor = 0;
  } else {
    Engine_EventWait(30);
    Engine_AudioPlayCue(result + 90);
    Colosso_LoadLettering(COLOSSO_PICTURE_WON_LOSE);
    /* FAKEMATCH(wrapper): the same cast as the first arm, which sets r1 before
     * r0; as a direct call, 1 of 72 instructions differ in tbs-en. */
    COLOSSEUM_SATRAGE_MATCH_START_MODE_TASK(3, 0);
    Engine_EventWait(result * 60 + 60);
    reactingActor = 8;
  }
  Engine_ActorShowEmote(reactingActor, EMOTE_IN_FRONT | 5, 0);
  while (AudioCommand_GetStateByte() != 0) {
    Engine_TaskWait(1);
  }
  Engine_AudioPlayCue(MUS_019);
  Engine_EventWait(30);
  Engine_AudioPlayCue(SE_289);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
}

#include "common/colosso_marker.h"
#include "common/colosso_item_icon.h"

/// Unused: writes eight uppercase hexadecimal digits followed by a terminator;
/// nothing calls it.
static UNUSED void Text_WriteU32AsHex(u8 *destination, u32 value) {
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
static void SceneState_EmptyHook(void) {}

/// Unused: marks the rival's path recorder finished (PATH_RECORDER_FINISHED);
/// nothing calls it.
static UNUSED void ColosseumSatrageMatch_MarkRivalFinished(void) {
  ((struct PathRecorder *)gSceneState)->mode = PATH_RECORDER_FINISHED;
}

static void ColosseumSatrageMatch_WaitForRivalFinish(void) {
  s16 *mode = &((struct PathRecorder *)gSceneState)->mode;

  while (*mode != 9) {
    Engine_TaskWait(1);
  }
}

/// Occasionally spawns a falling particle around the linked finish effect.
static void
ColosseumSatrageMatch_SpawnRandomSceneEffect(union FieldObject *fieldObject) {
  struct FieldActor *source = (struct FieldActor *)fieldObject;
  s32 position[3];
  u32 chance;

  if (source->velocityY >= -255 && source->velocityY <= 255) {
    source->motionFlags = 0;
  }
  chance = Engine_RandomNext();
  if (chance * 100 >> 16 <= 9) {
    struct FieldActor *particle;
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
      Engine_ActorSetSpriteFlags(particle, 0);
      Engine_ObjectSetScript(particle, gColosseumSatrageMatch_SparkScript);
      Object_SetMode(particle, 1);
      Object_SetMode(particle, 0);
    }
  }
}

/// Raises the linked finish effect, starts its script and clears the link.
static s32 ColosseumSatrageMatch_RaiseLinkedSceneEffect(
    struct ScriptInterpreter *interpreter) {
  struct FieldActor *source = (struct FieldActor *)interpreter;
  struct FieldActor *effect = Object_GetById(source->timer);

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
  struct ColossoGaugeWork *gauge = WORK_SLOT(HEAP_SLOT_COLOSSO_RACE);
  u8 *gameState;
  struct FieldActor *competitor;
  s32 finishAlreadyReached;
  s32 x;
  s32 z;
  u16 *itemActor;
  s16 *timer;

  finishAlreadyReached = Engine_GameFlagIsSet(FLAG_SCENE_TEMP_11);

  gameState = (u8 *)&gGameState;
  competitor = Object_GetById(((struct GameState *)gameState)->selectedActor);

  if (gauge->finishX < competitor->x.fixed) {
    x = gauge->finishX + PIXELS(12);
  } else {
    x = gauge->finishX - PIXELS(12);
  }

  if (finishAlreadyReached != 0) {
    z = gauge->finishZ + PIXELS(16);
    itemActor = &gauge->finishItemActorB;
  } else {
    z = gauge->finishZ - PIXELS(16);
    itemActor = &gauge->finishItemActorA;
  }

  timer = (s16 *)&competitor->timer;
  *timer = *itemActor;
  competitor->acceleration = PIXELS(1) / 4;
  competitor->speed = PIXELS(1);

  Engine_ObjectSetPosition(competitor, x, 0, z);
  Engine_GameFlagSet(FLAG_SCENE_TEMP_11);
  Engine_ObjectSetScript(competitor, gColosseumSatrageMatch_ApproachScript);

  while (*timer != 0) {
    Engine_TaskWait(1);
  }

  // Assign the item on the side this competitor reached first.
  if (finishAlreadyReached == 0) {
    /* FAKEMATCH(wrapper): called through a cast to the s32 call the bytes
     * show, which sets r1 before r0; as a direct call, 1 of 110 instructions
     * differ in tbs-en. */
    COLOSSEUM_SATRAGE_MATCH_APPLY_MATCHING_SLOT_VALUE(0, firstItem);
    UiWork_PushValueSlot(firstItem, 2);
  } else {
    /* FAKEMATCH(wrapper): the same cast as the first arm, which sets r1 before
     * r0; as a direct call, 1 of 110 instructions differ in tbs-en. */
    COLOSSEUM_SATRAGE_MATCH_APPLY_MATCHING_SLOT_VALUE(0, secondItem);
    UiWork_PushValueSlot(secondItem, 2);
  }

  gameState = (u8 *)&gGameState;
  UiWork_PushValueSlot(((struct GameState *)gameState)->selectedActor, 1);
  Engine_MessageShowCentered(MsgColossoIsaacGotItem, 3);
  ObjectDispatch_WaitForValue16(competitor);

  return finishAlreadyReached;
}

#include "common/colosso_path_rival.h"

/// Allocates the race work, records both competitors and installs the gauge
/// task.
void FieldScene_BuildDescriptorAndInstallTask(s32 leaderId, s32 rivalId,
                                              s32 segments, s32 finishX,
                                              s32 finishZ, s32 firstItem,
                                              s32 secondItem) {
  struct ColossoRaceWork *work;
  struct FieldActor *leader;
  struct FieldActor *rival;
  void *graphics;
  s32 vramSlot;

  work = Runtime_AllocateBlock(HEAP_SLOT_COLOSSO_RACE,
                               sizeof(struct ColossoRaceWork));
  graphics = Runtime_BumpAllocateAlternatePool(512);

  work->gauge.playerActor = leaderId;
  work->gauge.rivalActor = rivalId;
  work->gauge.finishItemActorA = firstItem;
  work->gauge.finishItemActorB = secondItem;
  work->gauge.segments = segments;
  work->gauge.finishX = finishX;
  work->gauge.finishZ = finishZ;

  leader = Object_GetById(leaderId);
  rival = Object_GetById(rivalId);

  // Mirror the recorded rival start when this is a fresh race entrance.
  if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
    rival->x.fixed = (finishX << 1) - leader->x.fixed;
    rival->z.fixed = leader->z.fixed;
  }

  work->gauge.level = 0;
  work->gauge.hold = 0;

  // Upload the gauge graphics and keep only its VRAM slot after freeing
  // scratch.
  FieldResource_DecodeType01(gColosso_GaugeGraphics, graphics);

  vramSlot = Resource_FindFreeEntry();
  work->gauge.vramBlock = vramSlot;
  Engine_VramLoad((s16)vramSlot, 512, graphics);

  FieldScheduler_AddCallback(Colosso_DrawGauge, 0xc76);

  Runtime_BumpFree(graphics);
}

/// Decodes the rival's recorded path and starts its playback task.
static void ColosseumSatrageMatch_StartRivalPath(s32 pathResource) {
  struct ColossoRaceWork *work = WORK_SLOT(HEAP_SLOT_COLOSSO_RACE);
  struct PathRecorder *recorder = (struct PathRecorder *)gSceneState;

  FieldResource_DecodeType01(Resource_GetTableEntry(pathResource), work->path);
  if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
    recorder->mode = PATH_RECORDER_PLAY;
    recorder->mirror = 1;
    recorder->actor = work->gauge.rivalActor;
    recorder->still = 0;
    recorder->pos = 0;
  }
  FieldScheduler_AddCallback(Colosso_UpdatePathRival, 0xc85);
}

#include "common/colosso_scene_state.h"
#include "common/object_findslot.h"

/// Pushes the adjacent obstacle only when the destination and space above are
/// clear.
static void ColosseumSatrageMatch_PushAdjacentObstacle(void) {

  struct FieldActor *pusher;
  struct FieldActor *obstacle;
  struct FieldActor *blocker;
  u32 packedOffset;
  u32 direction;
  struct FieldPosition destination;
  s32 zero;
  s32 actorId;

  actorId = gGameState.selectedActor;
  pusher = Object_GetById(actorId);

  direction = pusher->facing >> 12;

  packedOffset = gColosseumSatrageMatch_DirectionSteps[direction];
  destination.x = pusher->x.fixed + (packedOffset & 0xffff0000);
  destination.y = pusher->y.fixed;
  packedOffset <<= 16;
  destination.z = pusher->z.fixed + packedOffset;

  obstacle = SceneData_FindSlotAtPosition(&destination, pusher);
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

  blocker = SceneData_FindSlotAtPosition(&destination, obstacle);
  if (blocker != 0 && (blocker->collisionFlags & 1) != 0) {
    return;
  }

  destination.x = obstacle->x.fixed;
  destination.y = obstacle->y.fixed + 0x100000;
  destination.z = obstacle->z.fixed;

  blocker = SceneData_FindSlotAtPosition(&destination, obstacle);
  if (blocker != 0 && (blocker->collisionFlags & 1) != 0) {
    return;
  }

  obstacle->terrainId = 2;
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
  Object_SetMode(pusher, 8);
  Engine_TaskWait(15);

  obstacle->speed = 0x3333;
  obstacle->acceleration = 0x3333;
  Engine_ObjectSetPosition(obstacle, destination.x, destination.y,
                           destination.z);

  pusher->speed = 0x3333;
  pusher->acceleration = 0x3333;
  Engine_ObjectSetPosition(pusher, destination.x, destination.y, destination.z);

  Engine_AudioPlayCue(SE_238);
  FieldObject_WaitForMove(obstacle);
  Engine_AudioPlayCue(SE_288);

  obstacle->x.fixed = destination.x;
  obstacle->z.fixed = destination.z;
  obstacle->velocityX = zero;
  obstacle->velocityZ = zero;

  Object_SetMode(pusher, 1);
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
    SC_SET(SW_HOOK, (s32)ColosseumSatrageMatch_SpawnRandomSceneEffect),
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
    SC_POS(0, 0, 0),
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

struct SceneEntrance gColosseumSatrageMatchEntrances[] = {
    { COLOSSEUM_SATRAGE_MATCH_RACE_FROM_LEND_A_HAND, CONDITION_ALWAYS, 1272, 0, 168, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_EAST, CONDITION_ALWAYS, 1224, 0, 488, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { 10, CONDITION_ALWAYS, 384, 0, 264, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

u32 gColosseumSatrageMatchExits[] = {
    SCENE_EXITS(ColosseumSatrageMatch),
    SCENE_EXIT(COLOSSEUM_SATRAGE_MATCH_LEND_A_HAND_TO_RACE, ColosseumSatrageMatch, COLOSSEUM_SATRAGE_MATCH_RACE_FROM_LEND_A_HAND),
    SCENE_EXIT(COLOSSEUM_SATRAGE_MATCH_TO_COLOSSEUM_SOUTH, Colosseum, COLOSSEUM_FROM_COLOSSEUM_SATRAGE_MATCH_SOUTH),
    SCENE_EXIT(COLOSSEUM_SATRAGE_MATCH_TO_COLOSSEUM_NORTH, Colosseum, COLOSSEUM_FROM_COLOSSEUM_SATRAGE_MATCH_NORTH),
    SCENE_EXIT(COLOSSEUM_SATRAGE_MATCH_TO_BABI_PALACE, BabiPalace, BABI_PALACE_FROM_COLOSSEUM_AZART_MATCH),
    SCENE_EXITS_END,
};

struct ScenePlacement gColosseumSatrageMatchPlacements[] = {
    { 0x0, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1272), 0, PIXELS(168), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { IVAN, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { MIA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(264), 0, PIXELS(168), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(376), 0, PIXELS(168), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(568), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1272), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1224), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STONE_PILLAR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1176), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7,
        PIXELS(264), 0, PIXELS(208), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7,
        PIXELS(392), 0, PIXELS(208), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x103, CONDITION_ALWAYS, 7,
        PIXELS(360), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_SPROUT, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(520), 0, PIXELS(258), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(712), 0, PIXELS(280), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_PUDDLE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(808), 0, PIXELS(280), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x122, CONDITION_ALWAYS, 7,
        PIXELS(936), 0, PIXELS(216), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x122, CONDITION_ALWAYS, 7,
        PIXELS(968), 0, PIXELS(216), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x122, CONDITION_ALWAYS, 7,
        PIXELS(1000), 0, PIXELS(216), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x122, CONDITION_ALWAYS, 7,
        PIXELS(1032), 0, PIXELS(216), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x122, CONDITION_ALWAYS, 7,
        PIXELS(248), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x122, CONDITION_ALWAYS, 7,
        PIXELS(264), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x122, CONDITION_ALWAYS, 7,
        PIXELS(296), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x122, CONDITION_ALWAYS, 7,
        PIXELS(312), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x103, CONDITION_ALWAYS, 7,
        PIXELS(360), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1072), 0, PIXELS(280), FACING_WEST, TALK_KEEP_FACING, 0 },
    { SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7,
        PIXELS(840), 0, PIXELS(184), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7,
        PIXELS(840), 0, PIXELS(232), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1224), 0, PIXELS(488), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1208), 0, PIXELS(376), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(984), 0, PIXELS(376), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(760), 0, PIXELS(376), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(536), 0, PIXELS(376), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(312), 0, PIXELS(376), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(104), 0, PIXELS(240), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(104), 0, PIXELS(272), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

s32 gColosseumSatrageMatch_LiftPhase = 0;

s32 gColosseumSatrageMatch_LiftTimer = 0;

struct SceneEvent gColosseumSatrageMatchEvents[] = {
    { TOUCH_WALKING_INTO(FACING_SOUTH), 30, CONDITION_ALWAYS, EVENT_SCRIPT(Event_RunRisingSequence) },
    { TOUCH_WALKING_INTO(FACING_WEST), 30, CONDITION_ALWAYS, EVENT_SCRIPT(Event_CheckAhead) },
    { TOUCH_WALKING_INTO(FACING_WEST), 28, CONDITION_ALWAYS, EVENT_SCRIPT(Event_RollEastLogWest) },
    { TOUCH_WALKING_INTO(FACING_EAST), 27, CONDITION_ALWAYS, EVENT_SCRIPT(Event_RollEastLogEast) },
    { TOUCH_WALKING_INTO(FACING_WEST), 24, CONDITION_ALWAYS, EVENT_SCRIPT(Event_RollWestLogWest) },
    { TOUCH_WALKING_INTO(FACING_EAST), 25, CONDITION_ALWAYS, EVENT_SCRIPT(Event_RollWestLogEast) },
    { TOUCH_WALKING, 22, CONDITION_ALWAYS, EVENT_SCRIPT(Event_RideLog) },
    { TOUCH_WALKING, 21, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushPillar) },
    { TOUCH_WALKING, 20, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushPillarAndUpdateCells) },
    { TOUCH_WALKING, 26, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushPillarAndUpdateCells) },
    { EVENT_TOUCH, 50, FLAG_SCENE_TEMP_10, EVENT_SCRIPT(Event_ReachArena) },
    { SEARCH(SEARCH_CHEST), 101, FLAG_PLACE_TEMP_38, FIND_ITEM(ITEM_NUT) },
    { SEARCH(SEARCH_CHEST) | EVENT_TOUCH_FACING | FACING_WEST, 102, FLAG_PLACE_TEMP_39, FIND_ITEM(ITEM_SMOKE_BOMB) },
    { SEARCH(SEARCH_CHEST), 104, FLAG_PLACE_TEMP_3B, FIND_ITEM(ITEM_NUT) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST) | 0x50000000, FROST_TARGET_WEST, FLAG_PLACE_TEMP_31, EVENT_SCRIPT(Event_FreezeWestTarget) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST) | 0x50000000, FROST_TARGET_EAST, FLAG_PLACE_TEMP_32, EVENT_SCRIPT(Event_FreezeEastTarget) },
    { PSYNERGY_ON_ACTOR(ABILITY_FORCE) | 0x10000000, FORCE_TARGET, FLAG_PLACE_TEMP_30, EVENT_SCRIPT(Event_PrepareForce) },
    { PSYNERGY_ON_ACTOR(ABILITY_FORCE), FORCE_TARGET, FLAG_PLACE_TEMP_30, EVENT_SCRIPT(Event_KnockDownObstacle) },
    { PSYNERGY_ON_ACTOR(ABILITY_GROWTH), GROWTH_PLANT, FLAG_PLACE_TEMP_33, EVENT_SCRIPT(Event_GrowPlant) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), BOARD_WALK_WEST_PILLAR, CONDITION_ALWAYS, EVENT_SCRIPT(Event_IgnoreMove) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), BOARD_WALK_EAST_PILLAR, CONDITION_ALWAYS, EVENT_SCRIPT(Event_IgnoreMove) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), WALL_PILLAR, CONDITION_ALWAYS, EVENT_SCRIPT(Event_UpdateMovedPillarCell) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), MINI_MAZE_FIRST_PILLAR, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SaveMovedPillarColumns) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), MINI_MAZE_FIRST_PILLAR + 1, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SaveMovedPillarColumns) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), MINI_MAZE_FIRST_PILLAR + 2, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SaveMovedPillarColumns) },
    { PSYNERGY_ON_ACTOR(ABILITY_HALT), LIFT_OPERATOR, FLAG_PLACE_TEMP_34, EVENT_SCRIPT(Event_HaltLifts) },
    { EVENT_TALK, GUIDE, CONDITION_ALWAYS, EVENT_SCRIPT(Colosso_AskReadyToRace) },
    { EVENT_TALK, MINI_MAZE_ATTENDANT, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToMiniMazeAttendant) },
    { EVENT_TALK, SHIFTING_FLOOR_ATTENDANT, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToShiftingFloorAttendant) },
    { EVENT_TALK, LOG_ROLLING_ATTENDANT, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToLogRollingAttendant) },
    { EVENT_TALK, WALL_ATTENDANT, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToWallAttendant) },
    { EVENT_TALK, BOARD_WALK_ATTENDANT, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToBoardWalkAttendant) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), GUIDE, CONDITION_ALWAYS, MSGNO(MsgColossoRiverThisYearsFinalsAreIncredibleThis) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), MINI_MAZE_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoWallICantBelieveAnybodyEverGets) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), SHIFTING_FLOOR_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoWallHowDoTheyMakeThatFloor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), LOG_ROLLING_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoWallImAmazedPeopleCanActuallyCross) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), WALL_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoWallBoyThatBitOverThereMust) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), BOARD_WALK_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoWallIHeardBabiGotTheLog) },
    { EVENT_RAISED, 99, CONDITION_ALWAYS, EVENT_SCRIPT(Colosso_TakePlaceForMatch) },
    { SCENE_EVENTS_END },
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

s32 gColosso_RivalFinishScript[] = {
    SC_ARG(2),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(6),
    SC_CALL(ColosseumSatrageMatch_RaiseLinkedSceneEffect),
    SC_WAIT(3),
    SC_ARG(10),
    SC_WAIT(150),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_ARG(1),
    SC_END,
};

s32 gColosseumSatrageMatch_ApproachScript[] = {
    SC_ARG(2),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(6),
    SC_CALL(ColosseumSatrageMatch_RaiseLinkedSceneEffect),
    SC_WAIT(3),
    SC_ARG(28),
    SC_WAIT(150),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_ARG(1),
    SC_END,
};
