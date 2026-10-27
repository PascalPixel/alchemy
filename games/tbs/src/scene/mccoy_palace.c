/// @file McCoy's Palace in Bilibin: SceneId_McCoyPalace, with the area name
/// McCoy's Palace, which its grounds share (mccoy_palace_grounds.c). The party
/// comes in from the grounds by MCCOY_PALACE_FROM_MCCOY_PALACE_GROUNDS_WEST,
/// or straight into McCoy's room by
/// MCCOY_PALACE_FROM_MCCOY_PALACE_GROUNDS_EAST, which the red carpet's door
/// (step trigger 9) also reaches. Its people follow three story flags,
/// FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY, FLAG_KOLIMA_CURSE_LIFTED and
/// FLAG_MCCOY_PALACE_REWARD_TAKEN, in what they say and how they turn when
/// talked to (Scene_GetActorTemplates).
///
/// Arriving in McCoy's room before the curse is lifted plays
/// McCoyPalace_AudienceWithMcCoy: McCoy asks whether the party can save Kolima.
/// A refusal sends it away (FLAG_MCCOY_PALACE_JOB_REFUSED) to come back later;
/// an acceptance ends with McCoy taking back the key to the barricades and the
/// captain seeing the party out (FLAG_CAPTAIN_SEEING_PARTY_OUT). Outside
/// McCoy's door the captain then plays McCoyPalace_CaptainHintsAtBarricade,
/// which sets FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY.
///
/// Once the curse is lifted, the grounds' guards bring the party into McCoy's
/// room, and McCoyPalace_McCoyRewardsParty plays until
/// FLAG_MCCOY_PALACE_PARTY_REWARDED; McCoy's lines differ as the party has met
/// him before or not. The captain takes the party to the treasure room
/// (FLAG_REWARD_OFFERED), where McCoyPalace_PresentRewardChests shows four
/// chests: a search of one gives its Vial, Potion, Psy Crystal or Water of Life
/// (McCoyPalace_ClaimReward), and the others are then locked. The chest taken
/// from is kept only in map temp flags (FLAG_TOOK_VIAL and the others), so once
/// the party has left the palace all four search as locked. Leaving without
/// a reward (step trigger 51), the captain walks the leader back; with it (
/// step trigger 50) he sees the party off (FLAG_PARTY_SEEN_OFF).
///
/// Two barrels hold an Elixir and a Smoke Bomb, a wardrobe a silk negligee
/// and a bookcase the list of McCoy's gifts to his wife. McCoy speaks with his
/// portrait (SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY).

#include "save_block.h"
#include "scene_ids.h"
#include "unit/scene/mccoy_palace.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/event_commands.h"
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
#include "event_layer.h"
#include "event_inline.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "scene_tables.h"
#include "item_ids.h"
#include "actor_script.h"
#include "fixed_math.h"
#include "songs.h"

/// The palace's people and the treasure room's chests, numbering
/// gMcCoyPalaceActorTemplates' rows from ACTOR_FIRST_PLACED. Each note
/// gives what they say before the reward, then after it.
enum McCoyPalaceActorId {
  /// A burly grey-haired man; points the way to McCoy. "If you want to see Lord
  /// McCoy, follow the red carpet."; later "The people of Bilibin will never
  /// forget your bravery."
  /// ★ Named from the game: "If you want to see Lord McCoy, follow the red
  /// carpet." (MsgBilibinIfYouWantToSeeLord)
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
  /// A young man with brown hair; speaks of the old tree in Kolima Forest;
  /// later "Things are looking grim south of Goma Range..."
  /// ★ Named from the game: "I always believed that the old tree in Kolima
  /// Forest should be protected!" (MsgBilibinIAlwaysBelievedThatTheOld)
  ACTOR_YOUNG_MAN_PROTECT_OLD_TREE,
  /// A woman with fair hair standing in Lady McCoy's room. "This is Lady
  /// McCoy's room, so don't touch anything."; later "Milady was very upset when
  /// she left the room."
  /// ★ Named from the game: "This is Lady McCoy's room, so don't touch
  /// anything." (MsgBilibinThisIsLadyMccoysRoomSo)
  ACTOR_VILLAGER_LADY_MCCOYS_ROOM,
  /// A woman with fair hair who admires Lady McCoy. "Milady was renowned far
  /// and wide for her beauty!"; later "For once, milord didn't let Lady McCoy
  /// push him around!"
  /// ★ Named from the game: "Milady was renowned far and wide for her beauty!"
  /// (MsgBilibinMiladyWasRenownedFarAndWide)
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

/// Animations of the reward chests' sprite, SPRITE_CHEST, from its animation
/// table (graphics/sprite/spr_020.tsv).
enum {
  CHEST_ANIMATION_OPEN_EMPTY = 2, ///< the lid up over an empty chest, held
  /// The lid lifted on the treasure for six frames, then shut again (the
  /// sprite's animation 1, closed).
  CHEST_ANIMATION_PEEK_AND_SHUT = 5
};

/// The palace's temporary flags: the map temp flags last until the party leaves
/// the palace's map, the area temp flags until it leaves McCoy's Palace.
enum {
  /// The captain has opened McCoy's door, so walking through it is silent.
  /// ★ Named from its area name: McCoy's Palace
  FLAG_MCCOYS_DOOR_OPEN = FLAG_SCENE_TEMP_00,
  /// The party is choosing its reward among the four chests.
  /// ★ Named from the game: "You may select a single item from among these
  /// four chests." (MsgBilibinYouMaySelectASingleItem)
  FLAG_CHOOSING_REWARD = FLAG_SCENE_TEMP_02,
  /// The chest the reward was taken from, which then searches as empty.
  /// ★ Named from the game: "Vial" (MsgItemName)
  FLAG_TOOK_VIAL = FLAG_SCENE_TEMP_10,
  /// ★ Named from the game: "Potion" (MsgItemName)
  FLAG_TOOK_POTION = FLAG_SCENE_TEMP_11,
  /// ★ Named from the game: "Psy Crystal" (MsgItemName)
  FLAG_TOOK_PSY_CRYSTAL = FLAG_SCENE_TEMP_12,
  /// ★ Named from the game: "Water of Life" (MsgItemName)
  FLAG_TOOK_WATER_OF_LIFE = FLAG_SCENE_TEMP_13,
  /// The old man has praised the party once; he then warns of the holy tree.
  /// ★ Named from the game: "You are a fine warrior." (MsgBilibinFineWarrior)
  FLAG_OLD_MAN_HAS_PRAISED = FLAG_AREA_TEMP_02,
  /// Mind Read has heard the old man's first thought.
  /// ★ Named from the game: "Sometimes, we need children to remind us of our
  /// own responsibilities." (MsgBilibinSometimesWeNeedChildrenRemind)
  FLAG_OLD_MAN_THOUGHTS_READ = FLAG_AREA_TEMP_03,
  /// McCoy has had the captain see the party out after the first audience.
  /// ★ Named from the game: "I dinnae want tae talk about it anymore.
  /// Captain?" (MsgBilibinWellSGoing)
  FLAG_CAPTAIN_SEEING_PARTY_OUT = FLAG_AREA_TEMP_21,
  /// McCoy has had the captain take the party to the treasure room, until it
  /// takes a reward.
  /// ★ Named from the game: "Please show them their reward!"
  /// (MsgBilibinLordMccoyOrdered)
  FLAG_REWARD_OFFERED = FLAG_AREA_TEMP_22,
  /// The captain has seen the party off with its reward.
  /// ★ Named from the game: "You are always welcome in the palace of Lord
  /// McCoy." (MsgBilibinAlwaysWelcomeInPalaceLord)
  FLAG_PARTY_SEEN_OFF = FLAG_AREA_TEMP_33
};

static void McCoyPalace_ClaimReward(s32 chestActorId, s32 itemId,
                                    s32 chestFlag);
static void McCoyPalace_AudienceWithMcCoy(void);
static void McCoyPalace_CaptainHintsAtBarricade(void);
static void McCoyPalace_McCoyRewardsParty(void);
static void McCoyPalace_PresentRewardChests(void);

// The shared facing callback precedes the map's entry points.
#include "common/actor_facing.h"

const struct SceneEntrance *Scene_GetEntrances(void) {
  return gMcCoyPalaceEntrances;
}

const struct SceneExitRegion *Scene_GetExitRegions(void) { return NULL; }

const u32 *Scene_GetExits(void) { return gMcCoyPalaceExits; }

/// The people of McCoy's Palace. Once the reward is taken, the woman in
/// Milady's nightgown, the old man who praises the warriors, the soldier who
/// watched the party go to Kolima and the soldier who keeps the treasure turn
/// differently when talked to.
const struct ActorTemplate *Scene_GetActorTemplates(void) {
  struct ActorTemplate *templates;

  if (gSaveBlock.sceneId == (s32)&SceneId_McCoyPalace) {
    templates = gMcCoyPalaceActorTemplates;
    Scene_AssignViewFlags(templates);

    if (GameFlag_Get(FLAG_MCCOY_PALACE_REWARD_TAKEN) != 0) {
      // Through the global, not templates: the local splits each address into
      // base+144 and member+22 (9 of 38 instructions differ in tbs-en).
      gMcCoyPalaceActorTemplates[ACTOR_VILLAGER_IN_MILADYS_NIGHTGOWN -
                                 ACTOR_FIRST_PLACED]
          .talkFacing = TALK_FACING_KEEP;
      gMcCoyPalaceActorTemplates[ACTOR_OLD_MAN_PRAISING_THE_WARRIORS -
                                 ACTOR_FIRST_PLACED]
          .talkFacing = TALK_FACING_PARTY;
      gMcCoyPalaceActorTemplates[ACTOR_SOLDIER_WATCHING_KOLIMA -
                                 ACTOR_FIRST_PLACED]
          .talkFacing = 3;
      gMcCoyPalaceActorTemplates[ACTOR_SOLDIER_SHOWING_THE_REWARD -
                                 ACTOR_FIRST_PLACED]
          .talkFacing = TALK_FACING_PARTY_AND_BACK;
    }

    return templates;
  }
  return gMcCoyPalaceNoActorTemplates;
}

/// What the party can talk to and search in the palace.
const struct Trigger *Scene_GetTriggers(void) {
  if (gSaveBlock.sceneId == (s32)&SceneId_McCoyPalace) {
    return gMcCoyPalaceTriggers;
  }
  return gMcCoyPalaceNoTriggers;
}

/// McCoy's Palace, before the reward: a woman with fair hair asks about going
/// to Kolima. "Oh, are you willing to go to Kolima Forest?"
static void McCoyPalace_AskAboutGoingToKolima(void) {
  Event_Start();
  Event_SetMessageId(MsgBilibinYouWillingGoKolimaForest);
  Event_AskYesNoAndShowResponse(ACTOR_VILLAGER_ASKING_ABOUT_KOLIMA_FOREST, 0);
  Event_End();
}

/// McCoy's Palace, after the reward: the woman with Lady McCoy's silk
/// nightgown screams, then turns to the party. "AAAAH!!!"
static void McCoyPalace_TalkToWomanInMiladysNightgown(void) {
  Event_Start();
  Actor_ShowEmote(ACTOR_VILLAGER_IN_MILADYS_NIGHTGOWN,
                  EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_RunRepeatedMotion(ACTOR_VILLAGER_IN_MILADYS_NIGHTGOWN, 2);
  Event_WaitFrames(40);
  Event_SetMessageId(MsgBilibinAaaah);
  FieldEvent_ShowMessageThenWait(ACTOR_VILLAGER_IN_MILADYS_NIGHTGOWN, 0, 20);
  Actor_FaceActor(ACTOR_VILLAGER_IN_MILADYS_NIGHTGOWN, ACTOR_PARTY_LEADER, 0);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(ACTOR_VILLAGER_IN_MILADYS_NIGHTGOWN, 0, 10);
  Actor_FaceDirection(ACTOR_VILLAGER_IN_MILADYS_NIGHTGOWN,
                      FACING_NORTHWEST + FACING_STEP, 10);
  Event_End();
}

/// McCoy's Palace, after the reward: the old man praises the party the first
/// time, then warns of the holy tree.
/// "You are a fine warrior." After, "It's very reckless for such young
/// warriors to face the holy tree..."
static void McCoyPalace_TalkToOldManPraisingTheWarriors(void) {
  Event_Start();
  Event_SetMessageId(MsgBilibinFineWarrior);
  if (Event_GetFlag(FLAG_OLD_MAN_HAS_PRAISED) != 0) {
    Event_SetMessageId(MsgBilibinItsVeryRecklessForSuch);
  }
  Event_ShowMessage(ACTOR_OLD_MAN_PRAISING_THE_WARRIORS, 0);
  GameFlag_Set(FLAG_OLD_MAN_HAS_PRAISED);
  Event_End();
}

/// McCoy's Palace, after the reward: a soldier saw the party go to Kolima.
/// "I was watching from here. After all that fuss, you still went to Kolima?"
static void McCoyPalace_TalkToSoldierWatchingKolima(void) {
  Event_Start();
  Event_SetMessageId(MsgBilibinWasWatchingFromHereAfter);
  Event_AskYesNoAndShowResponse(ACTOR_SOLDIER_WATCHING_KOLIMA, 0);
  Event_End();
}

/// McCoy's Palace, the treasure room: the soldier who keeps the treasure. While
/// the party chooses, only one item; after, the reward was the greatest;
/// before, Bilibin's great treasures go to whoever ends the curse.
///
/// Unused: with flag 0x84d set he would say `Pick any one of the four
/// treasures.` instead, but nothing in the game names or sets that flag.
static void McCoyPalace_TalkToSoldierShowingTheReward(void) {
  Event_Start();

  if (GameFlag_Get(FLAG_CHOOSING_REWARD) != 0) {
    Event_SetMessageId(MsgBilibinMayChooseOnlyOneItem);
  } else if (GameFlag_Get(FLAG_MCCOY_PALACE_REWARD_TAKEN) != 0) {
    Event_SetMessageId(MsgBilibinRewardReceivedWasIndeedGreatest);
  } else {
    Event_SetMessageId(MsgBilibinDoThinkTheseBilibinsGreat);
    if (GameFlag_Get(0x84d) != 0) {
      gFieldWork->message++;
    }
  }

  Event_ShowMessageAndWait(ACTOR_SOLDIER_SHOWING_THE_REWARD, 0);
  Event_End();
}

/// McCoy's Palace, after the reward: Mind Read on the old man shows two
/// thoughts. "Sometimes, we need children to remind us of our own
/// responsibilities." Then "You looked very courageous walking toward
/// Kolima."
static void McCoyPalace_ReadOldMansThoughtsOnTheWarriors(void) {
  Event_Start();
  Event_SetMessageId(MsgBilibinSometimesWeNeedChildrenRemind);
  if (Event_GetFlag(FLAG_OLD_MAN_THOUGHTS_READ) != 0) {
    Event_SetMessageId(MsgBilibinLookedVeryCourageousWalkingToward);
  }
  Event_ShowMessage(ACTOR_OLD_MAN_PRAISING_THE_WARRIORS, 0);
  GameFlag_Set(FLAG_OLD_MAN_THOUGHTS_READ);
  Event_End();
}

/// McCoy's Palace, the treasure room: Mind Read on the soldier who keeps the
/// treasure, whose thoughts follow the curse and the reward.
static void McCoyPalace_ReadRewardSoldiersThoughts(void) {
  Event_Start();

  if (GameFlag_Get(FLAG_CHOOSING_REWARD) != 0) {
    Event_SetMessageId(MsgBilibinNonethelessIfYourLuckSour);
  } else if (GameFlag_Get(FLAG_KOLIMA_CURSE_LIFTED) == 0) {
    Event_SetMessageId(MsgBilibinGoodTreasureIfGetTurned);
  } else {
    Event_SetMessageId(MsgBilibinWeReallyGivingOurTreasure);
    if (GameFlag_Get(FLAG_MCCOY_PALACE_REWARD_TAKEN) != 0) {
      Event_SetMessageId(MsgBilibinGotPrettyNiceRewardBut);
    }
  }

  Event_ShowMessageAndWait(ACTOR_SOLDIER_SHOWING_THE_REWARD, 0);
  Event_End();
}

/// McCoy's Palace, step triggers 9, 12 and 13: walking north through a door
/// opens it with a sound, the leader walks through and the next room loads.
/// McCoy's door (step trigger 9) makes no sound once the captain has opened it.
static void McCoyPalace_WalkThroughDoor(void) {
  struct FieldWork *work;
  s32 hideDoorLayers;

  work = gFieldWork;
  hideDoorLayers = 0;
  if (work->stepTrigger == 9) {
    if (GameFlag_Get(FLAG_MCCOYS_DOOR_OPEN) == 0) {
      Sound_PlaySong(SE_THUMP);
      hideDoorLayers = 1;
    }
  } else {
    Sound_PlaySong(SE_DOOR_OPEN);
    hideDoorLayers = 1;
  }
  if (hideDoorLayers != 0) {
    TilesetAnim_Resume(1);
    TilesetAnim_Resume(2);
  }
  Event_Start();
  Event_WaitFrames(10);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  if (work->stepTrigger == 9)
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -16);
  else
    Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 3, -16);
  Event_WaitFrames(16);
  Import_EventSetExitRequest(work->stepTrigger);
  Event_End();
  TilesetAnim_Pause(1);
  TilesetAnim_Pause(2);
}

