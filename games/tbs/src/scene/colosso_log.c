/// @file
/// Colosso's third finals course: Isaac races another warrior through the
/// Scales of Justice, the Stepping-Stone Stage, the Wall Wedge, the Free-Climb
/// Area, the Moving Sidewalk Stage and a log-rolling area to the arena.

#include "script.h"
#include "version.h"
#include "game_state.h"
#include "io_write_queue.h"
#include "resource_ids.h"
#include "scene_ids.h"
#include "vram_tab.h"
#include "unit/import.h"
#include "unit/scene/colosso_log.h"
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
#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "colosso_log_rolling_stage.h"
#include "io_reg.h"
#include "keystate.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "field_service.h"
#include "flags.h"
#include "text/msg_ids.h"
#include "battle_unit.h"
#include "resource.h"
#include "ability_ids.h"
#include "edition.h"
#include "item_ids.h"
#include "objcmd.h"
#include "field_obj.h"
#include "heap_state.h"
#include "field_runtime.h"
#include "fixed_math.h"
#include "palbuf.h"
#include "colosso_gauge.h"
#include "macros.h"
#include "songs.h"

// Keep native call forms while UBFIX uses the declared void contracts.
#ifdef UBFIX
#define COLOSSO_LOG_FACE_DIRECTION Engine_ActorFaceDirection
#define COLOSSO_LOG_INITIALIZE_MODE_TASK                                       \
  ColossoLogRollingStage_InitializeModeTask
#define COLOSSO_LOG_APPLY_MATCHING_SLOT_VALUES Scene_GiveAndEquipItem
#else
#define COLOSSO_LOG_FACE_DIRECTION ((s32 (*)())Engine_ActorFaceDirection)
#define COLOSSO_LOG_INITIALIZE_MODE_TASK                                       \
  ((s32 (*)())ColossoLogRollingStage_InitializeModeTask)
#define COLOSSO_LOG_APPLY_MATCHING_SLOT_VALUES                                 \
  ((s32 (*)())Scene_GiveAndEquipItem)
#endif

static void ColossoLog_PlayPostBattleReunion(s32 battleResult);
static void ColossoLog_IntroduceFinalCourse(s32 guideId);
static void ColossoLogRollingStage_RunScriptedTransition(s32 mode);
static void ColossoLogRollingStage_NoopSceneHook(void);
static void ColossoLog_WaitForRivalToFinish(void);
static s32 ColossoLog_CollectFinishEquipment(s32 firstItem, s32 secondItem);
static void ColossoLog_PushActorAhead(void);

typedef void (*SceneTaskEntry)(void);

/// The third course's actors, by the guide's and attendants' explanations: the
/// Scales of Justice, the Stepping-Stone Stage, the Wall Wedge, the Free-Climb
/// Area, the Moving Sidewalk Stage and another log-rolling area lead to the
/// arena. The Wall Wedge's moving wall is actors 18 to 27; actors 15 to 17 are
/// rocks that settle where they fall (FLAG_SCENE_TEMP_14 to _16).
enum {
  OPPONENT = 8,        ///< the warrior racing Isaac to the arena
  FROST_TARGET = 9,    ///< on the eastern pan
  SCALE_PAN_WEST = 10, ///< the Scales of Justice
  SCALE_PAN_EAST = 11,
  SCALE_WEIGHT = 12, ///< pushed onto the western pan
  STONE = 13,
  WALL_OPERATOR = 28,
  GROWTH_PLANT = 29,
  SIDEWALK_BLOCK = 30,
  LONG_LOG = 31, ///< the log-rolling area's long log
  GUIDE = 32,    ///< explains the third course
  SCALES_ATTENDANT = 33,
  STEPPING_STONE_ATTENDANT = 34,
  WALL_WEDGE_ATTENDANT = 35,
  FREE_CLIMB_ATTENDANT = 36,
  MOVING_SIDEWALK_ATTENDANT = 37,
  LOG_ROLLING_ATTENDANT = 38,
  PRIZE_A = 39, ///< the arena's Claymore and Hunter's Sword
  PRIZE_B = 40
};

/// Sets the leader speed for the shared solo-party placement routine.
static inline void InitializeActorZero(void) {
  Engine_ActorSetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
}

/// Sets a companion speed for the shared solo-party placement routine.
static inline void InitializeSelectedActor(s32 actorId) {
  Engine_ActorSetSpeed(actorId, PIXELS(1), PIXELS(1) / 2);
}

extern s32 gColossoParticleCount;

const struct SceneEntrance *ColossoLog_GetEntrances(void) {
  return gColossoLogEntrances;
}

const struct SceneRegion *ColossoLog_GetRegions(void) { return 0; }

const u32 *ColossoLog_GetExits(void) { return gColossoLogExits; }

const struct ScenePlacement *ColossoLog_GetPlacements(void) {
  return gColossoLogPlacements;
}

/// Cycles the moving wall and raises the opening event beneath the rider.
static void ColossoLogRollingStage_SceneTask(void) {
  struct EventWork *work = gEventWork;
  struct FieldActor *actor = Object_GetById(gGameState.selectedActor);
  s32 row = actor->z.fixed >> 20;
  s32 i;
  s32 kind;

  if (gColossoSceneTaskState == 0) {
    gColossoSceneTaskStatus = (gColossoSceneTaskStatus + 1) & 3;
    for (i = 18; i <= 22; i++) {
      kind = gColossoLog_LogPattern[gColossoSceneTaskStatus][i - 18];
      Engine_ActorSetAnimation(i, kind);
      Engine_ActorSetAnimation(i + 5, kind + 8);
      Engine_MapCopyCellAttributes(32, 11, 1, 2, (i - 18) * 2 + 33, 11);
      if (kind != 7) {
        Engine_MapCopyCellAttributes(74, 12, 1, 1, (i - 18) * 2 + 33, 11);
      }
    }
    Engine_ActorSetAnimation(
        28, gColossoLog_LogPattern[gColossoSceneTaskStatus][5]);
  } else {
    for (i = 18; i <= 22; i++) {
      kind = gColossoLog_LogPattern[gColossoSceneTaskStatus][i - 18];
      if ((u32)(actor->x.fixed - (i << 21) + 0x31ffff) <= 0x13fffe) {
        if (row == 11 && kind == 4)
          work->raisedTrigger = kind;
        if (row == 12 && kind == 5)
          work->raisedTrigger = kind;
      }
    }
  }
  if ((u32)++gColossoSceneTaskState > 17) {
    gColossoSceneTaskState = 0;
  }
}

// Function from another file; its callers use no declaration (implicit int).

/// Resets the moving wall to its first displayed pattern.
static void ColossoLogRollingStage_ResetAndRunSceneTask(void) {
  SceneTaskEntry entry;

  gColossoSceneTaskState = 0;
  entry = ColossoLogRollingStage_SceneTask;
  gColossoSceneTaskStatus = 0;
  Engine_TaskRemoveCallback(entry);
  entry();
}

/// Resumes cycling the moving wall.
static void ColossoLogRollingStage_StartSceneTask(void) {
  FieldScheduler_AddCallback(ColossoLogRollingStage_SceneTask, 0xC80);
}

/// Waits for the wall pattern used by the tutorial, with a bounded timeout.
static void ColossoLogRollingStage_WaitForSceneTask(void) {
  s32 polls;

  Engine_TaskWait(10);

  polls = 0;
  while (gColossoSceneTaskStatus != 3 || gColossoSceneTaskState != 1) {
    Engine_TaskWait(1);
    polls++;
    if (polls > 119) {
      return;
    }
  }
}

/// On the Moving Sidewalk, until 0x363 is set, the belt carries the controlled
/// actor and the camera west.
static void Event_RideMovingSidewalk(void) {
  struct EventWork *work = gEventWork;
  struct GameState *state = &gGameState;
  s32 id = state->selectedActor;
  struct FieldActor *subject = work->viewCenter;
  struct FieldActor *actor = Object_GetById(id);
  s32 z = actor->z.part.pixel;

  if ((u32)(z - 183) <= 3) {
    subject->x.fixed += -(PIXELS(4) / 5);
    actor->x.fixed += -(PIXELS(4) / 5);
  }
}

/// Growth on the plant: it grows and opens a shortcut (0x360).
static void Event_GrowPlant(void) {
  Engine_GameFlagSet(0x360);
  {

    Map_CopyCellAttributes(47, 61, 1, 4, 49, 61);
  }
}

/// Moves a scale pan vertically at the shared speed and acceleration.
static inline void ColossoLog_MoveOnScales(struct FieldActor *pan, s32 height) {
  pan->acceleration = PIXELS(2) / 5;
  pan->speed = PIXELS(4) / 5;
  Engine_ObjectSetPosition(pan, pan->x.fixed, height, pan->z.fixed);
}

/// Frost on the target on the eastern pan: it freezes and the Scales of Justice
/// tip, the western pan down and the eastern pan up (0x362).
static void Event_FreezeOnScales(void) {
  struct FieldActor *object;

  object = Object_GetById(FROST_TARGET);
  object->scaleX = Q16_ONE;
  object->scaleY = Q16_ONE;

  object = Object_GetById(SCALE_PAN_EAST);
  ColossoLog_MoveOnScales(object, PIXELS(32));

  object = Object_GetById(SCALE_PAN_WEST);
  ColossoLog_MoveOnScales(object, PIXELS(4));

  Engine_GameFlagSet(0x362);
  Map_CopyCellAttributes(15, 12, 1, 1, 13, 12);
  Map_CopyCellAttributes(14, 12, 1, 1, 9, 12);
}

/// Stepping onto the western pan, unless the scales are frozen: it sinks with
/// the rider as the eastern pan rises (0x367).
static void Event_StepOnScales(void) {
  s32 actor;
  s32 lowered;
  struct FieldActor *log;
  struct FieldActor *rider;
  u8 *flags;

  actor = gGameState.selectedActor;
  lowered = GameFlag_IsSet(0x362);
  if (lowered == 0) {
    log = Object_GetById(SCALE_PAN_WEST);
    if (log != NULL) {
      Actor_SetDestination(actor, log->x.part.pixel, log->z.part.pixel);
    }
    Engine_ActorWaitForMove(actor);
    log = Object_GetById(SCALE_PAN_EAST);
    log->motionFlags = lowered;
    ColossoLog_MoveOnScales(log, PIXELS(32));
    log = Object_GetById(SCALE_PAN_WEST);
    log->motionFlags = lowered;
    ColossoLog_MoveOnScales(log, PIXELS(4));
    rider = Object_GetById(actor);
    flags = &rider->motionFlags;
    *flags = lowered;
    ColossoLog_MoveOnScales(rider, PIXELS(4));
    Engine_ActorSetSpriteFlags(rider, 1);
    Engine_ActorWaitForMove(actor);
    Map_CopyCellAttributes(0, 24, 1, 1, 9, 12);
    Engine_TaskWait(2);
    Engine_ActorSetSpriteFlags(rider, 1);
    *flags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    rider->terrainHeight = rider->y.fixed;
    Engine_GameFlagSet(0x367);
  }
}

/// Stepping off the western pan: the scales swing back.
static void Event_StepOffScales(void) {
  struct FieldActor *object;

  Engine_ActorSetAnimation(gGameState.selectedActor, 1);

  object = Object_GetById(SCALE_PAN_EAST);
  object->motionFlags = 0;
  ColossoLog_MoveOnScales(object, PIXELS(4));

  object = Object_GetById(SCALE_PAN_WEST);
  object->motionFlags = 0;
  ColossoLog_MoveOnScales(object, PIXELS(32));

  Engine_ActorWaitForMove(SCALE_PAN_WEST);
  {
    Map_CopyCellAttributes(0, 25, 1, 1, 9, 12);
  }
  Engine_TaskWait(2);
  Engine_GameFlagClear(0x367);
}

