/// @file
/// Vault (MapId_Vault), "the town at the heart of Angara", as the party first
/// finds it after Mt. Aleph's eruption: its streets, with the townspeople,
/// Ivan, the bell and the dog. The houses, the inn and its loft, where the
/// search for Hammet's stolen Shaman's Rod plays out, are vault_interiors.c.
/// Map_GetObjectEventTemplates and Map_GetEvents choose their tables by
/// FLAG_VAULT_THIEVES_JAILED, and once the party has reached Kalay
/// (FLAG_KALAY_REACHED) Map_Initialize sends it straight on to the town's
/// later map (VAULT_TO_VAULT_REVISIT, MapId_VaultRevisit,
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
///   leaving by either gate has him stay behind (EventScript_IvanStaysBehind),
///   and Map_Initialize has him rejoin when the party comes back in by a gate
///   or out of the Vault Sanctum.
/// - Once the thieves are jailed, the townspeople thank the party, and the
///   dog, given a Bone (EventScript_GiveBoneToDog,
///   FLAG_VAULT_DOG_SHOWN_PASSAGE), runs to a spot and jumps there three times;
///   the passage it shows opens only in the later map.
///
/// Sap, a Venus Djinni, hides near the bell, and the first ring since the party
/// came in by a gate (FLAG_VAULT_DJINNI_CALLED_BY_BELL) calls it out to stand
/// beside the bell; it joins only in the later map. The well rumbles with
/// water and a wooden box holds a Nut. The doors open with an animation
/// (gVault_*DoorCells) and lead into the houses, shops and inn
/// (MapId_VaultInteriors), the jail (MapId_VaultJail) and the Vault Sanctum
/// (VAULT_TO_SANCTUM); the metatile behaviors the events answer are enum
/// VaultCell (vault.h).
///
/// The rope bridge in the town's south-east spans a path that runs under it,
/// and stepping onto either end of it or off its west end switches the cells
/// beneath it between the deck and the path (common/vault_toggle.h).
///
/// Open questions:
/// - XXX (no evidence): what the ivy placed at (0, 0) with
///   MOVEMENT_TYPE_RESET_ACTION, object event 22, the row after
///   LOCALID_WOMAN_WANTING_THE_ROOF_FIXED, was for; it is drawn above the map's
///   top edge where no camera shows it, is not solid, and only the engine's
///   passes over every object event touch it (render probe vault-ivy, watching
///   ObjectTable_Get for it while the town loads, while the leader walks and
///   while Whirlwind is cast; vault_revisit.c's table has the same row), so
///   nothing in the ROM says what it was meant to do.

#include "game_state.h"
#include "animspr.h"
#include "window.h"
#include "unit/map/vault.h"
#include "vault.h"
#include "unit/map/common/vault_range.h"
#include "unit/map/common/effect_spawn.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/battle/battle.h"
#include "unit/game/game.h"
#include "unit/runtime/runtime.h"
#include "object_event.h"
#include "types.h"
#include "script_layer.h"
#include "map_tables.h"
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
  LOCALID_WELCOMING_YOUNG_MAN = 8,
  /// A woman with fair hair who asks about the eruption.
  /// ★ Named from the game: "Wasn't the eruption of Mt. Aleph incredible?"
  /// (MsgVaultWasntEruptionMtAlephIncredible)
  LOCALID_ERUPTION_WITNESS = 9,
  /// A woman.
  /// ★ Named from the game: "No man should steal from another during the
  /// panic of an eruption." (MsgVaultManShouldStealFromAnother)
  LOCALID_WOMAN_ON_STEALING = 11,
  /// A burly grey-haired man.
  /// ★ Named from the game: "Master Hammet, a wealthy landowner and merchant
  /// from Kalay, had his precious rod stolen from him."
  /// (MsgVaultMasterHammetAWealthyLandownerAnd)
  LOCALID_VILLAGER_ON_HAMMET = 10,
  /// An old man with a cane.
  /// ★ Named from the game: "Master Hammet's servant can't return to Kalay
  /// until he recovers the rod." (MsgVaultMasterHammetsServantCantReturnTo)
  LOCALID_OLD_MAN_ON_HAMMETS_SERVANT = 12,
  /// An old woman.
  /// ★ Named from the game: "Those travelers left in a big hurry."
  /// (MsgVaultThoseTravelersLeftInBig)
  LOCALID_OLD_WOMAN_ON_THE_TRAVELERS = 13,
  /// The two children who walk round the well, half a circle apart
  /// (gVault_BoyRoundTheWellRoute, gVault_GirlRoundTheWellRoute).
  /// ★ Named from what the player sees: a blond boy walking round the well
  /// (its render)
  LOCALID_BOY_ROUND_THE_WELL = 14,
  /// ★ Named from what the player sees: a girl with orange hair walking round
  /// the well (its render)
  LOCALID_GIRL_ROUND_THE_WELL = 15,
  /// A stocky, muscular man.
  /// ★ Named from the game: "Nobody's stealing anything on my watch!"
  /// (MsgVaultNobodysStealingAnything)
  LOCALID_STRONGMAN_ON_WATCH = 16,
  /// One of the three thieves, drawn with long purple hair as in the loft,
  /// standing outside until the party confronts them
  /// (FLAG_VAULT_THIEVES_ATTACKED).
  /// ★ Named from the game: "Are you accusing us of stealing Hammet's treasured
  /// Shaman's Rod?" (MsgVaultAccusingUsStealingHammetsTreasured)
  LOCALID_THIEF_IN_TOWN = 17,
  /// A bearded man in a long robe.
  /// ★ Named from the game: "I was just about to leave this town behind me when
  /// the volcano erupted." (MsgVaultIWasJustAboutToLeave)
  LOCALID_STRANDED_TRAVELER = 18,
  /// ★ Named from what the player sees: a white dog sitting among the graves
  /// (its render)
  LOCALID_VAULT_DOG = 19,
  /// A young man who fixes the roof himself, and is later found tied up in the
  /// inn's loft.
  /// ★ Named from the game: "Why pay for a carpenter when I can just hammer it
  /// a bit myself?" (MsgVaultFixTheRoofWhyPayFor)
  LOCALID_YOUNG_MAN_FIXING_THE_ROOF = 20,
  /// A woman who walks about looking for LOCALID_YOUNG_MAN_FIXING_THE_ROOF.
  /// ★ Named from the game: "He should be fixing the roof now, but I can't
  /// find him anywhere..." (MsgVaultFixingRoofCant)
  LOCALID_WOMAN_WANTING_THE_ROOF_FIXED = 21,
  /// Sap, a Venus Djinni, hiding until the bell calls it.
  /// ★ Named from the game: "Sap" (MsgDjinnName)
  LOCALID_SAP = 23
};

static void Vault_ShowStandingObjectEventMessage(s32 localId);
static void Vault_ShowMessageAndWait(s32 localId, s32 delay);
static void Vault_FaceObjectEventAndWait(s32 localId, s32 other, s32 delay);

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
  struct ObjectEvent *entity = (struct ObjectEvent *)fieldObject;
  struct UiRenderWork *work = (struct UiRenderWork *)gWindowWork[0];
  u8 *workspace = gWindowWork[12];
  s32 flag = 0;
  s32 selector = 18;
  struct ObjectEvent *leader;

  if (entity->targetX == OBJ_EVENT_NO_TARGET)
#ifdef UBFIX
    return;
#else
    return 0;
#endif

  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (*(s16 *)(workspace + 376) != 0 || work->mode != 0) {
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

const struct MapWarpDest *Map_GetWarpDests(void) { return gVault_WarpDests; }

const struct MapWarpRegion *Map_GetWarpRegions(void) { return NULL; }

const u32 *Map_GetWarps(void) { return gVault_Warps; }

const struct ObjectEventTemplate *Map_GetObjectEventTemplates(void) {
  if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) != 0)
    return gVault_ObjectEventTemplatesAfterThieves;
  return gVault_ObjectEventTemplates;
}

/// Counts the Venus Djinn that must leave Ivan before he leaves the party.
static int Vault_GetIvanVenusDjinnCount(void) {
  return Unit_Get(IVAN)->djinnOwnedCounts[ELEMENT_VENUS];
}

#include "common/vault_djinn.h"

