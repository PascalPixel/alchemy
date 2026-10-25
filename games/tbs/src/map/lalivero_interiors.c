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
/// Timber rooms B (MapId_TimberRoomsB) holds, by the warps that lead in, a
/// house (TIMBER_ROOMS_B_FROM_LALIVERO_WEST), a house
/// (TIMBER_ROOMS_B_FROM_LALIVERO_SOUTHEAST), the item shop with a room upstairs
/// (TIMBER_ROOMS_B_FROM_LALIVERO_NORTH), a house with an upper floor
/// (TIMBER_ROOMS_B_FROM_LALIVERO_NORTHWEST), the weapon and armor shop, entered
/// by either of its two doors (TIMBER_ROOMS_B_FROM_LALIVERO_SOUTH and
/// TIMBER_ROOMS_B_FROM_LALIVERO_EAST_SOUTHEAST), and the Sanctum
/// (TIMBER_ROOMS_B_FROM_LALIVERO_NORTHEAST). Each shopkeeper stands behind a
/// counter and trades only with a leader facing north across it
/// (EventScript_TalkToWeaponMerchant and the others); from any other side they
/// talk. Stepping into the Sanctum by TIMBER_ROOMS_B_SANCTUM makes it the place
/// a defeated party wakes. Barrels hold a Lucky Medal and an Antidote.
///
/// Timber rooms C (MapId_TimberRoomsC) holds, by the warps that lead in,
/// Faran's house (TIMBER_ROOMS_C_FROM_LALIVERO_SOUTHWEST and
/// TIMBER_ROOMS_C_FROM_LALIVERO_SOUTH), the inn with its upper floor and two
/// guest rooms (TIMBER_ROOMS_C_FROM_LALIVERO_NORTHEAST), and the way out to the
/// wharf where Babi's ship lies (TIMBER_ROOMS_C_FROM_LALIVERO_NORTHWEST,
/// leading on by TIMBER_ROOMS_C_TO_LEMURIAN_SHIP). Before Venus Lighthouse an
/// inn worker apologizes once for her distraction (EventScript_TalkToInnWorker,
/// FLAG_LALIVERO_INN_WORKER_APOLOGIZED) and goes back to her chores. The aerie
/// of Venus Lighthouse leads in by TIMBER_ROOMS_C_FROM_VENUS_LIGHTHOUSE_AERIE,
/// which plays EventScript_ReportToFaran: the party reports to Faran and Iodem,
/// in one of two versions by FLAG_SAW_SHIP_AT_IDEJIMA. Coord event 51
/// afterwards plays EventScript_IodemCallsOut (FLAG_IODEM_ASKED_TO_MEET), which
/// sends the party outside, where Iodem gives it the Black Orb (lalivero.c). A
/// barrel holds a Lucky Medal.

#include "action_script.h"
#include "save_block.h"
#include "map_ids.h"
#include "unit/import.h"
#include "unit/map/lalivero_interiors.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/field_services.h"
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
#include "script_layer.h"
#include "map_tables.h"
#include "field_service.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"
#include "songs.h"
#include "flags.h"
static void LaliveroInteriors_TellFaranAboutJenna(void);

/// A warp destination that sets FLAG_VENUS_LIGHTHOUSE_PARTY_RETURNED. No warp
/// leads to it and the warp destination table has no row for it: only the debug
/// map selector, which warps to any warp destination from 0 to 99, reaches it.
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

/// The object events of both maps, numbered by the rows of each map's object
/// event template tables. The people are named by what they say before Venus
/// Lighthouse and by what their sprites show.
enum LaliveroInteriorsLocalId {
  // The houses, the shops and the Sanctum (TimberRoomsB).
  /// A young man with brown hair in a purple shirt, in the house at
  /// TIMBER_ROOMS_B_FROM_LALIVERO_WEST. "Of course I have seen Sheba's
  /// power...
  /// It's incredible!"
  /// ★ Named from the game: "Of course I have seen"
  /// (MsgLaliveroInteriorsOfCourseIHaveSeenActor)
  LOCALID_VILLAGER_SAW_SHEBAS_POWER = LOCALID_FIRST_PLACED,
  /// A woman with fair hair, in the same house. "There are some who do not
  /// believe Sheba has any powers... Doubting Sheba? That is foolish!"
  /// ★ Named from the game: "That is foolish!"
  /// (MsgLaliveroInteriorsThereAreSomeWhoDoNot)
  LOCALID_WOMAN_DEFENDING_SHEBA,
  /// A man in a white turban, in the house at
  /// TIMBER_ROOMS_B_FROM_LALIVERO_SOUTHEAST. "Could Faran have tricked us
  /// all?"
  /// ★ Named from the game: "Could Faran have tricked us all?"
  /// (MsgLaliveroInteriorsCouldFaranHaveTrickedUsAll)
  LOCALID_VILLAGER_DOUBTING_FARAN,
  /// A woman in a green dress, in the same house. "If Sheba can't use her
  /// powers in a time like this, they're useless!"
  /// ★ Named from the game: "use her powers in a time like this, they're
  /// useless!" (MsgLaliveroInteriorsIfActorCantUseHerPowers)
  LOCALID_WOMAN_DOUBTING_SHEBA,
  /// An old man with a cane, upstairs in the item shop at
  /// TIMBER_ROOMS_B_FROM_LALIVERO_NORTH. "Sheba came flying to the ruins
  /// like a
  /// shooting star."
  /// ★ Named from the game: "came flying to the ruins like a shooting star."
  /// (MsgLaliveroInteriorsActorCameFlyingToTheRuins)
  LOCALID_OLD_MAN_SHOOTING_STAR,
  /// An old woman, in the same room. "Even Sheba couldn't have escaped six of
  /// them..."
  /// ★ Named from the game: "couldn't have escaped six of them..."
  /// (MsgLaliveroInteriorsEvenActorCouldntHaveEscapedSix)
  LOCALID_OLD_WOMAN_SIX_OF_THEM,
  /// A bald, muscular man, in the house at
  /// TIMBER_ROOMS_B_FROM_LALIVERO_NORTHWEST. "They say a man named Iodem
  /// is
  /// coming from Tolbi. I guess he plans to make us do even more work!"
  /// ★ Named from the game: "I guess he plans to make us do even more work!"
  /// (MsgLaliveroInteriorsTheySayAManNamedIodem)
  LOCALID_WORKER,
  /// A bald, muscular man lying asleep upstairs in that house. "No
  /// one's called me, so maybe I'll take a nap..."; after Venus Lighthouse,
  /// Mind Read hears "I'm worried that we won't get paid now that the
  /// construction has ended."
  /// ★ Named from the game: "No one's called me, so maybe I'll take a nap..."
  /// (MsgLaliveroInteriorsNoOnesCalledMeSoMaybe)
  LOCALID_NAPPING_WORKER,
  /// A strongman, in the house at TIMBER_ROOMS_B_FROM_LALIVERO_NORTHWEST.
  /// "Sheba said the cave at the ruins leads to the southeast."
  /// ★ Named from the game: "said the cave at the ruins leads to the
  /// southeast." (MsgLaliveroInteriorsActorSaidTheCaveAtThe)
  LOCALID_STRONGMAN_RUINS_CAVE,
  /// A strongman who walks about the room upstairs in that house. "The ruins
  /// that form the lighthouse foundation are riddled with caves."
  /// ★ Named from the game: "The ruins that form the lighthouse foundation are
  /// riddled with caves." (MsgLaliveroInteriorsTheRuinsThatFormTheLighthouse)
  LOCALID_STRONGMAN_RUINS_RIDDLE,
  /// ★ Named from what the player sees: the man behind the counter of the
  /// shop hung with swords and shields (its render)
  LOCALID_WEAPON_MERCHANT,
  /// ★ Named from what the player sees: the second man behind the counter of
  /// the shop hung with swords and shields (its render)
  LOCALID_ARMOR_MERCHANT,
  /// ★ Named from what the player sees: the young woman with a ponytail behind
  /// the counter of the shop stacked with barrels and jars (its render)
  LOCALID_ITEM_MERCHANT,
  /// The Sanctum's healer.
  /// ★ Named from what the player sees: the bearded man behind the Sanctum's
  /// counter (its render)
  LOCALID_HEALER,

