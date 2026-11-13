/// @file
/// Colosseum Final 2 (SceneId_ColosseumSatrageMatch) is the site of the second
/// finals match, where Isaac races Satrage through five stages to the arena
/// and fights him there.
///
/// Isaac has won the first match, and Satrage is the fourth-ranked warrior.
///
/// It works as Colosseum Final 1 does (colosseum_azart_match.c), and is reached
/// by the Colosseum's west passage. The stages run from east to west, through
/// the Mini-Maze, the shifting floor stage, the Log-Rolling Stage, The Wall and
/// the Board Walk.
///
/// Isaac comes here three times, each at its own entrance.
/// ColosseumSatrageMatch_Initialize restores the stages from their temporary
/// flags (FLAG_OBSTACLE_KNOCKED_DOWN to FLAG_WALL_PILLAR_MOVED, and the
/// Mini-Maze pillars' columns from FLAG_MINI_MAZE_FIRST_PILLAR_COLUMN). Then
/// it sets up the visit its entrance stands for.
///
/// - COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_WEST, the tour
///   (ColosseumSatrageMatch_IntroduceSecondFinals). The guide only greets the
///   party, since the first finals match's guide explained the rules. The five
///   stages' attendants describe their stages and offer a place for each
///   friend to cheer. The guide then sends Isaac back to the west passage
///   (COLOSSEUM_SATRAGE_MATCH_TO_COLOSSEUM_SOUTH).
/// - COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_EAST, `Lend a Hand`. Garet, Ivan
///   and Mia in turn may use Psynergy on the stages. Move shifts the
///   Mini-Maze's stone pillars and the stone pillar by The Wall. Halt stops the
///   lifts' operator and with him the shifting floor. Frost freezes two puddles
///   on the Log-Rolling Stage. Growth grows a plant up The Wall. Force knocks
///   a log down onto the Board Walk. After Mia,
///   COLOSSEUM_SATRAGE_MATCH_LEND_A_HAND_TO_RACE starts the race.
/// - COLOSSEUM_SATRAGE_MATCH_RACE_FROM_LEND_A_HAND, the race against Satrage,
///   who follows a recorded path, after the lettering `The 2nd Match` and a
///   count of three. At the arena the first to arrive takes the Chain Mail and
///   the second the Cotton Shirt. The battle is ENCOUNTER_ZONE_COLOSSO's third
///   or fourth group, the Satrage with or without the mail.
///
/// A win returns to COLOSSEUM_SATRAGE_MATCH_AFTER_MATCH_WON and on to the
/// Colosseum (COLOSSEUM_SATRAGE_MATCH_TO_COLOSSEUM_NORTH, arriving at
/// COLOSSEUM_FROM_COLOSSEUM_SATRAGE_MATCH_NORTH), which sets
/// FLAG_COLOSSO_SECOND_MATCH_WON.
///
/// A loss returns to COLOSSEUM_SATRAGE_MATCH_AFTER_MATCH_LOST, and
/// COLOSSEUM_SATRAGE_MATCH_TO_BABI_PALACE wakes the party in Babi's Palace from
/// what it takes for a dream.
///
/// The shifting floor's four lifts run on every visit until Halt stops them
/// (ColosseumSatrageMatch_UpdateLifts). Every
/// COLOSSEUM_SATRAGE_MATCH_LIFT_CYCLE_FRAMES the phase flips. In one phase all
/// four lifts are lined up across the water. In the other, two are up and two
/// down, by turns.
///
/// Each frame the task copies cell behaviors into row 13 from column 58 and
/// into cell (58, 11). It makes one pair of copies by phase once the last lift
/// has stopped, another while it moves. Each copied cell is a wall, a jump
/// across or open ground, as the lifts line up or part.

#include "field/script_operands.h"
#include "system/main_routine_work.h"
#include "system/io_write_queue.h"
#include "scene_ids.h"
#include "system/vram_cache.h"
#include "scene/import.h"
#include "types.h"
#include "field/scene_tables.h"

s32 ColosseumSatrageMatch_Initialize(void);
// UB: the scene calls this before its definition with no prototype, as a
// function returning int.
#ifdef UBFIX
void ColosseumSatrageMatch_StartRaceGauge(s32 leaderId, s32 opponentId,
                                          s32 segments, s32 finishX,
                                          s32 finishZ, s32 firstItem,
                                          s32 secondItem);
#endif
extern const u8 gColosseumSatrageMatch_RollLogScript[];
extern const u8 gColosso_LetteringPaletteOffsets[];
extern const s16 gColosso_LetteringDigit[];
extern const s16 gColosso_LetteringLose[];
extern char gHexDigits[20];
extern s32 gColosso_PrizeSparkleScript[];
extern s32 gColosso_PrizeScript[];
extern u32 gPushedActor_DirectionSteps[];
extern struct SceneEntrance gColosseumSatrageMatchEntrances[];
extern u32 gColosseumSatrageMatchExits[];
extern struct ActorPlacement gColosseumSatrageMatchActorPlacements[];
extern s32 gColosseumSatrageMatch_LiftPhase;
extern s32 gColosseumSatrageMatch_LiftTimer;
extern struct Trigger gColosseumSatrageMatchTriggers[];
extern s16 gColosso_LetteringSlot;
extern s16 gColosso_LetteringTitle[];
extern s16 gColosso_LetteringGo[];
extern s16 gColosso_LetteringWin[];
extern s16 gColosso_MarkerSlot;
extern s32 gColosso_OpponentFinishScript[];
extern s32 gColosso_LeaderFinishScript[];

#include "scene/colosso_solo.h"
#include "scene/colosso_marker.h"
#include "scene/colosso_setup.h"
#include "scene/colosso_occupant.h"
#include "field/field_sequence.h"
#include "field/pushed_actor.h"
#include "system/main_routine.h"
#include "field/event_commands.h"
#include "game/game.h"
#include "window/window.h"
#include "window/screens.h"
#include "sound/sound.h"
#include "system/system.h"
#include "battle/battle.h"
#include "system/input.h"
#include "debug.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "field/field_psynergy_work.h"
#include "field/event.h"
#include "resource_ids.h"
#include "field/event_inline.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "field/actor_script.h"
#include "system/heap_slots.h"
#include "scene/colosso_gauge.h"
#include "flags.h"
#include "sound/sound_driver_symbols.h"
#include "macros.h"

#define COLOSSEUM_SATRAGE_MATCH_LIFT_CYCLE_FRAMES 120
#define COLOSSEUM_SATRAGE_MATCH_LIFT_CROSSING_FRAME 75
#define COLOSSEUM_SATRAGE_MATCH_DEMONSTRATION_TIMEOUT_FRAMES 600
#define COLOSSEUM_SATRAGE_MATCH_LOG_TRAVEL_PIXELS 64

// UB: westward log travel passes -64, which cannot be shifted left as signed C.
// Both callers supply only -64, 0 or 64, so fixed-point multiplication fits.
#ifdef UBFIX
#define COLOSSEUM_SATRAGE_MATCH_OFFSET_FIXED(value) ((value) * px(1))
#else
#define COLOSSEUM_SATRAGE_MATCH_OFFSET_FIXED(value) ((value) << 16)
#endif

// UB: each of these calls a void function through an s32 function type, the
// call the bytes show; UBFIX calls it as declared.
#ifdef UBFIX
#define COLOSSEUM_SATRAGE_MATCH_SHOW_MESSAGE Event_ShowMessageAndWait
#define COLOSSEUM_SATRAGE_MATCH_MOVE_ACTOR_PAIR                                \
  ColosseumSatrageMatch_MovePusherAndLog
#else
#define COLOSSEUM_SATRAGE_MATCH_SHOW_MESSAGE                                   \
  ((s32 (*)())Event_ShowMessageAndWait)
#define COLOSSEUM_SATRAGE_MATCH_MOVE_ACTOR_PAIR                                \
  ((s32 (*)())ColosseumSatrageMatch_MovePusherAndLog)
#endif

static void ColosseumSatrageMatch_IntroduceSecondFinals(s32 attendantId);
static void Colosso_ShowOpeningLettering(s32 match);
static void Colosso_ShowMatchResult(s32 result);
static void Colosso_NoOp(void);
static void Colosso_WaitForOpponentToFinish(void);
static s32 Colosso_TakePrize(s32 firstItem, s32 secondItem);
static void ColosseumSatrageMatch_StartOpponentPath(s32 pathResource);
static void Colosso_PushActorAhead(void);

