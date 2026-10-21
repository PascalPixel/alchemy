/// @file
/// Vault's houses, shops and inn (scene 24, a row with no SceneId_ name; the
/// area names call it Vault) as the party finds them once it has reached
/// Kalay. They replace the first visit's interiors (vault_interiors.c) as
/// Vault's revisit scene (vault_revisit.c) replaces the town: each of its
/// doors 5 to 10 leads to the entrance of the same number here, and each room
/// leads back out to it.
///
/// The thieves the party caught have broken out of jail, and the talk is of
/// them and of Colosso, the festival about to begin in Tolbi: a villager heard
/// the prisoners say they wanted to break out before it began. The rooms, by
/// entrance:
/// - 5, the house of a man whose precious stone, he says, a cat ate; his wife
///   thought she saw the cat paw it, and does not believe him. Arriving here
///   sets him on the floor and copies a 4-by-3 block of cells from (0, 120)
///   to (8, 67) (XXX: what they show is not known).
/// - 6, the house of a child who wonders whether the thieves were from Lunpa
///   at all, since Dodonpa's henchmen never attacked.
/// - 7 and 11, the inn: the innkeeper behind the counter, and the girl at the
///   inn, as the Bone's description calls her, who offers Bones left over
///   from dinner. A Bone also lies on the floor beside her. Trigger 8 leads to
///   the inn's other room (entrance 12), where a cook works, and trigger 7
///   back.
/// - 8, the item shop, with a couple who talk of Colosso.
/// - 9, the weapon and armor shops.
/// - 10, the mayor's house. The mayor welcomes Isaac back and tells him the
///   thieves have escaped (FLAG_WELCOMED_BACK, a place flag, and
///   FLAG_VAULT_WARNED_OF_ESCAPED_THIEVES); a Mind Read on him before that
///   plays the welcome first. His wife says the family urn the thieves stole is
///   on display.
///
/// The Bones are how the party gets the dog in Vault to show its hidden
/// passage. The girl's offer gives one Bone each time it is accepted
/// (FLAG_VAULT_BONE_RECEIVED, shared with vault_interiors.c, changes only her
/// words), and searching the floor Bone (search trigger 41, Vault_FindBone)
/// gives another. While the leader stands beside the floor Bone,
/// Vault_UpdateBoneProximityFlag sets FLAG_SCENE_TEMP_50, which disables her
/// talk row, so that the A button there searches the Bone instead of talking
/// to her. The shopkeepers open their shops only when spoken to across the
/// counter, facing north, and otherwise only talk. The placements' condition
/// 0 is replaced as the scene loads with whether each actor stands inside the
/// room's view bounds, so only the people of the room entered appear. A jar,
/// two barrels and a wooden box keep the first visit's flags, so what the
/// party took then stays taken, and two books can be read.

#include "game_state.h"
#include "unit/scene/vault_interiors_revisit.h"
#include "unit/scene/common/vault_itemobj.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
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
#include "field_event.h"
#include "engine_call.h"
#include "field_scene.h"
#include "inventory.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "flags.h"

/// The townspeople, named by what they say on the party's return.
enum {
  ACTOR_GEM_OWNER = 8, ///< a cat ate his precious stone
  ACTOR_GEM_OWNERS_WIFE = 9,
  ACTOR_CHILD = 11,           ///< a child. "Daddy hasn't been paying attention"
  ACTOR_COLOSSO_HUSBAND = 12, ///< would like to see Colosso once more
  ACTOR_COLOSSO_WIFE = 13,
  ACTOR_MAYOR = 16, ///< "our mayor", as the villagers call him
  ACTOR_MAYORS_WIFE = 17,
  ACTOR_COOK = 23, ///< in the inn's other room; did some cooking
  /// The girl at the inn, as the Bone's description calls her.
  ACTOR_VAULT_BONE_GIVER = 18,
  ACTOR_VAULT_WEAPON_MERCHANT = 19,
  ACTOR_VAULT_ARMOR_MERCHANT = 20,
  ACTOR_VAULT_ITEM_MERCHANT = 21,
  ACTOR_VAULT_INNKEEPER = 22,
  VAULT_WEAPON_STOCK = 4,
  VAULT_ARMOR_STOCK = 5,
  VAULT_ITEM_STOCK = 6,
  VAULT_INN = 1
};