/// Leaving Vault by either gate (VAULT_CELL_SOUTH_GATE and _NORTH_GATE, before
/// VAULT_TO_WORLD_MAP_SOUTH and VAULT_TO_WORLD_MAP_NORTH): while Ivan is
/// helping search for the rod and the thieves are free, he walks out to the
/// gate, says he is still worried about the rod and will wait in Vault, leaves
/// any Venus Djinni with Isaac and leaves the party.
static void EventScript_IvanStaysBehind(void) {
  struct ObjectEvent *objectEvent;
  struct EventWork *work;

  work = gEventWork;
  if (GameFlag_IsSet(FLAG_VAULT_THIEVES_JAILED) != 0 ||
      GameFlag_IsSet(FLAG_VAULT_IVAN_JOINED_ROD_SEARCH) == 0) {
    Engine_ScriptRequestWarp(work->touchedTrigger - (VAULT_CELL_SOUTH_GATE -
                                                     VAULT_TO_WORLD_MAP_SOUTH));
    return;
  }
  // Bring Ivan out beside the party at the warp they are using.
  Engine_ScriptStart();
  objectEvent = ObjectEvent_Get(LOCALID_PARTY_LEADER);
  if (objectEvent != 0) {
    ObjectEvent_SetPosition(IVAN, objectEvent->x.fixed, objectEvent->z.fixed);
  }
  ObjectEvent_SetSpeed(IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
  if (work->touchedTrigger == VAULT_CELL_SOUTH_GATE) {
    ObjectEvent_WalkToAndWait(IVAN, 400, 448);
  } else {
    Camera_SetSpeed(PIXELS(4) / 5, PIXELS(1) / 10);
    Camera_MoveTo(PIXELS(224), -1, PIXELS(162), 1);
    ObjectEvent_WalkToAndWait(IVAN, 224, 162);
    Engine_CameraWaitForMove();
  }
  Engine_ObjectEventFaceEachOther(LOCALID_PARTY_LEADER, IVAN, 0);
  Engine_ScriptWait(20);
  Engine_ScriptSetMessage(MsgVaultLeavingImStillWorriedAbout);
  Script_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT | IVAN,
                            0, 20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_PARTY_LEADER, ANIMATION_NOD);
  // Return any Djinn before removing Ivan from the party.
  if (Vault_GetIvanVenusDjinnCount() != 0) {
    Engine_ScriptSetMessage(MsgVaultWaitDontWantTakeYour);
    Script_ShowMessage(IVAN, 0);
    Vault_ReturnIsaacsDjinni();
    Engine_TaskWait(20);
  }
  Party_RemoveOwnerRestored(IVAN);
  Engine_ScriptRequestWarp(work->touchedTrigger -
                           (VAULT_CELL_SOUTH_GATE - VAULT_TO_WORLD_MAP_SOUTH));
  Engine_ScriptCloseScreen();
  Engine_ScriptWaitForScreen();
  Engine_ScriptEnd();
}

/// In Vault, Isaac peers into the well and hears water rumbling down there.
static void EventScript_PeerIntoWell(void) {
  Engine_ScriptStart();
  Engine_MessageShowCentered(MsgFieldPeeredWell, 1);
  Engine_MessageShowCentered(MsgVaultCanHearWaterRumblingDown, 1);
  Engine_ScriptEnd();
}

const struct MapEvent *Map_GetEvents(void) {
  if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) != 0)
    return gVault_EventsAfterThieves;
  return gVault_Events;
}

/// Asks the current question and selects its ordinary or alternate reply.
static inline void Vault_AskYesNoAndShowReply(s32 speaker) {
  Engine_ScriptOpenMessage(speaker, 0);
  if (Engine_ScriptChooseYesNo(0, 0) != 0) {
    struct EventWork *work = gEventWork;

    ++work->message;
  }
  Engine_ScriptShowMessage(speaker, 0);
}

/// In Vault while the rod is missing, a townsperson asks about the eruption.
/// "Wasn't the eruption of Mt. Aleph incredible?" Yes: too terrified to sleep
/// for days. No: aren't you courageous!
static void EventScript_AskAboutEruption(void) {
  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgVaultWasntEruptionMtAlephIncredible);
  Vault_FaceObjectEventAndWait(LOCALID_ERUPTION_WITNESS, LOCALID_PARTY_LEADER,
                               2);
  Script_OpenMessage(LOCALID_ERUPTION_WITNESS, 0);
  if (Engine_ScriptChooseYesNo(0, 0) != 0) {
    gEventWork->message++;
  }
  Script_ShowMessage(LOCALID_ERUPTION_WITNESS, 0);
  ObjectEvent_FaceDirection(LOCALID_ERUPTION_WITNESS,
                            FACING_SOUTH + FACING_STEP, 0);
  Engine_ScriptEnd();
}

/// In Vault while the rod is missing, a townsperson says travelers left town
/// in a big hurry: did the party see them? Yes: two women and four men, the
/// girl and old man unhappy to be there. No: maybe they left earlier.
static void EventScript_AskAboutFleeingTravelers(void) {

  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgVaultThoseTravelersLeftInBig);
  Engine_ObjectEventSetAnimation(LOCALID_OLD_WOMAN_ON_THE_TRAVELERS,
                                 ANIMATION_STAND);
  Vault_FaceObjectEventAndWait(LOCALID_OLD_WOMAN_ON_THE_TRAVELERS,
                               LOCALID_PARTY_LEADER, 2);
  Vault_AskYesNoAndShowReply(LOCALID_OLD_WOMAN_ON_THE_TRAVELERS);
  Engine_ScriptEnd();
}

/// In Vault, one of the thieves loitering in town asks whether the party
/// accuses them; either answer gets a denial.
/// "Are you accusing us of stealing Hammet's treasured Shaman's Rod?"
static void EventScript_ThiefDeniesTheft(void) {

  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgVaultAccusingUsStealingHammetsTreasured);
  Vault_FaceObjectEventAndWait(LOCALID_THIEF_IN_TOWN, LOCALID_PARTY_LEADER, 2);
  Vault_AskYesNoAndShowReply(LOCALID_THIEF_IN_TOWN);
  Engine_ScriptEnd();
}

/// A stranded traveler asks whether the party is off on an adventure.
/// The traveler was stranded by the eruption, and asks while the rod is
/// missing.
/// "Off on an adventure, are you?"
/// Yes: impressive for youngsters. No: see the Sanctum while you're here.
static void EventScript_AskAboutAdventure(void) {

  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgVaultOffOnAdventure);
  Vault_FaceObjectEventAndWait(LOCALID_STRANDED_TRAVELER, LOCALID_PARTY_LEADER,
                               2);
  Vault_AskYesNoAndShowReply(LOCALID_STRANDED_TRAVELER);
  Engine_ScriptEnd();
}

/// A townsperson says some folks got robbed in the chaos of the eruption.
/// She says it while the rod is missing.
static void EventScript_TalkOfEruptionThefts(void) {
  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgVaultGuessFolksGot);
  Vault_ShowStandingObjectEventMessage(LOCALID_WOMAN_ON_STEALING);
  Engine_ScriptEnd();
}

/// In Vault while the rod is missing, the strongman on watch: nobody steals on
/// his watch, the thieves must be from out of town.
static void EventScript_StrongmanOnWatchSuspectsOutsiders(void) {
  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgVaultNobodysStealingAnything);
  Vault_ShowStandingObjectEventMessage(LOCALID_STRONGMAN_ON_WATCH);
  Engine_ScriptEnd();
}

/// In Vault while the rod is missing, the dog stops and barks.
static void EventScript_TalkToDog(void) {
  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgVaultRuffRrruff);
  Engine_ObjectEventSetAnimation(LOCALID_VAULT_DOG, 0);
  Vault_FaceObjectEventAndWait(LOCALID_VAULT_DOG, LOCALID_PARTY_LEADER, 2);
  Engine_ScriptShowMessage(LOCALID_VAULT_DOG, 0);
  Engine_ScriptEnd();
}

/// In Vault while the rod is missing, the boy round the well stops: poor Ivan
/// was left at the mayor's to look for the stolen rod.
static void EventScript_PityIvan(void) {
  struct ObjectEvent *objectEvent =
      ObjectEvent_GetById(LOCALID_BOY_ROUND_THE_WELL);
  s16 savedFacing = objectEvent->facing;

  objectEvent->timer |= 2;
  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgVaultPoorGuyLeft);
  Engine_ObjectEventSetAnimation(LOCALID_BOY_ROUND_THE_WELL, 0);
  Vault_FaceObjectEventAndWait(LOCALID_BOY_ROUND_THE_WELL, LOCALID_PARTY_LEADER,
                               2);
  Vault_ShowMessageAndWait(LOCALID_BOY_ROUND_THE_WELL, 10);
  objectEvent->facing = savedFacing;
  Engine_TaskWait(1);
  Engine_ScriptEnd();
  objectEvent->timer &= 1;
}

