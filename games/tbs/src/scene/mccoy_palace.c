/// @file
/// McCoy's Palace (SceneId_McCoyPalace), the palace of Lord McCoy, the lord
/// of Bilibin. The grounds outside its gate (SceneId_McCoyPalaceGrounds,
/// mccoy_palace_grounds.c) have the same area name, and the party comes in
/// from there (MCCOY_PALACE_GROUNDS_TO_MCCOY_PALACE, arriving at
/// MCCOY_PALACE_FROM_MCCOY_PALACE_GROUNDS_WEST;
/// MCCOY_PALACE_TO_MCCOY_PALACE_GROUNDS_WEST leads back out).
///
/// Where it falls in the story: the people of Kolima have been turned into
/// trees by the curse of the holy tree, which McCoy had cut down to build his
/// wife, Lady McCoy, a grand palace. McCoy has hired warrior after warrior to
/// end the curse, and not one has come back. His gate guards let the party
/// in, and the palace's people follow the story through three flags:
/// FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY (McCoy has turned the party away),
/// FLAG_KOLIMA_CURSE_LIFTED and FLAG_MCCOY_PALACE_REWARD_TAKEN, which change
/// what each of them says and how they turn when talked to.
///
/// The first audience. The red carpet leads to McCoy's door (trigger 9,
/// MCCOY_PALACE_HALL_TO_MCCOYS_ROOM to
/// MCCOY_PALACE_FROM_MCCOY_PALACE_GROUNDS_EAST, which
/// MCCOY_PALACE_GROUNDS_TO_MCCOY_PALACE_EAST also reaches), and arriving there
/// before the curse is lifted runs Event_AudienceWithMcCoy: McCoy's captain
/// announces the party, and McCoy, seeing children, asks whether they can save
/// Kolima. Refusing, the party is sent away (FLAG_MCCOY_PALACE_JOB_REFUSED) and
/// may come back to change their minds. Accepting, McCoy tosses them the key to
/// the barricades to the east, Garet bristles at being called children, and
/// McCoy decides that he cannot send such wee lads to their doom after all: he
/// takes the key back and his captain sees them out (place flag
/// FLAG_PLACE_TEMP_21). Outside McCoy's door the captain catches up with them
/// (Event_CaptainHintsAtBarricade), asks them to promise not to go to Kolima
/// and lets slip that the barricade may not need a key, which sets
/// FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY.
///
/// The reward. Once the curse is lifted, the gate guards arrest the party and
/// bring them in by MCCOY_PALACE_FROM_MCCOY_PALACE_GROUNDS_EAST again, and
/// Event_McCoyRewardsParty runs until the party has been rewarded
/// (FLAG_MCCOY_PALACE_PARTY_REWARDED): McCoy asks their names, and Garet, Ivan
/// and Mia introduce themselves; his lines differ as he has met them before or
/// not (the barricade hint flag); he thanks them, admits he relied on his money
/// and forgot his people's needs, and has the captain take them to the treasure
/// room (MCCOY_PALACE_HALL_TO_TREASURE_ROOM, place flag FLAG_PLACE_TEMP_22).
/// There the captain and the soldier who keeps the treasure present four chests
/// (Event_PresentRewardChests, FLAG_SCENE_TEMP_02 while choosing): searching
/// one gives its Vial, Potion, Psy Crystal or Water of Life (FLAG_SCENE_TEMP_10
/// to _13 record which); afterwards the chosen chest is empty and the others
/// locked. Walking out without a reward (trigger 51), the captain walks the
/// leader back; with it (trigger 50), he sees the party off
/// (FLAG_MCCOY_PALACE_PARTY_REWARDED, place flag FLAG_PLACE_TEMP_33).
///
/// Elsewhere in the palace: a woman with Lady McCoy's silk nightgown dreams of
/// sleeping in one, and after the reward screams when spoken to ("AAAAH!!!");
/// an old man who praises the party's courage (place flags FLAG_PLACE_TEMP_02
/// and FLAG_PLACE_TEMP_03 mark the first word and the first Mind Read); two
/// barrels hold an Elixir and a Smoke Bomb; a wardrobe holds a silk negligee
/// and a bookcase the list of McCoy's gifts to his wife. McCoy speaks with his
/// portrait (SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY). FLAG_SCENE_TEMP_00
/// records that the captain has opened McCoy's door.

#include "game_state.h"
#include "scene_ids.h"
#include "unit/scene/mccoy_palace.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "unit/runtime/runtime.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "flags.h"
#include "character_ids.h"
#include "types.h"
#include "call.h"
#include "field_event.h"
#include "field_service.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "field_scene.h"
#include "item_ids.h"
#include "objcmd.h"
#include "facing_object.h"
#include "fixed_math.h"
#include "songs.h"

/// The palace's people and the treasure room's chests, numbering
/// gMcCoyPalacePlacements' rows from ACTOR_FIRST_PLACED. Each note gives what
/// they say before the reward, then after it.
enum McCoyPalaceActor {
  /// A burly grey-haired man; points the way to McCoy. "If you want to see Lord
  /// McCoy, follow the red carpet."; later "The people of Bilibin will never
  /// forget your bravery."
  ACTOR_VILLAGER_FOLLOW_RED_CARPET = ACTOR_FIRST_PLACED,
  /// A woman in a green dress; warns the party about McCoy. "Make sure you
  /// don't offend Lord McCoy!"; later "I heard you received a spectacular
  /// reward."
  /// ★ Named from the game: "Make sure you don't offend Lord McCoy!"
  /// (MsgBilibinMakeSureYouDontOffendLord)
  ACTOR_VILLAGER_DO_NOT_OFFEND_MCCOY,
  /// A woman with fair hair.
  /// ★ Named from the game: "Oh, are you willing to go to Kolima Forest?"
  /// (MsgBilibinYouWillingGoKolimaForest)
  ACTOR_VILLAGER_ASKING_ABOUT_KOLIMA_FOREST,
  /// A young man with brown hair; speaks of the old tree in Kolima Forest. "I
  /// always
  /// believed that the old tree in Kolima Forest should be protected!"; later
  /// "Things are looking grim south of Goma Range..."
  ACTOR_YOUNG_MAN_PROTECT_OLD_TREE,
  /// A woman with fair hair standing in Lady McCoy's room. "This is Lady
  /// McCoy's room, so don't touch anything."; later "Milady was very upset when
  /// she left the room."
  ACTOR_VILLAGER_LADY_MCCOYS_ROOM,
  /// A woman with fair hair who admires Lady McCoy. "Milady was renowned far
  /// and wide for her beauty!"; later "For once, milord didn't let Lady McCoy
  /// push him around!"
  ACTOR_VILLAGER_MILADYS_BEAUTY,
  /// A woman with fair hair; screams when spoken to ("AAAAH!!!"); Mind Read:
  /// "Oh, no! This nightgown has a hole... Milady will be furious!"
  /// ★ Named from the game: "Oh, someday, I want to sleep in a silk nightgown
  /// like this." (MsgBilibinOhSomedayIWantToSleep)
  ACTOR_VILLAGER_IN_MILADYS_NIGHTGOWN,
  /// An old man with a cane.
  /// ★ Named from the game: "You are a fine warrior." (MsgBilibinFineWarrior)
  ACTOR_OLD_MAN_PRAISING_THE_WARRIORS,
  /// A soldier in a spiked helmet.
  /// ★ Named from the game: "I was watching from here. After all that fuss,
  /// you still went to Kolima?" (MsgBilibinWasWatchingFromHereAfter)
  ACTOR_SOLDIER_WATCHING_KOLIMA,
  /// A soldier in a spiked helmet; shows the treasure room's chests, as the
  /// captain asks.
  /// ★ Named from the game: "Please show them their reward!"
  /// (MsgBilibinLordMccoyOrdered)
  ACTOR_SOLDIER_SHOWING_THE_REWARD,
  /// McCoy, the chief of Bilibin, a stout man with red hair in a pink tunic.
  /// ★ Named from the game: "Lord McCoy" (MsgDebugGlyphName)
  ACTOR_LORD_MCCOY,
  /// The captain of McCoy's soldiers, drawn as they are, in a spiked helmet.
  /// ★ Named from the game: "I dinnae want tae talk about it anymore.
  /// Captain?" (MsgBilibinWellSGoing)
  ACTOR_CAPTAIN,
  /// The key McCoy tosses to the party.
  /// ★ Named from the game: "This key'll get yeh past the barricades tae the
  /// east..." (MsgBilibinHmmmWellGrant)
  ACTOR_BARRICADE_KEY,
  /// ★ Named from the game: "You may select a single item from among these
  /// four chests." (MsgBilibinYouMaySelectASingleItem)
  ACTOR_VIAL_CHEST,
  /// ★ Named from the game: "You may select a single item from among these
  /// four chests." (MsgBilibinYouMaySelectASingleItem)
  ACTOR_POTION_CHEST,
  /// ★ Named from the game: "You may select a single item from among these
  /// four chests." (MsgBilibinYouMaySelectASingleItem)
  ACTOR_PSY_CRYSTAL_CHEST,
  /// ★ Named from the game: "You may select a single item from among these
  /// four chests." (MsgBilibinYouMaySelectASingleItem)
  ACTOR_WATER_OF_LIFE_CHEST
};

static void McCoyPalace_ClaimReward(s32 chestActorId, s32 itemId,
                                    s32 chestFlag);
static void Event_AudienceWithMcCoy(void);
static void Event_CaptainHintsAtBarricade(void);
static void Event_McCoyRewardsParty(void);
static void Event_PresentRewardChests(void);

// The shared facing callback precedes the scene's entry points.
#include "common/object_facing.h"

const struct SceneEntrance *Scene_GetEntrances(void) {
  return gMcCoyPalaceEntrances;
}

const struct SceneRegion *Scene_GetRegions(void) { return NULL; }

const u32 *Scene_GetExits(void) { return gMcCoyPalaceExits; }

/// The people of McCoy's Palace. Once the reward is taken, the woman in
/// Milady's nightgown, the old man who praises the warriors, the soldier who
/// watched the party go to Kolima and the soldier who keeps the treasure turn
/// differently when talked to.
const struct ScenePlacement *Scene_GetPlacements(void) {
  struct ScenePlacement *placements;

  if (gGameState.scene == (s32)&SceneId_McCoyPalace) {
    placements = gMcCoyPalacePlacements;
    FieldScene_PrepareActors(placements);

    if (Engine_GameFlagIsSet(FLAG_MCCOY_PALACE_REWARD_TAKEN) != 0) {
      gMcCoyPalacePlacements[ACTOR_VILLAGER_IN_MILADYS_NIGHTGOWN -
                             ACTOR_FIRST_PLACED]
          .talkFacing = TALK_KEEP_FACING;
      gMcCoyPalacePlacements[ACTOR_OLD_MAN_PRAISING_THE_WARRIORS -
                             ACTOR_FIRST_PLACED]
          .talkFacing = TALK_FACE_PARTY;
      gMcCoyPalacePlacements[ACTOR_SOLDIER_WATCHING_KOLIMA - ACTOR_FIRST_PLACED]
          .talkFacing = 3;
      gMcCoyPalacePlacements[ACTOR_SOLDIER_SHOWING_THE_REWARD -
                             ACTOR_FIRST_PLACED]
          .talkFacing = TALK_FACE_PARTY_AND_BACK;
    }

    return placements;
  }
  return gMcCoyPalacePlacementsOther;
}

/// What the party can talk to and search in the palace.
const struct SceneEvent *Scene_GetEvents(void) {
  if (gGameState.scene == (s32)&SceneId_McCoyPalace) {
    return gMcCoyPalaceEvents;
  }
  return gMcCoyPalaceEventsOther;
}

/// McCoy's Palace, before the reward: a woman with fair hair asks about going
/// to Kolima. "Oh, are you willing to go to Kolima Forest?"
static void Event_AskAboutGoingToKolima(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgBilibinYouWillingGoKolimaForest);
  Engine_EventAskYesNo(ACTOR_VILLAGER_ASKING_ABOUT_KOLIMA_FOREST, 0);
  Engine_EventEnd();
}

