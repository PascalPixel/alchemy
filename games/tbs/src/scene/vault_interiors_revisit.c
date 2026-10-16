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

enum {
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

// Shared scene routines stay in their linked order.
#include "common/vault_spotflag.h"

// Return the arrival positions for the revisited houses and shops.
const struct SceneEntrance *VaultInteriorsRevisit_GetEntrances(void) {
  return gVaultInteriorsRevisit_Entrances;
}

// This scene has no region table.
const struct SceneRegion *VaultInteriorsRevisit_GetRegions(void) {
  return NULL;
}

// Returns the exits to the surrounding Vault map.
const u32 *VaultInteriorsRevisit_GetExits(void) {
  return gVaultInteriorsRevisit_Exits;
}

// Applies view conditions to the placements before the field spawns them.
const struct ScenePlacement *VaultInteriorsRevisit_GetPlacements(void) {
  struct ScenePlacement *placements = gVaultInteriorsRevisit_Placements;

  FieldScene_PrepareActors(placements);
  return placements;
}

// Shows the actor's selected message after selecting animation 1.
static void VaultInteriorsRevisit_ShowStandingActorDialogue(s32 actor) {
  Engine_EventBegin();
  Engine_ActorSetAnimation(actor, ANIM_STAND);
  Engine_EventShowMessage(actor, 0);
  Engine_EventEnd();
}

// Face the leader and show this resident's dialogue while standing still.
static inline void VaultInteriorsRevisit_FaceAndSpeak(s32 actor, s32 message) {
  Engine_EventSetMessage(message);
  Engine_ActorFaceEachOther(actor, ACTOR_PARTY_LEADER, 2);
  VaultInteriorsRevisit_ShowStandingActorDialogue(actor);
}

// The resident thinks the cat stood up.
static void VaultInteriorsRevisit_DiscussCatStandingUp(void) {
  VaultInteriorsRevisit_FaceAndSpeak(9, MsgVaultThoughtSawCatGetUp);
}

// The resident says Dodonpa's henchmen never attacked Vault.
static void VaultInteriorsRevisit_DiscussDodonpasHenchmen(void) {
  VaultInteriorsRevisit_FaceAndSpeak(11,
                                     MsgVaultDodonpasHenchmenNeverAttackedUs);
}

// The resident talks about the approaching Colosso tournament.
static void VaultInteriorsRevisit_DiscussColosso(void) {
  VaultInteriorsRevisit_FaceAndSpeak(12, MsgVaultItsNearTimeForColosso);
}

// Greets Isaac's returning party and records the conversation.
static void VaultInteriorsRevisit_GreetReturningParty(void) {
  Engine_EventBegin();
  // Welcome Isaac home before asking about the thieves.
  Engine_EventSetMessage(MsgVaultIsaacYouveComeBackVault);
  Engine_ActorFaceEachOther(16, ACTOR_PARTY_LEADER, 2);
  Engine_ActorSetAnimation(16, 1);
  Event_ShowMessageAndWait(16, 0, 20);
  Engine_ActorSetAnimationAndWait(16, ANIM_SHAKE_HEAD);
  Engine_EventWait(ACTOR_VAULT_ARMOR_MERCHANT);
  Event_ShowMessageAndWait(16, 0, 20);
  Actor_ShowEmote(16, EMOTE_IN_FRONT | 2, 60);
  Event_ShowMessageAndWait(16, 0, 30);
  // The negative answer advances to the next message before continuing.
  Event_OpenMessage(16, 0);
  if (Engine_EventChooseYesNo(0, 0) != 0) {
    gEventWork->message++;
  }
  Event_ShowMessageAndWait(16, 0, 20);
  GameFlag_Set(0x300);
  GameFlag_Set(0x868);
  Engine_EventEnd();
}

// After the greeting, the resident discusses the thieves' revenge.
static void VaultInteriorsRevisit_DiscussThievesRevenge(void) {
  VaultInteriorsRevisit_FaceAndSpeak(16, MsgVaultThoseGuysReallyWantedRevenge);
}

// The resident talks about cooking.
static void VaultInteriorsRevisit_DiscussCooking(void) {
  VaultInteriorsRevisit_FaceAndSpeak(23, MsgVaultDidSomeCookingNowIve);
}

// Offers a Bone, giving it only if the party accepts and has inventory space.
static void VaultInteriorsRevisit_OfferBone(void) {
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
    Engine_EventWait(ACTOR_VAULT_ARMOR_MERCHANT);
    Event_ShowMessage(ACTOR_VAULT_BONE_GIVER, 0);
    Engine_EventWait(ACTOR_VAULT_ARMOR_MERCHANT);
    Engine_ActorRunRepeatedMotion(ACTOR_VAULT_BONE_GIVER, 2);
    Engine_EventWait(ACTOR_VAULT_ARMOR_MERCHANT);
    // Give the Bone only after the offer is accepted and space is available.
    if (PartyInventory_HasSpace() == 0) {
      Engine_ActorSetAnimationAndWait(ACTOR_VAULT_BONE_GIVER, ANIM_SHAKE_HEAD);
      Engine_EventWait(ACTOR_VAULT_ARMOR_MERCHANT);
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
    Engine_EventWait(ACTOR_VAULT_ARMOR_MERCHANT);
    Engine_ActorSetAnimationAndWait(ACTOR_VAULT_BONE_GIVER, ANIM_NOD);
    Engine_EventWait(ACTOR_VAULT_ARMOR_MERCHANT);
    Event_ShowMessage(ACTOR_VAULT_BONE_GIVER, 0);
  }
  Actor_FaceDirection(ACTOR_VAULT_BONE_GIVER, FACING_SOUTH, 0);
  Engine_EventEnd();
}

// Mind Read on actor 16 can lead into the return greeting.
static void VaultInteriorsRevisit_GreetPartyDuringMindRead(void) {
  u8 *paused;
  u8 unpaused = 0;

  Engine_EventBegin();
  Engine_ActorRunRepeatedMotion(16, 1);
  Engine_EventEnd();
  // Keep the actor paused through the conversation, then resume action 2.
  paused = &Object_GetById(16)->paused;
  *paused = 1;
  VaultInteriorsRevisit_GreetReturningParty();
  paused = &Object_GetById(16)->paused;
  *paused = unpaused;
  Actor_SetBuiltinAction(16, 2);
}

// Shared Bone-item dialogue follows the actor conversations.
#include "common/vault_bone.h"

// Opens shop 4 when approached facing north; otherwise shows local news.
static void VaultInteriorsRevisit_TalkToWeaponMerchant(void) {
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

// Opens shop 5 when approached facing north; otherwise discusses Mt. Aleph.
static void VaultInteriorsRevisit_TalkToArmorMerchant(void) {
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

// Opens shop 6 when approached facing north; otherwise shows its dialogue.
static void VaultInteriorsRevisit_TalkToItemMerchant(void) {
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

// Opens inn 1 when approached facing north; otherwise shows its dialogue.
static void VaultInteriorsRevisit_TalkToInnkeeper(void) {
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

// Mind Read changes after the resident has given the party a Bone.
static void VaultInteriorsRevisit_ReadBoneGiversThoughts(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_VAULT_BONE_RECEIVED) == 0) {
    Engine_EventSetMessage(MsgVaultGeeAlwaysGetHungryWhen);
  } else {
    Engine_EventSetMessage(MsgVaultHeReallyLikesBonesWonder);
  }
  Engine_EventShowMessage(ACTOR_VAULT_BONE_GIVER, 0);
  Engine_EventEnd();
}

// Returns the talk, Psynergy, search and exit event table.
const struct SceneEvent *VaultInteriorsRevisit_GetEvents(void) {
  return gVaultInteriorsRevisit_Events;
}

// Initializes the selected interior: lowers actor 8 or starts the spot check.
s32 VaultInteriorsRevisit_Initialize(void) {
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
    // Keep the Bone offer conditional on the leader's position in the room.
    OverlayObject_ConfigureObject22WithResource17(0xe7, PIXELS(568), PIXELS(16),
                                                  PIXELS(672));
    FieldScheduler_AddCallback(Vault_UpdateBoneProximityFlag, 0xc80);
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
    SCENE_EXITS(24),        SCENE_EXIT(5, 23, 5),   SCENE_EXIT(6, 23, 6),
    SCENE_EXIT(7, 23, 7),   SCENE_EXIT(8, 23, 8),   SCENE_EXIT(9, 23, 9),
    SCENE_EXIT(10, 23, 10), SCENE_EXIT(11, 24, 11), SCENE_EXIT(12, 24, 12),
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
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgVaultWaahACatAteMyPrecious) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, EVENT_SCRIPT(VaultInteriorsRevisit_DiscussCatStandingUp) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgVaultThePrisonersSaidTheyWantedTo) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, EVENT_SCRIPT(VaultInteriorsRevisit_DiscussDodonpasHenchmen) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, EVENT_SCRIPT(VaultInteriorsRevisit_DiscussColosso) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgVaultTheWarriorsISawFightingAt) },
    { EVENT_TALK, 16, 0x300, EVENT_SCRIPT(VaultInteriorsRevisit_GreetReturningParty) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, EVENT_SCRIPT(VaultInteriorsRevisit_DiscussThievesRevenge) },
    { EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgVaultTheMayorsFamilyUrnIsOn) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgVaultThoseThievesYouGoToAll) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgVaultIHearThatABigFestival) },
    { EVENT_TALK, 19, CONDITION_ALWAYS, EVENT_SCRIPT(VaultInteriorsRevisit_TalkToWeaponMerchant) },
    { EVENT_TALK, 20, CONDITION_ALWAYS, EVENT_SCRIPT(VaultInteriorsRevisit_TalkToArmorMerchant) },
    { EVENT_TALK, 21, CONDITION_ALWAYS, EVENT_SCRIPT(VaultInteriorsRevisit_TalkToItemMerchant) },
    { EVENT_TALK, 22, CONDITION_ALWAYS, EVENT_SCRIPT(VaultInteriorsRevisit_TalkToInnkeeper) },
    { EVENT_TALK, 23, CONDITION_ALWAYS, MSGNO(MsgVaultWeDontGetALotOf) },
    { EVENT_TALK, 23, CONDITION_ALWAYS, EVENT_SCRIPT(VaultInteriorsRevisit_DiscussCooking) },
    { EVENT_TALK, 18, FLAG_SCENE_TEMP_50, EVENT_SCRIPT(VaultInteriorsRevisit_OfferBone) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgVaultIfIdKnownThisWasGoing) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgVaultMyHusbandSaysMyCatAte) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgVaultWhyDoYouSupposeThoseThieves) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgVaultDaddyHasntBeenPayingAttentionSince) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgVaultWheneverMyWifeUsedToGo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgVaultLatelyMyHusbandsBeenGoingOn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x410, 0x300, EVENT_SCRIPT(VaultInteriorsRevisit_GreetPartyDuringMindRead) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgVaultAsLongAsWeStayAlert) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgVaultWellBeSecuringItInA) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgVaultItsJustEmbarrassingThatWeLet) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgVaultILoveFestivalsIllBetColosso) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgVaultJustImagineHowTerribleItWould) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgVaultItsAGoodThingThatGiant) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgVaultWhyKeepTheStoreOpenIf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgVaultWontAnyoneComeStayTheNight) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS, MSGNO(MsgVaultAfterYouveBeenInThisBusiness) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, EVENT_SCRIPT(VaultInteriorsRevisit_ReadBoneGiversThoughts) },
    { SEARCH(SEARCH_UNNAMED), 41, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_FindBone) },
    { SEARCH(SEARCH_JAR), 100, 0xf4b, FIND_COINS(7) },
    { SEARCH(SEARCH_BARREL), 101, 0xf4c, FIND_COINS(4) },
    { SEARCH(SEARCH_BARREL), 102, 0xf4d, FIND_ITEM(ITEM_SLEEP_BOMB) },
    { SEARCH(SEARCH_WOODEN_BOX), 103, 0xf4e, FIND_ITEM(ITEM_MINT) },
    { 0xc4f3, 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgVaultTheresABookEntitledHistoryOf)) },
    { SEARCH(SEARCH_BOOKCASE), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgVaultTheresABookEntitledGemsOfAngara)) },
    { SCENE_EVENTS_END },
};
// clang-format on