/// In Vault while the rod is missing, the girl round the well stops: Hammet's
/// caravan left as soon as the eruption stopped.
static void EventScript_TalkOfHammetsCaravan(void) {
  struct ObjectEvent *objectEvent =
      ObjectEvent_GetById(LOCALID_GIRL_ROUND_THE_WELL);
  s16 savedFacing = objectEvent->facing;

  objectEvent->timer |= 2;
  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgVaultMasterHammetsCaravan);
  Engine_ObjectEventSetAnimation(LOCALID_GIRL_ROUND_THE_WELL, 0);
  Vault_FaceObjectEventAndWait(LOCALID_GIRL_ROUND_THE_WELL,
                               LOCALID_PARTY_LEADER, 2);
  Vault_ShowMessageAndWait(LOCALID_GIRL_ROUND_THE_WELL, 10);
  objectEvent->facing = savedFacing;
  Engine_TaskWait(1);
  Engine_ScriptEnd();
  objectEvent->timer &= 1;
}

/// In Vault, once the suspects' minds are read, the woman wanting
/// the roof fixed: the man who should be fixing it is probably in back
/// slacking off.
static void EventScript_LookForRoofer(void) {
  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgVaultFixingRoofCant);
  Vault_FaceObjectEventAndWait(LOCALID_WOMAN_WANTING_THE_ROOF_FIXED,
                               LOCALID_PARTY_LEADER, 2);
  Engine_ObjectEventShowEmote(LOCALID_WOMAN_WANTING_THE_ROOF_FIXED,
                              EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  Engine_ScriptWait(30);
  Engine_ScriptOpenMessage(LOCALID_WOMAN_WANTING_THE_ROOF_FIXED, 0);
  Engine_ScriptEnd();
}

/// Stops an object event, faces the leader and shows the current message.
static void Vault_ShowStandingObjectEventMessage(s32 localId) {
  Engine_ScriptStart();
  Engine_ObjectEventSetAnimation(localId, ANIMATION_STAND);
  Vault_FaceObjectEventAndWait(localId, LOCALID_PARTY_LEADER, 2);
  Engine_ScriptShowMessage(localId, 0);
  Engine_ScriptEnd();
}

#include "common/vault_talk.h"

/// The welcoming young man asks whether the party is leaving again.
/// He asks after the thieves are jailed. "So, you're hitting the road again?"
static void EventScript_AskAboutLeavingAgain(void) {
  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgVaultYoureHittingRoadAgain);
  Vault_FaceObjectEventAndWait(LOCALID_WELCOMING_YOUNG_MAN,
                               LOCALID_PARTY_LEADER, 2);
  Vault_AskYesNoAndShowReply(LOCALID_WELCOMING_YOUNG_MAN);
  Engine_ScriptEnd();
}

/// In Vault after the thieves are jailed, a townsperson praises their capture.
/// "It's great that you caught those thieves!"
static void EventScript_PraiseThievesCapture(void) {
  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgVaultGreatCaughtThieves);
  Vault_ShowStandingObjectEventMessage(LOCALID_WOMAN_ON_STEALING);
  Engine_ScriptEnd();
}

/// An old man talks about Ivan, Hammet's servant.
/// After the thieves are jailed, he says Ivan went out to Lunpa, or, with Ivan
/// in the party, that rescuing Hammet must have been impossible.
static void EventScript_TalkAboutIvan(void) {
  struct EventWork *workspace;
  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgVaultTalkingAboutHammetsServantIvan);
  if (Engine_GameFlagIsSet(FLAG_IN_PARTY(IVAN)) != 0) {
    workspace = gEventWork;
    ++workspace->message;
  }
  Vault_ShowStandingObjectEventMessage(LOCALID_OLD_MAN_ON_HAMMETS_SERVANT);
  Engine_ScriptEnd();
}

/// In Vault after the thieves are jailed, a townsperson says the eruption
/// devastated the roads; once the southern bridge is repaired, you can head
/// toward Kalay.
static void EventScript_TalkOfDamagedRoads(void) {
  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgVaultEruptionDevastatedRoads);
  Vault_ShowStandingObjectEventMessage(LOCALID_OLD_WOMAN_ON_THE_TRAVELERS);
  Engine_ScriptEnd();
}

/// The boy round the well asks after Ivan.
/// After the thieves are jailed, he hopes Ivan didn't go to Lunpa alone, or,
/// with Ivan in the party, asks when he joined up with Isaac.
static void EventScript_AskWhetherIvanWentToLunpa(void) {
  struct ObjectEvent *objectEvent =
      ObjectEvent_GetById(LOCALID_BOY_ROUND_THE_WELL);
  s16 savedFacing = objectEvent->facing;

  objectEvent->timer |= 2;
  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgVaultDidntGoLunpa);
  if (Engine_GameFlagIsSet(FLAG_IN_PARTY(IVAN)) != 0)
    ++gEventWork->message;
  Engine_ObjectEventSetAnimation(LOCALID_BOY_ROUND_THE_WELL, 0);
  Vault_FaceObjectEventAndWait(LOCALID_BOY_ROUND_THE_WELL, LOCALID_PARTY_LEADER,
                               2);
  Vault_ShowMessageAndWait(LOCALID_BOY_ROUND_THE_WELL, 10);
  objectEvent->facing = savedFacing;
  Engine_TaskWait(1);
  Engine_ScriptEnd();
  objectEvent->timer &= 1;
}

/// In Vault after the thieves are jailed, the girl round the well: everyone is
/// grateful the party captured the thieves.
static void EventScript_ThankForCapturingThieves(void) {
  struct ObjectEvent *objectEvent =
      ObjectEvent_GetById(LOCALID_GIRL_ROUND_THE_WELL);
  s16 savedFacing = objectEvent->facing;

  objectEvent->timer |= 2;
  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgVaultEveryoneGratefulCaptured);
  Engine_ObjectEventSetAnimation(LOCALID_GIRL_ROUND_THE_WELL, 0);
  Vault_FaceObjectEventAndWait(LOCALID_GIRL_ROUND_THE_WELL,
                               LOCALID_PARTY_LEADER, 2);
  Vault_ShowMessageAndWait(LOCALID_GIRL_ROUND_THE_WELL, 10);
  objectEvent->facing = savedFacing;
  Engine_TaskWait(1);
  Engine_ScriptEnd();
  objectEvent->timer &= 1;
}

/// In Vault after the thieves are jailed, the strongman on watch asks about the
/// prisoners. "Those men you captured... They're in jail now?" Yes: the
/// prison hadn't been used in a long time. No: they're upset; maybe go see
/// them.
static void EventScript_AskAboutPrisoners(void) {
  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgVaultThoseMenCapturedTheyreIn);
  Engine_ObjectEventSetAnimation(LOCALID_STRONGMAN_ON_WATCH, ANIMATION_STAND);
  Vault_FaceObjectEventAndWait(LOCALID_STRONGMAN_ON_WATCH, LOCALID_PARTY_LEADER,
                               2);
  Vault_AskYesNoAndShowReply(LOCALID_STRONGMAN_ON_WATCH);
  Engine_ScriptEnd();
}

/// In Vault after the thieves are jailed, the traveler asks who captured the
/// thieves; either answer is praised.
/// "Are you the ones who captured the thieves?"
static void EventScript_AskWhoCapturedThieves(void) {
  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgVaultOnesWhoCapturedThieves);
  Vault_FaceObjectEventAndWait(LOCALID_STRANDED_TRAVELER, LOCALID_PARTY_LEADER,
                               2);
  Vault_AskYesNoAndShowReply(LOCALID_STRANDED_TRAVELER);
  Engine_ScriptEnd();
}

