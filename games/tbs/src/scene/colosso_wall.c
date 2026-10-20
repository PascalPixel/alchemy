/// @file
/// Colosso's second finals course: Isaac races another warrior through the
/// Mini-Maze, the shifting floor, the Log-Rolling Stage, the Wall and the Board
/// Walk to the arena.

#include "script.h"
#include "game_state.h"
#include "io_write_queue.h"
#include "scene_ids.h"
#include "vram_tab.h"
#include "unit/import.h"
#include "unit/scene/colosso_wall.h"
#include "unit/scene/common/colosso_solo.h"
#include "unit/scene/common/colosso_marker.h"
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
#include "flags.h"
#include "songs.h"
#include "macros.h"

#define COLOSSO_WALL_FIRST_PLATFORM 22
#define COLOSSO_WALL_LAST_PLATFORM 25
#define COLOSSO_WALL_PLATFORM_CYCLE_FRAMES 120
#define COLOSSO_WALL_PLATFORM_CROSSING_FRAME 75
#define COLOSSO_WALL_DEMONSTRATION_TIMEOUT_FRAMES 600
#define COLOSSO_WALL_LOG_TRAVEL_PIXELS 64

// UB: westward log travel passes -64, which cannot be shifted left as signed C.
// Both callers supply only -64, 0 or 64, so fixed-point multiplication fits.
#ifdef UBFIX
#define COLOSSO_WALL_OFFSET_FIXED(value) ((value) * 0x10000)
#else
#define COLOSSO_WALL_OFFSET_FIXED(value) ((value) << 16)
#endif

#if LANGUAGE == LANGUAGE_JAPANESE || LANGUAGE == LANGUAGE_ENGLISH
#define COLOSSO_WALL_ALTERNATE_PORTRAIT_PALETTE_OFFSET 64
#else
#define COLOSSO_WALL_ALTERNATE_PORTRAIT_PALETTE_OFFSET 160
#endif

#if LANGUAGE == LANGUAGE_JAPANESE
#define COLOSSO_WALL_INTRO_SCALE 256
#else
#define COLOSSO_WALL_INTRO_SCALE 384
#endif

#if LANGUAGE == LANGUAGE_FRENCH
#define COLOSSO_WALL_PULSE_FIRST_SCALE 512
#define COLOSSO_WALL_PULSE_HIGH_SCALE 512
#define COLOSSO_WALL_PULSE_LOW_SCALE 384
#elif LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_ITALIAN
#define COLOSSO_WALL_PULSE_FIRST_SCALE 384
#define COLOSSO_WALL_PULSE_HIGH_SCALE 384
#define COLOSSO_WALL_PULSE_LOW_SCALE 256
#else
#define COLOSSO_WALL_PULSE_FIRST_SCALE 512
#define COLOSSO_WALL_PULSE_HIGH_SCALE 384
#define COLOSSO_WALL_PULSE_LOW_SCALE 256
#endif

#ifdef UBFIX
#define COLOSSO_WALL_SHOW_MESSAGE Engine_EventShowMessage
#define COLOSSO_WALL_MOVE_ACTOR_PAIR ColossoWall_MovePusherAndLog
#define COLOSSO_WALL_START_MODE_TASK ColossoWall_StartLetteringAnimation
#define COLOSSO_WALL_APPLY_MATCHING_SLOT_VALUE Scene_GiveAndEquipItem
#else
#define COLOSSO_WALL_SHOW_MESSAGE ((s32 (*)())Engine_EventShowMessage)
#define COLOSSO_WALL_MOVE_ACTOR_PAIR ((s32 (*)())ColossoWall_MovePusherAndLog)
#define COLOSSO_WALL_START_MODE_TASK                                           \
  ((s32 (*)())ColossoWall_StartLetteringAnimation)
#define COLOSSO_WALL_APPLY_MATCHING_SLOT_VALUE                                 \
  ((s32 (*)())Scene_GiveAndEquipItem)
#endif

// Forward declarations.
static void ColossoWall_IntroduceSecondFinals(s32 attendantId);
static void ColossoWall_ShowRoundStartLettering(s32 mode);
static void ColossoWall_ShowBattleResultLettering(s32 result);
static void SceneState_EmptyHook(void);
static void ColossoWall_WaitForRivalFinish(void);
static s32 ColossoWall_ApproachFinishAndTakeItem(s32 firstItem, s32 secondItem);
static void ColossoWall_StartRivalPath(s32 pathResource);
static void ColossoWall_PushAdjacentObstacle(void);

/// The second course's actors, by the guide's and attendants' explanations: the
/// Mini-Maze, the shifting floor, the Log-Rolling Stage, the Wall and the Board
/// Walk lead to the arena. The lifts are actors 22 to 25
/// (COLOSSO_WALL_FIRST_PLATFORM to COLOSSO_WALL_LAST_PLATFORM).
enum {
  OPPONENT = 8, ///< the warrior racing Isaac to the arena
  ROLLING_LOG_WEST = 15,
  ROLLING_LOG_EAST = 16,
  ROLLING_LOG_CENTER = 17,
  FORCE_TARGET = 18,
  GROWTH_PLANT = 19,
  FROST_TARGET_WEST = 20,
  FROST_TARGET_EAST = 21,
  FALLING_OBSTACLE = 30, ///< knocked down by Force on FORCE_TARGET
  LIFT_OPERATOR = 31,
  RIDING_LOG_NORTH = 32, ///< the Log-Rolling Stage's two logs
  RIDING_LOG_SOUTH = 33,
  GUIDE = 34, ///< explains the second course
  MINI_MAZE_ATTENDANT = 35,
  SHIFTING_FLOOR_ATTENDANT = 36,
  LOG_ROLLING_ATTENDANT = 37,
  WALL_ATTENDANT = 38,
  BOARD_WALK_ATTENDANT = 39,
  PRIZE_A = 40, ///< the arena's Chain Mail and Cotton Shirt
  PRIZE_B = 41
};

#define WORK_SLOT_STAGE 59

/// A field object seen by the block push: the pusher or the block, with its
/// position, push rates and collision flags.
typedef struct ColossoWallSceneRecord {
  u8 pad00[6];
  u16 facing;
  s32 x; ///< 16.16 fixed-point pixels
  s32 y; ///< 16.16 fixed-point pixels
  s32 z; ///< 16.16 fixed-point pixels
  u8 pad14[14];
  u8 state;
  u8 pad23;
  s32 motion24;
  u8 pad28[4];
  s32 motion2c;
  s32 rateX;
  s32 rateZ;
  u8 pad38[33];
  u8 flags;
} ColossoWallSceneRecord;

/// A world position, used to look for objects in the cells around a block.
typedef struct Position3 {
  s32 x; ///< 16.16 fixed-point pixels
  s32 y; ///< 16.16 fixed-point pixels
  s32 z; ///< 16.16 fixed-point pixels
} Position3;

/// The rival's recorded-path playback state, held in the scene state; status
/// 9 marks the rival finished.
struct StageControl {
  s16 status;
  s16 f2;
  s16 f4;
  s16 f6;
  s16 f8;
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
  return gColossoWallEntrances;
}

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

/// Returns the packed course and battle-result exits.
const u32 *Scene_GetExits(void) { return gColossoWallExits; }

/// Returns the course's competitors, obstacles and attendants.
const struct ScenePlacement *Scene_GetPlacements(void) {
  return gColossoWallPlacements;
}

/// Move on the first two rocks does nothing.
static void Event_IgnoreMove(void) {}

/// Pushes the obstacle immediately ahead of the selected actor.
static void Event_PushRock(void) { ColossoWall_PushAdjacentObstacle(); }