/// McCoy's Palace, after the reward: the woman with Lady McCoy's silk
/// nightgown screams, then turns to the party. "AAAAH!!!"
static void Event_TalkToWomanInMiladysNightgown(void) {
  Engine_EventBegin();
  Actor_ShowEmote(ACTOR_VILLAGER_IN_MILADYS_NIGHTGOWN, EMOTE_IN_FRONT | 2, 0);
  Engine_ActorRunRepeatedMotion(ACTOR_VILLAGER_IN_MILADYS_NIGHTGOWN, 2);
  Engine_EventWait(40);
  Engine_EventSetMessage(MsgBilibinAaaah);
  Event_ShowMessageAndWait(ACTOR_VILLAGER_IN_MILADYS_NIGHTGOWN, 0, 20);
  Actor_FaceActor(ACTOR_VILLAGER_IN_MILADYS_NIGHTGOWN, ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(ACTOR_VILLAGER_IN_MILADYS_NIGHTGOWN, 0, 10);
  Actor_FaceDirection(ACTOR_VILLAGER_IN_MILADYS_NIGHTGOWN,
                      FACING_NORTHWEST + FACING_STEP, 10);
  Engine_EventEnd();
}

/// McCoy's Palace, after the reward: the old man praises the party the first
/// time, then warns of the holy tree.
/// "You are a fine warrior." After, "It's very reckless for such young
/// warriors to face the holy tree..."
static void Event_TalkToOldManPraisingTheWarriors(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgBilibinFineWarrior);
  if (GameFlag_IsSet(FLAG_PLACE_TEMP_02) != 0) {
    Engine_EventSetMessage(MsgBilibinItsVeryRecklessForSuch);
  }
  Event_ShowMessage(ACTOR_OLD_MAN_PRAISING_THE_WARRIORS, 0);
  GameFlag_Set(FLAG_PLACE_TEMP_02);
  Engine_EventEnd();
}

/// McCoy's Palace, after the reward: a soldier saw the party go to Kolima.
/// "I was watching from here. After all that fuss, you still went to Kolima?"
static void Event_TalkToSoldierWatchingKolima(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgBilibinWasWatchingFromHereAfter);
  Engine_EventAskYesNo(ACTOR_SOLDIER_WATCHING_KOLIMA, 0);
  Engine_EventEnd();
}

/// McCoy's Palace, the treasure room: the soldier who keeps the treasure. While
/// the party chooses, only one item; after, the reward was the greatest;
/// before, Bilibin's great treasures go to whoever ends the curse.
static void Event_TalkToSoldierShowingTheReward(void) {
  Engine_EventBegin();

  if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_02) != 0) {
    Engine_EventSetMessage(MsgBilibinMayChooseOnlyOneItem);
  } else if (Engine_GameFlagIsSet(FLAG_MCCOY_PALACE_REWARD_TAKEN) != 0) {
    Engine_EventSetMessage(MsgBilibinRewardReceivedWasIndeedGreatest);
  } else {
    Engine_EventSetMessage(MsgBilibinDoThinkTheseBilibinsGreat);
    if (Engine_GameFlagIsSet(0x84d) != 0) {
      gEventWork->message++;
    }
  }

  Engine_EventShowMessage(ACTOR_SOLDIER_SHOWING_THE_REWARD, 0);
  Engine_EventEnd();
}

/// McCoy's Palace, after the reward: Mind Read on the old man shows two
/// thoughts. "Sometimes, we need children to remind us of our own
/// responsibilities." Then "You looked very courageous walking toward
/// Kolima."
static void Event_ReadOldMansThoughtsOnTheWarriors(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgBilibinSometimesWeNeedChildrenRemind);
  if (GameFlag_IsSet(FLAG_PLACE_TEMP_03) != 0) {
    Engine_EventSetMessage(MsgBilibinLookedVeryCourageousWalkingToward);
  }
  Event_ShowMessage(ACTOR_OLD_MAN_PRAISING_THE_WARRIORS, 0);
  GameFlag_Set(FLAG_PLACE_TEMP_03);
  Engine_EventEnd();
}

/// McCoy's Palace, the treasure room: Mind Read on the soldier who keeps the
/// treasure, whose thoughts follow the curse and the reward.
static void Event_ReadRewardSoldiersThoughts(void) {
  Engine_EventBegin();

  if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_02) != 0) {
    Engine_EventSetMessage(MsgBilibinNonethelessIfYourLuckSour);
  } else if (Engine_GameFlagIsSet(FLAG_KOLIMA_CURSE_LIFTED) == 0) {
    Engine_EventSetMessage(MsgBilibinGoodTreasureIfGetTurned);
  } else {
    Engine_EventSetMessage(MsgBilibinWeReallyGivingOurTreasure);
    if (Engine_GameFlagIsSet(FLAG_MCCOY_PALACE_REWARD_TAKEN) != 0) {
      Engine_EventSetMessage(MsgBilibinGotPrettyNiceRewardBut);
    }
  }

  Engine_EventShowMessage(ACTOR_SOLDIER_SHOWING_THE_REWARD, 0);
  Engine_EventEnd();
}

/// McCoy's Palace, triggers 9, 12 and 13: walking north through a door opens
/// it with a sound, the leader walks through and the next room loads.
/// McCoy's door (trigger 9) makes no sound once the captain has opened it.
static void Event_WalkThroughDoor(void) {
  struct EventWork *work;
  s32 hideDoorLayers;

  work = gEventWork;
  hideDoorLayers = 0;
  if (work->touchedTrigger == 9) {
    if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_00) == 0) {
      Engine_AudioPlayCue(SE_188);
      hideDoorLayers = 1;
    }
  } else {
    Engine_AudioPlayCue(SE_DOOR_OPEN);
    hideDoorLayers = 1;
  }
  if (hideDoorLayers != 0) {
    Map_ClearLayerEntryFlag(1);
    Map_ClearLayerEntryFlag(2);
  }
  Engine_EventBegin();
  Engine_EventWait(10);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  if (work->touchedTrigger == 9)
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -16);
  else
    Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 3, -16);
  Engine_EventWait(16);
  Engine_EventRequestExit(work->touchedTrigger);
  Engine_EventEnd();
  Map_SetLayerEntryFlag(1);
  Map_SetLayerEntryFlag(2);
}

/// The chest is empty. "Isaac checked the chest... but it was empty."
static void McCoyPalace_ShowChestEmpty(void) {
  Engine_EventBegin();
  Engine_MessageShowCentered(MsgBilibinIsaacCheckedChestButWas, 1);
  Engine_EventEnd();
}

/// The chest is locked. "The treasure chest is locked."
static void McCoyPalace_ShowChestLocked(void) {
  Engine_EventBegin();
  Engine_MessageShowCentered(MsgBilibinTreasureChestLocked, 1);
  Engine_EventEnd();
}

/// Searching a reward chest after the reward: the chosen one is empty, the
/// others locked.
static void McCoyPalace_ShowUnavailableChest(s32 chestFlag) {
  if (Engine_GameFlagIsSet(chestFlag) != 0) {
    McCoyPalace_ShowChestEmpty();
  } else {
    McCoyPalace_ShowChestLocked();
  }
}

/// McCoy's Palace, the treasure room: searching the first reward chest gives
/// a Vial, while no reward is taken.
static void Event_SearchVialChest(void) {
  if (Engine_GameFlagIsSet(FLAG_MCCOY_PALACE_REWARD_TAKEN) != 0) {
    McCoyPalace_ShowUnavailableChest(FLAG_SCENE_TEMP_10);
  } else {
    McCoyPalace_ClaimReward(ACTOR_VIAL_CHEST, ITEM_VIAL, FLAG_SCENE_TEMP_10);
  }
}

/// McCoy's Palace, the treasure room: searching the second reward chest gives
/// a Potion, while no reward is taken.
static void Event_SearchPotionChest(void) {
  if (Engine_GameFlagIsSet(FLAG_MCCOY_PALACE_REWARD_TAKEN) != 0) {
    McCoyPalace_ShowUnavailableChest(FLAG_SCENE_TEMP_11);
  } else {
    McCoyPalace_ClaimReward(ACTOR_POTION_CHEST, ITEM_POTION,
                            FLAG_SCENE_TEMP_11);
  }
}

/// McCoy's Palace, the treasure room: searching the third reward chest gives
/// a Psy Crystal, while no reward is taken.
static void Event_SearchPsyCrystalChest(void) {
  if (Engine_GameFlagIsSet(FLAG_MCCOY_PALACE_REWARD_TAKEN) != 0) {
    McCoyPalace_ShowUnavailableChest(FLAG_SCENE_TEMP_12);
  } else {
    McCoyPalace_ClaimReward(ACTOR_PSY_CRYSTAL_CHEST, ITEM_PSY_CRYSTAL,
                            FLAG_SCENE_TEMP_12);
  }
}

/// McCoy's Palace, the treasure room: searching the fourth reward chest gives
/// a Water of Life, while no reward is taken.
static void Event_SearchWaterOfLifeChest(void) {
  if (Engine_GameFlagIsSet(FLAG_MCCOY_PALACE_REWARD_TAKEN) != 0) {
    McCoyPalace_ShowUnavailableChest(FLAG_SCENE_TEMP_13);
  } else {
    McCoyPalace_ClaimReward(ACTOR_WATER_OF_LIFE_CHEST, ITEM_WATER_OF_LIFE,
                            FLAG_SCENE_TEMP_13);
  }
}

/// The chest opens and gives its item; with no room for it, the chest
/// closes again and the reward is still to choose.
static void McCoyPalace_ClaimReward(s32 chestActorId, s32 itemId,
                                    s32 chestFlag) {
  s32 emitterHandle;

  Engine_EventBegin();

  emitterHandle =
      BattleFx_PlayCueAndStartEmitterOnTarget(0, chestActorId, itemId);

  if (Engine_PartyGiveItem(itemId, 0) != -1) {
    Engine_ActorSetAnimation(chestActorId, 2);
    Engine_GameFlagSet(FLAG_MCCOY_PALACE_REWARD_TAKEN);
    Engine_GameFlagSet(chestFlag);
    Engine_GameFlagClear(FLAG_PLACE_TEMP_22);
    Engine_GameFlagClear(FLAG_SCENE_TEMP_02);
  } else {
    Engine_AudioPlayCue(SE_125);
    Engine_ActorSetAnimation(chestActorId, 5);
  }

  Engine_ObjectDispatchRelease((struct FieldActor *)emitterHandle);
  Engine_EventEnd();
}

/// McCoy's Palace, trigger 51, leaving the treasure room without a reward: the
/// captain walks the leader back. "Please take your reward before your
/// depart!"
static void Event_CaptainAsksPartyToChoose(void) {
  if (GameFlag_IsSet(FLAG_MCCOY_PALACE_REWARD_TAKEN) == 0) {
    if (Engine_GameFlagIsSet(FLAG_PLACE_TEMP_22) != 0) {
      Engine_EventBegin();
      Actor_ShowEmote(ACTOR_CAPTAIN, EMOTE_IN_FRONT, 0);
      Actor_FaceDirection(ACTOR_CAPTAIN, FACING_SOUTHWEST + FACING_STEP, 10);
      Engine_ActorRunRepeatedMotion(ACTOR_CAPTAIN, 2);
      Engine_EventWait(20);
      Engine_EventSetMessage(MsgBilibinPleaseTakeYourRewardBefore);
      Event_ShowMessage(ACTOR_CAPTAIN, 0);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 616, 762);
      Actor_FaceDirection(ACTOR_CAPTAIN, FACING_NORTH + FACING_STEP, 10);
      Engine_EventEnd();
    }
  }
}