/// In Vault after the thieves are jailed, the dog barks; carrying a Bone
/// before it has shown the passage lets the party give it one.
static void EventScript_TalkToDogAfterCapture(void) {
  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgVaultRuffRrruff2);
  Engine_ObjectEventSetAnimation(LOCALID_VAULT_DOG, 0);
  Vault_FaceObjectEventAndWait(LOCALID_VAULT_DOG, LOCALID_PARTY_LEADER, 2);
  Engine_ScriptShowMessage(LOCALID_VAULT_DOG, 0);
  Engine_ObjectEventSetAnimation(LOCALID_VAULT_DOG, ANIMATION_STAND);
  if (PartyInventory_FindOwner(ITEM_BONE) != -1 &&
      Engine_GameFlagIsSet(FLAG_VAULT_DOG_SHOWN_PASSAGE) == 0) {
    u16 *menuRequest = &gEventWork->requestedMenu;
    u16 open = 1;

    *menuRequest = open;
  }
  Engine_ScriptEnd();
}

/// The young man fixing the roof thanks the party before it leaves.
/// The party freed him, and he says so after the thieves are jailed.
/// "Thank you for the other day.
/// Are you leaving town already?"
static void EventScript_ThankBeforeDeparture(void) {
  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgVaultThankForOtherDayLeaving);
  Vault_FaceObjectEventAndWait(LOCALID_YOUNG_MAN_FIXING_THE_ROOF,
                               LOCALID_PARTY_LEADER, 2);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_YOUNG_MAN_FIXING_THE_ROOF,
                                        ANIMATION_NOD);
  Engine_ScriptWait(20);
  Engine_ScriptShowMessage(LOCALID_YOUNG_MAN_FIXING_THE_ROOF, 0);
  Engine_ScriptEnd();
}

/// In Vault, Mind Read on a townsperson: stealing during an eruption is
/// unforgivable, or how should the prisoners be punished?
static void EventScript_ReadMindOnTheft(void) {
  Engine_ScriptStart();
  if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) == 0)
    Engine_ScriptSetMessage(MsgVaultManShouldStealFromAnother);
  else
    Engine_ScriptSetMessage(MsgVaultHowPunishPrisoners);
  Vault_ShowThought(LOCALID_WOMAN_ON_STEALING);
  Engine_ScriptEnd();
}

/// In Vault, Mind Read on a townsperson: the strange travelers headed for the
/// northeast of the Goma Range, or the only way out is the cave in the Goma
/// Range.
static void EventScript_ReadMindOnTravelers(void) {
  Engine_ScriptStart();
  if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) == 0) {
    Engine_ScriptSetMessage(MsgVaultGroupTravelersWasStrangeBunch);
  } else {
    Engine_ScriptSetMessage(MsgVaultWithRoadOutOnlyWay);
  }
  Vault_ShowThought(LOCALID_OLD_WOMAN_ON_THE_TRAVELERS);
  Engine_ScriptEnd();
}

/// In Vault, Mind Read on the boy round the well: the three creeps at the inn
/// stole the rod, or where did Ivan go (when did they join him, with Ivan in
/// the party)?
static void EventScript_ReadMindOnIvan(void) {
  ObjectEvent_GetById(LOCALID_BOY_ROUND_THE_WELL)->timer |= 2;
  Engine_ScriptStart();
  if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) == 0) {
    Engine_ScriptSetMessage(MsgVaultBetWasThoseThreeCreeps);
  } else {
    Engine_ScriptSetMessage(MsgVaultWhereDidIvanGoBy);
    if (Engine_GameFlagIsSet(FLAG_IN_PARTY(IVAN)) != 0)
      ++gEventWork->message;
  }
  Vault_ShowThought(LOCALID_BOY_ROUND_THE_WELL);
  Engine_ScriptEnd();
  ObjectEvent_GetById(LOCALID_BOY_ROUND_THE_WELL)->timer &= 1;
}

/// In Vault, Mind Read on the girl round the well: riches can't save you from a
/// volcano, or the party must be stronger than they look.
static void EventScript_ReadMindOnWealth(void) {
  ObjectEvent_GetById(LOCALID_GIRL_ROUND_THE_WELL)->timer |= 2;
  Engine_ScriptStart();
  if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) == 0)
    Engine_ScriptSetMessage(MsgVaultSupposeDoesntMatterHowRich);
  else
    Engine_ScriptSetMessage(MsgVaultMustStrongerThanLookHave);
  Vault_ShowThought(LOCALID_GIRL_ROUND_THE_WELL);
  Engine_ScriptEnd();
  ObjectEvent_GetById(LOCALID_GIRL_ROUND_THE_WELL)->timer &= 1;
}

/// In Vault, Mind Read on the strongman on watch: everyone knows the three at
/// the inn stole the rod, or he feels much better with them behind bars.
static void EventScript_ReadMindOnSuspects(void) {
  Engine_ScriptStart();
  if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) == 0)
    Engine_ScriptSetMessage(MsgVaultEveryoneKnowsThoseThreeAt);
  else
    Engine_ScriptSetMessage(MsgVaultFeelMuchBetter);
  Vault_ShowThought(LOCALID_STRONGMAN_ON_WATCH);
  Engine_ScriptEnd();
}

/// In Vault, Mind Read on the dog: humans value money over their lives; after
/// the thieves are jailed, bring a bone for something useful, and once fed, a
/// careful search will reveal a passage leading underground.
static void EventScript_ReadDogMind(void) {
  struct ObjectEvent *objectEvent = ObjectEvent_GetById(LOCALID_VAULT_DOG);
  objectEvent->paused = 1;
  Engine_ScriptStart();
  if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) == 0) {
    Engine_ScriptSetMessage(MsgVaultStealingInMidstVolcanicEruption);
    Engine_ObjectEventSetAnimation(LOCALID_VAULT_DOG, 0);
    Engine_ScriptWait(2);
  } else if (Engine_GameFlagIsSet(FLAG_VAULT_DOG_SHOWN_PASSAGE) != 0) {
    Engine_ScriptSetMessage(MsgVaultCarefulSearchWillRevealPassage);
  } else {
    Engine_ScriptSetMessage(MsgVaultIfBringMeBoneIll);
  }
  Engine_ScriptShowMessage(LOCALID_VAULT_DOG, 0);
  Engine_ScriptEnd();
  objectEvent->paused = 0;
}

/// In Vault, Mind Read on the woman wanting the roof fixed: that sneak is
/// wasting time instead of fixing the roof.
static void EventScript_ReadWomanWantingTheRoofFixedMind(void) {
  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgVaultSneakWastingTime);
  Vault_ShowThought(LOCALID_WOMAN_WANTING_THE_ROOF_FIXED);
  Engine_ScriptEnd();
}

#include "common/vault_toggle.h"

/// Walks to the warp's threshold and requests the room's transition.
static void Vault_WalkThroughExit(s32 x, s32 y, s32 continuation) {
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
  ObjectEvent_WalkTo(LOCALID_PARTY_LEADER, x, y);
  gEventWork->transitionFrames = 16;
  Engine_ScriptRequestWarp(continuation);
}

/// The door of the gem owner's house opens and the party walks in.
/// It's VAULT_TO_VAULT_INTERIORS_EAST_SOUTHEAST, into the house where a man
/// keeps a sparkling rock the volcano threw.
static void EventScript_EnterGemOwnersHouse(void) {
  Engine_AudioPlayCue(SE_DOOR_OPEN);
  Engine_MapGridAnimateCells(gVault_GemOwnersHouseDoorCells, 56, 19);
  Vault_WalkThroughExit(408, 320, VAULT_TO_VAULT_INTERIORS_EAST_SOUTHEAST);
}

/// The door of a child's home opens and the party walks in.
/// It's VAULT_TO_VAULT_INTERIORS_SOUTHEAST, into the house east of the town's
/// centre.
static void EventScript_EnterChildsHouse(void) {
  Engine_AudioPlayCue(SE_DOOR_OPEN);
  Engine_MapGridAnimateCells(gVault_ChildsHouseDoorCells, 50, 18);
  Vault_WalkThroughExit(312, 304, VAULT_TO_VAULT_INTERIORS_SOUTHEAST);
}

/// The inn's door opens and the party walks in.
/// It's VAULT_TO_VAULT_INTERIORS_SOUTHWEST, into the inn where the three
/// suspects stay.
static void EventScript_EnterInn(void) {
  Engine_AudioPlayCue(SE_DOOR_OPEN);
  Engine_MapGridAnimateCells(gVault_InnDoorCells, 44, 17);
  Vault_WalkThroughExit(216, 288, VAULT_TO_VAULT_INTERIORS_SOUTHWEST);
}

