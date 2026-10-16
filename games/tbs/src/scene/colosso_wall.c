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
#include "sound_ids.h"

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
#define COLOSSO_WALL_APPLY_MATCHING_SLOT_VALUE                                 \
  SceneActor_ApplyValueAndMatchingSlots
#else
#define COLOSSO_WALL_SHOW_MESSAGE ((s32 (*)())Engine_EventShowMessage)
#define COLOSSO_WALL_MOVE_ACTOR_PAIR ((s32 (*)())ColossoWall_MovePusherAndLog)
#define COLOSSO_WALL_START_MODE_TASK                                           \
  ((s32 (*)())ColossoWall_StartLetteringAnimation)
#define COLOSSO_WALL_APPLY_MATCHING_SLOT_VALUE                                 \
  ((s32 (*)())SceneActor_ApplyValueAndMatchingSlots)
#endif

// Forward declarations.
static void ColossoWall_IntroduceSecondFinals(s32 attendantId);
static void ColossoWall_ShowRoundStartLettering(s32 mode);
static void ColossoWall_ShowBattleResultLettering(s32 result);
static void SceneState_EmptyHook(void);
static void ColossoWall_WaitForRivalFinish(void);
static s32 ColossoWall_ApproachFinishAndTakeItem(s32 finishItemActorA,
                                                 s32 finishItemActorB);
static void ColossoWall_StartRivalPath(s32 pathResource);
static void ColossoWall_PushAdjacentObstacle(void);

#define WORK_SLOT_STAGE 59

