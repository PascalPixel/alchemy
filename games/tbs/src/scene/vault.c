#include "game_state.h"
#include "animspr.h"
#include "window.h"
#include "unit/scene/vault.h"
#include "unit/scene/common/vault_range.h"
#include "unit/scene/common/effect_spawn.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/battle/battle.h"
#include "unit/game/game.h"
#include "unit/runtime/runtime.h"
#include "field_obj.h"
#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "tbs_edition.h"
#include "battle_party.h"
#include "text/msg_ids.h"
#include "call.h"
#include "fixed_point_position.h"
#include "iwram_call.h"
#include "inventory.h"
#include "b5_context.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"
#include "fixed_math.h"

enum { VAULT_DRIFT_RATE = PIXELS(1) / 10 };

// Forward declarations.
static void Vault_ShowStandingActorMessage(s32 actor);
static void Vault_ShowMessageAndWait(s32 actor, s32 delay);
static void Vault_FaceActorAndWait(s32 actor, s32 cue, s32 delay);

// Shared source is included here in ROM order.
#include "common/effect_create.h"
#include "common/effect_spawn.h"

// Shared source is included here in ROM order.
#include "common/vault_nearby.h"

// Stop actor 21 near the leader or while an interaction forces it to pause.
// UB: calling this s32 function through a void update hook is incompatible.
// Plain void changes 108 to 104 bytes in tbs-en.
#ifdef UBFIX
static void Vault_UpdateActor21Proximity(union FieldObject *fieldObject) {
#else
static s32 Vault_UpdateActor21Proximity(union FieldObject *fieldObject) {
#endif
  struct FieldActor *entity = (struct FieldActor *)fieldObject;
  struct UiRenderWork *render = (struct UiRenderWork *)gWindowWork[0];
  u8 *workspace = gWindowWork[12];
  s32 flag = 0;
  s32 selector = 18;
  struct FieldActor *leader;

  if (entity->targetX == ACTOR_NO_TARGET)
#ifdef UBFIX
    return;
#else
    return 0;
#endif

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (*(s16 *)(workspace + 376) != 0 || render->mode != 0) {
    selector = 26;
    flag = 1;
  }
  SceneActor_CheckFacingAndRange(entity, leader, selector, flag);
#ifdef UBFIX
  return;
#else
  return 0;
#endif
}

// Return the town's entrance positions.
struct SceneEntrance *Vault_GetEntrances(void) { return gVault_Entrances; }

// This overlay callback returns zero.
int SceneData_ReturnZero(void) { return 0; }

// Return the town's exits.
u32 *Vault_GetExits(void) { return gVault_Exits; }

// Select placements before or after the thieves are captured.
struct ScenePlacement *Vault_SelectPlacements(void) {
  if (Engine_GameFlagIsSet(0x855) != 0)
    return gVault_PlacementsAfterThieves;
  return gVault_Placements;
}

// Count the Venus Djinn that must leave Ivan before he leaves the party.
static int Vault_GetIvanVenusDjinnCount(void) {
  return UnitGet(IVAN)->djinnOwnedCounts[ELEMENT_VENUS];
}

// Shared source is included here in ROM order.
#include "common/vault_djinn.h"

// Stop at the town exit so Ivan can say farewell and return his Djinn.
static void Vault_SayFarewellToIvan(void) {
  struct FieldActor *actor;
  struct EventWork *eventWork;

  eventWork = gEventWork;
  if (GameFlag_IsSet(0x855) != 0 || GameFlag_IsSet(0x856) == 0) {
    Engine_EventRequestExit(eventWork->touchedTrigger - 19);
    return;
  }
  // Bring Ivan out beside the party at the exit they are using.
  Engine_EventBegin();
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Actor_SetPosition(IVAN, actor->x.fixed, actor->z.fixed);
  }
  Actor_SetSpeed(IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
  if (eventWork->touchedTrigger == 20) {
    Actor_WalkToAndWait(IVAN, 400, 448);
  } else {
    Camera_SetSpeed(PIXELS(4) / 5, PIXELS(1) / 10);
    Camera_MoveTo(PIXELS(224), -1, PIXELS(162), 1);
    Actor_WalkToAndWait(IVAN, 224, 162);
    Engine_CameraWaitForMove();
  }
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, IVAN, 0);
  Engine_EventWait(20);
  Engine_EventSetMessage(MsgVaultLeavingImStillWorriedAbout);
  Event_ShowMessageAndWait(0x9002, 0, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
  // Return any Djinn before removing Ivan from the party.
  if (Vault_GetIvanVenusDjinnCount() != 0) {
    Engine_EventSetMessage(MsgVaultWaitDontWantTakeYour);
    Event_ShowMessage(IVAN, 0);
    OverlayObject_RunObject2WhenFlagged();
    Engine_TaskWait(20);
  }
  Party_RemoveOwnerRestored(2);
  Engine_EventRequestExit(eventWork->touchedTrigger - 19);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventEnd();
}

// Listen to the water rumbling beneath the well.
static void Vault_PeerIntoWell(void) {
  Engine_EventBegin();
  Engine_MessageShowCentered(MsgFieldPeeredWell, 1);
  Engine_MessageShowCentered(MsgVaultCanHearWaterRumblingDown, 1);
  Engine_EventEnd();
}

// Select interactions before or after the thieves are captured.
struct SceneEvent *Vault_SelectEvents(void) {
  if (Engine_GameFlagIsSet(0x855) != 0)
    return gVault_EventsAfterThieves;
  return gVault_Events;
}

// Ask the current question and select its ordinary or alternate reply.
static inline void Vault_AskYesNoAndShowReply(s32 speaker) {
  Engine_EventOpenMessage(speaker, 0);
  if (Engine_EventChooseYesNo(0, 0) != 0) {
    struct EventWork *eventWork = gEventWork;

    ++eventWork->message;
  }
  Engine_EventShowMessage(speaker, 0);
}

// Ask whether the party saw Mt. Aleph's eruption.
static void Vault_DiscussEruption(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultWasntEruptionMtAlephIncredible);
  Vault_FaceActorAndWait(9, 0, 2);
  Event_OpenMessage(9, 0);
  if (Engine_EventChooseYesNo(0, 0) != 0) {
    gEventWork->message++;
  }
  Event_ShowMessage(9, 0);
  Actor_FaceDirection(9, 0x5000, 0);
  Engine_EventEnd();
}