  // Faran's house and the inn (TimberRoomsC).
  /// Faran's wife, who raised Sheba. "We received Sheba from the sky and
  /// raised her lovingly... I know Faran will rescue that child."
  /// ★ Named from the game: "His wife seems very calm, considering."
  /// (MsgLaliveroInteriorsHisWifeSeemsVeryCalmConsidering)
  LOCALID_FARANS_WIFE = LOCALID_FIRST_PLACED,
  /// Faran's son, a small boy with fair hair, younger than Sheba. "I just know
  /// that Father will save my elder sister, Sheba."
  /// ★ Named from the game: "I just know that Father will save my elder sister"
  /// (MsgLaliveroInteriorsIJustKnowThatFatherWill)
  LOCALID_FARANS_SON,
  /// A young woman with long brown hair, worried for Sheba. "His wife seems
  /// very calm, considering."; Mind Read hears "When someone like Sheba is
  /// kidnapped, it worries us all!"
  /// ★ Named from the game: "Deep down, she must be worried about"
  /// (MsgLaliveroInteriorsHisWifeSeemsVeryCalmConsidering)
  LOCALID_WORRIED_YOUNG_WOMAN,
  /// Placed off the map before Venus Lighthouse.
  /// ★ Named from the game: "Faran followed"
  /// (MsgLaliveroFaranFollowedActorImSureHell)
  LOCALID_FARAN,
  /// Placed off the map before Venus Lighthouse.
  /// ★ Named from the game: "Iodem of Tolbi"
  /// (MsgLaliveroIodemOfTolbiDoesntSeemToo)
  LOCALID_IODEM,
  /// A man with dark hair.
  /// ★ Named from the game: "Welcome to our inn." (MsgInnWelcome)
  LOCALID_INNKEEPER,
  /// A woman in a pink dress and a white apron who works about the inn, too
  /// worried about Sheba to keep her mind on it.
  /// ★ Named from the game: "Working like this keeps my mind occupied."
  /// (MsgLaliveroInteriorsWorkingLikeThisKeepsMyMind)
  LOCALID_INN_WORKER,
  /// A young man with brown hair in a purple shirt who walks about the hall
  /// upstairs in the inn. After Venus Lighthouse: "Fishing is so difficult
  /// nowadays because the ocean is so rough. We have little to cook when we
  /// can't fish."
  /// ★ Named from the game: "We have little to cook when we can't fish."
  /// (MsgLaliveroInteriorsFishingIsSoDifficultNowadaysBecause)
  LOCALID_COOK
};

/// The napping worker's pose, from his sprite's animation table
/// (graphics/sprite/spr_114.tsv): one frame held, lying asleep on his side.
enum { ANIM_WORKER_LIE_ASLEEP = 6 };

/// The map-section temp flag FLAG_MAPSEC_TEMP_00, which lasts until the party
/// enters another map section: the inn worker has apologized. ★ Named from the
/// game: "Oh, I'm sorry..." (MsgLaliveroImSorry)
enum { FLAG_LALIVERO_INN_WORKER_APOLOGIZED = FLAG_MAPSEC_TEMP_00 };

/// TimberRoomsC: the inn worker sighs between chores.
static s32 InnWorker_Sigh(struct Object *interpreter) {
  ObjectEvent_SetAttachedEffect(LOCALID_INN_WORKER, ATTACHED_EFFECT_IN_FRONT |
                                                        ATTACHED_EFFECT_ONCE);
  return 0;
}

const struct MapWarpDest *LaliveroInteriors_GetWarpDests(void) {
  return gLaliveroInteriors_WarpDests;
}

const struct MapWarpRegion *LaliveroInteriors_GetRegions(void) {
  if (gSaveBlock.mapId == (s32)&MapId_TimberRoomsB) {
    return gTimberRoomsB_Regions;
  }
  return gTimberRoomsC_Regions;
}

const u32 *LaliveroInteriors_GetWarps(void) { return gLaliveroInteriors_Warps; }

/// The people of each map; Faran's house fills up after Venus Lighthouse.
const struct ObjectEventTemplate *
LaliveroInteriors_GetObjectEventTemplates(void) {
  if (gSaveBlock.mapId == (s32)&MapId_TimberRoomsC) {
    // Faran's house and the inn.
    if (GameFlag_Get(FLAG_VENUS_LIGHTHOUSE_PARTY_RETURNED) != 0) {
      return gTimberRoomsC_ObjectEventTemplatesAfterLighthouse;
    }
    return gTimberRoomsC_ObjectEventTemplates;
  }
  return gTimberRoomsB_ObjectEventTemplates;
}

/// The side the leader faces: the facing rounded to the nearest of east,
/// south, west and north. Facing north, the leader is across the counter
/// from a shopkeeper.
static inline u16 LaliveroInteriors_LeaderGetSide(void) {
  return (ObjectEvent_GetById(LOCALID_PARTY_LEADER)->facing + 0x2000) & ~0x3fff;
}

/// The weapon merchant sells weapons, or talks of Sheba and the sea. In
/// Lalivero's shops (TimberRoomsB), across the counter, it's the weapon
/// shop; from elsewhere, before Venus Lighthouse, they ask whether the party
/// has heard that Sheba was taken hostage and Faran went after her, and after
/// it tell of the rough sea.
static void EventScript_TalkToWeaponMerchant(s32 keeper) {
  struct ObjectEvent *leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  // LaliveroInteriors_LeaderGetSide's rounding, written with the mask 0xc000,
  // which loads from the literal pool where the helper's builds the mask.
  u16 side = (leader->facing + 0x2000) & 0xc000;

  if (side == FACING_NORTH) {
    Shop_Run(LALIVERO_WEAPON_STOCK, keeper);
  } else if (GameFlag_Get(FLAG_VENUS_LIGHTHOUSE_PARTY_RETURNED)) {
    FieldScript_SetMessageId(MsgLaliveroWentOceanLook);
    FieldScript_ShowMessageAndWait(keeper, 0);
  } else {
    s32 message = MsgLaliveroTakenHostageFaran;

    FieldScript_SetMessageId(message);
    FieldMessage_ShowAtObject(keeper, 0);
    if (FieldScript_AskYesNoWithObjectEventAnimation(LOCALID_PARTY_LEADER, 0) ==
        0) {
      FieldScript_WaitFrames(10);
      FieldScript_SetMessageId(message + 1);
    } else {
      FieldScript_SetMessageId(message + 2);
    }
    FieldScript_ShowMessageAndWait(keeper, 0);
  }
}

/// Lalivero (TimberRoomsB): the armor shop across the counter; otherwise
/// small talk.
static void EventScript_TalkToArmorMerchant(s32 keeper) {
  if (LaliveroInteriors_LeaderGetSide() == FACING_NORTH) {
    Shop_Run(LALIVERO_ARMOR_STOCK, keeper);
  } else if (GameFlag_Get(FLAG_VENUS_LIGHTHOUSE_PARTY_RETURNED)) {
    FieldScript_SetMessageId(MsgLaliveroWantedSendShips);
    FieldScript_ShowMessageAndWait(keeper, 0);
  } else {
    FieldScript_SetMessageId(MsgLaliveroWarriorRightPlease);
    FieldScript_ShowMessageAndWait(keeper, 0);
  }
}

/// Lalivero (TimberRoomsB): the item shop across the counter; otherwise
/// small talk.
static void EventScript_TalkToItemMerchant(s32 keeper) {
  if (LaliveroInteriors_LeaderGetSide() == FACING_NORTH) {
    Shop_Run(LALIVERO_ITEM_STOCK, keeper);
  } else if (GameFlag_Get(FLAG_VENUS_LIGHTHOUSE_PARTY_RETURNED)) {
    FieldScript_SetMessageId(MsgLaliveroImpossibleGoOcean);
    FieldScript_ShowMessageAndWait(keeper, 0);
  } else {
    FieldScript_SetMessageId(MsgLaliveroGuessPeopleAngara);
    FieldScript_ShowMessageAndWait(keeper, 0);
  }
}

/// Lalivero Sanctum: the healer offers the Sanctum's services across the
/// counter; otherwise small talk of Babi's lighthouse.
static void EventScript_TalkToHealer(s32 healer) {
  if (LaliveroInteriors_LeaderGetSide() == FACING_NORTH) {
    Sanctum_Run(healer);
  } else if (GameFlag_Get(FLAG_VENUS_LIGHTHOUSE_PARTY_RETURNED)) {
    FieldScript_SetMessageId(MsgLaliveroBabiLighthouseStruck);
    FieldScript_ShowMessageAndWait(healer, 0);
  } else {
    FieldScript_SetMessageId(MsgLaliveroBabiIsntBuilding);
    FieldScript_ShowMessageAndWait(healer, 0);
  }
}