typedef struct ColossoWallSceneRecord {
  u8 pad00[6];
  u16 facing;
  s32 x;
  s32 y;
  s32 z;
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

typedef struct Position3 {
  s32 x;
  s32 y;
  s32 z;
} Position3;

struct StageControl {
  s16 status;
  s16 f2;
  s16 f4;
  s16 f6;
  s16 f8;
};

// Set the party leader's walking speed and acceleration.
static inline void InitializeActorZero(void) {
  Engine_ActorSetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
}

// Set the specified actor's walking speed and acceleration.
static inline void InitializeSelectedActor(s32 actorId) {
  Engine_ActorSetSpeed(actorId, 0x10000, 0x8000);
}

// Return the entrance rows for the second Colosso finals course.
const struct SceneEntrance *Scene_GetEntrances(void) {
  return gColossoWallEntrances;
}

// This course has no placement-clipping regions.
const struct SceneRegion *Scene_GetRegions(void) { return 0; }

// Return the packed course and battle-result exits.
const u32 *Scene_GetExits(void) { return gColossoWallExits; }

// Return the course's competitors, obstacles and attendants.
const struct ScenePlacement *Scene_GetPlacements(void) {
  return gColossoWallPlacements;
}

// Leave these actors unchanged when Move targets them.
static void ColossoWall_IgnoreMove(void) {}

// Push the obstacle immediately ahead of the selected actor.
static void ColossoWall_PushObstacleAhead(void) {
  ColossoWall_PushAdjacentObstacle();
}

// Update the first movable obstacle's collision after Move.
static void ColossoWall_UpdateMovedObstacleCollision(void) {
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

// Push an obstacle, then restore the first obstacle's collision cell.
static void ColossoWall_PushAndUpdateObstacleCollision(void) {
  ColossoWall_PushAdjacentObstacle();
  ColossoWall_UpdateMovedObstacleCollision();
}

// Save the movable obstacles' columns and rebuild their collision cells.
static void ColossoWall_SaveMovedObstacleColumns(void) {

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

// Enable the child effect for Force on the course obstacle.
static void ColossoWall_EnableForceChildEffect(void) {
  u8 *effectWork = gEffectWork;

  ((struct BattleFxScene *)effectWork)->childMode = 1;
}

// Spin the Force obstacle down into place and open its collision cells.
static void ColossoWall_KnockDownForceObstacle(void) {
  struct FieldActor *obstacle;
  s32 fallFrames;
  s32 rotationStep;
  s32 settleFrames;
  s32 settleRotationStep;
  struct AnimationObject *sprite;

  obstacle = Object_GetById(30);
  sprite = obstacle->sprite;
  GameFlag_Set(0x330);
  obstacle->acceleration = 0x1999;
  obstacle->speed = 0x13333;
  Engine_AudioPlayCue(227);
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
  Engine_AudioPlayCue(240);
  {
    s32 rotation = 0;

    sprite->rotation = rotation;
  }
  Engine_ActorSetAnimation(30, 4);
  obstacle->x.fixed = 0x1500000;
  obstacle->y.fixed = -0x80000;
  obstacle->z.fixed = 0x1080000;
  obstacle->velocityY = 0;
  obstacle->velocityX = 0;
  // Keep both the course and its saved collision strip in the new state.
  Map_CopyCellAttributes(19, 16, 1, 1, 20, 16);
  Map_CopyCellAttributes(20, 80, 1, 1, 21, 80);
}

// Stop the western Frost obstacle and update the frozen crossing.
static void ColossoWall_FreezeWestWaterObstacle(void) {
  struct FieldActor *obstacle;

  Engine_GameFlagSet(0x331);
  obstacle = Object_GetById(20);
  obstacle->motionFlags = 0;
  {

    Map_CopyCellAttributes(46, 17, 1, 1, 44, 17);
  }
}

// Stop the eastern Frost obstacle and update the frozen crossing.
static void ColossoWall_FreezeEastWaterObstacle(void) {
  struct FieldActor *obstacle;

  Engine_GameFlagSet(0x332);
  obstacle = Object_GetById(21);
  obstacle->motionFlags = 0;
  {

    Map_CopyCellAttributes(46, 17, 1, 1, 50, 17);
  }
}

// Record Growth on the obstacle and restore its climbable collision strip.
static void ColossoWall_UpdateGrowthObstacleCollision(void) {
  Engine_GameFlagSet(0x333);
  {

    Map_CopyCellAttributes(32, 37, 1, 4, 32, 77);
  }
}

// Run the rising-object sequence for the actor touching the obstacle.
static void ColossoWall_RunSelectedActorRisingEffect(void) {
  FieldBattleFx_RunRisingObjectSequence(gGameState.selectedActor, 6, 0);
}

// Let the field engine check the obstacle ahead of the leader.
static void ColossoWall_CheckLeaderAhead(void) { Engine_LeaderCheckAhead(); }

// Move a riding log and its pusher to aligned cell centers.
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
  Engine_AudioPlayCue(226);
  FieldObject_WaitForMove(pusher);
  Engine_AudioPlayCue(288);
  Object_SetMode(log, 2);
  Engine_EventEnd();
}

// Move the aligned riding log in the direction the player holds.
static void ColossoWall_PushAlignedRidingLog(void) {
  struct FieldActor *pusher = Object_GetById(gGameState.selectedActor);
  s32 column = pusher->x.fixed >> 20;
  s32 offsetX = 0;
  s32 logId = 32;

  if (pusher->z.fixed >> 20 > 12) {
    logId = 33;
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
    column = Object_GetById(32)->x.fixed >> 20;
    Map_CopyCellAttributes(52, 28, 1, 3, column, 10);
    column = Object_GetById(33)->x.fixed >> 20;
    Map_CopyCellAttributes(52, 28, 1, 3, column, 13);
  }
}

// Roll a log to half-cell coordinates while its pusher follows halfway.
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
  ObjectDispatch_InitFromTable4WithArgument(
      *(struct DispatchObject **)((u8 *)Runtime_AllocateBlock(27, 0xccc) + 480),
      (s32)log);
  Engine_ActorSetSpeed(pusher, 0x8000, 0x3333);
  Object_SetMode(pusherActor, 2);
  Engine_ObjectSetPosition(pusherActor, pusherActor->x.fixed + offsetX, 0,
                           pusherActor->z.fixed + offsetZ);
  Engine_AudioPlayCue(239);
  FieldObject_WaitForMove(pusherActor);
  Object_SetMode(pusherActor, 1);
  FieldObject_WaitForMove(log);
  Engine_AudioPlayCue(288);
  Engine_AudioPlayCue(213);
  Object_SetMode(log, 1);
  Engine_EventWait(15);
  Engine_EventEnd();
}

// Rebuild the three rolling logs' collision rectangles at their positions.
static void ColossoWall_UpdateRollingLogCollision(void) {
  s32 cell;

  Map_CopyCellAttributes(100, 11, 12, 4, 14, 11);

  cell = Object_GetById(15)->x.fixed >> 20;
  Map_CopyCellAttributes(13, 28, 1, 4, cell, 11);

  cell = Object_GetById(16)->x.fixed >> 20;
  Map_CopyCellAttributes(13, 28, 1, 4, cell, 11);

  cell = Object_GetById(17)->z.fixed >> 20;
  Map_CopyCellAttributes(13, 28, 4, 1, 18, cell);
}

// Roll the western log toward its western stopping point.
static void ColossoWall_RollWestLogWest(void) {
  ColossoWall_RollLogToCell(15, 29, 26);
  ColossoWall_UpdateRollingLogCollision();
}

// Roll the western log toward its eastern stopping point.
static void ColossoWall_RollWestLogEast(void) {
  ColossoWall_RollLogToCell(15, 33, 26);
  ColossoWall_UpdateRollingLogCollision();
}

// Roll the eastern log toward its western stopping point.
static void ColossoWall_RollEastLogWest(void) {
  ColossoWall_RollLogToCell(16, 45, 26);
  ColossoWall_UpdateRollingLogCollision();
}

// Roll the eastern log toward its eastern stopping point.
static void ColossoWall_RollEastLogEast(void) {
  ColossoWall_RollLogToCell(16, 49, 26);
  ColossoWall_UpdateRollingLogCollision();
}

// Roll the center log north and rebuild the log collision rectangles.
static UNUSED void ColossoWall_RollCenterLogNorth(void) {
  ColossoWall_RollLogToCell(17, 40, 23);
  ColossoWall_UpdateRollingLogCollision();
}

// Roll the center log south and rebuild the log collision rectangles.
static UNUSED void ColossoWall_RollCenterLogSouth(void) {
  ColossoWall_RollLogToCell(17, 40, 25);
  ColossoWall_UpdateRollingLogCollision();
}

// Carry the selected actor on the moving platforms and alternate their routes.
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
      FieldObject_SetPosition(platformActor, 0x3a80000, 0, 0xb80000);
      platformActor = Object_GetById(23);
      FieldObject_SetPosition(platformActor, 0x3c80000, 0, 0xf80000);
      platformActor = Object_GetById(24);
      FieldObject_SetPosition(platformActor, 0x3e80000, 0, 0xb80000);
      platformActor = Object_GetById(25);
      FieldObject_SetPosition(platformActor, 0x4080000, 0, 0xf80000);
      Engine_ActorSetAnimation(31, 11);
    } else {
      platformActor = Object_GetById(22);
      FieldObject_SetPosition(platformActor, 0x3a80000, 0, 0xd80000);
      platformActor = Object_GetById(23);
      FieldObject_SetPosition(platformActor, 0x3c80000, 0, 0xd80000);
      platformActor = Object_GetById(24);
      FieldObject_SetPosition(platformActor, 0x3e80000, 0, 0xd80000);
      platformActor = Object_GetById(25);
      FieldObject_SetPosition(platformActor, 0x4080000, 0, 0xd80000);
      Engine_ActorSetAnimation(31, 10);
    }
  }
  gColossoWall_SpectatorTimer++;
  if ((u32)gColossoWall_SpectatorTimer >= COLOSSO_WALL_PLATFORM_CYCLE_FRAMES) {
    if (GameFlag_IsSet(0x104) == 0) {
      gColossoWall_SpectatorTimer = 0;
    }
  }
}

// Stop the moving-platform task and restore its starting row.
static void ColossoWall_ResetMovingPlatforms(void) {
  gColossoWall_SpectatorTimer = 0;
  gColossoWall_SpectatorPhase = 0;
  Engine_TaskRemoveCallback(ColossoWall_UpdateMovingPlatforms);
  Actor_SetPosition(22, 0x3a80000, 0xd80000);
  Actor_SetPosition(23, 0x3c80000, 0xd80000);
  Actor_SetPosition(24, 0x3e80000, 0xd80000);
  Actor_SetPosition(25, 0x4080000, 0xd80000);
  Engine_ActorSetAnimation(31, 10);
}

// Start the moving-platform task and release its pause flag.
static void ColossoWall_StartMovingPlatforms(void) {
  FieldScheduler_AddCallback(ColossoWall_UpdateMovingPlatforms, 0xC85);
  Engine_GameFlagClear(0x104);
}

// Wait for the demonstration's crossing phase, with a ten-second timeout.
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