// Ask about the travelers who left town in a hurry.
static void Vault_AskAboutDepartedTravelers(void) {

  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultThoseTravelersLeftInBig);
  Engine_ActorSetAnimation(13, 1);
  Vault_FaceActorAndWait(13, 0, 2);
  Vault_AskYesNoAndShowReply(13);
  Engine_EventEnd();
}

// Respond to the accusation about Hammet's stolen treasure.
static void Vault_DiscussTheftAccusation(void) {

  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultAccusingUsStealingHammetsTreasured);
  Vault_FaceActorAndWait(17, 0, 2);
  Vault_AskYesNoAndShowReply(17);
  Engine_EventEnd();
}

// Ask whether the party is setting out on an adventure.
static void Vault_AskAboutAdventure(void) {

  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultOffOnAdventure);
  Vault_FaceActorAndWait(18, 0, 2);
  Vault_AskYesNoAndShowReply(18);
  Engine_EventEnd();
}

// Discuss the thefts committed during the eruption's confusion.
static void Vault_DiscussEruptionThefts(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultGuessFolksGot);
  Vault_ShowStandingActorMessage(11);
  Engine_EventEnd();
}

// Discuss the watch for out-of-town thieves.
static void Vault_DiscussWatchingForThieves(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultNobodysStealingAnything);
  Vault_ShowStandingActorMessage(16);
  Engine_EventEnd();
}

// Stop the dog long enough to hear its bark.
static void Vault_TalkToDog(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultRuffRrruff);
  Engine_ActorSetAnimation(19, 0);
  Vault_FaceActorAndWait(19, 0, 2);
  Engine_EventShowMessage(19, 0);
  Engine_EventEnd();
}

// Pause the villager's walk while discussing Ivan's search.
static void Vault_DiscussIvanSearchingForRod(void) {
  struct FieldActor *actor = Object_GetById(14);
  s16 savedFacing = actor->facing;

  actor->timer |= 2;
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultPoorGuyLeft);
  Engine_ActorSetAnimation(14, 0);
  Vault_FaceActorAndWait(14, 0, 2);
  Vault_ShowMessageAndWait(14, 10);
  actor->facing = savedFacing;
  Engine_TaskWait(1);
  Engine_EventEnd();
  actor->timer &= 1;
}

// Pause the villager's walk while discussing Hammet's caravan.
static void Vault_DiscussHammetsCaravan(void) {
  struct FieldActor *actor = Object_GetById(15);
  s16 savedFacing = actor->facing;

  actor->timer |= 2;
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultMasterHammetsCaravan);
  Engine_ActorSetAnimation(15, 0);
  Vault_FaceActorAndWait(15, 0, 2);
  Vault_ShowMessageAndWait(15, 10);
  actor->facing = savedFacing;
  Engine_TaskWait(1);
  Engine_EventEnd();
  actor->timer &= 1;
}

// Ask where the missing roof repairer has gone.
static void Vault_TalkAboutMissingRoofRepairer(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultFixingRoofCant);
  Vault_FaceActorAndWait(21, 0, 2);
  Engine_ActorShowEmote(21, 0x103, 0);
  Engine_EventWait(30);
  Engine_EventOpenMessage(21, 0);
  Engine_EventEnd();
}

// Stop an actor, face the leader and show the current message.
static void Vault_ShowStandingActorMessage(s32 actor) {
  Engine_EventBegin();
  Engine_ActorSetAnimation(actor, 1);
  Vault_FaceActorAndWait(actor, 0, 2);
  Engine_EventShowMessage(actor, 0);
  Engine_EventEnd();
}

// Shared source is included here in ROM order.
#include "common/vault_talk.h"

// Ask whether the party is taking to the road again.
static void Vault_AskAboutLeavingAgain(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultYoureHittingRoadAgain);
  Vault_FaceActorAndWait(8, 0, 2);
  Vault_AskYesNoAndShowReply(8);
  Engine_EventEnd();
}

// Thank the party for catching the thieves.
static void Vault_PraiseThievesCapture(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultGreatCaughtThieves);
  Vault_ShowStandingActorMessage(11);
  Engine_EventEnd();
}

// Discuss Ivan, with a different line while he is in the party.
static void Vault_TalkAboutIvan(void) {
  struct EventWork *workspace;
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultTalkingAboutHammetsServantIvan);
  if (Engine_GameFlagIsSet(2) != 0) {
    workspace = gEventWork;
    ++workspace->message;
  }
  Vault_ShowStandingActorMessage(12);
  Engine_EventEnd();
}

// Discuss the roads damaged by the eruption.
static void Vault_DiscussDamagedRoads(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultEruptionDevastatedRoads);
  Vault_ShowStandingActorMessage(13);
  Engine_EventEnd();
}

// Discuss Hammet's trip to Lunpa, then restore the villager's facing.
static void Vault_DiscussLunpa(void) {
  struct FieldActor *actor = Object_GetById(14);
  s16 savedFacing = actor->facing;

  actor->timer |= 2;
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultDidntGoLunpa);
  if (Engine_GameFlagIsSet(2) != 0)
    ++gEventWork->message;
  Engine_ActorSetAnimation(14, 0);
  Vault_FaceActorAndWait(14, 0, 2);
  Vault_ShowMessageAndWait(14, 10);
  actor->facing = savedFacing;
  Engine_TaskWait(1);
  Engine_EventEnd();
  actor->timer &= 1;
}

// Deliver the town's thanks, then restore the villager's facing.
static void Vault_ThankPartyForCapturingThieves(void) {
  struct FieldActor *actor = Object_GetById(15);
  s16 savedFacing = actor->facing;

  actor->timer |= 2;
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultEveryoneGratefulCaptured);
  Engine_ActorSetAnimation(15, 0);
  Vault_FaceActorAndWait(15, 0, 2);
  Vault_ShowMessageAndWait(15, 10);
  actor->facing = savedFacing;
  Engine_TaskWait(1);
  Engine_EventEnd();
  actor->timer &= 1;
}