/// When the weight reaches the western pan, it sinks the pan and the eastern
/// pan rises (0x368).
static void Event_SetWeightOnScales(void) {
  struct FieldActor *probe;
  struct FieldActor *log;
  struct FieldActor *upper;
  struct FieldActor *lower;
  s32 x;
  s32 z;

  probe = Object_GetById(SCALE_WEIGHT);
  x = probe->x.fixed >> 20;
  if (x == 9) {
    z = probe->z.fixed >> 20;
    if (z == 12) {
      log = Object_GetById(SCALE_WEIGHT);
      Engine_ActorSetSpriteFlags(log, 0);
      log->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
      log->motionFlags = 0;
      ColossoLog_MoveOnScales(log, PIXELS(4));
      upper = Object_GetById(SCALE_PAN_EAST);
      upper->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
      ColossoLog_MoveOnScales(upper, PIXELS(32));
      lower = Object_GetById(SCALE_PAN_WEST);
      ColossoLog_MoveOnScales(lower, PIXELS(4));
      Engine_GameFlagSet(0x368);
      Map_CopyCellAttributes(15, 12, 1, 1, 13, z);
      Map_CopyCellAttributes(1, 25, 1, 1, x, z);
    }
  }
}

/// Walking into the weight pushes it, perhaps onto the scales.
static void Event_PushWeight(void) {
  ColossoLog_PushActorAhead();
  Event_SetWeightOnScales();
}

/// After Move on the stone: its column is kept (byte flag 0x370) and its cell
/// rebuilt.
static void Event_MoveStone(void) {
  struct FieldActor *actor;
  s32 x;

  actor = Object_GetById(STONE);
  x = actor->x.fixed >> 20;
  FieldGameFlag_SetByte(880, x);
  Map_CopyCellAttributes(18, 10, 3, 1, 18, 11);
  Map_CopyCellAttributes(17, 11, 1, 1, x, 11);
}

/// Move on this rock needs nothing more.
static void Event_IgnoreMove(void) {}

/// Walking into a rock pushes it.
static void Event_PushRock(void) { ColossoLog_PushActorAhead(); }

/// A rock resting on the floor becomes a step (FLAG_SCENE_TEMP_14 to _16).
static void Event_SettleRocks(void) {
  struct FieldActor *actor;
  s32 slot;
  s32 x;
  s32 z;
  s32 x2;
  s32 z2;

  for (slot = 15; slot <= 17; slot++) {
    actor = Object_GetById(slot);
    if (Map_GetTerrainHeight(0, actor->x.fixed, actor->z.fixed) == 0) {
      actor->priorityFlags = 2;
      actor->motionFlags = 0;
      x = actor->x.fixed >> 20;
      z = actor->z.fixed >> 20;
      Map_CopyCellAttributes(83, 13, 1, 1, x, z);
      x2 = actor->x.fixed >> 20;
      z2 = actor->z.fixed >> 20;
      Map_CopyCellAttributes(83, 13, 1, 1, x2, z2 + 52);
      Engine_GameFlagSet(slot - 15 + FLAG_SCENE_TEMP_14);
    }
  }
}

/// Changes the eastern end collision according to the rider facing.
static void ColossoLog_UpdateEastEndCollision(void) {
  struct FieldActor *actor;
  s32 x;
  s32 z;
  s32 collisionKind;

  actor = Object_GetById(gGameState.selectedActor);
  x = actor->x.fixed >> 20;
  collisionKind = 23;
  z = actor->z.fixed >> 20;
  if (x == 81 && z == 12) {
    if ((actor->facing & 0xE000) == 0x4000) {
      collisionKind = 253;
    }
    SetMapCellCollision(0, x << 20, z << 20, collisionKind);
  }
}

/// Rejects blocked cells and cells occupied by one of the three obstacles.
static s32 ColossoLogRollingStage_CheckObstacleDestination(s32 x, s32 z) {
  struct FieldActor *actor;

  if (GetMapCellCollision(0, x, z) == 255) {
    return -2;
  }
  actor = Object_GetById(15);
  x >>= 20;
  z >>= 20;
  if (actor->x.fixed >> 20 == x && actor->z.fixed >> 20 == z) {
    return -1;
  }
  actor = Object_GetById(16);
  if (actor->x.fixed >> 20 == x && actor->z.fixed >> 20 == z) {
    return -1;
  }
  actor = Object_GetById(17);
  if (actor->x.fixed >> 20 == x && actor->z.fixed >> 20 == z) {
    return -1;
  }
  return 0;
}

/// Checks all four cells covered by the long rolling log.
static s32 ColossoLogRollingStage_CheckPathClearance(s32 x, s32 z) {
  if (ColossoLogRollingStage_CheckObstacleDestination(x, z - PIXELS(24)) != 0 ||
      ColossoLogRollingStage_CheckObstacleDestination(x, z - PIXELS(8)) != 0 ||
      ColossoLogRollingStage_CheckObstacleDestination(x, z + PIXELS(8)) != 0 ||
      ColossoLogRollingStage_CheckObstacleDestination(x, z + PIXELS(24)) != 0) {
    return -1;
  }
  return 0;
}

/// Walking into the long log rolls it, with the leader, the way the pad is held
/// until something stops it; at the east end it drops into place and the path
/// is made (0x369). Away from the log, a rock ahead is pushed instead.
static void Event_RollLongLog(void) {
  struct FieldActor *rider;
  struct FieldActor *log;
  struct EventWork *work;
  s32 angle;
  s32 actor;
  s32 x;
  s32 z;
  s32 steps;
  s32 nextX;
  s32 column;
  s32 acceleration;
  s32 mode;
  s32 step;
  s32 ahead[3];

  rider = Object_GetById(gGameState.selectedActor);
  log = Object_GetById(LONG_LOG);
  steps = 0;
  angle = gColossoEarlySequenceData[rider->facing >> 13];
  actor = gGameState.selectedActor;
  SET_VECTOR(ahead, (rider->x.fixed & -0x100000) + PIXELS(8), rider->y.fixed,
             (rider->z.fixed & -0x100000) + PIXELS(8));
  Vector_AddPolarOffset(PIXELS(16), angle, ahead);
  x = log->x.fixed;
  z = log->z.fixed;
  // Only a nearby rider rolls the log; other interactions push the actor ahead.
  if ((ahead[0] - x >= 0 ? ahead[0] - x : x - ahead[0]) > PIXELS(8) ||
      (ahead[2] - z >= 0 ? ahead[2] - z : z - ahead[2]) > PIXELS(32)) {
    goto far;
  }
  if (gKeysHeld & KEY_LEFT) {
    mode = 2;
    step = -8;
    for (;;) {
      nextX = x - PIXELS(16);
      if (ColossoLogRollingStage_CheckPathClearance(nextX, z) == 0) {
        steps++;
        x = nextX;
      } else {
        break;
      }
    }
  } else {
    if (!(gKeysHeld & KEY_RIGHT)) {
      return;
    }
    mode = 3;
    step = 8;
    for (;;) {
      nextX = x + PIXELS(16);
      if (ColossoLogRollingStage_CheckPathClearance(nextX, z) == 0) {
        steps++;
        x = nextX;
      } else {
        break;
      }
    }
  }
  if (steps == 0) {
    return;
  }
  // Clear the old footprint, then move the log and rider together.
  Map_CopyCellAttributes(74, 8, 1, 4, log->x.fixed >> 20, 9);
  Map_CopyCellAttributes(120, 60, 8, 5, 74, 60);
  Engine_EventBegin();
  Engine_ActorSetAnimation(actor, 8);
  Engine_EventWait(6);
  log->speed = PIXELS(1) / 2;
  acceleration = PIXELS(1) / 5;
  log->acceleration = acceleration;
  Object_SetMode(log, mode);
  Engine_ObjectSetPosition(log, x, 0, z);
  Engine_EventWait(6);
  Engine_ActorSetAnimation(actor, 2);
  work = Runtime_AllocateBlock(HEAP_SLOT_EVENT, sizeof(struct FieldWork));
  ObjectDispatch_StartCamera((struct DispatchObject *)work->viewCenter,
                             (s32)log);
  Actor_SetSpeed(actor, PIXELS(1) / 2, acceleration);
  Event_PlayCue(SE_239);
  Object_SetMode(rider, 2);
  Engine_ObjectSetPosition(rider, (steps * step * PIXELS(1)) + rider->x.fixed,
                           0, rider->z.fixed);
  FieldObject_WaitForMove(rider);
  Object_SetMode(rider, 1);
  FieldObject_WaitForMove(log);
  // At the eastern end the log settles into the course exit.
  if (x >= PIXELS(1328)) {
    GameFlag_Set(0x369);
    Engine_ActorSetAnimation(LONG_LOG, 3);
    Actor_SetDestinationOffset(LONG_LOG, 18, 6);
    Engine_EventWait(30);
    Object_SetMode(log, 8);
    FieldObject_WaitForMove(log);
    log->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    column = 84;
    Map_CopyCellAttributes(86, 10, 1, 2, column, 10);
    Map_CopyCellAttributes(86, 9, 1, 1, column, 12);
    Event_PlayCue(SE_288);
    Event_PlayCue(SE_240);
  } else {
    Object_SetMode(log, 1);
    Event_PlayCue(SE_288);
    Event_PlayCue(SE_213);
    column = x >> 20;
    Map_CopyCellAttributes(85, 9, 1, 4, column, 9);
    Map_CopyCellAttributes(85, 9, 1, 4, column, 61);
  }
  Engine_EventWait(15);
  Engine_EventEnd();
  return;
far:
  ColossoLog_PushActorAhead();
  Event_SettleRocks();
}

/// Displays the activated moving sidewalk and plays its arrival sounds.
static s32
ColossoLog_ShowActivatedSidewalkLayer(struct ScriptInterpreter *interpreter) {
  Map_SetLayerEntryFlag(1);
  Map_ClearLayerEntryFlag(2);
  Engine_AudioPlayCue(SE_288);
  Engine_AudioPlayCue(SE_217);
  return 0;
}

/// Force on the Moving Sidewalk's block: it slides away and the belt carries
/// no one any more (0x363).
static void Event_ForceSidewalkBlock(void) {
  struct FieldActor *effect;
  s32 moveRate;

  effect = Object_GetById(SIDEWALK_BLOCK);
  effect->motionFlags = 0;
  moveRate = PIXELS(8) / 5;
  effect->acceleration = moveRate;
  effect->speed = moveRate;
  Object_SetMode(effect, 2);
  Engine_ObjectSetScript(effect, gColossoSceneEventEffect);
  Engine_GameFlagSet(0x363);
}

/// Halt on the wall operator: the Wall Wedge stops at its next open pattern
/// (0x361).
static void Event_HaltWall(void) {
  BattleEffect_PauseObject(WALL_OPERATOR);
  Engine_GameFlagSet(0x361);
  Engine_TaskWait(10);
  while (gColossoSceneTaskStatus != 1 && gColossoSceneTaskStatus != 3)
    Engine_TaskWait(1);
  Engine_TaskWait(1);
  Engine_TaskRemoveCallback(ColossoLogRollingStage_SceneTask);
}

/// The rider passes through the moving wall's northern opening.
static void Event_PassNorthWallOpening(void) {
  struct FieldActor *actor;
#ifndef UBFIX
  s16 *gameWords;
#endif
  s32 *actorId;
  s32 z;

#ifdef UBFIX
  actorId = &gGameState.selectedActor;
#else
  // The original word-indexed address takes three more instructions here.
  gameWords = (s16 *)&gGameState;
  actorId = (s32 *)&gameWords[offsetof(struct GameState, selectedActor) /
                              sizeof(s16)];
#endif
  actor = Object_GetById(*actorId);
  actor->acceleration = PIXELS(1);
  actor->speed = PIXELS(2);
  Engine_ActorSetAttachedEffect(*actorId, 258);
  Object_SetMode(actor, 5);
  z = actor->z.fixed & 0xFFF00000;
  Engine_ObjectSetPosition(actor, actor->x.fixed, actor->y.fixed,
                           z + PIXELS(24));
  FieldObject_WaitForMove(actor);
}