/// The chest is empty. "Isaac checked the chest... but it was empty."
static void McCoyPalace_ShowChestEmpty(void) {
  Event_Start();
  FieldMessage_Show(MsgBilibinIsaacCheckedChestButWas, 1);
  Event_End();
}

/// The chest is locked. "The treasure chest is locked."
static void McCoyPalace_ShowChestLocked(void) {
  Event_Start();
  FieldMessage_Show(MsgBilibinTreasureChestLocked, 1);
  Event_End();
}

/// Searching a reward chest after the reward: the chosen one is empty, the
/// others locked.
static void McCoyPalace_ShowUnavailableChest(s32 chestFlag) {
  if (GameFlag_Get(chestFlag) != 0) {
    McCoyPalace_ShowChestEmpty();
  } else {
    McCoyPalace_ShowChestLocked();
  }
}

/// McCoy's Palace, the treasure room: searching the first reward chest gives
/// a Vial, while no reward is taken.
static void McCoyPalace_SearchVialChest(void) {
  if (GameFlag_Get(FLAG_MCCOY_PALACE_REWARD_TAKEN) != 0) {
    McCoyPalace_ShowUnavailableChest(FLAG_TOOK_VIAL);
  } else {
    McCoyPalace_ClaimReward(ACTOR_VIAL_CHEST, ITEM_VIAL, FLAG_TOOK_VIAL);
  }
}

/// McCoy's Palace, the treasure room: searching the second reward chest gives
/// a Potion, while no reward is taken.
static void McCoyPalace_SearchPotionChest(void) {
  if (GameFlag_Get(FLAG_MCCOY_PALACE_REWARD_TAKEN) != 0) {
    McCoyPalace_ShowUnavailableChest(FLAG_TOOK_POTION);
  } else {
    McCoyPalace_ClaimReward(ACTOR_POTION_CHEST, ITEM_POTION, FLAG_TOOK_POTION);
  }
}

/// McCoy's Palace, the treasure room: searching the third reward chest gives
/// a Psy Crystal, while no reward is taken.
static void McCoyPalace_SearchPsyCrystalChest(void) {
  if (GameFlag_Get(FLAG_MCCOY_PALACE_REWARD_TAKEN) != 0) {
    McCoyPalace_ShowUnavailableChest(FLAG_TOOK_PSY_CRYSTAL);
  } else {
    McCoyPalace_ClaimReward(ACTOR_PSY_CRYSTAL_CHEST, ITEM_PSY_CRYSTAL,
                            FLAG_TOOK_PSY_CRYSTAL);
  }
}

/// McCoy's Palace, the treasure room: searching the fourth reward chest gives
/// a Water of Life, while no reward is taken.
static void McCoyPalace_SearchWaterOfLifeChest(void) {
  if (GameFlag_Get(FLAG_MCCOY_PALACE_REWARD_TAKEN) != 0) {
    McCoyPalace_ShowUnavailableChest(FLAG_TOOK_WATER_OF_LIFE);
  } else {
    McCoyPalace_ClaimReward(ACTOR_WATER_OF_LIFE_CHEST, ITEM_WATER_OF_LIFE,
                            FLAG_TOOK_WATER_OF_LIFE);
  }
}

/// The chest opens and gives its item; with no room for it, the chest
/// closes again and the reward is still to choose.
static void McCoyPalace_ClaimReward(s32 chestActorId, s32 itemId,
                                    s32 chestFlag) {
  s32 emitterHandle;

  Event_Start();

  emitterHandle =
      FieldEffect_PlayCueAndStartEmitterOnTarget(0, chestActorId, itemId);

  if (PartyInventory_GiveItem(itemId, 0) != -1) {
    Import_ActorSetAnimation(chestActorId, CHEST_ANIMATION_OPEN_EMPTY);
    GameFlag_Set(FLAG_MCCOY_PALACE_REWARD_TAKEN);
    GameFlag_Set(chestFlag);
    GameFlag_Clear(FLAG_REWARD_OFFERED);
    GameFlag_Clear(FLAG_CHOOSING_REWARD);
  } else {
    Sound_PlaySong(SE_CHEST_CLOSE);
    Import_ActorSetAnimation(chestActorId, CHEST_ANIMATION_PEEK_AND_SHUT);
  }

  Actor_Release((struct Actor *)emitterHandle);
  Event_End();
}

/// McCoy's Palace, step trigger 51, leaving the treasure room without a reward:
/// the captain walks the leader back. "Please take your reward before your
/// depart!"
static void McCoyPalace_CaptainAsksPartyToChoose(void) {
  if (GameFlag_Get(FLAG_MCCOY_PALACE_REWARD_TAKEN) == 0) {
    if (GameFlag_Get(FLAG_REWARD_OFFERED) != 0) {
      Event_Start();
      Actor_ShowEmote(ACTOR_CAPTAIN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
      Actor_FaceDirection(ACTOR_CAPTAIN, FACING_SOUTHWEST + FACING_STEP, 10);
      Actor_RunRepeatedMotion(ACTOR_CAPTAIN, 2);
      Event_WaitFrames(20);
      Event_SetMessageId(MsgBilibinPleaseTakeYourRewardBefore);
      Event_ShowMessage(ACTOR_CAPTAIN, 0);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 616, 762);
      Actor_FaceDirection(ACTOR_CAPTAIN, FACING_NORTH + FACING_STEP, 10);
      Event_End();
    }
  }
}