/// In Vault, the door VAULT_TO_VAULT_INTERIORS_EAST opens into the item shop,
/// an old couple's home too; the leader walks in behind the doorway's
/// foreground tiles.
static void EventScript_EnterItemShop(void) {
  struct ObjectEvent *objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  struct AnimationObject *sprite = objectEvent->sprite;

  Engine_AudioPlayCue(SE_DOOR_OPEN);
  Engine_MapGridAnimateCells(gVault_ItemShopDoorCells, 54, 13);
  MapGrid_CopyCellAttributes(33, 20, 1, 3, 23, 12);
  objectEvent->priorityFlags &= ~OBJ_EVENT_PRIORITY_AUTOMATIC;
  sprite->image.priority = 3;
  Vault_WalkThroughExit(376, 224, VAULT_TO_VAULT_INTERIORS_EAST);
}

/// The weapon and armor shop's door opens and the party walks in.
/// It's VAULT_TO_VAULT_INTERIORS_NORTH_NORTHEAST, entered as the item shop is.
static void EventScript_EnterWeaponAndArmorShop(void) {
  struct ObjectEvent *objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  struct AnimationObject *sprite = objectEvent->sprite;

  Engine_AudioPlayCue(SE_DOOR_OPEN);
  Engine_MapGridAnimateCells(gVault_WeaponAndArmorShopDoorCells, 49, 10);
  MapGrid_CopyCellAttributes(33, 20, 1, 3, 18, 10);
  objectEvent->priorityFlags &= ~OBJ_EVENT_PRIORITY_AUTOMATIC;
  sprite->image.priority = 3;
  Vault_WalkThroughExit(296, 176, VAULT_TO_VAULT_INTERIORS_NORTH_NORTHEAST);
}

/// In Vault, the door VAULT_TO_VAULT_INTERIORS_NORTHWEST opens into the
/// mayor's house, where Ivan waits.
static void EventScript_EnterMayorsHouse(void) {
  Engine_AudioPlayCue(SE_DOOR_OPEN);
  Engine_MapGridAnimateCells(gVault_MayorsHouseDoorCells, 38, 6);
  Vault_WalkThroughExit(120, 144, VAULT_TO_VAULT_INTERIORS_NORTHWEST);
}

#include "common/vault_map_cells.h"

/// In Vault, the jail door opens and the party walks in, drawn behind the
/// doorway (VAULT_TO_VAULT_JAIL, the jail's VAULT_JAIL_FROM_VAULT).
static void EventScript_EnterJail(void) {
  struct ObjectEvent *objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  struct AnimationObject *sprite = objectEvent->sprite;

  Engine_AudioPlayCue(SE_DOOR_OPEN);
  Engine_MapGridAnimateCells(gVault_JailDoorCells, 35, 9);
  MapGrid_CopyCellAttributes(33, 20, 1, 3, 4, 10);
  objectEvent->priorityFlags &= ~OBJ_EVENT_PRIORITY_AUTOMATIC;
  sprite->image.priority = 3;
  Vault_WalkThroughExit(72, 160, VAULT_TO_VAULT_JAIL);
}

/// In Vault, the party walks through a doorless entrance into the room with
/// the crate (VAULT_TO_VAULT_INTERIORS_WEST).
static void EventScript_EnterPassage(void) {
  Engine_AudioPlayCue(SE_MAP_EXIT);
  Vault_WalkThroughExit(152, 264, VAULT_TO_VAULT_INTERIORS_WEST);
}

/// Moves an object event through a jump and waits up to sixty frames for it to
/// settle.
static void Vault_JumpObjectEventToAndWait(s32 localId, s32 x, s32 z,
                                           s32 verticalSpeed) {
  struct ObjectEvent *objectEvent = ObjectEvent_GetById(localId);
  int frames;
  Engine_ObjectEventSetSpeed(localId, PIXELS(3), PIXELS(3) / 2);
  objectEvent->gravity = PIXELS(1) / 2;
  objectEvent->bounce = 0;
  objectEvent->velocityY = verticalSpeed;
  Engine_ObjectEventSetSpriteFlags(objectEvent, 0);
  Engine_ObjectEventMoveToAndWait(localId, x, z);
  Engine_ObjectEventSetPosition(localId, x << 16, z << 16);
  for (frames = 60; frames != 0; --frames) {
    Engine_TaskWait(1);
    if ((objectEvent->velocityY >> 16) == 0)
      break;
  }
  Engine_ObjectEventSetSpriteFlags(objectEvent, 1);
  objectEvent->gravity = PIXELS(1);
}

/// In Vault, Isaac rings the bell; the first time since the party came in by a
/// gate, Sap, the Venus Djinni hiding nearby, jumps out and comes to the bell
/// (FLAG_VAULT_DJINNI_CALLED_BY_BELL). No event here lets the party talk to
/// it; it joins in the town's later map (vault_revisit.c).
static void EventScript_RingBell(void) {
  Engine_ScriptStart();
  Script_PlayCue(SE_100);
  Engine_ScriptWait(40);
  if (GameFlag_IsSet(FLAG_VAULT_DJINNI_CALLED_BY_BELL) == 0) {
    ObjectEvent_SetAttachedEffect(LOCALID_SAP, ATTACHED_EFFECT_IN_FRONT |
                                                   ATTACHED_EFFECT_ONCE);
    Engine_ObjectEventJump(LOCALID_SAP, 4, 0);
    Engine_ScriptWait(12);
    Engine_ObjectEventJump(LOCALID_SAP, 4, 0);
    Engine_ScriptWait(20);
    Vault_JumpObjectEventToAndWait(LOCALID_SAP, 392, 104, PIXELS(7));
    Engine_ScriptWait(20);
    ObjectEvent_WalkToAndWait(LOCALID_SAP, 408, 104);
    ObjectEvent_WalkToAndWait(LOCALID_SAP, 408, 120);
    GameFlag_Set(FLAG_VAULT_DJINNI_CALLED_BY_BELL);
  }
  Engine_ScriptEnd();
}

/// Given a Bone, the dog runs to the hidden passage and jumps there.
/// It does so after the thieves are jailed, jumping three times
/// (FLAG_VAULT_DOG_SHOWN_PASSAGE).
static void EventScript_GiveBoneToDog(void) {
  PartyInventory_Discard(ITEM_BONE);
  Engine_ScriptStart();
  Engine_ScriptWait(10);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_VAULT_DOG, 2);
  ObjectEvent_SetSpeed(LOCALID_VAULT_DOG, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_VAULT_DOG, 216, 408);
  Engine_ScriptWait(10);
  ObjectEvent_FaceDirection(LOCALID_VAULT_DOG, FACING_SOUTH, 20);
  Engine_ObjectEventJump(LOCALID_VAULT_DOG, 6, 0);
  Engine_ScriptWait(30);
  Engine_ObjectEventJump(LOCALID_VAULT_DOG, 6, 0);
  Engine_ScriptWait(30);
  Engine_ObjectEventJump(LOCALID_VAULT_DOG, 6, 0);
  Engine_ScriptWait(30);
  ObjectEvent_WalkToAndWait(LOCALID_VAULT_DOG, 216, 392);
  Engine_ScriptWait(10);
  ObjectEvent_FaceDirection(LOCALID_VAULT_DOG, FACING_SOUTH, 20);
  GameFlag_Set(FLAG_VAULT_DOG_SHOWN_PASSAGE);
  Engine_ScriptEnd();
}

