/// @file McCoy's Palace in Bilibin: MapId_McCoyPalace, with the area name
/// McCoy's Palace, which its grounds share (mccoy_palace_grounds.c). The party
/// comes in from the grounds by MCCOY_PALACE_FROM_MCCOY_PALACE_GROUNDS_WEST,
/// or straight into McCoy's room by
/// MCCOY_PALACE_FROM_MCCOY_PALACE_GROUNDS_EAST, which the red carpet's door
/// (coord event 9) also reaches. Its people follow three story flags,
/// FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY, FLAG_KOLIMA_CURSE_LIFTED and
/// FLAG_MCCOY_PALACE_REWARD_TAKEN, in what they say and how they turn when
/// talked to (Map_GetObjectEventTemplates).
///
/// Arriving in McCoy's room before the curse is lifted plays
/// EventScript_AudienceWithMcCoy: McCoy asks whether the party can save Kolima.
/// A refusal sends it away (FLAG_MCCOY_PALACE_JOB_REFUSED) to come back later;
/// an acceptance ends with McCoy taking back the key to the barricades and the
/// captain seeing the party out (FLAG_CAPTAIN_SEEING_PARTY_OUT). Outside
/// McCoy's door the captain then plays EventScript_CaptainHintsAtBarricade,
/// which sets FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY.
///
/// Once the curse is lifted, the grounds' guards bring the party into McCoy's
/// room, and EventScript_McCoyRewardsParty plays until
/// FLAG_MCCOY_PALACE_PARTY_REWARDED; McCoy's lines differ as the party has met
/// him before or not. The captain takes the party to the treasure room
/// (FLAG_REWARD_OFFERED), where EventScript_PresentRewardChests shows four
/// chests: a search of one gives its Vial, Potion, Psy Crystal or Water of Life
/// (McCoyPalace_ClaimReward), and the others are then locked. The chest taken
/// from is kept only in map temp flags (FLAG_TOOK_VIAL and the others), so once
/// the party has left the palace all four search as locked. Leaving without
/// a reward (coord event 51), the captain walks the leader back; with it (coord
/// event 50) he sees the party off (FLAG_PARTY_SEEN_OFF).
///
/// Two barrels hold an Elixir and a Smoke Bomb, a wardrobe a silk negligee
/// and a bookcase the list of McCoy's gifts to his wife. McCoy speaks with his
/// portrait (SPEAKER_PORTRAIT_RIGHT | LOCALID_LORD_MCCOY).
///
/// Open questions: none.

#include "game_state.h"
#include "map_ids.h"
#include "unit/map/mccoy_palace.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/field_services.h"
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
#include "script_layer.h"
#include "field_service.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "map_tables.h"
#include "item_ids.h"
#include "objcmd.h"
#include "facing_object.h"
#include "fixed_math.h"
#include "songs.h"

/// The palace's people and the treasure room's chests, numbering
/// gMcCoyPalaceObjectEventTemplates' rows from LOCALID_FIRST_PLACED. Each note
/// gives what they say before the reward, then after it.
enum McCoyPalaceLocalId {
  /// A burly grey-haired man; points the way to McCoy. "If you want to see Lord
  /// McCoy, follow the red carpet."; later "The people of Bilibin will never
  /// forget your bravery."
  /// ★ Named from the game: "If you want to see Lord McCoy, follow the red
  /// carpet." (MsgBilibinIfYouWantToSeeLord)
  LOCALID_VILLAGER_FOLLOW_RED_CARPET = LOCALID_FIRST_PLACED,
  /// A woman in a green dress; warns the party about McCoy. "Make sure you
  /// don't offend Lord McCoy!"; later "I heard you received a spectacular
  /// reward."
  /// ★ Named from the game: "Make sure you don't offend Lord McCoy!"
  /// (MsgBilibinMakeSureYouDontOffendLord)
  LOCALID_VILLAGER_DO_NOT_OFFEND_MCCOY,
  /// A woman with fair hair.
  /// ★ Named from the game: "Oh, are you willing to go to Kolima Forest?"
  /// (MsgBilibinYouWillingGoKolimaForest)
  LOCALID_VILLAGER_ASKING_ABOUT_KOLIMA_FOREST,
  /// A young man with brown hair; speaks of the old tree in Kolima Forest;
  /// later "Things are looking grim south of Goma Range..."
  /// ★ Named from the game: "I always believed that the old tree in Kolima
  /// Forest should be protected!" (MsgBilibinIAlwaysBelievedThatTheOld)
  LOCALID_YOUNG_MAN_PROTECT_OLD_TREE,
  /// A woman with fair hair standing in Lady McCoy's room. "This is Lady
  /// McCoy's room, so don't touch anything."; later "Milady was very upset when
  /// she left the room."
  /// ★ Named from the game: "This is Lady McCoy's room, so don't touch
  /// anything." (MsgBilibinThisIsLadyMccoysRoomSo)
  LOCALID_VILLAGER_LADY_MCCOYS_ROOM,
  /// A woman with fair hair who admires Lady McCoy. "Milady was renowned far
  /// and wide for her beauty!"; later "For once, milord didn't let Lady McCoy
  /// push him around!"
  /// ★ Named from the game: "Milady was renowned far and wide for her beauty!"
  /// (MsgBilibinMiladyWasRenownedFarAndWide)
  LOCALID_VILLAGER_MILADYS_BEAUTY,
  /// A woman with fair hair; screams when spoken to ("AAAAH!!!"); Mind Read:
  /// "Oh, no! This nightgown has a hole... Milady will be furious!"
  /// ★ Named from the game: "Oh, someday, I want to sleep in a silk nightgown
  /// like this." (MsgBilibinOhSomedayIWantToSleep)
  LOCALID_VILLAGER_IN_MILADYS_NIGHTGOWN,
  /// An old man with a cane.
  /// ★ Named from the game: "You are a fine warrior." (MsgBilibinFineWarrior)
  LOCALID_OLD_MAN_PRAISING_THE_WARRIORS,
  /// A soldier in a spiked helmet.
  /// ★ Named from the game: "I was watching from here. After all that fuss,
  /// you still went to Kolima?" (MsgBilibinWasWatchingFromHereAfter)
  LOCALID_SOLDIER_WATCHING_KOLIMA,
  /// A soldier in a spiked helmet; shows the treasure room's chests, as the
  /// captain asks.
  /// ★ Named from the game: "Please show them their reward!"
  /// (MsgBilibinLordMccoyOrdered)
  LOCALID_SOLDIER_SHOWING_THE_REWARD,
  /// McCoy, the chief of Bilibin, a stout man with red hair in a pink tunic.
  /// ★ Named from the game: "Lord McCoy" (MsgDebugGlyphName)
  LOCALID_LORD_MCCOY,
  /// The captain of McCoy's soldiers, drawn as they are, in a spiked helmet.
  /// ★ Named from the game: "I dinnae want tae talk about it anymore.
  /// Captain?" (MsgBilibinWellSGoing)
  LOCALID_CAPTAIN,
  /// The key McCoy tosses to the party.
  /// ★ Named from the game: "This key'll get yeh past the barricades tae the
  /// east..." (MsgBilibinHmmmWellGrant)
  LOCALID_BARRICADE_KEY,
  /// ★ Named from the game: "You may select a single item from among these
  /// four chests." (MsgBilibinYouMaySelectASingleItem)
  LOCALID_VIAL_CHEST,
  /// ★ Named from the game: "You may select a single item from among these
  /// four chests." (MsgBilibinYouMaySelectASingleItem)
  LOCALID_POTION_CHEST,
  /// ★ Named from the game: "You may select a single item from among these
  /// four chests." (MsgBilibinYouMaySelectASingleItem)
  LOCALID_PSY_CRYSTAL_CHEST,
  /// ★ Named from the game: "You may select a single item from among these
  /// four chests." (MsgBilibinYouMaySelectASingleItem)
  LOCALID_WATER_OF_LIFE_CHEST
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
/// the palace's map, the map-section temp flags until it leaves McCoy's Palace.
enum {
  /// The captain has opened McCoy's door, so walking through it is silent.
  /// ★ Named from its area name: McCoy's Palace
  FLAG_MCCOYS_DOOR_OPEN = FLAG_MAP_TEMP_00,
  /// The party is choosing its reward among the four chests.
  /// ★ Named from the game: "You may select a single item from among these
  /// four chests." (MsgBilibinYouMaySelectASingleItem)
  FLAG_CHOOSING_REWARD = FLAG_MAP_TEMP_02,
  /// The chest the reward was taken from, which then searches as empty.
  /// ★ Named from the game: "Vial" (MsgItemName)
  FLAG_TOOK_VIAL = FLAG_MAP_TEMP_10,
  /// ★ Named from the game: "Potion" (MsgItemName)
  FLAG_TOOK_POTION = FLAG_MAP_TEMP_11,
  /// ★ Named from the game: "Psy Crystal" (MsgItemName)
  FLAG_TOOK_PSY_CRYSTAL = FLAG_MAP_TEMP_12,
  /// ★ Named from the game: "Water of Life" (MsgItemName)
  FLAG_TOOK_WATER_OF_LIFE = FLAG_MAP_TEMP_13,
  /// The old man has praised the party once; he then warns of the holy tree.
  /// ★ Named from the game: "You are a fine warrior." (MsgBilibinFineWarrior)
  FLAG_OLD_MAN_HAS_PRAISED = FLAG_MAPSEC_TEMP_02,
  /// Mind Read has heard the old man's first thought.
  /// ★ Named from the game: "Sometimes, we need children to remind us of our
  /// own responsibilities." (MsgBilibinSometimesWeNeedChildrenRemind)
  FLAG_OLD_MAN_THOUGHTS_READ = FLAG_MAPSEC_TEMP_03,
  /// McCoy has had the captain see the party out after the first audience.
  /// ★ Named from the game: "I dinnae want tae talk about it anymore.
  /// Captain?" (MsgBilibinWellSGoing)
  FLAG_CAPTAIN_SEEING_PARTY_OUT = FLAG_MAPSEC_TEMP_21,
  /// McCoy has had the captain take the party to the treasure room, until it
  /// takes a reward.
  /// ★ Named from the game: "Please show them their reward!"
  /// (MsgBilibinLordMccoyOrdered)
  FLAG_REWARD_OFFERED = FLAG_MAPSEC_TEMP_22,
  /// The captain has seen the party off with its reward.
  /// ★ Named from the game: "You are always welcome in the palace of Lord
  /// McCoy." (MsgBilibinAlwaysWelcomeInPalaceLord)
  FLAG_PARTY_SEEN_OFF = FLAG_MAPSEC_TEMP_33
};

static void McCoyPalace_ClaimReward(s32 chestLocalId, s32 itemId,
                                    s32 chestFlag);
static void EventScript_AudienceWithMcCoy(void);
static void EventScript_CaptainHintsAtBarricade(void);
static void EventScript_McCoyRewardsParty(void);
static void EventScript_PresentRewardChests(void);

// The shared facing callback precedes the map's entry points.
#include "common/object_facing.h"

const struct MapWarpDest *Map_GetWarpDests(void) {
  return gMcCoyPalaceWarpDests;
}

const struct MapWarpRegion *Map_GetWarpRegions(void) { return NULL; }

const u32 *Map_GetWarps(void) { return gMcCoyPalaceWarps; }

/// The people of McCoy's Palace. Once the reward is taken, the woman in
/// Milady's nightgown, the old man who praises the warriors, the soldier who
/// watched the party go to Kolima and the soldier who keeps the treasure turn
/// differently when talked to.
const struct ObjectEventTemplate *Map_GetObjectEventTemplates(void) {
  struct ObjectEventTemplate *templates;

  if (gGameState.mapId == (s32)&MapId_McCoyPalace) {
    templates = gMcCoyPalaceObjectEventTemplates;
    Map_PrepareObjectEvents(templates);

    if (Engine_GameFlagIsSet(FLAG_MCCOY_PALACE_REWARD_TAKEN) != 0) {
      gMcCoyPalaceObjectEventTemplates[LOCALID_VILLAGER_IN_MILADYS_NIGHTGOWN -
                                       LOCALID_FIRST_PLACED]
          .talkFacing = TALK_FACING_KEEP;
      gMcCoyPalaceObjectEventTemplates[LOCALID_OLD_MAN_PRAISING_THE_WARRIORS -
                                       LOCALID_FIRST_PLACED]
          .talkFacing = TALK_FACING_PARTY;
      gMcCoyPalaceObjectEventTemplates[LOCALID_SOLDIER_WATCHING_KOLIMA -
                                       LOCALID_FIRST_PLACED]
          .talkFacing = 3;
      gMcCoyPalaceObjectEventTemplates[LOCALID_SOLDIER_SHOWING_THE_REWARD -
                                       LOCALID_FIRST_PLACED]
          .talkFacing = TALK_FACING_PARTY_AND_BACK;
    }

    return templates;
  }
  return gMcCoyPalaceNoObjectEventTemplates;
}

/// What the party can talk to and search in the palace.
const struct MapEvent *Map_GetEvents(void) {
  if (gGameState.mapId == (s32)&MapId_McCoyPalace) {
    return gMcCoyPalaceEvents;
  }
  return gMcCoyPalaceNoEvents;
}

/// McCoy's Palace, before the reward: a woman with fair hair asks about going
/// to Kolima. "Oh, are you willing to go to Kolima Forest?"
static void EventScript_AskAboutGoingToKolima(void) {
  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgBilibinYouWillingGoKolimaForest);
  Engine_ScriptAskYesNo(LOCALID_VILLAGER_ASKING_ABOUT_KOLIMA_FOREST, 0);
  Engine_ScriptEnd();
}