/// Lalivero (TimberRoomsC), the inn: a room for the night across the
/// counter; otherwise small talk.
static void EventScript_TalkToInnkeeper(s32 innkeeper) {
  if (LaliveroInteriors_LeaderGetSide() == FACING_NORTH) {
    Inn_CheckIn(LALIVERO_INN, innkeeper);
  } else if (GameFlag_Get(FLAG_VENUS_LIGHTHOUSE_PARTY_RETURNED)) {
    FieldScript_SetMessageId(MsgLaliveroDidntKnowTime);
    FieldScript_ShowMessageAndWait(innkeeper, 0);
  } else {
    FieldScript_SetMessageId(MsgLaliveroWantStayLet);
    FieldScript_ShowMessageAndWait(innkeeper, 0);
  }
}

/// Lalivero (TimberRoomsC), before Venus Lighthouse: the inn worker
/// stops her chores to apologize: worry for Sheba keeps her from concentrating.
/// A leader standing west of her steps aside to the south before she starts
/// again.
static void EventScript_TalkToInnWorker(void) {
  s16 side;

  side = LaliveroInteriors_LeaderGetSide();
  GameFlag_Set(FLAG_LALIVERO_INN_WORKER_APOLOGIZED);
  FieldScript_Start();
  Field_EndBusyAndReveal();
  FieldScript_SetMessageId(MsgLaliveroImSorry);
  FieldScript_WaitFrames(50);
  ObjectEvent_ShowEmote(LOCALID_INN_WORKER, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
  ObjectEvent_FaceObjectEvent(LOCALID_INN_WORKER, LOCALID_PARTY_LEADER, 20);
  Script_ShowMessage(LOCALID_INN_WORKER, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_INN_WORKER, ANIM_SHAKE_HEAD);
  FieldScript_WaitFrames(30);
  Script_ShowMessage(LOCALID_INN_WORKER, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_INN_WORKER, 2);
  FieldScript_WaitFrames(30);
  Script_ShowMessage(LOCALID_INN_WORKER, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_INN_WORKER, ANIM_NOD);
  FieldScript_WaitFrames(20);
  Script_ShowMessage(LOCALID_INN_WORKER, 0);
  if ((u16)side == FACING_WEST) {
    ObjectMotion_CommitPositionAndActivate(LOCALID_PARTY_LEADER, 0, 16);
    ObjectMotion_ArmCallback(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
    FieldScript_WaitFrames(20);
  }
  ObjectEvent_SetActionScript(LOCALID_INN_WORKER,
                              gLaliveroInteriors_InnWorkerChores);
  FieldScript_End();
}

/// Lalivero (TimberRoomsC), coord event 51 in Faran's house after the
/// report: the camera finds Iodem, who asks the party to wait for him outside.
static void EventScript_IodemCallsOut(void) {

  GameFlag_Set(FLAG_IODEM_ASKED_TO_MEET);
  FieldScript_Start();
  Field_EndBusyAndReveal();
  FieldScript_WaitFrames(10);
  FieldCamera_MoveWithinMapBounds(PIXELS(120), -1, PIXELS(96), 1);
  FieldCamera_WaitForArrival();
  FieldScript_WaitFrames(30);
  FieldScript_SetMessageId(MsgLaliveroPleaseWaitForMeOutside);
  FieldScript_ShowMessageAndWait(LOCALID_IODEM, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAngleToward(LOCALID_PARTY_LEADER, LOCALID_IODEM, 0);
  FieldScript_WaitFrames(30);
  ObjectMotion_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD);
  FieldScript_WaitFrames(30);
  FieldScript_End();
}

/// Lalivero (TimberRoomsC): Iodem, talked to before he has given the
/// Black Orb, repeats his request.
static void EventScript_TalkToIodem(void) {

  GameFlag_Set(FLAG_IODEM_ASKED_TO_MEET);
  FieldScript_SetMessageId(MsgLaliveroPleaseWaitForMeOutside);
  FieldScript_ShowMessageAndWait(LOCALID_IODEM, 0);
}

/// Both maps, a doorway out: take the touched coord event's warp with the exit
/// sound.
static void EventScript_LeaveBuilding(void) {
  struct FieldWork *work = gFieldWork;
  s16 *trigger = (s16 *)&work->coordEvent;

  Import_ScriptRequestWarp(*trigger);
  Sound_PlaySong(SE_MAP_EXIT);
}

/// What the party can talk to, touch and search in each map, before and
/// after Venus Lighthouse.
const struct MapEvent *LaliveroInteriors_GetEvents(void) {
  if (gSaveBlock.mapId == (s32)&MapId_TimberRoomsC) {
    if (GameFlag_Get(FLAG_VENUS_LIGHTHOUSE_PARTY_RETURNED) != 0) {
      return gTimberRoomsC_EventsAfterLighthouse;
    }
    return gTimberRoomsC_Events;
  }
  if (GameFlag_Get(FLAG_VENUS_LIGHTHOUSE_PARTY_RETURNED) != 0) {
    return gTimberRoomsB_EventsAfterLighthouse;
  }
  return gTimberRoomsB_Events;
}

/// Lalivero (TimberRoomsC),
/// TIMBER_ROOMS_C_FROM_VENUS_LIGHTHOUSE_AERIE back from Venus
/// Lighthouse: the party reports to Faran and Iodem. No sign of Sheba, nor of
/// Jenna and the others; Faran's faith that Sheba lived, what an Adept is, and
/// a farewell as the party sets out to search for Jenna.
static void EventScript_ReportToFaran(void) {
  struct ObjectEvent *leader;

  FieldScript_Start();
  Party_RestoreHpPp(1);
  // The party walks in to Faran and Iodem, and the others step out.
  ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, PIXELS(96), PIXELS(184));
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_FARAN, FACING_EAST, 0);
  ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_WEST, 0);
  FieldScript_SetMessageId(MsgLaliveroHowDidSearchForSheba);
  FieldScript_StartScreenTransition();
  FieldScript_WaitTransitionFrames();
  FieldScript_WaitFrames(10);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, 0x10000, 0x8000);
  ObjectEvent_WalkByAndWait(LOCALID_PARTY_LEADER, 0, -16);
  ObjectMotion_SetPositionAndReset(LOCALID_PARTY_LEADER, 104, 136);
  FieldScript_WaitFrames(10);
  FieldMotion_LaunchFromFocusedObject(LOCALID_GARET, -16, 16, FACING_NORTH);
  FieldMotion_LaunchFromFocusedObject(LOCALID_MIA, 0, 24, FACING_NORTH);
  FieldMotion_LaunchFromFocusedObject(LOCALID_IVAN, 16, 16, FACING_NORTH);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_GARET);
  FieldScript_WaitFrames(20);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_IVAN, 2);
  FieldScript_WaitFrames(20);
  FieldScript_ShowMessageAndWait(LOCALID_IVAN, 0);
  FieldScript_WaitFrames(10);
  // Faran and Iodem tell of the search for Sheba.
  ObjectEvent_FaceDirection(LOCALID_FARAN, FACING_SOUTH, 0);
  ObjectMotion_ArmCallback(LOCALID_IODEM, FACING_SOUTH, 0);
  FieldScript_WaitFrames(30);
  ObjectEvent_ShowEmote(LOCALID_FARAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 70);
  FieldScript_ShowMessageAndWait(LOCALID_FARAN, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_FARAN, ANIM_SHAKE_HEAD);
  FieldScript_WaitFrames(20);
  FieldScript_ShowMessageAndWait(LOCALID_FARAN, 0);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_FARAN, 2);
  FieldScript_WaitFrames(20);
  FieldScript_ShowMessageAndWait(LOCALID_FARAN, 0);
  FieldScript_WaitFrames(20);
  ObjectEvent_ShowEmote(LOCALID_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  FieldScript_ShowMessageAndWait(LOCALID_IODEM, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_IODEM, ANIM_SHAKE_HEAD);
  FieldScript_WaitFrames(20);
  FieldScript_ShowMessageAndWait(LOCALID_IODEM, 0);
  FieldScript_WaitFrames(20);
  FieldScript_WaitFrames(10);
  ObjectEvent_ShowEmote(LOCALID_FARAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 50);
  FieldScript_ShowMessageAndWait(LOCALID_FARAN, 0);
  // The party tells what became of Jenna and the others; the version that saw
  // the ship at Idejima knows Idejima was swept away with it.
  if (GameFlag_Get(FLAG_SAW_SHIP_AT_IDEJIMA) == 0) {
    LaliveroInteriors_TellFaranAboutJenna();
  } else {
    FieldScript_WaitFrames(20);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_FARAN, 2);
    FieldScript_WaitFrames(20);
    FieldScript_ShowMessageAndWait(LOCALID_FARAN, 0);
    FieldScript_WaitFrames(10);
    ObjectEvent_FaceObjectEvent(LOCALID_GARET, LOCALID_PARTY_LEADER, 50);
    ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
                          60);
    FieldScript_WaitFrames(10);
    ObjectMotion_ArmCallback(LOCALID_GARET, FACING_NORTH, 0);
    FieldScript_WaitFrames(20);
    FieldScript_ShowMessageAndWait(LOCALID_GARET, 0);
    FieldScript_WaitFrames(10);
    ObjectMotion_SetAnimationAndWait(LOCALID_MIA, ANIM_SHAKE_HEAD);
    FieldScript_WaitFrames(20);
    FieldScript_ShowMessageAndWait(LOCALID_MIA, 0);
    FieldScript_WaitFrames(10);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_IVAN, 2);
    FieldScript_WaitFrames(20);
    FieldScript_ShowMessageAndWait(LOCALID_IVAN, 0);
    FieldScript_WaitFrames(20);
    ObjectEvent_ShowEmote(LOCALID_IODEM, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
    Script_OpenMessage(LOCALID_IODEM, 0);
    FieldScript_AskYesNoWithObjectEventAnimation(LOCALID_PARTY_LEADER, 0);
    FieldScript_WaitFrames(20);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_IODEM, 2);
    FieldScript_WaitFrames(20);
    FieldScript_ShowMessageAndWait(LOCALID_IODEM, 0);
    FieldScript_WaitFrames(20);
    ObjectMotion_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD);
    FieldScript_WaitFrames(30);
    FieldScript_WaitFrames(10);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 2);
    FieldScript_WaitFrames(20);
    FieldScript_ShowMessageAndWait(LOCALID_GARET, 0);
    FieldScript_WaitFrames(10);
    ObjectMotion_SetAnimationAndWait(LOCALID_MIA, ANIM_SHAKE_HEAD);
    FieldScript_WaitFrames(20);
    FieldScript_ShowMessageAndWait(LOCALID_MIA, 0);
    FieldScript_WaitFrames(10);
    ObjectMotion_SetAnimationAndWait(LOCALID_IVAN, ANIM_NOD);
    FieldScript_WaitFrames(30);
    FieldScript_ShowMessageAndWait(LOCALID_IVAN, 0);
    FieldScript_WaitFrames(10);
    ObjectEvent_FaceObjectEvent(LOCALID_IVAN, LOCALID_PARTY_LEADER, 30);
    Script_OpenMessage(LOCALID_IVAN | SPEAKER_PORTRAIT_RIGHT, 0);
  }
  // Will the party keep searching? Faran answers either way.
  FieldScript_SetMessageId(MsgLaliveroDoNotWorryAboutSheba);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 0);
  ObjectEvent_FaceObjectEvent(LOCALID_GARET, LOCALID_PARTY_LEADER, 20);
  if (FieldScript_AskYesNoWithObjectEventAnimation(LOCALID_PARTY_LEADER, 0) ==
      0) {
    FieldScript_WaitFrames(20);
    ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
    ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTH, 0);
    ObjectMotion_ArmCallback(LOCALID_IVAN, FACING_NORTH, 0);
    FieldScript_WaitFrames(30);
    FieldScript_WaitFrames(10);
    ObjectMotion_SetAnimationAndWait(LOCALID_FARAN, ANIM_NOD);
    FieldScript_WaitFrames(30);
    FieldScript_ShowMessageAndWait(LOCALID_FARAN, 0);
    (gFieldWork->message)++;
  } else {
    FieldScript_WaitFrames(20);
    ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
    ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTH, 0);
    ObjectMotion_ArmCallback(LOCALID_IVAN, FACING_NORTH, 0);
    FieldScript_WaitFrames(30);
    FieldScript_WaitFrames(10);
    ObjectMotion_SetAnimationAndWait(LOCALID_FARAN, ANIM_SHAKE_HEAD);
    FieldScript_WaitFrames(20);
    (gFieldWork->message)++;
    Script_ShowMessage(LOCALID_FARAN, 0);
  }
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_FARAN, 2);
  FieldScript_WaitFrames(20);
  FieldScript_ShowMessageAndWait(LOCALID_FARAN, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_LinkPair(LOCALID_GARET, LOCALID_PARTY_LEADER, 0);
  ObjectEvent_LinkPair(LOCALID_MIA, LOCALID_IVAN, 0);
  FieldScript_WaitFrames(40);
  FieldScript_WaitFrames(10);
  // Faran believes Sheba lived; Iodem explains Adepts.
  ObjectEvent_ShowEmote(LOCALID_FARAN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  FieldScript_ShowMessageAndWait(LOCALID_FARAN, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_MIA, FACING_NORTH, 0);
  FieldScript_WaitFrames(50);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_FARAN, ANIM_NOD);
  FieldScript_WaitFrames(30);
  FieldScript_ShowMessageAndWait(LOCALID_FARAN, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_FARAN, 2);
  FieldScript_WaitFrames(20);
  FieldScript_ShowMessageAndWait(LOCALID_FARAN, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_ShowEmote(LOCALID_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  FieldScript_ShowMessageAndWait(LOCALID_MIA, 0);
  FieldScript_WaitFrames(20);
  ObjectMotion_ArmCallback(LOCALID_FARAN, FACING_EAST, 0);
  FieldScript_WaitFrames(20);
  FieldScript_ShowMessageAndWait(LOCALID_FARAN, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_IODEM, 2);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_WEST, 0);
  FieldScript_WaitFrames(20);
  FieldScript_ShowMessageAndWait(LOCALID_IODEM, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_FARAN, ANIM_SHAKE_HEAD);
  FieldScript_WaitFrames(20);
  FieldScript_ShowMessageAndWait(LOCALID_FARAN, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_IVAN, 2);
  FieldScript_WaitFrames(20);
  FieldScript_ShowMessageAndWait(LOCALID_IVAN, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_FARAN, FACING_SOUTH, 0);
  FieldScript_WaitFrames(30);
  FieldScript_ShowMessageAndWait(LOCALID_FARAN, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_ShowEmote(LOCALID_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
  FieldScript_ShowMessageAndWait(LOCALID_IODEM, 0);
  FieldScript_WaitFrames(20);
  ObjectEvent_ShowEmote(LOCALID_FARAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 65);
  ObjectMotion_ArmCallback(LOCALID_FARAN, FACING_EAST, 0);
  FieldScript_WaitFrames(20);
  FieldScript_ShowMessageAndWait(LOCALID_FARAN, 0);
  FieldScript_WaitFrames(20);
  ObjectEvent_ShowEmote(LOCALID_IODEM, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  FieldScript_ShowMessageAndWait(LOCALID_IODEM, 0);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_SOUTH, 0);
  FieldScript_WaitFrames(40);
  FieldScript_ShowMessageAndWait(LOCALID_IODEM, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_FARAN, FACING_SOUTH, 0);
  FieldScript_WaitFrames(50);
  ObjectEvent_ShowEmote(LOCALID_FARAN, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 60);
  FieldScript_ShowMessageAndWait(LOCALID_FARAN, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_FARAN, ANIM_NOD);
  FieldScript_WaitFrames(30);
  FieldScript_ShowMessageAndWait(LOCALID_FARAN, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVariantCallback(LOCALID_PARTY_LEADER, 2);
  ObjectMotion_SetVariantCallback(LOCALID_GARET, 2);
  ObjectMotion_SetVariantCallback(LOCALID_MIA, 2);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_IVAN, 2);
  FieldScript_WaitFrames(30);
  FieldScript_WaitFrames(10);
  ObjectMotion_ArmCallback(LOCALID_FARAN, FACING_EAST, 0);
  FieldScript_WaitFrames(20);
  FieldScript_ShowMessageAndWait(LOCALID_FARAN, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_IODEM, 2);
  FieldScript_WaitFrames(30);
  ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_WEST, 0);
  FieldScript_WaitFrames(20);
  FieldScript_ShowMessageAndWait(LOCALID_IODEM, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_ShowEmote(LOCALID_FARAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTHEAST, 0);
  FieldScript_WaitFrames(30);
  ObjectMotion_SetAnimationAndWait(LOCALID_GARET, ANIM_SHAKE_HEAD);
  FieldScript_WaitFrames(20);
  FieldScript_ShowMessageAndWait(LOCALID_GARET, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_IODEM, FACING_SOUTH, 0);
  FieldScript_WaitFrames(30);
  ObjectEvent_ShowEmote(LOCALID_IODEM, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  FieldScript_ShowMessageAndWait(LOCALID_IODEM, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 2);
  FieldScript_WaitFrames(20);
  FieldScript_ShowMessageAndWait(LOCALID_GARET, 0);
  FieldScript_WaitFrames(10);
  ObjectEvent_FaceDirection(LOCALID_FARAN, FACING_SOUTH, 0);
  FieldScript_WaitFrames(40);
  ObjectEvent_ShowEmote(LOCALID_FARAN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  FieldScript_ShowMessageAndWait(LOCALID_FARAN, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_ArmCallback(LOCALID_IVAN, FACING_WEST, 0);
  FieldScript_WaitFrames(30);
  ObjectEvent_ShowEmote(LOCALID_IVAN, EMOTE_IN_FRONT | EMOTE_SCOWL, 60);
  ObjectMotion_ArmCallback(LOCALID_IVAN, FACING_NORTH, 0);
  FieldScript_WaitFrames(30);
  ObjectMotion_SetAnimationAndWait(LOCALID_IVAN, ANIM_SHAKE_HEAD);
  FieldScript_WaitFrames(20);
  FieldScript_ShowMessageAndWait(LOCALID_IVAN, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_FARAN, ANIM_SHAKE_HEAD);
  FieldScript_WaitFrames(20);
  FieldScript_ShowMessageAndWait(LOCALID_FARAN, 0);
  FieldScript_WaitFrames(10);
  // Faran sends them off.
  ObjectMotion_SetVariantCallback(LOCALID_PARTY_LEADER, 2);
  ObjectMotion_SetVariantCallback(LOCALID_GARET, 2);
  ObjectMotion_SetVariantCallback(LOCALID_MIA, 2);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_IVAN, 2);
  FieldScript_WaitFrames(30);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_FARAN, ANIM_NOD);
  FieldScript_WaitFrames(30);
  FieldScript_ShowMessageAndWait(LOCALID_FARAN, 0);
  FieldScript_WaitFrames(20);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_NOD);
  Import_ObjectEventSetAnimation(LOCALID_MIA, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(LOCALID_IVAN, ANIM_NOD);
  FieldScript_WaitFrames(30);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_FARAN, 2);
  FieldScript_WaitFrames(20);
  FieldScript_ShowMessageAndWait(LOCALID_FARAN, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 2);
  FieldScript_WaitFrames(20);
  FieldScript_ShowMessageAndWait(LOCALID_GARET, 0);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_FARAN, ANIM_NOD);
  FieldScript_WaitFrames(30);
  FieldScript_ShowMessageAndWait(LOCALID_FARAN, 0);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetAnimationAndWait(LOCALID_MIA, ANIM_NOD);
  FieldScript_WaitFrames(30);
  FieldScript_ShowMessageAndWait(LOCALID_MIA, 0);
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 0);
  ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTHEAST, 0);
  ObjectMotion_ArmCallback(LOCALID_IVAN, FACING_NORTHWEST, 0);
  FieldScript_WaitFrames(30);
  ObjectMotion_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD);
  FieldScript_WaitFrames(10);
  Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_NOD);
  Import_ObjectEventSetAnimation(LOCALID_IVAN, ANIM_NOD);
  ObjectMotion_SetAnimationAndWait(LOCALID_MIA, ANIM_NOD);
  FieldScript_WaitFrames(30);
  // The others walk back into the leader.
  ObjectEvent_SetSpeed(LOCALID_GARET, 0x13333, 0x9999);
  ObjectEvent_SetSpeed(LOCALID_MIA, 0x13333, 0x9999);
  ObjectEvent_SetSpeed(LOCALID_IVAN, 0x13333, 0x9999);
  Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_WALK);
  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (leader != NULL) {
    ObjectMotion_ResetAndSetPosition(LOCALID_GARET, leader->x.part.pixel,
                                     leader->z.part.pixel);
  }
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_GARET);
  ObjectEvent_SetPosition(LOCALID_GARET, 0, 0);
  Import_ObjectEventSetAnimation(LOCALID_MIA, ANIM_WALK);
  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (leader != NULL) {
    ObjectMotion_ResetAndSetPosition(LOCALID_MIA, leader->x.part.pixel,
                                     leader->z.part.pixel);
  }
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_MIA);
  ObjectEvent_SetPosition(LOCALID_MIA, 0, 0);
  Import_ObjectEventSetAnimation(LOCALID_IVAN, ANIM_WALK);
  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (leader != NULL) {
    ObjectMotion_ResetAndSetPosition(LOCALID_IVAN, leader->x.part.pixel,
                                     leader->z.part.pixel);
  }
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_IVAN);
  Import_ObjectEventSetPosition(LOCALID_IVAN, 0, 0);
  FieldScript_WaitFrames(10);
  FieldScript_End();
}

