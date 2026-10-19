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
#include "flags.h"
#include "character_ids.h"
#include "songs.h"

enum { VAULT_DRIFT_RATE = PIXELS(1) / 10 };

/// Vault after the eruption, while Hammet's Shaman's Rod is missing and after
/// the party has jailed the thieves. The townspeople are named by what they
/// say.
enum {
  ACTOR_GREETER = 8, ///< greets the party. "Welcome to Vault"
  /// Asks about the eruption. "Wasn't the eruption of Mt. Aleph incredible?"
  ACTOR_ERUPTION_WITNESS = 9,
  ACTOR_THEFT_GOSSIP = 11,  ///< folks got robbed during the eruption
  ACTOR_HAMMET_GOSSIP = 10, ///< Master Hammet had his precious rod stolen
  ACTOR_IVAN_GOSSIP = 12,   ///< Hammet's servant can't return to Kalay
  /// Speaks of travelers. "Those travelers left in a big hurry."
  ACTOR_TRAVELER_WATCHER = 13,
  /// The two who walk in circles.
  ACTOR_FIRST_WALKER = 14,
  ACTOR_SECOND_WALKER = 15,
  /// Keeps watch. "Nobody's stealing anything on my watch!"
  ACTOR_WATCHMAN = 16,
  /// A thief. "Are you accusing us of stealing...?"
  ACTOR_LOITERING_THIEF = 17,
  ACTOR_STRANDED_TRAVELER = 18, ///< was leaving town when the volcano erupted
  ACTOR_VAULT_DOG = 19,
  ACTOR_ROOFER = 20, ///< a fiery rock burnt a hole through the roof
  /// Complains of the roofer. "He should be fixing the roof now"
  ACTOR_ROOF_COMPLAINER = 21,
  ACTOR_VAULT_VENUS_DJINNI = 23,
  VAULT_VENUS_DJINNI_INDEX = 4
};

// Forward declarations.
static void Vault_ShowStandingActorMessage(s32 actor);
static void Vault_ShowMessageAndWait(s32 actor, s32 delay);
static void Vault_FaceActorAndWait(s32 actor, s32 cue, s32 delay);

// Shared source is included here in ROM order.
#include "common/effect_create.h"
#include "common/effect_spawn.h"

// Shared source is included here in ROM order.
#include "common/vault_nearby.h"

// The roof complainer walks about looking for the roofer and stops to face
// the leader when near, or while a conversation holds the field.
// UB: calling this s32 function through a void update hook is incompatible.
// Plain void changes 108 to 104 bytes in tbs-en.
#ifdef UBFIX
static void
Vault_UpdateRoofComplainerProximity(union FieldObject *fieldObject) {
#else
static s32 Vault_UpdateRoofComplainerProximity(union FieldObject *fieldObject) {
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
  Vault_StopIfFacingNear(entity, leader, selector, flag);
#ifdef UBFIX
  return;
#else
  return 0;
#endif
}

/// Return the town's entrance positions.
struct SceneEntrance *Scene_GetEntrances(void) { return gVault_Entrances; }

/// The town has no regions.
const struct SceneRegion *Scene_GetRegions(void) { return NULL; }

/// Return the town's exits.
u32 *Scene_GetExits(void) { return gVault_Exits; }

/// Select placements before or after the thieves are captured.
struct ScenePlacement *Scene_GetPlacements(void) {
  if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) != 0)
    return gVault_PlacementsAfterThieves;
  return gVault_Placements;
}

/// Count the Venus Djinn that must leave Ivan before he leaves the party.
static int Vault_GetIvanVenusDjinnCount(void) {
  return UnitGet(IVAN)->djinnOwnedCounts[ELEMENT_VENUS];
}

// Shared source is included here in ROM order.
#include "common/vault_djinn.h"

/// Exits 1 and 2 (triggers 20 and 21): while Ivan is helping search for the
/// rod and the thieves are free, he walks out to the gate, says he is still
/// worried about the rod and will wait in Vault, leaves any Venus Djinni with
/// Isaac and leaves the party.
static void Event_IvanStaysBehind(void) {
  struct FieldActor *actor;
  struct EventWork *eventWork;

  eventWork = gEventWork;
  if (GameFlag_IsSet(FLAG_VAULT_THIEVES_JAILED) != 0 ||
      GameFlag_IsSet(FLAG_VAULT_IVAN_JOINED_ROD_SEARCH) == 0) {
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
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  // Return any Djinn before removing Ivan from the party.
  if (Vault_GetIvanVenusDjinnCount() != 0) {
    Engine_EventSetMessage(MsgVaultWaitDontWantTakeYour);
    Event_ShowMessage(IVAN, 0);
    Vault_ReturnIsaacsDjinni();
    Engine_TaskWait(20);
  }
  Party_RemoveOwnerRestored(IVAN);
  Engine_EventRequestExit(eventWork->touchedTrigger - 19);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_EventEnd();
}

/// Isaac peers into the well and hears water rumbling down there.
static void Event_PeerIntoWell(void) {
  Engine_EventBegin();
  Engine_MessageShowCentered(MsgFieldPeeredWell, 1);
  Engine_MessageShowCentered(MsgVaultCanHearWaterRumblingDown, 1);
  Engine_EventEnd();
}

/// Select interactions before or after the thieves are captured.
struct SceneEvent *Scene_GetEvents(void) {
  if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) != 0)
    return gVault_EventsAfterThieves;
  return gVault_Events;
}