/// McCoy's Palace, after the reward: the woman with Lady McCoy's silk
/// nightgown screams, then turns to the party. "AAAAH!!!"
static void EventScript_TalkToWomanInMiladysNightgown(void) {
  Engine_ScriptStart();
  ObjectEvent_ShowEmote(LOCALID_VILLAGER_IN_MILADYS_NIGHTGOWN,
                        EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_VILLAGER_IN_MILADYS_NIGHTGOWN, 2);
  Engine_ScriptWait(40);
  Engine_ScriptSetMessage(MsgBilibinAaaah);
  Script_ShowMessageAndWait(LOCALID_VILLAGER_IN_MILADYS_NIGHTGOWN, 0, 20);
  ObjectEvent_FaceObjectEvent(LOCALID_VILLAGER_IN_MILADYS_NIGHTGOWN,
                              LOCALID_PARTY_LEADER, 0);
  Engine_ScriptWait(20);
  Script_ShowMessageAndWait(LOCALID_VILLAGER_IN_MILADYS_NIGHTGOWN, 0, 10);
  ObjectEvent_FaceDirection(LOCALID_VILLAGER_IN_MILADYS_NIGHTGOWN,
                            FACING_NORTHWEST + FACING_STEP, 10);
  Engine_ScriptEnd();
}

/// McCoy's Palace, after the reward: the old man praises the party the first
/// time, then warns of the holy tree.
/// "You are a fine warrior." After, "It's very reckless for such young
/// warriors to face the holy tree..."
static void EventScript_TalkToOldManPraisingTheWarriors(void) {
  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgBilibinFineWarrior);
  if (GameFlag_IsSet(FLAG_OLD_MAN_HAS_PRAISED) != 0) {
    Engine_ScriptSetMessage(MsgBilibinItsVeryRecklessForSuch);
  }
  Script_ShowMessage(LOCALID_OLD_MAN_PRAISING_THE_WARRIORS, 0);
  GameFlag_Set(FLAG_OLD_MAN_HAS_PRAISED);
  Engine_ScriptEnd();
}

/// McCoy's Palace, after the reward: a soldier saw the party go to Kolima.
/// "I was watching from here. After all that fuss, you still went to Kolima?"
static void EventScript_TalkToSoldierWatchingKolima(void) {
  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgBilibinWasWatchingFromHereAfter);
  Engine_ScriptAskYesNo(LOCALID_SOLDIER_WATCHING_KOLIMA, 0);
  Engine_ScriptEnd();
}

/// McCoy's Palace, the treasure room: the soldier who keeps the treasure. While
/// the party chooses, only one item; after, the reward was the greatest;
/// before, Bilibin's great treasures go to whoever ends the curse.
///
/// Unused: with flag 0x84d set he would say `Pick any one of the four
/// treasures.` instead, but nothing in the game names or sets that flag.
static void EventScript_TalkToSoldierShowingTheReward(void) {
  Engine_ScriptStart();

  if (Engine_GameFlagIsSet(FLAG_CHOOSING_REWARD) != 0) {
    Engine_ScriptSetMessage(MsgBilibinMayChooseOnlyOneItem);
  } else if (Engine_GameFlagIsSet(FLAG_MCCOY_PALACE_REWARD_TAKEN) != 0) {
    Engine_ScriptSetMessage(MsgBilibinRewardReceivedWasIndeedGreatest);
  } else {
    Engine_ScriptSetMessage(MsgBilibinDoThinkTheseBilibinsGreat);
    if (Engine_GameFlagIsSet(0x84d) != 0) {
      gFieldWork->message++;
    }
  }

  Engine_ScriptShowMessage(LOCALID_SOLDIER_SHOWING_THE_REWARD, 0);
  Engine_ScriptEnd();
}

/// McCoy's Palace, after the reward: Mind Read on the old man shows two
/// thoughts. "Sometimes, we need children to remind us of our own
/// responsibilities." Then "You looked very courageous walking toward
/// Kolima."
static void EventScript_ReadOldMansThoughtsOnTheWarriors(void) {
  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgBilibinSometimesWeNeedChildrenRemind);
  if (GameFlag_IsSet(FLAG_OLD_MAN_THOUGHTS_READ) != 0) {
    Engine_ScriptSetMessage(MsgBilibinLookedVeryCourageousWalkingToward);
  }
  Script_ShowMessage(LOCALID_OLD_MAN_PRAISING_THE_WARRIORS, 0);
  GameFlag_Set(FLAG_OLD_MAN_THOUGHTS_READ);
  Engine_ScriptEnd();
}

/// McCoy's Palace, the treasure room: Mind Read on the soldier who keeps the
/// treasure, whose thoughts follow the curse and the reward.
static void EventScript_ReadRewardSoldiersThoughts(void) {
  Engine_ScriptStart();

  if (Engine_GameFlagIsSet(FLAG_CHOOSING_REWARD) != 0) {
    Engine_ScriptSetMessage(MsgBilibinNonethelessIfYourLuckSour);
  } else if (Engine_GameFlagIsSet(FLAG_KOLIMA_CURSE_LIFTED) == 0) {
    Engine_ScriptSetMessage(MsgBilibinGoodTreasureIfGetTurned);
  } else {
    Engine_ScriptSetMessage(MsgBilibinWeReallyGivingOurTreasure);
    if (Engine_GameFlagIsSet(FLAG_MCCOY_PALACE_REWARD_TAKEN) != 0) {
      Engine_ScriptSetMessage(MsgBilibinGotPrettyNiceRewardBut);
    }
  }

  Engine_ScriptShowMessage(LOCALID_SOLDIER_SHOWING_THE_REWARD, 0);
  Engine_ScriptEnd();
}

/// McCoy's Palace, coord events 9, 12 and 13: walking north through a door
/// opens it with a sound, the leader walks through and the next room loads.
/// McCoy's door (coord event 9) makes no sound once the captain has opened it.
static void EventScript_WalkThroughDoor(void) {
  struct EventWork *work;
  s32 hideDoorLayers;

  work = gFieldWork;
  hideDoorLayers = 0;
  if (work->coordEvent == 9) {
    if (Engine_GameFlagIsSet(FLAG_MCCOYS_DOOR_OPEN) == 0) {
      Engine_AudioPlayCue(SE_188);
      hideDoorLayers = 1;
    }
  } else {
    Engine_AudioPlayCue(SE_DOOR_OPEN);
    hideDoorLayers = 1;
  }
  if (hideDoorLayers != 0) {
    TilesetAnim_Resume(1);
    TilesetAnim_Resume(2);
  }
  Engine_ScriptStart();
  Engine_ScriptWait(10);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_WALK);
  if (work->coordEvent == 9)
    ObjectEvent_SetDestinationOffset(LOCALID_PARTY_LEADER, 0, -16);
  else
    ObjectEvent_CenterAndWalk(LOCALID_PARTY_LEADER, 3, -16);
  Engine_ScriptWait(16);
  Engine_ScriptRequestWarp(work->coordEvent);
  Engine_ScriptEnd();
  TilesetAnim_Pause(1);
  TilesetAnim_Pause(2);
}

/// The chest is empty. "Isaac checked the chest... but it was empty."
static void McCoyPalace_ShowChestEmpty(void) {
  Engine_ScriptStart();
  Engine_MessageShowCentered(MsgBilibinIsaacCheckedChestButWas, 1);
  Engine_ScriptEnd();
}

/// The chest is locked. "The treasure chest is locked."
static void McCoyPalace_ShowChestLocked(void) {
  Engine_ScriptStart();
  Engine_MessageShowCentered(MsgBilibinTreasureChestLocked, 1);
  Engine_ScriptEnd();
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
static void EventScript_SearchVialChest(void) {
  if (Engine_GameFlagIsSet(FLAG_MCCOY_PALACE_REWARD_TAKEN) != 0) {
    McCoyPalace_ShowUnavailableChest(FLAG_TOOK_VIAL);
  } else {
    McCoyPalace_ClaimReward(LOCALID_VIAL_CHEST, ITEM_VIAL, FLAG_TOOK_VIAL);
  }
}

/// McCoy's Palace, the treasure room: searching the second reward chest gives
/// a Potion, while no reward is taken.
static void EventScript_SearchPotionChest(void) {
  if (Engine_GameFlagIsSet(FLAG_MCCOY_PALACE_REWARD_TAKEN) != 0) {
    McCoyPalace_ShowUnavailableChest(FLAG_TOOK_POTION);
  } else {
    McCoyPalace_ClaimReward(LOCALID_POTION_CHEST, ITEM_POTION,
                            FLAG_TOOK_POTION);
  }
}

/// McCoy's Palace, the treasure room: searching the third reward chest gives
/// a Psy Crystal, while no reward is taken.
static void EventScript_SearchPsyCrystalChest(void) {
  if (Engine_GameFlagIsSet(FLAG_MCCOY_PALACE_REWARD_TAKEN) != 0) {
    McCoyPalace_ShowUnavailableChest(FLAG_TOOK_PSY_CRYSTAL);
  } else {
    McCoyPalace_ClaimReward(LOCALID_PSY_CRYSTAL_CHEST, ITEM_PSY_CRYSTAL,
                            FLAG_TOOK_PSY_CRYSTAL);
  }
}

/// McCoy's Palace, the treasure room: searching the fourth reward chest gives
/// a Water of Life, while no reward is taken.
static void EventScript_SearchWaterOfLifeChest(void) {
  if (Engine_GameFlagIsSet(FLAG_MCCOY_PALACE_REWARD_TAKEN) != 0) {
    McCoyPalace_ShowUnavailableChest(FLAG_TOOK_WATER_OF_LIFE);
  } else {
    McCoyPalace_ClaimReward(LOCALID_WATER_OF_LIFE_CHEST, ITEM_WATER_OF_LIFE,
                            FLAG_TOOK_WATER_OF_LIFE);
  }
}

/// The chest opens and gives its item; with no room for it, the chest
/// closes again and the reward is still to choose.
static void McCoyPalace_ClaimReward(s32 chestLocalId, s32 itemId,
                                    s32 chestFlag) {
  s32 emitterHandle;

  Engine_ScriptStart();

  emitterHandle =
      FieldEffect_PlayCueAndStartEmitterOnTarget(0, chestLocalId, itemId);

  if (Engine_PartyGiveItem(itemId, 0) != -1) {
    Engine_ObjectEventSetAnimation(chestLocalId, CHEST_ANIMATION_OPEN_EMPTY);
    Engine_GameFlagSet(FLAG_MCCOY_PALACE_REWARD_TAKEN);
    Engine_GameFlagSet(chestFlag);
    Engine_GameFlagClear(FLAG_REWARD_OFFERED);
    Engine_GameFlagClear(FLAG_CHOOSING_REWARD);
  } else {
    Engine_AudioPlayCue(SE_125);
    Engine_ObjectEventSetAnimation(chestLocalId, CHEST_ANIMATION_PEEK_AND_SHUT);
  }

  Engine_ObjectDispatchRelease((struct ObjectEvent *)emitterHandle);
  Engine_ScriptEnd();
}

/// McCoy's Palace, coord event 51, leaving the treasure room without a reward:
/// the captain walks the leader back. "Please take your reward before your
/// depart!"
static void EventScript_CaptainAsksPartyToChoose(void) {
  if (GameFlag_IsSet(FLAG_MCCOY_PALACE_REWARD_TAKEN) == 0) {
    if (Engine_GameFlagIsSet(FLAG_REWARD_OFFERED) != 0) {
      Engine_ScriptStart();
      ObjectEvent_ShowEmote(LOCALID_CAPTAIN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                            0);
      ObjectEvent_FaceDirection(LOCALID_CAPTAIN, FACING_SOUTHWEST + FACING_STEP,
                                10);
      Engine_ObjectEventRunRepeatedMotion(LOCALID_CAPTAIN, 2);
      Engine_ScriptWait(20);
      Engine_ScriptSetMessage(MsgBilibinPleaseTakeYourRewardBefore);
      Script_ShowMessage(LOCALID_CAPTAIN, 0);
      ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
      ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 616, 762);
      ObjectEvent_FaceDirection(LOCALID_CAPTAIN, FACING_NORTH + FACING_STEP,
                                10);
      Engine_ScriptEnd();
    }
  }
}

/// McCoy's Palace, coord event 50, leaving the treasure room with a reward: the
/// captain sees the party off. "You are always welcome in the palace of Lord
/// McCoy."
static void EventScript_CaptainSeesPartyOff(void) {
  if (GameFlag_IsSet(FLAG_MCCOY_PALACE_REWARD_TAKEN) != 0) {
    Engine_ScriptStart();
    ObjectEvent_FaceObjectEvent(LOCALID_PARTY_LEADER, LOCALID_CAPTAIN, 0);
    ObjectEvent_SetSpeed(LOCALID_CAPTAIN, PIXELS(3) / 5, PIXELS(3) / 10);
    ObjectEvent_WalkToAndWait(LOCALID_CAPTAIN, 622, 764);
    ObjectEvent_FaceDirection(LOCALID_CAPTAIN, FACING_NORTHEAST + FACING_STEP,
                              20);
    Engine_ObjectEventSetAnimationAndWait(LOCALID_CAPTAIN, 3);
    Engine_ObjectEventSetAnimationAndWait(LOCALID_SOLDIER_SHOWING_THE_REWARD,
                                          3);
    Engine_ScriptWait(20);
    ObjectEvent_FaceObjectEvent(LOCALID_CAPTAIN, LOCALID_PARTY_LEADER, 0);
    Engine_ScriptWait(20);
    Engine_ObjectEventSetAnimationAndWait(LOCALID_CAPTAIN, 3);
    Engine_ScriptSetMessage(MsgBilibinAlwaysWelcomeInPalaceLord);
    Script_ShowMessageAndWait(LOCALID_CAPTAIN, 0, 10);
    ObjectEvent_SetSpeed(LOCALID_CAPTAIN, PIXELS(4) / 5, PIXELS(2) / 5);
    ObjectEvent_WalkToAndWait(LOCALID_CAPTAIN, 570, 758);
    ObjectEvent_SetPosition(LOCALID_CAPTAIN, 0, 0);
    gFieldWork->transition =
        MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
    GameFlag_Set(FLAG_MCCOY_PALACE_PARTY_REWARDED);
    GameFlag_Set(FLAG_PARTY_SEEN_OFF);
    Engine_ScriptEnd();
  }
}

