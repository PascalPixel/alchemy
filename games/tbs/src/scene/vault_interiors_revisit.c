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

// Shared scene routines stay in their linked order.
#include "common/vault_spotflag.h"

// Returns this scene's entrance table through the overlay's byte-pointer API.
u8 *SceneData_GetScriptTable(void) { return (u8 *)gPlacement_Scripts; }

// This scene has no region table.
s32 SceneData_ReturnZero(void) { return 0; }

// Returns the exits to the surrounding Vault map.
u8 *SceneData_GetMessageTable(void) { return (u8 *)gPlacement_Messages; }

// Applies view conditions to the placements before the field spawns them.
u8 *SceneData_GetPreparedActors(void) {
  struct ScenePlacement *placements = gPlacement_Actors;

  FieldScene_PrepareActors(placements);
  return (u8 *)placements;
}

// Shows the actor's selected message after selecting animation 1.
static void Vault_ShowActorDialogue(s32 actor) {
  Engine_EventBegin();
  Engine_ActorSetAnimation(actor, 1);
  Engine_EventShowMessage(actor, 0);
  Engine_EventEnd();
}

// The resident thinks the cat stood up.
static void EV09_TALK(void) {
  Engine_EventSetMessage(MsgVaultThoughtSawCatGetUp);
  Engine_ActorFaceEachOther(9, ACTOR_PARTY_LEADER, 2);
  Vault_ShowActorDialogue(9);
}

// The resident says Dodonpa's henchmen never attacked Vault.
static void EV11_TALK(void) {
  Engine_EventSetMessage(MsgVaultDodonpasHenchmenNeverAttackedUs);
  Engine_ActorFaceEachOther(11, ACTOR_PARTY_LEADER, 2);
  Vault_ShowActorDialogue(11);
}

// The resident talks about the approaching Colosso tournament.
static void EV12_TALK(void) {
  Engine_EventSetMessage(MsgVaultItsNearTimeForColosso);
  Engine_ActorFaceEachOther(12, ACTOR_PARTY_LEADER, 2);
  Vault_ShowActorDialogue(12);
}

// Greets Isaac's returning party and records the conversation.
static void Vault_GreetReturningParty(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgVaultIsaacYouveComeBackVault);
  Engine_ActorFaceEachOther(16, ACTOR_PARTY_LEADER, 2);
  Engine_ActorSetAnimation(16, 1);
  Event_ShowMessageAndWait(16, 0, 20);
  Engine_ActorSetAnimationAndWait(16, 4);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(16, 0, 20);
  Actor_ShowEmote(16, 0x102, 60);
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
static void Vault_DiscussThievesRevenge(void) {
  Engine_EventSetMessage(MsgVaultThoseGuysReallyWantedRevenge);
  Engine_ActorFaceEachOther(16, ACTOR_PARTY_LEADER, 2);
  Vault_ShowActorDialogue(16);
}

// The resident talks about cooking.
static void EV23_TALK(void) {
  Engine_EventSetMessage(MsgVaultDidSomeCookingNowIve);
  Engine_ActorFaceEachOther(23, ACTOR_PARTY_LEADER, 2);
  Vault_ShowActorDialogue(23);
}

// Offers a Bone, giving it only if the party accepts and has inventory space.
static void EV18_TALK(void) {
  Engine_EventBegin();
  Engine_ActorFaceEachOther(18, ACTOR_PARTY_LEADER, 0);
  if (GameFlag_IsSet(0x85b) == 0) {
    Engine_EventSetMessage(MsgVaultHaveLotLeftoverBonesFrom);
    Event_OpenMessage(18, 0);
  } else {
    Engine_EventSetMessage(MsgVaultWantMoreBones);
    Event_OpenMessage(18, 0);
  }
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    // Show the offer before checking whether the party can carry the gift.
    Engine_EventWait(20);
    Event_ShowMessage(18, 0);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(18, 2);
    Engine_EventWait(20);
    if (PartyInventory_HasSpace() == 0) {
      Engine_ActorSetAnimationAndWait(18, 4);
      Engine_EventWait(20);
      Engine_EventSetMessage(MsgVaultWowHaveManyThingsArent);
      Event_ShowMessage(18, 0);
    } else {
      Engine_ItemShowFound(ITEM_BONE, 3);
      Engine_PartyGiveItem(ITEM_BONE, 0);
      GameFlag_Set(0x85b);
    }
  } else {
    // Declining selects the following line and the actor's response pose.
    gEventWork->message++;
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(18, 3);
    Engine_EventWait(20);
    Event_ShowMessage(18, 0);
  }
  Actor_FaceDirection(18, FACING_SOUTH, 0);
  Engine_EventEnd();
}

// Mind Read on actor 16 can lead into the return greeting.
static void EV1040_PSYNERGY(void) {
  u8 *paused;
  u8 clear = 0;

  Engine_EventBegin();
  Engine_ActorRunRepeatedMotion(16, 1);
  Engine_EventEnd();
  // Keep the actor paused through the conversation, then resume action 2.
  paused = &Object_GetById(16)->paused;
  *paused = 1;
  Vault_GreetReturningParty();
  paused = &Object_GetById(16)->paused;
  *paused = clear;
  Actor_SetBuiltinAction(16, 2);
}

