#include "game_state.h"
#include "version.h"
#include "global_cells.h"
#include "scene_ids.h"
#include "unit/import.h"
#include "unit/scene/crossbone_platform.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "unit/game/game.h"
#include "unit/runtime/runtime.h"
#include "unit/scene/common/colosso_item_icon.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "edition.h"
#include "field_event.h"
#include "field_service.h"
#include "field_scene.h"
#include "staged_actor.h"
#include "select_overlay_data_by_runtime_selector.h"
#include "io_reg.h"
#include "resource.h"
#include "field_obj.h"
#include "animspr.h"
#include "mem.h"
#include "dma.h"
#include "callback_scheduler.h"
#include "call.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"
#include "iowrite.h"
#include "field_effect.h"
#include "fixed_math.h"

#if TBS_PATCH_1
#define CROSSBONE_PURSUER_STEP_Z 16
#define CROSSBONE_LEADER_STEP_Z 24
#else
#define CROSSBONE_PURSUER_STEP_Z 24
#define CROSSBONE_LEADER_STEP_Z 16
#endif

// Forward declarations.
static void CrossbonePlatform_ClimbOrMoveFirstBlock(void);
static void CrossbonePlatform_FollowSecondBlock(void);
static void CrossbonePlatform_PositionSecondBlockMapObject(void);
static void CrossbonePlatform_InitializeScene1(void);
static void CrossbonePlatform_InitializeScene2(void);
static void CrossbonePlatform_InitializeScene3(void);

enum { ITEM_RED_KEY = 243, ITEM_BLUE_KEY = 244 };

enum { ACTOR_DJINNI = 15 };

enum { PASSAGE_INDEX_MASK = 0xff, PASSAGE_PLAY_OPENING = 0x100 };

// Publish the next raised-event trigger.
static __inline__ void SceneState_StoreStep(s16 *field, s32 step) {
  *field = step;
}

// Lift the first block and clear the terrain beneath its column.
static __inline__ void LiftBlock(s32 column, u8 motionFlags) {
  {
    s32 destinationColumn = column;
    Map_CopyCellAttributes(61, 36, 1, 1, destinationColumn, 42);
  }
  Object_GetById(8)->motionFlags = motionFlags;
  Object_GetById(8)->y.fixed = PIXELS(32);
}

// Shared source is included here in ROM order.
#include "common/object_staged_actor.h"

// Add the caught Djinni to the party.
static void CrossbonePlatform_JoinDjinni(void) {
  Djinn_JoinParty(ACTOR_DJINNI, 0, 6);
}

// Unused scene callback with no behavior.
UNUSED static void Resource3b4_EmptyHookA(void) {}

// Select entrances for the current platform room.
const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 selector = gGameState.scene;

  if (selector == (s32)&SceneId_CrossbonePlatform1) {
    return gCrossbonePlatformEntrances1;
  }
  if (selector == (s32)&SceneId_CrossbonePlatform2) {
    return gCrossbonePlatformEntrances2;
  }
  if (selector == (s32)&SceneId_CrossbonePlatform3) {
    return gCrossbonePlatformEntrances3;
  }
  return gCrossbonePlatformEntrancesOther;
}

// Return the scene protocol default.
s32 SceneData_ReturnZero(void) { return 0; }

// Return the shared exit table.
u32 *SceneData_GetSceneTable(void) { return gCrossbonePlatform_SceneTable; }

// Select actor placements for the current platform room.
const struct ScenePlacement *Scene_GetPlacements(void) {
  s16 selector = gGameState.scene;

  if (selector == (s32)&SceneId_CrossbonePlatform1) {
    return gCrossbonePlatformPlacements1;
  }
  if (selector == (s32)&SceneId_CrossbonePlatform2) {
    return gCrossbonePlatformPlacements2;
  }
  if (selector == (s32)&SceneId_CrossbonePlatform3) {
    return gCrossbonePlatformPlacements3;
  }
  return gCrossbonePlatformPlacementsOther;
}

// Leave Halt interactions with the pursuers unchanged.
static void Resource3b4_EmptyHookB(void) {}

