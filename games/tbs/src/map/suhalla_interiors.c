/// @file
/// The buildings of Suhalla: one map (MapId_SuhallaInteriors) whose rooms
/// are told apart by the warp destination the party arrives at, and whose area
/// name follows it:
///
/// - Suhalla: the houses, the shop and the inn behind the village's doors,
///   whose warps from the village (SUHALLA_TO_INN, SUHALLA_TO_HOUSE_WEST,
///   SUHALLA_TO_HOUSE_NORTHEAST and SUHALLA_TO_HOUSE_EAST) lead to the
///   warp destinations of the same numbers. Doorways join the inn to its two
///   guest rooms, and a ladder to its loft, climbed through two warp regions.
/// - Suhalla Sanctum: behind the village's wide door (SUHALLA_TO_SANCTUM),
///   with SUHALLA_INTERIORS_SANCTUM_NORTH as its second warp destination.
///
/// The rooms' people and talk come in two sets, each an object event template
/// table and a map event table, chosen by FLAG_BABI_SPOKE_AFTER_COLOSSO. Before
/// Babi's audience after Colosso the village talks of Lalivero across the Nol
/// River, out of reach under Tolbi's rule; after it, two of the soldiers who
/// were escorting Sheba home lie wounded in a house, travelers the sandstorms
/// blew back fill the inn, and an old woman half remembers a saying about the
/// death storms. The two tables agree up to LOCALID_HEALER; the travelers stand
/// off the map until the audience.
///
/// Stepping into the wounded soldiers' house (the coord event on metatile
/// behavior 50) plays EventScript_IodemQuestionsWoundedSoldiers once, while
/// Iodem travels with the party and has not yet reached Babi Lighthouse. It
/// sets FLAG_IODEM_MET_WOUNDED_SOLDIERS either way, which Iodem's lines in the
/// Suhalla Desert and at Suhalla Gate read. The merchant, the innkeeper and
/// the Healer serve a leader who faces north across their counters, and
/// otherwise talk by the same flag. Searching the ovens finds a Smoke Bomb
/// once (FLAG_SUHALLA_INTERIORS_OVEN_SMOKE_BOMB) and shows what is cooking.
///
/// Unused: the cactus steak line at the Smoke Bomb's oven, whose item row
/// always answers first, and the warp destination
/// SUHALLA_INTERIORS_HOUSE_WEST_UNUSED, to which no warp leads.
///
/// Open questions:
/// - None.

#include "game_state.h"
#include "script_layer.h"
#include "unit/import.h"
#include "unit/map/suhalla_interiors.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "field_service.h"
#include "text/msg_ids.h"
#include "types.h"
#include "ability_ids.h"
#include "map_tables.h"
#include "item_ids.h"
#include "flags.h"
#include "songs.h"
#include "field_runtime.h"

/// The object events, numbering the rows of
/// gSuhallaInteriorsObjectEventTemplates and of
/// gSuhallaInteriorsObjectEventTemplatesAfterAudience from
/// LOCALID_FIRST_PLACED; the two tables agree up to LOCALID_HEALER. Where an
/// object event's talk changes after Babi's audience, the note quotes it
/// before.
enum SuhallaInteriorsLocalId {
  /// A little girl with orange pigtails (SPRITE_GIRL_WITH_PIGTAILS), the
  /// daughter of the man who aids anyone in trouble. "My father loves people
  /// so much, he can't help but aid anyone in trouble."
  /// ★ Named from the game: "I guess I'm just my father's child!"
  /// (MsgSuhallaInteriorsWhenISeePeopleInTrouble)
  LOCALID_FATHERS_DAUGHTER = LOCALID_FIRST_PLACED,
  /// The man who aids anyone in trouble, a man with brown hair
  /// (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR), in whose house the wounded soldiers
  /// lie after the audience. "Let me know if you have any problems. I'll do
  /// anything I can to help."
  /// ★ Named from the game: "I want to help people so badly, I can't sit idly
  /// by!" (MsgSuhallaInteriorsIWantToHelpPeopleSo)
  LOCALID_FATHER_WHO_WANTS_TO_HELP,
  /// A bent old man with a cane (SPRITE_OLD_MAN_WITH_CANE). After the
  /// audience: "Do not approach the large cyclones in the middle of the
  /// Suhalla."
  /// ★ Named from the game: "Tolbi probably ignores small villages like
  /// Suhalla" (MsgSuhallaInteriorsTolbiProbablyIgnoresSmallVillagesLike)
  LOCALID_OLD_MAN_IGNORED_BY_TOLBI,
  /// In the old woman's sprite (SPRITE_OLD_WOMAN, as in vale_gate.c). "If you
  /// head east across
  /// Suhalla, Lalivero is so close you can see it across the Nol River..."
  /// After the audience she half remembers a saying about the death storms.
  /// ★ Named from what the player sees: a white-haired old woman in a dark
  /// dress (its render)
  LOCALID_OLD_WOMAN,
  /// A young woman with a brown ponytail (SPRITE_WOMAN_WITH_PONYTAIL).
  /// ★ Named from the game: "Ours is the only store in Suhalla."
  /// (MsgSuhallaOursOnlyStore)
  LOCALID_MERCHANT,
  /// A man with dark hair (SPRITE_MAN_WITH_DARK_HAIR), who opens the inn across
  /// its counter (EventScript_TalkToInnkeeper).
  /// ★ Named from the game: "Innkeeper" (MsgDebugGlyphName)
  LOCALID_INNKEEPER,
  /// A woman at the inn, with red-brown hair in a white apron
  /// (SPRITE_WOMAN_IN_PINK_APRON), the sprite of Kalay's innkeeper's wife.
  /// ★ Named from the game: "I wonder if any customers can get through those
  /// sandstorms..." (MsgSuhallaInteriorsIWonderIfAnyCustomersCan)
  LOCALID_INN_WOMAN,
  /// Travelers the sandstorms blew back, out of sight until the audience: two
  /// stocky strong men (SPRITE_STRONGMAN). "We
  /// went into the Suhalla to go build the lighthouse, but the storms caught
  /// us."
  /// ★ Named from the game: "We went into the Suhalla to go build the
  /// lighthouse" (MsgSuhallaInteriorsWeWentIntoTheSuhallaTo)
  LOCALID_LIGHTHOUSE_WORKER,
  /// "The sandstorms are so bad, we've given up trying to reach Babi
  /// Lighthouse."
  /// ★ Named from the game: "We went into the Suhalla to go build the
  /// lighthouse" (MsgSuhallaInteriorsWeWentIntoTheSuhallaTo)
  LOCALID_SECOND_LIGHTHOUSE_WORKER,
  /// A stout man with brown hair (SPRITE_STOUT_MAN).
  /// ★ Named from the game: "I set out for Babi Lighthouse to open a business
  /// there, but then I was blown back here again!" (MsgSuhallaSupposeFolkBlown)
  LOCALID_STRANDED_TRAVELER,
  /// A bearded man in a long robe (SPRITE_BEARDED_MAN_IN_LONG_ROBE), who keeps
  /// Suhalla Sanctum (EventScript_TalkToHealer).
  /// ★ Named from the game: "Healer" (MsgDebugGlyphName)
  LOCALID_HEALER,
  /// ★ Named from the game: "Iodem..." (MsgSuhallaIodem)
  LOCALID_IODEM,
  /// The wounded soldiers, lying in the beds of a house after Babi's audience.
  /// ★ Named from the game: "These old soldiers must rest until their wounds
  /// heal." (MsgSuhallaInteriorsTheseOldSoldiersMustRestUntil)
  LOCALID_SOLDIER_SOUTH,
  /// ★ Named from the game: "These old soldiers must rest until their wounds
  /// heal." (MsgSuhallaInteriorsTheseOldSoldiersMustRestUntil)
  LOCALID_SOLDIER_NORTH
};

