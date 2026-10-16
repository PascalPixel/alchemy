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

#if TBS_PATCH_LEVEL == 1
#define MOGALL_FOREST_EAST_ACTOR_CONDITION 0x30b
#else
#define MOGALL_FOREST_EAST_ACTOR_CONDITION CONDITION_ALWAYS
#endif

#if !TBS_PATCH_1
#define MOGALL_FOREST_EAST_SHARED_SPRITE_CONDITION CONDITION_ALWAYS
#else
#define MOGALL_FOREST_EAST_SHARED_SPRITE_CONDITION 0x30b
#endif

enum { ITEM_NUT = 181 };

// Shared source is included here in ROM order.
#include "common/object_staged_actor.h"

// Shared source is included here in ROM order.
#include "common/effect_create.h"
#include "common/effect_spawn.h"

// Start the Djinn encounter in the second forest area.
static void MogallForest_StartDjinnBattle(void) {
  Field_RequestDjinnBattle(18, 2);
}

// Jump the leader to an unobstructed fixed-point position and restore motion.
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
    Engine_AudioPlayCue(152);
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

// Apply the actor work value as its sprite draw parameter.
static s32
MogallForest_CopyTimerToChild(struct ScriptInterpreter *interpreter) {
  struct FieldActor *actor = (struct FieldActor *)interpreter;
  ObjectGroup_SetChildValue(actor, actor->timer);
  return 0;
}

// Clear the actor sprite flags when its script requests it.
static s32
MogallForest_ClearActorSpriteFlags(struct ScriptInterpreter *interpreter) {
  struct FieldActor *actor = (struct FieldActor *)interpreter;
  Engine_ActorSetSpriteFlags(actor, 0);
  return 0;
}

// Hop an actor to a pixel destination with the requested vertical speed.
static void MogallForest_HopActorTo(s32 actorId, s32 targetX, s32 targetZ,
                                    s32 verticalSpeed) {
  struct FieldActor *actor;

  actor = Object_GetById(actorId);
  Engine_ActorSetSpritePriority(actorId, 1);
  Actor_SetSpeed(actorId, PIXELS(3), PIXELS(3) / 2);
  Engine_AudioPlayCue(152);
  actor->velocityY = verticalSpeed;
  actor->gravity = PIXELS(1) / 2;
  actor->bounce = 0;
  Engine_ActorSetSpriteFlags(actor, 0);
  Actor_MoveToAndWait(actorId, targetX, targetZ);
  Actor_SetPosition(actorId, PIXELS(targetX), PIXELS(targetZ));
  Engine_ActorSetSpriteFlags(actor, 1);
  actor->gravity = PIXELS(1);
}

// Scatter a complete ring of rising puffs around an actor.
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
  Engine_AudioPlayCue(188);
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