/// The second finals match's actors, numbered in the order of
/// gColosseumSatrageMatchActorPlacements from ACTOR_FIRST_PLACED.
///
/// The rows of Isaac, Garet, Ivan and Mia take their own actors.
///
/// By the guide's and attendants' explanations, the Mini-Maze, the shifting
/// floor stage, the Log-Rolling Stage, The Wall and the Board Walk lead to the
/// arena. The stone pillars are drawn as the stone pillars Move pushes
/// elsewhere.
enum ColosseumSatrageMatchActorId {
  /// Satrage, racing Isaac to the arena.
  ACTOR_OPPONENT = ACTOR_FIRST_PLACED,
  /// The Board Walk's two stone pillars, on which Move does nothing.
  ACTOR_BOARD_WALK_WEST_PILLAR,
  ACTOR_BOARD_WALK_EAST_PILLAR,
  /// The stone pillar by The Wall, pushed or moved with Move.
  ACTOR_WALL_PILLAR,
  /// The Mini-Maze's three stone pillars, pushed or moved with Move.
  /// ColosseumSatrageMatch_Initialize stands them west to east, at columns 73,
  /// 76 and 79, until they have been moved.
  ACTOR_MINI_MAZE_FIRST_PILLAR,
  /// The Board Walk's two logs, the western and the eastern, in the room of its
  /// stone pillars
  /// (https://alchemy.pascalpixel.com/places/colosseum-satrage-match/map-1.png).
  /// A third, ACTOR_ROLLING_LOG_CENTER, is placed at (0, 0) and never moved
  /// there. ColosseumSatrageMatch_UnusedRollCenterLogNorth, its South twin and
  /// its row in the collision rebuild remain for it.
  ACTOR_ROLLING_LOG_WEST = ACTOR_MINI_MAZE_FIRST_PILLAR + 3,
  ACTOR_ROLLING_LOG_EAST,
  ACTOR_ROLLING_LOG_CENTER,
  /// The log Force strikes to knock ACTOR_FALLING_LOG down.
  ACTOR_FORCE_TARGET,
  /// The plant Growth grows up The Wall.
  ACTOR_GROWTH_PLANT,
  /// The Log-Rolling Stage's two puddles, which Frost freezes. They are drawn
  /// as the puddle Frost freezes in Lunpa (lunpa.h's SPRITE_PUDDLE).
  ACTOR_PUDDLE_WEST,
  ACTOR_PUDDLE_EAST,
  /// The shifting floor stage's four lifts, from west to east, which the
  /// lifts' operator runs.
  ACTOR_COLOSSEUM_SATRAGE_MATCH_FIRST_LIFT,
  ACTOR_COLOSSEUM_SATRAGE_MATCH_LAST_LIFT =
          ACTOR_COLOSSEUM_SATRAGE_MATCH_FIRST_LIFT +
      3,
  /// Four platforms in the lifts' sprite laid across the water channel at the
  /// Board Walk's south edge, drawn underfoot. Nothing but
  /// ColosseumSatrageMatch_Initialize touches them.
  ACTOR_BOARD_WALK_FIRST_PLATFORM,
  /// Knocked down onto the Board Walk by Force on ACTOR_FORCE_TARGET.
  ACTOR_FALLING_LOG = ACTOR_BOARD_WALK_FIRST_PLATFORM + 4,
  /// A young man with fair hair who works the shifting floor stage's lifts.
  ACTOR_LIFT_OPERATOR,
  /// The Log-Rolling Stage's two logs.
  ACTOR_RIDING_LOG_NORTH,
  ACTOR_RIDING_LOG_SOUTH,
  /// A soldier who greets the party at the second finals match.
  ACTOR_GUIDE,
  /// The stages' attendants, young men with fair hair in white tunics.
  ACTOR_MINI_MAZE_ATTENDANT,
  ACTOR_SHIFTING_FLOOR_ATTENDANT,
  ACTOR_LOG_ROLLING_ATTENDANT,
  ACTOR_WALL_ATTENDANT,
  ACTOR_BOARD_WALK_ATTENDANT,
  /// The arena's prizes, drawn as item icons at two blank actors
  /// (Colosso_ShowItemIcon).
  ACTOR_PRIZE_A,
  ACTOR_PRIZE_B
};

/// The temporary flags that keep what the friends changed on the stages, for
/// the race.
///
/// ColosseumSatrageMatch_Initialize restores them.
enum {
  FLAG_OBSTACLE_KNOCKED_DOWN =
      FLAG_AREA_TEMP_30,                       ///< by Force, on the Board Walk
  FLAG_WEST_TARGET_FROZEN = FLAG_AREA_TEMP_31, ///< by Frost
  FLAG_EAST_TARGET_FROZEN = FLAG_AREA_TEMP_32, ///< by Frost
  FLAG_PLANT_GROWN = FLAG_AREA_TEMP_33,        ///< by Growth, up The Wall
  FLAG_LIFTS_HALTED = FLAG_AREA_TEMP_34, ///< by Halt on ACTOR_LIFT_OPERATOR
  /// The stone pillar by The Wall stands at column 36.
  FLAG_WALL_PILLAR_MOVED = FLAG_AREA_TEMP_35,
  /// The bytes of flags that keep the Mini-Maze's three pillars' columns.
  FLAG_MINI_MAZE_FIRST_PILLAR_COLUMN = FLAG_AREA_TEMP_40,
  FLAG_MINI_MAZE_SECOND_PILLAR_COLUMN = FLAG_AREA_TEMP_48,
  FLAG_MINI_MAZE_THIRD_PILLAR_COLUMN = FLAG_AREA_TEMP_50,
  /// Someone has reached the arena
  /// (Colosso_TakePrize).
  FLAG_ARENA_REACHED = FLAG_SCENE_TEMP_11
};

/// SPRITE_FALLING_LOG's animations (falling_log.tsv).
enum FallingLogAnimation {
  FALLING_LOG_ANIMATION_STANDING = 3, ///< standing on end
  /// Lying across the water, bobbing.
  FALLING_LOG_ANIMATION_FALLEN = 4
};

const struct SceneEntrance *Scene_GetEntrances(void) {
  return gColosseumSatrageMatchEntrances;
}

const struct SceneExitRegion *Scene_GetExitRegions(void) { return 0; }

/// Returns the packed stage and battle-result exits.
const u32 *Scene_GetExits(void) { return gColosseumSatrageMatchExits; }

/// Returns Colosseum Final 2's warriors, obstacles and attendants.
const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gColosseumSatrageMatchActorPlacements;
}

/// Move on the Board Walk's two stone pillars does nothing.
static void ColosseumSatrageMatch_IgnoreMove(void) {}

/// Walking into a stone pillar pushes it.
static void ColosseumSatrageMatch_PushPillar(void) { Colosso_PushActorAhead(); }

/// Move on the stone pillar by The Wall, and each push, keeps where it stands.
static void ColosseumSatrageMatch_MoveWallPillar(void) {
  struct Actor *obstacle;
  s32 column;
  s32 row;

  obstacle = Actor_Get(ACTOR_WALL_PILLAR);
  if (FIELD_CELL(obstacle->x.fixed) == 36) {
    GameFlag_Set(FLAG_WALL_PILLAR_MOVED);
    column = 35;
    row = 77;
    MapGrid_CopyCellAttributes(35, 78, 1, 1, column, row);
  } else {
    GameFlag_Clear(FLAG_WALL_PILLAR_MOVED);
    column = 35;
    row = 77;
    MapGrid_CopyCellAttributes(34, 77, 1, 1, column, row);
  }
}

/// Walking into the stone pillar by The Wall (step triggers 20 and 26) pushes
/// it, then updates its cell.
static void ColosseumSatrageMatch_PushWallPillar(void) {
  Colosso_PushActorAhead();
  ColosseumSatrageMatch_MoveWallPillar();
}

/// Move on a stone pillar in the Mini-Maze, and each push, keeps where the
/// pillars stand.
static void ColosseumSatrageMatch_MoveMiniMazePillar(void) {
  struct Actor *obstacle;
  s32 column;

  MapGrid_CopyCellAttributes(100, 11, 12, 4, 14, 11);

  obstacle = Actor_Get(ACTOR_MINI_MAZE_FIRST_PILLAR);
  column = FIELD_CELL(obstacle->x.fixed);
  FieldGameFlag_SetByte(FLAG_MINI_MAZE_FIRST_PILLAR_COLUMN, column);
  MapGrid_CopyCellAttributes(71, 16, 1, 1, column, 16);

  obstacle = Actor_Get(ACTOR_MINI_MAZE_FIRST_PILLAR + 1);
  column = FIELD_CELL(obstacle->x.fixed);
  FieldGameFlag_SetByte(FLAG_MINI_MAZE_SECOND_PILLAR_COLUMN, column);
  MapGrid_CopyCellAttributes(71, 16, 1, 1, column, 16);

  obstacle = Actor_Get(ACTOR_MINI_MAZE_FIRST_PILLAR + 2);
  column = FIELD_CELL(obstacle->x.fixed);
  FieldGameFlag_SetByte(FLAG_MINI_MAZE_THIRD_PILLAR_COLUMN, column);
  MapGrid_CopyCellAttributes(71, 16, 1, 1, column, 16);
}

/// Force on the target over the Board Walk, before the Psynergy plays, gives
/// the Psynergy its child mode.
static void ColosseumSatrageMatch_PrepareForce(void) {
  u8 *effectWork = gFieldEffectWork;

  ((struct FieldEffectWork *)effectWork)->childMode = 1;
}

/// Force on the target over the Board Walk knocks the obstacle down.
static void ColosseumSatrageMatch_KnockDownObstacle(void) {
  struct Actor *obstacle;
  s32 fallFrames;
  s32 rotationStep;
  s32 settleFrames;
  s32 settleRotationStep;
  struct Sprite *sprite;

  obstacle = Actor_Get(ACTOR_FALLING_LOG);
  sprite = obstacle->sprite;
  Event_SetFlag(FLAG_OBSTACLE_KNOCKED_DOWN);
  obstacle->acceleration = px(1) / 10;
  obstacle->speedLimit = px(6) / 5;
  Sound_Play(SE_TOPPLE);
  FieldActor_SetMoveTarget(obstacle, 336, 10, 264);
  for (rotationStep = 0, fallFrames = 9; fallFrames >= 0; fallFrames--) {
    sprite->rotation -= rotationStep;
    wait(1);
    rotationStep += 36;
  }
  FieldActor_SetMoveTarget(obstacle, 330, -16, 264);
  for (settleRotationStep = 360, settleFrames = 21; settleFrames >= 0;
       settleFrames--) {
    sprite->rotation -= settleRotationStep;
    wait(1);
    settleRotationStep += 36;
  }
  Actor_WaitForMove(obstacle);
  Event_Wait(2);
  Sound_Play(SE_LOG_FALL);
  sprite->rotation = 0;
  Actor_SetAnimation(ACTOR_FALLING_LOG, FALLING_LOG_ANIMATION_FALLEN);
  obstacle->x.fixed = px(336);
  obstacle->y.fixed = -px(8);
  obstacle->z.fixed = px(264);
  obstacle->velocityY = 0;
  obstacle->velocityX = 0;
  MapGrid_CopyCellAttributes(19, 16, 1, 1, 20, 16);
  MapGrid_CopyCellAttributes(20, 80, 1, 1, 21, 80);
}