// Halt the platform controller and leave a fixed crossing behind.
static void ColossoWall_HaltMovingPlatforms(void) {
  BattleEffect_PauseObject(31);
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

// Take the finish item, wait for the rival and begin the finals battle.
static void ColossoWall_StartFinalsBattle(void) {
  s32 rivalFinishedFirst;
  s32 refreshCount;
  s32 resultBase;

  resultBase = 2;
  SceneState_EmptyHook();
  Engine_EventBegin();
  // Collect the finish item and let the recorded rival finish its approach.
  rivalFinishedFirst = ColossoWall_ApproachFinishAndTakeItem(77, 89);
  ColossoWall_WaitForRivalFinish();
  refreshCount = 9;
  do {
    Object_RefreshSelectorById(8);
    refreshCount = (refreshCount - 1);
  } while (refreshCount >= 0);
  // Bring the competitors together before selecting the battle's result bias.
  Actor_SetSpeed(8, 0x10000, 0x8000);
  Actor_WalkTo(8, 88, 0x100);
  Actor_SetSpeed(0, 0x10000, 0x8000);
  Actor_WalkToAndWait(0, 120, 0x100);
  Engine_ActorSetAnimation(8, 1);
  Engine_ActorFaceEachOther(0, 8, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimation(8, 3);
  Engine_ActorSetAnimationAndWait(0, 3);
  Engine_EventWait(20);
  Actor_SetSpeed(0, 0x20000, 0x10000);
  Actor_SetSpeed(8, 0x20000, 0x10000);
  Actor_WalkTo(0, 112, 0x100);
  Engine_ActorWalkToAndWait(8, 96, 0x100);
  Engine_ActorSetAnimation(0, 16);
  Engine_ActorSetAnimation(8, 9);
  Engine_EventWait(10);
  /* FAKEMATCH: resultBase is set to 2 at the top of the function, so resultBase
   * - rivalFinishedFirst + 1 is not folded; written 3 - rivalFinishedFirst or 2
   * - rivalFinishedFirst + 1 it differs 10 on tbs-en, two instructions fewer.
   */
  Field_RequestBattleFromEncounterTable(72,
                                        resultBase - rivalFinishedFirst + 1);
  Battle_SetStart(3);
  Event_SetNextDestination((s32)&SceneId_ColossoWall, 4);
  Event_SetDefeatDestination((s32)&SceneId_ColossoWall, 5);
  Engine_GameFlagSet(0x11a);
}

// Return the course's obstacle interactions and attendant dialogue.
const struct SceneEvent *Scene_GetEvents(void) { return gColossoWallEvents; }

// Stage the competitors and show the battle-result lettering.
static void ColossoWall_StageBattleResult(s32 result) {

  Engine_ActorDestroy(40);
  Engine_ActorDestroy(41);
  Party_RestoreHpPp(1);
  Engine_EventBegin();
  Actor_SetPosition(8, 0x580000, 0x1000000);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0x780000, 0x1000000);
  Actor_FaceActor(8, 0x4000, 0);
  Actor_FaceActor(ACTOR_PARTY_LEADER, 0x4000, 0);
  if (result < 0) {
    Engine_ActorSetAnimation(8, 10);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 35);
  } else {
    Engine_ActorSetAnimation(8, 8);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 28);
  }
  Engine_TaskWait(1);
  Camera_MoveTo(0x680000, 0, 0xc00000, 0);
  ColossoWall_ShowBattleResultLettering(result);
  Engine_EventEnd();
}

