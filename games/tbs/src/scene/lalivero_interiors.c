/// @file Lalivero's buildings: two scenes in one overlay, with the area name
/// Lalivero, and Lalivero Sanctum for the Sanctum's rooms of LaliveroShops
/// (LALIVERO_SHOPS_FROM_LALIVERO_NORTHEAST and LALIVERO_SHOPS_SANCTUM). Each
/// scene has tables for before and after Venus Lighthouse, chosen by
/// FLAG_RETURNED_FROM_VENUS_LIGHTHOUSE, as the town's are (lalivero.c).
///
/// SceneId_LaliveroShops holds the houses, the weapon, armor and item shops and
/// the Sanctum. Each shopkeeper stands behind a counter and trades only with a
/// leader facing north across it (Event_TalkToWeaponMerchant and the others);
/// from any other side they talk. Stepping into the Sanctum by
/// LALIVERO_SHOPS_SANCTUM makes it the place a defeated party wakes. Barrels
/// hold a Lucky Medal and an Antidote.
///
/// SceneId_LaliveroFaransHouse holds Faran's house, the inn and the way out to
/// the wharf where Babi's ship lies (LALIVERO_FARANS_HOUSE_TO_LEMURIAN_SHIP).
/// Before Venus Lighthouse an inn worker apologizes once for her distraction
/// (Event_TalkToInnWorker, FLAG_INN_WORKER_APOLOGIZED) and goes back to her
/// chores. The aerie of Venus Lighthouse leads in by
/// LALIVERO_FARANS_HOUSE_FROM_VENUS_LIGHTHOUSE_AERIE, which plays
/// Event_ReportToFaran: the party reports to Faran and Iodem, in one of two
/// versions by FLAG_SAW_SHIP_AT_IDEJIMA. Trigger 51 afterwards plays
/// Event_IodemCallsOut (FLAG_IODEM_ASKED_TO_MEET), which sends the party
/// outside, where Iodem gives it the Black Orb (lalivero.c). A barrel holds a
/// Lucky Medal.
///
/// Open questions:
/// - XXX: what animation 6 shows of the napping worker upstairs in the
///   northwest house; his sprite's animation table or a recording would settle
///   a name in enum ActorAnimation.

#include "script.h"
#include "game_state.h"
#include "scene_ids.h"
#include "unit/import.h"
#include "unit/scene/lalivero_interiors.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/game/game.h"
#include "unit/shop/shop.h"
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
#include "field_event.h"
#include "field_scene.h"
#include "field_service.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"
#include "songs.h"
#include "flags.h"
static void LaliveroInteriors_TellFaranAboutJenna(void);

/// An entrance that sets FLAG_RETURNED_FROM_VENUS_LIGHTHOUSE. No exit leads to
/// it and the entrance table has no row for it: only the debug scene selector,
/// which warps to any entrance from 0 to 99, reaches it.
enum {
  ENTRANCE_FROM_AERIE = 90,
};

/// The shops' stock rows, whose type column in sStockTable says they sell
/// weapons, armor and items, and the inn.
enum {
  LALIVERO_WEAPON_STOCK = 32,
  LALIVERO_ARMOR_STOCK = 33,
  LALIVERO_ITEM_STOCK = 34,
  LALIVERO_INN = 11
};

/// The actors of both scenes, numbered by the rows of each scene's placement
/// tables. The people are named by what they say before Venus Lighthouse and by
/// what their sprites show.
enum LaliveroInteriorsActor {
  // The houses, the shops and the Sanctum (LaliveroShops).
  /// A young man with brown hair in a purple shirt, in the house at
  /// LALIVERO_SHOPS_FROM_LALIVERO_WEST. "Of course I have seen Sheba's power...
  /// It's incredible!"
  /// ★ Named from the game: "Of course I have seen"
  /// (MsgLaliveroInteriorsOfCourseIHaveSeenActor)
  ACTOR_VILLAGER_SAW_SHEBAS_POWER = ACTOR_FIRST_PLACED,
  /// A woman with fair hair, in the same house. "There are some who do not
  /// believe Sheba has any powers... Doubting Sheba? That is foolish!"
  /// ★ Named from the game: "That is foolish!"
  /// (MsgLaliveroInteriorsThereAreSomeWhoDoNot)
  ACTOR_WOMAN_DEFENDING_SHEBA,
  /// A man in a white turban, in the house at
  /// LALIVERO_SHOPS_FROM_LALIVERO_SOUTHEAST. "Could Faran have tricked us all?"
  /// ★ Named from the game: "Could Faran have tricked us all?"
  /// (MsgLaliveroInteriorsCouldFaranHaveTrickedUsAll)
  ACTOR_VILLAGER_DOUBTING_FARAN,
  /// A woman in a green dress, in the same house. "If Sheba can't use her
  /// powers in a time like this, they're useless!"
  /// ★ Named from the game: "use her powers in a time like this, they're
  /// useless!" (MsgLaliveroInteriorsIfActorCantUseHerPowers)
  ACTOR_WOMAN_DOUBTING_SHEBA,
  /// An old man with a cane, upstairs in the item shop at
  /// LALIVERO_SHOPS_FROM_LALIVERO_NORTH. "Sheba came flying to the ruins like a
  /// shooting star."
  /// ★ Named from the game: "came flying to the ruins like a shooting star."
  /// (MsgLaliveroInteriorsActorCameFlyingToTheRuins)
  ACTOR_OLD_MAN_SHOOTING_STAR,
  /// An old woman, in the same room. "Even Sheba couldn't have escaped six of
  /// them..."
  /// ★ Named from the game: "couldn't have escaped six of them..."
  /// (MsgLaliveroInteriorsEvenActorCouldntHaveEscapedSix)
  ACTOR_OLD_WOMAN_SIX_OF_THEM,
  /// A bald, muscular man, in the house at
  /// LALIVERO_SHOPS_FROM_LALIVERO_NORTHWEST. "They say a man named Iodem is
  /// coming from Tolbi. I guess he plans to make us do even more work!"
  /// ★ Named from the game: "I guess he plans to make us do even more work!"
  /// (MsgLaliveroInteriorsTheySayAManNamedIodem)
  ACTOR_WORKER,
  /// A bald, muscular man upstairs in that house, posed in animation 6. "No
  /// one's called me, so maybe I'll take a nap..."; after Venus Lighthouse,
  /// Mind Read hears "I'm worried that we won't get paid now that the
  /// construction has ended."
  /// ★ Named from the game: "No one's called me, so maybe I'll take a nap..."
  /// (MsgLaliveroInteriorsNoOnesCalledMeSoMaybe)
  ACTOR_NAPPING_WORKER,
  /// A strongman, in the house at LALIVERO_SHOPS_FROM_LALIVERO_NORTHWEST.
  /// "Sheba said the cave at the ruins leads to the southeast."
  /// ★ Named from the game: "said the cave at the ruins leads to the
  /// southeast." (MsgLaliveroInteriorsActorSaidTheCaveAtThe)
  ACTOR_STRONGMAN_RUINS_CAVE,
  /// A strongman who walks about the room upstairs in that house. "The ruins
  /// that form the lighthouse foundation are riddled with caves."
  /// ★ Named from the game: "The ruins that form the lighthouse foundation are
  /// riddled with caves." (MsgLaliveroInteriorsTheRuinsThatFormTheLighthouse)
  ACTOR_STRONGMAN_RUINS_RIDDLE,
  /// ★ Named from what the player sees: the man behind the counter of the
  /// shop hung with swords and shields (its render)
  ACTOR_WEAPON_MERCHANT,
  /// ★ Named from what the player sees: the second man behind the counter of
  /// the shop hung with swords and shields (its render)
  ACTOR_ARMOR_MERCHANT,
  /// ★ Named from what the player sees: the young woman with a ponytail behind
  /// the counter of the shop stacked with barrels and jars (its render)
  ACTOR_ITEM_MERCHANT,
  /// The Sanctum's healer.
  /// ★ Named from what the player sees: the bearded man behind the Sanctum's
  /// counter (its render)
  ACTOR_HEALER,