/// The party tells Faran and Iodem what became of Jenna and the others, when
/// it did not see the ship at Idejima: Felix said he sent Jenna ahead to the
/// ship.
static void LaliveroInteriors_TellFaranAboutJenna(void) {
  FieldScript_SetMessageId(MsgLaliveroTellOthers);
  FieldScript_WaitFrames(20);

  ObjectMotion_SetVarCbAndRefresh(LOCALID_FARAN, 2);
  FieldScript_WaitFrames(20);
  FieldScript_ShowMessageAndWait(LOCALID_FARAN, 0);
  FieldScript_WaitFrames(10);

  ObjectMotion_SetAngleToward(LOCALID_GARET, LOCALID_PARTY_LEADER, 50);
  FieldEffect_SpawnLinked(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
                          60);
  FieldScript_WaitFrames(10);
  ObjectMotion_ArmCallback(LOCALID_GARET, FACING_NORTH, 0);
  FieldScript_WaitFrames(20);
  FieldScript_ShowMessageAndWait(LOCALID_GARET, 0);
  FieldScript_WaitFrames(10);

  ObjectMotion_SetAnimationAndWait(LOCALID_MIA, ANIM_SHAKE_HEAD);
  FieldScript_WaitFrames(20);
  FieldScript_ShowMessageAndWait(LOCALID_MIA, 0);
  FieldScript_WaitFrames(10);

  ObjectMotion_SetVarCbAndRefresh(LOCALID_IVAN, 2);
  FieldScript_WaitFrames(20);
  FieldScript_ShowMessageAndWait(LOCALID_IVAN, 0);
  FieldScript_WaitFrames(10);

  ObjectMotion_SetVarCbAndRefresh(LOCALID_IODEM, 2);
  FieldScript_WaitFrames(20);
  FieldScript_ShowMessageAndWait(LOCALID_IODEM, 0);
  FieldScript_WaitFrames(20);

  ObjectMotion_SetVarCbAndRefresh(LOCALID_PARTY_LEADER, 2);
  FieldScript_WaitFrames(20);
  FieldScript_WaitFrames(25);

  ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 2);
  FieldScript_WaitFrames(30);
  FieldScript_ShowMessageAndWait(LOCALID_GARET, 0);
  FieldScript_WaitFrames(30);

  ObjectMotion_SetAnimationAndWait(LOCALID_GARET, ANIM_SHAKE_HEAD);
  FieldScript_WaitFrames(20);
  FieldScript_ShowMessageAndWait(LOCALID_GARET, 0);
  FieldScript_WaitFrames(10);

  ObjectMotion_SetAnimationAndWait(LOCALID_MIA, ANIM_SHAKE_HEAD);
  FieldScript_WaitFrames(20);
  FieldScript_ShowMessageAndWait(LOCALID_MIA, 0);
  FieldScript_WaitFrames(10);

  ObjectMotion_SetAnimationAndWait(LOCALID_IVAN, ANIM_NOD);
  FieldScript_WaitFrames(30);
  FieldScript_ShowMessageAndWait(LOCALID_IVAN, 0);
  FieldScript_WaitFrames(10);

  ObjectMotion_SetAngleToward(LOCALID_IVAN, LOCALID_PARTY_LEADER, 30);
  FieldMessage_ShowAtObject(LOCALID_IVAN | SPEAKER_PORTRAIT_RIGHT, 0);
}

