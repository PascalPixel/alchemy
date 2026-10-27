/// @file
/// Log rooms B (SceneId_LogRoomsB), a sheet of rooms drawn in logs, all of them
/// Vault's: the town's houses, shops and inn once the party has reached Kalay.
/// They replace the first visit's interiors (log_rooms_a.c) as
/// SceneId_VaultRevisit (vault_revisit.c) replaces the town: each of its six
/// house doors leads to the entrance here named for the same door, and
/// each room leads back out through it.
///
/// The thieves have broken out of jail, and the talk is of them and of Colosso,
/// the festival about to begin in Tolbi. The rooms, by entrance:
/// - LOG_ROOMS_B_FROM_VAULT_REVISIT_SOUTHEAST, the gem owner's
///   house: he says a cat ate his precious stone, and his wife does not believe
///   him. Arriving here sets him on the floor and takes the stone off his
///   table: the 4-by-3 block of cells copied from (0, 120) to (8, 67) lies
///   under the window of the map grid's layer 2, at the table, and draws it
///   bare where the first visit's house (log_rooms_a.c) draws the sparkling
///   stone on it (render probe log-rooms-b, the room before and after).
/// - LOG_ROOMS_B_FROM_VAULT_REVISIT_SOUTH, the house of a boy who
///   doubts the thieves were from Lunpa.
/// - LOG_ROOMS_B_FROM_VAULT_REVISIT_SOUTHWEST and
///   LOG_ROOMS_B_INN_FROM_INN_UPSTAIRS, the inn, where the girl at
///   the inn offers Bones and a Bone lies on the floor beside her; its stairs
///   lead to the upstairs room, where a cook works.
/// - LOG_ROOMS_B_FROM_VAULT_REVISIT_EAST, the item shop.
/// - LOG_ROOMS_B_FROM_VAULT_REVISIT_NORTH, the weapon and armor
///   shops.
/// - LOG_ROOMS_B_FROM_VAULT_REVISIT_NORTHWEST, the mayor's house,
///   where the mayor welcomes Isaac back and warns that the thieves have
///   escaped (FLAG_VAULT_REVISIT_ISAAC_WELCOMED_BACK, an area flag, and
///   FLAG_VAULT_WARNED_OF_ESCAPED_THIEVES); a Mind Read on him before that
///   plays the welcome first.
///
/// The Bones are how the party gets the dog in Vault to show its hidden
/// passage. The girl's offer gives one Bone each time it is accepted
/// (FLAG_VAULT_BONE_RECEIVED, shared with log_rooms_a.c, changes only her
/// words), and searching the floor Bone (MB_LOG_ROOMS_A_INN_BARREL,
/// Vault_FindBone) gives another. While the leader stands beside the floor
/// Bone, Vault_UpdateBoneProximityFlag sets FLAG_VAULT_LEADER_BESIDE_BONE,
/// which disables her talk row, so that the A button there searches instead of
/// talking. The shopkeepers open their shops only when spoken to across the
/// counter. A jar, two barrels and a wooden box keep the first visit's flags,
/// so what the party took then stays taken, and two bookcases can be read.

#include "common.h"
#include "unit/scene/log_rooms_b.h"
#include "log_rooms_a.h"
#include "unit/scene/shared/vault_itemobj.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/event_commands.h"
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
#include "event_layer.h"
#include "engine_call.h"
#include "scene_tables.h"
#include "inventory.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "flags.h"
#include "heap.h"