/// Arriving in Vault: restores the dog's and Sap's places, sets the two
/// children and the woman wanting the roof fixed stopping near the leader,
/// and, while Ivan waits in Vault for the party, has him greet the two and
/// rejoin.
s32 Map_Initialize(void) {
  s32 suspectsMindRead;
  struct ObjectEvent *objectEvent;
  s32 lowBit;
  s32 zero;

  // Once the party has reached Kalay (FLAG_KALAY_REACHED), the town is its
  // later map, MapId_VaultRevisit (vault_revisit.c).
  if (Engine_GameFlagIsSet(FLAG_KALAY_REACHED) != 0) {
    Engine_ScriptRequestWarp(VAULT_TO_VAULT_REVISIT);
  }
  // Keep the bridge deck after a battle, and restore the two children's
  // proximity callbacks.
  if (Engine_GameFlagIsSet(FLAG_MAP_TEMP_00) != 0) {
    MapGrid_CopyCellAttributes(55, 26, 4, 2, 23, 26);
  }
  // A still object on the well that shows nothing (vault_revisit.c's open
  // questions).
  OverlayObject_CreateConfiguredObject(PIXELS(128), 0, PIXELS(420), 223);
  lowBit = 1;
  zero = 0;
  objectEvent = ObjectEvent_GetById(LOCALID_BOY_ROUND_THE_WELL);
  objectEvent->timer = lowBit;
#ifdef UBFIX
  objectEvent->update = Vault_UpdateWalkingVillager;
#else
  objectEvent->update =
      (void (*)(union FieldObject *))Vault_UpdateWalkingVillager;
#endif
  objectEvent = ObjectEvent_GetById(LOCALID_GIRL_ROUND_THE_WELL);
  objectEvent->timer = zero;
#ifdef UBFIX
  objectEvent->update = Vault_UpdateWalkingVillager;
#else
  objectEvent->update =
      (void (*)(union FieldObject *))Vault_UpdateWalkingVillager;
#endif
  if (Engine_GameFlagIsSet(FLAG_VAULT_DOG_SHOWN_PASSAGE) != 0) {
    ObjectEvent_SetPosition(LOCALID_VAULT_DOG, PIXELS(216), PIXELS(392));
  }
  suspectsMindRead = Engine_GameFlagIsSet(FLAG_VAULT_SUSPECTS_MIND_READ);
  if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) == 0) {
    if ((lowBit & suspectsMindRead) != 0) {
      objectEvent = ObjectEvent_GetById(LOCALID_WOMAN_WANTING_THE_ROOF_FIXED);
#ifdef UBFIX
      objectEvent->update = Vault_UpdateRoofComplainerProximity;
#else
      objectEvent->update =
          (void (*)(union FieldObject *))Vault_UpdateRoofComplainerProximity;
#endif
    }
  }
  // Back through the gates while Ivan waits in Vault: "Welcome back, you two!
  // Let's go find the rod!"
  if (gGameState.warpDest <= VAULT_NORTH_UNUSED) {
    if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
      Engine_GameFlagClear(FLAG_VAULT_DJINNI_CALLED_BY_BELL);
      if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) == 0) {
        if (Engine_GameFlagIsSet(FLAG_VAULT_IVAN_JOINED_ROD_SEARCH) != 0) {
          Engine_ScriptStart();
          objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
          if (objectEvent != 0) {
            Engine_ObjectEventSetPosition(IVAN, objectEvent->x.fixed,
                                          objectEvent->z.fixed);
          }
          if (gGameState.warpDest == VAULT_FROM_WORLD_MAP_SOUTH) {
            ObjectEvent_SetPosition(IVAN, PIXELS(400), PIXELS(448));
          } else {
            ObjectEvent_SetPosition(IVAN, PIXELS(224), PIXELS(162));
          }
          Engine_ObjectEventFaceEachOther(IVAN, LOCALID_PARTY_LEADER, 0);
          Engine_ScriptOpenScreen();
          Engine_ScriptWaitForScreen();
          Engine_ScriptWait(30);
          Engine_ObjectEventRunRepeatedMotion(IVAN, 2);
          Engine_ScriptSetMessage(MsgVaultWelcomeBackTwo);
          Engine_ScriptShowMessageAndWait(IVAN, 0, 20);
          Engine_ObjectEventSetAnimationAndWait(LOCALID_PARTY_LEADER,
                                                ANIMATION_NOD);
          ObjectEvent_SetSpeed(IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
          Engine_ObjectEventSetAnimation(IVAN, ANIMATION_WALK);
          objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
          if (objectEvent != 0) {
            Engine_ObjectEventSetDestination(IVAN, objectEvent->x.part.pixel,
                                             objectEvent->z.part.pixel);
          }
          Engine_ObjectEventWaitForMove(IVAN);
          Engine_ObjectEventSetPosition(IVAN, 0, 0);
          FieldScript_PrepareObjectAndApplyValue(IVAN, 0);
          Engine_ScriptEnd();
        }
      }
    }
  }
  if (Engine_GameFlagIsSet(FLAG_VAULT_DJINNI_CALLED_BY_BELL) != 0) {
    ObjectEvent_SetPosition(LOCALID_SAP, PIXELS(408), PIXELS(120));
  }
  // The same welcome out of the Vault Sanctum while Ivan is out of the party.
  if (gGameState.warpDest == VAULT_FROM_SANCTUM) {
    if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) == 0) {
      if (Engine_GameFlagIsSet(FLAG_VAULT_IVAN_JOINED_ROD_SEARCH) != 0) {
        if (Engine_GameFlagIsSet(FLAG_IN_PARTY(IVAN)) == 0) {
          Engine_ScriptStart();
          ObjectEvent_SetPosition(IVAN, PIXELS(40), PIXELS(310));
          Engine_ObjectEventFaceEachOther(IVAN, LOCALID_PARTY_LEADER, 0);
          Engine_ScriptOpenScreen();
          Engine_ScriptWaitForScreen();
          Engine_ScriptWait(30);
          Engine_ObjectEventRunRepeatedMotion(IVAN, 2);
          Engine_ScriptSetMessage(MsgVaultWelcomeBackTwo);
          Engine_ScriptShowMessageAndWait(IVAN, 0, 20);
          Engine_ObjectEventSetAnimationAndWait(LOCALID_PARTY_LEADER,
                                                ANIMATION_NOD);
          ObjectEvent_SetSpeed(IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
          Engine_ObjectEventSetAnimation(IVAN, ANIMATION_WALK);
          objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
          if (objectEvent != 0) {
            Engine_ObjectEventSetDestination(IVAN, objectEvent->x.part.pixel,
                                             objectEvent->z.part.pixel);
          }
          Engine_ObjectEventWaitForMove(IVAN);
          Engine_ObjectEventSetPosition(IVAN, 0, 0);
          FieldScript_PrepareObjectAndApplyValue(IVAN, 0);
          Engine_ScriptEnd();
        }
      }
    }
    Engine_GameFlagClear(FLAG_SHOW_LOCATION_NAME);
  } else {
    if (gGameState.warpDest == VAULT_FROM_VAULT_INTERIORS_WEST) {
      if (Engine_GameFlagIsSet(FLAG_VAULT_THIEVES_JAILED) != 0) {
        Engine_ObjectEventSetPosition(LOCALID_YOUNG_MAN_FIXING_THE_ROOF, 0, 0);
      }
    }
  }
  return 0;
}

#include "common/object_distance.h"
#include "common/vault_range.h"

static void Vault_ShowMessageAndWait(s32 localId, s32 delay) {
  Engine_ScriptShowMessage(localId, 0);
  Engine_ScriptWait(delay);
}

static void Vault_FaceObjectEventAndWait(s32 localId, s32 other, s32 delay) {
  Engine_ObjectEventFaceEachOther(localId, other, 0);
  Engine_ScriptWait(delay);
}

