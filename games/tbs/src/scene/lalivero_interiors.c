/// @file
/// Lalivero's interiors: two sheets of rooms drawn in timber in one overlay,
/// Timber rooms B and C. The game packs rooms that share a tile style onto one
/// map, and every room on these two is Lalivero's, entered from the town. Their
/// area name is Lalivero, and Lalivero Sanctum for the Sanctum's rooms of
/// TimberRoomsB (TIMBER_ROOMS_B_FROM_LALIVERO_NORTHEAST and
/// TIMBER_ROOMS_B_SANCTUM). Each map has tables for before and after Venus
/// Lighthouse, chosen by FLAG_VENUS_LIGHTHOUSE_PARTY_RETURNED, as the town's
/// are (lalivero.c).
///
/// Timber rooms B (SceneId_TimberRoomsB) holds, by the exits that lead in, a
/// house (TIMBER_ROOMS_B_FROM_LALIVERO_WEST), a house
/// (TIMBER_ROOMS_B_FROM_LALIVERO_SOUTHEAST), the item shop with a room upstairs
/// (TIMBER_ROOMS_B_FROM_LALIVERO_NORTH), a house with an upper floor
/// (TIMBER_ROOMS_B_FROM_LALIVERO_NORTHWEST), the weapon and armor shop, entered
/// by either of its two doors (TIMBER_ROOMS_B_FROM_LALIVERO_SOUTH and
/// TIMBER_ROOMS_B_FROM_LALIVERO_EAST_SOUTHEAST), and the Sanctum
/// (TIMBER_ROOMS_B_FROM_LALIVERO_NORTHEAST). Each shopkeeper stands behind a
/// counter and trades only with a leader facing north across it
/// (LaliveroInteriors_TalkToWeaponMerchant and the others); from any other side
/// they talk. Stepping into the Sanctum by TIMBER_ROOMS_B_SANCTUM makes it the
/// place a defeated party wakes. Barrels hold a Lucky Medal and an Antidote.
///
/// Timber rooms C (SceneId_TimberRoomsC) holds, by the exits that lead in,
/// Faran's house (TIMBER_ROOMS_C_FROM_LALIVERO_SOUTHWEST and
/// TIMBER_ROOMS_C_FROM_LALIVERO_SOUTH), the inn with its upper floor and two
/// guest rooms (TIMBER_ROOMS_C_FROM_LALIVERO_NORTHEAST), and the way out to the
/// wharf where Babi's ship lies (TIMBER_ROOMS_C_FROM_LALIVERO_NORTHWEST,
/// leading on by TIMBER_ROOMS_C_TO_LEMURIAN_SHIP). Before Venus Lighthouse an
/// inn worker apologizes once for her distraction
/// (LaliveroInteriors_TalkToInnWorker, FLAG_LALIVERO_INN_WORKER_APOLOGIZED) and
/// goes back to her chores. The aerie of Venus Lighthouse leads in by
/// TIMBER_ROOMS_C_FROM_VENUS_LIGHTHOUSE_AERIE, which plays
/// LaliveroInteriors_ReportToFaran: the party reports to Faran and Iodem, in
/// one of two versions by FLAG_SAW_SHIP_AT_IDEJIMA. Step trigger 51 afterwards
/// plays LaliveroInteriors_IodemCallsOut (FLAG_IODEM_ASKED_TO_MEET), which
/// sends the party outside, where Iodem gives it the Black Orb (lalivero.c). A
/// barrel holds a Lucky Medal.

#include "action_script.h"
#include "save_block.h"
#include "scene_ids.h"
#include "unit/import.h"
#include "unit/scene/lalivero_interiors.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/event_commands.h"
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
#include "event_layer.h"
#include "scene_tables.h"
#include "event_inline.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"
#include "songs.h"
#include "flags.h"
static void LaliveroInteriors_TellFaranAboutJenna(void);

/// An entrance that sets FLAG_VENUS_LIGHTHOUSE_PARTY_RETURNED. No exit
/// leads to it and the entrance table has no row for it: only the debug
/// map selector, which exits to any entrance from 0 to 99, reaches it.
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

/// The actors of both maps, numbered by the rows of each map's
/// actor template tables. The people are named by what they say before Venus
/// Lighthouse and by what their sprites show.
enum LaliveroInteriorsActorId {
  // The houses, the shops and the Sanctum (TimberRoomsB).
  /// A young man with brown hair in a purple shirt, in the house at
  /// TIMBER_ROOMS_B_FROM_LALIVERO_WEST. "Of course I have seen Sheba's
  /// power...
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
  /// TIMBER_ROOMS_B_FROM_LALIVERO_SOUTHEAST. "Could Faran have tricked us
  /// all?"
  /// ★ Named from the game: "Could Faran have tricked us all?"
  /// (MsgLaliveroInteriorsCouldFaranHaveTrickedUsAll)
  ACTOR_VILLAGER_DOUBTING_FARAN,
  /// A woman in a green dress, in the same house. "If Sheba can't use her
  /// powers in a time like this, they're useless!"
  /// ★ Named from the game: "use her powers in a time like this, they're
  /// useless!" (MsgLaliveroInteriorsIfActorCantUseHerPowers)
  ACTOR_WOMAN_DOUBTING_SHEBA,
  /// An old man with a cane, upstairs in the item shop at
  /// TIMBER_ROOMS_B_FROM_LALIVERO_NORTH. "Sheba came flying to the ruins
  /// like a
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
  /// TIMBER_ROOMS_B_FROM_LALIVERO_NORTHWEST. "They say a man named Iodem
  /// is
  /// coming from Tolbi. I guess he plans to make us do even more work!"
  /// ★ Named from the game: "I guess he plans to make us do even more work!"
  /// (MsgLaliveroInteriorsTheySayAManNamedIodem)
  ACTOR_WORKER,
  /// A bald, muscular man lying asleep upstairs in that house. "No
  /// one's called me, so maybe I'll take a nap..."; after Venus Lighthouse,
  /// Mind Read hears "I'm worried that we won't get paid now that the
  /// construction has ended."
  /// ★ Named from the game: "No one's called me, so maybe I'll take a nap..."
  /// (MsgLaliveroInteriorsNoOnesCalledMeSoMaybe)
  ACTOR_NAPPING_WORKER,
  /// A strongman, in the house at TIMBER_ROOMS_B_FROM_LALIVERO_NORTHWEST.
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