// Turn the Djinni toward the party leader each frame.
// UB: calling this s32 function through a void update hook is incompatible.
// Plain void changes 38 to 36 bytes in tbs-en.
#ifdef UBFIX
static void
CrossbonePlatform_FaceDjinniTowardLeader(union FieldObject *fieldObject) {
#else
static s32
CrossbonePlatform_FaceDjinniTowardLeader(union FieldObject *fieldObject) {
#endif
  struct FieldActor *actor = &fieldObject->actor;
  struct FieldActor *leader = Object_GetById(0);

  actor->facing = ArcTan2(leader->z.fixed - actor->z.fixed,
                          leader->x.fixed - actor->x.fixed);
#ifdef UBFIX
  return;
#else
  return 0;
#endif
}

// Make the Djinni jump away on the first approach.
static void CrossbonePlatform_DjinniFirstEscape(void) {
  struct FieldActor *djinni;

  if (GameFlag_IsSet(0x9c8) == 0) {
    GameFlag_Set(0x9c8);
    Engine_EventBegin();
    Camera_SetSpeed(PIXELS(2), PIXELS(1) / 4);
    Engine_CameraMoveToActor(ACTOR_DJINNI, 1);
    Engine_CameraWaitForMove();
    Actor_FaceDirection(ACTOR_DJINNI, 0, 20);
    Actor_SetAttachedEffect(ACTOR_DJINNI, 0x102);
    Engine_ActorRunRepeatedMotion(15, 2);
    Engine_EventWait(20);
    Actor_SetSpeed(ACTOR_DJINNI, PIXELS(1), PIXELS(1) / 2);
    Event_PlayCue(152);
    djinni = Object_GetById(ACTOR_DJINNI);
    djinni->velocityY = PIXELS(8);
    Actor_WalkToAndWait(ACTOR_DJINNI, 584, 680);
    Actor_FaceDirection(ACTOR_DJINNI, FACING_SOUTH, 20);
    Engine_EventEnd();
  }
}

// Send the Djinni across the room and start its stretched pose.
static void CrossbonePlatform_DjinniLongEscape(void) {
  struct FieldActor *spinningDjinni;
  s32 escapeComplete;
  struct FieldActor *jumpingDjinni;

  if (GameFlag_IsSet(0x9c8) != 0) {
    escapeComplete = GameFlag_IsSet(0x9c9);
    if (escapeComplete == 0) {
      GameFlag_Set(0x9c9);
      Engine_EventBegin();

      // Focus on the Djinni before its next jump away.
      Camera_SetSpeed(PIXELS(2), PIXELS(1) / 4);
      Engine_CameraMoveToActor(ACTOR_DJINNI, 1);
      Engine_CameraWaitForMove();
      Actor_FaceDirection(ACTOR_DJINNI, FACING_SOUTH, 20);
      Actor_SetAttachedEffect(ACTOR_DJINNI, 0x102);
      Engine_ActorRunRepeatedMotion(15, 2);
      Engine_EventWait(20);
      Actor_SetSpeed(ACTOR_DJINNI, PIXELS(1), PIXELS(1) / 2);
      Event_PlayCue(152);
      jumpingDjinni = Object_GetById(ACTOR_DJINNI);
      jumpingDjinni->velocityY = PIXELS(10);
      Actor_WalkToAndWait(ACTOR_DJINNI, 584, 664);
      Actor_FaceDirection(ACTOR_DJINNI, FACING_SOUTH, 20);
      Actor_SetAttachedEffect(ACTOR_DJINNI, 0x102);
      Engine_EventWait(30);

      // Dash across the platforms, then shake the room on landing.
      Actor_SetSpeed(ACTOR_DJINNI, PIXELS(8), PIXELS(1) / 4);
      Actor_WalkToAndWait(ACTOR_DJINNI, 664, 664);
      Actor_WalkToAndWait(ACTOR_DJINNI, 744, 664);
      Actor_WalkToAndWait(ACTOR_DJINNI, 824, 664);
      Engine_EventWait(10);
      Event_PlayCue(208);
      Map_SetShake(PIXELS(4), PIXELS(2), Q16_ONE);
      Engine_EventWait(20);
      Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
      Engine_EventWait(30);

      // Leave the Djinni tilted while its scale animation continues.
      Actor_SetPosition(ACTOR_DJINNI, PIXELS(888), PIXELS(664));
      spinningDjinni = Object_GetById(ACTOR_DJINNI);
      spinningDjinni->sprite->rotation = 0xf800;
      spinningDjinni->facing = 0;
      ObjectDispatch_ApplyValueToChildren(spinningDjinni, 0);
      Engine_ObjectSetScript(spinningDjinni,
                             gCrossbonePlatform_ActorFifteenMotionScript);
      Engine_EventEnd();
    }
  }
}

// Bring the Djinni to its final position and have it watch the leader.
static void CrossbonePlatform_DjinniFinalEscape(void) {
  struct FieldActor *djinni;

  if (GameFlag_IsSet(0x9c9) != 0 && GameFlag_IsSet(0x9ca) == 0) {
    GameFlag_Set(0x9ca);
    Engine_EventBegin();

    // Restore the sprite before the last jump and winding escape path.
    djinni = Object_GetById(ACTOR_DJINNI);
    djinni->sprite->rotation = 0;
    ObjectDispatch_ApplyValueToChildren(djinni, 16);
    Event_PlayCue(152);
    djinni = Object_GetById(ACTOR_DJINNI);
    djinni->velocityY = PIXELS(8);
    Actor_FaceDirection(ACTOR_DJINNI, FACING_WEST, 30);
    Actor_SetAttachedEffect(ACTOR_DJINNI, 0x102);
    Engine_ActorRunRepeatedMotion(15, 2);
    Engine_EventWait(20);
    Actor_SetSpeed(ACTOR_DJINNI, PIXELS(1), PIXELS(1) / 2);
    Event_PlayCue(152);
    djinni = Object_GetById(ACTOR_DJINNI);
    djinni->velocityY = PIXELS(4);
    Actor_WalkToAndWait(ACTOR_DJINNI, 880, 680);
    Engine_EventWait(10);
    Actor_SetAttachedEffect(ACTOR_DJINNI, 0x101);
    Actor_SetSpeed(ACTOR_DJINNI, PIXELS(2), PIXELS(1));
    Actor_WalkToAndWait(ACTOR_DJINNI, 880, 696);
    Actor_WalkToAndWait(ACTOR_DJINNI, 882, 704);
    Actor_WalkToAndWait(ACTOR_DJINNI, 880, 712);
    Actor_WalkToAndWait(ACTOR_DJINNI, 878, 720);
    Actor_WalkToAndWait(ACTOR_DJINNI, 880, 728);
    Actor_WalkToAndWait(ACTOR_DJINNI, 882, 736);
    Actor_WalkToAndWait(ACTOR_DJINNI, 880, 744);
    Actor_WalkToAndWait(ACTOR_DJINNI, 878, 752);
    Actor_WalkToAndWait(ACTOR_DJINNI, 880, 760);
    Actor_SetPosition(ACTOR_DJINNI, PIXELS(856), PIXELS(824));
    Engine_EventWait(10);
    Actor_FaceDirection(ACTOR_DJINNI, FACING_NORTH, 20);
    Actor_SetAttachedEffect(ACTOR_DJINNI, 0x100);
    djinni = Object_GetById(ACTOR_DJINNI);
#ifdef UBFIX
    djinni->update = CrossbonePlatform_FaceDjinniTowardLeader;
#else
    djinni->update =
        (void (*)(union FieldObject *))CrossbonePlatform_FaceDjinniTowardLeader;
#endif
    Engine_EventEnd();
  }
}

// Move the Djinni around the target ring at the speed of the leader.
static void CrossbonePlatform_MoveDjinniToTouchedTarget(void) {
  struct FieldActor *djinni;
  struct FieldActor *leader;
  s32 targetByteOffset;
  const u8 *targets;
  s32 zByteOffset;
  struct EventWork *work;

  work = gEventWork;
  if (Engine_GameFlagIsSet(0x9ca) != 0) {
    if (gGameState.pausedObject != ACTOR_DJINNI) {
      targetByteOffset = work->touchedTrigger;
      djinni = Object_GetById(ACTOR_DJINNI);
      leader = Object_GetById(0);
      djinni->speed = leader->speed;
      djinni = Object_GetById(ACTOR_DJINNI);
      leader = Object_GetById(0);
      djinni->acceleration = leader->speed;
      targetByteOffset -= 30;
      targets = (const u8 *)gCrossbonePlatform_ActorFifteenWalkTargets;
      // The table stores consecutive signed X/Z pixel pairs.
      targetByteOffset *= 2 * sizeof(s32);
      zByteOffset = targetByteOffset + sizeof(s32);
      Engine_ActorWalkTo(15, *(const s32 *)(targets + targetByteOffset),
                         *(const s32 *)(targets + zByteOffset));
    }
  }
}

// Stop the Djinni animation when Halt reaches it.
static void CrossbonePlatform_HaltDjinni(void) {
  Engine_EventBegin();
  Engine_ActorSetAnimation(15, 0);
  Engine_EventEnd();
}

// Select events for the current platform room.
const struct SceneEvent *Scene_GetEvents(void) {
  s16 selector = gGameState.scene;

  if (selector == (s32)&SceneId_CrossbonePlatform1) {
    return gCrossbonePlatformEvents1;
  }
  if (selector == (s32)&SceneId_CrossbonePlatform2) {
    return gCrossbonePlatformEvents2;
  }
  if (selector == (s32)&SceneId_CrossbonePlatform3) {
    return gCrossbonePlatformEvents3;
  }
  return gCrossbonePlatformEventsOther;
}

// Recheck the obstacle directly ahead of the leader.
static void CrossbonePlatform_CheckLeaderAhead(void) {
  Engine_LeaderCheckAhead();
}

// Open the selected blue-key passage, optionally playing its opening.
static void CrossbonePlatform_OpenPassage(s32 passageInfo) {
  s32 columnOffset = (passageInfo & PASSAGE_INDEX_MASK) * 4;
  s32 tileColumn = columnOffset + 77;
  s32 collisionColumn = columnOffset + 13;

  if (passageInfo & PASSAGE_PLAY_OPENING) {
    Engine_AudioPlayCue(157);
    Map_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
    Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
    Map_CopyCells(79, 29, 1, 3, tileColumn, 40);
    Engine_TaskWait(40);
  }
  Map_CopyCells(80, 29, 1, 3, tileColumn, 40);
  Map_CopyCellAttributes(collisionColumn, 40, 1, 1, collisionColumn, 41);
  Map_CopyCellAttributes(collisionColumn, 40, 1, 1, collisionColumn, 42);
}

// Open the red-key passage, optionally playing its opening.
static void CrossbonePlatform_OpenPlatformPassage(s32 passageInfo) {
  if ((passageInfo & PASSAGE_PLAY_OPENING) != 0) {
    Event_PlayCue(157);
    Map_SetShake(PIXELS(2), PIXELS(2), Q16_ONE);
    Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
    Map_CopyCells(84, 29, 1, 3, 70, 49);
    Engine_TaskWait(60);
  }
  Map_CopyCells(85, 29, 1, 3, 70, 49);
  Map_CopyCellAttributes(6, 49, 1, 1, 6, 50);
  Map_CopyCellAttributes(6, 49, 1, 1, 6, 51);
}

// Consume the red key when the leader faces the locked passage.
static void CrossbonePlatform_UseRedKey(void) {
  struct FieldActor *leader;

  leader = Object_GetById(0);
  if (leader->facing == FACING_NORTH) {
    if (GameFlag_IsSet(0x9c4) == 0) {
      if (PartyInventory_FindOwner(ITEM_RED_KEY) != -1) {
        GameFlag_Set(0x9c4);
        CrossbonePlatform_OpenPlatformPassage(PASSAGE_PLAY_OPENING);
        PartyInventory_Remove(ITEM_RED_KEY);
      }
    }
  }
}

// Consume a blue key and record which door was opened.
static void CrossbonePlatform_UseBlueKey(s32 doorIndex) {
  struct FieldActor *leader = Object_GetById(0);
  s32 flag;

  if (leader->facing != FACING_NORTH) {
    return;
  }
  flag = doorIndex + 2496;
  if (Engine_GameFlagIsSet(flag) != 0) {
    return;
  }
  if (PartyInventory_FindOwner(ITEM_BLUE_KEY) == -1) {
    return;
  }
  Engine_GameFlagSet(flag);
  CrossbonePlatform_OpenPassage(PASSAGE_PLAY_OPENING | doorIndex);
  PartyInventory_Remove(ITEM_BLUE_KEY);
}

// Try the first blue-key door.
static void CrossbonePlatform_OpenFirstBlueDoor(void) {
  CrossbonePlatform_UseBlueKey(0);
}

// Try the second blue-key door.
static void CrossbonePlatform_OpenSecondBlueDoor(void) {
  CrossbonePlatform_UseBlueKey(1);
}

// Try the third blue-key door and periodically recheck its other side.
static void CrossbonePlatform_OpenThirdBlueDoor(void) {
  struct FieldActor *leader;
  s16 *blockedSteps;
  s32 resetSteps;

  CrossbonePlatform_UseBlueKey(2);
  leader = Object_GetById(0);
  if (leader->facing == FACING_SOUTH) {
    blockedSteps = &gEventWork->blockedSteps;
    if (*blockedSteps > 12) {
      Engine_LeaderCheckAhead();
      resetSteps = 0;
      *blockedSteps = resetSteps;
    }
  }
}

// Try the fourth blue-key door and periodically recheck its other side.
static void CrossbonePlatform_OpenFourthBlueDoor(void) {
  struct FieldActor *leader;
  s16 *blockedSteps;
  s32 resetSteps;

  CrossbonePlatform_UseBlueKey(3);
  leader = Object_GetById(0);
  if (leader->facing == FACING_SOUTH) {
    blockedSteps = &gEventWork->blockedSteps;
    if (*blockedSteps > 12) {
      Engine_LeaderCheckAhead();
      resetSteps = 0;
      *blockedSteps = resetSteps;
    }
  }
}

// Reveal the red-key tile and its uncollected actor.
static void CrossbonePlatform_ShowRedKey(void) {
  s32 keyActorId = 12;
  s32 x = PIXELS(408);
  s32 z = PIXELS(776);

  Map_CopyCells(25, 45, 1, 2, 25, 48);
  if (Engine_GameFlagIsSet(0xeeb) == 0)
    Engine_ActorSetPosition(keyActorId, x, z);
  Engine_EventWait(1);
}

// Restore the unrevealed tile and move the red key out of view.
static void CrossbonePlatform_HideRedKey(void) {
  Map_CopyCells(24, 48, 1, 2, 25, 48);
  Actor_SetPosition(12, PIXELS(8), PIXELS(8));
}

// Collect the selected key, hide its actor and remember it was taken.
static inline void CrossbonePlatform_CollectKey(s32 actorId, s32 itemId,
                                                s32 collectedFlag) {
  Engine_ItemShowFound(itemId, 3);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
  Engine_PartyGiveItem(itemId, 0);
  Engine_ActorSetPosition(actorId, 0, 0);
  Engine_GameFlagSet(collectedFlag);
}

// Collect the first blue key with Catch.
static void CrossbonePlatform_CatchFirstBlueKey(void) {
  CrossbonePlatform_CollectKey(8, ITEM_BLUE_KEY, 0xee7);
}

// Collect the second blue key with Catch.
static void CrossbonePlatform_CatchSecondBlueKey(void) {
  CrossbonePlatform_CollectKey(9, ITEM_BLUE_KEY, 0xee8);
}

// Collect the third blue key with Catch.
static void CrossbonePlatform_CatchThirdBlueKey(void) {
  CrossbonePlatform_CollectKey(10, ITEM_BLUE_KEY, 0xee9);
}

// Collect the fourth blue key with Catch.
static void CrossbonePlatform_CatchFourthBlueKey(void) {
  CrossbonePlatform_CollectKey(11, ITEM_BLUE_KEY, 0xeea);
}

// Collect the red key with Catch.
static void CrossbonePlatform_CatchRedKey(void) {
  CrossbonePlatform_CollectKey(12, ITEM_RED_KEY, 0xeeb);
}

// Start the first chase when an uncloaked leader enters its area.
static void CrossbonePlatform_CheckFirstPursuerTrigger(void) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  s32 column = leader->x.fixed / PIXELS(16);
  s32 row = leader->z.fixed / PIXELS(16);
  struct EventWork *event = gEventWork;

  if (!GameFlag_IsSet(0x220) && gGameState.cloaked == 0 &&
      gGameState.pausedObject != 8 && (u32)(column - 21) <= 2 && row >= 10 &&
      row <= 11) {
    Engine_GameFlagSet(0x220);
    event->raisedTrigger = 91;
  }
}