// Focus on a creature, emit its reveal puffs and select its draw parameter.
static void MogallForest_FocusOnRevealedCreature(s32 actorId,
                                                 s32 drawParameter) {
  s32 cameraSpeed = PIXELS(2);
  s32 cameraAcceleration = PIXELS(1) / 4;

  Engine_CameraSetSpeed(cameraSpeed, cameraAcceleration);
  Engine_CameraMoveToActor(actorId, 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(30);
  MogallForest_SpawnPuffRing(actorId);
  Engine_ActorSetChildValue(actorId, drawParameter);
}

// Let a friendly creature face the leader and signal its route.
static inline void MogallForest_ShowGuideDirection(s32 guideId) {
  Engine_CameraFollowActor(guideId, 1);
  Engine_ActorFaceEachOther(guideId, ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(20);
  Engine_ActorStartRepeatedMotion(guideId, 2);
  Engine_ActorSetAttachedEffect(guideId, 258);
  Engine_EventWait(60);
}

// Select the entrances for the current forest area.
const struct SceneEntrance *Scene_GetEntrances(void) {
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

// Return the scene protocol default.
s32 SceneData_ReturnZero(void) { return 0; }

// Return the exit connections shared by the forest areas.
u32 *MogallForest_GetExits(void) { return gMogallForest_SceneTable; }

// Select actor placements for the current forest area.
const struct ScenePlacement *Scene_GetPlacements(void) {
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

// Push a log and settle it into the first area crossing when it arrives.
static void MogallForest_FirstArea_PushLog(void) {
  struct StagedActorProbe probe;

  Engine_EventBegin();
  if (StagedActor_FindClearPosition(&probe) != 0) {
    SceneActor_MoveAndRedraw(probe);
    if (probe.actorSlot == 9 && (probe.positionZ >> 20) == 26) {
      Engine_GameFlagSet(0x310);
      Engine_ActorSetAnimation(9, 3);
      Actor_SetSpeed(9, PIXELS(1) / 4, PIXELS(1) / 2);
      Engine_ActorSetDestinationOffset(9, 0, -16);
      Engine_EventWait(45);
      Engine_ActorSetAnimation(9, 8);
      Engine_AudioPlayCue(240);
      Engine_ActorSetSpritePriority(9, 1);
      Object_GetById(9)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
      Map_CopyCellAttributes(38, 27, 4, 2, 31, 25);
    }
  }
  Engine_EventEnd();
}

// Jump north across the first area log.
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

// Jump south onto the first area log and play its landing bounce.
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
    Engine_ActorSetAnimation(9, 7);
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

// Read the broken forest sign.
static void MogallForest_ReadBrokenSign(void) {
  Engine_EventBegin();
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Engine_MessageShowCentered(MSGNO(MsgMogallForestBrokenSign), 1);
  Engine_EventEnd();
}

// Reveal the northern guide and watch it hop toward the southern path.
static void MogallForest_FirstArea_RevealNorthGuide(void) {
  struct FieldActor *creature;

  creature = Object_GetById(10);

  Engine_EventBegin();

  MogallForest_FocusOnRevealedCreature(10, 1);
  MogallForest_HopActorTo(10, 88, 120, PIXELS(6));

  Effect_Spawn(creature->x.fixed, creature->y.fixed,
               creature->z.fixed + PIXELS(24), 0, 0, 0, 1, 0);

  MogallForest_ShowGuideDirection(10);

  MogallForest_HopActorTo(10, 88, 152, PIXELS(3));
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 10, 0);
  Engine_EventWait(10);

  MogallForest_HopActorTo(10, 120, 192, PIXELS(3));
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 10, 0);
  Engine_EventWait(10);

  MogallForest_HopActorTo(10, 120, 240, PIXELS(3));
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 10, 0);
  Engine_EventWait(10);

  Engine_GameFlagSet(768);
  Engine_ActorSetPosition(13, 0, 0);
  Engine_ActorSetPosition(10, 0, 0);

  Engine_EventEnd();
}

// Reveal a hostile creature and start its encounter when it reaches the leader.
static void MogallForest_FirstArea_RevealHostileCreature(void) {
  struct FieldActor *creature;

  creature = Object_GetById(11);

  Engine_EventBegin();

  MogallForest_FocusOnRevealedCreature(11, 0);
  MogallForest_HopActorTo(11, 408, 456, PIXELS(6));

  Effect_Spawn(creature->x.fixed, creature->y.fixed,
               creature->z.fixed + PIXELS(24), 0, 0, 0, 1, 0);

  Engine_CameraFollowActor(11, 1);
  Engine_ActorFaceEachOther(11, ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(30);
  Engine_ActorStartRepeatedMotion(11, 2);
  Engine_ActorShowEmote(11, EMOTE_IN_FRONT | 3, 0);
  Engine_AudioPlayCue(147);
  Engine_EventWait(60);

  MogallForest_HopActorTo(11, Object_GetById(0)->x.part.pixel,
                          Object_GetById(0)->z.part.pixel, PIXELS(4));

  Engine_EventWait(10);
  Engine_GameFlagSet(0x301);
  Engine_ActorSetPosition(14, 0, 0);

  Battle_SetStart(3);

  Field_RequestBattleFromEncounterTable(53, 0);

  Engine_EventEnd();
}

// Reveal the eastern guide and watch it hop out of the clearing.
static void MogallForest_FirstArea_RevealEastGuide(void) {
  struct FieldActor *creature;

  creature = Object_GetById(12);

  Engine_EventBegin();

  MogallForest_FocusOnRevealedCreature(12, 1);
  MogallForest_HopActorTo(12, 536, 344, PIXELS(7));

  Effect_Spawn(creature->x.fixed, creature->y.fixed,
               creature->z.fixed + PIXELS(16), 0, 0, 0, 1, 0);

  MogallForest_ShowGuideDirection(12);

  MogallForest_HopActorTo(12, 584, 344, PIXELS(3));
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 12, 0);
  Engine_EventWait(6);

  MogallForest_HopActorTo(12, 632, 344, PIXELS(3));
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 12, 0);
  Engine_EventWait(6);

  MogallForest_HopActorTo(12, 680, 344, PIXELS(3));
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 12, 0);
  Engine_EventWait(6);

  Engine_GameFlagSet(0x302);
  Engine_ActorSetPosition(15, 0, 0);

  Engine_EventEnd();
}

// Collect the floating nut with Catch.
static void MogallForest_CatchNut(void) {
  Engine_EventBegin();
  Engine_ActorSetPosition(16, 0, 0);
  Engine_GameFlagSet(4052);
  Engine_ItemShowFound(ITEM_NUT, 3);
  Engine_PartyGiveItem(ITEM_NUT, 0);
  Engine_EventEnd();
}

