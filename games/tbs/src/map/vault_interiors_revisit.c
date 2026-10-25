/// @file
/// Vault (MapId_VaultInteriorsRevisit): the town's houses, shops and inn once
/// the party has reached Kalay. They replace the first visit's interiors
/// (vault_interiors.c) as MapId_VaultRevisit (vault_revisit.c) replaces the
/// town: each of its six house doors leads to the warp destination here named
/// for the same door, and each room leads back out through it.
///
/// The thieves have broken out of jail, and the talk is of them and of Colosso,
/// the festival about to begin in Tolbi. The rooms, by warp destination:
/// - VAULT_INTERIORS_REVISIT_FROM_VAULT_REVISIT_SOUTHEAST, the gem owner's
///   house: he says a cat ate his precious stone, and his wife does not believe
///   him. Arriving here sets him on the floor and takes the stone off his
///   table: the 4-by-3 block of cells copied from (0, 120) to (8, 67) lies
///   under the window of the map grid's layer 2, at the table, and draws it
///   bare where the first visit's house (vault_interiors.c) draws the sparkling
///   stone on it (render probe vault-interiors-revisit, the room before and
///   after).
/// - VAULT_INTERIORS_REVISIT_FROM_VAULT_REVISIT_SOUTH, the house of a boy who
///   doubts the thieves were from Lunpa.
/// - VAULT_INTERIORS_REVISIT_FROM_VAULT_REVISIT_SOUTHWEST and
///   VAULT_INTERIORS_REVISIT_INN_FROM_INN_UPSTAIRS, the inn, where the girl at
///   the inn offers Bones and a Bone lies on the floor beside her; its stairs
///   lead to the upstairs room, where a cook works.
/// - VAULT_INTERIORS_REVISIT_FROM_VAULT_REVISIT_EAST, the item shop.
/// - VAULT_INTERIORS_REVISIT_FROM_VAULT_REVISIT_NORTH, the weapon and armor
///   shops.
/// - VAULT_INTERIORS_REVISIT_FROM_VAULT_REVISIT_NORTHWEST, the mayor's house,
///   where the mayor welcomes Isaac back and warns that the thieves have
///   escaped (FLAG_VAULT_REVISIT_ISAAC_WELCOMED_BACK, a map section flag, and
///   FLAG_VAULT_WARNED_OF_ESCAPED_THIEVES); a Mind Read on him before that
///   plays the welcome first.
///
/// The Bones are how the party gets the dog in Vault to show its hidden
/// passage. The girl's offer gives one Bone each time it is accepted
/// (FLAG_VAULT_BONE_RECEIVED, shared with vault_interiors.c, changes only her
/// words), and searching the floor Bone (MB_VAULT_INTERIORS_INN_BARREL,
/// Vault_FindBone) gives another. While the leader stands beside the floor
/// Bone, Vault_UpdateBoneProximityFlag sets FLAG_VAULT_LEADER_BESIDE_BONE,
/// which disables her talk row, so that the A button there searches instead of
/// talking. The shopkeepers open their shops only when spoken to across the
/// counter. A jar, two barrels and a wooden box keep the first visit's flags,
/// so what the party took then stays taken, and two bookcases can be read.

#include "save_block.h"
#include "unit/map/vault_interiors_revisit.h"
#include "vault_interiors.h"
#include "unit/map/common/vault_itemobj.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/field_services.h"
#include "unit/game/game.h"
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
#include "engine_call.h"
#include "map_tables.h"
#include "inventory.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "flags.h"
#include "heap.h"