// Let the first pursuer follow the leader, then return it home.
static void CrossbonePlatform_DodgeFirstPursuer(void) {
  struct FieldActor *leader;

  Engine_EventBegin();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(19) / 10, PIXELS(19) / 20);
  Actor_SetSpeed(8, PIXELS(19) / 10, PIXELS(19) / 20);
  Event_PlayCue(188);
  leader = Object_GetById(0);
  if (leader != 0) {
    Actor_SetDestination(8, leader->x.part.pixel, leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(8);
  Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, 24);
  Engine_EventWait(4);
  Event_PlayCue(188);
  Actor_SetDestinationOffset(8, 0, CROSSBONE_PURSUER_STEP_Z);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Actor_SetDestination(8, 360, 152);
  Engine_ActorWaitForMove(8);
  Engine_EventEnd();
  GameFlag_Clear(0x220);
}

// Start the second chase when the leader enters column ten.
static void CrossbonePlatform_CheckSecondPursuerTrigger(void) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  s32 column = leader->x.fixed / PIXELS(16);
  s32 row = leader->z.fixed / PIXELS(16);
  struct EventWork *work = gEventWork;

  if (GameFlag_IsSet(0x220) == 0 && gGameState.cloaked == 0 &&
      gGameState.pausedObject != 9 && column == 10 && (u32)(row - 16) <= 2) {
    GameFlag_Set(0x220);
    SceneState_StoreStep(&work->raisedTrigger, 92);
  }
}

// Let the second pursuer follow the leader, then return it home.
static void CrossbonePlatform_DodgeSecondPursuer(void) {
  struct FieldActor *leader;

  Engine_EventBegin();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(17) / 10, PIXELS(17) / 20);
  Actor_SetSpeed(9, PIXELS(17) / 10, PIXELS(17) / 20);
  Event_PlayCue(188);
  leader = Object_GetById(0);
  if (leader != 0) {
    Actor_SetDestination(9, leader->x.part.pixel, leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(9);
  Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, CROSSBONE_LEADER_STEP_Z);
  Event_PlayCue(188);
  Engine_EventWait(4);
  Actor_SetDestinationOffset(9, 0, 16);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Actor_SetDestination(9, 168, 264);
  Engine_ActorWaitForMove(9);
  Engine_EventEnd();
  GameFlag_Clear(0x220);
}

// Move a pursuer to the leader column and remember its last column.
static void CrossbonePlatform_TrackPursuerColumn(s32 pursuerIndex) {

  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  s32 column = leader->x.fixed / PIXELS(16);
  s32 row = leader->z.fixed / PIXELS(16);
  s32 pursuerId = pursuerIndex + 10;

  if (gGameState.pausedObject == pursuerId)
    return;
  if (column == gCrossbonePlatform_SlotColumns[pursuerIndex])
    return;

  Engine_ActorSetSpeed(pursuerId, PIXELS(9) / 2, PIXELS(9) / 4);
  Engine_AudioPlayCue(188);
  Engine_ActorSetDestination(pursuerId, (column << 4) + 8, 360);

  gCrossbonePlatform_SlotColumns[pursuerIndex] = column;

  if (row <= 22) {
    Engine_ActorSetDestinationOffset(ACTOR_PARTY_LEADER, 0, 8);
  }
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
}