// Push a log and update the second area crossing cells.
static void MogallForest_SecondArea_PushLog(void) {
  struct StagedActorProbe probe;
  s32 destinationColumn;
  s32 destinationRow;
  s32 rectangleHeight;
  s32 attributes;

  Engine_EventBegin();

  if (StagedActor_FindClearPosition(&probe) != 0) {
    SceneActor_MoveAndRedraw(probe);

    if (probe.actorSlot == 8 && (probe.positionZ >> 20) == 23) {
      destinationColumn = 35;
      destinationRow = 68;
      Map_CopyCellAttributes(35, 67, 4, 1, destinationColumn, destinationRow);
    } else if (probe.actorSlot == 10 && (probe.positionX >> 20) == 35) {

      attributes = 0;
      Engine_GameFlagSet(0x311);
      Engine_ActorSetAnimation(10, 3);
      Engine_ActorSetDestinationOffset(10, -16, 6);
      Engine_EventWait(30);
      Engine_ActorSetAnimation(10, 8);
      Engine_AudioPlayCue(240);

      Object_GetById(10)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;

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

// Mark the obstacle cell and clear the cells it could have moved from.
static inline void MogallForest_UpdateObstacleFootprint(s32 column, s32 row) {
  StagedActor_FillGridAttributeRectangle(2, column, row, 1, 1, 255);
  StagedActor_FillGridAttributeRectangle(2, column + 1, row, 1, 1, 0);
  StagedActor_FillGridAttributeRectangle(2, column - 1, row, 1, 1, 0);
  StagedActor_FillGridAttributeRectangle(2, column, row + 1, 1, 1, 0);
  StagedActor_FillGridAttributeRectangle(2, column, row - 1, 1, 1, 0);
}

// Refresh the moved obstacle footprint and lower it at the crossing.
static void MogallForest_SecondArea_UpdateMovableObstacle(void) {
  s32 column;
  s32 row;

  Engine_EventBegin();

  column = Object_GetById(11)->x.fixed >> 20;
  row = Object_GetById(11)->z.fixed >> 20;

  MogallForest_UpdateObstacleFootprint(column, row);

  if (column == 36 && row == 24) {
    struct FieldActor *obstacle = Object_GetById(11);

    obstacle->motionFlags = 0;
    obstacle->terrainHeight = -PIXELS(2);
    obstacle->y.fixed = -PIXELS(2);
  }

  Engine_EventEnd();
}

// Reveal the northern guide and follow its route south.
static void MogallForest_SecondArea_RevealNorthGuide(void) {
  struct FieldActor *creature;

  creature = Object_GetById(12);
  Engine_EventBegin();
  MogallForest_FocusOnRevealedCreature(12, 1);
  MogallForest_HopActorTo(12, 392, 104, PIXELS(7));
  Engine_EventWait(10);
  Effect_Spawn(creature->x.fixed, creature->y.fixed,
               (creature->z.fixed + PIXELS(4)), 0, 0, 0, 1, 0);
  MogallForest_ShowGuideDirection(12);
  MogallForest_HopActorTo(12, 424, 120, PIXELS(3));
  Engine_ActorFaceActor(0, 12, 0);
  Engine_EventWait(6);
  MogallForest_HopActorTo(12, 424, 168, PIXELS(3));
  Engine_ActorFaceActor(0, 12, 0);
  Engine_EventWait(6);
  MogallForest_HopActorTo(12, 424, 208, PIXELS(3));
  Engine_ActorFaceActor(0, 12, 0);
  Engine_EventWait(6);
  MogallForest_HopActorTo(12, 424, 232, PIXELS(3));
  Engine_ActorFaceActor(0, 12, 0);
  Engine_EventWait(6);
  Engine_ActorSetPosition(12, 0, 0);
  Engine_GameFlagSet(0x303);
  Engine_ActorSetPosition(15, 0, 0);
  Engine_EventEnd();
}

// Reveal the eastern guide and follow its route across the clearing.
static void MogallForest_SecondArea_RevealEastGuide(void) {
  struct FieldActor *creature;

  creature = Object_GetById(13);

  Engine_EventBegin();

  MogallForest_FocusOnRevealedCreature(13, 1);
  MogallForest_HopActorTo(13, 456, 104, PIXELS(7));
  Engine_EventWait(10);

  Effect_Spawn(creature->x.fixed, creature->y.fixed,
               creature->z.fixed + PIXELS(4), 0, 0, 0, 1, 0);

  MogallForest_ShowGuideDirection(13);

  MogallForest_HopActorTo(13, 472, 136, PIXELS(3));
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 13, 0);
  Engine_EventWait(6);

  MogallForest_HopActorTo(13, 504, 136, PIXELS(16) / 5);
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 13, 0);
  Engine_EventWait(6);

  MogallForest_HopActorTo(13, 552, 136, PIXELS(7) / 2);
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 13, 0);
  Engine_EventWait(6);

  MogallForest_HopActorTo(13, 584, 136, PIXELS(7) / 2);
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 13, 0);
  Engine_EventWait(6);

  Engine_ActorSetPosition(13, 0, 0);
  Engine_GameFlagSet(772);
  Engine_ActorSetPosition(16, 0, 0);

  Engine_EventEnd();
}

// Move the southern creature to its next reveal position.
static void MogallForest_SecondArea_RelocateSouthGuide(void) {
  struct FieldActor *creature;

  Engine_EventBegin();
  MogallForest_FocusOnRevealedCreature(14, 1);
  MogallForest_HopActorTo(14, 424, 480, PIXELS(38) / 5);
  Engine_EventWait(2);
  MogallForest_SpawnPuffRing(14);
  Actor_SetChildValue(14, 15);
  creature = Actor_Get(14);
  Engine_ActorSetSpriteFlags(creature, 0);
  Engine_EventWait(30);
  GameFlag_Set(0x305);
  Actor_SetPosition(17, PIXELS(424), PIXELS(480));
  Engine_EventEnd();
}

// Reveal the southern creature and watch it guide the leader west.
static void MogallForest_SecondArea_RevealWestGuide(void) {
  struct FieldActor *creature;

  creature = Object_GetById(14);

  Engine_EventBegin();

  MogallForest_FocusOnRevealedCreature(14, 1);
  MogallForest_HopActorTo(14, 392, 504, PIXELS(6));
  Engine_EventWait(10);

  Effect_Spawn(creature->x.fixed, creature->y.fixed,
               creature->z.fixed + PIXELS(4), 0, 0, 0, 1, 0);

  MogallForest_ShowGuideDirection(14);

  MogallForest_HopActorTo(14, 360, 528, PIXELS(3));
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 14, 0);
  Engine_EventWait(6);

  MogallForest_HopActorTo(14, 328, 528, PIXELS(3));
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 14, 0);
  Engine_EventWait(6);

  MogallForest_HopActorTo(14, 288, 528, PIXELS(3));
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 14, 0);
  Engine_EventWait(6);

  MogallForest_HopActorTo(14, 256, 528, PIXELS(3));
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 14, 0);
  Engine_EventWait(6);

  Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 1);
  Engine_ActorSetPosition(14, 0, 0);
  Engine_EventWait(30);
  Engine_GameFlagSet(0x306);
  Engine_ActorSetPosition(17, 0, 0);

  Engine_EventEnd();
}