/// The townspeople, numbering the object event template rows from
/// LOCALID_FIRST_PLACED and named by what they say on the party's return.
enum {
  /// A young man whose precious stone, he says, a cat ate.
  /// ★ Named from the game: "I kept worrying that they would take my gem."
  /// (MsgVaultICanSleepWellNowThat)
  LOCALID_GEM_OWNER = LOCALID_FIRST_PLACED,
  /// A woman with fair hair.
  /// ★ Named from the game: "My husband says my cat ate the treasure"
  /// (MsgVaultMyHusbandSaysMyCatAte)
  LOCALID_GEM_OWNERS_WIFE,
  /// A burly grey-haired man in the boy's house.
  /// ★ Named from the game: "The prisoners said they wanted to break out of
  /// jail before Colosso began." (MsgVaultThePrisonersSaidTheyWantedTo)
  LOCALID_VILLAGER_HEARD_PRISONERS,
  /// A small boy.
  /// ★ Named from the game: "Daddy hasn't been paying attention"
  /// (MsgVaultDaddyHasntBeenPayingAttentionSince)
  LOCALID_BOY,
  /// An old man with a cane who would like to see Colosso once more.
  /// ★ Named from the game: "Whenever my wife used to go to Tolbi for Colosso"
  /// (MsgVaultWheneverMyWifeUsedToGo)
  LOCALID_COLOSSO_HUSBAND,
  /// An old woman.
  /// ★ Named from the game: "Lately, my husband's been going on and on about
  /// Colosso." (MsgVaultLatelyMyHusbandsBeenGoingOn)
  LOCALID_COLOSSO_WIFE,
  /// A small boy in the mayor's house.
  /// ★ Named from the game: "Those thieves! You go to all that trouble catching
  /// them, and then we let them get away." (MsgVaultThoseThievesYouGoToAll)
  LOCALID_BOY_ON_THE_THIEVES_ESCAPE,
  /// A little girl in the mayor's house.
  /// ★ Named from the game: "I hear that a big festival is about to start in
  /// some faraway town." (MsgVaultIHearThatABigFestival)
  LOCALID_GIRL_CURIOUS_ABOUT_THE_FESTIVAL,
  /// The mayor, an old man with white hair.
  /// ★ Named from the game: "My husband, the mayor"
  /// (MsgVaultMyHusbandTheMayorCantBelieve)
  LOCALID_MAYOR,
  /// A woman in a green dress.
  /// ★ Named from the game: "My husband, the mayor, can't believe anyone from
  /// Vault could be the thief." (MsgVaultMyHusbandTheMayorCantBelieve)
  LOCALID_MAYORS_WIFE,
  /// The girl at the inn, as the Bone's description calls her, a young woman
  /// with fair hair.
  /// ★ Named from the game: "A bone from the girl at the inn"
  /// (MsgItemPlainName)
  LOCALID_GIRL_AT_THE_INN,
  /// ★ Named from the game: "I'm sorry I've only got these cheap weapons to
  /// sell right now." (MsgVaultAllOurBestWeaponsWereStolen)
  LOCALID_VAULT_WEAPON_MERCHANT,
  /// ★ Named from the game: "I wonder if they left us alone because we don't
  /// carry any valuable armor?" (MsgVaultIWonderIfTheyLeftUs)
  LOCALID_VAULT_ARMOR_MERCHANT,
  /// ★ Named from the game: "Why keep the store open if we don't have any
  /// customers?" (MsgVaultWhyKeepTheStoreOpenIf)
  LOCALID_VAULT_ITEM_MERCHANT,
  /// A man with dark hair behind the inn's counter.
  /// ★ Named from the game: "Won't anyone come stay the night here?"
  /// (MsgVaultWontAnyoneComeStayTheNight)
  LOCALID_VAULT_INNKEEPER,
  /// A woman in an apron, in the inn's other room.
  /// ★ Named from the game: "I did some cooking, so now I've got all these
  /// useless bones." (MsgVaultDidSomeCookingNowIve)
  LOCALID_COOK
};

/// The stock and inn numbers the counters open.
enum {
  VAULT_WEAPON_STOCK = 4,
  VAULT_ARMOR_STOCK = 5,
  VAULT_ITEM_STOCK = 6,
  VAULT_INN = 1
};

/// Set once the mayor has told Isaac the thieves escaped from jail.
/// ★ Named from the game: "You've come back to Vault!"
/// (MsgVaultIsaacYouveComeBackVault)
enum { FLAG_VAULT_REVISIT_ISAAC_WELCOMED_BACK = FLAG_MAPSEC_TEMP_00 };

// Shared map routines stay in their linked order.
#include "common/vault_spotflag.h"

const struct MapWarpDest *Map_GetWarpDests(void) {
  return gVaultInteriorsRevisit_WarpDests;
}

const struct MapWarpRegion *Map_GetWarpRegions(void) { return NULL; }

const u32 *Map_GetWarps(void) { return gVaultInteriorsRevisit_Warps; }

/// Gives the object event templates' condition 0 whether each lies inside the
/// view bounds of the room entered (Map_AssignViewFlags), so only that
/// room's people appear.
const struct ObjectEventTemplate *Map_GetObjectEventTemplates(void) {
  struct ObjectEventTemplate *templates =
      gVaultInteriorsRevisit_ObjectEventTemplates;

  Map_AssignViewFlags(templates);
  return templates;
}

/// The object event stands still and says the selected message.
static void VaultInteriorsRevisit_ShowStandingObjectEventDialogue(s32 localId) {
  FieldScript_Start();
  Import_ObjectEventSetAnimation(localId, ANIM_STAND);
  FieldScript_ShowMessageAndWait(localId, 0);
  FieldScript_End();
}

