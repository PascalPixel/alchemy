/// @file
/// Vault (SceneId_Vault), "the town at the heart of Angara", as the party first
/// finds it, just after Mt. Aleph's eruption: the first town on the journey,
/// where Vale's villagers send the party (vale_gate.c). In the chaos of the
/// eruption thieves struck: Master Hammet, a wealthy landowner and merchant
/// from Kalay, had his precious rod, the Shaman's Rod, stolen, and his servant
/// Ivan was left at the mayor's and cannot go home until he recovers it. This
/// scene is the town's streets, with the townspeople's talk, Ivan's comings and
/// goings, the bell and the dog; the houses, the inn and its loft, where the
/// story plays out, are vault_interiors.c. Scene_GetPlacements and
/// Scene_GetEvents choose by FLAG_VAULT_THIEVES_JAILED.
///
/// - While the rod is missing, the townspeople talk of the eruption, of the
///   stolen rod, and of strange travelers who left in a hurry: two women and
///   four men, the young girl and the old man among them unhappy to be there.
///   Read with Mind Read, they blame the three strangers staying at the inn.
///   One of the three loiters in town, denying everything, until the party
///   has confronted them in the loft (FLAG_VAULT_THIEVES_ATTACKED removes
///   him). A young man complains of the hole a fiery rock burnt through a roof;
///   once the suspects' minds have been read (FLAG_VAULT_SUSPECTS_MIND_READ)
///   he is missing and a woman walks about looking for him, for he went up
///   into the inn's loft to fix its roof and was tied up there.
/// - Once Ivan has joined the search (FLAG_VAULT_IVAN_JOINED_ROD_SEARCH),
///   walking out of either gate (triggers 20 and 21, before
///   VAULT_TO_WORLD_MAP_SOUTH and VAULT_TO_WORLD_MAP_NORTH) has him stay behind
///   to wait in Vault (Event_IvanStaysBehind): he hands back any Venus Djinni
///   of Isaac's he holds and leaves the party. Coming back in by a gate
///   (VAULT_FROM_WORLD_MAP_SOUTH, or ENTRANCE_DEFAULT or
///   VAULT_NORTH_GATE_UNUSED at the north gate) or out of the Vault Sanctum
///   (VAULT_FROM_SANCTUM), Scene_Initialize has him greet the two and rejoin.
/// - After the thieves are jailed, the townspeople thank the party, Ivan has
///   gone out to Lunpa to rescue Hammet, and the dog, given a Bone
///   (Event_GiveBoneToDog, FLAG_VAULT_DOG_SHOWN_PASSAGE), runs to a spot and
///   jumps there three times. Its thoughts then hint at a passage leading
///   underground, which opens only in the town's later scene
///   (vault_revisit.c).
///
/// Sap, a Venus Djinni, hides near the bell. Searching the bell rings it, and
/// the first ring since the party came in by a gate
/// (FLAG_VAULT_DJINNI_CALLED_BY_BELL) calls Sap out to stand beside it; no
/// event here lets the party talk to Sap, which joins only in the town's
/// later scene. The well rumbles with water and a wooden box holds a Nut.
/// The doors lead into the houses, shops and inn (VaultInteriors, through the
/// VAULT_TO_VAULT_INTERIORS_ exits), the jail (VAULT_TO_VAULT_JAIL) and the
/// Vault Sanctum (VAULT_TO_SANCTUM), and the two gates to the world map. Once
/// the party has reached Kalay (FLAG_REACHED_KALAY), Scene_Initialize sends it
/// straight on to the later scene (VAULT_TO_VAULT_REVISIT,
/// SceneId_VaultRevisit).

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

static void Vault_ShowStandingActorMessage(s32 actor);
static void Vault_ShowMessageAndWait(s32 actor, s32 delay);
static void Vault_FaceActorAndWait(s32 actor, s32 other, s32 delay);

#include "common/effect_create.h"
#include "common/effect_spawn.h"

#include "common/vault_nearby.h"

// The woman wanting the roof fixed walks about looking for the man who
// should be fixing it and stops to face
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

const struct SceneEntrance *Scene_GetEntrances(void) {
  return gVault_Entrances;
}

const struct SceneRegion *Scene_GetRegions(void) { return NULL; }

const u32 *Scene_GetExits(void) { return gVault_Exits; }

const struct ScenePlacement *Scene_GetPlacements(void) {
  if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) != 0)
    return gVault_PlacementsAfterThieves;
  return gVault_Placements;
}

/// Counts the Venus Djinn that must leave Ivan before he leaves the party.
static int Vault_GetIvanVenusDjinnCount(void) {
  return UnitGet(IVAN)->djinnOwnedCounts[ELEMENT_VENUS];
}

#include "common/vault_djinn.h"

/// Leaving Vault by either gate (triggers 20 and 21, before
/// VAULT_TO_WORLD_MAP_SOUTH and VAULT_TO_WORLD_MAP_NORTH): while Ivan is
/// helping search for the rod and the thieves are free, he walks out to the
/// gate, says he is still worried about the rod and will wait in Vault, leaves
/// any Venus Djinni with Isaac and leaves the party.
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
  Event_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT | IVAN,
                           0, 20);
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

/// In Vault, Isaac peers into the well and hears water rumbling down there.
static void Event_PeerIntoWell(void) {
  Engine_EventBegin();
  Engine_MessageShowCentered(MsgFieldPeeredWell, 1);
  Engine_MessageShowCentered(MsgVaultCanHearWaterRumblingDown, 1);
  Engine_EventEnd();
}

const struct SceneEvent *Scene_GetEvents(void) {
  if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) != 0)
    return gVault_EventsAfterThieves;
  return gVault_Events;
}

/// Asks the current question and selects its ordinary or alternate reply.
static inline void Vault_AskYesNoAndShowReply(s32 speaker) {
  Engine_EventOpenMessage(speaker, 0);
  if (Engine_EventChooseYesNo(0, 0) != 0) {
    struct EventWork *eventWork = gEventWork;

    ++eventWork->message;
  }
  Engine_EventShowMessage(speaker, 0);
}