// Ask about the captured thieves now held in town.
static void Vault_DiscussCapturedPrisoners(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultThoseMenCapturedTheyreIn);
  Engine_ActorSetAnimation(16, 1);
  Vault_FaceActorAndWait(16, 0, 2);
  Vault_AskYesNoAndShowReply(16);
  Engine_EventEnd();
}

// Ask whether these are the people who captured the thieves.
static void Vault_AskAboutCapturingThieves(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultOnesWhoCapturedThieves);
  Vault_FaceActorAndWait(18, 0, 2);
  Vault_AskYesNoAndShowReply(18);
  Engine_EventEnd();
}

// Let the dog notice a carried bone after the thieves are captured.
static void Vault_TalkToDogAfterCapture(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultRuffRrruff2);
  Engine_ActorSetAnimation(19, 0);
  Vault_FaceActorAndWait(19, 0, 2);
  Engine_EventShowMessage(19, 0);
  Engine_ActorSetAnimation(19, 1);
  if (PartyInventory_FindOwner(ITEM_BONE) != -1 &&
      Engine_GameFlagIsSet(0x858) == 0) {
    u16 *p = &gEventWork->unk172;
    u16 value = 1;

    *p = value;
  }
  Engine_EventEnd();
}

// Thank the party again before it leaves town.
static void Vault_ThankPartyBeforeDeparture(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultThankForOtherDayLeaving);
  Vault_FaceActorAndWait(20, 0, 2);
  Engine_ActorSetAnimationAndWait(20, 3);
  Engine_EventWait(20);
  Engine_EventShowMessage(20, 0);
  Engine_EventEnd();
}

// Read thoughts about the thefts or the captured prisoners' punishment.
static void Vault_ReadThoughtsOnTheft(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(0x855) == 0)
    Engine_EventSetMessage(MsgVaultManShouldStealFromAnother);
  else
    Engine_EventSetMessage(MsgVaultHowPunishPrisoners);
  SceneActor_RunActorCommandWithFlag91(11);
  Engine_EventEnd();
}

// Read thoughts about the strange travelers or the remaining route out.
static void Vault_ReadThoughtsOnTravelers(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(0x855) == 0) {
    Engine_EventSetMessage(MsgVaultGroupTravelersWasStrangeBunch);
  } else {
    Engine_EventSetMessage(MsgVaultWithRoadOutOnlyWay);
  }
  SceneActor_RunActorCommandWithFlag91(13);
  Engine_EventEnd();
}

// Read suspicions about the thieves or concern over Ivan's whereabouts.
static void Vault_ReadThoughtsOnIvan(void) {
  Object_GetById(14)->timer |= 2;
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(0x855) == 0) {
    Engine_EventSetMessage(MsgVaultBetWasThoseThreeCreeps);
  } else {
    Engine_EventSetMessage(MsgVaultWhereDidIvanGoBy);
    if (Engine_GameFlagIsSet(2) != 0)
      ++gEventWork->message;
  }
  SceneActor_RunActorCommandWithFlag91(14);
  Engine_EventEnd();
  Object_GetById(14)->timer &= 1;
}

// Read the villager's thoughts before or after the thieves' capture.
static void Vault_ReadThoughtsOnWealth(void) {
  Object_GetById(15)->timer |= 2;
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(0x855) == 0)
    Engine_EventSetMessage(MsgVaultSupposeDoesntMatterHowRich);
  else
    Engine_EventSetMessage(MsgVaultMustStrongerThanLookHave);
  SceneActor_RunActorCommandWithFlag91(15);
  Engine_EventEnd();
  Object_GetById(15)->timer &= 1;
}

// Read the villager's suspicions or relief after the thieves' capture.
static void Vault_ReadThoughtsOnSuspects(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(0x855) == 0)
    Engine_EventSetMessage(MsgVaultEveryoneKnowsThoseThreeAt);
  else
    Engine_EventSetMessage(MsgVaultFeelMuchBetter);
  SceneActor_RunActorCommandWithFlag91(16);
  Engine_EventEnd();
}

// Pause the dog while reading its thoughts about theft, bones or the passage.
static void Vault_ReadDogThoughts(void) {
  struct FieldActor *actor = Object_GetById(19);
  actor->paused = 1;
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(0x855) == 0) {
    Engine_EventSetMessage(MsgVaultStealingInMidstVolcanicEruption);
    Engine_ActorSetAnimation(19, 0);
    Engine_EventWait(2);
  } else if (Engine_GameFlagIsSet(0x858) != 0) {
    Engine_EventSetMessage(MsgVaultCarefulSearchWillRevealPassage);
  } else {
    Engine_EventSetMessage(MsgVaultIfBringMeBoneIll);
  }
  Engine_EventShowMessage(19, 0);
  Engine_EventEnd();
  actor->paused = 0;
}

// Read the villager's thoughts about the missing roof repairer.
static void Vault_ReadThoughtsOnRoofRepairer(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultSneakWastingTime);
  SceneActor_RunActorCommandWithFlag91(21);
  Engine_EventEnd();
}

// Shared source is included here in ROM order.
#include "common/vault_toggle.h"

// Walk to the exit threshold and request the room's transition.
static void Vault_WalkThroughExit(s32 x, s32 y, s32 continuation) {
  Actor_SetSpeed(0, PIXELS(1) / 2, PIXELS(1) / 4);
  Actor_WalkTo(ACTOR_PARTY_LEADER, x, y);
  gEventWork->transitionFrames = 16;
  Engine_EventRequestExit(continuation);
}

// Open and enter the southeastern house.
static void Vault_EnterSoutheastHouse(void) {
  Engine_AudioPlayCue(158);
  Engine_MapAnimateCells(gVault_Scene5Cells, 56, 19);
  Vault_WalkThroughExit(408, 320, 5);
}