/// Frost on the Log-Rolling Stage's western target freezes it and opens its
/// crossing (FLAG_WEST_TARGET_FROZEN).
static void ColosseumSatrageMatch_FreezeWestTarget(void) {
  struct Actor *obstacle;

  GameFlag_Set(FLAG_WEST_TARGET_FROZEN);
  obstacle = Actor_Get(ACTOR_PUDDLE_WEST);
  obstacle->motionFlags = 0;
  MapGrid_CopyCellAttributes(46, 17, 1, 1, 44, 17);
}

/// Frost on the Log-Rolling Stage's eastern target freezes it and opens its
/// crossing (FLAG_EAST_TARGET_FROZEN).
static void ColosseumSatrageMatch_FreezeEastTarget(void) {
  struct Actor *obstacle;

  GameFlag_Set(FLAG_EAST_TARGET_FROZEN);
  obstacle = Actor_Get(ACTOR_PUDDLE_EAST);
  obstacle->motionFlags = 0;
  MapGrid_CopyCellAttributes(46, 17, 1, 1, 50, 17);
}

/// Growth on the plant by The Wall grows it into a strip that can be climbed
/// (FLAG_PLANT_GROWN).
static void ColosseumSatrageMatch_GrowPlant(void) {
  GameFlag_Set(FLAG_PLANT_GROWN);
  MapGrid_CopyCellAttributes(32, 37, 1, 4, 32, 77);
}

/// The selected actor jumps down off the ledge by an obstacle.
static void ColosseumSatrageMatch_JumpDownLedge(void) {
  FieldActor_JumpDownLedge(gMainRoutineWork.leaderActorId, 6, 0);
}

/// Walking west into the obstacle at step trigger 30 in Colosseum Final 2
/// makes the leader jump forward where he can (Field_TryJumpForward).
static void ColosseumSatrageMatch_JumpWestFromLedge(void) {
  Field_TryJumpForward();
}

/// Moves a riding log and its pusher to aligned cell centers.
static void ColosseumSatrageMatch_MovePusherAndLog(s32 logId, s32 offsetX,
                                                   s32 offsetZ) {
  struct Actor *pusher;
  struct Actor *log;
  s32 destinationX;
  s32 destinationZ;

  pusher = Actor_Get(gMainRoutineWork.leaderActorId);
  log = Actor_Get(logId);
  Event_Start();
  {
    destinationX =
        ((pusher->x.fixed + COLOSSEUM_SATRAGE_MATCH_OFFSET_FIXED(offsetX)) &
         FIELD_CELL_MASK) +
        px(8);
    destinationZ =
        ((pusher->z.fixed + COLOSSEUM_SATRAGE_MATCH_OFFSET_FIXED(offsetZ)) &
         FIELD_CELL_MASK) +
        px(8);

    pusher->speedLimit = px(1);
    pusher->acceleration = px(1) / 2;
    FieldActor_SetMoveTargetFixed(pusher, destinationX, pusher->y.fixed,
                                  destinationZ);
  }
  Actor_SelectAnimation(pusher, 27);
  {
    destinationX =
        ((log->x.fixed + COLOSSEUM_SATRAGE_MATCH_OFFSET_FIXED(offsetX)) &
         FIELD_CELL_MASK) +
        px(8);
    destinationZ =
        ((log->z.fixed + COLOSSEUM_SATRAGE_MATCH_OFFSET_FIXED(offsetZ)) &
         FIELD_CELL_MASK) +
        px(8);

    log->speedLimit = px(1);
    log->acceleration = px(1) / 2;
    FieldActor_SetMoveTargetFixed(log, destinationX, log->y.fixed,
                                  destinationZ);
  }
  if (offsetX < 0 || offsetZ < 0) {
    Actor_SelectAnimation(log, 4);
  } else {
    Actor_SelectAnimation(log, 3);
  }
  Sound_Play(SE_LOG_PUSH);
  Actor_WaitForMove(pusher);
  Sound_Play(SE_STOP_ACTION_SE);
  Actor_SelectAnimation(log, 2);
  Event_End();
}

/// Standing on one of the Log-Rolling Stage's logs (step trigger 22) carries
/// the rider across the water the way the player holds.
static void ColosseumSatrageMatch_RideLog(void) {
  struct Actor *pusher = Actor_Get(gMainRoutineWork.leaderActorId);
  s32 column = FIELD_CELL(pusher->x.fixed);
  s32 offsetX = 0;
  s32 logId = ACTOR_RIDING_LOG_NORTH;

  if (FIELD_CELL(pusher->z.fixed) > 12) {
    logId = ACTOR_RIDING_LOG_SOUTH;
  }
  if (FIELD_CELL(Actor_Get(logId)->x.fixed) != column) {
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
    MapGrid_CopyCellAttributes(120, 10, 5, 6, 48, 10);
    column = FIELD_CELL(Actor_Get(ACTOR_RIDING_LOG_NORTH)->x.fixed);
    MapGrid_CopyCellAttributes(52, 28, 1, 3, column, 10);
    column = FIELD_CELL(Actor_Get(ACTOR_RIDING_LOG_SOUTH)->x.fixed);
    MapGrid_CopyCellAttributes(52, 28, 1, 3, column, 13);
  }
}

/// Rolls a log to half-cell coordinates while its pusher follows halfway.
static void ColosseumSatrageMatch_RollLogToCell(s32 logId, s32 halfCellX,
                                                s32 halfCellZ) {
  s32 pusher = gMainRoutineWork.leaderActorId;
  struct Actor *pusherActor = Actor_Get(pusher);
  struct Actor *log = Actor_Get(logId);
  s32 heading = (pusherActor->facing + TRIG_TURN / 16) & 0xe000;
  s32 alongX = FIELD_CELL(log->x.fixed) != halfCellX / 2;
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
  Event_Start();
  Actor_SetAnimation(pusher, ANIM_PUSH);
  Event_Wait(6);
  log->speedLimit = px(1) / 2;
  log->acceleration = px(1) / 5;
  Actor_SelectAnimation(
      log, gColosseumSatrageMatch_RollLogScript[heading / (TRIG_TURN / 4)]);
  FieldActor_SetMoveTargetFixed(log, halfCellX, 0, halfCellZ);
  Event_Wait(6);
  Actor_SetAnimation(pusher, ANIM_WALK);
  // Follow halfway behind the rolling log while its finish effect runs.
  Actor_StartCamera(((struct FieldWork *)HeapSlot_AllocEwram(
                         HEAP_SLOT_FIELD, sizeof(struct FieldWork)))
                        ->camera,
                    (s32)log);
  Actor_SetSpeedAndAcceleration(pusher, 0.5, 0.2);
  Actor_SelectAnimation(pusherActor, ANIM_WALK);
  FieldActor_SetMoveTargetFixed(pusherActor, pusherActor->x.fixed + offsetX, 0,
                                pusherActor->z.fixed + offsetZ);
  Sound_Play(SE_HEAVY_MOVE);
  Actor_WaitForMove(pusherActor);
  Actor_SelectAnimation(pusherActor, ANIM_STAND);
  Actor_WaitForMove(log);
  Sound_Play(SE_STOP_ACTION_SE);
  Sound_Play(SE_HEAVY_STOP);
  Actor_SelectAnimation(log, 1);
  Event_Wait(15);
  Event_End();
}

/// Rebuilds the cell behaviors under the three rolling logs at their
/// positions.
static void ColosseumSatrageMatch_UpdateRollingLogCollision(void) {
  s32 cell;

  MapGrid_CopyCellAttributes(100, 11, 12, 4, 14, 11);

  cell = FIELD_CELL(Actor_Get(ACTOR_ROLLING_LOG_WEST)->x.fixed);
  MapGrid_CopyCellAttributes(13, 28, 1, 4, cell, 11);

  cell = FIELD_CELL(Actor_Get(ACTOR_ROLLING_LOG_EAST)->x.fixed);
  MapGrid_CopyCellAttributes(13, 28, 1, 4, cell, 11);

  cell = FIELD_CELL(Actor_Get(ACTOR_ROLLING_LOG_CENTER)->z.fixed);
  MapGrid_CopyCellAttributes(13, 28, 4, 1, 18, cell);
}

/// Walking west into the Board Walk's western log rolls it west.
static void ColosseumSatrageMatch_RollWestLogWest(void) {
  ColosseumSatrageMatch_RollLogToCell(ACTOR_ROLLING_LOG_WEST, 29, 26);
  ColosseumSatrageMatch_UpdateRollingLogCollision();
}

/// Walking east into the Board Walk's western log (step trigger 25) rolls it
/// east.
static void ColosseumSatrageMatch_RollWestLogEast(void) {
  ColosseumSatrageMatch_RollLogToCell(ACTOR_ROLLING_LOG_WEST, 33, 26);
  ColosseumSatrageMatch_UpdateRollingLogCollision();
}

/// Walking west into the Board Walk's eastern log (step trigger 28) rolls it
/// west.
static void ColosseumSatrageMatch_RollEastLogWest(void) {
  ColosseumSatrageMatch_RollLogToCell(ACTOR_ROLLING_LOG_EAST, 45, 26);
  ColosseumSatrageMatch_UpdateRollingLogCollision();
}

/// Walking east into the Board Walk's eastern log (step trigger 27) rolls it
/// east.
static void ColosseumSatrageMatch_RollEastLogEast(void) {
  ColosseumSatrageMatch_RollLogToCell(ACTOR_ROLLING_LOG_EAST, 49, 26);
  ColosseumSatrageMatch_UpdateRollingLogCollision();
}

/// Unused: no trigger row calls it.
///
/// It would have rolled ACTOR_ROLLING_LOG_CENTER, placed at (0, 0) and never
/// moved to the Board Walk, to a target 16 pixels north of its South twin's.
/// The western and eastern logs roll west and east the same way
/// (ColosseumSatrageMatch_RollWestLogWest and the rest).
static UNUSED void ColosseumSatrageMatch_UnusedRollCenterLogNorth(void) {
  ColosseumSatrageMatch_RollLogToCell(ACTOR_ROLLING_LOG_CENTER, 40, 23);
  ColosseumSatrageMatch_UpdateRollingLogCollision();
}

