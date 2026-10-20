/// @file
/// Mogall Forest's three areas. Apes (sprite 0xbf) hide in the undergrowth;
/// Force on a hiding place (an invisible actor, sprite 0x16, on the same spot)
/// brings one out. Most point the way on and leave; the hostile ones attack,
/// fought as encounter table 53 slot 0, the "Ape", and the last is the Killer
/// Ape of slot 1. Logs (sprites 0xcd and 0xcf) can be pushed into the river.

#include "script.h"
#include "game_state.h"
#include "engine_call.h"
#include "global_cells.h"
#include "unit/import.h"
#include "unit/scene/mogall_forest.h"
#include "unit/scene/common/object_orbit.h"
#include "unit/scene/common/effect_spawn.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"

#define FIELD_STAGED_ACTOR_IMPORTS
#include "field_obj.h"
#include "mapcopy.h"
#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "djinn_ids.h"
#include "field_effect.h"
#include "staged_actor.h"
#include "select_overlay_data_by_runtime_selector.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "edition.h"
#include "call.h"
#include "scene_ids.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "objcmd.h"
#include "field_runtime.h"
#include "fixed_math.h"
#include "vector3.h"
#include "flags.h"
#include "item_ids.h"
#include "songs.h"

/// The first area.
enum MogallForest1Actor {
  ACTOR_FIRST_AREA_RIVER_LOG = ACTOR_FIRST_PLACED + 1,
  ACTOR_FIRST_AREA_NORTH_APE,
  ACTOR_FIRST_AREA_HOSTILE_APE,
  ACTOR_FIRST_AREA_EAST_APE,
  ACTOR_FIRST_AREA_NORTH_HIDEOUT,
  ACTOR_FIRST_AREA_HOSTILE_HIDEOUT,
  ACTOR_FIRST_AREA_EAST_HIDEOUT,
  ACTOR_NUT ///< a Nut floating over the river, caught with Catch
};

/// The second area.
enum MogallForest2Actor {
  ACTOR_SECOND_AREA_LOG = ACTOR_FIRST_PLACED,
  ACTOR_SECOND_AREA_RIVER_LOG = ACTOR_FIRST_PLACED + 2,
  ACTOR_SECOND_AREA_OBSTACLE, ///< sprite 0x125, moved with Move
  ACTOR_SECOND_AREA_NORTH_APE,
  ACTOR_SECOND_AREA_EAST_APE,
  ACTOR_SECOND_AREA_SOUTH_APE,
  ACTOR_SECOND_AREA_NORTH_HIDEOUT,
  ACTOR_SECOND_AREA_EAST_HIDEOUT,
  ACTOR_SECOND_AREA_SOUTH_HIDEOUT,
  ACTOR_QUARTZ ///< Venus Djinni 2
};

/// The third area.
enum MogallForest3Actor {
  ACTOR_THIRD_AREA_LOG = ACTOR_FIRST_PLACED,
  ACTOR_THIRD_AREA_RIVER_LOG_1,
  ACTOR_THIRD_AREA_RIVER_LOG_2 = ACTOR_FIRST_PLACED + 3,
  ACTOR_THIRD_AREA_NORTH_OBSTACLE = ACTOR_FIRST_PLACED + 5,
  ACTOR_THIRD_AREA_SOUTH_OBSTACLE,
  ACTOR_THIRD_AREA_HOSTILE_APE,
  ACTOR_THIRD_AREA_CENTRAL_APE,
  ACTOR_THIRD_AREA_WEST_APE,
  ACTOR_THIRD_AREA_EAST_APE, ///< later the Killer Ape
  ACTOR_THIRD_AREA_HOSTILE_HIDEOUT,
  ACTOR_THIRD_AREA_CENTRAL_HIDEOUT,
  ACTOR_THIRD_AREA_WEST_HIDEOUT,
  ACTOR_THIRD_AREA_EAST_HIDEOUT
};

/// Kept until the party leaves the forest: each ape once it has come out
/// (or, for two, moved on to a second hiding place) and each rolled log.
enum {
  FLAG_FIRST_AREA_NORTH_APE_OUT = 0x300,
  FLAG_FIRST_AREA_HOSTILE_APE_OUT,
  FLAG_FIRST_AREA_EAST_APE_OUT,
  FLAG_SECOND_AREA_NORTH_APE_OUT,
  FLAG_SECOND_AREA_EAST_APE_OUT,
  FLAG_SECOND_AREA_SOUTH_APE_MOVED,
  FLAG_SECOND_AREA_SOUTH_APE_OUT,
  FLAG_THIRD_AREA_HOSTILE_APE_OUT,
  FLAG_THIRD_AREA_CENTRAL_APE_OUT,
  FLAG_THIRD_AREA_WEST_APE_OUT,
  FLAG_THIRD_AREA_EAST_APE_MOVED,
  FLAG_THIRD_AREA_EAST_APE_OUT,
  FLAG_FIRST_AREA_LOG_ROLLED = 0x310,
  FLAG_SECOND_AREA_LOG_ROLLED,
  FLAG_THIRD_AREA_FIRST_LOG_ROLLED,
  FLAG_THIRD_AREA_SECOND_LOG_ROLLED,
  FLAG_PSYNERGY_STONE = 0x350
};

/// Scene flag, kept while the party stays in the area.
enum { FLAG_SOUTH_OBSTACLE_SETTLED = FLAG_SCENE_TEMP_00 + 0x14 };

enum {
  FLAG_NUT_CAUGHT = 0xfd4,
  FLAG_CHEST_APPLE = 0xf1b,
  FLAG_CHEST_ITEM_5D = 0xf1a
};

// The third area's east ape: in the first patch it is placed only until it
// has come out, and from the first patch on the hiding place that shares its
// sprite too.
#if TBS_PATCH_LEVEL == 1
#define MOGALL_FOREST_EAST_ACTOR_CONDITION FLAG_THIRD_AREA_EAST_APE_OUT
#else
#define MOGALL_FOREST_EAST_ACTOR_CONDITION CONDITION_ALWAYS
#endif

#if !TBS_PATCH_1
#define MOGALL_FOREST_EAST_SHARED_SPRITE_CONDITION CONDITION_ALWAYS
#else
#define MOGALL_FOREST_EAST_SHARED_SPRITE_CONDITION FLAG_THIRD_AREA_EAST_APE_OUT
#endif

#include "common/object_staged_actor.h"

#include "common/effect_create.h"
#include "common/effect_spawn.h"

/// Talking to Quartz in the second area: the Djinni battle (Djinni 2).
static void MogallForest_StartDjinnBattle(void) {
  Field_RequestDjinnBattle(18, DJINNI_QUARTZ);
}

/// Jumps the leader to an unobstructed fixed-point position and restores
/// motion.
static s32 MogallForest_TryJumpLeaderTo(struct FieldPosition *position) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  u8 *motionFlags = &leader->motionFlags;
  s32 savedMotionFlags = *motionFlags;

  s32 collision = FieldObject_CheckMovementCollision(leader, position);

  if (collision == 0) {
    s32 jumpMotionFlags;

    Engine_EventBegin();
    Object_SetMode(leader, 6);
    Engine_TaskWait(6);
    Engine_AudioPlayCue(SE_JUMP);
    Object_SetMode(leader, 7);
    leader->speed = PIXELS(3);
    leader->acceleration = PIXELS(2);
    leader->velocityY = PIXELS(4);
    jumpMotionFlags = 0x7e;
    jumpMotionFlags &= *motionFlags;
    *motionFlags = jumpMotionFlags;
    Engine_ActorSetSpriteFlags(leader, 0);
    Engine_ActorMoveToAndWait(ACTOR_PARTY_LEADER, (s16)((u32)position->x >> 16),
                              (s16)((u32)position->z >> 16));
    Object_SetMode(leader, 6);
    Engine_ActorSetSpriteFlags(leader, 1);
    *motionFlags = savedMotionFlags;
    Engine_EventEnd();
    return 1;
  }
  return 0;
}

/// Script command: the actor shows the part its timer names.
static s32
MogallForest_CopyTimerToChild(struct ScriptInterpreter *interpreter) {
  struct FieldActor *actor = (struct FieldActor *)interpreter;
  ObjectGroup_SetChildValue(actor, actor->timer);
  return 0;
}

/// Clears the actor sprite flags when its script requests it.
static s32
MogallForest_ClearActorSpriteFlags(struct ScriptInterpreter *interpreter) {
  struct FieldActor *actor = (struct FieldActor *)interpreter;
  Engine_ActorSetSpriteFlags(actor, 0);
  return 0;
}

/// Hops an actor to a pixel destination with the requested vertical speed.
static void MogallForest_HopActorTo(s32 actorId, s32 targetX, s32 targetZ,
                                    s32 verticalSpeed) {
  struct FieldActor *actor;

  actor = Object_GetById(actorId);
  Engine_ActorSetSpritePriority(actorId, 1);
  Actor_SetSpeed(actorId, PIXELS(3), PIXELS(3) / 2);
  Engine_AudioPlayCue(SE_JUMP);
  actor->velocityY = verticalSpeed;
  actor->gravity = PIXELS(1) / 2;
  actor->bounce = 0;
  Engine_ActorSetSpriteFlags(actor, 0);
  Actor_MoveToAndWait(actorId, targetX, targetZ);
  Actor_SetPosition(actorId, PIXELS(targetX), PIXELS(targetZ));
  Engine_ActorSetSpriteFlags(actor, 1);
  actor->gravity = PIXELS(1);
}

/// Scatters a complete ring of rising puffs around an actor.
static void MogallForest_SpawnPuffRing(s32 id) {
  struct Vector3 velocity;
  struct EffectOptions options;
  struct EffectOptions *effectOptions;
  struct Vector3 *emissionVelocity;
  struct FieldActor *actor;
  u32 directionIndex;
  s32 x;
  s32 z;

  actor = Object_GetById(id);
  Engine_AudioPlayCue(SE_188);
  effectOptions = &options;
  effectOptions->priority = 1;
  for (directionIndex = 0; directionIndex <= 16; directionIndex++) {
    emissionVelocity = &velocity;
    emissionVelocity->x = Engine_MathCos(directionIndex << 12);
    emissionVelocity->y = 0;
    z = Engine_MathSin(directionIndex << 12);
    x = emissionVelocity->x;
    emissionVelocity->z = z;
    x += x / 3;
    emissionVelocity->x = x;
    Effect_Spawn(actor->x.fixed, PIXELS(16), actor->z.fixed, x,
                 emissionVelocity->y + PIXELS(1) / 10, z, EFFECT_USE_PRIORITY,
                 effectOptions);
  }
}