/// In Vault while the rod is missing, a townsperson asks about the eruption.
/// "Wasn't the eruption of Mt. Aleph incredible?" Yes: too terrified to sleep
/// for days. No: aren't you courageous!
static void Event_AskAboutEruption(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultWasntEruptionMtAlephIncredible);
  Vault_FaceActorAndWait(ACTOR_ERUPTION_WITNESS, ACTOR_PARTY_LEADER, 2);
  Event_OpenMessage(ACTOR_ERUPTION_WITNESS, 0);
  if (Engine_EventChooseYesNo(0, 0) != 0) {
    gEventWork->message++;
  }
  Event_ShowMessage(ACTOR_ERUPTION_WITNESS, 0);
  Actor_FaceDirection(ACTOR_ERUPTION_WITNESS, FACING_SOUTH + FACING_STEP, 0);
  Engine_EventEnd();
}

/// In Vault while the rod is missing, a townsperson says travelers left town
/// in a big hurry: did the party see them? Yes: two women and four men, the
/// girl and old man unhappy to be there. No: maybe they left earlier.
static void Event_AskAboutFleeingTravelers(void) {

  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultThoseTravelersLeftInBig);
  Engine_ActorSetAnimation(ACTOR_OLD_WOMAN_ON_THE_TRAVELERS, ANIM_STAND);
  Vault_FaceActorAndWait(ACTOR_OLD_WOMAN_ON_THE_TRAVELERS, ACTOR_PARTY_LEADER,
                         2);
  Vault_AskYesNoAndShowReply(ACTOR_OLD_WOMAN_ON_THE_TRAVELERS);
  Engine_EventEnd();
}

/// In Vault, one of the thieves loitering in town asks whether the party
/// accuses them; either answer gets a denial.
/// "Are you accusing us of stealing Hammet's treasured Shaman's Rod?"
static void Event_ThiefDeniesTheft(void) {

  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultAccusingUsStealingHammetsTreasured);
  Vault_FaceActorAndWait(ACTOR_THIEF_IN_TOWN, ACTOR_PARTY_LEADER, 2);
  Vault_AskYesNoAndShowReply(ACTOR_THIEF_IN_TOWN);
  Engine_EventEnd();
}

/// A stranded traveler asks whether the party is off on an adventure.
/// The traveler was stranded by the eruption, and asks while the rod is
/// missing.
/// "Off on an adventure, are you?"
/// Yes: impressive for youngsters. No: see the Sanctum while you're here.
static void Event_AskAboutAdventure(void) {

  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultOffOnAdventure);
  Vault_FaceActorAndWait(ACTOR_STRANDED_TRAVELER, ACTOR_PARTY_LEADER, 2);
  Vault_AskYesNoAndShowReply(ACTOR_STRANDED_TRAVELER);
  Engine_EventEnd();
}

/// A townsperson says some folks got robbed in the chaos of the eruption.
/// She says it while the rod is missing.
static void Event_TalkOfEruptionThefts(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultGuessFolksGot);
  Vault_ShowStandingActorMessage(ACTOR_WOMAN_ON_STEALING);
  Engine_EventEnd();
}

/// In Vault while the rod is missing, the strongman on watch: nobody steals on
/// his watch, the thieves must be from out of town.
static void Event_StrongmanOnWatchSuspectsOutsiders(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultNobodysStealingAnything);
  Vault_ShowStandingActorMessage(ACTOR_STRONGMAN_ON_WATCH);
  Engine_EventEnd();
}

/// In Vault while the rod is missing, the dog stops and barks.
static void Event_TalkToDog(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultRuffRrruff);
  Engine_ActorSetAnimation(ACTOR_VAULT_DOG, 0);
  Vault_FaceActorAndWait(ACTOR_VAULT_DOG, ACTOR_PARTY_LEADER, 2);
  Engine_EventShowMessage(ACTOR_VAULT_DOG, 0);
  Engine_EventEnd();
}

/// In Vault while the rod is missing, the boy round the well stops: poor Ivan
/// was left at the mayor's to look for the stolen rod.
static void Event_PityIvan(void) {
  struct FieldActor *actor = Object_GetById(ACTOR_BOY_ROUND_THE_WELL);
  s16 savedFacing = actor->facing;

  actor->timer |= 2;
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultPoorGuyLeft);
  Engine_ActorSetAnimation(ACTOR_BOY_ROUND_THE_WELL, 0);
  Vault_FaceActorAndWait(ACTOR_BOY_ROUND_THE_WELL, ACTOR_PARTY_LEADER, 2);
  Vault_ShowMessageAndWait(ACTOR_BOY_ROUND_THE_WELL, 10);
  actor->facing = savedFacing;
  Engine_TaskWait(1);
  Engine_EventEnd();
  actor->timer &= 1;
}

/// In Vault while the rod is missing, the girl round the well stops: Hammet's
/// caravan left as soon as the eruption stopped.
static void Event_TalkOfHammetsCaravan(void) {
  struct FieldActor *actor = Object_GetById(ACTOR_GIRL_ROUND_THE_WELL);
  s16 savedFacing = actor->facing;

  actor->timer |= 2;
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultMasterHammetsCaravan);
  Engine_ActorSetAnimation(ACTOR_GIRL_ROUND_THE_WELL, 0);
  Vault_FaceActorAndWait(ACTOR_GIRL_ROUND_THE_WELL, ACTOR_PARTY_LEADER, 2);
  Vault_ShowMessageAndWait(ACTOR_GIRL_ROUND_THE_WELL, 10);
  actor->facing = savedFacing;
  Engine_TaskWait(1);
  Engine_EventEnd();
  actor->timer &= 1;
}

/// In Vault, once the suspects' minds are read, the woman wanting
/// the roof fixed: the man who should be fixing it is probably in back
/// slacking off.
static void Event_LookForRoofer(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultFixingRoofCant);
  Vault_FaceActorAndWait(ACTOR_WOMAN_WANTING_THE_ROOF_FIXED, ACTOR_PARTY_LEADER,
                         2);
  Engine_ActorShowEmote(ACTOR_WOMAN_WANTING_THE_ROOF_FIXED, 0x103, 0);
  Engine_EventWait(30);
  Engine_EventOpenMessage(ACTOR_WOMAN_WANTING_THE_ROOF_FIXED, 0);
  Engine_EventEnd();
}