/// Unused: as ColosseumSatrageMatch_UnusedRollCenterLogNorth, rolling
/// ACTOR_ROLLING_LOG_CENTER, which is not on the Board Walk, to the target 16
/// pixels south of that one's.
static UNUSED void ColosseumSatrageMatch_UnusedRollCenterLogSouth(void) {
  ColosseumSatrageMatch_RollLogToCell(ACTOR_ROLLING_LOG_CENTER, 40, 25);
  ColosseumSatrageMatch_UpdateRollingLogCollision();
}

/// The shifting floor's task carries the selected actor on the lifts.
static void ColosseumSatrageMatch_UpdateLifts(void) {
  s32 liftId;
  struct Actor *rider;
  struct Actor *lift;
  s32 liftX;
  s32 riderX;
  s32 deltaX;
  s32 liftZ;
  s32 riderZ;
  s32 deltaZ;
  struct Actor *liftActor;

  rider = Actor_Get(gMainRoutineWork.leaderActorId);
  // Carry a rider only while both horizontal distances are less than ten
  // pixels.
  for (liftId = ACTOR_COLOSSEUM_SATRAGE_MATCH_FIRST_LIFT;
       liftId <= ACTOR_COLOSSEUM_SATRAGE_MATCH_LAST_LIFT; liftId++) {
    lift = Actor_Get(liftId);
    lift->paused = 0;
    liftX = lift->x.fixed;
    riderX = rider->x.fixed;
    deltaX = liftX - riderX;
    if (deltaX >= 0) {
      if (deltaX >= px(10)) {
        continue;
      }
    } else {
      riderX -= liftX;
      if (riderX >= px(10)) {
        continue;
      }
    }
    liftZ = lift->z.fixed;
    riderZ = rider->z.fixed;
    deltaZ = liftZ - riderZ;
    if (deltaZ >= 0) {
      if (deltaZ >= px(10)) {
        continue;
      }
    } else {
      riderZ -= liftZ;
      if (riderZ >= px(10)) {
        continue;
      }
    }
    if (GameFlag_Get(FLAG_FIELD_HANDLING_REQUESTS) != 0) {
      riderZ = lift->z.fixed;
    } else {
      riderZ = rider->z.fixed;
      deltaZ = lift->velocityZ;
      riderZ += deltaZ;
    }
    rider->z.fixed = riderZ;
  }
  // The final lift's destination selects the cell behaviors for the row.
  if (gColosseumSatrageMatch_LiftTimer != 0 &&
      lift->targetX == ACTOR_NO_TARGET) {
    if (gColosseumSatrageMatch_LiftPhase == 0) {
      MapGrid_CopyCellAttributes(58, 28, 7, 1, 58, 13);
    } else {
      MapGrid_CopyCellAttributes(58, 10, 1, 1, 58, 11);
    }
  } else {
    MapGrid_CopyCellAttributes(57, 11, 1, 1, 58, 11);
    MapGrid_CopyCellAttributes(58, 14, 7, 1, 58, 13);
  }
  if (gColosseumSatrageMatch_LiftTimer == 0) {
    gColosseumSatrageMatch_LiftPhase ^= 1;
    if (gColosseumSatrageMatch_LiftPhase != 0) {
      liftActor = Actor_Get(ACTOR_COLOSSEUM_SATRAGE_MATCH_FIRST_LIFT);
      FieldActor_SetMoveTarget(liftActor, 936, 0, 184);
      liftActor = Actor_Get(ACTOR_COLOSSEUM_SATRAGE_MATCH_FIRST_LIFT + 1);
      FieldActor_SetMoveTarget(liftActor, 968, 0, 248);
      liftActor = Actor_Get(ACTOR_COLOSSEUM_SATRAGE_MATCH_FIRST_LIFT + 2);
      FieldActor_SetMoveTarget(liftActor, 1000, 0, 184);
      liftActor = Actor_Get(ACTOR_COLOSSEUM_SATRAGE_MATCH_LAST_LIFT);
      FieldActor_SetMoveTarget(liftActor, 1032, 0, 248);
      Actor_SetAnimation(ACTOR_LIFT_OPERATOR, 11);
    } else {
      liftActor = Actor_Get(ACTOR_COLOSSEUM_SATRAGE_MATCH_FIRST_LIFT);
      FieldActor_SetMoveTarget(liftActor, 936, 0, 216);
      liftActor = Actor_Get(ACTOR_COLOSSEUM_SATRAGE_MATCH_FIRST_LIFT + 1);
      FieldActor_SetMoveTarget(liftActor, 968, 0, 216);
      liftActor = Actor_Get(ACTOR_COLOSSEUM_SATRAGE_MATCH_FIRST_LIFT + 2);
      FieldActor_SetMoveTarget(liftActor, 1000, 0, 216);
      liftActor = Actor_Get(ACTOR_COLOSSEUM_SATRAGE_MATCH_LAST_LIFT);
      FieldActor_SetMoveTarget(liftActor, 1032, 0, 216);
      Actor_SetAnimation(ACTOR_LIFT_OPERATOR, 10);
    }
  }
  gColosseumSatrageMatch_LiftTimer++;
  if ((u32)gColosseumSatrageMatch_LiftTimer >=
      COLOSSEUM_SATRAGE_MATCH_LIFT_CYCLE_FRAMES) {
    if (GameFlag_Get(FLAG_FIELD_HANDLING_REQUESTS) == 0) {
      gColosseumSatrageMatch_LiftTimer = 0;
    }
  }
}

/// Stops the moving-lift task and restores its starting row.
static void ColosseumSatrageMatch_ResetLifts(void) {
  gColosseumSatrageMatch_LiftTimer = 0;
  gColosseumSatrageMatch_LiftPhase = 0;
  Task_Remove(ColosseumSatrageMatch_UpdateLifts);
  Actor_SetPosition(ACTOR_COLOSSEUM_SATRAGE_MATCH_FIRST_LIFT, 936, 216);
  Actor_SetPosition(ACTOR_COLOSSEUM_SATRAGE_MATCH_FIRST_LIFT + 1, 968, 216);
  Actor_SetPosition(ACTOR_COLOSSEUM_SATRAGE_MATCH_FIRST_LIFT + 2, 1000, 216);
  Actor_SetPosition(ACTOR_COLOSSEUM_SATRAGE_MATCH_LAST_LIFT, 1032, 216);
  Actor_SetAnimation(ACTOR_LIFT_OPERATOR, 10);
}

/// Starts the moving-lift task and releases its pause flag.
static void ColosseumSatrageMatch_StartLifts(void) {
  Task_AddFrame(ColosseumSatrageMatch_UpdateLifts, TASK_PASS_FRAME + 5);
  GameFlag_Clear(FLAG_FIELD_HANDLING_REQUESTS);
}

/// Waits, for up to ten seconds, until the four lifts stand lined up across
/// the water, for the demonstration to jump onto them.
static void ColosseumSatrageMatch_WaitForLiftsLinedUp(void) {
  s32 elapsedFrames;

  wait(10);

  elapsedFrames = 0;
  while (gColosseumSatrageMatch_LiftPhase != 0 ||
         gColosseumSatrageMatch_LiftTimer !=
             COLOSSEUM_SATRAGE_MATCH_LIFT_CROSSING_FRAME) {
    wait(1);
    elapsedFrames++;
    if (elapsedFrames >= COLOSSEUM_SATRAGE_MATCH_DEMONSTRATION_TIMEOUT_FRAMES) {
      return;
    }
  }
}

/// Halt on the shifting floor's lift operator stops the lifts and leaves a
/// crossing (FLAG_LIFTS_HALTED).
static void ColosseumSatrageMatch_HaltLifts(void) {
  FieldPsynergy_HaltActor(ACTOR_LIFT_OPERATOR);
  GameFlag_Set(FLAG_LIFTS_HALTED);

  if (gColosseumSatrageMatch_LiftPhase != 0) {
    gColosseumSatrageMatch_LiftTimer = 0;
  }

  wait(30);
  wait(1);

  Task_Remove(ColosseumSatrageMatch_UpdateLifts);

  MapGrid_CopyCellAttributes(58, 28, 7, 1, 58, 13);
  MapGrid_CopyCellAttributes(57, 11, 1, 1, 58, 11);
}

/// Reaching Colosseum Final 2's arena, the leader takes a prize and fights
/// Satrage.
static void ColosseumSatrageMatch_ReachArena(void) {
  s32 opponentFinishedFirst;
  s32 refreshCount;
  s32 resultBase = 2;
  Colosso_NoOp();
  Event_Start();
  opponentFinishedFirst = Colosso_TakePrize(ITEM_CHAIN_MAIL, ITEM_COTTON_SHIRT);
  Colosso_WaitForOpponentToFinish();
  refreshCount = 9;
  do {
    Actor_WaitForScriptEndById(ACTOR_OPPONENT);
    refreshCount = refreshCount - 1;
  } while (refreshCount >= 0);
  Actor_SetSpeed(ACTOR_OPPONENT, 1);
  Actor_WalkTo(ACTOR_OPPONENT, 88, 256);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 1);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 120, 256);
  Actor_SetAnimation(ACTOR_OPPONENT, ANIM_STAND);
  Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_OPPONENT, 0);
  Event_Wait(10);
  Actor_SetAnimation(ACTOR_OPPONENT, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(20);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 2);
  Actor_SetSpeed(ACTOR_OPPONENT, 2);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 112, 256);
  Actor_WalkToAndWait(ACTOR_OPPONENT, 96, 256);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_STRIDE);
  Actor_SetAnimation(ACTOR_OPPONENT, WARRIOR_ANIMATION_FIGHTING_STANCE);
  Event_Wait(10);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_COLOSSO,
                                        resultBase - opponentFinishedFirst + 1);
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  Event_SetAfterBattleDestination(SCENE_COLOSSEUM_SATRAGE_MATCH,
                                  COLOSSEUM_SATRAGE_MATCH_AFTER_MATCH_WON);
  Event_SetDefeatDestination(SCENE_COLOSSEUM_SATRAGE_MATCH,
                             COLOSSEUM_SATRAGE_MATCH_AFTER_MATCH_LOST);
  GameFlag_Set(FLAG_KEEP_MUSIC_ONCE);
}

