// Bilibin's village, river crossing and raised warehouse approach.
// Town dialogue changes with the tree curse; the river route includes a rescue
// and Jill's reward for helping the stranded villager.

#include "game_state.h"
#include "scene_ids.h"
#include "unit/import.h"
#include "unit/scene/bilibin.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "unit/runtime/runtime.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "bilibin.h"
#include "types.h"
#include "field_event.h"
#include "field_service.h"
#include "field_scene.h"
#include "call.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "objcmd.h"
#include "facing_object.h"
#include "field_obj.h"

// Forward declarations.
static void Bilibin_RestoreVillageActors(void);
static void Bilibin_UpdateVillageBlockCells(void);
static void Bilibin_RestoreRiverActors(void);
static void Bilibin_UpdateRiverBlockCells(void);
static void Bilibin_RestoreRaisedEntrance(void);
static void Bilibin_UpdateCornerSpawn(void);
static void Bilibin_SpawnTransientCornerEffect(struct FieldActor *actor);
static void Bilibin_UpdateRaisedBlockCells(void);

struct SpawnCounter {
  s16 frames;
};

struct SpawnCounter gCornerSpawnCounter;

// Shared source is included here in ROM order.
#include "common/object_facing.h"

// Selects the entrance positions for the current part of Bilibin.
const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_Bilibin1) {
    return gBilibinEntrances1;
  }
  if (scene == (s32)&SceneId_Bilibin3) {
    return gBilibinEntrances3;
  }
  if (scene == (s32)&SceneId_Bilibin2) {
    return gBilibinEntrances2;
  }
  return (const struct SceneEntrance *)gBilibinEntrancesOther;
}

// Only the raised approach supplies exit-region boxes.
const struct SceneRegion *Scene_GetRegions(void) {
  if (gGameState.scene == (s32)&SceneId_Bilibin2) {
    return (const struct SceneRegion *)gBilibinRegions2;
  }
  return 0;
}

// Returns the packed exit destinations shared by the three maps.
u8 *SceneData_GetMessageTable(void) { return (u8 *)gPlacement_Messages; }

// Adds the Jupiter Djinni found beyond the warehouse approach.
static void Bilibin_RecruitJupiterDjinni(void) {
  Djinn_JoinParty(9, ELEMENT_JUPITER, 0);
}

// Selects the village, river or raised-approach actors.
const struct ScenePlacement *Scene_GetPlacements(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_Bilibin1) {
    return gBilibinPlacements1;
  }
  if (scene == (s32)&SceneId_Bilibin3) {
    return gBilibinPlacements3;
  }
  if (scene == (s32)&SceneId_Bilibin2) {
    return gBilibinPlacements2;
  }
  return gBilibinPlacementsOther;
}

// Reads the two-part description of the deep well.
static void Bilibin_InspectWell(void) {
  Engine_EventBegin();
  Engine_MessageShowCentered(MsgFieldPeeredWell, 1);
  Engine_MessageShowCentered(MsgBilibinBottomNotVisibleLooksVery, 1);
  Engine_EventEnd();
}

// Selects the dialogue, searches and interactions for the current map.
const struct SceneEvent *Scene_GetEvents(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_Bilibin1) {
    return gBilibinEvents1;
  }
  if (scene == (s32)&SceneId_Bilibin3) {
    return gBilibinEvents3;
  }
  if (scene == (s32)&SceneId_Bilibin2) {
    return gBilibinEvents2;
  }
  return gBilibinEventsOther;
}

// Reads the sign restricting entry without permission.
static void Bilibin_ReadRestrictedAreaSign(void) {
  Engine_EventBegin();
  Engine_MessageShowCentered(MsgBilibinAreaOffLimitsThoseWithout, 1);
  Engine_EventEnd();
}

// Reads McCoy's warning on the hidden warehouse.
static void Bilibin_ReadWarehouseSign(void) {
  Engine_EventBegin();
  Engine_MessageShowCentered(MsgBilibinMccoysHiddenWarehouseDoNot, 1);
  Engine_EventEnd();
}

// Describes the human shape shared by the river's transformed villagers.
static void Bilibin_InspectHumanShapedTree(void) {
  Engine_EventBegin();
  Engine_MessageShowCentered(MsgBilibinThereTreeLooksLikePerson, 1);
  Engine_EventEnd();
}

// Examines the tree near the village entrance, with a later line on repeat
// visits.
static void Bilibin_ExamineTreeResemblingMan(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgBilibinItsTreeButAlmostLooks);
  if (GameFlag_IsSet(0x301) != 0) {
    gEventWork->message++;
  }
  Event_ShowMessage(9, 0);
  GameFlag_Set(0x301);
  Engine_EventEnd();
}

// Asks whether the party saw the strange tree at the entrance.
static void Bilibin_AskAboutEntranceTree(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgBilibinDidSeeTreeAtEntrance);
  Engine_EventAskYesNo(12, 0);
  Engine_EventEnd();
}

// Asks whether the party tried the route southeast from Bilibin.
static void Bilibin_AskAboutSoutheastRoute(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgBilibinHaveTriedHeadingSoutheastFrom);
  Engine_EventAskYesNo(14, 0);
  Engine_EventEnd();
}

// Asks the party to believe the speaker was turned into a tree.
static void Bilibin_AskAboutTreeTransformation(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgBilibinWasTurnedIntoTreeFor);
  Engine_EventAskYesNo(21, 0);
  Engine_EventEnd();
}