/// Stops an actor, faces the leader and shows the current message.
static void Vault_ShowStandingActorMessage(s32 actor) {
  Engine_EventBegin();
  Engine_ActorSetAnimation(actor, ANIM_STAND);
  Vault_FaceActorAndWait(actor, ACTOR_PARTY_LEADER, 2);
  Engine_EventShowMessage(actor, 0);
  Engine_EventEnd();
}

#include "common/vault_talk.h"

/// The welcoming young man asks whether the party is leaving again.
/// He asks after the thieves are jailed. "So, you're hitting the road again?"
static void Event_AskAboutLeavingAgain(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultYoureHittingRoadAgain);
  Vault_FaceActorAndWait(ACTOR_WELCOMING_YOUNG_MAN, ACTOR_PARTY_LEADER, 2);
  Vault_AskYesNoAndShowReply(ACTOR_WELCOMING_YOUNG_MAN);
  Engine_EventEnd();
}

/// In Vault after the thieves are jailed, a townsperson praises their capture.
/// "It's great that you caught those thieves!"
static void Event_PraiseThievesCapture(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultGreatCaughtThieves);
  Vault_ShowStandingActorMessage(ACTOR_WOMAN_ON_STEALING);
  Engine_EventEnd();
}

/// An old man talks about Ivan, Hammet's servant.
/// After the thieves are jailed, he says Ivan went out to Lunpa, or, with Ivan
/// in the party, that rescuing Hammet must have been impossible.
static void Event_TalkAboutIvan(void) {
  struct EventWork *workspace;
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultTalkingAboutHammetsServantIvan);
  if (Engine_GameFlagIsSet(FLAG_IN_PARTY(IVAN)) != 0) {
    workspace = gEventWork;
    ++workspace->message;
  }
  Vault_ShowStandingActorMessage(ACTOR_OLD_MAN_ON_HAMMETS_SERVANT);
  Engine_EventEnd();
}

/// In Vault after the thieves are jailed, a townsperson says the eruption
/// devastated the roads; once the southern bridge is repaired, you can head
/// toward Kalay.
static void Event_TalkOfDamagedRoads(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultEruptionDevastatedRoads);
  Vault_ShowStandingActorMessage(ACTOR_OLD_WOMAN_ON_THE_TRAVELERS);
  Engine_EventEnd();
}

/// The boy round the well asks after Ivan.
/// After the thieves are jailed, he hopes Ivan didn't go to Lunpa alone, or,
/// with Ivan in the party, asks when he joined up with Isaac.
static void Event_AskWhetherIvanWentToLunpa(void) {
  struct FieldActor *actor = Object_GetById(ACTOR_BOY_ROUND_THE_WELL);
  s16 savedFacing = actor->facing;

  actor->timer |= 2;
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultDidntGoLunpa);
  if (Engine_GameFlagIsSet(FLAG_IN_PARTY(IVAN)) != 0)
    ++gEventWork->message;
  Engine_ActorSetAnimation(ACTOR_BOY_ROUND_THE_WELL, 0);
  Vault_FaceActorAndWait(ACTOR_BOY_ROUND_THE_WELL, ACTOR_PARTY_LEADER, 2);
  Vault_ShowMessageAndWait(ACTOR_BOY_ROUND_THE_WELL, 10);
  actor->facing = savedFacing;
  Engine_TaskWait(1);
  Engine_EventEnd();
  actor->timer &= 1;
}

/// In Vault after the thieves are jailed, the girl round the well: everyone is
/// grateful the party captured the thieves.
static void Event_ThankForCapturingThieves(void) {
  struct FieldActor *actor = Object_GetById(ACTOR_GIRL_ROUND_THE_WELL);
  s16 savedFacing = actor->facing;

  actor->timer |= 2;
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultEveryoneGratefulCaptured);
  Engine_ActorSetAnimation(ACTOR_GIRL_ROUND_THE_WELL, 0);
  Vault_FaceActorAndWait(ACTOR_GIRL_ROUND_THE_WELL, ACTOR_PARTY_LEADER, 2);
  Vault_ShowMessageAndWait(ACTOR_GIRL_ROUND_THE_WELL, 10);
  actor->facing = savedFacing;
  Engine_TaskWait(1);
  Engine_EventEnd();
  actor->timer &= 1;
}

/// In Vault after the thieves are jailed, the strongman on watch asks about the
/// prisoners. "Those men you captured... They're in jail now?" Yes: the
/// prison hadn't been used in a long time. No: they're upset; maybe go see
/// them.
static void Event_AskAboutPrisoners(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultThoseMenCapturedTheyreIn);
  Engine_ActorSetAnimation(ACTOR_STRONGMAN_ON_WATCH, ANIM_STAND);
  Vault_FaceActorAndWait(ACTOR_STRONGMAN_ON_WATCH, ACTOR_PARTY_LEADER, 2);
  Vault_AskYesNoAndShowReply(ACTOR_STRONGMAN_ON_WATCH);
  Engine_EventEnd();
}

/// In Vault after the thieves are jailed, the traveler asks who captured the
/// thieves; either answer is praised.
/// "Are you the ones who captured the thieves?"
static void Event_AskWhoCapturedThieves(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultOnesWhoCapturedThieves);
  Vault_FaceActorAndWait(ACTOR_STRANDED_TRAVELER, ACTOR_PARTY_LEADER, 2);
  Vault_AskYesNoAndShowReply(ACTOR_STRANDED_TRAVELER);
  Engine_EventEnd();
}

/// In Vault after the thieves are jailed, the dog barks; carrying a Bone
/// before it has shown the passage lets the party give it one.
static void Event_TalkToDogAfterCapture(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultRuffRrruff2);
  Engine_ActorSetAnimation(ACTOR_VAULT_DOG, 0);
  Vault_FaceActorAndWait(ACTOR_VAULT_DOG, ACTOR_PARTY_LEADER, 2);
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

