/// @file
/// Timber rooms A (SceneId_TimberRoomsA), a sheet of rooms drawn in timber, all
/// of them inside Suhalla's buildings, whose rooms are told apart by the
/// entrance the party arrives at, and whose area name follows it:
///
/// - Suhalla: the houses, the shop and the inn behind the village's doors,
///   whose exits from the village (SUHALLA_TO_INN, SUHALLA_TO_HOUSE_WEST,
///   SUHALLA_TO_HOUSE_NORTHEAST and SUHALLA_TO_HOUSE_EAST) lead to the
///   entrances of the same numbers. Doorways join the inn to its two
///   guest rooms, and a ladder to its loft, climbed through two exit regions.
/// - Suhalla Sanctum: behind the village's wide door (SUHALLA_TO_SANCTUM),
///   with TIMBER_ROOMS_A_SANCTUM_NORTH as its second entrance.
///
/// The rooms' people and talk come in two sets, each an actor placement
/// table and a trigger table, chosen by FLAG_BABI_SPOKE_AFTER_COLOSSO. Before
/// Babi's audience after Colosso the village talks of Lalivero across the Nol
/// River, out of reach under Tolbi's rule; after it, two of the soldiers who
/// were escorting Sheba home lie wounded in a house, travelers the sandstorms
/// blew back fill the inn, and an old woman half remembers a saying about the
/// death storms. The two tables agree up to ACTOR_HEALER; the travelers stand
/// off the map until the audience.
///
/// Stepping into the wounded soldiers' house (the step trigger on cell
/// behavior 50) plays TimberRoomsA_IodemQuestionsWoundedSoldiers once, while
/// Iodem travels with the party and has not yet reached Babi Lighthouse. It
/// sets FLAG_IODEM_MET_WOUNDED_SOLDIERS either way, which Iodem's lines in the
/// Suhalla Desert and at Suhalla Gate read. The merchant, the innkeeper and
/// the Healer serve a leader who faces north across their counters, and
/// otherwise talk by the same flag. Searching the ovens finds a Smoke Bomb
/// once (FLAG_TIMBER_ROOMS_A_OVEN_SMOKE_BOMB) and shows what is cooking.
///
/// Unused: the cactus steak line at the Smoke Bomb's oven, whose item row
/// always answers first, and the entrance
/// TIMBER_ROOMS_A_HOUSE_WEST_UNUSED, to which no exit leads.

#include "main_routine_work.h"
#include "event.h"
#include "module/import.h"
#include "module/scene/timber_rooms_a.h"
#include "module/overworld/overworld.h"
#include "module/system/main_routine.h"
#include "module/overworld/event_commands.h"
#include "event_inline.h"
#include "text/msg_ids.h"
#include "types.h"
#include "ability_ids.h"
#include "scene_tables.h"
#include "item_ids.h"
#include "flags.h"
#include "sounds.h"
#include "scene.h"