  // Faran's house and the inn (LaliveroFaransHouse).
  /// Faran's wife, who raised Sheba. "We received Sheba from the sky and
  /// raised her lovingly... I know Faran will rescue that child."
  /// ★ Named from the game: "His wife seems very calm, considering."
  /// (MsgLaliveroInteriorsHisWifeSeemsVeryCalmConsidering)
  ACTOR_FARANS_WIFE = ACTOR_FIRST_PLACED,
  /// Faran's son, a small boy with fair hair, younger than Sheba. "I just know
  /// that Father will save my elder sister, Sheba."
  /// ★ Named from the game: "I just know that Father will save my elder sister"
  /// (MsgLaliveroInteriorsIJustKnowThatFatherWill)
  ACTOR_FARANS_SON,
  /// A young woman with long brown hair, worried for Sheba. "His wife seems
  /// very calm, considering."; Mind Read hears "When someone like Sheba is
  /// kidnapped, it worries us all!"
  /// ★ Named from the game: "Deep down, she must be worried about"
  /// (MsgLaliveroInteriorsHisWifeSeemsVeryCalmConsidering)
  ACTOR_WORRIED_YOUNG_WOMAN,
  /// Placed off the map before Venus Lighthouse.
  /// ★ Named from the game: "Faran followed"
  /// (MsgLaliveroFaranFollowedActorImSureHell)
  ACTOR_FARAN,
  /// Placed off the map before Venus Lighthouse.
  /// ★ Named from the game: "Iodem of Tolbi"
  /// (MsgLaliveroIodemOfTolbiDoesntSeemToo)
  ACTOR_IODEM,
  /// A man with dark hair.
  /// ★ Named from the game: "Welcome to our inn." (MsgInnWelcome)
  ACTOR_INNKEEPER,
  /// A woman in a pink dress and a white apron who works about the inn, too
  /// worried about Sheba to keep her mind on it.
  /// ★ Named from the game: "Working like this keeps my mind occupied."
  /// (MsgLaliveroInteriorsWorkingLikeThisKeepsMyMind)
  ACTOR_INN_WORKER,
  /// A young man with brown hair in a purple shirt who walks about the hall
  /// upstairs in the inn. After Venus Lighthouse: "Fishing is so difficult
  /// nowadays because the ocean is so rough. We have little to cook when we
  /// can't fish."
  /// ★ Named from the game: "We have little to cook when we can't fish."
  /// (MsgLaliveroInteriorsFishingIsSoDifficultNowadaysBecause)
  ACTOR_COOK
};

/// The place flag FLAG_PLACE_TEMP_00, which lasts until the party enters
/// another place: the inn worker has apologized.
/// ★ Named from the game: "Oh, I'm sorry..." (MsgLaliveroImSorry)
enum { FLAG_INN_WORKER_APOLOGIZED = FLAG_PLACE_TEMP_00 };

/// LaliveroFaransHouse: the inn worker sighs between chores.
static s32 InnWorker_Sigh(struct ScriptInterpreter *interpreter) {
  Actor_SetAttachedEffect(ACTOR_INN_WORKER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  return 0;
}

const struct SceneEntrance *LaliveroInteriors_GetEntrances(void) {
  return gLaliveroInteriors_Entrances;
}

const struct SceneRegion *LaliveroInteriors_GetRegions(void) {
  if (gGameState.scene == (s32)&SceneId_LaliveroShops) {
    return gLaliveroInteriors_ShopsRegions;
  }
  return gLaliveroInteriors_FaransHouseRegions;
}

const u32 *LaliveroInteriors_GetExits(void) { return gLaliveroInteriors_Exits; }

/// The people of each scene; Faran's house fills up after Venus Lighthouse.
const struct ScenePlacement *LaliveroInteriors_GetPlacements(void) {
  if (gGameState.scene == (s32)&SceneId_LaliveroFaransHouse) {
    // Faran's house and the inn.
    if (Engine_GameFlagIsSet(FLAG_RETURNED_FROM_VENUS_LIGHTHOUSE) != 0) {
      return gLaliveroInteriors_FaransHousePlacementsAfterLighthouse;
    }
    return gLaliveroInteriors_FaransHousePlacements;
  }
  return gLaliveroInteriors_ShopsPlacements;
}

/// The side the leader faces: the facing rounded to the nearest of east,
/// south, west and north. Facing north, the leader is across the counter
/// from a shopkeeper.
static inline u16 Leader_GetSide(void) {
  return (Object_GetById(ACTOR_PARTY_LEADER)->facing + 0x2000) & ~0x3fff;
}

/// The weapon merchant sells weapons, or talks of Sheba and the sea. In
/// Lalivero's shops (LaliveroShops), across the counter, it's the weapon shop;
/// from elsewhere, before Venus Lighthouse, they ask whether the party has
/// heard that Sheba was taken hostage and Faran went after her, and after it
/// tell of the rough sea.
static void Event_TalkToWeaponMerchant(s32 keeper) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  // Leader_GetSide's rounding, written with the mask 0xc000, which
  // loads from the literal pool where the helper's builds the mask.
  u16 side = (leader->facing + 0x2000) & 0xc000;

  if (side == FACING_NORTH) {
    Shop_Run(LALIVERO_WEAPON_STOCK, keeper);
  } else if (Engine_GameFlagIsSet(FLAG_RETURNED_FROM_VENUS_LIGHTHOUSE)) {
    Engine_EventSetMessage(MsgLaliveroWentOceanLook);
    Engine_EventShowMessage(keeper, 0);
  } else {
    s32 message = MsgLaliveroTakenHostageFaran;

    Engine_EventSetMessage(message);
    Engine_EventOpenMessage(keeper, 0);
    if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
      Engine_EventWait(10);
      Engine_EventSetMessage(message + 1);
    } else {
      Engine_EventSetMessage(message + 2);
    }
    Engine_EventShowMessage(keeper, 0);
  }
}

/// Lalivero (LaliveroShops): the armor shop across the counter; otherwise
/// small talk.
static void Event_TalkToArmorMerchant(s32 keeper) {
  if (Leader_GetSide() == FACING_NORTH) {
    Shop_Run(LALIVERO_ARMOR_STOCK, keeper);
  } else if (Engine_GameFlagIsSet(FLAG_RETURNED_FROM_VENUS_LIGHTHOUSE)) {
    Engine_EventSetMessage(MsgLaliveroWantedSendShips);
    Engine_EventShowMessage(keeper, 0);
  } else {
    Engine_EventSetMessage(MsgLaliveroWarriorRightPlease);
    Engine_EventShowMessage(keeper, 0);
  }
}

/// Lalivero (LaliveroShops): the item shop across the counter; otherwise
/// small talk.
static void Event_TalkToItemMerchant(s32 keeper) {
  if (Leader_GetSide() == FACING_NORTH) {
    Shop_Run(LALIVERO_ITEM_STOCK, keeper);
  } else if (Engine_GameFlagIsSet(FLAG_RETURNED_FROM_VENUS_LIGHTHOUSE)) {
    Engine_EventSetMessage(MsgLaliveroImpossibleGoOcean);
    Engine_EventShowMessage(keeper, 0);
  } else {
    Engine_EventSetMessage(MsgLaliveroGuessPeopleAngara);
    Engine_EventShowMessage(keeper, 0);
  }
}

/// Lalivero Sanctum: the healer offers the Sanctum's services across the
/// counter; otherwise small talk of Babi's lighthouse.
static void Event_TalkToHealer(s32 healer) {
  if (Leader_GetSide() == FACING_NORTH) {
    Shop_ConfirmAct(healer);
  } else if (Engine_GameFlagIsSet(FLAG_RETURNED_FROM_VENUS_LIGHTHOUSE)) {
    Engine_EventSetMessage(MsgLaliveroBabiLighthouseStruck);
    Engine_EventShowMessage(healer, 0);
  } else {
    Engine_EventSetMessage(MsgLaliveroBabiIsntBuilding);
    Engine_EventShowMessage(healer, 0);
  }
}