// Asks whether the party are the three warriors who broke the curse.
static void Bilibin_AskAboutCurseBreakers(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgBilibinCurseWasBrokenThanksEfforts);
  Engine_EventAskYesNo(16, 0);
  Engine_EventEnd();
}

// Asks whether the party has visited Imil to the north.
static void Bilibin_AskAboutImil(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgBilibinHaveEverBeenVillageImil);
  Engine_EventAskYesNo(18, 0);
  Engine_EventEnd();
}

// Stops wandering actors, animates the touched entrance and walks through its
// exit.
static void Bilibin_AnimateExitCells(void) {

  u32 actorSlot;
  struct FieldActor *actor;
  s32 exitIndex;
  u8 *originBytes;
  s32 originOffset;
  s32 rowOffset;
  s32 column;
  s32 row;
  struct EventWork *work;

  work = gEventWork;
  Engine_EventBegin();
  // Freeze placed actors before the exit cells start animating.
  for (actorSlot = ACTOR_FIRST_PLACED; actorSlot < 66; actorSlot++) {
    actor = Object_GetById(actorSlot);
    if (actor != 0) {
      actor->motionFlags = 0;
    }
  }
  exitIndex = (((u16)work->touchedTrigger - 3) << 16) >> 16;
  if (exitIndex == 6) {
    Event_PlayCue(188);
  } else {
    Engine_AudioPlayCue(158);
  }
  // Each touched trigger selects one cell origin and animation script.
  originOffset = exitIndex << 2;
  originBytes = (u8 *)gVillage_DoorCellOrigins;
  column = *(s16 *)(originBytes + originOffset);
  rowOffset = originOffset + 2;
  row = *(s16 *)(originBytes + rowOffset);
  Engine_MapAnimateCells(gVillage_DoorCellSteps[exitIndex], column, row);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
  Object_GetById(0)->motionFlags = 0;
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  if (exitIndex == 6) {
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -4);
  } else {
    Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 3, -16);
  }
  if (exitIndex == 4) {
    Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 3);
  } else {
    Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 2);
  }
  Engine_EventWait(16);
  Engine_EventRequestExit(exitIndex + 3);
  Engine_EventEnd();
}

// Pulls the stranded villager clear of the river in three stages.
static void Bilibin_RescueVillagerFromRiver(void) {
  struct FieldActor *leader;
  struct FieldActor *rescueTarget;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  rescueTarget = Object_GetById(11);
  if ((rescueTarget->x.fixed >> 20) == 6) {
    Engine_EventBegin();
    Engine_ActorSetSpritePriority(11, 1);
    Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_EventWait(20);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x3333, 0x1999);
    Actor_SetSpeed(11, 0x3333, 0x1999);
    Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
    rescueTarget->motionFlags = 0;
    // Alternate the pulling animation with short moves along the bank.
    leader->scaleX = -0x10000;
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 16);
    Actor_MoveToAndWait(11, 111, 196);
    leader->scaleX = 0x10000;
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 128, 185);
    Engine_EventWait(20);
    leader->scaleX = -0x10000;
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 16);
    Actor_MoveToAndWait(11, 121, 190);
    leader->scaleX = 0x10000;
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 141, 189);
    Engine_EventWait(20);
    leader->scaleX = -0x10000;
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 16);
    Actor_MoveToAndWait(11, 132, 186);
    leader->scaleX = 0x10000;
    Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags |= ACTOR_FACES_MOTION;
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x9999, 0x4ccc);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 166, 185);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 20);
    Engine_ActorSetSpritePriority(11, 2);
    RunSceneTransitionEffect(0, 11);
    Engine_TaskWait(10);
    // Restore the rescued villager's presentation before the thank-you line.
    Engine_EventSetMessage(MsgBilibinThankSavedMeFromBeing);
    Event_ShowMessage(11, 0);
    Engine_PsynergyCancel();
    Engine_TaskWait(10);
    GameFlag_Set(0x848);
    Engine_EventEnd();
  }
}

// Checks the blocked direction before allowing another movement step.
static void Bilibin_CheckLeaderAhead(void) { Engine_LeaderCheckAhead(); }