/// Returns Colosseum Final 2's obstacle interactions and attendant dialogue.
const struct Trigger *Scene_GetTriggers(void) {
  return gColosseumSatrageMatchTriggers;
}

/// Back from the battle, Isaac and Satrage stand in the arena as the result is
/// shown.
static void ColosseumSatrageMatch_StageBattleResult(s32 result) {
  Actor_ReleaseById(ACTOR_PRIZE_A);
  Actor_ReleaseById(ACTOR_PRIZE_B);
  Party_RestoreHpPp(1);
  Event_Start();
  Actor_SetPosition(ACTOR_OPPONENT, 88, 256);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 120, 256);
  // BUG: FACING_SOUTH is passed where Actor_FaceActor takes an
  // actor id; Actor_Lookup has no actor past 191, so
  // neither turns.
#ifdef BUGFIX
  Actor_FaceDirection(ACTOR_OPPONENT, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
#else
  Actor_FaceActor(ACTOR_OPPONENT, FACING_SOUTH, 0);
  Actor_FaceActor(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
#endif
  if (result < 0) {
    Actor_SetAnimation(ACTOR_OPPONENT, WARRIOR_ANIMATION_ARMS_RAISED);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_CROUCH);
  } else {
    Actor_SetAnimation(ACTOR_OPPONENT, WARRIOR_ANIMATION_DEFEATED);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_ARMS_RAISED);
  }
  wait(1);
  FieldCamera_MoveTo(104, 0, 192, 0);
  Colosso_ShowMatchResult(result);
  Event_End();
}

/// Sets up Colosseum Final 2 for the visit its entrance stands for.
s32 ColosseumSatrageMatch_Initialize(void) {
  struct Actor *actor;
  s32 x;
  s32 obstacleId;
  s32 column;
  s32 liftId;
  s32 columnOffset;
  s32 row;

  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_FADE, 0);
  GameFlag_Set(FLAG_RETREAT_BLOCKED);
  MapGrid_CopyCellAttributes(14, 11, 12, 4, 100, 11);
  MapGrid_CopyCellAttributes(48, 10, 5, 6, 120, 10);
  for (obstacleId = ACTOR_BOARD_WALK_FIRST_PLATFORM;
       obstacleId <= ACTOR_FALLING_LOG; obstacleId++) {
    actor = Actor_Get(obstacleId);
    Actor_SelectAnimation(actor, 4);
    actor->motionFlags = 0;
    actor->y.fixed = 0;
    actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  }
  Actor_Get(ACTOR_FORCE_TARGET)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  if (GameFlag_Get(FLAG_OBSTACLE_KNOCKED_DOWN)) {
    actor = Actor_Get(ACTOR_FALLING_LOG);
    actor->x.fixed = px(336);
    actor->y.fixed = -px(8);
    actor->z.fixed = px(264);
    MapGrid_CopyCellAttributes(19, 16, 1, 1, 20, 16);
    MapGrid_CopyCellAttributes(20, 80, 1, 1, 21, 80);
  } else {
    actor = Actor_Get(ACTOR_FALLING_LOG);
    Actor_SelectAnimation(actor, FALLING_LOG_ANIMATION_STANDING);
    actor->y.fixed = px(16);
  }
  Actor_Get(ACTOR_WALL_PILLAR)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  // With the wall pillar moved, the cell it stood on takes the ground of the
  // cell south of it. The map stores the two alike, open ground, so the copy
  // only matters where the cell has been changed since the map loaded.
  if (GameFlag_Get(FLAG_WALL_PILLAR_MOVED)) {
    MapGrid_CopyCellAttributes(35, 78, 1, 1, 35, 77);
  }
  if (GameFlag_Get(FLAG_PLANT_GROWN)) {
    Actor_SetAnimation(ACTOR_GROWTH_PLANT, ANIM_SHAKE_HEAD);
    MapGrid_CopyCellAttributes(32, 37, 1, 4, 32, 77);
  }
  if (GameFlag_Get(FLAG_WEST_TARGET_FROZEN)) {
    Actor_Get(ACTOR_PUDDLE_WEST)->motionFlags = 0;
    Actor_Get(ACTOR_PUDDLE_WEST)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    Actor_SetAnimation(ACTOR_PUDDLE_WEST, PUDDLE_ANIMATION_PILLAR);
    MapGrid_CopyCellAttributes(46, 17, 1, 1, 44, 17);
  }
  if (GameFlag_Get(FLAG_EAST_TARGET_FROZEN)) {
    Actor_Get(ACTOR_PUDDLE_EAST)->motionFlags = 0;
    Actor_Get(ACTOR_PUDDLE_EAST)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    Actor_SetAnimation(ACTOR_PUDDLE_EAST, PUDDLE_ANIMATION_PILLAR);
    MapGrid_CopyCellAttributes(46, 17, 1, 1, 50, 17);
  }
  actor = Actor_Get(ACTOR_RIDING_LOG_NORTH);
  column = FIELD_CELL(actor->x.fixed);
  actor->motionFlags = 0;
  actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  MapGrid_CopyCellAttributes(52, 28, 1, 3, column, 10);
  actor = Actor_Get(ACTOR_RIDING_LOG_SOUTH);
  column = FIELD_CELL(actor->x.fixed);
  actor->motionFlags = 0;
  actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  MapGrid_CopyCellAttributes(52, 28, 1, 3, column, 13);

  // Each Mini-Maze pillar returns to the column its area temp flag byte
  // keeps.
  x = FieldGameFlag_GetByte(FLAG_MINI_MAZE_FIRST_PILLAR_COLUMN);
  if (x == 0) {
    x = 73;
  }
  actor = Actor_Get(ACTOR_MINI_MAZE_FIRST_PILLAR);
  actor->x.fixed = (x << 20) + px(8);
  actor->motionFlags = 0;
  actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  MapGrid_CopyCellAttributes(71, 16, 1, 1, x, 16);
  x = FieldGameFlag_GetByte(FLAG_MINI_MAZE_SECOND_PILLAR_COLUMN);
  if (x == 0) {
    x = 76;
  }
  actor = Actor_Get(ACTOR_MINI_MAZE_FIRST_PILLAR + 1);
  actor->x.fixed = (x << 20) + px(8);
  actor->motionFlags = 0;
  actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  MapGrid_CopyCellAttributes(71, 16, 1, 1, x, 16);
  x = FieldGameFlag_GetByte(FLAG_MINI_MAZE_THIRD_PILLAR_COLUMN);
  if (x == 0) {
    x = 79;
  }
  actor = Actor_Get(ACTOR_MINI_MAZE_FIRST_PILLAR + 2);
  actor->x.fixed = (x << 20) + px(8);
  actor->motionFlags = 0;
  actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  MapGrid_CopyCellAttributes(71, 16, 1, 1, x, 16);

  ColosseumSatrageMatch_UpdateRollingLogCollision();
  // With FLAG_LIFTS_HALTED set the lifts stay where they are; otherwise their
  // task resumes.
  Actor_SetAnimation(ACTOR_LIFT_OPERATOR, 10);
  if (GameFlag_Get(FLAG_LIFTS_HALTED)) {
    for (liftId = ACTOR_COLOSSEUM_SATRAGE_MATCH_FIRST_LIFT, row = 13,
        columnOffset = 58;
         liftId <= ACTOR_COLOSSEUM_SATRAGE_MATCH_LAST_LIFT;
         liftId++, columnOffset += 2) {
      actor = Actor_Get(liftId);
      actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
      Actor_SelectAnimation(actor, 4);
      MapGrid_CopyCellAttributes(56, 13, 1, 1, columnOffset, row);
    }
    Actor_SetAnimation(ACTOR_LIFT_OPERATOR, 10);
    FieldPsynergy_HaltActor(ACTOR_LIFT_OPERATOR);
  } else {
    for (liftId = ACTOR_COLOSSEUM_SATRAGE_MATCH_FIRST_LIFT;
         liftId <= ACTOR_COLOSSEUM_SATRAGE_MATCH_LAST_LIFT; liftId++) {
      actor = Actor_Get(liftId);
      actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
      Actor_SelectAnimation(actor, 4);
      actor->speedLimit = px(1) / 2;
      actor->acceleration = px(1) / 5;
    }
    Task_AddFrame(ColosseumSatrageMatch_UpdateLifts, TASK_PASS_FRAME + 5);
    Task_SetFlags(ColosseumSatrageMatch_UpdateLifts, 1);
  }
  Actor_SetAnimation(ACTOR_OPPONENT, WARRIOR_ANIMATION_FIGHTING_STANCE);
  gMainRoutineWork.movementMode = LEADER_MOVEMENT_WALK;
  Colosso_ShowItemIcon(ACTOR_PRIZE_B, ITEM_COTTON_SHIRT);
  Colosso_ShowItemIcon(ACTOR_PRIZE_A, ITEM_CHAIN_MAIL);
  Actor_SetSpriteColorById(ACTOR_OPPONENT, SPRITE_COLOR_GRB);
  // The race, the friends' turns, the tour, and the battle's result.
  switch (gMainRoutineWork.entrance) {
  case COLOSSEUM_SATRAGE_MATCH_RACE_FROM_LEND_A_HAND:
    ColosseumSatrageMatch_StartRaceGauge(ACTOR_PARTY_LEADER, ACTOR_OPPONENT, 5,
                                         px(104), px(256), ACTOR_PRIZE_A,
                                         ACTOR_PRIZE_B);
    MapGrid_CopyCellsLayered(127, 0, 1, 2, 79, 6);
    Actor_ReleaseById(ACTOR_GUIDE);
    Actor_ReleaseById(ACTOR_MINI_MAZE_ATTENDANT);
    Actor_ReleaseById(ACTOR_SHIFTING_FLOOR_ATTENDANT);
    Actor_ReleaseById(ACTOR_LOG_ROLLING_ATTENDANT);
    Actor_ReleaseById(ACTOR_WALL_ATTENDANT);
    Actor_ReleaseById(ACTOR_BOARD_WALK_ATTENDANT);
    if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
      Sound_Play(SOUND_FADE_OUT_BGM);
      Colosso_ControlCharacterAlone(ISAAC);
      Colosso_PlaceFriendsAtCheeringSpots();
      Colosso_ShowOpeningLettering(2);
    }
    Actor_WatchAndSetScript(ACTOR_GARET, ACTOR_PARTY_LEADER);
    Actor_WatchAndSetScript(ACTOR_IVAN, ACTOR_PARTY_LEADER);
    Actor_WatchAndSetScript(ACTOR_MIA, ACTOR_PARTY_LEADER);
    ColosseumSatrageMatch_StartOpponentPath(RESOURCE_OPPONENT_PATH_B);
    break;
  case COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_EAST:
    Task_AddFrame(Colosso_RaiseTakePlaceOnPsynergy, TASK_PASS_FRAME);
    Actor_ReleaseById(ACTOR_PRIZE_A);
    Actor_ReleaseById(ACTOR_PRIZE_B);
    if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
      Colosso_PlaceFriendsAtCheeringSpots();
      Colosso_ControlCharacterAlone(GARET);
      Colosso_ShowOpeningLettering(0);
    }
    break;
  case COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_WEST:
    if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
      ColosseumSatrageMatch_IntroduceSecondFinals(ACTOR_GUIDE);
      Colosso_ClearCheeringSpots();
    }
    break;
  case COLOSSEUM_SATRAGE_MATCH_AFTER_MATCH_WON:
    ColosseumSatrageMatch_StageBattleResult(2);
    Import_EventSetExitRequest(COLOSSEUM_SATRAGE_MATCH_TO_COLOSSEUM_NORTH);
    break;
  case COLOSSEUM_SATRAGE_MATCH_AFTER_MATCH_LOST:
    ColosseumSatrageMatch_StageBattleResult(-2);
    Import_EventSetExitRequest(COLOSSEUM_SATRAGE_MATCH_TO_BABI_PALACE);
    break;
  }
  return 0;
}