/// The young man fixing the roof thanks the party before it leaves.
/// The party freed him, and he says so after the thieves are jailed.
/// "Thank you for the other day.
/// Are you leaving town already?"
static void Event_ThankBeforeDeparture(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultThankForOtherDayLeaving);
  Vault_FaceActorAndWait(ACTOR_YOUNG_MAN_FIXING_THE_ROOF, ACTOR_PARTY_LEADER,
                         2);
  Engine_ActorSetAnimationAndWait(ACTOR_YOUNG_MAN_FIXING_THE_ROOF, ANIM_NOD);
  Engine_EventWait(20);
  Engine_EventShowMessage(ACTOR_YOUNG_MAN_FIXING_THE_ROOF, 0);
  Engine_EventEnd();
}

/// In Vault, Mind Read on a townsperson: stealing during an eruption is
/// unforgivable, or how should the prisoners be punished?
static void Event_ReadMindOnTheft(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) == 0)
    Engine_EventSetMessage(MsgVaultManShouldStealFromAnother);
  else
    Engine_EventSetMessage(MsgVaultHowPunishPrisoners);
  Vault_ShowThought(ACTOR_WOMAN_ON_STEALING);
  Engine_EventEnd();
}

/// In Vault, Mind Read on a townsperson: the strange travelers headed for the
/// northeast of the Goma Range, or the only way out is the cave in the Goma
/// Range.
static void Event_ReadMindOnTravelers(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) == 0) {
    Engine_EventSetMessage(MsgVaultGroupTravelersWasStrangeBunch);
  } else {
    Engine_EventSetMessage(MsgVaultWithRoadOutOnlyWay);
  }
  Vault_ShowThought(ACTOR_OLD_WOMAN_ON_THE_TRAVELERS);
  Engine_EventEnd();
}

/// In Vault, Mind Read on the boy round the well: the three creeps at the inn
/// stole the rod, or where did Ivan go (when did they join him, with Ivan in
/// the party)?
static void Event_ReadMindOnIvan(void) {
  Object_GetById(ACTOR_BOY_ROUND_THE_WELL)->timer |= 2;
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) == 0) {
    Engine_EventSetMessage(MsgVaultBetWasThoseThreeCreeps);
  } else {
    Engine_EventSetMessage(MsgVaultWhereDidIvanGoBy);
    if (Engine_GameFlagIsSet(FLAG_IN_PARTY(IVAN)) != 0)
      ++gEventWork->message;
  }
  Vault_ShowThought(ACTOR_BOY_ROUND_THE_WELL);
  Engine_EventEnd();
  Object_GetById(ACTOR_BOY_ROUND_THE_WELL)->timer &= 1;
}

/// In Vault, Mind Read on the girl round the well: riches can't save you from a
/// volcano, or the party must be stronger than they look.
static void Event_ReadMindOnWealth(void) {
  Object_GetById(ACTOR_GIRL_ROUND_THE_WELL)->timer |= 2;
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) == 0)
    Engine_EventSetMessage(MsgVaultSupposeDoesntMatterHowRich);
  else
    Engine_EventSetMessage(MsgVaultMustStrongerThanLookHave);
  Vault_ShowThought(ACTOR_GIRL_ROUND_THE_WELL);
  Engine_EventEnd();
  Object_GetById(ACTOR_GIRL_ROUND_THE_WELL)->timer &= 1;
}

/// In Vault, Mind Read on the strongman on watch: everyone knows the three at
/// the inn stole the rod, or he feels much better with them behind bars.
static void Event_ReadMindOnSuspects(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) == 0)
    Engine_EventSetMessage(MsgVaultEveryoneKnowsThoseThreeAt);
  else
    Engine_EventSetMessage(MsgVaultFeelMuchBetter);
  Vault_ShowThought(ACTOR_STRONGMAN_ON_WATCH);
  Engine_EventEnd();
}

/// In Vault, Mind Read on the dog: humans value money over their lives; after
/// the thieves are jailed, bring a bone for something useful, and once fed, a
/// careful search will reveal a passage leading underground.
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

/// In Vault, Mind Read on the woman wanting the roof fixed: that sneak is
/// wasting time instead of fixing the roof.
static void Event_ReadWomanWantingTheRoofFixedMind(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultSneakWastingTime);
  Vault_ShowThought(ACTOR_WOMAN_WANTING_THE_ROOF_FIXED);
  Engine_EventEnd();
}

#include "common/vault_toggle.h"

/// Walks to the exit threshold and requests the room's transition.
static void Vault_WalkThroughExit(s32 x, s32 y, s32 continuation) {
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
  Actor_WalkTo(ACTOR_PARTY_LEADER, x, y);
  gEventWork->transitionFrames = 16;
  Engine_EventRequestExit(continuation);
}

/// The door of the southeastern house opens and the party walks in.
/// It's VAULT_TO_VAULT_INTERIORS_EAST_SOUTHEAST, into the house where a man
/// keeps a sparkling rock the volcano threw.
static void Event_EnterSoutheastHouse(void) {
  Engine_AudioPlayCue(SE_DOOR_OPEN);
  Engine_MapAnimateCells(gVault_SoutheastHouseDoorCells, 56, 19);
  Vault_WalkThroughExit(408, 320, VAULT_TO_VAULT_INTERIORS_EAST_SOUTHEAST);
}

/// The door of a child's home opens and the party walks in.
/// It's VAULT_TO_VAULT_INTERIORS_SOUTHEAST, into the house east of the town's
/// center.
static void Event_EnterCentralEastHouse(void) {
  Engine_AudioPlayCue(SE_DOOR_OPEN);
  Engine_MapAnimateCells(gVault_CentralEastHouseDoorCells, 50, 18);
  Vault_WalkThroughExit(312, 304, VAULT_TO_VAULT_INTERIORS_SOUTHEAST);
}

/// The inn's door opens and the party walks in.
/// It's VAULT_TO_VAULT_INTERIORS_SOUTHWEST, into the inn where the three
/// suspects stay.
static void Event_EnterInn(void) {
  Engine_AudioPlayCue(SE_DOOR_OPEN);
  Engine_MapAnimateCells(gVault_InnDoorCells, 44, 17);
  Vault_WalkThroughExit(216, 288, VAULT_TO_VAULT_INTERIORS_SOUTHWEST);
}