// Push a log and settle either crossing log when it reaches its destination.
static void MogallForest_ThirdArea_PushLog(void) {
  struct StagedActorProbe probe;
  s32 landed;

  Engine_EventBegin();
  landed = 0;
  if (StagedActor_FindClearPosition(&probe) != 0) {
    SceneActor_MoveAndRedraw(probe);
    if (probe.actorSlot == 9 || probe.actorSlot == 11) {
      if (probe.actorSlot == 9) {
        Map_CopyCellAttributes(38, 68, 1, 4, probe.positionX >> 20, 68);
        if (probe.positionX >> 20 == 42) {
          Map_CopyCellAttributes(26, 20, 2, 4, probe.positionX >> 20, 23);
          Engine_ActorSetSpritePriority(9, 1);
          landed = 1;
          GameFlag_Set(0x312);
        }
      } else {
        if (probe.positionX >> 20 == 40) {
          Map_CopyCellAttributes(26, 20, 2, 4, probe.positionX >> 20, 32);
          Engine_ActorSetSpritePriority(11, 1);
          landed = 1;
          GameFlag_Set(0x313);
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
      Engine_AudioPlayCue(240);
      Actor_Get(probe.actorSlot)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    } else {
      if (probe.actorSlot == 8)
        Map_CopyCellAttributes(42, 49, 1, 4, probe.positionX >> 20, 49);
    }
  }
  Engine_EventEnd();
}

// Jump west onto the third area log and play its landing bounce.
static void MogallForest_ThirdArea_JumpWest(void) {
  struct FieldActor *leader;
  u8 *motionFlags;
  u8 savedMotionFlags;
  struct FieldPosition position;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  motionFlags = &leader->motionFlags;
  savedMotionFlags = *motionFlags;

  position.x = Object_GetById(0)->x.fixed - PIXELS(32);
  position.y = Object_GetById(0)->y.fixed;
  position.z = Object_GetById(0)->z.fixed;

  if (MogallForest_TryJumpLeaderTo(&position) != 0) {

    Engine_EventBegin();

    *motionFlags = 0;
    Engine_ActorSetAnimation(11, 7);

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

// Refresh the northern movable obstacle footprint and crossing height.
static void MogallForest_ThirdArea_UpdateNorthObstacle(void) {
  s32 column;
  s32 row;

  Engine_EventBegin();

  column = Object_GetById(13)->x.fixed >> 20;
  row = Object_GetById(13)->z.fixed >> 20;

  MogallForest_UpdateObstacleFootprint(column, row);

  if (column == 45 && row == 6) {
    struct FieldActor *obstacle = Object_GetById(13);

    obstacle->motionFlags = 0;
    obstacle->terrainHeight = -PIXELS(2);
    obstacle->y.fixed = -PIXELS(2);
  }

  Engine_EventEnd();
}

// Refresh the southern obstacle and mark the crossing when it settles.
static void MogallForest_ThirdArea_UpdateSouthObstacle(void) {

  s32 column;
  s32 row;
  struct FieldActor *obstacle;

  Engine_EventBegin();
  obstacle = Object_GetById(14);
  column = obstacle->x.fixed;
  obstacle = Object_GetById(14);
  row = obstacle->z.fixed;
  column >>= 20;
  row >>= 20;
  MogallForest_UpdateObstacleFootprint(column, row);
  obstacle = Object_GetById(14);
  if ((obstacle->z.fixed >> 20) == 27) {
    obstacle = Object_GetById(14);
    obstacle->motionFlags = 0;
    obstacle->terrainHeight = -PIXELS(2);
    obstacle->y.fixed = -PIXELS(2);
    Engine_GameFlagSet(0x214);
    StagedActor_FillGridAttributeRectangle(2, 43, 23, 1, 1, 255);
  }
  Engine_EventEnd();
}

// Reveal a hostile creature and make it jump toward the leader.
static void MogallForest_ThirdArea_RevealHostileCreature(void) {

  struct FieldActor *creature;
  s32 jumpSpeed;
  struct FieldActor *leaderForX;
  s32 leaderX;
  struct FieldActor *leaderForZ;

  creature = Object_GetById(15);
  jumpSpeed = PIXELS(8);
  Engine_EventBegin();
  MogallForest_FocusOnRevealedCreature(15, 0);
  MogallForest_HopActorTo(15, 472, 104, jumpSpeed);
  Engine_EventWait(10);
  Effect_Spawn(creature->x.fixed, creature->y.fixed,
               (creature->z.fixed + jumpSpeed), 0, 0, 0, 1, 0);
  Engine_CameraFollowActor(15, 1);
  Engine_ActorFaceEachOther(15, ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(30);
  Engine_ActorStartRepeatedMotion(15, 2);
  Engine_ActorShowEmote(15, EMOTE_IN_FRONT | 3, 0);
  Engine_AudioPlayCue(147);
  Engine_EventWait(60);
  leaderForX = Object_GetById(ACTOR_PARTY_LEADER);
  leaderX = leaderForX->x.part.pixel;
  leaderForZ = Object_GetById(ACTOR_PARTY_LEADER);
  MogallForest_HopActorTo(15, leaderX, leaderForZ->z.part.pixel, PIXELS(6));
  Engine_EventWait(10);
  Engine_GameFlagSet(0x307);
  Battle_SetStart(3);
  Field_RequestBattleFromEncounterTable(53, 0);
  Engine_EventEnd();
}

// Reveal the central guide and follow its route toward the southern clearing.
static void MogallForest_ThirdArea_RevealCentralGuide(void) {
  struct FieldActor *creature;

  creature = Object_GetById(16);

  Engine_EventBegin();

  MogallForest_FocusOnRevealedCreature(16, 1);
  MogallForest_HopActorTo(16, 456, 152, PIXELS(6));
  Engine_EventWait(10);

  Effect_Spawn(creature->x.fixed, creature->y.fixed,
               creature->z.fixed + PIXELS(4), 0, 0, 0, 1, 0);

  MogallForest_ShowGuideDirection(16);

  MogallForest_HopActorTo(16, 448, 192, PIXELS(3));
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 16, 0);
  Engine_EventWait(6);

  MogallForest_HopActorTo(16, 424, 208, PIXELS(3));
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 16, 0);
  Engine_EventWait(6);

  MogallForest_HopActorTo(16, 424, 224, PIXELS(3));
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 16, 0);
  Engine_EventWait(6);

  Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 1);
  Engine_ActorSetPosition(16, 0, 0);
  Engine_EventWait(30);
  Engine_GameFlagSet(776);
  Engine_ActorSetPosition(20, 0, 0);

  Engine_EventEnd();
}

// Reveal the western guide and follow its route out of the clearing.
static void MogallForest_ThirdArea_RevealWestGuide(void) {
  struct FieldActor *creature;

  creature = Object_GetById(17);

  Engine_EventBegin();

  MogallForest_FocusOnRevealedCreature(17, 1);
  MogallForest_HopActorTo(17, 392, 104, PIXELS(6));
  Engine_EventWait(10);

  Effect_Spawn(creature->x.fixed, creature->y.fixed,
               creature->z.fixed + PIXELS(4), 0, 0, 0, 1, 0);

  MogallForest_ShowGuideDirection(17);

  MogallForest_HopActorTo(17, 376, 152, PIXELS(6));
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 17, 0);
  Engine_EventWait(10);

  MogallForest_HopActorTo(17, 328, 160, PIXELS(3));
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 17, 0);
  Engine_EventWait(6);

  MogallForest_HopActorTo(17, 296, 160, PIXELS(3));
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 17, 0);
  Engine_EventWait(6);

  Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 1);
  Engine_ActorSetPosition(17, 0, 0);
  Engine_EventWait(30);
  Engine_GameFlagSet(0x309);
  Engine_ActorSetPosition(21, 0, 0);

  Engine_EventEnd();
}