/// Updates the first movable obstacle's collision after Move.
static void Event_UpdateMovedRockCell(void) {
  struct FieldActor *obstacle;
  s32 column;
  s32 row;

  obstacle = Object_GetById(11);
  if ((obstacle->x.fixed >> 20) == 36) {
    Engine_GameFlagSet(0x335);
    column = 0x23;
    row = 0x4D;
    Map_CopyCellAttributes(0x23, 0x4E, 1, 1, column, row);
  } else {
    Engine_GameFlagClear(0x335);
    column = 0x23;
    row = 0x4D;
    Map_CopyCellAttributes(0x22, 0x4D, 1, 1, column, row);
  }
}

/// Pushes an obstacle, then restores the first obstacle's collision cell.
static void Event_PushRockAndUpdateCells(void) {
  ColossoWall_PushAdjacentObstacle();
  Event_UpdateMovedRockCell();
}

/// Saves the movable obstacles' columns and rebuilds their collision cells.
static void Event_SaveMovedRockColumns(void) {

  struct FieldActor *obstacle;
  s32 column;

  Map_CopyCellAttributes(100, 11, 12, 4, 14, 11);

  obstacle = Object_GetById(12);
  column = obstacle->x.fixed >> 20;
  FieldGameFlag_SetByte(832, column);
  Map_CopyCellAttributes(71, 16, 1, 1, column, 16);

  obstacle = Object_GetById(13);
  column = obstacle->x.fixed >> 20;
  FieldGameFlag_SetByte(840, column);
  Map_CopyCellAttributes(71, 16, 1, 1, column, 16);

  obstacle = Object_GetById(14);
  column = obstacle->x.fixed >> 20;
  FieldGameFlag_SetByte(848, column);
  Map_CopyCellAttributes(71, 16, 1, 1, column, 16);
}

/// Enables the child effect for Force on the course obstacle.
static void Event_PrepareForce(void) {
  u8 *effectWork = gEffectWork;

  ((struct BattleFxScene *)effectWork)->childMode = 1;
}

/// Force on the target: the obstacle spins down into place and the way past it
/// opens (0x330).
static void Event_KnockDownObstacle(void) {
  struct FieldActor *obstacle;
  s32 fallFrames;
  s32 rotationStep;
  s32 settleFrames;
  s32 settleRotationStep;
  struct AnimationObject *sprite;

  obstacle = Object_GetById(FALLING_OBSTACLE);
  sprite = obstacle->sprite;
  GameFlag_Set(0x330);
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
  // Keep both the course and its saved collision strip in the new state.
  Map_CopyCellAttributes(19, 16, 1, 1, 20, 16);
  Map_CopyCellAttributes(20, 80, 1, 1, 21, 80);
}

/// Frost on the western target: it freezes and its crossing opens (0x331).
static void Event_FreezeWestTarget(void) {
  struct FieldActor *obstacle;

  Engine_GameFlagSet(0x331);
  obstacle = Object_GetById(FROST_TARGET_WEST);
  obstacle->motionFlags = 0;
  {

    Map_CopyCellAttributes(46, 17, 1, 1, 44, 17);
  }
}

/// Frost on the eastern target: it freezes and its crossing opens (0x332).
static void Event_FreezeEastTarget(void) {
  struct FieldActor *obstacle;

  Engine_GameFlagSet(0x332);
  obstacle = Object_GetById(FROST_TARGET_EAST);
  obstacle->motionFlags = 0;
  {

    Map_CopyCellAttributes(46, 17, 1, 1, 50, 17);
  }
}

/// Growth on the plant: it grows into a strip that can be climbed (0x333).
static void Event_GrowPlant(void) {
  Engine_GameFlagSet(0x333);
  {

    Map_CopyCellAttributes(32, 37, 1, 4, 32, 77);
  }
}

/// Runs the rising-object sequence for the actor touching the obstacle.
static void Event_RunRisingSequence(void) {
  FieldBattleFx_RunRisingObjectSequence(gGameState.selectedActor, 6, 0);
}

static void Event_CheckAhead(void) { Engine_LeaderCheckAhead(); }

/// Moves a riding log and its pusher to aligned cell centers.
static void ColossoWall_MovePusherAndLog(s32 logId, s32 offsetX, s32 offsetZ) {

  struct FieldActor *pusher;
  struct FieldActor *log;
  s32 destinationX;
  s32 destinationZ;

  pusher = Object_GetById(gGameState.selectedActor);
  log = Object_GetById(logId);
  Engine_EventBegin();
  {
    destinationX =
        ((pusher->x.fixed + COLOSSO_WALL_OFFSET_FIXED(offsetX)) & 0xFFF00000) +
        0x80000;
    destinationZ =
        ((pusher->z.fixed + COLOSSO_WALL_OFFSET_FIXED(offsetZ)) & 0xFFF00000) +
        0x80000;

    pusher->speed = 0x10000;
    pusher->acceleration = 0x8000;
    Engine_ObjectSetPosition(pusher, destinationX, pusher->y.fixed,
                             destinationZ);
  }
  Object_SetMode(pusher, 27);
  {
    destinationX =
        ((log->x.fixed + COLOSSO_WALL_OFFSET_FIXED(offsetX)) & 0xFFF00000) +
        0x80000;
    destinationZ =
        ((log->z.fixed + COLOSSO_WALL_OFFSET_FIXED(offsetZ)) & 0xFFF00000) +
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

/// Moves the aligned riding log in the direction the player holds.
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
      offsetX = -COLOSSO_WALL_LOG_TRAVEL_PIXELS;
    }
  } else if (gKeysHeld & KEY_RIGHT) {
    offsetX = COLOSSO_WALL_LOG_TRAVEL_PIXELS;
  }
  if (offsetX != 0) {
    ColossoWall_MovePusherAndLog(logId, offsetX, 0);
    // Replace both log footprints after the moving pair has stopped.
    Map_CopyCellAttributes(120, 10, 5, 6, 48, 10);
    column = Object_GetById(RIDING_LOG_NORTH)->x.fixed >> 20;
    Map_CopyCellAttributes(52, 28, 1, 3, column, 10);
    column = Object_GetById(RIDING_LOG_SOUTH)->x.fixed >> 20;
    Map_CopyCellAttributes(52, 28, 1, 3, column, 13);
  }
}