/// The guide gives the party the tour of Colosseum Final 2
/// (COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_WEST).
static void ColosseumSatrageMatch_IntroduceSecondFinals(s32 attendantId) {
  s32 attendantZ;
  s32 attendantX;
  struct Actor *actor;

  actor = Actor_Get(attendantId);
  attendantX = actor->x.part.pixel;
  attendantZ = actor->z.part.pixel;
  Event_Start();
  Actor_SetSpeed(attendantId, 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 1);
  Actor_SetSpeed(ACTOR_GARET, 1);
  Actor_SetSpeed(ACTOR_IVAN, 1);
  Actor_SetSpeed(ACTOR_MIA, 1);
  Actor_SetPositionFixed(ACTOR_PARTY_LEADER, (attendantX << 16),
                         ((attendantZ << 16) - px(48)));
  Actor_SetPositionFixed(ACTOR_GARET, (attendantX << 16) - px(16),
                         (attendantZ << 16) - px(40));
  Actor_SetPositionFixed(ACTOR_IVAN, (attendantX << 16) + px(16),
                         (attendantZ << 16) - px(40));
  Actor_SetPositionFixed(ACTOR_MIA, (attendantX << 16),
                         ((attendantZ << 16) - px(32)));
  Actor_SetPositionFixed(attendantId, (attendantX << 16),
                         ((attendantZ << 16) - px(80)));
  Actor_Get(ACTOR_PARTY_LEADER)->facing = FACING_NORTH;
  FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_SetMessageId(MsgColossoSiteSecondFinals);
  Actor_SetAnimationAndWait(attendantId, ANIM_NOD);
  // "This is the site of the second finals match."
  Event_ShowMessage(attendantId, 0);
  Actor_RunRepeatedMotion(attendantId, 2);
  // "As I explained earlier, each stage has its own attendant."
  Event_ShowMessage(attendantId, 0);
  Actor_RunRepeatedMotion(attendantId, 2);
  // "Please ask the attendants directly for descriptions of each..."
  Event_ShowMessage(attendantId, 0);
  Actor_RunRepeatedMotion(attendantId, 2);
  // "...Isaac, that first match was terrific. I'll be right here,..."
  Event_ShowMessage(attendantId, 0);
  Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(6);
  Actor_SetAnimation(ACTOR_GARET, ANIM_WALK);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetDestination(ACTOR_GARET, actor->x.part.pixel, actor->z.part.pixel);
  }
  Actor_SetAnimation(ACTOR_IVAN, ANIM_WALK);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetDestination(ACTOR_IVAN, actor->x.part.pixel, actor->z.part.pixel);
  }
  Actor_SetAnimation(ACTOR_MIA, ANIM_WALK);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetDestination(ACTOR_MIA, actor->x.part.pixel, actor->z.part.pixel);
  }
  Actor_WalkToAndWait(attendantId, attendantX - 16, attendantZ - 64);
  Actor_SetPosition(ACTOR_GARET, 0, 0);
  Actor_SetPosition(ACTOR_IVAN, 0, 0);
  Actor_SetPosition(ACTOR_MIA, 0, 0);
  Actor_WalkToAndWait(attendantId, attendantX - 16, attendantZ - 16);
  Actor_WalkToAndWait(attendantId, attendantX, attendantZ);
  Actor_FaceDirection(attendantId, FACING_NORTH, 10);
  Event_End();
}

/// The Mini-Maze's attendant explains stage 1 of Colosseum Final 2.
static void ColosseumSatrageMatch_TalkToMiniMazeAttendant(s32 attendant) {
  s32 result;

  if (gMainRoutineWork.entrance ==
      COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_EAST) {
    Colosso_AskDoneCheering(attendant);
  } else {
    Event_Start();
    result = Colosso_GreetAtStage(attendant, 1);
    if (result == COLOSSO_GUIDE_DESCRIBE) {
      Event_SetMessageId(MsgColossoStageDubbedMini);
      FieldCamera_SetSpeed(3);
      FieldCamera_MoveTo(1224, CAMERA_KEEP, 184, 1);
      FieldCamera_WaitForMove();
      // "This stage is dubbed the Mini-Maze."
      Event_ShowMessage(attendant, 0);
      Colosso_FadeInWarrior(ACTOR_PARTY_LEADER, 1272, 168);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, 1.5);
      Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 1288, 184);
      Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 1288, 216);
      Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 1224, 216);
      Event_ShowMessage(attendant, 0);
      Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 1224, 248);
      Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 1192, 248);
      Event_Wait(3);
      Actor_Get(ACTOR_PARTY_LEADER)->velocityY = px(4);
      Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_ARMS_RAISED);
      Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                              ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
      Event_Wait(30);
      Event_ShowMessage(attendant, 0);
      Colosso_RestoreWarrior(ACTOR_PARTY_LEADER);
      FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 0);
      Colosso_RepeatStageNumber(attendant, 1);
    } else if (result == COLOSSO_GUIDE_OFFER_CHEERING) {
      Event_SetMessageId(MsgColossoMainThingStage);
      // "The main thing with this stage is to proceed through the..."
      Event_ShowMessage(attendant, 0);
    }
    Colosso_OfferCheeringSpot(result, attendant, 1);
    Event_End();
  }
}

/// The shifting floor stage's attendant explains stage 2 of Colosseum Final 2.
static void ColosseumSatrageMatch_TalkToShiftingFloorAttendant(s32 attendant) {
  s32 result;

  if (gMainRoutineWork.entrance ==
      COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_EAST) {
    Colosso_AskDoneCheering(attendant);
  } else {
    Event_Start();
    result = Colosso_GreetAtStage(attendant, 2);
    if (result == COLOSSO_GUIDE_DESCRIBE) {
      Event_SetMessageId(MsgColossoShiftingFloorStage);
      ColosseumSatrageMatch_ResetLifts();
      FieldCamera_SetSpeed(3);
      FieldCamera_MoveTo(984, CAMERA_KEEP, 232, 1);
      FieldCamera_WaitForMove();
      COLOSSEUM_SATRAGE_MATCH_SHOW_MESSAGE(attendant, 0);
      ColosseumSatrageMatch_StartLifts();
      Event_ShowMessage(attendant, 0);
      Colosso_FadeInWarrior(ACTOR_PARTY_LEADER, 1080, 264);
      Event_Wait(15);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, 1.5);
      Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 1080, 216);
      Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 1064, 216);
      ColosseumSatrageMatch_WaitForLiftsLinedUp();
      Field_TryJumpForward();
      FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
      Event_ShowMessage(attendant, 0);
      Colosso_RestoreWarrior(ACTOR_PARTY_LEADER);
      FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 0);
      Colosso_RepeatStageNumber(attendant, 2);
    } else {
      if (result == COLOSSO_GUIDE_OFFER_CHEERING) {
        Event_SetMessageId(MsgColossoOperatorLiftsWillCheerFor);
        // "The operator of the lifts will cheer for you here."
        Event_ShowMessage(attendant, 0);
      }
    }
    Colosso_OfferCheeringSpot(result, attendant, 2);
    Event_End();
  }
}