/// Lalivero (LaliveroFaransHouse), the inn: a room for the night across the
/// counter; otherwise small talk.
static void Event_TalkToInnkeeper(s32 innkeeper) {
  if (Leader_GetSide() == FACING_NORTH) {
    Inn_CheckIn(LALIVERO_INN, innkeeper);
  } else if (Engine_GameFlagIsSet(FLAG_RETURNED_FROM_VENUS_LIGHTHOUSE)) {
    Engine_EventSetMessage(MsgLaliveroDidntKnowTime);
    Engine_EventShowMessage(innkeeper, 0);
  } else {
    Engine_EventSetMessage(MsgLaliveroWantStayLet);
    Engine_EventShowMessage(innkeeper, 0);
  }
}

/// Lalivero (LaliveroFaransHouse), before Venus Lighthouse: the inn worker
/// stops her chores to apologize: worry for Sheba keeps her from concentrating.
/// A leader standing west of her steps aside to the south before she starts
/// again.
static void Event_TalkToInnWorker(void) {
  s16 side;

  side = Leader_GetSide();
  GameFlag_Set(FLAG_INN_WORKER_APOLOGIZED);
  Engine_EventBegin();
  Engine_FieldEndBusyAndReveal();
  Engine_EventSetMessage(MsgLaliveroImSorry);
  Engine_EventWait(50);
  Actor_ShowEmote(ACTOR_INN_WORKER, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
  Actor_FaceActor(ACTOR_INN_WORKER, ACTOR_PARTY_LEADER, 20);
  Event_ShowMessage(ACTOR_INN_WORKER, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_INN_WORKER, ANIM_SHAKE_HEAD);
  Engine_EventWait(30);
  Event_ShowMessage(ACTOR_INN_WORKER, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_INN_WORKER, 2);
  Engine_EventWait(30);
  Event_ShowMessage(ACTOR_INN_WORKER, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_INN_WORKER, ANIM_NOD);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_INN_WORKER, 0);
  if ((u16)side == FACING_WEST) {
    Engine_ActorWalkByAndWait(ACTOR_PARTY_LEADER, 0, 16);
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Engine_EventWait(20);
  }
  Actor_SetActionScript(ACTOR_INN_WORKER, gLaliveroInteriors_InnWorkerChores);
  Engine_EventEnd();
}

/// Lalivero (LaliveroFaransHouse), trigger 51 in Faran's house after the
/// report: the camera finds Iodem, who asks the party to wait for him outside.
static void Event_IodemCallsOut(void) {

  Engine_GameFlagSet(FLAG_IODEM_ASKED_TO_MEET);
  Engine_EventBegin();
  Engine_FieldEndBusyAndReveal();
  Engine_EventWait(10);
  Engine_CameraMoveTo(PIXELS(120), -1, PIXELS(96), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(30);
  Engine_EventSetMessage(MsgLaliveroPleaseWaitForMeOutside);
  Engine_EventShowMessage(ACTOR_IODEM, 0);
  Engine_EventWait(10);
  Engine_ActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_IODEM, 0);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(30);
  Engine_EventEnd();
}

/// Lalivero (LaliveroFaransHouse): Iodem, talked to before he has given the
/// Black Orb, repeats his request.
static void Event_TalkToIodem(void) {

  Engine_GameFlagSet(FLAG_IODEM_ASKED_TO_MEET);
  Engine_EventSetMessage(MsgLaliveroPleaseWaitForMeOutside);
  Engine_EventShowMessage(ACTOR_IODEM, 0);
}

/// Both scenes, a doorway out: take the touched trigger's exit with the exit
/// sound.
static void Event_LeaveBuilding(void) {
  struct EventWork *work = gEventWork;
  s16 *trigger = (s16 *)&work->touchedTrigger;

  Engine_EventRequestExit(*trigger);
  Engine_AudioPlayCue(SE_MAP_EXIT);
}

/// What the party can talk to, touch and search in each scene, before and
/// after Venus Lighthouse.
const struct SceneEvent *LaliveroInteriors_GetEvents(void) {
  if (gGameState.scene == (s32)&SceneId_LaliveroFaransHouse) {
    if (Engine_GameFlagIsSet(FLAG_RETURNED_FROM_VENUS_LIGHTHOUSE) != 0) {
      return gLaliveroInteriors_FaransHouseEventsAfterLighthouse;
    }
    return gLaliveroInteriors_FaransHouseEvents;
  }
  if (Engine_GameFlagIsSet(FLAG_RETURNED_FROM_VENUS_LIGHTHOUSE) != 0) {
    return gLaliveroInteriors_ShopsEventsAfterLighthouse;
  }
  return gLaliveroInteriors_ShopsEvents;
}