// After the rescue, Jill thanks the party with a Hard Nut.
static void Bilibin_ReceiveJillsHardNut(void) {
  struct FieldActor *leader;
  struct FieldActor *nearbyVillager;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (GameFlag_IsSet(0x845) == 0 || GameFlag_IsSet(0x848) == 0)
    return;

  Engine_EventBegin();
  // Bring the three villagers and the leader into the reward conversation.
  Camera_SetSpeed(0x26666, 0x4ccc);
  Camera_MoveTo(0x1070000, -1, 0xad0000, 1);
  Engine_CameraWaitForMove();
  nearbyVillager = Actor_Get(12);
  if (nearbyVillager->x.fixed > leader->x.fixed) {
    Actor_FaceDirection(13, FACING_SOUTH + FACING_STEP, 20);
    Actor_ShowEmote(13, EMOTE_IN_FRONT, 20);
    Engine_EventSetMessage(MsgBilibinYoureGuy);
    Event_ShowMessageAndWait(13, 0, 10);
    Actor_ShowEmote(12, EMOTE_IN_FRONT, 0);
  } else {
    Actor_FaceDirection(12, FACING_SOUTHEAST + FACING_STEP, 20);
    Actor_ShowEmote(12, EMOTE_IN_FRONT, 20);
    Engine_EventSetMessage(MsgBilibinYoureGuy);
    Event_ShowMessageAndWait(12, 0, 10);
    Actor_ShowEmote(13, EMOTE_IN_FRONT, 0);
  }
  Actor_ShowEmote(14, EMOTE_IN_FRONT, 0);
  Actor_FaceDirection(14, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(12, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(13, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 268, 184);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 40);
  Engine_ActorRunRepeatedMotion(13, 2);
  Event_ShowMessageAndWait(13, 0, 10);
  Actor_FaceDirection(13, FACING_EAST, 0);
  Actor_FaceDirection(14, FACING_SOUTHEAST + FACING_STEP, 20);
  Actor_FaceDirection(12, FACING_WEST, 20);
  Engine_ActorSetAnimationAndWait(12, ANIM_NOD);
  Actor_SetAttachedEffect(14, 0x102);
  Engine_EventWait(40);
  Actor_FaceDirection(14, FACING_SOUTHEAST + FACING_STEP, 10);
  Actor_FaceDirection(12, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(13, FACING_SOUTHEAST + FACING_STEP, 10);
  Engine_ActorRunRepeatedMotion(14, 1);
  Event_ShowMessageAndWait(14, 0, 10);
  Engine_ActorSetAnimation(12, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(13, ANIM_NOD);
  Engine_EventWait(20);
  Event_ShowMessage(14, 0);
  Actor_SetSpeed(14, 0x9999, 0x4ccc);
  Object_GetById(14)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(14, 266, 172);
  Engine_EventWait(1);
  Object_GetById(14)->steeringFlags |= ACTOR_FACES_MOTION;
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(14, ANIM_NOD);
  Event_ShowMessageAndWait(14, 0, 10);
  // Show the named gift before adding it to the party's inventory.
  Engine_MessageShowCentered(MsgBilibinJillGaveIsaacSpecialGift, 1);
  gEventWork->message++;
  Engine_ItemShowFound(ITEM_HARD_NUT, 3);
  Engine_PartyGiveItem(ITEM_HARD_NUT, 0);
  Engine_ActorSetAnimationAndWait(14, ANIM_NOD);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_SetSpeed(14, 0x10000, 0x8000);
  Object_GetById(14)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(14, 262, 156);
  Engine_EventWait(1);
  {
    struct FieldActor *jill = Actor_Get(14);
    u8 steeringFlags = jill->steeringFlags | ACTOR_FACES_MOTION;

    jill->steeringFlags = steeringFlags;
  }
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(12, 2);
  Event_ShowMessageAndWait(12, 0, 10);
  Engine_ActorSetAnimation(12, ANIM_NOD);
  Engine_ActorSetAnimation(13, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(14, ANIM_NOD);
  // Let the villagers resume their shared facing script after the gift.
  FieldObject_SetTargetAndCallback(12, 0x10000, gVillage_VillagerActions);
  FieldObject_SetTargetAndCallback(13, 0x10000, gVillage_VillagerActions);
  FieldObject_SetTargetAndCallback(14, 0x10000, gVillage_VillagerActions);
  GameFlag_Set(0x849);
  Engine_EventEnd();
}

// Restores the current map's actors, collision cells and opening transition.
s32 Scene_Initialize(void) {
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  if (gGameState.scene == (s32)&SceneId_Bilibin1) {
    Bilibin_RestoreVillageActors();
  } else if (gGameState.scene == (s32)&SceneId_Bilibin3) {
    Bilibin_RestoreRiverActors();
    FieldScheduler_AddCallback(Bilibin_UpdateCornerSpawn, 0xc80);
  } else if (gGameState.scene == (s32)&SceneId_Bilibin2) {
    Bilibin_RestoreRaisedEntrance();
  }
  return 0;
}

// Restores the village population and entrance-dependent actor positions.
static void Bilibin_RestoreVillageActors(void) {

  struct FieldActor *actor;
  s16 entrance;

  if (GameFlag_IsSet(0x845) != 0) {
    Engine_ActorSetPosition(9, 0, 0);
    Actor_FaceDirection(14, FACING_SOUTHEAST + FACING_STEP, 0);
    Actor_FaceDirection(15, FACING_SOUTH + FACING_STEP, 0);
  } else {
    actor = Actor_Get(9);
    Engine_ActorSetSpriteFlags(actor, 0);
    Actor_SetPosition(21, 0, 0);
  }
  // Apply arrival-specific changes after restoring the population.
  actor = Actor_Get(8);
  actor->scaleY = 0x18000;
  entrance = gGameState.entrance;
  if (entrance == 10) {
    Actor_SetPosition(8, 0, 0);
  } else if (entrance == 9) {
    GameFlag_Clear(FLAG_ARRIVAL_EVENT_PENDING);
  }
  if (GameFlag_IsSet(0x109) == 0) {
    entrance = gGameState.entrance;
    if (entrance == 11) {
      Actor_SetPosition(20, PIXELS(248), PIXELS(216));
    }
  }
  Bilibin_UpdateVillageBlockCells();
  if (GameFlag_IsSet(0x84a) != 0 && GameFlag_IsSet(0x84b) == 0) {
    GameFlag_Set(0x304);
  }
}

// Updates collision cells around the movable block and moves an overlapping
// leader clear.
static void Bilibin_UpdateVillageBlockCells(void) {
  s32 leaderRow;
  s32 leaderColumn;
  struct FieldActor *leader;
  struct FieldActor *block;
  s32 blockX;
  s32 blockRow;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  block = Object_GetById(20);
  blockRow = block->z.fixed >> 20;
  leaderColumn = (leader->x.fixed >> 20);
  leaderRow = (leader->z.fixed >> 20);
  blockX = block->x.fixed;
  // Repaint the block's lane before clearing the cells it currently occupies.
  Map_CopyCellAttributes(15, 11, 3, 1, 15, 12);
  Map_CopyCellAttributes(15, 11, 3, 1, 15, 13);
  Map_CopyCellAttributes(15, 11, 3, 1, 15, 14);
  Map_CopyCellAttributes(1, 0, 1, 1, (blockX >> 20), blockRow);
  if ((blockX >> 20) != 16 || blockRow != 13) {
    Map_CopyCellAttributes(0, 0, 1, 1, 16, 13);
  }
  // The block may have moved onto the leader while this event was running.
  if (leaderColumn == 16) {
    if (leaderRow == 13) {
      Engine_EventBegin();
      Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT, 20);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x10000);
      Engine_ActorJump(ACTOR_PARTY_LEADER, 6, 0);
      if (blockRow == 13) {
        Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 262, 196);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 20);
      } else {
        Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 286, 218);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 20);
      }
      Engine_EventEnd();
    }
  }
}