/// The four reward chests stand in a row.
static __inline__ void McCoyPalace_PositionRewardChests(void) {
  ObjectEvent_SetPosition(LOCALID_VIAL_CHEST, PIXELS(616), PIXELS(728));
  ObjectEvent_SetPosition(LOCALID_POTION_CHEST, PIXELS(632), PIXELS(728));
  ObjectEvent_SetPosition(LOCALID_PSY_CRYSTAL_CHEST, PIXELS(648), PIXELS(728));
  Engine_ObjectEventSetPosition(LOCALID_WATER_OF_LIFE_CHEST, PIXELS(664),
                                PIXELS(728));
}

/// The cells before the chest row can be walked on.
static __inline__ void McCoyPalace_OpenRewardChestRow(void) {
  MapGrid_CopyCellAttributes(38, 55, 4, 1, 38, 45);
  MapGrid_CopyCellAttributes(42, 55, 4, 1, 38, 46);
}

/// Sets McCoy's Palace up: McCoy's first audience, the captain outside McCoy's
/// door after it, the reward audience once Kolima's curse is lifted, or the
/// reward chests where the party left them.
s32 Map_Initialize(void) {
  s32 rewardTaken;
  s32 curseLifted;
  struct ObjectEvent *chest;

  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  TilesetAnim_Pause(1);
  TilesetAnim_Pause(2);
  Engine_GameFlagSet(FLAG_MCCOY_PALACE_VISITED);
  if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) != 0) {
    Engine_GameFlagClear(FLAG_MCCOYS_DOOR_OPEN);
  }
  // A first audience and a return visit have different greetings.
  if (Engine_GameFlagIsSet(FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY) == 0 &&
      Engine_GameFlagIsSet(FLAG_KOLIMA_CURSE_LIFTED) == 0) {
    if (gGameState.warpDest == MCCOY_PALACE_FROM_MCCOY_PALACE_GROUNDS_EAST) {
      EventScript_AudienceWithMcCoy();
    } else if (gGameState.warpDest ==
                   MCCOY_PALACE_ENTRANCE_ROOM_FROM_MCCOYS_ROOM &&
               Engine_GameFlagIsSet(FLAG_CAPTAIN_SEEING_PARTY_OUT) != 0) {
      EventScript_CaptainHintsAtBarricade();
    }
  } else {
    rewardTaken = Engine_GameFlagIsSet(FLAG_MCCOY_PALACE_REWARD_TAKEN);
    if (rewardTaken != 0) {
    } else {
      if (gGameState.warpDest == MCCOY_PALACE_FROM_MCCOY_PALACE_GROUNDS_EAST) {
        if (Engine_GameFlagIsSet(FLAG_MCCOY_PALACE_PARTY_REWARDED) == 0) {
          curseLifted = Engine_GameFlagIsSet(FLAG_KOLIMA_CURSE_LIFTED);
          if (curseLifted != 0) {
            EventScript_McCoyRewardsParty();
          }
        }
      } else {
        // Restore the displayed chest row after reloading the treasure room.
        if (gGameState.warpDest ==
            MCCOY_PALACE_TREASURE_ROOM_FROM_ENTRANCE_ROOM) {
          if (Engine_GameFlagIsSet(FLAG_REWARD_OFFERED) != 0) {
            if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) != 0) {
              McCoyPalace_OpenRewardChestRow();
              McCoyPalace_PositionRewardChests();
              chest = ObjectEvent_GetById(LOCALID_VIAL_CHEST);
              Engine_ObjectEventSetSpriteFlags(chest, 0);
              chest = ObjectEvent_GetById(LOCALID_POTION_CHEST);
              Engine_ObjectEventSetSpriteFlags(chest, 0);
              chest = ObjectEvent_GetById(LOCALID_PSY_CRYSTAL_CHEST);
              Engine_ObjectEventSetSpriteFlags(chest, 0);
              chest = ObjectEvent_GetById(LOCALID_WATER_OF_LIFE_CHEST);
              Engine_ObjectEventSetSpriteFlags(chest, 0);
              ObjectEvent_GetById(LOCALID_VIAL_CHEST)->motionFlags =
                  rewardTaken;
              ObjectEvent_GetById(LOCALID_POTION_CHEST)->motionFlags =
                  rewardTaken;
              ObjectEvent_GetById(LOCALID_PSY_CRYSTAL_CHEST)->motionFlags =
                  rewardTaken;
              ObjectEvent_GetById(LOCALID_WATER_OF_LIFE_CHEST)->motionFlags =
                  rewardTaken;
              chest = ObjectEvent_GetById(LOCALID_VIAL_CHEST);
              chest->y.fixed = -PIXELS(4);
              chest = ObjectEvent_GetById(LOCALID_POTION_CHEST);
              chest->y.fixed = -PIXELS(4);
              chest = ObjectEvent_GetById(LOCALID_PSY_CRYSTAL_CHEST);
              chest->y.fixed = -PIXELS(4);
              chest = ObjectEvent_GetById(LOCALID_WATER_OF_LIFE_CHEST);
              chest->y.fixed = -PIXELS(4);
            } else {
              EventScript_PresentRewardChests();
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
  ObjectEvent_SetSpeed(GARET, PIXELS(3) / 5, PIXELS(3) / 10);
  ObjectEvent_SetSpeed(IVAN, PIXELS(3) / 5, PIXELS(3) / 10);
  ObjectEvent_SetSpeed(MIA, PIXELS(1), PIXELS(1) / 2);
  Engine_ObjectEventSetAnimation(GARET, ANIM_WALK);
  Engine_ObjectEventSetAnimation(IVAN, ANIM_WALK);
  Engine_ObjectEventSetAnimation(MIA, ANIM_WALK);
}

/// McCoy's Palace, through McCoy's door before the curse is lifted, the first
/// audience: the captain announces the party and McCoy asks whether they can
/// save Kolima. Refusing, he calls them too young and they leave; coming back,
/// they may change their minds. Accepting, McCoy gives the key to the
/// barricades, Garet bristles at being called children, and McCoy takes the
/// key back and has the captain see them out. The party arrives at
/// MCCOY_PALACE_FROM_MCCOY_PALACE_GROUNDS_EAST, beyond the door.
static void EventScript_AudienceWithMcCoy(void) {
  struct ObjectEvent *objectEvent;

  // Stage the first audience, or place the leader for a return visit.
  Engine_ScriptStart();
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Engine_TaskWait(1);
  Engine_ScriptGetViewCenter()->motionFlags = 0;
  FieldCamera_MoveTo(PIXELS(894), -1, PIXELS(664), 0);
  Engine_TaskWait(1);
  Engine_ObjectEventSetPosition(LOCALID_PARTY_LEADER, 0, 0);
  if (GameFlag_IsSet(FLAG_MCCOY_PALACE_JOB_REFUSED) != 0) {
    FieldCamera_MoveTo(PIXELS(894), -1, PIXELS(698), 0);
    ObjectEvent_SetPosition(LOCALID_CAPTAIN, PIXELS(876), PIXELS(634));
    ObjectEvent_FaceDirection(LOCALID_CAPTAIN, FACING_SOUTHEAST + FACING_STEP,
                              0);
    ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, PIXELS(894), PIXELS(798));
  }
  Engine_MapGridRedraw();
  Engine_TaskWait(1);
  gFieldWork->transition = MAP_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  gFieldWork->transitionFrames = 40;
  Engine_ScriptOpenScreen();
  Engine_ScriptWaitForScreen();
  if (Engine_GameFlagIsSet(FLAG_MCCOY_PALACE_JOB_REFUSED) == 0) {
    Engine_ScriptWait(80);
    ObjectEvent_SetPosition(LOCALID_CAPTAIN, PIXELS(894), PIXELS(798));
    FieldCamera_SetSpeed(PIXELS(3) / 5, PIXELS(3) / 40);
    FieldCamera_MoveTo(PIXELS(894), -1, PIXELS(698), 1);
    ObjectEvent_SetSpeed(LOCALID_CAPTAIN, PIXELS(4) / 5, PIXELS(2) / 5);
    ObjectEvent_WalkTo(LOCALID_CAPTAIN, 894, 696);
    Engine_ScriptWait(80);
    FieldCamera_MoveTo(PIXELS(894), -1, PIXELS(664), 1);
    Engine_ObjectEventWaitForMove(LOCALID_CAPTAIN);
    ObjectEvent_WalkToAndWait(LOCALID_CAPTAIN, 842, 696);
    ObjectEvent_WalkToAndWait(LOCALID_CAPTAIN, 842, 636);
    ObjectEvent_FaceDirection(LOCALID_LORD_MCCOY,
                              FACING_SOUTHWEST + FACING_STEP, 20);
    ObjectEvent_WalkToAndWait(LOCALID_CAPTAIN, 876, 634);
    Engine_ObjectEventSetAnimationAndWait(LOCALID_CAPTAIN, 3);
    Engine_ScriptWait(20);
    Engine_ObjectEventSetAnimationAndWait(LOCALID_LORD_MCCOY, 3);
    Engine_ScriptWait(10);
    // The captain announces the warriors before they enter the audience.
    Engine_ScriptSetMessage(MsgBilibinMatter);
    Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | LOCALID_LORD_MCCOY, 0,
                              10);
    Engine_ObjectEventRunRepeatedMotion(LOCALID_CAPTAIN, 2);
    Engine_ScriptShowMessageAndWait(LOCALID_CAPTAIN, 0, 20);
    Engine_ObjectEventRunRepeatedMotion(LOCALID_LORD_MCCOY, 1);
    Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | LOCALID_LORD_MCCOY, 0,
                              10);
    Engine_ObjectEventSetAnimationAndWait(LOCALID_CAPTAIN, 3);
    Engine_ScriptWait(40);
    ObjectEvent_ShowEmote(LOCALID_LORD_MCCOY, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
                          60);
    Script_ShowMessage(SPEAKER_PORTRAIT_RIGHT | LOCALID_LORD_MCCOY, 0);
    Engine_ObjectEventRunRepeatedMotion(LOCALID_LORD_MCCOY, 1);
    Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | LOCALID_LORD_MCCOY, 0,
                              10);
    Engine_ObjectEventSetAttachedEffect(
        LOCALID_CAPTAIN, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Engine_ScriptWait(60);
    ObjectEvent_FaceDirection(LOCALID_CAPTAIN, FACING_SOUTHEAST + FACING_STEP,
                              10);
    ObjectEvent_FaceDirection(LOCALID_LORD_MCCOY, FACING_SOUTH + FACING_STEP,
                              10);
    FieldCamera_MoveTo(PIXELS(894), -1, PIXELS(698), 1);
    ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, PIXELS(894), PIXELS(798));
    ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(3) / 5, PIXELS(3) / 10);
    ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 894, 726);
    Engine_ScriptWait(20);
    Engine_ObjectEventSetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD);
    Engine_ScriptWait(10);
    Engine_ObjectEventRunRepeatedMotion(LOCALID_LORD_MCCOY, 1);
    Engine_ScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | LOCALID_LORD_MCCOY, 0);
    FieldCamera_MoveTo(PIXELS(894), -1, PIXELS(664), 1);
    Engine_ObjectEventWalkToAndWait(LOCALID_PARTY_LEADER, 894, 684);
    objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
    if (objectEvent != 0) {
      Engine_ObjectEventSetPosition(GARET, objectEvent->x.fixed,
                                    objectEvent->z.fixed);
    }
    objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
    if (objectEvent != 0) {
      Engine_ObjectEventSetPosition(IVAN, objectEvent->x.fixed,
                                    objectEvent->z.fixed);
    }
    if (Engine_GameFlagIsSet(FLAG_IN_PARTY(MIA)) != 0) {
      objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
      if (objectEvent != 0) {
        Engine_ObjectEventSetPosition(MIA, objectEvent->x.fixed,
                                      objectEvent->z.fixed);
      }
    }
    McCoyPalace_PrepareCompanionApproach();
    ObjectEvent_SetDestinationOffset(GARET, -16, 16);
    Engine_ObjectEventSetDestinationOffset(IVAN, 16, 16);
    if (Engine_GameFlagIsSet(FLAG_IN_PARTY(MIA)) != 0) {
      Engine_ObjectEventSetDestinationOffset(MIA, 32, 16);
    }
    Engine_ObjectEventWaitForMove(IVAN);
    Engine_ObjectEventSetAnimation(GARET, ANIM_STAND);
    Engine_ObjectEventSetAnimation(IVAN, ANIM_STAND);
    Engine_ObjectEventSetAnimation(MIA, ANIM_STAND);
    Engine_ScriptWait(10);
    ObjectEvent_FaceDirection(MIA, FACING_NORTH, 0);
    ObjectEvent_FaceDirection(GARET, FACING_NORTH, 0);
    ObjectEvent_FaceDirection(IVAN, FACING_NORTH, 20);
    Engine_ObjectEventJump(LOCALID_LORD_MCCOY, 2, 20);
    ObjectEvent_FaceDirection(LOCALID_LORD_MCCOY,
                              FACING_SOUTHWEST + FACING_STEP, 10);
    Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | LOCALID_LORD_MCCOY, 0,
                              10);
    ObjectEvent_FaceDirection(LOCALID_CAPTAIN, FACING_EAST + FACING_STEP, 10);
    Engine_ObjectEventSetAnimationAndWait(LOCALID_CAPTAIN, 3);
    ObjectEvent_FaceDirection(LOCALID_LORD_MCCOY, FACING_SOUTH + FACING_STEP,
                              40);
    ObjectEvent_FaceDirection(LOCALID_LORD_MCCOY,
                              FACING_SOUTHWEST + FACING_STEP, 10);
    Engine_ObjectEventSetAnimationAndWait(LOCALID_LORD_MCCOY, 4);
    Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | LOCALID_LORD_MCCOY, 0,
                              10);
    Engine_ObjectEventSetAttachedEffect(
        LOCALID_CAPTAIN, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Engine_ScriptWait(40);
    ObjectEvent_FaceDirection(LOCALID_LORD_MCCOY, FACING_SOUTH + FACING_STEP,
                              20);
    ObjectEvent_ShowEmote(LOCALID_LORD_MCCOY, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
                          40);
    // Let the party accept the assignment or leave after McCoy refuses them.
    Engine_ScriptOpenMessage(SPEAKER_PORTRAIT_RIGHT | LOCALID_LORD_MCCOY, 0);
    ObjectEvent_FaceDirection(GARET, FACING_NORTHEAST, 0);
    ObjectEvent_FaceDirection(IVAN, FACING_NORTHWEST, 0);
    if (Engine_ScriptChooseYesNo(LOCALID_PARTY_LEADER, 0) == 0) {
      // The initial offer and reconsideration share their accept/refuse paths.
      goto accepted;
    }
  declined:
    Engine_ScriptSetMessage(MsgBilibinTooYoungForTheJob);
    Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | LOCALID_LORD_MCCOY, 0,
                              10);
    ObjectEvent_FaceDirection(LOCALID_CAPTAIN, FACING_SOUTHEAST + FACING_STEP,
                              0);
    Engine_ObjectEventSetAnimationAndWait(LOCALID_LORD_MCCOY, 4);
    Engine_ScriptShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | LOCALID_LORD_MCCOY,
                                    0, 10);
    ObjectEvent_FaceDirection(GARET, FACING_NORTH, 0);
    ObjectEvent_FaceDirection(IVAN, FACING_NORTH, 0);
    Engine_ObjectEventSetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD);
    Engine_ObjectEventSetAnimation(GARET, ANIM_WALK);
    objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
    if (objectEvent != 0) {
      Engine_ObjectEventSetDestination(GARET, objectEvent->x.part.pixel,
                                       objectEvent->z.part.pixel);
    }
    Engine_ObjectEventSetAnimation(IVAN, ANIM_WALK);
    objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
    if (objectEvent != 0) {
      Engine_ObjectEventSetDestination(IVAN, objectEvent->x.part.pixel,
                                       objectEvent->z.part.pixel);
    }
    if (Engine_GameFlagIsSet(FLAG_IN_PARTY(MIA)) != 0) {
      Engine_ObjectEventSetAnimation(MIA, ANIM_WALK);
      objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
      if (objectEvent != 0) {
        Engine_ObjectEventSetDestination(MIA, objectEvent->x.part.pixel,
                                         objectEvent->z.part.pixel);
      }
    }
    Engine_ObjectEventWaitForMove(IVAN);
    Engine_ObjectEventSetPosition(GARET, 0, 0);
    Engine_ObjectEventSetPosition(IVAN, 0, 0);
    ObjectEvent_SetPosition(MIA, 0, 0);
    Engine_GameFlagSet(FLAG_MCCOY_PALACE_JOB_REFUSED);
    ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
    ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 894, 752);
    gFieldWork->transitionFrames = 16;
    Engine_ScriptCloseScreen();
    Engine_ScriptWaitForScreen();
    goto leave;
  }
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(3) / 5, PIXELS(3) / 10);
  Engine_ObjectEventWalkTo(LOCALID_PARTY_LEADER, 894, 684);
  Engine_ScriptWait(80);
  Engine_CameraSetSpeed(PIXELS(3) / 5, PIXELS(3) / 40);
  Engine_CameraMoveTo(PIXELS(894), -1, PIXELS(664), 1);
  Engine_ObjectEventWaitForMove(LOCALID_PARTY_LEADER);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_STAND);
  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (objectEvent != 0) {
    Engine_ObjectEventSetPosition(GARET, objectEvent->x.fixed,
                                  objectEvent->z.fixed);
  }
  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (objectEvent != 0) {
    Engine_ObjectEventSetPosition(IVAN, objectEvent->x.fixed,
                                  objectEvent->z.fixed);
  }
  if (Engine_GameFlagIsSet(FLAG_IN_PARTY(MIA)) != 0) {
    objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
    if (objectEvent != 0) {
      Engine_ObjectEventSetPosition(MIA, objectEvent->x.fixed,
                                    objectEvent->z.fixed);
    }
  }
  McCoyPalace_PrepareCompanionApproach();
  ObjectEvent_SetDestinationOffset(GARET, -16, 16);
  Engine_ObjectEventSetDestinationOffset(IVAN, 16, 16);
  if (Engine_GameFlagIsSet(FLAG_IN_PARTY(MIA)) != 0) {
    Engine_ObjectEventSetDestinationOffset(MIA, 32, 16);
  }
  Engine_ObjectEventWaitForMove(IVAN);
  Engine_ObjectEventSetAnimation(GARET, ANIM_STAND);
  Engine_ObjectEventSetAnimation(IVAN, ANIM_STAND);
  Engine_ObjectEventSetAnimation(MIA, ANIM_STAND);
  Engine_ScriptWait(10);
  ObjectEvent_FaceDirection(MIA, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTH, 20);
  Engine_ObjectEventShowEmote(LOCALID_LORD_MCCOY,
                              EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  // A returning party can reconsider the assignment without replaying the
  // captain's announcement.
  Engine_ScriptSetMessage(MsgBilibinYehveChangeHeart);
  Engine_ScriptOpenMessage(SPEAKER_PORTRAIT_RIGHT | LOCALID_LORD_MCCOY, 0);
  if (Engine_ScriptChooseYesNo(LOCALID_PARTY_LEADER, 0) == 1) {
    // Reuse the initial refusal's dialogue and departure.
    goto declined;
  }
accepted:
  // Both ways of accepting join the same explanation and departure.
  ObjectEvent_FaceDirection(MIA, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTH, 20);
  Engine_ObjectEventSetAnimation(MIA, ANIM_NOD);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  Engine_ObjectEventSetAnimation(GARET, ANIM_NOD);
  Engine_ObjectEventSetAnimationAndWait(IVAN, ANIM_NOD);
  Engine_ObjectEventShowEmote(LOCALID_LORD_MCCOY,
                              EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Engine_ScriptSetMessage(MsgBilibinHmmmWellGrant);
  Script_ShowMessage(SPEAKER_PORTRAIT_RIGHT | LOCALID_LORD_MCCOY, 0);
  // Animate McCoy tossing the barricade key to the party.
  objectEvent = ObjectEvent_GetById(LOCALID_BARRICADE_KEY);
  Engine_ObjectEventSetSpriteFlags(objectEvent, 0);
  objectEvent = ObjectEvent_GetById(LOCALID_BARRICADE_KEY);
  objectEvent->scaleX = Q16_ONE / 2;
  objectEvent->scaleY = Q16_ONE / 2;
  objectEvent = ObjectEvent_GetById(LOCALID_LORD_MCCOY);
  if (objectEvent != 0) {
    Engine_ObjectEventSetPosition(LOCALID_BARRICADE_KEY, objectEvent->x.fixed,
                                  objectEvent->z.fixed);
  }
  Engine_TaskWait(1);
  Engine_ObjectEventJump(LOCALID_BARRICADE_KEY, 6, 0);
  ObjectEvent_SetSpeed(LOCALID_BARRICADE_KEY, PIXELS(2), PIXELS(1));
  ObjectEvent_MoveToAndWait(LOCALID_BARRICADE_KEY, 894, 668);
  Engine_ScriptWait(40);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | LOCALID_LORD_MCCOY, 0, 10);
  ObjectEvent_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                        0);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_LORD_MCCOY, 4);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | LOCALID_LORD_MCCOY, 0, 10);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 60);
  ObjectEvent_FaceDirection(GARET, FACING_NORTHEAST, 10);
  Engine_ScriptOpenMessage(SPEAKER_WINDOW_ABOVE | GARET, 0);
  ObjectEvent_FaceDirection(MIA, FACING_NORTHWEST, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTHWEST, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHWEST, 0);
  if (Engine_ScriptChooseYesNo(LOCALID_PARTY_LEADER, 0) == 1) {
    do {
      Engine_ObjectEventStartRepeatedMotion(GARET, 2);
      Engine_ObjectEventRunRepeatedMotion(IVAN, 2);
      Engine_ScriptSetMessage(MsgBilibinSeriousDoesntBother);
      Engine_ScriptOpenMessage(SPEAKER_WINDOW_ABOVE | GARET, 0);
    } while (Engine_ScriptChooseYesNo(LOCALID_PARTY_LEADER, 0) != 1);
  }
  Engine_ObjectEventSetAnimationAndWait(GARET, ANIM_NOD);
  // Resolve Garet\'s objections before McCoy dismisses the party.
  Engine_ScriptSetMessage(MsgBilibinWellDontLet);
  Script_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | GARET, 0, 10);
  ObjectEvent_FaceDirection(MIA, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTH, 10);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD);
  Engine_ScriptWait(20);
  ObjectEvent_ShowEmote(LOCALID_LORD_MCCOY, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
                        60);
  Engine_ScriptShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | LOCALID_LORD_MCCOY,
                                  0, 20);
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  ObjectEvent_FaceDirection(LOCALID_LORD_MCCOY, FACING_SOUTHEAST + FACING_STEP,
                            10);
  ObjectEvent_ShowEmote(LOCALID_LORD_MCCOY, EMOTE_IN_FRONT | EMOTE_QUESTION,
                        60);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Engine_ObjectEventFaceDirection(GARET, FACING_EAST, 20);
  Engine_ObjectEventRunRepeatedMotion(GARET, 1);
  Script_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | GARET, 0, 10);
  ObjectEvent_FaceDirection(MIA, FACING_WEST, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHEAST, 20);
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  ObjectEvent_FaceDirection(IVAN, FACING_WEST, 10);
  Engine_ScriptShowMessageAndWait(SPEAKER_WINDOW_ABOVE | IVAN, 0, 10);
  Engine_ObjectEventRunRepeatedMotion(GARET, 1);
  Engine_ScriptWait(40);
  Engine_ObjectEventSetAnimationAndWait(GARET, ANIM_NOD);
  Script_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | GARET, 0, 10);
  ObjectEvent_FaceDirection(GARET, FACING_NORTHEAST, 10);
  Engine_ObjectEventStartRepeatedMotion(GARET, 1);
  Engine_ScriptOpenMessage(SPEAKER_WINDOW_ABOVE | GARET, 0);
  ObjectEvent_FaceDirection(MIA, FACING_NORTHWEST, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTHWEST, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHWEST, 0);
  while (Engine_ScriptChooseYesNo(LOCALID_PARTY_LEADER, 0) != 0) {
    Engine_ScriptSetMessage(MsgBilibinComeSaidYoud);
    Engine_ScriptOpenMessage(SPEAKER_WINDOW_ABOVE | GARET, 0);
  }
  Engine_ScriptWait(10);
  ObjectEvent_FaceDirection(MIA, FACING_WEST, 0);
  Engine_ObjectEventFaceDirection(GARET, FACING_EAST, 10);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHEAST, 10);
  Engine_ObjectEventSetAnimationAndWait(GARET, ANIM_NOD);
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  ObjectEvent_FaceDirection(IVAN, FACING_WEST, 10);
  Engine_ObjectEventSetAnimationAndWait(IVAN, ANIM_SHAKE_HEAD);
  Engine_ScriptSetMessage(MsgBilibinWellSGoing);
  Script_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | IVAN, 0, 20);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_LORD_MCCOY, 1);
  ObjectEvent_FaceDirection(LOCALID_LORD_MCCOY, FACING_SOUTH + FACING_STEP, 10);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_LORD_MCCOY, 4);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | LOCALID_LORD_MCCOY, 0, 10);
  ObjectEvent_FaceDirection(MIA, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTH, 20);
  Engine_ObjectEventSetAnimation(MIA, ANIM_NOD);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  Engine_ObjectEventSetAnimation(GARET, ANIM_NOD);
  Engine_ObjectEventSetAnimationAndWait(IVAN, ANIM_NOD);
  Engine_ScriptWait(40);
  ObjectEvent_ShowEmote(LOCALID_LORD_MCCOY, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
                        80);
  ObjectEvent_FaceDirection(LOCALID_CAPTAIN, FACING_EAST + FACING_STEP, 10);
  ObjectEvent_SetAttachedEffect(LOCALID_CAPTAIN, ATTACHED_EFFECT_IN_FRONT |
                                                     ATTACHED_EFFECT_ONCE);
  Engine_ScriptWait(40);
  Engine_ScriptShowMessageAndWait(LOCALID_CAPTAIN, 0, 10);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_LORD_MCCOY, 1);
  Engine_ScriptWait(40);
  ObjectEvent_FaceDirection(LOCALID_LORD_MCCOY, FACING_SOUTHWEST + FACING_STEP,
                            20);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | LOCALID_LORD_MCCOY, 0, 10);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_CAPTAIN, 3);
  Engine_ScriptWait(20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_LORD_MCCOY, 4);
  Engine_ObjectEventSetAnimation(LOCALID_LORD_MCCOY, ANIM_SHAKE_HEAD);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | LOCALID_LORD_MCCOY, 0, 10);
  Engine_ObjectEventJump(LOCALID_BARRICADE_KEY, 6, 0);
  objectEvent = ObjectEvent_GetById(LOCALID_LORD_MCCOY);
  if (objectEvent != 0) {
    Engine_ObjectEventSetDestination(LOCALID_BARRICADE_KEY,
                                     objectEvent->x.part.pixel,
                                     objectEvent->z.part.pixel);
  }
  Engine_ObjectEventWaitForMove(LOCALID_BARRICADE_KEY);
  Engine_ObjectEventSetPosition(LOCALID_BARRICADE_KEY, 0, 0);
  Engine_ScriptWait(20);
  ObjectEvent_SetAttachedEffect(MIA, ATTACHED_EFFECT_IN_FRONT |
                                         ATTACHED_EFFECT_ONCE);
  ObjectEvent_SetAttachedEffect(LOCALID_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT |
                                                          ATTACHED_EFFECT_ONCE);
  ObjectEvent_SetAttachedEffect(GARET, ATTACHED_EFFECT_IN_FRONT |
                                           ATTACHED_EFFECT_ONCE);
  Engine_ObjectEventSetAttachedEffect(IVAN, ATTACHED_EFFECT_IN_FRONT |
                                                ATTACHED_EFFECT_ONCE);
  Engine_ScriptWait(40);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_CAPTAIN, 2);
  Engine_ScriptShowMessageAndWait(LOCALID_CAPTAIN, 0, 10);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_LORD_MCCOY, 3);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | LOCALID_LORD_MCCOY, 0, 20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_CAPTAIN, 3);
  Engine_ObjectEventRunRepeatedMotion(IVAN, 2);
  Engine_ScriptWait(20);
  Script_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | IVAN, 0, 10);
  ObjectEvent_FaceDirection(LOCALID_CAPTAIN, FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_FaceDirection(LOCALID_LORD_MCCOY, FACING_SOUTHEAST + FACING_STEP,
                            10);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_LORD_MCCOY, 4);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | LOCALID_LORD_MCCOY, 0, 10);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 60);
  Engine_ScriptShowMessageAndWait(SPEAKER_WINDOW_ABOVE | GARET, 0, 10);
  ObjectEvent_FaceDirection(LOCALID_LORD_MCCOY, FACING_SOUTH + FACING_STEP, 10);
  Engine_ObjectEventSetAnimation(LOCALID_LORD_MCCOY, ANIM_SHAKE_HEAD);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | LOCALID_LORD_MCCOY, 0, 10);
  Engine_ObjectEventSetAnimation(IVAN, ANIM_SHAKE_HEAD);
  Engine_ScriptShowMessageAndWait(SPEAKER_WINDOW_ABOVE | IVAN, 0, 10);
  ObjectEvent_FaceDirection(LOCALID_LORD_MCCOY, FACING_SOUTHEAST + FACING_STEP,
                            10);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_LORD_MCCOY, 3);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | LOCALID_LORD_MCCOY, 0, 10);
  ObjectEvent_ShowEmote(MIA, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_SCOWL, 60);
  ObjectEvent_FaceDirection(LOCALID_LORD_MCCOY, FACING_SOUTHWEST + FACING_STEP,
                            10);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_LORD_MCCOY, 3);
  Engine_ScriptWait(10);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | LOCALID_LORD_MCCOY, 0, 10);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_CAPTAIN, 2);
  ObjectEvent_FaceDirection(LOCALID_CAPTAIN, FACING_EAST + FACING_STEP, 10);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_CAPTAIN, 3);
  Engine_ScriptWait(20);
  Object_SetTargetAndCallback(LOCALID_PARTY_LEADER,
                              OBJ_EVENT_LINK_KEEP_MOTION | LOCALID_CAPTAIN,
                              gPalace_FacingActions);
  // Keep the party looking at the captain while he leaves the audience.
  Object_SetTargetAndCallback(GARET,
                              OBJ_EVENT_LINK_KEEP_MOTION | LOCALID_CAPTAIN,
                              gPalace_FacingActions);
  Object_SetTargetAndCallback(IVAN,
                              OBJ_EVENT_LINK_KEEP_MOTION | LOCALID_CAPTAIN,
                              gPalace_FacingActions);
  Object_SetTargetAndCallback(MIA, OBJ_EVENT_LINK_KEEP_MOTION | LOCALID_CAPTAIN,
                              gPalace_FacingActions);
  ObjectEvent_SetSpeed(LOCALID_CAPTAIN, PIXELS(3) / 5, PIXELS(3) / 10);
  ObjectEvent_WalkToAndWait(LOCALID_CAPTAIN, 852, 646);
  ObjectEvent_WalkToAndWait(LOCALID_CAPTAIN, 852, 666);
  ObjectEvent_WalkToAndWait(LOCALID_CAPTAIN, 864, 672);
  ObjectEvent_FaceDirection(LOCALID_CAPTAIN, FACING_EAST + FACING_STEP, 10);
  Script_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | LOCALID_CAPTAIN, 0, 20);
  Engine_ObjectEventStop(LOCALID_PARTY_LEADER);
  Engine_ObjectEventStop(GARET);
  Engine_ObjectEventStop(IVAN);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
                        0);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  ObjectEvent_ShowEmote(IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  Engine_ObjectEventSetAnimation(GARET, ANIM_NOD);
  Engine_ObjectEventSetAnimationAndWait(IVAN, ANIM_NOD);
  Object_SetTargetAndCallback(LOCALID_CAPTAIN,
                              OBJ_EVENT_LINK_KEEP_MOTION | LOCALID_PARTY_LEADER,
                              gPalace_FacingActions);
  Engine_ObjectEventSetAnimation(GARET, ANIM_WALK);
  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (objectEvent != 0) {
    Engine_ObjectEventSetDestination(GARET, objectEvent->x.part.pixel,
                                     objectEvent->z.part.pixel);
  }
  Engine_ObjectEventSetAnimation(IVAN, ANIM_WALK);
  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (objectEvent != 0) {
    Engine_ObjectEventSetDestination(IVAN, objectEvent->x.part.pixel,
                                     objectEvent->z.part.pixel);
  }
  if (Engine_GameFlagIsSet(FLAG_IN_PARTY(MIA)) != 0) {
    Engine_ObjectEventSetAnimation(MIA, ANIM_WALK);
    objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
    if (objectEvent != 0) {
      Engine_ObjectEventSetDestination(MIA, objectEvent->x.part.pixel,
                                       objectEvent->z.part.pixel);
    }
  }
  Engine_ObjectEventWaitForMove(IVAN);
  Engine_ObjectEventSetPosition(GARET, 0, 0);
  Engine_ObjectEventSetPosition(IVAN, 0, 0);
  Engine_ObjectEventSetPosition(MIA, 0, 0);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  Engine_ObjectEventWalkToAndWait(LOCALID_PARTY_LEADER, 894, 752);
  gFieldWork->transitionFrames = 16;
  Engine_ScriptCloseScreen();
  Engine_ScriptWaitForScreen();
  Engine_GameFlagSet(FLAG_CAPTAIN_SEEING_PARTY_OUT);
leave:
  Engine_ScriptRequestWarp(MCCOY_PALACE_MCCOYS_ROOM_TO_ENTRANCE_ROOM);
  Engine_ScriptEnd();
}