/// Set once the mayor has told Isaac the thieves escaped from jail.
enum { FLAG_WELCOMED_BACK = FLAG_SCENE_TEMP_00 + 0x100 };

// Shared scene routines stay in their linked order.
#include "common/vault_spotflag.h"

const struct SceneEntrance *Scene_GetEntrances(void) {
  return gVaultInteriorsRevisit_Entrances;
}

const struct SceneRegion *Scene_GetRegions(void) { return NULL; }

const u32 *Scene_GetExits(void) { return gVaultInteriorsRevisit_Exits; }

/// Gives the placements' condition 0 whether each lies inside the view bounds
/// of the room entered (FieldScene_PrepareActors), so only that room's people
/// appear.
const struct ScenePlacement *Scene_GetPlacements(void) {
  struct ScenePlacement *placements = gVaultInteriorsRevisit_Placements;

  FieldScene_PrepareActors(placements);
  return placements;
}

/// The actor stands still and says the selected message.
static void VaultInteriorsRevisit_ShowStandingActorDialogue(s32 actor) {
  Engine_EventBegin();
  Engine_ActorSetAnimation(actor, ANIM_STAND);
  Engine_EventShowMessage(actor, 0);
  Engine_EventEnd();
}

/// Faces the leader and shows this resident's dialogue while standing still.
static inline void VaultInteriorsRevisit_FaceAndSpeak(s32 actor, s32 message) {
  Engine_EventSetMessage(message);
  Engine_ActorFaceEachOther(actor, ACTOR_PARTY_LEADER, 2);
  VaultInteriorsRevisit_ShowStandingActorDialogue(actor);
}

/// In the house behind door 5, the wife thought she saw the cat paw the
/// gemstone on the table, and then the treasure was gone.
static void Event_WifeSawCatTakeGem(void) {
  VaultInteriorsRevisit_FaceAndSpeak(ACTOR_GEM_OWNERS_WIFE,
                                     MsgVaultThoughtSawCatGetUp);
}

/// In the house behind door 6, the child points out Dodonpa's henchmen never
/// attacked: maybe the thieves weren't from Lunpa after all.
static void Event_ChildDoubtsThievesWereFromLunpa(void) {
  VaultInteriorsRevisit_FaceAndSpeak(ACTOR_CHILD,
                                     MsgVaultDodonpasHenchmenNeverAttackedUs);
}

/// In the item shop, the husband would like to see Colosso in Tolbi once more
/// before he dies.
static void Event_HusbandLongsForColosso(void) {
  VaultInteriorsRevisit_FaceAndSpeak(ACTOR_COLOSSO_HUSBAND,
                                     MsgVaultItsNearTimeForColosso);
}

/// In his house, the mayor welcomes Isaac back: the thieves he caught have
/// escaped from jail, and Vault feared they would come after him. Asked
/// whether the party has run into them, a yes and a no get different warnings.
/// Sets FLAG_WELCOMED_BACK, after which the mayor only warns of their revenge,
/// and FLAG_VAULT_WARNED_OF_ESCAPED_THIEVES (0x868).
static void Event_WelcomeIsaacBack(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultIsaacYouveComeBackVault);
  Engine_ActorFaceEachOther(ACTOR_MAYOR, ACTOR_PARTY_LEADER, 2);
  Engine_ActorSetAnimation(ACTOR_MAYOR, ANIM_STAND);
  Event_ShowMessageAndWait(ACTOR_MAYOR, 0, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_MAYOR, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_MAYOR, 0, 20);
  Actor_ShowEmote(ACTOR_MAYOR, EMOTE_IN_FRONT | 2, 60);
  Event_ShowMessageAndWait(ACTOR_MAYOR, 0, 30);
  // The negative answer advances to the next message before continuing.
  Event_OpenMessage(ACTOR_MAYOR, 0);
  if (Engine_EventChooseYesNo(0, 0) != 0) {
    gEventWork->message++;
  }
  Event_ShowMessageAndWait(ACTOR_MAYOR, 0, 20);
  GameFlag_Set(FLAG_WELCOMED_BACK);
  GameFlag_Set(FLAG_VAULT_WARNED_OF_ESCAPED_THIEVES);
  Engine_EventEnd();
}