// Move the eastern creature to its next reveal position.
static void MogallForest_ThirdArea_RelocateEastGuide(void) {
  struct FieldActor *creature;

  Engine_EventBegin();
  MogallForest_FocusOnRevealedCreature(18, 1);
  Camera_MoveTo(PIXELS(744), -1, PIXELS(504), 1);
  MogallForest_HopActorTo(18, 744, 504, PIXELS(9));
  MogallForest_SpawnPuffRing(18);
  Actor_SetChildValue(18, 15);
  creature = Actor_Get(18);
  Engine_ActorSetSpriteFlags(creature, 0);
  Engine_EventWait(30);
  GameFlag_Set(0x30a);
  Actor_SetPosition(22, PIXELS(744), PIXELS(504));
  Engine_EventEnd();
}

// Reveal the eastern creature and follow its route south.
static void MogallForest_ThirdArea_RevealSouthGuide(void) {
  struct FieldActor *creature;

  creature = Object_GetById(18);

  Engine_EventBegin();

  MogallForest_FocusOnRevealedCreature(18, 1);
  MogallForest_HopActorTo(18, 712, 536, PIXELS(6));
  Engine_EventWait(10);

  Effect_Spawn(creature->x.fixed, creature->y.fixed,
               creature->z.fixed + PIXELS(4), 0, 0, 0, 1, 0);

  MogallForest_ShowGuideDirection(18);

  MogallForest_HopActorTo(18, 712, 568, PIXELS(6));
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 18, 0);
  Engine_EventWait(10);

  MogallForest_HopActorTo(18, 712, 600, PIXELS(3));
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 18, 0);
  Engine_EventWait(6);

  MogallForest_HopActorTo(18, 736, 640, PIXELS(3));
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 18, 0);
  Engine_EventWait(6);

  MogallForest_HopActorTo(18, 736, 704, PIXELS(3));
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 18, 0);
  Engine_EventWait(6);

  Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 1);
  Engine_ActorSetPosition(18, 0, 0);
  Engine_EventWait(30);
  Engine_GameFlagSet(0x30b);

  Engine_EventEnd();
}

// Watch the final creature lead the party farther into the forest.
static void MogallForest_FollowCreatureTowardFinalEncounter(void) {
  struct FieldActor *creature;

  creature = Object_GetById(18);
  Engine_EventBegin();
  Actor_SetPosition(18, PIXELS(136), PIXELS(360));
  MogallForest_FocusOnRevealedCreature(18, 1);
  MogallForest_HopActorTo(18, 136, 408, PIXELS(8));
  Engine_EventWait(10);
  Effect_Spawn(creature->x.fixed, creature->y.fixed,
               (creature->z.fixed + PIXELS(4)), 0, 0, 0, 1, 0);
  Actor_FaceDirection(18, FACING_NORTH, 40);
  Actor_SetAttachedEffect(18, 0x102);
  Engine_ActorRunRepeatedMotion(18, 2);
  Engine_CameraFollowActor(18, 1);
  MogallForest_HopActorTo(18, 136, 440, PIXELS(6));
  Actor_FaceActor(ACTOR_PARTY_LEADER, 18, 0);
  Engine_EventWait(10);
  MogallForest_HopActorTo(18, 136, 472, PIXELS(3));
  Actor_FaceActor(ACTOR_PARTY_LEADER, 18, 0);
  Engine_EventWait(6);
  MogallForest_HopActorTo(18, 136, 504, PIXELS(3));
  Actor_FaceActor(ACTOR_PARTY_LEADER, 18, 0);
  Engine_EventWait(6);
  Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 1);
  Actor_SetPosition(18, 0, 0);
  Engine_EventWait(60);
  GameFlag_Set(0x89d);
  Engine_EventEnd();
}