/// The townspeople, numbering the actor template rows from
/// ACTOR_FIRST_PLACED and named by what they say on the party's return.
enum {
  /// A young man whose precious stone, he says, a cat ate.
  /// ★ Named from the game: "I kept worrying that they would take my gem."
  /// (MsgVaultICanSleepWellNowThat)
  ACTOR_GEM_OWNER = ACTOR_FIRST_PLACED,
  /// A woman with fair hair.
  /// ★ Named from the game: "My husband says my cat ate the treasure"
  /// (MsgVaultMyHusbandSaysMyCatAte)
  ACTOR_GEM_OWNERS_WIFE,
  /// A burly grey-haired man in the boy's house.
  /// ★ Named from the game: "The prisoners said they wanted to break out of
  /// jail before Colosso began." (MsgVaultThePrisonersSaidTheyWantedTo)
  ACTOR_VILLAGER_HEARD_PRISONERS,
  /// A small boy.
  /// ★ Named from the game: "Daddy hasn't been paying attention"
  /// (MsgVaultDaddyHasntBeenPayingAttentionSince)
  ACTOR_BOY,
  /// An old man with a cane who would like to see Colosso once more.
  /// ★ Named from the game: "Whenever my wife used to go to Tolbi for Colosso"
  /// (MsgVaultWheneverMyWifeUsedToGo)
  ACTOR_COLOSSO_HUSBAND,
  /// An old woman.
  /// ★ Named from the game: "Lately, my husband's been going on and on about
  /// Colosso." (MsgVaultLatelyMyHusbandsBeenGoingOn)
  ACTOR_COLOSSO_WIFE,
  /// A small boy in the mayor's house.
  /// ★ Named from the game: "Those thieves! You go to all that trouble catching
  /// them, and then we let them get away." (MsgVaultThoseThievesYouGoToAll)
  ACTOR_BOY_ON_THE_THIEVES_ESCAPE,
  /// A little girl in the mayor's house.
  /// ★ Named from the game: "I hear that a big festival is about to start in
  /// some faraway town." (MsgVaultIHearThatABigFestival)
  ACTOR_GIRL_CURIOUS_ABOUT_THE_FESTIVAL,
  /// The mayor, an old man with white hair.
  /// ★ Named from the game: "My husband, the mayor"
  /// (MsgVaultMyHusbandTheMayorCantBelieve)
  ACTOR_MAYOR,
  /// A woman in a green dress.
  /// ★ Named from the game: "My husband, the mayor, can't believe anyone from
  /// Vault could be the thief." (MsgVaultMyHusbandTheMayorCantBelieve)
  ACTOR_MAYORS_WIFE,
  /// The girl at the inn, as the Bone's description calls her, a young woman
  /// with fair hair.
  /// ★ Named from the game: "A bone from the girl at the inn"
  /// (MsgItemPlainName)
  ACTOR_GIRL_AT_THE_INN,
  /// ★ Named from the game: "I'm sorry I've only got these cheap weapons to
  /// sell right now." (MsgVaultAllOurBestWeaponsWereStolen)
  ACTOR_VAULT_WEAPON_MERCHANT,
  /// ★ Named from the game: "I wonder if they left us alone because we don't
  /// carry any valuable armor?" (MsgVaultIWonderIfTheyLeftUs)
  ACTOR_VAULT_ARMOR_MERCHANT,
  /// ★ Named from the game: "Why keep the store open if we don't have any
  /// customers?" (MsgVaultWhyKeepTheStoreOpenIf)
  ACTOR_VAULT_ITEM_MERCHANT,
  /// A man with dark hair behind the inn's counter.
  /// ★ Named from the game: "Won't anyone come stay the night here?"
  /// (MsgVaultWontAnyoneComeStayTheNight)
  ACTOR_VAULT_INNKEEPER,
  /// A woman in an apron, in the inn's other room.
  /// ★ Named from the game: "I did some cooking, so now I've got all these
  /// useless bones." (MsgVaultDidSomeCookingNowIve)
  ACTOR_COOK
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
enum { FLAG_VAULT_REVISIT_ISAAC_WELCOMED_BACK = FLAG_AREA_TEMP_00 };

// Shared scene routines stay in their linked order.
#include "shared/vault_spotflag.h"

const struct SceneEntrance *Scene_GetEntrances(void) {
  return gLogRoomsB_Entrances;
}

const struct SceneExitRegion *Scene_GetExitRegions(void) { return NULL; }

const u32 *Scene_GetExits(void) { return gLogRoomsB_Exits; }

/// Gives the actor templates' condition 0 whether each lies inside the
/// view bounds of the room entered (Scene_AssignViewFlags), so only that
/// room's people appear.
const struct ActorTemplate *Scene_GetActorTemplates(void) {
  struct ActorTemplate *templates = gLogRoomsB_ActorTemplates;

  Scene_AssignViewFlags(templates);
  return templates;
}

/// The actor stands still and says the selected message.
static void LogRoomsB_ShowStandingActorDialogue(s32 actorId) {
  Event_Start();
  Import_ActorSetAnimation(actorId, ANIM_STAND);
  Event_ShowMessageAndWait(actorId, 0);
  Event_End();
}

/// Faces the leader and shows this resident's dialogue while standing still.
static inline void LogRoomsB_FaceAndSpeak(s32 actorId, s32 message) {
  Event_SetMessageId(message);
  Actor_LinkPair(actorId, ACTOR_PARTY_LEADER, 2);
  LogRoomsB_ShowStandingActorDialogue(actorId);
}

/// The wife thought she saw the cat paw the gemstone on the table.
/// Then the treasure was gone. She's in the house at
/// LOG_ROOMS_B_FROM_VAULT_REVISIT_SOUTHEAST.
static void LogRoomsB_WifeSawCatTakeGem(void) {
  LogRoomsB_FaceAndSpeak(ACTOR_GEM_OWNERS_WIFE, MsgVaultThoughtSawCatGetUp);
}

/// The boy doubts the thieves were from Lunpa after all.
/// He points out Dodonpa's henchmen never attacked. He's in the house at
/// LOG_ROOMS_B_FROM_VAULT_REVISIT_SOUTH.
static void LogRoomsB_BoyDoubtsThievesWereFromLunpa(void) {
  LogRoomsB_FaceAndSpeak(ACTOR_BOY, MsgVaultDodonpasHenchmenNeverAttackedUs);
}

/// In the item shop, the husband would like to see Colosso in Tolbi once more
/// before he dies.
static void LogRoomsB_HusbandLongsForColosso(void) {
  LogRoomsB_FaceAndSpeak(ACTOR_COLOSSO_HUSBAND, MsgVaultItsNearTimeForColosso);
}

/// In his house, the mayor welcomes Isaac back: the thieves he caught have
/// escaped from jail, and Vault feared they would come after him. Asked whether
/// the party has run into them, a yes and a no get different warnings. Sets
/// FLAG_VAULT_REVISIT_ISAAC_WELCOMED_BACK, after which the mayor only warns of
/// their revenge, and FLAG_VAULT_WARNED_OF_ESCAPED_THIEVES.
static void LogRoomsB_WelcomeIsaacBack(void) {
  Event_Start();
  Event_SetMessageId(MsgVaultIsaacYouveComeBackVault);
  Actor_LinkPair(ACTOR_MAYOR, ACTOR_PARTY_LEADER, 2);
  Import_ActorSetAnimation(ACTOR_MAYOR, ANIM_STAND);
  FieldEvent_ShowMessageThenWait(ACTOR_MAYOR, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_MAYOR, ANIM_SHAKE_HEAD);
  Event_WaitTicks(20);
  FieldEvent_ShowMessageThenWait(ACTOR_MAYOR, 0, 20);
  Actor_ShowEmote(ACTOR_MAYOR, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  FieldEvent_ShowMessageThenWait(ACTOR_MAYOR, 0, 30);
  // The negative answer advances to the next message before continuing.
  Event_OpenMessage(ACTOR_MAYOR, 0);
  if (Event_AskYesNoWithActorAnimation(0, 0) != 0) {
    gFieldWork->message++;
  }
  FieldEvent_ShowMessageThenWait(ACTOR_MAYOR, 0, 20);
  GameFlag_Set(FLAG_VAULT_REVISIT_ISAAC_WELCOMED_BACK);
  GameFlag_Set(FLAG_VAULT_WARNED_OF_ESCAPED_THIEVES);
  Event_End();
}

/// After the welcome: those thieves really wanted revenge, so take care.
static void LogRoomsB_WarnOfThievesRevenge(void) {
  LogRoomsB_FaceAndSpeak(ACTOR_MAYOR, MsgVaultThoseGuysReallyWantedRevenge);
}

/// Unused: the cook in the inn's other room offers the party her leftover
/// bones, but gives none.
///
/// She would have said `I did some cooking, so now I've got all these useless
/// bones.` and asked `...You want some?`. His earlier talk row,
/// MsgVaultWeDontGetALotOf with no condition, always answers first
/// (Scene_FindTrigger takes the first match), so the player only ever hears
/// that travelers are few in winter.
static void LogRoomsB_UnusedOfferLeftoverBones(void) {
  LogRoomsB_FaceAndSpeak(ACTOR_COOK, MsgVaultDidSomeCookingNowIve);
}

/// The girl at the inn offers leftover bones from dinner, or more of them once
/// one was taken (FLAG_VAULT_BONE_RECEIVED); a yes gets a Bone if there is
/// room, a no a nod. Her talk row needs FLAG_VAULT_LEADER_BESIDE_BONE clear,
/// that is, the leader not standing beside the floor Bone.
static void LogRoomsB_OfferBones(void) {
  Event_Start();
  Actor_LinkPair(ACTOR_GIRL_AT_THE_INN, ACTOR_PARTY_LEADER, 0);
  if (GameFlag_Get(FLAG_VAULT_BONE_RECEIVED) == 0) {
    Event_SetMessageId(MsgVaultHaveLotLeftoverBonesFrom);
    Event_OpenMessage(ACTOR_GIRL_AT_THE_INN, 0);
  } else {
    Event_SetMessageId(MsgVaultWantMoreBones);
    Event_OpenMessage(ACTOR_GIRL_AT_THE_INN, 0);
  }
  if (Event_AskYesNoWithActorAnimation(0, 0) == 0) {
    // Show the offer before checking whether the party can carry the gift.
    Event_WaitTicks(20);
    Event_ShowMessage(ACTOR_GIRL_AT_THE_INN, 0);
    Event_WaitTicks(20);
    Actor_RunRepeatedMotion(ACTOR_GIRL_AT_THE_INN, 2);
    Event_WaitTicks(20);
    // Give the Bone only after the offer is accepted and space is available.
    if (PartyInventory_HasSpace() == 0) {
      Actor_SetAnimationAndWait(ACTOR_GIRL_AT_THE_INN, ANIM_SHAKE_HEAD);
      Event_WaitTicks(20);
      Event_SetMessageId(MsgVaultWowHaveManyThingsArent);
      Event_ShowMessage(ACTOR_GIRL_AT_THE_INN, 0);
    } else {
      ItemDisplay_ShowItem(ITEM_BONE, 3);
      PartyInventory_GiveItem(ITEM_BONE, 0);
      GameFlag_Set(FLAG_VAULT_BONE_RECEIVED);
    }
  } else {
    // Declining selects the following line and the actor's response
    // pose.
    gFieldWork->message++;
    Event_WaitTicks(20);
    Actor_SetAnimationAndWait(ACTOR_GIRL_AT_THE_INN, ANIM_NOD);
    Event_WaitTicks(20);
    Event_ShowMessage(ACTOR_GIRL_AT_THE_INN, 0);
  }
  Actor_FaceDirection(ACTOR_GIRL_AT_THE_INN, FACING_SOUTH, 0);
  Event_End();
}

/// Mind Read on the mayor before the welcome: he notices the party and
/// welcomes Isaac back.
static void LogRoomsB_ReadMindAndWelcomeIsaacBack(void) {
  u8 *paused;
  u8 unpaused = 0;

  Event_Start();
  Actor_RunRepeatedMotion(ACTOR_MAYOR, 1);
  Event_End();
  // Keep the actor paused through the conversation, then resume
  // action 2.
  paused = &Actor_GetById(ACTOR_MAYOR)->paused;
  *paused = 1;
  LogRoomsB_WelcomeIsaacBack();
  paused = &Actor_GetById(ACTOR_MAYOR)->paused;
  *paused = unpaused;
  Actor_SetMovementTypeById(ACTOR_MAYOR, 2);
}

// Shared Bone-item dialogue follows the actor conversations.
#include "shared/vault_bone.h"

/// Across the counter the weapon shop opens; from elsewhere the merchant talks
/// of a river of fire on the other side of Mt. Aleph.
static void LogRoomsB_TalkToWeaponMerchant(void) {
  u32 leaderFacing;

  leaderFacing = Actor_GetById(ACTOR_PARTY_LEADER)->facing;
  Event_Start();

  if (FACING_IS_NORTH(leaderFacing)) {
    Shop_Run(VAULT_WEAPON_STOCK, ACTOR_VAULT_WEAPON_MERCHANT);
  } else {
    Event_SetMessageId(MsgVaultTheresRiverFireOnOther);
    Event_ShowMessageAndWait(ACTOR_VAULT_WEAPON_MERCHANT, 0);
  }

  Event_End();
}

/// Across the counter the armor shop opens; from elsewhere the merchant says
/// the eruption split the continent in two.
static void LogRoomsB_TalkToArmorMerchant(void) {
  u32 leaderFacing;

  leaderFacing = Actor_GetById(ACTOR_PARTY_LEADER)->facing;
  Event_Start();

  if (FACING_IS_NORTH(leaderFacing)) {
    Shop_Run(VAULT_ARMOR_STOCK, ACTOR_VAULT_ARMOR_MERCHANT);
  } else {
    Event_SetMessageId(MsgVaultTheySayVolcanoOnMt);
    Event_ShowMessageAndWait(ACTOR_VAULT_ARMOR_MERCHANT, 0);
  }

  Event_End();
}

/// Across the counter the item shop opens; from elsewhere the merchant has had
/// few customers lately.
static void LogRoomsB_TalkToItemMerchant(void) {
  u32 leaderFacing;

  leaderFacing = Actor_GetById(ACTOR_PARTY_LEADER)->facing;
  Event_Start();

  if (FACING_IS_NORTH(leaderFacing)) {
    Shop_Run(VAULT_ITEM_STOCK, ACTOR_VAULT_ITEM_MERCHANT);
  } else {
    Event_SetMessageId(MsgVaultWeHaventBeenGettingMany);
    Event_ShowMessageAndWait(ACTOR_VAULT_ITEM_MERCHANT, 0);
  }

  Event_End();
}

/// Across the counter the inn opens; from elsewhere the innkeeper complains
/// that no one comes, what with the volcano.
static void LogRoomsB_TalkToInnkeeper(void) {
  u32 leaderFacing;

  leaderFacing = Actor_GetById(ACTOR_PARTY_LEADER)->facing;
  Event_Start();

  if (FACING_IS_NORTH(leaderFacing)) {
    Inn_CheckIn(VAULT_INN, ACTOR_VAULT_INNKEEPER);
  } else {
    Event_SetMessageId(MsgVaultMmmmNothingDoNothingDo);
    Event_ShowMessageAndWait(ACTOR_VAULT_INNKEEPER, 0);
  }

  Event_End();
}

/// Mind Read on the girl at the inn: hungry while cooking, or, once a Bone was
/// given, wondering whether "he" has a calcium deficiency.
static void LogRoomsB_ReadBoneGiverMind(void) {
  Event_Start();
  if (GameFlag_Get(FLAG_VAULT_BONE_RECEIVED) == 0) {
    Event_SetMessageId(MsgVaultGeeAlwaysGetHungryWhen);
  } else {
    Event_SetMessageId(MsgVaultHeReallyLikesBonesWonder);
  }
  Event_ShowMessageAndWait(ACTOR_GIRL_AT_THE_INN, 0);
  Event_End();
}

/// Returns the talk, Psynergy, examine trigger and exit table.
const struct Trigger *Scene_GetTriggers(void) { return gLogRoomsB_Triggers; }

/// Opens the room through a window transition. Entering the house at
/// LOG_ROOMS_B_FROM_VAULT_REVISIT_SOUTHEAST sets its owner on the
/// floor and takes the stone off his table; entering the inn
/// (LOG_ROOMS_B_FROM_VAULT_REVISIT_SOUTHWEST, or from upstairs by
/// LOG_ROOMS_B_INN_FROM_INN_UPSTAIRS)
/// places the floor Bone beside the girl and starts
/// Vault_UpdateBoneProximityFlag.
s32 Scene_Initialize(void) {
  s32 entrance;
  s32 clearedState;

  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  entrance = gCommon.entrance;

  if (entrance == LOG_ROOMS_B_FROM_VAULT_REVISIT_SOUTHEAST) {
    // Draw the table bare, its stone gone, and set the gem owner back on the
    // floor.
    MapGrid_CopyCellsTo(0, 120, 8, 67, 4, 3);
    clearedState = 0;
    Actor_GetById(ACTOR_GEM_OWNER)->motionFlags = clearedState;
    Actor_GetById(ACTOR_GEM_OWNER)->y.fixed = clearedState;
    Actor_GetById(ACTOR_GEM_OWNER)->groundHeight = clearedState;
  } else if (entrance == LOG_ROOMS_B_FROM_VAULT_REVISIT_SOUTHWEST ||
             entrance == LOG_ROOMS_B_INN_FROM_INN_UPSTAIRS) {
    // Show the Bone lying in the room, and track whether the leader stands
    // beside it (FLAG_VAULT_LEADER_BESIDE_BONE).
    Vault_PlaceItemIcon(ITEM_BONE, PIXELS(568), PIXELS(16), PIXELS(672));
    Task_AddFrame(Vault_UpdateBoneProximityFlag, TASK_PASS_FRAME);
  }

  return 0;
}

// Shared item-object routines follow the scene initializer.
#include "shared/vault_itemobj.h"

struct SceneEntrance gLogRoomsB_Entrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {LOG_ROOMS_B_FROM_VAULT_REVISIT_SOUTHEAST, CONDITION_ALWAYS, 192, 0, 220,
     FACING_NORTH, 0, 32, 0, 272, 240, 0},
    {LOG_ROOMS_B_FROM_VAULT_REVISIT_SOUTH, CONDITION_ALWAYS, 432, 0, 254,
     FACING_NORTH, 0, 288, 0, 528, 272, 0},
    {LOG_ROOMS_B_FROM_VAULT_REVISIT_SOUTHWEST, CONDITION_ALWAYS, 720, 0, 782,
     FACING_NORTH, 0, 432, 576, 800, 800, 0},
    {LOG_ROOMS_B_FROM_VAULT_REVISIT_EAST, CONDITION_ALWAYS, 624, 0, 236,
     FACING_NORTH, 0, 560, 32, 848, 256, 0},
    {LOG_ROOMS_B_FROM_VAULT_REVISIT_NORTH, CONDITION_ALWAYS, 144, 0, 492,
     FACING_NORTH, 0, 16, 288, 256, 512, 0},
    {LOG_ROOMS_B_FROM_VAULT_REVISIT_NORTHWEST, CONDITION_ALWAYS, 416, 0, 526,
     FACING_NORTH, 0, 320, 288, 592, 544, 0},
    {LOG_ROOMS_B_INN_FROM_INN_UPSTAIRS, CONDITION_ALWAYS, 744, 0, 640,
     FACING_SOUTH, 0, 432, 576, 800, 800, 0},
    {LOG_ROOMS_B_INN_UPSTAIRS_FROM_INN, CONDITION_ALWAYS, 358, 0, 632,
     FACING_SOUTH, 0, 32, 576, 400, 776, 0},
    {SCENE_TABLE_END},
};

u32 gLogRoomsB_Exits[] = {
    SCENE_EXITS(LogRoomsB),
    SCENE_EXIT(LOG_ROOMS_B_TO_VAULT_REVISIT_SOUTHEAST, VaultRevisit,
               VAULT_REVISIT_FROM_LOG_ROOMS_B_SOUTHEAST),
    SCENE_EXIT(LOG_ROOMS_B_TO_VAULT_REVISIT_SOUTH, VaultRevisit,
               VAULT_REVISIT_FROM_LOG_ROOMS_B_SOUTH),
    SCENE_EXIT(LOG_ROOMS_B_TO_VAULT_REVISIT_SOUTHWEST, VaultRevisit,
               VAULT_REVISIT_FROM_LOG_ROOMS_B_SOUTHWEST),
    SCENE_EXIT(LOG_ROOMS_B_TO_VAULT_REVISIT_EAST, VaultRevisit,
               VAULT_REVISIT_FROM_LOG_ROOMS_B_EAST),
    SCENE_EXIT(LOG_ROOMS_B_TO_VAULT_REVISIT_NORTH, VaultRevisit,
               VAULT_REVISIT_FROM_LOG_ROOMS_B_NORTH),
    SCENE_EXIT(LOG_ROOMS_B_TO_VAULT_REVISIT_NORTHWEST, VaultRevisit,
               VAULT_REVISIT_FROM_LOG_ROOMS_B_NORTHWEST),
    SCENE_EXIT(LOG_ROOMS_B_INN_UPSTAIRS_TO_INN, LogRoomsB,
               LOG_ROOMS_B_INN_FROM_INN_UPSTAIRS),
    SCENE_EXIT(LOG_ROOMS_B_INN_TO_INN_UPSTAIRS, LogRoomsB,
               LOG_ROOMS_B_INN_UPSTAIRS_FROM_INN),
    SCENE_EXITS_END,
};

// clang-format off
struct ActorTemplate gLogRoomsB_ActorTemplates[] = {
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
    { SCENE_TABLE_END },
};

struct Trigger gLogRoomsB_Triggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, LOG_ROOMS_B_TO_VAULT_REVISIT_SOUTHEAST },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, LOG_ROOMS_B_TO_VAULT_REVISIT_SOUTH },
    { TRIGGER_EXIT, 3, CONDITION_ALWAYS, LOG_ROOMS_B_TO_VAULT_REVISIT_SOUTHWEST },
    { TRIGGER_EXIT, 4, CONDITION_ALWAYS, LOG_ROOMS_B_TO_VAULT_REVISIT_EAST },
    { TRIGGER_EXIT, 5, CONDITION_ALWAYS, LOG_ROOMS_B_TO_VAULT_REVISIT_NORTH },
    { TRIGGER_EXIT, 6, CONDITION_ALWAYS, LOG_ROOMS_B_TO_VAULT_REVISIT_NORTHWEST },
    { TRIGGER_EXIT, 7, CONDITION_ALWAYS, LOG_ROOMS_B_INN_UPSTAIRS_TO_INN },
    { TRIGGER_EXIT, 8, CONDITION_ALWAYS, LOG_ROOMS_B_INN_TO_INN_UPSTAIRS },
    { TRIGGER_TALK, ACTOR_GEM_OWNER, CONDITION_ALWAYS, MSGNO(MsgVaultWaahACatAteMyPrecious) },
    { TRIGGER_TALK, ACTOR_GEM_OWNERS_WIFE, CONDITION_ALWAYS, EVENT(LogRoomsB_WifeSawCatTakeGem) },
    { TRIGGER_TALK, ACTOR_VILLAGER_HEARD_PRISONERS, CONDITION_ALWAYS, MSGNO(MsgVaultThePrisonersSaidTheyWantedTo) },
    { TRIGGER_TALK, ACTOR_BOY, CONDITION_ALWAYS, EVENT(LogRoomsB_BoyDoubtsThievesWereFromLunpa) },
    { TRIGGER_TALK, ACTOR_COLOSSO_HUSBAND, CONDITION_ALWAYS, EVENT(LogRoomsB_HusbandLongsForColosso) },
    { TRIGGER_TALK, ACTOR_COLOSSO_WIFE, CONDITION_ALWAYS, MSGNO(MsgVaultTheWarriorsISawFightingAt) },
    { TRIGGER_TALK, ACTOR_MAYOR, FLAG_VAULT_REVISIT_ISAAC_WELCOMED_BACK, EVENT(LogRoomsB_WelcomeIsaacBack) },
    { TRIGGER_TALK, ACTOR_MAYOR, CONDITION_ALWAYS, EVENT(LogRoomsB_WarnOfThievesRevenge) },
    { TRIGGER_TALK, ACTOR_MAYORS_WIFE, CONDITION_ALWAYS, MSGNO(MsgVaultTheMayorsFamilyUrnIsOn) },
    { TRIGGER_TALK, ACTOR_BOY_ON_THE_THIEVES_ESCAPE, CONDITION_ALWAYS, MSGNO(MsgVaultThoseThievesYouGoToAll) },
    { TRIGGER_TALK, ACTOR_GIRL_CURIOUS_ABOUT_THE_FESTIVAL, CONDITION_ALWAYS, MSGNO(MsgVaultIHearThatABigFestival) },
    { TRIGGER_TALK, ACTOR_VAULT_WEAPON_MERCHANT, CONDITION_ALWAYS, EVENT(LogRoomsB_TalkToWeaponMerchant) },
    { TRIGGER_TALK, ACTOR_VAULT_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT(LogRoomsB_TalkToArmorMerchant) },
    { TRIGGER_TALK, ACTOR_VAULT_ITEM_MERCHANT, CONDITION_ALWAYS, EVENT(LogRoomsB_TalkToItemMerchant) },
    { TRIGGER_TALK, ACTOR_VAULT_INNKEEPER, CONDITION_ALWAYS, EVENT(LogRoomsB_TalkToInnkeeper) },
    { TRIGGER_TALK, ACTOR_COOK, CONDITION_ALWAYS, MSGNO(MsgVaultWeDontGetALotOf) },
    // Unused: the row above always answers the cook first.
    { TRIGGER_TALK, ACTOR_COOK, CONDITION_ALWAYS, EVENT(LogRoomsB_UnusedOfferLeftoverBones) },
    { TRIGGER_TALK, ACTOR_GIRL_AT_THE_INN, FLAG_VAULT_LEADER_BESIDE_BONE, EVENT(LogRoomsB_OfferBones) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GEM_OWNER, CONDITION_ALWAYS, MSGNO(MsgVaultIfIdKnownThisWasGoing) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GEM_OWNERS_WIFE, CONDITION_ALWAYS, MSGNO(MsgVaultMyHusbandSaysMyCatAte) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_HEARD_PRISONERS, CONDITION_ALWAYS, MSGNO(MsgVaultWhyDoYouSupposeThoseThieves) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY, CONDITION_ALWAYS, MSGNO(MsgVaultDaddyHasntBeenPayingAttentionSince) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_COLOSSO_HUSBAND, CONDITION_ALWAYS, MSGNO(MsgVaultWheneverMyWifeUsedToGo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_COLOSSO_WIFE, CONDITION_ALWAYS, MSGNO(MsgVaultLatelyMyHusbandsBeenGoingOn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x400 | ACTOR_MAYOR, FLAG_VAULT_REVISIT_ISAAC_WELCOMED_BACK, EVENT(LogRoomsB_ReadMindAndWelcomeIsaacBack) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAYOR, CONDITION_ALWAYS, MSGNO(MsgVaultAsLongAsWeStayAlert) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAYORS_WIFE, CONDITION_ALWAYS, MSGNO(MsgVaultWellBeSecuringItInA) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_ON_THE_THIEVES_ESCAPE, CONDITION_ALWAYS, MSGNO(MsgVaultItsJustEmbarrassingThatWeLet) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_CURIOUS_ABOUT_THE_FESTIVAL, CONDITION_ALWAYS, MSGNO(MsgVaultILoveFestivalsIllBetColosso) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VAULT_WEAPON_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgVaultJustImagineHowTerribleItWould) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VAULT_ARMOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgVaultItsAGoodThingThatGiant) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VAULT_ITEM_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgVaultWhyKeepTheStoreOpenIf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VAULT_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgVaultWontAnyoneComeStayTheNight) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_COOK, CONDITION_ALWAYS, MSGNO(MsgVaultAfterYouveBeenInThisBusiness) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_AT_THE_INN, CONDITION_ALWAYS, EVENT(LogRoomsB_ReadBoneGiverMind) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), MB_LOG_ROOMS_A_INN_BARREL, CONDITION_ALWAYS, EVENT(Vault_FindBone) },
    { EXAMINE_TRIGGER(EXAMINE_JAR), MB_LOG_ROOMS_A_JAR, FLAG_LOG_ROOMS_A_JAR_COINS, FIND_COINS(7) },
    { EXAMINE_TRIGGER(EXAMINE_BARREL), MB_LOG_ROOMS_A_FIRST_BARREL, FLAG_LOG_ROOMS_A_BARREL_COINS, FIND_COINS(4) },
    { EXAMINE_TRIGGER(EXAMINE_BARREL), MB_LOG_ROOMS_A_SECOND_BARREL, FLAG_LOG_ROOMS_A_BARREL_SLEEP_BOMB, FIND_ITEM(ITEM_SLEEP_BOMB) },
    { EXAMINE_TRIGGER(EXAMINE_WOODEN_BOX), MB_LOG_ROOMS_A_WOODEN_BOX, FLAG_LOG_ROOMS_A_BOX_MINT, FIND_ITEM(ITEM_MINT) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE) | TRIGGER_FLAG_FACING | FACING_NORTH, MB_LOG_ROOMS_A_HISTORY_BOOKCASE, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgVaultTheresABookEntitledHistoryOf)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), MB_LOG_ROOMS_A_GEMS_BOOKCASE, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgVaultTheresABookEntitledGemsOfAngara)) },
    { TRIGGERS_END },
};
// clang-format on