/// Ask the current question and select its ordinary or alternate reply.
static inline void Vault_AskYesNoAndShowReply(s32 speaker) {
  Engine_EventOpenMessage(speaker, 0);
  if (Engine_EventChooseYesNo(0, 0) != 0) {
    struct EventWork *eventWork = gEventWork;

    ++eventWork->message;
  }
  Engine_EventShowMessage(speaker, 0);
}

/// Asks about the eruption. "Wasn't the eruption of Mt. Aleph incredible?"
/// Yes: too terrified to sleep for days. No: aren't you courageous!
static void Event_AskAboutEruption(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultWasntEruptionMtAlephIncredible);
  Vault_FaceActorAndWait(ACTOR_ERUPTION_WITNESS, 0, 2);
  Event_OpenMessage(ACTOR_ERUPTION_WITNESS, 0);
  if (Engine_EventChooseYesNo(0, 0) != 0) {
    gEventWork->message++;
  }
  Event_ShowMessage(ACTOR_ERUPTION_WITNESS, 0);
  Actor_FaceDirection(ACTOR_ERUPTION_WITNESS, FACING_SOUTH + FACING_STEP, 0);
  Engine_EventEnd();
}

/// Travelers left town in a big hurry: did the party see them? Yes: two women
/// and four men, the girl and old man unhappy to be there. No: maybe they left
/// earlier.
static void Event_AskAboutFleeingTravelers(void) {

  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultThoseTravelersLeftInBig);
  Engine_ActorSetAnimation(ACTOR_TRAVELER_WATCHER, ANIM_STAND);
  Vault_FaceActorAndWait(ACTOR_TRAVELER_WATCHER, 0, 2);
  Vault_AskYesNoAndShowReply(ACTOR_TRAVELER_WATCHER);
  Engine_EventEnd();
}

/// One of the thieves asks whether the party accuses them; either answer gets a
/// denial. "Are you accusing us of stealing Hammet's treasured Shaman's Rod?"
static void Event_ThiefDeniesTheft(void) {

  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultAccusingUsStealingHammetsTreasured);
  Vault_FaceActorAndWait(ACTOR_LOITERING_THIEF, 0, 2);
  Vault_AskYesNoAndShowReply(ACTOR_LOITERING_THIEF);
  Engine_EventEnd();
}

/// Asks whether the party is off on an adventure. "Off on an adventure, are
/// you?" Yes: impressive for youngsters. No: see the Sanctum while you're here.
static void Event_AskAboutAdventure(void) {

  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultOffOnAdventure);
  Vault_FaceActorAndWait(ACTOR_STRANDED_TRAVELER, 0, 2);
  Vault_AskYesNoAndShowReply(ACTOR_STRANDED_TRAVELER);
  Engine_EventEnd();
}

/// Some folks got robbed during the chaos of the eruption.
static void Event_TalkOfEruptionThefts(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultGuessFolksGot);
  Vault_ShowStandingActorMessage(ACTOR_THEFT_GOSSIP);
  Engine_EventEnd();
}

/// The watchman: nobody steals on his watch, the thieves must be from out of
/// town.
static void Event_WatchmanSuspectsOutsiders(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultNobodysStealingAnything);
  Vault_ShowStandingActorMessage(ACTOR_WATCHMAN);
  Engine_EventEnd();
}

/// The dog stops and barks.
static void Event_TalkToDog(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultRuffRrruff);
  Engine_ActorSetAnimation(ACTOR_VAULT_DOG, 0);
  Vault_FaceActorAndWait(ACTOR_VAULT_DOG, 0, 2);
  Engine_EventShowMessage(ACTOR_VAULT_DOG, 0);
  Engine_EventEnd();
}

/// The first walker stops: poor Ivan was left at the mayor's to look for the
/// stolen rod.
static void Event_PityIvan(void) {
  struct FieldActor *actor = Object_GetById(ACTOR_FIRST_WALKER);
  s16 savedFacing = actor->facing;

  actor->timer |= 2;
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultPoorGuyLeft);
  Engine_ActorSetAnimation(ACTOR_FIRST_WALKER, 0);
  Vault_FaceActorAndWait(ACTOR_FIRST_WALKER, 0, 2);
  Vault_ShowMessageAndWait(ACTOR_FIRST_WALKER, 10);
  actor->facing = savedFacing;
  Engine_TaskWait(1);
  Engine_EventEnd();
  actor->timer &= 1;
}

/// The second walker stops: Hammet's caravan left as soon as the eruption
/// stopped.
static void Event_TalkOfHammetsCaravan(void) {
  struct FieldActor *actor = Object_GetById(ACTOR_SECOND_WALKER);
  s16 savedFacing = actor->facing;

  actor->timer |= 2;
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultMasterHammetsCaravan);
  Engine_ActorSetAnimation(ACTOR_SECOND_WALKER, 0);
  Vault_FaceActorAndWait(ACTOR_SECOND_WALKER, 0, 2);
  Vault_ShowMessageAndWait(ACTOR_SECOND_WALKER, 10);
  actor->facing = savedFacing;
  Engine_TaskWait(1);
  Engine_EventEnd();
  actor->timer &= 1;
}

/// The roofer should be fixing the roof, but is probably in back slacking off.
static void Event_LookForRoofer(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultFixingRoofCant);
  Vault_FaceActorAndWait(ACTOR_ROOF_COMPLAINER, 0, 2);
  Engine_ActorShowEmote(ACTOR_ROOF_COMPLAINER, 0x103, 0);
  Engine_EventWait(30);
  Engine_EventOpenMessage(ACTOR_ROOF_COMPLAINER, 0);
  Engine_EventEnd();
}