/// In Vault, the door VAULT_TO_VAULT_INTERIORS_EAST opens into the item shop,
/// an old couple's home too; the leader walks in behind the doorway's
/// foreground tiles.
static void Event_EnterItemShop(void) {
  struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);
  struct AnimationObject *sprite = actor->sprite;

  Engine_AudioPlayCue(SE_DOOR_OPEN);
  Engine_MapAnimateCells(gVault_ItemShopDoorCells, 54, 13);
  Map_CopyCellAttributes(33, 20, 1, 3, 23, 12);
  actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  sprite->image.priority = 3;
  Vault_WalkThroughExit(376, 224, VAULT_TO_VAULT_INTERIORS_EAST);
}

/// The weapon and armor shop's door opens and the party walks in.
/// It's VAULT_TO_VAULT_INTERIORS_NORTH_NORTHEAST, entered as the item shop is.
static void Event_EnterWeaponAndArmorShop(void) {
  struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);
  struct AnimationObject *sprite = actor->sprite;

  Engine_AudioPlayCue(SE_DOOR_OPEN);
  Engine_MapAnimateCells(gVault_WeaponAndArmorShopDoorCells, 49, 10);
  Map_CopyCellAttributes(33, 20, 1, 3, 18, 10);
  actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  sprite->image.priority = 3;
  Vault_WalkThroughExit(296, 176, VAULT_TO_VAULT_INTERIORS_NORTH_NORTHEAST);
}

/// In Vault, the door VAULT_TO_VAULT_INTERIORS_NORTHWEST opens into the
/// mayor's house, where Ivan waits.
static void Event_EnterMayorsHouse(void) {
  Engine_AudioPlayCue(SE_DOOR_OPEN);
  Engine_MapAnimateCells(gVault_MayorsHouseDoorCells, 38, 6);
  Vault_WalkThroughExit(120, 144, VAULT_TO_VAULT_INTERIORS_NORTHWEST);
}

#include "common/vault_scene_cells.h"

/// In Vault, the jail door opens and the party walks in, drawn behind the
/// doorway (VAULT_TO_VAULT_JAIL, the jail's VAULT_JAIL_FROM_VAULT).
static void Event_EnterJail(void) {
  struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);
  struct AnimationObject *sprite = actor->sprite;

  Engine_AudioPlayCue(SE_DOOR_OPEN);
  Engine_MapAnimateCells(gVault_JailDoorCells, 35, 9);
  Map_CopyCellAttributes(33, 20, 1, 3, 4, 10);
  actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  sprite->image.priority = 3;
  Vault_WalkThroughExit(72, 160, VAULT_TO_VAULT_JAIL);
}

/// In Vault, the party walks through a doorless entrance into the room with
/// the crate (VAULT_TO_VAULT_INTERIORS_WEST).
static void Event_EnterPassage(void) {
  Engine_AudioPlayCue(SE_MAP_EXIT);
  Vault_WalkThroughExit(152, 264, VAULT_TO_VAULT_INTERIORS_WEST);
}

/// Moves an actor through a jump and waits up to sixty frames for it to settle.
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

/// In Vault, Isaac rings the bell; the first time since the party came in by a
/// gate, Sap, the Venus Djinni hiding nearby, jumps out and comes to the bell
/// (FLAG_VAULT_DJINNI_CALLED_BY_BELL). No event here lets the party talk to
/// it; it joins in the town's later scene (vault_revisit.c).
static void Event_RingBell(void) {
  Engine_EventBegin();
  Event_PlayCue(SE_100);
  Engine_EventWait(40);
  if (GameFlag_IsSet(FLAG_VAULT_DJINNI_CALLED_BY_BELL) == 0) {
    Actor_SetAttachedEffect(ACTOR_SAP, 0x102);
    Engine_ActorJump(ACTOR_SAP, 4, 0);
    Engine_EventWait(12);
    Engine_ActorJump(ACTOR_SAP, 4, 0);
    Engine_EventWait(20);
    Vault_JumpActorToAndWait(ACTOR_SAP, 392, 104, PIXELS(7));
    Engine_EventWait(20);
    Actor_WalkToAndWait(ACTOR_SAP, 408, 104);
    Actor_WalkToAndWait(ACTOR_SAP, 408, 120);
    GameFlag_Set(FLAG_VAULT_DJINNI_CALLED_BY_BELL);
  }
  Engine_EventEnd();
}

/// Given a Bone, the dog runs to the hidden passage and jumps there.
/// It does so after the thieves are jailed, jumping three times
/// (FLAG_VAULT_DOG_SHOWN_PASSAGE).
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

/// Arriving in Vault: restores the dog's and Sap's places, sets the two
/// children and the woman wanting the roof fixed stopping near the leader,
/// and, while Ivan waits in Vault for the party, has him greet the two and
/// rejoin.
s32 Scene_Initialize(void) {
  s32 suspectsMindRead;
  struct FieldActor *actor;
  s32 lowBit;
  s32 zero;

  // Once the party has reached Kalay (FLAG_REACHED_KALAY), the town is its
  // later scene, SceneId_VaultRevisit (vault_revisit.c).
  if (Engine_GameFlagIsSet(FLAG_REACHED_KALAY) != 0) {
    Engine_EventRequestExit(VAULT_TO_VAULT_REVISIT);
  }
  // Restore the passage cells and the two children's proximity callbacks.
  if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_00) != 0) {
    Map_CopyCellAttributes(55, 26, 4, 2, 23, 26);
  }
  OverlayObject_CreateConfiguredObject(PIXELS(128), 0, PIXELS(420), 223);
  lowBit = 1;
  zero = 0;
  actor = Object_GetById(ACTOR_BOY_ROUND_THE_WELL);
  actor->timer = lowBit;
#ifdef UBFIX
  actor->update = Vault_UpdateWalkingVillager;
#else
  actor->update = (void (*)(union FieldObject *))Vault_UpdateWalkingVillager;