  // Faran's house and the inn (TimberRoomsC).
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

/// The napping worker's pose, from his sprite's animation table
/// (graphics/sprite/spr_114.tsv): one frame held, lying asleep on his side.
enum { ANIM_WORKER_LIE_ASLEEP = 6 };

/// The area temp flag FLAG_AREA_TEMP_00, which lasts until the party
/// enters another area: the inn worker has apologized. ★ Named from the
/// game: "Oh, I'm sorry..." (MsgLaliveroImSorry)
enum { FLAG_LALIVERO_INN_WORKER_APOLOGIZED = FLAG_AREA_TEMP_00 };

/// TimberRoomsC: the inn worker sighs between chores.
static s32 InnWorker_Sigh(struct Actor *interpreter) {
  Actor_SetAttachedEffect(ACTOR_INN_WORKER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  return 0;
}

const struct SceneEntrance *LaliveroInteriors_GetEntrances(void) {
  return gLaliveroInteriors_Entrances;
}

const struct SceneExitRegion *LaliveroInteriors_GetRegions(void) {
  if (gSaveBlock.sceneId == (s32)&SceneId_TimberRoomsB) {
    return gTimberRoomsB_Regions;
  }
  return gTimberRoomsC_Regions;
}

const u32 *LaliveroInteriors_GetExits(void) { return gLaliveroInteriors_Exits; }

/// The people of each map; Faran's house fills up after Venus Lighthouse.
const struct ActorTemplate *LaliveroInteriors_GetActorTemplates(void) {
  if (gSaveBlock.sceneId == (s32)&SceneId_TimberRoomsC) {
    // Faran's house and the inn.
    if (GameFlag_Get(FLAG_VENUS_LIGHTHOUSE_PARTY_RETURNED) != 0) {
      return gTimberRoomsC_ActorTemplatesAfterLighthouse;
    }
    return gTimberRoomsC_ActorTemplates;
  }
  return gTimberRoomsB_ActorTemplates;
}

/// The side the leader faces: the facing rounded to the nearest of east,
/// south, west and north. Facing north, the leader is across the counter
/// from a shopkeeper.
static inline u16 LaliveroInteriors_LeaderGetSide(void) {
  return (Actor_GetById(ACTOR_PARTY_LEADER)->facing + 0x2000) & ~0x3fff;
}

/// The weapon merchant sells weapons, or talks of Sheba and the sea. In
/// Lalivero's shops (TimberRoomsB), across the counter, it's the weapon
/// shop; from elsewhere, before Venus Lighthouse, they ask whether the party
/// has heard that Sheba was taken hostage and Faran went after her, and after
/// it tell of the rough sea.
static void LaliveroInteriors_TalkToWeaponMerchant(s32 keeper) {
  struct Actor *leader = Actor_GetById(ACTOR_PARTY_LEADER);
  // LaliveroInteriors_LeaderGetSide's rounding, written with the mask 0xc000,
  // which loads from the literal pool where the helper's builds the mask.
  u16 side = (leader->facing + 0x2000) & 0xc000;

  if (side == FACING_NORTH) {
    Shop_Run(LALIVERO_WEAPON_STOCK, keeper);
  } else if (GameFlag_Get(FLAG_VENUS_LIGHTHOUSE_PARTY_RETURNED)) {
    Event_SetMessageId(MsgLaliveroWentOceanLook);
    Event_ShowMessageAndWait(keeper, 0);
  } else {
    s32 message = MsgLaliveroTakenHostageFaran;

    Event_SetMessageId(message);
    FieldMessage_ShowAtActor(keeper, 0);
    if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
      Event_WaitFrames(10);
      Event_SetMessageId(message + 1);
    } else {
      Event_SetMessageId(message + 2);
    }
    Event_ShowMessageAndWait(keeper, 0);
  }
}

/// Lalivero (TimberRoomsB): the armor shop across the counter; otherwise
/// small talk.
static void LaliveroInteriors_TalkToArmorMerchant(s32 keeper) {
  if (LaliveroInteriors_LeaderGetSide() == FACING_NORTH) {
    Shop_Run(LALIVERO_ARMOR_STOCK, keeper);
  } else if (GameFlag_Get(FLAG_VENUS_LIGHTHOUSE_PARTY_RETURNED)) {
    Event_SetMessageId(MsgLaliveroWantedSendShips);
    Event_ShowMessageAndWait(keeper, 0);
  } else {
    Event_SetMessageId(MsgLaliveroWarriorRightPlease);
    Event_ShowMessageAndWait(keeper, 0);
  }
}

/// Lalivero (TimberRoomsB): the item shop across the counter; otherwise
/// small talk.
static void LaliveroInteriors_TalkToItemMerchant(s32 keeper) {
  if (LaliveroInteriors_LeaderGetSide() == FACING_NORTH) {
    Shop_Run(LALIVERO_ITEM_STOCK, keeper);
  } else if (GameFlag_Get(FLAG_VENUS_LIGHTHOUSE_PARTY_RETURNED)) {
    Event_SetMessageId(MsgLaliveroImpossibleGoOcean);
    Event_ShowMessageAndWait(keeper, 0);
  } else {
    Event_SetMessageId(MsgLaliveroGuessPeopleAngara);
    Event_ShowMessageAndWait(keeper, 0);
  }
}

/// Lalivero Sanctum: the healer offers the Sanctum's services across the
/// counter; otherwise small talk of Babi's lighthouse.
static void LaliveroInteriors_TalkToHealer(s32 healer) {
  if (LaliveroInteriors_LeaderGetSide() == FACING_NORTH) {
    Sanctum_Run(healer);
  } else if (GameFlag_Get(FLAG_VENUS_LIGHTHOUSE_PARTY_RETURNED)) {
    Event_SetMessageId(MsgLaliveroBabiLighthouseStruck);
    Event_ShowMessageAndWait(healer, 0);
  } else {
    Event_SetMessageId(MsgLaliveroBabiIsntBuilding);
    Event_ShowMessageAndWait(healer, 0);
  }
}

/// Lalivero (TimberRoomsC), the inn: a room for the night across the
/// counter; otherwise small talk.
static void LaliveroInteriors_TalkToInnkeeper(s32 innkeeper) {
  if (LaliveroInteriors_LeaderGetSide() == FACING_NORTH) {
    Inn_CheckIn(LALIVERO_INN, innkeeper);
  } else if (GameFlag_Get(FLAG_VENUS_LIGHTHOUSE_PARTY_RETURNED)) {
    Event_SetMessageId(MsgLaliveroDidntKnowTime);
    Event_ShowMessageAndWait(innkeeper, 0);
  } else {
    Event_SetMessageId(MsgLaliveroWantStayLet);
    Event_ShowMessageAndWait(innkeeper, 0);
  }
}

/// Lalivero (TimberRoomsC), before Venus Lighthouse: the inn worker
/// stops her chores to apologize: worry for Sheba keeps her from concentrating.
/// A leader standing west of her steps aside to the south before she starts
/// again.
static void LaliveroInteriors_TalkToInnWorker(void) {
  s16 side;

  side = LaliveroInteriors_LeaderGetSide();
  GameFlag_Set(FLAG_LALIVERO_INN_WORKER_APOLOGIZED);
  Event_Start();
  Field_EndBusyAndReveal();
  Event_SetMessageId(MsgLaliveroImSorry);
  Event_WaitFrames(50);
  Actor_ShowEmote(ACTOR_INN_WORKER, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
  Actor_FaceActor(ACTOR_INN_WORKER, ACTOR_PARTY_LEADER, 20);
  Event_ShowMessage(ACTOR_INN_WORKER, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_INN_WORKER, ANIM_SHAKE_HEAD);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_INN_WORKER, 0);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_INN_WORKER, 2);
  Event_WaitFrames(30);
  Event_ShowMessage(ACTOR_INN_WORKER, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_INN_WORKER, ANIM_NOD);
  Event_WaitFrames(20);
  Event_ShowMessage(ACTOR_INN_WORKER, 0);
  if ((u16)side == FACING_WEST) {
    Actor_CommitPositionAndActivate(ACTOR_PARTY_LEADER, 0, 16);
    Actor_ArmCallback(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Event_WaitFrames(20);
  }
  Actor_SetScript(ACTOR_INN_WORKER, gLaliveroInteriors_InnWorkerChores);
  Event_End();
}

/// Lalivero (TimberRoomsC), step trigger 51 in Faran's house after the
/// report: the camera finds Iodem, who asks the party to wait for him outside.
static void LaliveroInteriors_IodemCallsOut(void) {

  GameFlag_Set(FLAG_IODEM_ASKED_TO_MEET);
  Event_Start();
  Field_EndBusyAndReveal();
  Event_WaitFrames(10);
  FieldCamera_MoveWithinMapBounds(PIXELS(120), -1, PIXELS(96), 1);
  FieldCamera_WaitForArrival();
  Event_WaitFrames(30);
  Event_SetMessageId(MsgLaliveroPleaseWaitForMeOutside);
  Event_ShowMessageAndWait(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_SetAngleToward(ACTOR_PARTY_LEADER, ACTOR_IODEM, 0);
  Event_WaitFrames(30);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_WaitFrames(30);
  Event_End();
}

/// Lalivero (TimberRoomsC): Iodem, talked to before he has given the
/// Black Orb, repeats his request.
static void LaliveroInteriors_TalkToIodem(void) {

  GameFlag_Set(FLAG_IODEM_ASKED_TO_MEET);
  Event_SetMessageId(MsgLaliveroPleaseWaitForMeOutside);
  Event_ShowMessageAndWait(ACTOR_IODEM, 0);
}

/// Both maps, a doorway out: take the touched step trigger's exit with the exit
/// sound.
static void LaliveroInteriors_LeaveBuilding(void) {
  struct FieldWork *work = gFieldWork;
  s16 *trigger = (s16 *)&work->stepTrigger;

  Import_EventSetExitRequest(*trigger);
  Sound_PlaySong(SE_MAP_EXIT);
}

/// What the party can talk to, touch and search in each map, before and
/// after Venus Lighthouse.
const struct Trigger *LaliveroInteriors_GetTriggers(void) {
  if (gSaveBlock.sceneId == (s32)&SceneId_TimberRoomsC) {
    if (GameFlag_Get(FLAG_VENUS_LIGHTHOUSE_PARTY_RETURNED) != 0) {
      return gTimberRoomsC_TriggersAfterLighthouse;
    }
    return gTimberRoomsC_Triggers;
  }
  if (GameFlag_Get(FLAG_VENUS_LIGHTHOUSE_PARTY_RETURNED) != 0) {
    return gTimberRoomsB_TriggersAfterLighthouse;
  }
  return gTimberRoomsB_Triggers;
}

/// Lalivero (TimberRoomsC),
/// TIMBER_ROOMS_C_FROM_VENUS_LIGHTHOUSE_AERIE back from Venus
/// Lighthouse: the party reports to Faran and Iodem. No sign of Sheba, nor of
/// Jenna and the others; Faran's faith that Sheba lived, what an Adept is, and
/// a farewell as the party sets out to search for Jenna.
static void LaliveroInteriors_ReportToFaran(void) {
  struct Actor *leader;

  Event_Start();
  Party_RestoreHpPp(1);
  // The party walks in to Faran and Iodem, and the others step out.
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(96), PIXELS(184));
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_FARAN, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_IODEM, FACING_WEST, 0);
  Event_SetMessageId(MsgLaliveroHowDidSearchForSheba);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_WaitFrames(10);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
  Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 0, -16);
  Actor_SetPositionAndReset(ACTOR_PARTY_LEADER, 104, 136);
  Event_WaitFrames(10);
  FieldActor_LaunchFromLeader(ACTOR_GARET, -16, 16, FACING_NORTH);
  FieldActor_LaunchFromLeader(ACTOR_MIA, 0, 24, FACING_NORTH);
  FieldActor_LaunchFromLeader(ACTOR_IVAN, 16, 16, FACING_NORTH);
  Actor_WaitForMoveById(ACTOR_GARET);
  Event_WaitFrames(20);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Event_WaitFrames(20);
  Event_ShowMessageAndWait(ACTOR_IVAN, 0);
  Event_WaitFrames(10);
  // Faran and Iodem tell of the search for Sheba.
  Actor_FaceDirection(ACTOR_FARAN, FACING_SOUTH, 0);
  Actor_ArmCallback(ACTOR_IODEM, FACING_SOUTH, 0);
  Event_WaitFrames(30);
  Actor_ShowEmote(ACTOR_FARAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 70);
  Event_ShowMessageAndWait(ACTOR_FARAN, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_FARAN, ANIM_SHAKE_HEAD);
  Event_WaitFrames(20);
  Event_ShowMessageAndWait(ACTOR_FARAN, 0);
  Event_WaitFrames(20);
  Actor_RunRepeatedMotion(ACTOR_FARAN, 2);
  Event_WaitFrames(20);
  Event_ShowMessageAndWait(ACTOR_FARAN, 0);
  Event_WaitFrames(20);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_ShowMessageAndWait(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_IODEM, ANIM_SHAKE_HEAD);
  Event_WaitFrames(20);
  Event_ShowMessageAndWait(ACTOR_IODEM, 0);
  Event_WaitFrames(20);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_FARAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 50);
  Event_ShowMessageAndWait(ACTOR_FARAN, 0);
  // The party tells what became of Jenna and the others; the version that saw
  // the ship at Idejima knows Idejima was swept away with it.
  if (GameFlag_Get(FLAG_SAW_SHIP_AT_IDEJIMA) == 0) {
    LaliveroInteriors_TellFaranAboutJenna();
  } else {
    Event_WaitFrames(20);
    Actor_RunRepeatedMotion(ACTOR_FARAN, 2);
    Event_WaitFrames(20);
    Event_ShowMessageAndWait(ACTOR_FARAN, 0);
    Event_WaitFrames(10);
    Actor_FaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 50);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
    Event_WaitFrames(10);
    Actor_ArmCallback(ACTOR_GARET, FACING_NORTH, 0);
    Event_WaitFrames(20);
    Event_ShowMessageAndWait(ACTOR_GARET, 0);
    Event_WaitFrames(10);
    Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
    Event_WaitFrames(20);
    Event_ShowMessageAndWait(ACTOR_MIA, 0);
    Event_WaitFrames(10);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Event_WaitFrames(20);
    Event_ShowMessageAndWait(ACTOR_IVAN, 0);
    Event_WaitFrames(20);
    Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
    Event_OpenMessage(ACTOR_IODEM, 0);
    Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0);
    Event_WaitFrames(20);
    Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
    Event_WaitFrames(20);
    Event_ShowMessageAndWait(ACTOR_IODEM, 0);
    Event_WaitFrames(20);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Event_WaitFrames(30);
    Event_WaitFrames(10);
    Actor_RunRepeatedMotion(ACTOR_GARET, 2);
    Event_WaitFrames(20);
    Event_ShowMessageAndWait(ACTOR_GARET, 0);
    Event_WaitFrames(10);
    Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
    Event_WaitFrames(20);
    Event_ShowMessageAndWait(ACTOR_MIA, 0);
    Event_WaitFrames(10);
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    Event_WaitFrames(30);
    Event_ShowMessageAndWait(ACTOR_IVAN, 0);
    Event_WaitFrames(10);
    Actor_FaceActor(ACTOR_IVAN, ACTOR_PARTY_LEADER, 30);
    Event_OpenMessage(ACTOR_IVAN | SPEAKER_PORTRAIT_RIGHT, 0);
  }
  // Will the party keep searching? Faran answers either way.
  Event_SetMessageId(MsgLaliveroDoNotWorryAboutSheba);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 20);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_WaitFrames(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
    Actor_ArmCallback(ACTOR_IVAN, FACING_NORTH, 0);
    Event_WaitFrames(30);
    Event_WaitFrames(10);
    Actor_SetAnimationAndWait(ACTOR_FARAN, ANIM_NOD);
    Event_WaitFrames(30);
    Event_ShowMessageAndWait(ACTOR_FARAN, 0);
    (gFieldWork->message)++;
  } else {
    Event_WaitFrames(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
    Actor_ArmCallback(ACTOR_IVAN, FACING_NORTH, 0);
    Event_WaitFrames(30);
    Event_WaitFrames(10);
    Actor_SetAnimationAndWait(ACTOR_FARAN, ANIM_SHAKE_HEAD);
    Event_WaitFrames(20);
    (gFieldWork->message)++;
    Event_ShowMessage(ACTOR_FARAN, 0);
  }
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_FARAN, 2);
  Event_WaitFrames(20);
  Event_ShowMessageAndWait(ACTOR_FARAN, 0);
  Event_WaitFrames(10);
  Actor_LinkPair(ACTOR_GARET, ACTOR_PARTY_LEADER, 0);
  Actor_LinkPair(ACTOR_MIA, ACTOR_IVAN, 0);
  Event_WaitFrames(40);
  Event_WaitFrames(10);
  // Faran believes Sheba lived; Iodem explains Adepts.
  Actor_ShowEmote(ACTOR_FARAN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_ShowMessageAndWait(ACTOR_FARAN, 0);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Event_WaitFrames(50);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_FARAN, ANIM_NOD);
  Event_WaitFrames(30);
  Event_ShowMessageAndWait(ACTOR_FARAN, 0);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_FARAN, 2);
  Event_WaitFrames(20);
  Event_ShowMessageAndWait(ACTOR_FARAN, 0);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Event_ShowMessageAndWait(ACTOR_MIA, 0);
  Event_WaitFrames(20);
  Actor_ArmCallback(ACTOR_FARAN, FACING_EAST, 0);
  Event_WaitFrames(20);
  Event_ShowMessageAndWait(ACTOR_FARAN, 0);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_WaitFrames(20);
  Actor_FaceDirection(ACTOR_IODEM, FACING_WEST, 0);
  Event_WaitFrames(20);
  Event_ShowMessageAndWait(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_FARAN, ANIM_SHAKE_HEAD);
  Event_WaitFrames(20);
  Event_ShowMessageAndWait(ACTOR_FARAN, 0);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Event_WaitFrames(20);
  Event_ShowMessageAndWait(ACTOR_IVAN, 0);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_FARAN, FACING_SOUTH, 0);
  Event_WaitFrames(30);
  Event_ShowMessageAndWait(ACTOR_FARAN, 0);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
  Event_ShowMessageAndWait(ACTOR_IODEM, 0);
  Event_WaitFrames(20);
  Actor_ShowEmote(ACTOR_FARAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 65);
  Actor_ArmCallback(ACTOR_FARAN, FACING_EAST, 0);
  Event_WaitFrames(20);
  Event_ShowMessageAndWait(ACTOR_FARAN, 0);
  Event_WaitFrames(20);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Event_ShowMessageAndWait(ACTOR_IODEM, 0);
  Event_WaitFrames(20);
  Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTH, 0);
  Event_WaitFrames(40);
  Event_ShowMessageAndWait(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_FARAN, FACING_SOUTH, 0);
  Event_WaitFrames(50);
  Actor_ShowEmote(ACTOR_FARAN, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 60);
  Event_ShowMessageAndWait(ACTOR_FARAN, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_FARAN, ANIM_NOD);
  Event_WaitFrames(30);
  Event_ShowMessageAndWait(ACTOR_FARAN, 0);
  Event_WaitFrames(10);
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  Actor_StartRepeatedMotion(ACTOR_MIA, 2);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Event_WaitFrames(30);
  Event_WaitFrames(10);
  Actor_ArmCallback(ACTOR_FARAN, FACING_EAST, 0);
  Event_WaitFrames(20);
  Event_ShowMessageAndWait(ACTOR_FARAN, 0);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_WaitFrames(30);
  Actor_FaceDirection(ACTOR_IODEM, FACING_WEST, 0);
  Event_WaitFrames(20);
  Event_ShowMessageAndWait(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_FARAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
  Event_WaitFrames(30);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
  Event_WaitFrames(20);
  Event_ShowMessageAndWait(ACTOR_GARET, 0);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_IODEM, FACING_SOUTH, 0);
  Event_WaitFrames(30);
  Actor_ShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Event_ShowMessageAndWait(ACTOR_IODEM, 0);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_WaitFrames(20);
  Event_ShowMessageAndWait(ACTOR_GARET, 0);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_FARAN, FACING_SOUTH, 0);
  Event_WaitFrames(40);
  Actor_ShowEmote(ACTOR_FARAN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_ShowMessageAndWait(ACTOR_FARAN, 0);
  Event_WaitFrames(10);
  Actor_ArmCallback(ACTOR_IVAN, FACING_WEST, 0);
  Event_WaitFrames(30);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_SCOWL, 60);
  Actor_ArmCallback(ACTOR_IVAN, FACING_NORTH, 0);
  Event_WaitFrames(30);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
  Event_WaitFrames(20);
  Event_ShowMessageAndWait(ACTOR_IVAN, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_FARAN, ANIM_SHAKE_HEAD);
  Event_WaitFrames(20);
  Event_ShowMessageAndWait(ACTOR_FARAN, 0);
  Event_WaitFrames(10);
  // Faran sends them off.
  Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Actor_StartRepeatedMotion(ACTOR_GARET, 2);
  Actor_StartRepeatedMotion(ACTOR_MIA, 2);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Event_WaitFrames(30);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_FARAN, ANIM_NOD);
  Event_WaitFrames(30);
  Event_ShowMessageAndWait(ACTOR_FARAN, 0);
  Event_WaitFrames(20);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_MIA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_WaitFrames(30);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_FARAN, 2);
  Event_WaitFrames(20);
  Event_ShowMessageAndWait(ACTOR_FARAN, 0);
  Event_WaitFrames(10);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_WaitFrames(20);
  Event_ShowMessageAndWait(ACTOR_GARET, 0);
  Event_WaitFrames(10);
  Actor_SetAnimationAndWait(ACTOR_FARAN, ANIM_NOD);
  Event_WaitFrames(30);
  Event_ShowMessageAndWait(ACTOR_FARAN, 0);
  Event_WaitFrames(20);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_WaitFrames(30);
  Event_ShowMessageAndWait(ACTOR_MIA, 0);
  Event_WaitFrames(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
  Actor_ArmCallback(ACTOR_IVAN, FACING_NORTHWEST, 0);
  Event_WaitFrames(30);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_WaitFrames(10);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_WaitFrames(30);
  // The others walk back into the leader.
  Actor_SetSpeed(ACTOR_GARET, 0x13333, 0x9999);
  Actor_SetSpeed(ACTOR_MIA, 0x13333, 0x9999);
  Actor_SetSpeed(ACTOR_IVAN, 0x13333, 0x9999);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_WALK);
  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_ResetAndSetPosition(ACTOR_GARET, leader->x.part.pixel,
                              leader->z.part.pixel);
  }
  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_SetPosition(ACTOR_GARET, 0, 0);
  Import_ActorSetAnimation(ACTOR_MIA, ANIM_WALK);
  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_ResetAndSetPosition(ACTOR_MIA, leader->x.part.pixel,
                              leader->z.part.pixel);
  }
  Actor_WaitForMoveById(ACTOR_MIA);
  Actor_SetPosition(ACTOR_MIA, 0, 0);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_WALK);
  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_ResetAndSetPosition(ACTOR_IVAN, leader->x.part.pixel,
                              leader->z.part.pixel);
  }
  Actor_WaitForMoveById(ACTOR_IVAN);
  Import_ActorSetPosition(ACTOR_IVAN, 0, 0);
  Event_WaitFrames(10);
  Event_End();
}