// Restore obstacle progress and set up the selected course entrance.
s32 ColossoWall_ApplyEntryState(void) {
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
  Engine_GameFlagSet(0x144);
  Map_CopyCellAttributes(14, 11, 12, 4, 100, 11);
  Map_CopyCellAttributes(48, 10, 5, 6, 120, 10);
  for (obstacleId = 26; obstacleId <= 30; obstacleId++) {
    actor = Object_GetById(obstacleId);
    Object_SetMode(actor, 4);
    actor->motionFlags = 0;
    actor->y.fixed = 0;
    actor->priorityFlags = 2;
  }
  Object_GetById(18)->priorityFlags = 2;
  if (Engine_GameFlagIsSet(0x330)) {
    actor = Object_GetById(30);
    actor->x.fixed = 0x1500000;
    actor->y.fixed = -0x80000;
    actor->z.fixed = 0x1080000;
    Map_CopyCellAttributes(19, 16, 1, 1, 20, 16);
    Map_CopyCellAttributes(20, 80, 1, 1, 21, 80);
  } else {
    actor = Object_GetById(30);
    Object_SetMode(actor, 3);
    actor->y.fixed = 0x100000;
  }
  Object_GetById(11)->priorityFlags = 2;
  zero = 0;
  if (Engine_GameFlagIsSet(0x335)) {
    Map_CopyCellAttributes(35, 78, 1, 1, 35, 77);
  }
  if (Engine_GameFlagIsSet(0x333)) {
    Engine_ActorSetAnimation(19, 4);
    Map_CopyCellAttributes(32, 37, 1, 4, 32, 77);
  }
  if (Engine_GameFlagIsSet(0x331)) {
    Object_GetById(20)->motionFlags = zero;
    Object_GetById(20)->priorityFlags = 2;
    Engine_ActorSetAnimation(20, 5);
    Map_CopyCellAttributes(46, 17, 1, 1, 44, 17);
  }
  if (Engine_GameFlagIsSet(0x332)) {
    Object_GetById(21)->motionFlags = zero;
    Object_GetById(21)->priorityFlags = 2;
    Engine_ActorSetAnimation(21, 5);
    Map_CopyCellAttributes(46, 17, 1, 1, 50, 17);
  }
  actor = Object_GetById(32);
  column = actor->x.fixed >> 20;
  actor->motionFlags = zero;
  actor->priorityFlags = 2;
  Map_CopyCellAttributes(52, 28, 1, 3, column, 10);
  actor = Object_GetById(33);
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
  Engine_ActorSetAnimation(31, 10);
  if (Engine_GameFlagIsSet(0x334)) {
    for (platformId = COLOSSO_WALL_FIRST_PLATFORM, row = 13, columnOffset = 58;
         platformId <= COLOSSO_WALL_LAST_PLATFORM;
         platformId++, columnOffset += 2) {
      actor = Object_GetById(platformId);
      actor->priorityFlags = 2;
      Object_SetMode(actor, 4);
      Map_CopyCellAttributes(56, 13, 1, 1, columnOffset, row);
    }
    Engine_ActorSetAnimation(31, 10);
    BattleEffect_PauseObject(31);
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
  Engine_ActorSetAnimation(8, 9);
  gGameState.movementMode = 0;
  Colosso_ShowItemIcon(41, 89);
  Colosso_ShowItemIcon(40, 77);
  Engine_ActorSetChildValue(8, 1);
  // Entrances distinguish the race, solo practice, briefing and battle results.
  switch (gGameState.entrance) {
  case 1:
    FieldScene_BuildDescriptorAndInstallTask(0, 8, 5, 0x680000, 0x1000000, 40,
                                             41);
    Map_CopyCellsLayered(127, 0, 1, 2, 79, 6);
    Engine_ActorDestroy(34);
    Engine_ActorDestroy(35);
    Engine_ActorDestroy(36);
    Engine_ActorDestroy(37);
    Engine_ActorDestroy(38);
    Engine_ActorDestroy(39);
    if (!Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION)) {
      Engine_AudioPlayCue(17);
      Colosso_SelectSoloCompetitor(0);
      SceneActor_PlacePartyAtSavedTiles();
      ColossoWall_ShowRoundStartLettering(2);
    }
    Object_LinkObjectAndSetCallback(1, 0);
    Object_LinkObjectAndSetCallback(2, 0);
    Object_LinkObjectAndSetCallback(3, 0);
    ColossoWall_StartRivalPath((s32)&ResourceId_RivalPathB);
    break;
  case 2:
    FieldScheduler_AddCallback(SceneState_RaiseTriggerOnPsynergyTarget, 0xc80);
    Engine_ActorDestroy(40);
    Engine_ActorDestroy(41);
    if (!Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION)) {
      SceneActor_PlacePartyAtSavedTiles();
      Colosso_SelectSoloCompetitor(1);
      ColossoWall_ShowRoundStartLettering(0);
    }
    break;
  case 3:
    if (!Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION)) {
      ColossoWall_IntroduceSecondFinals(34);
      Colosso_ClearSavedActorPositions();
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

// Introduce the second finals course, then gather the party behind Isaac.
static void ColossoWall_IntroduceSecondFinals(s32 attendantId) {
  s32 attendantZ;
  s32 attendantX;
  struct FieldActor *actor;

  actor = Object_GetById(attendantId);
  attendantX = actor->x.part.pixel;
  attendantZ = actor->z.part.pixel;
  Engine_EventBegin();
  Actor_SetSpeed(attendantId, 0x10000, 0x8000);
  Actor_SetSpeed(0, 0x10000, 0x8000);
  Actor_SetSpeed(1, 0x10000, 0x8000);
  Actor_SetSpeed(2, 0x10000, 0x8000);
  Actor_SetSpeed(3, 0x10000, 0x8000);
  Engine_ActorSetPosition(0, (attendantX << 16),
                          ((attendantZ << 16) - 0x300000));
  Engine_ActorSetPosition(1, (attendantX << 16) - 0x100000,
                          (attendantZ << 16) - 0x280000);
  Engine_ActorSetPosition(2, (attendantX << 16) + 0x100000,
                          (attendantZ << 16) - 0x280000);
  Engine_ActorSetPosition(3, (attendantX << 16),
                          ((attendantZ << 16) - 0x200000));
  Engine_ActorSetPosition(attendantId, (attendantX << 16),
                          ((attendantZ << 16) - 0x500000));
  actor = Object_GetById(0);
  actor->facing = 0xc000;
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
  Engine_ActorSetAnimationAndWait(0, 3);
  Engine_EventWait(6);
  Engine_ActorSetAnimation(1, 2);
  actor = Object_GetById(0);
  if (actor != 0) {
    Engine_ActorSetDestination(1, actor->x.part.pixel, actor->z.part.pixel);
  }
  Engine_ActorSetAnimation(2, 2);
  actor = Object_GetById(0);
  if (actor != 0) {
    Engine_ActorSetDestination(2, actor->x.part.pixel, actor->z.part.pixel);
  }
  Engine_ActorSetAnimation(3, 2);
  actor = Object_GetById(0);
  if (actor != 0) {
    Engine_ActorSetDestination(3, actor->x.part.pixel, actor->z.part.pixel);
  }
  Engine_ActorWalkToAndWait(attendantId, (attendantX - 16), (attendantZ - 64));
  Engine_ActorSetPosition(1, 0, 0);
  Engine_ActorSetPosition(2, 0, 0);
  Engine_ActorSetPosition(3, 0, 0);
  Engine_ActorWalkToAndWait(attendantId, (attendantX - 16), (attendantZ - 16));
  Engine_ActorWalkToAndWait(attendantId, attendantX, attendantZ);
  Engine_ActorFaceDirection(attendantId, 0xc000, 10);
  Engine_EventEnd();
}

// Explain and demonstrate the Mini-Maze, or finish the solo round.
static void ColossoWall_ExplainMiniMaze(s32 speaker) {
  s32 result;
  struct FieldActor *actor;

  if (gGameState.entrance == 2) {
    Colosso_FinishSoloRound(speaker);
  } else {
    Engine_EventBegin();
    result = SceneDialogue_RunFlagGatedPromptInteraction(speaker, 1);
    if (result == 0) {
      Engine_EventSetMessage(MsgColossoStageDubbedMini);
      Engine_CameraSetSpeed(0x30000, 0x6000);
      Engine_CameraMoveTo(0x4c80000, -1, 0xb80000, 1);
      Engine_CameraWaitForMove();
      Engine_EventShowMessage(speaker, 0);
      Colosso_FadeInCompetitor(ACTOR_PARTY_LEADER, 0x4f8, 168);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x18000, 0xc000);
      OverlayObject_PlaceWithScale14000(ACTOR_PARTY_LEADER, 0x508, 184);
      OverlayObject_PlaceWithScale14000(ACTOR_PARTY_LEADER, 0x508, 216);
      OverlayObject_PlaceWithScale14000(ACTOR_PARTY_LEADER, 0x4c8, 216);
      Engine_EventShowMessage(speaker, 0);
      OverlayObject_PlaceWithScale14000(ACTOR_PARTY_LEADER, 0x4c8, 248);
      OverlayObject_PlaceWithScale14000(ACTOR_PARTY_LEADER, 0x4a8, 248);
      Engine_EventWait(3);
      actor = Object_GetById(ACTOR_PARTY_LEADER);
      actor->velocityY = 0x40000;
      Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 28);
      Engine_ActorSetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
      Engine_EventWait(30);
      Engine_EventShowMessage(speaker, 0);
      Colosso_RestoreCompetitor(ACTOR_PARTY_LEADER);
      Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
      Colosso_ShowFollowUpPrompt(speaker, 1);
    } else if (result == 1) {
      Engine_EventSetMessage(MsgColossoMainThingStage);
      Engine_EventShowMessage(speaker, 0);
    }
    FieldScene_RunMiddleSequence(result, speaker, 1);
    Engine_EventEnd();
  }
}