/// Stop an actor, face the leader and show the current message.
static void Vault_ShowStandingActorMessage(s32 actor) {
  Engine_EventBegin();
  Engine_ActorSetAnimation(actor, ANIM_STAND);
  Vault_FaceActorAndWait(actor, 0, 2);
  Engine_EventShowMessage(actor, 0);
  Engine_EventEnd();
}

// Shared source is included here in ROM order.
#include "common/vault_talk.h"

/// After the thieves are jailed, asks whether the party is leaving again.
/// "So, you're hitting the road again?"
static void Event_AskAboutLeavingAgain(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultYoureHittingRoadAgain);
  Vault_FaceActorAndWait(ACTOR_GREETER, 0, 2);
  Vault_AskYesNoAndShowReply(ACTOR_GREETER);
  Engine_EventEnd();
}

/// Praises the thieves' capture. "It's great that you caught those thieves!"
static void Event_PraiseThievesCapture(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultGreatCaughtThieves);
  Vault_ShowStandingActorMessage(ACTOR_THEFT_GOSSIP);
  Engine_EventEnd();
}

/// Ivan went out to Lunpa, or, with Ivan in the party, rescuing Hammet must
/// have been impossible.
static void Event_TalkAboutIvan(void) {
  struct EventWork *workspace;
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultTalkingAboutHammetsServantIvan);
  // Flag 2 is set while Ivan is in the party.
  if (Engine_GameFlagIsSet(FLAG_IN_PARTY(IVAN)) != 0) {
    workspace = gEventWork;
    ++workspace->message;
  }
  Vault_ShowStandingActorMessage(ACTOR_IVAN_GOSSIP);
  Engine_EventEnd();
}

/// The eruption devastated the roads; once the southern bridge is repaired,
/// you can head toward Kalay.
static void Event_TalkOfDamagedRoads(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultEruptionDevastatedRoads);
  Vault_ShowStandingActorMessage(ACTOR_TRAVELER_WATCHER);
  Engine_EventEnd();
}

/// The first walker hopes Ivan didn't go to Lunpa alone, or, with Ivan in the
/// party, asks when he joined up with Isaac.
static void Event_AskWhetherIvanWentToLunpa(void) {
  struct FieldActor *actor = Object_GetById(ACTOR_FIRST_WALKER);
  s16 savedFacing = actor->facing;

  actor->timer |= 2;
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultDidntGoLunpa);
  if (Engine_GameFlagIsSet(FLAG_IN_PARTY(IVAN)) != 0)
    ++gEventWork->message;
  Engine_ActorSetAnimation(ACTOR_FIRST_WALKER, 0);
  Vault_FaceActorAndWait(ACTOR_FIRST_WALKER, 0, 2);
  Vault_ShowMessageAndWait(ACTOR_FIRST_WALKER, 10);
  actor->facing = savedFacing;
  Engine_TaskWait(1);
  Engine_EventEnd();
  actor->timer &= 1;
}

/// The second walker: everyone is grateful the party captured the thieves.
static void Event_ThankForCapturingThieves(void) {
  struct FieldActor *actor = Object_GetById(ACTOR_SECOND_WALKER);
  s16 savedFacing = actor->facing;

  actor->timer |= 2;
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultEveryoneGratefulCaptured);
  Engine_ActorSetAnimation(ACTOR_SECOND_WALKER, 0);
  Vault_FaceActorAndWait(ACTOR_SECOND_WALKER, 0, 2);
  Vault_ShowMessageAndWait(ACTOR_SECOND_WALKER, 10);
  actor->facing = savedFacing;
  Engine_TaskWait(1);
  Engine_EventEnd();
  actor->timer &= 1;
}

/// Asks about the prisoners. "Those men you captured... They're in jail now?"
/// Yes: the prison hadn't been used in a long time. No: they're upset; maybe go
/// see them.
static void Event_AskAboutPrisoners(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultThoseMenCapturedTheyreIn);
  Engine_ActorSetAnimation(ACTOR_WATCHMAN, ANIM_STAND);
  Vault_FaceActorAndWait(ACTOR_WATCHMAN, 0, 2);
  Vault_AskYesNoAndShowReply(ACTOR_WATCHMAN);
  Engine_EventEnd();
}

/// Asks who captured the thieves; either answer is praised.
/// "Are you the ones who captured the thieves?"
static void Event_AskWhoCapturedThieves(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultOnesWhoCapturedThieves);
  Vault_FaceActorAndWait(ACTOR_STRANDED_TRAVELER, 0, 2);
  Vault_AskYesNoAndShowReply(ACTOR_STRANDED_TRAVELER);
  Engine_EventEnd();
}

/// The dog barks; carrying a Bone before it has shown the passage lets the
/// party give it one.
static void Event_TalkToDogAfterCapture(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultRuffRrruff2);
  Engine_ActorSetAnimation(ACTOR_VAULT_DOG, 0);
  Vault_FaceActorAndWait(ACTOR_VAULT_DOG, 0, 2);
  Engine_EventShowMessage(ACTOR_VAULT_DOG, 0);
  Engine_ActorSetAnimation(ACTOR_VAULT_DOG, ANIM_STAND);
  if (PartyInventory_FindOwner(ITEM_BONE) != -1 &&
      Engine_GameFlagIsSet(FLAG_VAULT_DOG_SHOWN_PASSAGE) == 0) {
    u16 *p = &gEventWork->unk172;
    u16 value = 1;

    *p = value;
  }
  Engine_EventEnd();
}