/// The party tells Faran and Iodem what became of Jenna and the others, when
/// it did not see the ship at Idejima: Felix said he sent Jenna ahead to the
/// ship.
static void LaliveroInteriors_TellFaranAboutJenna(void) {
  Event_SetMessageId(MsgLaliveroTellOthers);
  Event_WaitFrames(20);

  Actor_RunRepeatedMotion(ACTOR_FARAN, 2);
  Event_WaitFrames(20);
  Event_ShowMessageAndWait(ACTOR_FARAN, 0);
  Event_WaitFrames(10);

  Actor_SetAngleToward(ACTOR_GARET, ACTOR_PARTY_LEADER, 50);
  FieldEffect_SpawnAttached(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
                            60);
  Event_WaitFrames(10);
  Actor_ArmCallback(ACTOR_GARET, FACING_NORTH, 0);
  Event_WaitFrames(20);
  Event_ShowMessageAndWait(ACTOR_GARET, 0);
  Event_WaitFrames(10);

  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
  Event_WaitFrames(20);
  Event_ShowMessageAndWait(ACTOR_MIA, 0);
  Event_WaitFrames(10);

  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Event_WaitFrames(20);
  Event_ShowMessageAndWait(ACTOR_IVAN, 0);
  Event_WaitFrames(10);

  Actor_RunRepeatedMotion(ACTOR_IODEM, 2);
  Event_WaitFrames(20);
  Event_ShowMessageAndWait(ACTOR_IODEM, 0);
  Event_WaitFrames(20);

  Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Event_WaitFrames(20);
  Event_WaitFrames(25);

  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  Event_WaitFrames(30);
  Event_ShowMessageAndWait(ACTOR_GARET, 0);
  Event_WaitFrames(30);

  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
  Event_WaitFrames(20);
  Event_ShowMessageAndWait(ACTOR_GARET, 0);
  Event_WaitFrames(10);

  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
  Event_WaitFrames(20);
  Event_ShowMessageAndWait(ACTOR_MIA, 0);
  Event_WaitFrames(10);

  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_WaitFrames(30);
  Event_ShowMessageAndWait(ACTOR_IVAN, 0);
  Event_WaitFrames(10);

  Actor_SetAngleToward(ACTOR_IVAN, ACTOR_PARTY_LEADER, 30);
  FieldMessage_ShowAtActor(ACTOR_IVAN | SPEAKER_PORTRAIT_RIGHT, 0);
}