// Shared Bone-item dialogue follows the actor conversations.
#include "common/vault_bone.h"

// Opens shop 4 when approached facing north; otherwise shows local news.
static void EV19_TALK(void) {
  u32 leaderFacing;

  leaderFacing = Object_GetById(ACTOR_PARTY_LEADER)->facing;
  Engine_EventBegin();

  if (FACING_IS_NORTH(leaderFacing)) {
    Engine_ShopOpen(4, 19);
  } else {
    Engine_EventSetMessage(MsgVaultTheresRiverFireOnOther);
    Engine_EventShowMessage(19, 0);
  }

  Engine_EventEnd();
}

// Opens shop 5 when approached facing north; otherwise discusses Mt. Aleph.
static void EV20_TALK(void) {
  u32 leaderFacing;

  leaderFacing = Object_GetById(ACTOR_PARTY_LEADER)->facing;
  Engine_EventBegin();

  if (FACING_IS_NORTH(leaderFacing)) {
    Engine_ShopOpen(5, 20);
  } else {
    Engine_EventSetMessage(MsgVaultTheySayVolcanoOnMt);
    Engine_EventShowMessage(20, 0);
  }

  Engine_EventEnd();
}

// Opens shop 6 when approached facing north; otherwise shows its dialogue.
static void EV21_TALK(void) {
  u32 leaderFacing;

  leaderFacing = Object_GetById(ACTOR_PARTY_LEADER)->facing;
  Engine_EventBegin();

  if (FACING_IS_NORTH(leaderFacing)) {
    Engine_ShopOpen(6, 21);
  } else {
    Engine_EventSetMessage(MsgVaultWeHaventBeenGettingMany);
    Engine_EventShowMessage(21, 0);
  }

  Engine_EventEnd();
}

// Opens inn 1 when approached facing north; otherwise shows its dialogue.
static void EV22_TALK(void) {
  u32 leaderFacing;

  leaderFacing = Object_GetById(ACTOR_PARTY_LEADER)->facing;
  Engine_EventBegin();

  if (FACING_IS_NORTH(leaderFacing)) {
    Engine_InnOpen(1, 22);
  } else {
    Engine_EventSetMessage(MsgVaultMmmmNothingDoNothingDo);
    Engine_EventShowMessage(22, 0);
  }

  Engine_EventEnd();
}

// Mind Read changes after the resident has given the party a Bone.
static void EV18_PSYNERGY(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(0x85B) == 0) {
    Engine_EventSetMessage(MsgVaultGeeAlwaysGetHungryWhen);
  } else {
    Engine_EventSetMessage(MsgVaultHeReallyLikesBonesWonder);
  }
  Engine_EventShowMessage(18, 0);
  Engine_EventEnd();
}

// Returns the talk, Psynergy, search and exit event table.
u8 *SceneData_GetEffectTable(void) { return (u8 *)gPlacement_Effects; }

// Initializes the selected interior: lowers actor 8 or starts the spot check.
s32 FieldScene_InitSceneStateByStep(void) {
  s32 entrance;
  s32 zero;

  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  entrance = gGameState.entrance;

  if (entrance == 5) {

    Map_CopyCellsTo(0, 120, 8, 67, 4, 3);
    zero = 0;
    Object_GetById(8)->motionFlags = zero;
    Object_GetById(8)->y.fixed = zero;
    Object_GetById(8)->terrainHeight = zero;
  } else if (entrance == 7 || entrance == 11) {
    OverlayObject_ConfigureObject22WithResource17(0xe7, PIXELS(568), PIXELS(16),
                                                  PIXELS(672));
    FieldScheduler_AddCallback(SceneState_SetFlagByActorPosition, 0xc80);
  }

  return 0;
}

// Shared item-object routines follow the scene initializer.
#include "common/vault_itemobj.h"