/// The actors, numbering the rows of
/// gTimberRoomsAActorPlacements and of
/// gTimberRoomsAActorPlacementsAfterAudience from
/// ACTOR_FIRST_PLACED; the two tables agree up to ACTOR_HEALER. Where an
/// actor's talk changes after Babi's audience, the note quotes it
/// before.
enum TimberRoomsAActorId {
  /// A little girl with orange pigtails (SPRITE_GIRL_WITH_PIGTAILS), the
  /// daughter of the man who aids anyone in trouble. "My father loves people
  /// so much, he can't help but aid anyone in trouble."
  /// ★ Named from the game: "I guess I'm just my father's child!"
  /// (MsgSuhallaInteriorsWhenISeePeopleInTrouble)
  ACTOR_FATHERS_DAUGHTER = ACTOR_FIRST_PLACED,
  /// The man who aids anyone in trouble, a man with brown hair
  /// (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR), in whose house the wounded soldiers
  /// lie after the audience. "Let me know if you have any problems. I'll do
  /// anything I can to help."
  /// ★ Named from the game: "I want to help people so badly, I can't sit idly
  /// by!" (MsgSuhallaInteriorsIWantToHelpPeopleSo)
  ACTOR_FATHER_WHO_WANTS_TO_HELP,
  /// A bent old man with a cane (SPRITE_OLD_MAN_WITH_CANE). After the
  /// audience: "Do not approach the large cyclones in the middle of the
  /// Suhalla."
  /// ★ Named from the game: "Tolbi probably ignores small villages like
  /// Suhalla" (MsgSuhallaInteriorsTolbiProbablyIgnoresSmallVillagesLike)
  ACTOR_OLD_MAN_IGNORED_BY_TOLBI,
  /// In the old woman's sprite (SPRITE_OLD_WOMAN, as in vale_plaza.c). "If you
  /// head east across
  /// Suhalla, Lalivero is so close you can see it across the Nol River..."
  /// After the audience she half remembers a saying about the death storms.
  /// ★ Named from what the player sees: a white-haired old woman in a dark
  /// dress (its render)
  ACTOR_OLD_WOMAN,
  /// A young woman with a brown ponytail (SPRITE_WOMAN_WITH_PONYTAIL).
  /// ★ Named from the game: "Ours is the only store in Suhalla."
  /// (MsgSuhallaOursOnlyStore)
  ACTOR_MERCHANT,
  /// A man with dark hair (SPRITE_MAN_WITH_DARK_HAIR), who opens the inn across
  /// its counter (TimberRoomsA_TalkToInnkeeper).
  /// ★ Named from the game: "Innkeeper" (MsgDebugGlyphName)
  ACTOR_INNKEEPER,
  /// A woman at the inn, with red-brown hair in a white apron
  /// (SPRITE_WOMAN_IN_PINK_APRON), the sprite of Kalay's innkeeper's wife.
  /// ★ Named from the game: "I wonder if any customers can get through those
  /// sandstorms..." (MsgSuhallaInteriorsIWonderIfAnyCustomersCan)
  ACTOR_INN_WOMAN,
  /// Travelers the sandstorms blew back, out of sight until the audience: two
  /// stocky strong men (SPRITE_STRONGMAN). "We
  /// went into the Suhalla to go build the lighthouse, but the storms caught
  /// us."
  /// ★ Named from the game: "We went into the Suhalla to go build the
  /// lighthouse" (MsgSuhallaInteriorsWeWentIntoTheSuhallaTo)
  ACTOR_LIGHTHOUSE_WORKER,
  /// "The sandstorms are so bad, we've given up trying to reach Babi
  /// Lighthouse."
  /// ★ Named from the game: "We went into the Suhalla to go build the
  /// lighthouse" (MsgSuhallaInteriorsWeWentIntoTheSuhallaTo)
  ACTOR_SECOND_LIGHTHOUSE_WORKER,
  /// A stout man with brown hair (SPRITE_STOUT_MAN).
  /// ★ Named from the game: "I set out for Babi Lighthouse to open a business
  /// there, but then I was blown back here again!" (MsgSuhallaSupposeFolkBlown)
  ACTOR_STRANDED_TRAVELER,
  /// A bearded man in a long robe (SPRITE_BEARDED_MAN_IN_LONG_ROBE), who keeps
  /// Suhalla Sanctum (TimberRoomsA_TalkToHealer).
  /// ★ Named from the game: "Healer" (MsgDebugGlyphName)
  ACTOR_HEALER,
  /// ★ Named from the game: "Iodem..." (MsgSuhallaIodem)
  ACTOR_IODEM,
  /// The wounded soldiers, lying in the beds of a house after Babi's audience.
  /// ★ Named from the game: "These old soldiers must rest until their wounds
  /// heal." (MsgSuhallaInteriorsTheseOldSoldiersMustRestUntil)
  ACTOR_SOLDIER_SOUTH,
  /// ★ Named from the game: "These old soldiers must rest until their wounds
  /// heal." (MsgSuhallaInteriorsTheseOldSoldiersMustRestUntil)
  ACTOR_SOLDIER_NORTH
};

enum { SHOP_SUHALLA = 31, INN_SUHALLA = 10 };

const struct SceneEntrance *Scene_GetEntrances(void) {
  return gTimberRoomsAEntrances;
}

const struct SceneExitRegion *Scene_GetExitRegions(void) {
  return gTimberRoomsARegions;
}

const u32 *Scene_GetExits(void) { return gTimberRoomsAExits; }

/// Suhalla's people, joined after Babi's audience by Iodem, the wounded
/// soldiers and the travelers the sandstorms brought.
const struct ActorPlacement *Scene_GetActorPlacements(void) {
  if (GameFlag_Get(FLAG_BABI_SPOKE_AFTER_COLOSSO) != 0) {
    return gTimberRoomsAActorPlacementsAfterAudience;
  }
  return gTimberRoomsAActorPlacements;
}

/// Suhalla's merchant. Spoken to facing north, across the counter, the
/// merchant opens the shop; otherwise says Suhalla's is the only store, or,
/// after Babi's audience, asks whether the party is surprised to find so few
/// shops.
static void TimberRoomsA_TalkToMerchant(s32 actorId) {
  u16 facing = (Actor_GetById(ACTOR_PARTY_LEADER)->facing + 0x2000) & ~0x3fff;
  if (facing == FACING_NORTH) {
    Shop_Run(SHOP_SUHALLA, actorId);
  } else if (GameFlag_Get(FLAG_BABI_SPOKE_AFTER_COLOSSO)) {
    s32 question = MsgSuhallaArentSurprisedFind;
    Event_SetMessageId(question);
    FieldMessage_ShowAtActor(actorId, 0);
    if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
      Event_Wait(10);
      Event_SetMessageId(question + 1);
    } else {
      Event_SetMessageId(question + 2);
    }
    Event_ShowMessageAndWait(actorId, 0);
  } else {
    Event_SetMessageId(MsgSuhallaOursOnlyStore);
    Event_ShowMessageAndWait(actorId, 0);
  }
}