// Reveal the enlarged creature, scatter its puffs and start the final
// encounter.
static void MogallForest_StartFinalCreatureEncounter(void) {
  struct Vector3 velocity;
  struct Vector3 *emissionVelocity;
  struct FieldActor *actor;
  u32 directionIndex;
  s32 x;
  s32 z;

  actor = Object_GetById(18);
  Engine_EventBegin();
  // Shake the hiding place twice before the creature appears.
  Engine_ActorSetChildValue(18, 15);
  Engine_ActorSetSpriteFlags(Object_GetById(18), 0);
  Actor_SetPosition(18, PIXELS(136), PIXELS(360));
  Engine_CameraSetSpeed(PIXELS(1) / 2, PIXELS(1) / 16);
  Camera_MoveTo(PIXELS(136), -1, PIXELS(392), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(60);
  Map_SetShake(PIXELS(5), PIXELS(5), Q16_ONE);
  MogallForest_SpawnPuffRing(18);
  Engine_ActorStartRepeatedMotion(0, 2);
  Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Engine_MapWaitWorkValuesBelow256();
  Engine_ActorFaceDirection(0, FACING_NORTH, 20);
  Engine_EventWait(40);
  Map_SetShake(PIXELS(5), PIXELS(5), Q16_ONE);
  MogallForest_SpawnPuffRing(18);
  Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
  Engine_MapWaitWorkValuesBelow256();
  Engine_EventWait(40);
  MogallForest_SpawnPuffRing(18);
  actor->scaleX = 6 * Q16_ONE / 5;
  actor->scaleY = 6 * Q16_ONE / 5;
  Engine_ActorSetChildValue(18, 5);
  MogallForest_HopActorTo(18, 136, 392, PIXELS(15));
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
  Engine_AudioPlayCue(148);
  Engine_ActorRunRepeatedMotion(18, 2);
  Engine_EventWait(20);
  // Jump toward the leader, then enter the final creature encounter.
  {
    s32 leaderX = Object_GetById(0)->x.part.pixel;

    MogallForest_HopActorTo(18, leaderX, Object_GetById(0)->z.part.pixel - 16,
                            PIXELS(8));
  }
  Engine_EventWait(10);
  Battle_SetStart(3);
  Event_SetNextDestination((s32)&SceneId_MogallForest3, 15);
  Field_RequestBattleFromEncounterTable(53, 1);
  Engine_EventEnd();
}

// Select the event table for the current forest area.
const struct SceneEvent *Scene_GetEvents(void) {
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

// Restore the current clearing, revealed creatures and completed crossings.
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
      if (GameFlag_IsSet(0x89c) == 0) {
        Engine_EventBegin();
        Engine_TaskWait(1);
        Engine_ActorSetChildValue(10, 1);
        Actor_SetPosition(10, PIXELS(92), PIXELS(120));
        Actor_FaceDirection(10, FACING_NORTH + FACING_STEP, 0);
        Actor_SetSpeed(0, PIXELS(2) / 5, PIXELS(1) / 5);
        Engine_ActorWalkTo(0, 136, 64);
        Engine_EventOpenScreen();
        Engine_EventWaitForScreen();
        Engine_ActorWaitForMove(0);
        Engine_EventWait(30);
        Actor_ShowEmote(10, EMOTE_IN_FRONT, 0);
        Engine_ActorRunRepeatedMotion(10, 2);
        Engine_EventWait(30);
        MogallForest_HopActorTo(10, 136, 116, PIXELS(7));
        MogallForest_SpawnPuffRing(10);
        Engine_ActorSetChildValue(10, 15);
        Engine_ActorSetSpriteFlags(Object_GetById(10), 0);
        Engine_GameFlagSet(0x89c);
        Engine_EventWait(60);
        Engine_EventEnd();
      }
      if (Engine_GameFlagIsSet(0x109) == 0) {
        break;
      }
      if (Engine_GameFlagIsSet(768) != 0) {
        break;
      }
      Engine_ActorSetChildValue(10, 15);
      Actor_SetPosition(10, PIXELS(136), PIXELS(116));
      break;

    case 7:
    case 8:
    case 9:
      actor = Object_GetById(0);
      if (actor != 0) {
        Engine_ActorSetPosition(16, actor->x.fixed, actor->z.fixed);
      }
      actor = Object_GetById(16);
      actor->update = 0;
      if (Engine_GameFlagIsSet(0x109) != 0) {
        actor = Object_GetById(16);
        actor->y.fixed = PIXELS(32);
      }
      Engine_TaskWait(1);
      Engine_ActorSetPosition(16, PIXELS(632), PIXELS(440));
      if (Engine_GameFlagIsSet(0xfd4) == 0) {
        InitializeStagedActorSceneOrbitingEffect(16);
      }
      Engine_ActorSetChildValue(11, 15);
      Engine_ActorSetChildValue(12, 15);
      Engine_ActorSetSpriteFlags(Object_GetById(11), 0);
      Engine_ActorSetSpriteFlags(Object_GetById(12), 0);
      FieldScene_RedrawActorFootprint(8);
      if (Engine_GameFlagIsSet(784) == 0) {
        FieldScene_RedrawActorFootprint(9);
        break;
      }
      Engine_TaskWait(1);
      Actor_SetPosition(9, PIXELS(528), PIXELS(408));
      Engine_ActorSetAnimation(9, 4);
      Map_CopyCellAttributes(38, 27, 4, 2, 31, 25);
      Object_GetById(9)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
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
      if (Engine_GameFlagIsSet(0x303) == 0) {
        Engine_ActorSetChildValue(12, 15);
        Engine_ActorSetSpriteFlags(Object_GetById(12), 0);
      }
      if (Engine_GameFlagIsSet(772) != 0) {
        break;
      }
      Engine_ActorSetChildValue(13, 15);
      Engine_ActorSetSpriteFlags(Object_GetById(13), 0);
      break;

    case 10:
    case 11:
    case 12:
      if (Engine_GameFlagIsSet(0x311) == 0) {
        FieldScene_RedrawActorFootprint(10);
      } else {
        s32 attr = 0;

        Engine_TaskWait(1);
        Actor_SetPosition(10, PIXELS(552), PIXELS(510));
        Engine_ActorSetAnimation(10, 4);
        Object_GetById(10)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
        Map_CopyCellAttributes(44, 30, 2, 4, 34, 30);
#if TBS_PATCH_1
        StagedActor_FillGridAttributeRectangle(0, 35, 29, 1, 4, attr);
#endif
      }
      FieldScene_RedrawActorFootprint(8);
      FieldScene_RedrawActorFootprint(9);
      column = Object_GetById(11)->x.fixed;
      fixedZ = Object_GetById(11)->z.fixed;
      column >>= 20;
      StagedActor_FillGridAttributeRectangle(2, column, fixedZ >> 20, 1, 1,
                                             255);
      Engine_TaskWait(1);
      Engine_ActorSetChildValue(11, 6);
      Object_GetById(8)->collisionFlags |= 8;
      if (Engine_GameFlagIsSet(0x306) != 0) {
        break;
      }
      Engine_ActorSetChildValue(14, 15);
      Engine_ActorSetSpriteFlags(Object_GetById(14), 0);
      if (Engine_GameFlagIsSet(0x305) == 0) {
        break;
      }
      Actor_SetPosition(14, PIXELS(424), PIXELS(480));
      Actor_SetPosition(17, PIXELS(424), PIXELS(480));
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
      if (Engine_GameFlagIsSet(0x307) == 0) {
        Engine_ActorSetChildValue(15, 15);
        Engine_ActorSetSpriteFlags(Object_GetById(15), 0);
        Engine_ActorSetSpriteFlags(Object_GetById(19), 0);
      }
      if (Engine_GameFlagIsSet(776) == 0) {
        Engine_ActorSetChildValue(16, 15);
        Engine_ActorSetSpriteFlags(Object_GetById(16), 0);
        Engine_ActorSetSpriteFlags(Object_GetById(20), 0);
      }
      if (Engine_GameFlagIsSet(0x309) != 0) {
        break;
      }
      Engine_ActorSetChildValue(17, 15);
      Engine_ActorSetSpriteFlags(Object_GetById(17), 0);
      Engine_ActorSetSpriteFlags(Object_GetById(21), 0);
      break;

    case 7:
      column = Object_GetById(13)->x.fixed;
      fixedZ = Object_GetById(13)->z.fixed;
      column >>= 20;
      StagedActor_FillGridAttributeRectangle(2, column, fixedZ >> 20, 1, 1,
                                             255);
      Engine_ActorSetChildValue(13, 6);
      Engine_TaskWait(1);
      Object_GetById(8)->collisionFlags |= 8;
      FieldScene_RedrawActorFootprint(8);
      break;

    case 8:
    case 9:
    case 10:
    case 11:
      OverlayObject_CreateConfiguredObject(PIXELS(734), 0, PIXELS(370), 223);
      OverlayObject_CreateConfiguredObject(PIXELS(754), 0, PIXELS(370), 223);
      FieldScene_RedrawActorFootprint(10);
      FieldScene_RedrawActorFootprint(12);
      if (Engine_GameFlagIsSet(0x312) == 0) {
        FieldScene_RedrawActorFootprint(9);
      } else {
        Engine_TaskWait(1);
        Engine_ActorSetAnimation(9, 4);
        Engine_ActorSetPosition(9, PIXELS(698), PIXELS(398));
        Object_GetById(9)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
        Map_CopyCellAttributes(26, 20, 2, 4, 42, 23);
        Engine_GameFlagSet(532);
        Actor_SetPosition(14, PIXELS(632), PIXELS(440));
        Engine_ActorSetSpriteFlags(Object_GetById(14), 0);
      }
      if (Engine_GameFlagIsSet(0x313) == 0) {
        FieldScene_RedrawActorFootprint(11);
      } else {
        Engine_TaskWait(1);
        Engine_ActorSetAnimation(11, 4);
        Engine_ActorSetPosition(11, PIXELS(666), PIXELS(550));
        Object_GetById(11)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
        Map_CopyCellAttributes(26, 20, 2, 4, 40, 32);
      }
      column = Object_GetById(14)->x.fixed;
      fixedZ = Object_GetById(14)->z.fixed;
      column >>= 20;
      StagedActor_FillGridAttributeRectangle(2, column, fixedZ >> 20, 1, 1,
                                             255);
      Engine_ActorSetChildValue(14, 6);
      Engine_TaskWait(1);
      Object_GetById(9)->collisionFlags |= 8;
      if (Engine_GameFlagIsSet(0x30b) == 0) {
        Engine_ActorSetChildValue(18, 15);
        Engine_ActorSetSpriteFlags(Object_GetById(18), 0);
        if (Engine_GameFlagIsSet(0x30a) != 0) {
          Actor_SetPosition(22, PIXELS(744), PIXELS(504));
          Actor_SetPosition(18, PIXELS(744), PIXELS(504));
        }
#if TBS_PATCH_2
      } else {
        Engine_ActorSetPosition(18, 0, 0);
#endif
      }
      MogallForest_ThirdArea_UpdateSouthObstacle();
      break;

    case 12:
    case 13:
      Actor_SetActionScript(18, gMogallForest_ActorScript);
      if (Engine_GameFlagIsSet(0x893) == 0) {
        break;
      }
      if (Engine_GameFlagIsSet(0x89e) == 0) {
        break;
      }
      Engine_GameFlagSet(0x88f);
      break;

    case 15:
      Engine_GameFlagSet(0x89e);
      break;
    }
  }
  return 0;
}