/// After the welcome: those thieves really wanted revenge, so take care.
static void Event_WarnOfThievesRevenge(void) {
  VaultInteriorsRevisit_FaceAndSpeak(ACTOR_MAYOR,
                                     MsgVaultThoseGuysReallyWantedRevenge);
}

/// The cook has useless bones left from cooking and asks if the party wants
/// some, but gives none. The cook's earlier talk row, a plain message with no
/// condition, always answers first (Scene_FindEvent takes the first match), so
/// this is never shown.
static void Event_OfferLeftoverBones(void) {
  VaultInteriorsRevisit_FaceAndSpeak(ACTOR_COOK, MsgVaultDidSomeCookingNowIve);
}

/// The girl at the inn offers leftover bones from dinner, or more of them once
/// one was taken (FLAG_VAULT_BONE_RECEIVED); a yes gets a Bone if there is
/// room, a no a nod. Her talk row needs FLAG_SCENE_TEMP_50 clear, that is, the
/// leader not standing beside the floor Bone.
static void Event_OfferBones(void) {
  Engine_EventBegin();
  Engine_ActorFaceEachOther(ACTOR_VAULT_BONE_GIVER, ACTOR_PARTY_LEADER, 0);
  if (GameFlag_IsSet(FLAG_VAULT_BONE_RECEIVED) == 0) {
    Engine_EventSetMessage(MsgVaultHaveLotLeftoverBonesFrom);
    Event_OpenMessage(ACTOR_VAULT_BONE_GIVER, 0);
  } else {
    Engine_EventSetMessage(MsgVaultWantMoreBones);
    Event_OpenMessage(ACTOR_VAULT_BONE_GIVER, 0);
  }
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    // Show the offer before checking whether the party can carry the gift.
    Engine_EventWait(20);
    Event_ShowMessage(ACTOR_VAULT_BONE_GIVER, 0);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(ACTOR_VAULT_BONE_GIVER, 2);
    Engine_EventWait(20);
    // Give the Bone only after the offer is accepted and space is available.
    if (PartyInventory_HasSpace() == 0) {
      Engine_ActorSetAnimationAndWait(ACTOR_VAULT_BONE_GIVER, ANIM_SHAKE_HEAD);
      Engine_EventWait(20);
      Engine_EventSetMessage(MsgVaultWowHaveManyThingsArent);
      Event_ShowMessage(ACTOR_VAULT_BONE_GIVER, 0);
    } else {
      Engine_ItemShowFound(ITEM_BONE, 3);
      Engine_PartyGiveItem(ITEM_BONE, 0);
      GameFlag_Set(FLAG_VAULT_BONE_RECEIVED);
    }
  } else {
    // Declining selects the following line and the actor's response pose.
    gEventWork->message++;
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(ACTOR_VAULT_BONE_GIVER, ANIM_NOD);
    Engine_EventWait(20);
    Event_ShowMessage(ACTOR_VAULT_BONE_GIVER, 0);
  }
  Actor_FaceDirection(ACTOR_VAULT_BONE_GIVER, FACING_SOUTH, 0);
  Engine_EventEnd();
}