/// Faces the leader and shows this resident's dialogue while standing still.
static inline void VaultInteriorsRevisit_FaceAndSpeak(s32 localId,
                                                      s32 message) {
  FieldScript_SetMessageId(message);
  ObjectEvent_LinkPair(localId, LOCALID_PARTY_LEADER, 2);
  VaultInteriorsRevisit_ShowStandingObjectEventDialogue(localId);
}

/// The wife thought she saw the cat paw the gemstone on the table.
/// Then the treasure was gone. She's in the house at
/// VAULT_INTERIORS_REVISIT_FROM_VAULT_REVISIT_SOUTHEAST.
static void EventScript_WifeSawCatTakeGem(void) {
  VaultInteriorsRevisit_FaceAndSpeak(LOCALID_GEM_OWNERS_WIFE,
                                     MsgVaultThoughtSawCatGetUp);
}

/// The boy doubts the thieves were from Lunpa after all.
/// He points out Dodonpa's henchmen never attacked. He's in the house at
/// VAULT_INTERIORS_REVISIT_FROM_VAULT_REVISIT_SOUTH.
static void EventScript_BoyDoubtsThievesWereFromLunpa(void) {
  VaultInteriorsRevisit_FaceAndSpeak(LOCALID_BOY,
                                     MsgVaultDodonpasHenchmenNeverAttackedUs);
}

/// In the item shop, the husband would like to see Colosso in Tolbi once more
/// before he dies.
static void EventScript_HusbandLongsForColosso(void) {
  VaultInteriorsRevisit_FaceAndSpeak(LOCALID_COLOSSO_HUSBAND,
                                     MsgVaultItsNearTimeForColosso);
}

/// In his house, the mayor welcomes Isaac back: the thieves he caught have
/// escaped from jail, and Vault feared they would come after him. Asked whether
/// the party has run into them, a yes and a no get different warnings. Sets
/// FLAG_VAULT_REVISIT_ISAAC_WELCOMED_BACK, after which the mayor only warns of
/// their revenge, and FLAG_VAULT_WARNED_OF_ESCAPED_THIEVES.
static void EventScript_WelcomeIsaacBack(void) {
  FieldScript_Start();
  FieldScript_SetMessageId(MsgVaultIsaacYouveComeBackVault);
  ObjectEvent_LinkPair(LOCALID_MAYOR, LOCALID_PARTY_LEADER, 2);
  Import_ObjectEventSetAnimation(LOCALID_MAYOR, ANIM_STAND);
  Script_ShowMessageAndWait(LOCALID_MAYOR, 0, 20);
  ObjectMotion_SetAnimationAndWait(LOCALID_MAYOR, ANIM_SHAKE_HEAD);
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(LOCALID_MAYOR, 0, 20);
  ObjectEvent_ShowEmote(LOCALID_MAYOR, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Script_ShowMessageAndWait(LOCALID_MAYOR, 0, 30);
  // The negative answer advances to the next message before continuing.
  Script_OpenMessage(LOCALID_MAYOR, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) != 0) {
    gFieldWork->message++;
  }
  Script_ShowMessageAndWait(LOCALID_MAYOR, 0, 20);
  GameFlag_Set(FLAG_VAULT_REVISIT_ISAAC_WELCOMED_BACK);
  GameFlag_Set(FLAG_VAULT_WARNED_OF_ESCAPED_THIEVES);
  FieldScript_End();
}

/// After the welcome: those thieves really wanted revenge, so take care.
static void EventScript_WarnOfThievesRevenge(void) {
  VaultInteriorsRevisit_FaceAndSpeak(LOCALID_MAYOR,
                                     MsgVaultThoseGuysReallyWantedRevenge);
}

/// Unused: the cook in the inn's other room offers the party her leftover
/// bones, but gives none.
///
/// She would have said `I did some cooking, so now I've got all these useless
/// bones.` and asked `...You want some?`. His earlier talk row,
/// MsgVaultWeDontGetALotOf with no condition, always answers first
/// (Map_FindEvent takes the first match), so the player only ever hears that
/// travelers are few in winter.
static void EventScript_UnusedOfferLeftoverBones(void) {
  VaultInteriorsRevisit_FaceAndSpeak(LOCALID_COOK,
                                     MsgVaultDidSomeCookingNowIve);
}