/// Sets both maps up as the party enters: shopkeepers and the innkeeper
/// behind their counters, the Sanctum as the place to wake after a defeat,
/// the inn worker back at her chores, and the report to Faran on coming back
/// from Venus Lighthouse.
s32 Map_Initialize(void) {
  s32 mapId;
  s16 warpDest;
  struct ObjectEvent *objectEvent;

  if (gSaveBlock.warpDest == ENTRANCE_FROM_AERIE) {
    GameFlag_Set(FLAG_VENUS_LIGHTHOUSE_PARTY_RETURNED);
  }
  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  mapId = gSaveBlock.mapId;
  if (mapId == (s32)&MapId_TimberRoomsB) {
    // Each shopkeeper is drawn under the counter's top, and counts as nearer
    // to talk to (OBJ_EVENT_NEAR_TALK) so that the leader reaches it across the
    // counter.
    objectEvent = ObjectEvent_GetById(LOCALID_ITEM_MERCHANT);
    objectEvent->priorityFlags = 0;
    objectEvent->collisionFlags |= OBJ_EVENT_NEAR_TALK;
    objectEvent->sprite->image.priority = 2;
    objectEvent = ObjectEvent_GetById(LOCALID_WEAPON_MERCHANT);
    objectEvent->priorityFlags = 0;
    objectEvent->collisionFlags |= OBJ_EVENT_NEAR_TALK;
    objectEvent->sprite->image.priority = 2;
    objectEvent = ObjectEvent_GetById(LOCALID_ARMOR_MERCHANT);
    objectEvent->collisionFlags |= OBJ_EVENT_NEAR_TALK;
    objectEvent->priorityFlags = 0;
    objectEvent->sprite->image.priority = 2;
    Import_ObjectEventSetAnimation(LOCALID_NAPPING_WORKER,
                                   ANIM_WORKER_LIE_ASLEEP);
    warpDest = gSaveBlock.warpDest;
    if (warpDest == TIMBER_ROOMS_B_SANCTUM) {
      gSaveBlock.savedMap = mapId;
      gSaveBlock.savedWarpDest = warpDest;
    }
  }
  if (gSaveBlock.mapId == (s32)&MapId_TimberRoomsC) {
    objectEvent = ObjectEvent_GetById(LOCALID_INNKEEPER);
    objectEvent->collisionFlags |= OBJ_EVENT_NEAR_TALK;
    objectEvent->priorityFlags = 0;
    objectEvent->sprite->image.priority = 2;
    if (GameFlag_Get(FLAG_LALIVERO_INN_WORKER_APOLOGIZED) != 0) {
      ObjectEvent_SetActionScript(LOCALID_INN_WORKER,
                                  gLaliveroInteriors_InnWorkerChores);
    }
    if (gSaveBlock.warpDest == TIMBER_ROOMS_C_FROM_VENUS_LIGHTHOUSE_AERIE) {
      EventScript_ReportToFaran();
      // Iodem and Faran turn toward the leader, as their later object event
      // templates do.
      ObjectEvent_SetMovementType(
          (struct Object *)ObjectEvent_GetById(LOCALID_IODEM),
          MOVEMENT_TYPE_FACE_LEADER);
      ObjectEvent_SetMovementType(
          (struct Object *)ObjectEvent_GetById(LOCALID_FARAN),
          MOVEMENT_TYPE_FACE_LEADER);
      gSaveBlock.warpDest = TIMBER_ROOMS_C_FROM_LALIVERO_SOUTHWEST;
    }
  }
  return 0;
}