// Align the third actor with the leader column.
static void CrossbonePlatform_TrackThirdPursuerColumn(void) {
  CrossbonePlatform_TrackPursuerColumn(0);
}

// Align the fourth actor with the leader column.
static void CrossbonePlatform_TrackFourthPursuerColumn(void) {
  CrossbonePlatform_TrackPursuerColumn(1);
}

// Unused callback that aligns the fifth actor with the leader column.
UNUSED static void CrossbonePlatform_TrackFifthPursuerColumn(void) {
  CrossbonePlatform_TrackPursuerColumn(2);
}

// Let the third pursuer follow the leader, then return it home.
static void CrossbonePlatform_DodgeThirdPursuer(void) {
  struct FieldActor *leader;

  if (gGameState.pausedObject != 10) {
    Engine_EventBegin();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(17) / 10, PIXELS(17) / 20);
    Actor_SetSpeed(10, PIXELS(17) / 10, PIXELS(17) / 20);
    Event_PlayCue(188);
    leader = Object_GetById(0);
    if (leader != 0) {
      Actor_SetDestination(10, leader->x.part.pixel, leader->z.part.pixel);
    }
    Engine_ActorWaitForMove(10);
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, CROSSBONE_LEADER_STEP_Z);
    Engine_EventWait(4);
    Event_PlayCue(188);
    Actor_SetDestinationOffset(10, 0, 16);
    Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
    Actor_SetDestination(10, 264, 360);
    Engine_ActorWaitForMove(10);
    Engine_EventWait(10);
    Engine_EventEnd();
  }
}

// Align the fourth pursuer before sending it after the leader.
static void CrossbonePlatform_DodgeFourthPursuer(void) {
  struct FieldActor *leader;
  s32 leaderColumn;

  if (gGameState.pausedObject != 11) {
    Engine_EventBegin();
    Actor_SetSpeed(0, PIXELS(17) / 10, PIXELS(17) / 20);
    Engine_ActorSetSpeed(11, PIXELS(17) / 10, PIXELS(17) / 20);
    Engine_AudioPlayCue(188);
    leaderColumn = Object_GetById(0)->x.fixed / PIXELS(16);
    if (leaderColumn > Object_GetById(11)->x.fixed / PIXELS(16)) {
      Engine_ActorSetDestinationOffset(11, 8, 0);
    }
    leaderColumn = Object_GetById(0)->x.fixed / PIXELS(16);
    if (leaderColumn < Object_GetById(11)->x.fixed / PIXELS(16)) {
      FieldObjectMotion_OffsetPositionAndResetMotion(11, -8, 0);
    }
    Engine_ActorWaitForMove(11);
    leader = Object_GetById(0);
    if (leader != 0) {
      Engine_ActorSetDestination(11, leader->x.part.pixel,
                                 leader->z.part.pixel);
    }
    Engine_ActorWaitForMove(11);
    Engine_ActorSetDestinationOffset(0, 0, CROSSBONE_LEADER_STEP_Z);
    Engine_EventWait(4);
    Engine_AudioPlayCue(188);
    Engine_ActorSetDestinationOffset(11, 0, 16);
    Engine_ActorWaitForMove(0);
    Engine_ActorSetDestination(11, 344, 360);
    Engine_ActorWaitForMove(11);
    Engine_EventWait(10);
    Engine_EventEnd();
  }
}

// Start the fifth chase when the leader enters column nineteen.
static void CrossbonePlatform_CheckFifthPursuerTrigger(void) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  s32 column = leader->x.fixed / PIXELS(16);
  s32 row = leader->z.fixed / PIXELS(16);
  struct EventWork *work = gEventWork;
  struct GameState *state = &gGameState;

  if (state->pausedObject != 12 && GameFlag_IsSet(0x220) == 0 &&
      state->cloaked == 0 && column == 19 && (u32)(row - 15) <= 1) {
    GameFlag_Set(0x220);
    SceneState_StoreStep(&work->raisedTrigger, 96);
  }
}

// Let the fifth pursuer follow the leader, then return it home.
static void CrossbonePlatform_DodgeFifthPursuer(void) {
  struct FieldActor *leader;

  Engine_EventBegin();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(17) / 10, PIXELS(17) / 20);
  Actor_SetSpeed(12, PIXELS(17) / 10, PIXELS(17) / 20);
  Event_PlayCue(188);
  leader = Object_GetById(0);
  if (leader != 0) {
    Actor_SetDestination(12, leader->x.part.pixel, leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(12);
  Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, CROSSBONE_LEADER_STEP_Z);
  Event_PlayCue(188);
  Actor_SetDestinationOffset(12, 0, 16);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Actor_SetDestination(12, 312, 232);
  Engine_ActorWaitForMove(12);
  Engine_EventEnd();
  GameFlag_Clear(0x220);
}

// Apply the blend used while Reveal exposes the room.
static void CrossbonePlatform_BeginRevealBlend(void) {
  u32 coefficient = 208;

  coefficient <<= 4;
  REG_BLDALPHA = coefficient;
}

// Restore the blend after Reveal ends.
static void CrossbonePlatform_EndRevealBlend(void) {
  u16 value = 0x0607;

  REG_BLDALPHA = value;
}

// Choose climbing or block movement from the leader direction.
static void CrossbonePlatform_UseLadder(void) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  u16 facing = leader->facing;

  if (facing == FACING_NORTH) {
    if (Engine_GameFlagIsSet(0x206) != 0) {
      {
        s32 destinationColumn = 45;
        s32 destinationRow = 43;
        Map_CopyCellAttributes(46, 43, 1, 1, destinationColumn, destinationRow);
      }
    }
    Engine_GameFlagClear(0x207);
    Field_ClimbUp();
  } else if (facing == FACING_SOUTH) {
    Field_ClimbDown();
  } else if (facing == 0) {
    if (Engine_GameFlagIsSet(0x206) != 0) {
      {
        s32 destinationColumn = 45;
        s32 destinationRow = 43;
        Map_CopyCellAttributes(58, 36, 1, 1, destinationColumn, destinationRow);
      }
    }
    Engine_GameFlagSet(0x207);
    Engine_LeaderCheckAhead();
  } else if (facing == FACING_WEST) {
    if (leader->y.fixed == 0) {
      CrossbonePlatform_ClimbOrMoveFirstBlock();
    } else {
      Engine_LeaderCheckAhead();
    }
  }
}

// Unused scene callback with no behavior.
UNUSED static void FieldScene_NoOpCallback(void) {}

// Stop the frozen puddle and update its supporting terrain.
static void CrossbonePlatform_FreezePuddle(void) {
  struct FieldActor *puddle = Object_GetById(13);

  Map_CopyCellAttributes(40, 54, 1, 1, 40, 55);
  if (puddle != 0) {
    u8 *motionFlags = &Object_GetById(13)->motionFlags;
    u8 *priorityFlags = &puddle->priorityFlags;

    *motionFlags = 0;
    *priorityFlags = 2;
  }
  Engine_GameFlagSet(512);
}

// Check the eastern obstacle or route the interaction to the first block.
static void CrossbonePlatform_ChooseClimbOrPush(void) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);

  if (leader->facing == 0) {
    Engine_LeaderCheckAhead();
  } else {
    CrossbonePlatform_ClimbOrMoveFirstBlock();
  }
}

// Restore the first block row before applying its current footprint.
static void CrossbonePlatform_RestoreActorEightTerrain(void) {
  Map_CopyCellAttributes(57, 42, 1, 1, 40, 42);
  Map_CopyCellAttributes(57, 42, 1, 1, 41, 42);
  Map_CopyCellAttributes(58, 42, 1, 1, 42, 42);
  Map_CopyCellAttributes(62, 37, 3, 1, 37, 42);

  Object_GetById(8)->motionFlags = 1;
}