/// The camera goes to the ape's hiding place, which bursts in a ring of
/// puffs, and the ape shows itself.
static void MogallForest_ApeComesOut(s32 actorId, s32 drawParameter) {
  s32 cameraSpeed = PIXELS(2);
  s32 cameraAcceleration = PIXELS(1) / 4;

  Engine_CameraSetSpeed(cameraSpeed, cameraAcceleration);
  Engine_CameraMoveToActor(actorId, 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(30);
  MogallForest_SpawnPuffRing(actorId);
  Engine_ActorSetChildValue(actorId, drawParameter);
}

/// The ape turns to the leader, hops and gestures down the way to go.
static inline void MogallForest_ApePointsTheWay(s32 guideId) {
  Engine_CameraFollowActor(guideId, 1);
  Engine_ActorFaceEachOther(guideId, ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(20);
  Engine_ActorStartRepeatedMotion(guideId, 2);
  Engine_ActorSetAttachedEffect(guideId, 258);
  Engine_EventWait(60);
}

const struct SceneEntrance *MogallForest_GetEntrances(void) {
  s16 selector = gGameState.scene;

  if (selector == (s32)&SceneId_MogallForest1) {
    return gMogallForestEntrances1;
  }
  if (selector == (s32)&SceneId_MogallForest2) {
    return gMogallForestEntrances2;
  }
  if (selector == (s32)&SceneId_MogallForest3) {
    return gMogallForestEntrances3;
  }
  return gMogallForestEntrancesOther;
}

const struct SceneRegion *MogallForest_GetRegions(void) { return NULL; }

/// The exits, shared by the three areas.
const u32 *MogallForest_GetExits(void) { return gMogallForest_Exits; }

const struct ScenePlacement *MogallForest_GetPlacements(void) {
  s16 selector = gGameState.scene;

  if (selector == (s32)&SceneId_MogallForest1) {
    return gMogallForestPlacements1;
  }
  if (selector == (s32)&SceneId_MogallForest2) {
    return gMogallForestPlacements2;
  }
  if (selector == (s32)&SceneId_MogallForest3) {
    return gMogallForestPlacements3;
  }
  return gMogallForestPlacementsOther;
}

/// Pushing a log in the first area (walking into trigger 20): the river log
/// pushed to row 26 rolls into the water and can be crossed.
static void MogallForest_FirstArea_PushLog(void) {
  struct StagedActorProbe probe;

  Engine_EventBegin();
  if (StagedActor_FindClearPosition(&probe) != 0) {
    SceneActor_MoveAndRedraw(probe);
    if (probe.actorSlot == ACTOR_FIRST_AREA_RIVER_LOG &&
        (probe.positionZ >> 20) == 26) {
      Engine_GameFlagSet(FLAG_FIRST_AREA_LOG_ROLLED);
      Engine_ActorSetAnimation(ACTOR_FIRST_AREA_RIVER_LOG, 3);
      Actor_SetSpeed(ACTOR_FIRST_AREA_RIVER_LOG, PIXELS(1) / 4, PIXELS(1) / 2);
      Engine_ActorSetDestinationOffset(ACTOR_FIRST_AREA_RIVER_LOG, 0, -16);
      Engine_EventWait(45);
      Engine_ActorSetAnimation(ACTOR_FIRST_AREA_RIVER_LOG, 8);
      Engine_AudioPlayCue(SE_240);
      Engine_ActorSetSpritePriority(ACTOR_FIRST_AREA_RIVER_LOG, 1);
      Object_GetById(ACTOR_FIRST_AREA_RIVER_LOG)->priorityFlags =
          ACTOR_PRIORITY_UNDERFOOT;
      Map_CopyCellAttributes(38, 27, 4, 2, 31, 25);
    }
  }
  Engine_EventEnd();
}

/// Walking north into trigger 25: the leader jumps north across the log.
static void MogallForest_FirstArea_JumpNorth(void) {
  struct FieldPosition position;
#if TBS_PATCH_1
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);

  position.x = (leader->x.fixed & 0xfff00000) + PIXELS(8);
  position.y = leader->y.fixed;
  position.z = (leader->z.fixed & 0xfff00000) - PIXELS(24);
#else
  position.x = Object_GetById(ACTOR_PARTY_LEADER)->x.fixed;
  position.y = Object_GetById(ACTOR_PARTY_LEADER)->y.fixed;
  position.z = Object_GetById(ACTOR_PARTY_LEADER)->z.fixed - PIXELS(32);
#endif
  MogallForest_TryJumpLeaderTo(&position);
}

/// Walking south into trigger 26: the leader jumps onto the log, which
/// bobs under the weight.
static void MogallForest_FirstArea_JumpSouth(void) {
  struct FieldPosition position;
  struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);
  u8 *motionFlags = &actor->motionFlags;
  s32 savedMotionFlags = *motionFlags;

#if TBS_PATCH_1
  position.x = (actor->x.fixed & 0xfff00000) + PIXELS(8);
  position.y = actor->y.fixed;
  position.z = (actor->z.fixed & 0xfff00000) + PIXELS(40);
#else
  position.x = Object_GetById(ACTOR_PARTY_LEADER)->x.fixed;
  position.y = Object_GetById(ACTOR_PARTY_LEADER)->y.fixed;
  position.z = Object_GetById(ACTOR_PARTY_LEADER)->z.fixed + PIXELS(32);
#endif
  if (MogallForest_TryJumpLeaderTo(&position) != 0) {
    Engine_EventBegin();
    *motionFlags = 0;
    Engine_ActorSetAnimation(ACTOR_FIRST_AREA_RIVER_LOG, 7);
    actor->y.fixed += -PIXELS(1);
    actor->terrainHeight += -PIXELS(1);
    Engine_TaskWait(2);
    actor->y.fixed += -PIXELS(1);
    actor->terrainHeight += -PIXELS(1);
    Engine_TaskWait(10);
    actor->y.fixed += PIXELS(1);
    actor->terrainHeight += PIXELS(1);
    Engine_TaskWait(4);
    actor->y.fixed += PIXELS(1);
    actor->terrainHeight += PIXELS(1);
    *motionFlags = savedMotionFlags;
    Engine_EventEnd();
  }
}

/// Searching the sign (trigger 27). "The broken sign reads: North: Fuchin
/// Temple / South: Mogall Prairie..."
static void MogallForest_ReadBrokenSign(void) {
  Engine_EventBegin();
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Engine_MessageShowCentered(MSGNO(MsgMogallForestBrokenSign), 1);
  Engine_EventEnd();
}

/// Force on the north hiding place in the first area: the ape comes out,
/// points the way and hops off south.
static void MogallForest_FirstArea_NorthApeShowsWay(void) {
  struct FieldActor *creature;

  creature = Object_GetById(ACTOR_FIRST_AREA_NORTH_APE);

  Engine_EventBegin();

  MogallForest_ApeComesOut(ACTOR_FIRST_AREA_NORTH_APE, 1);
  MogallForest_HopActorTo(ACTOR_FIRST_AREA_NORTH_APE, 88, 120, PIXELS(6));

  Effect_Spawn(creature->x.fixed, creature->y.fixed,
               creature->z.fixed + PIXELS(24), 0, 0, 0, 1, 0);

  MogallForest_ApePointsTheWay(ACTOR_FIRST_AREA_NORTH_APE);

  MogallForest_HopActorTo(ACTOR_FIRST_AREA_NORTH_APE, 88, 152, PIXELS(3));
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_FIRST_AREA_NORTH_APE, 0);
  Engine_EventWait(10);

  MogallForest_HopActorTo(ACTOR_FIRST_AREA_NORTH_APE, 120, 192, PIXELS(3));
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_FIRST_AREA_NORTH_APE, 0);
  Engine_EventWait(10);

  MogallForest_HopActorTo(ACTOR_FIRST_AREA_NORTH_APE, 120, 240, PIXELS(3));
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_FIRST_AREA_NORTH_APE, 0);
  Engine_EventWait(10);

  Engine_GameFlagSet(FLAG_FIRST_AREA_NORTH_APE_OUT);
  Engine_ActorSetPosition(ACTOR_FIRST_AREA_NORTH_HIDEOUT, 0, 0);
  Engine_ActorSetPosition(ACTOR_FIRST_AREA_NORTH_APE, 0, 0);

  Engine_EventEnd();
}

/// Force on the hostile ape's hiding place: it comes out angry, leaps at the
/// leader and the battle with the Ape begins (encounter table 53, slot 0).
static void MogallForest_FirstArea_ApeAttacks(void) {
  struct FieldActor *creature;

  creature = Object_GetById(ACTOR_FIRST_AREA_HOSTILE_APE);

  Engine_EventBegin();

  MogallForest_ApeComesOut(ACTOR_FIRST_AREA_HOSTILE_APE, 0);
  MogallForest_HopActorTo(ACTOR_FIRST_AREA_HOSTILE_APE, 408, 456, PIXELS(6));

  Effect_Spawn(creature->x.fixed, creature->y.fixed,
               creature->z.fixed + PIXELS(24), 0, 0, 0, 1, 0);

  Engine_CameraFollowActor(ACTOR_FIRST_AREA_HOSTILE_APE, 1);
  Engine_ActorFaceEachOther(ACTOR_FIRST_AREA_HOSTILE_APE, ACTOR_PARTY_LEADER,
                            0);
  Engine_EventWait(30);
  Engine_ActorStartRepeatedMotion(ACTOR_FIRST_AREA_HOSTILE_APE, 2);
  Engine_ActorShowEmote(ACTOR_FIRST_AREA_HOSTILE_APE, EMOTE_IN_FRONT | 3, 0);
  Engine_AudioPlayCue(SE_147);
  Engine_EventWait(60);

  MogallForest_HopActorTo(ACTOR_FIRST_AREA_HOSTILE_APE,
                          Object_GetById(ACTOR_PARTY_LEADER)->x.part.pixel,
                          Object_GetById(ACTOR_PARTY_LEADER)->z.part.pixel,
                          PIXELS(4));

  Engine_EventWait(10);
  Engine_GameFlagSet(FLAG_FIRST_AREA_HOSTILE_APE_OUT);
  Engine_ActorSetPosition(ACTOR_FIRST_AREA_HOSTILE_HIDEOUT, 0, 0);

  Battle_SetStart(3);

  Field_RequestBattleFromEncounterTable(53, 0);

  Engine_EventEnd();
}

/// Force on the east hiding place in the first area: the ape comes out,
/// points the way and hops off east.
static void MogallForest_FirstArea_EastApeShowsWay(void) {
  struct FieldActor *creature;

  creature = Object_GetById(ACTOR_FIRST_AREA_EAST_APE);

  Engine_EventBegin();

  MogallForest_ApeComesOut(ACTOR_FIRST_AREA_EAST_APE, 1);
  MogallForest_HopActorTo(ACTOR_FIRST_AREA_EAST_APE, 536, 344, PIXELS(7));

  Effect_Spawn(creature->x.fixed, creature->y.fixed,
               creature->z.fixed + PIXELS(16), 0, 0, 0, 1, 0);

  MogallForest_ApePointsTheWay(ACTOR_FIRST_AREA_EAST_APE);

  MogallForest_HopActorTo(ACTOR_FIRST_AREA_EAST_APE, 584, 344, PIXELS(3));
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_FIRST_AREA_EAST_APE, 0);
  Engine_EventWait(6);

  MogallForest_HopActorTo(ACTOR_FIRST_AREA_EAST_APE, 632, 344, PIXELS(3));
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_FIRST_AREA_EAST_APE, 0);
  Engine_EventWait(6);

  MogallForest_HopActorTo(ACTOR_FIRST_AREA_EAST_APE, 680, 344, PIXELS(3));
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_FIRST_AREA_EAST_APE, 0);
  Engine_EventWait(6);

  Engine_GameFlagSet(FLAG_FIRST_AREA_EAST_APE_OUT);
  Engine_ActorSetPosition(ACTOR_FIRST_AREA_EAST_HIDEOUT, 0, 0);

  Engine_EventEnd();
}

/// Catch on the Nut floating over the river: it is caught and found.
static void MogallForest_CatchNut(void) {
  Engine_EventBegin();
  Engine_ActorSetPosition(ACTOR_NUT, 0, 0);
  Engine_GameFlagSet(FLAG_NUT_CAUGHT);
  Engine_ItemShowFound(ITEM_NUT, 3);
  Engine_PartyGiveItem(ITEM_NUT, 0);
  Engine_EventEnd();
}