/// Mind Read on the mayor before the welcome: he notices the party and
/// welcomes Isaac back.
static void Event_ReadMindAndWelcomeIsaacBack(void) {
  u8 *paused;
  u8 unpaused = 0;

  Engine_EventBegin();
  Engine_ActorRunRepeatedMotion(ACTOR_MAYOR, 1);
  Engine_EventEnd();
  // Keep the actor paused through the conversation, then resume action 2.
  paused = &Object_GetById(ACTOR_MAYOR)->paused;
  *paused = 1;
  Event_WelcomeIsaacBack();
  paused = &Object_GetById(ACTOR_MAYOR)->paused;
  *paused = unpaused;
  Actor_SetBuiltinAction(ACTOR_MAYOR, 2);
}

// Shared Bone-item dialogue follows the actor conversations.
#include "common/vault_bone.h"

/// Across the counter the weapon shop opens; from elsewhere the merchant talks
/// of a river of fire on the other side of Mt. Aleph.
static void Event_TalkToWeaponMerchant(void) {
  u32 leaderFacing;

  leaderFacing = Object_GetById(ACTOR_PARTY_LEADER)->facing;
  Engine_EventBegin();

  if (FACING_IS_NORTH(leaderFacing)) {
    Engine_ShopOpen(VAULT_WEAPON_STOCK, ACTOR_VAULT_WEAPON_MERCHANT);
  } else {
    Engine_EventSetMessage(MsgVaultTheresRiverFireOnOther);
    Engine_EventShowMessage(ACTOR_VAULT_WEAPON_MERCHANT, 0);
  }

  Engine_EventEnd();
}

/// Across the counter the armor shop opens; from elsewhere the merchant says
/// the eruption split the continent in two.
static void Event_TalkToArmorMerchant(void) {
  u32 leaderFacing;

  leaderFacing = Object_GetById(ACTOR_PARTY_LEADER)->facing;
  Engine_EventBegin();

  if (FACING_IS_NORTH(leaderFacing)) {
    Engine_ShopOpen(VAULT_ARMOR_STOCK, ACTOR_VAULT_ARMOR_MERCHANT);
  } else {
    Engine_EventSetMessage(MsgVaultTheySayVolcanoOnMt);
    Engine_EventShowMessage(ACTOR_VAULT_ARMOR_MERCHANT, 0);
  }

  Engine_EventEnd();
}

/// Across the counter the item shop opens; from elsewhere the merchant has had
/// few customers lately.
static void Event_TalkToItemMerchant(void) {
  u32 leaderFacing;

  leaderFacing = Object_GetById(ACTOR_PARTY_LEADER)->facing;
  Engine_EventBegin();

  if (FACING_IS_NORTH(leaderFacing)) {
    Engine_ShopOpen(VAULT_ITEM_STOCK, ACTOR_VAULT_ITEM_MERCHANT);
  } else {
    Engine_EventSetMessage(MsgVaultWeHaventBeenGettingMany);
    Engine_EventShowMessage(ACTOR_VAULT_ITEM_MERCHANT, 0);
  }

  Engine_EventEnd();
}

/// Across the counter the inn opens; from elsewhere the innkeeper complains
/// that no one comes, what with the volcano.
static void Event_TalkToInnkeeper(void) {
  u32 leaderFacing;

  leaderFacing = Object_GetById(ACTOR_PARTY_LEADER)->facing;
  Engine_EventBegin();

  if (FACING_IS_NORTH(leaderFacing)) {
    Engine_InnOpen(VAULT_INN, ACTOR_VAULT_INNKEEPER);
  } else {
    Engine_EventSetMessage(MsgVaultMmmmNothingDoNothingDo);
    Engine_EventShowMessage(ACTOR_VAULT_INNKEEPER, 0);
  }

  Engine_EventEnd();
}

/// Mind Read on the girl at the inn: hungry while cooking, or, once a Bone was
/// given, wondering whether "he" has a calcium deficiency.
static void Event_ReadBoneGiverMind(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_VAULT_BONE_RECEIVED) == 0) {
    Engine_EventSetMessage(MsgVaultGeeAlwaysGetHungryWhen);
  } else {
    Engine_EventSetMessage(MsgVaultHeReallyLikesBonesWonder);
  }
  Engine_EventShowMessage(ACTOR_VAULT_BONE_GIVER, 0);
  Engine_EventEnd();
}