/// The rider passes through the moving wall's southern opening.
static void Event_PassSouthWallOpening(void) {
  struct FieldActor *actor;
#ifndef UBFIX
  s16 *gameWords;
#endif
  s32 *actorId;
  s32 z;

#ifdef UBFIX
  actorId = &gGameState.selectedActor;
#else
  // The original word-indexed address takes three more instructions here.
  gameWords = (s16 *)&gGameState;
  actorId = (s32 *)&gameWords[offsetof(struct GameState, selectedActor) /
                              sizeof(s16)];
#endif
  actor = Object_GetById(*actorId);
  if (actor->x.fixed > PIXELS(664)) {
    actor->x.fixed = PIXELS(664);
  }
  actor->acceleration = PIXELS(1);
  actor->speed = PIXELS(2);
  Object_SetMode(actor, 5);
  z = actor->z.fixed & 0xFFF00000;
  Engine_ObjectSetPosition(actor, actor->x.fixed, actor->y.fixed,
                           z + PIXELS(12));
  FieldObject_WaitForMove(actor);
  Engine_ActorSetAttachedEffect(*actorId, 258);
  FieldBattleFx_RunRisingObjectSequence(*actorId, 6, 0);
}

UNUSED static void ColossoLogRollingStage_NoopSceneEventHook(void) {}

/// Offers to practise the course with a selected solo competitor.
UNUSED static void ColossoLog_RunCompetitorTalk(s32 competitorId) {
  struct EventWork *work;
  s32 actorId;
  s32 messageOffset;
  s32 entrance;

  work = gEventWork;
  actorId = gGameState.selectedActor;
  entrance = gGameState.entrance;
  if (entrance == 2) {
    Engine_EventBegin();
    messageOffset = competitorId * 3;
    Engine_EventSetMessage(messageOffset + MsgColossoDidntThinkBattles);
    Engine_EventOpenMessage(competitorId, 0);
    if (Engine_EventChooseYesNo(actorId, 0) == 0) {
      Engine_EventSetMessage(messageOffset + MsgColossoDidntThinkBattles + 1);
      Engine_EventShowMessage(competitorId, 0);
      work->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
      work->transitionFrames = 15;
      Engine_EventCloseScreen();
      Engine_EventWaitForScreen();
      Colosso_ControlCharacterAlone(competitorId);
      Engine_EventOpenScreen();
      Engine_EventWaitForScreen();
    } else {
      Engine_EventSetMessage(messageOffset + MsgColossoDidntThinkBattles + 2);
      Engine_EventShowMessage(competitorId, 0);
    }
    Engine_EventEnd();
  }
}

/// Reaching the arena: the leader takes a prize (the Claymore if first, the
/// Hunter's Sword if second), and he and his opponent face off and fight.
/// FLAG_SCENE_TEMP_11 says the arena was already reached; the arena's touch
/// works while FLAG_SCENE_TEMP_10 is clear.
static void Event_ReachArena(void) {
  s32 place;
  s32 i;
  s32 last;

  /* FAKEMATCH: the last place is set at the top as a forced temporary, so the
   * weight stays 4 - place + 1 instead of folding to 5 - place. */
  last = 4;
  ColossoLogRollingStage_NoopSceneHook();
  Engine_EventBegin();
  place = ColossoLog_CollectFinishEquipment(3, 17);
  ColossoLog_WaitForRivalToFinish();
  for (i = 0; i < 10; i++) {
    Object_RefreshSelectorById(OPPONENT);
  }
  Actor_SetSpeed(OPPONENT, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkTo(OPPONENT, 1528, 192);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 1496, 192);
  Engine_ActorSetAnimation(OPPONENT, 1);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, OPPONENT, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimation(OPPONENT, 3);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
  Engine_EventWait(20);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(2), PIXELS(1));
  Actor_SetSpeed(OPPONENT, PIXELS(2), PIXELS(1));
  Actor_WalkTo(ACTOR_PARTY_LEADER, 1504, 192);
  Actor_WalkToAndWait(OPPONENT, 1520, 192);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 16);
  Engine_ActorSetAnimation(OPPONENT, 9);
  Engine_EventWait(10);
  Field_RequestBattleFromEncounterTable(72, last - place + 1);
  Battle_SetStart(3);
  Event_SetNextDestination((s32)&SceneId_ColossoLog, 4);
  Event_SetDefeatDestination((s32)&SceneId_ColossoLog, 5);
  Engine_GameFlagSet(FLAG_KEEP_MUSIC);
}

/// Walking into something: unless it is an actor, it is checked.
static void Event_CheckAhead(void) {
  if (Colosso_FindActorAhead() == 0) {
    Engine_LeaderCheckAhead();
  }
}

/// Walking into something: the weight is pushed, anything else is checked.
static void Event_PushWeightOrCheckAhead(void) {
  if (Colosso_FindActorAhead() == 0) {
    Engine_LeaderCheckAhead();
  } else {
    Event_PushWeight();
  }
}

const struct SceneEvent *ColossoLog_GetEvents(void) {
  return gColossoLogEvents;
}

/// Restores obstacle progress, then enters practice, introduction or battle
/// aftermath.
s32 ColossoLog_Initialize(void) {
  struct FieldActor *actor;
  s32 i;
  s32 height;
  s32 column;

  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Engine_GameFlagSet(FLAG_RETREAT_BLOCKED);
  // Restore the scale pans, movable weight and supporting collision cells.
  FieldScheduler_AddCallback(ColossoLog_UpdateEastEndCollision, 3200);
  Map_CopyCellAttributes(74, 60, 8, 6, 120, 60);

  Engine_ActorSetSpriteFlags(Object_GetById(FROST_TARGET), 0);

  actor = Object_GetById(SCALE_PAN_WEST);
  Engine_ActorSetSpriteFlags(actor, 0);
  actor->motionFlags = 0;
  actor->y.fixed = PIXELS(32);

  actor = Object_GetById(SCALE_PAN_EAST);
  Engine_ActorSetSpriteFlags(actor, 0);
  actor->motionFlags = 0;
  actor->y.fixed = PIXELS(4);

  if (Engine_GameFlagIsSet(866) != 0) {
    Engine_ActorSetAnimation(FROST_TARGET, 5);
    actor = Object_GetById(SCALE_PAN_WEST);
    actor->y.fixed = PIXELS(4);
    actor = Object_GetById(SCALE_PAN_EAST);
    actor->y.fixed = PIXELS(32);
    Map_CopyCellAttributes(15, 12, 1, 1, 13, 12);
  } else {
    actor = Object_GetById(FROST_TARGET);
    actor->scaleX = 3 * Q16_ONE / 2;
    actor->scaleY = 3 * Q16_ONE / 2;
    if (Engine_GameFlagIsSet(871) != 0) {
      Map_CopyCellAttributes(0, 24, 1, 1, 9, 12);
    } else {
      Map_CopyCellAttributes(0, 25, 1, 1, 9, 12);
    }
  }

  if (Engine_GameFlagIsSet(872) != 0) {
    Map_CopyCellAttributes(15, 12, 1, 1, 13, 12);
    Map_CopyCellAttributes(1, 25, 1, 1, 9, 12);
    actor = Object_GetById(SCALE_WEIGHT);
    Engine_ActorSetSpriteFlags(actor, 0);
    actor->motionFlags = 0;
    actor->y.fixed = PIXELS(2);
    actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    actor = Object_GetById(SCALE_PAN_WEST);
    actor->y.fixed = PIXELS(4);
    actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    actor = Object_GetById(SCALE_PAN_EAST);
    actor->y.fixed = PIXELS(32);
  }

  column = FieldGameFlag_GetByte(880);
  if (column == 0) {
    column = 19;
  }
  actor = Object_GetById(STONE);
  actor->x.fixed = (column << 20) + (PIXELS(8));
  actor->motionFlags = 0;
  actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  Map_CopyCellAttributes(18, 10, 3, 1, 18, 11);
  Map_CopyCellAttributes(17, 11, 1, 1, column, 11);

  for (i = 15; i <= 17; i++) {
    actor = Object_GetById(i);
    height = Map_GetTerrainHeight(0, actor->x.fixed, actor->z.fixed);
    if (actor->y.fixed == 0 && height == 0) {
      actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
      actor->motionFlags = height;
      Map_CopyCellAttributes(83, 13, 1, 1, actor->x.fixed >> 20,
                             actor->z.fixed >> 20);
      Map_CopyCellAttributes(83, 13, 1, 1, actor->x.fixed >> 20,
                             (actor->z.fixed >> 20) + 52);
    }
  }

  // Halt preserves a stationary wall; otherwise restart its pattern cycle.
  if (Engine_GameFlagIsSet(865) != 0) {
    for (i = 18; i <= 22; i++) {
      actor = Object_GetById(i);
      actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
      Object_SetMode(actor, 2);
      actor = Object_GetById(i + 5);
      actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
      actor->motionFlags = 0;
      actor->y.fixed = PIXELS(32);
      Object_SetMode(actor, 10);
      Map_CopyCellAttributes(74, 12, 1, 1, i * 2 - 3, 11);
    }
    Engine_ActorSetAnimation(WALL_OPERATOR, 10);
    BattleEffect_PauseObject(WALL_OPERATOR);
  } else {
    for (i = 18; i <= 22; i++) {
      actor = Object_GetById(i);
      actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
      actor = Object_GetById(i + 5);
      actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
      actor->motionFlags = 0;
      actor->y.fixed = PIXELS(32);
    }
    FieldScheduler_AddCallback(ColossoLogRollingStage_SceneTask, 3200);
  }

  if (Engine_GameFlagIsSet(864) != 0) {
    Engine_ActorSetAnimation(GROWTH_PLANT, 4);
    Map_CopyCellAttributes(47, 61, 1, 4, 49, 61);
  }

  if (Engine_GameFlagIsSet(867) != 0) {
    Map_SetLayerEntryFlag(1);
    actor = Object_GetById(SIDEWALK_BLOCK);
    actor->motionFlags = 0;
    actor->x.fixed = PIXELS(1130);
    actor->z.fixed = PIXELS(184);
    Engine_ActorSetSpriteFlags(actor, 0);
    Object_SetMode(actor, 3);
    Engine_ObjectSetScript(actor, gColossoLog_Record30Script);
  } else {
    Map_SetLayerEntryFlag(2);
  }

  if (Engine_GameFlagIsSet(873) != 0) {
    actor = Object_GetById(LONG_LOG);
    Object_SetMode(actor, 8);
    actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    Map_CopyCellAttributes(86, 10, 1, 2, 84, 10);
    Map_CopyCellAttributes(86, 9, 1, 1, 84, 12);
  } else {
    actor = Object_GetById(LONG_LOG);
    Map_CopyCellAttributes(85, 9, 1, 4, actor->x.fixed >> 20, 9);
    Map_CopyCellAttributes(85, 9, 1, 4, actor->x.fixed >> 20, 61);
  }

  actor = Object_GetById(FROST_TARGET);
  actor->motionFlags = 0;
  actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  actor = Object_GetById(SCALE_PAN_WEST);
  actor->motionFlags = 0;
  actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  actor = Object_GetById(SCALE_PAN_EAST);
  actor->motionFlags = 0;
  actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  Engine_ActorSetAnimation(OPPONENT, 9);
  gGameState.movementMode = 0;
  Colosso_ShowItemIcon(PRIZE_A, 3);
  Colosso_ShowItemIcon(PRIZE_B, 17);
  Engine_ActorSetChildValue(OPPONENT, 2);

  // Enter the race, solo practice, course tour or post-battle reunion.
  switch (gGameState.entrance) {
  case 1:
    Colosso_StartRaceGauge(0, 8, 6, PIXELS(1512), PIXELS(192), 39, 40);
    Map_CopyCellsLayered(127, 0, 1, 2, 5, 2);
    Engine_ActorDestroy(GUIDE);
    Engine_ActorDestroy(SCALES_ATTENDANT);
    Engine_ActorDestroy(STEPPING_STONE_ATTENDANT);
    Engine_ActorDestroy(WALL_WEDGE_ATTENDANT);
    Engine_ActorDestroy(FREE_CLIMB_ATTENDANT);
    Engine_ActorDestroy(MOVING_SIDEWALK_ATTENDANT);
    Engine_ActorDestroy(LOG_ROLLING_ATTENDANT);
    if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
      Engine_AudioPlayCue(MUS_017);
      Colosso_ControlCharacterAlone(0);
      Colosso_PlaceFriendsAtCheeringSpots();
      Object_LinkObjectAndSetCallback(1, 0);
      ColossoLogRollingStage_RunScriptedTransition(3);
    }
    Object_LinkObjectAndSetCallback(1, 0);
    Object_LinkObjectAndSetCallback(2, 0);
    Object_LinkObjectAndSetCallback(3, 0);
    Colosso_StartRival((s32)&ResourceId_RivalPathC);
    break;

  case 2:
    FieldScheduler_AddCallback(Colosso_RaiseTakePlaceOnPsynergy, 3200);
    Engine_ActorDestroy(PRIZE_A);
    Engine_ActorDestroy(PRIZE_B);
    if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) != 0) {
      break;
    }
    Colosso_PlaceFriendsAtCheeringSpots();
    Colosso_ControlCharacterAlone(1);
    ColossoLogRollingStage_RunScriptedTransition(0);
    break;

  case 3:
    if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) != 0) {
      break;
    }
    ColossoLog_IntroduceFinalCourse(GUIDE);
    Colosso_ClearCheeringSpots();
    break;

  case 4:
    ColossoLog_PlayPostBattleReunion(1);
    Engine_EventRequestExit(4);
    Engine_GameFlagSet(FLAG_COLOSSO_OVER);
    Engine_GameFlagSet(FLAG_WON_COLOSSO);
    break;

  case 5:
    ColossoLog_PlayPostBattleReunion(-1);
    Engine_EventRequestExit(5);
    Engine_GameFlagSet(FLAG_COLOSSO_OVER);
    break;
  }
  return 0;
}