enum { SHOP_SUHALLA = 31, INN_SUHALLA = 10 };

const struct MapWarpDest *Map_GetWarpDests(void) {
  return gSuhallaInteriorsWarpDests;
}

const struct MapWarpRegion *Map_GetWarpRegions(void) {
  return gSuhallaInteriorsRegions;
}

const u32 *Map_GetWarps(void) { return gSuhallaInteriorsWarps; }

/// Suhalla's people, joined after Babi's audience by Iodem, the wounded
/// soldiers and the travelers the sandstorms brought.
const struct ObjectEventTemplate *Map_GetObjectEventTemplates(void) {
  if (Engine_GameFlagIsSet(FLAG_BABI_SPOKE_AFTER_COLOSSO) != 0) {
    return gSuhallaInteriorsObjectEventTemplatesAfterAudience;
  }
  return gSuhallaInteriorsObjectEventTemplates;
}

/// Suhalla's merchant. Spoken to facing north, across the counter, the
/// merchant opens the shop; otherwise says Suhalla's is the only store, or,
/// after Babi's audience, asks whether the party is surprised to find so few
/// shops.
static void EventScript_TalkToMerchant(s32 localId) {
  u16 facing =
      (ObjectEvent_GetById(LOCALID_PARTY_LEADER)->facing + 0x2000) & ~0x3fff;
  if (facing == FACING_NORTH) {
    Engine_ShopOpen(SHOP_SUHALLA, localId);
  } else if (Engine_GameFlagIsSet(FLAG_BABI_SPOKE_AFTER_COLOSSO)) {
    s32 question = MsgSuhallaArentSurprisedFind;
    Engine_ScriptSetMessage(question);
    Engine_ScriptOpenMessage(localId, 0);
    if (Engine_ScriptChooseYesNo(LOCALID_PARTY_LEADER, 0) == 0) {
      Engine_ScriptWait(10);
      Engine_ScriptSetMessage(question + 1);
    } else {
      Engine_ScriptSetMessage(question + 2);
    }
    Engine_ScriptShowMessage(localId, 0);
  } else {
    Engine_ScriptSetMessage(MsgSuhallaOursOnlyStore);
    Engine_ScriptShowMessage(localId, 0);
  }
}

/// Suhalla's innkeeper. Spoken to facing north, across the counter, the
/// innkeeper offers a room; otherwise has nothing to do until Tolbi's festival
/// ends, or, after Babi's audience, has been busy since the sandstorms began.
static void EventScript_TalkToInnkeeper(s32 localId) {
  u16 facing =
      (ObjectEvent_GetById(LOCALID_PARTY_LEADER)->facing + 0x2000) & ~0x3fff;
  if (facing == FACING_NORTH) {
    Engine_InnCheckIn(INN_SUHALLA, localId);
  } else if (Engine_GameFlagIsSet(FLAG_BABI_SPOKE_AFTER_COLOSSO)) {
    Engine_ScriptSetMessage(MsgSuhallaBusyEverSince);
    Engine_ScriptShowMessage(localId, 0);
  } else {
    Engine_ScriptSetMessage(MsgSuhallaSighNothinMaybe);
    Engine_ScriptShowMessage(localId, 0);
  }
}