/// McCoy's Palace, step trigger 50, leaving the treasure room with a reward:
/// the captain sees the party off. "You are always welcome in the palace of
/// Lord McCoy."
static void McCoyPalace_CaptainSeesPartyOff(void) {
  if (GameFlag_Get(FLAG_MCCOY_PALACE_REWARD_TAKEN) != 0) {
    Event_Start();
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_CAPTAIN, 0);
    Actor_SetSpeed(ACTOR_CAPTAIN, PIXELS(3) / 5, PIXELS(3) / 10);
    Actor_WalkToAndWait(ACTOR_CAPTAIN, 622, 764);
    Actor_FaceDirection(ACTOR_CAPTAIN, FACING_NORTHEAST + FACING_STEP, 20);
    Actor_SetAnimationAndWait(ACTOR_CAPTAIN, 3);
    Actor_SetAnimationAndWait(ACTOR_SOLDIER_SHOWING_THE_REWARD, 3);
    Event_WaitFrames(20);
    Actor_FaceActor(ACTOR_CAPTAIN, ACTOR_PARTY_LEADER, 0);
    Event_WaitFrames(20);
    Actor_SetAnimationAndWait(ACTOR_CAPTAIN, 3);
    Event_SetMessageId(MsgBilibinAlwaysWelcomeInPalaceLord);
    FieldEvent_ShowMessageThenWait(ACTOR_CAPTAIN, 0, 10);
    Actor_SetSpeed(ACTOR_CAPTAIN, PIXELS(4) / 5, PIXELS(2) / 5);
    Actor_WalkToAndWait(ACTOR_CAPTAIN, 570, 758);
    Actor_SetPosition(ACTOR_CAPTAIN, 0, 0);
    gFieldWork->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
    GameFlag_Set(FLAG_MCCOY_PALACE_PARTY_REWARDED);
    GameFlag_Set(FLAG_PARTY_SEEN_OFF);
    Event_End();
  }
}

/// The four reward chests stand in a row.
static __inline__ void McCoyPalace_PositionRewardChests(void) {
  Actor_SetPosition(ACTOR_VIAL_CHEST, PIXELS(616), PIXELS(728));
  Actor_SetPosition(ACTOR_POTION_CHEST, PIXELS(632), PIXELS(728));
  Actor_SetPosition(ACTOR_PSY_CRYSTAL_CHEST, PIXELS(648), PIXELS(728));
  Import_ActorSetPosition(ACTOR_WATER_OF_LIFE_CHEST, PIXELS(664), PIXELS(728));
}

/// The cells before the chest row can be walked on.
static __inline__ void McCoyPalace_OpenRewardChestRow(void) {
  MapGrid_CopyCellAttributes(38, 55, 4, 1, 38, 45);
  MapGrid_CopyCellAttributes(42, 55, 4, 1, 38, 46);
}

/// Sets McCoy's Palace up: McCoy's first audience, the captain outside McCoy's
/// door after it, the reward audience once Kolima's curse is lifted, or the
/// reward chests where the party left them.
s32 Scene_Initialize(void) {
  s32 rewardTaken;
  s32 curseLifted;
  struct Actor *chest;

  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  TilesetAnim_Pause(1);
  TilesetAnim_Pause(2);
  GameFlag_Set(FLAG_MCCOY_PALACE_VISITED);
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) != 0) {
    GameFlag_Clear(FLAG_MCCOYS_DOOR_OPEN);
  }
  // A first audience and a return visit have different greetings.
  if (GameFlag_Get(FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY) == 0 &&
      GameFlag_Get(FLAG_KOLIMA_CURSE_LIFTED) == 0) {
    if (gSaveBlock.entrance == MCCOY_PALACE_FROM_MCCOY_PALACE_GROUNDS_EAST) {
      McCoyPalace_AudienceWithMcCoy();
    } else if (gSaveBlock.entrance ==
                   MCCOY_PALACE_ENTRANCE_ROOM_FROM_MCCOYS_ROOM &&
               GameFlag_Get(FLAG_CAPTAIN_SEEING_PARTY_OUT) != 0) {
      McCoyPalace_CaptainHintsAtBarricade();
    }
  } else {
    rewardTaken = GameFlag_Get(FLAG_MCCOY_PALACE_REWARD_TAKEN);
    if (rewardTaken != 0) {
    } else {
      if (gSaveBlock.entrance == MCCOY_PALACE_FROM_MCCOY_PALACE_GROUNDS_EAST) {
        if (GameFlag_Get(FLAG_MCCOY_PALACE_PARTY_REWARDED) == 0) {
          curseLifted = GameFlag_Get(FLAG_KOLIMA_CURSE_LIFTED);
          if (curseLifted != 0) {
            McCoyPalace_McCoyRewardsParty();
          }
        }
      } else {
        // Restore the displayed chest row after reloading the treasure room.
        if (gSaveBlock.entrance ==
            MCCOY_PALACE_TREASURE_ROOM_FROM_ENTRANCE_ROOM) {
          if (GameFlag_Get(FLAG_REWARD_OFFERED) != 0) {
            if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) != 0) {
              McCoyPalace_OpenRewardChestRow();
              McCoyPalace_PositionRewardChests();
              chest = Actor_GetById(ACTOR_VIAL_CHEST);
              Actor_SetSpriteFlags(chest, 0);
              chest = Actor_GetById(ACTOR_POTION_CHEST);
              Actor_SetSpriteFlags(chest, 0);
              chest = Actor_GetById(ACTOR_PSY_CRYSTAL_CHEST);
              Actor_SetSpriteFlags(chest, 0);
              chest = Actor_GetById(ACTOR_WATER_OF_LIFE_CHEST);
              Actor_SetSpriteFlags(chest, 0);
              Actor_GetById(ACTOR_VIAL_CHEST)->motionFlags = rewardTaken;
              Actor_GetById(ACTOR_POTION_CHEST)->motionFlags = rewardTaken;
              Actor_GetById(ACTOR_PSY_CRYSTAL_CHEST)->motionFlags = rewardTaken;
              Actor_GetById(ACTOR_WATER_OF_LIFE_CHEST)->motionFlags =
                  rewardTaken;
              chest = Actor_GetById(ACTOR_VIAL_CHEST);
              chest->y.fixed = -PIXELS(4);
              chest = Actor_GetById(ACTOR_POTION_CHEST);
              chest->y.fixed = -PIXELS(4);
              chest = Actor_GetById(ACTOR_PSY_CRYSTAL_CHEST);
              chest->y.fixed = -PIXELS(4);
              chest = Actor_GetById(ACTOR_WATER_OF_LIFE_CHEST);
              chest->y.fixed = -PIXELS(4);
            } else {
              McCoyPalace_PresentRewardChests();
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
  Actor_SetSpeed(ACTOR_GARET, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_SetSpeed(ACTOR_IVAN, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_SetSpeed(ACTOR_MIA, PIXELS(1), PIXELS(1) / 2);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_WALK);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_WALK);
  Import_ActorSetAnimation(ACTOR_MIA, ANIM_WALK);
}

/// McCoy's Palace, through McCoy's door before the curse is lifted, the first
/// audience: the captain announces the party and McCoy asks whether they can
/// save Kolima. Refusing, he calls them too young and they leave; coming back,
/// they may change their minds. Accepting, McCoy gives the key to the
/// barricades, Garet bristles at being called children, and McCoy takes the
/// key back and has the captain see them out. The party arrives at
/// MCCOY_PALACE_FROM_MCCOY_PALACE_GROUNDS_EAST, beyond the door.
static void McCoyPalace_AudienceWithMcCoy(void) {
  struct Actor *actor;

  // Stage the first audience, or place the leader for a return visit.
  Event_Start();
  FieldCamera_MoveTo(-1, -1, -1, 0);
  WaitFrames(1);
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_MoveTo(PIXELS(894), -1, PIXELS(664), 0);
  WaitFrames(1);
  Import_ActorSetPosition(ACTOR_PARTY_LEADER, 0, 0);
  if (Event_GetFlag(FLAG_MCCOY_PALACE_JOB_REFUSED) != 0) {
    FieldCamera_MoveTo(PIXELS(894), -1, PIXELS(698), 0);
    Actor_SetPosition(ACTOR_CAPTAIN, PIXELS(876), PIXELS(634));
    Actor_FaceDirection(ACTOR_CAPTAIN, FACING_SOUTHEAST + FACING_STEP, 0);
    Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(894), PIXELS(798));
  }
  MapGrid_CenterOnFollowedPoint();
  WaitFrames(1);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  gFieldWork->transitionFrames = 40;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  if (GameFlag_Get(FLAG_MCCOY_PALACE_JOB_REFUSED) == 0) {
    Event_WaitFrames(80);
    Actor_SetPosition(ACTOR_CAPTAIN, PIXELS(894), PIXELS(798));
    FieldCamera_SetSpeed(PIXELS(3) / 5, PIXELS(3) / 40);
    FieldCamera_MoveTo(PIXELS(894), -1, PIXELS(698), 1);
    Actor_SetSpeed(ACTOR_CAPTAIN, PIXELS(4) / 5, PIXELS(2) / 5);
    Actor_WalkTo(ACTOR_CAPTAIN, 894, 696);
    Event_WaitFrames(80);
    FieldCamera_MoveTo(PIXELS(894), -1, PIXELS(664), 1);
    Actor_WaitForMoveById(ACTOR_CAPTAIN);
    Actor_WalkToAndWait(ACTOR_CAPTAIN, 842, 696);
    Actor_WalkToAndWait(ACTOR_CAPTAIN, 842, 636);
    Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTHWEST + FACING_STEP, 20);
    Actor_WalkToAndWait(ACTOR_CAPTAIN, 876, 634);
    Actor_SetAnimationAndWait(ACTOR_CAPTAIN, 3);
    Event_WaitFrames(20);
    Actor_SetAnimationAndWait(ACTOR_LORD_MCCOY, 3);
    Event_WaitFrames(10);
    // The captain announces the warriors before they enter the audience.
    Event_SetMessageId(MsgBilibinMatter);
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0,
                                   10);
    Actor_RunRepeatedMotion(ACTOR_CAPTAIN, 2);
    Event_ShowMessageThenWait(ACTOR_CAPTAIN, 0, 20);
    Actor_RunRepeatedMotion(ACTOR_LORD_MCCOY, 1);
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0,
                                   10);
    Actor_SetAnimationAndWait(ACTOR_CAPTAIN, 3);
    Event_WaitFrames(40);
    Actor_ShowEmote(ACTOR_LORD_MCCOY, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0);
    Actor_RunRepeatedMotion(ACTOR_LORD_MCCOY, 1);
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0,
                                   10);
    FieldEffect_SetAttached(ACTOR_CAPTAIN,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Event_WaitFrames(60);
    Actor_FaceDirection(ACTOR_CAPTAIN, FACING_SOUTHEAST + FACING_STEP, 10);
    Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTH + FACING_STEP, 10);
    FieldCamera_MoveTo(PIXELS(894), -1, PIXELS(698), 1);
    Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(894), PIXELS(798));
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 5, PIXELS(3) / 10);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 894, 726);
    Event_WaitFrames(20);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Event_WaitFrames(10);
    Actor_RunRepeatedMotion(ACTOR_LORD_MCCOY, 1);
    Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0);
    FieldCamera_MoveTo(PIXELS(894), -1, PIXELS(664), 1);
    Actor_SetPositionAndReset(ACTOR_PARTY_LEADER, 894, 684);
    actor = Actor_GetById(ACTOR_PARTY_LEADER);
    if (actor != 0) {
      Import_ActorSetPosition(ACTOR_GARET, actor->x.fixed, actor->z.fixed);
    }
    actor = Actor_GetById(ACTOR_PARTY_LEADER);
    if (actor != 0) {
      Import_ActorSetPosition(ACTOR_IVAN, actor->x.fixed, actor->z.fixed);
    }
    if (GameFlag_Get(FLAG_IN_PARTY(MIA)) != 0) {
      actor = Actor_GetById(ACTOR_PARTY_LEADER);
      if (actor != 0) {
        Import_ActorSetPosition(ACTOR_MIA, actor->x.fixed, actor->z.fixed);
      }
    }
    McCoyPalace_PrepareCompanionApproach();
    Actor_SetDestinationOffset(ACTOR_GARET, -16, 16);
    Actor_OffsetPositionAndResetMotion(ACTOR_IVAN, 16, 16);
    if (GameFlag_Get(FLAG_IN_PARTY(MIA)) != 0) {
      Actor_OffsetPositionAndResetMotion(ACTOR_MIA, 32, 16);
    }
    Actor_WaitForMoveById(ACTOR_IVAN);
    Import_ActorSetAnimation(ACTOR_GARET, ANIM_STAND);
    Import_ActorSetAnimation(ACTOR_IVAN, ANIM_STAND);
    Import_ActorSetAnimation(ACTOR_MIA, ANIM_STAND);
    Event_WaitFrames(10);
    Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 20);
    Actor_Jump(ACTOR_LORD_MCCOY, 2, 20);
    Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTHWEST + FACING_STEP, 10);
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0,
                                   10);
    Actor_FaceDirection(ACTOR_CAPTAIN, FACING_EAST + FACING_STEP, 10);
    Actor_SetAnimationAndWait(ACTOR_CAPTAIN, 3);
    Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTH + FACING_STEP, 40);
    Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTHWEST + FACING_STEP, 10);
    Actor_SetAnimationAndWait(ACTOR_LORD_MCCOY, 4);
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0,
                                   10);
    FieldEffect_SetAttached(ACTOR_CAPTAIN,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Event_WaitFrames(40);
    Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTH + FACING_STEP, 20);
    Actor_ShowEmote(ACTOR_LORD_MCCOY, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
    // Let the party accept the assignment or leave after McCoy refuses them.
    FieldMessage_ShowAtActor(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0);
    Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
    if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
      // The initial offer and reconsideration share their accept/refuse paths.
      goto accepted;
    }
  declined:
    Event_SetMessageId(MsgBilibinTooYoungForTheJob);
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0,
                                   10);
    Actor_FaceDirection(ACTOR_CAPTAIN, FACING_SOUTHEAST + FACING_STEP, 0);
    Actor_SetAnimationAndWait(ACTOR_LORD_MCCOY, 4);
    Event_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
    Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Import_ActorSetAnimation(ACTOR_GARET, ANIM_WALK);
    actor = Actor_GetById(ACTOR_PARTY_LEADER);
    if (actor != 0) {
      Actor_ResetAndSetPosition(ACTOR_GARET, actor->x.part.pixel,
                                actor->z.part.pixel);
    }
    Import_ActorSetAnimation(ACTOR_IVAN, ANIM_WALK);
    actor = Actor_GetById(ACTOR_PARTY_LEADER);
    if (actor != 0) {
      Actor_ResetAndSetPosition(ACTOR_IVAN, actor->x.part.pixel,
                                actor->z.part.pixel);
    }
    if (GameFlag_Get(FLAG_IN_PARTY(MIA)) != 0) {
      Import_ActorSetAnimation(ACTOR_MIA, ANIM_WALK);
      actor = Actor_GetById(ACTOR_PARTY_LEADER);
      if (actor != 0) {
        Actor_ResetAndSetPosition(ACTOR_MIA, actor->x.part.pixel,
                                  actor->z.part.pixel);
      }
    }
    Actor_WaitForMoveById(ACTOR_IVAN);
    Import_ActorSetPosition(ACTOR_GARET, 0, 0);
    Import_ActorSetPosition(ACTOR_IVAN, 0, 0);
    Actor_SetPosition(ACTOR_MIA, 0, 0);
    GameFlag_Set(FLAG_MCCOY_PALACE_JOB_REFUSED);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 894, 752);
    gFieldWork->transitionFrames = 16;
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    goto leave;
  }
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_ResetAndSetPositionInMode2(ACTOR_PARTY_LEADER, 894, 684);
  Event_WaitFrames(80);
  FieldCamera_SetSpeedParameters(PIXELS(3) / 5, PIXELS(3) / 40);
  FieldCamera_MoveWithinMapBounds(PIXELS(894), -1, PIXELS(664), 1);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  actor = Actor_GetById(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Import_ActorSetPosition(ACTOR_GARET, actor->x.fixed, actor->z.fixed);
  }
  actor = Actor_GetById(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Import_ActorSetPosition(ACTOR_IVAN, actor->x.fixed, actor->z.fixed);
  }
  if (GameFlag_Get(FLAG_IN_PARTY(MIA)) != 0) {
    actor = Actor_GetById(ACTOR_PARTY_LEADER);
    if (actor != 0) {
      Import_ActorSetPosition(ACTOR_MIA, actor->x.fixed, actor->z.fixed);
    }
  }
  McCoyPalace_PrepareCompanionApproach();
  Actor_SetDestinationOffset(ACTOR_GARET, -16, 16);
  Actor_OffsetPositionAndResetMotion(ACTOR_IVAN, 16, 16);
  if (GameFlag_Get(FLAG_IN_PARTY(MIA)) != 0) {
    Actor_OffsetPositionAndResetMotion(ACTOR_MIA, 32, 16);
  }
  Actor_WaitForMoveById(ACTOR_IVAN);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_STAND);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_STAND);
  Import_ActorSetAnimation(ACTOR_MIA, ANIM_STAND);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 20);
  FieldEffect_SpawnAttached(ACTOR_LORD_MCCOY, EMOTE_IN_FRONT | EMOTE_QUESTION,
                            60);
  // A returning party can reconsider the assignment without replaying the
  // captain's announcement.
  Event_SetMessageId(MsgBilibinYehveChangeHeart);
  FieldMessage_ShowAtActor(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 1) {
    // Reuse the initial refusal's dialogue and departure.
    goto declined;
  }