/// Advances a reunion particle; timer and work hold signed 1/256-pixel steps.
static s32 ColossoLogRollingStage_AdvanceParticleMotion(
    struct ScriptInterpreter *interpreter) {
  struct FieldActor *particle = (struct FieldActor *)interpreter;
  particle->x.fixed += particle->timer * 256;
  particle->y.fixed += (s16)particle->work * 256;
  particle->scaleX += Q16_ONE / 40;
  particle->scaleY += Q16_ONE / 40;
  particle->timer += 5;
  particle->work = (s16)particle->work - 1;
  return 0;
}

/// Shows one of three reunion particles at its scheduled point in the cycle.
static void ColossoLogRollingStage_SpawnPeriodicParticle(void) {
  struct FieldActor *particle;
  struct FieldActor *source;
  s32 x;
  s32 y;
  s32 kind;
  s32 count;

  particle = Object_GetById(0);
  count = gColossoParticleCount + 1;
  kind = 41;
  x = particle->x.fixed;
  y = particle->y.fixed;
  gColossoParticleCount = count;
  switch (count % 180) {
  case 10:
    break;
  case 20:
    kind = 42;
    break;
  case 30:
    kind = 43;
    break;
  default:
    return;
  }
  particle = Object_GetById(kind);
  if (particle == 0) {
    return;
  }
  source = Object_GetById(0);
  if (source != 0) {
    Engine_ActorSetPosition(kind, source->x.fixed, source->z.fixed);
  }
  Engine_ActorSetSpriteFlags(Object_GetById(kind), 0);
  particle->motionFlags = 0;
  particle->scaleX = 2 * Q16_ONE / 5;
  particle->scaleY = 2 * Q16_ONE / 5;
  {
    s32 height = PIXELS(4);
    particle->x.fixed = x + height;
    height += y;
    particle->y.fixed = height;
    particle->targetY = height;
  }
  particle->timer = 25;
  particle->work = 128;
  Actor_SetActionScript(kind, gColossoParticleKinds);
}

/// Reunites the party after the battle and plays the resting conversation.
static void ColossoLog_PlayPostBattleReunion(s32 battleResult) {
  struct FieldActor *actor;
  const s32 *leaderScaleScript;
  SchedulerCallback particleCallback;

  // Place the rival and companions around the resting leader.
  Engine_ActorDestroy(PRIZE_A);
  Engine_ActorDestroy(PRIZE_B);
  Party_RestoreHpPp(1);
  Engine_AudioPlayCue(MUS_017);
  Engine_EventBegin();
  Actor_SetPosition(OPPONENT, PIXELS(1544), PIXELS(192));
  if (battleResult < 0) {
    Engine_ActorSetAnimation(OPPONENT, 10);
  } else {
    Engine_ActorSetAnimation(OPPONENT, 8);
  }
  Actor_SetActionScript(OPPONENT, gColossoLog_Actor8Action);
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(1504), PIXELS(192));
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  {
    u16 shown = 0;

    actor->facing = shown;
  }
  Actor_SetActionScript(0, gColossoLog_LeaderActionA);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 35);
  Actor_SetSpeed(1, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(IVAN, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(MIA, PIXELS(1), PIXELS(1) / 2);
  Actor_SetPosition(GARET, PIXELS(1464), PIXELS(184));
  Actor_SetPosition(IVAN, PIXELS(1464), PIXELS(200));
  Actor_SetPosition(MIA, PIXELS(1448), PIXELS(192));
  actor = Object_GetById(1);
  {
    u16 shown = 0;

    actor->facing = shown;
  }
  actor = Object_GetById(2);
  {
    u16 shown = 0;

    actor->facing = shown;
  }
  actor = Object_GetById(3);
  {
    u16 shown = 0;

    actor->facing = shown;
  }
  Engine_TaskWait(1);
  Engine_CameraFollowActor(0, 0);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_GREY, 1);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventSetMessage(MsgColossoIsaac);
  Engine_EventWait(60);
  leaderScaleScript = gColossoMultiPhaseData;
  Actor_SetActionScript(ACTOR_PARTY_LEADER, leaderScaleScript);
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  actor->scaleX = Q16_ONE;
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  actor->scaleY = Q16_ONE;
  Engine_ActorSetAnimationAndWait(0, 36);
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  actor->x.fixed += PIXELS(3);
  Engine_EventWait(10);
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  Engine_ActorSetSpriteFlags(actor, 0);
  Engine_EventWait(20);
  Actor_SetActionScript(0, gColossoLog_LeaderActionB);
  // Let each companion react before the leader settles back to sleep.
  Event_ShowMessage(GARET, 0);
  Engine_EventWait(20);
  Actor_WalkToAndWait(1, 1504, 176);
  Actor_FaceDirection(GARET, FACING_SOUTH, 10);
  Engine_ActorShowEmote(1, 0x100, 20);
  Event_ShowMessage(IVAN, 0);
  Object_LinkObjectAndSetCallback(1, 2);
  Engine_EventWait(30);
  Actor_WalkToAndWait(IVAN, 1488, 176);
  Actor_WalkTo(GARET, 1520, 184);
  Actor_WalkToAndWait(IVAN, 1504, 176);
  Engine_ActorSetAnimation(GARET, 1);
  Actor_FaceDirection(GARET, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(IVAN, FACING_SOUTH, 10);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Engine_EventWait(10);
  Object_LinkObjectAndSetCallback(2, 1);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(1, 4);
  Engine_EventWait(30);
  Event_ShowMessage(MIA, 0);
  Object_LinkObjectAndSetCallback(1, 3);
  Object_LinkObjectAndSetCallback(2, 3);
  Actor_WalkToAndWait(MIA, 1488, 184);
  Object_LinkObjectAndSetCallback(2, 0);
  Object_LinkObjectAndSetCallback(1, 0);
  Actor_ShowEmote(GARET, 0x102, 60);
  Object_LinkObjectAndSetCallback(2, 1);
  Object_LinkObjectAndSetCallback(1, 2);
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(MIA, 2);
  Engine_EventWait(10);
  Object_LinkObjectAndSetCallback(2, 3);
  Object_LinkObjectAndSetCallback(1, 3);
  Engine_EventWait(20);
  Event_ShowMessage(MIA, 0);
  Actor_SetActionScript(0, gColossoLog_LeaderActionC);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Object_LinkObjectAndSetCallback(1, 0);
  Engine_EventWait(20);
  Event_ShowMessage(GARET, 0);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Object_LinkObjectAndSetCallback(2, 0);
  Engine_EventWait(20);
  Event_ShowMessage(IVAN, 0);
  Engine_ActorRunRepeatedMotion(MIA, 2);
  Object_LinkObjectAndSetCallback(3, 0);
  Engine_EventWait(20);
  Event_ShowMessage(MIA, 0);
  Actor_SetActionScript(ACTOR_PARTY_LEADER, leaderScaleScript);
  Engine_EventWait(60);
  particleCallback = ColossoLogRollingStage_SpawnPeriodicParticle;
  // Time the small reunion effects to the companions' reactions.
  gColossoParticleCount = 9;
  Task_AddCallback(particleCallback, 0xc80);
  Engine_EventWait(5);
  Engine_TaskRemoveCallback(particleCallback);
  Engine_EventWait(55);
  Actor_ShowEmote(GARET, 0x101, 60);
  Task_AddCallback(particleCallback, 0xc80);
  Engine_EventWait(20);
  Engine_TaskRemoveCallback(particleCallback);
  Engine_EventWait(40);
  Actor_ShowEmote(IVAN, 0x101, 60);
  gColossoParticleCount = 9;
  Task_AddCallback(particleCallback, 0xc80);
  Engine_EventWait(35);
  Engine_TaskRemoveCallback(particleCallback);
  Engine_EventWait(25);
  Actor_ShowEmote(MIA, 0x102, 60);
  gColossoParticleCount = 9;
  Task_AddCallback(particleCallback, 0xc80);
  Engine_EventWait(35);
  Engine_TaskRemoveCallback(particleCallback);
  Engine_EventWait(25);
  Actor_ShowEmote(2, 0x102, 60);
  Object_LinkObjectAndSetCallback(3, 2);
  Object_LinkObjectAndSetCallback(2, 3);
  Engine_EventWait(60);
  Object_LinkObjectAndSetCallback(3, 0);
  Object_LinkObjectAndSetCallback(2, 0);
  gColossoParticleCount = 9;
  Task_AddCallback(particleCallback, 0xc80);
  Engine_EventWait(35);
  Engine_TaskRemoveCallback(particleCallback);
  Engine_EventWait(25);
  Actor_ShowEmote(MIA, 0x108, 60);
  Engine_ActorStartRepeatedMotion(GARET, 3);
  Engine_ActorStartRepeatedMotion(2, 3);
  Engine_ActorRunRepeatedMotion(3, 3);
  Object_LinkObjectAndSetCallback(3, 2);
  Object_LinkObjectAndSetCallback(1, 2);
  gColossoParticleCount = 9;
  Task_AddCallback(particleCallback, 0xc80);
  Engine_ActorSetAnimation(GARET, 3);
  Engine_ActorSetAnimation(IVAN, 3);
  Engine_ActorSetAnimation(MIA, 3);
  Engine_EventWait(60);
  // The companions begin to leave, then discuss the sleeping leader off-screen.
  Actor_WalkTo(MIA, 1464, 200);
  Engine_EventWait(5);
  Actor_WalkTo(IVAN, 1368, 184);
  Engine_EventWait(3);
  Actor_WalkToAndWait(GARET, 1512, 184);
  Actor_WalkTo(GARET, 1368, 184);
  Engine_ActorWaitForMove(MIA);
  Engine_ActorSetAnimation(3, 1);
  Object_LinkObjectAndSetCallback(3, 0);
  Engine_EventWait(60);
  Actor_WalkToAndWait(MIA, 1432, 200);
  Actor_WalkTo(MIA, 1368, 184);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(30);
  Actor_SetPosition(GARET, PIXELS(1512), PIXELS(176));
  Actor_SetPosition(IVAN, PIXELS(1464), PIXELS(192));
  Actor_SetPosition(MIA, PIXELS(1560), PIXELS(200));
  ObjectSystem_SuspendProjectionAndShowSprites();
  ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_PLAIN, 2);
  Engine_ColorBufferInterpolate(1);
  Engine_EventSetMessage(MsgColossoIsaacFellAsleep);
  Event_ShowMessage(GARET, 0);
  Event_ShowMessage(IVAN, 0);
  Event_ShowMessage(MIA, 0);
  Engine_EventWait(60);
  Engine_EventEnd();
}