/// Pushing a log in the second area (walking into trigger 20): the log at row
/// 23 changes the ground beside it; the river log pushed to column 35 rolls
/// into the water.
static void MogallForest_SecondArea_PushLog(void) {
  struct StagedActorProbe probe;
  s32 destinationColumn;
  s32 destinationRow;
  s32 rectangleHeight;
  s32 attributes;

  Engine_EventBegin();

  if (StagedActor_FindClearPosition(&probe) != 0) {
    SceneActor_MoveAndRedraw(probe);

    if (probe.actorSlot == ACTOR_SECOND_AREA_LOG &&
        (probe.positionZ >> 20) == 23) {
      destinationColumn = 35;
      destinationRow = 68;
      Map_CopyCellAttributes(35, 67, 4, 1, destinationColumn, destinationRow);
    } else if (probe.actorSlot == ACTOR_SECOND_AREA_RIVER_LOG &&
               (probe.positionX >> 20) == 35) {

      attributes = 0;
      Engine_GameFlagSet(FLAG_SECOND_AREA_LOG_ROLLED);
      Engine_ActorSetAnimation(ACTOR_SECOND_AREA_RIVER_LOG, 3);
      Engine_ActorSetDestinationOffset(ACTOR_SECOND_AREA_RIVER_LOG, -16, 6);
      Engine_EventWait(30);
      Engine_ActorSetAnimation(ACTOR_SECOND_AREA_RIVER_LOG, 8);
      Engine_AudioPlayCue(SE_240);

      Object_GetById(ACTOR_SECOND_AREA_RIVER_LOG)->priorityFlags =
          ACTOR_PRIORITY_UNDERFOOT;

      destinationColumn = 34;
      destinationRow = 30;
      Map_CopyCellAttributes(44, 30, 2, 4, destinationColumn, destinationRow);
      rectangleHeight = 4;
      StagedActor_FillGridAttributeRectangle(2, 35, 30, 1, rectangleHeight,
                                             attributes);
    }
  }

  Engine_EventEnd();
}

/// Marks the obstacle cell and clears the cells it could have moved from.
static inline void MogallForest_UpdateObstacleFootprint(s32 column, s32 row) {
  StagedActor_FillGridAttributeRectangle(2, column, row, 1, 1, 255);
  StagedActor_FillGridAttributeRectangle(2, column + 1, row, 1, 1, 0);
  StagedActor_FillGridAttributeRectangle(2, column - 1, row, 1, 1, 0);
  StagedActor_FillGridAttributeRectangle(2, column, row + 1, 1, 1, 0);
  StagedActor_FillGridAttributeRectangle(2, column, row - 1, 1, 1, 0);
}

/// Move on the second area's obstacle: its footprint follows it, and at
/// (36, 24) it sinks two pixels into place.
static void MogallForest_SecondArea_UpdateMovableObstacle(void) {
  s32 column;
  s32 row;

  Engine_EventBegin();

  column = Object_GetById(ACTOR_SECOND_AREA_OBSTACLE)->x.fixed >> 20;
  row = Object_GetById(ACTOR_SECOND_AREA_OBSTACLE)->z.fixed >> 20;

  MogallForest_UpdateObstacleFootprint(column, row);

  if (column == 36 && row == 24) {
    struct FieldActor *obstacle = Object_GetById(ACTOR_SECOND_AREA_OBSTACLE);

    obstacle->motionFlags = 0;
    obstacle->terrainHeight = -PIXELS(2);
    obstacle->y.fixed = -PIXELS(2);
  }

  Engine_EventEnd();
}

/// Force on the north hiding place in the second area: the ape points the
/// way and hops off south.
static void MogallForest_SecondArea_NorthApeShowsWay(void) {
  struct FieldActor *creature;

  creature = Object_GetById(ACTOR_SECOND_AREA_NORTH_APE);
  Engine_EventBegin();
  MogallForest_ApeComesOut(ACTOR_SECOND_AREA_NORTH_APE, 1);
  MogallForest_HopActorTo(ACTOR_SECOND_AREA_NORTH_APE, 392, 104, PIXELS(7));
  Engine_EventWait(10);
  Effect_Spawn(creature->x.fixed, creature->y.fixed,
               (creature->z.fixed + PIXELS(4)), 0, 0, 0, 1, 0);
  MogallForest_ApePointsTheWay(ACTOR_SECOND_AREA_NORTH_APE);
  MogallForest_HopActorTo(ACTOR_SECOND_AREA_NORTH_APE, 424, 120, PIXELS(3));
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_SECOND_AREA_NORTH_APE, 0);
  Engine_EventWait(6);
  MogallForest_HopActorTo(ACTOR_SECOND_AREA_NORTH_APE, 424, 168, PIXELS(3));
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_SECOND_AREA_NORTH_APE, 0);
  Engine_EventWait(6);
  MogallForest_HopActorTo(ACTOR_SECOND_AREA_NORTH_APE, 424, 208, PIXELS(3));
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_SECOND_AREA_NORTH_APE, 0);
  Engine_EventWait(6);
  MogallForest_HopActorTo(ACTOR_SECOND_AREA_NORTH_APE, 424, 232, PIXELS(3));
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_SECOND_AREA_NORTH_APE, 0);
  Engine_EventWait(6);
  Engine_ActorSetPosition(ACTOR_SECOND_AREA_NORTH_APE, 0, 0);
  Engine_GameFlagSet(FLAG_SECOND_AREA_NORTH_APE_OUT);
  Engine_ActorSetPosition(ACTOR_SECOND_AREA_NORTH_HIDEOUT, 0, 0);
  Engine_EventEnd();
}

/// Force on the east hiding place in the second area: the ape points the way
/// and hops off east.
static void MogallForest_SecondArea_EastApeShowsWay(void) {
  struct FieldActor *creature;

  creature = Object_GetById(ACTOR_SECOND_AREA_EAST_APE);

  Engine_EventBegin();

  MogallForest_ApeComesOut(ACTOR_SECOND_AREA_EAST_APE, 1);
  MogallForest_HopActorTo(ACTOR_SECOND_AREA_EAST_APE, 456, 104, PIXELS(7));
  Engine_EventWait(10);

  Effect_Spawn(creature->x.fixed, creature->y.fixed,
               creature->z.fixed + PIXELS(4), 0, 0, 0, 1, 0);

  MogallForest_ApePointsTheWay(ACTOR_SECOND_AREA_EAST_APE);

  MogallForest_HopActorTo(ACTOR_SECOND_AREA_EAST_APE, 472, 136, PIXELS(3));
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_SECOND_AREA_EAST_APE, 0);
  Engine_EventWait(6);

  MogallForest_HopActorTo(ACTOR_SECOND_AREA_EAST_APE, 504, 136, PIXELS(16) / 5);
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_SECOND_AREA_EAST_APE, 0);
  Engine_EventWait(6);

  MogallForest_HopActorTo(ACTOR_SECOND_AREA_EAST_APE, 552, 136, PIXELS(7) / 2);
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_SECOND_AREA_EAST_APE, 0);
  Engine_EventWait(6);

  MogallForest_HopActorTo(ACTOR_SECOND_AREA_EAST_APE, 584, 136, PIXELS(7) / 2);
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_SECOND_AREA_EAST_APE, 0);
  Engine_EventWait(6);

  Engine_ActorSetPosition(ACTOR_SECOND_AREA_EAST_APE, 0, 0);
  Engine_GameFlagSet(FLAG_SECOND_AREA_EAST_APE_OUT);
  Engine_ActorSetPosition(ACTOR_SECOND_AREA_EAST_HIDEOUT, 0, 0);

  Engine_EventEnd();
}

/// Force on the south hiding place the first time: the ape bolts to another
/// hiding place nearby.
static void MogallForest_SecondArea_SouthApeHidesAgain(void) {
  struct FieldActor *creature;

  Engine_EventBegin();
  MogallForest_ApeComesOut(ACTOR_SECOND_AREA_SOUTH_APE, 1);
  MogallForest_HopActorTo(ACTOR_SECOND_AREA_SOUTH_APE, 424, 480,
                          PIXELS(38) / 5);
  Engine_EventWait(2);
  MogallForest_SpawnPuffRing(ACTOR_SECOND_AREA_SOUTH_APE);
  Actor_SetChildValue(ACTOR_SECOND_AREA_SOUTH_APE, 15);
  creature = Actor_Get(ACTOR_SECOND_AREA_SOUTH_APE);
  Engine_ActorSetSpriteFlags(creature, 0);
  Engine_EventWait(30);
  GameFlag_Set(FLAG_SECOND_AREA_SOUTH_APE_MOVED);
  Actor_SetPosition(ACTOR_SECOND_AREA_SOUTH_HIDEOUT, PIXELS(424), PIXELS(480));
  Engine_EventEnd();
}

/// Force on its new hiding place: the ape points the way and hops off west.
static void MogallForest_SecondArea_SouthApeShowsWay(void) {
  struct FieldActor *creature;

  creature = Object_GetById(ACTOR_SECOND_AREA_SOUTH_APE);

  Engine_EventBegin();

  MogallForest_ApeComesOut(ACTOR_SECOND_AREA_SOUTH_APE, 1);
  MogallForest_HopActorTo(ACTOR_SECOND_AREA_SOUTH_APE, 392, 504, PIXELS(6));
  Engine_EventWait(10);

  Effect_Spawn(creature->x.fixed, creature->y.fixed,
               creature->z.fixed + PIXELS(4), 0, 0, 0, 1, 0);

  MogallForest_ApePointsTheWay(ACTOR_SECOND_AREA_SOUTH_APE);

  MogallForest_HopActorTo(ACTOR_SECOND_AREA_SOUTH_APE, 360, 528, PIXELS(3));
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_SECOND_AREA_SOUTH_APE, 0);
  Engine_EventWait(6);

  MogallForest_HopActorTo(ACTOR_SECOND_AREA_SOUTH_APE, 328, 528, PIXELS(3));
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_SECOND_AREA_SOUTH_APE, 0);
  Engine_EventWait(6);

  MogallForest_HopActorTo(ACTOR_SECOND_AREA_SOUTH_APE, 288, 528, PIXELS(3));
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_SECOND_AREA_SOUTH_APE, 0);
  Engine_EventWait(6);

  MogallForest_HopActorTo(ACTOR_SECOND_AREA_SOUTH_APE, 256, 528, PIXELS(3));
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_SECOND_AREA_SOUTH_APE, 0);
  Engine_EventWait(6);

  Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 1);
  Engine_ActorSetPosition(ACTOR_SECOND_AREA_SOUTH_APE, 0, 0);
  Engine_EventWait(30);
  Engine_GameFlagSet(FLAG_SECOND_AREA_SOUTH_APE_OUT);
  Engine_ActorSetPosition(ACTOR_SECOND_AREA_SOUTH_HIDEOUT, 0, 0);

  Engine_EventEnd();
}

/// Pushing a log in the third area (walking into trigger 20): each river
/// log rolls into the water at its column (42 and 40) and can be crossed.
static void MogallForest_ThirdArea_PushLog(void) {
  struct StagedActorProbe probe;
  s32 landed;

  Engine_EventBegin();
  landed = 0;
  if (StagedActor_FindClearPosition(&probe) != 0) {
    SceneActor_MoveAndRedraw(probe);
    if (probe.actorSlot == ACTOR_THIRD_AREA_RIVER_LOG_1 ||
        probe.actorSlot == ACTOR_THIRD_AREA_RIVER_LOG_2) {
      if (probe.actorSlot == ACTOR_THIRD_AREA_RIVER_LOG_1) {
        Map_CopyCellAttributes(38, 68, 1, 4, probe.positionX >> 20, 68);
        if (probe.positionX >> 20 == 42) {
          Map_CopyCellAttributes(26, 20, 2, 4, probe.positionX >> 20, 23);
          Engine_ActorSetSpritePriority(ACTOR_THIRD_AREA_RIVER_LOG_1, 1);
          landed = 1;
          GameFlag_Set(FLAG_THIRD_AREA_FIRST_LOG_ROLLED);
        }
      } else {
        if (probe.positionX >> 20 == 40) {
          Map_CopyCellAttributes(26, 20, 2, 4, probe.positionX >> 20, 32);
          Engine_ActorSetSpritePriority(ACTOR_THIRD_AREA_RIVER_LOG_2, 1);
          landed = 1;
          GameFlag_Set(FLAG_THIRD_AREA_SECOND_LOG_ROLLED);
        }
      }
      if (landed == 0) {
        Engine_EventEnd();
        return;
      }
      Engine_ActorSetAnimation(probe.actorSlot, 3);
      Engine_ActorSetDestinationOffset(probe.actorSlot, 18, 6);
      Engine_EventWait(30);
      Engine_ActorSetAnimation(probe.actorSlot, 8);
      Engine_AudioPlayCue(SE_240);
      Actor_Get(probe.actorSlot)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    } else {
      if (probe.actorSlot == ACTOR_THIRD_AREA_LOG)
        Map_CopyCellAttributes(42, 49, 1, 4, probe.positionX >> 20, 49);
    }
  }
  Engine_EventEnd();
}