#endif
  actor = Object_GetById(ACTOR_GIRL_ROUND_THE_WELL);
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
      actor = Object_GetById(ACTOR_WOMAN_WANTING_THE_ROOF_FIXED);
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
  if (gGameState.entrance <= VAULT_NORTH_GATE_UNUSED) {
    if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
      Engine_GameFlagClear(FLAG_VAULT_DJINNI_CALLED_BY_BELL);
      if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) == 0) {
        if (Engine_GameFlagIsSet(FLAG_VAULT_IVAN_JOINED_ROD_SEARCH) != 0) {
          Engine_EventBegin();
          actor = Object_GetById(ACTOR_PARTY_LEADER);
          if (actor != 0) {
            Engine_ActorSetPosition(IVAN, actor->x.fixed, actor->z.fixed);
          }
          if (gGameState.entrance == VAULT_FROM_WORLD_MAP_SOUTH) {
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
    Actor_SetPosition(ACTOR_SAP, PIXELS(408), PIXELS(120));
  }
  // The same welcome out of the Vault Sanctum while Ivan is out of the party.
  if (gGameState.entrance == VAULT_FROM_SANCTUM) {
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
    if (gGameState.entrance == VAULT_FROM_VAULT_INTERIORS_WEST) {
      if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) != 0) {
        Engine_ActorSetPosition(ACTOR_YOUNG_MAN_FIXING_THE_ROOF, 0, 0);
      }
    }
  }
  return 0;
}

#include "common/object_distance.h"
#include "common/vault_range.h"

static void Vault_ShowMessageAndWait(s32 actor, s32 delay) {
  Engine_EventShowMessage(actor, 0);
  Engine_EventWait(delay);
}

static void Vault_FaceActorAndWait(s32 actor, s32 other, s32 delay) {
  Engine_ActorFaceEachOther(actor, other, 0);
  Engine_EventWait(delay);
}

/// Applies drift and gradually damps both rates toward zero.
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

/// Unused: spawns a drifting sprite near the dog, sharing its drawing
/// priority; nothing calls it.
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
    /* FAKEMATCH(temporary): the coordinate local x holds the direction mask;
     * as the literal 1, 47 of 141 instructions differ in tbs-en. */
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