/// Stages the party behind the guide for the introduction to the third finals.
static void ColossoLog_IntroduceFinalCourse(s32 guideId) {
  s32 guideZ;
  s32 guideX;
  struct FieldActor *actor;

  actor = Object_GetById(guideId);
  guideX = actor->x.part.pixel;
  guideZ = actor->z.part.pixel;
  Engine_EventBegin();
  Actor_SetSpeed(guideId, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(0, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(1, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(2, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(3, PIXELS(1), PIXELS(1) / 2);
  Engine_ActorSetPosition(ACTOR_PARTY_LEADER, PIXELS(guideX),
                          (PIXELS(guideZ) - PIXELS(48)));
  Engine_ActorSetPosition(1, PIXELS(guideX) - PIXELS(16),
                          PIXELS(guideZ) - PIXELS(40));
  Engine_ActorSetPosition(2, PIXELS(guideX) + PIXELS(16),
                          PIXELS(guideZ) - PIXELS(40));
  Engine_ActorSetPosition(3, PIXELS(guideX), (PIXELS(guideZ) - PIXELS(32)));
  Engine_ActorSetPosition(guideId, PIXELS(guideX),
                          (PIXELS(guideZ) - PIXELS(80)));
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  actor->facing = FACING_NORTH;
  Engine_CameraFollowActor(0, 0);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventSetMessage(MsgColossoSiteThirdFinals);
  Engine_ActorSetAnimationAndWait(guideId, 3);
  Engine_EventShowMessage(guideId, 0);
  Engine_ActorRunRepeatedMotion(guideId, 2);
  Engine_EventShowMessage(guideId, 0);
  Engine_ActorRunRepeatedMotion(guideId, 2);
  Engine_EventShowMessage(guideId, 0);
  Engine_ActorRunRepeatedMotion(guideId, 2);
  Engine_EventShowMessage(guideId, 0);
  Engine_ActorSetAnimation(3, 3);
  Engine_ActorSetAnimation(1, 3);
  Engine_ActorSetAnimation(2, 3);
  Engine_ActorSetAnimationAndWait(0, 3);
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
  Engine_ActorWalkToAndWait(guideId, (guideX - 16), (guideZ - 64));
  Engine_ActorSetPosition(1, 0, 0);
  Engine_ActorSetPosition(2, 0, 0);
  Engine_ActorSetPosition(3, 0, 0);
  Engine_ActorWalkToAndWait(guideId, (guideX - 16), (guideZ - 16));
  Engine_ActorWalkToAndWait(guideId, guideX, guideZ);
  Engine_ActorFaceDirection(guideId, FACING_NORTH, 10);
  Engine_EventEnd();
}

/// The Scales of Justice's attendant: the first time, the pans are shown; then
/// the objective.
static void Event_TalkToScalesAttendant(s32 guideId) {
  struct FieldActor *pan1;
  struct FieldActor *pan2;
  struct FieldActor *back1;
  struct FieldActor *back2;
  s32 promptResult;

  if (gGameState.entrance == 2) {
    Colosso_AskDoneCheering(guideId);
    return;
  }
  Engine_EventBegin();
  promptResult = Colosso_GreetAtStage(guideId, 1);
  if (promptResult == 0) {
    // Point out the pans, demonstrate both heights, then restore the course.
    Engine_EventSetMessage(MsgColossoStageCalledScales);
    Engine_CameraSetSpeed(PIXELS(3), PIXELS(3) / 8);
    Engine_CameraMoveTo(PIXELS(152), -1, PIXELS(200), 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(30);
    Engine_EventShowMessage(guideId, 0);
    Colosso_ShowPointer(104, 68, 0);
    Engine_EventWait(60);
    Colosso_MovePointer(168, 96, 10);
    Engine_EventWait(70);
    Engine_EventShowMessage(guideId, 0);
    Colosso_HidePointer();
    Engine_TaskWait(2);
    pan1 = Object_GetById(SCALE_PAN_WEST);
    pan1->motionFlags = 0;
    ColossoLog_MoveOnScales(pan1, PIXELS(4));
    pan2 = Object_GetById(SCALE_PAN_EAST);
    pan2->motionFlags = 0;
    ColossoLog_MoveOnScales(pan2, PIXELS(32));
    FieldObject_WaitForMove(pan2);
    Engine_EventWait(45);
    back1 = Object_GetById(SCALE_PAN_WEST);
    back1->motionFlags = 0;
    ColossoLog_MoveOnScales(back1, PIXELS(32));
    back2 = Object_GetById(SCALE_PAN_EAST);
    back2->motionFlags = 0;
    ColossoLog_MoveOnScales(back2, PIXELS(4));
    FieldObject_WaitForMove(back2);
    Engine_EventWait(15);
    Engine_EventShowMessage(guideId, 0);
    Colosso_ShowPointer(104, 68, 0);
    Engine_EventWait(30);
    Colosso_MovePointer(168, 96, 10);
    Engine_EventWait(40);
    Colosso_MovePointer(104, 68, 10);
    Engine_EventWait(70);
    Engine_EventShowMessage(guideId, 0);
    Colosso_HidePointer();
    Engine_TaskWait(2);
    Engine_CameraFollowActor(0, 0);
    Colosso_RepeatStageNumber(guideId, 1);
  } else if (promptResult == 1) {
    Engine_EventSetMessage(MsgColossoObjectiveGetAcross);
    Engine_EventShowMessage(guideId, 0);
  }
  Colosso_OfferCheeringSpot(promptResult, guideId, 1);
  Engine_EventEnd();
}

/// The Stepping-Stone Stage's attendant: the first time, a warrior finds the
/// way; then the objective.
static void Event_TalkToSteppingStoneAttendant(s32 guideId) {
  s32 promptResult;
  s32 x;

  if (gGameState.entrance == 2) {
    Colosso_AskDoneCheering(guideId);
    return;
  }
  Engine_EventBegin();
  promptResult = Colosso_GreetAtStage(guideId, 2);
  if (promptResult == 0) {
    Engine_EventSetMessage(MsgColossoSteppingStoneStage);
    Camera_SetSpeed(PIXELS(3), PIXELS(3) / 8);
    Camera_MoveTo(PIXELS(376), -1, PIXELS(152), 1);
    x = 280;
    Engine_CameraWaitForMove();
    Engine_EventWait(30);
    Event_ShowMessage(guideId, 0);
    Colosso_FadeInCompetitor(0, x, 200);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 2, PIXELS(3) / 4);
    Colosso_WalkActorToAndStand(0, x, 152);
    Colosso_WalkActorToAndStand(0, 296, 152);
    Engine_EventWait(10);
    Engine_LeaderCheckAhead();
    Camera_MoveTo(-1, -1, -1, 0);
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 15);
    Engine_LeaderCheckAhead();
    Camera_MoveTo(-1, -1, -1, 0);
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 15);
    Engine_LeaderCheckAhead();
    Camera_MoveTo(-1, -1, -1, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 15);
    Event_ShowMessage(guideId, 0);
    Colosso_ShowPointer(96, 40, 0);
    Colosso_MovePointer(128, 40, 10);
    Engine_EventWait(30);
    Colosso_MovePointer(160, 40, 10);
    Engine_EventWait(30);
    Colosso_MovePointer(160, 72, 10);
    Engine_EventWait(30);
    Event_ShowMessage(guideId, 0);
    Colosso_HidePointer();
    Colosso_RestoreCompetitor(0);
    Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
    Colosso_RepeatStageNumber(guideId, 2);
  } else if (promptResult == 1) {
    Engine_EventSetMessage(MsgColossoYourGoalInStageSimple);
    Event_ShowMessage(guideId, 0);
  }
  Colosso_OfferCheeringSpot(promptResult, guideId, 2);
  Engine_EventEnd();
}

/// The Wall Wedge's attendant: the first time, a warrior times his way through
/// the moving wall; then that its operator will cheer for Isaac.
static void Event_TalkToWallWedgeAttendant(s32 guideId) {
  struct EventWork *work;
  s32 promptResult;

  if (gGameState.entrance == 2) {
    Colosso_AskDoneCheering(guideId);
  } else {
    Engine_EventBegin();
    promptResult = Colosso_GreetAtStage(guideId, 3);
    if (promptResult == 0) {
      // Show the pattern cycle before demonstrating a timed passage.
      work = gEventWork;
      Engine_EventSetMessage(MsgColossoPlaceCalledWall);
      ColossoLogRollingStage_ResetAndRunSceneTask();
      Engine_CameraSetSpeed(PIXELS(3), PIXELS(3) / 8);
      Engine_CameraMoveTo(PIXELS(616), -1, PIXELS(184), 1);
      Engine_CameraWaitForMove();
      Engine_EventWait(30);
      Event_ShowMessage(guideId, 0);
      ColossoLogRollingStage_StartSceneTask();
      Engine_EventWait(60);
      Engine_EventShowMessage(guideId, 0);
      Colosso_FadeInCompetitor(0, 504, 200);
      // UB: the void target is called through an incompatible return type.
      /* FAKEMATCH: called through a cast to the s32 call type the bytes show,
       * its result discarded, so r0 loads last; as a direct call, 3
       * instructions differ in tbs-en. */
      COLOSSO_LOG_FACE_DIRECTION(0, 0, 0);
      ColossoLogRollingStage_WaitForSceneTask();
      Actor_SetSpeed(0, PIXELS(3) / 2, PIXELS(3) / 4);
      Colosso_WalkActorTo(0, 680, 200);
      if (work->raisedTrigger != 5) {
        do {
          Engine_TaskWait(1);
          if (++promptResult > 239) {
            break;
          }
        } while (work->raisedTrigger != 5);
      }
      Event_PassSouthWallOpening();
      Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
      Actor_ShowEmote(0, 0x103, 60);
      Engine_EventShowMessage(guideId, 0);
      Colosso_RestoreCompetitor(0);
      Engine_CameraFollowActor(0, 0);
      Colosso_RepeatStageNumber(guideId, 3);
      work->raisedTrigger = 0;
    } else if (promptResult == 1) {
      Engine_EventSetMessage(MsgColossoOperatorWallsCheer);
      Engine_EventShowMessage(guideId, 0);
    }
    Colosso_OfferCheeringSpot(promptResult, guideId, 3);
    Engine_EventEnd();
  }
}

/// The Free-Climb Area's attendant: the first time, a warrior climbs across;
/// then the objective.
static void Event_TalkToFreeClimbAttendant(s32 guideId) {
  s32 result;
  struct FieldActor *actor;
  s32 x;
  s32 y;
  s32 raisedHeight;
  s32 shiftedX;

  if (gGameState.entrance == 2) {
    Colosso_AskDoneCheering(guideId);
  } else {
    Engine_EventBegin();
    result = Colosso_GreetAtStage(guideId, 4);
    if (result == 0) {
      Engine_EventSetMessage(MsgColossoPlaceNormallyCalledFreeClimb);
      Camera_SetSpeed(PIXELS(3), PIXELS(3) / 8);
      Camera_MoveTo(PIXELS(856), -1, PIXELS(168), 1);
      Engine_CameraWaitForMove();
      Engine_EventWait(30);
      Event_ShowMessage(guideId, 0);
      Event_ShowMessage(guideId, 0);
      Colosso_FadeInCompetitor(0, 816, 200);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 840, 200);
      Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
      Field_ClimbUp();
      Camera_MoveTo(-1, -1, -1, 0);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
      actor = Object_GetById(ACTOR_PARTY_LEADER);
      y = actor->y.fixed;
      x = actor->x.fixed;
      Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
      Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 10);
      raisedHeight = PIXELS(6) + y;
      Engine_ObjectSetPosition(actor, x, raisedHeight, actor->z.fixed);
      FieldObject_WaitForMove(actor);
      Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 14);
      shiftedX = PIXELS(64) + x;
      Engine_ObjectSetPosition(actor, shiftedX, raisedHeight, actor->z.fixed);
      FieldObject_WaitForMove(actor);
      Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 10);
      Engine_ObjectSetPosition(actor, shiftedX, y + PIXELS(54), actor->z.fixed);
      FieldObject_WaitForMove(actor);
      Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 15);
      Engine_ObjectSetPosition(actor, x + PIXELS(48), y + PIXELS(54),
                               actor->z.fixed);
      FieldObject_WaitForMove(actor);
      Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 12);
      Event_ShowMessage(guideId, 0);
      Colosso_RestoreCompetitor(0);
      Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
      Colosso_RepeatStageNumber(guideId, 4);
    } else if (result == 1) {
      Engine_EventSetMessage(MsgColossoClearStageMustAbleChange);
      Event_ShowMessage(guideId, 0);
    }
    Colosso_OfferCheeringSpot(result, guideId, 4);
    Engine_EventEnd();
  }
}