accepted:
  // Both ways of accepting join the same explanation and departure.
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 20);
  Import_ActorSetAnimation(ACTOR_MIA, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  FieldEffect_SpawnAttached(ACTOR_LORD_MCCOY, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
                            60);
  Event_SetMessageId(MsgBilibinHmmmWellGrant);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0);
  // Animate McCoy tossing the barricade key to the party.
  actor = Actor_GetById(ACTOR_BARRICADE_KEY);
  Actor_SetSpriteFlags(actor, 0);
  actor = Actor_GetById(ACTOR_BARRICADE_KEY);
  actor->scaleX = Q16_ONE / 2;
  actor->scaleY = Q16_ONE / 2;
  actor = Actor_GetById(ACTOR_LORD_MCCOY);
  if (actor != 0) {
    Import_ActorSetPosition(ACTOR_BARRICADE_KEY, actor->x.fixed,
                            actor->z.fixed);
  }
  WaitFrames(1);
  Actor_Jump(ACTOR_BARRICADE_KEY, 6, 0);
  Actor_SetSpeed(ACTOR_BARRICADE_KEY, PIXELS(2), PIXELS(1));
  Actor_MoveToAndWait(ACTOR_BARRICADE_KEY, 894, 668);
  Event_WaitFrames(40);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0,
                                 10);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Actor_SetAnimationAndWait(ACTOR_LORD_MCCOY, 4);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0,
                                 10);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 60);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 10);
  FieldMessage_ShowAtActor(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 1) {
    do {
      Actor_StartRepeatedMotion(ACTOR_GARET, 2);
      Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
      Event_SetMessageId(MsgBilibinSeriousDoesntBother);
      FieldMessage_ShowAtActor(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0);
    } while (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) != 1);
  }
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  // Resolve Garet\'s objections before McCoy dismisses the party.
  Event_SetMessageId(MsgBilibinWellDontLet);
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0, 10);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 10);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_WaitFrames(20);
  Actor_ShowEmote(ACTOR_LORD_MCCOY, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 20);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTHEAST + FACING_STEP, 10);
  Actor_ShowEmote(ACTOR_LORD_MCCOY, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Actor_ArmCallback(ACTOR_GARET, FACING_EAST, 20);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0, 10);
  Actor_FaceDirection(ACTOR_MIA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 20);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 10);
  Event_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_IVAN, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  Event_WaitFrames(40);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0, 10);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 10);
  Actor_StartRepeatedMotion(ACTOR_GARET, 1);
  FieldMessage_ShowAtActor(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  while (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) != 0) {
    Event_SetMessageId(MsgBilibinComeSaidYoud);
    FieldMessage_ShowAtActor(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0);
  }
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_MIA, FACING_WEST, 0);
  Actor_ArmCallback(ACTOR_GARET, FACING_EAST, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 10);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 10);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
  Event_SetMessageId(MsgBilibinWellSGoing);
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_IVAN, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_LORD_MCCOY, 1);
  Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTH + FACING_STEP, 10);
  Actor_SetAnimationAndWait(ACTOR_LORD_MCCOY, 4);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0,
                                 10);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 20);
  Import_ActorSetAnimation(ACTOR_MIA, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_WaitFrames(40);
  Actor_ShowEmote(ACTOR_LORD_MCCOY, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
  Actor_FaceDirection(ACTOR_CAPTAIN, FACING_EAST + FACING_STEP, 10);
  Actor_SetAttachedEffect(ACTOR_CAPTAIN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_WaitFrames(40);
  Event_ShowMessageThenWait(ACTOR_CAPTAIN, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_LORD_MCCOY, 1);
  Event_WaitFrames(40);
  Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTHWEST + FACING_STEP, 20);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0,
                                 10);
  Actor_SetAnimationAndWait(ACTOR_CAPTAIN, 3);
  Event_WaitFrames(20);
  Actor_SetAnimationAndWait(ACTOR_LORD_MCCOY, 4);
  Import_ActorSetAnimation(ACTOR_LORD_MCCOY, ANIM_SHAKE_HEAD);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0,
                                 10);
  Actor_Jump(ACTOR_BARRICADE_KEY, 6, 0);
  actor = Actor_GetById(ACTOR_LORD_MCCOY);
  if (actor != 0) {
    Actor_ResetAndSetPosition(ACTOR_BARRICADE_KEY, actor->x.part.pixel,
                              actor->z.part.pixel);
  }
  Actor_WaitForMoveById(ACTOR_BARRICADE_KEY);
  Import_ActorSetPosition(ACTOR_BARRICADE_KEY, 0, 0);
  Event_WaitFrames(20);
  Actor_SetAttachedEffect(ACTOR_MIA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEffect_SetAttached(ACTOR_IVAN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_WaitFrames(40);
  Actor_RunRepeatedMotion(ACTOR_CAPTAIN, 2);
  Event_ShowMessageThenWait(ACTOR_CAPTAIN, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_LORD_MCCOY, 3);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0,
                                 20);
  Actor_SetAnimationAndWait(ACTOR_CAPTAIN, 3);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_IVAN, 0, 10);
  Actor_FaceDirection(ACTOR_CAPTAIN, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTHEAST + FACING_STEP, 10);
  Actor_SetAnimationAndWait(ACTOR_LORD_MCCOY, 4);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0,
                                 10);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 60);
  Event_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0, 10);
  Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTH + FACING_STEP, 10);
  Import_ActorSetAnimation(ACTOR_LORD_MCCOY, ANIM_SHAKE_HEAD);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0,
                                 10);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_SHAKE_HEAD);
  Event_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_IVAN, 0, 10);
  Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTHEAST + FACING_STEP, 10);
  Actor_SetAnimationAndWait(ACTOR_LORD_MCCOY, 3);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0,
                                 10);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_SCOWL, 60);
  Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTHWEST + FACING_STEP, 10);
  Actor_SetAnimationAndWait(ACTOR_LORD_MCCOY, 3);
  Event_WaitFrames(10);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0,
                                 10);
  Actor_RunRepeatedMotion(ACTOR_CAPTAIN, 2);
  Actor_FaceDirection(ACTOR_CAPTAIN, FACING_EAST + FACING_STEP, 10);
  Actor_SetAnimationAndWait(ACTOR_CAPTAIN, 3);
  Event_WaitFrames(20);
  FieldActor_SetTargetAndCallback(ACTOR_PARTY_LEADER,
                                  ACTOR_LINK_KEEP_MOTION | ACTOR_CAPTAIN,
                                  gPalace_FacingActions);
  // Keep the party looking at the captain while he leaves the audience.
  FieldActor_SetTargetAndCallback(ACTOR_GARET,
                                  ACTOR_LINK_KEEP_MOTION | ACTOR_CAPTAIN,
                                  gPalace_FacingActions);
  FieldActor_SetTargetAndCallback(ACTOR_IVAN,
                                  ACTOR_LINK_KEEP_MOTION | ACTOR_CAPTAIN,
                                  gPalace_FacingActions);
  FieldActor_SetTargetAndCallback(
      ACTOR_MIA, ACTOR_LINK_KEEP_MOTION | ACTOR_CAPTAIN, gPalace_FacingActions);
  Actor_SetSpeed(ACTOR_CAPTAIN, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_WalkToAndWait(ACTOR_CAPTAIN, 852, 646);
  Actor_WalkToAndWait(ACTOR_CAPTAIN, 852, 666);
  Actor_WalkToAndWait(ACTOR_CAPTAIN, 864, 672);
  Actor_FaceDirection(ACTOR_CAPTAIN, FACING_EAST + FACING_STEP, 10);
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_CAPTAIN, 0, 20);
  Actor_Stop(ACTOR_PARTY_LEADER);
  Actor_Stop(ACTOR_GARET);
  Actor_Stop(ACTOR_IVAN);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  FieldActor_SetTargetAndCallback(ACTOR_CAPTAIN,
                                  ACTOR_LINK_KEEP_MOTION | ACTOR_PARTY_LEADER,
                                  gPalace_FacingActions);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_WALK);
  actor = Actor_GetById(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Actor_ResetAndSetPosition(ACTOR_GARET, actor->x.part.pixel,
                              actor->z.part.pixel);
  }
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_WALK);
  actor = Actor_GetById(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Actor_ResetAndSetPosition(ACTOR_IVAN, actor->x.part.pixel,
                              actor->z.part.pixel);
  }
  if (GameFlag_Get(FLAG_IN_PARTY(MIA)) != 0) {
    Import_ActorSetAnimation(ACTOR_MIA, ANIM_WALK);
    actor = Actor_GetById(ACTOR_PARTY_LEADER);
    if (actor != 0) {
      Actor_ResetAndSetPosition(ACTOR_MIA, actor->x.part.pixel,
                                actor->z.part.pixel);
    }
  }
  Actor_WaitForMoveById(ACTOR_IVAN);
  Import_ActorSetPosition(ACTOR_GARET, 0, 0);
  Import_ActorSetPosition(ACTOR_IVAN, 0, 0);
  Import_ActorSetPosition(ACTOR_MIA, 0, 0);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  Actor_SetPositionAndReset(ACTOR_PARTY_LEADER, 894, 752);
  gFieldWork->transitionFrames = 16;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  GameFlag_Set(FLAG_CAPTAIN_SEEING_PARTY_OUT);