/// Rolls a log to half-cell coordinates while its pusher follows halfway.
static void ColossoWall_RollLogToCell(s32 logId, s32 halfCellX, s32 halfCellZ) {
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
  Object_SetMode(log, gColossoWall_RollLogScript[heading / 0x4000]);
  Engine_ObjectSetPosition(log, halfCellX, 0, halfCellZ);
  Engine_EventWait(6);
  Engine_ActorSetAnimation(pusher, 2);
  // Follow halfway behind the rolling log while its finish effect runs.
  ObjectDispatch_StartCamera(
      *(struct DispatchObject **)((u8 *)Runtime_AllocateBlock(27, 0xccc) + 480),
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
static void ColossoWall_UpdateRollingLogCollision(void) {
  s32 cell;

  Map_CopyCellAttributes(100, 11, 12, 4, 14, 11);

  cell = Object_GetById(ROLLING_LOG_WEST)->x.fixed >> 20;
  Map_CopyCellAttributes(13, 28, 1, 4, cell, 11);

  cell = Object_GetById(ROLLING_LOG_EAST)->x.fixed >> 20;
  Map_CopyCellAttributes(13, 28, 1, 4, cell, 11);

  cell = Object_GetById(ROLLING_LOG_CENTER)->z.fixed >> 20;
  Map_CopyCellAttributes(13, 28, 4, 1, 18, cell);
}

static void Event_RollWestLogWest(void) {
  ColossoWall_RollLogToCell(ROLLING_LOG_WEST, 29, 26);
  ColossoWall_UpdateRollingLogCollision();
}

static void Event_RollWestLogEast(void) {
  ColossoWall_RollLogToCell(ROLLING_LOG_WEST, 33, 26);
  ColossoWall_UpdateRollingLogCollision();
}

static void Event_RollEastLogWest(void) {
  ColossoWall_RollLogToCell(ROLLING_LOG_EAST, 45, 26);
  ColossoWall_UpdateRollingLogCollision();
}

static void Event_RollEastLogEast(void) {
  ColossoWall_RollLogToCell(ROLLING_LOG_EAST, 49, 26);
  ColossoWall_UpdateRollingLogCollision();
}

/// Rolls the center log north and rebuilds the log collision rectangles.
static UNUSED void ColossoWall_RollCenterLogNorth(void) {
  ColossoWall_RollLogToCell(ROLLING_LOG_CENTER, 40, 23);
  ColossoWall_UpdateRollingLogCollision();
}

/// Rolls the center log south and rebuilds the log collision rectangles.
static UNUSED void ColossoWall_RollCenterLogSouth(void) {
  ColossoWall_RollLogToCell(ROLLING_LOG_CENTER, 40, 25);
  ColossoWall_UpdateRollingLogCollision();
}

/// Carries the selected actor on the moving platforms and alternates their
/// routes.
static void ColossoWall_UpdateMovingPlatforms(void) {
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
  for (platformId = COLOSSO_WALL_FIRST_PLATFORM;
       platformId <= COLOSSO_WALL_LAST_PLATFORM; platformId++) {
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
    if (GameFlag_IsSet(0x104) != 0) {
      riderZ = platform->z.fixed;
    } else {
      riderZ = rider->z.fixed;
      deltaZ = platform->velocityZ;
      riderZ += deltaZ;
    }
    rider->z.fixed = riderZ;
  }
  // The final platform's destination selects the collision tiles for the row.
  if (gColossoWall_SpectatorTimer != 0 && platform->targetX == 0x80000000) {
    if (gColossoWall_SpectatorPhase == 0) {
      Map_CopyCellAttributes(58, 28, 7, 1, 58, 13);
    } else {
      Map_CopyCellAttributes(58, 10, 1, 1, 58, 11);
    }
  } else {
    Map_CopyCellAttributes(57, 11, 1, 1, 58, 11);
    Map_CopyCellAttributes(58, 14, 7, 1, 58, 13);
  }
  // Start the next alternating route when the two-second cycle wraps.
  if (gColossoWall_SpectatorTimer == 0) {
    gColossoWall_SpectatorPhase ^= 1;
    if (gColossoWall_SpectatorPhase != 0) {
      platformActor = Object_GetById(22);
      FieldObject_SetPosition(platformActor, PIXELS(936), 0, PIXELS(184));
      platformActor = Object_GetById(23);
      FieldObject_SetPosition(platformActor, PIXELS(968), 0, PIXELS(248));
      platformActor = Object_GetById(24);
      FieldObject_SetPosition(platformActor, PIXELS(1000), 0, PIXELS(184));
      platformActor = Object_GetById(25);
      FieldObject_SetPosition(platformActor, PIXELS(1032), 0, PIXELS(248));
      Engine_ActorSetAnimation(LIFT_OPERATOR, 11);
    } else {
      platformActor = Object_GetById(22);
      FieldObject_SetPosition(platformActor, PIXELS(936), 0, PIXELS(216));
      platformActor = Object_GetById(23);
      FieldObject_SetPosition(platformActor, PIXELS(968), 0, PIXELS(216));
      platformActor = Object_GetById(24);
      FieldObject_SetPosition(platformActor, PIXELS(1000), 0, PIXELS(216));
      platformActor = Object_GetById(25);
      FieldObject_SetPosition(platformActor, PIXELS(1032), 0, PIXELS(216));
      Engine_ActorSetAnimation(LIFT_OPERATOR, 10);
    }
  }
  gColossoWall_SpectatorTimer++;
  if ((u32)gColossoWall_SpectatorTimer >= COLOSSO_WALL_PLATFORM_CYCLE_FRAMES) {
    if (GameFlag_IsSet(0x104) == 0) {
      gColossoWall_SpectatorTimer = 0;
    }
  }
}

/// Stops the moving-platform task and restores its starting row.
static void ColossoWall_ResetMovingPlatforms(void) {
  gColossoWall_SpectatorTimer = 0;
  gColossoWall_SpectatorPhase = 0;
  Engine_TaskRemoveCallback(ColossoWall_UpdateMovingPlatforms);
  Actor_SetPosition(22, PIXELS(936), PIXELS(216));
  Actor_SetPosition(23, PIXELS(968), PIXELS(216));
  Actor_SetPosition(24, PIXELS(1000), PIXELS(216));
  Actor_SetPosition(25, PIXELS(1032), PIXELS(216));
  Engine_ActorSetAnimation(LIFT_OPERATOR, 10);
}

/// Starts the moving-platform task and releases its pause flag.
static void ColossoWall_StartMovingPlatforms(void) {
  FieldScheduler_AddCallback(ColossoWall_UpdateMovingPlatforms, 0xC85);
  Engine_GameFlagClear(0x104);
}

/// Waits for the demonstration's crossing phase, with a ten-second timeout.
static void ColossoWall_WaitForPlatformCrossingPhase(void) {
  s32 elapsedFrames;

  Engine_TaskWait(10);

  elapsedFrames = 0;
  while (gColossoWall_SpectatorPhase != 0 ||
         gColossoWall_SpectatorTimer != COLOSSO_WALL_PLATFORM_CROSSING_FRAME) {
    Engine_TaskWait(1);
    elapsedFrames++;
    if (elapsedFrames >= COLOSSO_WALL_DEMONSTRATION_TIMEOUT_FRAMES) {
      return;
    }
  }
}

/// Halt on the lift operator: the lifts stop and leave a crossing (0x334).
static void Event_HaltLifts(void) {
  BattleEffect_PauseObject(LIFT_OPERATOR);
  Engine_GameFlagSet(820);

  if (gColossoWall_SpectatorPhase != 0) {
    gColossoWall_SpectatorTimer = 0;
  }

  Engine_TaskWait(30);
  Engine_TaskWait(1);

  Engine_TaskRemoveCallback(ColossoWall_UpdateMovingPlatforms);

  Map_CopyCellAttributes(58, 28, 7, 1, 58, 13);
  Map_CopyCellAttributes(57, 11, 1, 1, 58, 11);
}

/// Reaching the arena: the leader takes a prize (the Chain Mail if first, the
/// Cotton Shirt if second), and he and his opponent face off and fight.
/// FLAG_SCENE_TEMP_11 says the arena was already reached; the arena's touch
/// works while FLAG_SCENE_TEMP_10 is clear.
static void Event_ReachArena(void) {
  s32 rivalFinishedFirst;
  s32 refreshCount;
  s32 resultBase;

  resultBase = 2;
  SceneState_EmptyHook();
  Engine_EventBegin();
  rivalFinishedFirst = ColossoWall_ApproachFinishAndTakeItem(77, 89);
  ColossoWall_WaitForRivalFinish();
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
  /* FAKEMATCH: resultBase, set to 2 at the top, keeps the sum unfolded; as
   * 3 - rivalFinishedFirst, 10 instructions differ in tbs-en. */
  Field_RequestBattleFromEncounterTable(72,
                                        resultBase - rivalFinishedFirst + 1);
  Battle_SetStart(3);
  Event_SetNextDestination((s32)&SceneId_ColossoWall, 4);
  Event_SetDefeatDestination((s32)&SceneId_ColossoWall, 5);
  Engine_GameFlagSet(FLAG_KEEP_MUSIC);
}

/// Returns the course's obstacle interactions and attendant dialogue.
const struct SceneEvent *Scene_GetEvents(void) { return gColossoWallEvents; }

/// Back from the battle: the two warriors stand in the arena as the result is
/// shown.
static void ColossoWall_StageBattleResult(s32 result) {

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
  ColossoWall_ShowBattleResultLettering(result);
  Engine_EventEnd();
}

/// Restores obstacle progress and sets up the selected course entrance.
s32 ColossoWall_Initialize(void) {
  struct FieldActor *actor;
  s32 x;
  s32 obstacleId;
  s32 column;
  s32 platformId;
  s32 columnOffset;
  s32 row;
  s32 zero;

  gEventWork->startTransition = 0;
  // Restore the collision templates before applying saved puzzle progress.
  Engine_GameFlagSet(FLAG_RETREAT_BLOCKED);
  Map_CopyCellAttributes(14, 11, 12, 4, 100, 11);
  Map_CopyCellAttributes(48, 10, 5, 6, 120, 10);
  for (obstacleId = 26; obstacleId <= 30; obstacleId++) {
    actor = Object_GetById(obstacleId);
    Object_SetMode(actor, 4);
    actor->motionFlags = 0;
    actor->y.fixed = 0;
    actor->priorityFlags = 2;
  }
  Object_GetById(FORCE_TARGET)->priorityFlags = 2;
  if (Engine_GameFlagIsSet(0x330)) {
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
  Object_GetById(11)->priorityFlags = 2;
  zero = 0;
  if (Engine_GameFlagIsSet(0x335)) {
    Map_CopyCellAttributes(35, 78, 1, 1, 35, 77);
  }
  if (Engine_GameFlagIsSet(0x333)) {
    Engine_ActorSetAnimation(GROWTH_PLANT, 4);
    Map_CopyCellAttributes(32, 37, 1, 4, 32, 77);
  }
  if (Engine_GameFlagIsSet(0x331)) {
    Object_GetById(FROST_TARGET_WEST)->motionFlags = zero;
    Object_GetById(FROST_TARGET_WEST)->priorityFlags = 2;
    Engine_ActorSetAnimation(FROST_TARGET_WEST, 5);
    Map_CopyCellAttributes(46, 17, 1, 1, 44, 17);
  }
  if (Engine_GameFlagIsSet(0x332)) {
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

  // Each movable obstacle stores its last occupied column in the scene flags.
  x = FieldGameFlag_GetByte(0x340);
  if (x == 0) {
    x = 73;
  }
  actor = Object_GetById(12);
  actor->x.fixed = (x << 20) + 0x80000;
  actor->motionFlags = zero;
  actor->priorityFlags = 2;
  Map_CopyCellAttributes(71, 16, 1, 1, x, 16);
  x = FieldGameFlag_GetByte(0x348);
  if (x == 0) {
    x = 76;
  }
  actor = Object_GetById(13);
  actor->x.fixed = (x << 20) + 0x80000;
  actor->motionFlags = zero;
  actor->priorityFlags = 2;
  Map_CopyCellAttributes(71, 16, 1, 1, x, 16);
  x = FieldGameFlag_GetByte(0x350);
  if (x == 0) {
    x = 79;
  }
  actor = Object_GetById(14);
  actor->x.fixed = (x << 20) + 0x80000;
  actor->motionFlags = zero;
  actor->priorityFlags = 2;
  Map_CopyCellAttributes(71, 16, 1, 1, x, 16);

  ColossoWall_UpdateRollingLogCollision();
  // A Halted controller leaves stationary platforms; otherwise resume the task.
  Engine_ActorSetAnimation(LIFT_OPERATOR, 10);
  if (Engine_GameFlagIsSet(0x334)) {
    for (platformId = COLOSSO_WALL_FIRST_PLATFORM, row = 13, columnOffset = 58;
         platformId <= COLOSSO_WALL_LAST_PLATFORM;
         platformId++, columnOffset += 2) {
      actor = Object_GetById(platformId);
      actor->priorityFlags = 2;
      Object_SetMode(actor, 4);
      Map_CopyCellAttributes(56, 13, 1, 1, columnOffset, row);
    }
    Engine_ActorSetAnimation(LIFT_OPERATOR, 10);
    BattleEffect_PauseObject(LIFT_OPERATOR);
  } else {
    for (platformId = COLOSSO_WALL_FIRST_PLATFORM;
         platformId <= COLOSSO_WALL_LAST_PLATFORM; platformId++) {
      actor = Object_GetById(platformId);
      actor->priorityFlags = 2;
      Object_SetMode(actor, 4);
      actor->speed = 0x8000;
      actor->acceleration = 0x3333;
    }
    FieldScheduler_AddCallback(ColossoWall_UpdateMovingPlatforms, 0xc85);
    Scheduler_SetTaskFlags((u32)ColossoWall_UpdateMovingPlatforms, 1);
  }
  Engine_ActorSetAnimation(OPPONENT, 9);
  gGameState.movementMode = 0;
  Colosso_ShowItemIcon(PRIZE_B, 89);
  Colosso_ShowItemIcon(PRIZE_A, 77);
  Engine_ActorSetChildValue(OPPONENT, 1);
  // Entrances distinguish the race, solo practice, briefing and battle results.
  switch (gGameState.entrance) {
  case 1:
    FieldScene_BuildDescriptorAndInstallTask(0, 8, 5, 0x680000, 0x1000000, 40,
                                             41);
    Map_CopyCellsLayered(127, 0, 1, 2, 79, 6);
    Engine_ActorDestroy(GUIDE);
    Engine_ActorDestroy(MINI_MAZE_ATTENDANT);
    Engine_ActorDestroy(SHIFTING_FLOOR_ATTENDANT);
    Engine_ActorDestroy(LOG_ROLLING_ATTENDANT);
    Engine_ActorDestroy(WALL_ATTENDANT);
    Engine_ActorDestroy(BOARD_WALK_ATTENDANT);
    if (!Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION)) {
      Engine_AudioPlayCue(MUS_017);
      Colosso_ControlCharacterAlone(0);
      Colosso_PlaceFriendsAtCheeringSpots();
      ColossoWall_ShowRoundStartLettering(2);
    }
    Object_LinkObjectAndSetCallback(1, 0);
    Object_LinkObjectAndSetCallback(2, 0);
    Object_LinkObjectAndSetCallback(3, 0);
    ColossoWall_StartRivalPath((s32)&ResourceId_RivalPathB);
    break;
  case 2:
    FieldScheduler_AddCallback(Colosso_RaiseTakePlaceOnPsynergy, 0xc80);
    Engine_ActorDestroy(PRIZE_A);
    Engine_ActorDestroy(PRIZE_B);
    if (!Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION)) {
      Colosso_PlaceFriendsAtCheeringSpots();
      Colosso_ControlCharacterAlone(1);
      ColossoWall_ShowRoundStartLettering(0);
    }
    break;
  case 3:
    if (!Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION)) {
      ColossoWall_IntroduceSecondFinals(GUIDE);
      Colosso_ClearCheeringSpots();
    }
    break;
  case 4:
    ColossoWall_StageBattleResult(2);
    Engine_EventRequestExit(4);
    break;
  case 5:
    ColossoWall_StageBattleResult(-2);
    Engine_EventRequestExit(5);
    break;
  }
  return 0;
}

/// Introduces the second finals course, then gathers the party behind Isaac.
static void ColossoWall_IntroduceSecondFinals(s32 attendantId) {
  s32 attendantZ;
  s32 attendantX;
  struct FieldActor *actor;

  actor = Object_GetById(attendantId);
  attendantX = actor->x.part.pixel;
  attendantZ = actor->z.part.pixel;
  Engine_EventBegin();
  Actor_SetSpeed(attendantId, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(1, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(2, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(3, PIXELS(1), PIXELS(1) / 2);
  Engine_ActorSetPosition(ACTOR_PARTY_LEADER, (attendantX << 16),
                          ((attendantZ << 16) - 0x300000));
  Engine_ActorSetPosition(1, (attendantX << 16) - 0x100000,
                          (attendantZ << 16) - 0x280000);
  Engine_ActorSetPosition(2, (attendantX << 16) + 0x100000,
                          (attendantZ << 16) - 0x280000);
  Engine_ActorSetPosition(3, (attendantX << 16),
                          ((attendantZ << 16) - 0x200000));
  Engine_ActorSetPosition(attendantId, (attendantX << 16),
                          ((attendantZ << 16) - 0x500000));
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  actor->facing = FACING_NORTH;
  Engine_CameraFollowActor(0, 0);
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
  Engine_ActorSetAnimation(3, 3);
  Engine_ActorSetAnimation(1, 3);
  Engine_ActorSetAnimation(2, 3);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
  Engine_EventWait(6);
  Engine_ActorSetAnimation(1, 2);
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Engine_ActorSetDestination(1, actor->x.part.pixel, actor->z.part.pixel);
  }
  Engine_ActorSetAnimation(2, 2);
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Engine_ActorSetDestination(2, actor->x.part.pixel, actor->z.part.pixel);
  }
  Engine_ActorSetAnimation(3, 2);
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Engine_ActorSetDestination(3, actor->x.part.pixel, actor->z.part.pixel);
  }
  Engine_ActorWalkToAndWait(attendantId, (attendantX - 16), (attendantZ - 64));
  Engine_ActorSetPosition(1, 0, 0);
  Engine_ActorSetPosition(2, 0, 0);
  Engine_ActorSetPosition(3, 0, 0);
  Engine_ActorWalkToAndWait(attendantId, (attendantX - 16), (attendantZ - 16));
  Engine_ActorWalkToAndWait(attendantId, attendantX, attendantZ);
  Engine_ActorFaceDirection(attendantId, FACING_NORTH, 10);
  Engine_EventEnd();
}

/// The Mini-Maze's attendant: the first time, a warrior runs the maze; then the
/// objective.
static void Event_TalkToMiniMazeAttendant(s32 attendant) {
  s32 result;
  struct FieldActor *actor;

  if (gGameState.entrance == 2) {
    Colosso_AskDoneCheering(attendant);
  } else {
    Engine_EventBegin();
    result = Colosso_GreetAtStage(attendant, 1);
    if (result == 0) {
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
      Engine_ActorSetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
      Engine_EventWait(30);
      Engine_EventShowMessage(attendant, 0);
      Colosso_RestoreCompetitor(ACTOR_PARTY_LEADER);
      Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
      Colosso_RepeatStageNumber(attendant, 1);
    } else if (result == 1) {
      Engine_EventSetMessage(MsgColossoMainThingStage);
      Engine_EventShowMessage(attendant, 0);
    }
    Colosso_OfferCheeringSpot(result, attendant, 1);
    Engine_EventEnd();
  }
}

/// The shifting floor's attendant: the first time, a warrior rides the lifts;
/// then, that the lifts' operator will cheer for Isaac.
static void Event_TalkToShiftingFloorAttendant(s32 attendant) {
  s32 result;

  if (gGameState.entrance == 2) {
    Colosso_AskDoneCheering(attendant);
  } else {
    Engine_EventBegin();
    result = Colosso_GreetAtStage(attendant, 2);
    if (result == 0) {
      Engine_EventSetMessage(MsgColossoShiftingFloorStage);
      ColossoWall_ResetMovingPlatforms();
      Camera_SetSpeed(0x30000, 0x6000);
      Camera_MoveTo(PIXELS(984), -1, PIXELS(232), 1);
      Engine_CameraWaitForMove();
      // UB: the void target is called through an incompatible return type.
      /* FAKEMATCH: called through a cast to the s32 call type the bytes show,
       * its result discarded; as a direct call, 2 instructions differ in
       * tbs-en. */
      COLOSSO_WALL_SHOW_MESSAGE(attendant, 0);
      ColossoWall_StartMovingPlatforms();
      Event_ShowMessage(attendant, 0);
      Colosso_FadeInCompetitor(ACTOR_PARTY_LEADER, 1080, 264);
      Engine_EventWait(15);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 2, PIXELS(3) / 4);
      Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 1080, 216);
      Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 1064, 216);
      ColossoWall_WaitForPlatformCrossingPhase();
      Engine_LeaderCheckAhead();
      Camera_MoveTo(-1, -1, -1, 0);
      Event_ShowMessage(attendant, 0);
      Colosso_RestoreCompetitor(ACTOR_PARTY_LEADER);
      Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
      Colosso_RepeatStageNumber(attendant, 2);
    } else {
      if (result == 1) {
        Engine_EventSetMessage(MsgColossoOperatorLiftsWillCheerFor);
        Event_ShowMessage(attendant, 0);
      }
    }
    Colosso_OfferCheeringSpot(result, attendant, 2);
    Engine_EventEnd();
  }
}