/// McCoy's Palace, trigger 50, leaving the treasure room with a reward: the
/// captain sees the party off. "You are always welcome in the palace of Lord
/// McCoy."
static void Event_CaptainSeesPartyOff(void) {
  if (GameFlag_IsSet(FLAG_MCCOY_PALACE_REWARD_TAKEN) != 0) {
    Engine_EventBegin();
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_CAPTAIN, 0);
    Actor_SetSpeed(ACTOR_CAPTAIN, PIXELS(3) / 5, PIXELS(3) / 10);
    Actor_WalkToAndWait(ACTOR_CAPTAIN, 622, 764);
    Actor_FaceDirection(ACTOR_CAPTAIN, FACING_NORTHEAST + FACING_STEP, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_CAPTAIN, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_SOLDIER_SHOWING_THE_REWARD, 3);
    Engine_EventWait(20);
    Actor_FaceActor(ACTOR_CAPTAIN, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(ACTOR_CAPTAIN, 3);
    Engine_EventSetMessage(MsgBilibinAlwaysWelcomeInPalaceLord);
    Event_ShowMessageAndWait(ACTOR_CAPTAIN, 0, 10);
    Actor_SetSpeed(ACTOR_CAPTAIN, PIXELS(4) / 5, PIXELS(2) / 5);
    Actor_WalkToAndWait(ACTOR_CAPTAIN, 570, 758);
    Actor_SetPosition(ACTOR_CAPTAIN, 0, 0);
    gEventWork->startTransition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
    GameFlag_Set(FLAG_MCCOY_PALACE_PARTY_REWARDED);
    GameFlag_Set(FLAG_PLACE_TEMP_33);
    Engine_EventEnd();
  }
}

/// The four reward chests stand in a row.
static __inline__ void McCoyPalace_PositionRewardChests(void) {
  Actor_SetPosition(ACTOR_VIAL_CHEST, PIXELS(616), PIXELS(728));
  Actor_SetPosition(ACTOR_POTION_CHEST, PIXELS(632), PIXELS(728));
  Actor_SetPosition(ACTOR_PSY_CRYSTAL_CHEST, PIXELS(648), PIXELS(728));
  Engine_ActorSetPosition(ACTOR_WATER_OF_LIFE_CHEST, PIXELS(664), PIXELS(728));
}

/// The cells before the chest row can be walked on.
static __inline__ void McCoyPalace_OpenRewardChestRow(void) {
  Map_CopyCellAttributes(38, 55, 4, 1, 38, 45);
  Map_CopyCellAttributes(42, 55, 4, 1, 38, 46);
}

/// Sets McCoy's Palace up: McCoy's first audience, the captain outside McCoy's
/// door after it, the reward audience once Kolima's curse is lifted, or the
/// reward chests where the party left them.
s32 Scene_Initialize(void) {
  s32 rewardTaken;
  s32 curseLifted;
  struct FieldActor *chest;

  gEventWork->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  Map_SetLayerEntryFlag(1);
  Map_SetLayerEntryFlag(2);
  Engine_GameFlagSet(FLAG_MCCOY_PALACE_VISITED);
  if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) != 0) {
    Engine_GameFlagClear(FLAG_SCENE_TEMP_00);
  }
  // A first audience and a return to the entrance have different greetings.
  if (Engine_GameFlagIsSet(FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY) == 0 &&
      Engine_GameFlagIsSet(FLAG_KOLIMA_CURSE_LIFTED) == 0) {
    if (gGameState.entrance == MCCOY_PALACE_FROM_MCCOY_PALACE_GROUNDS_EAST) {
      Event_AudienceWithMcCoy();
    } else if (gGameState.entrance == MCCOY_PALACE_HALL_FROM_MCCOYS_ROOM &&
               Engine_GameFlagIsSet(FLAG_PLACE_TEMP_21) != 0) {
      Event_CaptainHintsAtBarricade();
    }
  } else {
    rewardTaken = Engine_GameFlagIsSet(FLAG_MCCOY_PALACE_REWARD_TAKEN);
    if (rewardTaken != 0) {
    } else {
      if (gGameState.entrance == MCCOY_PALACE_FROM_MCCOY_PALACE_GROUNDS_EAST) {
        if (Engine_GameFlagIsSet(FLAG_MCCOY_PALACE_PARTY_REWARDED) == 0) {
          curseLifted = Engine_GameFlagIsSet(FLAG_KOLIMA_CURSE_LIFTED);
          if (curseLifted != 0) {
            Event_McCoyRewardsParty();
          }
        }
      } else {
        // Restore the displayed chest row after reloading the treasure room.
        if (gGameState.entrance == MCCOY_PALACE_TREASURE_ROOM_FROM_HALL) {
          if (Engine_GameFlagIsSet(FLAG_PLACE_TEMP_22) != 0) {
            if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) != 0) {
              McCoyPalace_OpenRewardChestRow();
              McCoyPalace_PositionRewardChests();
              chest = Object_GetById(ACTOR_VIAL_CHEST);
              Engine_ActorSetSpriteFlags(chest, 0);
              chest = Object_GetById(ACTOR_POTION_CHEST);
              Engine_ActorSetSpriteFlags(chest, 0);
              chest = Object_GetById(ACTOR_PSY_CRYSTAL_CHEST);
              Engine_ActorSetSpriteFlags(chest, 0);
              chest = Object_GetById(ACTOR_WATER_OF_LIFE_CHEST);
              Engine_ActorSetSpriteFlags(chest, 0);
              Object_GetById(ACTOR_VIAL_CHEST)->motionFlags = rewardTaken;
              Object_GetById(ACTOR_POTION_CHEST)->motionFlags = rewardTaken;
              Object_GetById(ACTOR_PSY_CRYSTAL_CHEST)->motionFlags =
                  rewardTaken;
              Object_GetById(ACTOR_WATER_OF_LIFE_CHEST)->motionFlags =
                  rewardTaken;
              chest = Object_GetById(ACTOR_VIAL_CHEST);
              chest->y.fixed = -PIXELS(4);
              chest = Object_GetById(ACTOR_POTION_CHEST);
              chest->y.fixed = -PIXELS(4);
              chest = Object_GetById(ACTOR_PSY_CRYSTAL_CHEST);
              chest->y.fixed = -PIXELS(4);
              chest = Object_GetById(ACTOR_WATER_OF_LIFE_CHEST);
              chest->y.fixed = -PIXELS(4);
            } else {
              Event_PresentRewardChests();
            }
          }
        }
      }
    }
  }
  return 0;
}

/// Garet, Ivan and Mia get ready to walk up to McCoy.
static __inline__ void McCoyPalace_PrepareCompanionApproach(void) {
  Actor_SetSpeed(GARET, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_SetSpeed(IVAN, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_SetSpeed(MIA, PIXELS(1), PIXELS(1) / 2);
  Engine_ActorSetAnimation(GARET, ANIM_WALK);
  Engine_ActorSetAnimation(IVAN, ANIM_WALK);
  Engine_ActorSetAnimation(MIA, ANIM_WALK);
}

/// McCoy's Palace, through McCoy's door before the curse is lifted, the first
/// audience: the captain announces the party and McCoy asks whether they can
/// save Kolima. Refusing, he calls them too young and they leave; coming back,
/// they may change their minds. Accepting, McCoy gives the key to the
/// barricades, Garet bristles at being called children, and McCoy takes the
/// key back and has the captain see them out. The party arrives at
/// MCCOY_PALACE_FROM_MCCOY_PALACE_GROUNDS_EAST, beyond the door.
static void Event_AudienceWithMcCoy(void) {
  struct FieldActor *actor;

  // Stage the first audience, or place the leader for a return visit.
  Engine_EventBegin();
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_TaskWait(1);
  Engine_EventGetViewCenter()->motionFlags = 0;
  Camera_MoveTo(PIXELS(894), -1, PIXELS(664), 0);
  Engine_TaskWait(1);
  Engine_ActorSetPosition(ACTOR_PARTY_LEADER, 0, 0);
  if (GameFlag_IsSet(FLAG_MCCOY_PALACE_JOB_REFUSED) != 0) {
    Camera_MoveTo(PIXELS(894), -1, PIXELS(698), 0);
    Actor_SetPosition(ACTOR_CAPTAIN, PIXELS(876), PIXELS(634));
    Actor_FaceDirection(ACTOR_CAPTAIN, FACING_SOUTHEAST + FACING_STEP, 0);
    Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(894), PIXELS(798));
  }
  Engine_MapRedraw();
  Engine_TaskWait(1);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  gEventWork->transitionFrames = 40;
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  if (Engine_GameFlagIsSet(FLAG_MCCOY_PALACE_JOB_REFUSED) == 0) {
    Engine_EventWait(80);
    Actor_SetPosition(ACTOR_CAPTAIN, PIXELS(894), PIXELS(798));
    Camera_SetSpeed(PIXELS(3) / 5, PIXELS(3) / 40);
    Camera_MoveTo(PIXELS(894), -1, PIXELS(698), 1);
    Actor_SetSpeed(ACTOR_CAPTAIN, PIXELS(4) / 5, PIXELS(2) / 5);
    Actor_WalkTo(ACTOR_CAPTAIN, 894, 696);
    Engine_EventWait(80);
    Camera_MoveTo(PIXELS(894), -1, PIXELS(664), 1);
    Engine_ActorWaitForMove(ACTOR_CAPTAIN);
    Actor_WalkToAndWait(ACTOR_CAPTAIN, 842, 696);
    Actor_WalkToAndWait(ACTOR_CAPTAIN, 842, 636);
    Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTHWEST + FACING_STEP, 20);
    Actor_WalkToAndWait(ACTOR_CAPTAIN, 876, 634);
    Engine_ActorSetAnimationAndWait(ACTOR_CAPTAIN, 3);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(ACTOR_LORD_MCCOY, 3);
    Engine_EventWait(10);
    // The captain announces the warriors before they enter the audience.
    Engine_EventSetMessage(MsgBilibinMatter);
    Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
    Engine_ActorRunRepeatedMotion(ACTOR_CAPTAIN, 2);
    Engine_EventShowMessageAndWait(ACTOR_CAPTAIN, 0, 20);
    Engine_ActorRunRepeatedMotion(ACTOR_LORD_MCCOY, 1);
    Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
    Engine_ActorSetAnimationAndWait(ACTOR_CAPTAIN, 3);
    Engine_EventWait(40);
    Actor_ShowEmote(ACTOR_LORD_MCCOY, EMOTE_IN_FRONT | 5, 60);
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0);
    Engine_ActorRunRepeatedMotion(ACTOR_LORD_MCCOY, 1);
    Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
    Engine_ActorSetAttachedEffect(ACTOR_CAPTAIN, ATTACHED_EFFECT_IN_FRONT |
                                                     ATTACHED_EFFECT_ONCE);
    Engine_EventWait(60);
    Actor_FaceDirection(ACTOR_CAPTAIN, FACING_SOUTHEAST + FACING_STEP, 10);
    Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTH + FACING_STEP, 10);
    Camera_MoveTo(PIXELS(894), -1, PIXELS(698), 1);
    Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(894), PIXELS(798));
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 5, PIXELS(3) / 10);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 894, 726);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(ACTOR_LORD_MCCOY, 1);
    Engine_EventShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0);
    Camera_MoveTo(PIXELS(894), -1, PIXELS(664), 1);
    Engine_ActorWalkToAndWait(ACTOR_PARTY_LEADER, 894, 684);
    actor = Object_GetById(ACTOR_PARTY_LEADER);
    if (actor != 0) {
      Engine_ActorSetPosition(GARET, actor->x.fixed, actor->z.fixed);
    }
    actor = Object_GetById(ACTOR_PARTY_LEADER);
    if (actor != 0) {
      Engine_ActorSetPosition(IVAN, actor->x.fixed, actor->z.fixed);
    }
    if (Engine_GameFlagIsSet(FLAG_IN_PARTY(MIA)) != 0) {
      actor = Object_GetById(ACTOR_PARTY_LEADER);
      if (actor != 0) {
        Engine_ActorSetPosition(MIA, actor->x.fixed, actor->z.fixed);
      }
    }
    McCoyPalace_PrepareCompanionApproach();
    Actor_SetDestinationOffset(GARET, -16, 16);
    Engine_ActorSetDestinationOffset(IVAN, 16, 16);
    if (Engine_GameFlagIsSet(FLAG_IN_PARTY(MIA)) != 0) {
      Engine_ActorSetDestinationOffset(MIA, 32, 16);
    }
    Engine_ActorWaitForMove(IVAN);
    Engine_ActorSetAnimation(GARET, ANIM_STAND);
    Engine_ActorSetAnimation(IVAN, ANIM_STAND);
    Engine_ActorSetAnimation(MIA, ANIM_STAND);
    Engine_EventWait(10);
    Actor_FaceDirection(MIA, FACING_NORTH, 0);
    Actor_FaceDirection(GARET, FACING_NORTH, 0);
    Actor_FaceDirection(IVAN, FACING_NORTH, 20);
    Engine_ActorJump(ACTOR_LORD_MCCOY, 2, 20);
    Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTHWEST + FACING_STEP, 10);
    Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
    Actor_FaceDirection(ACTOR_CAPTAIN, FACING_EAST + FACING_STEP, 10);
    Engine_ActorSetAnimationAndWait(ACTOR_CAPTAIN, 3);
    Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTH + FACING_STEP, 40);
    Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTHWEST + FACING_STEP, 10);
    Engine_ActorSetAnimationAndWait(ACTOR_LORD_MCCOY, 4);
    Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
    Engine_ActorSetAttachedEffect(ACTOR_CAPTAIN, ATTACHED_EFFECT_IN_FRONT |
                                                     ATTACHED_EFFECT_ONCE);
    Engine_EventWait(40);
    Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTH + FACING_STEP, 20);
    Actor_ShowEmote(ACTOR_LORD_MCCOY, EMOTE_IN_FRONT | 5, 40);
    // Let the party accept the assignment or leave after McCoy refuses them.
    Engine_EventOpenMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0);
    Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
    Actor_FaceDirection(IVAN, FACING_NORTHWEST, 0);
    if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
      // The initial offer and reconsideration share their accept/refuse paths.
      goto accepted;
    }
  declined:
    Engine_EventSetMessage(MsgBilibinTooYoungForTheJob);
    Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
    Actor_FaceDirection(ACTOR_CAPTAIN, FACING_SOUTHEAST + FACING_STEP, 0);
    Engine_ActorSetAnimationAndWait(ACTOR_LORD_MCCOY, 4);
    Engine_EventShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0,
                                   10);
    Actor_FaceDirection(GARET, FACING_NORTH, 0);
    Actor_FaceDirection(IVAN, FACING_NORTH, 0);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_ActorSetAnimation(GARET, ANIM_WALK);
    actor = Object_GetById(ACTOR_PARTY_LEADER);
    if (actor != 0) {
      Engine_ActorSetDestination(GARET, actor->x.part.pixel,
                                 actor->z.part.pixel);
    }
    Engine_ActorSetAnimation(IVAN, ANIM_WALK);
    actor = Object_GetById(ACTOR_PARTY_LEADER);
    if (actor != 0) {
      Engine_ActorSetDestination(IVAN, actor->x.part.pixel,
                                 actor->z.part.pixel);
    }
    if (Engine_GameFlagIsSet(FLAG_IN_PARTY(MIA)) != 0) {
      Engine_ActorSetAnimation(MIA, ANIM_WALK);
      actor = Object_GetById(ACTOR_PARTY_LEADER);
      if (actor != 0) {
        Engine_ActorSetDestination(MIA, actor->x.part.pixel,
                                   actor->z.part.pixel);
      }
    }
    Engine_ActorWaitForMove(IVAN);
    Engine_ActorSetPosition(GARET, 0, 0);
    Engine_ActorSetPosition(IVAN, 0, 0);
    Actor_SetPosition(MIA, 0, 0);
    Engine_GameFlagSet(FLAG_MCCOY_PALACE_JOB_REFUSED);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 894, 752);
    gEventWork->transitionFrames = 16;
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    goto leave;
  }
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 5, PIXELS(3) / 10);
  Engine_ActorWalkTo(ACTOR_PARTY_LEADER, 894, 684);
  Engine_EventWait(80);
  Engine_CameraSetSpeed(PIXELS(3) / 5, PIXELS(3) / 40);
  Engine_CameraMoveTo(PIXELS(894), -1, PIXELS(664), 1);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Engine_ActorSetPosition(GARET, actor->x.fixed, actor->z.fixed);
  }
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Engine_ActorSetPosition(IVAN, actor->x.fixed, actor->z.fixed);
  }
  if (Engine_GameFlagIsSet(FLAG_IN_PARTY(MIA)) != 0) {
    actor = Object_GetById(ACTOR_PARTY_LEADER);
    if (actor != 0) {
      Engine_ActorSetPosition(MIA, actor->x.fixed, actor->z.fixed);
    }
  }
  McCoyPalace_PrepareCompanionApproach();
  Actor_SetDestinationOffset(GARET, -16, 16);
  Engine_ActorSetDestinationOffset(IVAN, 16, 16);
  if (Engine_GameFlagIsSet(FLAG_IN_PARTY(MIA)) != 0) {
    Engine_ActorSetDestinationOffset(MIA, 32, 16);
  }
  Engine_ActorWaitForMove(IVAN);
  Engine_ActorSetAnimation(GARET, ANIM_STAND);
  Engine_ActorSetAnimation(IVAN, ANIM_STAND);
  Engine_ActorSetAnimation(MIA, ANIM_STAND);
  Engine_EventWait(10);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 20);
  Engine_ActorShowEmote(ACTOR_LORD_MCCOY, EMOTE_IN_FRONT | 1, 60);
  // A returning party can reconsider the assignment without replaying the
  // captain's announcement.
  Engine_EventSetMessage(MsgBilibinYehveChangeHeart);
  Engine_EventOpenMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0);
  if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 1) {
    // Reuse the initial refusal's dialogue and departure.
    goto declined;
  }