/// The girl at the inn offers leftover bones from dinner, or more of them once
/// one was taken (FLAG_VAULT_BONE_RECEIVED); a yes gets a Bone if there is
/// room, a no a nod. Her talk row needs FLAG_VAULT_LEADER_BESIDE_BONE clear,
/// that is, the leader not standing beside the floor Bone.
static void EventScript_OfferBones(void) {
  FieldScript_Start();
  ObjectEvent_LinkPair(LOCALID_GIRL_AT_THE_INN, LOCALID_PARTY_LEADER, 0);
  if (GameFlag_Get(FLAG_VAULT_BONE_RECEIVED) == 0) {
    FieldScript_SetMessageId(MsgVaultHaveLotLeftoverBonesFrom);
    Script_OpenMessage(LOCALID_GIRL_AT_THE_INN, 0);
  } else {
    FieldScript_SetMessageId(MsgVaultWantMoreBones);
    Script_OpenMessage(LOCALID_GIRL_AT_THE_INN, 0);
  }
  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
    // Show the offer before checking whether the party can carry the gift.
    FieldScript_WaitFrames(20);
    Script_ShowMessage(LOCALID_GIRL_AT_THE_INN, 0);
    FieldScript_WaitFrames(20);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_GIRL_AT_THE_INN, 2);
    FieldScript_WaitFrames(20);
    // Give the Bone only after the offer is accepted and space is available.
    if (PartyInventory_HasSpace() == 0) {
      ObjectMotion_SetAnimationAndWait(LOCALID_GIRL_AT_THE_INN,
                                       ANIM_SHAKE_HEAD);
      FieldScript_WaitFrames(20);
      FieldScript_SetMessageId(MsgVaultWowHaveManyThingsArent);
      Script_ShowMessage(LOCALID_GIRL_AT_THE_INN, 0);
    } else {
      ItemDisplay_ShowItem(ITEM_BONE, 3);
      PartyInventory_GiveItem(ITEM_BONE, 0);
      GameFlag_Set(FLAG_VAULT_BONE_RECEIVED);
    }
  } else {
    // Declining selects the following line and the object event's response
    // pose.
    gFieldWork->message++;
    FieldScript_WaitFrames(20);
    ObjectMotion_SetAnimationAndWait(LOCALID_GIRL_AT_THE_INN, ANIM_NOD);
    FieldScript_WaitFrames(20);
    Script_ShowMessage(LOCALID_GIRL_AT_THE_INN, 0);
  }
  ObjectEvent_FaceDirection(LOCALID_GIRL_AT_THE_INN, FACING_SOUTH, 0);
  FieldScript_End();
}

/// Mind Read on the mayor before the welcome: he notices the party and
/// welcomes Isaac back.
static void EventScript_ReadMindAndWelcomeIsaacBack(void) {
  u8 *paused;
  u8 unpaused = 0;

  FieldScript_Start();
  ObjectMotion_SetVarCbAndRefresh(LOCALID_MAYOR, 1);
  FieldScript_End();
  // Keep the object event paused through the conversation, then resume
  // action 2.
  paused = &ObjectEvent_GetById(LOCALID_MAYOR)->paused;
  *paused = 1;
  EventScript_WelcomeIsaacBack();
  paused = &ObjectEvent_GetById(LOCALID_MAYOR)->paused;
  *paused = unpaused;
  ObjectEvent_SetBuiltinAction(LOCALID_MAYOR, 2);
}

// Shared Bone-item dialogue follows the object event conversations.
#include "common/vault_bone.h"

/// Across the counter the weapon shop opens; from elsewhere the merchant talks
/// of a river of fire on the other side of Mt. Aleph.
static void EventScript_TalkToWeaponMerchant(void) {
  u32 leaderFacing;

  leaderFacing = ObjectEvent_GetById(LOCALID_PARTY_LEADER)->facing;
  FieldScript_Start();

  if (FACING_IS_NORTH(leaderFacing)) {
    Shop_Run(VAULT_WEAPON_STOCK, LOCALID_VAULT_WEAPON_MERCHANT);
  } else {
    FieldScript_SetMessageId(MsgVaultTheresRiverFireOnOther);
    FieldScript_ShowMessageAndWait(LOCALID_VAULT_WEAPON_MERCHANT, 0);
  }

  FieldScript_End();
}

/// Across the counter the armor shop opens; from elsewhere the merchant says
/// the eruption split the continent in two.
static void EventScript_TalkToArmorMerchant(void) {
  u32 leaderFacing;

  leaderFacing = ObjectEvent_GetById(LOCALID_PARTY_LEADER)->facing;
  FieldScript_Start();

  if (FACING_IS_NORTH(leaderFacing)) {
    Shop_Run(VAULT_ARMOR_STOCK, LOCALID_VAULT_ARMOR_MERCHANT);
  } else {
    FieldScript_SetMessageId(MsgVaultTheySayVolcanoOnMt);
    FieldScript_ShowMessageAndWait(LOCALID_VAULT_ARMOR_MERCHANT, 0);
  }

  FieldScript_End();
}