// Shared source is included here in ROM order.
#include "common/object_orbit.h"

// Pixel step by direction: X in the upper halfword, Z in the lower.
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

u32 gMogallForest_SceneTable[] = {
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
    { 0xbf, 0x300, (s32)gMogallForest_ActorScript,
        PIXELS(136), 0, PIXELS(120), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xbf, 0x301, (s32)gMogallForest_ActorScript,
        PIXELS(376), 0, PIXELS(432), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xbf, 0x302, (s32)gMogallForest_ActorScript,
        PIXELS(504), 0, PIXELS(312), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x16, 0x300, (s32)gMogallForestPlacements1_1,
        PIXELS(136), 0, PIXELS(120), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x16, 0x301, (s32)gMogallForestPlacements1_1,
        PIXELS(376), 0, PIXELS(432), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x16, 0x302, (s32)gMogallForestPlacements1_1,
        PIXELS(504), 0, PIXELS(312), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x16, 0xfd4, 7,
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
    { 0xbf, 0x303, (s32)gMogallForest_ActorScript,
        PIXELS(360), 0, PIXELS(80), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xbf, 0x304, (s32)gMogallForest_ActorScript,
        PIXELS(488), 0, PIXELS(80), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xbf, 0x306, (s32)gMogallForest_ActorScript,
        PIXELS(504), 0, PIXELS(480), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x16, 0x303, (s32)gMogallForestPlacements1_1,
        PIXELS(360), 0, PIXELS(80), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x16, 0x304, (s32)gMogallForestPlacements1_1,
        PIXELS(488), 0, PIXELS(80), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x16, 0x306, (s32)gMogallForestPlacements1_1,
        PIXELS(504), 0, PIXELS(480), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x5a, 0x32, ACTOR_STAND,
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
    { 0xbf, 0x307, (s32)gMogallForest_ActorScript,
        PIXELS(488), 0, PIXELS(88), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xbf, 0x308, (s32)gMogallForest_ActorScript,
        PIXELS(424), 0, PIXELS(120), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xbf, 0x309, (s32)gMogallForest_ActorScript,
        PIXELS(360), 0, PIXELS(80), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xbf, MOGALL_FOREST_EAST_ACTOR_CONDITION, (s32)gMogallForest_ActorScript,
        PIXELS(744), 0, PIXELS(400), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x16, 0x307, (s32)gMogallForestPlacements1_1,
        PIXELS(488), 0, PIXELS(88), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x16, 0x308, (s32)gMogallForestPlacements1_1,
        PIXELS(424), 0, PIXELS(120), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x16, 0x309, (s32)gMogallForestPlacements1_1,
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
    { 0x202, 20, CONDITION_ALWAYS, EVENT_SCRIPT(MogallForest_FirstArea_PushLog) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 25, CONDITION_ALWAYS, EVENT_SCRIPT(MogallForest_FirstArea_JumpNorth) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 26, CONDITION_ALWAYS, EVENT_SCRIPT(MogallForest_FirstArea_JumpSouth) },
    { SEARCH(SEARCH_UNNAMED), 27, CONDITION_ALWAYS, EVENT_SCRIPT(MogallForest_ReadBrokenSign) },
    { PSYNERGY_ON_ACTOR(ABILITY_FORCE), 13, 0x300, EVENT_SCRIPT(MogallForest_FirstArea_RevealNorthGuide) },
    { PSYNERGY_ON_ACTOR(ABILITY_FORCE), 14, 0x301, EVENT_SCRIPT(MogallForest_FirstArea_RevealHostileCreature) },
    { PSYNERGY_ON_ACTOR(ABILITY_FORCE), 15, 0x302, EVENT_SCRIPT(MogallForest_FirstArea_RevealEastGuide) },
    { PSYNERGY_ON_ACTOR(ABILITY_CATCH), 16, 0xfd4, EVENT_SCRIPT(MogallForest_CatchNut) },
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
    { 0x202, 20, CONDITION_ALWAYS, EVENT_SCRIPT(MogallForest_SecondArea_PushLog) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 11, CONDITION_ALWAYS, EVENT_SCRIPT(MogallForest_SecondArea_UpdateMovableObstacle) },
    { PSYNERGY_ON_ACTOR(ABILITY_FORCE), 15, 0x303, EVENT_SCRIPT(MogallForest_SecondArea_RevealNorthGuide) },
    { PSYNERGY_ON_ACTOR(ABILITY_FORCE), 16, 0x304, EVENT_SCRIPT(MogallForest_SecondArea_RevealEastGuide) },
    { PSYNERGY_ON_ACTOR(ABILITY_FORCE), 17, 0x305, EVENT_SCRIPT(MogallForest_SecondArea_RelocateSouthGuide) },
    { PSYNERGY_ON_ACTOR(ABILITY_FORCE), 17, 0x306, EVENT_SCRIPT(MogallForest_SecondArea_RevealWestGuide) },
    { SEARCH(SEARCH_CHEST), 100, 0xf1b, FIND_ITEM(0xc1) },
    { EVENT_TALK, 18, CONDITION_ALWAYS, EVENT_SCRIPT(MogallForest_StartDjinnBattle) },
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
    {0x202, 20, CONDITION_ALWAYS, EVENT_SCRIPT(MogallForest_ThirdArea_PushLog)},
    {TOUCH_WALKING_INTO(FACING_WEST), 25, CONDITION_ALWAYS,
     EVENT_SCRIPT(MogallForest_ThirdArea_JumpWest)},
    {EVENT_TOUCH, 22, 0x89d,
     EVENT_SCRIPT(MogallForest_FollowCreatureTowardFinalEncounter)},
    {EVENT_TOUCH, 23, 0x89e,
     EVENT_SCRIPT(MogallForest_StartFinalCreatureEncounter)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 13, CONDITION_ALWAYS,
     EVENT_SCRIPT(MogallForest_ThirdArea_UpdateNorthObstacle)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 14, 0x214,
     EVENT_SCRIPT(MogallForest_ThirdArea_UpdateSouthObstacle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FORCE), 19, 0x307,
     EVENT_SCRIPT(MogallForest_ThirdArea_RevealHostileCreature)},
    {PSYNERGY_ON_ACTOR(ABILITY_FORCE), 20, 0x308,
     EVENT_SCRIPT(MogallForest_ThirdArea_RevealCentralGuide)},
    {PSYNERGY_ON_ACTOR(ABILITY_FORCE), 21, 0x309,
     EVENT_SCRIPT(MogallForest_ThirdArea_RevealWestGuide)},
    {PSYNERGY_ON_ACTOR(ABILITY_FORCE), 22, 0x30a,
     EVENT_SCRIPT(MogallForest_ThirdArea_RelocateEastGuide)},
    {PSYNERGY_ON_ACTOR(ABILITY_FORCE), 22, 0x30b,
     EVENT_SCRIPT(MogallForest_ThirdArea_RevealSouthGuide)},
    {SEARCH(SEARCH_CHEST), 100, 0xf1a, FIND_ITEM(0x5d)},
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
    {SEARCH(SEARCH_UNNAMED), 100, 0x350, FIND_PSYNERGY_STONE},
    {SCENE_EVENTS_END},
};