/// Thanks the party before it leaves.
/// "Thank you for the other day. Are you leaving town already?"
static void Event_ThankBeforeDeparture(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultThankForOtherDayLeaving);
  Vault_FaceActorAndWait(ACTOR_ROOFER, 0, 2);
  Engine_ActorSetAnimationAndWait(ACTOR_ROOFER, ANIM_NOD);
  Engine_EventWait(20);
  Engine_EventShowMessage(ACTOR_ROOFER, 0);
  Engine_EventEnd();
}

/// Mind Read: stealing during an eruption is unforgivable, or how should the
/// prisoners be punished?
static void Event_ReadMindOnTheft(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) == 0)
    Engine_EventSetMessage(MsgVaultManShouldStealFromAnother);
  else
    Engine_EventSetMessage(MsgVaultHowPunishPrisoners);
  Vault_ShowThought(ACTOR_THEFT_GOSSIP);
  Engine_EventEnd();
}

/// Mind Read: the strange travelers headed for the northeast of the Goma
/// Range, or the only way out is the cave in the Goma Range.
static void Event_ReadMindOnTravelers(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) == 0) {
    Engine_EventSetMessage(MsgVaultGroupTravelersWasStrangeBunch);
  } else {
    Engine_EventSetMessage(MsgVaultWithRoadOutOnlyWay);
  }
  Vault_ShowThought(ACTOR_TRAVELER_WATCHER);
  Engine_EventEnd();
}

/// Mind Read on the first walker: the three creeps at the inn stole the rod,
/// or where did Ivan go (when did they join him, with Ivan in the party)?
static void Event_ReadMindOnIvan(void) {
  Object_GetById(ACTOR_FIRST_WALKER)->timer |= 2;
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) == 0) {
    Engine_EventSetMessage(MsgVaultBetWasThoseThreeCreeps);
  } else {
    Engine_EventSetMessage(MsgVaultWhereDidIvanGoBy);
    if (Engine_GameFlagIsSet(FLAG_IN_PARTY(IVAN)) != 0)
      ++gEventWork->message;
  }
  Vault_ShowThought(ACTOR_FIRST_WALKER);
  Engine_EventEnd();
  Object_GetById(ACTOR_FIRST_WALKER)->timer &= 1;
}

/// Mind Read on the second walker: riches can't save you from a volcano, or the
/// party must be stronger than they look.
static void Event_ReadMindOnWealth(void) {
  Object_GetById(ACTOR_SECOND_WALKER)->timer |= 2;
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) == 0)
    Engine_EventSetMessage(MsgVaultSupposeDoesntMatterHowRich);
  else
    Engine_EventSetMessage(MsgVaultMustStrongerThanLookHave);
  Vault_ShowThought(ACTOR_SECOND_WALKER);
  Engine_EventEnd();
  Object_GetById(ACTOR_SECOND_WALKER)->timer &= 1;
}

/// Mind Read on the watchman: everyone knows the three at the inn stole the
/// rod, or he feels much better with them behind bars.
static void Event_ReadMindOnSuspects(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) == 0)
    Engine_EventSetMessage(MsgVaultEveryoneKnowsThoseThreeAt);
  else
    Engine_EventSetMessage(MsgVaultFeelMuchBetter);
  Vault_ShowThought(ACTOR_WATCHMAN);
  Engine_EventEnd();
}

/// Mind Read on the dog: humans value money over their lives; after the
/// thieves are jailed, bring a bone for something useful, and once fed, a
/// careful search will reveal a passage.
static void Event_ReadDogMind(void) {
  struct FieldActor *actor = Object_GetById(ACTOR_VAULT_DOG);
  actor->paused = 1;
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) == 0) {
    Engine_EventSetMessage(MsgVaultStealingInMidstVolcanicEruption);
    Engine_ActorSetAnimation(ACTOR_VAULT_DOG, 0);
    Engine_EventWait(2);
  } else if (Engine_GameFlagIsSet(FLAG_VAULT_DOG_SHOWN_PASSAGE) != 0) {
    Engine_EventSetMessage(MsgVaultCarefulSearchWillRevealPassage);
  } else {
    Engine_EventSetMessage(MsgVaultIfBringMeBoneIll);
  }
  Engine_EventShowMessage(ACTOR_VAULT_DOG, 0);
  Engine_EventEnd();
  actor->paused = 0;
}

/// Mind Read: that sneak is wasting time instead of fixing the roof.
static void Event_ReadMindOnRoofer(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultSneakWastingTime);
  Vault_ShowThought(ACTOR_ROOF_COMPLAINER);
  Engine_EventEnd();
}

// Shared source is included here in ROM order.
#include "common/vault_toggle.h"

/// Walk to the exit threshold and request the room's transition.
static void Vault_WalkThroughExit(s32 x, s32 y, s32 continuation) {
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
  Actor_WalkTo(ACTOR_PARTY_LEADER, x, y);
  gEventWork->transitionFrames = 16;
  Engine_EventRequestExit(continuation);
}

/// Open and enter the southeastern house.
static void Event_EnterSoutheastHouse(void) {
  Engine_AudioPlayCue(SE_DOOR_OPEN);
  Engine_MapAnimateCells(gVault_Scene5Cells, 56, 19);
  Vault_WalkThroughExit(408, 320, 5);
}

/// Open and enter the house east of the town's center.
static void Event_EnterCentralEastHouse(void) {
  Engine_AudioPlayCue(SE_DOOR_OPEN);
  Engine_MapAnimateCells(gVault_Scene6Cells, 50, 18);
  Vault_WalkThroughExit(312, 304, 6);
}

/// Open and enter the central house.
static void Event_EnterCentralHouse(void) {
  Engine_AudioPlayCue(SE_DOOR_OPEN);
  Engine_MapAnimateCells(gVault_Scene7Cells, 44, 17);
  Vault_WalkThroughExit(216, 288, 7);
}