/// Across the counter the item shop opens; from elsewhere the merchant has had
/// few customers lately.
static void EventScript_TalkToItemMerchant(void) {
  u32 leaderFacing;

  leaderFacing = ObjectEvent_GetById(LOCALID_PARTY_LEADER)->facing;
  FieldScript_Start();

  if (FACING_IS_NORTH(leaderFacing)) {
    Shop_Run(VAULT_ITEM_STOCK, LOCALID_VAULT_ITEM_MERCHANT);
  } else {
    FieldScript_SetMessageId(MsgVaultWeHaventBeenGettingMany);
    FieldScript_ShowMessageAndWait(LOCALID_VAULT_ITEM_MERCHANT, 0);
  }

  FieldScript_End();
}

/// Across the counter the inn opens; from elsewhere the innkeeper complains
/// that no one comes, what with the volcano.
static void EventScript_TalkToInnkeeper(void) {
  u32 leaderFacing;

  leaderFacing = ObjectEvent_GetById(LOCALID_PARTY_LEADER)->facing;
  FieldScript_Start();

  if (FACING_IS_NORTH(leaderFacing)) {
    Inn_CheckIn(VAULT_INN, LOCALID_VAULT_INNKEEPER);
  } else {
    FieldScript_SetMessageId(MsgVaultMmmmNothingDoNothingDo);
    FieldScript_ShowMessageAndWait(LOCALID_VAULT_INNKEEPER, 0);
  }

  FieldScript_End();
}

/// Mind Read on the girl at the inn: hungry while cooking, or, once a Bone was
/// given, wondering whether "he" has a calcium deficiency.
static void EventScript_ReadBoneGiverMind(void) {
  FieldScript_Start();
  if (GameFlag_Get(FLAG_VAULT_BONE_RECEIVED) == 0) {
    FieldScript_SetMessageId(MsgVaultGeeAlwaysGetHungryWhen);
  } else {
    FieldScript_SetMessageId(MsgVaultHeReallyLikesBonesWonder);
  }
  FieldScript_ShowMessageAndWait(LOCALID_GIRL_AT_THE_INN, 0);
  FieldScript_End();
}

/// Returns the talk, Psynergy, bg event and warp table.
const struct MapEvent *Map_GetEvents(void) {
  return gVaultInteriorsRevisit_Events;
}

/// Opens the room through a window transition. Entering the house at
/// VAULT_INTERIORS_REVISIT_FROM_VAULT_REVISIT_SOUTHEAST sets its owner on the
/// floor and takes the stone off his table; entering the inn
/// (VAULT_INTERIORS_REVISIT_FROM_VAULT_REVISIT_SOUTHWEST, or from upstairs by
/// VAULT_INTERIORS_REVISIT_INN_FROM_INN_UPSTAIRS)
/// places the floor Bone beside the girl and starts
/// Vault_UpdateBoneProximityFlag.
s32 Map_Initialize(void) {
  s32 warpDest;
  s32 clearedState;

  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  warpDest = gSaveBlock.warpDest;

  if (warpDest == VAULT_INTERIORS_REVISIT_FROM_VAULT_REVISIT_SOUTHEAST) {
    // Draw the table bare, its stone gone, and set the gem owner back on the
    // floor.
    MapGrid_CopyCellsTo(0, 120, 8, 67, 4, 3);
    clearedState = 0;
    ObjectEvent_GetById(LOCALID_GEM_OWNER)->motionFlags = clearedState;
    ObjectEvent_GetById(LOCALID_GEM_OWNER)->y.fixed = clearedState;
    ObjectEvent_GetById(LOCALID_GEM_OWNER)->groundHeight = clearedState;
  } else if (warpDest == VAULT_INTERIORS_REVISIT_FROM_VAULT_REVISIT_SOUTHWEST ||
             warpDest == VAULT_INTERIORS_REVISIT_INN_FROM_INN_UPSTAIRS) {
    // Show the Bone lying in the room, and track whether the leader stands
    // beside it (FLAG_VAULT_LEADER_BESIDE_BONE).
    Vault_PlaceItemIcon(ITEM_BONE, PIXELS(568), PIXELS(16), PIXELS(672));
    FieldScheduler_AddCallback(Vault_UpdateBoneProximityFlag,
                               TASK_PRIORITY_MAP);
  }

  return 0;
}

// Shared item-object routines follow the map initializer.
#include "common/vault_itemobj.h"