/// Applies drift and gradually damps both rates toward zero.
/// This custom update uses speed/acceleration as its two drift rates and
/// mirrors the resulting position into the object event's target coordinates.
static void Vault_UpdateDriftingEffect(union FieldObject *fieldObject) {
  struct ObjectEvent *effect = (struct ObjectEvent *)fieldObject;
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
  struct ObjectEvent *anchor;
  struct ObjectEvent *effect;
  struct AnimationObject *sprite;
  s32 x;
  s32 z;
  u8 zero;

  anchor = (struct ObjectEvent *)ObjectEvent_GetById(LOCALID_VAULT_DOG);
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
    effect = (struct ObjectEvent *)Engine_ObjectCreate(0xac, x, baseY, z);
  }
  if (effect == NULL)
    return;
  sprite = effect->sprite;
  switch (Engine_RandomNext() & 1) {
  case 1:
    ObjectEvent_SetMode(effect, 3);
    Engine_ObjectSetScript(effect, gVault_DriftScriptA);
    break;
  default:
    ObjectEvent_SetMode(effect, 2);
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

// The three scripts of gEffectScripts, which FieldEffect_Spawn picks by the low
// bits of its flags: each holds its effect for about 24, 46 or 128 frames,
// then frees it.
s32 gVault_ShortEffectScript[] = {
    SC_WAIT(1),           SC_LOOP(22, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gVault_MediumEffectScript[] = {
    SC_WAIT(1),           SC_LOOP(44, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gVault_LongEffectScript[] = {
    SC_WAIT(1),           SC_LOOP(126, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),           SC_FREE,
};

const s32 *gEffectScripts[] = {
    gVault_ShortEffectScript,
    gVault_MediumEffectScript,
    gVault_LongEffectScript,
};

/// LOCALID_WOMAN_WANTING_THE_ROOF_FIXED's search for the young man fixing the
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

/// LOCALID_BOY_ROUND_THE_WELL's walk, round eight points for as long as the
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

/// LOCALID_GIRL_ROUND_THE_WELL's walk round the same eight points, half a
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

struct MapWarpDest gVault_WarpDests[] = {
    {WARP_DEST_DEFAULT, CONDITION_ALWAYS, 223, 0, 135, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {VAULT_FROM_WORLD_MAP_SOUTH, CONDITION_ALWAYS, 400, 0, 472, FACING_NORTH, 0,
     -1, -1, -1, -1, 0},
    {VAULT_NORTH_UNUSED, CONDITION_ALWAYS, 224, 0, 138, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
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
    {MAP_TABLE_END},
};

u32 gVault_Warps[] = {
    MAP_WARPS(Vault),
    MAP_WARP(VAULT_TO_WORLD_MAP_SOUTH, WorldMap, WORLD_MAP_FROM_VAULT_SOUTH),
    MAP_WARP(VAULT_TO_WORLD_MAP_NORTH, WorldMap, WORLD_MAP_FROM_VAULT_NORTH),
    MAP_WARP(VAULT_TO_VAULT_INTERIORS_EAST_SOUTHEAST, VaultInteriors,
             VAULT_INTERIORS_FROM_VAULT_EAST_SOUTHEAST),
    MAP_WARP(VAULT_TO_VAULT_INTERIORS_SOUTHEAST, VaultInteriors,
             VAULT_INTERIORS_FROM_VAULT_SOUTHEAST),
    MAP_WARP(VAULT_TO_VAULT_INTERIORS_SOUTHWEST, VaultInteriors,
             VAULT_INTERIORS_FROM_VAULT_SOUTHWEST),
    MAP_WARP(VAULT_TO_VAULT_INTERIORS_EAST, VaultInteriors,
             VAULT_INTERIORS_FROM_VAULT_EAST),
    MAP_WARP(VAULT_TO_VAULT_INTERIORS_NORTH_NORTHEAST, VaultInteriors,
             VAULT_INTERIORS_FROM_VAULT_NORTH_NORTHEAST),
    MAP_WARP(VAULT_TO_VAULT_INTERIORS_NORTHWEST, VaultInteriors,
             VAULT_INTERIORS_FROM_VAULT_NORTHWEST),
    MAP_WARP(VAULT_TO_SANCTUM, Sanctum, SANCTUM_FROM_VAULT),
    MAP_WARP(VAULT_TO_VAULT_JAIL, VaultJail, VAULT_JAIL_FROM_VAULT),
    MAP_WARP(VAULT_TO_VAULT_INTERIORS_WEST, VaultInteriors,
             VAULT_INTERIORS_FROM_VAULT_WEST),
    MAP_WARP(VAULT_TO_VAULT_REVISIT, VaultRevisit, VAULT_REVISIT_FROM_VAULT),
    MAP_WARPS_END,
};

// clang-format off
struct ObjectEventTemplate gVault_ObjectEventTemplates[] = {
    { IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(381), 0, PIXELS(342), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(280), 0, PIXELS(396), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(264), 0, PIXELS(150), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(244), 0, PIXELS(204), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(124), 0, PIXELS(196), FACING_SOUTHEAST, TALK_FACE_PARTY, 0 },
    { SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(244), 0, PIXELS(300), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, (s32)gVault_BoyRoundTheWellRoute,
        PIXELS(132), 0, PIXELS(380), FACING_WEST, TALK_KEEP_FACING, 0 },
    { SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, (s32)gVault_GirlRoundTheWellRoute,
        PIXELS(132), 0, PIXELS(420), FACING_EAST, TALK_KEEP_FACING, 0 },
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(332), 0, PIXELS(412), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_PERSON_IN_PURPLE, FLAG_VAULT_THIEVES_ATTACKED, MOVEMENT_TYPE_STAND,
        PIXELS(92), 0, PIXELS(284), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(92), 0, PIXELS(296), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_DOG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(216), 0, PIXELS(392), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, FLAG_VAULT_SUSPECTS_MIND_READ, MOVEMENT_TYPE_STAND,
        PIXELS(152), 0, PIXELS(296), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, IF_FLAG_SET(FLAG_VAULT_SUSPECTS_MIND_READ), (s32)gVault_WomanWantingTheRoofFixedRoute,
        PIXELS(168), 0, PIXELS(312), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_IVY, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { VENUS_DJINNI, FLAG_DJINNI_0 + DJINNI_SAP, MOVEMENT_TYPE_STAND,
        PIXELS(344), 0, PIXELS(96), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gVault_ObjectEventTemplatesAfterThieves[] = {
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(381), 0, PIXELS(342), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(280), 0, PIXELS(396), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(264), 0, PIXELS(150), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(244), 0, PIXELS(204), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(124), 0, PIXELS(196), FACING_SOUTHEAST, TALK_FACE_PARTY, 0 },
    { SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(244), 0, PIXELS(300), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, (s32)gVault_BoyRoundTheWellRoute,
        PIXELS(132), 0, PIXELS(380), FACING_WEST, TALK_KEEP_FACING, 0 },
    { SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, (s32)gVault_GirlRoundTheWellRoute,
        PIXELS(132), 0, PIXELS(420), FACING_EAST, TALK_KEEP_FACING, 0 },
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(332), 0, PIXELS(412), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_BLANK, FLAG_VAULT_THIEVES_ATTACKED, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(92), 0, PIXELS(296), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_DOG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(216), 0, PIXELS(440), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(152), 0, PIXELS(296), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, FLAG_VAULT_THIEVES_JAILED, (s32)gVault_WomanWantingTheRoofFixedRoute,
        PIXELS(168), 0, PIXELS(312), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_IVY, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
    { VENUS_DJINNI, FLAG_DJINNI_0 + DJINNI_SAP, MOVEMENT_TYPE_STAND,
        PIXELS(344), 0, PIXELS(96), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { MAP_TABLE_END },
};

struct MapEvent gVault_Events[] = {
    { MAP_EVENT_COORD, VAULT_CELL_SOUTH_GATE, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_IvanStaysBehind) },
    { MAP_EVENT_COORD, VAULT_CELL_NORTH_GATE, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_IvanStaysBehind) },
    { MAP_EVENT_COORD, VAULT_CELL_BRIDGE_END, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_RaisePathToBridge) },
    { MAP_EVENT_COORD, VAULT_CELL_WEST_OF_BRIDGE, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_LowerPathUnderBridge) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), VAULT_CELL_GEM_OWNERS_HOUSE_DOOR, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterGemOwnersHouse) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), VAULT_CELL_CHILDS_HOUSE_DOOR, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterChildsHouse) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), VAULT_CELL_INN_DOOR, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterInn) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), VAULT_CELL_ITEM_SHOP_DOOR, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterItemShop) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), VAULT_CELL_WEAPON_AND_ARMOR_SHOP_DOOR, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterWeaponAndArmorShop) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), VAULT_CELL_MAYORS_HOUSE_DOOR, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterMayorsHouse) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), VAULT_CELL_SANCTUM_DOORWAY, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_OpenDoorwayAndEnter) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), VAULT_CELL_JAIL_DOOR, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterJail) },
    { MAP_EVENT_COORD, VAULT_CELL_WEST_ENTRANCE, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterPassage) },
    { MAP_EVENT_TALK, LOCALID_WELCOMING_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgVaultWelcomeToVaultTheTownAt) },
    { MAP_EVENT_TALK, LOCALID_ERUPTION_WITNESS, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_AskAboutEruption) },
    { MAP_EVENT_TALK, LOCALID_VILLAGER_ON_HAMMET, CONDITION_ALWAYS, MSGNO(MsgVaultMasterHammetAWealthyLandownerAnd) },
    { MAP_EVENT_TALK, LOCALID_WOMAN_ON_STEALING, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkOfEruptionThefts) },
    { MAP_EVENT_TALK, LOCALID_OLD_MAN_ON_HAMMETS_SERVANT, CONDITION_ALWAYS, MSGNO(MsgVaultMasterHammetsServantCantReturnTo) },
    { MAP_EVENT_TALK, LOCALID_OLD_WOMAN_ON_THE_TRAVELERS, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_AskAboutFleeingTravelers) },
    { MAP_EVENT_TALK, LOCALID_BOY_ROUND_THE_WELL, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_PityIvan) },
    { MAP_EVENT_TALK, LOCALID_GIRL_ROUND_THE_WELL, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkOfHammetsCaravan) },
    { MAP_EVENT_TALK, LOCALID_STRONGMAN_ON_WATCH, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_StrongmanOnWatchSuspectsOutsiders) },
    { MAP_EVENT_TALK, LOCALID_THIEF_IN_TOWN, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ThiefDeniesTheft) },
    { MAP_EVENT_TALK, LOCALID_STRANDED_TRAVELER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_AskAboutAdventure) },
    { MAP_EVENT_TALK, LOCALID_VAULT_DOG, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToDog) },
    { MAP_EVENT_TALK, LOCALID_YOUNG_MAN_FIXING_THE_ROOF, CONDITION_ALWAYS, MSGNO(MsgVaultAFieryRockBurntAHole) },
    { MAP_EVENT_TALK, LOCALID_WOMAN_WANTING_THE_ROOF_FIXED, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LookForRoofer) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WELCOMING_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgVaultISupposeTheseChildrenCantGo) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ERUPTION_WITNESS, CONDITION_ALWAYS, MSGNO(MsgVaultMtAlephsEruptionWasBeyondDescription) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_VILLAGER_ON_HAMMET, CONDITION_ALWAYS, MSGNO(MsgVaultWhyDoesAWealthyManLike) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WOMAN_ON_STEALING, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ReadMindOnTheft) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_OLD_MAN_ON_HAMMETS_SERVANT, CONDITION_ALWAYS, MSGNO(MsgVaultPoorIvanButTheRodMay) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_OLD_WOMAN_ON_THE_TRAVELERS, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ReadMindOnTravelers) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BOY_ROUND_THE_WELL, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ReadMindOnIvan) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_GIRL_ROUND_THE_WELL, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ReadMindOnWealth) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STRONGMAN_ON_WATCH, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ReadMindOnSuspects) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_THIEF_IN_TOWN, CONDITION_ALWAYS, MSGNO(MsgVaultHmphItLooksLikeTheyreOnto) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STRANDED_TRAVELER, CONDITION_ALWAYS, MSGNO(MsgVaultIWasJustAboutToLeave) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_VAULT_DOG, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ReadDogMind) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_YOUNG_MAN_FIXING_THE_ROOF, CONDITION_ALWAYS, MSGNO(MsgVaultFixTheRoofWhyPayFor) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WOMAN_WANTING_THE_ROOF_FIXED, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ReadWomanWantingTheRoofFixedMind) },
    { BG_EVENT(BG_EVENT_UNNAMED), VAULT_CELL_BELL, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_RingBell) },
    { BG_EVENT(BG_EVENT_UNNAMED), VAULT_CELL_WELL, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_PeerIntoWell) },
    { BG_EVENT(BG_EVENT_WOODEN_BOX), VAULT_CELL_WOODEN_BOX, FLAG_VAULT_BOX_NUT, FIND_ITEM(ITEM_NUT) },
    { MAP_EVENTS_END },
};