/// Suhalla Sanctum's Healer. Spoken to facing north, across the counter, the
/// Healer offers the sanctum's services; otherwise talks of Lalivero under
/// Tolbi's rule, or, after Babi's audience, wonders why there are so many
/// sandstorms.
static void EventScript_TalkToHealer(s32 localId) {
  u16 facing =
      (ObjectEvent_GetById(LOCALID_PARTY_LEADER)->facing + 0x2000) & ~0x3fff;
  if (facing == FACING_NORTH) {
    Engine_ShopConfirmAct(localId);
  } else if (Engine_GameFlagIsSet(FLAG_BABI_SPOKE_AFTER_COLOSSO)) {
    Engine_ScriptSetMessage(MsgSuhallaWonderWhySandstorms);
    Engine_ScriptShowMessage(localId, 0);
  } else {
    Engine_ScriptSetMessage(MsgSuhallaGetSickThinkingAboutLalivero);
    Engine_ScriptShowMessage(localId, 0);
  }
}

/// A traveler the storms blew back asks whether the party was blown here too.
/// That's in Suhalla after Babi's audience, and the question is whether the
/// party was on the way to Babi Lighthouse: yes, and the traveler means to give
/// up and follow the northern mountain back to Tolbi; no, and tells of setting
/// out to open a business at the lighthouse.
static void EventScript_AskIfBlownHere(s32 localId) {
  s32 question = MsgSuhallaSupposeFolkBlown;
  Engine_ScriptSetMessage(question);
  Engine_ScriptOpenMessage(localId, 0);
  if (Engine_ScriptChooseYesNo(LOCALID_PARTY_LEADER, 0) == 0) {
    Engine_ScriptWait(10);
    Engine_ScriptSetMessage(question + 1);
  } else {
    Engine_ScriptSetMessage(question + 2);
  }
  Engine_ScriptShowMessage(localId, 0);
}

/// The doors of Suhalla and Suhalla Sanctum, and the doors between the inn and
/// its guest rooms: the party leaves by the touched warp.
static void EventScript_LeaveByWarp(void) {
  struct EventWork *state = gFieldWork;
  s16 *touchedTrigger = (s16 *)&state->coordEvent;

  Engine_ScriptRequestWarp(*touchedTrigger);
  Engine_AudioPlayCue(SE_MAP_EXIT);
}

