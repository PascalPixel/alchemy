/// @file
/// Vault (SceneId_Vault), "the town at the heart of Angara", as the party first
/// finds it after Mt. Aleph's eruption: its streets, with the townspeople,
/// Ivan, the bell and the dog. The houses, the inn and its loft, where the
/// search for Hammet's stolen Shaman's Rod plays out, are log_rooms_a.c.
/// Scene_GetActorTemplates and Scene_GetTriggers choose their tables by
/// FLAG_VAULT_THIEVES_JAILED, and once the party has reached Kalay
/// (FLAG_KALAY_REACHED) Scene_Initialize sends it straight on to the town's
/// later scene (VAULT_TO_VAULT_REVISIT, SceneId_VaultRevisit,
/// vault_revisit.c).
///
/// The story flags change the streets in order:
/// - While the rod is missing, the townspeople talk of the eruption, the
///   theft and the travelers who left in a hurry, and Mind Read on them blames
///   the three strangers staying at the inn. One of the three loiters in town
///   until the party has confronted them in the loft
///   (FLAG_VAULT_THIEVES_ATTACKED). Once the suspects' minds have been read
///   (FLAG_VAULT_SUSPECTS_MIND_READ) the young man fixing a roof is missing,
///   and a woman walks about looking for him.
/// - While Ivan is helping the search (FLAG_VAULT_IVAN_JOINED_ROD_SEARCH),
///   leaving by either gate has him stay behind (Vault_IvanStaysBehind),
///   and Scene_Initialize has him rejoin when the party comes back in by a gate
///   or out of the Vault Sanctum.
/// - Once the thieves are jailed, the townspeople thank the party, and the
///   dog, given a Bone (Vault_GiveBoneToDog,
///   FLAG_VAULT_DOG_SHOWN_PASSAGE), runs to a spot and jumps there three times;
///   the passage it shows opens only in the later scene.
///
/// Sap, a Venus Djinni, hides near the bell, and the first ring since the party
/// came in by a gate (FLAG_VAULT_SAP_CALLED_BY_BELL) calls it out to stand
/// beside the bell; it joins only in the later scene. The well rumbles with
/// water and a wooden box holds a Nut. The doors open with an animation
/// (gVault_*DoorCells) and lead into the houses, shops and inn
/// (SceneId_LogRoomsA), the jail (SceneId_StoneRooms) and the Vault Sanctum
/// (VAULT_TO_SANCTUM); the metatile behaviors the events answer are enum
/// VaultMetatileBehavior (vault.h).
///
/// The rope bridge in the town's south-east spans a path that runs under it,
/// and stepping onto either end of it or off its west end switches the cells
/// beneath it between the deck and the path (common/vault_toggle.h).

#include "save_block.h"
#include "animspr.h"
#include "window.h"
#include "unit/scene/vault.h"
#include "vault.h"
#include "unit/scene/common/vault_range.h"
#include "unit/scene/common/effect_spawn.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/event_commands.h"
#include "unit/battle/battle.h"
#include "unit/game/game.h"
#include "unit/runtime/runtime.h"
#include "actor.h"
#include "types.h"
#include "event_layer.h"
#include "scene_tables.h"
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
#include "ability_ids.h"
#include "item_ids.h"
#include "actor_script.h"
#include "fixed_math.h"
#include "flags.h"
#include "djinn_ids.h"
#include "character_ids.h"
#include "songs.h"

enum { VAULT_DRIFT_RATE = PIXELS(1) / 10 };

/// The townspeople, named by what they say.
enum {
  /// A young man.
  /// ★ Named from the game: "Welcome to Vault, the town at the heart of
  /// Angara." (MsgVaultWelcomeToVaultTheTownAt)
  ACTOR_WELCOMING_YOUNG_MAN = 8,
  /// A woman with fair hair who asks about the eruption.
  /// ★ Named from the game: "Wasn't the eruption of Mt. Aleph incredible?"
  /// (MsgVaultWasntEruptionMtAlephIncredible)
  ACTOR_ERUPTION_WITNESS = 9,
  /// A woman.
  /// ★ Named from the game: "No man should steal from another during the
  /// panic of an eruption." (MsgVaultManShouldStealFromAnother)
  ACTOR_WOMAN_ON_STEALING = 11,
  /// A burly grey-haired man.
  /// ★ Named from the game: "Master Hammet, a wealthy landowner and merchant
  /// from Kalay, had his precious rod stolen from him."
  /// (MsgVaultMasterHammetAWealthyLandownerAnd)
  ACTOR_VILLAGER_ON_HAMMET = 10,
  /// An old man with a cane.
  /// ★ Named from the game: "Master Hammet's servant can't return to Kalay
  /// until he recovers the rod." (MsgVaultMasterHammetsServantCantReturnTo)
  ACTOR_OLD_MAN_ON_HAMMETS_SERVANT = 12,
  /// An old woman.
  /// ★ Named from the game: "Those travelers left in a big hurry."
  /// (MsgVaultThoseTravelersLeftInBig)
  ACTOR_OLD_WOMAN_ON_THE_TRAVELERS = 13,
  /// The two children who walk round the well, half a circle apart
  /// (gVault_BoyRoundTheWellRoute, gVault_GirlRoundTheWellRoute).
  /// ★ Named from what the player sees: a blond boy walking round the well
  /// (its render)
  ACTOR_BOY_ROUND_THE_WELL = 14,
  /// ★ Named from what the player sees: a girl with orange hair walking round
  /// the well (its render)
  ACTOR_GIRL_ROUND_THE_WELL = 15,
  /// A stocky, muscular man.
  /// ★ Named from the game: "Nobody's stealing anything on my watch!"
  /// (MsgVaultNobodysStealingAnything)
  ACTOR_STRONGMAN_ON_WATCH = 16,
  /// One of the three thieves, drawn with long purple hair as in the loft,
  /// standing outside until the party confronts them
  /// (FLAG_VAULT_THIEVES_ATTACKED).
  /// ★ Named from the game: "Are you accusing us of stealing Hammet's treasured
  /// Shaman's Rod?" (MsgVaultAccusingUsStealingHammetsTreasured)
  ACTOR_THIEF_IN_TOWN = 17,
  /// A bearded man in a long robe.
  /// ★ Named from the game: "I was just about to leave this town behind me when
  /// the volcano erupted." (MsgVaultIWasJustAboutToLeave)
  ACTOR_STRANDED_TRAVELER = 18,
  /// ★ Named from what the player sees: a white dog sitting among the graves
  /// (its render)
  ACTOR_VAULT_DOG = 19,
  /// A young man who fixes the roof himself, and is later found tied up in the
  /// inn's loft.
  /// ★ Named from the game: "Why pay for a carpenter when I can just hammer it
  /// a bit myself?" (MsgVaultFixTheRoofWhyPayFor)
  ACTOR_YOUNG_MAN_FIXING_THE_ROOF = 20,
  /// A woman who walks about looking for ACTOR_YOUNG_MAN_FIXING_THE_ROOF.
  /// ★ Named from the game: "He should be fixing the roof now, but I can't
  /// find him anywhere..." (MsgVaultFixingRoofCant)
  ACTOR_WOMAN_WANTING_THE_ROOF_FIXED = 21,
  /// Sap, a Venus Djinni, hiding until the bell calls it.
  /// ★ Named from the game: "Sap" (MsgDjinnName)
  ACTOR_SAP = 23
};

static void Vault_ShowStandingActorMessage(s32 actorId);
static void Vault_ShowMessageAndWait(s32 actorId, s32 delay);
static void Vault_FaceActorAndWait(s32 actorId, s32 other, s32 delay);

#include "common/effect_create.h"
#include "common/effect_spawn.h"

#include "common/vault_nearby.h"