/// McCoy's Palace, at MCCOY_PALACE_ENTRANCE_ROOM_FROM_MCCOYS_ROOM after the
/// first audience: outside McCoy's door, his captain is sorry the party was
/// rejected, asks them not to go to Kolima and lets slip that the barricade may
/// not need a key.
static void EventScript_CaptainHintsAtBarricade(void) {
  Engine_ScriptStart();
  Engine_ScriptOpenScreen();
  Engine_ScriptWaitForScreen();
  ObjectEvent_FaceDirection(LOCALID_CAPTAIN, FACING_SOUTHEAST + FACING_STEP, 0);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(3) / 5, PIXELS(3) / 10);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 256, 660);
  Engine_ScriptWait(20);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Engine_GameFlagSet(FLAG_MCCOYS_DOOR_OPEN);
  Script_PlaySong(SE_188);
  TilesetAnim_Resume(1);
  TilesetAnim_Resume(2);
  Engine_ObjectEventSetPosition(LOCALID_CAPTAIN, PIXELS(256), PIXELS(632));
  Engine_TaskWait(1);
  ObjectEvent_SetSpeed(LOCALID_CAPTAIN, PIXELS(3) / 5, PIXELS(3) / 10);
  ObjectEvent_WalkToAndWait(LOCALID_CAPTAIN, 256, 644);
  TilesetAnim_Pause(1);
  TilesetAnim_Pause(2);
  Engine_ScriptWait(20);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_CAPTAIN, 2);
  Engine_ScriptSetMessage(MsgBilibinNameSorryRejected);
  Engine_ScriptShowMessageAndWait(LOCALID_CAPTAIN, 0, 10);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER,
                        EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 264, 660);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_WEST, 0);
  ObjectEvent_WalkToAndWait(LOCALID_CAPTAIN, 248, 660);
  ObjectEvent_FaceDirection(LOCALID_CAPTAIN, FACING_EAST + FACING_STEP, 40);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_CAPTAIN, 4);
  Engine_ScriptShowMessage(LOCALID_CAPTAIN, 0);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_CAPTAIN, 3);
  Engine_ScriptAskYesNo(LOCALID_CAPTAIN, 0);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_CAPTAIN, 2);
  Engine_ScriptShowMessageAndWait(LOCALID_CAPTAIN, 0, 10);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION,
                        60);
  ObjectEvent_SetAttachedEffect(LOCALID_CAPTAIN, ATTACHED_EFFECT_IN_FRONT |
                                                     ATTACHED_EFFECT_ONCE);
  Engine_ScriptWait(60);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_CAPTAIN, 1);
  Engine_ScriptShowMessageAndWait(LOCALID_CAPTAIN, 0, 10);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_CAPTAIN, 3);
  Script_ShowMessage(LOCALID_CAPTAIN, 0);
  ObjectEvent_SetSpeed(LOCALID_CAPTAIN, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_WalkToAndWait(LOCALID_CAPTAIN, 248, 772);
  Engine_ObjectEventSetPosition(LOCALID_CAPTAIN, 0, 0);
  Engine_GameFlagClear(FLAG_SHOW_LOCATION_NAME);
  Engine_GameFlagSet(FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY);
  Engine_ScriptEnd();
}