/// In Suhalla after Babi's audience, stepping into the house where the two
/// wounded soldiers lie (metatile behavior 50), once: Iodem steps out of the
/// party and questions them. "Iodem..." They were escorting Sheba to Lalivero
/// when a sandstorm hit them, and tell of others who stopped the cyclones with
/// water and slew the monsters causing them. Iodem asks whether these are the
/// ones the party follows, resolves either way to search the desert for Sheba
/// and pursue them, and rejoins the party. Sets
/// FLAG_IODEM_MET_WOUNDED_SOLDIERS either way; once Iodem has reached Babi
/// Lighthouse (FLAG_BABI_LIGHTHOUSE_IODEM_ARRIVED), the scene does not play.
static void EventScript_IodemQuestionsWoundedSoldiers(void) {
  struct ObjectEvent *leader;
  GameFlag_Set(FLAG_IODEM_MET_WOUNDED_SOLDIERS);
  if (GameFlag_IsSet(FLAG_BABI_LIGHTHOUSE_IODEM_ARRIVED) == 0) {
    Script_PlaySong(MUS_030);
    Script_Start();
    // Frame the speakers and bring the leader over before the dialogue.
    FieldCamera_MoveTo(PIXELS(368), -1, PIXELS(104), 1);
    ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 368, 160);
    ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
    FieldMotion_LaunchFromFocusedObject(LOCALID_IODEM, 0, -16, FACING_NORTH);
    ObjectEvent_WaitForMove(LOCALID_IODEM);
    FieldCamera_WaitForMove();
    Script_SetMessage(MsgSuhallaIodem);
    Script_Wait(10);
    ObjectEvent_RunRepeatedMotion(LOCALID_SOLDIER_SOUTH, 2);
    Script_Wait(20);
    Script_ShowMessage(LOCALID_SOLDIER_SOUTH, 0);
    Script_Wait(10);
    ObjectEvent_RunRepeatedMotion(LOCALID_IODEM, 2);
    Script_Wait(20);
    ObjectEvent_SetSpeed(LOCALID_IODEM, PIXELS(6) / 5, PIXELS(3) / 5);
    ObjectEvent_WalkByAndWait(LOCALID_IODEM, 0, -16);
    ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_EAST, 0);
    Script_Wait(30);
    ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_NORTHEAST, 0);
    Script_Wait(30);
    ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_EAST, 0);
    Script_Wait(30);
    ObjectEvent_ShowEmote(LOCALID_IODEM, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                          40);
    Script_ShowMessage(LOCALID_IODEM, 0);
    Script_Wait(10);
    ObjectEvent_RunRepeatedMotion(LOCALID_SOLDIER_NORTH, 2);
    Script_Wait(20);
    Script_ShowMessage(LOCALID_SOLDIER_NORTH, 0);
    Script_Wait(10);
    ObjectEvent_SetSpeed(LOCALID_IODEM, PIXELS(6) / 5, PIXELS(3) / 5);
    ObjectEvent_WalkByAndWait(LOCALID_IODEM, 0, -24);
    ObjectEvent_WalkByAndWait(LOCALID_IODEM, 48, 0);
    ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_NORTH, 0);
    Script_Wait(30);
    Script_ShowMessage(LOCALID_IODEM, 0);
    Script_Wait(20);
    ObjectEvent_ShowEmote(LOCALID_IODEM, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                          40);
    Script_ShowMessage(LOCALID_IODEM, 0);
    Script_Wait(20);
    ObjectEvent_ShowEmote(LOCALID_SOLDIER_SOUTH, EMOTE_IN_FRONT | EMOTE_WINCE,
                          40);
    Script_ShowMessage(LOCALID_SOLDIER_SOUTH, 0);
    Script_Wait(10);
    ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_SOUTHEAST, 0);
    Script_Wait(20);
    Script_ShowMessage(LOCALID_IODEM, 0);
    Script_Wait(10);
    ObjectEvent_RunRepeatedMotion(LOCALID_SOLDIER_NORTH, 2);
    Script_Wait(20);
    ObjectEvent_SetAnimationAndWait(LOCALID_SOLDIER_NORTH,
                                    ANIMATION_SHAKE_HEAD);
    Script_Wait(20);
    Script_ShowMessage(LOCALID_SOLDIER_NORTH, 0);
    Script_Wait(10);
    ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_NORTH, 0);
    Script_Wait(30);
    ObjectEvent_ShowEmote(LOCALID_IODEM, EMOTE_IN_FRONT | EMOTE_SCOWL, 40);
    Script_ShowMessage(LOCALID_IODEM, 0);
    Script_Wait(10);
    ObjectEvent_StartRepeatedMotion(LOCALID_SOLDIER_SOUTH, 2);
    ObjectEvent_RunRepeatedMotion(LOCALID_SOLDIER_NORTH, 2);
    Script_Wait(20);
    ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(6) / 5, PIXELS(3) / 5);
    ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 368, 104);
    ObjectEvent_WalkByAndWait(LOCALID_PARTY_LEADER, 16, 0);
    ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_EAST, 0);
    Script_Wait(20);
    Script_Wait(10);
    ObjectEvent_SetAnimationAndWait(LOCALID_IODEM, ANIMATION_SHAKE_HEAD);
    Script_Wait(20);
    Script_ShowMessage(LOCALID_IODEM, 0);
    Script_Wait(20);
    ObjectEvent_ShowEmote(LOCALID_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
    Script_ShowMessage(LOCALID_IODEM, 0);
    Script_Wait(10);
    ObjectEvent_RunRepeatedMotion(LOCALID_SOLDIER_SOUTH, 2);
    Script_Wait(20);
    Script_ShowMessage(LOCALID_SOLDIER_SOUTH, 0);
    Script_Wait(10);
    ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_SOLDIER_SOUTH, 0);
    ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_SOUTHEAST, 0);
    Script_Wait(20);
    Script_ShowMessage(LOCALID_IODEM, 0);
    Script_Wait(10);
    ObjectEvent_RunRepeatedMotion(LOCALID_SOLDIER_NORTH, 2);
    Script_Wait(20);
    Script_ShowMessage(LOCALID_SOLDIER_NORTH, 0);
    Script_Wait(10);
    ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_SOLDIER_NORTH, 0);
    ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_NORTH, 0);
    Script_Wait(40);
    ObjectEvent_SetAnimationAndWait(LOCALID_IODEM, ANIMATION_NOD);
    Script_Wait(30);
    Script_ShowMessage(LOCALID_IODEM, 0);
    Script_Wait(10);
    ObjectEvent_RunRepeatedMotion(LOCALID_SOLDIER_NORTH, 2);
    Script_Wait(20);
    ObjectEvent_SetAnimationAndWait(LOCALID_SOLDIER_NORTH,
                                    ANIMATION_SHAKE_HEAD);
    Script_Wait(20);
    Script_ShowMessage(LOCALID_SOLDIER_NORTH, 0);
    Script_Wait(10);
    ObjectEvent_FaceEachOther(LOCALID_IODEM, LOCALID_PARTY_LEADER, 30);
    Script_ShowMessage(LOCALID_IODEM, 0);
    Script_Wait(10);
    ObjectEvent_RunRepeatedMotion(LOCALID_SOLDIER_SOUTH, 2);
    Script_Wait(20);
    Script_ShowMessage(LOCALID_SOLDIER_SOUTH, 0);
    Script_Wait(10);
    ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_SOLDIER_SOUTH, 0);
    ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_SOUTHEAST, 0);
    Script_Wait(40);
    Script_Wait(10);
    ObjectEvent_RunRepeatedMotion(LOCALID_SOLDIER_NORTH, 2);
    Script_Wait(20);
    Script_ShowMessage(LOCALID_SOLDIER_NORTH, 0);
    Script_Wait(10);
    ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_SOLDIER_NORTH, 0);
    ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_NORTH, 0);
    Script_Wait(40);
    ObjectEvent_SetAnimation(LOCALID_PARTY_LEADER, ANIMATION_NOD);
    ObjectEvent_SetAnimationAndWait(LOCALID_IODEM, ANIMATION_NOD);
    Script_Wait(30);
    Script_Wait(10);
    ObjectEvent_SetAnimationAndWait(LOCALID_SOLDIER_SOUTH,
                                    ANIMATION_SHAKE_HEAD);
    Script_Wait(20);
    Script_ShowMessage(LOCALID_SOLDIER_SOUTH, 0);
    Script_Wait(10);
    ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER,
                          EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    ObjectEvent_ShowEmote(LOCALID_IODEM, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                          40);
    Script_Wait(10);
    ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_SOLDIER_SOUTH, 0);
    ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_SOUTHEAST, 0);
    Script_Wait(40);
    Script_Wait(10);
    ObjectEvent_RunRepeatedMotion(LOCALID_SOLDIER_NORTH, 2);
    Script_Wait(20);
    Script_ShowMessage(LOCALID_SOLDIER_NORTH, 0);
    Script_Wait(10);
    ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER,
                          EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 50);
    ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_SOLDIER_NORTH, 0);
    Script_Wait(30);
    ObjectEvent_RunRepeatedMotion(LOCALID_SOLDIER_NORTH, 2);
    Script_Wait(30);
    Script_Wait(10);
    ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_NORTH, 0);
    Script_Wait(30);
    Script_ShowMessage(LOCALID_IODEM, 0);
    Script_Wait(10);
    ObjectEvent_RunRepeatedMotion(LOCALID_SOLDIER_NORTH, 2);
    Script_Wait(20);
    ObjectEvent_SetAnimationAndWait(LOCALID_SOLDIER_NORTH, ANIMATION_NOD);
    Script_Wait(30);
    Script_Wait(10);
    ObjectEvent_ShowEmote(LOCALID_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
    Script_ShowMessage(LOCALID_IODEM, 0);
    Script_Wait(10);
    ObjectEvent_SetAnimationAndWait(LOCALID_SOLDIER_SOUTH,
                                    ANIMATION_SHAKE_HEAD);
    Script_Wait(20);
    Script_ShowMessage(LOCALID_SOLDIER_SOUTH, 0);
    Script_Wait(10);
    ObjectEvent_ShowEmote(LOCALID_IODEM, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
    Script_Wait(10);
    ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_SOLDIER_SOUTH, 0);
    ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_SOUTHEAST, 0);
    Script_Wait(20);
    Script_Wait(10);
    ObjectEvent_RunRepeatedMotion(LOCALID_SOLDIER_SOUTH, 2);
    Script_Wait(20);
    Script_ShowMessage(LOCALID_SOLDIER_SOUTH, 0);
    Script_Wait(10);
    ObjectEvent_ShowEmote(LOCALID_IODEM, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                          40);
    Script_ShowMessage(LOCALID_IODEM, 0);
    Script_Wait(10);
    ObjectEvent_RunRepeatedMotion(LOCALID_SOLDIER_NORTH, 2);
    Script_Wait(20);
    Script_ShowMessage(LOCALID_SOLDIER_NORTH, 0);
    ObjectEvent_RunRepeatedMotion(LOCALID_SOLDIER_NORTH, 2);
    Script_Wait(20);
    Script_Wait(10);
    ObjectEvent_FaceEachOther(LOCALID_IODEM, LOCALID_PARTY_LEADER, 0);
    Script_Wait(30);
    Script_ShowMessage(LOCALID_IODEM, 0);
    Script_Wait(10);
    ObjectEvent_SetAnimationAndWait(LOCALID_SOLDIER_SOUTH,
                                    ANIMATION_SHAKE_HEAD);
    Script_Wait(20);
    Script_ShowMessage(LOCALID_SOLDIER_SOUTH, 0);
    Script_Wait(10);
    ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE,
                          0);
    ObjectEvent_ShowEmote(LOCALID_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
    ObjectEvent_ShowEmote(LOCALID_SOLDIER_NORTH, EMOTE_IN_FRONT | EMOTE_WINCE,
                          50);
    Script_ShowMessage(LOCALID_SOLDIER_NORTH, 0);
    Script_Wait(10);
    ObjectEvent_RunRepeatedMotion(LOCALID_SOLDIER_NORTH, 2);
    Script_Wait(20);
    Script_ShowMessage(LOCALID_SOLDIER_NORTH, 0);
    ObjectEvent_RunRepeatedMotion(LOCALID_SOLDIER_NORTH, 3);
    Script_Wait(20);
    Script_Wait(10);
    ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_NORTH, 0);
    Script_Wait(30);
    ObjectEvent_RunRepeatedMotion(LOCALID_IODEM, 2);
    Script_Wait(10);
    Script_ShowMessage(LOCALID_IODEM, 0);
    Script_Wait(10);
    ObjectEvent_RunRepeatedMotion(LOCALID_SOLDIER_SOUTH, 2);
    Script_Wait(20);
    Script_ShowMessage(LOCALID_SOLDIER_SOUTH, 0);
    Script_Wait(10);
    ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_SOLDIER_SOUTH, 0);
    ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_SOUTHEAST, 0);
    Script_Wait(30);
    ObjectEvent_SetAnimationAndWait(LOCALID_IODEM, ANIMATION_NOD);
    Script_Wait(30);
    Script_ShowMessage(LOCALID_IODEM, 0);
    Script_Wait(10);
    ObjectEvent_StartRepeatedMotion(LOCALID_SOLDIER_SOUTH, 2);
    ObjectEvent_RunRepeatedMotion(LOCALID_SOLDIER_NORTH, 2);
    Script_Wait(30);
    Script_Wait(10);
    ObjectEvent_FaceEachOther(LOCALID_IODEM, LOCALID_PARTY_LEADER, 20);
    ObjectEvent_WalkByAndWait(LOCALID_IODEM, -12, 0);
    Script_Wait(20);
    // Both answers converge on the following line after different waits.
    Script_OpenMessage(LOCALID_IODEM, 0);
    if (Script_ChooseYesNo(LOCALID_PARTY_LEADER, 0) == 0) {
      Script_Wait(20);
      Script_ShowMessage(LOCALID_IODEM, 0);
      gFieldWork->message++;
    } else {
      Script_Wait(10);
      gFieldWork->message++;
      Script_ShowMessage(LOCALID_IODEM, 0);
    }
    Script_Wait(10);
    ObjectEvent_RunRepeatedMotion(LOCALID_IODEM, 2);
    Script_Wait(20);
    Script_ShowMessage(LOCALID_IODEM, 0);
    Script_Wait(10);
    ObjectEvent_ShowEmote(LOCALID_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
    Script_ShowMessage(LOCALID_IODEM, 0);
    Script_Wait(10);
    ObjectEvent_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIMATION_NOD);
    Script_Wait(20);
    ObjectEvent_SetAnimationAndWait(LOCALID_IODEM, ANIMATION_NOD);
    Script_Wait(30);
    Script_PlaySong(MUS_030);
    ObjectEvent_SetSpeed(LOCALID_IODEM, PIXELS(6) / 5, PIXELS(3) / 5);
    ObjectEvent_SetAnimation(LOCALID_IODEM, ANIMATION_WALK);
    // Iodem walks into the leader and is hidden: he travels in the party.
    leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
    if (leader != 0) {
      ObjectEvent_SetDestination(LOCALID_IODEM, leader->x.part.pixel,
                                 leader->z.part.pixel);
    }
    ObjectEvent_WaitForMove(LOCALID_IODEM);
    ObjectEvent_SetPosition(LOCALID_IODEM, 0, 0);
    Script_Wait(10);
    // Back to the map section's own music.
    Map_PlayMusic();
    Script_End();
  }
}