// The woman wanting the roof fixed walks about looking for the man who
// should be fixing it and stops to face
// the leader when near, or while a conversation holds the field.
// UB: calling this s32 function through a void update routine is incompatible.
// Plain void changes 108 to 104 bytes in tbs-en.
#ifdef UBFIX
static void Vault_UpdateRoofComplainerProximity(struct Actor *updated) {
#else
static s32 Vault_UpdateRoofComplainerProximity(struct Actor *updated) {
#endif
  struct Actor *entity = (struct Actor *)updated;
  struct WindowWork *work = (struct WindowWork *)gWindowWork[0];
  u8 *workspace = gWindowWork[12];
  s32 flag = 0;
  s32 selector = 18;
  struct Actor *leader;

  if (entity->targetX == ACTOR_NO_TARGET)
#ifdef UBFIX
    return;
#else
    return 0;
#endif

  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (*(s16 *)(workspace + 376) != 0 || work->mindRead != 0) {
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

const struct SceneEntrance *Scene_GetEntrances(void) {
  return gVault_Entrances;
}

const struct SceneExitRegion *Scene_GetExitRegions(void) { return NULL; }

const u32 *Scene_GetExits(void) { return gVault_Exits; }

const struct ActorTemplate *Scene_GetActorTemplates(void) {
  if (GameFlag_Get(FLAG_VAULT_THIEVES_JAILED) != 0)
    return gVault_ActorTemplatesAfterThieves;
  return gVault_ActorTemplates;
}

/// Counts the Venus Djinn that must leave Ivan before he leaves the party.
static int Vault_GetIvanVenusDjinnCount(void) {
  return Unit_Get(IVAN)->djinnHeldCounts[ELEMENT_VENUS];
}

#include "common/vault_djinn.h"

/// Leaving Vault by either gate (MB_VAULT_SOUTH_GATE and _NORTH_GATE, before
/// VAULT_TO_WORLD_MAP_SOUTH and VAULT_TO_WORLD_MAP_NORTH): while Ivan is
/// helping search for the rod and the thieves are free, he walks out to the
/// gate, says he is still worried about the rod and will wait in Vault, leaves
/// any Venus Djinni with Isaac and leaves the party.
static void Vault_IvanStaysBehind(void) {
  struct Actor *actor;
  struct FieldWork *work;

  work = gFieldWork;
  if (GameFlag_Get(FLAG_VAULT_THIEVES_JAILED) != 0 ||
      GameFlag_Get(FLAG_VAULT_IVAN_JOINED_ROD_SEARCH) == 0) {
    Import_EventSetExitRequest(
        work->stepTrigger - (MB_VAULT_SOUTH_GATE - VAULT_TO_WORLD_MAP_SOUTH));
    return;
  }
  // Bring Ivan out beside the party at the exit they are using.
  Event_Start();
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Actor_SetPosition(ACTOR_IVAN, actor->x.fixed, actor->z.fixed);
  }
  Actor_SetSpeed(ACTOR_IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
  if (work->stepTrigger == MB_VAULT_SOUTH_GATE) {
    Actor_WalkToAndWait(ACTOR_IVAN, 400, 448);
  } else {
    FieldCamera_SetSpeed(PIXELS(4) / 5, PIXELS(1) / 10);
    FieldCamera_MoveTo(PIXELS(224), -1, PIXELS(162), 1);
    Actor_WalkToAndWait(ACTOR_IVAN, 224, 162);
    FieldCamera_WaitForArrival();
  }
  Actor_LinkPair(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
  Event_WaitFrames(20);
  Event_SetMessageId(MsgVaultLeavingImStillWorriedAbout);
  FieldEvent_ShowMessageThenWait(
      SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT | ACTOR_IVAN, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  // Return any Djinn before removing Ivan from the party.
  if (Vault_GetIvanVenusDjinnCount() != 0) {
    Event_SetMessageId(MsgVaultWaitDontWantTakeYour);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Vault_ReturnIsaacsDjinni();
    WaitFrames(20);
  }
  Party_RemoveMemberRestored(IVAN);
  Import_EventSetExitRequest(work->stepTrigger -
                             (MB_VAULT_SOUTH_GATE - VAULT_TO_WORLD_MAP_SOUTH));
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

/// In Vault, Isaac peers into the well and hears water rumbling down there.
static void Vault_PeerIntoWell(void) {
  Event_Start();
  FieldMessage_Show(MsgFieldPeeredWell, 1);
  FieldMessage_Show(MsgVaultCanHearWaterRumblingDown, 1);
  Event_End();
}

const struct Trigger *Scene_GetTriggers(void) {
  if (GameFlag_Get(FLAG_VAULT_THIEVES_JAILED) != 0)
    return gVault_TriggersAfterThieves;
  return gVault_Triggers;
}

/// Asks the current question and selects its ordinary or alternate reply.
static inline void Vault_AskYesNoAndShowReply(s32 speaker) {
  FieldMessage_ShowAtActor(speaker, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) != 0) {
    struct FieldWork *work = gFieldWork;

    ++work->message;
  }
  Event_ShowMessageAndWait(speaker, 0);
}

/// In Vault while the rod is missing, a townsperson asks about the eruption.
/// "Wasn't the eruption of Mt. Aleph incredible?" Yes: too terrified to sleep
/// for days. No: aren't you courageous!
static void Vault_AskAboutEruption(void) {
  Event_Start();
  Event_SetMessageId(MsgVaultWasntEruptionMtAlephIncredible);
  Vault_FaceActorAndWait(ACTOR_ERUPTION_WITNESS, ACTOR_PARTY_LEADER, 2);
  Event_OpenMessage(ACTOR_ERUPTION_WITNESS, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) != 0) {
    gFieldWork->message++;
  }
  Event_ShowMessage(ACTOR_ERUPTION_WITNESS, 0);
  Actor_FaceDirection(ACTOR_ERUPTION_WITNESS, FACING_SOUTH + FACING_STEP, 0);
  Event_End();
}

/// In Vault while the rod is missing, a townsperson says travelers left town
/// in a big hurry: did the party see them? Yes: two women and four men, the
/// girl and old man unhappy to be there. No: maybe they left earlier.
static void Vault_AskAboutFleeingTravelers(void) {

  Event_Start();
  Event_SetMessageId(MsgVaultThoseTravelersLeftInBig);
  Import_ActorSetAnimation(ACTOR_OLD_WOMAN_ON_THE_TRAVELERS, ANIM_STAND);
  Vault_FaceActorAndWait(ACTOR_OLD_WOMAN_ON_THE_TRAVELERS, ACTOR_PARTY_LEADER,
                         2);
  Vault_AskYesNoAndShowReply(ACTOR_OLD_WOMAN_ON_THE_TRAVELERS);
  Event_End();
}

/// In Vault, one of the thieves loitering in town asks whether the party
/// accuses them; either answer gets a denial.
/// "Are you accusing us of stealing Hammet's treasured Shaman's Rod?"
static void Vault_ThiefDeniesTheft(void) {

  Event_Start();
  Event_SetMessageId(MsgVaultAccusingUsStealingHammetsTreasured);
  Vault_FaceActorAndWait(ACTOR_THIEF_IN_TOWN, ACTOR_PARTY_LEADER, 2);
  Vault_AskYesNoAndShowReply(ACTOR_THIEF_IN_TOWN);
  Event_End();
}

/// A stranded traveler asks whether the party is off on an adventure.
/// The traveler was stranded by the eruption, and asks while the rod is
/// missing.
/// "Off on an adventure, are you?"
/// Yes: impressive for youngsters. No: see the Sanctum while you're here.
static void Vault_AskAboutAdventure(void) {

  Event_Start();
  Event_SetMessageId(MsgVaultOffOnAdventure);
  Vault_FaceActorAndWait(ACTOR_STRANDED_TRAVELER, ACTOR_PARTY_LEADER, 2);
  Vault_AskYesNoAndShowReply(ACTOR_STRANDED_TRAVELER);
  Event_End();
}

/// A townsperson says some folks got robbed in the chaos of the eruption.
/// She says it while the rod is missing.
static void Vault_TalkOfEruptionThefts(void) {
  Event_Start();
  Event_SetMessageId(MsgVaultGuessFolksGot);
  Vault_ShowStandingActorMessage(ACTOR_WOMAN_ON_STEALING);
  Event_End();
}

/// In Vault while the rod is missing, the strongman on watch: nobody steals on
/// his watch, the thieves must be from out of town.
static void Vault_StrongmanOnWatchSuspectsOutsiders(void) {
  Event_Start();
  Event_SetMessageId(MsgVaultNobodysStealingAnything);
  Vault_ShowStandingActorMessage(ACTOR_STRONGMAN_ON_WATCH);
  Event_End();
}

/// In Vault while the rod is missing, the dog stops and barks.
static void Vault_TalkToDog(void) {
  Event_Start();
  Event_SetMessageId(MsgVaultRuffRrruff);
  Import_ActorSetAnimation(ACTOR_VAULT_DOG, 0);
  Vault_FaceActorAndWait(ACTOR_VAULT_DOG, ACTOR_PARTY_LEADER, 2);
  Event_ShowMessageAndWait(ACTOR_VAULT_DOG, 0);
  Event_End();
}

/// In Vault while the rod is missing, the boy round the well stops: poor Ivan
/// was left at the mayor's to look for the stolen rod.
static void Vault_PityIvan(void) {
  struct Actor *actor = Actor_GetById(ACTOR_BOY_ROUND_THE_WELL);
  s16 savedFacing = actor->facing;

  actor->timer |= 2;
  Event_Start();
  Event_SetMessageId(MsgVaultPoorGuyLeft);
  Import_ActorSetAnimation(ACTOR_BOY_ROUND_THE_WELL, 0);
  Vault_FaceActorAndWait(ACTOR_BOY_ROUND_THE_WELL, ACTOR_PARTY_LEADER, 2);
  Vault_ShowMessageAndWait(ACTOR_BOY_ROUND_THE_WELL, 10);
  actor->facing = savedFacing;
  WaitFrames(1);
  Event_End();
  actor->timer &= 1;
}

/// In Vault while the rod is missing, the girl round the well stops: Hammet's
/// caravan left as soon as the eruption stopped.
static void Vault_TalkOfHammetsCaravan(void) {
  struct Actor *actor = Actor_GetById(ACTOR_GIRL_ROUND_THE_WELL);
  s16 savedFacing = actor->facing;

  actor->timer |= 2;
  Event_Start();
  Event_SetMessageId(MsgVaultMasterHammetsCaravan);
  Import_ActorSetAnimation(ACTOR_GIRL_ROUND_THE_WELL, 0);
  Vault_FaceActorAndWait(ACTOR_GIRL_ROUND_THE_WELL, ACTOR_PARTY_LEADER, 2);
  Vault_ShowMessageAndWait(ACTOR_GIRL_ROUND_THE_WELL, 10);
  actor->facing = savedFacing;
  WaitFrames(1);
  Event_End();
  actor->timer &= 1;
}

/// In Vault, once the suspects' minds are read, the woman wanting
/// the roof fixed: the man who should be fixing it is probably in back
/// slacking off.
static void Vault_LookForRoofer(void) {
  Event_Start();
  Event_SetMessageId(MsgVaultFixingRoofCant);
  Vault_FaceActorAndWait(ACTOR_WOMAN_WANTING_THE_ROOF_FIXED, ACTOR_PARTY_LEADER,
                         2);
  FieldEffect_SpawnAttached(ACTOR_WOMAN_WANTING_THE_ROOF_FIXED,
                            EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  Event_WaitFrames(30);
  FieldMessage_ShowAtActor(ACTOR_WOMAN_WANTING_THE_ROOF_FIXED, 0);
  Event_End();
}

/// Stops an actor, faces the leader and shows the current message.
static void Vault_ShowStandingActorMessage(s32 actorId) {
  Event_Start();
  Import_ActorSetAnimation(actorId, ANIM_STAND);
  Vault_FaceActorAndWait(actorId, ACTOR_PARTY_LEADER, 2);
  Event_ShowMessageAndWait(actorId, 0);
  Event_End();
}

#include "common/vault_talk.h"

/// The welcoming young man asks whether the party is leaving again.
/// He asks after the thieves are jailed. "So, you're hitting the road again?"
static void Vault_AskAboutLeavingAgain(void) {
  Event_Start();
  Event_SetMessageId(MsgVaultYoureHittingRoadAgain);
  Vault_FaceActorAndWait(ACTOR_WELCOMING_YOUNG_MAN, ACTOR_PARTY_LEADER, 2);
  Vault_AskYesNoAndShowReply(ACTOR_WELCOMING_YOUNG_MAN);
  Event_End();
}

/// In Vault after the thieves are jailed, a townsperson praises their capture.
/// "It's great that you caught those thieves!"
static void Vault_PraiseThievesCapture(void) {
  Event_Start();
  Event_SetMessageId(MsgVaultGreatCaughtThieves);
  Vault_ShowStandingActorMessage(ACTOR_WOMAN_ON_STEALING);
  Event_End();
}

/// An old man talks about Ivan, Hammet's servant.
/// After the thieves are jailed, he says Ivan went out to Lunpa, or, with Ivan
/// in the party, that rescuing Hammet must have been impossible.
static void Vault_TalkAboutIvan(void) {
  struct FieldWork *workspace;
  Event_Start();
  Event_SetMessageId(MsgVaultTalkingAboutHammetsServantIvan);
  if (GameFlag_Get(FLAG_IN_PARTY(IVAN)) != 0) {
    workspace = gFieldWork;
    ++workspace->message;
  }
  Vault_ShowStandingActorMessage(ACTOR_OLD_MAN_ON_HAMMETS_SERVANT);
  Event_End();
}

/// In Vault after the thieves are jailed, a townsperson says the eruption
/// devastated the roads; once the southern bridge is repaired, you can head
/// toward Kalay.
static void Vault_TalkOfDamagedRoads(void) {
  Event_Start();
  Event_SetMessageId(MsgVaultEruptionDevastatedRoads);
  Vault_ShowStandingActorMessage(ACTOR_OLD_WOMAN_ON_THE_TRAVELERS);
  Event_End();
}

/// The boy round the well asks after Ivan.
/// After the thieves are jailed, he hopes Ivan didn't go to Lunpa alone, or,
/// with Ivan in the party, asks when he joined up with Isaac.
static void Vault_AskWhetherIvanWentToLunpa(void) {
  struct Actor *actor = Actor_GetById(ACTOR_BOY_ROUND_THE_WELL);
  s16 savedFacing = actor->facing;

  actor->timer |= 2;
  Event_Start();
  Event_SetMessageId(MsgVaultDidntGoLunpa);
  if (GameFlag_Get(FLAG_IN_PARTY(IVAN)) != 0)
    ++gFieldWork->message;
  Import_ActorSetAnimation(ACTOR_BOY_ROUND_THE_WELL, 0);
  Vault_FaceActorAndWait(ACTOR_BOY_ROUND_THE_WELL, ACTOR_PARTY_LEADER, 2);
  Vault_ShowMessageAndWait(ACTOR_BOY_ROUND_THE_WELL, 10);
  actor->facing = savedFacing;
  WaitFrames(1);
  Event_End();
  actor->timer &= 1;
}

/// In Vault after the thieves are jailed, the girl round the well: everyone is
/// grateful the party captured the thieves.
static void Vault_ThankForCapturingThieves(void) {
  struct Actor *actor = Actor_GetById(ACTOR_GIRL_ROUND_THE_WELL);
  s16 savedFacing = actor->facing;

  actor->timer |= 2;
  Event_Start();
  Event_SetMessageId(MsgVaultEveryoneGratefulCaptured);
  Import_ActorSetAnimation(ACTOR_GIRL_ROUND_THE_WELL, 0);
  Vault_FaceActorAndWait(ACTOR_GIRL_ROUND_THE_WELL, ACTOR_PARTY_LEADER, 2);
  Vault_ShowMessageAndWait(ACTOR_GIRL_ROUND_THE_WELL, 10);
  actor->facing = savedFacing;
  WaitFrames(1);
  Event_End();
  actor->timer &= 1;
}

/// In Vault after the thieves are jailed, the strongman on watch asks about the
/// prisoners. "Those men you captured... They're in jail now?" Yes: the
/// prison hadn't been used in a long time. No: they're upset; maybe go see
/// them.
static void Vault_AskAboutPrisoners(void) {
  Event_Start();
  Event_SetMessageId(MsgVaultThoseMenCapturedTheyreIn);
  Import_ActorSetAnimation(ACTOR_STRONGMAN_ON_WATCH, ANIM_STAND);
  Vault_FaceActorAndWait(ACTOR_STRONGMAN_ON_WATCH, ACTOR_PARTY_LEADER, 2);
  Vault_AskYesNoAndShowReply(ACTOR_STRONGMAN_ON_WATCH);
  Event_End();
}

/// In Vault after the thieves are jailed, the traveler asks who captured the
/// thieves; either answer is praised.
/// "Are you the ones who captured the thieves?"
static void Vault_AskWhoCapturedThieves(void) {
  Event_Start();
  Event_SetMessageId(MsgVaultOnesWhoCapturedThieves);
  Vault_FaceActorAndWait(ACTOR_STRANDED_TRAVELER, ACTOR_PARTY_LEADER, 2);
  Vault_AskYesNoAndShowReply(ACTOR_STRANDED_TRAVELER);
  Event_End();
}

/// In Vault after the thieves are jailed, the dog barks; carrying a Bone
/// before it has shown the passage lets the party give it one.
static void Vault_TalkToDogAfterCapture(void) {
  Event_Start();
  Event_SetMessageId(MsgVaultRuffRrruff2);
  Import_ActorSetAnimation(ACTOR_VAULT_DOG, 0);
  Vault_FaceActorAndWait(ACTOR_VAULT_DOG, ACTOR_PARTY_LEADER, 2);
  Event_ShowMessageAndWait(ACTOR_VAULT_DOG, 0);
  Import_ActorSetAnimation(ACTOR_VAULT_DOG, ANIM_STAND);
  if (PartyInventory_FindMember(ITEM_BONE) != -1 &&
      GameFlag_Get(FLAG_VAULT_DOG_SHOWN_PASSAGE) == 0) {
    u16 *menuRequest = &gFieldWork->fieldMenu;
    u16 open = 1;

    *menuRequest = open;
  }
  Event_End();
}

/// The young man fixing the roof thanks the party before it leaves.
/// The party freed him, and he says so after the thieves are jailed.
/// "Thank you for the other day.
/// Are you leaving town already?"
static void Vault_ThankBeforeDeparture(void) {
  Event_Start();
  Event_SetMessageId(MsgVaultThankForOtherDayLeaving);
  Vault_FaceActorAndWait(ACTOR_YOUNG_MAN_FIXING_THE_ROOF, ACTOR_PARTY_LEADER,
                         2);
  Actor_SetAnimationAndWait(ACTOR_YOUNG_MAN_FIXING_THE_ROOF, ANIM_NOD);
  Event_WaitFrames(20);
  Event_ShowMessageAndWait(ACTOR_YOUNG_MAN_FIXING_THE_ROOF, 0);
  Event_End();
}

/// In Vault, Mind Read on a townsperson: stealing during an eruption is
/// unforgivable, or how should the prisoners be punished?
static void Vault_ReadMindOnTheft(void) {
  Event_Start();
  if (GameFlag_Get(FLAG_VAULT_THIEVES_JAILED) == 0)
    Event_SetMessageId(MsgVaultManShouldStealFromAnother);
  else
    Event_SetMessageId(MsgVaultHowPunishPrisoners);
  Vault_ShowThought(ACTOR_WOMAN_ON_STEALING);
  Event_End();
}

/// In Vault, Mind Read on a townsperson: the strange travelers headed for the
/// northeast of the Goma Range, or the only way out is the cave in the Goma
/// Range.
static void Vault_ReadMindOnTravelers(void) {
  Event_Start();
  if (GameFlag_Get(FLAG_VAULT_THIEVES_JAILED) == 0) {
    Event_SetMessageId(MsgVaultGroupTravelersWasStrangeBunch);
  } else {
    Event_SetMessageId(MsgVaultWithRoadOutOnlyWay);
  }
  Vault_ShowThought(ACTOR_OLD_WOMAN_ON_THE_TRAVELERS);
  Event_End();
}

/// In Vault, Mind Read on the boy round the well: the three creeps at the inn
/// stole the rod, or where did Ivan go (when did they join him, with Ivan in
/// the party)?
static void Vault_ReadMindOnIvan(void) {
  Actor_GetById(ACTOR_BOY_ROUND_THE_WELL)->timer |= 2;
  Event_Start();
  if (GameFlag_Get(FLAG_VAULT_THIEVES_JAILED) == 0) {
    Event_SetMessageId(MsgVaultBetWasThoseThreeCreeps);
  } else {
    Event_SetMessageId(MsgVaultWhereDidIvanGoBy);
    if (GameFlag_Get(FLAG_IN_PARTY(IVAN)) != 0)
      ++gFieldWork->message;
  }
  Vault_ShowThought(ACTOR_BOY_ROUND_THE_WELL);
  Event_End();
  Actor_GetById(ACTOR_BOY_ROUND_THE_WELL)->timer &= 1;
}

/// In Vault, Mind Read on the girl round the well: riches can't save you from a
/// volcano, or the party must be stronger than they look.
static void Vault_ReadMindOnWealth(void) {
  Actor_GetById(ACTOR_GIRL_ROUND_THE_WELL)->timer |= 2;
  Event_Start();
  if (GameFlag_Get(FLAG_VAULT_THIEVES_JAILED) == 0)
    Event_SetMessageId(MsgVaultSupposeDoesntMatterHowRich);
  else
    Event_SetMessageId(MsgVaultMustStrongerThanLookHave);
  Vault_ShowThought(ACTOR_GIRL_ROUND_THE_WELL);
  Event_End();
  Actor_GetById(ACTOR_GIRL_ROUND_THE_WELL)->timer &= 1;
}

/// In Vault, Mind Read on the strongman on watch: everyone knows the three at
/// the inn stole the rod, or he feels much better with them behind bars.
static void Vault_ReadMindOnSuspects(void) {
  Event_Start();
  if (GameFlag_Get(FLAG_VAULT_THIEVES_JAILED) == 0)
    Event_SetMessageId(MsgVaultEveryoneKnowsThoseThreeAt);
  else
    Event_SetMessageId(MsgVaultFeelMuchBetter);
  Vault_ShowThought(ACTOR_STRONGMAN_ON_WATCH);
  Event_End();
}

/// In Vault, Mind Read on the dog: humans value money over their lives; after
/// the thieves are jailed, bring a bone for something useful, and once fed, a
/// careful search will reveal a passage leading underground.
static void Vault_ReadDogMind(void) {
  struct Actor *actor = Actor_GetById(ACTOR_VAULT_DOG);
  actor->paused = 1;
  Event_Start();
  if (GameFlag_Get(FLAG_VAULT_THIEVES_JAILED) == 0) {
    Event_SetMessageId(MsgVaultStealingInMidstVolcanicEruption);
    Import_ActorSetAnimation(ACTOR_VAULT_DOG, 0);
    Event_WaitFrames(2);
  } else if (GameFlag_Get(FLAG_VAULT_DOG_SHOWN_PASSAGE) != 0) {
    Event_SetMessageId(MsgVaultCarefulSearchWillRevealPassage);
  } else {
    Event_SetMessageId(MsgVaultIfBringMeBoneIll);
  }
  Event_ShowMessageAndWait(ACTOR_VAULT_DOG, 0);
  Event_End();
  actor->paused = 0;
}

/// In Vault, Mind Read on the woman wanting the roof fixed: that sneak is
/// wasting time instead of fixing the roof.
static void Vault_ReadWomanWantingTheRoofFixedMind(void) {
  Event_Start();
  Event_SetMessageId(MsgVaultSneakWastingTime);
  Vault_ShowThought(ACTOR_WOMAN_WANTING_THE_ROOF_FIXED);
  Event_End();
}

#include "common/vault_toggle.h"

/// Walks to the exit's threshold and requests the room's transition.
static void Vault_WalkThroughExit(s32 x, s32 y, s32 continuation) {
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
  Actor_WalkTo(ACTOR_PARTY_LEADER, x, y);
  gFieldWork->transitionFrames = 16;
  Import_EventSetExitRequest(continuation);
}

/// The door of the gem owner's house opens and the party walks in.
/// It's VAULT_TO_LOG_ROOMS_A_EAST_SOUTHEAST, into the house where a man
/// keeps a sparkling rock the volcano threw.
static void Vault_EnterGemOwnersHouse(void) {
  Sound_PlaySong(SE_DOOR_OPEN);
  MapGrid_PlayMetatileCopySequence(gVault_GemOwnersHouseDoorCells, 56, 19);
  Vault_WalkThroughExit(408, 320, VAULT_TO_LOG_ROOMS_A_EAST_SOUTHEAST);
}

/// The door of a child's home opens and the party walks in.
/// It's VAULT_TO_LOG_ROOMS_A_SOUTHEAST, into the house east of the town's
/// centre.
static void Vault_EnterChildsHouse(void) {
  Sound_PlaySong(SE_DOOR_OPEN);
  MapGrid_PlayMetatileCopySequence(gVault_ChildsHouseDoorCells, 50, 18);
  Vault_WalkThroughExit(312, 304, VAULT_TO_LOG_ROOMS_A_SOUTHEAST);
}

/// The inn's door opens and the party walks in.
/// It's VAULT_TO_LOG_ROOMS_A_SOUTHWEST, into the inn where the three
/// suspects stay.
static void Vault_EnterInn(void) {
  Sound_PlaySong(SE_DOOR_OPEN);
  MapGrid_PlayMetatileCopySequence(gVault_InnDoorCells, 44, 17);
  Vault_WalkThroughExit(216, 288, VAULT_TO_LOG_ROOMS_A_SOUTHWEST);
}

/// In Vault, the door VAULT_TO_LOG_ROOMS_A_EAST opens into the item shop,
/// an old couple's home too; the leader walks in behind the doorway's
/// foreground tiles.
static void Vault_EnterItemShop(void) {
  struct Actor *actor = Actor_GetById(ACTOR_PARTY_LEADER);
  struct Sprite *sprite = actor->sprite;

  Sound_PlaySong(SE_DOOR_OPEN);
  MapGrid_PlayMetatileCopySequence(gVault_ItemShopDoorCells, 54, 13);
  MapGrid_CopyCellAttributes(33, 20, 1, 3, 23, 12);
  actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  sprite->image.priority = 3;
  Vault_WalkThroughExit(376, 224, VAULT_TO_LOG_ROOMS_A_EAST);
}

/// The weapon and armor shop's door opens and the party walks in.
/// It's VAULT_TO_LOG_ROOMS_A_NORTH_NORTHEAST, entered as the item shop is.
static void Vault_EnterWeaponAndArmorShop(void) {
  struct Actor *actor = Actor_GetById(ACTOR_PARTY_LEADER);
  struct Sprite *sprite = actor->sprite;

  Sound_PlaySong(SE_DOOR_OPEN);
  MapGrid_PlayMetatileCopySequence(gVault_WeaponAndArmorShopDoorCells, 49, 10);
  MapGrid_CopyCellAttributes(33, 20, 1, 3, 18, 10);
  actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  sprite->image.priority = 3;
  Vault_WalkThroughExit(296, 176, VAULT_TO_LOG_ROOMS_A_NORTH_NORTHEAST);
}

/// In Vault, the door VAULT_TO_LOG_ROOMS_A_NORTHWEST opens into the
/// mayor's house, where Ivan waits.
static void Vault_EnterMayorsHouse(void) {
  Sound_PlaySong(SE_DOOR_OPEN);
  MapGrid_PlayMetatileCopySequence(gVault_MayorsHouseDoorCells, 38, 6);
  Vault_WalkThroughExit(120, 144, VAULT_TO_LOG_ROOMS_A_NORTHWEST);
}

#include "common/vault_map_cells.h"

/// In Vault, the jail door opens and the party walks in, drawn behind the
/// doorway (VAULT_TO_STONE_ROOMS, the jail's STONE_ROOMS_FROM_VAULT).
static void Vault_EnterJail(void) {
  struct Actor *actor = Actor_GetById(ACTOR_PARTY_LEADER);
  struct Sprite *sprite = actor->sprite;

  Sound_PlaySong(SE_DOOR_OPEN);
  MapGrid_PlayMetatileCopySequence(gVault_JailDoorCells, 35, 9);
  MapGrid_CopyCellAttributes(33, 20, 1, 3, 4, 10);
  actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  sprite->image.priority = 3;
  Vault_WalkThroughExit(72, 160, VAULT_TO_STONE_ROOMS);
}

/// In Vault, the party walks through a doorless entrance into the room with
/// the crate (VAULT_TO_LOG_ROOMS_A_WEST).
static void Vault_EnterPassage(void) {
  Sound_PlaySong(SE_MAP_EXIT);
  Vault_WalkThroughExit(152, 264, VAULT_TO_LOG_ROOMS_A_WEST);
}

/// Moves an actor through a jump and waits up to sixty frames for it to
/// settle.
static void Vault_JumpActorToAndWait(s32 actorId, s32 x, s32 z,
                                     s32 verticalSpeed) {
  struct Actor *actor = Actor_GetById(actorId);
  int frames;
  Actor_SetSpeedParameters(actorId, PIXELS(3), PIXELS(3) / 2);
  actor->gravity = PIXELS(1) / 2;
  actor->bounce = 0;
  actor->velocityY = verticalSpeed;
  Actor_SetSpriteFlags(actor, 0);
  Actor_SetPositionAndCommit(actorId, x, z);
  Import_ActorSetPosition(actorId, x << 16, z << 16);
  for (frames = 60; frames != 0; --frames) {
    WaitFrames(1);
    if ((actor->velocityY >> 16) == 0)
      break;
  }
  Actor_SetSpriteFlags(actor, 1);
  actor->gravity = PIXELS(1);
}

/// In Vault, Isaac rings the bell; the first time since the party came in by a
/// gate, Sap, the Venus Djinni hiding nearby, jumps out and comes to the bell
/// (FLAG_VAULT_SAP_CALLED_BY_BELL). No event here lets the party talk to
/// it; it joins in the town's later scene (vault_revisit.c).
static void Vault_RingBell(void) {
  Event_Start();
  Event_PlaySong(SE_BELL);
  Event_WaitFrames(40);
  if (Event_GetFlag(FLAG_VAULT_SAP_CALLED_BY_BELL) == 0) {
    Actor_SetAttachedEffect(ACTOR_SAP,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_Jump(ACTOR_SAP, 4, 0);
    Event_WaitFrames(12);
    Actor_Jump(ACTOR_SAP, 4, 0);
    Event_WaitFrames(20);
    Vault_JumpActorToAndWait(ACTOR_SAP, 392, 104, PIXELS(7));
    Event_WaitFrames(20);
    Actor_WalkToAndWait(ACTOR_SAP, 408, 104);
    Actor_WalkToAndWait(ACTOR_SAP, 408, 120);
    GameFlag_Set(FLAG_VAULT_SAP_CALLED_BY_BELL);
  }
  Event_End();
}

/// Given a Bone, the dog runs to the hidden passage and jumps there.
/// It does so after the thieves are jailed, jumping three times
/// (FLAG_VAULT_DOG_SHOWN_PASSAGE).
static void Vault_GiveBoneToDog(void) {
  PartyInventory_Discard(ITEM_BONE);
  Event_Start();
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_VAULT_DOG, 2);
  Actor_SetSpeed(ACTOR_VAULT_DOG, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_VAULT_DOG, 216, 408);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_VAULT_DOG, FACING_SOUTH, 20);
  Actor_Jump(ACTOR_VAULT_DOG, 6, 0);
  Event_WaitFrames(30);
  Actor_Jump(ACTOR_VAULT_DOG, 6, 0);
  Event_WaitFrames(30);
  Actor_Jump(ACTOR_VAULT_DOG, 6, 0);
  Event_WaitFrames(30);
  Actor_WalkToAndWait(ACTOR_VAULT_DOG, 216, 392);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_VAULT_DOG, FACING_SOUTH, 20);
  GameFlag_Set(FLAG_VAULT_DOG_SHOWN_PASSAGE);
  Event_End();
}

/// Arriving in Vault: restores the dog's and Sap's places, sets the two
/// children and the woman wanting the roof fixed stopping near the leader,
/// and, while Ivan waits in Vault for the party, has him greet the two and
/// rejoin.
s32 Scene_Initialize(void) {
  s32 suspectsMindRead;
  struct Actor *actor;
  s32 lowBit;
  s32 zero;

  // Once the party has reached Kalay (FLAG_KALAY_REACHED), the town is its
  // later scene, SceneId_VaultRevisit (vault_revisit.c).
  if (GameFlag_Get(FLAG_KALAY_REACHED) != 0) {
    Import_EventSetExitRequest(VAULT_TO_VAULT_REVISIT);
  }
  // Keep the bridge deck after a battle, and restore the two children's
  // proximity update routines.
  if (GameFlag_Get(FLAG_VAULT_ON_BRIDGE) != 0) {
    MapGrid_CopyCellAttributes(55, 26, 4, 2, 23, 26);
  }
  // A still actor on the well that shows nothing (vault_revisit.c's open
  // questions).
  Actor_CreateConfigured(PIXELS(128), 0, PIXELS(420), SPRITE_CRATE);
  lowBit = 1;
  zero = 0;
  actor = Actor_GetById(ACTOR_BOY_ROUND_THE_WELL);
  actor->timer = lowBit;
#ifdef UBFIX
  actor->update = Vault_UpdateWalkingVillager;
#else
  actor->update = (void (*)(struct Actor *))Vault_UpdateWalkingVillager;
#endif
  actor = Actor_GetById(ACTOR_GIRL_ROUND_THE_WELL);
  actor->timer = zero;
#ifdef UBFIX
  actor->update = Vault_UpdateWalkingVillager;
#else
  actor->update = (void (*)(struct Actor *))Vault_UpdateWalkingVillager;
#endif
  if (GameFlag_Get(FLAG_VAULT_DOG_SHOWN_PASSAGE) != 0) {
    Actor_SetPosition(ACTOR_VAULT_DOG, PIXELS(216), PIXELS(392));
  }
  suspectsMindRead = GameFlag_Get(FLAG_VAULT_SUSPECTS_MIND_READ);
  if (GameFlag_Get(FLAG_VAULT_THIEVES_JAILED) == 0) {
    if ((lowBit & suspectsMindRead) != 0) {
      actor = Actor_GetById(ACTOR_WOMAN_WANTING_THE_ROOF_FIXED);
#ifdef UBFIX
      actor->update = Vault_UpdateRoofComplainerProximity;
#else
      actor->update =
          (void (*)(struct Actor *))Vault_UpdateRoofComplainerProximity;
#endif
    }
  }
  // Back through the gates while Ivan waits in Vault: "Welcome back, you two!
  // Let's go find the rod!"
  if (gSaveBlock.entrance <= VAULT_NORTH_UNUSED) {
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
      GameFlag_Clear(FLAG_VAULT_SAP_CALLED_BY_BELL);
      if (GameFlag_Get(FLAG_VAULT_THIEVES_JAILED) == 0) {
        if (GameFlag_Get(FLAG_VAULT_IVAN_JOINED_ROD_SEARCH) != 0) {
          Event_Start();
          actor = Actor_GetById(ACTOR_PARTY_LEADER);
          if (actor != 0) {
            Import_ActorSetPosition(ACTOR_IVAN, actor->x.fixed, actor->z.fixed);
          }
          if (gSaveBlock.entrance == VAULT_FROM_WORLD_MAP_SOUTH) {
            Actor_SetPosition(ACTOR_IVAN, PIXELS(400), PIXELS(448));
          } else {
            Actor_SetPosition(ACTOR_IVAN, PIXELS(224), PIXELS(162));
          }
          Actor_LinkPair(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
          Event_StartScreenTransition();
          Event_WaitTransitionFrames();
          Event_WaitFrames(30);
          Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
          Event_SetMessageId(MsgVaultWelcomeBackTwo);
          Event_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
          Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
          Actor_SetSpeed(ACTOR_IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
          Import_ActorSetAnimation(ACTOR_IVAN, ANIM_WALK);
          actor = Actor_GetById(ACTOR_PARTY_LEADER);
          if (actor != 0) {
            Actor_ResetAndSetPosition(ACTOR_IVAN, actor->x.part.pixel,
                                      actor->z.part.pixel);
          }
          Actor_WaitForMoveById(ACTOR_IVAN);
          Import_ActorSetPosition(ACTOR_IVAN, 0, 0);
          Event_PrepareActorAndApplyValue(IVAN, 0);
          Event_End();
        }
      }
    }
  }
  if (GameFlag_Get(FLAG_VAULT_SAP_CALLED_BY_BELL) != 0) {
    Actor_SetPosition(ACTOR_SAP, PIXELS(408), PIXELS(120));
  }
  // The same welcome out of the Vault Sanctum while Ivan is out of the party.
  if (gSaveBlock.entrance == VAULT_FROM_SANCTUM) {
    if (GameFlag_Get(FLAG_VAULT_THIEVES_JAILED) == 0) {
      if (GameFlag_Get(FLAG_VAULT_IVAN_JOINED_ROD_SEARCH) != 0) {
        if (GameFlag_Get(FLAG_IN_PARTY(IVAN)) == 0) {
          Event_Start();
          Actor_SetPosition(ACTOR_IVAN, PIXELS(40), PIXELS(310));
          Actor_LinkPair(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
          Event_StartScreenTransition();
          Event_WaitTransitionFrames();
          Event_WaitFrames(30);
          Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
          Event_SetMessageId(MsgVaultWelcomeBackTwo);
          Event_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
          Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
          Actor_SetSpeed(ACTOR_IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
          Import_ActorSetAnimation(ACTOR_IVAN, ANIM_WALK);
          actor = Actor_GetById(ACTOR_PARTY_LEADER);
          if (actor != 0) {
            Actor_ResetAndSetPosition(ACTOR_IVAN, actor->x.part.pixel,
                                      actor->z.part.pixel);
          }
          Actor_WaitForMoveById(ACTOR_IVAN);
          Import_ActorSetPosition(ACTOR_IVAN, 0, 0);
          Event_PrepareActorAndApplyValue(IVAN, 0);
          Event_End();
        }
      }
    }
    GameFlag_Clear(FLAG_SHOW_AREA_NAME);
  } else {
    if (gSaveBlock.entrance == VAULT_FROM_LOG_ROOMS_A_WEST) {
      if (GameFlag_Get(FLAG_VAULT_THIEVES_JAILED) != 0) {
        Import_ActorSetPosition(ACTOR_YOUNG_MAN_FIXING_THE_ROOF, 0, 0);
      }
    }
  }
  return 0;
}

#include "common/actor_distance.h"
#include "common/vault_range.h"

static void Vault_ShowMessageAndWait(s32 actorId, s32 delay) {
  Event_ShowMessageAndWait(actorId, 0);
  Event_WaitFrames(delay);
}

static void Vault_FaceActorAndWait(s32 actorId, s32 other, s32 delay) {
  Actor_LinkPair(actorId, other, 0);
  Event_WaitFrames(delay);
}

/// Applies drift and gradually damps both rates toward zero.
/// This custom update uses speed/acceleration as its two drift rates and
/// mirrors the resulting position into the actor's target coordinates.
static void Vault_UpdateDriftingEffect(struct Actor *updated) {
  struct Actor *effect = (struct Actor *)updated;
  s32 horizontalRate;
  s32 verticalRate;

  effect->x.fixed += effect->speedLimit;
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

  horizontalRate = effect->speedLimit;
  effect->speedLimit = horizontalRate - Math_Divide(horizontalRate, 28);
  verticalRate = effect->acceleration;
  effect->acceleration = verticalRate - Math_Divide(verticalRate, 28);
}

/// Unused: spawns a drifting sprite near the dog, sharing its drawing
/// priority; nothing calls it.
UNUSED static void Vault_SpawnDriftingEffect(s32 flags) {
  struct Actor *anchor;
  struct Actor *effect;
  struct Sprite *sprite;
  s32 x;
  s32 z;
  u8 zero;

  anchor = Actor_GetById(ACTOR_VAULT_DOG);
  if (anchor == NULL)
    return;
  // Pick a small offset around the anchor and create the drifting sprite there.
  x = Import_RandomNext();
  z = Import_RandomNext();
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
    effect = Actor_Create(0xac, x, baseY, z);
  }
  if (effect == NULL)
    return;
  sprite = effect->sprite;
  switch (Import_RandomNext() & 1) {
  case 1:
    Actor_SelectAnimation(effect, 3);
    Actor_StartScript(effect, gVault_DriftScriptA);
    break;
  default:
    Actor_SelectAnimation(effect, 2);
    Actor_StartScript(effect, gVault_DriftScriptB);
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

    driftMultiplier = (u32)Import_RandomNext() % 10 + 5;
    /* FAKEMATCH(temporary): the coordinate local x holds the direction mask;
     * as the literal 1, 47 of 141 instructions differ in tbs-en. */
    x = 1;
    flags &= x;
    directionBias = flags ^ x;
    directionBias <<= 2;
    driftMultiplier += directionBias;
    effect->acceleration =
        (2 * VAULT_DRIFT_RATE * flags - VAULT_DRIFT_RATE) * driftMultiplier;
    driftMultiplier = (u32)Import_RandomNext() % 15 - 7;
    effect->speedLimit = VAULT_DRIFT_RATE * driftMultiplier;
    effect->timer = zero;
  } else {
    s32 driftMultiplier;

    driftMultiplier = (u32)Import_RandomNext() % 10 + 8;
    effect->speedLimit =
        (2 * VAULT_DRIFT_RATE * flags - VAULT_DRIFT_RATE) * driftMultiplier;
    driftMultiplier = (u32)Import_RandomNext() % 14 + 1;
    effect->acceleration = VAULT_DRIFT_RATE * driftMultiplier;
    effect->timer = 1;
  }
  // Install the custom motion and share the anchor's drawing priority.
  {
    void (*update)(struct Actor *) = Vault_UpdateDriftingEffect;
    u8 *spriteFlags;

    /* FAKEMATCH(one-pass): the block takes the flags address before the update
     * store; as sprite->flags = 0 after it, 1 of 141 instructions differ in
     * tbs-en. */
    do {
      spriteFlags = &sprite->flags;
    } while (0);
    effect->update = update;
    *spriteFlags = 0;
  }
  sprite->image.priority = anchor->sprite->image.priority;
}

// The three scripts of gFieldEffect_DurationScripts, which FieldEffect_Spawn
// picks by the low bits of its flags: each holds its effect for about 24, 46 or
// 128 frames, then releases it.
s32 gVault_ShortEffectScript[] = {
    SC_WAIT(1),           SC_LOOP(22, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),          SC_RELEASE,
};

s32 gVault_MediumEffectScript[] = {
    SC_WAIT(1),           SC_LOOP(44, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),          SC_RELEASE,
};

s32 gVault_LongEffectScript[] = {
    SC_WAIT(1),           SC_LOOP(126, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),           SC_RELEASE,
};

const s32 *gFieldEffect_DurationScripts[] = {
    gVault_ShortEffectScript,
    gVault_MediumEffectScript,
    gVault_LongEffectScript,
};

/// ACTOR_WOMAN_WANTING_THE_ROOF_FIXED's search for the young man fixing the
/// roof: to one end of her path and the other, looking west and east at each,
/// for as long as the party stays on the map.
s32 gVault_WomanWantingTheRoofFixedRoute[] = {
    SC_SET(SW_SPEED, PIXELS(2) / 5),
    SC_SET(SW_ACCELERATION, PIXELS(1) / 5),
    SC_LABEL(1),
    SC_SET(SW_FACING, FACING_WEST),
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(168), 0, PIXELS(312)),
    SC_SYNC,
    SC_ANIMATION(1),
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
    SC_ANIMATION(4),
    SC_WAIT(240),
    SC_SET(SW_FACING, FACING_EAST),
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(200), 0, PIXELS(312)),
    SC_SYNC,
    SC_ANIMATION(1),
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
    SC_ANIMATION(4),
    SC_WAIT(240),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
    SC_SET(SW_SPEED, PIXELS(2) / 5),
    SC_SET(SW_ACCELERATION, PIXELS(1) / 5),
    SC_SET(SW_COLLISION, 129),
    SC_SET(SW_RADIUS, 0x280000),
    SC_LABEL(1),
    SC_ANIMATION(2),
    SC_WANDER_HOME(0x80000, 0x100000, 0x180000),
    SC_SYNC,
    SC_ANIMATION(0),
    SC_WAIT(40),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
};

/// ACTOR_BOY_ROUND_THE_WELL's walk, round eight points for as long as the
/// party stays on the map.
s32 gVault_BoyRoundTheWellRoute[] = {
    SC_ANIMATION(2),
    SC_SET(SW_SPEED, PIXELS(4) / 5),
    SC_SET(SW_ACCELERATION, PIXELS(1) / 5),
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

/// ACTOR_GIRL_ROUND_THE_WELL's walk round the same eight points, half a
/// circle apart.
s32 gVault_GirlRoundTheWellRoute[] = {
    SC_ANIMATION(2),
    SC_SET(SW_SPEED, PIXELS(4) / 5),
    SC_SET(SW_ACCELERATION, PIXELS(1) / 5),
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
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 223, 0, 135, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {VAULT_FROM_WORLD_MAP_SOUTH, CONDITION_ALWAYS, 400, 0, 472, FACING_NORTH, 0,
     -1, -1, -1, -1, 0},
    {VAULT_NORTH_UNUSED, CONDITION_ALWAYS, 224, 0, 138, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {VAULT_FROM_LOG_ROOMS_A_EAST_SOUTHEAST, CONDITION_ALWAYS, 408, 0, 344,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {VAULT_FROM_LOG_ROOMS_A_SOUTHEAST, CONDITION_ALWAYS, 312, 0, 328,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {VAULT_FROM_LOG_ROOMS_A_SOUTHWEST, CONDITION_ALWAYS, 216, 0, 312,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {VAULT_FROM_LOG_ROOMS_A_EAST, CONDITION_ALWAYS, 376, 0, 248, FACING_SOUTH,
     0, -1, -1, -1, -1, 0},
    {VAULT_FROM_LOG_ROOMS_A_NORTH_NORTHEAST, CONDITION_ALWAYS, 296, 0, 200,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {VAULT_FROM_LOG_ROOMS_A_NORTHWEST, CONDITION_ALWAYS, 120, 0, 168,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {VAULT_FROM_SANCTUM, CONDITION_ALWAYS, 64, 0, 294, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {VAULT_FROM_STONE_ROOMS, CONDITION_ALWAYS, 70, 0, 183, FACING_SOUTH, 0, -1,
     -1, -1, -1, 0},
    {VAULT_FROM_LOG_ROOMS_A_WEST, CONDITION_ALWAYS, 152, 0, 272, FACING_SOUTH,
     0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gVault_Exits[] = {
    SCENE_EXITS(Vault),
    SCENE_EXIT(VAULT_TO_WORLD_MAP_SOUTH, WorldMap, WORLD_MAP_FROM_VAULT_SOUTH),
    SCENE_EXIT(VAULT_TO_WORLD_MAP_NORTH, WorldMap, WORLD_MAP_FROM_VAULT_NORTH),
    SCENE_EXIT(VAULT_TO_LOG_ROOMS_A_EAST_SOUTHEAST, LogRoomsA,
               LOG_ROOMS_A_FROM_VAULT_EAST_SOUTHEAST),
    SCENE_EXIT(VAULT_TO_LOG_ROOMS_A_SOUTHEAST, LogRoomsA,
               LOG_ROOMS_A_FROM_VAULT_SOUTHEAST),
    SCENE_EXIT(VAULT_TO_LOG_ROOMS_A_SOUTHWEST, LogRoomsA,
               LOG_ROOMS_A_FROM_VAULT_SOUTHWEST),
    SCENE_EXIT(VAULT_TO_LOG_ROOMS_A_EAST, LogRoomsA,
               LOG_ROOMS_A_FROM_VAULT_EAST),
    SCENE_EXIT(VAULT_TO_LOG_ROOMS_A_NORTH_NORTHEAST, LogRoomsA,
               LOG_ROOMS_A_FROM_VAULT_NORTH_NORTHEAST),
    SCENE_EXIT(VAULT_TO_LOG_ROOMS_A_NORTHWEST, LogRoomsA,
               LOG_ROOMS_A_FROM_VAULT_NORTHWEST),
    SCENE_EXIT(VAULT_TO_SANCTUM, Sanctum, SANCTUM_FROM_VAULT),
    SCENE_EXIT(VAULT_TO_STONE_ROOMS, StoneRooms, STONE_ROOMS_FROM_VAULT),
    SCENE_EXIT(VAULT_TO_LOG_ROOMS_A_WEST, LogRoomsA,
               LOG_ROOMS_A_FROM_VAULT_WEST),
    SCENE_EXIT(VAULT_TO_VAULT_REVISIT, VaultRevisit, VAULT_REVISIT_FROM_VAULT),
    SCENE_EXITS_END,
};

// clang-format off
struct ActorTemplate gVault_ActorTemplates[] = {
    { ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(381), 0, PIXELS(342), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(280), 0, PIXELS(396), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(264), 0, PIXELS(150), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(244), 0, PIXELS(204), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(124), 0, PIXELS(196), FACING_SOUTHEAST, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(244), 0, PIXELS(300), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, (s32)gVault_BoyRoundTheWellRoute,
        PIXELS(132), 0, PIXELS(380), FACING_WEST, TALK_FACING_KEEP, 0 },
    { SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, (s32)gVault_GirlRoundTheWellRoute,
        PIXELS(132), 0, PIXELS(420), FACING_EAST, TALK_FACING_KEEP, 0 },
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(332), 0, PIXELS(412), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_PERSON_IN_PURPLE, FLAG_VAULT_THIEVES_ATTACKED, MOVEMENT_TYPE_STAND,
        PIXELS(92), 0, PIXELS(284), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(92), 0, PIXELS(296), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_DOG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(216), 0, PIXELS(392), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, FLAG_VAULT_SUSPECTS_MIND_READ, MOVEMENT_TYPE_STAND,
        PIXELS(152), 0, PIXELS(296), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, IF_FLAG_SET(FLAG_VAULT_SUSPECTS_MIND_READ), (s32)gVault_WomanWantingTheRoofFixedRoute,
        PIXELS(168), 0, PIXELS(312), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    // XXX (no evidence): what this ivy was for; it stands above the map's top
    // edge where no camera shows it, is not solid, and only the engine's passes
    // over every actor touch it (render probe vault-ivy).
    { SPRITE_IVY, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0 },
    { VENUS_DJINNI, FLAG_DJINNI(DJINNI_SAP), MOVEMENT_TYPE_STAND,
        PIXELS(344), 0, PIXELS(96), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ActorTemplate gVault_ActorTemplatesAfterThieves[] = {
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(381), 0, PIXELS(342), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(280), 0, PIXELS(396), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(264), 0, PIXELS(150), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(244), 0, PIXELS(204), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(124), 0, PIXELS(196), FACING_SOUTHEAST, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(244), 0, PIXELS(300), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, (s32)gVault_BoyRoundTheWellRoute,
        PIXELS(132), 0, PIXELS(380), FACING_WEST, TALK_FACING_KEEP, 0 },
    { SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, (s32)gVault_GirlRoundTheWellRoute,
        PIXELS(132), 0, PIXELS(420), FACING_EAST, TALK_FACING_KEEP, 0 },
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(332), 0, PIXELS(412), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_BLANK, FLAG_VAULT_THIEVES_ATTACKED, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(92), 0, PIXELS(296), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_DOG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(216), 0, PIXELS(440), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(152), 0, PIXELS(296), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, FLAG_VAULT_THIEVES_JAILED, (s32)gVault_WomanWantingTheRoofFixedRoute,
        PIXELS(168), 0, PIXELS(312), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_IVY, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0 },
    { VENUS_DJINNI, FLAG_DJINNI(DJINNI_SAP), MOVEMENT_TYPE_STAND,
        PIXELS(344), 0, PIXELS(96), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct Trigger gVault_Triggers[] = {
    { TRIGGER_STEP, MB_VAULT_SOUTH_GATE, CONDITION_ALWAYS, EVENT(Vault_IvanStaysBehind) },
    { TRIGGER_STEP, MB_VAULT_NORTH_GATE, CONDITION_ALWAYS, EVENT(Vault_IvanStaysBehind) },
    { TRIGGER_STEP, MB_VAULT_BRIDGE_END, CONDITION_ALWAYS, EVENT(Vault_RaisePathToBridge) },
    { TRIGGER_STEP, MB_VAULT_WEST_OF_BRIDGE, CONDITION_ALWAYS, EVENT(Vault_LowerPathUnderBridge) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), MB_VAULT_GEM_OWNERS_HOUSE_DOOR, CONDITION_ALWAYS, EVENT(Vault_EnterGemOwnersHouse) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), MB_VAULT_CHILDS_HOUSE_DOOR, CONDITION_ALWAYS, EVENT(Vault_EnterChildsHouse) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), MB_VAULT_INN_DOOR, CONDITION_ALWAYS, EVENT(Vault_EnterInn) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), MB_VAULT_ITEM_SHOP_DOOR, CONDITION_ALWAYS, EVENT(Vault_EnterItemShop) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), MB_VAULT_WEAPON_AND_ARMOR_SHOP_DOOR, CONDITION_ALWAYS, EVENT(Vault_EnterWeaponAndArmorShop) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), MB_VAULT_MAYORS_HOUSE_DOOR, CONDITION_ALWAYS, EVENT(Vault_EnterMayorsHouse) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), MB_VAULT_SANCTUM_DOORWAY, CONDITION_ALWAYS, EVENT(Vault_OpenDoorwayAndEnter) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), MB_VAULT_DOOR_TO_JAIL, CONDITION_ALWAYS, EVENT(Vault_EnterJail) },
    { TRIGGER_STEP, MB_VAULT_WEST_ENTRANCE, CONDITION_ALWAYS, EVENT(Vault_EnterPassage) },
    { TRIGGER_TALK, ACTOR_WELCOMING_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgVaultWelcomeToVaultTheTownAt) },
    { TRIGGER_TALK, ACTOR_ERUPTION_WITNESS, CONDITION_ALWAYS, EVENT(Vault_AskAboutEruption) },
    { TRIGGER_TALK, ACTOR_VILLAGER_ON_HAMMET, CONDITION_ALWAYS, MSGNO(MsgVaultMasterHammetAWealthyLandownerAnd) },
    { TRIGGER_TALK, ACTOR_WOMAN_ON_STEALING, CONDITION_ALWAYS, EVENT(Vault_TalkOfEruptionThefts) },
    { TRIGGER_TALK, ACTOR_OLD_MAN_ON_HAMMETS_SERVANT, CONDITION_ALWAYS, MSGNO(MsgVaultMasterHammetsServantCantReturnTo) },
    { TRIGGER_TALK, ACTOR_OLD_WOMAN_ON_THE_TRAVELERS, CONDITION_ALWAYS, EVENT(Vault_AskAboutFleeingTravelers) },
    { TRIGGER_TALK, ACTOR_BOY_ROUND_THE_WELL, CONDITION_ALWAYS, EVENT(Vault_PityIvan) },
    { TRIGGER_TALK, ACTOR_GIRL_ROUND_THE_WELL, CONDITION_ALWAYS, EVENT(Vault_TalkOfHammetsCaravan) },
    { TRIGGER_TALK, ACTOR_STRONGMAN_ON_WATCH, CONDITION_ALWAYS, EVENT(Vault_StrongmanOnWatchSuspectsOutsiders) },
    { TRIGGER_TALK, ACTOR_THIEF_IN_TOWN, CONDITION_ALWAYS, EVENT(Vault_ThiefDeniesTheft) },
    { TRIGGER_TALK, ACTOR_STRANDED_TRAVELER, CONDITION_ALWAYS, EVENT(Vault_AskAboutAdventure) },
    { TRIGGER_TALK, ACTOR_VAULT_DOG, CONDITION_ALWAYS, EVENT(Vault_TalkToDog) },
    { TRIGGER_TALK, ACTOR_YOUNG_MAN_FIXING_THE_ROOF, CONDITION_ALWAYS, MSGNO(MsgVaultAFieryRockBurntAHole) },
    { TRIGGER_TALK, ACTOR_WOMAN_WANTING_THE_ROOF_FIXED, CONDITION_ALWAYS, EVENT(Vault_LookForRoofer) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WELCOMING_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgVaultISupposeTheseChildrenCantGo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ERUPTION_WITNESS, CONDITION_ALWAYS, MSGNO(MsgVaultMtAlephsEruptionWasBeyondDescription) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_HAMMET, CONDITION_ALWAYS, MSGNO(MsgVaultWhyDoesAWealthyManLike) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_STEALING, CONDITION_ALWAYS, EVENT(Vault_ReadMindOnTheft) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_HAMMETS_SERVANT, CONDITION_ALWAYS, MSGNO(MsgVaultPoorIvanButTheRodMay) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_THE_TRAVELERS, CONDITION_ALWAYS, EVENT(Vault_ReadMindOnTravelers) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_ROUND_THE_WELL, CONDITION_ALWAYS, EVENT(Vault_ReadMindOnIvan) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_ROUND_THE_WELL, CONDITION_ALWAYS, EVENT(Vault_ReadMindOnWealth) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRONGMAN_ON_WATCH, CONDITION_ALWAYS, EVENT(Vault_ReadMindOnSuspects) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_THIEF_IN_TOWN, CONDITION_ALWAYS, MSGNO(MsgVaultHmphItLooksLikeTheyreOnto) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRANDED_TRAVELER, CONDITION_ALWAYS, MSGNO(MsgVaultIWasJustAboutToLeave) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VAULT_DOG, CONDITION_ALWAYS, EVENT(Vault_ReadDogMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_FIXING_THE_ROOF, CONDITION_ALWAYS, MSGNO(MsgVaultFixTheRoofWhyPayFor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_WANTING_THE_ROOF_FIXED, CONDITION_ALWAYS, EVENT(Vault_ReadWomanWantingTheRoofFixedMind) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), MB_VAULT_BELL, CONDITION_ALWAYS, EVENT(Vault_RingBell) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), MB_VAULT_WELL, CONDITION_ALWAYS, EVENT(Vault_PeerIntoWell) },
    { EXAMINE_TRIGGER(EXAMINE_WOODEN_BOX), MB_VAULT_WOODEN_BOX, FLAG_VAULT_BOX_NUT, FIND_ITEM(ITEM_NUT) },
    { TRIGGERS_END },
};

struct Trigger gVault_TriggersAfterThieves[] = {
    { TRIGGER_STEP, MB_VAULT_SOUTH_GATE, CONDITION_ALWAYS, EVENT(Vault_IvanStaysBehind) },
    { TRIGGER_STEP, MB_VAULT_NORTH_GATE, CONDITION_ALWAYS, EVENT(Vault_IvanStaysBehind) },
    { TRIGGER_STEP, MB_VAULT_BRIDGE_END, CONDITION_ALWAYS, EVENT(Vault_RaisePathToBridge) },
    { TRIGGER_STEP, MB_VAULT_WEST_OF_BRIDGE, CONDITION_ALWAYS, EVENT(Vault_LowerPathUnderBridge) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), MB_VAULT_GEM_OWNERS_HOUSE_DOOR, CONDITION_ALWAYS, EVENT(Vault_EnterGemOwnersHouse) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), MB_VAULT_CHILDS_HOUSE_DOOR, CONDITION_ALWAYS, EVENT(Vault_EnterChildsHouse) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), MB_VAULT_INN_DOOR, CONDITION_ALWAYS, EVENT(Vault_EnterInn) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), MB_VAULT_ITEM_SHOP_DOOR, CONDITION_ALWAYS, EVENT(Vault_EnterItemShop) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), MB_VAULT_WEAPON_AND_ARMOR_SHOP_DOOR, CONDITION_ALWAYS, EVENT(Vault_EnterWeaponAndArmorShop) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), MB_VAULT_MAYORS_HOUSE_DOOR, CONDITION_ALWAYS, EVENT(Vault_EnterMayorsHouse) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), MB_VAULT_SANCTUM_DOORWAY, CONDITION_ALWAYS, EVENT(Vault_OpenDoorwayAndEnter) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), MB_VAULT_DOOR_TO_JAIL, CONDITION_ALWAYS, EVENT(Vault_EnterJail) },
    { TRIGGER_STEP, MB_VAULT_WEST_ENTRANCE, CONDITION_ALWAYS, EVENT(Vault_EnterPassage) },
    { TRIGGER_TALK, ACTOR_WELCOMING_YOUNG_MAN, CONDITION_ALWAYS, EVENT(Vault_AskAboutLeavingAgain) },
    { TRIGGER_TALK, ACTOR_ERUPTION_WITNESS, CONDITION_ALWAYS, MSGNO(MsgVaultSmokeIsStillRisingFromMt) },
    { TRIGGER_TALK, ACTOR_VILLAGER_ON_HAMMET, CONDITION_ALWAYS, MSGNO(MsgVaultMasterHammetTheWealthyMerchantI) },
    { TRIGGER_TALK, ACTOR_WOMAN_ON_STEALING, CONDITION_ALWAYS, EVENT(Vault_PraiseThievesCapture) },
    { TRIGGER_TALK, ACTOR_OLD_MAN_ON_HAMMETS_SERVANT, CONDITION_ALWAYS, EVENT(Vault_TalkAboutIvan) },
    { TRIGGER_TALK, ACTOR_OLD_WOMAN_ON_THE_TRAVELERS, CONDITION_ALWAYS, EVENT(Vault_TalkOfDamagedRoads) },
    { TRIGGER_TALK, ACTOR_BOY_ROUND_THE_WELL, CONDITION_ALWAYS, EVENT(Vault_AskWhetherIvanWentToLunpa) },
    { TRIGGER_TALK, ACTOR_GIRL_ROUND_THE_WELL, CONDITION_ALWAYS, EVENT(Vault_ThankForCapturingThieves) },
    { TRIGGER_TALK, ACTOR_STRONGMAN_ON_WATCH, CONDITION_ALWAYS, EVENT(Vault_AskAboutPrisoners) },
    { TRIGGER_TALK, ACTOR_STRANDED_TRAVELER, CONDITION_ALWAYS, EVENT(Vault_AskWhoCapturedThieves) },
    { TRIGGER_TALK, ACTOR_VAULT_DOG, CONDITION_ALWAYS, EVENT(Vault_TalkToDogAfterCapture) },
    { TRIGGER_TALK, ACTOR_YOUNG_MAN_FIXING_THE_ROOF, CONDITION_ALWAYS, EVENT(Vault_ThankBeforeDeparture) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WELCOMING_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgVaultYouveMadeOurTownPeacefulAt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ERUPTION_WITNESS, CONDITION_ALWAYS, MSGNO(MsgVaultNowMtAlephHasCalmedDown) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_HAMMET, CONDITION_ALWAYS, MSGNO(MsgVaultOfAllThePlacesWhyWould) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_STEALING, CONDITION_ALWAYS, EVENT(Vault_ReadMindOnTheft) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_HAMMETS_SERVANT, CONDITION_ALWAYS, MSGNO(MsgVaultNobodyGetsIntoLunpaExceptFor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_THE_TRAVELERS, CONDITION_ALWAYS, EVENT(Vault_ReadMindOnTravelers) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_ROUND_THE_WELL, CONDITION_ALWAYS, EVENT(Vault_ReadMindOnIvan) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_ROUND_THE_WELL, CONDITION_ALWAYS, EVENT(Vault_ReadMindOnWealth) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRONGMAN_ON_WATCH, CONDITION_ALWAYS, EVENT(Vault_ReadMindOnSuspects) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRANDED_TRAVELER, CONDITION_ALWAYS, MSGNO(MsgVaultThereIsAlwaysAFightWhen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VAULT_DOG, CONDITION_ALWAYS, EVENT(Vault_ReadDogMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_FIXING_THE_ROOF, CONDITION_ALWAYS, MSGNO(MsgVaultSniffAhTheSmellOfCooking) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), MB_VAULT_BELL, CONDITION_ALWAYS, EVENT(Vault_RingBell) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), MB_VAULT_WELL, CONDITION_ALWAYS, EVENT(Vault_PeerIntoWell) },
    { USE_ITEM_ON_ACTOR(ITEM_BONE), ACTOR_VAULT_DOG, FLAG_VAULT_DOG_SHOWN_PASSAGE, EVENT(Vault_GiveBoneToDog) },
    { EXAMINE_TRIGGER(EXAMINE_WOODEN_BOX), MB_VAULT_WOODEN_BOX, FLAG_VAULT_BOX_NUT, FIND_ITEM(ITEM_NUT) },
    { TRIGGERS_END },
};
// clang-format on