struct MapWarpDest gVaultInteriorsRevisit_WarpDests[] = {
    {WARP_DEST_DEFAULT, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {VAULT_INTERIORS_REVISIT_FROM_VAULT_REVISIT_SOUTHEAST, CONDITION_ALWAYS,
     192, 0, 220, FACING_NORTH, 0, 32, 0, 272, 240, 0},
    {VAULT_INTERIORS_REVISIT_FROM_VAULT_REVISIT_SOUTH, CONDITION_ALWAYS, 432, 0,
     254, FACING_NORTH, 0, 288, 0, 528, 272, 0},
    {VAULT_INTERIORS_REVISIT_FROM_VAULT_REVISIT_SOUTHWEST, CONDITION_ALWAYS,
     720, 0, 782, FACING_NORTH, 0, 432, 576, 800, 800, 0},
    {VAULT_INTERIORS_REVISIT_FROM_VAULT_REVISIT_EAST, CONDITION_ALWAYS, 624, 0,
     236, FACING_NORTH, 0, 560, 32, 848, 256, 0},
    {VAULT_INTERIORS_REVISIT_FROM_VAULT_REVISIT_NORTH, CONDITION_ALWAYS, 144, 0,
     492, FACING_NORTH, 0, 16, 288, 256, 512, 0},
    {VAULT_INTERIORS_REVISIT_FROM_VAULT_REVISIT_NORTHWEST, CONDITION_ALWAYS,
     416, 0, 526, FACING_NORTH, 0, 320, 288, 592, 544, 0},
    {VAULT_INTERIORS_REVISIT_INN_FROM_INN_UPSTAIRS, CONDITION_ALWAYS, 744, 0,
     640, FACING_SOUTH, 0, 432, 576, 800, 800, 0},
    {VAULT_INTERIORS_REVISIT_INN_UPSTAIRS_FROM_INN, CONDITION_ALWAYS, 358, 0,
     632, FACING_SOUTH, 0, 32, 576, 400, 776, 0},
    {MAP_TABLE_END},
};

u32 gVaultInteriorsRevisit_Warps[] = {
    MAP_WARPS(VaultInteriorsRevisit),
    MAP_WARP(VAULT_INTERIORS_REVISIT_TO_VAULT_REVISIT_SOUTHEAST, VaultRevisit,
             VAULT_REVISIT_FROM_VAULT_INTERIORS_REVISIT_SOUTHEAST),
    MAP_WARP(VAULT_INTERIORS_REVISIT_TO_VAULT_REVISIT_SOUTH, VaultRevisit,
             VAULT_REVISIT_FROM_VAULT_INTERIORS_REVISIT_SOUTH),
    MAP_WARP(VAULT_INTERIORS_REVISIT_TO_VAULT_REVISIT_SOUTHWEST, VaultRevisit,
             VAULT_REVISIT_FROM_VAULT_INTERIORS_REVISIT_SOUTHWEST),
    MAP_WARP(VAULT_INTERIORS_REVISIT_TO_VAULT_REVISIT_EAST, VaultRevisit,
             VAULT_REVISIT_FROM_VAULT_INTERIORS_REVISIT_EAST),
    MAP_WARP(VAULT_INTERIORS_REVISIT_TO_VAULT_REVISIT_NORTH, VaultRevisit,
             VAULT_REVISIT_FROM_VAULT_INTERIORS_REVISIT_NORTH),
    MAP_WARP(VAULT_INTERIORS_REVISIT_TO_VAULT_REVISIT_NORTHWEST, VaultRevisit,
             VAULT_REVISIT_FROM_VAULT_INTERIORS_REVISIT_NORTHWEST),
    MAP_WARP(VAULT_INTERIORS_REVISIT_INN_UPSTAIRS_TO_INN, VaultInteriorsRevisit,
             VAULT_INTERIORS_REVISIT_INN_FROM_INN_UPSTAIRS),
    MAP_WARP(VAULT_INTERIORS_REVISIT_INN_TO_INN_UPSTAIRS, VaultInteriorsRevisit,
             VAULT_INTERIORS_REVISIT_INN_UPSTAIRS_FROM_INN),
    MAP_WARPS_END,
};

// clang-format off
struct ObjectEventTemplate gVaultInteriorsRevisit_ObjectEventTemplates[] = {
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, 0, MOVEMENT_TYPE_STAND,
        PIXELS(160), 0, PIXELS(136), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, 0, 3,
        PIXELS(152), 0, PIXELS(72), FACING_WEST, TALK_FACING_PARTY, 0 },
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, 0, MOVEMENT_TYPE_STAND,
        PIXELS(456), 0, PIXELS(184), FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_BOY_WITH_FAIR_HAIR, 0, MOVEMENT_TYPE_WANDER,
        PIXELS(368), 0, PIXELS(120), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, 0, MOVEMENT_TYPE_WANDER,
        PIXELS(744), 0, PIXELS(200), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_WOMAN, 0, MOVEMENT_TYPE_STAND,
        PIXELS(752), 0, PIXELS(136), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_BOY_WITH_FAIR_HAIR, 0, MOVEMENT_TYPE_STAND,
        PIXELS(496), 0, PIXELS(416), FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_GIRL_WITH_PIGTAILS, 0, MOVEMENT_TYPE_STAND,
        PIXELS(512), 0, PIXELS(416), FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_OLD_MAN_WITH_WHITE_HAIR, 0, MOVEMENT_TYPE_STAND,
        PIXELS(536), 0, PIXELS(451), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, 0, MOVEMENT_TYPE_STAND,
        PIXELS(416), 0, PIXELS(464), FACING_NORTH, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, 0, MOVEMENT_TYPE_STAND,
        PIXELS(568), 0, PIXELS(648), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_BALD_MAN_IN_RED, 0, MOVEMENT_TYPE_STAND,
        PIXELS(200), 0, PIXELS(392), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_IN_GREEN, 0, MOVEMENT_TYPE_STAND,
        PIXELS(88), 0, PIXELS(392), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_WITH_PONYTAIL, 0, MOVEMENT_TYPE_STAND,
        PIXELS(632), 0, PIXELS(136), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_MAN_WITH_DARK_HAIR, 0, MOVEMENT_TYPE_STAND,
        PIXELS(696), 0, PIXELS(696), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_IN_PINK_APRON, 0, MOVEMENT_TYPE_WANDER,
        PIXELS(248), 0, PIXELS(712), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { MAP_TABLE_END },
};