// Restores the river actors and crossing after movement, rescue or reward
// events.
static void Bilibin_RestoreRiverActors(void) {
  struct FieldActor *rescueTarget;
  s32 populationState;
  struct FieldActor *riverActor;
  struct FieldActor *actor;
  s32 crossingState;

  riverActor = Object_GetById(10);
  rescueTarget = Object_GetById(11);
  actor = Actor_Get(8);
  Engine_ActorSetSpriteFlags(actor, 0);
  populationState = GameFlag_IsSet(0x845);
  if (populationState != 0) {
    // Hide the old actor arrangement and restore the crossing's map cells.
    Actor_SetPosition(9, 0, 0);
    Actor_SetPosition(10, 0, 0);
    Actor_SetPosition(11, 0, 0);
    Map_CopyCellsTo(56, 15, 40, 15, 1, 2);
    Map_CopyCellAttributes(26, 15, 1, 3, 10, 15);
    if (GameFlag_IsSet(0x849) == 0) {
      if (GameFlag_IsSet(0x848) == 0) {
        Engine_ActorSetPosition(14, 0, 0);
        Actor_FaceDirection(12, FACING_NORTH + FACING_STEP, 0);
        Actor_FaceDirection(13, FACING_NORTHWEST + FACING_STEP, 0);
      }
    } else {
      Actor_FaceDirection(12, FACING_NORTH + FACING_STEP, 0);
      Actor_FaceDirection(13, FACING_NORTHWEST + FACING_STEP, 0);
    }
  } else {
    Actor_SetPosition(12, 0, 0);
    Actor_SetPosition(13, 0, 0);
    Actor_SetPosition(14, 0, 0);
    actor = Actor_Get(9);
    Engine_ActorSetSpriteFlags(actor, 0);
    actor = Actor_Get(10);
    Engine_ActorSetSpriteFlags(actor, 0);
    actor = Actor_Get(11);
    Engine_ActorSetSpriteFlags(actor, 0);
    riverActor->motionFlags = populationState;
    // Restore the moved crossing and the villager's remembered rescue position.
    crossingState = GameFlag_IsSet(0x881);
    if (crossingState != 0) {
      Object_GetById(9)->collisionFlags |= 16;
      Object_GetById(16)->collisionFlags |= 16;
      Object_GetById(11)->collisionFlags |= 16;
      Actor_SetPosition(16, PIXELS(142), PIXELS(156));
      actor = Actor_Get(16);
      Engine_ActorSetSpriteFlags(actor, 0);
      Actor_SetPosition(10, PIXELS(142), PIXELS(156));
      riverActor->sprite->rotation = 0x4000;
      riverActor->y.fixed += -0x80000;
      if (GameFlag_IsSet(0x848) != 0) {
        Actor_SetPosition(11, PIXELS(132), PIXELS(186));
      } else {
        Actor_SetPosition(11, PIXELS(88), PIXELS(196));
        Engine_ActorSetSpritePriority(11, 3);
        rescueTarget->collisionFlags |= 4;
      }
    } else {
      riverActor->y.fixed = PIXELS(32);
      rescueTarget->motionFlags = crossingState;
      rescueTarget->y.fixed = PIXELS(48);
    }
  }
  Bilibin_UpdateRiverBlockCells();
}

// Restores ten river cells, then clears the movable block's current cell.
static void Bilibin_UpdateRiverBlockCells(void) {
  struct FieldActor *actor;
  s32 blockColumn;
  s32 blockRow;
  u32 cellOffset;

  actor = Object_GetById(8);
  blockColumn = actor->x.fixed >> 20;
  blockRow = actor->z.fixed >> 20;

  // The packed table stores ten byte-sized column/row pairs.
  for (cellOffset = 0; cellOffset < 20; cellOffset += 2) {
    s32 column = ((u8 *)gVillage_RepaintCells)[cellOffset];
    s32 row = ((u8 *)gVillage_RepaintCells)[cellOffset + 1];
    Map_CopyCellAttributes(1, 0, 1, 1, column, row);
  }

  Map_CopyCellAttributes(0, 0, 1, 1, blockColumn, blockRow);
}