/// Garet, Ivan and Mia turn to talk among themselves.
static void McCoyPalace_FaceCompanionsForDiscussion(void) {
  ObjectEvent_FaceDirection(GARET, FACING_NORTHEAST, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTHWEST, 0);
  ObjectEvent_FaceDirection(MIA, FACING_WEST, 0);
}

/// Garet, Ivan and Mia face McCoy again.
static void McCoyPalace_FaceCompanionsForAudience(void) {
  ObjectEvent_FaceDirection(GARET, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(MIA, FACING_NORTHWEST, 0);
}

/// McCoy's Palace, once Kolima's curse is lifted: brought in under arrest, the
/// party is thanked by McCoy and sent to choose a reward.
/// The gate guards bring the party in from the grounds
/// (MCCOY_PALACE_GROUNDS_TO_MCCOY_PALACE_EAST) to
/// MCCOY_PALACE_FROM_MCCOY_PALACE_GROUNDS_EAST, before McCoy.
static void EventScript_McCoyRewardsParty(void) {
  struct ObjectEvent *viewCenter;
  struct ObjectEvent *objectEvent;
  s32 advanceAlternateReply;

  Engine_ScriptStart();
  // Reveal the approach, then walk the leader and the captain into the
  // audience.
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Engine_TaskWait(1);
  viewCenter = Engine_ScriptGetViewCenter();
  advanceAlternateReply = 0;
  viewCenter->motionFlags = advanceAlternateReply;
  FieldCamera_MoveTo(PIXELS(894), -1, PIXELS(664), 0);
  Engine_TaskWait(1);
  ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, 0, 0);
  Engine_MapGridRedraw();
  Engine_TaskWait(1);

  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  gFieldWork->transitionFrames = 16;

  Engine_ScriptOpenScreen();
  Engine_ScriptWaitForScreen();
  Engine_ScriptWait(40);
  ObjectEvent_SetPosition(LOCALID_CAPTAIN, PIXELS(888), PIXELS(798));
  ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, PIXELS(904), PIXELS(798));
  FieldCamera_SetSpeed(PIXELS(3) / 5, PIXELS(3) / 40);
  FieldCamera_MoveTo(PIXELS(894), -1, PIXELS(698), 1);
  ObjectEvent_SetSpeed(LOCALID_CAPTAIN, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(3) / 5, PIXELS(3) / 10);
  ObjectEvent_WalkTo(LOCALID_CAPTAIN, 888, 720);
  ObjectEvent_WalkTo(LOCALID_PARTY_LEADER, 904, 736);
  Engine_ScriptWait(60);
  Engine_ObjectEventWaitForMove(LOCALID_CAPTAIN);
  Engine_ObjectEventSetAnimation(LOCALID_CAPTAIN, ANIM_STAND);
  Engine_ObjectEventWaitForMove(LOCALID_PARTY_LEADER);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_STAND);
  Engine_ScriptWait(20);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_CAPTAIN, 2);
  // Present the returning warriors, using the reply for their earlier
  // reception.
  Engine_ScriptSetMessage(MsgBilibinWeveBroughtWarriorsMilord);
  // Adjacent variants depend on whether McCoy's earlier request was declined.

  advanceAlternateReply = 1;
  if (GameFlag_IsSet(FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY) == 0) {
    gFieldWork->message++;
    advanceAlternateReply = 0;
  }
  Script_ShowMessage(LOCALID_CAPTAIN, 0);
  if (advanceAlternateReply != 0) {
    gFieldWork->message++;
  }

  FieldCamera_MoveTo(PIXELS(894), -1, PIXELS(664), 1);
  ObjectEvent_SetActionScript(LOCALID_CAPTAIN, gPalace_PacingActions);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 894, 684);

  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (objectEvent != 0) {
    ObjectEvent_SetPosition(GARET, objectEvent->x.fixed, objectEvent->z.fixed);
  }
  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (objectEvent != 0) {
    ObjectEvent_SetPosition(IVAN, objectEvent->x.fixed, objectEvent->z.fixed);
  }
  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (objectEvent != 0) {
    ObjectEvent_SetPosition(MIA, objectEvent->x.fixed, objectEvent->z.fixed);
  }

  McCoyPalace_PrepareCompanionApproach();
  ObjectEvent_SetDestinationOffset(GARET, -16, 16);
  ObjectEvent_SetDestinationOffset(IVAN, 16, 16);
  ObjectEvent_SetDestinationOffset(MIA, 32, 16);
  Engine_ObjectEventWaitForMove(IVAN);
  Engine_ObjectEventSetAnimation(GARET, ANIM_STAND);
  Engine_ObjectEventSetAnimation(IVAN, ANIM_STAND);
  Engine_ObjectEventSetAnimation(MIA, ANIM_STAND);
  Engine_ScriptWait(10);
  ObjectEvent_FaceDirection(GARET, FACING_NORTH, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTH, 0);
  Engine_ObjectEventWaitForMove(MIA);
  ObjectEvent_FaceDirection(MIA, FACING_NORTHWEST, 0);
  ObjectEvent_RefreshSelectorById(LOCALID_CAPTAIN);
  Engine_ScriptWait(20);

  advanceAlternateReply = 1;
  if (GameFlag_IsSet(FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY) == 0) {
    gFieldWork->message++;
    advanceAlternateReply = 0;
  }
  Engine_ObjectEventRunRepeatedMotion(LOCALID_LORD_MCCOY, 3);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | LOCALID_LORD_MCCOY, 0, 20);
  if (advanceAlternateReply != 0) {
    gFieldWork->message++;
  }

  advanceAlternateReply = 1;
  if (GameFlag_IsSet(FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY) == 0) {
    gFieldWork->message++;
    advanceAlternateReply = 0;
  }
  Engine_ObjectEventRunRepeatedMotion(LOCALID_LORD_MCCOY, 1);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | LOCALID_LORD_MCCOY, 0, 10);
  if (advanceAlternateReply != 0) {
    gFieldWork->message++;
  }

  // Let the companions answer McCoy\'s thanks in turn.
  McCoyPalace_FaceCompanionsForDiscussion();
  Engine_ScriptWait(20);
  if (GameFlag_IsSet(FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY) != 0) {
    ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE,
                          60);
    ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  } else {
    Engine_ScriptWait(40);
  }

  ObjectEvent_FaceDirection(GARET, FACING_NORTH, 10);
  Script_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | GARET, 0, 10);
  ObjectEvent_FaceDirection(IVAN, FACING_NORTH, 10);
  Engine_ObjectEventSetAnimationAndWait(IVAN, ANIM_NOD);
  Script_ShowMessage(SPEAKER_WINDOW_ABOVE | IVAN, 0);
  ObjectEvent_FaceDirection(MIA, FACING_NORTHWEST, 10);
  Engine_ObjectEventSetAnimationAndWait(MIA, ANIM_NOD);
  Script_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | MIA, 0, 20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_LORD_MCCOY, 3);
  Engine_ScriptWait(20);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | LOCALID_LORD_MCCOY, 0, 20);
  ObjectEvent_ShowEmote(GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);

  if (GameFlag_IsSet(FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY) != 0) {
    Engine_ObjectEventRunRepeatedMotion(LOCALID_LORD_MCCOY, 1);
    Engine_ObjectEventSetAnimationAndWait(LOCALID_LORD_MCCOY, 4);
    Script_OpenMessage(SPEAKER_PORTRAIT_RIGHT | LOCALID_LORD_MCCOY, 0);
    McCoyPalace_FaceCompanionsForDiscussion();
    advanceAlternateReply = 1;
    if (Engine_ScriptChooseYesNo(LOCALID_PARTY_LEADER, 0) != 0) {
      gFieldWork->message++;
      advanceAlternateReply = 0;
    }
    ObjectEvent_FaceDirection(LOCALID_LORD_MCCOY, FACING_SOUTH + FACING_STEP,
                              0);
    McCoyPalace_FaceCompanionsForAudience();
    Engine_ScriptWait(10);
    Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | LOCALID_LORD_MCCOY, 0,
                              10);
    if (advanceAlternateReply != 0) {
      gFieldWork->message++;
    }
    ObjectEvent_ShowEmote(LOCALID_LORD_MCCOY, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  } else {
    gFieldWork->message += 4;
  }

  Script_OpenMessage(SPEAKER_PORTRAIT_RIGHT | LOCALID_LORD_MCCOY, 0);
  McCoyPalace_FaceCompanionsForDiscussion();
  if (Engine_ScriptChooseYesNo(LOCALID_PARTY_LEADER, 0) == 0) {
    Engine_ScriptSetMessage(MsgBilibinHumblyThank);
  } else {
    Engine_ScriptSetMessage(MsgBilibinNaeYehDinnaeNeedTae);
  }

  McCoyPalace_FaceCompanionsForAudience();
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | LOCALID_LORD_MCCOY, 0, 20);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_CAPTAIN, 1);
  // McCoy explains his caution and offers a choice from the treasury.
  Engine_ScriptSetMessage(MsgBilibinWasButWorriedYehMight);
  Script_ShowMessageAndWait(LOCALID_CAPTAIN, 0, 10);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTHWEST, 0);
  ObjectEvent_FaceDirection(GARET, FACING_NORTHEAST, 0);
  ObjectEvent_FaceDirection(IVAN, FACING_EAST, 0);
  ObjectEvent_FaceDirection(MIA, FACING_WEST, 40);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_LORD_MCCOY, 2);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | LOCALID_LORD_MCCOY, 0, 10);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
  McCoyPalace_FaceCompanionsForAudience();
  Engine_ScriptWait(10);
  ObjectEvent_ShowEmote(LOCALID_LORD_MCCOY, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
                        60);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | LOCALID_LORD_MCCOY, 0, 10);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  Engine_ObjectEventSetAnimation(GARET, ANIM_NOD);
  Engine_ObjectEventSetAnimation(IVAN, ANIM_NOD);
  Engine_ObjectEventSetAnimationAndWait(MIA, ANIM_NOD);
  Engine_ScriptWait(20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_LORD_MCCOY, 3);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | LOCALID_LORD_MCCOY, 0, 10);
  ObjectEvent_ShowEmote(LOCALID_LORD_MCCOY, EMOTE_IN_FRONT | EMOTE_SMILE, 60);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | LOCALID_LORD_MCCOY, 0, 10);
  ObjectEvent_FaceDirection(LOCALID_LORD_MCCOY, FACING_SOUTHEAST + FACING_STEP,
                            10);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | LOCALID_LORD_MCCOY, 0, 10);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_LORD_MCCOY, 3);
  Script_OpenMessage(SPEAKER_PORTRAIT_RIGHT | LOCALID_LORD_MCCOY, 0);

  McCoyPalace_FaceCompanionsForDiscussion();
  advanceAlternateReply = 1;
  if (Engine_ScriptChooseYesNo(LOCALID_PARTY_LEADER, 0) == 1) {
    gFieldWork->message++;
    advanceAlternateReply = 0;
  }
  McCoyPalace_FaceCompanionsForAudience();
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | LOCALID_LORD_MCCOY, 0, 10);
  if (advanceAlternateReply != 0) {
    gFieldWork->message++;
  }

  ObjectEvent_FaceDirection(LOCALID_LORD_MCCOY, FACING_SOUTHWEST + FACING_STEP,
                            10);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_CAPTAIN, 1);
  ObjectEvent_FaceDirection(LOCALID_CAPTAIN, FACING_EAST + FACING_STEP, 20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_LORD_MCCOY, 3);
  Engine_ScriptWait(20);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_CAPTAIN, 3);
  Script_ShowMessageAndWait(LOCALID_CAPTAIN, 0, 10);
  ObjectEvent_FaceDirection(LOCALID_CAPTAIN, FACING_SOUTHEAST + FACING_STEP,
                            10);
  ObjectEvent_FaceDirection(LOCALID_LORD_MCCOY, FACING_SOUTHEAST + FACING_STEP,
                            20);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_LORD_MCCOY, 1);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | LOCALID_LORD_MCCOY, 0, 10);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_LORD_MCCOY, 3);
  Script_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | LOCALID_LORD_MCCOY, 0, 10);
  Engine_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
  Engine_ObjectEventSetAnimation(GARET, ANIM_NOD);
  Engine_ObjectEventSetAnimation(IVAN, ANIM_NOD);
  Engine_ObjectEventSetAnimationAndWait(MIA, ANIM_NOD);
  Engine_ScriptWait(20);

  Engine_ObjectEventSetAnimation(GARET, ANIM_WALK);
  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (objectEvent != 0) {
    ObjectEvent_SetDestination(GARET, objectEvent->x.part.pixel,
                               objectEvent->z.part.pixel);
  }
  Engine_ObjectEventSetAnimation(IVAN, ANIM_WALK);
  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (objectEvent != 0) {
    ObjectEvent_SetDestination(IVAN, objectEvent->x.part.pixel,
                               objectEvent->z.part.pixel);
  }
  Engine_ObjectEventSetAnimation(MIA, ANIM_WALK);
  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (objectEvent != 0) {
    ObjectEvent_SetDestination(MIA, objectEvent->x.part.pixel,
                               objectEvent->z.part.pixel);
  }

  Engine_ObjectEventWaitForMove(GARET);
  ObjectEvent_SetPosition(GARET, 0, 0);
  ObjectEvent_SetPosition(IVAN, 0, 0);
  Engine_ObjectEventWaitForMove(MIA);
  ObjectEvent_SetPosition(MIA, 0, 0);
  Engine_ScriptWait(20);
  ObjectEvent_FaceDirection(LOCALID_LORD_MCCOY, FACING_SOUTH + FACING_STEP, 0);
  Object_SetTargetAndCallback(LOCALID_PARTY_LEADER,
                              OBJ_EVENT_LINK_KEEP_MOTION | LOCALID_CAPTAIN,
                              gPalace_FacingActions);
  ObjectEvent_WalkToAndWait(LOCALID_CAPTAIN, 852, 646);
  ObjectEvent_WalkToAndWait(LOCALID_CAPTAIN, 852, 666);
  ObjectEvent_WalkToAndWait(LOCALID_CAPTAIN, 864, 672);
  ObjectEvent_FaceDirection(LOCALID_CAPTAIN, FACING_EAST + FACING_STEP, 10);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_CAPTAIN, 1);
  Engine_ScriptWait(10);
  Script_ShowMessageAndWait(LOCALID_CAPTAIN, 0, 10);
  ObjectEvent_WalkToAndWait(LOCALID_CAPTAIN, 886, 708);
  ObjectEvent_WalkTo(LOCALID_CAPTAIN, 894, 764);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 894, 764);
  Engine_ScriptCloseScreen();
  Engine_ScriptWaitForScreen();
  GameFlag_Set(FLAG_REWARD_OFFERED);
  if (GameFlag_IsSet(FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY) == 0) {
    GameFlag_Set(FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY);
    GameFlag_Set(FLAG_MCCOY_PALACE_ADMITTED);
  }
  Engine_ScriptRequestWarp(MCCOY_PALACE_ENTRANCE_ROOM_TO_TREASURE_ROOM);
  Engine_ScriptEnd();
}