/// The Log-Rolling Stage's attendant: a warrior rides a log across; then the
/// objective.
static void Event_TalkToLogRollingAttendant(s32 attendant) {
  s32 result;

  if (gGameState.entrance == 2) {
    Colosso_AskDoneCheering(attendant);
  } else {
    Engine_EventBegin();
    result = Colosso_GreetAtStage(attendant, 3);
    if (result == 0) {
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
      // UB: the void target is called through an incompatible return type.
      /* FAKEMATCH: called through a cast to the s32 call type the bytes show,
       * its result discarded; as a direct call, 2 instructions differ in
       * tbs-en. */
      COLOSSO_WALL_MOVE_ACTOR_PAIR(RIDING_LOG_SOUTH, -64, 0);
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
    } else if (result == 1) {
      Engine_EventSetMessage(MsgColossoHereYourObjectiveRideLogs);
      Event_ShowMessage(attendant, 0);
    }
    Colosso_OfferCheeringSpot(result, attendant, 3);
    Engine_EventEnd();
  }
}

/// The Wall's attendant: a warrior climbs it; then the objective.
static void Event_TalkToWallAttendant(s32 attendant) {
  s32 z;
  struct FieldActor *actor;
  s32 result;

  if (gGameState.entrance == 2) {
    Colosso_AskDoneCheering(attendant);
    return;
  }
  Engine_EventBegin();
  result = Colosso_GreetAtStage(attendant, 4);
  if (result == 0) {
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
  } else if (result == 1) {
    Engine_EventSetMessage(MsgColossoScalingWallQuickly);
    Engine_EventShowMessage(attendant, 0);
  }
  Colosso_OfferCheeringSpot(result, attendant, 4);
  Engine_EventEnd();
}