/// Sets both maps up as the party enters: shopkeepers and the innkeeper
/// behind their counters, the Sanctum as the place to wake after a defeat,
/// the inn worker back at her chores, and the report to Faran on coming back
/// from Venus Lighthouse.
s32 Scene_Initialize(void) {
  s32 sceneId;
  s16 entrance;
  struct Actor *actor;

  if (gSaveBlock.entrance == ENTRANCE_FROM_AERIE) {
    GameFlag_Set(FLAG_VENUS_LIGHTHOUSE_PARTY_RETURNED);
  }
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  sceneId = gSaveBlock.sceneId;
  if (sceneId == (s32)&SceneId_TimberRoomsB) {
    // Each shopkeeper is drawn under the counter's top, and counts as nearer
    // to talk to (ACTOR_NEAR_TALK) so that the leader reaches it across the
    // counter.
    actor = Actor_GetById(ACTOR_ITEM_MERCHANT);
    actor->priorityFlags = 0;
    actor->collisionFlags |= ACTOR_NEAR_TALK;
    actor->sprite->image.priority = 2;
    actor = Actor_GetById(ACTOR_WEAPON_MERCHANT);
    actor->priorityFlags = 0;
    actor->collisionFlags |= ACTOR_NEAR_TALK;
    actor->sprite->image.priority = 2;
    actor = Actor_GetById(ACTOR_ARMOR_MERCHANT);
    actor->collisionFlags |= ACTOR_NEAR_TALK;
    actor->priorityFlags = 0;
    actor->sprite->image.priority = 2;
    Import_ActorSetAnimation(ACTOR_NAPPING_WORKER, ANIM_WORKER_LIE_ASLEEP);
    entrance = gSaveBlock.entrance;
    if (entrance == TIMBER_ROOMS_B_SANCTUM) {
      gSaveBlock.restartScene = sceneId;
      gSaveBlock.restartEntrance = entrance;
    }
  }
  if (gSaveBlock.sceneId == (s32)&SceneId_TimberRoomsC) {
    actor = Actor_GetById(ACTOR_INNKEEPER);
    actor->collisionFlags |= ACTOR_NEAR_TALK;
    actor->priorityFlags = 0;
    actor->sprite->image.priority = 2;
    if (GameFlag_Get(FLAG_LALIVERO_INN_WORKER_APOLOGIZED) != 0) {
      Actor_SetScript(ACTOR_INN_WORKER, gLaliveroInteriors_InnWorkerChores);
    }
    if (gSaveBlock.entrance == TIMBER_ROOMS_C_FROM_VENUS_LIGHTHOUSE_AERIE) {
      LaliveroInteriors_ReportToFaran();
      // Iodem and Faran turn toward the leader, as their later actor
      // templates do.
      Actor_SetMovementType(Actor_GetById(ACTOR_IODEM),
                            MOVEMENT_TYPE_FACE_LEADER);
      Actor_SetMovementType(Actor_GetById(ACTOR_FARAN),
                            MOVEMENT_TYPE_FACE_LEADER);
      gSaveBlock.entrance = TIMBER_ROOMS_C_FROM_LALIVERO_SOUTHWEST;
    }
  }
  return 0;
}