/// McCoy's Palace, at MCCOY_PALACE_TREASURE_ROOM_FROM_ENTRANCE_ROOM, the
/// treasure room: the captain shows the four chests and the soldier who keeps
/// the treasure explains that only one may be chosen.
static void EventScript_PresentRewardChests(void) {
  struct ObjectEvent *objectEvent;

  Engine_ScriptStart();
  // Restore the chest-row metatile behavior patches before presenting the
  // reward.
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Engine_ScriptGetViewCenter()->motionFlags = 0;
  Engine_CameraMoveTo(PIXELS(628), -1, PIXELS(748), 0);
  McCoyPalace_OpenRewardChestRow();
  ObjectEvent_GetById(LOCALID_PARTY_LEADER)->facing = 0;
  ObjectEvent_SetPosition(LOCALID_PARTY_LEADER, PIXELS(577), PIXELS(760));
  ObjectEvent_GetById(LOCALID_CAPTAIN)->facing = 0;
  Engine_ObjectEventSetPosition(LOCALID_CAPTAIN, PIXELS(592), PIXELS(760));
  ObjectEvent_GetById(LOCALID_SOLDIER_SHOWING_THE_REWARD)->facing =
      FACING_WEST + FACING_STEP;
  ObjectEvent_SetPosition(LOCALID_SOLDIER_SHOWING_THE_REWARD, PIXELS(662),
                          PIXELS(764));
  McCoyPalace_PositionRewardChests();
  Engine_ObjectEventSetSpriteFlags(ObjectEvent_GetById(LOCALID_VIAL_CHEST), 0);
  Engine_ObjectEventSetSpriteFlags(ObjectEvent_GetById(LOCALID_POTION_CHEST),
                                   0);
  Engine_ObjectEventSetSpriteFlags(
      ObjectEvent_GetById(LOCALID_PSY_CRYSTAL_CHEST), 0);
  Engine_ObjectEventSetSpriteFlags(
      ObjectEvent_GetById(LOCALID_WATER_OF_LIFE_CHEST), 0);
  objectEvent = ObjectEvent_GetById(LOCALID_VIAL_CHEST);
  objectEvent->motionFlags = 0;
  ObjectEvent_GetById(LOCALID_POTION_CHEST)->motionFlags = 0;
  ObjectEvent_GetById(LOCALID_PSY_CRYSTAL_CHEST)->motionFlags = 0;
  ObjectEvent_GetById(LOCALID_WATER_OF_LIFE_CHEST)->motionFlags = 0;
  ObjectEvent_GetById(LOCALID_VIAL_CHEST)->y.fixed = -PIXELS(4);
  ObjectEvent_GetById(LOCALID_POTION_CHEST)->y.fixed = -PIXELS(4);
  ObjectEvent_GetById(LOCALID_PSY_CRYSTAL_CHEST)->y.fixed = -PIXELS(4);
  ObjectEvent_GetById(LOCALID_WATER_OF_LIFE_CHEST)->y.fixed = -PIXELS(4);
  Engine_MapGridRedraw();
  Engine_TaskWait(1);
  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  gFieldWork->transitionFrames = 16;
  Engine_ScriptOpenScreen();
  Engine_ScriptWaitForScreen();
  ObjectEvent_SetSpeed(LOCALID_CAPTAIN, PIXELS(3) / 5, PIXELS(3) / 10);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(3) / 5, PIXELS(3) / 10);
  ObjectEvent_WalkTo(LOCALID_CAPTAIN, 628, 764);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 612, 764);
  Engine_ObjectEventSetAnimation(LOCALID_CAPTAIN, ANIM_STAND);
  Engine_ScriptWait(20);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_CAPTAIN, 1);
  // The captain and the keeper explain that only one chest may be chosen.
  Engine_ScriptSetMessage(MsgBilibinLordMccoyOrdered);
  Engine_ScriptShowMessageAndWait(LOCALID_CAPTAIN, 0, 10);
  ObjectEvent_WalkToAndWait(LOCALID_CAPTAIN, 622, 780);
  ObjectEvent_FaceDirection(LOCALID_CAPTAIN, FACING_NORTH, 10);
  Engine_ObjectEventRunRepeatedMotion(LOCALID_SOLDIER_SHOWING_THE_REWARD, 2);
  Engine_ScriptShowMessageAndWait(LOCALID_SOLDIER_SHOWING_THE_REWARD, 0, 10);
  Engine_ObjectEventSetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD);
  Engine_GameFlagClear(FLAG_SHOW_LOCATION_NAME);
  Engine_GameFlagSet(FLAG_CHOOSING_REWARD);
  Engine_ScriptEnd();
}