/// Enter the northeastern doorway behind its foreground tiles.
static void Event_EnterNortheastHouse(void) {
  struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);
  struct AnimationObject *sprite = actor->sprite;

  Engine_AudioPlayCue(SE_DOOR_OPEN);
  Engine_MapAnimateCells(gVault_Scene8Cells, 54, 13);
  Map_CopyCellAttributes(33, 20, 1, 3, 23, 12);
  actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  sprite->image.priority = 3;
  Vault_WalkThroughExit(376, 224, 8);
}

/// Enter the northern doorway behind its foreground tiles.
static void Event_EnterNorthHouse(void) {
  struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);
  struct AnimationObject *sprite = actor->sprite;

  Engine_AudioPlayCue(SE_DOOR_OPEN);
  Engine_MapAnimateCells(gVault_Scene9Cells, 49, 10);
  Map_CopyCellAttributes(33, 20, 1, 3, 18, 10);
  actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  sprite->image.priority = 3;
  Vault_WalkThroughExit(296, 176, 9);
}

/// Open and enter the northwestern house.
static void Event_EnterNorthwestHouse(void) {
  Engine_AudioPlayCue(SE_DOOR_OPEN);
  Engine_MapAnimateCells(gVault_Scene10Cells, 38, 6);
  Vault_WalkThroughExit(120, 144, 10);
}

// Shared source is included here in ROM order.
#include "common/vault_scene_cells.h"

/// The jail door opens and the party walks in, drawn behind the doorway.
static void Event_EnterJail(void) {
  struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);
  struct AnimationObject *sprite = actor->sprite;

  Engine_AudioPlayCue(SE_DOOR_OPEN);
  Engine_MapAnimateCells(gVault_Scene12Cells, 35, 9);
  Map_CopyCellAttributes(33, 20, 1, 3, 4, 10);
  actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  sprite->image.priority = 3;
  Vault_WalkThroughExit(72, 160, 12);
}

/// The party walks through the doorless entrance into the interiors (exit 13).
static void Event_EnterPassage(void) {
  Engine_AudioPlayCue(SE_MAP_EXIT);
  Vault_WalkThroughExit(152, 264, 13);
}

/// Move an actor through a jump and wait up to sixty frames for it to settle.
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

/// Isaac rings the bell; the first time, the Venus Djinni hiding nearby jumps
/// out and comes to the bell.
static void Event_RingBell(void) {
  Engine_EventBegin();
  Event_PlayCue(SE_100);
  Engine_EventWait(40);
  if (GameFlag_IsSet(FLAG_VAULT_DJINNI_CALLED_BY_BELL) == 0) {
    Actor_SetAttachedEffect(ACTOR_VAULT_VENUS_DJINNI, 0x102);
    Engine_ActorJump(ACTOR_VAULT_VENUS_DJINNI, 4, 0);
    Engine_EventWait(12);
    Engine_ActorJump(ACTOR_VAULT_VENUS_DJINNI, 4, 0);
    Engine_EventWait(20);
    Vault_JumpActorToAndWait(ACTOR_VAULT_VENUS_DJINNI, 392, 104, PIXELS(7));
    Engine_EventWait(20);
    Actor_WalkToAndWait(ACTOR_VAULT_VENUS_DJINNI, 408, 104);
    Actor_WalkToAndWait(ACTOR_VAULT_VENUS_DJINNI, 408, 120);
    GameFlag_Set(FLAG_VAULT_DJINNI_CALLED_BY_BELL);
  }
  Engine_EventEnd();
}

/// Given a Bone, the dog runs to the hidden passage and jumps there three
/// times.
static void Event_GiveBoneToDog(void) {
  PartyInventory_Discard(ITEM_BONE);
  Engine_EventBegin();
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_VAULT_DOG, 2);
  Actor_SetSpeed(ACTOR_VAULT_DOG, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_VAULT_DOG, 216, 408);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_VAULT_DOG, FACING_SOUTH, 20);
  Engine_ActorJump(ACTOR_VAULT_DOG, 6, 0);
  Engine_EventWait(30);
  Engine_ActorJump(ACTOR_VAULT_DOG, 6, 0);
  Engine_EventWait(30);
  Engine_ActorJump(ACTOR_VAULT_DOG, 6, 0);
  Engine_EventWait(30);
  Actor_WalkToAndWait(ACTOR_VAULT_DOG, 216, 392);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_VAULT_DOG, FACING_SOUTH, 20);
  GameFlag_Set(FLAG_VAULT_DOG_SHOWN_PASSAGE);
  Engine_EventEnd();
}

/// On arrival: restores the dog's and the Djinni's places, sets the walkers
/// and the roof complainer stopping near the leader, and, while Ivan waits in
/// Vault for the party, has him greet the two and rejoin.
s32 Scene_Initialize(void) {
  s32 suspectsMindRead;
  struct FieldActor *actor;
  s32 lowBit;
  s32 zero;

  // Later in the story (flag 0x87a, set in Kalay and the Lamakan Desert) the
  // town is replaced by its revisit scene.
  if (Engine_GameFlagIsSet(FLAG_REACHED_KALAY) != 0) {
    Engine_EventRequestExit(14);
  }
  // Restore the passage cells and the two walkers' proximity callbacks.
  if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_00) != 0) {
    Map_CopyCellAttributes(55, 26, 4, 2, 23, 26);
  }
  OverlayObject_CreateConfiguredObject(PIXELS(128), 0, PIXELS(420), 223);
  lowBit = 1;
  zero = 0;
  actor = Object_GetById(ACTOR_FIRST_WALKER);
  actor->timer = lowBit;