struct MapEvent gVaultInteriorsRevisit_Events[] = {
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, VAULT_INTERIORS_REVISIT_TO_VAULT_REVISIT_SOUTHEAST },
    { MAP_EVENT_WARP, 2, CONDITION_ALWAYS, VAULT_INTERIORS_REVISIT_TO_VAULT_REVISIT_SOUTH },
    { MAP_EVENT_WARP, 3, CONDITION_ALWAYS, VAULT_INTERIORS_REVISIT_TO_VAULT_REVISIT_SOUTHWEST },
    { MAP_EVENT_WARP, 4, CONDITION_ALWAYS, VAULT_INTERIORS_REVISIT_TO_VAULT_REVISIT_EAST },
    { MAP_EVENT_WARP, 5, CONDITION_ALWAYS, VAULT_INTERIORS_REVISIT_TO_VAULT_REVISIT_NORTH },
    { MAP_EVENT_WARP, 6, CONDITION_ALWAYS, VAULT_INTERIORS_REVISIT_TO_VAULT_REVISIT_NORTHWEST },
    { MAP_EVENT_WARP, 7, CONDITION_ALWAYS, VAULT_INTERIORS_REVISIT_INN_UPSTAIRS_TO_INN },
    { MAP_EVENT_WARP, 8, CONDITION_ALWAYS, VAULT_INTERIORS_REVISIT_INN_TO_INN_UPSTAIRS },
    { MAP_EVENT_TALK, LOCALID_GEM_OWNER, CONDITION_ALWAYS, MSGNO(MsgVaultWaahACatAteMyPrecious) },
    { MAP_EVENT_TALK, LOCALID_GEM_OWNERS_WIFE, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WifeSawCatTakeGem) },
    { MAP_EVENT_TALK, LOCALID_VILLAGER_HEARD_PRISONERS, CONDITION_ALWAYS, MSGNO(MsgVaultThePrisonersSaidTheyWantedTo) },
    { MAP_EVENT_TALK, LOCALID_BOY, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_BoyDoubtsThievesWereFromLunpa) },
    { MAP_EVENT_TALK, LOCALID_COLOSSO_HUSBAND, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_HusbandLongsForColosso) },
    { MAP_EVENT_TALK, LOCALID_COLOSSO_WIFE, CONDITION_ALWAYS, MSGNO(MsgVaultTheWarriorsISawFightingAt) },
    { MAP_EVENT_TALK, LOCALID_MAYOR, FLAG_VAULT_REVISIT_ISAAC_WELCOMED_BACK, EVENT_SCRIPT(EventScript_WelcomeIsaacBack) },
    { MAP_EVENT_TALK, LOCALID_MAYOR, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WarnOfThievesRevenge) },
    { MAP_EVENT_TALK, LOCALID_MAYORS_WIFE, CONDITION_ALWAYS, MSGNO(MsgVaultTheMayorsFamilyUrnIsOn) },
    { MAP_EVENT_TALK, LOCALID_BOY_ON_THE_THIEVES_ESCAPE, CONDITION_ALWAYS, MSGNO(MsgVaultThoseThievesYouGoToAll) },
    { MAP_EVENT_TALK, LOCALID_GIRL_CURIOUS_ABOUT_THE_FESTIVAL, CONDITION_ALWAYS, MSGNO(MsgVaultIHearThatABigFestival) },
    { MAP_EVENT_TALK, LOCALID_VAULT_WEAPON_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToWeaponMerchant) },
    { MAP_EVENT_TALK, LOCALID_VAULT_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToArmorMerchant) },
    { MAP_EVENT_TALK, LOCALID_VAULT_ITEM_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToItemMerchant) },
    { MAP_EVENT_TALK, LOCALID_VAULT_INNKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToInnkeeper) },
    { MAP_EVENT_TALK, LOCALID_COOK, CONDITION_ALWAYS, MSGNO(MsgVaultWeDontGetALotOf) },
    // Unused: the row above always answers the cook first.
    { MAP_EVENT_TALK, LOCALID_COOK, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_UnusedOfferLeftoverBones) },
    { MAP_EVENT_TALK, LOCALID_GIRL_AT_THE_INN, FLAG_VAULT_LEADER_BESIDE_BONE, EVENT_SCRIPT(EventScript_OfferBones) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_GEM_OWNER, CONDITION_ALWAYS, MSGNO(MsgVaultIfIdKnownThisWasGoing) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_GEM_OWNERS_WIFE, CONDITION_ALWAYS, MSGNO(MsgVaultMyHusbandSaysMyCatAte) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_VILLAGER_HEARD_PRISONERS, CONDITION_ALWAYS, MSGNO(MsgVaultWhyDoYouSupposeThoseThieves) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BOY, CONDITION_ALWAYS, MSGNO(MsgVaultDaddyHasntBeenPayingAttentionSince) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_COLOSSO_HUSBAND, CONDITION_ALWAYS, MSGNO(MsgVaultWheneverMyWifeUsedToGo) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_COLOSSO_WIFE, CONDITION_ALWAYS, MSGNO(MsgVaultLatelyMyHusbandsBeenGoingOn) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), 0x400 | LOCALID_MAYOR, FLAG_VAULT_REVISIT_ISAAC_WELCOMED_BACK, EVENT_SCRIPT(EventScript_ReadMindAndWelcomeIsaacBack) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_MAYOR, CONDITION_ALWAYS, MSGNO(MsgVaultAsLongAsWeStayAlert) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_MAYORS_WIFE, CONDITION_ALWAYS, MSGNO(MsgVaultWellBeSecuringItInA) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_BOY_ON_THE_THIEVES_ESCAPE, CONDITION_ALWAYS, MSGNO(MsgVaultItsJustEmbarrassingThatWeLet) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_GIRL_CURIOUS_ABOUT_THE_FESTIVAL, CONDITION_ALWAYS, MSGNO(MsgVaultILoveFestivalsIllBetColosso) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_VAULT_WEAPON_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgVaultJustImagineHowTerribleItWould) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_VAULT_ARMOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgVaultItsAGoodThingThatGiant) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_VAULT_ITEM_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgVaultWhyKeepTheStoreOpenIf) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_VAULT_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgVaultWontAnyoneComeStayTheNight) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_COOK, CONDITION_ALWAYS, MSGNO(MsgVaultAfterYouveBeenInThisBusiness) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_GIRL_AT_THE_INN, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ReadBoneGiverMind) },
    { BG_EVENT(BG_EVENT_SILENT), MB_VAULT_INTERIORS_INN_BARREL, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_FindBone) },
    { BG_EVENT(BG_EVENT_JAR), MB_VAULT_INTERIORS_JAR, FLAG_VAULT_INTERIORS_JAR_COINS, FIND_COINS(7) },
    { BG_EVENT(BG_EVENT_BARREL), MB_VAULT_INTERIORS_FIRST_BARREL, FLAG_VAULT_INTERIORS_BARREL_COINS, FIND_COINS(4) },
    { BG_EVENT(BG_EVENT_BARREL), MB_VAULT_INTERIORS_SECOND_BARREL, FLAG_VAULT_INTERIORS_BARREL_SLEEP_BOMB, FIND_ITEM(ITEM_SLEEP_BOMB) },
    { BG_EVENT(BG_EVENT_WOODEN_BOX), MB_VAULT_INTERIORS_WOODEN_BOX, FLAG_VAULT_INTERIORS_BOX_MINT, FIND_ITEM(ITEM_MINT) },
    { BG_EVENT(BG_EVENT_BOOKCASE) | MAP_EVENT_FLAG_FACING | FACING_NORTH, MB_VAULT_INTERIORS_HISTORY_BOOKCASE, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgVaultTheresABookEntitledHistoryOf)) },
    { BG_EVENT(BG_EVENT_BOOKCASE), MB_VAULT_INTERIORS_GEMS_BOOKCASE, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgVaultTheresABookEntitledGemsOfAngara)) },
    { MAP_EVENTS_END },
};
// clang-format on