/// Suhalla's innkeeper. Spoken to facing north, across the counter, the
/// innkeeper offers a room; otherwise has nothing to do until Tolbi's festival
/// ends, or, after Babi's audience, has been busy since the sandstorms began.
static void TimberRoomsA_TalkToInnkeeper(s32 actorId) {
  u16 facing = (Actor_GetById(ACTOR_PARTY_LEADER)->facing + 0x2000) & ~0x3fff;
  if (facing == FACING_NORTH) {
    Inn_CheckIn(INN_SUHALLA, actorId);
  } else if (GameFlag_Get(FLAG_BABI_SPOKE_AFTER_COLOSSO)) {
    Event_SetMessageId(MsgSuhallaBusyEverSince);
    Event_ShowMessageAndWait(actorId, 0);
  } else {
    Event_SetMessageId(MsgSuhallaSighNothinMaybe);
    Event_ShowMessageAndWait(actorId, 0);
  }
}

/// Suhalla Sanctum's Healer. Spoken to facing north, across the counter, the
/// Healer offers the sanctum's services; otherwise talks of Lalivero under
/// Tolbi's rule, or, after Babi's audience, wonders why there are so many
/// sandstorms.
static void TimberRoomsA_TalkToHealer(s32 actorId) {
  u16 facing = (Actor_GetById(ACTOR_PARTY_LEADER)->facing + 0x2000) & ~0x3fff;
  if (facing == FACING_NORTH) {
    Sanctum_Run(actorId);
  } else if (GameFlag_Get(FLAG_BABI_SPOKE_AFTER_COLOSSO)) {
    Event_SetMessageId(MsgSuhallaWonderWhySandstorms);
    Event_ShowMessageAndWait(actorId, 0);
  } else {
    Event_SetMessageId(MsgSuhallaGetSickThinkingAboutLalivero);
    Event_ShowMessageAndWait(actorId, 0);
  }
}

/// A traveler the storms blew back asks whether the party was blown here too.
/// That's in Suhalla after Babi's audience, and the question is whether the
/// party was on the way to Babi Lighthouse: yes, and the traveler means to give
/// up and follow the northern mountain back to Tolbi; no, and tells of setting
/// out to open a business at the lighthouse.
static void TimberRoomsA_AskIfBlownHere(s32 actorId) {
  s32 question = MsgSuhallaSupposeFolkBlown;
  Event_SetMessageId(question);
  FieldMessage_ShowAtActor(actorId, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(question + 1);
  } else {
    Event_SetMessageId(question + 2);
  }
  Event_ShowMessageAndWait(actorId, 0);
}

/// The doors of Suhalla and Suhalla Sanctum, and the doors between the inn and
/// its guest rooms: the party leaves by the touched exit.
static void TimberRoomsA_LeaveByExit(void) {
  struct FieldWork *state = gFieldWork;
  s16 *touchedTrigger = (s16 *)&state->stepTrigger;

  Import_EventSetExitRequest(*touchedTrigger);
  Sound_Play(SE_MAP_EXIT);
}