/// The inn worker's chores once she has apologized: she paces between two
/// spots, turning to each side, and sighs now and then.
s32 gLaliveroInteriors_InnWorkerChores[] = {
    SC_SET(SW_COLLISION, OBJ_EVENT_BLOCKED_BY_SOLID | OBJ_EVENT_SOLID),
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

struct MapWarpDest gLaliveroInteriors_WarpDests[] = {
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
    {MAP_TABLE_END},
};

// clang-format off
struct MapWarpRegion gTimberRoomsB_Regions[] = {
    {196, 28, 595, 204, 36, 603, CONDITION_ALWAYS, TIMBER_ROOMS_B_TO_LALIVERO_EAST_SOUTHEAST},
    {MAP_TABLE_END},
};

struct MapWarpRegion gTimberRoomsC_Regions[] = {
    {212, 28, 83, 220, 36, 91, CONDITION_ALWAYS, TIMBER_ROOMS_C_TO_LALIVERO_SOUTH},
    {MAP_TABLE_END},
};

u32 gLaliveroInteriors_Warps[] = {
    MAP_WARPS(TimberRoomsB),
    MAP_WARP(TIMBER_ROOMS_B_TO_LALIVERO_WEST, Lalivero, LALIVERO_FROM_TIMBER_ROOMS_B_WEST),
    MAP_WARP(TIMBER_ROOMS_B_TO_LALIVERO_SOUTHEAST, Lalivero, LALIVERO_FROM_TIMBER_ROOMS_B_SOUTHEAST),
    MAP_WARP(TIMBER_ROOMS_B_ITEM_SHOP_UPSTAIRS_TO_ITEM_SHOP, TimberRoomsB, TIMBER_ROOMS_B_ITEM_SHOP_FROM_ITEM_SHOP_UPSTAIRS),
    MAP_WARP(TIMBER_ROOMS_B_TO_LALIVERO_NORTH, Lalivero, LALIVERO_FROM_TIMBER_ROOMS_B_NORTH),
    MAP_WARP(TIMBER_ROOMS_B_ITEM_SHOP_TO_ITEM_SHOP_UPSTAIRS, TimberRoomsB, TIMBER_ROOMS_B_ITEM_SHOP_UPSTAIRS_FROM_ITEM_SHOP),
    MAP_WARP(TIMBER_ROOMS_B_TO_LALIVERO_NORTHWEST, Lalivero, LALIVERO_FROM_TIMBER_ROOMS_B_NORTHWEST),
    MAP_WARP(TIMBER_ROOMS_B_NORTHWEST_HOUSE_TO_NORTHWEST_HOUSE_UPSTAIRS, TimberRoomsB, TIMBER_ROOMS_B_NORTHWEST_HOUSE_UPSTAIRS_FROM_NORTHWEST_HOUSE),
    MAP_WARP(TIMBER_ROOMS_B_NORTHWEST_HOUSE_UPSTAIRS_TO_NORTHWEST_HOUSE, TimberRoomsB, TIMBER_ROOMS_B_NORTHWEST_HOUSE_FROM_NORTHWEST_HOUSE_UPSTAIRS),
    MAP_WARP(TIMBER_ROOMS_B_TO_LALIVERO_SOUTH, Lalivero, LALIVERO_FROM_TIMBER_ROOMS_B_SOUTH),
    MAP_WARP(TIMBER_ROOMS_B_TO_LALIVERO_EAST_SOUTHEAST, Lalivero, LALIVERO_FROM_TIMBER_ROOMS_B_EAST_SOUTHEAST),
    MAP_WARP(TIMBER_ROOMS_B_TO_LALIVERO_NORTHEAST, Lalivero, LALIVERO_FROM_TIMBER_ROOMS_B_NORTHEAST),
    MAP_WARPS(TimberRoomsC),
    MAP_WARP(TIMBER_ROOMS_C_TO_LALIVERO_SOUTHWEST, Lalivero, LALIVERO_FROM_TIMBER_ROOMS_C_SOUTHWEST),
    MAP_WARP(TIMBER_ROOMS_C_TO_LALIVERO_SOUTH, Lalivero, LALIVERO_FROM_TIMBER_ROOMS_C_SOUTH),
    MAP_WARP(TIMBER_ROOMS_C_TO_LALIVERO_NORTHEAST, Lalivero, LALIVERO_FROM_TIMBER_ROOMS_C_NORTHEAST),
    MAP_WARP(TIMBER_ROOMS_C_INN_TO_INN_UPSTAIRS, TimberRoomsC, TIMBER_ROOMS_C_INN_UPSTAIRS_FROM_INN),
    MAP_WARP(TIMBER_ROOMS_C_INN_UPSTAIRS_TO_INN, TimberRoomsC, TIMBER_ROOMS_C_INN_FROM_INN_UPSTAIRS),
    MAP_WARP(TIMBER_ROOMS_C_INN_UPSTAIRS_TO_GUEST_ROOM_WEST, TimberRoomsC, TIMBER_ROOMS_C_GUEST_ROOM_WEST_FROM_INN_UPSTAIRS),
    MAP_WARP(TIMBER_ROOMS_C_INN_UPSTAIRS_TO_GUEST_ROOM_EAST, TimberRoomsC, TIMBER_ROOMS_C_GUEST_ROOM_EAST_FROM_INN_UPSTAIRS),
    MAP_WARP(TIMBER_ROOMS_C_GUEST_ROOM_WEST_TO_INN_UPSTAIRS, TimberRoomsC, TIMBER_ROOMS_C_INN_UPSTAIRS_FROM_GUEST_ROOM_WEST),
    MAP_WARP(TIMBER_ROOMS_C_GUEST_ROOM_EAST_TO_INN_UPSTAIRS, TimberRoomsC, TIMBER_ROOMS_C_INN_UPSTAIRS_FROM_GUEST_ROOM_EAST),
    MAP_WARP(TIMBER_ROOMS_C_TO_LALIVERO_NORTHWEST, Lalivero, LALIVERO_FROM_TIMBER_ROOMS_C_NORTHWEST),
    MAP_WARP(TIMBER_ROOMS_C_TO_LEMURIAN_SHIP, LemurianShip, LEMURIAN_SHIP_FROM_TIMBER_ROOMS_C),
    MAP_WARPS_END,
};

struct ObjectEventTemplate gTimberRoomsB_ObjectEventTemplates[] = {
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
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gTimberRoomsC_ObjectEventTemplates[] = {
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
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gTimberRoomsC_ObjectEventTemplatesAfterLighthouse[] = {
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
    { LOCALID_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { LOCALID_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { LOCALID_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { MAP_TABLE_END },
};

struct MapEvent gTimberRoomsB_Events[] = {
    { MAP_EVENT_COORD, 1, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveBuilding) },
    { MAP_EVENT_COORD, 2, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveBuilding) },
    { MAP_EVENT_COORD, 3, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveBuilding) },
    { MAP_EVENT_COORD, 4, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveBuilding) },
    { MAP_EVENT_COORD, 5, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveBuilding) },
    { MAP_EVENT_COORD, 6, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveBuilding) },
    { MAP_EVENT_COORD, 7, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveBuilding) },
    { MAP_EVENT_COORD, 8, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveBuilding) },
    { MAP_EVENT_COORD, 9, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveBuilding) },
    { MAP_EVENT_COORD, 11, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveBuilding) },
    { MAP_EVENT_TALK, LOCALID_VILLAGER_SAW_SHEBAS_POWER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsOfCourseIHaveSeenActor) },
    { MAP_EVENT_TALK, LOCALID_WOMAN_DEFENDING_SHEBA, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsThereAreSomeWhoDoNot) },
    { MAP_EVENT_TALK, LOCALID_VILLAGER_DOUBTING_FARAN, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsCouldFaranHaveTrickedUsAll) },
    { MAP_EVENT_TALK, LOCALID_WOMAN_DOUBTING_SHEBA, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIfActorCantUseHerPowers) },
    { MAP_EVENT_TALK, LOCALID_OLD_MAN_SHOOTING_STAR, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsActorCameFlyingToTheRuins) },
    { MAP_EVENT_TALK, LOCALID_OLD_WOMAN_SIX_OF_THEM, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsEvenActorCouldntHaveEscapedSix) },
    { MAP_EVENT_TALK, LOCALID_WORKER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsTheySayAManNamedIodem) },
    { MAP_EVENT_TALK, LOCALID_NAPPING_WORKER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsNoOnesCalledMeSoMaybe) },
    { MAP_EVENT_TALK, LOCALID_STRONGMAN_RUINS_CAVE, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsActorSaidTheCaveAtThe) },
    { MAP_EVENT_TALK, LOCALID_STRONGMAN_RUINS_RIDDLE, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsTheRuinsThatFormTheLighthouse) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_VILLAGER_SAW_SHEBAS_POWER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsActorIfThePowerThatYou) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WOMAN_DEFENDING_SHEBA, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsActorIsTheSaviorOfLalivero) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_VILLAGER_DOUBTING_FARAN, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsALegendarySaviorWillAppearIn) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WOMAN_DOUBTING_SHEBA, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIdLikeToBelieveInActor) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_OLD_MAN_SHOOTING_STAR, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsEvenThoughSheFellOutOf) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_OLD_WOMAN_SIX_OF_THEM, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIfSomethingHappensToActorWhat) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WORKER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWhenIodemGetsHereHesGoing) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_NAPPING_WORKER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsItsBeenSoLongSinceI) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STRONGMAN_RUINS_CAVE, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIWonderHowActorKnewAbout) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STRONGMAN_RUINS_RIDDLE, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsThoseCavesAreTooDangerousFor) },
    { MAP_EVENT_TALK, LOCALID_WEAPON_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToWeaponMerchant) },
    { MAP_EVENT_TALK, LOCALID_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToArmorMerchant) },
    { MAP_EVENT_TALK, LOCALID_ITEM_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToItemMerchant) },
    { MAP_EVENT_TALK, LOCALID_HEALER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToHealer) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WEAPON_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsTheLaliveransAreATimidPeople) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ARMOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsICantRescueActorIfI) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ITEM_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIfAnythingHappensToActorIll) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_HEALER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIBetBabisTowerWillBe) },
    { BG_EVENT(BG_EVENT_BARREL), 100, FLAG_TIMBER_ROOMS_B_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { BG_EVENT(BG_EVENT_BARREL), 101, FLAG_TIMBER_ROOMS_B_ANTIDOTE, FIND_ITEM(ITEM_ANTIDOTE) },
    { BG_EVENT(BG_EVENT_OVEN), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsShesMakingMushroomSoupInLalivero)) },
    { BG_EVENT(BG_EVENT_OVEN), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsItsALaliveroSaladWithPotato)) },
    { BG_EVENT(BG_EVENT_OVEN), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsThePotIsFullOfA)) },
    { MAP_EVENTS_END },
};