// Open and enter the house east of the town's center.
static void Vault_EnterCentralEastHouse(void) {
  Engine_AudioPlayCue(158);
  Engine_MapAnimateCells(gVault_Scene6Cells, 50, 18);
  Vault_WalkThroughExit(312, 304, 6);
}

// Open and enter the central house.
static void Vault_EnterCentralHouse(void) {
  Engine_AudioPlayCue(158);
  Engine_MapAnimateCells(gVault_Scene7Cells, 44, 17);
  Vault_WalkThroughExit(216, 288, 7);
}

// Enter the northeastern doorway behind its foreground tiles.
static void Vault_EnterNortheastHouse(void) {
  struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);
  struct AnimationObject *sprite = actor->sprite;

  Engine_AudioPlayCue(158);
  Engine_MapAnimateCells(gVault_Scene8Cells, 54, 13);
  {

    Map_CopyCellAttributes(33, 20, 1, 3, 23, 12);
  }
  actor->priorityFlags &= ~1;
  sprite->image.priority = 3;
  Vault_WalkThroughExit(376, 224, 8);
}

// Enter the northern doorway behind its foreground tiles.
static void Vault_EnterNorthHouse(void) {
  struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);
  struct AnimationObject *sprite = actor->sprite;

  Engine_AudioPlayCue(158);
  Engine_MapAnimateCells(gVault_Scene9Cells, 49, 10);
  {

    Map_CopyCellAttributes(33, 20, 1, 3, 18, 10);
  }
  actor->priorityFlags &= ~1;
  sprite->image.priority = 3;
  Vault_WalkThroughExit(296, 176, 9);
}

// Open and enter the northwestern house.
static void Vault_EnterNorthwestHouse(void) {
  Engine_AudioPlayCue(158);
  Engine_MapAnimateCells(gVault_Scene10Cells, 38, 6);
  Vault_WalkThroughExit(120, 144, 10);
}

// Shared source is included here in ROM order.
#include "common/vault_scene_cells.h"

// Enter the western doorway behind its foreground tiles.
static void Vault_EnterWestHouse(void) {
  struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);
  struct AnimationObject *sprite = actor->sprite;

  Engine_AudioPlayCue(158);
  Engine_MapAnimateCells(gVault_Scene12Cells, 35, 9);
  {

    Map_CopyCellAttributes(33, 20, 1, 3, 4, 10);
  }
  actor->priorityFlags &= ~1;
  sprite->image.priority = 3;
  Vault_WalkThroughExit(72, 160, 12);
}

// Walk through the town's remaining passage.
static void Vault_EnterPassage(void) {
  Engine_AudioPlayCue(123);
  Vault_WalkThroughExit(152, 264, 13);
}

// Move an actor through a jump and wait up to sixty frames for it to settle.
static void Vault_JumpActorToAndWait(s32 actorId, s32 x, s32 z,
                                     s32 verticalSpeed) {
  struct FieldActor *actor = Object_GetById(actorId);
  int frames;
  Engine_ActorSetSpeed(actorId, PIXELS(3), PIXELS(3) / 2);
  actor->gravity = PIXELS(1) / 2;
  actor->bounce = 0;
  actor->velocityY = verticalSpeed;
  Engine_ActorSetSpriteFlags(actor, 0);
  Engine_ActorMoveToAndWait(actorId, x, z);
  Engine_ActorSetPosition(actorId, x << 16, z << 16);
  for (frames = 60; frames != 0; --frames) {
    Engine_TaskWait(1);
    if ((actor->velocityY >> 16) == 0)
      break;
  }
  Engine_ActorSetSpriteFlags(actor, 1);
  actor->gravity = PIXELS(1);
}

// Ring the bell and make actor 23 jump down once.
static void Vault_RingBell(void) {
  Engine_EventBegin();
  Event_PlayCue(100);
  Engine_EventWait(40);
  if (GameFlag_IsSet(0x867) == 0) {
    Actor_SetAttachedEffect(23, 0x102);
    Engine_ActorJump(23, 4, 0);
    Engine_EventWait(12);
    Engine_ActorJump(23, 4, 0);
    Engine_EventWait(20);
    Vault_JumpActorToAndWait(23, 392, 104, PIXELS(7));
    Engine_EventWait(20);
    Actor_WalkToAndWait(23, 408, 104);
    Actor_WalkToAndWait(23, 408, 120);
    GameFlag_Set(0x867);
  }
  Engine_EventEnd();
}