/// In Suhalla after Babi's audience, stepping into the house where the two
/// wounded soldiers lie (cell behavior 50), once: Iodem steps out of the
/// party and questions them. "Iodem..." They were escorting Sheba to Lalivero
/// when a sandstorm hit them, and tell of others who stopped the cyclones with
/// water and slew the monsters causing them. Iodem asks whether these are the
/// ones the party follows, resolves either way to search the desert for Sheba
/// and pursue them, and rejoins the party. Sets
/// FLAG_IODEM_MET_WOUNDED_SOLDIERS either way; once Iodem has reached Babi
/// Lighthouse (FLAG_BABI_LIGHTHOUSE_IODEM_ARRIVED), the event does not play.
static void TimberRoomsA_IodemQuestionsWoundedSoldiers(void) {
  struct Actor *leader;
  GameFlag_Set(FLAG_IODEM_MET_WOUNDED_SOLDIERS);
  if (GameFlag_Get(FLAG_BABI_LIGHTHOUSE_IODEM_ARRIVED) == 0) {
    Event_PlaySound(MUS_BABI);
    Event_Start();
    // Frame the speakers and bring the leader over before the dialogue.
    FieldCamera_MoveTo(PIXELS(368), -1, PIXELS(104), 1);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 368, 160);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    FieldActor_LaunchFromLeader(ACTOR_IODEM, 0, -16, FACING_NORTH);
    Actor_WaitForMoveById(ACTOR_IODEM);
    FieldCamera_WaitForMove();
    Event_SetMessageId(MsgSuhallaIodem);
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_SOLDIER_SOUTH, 2);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_SOLDIER_SOUTH, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
    Event_Wait(20);
    Actor_SetSpeed(ACTOR_IODEM, PIXELS(6) / 5, PIXELS(3) / 5);
    Actor_WalkByAndWait(ACTOR_IODEM, 0, -16);
    Actor_FaceDirection(ACTOR_IODEM, FACING_EAST, 0);
    Event_Wait(30);
    Actor_FaceDirection(ACTOR_IODEM, FACING_NORTHEAST, 0);
    Event_Wait(30);
    Actor_FaceDirection(ACTOR_IODEM, FACING_EAST, 0);
    Event_Wait(30);
    Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
    Event_ShowMessage(ACTOR_IODEM, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_SOLDIER_NORTH, 2);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_SOLDIER_NORTH, 0);
    Event_Wait(10);
    Actor_SetSpeed(ACTOR_IODEM, PIXELS(6) / 5, PIXELS(3) / 5);
    Actor_WalkByAndWait(ACTOR_IODEM, 0, -24);
    Actor_WalkByAndWait(ACTOR_IODEM, 48, 0);
    Actor_FaceDirection(ACTOR_IODEM, FACING_NORTH, 0);
    Event_Wait(30);
    Event_ShowMessage(ACTOR_IODEM, 0);
    Event_Wait(20);
    Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
    Event_ShowMessage(ACTOR_IODEM, 0);
    Event_Wait(20);
    Actor_ShowEmote(ACTOR_SOLDIER_SOUTH, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
    Event_ShowMessage(ACTOR_SOLDIER_SOUTH, 0);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTHEAST, 0);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_IODEM, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_SOLDIER_NORTH, 2);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_SOLDIER_NORTH, ANIM_SHAKE_HEAD);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_SOLDIER_NORTH, 0);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_IODEM, FACING_NORTH, 0);
    Event_Wait(30);
    Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_SCOWL, 40);
    Event_ShowMessage(ACTOR_IODEM, 0);
    Event_Wait(10);
    Actor_StartRepeatedMotion(ACTOR_SOLDIER_SOUTH, 2);
    Actor_RunRepeatedMotion(ACTOR_SOLDIER_NORTH, 2);
    Event_Wait(20);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(6) / 5, PIXELS(3) / 5);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 368, 104);
    Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 16, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
    Event_Wait(20);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_SHAKE_HEAD);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_IODEM, 0);
    Event_Wait(20);
    Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
    Event_ShowMessage(ACTOR_IODEM, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_SOLDIER_SOUTH, 2);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_SOLDIER_SOUTH, 0);
    Event_Wait(10);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_SOLDIER_SOUTH, 0);
    Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTHEAST, 0);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_IODEM, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_SOLDIER_NORTH, 2);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_SOLDIER_NORTH, 0);
    Event_Wait(10);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_SOLDIER_NORTH, 0);
    Actor_FaceDirection(ACTOR_IODEM, FACING_NORTH, 0);
    Event_Wait(40);
    Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
    Event_Wait(30);
    Event_ShowMessage(ACTOR_IODEM, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_SOLDIER_NORTH, 2);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_SOLDIER_NORTH, ANIM_SHAKE_HEAD);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_SOLDIER_NORTH, 0);
    Event_Wait(10);
    Actor_FaceEachOther(ACTOR_IODEM, ACTOR_PARTY_LEADER, 30);
    Event_ShowMessage(ACTOR_IODEM, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_SOLDIER_SOUTH, 2);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_SOLDIER_SOUTH, 0);
    Event_Wait(10);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_SOLDIER_SOUTH, 0);
    Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTHEAST, 0);
    Event_Wait(40);
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_SOLDIER_NORTH, 2);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_SOLDIER_NORTH, 0);
    Event_Wait(10);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_SOLDIER_NORTH, 0);
    Actor_FaceDirection(ACTOR_IODEM, FACING_NORTH, 0);
    Event_Wait(40);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
    Event_Wait(30);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_SOLDIER_SOUTH, ANIM_SHAKE_HEAD);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_SOLDIER_SOUTH, 0);
    Event_Wait(10);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
    Event_Wait(10);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_SOLDIER_SOUTH, 0);
    Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTHEAST, 0);
    Event_Wait(40);
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_SOLDIER_NORTH, 2);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_SOLDIER_NORTH, 0);
    Event_Wait(10);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 50);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_SOLDIER_NORTH, 0);
    Event_Wait(30);
    Actor_RunRepeatedMotion(ACTOR_SOLDIER_NORTH, 2);
    Event_Wait(30);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_IODEM, FACING_NORTH, 0);
    Event_Wait(30);
    Event_ShowMessage(ACTOR_IODEM, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_SOLDIER_NORTH, 2);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_SOLDIER_NORTH, ANIM_NOD);
    Event_Wait(30);
    Event_Wait(10);
    Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
    Event_ShowMessage(ACTOR_IODEM, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_SOLDIER_SOUTH, ANIM_SHAKE_HEAD);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_SOLDIER_SOUTH, 0);
    Event_Wait(10);
    Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
    Event_Wait(10);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_SOLDIER_SOUTH, 0);
    Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTHEAST, 0);
    Event_Wait(20);
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_SOLDIER_SOUTH, 2);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_SOLDIER_SOUTH, 0);
    Event_Wait(10);
    Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
    Event_ShowMessage(ACTOR_IODEM, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_SOLDIER_NORTH, 2);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_SOLDIER_NORTH, 0);
    Actor_RunRepeatedMotion(ACTOR_SOLDIER_NORTH, 2);
    Event_Wait(20);
    Event_Wait(10);
    Actor_FaceEachOther(ACTOR_IODEM, ACTOR_PARTY_LEADER, 0);
    Event_Wait(30);
    Event_ShowMessage(ACTOR_IODEM, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_SOLDIER_SOUTH, ANIM_SHAKE_HEAD);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_SOLDIER_SOUTH, 0);
    Event_Wait(10);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
    Actor_ShowEmote(ACTOR_SOLDIER_NORTH, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
    Event_ShowMessage(ACTOR_SOLDIER_NORTH, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_SOLDIER_NORTH, 2);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_SOLDIER_NORTH, 0);
    Actor_RunRepeatedMotion(ACTOR_SOLDIER_NORTH, 3);
    Event_Wait(20);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_IODEM, FACING_NORTH, 0);
    Event_Wait(30);
    Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
    Event_Wait(10);
    Event_ShowMessage(ACTOR_IODEM, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_SOLDIER_SOUTH, 2);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_SOLDIER_SOUTH, 0);
    Event_Wait(10);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_SOLDIER_SOUTH, 0);
    Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTHEAST, 0);
    Event_Wait(30);
    Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
    Event_Wait(30);
    Event_ShowMessage(ACTOR_IODEM, 0);
    Event_Wait(10);
    Actor_StartRepeatedMotion(ACTOR_SOLDIER_SOUTH, 2);
    Actor_RunRepeatedMotion(ACTOR_SOLDIER_NORTH, 2);
    Event_Wait(30);
    Event_Wait(10);
    Actor_FaceEachOther(ACTOR_IODEM, ACTOR_PARTY_LEADER, 20);
    Actor_WalkByAndWait(ACTOR_IODEM, -12, 0);
    Event_Wait(20);
    // Both answers converge on the following line after different waits.
    Event_OpenMessage(ACTOR_IODEM, 0);
    if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
      Event_Wait(20);
      Event_ShowMessage(ACTOR_IODEM, 0);
      gFieldWork->message++;
    } else {
      Event_Wait(10);
      gFieldWork->message++;
      Event_ShowMessage(ACTOR_IODEM, 0);
    }
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
    Event_Wait(20);
    Event_ShowMessage(ACTOR_IODEM, 0);
    Event_Wait(10);
    Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
    Event_ShowMessage(ACTOR_IODEM, 0);
    Event_Wait(10);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
    Event_Wait(30);
    Event_PlaySound(MUS_BABI);
    Actor_SetSpeed(ACTOR_IODEM, PIXELS(6) / 5, PIXELS(3) / 5);
    Actor_SetAnimation(ACTOR_IODEM, ANIM_WALK);
    // Iodem walks into the leader and is hidden: he travels in the party.
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (leader != 0) {
      Actor_SetDestination(ACTOR_IODEM, leader->x.part.pixel,
                           leader->z.part.pixel);
    }
    Actor_WaitForMoveById(ACTOR_IODEM);
    Actor_SetPosition(ACTOR_IODEM, 0, 0);
    Event_Wait(10);
    // Back to the area's own music.
    Scene_PlayMusic();
    Event_End();
  }
}