// Restores the block cells and starts the raised arrivals above floor height.
static void Bilibin_RestoreRaisedEntrance(void) {
  struct FieldActor *leader;

  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  Bilibin_UpdateRaisedBlockCells();
  if (gGameState.entrance == 3 || gGameState.entrance == 4) {
    if (Engine_GameFlagIsSet(0x109) == 0) {
      leader = Object_GetById(ACTOR_PARTY_LEADER);
      Engine_EventBegin();
      leader->y.fixed = PIXELS(16);
      Engine_CameraMoveTo(leader->x.fixed, leader->y.fixed, leader->z.fixed, 0);
      Engine_MapRedraw();
      Engine_EventEnd();
      Engine_TaskWait(1);
    }
  }
}

// Shared source is included here in ROM order.
#include "common/object_findcell.h"

// Pushes the faced block one cell, moves the leader with it and refreshes
// collision.
static void Bilibin_PushFacedBlock(void) {
  struct FieldActor *leader;
  struct FieldActor *block;
  u32 direction;
  union FieldCoordinate destination[3];

  leader = Object_GetById(0);
  direction = leader->facing >> 12;
  block = SceneActor_FindActorAtCell(
      (leader->x.part.pixel + (gBilibin_FacingCellSteps[direction] >> 16)) >> 4,
      (leader->z.part.pixel + (s16)gBilibin_FacingCellSteps[direction]) >> 4);
  if (block != NULL) {
    block->terrainId = 2;
    destination[0].fixed =
        block->x.fixed + (gBilibin_FacingCellSteps[direction] & -0x10000);
    destination[1].fixed = block->y.fixed;
    destination[2].fixed =
        block->z.fixed + (gBilibin_FacingCellSteps[direction] << 16);
    if (FieldObject_CheckMovementCollision(block, destination) <= 0) {
      // Commit the block and leader together, then restore the standing mode.
      Object_SetMode(leader, 8);
      Engine_TaskWait(15);
      Engine_AudioPlayCue(185);
      block->speed = 0x3333;
      block->acceleration = 0x3333;
      Engine_ObjectSetPosition(block, destination[0].fixed,
                               destination[1].fixed, destination[2].fixed);
      leader->speed = 0x3333;
      leader->acceleration = 0x3333;
      Engine_ObjectSetPosition(leader, destination[0].fixed,
                               destination[1].fixed, destination[2].fixed);
      Engine_ObjectCommitPosition(block);
      block->x.fixed = destination[0].fixed;
      block->z.fixed = destination[2].fixed;
      block->velocityX = 0;
      block->velocityZ = 0;
      Object_SetMode(leader, 1);
      // Each map derives its collision cells from this same moved actor.
      if (gGameState.scene == (s32)&SceneId_Bilibin3)
        Bilibin_UpdateRiverBlockCells();
      else if (gGameState.scene == (s32)&SceneId_Bilibin1)
        Bilibin_UpdateVillageBlockCells();
      else if (gGameState.scene == (s32)&SceneId_Bilibin2)
        Bilibin_UpdateRaisedBlockCells();
    }
  }
}

// Spawns a corner effect every thirty frames while the selected actor is low
// enough.
static void Bilibin_UpdateCornerSpawn(void) {
  struct FieldActor *actor;

  actor = Object_GetById(gGameState.selectedActor);
  if (actor->x.fixed < PIXELS(142)) {
    if (actor->y.fixed < PIXELS(8)) {
      if (gCornerSpawnCounter.frames == 0)
        Bilibin_SpawnTransientCornerEffect(actor);
      if (++gCornerSpawnCounter.frames == 30)
        gCornerSpawnCounter.frames = 0;
    } else {
      gCornerSpawnCounter.frames = 0;
    }
  }
}

// Spawns the blended corner effect, whose script deletes it after thirty
// frames.
static void Bilibin_SpawnTransientCornerEffect(struct FieldActor *actor) {
  struct FieldActor *effect;
  struct FieldSprite *sprite;

  effect =
      Engine_ObjectCreate(24, actor->x.fixed, actor->y.fixed, actor->z.fixed);
  if (effect == NULL)
    return;

  sprite = effect->sprite;
  Engine_ObjectSetScript(effect, gVillage_SpawnScript);
  effect->motionFlags = 0;
  effect->terrainId = 1;
  effect->priorityFlags = 2;
  if (sprite == NULL)
    return;

  AnimationObjects_SelectAnimation(sprite, 2);
  sprite->flags = 0;
  sprite->blendMode = 1;
  sprite->priority = 3;
}

// Updates the two passage cells according to the movable block's current row.
static void Bilibin_UpdateRaisedBlockCells(void) {
  struct FieldActor *block;

  block = Actor_Get(8);
  if (block == NULL)
    return;

  if (block->z.fixed >> 20 == 6)
    Map_CopyCellAttributes(2, 0, 1, 1, 14, 6);
  else
    Map_CopyCellAttributes(0, 0, 1, 1, 14, 6);

  if (block->z.fixed >> 20 == 9)
    Map_CopyCellAttributes(2, 0, 1, 1, 14, 9);
  else
    Map_CopyCellAttributes(1, 0, 1, 1, 14, 9);
}