// Spend a bone so the dog indicates the hidden passage.
static void Vault_GiveBoneToDog(void) {
  PartyInventory_Discard(ITEM_BONE);
  Engine_EventBegin();
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(19, 2);
  Actor_SetSpeed(19, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(19, 216, 408);
  Engine_EventWait(10);
  Actor_FaceDirection(19, 0x4000, 20);
  Engine_ActorJump(19, 6, 0);
  Engine_EventWait(30);
  Engine_ActorJump(19, 6, 0);
  Engine_EventWait(30);
  Engine_ActorJump(19, 6, 0);
  Engine_EventWait(30);
  Actor_WalkToAndWait(19, 216, 392);
  Engine_EventWait(10);
  Actor_FaceDirection(19, 0x4000, 20);
  GameFlag_Set(0x858);
  Engine_EventEnd();
}

// Restore town changes, facing callbacks and Ivan's entrance greeting.
s32 Vault_RestoreEntryState(void) {
  s32 actor21Flag;
  struct FieldActor *actor;
  s32 lowBit;
  s32 zero;

  if (Engine_GameFlagIsSet(0x87a) != 0) {
    Engine_EventRequestExit(14);
  }
  // Restore the passage cells and the two villagers' proximity callbacks.
  if (Engine_GameFlagIsSet(0x200) != 0) {
    Map_CopyCellAttributes(55, 26, 4, 2, 23, 26);
  }
  OverlayObject_CreateConfiguredObject(PIXELS(128), 0, PIXELS(420), 223);
  lowBit = 1;
  zero = 0;
  actor = Object_GetById(14);
  actor->timer = lowBit;
#ifdef UBFIX
  actor->update = SceneActor_UpdateProximityToLeader;
#else
  actor->update =
      (void (*)(union FieldObject *))SceneActor_UpdateProximityToLeader;
#endif
  actor = Object_GetById(15);
  actor->timer = zero;
#ifdef UBFIX
  actor->update = SceneActor_UpdateProximityToLeader;
#else
  actor->update =
      (void (*)(union FieldObject *))SceneActor_UpdateProximityToLeader;
#endif
  if (Engine_GameFlagIsSet(0x858) != 0) {
    Actor_SetPosition(19, PIXELS(216), PIXELS(392));
  }
  actor21Flag = Engine_GameFlagIsSet(0x853);
  if (Engine_GameFlagIsSet(0x855) == 0) {
    if ((lowBit & actor21Flag) != 0) {
      actor = Object_GetById(21);
#ifdef UBFIX
      actor->update = Vault_UpdateActor21Proximity;
#else
      actor->update =
          (void (*)(union FieldObject *))Vault_UpdateActor21Proximity;
#endif
    }
  }
  // Before the thieves are captured, Ivan welcomes the party back at the gates.
  if (gGameState.entrance <= 2) {
    if (Engine_GameFlagIsSet(0x109) == 0) {
      Engine_GameFlagClear(0x867);
      if (Engine_GameFlagIsSet(0x855) == 0) {
        if (Engine_GameFlagIsSet(0x856) != 0) {
          Engine_EventBegin();
          actor = Object_GetById(0);
          if (actor != 0) {
            Engine_ActorSetPosition(2, actor->x.fixed, actor->z.fixed);
          }
          if (gGameState.entrance == 1) {
            Actor_SetPosition(2, PIXELS(400), PIXELS(448));
          } else {
            Actor_SetPosition(2, PIXELS(224), PIXELS(162));
          }
          Engine_ActorFaceEachOther(2, 0, 0);
          Engine_EventOpenScreen();
          Engine_EventWaitForScreen();
          Engine_EventWait(30);
          Engine_ActorRunRepeatedMotion(2, 2);
          Engine_EventSetMessage(MsgVaultWelcomeBackTwo);
          Engine_EventShowMessageAndWait(2, 0, 20);
          Engine_ActorSetAnimationAndWait(0, 3);
          Actor_SetSpeed(2, PIXELS(4) / 5, PIXELS(2) / 5);
          Engine_ActorSetAnimation(2, 2);
          actor = Object_GetById(0);
          if (actor != 0) {
            Engine_ActorSetDestination(2, actor->x.part.pixel,
                                       actor->z.part.pixel);
          }
          Engine_ActorWaitForMove(2);
          Engine_ActorSetPosition(2, 0, 0);
          Event_PrepareObjectAndApplyValue(2, 0);
          Engine_EventEnd();
        }
      }
    }
  }
  if (Engine_GameFlagIsSet(0x867) != 0) {
    Actor_SetPosition(23, PIXELS(408), PIXELS(120));
  }
  // Use the same greeting for the western entrance while Ivan is absent.
  if (gGameState.entrance == 11) {
    if (Engine_GameFlagIsSet(0x855) == 0) {
      if (Engine_GameFlagIsSet(0x856) != 0) {
        if (Engine_GameFlagIsSet(2) == 0) {
          Engine_EventBegin();
          Actor_SetPosition(2, PIXELS(40), PIXELS(310));
          Engine_ActorFaceEachOther(2, 0, 0);
          Engine_EventOpenScreen();
          Engine_EventWaitForScreen();
          Engine_EventWait(30);
          Engine_ActorRunRepeatedMotion(2, 2);
          Engine_EventSetMessage(MsgVaultWelcomeBackTwo);
          Engine_EventShowMessageAndWait(2, 0, 20);
          Engine_ActorSetAnimationAndWait(0, 3);
          Actor_SetSpeed(2, PIXELS(4) / 5, PIXELS(2) / 5);
          Engine_ActorSetAnimation(2, 2);
          actor = Object_GetById(0);
          if (actor != 0) {
            Engine_ActorSetDestination(2, actor->x.part.pixel,
                                       actor->z.part.pixel);
          }
          Engine_ActorWaitForMove(2);
          Engine_ActorSetPosition(2, 0, 0);
          Event_PrepareObjectAndApplyValue(2, 0);
          Engine_EventEnd();
        }
      }
    }
    Engine_GameFlagClear(0x12f);
  } else {
    if (gGameState.entrance == 13) {
      if (Engine_GameFlagIsSet(0x855) != 0) {
        Engine_ActorSetPosition(20, 0, 0);
      }
    }
  }
  return 0;
}

// Shared source is included here in ROM order.
#include "common/object_distance.h"
#include "common/vault_range.h"

// Show the current message and pause before continuing the scene.
static void Vault_ShowMessageAndWait(s32 actor, s32 delay) {
  Engine_EventShowMessage(actor, 0);
  Engine_EventWait(delay);
}

// Face another actor and pause before continuing the conversation.
static void Vault_FaceActorAndWait(s32 actor, s32 cue, s32 delay) {
  Engine_ActorFaceEachOther(actor, cue, 0);
  Engine_EventWait(delay);
}

// Apply drift and gradually damp both rates toward zero.
// This custom update uses speed/acceleration as its two drift rates and mirrors
// the resulting position into the actor's target coordinates.
static void Vault_UpdateDriftingEffect(union FieldObject *fieldObject) {
  struct FieldActor *effect = (struct FieldActor *)fieldObject;
  s32 horizontalRate;
  s32 verticalRate;

  effect->x.fixed += effect->speed;
  effect->targetX = effect->x.fixed;

  if (effect->timer != 0) {
    effect->y.fixed += effect->acceleration;
    effect->targetY = effect->y.fixed;
  } else {
    effect->z.fixed += effect->acceleration;
    effect->targetZ = effect->z.fixed;
    effect->y.fixed += PIXELS(1) / 64;
    effect->targetY = effect->y.fixed;
  }

  horizontalRate = effect->speed;
  effect->speed = horizontalRate - Math_Divide(horizontalRate, 28);
  verticalRate = effect->acceleration;
  effect->acceleration = verticalRate - Math_Divide(verticalRate, 28);
}

// Spawn a drifting sprite near the dog, sharing its drawing priority.
UNUSED static void Vault_SpawnDriftingEffect(s32 flags) {
  struct FieldActor *anchor;
  struct FieldActor *effect;
  struct AnimationObject *sprite;
  s32 x;
  s32 z;
  u8 zero;

  anchor = (struct FieldActor *)Object_GetById(19);
  if (anchor == NULL)
    return;
  // Pick a small offset around the anchor and create the drifting sprite there.
  x = Engine_RandomNext();
  z = Engine_RandomNext();
  x <<= 3;
  x = (u32)x >> 16;
  z <<= 3;
  x -= 4;
  z = (u32)z >> 16;
  z -= 4;
  x <<= 16;
  z <<= 16;
  {
    s32 baseX = anchor->x.fixed;
    s32 baseZ = anchor->z.fixed;
    s32 baseY;

    x += baseX;
    baseY = anchor->y.fixed;
    z += baseZ;
    effect = (struct FieldActor *)Engine_ObjectCreate(0xac, x, baseY, z);
  }
  if (effect == NULL)
    return;
  sprite = effect->sprite;
  switch (Engine_RandomNext() & 1) {
  case 1:
    Object_SetMode(effect, 3);
    Engine_ObjectSetScript(effect, gVault_DriftScriptA);
    break;
  default:
    Object_SetMode(effect, 2);
    Engine_ObjectSetScript(effect, gVault_DriftScriptB);
    break;
  }
  {
    u8 *motionFlags = &effect->motionFlags;

    zero = 0;
    *motionFlags = zero;
  }
  // Choose either depth drift with a slow rise or vertical drift.
  if (flags & 2) {
    s32 driftMultiplier;
    s32 directionBias;

    driftMultiplier = (u32)Engine_RandomNext() % 10 + 5;
    /* FAKEMATCH: reuse the coordinate local for the direction mask. With the
     * literal 1, also as (flags ^ 1) << 2 or through a mask local of its own:
     * differ 116. */
    x = 1;
    flags &= x;
    directionBias = flags ^ x;
    directionBias <<= 2;
    driftMultiplier += directionBias;
    effect->acceleration =
        (2 * VAULT_DRIFT_RATE * flags - VAULT_DRIFT_RATE) * driftMultiplier;
    driftMultiplier = (u32)Engine_RandomNext() % 15 - 7;
    effect->speed = VAULT_DRIFT_RATE * driftMultiplier;
    effect->timer = zero;
  } else {
    s32 driftMultiplier;

    driftMultiplier = (u32)Engine_RandomNext() % 10 + 8;
    effect->speed =
        (2 * VAULT_DRIFT_RATE * flags - VAULT_DRIFT_RATE) * driftMultiplier;
    driftMultiplier = (u32)Engine_RandomNext() % 14 + 1;
    effect->acceleration = VAULT_DRIFT_RATE * driftMultiplier;
    effect->timer = 1;
  }
  // Install the custom motion and share the anchor's drawing priority.
  {
    void (*update)(union FieldObject *) = Vault_UpdateDriftingEffect;
    u8 *spriteFlags;

    /* FAKEMATCH: preserve the sprite-flags address scheduling boundary. As
     * plain stores, sprite->flags = 0 after the update pointer or through the
     * pointer local: differ 2; the flags stored first: differ 4. */
    do {
      spriteFlags = &sprite->flags;
    } while (0);
    effect->update = update;
    *spriteFlags = 0;
  }
  sprite->image.priority = anchor->sprite->image.priority;
}

s32 gEffectScript0[] = {
    SC_WAIT(1),      SC_LOOP(22, SC_TOP), SC_WAIT(1),
    SC_POS(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gEffectScript1[] = {
    SC_WAIT(1),      SC_LOOP(44, SC_TOP), SC_WAIT(1),
    SC_POS(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gEffectScript2[] = {
    SC_WAIT(1),      SC_LOOP(126, SC_TOP), SC_WAIT(1),
    SC_POS(0, 0, 0), SC_WAIT(1),           SC_FREE,
};

const s32 *gEffectScripts[] = {
    gEffectScript0,
    gEffectScript1,
    gEffectScript2,
};

s32 gVault_Actor2[] = {
    SC_SET(SW_SPEED, PIXELS(2) / 5),
    SC_SET(SW_ACCEL, PIXELS(1) / 5),
    SC_LABEL(1),
    SC_SET(SW_FACING, FACING_WEST),
    SC_ARG(2),
    SC_MOVE(PIXELS(168), 0, PIXELS(312)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(20),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(20),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(20),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(20),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(20),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(10),
    SC_ARG(4),
    SC_WAIT(240),
    SC_SET(SW_FACING, FACING_EAST),
    SC_ARG(2),
    SC_MOVE(PIXELS(200), 0, PIXELS(312)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(20),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(20),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(20),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(20),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(20),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(10),
    SC_ARG(4),
    SC_WAIT(240),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
    SC_SET(SW_SPEED, PIXELS(2) / 5),
    SC_SET(SW_ACCEL, PIXELS(1) / 5),
    SC_SET(SW_COLLISION, 129),
    SC_SET(SW_RADIUS, 0x280000),
    SC_LABEL(1),
    SC_ARG(2),
    SC_WANDER_HOME(0x80000, 0x100000, 0x180000),
    SC_SYNC,
    SC_ARG(0),
    SC_WAIT(40),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
};

s32 gVault_Actor0[] = {
    SC_ARG(2),
    SC_SET(SW_SPEED, PIXELS(4) / 5),
    SC_SET(SW_ACCEL, PIXELS(1) / 5),
    SC_LABEL(1),
    SC_MOVE(PIXELS(136), 0, PIXELS(376)),
    SC_SYNC,
    SC_MOVE(PIXELS(120), 0, PIXELS(376)),
    SC_SYNC,
    SC_MOVE(PIXELS(104), 0, PIXELS(392)),
    SC_SYNC,
    SC_MOVE(PIXELS(104), 0, PIXELS(408)),
    SC_SYNC,
    SC_MOVE(PIXELS(120), 0, PIXELS(424)),
    SC_SYNC,
    SC_MOVE(PIXELS(136), 0, PIXELS(424)),
    SC_SYNC,
    SC_MOVE(PIXELS(152), 0, PIXELS(408)),
    SC_SYNC,
    SC_MOVE(PIXELS(152), 0, PIXELS(392)),
    SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
};

s32 gVault_Actor1[] = {
    SC_ARG(2),
    SC_SET(SW_SPEED, PIXELS(4) / 5),
    SC_SET(SW_ACCEL, PIXELS(1) / 5),
    SC_LABEL(1),
    SC_MOVE(PIXELS(120), 0, PIXELS(424)),
    SC_SYNC,
    SC_MOVE(PIXELS(136), 0, PIXELS(424)),
    SC_SYNC,
    SC_MOVE(PIXELS(152), 0, PIXELS(408)),
    SC_SYNC,
    SC_MOVE(PIXELS(152), 0, PIXELS(392)),
    SC_SYNC,
    SC_MOVE(PIXELS(136), 0, PIXELS(376)),
    SC_SYNC,
    SC_MOVE(PIXELS(120), 0, PIXELS(376)),
    SC_SYNC,
    SC_MOVE(PIXELS(104), 0, PIXELS(392)),
    SC_SYNC,
    SC_MOVE(PIXELS(104), 0, PIXELS(408)),
    SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
};

struct SceneEntrance gVault_Entrances[] = {
    {0, CONDITION_ALWAYS, 223, 0, 135, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 400, 0, 472, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {2, CONDITION_ALWAYS, 224, 0, 138, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {5, CONDITION_ALWAYS, 408, 0, 344, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {6, CONDITION_ALWAYS, 312, 0, 328, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {7, CONDITION_ALWAYS, 216, 0, 312, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {8, CONDITION_ALWAYS, 376, 0, 248, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {9, CONDITION_ALWAYS, 296, 0, 200, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {10, CONDITION_ALWAYS, 120, 0, 168, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {11, CONDITION_ALWAYS, 64, 0, 294, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {12, CONDITION_ALWAYS, 70, 0, 183, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {13, CONDITION_ALWAYS, 152, 0, 272, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gVault_Exits[] = {
    SCENE_EXITS(20),       SCENE_EXIT(1, 2, 3),   SCENE_EXIT(2, 2, 44),
    SCENE_EXIT(5, 21, 5),  SCENE_EXIT(6, 21, 6),  SCENE_EXIT(7, 21, 7),
    SCENE_EXIT(8, 21, 8),  SCENE_EXIT(9, 21, 9),  SCENE_EXIT(10, 21, 10),
    SCENE_EXIT(11, 9, 10), SCENE_EXIT(12, 22, 1), SCENE_EXIT(13, 21, 13),
    SCENE_EXIT(14, 23, 1), SCENE_EXITS_END,
};

// clang-format off
struct ScenePlacement gVault_Placements[] = {
    { IVAN, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(381), 0, PIXELS(342), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(280), 0, PIXELS(396), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x65, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(264), 0, PIXELS(150), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x73, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(244), 0, PIXELS(204), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(124), 0, PIXELS(196), FACING_SOUTHEAST, TALK_FACE_PARTY, 0 },
    { 0x6f, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(244), 0, PIXELS(300), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x6b, CONDITION_ALWAYS, (s32)gVault_Actor0,
        PIXELS(132), 0, PIXELS(380), FACING_WEST, TALK_KEEP_FACING, 0 },
    { 0x66, CONDITION_ALWAYS, (s32)gVault_Actor1,
        PIXELS(132), 0, PIXELS(420), FACING_EAST, TALK_KEEP_FACING, 0 },
    { 0x74, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(332), 0, PIXELS(412), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x40, 0x854, ACTOR_STAND,
        PIXELS(92), 0, PIXELS(284), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x7a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(92), 0, PIXELS(296), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x68, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(216), 0, PIXELS(392), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x6c, 0x853, ACTOR_STAND,
        PIXELS(152), 0, PIXELS(296), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x6a, IF_FLAG_SET(0x853), (s32)gVault_Actor2,
        PIXELS(168), 0, PIXELS(312), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0xf8, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0x5a, 0x34, ACTOR_STAND,
        PIXELS(344), 0, PIXELS(96), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gVault_PlacementsAfterThieves[] = {
    { 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(381), 0, PIXELS(342), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(280), 0, PIXELS(396), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x65, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(264), 0, PIXELS(150), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x73, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(244), 0, PIXELS(204), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(124), 0, PIXELS(196), FACING_SOUTHEAST, TALK_FACE_PARTY, 0 },
    { 0x6f, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(244), 0, PIXELS(300), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x6b, CONDITION_ALWAYS, (s32)gVault_Actor0,
        PIXELS(132), 0, PIXELS(380), FACING_WEST, TALK_KEEP_FACING, 0 },
    { 0x66, CONDITION_ALWAYS, (s32)gVault_Actor1,
        PIXELS(132), 0, PIXELS(420), FACING_EAST, TALK_KEEP_FACING, 0 },
    { 0x74, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(332), 0, PIXELS(412), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x16, 0x854, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x7a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(92), 0, PIXELS(296), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x68, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(216), 0, PIXELS(440), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(152), 0, PIXELS(296), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x6a, 0x855, (s32)gVault_Actor2,
        PIXELS(168), 0, PIXELS(312), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0xf8, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0x5a, 0x34, ACTOR_STAND,
        PIXELS(344), 0, PIXELS(96), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gVault_Events[] = {
    { EVENT_TOUCH, 20, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_SayFarewellToIvan) },
    { EVENT_TOUCH, 21, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_SayFarewellToIvan) },
    { EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_EnableAlternatePassageCells) },
    { EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_RestorePassageCells) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_EnterSoutheastHouse) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_EnterCentralEastHouse) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_EnterCentralHouse) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_EnterNortheastHouse) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 9, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_EnterNorthHouse) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_EnterNorthwestHouse) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 11, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_OpenDoorwayAndEnter) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 12, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_EnterWestHouse) },
    { EVENT_TOUCH, 13, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_EnterPassage) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgVaultWelcomeToVaultTheTownAt) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_DiscussEruption) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgVaultMasterHammetAWealthyLandownerAnd) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_DiscussEruptionThefts) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgVaultMasterHammetsServantCantReturnTo) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_AskAboutDepartedTravelers) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_DiscussIvanSearchingForRod) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_DiscussHammetsCaravan) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_DiscussWatchingForThieves) },
    { EVENT_TALK, 17, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_DiscussTheftAccusation) },
    { EVENT_TALK, 18, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_AskAboutAdventure) },
    { EVENT_TALK, 19, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_TalkToDog) },
    { EVENT_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgVaultAFieryRockBurntAHole) },
    { EVENT_TALK, 21, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_TalkAboutMissingRoofRepairer) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgVaultISupposeTheseChildrenCantGo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgVaultMtAlephsEruptionWasBeyondDescription) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgVaultWhyDoesAWealthyManLike) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_ReadThoughtsOnTheft) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgVaultPoorIvanButTheRodMay) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_ReadThoughtsOnTravelers) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_ReadThoughtsOnIvan) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_ReadThoughtsOnWealth) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_ReadThoughtsOnSuspects) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgVaultHmphItLooksLikeTheyreOnto) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgVaultIWasJustAboutToLeave) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_ReadDogThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgVaultFixTheRoofWhyPayFor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_ReadThoughtsOnRoofRepairer) },
    { SEARCH(SEARCH_UNNAMED), 30, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_RingBell) },
    { SEARCH(SEARCH_UNNAMED), 90, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_PeerIntoWell) },
    { SEARCH(SEARCH_WOODEN_BOX), 101, 0xf4a, FIND_ITEM(ITEM_NUT) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gVault_EventsAfterThieves[] = {
    { EVENT_TOUCH, 20, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_SayFarewellToIvan) },
    { EVENT_TOUCH, 21, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_SayFarewellToIvan) },
    { EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_EnableAlternatePassageCells) },
    { EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_RestorePassageCells) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_EnterSoutheastHouse) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_EnterCentralEastHouse) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_EnterCentralHouse) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_EnterNortheastHouse) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 9, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_EnterNorthHouse) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_EnterNorthwestHouse) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 11, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_OpenDoorwayAndEnter) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 12, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_EnterWestHouse) },
    { EVENT_TOUCH, 13, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_EnterPassage) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_AskAboutLeavingAgain) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgVaultSmokeIsStillRisingFromMt) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgVaultMasterHammetTheWealthyMerchantI) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_PraiseThievesCapture) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_TalkAboutIvan) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_DiscussDamagedRoads) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_DiscussLunpa) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_ThankPartyForCapturingThieves) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_DiscussCapturedPrisoners) },
    { EVENT_TALK, 18, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_AskAboutCapturingThieves) },
    { EVENT_TALK, 19, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_TalkToDogAfterCapture) },
    { EVENT_TALK, 20, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_ThankPartyBeforeDeparture) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgVaultYouveMadeOurTownPeacefulAt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgVaultNowMtAlephHasCalmedDown) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgVaultOfAllThePlacesWhyWould) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_ReadThoughtsOnTheft) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgVaultNobodyGetsIntoLunpaExceptFor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_ReadThoughtsOnTravelers) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_ReadThoughtsOnIvan) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_ReadThoughtsOnWealth) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_ReadThoughtsOnSuspects) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgVaultThereIsAlwaysAFightWhen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_ReadDogThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgVaultSniffAhTheSmellOfCooking) },
    { SEARCH(SEARCH_UNNAMED), 30, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_RingBell) },
    { SEARCH(SEARCH_UNNAMED), 90, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_PeerIntoWell) },
    { 0xe714, 19, 0x858, EVENT_SCRIPT(Vault_GiveBoneToDog) },
    { SEARCH(SEARCH_WOODEN_BOX), 101, 0xf4a, FIND_ITEM(ITEM_NUT) },
    { SCENE_EVENTS_END },
};
// clang-format on