#ifdef UBFIX
  actor->update = Vault_UpdateWalkingVillager;
#else
  actor->update = (void (*)(union FieldObject *))Vault_UpdateWalkingVillager;
#endif
  actor = Object_GetById(ACTOR_SECOND_WALKER);
  actor->timer = zero;
#ifdef UBFIX
  actor->update = Vault_UpdateWalkingVillager;
#else
  actor->update = (void (*)(union FieldObject *))Vault_UpdateWalkingVillager;
#endif
  if (Engine_GameFlagIsSet(FLAG_VAULT_DOG_SHOWN_PASSAGE) != 0) {
    Actor_SetPosition(ACTOR_VAULT_DOG, PIXELS(216), PIXELS(392));
  }
  suspectsMindRead = Engine_GameFlagIsSet(FLAG_VAULT_SUSPECTS_MIND_READ);
  if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) == 0) {
    if ((lowBit & suspectsMindRead) != 0) {
      actor = Object_GetById(ACTOR_ROOF_COMPLAINER);
#ifdef UBFIX
      actor->update = Vault_UpdateRoofComplainerProximity;
#else
      actor->update =
          (void (*)(union FieldObject *))Vault_UpdateRoofComplainerProximity;
#endif
    }
  }
  // Back through the gates while Ivan waits in Vault: "Welcome back, you two!
  // Let's go find the rod!"
  if (gGameState.entrance <= 2) {
    if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
      Engine_GameFlagClear(FLAG_VAULT_DJINNI_CALLED_BY_BELL);
      if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) == 0) {
        if (Engine_GameFlagIsSet(FLAG_VAULT_IVAN_JOINED_ROD_SEARCH) != 0) {
          Engine_EventBegin();
          actor = Object_GetById(ACTOR_PARTY_LEADER);
          if (actor != 0) {
            Engine_ActorSetPosition(IVAN, actor->x.fixed, actor->z.fixed);
          }
          if (gGameState.entrance == 1) {
            Actor_SetPosition(IVAN, PIXELS(400), PIXELS(448));
          } else {
            Actor_SetPosition(IVAN, PIXELS(224), PIXELS(162));
          }
          Engine_ActorFaceEachOther(IVAN, ACTOR_PARTY_LEADER, 0);
          Engine_EventOpenScreen();
          Engine_EventWaitForScreen();
          Engine_EventWait(30);
          Engine_ActorRunRepeatedMotion(IVAN, 2);
          Engine_EventSetMessage(MsgVaultWelcomeBackTwo);
          Engine_EventShowMessageAndWait(IVAN, 0, 20);
          Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
          Actor_SetSpeed(IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
          Engine_ActorSetAnimation(IVAN, ANIM_WALK);
          actor = Object_GetById(ACTOR_PARTY_LEADER);
          if (actor != 0) {
            Engine_ActorSetDestination(IVAN, actor->x.part.pixel,
                                       actor->z.part.pixel);
          }
          Engine_ActorWaitForMove(IVAN);
          Engine_ActorSetPosition(IVAN, 0, 0);
          Event_PrepareObjectAndApplyValue(IVAN, 0);
          Engine_EventEnd();
        }
      }
    }
  }
  if (Engine_GameFlagIsSet(FLAG_VAULT_DJINNI_CALLED_BY_BELL) != 0) {
    Actor_SetPosition(ACTOR_VAULT_VENUS_DJINNI, PIXELS(408), PIXELS(120));
  }
  // The same welcome at the western entrance (entrance 11) while Ivan is out of
  // the party (flag 2 clear).
  if (gGameState.entrance == 11) {
    if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) == 0) {
      if (Engine_GameFlagIsSet(FLAG_VAULT_IVAN_JOINED_ROD_SEARCH) != 0) {
        if (Engine_GameFlagIsSet(FLAG_IN_PARTY(IVAN)) == 0) {
          Engine_EventBegin();
          Actor_SetPosition(IVAN, PIXELS(40), PIXELS(310));
          Engine_ActorFaceEachOther(IVAN, ACTOR_PARTY_LEADER, 0);
          Engine_EventOpenScreen();
          Engine_EventWaitForScreen();
          Engine_EventWait(30);
          Engine_ActorRunRepeatedMotion(IVAN, 2);
          Engine_EventSetMessage(MsgVaultWelcomeBackTwo);
          Engine_EventShowMessageAndWait(IVAN, 0, 20);
          Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
          Actor_SetSpeed(IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
          Engine_ActorSetAnimation(IVAN, ANIM_WALK);
          actor = Object_GetById(ACTOR_PARTY_LEADER);
          if (actor != 0) {
            Engine_ActorSetDestination(IVAN, actor->x.part.pixel,
                                       actor->z.part.pixel);
          }
          Engine_ActorWaitForMove(IVAN);
          Engine_ActorSetPosition(IVAN, 0, 0);
          Event_PrepareObjectAndApplyValue(IVAN, 0);
          Engine_EventEnd();
        }
      }
    }
    Engine_GameFlagClear(FLAG_SHOW_LOCATION_NAME);
  } else {
    if (gGameState.entrance == 13) {
      if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) != 0) {
        Engine_ActorSetPosition(ACTOR_ROOFER, 0, 0);
      }
    }
  }
  return 0;
}

// Shared source is included here in ROM order.
#include "common/object_distance.h"
#include "common/vault_range.h"