/// The doors and inner warps, people and ovens of Suhalla's buildings and
/// Suhalla Sanctum, before or after Babi's audience.
const struct MapEvent *Map_GetEvents(void) {
  if (Engine_GameFlagIsSet(FLAG_BABI_SPOKE_AFTER_COLOSSO) != 0) {
    return gSuhallaInteriorsEventsAfterAudience;
  }
  return gSuhallaInteriorsEvents;
}

/// Sets Suhalla's buildings up as the party enters: the merchant and
/// innkeeper behind their counters, and the two wounded soldiers lying in their
/// beds. SUHALLA_INTERIORS_AFTER_BABI_SPOKE sets
/// FLAG_BABI_SPOKE_AFTER_COLOSSO.
s32 Map_Initialize(void) {
  struct ObjectEvent *objectEvent;
  if (gGameState.warpDest == SUHALLA_INTERIORS_AFTER_BABI_SPOKE)
    Engine_GameFlagSet(FLAG_BABI_SPOKE_AFTER_COLOSSO);
  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  gFieldWork->transitionFrames = 24;
  // Nearer to talk to, across their counters.
  ObjectEvent_GetById(LOCALID_MERCHANT)->collisionFlags |= OBJ_EVENT_NEAR_TALK;
  ObjectEvent_GetById(LOCALID_INNKEEPER)->collisionFlags |= OBJ_EVENT_NEAR_TALK;
  // Each soldier lies a quarter turn over, with no shadow.
  objectEvent = ObjectEvent_GetById(LOCALID_SOLDIER_SOUTH);
  objectEvent->sprite->flags = 0;
  objectEvent->sprite->rotation = 0x4000;
  objectEvent->sprite->image.priority = 1;
  objectEvent = ObjectEvent_GetById(LOCALID_SOLDIER_NORTH);
  objectEvent->sprite->flags = 0;
  objectEvent->sprite->rotation = 0x4000;
  objectEvent->motionFlags = OBJ_EVENT_FALLS;
  objectEvent->y.fixed = 0;
  return 0;
}