u16 gVault_Scene5Cells[] = {
    MAP_CELL_STEP(35, 52, 2, 2, 4),
    MAP_CELL_STEP(33, 52, 2, 2, 4),
    MAP_CELL_STEPS_END,
};

u16 gVault_Scene6Cells[] = {
    MAP_CELL_STEP(35, 50, 2, 2, 4),
    MAP_CELL_STEP(33, 50, 2, 2, 4),
    MAP_CELL_STEPS_END,
};

u16 gVault_Scene7Cells[] = {
    MAP_CELL_STEP(35, 48, 2, 2, 4),
    MAP_CELL_STEP(33, 48, 2, 2, 4),
    MAP_CELL_STEPS_END,
};

u16 gVault_Scene8Cells[] = {
    MAP_CELL_STEP(42, 48, 2, 2, 4),
    MAP_CELL_STEP(40, 48, 2, 2, 4),
    MAP_CELL_STEPS_END,
};

u16 gVault_Scene9Cells[] = {
    MAP_CELL_STEP(42, 46, 2, 2, 4),
    MAP_CELL_STEP(40, 46, 2, 2, 4),
    MAP_CELL_STEPS_END,
};

u16 gVault_Scene10Cells[] = {
    MAP_CELL_STEP(35, 46, 2, 2, 4),
    MAP_CELL_STEP(33, 46, 2, 2, 4),
    MAP_CELL_STEPS_END,
};

u16 gVault_Scene12Cells[] = {
    MAP_CELL_STEP(35, 33, 2, 2, 4),
    MAP_CELL_STEP(33, 33, 2, 2, 4),
    MAP_CELL_STEPS_END,
};

s32 gVault_DriftScriptA[] = {
    SC_WAIT(1),
    SC_LOOP(118, SC_TOP),
    SC_FREE,
};

s32 gVault_DriftScriptB[] = {
    SC_WAIT(1),
    SC_LOOP(38, SC_TOP),
    SC_FREE,
};