// DispatchObject is the engine's script-control view of these same actors.

/// Starts a scripted pixel destination while preserving the actor height.
static void ColossoLogRollingStage_PositionActor(s32 selector, s32 x, s32 z) {
  struct FieldActor *actor;

  actor = Engine_ActorLookup(selector);
  if (actor != 0) {
    ObjectDispatch_StartIdle((struct DispatchObject *)actor);
    Object_SetMode(actor, 5);
    Engine_ObjectSetPosition(actor, x << 16, actor->y.fixed, z << 16);
  }
}

/// Moves to a scripted pixel destination, waits, then restores the idle mode.
static void ColossoLogRollingStage_PositionAndActivateActor(s32 selector, s32 x,
                                                            s32 z) {
  struct FieldActor *actor;

  actor = Engine_ActorLookup(selector);
  if (actor != 0) {
    ObjectDispatch_StartIdle((struct DispatchObject *)actor);
    Object_SetMode(actor, 5);
    Engine_ObjectSetPosition(actor, x << 16, actor->y.fixed, z << 16);
    FieldObject_WaitForMove(actor);
    Object_SetMode(actor, 1);
  }
}

/// The Moving Sidewalk Stage's attendant: the first time, a warrior walks
/// against the belt; then the objective.
static void Event_TalkToMovingSidewalkAttendant(s32 guideId) {
  s32 i;
  s32 result;
  struct FieldActor *actor;

  if (gGameState.entrance == 2) {
    Colosso_AskDoneCheering(guideId);
  } else {
    Engine_EventBegin();
    result = Colosso_GreetAtStage(guideId, 5);
    if (result == 0) {
      // Walk against the sidewalk, then show how its current pushes the rider.
      Engine_EventSetMessage(MsgColossoCalledMovingSidewalkStage);
      Camera_SetSpeed(PIXELS(3), PIXELS(3) / 8);
      Camera_MoveTo(PIXELS(1080), -1, PIXELS(168), 1);
      Engine_CameraWaitForMove();
      Engine_EventWait(30);
      Event_ShowMessage(guideId, 0);
      Event_ShowMessage(guideId, 0);
      Colosso_FadeInCompetitor(0, 984, 184);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 2, PIXELS(3) / 4);
      ColossoLogRollingStage_PositionAndActivateActor(0, 992, 184);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 10, PIXELS(3) / 20);
      ColossoLogRollingStage_PositionActor(0, 1120, 184);
      Engine_EventWait(120);
      Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x101);
      Engine_EventWait(120);
      OverlayObject_ResetMotionFields(0);
      Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
#if !TBS_PATCH_3
      Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x100);
#endif
      Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x105, 0);
      actor = Object_GetById(ACTOR_PARTY_LEADER);
      for (i = 119; i >= 0; i--) {
        if (actor->x.fixed > PIXELS(992)) {
          actor->x.fixed -= PIXELS(6) / 5;
        }
        Engine_TaskWait(1);
      }
      Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x103, 60);
      ColossoLogRollingStage_PositionActor(0, 1120, 184);
      Event_ShowMessage(guideId, 0);
      OverlayObject_ResetMotionFields(0);
      gGameState.movementMode = 1;
      Colosso_RestoreCompetitor(0);
      Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
      Colosso_RepeatStageNumber(guideId, 5);
    } else if (result == 1) {
      Engine_EventSetMessage(MsgColossoInStageMustTryOutpace);
      Event_ShowMessage(guideId, 0);
    }
    Colosso_OfferCheeringSpot(result, guideId, 5);
    Engine_EventEnd();
  }
}