// Restore support under the first block at the eastern cell.
static void ActorPresentation_SetSceneCell58AndMarkActorEight(void) {

  Map_CopyCellAttributes(58, 41, 1, 1, 42, 42);
  Object_GetById(8)->priorityFlags = 2;
}

// Restore support under the first block at the middle cell.
static void CrossbonePlatform_RestoreRightUnderfootCell(void) {

  Map_CopyCellAttributes(44, 42, 1, 1, 41, 42);
  Object_GetById(8)->priorityFlags = 2;
}

// Restore support under the first block at the western cell.
static void CrossbonePlatform_RestoreLeftUnderfootCell(void) {

  Map_CopyCellAttributes(39, 42, 1, 1, 40, 42);
  Object_GetById(8)->priorityFlags = 2;
}

// Rebuild the first block support cells from its height and column.
static void CrossbonePlatform_UpdateFirstBlockTerrain(void) {
  struct FieldActor *actor = Object_GetById(8);
  s32 x = actor->x.fixed;
  s32 height = actor->y.fixed;
  s32 column = x / PIXELS(16);
  u8 lowered;

  if (height == 0) {
    Object_GetById(8)->priorityFlags = 2;
  }
  CrossbonePlatform_RestoreActorEightTerrain();
  lowered = 0;
  Object_GetById(8)->motionFlags = 3;
  if (column == 40) {
    CrossbonePlatform_RestoreLeftUnderfootCell();
  } else if (column == 42) {
    ActorPresentation_SetSceneCell58AndMarkActorEight();
  } else if (column == 41) {
    CrossbonePlatform_RestoreRightUnderfootCell();
  } else if (column == 39) {
    LiftBlock(column, lowered);
  } else if (column == 38) {
    LiftBlock(column, lowered);
  } else if (column == 37) {
    LiftBlock(column, lowered);
  }
}

// Use the ladder at column thirty-eight or move the first block.
static void CrossbonePlatform_ClimbOrMoveFirstBlock(void) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  struct FieldActor *block = Object_GetById(8);
  s32 leaderColumn = leader->x.fixed / PIXELS(16);
  s32 blockColumn = block->x.fixed / PIXELS(16);

  if (leaderColumn == 38 && blockColumn != 38) {
    u16 facing = leader->facing;

    if (facing == FACING_NORTH) {
      Field_ClimbUp();
      return;
    }
    if (facing == FACING_SOUTH) {
      Field_ClimbDown();
      return;
    }
  }

  CrossbonePlatform_RestoreActorEightTerrain();
  StagedActor_AdvancePair();
  CrossbonePlatform_UpdateFirstBlockTerrain();
}

// Detach the second block follower and restore the ladder cell.
static void CrossbonePlatform_ReleaseActorNinePlatform(void) {
  Scheduler_RemoveCallback(CrossbonePlatform_FollowSecondBlock);
  Actor_SetPosition(14, 0, 0);
  if (GameFlag_IsSet(0x207) != 0) {
    Map_CopyCellAttributes(58, 36, 1, 1, 45, 43);
  } else {
    Map_CopyCellAttributes(46, 43, 1, 1, 45, 43);
  }
  CrossbonePlatform_PositionSecondBlockMapObject();
  GameFlag_Set(0x206);
}

// Release the second block platform when it reaches the ladder cell.
static void CrossbonePlatform_UpdateSecondBlockTerrain(void) {
  struct FieldActor *block = Object_GetById(9);
  s32 column = block->x.fixed / PIXELS(16);
  s32 row = block->z.fixed / PIXELS(16);

  if (column == 45 && row == 43) {
    CrossbonePlatform_ReleaseActorNinePlatform();
  }
}

// Move the second block and check whether its platform is released.
static void CrossbonePlatform_MoveSecondBlock(void) {
  StagedActor_AdvancePair();
  CrossbonePlatform_UpdateSecondBlockTerrain();
}

// Restore the terrain at both ends of the third block row.
static void CrossbonePlatform_RestoreThirdBlockTerrain(void) {

  Map_CopyCellAttributes(38, 56, 1, 1, 38, 55);
  Map_CopyCellAttributes(42, 56, 1, 1, 42, 55);
}

// Restore the third block support at the western end.
static void CrossbonePlatform_RestoreActorTenLeftCell(void) {

  Map_CopyCellAttributes(40, 54, 1, 1, 38, 55);
  Object_GetById(10)->motionFlags = 0;
}

// Restore the third block support at the eastern end.
static void CrossbonePlatform_RestoreActorTenRightCell(void) {

  Map_CopyCellAttributes(40, 54, 1, 1, 42, 55);
  Object_GetById(10)->motionFlags = 0;
}

// Rebuild support for the third block from its height and destination.
static void CrossbonePlatform_UpdateThirdBlockTerrain(void) {
  struct FieldActor *block = Object_GetById(10);
  s32 heightCells = block->y.fixed / PIXELS(16);
  s32 column = block->x.fixed / PIXELS(16);
  s32 row = block->z.fixed / PIXELS(16);

  if (!GameFlag_IsSet(0x300) && heightCells <= 2) {
    CrossbonePlatform_RestoreActorTenLeftCell();
    Engine_GameFlagSet(0x300);
  }
  if (row == 55) {
    if (column == 42) {
      CrossbonePlatform_RestoreActorTenRightCell();
    }
    if (column == 38) {
      CrossbonePlatform_RestoreActorTenLeftCell();
    }
  } else {
    CrossbonePlatform_RestoreThirdBlockTerrain();
  }
}

// Move the third block and refresh its supporting terrain.
static void CrossbonePlatform_MoveThirdBlock(void) {
  CrossbonePlatform_RestoreThirdBlockTerrain();
  StagedActor_AdvancePair();
  CrossbonePlatform_UpdateThirdBlockTerrain();
}

// Use the rising-object sequence from the south, otherwise move the block.
static void CrossbonePlatform_ChooseThirdBlockMotion(void) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);

  if (leader->facing == FACING_SOUTH) {
    FieldBattleFx_RunRisingObjectSequence(0, 6, 0);
  } else {
    CrossbonePlatform_MoveThirdBlock();
  }
}

// Clear the support row before repainting the fourth and fifth blocks.
static void CrossbonePlatform_RestoreFourthAndFifthBlockTerrain(void) {

  Map_CopyCellAttributes(48, 55, 1, 1, 49, 55);
  Map_CopyCellAttributes(48, 55, 1, 1, 50, 55);
  Map_CopyCellAttributes(48, 55, 1, 1, 51, 55);
  Map_CopyCellAttributes(48, 55, 1, 1, 52, 55);
}

// Place support cells beneath the fourth and fifth blocks.
static void ActorPresentation_RepaintCellsAtActorsElevenAndTwelve(void) {
  struct FieldActor *block;
  s32 column;

  CrossbonePlatform_RestoreFourthAndFifthBlockTerrain();

  block = Object_GetById(11);
  column = block->x.fixed / PIXELS(16);
  Map_CopyCellAttributes(53, 55, 1, 1, column, 55);
  block = Object_GetById(12);
  column = block->x.fixed / PIXELS(16);
  Map_CopyCellAttributes(53, 55, 1, 1, column, 55);
}

// Refresh the shared support row after the fourth block moves.
static void CrossbonePlatform_UpdateFourthBlockTerrain(void) {
  ActorPresentation_RepaintCellsAtActorsElevenAndTwelve();
}

// Move the fourth block and refresh its supporting terrain.
static void CrossbonePlatform_MoveFourthBlock(void) {
  CrossbonePlatform_RestoreFourthAndFifthBlockTerrain();
  StagedActor_AdvancePair();
  CrossbonePlatform_UpdateFourthBlockTerrain();
}

// Refresh the shared support row after the fifth block moves.
static void CrossbonePlatform_UpdateFifthBlockTerrain(void) {
  ActorPresentation_RepaintCellsAtActorsElevenAndTwelve();
}

// Unused callback that moves the fifth block and refreshes its terrain.
UNUSED static void CrossbonePlatform_MoveFifthBlock(void) {
  CrossbonePlatform_RestoreFourthAndFifthBlockTerrain();
  StagedActor_AdvancePair();
  CrossbonePlatform_UpdateFifthBlockTerrain();
}