struct MapWarpDest gSuhallaInteriorsWarpDests[] = {
    {WARP_DEST_DEFAULT, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {SUHALLA_INTERIORS_INN, CONDITION_ALWAYS, 256, 0, 456, FACING_NORTH, 0, 0,
     280, 336, 480, 0},
    {SUHALLA_INTERIORS_SANCTUM, CONDITION_ALWAYS, 432, 0, 728, FACING_NORTH, 0,
     312, 528, 552, 752, 0},
    {SUHALLA_INTERIORS_HOUSE_WEST, CONDITION_ALWAYS, 640, 0, 168, FACING_NORTH,
     0, 496, 16, 736, 192, 0},
    {SUHALLA_INTERIORS_HOUSE_NORTHEAST, CONDITION_ALWAYS, 112, 0, 168,
     FACING_NORTH, 0, 0, 16, 240, 208, 0},
    {SUHALLA_INTERIORS_HOUSE_EAST, CONDITION_ALWAYS, 352, 0, 216, FACING_NORTH,
     0, 256, 16, 496, 232, 0},
    {SUHALLA_INTERIORS_SANCTUM_NORTH, CONDITION_ALWAYS, 432, 0, 630,
     FACING_NORTH, 0, 312, 528, 552, 752, 0},
    {SUHALLA_INTERIORS_HOUSE_WEST_UNUSED, CONDITION_ALWAYS, 680, 0, 120,
     FACING_WEST, 0, 496, 16, 736, 192, 0},
    {SUHALLA_INTERIORS_INN_FROM_GUEST_ROOM_WEST, CONDITION_ALWAYS, 104, 0, 360,
     FACING_NORTH, 0, 0, 280, 336, 480, 0},
    {SUHALLA_INTERIORS_INN_FROM_GUEST_ROOM_EAST, CONDITION_ALWAYS, 152, 0, 360,
     FACING_NORTH, 0, 0, 280, 336, 480, 0},
    {SUHALLA_INTERIORS_GUEST_ROOM_WEST_FROM_INN, CONDITION_ALWAYS, 472, 0, 344,
     FACING_SOUTH, 0, 344, 280, 584, 440, 0},
    {SUHALLA_INTERIORS_GUEST_ROOM_EAST_FROM_INN, CONDITION_ALWAYS, 712, 0, 344,
     FACING_SOUTH, 0, 600, 280, 840, 440, 0},
    {SUHALLA_INTERIORS_INN_FROM_LOFT, CONDITION_ALWAYS, 88, 0, 352,
     FACING_SOUTH, 0, 0, 280, 336, 480, 0},
    {SUHALLA_INTERIORS_LOFT_FROM_INN, CONDITION_ALWAYS, 88, 0, 584,
     FACING_NORTH, 0, 16, 528, 256, 688, 0},
    {MAP_TABLE_END},
};

// clang-format off
struct MapWarpRegion gSuhallaInteriorsRegions[] = {
    {83, 28, 356, 91, 36, 364, CONDITION_ALWAYS, SUHALLA_INTERIORS_INN_TO_LOFT},
    {83, -20, 596, 91, -12, 604, CONDITION_ALWAYS, SUHALLA_INTERIORS_LOFT_TO_INN},
    {-1, 0, 0, 0, 0, 0, 0, 0},
};

u32 gSuhallaInteriorsWarps[] = {
    MAP_WARPS(SuhallaInteriors),
    MAP_WARP(SUHALLA_INTERIORS_INN_TO_SUHALLA, Suhalla, SUHALLA_FROM_INN),
    MAP_WARP(SUHALLA_INTERIORS_SANCTUM_TO_SUHALLA, Suhalla, SUHALLA_FROM_SANCTUM),
    MAP_WARP(SUHALLA_INTERIORS_HOUSE_WEST_TO_SUHALLA, Suhalla, SUHALLA_FROM_HOUSE_WEST),
    MAP_WARP(SUHALLA_INTERIORS_HOUSE_NORTHEAST_TO_SUHALLA, Suhalla, SUHALLA_FROM_HOUSE_NORTHEAST),
    MAP_WARP(SUHALLA_INTERIORS_HOUSE_EAST_TO_SUHALLA, Suhalla, SUHALLA_FROM_HOUSE_EAST),
    MAP_WARP(SUHALLA_INTERIORS_INN_TO_GUEST_ROOM_WEST, SuhallaInteriors, SUHALLA_INTERIORS_GUEST_ROOM_WEST_FROM_INN),
    MAP_WARP(SUHALLA_INTERIORS_INN_TO_GUEST_ROOM_EAST, SuhallaInteriors, SUHALLA_INTERIORS_GUEST_ROOM_EAST_FROM_INN),
    MAP_WARP(SUHALLA_INTERIORS_GUEST_ROOM_WEST_TO_INN, SuhallaInteriors, SUHALLA_INTERIORS_INN_FROM_GUEST_ROOM_WEST),
    MAP_WARP(SUHALLA_INTERIORS_GUEST_ROOM_EAST_TO_INN, SuhallaInteriors, SUHALLA_INTERIORS_INN_FROM_GUEST_ROOM_EAST),
    MAP_WARP(SUHALLA_INTERIORS_INN_TO_LOFT, SuhallaInteriors, SUHALLA_INTERIORS_LOFT_FROM_INN),
    MAP_WARP(SUHALLA_INTERIORS_LOFT_TO_INN, SuhallaInteriors, SUHALLA_INTERIORS_INN_FROM_LOFT),
    MAP_WARPS_END,
};

struct ObjectEventTemplate gSuhallaInteriorsObjectEventTemplates[] = {
    { SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(368), 0, PIXELS(192), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(328), 0, PIXELS(80), FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(56), 0, PIXELS(120), FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(160), 0, PIXELS(104), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_WITH_PONYTAIL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(568), 0, PIXELS(96), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_WITH_DARK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(272), 0, PIXELS(392), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_IN_PINK_APRON, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(224), 0, PIXELS(344), FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_STOUT_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(432), 0, PIXELS(600), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gSuhallaInteriorsObjectEventTemplatesAfterAudience[] = {
    { SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(400), 0, PIXELS(184), FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(416), 0, PIXELS(184), FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(56), 0, PIXELS(120), FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(160), 0, PIXELS(104), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_WITH_PONYTAIL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(568), 0, PIXELS(96), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_WITH_DARK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(272), 0, PIXELS(392), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_IN_PINK_APRON, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(224), 0, PIXELS(344), FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(688), 0, PIXELS(384), FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(712), 0, PIXELS(408), FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_STOUT_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(136), 0, PIXELS(616), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(432), 0, PIXELS(600), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_IODEM, FLAG_IODEM_MET_WOUNDED_SOLDIERS, MOVEMENT_TYPE_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_EAST, TALK_FACING_KEEP, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(416), 0, PIXELS(136), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(416), 0, PIXELS(80), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

struct MapEvent gSuhallaInteriorsEvents[] = {
    { MAP_EVENT_COORD, 1, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveByWarp) },
    { MAP_EVENT_COORD, 2, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveByWarp) },
    { MAP_EVENT_COORD, 3, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveByWarp) },
    { MAP_EVENT_COORD, 4, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveByWarp) },
    { MAP_EVENT_COORD, 5, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveByWarp) },
    { MAP_EVENT_COORD, 10, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveByWarp) },
    { MAP_EVENT_COORD, 11, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveByWarp) },
    { MAP_EVENT_COORD, 12, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveByWarp) },
    { MAP_EVENT_COORD, 13, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveByWarp) },
    { MAP_EVENT_TALK, LOCALID_FATHERS_DAUGHTER, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsMyFatherLovesPeopleSoMuch) },
    { MAP_EVENT_TALK, LOCALID_FATHER_WHO_WANTS_TO_HELP, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsLetMeKnowIfYouHave) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_FATHERS_DAUGHTER, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsWhenISeePeopleInTrouble) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_FATHER_WHO_WANTS_TO_HELP, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsIWantToHelpPeopleSo) },
    { MAP_EVENT_TALK, LOCALID_OLD_MAN_IGNORED_BY_TOLBI, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsTolbiProbablyIgnoresSmallVillagesLike) },
    { MAP_EVENT_TALK, LOCALID_OLD_WOMAN, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsIfYouHeadEastAcrossSuhalla) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_OLD_MAN_IGNORED_BY_TOLBI, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsSuhallaIsADesertVillageI) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_OLD_WOMAN, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsGoodThingItsNotEasyTo) },
    { MAP_EVENT_TALK, LOCALID_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToMerchant) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsAVillageThisSmallCantSupport) },
    { MAP_EVENT_TALK, LOCALID_INNKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToInnkeeper) },
    { MAP_EVENT_TALK, LOCALID_INN_WOMAN, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsIWonderIfAnyCustomersCan) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsOnceTheFestivalEndsTheTravelers) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_INN_WOMAN, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsForSomeReasonAnyoneWhoGets) },
    { MAP_EVENT_TALK, LOCALID_HEALER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToHealer) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_HEALER, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsTheHolyChildActorWillSave) },
    { BG_EVENT(BG_EVENT_OVEN), 100, FLAG_SUHALLA_INTERIORS_OVEN_SMOKE_BOMB, FIND_ITEM(ITEM_SMOKE_BOMB) },
    // Unused: this oven's item row answers first (see the maps README).
    { BG_EVENT(BG_EVENT_OVEN), 100, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgSuhallaInteriorsItsACactusSteakIWonder)) },
    { BG_EVENT(BG_EVENT_OVEN), 101, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgSuhallaInteriorsHesMakingMulukhiyaSoupItTastes)) },
    { MAP_EVENTS_END },
};