// Explain and demonstrate the shifting floor, or finish the solo round.
static void ColossoWall_ExplainShiftingFloor(s32 speaker) {
  s32 result;

  if (gGameState.entrance == 2) {
    Colosso_FinishSoloRound(speaker);
  } else {
    Engine_EventBegin();
    result = SceneDialogue_RunFlagGatedPromptInteraction(speaker, 2);
    if (result == 0) {
      Engine_EventSetMessage(MsgColossoShiftingFloorStage);
      ColossoWall_ResetMovingPlatforms();
      Camera_SetSpeed(0x30000, 0x6000);
      Camera_MoveTo(0x3d80000, -1, 0xe80000, 1);
      Engine_CameraWaitForMove();
      // UB: the void target is called through an incompatible return type.
      /* FAKEMATCH: called through a cast to the s32 call type the bytes show,
       * its result discarded (as call.h's Value2 did, 2026-10-13); called
       * directly or through Call2 it differs 2 on tbs-en.
       * Direct prototype rechecked 2026-10-16: 2 argument instructions
       * differ in tbs-en. */
      COLOSSO_WALL_SHOW_MESSAGE(speaker, 0);
      ColossoWall_StartMovingPlatforms();
      Event_ShowMessage(speaker, 0);
      Colosso_FadeInCompetitor(0, 0x438, 0x108);
      Engine_EventWait(15);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x18000, 0xc000);
      OverlayObject_PlaceWithScale14000(0, 0x438, 216);
      OverlayObject_PlaceWithScale14000(0, 0x428, 216);
      ColossoWall_WaitForPlatformCrossingPhase();
      Engine_LeaderCheckAhead();
      Camera_MoveTo(-1, -1, -1, 0);
      Event_ShowMessage(speaker, 0);
      Colosso_RestoreCompetitor(0);
      Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
      Colosso_ShowFollowUpPrompt(speaker, 2);
    } else {
      if (result == 1) {
        Engine_EventSetMessage(MsgColossoOperatorLiftsWillCheerFor);
        Event_ShowMessage(speaker, 0);
      }
    }
    FieldScene_RunMiddleSequence(result, speaker, 2);
    Engine_EventEnd();
  }
}

// Explain and demonstrate riding logs, or finish the solo round.
static void ColossoWall_ExplainLogRolling(s32 speaker) {
  s32 result;

  if (gGameState.entrance == 2) {
    Colosso_FinishSoloRound(speaker);
  } else {
    Engine_EventBegin();
    result = SceneDialogue_RunFlagGatedPromptInteraction(speaker, 3);
    if (result == 0) {
      Engine_EventSetMessage(MsgColossoLogRollingStage);
      Camera_SetSpeed(0x30000, 0x6000);
      Camera_MoveTo(0x2f00000, -1, 0xc00000, 1);
      Engine_CameraWaitForMove();
      Engine_EventWait(60);
      Camera_SetSpeed(0x10000, 0x2000);
      Camera_MoveTo(0x2f00000, -1, 0xe00000, 1);
      Engine_CameraWaitForMove();
      Event_ShowMessage(speaker, 0);
      Colosso_FadeInCompetitor(0, 0x358, 0x108);
      Engine_EventWait(10);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x18000, 0xc000);
      OverlayObject_PlaceWithScale14000(0, 0x358, 0x108);
      OverlayObject_PlaceWithScale14000(0, 0x358, 232);
      Event_ShowMessage(speaker, 0);
      OverlayObject_PlaceWithScale14000(0, 0x348, 232);
      Engine_EventWait(10);
      // UB: the void target is called through an incompatible return type.
      /* FAKEMATCH: called through a cast to the s32 call type the bytes show,
       * its result discarded (as call.h's Value3 did, 2026-10-13); called
       * directly or through Call3 it differs 2 on tbs-en.
       * Direct prototype rechecked 2026-10-16: 2 argument instructions
       * differ in tbs-en. */
      COLOSSO_WALL_MOVE_ACTOR_PAIR(33, -64, 0);
      Camera_MoveTo(0x2f00000, -1, 0xd80000, 1);
      Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
      Engine_EventWait(10);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x2f8, 232);
      Engine_EventWait(10);
      Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 30);
      Event_ShowMessage(speaker, 0);
      Colosso_RestoreCompetitor(0);
      Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
      Actor_SetPosition(33, 0x3480000, 0xe80000);
      Colosso_ShowFollowUpPrompt(speaker, 3);
    } else if (result == 1) {
      Engine_EventSetMessage(MsgColossoHereYourObjectiveRideLogs);
      Event_ShowMessage(speaker, 0);
    }
    FieldScene_RunMiddleSequence(result, speaker, 3);
    Engine_EventEnd();
  }
}

// Explain and demonstrate scaling the Wall, or finish the solo round.
static void ColossoWall_ExplainWallClimb(s32 speaker) {
  s32 z;
  struct FieldActor *actor;
  s32 result;

  if (gGameState.entrance == 2) {
    Colosso_FinishSoloRound(speaker);
    return;
  }
  Engine_EventBegin();
  result = SceneDialogue_RunFlagGatedPromptInteraction(speaker, 4);
  if (result == 0) {
    Engine_EventSetMessage(MsgColossoFansJustCallWall);
    Engine_CameraSetSpeed(0x30000, 0x6000);
    Camera_MoveTo(0x2180000, -1, 0xf00000, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(45);
    Camera_SetSpeed(0x10000, 0x2000);
    Camera_MoveTo(0x2180000, -1, 0xc00000, 1);
    z = 264;
    Engine_CameraWaitForMove();
    Engine_EventShowMessage(speaker, 0);
    Colosso_FadeInCompetitor(ACTOR_PARTY_LEADER, 632, z);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 616, z);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 20);
    Field_ClimbUp();
    Camera_SetSpeed(0x4000, 0x800);
    Camera_MoveTo(0x2180000, -1, 0xa00000, 1);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 10);
    actor = Object_GetById(ACTOR_PARTY_LEADER);
    Engine_ObjectSetPosition(actor, actor->x.fixed, actor->y.fixed + 0x400000,
                             actor->z.fixed);
    Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
    Field_ClimbUp();
    Camera_MoveTo(-1, -1, -1, 0);
    Event_ShowMessage(speaker, 0);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x18000, 0xc000);
    OverlayObject_PlaceWithScale14000(ACTOR_PARTY_LEADER, 488, 248);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 20);
    FieldBattleFx_RunRisingObjectSequence(ACTOR_PARTY_LEADER, 6, 0);
    Engine_CameraMoveTo(0x2180000, -1, 0xa00000, 1);
    Engine_EventShowMessage(speaker, 0);
    Colosso_RestoreCompetitor(ACTOR_PARTY_LEADER);
    Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
    Colosso_ShowFollowUpPrompt(speaker, 4);
  } else if (result == 1) {
    Engine_EventSetMessage(MsgColossoScalingWallQuickly);
    Engine_EventShowMessage(speaker, 0);
  }
  FieldScene_RunMiddleSequence(result, speaker, 4);
  Engine_EventEnd();
}