/// The inn worker's chores once she has apologized: she paces between two
/// spots, turning to each side, and sighs now and then.
s32 gLaliveroInteriors_InnWorkerChores[] = {
    SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_SOLID),
    SC_SET(SW_SPEED, 0x26666),
    SC_SET(SW_ACCELERATION, 0x13333),
    SC_LABEL(1),
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(128), 0, PIXELS(300)),
    SC_SYNC,
    SC_SET(SW_FACING, FACING_NORTHWEST + FACING_STEP),
    SC_ANIMATION(3),
    SC_WAIT(30),
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(168), 0, PIXELS(300)),
    SC_SYNC,
    SC_SET(SW_FACING, FACING_NORTH + FACING_STEP),
    SC_ANIMATION(3),
    SC_WAIT(50),
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(128), 0, PIXELS(300)),
    SC_SYNC,
    SC_SET(SW_FACING, FACING_NORTHWEST + FACING_STEP),
    SC_ANIMATION(3),
    SC_WAIT(20),
    SC_CALL(InnWorker_Sigh),
    SC_WAIT(30),
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(168), 0, PIXELS(300)),
    SC_SYNC,
    SC_SET(SW_FACING, FACING_NORTH + FACING_STEP),
    SC_ANIMATION(3),
    SC_WAIT(30),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
};

struct SceneEntrance gLaliveroInteriors_Entrances[] = {
    {TIMBER_ROOMS_B_FROM_LALIVERO_WEST, CONDITION_ALWAYS, 144, 0, 168,
     FACING_NORTH, 0, 0, 8, 240, 192, 0},
    {TIMBER_ROOMS_B_FROM_LALIVERO_SOUTHEAST, CONDITION_ALWAYS, 400, 0, 168,
     FACING_NORTH, 0, 256, 8, 496, 192, 0},
    {TIMBER_ROOMS_B_ITEM_SHOP_UPSTAIRS_FROM_ITEM_SHOP, CONDITION_ALWAYS, 648, 0,
     72, FACING_NORTH, 0, 488, 8, 728, 184, 0},
    {TIMBER_ROOMS_B_FROM_LALIVERO_NORTH, CONDITION_ALWAYS, 128, 0, 472,
     FACING_NORTH, 0, 0, 216, 240, 488, 0},
    {TIMBER_ROOMS_B_ITEM_SHOP_FROM_ITEM_SHOP_UPSTAIRS, CONDITION_ALWAYS, 136, 0,
     296, FACING_SOUTH, 0, 0, 216, 240, 488, 0},
    {TIMBER_ROOMS_B_FROM_LALIVERO_NORTHWEST, CONDITION_ALWAYS, 368, 0, 472,
     FACING_NORTH, 0, 240, 216, 480, 488, 0},
    {TIMBER_ROOMS_B_NORTHWEST_HOUSE_FROM_NORTHWEST_HOUSE_UPSTAIRS,
     CONDITION_ALWAYS, 392, 0, 296, FACING_SOUTH, 0, 240, 216, 480, 488, 0},
    {TIMBER_ROOMS_B_NORTHWEST_HOUSE_UPSTAIRS_FROM_NORTHWEST_HOUSE,
     CONDITION_ALWAYS, 648, 0, 280, FACING_NORTH, 0, 480, 216, 720, 488, 0},
    {TIMBER_ROOMS_B_FROM_LALIVERO_SOUTH, CONDITION_ALWAYS, 96, 0, 680,
     FACING_NORTH, 0, 8, 520, 312, 696, 0},
    {TIMBER_ROOMS_B_FROM_LALIVERO_EAST_SOUTHEAST, CONDITION_ALWAYS, 200, 0, 600,
     FACING_SOUTH, 0, 8, 520, 312, 696, 0},
    {TIMBER_ROOMS_B_FROM_LALIVERO_NORTHEAST, CONDITION_ALWAYS, 432, 0, 728,
     FACING_NORTH, 0, 312, 520, 552, 744, 0},
    {TIMBER_ROOMS_B_SANCTUM, CONDITION_ALWAYS, 432, 0, 630, FACING_NORTH, 0,
     312, 520, 552, 744, 0},
    {TIMBER_ROOMS_C_FROM_LALIVERO_SOUTHWEST, CONDITION_ALWAYS, 96, 0, 168,
     FACING_NORTH, 0, 8, 8, 312, 192, 0},
    {TIMBER_ROOMS_C_FROM_LALIVERO_SOUTH, CONDITION_ALWAYS, 216, 0, 88,
     FACING_SOUTH, 0, 8, 8, 312, 192, 0},
    {TIMBER_ROOMS_C_FROM_LALIVERO_NORTHEAST, CONDITION_ALWAYS, 224, 0, 392,
     FACING_NORTH, 0, 8, 232, 312, 416, 0},
    {TIMBER_ROOMS_C_INN_FROM_INN_UPSTAIRS, CONDITION_ALWAYS, 88, 0, 312,
     FACING_SOUTH, 0, 8, 232, 312, 416, 0},
    {TIMBER_ROOMS_C_INN_UPSTAIRS_FROM_INN, CONDITION_ALWAYS, 384, 0, 368,
     FACING_NORTH, 0, 320, 240, 632, 400, 0},
    {TIMBER_ROOMS_C_INN_UPSTAIRS_FROM_GUEST_ROOM_WEST, CONDITION_ALWAYS, 456, 0,
     376, FACING_NORTH, 0, 320, 240, 632, 400, 0},
    {TIMBER_ROOMS_C_INN_UPSTAIRS_FROM_GUEST_ROOM_EAST, CONDITION_ALWAYS, 504, 0,
     376, FACING_NORTH, 0, 320, 240, 632, 400, 0},
    {TIMBER_ROOMS_C_GUEST_ROOM_WEST_FROM_INN_UPSTAIRS, CONDITION_ALWAYS, 472, 0,
     72, FACING_SOUTH, 0, 344, 8, 584, 168, 0},
    {TIMBER_ROOMS_C_GUEST_ROOM_EAST_FROM_INN_UPSTAIRS, CONDITION_ALWAYS, 744, 0,
     72, FACING_SOUTH, 0, 632, 8, 872, 168, 0},
    {TIMBER_ROOMS_C_FROM_LALIVERO_NORTHWEST, CONDITION_ALWAYS, 832, 0, 872,
     FACING_NORTH, 0, 280, 504, 912, 896, 0},
    {TIMBER_ROOMS_C_FROM_LEMURIAN_SHIP, CONDITION_ALWAYS, 464, 0, 832,
     FACING_SOUTH, 0, 280, 504, 912, 896, 0},
    {TIMBER_ROOMS_C_FROM_VENUS_LIGHTHOUSE_AERIE, CONDITION_ALWAYS, 96, 0, 168,
     FACING_NORTH, 0, 8, 8, 312, 192, 0},
    {SCENE_TABLE_END},
};