/// Walking west into trigger 25: the leader jumps onto the second river log,
/// which bobs under the weight.
static void MogallForest_ThirdArea_JumpWest(void) {
  struct FieldActor *leader;
  u8 *motionFlags;
  u8 savedMotionFlags;
  struct FieldPosition position;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  motionFlags = &leader->motionFlags;
  savedMotionFlags = *motionFlags;

  position.x = Object_GetById(ACTOR_PARTY_LEADER)->x.fixed - PIXELS(32);
  position.y = Object_GetById(ACTOR_PARTY_LEADER)->y.fixed;
  position.z = Object_GetById(ACTOR_PARTY_LEADER)->z.fixed;

  if (MogallForest_TryJumpLeaderTo(&position) != 0) {

    Engine_EventBegin();

    *motionFlags = 0;
    Engine_ActorSetAnimation(ACTOR_THIRD_AREA_RIVER_LOG_2, 7);

    leader->y.fixed += -PIXELS(1);
    leader->terrainHeight += -PIXELS(1);
    Engine_TaskWait(2);

    leader->y.fixed += -PIXELS(1);
    leader->terrainHeight += -PIXELS(1);
    Engine_TaskWait(10);

    leader->y.fixed += PIXELS(1);
    leader->terrainHeight += PIXELS(1);
    Engine_TaskWait(4);

    leader->y.fixed += PIXELS(1);
    leader->terrainHeight += PIXELS(1);

    *motionFlags = savedMotionFlags;
    Engine_EventEnd();
  }
}

/// Move on the third area's north obstacle: at (45, 6) it sinks into place.
static void MogallForest_ThirdArea_UpdateNorthObstacle(void) {
  s32 column;
  s32 row;

  Engine_EventBegin();

  column = Object_GetById(ACTOR_THIRD_AREA_NORTH_OBSTACLE)->x.fixed >> 20;
  row = Object_GetById(ACTOR_THIRD_AREA_NORTH_OBSTACLE)->z.fixed >> 20;

  MogallForest_UpdateObstacleFootprint(column, row);

  if (column == 45 && row == 6) {
    struct FieldActor *obstacle =
        Object_GetById(ACTOR_THIRD_AREA_NORTH_OBSTACLE);

    obstacle->motionFlags = 0;
    obstacle->terrainHeight = -PIXELS(2);
    obstacle->y.fixed = -PIXELS(2);
  }

  Engine_EventEnd();
}

/// Move on the south obstacle: on row 27 it sinks into place and stays.
static void MogallForest_ThirdArea_UpdateSouthObstacle(void) {

  s32 column;
  s32 row;
  struct FieldActor *obstacle;

  Engine_EventBegin();
  obstacle = Object_GetById(ACTOR_THIRD_AREA_SOUTH_OBSTACLE);
  column = obstacle->x.fixed;
  obstacle = Object_GetById(ACTOR_THIRD_AREA_SOUTH_OBSTACLE);
  row = obstacle->z.fixed;
  column >>= 20;
  row >>= 20;
  MogallForest_UpdateObstacleFootprint(column, row);
  obstacle = Object_GetById(ACTOR_THIRD_AREA_SOUTH_OBSTACLE);
  if ((obstacle->z.fixed >> 20) == 27) {
    obstacle = Object_GetById(ACTOR_THIRD_AREA_SOUTH_OBSTACLE);
    obstacle->motionFlags = 0;
    obstacle->terrainHeight = -PIXELS(2);
    obstacle->y.fixed = -PIXELS(2);
    Engine_GameFlagSet(FLAG_SOUTH_OBSTACLE_SETTLED);
    StagedActor_FillGridAttributeRectangle(2, 43, 23, 1, 1, 255);
  }
  Engine_EventEnd();
}

/// Force on the third area's hostile hiding place: the ape leaps at the
/// leader and the battle with the Ape begins.
static void MogallForest_ThirdArea_ApeAttacks(void) {

  struct FieldActor *creature;
  s32 jumpSpeed;
  struct FieldActor *leaderForX;
  s32 leaderX;
  struct FieldActor *leaderForZ;

  creature = Object_GetById(ACTOR_THIRD_AREA_HOSTILE_APE);
  jumpSpeed = PIXELS(8);
  Engine_EventBegin();
  MogallForest_ApeComesOut(ACTOR_THIRD_AREA_HOSTILE_APE, 0);
  MogallForest_HopActorTo(ACTOR_THIRD_AREA_HOSTILE_APE, 472, 104, jumpSpeed);
  Engine_EventWait(10);
  Effect_Spawn(creature->x.fixed, creature->y.fixed,
               (creature->z.fixed + jumpSpeed), 0, 0, 0, 1, 0);
  Engine_CameraFollowActor(ACTOR_THIRD_AREA_HOSTILE_APE, 1);
  Engine_ActorFaceEachOther(ACTOR_THIRD_AREA_HOSTILE_APE, ACTOR_PARTY_LEADER,
                            0);
  Engine_EventWait(30);
  Engine_ActorStartRepeatedMotion(ACTOR_THIRD_AREA_HOSTILE_APE, 2);
  Engine_ActorShowEmote(ACTOR_THIRD_AREA_HOSTILE_APE, EMOTE_IN_FRONT | 3, 0);
  Engine_AudioPlayCue(SE_147);
  Engine_EventWait(60);
  leaderForX = Object_GetById(ACTOR_PARTY_LEADER);
  leaderX = leaderForX->x.part.pixel;
  leaderForZ = Object_GetById(ACTOR_PARTY_LEADER);
  MogallForest_HopActorTo(ACTOR_THIRD_AREA_HOSTILE_APE, leaderX,
                          leaderForZ->z.part.pixel, PIXELS(6));
  Engine_EventWait(10);
  Engine_GameFlagSet(FLAG_THIRD_AREA_HOSTILE_APE_OUT);
  Battle_SetStart(3);
  Field_RequestBattleFromEncounterTable(53, 0);
  Engine_EventEnd();
}

/// Force on the central hiding place: the ape points the way and hops off
/// south.
static void MogallForest_ThirdArea_CentralApeShowsWay(void) {
  struct FieldActor *creature;

  creature = Object_GetById(ACTOR_THIRD_AREA_CENTRAL_APE);

  Engine_EventBegin();

  MogallForest_ApeComesOut(ACTOR_THIRD_AREA_CENTRAL_APE, 1);
  MogallForest_HopActorTo(ACTOR_THIRD_AREA_CENTRAL_APE, 456, 152, PIXELS(6));
  Engine_EventWait(10);

  Effect_Spawn(creature->x.fixed, creature->y.fixed,
               creature->z.fixed + PIXELS(4), 0, 0, 0, 1, 0);

  MogallForest_ApePointsTheWay(ACTOR_THIRD_AREA_CENTRAL_APE);

  MogallForest_HopActorTo(ACTOR_THIRD_AREA_CENTRAL_APE, 448, 192, PIXELS(3));
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_THIRD_AREA_CENTRAL_APE, 0);
  Engine_EventWait(6);

  MogallForest_HopActorTo(ACTOR_THIRD_AREA_CENTRAL_APE, 424, 208, PIXELS(3));
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_THIRD_AREA_CENTRAL_APE, 0);
  Engine_EventWait(6);

  MogallForest_HopActorTo(ACTOR_THIRD_AREA_CENTRAL_APE, 424, 224, PIXELS(3));
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_THIRD_AREA_CENTRAL_APE, 0);
  Engine_EventWait(6);

  Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 1);
  Engine_ActorSetPosition(ACTOR_THIRD_AREA_CENTRAL_APE, 0, 0);
  Engine_EventWait(30);
  Engine_GameFlagSet(FLAG_THIRD_AREA_CENTRAL_APE_OUT);
  Engine_ActorSetPosition(ACTOR_THIRD_AREA_CENTRAL_HIDEOUT, 0, 0);

  Engine_EventEnd();
}

/// Force on the west hiding place: the ape points the way and hops off west.
static void MogallForest_ThirdArea_WestApeShowsWay(void) {
  struct FieldActor *creature;

  creature = Object_GetById(ACTOR_THIRD_AREA_WEST_APE);

  Engine_EventBegin();

  MogallForest_ApeComesOut(ACTOR_THIRD_AREA_WEST_APE, 1);
  MogallForest_HopActorTo(ACTOR_THIRD_AREA_WEST_APE, 392, 104, PIXELS(6));
  Engine_EventWait(10);

  Effect_Spawn(creature->x.fixed, creature->y.fixed,
               creature->z.fixed + PIXELS(4), 0, 0, 0, 1, 0);

  MogallForest_ApePointsTheWay(ACTOR_THIRD_AREA_WEST_APE);

  MogallForest_HopActorTo(ACTOR_THIRD_AREA_WEST_APE, 376, 152, PIXELS(6));
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_THIRD_AREA_WEST_APE, 0);
  Engine_EventWait(10);

  MogallForest_HopActorTo(ACTOR_THIRD_AREA_WEST_APE, 328, 160, PIXELS(3));
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_THIRD_AREA_WEST_APE, 0);
  Engine_EventWait(6);

  MogallForest_HopActorTo(ACTOR_THIRD_AREA_WEST_APE, 296, 160, PIXELS(3));
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_THIRD_AREA_WEST_APE, 0);
  Engine_EventWait(6);

  Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 1);
  Engine_ActorSetPosition(ACTOR_THIRD_AREA_WEST_APE, 0, 0);
  Engine_EventWait(30);
  Engine_GameFlagSet(FLAG_THIRD_AREA_WEST_APE_OUT);
  Engine_ActorSetPosition(ACTOR_THIRD_AREA_WEST_HIDEOUT, 0, 0);

  Engine_EventEnd();
}

/// Force on the east hiding place the first time: the ape bolts to another
/// hiding place to the south.
static void MogallForest_ThirdArea_EastApeHidesAgain(void) {
  struct FieldActor *creature;

  Engine_EventBegin();
  MogallForest_ApeComesOut(ACTOR_THIRD_AREA_EAST_APE, 1);
  Camera_MoveTo(PIXELS(744), -1, PIXELS(504), 1);
  MogallForest_HopActorTo(ACTOR_THIRD_AREA_EAST_APE, 744, 504, PIXELS(9));
  MogallForest_SpawnPuffRing(ACTOR_THIRD_AREA_EAST_APE);
  Actor_SetChildValue(ACTOR_THIRD_AREA_EAST_APE, 15);
  creature = Actor_Get(ACTOR_THIRD_AREA_EAST_APE);
  Engine_ActorSetSpriteFlags(creature, 0);
  Engine_EventWait(30);
  GameFlag_Set(FLAG_THIRD_AREA_EAST_APE_MOVED);
  Actor_SetPosition(ACTOR_THIRD_AREA_EAST_HIDEOUT, PIXELS(744), PIXELS(504));
  Engine_EventEnd();
}

/// Force on its new hiding place: the ape points the way and hops off south.
static void MogallForest_ThirdArea_EastApeShowsWay(void) {
  struct FieldActor *creature;

  creature = Object_GetById(ACTOR_THIRD_AREA_EAST_APE);

  Engine_EventBegin();

  MogallForest_ApeComesOut(ACTOR_THIRD_AREA_EAST_APE, 1);
  MogallForest_HopActorTo(ACTOR_THIRD_AREA_EAST_APE, 712, 536, PIXELS(6));
  Engine_EventWait(10);

  Effect_Spawn(creature->x.fixed, creature->y.fixed,
               creature->z.fixed + PIXELS(4), 0, 0, 0, 1, 0);

  MogallForest_ApePointsTheWay(ACTOR_THIRD_AREA_EAST_APE);

  MogallForest_HopActorTo(ACTOR_THIRD_AREA_EAST_APE, 712, 568, PIXELS(6));
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_THIRD_AREA_EAST_APE, 0);
  Engine_EventWait(10);

  MogallForest_HopActorTo(ACTOR_THIRD_AREA_EAST_APE, 712, 600, PIXELS(3));
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_THIRD_AREA_EAST_APE, 0);
  Engine_EventWait(6);

  MogallForest_HopActorTo(ACTOR_THIRD_AREA_EAST_APE, 736, 640, PIXELS(3));
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_THIRD_AREA_EAST_APE, 0);
  Engine_EventWait(6);

  MogallForest_HopActorTo(ACTOR_THIRD_AREA_EAST_APE, 736, 704, PIXELS(3));
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_THIRD_AREA_EAST_APE, 0);
  Engine_EventWait(6);

  Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 1);
  Engine_ActorSetPosition(ACTOR_THIRD_AREA_EAST_APE, 0, 0);
  Engine_EventWait(30);
  Engine_GameFlagSet(FLAG_THIRD_AREA_EAST_APE_OUT);

  Engine_EventEnd();
}