// Move a demonstration obstacle to pixel coordinates while its pusher follows.
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
  Engine_AudioPlayCue(239);
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
  Engine_AudioPlayCue(288);
  Engine_AudioPlayCue(213);
  Engine_EventWait(15);
}

// Explain and demonstrate the Board Walk, or finish the solo round.
static void ColossoWall_ExplainBoardWalk(s32 speaker) {
  s32 result;

  if (gGameState.entrance == 2) {
    Colosso_FinishSoloRound(speaker);
    return;
  }
  Engine_EventBegin();
  result = SceneDialogue_RunFlagGatedPromptInteraction(speaker, 5);
  if (result == 0) {
    Engine_EventSetMessage(MsgColossoPlaceCalledBoard);
    Engine_CameraSetSpeed(0x20000, 0x4000);
    Camera_MoveTo(0x1480000, -1, 0x1080000, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(30);
    Camera_SetSpeed(0x18000, 0x3000);
    Camera_MoveTo(0x1380000, -1, 0xb00000, 1);
    Engine_CameraWaitForMove();
    Engine_EventShowMessage(speaker, 0);
    Colosso_FadeInCompetitor(ACTOR_PARTY_LEADER, 408, 264);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x18000, 0xc000);
    OverlayObject_PlaceWithScale14000(ACTOR_PARTY_LEADER, 408, 216);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 10);
    Engine_EventShowMessage(speaker, 0);
    ColossoWall_PushBlockToCell(16, 360, 208);
    Engine_ActorShowEmote(ACTOR_PARTY_LEADER, 264, 45);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x18000, 0xc000);
    OverlayObject_PlaceWithScale14000(ACTOR_PARTY_LEADER, 376, 216);
    OverlayObject_PlaceWithScale14000(ACTOR_PARTY_LEADER, 376, 248);
    OverlayObject_PlaceWithScale14000(ACTOR_PARTY_LEADER, 312, 248);
    Engine_EventShowMessage(speaker, 0);
    Colosso_RestoreCompetitor(ACTOR_PARTY_LEADER);
    Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
    Actor_SetPosition(16, 0x1880000, 0xd00000);
    Colosso_ShowFollowUpPrompt(speaker, 5);
  } else if (result == 1) {
    Engine_EventSetMessage(MsgColossoFocusRollingLogs);
    Engine_EventShowMessage(speaker, 0);
  }
  FieldScene_RunMiddleSequence(result, speaker, 5);
  Engine_EventEnd();
}

// Shared source is included here in ROM order.
#include "common/colosso_solo.h"
#include "common/item_equip.h"
#include "common/colosso_task.h"