/// The doors and inner exits, people and ovens of Suhalla's buildings and
/// Suhalla Sanctum, before or after Babi's audience.
const struct Trigger *Scene_GetTriggers(void) {
  if (GameFlag_Get(FLAG_BABI_SPOKE_AFTER_COLOSSO) != 0) {
    return gTimberRoomsATriggersAfterAudience;
  }
  return gTimberRoomsATriggers;
}

/// Sets Suhalla's buildings up as the party enters: the merchant and
/// innkeeper behind their counters, and the two wounded soldiers lying in their
/// beds. TIMBER_ROOMS_A_AFTER_BABI_SPOKE sets
/// FLAG_BABI_SPOKE_AFTER_COLOSSO.
s32 Scene_Initialize(void) {
  struct Actor *actor;
  if (gMainRoutineWork.entrance == TIMBER_ROOMS_A_AFTER_BABI_SPOKE)
    GameFlag_Set(FLAG_BABI_SPOKE_AFTER_COLOSSO);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  gFieldWork->transitionFrames = 24;
  // Nearer to talk to, across their counters.
  Actor_GetById(ACTOR_MERCHANT)->collisionFlags |= ACTOR_NEAR_TALK;
  Actor_GetById(ACTOR_INNKEEPER)->collisionFlags |= ACTOR_NEAR_TALK;
  // Each soldier lies a quarter turn over, with no shadow.
  actor = Actor_GetById(ACTOR_SOLDIER_SOUTH);
  actor->sprite->flags = 0;
  actor->sprite->rotation = 0x4000;
  actor->sprite->image.priority = 1;
  actor = Actor_GetById(ACTOR_SOLDIER_NORTH);
  actor->sprite->flags = 0;
  actor->sprite->rotation = 0x4000;
  actor->motionFlags = ACTOR_FALLS;
  actor->y.fixed = 0;
  return 0;
}