// Keep actor fourteen just above and behind the second block.
static void CrossbonePlatform_FollowSecondBlock(void) {
  struct FieldActor *target = Object_GetById(14);
  struct FieldActor *source = Object_GetById(9);

  target->y.fixed = PIXELS(32);
  target->x.fixed = source->x.fixed;
  target->z.fixed = source->z.fixed + PIXELS(1);
}

// Align map object 107 with the second block.
static void CrossbonePlatform_PositionSecondBlockMapObject(void) {
  struct FieldActor *frame = Object_GetById(9);

  Engine_MapObjectSetPosition(107, frame->x.fixed, frame->z.fixed + PIXELS(1));
}

// Choose block drawing priorities from the leader height and row.
static void CrossbonePlatform_UpdateBlockPriorities(void) {
  struct FieldActor *leader = Object_GetById(0);
  u8 priority;

  if (leader->y.fixed > PIXELS(16)) {
    priority = ACTOR_PRIORITY_UNDERFOOT;
    Object_GetById(8)->priorityFlags = priority;
    if (Object_GetById(10)->y.fixed == 0)
      Object_GetById(10)->priorityFlags = priority;
    Object_GetById(11)->priorityFlags = priority;
  } else {
    if (Object_GetById(10)->y.fixed == 0 &&
        Object_GetById(0)->z.fixed / PIXELS(16) > 56) {
      Engine_ActorSetSpritePriority(10, 3);
    } else {
      Engine_ActorSetSpritePriority(10, 1);
      Object_GetById(10)->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
    }
    priority = 0;
    Object_GetById(11)->priorityFlags = priority;
  }
  Object_GetById(12)->priorityFlags = priority;
}

// Initialize the key, pursuer or movable-block room.
s32 Scene_Initialize(void) {
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  if (gGameState.scene == (s32)&SceneId_CrossbonePlatform1) {
    CrossbonePlatform_InitializeScene1();
  }
  if (gGameState.scene == (s32)&SceneId_CrossbonePlatform2) {
    CrossbonePlatform_InitializeScene2();
  }
  if (gGameState.scene == (s32)&SceneId_CrossbonePlatform3) {
    CrossbonePlatform_InitializeScene3();
  }
  return 0;
}

// Unused callback that cycles key icon displays.
UNUSED static void CrossbonePlatform_UpdateItemIcons(void) {
  if (++gCrossbonePlatform_IconTimer > 16) {
    gCrossbonePlatform_IconTimer = 0;
  }
  switch (gCrossbonePlatform_IconTimer) {
  case 12:
    if (Engine_GameFlagIsSet(0xee7) == 0) {
      Actor_SetPosition(8, PIXELS(232), PIXELS(872));
    }
    if (Engine_GameFlagIsSet(0xee8) == 0) {
      Actor_SetPosition(9, PIXELS(296), PIXELS(824));
    }
    if (Engine_GameFlagIsSet(0xee9) == 0) {
      Actor_SetPosition(10, PIXELS(328), PIXELS(760));
    }
    if (Engine_GameFlagIsSet(0xeea) == 0) {
      Actor_SetPosition(11, PIXELS(360), PIXELS(872));
    }
    break;
  case 10:
    Colosso_ShowItemIcon(8, 0);
    break;
  case 8:
    Colosso_ShowItemIcon(9, 0);
    break;
  case 6:
    Colosso_ShowItemIcon(10, 0);
    break;
  case 4:
    Colosso_ShowItemIcon(11, 0);
    break;
  case 2:
    Colosso_ShowItemIcon(12, 1);
    break;
  }
}

// Shared source is included here in ROM order.
#include "common/colosso_item_icon.h"

// Restore collected keys and already opened passages.
static void CrossbonePlatform_InitializeScene1(void) {
  Engine_TaskWait(1);
  Colosso_ShowItemIcon(12, ITEM_RED_KEY);
  Colosso_ShowItemIcon(11, ITEM_BLUE_KEY);
  Colosso_ShowItemIcon(10, ITEM_BLUE_KEY);
  Colosso_ShowItemIcon(9, ITEM_BLUE_KEY);
  Colosso_ShowItemIcon(8, ITEM_BLUE_KEY);
  if (GameFlag_IsSet(0xee7) == 0) {
    Actor_SetPosition(8, PIXELS(232), PIXELS(872));
  }
  if (GameFlag_IsSet(0xee8) == 0) {
    Actor_SetPosition(9, PIXELS(296), PIXELS(824));
  }
  if (GameFlag_IsSet(0xee9) == 0) {
    Actor_SetPosition(10, PIXELS(328), PIXELS(760));
  }
  if (GameFlag_IsSet(0xeea) == 0) {
    Actor_SetPosition(11, PIXELS(360), PIXELS(872));
  }
  if (GameFlag_IsSet(0x9c0) != 0) {
    CrossbonePlatform_OpenPassage(0);
  }
  if (GameFlag_IsSet(0x9c1) != 0) {
    CrossbonePlatform_OpenPassage(1);
  }
  if (GameFlag_IsSet(0x9c2) != 0) {
    CrossbonePlatform_OpenPassage(2);
  }
  if (GameFlag_IsSet(0x9c3) != 0) {
    CrossbonePlatform_OpenPassage(3);
  }
  if (GameFlag_IsSet(0x9c4) != 0) {
    CrossbonePlatform_OpenPlatformPassage(0);
  }
}

// Configure the pursuers, their chase triggers and blending.
static void CrossbonePlatform_InitializeScene2(void) {
  Object_GetById(8)->collisionFlags = 1;
  Object_GetById(9)->collisionFlags = 1;
  Object_GetById(10)->collisionFlags = 1;
  Object_GetById(11)->collisionFlags = 1;
  Object_GetById(8)->scaleX = 7 * Q16_ONE / 10;
  Object_GetById(9)->scaleX = 7 * Q16_ONE / 10;
  Object_GetById(10)->scaleX = 7 * Q16_ONE / 10;
  Object_GetById(11)->scaleX = 7 * Q16_ONE / 10;
  Object_GetById(12)->scaleX = 7 * Q16_ONE / 10;
  FieldScheduler_AddOrUpdateCallback(CrossbonePlatform_CheckFifthPursuerTrigger,
                                     0xc80);
  FieldScheduler_AddOrUpdateCallback(
      CrossbonePlatform_CheckSecondPursuerTrigger, 0xc80);
  FieldScheduler_AddCallback(CrossbonePlatform_CheckFirstPursuerTrigger, 0xc80);
  Io_Write16(0x3f42, &REG_BLDCNT);
  Io_Write16(0x607, &REG_BLDALPHA);
}

// Restore movable blocks, terrain and the Djinni escape progress.
static void CrossbonePlatform_InitializeScene3(void) {
  struct FieldActor *djinni;

  // Reattach the second block follower and rebuild all block support cells.
  Object_GetById(14)->motionFlags = 0;
  FieldScheduler_AddOrUpdateCallback(CrossbonePlatform_FollowSecondBlock,
                                     0xc80);
  FieldScheduler_AddCallback(CrossbonePlatform_UpdateBlockPriorities, 0xc80);
  MapObject_SetPosition(107, 0, 0);
  if (GameFlag_IsSet(0xed9) != 0) {
    Engine_ActorSetAnimation(14, 2);
  }
  CrossbonePlatform_UpdateFirstBlockTerrain();
  CrossbonePlatform_UpdateSecondBlockTerrain();
  CrossbonePlatform_UpdateThirdBlockTerrain();
  CrossbonePlatform_UpdateFourthBlockTerrain();
  CrossbonePlatform_UpdateFifthBlockTerrain();
#if TBS_PATCH_1
  Engine_ActorSetSpritePriority(8, 3);
#endif
  Object_GetById(11)->motionFlags = 0;
  Object_GetById(12)->motionFlags = 0;
  ActorPresentation_RepaintCellsAtActorsElevenAndTwelve();
  if (GameFlag_IsSet(0x200) != 0) {
    CrossbonePlatform_FreezePuddle();
    Engine_ActorSetAnimation(13, 5);
  }

  // Restore the surviving Djinni at the last completed escape stage.
  if (GameFlag_IsSet(0x109) == 0) {
    if (GameFlag_IsSet(0x9ca) != 0) {
      Actor_SetPosition(ACTOR_DJINNI, PIXELS(856), PIXELS(824));
      djinni = Object_GetById(ACTOR_DJINNI);
#ifdef UBFIX
      djinni->update = CrossbonePlatform_FaceDjinniTowardLeader;
#else
      djinni->update = (void (*)(
          union FieldObject *))CrossbonePlatform_FaceDjinniTowardLeader;
#endif
    } else if (GameFlag_IsSet(0x9c9) != 0) {
      Actor_SetPosition(ACTOR_DJINNI, PIXELS(888), PIXELS(664));
      djinni = Object_GetById(ACTOR_DJINNI);
      djinni->sprite->rotation = 0;
      ObjectDispatch_ApplyValueToChildren(djinni, 16);
    } else if (GameFlag_IsSet(0x9c8) != 0) {
      Actor_SetPosition(ACTOR_DJINNI, PIXELS(584), PIXELS(680));
    } else {
      Actor_SetPosition(ACTOR_DJINNI, PIXELS(584), PIXELS(664));
    }
  }
}

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