// clang-format off
struct SceneExitRegion gTimberRoomsB_Regions[] = {
    {196, 28, 595, 204, 36, 603, CONDITION_ALWAYS, TIMBER_ROOMS_B_TO_LALIVERO_EAST_SOUTHEAST},
    {SCENE_TABLE_END},
};

struct SceneExitRegion gTimberRoomsC_Regions[] = {
    {212, 28, 83, 220, 36, 91, CONDITION_ALWAYS, TIMBER_ROOMS_C_TO_LALIVERO_SOUTH},
    {SCENE_TABLE_END},
};

u32 gLaliveroInteriors_Exits[] = {
    SCENE_EXITS(TimberRoomsB),
    SCENE_EXIT(TIMBER_ROOMS_B_TO_LALIVERO_WEST, Lalivero, LALIVERO_FROM_TIMBER_ROOMS_B_WEST),
    SCENE_EXIT(TIMBER_ROOMS_B_TO_LALIVERO_SOUTHEAST, Lalivero, LALIVERO_FROM_TIMBER_ROOMS_B_SOUTHEAST),
    SCENE_EXIT(TIMBER_ROOMS_B_ITEM_SHOP_UPSTAIRS_TO_ITEM_SHOP, TimberRoomsB, TIMBER_ROOMS_B_ITEM_SHOP_FROM_ITEM_SHOP_UPSTAIRS),
    SCENE_EXIT(TIMBER_ROOMS_B_TO_LALIVERO_NORTH, Lalivero, LALIVERO_FROM_TIMBER_ROOMS_B_NORTH),
    SCENE_EXIT(TIMBER_ROOMS_B_ITEM_SHOP_TO_ITEM_SHOP_UPSTAIRS, TimberRoomsB, TIMBER_ROOMS_B_ITEM_SHOP_UPSTAIRS_FROM_ITEM_SHOP),
    SCENE_EXIT(TIMBER_ROOMS_B_TO_LALIVERO_NORTHWEST, Lalivero, LALIVERO_FROM_TIMBER_ROOMS_B_NORTHWEST),
    SCENE_EXIT(TIMBER_ROOMS_B_NORTHWEST_HOUSE_TO_NORTHWEST_HOUSE_UPSTAIRS, TimberRoomsB, TIMBER_ROOMS_B_NORTHWEST_HOUSE_UPSTAIRS_FROM_NORTHWEST_HOUSE),
    SCENE_EXIT(TIMBER_ROOMS_B_NORTHWEST_HOUSE_UPSTAIRS_TO_NORTHWEST_HOUSE, TimberRoomsB, TIMBER_ROOMS_B_NORTHWEST_HOUSE_FROM_NORTHWEST_HOUSE_UPSTAIRS),
    SCENE_EXIT(TIMBER_ROOMS_B_TO_LALIVERO_SOUTH, Lalivero, LALIVERO_FROM_TIMBER_ROOMS_B_SOUTH),
    SCENE_EXIT(TIMBER_ROOMS_B_TO_LALIVERO_EAST_SOUTHEAST, Lalivero, LALIVERO_FROM_TIMBER_ROOMS_B_EAST_SOUTHEAST),
    SCENE_EXIT(TIMBER_ROOMS_B_TO_LALIVERO_NORTHEAST, Lalivero, LALIVERO_FROM_TIMBER_ROOMS_B_NORTHEAST),
    SCENE_EXITS(TimberRoomsC),
    SCENE_EXIT(TIMBER_ROOMS_C_TO_LALIVERO_SOUTHWEST, Lalivero, LALIVERO_FROM_TIMBER_ROOMS_C_SOUTHWEST),
    SCENE_EXIT(TIMBER_ROOMS_C_TO_LALIVERO_SOUTH, Lalivero, LALIVERO_FROM_TIMBER_ROOMS_C_SOUTH),
    SCENE_EXIT(TIMBER_ROOMS_C_TO_LALIVERO_NORTHEAST, Lalivero, LALIVERO_FROM_TIMBER_ROOMS_C_NORTHEAST),
    SCENE_EXIT(TIMBER_ROOMS_C_INN_TO_INN_UPSTAIRS, TimberRoomsC, TIMBER_ROOMS_C_INN_UPSTAIRS_FROM_INN),
    SCENE_EXIT(TIMBER_ROOMS_C_INN_UPSTAIRS_TO_INN, TimberRoomsC, TIMBER_ROOMS_C_INN_FROM_INN_UPSTAIRS),
    SCENE_EXIT(TIMBER_ROOMS_C_INN_UPSTAIRS_TO_GUEST_ROOM_WEST, TimberRoomsC, TIMBER_ROOMS_C_GUEST_ROOM_WEST_FROM_INN_UPSTAIRS),
    SCENE_EXIT(TIMBER_ROOMS_C_INN_UPSTAIRS_TO_GUEST_ROOM_EAST, TimberRoomsC, TIMBER_ROOMS_C_GUEST_ROOM_EAST_FROM_INN_UPSTAIRS),
    SCENE_EXIT(TIMBER_ROOMS_C_GUEST_ROOM_WEST_TO_INN_UPSTAIRS, TimberRoomsC, TIMBER_ROOMS_C_INN_UPSTAIRS_FROM_GUEST_ROOM_WEST),
    SCENE_EXIT(TIMBER_ROOMS_C_GUEST_ROOM_EAST_TO_INN_UPSTAIRS, TimberRoomsC, TIMBER_ROOMS_C_INN_UPSTAIRS_FROM_GUEST_ROOM_EAST),
    SCENE_EXIT(TIMBER_ROOMS_C_TO_LALIVERO_NORTHWEST, Lalivero, LALIVERO_FROM_TIMBER_ROOMS_C_NORTHWEST),
    SCENE_EXIT(TIMBER_ROOMS_C_TO_LEMURIAN_SHIP, LemurianShip, LEMURIAN_SHIP_FROM_TIMBER_ROOMS_C),
    SCENE_EXITS_END,
};