/// ACTOR_WOMAN_WANTING_THE_ROOF_FIXED's search for the young man fixing the
/// roof: to one end of her path and the other, looking west and east at each,
/// for as long as the scene lasts.
s32 gVault_WomanWantingTheRoofFixedRoute[] = {
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

/// ACTOR_BOY_ROUND_THE_WELL's walk, round eight points for as long as the scene
/// lasts.
s32 gVault_BoyRoundTheWellRoute[] = {
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

/// ACTOR_GIRL_ROUND_THE_WELL's walk round the same eight points, half a circle
/// apart.
s32 gVault_GirlRoundTheWellRoute[] = {
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
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 223, 0, 135, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {VAULT_FROM_WORLD_MAP_SOUTH, CONDITION_ALWAYS, 400, 0, 472, FACING_NORTH, 0,
     -1, -1, -1, -1, 0},
    {VAULT_NORTH_GATE_UNUSED, CONDITION_ALWAYS, 224, 0, 138, FACING_SOUTH, 0,
     -1, -1, -1, -1, 0},
    {VAULT_FROM_VAULT_INTERIORS_EAST_SOUTHEAST, CONDITION_ALWAYS, 408, 0, 344,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {VAULT_FROM_VAULT_INTERIORS_SOUTHEAST, CONDITION_ALWAYS, 312, 0, 328,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {VAULT_FROM_VAULT_INTERIORS_SOUTHWEST, CONDITION_ALWAYS, 216, 0, 312,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {VAULT_FROM_VAULT_INTERIORS_EAST, CONDITION_ALWAYS, 376, 0, 248,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {VAULT_FROM_VAULT_INTERIORS_NORTH_NORTHEAST, CONDITION_ALWAYS, 296, 0, 200,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {VAULT_FROM_VAULT_INTERIORS_NORTHWEST, CONDITION_ALWAYS, 120, 0, 168,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {VAULT_FROM_SANCTUM, CONDITION_ALWAYS, 64, 0, 294, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {VAULT_FROM_VAULT_JAIL, CONDITION_ALWAYS, 70, 0, 183, FACING_SOUTH, 0, -1,
     -1, -1, -1, 0},
    {VAULT_FROM_VAULT_INTERIORS_WEST, CONDITION_ALWAYS, 152, 0, 272,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gVault_Exits[] = {
    SCENE_EXITS(Vault),
    SCENE_EXIT(VAULT_TO_WORLD_MAP_SOUTH, WorldMap, WORLD_MAP_FROM_VAULT_SOUTH),
    SCENE_EXIT(VAULT_TO_WORLD_MAP_NORTH, WorldMap, WORLD_MAP_FROM_VAULT_NORTH),
    SCENE_EXIT(VAULT_TO_VAULT_INTERIORS_EAST_SOUTHEAST, VaultInteriors,
               VAULT_INTERIORS_FROM_VAULT_EAST_SOUTHEAST),
    SCENE_EXIT(VAULT_TO_VAULT_INTERIORS_SOUTHEAST, VaultInteriors,
               VAULT_INTERIORS_FROM_VAULT_SOUTHEAST),
    SCENE_EXIT(VAULT_TO_VAULT_INTERIORS_SOUTHWEST, VaultInteriors,
               VAULT_INTERIORS_FROM_VAULT_SOUTHWEST),
    SCENE_EXIT(VAULT_TO_VAULT_INTERIORS_EAST, VaultInteriors,
               VAULT_INTERIORS_FROM_VAULT_EAST),
    SCENE_EXIT(VAULT_TO_VAULT_INTERIORS_NORTH_NORTHEAST, VaultInteriors,
               VAULT_INTERIORS_FROM_VAULT_NORTH_NORTHEAST),
    SCENE_EXIT(VAULT_TO_VAULT_INTERIORS_NORTHWEST, VaultInteriors,
               VAULT_INTERIORS_FROM_VAULT_NORTHWEST),
    SCENE_EXIT(VAULT_TO_SANCTUM, Sanctum, SANCTUM_FROM_VAULT),
    SCENE_EXIT(VAULT_TO_VAULT_JAIL, VaultJail, VAULT_JAIL_FROM_VAULT),
    SCENE_EXIT(VAULT_TO_VAULT_INTERIORS_WEST, VaultInteriors,
               VAULT_INTERIORS_FROM_VAULT_WEST),
    SCENE_EXIT(VAULT_TO_VAULT_REVISIT, VaultRevisit, VAULT_REVISIT_FROM_VAULT),
    SCENE_EXITS_END,
};

// clang-format off
struct ScenePlacement gVault_Placements[] = {
    { IVAN, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(381), 0, PIXELS(342), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(280), 0, PIXELS(396), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(264), 0, PIXELS(150), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(244), 0, PIXELS(204), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(124), 0, PIXELS(196), FACING_SOUTHEAST, TALK_FACE_PARTY, 0 },
    { SPRITE_OLD_WOMAN, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(244), 0, PIXELS(300), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, (s32)gVault_BoyRoundTheWellRoute,
        PIXELS(132), 0, PIXELS(380), FACING_WEST, TALK_KEEP_FACING, 0 },
    { SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, (s32)gVault_GirlRoundTheWellRoute,
        PIXELS(132), 0, PIXELS(420), FACING_EAST, TALK_KEEP_FACING, 0 },
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(332), 0, PIXELS(412), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_PERSON_IN_PURPLE, FLAG_VAULT_THIEVES_ATTACKED, ACTOR_STAND,
        PIXELS(92), 0, PIXELS(284), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(92), 0, PIXELS(296), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_DOG, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(216), 0, PIXELS(392), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, FLAG_VAULT_SUSPECTS_MIND_READ, ACTOR_STAND,
        PIXELS(152), 0, PIXELS(296), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, IF_FLAG_SET(FLAG_VAULT_SUSPECTS_MIND_READ), (s32)gVault_WomanWantingTheRoofFixedRoute,
        PIXELS(168), 0, PIXELS(312), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0xf8, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { VENUS_DJINNI, FLAG_DJINNI_0 + DJINNI_SAP, ACTOR_STAND,
        PIXELS(344), 0, PIXELS(96), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gVault_PlacementsAfterThieves[] = {
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(381), 0, PIXELS(342), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(280), 0, PIXELS(396), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(264), 0, PIXELS(150), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(244), 0, PIXELS(204), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(124), 0, PIXELS(196), FACING_SOUTHEAST, TALK_FACE_PARTY, 0 },
    { SPRITE_OLD_WOMAN, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(244), 0, PIXELS(300), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, (s32)gVault_BoyRoundTheWellRoute,
        PIXELS(132), 0, PIXELS(380), FACING_WEST, TALK_KEEP_FACING, 0 },
    { SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, (s32)gVault_GirlRoundTheWellRoute,
        PIXELS(132), 0, PIXELS(420), FACING_EAST, TALK_KEEP_FACING, 0 },
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(332), 0, PIXELS(412), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_BLANK, FLAG_VAULT_THIEVES_ATTACKED, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(92), 0, PIXELS(296), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_DOG, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(216), 0, PIXELS(440), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(152), 0, PIXELS(296), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, FLAG_VAULT_THIEVES_JAILED, (s32)gVault_WomanWantingTheRoofFixedRoute,
        PIXELS(168), 0, PIXELS(312), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0xf8, CONDITION_ALWAYS, 7,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { VENUS_DJINNI, FLAG_DJINNI_0 + DJINNI_SAP, ACTOR_STAND,
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
    { TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterInn) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterItemShop) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 9, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterWeaponAndArmorShop) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterMayorsHouse) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 11, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_OpenDoorwayAndEnter) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 12, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterJail) },
    { EVENT_TOUCH, 13, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterPassage) },
    { EVENT_TALK, ACTOR_WELCOMING_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgVaultWelcomeToVaultTheTownAt) },
    { EVENT_TALK, ACTOR_ERUPTION_WITNESS, CONDITION_ALWAYS, EVENT_SCRIPT(Event_AskAboutEruption) },
    { EVENT_TALK, ACTOR_VILLAGER_ON_HAMMET, CONDITION_ALWAYS, MSGNO(MsgVaultMasterHammetAWealthyLandownerAnd) },
    { EVENT_TALK, ACTOR_WOMAN_ON_STEALING, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkOfEruptionThefts) },
    { EVENT_TALK, ACTOR_OLD_MAN_ON_HAMMETS_SERVANT, CONDITION_ALWAYS, MSGNO(MsgVaultMasterHammetsServantCantReturnTo) },
    { EVENT_TALK, ACTOR_OLD_WOMAN_ON_THE_TRAVELERS, CONDITION_ALWAYS, EVENT_SCRIPT(Event_AskAboutFleeingTravelers) },
    { EVENT_TALK, ACTOR_BOY_ROUND_THE_WELL, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PityIvan) },
    { EVENT_TALK, ACTOR_GIRL_ROUND_THE_WELL, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkOfHammetsCaravan) },
    { EVENT_TALK, ACTOR_STRONGMAN_ON_WATCH, CONDITION_ALWAYS, EVENT_SCRIPT(Event_StrongmanOnWatchSuspectsOutsiders) },
    { EVENT_TALK, ACTOR_THIEF_IN_TOWN, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ThiefDeniesTheft) },
    { EVENT_TALK, ACTOR_STRANDED_TRAVELER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_AskAboutAdventure) },
    { EVENT_TALK, ACTOR_VAULT_DOG, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToDog) },
    { EVENT_TALK, ACTOR_YOUNG_MAN_FIXING_THE_ROOF, CONDITION_ALWAYS, MSGNO(MsgVaultAFieryRockBurntAHole) },
    { EVENT_TALK, ACTOR_WOMAN_WANTING_THE_ROOF_FIXED, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LookForRoofer) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WELCOMING_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgVaultISupposeTheseChildrenCantGo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ERUPTION_WITNESS, CONDITION_ALWAYS, MSGNO(MsgVaultMtAlephsEruptionWasBeyondDescription) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_HAMMET, CONDITION_ALWAYS, MSGNO(MsgVaultWhyDoesAWealthyManLike) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_STEALING, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadMindOnTheft) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_HAMMETS_SERVANT, CONDITION_ALWAYS, MSGNO(MsgVaultPoorIvanButTheRodMay) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_THE_TRAVELERS, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadMindOnTravelers) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_ROUND_THE_WELL, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadMindOnIvan) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_ROUND_THE_WELL, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadMindOnWealth) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRONGMAN_ON_WATCH, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadMindOnSuspects) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_THIEF_IN_TOWN, CONDITION_ALWAYS, MSGNO(MsgVaultHmphItLooksLikeTheyreOnto) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRANDED_TRAVELER, CONDITION_ALWAYS, MSGNO(MsgVaultIWasJustAboutToLeave) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VAULT_DOG, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadDogMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_FIXING_THE_ROOF, CONDITION_ALWAYS, MSGNO(MsgVaultFixTheRoofWhyPayFor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_WANTING_THE_ROOF_FIXED, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadWomanWantingTheRoofFixedMind) },
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
    { TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterInn) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterItemShop) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 9, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterWeaponAndArmorShop) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterMayorsHouse) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 11, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_OpenDoorwayAndEnter) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 12, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterJail) },
    { EVENT_TOUCH, 13, CONDITION_ALWAYS, EVENT_SCRIPT(Event_EnterPassage) },
    { EVENT_TALK, ACTOR_WELCOMING_YOUNG_MAN, CONDITION_ALWAYS, EVENT_SCRIPT(Event_AskAboutLeavingAgain) },
    { EVENT_TALK, ACTOR_ERUPTION_WITNESS, CONDITION_ALWAYS, MSGNO(MsgVaultSmokeIsStillRisingFromMt) },
    { EVENT_TALK, ACTOR_VILLAGER_ON_HAMMET, CONDITION_ALWAYS, MSGNO(MsgVaultMasterHammetTheWealthyMerchantI) },
    { EVENT_TALK, ACTOR_WOMAN_ON_STEALING, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PraiseThievesCapture) },
    { EVENT_TALK, ACTOR_OLD_MAN_ON_HAMMETS_SERVANT, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkAboutIvan) },
    { EVENT_TALK, ACTOR_OLD_WOMAN_ON_THE_TRAVELERS, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkOfDamagedRoads) },
    { EVENT_TALK, ACTOR_BOY_ROUND_THE_WELL, CONDITION_ALWAYS, EVENT_SCRIPT(Event_AskWhetherIvanWentToLunpa) },
    { EVENT_TALK, ACTOR_GIRL_ROUND_THE_WELL, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ThankForCapturingThieves) },
    { EVENT_TALK, ACTOR_STRONGMAN_ON_WATCH, CONDITION_ALWAYS, EVENT_SCRIPT(Event_AskAboutPrisoners) },
    { EVENT_TALK, ACTOR_STRANDED_TRAVELER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_AskWhoCapturedThieves) },
    { EVENT_TALK, ACTOR_VAULT_DOG, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToDogAfterCapture) },
    { EVENT_TALK, ACTOR_YOUNG_MAN_FIXING_THE_ROOF, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ThankBeforeDeparture) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WELCOMING_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgVaultYouveMadeOurTownPeacefulAt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ERUPTION_WITNESS, CONDITION_ALWAYS, MSGNO(MsgVaultNowMtAlephHasCalmedDown) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_HAMMET, CONDITION_ALWAYS, MSGNO(MsgVaultOfAllThePlacesWhyWould) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_STEALING, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadMindOnTheft) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_HAMMETS_SERVANT, CONDITION_ALWAYS, MSGNO(MsgVaultNobodyGetsIntoLunpaExceptFor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_THE_TRAVELERS, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadMindOnTravelers) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_ROUND_THE_WELL, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadMindOnIvan) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_ROUND_THE_WELL, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadMindOnWealth) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRONGMAN_ON_WATCH, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadMindOnSuspects) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRANDED_TRAVELER, CONDITION_ALWAYS, MSGNO(MsgVaultThereIsAlwaysAFightWhen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VAULT_DOG, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadDogMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_FIXING_THE_ROOF, CONDITION_ALWAYS, MSGNO(MsgVaultSniffAhTheSmellOfCooking) },
    { SEARCH(SEARCH_UNNAMED), 30, CONDITION_ALWAYS, EVENT_SCRIPT(Event_RingBell) },
    { SEARCH(SEARCH_UNNAMED), 90, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PeerIntoWell) },
    { USE_ITEM_ON_ACTOR(ITEM_BONE), ACTOR_VAULT_DOG, FLAG_VAULT_DOG_SHOWN_PASSAGE, EVENT_SCRIPT(Event_GiveBoneToDog) },
    { SEARCH(SEARCH_WOODEN_BOX), 101, FLAG_VAULT_BOX_NUT, FIND_ITEM(ITEM_NUT) },
    { SCENE_EVENTS_END },
};
// clang-format on