struct SceneEntrance gTimberRoomsAEntrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {TIMBER_ROOMS_A_INN, CONDITION_ALWAYS, 256, 0, 456, FACING_NORTH, 0, 0, 280,
     336, 480, 0},
    {TIMBER_ROOMS_A_SANCTUM, CONDITION_ALWAYS, 432, 0, 728, FACING_NORTH, 0,
     312, 528, 552, 752, 0},
    {TIMBER_ROOMS_A_HOUSE_WEST, CONDITION_ALWAYS, 640, 0, 168, FACING_NORTH, 0,
     496, 16, 736, 192, 0},
    {TIMBER_ROOMS_A_HOUSE_NORTHEAST, CONDITION_ALWAYS, 112, 0, 168,
     FACING_NORTH, 0, 0, 16, 240, 208, 0},
    {TIMBER_ROOMS_A_HOUSE_EAST, CONDITION_ALWAYS, 352, 0, 216, FACING_NORTH, 0,
     256, 16, 496, 232, 0},
    {TIMBER_ROOMS_A_SANCTUM_NORTH, CONDITION_ALWAYS, 432, 0, 630, FACING_NORTH,
     0, 312, 528, 552, 752, 0},
    {TIMBER_ROOMS_A_HOUSE_WEST_UNUSED, CONDITION_ALWAYS, 680, 0, 120,
     FACING_WEST, 0, 496, 16, 736, 192, 0},
    {TIMBER_ROOMS_A_INN_FROM_GUEST_ROOM_WEST, CONDITION_ALWAYS, 104, 0, 360,
     FACING_NORTH, 0, 0, 280, 336, 480, 0},
    {TIMBER_ROOMS_A_INN_FROM_GUEST_ROOM_EAST, CONDITION_ALWAYS, 152, 0, 360,
     FACING_NORTH, 0, 0, 280, 336, 480, 0},
    {TIMBER_ROOMS_A_GUEST_ROOM_WEST_FROM_INN, CONDITION_ALWAYS, 472, 0, 344,
     FACING_SOUTH, 0, 344, 280, 584, 440, 0},
    {TIMBER_ROOMS_A_GUEST_ROOM_EAST_FROM_INN, CONDITION_ALWAYS, 712, 0, 344,
     FACING_SOUTH, 0, 600, 280, 840, 440, 0},
    {TIMBER_ROOMS_A_INN_FROM_LOFT, CONDITION_ALWAYS, 88, 0, 352, FACING_SOUTH,
     0, 0, 280, 336, 480, 0},
    {TIMBER_ROOMS_A_LOFT_FROM_INN, CONDITION_ALWAYS, 88, 0, 584, FACING_NORTH,
     0, 16, 528, 256, 688, 0},
    {SCENE_TABLE_END},
};

// clang-format off
struct SceneExitRegion gTimberRoomsARegions[] = {
    {83, 28, 356, 91, 36, 364, CONDITION_ALWAYS, TIMBER_ROOMS_A_INN_TO_LOFT},
    {83, -20, 596, 91, -12, 604, CONDITION_ALWAYS, TIMBER_ROOMS_A_LOFT_TO_INN},
    {-1, 0, 0, 0, 0, 0, 0, 0},
};