/// Touching trigger 22 in the third area, once: the ape appears again,
/// turns to the party and leads it south to a hiding place.
static void MogallForest_ApeLeadsToKillerApe(void) {
  struct FieldActor *creature;

  creature = Object_GetById(ACTOR_THIRD_AREA_EAST_APE);
  Engine_EventBegin();
  Actor_SetPosition(ACTOR_THIRD_AREA_EAST_APE, PIXELS(136), PIXELS(360));
  MogallForest_ApeComesOut(ACTOR_THIRD_AREA_EAST_APE, 1);
  MogallForest_HopActorTo(ACTOR_THIRD_AREA_EAST_APE, 136, 408, PIXELS(8));
  Engine_EventWait(10);
  Effect_Spawn(creature->x.fixed, creature->y.fixed,
               (creature->z.fixed + PIXELS(4)), 0, 0, 0, 1, 0);
  Actor_FaceDirection(ACTOR_THIRD_AREA_EAST_APE, FACING_NORTH, 40);
  Actor_SetAttachedEffect(ACTOR_THIRD_AREA_EAST_APE, 0x102);
  Engine_ActorRunRepeatedMotion(ACTOR_THIRD_AREA_EAST_APE, 2);
  Engine_CameraFollowActor(ACTOR_THIRD_AREA_EAST_APE, 1);
  MogallForest_HopActorTo(ACTOR_THIRD_AREA_EAST_APE, 136, 440, PIXELS(6));
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_THIRD_AREA_EAST_APE, 0);
  Engine_EventWait(10);
  MogallForest_HopActorTo(ACTOR_THIRD_AREA_EAST_APE, 136, 472, PIXELS(3));
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_THIRD_AREA_EAST_APE, 0);
  Engine_EventWait(6);
  MogallForest_HopActorTo(ACTOR_THIRD_AREA_EAST_APE, 136, 504, PIXELS(3));
  Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_THIRD_AREA_EAST_APE, 0);
  Engine_EventWait(6);
  Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 1);
  Actor_SetPosition(ACTOR_THIRD_AREA_EAST_APE, 0, 0);
  Engine_EventWait(60);
  GameFlag_Set(FLAG_MOGALL_FOREST_LED_TO_KILLER_APE);
  Engine_EventEnd();
}

/// Touching trigger 23 until it is beaten: the hiding place shakes twice and
/// out comes the Killer Ape, a fifth again as big, to fight (encounter table
/// 53, slot 1); the party returns to entrance 15.
static void MogallForest_KillerApeAttacks(void) {
  struct Vector3 velocity;
  struct Vector3 *emissionVelocity;
  struct FieldActor *actor;
  u32 directionIndex;
  s32 x;
  s32 z;

  actor = Object_GetById(ACTOR_THIRD_AREA_EAST_APE);
  Engine_EventBegin();
  // Shake the hiding place twice before the creature appears.
  Engine_ActorSetChildValue(ACTOR_THIRD_AREA_EAST_APE, 15);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_THIRD_AREA_EAST_APE), 0);
  Actor_SetPosition(ACTOR_THIRD_AREA_EAST_APE, PIXELS(136), PIXELS(360));
  Engine_CameraSetSpeed(PIXELS(1) / 2, PIXELS(1) / 16);
  Camera_MoveTo(PIXELS(136), -1, PIXELS(392), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(60);
  Map_SetShake(PIXELS(5), PIXELS(5), Q16_ONE);
  MogallForest_SpawnPuffRing(ACTOR_THIRD_AREA_EAST_APE);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Engine_MapWaitWorkValuesBelow256();
  Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
  Engine_EventWait(40);
  Map_SetShake(PIXELS(5), PIXELS(5), Q16_ONE);
  MogallForest_SpawnPuffRing(ACTOR_THIRD_AREA_EAST_APE);
  Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Engine_MapWaitWorkValuesBelow256();
  Engine_EventWait(40);
  MogallForest_SpawnPuffRing(ACTOR_THIRD_AREA_EAST_APE);
  actor->scaleX = 6 * Q16_ONE / 5;
  actor->scaleY = 6 * Q16_ONE / 5;
  Engine_ActorSetChildValue(ACTOR_THIRD_AREA_EAST_APE, 5);
  MogallForest_HopActorTo(ACTOR_THIRD_AREA_EAST_APE, 136, 392, PIXELS(15));
  Engine_EventWait(15);
  // The larger reveal uses a wider, level ring of puffs.
  Map_SetShake(PIXELS(5), PIXELS(5), Q16_ONE);
  for (directionIndex = 0; directionIndex <= 16; directionIndex++) {
    emissionVelocity = &velocity;
    emissionVelocity->x = Engine_MathCos(directionIndex << 12);
    emissionVelocity->y = 0;
    z = Engine_MathSin(directionIndex << 12);
    x = emissionVelocity->x;
    emissionVelocity->z = z;
    x += x / 2;
    emissionVelocity->x = x;
    Effect_Spawn(actor->x.fixed, 0, actor->z.fixed, x, emissionVelocity->y, z,
                 1, NULL);
  }
  Engine_EventWait(30);
  Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Engine_MapWaitWorkValuesBelow256();
  Engine_AudioPlayCue(SE_148);
  Engine_ActorRunRepeatedMotion(ACTOR_THIRD_AREA_EAST_APE, 2);
  Engine_EventWait(20);
  // Jump toward the leader, then enter the final creature encounter.
  {
    s32 leaderX = Object_GetById(ACTOR_PARTY_LEADER)->x.part.pixel;

    MogallForest_HopActorTo(
        ACTOR_THIRD_AREA_EAST_APE, leaderX,
        Object_GetById(ACTOR_PARTY_LEADER)->z.part.pixel - 16, PIXELS(8));
  }
  Engine_EventWait(10);
  Battle_SetStart(3);
  Event_SetNextDestination((s32)&SceneId_MogallForest3, 15);
  Field_RequestBattleFromEncounterTable(53, 1);
  Engine_EventEnd();
}

const struct SceneEvent *MogallForest_GetEvents(void) {
  s16 selector = gGameState.scene;

  if (selector == (s32)&SceneId_MogallForest1) {
    return gMogallForestEvents1;
  }
  if (selector == (s32)&SceneId_MogallForest2) {
    return gMogallForestEvents2;
  }
  if (selector == (s32)&SceneId_MogallForest3) {
    return gMogallForestEvents3;
  }
  return gMogallForestEventsOther;
}