accepted:
  // Both ways of accepting join the same explanation and departure.
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 20);
  Engine_ActorSetAnimation(MIA, ANIM_NOD);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  Engine_ActorShowEmote(ACTOR_LORD_MCCOY, EMOTE_IN_FRONT | 5, 60);
  Engine_EventSetMessage(MsgBilibinHmmmWellGrant);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0);
  // Animate McCoy tossing the barricade key to the party.
  actor = Object_GetById(ACTOR_BARRICADE_KEY);
  Engine_ActorSetSpriteFlags(actor, 0);
  actor = Object_GetById(ACTOR_BARRICADE_KEY);
  actor->scaleX = Q16_ONE / 2;
  actor->scaleY = Q16_ONE / 2;
  actor = Object_GetById(ACTOR_LORD_MCCOY);
  if (actor != 0) {
    Engine_ActorSetPosition(ACTOR_BARRICADE_KEY, actor->x.fixed,
                            actor->z.fixed);
  }
  Engine_TaskWait(1);
  Engine_ActorJump(ACTOR_BARRICADE_KEY, 6, 0);
  Actor_SetSpeed(ACTOR_BARRICADE_KEY, PIXELS(2), PIXELS(1));
  Actor_MoveToAndWait(ACTOR_BARRICADE_KEY, 894, 668);
  Engine_EventWait(40);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 1, 60);
  Engine_ActorSetAnimationAndWait(ACTOR_LORD_MCCOY, 4);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 3, 60);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 10);
  Engine_EventOpenMessage(SPEAKER_WINDOW_ABOVE | GARET, 0);
  Actor_FaceDirection(MIA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(IVAN, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 1) {
    do {
      Engine_ActorStartRepeatedMotion(GARET, 2);
      Engine_ActorRunRepeatedMotion(IVAN, 2);
      Engine_EventSetMessage(MsgBilibinSeriousDoesntBother);
      Engine_EventOpenMessage(SPEAKER_WINDOW_ABOVE | GARET, 0);
    } while (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) != 1);
  }
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  // Resolve Garet\'s objections before McCoy dismisses the party.
  Engine_EventSetMessage(MsgBilibinWellDontLet);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | GARET, 0, 10);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 10);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(20);
  Actor_ShowEmote(ACTOR_LORD_MCCOY, EMOTE_IN_FRONT | 5, 60);
  Engine_EventShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0,
                                 20);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 2, 60);
  Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTHEAST + FACING_STEP, 10);
  Actor_ShowEmote(ACTOR_LORD_MCCOY, EMOTE_IN_FRONT | 1, 60);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 1, 40);
  Engine_ActorFaceDirection(GARET, FACING_EAST, 20);
  Engine_ActorRunRepeatedMotion(GARET, 1);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | GARET, 0, 10);
  Actor_FaceDirection(MIA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 20);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 2, 60);
  Actor_FaceDirection(IVAN, FACING_WEST, 10);
  Engine_EventShowMessageAndWait(SPEAKER_WINDOW_ABOVE | IVAN, 0, 10);
  Engine_ActorRunRepeatedMotion(GARET, 1);
  Engine_EventWait(40);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | GARET, 0, 10);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 10);
  Engine_ActorStartRepeatedMotion(GARET, 1);
  Engine_EventOpenMessage(SPEAKER_WINDOW_ABOVE | GARET, 0);
  Actor_FaceDirection(MIA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(IVAN, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  while (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) != 0) {
    Engine_EventSetMessage(MsgBilibinComeSaidYoud);
    Engine_EventOpenMessage(SPEAKER_WINDOW_ABOVE | GARET, 0);
  }
  Engine_EventWait(10);
  Actor_FaceDirection(MIA, FACING_WEST, 0);
  Engine_ActorFaceDirection(GARET, FACING_EAST, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 10);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 5, 60);
  Actor_FaceDirection(IVAN, FACING_WEST, 10);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_SHAKE_HEAD);
  Engine_EventSetMessage(MsgBilibinWellSGoing);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | IVAN, 0, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_LORD_MCCOY, 1);
  Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTH + FACING_STEP, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_LORD_MCCOY, 4);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 20);
  Engine_ActorSetAnimation(MIA, ANIM_NOD);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  Engine_EventWait(40);
  Actor_ShowEmote(ACTOR_LORD_MCCOY, EMOTE_IN_FRONT | 5, 80);
  Actor_FaceDirection(ACTOR_CAPTAIN, FACING_EAST + FACING_STEP, 10);
  Actor_SetAttachedEffect(ACTOR_CAPTAIN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Engine_EventWait(40);
  Engine_EventShowMessageAndWait(ACTOR_CAPTAIN, 0, 10);
  Engine_ActorRunRepeatedMotion(ACTOR_LORD_MCCOY, 1);
  Engine_EventWait(40);
  Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTHWEST + FACING_STEP, 20);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_CAPTAIN, 3);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_LORD_MCCOY, 4);
  Engine_ActorSetAnimation(ACTOR_LORD_MCCOY, 4);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
  Engine_ActorJump(ACTOR_BARRICADE_KEY, 6, 0);
  actor = Object_GetById(ACTOR_LORD_MCCOY);
  if (actor != 0) {
    Engine_ActorSetDestination(ACTOR_BARRICADE_KEY, actor->x.part.pixel,
                               actor->z.part.pixel);
  }
  Engine_ActorWaitForMove(ACTOR_BARRICADE_KEY);
  Engine_ActorSetPosition(ACTOR_BARRICADE_KEY, 0, 0);
  Engine_EventWait(20);
  Actor_SetAttachedEffect(MIA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Engine_ActorSetAttachedEffect(IVAN, ATTACHED_EFFECT_IN_FRONT |
                                          ATTACHED_EFFECT_ONCE);
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(ACTOR_CAPTAIN, 2);
  Engine_EventShowMessageAndWait(ACTOR_CAPTAIN, 0, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_LORD_MCCOY, 3);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_CAPTAIN, 3);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | IVAN, 0, 10);
  Actor_FaceDirection(ACTOR_CAPTAIN, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTHEAST + FACING_STEP, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_LORD_MCCOY, 4);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 3, 60);
  Engine_EventShowMessageAndWait(SPEAKER_WINDOW_ABOVE | GARET, 0, 10);
  Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTH + FACING_STEP, 10);
  Engine_ActorSetAnimation(ACTOR_LORD_MCCOY, 4);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
  Engine_ActorSetAnimation(IVAN, ANIM_SHAKE_HEAD);
  Engine_EventShowMessageAndWait(SPEAKER_WINDOW_ABOVE | IVAN, 0, 10);
  Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTHEAST + FACING_STEP, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_LORD_MCCOY, 3);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 7, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 7, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 7, 0);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 7, 60);
  Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTHWEST + FACING_STEP, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_LORD_MCCOY, 3);
  Engine_EventWait(10);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
  Engine_ActorRunRepeatedMotion(ACTOR_CAPTAIN, 2);
  Actor_FaceDirection(ACTOR_CAPTAIN, FACING_EAST + FACING_STEP, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_CAPTAIN, 3);
  Engine_EventWait(20);
  FieldObject_SetTargetAndCallback(ACTOR_PARTY_LEADER,
                                   ACTOR_LINK_KEEP_MOTION | ACTOR_CAPTAIN,
                                   gPalace_FacingActions);
  // Keep the party looking at the captain while he leaves the audience.
  FieldObject_SetTargetAndCallback(
      GARET, ACTOR_LINK_KEEP_MOTION | ACTOR_CAPTAIN, gPalace_FacingActions);
  FieldObject_SetTargetAndCallback(IVAN, ACTOR_LINK_KEEP_MOTION | ACTOR_CAPTAIN,
                                   gPalace_FacingActions);
  FieldObject_SetTargetAndCallback(MIA, ACTOR_LINK_KEEP_MOTION | ACTOR_CAPTAIN,
                                   gPalace_FacingActions);
  Actor_SetSpeed(ACTOR_CAPTAIN, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_WalkToAndWait(ACTOR_CAPTAIN, 852, 646);
  Actor_WalkToAndWait(ACTOR_CAPTAIN, 852, 666);
  Actor_WalkToAndWait(ACTOR_CAPTAIN, 864, 672);
  Actor_FaceDirection(ACTOR_CAPTAIN, FACING_EAST + FACING_STEP, 10);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_CAPTAIN, 0, 20);
  Engine_ActorStop(ACTOR_PARTY_LEADER);
  Engine_ActorStop(GARET);
  Engine_ActorStop(IVAN);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 5, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 5, 0);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 5, 60);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  FieldObject_SetTargetAndCallback(ACTOR_CAPTAIN,
                                   ACTOR_LINK_KEEP_MOTION | ACTOR_PARTY_LEADER,
                                   gPalace_FacingActions);
  Engine_ActorSetAnimation(GARET, ANIM_WALK);
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Engine_ActorSetDestination(GARET, actor->x.part.pixel, actor->z.part.pixel);
  }
  Engine_ActorSetAnimation(IVAN, ANIM_WALK);
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Engine_ActorSetDestination(IVAN, actor->x.part.pixel, actor->z.part.pixel);
  }
  if (Engine_GameFlagIsSet(FLAG_IN_PARTY(MIA)) != 0) {
    Engine_ActorSetAnimation(MIA, ANIM_WALK);
    actor = Object_GetById(ACTOR_PARTY_LEADER);
    if (actor != 0) {
      Engine_ActorSetDestination(MIA, actor->x.part.pixel, actor->z.part.pixel);
    }
  }
  Engine_ActorWaitForMove(IVAN);
  Engine_ActorSetPosition(GARET, 0, 0);
  Engine_ActorSetPosition(IVAN, 0, 0);
  Engine_ActorSetPosition(MIA, 0, 0);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  Engine_ActorWalkToAndWait(ACTOR_PARTY_LEADER, 894, 752);
  gEventWork->transitionFrames = 16;
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  Engine_GameFlagSet(FLAG_PLACE_TEMP_21);
leave:
  Engine_EventRequestExit(MCCOY_PALACE_MCCOYS_ROOM_TO_HALL);
  Engine_EventEnd();
}