leave:
  Import_EventSetExitRequest(MCCOY_PALACE_MCCOYS_ROOM_TO_ENTRANCE_ROOM);
  Event_End();
}

/// McCoy's Palace, at MCCOY_PALACE_ENTRANCE_ROOM_FROM_MCCOYS_ROOM after the
/// first audience: outside McCoy's door, his captain is sorry the party was
/// rejected, asks them not to go to Kolima and lets slip that the barricade may
/// not need a key.
static void McCoyPalace_CaptainHintsAtBarricade(void) {
  Event_Start();
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_FaceDirection(ACTOR_CAPTAIN, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 256, 660);
  Event_WaitFrames(20);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  GameFlag_Set(FLAG_MCCOYS_DOOR_OPEN);
  Event_PlaySong(SE_THUMP);
  TilesetAnim_Resume(1);
  TilesetAnim_Resume(2);
  Import_ActorSetPosition(ACTOR_CAPTAIN, PIXELS(256), PIXELS(632));
  WaitFrames(1);
  Actor_SetSpeed(ACTOR_CAPTAIN, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_WalkToAndWait(ACTOR_CAPTAIN, 256, 644);
  TilesetAnim_Pause(1);
  TilesetAnim_Pause(2);
  Event_WaitFrames(20);
  Actor_RunRepeatedMotion(ACTOR_CAPTAIN, 2);
  Event_SetMessageId(MsgBilibinNameSorryRejected);
  Event_ShowMessageThenWait(ACTOR_CAPTAIN, 0, 10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 264, 660);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_WalkToAndWait(ACTOR_CAPTAIN, 248, 660);
  Actor_FaceDirection(ACTOR_CAPTAIN, FACING_EAST + FACING_STEP, 40);
  Actor_SetAnimationAndWait(ACTOR_CAPTAIN, 4);
  Event_ShowMessageAndWait(ACTOR_CAPTAIN, 0);
  Actor_SetAnimationAndWait(ACTOR_CAPTAIN, 3);
  Event_AskYesNoAndShowResponse(ACTOR_CAPTAIN, 0);
  Actor_RunRepeatedMotion(ACTOR_CAPTAIN, 2);
  Event_ShowMessageThenWait(ACTOR_CAPTAIN, 0, 10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Actor_SetAttachedEffect(ACTOR_CAPTAIN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_WaitFrames(60);
  Actor_RunRepeatedMotion(ACTOR_CAPTAIN, 1);
  Event_ShowMessageThenWait(ACTOR_CAPTAIN, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_CAPTAIN, 3);
  Event_ShowMessage(ACTOR_CAPTAIN, 0);
  Actor_SetSpeed(ACTOR_CAPTAIN, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_CAPTAIN, 248, 772);
  Import_ActorSetPosition(ACTOR_CAPTAIN, 0, 0);
  GameFlag_Clear(FLAG_SHOW_AREA_NAME);
  GameFlag_Set(FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY);
  Event_End();
}

/// Garet, Ivan and Mia turn to talk among themselves.
static void McCoyPalace_FaceCompanionsForDiscussion(void) {
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_WEST, 0);
}

/// Garet, Ivan and Mia face McCoy again.
static void McCoyPalace_FaceCompanionsForAudience(void) {
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHWEST, 0);
}

/// McCoy's Palace, once Kolima's curse is lifted: brought in under arrest, the
/// party is thanked by McCoy and sent to choose a reward.
/// The gate guards bring the party in from the grounds
/// (MCCOY_PALACE_GROUNDS_TO_MCCOY_PALACE_EAST) to
/// MCCOY_PALACE_FROM_MCCOY_PALACE_GROUNDS_EAST, before McCoy.
static void McCoyPalace_McCoyRewardsParty(void) {
  struct Actor *viewCenter;
  struct Actor *actor;
  s32 advanceAlternateReply;

  Event_Start();
  // Reveal the approach, then walk the leader and the captain into the
  // audience.
  FieldCamera_MoveTo(-1, -1, -1, 0);
  WaitFrames(1);
  viewCenter = FieldCamera_GetActor();
  advanceAlternateReply = 0;
  viewCenter->motionFlags = advanceAlternateReply;
  FieldCamera_MoveTo(PIXELS(894), -1, PIXELS(664), 0);
  WaitFrames(1);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  MapGrid_CenterOnFollowedPoint();
  WaitFrames(1);

  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  gFieldWork->transitionFrames = 16;

  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_WaitFrames(40);
  Actor_SetPosition(ACTOR_CAPTAIN, PIXELS(888), PIXELS(798));
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(904), PIXELS(798));
  FieldCamera_SetSpeed(PIXELS(3) / 5, PIXELS(3) / 40);
  FieldCamera_MoveTo(PIXELS(894), -1, PIXELS(698), 1);
  Actor_SetSpeed(ACTOR_CAPTAIN, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_WalkTo(ACTOR_CAPTAIN, 888, 720);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 904, 736);
  Event_WaitFrames(60);
  Actor_WaitForMoveById(ACTOR_CAPTAIN);
  Import_ActorSetAnimation(ACTOR_CAPTAIN, ANIM_STAND);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Event_WaitFrames(20);
  Actor_RunRepeatedMotion(ACTOR_CAPTAIN, 2);
  // Present the returning warriors, using the reply for their earlier
  // reception.
  Event_SetMessageId(MsgBilibinWeveBroughtWarriorsMilord);
  // Adjacent variants depend on whether McCoy's earlier request was declined.

  advanceAlternateReply = 1;
  if (Event_GetFlag(FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY) == 0) {
    gFieldWork->message++;
    advanceAlternateReply = 0;
  }
  Event_ShowMessage(ACTOR_CAPTAIN, 0);
  if (advanceAlternateReply != 0) {
    gFieldWork->message++;
  }

  FieldCamera_MoveTo(PIXELS(894), -1, PIXELS(664), 1);
  Actor_SetScript(ACTOR_CAPTAIN, gPalace_PacingActions);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 894, 684);

  actor = Actor_GetById(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Actor_SetPosition(ACTOR_GARET, actor->x.fixed, actor->z.fixed);
  }
  actor = Actor_GetById(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Actor_SetPosition(ACTOR_IVAN, actor->x.fixed, actor->z.fixed);
  }
  actor = Actor_GetById(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Actor_SetPosition(ACTOR_MIA, actor->x.fixed, actor->z.fixed);
  }

  McCoyPalace_PrepareCompanionApproach();
  Actor_SetDestinationOffset(ACTOR_GARET, -16, 16);
  Actor_SetDestinationOffset(ACTOR_IVAN, 16, 16);
  Actor_SetDestinationOffset(ACTOR_MIA, 32, 16);
  Actor_WaitForMoveById(ACTOR_IVAN);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_STAND);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_STAND);
  Import_ActorSetAnimation(ACTOR_MIA, ANIM_STAND);
  Event_WaitFrames(10);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Actor_WaitForMoveById(ACTOR_MIA);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHWEST, 0);
  Actor_RefreshSelectorById(ACTOR_CAPTAIN);
  Event_WaitFrames(20);

  advanceAlternateReply = 1;
  if (Event_GetFlag(FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY) == 0) {
    gFieldWork->message++;
    advanceAlternateReply = 0;
  }
  Actor_RunRepeatedMotion(ACTOR_LORD_MCCOY, 3);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0,
                                 20);
  if (advanceAlternateReply != 0) {
    gFieldWork->message++;
  }

  advanceAlternateReply = 1;
  if (Event_GetFlag(FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY) == 0) {
    gFieldWork->message++;
    advanceAlternateReply = 0;
  }
  Actor_RunRepeatedMotion(ACTOR_LORD_MCCOY, 1);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0,
                                 10);
  if (advanceAlternateReply != 0) {
    gFieldWork->message++;
  }

  // Let the companions answer McCoy\'s thanks in turn.
  McCoyPalace_FaceCompanionsForDiscussion();
  Event_WaitFrames(20);
  if (GameFlag_Get(FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY) != 0) {
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  } else {
    Event_WaitFrames(40);
  }

  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 10);
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0, 10);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 10);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_IVAN, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHWEST, 10);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_MIA, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_LORD_MCCOY, 3);
  Event_WaitFrames(20);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0,
                                 20);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);

  if (GameFlag_Get(FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY) != 0) {
    Actor_RunRepeatedMotion(ACTOR_LORD_MCCOY, 1);
    Actor_SetAnimationAndWait(ACTOR_LORD_MCCOY, 4);
    Event_OpenMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0);
    McCoyPalace_FaceCompanionsForDiscussion();
    advanceAlternateReply = 1;
    if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) != 0) {
      gFieldWork->message++;
      advanceAlternateReply = 0;
    }
    Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTH + FACING_STEP, 0);
    McCoyPalace_FaceCompanionsForAudience();
    Event_WaitFrames(10);
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0,
                                   10);
    if (advanceAlternateReply != 0) {
      gFieldWork->message++;
    }
    Actor_ShowEmote(ACTOR_LORD_MCCOY, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  } else {
    gFieldWork->message += 4;
  }

  Event_OpenMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0);
  McCoyPalace_FaceCompanionsForDiscussion();
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_SetMessageId(MsgBilibinHumblyThank);
  } else {
    Event_SetMessageId(MsgBilibinNaeYehDinnaeNeedTae);
  }

  McCoyPalace_FaceCompanionsForAudience();
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0,
                                 20);
  Actor_RunRepeatedMotion(ACTOR_CAPTAIN, 1);
  // McCoy explains his caution and offers a choice from the treasury.
  Event_SetMessageId(MsgBilibinWasButWorriedYehMight);
  FieldEvent_ShowMessageThenWait(ACTOR_CAPTAIN, 0, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_WEST, 40);
  Actor_RunRepeatedMotion(ACTOR_LORD_MCCOY, 2);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0,
                                 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  McCoyPalace_FaceCompanionsForAudience();
  Event_WaitFrames(10);
  Actor_ShowEmote(ACTOR_LORD_MCCOY, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0,
                                 10);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_WaitFrames(20);
  Actor_SetAnimationAndWait(ACTOR_LORD_MCCOY, 3);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0,
                                 10);
  Actor_ShowEmote(ACTOR_LORD_MCCOY, EMOTE_IN_FRONT | EMOTE_SMILE, 60);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0,
                                 10);
  Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTHEAST + FACING_STEP, 10);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0,
                                 10);
  Actor_SetAnimationAndWait(ACTOR_LORD_MCCOY, 3);
  Event_OpenMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0);

  McCoyPalace_FaceCompanionsForDiscussion();
  advanceAlternateReply = 1;
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 1) {
    gFieldWork->message++;
    advanceAlternateReply = 0;
  }
  McCoyPalace_FaceCompanionsForAudience();
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0,
                                 10);
  if (advanceAlternateReply != 0) {
    gFieldWork->message++;
  }

  Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTHWEST + FACING_STEP, 10);
  Actor_RunRepeatedMotion(ACTOR_CAPTAIN, 1);
  Actor_FaceDirection(ACTOR_CAPTAIN, FACING_EAST + FACING_STEP, 20);
  Actor_SetAnimationAndWait(ACTOR_LORD_MCCOY, 3);
  Event_WaitFrames(20);
  Actor_SetAnimationAndWait(ACTOR_CAPTAIN, 3);
  FieldEvent_ShowMessageThenWait(ACTOR_CAPTAIN, 0, 10);
  Actor_FaceDirection(ACTOR_CAPTAIN, FACING_SOUTHEAST + FACING_STEP, 10);
  Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTHEAST + FACING_STEP, 20);
  Actor_RunRepeatedMotion(ACTOR_LORD_MCCOY, 1);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0,
                                 10);
  Actor_SetAnimationAndWait(ACTOR_LORD_MCCOY, 3);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0,
                                 10);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_GARET, ANIM_NOD);
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_WaitFrames(20);

  Import_ActorSetAnimation(ACTOR_GARET, ANIM_WALK);
  actor = Actor_GetById(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Actor_SetDestination(ACTOR_GARET, actor->x.part.pixel, actor->z.part.pixel);
  }
  Import_ActorSetAnimation(ACTOR_IVAN, ANIM_WALK);
  actor = Actor_GetById(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Actor_SetDestination(ACTOR_IVAN, actor->x.part.pixel, actor->z.part.pixel);
  }
  Import_ActorSetAnimation(ACTOR_MIA, ANIM_WALK);
  actor = Actor_GetById(ACTOR_PARTY_LEADER);
  if (actor != 0) {
    Actor_SetDestination(ACTOR_MIA, actor->x.part.pixel, actor->z.part.pixel);
  }

  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_SetPosition(ACTOR_GARET, 0, 0);
  Actor_SetPosition(ACTOR_IVAN, 0, 0);
  Actor_WaitForMoveById(ACTOR_MIA);
  Actor_SetPosition(ACTOR_MIA, 0, 0);
  Event_WaitFrames(20);
  Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTH + FACING_STEP, 0);
  FieldActor_SetTargetAndCallback(ACTOR_PARTY_LEADER,
                                  ACTOR_LINK_KEEP_MOTION | ACTOR_CAPTAIN,
                                  gPalace_FacingActions);
  Actor_WalkToAndWait(ACTOR_CAPTAIN, 852, 646);
  Actor_WalkToAndWait(ACTOR_CAPTAIN, 852, 666);
  Actor_WalkToAndWait(ACTOR_CAPTAIN, 864, 672);
  Actor_FaceDirection(ACTOR_CAPTAIN, FACING_EAST + FACING_STEP, 10);
  Actor_RunRepeatedMotion(ACTOR_CAPTAIN, 1);
  Event_WaitFrames(10);
  FieldEvent_ShowMessageThenWait(ACTOR_CAPTAIN, 0, 10);
  Actor_WalkToAndWait(ACTOR_CAPTAIN, 886, 708);
  Actor_WalkTo(ACTOR_CAPTAIN, 894, 764);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 894, 764);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  GameFlag_Set(FLAG_REWARD_OFFERED);
  if (Event_GetFlag(FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY) == 0) {
    GameFlag_Set(FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY);
    GameFlag_Set(FLAG_MCCOY_PALACE_ADMITTED);
  }
  Import_EventSetExitRequest(MCCOY_PALACE_ENTRANCE_ROOM_TO_TREASURE_ROOM);
  Event_End();
}