u32 gTimberRoomsAExits[] = {
    SCENE_EXITS(TimberRoomsA),
    SCENE_EXIT(TIMBER_ROOMS_A_INN_TO_SUHALLA, Suhalla, SUHALLA_FROM_INN),
    SCENE_EXIT(TIMBER_ROOMS_A_SANCTUM_TO_SUHALLA, Suhalla, SUHALLA_FROM_SANCTUM),
    SCENE_EXIT(TIMBER_ROOMS_A_HOUSE_WEST_TO_SUHALLA, Suhalla, SUHALLA_FROM_HOUSE_WEST),
    SCENE_EXIT(TIMBER_ROOMS_A_HOUSE_NORTHEAST_TO_SUHALLA, Suhalla, SUHALLA_FROM_HOUSE_NORTHEAST),
    SCENE_EXIT(TIMBER_ROOMS_A_HOUSE_EAST_TO_SUHALLA, Suhalla, SUHALLA_FROM_HOUSE_EAST),
    SCENE_EXIT(TIMBER_ROOMS_A_INN_TO_GUEST_ROOM_WEST, TimberRoomsA, TIMBER_ROOMS_A_GUEST_ROOM_WEST_FROM_INN),
    SCENE_EXIT(TIMBER_ROOMS_A_INN_TO_GUEST_ROOM_EAST, TimberRoomsA, TIMBER_ROOMS_A_GUEST_ROOM_EAST_FROM_INN),
    SCENE_EXIT(TIMBER_ROOMS_A_GUEST_ROOM_WEST_TO_INN, TimberRoomsA, TIMBER_ROOMS_A_INN_FROM_GUEST_ROOM_WEST),
    SCENE_EXIT(TIMBER_ROOMS_A_GUEST_ROOM_EAST_TO_INN, TimberRoomsA, TIMBER_ROOMS_A_INN_FROM_GUEST_ROOM_EAST),
    SCENE_EXIT(TIMBER_ROOMS_A_INN_TO_LOFT, TimberRoomsA, TIMBER_ROOMS_A_LOFT_FROM_INN),
    SCENE_EXIT(TIMBER_ROOMS_A_LOFT_TO_INN, TimberRoomsA, TIMBER_ROOMS_A_INN_FROM_LOFT),
    SCENE_EXITS_END,
};

struct ActorPlacement gTimberRoomsAActorPlacements[] = {
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
    { SCENE_TABLE_END },
};

struct ActorPlacement gTimberRoomsAActorPlacementsAfterAudience[] = {
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
    { SCENE_TABLE_END },
};

struct Trigger gTimberRoomsATriggers[] = {
    { TRIGGER_STEP, 1, CONDITION_ALWAYS, EVENT(TimberRoomsA_LeaveByExit) },
    { TRIGGER_STEP, 2, CONDITION_ALWAYS, EVENT(TimberRoomsA_LeaveByExit) },
    { TRIGGER_STEP, 3, CONDITION_ALWAYS, EVENT(TimberRoomsA_LeaveByExit) },
    { TRIGGER_STEP, 4, CONDITION_ALWAYS, EVENT(TimberRoomsA_LeaveByExit) },
    { TRIGGER_STEP, 5, CONDITION_ALWAYS, EVENT(TimberRoomsA_LeaveByExit) },
    { TRIGGER_STEP, 10, CONDITION_ALWAYS, EVENT(TimberRoomsA_LeaveByExit) },
    { TRIGGER_STEP, 11, CONDITION_ALWAYS, EVENT(TimberRoomsA_LeaveByExit) },
    { TRIGGER_STEP, 12, CONDITION_ALWAYS, EVENT(TimberRoomsA_LeaveByExit) },
    { TRIGGER_STEP, 13, CONDITION_ALWAYS, EVENT(TimberRoomsA_LeaveByExit) },
    { TRIGGER_TALK, ACTOR_FATHERS_DAUGHTER, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsMyFatherLovesPeopleSoMuch) },
    { TRIGGER_TALK, ACTOR_FATHER_WHO_WANTS_TO_HELP, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsLetMeKnowIfYouHave) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FATHERS_DAUGHTER, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsWhenISeePeopleInTrouble) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FATHER_WHO_WANTS_TO_HELP, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsIWantToHelpPeopleSo) },
    { TRIGGER_TALK, ACTOR_OLD_MAN_IGNORED_BY_TOLBI, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsTolbiProbablyIgnoresSmallVillagesLike) },
    { TRIGGER_TALK, ACTOR_OLD_WOMAN, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsIfYouHeadEastAcrossSuhalla) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_IGNORED_BY_TOLBI, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsSuhallaIsADesertVillageI) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsGoodThingItsNotEasyTo) },
    { TRIGGER_TALK, ACTOR_MERCHANT, CONDITION_ALWAYS, EVENT(TimberRoomsA_TalkToMerchant) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsAVillageThisSmallCantSupport) },
    { TRIGGER_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, EVENT(TimberRoomsA_TalkToInnkeeper) },
    { TRIGGER_TALK, ACTOR_INN_WOMAN, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsIWonderIfAnyCustomersCan) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsOnceTheFestivalEndsTheTravelers) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_WOMAN, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsForSomeReasonAnyoneWhoGets) },
    { TRIGGER_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT(TimberRoomsA_TalkToHealer) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsTheHolyChildActorWillSave) },
    { EXAMINE_TRIGGER(EXAMINE_OVEN), 100, FLAG_TIMBER_ROOMS_A_OVEN_SMOKE_BOMB, FIND_ITEM(ITEM_SMOKE_BOMB) },
    // Unused: this oven's item row answers first (see the maps README).
    { EXAMINE_TRIGGER(EXAMINE_OVEN), 100, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgSuhallaInteriorsItsACactusSteakIWonder)) },
    { EXAMINE_TRIGGER(EXAMINE_OVEN), 101, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgSuhallaInteriorsHesMakingMulukhiyaSoupItTastes)) },
    { TRIGGERS_END },
};