/// Returns the talk, Psynergy, search and exit event table.
const struct SceneEvent *Scene_GetEvents(void) {
  return gVaultInteriorsRevisit_Events;
}

/// Opens the room through a window transition. Entering the house behind door
/// 5 sets its owner on the floor and copies a block of cells; entering the inn
/// (entrance 7 or 11) places the floor Bone beside the girl and starts
/// Vault_UpdateBoneProximityFlag.
s32 Scene_Initialize(void) {
  s32 entrance;
  s32 clearedState;

  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  entrance = gGameState.entrance;

  if (entrance == 5) {
    // Restore the lowered actor and replacement cells in this room.
    Map_CopyCellsTo(0, 120, 8, 67, 4, 3);
    clearedState = 0;
    Object_GetById(8)->motionFlags = clearedState;
    Object_GetById(8)->y.fixed = clearedState;
    Object_GetById(8)->terrainHeight = clearedState;
  } else if (entrance == 7 || entrance == 11) {
    // Show the Bone lying in the room, and track whether the leader stands
    // beside it (FLAG_SCENE_TEMP_50).
    Vault_PlaceItemIcon(ITEM_BONE, PIXELS(568), PIXELS(16), PIXELS(672));
    FieldScheduler_AddCallback(Vault_UpdateBoneProximityFlag,
                               TASK_PRIORITY_SCENE);
  }

  return 0;
}

// Shared item-object routines follow the scene initializer.
#include "common/vault_itemobj.h"

struct SceneEntrance gVaultInteriorsRevisit_Entrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {5, CONDITION_ALWAYS, 192, 0, 220, FACING_NORTH, 0, 32, 0, 272, 240, 0},
    {6, CONDITION_ALWAYS, 432, 0, 254, FACING_NORTH, 0, 288, 0, 528, 272, 0},
    {7, CONDITION_ALWAYS, 720, 0, 782, FACING_NORTH, 0, 432, 576, 800, 800, 0},
    {8, CONDITION_ALWAYS, 624, 0, 236, FACING_NORTH, 0, 560, 32, 848, 256, 0},
    {9, CONDITION_ALWAYS, 144, 0, 492, FACING_NORTH, 0, 16, 288, 256, 512, 0},
    {10, CONDITION_ALWAYS, 416, 0, 526, FACING_NORTH, 0, 320, 288, 592, 544, 0},
    {11, CONDITION_ALWAYS, 744, 0, 640, FACING_SOUTH, 0, 432, 576, 800, 800, 0},
    {12, CONDITION_ALWAYS, 358, 0, 632, FACING_SOUTH, 0, 32, 576, 400, 776, 0},
    {SCENE_TABLE_END},
};

u32 gVaultInteriorsRevisit_Exits[] = {
    SCENE_EXITS(VaultInteriorsRevisit),
    SCENE_EXIT(VAULT_INTERIORS_REVISIT_TO_VAULT_REVISIT_SOUTHEAST, VaultRevisit,
               VAULT_REVISIT_FROM_VAULT_INTERIORS_REVISIT_SOUTHEAST),
    SCENE_EXIT(VAULT_INTERIORS_REVISIT_TO_VAULT_REVISIT_SOUTH, VaultRevisit,
               VAULT_REVISIT_FROM_VAULT_INTERIORS_REVISIT_SOUTH),
    SCENE_EXIT(VAULT_INTERIORS_REVISIT_TO_VAULT_REVISIT_SOUTHWEST, VaultRevisit,
               VAULT_REVISIT_FROM_VAULT_INTERIORS_REVISIT_SOUTHWEST),
    SCENE_EXIT(VAULT_INTERIORS_REVISIT_TO_VAULT_REVISIT_EAST, VaultRevisit,
               VAULT_REVISIT_FROM_VAULT_INTERIORS_REVISIT_EAST),
    SCENE_EXIT(VAULT_INTERIORS_REVISIT_TO_VAULT_REVISIT_NORTH, VaultRevisit,
               VAULT_REVISIT_FROM_VAULT_INTERIORS_REVISIT_NORTH),
    SCENE_EXIT(VAULT_INTERIORS_REVISIT_TO_VAULT_REVISIT_NORTHWEST, VaultRevisit,
               VAULT_REVISIT_FROM_VAULT_INTERIORS_REVISIT_NORTHWEST),
    SCENE_EXIT(11, VaultInteriorsRevisit, 11),
    SCENE_EXIT(12, VaultInteriorsRevisit, 12),
    SCENE_EXITS_END,
};