s32 gVillage_VillagerActions[] = {
    SC_CALL(OverlayObject_UpdateFacingTowardTarget),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

u16 gBilibinEntrancesOther[] = {
    MAP_CELL_STEP(0, 65535, 168, 0, 168),
    MAP_CELL_STEP(16384, 0, 65535, 65535, 65535),
    MAP_CELL_STEPS_END,
};

struct SceneEntrance gBilibinEntrances1[] = {
    {1, CONDITION_ALWAYS, 240, 0, 472, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {2, CONDITION_ALWAYS, 224, 0, 88, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {3, CONDITION_ALWAYS, 376, 0, 360, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {4, CONDITION_ALWAYS, 200, 0, 328, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {5, CONDITION_ALWAYS, 104, 0, 344, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {6, CONDITION_ALWAYS, 168, 0, 216, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {7, CONDITION_ALWAYS, 72, 0, 216, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {8, CONDITION_ALWAYS, 296, 0, 184, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {9, CONDITION_ALWAYS, 400, 0, 168, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {10, CONDITION_ALWAYS, 40, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {11, CONDITION_ALWAYS, 264, 0, 200, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gBilibinEntrances3[] = {
    {0, CONDITION_ALWAYS, 168, 0, 168, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 56, 0, 376, FACING_EAST, 0, 16, 16, 400, 400, 0},
    {2, CONDITION_ALWAYS, 376, 0, 184, FACING_WEST, 0, 16, 16, 400, 400, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gBilibinEntrances2[] = {
    {0, CONDITION_ALWAYS, 232, 0, 144, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 120, 0, 408, FACING_NORTH, 0, 0, 288, 240, 448, 0},
    {2, CONDITION_ALWAYS, 120, 0, 384, FACING_SOUTH, 0, 0, 288, 240, 448, 0},
    {3, CONDITION_ALWAYS, 72, 4, 128, FACING_NORTH, 0, 32, 32, 384, 304, 0},
    {4, CONDITION_ALWAYS, 296, 4, 224, FACING_NORTH, 0, 32, 32, 384, 304, 0},
    {SCENE_TABLE_END},
};

// clang-format off
s16 gBilibinRegions2[] = {
    66, 26, 128, 78, 38, 140, -1, 3,
    290, 26, 224, 302, 38, 236, -1, 4,
    -1, 0, 0, 0, 0, 0, 0, 0,
};

u32 gPlacement_Messages[] = {
    SCENE_EXITS(30),
    SCENE_EXIT(1, 2, 5),
    SCENE_EXIT(2, 34, 1),
    SCENE_EXIT(3, 31, 6),
    SCENE_EXIT(4, 31, 1),
    SCENE_EXIT(5, 31, 2),
    SCENE_EXIT(6, 31, 3),
    SCENE_EXIT(7, 31, 5),
    SCENE_EXIT(8, 31, 4),
    SCENE_EXIT(9, 9, 35),
    SCENE_EXIT(10, 32, 1),
    SCENE_EXIT(11, 32, 4),
    SCENE_EXIT(0, 32, 0),
    SCENE_EXIT(1, 30, 10),
    SCENE_EXIT(2, 32, 3),
    SCENE_EXIT(3, 32, 2),
    SCENE_EXIT(4, 30, 11),
    SCENE_EXIT(0, 35, 0),
    SCENE_EXIT(1, 2, 6),
    SCENE_EXIT(2, 2, 47),
    SCENE_EXITS_END,
};

struct ScenePlacement gBilibinPlacementsOther[] = {
    { SCENE_TABLE_END },
};

struct ScenePlacement gBilibinPlacements1[] = {
    { 0xf8, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(40), 0, PIXELS(140), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x69, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(256), 0, PIXELS(440), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(264), 0, PIXELS(328), FACING_SOUTHWEST, TALK_FACE_PARTY, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(312), 0, PIXELS(240), FACING_NORTHEAST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x65, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(176), 0, PIXELS(328), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x73, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(128), 0, PIXELS(224), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(176), 0, PIXELS(96), FACING_EAST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(192), 0, PIXELS(96), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6b, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(208), 0, PIXELS(200), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x66, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(192), 0, PIXELS(208), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x74, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(232), 0, PIXELS(296), FACING_SOUTHWEST, TALK_FACE_PARTY, 0 },
    { 0x87, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(392), 0, PIXELS(360), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0xf3, CONDITION_ALWAYS, 7,
        PIXELS(264), 0, PIXELS(216), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x74, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(264), 0, PIXELS(432), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gBilibinPlacements3[] = {
    { 0xf1, CONDITION_ALWAYS, 7,
        PIXELS(136), 0, PIXELS(300), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x69, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(272), 0, PIXELS(154), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0x69, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(272), 0, PIXELS(154), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0x69, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(272), 0, PIXELS(154), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(282), 0, PIXELS(170), FACING_WEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(246), 0, PIXELS(170), FACING_SOUTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(262), 0, PIXELS(156), FACING_WEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x17, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x16, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gBilibinPlacements2[] = {
    { 0xf3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(232), 0, PIXELS(100), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x5d, 0x6c, ACTOR_STAND,
        PIXELS(328), 0, PIXELS(152), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gBilibinEventsOther[] = {
    { SCENE_EVENTS_END },
};

struct SceneEvent gBilibinEvents1[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_EXIT, 10, CONDITION_ALWAYS, 10 },
    { EVENT_EXIT, 11, CONDITION_ALWAYS, 11 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(Bilibin_AnimateExitCells) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT_SCRIPT(Bilibin_AnimateExitCells) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT_SCRIPT(Bilibin_AnimateExitCells) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(Bilibin_AnimateExitCells) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(Bilibin_AnimateExitCells) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS, EVENT_SCRIPT(Bilibin_AnimateExitCells) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 9, CONDITION_ALWAYS, EVENT_SCRIPT(Bilibin_AnimateExitCells) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, EVENT_SCRIPT(Bilibin_ExamineTreeResemblingMan) },
    { EVENT_TALK, 21, CONDITION_ALWAYS, EVENT_SCRIPT(Bilibin_AskAboutTreeTransformation) },
    { EVENT_TALK, 10, 0x845, MSGNO(MsgBilibinWelcomeToBilibinPleaseJustIgnore) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgBilibinWelcomeToBilibinWellForgetThat) },
    { EVENT_TALK, 11, 0x845, MSGNO(MsgBilibinIHearALargeGroupCrossed) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgBilibinIfYoureTalkingAboutTheTravelers) },
    { EVENT_TALK, 12, 0x845, EVENT_SCRIPT(Bilibin_AskAboutEntranceTree) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgBilibinYoureWonderingAboutTheTreeNear) },
    { EVENT_TALK, 13, 0x845, MSGNO(MsgBilibinLordMccoyHasBeenQuiteFrightened) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgBilibinISawLordMccoyTheChief) },
    { EVENT_TALK, 14, 0x845, EVENT_SCRIPT(Bilibin_AskAboutSoutheastRoute) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgBilibinIHearTheBarricadeGateThat) },
    { EVENT_TALK, 15, 0x845, MSGNO(MsgBilibinTheRoadToKolimaIsBlocked) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgBilibinThoseThreeKidsVanishedDuringAll) },
    { EVENT_TALK, 16, 0x845, MSGNO(MsgBilibinEverSinceThatManTurnedInto) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, EVENT_SCRIPT(Bilibin_AskAboutCurseBreakers) },
    { EVENT_TALK, 17, 0x845, MSGNO(MsgBilibinIsItTrueThatAMountain) },
    { EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgBilibinLordMccoysMenSaidTheySaw) },
    { EVENT_TALK, 18, 0x845, MSGNO(MsgBilibinTheresATowerAtTheTip) },
    { EVENT_TALK, 18, CONDITION_ALWAYS, EVENT_SCRIPT(Bilibin_AskAboutImil) },
    { EVENT_TALK, 19, 0x845, MSGNO(MsgBilibinSomethingChangedInTheEruptionNow) },
    { EVENT_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgBilibinTheEruptionOnTheOtherSide) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgBilibinHelpMeSomebody) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgBilibinINeverWantToBeTurned) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, 0x845, MSGNO(MsgBilibinYouGuysAreSoUnluckyArriving) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgBilibinNotManyLumberjacksAreWillingTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, 0x845, MSGNO(MsgBilibinYouMustBeMadItsAlmost) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgBilibinOneOfTheTwoPeopleThat) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, 0x845, MSGNO(MsgBilibinWeEndedUpLikeThisBecause) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgBilibinThatGuyMightTurnIntoA) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, 0x845, MSGNO(MsgBilibinEveryoneGetsWorriedWhenTownLeaders) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgBilibinLordMccoySeemsToBeHaving) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, 0x845, MSGNO(MsgBilibinWhereDidTheseKidsComeFrom) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgBilibinApparentlyJustAboutAnyoneCouldJust) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, 0x845, MSGNO(MsgBilibinNoOneWouldDareGoAfter) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgBilibinTheGangOfThreeHaTheyre) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, 0x845, MSGNO(MsgBilibinHeBeggedForHelpAsHe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgBilibinImSureLordMccoyWillGive) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, 0x845, MSGNO(MsgBilibinIWantToSeeThatPretty) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgBilibinTheyMustBeReportingYourDeeds) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, 0x845, MSGNO(MsgBilibinOnlyGreatAdeptsCanEnterThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgBilibinIWantToShareWithYou) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, 0x845, MSGNO(MsgBilibinThatExplosionMustHaveBeenA) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgBilibinAsLongAsImOkayI) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 20, CONDITION_ALWAYS, EVENT_SCRIPT(Bilibin_UpdateVillageBlockCells) },
    { 0x202, 20, CONDITION_ALWAYS, EVENT_SCRIPT(Bilibin_PushFacedBlock) },
    { PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 8, CONDITION_ALWAYS, 0 },
    { SEARCH(SEARCH_JAR), 100, 0xf50, FIND_ITEM(0xb4) },
    { SEARCH(SEARCH_JAR), 101, 0xf51, FIND_COINS(5) },
    { SEARCH(SEARCH_UNNAMED), 90, CONDITION_ALWAYS, EVENT_SCRIPT(Bilibin_InspectWell) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gBilibinEvents3[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_TALK, 9, CONDITION_ALWAYS, EVENT_SCRIPT(Bilibin_InspectHumanShapedTree) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, EVENT_SCRIPT(Bilibin_InspectHumanShapedTree) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, EVENT_SCRIPT(Bilibin_InspectHumanShapedTree) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, EVENT_SCRIPT(Bilibin_InspectHumanShapedTree) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, IF_FLAG_SET(0x881), MSGNO(MsgBilibinTheyCrossedTheRiverAfterPushing) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgBilibinTheyreSoHeavy) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, 0x848, MSGNO(MsgBilibinShellBeSweptDownstreamIfShe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgBilibinOhItLooksLikeSheIs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, 0x848, MSGNO(MsgBilibinShellBeSweptDownstreamIfShe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgBilibinOhItLooksLikeSheIs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, 0x848, MSGNO(MsgBilibinAhhhhPleaseAnyonePleaseHelp) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgBilibinThankSavedMeFromBeing) },
    { EVENT_TALK, 12, 0x848, MSGNO(MsgBilibinIHeardWeWereOnceCalled) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgBilibinItWasAwfulLuckBeingTurned) },
    { EVENT_TALK, 13, 0x848, MSGNO(MsgBilibinIfOnlyWeHadntTriedGetting) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgBilibinIReallyLikeThisSpotBy) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgBilibinIWillThinkOfYouWhenever) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, 0x848, MSGNO(MsgBilibinWeWorkedSoHardIfOnly) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgBilibinWhoseIdeaWasItToPick) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, 0x848, MSGNO(MsgBilibinWhatACruelWorldWeJust) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgBilibinWellItWasntMyIdeaTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgBilibinThesePersimmonsAreJustLikeMy) },
    { SEARCH(SEARCH_UNNAMED), 11, CONDITION_ALWAYS, EVENT_SCRIPT(Bilibin_ReadRestrictedAreaSign) },
    { SEARCH(SEARCH_UNNAMED), 30, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgFieldDoorTightlyLocked)) },
    { EVENT_TOUCH, 13, 0x849, EVENT_SCRIPT(Bilibin_ReceiveJillsHardNut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 11, 0x848, EVENT_SCRIPT(Bilibin_RescueVillagerFromRiver) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 9, CONDITION_ALWAYS, EVENT_SCRIPT(Bilibin_CheckLeaderAhead) },
    { TOUCH_WALKING_INTO(FACING_WEST), 10, CONDITION_ALWAYS, EVENT_SCRIPT(Bilibin_CheckLeaderAhead) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(Bilibin_PushFacedBlock) },
    { 0x202, 12, CONDITION_ALWAYS, EVENT_SCRIPT(Bilibin_PushFacedBlock) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, CONDITION_ALWAYS, EVENT_SCRIPT(Bilibin_UpdateRiverBlockCells) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gBilibinEvents2[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { SEARCH(SEARCH_UNNAMED), 10, CONDITION_ALWAYS, EVENT_SCRIPT(Bilibin_ReadWarehouseSign) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, EVENT_SCRIPT(Bilibin_RecruitJupiterDjinni) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, CONDITION_ALWAYS, EVENT_SCRIPT(Bilibin_UpdateRaisedBlockCells) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 11, CONDITION_ALWAYS, EVENT_SCRIPT(Bilibin_PushFacedBlock) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 12, CONDITION_ALWAYS, EVENT_SCRIPT(Bilibin_PushFacedBlock) },
    { 0x202, 13, CONDITION_ALWAYS, EVENT_SCRIPT(Bilibin_PushFacedBlock) },
    { TOUCH_WALKING_INTO(FACING_EAST), 11, CONDITION_ALWAYS, EVENT_SCRIPT(Bilibin_CheckLeaderAhead) },
    { TOUCH_WALKING_INTO(FACING_WEST), 11, CONDITION_ALWAYS, EVENT_SCRIPT(Bilibin_CheckLeaderAhead) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 12, CONDITION_ALWAYS, EVENT_SCRIPT(Bilibin_CheckLeaderAhead) },
    { SCENE_EVENTS_END },
};

u16 gVillage_DoorCellStep6[] = {
    MAP_CELL_STEP(37, 33, 4, 2, 5), MAP_CELL_STEP(33, 33, 4, 2, 5), MAP_CELL_STEPS_END,
};

u16 gVillage_DoorCellStep4[] = {
    MAP_CELL_STEP(35, 43, 2, 2, 5), MAP_CELL_STEP(33, 43, 2, 2, 5), MAP_CELL_STEPS_END,
};

u16 gVillage_DoorCellStep5[] = {
    MAP_CELL_STEP(35, 52, 2, 2, 5), MAP_CELL_STEP(33, 52, 2, 2, 5), MAP_CELL_STEPS_END,
};

u16 gVillage_DoorCellStep1[] = {
    MAP_CELL_STEP(35, 54, 2, 2, 5), MAP_CELL_STEP(33, 54, 2, 2, 5), MAP_CELL_STEPS_END,
};

u16 gVillage_DoorCellStep0[] = {
    MAP_CELL_STEP(35, 48, 2, 2, 5), MAP_CELL_STEP(33, 48, 2, 2, 5), MAP_CELL_STEPS_END,
};

u16 gVillage_DoorCellStep2[] = {
    MAP_CELL_STEP(35, 46, 2, 2, 5), MAP_CELL_STEP(33, 46, 2, 2, 5), MAP_CELL_STEPS_END,
};

u16 gVillage_DoorCellStep3[] = {
    MAP_CELL_STEP(35, 50, 2, 2, 5), MAP_CELL_STEP(33, 50, 2, 2, 5), MAP_CELL_STEPS_END,
};

u16 *gVillage_DoorCellSteps[] = {
    gVillage_DoorCellStep0,
    gVillage_DoorCellStep1,
    gVillage_DoorCellStep2,
    gVillage_DoorCellStep3,
    gVillage_DoorCellStep4,
    gVillage_DoorCellStep5,
    gVillage_DoorCellStep6,
};

u32 gVillage_DoorCellOrigins[] = {
    0x140036, 0x12002b, 0x130025, 0xb0029, 0xb0023, 0x90031, 0x60037,
};

u32 gVillage_RepaintCells[] = {
    0x12091208, 0x1308120a, 0x130a1309, 0x1409130b, 0x140b140a,
};
// clang-format on

// Pixel step by direction: X in the upper halfword, Z in the lower.
#define DIRECTION_STEP(dx, dz) (((dx) << 16) | ((dz) & 0xffff))

s32 gBilibin_FacingCellSteps[] = {
    DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(16, 0),
};

s32 gVillage_SpawnScript[] = {
    SC_WAIT(30),
    SC_FREE,
};