// Each door's opening, two frames of map cells, named for the room it opens
// into (Vault_EnterGemOwnersHouse and the rest).
u16 gVault_GemOwnersHouseDoorCells[] = {
    MAPGRID_ANIMATION_STEP(35, 52, 2, 2, 4),
    MAPGRID_ANIMATION_STEP(33, 52, 2, 2, 4),
    MAPGRID_ANIMATION_STEPS_END,
};

u16 gVault_ChildsHouseDoorCells[] = {
    MAPGRID_ANIMATION_STEP(35, 50, 2, 2, 4),
    MAPGRID_ANIMATION_STEP(33, 50, 2, 2, 4),
    MAPGRID_ANIMATION_STEPS_END,
};

u16 gVault_InnDoorCells[] = {
    MAPGRID_ANIMATION_STEP(35, 48, 2, 2, 4),
    MAPGRID_ANIMATION_STEP(33, 48, 2, 2, 4),
    MAPGRID_ANIMATION_STEPS_END,
};

u16 gVault_ItemShopDoorCells[] = {
    MAPGRID_ANIMATION_STEP(42, 48, 2, 2, 4),
    MAPGRID_ANIMATION_STEP(40, 48, 2, 2, 4),
    MAPGRID_ANIMATION_STEPS_END,
};

u16 gVault_WeaponAndArmorShopDoorCells[] = {
    MAPGRID_ANIMATION_STEP(42, 46, 2, 2, 4),
    MAPGRID_ANIMATION_STEP(40, 46, 2, 2, 4),
    MAPGRID_ANIMATION_STEPS_END,
};

u16 gVault_MayorsHouseDoorCells[] = {
    MAPGRID_ANIMATION_STEP(35, 46, 2, 2, 4),
    MAPGRID_ANIMATION_STEP(33, 46, 2, 2, 4),
    MAPGRID_ANIMATION_STEPS_END,
};

u16 gVault_JailDoorCells[] = {
    MAPGRID_ANIMATION_STEP(35, 33, 2, 2, 4),
    MAPGRID_ANIMATION_STEP(33, 33, 2, 2, 4),
    MAPGRID_ANIMATION_STEPS_END,
};

s32 gVault_DriftScriptA[] = {
    SC_WAIT(1),
    SC_LOOP(118, SC_TOP),
    SC_RELEASE,
};

s32 gVault_DriftScriptB[] = {
    SC_WAIT(1),
    SC_LOOP(38, SC_TOP),
    SC_RELEASE,
};