// Choose the lettering animation script and reset its task state.
static void ColossoWall_StartLetteringAnimation(u32 mode, u32 tileVariant) {
  s32 script;

  gColosso_ModeTaskMode = mode;
  gColosso_ModeTaskParam = (tileVariant << 4);

  {
    s32 priority = 0xc80;
    FieldScheduler_AddCallback(Colosso_UpdateModeTask, priority);
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

  gColosso_ModeTaskTimer = 0;
  gColosso_ModeTaskScript = (s16 *)script;
  gColosso_ModeMoveTarget = 0;
  gColosso_ModeMoveDuration = 0;
  gColosso_ModeTaskPosition = 0;
}

// Show the course-opening lettering and play its timed audio cues.
static void ColossoWall_ShowRoundStartLettering(s32 mode) {
  if (mode == 0) {
    Engine_EventBegin();
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(30);
    Engine_AudioPlayCue(0x59);
    Colosso_LoadPortrait(0);
    ColossoWall_StartLetteringAnimation(1, 0);
    Engine_EventWait(120);
    Engine_EventEnd();
    return;
  }

  Engine_AudioPlayCue(0xf7);
  Engine_EventBegin();
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  {
    s16 *openingScript = gColossoWall_ModeRecordDefault;
    *(s16 *)((u8 *)openingScript + 30) = (mode * 60);
  }
  Engine_EventWait(30);
  Engine_AudioPlayCue(mode + 0x5a);
  Colosso_LoadPortrait(mode);
  ColossoWall_StartLetteringAnimation(1, 0);
  Engine_EventWait(120);

  // Finish the opening cue before the final lettering pulses and start signal.
  while (AudioCommand_GetStateByte() != 0) {
    Engine_TaskWait(1);
  }

  Engine_AudioPlayCue(0x121);
  Colosso_LoadPortrait(5);
  ColossoWall_StartLetteringAnimation(2, 0);
  Engine_AudioPlayCue(0xec);
  Engine_EventWait(60);
  ColossoWall_StartLetteringAnimation(2, 1);
  Engine_AudioPlayCue(0xec);
  Engine_EventWait(60);
  Colosso_LoadPortrait(6);
  ColossoWall_StartLetteringAnimation(2, 0);
  Engine_AudioPlayCue(0xec);
  Engine_EventWait(60);
  Colosso_LoadPortrait(7);
  ColossoWall_StartLetteringAnimation(4, 0);
  Engine_AudioPlayCue(0xed);
  Audio_PlayCueFromEventWork();
  Engine_EventEnd();
  Engine_GameFlagSet(0x123);
}

// Show the win-or-loss lettering and the appropriate competitor's reaction.
static void ColossoWall_ShowBattleResultLettering(s32 result) {

  s32 reactingActor;

  Engine_AudioPlayCue(247);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  gColossoWall_ModeRecordThree[13] = result * 60;
  STORE_CONST(gColossoWall_ModeRecordThreeAlt[13]) =
      (result < 0 ? -result : result) * 60;
  if (result < 0) {
    Engine_EventWait(30);
    Engine_AudioPlayCue(86);
    Colosso_LoadPortrait(8);
    // UB: the void target is called through an incompatible return type.
    /* FAKEMATCH: called through a cast to the s32 call type the bytes show, its
     * result discarded (as call.h's Value2 did, 2026-10-13); called directly or
     * through Call2 each of the two calls differs 2 on tbs-en, both 4.
     * Direct prototype rechecked 2026-10-16: 2 argument instructions
     * differ in tbs-en. */
    COLOSSO_WALL_START_MODE_TASK(3, 1);
    Engine_EventWait(-result * 60 + 60);
    reactingActor = 0;
  } else {
    Engine_EventWait(30);
    Engine_AudioPlayCue(result + 90);
    Colosso_LoadPortrait(4);
    // UB: the void target is called through an incompatible return type.
    /* FAKEMATCH: called through the s32 cast like the call above; called
     * directly or through Call2 it differs 2 on tbs-en.
     * Direct prototype rechecked 2026-10-16: 2 argument instructions
     * differ in tbs-en. */
    COLOSSO_WALL_START_MODE_TASK(3, 0);
    Engine_EventWait(result * 60 + 60);
    reactingActor = 8;
  }
  Engine_ActorShowEmote(reactingActor, 0x105, 0);
  while (AudioCommand_GetStateByte() != 0) {
    Engine_TaskWait(1);
  }
  Engine_AudioPlayCue(19);
  Engine_EventWait(30);
  Engine_AudioPlayCue(0x121);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
}

// Shared source is included here in ROM order.
#include "common/colosso_marker.h"
#include "common/colosso_item_icon.h"

// Write eight uppercase hexadecimal digits followed by a terminator.
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

// Retain the empty hook called before the finish sequence.
static void SceneState_EmptyHook(void) {}

// Mark the rival's path state as finished.
static UNUSED void ColossoWall_MarkRivalFinished(void) {
  ((struct StageControl *)gSceneState)->status = 9;
}

// Wait until the rival's path state reaches its finish marker.
static void ColossoWall_WaitForRivalFinish(void) {
  s16 *status = &((struct StageControl *)gSceneState)->status;

  while (*status != 9) {
    Engine_TaskWait(1);
  }
}

// Occasionally spawn a falling particle around the linked finish effect.
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

    position[0] = source->f08;
    position[1] = source->f0c;
    position[2] = source->f10;
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

// Raise the linked finish effect, start its script and clear the link.
static s32
ColossoWall_RaiseLinkedSceneEffect(struct ScriptInterpreter *interpreter) {
  RaisedEffect *source = (RaisedEffect *)interpreter;
  RaisedEffect *effect = (RaisedEffect *)Object_GetById(source->f64);

  Engine_ObjectSetPosition(effect, source->f08, source->f0c + 0x240000,
                           source->f10);
  effect->f55 = 0;
  Engine_ObjectSetScript(effect, gColossoWall_RaiseScript);
  Engine_AudioPlayCue(SOUND_TREASURE_FOUND);
  source->f64 = 0;
  return 0;
}

// Approach the finish item on the unused side and show the item message.
static s32 ColossoWall_ApproachFinishAndTakeItem(s32 finishItemActorA,
                                                 s32 finishItemActorB) {
  u8 *raceWork = *(u8 **)((u8 *)gWorkSlot + WORK_SLOT_STAGE * 4);
  u8 *gameState;
  struct FieldActor *competitor;
  s32 finishAlreadyReached;
  s32 x;
  s32 z;
  u16 *itemActor;
  s16 *timer;

  finishAlreadyReached = Engine_GameFlagIsSet(0x211);

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
  Engine_GameFlagSet(0x211);
  Engine_ObjectSetScript(competitor, gColossoWall_ApproachScript);

  while (*timer != 0) {
    Engine_TaskWait(1);
  }

  // Assign the item on the side this competitor reached first.
  if (finishAlreadyReached == 0) {
    // UB: the void target is called through an incompatible return type.
    /* FAKEMATCH: called through a cast to the s32 call type the bytes show;
     * called by its void prototype the argument moves are ordered differently:
     * differ 2 on tbs-en, 4 with the call below.
     * Direct prototype rechecked 2026-10-16: 2 argument instructions
     * differ in tbs-en. */
    COLOSSO_WALL_APPLY_MATCHING_SLOT_VALUE(0, finishItemActorA);
    UiWork_PushValueSlot(finishItemActorA, 2);
  } else {
    // UB: the void target is called through an incompatible return type.
    /* FAKEMATCH: the same cast as above; by its prototype differ 2 on tbs-en.
     * Direct prototype rechecked 2026-10-16: 2 argument instructions
     * differ in tbs-en. */
    COLOSSO_WALL_APPLY_MATCHING_SLOT_VALUE(0, finishItemActorB);
    UiWork_PushValueSlot(finishItemActorB, 2);
  }

  gameState = (u8 *)&gGameState;
  UiWork_PushValueSlot(((struct GameState *)gameState)->selectedActor, 1);
  Engine_MessageShowCentered(MsgColossoIsaacGotItem, 3);
  ObjectDispatch_WaitForValue16(competitor);

  return finishAlreadyReached;
}

// Shared source is included here in ROM order.
#include "common/colosso_path_rival.h"

// Allocate the race work, record both competitors and install the gauge task.
void FieldScene_BuildDescriptorAndInstallTask(s32 leaderId, s32 rivalId,
                                              s32 raceMode, s32 finishX,
                                              s32 finishZ, s32 finishItemActorA,
                                              s32 finishItemActorB) {
  u8 *raceWork;
  struct FieldActor *leader;
  struct FieldActor *rival;
  void *graphics;
  s32 vramSlot;

  raceWork = Runtime_AllocateBlock(59, 0x7170);
  graphics = Runtime_BumpAllocateAlternatePool(512);

  *(u16 *)(raceWork + 222) = leaderId;
  *(u16 *)(raceWork + 224) = rivalId;
  *(u16 *)(raceWork + 226) = finishItemActorA;
  *(u16 *)(raceWork + 228) = finishItemActorB;
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

// Decode the rival's recorded path and start its playback task.
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

// Shared source is included here in ROM order.
#include "common/colosso_scene_state.h"
#include "common/object_findslot.h"

// Push the adjacent obstacle only when the destination and space above are
// clear.
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

  Engine_AudioPlayCue(0xee);
  FieldObject_WaitForMove(obstacle);
  Engine_AudioPlayCue(0x120);

  obstacle->x = destination.x;
  obstacle->z = destination.z;
  obstacle->motion24 = zero;
  obstacle->motion2c = zero;

  Object_SetMode(pusher, 1);
}

// Shared source is included here in ROM order.
#include "common/colosso_occupant.h"

const u8 gColossoWall_RollLogScript[] = {2, 2, 3, 3};

// clang-format off
const u8 gColosso_PortraitPaletteOffsets[] = {
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
    { TOUCH_WALKING_INTO(FACING_SOUTH), 30, CONDITION_ALWAYS, EVENT_SCRIPT(ColossoWall_RunSelectedActorRisingEffect) },
    { TOUCH_WALKING_INTO(FACING_WEST), 30, CONDITION_ALWAYS, EVENT_SCRIPT(ColossoWall_CheckLeaderAhead) },
    { TOUCH_WALKING_INTO(FACING_WEST), 28, CONDITION_ALWAYS, EVENT_SCRIPT(ColossoWall_RollEastLogWest) },
    { TOUCH_WALKING_INTO(FACING_EAST), 27, CONDITION_ALWAYS, EVENT_SCRIPT(ColossoWall_RollEastLogEast) },
    { TOUCH_WALKING_INTO(FACING_WEST), 24, CONDITION_ALWAYS, EVENT_SCRIPT(ColossoWall_RollWestLogWest) },
    { TOUCH_WALKING_INTO(FACING_EAST), 25, CONDITION_ALWAYS, EVENT_SCRIPT(ColossoWall_RollWestLogEast) },
    { 0x202, 22, CONDITION_ALWAYS, EVENT_SCRIPT(ColossoWall_PushAlignedRidingLog) },
    { 0x202, 21, CONDITION_ALWAYS, EVENT_SCRIPT(ColossoWall_PushObstacleAhead) },
    { 0x202, 20, CONDITION_ALWAYS, EVENT_SCRIPT(ColossoWall_PushAndUpdateObstacleCollision) },
    { 0x202, 26, CONDITION_ALWAYS, EVENT_SCRIPT(ColossoWall_PushAndUpdateObstacleCollision) },
    { EVENT_TOUCH, 50, 0x210, EVENT_SCRIPT(ColossoWall_StartFinalsBattle) },
    { SEARCH(SEARCH_CHEST), 101, 0x338, FIND_ITEM(ITEM_NUT) },
    { 0x8413, 102, 0x339, FIND_ITEM(ITEM_SMOKE_BOMB) },
    { SEARCH(SEARCH_CHEST), 104, 0x33b, FIND_ITEM(ITEM_NUT) },
    { 0x50001815, 20, 0x331, EVENT_SCRIPT(ColossoWall_FreezeWestWaterObstacle) },
    { 0x50001815, 21, 0x332, EVENT_SCRIPT(ColossoWall_FreezeEastWaterObstacle) },
    { 0x10008e15, 18, 0x330, EVENT_SCRIPT(ColossoWall_EnableForceChildEffect) },
    { PSYNERGY_ON_ACTOR(ABILITY_FORCE), 18, 0x330, EVENT_SCRIPT(ColossoWall_KnockDownForceObstacle) },
    { PSYNERGY_ON_ACTOR(ABILITY_GROWTH), 19, 0x333, EVENT_SCRIPT(ColossoWall_UpdateGrowthObstacleCollision) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 9, CONDITION_ALWAYS, EVENT_SCRIPT(ColossoWall_IgnoreMove) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 10, CONDITION_ALWAYS, EVENT_SCRIPT(ColossoWall_IgnoreMove) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 11, CONDITION_ALWAYS, EVENT_SCRIPT(ColossoWall_UpdateMovedObstacleCollision) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 12, CONDITION_ALWAYS, EVENT_SCRIPT(ColossoWall_SaveMovedObstacleColumns) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 13, CONDITION_ALWAYS, EVENT_SCRIPT(ColossoWall_SaveMovedObstacleColumns) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 14, CONDITION_ALWAYS, EVENT_SCRIPT(ColossoWall_SaveMovedObstacleColumns) },
    { PSYNERGY_ON_ACTOR(ABILITY_HALT), 31, 0x334, EVENT_SCRIPT(ColossoWall_HaltMovingPlatforms) },
    { EVENT_TALK, 34, CONDITION_ALWAYS, EVENT_SCRIPT(RunPartyCountInteraction) },
    { EVENT_TALK, 35, CONDITION_ALWAYS, EVENT_SCRIPT(ColossoWall_ExplainMiniMaze) },
    { EVENT_TALK, 36, CONDITION_ALWAYS, EVENT_SCRIPT(ColossoWall_ExplainShiftingFloor) },
    { EVENT_TALK, 37, CONDITION_ALWAYS, EVENT_SCRIPT(ColossoWall_ExplainLogRolling) },
    { EVENT_TALK, 38, CONDITION_ALWAYS, EVENT_SCRIPT(ColossoWall_ExplainWallClimb) },
    { EVENT_TALK, 39, CONDITION_ALWAYS, EVENT_SCRIPT(ColossoWall_ExplainBoardWalk) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 34, CONDITION_ALWAYS, MSGNO(MsgColossoRiverThisYearsFinalsAreIncredibleThis) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 35, CONDITION_ALWAYS, MSGNO(MsgColossoWallICantBelieveAnybodyEverGets) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 36, CONDITION_ALWAYS, MSGNO(MsgColossoWallHowDoTheyMakeThatFloor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 37, CONDITION_ALWAYS, MSGNO(MsgColossoWallImAmazedPeopleCanActuallyCross) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 38, CONDITION_ALWAYS, MSGNO(MsgColossoWallBoyThatBitOverThereMust) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 39, CONDITION_ALWAYS, MSGNO(MsgColossoWallIHeardBabiGotTheLog) },
    { EVENT_RAISED, 99, CONDITION_ALWAYS, EVENT_SCRIPT(Colosso_RememberSupportPosition) },
    { SCENE_EVENTS_END },
};

s16 gColosso_PortraitSlot = -1;

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