struct SceneEntrance gPlacement_Scripts[] = {
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

u32 gPlacement_Messages[] = {
    SCENE_EXITS(24),        SCENE_EXIT(5, 23, 5),   SCENE_EXIT(6, 23, 6),
    SCENE_EXIT(7, 23, 7),   SCENE_EXIT(8, 23, 8),   SCENE_EXIT(9, 23, 9),
    SCENE_EXIT(10, 23, 10), SCENE_EXIT(11, 24, 11), SCENE_EXIT(12, 24, 12),
    SCENE_EXITS_END,
};

// clang-format off
struct ScenePlacement gPlacement_Actors[] = {
    { 0x6c, 0x0, ACTOR_STAND,
        PIXELS(160), 0, PIXELS(136), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6a, 0x0, 3,
        PIXELS(152), 0, PIXELS(72), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x65, 0x0, ACTOR_STAND,
        PIXELS(456), 0, PIXELS(184), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x6b, 0x0, ACTOR_WANDER,
        PIXELS(368), 0, PIXELS(120), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x67, 0x0, ACTOR_WANDER,
        PIXELS(744), 0, PIXELS(200), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x6f, 0x0, ACTOR_STAND,
        PIXELS(752), 0, PIXELS(136), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x6b, 0x0, ACTOR_STAND,
        PIXELS(496), 0, PIXELS(416), FACING_EAST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x66, 0x0, ACTOR_STAND,
        PIXELS(512), 0, PIXELS(416), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x75, 0x0, ACTOR_STAND,
        PIXELS(536), 0, PIXELS(451), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x73, 0x0, ACTOR_STAND,
        PIXELS(416), 0, PIXELS(464), FACING_NORTH, TALK_FACE_PARTY, 0 },
    { 0x6a, 0x0, ACTOR_STAND,
        PIXELS(568), 0, PIXELS(648), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x7c, 0x0, ACTOR_STAND,
        PIXELS(200), 0, PIXELS(392), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x7d, 0x0, ACTOR_STAND,
        PIXELS(88), 0, PIXELS(392), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x76, 0x0, ACTOR_STAND,
        PIXELS(632), 0, PIXELS(136), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x77, 0x0, ACTOR_STAND,
        PIXELS(696), 0, PIXELS(696), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x80, 0x0, ACTOR_WANDER,
        PIXELS(248), 0, PIXELS(712), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gPlacement_Effects[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 5 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 6 },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, 7 },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, 8 },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, 9 },
    { EVENT_EXIT, 6, CONDITION_ALWAYS, 10 },
    { EVENT_EXIT, 7, CONDITION_ALWAYS, 11 },
    { EVENT_EXIT, 8, CONDITION_ALWAYS, 12 },
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgVaultWaahACatAteMyPrecious) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, EVENT_SCRIPT(EV09_TALK) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgVaultThePrisonersSaidTheyWantedTo) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, EVENT_SCRIPT(EV11_TALK) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, EVENT_SCRIPT(EV12_TALK) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgVaultTheWarriorsISawFightingAt) },
    { EVENT_TALK, 16, 0x300, EVENT_SCRIPT(Vault_GreetReturningParty) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, EVENT_SCRIPT(Vault_DiscussThievesRevenge) },
    { EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgVaultTheMayorsFamilyUrnIsOn) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgVaultThoseThievesYouGoToAll) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgVaultIHearThatABigFestival) },
    { EVENT_TALK, 19, CONDITION_ALWAYS, EVENT_SCRIPT(EV19_TALK) },
    { EVENT_TALK, 20, CONDITION_ALWAYS, EVENT_SCRIPT(EV20_TALK) },
    { EVENT_TALK, 21, CONDITION_ALWAYS, EVENT_SCRIPT(EV21_TALK) },
    { EVENT_TALK, 22, CONDITION_ALWAYS, EVENT_SCRIPT(EV22_TALK) },
    { EVENT_TALK, 23, CONDITION_ALWAYS, MSGNO(MsgVaultWeDontGetALotOf) },
    { EVENT_TALK, 23, CONDITION_ALWAYS, EVENT_SCRIPT(EV23_TALK) },
    { EVENT_TALK, 18, 0x250, EVENT_SCRIPT(EV18_TALK) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgVaultIfIdKnownThisWasGoing) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgVaultMyHusbandSaysMyCatAte) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgVaultWhyDoYouSupposeThoseThieves) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgVaultDaddyHasntBeenPayingAttentionSince) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgVaultWheneverMyWifeUsedToGo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgVaultLatelyMyHusbandsBeenGoingOn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x410, 0x300, EVENT_SCRIPT(EV1040_PSYNERGY) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgVaultAsLongAsWeStayAlert) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgVaultWellBeSecuringItInA) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgVaultItsJustEmbarrassingThatWeLet) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgVaultILoveFestivalsIllBetColosso) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgVaultJustImagineHowTerribleItWould) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgVaultItsAGoodThingThatGiant) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgVaultWhyKeepTheStoreOpenIf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgVaultWontAnyoneComeStayTheNight) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS, MSGNO(MsgVaultAfterYouveBeenInThisBusiness) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, EVENT_SCRIPT(EV18_PSYNERGY) },
    { SEARCH(SEARCH_UNNAMED), 41, CONDITION_ALWAYS, EVENT_SCRIPT(EV41_SEARCH) },
    { SEARCH(SEARCH_JAR), 100, 0xf4b, FIND_COINS(7) },
    { SEARCH(SEARCH_BARREL), 101, 0xf4c, FIND_COINS(4) },
    { SEARCH(SEARCH_BARREL), 102, 0xf4d, FIND_ITEM(ITEM_SLEEP_BOMB) },
    { SEARCH(SEARCH_WOODEN_BOX), 103, 0xf4e, FIND_ITEM(0xc3) },
    { 0xc4f3, 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgVaultTheresABookEntitledHistoryOf)) },
    { SEARCH(SEARCH_BOOKCASE), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgVaultTheresABookEntitledGemsOfAngara)) },
    { SCENE_EVENTS_END },
};
// clang-format on