/// McCoy's Palace, at MCCOY_PALACE_TREASURE_ROOM_FROM_ENTRANCE_ROOM, the
/// treasure room: the captain shows the four chests and the soldier who keeps
/// the treasure explains that only one may be chosen.
static void McCoyPalace_PresentRewardChests(void) {
  struct Actor *actor;

  Event_Start();
  // Restore the chest-row metatile behavior patches before presenting the
  // reward.
  FieldCamera_MoveTo(-1, -1, -1, 0);
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_MoveWithinMapBounds(PIXELS(628), -1, PIXELS(748), 0);
  McCoyPalace_OpenRewardChestRow();
  Actor_GetById(ACTOR_PARTY_LEADER)->facing = 0;
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(577), PIXELS(760));
  Actor_GetById(ACTOR_CAPTAIN)->facing = 0;
  Import_ActorSetPosition(ACTOR_CAPTAIN, PIXELS(592), PIXELS(760));
  Actor_GetById(ACTOR_SOLDIER_SHOWING_THE_REWARD)->facing =
      FACING_WEST + FACING_STEP;
  Actor_SetPosition(ACTOR_SOLDIER_SHOWING_THE_REWARD, PIXELS(662), PIXELS(764));
  McCoyPalace_PositionRewardChests();
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_VIAL_CHEST), 0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_POTION_CHEST), 0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_PSY_CRYSTAL_CHEST), 0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_WATER_OF_LIFE_CHEST), 0);
  actor = Actor_GetById(ACTOR_VIAL_CHEST);
  actor->motionFlags = 0;
  Actor_GetById(ACTOR_POTION_CHEST)->motionFlags = 0;
  Actor_GetById(ACTOR_PSY_CRYSTAL_CHEST)->motionFlags = 0;
  Actor_GetById(ACTOR_WATER_OF_LIFE_CHEST)->motionFlags = 0;
  Actor_GetById(ACTOR_VIAL_CHEST)->y.fixed = -PIXELS(4);
  Actor_GetById(ACTOR_POTION_CHEST)->y.fixed = -PIXELS(4);
  Actor_GetById(ACTOR_PSY_CRYSTAL_CHEST)->y.fixed = -PIXELS(4);
  Actor_GetById(ACTOR_WATER_OF_LIFE_CHEST)->y.fixed = -PIXELS(4);
  MapGrid_CenterOnFollowedPoint();
  WaitFrames(1);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  gFieldWork->transitionFrames = 16;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_SetSpeed(ACTOR_CAPTAIN, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_WalkTo(ACTOR_CAPTAIN, 628, 764);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 612, 764);
  Import_ActorSetAnimation(ACTOR_CAPTAIN, ANIM_STAND);
  Event_WaitFrames(20);
  Actor_RunRepeatedMotion(ACTOR_CAPTAIN, 1);
  // The captain and the keeper explain that only one chest may be chosen.
  Event_SetMessageId(MsgBilibinLordMccoyOrdered);
  Event_ShowMessageThenWait(ACTOR_CAPTAIN, 0, 10);
  Actor_WalkToAndWait(ACTOR_CAPTAIN, 622, 780);
  Actor_FaceDirection(ACTOR_CAPTAIN, FACING_NORTH, 10);
  Actor_RunRepeatedMotion(ACTOR_SOLDIER_SHOWING_THE_REWARD, 2);
  Event_ShowMessageThenWait(ACTOR_SOLDIER_SHOWING_THE_REWARD, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  GameFlag_Clear(FLAG_SHOW_AREA_NAME);
  GameFlag_Set(FLAG_CHOOSING_REWARD);
  Event_End();
}

s32 gPalace_FacingActions[] = {
    SC_CALL(Scene_TurnTowardTarget),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gPalace_PacingActions[] = {
    SC_ANIMATION(2),
    SC_MOVE(PIXELS(842), 0, PIXELS(696)),
    SC_SYNC,
    SC_MOVE(PIXELS(842), 0, PIXELS(636)),
    SC_SYNC,
    SC_MOVE(PIXELS(876), 0, PIXELS(634)),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_SOUTHEAST + FACING_STEP),
    SC_WAIT(1),
    SC_END,
};