s32 gPalace_FacingActions[] = {
    SC_CALL(Map_TurnTowardTarget),
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

struct MapWarpDest gMcCoyPalaceWarpDests[] = {
    {WARP_DEST_DEFAULT, CONDITION_ALWAYS, 256, 0, 808, FACING_NORTH, 0, -1, -1,
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
    {MAP_TABLE_END},
};

u32 gMcCoyPalaceWarps[] = {
    MAP_WARPS(McCoyPalace),
    MAP_WARP(MCCOY_PALACE_TO_MCCOY_PALACE_GROUNDS_WEST, McCoyPalaceGrounds,
             MCCOY_PALACE_GROUNDS_FROM_MCCOY_PALACE),
    MAP_WARP(MCCOY_PALACE_ENTRANCE_ROOM_TO_SOUTHWEST_LANDING, McCoyPalace,
             MCCOY_PALACE_SOUTHWEST_LANDING_FROM_ENTRANCE_ROOM),
    MAP_WARP(MCCOY_PALACE_ENTRANCE_ROOM_TO_SOUTHEAST_LANDING, McCoyPalace,
             MCCOY_PALACE_SOUTHEAST_LANDING_FROM_ENTRANCE_ROOM),
    MAP_WARP(MCCOY_PALACE_ENTRANCE_ROOM_TO_LADY_MCCOYS_ROOM_WEST, McCoyPalace,
             MCCOY_PALACE_LADY_MCCOYS_ROOM_FROM_ENTRANCE_ROOM_WEST),
    MAP_WARP(MCCOY_PALACE_ENTRANCE_ROOM_TO_LADY_MCCOYS_ROOM_EAST, McCoyPalace,
             MCCOY_PALACE_LADY_MCCOYS_ROOM_FROM_ENTRANCE_ROOM_EAST),
    MAP_WARP(MCCOY_PALACE_ENTRANCE_ROOM_TO_TREASURE_ROOM, McCoyPalace,
             MCCOY_PALACE_TREASURE_ROOM_FROM_ENTRANCE_ROOM),
    MAP_WARP(MCCOY_PALACE_ENTRANCE_ROOM_TO_NORTHWEST_LANDING, McCoyPalace,
             MCCOY_PALACE_NORTHWEST_LANDING_FROM_ENTRANCE_ROOM),
    MAP_WARP(MCCOY_PALACE_ENTRANCE_ROOM_TO_NORTHEAST_LANDING, McCoyPalace,
             MCCOY_PALACE_NORTHEAST_LANDING_FROM_ENTRANCE_ROOM),
    MAP_WARP(MCCOY_PALACE_ENTRANCE_ROOM_TO_MCCOYS_ROOM, McCoyPalace,
             MCCOY_PALACE_FROM_MCCOY_PALACE_GROUNDS_EAST),
    MAP_WARP(MCCOY_PALACE_LADY_MCCOYS_ROOM_TO_ENTRANCE_ROOM_WEST, McCoyPalace,
             MCCOY_PALACE_ENTRANCE_ROOM_FROM_LADY_MCCOYS_ROOM_WEST),
    MAP_WARP(MCCOY_PALACE_LADY_MCCOYS_ROOM_TO_ENTRANCE_ROOM_EAST, McCoyPalace,
             MCCOY_PALACE_ENTRANCE_ROOM_FROM_LADY_MCCOYS_ROOM_EAST),
    MAP_WARP(MCCOY_PALACE_LADY_MCCOYS_ROOM_TO_WEST_BEDROOM, McCoyPalace,
             MCCOY_PALACE_WEST_BEDROOM_FROM_LADY_MCCOYS_ROOM),
    MAP_WARP(MCCOY_PALACE_LADY_MCCOYS_ROOM_TO_EAST_BEDROOM, McCoyPalace,
             MCCOY_PALACE_EAST_BEDROOM_FROM_LADY_MCCOYS_ROOM),
    MAP_WARP(MCCOY_PALACE_SOUTHWEST_LANDING_TO_ENTRANCE_ROOM, McCoyPalace,
             MCCOY_PALACE_ENTRANCE_ROOM_FROM_SOUTHWEST_LANDING),
    MAP_WARP(MCCOY_PALACE_SOUTHWEST_LANDING_TO_SOUTHWEST_TOWER, McCoyPalace,
             MCCOY_PALACE_SOUTHWEST_TOWER_FROM_SOUTHWEST_LANDING),
    MAP_WARP(MCCOY_PALACE_SOUTHEAST_LANDING_TO_SOUTHEAST_TOWER, McCoyPalace,
             MCCOY_PALACE_SOUTHEAST_TOWER_FROM_SOUTHEAST_LANDING),
    MAP_WARP(MCCOY_PALACE_SOUTHEAST_LANDING_TO_ENTRANCE_ROOM, McCoyPalace,
             MCCOY_PALACE_ENTRANCE_ROOM_FROM_SOUTHEAST_LANDING),
    MAP_WARP(MCCOY_PALACE_WEST_BEDROOM_TO_LADY_MCCOYS_ROOM, McCoyPalace,
             MCCOY_PALACE_LADY_MCCOYS_ROOM_FROM_WEST_BEDROOM),
    MAP_WARP(MCCOY_PALACE_EAST_BEDROOM_TO_LADY_MCCOYS_ROOM, McCoyPalace,
             MCCOY_PALACE_LADY_MCCOYS_ROOM_FROM_EAST_BEDROOM),
    MAP_WARP(MCCOY_PALACE_NORTHEAST_LANDING_TO_ENTRANCE_ROOM, McCoyPalace,
             MCCOY_PALACE_ENTRANCE_ROOM_FROM_NORTHEAST_LANDING),
    MAP_WARP(MCCOY_PALACE_NORTHEAST_LANDING_TO_NORTHEAST_TOWER, McCoyPalace,
             MCCOY_PALACE_NORTHEAST_TOWER_FROM_NORTHEAST_LANDING),
    MAP_WARP(MCCOY_PALACE_NORTHWEST_LANDING_TO_ENTRANCE_ROOM, McCoyPalace,
             MCCOY_PALACE_ENTRANCE_ROOM_FROM_NORTHWEST_LANDING),
    MAP_WARP(MCCOY_PALACE_NORTHWEST_LANDING_TO_NORTHWEST_TOWER, McCoyPalace,
             MCCOY_PALACE_NORTHWEST_TOWER_FROM_NORTHWEST_LANDING),
    MAP_WARP(MCCOY_PALACE_NORTHWEST_TOWER_TO_NORTHWEST_LANDING, McCoyPalace,
             MCCOY_PALACE_NORTHWEST_LANDING_FROM_NORTHWEST_TOWER),
    MAP_WARP(MCCOY_PALACE_NORTHEAST_TOWER_TO_NORTHEAST_LANDING, McCoyPalace,
             MCCOY_PALACE_NORTHEAST_LANDING_FROM_NORTHEAST_TOWER),
    MAP_WARP(MCCOY_PALACE_SOUTHWEST_TOWER_TO_SOUTHWEST_LANDING, McCoyPalace,
             MCCOY_PALACE_SOUTHWEST_LANDING_FROM_SOUTHWEST_TOWER),
    MAP_WARP(MCCOY_PALACE_SOUTHEAST_TOWER_TO_SOUTHEAST_LANDING, McCoyPalace,
             MCCOY_PALACE_SOUTHEAST_LANDING_FROM_SOUTHEAST_TOWER),
    MAP_WARP(MCCOY_PALACE_TREASURE_ROOM_TO_ENTRANCE_ROOM, McCoyPalace,
             MCCOY_PALACE_ENTRANCE_ROOM_FROM_TREASURE_ROOM),
    MAP_WARP(MCCOY_PALACE_MCCOYS_ROOM_TO_ENTRANCE_ROOM, McCoyPalace,
             MCCOY_PALACE_ENTRANCE_ROOM_FROM_MCCOYS_ROOM),
    MAP_WARPS_END,
};

/// Unused: the getters return it for any map but MapId_McCoyPalace, and the
/// overlay serves no other.
struct ObjectEventTemplate gMcCoyPalaceNoObjectEventTemplates[] = {
    {MAP_TABLE_END},
};

// clang-format off
struct ObjectEventTemplate gMcCoyPalaceObjectEventTemplates[] = {
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
    { GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTHWEST, TALK_FACING_KEEP, 0 },
    { IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTHEAST, TALK_FACING_KEEP, 0 },
    { MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_CHEST, CONDITION_IN_VIEW, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(616), 0, PIXELS(616), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_CHEST, CONDITION_IN_VIEW, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(632), 0, PIXELS(616), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_CHEST, CONDITION_IN_VIEW, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(648), 0, PIXELS(616), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_CHEST, CONDITION_IN_VIEW, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(664), 0, PIXELS(616), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_CHEST, CONDITION_IN_VIEW, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(616), 0, PIXELS(648), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_CHEST, CONDITION_IN_VIEW, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(648), 0, PIXELS(648), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_CHEST, CONDITION_IN_VIEW, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(664), 0, PIXELS(648), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_CHEST, CONDITION_IN_VIEW, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(616), 0, PIXELS(664), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_CHEST, CONDITION_IN_VIEW, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(648), 0, PIXELS(664), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_CHEST, CONDITION_IN_VIEW, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(664), 0, PIXELS(664), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { MAP_TABLE_END },
};

/// Unused, as gMcCoyPalaceNoObjectEventTemplates is.
struct MapEvent gMcCoyPalaceNoEvents[] = {
    { MAP_EVENTS_END },
};

struct MapEvent gMcCoyPalaceEvents[] = {
    { MAP_EVENT_WARP, 1, CONDITION_ALWAYS, MCCOY_PALACE_TO_MCCOY_PALACE_GROUNDS_WEST },
    { MAP_EVENT_WARP, 2, CONDITION_ALWAYS, MCCOY_PALACE_ENTRANCE_ROOM_TO_SOUTHWEST_LANDING },
    { MAP_EVENT_WARP, 3, CONDITION_ALWAYS, MCCOY_PALACE_ENTRANCE_ROOM_TO_SOUTHEAST_LANDING },
    { MAP_EVENT_WARP, 4, CONDITION_ALWAYS, MCCOY_PALACE_ENTRANCE_ROOM_TO_LADY_MCCOYS_ROOM_WEST },
    { MAP_EVENT_WARP, 5, CONDITION_ALWAYS, MCCOY_PALACE_ENTRANCE_ROOM_TO_LADY_MCCOYS_ROOM_EAST },
    { MAP_EVENT_WARP, 6, CONDITION_ALWAYS, MCCOY_PALACE_ENTRANCE_ROOM_TO_TREASURE_ROOM },
    { MAP_EVENT_WARP, 7, CONDITION_ALWAYS, MCCOY_PALACE_ENTRANCE_ROOM_TO_NORTHWEST_LANDING },
    { MAP_EVENT_WARP, 8, CONDITION_ALWAYS, MCCOY_PALACE_ENTRANCE_ROOM_TO_NORTHEAST_LANDING },
    { COORD_EVENT_FACING(FACING_NORTH), 9, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WalkThroughDoor) },
    { MAP_EVENT_WARP, 10, CONDITION_ALWAYS, MCCOY_PALACE_LADY_MCCOYS_ROOM_TO_ENTRANCE_ROOM_WEST },
    { MAP_EVENT_WARP, 11, CONDITION_ALWAYS, MCCOY_PALACE_LADY_MCCOYS_ROOM_TO_ENTRANCE_ROOM_EAST },
    { COORD_EVENT_FACING(FACING_NORTH), 12, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WalkThroughDoor) },
    { COORD_EVENT_FACING(FACING_NORTH), 13, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_WalkThroughDoor) },
    { MAP_EVENT_WARP, 14, CONDITION_ALWAYS, MCCOY_PALACE_SOUTHWEST_LANDING_TO_ENTRANCE_ROOM },
    { MAP_EVENT_WARP, 15, CONDITION_ALWAYS, MCCOY_PALACE_SOUTHWEST_LANDING_TO_SOUTHWEST_TOWER },
    { MAP_EVENT_WARP, 16, CONDITION_ALWAYS, MCCOY_PALACE_SOUTHEAST_LANDING_TO_SOUTHEAST_TOWER },
    { MAP_EVENT_WARP, 17, CONDITION_ALWAYS, MCCOY_PALACE_SOUTHEAST_LANDING_TO_ENTRANCE_ROOM },
    { MAP_EVENT_WARP, 18, CONDITION_ALWAYS, MCCOY_PALACE_WEST_BEDROOM_TO_LADY_MCCOYS_ROOM },
    { MAP_EVENT_WARP, 19, CONDITION_ALWAYS, MCCOY_PALACE_EAST_BEDROOM_TO_LADY_MCCOYS_ROOM },
    { MAP_EVENT_WARP, 20, CONDITION_ALWAYS, MCCOY_PALACE_NORTHEAST_LANDING_TO_ENTRANCE_ROOM },
    { MAP_EVENT_WARP, 21, CONDITION_ALWAYS, MCCOY_PALACE_NORTHEAST_LANDING_TO_NORTHEAST_TOWER },
    { MAP_EVENT_WARP, 22, CONDITION_ALWAYS, MCCOY_PALACE_NORTHWEST_LANDING_TO_ENTRANCE_ROOM },
    { MAP_EVENT_WARP, 23, CONDITION_ALWAYS, MCCOY_PALACE_NORTHWEST_LANDING_TO_NORTHWEST_TOWER },
    { MAP_EVENT_WARP, 24, CONDITION_ALWAYS, MCCOY_PALACE_NORTHWEST_TOWER_TO_NORTHWEST_LANDING },
    { MAP_EVENT_WARP, 25, CONDITION_ALWAYS, MCCOY_PALACE_NORTHEAST_TOWER_TO_NORTHEAST_LANDING },
    { MAP_EVENT_WARP, 26, CONDITION_ALWAYS, MCCOY_PALACE_SOUTHWEST_TOWER_TO_SOUTHWEST_LANDING },
    { MAP_EVENT_WARP, 27, CONDITION_ALWAYS, MCCOY_PALACE_SOUTHEAST_TOWER_TO_SOUTHEAST_LANDING },
    { MAP_EVENT_WARP, 28, CONDITION_ALWAYS, MCCOY_PALACE_TREASURE_ROOM_TO_ENTRANCE_ROOM },
    { MAP_EVENT_WARP, 29, CONDITION_ALWAYS, MCCOY_PALACE_MCCOYS_ROOM_TO_ENTRANCE_ROOM },
    { MAP_EVENT_TALK, LOCALID_VILLAGER_FOLLOW_RED_CARPET, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinIfYouWantToSeeLord) },
    { MAP_EVENT_TALK, LOCALID_VILLAGER_FOLLOW_RED_CARPET, CONDITION_ALWAYS, MSGNO(MsgBilibinThePeopleOfBilibinWillNever) },
    { MAP_EVENT_TALK, LOCALID_VILLAGER_DO_NOT_OFFEND_MCCOY, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinMakeSureYouDontOffendLord) },
    { MAP_EVENT_TALK, LOCALID_VILLAGER_DO_NOT_OFFEND_MCCOY, CONDITION_ALWAYS, MSGNO(MsgBilibinIHeardYouReceivedASpectacular) },
    { MAP_EVENT_TALK, LOCALID_VILLAGER_ASKING_ABOUT_KOLIMA_FOREST, FLAG_MCCOY_PALACE_REWARD_TAKEN, EVENT_SCRIPT(EventScript_AskAboutGoingToKolima) },
    { MAP_EVENT_TALK, LOCALID_VILLAGER_ASKING_ABOUT_KOLIMA_FOREST, CONDITION_ALWAYS, MSGNO(MsgBilibinLadyMccoyIsQuiteUpsetThat) },
    { MAP_EVENT_TALK, LOCALID_YOUNG_MAN_PROTECT_OLD_TREE, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinIAlwaysBelievedThatTheOld) },
    { MAP_EVENT_TALK, LOCALID_YOUNG_MAN_PROTECT_OLD_TREE, CONDITION_ALWAYS, MSGNO(MsgBilibinThingsAreLookingGrimSouthOf) },
    { MAP_EVENT_TALK, LOCALID_VILLAGER_LADY_MCCOYS_ROOM, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinThisIsLadyMccoysRoomSo) },
    { MAP_EVENT_TALK, LOCALID_VILLAGER_LADY_MCCOYS_ROOM, CONDITION_ALWAYS, MSGNO(MsgBilibinMiladyWasVeryUpsetWhenShe) },
    { MAP_EVENT_TALK, LOCALID_VILLAGER_MILADYS_BEAUTY, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinMiladyWasRenownedFarAndWide) },
    { MAP_EVENT_TALK, LOCALID_VILLAGER_MILADYS_BEAUTY, CONDITION_ALWAYS, MSGNO(MsgBilibinForOnceMilordDidntLetLady) },
    { MAP_EVENT_TALK, LOCALID_VILLAGER_IN_MILADYS_NIGHTGOWN, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinOhSomedayIWantToSleep) },
    { MAP_EVENT_TALK, LOCALID_VILLAGER_IN_MILADYS_NIGHTGOWN, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToWomanInMiladysNightgown) },
    { MAP_EVENT_TALK, LOCALID_OLD_MAN_PRAISING_THE_WARRIORS, FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY, MSGNO(MsgBilibinYouCanSeeKolimaPrettyWell) },
    { MAP_EVENT_TALK, LOCALID_OLD_MAN_PRAISING_THE_WARRIORS, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinHeyIfYouCantDoThe) },
    { MAP_EVENT_TALK, LOCALID_OLD_MAN_PRAISING_THE_WARRIORS, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToOldManPraisingTheWarriors) },
    { MAP_EVENT_TALK, LOCALID_SOLDIER_WATCHING_KOLIMA, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinHmmItSeemsTheCurseOf) },
    { MAP_EVENT_TALK, LOCALID_SOLDIER_WATCHING_KOLIMA, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToSoldierWatchingKolima) },
    { MAP_EVENT_TALK, LOCALID_CAPTAIN, CONDITION_ALWAYS, MSGNO(MsgBilibinYouMaySelectASingleItem) },
    { MAP_EVENT_TALK, LOCALID_SOLDIER_SHOWING_THE_REWARD, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToSoldierShowingTheReward) },
    { MAP_EVENT_TALK, LOCALID_LORD_MCCOY, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinSorryLadsButKolimasFutureIs) },
    { MAP_EVENT_TALK, LOCALID_LORD_MCCOY, CONDITION_ALWAYS, MSGNO(MsgBilibinYehveTaughtMeThatICannae) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_VILLAGER_FOLLOW_RED_CARPET, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinHowManyTimesDoIHave) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_VILLAGER_FOLLOW_RED_CARPET, CONDITION_ALWAYS, MSGNO(MsgBilibinIFeelSorryForTheWarriors) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_VILLAGER_DO_NOT_OFFEND_MCCOY, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinTheyShouldNeverHaveLetSuch) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_VILLAGER_DO_NOT_OFFEND_MCCOY, CONDITION_ALWAYS, MSGNO(MsgBilibinIWonderWhatKindOfTreasure) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_VILLAGER_ASKING_ABOUT_KOLIMA_FOREST, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinSendingTheseChildrenToKolimaForest) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_VILLAGER_ASKING_ABOUT_KOLIMA_FOREST, CONDITION_ALWAYS, MSGNO(MsgBilibinIWishShedJustStopWhining) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_YOUNG_MAN_PROTECT_OLD_TREE, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinIShouldntHaveSaidAnythingWhen) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_YOUNG_MAN_PROTECT_OLD_TREE, CONDITION_ALWAYS, MSGNO(MsgBilibinTheydBeGuaranteedAGoodLife) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_VILLAGER_LADY_MCCOYS_ROOM, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinOfAllTheNerveComingInto) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_VILLAGER_LADY_MCCOYS_ROOM, CONDITION_ALWAYS, MSGNO(MsgBilibinICanTellWhenShesUpset) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_VILLAGER_MILADYS_BEAUTY, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinWillIEverFindSomeoneTo) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_VILLAGER_MILADYS_BEAUTY, CONDITION_ALWAYS, MSGNO(MsgBilibinSometimesAMansJustGottaPut) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_VILLAGER_IN_MILADYS_NIGHTGOWN, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinOhNoThisNightgownHasA) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_VILLAGER_IN_MILADYS_NIGHTGOWN, CONDITION_ALWAYS, MSGNO(MsgBilibinTheHoleInThisNightgownIs) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_OLD_MAN_PRAISING_THE_WARRIORS, FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY, MSGNO(MsgBilibinLordMccoyHasntComeHereFor) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_OLD_MAN_PRAISING_THE_WARRIORS, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinItsAToughThingBeingSo) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_OLD_MAN_PRAISING_THE_WARRIORS, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ReadOldMansThoughtsOnTheWarriors) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SOLDIER_WATCHING_KOLIMA, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinKolimaForestUsedToBeSo) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SOLDIER_WATCHING_KOLIMA, CONDITION_ALWAYS, MSGNO(MsgBilibinACourageousWarriorWouldntThinkTwice) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_CAPTAIN, CONDITION_ALWAYS, MSGNO(MsgBilibinTheseCourageousWarriorsDeserveToReceive) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_SOLDIER_SHOWING_THE_REWARD, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ReadRewardSoldiersThoughts) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_LORD_MCCOY, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinThereIsNothingMoreToTalk) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_LORD_MCCOY, CONDITION_ALWAYS, MSGNO(MsgBilibinButICannaeFiggerOutHow) },
    { BG_EVENT(BG_EVENT_SILENT), 100, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SearchVialChest) },
    { BG_EVENT(BG_EVENT_SILENT), 101, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SearchPotionChest) },
    { BG_EVENT(BG_EVENT_SILENT), 102, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SearchPsyCrystalChest) },
    { BG_EVENT(BG_EVENT_SILENT), 103, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SearchWaterOfLifeChest) },
    { MAP_EVENT_COORD, 50, FLAG_PARTY_SEEN_OFF, EVENT_SCRIPT(EventScript_CaptainSeesPartyOff) },
    { MAP_EVENT_COORD, 51, FLAG_PARTY_SEEN_OFF, EVENT_SCRIPT(EventScript_CaptainAsksPartyToChoose) },
    { BG_EVENT(BG_EVENT_BARREL), 120, FLAG_MCCOY_PALACE_BARREL_ELIXIR, FIND_ITEM(ITEM_ELIXIR) },
    { BG_EVENT(BG_EVENT_BARREL), 121, FLAG_MCCOY_PALACE_BARREL_SMOKE_BOMB, FIND_ITEM(ITEM_SMOKE_BOMB) },
    { BG_EVENT(BG_EVENT_WARDROBE), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBilibinTheresASilkNegligeeIsaacPut)) },
    { BG_EVENT(BG_EVENT_BOOKCASE), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBilibinTheresASheetOfPaperTitled)) },
    { MAP_EVENTS_END },
};
// clang-format on