struct MapEvent gTimberRoomsB_EventsAfterLighthouse[] = {
    { MAP_EVENT_COORD, 1, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveBuilding) },
    { MAP_EVENT_COORD, 2, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveBuilding) },
    { MAP_EVENT_COORD, 3, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveBuilding) },
    { MAP_EVENT_COORD, 4, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveBuilding) },
    { MAP_EVENT_COORD, 5, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveBuilding) },
    { MAP_EVENT_COORD, 6, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveBuilding) },
    { MAP_EVENT_COORD, 7, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveBuilding) },
    { MAP_EVENT_COORD, 8, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveBuilding) },
    { MAP_EVENT_COORD, 9, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveBuilding) },
    { MAP_EVENT_COORD, 11, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveBuilding) },
    { MAP_EVENT_TALK, LOCALID_VILLAGER_SAW_SHEBAS_POWER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsTheContinentBrokeApartAroundThe) },
    { MAP_EVENT_TALK, LOCALID_WOMAN_DEFENDING_SHEBA, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWhatHappenedToThoseLivingOn) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_VILLAGER_SAW_SHEBAS_POWER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIShudderToThinkWhatWould) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WOMAN_DEFENDING_SHEBA, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWereLuckyThatWeLiveNorth) },
    { MAP_EVENT_TALK, LOCALID_VILLAGER_DOUBTING_FARAN, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsEveryoneSaysThatActorIsOkay) },
    { MAP_EVENT_TALK, LOCALID_WOMAN_DOUBTING_SHEBA, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIRealizedHowThoughtlessIWas) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_VILLAGER_DOUBTING_FARAN, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIfSheIsIndeedTheChild) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WOMAN_DOUBTING_SHEBA, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWeWantToBelieveInSomething) },
    { MAP_EVENT_TALK, LOCALID_OLD_MAN_SHOOTING_STAR, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsActorShouldBeFineSheWas) },
    { MAP_EVENT_TALK, LOCALID_OLD_WOMAN_SIX_OF_THEM, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsActorWillWakeWhenTheBeacon) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_OLD_MAN_SHOOTING_STAR, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsSeeingActorDiveMustHaveBeen) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_OLD_WOMAN_SIX_OF_THEM, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsICantWaitUntilActorIs) },
    { MAP_EVENT_TALK, LOCALID_WORKER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsBabiLighthouseCrumbledWhenAllThe) },
    { MAP_EVENT_TALK, LOCALID_NAPPING_WORKER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIAlwaysWantedABreakBut) },
    { MAP_EVENT_TALK, LOCALID_STRONGMAN_RUINS_CAVE, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsSoTheRuinsReallyAreConnected) },
    { MAP_EVENT_TALK, LOCALID_STRONGMAN_RUINS_RIDDLE, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsYouFinallySolvedTheMysteryOf) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WORKER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWillTheyContinueOnOrQuit) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_NAPPING_WORKER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsImWorriedThatWeWontGet) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STRONGMAN_RUINS_CAVE, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWeCantPossiblyDigAHole) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_STRONGMAN_RUINS_RIDDLE, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsTheMysteryOfTheRuinsShould) },
    { MAP_EVENT_TALK, LOCALID_WEAPON_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToWeaponMerchant) },
    { MAP_EVENT_TALK, LOCALID_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToArmorMerchant) },
    { MAP_EVENT_TALK, LOCALID_ITEM_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToItemMerchant) },
    { MAP_EVENT_TALK, LOCALID_HEALER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToHealer) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WEAPON_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsNoOneCouldPossiblySwimIn) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ARMOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWhenWillTheOceanBeCalm) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ITEM_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsTheChampaArePeopleFromA) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_HEALER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsFirstTheLighthouseWasStruckDown) },
    { BG_EVENT(BG_EVENT_BARREL), 100, FLAG_TIMBER_ROOMS_B_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { BG_EVENT(BG_EVENT_BARREL), 101, FLAG_TIMBER_ROOMS_B_ANTIDOTE, FIND_ITEM(ITEM_ANTIDOTE) },
    { BG_EVENT(BG_EVENT_OVEN), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsShesMakingMushroomSoupInLalivero)) },
    { BG_EVENT(BG_EVENT_OVEN), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsItsALaliveroSaladWithPotato)) },
    { BG_EVENT(BG_EVENT_OVEN), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsThePotIsFullOfA)) },
    { MAP_EVENTS_END },
};