/// Moves a demonstration obstacle to pixel coordinates while its pusher
/// follows.
static void ColossoWall_PushBlockToCell(s32 obstacleId, s32 column, s32 row) {
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

/// The Board Walk's attendant: a warrior rolls a log into a path; then the
/// objective.
static void Event_TalkToBoardWalkAttendant(s32 attendant) {
  s32 result;

  if (gGameState.entrance == 2) {
    Colosso_AskDoneCheering(attendant);
    return;
  }
  Engine_EventBegin();
  result = Colosso_GreetAtStage(attendant, 5);
  if (result == 0) {
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
    ColossoWall_PushBlockToCell(ROLLING_LOG_EAST, 360, 208);
    Engine_ActorShowEmote(ACTOR_PARTY_LEADER, 264, 45);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 2, PIXELS(3) / 4);
    Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 376, 216);
    Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 376, 248);
    Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 312, 248);
    Engine_EventShowMessage(attendant, 0);
    Colosso_RestoreCompetitor(ACTOR_PARTY_LEADER);
    Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
    Actor_SetPosition(ROLLING_LOG_EAST, PIXELS(392), PIXELS(208));
    Colosso_RepeatStageNumber(attendant, 5);
  } else if (result == 1) {
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
static void ColossoWall_StartLetteringAnimation(u32 mode, u32 tileVariant) {
  s32 script;

  gColosso_LetteringMode = mode;
  gColosso_LetteringParam = (tileVariant << 4);

  {
    s32 priority = 0xc80;
    FieldScheduler_AddCallback(Colosso_UpdateLettering, priority);
  }

  script = (s32)gColossoWall_ModeRecordDefault;
  if (mode == 2) {
    script = (s32)gColossoWall_ModeRecordTwo;
  }
  if (mode == 4) {
    script = (s32)gColossoWall_ModeRecordFour;
  }
  if (mode == 3) {
    if (tileVariant != 0) {
      script = (s32)gColossoWall_ModeRecordThreeAlt;
    } else {
      script = (s32)gColossoWall_ModeRecordThree;
    }
  }

  gColosso_LetteringTimer = 0;
  gColosso_LetteringScript = (s16 *)script;
  gColosso_ModeMoveTarget = 0;
  gColosso_ModeMoveDuration = 0;
  gColosso_LetteringPosition = 0;
}

/// Shows the course-opening lettering and plays its timed audio cues.
static void ColossoWall_ShowRoundStartLettering(s32 mode) {
  if (mode == 0) {
    Engine_EventBegin();
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(30);
    Engine_AudioPlayCue(MUS_089);
    Colosso_LoadLettering(0);
    ColossoWall_StartLetteringAnimation(1, 0);
    Engine_EventWait(120);
    Engine_EventEnd();
    return;
  }

  Engine_AudioPlayCue(SE_247);
  Engine_EventBegin();
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  {
    s16 *openingScript = gColossoWall_ModeRecordDefault;
    *(s16 *)((u8 *)openingScript + 30) = (mode * 60);
  }
  Engine_EventWait(30);
  Engine_AudioPlayCue(mode + 0x5a);
  Colosso_LoadLettering(mode);
  ColossoWall_StartLetteringAnimation(1, 0);
  Engine_EventWait(120);

  // Finish the opening cue before the final lettering pulses and start signal.
  while (AudioCommand_GetStateByte() != 0) {
    Engine_TaskWait(1);
  }

  Engine_AudioPlayCue(SE_289);
  Colosso_LoadLettering(5);
  ColossoWall_StartLetteringAnimation(2, 0);
  Engine_AudioPlayCue(SE_236);
  Engine_EventWait(60);
  ColossoWall_StartLetteringAnimation(2, 1);
  Engine_AudioPlayCue(SE_236);
  Engine_EventWait(60);
  Colosso_LoadLettering(6);
  ColossoWall_StartLetteringAnimation(2, 0);
  Engine_AudioPlayCue(SE_236);
  Engine_EventWait(60);
  Colosso_LoadLettering(7);
  ColossoWall_StartLetteringAnimation(4, 0);
  Engine_AudioPlayCue(SE_237);
  Audio_PlayCueFromEventWork();
  Engine_EventEnd();
  Engine_GameFlagSet(0x123);
}

/// Shows the win-or-loss lettering and the appropriate competitor's reaction.
static void ColossoWall_ShowBattleResultLettering(s32 result) {

  s32 reactingActor;

  Engine_AudioPlayCue(SE_247);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  gColossoWall_ModeRecordThree[13] = result * 60;
  STORE_CONST(gColossoWall_ModeRecordThreeAlt[13]) =
      (result < 0 ? -result : result) * 60;
  if (result < 0) {
    Engine_EventWait(30);
    Engine_AudioPlayCue(MUS_086);
    Colosso_LoadLettering(8);
    // UB: the void target is called through an incompatible return type.
    /* FAKEMATCH: called through a cast to the s32 call type the bytes show, its
     * result discarded; as a direct call, 2 instructions differ in tbs-en. */
    COLOSSO_WALL_START_MODE_TASK(3, 1);
    Engine_EventWait(-result * 60 + 60);
    reactingActor = 0;
  } else {
    Engine_EventWait(30);
    Engine_AudioPlayCue(result + 90);
    Colosso_LoadLettering(4);
    // UB: the void target is called through an incompatible return type.
    /* FAKEMATCH: called through the s32 cast like the call above; as a direct
     * call, 2 instructions differ in tbs-en. */
    COLOSSO_WALL_START_MODE_TASK(3, 0);
    Engine_EventWait(result * 60 + 60);
    reactingActor = 8;
  }
  Engine_ActorShowEmote(reactingActor, 0x105, 0);
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

/// Writes eight uppercase hexadecimal digits followed by a terminator.
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

static UNUSED void ColossoWall_MarkRivalFinished(void) {
  ((struct StageControl *)gSceneState)->status = 9;
}

static void ColossoWall_WaitForRivalFinish(void) {
  s16 *status = &((struct StageControl *)gSceneState)->status;

  while (*status != 9) {
    Engine_TaskWait(1);
  }
}

/// Occasionally spawns a falling particle around the linked finish effect.
static void ColossoWall_SpawnRandomSceneEffect(union FieldObject *fieldObject) {
  SparkSource *source = (SparkSource *)fieldObject;
  s32 position[3];
  u32 chance;

  if (source->f28 >= -255 && source->f28 <= 255) {
    source->f55 = 0;
  }
  chance = Engine_RandomNext();
  if (chance * 100 >> 16 <= 9) {
    SparkSource *particle;
    s32 radius;
    s32 angle;

    SET_VECTOR(position, source->f08, source->f0c, source->f10);
    radius = Engine_RandomNext();
    angle = Engine_RandomNext();
    Vector_AddPolarOffset(radius << 4, angle, position);
    {
      s32 x = position[0];
      s32 y = position[1];
      s32 z = position[2];

      particle = (SparkSource *)Engine_ObjectCreate(285, x, y, z);
    }
    if (particle != 0) {
      particle->f55 = 0;
      Engine_ActorSetSpriteFlags(particle, 0);
      Engine_ObjectSetScript(particle, gColossoWall_SparkScript);
      Object_SetMode(particle, 1);
      Object_SetMode(particle, 0);
    }
  }
}

/// Raises the linked finish effect, starts its script and clears the link.
static s32
ColossoWall_RaiseLinkedSceneEffect(struct ScriptInterpreter *interpreter) {
  RaisedEffect *source = (RaisedEffect *)interpreter;
  RaisedEffect *effect = (RaisedEffect *)Object_GetById(source->f64);

  Engine_ObjectSetPosition(effect, source->f08, source->f0c + 0x240000,
                           source->f10);
  effect->f55 = 0;
  Engine_ObjectSetScript(effect, gColossoWall_RaiseScript);
  Engine_AudioPlayCue(MUS_TREASURE_FOUND);
  source->f64 = 0;
  return 0;
}

/// Approaches the finish item on the unused side and shows the item message.
static s32 ColossoWall_ApproachFinishAndTakeItem(s32 firstItem,
                                                 s32 secondItem) {
  u8 *raceWork = *(u8 **)((u8 *)gWorkSlot + WORK_SLOT_STAGE * 4);
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

  if (*(s32 *)(raceWork + 232) < competitor->x.fixed) {
    x = *(s32 *)(raceWork + 232) + 0xc0000;
  } else {
    x = *(s32 *)(raceWork + 232) - 0xc0000;
  }

  if (finishAlreadyReached != 0) {
    z = *(s32 *)(raceWork + 236) + 0x100000;
    itemActor = (u16 *)(raceWork + 228);
  } else {
    z = *(s32 *)(raceWork + 236) - 0x100000;
    itemActor = (u16 *)(raceWork + 226);
  }

  timer = (s16 *)&competitor->timer;
  *timer = *itemActor;
  competitor->acceleration = 0x4000;
  competitor->speed = 0x10000;

  Engine_ObjectSetPosition(competitor, x, 0, z);
  Engine_GameFlagSet(FLAG_SCENE_TEMP_11);
  Engine_ObjectSetScript(competitor, gColossoWall_ApproachScript);

  while (*timer != 0) {
    Engine_TaskWait(1);
  }

  // Assign the item on the side this competitor reached first.
  if (finishAlreadyReached == 0) {
    // UB: the void target is called through an incompatible return type.
    /* FAKEMATCH: called through a cast to the s32 call type the bytes show; as
     * a call by its void prototype, the argument moves swap and 2 instructions
     * differ in tbs-en. */
    COLOSSO_WALL_APPLY_MATCHING_SLOT_VALUE(0, firstItem);
    UiWork_PushValueSlot(firstItem, 2);
  } else {
    // UB: the void target is called through an incompatible return type.
    /* FAKEMATCH: the same cast as above; as a call by its prototype, 2
     * instructions differ in tbs-en. */
    COLOSSO_WALL_APPLY_MATCHING_SLOT_VALUE(0, secondItem);
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
                                              s32 raceMode, s32 finishX,
                                              s32 finishZ, s32 firstItem,
                                              s32 secondItem) {
  u8 *raceWork;
  struct FieldActor *leader;
  struct FieldActor *rival;
  void *graphics;
  s32 vramSlot;

  raceWork = Runtime_AllocateBlock(59, 0x7170);
  graphics = Runtime_BumpAllocateAlternatePool(512);

  *(u16 *)(raceWork + 222) = leaderId;
  *(u16 *)(raceWork + 224) = rivalId;
  *(u16 *)(raceWork + 226) = firstItem;
  *(u16 *)(raceWork + 228) = secondItem;
  *(u16 *)(raceWork + 230) = raceMode;
  *(s32 *)(raceWork + 232) = finishX;
  *(s32 *)(raceWork + 236) = finishZ;

  leader = Object_GetById(leaderId);
  rival = Object_GetById(rivalId);

  // Mirror the recorded rival start when this is a fresh race entrance.
  if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
    rival->x.fixed = (finishX << 1) - leader->x.fixed;
    rival->z.fixed = leader->z.fixed;
  }

  *(u16 *)(raceWork + 218) = 0;
  *(u16 *)(raceWork + 220) = 0;

  // Upload the gauge graphics and keep only its VRAM slot after freeing
  // scratch.
  FieldResource_DecodeType01(gColosso_GaugeGraphics, graphics);

  vramSlot = Resource_FindFreeEntry();
  *(u16 *)(raceWork + 216) = vramSlot;
  Engine_VramLoad((s16)vramSlot, 512, graphics);

  FieldScheduler_AddCallback(Colosso_DrawGauge, 0xc76);

  Runtime_BumpFree(graphics);
}

/// Decodes the rival's recorded path and starts its playback task.
static void ColossoWall_StartRivalPath(s32 pathResource) {
  u8 *raceWork = *(u8 **)((u8 *)gWorkSlot + WORK_SLOT_STAGE * 4);
  struct StageControl *pathState = (struct StageControl *)gSceneState;

  FieldResource_DecodeType01(Resource_GetTableEntry(pathResource),
                             raceWork + 240);
  if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
    pathState->status = 1;
    pathState->f2 = 1;
    pathState->f4 = *(u16 *)(raceWork + 224);
    pathState->f8 = 0;
    pathState->f6 = 0;
  }
  FieldScheduler_AddCallback(Colosso_UpdatePathRival, 0xc85);
}

#include "common/colosso_scene_state.h"
#include "common/object_findslot.h"

/// Pushes the adjacent obstacle only when the destination and space above are
/// clear.
static void ColossoWall_PushAdjacentObstacle(void) {

  ColossoWallSceneRecord *pusher;
  ColossoWallSceneRecord *obstacle;
  ColossoWallSceneRecord *blocker;
  u32 packedOffset;
  u32 direction;
  Position3 destination;
  s32 zero;
  s32 actorId;

  actorId = gGameState.selectedActor;
  pusher = (ColossoWallSceneRecord *)Object_GetById(actorId);

  direction = pusher->facing >> 12;

  packedOffset = gColossoWall_DirectionSteps[direction];
  destination.x = pusher->x + (packedOffset & 0xffff0000);
  destination.y = pusher->y;
  packedOffset <<= 16;
  destination.z = pusher->z + packedOffset;

  obstacle = SceneData_FindSlotAtPosition((s32 *)&destination, pusher);
  if (obstacle == 0) {
    return;
  }

  // Require a free destination cell and free space immediately above the block.
  packedOffset = gColossoWall_DirectionSteps[direction];
  destination.x = obstacle->x + (packedOffset & 0xffff0000);
  destination.y = obstacle->y;
  packedOffset <<= 16;
  destination.z = obstacle->z + packedOffset;

  blocker = SceneData_FindSlotAtPosition((s32 *)&destination, obstacle);
  if (blocker != 0 && (blocker->flags & 1) != 0) {
    return;
  }

  destination.x = obstacle->x;
  destination.y = obstacle->y + 0x100000;
  destination.z = obstacle->z;

  blocker = SceneData_FindSlotAtPosition((s32 *)&destination, obstacle);
  if (blocker != 0 && (blocker->flags & 1) != 0) {
    return;
  }

  obstacle->state = 2;
  zero = 0;

  packedOffset = gColossoWall_DirectionSteps[direction];
  destination.x = obstacle->x + (packedOffset & 0xffff0000);
  destination.y = obstacle->y;
  packedOffset <<= 16;
  destination.z = obstacle->z + packedOffset;

  if (FieldObject_CheckMovementCollision(obstacle, &destination) > 0) {
    return;
  }

  // Start the push only after all occupancy and terrain checks have passed.
  Object_SetMode(pusher, 8);
  Engine_TaskWait(15);

  obstacle->rateX = 0x3333;
  obstacle->rateZ = 0x3333;
  Engine_ObjectSetPosition(obstacle, destination.x, destination.y,
                           destination.z);

  pusher->rateX = 0x3333;
  pusher->rateZ = 0x3333;
  Engine_ObjectSetPosition(pusher, destination.x, destination.y, destination.z);

  Engine_AudioPlayCue(SE_238);
  FieldObject_WaitForMove(obstacle);
  Engine_AudioPlayCue(SE_288);

  obstacle->x = destination.x;
  obstacle->z = destination.z;
  obstacle->motion24 = zero;
  obstacle->motion2c = zero;

  Object_SetMode(pusher, 1);
}

#include "common/colosso_occupant.h"

const u8 gColossoWall_RollLogScript[] = {2, 2, 3, 3};

// clang-format off
const u8 gColosso_LetteringPaletteOffsets[] = {
    0, 32, 32, 32, 96, 64, 64,
    COLOSSO_WALL_ALTERNATE_PORTRAIT_PALETTE_OFFSET, 128, 0,
};

const s16 gColossoWall_ModeRecordTwo[] = {
    0x1000, 512, 1, 0x2000, 0, 1, 0x1000, 256,
    6, 0x2000, 16, 6, 0x7fff, 30, 0x2000, 0,
    30, 0x7fff, 30, -1,
};

const s16 gColossoWall_ModeRecordThreeAlt[] = {
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

s32 gColossoWall_SparkScript[] = {
    SC_ADD(SW_Y, -0x4000),
    SC_WAIT(1),
    SC_LOOP(60, SC_TOP),
    SC_FREE,
};

s32 gColossoWall_RaiseScript[] = {
    SC_SET(SW_HOOK, (s32)ColossoWall_SpawnRandomSceneEffect),
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

u32 gColossoWall_DirectionSteps[] = {
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

struct SceneEntrance gColossoWallEntrances[] = {
    { 1, CONDITION_ALWAYS, 1272, 0, 168, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { 2, CONDITION_ALWAYS, 1224, 0, 488, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { 10, CONDITION_ALWAYS, 384, 0, 264, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

u32 gColossoWallExits[] = {
    SCENE_EXITS(144),
    SCENE_EXIT(10, 144, 1),
    SCENE_EXIT(11, 140, 68),
    SCENE_EXIT(4, 140, 31),
    SCENE_EXIT(5, 138, 98),
    SCENE_EXITS_END,
};

struct ScenePlacement gColossoWallPlacements[] = {
    { 0x0, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1272), 0, PIXELS(168), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { IVAN, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { MIA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0x3e, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(264), 0, PIXELS(168), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(376), 0, PIXELS(168), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(568), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1272), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1224), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1176), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xcd, CONDITION_ALWAYS, 7,
        PIXELS(264), 0, PIXELS(208), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xcd, CONDITION_ALWAYS, 7,
        PIXELS(392), 0, PIXELS(208), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xcf, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x103, CONDITION_ALWAYS, 7,
        PIXELS(360), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xee, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(520), 0, PIXELS(258), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xe3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(712), 0, PIXELS(280), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xe3, CONDITION_ALWAYS, ACTOR_STAND,
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
    { 0xa5, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1072), 0, PIXELS(280), FACING_WEST, TALK_KEEP_FACING, 0 },
    { 0xed, CONDITION_ALWAYS, 7,
        PIXELS(840), 0, PIXELS(184), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xed, CONDITION_ALWAYS, 7,
        PIXELS(840), 0, PIXELS(232), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x98, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1224), 0, PIXELS(488), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0xa5, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1208), 0, PIXELS(376), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0xa5, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(984), 0, PIXELS(376), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0xa5, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(760), 0, PIXELS(376), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0xa5, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(536), 0, PIXELS(376), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0xa5, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(312), 0, PIXELS(376), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x16, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(104), 0, PIXELS(240), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x16, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(104), 0, PIXELS(272), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

s32 gColossoWall_SpectatorPhase = 0;

s32 gColossoWall_SpectatorTimer = 0;

struct SceneEvent gColossoWallEvents[] = {
    { TOUCH_WALKING_INTO(FACING_SOUTH), 30, CONDITION_ALWAYS, EVENT_SCRIPT(Event_RunRisingSequence) },
    { TOUCH_WALKING_INTO(FACING_WEST), 30, CONDITION_ALWAYS, EVENT_SCRIPT(Event_CheckAhead) },
    { TOUCH_WALKING_INTO(FACING_WEST), 28, CONDITION_ALWAYS, EVENT_SCRIPT(Event_RollEastLogWest) },
    { TOUCH_WALKING_INTO(FACING_EAST), 27, CONDITION_ALWAYS, EVENT_SCRIPT(Event_RollEastLogEast) },
    { TOUCH_WALKING_INTO(FACING_WEST), 24, CONDITION_ALWAYS, EVENT_SCRIPT(Event_RollWestLogWest) },
    { TOUCH_WALKING_INTO(FACING_EAST), 25, CONDITION_ALWAYS, EVENT_SCRIPT(Event_RollWestLogEast) },
    { TOUCH_WALKING, 22, CONDITION_ALWAYS, EVENT_SCRIPT(Event_RideLog) },
    { TOUCH_WALKING, 21, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushRock) },
    { TOUCH_WALKING, 20, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushRockAndUpdateCells) },
    { TOUCH_WALKING, 26, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushRockAndUpdateCells) },
    { EVENT_TOUCH, 50, FLAG_SCENE_TEMP_10, EVENT_SCRIPT(Event_ReachArena) },
    { SEARCH(SEARCH_CHEST), 101, 0x338, FIND_ITEM(ITEM_NUT) },
    { 0x8413, 102, 0x339, FIND_ITEM(ITEM_SMOKE_BOMB) },
    { SEARCH(SEARCH_CHEST), 104, 0x33b, FIND_ITEM(ITEM_NUT) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST) | 0x50000000, FROST_TARGET_WEST, 0x331, EVENT_SCRIPT(Event_FreezeWestTarget) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST) | 0x50000000, FROST_TARGET_EAST, 0x332, EVENT_SCRIPT(Event_FreezeEastTarget) },
    { PSYNERGY_ON_ACTOR(ABILITY_FORCE) | 0x10000000, FORCE_TARGET, 0x330, EVENT_SCRIPT(Event_PrepareForce) },
    { PSYNERGY_ON_ACTOR(ABILITY_FORCE), FORCE_TARGET, 0x330, EVENT_SCRIPT(Event_KnockDownObstacle) },
    { PSYNERGY_ON_ACTOR(ABILITY_GROWTH), GROWTH_PLANT, 0x333, EVENT_SCRIPT(Event_GrowPlant) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 9, CONDITION_ALWAYS, EVENT_SCRIPT(Event_IgnoreMove) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 10, CONDITION_ALWAYS, EVENT_SCRIPT(Event_IgnoreMove) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 11, CONDITION_ALWAYS, EVENT_SCRIPT(Event_UpdateMovedRockCell) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 12, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SaveMovedRockColumns) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 13, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SaveMovedRockColumns) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 14, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SaveMovedRockColumns) },
    { PSYNERGY_ON_ACTOR(ABILITY_HALT), LIFT_OPERATOR, 0x334, EVENT_SCRIPT(Event_HaltLifts) },
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