struct MapEvent gVault_EventsAfterThieves[] = {
    { MAP_EVENT_COORD, VAULT_CELL_SOUTH_GATE, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_IvanStaysBehind) },
    { MAP_EVENT_COORD, VAULT_CELL_NORTH_GATE, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_IvanStaysBehind) },
    { MAP_EVENT_COORD, VAULT_CELL_BRIDGE_END, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_RaisePathToBridge) },
    { MAP_EVENT_COORD, VAULT_CELL_WEST_OF_BRIDGE, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_LowerPathUnderBridge) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), VAULT_CELL_GEM_OWNERS_HOUSE_DOOR, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterGemOwnersHouse) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), VAULT_CELL_CHILDS_HOUSE_DOOR, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterChildsHouse) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), VAULT_CELL_INN_DOOR, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterInn) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), VAULT_CELL_ITEM_SHOP_DOOR, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterItemShop) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), VAULT_CELL_WEAPON_AND_ARMOR_SHOP_DOOR, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterWeaponAndArmorShop) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), VAULT_CELL_MAYORS_HOUSE_DOOR, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterMayorsHouse) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), VAULT_CELL_SANCTUM_DOORWAY, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_OpenDoorwayAndEnter) },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), VAULT_CELL_JAIL_DOOR, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterJail) },
    { MAP_EVENT_COORD, VAULT_CELL_WEST_ENTRANCE, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_EnterPassage) },
    { MAP_EVENT_TALK, LOCALID_WELCOMING_YOUNG_MAN, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_AskAboutLeavingAgain) },
    { MAP_EVENT_TALK, LOCALID_ERUPTION_WITNESS, CONDITION_ALWAYS, MSGNO(MsgVaultSmokeIsStillRisingFromMt) },
    { MAP_EVENT_TALK, LOCALID_VILLAGER_ON_HAMMET, CONDITION_ALWAYS, MSGNO(MsgVaultMasterHammetTheWealthyMerchantI) },
    { MAP_EVENT_TALK, LOCALID_WOMAN_ON_STEALING, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_PraiseThievesCapture) },
    { MAP_EVENT_TALK, LOCALID_OLD_MAN_ON_HAMMETS_SERVANT, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkAboutIvan) },
    { MAP_EVENT_TALK, LOCALID_OLD_WOMAN_ON_THE_TRAVELERS, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkOfDamagedRoads) },
    { MAP_EVENT_TALK, LOCALID_BOY_ROUND_THE_WELL, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_AskWhetherIvanWentToLunpa) },
    { MAP_EVENT_TALK, LOCALID_GIRL_ROUND_THE_WELL, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ThankForCapturingThieves) },
    { MAP_EVENT_TALK, LOCALID_STRONGMAN_ON_WATCH, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_AskAboutPrisoners) },
    { MAP_EVENT_TALK, LOCALID_STRANDED_TRAVELER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_AskWhoCapturedThieves) },
    { MAP_EVENT_TALK, LOCALID_VAULT_DOG, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToDogAfterCapture) },
    { MAP_EVENT_TALK, LOCALID_YOUNG_MAN_FIXING_THE_ROOF, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ThankBeforeDeparture) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WELCOMING_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgVaultYouveMadeOurTownPeacefulAt) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ERUPTION_WITNESS, CONDITION_ALWAYS, MSGNO(MsgVaultNowMtAlephHasCalmedDown) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_VILLAGER_ON_HAMMET, CONDITION_ALWAYS, MSGNO(MsgVaultOfAllThePlacesWhyWould) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WOMAN_ON_STEALING, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ReadMindOnTheft) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_OLD_MAN_ON_HAMMETS_SERVANT, CONDITION_ALWAYS, MSGNO(MsgVaultNobodyGetsIntoLunpaExceptFor) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_OLD_WOMAN_ON_THE_TRAVELERS, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ReadMindOnTravelers) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BOY_ROUND_THE_WELL, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ReadMindOnIvan) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_GIRL_ROUND_THE_WELL, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ReadMindOnWealth) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STRONGMAN_ON_WATCH, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ReadMindOnSuspects) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STRANDED_TRAVELER, CONDITION_ALWAYS, MSGNO(MsgVaultThereIsAlwaysAFightWhen) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_VAULT_DOG, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ReadDogMind) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_YOUNG_MAN_FIXING_THE_ROOF, CONDITION_ALWAYS, MSGNO(MsgVaultSniffAhTheSmellOfCooking) },
    { BG_EVENT(BG_EVENT_UNNAMED), VAULT_CELL_BELL, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_RingBell) },
    { BG_EVENT(BG_EVENT_UNNAMED), VAULT_CELL_WELL, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_PeerIntoWell) },
    { USE_ITEM_ON_OBJ_EVENT(ITEM_BONE), LOCALID_VAULT_DOG, FLAG_VAULT_DOG_SHOWN_PASSAGE, EVENT_SCRIPT(EventScript_GiveBoneToDog) },
    { BG_EVENT(BG_EVENT_WOODEN_BOX), VAULT_CELL_WOODEN_BOX, FLAG_VAULT_BOX_NUT, FIND_ITEM(ITEM_NUT) },
    { MAP_EVENTS_END },
};
// clang-format on

// Each door's opening, two frames of map cells, named for the room it opens
// into (EventScript_EnterGemOwnersHouse and the rest).
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
    SC_FREE,
};

s32 gVault_DriftScriptB[] = {
    SC_WAIT(1),
    SC_LOOP(38, SC_TOP),
    SC_FREE,
};