/// McCoy's Palace, at MCCOY_PALACE_HALL_FROM_MCCOYS_ROOM after the first
/// audience: outside McCoy's door,
/// his captain is sorry the party was rejected, asks them not to go to Kolima
/// and lets slip that the barricade may not need a key.
static void Event_CaptainHintsAtBarricade(void) {
  Engine_EventBegin();
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Actor_FaceDirection(ACTOR_CAPTAIN, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 256, 660);
  Engine_EventWait(20);
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_GameFlagSet(FLAG_SCENE_TEMP_00);
  Event_PlayCue(SE_188);
  Map_ClearLayerEntryFlag(1);
  Map_ClearLayerEntryFlag(2);
  Engine_ActorSetPosition(ACTOR_CAPTAIN, PIXELS(256), PIXELS(632));
  Engine_TaskWait(1);
  Actor_SetSpeed(ACTOR_CAPTAIN, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_WalkToAndWait(ACTOR_CAPTAIN, 256, 644);
  Map_SetLayerEntryFlag(1);
  Map_SetLayerEntryFlag(2);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(ACTOR_CAPTAIN, 2);
  Engine_EventSetMessage(MsgBilibinNameSorryRejected);
  Engine_EventShowMessageAndWait(ACTOR_CAPTAIN, 0, 10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT, 40);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 264, 660);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_WalkToAndWait(ACTOR_CAPTAIN, 248, 660);
  Actor_FaceDirection(ACTOR_CAPTAIN, FACING_EAST + FACING_STEP, 40);
  Engine_ActorSetAnimationAndWait(ACTOR_CAPTAIN, 4);
  Engine_EventShowMessage(ACTOR_CAPTAIN, 0);
  Engine_ActorSetAnimationAndWait(ACTOR_CAPTAIN, 3);
  Engine_EventAskYesNo(ACTOR_CAPTAIN, 0);
  Engine_ActorRunRepeatedMotion(ACTOR_CAPTAIN, 2);
  Engine_EventShowMessageAndWait(ACTOR_CAPTAIN, 0, 10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 1, 60);
  Actor_SetAttachedEffect(ACTOR_CAPTAIN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Engine_EventWait(60);
  Engine_ActorRunRepeatedMotion(ACTOR_CAPTAIN, 1);
  Engine_EventShowMessageAndWait(ACTOR_CAPTAIN, 0, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_CAPTAIN, 3);
  Event_ShowMessage(ACTOR_CAPTAIN, 0);
  Actor_SetSpeed(ACTOR_CAPTAIN, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_CAPTAIN, 248, 772);
  Engine_ActorSetPosition(ACTOR_CAPTAIN, 0, 0);
  Engine_GameFlagClear(FLAG_SHOW_LOCATION_NAME);
  Engine_GameFlagSet(FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY);
  Engine_EventEnd();
}

/// Garet, Ivan and Mia turn to talk among themselves.
static void McCoyPalace_FaceCompanionsForDiscussion(void) {
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(IVAN, FACING_NORTHWEST, 0);
  Actor_FaceDirection(MIA, FACING_WEST, 0);
}

/// Garet, Ivan and Mia face McCoy again.
static void McCoyPalace_FaceCompanionsForAudience(void) {
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTHWEST, 0);
}

/// McCoy's Palace, once Kolima's curse is lifted: brought in under arrest, the
/// party is thanked by McCoy and sent to choose a reward.
/// The gate guards bring the party in from the grounds
/// (MCCOY_PALACE_GROUNDS_TO_MCCOY_PALACE_EAST) to
/// MCCOY_PALACE_FROM_MCCOY_PALACE_GROUNDS_EAST, before McCoy.
static void Event_McCoyRewardsParty(void) {
  struct FieldActor *viewCenter;
  struct FieldActor *actor;
  s32 advanceAlternateReply;

  Engine_EventBegin();
  // Reveal the approach, then walk the leader and the captain into the
  // audience.
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_TaskWait(1);
  viewCenter = Engine_EventGetViewCenter();
  advanceAlternateReply = 0;
  viewCenter->motionFlags = advanceAlternateReply;
  Camera_MoveTo(PIXELS(894), -1, PIXELS(664), 0);
  Engine_TaskWait(1);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  Engine_MapRedraw();
  Engine_TaskWait(1);

  gEventWork->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  gEventWork->transitionFrames = 16;

  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(40);
  Actor_SetPosition(ACTOR_CAPTAIN, PIXELS(888), PIXELS(798));
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(904), PIXELS(798));
  Camera_SetSpeed(PIXELS(3) / 5, PIXELS(3) / 40);
  Camera_MoveTo(PIXELS(894), -1, PIXELS(698), 1);
  Actor_SetSpeed(ACTOR_CAPTAIN, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_WalkTo(ACTOR_CAPTAIN, 888, 720);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 904, 736);
  Engine_EventWait(60);
  Engine_ActorWaitForMove(ACTOR_CAPTAIN);
  Engine_ActorSetAnimation(ACTOR_CAPTAIN, 1);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(ACTOR_CAPTAIN, 2);
  // Present the returning warriors, using the reply for their earlier
  // reception.
  Engine_EventSetMessage(MsgBilibinWeveBroughtWarriorsMilord);
  // Adjacent variants depend on whether McCoy's earlier request was declined.

  advanceAlternateReply = 1;
  if (GameFlag_IsSet(FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY) == 0) {
    gEventWork->message++;
    advanceAlternateReply = 0;
  }
  Event_ShowMessage(ACTOR_CAPTAIN, 0);
  if (advanceAlternateReply != 0) {
    gEventWork->message++;
  }

  Camera_MoveTo(PIXELS(894), -1, PIXELS(664), 1);
  Actor_SetActionScript(ACTOR_CAPTAIN, gPalace_PacingActions);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 894, 684);

  actor = Object_GetById(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Actor_SetPosition(GARET, actor->x.fixed, actor->z.fixed);
  }
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Actor_SetPosition(IVAN, actor->x.fixed, actor->z.fixed);
  }
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Actor_SetPosition(MIA, actor->x.fixed, actor->z.fixed);
  }

  McCoyPalace_PrepareCompanionApproach();
  Actor_SetDestinationOffset(GARET, -16, 16);
  Actor_SetDestinationOffset(IVAN, 16, 16);
  Actor_SetDestinationOffset(MIA, 32, 16);
  Engine_ActorWaitForMove(IVAN);
  Engine_ActorSetAnimation(GARET, ANIM_STAND);
  Engine_ActorSetAnimation(IVAN, ANIM_STAND);
  Engine_ActorSetAnimation(MIA, ANIM_STAND);
  Engine_EventWait(10);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Engine_ActorWaitForMove(MIA);
  Actor_FaceDirection(MIA, FACING_NORTHWEST, 0);
  Object_RefreshSelectorById(ACTOR_CAPTAIN);
  Engine_EventWait(20);

  advanceAlternateReply = 1;
  if (GameFlag_IsSet(FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY) == 0) {
    gEventWork->message++;
    advanceAlternateReply = 0;
  }
  Engine_ActorRunRepeatedMotion(ACTOR_LORD_MCCOY, 3);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 20);
  if (advanceAlternateReply != 0) {
    gEventWork->message++;
  }

  advanceAlternateReply = 1;
  if (GameFlag_IsSet(FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY) == 0) {
    gEventWork->message++;
    advanceAlternateReply = 0;
  }
  Engine_ActorRunRepeatedMotion(ACTOR_LORD_MCCOY, 1);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
  if (advanceAlternateReply != 0) {
    gEventWork->message++;
  }

  // Let the companions answer McCoy\'s thanks in turn.
  McCoyPalace_FaceCompanionsForDiscussion();
  Engine_EventWait(20);
  if (GameFlag_IsSet(FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY) != 0) {
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 60);
    Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 5, 40);
  } else {
    Engine_EventWait(40);
  }

  Actor_FaceDirection(GARET, FACING_NORTH, 10);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | GARET, 0, 10);
  Actor_FaceDirection(IVAN, FACING_NORTH, 10);
  Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | IVAN, 0);
  Actor_FaceDirection(MIA, FACING_NORTHWEST, 10);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | MIA, 0, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_LORD_MCCOY, 3);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 20);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 3, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 60);

  if (GameFlag_IsSet(FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY) != 0) {
    Engine_ActorRunRepeatedMotion(ACTOR_LORD_MCCOY, 1);
    Engine_ActorSetAnimationAndWait(ACTOR_LORD_MCCOY, 4);
    Event_OpenMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0);
    McCoyPalace_FaceCompanionsForDiscussion();
    advanceAlternateReply = 1;
    if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) != 0) {
      gEventWork->message++;
      advanceAlternateReply = 0;
    }
    Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTH + FACING_STEP, 0);
    McCoyPalace_FaceCompanionsForAudience();
    Engine_EventWait(10);
    Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
    if (advanceAlternateReply != 0) {
      gEventWork->message++;
    }
    Actor_ShowEmote(ACTOR_LORD_MCCOY, EMOTE_IN_FRONT | 2, 60);
  } else {
    gEventWork->message += 4;
  }

  Event_OpenMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0);
  McCoyPalace_FaceCompanionsForDiscussion();
  if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
    Engine_EventSetMessage(MsgBilibinHumblyThank);
  } else {
    Engine_EventSetMessage(MsgBilibinNaeYehDinnaeNeedTae);
  }

  McCoyPalace_FaceCompanionsForAudience();
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_CAPTAIN, 1);
  // McCoy explains his caution and offers a choice from the treasury.
  Engine_EventSetMessage(MsgBilibinWasButWorriedYehMight);
  Event_ShowMessageAndWait(ACTOR_CAPTAIN, 0, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(IVAN, FACING_EAST, 0);
  Actor_FaceDirection(MIA, FACING_WEST, 40);
  Engine_ActorRunRepeatedMotion(ACTOR_LORD_MCCOY, 2);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  McCoyPalace_FaceCompanionsForAudience();
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_LORD_MCCOY, EMOTE_IN_FRONT | 5, 60);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimation(IVAN, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_LORD_MCCOY, 3);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
  Actor_ShowEmote(ACTOR_LORD_MCCOY, EMOTE_IN_FRONT | 8, 60);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
  Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTHEAST + FACING_STEP, 10);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_LORD_MCCOY, 3);
  Event_OpenMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0);

  McCoyPalace_FaceCompanionsForDiscussion();
  advanceAlternateReply = 1;
  if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 1) {
    gEventWork->message++;
    advanceAlternateReply = 0;
  }
  McCoyPalace_FaceCompanionsForAudience();
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
  if (advanceAlternateReply != 0) {
    gEventWork->message++;
  }

  Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTHWEST + FACING_STEP, 10);
  Engine_ActorRunRepeatedMotion(ACTOR_CAPTAIN, 1);
  Actor_FaceDirection(ACTOR_CAPTAIN, FACING_EAST + FACING_STEP, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_LORD_MCCOY, 3);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_CAPTAIN, 3);
  Event_ShowMessageAndWait(ACTOR_CAPTAIN, 0, 10);
  Actor_FaceDirection(ACTOR_CAPTAIN, FACING_SOUTHEAST + FACING_STEP, 10);
  Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTHEAST + FACING_STEP, 20);
  Engine_ActorRunRepeatedMotion(ACTOR_LORD_MCCOY, 1);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_LORD_MCCOY, 3);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_ActorSetAnimation(GARET, ANIM_NOD);
  Engine_ActorSetAnimation(IVAN, ANIM_NOD);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
  Engine_EventWait(20);

  Engine_ActorSetAnimation(GARET, ANIM_WALK);
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Actor_SetDestination(GARET, actor->x.part.pixel, actor->z.part.pixel);
  }
  Engine_ActorSetAnimation(IVAN, ANIM_WALK);
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Actor_SetDestination(IVAN, actor->x.part.pixel, actor->z.part.pixel);
  }
  Engine_ActorSetAnimation(MIA, ANIM_WALK);
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Actor_SetDestination(MIA, actor->x.part.pixel, actor->z.part.pixel);
  }

  Engine_ActorWaitForMove(GARET);
  Actor_SetPosition(GARET, 0, 0);
  Actor_SetPosition(IVAN, 0, 0);
  Engine_ActorWaitForMove(MIA);
  Actor_SetPosition(MIA, 0, 0);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTH + FACING_STEP, 0);
  FieldObject_SetTargetAndCallback(ACTOR_PARTY_LEADER,
                                   ACTOR_LINK_KEEP_MOTION | ACTOR_CAPTAIN,
                                   gPalace_FacingActions);
  Actor_WalkToAndWait(ACTOR_CAPTAIN, 852, 646);
  Actor_WalkToAndWait(ACTOR_CAPTAIN, 852, 666);
  Actor_WalkToAndWait(ACTOR_CAPTAIN, 864, 672);
  Actor_FaceDirection(ACTOR_CAPTAIN, FACING_EAST + FACING_STEP, 10);
  Engine_ActorRunRepeatedMotion(ACTOR_CAPTAIN, 1);
  Engine_EventWait(10);
  Event_ShowMessageAndWait(ACTOR_CAPTAIN, 0, 10);
  Actor_WalkToAndWait(ACTOR_CAPTAIN, 886, 708);
  Actor_WalkTo(ACTOR_CAPTAIN, 894, 764);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 894, 764);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  GameFlag_Set(FLAG_PLACE_TEMP_22);
  if (GameFlag_IsSet(FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY) == 0) {
    GameFlag_Set(FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY);
    GameFlag_Set(FLAG_MCCOY_PALACE_ADMITTED);
  }
  Engine_EventRequestExit(MCCOY_PALACE_HALL_TO_TREASURE_ROOM);
  Engine_EventEnd();
}