/// The Log-Rolling Stage's attendant explains stage 3 of Colosseum Final 2.
static void ColosseumSatrageMatch_TalkToLogRollingAttendant(s32 attendant) {
  s32 result;

  if (gMainRoutineWork.entrance ==
      COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_EAST) {
    Colosso_AskDoneCheering(attendant);
  } else {
    Event_Start();
    result = Colosso_GreetAtStage(attendant, 3);
    if (result == COLOSSO_GUIDE_DESCRIBE) {
      Event_SetMessageId(MsgColossoLogRollingStage);
      FieldCamera_SetSpeed(3);
      FieldCamera_MoveTo(752, CAMERA_KEEP, 192, 1);
      FieldCamera_WaitForMove();
      Event_Wait(60);
      FieldCamera_SetSpeed(1);
      FieldCamera_MoveTo(752, CAMERA_KEEP, 224, 1);
      FieldCamera_WaitForMove();
      // "This is the Log- Rolling Stage."
      Event_ShowMessage(attendant, 0);
      Colosso_FadeInWarrior(ACTOR_PARTY_LEADER, 856, 264);
      Event_Wait(10);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, 1.5);
      Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 856, 264);
      Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 856, 232);
      Event_ShowMessage(attendant, 0);
      Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 840, 232);
      Event_Wait(10);
      COLOSSEUM_SATRAGE_MATCH_MOVE_ACTOR_PAIR(ACTOR_RIDING_LOG_SOUTH, -64, 0);
      FieldCamera_MoveTo(752, CAMERA_KEEP, 216, 1);
      Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
      Event_Wait(10);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, 1);
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 760, 232);
      Event_Wait(10);
      Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 30);
      Event_ShowMessage(attendant, 0);
      Colosso_RestoreWarrior(ACTOR_PARTY_LEADER);
      FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 0);
      Actor_SetPosition(ACTOR_RIDING_LOG_SOUTH, 840, 232);
      Colosso_RepeatStageNumber(attendant, 3);
    } else if (result == COLOSSO_GUIDE_OFFER_CHEERING) {
      Event_SetMessageId(MsgColossoHereYourObjectiveRideLogs);
      // "Here, your objective is to ride the logs as quickly as possible."
      Event_ShowMessage(attendant, 0);
    }
    Colosso_OfferCheeringSpot(result, attendant, 3);
    Event_End();
  }
}

/// The Wall's attendant explains stage 4 of Colosseum Final 2.
static void ColosseumSatrageMatch_TalkToWallAttendant(s32 attendant) {
  struct Actor *actor;
  s32 result;

  if (gMainRoutineWork.entrance ==
      COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_EAST) {
    Colosso_AskDoneCheering(attendant);
    return;
  }
  Event_Start();
  result = Colosso_GreetAtStage(attendant, 4);
  if (result == COLOSSO_GUIDE_DESCRIBE) {
    Event_SetMessageId(MsgColossoFansJustCallWall);
    FieldCamera_SetSpeed(3);
    FieldCamera_MoveTo(536, CAMERA_KEEP, 240, 1);
    FieldCamera_WaitForMove();
    Event_Wait(45);
    FieldCamera_SetSpeed(1);
    FieldCamera_MoveTo(536, CAMERA_KEEP, 192, 1);
    {
      s32 z = 264;

      FieldCamera_WaitForMove();
      // "The fans just call this The Wall."
      Event_ShowMessage(attendant, 0);
      Colosso_FadeInWarrior(ACTOR_PARTY_LEADER, 632, z);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, 1);
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 616, z);
      Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
      Field_ClimbUp();
      FieldCamera_SetSpeed(0.25);
      FieldCamera_MoveTo(536, CAMERA_KEEP, 160, 1);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.5);
      Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_CLIMB);
      actor = Actor_Get(ACTOR_PARTY_LEADER);
      FieldActor_SetMoveTargetFixed(actor, actor->x.fixed,
                                    actor->y.fixed + px(64), actor->z.fixed);
      Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
      Field_ClimbUp();
      FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
      Event_ShowMessage(attendant, 0);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, 1.5);
      Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 488, 248);
      Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 20);
      FieldActor_JumpDownLedge(ACTOR_PARTY_LEADER, 6, 0);
      FieldCamera_MoveTo(536, CAMERA_KEEP, 160, 1);
      Event_ShowMessage(attendant, 0);
      Colosso_RestoreWarrior(ACTOR_PARTY_LEADER);
      FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 0);
      Colosso_RepeatStageNumber(attendant, 4);
    }
  } else if (result == COLOSSO_GUIDE_OFFER_CHEERING) {
    Event_SetMessageId(MsgColossoScalingWallQuickly);
    // "Scaling the wall quickly is priority one in this stage."
    Event_ShowMessage(attendant, 0);
  }
  Colosso_OfferCheeringSpot(result, attendant, 4);
  Event_End();
}

/// Moves a demonstration obstacle to pixel coordinates while its pusher
/// follows.
static void ColosseumSatrageMatch_PushObstacleToCell(s32 obstacleId, s32 column,
                                                     s32 row) {
  s32 pusher = gMainRoutineWork.leaderActorId;
  struct Actor *pusherActor = Actor_Get(pusher);
  struct Actor *obstacle = Actor_Get(obstacleId);
  s32 alongX = FIELD_CELL(obstacle->x.fixed) != column / 2;
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
  Actor_SetAnimation(pusher, ANIM_PUSH);
  Event_Wait(6);
  obstacle->speedLimit = px(1) / 2;
  obstacle->acceleration = px(1) / 5;
  Sound_Play(SE_HEAVY_MOVE);
  Actor_SelectAnimation(obstacle, 3);
  FieldActor_SetMoveTargetFixed(obstacle, column, 0, row);
  Event_Wait(6);
  Actor_SetAnimation(pusher, ANIM_WALK);
  Actor_SetSpeedAndAcceleration(pusher, 0.5, 0.2);
  Actor_SelectAnimation(pusherActor, ANIM_WALK);
  FieldActor_SetMoveTargetFixed(pusherActor, pusherActor->x.fixed + offsetX, 0,
                                pusherActor->z.fixed + offsetZ);
  Actor_WaitForMove(pusherActor);
  Actor_SelectAnimation(pusherActor, ANIM_STAND);
  Actor_WaitForMove(obstacle);
  Actor_SelectAnimation(obstacle, 1);
  Sound_Play(SE_STOP_ACTION_SE);
  Sound_Play(SE_HEAVY_STOP);
  Event_Wait(15);
}

/// The Board Walk's attendant explains stage 5 of Colosseum Final 2.
static void ColosseumSatrageMatch_TalkToBoardWalkAttendant(s32 attendant) {
  s32 result;

  if (gMainRoutineWork.entrance ==
      COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_EAST) {
    Colosso_AskDoneCheering(attendant);
    return;
  }
  Event_Start();
  result = Colosso_GreetAtStage(attendant, 5);
  if (result == COLOSSO_GUIDE_DESCRIBE) {
    Event_SetMessageId(MsgColossoPlaceCalledBoard);
    FieldCamera_SetSpeed(2);
    FieldCamera_MoveTo(328, CAMERA_KEEP, 264, 1);
    FieldCamera_WaitForMove();
    Event_Wait(30);
    FieldCamera_SetSpeed(1.5);
    FieldCamera_MoveTo(312, CAMERA_KEEP, 176, 1);
    FieldCamera_WaitForMove();
    // "This place is called the Board Walk."
    Event_ShowMessage(attendant, 0);
    Colosso_FadeInWarrior(ACTOR_PARTY_LEADER, 408, 264);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 1.5);
    Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 408, 216);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 10);
    Event_ShowMessage(attendant, 0);
    ColosseumSatrageMatch_PushObstacleToCell(ACTOR_ROLLING_LOG_EAST, 360, 208);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_SMILE, 45);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 1.5);
    Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 376, 216);
    Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 376, 248);
    Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 312, 248);
    Event_ShowMessage(attendant, 0);
    Colosso_RestoreWarrior(ACTOR_PARTY_LEADER);
    FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 0);
    Actor_SetPosition(ACTOR_ROLLING_LOG_EAST, 392, 208);
    Colosso_RepeatStageNumber(attendant, 5);
  } else if (result == COLOSSO_GUIDE_OFFER_CHEERING) {
    Event_SetMessageId(MsgColossoFocusRollingLogs);
    // "Your focus here will be rolling the logs to form a path."
    Event_ShowMessage(attendant, 0);
  }
  Colosso_OfferCheeringSpot(result, attendant, 5);
  Event_End();
}

// This scene's exits for colosso_solo.c.
#define COLOSSO_SOLO_TO_RACE COLOSSEUM_SATRAGE_MATCH_LEND_A_HAND_TO_RACE
#define COLOSSO_SOLO_TO_COLOSSEUM COLOSSEUM_SATRAGE_MATCH_TO_COLOSSEUM_SOUTH
#include "shared/colosso_solo.c"
#include "shared/item_equip.c"
#include "shared/colosso_task.c"

#include "shared/colosso_lettering.c"

#include "shared/colosso_marker.c"
#include "shared/colosso_item_icon.c"

#include "shared/colosso_hex_string.c"
#include "shared/colosso_finish.c"

#include "shared/colosso_path_opponent.c"

/// Colosseum Final 2's own copy of Colosso_StartRaceGauge
/// (shared/colosso_setup.c), which passes the gauge routine's address as it is.
void ColosseumSatrageMatch_StartRaceGauge(s32 leaderId, s32 opponentId,
                                          s32 segments, s32 finishX,
                                          s32 finishZ, s32 firstItem,
                                          s32 secondItem) {
  struct ColossoRaceWork *work;
  struct Actor *leader;
  struct Actor *opponent;
  void *graphics;
  s32 vramSlot;

  work = HeapSlot_AllocEwram(HEAP_SLOT_COLOSSO_RACE,
                             sizeof(struct ColossoRaceWork));
  graphics = Heap_AllocEwram(512);

  work->gauge.playerActorId = leaderId;
  work->gauge.opponentActorId = opponentId;
  work->gauge.finishItemActorIdA = firstItem;
  work->gauge.finishItemActorIdB = secondItem;
  work->gauge.segments = segments;
  work->gauge.finishX = finishX;
  work->gauge.finishZ = finishZ;

  leader = Actor_Get(leaderId);
  opponent = Actor_Get(opponentId);

  // Unless the party returns to where it stood, the opponent starts level
  // with the leader, as far on the other side of the finish.
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
    opponent->x.fixed = (finishX << 1) - leader->x.fixed;
    opponent->z.fixed = leader->z.fixed;
  }

  work->gauge.level = 0;
  work->gauge.hold = 0;

  FieldDecompress_TaggedLz(gColosso_GaugeGraphics, graphics);

  vramSlot = VramCache_FindFreeEntry();
  work->gauge.vramBlock = vramSlot;
  VramCache_Load((s16)vramSlot, 512, graphics);

  Task_AddFrame(Colosso_DrawGauge, TASK_PASS_FRAME - 10);

  Heap_Release(graphics);
}