// Each door's opening, two frames of map cells, named for the room it opens
// into (Event_EnterSoutheastHouse and the rest).
u16 gVault_SoutheastHouseDoorCells[] = {
    MAP_CELL_STEP(35, 52, 2, 2, 4),
    MAP_CELL_STEP(33, 52, 2, 2, 4),
    MAP_CELL_STEPS_END,
};

u16 gVault_CentralEastHouseDoorCells[] = {
    MAP_CELL_STEP(35, 50, 2, 2, 4),
    MAP_CELL_STEP(33, 50, 2, 2, 4),
    MAP_CELL_STEPS_END,
};

u16 gVault_InnDoorCells[] = {
    MAP_CELL_STEP(35, 48, 2, 2, 4),
    MAP_CELL_STEP(33, 48, 2, 2, 4),
    MAP_CELL_STEPS_END,
};

u16 gVault_ItemShopDoorCells[] = {
    MAP_CELL_STEP(42, 48, 2, 2, 4),
    MAP_CELL_STEP(40, 48, 2, 2, 4),
    MAP_CELL_STEPS_END,
};

u16 gVault_WeaponAndArmorShopDoorCells[] = {
    MAP_CELL_STEP(42, 46, 2, 2, 4),
    MAP_CELL_STEP(40, 46, 2, 2, 4),
    MAP_CELL_STEPS_END,
};

u16 gVault_MayorsHouseDoorCells[] = {
    MAP_CELL_STEP(35, 46, 2, 2, 4),
    MAP_CELL_STEP(33, 46, 2, 2, 4),
    MAP_CELL_STEPS_END,
};

u16 gVault_JailDoorCells[] = {
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