s16 gColossoWall_ModeRecordDefault[] = {
    0x4000, -188, 2048, 0x1000, COLOSSO_WALL_INTRO_SCALE, 1, 0x2000, 16,
    1, 0x7fff, 14, 0x3000, 0, 20, 0x7fff, 60,
    0x3000, 2048, 20, 0x7fff, 60, -1,
};

s16 gColossoWall_ModeRecordFour[] = {
    0x1000, COLOSSO_WALL_PULSE_FIRST_SCALE, 1, 0x2000, 0, 1, 0x1000, COLOSSO_WALL_PULSE_LOW_SCALE,
    6, 0x2000, 16, 6, 0x7fff, 6, 0x1000, COLOSSO_WALL_PULSE_HIGH_SCALE,
    6, 0x7fff, 6, 0x1000, COLOSSO_WALL_PULSE_LOW_SCALE, 6, 0x7fff, 6,
    0x1000, COLOSSO_WALL_PULSE_HIGH_SCALE, 6, 0x7fff, 6, 0x1000, COLOSSO_WALL_PULSE_LOW_SCALE, 6,
    0x7fff, 6, 0x2000, 0, 30, 0x1000, COLOSSO_WALL_PULSE_HIGH_SCALE, 6,
    0x7fff, 6, 0x1000, COLOSSO_WALL_PULSE_LOW_SCALE, 6, 0x7fff, 6, 0x1000,
    COLOSSO_WALL_PULSE_HIGH_SCALE, 6, 0x7fff, 6, 0x1000, COLOSSO_WALL_PULSE_LOW_SCALE, 6, 0x7fff,
    6, 0x1000, COLOSSO_WALL_PULSE_HIGH_SCALE, 6, 0x7fff, 6, -1,

};

s16 gColossoWall_ModeRecordThree[] = {
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
    SC_CALL(ColossoWall_RaiseLinkedSceneEffect),
    SC_WAIT(3),
    SC_ARG(10),
    SC_WAIT(150),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_ARG(1),
    SC_END,
};

s32 gColossoWall_ApproachScript[] = {
    SC_ARG(2),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(6),
    SC_CALL(ColossoWall_RaiseLinkedSceneEffect),
    SC_WAIT(3),
    SC_ARG(28),
    SC_WAIT(150),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_ARG(1),
    SC_END,
};