struct Trigger gTimberRoomsATriggersAfterAudience[] = {
    { TRIGGER_STEP, 50, FLAG_IODEM_MET_WOUNDED_SOLDIERS, EVENT(TimberRoomsA_IodemQuestionsWoundedSoldiers) },
    { TRIGGER_STEP, 1, CONDITION_ALWAYS, EVENT(TimberRoomsA_LeaveByExit) },
    { TRIGGER_STEP, 2, CONDITION_ALWAYS, EVENT(TimberRoomsA_LeaveByExit) },
    { TRIGGER_STEP, 3, CONDITION_ALWAYS, EVENT(TimberRoomsA_LeaveByExit) },
    { TRIGGER_STEP, 4, CONDITION_ALWAYS, EVENT(TimberRoomsA_LeaveByExit) },
    { TRIGGER_STEP, 5, CONDITION_ALWAYS, EVENT(TimberRoomsA_LeaveByExit) },
    { TRIGGER_STEP, 10, CONDITION_ALWAYS, EVENT(TimberRoomsA_LeaveByExit) },
    { TRIGGER_STEP, 11, CONDITION_ALWAYS, EVENT(TimberRoomsA_LeaveByExit) },
    { TRIGGER_STEP, 12, CONDITION_ALWAYS, EVENT(TimberRoomsA_LeaveByExit) },
    { TRIGGER_STEP, 13, CONDITION_ALWAYS, EVENT(TimberRoomsA_LeaveByExit) },
    { TRIGGER_TALK, ACTOR_FATHERS_DAUGHTER, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsTheWoundedAreSleepingSoDont) },
    { TRIGGER_TALK, ACTOR_FATHER_WHO_WANTS_TO_HELP, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsIAmAmazedTheseSoldiersSurvived) },
    { TRIGGER_TALK, ACTOR_SOLDIER_SOUTH, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsWhyWereTheyHeadedTowardEastern) },
    { TRIGGER_TALK, ACTOR_SOLDIER_NORTH, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsWhereIsActorWhatHasHappened) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FATHERS_DAUGHTER, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsTheseOldSoldiersMustRestUntil) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FATHER_WHO_WANTS_TO_HELP, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsWhoCouldHaveDoneThisTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SOLDIER_SOUTH, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsAColumnOfWaterShotUp) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SOLDIER_NORTH, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsActorWasNotFarBehindUs) },
    { TRIGGER_TALK, ACTOR_OLD_MAN_IGNORED_BY_TOLBI, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsDoNotApproachTheLargeCyclones) },
    { TRIGGER_TALK, ACTOR_OLD_WOMAN, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsDeathStormsHuhThereWasA) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_IGNORED_BY_TOLBI, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsOnceYouGetSweptUpIn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsYeWhoSeekSomethingAboutEyes) },
    { TRIGGER_TALK, ACTOR_MERCHANT, CONDITION_ALWAYS, EVENT(TimberRoomsA_TalkToMerchant) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsWeveHadMoreCustomersLatelyBecause) },
    { TRIGGER_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, EVENT(TimberRoomsA_TalkToInnkeeper) },
    { TRIGGER_TALK, ACTOR_INN_WOMAN, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsItWillTakeTimeForThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsTheSandstormsAreSendingCustomersOur) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_WOMAN, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsPleaseGreatSandstormsSendUsMore) },
    { TRIGGER_TALK, ACTOR_LIGHTHOUSE_WORKER, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsWeWentIntoTheSuhallaTo) },
    { TRIGGER_TALK, ACTOR_SECOND_LIGHTHOUSE_WORKER, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsTheSandstormsAreSoBadWeve) },
    { TRIGGER_TALK, ACTOR_STRANDED_TRAVELER, CONDITION_ALWAYS, EVENT(TimberRoomsA_AskIfBlownHere) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LIGHTHOUSE_WORKER, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsEvenIfWeCantGoIt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_LIGHTHOUSE_WORKER, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsSoShallWeGiveUpOn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRANDED_TRAVELER, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsWhenThatCycloneThrewUsInto) },
    { TRIGGER_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT(TimberRoomsA_TalkToHealer) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsIShudderToThinkSandstormsWill) },
    { EXAMINE_TRIGGER(EXAMINE_OVEN), 100, FLAG_TIMBER_ROOMS_A_OVEN_SMOKE_BOMB, FIND_ITEM(ITEM_SMOKE_BOMB) },
    // Unused: this oven's item row answers first (see the maps README).
    { EXAMINE_TRIGGER(EXAMINE_OVEN), 100, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgSuhallaInteriorsItsACactusSteakIWonder)) },
    { EXAMINE_TRIGGER(EXAMINE_OVEN), 101, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgSuhallaInteriorsHesMakingMulukhiyaSoupItTastes)) },
    { TRIGGERS_END },
};
// clang-format on