/// McCoy's Palace, at MCCOY_PALACE_TREASURE_ROOM_FROM_HALL, the treasure room:
/// the captain shows the four chests and the soldier who keeps the treasure
/// explains that only one may be chosen.
static void Event_PresentRewardChests(void) {
  struct FieldActor *actor;

  Engine_EventBegin();
  // Restore the chest-row collision patches before presenting the reward.
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_EventGetViewCenter()->motionFlags = 0;
  Engine_CameraMoveTo(0x2740000, -1, 0x2ec0000, 0);
  McCoyPalace_OpenRewardChestRow();
  Object_GetById(ACTOR_PARTY_LEADER)->facing = 0;
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(577), PIXELS(760));
  Object_GetById(ACTOR_CAPTAIN)->facing = 0;
  Engine_ActorSetPosition(ACTOR_CAPTAIN, PIXELS(592), PIXELS(760));
  Object_GetById(ACTOR_SOLDIER_SHOWING_THE_REWARD)->facing =
      FACING_WEST + FACING_STEP;
  Actor_SetPosition(ACTOR_SOLDIER_SHOWING_THE_REWARD, PIXELS(662), PIXELS(764));
  McCoyPalace_PositionRewardChests();
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_VIAL_CHEST), 0);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_POTION_CHEST), 0);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_PSY_CRYSTAL_CHEST), 0);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_WATER_OF_LIFE_CHEST), 0);
  actor = Object_GetById(ACTOR_VIAL_CHEST);
  actor->motionFlags = 0;
  Object_GetById(ACTOR_POTION_CHEST)->motionFlags = 0;
  Object_GetById(ACTOR_PSY_CRYSTAL_CHEST)->motionFlags = 0;
  Object_GetById(ACTOR_WATER_OF_LIFE_CHEST)->motionFlags = 0;
  Object_GetById(ACTOR_VIAL_CHEST)->y.fixed = -PIXELS(4);
  Object_GetById(ACTOR_POTION_CHEST)->y.fixed = -PIXELS(4);
  Object_GetById(ACTOR_PSY_CRYSTAL_CHEST)->y.fixed = -PIXELS(4);
  Object_GetById(ACTOR_WATER_OF_LIFE_CHEST)->y.fixed = -PIXELS(4);
  Engine_MapRedraw();
  Engine_TaskWait(1);
  gEventWork->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  gEventWork->transitionFrames = 16;
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Actor_SetSpeed(ACTOR_CAPTAIN, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_WalkTo(ACTOR_CAPTAIN, 628, 764);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 612, 764);
  Engine_ActorSetAnimation(ACTOR_CAPTAIN, 1);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(ACTOR_CAPTAIN, 1);
  // The captain and the keeper explain that only one chest may be chosen.
  Engine_EventSetMessage(MsgBilibinLordMccoyOrdered);
  Engine_EventShowMessageAndWait(ACTOR_CAPTAIN, 0, 10);
  Actor_WalkToAndWait(ACTOR_CAPTAIN, 622, 780);
  Actor_FaceDirection(ACTOR_CAPTAIN, FACING_NORTH, 10);
  Engine_ActorRunRepeatedMotion(ACTOR_SOLDIER_SHOWING_THE_REWARD, 2);
  Engine_EventShowMessageAndWait(ACTOR_SOLDIER_SHOWING_THE_REWARD, 0, 10);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_GameFlagClear(FLAG_SHOW_LOCATION_NAME);
  Engine_GameFlagSet(FLAG_SCENE_TEMP_02);
  Engine_EventEnd();
}

s32 gPalace_FacingActions[] = {
    SC_CALL(Scene_TurnTowardTarget),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gPalace_PacingActions[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(842), 0, PIXELS(696)),
    SC_SYNC,
    SC_MOVE(PIXELS(842), 0, PIXELS(636)),
    SC_SYNC,
    SC_MOVE(PIXELS(876), 0, PIXELS(634)),
    SC_SYNC,
    SC_ARG(1),
    SC_SET(SW_FACING, FACING_SOUTHEAST + FACING_STEP),
    SC_WAIT(1),
    SC_END,
};

struct SceneEntrance gMcCoyPalaceEntrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 256, 0, 808, FACING_NORTH, 0, -1, -1,
     -1, -1, 0},
    {MCCOY_PALACE_FROM_MCCOY_PALACE_GROUNDS_WEST, CONDITION_ALWAYS, 256, 0, 808,
     FACING_NORTH, 0, 32, 496, 480, 856, 0},
    {MCCOY_PALACE_HALL_FROM_SOUTHWEST_LANDING, CONDITION_ALWAYS, 120, 0, 792,
     FACING_EAST, 0, 32, 496, 480, 856, 0},
    {MCCOY_PALACE_HALL_FROM_SOUTHEAST_LANDING, CONDITION_ALWAYS, 392, 0, 792,
     FACING_WEST, 0, 32, 496, 480, 856, 0},
    {MCCOY_PALACE_HALL_FROM_LADY_MCCOYS_ROOM_WEST, CONDITION_ALWAYS, 232, 0,
     712, FACING_EAST, 0, 32, 496, 480, 856, 0},
    {MCCOY_PALACE_HALL_FROM_LADY_MCCOYS_ROOM_EAST, CONDITION_ALWAYS, 280, 0,
     712, FACING_WEST, 0, 32, 496, 480, 856, 0},
    {MCCOY_PALACE_HALL_FROM_TREASURE_ROOM, CONDITION_ALWAYS, 152, 0, 632,
     FACING_WEST, 0, 32, 496, 480, 856, 0},
    {MCCOY_PALACE_HALL_FROM_NORTHWEST_LANDING, CONDITION_ALWAYS, 104, 0, 552,
     FACING_EAST, 0, 32, 496, 480, 856, 0},
    {MCCOY_PALACE_HALL_FROM_NORTHEAST_LANDING, CONDITION_ALWAYS, 408, 0, 552,
     FACING_WEST, 0, 32, 496, 480, 856, 0},
    {MCCOY_PALACE_HALL_FROM_MCCOYS_ROOM, CONDITION_ALWAYS, 256, 0, 632,
     FACING_SOUTH, 0, 32, 496, 480, 856, 0},
    {MCCOY_PALACE_LADY_MCCOYS_ROOM_FROM_HALL_WEST, CONDITION_ALWAYS, 152, 0,
     376, FACING_WEST, 0, 56, 304, 360, 464, 0},
    {MCCOY_PALACE_LADY_MCCOYS_ROOM_FROM_HALL_EAST, CONDITION_ALWAYS, 264, 0,
     376, FACING_EAST, 0, 56, 304, 360, 464, 0},
    {MCCOY_PALACE_LADY_MCCOYS_ROOM_FROM_WEST_BEDROOM, CONDITION_ALWAYS, 104, 0,
     376, FACING_SOUTH, 0, 56, 304, 360, 464, 0},
    {MCCOY_PALACE_LADY_MCCOYS_ROOM_FROM_EAST_BEDROOM, CONDITION_ALWAYS, 312, 0,
     376, FACING_SOUTH, 0, 56, 304, 360, 464, 0},
    {MCCOY_PALACE_SOUTHWEST_LANDING_FROM_HALL, CONDITION_ALWAYS, 696, 0, 96,
     FACING_WEST, 0, 592, 0, 832, 160, 0},
    {MCCOY_PALACE_SOUTHWEST_LANDING_FROM_SOUTHWEST_TOWER, CONDITION_ALWAYS, 680,
     0, 72, FACING_WEST, 0, 592, 0, 832, 160, 0},
    {MCCOY_PALACE_SOUTHEAST_LANDING_FROM_SOUTHEAST_TOWER, CONDITION_ALWAYS, 952,
     0, 72, FACING_EAST, 0, 800, 0, 1040, 160, 0},
    {MCCOY_PALACE_SOUTHEAST_LANDING_FROM_HALL, CONDITION_ALWAYS, 936, 0, 96,
     FACING_EAST, 0, 800, 0, 1040, 160, 0},
    {MCCOY_PALACE_WEST_BEDROOM_FROM_LADY_MCCOYS_ROOM, CONDITION_ALWAYS, 88, 0,
     200, FACING_NORTH, 0, 0, 16, 240, 240, 0},
    {MCCOY_PALACE_EAST_BEDROOM_FROM_LADY_MCCOYS_ROOM, CONDITION_ALWAYS, 360, 0,
     200, FACING_NORTH, 0, 208, 16, 448, 240, 0},
    {MCCOY_PALACE_NORTHEAST_LANDING_FROM_HALL, CONDITION_ALWAYS, 536, 0, 112,
     FACING_EAST, 0, 400, 0, 640, 160, 0},
    {MCCOY_PALACE_NORTHEAST_LANDING_FROM_NORTHEAST_TOWER, CONDITION_ALWAYS, 536,
     0, 88, FACING_EAST, 0, 400, 0, 640, 160, 0},
    {MCCOY_PALACE_NORTHWEST_LANDING_FROM_HALL, CONDITION_ALWAYS, 520, 0, 312,
     FACING_WEST, 0, 400, 208, 640, 368, 0},
    {MCCOY_PALACE_NORTHWEST_LANDING_FROM_NORTHWEST_TOWER, CONDITION_ALWAYS, 520,
     0, 296, FACING_WEST, 0, 400, 208, 640, 368, 0},
    {MCCOY_PALACE_NORTHWEST_TOWER_FROM_NORTHWEST_LANDING, CONDITION_ALWAYS, 728,
     0, 264, FACING_EAST, 0, 592, 192, 832, 352, 0},
    {MCCOY_PALACE_NORTHEAST_TOWER_FROM_NORTHEAST_LANDING, CONDITION_ALWAYS, 904,
     0, 264, FACING_WEST, 0, 792, 192, 1032, 352, 0},
    {MCCOY_PALACE_SOUTHWEST_TOWER_FROM_SOUTHWEST_LANDING, CONDITION_ALWAYS, 744,
     0, 424, FACING_EAST, 0, 592, 352, 832, 512, 0},
    {MCCOY_PALACE_SOUTHEAST_TOWER_FROM_SOUTHEAST_LANDING, CONDITION_ALWAYS, 888,
     0, 424, FACING_WEST, 0, 792, 352, 1032, 512, 0},
    {MCCOY_PALACE_TREASURE_ROOM_FROM_HALL, CONDITION_ALWAYS, 600, 0, 760,
     FACING_EAST, 0, 512, 560, 752, 816, 0},
    {MCCOY_PALACE_FROM_MCCOY_PALACE_GROUNDS_EAST, CONDITION_ALWAYS, 896, 0, 728,
     FACING_NORTH, 0, 776, 560, 1016, 768, 0},
    {SCENE_TABLE_END},
};