/// On entering an area: the first time, an ape peeks out and hides (the
/// first area); apes already out stay gone, rolled logs and moved obstacles
/// stay put; back from the Killer Ape (entrance 15) it is marked beaten.
s32 MogallForest_Initialize(void) {
  struct FieldActor *actor;
  s32 sceneId;
  s32 entrance;
  s32 column;
  s32 fixedZ;

  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  sceneId = gGameState.scene;
  if (sceneId == (s32)&SceneId_MogallForest1) {
    // Show the first guide once, or restore the first area's completed
    // crossing.
    entrance = gGameState.entrance;
    switch (entrance) {
    case 1:
    case 2:
    case 3:
    case 4:
      if (GameFlag_IsSet(FLAG_MOGALL_FOREST_FIRST_APE_SEEN) == 0) {
        Engine_EventBegin();
        Engine_TaskWait(1);
        Engine_ActorSetChildValue(ACTOR_FIRST_AREA_NORTH_APE, 1);
        Actor_SetPosition(ACTOR_FIRST_AREA_NORTH_APE, PIXELS(92), PIXELS(120));
        Actor_FaceDirection(ACTOR_FIRST_AREA_NORTH_APE,
                            FACING_NORTH + FACING_STEP, 0);
        Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(2) / 5, PIXELS(1) / 5);
        Engine_ActorWalkTo(ACTOR_PARTY_LEADER, 136, 64);
        Engine_EventOpenScreen();
        Engine_EventWaitForScreen();
        Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
        Engine_EventWait(30);
        Actor_ShowEmote(ACTOR_FIRST_AREA_NORTH_APE, EMOTE_IN_FRONT, 0);
        Engine_ActorRunRepeatedMotion(ACTOR_FIRST_AREA_NORTH_APE, 2);
        Engine_EventWait(30);
        MogallForest_HopActorTo(ACTOR_FIRST_AREA_NORTH_APE, 136, 116,
                                PIXELS(7));
        MogallForest_SpawnPuffRing(ACTOR_FIRST_AREA_NORTH_APE);
        Engine_ActorSetChildValue(ACTOR_FIRST_AREA_NORTH_APE, 15);
        Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_FIRST_AREA_NORTH_APE),
                                   0);
        Engine_GameFlagSet(FLAG_MOGALL_FOREST_FIRST_APE_SEEN);
        Engine_EventWait(60);
        Engine_EventEnd();
      }
      if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
        break;
      }
      if (Engine_GameFlagIsSet(FLAG_FIRST_AREA_NORTH_APE_OUT) != 0) {
        break;
      }
      Engine_ActorSetChildValue(ACTOR_FIRST_AREA_NORTH_APE, 15);
      Actor_SetPosition(ACTOR_FIRST_AREA_NORTH_APE, PIXELS(136), PIXELS(116));
      break;

    case 7:
    case 8:
    case 9:
      actor = Object_GetById(ACTOR_PARTY_LEADER);
      if (actor != 0) {
        Engine_ActorSetPosition(ACTOR_NUT, actor->x.fixed, actor->z.fixed);
      }
      actor = Object_GetById(ACTOR_NUT);
      actor->update = 0;
      if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) != 0) {
        actor = Object_GetById(ACTOR_NUT);
        actor->y.fixed = PIXELS(32);
      }
      Engine_TaskWait(1);
      Engine_ActorSetPosition(ACTOR_NUT, PIXELS(632), PIXELS(440));
      if (Engine_GameFlagIsSet(FLAG_NUT_CAUGHT) == 0) {
        Scene_FloatNut(ACTOR_NUT);
      }
      Engine_ActorSetChildValue(ACTOR_FIRST_AREA_HOSTILE_APE, 15);
      Engine_ActorSetChildValue(ACTOR_FIRST_AREA_EAST_APE, 15);
      Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_FIRST_AREA_HOSTILE_APE),
                                 0);
      Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_FIRST_AREA_EAST_APE), 0);
      FieldScene_RedrawActorFootprint(8);
      if (Engine_GameFlagIsSet(FLAG_FIRST_AREA_LOG_ROLLED) == 0) {
        FieldScene_RedrawActorFootprint(ACTOR_FIRST_AREA_RIVER_LOG);
        break;
      }
      Engine_TaskWait(1);
      Actor_SetPosition(ACTOR_FIRST_AREA_RIVER_LOG, PIXELS(528), PIXELS(408));
      Engine_ActorSetAnimation(ACTOR_FIRST_AREA_RIVER_LOG, 4);
      Map_CopyCellAttributes(38, 27, 4, 2, 31, 25);
      Object_GetById(ACTOR_FIRST_AREA_RIVER_LOG)->priorityFlags =
          ACTOR_PRIORITY_UNDERFOOT;
      break;
    }
  } else if (sceneId == (s32)&SceneId_MogallForest2) {
    // Restore revealed guides, moved obstacles and the second crossing.
    entrance = gGameState.entrance;
    switch (entrance) {
    case 3:
    case 4:
    case 5:
    case 6:
      if (Engine_GameFlagIsSet(FLAG_SECOND_AREA_NORTH_APE_OUT) == 0) {
        Engine_ActorSetChildValue(ACTOR_SECOND_AREA_NORTH_APE, 15);
        Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_SECOND_AREA_NORTH_APE),
                                   0);
      }
      if (Engine_GameFlagIsSet(FLAG_SECOND_AREA_EAST_APE_OUT) != 0) {
        break;
      }
      Engine_ActorSetChildValue(ACTOR_SECOND_AREA_EAST_APE, 15);
      Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_SECOND_AREA_EAST_APE), 0);
      break;

    case 10:
    case 11:
    case 12:
      if (Engine_GameFlagIsSet(FLAG_SECOND_AREA_LOG_ROLLED) == 0) {
        FieldScene_RedrawActorFootprint(ACTOR_SECOND_AREA_RIVER_LOG);
      } else {
        s32 attr = 0;

        Engine_TaskWait(1);
        Actor_SetPosition(ACTOR_SECOND_AREA_RIVER_LOG, PIXELS(552),
                          PIXELS(510));
        Engine_ActorSetAnimation(ACTOR_SECOND_AREA_RIVER_LOG, 4);
        Object_GetById(ACTOR_SECOND_AREA_RIVER_LOG)->priorityFlags =
            ACTOR_PRIORITY_UNDERFOOT;
        Map_CopyCellAttributes(44, 30, 2, 4, 34, 30);
#if TBS_PATCH_1
        StagedActor_FillGridAttributeRectangle(ACTOR_PARTY_LEADER, 35, 29, 1, 4,
                                               attr);
#endif
      }
      FieldScene_RedrawActorFootprint(ACTOR_SECOND_AREA_LOG);
      FieldScene_RedrawActorFootprint(9);
      column = Object_GetById(ACTOR_SECOND_AREA_OBSTACLE)->x.fixed;
      fixedZ = Object_GetById(ACTOR_SECOND_AREA_OBSTACLE)->z.fixed;
      column >>= 20;
      StagedActor_FillGridAttributeRectangle(2, column, fixedZ >> 20, 1, 1,
                                             255);
      Engine_TaskWait(1);
      Engine_ActorSetChildValue(ACTOR_SECOND_AREA_OBSTACLE, 6);
      Object_GetById(ACTOR_SECOND_AREA_LOG)->collisionFlags |= 8;
      if (Engine_GameFlagIsSet(FLAG_SECOND_AREA_SOUTH_APE_OUT) != 0) {
        break;
      }
      Engine_ActorSetChildValue(ACTOR_SECOND_AREA_SOUTH_APE, 15);
      Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_SECOND_AREA_SOUTH_APE),
                                 0);
      if (Engine_GameFlagIsSet(FLAG_SECOND_AREA_SOUTH_APE_MOVED) == 0) {
        break;
      }
      Actor_SetPosition(ACTOR_SECOND_AREA_SOUTH_APE, PIXELS(424), PIXELS(480));
      Actor_SetPosition(ACTOR_SECOND_AREA_SOUTH_HIDEOUT, PIXELS(424),
                        PIXELS(480));
      break;
    }
  } else if (sceneId == (s32)&SceneId_MogallForest3) {
    // Restore both crossing logs and the creatures leading to the final
    // encounter.
    entrance = gGameState.entrance;
    switch (entrance) {
    case 3:
    case 4:
    case 5:
    case 6:
      Engine_TaskWait(1);
      if (Engine_GameFlagIsSet(FLAG_THIRD_AREA_HOSTILE_APE_OUT) == 0) {
        Engine_ActorSetChildValue(ACTOR_THIRD_AREA_HOSTILE_APE, 15);
        Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_THIRD_AREA_HOSTILE_APE),
                                   0);
        Engine_ActorSetSpriteFlags(
            Object_GetById(ACTOR_THIRD_AREA_HOSTILE_HIDEOUT), 0);
      }
      if (Engine_GameFlagIsSet(FLAG_THIRD_AREA_CENTRAL_APE_OUT) == 0) {
        Engine_ActorSetChildValue(ACTOR_THIRD_AREA_CENTRAL_APE, 15);
        Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_THIRD_AREA_CENTRAL_APE),
                                   0);
        Engine_ActorSetSpriteFlags(
            Object_GetById(ACTOR_THIRD_AREA_CENTRAL_HIDEOUT), 0);
      }
      if (Engine_GameFlagIsSet(FLAG_THIRD_AREA_WEST_APE_OUT) != 0) {
        break;
      }
      Engine_ActorSetChildValue(ACTOR_THIRD_AREA_WEST_APE, 15);
      Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_THIRD_AREA_WEST_APE), 0);
      Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_THIRD_AREA_WEST_HIDEOUT),
                                 0);
      break;

    case 7:
      column = Object_GetById(ACTOR_THIRD_AREA_NORTH_OBSTACLE)->x.fixed;
      fixedZ = Object_GetById(ACTOR_THIRD_AREA_NORTH_OBSTACLE)->z.fixed;
      column >>= 20;
      StagedActor_FillGridAttributeRectangle(2, column, fixedZ >> 20, 1, 1,
                                             255);
      Engine_ActorSetChildValue(ACTOR_THIRD_AREA_NORTH_OBSTACLE, 6);
      Engine_TaskWait(1);
      Object_GetById(ACTOR_THIRD_AREA_LOG)->collisionFlags |= 8;
      FieldScene_RedrawActorFootprint(ACTOR_THIRD_AREA_LOG);
      break;

    case 8:
    case 9:
    case 10:
    case 11:
      OverlayObject_CreateConfiguredObject(PIXELS(734), 0, PIXELS(370), 223);
      OverlayObject_CreateConfiguredObject(PIXELS(754), 0, PIXELS(370), 223);
      FieldScene_RedrawActorFootprint(10);
      FieldScene_RedrawActorFootprint(12);
      if (Engine_GameFlagIsSet(FLAG_THIRD_AREA_FIRST_LOG_ROLLED) == 0) {
        FieldScene_RedrawActorFootprint(ACTOR_THIRD_AREA_RIVER_LOG_1);
      } else {
        Engine_TaskWait(1);
        Engine_ActorSetAnimation(ACTOR_THIRD_AREA_RIVER_LOG_1, 4);
        Engine_ActorSetPosition(ACTOR_THIRD_AREA_RIVER_LOG_1, PIXELS(698),
                                PIXELS(398));
        Object_GetById(ACTOR_THIRD_AREA_RIVER_LOG_1)->priorityFlags |=
            ACTOR_PRIORITY_UNDERFOOT;
        Map_CopyCellAttributes(26, 20, 2, 4, 42, 23);
        Engine_GameFlagSet(FLAG_SOUTH_OBSTACLE_SETTLED);
        Actor_SetPosition(ACTOR_THIRD_AREA_SOUTH_OBSTACLE, PIXELS(632),
                          PIXELS(440));
        Engine_ActorSetSpriteFlags(
            Object_GetById(ACTOR_THIRD_AREA_SOUTH_OBSTACLE), 0);
      }
      if (Engine_GameFlagIsSet(FLAG_THIRD_AREA_SECOND_LOG_ROLLED) == 0) {
        FieldScene_RedrawActorFootprint(ACTOR_THIRD_AREA_RIVER_LOG_2);
      } else {
        Engine_TaskWait(1);
        Engine_ActorSetAnimation(ACTOR_THIRD_AREA_RIVER_LOG_2, 4);
        Engine_ActorSetPosition(ACTOR_THIRD_AREA_RIVER_LOG_2, PIXELS(666),
                                PIXELS(550));
        Object_GetById(ACTOR_THIRD_AREA_RIVER_LOG_2)->priorityFlags =
            ACTOR_PRIORITY_UNDERFOOT;
        Map_CopyCellAttributes(26, 20, 2, 4, 40, 32);
      }
      column = Object_GetById(ACTOR_THIRD_AREA_SOUTH_OBSTACLE)->x.fixed;
      fixedZ = Object_GetById(ACTOR_THIRD_AREA_SOUTH_OBSTACLE)->z.fixed;
      column >>= 20;
      StagedActor_FillGridAttributeRectangle(2, column, fixedZ >> 20, 1, 1,
                                             255);
      Engine_ActorSetChildValue(ACTOR_THIRD_AREA_SOUTH_OBSTACLE, 6);
      Engine_TaskWait(1);
      Object_GetById(ACTOR_THIRD_AREA_RIVER_LOG_1)->collisionFlags |= 8;
      if (Engine_GameFlagIsSet(FLAG_THIRD_AREA_EAST_APE_OUT) == 0) {
        Engine_ActorSetChildValue(ACTOR_THIRD_AREA_EAST_APE, 15);
        Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_THIRD_AREA_EAST_APE),
                                   0);
        if (Engine_GameFlagIsSet(FLAG_THIRD_AREA_EAST_APE_MOVED) != 0) {
          Actor_SetPosition(ACTOR_THIRD_AREA_EAST_HIDEOUT, PIXELS(744),
                            PIXELS(504));
          Actor_SetPosition(ACTOR_THIRD_AREA_EAST_APE, PIXELS(744),
                            PIXELS(504));
        }
#if TBS_PATCH_2
      } else {
        Engine_ActorSetPosition(ACTOR_THIRD_AREA_EAST_APE, 0, 0);
#endif
      }
      MogallForest_ThirdArea_UpdateSouthObstacle();
      break;

    case 12:
    case 13:
      Actor_SetActionScript(ACTOR_THIRD_AREA_EAST_APE,
                            gMogallForest_ActorScript);
      if (Engine_GameFlagIsSet(FLAG_NYUNPA_TAUGHT_FORCE) == 0) {
        break;
      }
      if (Engine_GameFlagIsSet(FLAG_MOGALL_FOREST_KILLER_APE_BEATEN) == 0) {
        break;
      }
      Engine_GameFlagSet(FLAG_NYUNPA_ENLIGHTENED);
      break;

    case 15:
      Engine_GameFlagSet(FLAG_MOGALL_FOREST_KILLER_APE_BEATEN);
      break;
    }
  }
  return 0;
}

#include "common/object_orbit.h"

/// Pixel step by direction: X in the upper halfword, Z in the lower.
#define DIRECTION_STEP(dx, dz) (((u32)(dx) << 16) | ((dz) & 0xffff))

s32 gStagedActor_DirectionSteps[] = {
    DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(16, 0),
};

// clang-format off
u32 gStagedActor_FootprintKinds[] = {
    207, 205, 228, 229, 298, 297,
};

s32 gStagedActor_FootprintBounds[] = {
    -32, -8, 32, 8,
    -8, -32, 8, 32,
    -32, -16, 32, 0,
    -8, -32, 8, 32,
    -32, -8, 32, 8,
    -8, -32, 8, 32,
};