/// Lalivero (LaliveroFaransHouse),
/// LALIVERO_FARANS_HOUSE_FROM_VENUS_LIGHTHOUSE_AERIE back from Venus
/// Lighthouse: the party reports to Faran and Iodem. No sign of Sheba, nor of
/// Jenna and the others; Faran's faith that Sheba lived, what an Adept is, and
/// a farewell as the party sets out to search for Jenna.
static void Event_ReportToFaran(void) {
  struct FieldActor *leader;

  Engine_EventBegin();
  Party_RestoreHpPp(1);
  // The party walks in to Faran and Iodem, and the others step out.
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(96), PIXELS(184));
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_FARAN, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_IODEM, FACING_WEST, 0);
  Engine_EventSetMessage(MsgLaliveroHowDidSearchForSheba);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(10);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
  Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 0, -16);
  Engine_ActorWalkToAndWait(ACTOR_PARTY_LEADER, 104, 136);
  Engine_EventWait(10);
  FieldMotion_LaunchFromFocusedObject(GARET, -16, 16, FACING_NORTH);
  FieldMotion_LaunchFromFocusedObject(MIA, 0, 24, FACING_NORTH);
  FieldMotion_LaunchFromFocusedObject(IVAN, 16, 16, FACING_NORTH);
  Engine_ActorWaitForMove(GARET);
  Engine_EventWait(20);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Engine_EventWait(20);
  Engine_EventShowMessage(IVAN, 0);
  Engine_EventWait(10);
  // Faran and Iodem tell of the search for Sheba.
  Actor_FaceDirection(ACTOR_FARAN, FACING_SOUTH, 0);
  Engine_ActorFaceDirection(ACTOR_IODEM, FACING_SOUTH, 0);
  Engine_EventWait(30);
  Actor_ShowEmote(ACTOR_FARAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 70);
  Engine_EventShowMessage(ACTOR_FARAN, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_FARAN, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Engine_EventShowMessage(ACTOR_FARAN, 0);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(ACTOR_FARAN, 2);
  Engine_EventWait(20);
  Engine_EventShowMessage(ACTOR_FARAN, 0);
  Engine_EventWait(20);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Engine_EventShowMessage(ACTOR_IODEM, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_IODEM, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Engine_EventShowMessage(ACTOR_IODEM, 0);
  Engine_EventWait(20);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_FARAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 50);
  Engine_EventShowMessage(ACTOR_FARAN, 0);
  // The party tells what became of Jenna and the others; the version that saw
  // the ship at Idejima knows Idejima was swept away with it.
  if (GameFlag_IsSet(FLAG_SAW_SHIP_AT_IDEJIMA) == 0) {
    LaliveroInteriors_TellFaranAboutJenna();
  } else {
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(ACTOR_FARAN, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(ACTOR_FARAN, 0);
    Engine_EventWait(10);
    Actor_FaceActor(GARET, ACTOR_PARTY_LEADER, 50);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
    Engine_EventWait(10);
    Engine_ActorFaceDirection(GARET, FACING_NORTH, 0);
    Engine_EventWait(20);
    Engine_EventShowMessage(GARET, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(MIA, ANIM_SHAKE_HEAD);
    Engine_EventWait(20);
    Engine_EventShowMessage(MIA, 0);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(IVAN, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(IVAN, 0);
    Engine_EventWait(20);
    Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
    Event_OpenMessage(ACTOR_IODEM, 0);
    Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(ACTOR_IODEM, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(ACTOR_IODEM, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_EventWait(30);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(GARET, 2);
    Engine_EventWait(20);
    Engine_EventShowMessage(GARET, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(MIA, ANIM_SHAKE_HEAD);
    Engine_EventWait(20);
    Engine_EventShowMessage(MIA, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
    Engine_EventWait(30);
    Engine_EventShowMessage(IVAN, 0);
    Engine_EventWait(10);
    Actor_FaceActor(IVAN, ACTOR_PARTY_LEADER, 30);
    Event_OpenMessage(IVAN | SPEAKER_PORTRAIT_RIGHT, 0);
  }
  // Will the party keep searching? Faran answers either way.
  Engine_EventSetMessage(MsgLaliveroDoNotWorryAboutSheba);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceActor(GARET, ACTOR_PARTY_LEADER, 20);
  if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
    Engine_EventWait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(GARET, FACING_NORTH, 0);
    Engine_ActorFaceDirection(IVAN, FACING_NORTH, 0);
    Engine_EventWait(30);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(ACTOR_FARAN, ANIM_NOD);
    Engine_EventWait(30);
    Engine_EventShowMessage(ACTOR_FARAN, 0);
    (gEventWork->message)++;
  } else {
    Engine_EventWait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(GARET, FACING_NORTH, 0);
    Engine_ActorFaceDirection(IVAN, FACING_NORTH, 0);
    Engine_EventWait(30);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(ACTOR_FARAN, ANIM_SHAKE_HEAD);
    Engine_EventWait(20);
    (gEventWork->message)++;
    Event_ShowMessage(ACTOR_FARAN, 0);
  }
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_FARAN, 2);
  Engine_EventWait(20);
  Engine_EventShowMessage(ACTOR_FARAN, 0);
  Engine_EventWait(10);
  Engine_ActorFaceEachOther(GARET, ACTOR_PARTY_LEADER, 0);
  Engine_ActorFaceEachOther(MIA, IVAN, 0);
  Engine_EventWait(40);
  Engine_EventWait(10);
  // Faran believes Sheba lived; Iodem explains Adepts.
  Actor_ShowEmote(ACTOR_FARAN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Engine_EventShowMessage(ACTOR_FARAN, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Engine_EventWait(50);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_FARAN, ANIM_NOD);
  Engine_EventWait(30);
  Engine_EventShowMessage(ACTOR_FARAN, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_FARAN, 2);
  Engine_EventWait(20);
  Engine_EventShowMessage(ACTOR_FARAN, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Engine_EventShowMessage(MIA, 0);
  Engine_EventWait(20);
  Engine_ActorFaceDirection(ACTOR_FARAN, FACING_EAST, 0);
  Engine_EventWait(20);
  Engine_EventShowMessage(ACTOR_FARAN, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_IODEM, 2);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_IODEM, FACING_WEST, 0);
  Engine_EventWait(20);
  Engine_EventShowMessage(ACTOR_IODEM, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_FARAN, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Engine_EventShowMessage(ACTOR_FARAN, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Engine_EventWait(20);
  Engine_EventShowMessage(IVAN, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_FARAN, FACING_SOUTH, 0);
  Engine_EventWait(30);
  Engine_EventShowMessage(ACTOR_FARAN, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
  Engine_EventShowMessage(ACTOR_IODEM, 0);
  Engine_EventWait(20);
  Actor_ShowEmote(ACTOR_FARAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 65);
  Engine_ActorFaceDirection(ACTOR_FARAN, FACING_EAST, 0);
  Engine_EventWait(20);
  Engine_EventShowMessage(ACTOR_FARAN, 0);
  Engine_EventWait(20);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Engine_EventShowMessage(ACTOR_IODEM, 0);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTH, 0);
  Engine_EventWait(40);
  Engine_EventShowMessage(ACTOR_IODEM, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_FARAN, FACING_SOUTH, 0);
  Engine_EventWait(50);
  Actor_ShowEmote(ACTOR_FARAN, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 60);
  Engine_EventShowMessage(ACTOR_FARAN, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_FARAN, ANIM_NOD);
  Engine_EventWait(30);
  Engine_EventShowMessage(ACTOR_FARAN, 0);
  Engine_EventWait(10);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_ActorStartRepeatedMotion(MIA, 2);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Engine_EventWait(30);
  Engine_EventWait(10);
  Engine_ActorFaceDirection(ACTOR_FARAN, FACING_EAST, 0);
  Engine_EventWait(20);
  Engine_EventShowMessage(ACTOR_FARAN, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_IODEM, 2);
  Engine_EventWait(30);
  Actor_FaceDirection(ACTOR_IODEM, FACING_WEST, 0);
  Engine_EventWait(20);
  Engine_EventShowMessage(ACTOR_IODEM, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_FARAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Engine_EventShowMessage(GARET, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTH, 0);
  Engine_EventWait(30);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Engine_EventShowMessage(ACTOR_IODEM, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(20);
  Engine_EventShowMessage(GARET, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_FARAN, FACING_SOUTH, 0);
  Engine_EventWait(40);
  Actor_ShowEmote(ACTOR_FARAN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Engine_EventShowMessage(ACTOR_FARAN, 0);
  Engine_EventWait(10);
  Engine_ActorFaceDirection(IVAN, FACING_WEST, 0);
  Engine_EventWait(30);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_SCOWL, 60);
  Engine_ActorFaceDirection(IVAN, FACING_NORTH, 0);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Engine_EventShowMessage(IVAN, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_FARAN, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Engine_EventShowMessage(ACTOR_FARAN, 0);
  Engine_EventWait(10);
  // Faran sends them off.
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_ActorStartRepeatedMotion(MIA, 2);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Engine_EventWait(30);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_FARAN, ANIM_NOD);
  Engine_EventWait(30);
  Engine_EventShowMessage(ACTOR_FARAN, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimation(MIA, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  Engine_EventWait(30);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_FARAN, 2);
  Engine_EventWait(20);
  Engine_EventShowMessage(ACTOR_FARAN, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(20);
  Engine_EventShowMessage(GARET, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_FARAN, ANIM_NOD);
  Engine_EventWait(30);
  Engine_EventShowMessage(ACTOR_FARAN, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
  Engine_EventWait(30);
  Engine_EventShowMessage(MIA, 0);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
  Engine_ActorFaceDirection(IVAN, FACING_NORTHWEST, 0);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(10);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimation(IVAN, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
  Engine_EventWait(30);
  // The others walk back into the leader.
  Actor_SetSpeed(GARET, 0x13333, 0x9999);
  Actor_SetSpeed(MIA, 0x13333, 0x9999);
  Actor_SetSpeed(IVAN, 0x13333, 0x9999);
  Engine_ActorSetAnimation(GARET, ANIM_WALK);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Engine_ActorSetDestination(GARET, leader->x.part.pixel,
                               leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(GARET);
  Actor_SetPosition(GARET, 0, 0);
  Engine_ActorSetAnimation(MIA, ANIM_WALK);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Engine_ActorSetDestination(MIA, leader->x.part.pixel, leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(MIA);
  Actor_SetPosition(MIA, 0, 0);
  Engine_ActorSetAnimation(IVAN, ANIM_WALK);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Engine_ActorSetDestination(IVAN, leader->x.part.pixel,
                               leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(IVAN);
  Engine_ActorSetPosition(IVAN, 0, 0);
  Engine_EventWait(10);
  Engine_EventEnd();
}

/// The party tells Faran and Iodem what became of Jenna and the others, when
/// it did not see the ship at Idejima: Felix said he sent Jenna ahead to the
/// ship.
static void LaliveroInteriors_TellFaranAboutJenna(void) {
  Engine_EventSetMessage(MsgLaliveroTellOthers);
  Engine_EventWait(20);

  Engine_ActorRunRepeatedMotion(ACTOR_FARAN, 2);
  Engine_EventWait(20);
  Engine_EventShowMessage(ACTOR_FARAN, 0);
  Engine_EventWait(10);

  Engine_ActorFaceActor(GARET, ACTOR_PARTY_LEADER, 50);
  Engine_ActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
                        60);
  Engine_EventWait(10);
  Engine_ActorFaceDirection(GARET, FACING_NORTH, 0);
  Engine_EventWait(20);
  Engine_EventShowMessage(GARET, 0);
  Engine_EventWait(10);

  Engine_ActorSetAnimationAndWait(MIA, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Engine_EventShowMessage(MIA, 0);
  Engine_EventWait(10);

  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Engine_EventWait(20);
  Engine_EventShowMessage(IVAN, 0);
  Engine_EventWait(10);

  Engine_ActorRunRepeatedMotion(ACTOR_IODEM, 2);
  Engine_EventWait(20);
  Engine_EventShowMessage(ACTOR_IODEM, 0);
  Engine_EventWait(20);

  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_EventWait(20);
  Engine_EventWait(25);

  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(30);
  Engine_EventShowMessage(GARET, 0);
  Engine_EventWait(30);

  Engine_ActorSetAnimationAndWait(GARET, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Engine_EventShowMessage(GARET, 0);
  Engine_EventWait(10);

  Engine_ActorSetAnimationAndWait(MIA, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Engine_EventShowMessage(MIA, 0);
  Engine_EventWait(10);

  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  Engine_EventWait(30);
  Engine_EventShowMessage(IVAN, 0);
  Engine_EventWait(10);

  Engine_ActorFaceActor(IVAN, ACTOR_PARTY_LEADER, 30);
  Engine_EventOpenMessage(IVAN | SPEAKER_PORTRAIT_RIGHT, 0);
}

/// Sets both scenes up as the party enters: shopkeepers and the innkeeper
/// behind their counters, the Sanctum as the place to wake after a defeat,
/// the inn worker back at her chores, and the report to Faran on coming back
/// from Venus Lighthouse.
s32 Scene_Initialize(void) {
  s32 scene;
  s16 entrance;
  struct FieldActor *actor;

  if (gGameState.entrance == ENTRANCE_FROM_AERIE) {
    Engine_GameFlagSet(FLAG_RETURNED_FROM_VENUS_LIGHTHOUSE);
  }
  gEventWork->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  scene = gGameState.scene;
  if (scene == (s32)&SceneId_LaliveroShops) {
    // Each shopkeeper is drawn under the counter's top, and counts as nearer
    // to talk to (ACTOR_NEAR_TALK) so that the leader reaches it across the
    // counter.
    actor = Object_GetById(ACTOR_ITEM_MERCHANT);
    actor->priorityFlags = 0;
    actor->collisionFlags |= ACTOR_NEAR_TALK;
    actor->sprite->image.priority = 2;
    actor = Object_GetById(ACTOR_WEAPON_MERCHANT);
    actor->priorityFlags = 0;
    actor->collisionFlags |= ACTOR_NEAR_TALK;
    actor->sprite->image.priority = 2;
    actor = Object_GetById(ACTOR_ARMOR_MERCHANT);
    actor->collisionFlags |= ACTOR_NEAR_TALK;
    actor->priorityFlags = 0;
    actor->sprite->image.priority = 2;
    Engine_ActorSetAnimation(ACTOR_NAPPING_WORKER, 6);
    entrance = gGameState.entrance;
    if (entrance == LALIVERO_SHOPS_SANCTUM) {
      gGameState.savedScene = scene;
      gGameState.savedEntrance = entrance;
    }
  }
  if (gGameState.scene == (s32)&SceneId_LaliveroFaransHouse) {
    actor = Object_GetById(ACTOR_INNKEEPER);
    actor->collisionFlags |= ACTOR_NEAR_TALK;
    actor->priorityFlags = 0;
    actor->sprite->image.priority = 2;
    if (Engine_GameFlagIsSet(FLAG_INN_WORKER_APOLOGIZED) != 0) {
      Actor_SetActionScript(ACTOR_INN_WORKER,
                            gLaliveroInteriors_InnWorkerChores);
    }
    if (gGameState.entrance ==
        LALIVERO_FARANS_HOUSE_FROM_VENUS_LIGHTHOUSE_AERIE) {
      Event_ReportToFaran();
      // Iodem and Faran turn toward the leader, as their later placements do.
      Engine_ActorSetActionCallback(Object_GetById(ACTOR_IODEM),
                                    ACTOR_FACE_LEADER);
      Engine_ActorSetActionCallback(Object_GetById(ACTOR_FARAN),
                                    ACTOR_FACE_LEADER);
      gGameState.entrance = LALIVERO_FARANS_HOUSE_FROM_LALIVERO_SOUTHWEST;
    }
  }
  return 0;
}

/// The inn worker's chores once she has apologized: she paces between two
/// spots, turning to each side, and sighs now and then.
s32 gLaliveroInteriors_InnWorkerChores[] = {
    SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_SOLID),
    SC_SET(SW_SPEED, 0x26666),
    SC_SET(SW_ACCEL, 0x13333),
    SC_LABEL(1),
    SC_ARG(2),
    SC_MOVE(PIXELS(128), 0, PIXELS(300)),
    SC_SYNC,
    SC_SET(SW_FACING, FACING_NORTHWEST + FACING_STEP),
    SC_ARG(3),
    SC_WAIT(30),
    SC_ARG(2),
    SC_MOVE(PIXELS(168), 0, PIXELS(300)),
    SC_SYNC,
    SC_SET(SW_FACING, FACING_NORTH + FACING_STEP),
    SC_ARG(3),
    SC_WAIT(50),
    SC_ARG(2),
    SC_MOVE(PIXELS(128), 0, PIXELS(300)),
    SC_SYNC,
    SC_SET(SW_FACING, FACING_NORTHWEST + FACING_STEP),
    SC_ARG(3),
    SC_WAIT(20),
    SC_CALL(InnWorker_Sigh),
    SC_WAIT(30),
    SC_ARG(2),
    SC_MOVE(PIXELS(168), 0, PIXELS(300)),
    SC_SYNC,
    SC_SET(SW_FACING, FACING_NORTH + FACING_STEP),
    SC_ARG(3),
    SC_WAIT(30),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
};

struct SceneEntrance gLaliveroInteriors_Entrances[] = {
    {LALIVERO_SHOPS_FROM_LALIVERO_WEST, CONDITION_ALWAYS, 144, 0, 168,
     FACING_NORTH, 0, 0, 8, 240, 192, 0},
    {LALIVERO_SHOPS_FROM_LALIVERO_SOUTHEAST, CONDITION_ALWAYS, 400, 0, 168,
     FACING_NORTH, 0, 256, 8, 496, 192, 0},
    {LALIVERO_SHOPS_ITEM_SHOP_UPSTAIRS_FROM_ITEM_SHOP, CONDITION_ALWAYS, 648, 0,
     72, FACING_NORTH, 0, 488, 8, 728, 184, 0},
    {LALIVERO_SHOPS_FROM_LALIVERO_NORTH, CONDITION_ALWAYS, 128, 0, 472,
     FACING_NORTH, 0, 0, 216, 240, 488, 0},
    {LALIVERO_SHOPS_ITEM_SHOP_FROM_ITEM_SHOP_UPSTAIRS, CONDITION_ALWAYS, 136, 0,
     296, FACING_SOUTH, 0, 0, 216, 240, 488, 0},
    {LALIVERO_SHOPS_FROM_LALIVERO_NORTHWEST, CONDITION_ALWAYS, 368, 0, 472,
     FACING_NORTH, 0, 240, 216, 480, 488, 0},
    {LALIVERO_SHOPS_NORTHWEST_HOUSE_FROM_NORTHWEST_HOUSE_UPSTAIRS,
     CONDITION_ALWAYS, 392, 0, 296, FACING_SOUTH, 0, 240, 216, 480, 488, 0},
    {LALIVERO_SHOPS_NORTHWEST_HOUSE_UPSTAIRS_FROM_NORTHWEST_HOUSE,
     CONDITION_ALWAYS, 648, 0, 280, FACING_NORTH, 0, 480, 216, 720, 488, 0},
    {LALIVERO_SHOPS_FROM_LALIVERO_SOUTH, CONDITION_ALWAYS, 96, 0, 680,
     FACING_NORTH, 0, 8, 520, 312, 696, 0},
    {LALIVERO_SHOPS_FROM_LALIVERO_EAST_SOUTHEAST, CONDITION_ALWAYS, 200, 0, 600,
     FACING_SOUTH, 0, 8, 520, 312, 696, 0},
    {LALIVERO_SHOPS_FROM_LALIVERO_NORTHEAST, CONDITION_ALWAYS, 432, 0, 728,
     FACING_NORTH, 0, 312, 520, 552, 744, 0},
    {LALIVERO_SHOPS_SANCTUM, CONDITION_ALWAYS, 432, 0, 630, FACING_NORTH, 0,
     312, 520, 552, 744, 0},
    {LALIVERO_FARANS_HOUSE_FROM_LALIVERO_SOUTHWEST, CONDITION_ALWAYS, 96, 0,
     168, FACING_NORTH, 0, 8, 8, 312, 192, 0},
    {LALIVERO_FARANS_HOUSE_FROM_LALIVERO_SOUTH, CONDITION_ALWAYS, 216, 0, 88,
     FACING_SOUTH, 0, 8, 8, 312, 192, 0},
    {LALIVERO_FARANS_HOUSE_FROM_LALIVERO_NORTHEAST, CONDITION_ALWAYS, 224, 0,
     392, FACING_NORTH, 0, 8, 232, 312, 416, 0},
    {LALIVERO_FARANS_HOUSE_INN_FROM_INN_UPSTAIRS, CONDITION_ALWAYS, 88, 0, 312,
     FACING_SOUTH, 0, 8, 232, 312, 416, 0},
    {LALIVERO_FARANS_HOUSE_INN_UPSTAIRS_FROM_INN, CONDITION_ALWAYS, 384, 0, 368,
     FACING_NORTH, 0, 320, 240, 632, 400, 0},
    {LALIVERO_FARANS_HOUSE_INN_UPSTAIRS_FROM_GUEST_ROOM_WEST, CONDITION_ALWAYS,
     456, 0, 376, FACING_NORTH, 0, 320, 240, 632, 400, 0},
    {LALIVERO_FARANS_HOUSE_INN_UPSTAIRS_FROM_GUEST_ROOM_EAST, CONDITION_ALWAYS,
     504, 0, 376, FACING_NORTH, 0, 320, 240, 632, 400, 0},
    {LALIVERO_FARANS_HOUSE_GUEST_ROOM_WEST_FROM_INN_UPSTAIRS, CONDITION_ALWAYS,
     472, 0, 72, FACING_SOUTH, 0, 344, 8, 584, 168, 0},
    {LALIVERO_FARANS_HOUSE_GUEST_ROOM_EAST_FROM_INN_UPSTAIRS, CONDITION_ALWAYS,
     744, 0, 72, FACING_SOUTH, 0, 632, 8, 872, 168, 0},
    {LALIVERO_FARANS_HOUSE_FROM_LALIVERO_NORTHWEST, CONDITION_ALWAYS, 832, 0,
     872, FACING_NORTH, 0, 280, 504, 912, 896, 0},
    {LALIVERO_FARANS_HOUSE_FROM_LEMURIAN_SHIP, CONDITION_ALWAYS, 464, 0, 832,
     FACING_SOUTH, 0, 280, 504, 912, 896, 0},
    {LALIVERO_FARANS_HOUSE_FROM_VENUS_LIGHTHOUSE_AERIE, CONDITION_ALWAYS, 96, 0,
     168, FACING_NORTH, 0, 8, 8, 312, 192, 0},
    {SCENE_TABLE_END},
};

// clang-format off
struct SceneRegion gLaliveroInteriors_ShopsRegions[] = {
    {196, 28, 595, 204, 36, 603, CONDITION_ALWAYS, LALIVERO_SHOPS_TO_LALIVERO_EAST_SOUTHEAST},
    {SCENE_TABLE_END},
};

struct SceneRegion gLaliveroInteriors_FaransHouseRegions[] = {
    {212, 28, 83, 220, 36, 91, CONDITION_ALWAYS, LALIVERO_FARANS_HOUSE_TO_LALIVERO_SOUTH},
    {SCENE_TABLE_END},
};

u32 gLaliveroInteriors_Exits[] = {
    SCENE_EXITS(LaliveroShops),
    SCENE_EXIT(LALIVERO_SHOPS_TO_LALIVERO_WEST, Lalivero, LALIVERO_FROM_LALIVERO_SHOPS_WEST),
    SCENE_EXIT(LALIVERO_SHOPS_TO_LALIVERO_SOUTHEAST, Lalivero, LALIVERO_FROM_LALIVERO_SHOPS_SOUTHEAST),
    SCENE_EXIT(LALIVERO_SHOPS_ITEM_SHOP_UPSTAIRS_TO_ITEM_SHOP, LaliveroShops, LALIVERO_SHOPS_ITEM_SHOP_FROM_ITEM_SHOP_UPSTAIRS),
    SCENE_EXIT(LALIVERO_SHOPS_TO_LALIVERO_NORTH, Lalivero, LALIVERO_FROM_LALIVERO_SHOPS_NORTH),
    SCENE_EXIT(LALIVERO_SHOPS_ITEM_SHOP_TO_ITEM_SHOP_UPSTAIRS, LaliveroShops, LALIVERO_SHOPS_ITEM_SHOP_UPSTAIRS_FROM_ITEM_SHOP),
    SCENE_EXIT(LALIVERO_SHOPS_TO_LALIVERO_NORTHWEST, Lalivero, LALIVERO_FROM_LALIVERO_SHOPS_NORTHWEST),
    SCENE_EXIT(LALIVERO_SHOPS_NORTHWEST_HOUSE_TO_NORTHWEST_HOUSE_UPSTAIRS, LaliveroShops, LALIVERO_SHOPS_NORTHWEST_HOUSE_UPSTAIRS_FROM_NORTHWEST_HOUSE),
    SCENE_EXIT(LALIVERO_SHOPS_NORTHWEST_HOUSE_UPSTAIRS_TO_NORTHWEST_HOUSE, LaliveroShops, LALIVERO_SHOPS_NORTHWEST_HOUSE_FROM_NORTHWEST_HOUSE_UPSTAIRS),
    SCENE_EXIT(LALIVERO_SHOPS_TO_LALIVERO_SOUTH, Lalivero, LALIVERO_FROM_LALIVERO_SHOPS_SOUTH),
    SCENE_EXIT(LALIVERO_SHOPS_TO_LALIVERO_EAST_SOUTHEAST, Lalivero, LALIVERO_FROM_LALIVERO_SHOPS_EAST_SOUTHEAST),
    SCENE_EXIT(LALIVERO_SHOPS_TO_LALIVERO_NORTHEAST, Lalivero, LALIVERO_FROM_LALIVERO_SHOPS_NORTHEAST),
    SCENE_EXITS(LaliveroFaransHouse),
    SCENE_EXIT(LALIVERO_FARANS_HOUSE_TO_LALIVERO_SOUTHWEST, Lalivero, LALIVERO_FROM_LALIVERO_FARANS_HOUSE_SOUTHWEST),
    SCENE_EXIT(LALIVERO_FARANS_HOUSE_TO_LALIVERO_SOUTH, Lalivero, LALIVERO_FROM_LALIVERO_FARANS_HOUSE_SOUTH),
    SCENE_EXIT(LALIVERO_FARANS_HOUSE_TO_LALIVERO_NORTHEAST, Lalivero, LALIVERO_FROM_LALIVERO_FARANS_HOUSE_NORTHEAST),
    SCENE_EXIT(LALIVERO_FARANS_HOUSE_INN_TO_INN_UPSTAIRS, LaliveroFaransHouse, LALIVERO_FARANS_HOUSE_INN_UPSTAIRS_FROM_INN),
    SCENE_EXIT(LALIVERO_FARANS_HOUSE_INN_UPSTAIRS_TO_INN, LaliveroFaransHouse, LALIVERO_FARANS_HOUSE_INN_FROM_INN_UPSTAIRS),
    SCENE_EXIT(LALIVERO_FARANS_HOUSE_INN_UPSTAIRS_TO_GUEST_ROOM_WEST, LaliveroFaransHouse, LALIVERO_FARANS_HOUSE_GUEST_ROOM_WEST_FROM_INN_UPSTAIRS),
    SCENE_EXIT(LALIVERO_FARANS_HOUSE_INN_UPSTAIRS_TO_GUEST_ROOM_EAST, LaliveroFaransHouse, LALIVERO_FARANS_HOUSE_GUEST_ROOM_EAST_FROM_INN_UPSTAIRS),
    SCENE_EXIT(LALIVERO_FARANS_HOUSE_GUEST_ROOM_WEST_TO_INN_UPSTAIRS, LaliveroFaransHouse, LALIVERO_FARANS_HOUSE_INN_UPSTAIRS_FROM_GUEST_ROOM_WEST),
    SCENE_EXIT(LALIVERO_FARANS_HOUSE_GUEST_ROOM_EAST_TO_INN_UPSTAIRS, LaliveroFaransHouse, LALIVERO_FARANS_HOUSE_INN_UPSTAIRS_FROM_GUEST_ROOM_EAST),
    SCENE_EXIT(LALIVERO_FARANS_HOUSE_TO_LALIVERO_NORTHWEST, Lalivero, LALIVERO_FROM_LALIVERO_FARANS_HOUSE_NORTHWEST),
    SCENE_EXIT(LALIVERO_FARANS_HOUSE_TO_LEMURIAN_SHIP, LemurianShip, LEMURIAN_SHIP_FROM_LALIVERO_FARANS_HOUSE),
    SCENE_EXITS_END,
};

struct ScenePlacement gLaliveroInteriors_ShopsPlacements[] = {
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(176), 0, PIXELS(104), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(72), 0, PIXELS(80), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { SPRITE_MAN_IN_WHITE_TURBAN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(328), 0, PIXELS(136), FACING_NORTHEAST, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(344), 0, PIXELS(120), FACING_SOUTHWEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, ACTOR_WANDER_NEAR,
        PIXELS(616), 0, PIXELS(104), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_OLD_WOMAN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(664), 0, PIXELS(168), FACING_WEST, TALK_FACE_PARTY, 0 },
    { SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(400), 0, PIXELS(416), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(550), 0, PIXELS(368), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(344), 0, PIXELS(304), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(600), 0, PIXELS(408), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_BALD_MAN_IN_RED, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(132), 0, PIXELS(616), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_YOUNG_MAN_IN_GREEN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(92), 0, PIXELS(616), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_WITH_PONYTAIL, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(68), 0, PIXELS(340), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(432), 0, PIXELS(600), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gLaliveroInteriors_FaransHousePlacements[] = {
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(168), 0, PIXELS(112), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(240), 0, PIXELS(96), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_YOUNG_WOMAN_IN_GREEN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(136), 0, PIXELS(88), FACING_SOUTHEAST, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_FARAN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_SOUTHEAST, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_IODEM, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_SOUTHEAST, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_MAN_WITH_DARK_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(248), 0, PIXELS(336), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_IN_PINK_APRON, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(136), 0, PIXELS(300), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(544), 0, PIXELS(368), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gLaliveroInteriors_FaransHousePlacementsAfterLighthouse[] = {
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(168), 0, PIXELS(112), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(240), 0, PIXELS(96), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_YOUNG_WOMAN_IN_GREEN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(216), 0, PIXELS(168), FACING_WEST, TALK_FACE_PARTY, 0 },
    { SPRITE_FARAN, CONDITION_ALWAYS, ACTOR_FACE_LEADER,
        PIXELS(104), 0, PIXELS(96), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_IODEM, CONDITION_ALWAYS, ACTOR_FACE_LEADER,
        PIXELS(128), 0, PIXELS(96), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_MAN_WITH_DARK_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(248), 0, PIXELS(336), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_IN_PINK_APRON, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(160), 0, PIXELS(304), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(544), 0, PIXELS(368), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { IVAN, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { MIA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gLaliveroInteriors_ShopsEvents[] = {
    { EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveBuilding) },
    { EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveBuilding) },
    { EVENT_TOUCH, 3, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveBuilding) },
    { EVENT_TOUCH, 4, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveBuilding) },
    { EVENT_TOUCH, 5, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveBuilding) },
    { EVENT_TOUCH, 6, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveBuilding) },
    { EVENT_TOUCH, 7, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveBuilding) },
    { EVENT_TOUCH, 8, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveBuilding) },
    { EVENT_TOUCH, 9, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveBuilding) },
    { EVENT_TOUCH, 11, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveBuilding) },
    { EVENT_TALK, ACTOR_VILLAGER_SAW_SHEBAS_POWER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsOfCourseIHaveSeenActor) },
    { EVENT_TALK, ACTOR_WOMAN_DEFENDING_SHEBA, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsThereAreSomeWhoDoNot) },
    { EVENT_TALK, ACTOR_VILLAGER_DOUBTING_FARAN, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsCouldFaranHaveTrickedUsAll) },
    { EVENT_TALK, ACTOR_WOMAN_DOUBTING_SHEBA, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIfActorCantUseHerPowers) },
    { EVENT_TALK, ACTOR_OLD_MAN_SHOOTING_STAR, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsActorCameFlyingToTheRuins) },
    { EVENT_TALK, ACTOR_OLD_WOMAN_SIX_OF_THEM, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsEvenActorCouldntHaveEscapedSix) },
    { EVENT_TALK, ACTOR_WORKER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsTheySayAManNamedIodem) },
    { EVENT_TALK, ACTOR_NAPPING_WORKER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsNoOnesCalledMeSoMaybe) },
    { EVENT_TALK, ACTOR_STRONGMAN_RUINS_CAVE, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsActorSaidTheCaveAtThe) },
    { EVENT_TALK, ACTOR_STRONGMAN_RUINS_RIDDLE, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsTheRuinsThatFormTheLighthouse) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_SAW_SHEBAS_POWER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsActorIfThePowerThatYou) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_DEFENDING_SHEBA, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsActorIsTheSaviorOfLalivero) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_DOUBTING_FARAN, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsALegendarySaviorWillAppearIn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_DOUBTING_SHEBA, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIdLikeToBelieveInActor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_SHOOTING_STAR, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsEvenThoughSheFellOutOf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_SIX_OF_THEM, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIfSomethingHappensToActorWhat) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WORKER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWhenIodemGetsHereHesGoing) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_NAPPING_WORKER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsItsBeenSoLongSinceI) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRONGMAN_RUINS_CAVE, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIWonderHowActorKnewAbout) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRONGMAN_RUINS_RIDDLE, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsThoseCavesAreTooDangerousFor) },
    { EVENT_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToWeaponMerchant) },
    { EVENT_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToArmorMerchant) },
    { EVENT_TALK, ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToItemMerchant) },
    { EVENT_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToHealer) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsTheLaliveransAreATimidPeople) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsICantRescueActorIfI) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIfAnythingHappensToActorIll) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIBetBabisTowerWillBe) },
    { SEARCH(SEARCH_BARREL), 100, FLAG_LALIVERO_SHOPS_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { SEARCH(SEARCH_BARREL), 101, FLAG_LALIVERO_SHOPS_ANTIDOTE, FIND_ITEM(ITEM_ANTIDOTE) },
    { SEARCH(SEARCH_OVEN), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsShesMakingMushroomSoupInLalivero)) },
    { SEARCH(SEARCH_OVEN), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsItsALaliveroSaladWithPotato)) },
    { SEARCH(SEARCH_OVEN), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsThePotIsFullOfA)) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gLaliveroInteriors_ShopsEventsAfterLighthouse[] = {
    { EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveBuilding) },
    { EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveBuilding) },
    { EVENT_TOUCH, 3, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveBuilding) },
    { EVENT_TOUCH, 4, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveBuilding) },
    { EVENT_TOUCH, 5, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveBuilding) },
    { EVENT_TOUCH, 6, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveBuilding) },
    { EVENT_TOUCH, 7, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveBuilding) },
    { EVENT_TOUCH, 8, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveBuilding) },
    { EVENT_TOUCH, 9, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveBuilding) },
    { EVENT_TOUCH, 11, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveBuilding) },
    { EVENT_TALK, ACTOR_VILLAGER_SAW_SHEBAS_POWER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsTheContinentBrokeApartAroundThe) },
    { EVENT_TALK, ACTOR_WOMAN_DEFENDING_SHEBA, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWhatHappenedToThoseLivingOn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_SAW_SHEBAS_POWER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIShudderToThinkWhatWould) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_DEFENDING_SHEBA, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWereLuckyThatWeLiveNorth) },
    { EVENT_TALK, ACTOR_VILLAGER_DOUBTING_FARAN, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsEveryoneSaysThatActorIsOkay) },
    { EVENT_TALK, ACTOR_WOMAN_DOUBTING_SHEBA, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIRealizedHowThoughtlessIWas) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_DOUBTING_FARAN, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIfSheIsIndeedTheChild) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_DOUBTING_SHEBA, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWeWantToBelieveInSomething) },
    { EVENT_TALK, ACTOR_OLD_MAN_SHOOTING_STAR, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsActorShouldBeFineSheWas) },
    { EVENT_TALK, ACTOR_OLD_WOMAN_SIX_OF_THEM, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsActorWillWakeWhenTheBeacon) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_SHOOTING_STAR, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsSeeingActorDiveMustHaveBeen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_SIX_OF_THEM, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsICantWaitUntilActorIs) },
    { EVENT_TALK, ACTOR_WORKER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsBabiLighthouseCrumbledWhenAllThe) },
    { EVENT_TALK, ACTOR_NAPPING_WORKER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIAlwaysWantedABreakBut) },
    { EVENT_TALK, ACTOR_STRONGMAN_RUINS_CAVE, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsSoTheRuinsReallyAreConnected) },
    { EVENT_TALK, ACTOR_STRONGMAN_RUINS_RIDDLE, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsYouFinallySolvedTheMysteryOf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WORKER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWillTheyContinueOnOrQuit) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_NAPPING_WORKER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsImWorriedThatWeWontGet) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRONGMAN_RUINS_CAVE, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWeCantPossiblyDigAHole) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRONGMAN_RUINS_RIDDLE, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsTheMysteryOfTheRuinsShould) },
    { EVENT_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToWeaponMerchant) },
    { EVENT_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToArmorMerchant) },
    { EVENT_TALK, ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToItemMerchant) },
    { EVENT_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToHealer) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsNoOneCouldPossiblySwimIn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWhenWillTheOceanBeCalm) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsTheChampaArePeopleFromA) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsFirstTheLighthouseWasStruckDown) },
    { SEARCH(SEARCH_BARREL), 100, FLAG_LALIVERO_SHOPS_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { SEARCH(SEARCH_BARREL), 101, FLAG_LALIVERO_SHOPS_ANTIDOTE, FIND_ITEM(ITEM_ANTIDOTE) },
    { SEARCH(SEARCH_OVEN), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsShesMakingMushroomSoupInLalivero)) },
    { SEARCH(SEARCH_OVEN), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsItsALaliveroSaladWithPotato)) },
    { SEARCH(SEARCH_OVEN), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsThePotIsFullOfA)) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gLaliveroInteriors_FaransHouseEvents[] = {
    { EVENT_TOUCH, 21, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveBuilding) },
    { EVENT_TOUCH, 23, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveBuilding) },
    { EVENT_TOUCH, 24, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveBuilding) },
    { EVENT_TOUCH, 25, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveBuilding) },
    { EVENT_TOUCH, 26, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveBuilding) },
    { EVENT_TOUCH, 27, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveBuilding) },
    { EVENT_TOUCH, 28, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveBuilding) },
    { EVENT_TOUCH, 29, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveBuilding) },
    { EVENT_TOUCH, 30, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveBuilding) },
    { EVENT_TOUCH, 31, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveBuilding) },
    { EVENT_TALK, ACTOR_FARANS_WIFE, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWeReceivedActorFromTheSky) },
    { EVENT_TALK, ACTOR_FARANS_SON, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIJustKnowThatFatherWill) },
    { EVENT_TALK, ACTOR_WORRIED_YOUNG_WOMAN, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsHisWifeSeemsVeryCalmConsidering) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FARANS_WIFE, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsItsSoPainfulToEvenThink) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FARANS_SON, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIJustWishSomeoneWouldPlease) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WORRIED_YOUNG_WOMAN, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsThatCoupleRaisedActorAsThough) },
    { EVENT_TALK, ACTOR_INN_WORKER, FLAG_INN_WORKER_APOLOGIZED, EVENT_SCRIPT(Event_TalkToInnWorker) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x400 | ACTOR_INN_WORKER, FLAG_INN_WORKER_APOLOGIZED, EVENT_SCRIPT(Event_TalkToInnWorker) },
    { EVENT_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToInnkeeper) },
    { EVENT_TALK, ACTOR_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWorkingLikeThisKeepsMyMind) },
    { EVENT_TALK, ACTOR_COOK, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWeMustObeyTolbiThatsWhat) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWhyDidThisHaveToHappen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsHousecleaningChangingSheetsItsAllDone) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_COOK, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsOnceThisIncidentIsOverI) },
    { SEARCH(SEARCH_BARREL), 102, FLAG_LALIVERO_FARANS_HOUSE_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { SEARCH(SEARCH_OVEN), 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsShesFryingFreshwaterFishFromThe)) },
    { SEARCH(SEARCH_OVEN), 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsItsRiceMadeWithGrainsFrom)) },
    { SEARCH(SEARCH_OVEN), 205, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsShesMakingRoastChickenThisScent)) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gLaliveroInteriors_FaransHouseEventsAfterLighthouse[] = {
    { EVENT_TOUCH, 21, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveBuilding) },
    { EVENT_TOUCH, 23, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveBuilding) },
    { EVENT_TOUCH, 24, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveBuilding) },
    { EVENT_TOUCH, 25, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveBuilding) },
    { EVENT_TOUCH, 26, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveBuilding) },
    { EVENT_TOUCH, 27, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveBuilding) },
    { EVENT_TOUCH, 28, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveBuilding) },
    { EVENT_TOUCH, 29, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveBuilding) },
    { EVENT_TOUCH, 30, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveBuilding) },
    { EVENT_TOUCH, 31, CONDITION_ALWAYS, EVENT_SCRIPT(Event_LeaveBuilding) },
    { EVENT_TOUCH, 51, FLAG_IODEM_ASKED_TO_MEET, EVENT_SCRIPT(Event_IodemCallsOut) },
    { EVENT_TALK, ACTOR_IODEM, FLAG_IODEM_FAREWELL_SEEN, EVENT_SCRIPT(Event_TalkToIodem) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_IODEM, FLAG_IODEM_FAREWELL_SEEN, MSGNO(MsgLaliveroInteriorsIMustExplainTheirPromiseTo) },
    { EVENT_TALK, ACTOR_FARANS_WIFE, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsFaranSaysKnowingHerPowersActor) },
    { EVENT_TALK, ACTOR_FARANS_SON, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsYouAreLeavingAfterAllArent) },
    { EVENT_TALK, ACTOR_WORRIED_YOUNG_WOMAN, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsTheySayTheLighthouseCrumbledDown) },
    { EVENT_TALK, ACTOR_FARAN, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsYouWillFindJennaSomedayFarewell) },
    { EVENT_TALK, ACTOR_IODEM, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIsaacThatBoatBelongsToYou) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FARANS_WIFE, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIDontNeedFameOrFortune) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FARANS_SON, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWhyWontAnyoneStopIsaacAnd) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WORRIED_YOUNG_WOMAN, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsTolbiIsToBeBlamedFor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FARAN, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsSoThereAreSomePeopleLike) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_IODEM, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsPleaseUseThatShipToFind) },
    { EVENT_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToInnkeeper) },
    { EVENT_TALK, ACTOR_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIMustSayItIsNice) },
    { EVENT_TALK, ACTOR_COOK, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsFishingIsSoDifficultNowadaysBecause) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWhyWouldTheLightingMakeSuch) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsImSleepyTheseBrightNightsHave) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_COOK, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsGeeWhatAmIGoingTo) },
    { SEARCH(SEARCH_BARREL), 102, FLAG_LALIVERO_FARANS_HOUSE_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { SEARCH(SEARCH_OVEN), 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsShesFryingFreshwaterFishFromThe)) },
    { SEARCH(SEARCH_OVEN), 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsItsRiceMadeWithGrainsFrom)) },
    { SEARCH(SEARCH_OVEN), 205, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsShesMakingRoastChickenThisScent)) },
    { SCENE_EVENTS_END },
};
// clang-format on