u32 gMcCoyPalaceExits[] = {
    SCENE_EXITS(McCoyPalace),
    SCENE_EXIT(MCCOY_PALACE_TO_MCCOY_PALACE_GROUNDS_WEST, McCoyPalaceGrounds,
               MCCOY_PALACE_GROUNDS_FROM_MCCOY_PALACE),
    SCENE_EXIT(MCCOY_PALACE_HALL_TO_SOUTHWEST_LANDING, McCoyPalace,
               MCCOY_PALACE_SOUTHWEST_LANDING_FROM_HALL),
    SCENE_EXIT(MCCOY_PALACE_HALL_TO_SOUTHEAST_LANDING, McCoyPalace,
               MCCOY_PALACE_SOUTHEAST_LANDING_FROM_HALL),
    SCENE_EXIT(MCCOY_PALACE_HALL_TO_LADY_MCCOYS_ROOM_WEST, McCoyPalace,
               MCCOY_PALACE_LADY_MCCOYS_ROOM_FROM_HALL_WEST),
    SCENE_EXIT(MCCOY_PALACE_HALL_TO_LADY_MCCOYS_ROOM_EAST, McCoyPalace,
               MCCOY_PALACE_LADY_MCCOYS_ROOM_FROM_HALL_EAST),
    SCENE_EXIT(MCCOY_PALACE_HALL_TO_TREASURE_ROOM, McCoyPalace,
               MCCOY_PALACE_TREASURE_ROOM_FROM_HALL),
    SCENE_EXIT(MCCOY_PALACE_HALL_TO_NORTHWEST_LANDING, McCoyPalace,
               MCCOY_PALACE_NORTHWEST_LANDING_FROM_HALL),
    SCENE_EXIT(MCCOY_PALACE_HALL_TO_NORTHEAST_LANDING, McCoyPalace,
               MCCOY_PALACE_NORTHEAST_LANDING_FROM_HALL),
    SCENE_EXIT(MCCOY_PALACE_HALL_TO_MCCOYS_ROOM, McCoyPalace,
               MCCOY_PALACE_FROM_MCCOY_PALACE_GROUNDS_EAST),
    SCENE_EXIT(MCCOY_PALACE_LADY_MCCOYS_ROOM_TO_HALL_WEST, McCoyPalace,
               MCCOY_PALACE_HALL_FROM_LADY_MCCOYS_ROOM_WEST),
    SCENE_EXIT(MCCOY_PALACE_LADY_MCCOYS_ROOM_TO_HALL_EAST, McCoyPalace,
               MCCOY_PALACE_HALL_FROM_LADY_MCCOYS_ROOM_EAST),
    SCENE_EXIT(MCCOY_PALACE_LADY_MCCOYS_ROOM_TO_WEST_BEDROOM, McCoyPalace,
               MCCOY_PALACE_WEST_BEDROOM_FROM_LADY_MCCOYS_ROOM),
    SCENE_EXIT(MCCOY_PALACE_LADY_MCCOYS_ROOM_TO_EAST_BEDROOM, McCoyPalace,
               MCCOY_PALACE_EAST_BEDROOM_FROM_LADY_MCCOYS_ROOM),
    SCENE_EXIT(MCCOY_PALACE_SOUTHWEST_LANDING_TO_HALL, McCoyPalace,
               MCCOY_PALACE_HALL_FROM_SOUTHWEST_LANDING),
    SCENE_EXIT(MCCOY_PALACE_SOUTHWEST_LANDING_TO_SOUTHWEST_TOWER, McCoyPalace,
               MCCOY_PALACE_SOUTHWEST_TOWER_FROM_SOUTHWEST_LANDING),
    SCENE_EXIT(MCCOY_PALACE_SOUTHEAST_LANDING_TO_SOUTHEAST_TOWER, McCoyPalace,
               MCCOY_PALACE_SOUTHEAST_TOWER_FROM_SOUTHEAST_LANDING),
    SCENE_EXIT(MCCOY_PALACE_SOUTHEAST_LANDING_TO_HALL, McCoyPalace,
               MCCOY_PALACE_HALL_FROM_SOUTHEAST_LANDING),
    SCENE_EXIT(MCCOY_PALACE_WEST_BEDROOM_TO_LADY_MCCOYS_ROOM, McCoyPalace,
               MCCOY_PALACE_LADY_MCCOYS_ROOM_FROM_WEST_BEDROOM),
    SCENE_EXIT(MCCOY_PALACE_EAST_BEDROOM_TO_LADY_MCCOYS_ROOM, McCoyPalace,
               MCCOY_PALACE_LADY_MCCOYS_ROOM_FROM_EAST_BEDROOM),
    SCENE_EXIT(MCCOY_PALACE_NORTHEAST_LANDING_TO_HALL, McCoyPalace,
               MCCOY_PALACE_HALL_FROM_NORTHEAST_LANDING),
    SCENE_EXIT(MCCOY_PALACE_NORTHEAST_LANDING_TO_NORTHEAST_TOWER, McCoyPalace,
               MCCOY_PALACE_NORTHEAST_TOWER_FROM_NORTHEAST_LANDING),
    SCENE_EXIT(MCCOY_PALACE_NORTHWEST_LANDING_TO_HALL, McCoyPalace,
               MCCOY_PALACE_HALL_FROM_NORTHWEST_LANDING),
    SCENE_EXIT(MCCOY_PALACE_NORTHWEST_LANDING_TO_NORTHWEST_TOWER, McCoyPalace,
               MCCOY_PALACE_NORTHWEST_TOWER_FROM_NORTHWEST_LANDING),
    SCENE_EXIT(MCCOY_PALACE_NORTHWEST_TOWER_TO_NORTHWEST_LANDING, McCoyPalace,
               MCCOY_PALACE_NORTHWEST_LANDING_FROM_NORTHWEST_TOWER),
    SCENE_EXIT(MCCOY_PALACE_NORTHEAST_TOWER_TO_NORTHEAST_LANDING, McCoyPalace,
               MCCOY_PALACE_NORTHEAST_LANDING_FROM_NORTHEAST_TOWER),
    SCENE_EXIT(MCCOY_PALACE_SOUTHWEST_TOWER_TO_SOUTHWEST_LANDING, McCoyPalace,
               MCCOY_PALACE_SOUTHWEST_LANDING_FROM_SOUTHWEST_TOWER),
    SCENE_EXIT(MCCOY_PALACE_SOUTHEAST_TOWER_TO_SOUTHEAST_LANDING, McCoyPalace,
               MCCOY_PALACE_SOUTHEAST_LANDING_FROM_SOUTHEAST_TOWER),
    SCENE_EXIT(MCCOY_PALACE_TREASURE_ROOM_TO_HALL, McCoyPalace,
               MCCOY_PALACE_HALL_FROM_TREASURE_ROOM),
    SCENE_EXIT(MCCOY_PALACE_MCCOYS_ROOM_TO_HALL, McCoyPalace,
               MCCOY_PALACE_HALL_FROM_MCCOYS_ROOM),
    SCENE_EXITS_END,
};

struct ScenePlacement gMcCoyPalacePlacementsOther[] = {
    {SCENE_TABLE_END},
};

// clang-format off
struct ScenePlacement gMcCoyPalacePlacements[] = {
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, 0x0, ACTOR_STAND,
        PIXELS(265), 0, PIXELS(702), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, 0x0, ACTOR_STAND,
        PIXELS(342), 0, PIXELS(632), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, 0x0, ACTOR_STAND,
        PIXELS(360), 0, PIXELS(632), FACING_EAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, 0x0, ACTOR_WANDER,
        PIXELS(166), 0, PIXELS(728), FACING_EAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, 0x0, ACTOR_STAND,
        PIXELS(326), 0, PIXELS(378), FACING_SOUTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, 0x0, ACTOR_WANDER,
        PIXELS(362), 0, PIXELS(142), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, 0x0, ACTOR_STAND,
        PIXELS(296), 0, PIXELS(87), FACING_NORTHWEST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, 0x0, ACTOR_STAND,
        PIXELS(952), 0, PIXELS(289), FACING_EAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_SOLDIER_IN_SPIKED_HELMET, 0x0, ACTOR_STAND,
        PIXELS(951), 0, PIXELS(438), FACING_SOUTH + FACING_STEP, TALK_KEEP_FACING, 0 },
    { SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(616), 0, PIXELS(728), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_MCCOY, 0x0, ACTOR_STAND,
        PIXELS(894), 0, PIXELS(634), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0xf5, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_CHEST, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_CHEST, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_CHEST, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_CHEST, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTHWEST, TALK_KEEP_FACING, 0 },
    { IVAN, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTHEAST, TALK_KEEP_FACING, 0 },
    { MIA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { SPRITE_CHEST, 0x0, 7,
        PIXELS(616), 0, PIXELS(616), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_CHEST, 0x0, 7,
        PIXELS(632), 0, PIXELS(616), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_CHEST, 0x0, 7,
        PIXELS(648), 0, PIXELS(616), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_CHEST, 0x0, 7,
        PIXELS(664), 0, PIXELS(616), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_CHEST, 0x0, 7,
        PIXELS(616), 0, PIXELS(648), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_CHEST, 0x0, 7,
        PIXELS(648), 0, PIXELS(648), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_CHEST, 0x0, 7,
        PIXELS(664), 0, PIXELS(648), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_CHEST, 0x0, 7,
        PIXELS(616), 0, PIXELS(664), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_CHEST, 0x0, 7,
        PIXELS(648), 0, PIXELS(664), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_CHEST, 0x0, 7,
        PIXELS(664), 0, PIXELS(664), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SCENE_TABLE_END },
};

struct SceneEvent gMcCoyPalaceEventsOther[] = {
    { SCENE_EVENTS_END },
};