/// Show the current message and pause before continuing the scene.
static void Vault_ShowMessageAndWait(s32 actor, s32 delay) {
  Engine_EventShowMessage(actor, 0);
  Engine_EventWait(delay);
}

/// Face another actor and pause before continuing the conversation.
static void Vault_FaceActorAndWait(s32 actor, s32 cue, s32 delay) {
  Engine_ActorFaceEachOther(actor, cue, 0);
  Engine_EventWait(delay);
}

/// Apply drift and gradually damp both rates toward zero.
/// This custom update uses speed/acceleration as its two drift rates and
/// mirrors the resulting position into the actor's target coordinates.
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

/// Spawn a drifting sprite near the dog, sharing its drawing priority.
UNUSED static void Vault_SpawnDriftingEffect(s32 flags) {
  struct FieldActor *anchor;
  struct FieldActor *effect;
  struct AnimationObject *sprite;
  s32 x;
  s32 z;
  u8 zero;

  anchor = (struct FieldActor *)Object_GetById(ACTOR_VAULT_DOG);
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
    SCENE_EXITS(SCENE_VAULT),
    SCENE_EXIT(1, SCENE_WORLD_MAP, 3),
    SCENE_EXIT(2, SCENE_WORLD_MAP, 44),
    SCENE_EXIT(5, 21, 5),
    SCENE_EXIT(6, 21, 6),
    SCENE_EXIT(7, 21, 7),
    SCENE_EXIT(8, 21, 8),
    SCENE_EXIT(9, 21, 9),
    SCENE_EXIT(10, 21, 10),
    SCENE_EXIT(11, 9, 10),
    SCENE_EXIT(12, SCENE_VAULT_JAIL, 1),
    SCENE_EXIT(13, 21, 13),
    SCENE_EXIT(14, SCENE_VAULT_REVISIT, 1),
    SCENE_EXITS_END,
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
    { 0x40, FLAG_VAULT_THIEVES_CONFRONTED, ACTOR_STAND,
        PIXELS(92), 0, PIXELS(284), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x7a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(92), 0, PIXELS(296), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x68, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(216), 0, PIXELS(392), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x6c, FLAG_VAULT_SUSPECTS_MIND_READ, ACTOR_STAND,
        PIXELS(152), 0, PIXELS(296), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x6a, IF_FLAG_SET(FLAG_VAULT_SUSPECTS_MIND_READ), (s32)gVault_Actor2,
        PIXELS(168), 0, PIXELS(312), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0xf8, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0x5a, FLAG_DJINNI_0 + VAULT_VENUS_DJINNI_INDEX, ACTOR_STAND,
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
    { 0x16, FLAG_VAULT_THIEVES_CONFRONTED, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x7a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(92), 0, PIXELS(296), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x68, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(216), 0, PIXELS(440), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(152), 0, PIXELS(296), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x6a, FLAG_VAULT_THIEVES_JAILED, (s32)gVault_Actor2,
        PIXELS(168), 0, PIXELS(312), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0xf8, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0x5a, FLAG_DJINNI_0 + VAULT_VENUS_DJINNI_INDEX, ACTOR_STAND,
        PIXELS(344), 0, PIXELS(96), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gVault_Events[] = {
    { EVENT_TOUCH, 20, CONDITION_ALWAYS, EVENT_SCRIPT(Event_IvanStaysBehind) },
    { EVENT_TOUCH, 21, CONDITION_ALWAYS, EVENT_SCRIPT(Event_IvanStaysBehind) },
    { EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_EnableAlternatePassageCells) },
    { EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_RestorePassageCells) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterSoutheastHouse) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterCentralEastHouse) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterCentralHouse) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterNortheastHouse) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 9, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterNorthHouse) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterNorthwestHouse) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 11, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_OpenDoorwayAndEnter) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 12, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterJail) },
    { EVENT_TOUCH, 13, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterPassage) },
    { EVENT_TALK, ACTOR_GREETER, CONDITION_ALWAYS, MSGNO(MsgVaultWelcomeToVaultTheTownAt) },
    { EVENT_TALK, ACTOR_ERUPTION_WITNESS, CONDITION_ALWAYS, EVENT_SCRIPT(Event_AskAboutEruption) },
    { EVENT_TALK, ACTOR_HAMMET_GOSSIP, CONDITION_ALWAYS, MSGNO(MsgVaultMasterHammetAWealthyLandownerAnd) },
    { EVENT_TALK, ACTOR_THEFT_GOSSIP, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkOfEruptionThefts) },
    { EVENT_TALK, ACTOR_IVAN_GOSSIP, CONDITION_ALWAYS, MSGNO(MsgVaultMasterHammetsServantCantReturnTo) },
    { EVENT_TALK, ACTOR_TRAVELER_WATCHER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_AskAboutFleeingTravelers) },
    { EVENT_TALK, ACTOR_FIRST_WALKER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PityIvan) },
    { EVENT_TALK, ACTOR_SECOND_WALKER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkOfHammetsCaravan) },
    { EVENT_TALK, ACTOR_WATCHMAN, CONDITION_ALWAYS, EVENT_SCRIPT(Event_WatchmanSuspectsOutsiders) },
    { EVENT_TALK, ACTOR_LOITERING_THIEF, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ThiefDeniesTheft) },
    { EVENT_TALK, ACTOR_STRANDED_TRAVELER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_AskAboutAdventure) },
    { EVENT_TALK, ACTOR_VAULT_DOG, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToDog) },
    { EVENT_TALK, ACTOR_ROOFER, CONDITION_ALWAYS, MSGNO(MsgVaultAFieryRockBurntAHole) },
    { EVENT_TALK, ACTOR_ROOF_COMPLAINER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LookForRoofer) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GREETER, CONDITION_ALWAYS, MSGNO(MsgVaultISupposeTheseChildrenCantGo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ERUPTION_WITNESS, CONDITION_ALWAYS, MSGNO(MsgVaultMtAlephsEruptionWasBeyondDescription) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HAMMET_GOSSIP, CONDITION_ALWAYS, MSGNO(MsgVaultWhyDoesAWealthyManLike) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_THEFT_GOSSIP, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadMindOnTheft) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_IVAN_GOSSIP, CONDITION_ALWAYS, MSGNO(MsgVaultPoorIvanButTheRodMay) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TRAVELER_WATCHER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadMindOnTravelers) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_WALKER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadMindOnIvan) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_WALKER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadMindOnWealth) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WATCHMAN, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadMindOnSuspects) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LOITERING_THIEF, CONDITION_ALWAYS, MSGNO(MsgVaultHmphItLooksLikeTheyreOnto) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRANDED_TRAVELER, CONDITION_ALWAYS, MSGNO(MsgVaultIWasJustAboutToLeave) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VAULT_DOG, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadDogMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROOFER, CONDITION_ALWAYS, MSGNO(MsgVaultFixTheRoofWhyPayFor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROOF_COMPLAINER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadMindOnRoofer) },
    { SEARCH(SEARCH_UNNAMED), 30, CONDITION_ALWAYS, EVENT_SCRIPT(Event_RingBell) },
    { SEARCH(SEARCH_UNNAMED), 90, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PeerIntoWell) },
    { SEARCH(SEARCH_WOODEN_BOX), 101, FLAG_VAULT_BOX_NUT, FIND_ITEM(ITEM_NUT) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gVault_EventsAfterThieves[] = {
    { EVENT_TOUCH, 20, CONDITION_ALWAYS, EVENT_SCRIPT(Event_IvanStaysBehind) },
    { EVENT_TOUCH, 21, CONDITION_ALWAYS, EVENT_SCRIPT(Event_IvanStaysBehind) },
    { EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_EnableAlternatePassageCells) },
    { EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_RestorePassageCells) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterSoutheastHouse) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterCentralEastHouse) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterCentralHouse) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterNortheastHouse) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 9, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterNorthHouse) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterNorthwestHouse) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 11, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_OpenDoorwayAndEnter) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 12, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterJail) },
    { EVENT_TOUCH, 13, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterPassage) },
    { EVENT_TALK, ACTOR_GREETER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_AskAboutLeavingAgain) },
    { EVENT_TALK, ACTOR_ERUPTION_WITNESS, CONDITION_ALWAYS, MSGNO(MsgVaultSmokeIsStillRisingFromMt) },
    { EVENT_TALK, ACTOR_HAMMET_GOSSIP, CONDITION_ALWAYS, MSGNO(MsgVaultMasterHammetTheWealthyMerchantI) },
    { EVENT_TALK, ACTOR_THEFT_GOSSIP, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PraiseThievesCapture) },
    { EVENT_TALK, ACTOR_IVAN_GOSSIP, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkAboutIvan) },
    { EVENT_TALK, ACTOR_TRAVELER_WATCHER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkOfDamagedRoads) },
    { EVENT_TALK, ACTOR_FIRST_WALKER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_AskWhetherIvanWentToLunpa) },
    { EVENT_TALK, ACTOR_SECOND_WALKER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ThankForCapturingThieves) },
    { EVENT_TALK, ACTOR_WATCHMAN, CONDITION_ALWAYS, EVENT_SCRIPT(Event_AskAboutPrisoners) },
    { EVENT_TALK, ACTOR_STRANDED_TRAVELER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_AskWhoCapturedThieves) },
    { EVENT_TALK, ACTOR_VAULT_DOG, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToDogAfterCapture) },
    { EVENT_TALK, ACTOR_ROOFER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ThankBeforeDeparture) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GREETER, CONDITION_ALWAYS, MSGNO(MsgVaultYouveMadeOurTownPeacefulAt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ERUPTION_WITNESS, CONDITION_ALWAYS, MSGNO(MsgVaultNowMtAlephHasCalmedDown) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HAMMET_GOSSIP, CONDITION_ALWAYS, MSGNO(MsgVaultOfAllThePlacesWhyWould) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_THEFT_GOSSIP, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadMindOnTheft) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_IVAN_GOSSIP, CONDITION_ALWAYS, MSGNO(MsgVaultNobodyGetsIntoLunpaExceptFor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TRAVELER_WATCHER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadMindOnTravelers) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_WALKER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadMindOnIvan) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_WALKER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadMindOnWealth) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WATCHMAN, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadMindOnSuspects) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRANDED_TRAVELER, CONDITION_ALWAYS, MSGNO(MsgVaultThereIsAlwaysAFightWhen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VAULT_DOG, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadDogMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROOFER, CONDITION_ALWAYS, MSGNO(MsgVaultSniffAhTheSmellOfCooking) },
    { SEARCH(SEARCH_UNNAMED), 30, CONDITION_ALWAYS, EVENT_SCRIPT(Event_RingBell) },
    { SEARCH(SEARCH_UNNAMED), 90, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PeerIntoWell) },
    { USE_ITEM_ON_ACTOR(ITEM_BONE), ACTOR_VAULT_DOG, FLAG_VAULT_DOG_SHOWN_PASSAGE, EVENT_SCRIPT(Event_GiveBoneToDog) },
    { SEARCH(SEARCH_WOODEN_BOX), 101, FLAG_VAULT_BOX_NUT, FIND_ITEM(ITEM_NUT) },
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