struct SceneEntrance gMcCoyPalaceEntrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 256, 0, 808, FACING_NORTH, 0, -1, -1,
     -1, -1, 0},
    {MCCOY_PALACE_FROM_MCCOY_PALACE_GROUNDS_WEST, CONDITION_ALWAYS, 256, 0, 808,
     FACING_NORTH, 0, 32, 496, 480, 856, 0},
    {MCCOY_PALACE_ENTRANCE_ROOM_FROM_SOUTHWEST_LANDING, CONDITION_ALWAYS, 120,
     0, 792, FACING_EAST, 0, 32, 496, 480, 856, 0},
    {MCCOY_PALACE_ENTRANCE_ROOM_FROM_SOUTHEAST_LANDING, CONDITION_ALWAYS, 392,
     0, 792, FACING_WEST, 0, 32, 496, 480, 856, 0},
    {MCCOY_PALACE_ENTRANCE_ROOM_FROM_LADY_MCCOYS_ROOM_WEST, CONDITION_ALWAYS,
     232, 0, 712, FACING_EAST, 0, 32, 496, 480, 856, 0},
    {MCCOY_PALACE_ENTRANCE_ROOM_FROM_LADY_MCCOYS_ROOM_EAST, CONDITION_ALWAYS,
     280, 0, 712, FACING_WEST, 0, 32, 496, 480, 856, 0},
    {MCCOY_PALACE_ENTRANCE_ROOM_FROM_TREASURE_ROOM, CONDITION_ALWAYS, 152, 0,
     632, FACING_WEST, 0, 32, 496, 480, 856, 0},
    {MCCOY_PALACE_ENTRANCE_ROOM_FROM_NORTHWEST_LANDING, CONDITION_ALWAYS, 104,
     0, 552, FACING_EAST, 0, 32, 496, 480, 856, 0},
    {MCCOY_PALACE_ENTRANCE_ROOM_FROM_NORTHEAST_LANDING, CONDITION_ALWAYS, 408,
     0, 552, FACING_WEST, 0, 32, 496, 480, 856, 0},
    {MCCOY_PALACE_ENTRANCE_ROOM_FROM_MCCOYS_ROOM, CONDITION_ALWAYS, 256, 0, 632,
     FACING_SOUTH, 0, 32, 496, 480, 856, 0},
    {MCCOY_PALACE_LADY_MCCOYS_ROOM_FROM_ENTRANCE_ROOM_WEST, CONDITION_ALWAYS,
     152, 0, 376, FACING_WEST, 0, 56, 304, 360, 464, 0},
    {MCCOY_PALACE_LADY_MCCOYS_ROOM_FROM_ENTRANCE_ROOM_EAST, CONDITION_ALWAYS,
     264, 0, 376, FACING_EAST, 0, 56, 304, 360, 464, 0},
    {MCCOY_PALACE_LADY_MCCOYS_ROOM_FROM_WEST_BEDROOM, CONDITION_ALWAYS, 104, 0,
     376, FACING_SOUTH, 0, 56, 304, 360, 464, 0},
    {MCCOY_PALACE_LADY_MCCOYS_ROOM_FROM_EAST_BEDROOM, CONDITION_ALWAYS, 312, 0,
     376, FACING_SOUTH, 0, 56, 304, 360, 464, 0},
    {MCCOY_PALACE_SOUTHWEST_LANDING_FROM_ENTRANCE_ROOM, CONDITION_ALWAYS, 696,
     0, 96, FACING_WEST, 0, 592, 0, 832, 160, 0},
    {MCCOY_PALACE_SOUTHWEST_LANDING_FROM_SOUTHWEST_TOWER, CONDITION_ALWAYS, 680,
     0, 72, FACING_WEST, 0, 592, 0, 832, 160, 0},
    {MCCOY_PALACE_SOUTHEAST_LANDING_FROM_SOUTHEAST_TOWER, CONDITION_ALWAYS, 952,
     0, 72, FACING_EAST, 0, 800, 0, 1040, 160, 0},
    {MCCOY_PALACE_SOUTHEAST_LANDING_FROM_ENTRANCE_ROOM, CONDITION_ALWAYS, 936,
     0, 96, FACING_EAST, 0, 800, 0, 1040, 160, 0},
    {MCCOY_PALACE_WEST_BEDROOM_FROM_LADY_MCCOYS_ROOM, CONDITION_ALWAYS, 88, 0,
     200, FACING_NORTH, 0, 0, 16, 240, 240, 0},
    {MCCOY_PALACE_EAST_BEDROOM_FROM_LADY_MCCOYS_ROOM, CONDITION_ALWAYS, 360, 0,
     200, FACING_NORTH, 0, 208, 16, 448, 240, 0},
    {MCCOY_PALACE_NORTHEAST_LANDING_FROM_ENTRANCE_ROOM, CONDITION_ALWAYS, 536,
     0, 112, FACING_EAST, 0, 400, 0, 640, 160, 0},
    {MCCOY_PALACE_NORTHEAST_LANDING_FROM_NORTHEAST_TOWER, CONDITION_ALWAYS, 536,
     0, 88, FACING_EAST, 0, 400, 0, 640, 160, 0},
    {MCCOY_PALACE_NORTHWEST_LANDING_FROM_ENTRANCE_ROOM, CONDITION_ALWAYS, 520,
     0, 312, FACING_WEST, 0, 400, 208, 640, 368, 0},
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
    {MCCOY_PALACE_TREASURE_ROOM_FROM_ENTRANCE_ROOM, CONDITION_ALWAYS, 600, 0,
     760, FACING_EAST, 0, 512, 560, 752, 816, 0},
    {MCCOY_PALACE_FROM_MCCOY_PALACE_GROUNDS_EAST, CONDITION_ALWAYS, 896, 0, 728,
     FACING_NORTH, 0, 776, 560, 1016, 768, 0},
    {SCENE_TABLE_END},
};

u32 gMcCoyPalaceExits[] = {
    SCENE_EXITS(McCoyPalace),
    SCENE_EXIT(MCCOY_PALACE_TO_MCCOY_PALACE_GROUNDS_WEST, McCoyPalaceGrounds,
               MCCOY_PALACE_GROUNDS_FROM_MCCOY_PALACE),
    SCENE_EXIT(MCCOY_PALACE_ENTRANCE_ROOM_TO_SOUTHWEST_LANDING, McCoyPalace,
               MCCOY_PALACE_SOUTHWEST_LANDING_FROM_ENTRANCE_ROOM),
    SCENE_EXIT(MCCOY_PALACE_ENTRANCE_ROOM_TO_SOUTHEAST_LANDING, McCoyPalace,
               MCCOY_PALACE_SOUTHEAST_LANDING_FROM_ENTRANCE_ROOM),
    SCENE_EXIT(MCCOY_PALACE_ENTRANCE_ROOM_TO_LADY_MCCOYS_ROOM_WEST, McCoyPalace,
               MCCOY_PALACE_LADY_MCCOYS_ROOM_FROM_ENTRANCE_ROOM_WEST),
    SCENE_EXIT(MCCOY_PALACE_ENTRANCE_ROOM_TO_LADY_MCCOYS_ROOM_EAST, McCoyPalace,
               MCCOY_PALACE_LADY_MCCOYS_ROOM_FROM_ENTRANCE_ROOM_EAST),
    SCENE_EXIT(MCCOY_PALACE_ENTRANCE_ROOM_TO_TREASURE_ROOM, McCoyPalace,
               MCCOY_PALACE_TREASURE_ROOM_FROM_ENTRANCE_ROOM),
    SCENE_EXIT(MCCOY_PALACE_ENTRANCE_ROOM_TO_NORTHWEST_LANDING, McCoyPalace,
               MCCOY_PALACE_NORTHWEST_LANDING_FROM_ENTRANCE_ROOM),
    SCENE_EXIT(MCCOY_PALACE_ENTRANCE_ROOM_TO_NORTHEAST_LANDING, McCoyPalace,
               MCCOY_PALACE_NORTHEAST_LANDING_FROM_ENTRANCE_ROOM),
    SCENE_EXIT(MCCOY_PALACE_ENTRANCE_ROOM_TO_MCCOYS_ROOM, McCoyPalace,
               MCCOY_PALACE_FROM_MCCOY_PALACE_GROUNDS_EAST),
    SCENE_EXIT(MCCOY_PALACE_LADY_MCCOYS_ROOM_TO_ENTRANCE_ROOM_WEST, McCoyPalace,
               MCCOY_PALACE_ENTRANCE_ROOM_FROM_LADY_MCCOYS_ROOM_WEST),
    SCENE_EXIT(MCCOY_PALACE_LADY_MCCOYS_ROOM_TO_ENTRANCE_ROOM_EAST, McCoyPalace,
               MCCOY_PALACE_ENTRANCE_ROOM_FROM_LADY_MCCOYS_ROOM_EAST),
    SCENE_EXIT(MCCOY_PALACE_LADY_MCCOYS_ROOM_TO_WEST_BEDROOM, McCoyPalace,
               MCCOY_PALACE_WEST_BEDROOM_FROM_LADY_MCCOYS_ROOM),
    SCENE_EXIT(MCCOY_PALACE_LADY_MCCOYS_ROOM_TO_EAST_BEDROOM, McCoyPalace,
               MCCOY_PALACE_EAST_BEDROOM_FROM_LADY_MCCOYS_ROOM),
    SCENE_EXIT(MCCOY_PALACE_SOUTHWEST_LANDING_TO_ENTRANCE_ROOM, McCoyPalace,
               MCCOY_PALACE_ENTRANCE_ROOM_FROM_SOUTHWEST_LANDING),
    SCENE_EXIT(MCCOY_PALACE_SOUTHWEST_LANDING_TO_SOUTHWEST_TOWER, McCoyPalace,
               MCCOY_PALACE_SOUTHWEST_TOWER_FROM_SOUTHWEST_LANDING),
    SCENE_EXIT(MCCOY_PALACE_SOUTHEAST_LANDING_TO_SOUTHEAST_TOWER, McCoyPalace,
               MCCOY_PALACE_SOUTHEAST_TOWER_FROM_SOUTHEAST_LANDING),
    SCENE_EXIT(MCCOY_PALACE_SOUTHEAST_LANDING_TO_ENTRANCE_ROOM, McCoyPalace,
               MCCOY_PALACE_ENTRANCE_ROOM_FROM_SOUTHEAST_LANDING),
    SCENE_EXIT(MCCOY_PALACE_WEST_BEDROOM_TO_LADY_MCCOYS_ROOM, McCoyPalace,
               MCCOY_PALACE_LADY_MCCOYS_ROOM_FROM_WEST_BEDROOM),
    SCENE_EXIT(MCCOY_PALACE_EAST_BEDROOM_TO_LADY_MCCOYS_ROOM, McCoyPalace,
               MCCOY_PALACE_LADY_MCCOYS_ROOM_FROM_EAST_BEDROOM),
    SCENE_EXIT(MCCOY_PALACE_NORTHEAST_LANDING_TO_ENTRANCE_ROOM, McCoyPalace,
               MCCOY_PALACE_ENTRANCE_ROOM_FROM_NORTHEAST_LANDING),
    SCENE_EXIT(MCCOY_PALACE_NORTHEAST_LANDING_TO_NORTHEAST_TOWER, McCoyPalace,
               MCCOY_PALACE_NORTHEAST_TOWER_FROM_NORTHEAST_LANDING),
    SCENE_EXIT(MCCOY_PALACE_NORTHWEST_LANDING_TO_ENTRANCE_ROOM, McCoyPalace,
               MCCOY_PALACE_ENTRANCE_ROOM_FROM_NORTHWEST_LANDING),
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
    SCENE_EXIT(MCCOY_PALACE_TREASURE_ROOM_TO_ENTRANCE_ROOM, McCoyPalace,
               MCCOY_PALACE_ENTRANCE_ROOM_FROM_TREASURE_ROOM),
    SCENE_EXIT(MCCOY_PALACE_MCCOYS_ROOM_TO_ENTRANCE_ROOM, McCoyPalace,
               MCCOY_PALACE_ENTRANCE_ROOM_FROM_MCCOYS_ROOM),
    SCENE_EXITS_END,
};

/// Unused: the getters return it for any map but SceneId_McCoyPalace, and the
/// overlay serves no other.
struct ActorTemplate gMcCoyPalaceNoActorTemplates[] = {
    {SCENE_TABLE_END},
};

// clang-format off
struct ActorTemplate gMcCoyPalaceActorTemplates[] = {
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        PIXELS(265), 0, PIXELS(702), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        PIXELS(342), 0, PIXELS(632), FACING_NORTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        PIXELS(360), 0, PIXELS(632), FACING_EAST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_WANDER,
        PIXELS(166), 0, PIXELS(728), FACING_EAST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        PIXELS(326), 0, PIXELS(378), FACING_SOUTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_WANDER,
        PIXELS(362), 0, PIXELS(142), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        PIXELS(296), 0, PIXELS(87), FACING_NORTHWEST + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        PIXELS(952), 0, PIXELS(289), FACING_EAST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        PIXELS(951), 0, PIXELS(438), FACING_SOUTH + FACING_STEP, TALK_FACING_KEEP, 0 },
    { SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(616), 0, PIXELS(728), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_MCCOY, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        PIXELS(894), 0, PIXELS(634), FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_KEY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTHWEST, TALK_FACING_KEEP, 0 },
    { ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTHEAST, TALK_FACING_KEEP, 0 },
    { ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_CHEST, CONDITION_IN_VIEW, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(616), 0, PIXELS(616), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_CHEST, CONDITION_IN_VIEW, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(632), 0, PIXELS(616), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_CHEST, CONDITION_IN_VIEW, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(648), 0, PIXELS(616), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_CHEST, CONDITION_IN_VIEW, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(664), 0, PIXELS(616), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_CHEST, CONDITION_IN_VIEW, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(616), 0, PIXELS(648), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_CHEST, CONDITION_IN_VIEW, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(648), 0, PIXELS(648), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_CHEST, CONDITION_IN_VIEW, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(664), 0, PIXELS(648), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_CHEST, CONDITION_IN_VIEW, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(616), 0, PIXELS(664), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_CHEST, CONDITION_IN_VIEW, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(648), 0, PIXELS(664), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_CHEST, CONDITION_IN_VIEW, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(664), 0, PIXELS(664), FACING_SOUTH, TALK_FACING_KEEP, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SCENE_TABLE_END },
};