s32 gCrossbonePlatform_ActorFifteenMotionScript[] = {
    SC_ADD(SW_SCALE_X, -Q16_ONE / 4),
    SC_ADD(SW_SCALE_Y, Q16_ONE / 8),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, Q16_ONE / 8),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 16),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, Q16_ONE / 16),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 32),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, Q16_ONE / 16),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 32),
    SC_WAIT(4),
    SC_ADD(SW_SCALE_X, -Q16_ONE / 4),
    SC_ADD(SW_SCALE_Y, Q16_ONE / 8),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, Q16_ONE / 8),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 16),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, Q16_ONE / 16),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 32),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, Q16_ONE / 16),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 32),
    SC_WAIT(64),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gCrossbonePlatform_ActorFifteenWalkTargets[] = {
    888, 856,
    872, 856,
    856, 856,
    840, 856,
    824, 856,
    824, 840,
    824, 824,
    840, 824,
    856, 824,
    872, 824,
    888, 824,
    888, 840,
};

struct SceneEntrance gCrossbonePlatformEntrancesOther[] = {
    { 0, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gCrossbonePlatformEntrances1[] = {
    { 1, CONDITION_ALWAYS, 200, 0, 968, FACING_NORTH, 0, 24, 560, 504, 1008, 0 },
    { 2, CONDITION_ALWAYS, 88, 0, 968, FACING_NORTH, 0, 24, 560, 504, 1008, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gCrossbonePlatformEntrances2[] = {
    { 1, CONDITION_ALWAYS, 200, 0, 408, FACING_NORTH, 0, 40, 0, 464, 440, 0 },
    { 2, CONDITION_ALWAYS, 88, 0, 392, FACING_NORTH, 0, 40, 0, 464, 440, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gCrossbonePlatformEntrances3[] = {
    { 1, CONDITION_ALWAYS, 776, 0, 952, FACING_NORTH, 0, 512, 544, 928, 992, 0 },
    { 2, CONDITION_ALWAYS, 664, 0, 952, FACING_NORTH, 0, 512, 544, 928, 992, 0 },
    { SCENE_TABLE_END },
};

u32 gCrossbonePlatform_SceneTable[] = {
    SCENE_EXITS(117),
    SCENE_EXIT(1, 128, 3),
    SCENE_EXIT(2, 128, 4),
    SCENE_EXIT(0, 118, 0),
    SCENE_EXIT(1, 129, 3),
    SCENE_EXIT(2, 129, 4),
    SCENE_EXIT(0, 120, 0),
    SCENE_EXIT(1, 131, 3),
    SCENE_EXIT(2, 131, 4),
    SCENE_EXITS_END,
};

struct ScenePlacement gCrossbonePlatformPlacementsOther[] = {
    { SCENE_TABLE_END },
};

struct ScenePlacement gCrossbonePlatformPlacements1[] = {
    { 0x16, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(80), 0, PIXELS(1016), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x16, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(80), 0, PIXELS(1016), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x16, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(80), 0, PIXELS(1016), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x16, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(80), 0, PIXELS(1016), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x16, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(80), 0, PIXELS(1016), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gCrossbonePlatformPlacements2[] = {
    { 0x1f4, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(360), 0, PIXELS(152), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x1f4, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(168), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x1f4, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(264), 0, PIXELS(360), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x1f4, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(344), 0, PIXELS(360), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x1f4, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(312), 0, PIXELS(232), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gCrossbonePlatformPlacements3[] = {
    { 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(616), 0, PIXELS(680), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(728), 0, PIXELS(744), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(600), 0, PIXELS(856), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(824), 0, PIXELS(888), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(840), 0, PIXELS(888), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xe3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(648), 0, PIXELS(888), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x14, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(728), 0, PIXELS(760), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x5a, 0x36, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gCrossbonePlatformEventsOther[] = {
    { SCENE_EVENTS_END },
};

struct SceneEvent gCrossbonePlatformEvents1[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, 5), 0, CONDITION_ALWAYS, EVENT_SCRIPT(CrossbonePlatform_ShowRedKey) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, 4), 0, CONDITION_ALWAYS, EVENT_SCRIPT(CrossbonePlatform_HideRedKey) },
    { PSYNERGY_ON_ACTOR(ABILITY_CATCH), 8, CONDITION_ALWAYS, EVENT_SCRIPT(CrossbonePlatform_CatchFirstBlueKey) },
    { PSYNERGY_ON_ACTOR(ABILITY_CATCH), 9, CONDITION_ALWAYS, EVENT_SCRIPT(CrossbonePlatform_CatchSecondBlueKey) },
    { PSYNERGY_ON_ACTOR(ABILITY_CATCH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(CrossbonePlatform_CatchThirdBlueKey) },
    { PSYNERGY_ON_ACTOR(ABILITY_CATCH), 11, CONDITION_ALWAYS, EVENT_SCRIPT(CrossbonePlatform_CatchFourthBlueKey) },
    { PSYNERGY_ON_ACTOR(ABILITY_CATCH), 12, CONDITION_ALWAYS, EVENT_SCRIPT(CrossbonePlatform_CatchRedKey) },
    { EVENT_TOUCH, 50, CONDITION_ALWAYS, EVENT_SCRIPT(CrossbonePlatform_UseRedKey) },
    { EVENT_TOUCH, 51, CONDITION_ALWAYS, EVENT_SCRIPT(CrossbonePlatform_OpenFirstBlueDoor) },
    { EVENT_TOUCH, 52, CONDITION_ALWAYS, EVENT_SCRIPT(CrossbonePlatform_OpenSecondBlueDoor) },
    { EVENT_TOUCH, 53, CONDITION_ALWAYS, EVENT_SCRIPT(CrossbonePlatform_OpenThirdBlueDoor) },
    { EVENT_TOUCH, 54, CONDITION_ALWAYS, EVENT_SCRIPT(CrossbonePlatform_OpenFourthBlueDoor) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 53, CONDITION_ALWAYS, EVENT_SCRIPT(CrossbonePlatform_CheckLeaderAhead) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 54, CONDITION_ALWAYS, EVENT_SCRIPT(CrossbonePlatform_CheckLeaderAhead) },
    { SEARCH(SEARCH_CHEST), 100, 0xeca, FIND_COINS(333) },
    { SEARCH(SEARCH_CHEST), 101, 0xecb, FIND_ITEM(0x10b) },
    { SEARCH(SEARCH_CHEST), 102, 0xecc, FIND_ITEM(0xc0) },
    { SEARCH(SEARCH_CHEST), 103, 0xecd, FIND_ITEM(ITEM_SMOKE_BOMB) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gCrossbonePlatformEvents2[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_TOUCH, 53, CONDITION_ALWAYS, EVENT_SCRIPT(CrossbonePlatform_TrackThirdPursuerColumn) },
    { EVENT_TOUCH, 54, CONDITION_ALWAYS, EVENT_SCRIPT(CrossbonePlatform_TrackFourthPursuerColumn) },
    { EVENT_TOUCH, 61, CONDITION_ALWAYS, EVENT_SCRIPT(CrossbonePlatform_DodgeThirdPursuer) },
    { EVENT_TOUCH, 62, CONDITION_ALWAYS, EVENT_SCRIPT(CrossbonePlatform_DodgeFourthPursuer) },
    { EVENT_RAISED, 96, CONDITION_ALWAYS, EVENT_SCRIPT(CrossbonePlatform_DodgeFifthPursuer) },
    { EVENT_RAISED, 92, CONDITION_ALWAYS, EVENT_SCRIPT(CrossbonePlatform_DodgeSecondPursuer) },
    { EVENT_RAISED, 91, CONDITION_ALWAYS, EVENT_SCRIPT(CrossbonePlatform_DodgeFirstPursuer) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, 5), 0, CONDITION_ALWAYS, EVENT_SCRIPT(CrossbonePlatform_BeginRevealBlend) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, 4), 0, CONDITION_ALWAYS, EVENT_SCRIPT(CrossbonePlatform_EndRevealBlend) },
    { PSYNERGY_ON_ACTOR(ABILITY_HALT), 8, CONDITION_ALWAYS, EVENT_SCRIPT(Resource3b4_EmptyHookB) },
    { PSYNERGY_ON_ACTOR(ABILITY_HALT), 9, CONDITION_ALWAYS, EVENT_SCRIPT(Resource3b4_EmptyHookB) },
    { PSYNERGY_ON_ACTOR(ABILITY_HALT), 10, CONDITION_ALWAYS, EVENT_SCRIPT(Resource3b4_EmptyHookB) },
    { PSYNERGY_ON_ACTOR(ABILITY_HALT), 11, CONDITION_ALWAYS, EVENT_SCRIPT(Resource3b4_EmptyHookB) },
    { PSYNERGY_ON_ACTOR(ABILITY_HALT), 12, CONDITION_ALWAYS, EVENT_SCRIPT(Resource3b4_EmptyHookB) },
    { SEARCH(SEARCH_CHEST), 100, 0xece, FIND_COINS(444) },
    { SEARCH(SEARCH_CHEST), 101, 0xecf, FIND_ITEM(ITEM_SLEEP_BOMB) },
    { SEARCH(SEARCH_CHEST), 102, 0xed0, FIND_ITEM(0x60) },
    { SEARCH(SEARCH_CHEST), 103, 0xed1, FIND_ITEM(0xba) },
    { SCENE_EVENTS_END },
};
// clang-format on

struct SceneEvent gCrossbonePlatformEvents3[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
    {EVENT_EXIT, 2, CONDITION_ALWAYS, 2},
    {0x202, 50, CONDITION_ALWAYS,
     EVENT_SCRIPT(CrossbonePlatform_MoveThirdBlock)},
    {0x202, 51, CONDITION_ALWAYS,
     EVENT_SCRIPT(CrossbonePlatform_ClimbOrMoveFirstBlock)},
    {0x202, 52, CONDITION_ALWAYS,
     EVENT_SCRIPT(CrossbonePlatform_MoveSecondBlock)},
    {0x202, 53, CONDITION_ALWAYS,
     EVENT_SCRIPT(CrossbonePlatform_MoveFourthBlock)},
#if TBS_PATCH_1
    {0x202, 61, CONDITION_ALWAYS,
     EVENT_SCRIPT(CrossbonePlatform_ClimbOrMoveFirstBlock)},
#endif
    {0x202, 54, CONDITION_ALWAYS,
     EVENT_SCRIPT(CrossbonePlatform_ChooseThirdBlockMotion)},
    {0x202, 60, CONDITION_ALWAYS, EVENT_SCRIPT(CrossbonePlatform_UseLadder)},
    {0x202, 70, CONDITION_ALWAYS,
     EVENT_SCRIPT(CrossbonePlatform_ChooseClimbOrPush)},
    {EVENT_TOUCH, 20, 0x36, EVENT_SCRIPT(CrossbonePlatform_DjinniFirstEscape)},
    {EVENT_TOUCH, 21, 0x36, EVENT_SCRIPT(CrossbonePlatform_DjinniLongEscape)},
    {EVENT_TOUCH, 22, 0x36, EVENT_SCRIPT(CrossbonePlatform_DjinniFinalEscape)},
    {EVENT_TOUCH, 30, 0x36,
     EVENT_SCRIPT(CrossbonePlatform_MoveDjinniToTouchedTarget)},
    {EVENT_TOUCH, 31, 0x36,
     EVENT_SCRIPT(CrossbonePlatform_MoveDjinniToTouchedTarget)},
    {EVENT_TOUCH, 32, 0x36,
     EVENT_SCRIPT(CrossbonePlatform_MoveDjinniToTouchedTarget)},
    {EVENT_TOUCH, 33, 0x36,
     EVENT_SCRIPT(CrossbonePlatform_MoveDjinniToTouchedTarget)},
    {EVENT_TOUCH, 34, 0x36,
     EVENT_SCRIPT(CrossbonePlatform_MoveDjinniToTouchedTarget)},
    {EVENT_TOUCH, 35, 0x36,
     EVENT_SCRIPT(CrossbonePlatform_MoveDjinniToTouchedTarget)},
    {EVENT_TOUCH, 36, 0x36,
     EVENT_SCRIPT(CrossbonePlatform_MoveDjinniToTouchedTarget)},
    {EVENT_TOUCH, 37, 0x36,
     EVENT_SCRIPT(CrossbonePlatform_MoveDjinniToTouchedTarget)},
    {EVENT_TOUCH, 38, 0x36,
     EVENT_SCRIPT(CrossbonePlatform_MoveDjinniToTouchedTarget)},
    {EVENT_TOUCH, 39, 0x36,
     EVENT_SCRIPT(CrossbonePlatform_MoveDjinniToTouchedTarget)},
    {EVENT_TOUCH, 40, 0x36,
     EVENT_SCRIPT(CrossbonePlatform_MoveDjinniToTouchedTarget)},
    {EVENT_TOUCH, 41, 0x36,
     EVENT_SCRIPT(CrossbonePlatform_MoveDjinniToTouchedTarget)},
    {0x7, 15, CONDITION_ALWAYS, EVENT_SCRIPT(CrossbonePlatform_JoinDjinni)},
    {PSYNERGY_ON_ACTOR(ABILITY_HALT), 15, CONDITION_ALWAYS,
     EVENT_SCRIPT(CrossbonePlatform_HaltDjinni)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 13, 0x200,
     EVENT_SCRIPT(CrossbonePlatform_FreezePuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, 0x201,
     EVENT_SCRIPT(CrossbonePlatform_UpdateFirstBlockTerrain)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 9, 0x206,
     EVENT_SCRIPT(CrossbonePlatform_UpdateSecondBlockTerrain)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 10, 0x203,
     EVENT_SCRIPT(CrossbonePlatform_UpdateThirdBlockTerrain)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 11, 0x204,
     EVENT_SCRIPT(CrossbonePlatform_UpdateFourthBlockTerrain)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 12, 0x205,
     EVENT_SCRIPT(CrossbonePlatform_UpdateFifthBlockTerrain)},
    {SEARCH(SEARCH_CHEST), 104, 0xed6, FIND_COINS(666)},
    {SEARCH(SEARCH_CHEST), 105, 0xed7, FIND_ITEM(ITEM_WATER_OF_LIFE)},
    {SEARCH(SEARCH_CHEST), 106, 0xed8, FIND_ITEM(ITEM_ELIXIR)},
    {SEARCH(SEARCH_CHEST), 107, 0xed9, FIND_ITEM(0x26)},
    {SCENE_EVENTS_END},
};

u32 gCrossbonePlatform_SlotColumns[] = {
    16,
    21,
    0,
};

s32 gCrossbonePlatform_IconTimer = 0;