/// Decodes the opponent's recorded path and starts its playback task.
static void ColosseumSatrageMatch_StartOpponentPath(s32 pathResource) {
  struct ColossoRaceWork *work = HEAP_SLOT_BLOCK(HEAP_SLOT_COLOSSO_RACE);
  struct PathRecorder *recorder = (struct PathRecorder *)gSceneLocalState;

  FieldDecompress_TaggedLz(Resource_GetTableEntry(pathResource), work->path);
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
    recorder->mode = PATH_RECORDER_PLAY;
    recorder->mirror = 1;
    recorder->actorId = work->gauge.opponentActorId;
    recorder->still = 0;
    recorder->pathIndex = 0;
  }
  Task_AddFrame(Colosso_UpdatePathOpponent, TASK_PASS_FRAME + 5);
}

#include "shared/colosso_map_state.c"
#include "shared/actor_find_at_position.c"

#include "shared/colosso_occupant.c"

/// Closes the arena's step trigger. Nothing in the game sets it.
#define FLAG_COLOSSEUM_ARENA_CLOSED FLAG_SCENE_TEMP_10
/// The chest holding a Smoke Bomb has been opened.
#define FLAG_COLOSSEUM_SATRAGE_MATCH_SMOKE_BOMB_CHEST_OPENED FLAG_AREA_TEMP_39

const u8 gColosseumSatrageMatch_RollLogScript[] = {2, 2, 3, 3};

#include "shared/colosso_lettering_tables.c"
// clang-format off

char gHexDigits[20] = "0123456789ABCDEF";

#include "shared/colosso_prize_scripts.c"

u32 gPushedActor_DirectionSteps[] = {
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

struct ActorPlacement gColosseumSatrageMatchActorPlacements[] = {
    ACTOR_PLACEMENT(ACTOR_PARTY_LEADER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        1272, 0, 168, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        264, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        376, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        568, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        1272, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        1224, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        1176, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7,
        264, 0, 208, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7,
        392, 0, 208, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FALLING_LOG, CONDITION_ALWAYS, 7,
        360, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        520, 0, 258, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        712, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        808, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, 7,
        936, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, 7,
        968, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, 7,
        1000, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, 7,
        1032, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, 7,
        248, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, 7,
        264, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, 7,
        296, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, 7,
        312, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FALLING_LOG, CONDITION_ALWAYS, 7,
        360, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        1072, 0, 280, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7,
        840, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7,
        840, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        1224, 0, 488, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        1208, 0, 376, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        984, 0, 376, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        760, 0, 376, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        536, 0, 376, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        312, 0, 376, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        104, 0, 240, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        104, 0, 272, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

s32 gColosseumSatrageMatch_LiftPhase = 0;

s32 gColosseumSatrageMatch_LiftTimer = 0;

struct Trigger gColosseumSatrageMatchTriggers[] = {
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 30, CONDITION_ALWAYS, EVENT(ColosseumSatrageMatch_JumpDownLedge) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), 30, CONDITION_ALWAYS, EVENT(ColosseumSatrageMatch_JumpWestFromLedge) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), 28, CONDITION_ALWAYS, EVENT(ColosseumSatrageMatch_RollEastLogWest) },
    { STEP_TRIGGER_WALKING_INTO(FACING_EAST), 27, CONDITION_ALWAYS, EVENT(ColosseumSatrageMatch_RollEastLogEast) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), 24, CONDITION_ALWAYS, EVENT(ColosseumSatrageMatch_RollWestLogWest) },
    { STEP_TRIGGER_WALKING_INTO(FACING_EAST), 25, CONDITION_ALWAYS, EVENT(ColosseumSatrageMatch_RollWestLogEast) },
    { STEP_TRIGGER_WALKING, 22, CONDITION_ALWAYS, EVENT(ColosseumSatrageMatch_RideLog) },
    { STEP_TRIGGER_WALKING, 21, CONDITION_ALWAYS, EVENT(ColosseumSatrageMatch_PushPillar) },
    { STEP_TRIGGER_WALKING, 20, CONDITION_ALWAYS, EVENT(ColosseumSatrageMatch_PushWallPillar) },
    { STEP_TRIGGER_WALKING, 26, CONDITION_ALWAYS, EVENT(ColosseumSatrageMatch_PushWallPillar) },
    { TRIGGER_STEP, 50, FLAG_COLOSSEUM_ARENA_CLOSED, EVENT(ColosseumSatrageMatch_ReachArena) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 101, FLAG_AREA_TEMP_38, FIND_ITEM(ITEM_NUT) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST) | TRIGGER_FLAG_FACING | FACING_WEST, 102, FLAG_COLOSSEUM_SATRAGE_MATCH_SMOKE_BOMB_CHEST_OPENED, FIND_ITEM(ITEM_SMOKE_BOMB) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 104, FLAG_AREA_TEMP_3B, FIND_ITEM(ITEM_NUT) },
    { PSYNERGY_PHASE_ON_ACTOR(ABILITY_FROST, PSYNERGY_PHASE_ACTS), ACTOR_PUDDLE_WEST, FLAG_WEST_TARGET_FROZEN, EVENT(ColosseumSatrageMatch_FreezeWestTarget) },
    { PSYNERGY_PHASE_ON_ACTOR(ABILITY_FROST, PSYNERGY_PHASE_ACTS), ACTOR_PUDDLE_EAST, FLAG_EAST_TARGET_FROZEN, EVENT(ColosseumSatrageMatch_FreezeEastTarget) },
    { PSYNERGY_PHASE_ON_ACTOR(ABILITY_FORCE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FORCE_TARGET, FLAG_OBSTACLE_KNOCKED_DOWN, EVENT(ColosseumSatrageMatch_PrepareForce) },
    { PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_FORCE_TARGET, FLAG_OBSTACLE_KNOCKED_DOWN, EVENT(ColosseumSatrageMatch_KnockDownObstacle) },
    { PSYNERGY_ON_ACTOR(ABILITY_GROWTH), ACTOR_GROWTH_PLANT, FLAG_PLANT_GROWN, EVENT(ColosseumSatrageMatch_GrowPlant) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_BOARD_WALK_WEST_PILLAR, CONDITION_ALWAYS, EVENT(ColosseumSatrageMatch_IgnoreMove) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_BOARD_WALK_EAST_PILLAR, CONDITION_ALWAYS, EVENT(ColosseumSatrageMatch_IgnoreMove) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_WALL_PILLAR, CONDITION_ALWAYS, EVENT(ColosseumSatrageMatch_MoveWallPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_MINI_MAZE_FIRST_PILLAR, CONDITION_ALWAYS, EVENT(ColosseumSatrageMatch_MoveMiniMazePillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_MINI_MAZE_FIRST_PILLAR + 1, CONDITION_ALWAYS, EVENT(ColosseumSatrageMatch_MoveMiniMazePillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_MINI_MAZE_FIRST_PILLAR + 2, CONDITION_ALWAYS, EVENT(ColosseumSatrageMatch_MoveMiniMazePillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_HALT), ACTOR_LIFT_OPERATOR, FLAG_LIFTS_HALTED, EVENT(ColosseumSatrageMatch_HaltLifts) },
    { TRIGGER_TALK, ACTOR_GUIDE, CONDITION_ALWAYS, EVENT(Colosso_AskReadyToRace) },
    { TRIGGER_TALK, ACTOR_MINI_MAZE_ATTENDANT, CONDITION_ALWAYS, EVENT(ColosseumSatrageMatch_TalkToMiniMazeAttendant) },
    { TRIGGER_TALK, ACTOR_SHIFTING_FLOOR_ATTENDANT, CONDITION_ALWAYS, EVENT(ColosseumSatrageMatch_TalkToShiftingFloorAttendant) },
    { TRIGGER_TALK, ACTOR_LOG_ROLLING_ATTENDANT, CONDITION_ALWAYS, EVENT(ColosseumSatrageMatch_TalkToLogRollingAttendant) },
    { TRIGGER_TALK, ACTOR_WALL_ATTENDANT, CONDITION_ALWAYS, EVENT(ColosseumSatrageMatch_TalkToWallAttendant) },
    { TRIGGER_TALK, ACTOR_BOARD_WALK_ATTENDANT, CONDITION_ALWAYS, EVENT(ColosseumSatrageMatch_TalkToBoardWalkAttendant) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GUIDE, CONDITION_ALWAYS, MSGNO(MsgColossoRiverThisYearsFinalsAreIncredibleThis) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MINI_MAZE_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoWallICantBelieveAnybodyEverGets) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SHIFTING_FLOOR_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoWallHowDoTheyMakeThatFloor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LOG_ROLLING_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoWallImAmazedPeopleCanActuallyCross) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WALL_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoWallBoyThatBitOverThereMust) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOARD_WALK_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoWallIHeardBabiGotTheLog) },
    { TRIGGER_RAISED, 99, CONDITION_ALWAYS, EVENT(Colosso_TakePlaceForMatch) },
    { TRIGGER_TABLE_END },
};

s16 gColosso_LetteringSlot = -1;

#include "shared/colosso_lettering_scripts.c"
// clang-format on

s16 gColosso_MarkerSlot = -1;

#include "shared/colosso_finish_scripts.c"