/// The log-rolling area's attendant: the first time, a marker traces the log's
/// route; then the objective.
static void Event_TalkToLogRollingAttendant(s32 actor) {
  s32 state;

  if (gGameState.entrance == 2) {
    Colosso_AskDoneCheering(actor);
    return;
  }

  Engine_EventBegin();
  state = Colosso_GreetAtStage(actor, 6);

  if (state == 0) {
    Engine_EventSetMessage(MsgColossoAnotherLogRollingArea);
    Engine_CameraSetSpeed(PIXELS(3), PIXELS(3) / 8);
    Engine_CameraMoveTo(PIXELS(1288), -1, PIXELS(152), 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(30);
    Engine_EventShowMessage(actor, 0);
    Colosso_ShowPointer(180, 88, 0);
    Engine_EventWait(60);
    Engine_EventShowMessage(actor, 0);
    Colosso_MovePointer(32, 84, 10);
    Engine_EventWait(30);
    Engine_EventShowMessage(actor, 0);
    Colosso_MovePointer(96, 84, 30);
    Engine_EventWait(60);
    Engine_EventShowMessage(actor, 0);
    Colosso_HidePointer();
    Engine_EventWait(2);
    Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
    Colosso_RepeatStageNumber(actor, 6);
  } else if (state == 1) {
    Engine_EventSetMessage(MsgColossoHereMustFigureOutHow);
    Engine_EventShowMessage(actor, 0);
  }

  Colosso_OfferCheeringSpot(state, actor, 6);
  Engine_EventEnd();
}

#include "common/colosso_solo.h"
#include "common/item_equip.h"
#include "common/colosso_task.h"

/// Selects the portrait task script and clears its movement state.
static void ColossoLogRollingStage_InitializeModeTask(u32 mode, u32 parameter) {
  s32 handler;

  gColosso_LetteringMode = mode;
  gColosso_LetteringParam = (parameter << 4);

  {
    s32 budget = 0xc80;
    SchedulerCallback task = Colosso_UpdateLettering;
    FieldScheduler_AddCallback(task, budget);
  }

  handler = (s32)gColossoModeScriptDefault;
  if (mode == 2) {
    handler = (s32)gColossoModeScript2;
  }
  if (mode == 4) {
    handler = (s32)gColossoModeScript4;
  }
  if (mode == 3) {
    if (parameter != 0) {
      handler = (s32)gColossoModeScript3B;
    } else {
      handler = (s32)gColossoModeScript3A;
    }
  }

  gColosso_LetteringTimer = 0;
  gColosso_LetteringScript = (s16 *)handler;
  gColosso_ModeMoveTarget = 0;
  gColosso_ModeMoveDuration = 0;
  gColosso_LetteringPosition = 0;
}

/// Plays the countdown portraits and the start-of-race transition.
static void ColossoLogRollingStage_RunScriptedTransition(s32 mode) {
  if (mode == 0) {
    Engine_EventBegin();
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(30);
    Engine_AudioPlayCue(MUS_089);
    Colosso_LoadLettering(0);
    ColossoLogRollingStage_InitializeModeTask(1, 0);
    Engine_EventWait(120);
    Engine_EventEnd();
    return;
  }

  Engine_AudioPlayCue(SE_247);
  Engine_EventBegin();
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  {
    s16 *base = gColossoModeScriptDefault;
    *(s16 *)((u8 *)base + 30) = (mode * 60);
  }
  Engine_EventWait(30);
  Engine_AudioPlayCue(mode + 0x5a);
  Colosso_LoadLettering(mode);
  ColossoLogRollingStage_InitializeModeTask(1, 0);
  Engine_EventWait(120);

  while (AudioCommand_GetStateByte() != 0) {
    Engine_TaskWait(1);
  }

  Engine_AudioPlayCue(SE_289);
  Colosso_LoadLettering(5);
  ColossoLogRollingStage_InitializeModeTask(2, 0);
  Engine_AudioPlayCue(SE_236);
  Engine_EventWait(60);
  ColossoLogRollingStage_InitializeModeTask(2, 1);
  Engine_AudioPlayCue(SE_236);
  Engine_EventWait(60);
  Colosso_LoadLettering(6);
  ColossoLogRollingStage_InitializeModeTask(2, 0);
  Engine_AudioPlayCue(SE_236);
  Engine_EventWait(60);
  Colosso_LoadLettering(7);
  ColossoLogRollingStage_InitializeModeTask(4, 0);
  Engine_AudioPlayCue(SE_237);
  Audio_PlayCueFromEventWork();
  Engine_EventEnd();
  Engine_GameFlagSet(0x123);
}

/// Unused portrait countdown for a signed number of seconds.
UNUSED static void FieldScene_RunScene3bcSequenceA(s32 countdown) {
  s32 actorId;

  Engine_AudioPlayCue(SE_247);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  gColossoModeScript3A[13] = countdown * 60;
  STORE_CONST(gColossoModeScript3B[13]) =
      (countdown < 0 ? -countdown : countdown) * 60;
  if (countdown < 0) {
    Engine_EventWait(30);
    Engine_AudioPlayCue(MUS_086);
    Colosso_LoadLettering(8);
    // UB: the void target is called through an incompatible return type.
    /* FAKEMATCH: called through a cast to the s32 call type the bytes show, its
     * result discarded, so r1 loads before r0; as a direct call, 2
     * instructions differ in tbs-en. */
    COLOSSO_LOG_INITIALIZE_MODE_TASK(3, 1);
    Engine_EventWait(-countdown * 60 + 60);
    actorId = 0;
  } else {
    Engine_EventWait(30);
    Engine_AudioPlayCue(countdown + 90);
    Colosso_LoadLettering(4);
    // UB: the void target is called through an incompatible return type.
    /* FAKEMATCH: called through the s32 cast, its result discarded, as in the
     * first arm; as a direct call, 2 instructions differ in tbs-en. */
    COLOSSO_LOG_INITIALIZE_MODE_TASK(3, 0);
    Engine_EventWait(countdown * 60 + 60);
    actorId = 8;
  }
  Engine_ActorShowEmote(actorId, 0x105, 0);
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

/// Writes eight hexadecimal digits followed by a terminator.
UNUSED static void WriteU32AsHex(u8 *hexText, u32 value) {
  s32 digitIndex;

  hexText += 8;
  *hexText = 0;
  hexText--;
  for (digitIndex = 7; digitIndex >= 0; digitIndex--) {
    *hexText = gColossoHexChars[value & 15];
    value >>= 4;
    hexText--;
  }
}

static void ColossoLogRollingStage_NoopSceneHook(void) {}

/// Marks the rival path recorder as finished.
UNUSED static void ColossoLog_MarkRivalFinished(void) {
  struct PathRecorder *recorder = (struct PathRecorder *)gSceneState;

  recorder->mode = 9;
}

/// Waits until the rival path recorder reaches its finished mode.
static void ColossoLog_WaitForRivalToFinish(void) {
  struct PathRecorder *recorder = (struct PathRecorder *)gSceneState;

  while (recorder->mode != 9) {
    Engine_TaskWait(1);
  }
}

/// Occasionally scatters an effect around a moving source actor.
static void
ColossoLogRollingStage_SpawnRandomSceneEffect(union FieldObject *fieldObject) {
  struct FieldActor *source = (struct FieldActor *)fieldObject;

  s32 position[3];
  u32 randomValue;

  if (source->velocityY >= -255 && source->velocityY <= 255) {
    source->motionFlags = 0;
  }
  randomValue = Engine_RandomNext();
  if (randomValue * 100 >> 16 <= 9) {
    struct FieldActor *effect;
    s32 angle;
    s32 radius;

    SET_VECTOR(position, source->x.fixed, source->y.fixed, source->z.fixed);
    angle = Engine_RandomNext();
    radius = Engine_RandomNext();
    Vector_AddPolarOffset(angle << 4, radius, position);
    {
      s32 x = position[0];
      s32 y = position[1];
      s32 z = position[2];

      effect = Engine_ObjectCreate(285, x, y, z);
    }
    if (effect != 0) {
      effect->motionFlags = 0;
      Engine_ActorSetSpriteFlags(effect, 0);
      Engine_ObjectSetScript(effect, gColossoRandomEffect);
      Object_SetMode(effect, 1);
      Object_SetMode(effect, 0);
    }
  }
}

/// Raises the linked item actor and clears the source link.
static s32 ColossoLogRollingStage_RaiseLinkedSceneEffect(
    struct ScriptInterpreter *interpreter) {
  struct FieldActor *source = (struct FieldActor *)interpreter;

  struct FieldActor *effect = Object_GetById(source->timer);

  Engine_ObjectSetPosition(effect, source->x.fixed,
                           source->y.fixed + PIXELS(36), source->z.fixed);
  effect->motionFlags = 0;
  Engine_ObjectSetScript(effect, gColossoLinkedEffect);
  Engine_AudioPlayCue(MUS_TREASURE_FOUND);
  source->timer = 0;
  return 0;
}

/// Moves to the available finish item, equips it and announces it.
static s32 ColossoLog_CollectFinishEquipment(s32 firstItem, s32 secondItem) {
  struct ColossoGaugeWork *work = (struct ColossoGaugeWork *)gColossoWork;
  struct FieldActor *actor;
  s32 rivalFinishedFirst;
  s32 x;
  s32 z;
  u16 *itemActorId;
  s16 *linkedItemActor;

  rivalFinishedFirst = Engine_GameFlagIsSet(FLAG_SCENE_TEMP_11);
  actor = Object_GetById(gGameState.selectedActor);
  if (work->finishX < actor->x.fixed) {
    x = work->finishX + PIXELS(12);
  } else {
    x = work->finishX - PIXELS(12);
  }
  if (rivalFinishedFirst != 0) {
    z = work->finishZ + PIXELS(16);
    itemActorId = &work->finishItemActorB;
  } else {
    z = work->finishZ - PIXELS(16);
    itemActorId = &work->finishItemActorA;
  }
  linkedItemActor = &actor->timer;
  *linkedItemActor = *itemActorId;
  actor->acceleration = PIXELS(1) / 4;
  actor->speed = PIXELS(1);
  Engine_ObjectSetPosition(actor, x, 0, z);
  Engine_GameFlagSet(FLAG_SCENE_TEMP_11);
  Engine_ObjectSetScript(actor, gColossoLog_PlaceScript);
  while (*linkedItemActor != 0) {
    Engine_TaskWait(1);
  }
  if (rivalFinishedFirst == 0) {
    // UB: the void target is called through an incompatible return type.
    /* FAKEMATCH: the void function is called as returning s32, which loads r1
     * before r0; by its prototype, 2 instructions differ in tbs-en. */
    COLOSSO_LOG_APPLY_MATCHING_SLOT_VALUES(0, firstItem);
    UiWork_PushValueSlot(firstItem, 2);
  } else {
    // UB: the void target is called through an incompatible return type.
    /* FAKEMATCH: called as returning s32, as in the first arm; by its
     * prototype, 2 instructions differ in tbs-en. */
    COLOSSO_LOG_APPLY_MATCHING_SLOT_VALUES(0, secondItem);
    UiWork_PushValueSlot(secondItem, 2);
  }
  UiWork_PushValueSlot(gGameState.selectedActor, 1);
  Engine_MessageShowCentered(MsgColossoIsaacGotItem, 3);
  ObjectDispatch_WaitForValue16(actor);
  return rivalFinishedFirst;
}

#include "common/colosso_path_rival.h"
#include "common/colosso_setup.h"
#include "common/colosso_scene_state.h"
#include "common/object_findslot.h"

/// Pushes a neighboring actor one cell when its destination and overhead are
/// clear.
static void ColossoLog_PushActorAhead(void) {
  struct FieldActor *subject;
  struct FieldActor *target;
  struct FieldActor *blocker;
  u32 step;
  u32 direction;
  struct FieldPosition position;
  s32 zero;
  s32 subjectHandle;

  subjectHandle = gGameState.selectedActor;
  subject = Object_GetById(subjectHandle);

  direction = subject->facing >> 12;

  step = gStagedActor_DirectionSteps[direction];
  position.x = subject->x.fixed + (step & 0xffff0000);
  position.y = subject->y.fixed;
  step <<= 16;
  position.z = subject->z.fixed + step;

  target = SceneData_FindSlotAtPosition(&position, subject);
  if (target == 0) {
    return;
  }

  step = gStagedActor_DirectionSteps[direction];
  position.x = target->x.fixed + (step & 0xffff0000);
  position.y = target->y.fixed;
  step <<= 16;
  position.z = target->z.fixed + step;

  blocker = SceneData_FindSlotAtPosition(&position, target);
  if (blocker != 0 && (blocker->collisionFlags & 1) != 0) {
    return;
  }

  position.x = target->x.fixed;
  position.y = target->y.fixed + PIXELS(16);
  position.z = target->z.fixed;

  blocker = SceneData_FindSlotAtPosition(&position, target);
  if (blocker != 0 && (blocker->collisionFlags & 1) != 0) {
    return;
  }

  target->terrainId = 2;
  zero = 0;

  step = gStagedActor_DirectionSteps[direction];
  position.x = target->x.fixed + (step & 0xffff0000);
  position.y = target->y.fixed;
  step <<= 16;
  position.z = target->z.fixed + step;

  if (FieldObject_CheckMovementCollision(target, &position) > 0) {
    return;
  }

  Object_SetMode(subject, 8);
  Engine_TaskWait(15);

  target->speed = PIXELS(1) / 5;
  target->acceleration = PIXELS(1) / 5;
  Engine_ObjectSetPosition(target, position.x, position.y, position.z);

  subject->speed = PIXELS(1) / 5;
  subject->acceleration = PIXELS(1) / 5;
  Engine_ObjectSetPosition(subject, position.x, position.y, position.z);

  Engine_AudioPlayCue(SE_238);
  FieldObject_WaitForMove(target);
  Engine_AudioPlayCue(SE_288);

  target->x.fixed = position.x;
  target->z.fixed = position.z;
  target->velocityX = zero;
  target->velocityZ = zero;

  Object_SetMode(subject, 1);
}

#include "common/colosso_occupant.h"

const s32 gColossoLog_Record30Script[] = {
    SC_SET(SW_Y, PIXELS(1)),     SC_WAIT(10), SC_SET(SW_Y, 0), SC_WAIT(10),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

const s8 gColossoLog_LogPattern[][6] = {
    {7, 5, 7, 5, 7, 11},
    {4, 6, 4, 6, 4, 10},
    {5, 7, 5, 7, 5, 12},
    {6, 4, 6, 4, 6, 10},
};

// clang-format off
const u16 gColossoEarlySequenceData[] = {
    0, 0, 0x4000, 0x8000, 0x8000, 0x8000, 0xc000, 0,
};

const s32 gColossoSceneEventEffect[] = {
    SC_MOVE(PIXELS(1088), 0, PIXELS(184)),
    SC_SYNC,
    SC_MOVE(PIXELS(1130), 0, PIXELS(184)),
    SC_SYNC,
    SC_CALL(ColossoLog_ShowActivatedSidewalkLayer),
    SC_LABEL(1),
    SC_SET(SW_Y, PIXELS(1)),
    SC_WAIT(10),
    SC_SET(SW_Y, 0),
    SC_WAIT(10),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
};

const u8 gColosso_LetteringPaletteOffsets[] = {
#if LANGUAGE == LANGUAGE_JAPANESE || LANGUAGE == LANGUAGE_ENGLISH
    0, 32, 32, 32, 96, 64, 64, 64, 128, 0,
#else
    0, 32, 32, 32, 96, 64, 64, 160, 128, 0,
#endif
};

const s16 gColossoModeScript2[] = {
    0x1000, 512, 1, 0x2000, 0, 1, 0x1000, 256,
    6, 0x2000, 16, 6, 0x7fff, 30, 0x2000, 0,
    30, 0x7fff, 30, -1,
};

const s16 gColossoModeScript3B[] = {
    0x1000, 128, 1, 0x2000, 0, 1, 0x1000, 256,
    6, 0x2000, 16, 6, 0x7fff, 60, 0x1000, 384,
    6, 0x7fff, 6, 0x1000, 240, 6, 0x7fff, 6,
    0x1000, 368, 6, 0x7fff, 6, 0x1000, 224, 6,
    0x7fff, 6, 0x2000, 0, 30, 0x1000, 352, 6,
    0x7fff, 6, 0x1000, 208, 6, 0x7fff, 6, 0x1000,
    336, 6, 0x7fff, 6, 0x1000, 192, 6, 0x7fff,
    6, 0x1000, 320, 6, 0x7fff, 6, -1,
};

char gColossoHexChars[20] = "0123456789ABCDEF";

s32 gColossoRandomEffect[] = {
    SC_ADD(SW_Y, -(PIXELS(1) / 4)),
    SC_WAIT(1),
    SC_LOOP(60, SC_TOP),
    SC_FREE,
};

s32 gColossoLinkedEffect[] = {
    SC_SET(SW_HOOK, (s32)ColossoLogRollingStage_SpawnRandomSceneEffect),
    SC_LABEL(1),
    SC_WAIT(10),
    SC_SET(SW_SCALE_X, 11 * Q16_ONE / 8),
    SC_SET(SW_SCALE_Y, 11 * Q16_ONE / 8),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 3 * Q16_ONE / 2),
    SC_SET(SW_SCALE_Y, 3 * Q16_ONE / 2),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 9 * Q16_ONE / 8),
    SC_SET(SW_SCALE_Y, 9 * Q16_ONE / 8),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, Q16_ONE),
    SC_SET(SW_SCALE_Y, Q16_ONE),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 11 * Q16_ONE / 8),
    SC_SET(SW_SCALE_Y, 11 * Q16_ONE / 8),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 3 * Q16_ONE / 2),
    SC_SET(SW_SCALE_Y, 3 * Q16_ONE / 2),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 9 * Q16_ONE / 8),
    SC_SET(SW_SCALE_Y, 9 * Q16_ONE / 8),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, Q16_ONE),
    SC_SET(SW_SCALE_Y, Q16_ONE),
    SC_WAIT(2),
    SC_WAIT(50),
    SC_LOOP(2, SC_TO(1)),
    SC_POS(0, 0, 0),
    SC_SET(SW_HOOK, 0),
    SC_END,
};

/// Pixel step by direction: X in the upper halfword, Z in the lower.
#define DIRECTION_STEP(x, z) ((u32)(x) << 16 | (u16)(z))