/// Unused, as gMcCoyPalaceNoActorTemplates is.
struct Trigger gMcCoyPalaceNoTriggers[] = {
    { TRIGGERS_END },
};

struct Trigger gMcCoyPalaceTriggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, MCCOY_PALACE_TO_MCCOY_PALACE_GROUNDS_WEST },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, MCCOY_PALACE_ENTRANCE_ROOM_TO_SOUTHWEST_LANDING },
    { TRIGGER_EXIT, 3, CONDITION_ALWAYS, MCCOY_PALACE_ENTRANCE_ROOM_TO_SOUTHEAST_LANDING },
    { TRIGGER_EXIT, 4, CONDITION_ALWAYS, MCCOY_PALACE_ENTRANCE_ROOM_TO_LADY_MCCOYS_ROOM_WEST },
    { TRIGGER_EXIT, 5, CONDITION_ALWAYS, MCCOY_PALACE_ENTRANCE_ROOM_TO_LADY_MCCOYS_ROOM_EAST },
    { TRIGGER_EXIT, 6, CONDITION_ALWAYS, MCCOY_PALACE_ENTRANCE_ROOM_TO_TREASURE_ROOM },
    { TRIGGER_EXIT, 7, CONDITION_ALWAYS, MCCOY_PALACE_ENTRANCE_ROOM_TO_NORTHWEST_LANDING },
    { TRIGGER_EXIT, 8, CONDITION_ALWAYS, MCCOY_PALACE_ENTRANCE_ROOM_TO_NORTHEAST_LANDING },
    { STEP_TRIGGER_FACING(FACING_NORTH), 9, CONDITION_ALWAYS, EVENT(McCoyPalace_WalkThroughDoor) },
    { TRIGGER_EXIT, 10, CONDITION_ALWAYS, MCCOY_PALACE_LADY_MCCOYS_ROOM_TO_ENTRANCE_ROOM_WEST },
    { TRIGGER_EXIT, 11, CONDITION_ALWAYS, MCCOY_PALACE_LADY_MCCOYS_ROOM_TO_ENTRANCE_ROOM_EAST },
    { STEP_TRIGGER_FACING(FACING_NORTH), 12, CONDITION_ALWAYS, EVENT(McCoyPalace_WalkThroughDoor) },
    { STEP_TRIGGER_FACING(FACING_NORTH), 13, CONDITION_ALWAYS, EVENT(McCoyPalace_WalkThroughDoor) },
    { TRIGGER_EXIT, 14, CONDITION_ALWAYS, MCCOY_PALACE_SOUTHWEST_LANDING_TO_ENTRANCE_ROOM },
    { TRIGGER_EXIT, 15, CONDITION_ALWAYS, MCCOY_PALACE_SOUTHWEST_LANDING_TO_SOUTHWEST_TOWER },
    { TRIGGER_EXIT, 16, CONDITION_ALWAYS, MCCOY_PALACE_SOUTHEAST_LANDING_TO_SOUTHEAST_TOWER },
    { TRIGGER_EXIT, 17, CONDITION_ALWAYS, MCCOY_PALACE_SOUTHEAST_LANDING_TO_ENTRANCE_ROOM },
    { TRIGGER_EXIT, 18, CONDITION_ALWAYS, MCCOY_PALACE_WEST_BEDROOM_TO_LADY_MCCOYS_ROOM },
    { TRIGGER_EXIT, 19, CONDITION_ALWAYS, MCCOY_PALACE_EAST_BEDROOM_TO_LADY_MCCOYS_ROOM },
    { TRIGGER_EXIT, 20, CONDITION_ALWAYS, MCCOY_PALACE_NORTHEAST_LANDING_TO_ENTRANCE_ROOM },
    { TRIGGER_EXIT, 21, CONDITION_ALWAYS, MCCOY_PALACE_NORTHEAST_LANDING_TO_NORTHEAST_TOWER },
    { TRIGGER_EXIT, 22, CONDITION_ALWAYS, MCCOY_PALACE_NORTHWEST_LANDING_TO_ENTRANCE_ROOM },
    { TRIGGER_EXIT, 23, CONDITION_ALWAYS, MCCOY_PALACE_NORTHWEST_LANDING_TO_NORTHWEST_TOWER },
    { TRIGGER_EXIT, 24, CONDITION_ALWAYS, MCCOY_PALACE_NORTHWEST_TOWER_TO_NORTHWEST_LANDING },
    { TRIGGER_EXIT, 25, CONDITION_ALWAYS, MCCOY_PALACE_NORTHEAST_TOWER_TO_NORTHEAST_LANDING },
    { TRIGGER_EXIT, 26, CONDITION_ALWAYS, MCCOY_PALACE_SOUTHWEST_TOWER_TO_SOUTHWEST_LANDING },
    { TRIGGER_EXIT, 27, CONDITION_ALWAYS, MCCOY_PALACE_SOUTHEAST_TOWER_TO_SOUTHEAST_LANDING },
    { TRIGGER_EXIT, 28, CONDITION_ALWAYS, MCCOY_PALACE_TREASURE_ROOM_TO_ENTRANCE_ROOM },
    { TRIGGER_EXIT, 29, CONDITION_ALWAYS, MCCOY_PALACE_MCCOYS_ROOM_TO_ENTRANCE_ROOM },
    { TRIGGER_TALK, ACTOR_VILLAGER_FOLLOW_RED_CARPET, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinIfYouWantToSeeLord) },
    { TRIGGER_TALK, ACTOR_VILLAGER_FOLLOW_RED_CARPET, CONDITION_ALWAYS, MSGNO(MsgBilibinThePeopleOfBilibinWillNever) },
    { TRIGGER_TALK, ACTOR_VILLAGER_DO_NOT_OFFEND_MCCOY, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinMakeSureYouDontOffendLord) },
    { TRIGGER_TALK, ACTOR_VILLAGER_DO_NOT_OFFEND_MCCOY, CONDITION_ALWAYS, MSGNO(MsgBilibinIHeardYouReceivedASpectacular) },
    { TRIGGER_TALK, ACTOR_VILLAGER_ASKING_ABOUT_KOLIMA_FOREST, FLAG_MCCOY_PALACE_REWARD_TAKEN, EVENT(McCoyPalace_AskAboutGoingToKolima) },
    { TRIGGER_TALK, ACTOR_VILLAGER_ASKING_ABOUT_KOLIMA_FOREST, CONDITION_ALWAYS, MSGNO(MsgBilibinLadyMccoyIsQuiteUpsetThat) },
    { TRIGGER_TALK, ACTOR_YOUNG_MAN_PROTECT_OLD_TREE, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinIAlwaysBelievedThatTheOld) },
    { TRIGGER_TALK, ACTOR_YOUNG_MAN_PROTECT_OLD_TREE, CONDITION_ALWAYS, MSGNO(MsgBilibinThingsAreLookingGrimSouthOf) },
    { TRIGGER_TALK, ACTOR_VILLAGER_LADY_MCCOYS_ROOM, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinThisIsLadyMccoysRoomSo) },
    { TRIGGER_TALK, ACTOR_VILLAGER_LADY_MCCOYS_ROOM, CONDITION_ALWAYS, MSGNO(MsgBilibinMiladyWasVeryUpsetWhenShe) },
    { TRIGGER_TALK, ACTOR_VILLAGER_MILADYS_BEAUTY, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinMiladyWasRenownedFarAndWide) },
    { TRIGGER_TALK, ACTOR_VILLAGER_MILADYS_BEAUTY, CONDITION_ALWAYS, MSGNO(MsgBilibinForOnceMilordDidntLetLady) },
    { TRIGGER_TALK, ACTOR_VILLAGER_IN_MILADYS_NIGHTGOWN, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinOhSomedayIWantToSleep) },
    { TRIGGER_TALK, ACTOR_VILLAGER_IN_MILADYS_NIGHTGOWN, CONDITION_ALWAYS, EVENT(McCoyPalace_TalkToWomanInMiladysNightgown) },
    { TRIGGER_TALK, ACTOR_OLD_MAN_PRAISING_THE_WARRIORS, FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY, MSGNO(MsgBilibinYouCanSeeKolimaPrettyWell) },
    { TRIGGER_TALK, ACTOR_OLD_MAN_PRAISING_THE_WARRIORS, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinHeyIfYouCantDoThe) },
    { TRIGGER_TALK, ACTOR_OLD_MAN_PRAISING_THE_WARRIORS, CONDITION_ALWAYS, EVENT(McCoyPalace_TalkToOldManPraisingTheWarriors) },
    { TRIGGER_TALK, ACTOR_SOLDIER_WATCHING_KOLIMA, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinHmmItSeemsTheCurseOf) },
    { TRIGGER_TALK, ACTOR_SOLDIER_WATCHING_KOLIMA, CONDITION_ALWAYS, EVENT(McCoyPalace_TalkToSoldierWatchingKolima) },
    { TRIGGER_TALK, ACTOR_CAPTAIN, CONDITION_ALWAYS, MSGNO(MsgBilibinYouMaySelectASingleItem) },
    { TRIGGER_TALK, ACTOR_SOLDIER_SHOWING_THE_REWARD, CONDITION_ALWAYS, EVENT(McCoyPalace_TalkToSoldierShowingTheReward) },
    { TRIGGER_TALK, ACTOR_LORD_MCCOY, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinSorryLadsButKolimasFutureIs) },
    { TRIGGER_TALK, ACTOR_LORD_MCCOY, CONDITION_ALWAYS, MSGNO(MsgBilibinYehveTaughtMeThatICannae) },
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
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_PRAISING_THE_WARRIORS, CONDITION_ALWAYS, EVENT(McCoyPalace_ReadOldMansThoughtsOnTheWarriors) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SOLDIER_WATCHING_KOLIMA, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinKolimaForestUsedToBeSo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SOLDIER_WATCHING_KOLIMA, CONDITION_ALWAYS, MSGNO(MsgBilibinACourageousWarriorWouldntThinkTwice) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CAPTAIN, CONDITION_ALWAYS, MSGNO(MsgBilibinTheseCourageousWarriorsDeserveToReceive) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SOLDIER_SHOWING_THE_REWARD, CONDITION_ALWAYS, EVENT(McCoyPalace_ReadRewardSoldiersThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LORD_MCCOY, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinThereIsNothingMoreToTalk) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LORD_MCCOY, CONDITION_ALWAYS, MSGNO(MsgBilibinButICannaeFiggerOutHow) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), 100, CONDITION_ALWAYS, EVENT(McCoyPalace_SearchVialChest) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), 101, CONDITION_ALWAYS, EVENT(McCoyPalace_SearchPotionChest) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), 102, CONDITION_ALWAYS, EVENT(McCoyPalace_SearchPsyCrystalChest) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), 103, CONDITION_ALWAYS, EVENT(McCoyPalace_SearchWaterOfLifeChest) },
    { TRIGGER_STEP, 50, FLAG_PARTY_SEEN_OFF, EVENT(McCoyPalace_CaptainSeesPartyOff) },
    { TRIGGER_STEP, 51, FLAG_PARTY_SEEN_OFF, EVENT(McCoyPalace_CaptainAsksPartyToChoose) },
    { EXAMINE_TRIGGER(EXAMINE_BARREL), 120, FLAG_MCCOY_PALACE_BARREL_ELIXIR, FIND_ITEM(ITEM_ELIXIR) },
    { EXAMINE_TRIGGER(EXAMINE_BARREL), 121, FLAG_MCCOY_PALACE_BARREL_SMOKE_BOMB, FIND_ITEM(ITEM_SMOKE_BOMB) },
    { EXAMINE_TRIGGER(EXAMINE_WARDROBE), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBilibinTheresASilkNegligeeIsaacPut)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBilibinTheresASheetOfPaperTitled)) },
    { TRIGGERS_END },
};
// clang-format on