struct ActorTemplate gTimberRoomsB_ActorTemplates[] = {
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(176), 0, PIXELS(104), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(72), 0, PIXELS(80), FACING_NORTH, TALK_FACING_PARTY, 0 },
    { SPRITE_MAN_IN_WHITE_TURBAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(328), 0, PIXELS(136), FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(344), 0, PIXELS(120), FACING_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR,
        PIXELS(616), 0, PIXELS(104), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(664), 0, PIXELS(168), FACING_WEST, TALK_FACING_PARTY, 0 },
    { SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(400), 0, PIXELS(416), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(550), 0, PIXELS(368), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(344), 0, PIXELS(304), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(600), 0, PIXELS(408), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_BALD_MAN_IN_RED, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(132), 0, PIXELS(616), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_YOUNG_MAN_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(92), 0, PIXELS(616), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_WITH_PONYTAIL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(68), 0, PIXELS(340), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(432), 0, PIXELS(600), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ActorTemplate gTimberRoomsC_ActorTemplates[] = {
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(168), 0, PIXELS(112), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(240), 0, PIXELS(96), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_YOUNG_WOMAN_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(136), 0, PIXELS(88), FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_FARAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_IODEM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_MAN_WITH_DARK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(248), 0, PIXELS(336), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_IN_PINK_APRON, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(136), 0, PIXELS(300), FACING_NORTH, TALK_FACING_KEEP, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(544), 0, PIXELS(368), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ActorTemplate gTimberRoomsC_ActorTemplatesAfterLighthouse[] = {
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(168), 0, PIXELS(112), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(240), 0, PIXELS(96), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_YOUNG_WOMAN_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(216), 0, PIXELS(168), FACING_WEST, TALK_FACING_PARTY, 0 },
    { SPRITE_FARAN, CONDITION_ALWAYS, MOVEMENT_TYPE_FACE_LEADER,
        PIXELS(104), 0, PIXELS(96), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_IODEM, CONDITION_ALWAYS, MOVEMENT_TYPE_FACE_LEADER,
        PIXELS(128), 0, PIXELS(96), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_MAN_WITH_DARK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(248), 0, PIXELS(336), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_IN_PINK_APRON, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(160), 0, PIXELS(304), FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        PIXELS(544), 0, PIXELS(368), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct Trigger gTimberRoomsB_Triggers[] = {
    { TRIGGER_STEP, 1, CONDITION_ALWAYS, EVENT(LaliveroInteriors_LeaveBuilding) },
    { TRIGGER_STEP, 2, CONDITION_ALWAYS, EVENT(LaliveroInteriors_LeaveBuilding) },
    { TRIGGER_STEP, 3, CONDITION_ALWAYS, EVENT(LaliveroInteriors_LeaveBuilding) },
    { TRIGGER_STEP, 4, CONDITION_ALWAYS, EVENT(LaliveroInteriors_LeaveBuilding) },
    { TRIGGER_STEP, 5, CONDITION_ALWAYS, EVENT(LaliveroInteriors_LeaveBuilding) },
    { TRIGGER_STEP, 6, CONDITION_ALWAYS, EVENT(LaliveroInteriors_LeaveBuilding) },
    { TRIGGER_STEP, 7, CONDITION_ALWAYS, EVENT(LaliveroInteriors_LeaveBuilding) },
    { TRIGGER_STEP, 8, CONDITION_ALWAYS, EVENT(LaliveroInteriors_LeaveBuilding) },
    { TRIGGER_STEP, 9, CONDITION_ALWAYS, EVENT(LaliveroInteriors_LeaveBuilding) },
    { TRIGGER_STEP, 11, CONDITION_ALWAYS, EVENT(LaliveroInteriors_LeaveBuilding) },
    { TRIGGER_TALK, ACTOR_VILLAGER_SAW_SHEBAS_POWER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsOfCourseIHaveSeenActor) },
    { TRIGGER_TALK, ACTOR_WOMAN_DEFENDING_SHEBA, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsThereAreSomeWhoDoNot) },
    { TRIGGER_TALK, ACTOR_VILLAGER_DOUBTING_FARAN, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsCouldFaranHaveTrickedUsAll) },
    { TRIGGER_TALK, ACTOR_WOMAN_DOUBTING_SHEBA, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIfActorCantUseHerPowers) },
    { TRIGGER_TALK, ACTOR_OLD_MAN_SHOOTING_STAR, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsActorCameFlyingToTheRuins) },
    { TRIGGER_TALK, ACTOR_OLD_WOMAN_SIX_OF_THEM, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsEvenActorCouldntHaveEscapedSix) },
    { TRIGGER_TALK, ACTOR_WORKER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsTheySayAManNamedIodem) },
    { TRIGGER_TALK, ACTOR_NAPPING_WORKER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsNoOnesCalledMeSoMaybe) },
    { TRIGGER_TALK, ACTOR_STRONGMAN_RUINS_CAVE, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsActorSaidTheCaveAtThe) },
    { TRIGGER_TALK, ACTOR_STRONGMAN_RUINS_RIDDLE, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsTheRuinsThatFormTheLighthouse) },
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
    { TRIGGER_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, EVENT(LaliveroInteriors_TalkToWeaponMerchant) },
    { TRIGGER_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT(LaliveroInteriors_TalkToArmorMerchant) },
    { TRIGGER_TALK, ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS, EVENT(LaliveroInteriors_TalkToItemMerchant) },
    { TRIGGER_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT(LaliveroInteriors_TalkToHealer) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsTheLaliveransAreATimidPeople) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsICantRescueActorIfI) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIfAnythingHappensToActorIll) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIBetBabisTowerWillBe) },
    { EXAMINE_TRIGGER(EXAMINE_BARREL), 100, FLAG_TIMBER_ROOMS_B_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { EXAMINE_TRIGGER(EXAMINE_BARREL), 101, FLAG_TIMBER_ROOMS_B_ANTIDOTE, FIND_ITEM(ITEM_ANTIDOTE) },
    { EXAMINE_TRIGGER(EXAMINE_OVEN), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsShesMakingMushroomSoupInLalivero)) },
    { EXAMINE_TRIGGER(EXAMINE_OVEN), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsItsALaliveroSaladWithPotato)) },
    { EXAMINE_TRIGGER(EXAMINE_OVEN), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsThePotIsFullOfA)) },
    { TRIGGERS_END },
};

struct Trigger gTimberRoomsB_TriggersAfterLighthouse[] = {
    { TRIGGER_STEP, 1, CONDITION_ALWAYS, EVENT(LaliveroInteriors_LeaveBuilding) },
    { TRIGGER_STEP, 2, CONDITION_ALWAYS, EVENT(LaliveroInteriors_LeaveBuilding) },
    { TRIGGER_STEP, 3, CONDITION_ALWAYS, EVENT(LaliveroInteriors_LeaveBuilding) },
    { TRIGGER_STEP, 4, CONDITION_ALWAYS, EVENT(LaliveroInteriors_LeaveBuilding) },
    { TRIGGER_STEP, 5, CONDITION_ALWAYS, EVENT(LaliveroInteriors_LeaveBuilding) },
    { TRIGGER_STEP, 6, CONDITION_ALWAYS, EVENT(LaliveroInteriors_LeaveBuilding) },
    { TRIGGER_STEP, 7, CONDITION_ALWAYS, EVENT(LaliveroInteriors_LeaveBuilding) },
    { TRIGGER_STEP, 8, CONDITION_ALWAYS, EVENT(LaliveroInteriors_LeaveBuilding) },
    { TRIGGER_STEP, 9, CONDITION_ALWAYS, EVENT(LaliveroInteriors_LeaveBuilding) },
    { TRIGGER_STEP, 11, CONDITION_ALWAYS, EVENT(LaliveroInteriors_LeaveBuilding) },
    { TRIGGER_TALK, ACTOR_VILLAGER_SAW_SHEBAS_POWER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsTheContinentBrokeApartAroundThe) },
    { TRIGGER_TALK, ACTOR_WOMAN_DEFENDING_SHEBA, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWhatHappenedToThoseLivingOn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_SAW_SHEBAS_POWER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIShudderToThinkWhatWould) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_DEFENDING_SHEBA, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWereLuckyThatWeLiveNorth) },
    { TRIGGER_TALK, ACTOR_VILLAGER_DOUBTING_FARAN, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsEveryoneSaysThatActorIsOkay) },
    { TRIGGER_TALK, ACTOR_WOMAN_DOUBTING_SHEBA, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIRealizedHowThoughtlessIWas) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_DOUBTING_FARAN, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIfSheIsIndeedTheChild) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_DOUBTING_SHEBA, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWeWantToBelieveInSomething) },
    { TRIGGER_TALK, ACTOR_OLD_MAN_SHOOTING_STAR, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsActorShouldBeFineSheWas) },
    { TRIGGER_TALK, ACTOR_OLD_WOMAN_SIX_OF_THEM, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsActorWillWakeWhenTheBeacon) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_SHOOTING_STAR, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsSeeingActorDiveMustHaveBeen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_SIX_OF_THEM, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsICantWaitUntilActorIs) },
    { TRIGGER_TALK, ACTOR_WORKER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsBabiLighthouseCrumbledWhenAllThe) },
    { TRIGGER_TALK, ACTOR_NAPPING_WORKER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIAlwaysWantedABreakBut) },
    { TRIGGER_TALK, ACTOR_STRONGMAN_RUINS_CAVE, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsSoTheRuinsReallyAreConnected) },
    { TRIGGER_TALK, ACTOR_STRONGMAN_RUINS_RIDDLE, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsYouFinallySolvedTheMysteryOf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WORKER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWillTheyContinueOnOrQuit) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_NAPPING_WORKER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsImWorriedThatWeWontGet) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRONGMAN_RUINS_CAVE, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWeCantPossiblyDigAHole) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRONGMAN_RUINS_RIDDLE, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsTheMysteryOfTheRuinsShould) },
    { TRIGGER_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, EVENT(LaliveroInteriors_TalkToWeaponMerchant) },
    { TRIGGER_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT(LaliveroInteriors_TalkToArmorMerchant) },
    { TRIGGER_TALK, ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS, EVENT(LaliveroInteriors_TalkToItemMerchant) },
    { TRIGGER_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT(LaliveroInteriors_TalkToHealer) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsNoOneCouldPossiblySwimIn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWhenWillTheOceanBeCalm) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsTheChampaArePeopleFromA) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsFirstTheLighthouseWasStruckDown) },
    { EXAMINE_TRIGGER(EXAMINE_BARREL), 100, FLAG_TIMBER_ROOMS_B_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { EXAMINE_TRIGGER(EXAMINE_BARREL), 101, FLAG_TIMBER_ROOMS_B_ANTIDOTE, FIND_ITEM(ITEM_ANTIDOTE) },
    { EXAMINE_TRIGGER(EXAMINE_OVEN), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsShesMakingMushroomSoupInLalivero)) },
    { EXAMINE_TRIGGER(EXAMINE_OVEN), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsItsALaliveroSaladWithPotato)) },
    { EXAMINE_TRIGGER(EXAMINE_OVEN), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsThePotIsFullOfA)) },
    { TRIGGERS_END },
};