s32 gEffectScript0[] = {
    SC_WAIT(1),
    SC_LOOP(22, SC_TOP),
    SC_WAIT(1),
    SC_POS(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
};

s32 gEffectScript1[] = {
    SC_WAIT(1),
    SC_LOOP(44, SC_TOP),
    SC_WAIT(1),
    SC_POS(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
};

s32 gEffectScript2[] = {
    SC_WAIT(1),
    SC_LOOP(126, SC_TOP),
    SC_WAIT(1),
    SC_POS(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
};

const s32 *gEffectScripts[] = {
    gEffectScript0,
    gEffectScript1,
    gEffectScript2,
};

s32 gMogallForestPlacements2_0[] = {
    SC_ARG(2),
    SC_SET(0x23, 6),
    SC_CALL(MogallForest_CopyTimerToChild),
    SC_END,
};

s32 gMogallForest_ActorScript[] = {
    SC_SET(SW_SCALE_X, 4 * Q16_ONE / 5),
    SC_SET(SW_SCALE_Y, 4 * Q16_ONE / 5),
    SC_SET(0x23, 15),
    SC_CALL(MogallForest_CopyTimerToChild),
    SC_CALL(MogallForest_ClearActorSpriteFlags),
    SC_END,
};

s32 gMogallForestPlacements1_1[] = {
    SC_SET(SW_MOTION, 0),
    SC_SET(SW_Y, 0),
    SC_SET(SW_TARGET_Y, PIXELS(-32768)),
    SC_CALL(MogallForest_ClearActorSpriteFlags),
    SC_WAIT(2),
    SC_END,
};

struct SceneEntrance gMogallForestEntrances1[] = {
    { 1, CONDITION_ALWAYS, 136, 0, 24, FACING_SOUTH, 0, 0, 0, 272, 224, 0 },
    { 2, CONDITION_ALWAYS, 24, 0, 128, FACING_EAST, 0, 0, 0, 272, 224, 0 },
    { 3, CONDITION_ALWAYS, 248, 0, 128, FACING_WEST, 0, 0, 0, 272, 224, 0 },
    { 4, CONDITION_ALWAYS, 136, 0, 200, FACING_NORTH, 0, 0, 0, 272, 224, 0 },
    { 5, CONDITION_ALWAYS, 72, 0, 264, FACING_SOUTH, 0, 0, 240, 272, 464, 0 },
    { 6, CONDITION_ALWAYS, 200, 0, 440, FACING_NORTH, 0, 0, 240, 272, 464, 0 },
    { 7, CONDITION_ALWAYS, 416, 0, 264, FACING_SOUTH, 0, 288, 240, 672, 624, 0 },
    { 8, CONDITION_ALWAYS, 560, 0, 264, FACING_SOUTH, 0, 288, 240, 672, 624, 0 },
    { 9, CONDITION_ALWAYS, 648, 0, 344, FACING_WEST, 0, 288, 240, 672, 624, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gMogallForestEntrances2[] = {
    { 0, CONDITION_ALWAYS, 120, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { 1, CONDITION_ALWAYS, 24, 0, 128, FACING_EAST, 0, 0, 0, 272, 224, 0 },
    { 2, CONDITION_ALWAYS, 248, 0, 128, FACING_WEST, 0, 0, 0, 272, 224, 0 },
    { 3, CONDITION_ALWAYS, 424, 0, 24, FACING_SOUTH, 0, 288, 0, 560, 224, 0 },
    { 4, CONDITION_ALWAYS, 312, 0, 136, FACING_EAST, 0, 288, 0, 560, 224, 0 },
    { 5, CONDITION_ALWAYS, 536, 0, 120, FACING_WEST, 0, 288, 0, 560, 224, 0 },
    { 6, CONDITION_ALWAYS, 424, 0, 200, FACING_NORTH, 0, 288, 0, 560, 224, 0 },
    { 7, CONDITION_ALWAYS, 744, 0, 24, FACING_SOUTH, 0, 576, 0, 848, 224, 0 },
    { 8, CONDITION_ALWAYS, 744, 0, 200, FACING_NORTH, 0, 576, 0, 848, 224, 0 },
    { 9, CONDITION_ALWAYS, 24, 0, 368, FACING_EAST, 0, 0, 240, 272, 464, 0 },
    { 10, CONDITION_ALWAYS, 488, 0, 264, FACING_SOUTH, 0, 288, 240, 736, 624, 0 },
    { 11, CONDITION_ALWAYS, 312, 0, 528, FACING_EAST, 0, 288, 240, 736, 624, 0 },
    { 12, CONDITION_ALWAYS, 360, 0, 600, FACING_NORTH, 0, 288, 240, 736, 624, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gMogallForestEntrances3[] = {
    { 0, CONDITION_ALWAYS, 172, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { 1, CONDITION_ALWAYS, 24, 0, 128, FACING_EAST, 0, 0, 0, 272, 224, 0 },
    { 2, CONDITION_ALWAYS, 248, 0, 128, FACING_WEST, 0, 0, 0, 272, 224, 0 },
    { 3, CONDITION_ALWAYS, 424, 0, 24, FACING_SOUTH, 0, 288, 0, 560, 224, 0 },
    { 4, CONDITION_ALWAYS, 312, 0, 160, FACING_EAST, 0, 288, 0, 560, 224, 0 },
    { 5, CONDITION_ALWAYS, 536, 0, 160, FACING_WEST, 0, 288, 0, 560, 224, 0 },
    { 6, CONDITION_ALWAYS, 424, 0, 200, FACING_NORTH, 0, 288, 0, 560, 224, 0 },
    { 7, CONDITION_ALWAYS, 824, 0, 136, FACING_WEST, 0, 576, 0, 848, 224, 0 },
    { 8, CONDITION_ALWAYS, 728, 0, 264, FACING_SOUTH, 0, 400, 240, 848, 640, 0 },
    { 9, CONDITION_ALWAYS, 824, 0, 512, FACING_WEST, 0, 400, 240, 848, 640, 0 },
    { 10, CONDITION_ALWAYS, 824, 0, 608, FACING_WEST, 0, 400, 240, 848, 640, 0 },
    { 11, CONDITION_ALWAYS, 736, 0, 616, FACING_NORTH, 0, 400, 240, 848, 640, 0 },
    { 12, CONDITION_ALWAYS, 264, 0, 264, FACING_SOUTH, 0, 0, 240, 384, 496, 0 },
    { 13, CONDITION_ALWAYS, 136, 0, 472, FACING_NORTH, 0, 0, 240, 384, 496, 0 },
    { 15, CONDITION_ALWAYS, 136, 0, 432, FACING_NORTH, 0, 0, 240, 384, 496, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gMogallForestEntrancesOther[] = {
    { 1, CONDITION_ALWAYS, 312, 0, 128, FACING_EAST, 0, 288, 0, 560, 224, 0 },
    { 2, CONDITION_ALWAYS, 248, 0, 368, FACING_WEST, 0, 0, 240, 272, 464, 0 },
    { 3, CONDITION_ALWAYS, 424, 0, 440, FACING_NORTH, 0, 288, 240, 560, 464, 0 },
    { 4, CONDITION_ALWAYS, 712, 0, 264, FACING_SOUTH, 0, 576, 240, 848, 464, 0 },
    { 5, CONDITION_ALWAYS, 136, 0, 680, FACING_NORTH, 0, 0, 480, 272, 704, 0 },
    { 6, CONDITION_ALWAYS, 312, 0, 552, FACING_EAST, 0, 288, 480, 560, 704, 0 },
    { 7, CONDITION_ALWAYS, 312, 0, 648, FACING_EAST, 0, 288, 480, 560, 704, 0 },
    { 8, CONDITION_ALWAYS, 648, 0, 680, FACING_NORTH, 0, 576, 480, 848, 704, 0 },
    { 9, CONDITION_ALWAYS, 536, 0, 128, FACING_WEST, 0, 288, 0, 560, 224, 0 },
    { 10, CONDITION_ALWAYS, 24, 0, 368, FACING_EAST, 0, 0, 240, 272, 464, 0 },
    { SCENE_TABLE_END },
};

u32 gMogallForest_Exits[] = {
    SCENE_EXITS(68),
    SCENE_EXIT(1, 2, 13),
    SCENE_EXIT(2, 71, 2),
    SCENE_EXIT(3, 71, 1),
    SCENE_EXIT(4, 68, 5),
    SCENE_EXIT(5, 68, 4),
    SCENE_EXIT(6, 68, 7),
    SCENE_EXIT(7, 68, 6),
    SCENE_EXIT(8, 71, 8),
    SCENE_EXIT(9, 69, 1),
    SCENE_EXIT(0, 69, 0),
    SCENE_EXIT(1, 68, 9),
    SCENE_EXIT(2, 69, 4),
    SCENE_EXIT(3, 71, 3),
    SCENE_EXIT(4, 69, 2),
    SCENE_EXIT(5, 69, 9),
    SCENE_EXIT(6, 69, 7),
    SCENE_EXIT(7, 69, 6),
    SCENE_EXIT(8, 69, 10),
    SCENE_EXIT(9, 69, 5),
    SCENE_EXIT(10, 69, 8),
    SCENE_EXIT(11, 70, 2),
    SCENE_EXIT(12, 71, 4),
    SCENE_EXIT(0, 70, 0),
    SCENE_EXIT(1, 70, 5),
    SCENE_EXIT(2, 69, 11),
    SCENE_EXIT(3, 71, 5),
    SCENE_EXIT(4, 70, 7),
    SCENE_EXIT(5, 70, 1),
    SCENE_EXIT(6, 70, 8),
    SCENE_EXIT(7, 70, 4),
    SCENE_EXIT(8, 70, 6),
    SCENE_EXIT(9, 71, 6),
    SCENE_EXIT(10, 71, 7),
    SCENE_EXIT(11, 70, 12),
    SCENE_EXIT(12, 70, 11),
    SCENE_EXIT(13, 2, 43),
    SCENE_EXIT(0, 71, 0),
    SCENE_EXIT(1, 68, 3),
    SCENE_EXIT(2, 68, 2),
    SCENE_EXIT(3, 69, 3),
    SCENE_EXIT(4, 69, 12),
    SCENE_EXIT(5, 70, 3),
    SCENE_EXIT(6, 70, 9),
    SCENE_EXIT(7, 70, 10),
    SCENE_EXIT(8, 68, 8),
    SCENE_EXIT(11, 71, 1),
    SCENE_EXIT(12, 71, 2),
    SCENE_EXIT(13, 71, 3),
    SCENE_EXIT(14, 71, 4),
    SCENE_EXIT(15, 71, 5),
    SCENE_EXIT(16, 71, 6),
    SCENE_EXIT(17, 71, 7),
    SCENE_EXIT(18, 71, 8),
    SCENE_EXIT(19, 71, 9),
    SCENE_EXIT(20, 71, 10),
    SCENE_EXITS_END,
};

struct ScenePlacement gMogallForestPlacements1[] = {
    { 0xcd, CONDITION_ALWAYS, 7,
        PIXELS(488), 0, PIXELS(464), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xcf, CONDITION_ALWAYS, 7,
        PIXELS(528), 0, PIXELS(504), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xbf, FLAG_FIRST_AREA_NORTH_APE_OUT, (s32)gMogallForest_ActorScript,
        PIXELS(136), 0, PIXELS(120), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xbf, FLAG_FIRST_AREA_HOSTILE_APE_OUT, (s32)gMogallForest_ActorScript,
        PIXELS(376), 0, PIXELS(432), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xbf, FLAG_FIRST_AREA_EAST_APE_OUT, (s32)gMogallForest_ActorScript,
        PIXELS(504), 0, PIXELS(312), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x16, FLAG_FIRST_AREA_NORTH_APE_OUT, (s32)gMogallForestPlacements1_1,
        PIXELS(136), 0, PIXELS(120), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x16, FLAG_FIRST_AREA_HOSTILE_APE_OUT, (s32)gMogallForestPlacements1_1,
        PIXELS(376), 0, PIXELS(432), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x16, FLAG_FIRST_AREA_EAST_APE_OUT, (s32)gMogallForestPlacements1_1,
        PIXELS(504), 0, PIXELS(312), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x16, FLAG_NUT_CAUGHT, 7,
        PIXELS(632), 0, PIXELS(440), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gMogallForestPlacements2[] = {
    { 0xcf, CONDITION_ALWAYS, 7,
        PIXELS(592), 0, PIXELS(360), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xcf, CONDITION_ALWAYS, 7,
        PIXELS(592), 0, PIXELS(456), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xcd, CONDITION_ALWAYS, 7,
        PIXELS(648), 0, PIXELS(512), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x125, CONDITION_ALWAYS, (s32)gMogallForestPlacements2_0,
        PIXELS(584), 0, PIXELS(376), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xbf, FLAG_SECOND_AREA_NORTH_APE_OUT, (s32)gMogallForest_ActorScript,
        PIXELS(360), 0, PIXELS(80), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xbf, FLAG_SECOND_AREA_EAST_APE_OUT, (s32)gMogallForest_ActorScript,
        PIXELS(488), 0, PIXELS(80), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xbf, FLAG_SECOND_AREA_SOUTH_APE_OUT, (s32)gMogallForest_ActorScript,
        PIXELS(504), 0, PIXELS(480), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x16, FLAG_SECOND_AREA_NORTH_APE_OUT, (s32)gMogallForestPlacements1_1,
        PIXELS(360), 0, PIXELS(80), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x16, FLAG_SECOND_AREA_EAST_APE_OUT, (s32)gMogallForestPlacements1_1,
        PIXELS(488), 0, PIXELS(80), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x16, FLAG_SECOND_AREA_SOUTH_APE_OUT, (s32)gMogallForestPlacements1_1,
        PIXELS(504), 0, PIXELS(480), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x5a, FLAG_DJINNI_0 + 2, ACTOR_STAND,
        PIXELS(488), 0, PIXELS(568), FACING_EAST, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gMogallForestPlacements3[] = {
    { 0xcd, CONDITION_ALWAYS, 7,
        PIXELS(680), 0, PIXELS(96), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xcd, CONDITION_ALWAYS, 7,
        PIXELS(616), 0, PIXELS(400), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xcd, CONDITION_ALWAYS, 7,
        PIXELS(552), 0, PIXELS(480), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xcd, CONDITION_ALWAYS, 7,
        PIXELS(600), 0, PIXELS(544), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xcf, CONDITION_ALWAYS, 7,
        PIXELS(544), 0, PIXELS(536), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x125, CONDITION_ALWAYS, (s32)gMogallForestPlacements2_0,
        PIXELS(696), 0, PIXELS(120), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x125, CONDITION_ALWAYS, (s32)gMogallForestPlacements2_0,
        PIXELS(632), 0, PIXELS(408), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xbf, FLAG_THIRD_AREA_HOSTILE_APE_OUT, (s32)gMogallForest_ActorScript,
        PIXELS(488), 0, PIXELS(88), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xbf, FLAG_THIRD_AREA_CENTRAL_APE_OUT, (s32)gMogallForest_ActorScript,
        PIXELS(424), 0, PIXELS(120), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xbf, FLAG_THIRD_AREA_WEST_APE_OUT, (s32)gMogallForest_ActorScript,
        PIXELS(360), 0, PIXELS(80), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xbf, MOGALL_FOREST_EAST_ACTOR_CONDITION, (s32)gMogallForest_ActorScript,
        PIXELS(744), 0, PIXELS(400), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x16, FLAG_THIRD_AREA_HOSTILE_APE_OUT, (s32)gMogallForestPlacements1_1,
        PIXELS(488), 0, PIXELS(88), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x16, FLAG_THIRD_AREA_CENTRAL_APE_OUT, (s32)gMogallForestPlacements1_1,
        PIXELS(424), 0, PIXELS(120), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x16, FLAG_THIRD_AREA_WEST_APE_OUT, (s32)gMogallForestPlacements1_1,
        PIXELS(360), 0, PIXELS(80), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x16, MOGALL_FOREST_EAST_SHARED_SPRITE_CONDITION, (s32)gMogallForestPlacements1_1,
        PIXELS(744), 0, PIXELS(400), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SCENE_TABLE_END },
};

struct ScenePlacement gMogallForestPlacementsOther[] = {
    { SCENE_TABLE_END },
};

struct SceneEvent gMogallForestEvents1[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
    { EVENT_EXIT, 6, CONDITION_ALWAYS, 6 },
    { EVENT_EXIT, 7, CONDITION_ALWAYS, 7 },
    { EVENT_EXIT, 8, CONDITION_ALWAYS, 8 },
    { EVENT_EXIT, 9, CONDITION_ALWAYS, 9 },
    { TOUCH_WALKING, 20, CONDITION_ALWAYS, EVENT_SCRIPT(MogallForest_FirstArea_PushLog) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 25, CONDITION_ALWAYS, EVENT_SCRIPT(MogallForest_FirstArea_JumpNorth) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 26, CONDITION_ALWAYS, EVENT_SCRIPT(MogallForest_FirstArea_JumpSouth) },
    { SEARCH(SEARCH_UNNAMED), 27, CONDITION_ALWAYS, EVENT_SCRIPT(MogallForest_ReadBrokenSign) },
    { PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_FIRST_AREA_NORTH_HIDEOUT, FLAG_FIRST_AREA_NORTH_APE_OUT, EVENT_SCRIPT(MogallForest_FirstArea_NorthApeShowsWay) },
    { PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_FIRST_AREA_HOSTILE_HIDEOUT, FLAG_FIRST_AREA_HOSTILE_APE_OUT, EVENT_SCRIPT(MogallForest_FirstArea_ApeAttacks) },
    { PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_FIRST_AREA_EAST_HIDEOUT, FLAG_FIRST_AREA_EAST_APE_OUT, EVENT_SCRIPT(MogallForest_FirstArea_EastApeShowsWay) },
    { PSYNERGY_ON_ACTOR(ABILITY_CATCH), ACTOR_NUT, FLAG_NUT_CAUGHT, EVENT_SCRIPT(MogallForest_CatchNut) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gMogallForestEvents2[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
    { EVENT_EXIT, 6, CONDITION_ALWAYS, 6 },
    { EVENT_EXIT, 7, CONDITION_ALWAYS, 7 },
    { EVENT_EXIT, 8, CONDITION_ALWAYS, 8 },
    { EVENT_EXIT, 9, CONDITION_ALWAYS, 9 },
    { EVENT_EXIT, 10, CONDITION_ALWAYS, 10 },
    { EVENT_EXIT, 11, CONDITION_ALWAYS, 11 },
    { EVENT_EXIT, 12, CONDITION_ALWAYS, 12 },
    { TOUCH_WALKING, 20, CONDITION_ALWAYS, EVENT_SCRIPT(MogallForest_SecondArea_PushLog) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SECOND_AREA_OBSTACLE, CONDITION_ALWAYS, EVENT_SCRIPT(MogallForest_SecondArea_UpdateMovableObstacle) },
    { PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_SECOND_AREA_NORTH_HIDEOUT, FLAG_SECOND_AREA_NORTH_APE_OUT, EVENT_SCRIPT(MogallForest_SecondArea_NorthApeShowsWay) },
    { PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_SECOND_AREA_EAST_HIDEOUT, FLAG_SECOND_AREA_EAST_APE_OUT, EVENT_SCRIPT(MogallForest_SecondArea_EastApeShowsWay) },
    { PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_SECOND_AREA_SOUTH_HIDEOUT, FLAG_SECOND_AREA_SOUTH_APE_MOVED, EVENT_SCRIPT(MogallForest_SecondArea_SouthApeHidesAgain) },
    { PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_SECOND_AREA_SOUTH_HIDEOUT, FLAG_SECOND_AREA_SOUTH_APE_OUT, EVENT_SCRIPT(MogallForest_SecondArea_SouthApeShowsWay) },
    { SEARCH(SEARCH_CHEST), 100, FLAG_CHEST_APPLE, FIND_ITEM(ITEM_APPLE) },
    { EVENT_TALK, ACTOR_QUARTZ, CONDITION_ALWAYS, EVENT_SCRIPT(MogallForest_StartDjinnBattle) },
    { SCENE_EVENTS_END },
};
// clang-format on

struct SceneEvent gMogallForestEvents3[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
    {EVENT_EXIT, 2, CONDITION_ALWAYS, 2},
    {EVENT_EXIT, 3, CONDITION_ALWAYS, 3},
    {EVENT_EXIT, 4, CONDITION_ALWAYS, 4},
    {EVENT_EXIT, 5, CONDITION_ALWAYS, 5},
    {EVENT_EXIT, 6, CONDITION_ALWAYS, 6},
    {EVENT_EXIT, 7, CONDITION_ALWAYS, 7},
    {EVENT_EXIT, 8, CONDITION_ALWAYS, 8},
    {EVENT_EXIT, 9, CONDITION_ALWAYS, 9},
    {EVENT_EXIT, 10, CONDITION_ALWAYS, 10},
    {EVENT_EXIT, 11, CONDITION_ALWAYS, 11},
    {EVENT_EXIT, 12, CONDITION_ALWAYS, 12},
    {EVENT_EXIT, 13, CONDITION_ALWAYS, 13},
    {TOUCH_WALKING, 20, CONDITION_ALWAYS,
     EVENT_SCRIPT(MogallForest_ThirdArea_PushLog)},
    {TOUCH_WALKING_INTO(FACING_WEST), 25, CONDITION_ALWAYS,
     EVENT_SCRIPT(MogallForest_ThirdArea_JumpWest)},
    {EVENT_TOUCH, 22, FLAG_MOGALL_FOREST_LED_TO_KILLER_APE,
     EVENT_SCRIPT(MogallForest_ApeLeadsToKillerApe)},
    {EVENT_TOUCH, 23, FLAG_MOGALL_FOREST_KILLER_APE_BEATEN,
     EVENT_SCRIPT(MogallForest_KillerApeAttacks)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_THIRD_AREA_NORTH_OBSTACLE,
     CONDITION_ALWAYS,
     EVENT_SCRIPT(MogallForest_ThirdArea_UpdateNorthObstacle)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_THIRD_AREA_SOUTH_OBSTACLE,
     FLAG_SOUTH_OBSTACLE_SETTLED,
     EVENT_SCRIPT(MogallForest_ThirdArea_UpdateSouthObstacle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_THIRD_AREA_HOSTILE_HIDEOUT,
     FLAG_THIRD_AREA_HOSTILE_APE_OUT,
     EVENT_SCRIPT(MogallForest_ThirdArea_ApeAttacks)},
    {PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_THIRD_AREA_CENTRAL_HIDEOUT,
     FLAG_THIRD_AREA_CENTRAL_APE_OUT,
     EVENT_SCRIPT(MogallForest_ThirdArea_CentralApeShowsWay)},
    {PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_THIRD_AREA_WEST_HIDEOUT,
     FLAG_THIRD_AREA_WEST_APE_OUT,
     EVENT_SCRIPT(MogallForest_ThirdArea_WestApeShowsWay)},
    {PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_THIRD_AREA_EAST_HIDEOUT,
     FLAG_THIRD_AREA_EAST_APE_MOVED,
     EVENT_SCRIPT(MogallForest_ThirdArea_EastApeHidesAgain)},
    {PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_THIRD_AREA_EAST_HIDEOUT,
     FLAG_THIRD_AREA_EAST_APE_OUT,
     EVENT_SCRIPT(MogallForest_ThirdArea_EastApeShowsWay)},
    {SEARCH(SEARCH_CHEST), 100, FLAG_CHEST_ITEM_5D,
     FIND_ITEM(ITEM_ELVEN_SHIRT)},
    {SCENE_EVENTS_END},
};

struct SceneEvent gMogallForestEventsOther[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
    {EVENT_EXIT, 2, CONDITION_ALWAYS, 2},
    {EVENT_EXIT, 3, CONDITION_ALWAYS, 3},
    {EVENT_EXIT, 4, CONDITION_ALWAYS, 4},
    {EVENT_EXIT, 5, CONDITION_ALWAYS, 5},
    {EVENT_EXIT, 6, CONDITION_ALWAYS, 6},
    {EVENT_EXIT, 7, CONDITION_ALWAYS, 7},
    {EVENT_EXIT, 8, CONDITION_ALWAYS, 8},
    {EVENT_EXIT, 11, CONDITION_ALWAYS, 20},
    {EVENT_EXIT, 12, CONDITION_ALWAYS, 19},
    {EVENT_EXIT, 13, CONDITION_ALWAYS, 13},
    {EVENT_EXIT, 14, CONDITION_ALWAYS, 14},
    {EVENT_EXIT, 15, CONDITION_ALWAYS, 15},
    {EVENT_EXIT, 16, CONDITION_ALWAYS, 16},
    {EVENT_EXIT, 17, CONDITION_ALWAYS, 17},
    {EVENT_EXIT, 18, CONDITION_ALWAYS, 18},
    {SEARCH(SEARCH_UNNAMED), 100, FLAG_PSYNERGY_STONE, FIND_PSYNERGY_STONE},
    {SCENE_EVENTS_END},
};