// clang-format off
struct ScenePlacement gVaultInteriorsRevisit_Placements[] = {
    { 0x6c, 0, ACTOR_STAND,
        PIXELS(160), 0, PIXELS(136), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6a, 0, 3,
        PIXELS(152), 0, PIXELS(72), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x65, 0, ACTOR_STAND,
        PIXELS(456), 0, PIXELS(184), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x6b, 0, ACTOR_WANDER,
        PIXELS(368), 0, PIXELS(120), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x67, 0, ACTOR_WANDER,
        PIXELS(744), 0, PIXELS(200), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x6f, 0, ACTOR_STAND,
        PIXELS(752), 0, PIXELS(136), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x6b, 0, ACTOR_STAND,
        PIXELS(496), 0, PIXELS(416), FACING_EAST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x66, 0, ACTOR_STAND,
        PIXELS(512), 0, PIXELS(416), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x75, 0, ACTOR_STAND,
        PIXELS(536), 0, PIXELS(451), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x73, 0, ACTOR_STAND,
        PIXELS(416), 0, PIXELS(464), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0x6a, 0, ACTOR_STAND,
        PIXELS(568), 0, PIXELS(648), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x7c, 0, ACTOR_STAND,
        PIXELS(200), 0, PIXELS(392), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x7d, 0, ACTOR_STAND,
        PIXELS(88), 0, PIXELS(392), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x76, 0, ACTOR_STAND,
        PIXELS(632), 0, PIXELS(136), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x77, 0, ACTOR_STAND,
        PIXELS(696), 0, PIXELS(696), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x80, 0, ACTOR_WANDER,
        PIXELS(248), 0, PIXELS(712), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gVaultInteriorsRevisit_Events[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 5 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 6 },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, 7 },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, 8 },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, 9 },
    { EVENT_EXIT, 6, CONDITION_ALWAYS, 10 },
    { EVENT_EXIT, 7, CONDITION_ALWAYS, 11 },
    { EVENT_EXIT, 8, CONDITION_ALWAYS, 12 },
    { EVENT_TALK, ACTOR_GEM_OWNER, CONDITION_ALWAYS, MSGNO(MsgVaultWaahACatAteMyPrecious) },
    { EVENT_TALK, ACTOR_GEM_OWNERS_WIFE, CONDITION_ALWAYS, EVENT_SCRIPT(Event_WifeSawCatTakeGem) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgVaultThePrisonersSaidTheyWantedTo) },
    { EVENT_TALK, ACTOR_CHILD, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ChildDoubtsThievesWereFromLunpa) },
    { EVENT_TALK, ACTOR_COLOSSO_HUSBAND, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HusbandLongsForColosso) },
    { EVENT_TALK, ACTOR_COLOSSO_WIFE, CONDITION_ALWAYS, MSGNO(MsgVaultTheWarriorsISawFightingAt) },
    { EVENT_TALK, ACTOR_MAYOR, FLAG_WELCOMED_BACK, EVENT_SCRIPT(Event_WelcomeIsaacBack) },
    { EVENT_TALK, ACTOR_MAYOR, CONDITION_ALWAYS, EVENT_SCRIPT(Event_WarnOfThievesRevenge) },
    { EVENT_TALK, ACTOR_MAYORS_WIFE, CONDITION_ALWAYS, MSGNO(MsgVaultTheMayorsFamilyUrnIsOn) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgVaultThoseThievesYouGoToAll) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgVaultIHearThatABigFestival) },
    { EVENT_TALK, ACTOR_VAULT_WEAPON_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToWeaponMerchant) },
    { EVENT_TALK, ACTOR_VAULT_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToArmorMerchant) },
    { EVENT_TALK, ACTOR_VAULT_ITEM_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToItemMerchant) },
    { EVENT_TALK, ACTOR_VAULT_INNKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToInnkeeper) },
    { EVENT_TALK, ACTOR_COOK, CONDITION_ALWAYS, MSGNO(MsgVaultWeDontGetALotOf) },
    { EVENT_TALK, ACTOR_COOK, CONDITION_ALWAYS, EVENT_SCRIPT(Event_OfferLeftoverBones) },
    { EVENT_TALK, ACTOR_VAULT_BONE_GIVER, FLAG_SCENE_TEMP_50, EVENT_SCRIPT(Event_OfferBones) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GEM_OWNER, CONDITION_ALWAYS, MSGNO(MsgVaultIfIdKnownThisWasGoing) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GEM_OWNERS_WIFE, CONDITION_ALWAYS, MSGNO(MsgVaultMyHusbandSaysMyCatAte) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgVaultWhyDoYouSupposeThoseThieves) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHILD, CONDITION_ALWAYS, MSGNO(MsgVaultDaddyHasntBeenPayingAttentionSince) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_COLOSSO_HUSBAND, CONDITION_ALWAYS, MSGNO(MsgVaultWheneverMyWifeUsedToGo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_COLOSSO_WIFE, CONDITION_ALWAYS, MSGNO(MsgVaultLatelyMyHusbandsBeenGoingOn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x410, FLAG_WELCOMED_BACK, EVENT_SCRIPT(Event_ReadMindAndWelcomeIsaacBack) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAYOR, CONDITION_ALWAYS, MSGNO(MsgVaultAsLongAsWeStayAlert) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAYORS_WIFE, CONDITION_ALWAYS, MSGNO(MsgVaultWellBeSecuringItInA) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgVaultItsJustEmbarrassingThatWeLet) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgVaultILoveFestivalsIllBetColosso) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VAULT_WEAPON_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgVaultJustImagineHowTerribleItWould) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VAULT_ARMOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgVaultItsAGoodThingThatGiant) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VAULT_ITEM_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgVaultWhyKeepTheStoreOpenIf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VAULT_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgVaultWontAnyoneComeStayTheNight) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_COOK, CONDITION_ALWAYS, MSGNO(MsgVaultAfterYouveBeenInThisBusiness) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VAULT_BONE_GIVER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadBoneGiverMind) },
    { SEARCH(SEARCH_UNNAMED), 41, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_FindBone) },
    { SEARCH(SEARCH_JAR), 100, FLAG_VAULT_INTERIORS_JAR_COINS, FIND_COINS(7) },
    { SEARCH(SEARCH_BARREL), 101, FLAG_VAULT_INTERIORS_BARREL_COINS, FIND_COINS(4) },
    { SEARCH(SEARCH_BARREL), 102, FLAG_VAULT_INTERIORS_BARREL_SLEEP_BOMB, FIND_ITEM(ITEM_SLEEP_BOMB) },
    { SEARCH(SEARCH_WOODEN_BOX), 103, FLAG_VAULT_INTERIORS_BOX_MINT, FIND_ITEM(ITEM_MINT) },
    { 0xc4f3, 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgVaultTheresABookEntitledHistoryOf)) },
    { SEARCH(SEARCH_BOOKCASE), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgVaultTheresABookEntitledGemsOfAngara)) },
    { SCENE_EVENTS_END },
};
// clang-format on