u32 gStagedActor_DirectionSteps[] = {
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

struct SceneEntrance gColossoLogEntrances[] = {
    { 0, CONDITION_ALWAYS, 88, 0, 104, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { 1, CONDITION_ALWAYS, 88, 0, 104, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { 2, CONDITION_ALWAYS, 136, 0, 392, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { 4, CONDITION_ALWAYS, 88, 0, 104, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { 5, CONDITION_ALWAYS, 88, 0, 104, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

u32 gColossoLogExits[] = {
    SCENE_EXITS(145),
    SCENE_EXIT(10, 145, 1),
    SCENE_EXIT(11, 140, 66),
    SCENE_EXIT(4, 138, 99),
    SCENE_EXIT(5, 138, 99),
    SCENE_EXITS_END,
};

struct ScenePlacement gColossoLogPlacements[] = {
    { 0x0, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(88), 0, PIXELS(104), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { IVAN, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { MIA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0x3e, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0xe3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(216), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x122, CONDITION_ALWAYS, 7,
        PIXELS(152), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x122, CONDITION_ALWAYS, 7,
        PIXELS(216), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(88), 0, PIXELS(168), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(312), 0, PIXELS(184), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(712), 0, PIXELS(184), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1240), 0, PIXELS(136), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1272), 0, PIXELS(168), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1304), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x123, CONDITION_ALWAYS, 7,
        PIXELS(536), 0, PIXELS(184), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x123, CONDITION_ALWAYS, 7,
        PIXELS(568), 0, PIXELS(184), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x123, CONDITION_ALWAYS, 7,
        PIXELS(600), 0, PIXELS(184), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x123, CONDITION_ALWAYS, 7,
        PIXELS(632), 0, PIXELS(184), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x123, CONDITION_ALWAYS, 7,
        PIXELS(664), 0, PIXELS(184), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x123, CONDITION_ALWAYS, 7,
        PIXELS(536), 0, PIXELS(216), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x123, CONDITION_ALWAYS, 7,
        PIXELS(568), 0, PIXELS(216), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x123, CONDITION_ALWAYS, 7,
        PIXELS(600), 0, PIXELS(216), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x123, CONDITION_ALWAYS, 7,
        PIXELS(632), 0, PIXELS(216), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x123, CONDITION_ALWAYS, 7,
        PIXELS(664), 0, PIXELS(216), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xa5, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(720), 0, PIXELS(216), FACING_WEST, TALK_KEEP_FACING, 0 },
    { 0xee, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(792), 0, PIXELS(194), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x126, CONDITION_ALWAYS, 7,
        PIXELS(1080), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xcd, CONDITION_ALWAYS, 7,
        PIXELS(1208), 0, PIXELS(176), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x98, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(136), 0, PIXELS(408), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0xa5, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(152), 0, PIXELS(312), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0xa5, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(376), 0, PIXELS(312), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0xa5, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(600), 0, PIXELS(312), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0xa5, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(856), 0, PIXELS(312), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0xa5, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1080), 0, PIXELS(312), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0xa5, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1304), 0, PIXELS(312), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x16, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1512), 0, PIXELS(176), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x16, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(1512), 0, PIXELS(208), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x118, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x118, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x118, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SCENE_TABLE_END },
};

s32 gColossoSceneTaskStatus = 0;

s32 gColossoSceneTaskState = 0;

struct SceneEvent gColossoLogEvents[] = {
    { TOUCH_WALKING, 90, CONDITION_ALWAYS, EVENT_SCRIPT(Event_CheckAhead) },
    { TOUCH_WALKING, 91, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushWeightOrCheckAhead) },
    { EVENT_TOUCH, 11, 0x363, EVENT_SCRIPT(Event_RideMovingSidewalk) },
    { TOUCH_WALKING, 20, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushWeight) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), SCALE_WEIGHT, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SetWeightOnScales) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), STONE, CONDITION_ALWAYS, EVENT_SCRIPT(Event_MoveStone) },
    { TOUCH_WALKING, 22, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushRock) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 14, CONDITION_ALWAYS, EVENT_SCRIPT(Event_IgnoreMove) },
    { TOUCH_WALKING, 23, CONDITION_ALWAYS, EVENT_SCRIPT(Event_RollLongLog) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 15, FLAG_SCENE_TEMP_14, EVENT_SCRIPT(Event_SettleRocks) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 16, FLAG_SCENE_TEMP_15, EVENT_SCRIPT(Event_SettleRocks) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 17, FLAG_SCENE_TEMP_16, EVENT_SCRIPT(Event_SettleRocks) },
    { EVENT_TOUCH, 15, 0x367, EVENT_SCRIPT(Event_StepOnScales) },
    { EVENT_TOUCH, 16, IF_FLAG_SET(0x367), EVENT_SCRIPT(Event_StepOffScales) },
    { EVENT_TOUCH, 50, FLAG_SCENE_TEMP_10, EVENT_SCRIPT(Event_ReachArena) },
    { SEARCH(SEARCH_CHEST), 101, 0x36a, FIND_ITEM(ITEM_SMOKE_BOMB) },
    { SEARCH(SEARCH_CHEST), 102, 0x36b, FIND_ITEM(ITEM_NUT) },
    { 0x8413, 103, 0x36c, FIND_ITEM(ITEM_SLEEP_BOMB) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST) | 0x50000000, FROST_TARGET, 0x362, EVENT_SCRIPT(Event_FreezeOnScales) },
    { PSYNERGY_ON_ACTOR(ABILITY_GROWTH), GROWTH_PLANT, 0x360, EVENT_SCRIPT(Event_GrowPlant) },
    { PSYNERGY_ON_ACTOR(ABILITY_HALT), WALL_OPERATOR, 0x361, EVENT_SCRIPT(Event_HaltWall) },
    { PSYNERGY_ON_ACTOR(ABILITY_FORCE) | 0x50000000, SIDEWALK_BLOCK, 0x363, EVENT_SCRIPT(Event_ForceSidewalkBlock) },
    { EVENT_RAISED, 4, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PassNorthWallOpening) },
    { EVENT_RAISED, 5, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PassSouthWallOpening) },
    { EVENT_TALK, GUIDE, CONDITION_ALWAYS, EVENT_SCRIPT(Colosso_AskReadyToRace) },
    { EVENT_TALK, SCALES_ATTENDANT, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToScalesAttendant) },
    { EVENT_TALK, STEPPING_STONE_ATTENDANT, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToSteppingStoneAttendant) },
    { EVENT_TALK, WALL_WEDGE_ATTENDANT, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToWallWedgeAttendant) },
    { EVENT_TALK, FREE_CLIMB_ATTENDANT, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToFreeClimbAttendant) },
    { EVENT_TALK, MOVING_SIDEWALK_ATTENDANT, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToMovingSidewalkAttendant) },
    { EVENT_TALK, LOG_ROLLING_ATTENDANT, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToLogRollingAttendant) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), GUIDE, CONDITION_ALWAYS, MSGNO(MsgColossoRiverThisYearsFinalsAreIncredibleThis) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), SCALES_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoLogTheStreetVendorsCornOnThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), STEPPING_STONE_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoLogICantSeeAnythingWhenIm) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), WALL_WEDGE_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoLogItsNotVeryExcitingWorkBut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), FREE_CLIMB_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoLogStandingForLongPeriodsOfTime) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), MOVING_SIDEWALK_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoLogIWasUpAllNightTrying) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), LOG_ROLLING_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoLogSomeTouristToldMeILook) },
    { EVENT_RAISED, 99, CONDITION_ALWAYS, EVENT_SCRIPT(Colosso_TakePlaceForMatch) },
    { SCENE_EVENTS_END },
};

s32 gColossoLog_Actor8Action[] = {
    SC_ADD(SW_SCALE_Y, 512),
    SC_ADD(SW_SCALE_X, -256),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, 512),
    SC_ADD(SW_SCALE_X, -256),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, 512),
    SC_ADD(SW_SCALE_X, -256),
    SC_WAIT(4),
    SC_ADD(SW_SCALE_Y, -512),
    SC_ADD(SW_SCALE_X, 256),
    SC_WAIT(4),
    SC_ADD(SW_SCALE_Y, -512),
    SC_ADD(SW_SCALE_X, 256),
    SC_WAIT(4),
    SC_ADD(SW_SCALE_Y, -512),
    SC_ADD(SW_SCALE_X, 256),
    SC_WAIT(10),
    SC_LOOP(SC_FOREVER, SC_TOP),
    SC_END,
};

s32 gColossoLog_LeaderActionA[] = {
    SC_ADD(SW_SCALE_Y, 512),
    SC_ADD(SW_SCALE_X, -256),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, 512),
    SC_ADD(SW_SCALE_X, -256),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, 512),
    SC_ADD(SW_SCALE_X, -256),
    SC_WAIT(6),
    SC_ADD(SW_SCALE_Y, -512),
    SC_ADD(SW_SCALE_X, 256),
    SC_WAIT(4),
    SC_ADD(SW_SCALE_Y, -512),
    SC_ADD(SW_SCALE_X, 256),
    SC_WAIT(4),
    SC_ADD(SW_SCALE_Y, -512),
    SC_ADD(SW_SCALE_X, 256),
    SC_WAIT(12),
    SC_LOOP(SC_FOREVER, SC_TOP),
    SC_END,
};

s32 gColossoLog_LeaderActionB[] = {
    SC_SET(SW_SCALE_X, Q16_ONE),
    SC_SET(SW_SCALE_Y, Q16_ONE),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_Y, 3 * Q16_ONE / 64),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, 3 * Q16_ONE / 64),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, 3 * Q16_ONE / 64),
    SC_WAIT(16),
    SC_ADD(SW_SCALE_Y, -(3 * Q16_ONE / 64)),
    SC_WAIT(4),
    SC_ADD(SW_SCALE_Y, -(3 * Q16_ONE / 64)),
    SC_WAIT(4),
    SC_ADD(SW_SCALE_Y, -(3 * Q16_ONE / 64)),
    SC_WAIT(32),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
};

s32 gColossoLog_LeaderActionC[] = {
    SC_SET(SW_SCALE_X, Q16_ONE),
    SC_SET(SW_SCALE_Y, Q16_ONE),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_Y, 3 * Q16_ONE / 64),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, 3 * Q16_ONE / 64),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_Y, 3 * Q16_ONE / 64),
    SC_WAIT(26),
    SC_ADD(SW_SCALE_Y, -(3 * Q16_ONE / 64)),
    SC_WAIT(4),
    SC_ADD(SW_SCALE_Y, -(3 * Q16_ONE / 64)),
    SC_WAIT(4),
    SC_ADD(SW_SCALE_Y, -(3 * Q16_ONE / 64)),
    SC_WAIT(52),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
};

s32 gColossoMultiPhaseData[] = {
    SC_SET(SW_SCALE_X, Q16_ONE),
    SC_SET(SW_SCALE_Y, Q16_ONE),
    SC_END,
};

s32 gColossoParticleKinds[] = {
    SC_CALL(ColossoLogRollingStage_AdvanceParticleMotion),
    SC_WAIT(1),
    SC_LOOP(60, SC_TOP),
    SC_POS(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

s16 gColosso_LetteringSlot = -1;

s16 gColossoModeScriptDefault[] = {
#if LANGUAGE == LANGUAGE_JAPANESE
    0x4000, -188, 2048, 0x1000, 256, 1, 0x2000, 16,
#else
    0x4000, -188, 2048, 0x1000, 384, 1, 0x2000, 16,
#endif
    1, 0x7fff, 14, 0x3000, 0, 20, 0x7fff, 60,
    0x3000, 2048, 20, 0x7fff, 60, -1,
};

s16 gColossoModeScript4[] = {
#if LANGUAGE == LANGUAGE_FRENCH
    0x1000, 512, 1, 0x2000, 0, 1, 0x1000, 384,
    6, 0x2000, 16, 6, 0x7fff, 6, 0x1000, 512,
    6, 0x7fff, 6, 0x1000, 384, 6, 0x7fff, 6,
    0x1000, 512, 6, 0x7fff, 6, 0x1000, 384, 6,
    0x7fff, 6, 0x2000, 0, 30, 0x1000, 512, 6,
    0x7fff, 6, 0x1000, 384, 6, 0x7fff, 6, 0x1000,
    512, 6, 0x7fff, 6, 0x1000, 384, 6, 0x7fff,
    6, 0x1000, 512, 6, 0x7fff, 6, -1,
#elif LANGUAGE == LANGUAGE_SPANISH || LANGUAGE == LANGUAGE_ITALIAN
    0x1000, 384, 1, 0x2000, 0, 1, 0x1000, 256,
    6, 0x2000, 16, 6, 0x7fff, 6, 0x1000, 384,
    6, 0x7fff, 6, 0x1000, 256, 6, 0x7fff, 6,
    0x1000, 384, 6, 0x7fff, 6, 0x1000, 256, 6,
    0x7fff, 6, 0x2000, 0, 30, 0x1000, 384, 6,
    0x7fff, 6, 0x1000, 256, 6, 0x7fff, 6, 0x1000,
    384, 6, 0x7fff, 6, 0x1000, 256, 6, 0x7fff,
    6, 0x1000, 384, 6, 0x7fff, 6, -1,
#else
    0x1000, 512, 1, 0x2000, 0, 1, 0x1000, 256,
    6, 0x2000, 16, 6, 0x7fff, 6, 0x1000, 384,
    6, 0x7fff, 6, 0x1000, 256, 6, 0x7fff, 6,
    0x1000, 384, 6, 0x7fff, 6, 0x1000, 256, 6,
    0x7fff, 6, 0x2000, 0, 30, 0x1000, 384, 6,
    0x7fff, 6, 0x1000, 256, 6, 0x7fff, 6, 0x1000,
    384, 6, 0x7fff, 6, 0x1000, 256, 6, 0x7fff,
    6, 0x1000, 384, 6, 0x7fff, 6, -1,
#endif
};

s16 gColossoModeScript3A[] = {
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
    SC_CALL(ColossoLogRollingStage_RaiseLinkedSceneEffect),
    SC_WAIT(3),
    SC_ARG(10),
    SC_WAIT(150),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_ARG(1),
    SC_END,
};

s32 gColossoLog_PlaceScript[] = {
    SC_ARG(2),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(6),
    SC_CALL(ColossoLogRollingStage_RaiseLinkedSceneEffect),
    SC_WAIT(3),
    SC_ARG(28),
    SC_WAIT(150),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_ARG(1),
    SC_END,
};