struct MapEvent gSuhallaInteriorsEventsAfterAudience[] = {
    { MAP_EVENT_COORD, 50, FLAG_IODEM_MET_WOUNDED_SOLDIERS, EVENT_SCRIPT(EventScript_IodemQuestionsWoundedSoldiers) },
    { MAP_EVENT_COORD, 1, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveByWarp) },
    { MAP_EVENT_COORD, 2, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveByWarp) },
    { MAP_EVENT_COORD, 3, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveByWarp) },
    { MAP_EVENT_COORD, 4, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveByWarp) },
    { MAP_EVENT_COORD, 5, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveByWarp) },
    { MAP_EVENT_COORD, 10, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveByWarp) },
    { MAP_EVENT_COORD, 11, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveByWarp) },
    { MAP_EVENT_COORD, 12, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveByWarp) },
    { MAP_EVENT_COORD, 13, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveByWarp) },
    { MAP_EVENT_TALK, LOCALID_FATHERS_DAUGHTER, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsTheWoundedAreSleepingSoDont) },
    { MAP_EVENT_TALK, LOCALID_FATHER_WHO_WANTS_TO_HELP, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsIAmAmazedTheseSoldiersSurvived) },
    { MAP_EVENT_TALK, LOCALID_SOLDIER_SOUTH, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsWhyWereTheyHeadedTowardEastern) },
    { MAP_EVENT_TALK, LOCALID_SOLDIER_NORTH, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsWhereIsActorWhatHasHappened) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_FATHERS_DAUGHTER, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsTheseOldSoldiersMustRestUntil) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_FATHER_WHO_WANTS_TO_HELP, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsWhoCouldHaveDoneThisTo) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SOLDIER_SOUTH, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsAColumnOfWaterShotUp) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SOLDIER_NORTH, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsActorWasNotFarBehindUs) },
    { MAP_EVENT_TALK, LOCALID_OLD_MAN_IGNORED_BY_TOLBI, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsDoNotApproachTheLargeCyclones) },
    { MAP_EVENT_TALK, LOCALID_OLD_WOMAN, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsDeathStormsHuhThereWasA) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_OLD_MAN_IGNORED_BY_TOLBI, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsOnceYouGetSweptUpIn) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_OLD_WOMAN, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsYeWhoSeekSomethingAboutEyes) },
    { MAP_EVENT_TALK, LOCALID_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToMerchant) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsWeveHadMoreCustomersLatelyBecause) },
    { MAP_EVENT_TALK, LOCALID_INNKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToInnkeeper) },
    { MAP_EVENT_TALK, LOCALID_INN_WOMAN, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsItWillTakeTimeForThe) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsTheSandstormsAreSendingCustomersOur) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_INN_WOMAN, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsPleaseGreatSandstormsSendUsMore) },
    { MAP_EVENT_TALK, LOCALID_LIGHTHOUSE_WORKER, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsWeWentIntoTheSuhallaTo) },
    { MAP_EVENT_TALK, LOCALID_SECOND_LIGHTHOUSE_WORKER, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsTheSandstormsAreSoBadWeve) },
    { MAP_EVENT_TALK, LOCALID_STRANDED_TRAVELER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_AskIfBlownHere) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_LIGHTHOUSE_WORKER, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsEvenIfWeCantGoIt) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SECOND_LIGHTHOUSE_WORKER, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsSoShallWeGiveUpOn) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STRANDED_TRAVELER, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsWhenThatCycloneThrewUsInto) },
    { MAP_EVENT_TALK, LOCALID_HEALER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToHealer) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_HEALER, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsIShudderToThinkSandstormsWill) },
    { BG_EVENT(BG_EVENT_OVEN), 100, FLAG_SUHALLA_INTERIORS_OVEN_SMOKE_BOMB, FIND_ITEM(ITEM_SMOKE_BOMB) },
    // Unused: this oven's item row answers first (see the maps README).
    { BG_EVENT(BG_EVENT_OVEN), 100, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgSuhallaInteriorsItsACactusSteakIWonder)) },
    { BG_EVENT(BG_EVENT_OVEN), 101, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgSuhallaInteriorsHesMakingMulukhiyaSoupItTastes)) },
    { MAP_EVENTS_END },
};
// clang-format on