struct Trigger gTimberRoomsC_Triggers[] = {
    { TRIGGER_STEP, 21, CONDITION_ALWAYS, EVENT(LaliveroInteriors_LeaveBuilding) },
    { TRIGGER_STEP, 23, CONDITION_ALWAYS, EVENT(LaliveroInteriors_LeaveBuilding) },
    { TRIGGER_STEP, 24, CONDITION_ALWAYS, EVENT(LaliveroInteriors_LeaveBuilding) },
    { TRIGGER_STEP, 25, CONDITION_ALWAYS, EVENT(LaliveroInteriors_LeaveBuilding) },
    { TRIGGER_STEP, 26, CONDITION_ALWAYS, EVENT(LaliveroInteriors_LeaveBuilding) },
    { TRIGGER_STEP, 27, CONDITION_ALWAYS, EVENT(LaliveroInteriors_LeaveBuilding) },
    { TRIGGER_STEP, 28, CONDITION_ALWAYS, EVENT(LaliveroInteriors_LeaveBuilding) },
    { TRIGGER_STEP, 29, CONDITION_ALWAYS, EVENT(LaliveroInteriors_LeaveBuilding) },
    { TRIGGER_STEP, 30, CONDITION_ALWAYS, EVENT(LaliveroInteriors_LeaveBuilding) },
    { TRIGGER_STEP, 31, CONDITION_ALWAYS, EVENT(LaliveroInteriors_LeaveBuilding) },
    { TRIGGER_TALK, ACTOR_FARANS_WIFE, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWeReceivedActorFromTheSky) },
    { TRIGGER_TALK, ACTOR_FARANS_SON, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIJustKnowThatFatherWill) },
    { TRIGGER_TALK, ACTOR_WORRIED_YOUNG_WOMAN, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsHisWifeSeemsVeryCalmConsidering) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FARANS_WIFE, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsItsSoPainfulToEvenThink) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FARANS_SON, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIJustWishSomeoneWouldPlease) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WORRIED_YOUNG_WOMAN, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsThatCoupleRaisedActorAsThough) },
    { TRIGGER_TALK, ACTOR_INN_WORKER, FLAG_LALIVERO_INN_WORKER_APOLOGIZED, EVENT(LaliveroInteriors_TalkToInnWorker) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x400 | ACTOR_INN_WORKER, FLAG_LALIVERO_INN_WORKER_APOLOGIZED, EVENT(LaliveroInteriors_TalkToInnWorker) },
    { TRIGGER_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, EVENT(LaliveroInteriors_TalkToInnkeeper) },
    { TRIGGER_TALK, ACTOR_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWorkingLikeThisKeepsMyMind) },
    { TRIGGER_TALK, ACTOR_COOK, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWeMustObeyTolbiThatsWhat) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWhyDidThisHaveToHappen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsHousecleaningChangingSheetsItsAllDone) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_COOK, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsOnceThisIncidentIsOverI) },
    { EXAMINE_TRIGGER(EXAMINE_BARREL), 102, FLAG_TIMBER_ROOMS_C_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { EXAMINE_TRIGGER(EXAMINE_OVEN), 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsShesFryingFreshwaterFishFromThe)) },
    { EXAMINE_TRIGGER(EXAMINE_OVEN), 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsItsRiceMadeWithGrainsFrom)) },
    { EXAMINE_TRIGGER(EXAMINE_OVEN), 205, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsShesMakingRoastChickenThisScent)) },
    { TRIGGERS_END },
};

struct Trigger gTimberRoomsC_TriggersAfterLighthouse[] = {
    { TRIGGER_STEP, 21, CONDITION_ALWAYS, EVENT(LaliveroInteriors_LeaveBuilding) },
    { TRIGGER_STEP, 23, CONDITION_ALWAYS, EVENT(LaliveroInteriors_LeaveBuilding) },
    { TRIGGER_STEP, 24, CONDITION_ALWAYS, EVENT(LaliveroInteriors_LeaveBuilding) },
    { TRIGGER_STEP, 25, CONDITION_ALWAYS, EVENT(LaliveroInteriors_LeaveBuilding) },
    { TRIGGER_STEP, 26, CONDITION_ALWAYS, EVENT(LaliveroInteriors_LeaveBuilding) },
    { TRIGGER_STEP, 27, CONDITION_ALWAYS, EVENT(LaliveroInteriors_LeaveBuilding) },
    { TRIGGER_STEP, 28, CONDITION_ALWAYS, EVENT(LaliveroInteriors_LeaveBuilding) },
    { TRIGGER_STEP, 29, CONDITION_ALWAYS, EVENT(LaliveroInteriors_LeaveBuilding) },
    { TRIGGER_STEP, 30, CONDITION_ALWAYS, EVENT(LaliveroInteriors_LeaveBuilding) },
    { TRIGGER_STEP, 31, CONDITION_ALWAYS, EVENT(LaliveroInteriors_LeaveBuilding) },
    { TRIGGER_STEP, 51, FLAG_IODEM_ASKED_TO_MEET, EVENT(LaliveroInteriors_IodemCallsOut) },
    { TRIGGER_TALK, ACTOR_IODEM, FLAG_IODEM_FAREWELL_SEEN, EVENT(LaliveroInteriors_TalkToIodem) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_IODEM, FLAG_IODEM_FAREWELL_SEEN, MSGNO(MsgLaliveroInteriorsIMustExplainTheirPromiseTo) },
    { TRIGGER_TALK, ACTOR_FARANS_WIFE, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsFaranSaysKnowingHerPowersActor) },
    { TRIGGER_TALK, ACTOR_FARANS_SON, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsYouAreLeavingAfterAllArent) },
    { TRIGGER_TALK, ACTOR_WORRIED_YOUNG_WOMAN, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsTheySayTheLighthouseCrumbledDown) },
    { TRIGGER_TALK, ACTOR_FARAN, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsYouWillFindJennaSomedayFarewell) },
    { TRIGGER_TALK, ACTOR_IODEM, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIsaacThatBoatBelongsToYou) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FARANS_WIFE, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIDontNeedFameOrFortune) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FARANS_SON, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWhyWontAnyoneStopIsaacAnd) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WORRIED_YOUNG_WOMAN, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsTolbiIsToBeBlamedFor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FARAN, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsSoThereAreSomePeopleLike) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_IODEM, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsPleaseUseThatShipToFind) },
    { TRIGGER_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, EVENT(LaliveroInteriors_TalkToInnkeeper) },
    { TRIGGER_TALK, ACTOR_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIMustSayItIsNice) },
    { TRIGGER_TALK, ACTOR_COOK, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsFishingIsSoDifficultNowadaysBecause) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWhyWouldTheLightingMakeSuch) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsImSleepyTheseBrightNightsHave) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_COOK, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsGeeWhatAmIGoingTo) },
    { EXAMINE_TRIGGER(EXAMINE_BARREL), 102, FLAG_TIMBER_ROOMS_C_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { EXAMINE_TRIGGER(EXAMINE_OVEN), 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsShesFryingFreshwaterFishFromThe)) },
    { EXAMINE_TRIGGER(EXAMINE_OVEN), 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsItsRiceMadeWithGrainsFrom)) },
    { EXAMINE_TRIGGER(EXAMINE_OVEN), 205, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsShesMakingRoastChickenThisScent)) },
    { TRIGGERS_END },
};
// clang-format on