struct SceneEvent gMcCoyPalaceEvents[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, MCCOY_PALACE_TO_MCCOY_PALACE_GROUNDS_WEST },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, MCCOY_PALACE_HALL_TO_SOUTHWEST_LANDING },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, MCCOY_PALACE_HALL_TO_SOUTHEAST_LANDING },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, MCCOY_PALACE_HALL_TO_LADY_MCCOYS_ROOM_WEST },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, MCCOY_PALACE_HALL_TO_LADY_MCCOYS_ROOM_EAST },
    { EVENT_EXIT, 6, CONDITION_ALWAYS, MCCOY_PALACE_HALL_TO_TREASURE_ROOM },
    { EVENT_EXIT, 7, CONDITION_ALWAYS, MCCOY_PALACE_HALL_TO_NORTHWEST_LANDING },
    { EVENT_EXIT, 8, CONDITION_ALWAYS, MCCOY_PALACE_HALL_TO_NORTHEAST_LANDING },
    { TOUCH_FACING(FACING_NORTH), 9, CONDITION_ALWAYS, EVENT_SCRIPT(Event_WalkThroughDoor) },
    { EVENT_EXIT, 10, CONDITION_ALWAYS, MCCOY_PALACE_LADY_MCCOYS_ROOM_TO_HALL_WEST },
    { EVENT_EXIT, 11, CONDITION_ALWAYS, MCCOY_PALACE_LADY_MCCOYS_ROOM_TO_HALL_EAST },
    { TOUCH_FACING(FACING_NORTH), 12, CONDITION_ALWAYS, EVENT_SCRIPT(Event_WalkThroughDoor) },
    { TOUCH_FACING(FACING_NORTH), 13, CONDITION_ALWAYS, EVENT_SCRIPT(Event_WalkThroughDoor) },
    { EVENT_EXIT, 14, CONDITION_ALWAYS, MCCOY_PALACE_SOUTHWEST_LANDING_TO_HALL },
    { EVENT_EXIT, 15, CONDITION_ALWAYS, MCCOY_PALACE_SOUTHWEST_LANDING_TO_SOUTHWEST_TOWER },
    { EVENT_EXIT, 16, CONDITION_ALWAYS, MCCOY_PALACE_SOUTHEAST_LANDING_TO_SOUTHEAST_TOWER },
    { EVENT_EXIT, 17, CONDITION_ALWAYS, MCCOY_PALACE_SOUTHEAST_LANDING_TO_HALL },
    { EVENT_EXIT, 18, CONDITION_ALWAYS, MCCOY_PALACE_WEST_BEDROOM_TO_LADY_MCCOYS_ROOM },
    { EVENT_EXIT, 19, CONDITION_ALWAYS, MCCOY_PALACE_EAST_BEDROOM_TO_LADY_MCCOYS_ROOM },
    { EVENT_EXIT, 20, CONDITION_ALWAYS, MCCOY_PALACE_NORTHEAST_LANDING_TO_HALL },
    { EVENT_EXIT, 21, CONDITION_ALWAYS, MCCOY_PALACE_NORTHEAST_LANDING_TO_NORTHEAST_TOWER },
    { EVENT_EXIT, 22, CONDITION_ALWAYS, MCCOY_PALACE_NORTHWEST_LANDING_TO_HALL },
    { EVENT_EXIT, 23, CONDITION_ALWAYS, MCCOY_PALACE_NORTHWEST_LANDING_TO_NORTHWEST_TOWER },
    { EVENT_EXIT, 24, CONDITION_ALWAYS, MCCOY_PALACE_NORTHWEST_TOWER_TO_NORTHWEST_LANDING },
    { EVENT_EXIT, 25, CONDITION_ALWAYS, MCCOY_PALACE_NORTHEAST_TOWER_TO_NORTHEAST_LANDING },
    { EVENT_EXIT, 26, CONDITION_ALWAYS, MCCOY_PALACE_SOUTHWEST_TOWER_TO_SOUTHWEST_LANDING },
    { EVENT_EXIT, 27, CONDITION_ALWAYS, MCCOY_PALACE_SOUTHEAST_TOWER_TO_SOUTHEAST_LANDING },
    { EVENT_EXIT, 28, CONDITION_ALWAYS, MCCOY_PALACE_TREASURE_ROOM_TO_HALL },
    { EVENT_EXIT, 29, CONDITION_ALWAYS, MCCOY_PALACE_MCCOYS_ROOM_TO_HALL },
    { EVENT_TALK, ACTOR_VILLAGER_FOLLOW_RED_CARPET, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinIfYouWantToSeeLord) },
    { EVENT_TALK, ACTOR_VILLAGER_FOLLOW_RED_CARPET, CONDITION_ALWAYS, MSGNO(MsgBilibinThePeopleOfBilibinWillNever) },
    { EVENT_TALK, ACTOR_VILLAGER_DO_NOT_OFFEND_MCCOY, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinMakeSureYouDontOffendLord) },
    { EVENT_TALK, ACTOR_VILLAGER_DO_NOT_OFFEND_MCCOY, CONDITION_ALWAYS, MSGNO(MsgBilibinIHeardYouReceivedASpectacular) },
    { EVENT_TALK, ACTOR_VILLAGER_ASKING_ABOUT_KOLIMA_FOREST, FLAG_MCCOY_PALACE_REWARD_TAKEN, EVENT_SCRIPT(Event_AskAboutGoingToKolima) },
    { EVENT_TALK, ACTOR_VILLAGER_ASKING_ABOUT_KOLIMA_FOREST, CONDITION_ALWAYS, MSGNO(MsgBilibinLadyMccoyIsQuiteUpsetThat) },
    { EVENT_TALK, ACTOR_YOUNG_MAN_PROTECT_OLD_TREE, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinIAlwaysBelievedThatTheOld) },
    { EVENT_TALK, ACTOR_YOUNG_MAN_PROTECT_OLD_TREE, CONDITION_ALWAYS, MSGNO(MsgBilibinThingsAreLookingGrimSouthOf) },
    { EVENT_TALK, ACTOR_VILLAGER_LADY_MCCOYS_ROOM, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinThisIsLadyMccoysRoomSo) },
    { EVENT_TALK, ACTOR_VILLAGER_LADY_MCCOYS_ROOM, CONDITION_ALWAYS, MSGNO(MsgBilibinMiladyWasVeryUpsetWhenShe) },
    { EVENT_TALK, ACTOR_VILLAGER_MILADYS_BEAUTY, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinMiladyWasRenownedFarAndWide) },
    { EVENT_TALK, ACTOR_VILLAGER_MILADYS_BEAUTY, CONDITION_ALWAYS, MSGNO(MsgBilibinForOnceMilordDidntLetLady) },
    { EVENT_TALK, ACTOR_VILLAGER_IN_MILADYS_NIGHTGOWN, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinOhSomedayIWantToSleep) },
    { EVENT_TALK, ACTOR_VILLAGER_IN_MILADYS_NIGHTGOWN, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToWomanInMiladysNightgown) },
    { EVENT_TALK, ACTOR_OLD_MAN_PRAISING_THE_WARRIORS, FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY, MSGNO(MsgBilibinYouCanSeeKolimaPrettyWell) },
    { EVENT_TALK, ACTOR_OLD_MAN_PRAISING_THE_WARRIORS, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinHeyIfYouCantDoThe) },
    { EVENT_TALK, ACTOR_OLD_MAN_PRAISING_THE_WARRIORS, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToOldManPraisingTheWarriors) },
    { EVENT_TALK, ACTOR_SOLDIER_WATCHING_KOLIMA, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinHmmItSeemsTheCurseOf) },
    { EVENT_TALK, ACTOR_SOLDIER_WATCHING_KOLIMA, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToSoldierWatchingKolima) },
    { EVENT_TALK, ACTOR_CAPTAIN, CONDITION_ALWAYS, MSGNO(MsgBilibinYouMaySelectASingleItem) },
    { EVENT_TALK, ACTOR_SOLDIER_SHOWING_THE_REWARD, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToSoldierShowingTheReward) },
    { EVENT_TALK, ACTOR_LORD_MCCOY, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinSorryLadsButKolimasFutureIs) },
    { EVENT_TALK, ACTOR_LORD_MCCOY, CONDITION_ALWAYS, MSGNO(MsgBilibinYehveTaughtMeThatICannae) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_FOLLOW_RED_CARPET, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinHowManyTimesDoIHave) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_FOLLOW_RED_CARPET, CONDITION_ALWAYS, MSGNO(MsgBilibinIFeelSorryForTheWarriors) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_DO_NOT_OFFEND_MCCOY, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinTheyShouldNeverHaveLetSuch) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_DO_NOT_OFFEND_MCCOY, CONDITION_ALWAYS, MSGNO(MsgBilibinIWonderWhatKindOfTreasure) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ASKING_ABOUT_KOLIMA_FOREST, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinSendingTheseChildrenToKolimaForest) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ASKING_ABOUT_KOLIMA_FOREST, CONDITION_ALWAYS, MSGNO(MsgBilibinIWishShedJustStopWhining) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_PROTECT_OLD_TREE, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinIShouldntHaveSaidAnythingWhen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_PROTECT_OLD_TREE, CONDITION_ALWAYS, MSGNO(MsgBilibinTheydBeGuaranteedAGoodLife) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_LADY_MCCOYS_ROOM, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinOfAllTheNerveComingInto) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_LADY_MCCOYS_ROOM, CONDITION_ALWAYS, MSGNO(MsgBilibinICanTellWhenShesUpset) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_MILADYS_BEAUTY, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinWillIEverFindSomeoneTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_MILADYS_BEAUTY, CONDITION_ALWAYS, MSGNO(MsgBilibinSometimesAMansJustGottaPut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_IN_MILADYS_NIGHTGOWN, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinOhNoThisNightgownHasA) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_IN_MILADYS_NIGHTGOWN, CONDITION_ALWAYS, MSGNO(MsgBilibinTheHoleInThisNightgownIs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_PRAISING_THE_WARRIORS, FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY, MSGNO(MsgBilibinLordMccoyHasntComeHereFor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_PRAISING_THE_WARRIORS, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinItsAToughThingBeingSo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_PRAISING_THE_WARRIORS, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadOldMansThoughtsOnTheWarriors) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SOLDIER_WATCHING_KOLIMA, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinKolimaForestUsedToBeSo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SOLDIER_WATCHING_KOLIMA, CONDITION_ALWAYS, MSGNO(MsgBilibinACourageousWarriorWouldntThinkTwice) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CAPTAIN, CONDITION_ALWAYS, MSGNO(MsgBilibinTheseCourageousWarriorsDeserveToReceive) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SOLDIER_SHOWING_THE_REWARD, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadRewardSoldiersThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LORD_MCCOY, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinThereIsNothingMoreToTalk) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LORD_MCCOY, CONDITION_ALWAYS, MSGNO(MsgBilibinButICannaeFiggerOutHow) },
    { SEARCH(SEARCH_UNNAMED), 100, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SearchVialChest) },
    { SEARCH(SEARCH_UNNAMED), 101, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SearchPotionChest) },
    { SEARCH(SEARCH_UNNAMED), 102, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SearchPsyCrystalChest) },
    { SEARCH(SEARCH_UNNAMED), 103, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SearchWaterOfLifeChest) },
    { EVENT_TOUCH, 50, FLAG_PLACE_TEMP_33, EVENT_SCRIPT(Event_CaptainSeesPartyOff) },
    { EVENT_TOUCH, 51, FLAG_PLACE_TEMP_33, EVENT_SCRIPT(Event_CaptainAsksPartyToChoose) },
    { SEARCH(SEARCH_BARREL), 120, FLAG_MCCOY_PALACE_BARREL_ELIXIR, FIND_ITEM(ITEM_ELIXIR) },
    { SEARCH(SEARCH_BARREL), 121, FLAG_MCCOY_PALACE_BARREL_SMOKE_BOMB, FIND_ITEM(ITEM_SMOKE_BOMB) },
    { SEARCH(SEARCH_WARDROBE), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBilibinTheresASilkNegligeeIsaacPut)) },
    { SEARCH(SEARCH_BOOKCASE), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBilibinTheresASheetOfPaperTitled)) },
    { SCENE_EVENTS_END },
};
// clang-format on