struct MapEvent gTimberRoomsC_Events[] = {
    { MAP_EVENT_COORD, 21, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveBuilding) },
    { MAP_EVENT_COORD, 23, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveBuilding) },
    { MAP_EVENT_COORD, 24, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveBuilding) },
    { MAP_EVENT_COORD, 25, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveBuilding) },
    { MAP_EVENT_COORD, 26, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveBuilding) },
    { MAP_EVENT_COORD, 27, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveBuilding) },
    { MAP_EVENT_COORD, 28, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveBuilding) },
    { MAP_EVENT_COORD, 29, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveBuilding) },
    { MAP_EVENT_COORD, 30, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveBuilding) },
    { MAP_EVENT_COORD, 31, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveBuilding) },
    { MAP_EVENT_TALK, LOCALID_FARANS_WIFE, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWeReceivedActorFromTheSky) },
    { MAP_EVENT_TALK, LOCALID_FARANS_SON, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIJustKnowThatFatherWill) },
    { MAP_EVENT_TALK, LOCALID_WORRIED_YOUNG_WOMAN, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsHisWifeSeemsVeryCalmConsidering) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_FARANS_WIFE, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsItsSoPainfulToEvenThink) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_FARANS_SON, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIJustWishSomeoneWouldPlease) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WORRIED_YOUNG_WOMAN, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsThatCoupleRaisedActorAsThough) },
    { MAP_EVENT_TALK, LOCALID_INN_WORKER, FLAG_LALIVERO_INN_WORKER_APOLOGIZED, EVENT_SCRIPT(EventScript_TalkToInnWorker) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), 0x400 | LOCALID_INN_WORKER, FLAG_LALIVERO_INN_WORKER_APOLOGIZED, EVENT_SCRIPT(EventScript_TalkToInnWorker) },
    { MAP_EVENT_TALK, LOCALID_INNKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToInnkeeper) },
    { MAP_EVENT_TALK, LOCALID_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWorkingLikeThisKeepsMyMind) },
    { MAP_EVENT_TALK, LOCALID_COOK, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWeMustObeyTolbiThatsWhat) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWhyDidThisHaveToHappen) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsHousecleaningChangingSheetsItsAllDone) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_COOK, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsOnceThisIncidentIsOverI) },
    { BG_EVENT(BG_EVENT_BARREL), 102, FLAG_TIMBER_ROOMS_C_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { BG_EVENT(BG_EVENT_OVEN), 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsShesFryingFreshwaterFishFromThe)) },
    { BG_EVENT(BG_EVENT_OVEN), 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsItsRiceMadeWithGrainsFrom)) },
    { BG_EVENT(BG_EVENT_OVEN), 205, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsShesMakingRoastChickenThisScent)) },
    { MAP_EVENTS_END },
};

struct MapEvent gTimberRoomsC_EventsAfterLighthouse[] = {
    { MAP_EVENT_COORD, 21, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveBuilding) },
    { MAP_EVENT_COORD, 23, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveBuilding) },
    { MAP_EVENT_COORD, 24, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveBuilding) },
    { MAP_EVENT_COORD, 25, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveBuilding) },
    { MAP_EVENT_COORD, 26, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveBuilding) },
    { MAP_EVENT_COORD, 27, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveBuilding) },
    { MAP_EVENT_COORD, 28, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveBuilding) },
    { MAP_EVENT_COORD, 29, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveBuilding) },
    { MAP_EVENT_COORD, 30, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveBuilding) },
    { MAP_EVENT_COORD, 31, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_LeaveBuilding) },
    { MAP_EVENT_COORD, 51, FLAG_IODEM_ASKED_TO_MEET, EVENT_SCRIPT(EventScript_IodemCallsOut) },
    { MAP_EVENT_TALK, LOCALID_IODEM, FLAG_IODEM_FAREWELL_SEEN, EVENT_SCRIPT(EventScript_TalkToIodem) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_IODEM, FLAG_IODEM_FAREWELL_SEEN, MSGNO(MsgLaliveroInteriorsIMustExplainTheirPromiseTo) },
    { MAP_EVENT_TALK, LOCALID_FARANS_WIFE, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsFaranSaysKnowingHerPowersActor) },
    { MAP_EVENT_TALK, LOCALID_FARANS_SON, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsYouAreLeavingAfterAllArent) },
    { MAP_EVENT_TALK, LOCALID_WORRIED_YOUNG_WOMAN, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsTheySayTheLighthouseCrumbledDown) },
    { MAP_EVENT_TALK, LOCALID_FARAN, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsYouWillFindJennaSomedayFarewell) },
    { MAP_EVENT_TALK, LOCALID_IODEM, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIsaacThatBoatBelongsToYou) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_FARANS_WIFE, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIDontNeedFameOrFortune) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_FARANS_SON, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWhyWontAnyoneStopIsaacAnd) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WORRIED_YOUNG_WOMAN, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsTolbiIsToBeBlamedFor) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_FARAN, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsSoThereAreSomePeopleLike) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_IODEM, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsPleaseUseThatShipToFind) },
    { MAP_EVENT_TALK, LOCALID_INNKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToInnkeeper) },
    { MAP_EVENT_TALK, LOCALID_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsIMustSayItIsNice) },
    { MAP_EVENT_TALK, LOCALID_COOK, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsFishingIsSoDifficultNowadaysBecause) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsWhyWouldTheLightingMakeSuch) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsImSleepyTheseBrightNightsHave) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_COOK, CONDITION_ALWAYS, MSGNO(MsgLaliveroInteriorsGeeWhatAmIGoingTo) },
    { BG_EVENT(BG_EVENT_BARREL), 102, FLAG_TIMBER_ROOMS_C_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { BG_EVENT(BG_EVENT_OVEN), 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsShesFryingFreshwaterFishFromThe)) },
    { BG_EVENT(BG_EVENT_OVEN), 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsItsRiceMadeWithGrainsFrom)) },
    { BG_EVENT(BG_EVENT_OVEN), 205, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLaliveroInteriorsShesMakingRoastChickenThisScent)) },
    { MAP_EVENTS_END },
};
// clang-format on
