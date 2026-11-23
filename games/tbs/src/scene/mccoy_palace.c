/// @file McCoy's Palace in Bilibin, the home of the town's chief.
///
/// The palace is one scene, SceneId_McCoyPalace, sharing the area name McCoy's
/// Palace with its grounds (mccoy_palace_grounds.c). Its rooms are side by
/// side on one map, each in its own camera box: the entrance hall with its red
/// carpet, McCoy's room at the carpet's top, Lady McCoy's room with a bedroom
/// through each of its two back doors, the landings and small tower rooms
/// at the hall's four corners, and the treasure room below, reached from the
/// west of the hall.
///
/// The party first comes in by the front door once the east guard on the
/// grounds lets it in. A man in the hall says to follow the red carpet, and
/// walking through the door at its top (McCoyPalace_WalkThroughDoor) brings
/// Isaac before McCoy, a stout red-haired man who speaks with a heavy accent
/// and a portrait. Before Kolima's curse is lifted this is the first audience
/// (McCoyPalace_AudienceWithMcCoy): McCoy asks whether the party will save
/// Kolima. If Isaac says no, he is sent away and can come back to change his
/// mind. If he says yes, McCoy tosses him the key to the barricades, then
/// takes it back, deciding they are too young, and his captain sees the party
/// out. Outside his door the captain is sorry, and lets slip that
/// the barricade on the way to Kolima may not need a key
/// (McCoyPalace_CaptainTalksAboutBarricade); from then on the people speak as
/// if the party had been turned away
/// (FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY).
///
/// Once the curse is lifted, the guards on the grounds march the party
/// straight into McCoy's room, where McCoy thanks it
/// (McCoyPalace_McCoyRewardsParty). His lines differ as the party has met him
/// before or not. The captain then leads it down to the treasure room, where
/// four chests stand in a row before the barred gate
/// (McCoyPalace_PresentRewardChests): from west to east a Vial, a Potion, a
/// Psy Crystal and a Water of Life. Only one may be taken
/// (McCoyPalace_ClaimReward); the others are then locked. Heading back up the
/// stairs without a reward, the captain stops Isaac and sends him back to the
/// chests; once he has one, the captain bids the party farewell. When the
/// party comes back later, the four chests are gone, and which one it opened
/// was only ever kept in the scene's temporary flags (FLAG_TOOK_VIAL and the
/// others).
///
/// The palace's people follow three story flags,
/// FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY, FLAG_KOLIMA_CURSE_LIFTED and
/// FLAG_MCCOY_PALACE_REWARD_TAKEN, which decide what they say and how they
/// turn when talked to (Scene_GetActorPlacements).
///
/// A barrel by the chimney in the hall holds an Elixir and one in the
/// small room at the south-west corner a Smoke Bomb. The wardrobe in the east
/// bedroom holds Lady McCoy's silk negligee, and the bookcase in the west
/// bedroom the long list of gifts McCoy has bought her.

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "types.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/actor_script.h"
#include "field/event_commands.h"
#include "field/event_inline.h"
#include "field/field.h"
#include "field/scene_tables.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/main_routine_work.h"
#include "system/system.h"
#include "text/msg_ids.h"

extern s32 gPalace_FacingActions[];
extern s32 gPalace_PacingActions[];
extern struct SceneEntrance gMcCoyPalaceEntrances[];
extern u32 gMcCoyPalaceExits[];
extern struct ActorPlacement gMcCoyPalaceNoActorPlacements[];
extern struct ActorPlacement gMcCoyPalaceActorPlacements[];
extern struct Trigger gMcCoyPalaceNoTriggers[];
extern struct Trigger gMcCoyPalaceTriggers[];

/// The palace's people and the treasure room's chests, numbering
/// gMcCoyPalaceActorPlacements' rows from ACTOR_FIRST_PLACED.
/// Each note gives what they say before the reward, then after it.
enum McCoyPalaceActorId {
  /// A burly grey-haired man who points the way to McCoy. "If you want to see
  /// Lord McCoy, follow the red carpet." Later: "The people of Bilibin will
  /// never forget your bravery."
  ACTOR_VILLAGER_FOLLOW_RED_CARPET = ACTOR_FIRST_PLACED,
  /// A woman in a green dress who warns the party about McCoy. "Make sure you
  /// don't offend Lord McCoy!" Later: "I heard you received a spectacular
  /// reward."
  ACTOR_VILLAGER_DO_NOT_OFFEND_MCCOY,
  /// A woman with fair hair in the hall who asks whether the party is willing
  /// to go to Kolima Forest. Later: "Lady McCoy is quite upset that the palace
  /// construction was halted."
  ACTOR_VILLAGER_ASKING_ABOUT_KOLIMA_FOREST,
  /// A young man with brown hair who speaks of the old tree in Kolima Forest.
  /// Later: "Things are looking grim south of Goma Range..."
  ACTOR_YOUNG_MAN_PROTECT_OLD_TREE,
  /// A woman with fair hair standing in Lady McCoy's room. "This is Lady
  /// McCoy's room, so don't touch anything." Later: "Milady was very upset when
  /// she left the room."
  ACTOR_VILLAGER_LADY_MCCOYS_ROOM,
  /// A woman with fair hair who admires Lady McCoy. "Milady was renowned far
  /// and wide for her beauty!" Later: "For once, milord didn't let Lady McCoy
  /// push him around!"
  ACTOR_VILLAGER_MILADYS_BEAUTY,
  /// A woman with fair hair who screams when spoken to ("AAAAH!!!"). Mind Read:
  /// "Oh, no! This nightgown has a hole... Milady will be furious!"
  ACTOR_VILLAGER_IN_MILADYS_NIGHTGOWN,
  /// An old man with a cane in the north-east tower room. "You can see Kolima
  /// pretty well from here." Later he praises the party.
  ACTOR_OLD_MAN_PRAISING_THE_WARRIORS,
  /// A soldier in a spiked helmet in the south-east tower room. "Hmm. It seems
  /// the curse of the tree has not yet come to Bilibin." Later he says he
  /// watched the party go to Kolima.
  ACTOR_SOLDIER_WATCHING_KOLIMA,
  /// A soldier in a spiked helmet who guards the treasure room's barred gate,
  /// and shows the party the four chests when the captain brings it down.
  ACTOR_SOLDIER_SHOWING_THE_REWARD,
  /// McCoy, the chief of Bilibin, a stout man with red hair in a pink tunic.
  ACTOR_LORD_MCCOY,
  /// The captain of McCoy's soldiers, drawn as they are, in a spiked helmet.
  ACTOR_CAPTAIN,
  /// The key McCoy tosses to the party.
  ACTOR_BARRICADE_KEY,
  ACTOR_VIAL_CHEST,
  ACTOR_POTION_CHEST,
  ACTOR_PSY_CRYSTAL_CHEST,
  ACTOR_WATER_OF_LIFE_CHEST
};

/// The cells the triggers answer besides the exits, whose cell behaviors are
/// their exits' numbers.
enum McCoyPalaceCellBehavior {
  /// The row just in front of the four reward chests, cells (38, 46) to
  /// (41, 46), copied there from (42, 55) to (45, 55) when the chests are
  /// brought out.
  CELL_BEHAVIOR_MCCOY_PALACE_BEFORE_REWARD_CHESTS = 50,
  /// The treasure room's floor at the foot of its stairs, cell (36, 47), one
  /// step before the way out.
  CELL_BEHAVIOR_MCCOY_PALACE_FOOT_OF_TREASURE_STAIRS = 51,
  /// The four reward chests' cells, (38, 45) to (41, 45) from west to east,
  /// copied there from (38, 55) to (41, 55) when the chests are brought out.
  CELL_BEHAVIOR_MCCOY_PALACE_VIAL_CHEST = 100,
  CELL_BEHAVIOR_MCCOY_PALACE_POTION_CHEST = 101,
  CELL_BEHAVIOR_MCCOY_PALACE_PSY_CRYSTAL_CHEST = 102,
  CELL_BEHAVIOR_MCCOY_PALACE_WATER_OF_LIFE_CHEST = 103,
  /// The barrel by the chimney in the hall, cell (23, 40), with an Elixir.
  CELL_BEHAVIOR_MCCOY_PALACE_ELIXIR_BARREL = 120,
  /// The barrel in the small room at the south-west corner, cell (45, 29), with
  /// a Smoke Bomb.
  CELL_BEHAVIOR_MCCOY_PALACE_SMOKE_BOMB_BARREL = 121,
  /// The east bedroom's wardrobe, cells (17, 3) to (18, 4), with Lady McCoy's
  /// silk negligee.
  CELL_BEHAVIOR_MCCOY_PALACE_WARDROBE = 200,
  /// The west bedroom's bookcase, cells (9, 3) to (10, 4), with the list of
  /// McCoy's gifts to his wife.
  CELL_BEHAVIOR_MCCOY_PALACE_BOOKCASE = 201
};

/// The palace's temporary flags.
/// The scene temp flags last until the party leaves the palace's scene, the
/// area temp flags until it leaves McCoy's Palace.
enum {
  /// The captain has opened McCoy's door, so walking through it is silent.
  FLAG_MCCOYS_DOOR_OPEN = FLAG_SCENE_TEMP_00,
  /// The party is choosing its reward among the four chests.
  FLAG_CHOOSING_REWARD = FLAG_SCENE_TEMP_02,
  /// The chest the reward was taken from, which then searches as empty.
  FLAG_TOOK_VIAL = FLAG_SCENE_TEMP_10,
  FLAG_TOOK_POTION = FLAG_SCENE_TEMP_11,
  FLAG_TOOK_PSY_CRYSTAL = FLAG_SCENE_TEMP_12,
  FLAG_TOOK_WATER_OF_LIFE = FLAG_SCENE_TEMP_13,
  /// The old man has praised the party once. He then warns of the holy tree.
  FLAG_OLD_MAN_HAS_PRAISED = FLAG_AREA_TEMP_02,
  /// Mind Read has heard the old man's first thought.
  FLAG_OLD_MAN_THOUGHTS_READ = FLAG_AREA_TEMP_03,
  /// McCoy has had the captain see the party out after the first audience.
  FLAG_CAPTAIN_SEEING_PARTY_OUT = FLAG_AREA_TEMP_21,
  /// McCoy has had the captain take the party to the treasure room, until it
  /// takes a reward.
  FLAG_REWARD_OFFERED = FLAG_AREA_TEMP_22,
  /// The captain has seen the party off with its reward.
  FLAG_PARTY_SEEN_OFF = FLAG_AREA_TEMP_33
};

static void McCoyPalace_ClaimReward(s32 chestActorId, s32 itemId,
                                    s32 chestFlag);
static void McCoyPalace_AudienceWithMcCoy(void);
static void McCoyPalace_CaptainTalksAboutBarricade(void);
static void McCoyPalace_McCoyRewardsParty(void);
static void McCoyPalace_PresentRewardChests(void);

// The shared facing routine precedes the scene's entry points.
#include "shared/actor_facing.c"

const struct SceneEntrance *Scene_GetEntrances(void) {
  return gMcCoyPalaceEntrances;
}

const struct SceneExitRegion *Scene_GetExitRegions(void) { return NULL; }

const u32 *Scene_GetExits(void) { return gMcCoyPalaceExits; }

/// The people of McCoy's Palace. Once the party has its reward, the woman in
/// Lady McCoy's nightgown no longer turns when spoken to, and the old man and
/// the soldiers in the tower rooms and the treasure room turn to the party.
const struct ActorPlacement *Scene_GetActorPlacements(void) {
  struct ActorPlacement *placements;

  if (gMainRoutineWork.next == SCENE_MC_COY_PALACE) {
    placements = gMcCoyPalaceActorPlacements;
    Scene_AssignViewFlags(placements);

    if (GameFlag_Get(FLAG_MCCOY_PALACE_REWARD_TAKEN) != 0) {
      gMcCoyPalaceActorPlacements[ACTOR_VILLAGER_IN_MILADYS_NIGHTGOWN -
                                  ACTOR_FIRST_PLACED]
          .talkFacing = TALK_FACING_KEEP;
      gMcCoyPalaceActorPlacements[ACTOR_OLD_MAN_PRAISING_THE_WARRIORS -
                                  ACTOR_FIRST_PLACED]
          .talkFacing = TALK_FACING_PARTY;
      gMcCoyPalaceActorPlacements[ACTOR_SOLDIER_WATCHING_KOLIMA -
                                  ACTOR_FIRST_PLACED]
          .talkFacing = TALK_FACING_PARTY_THEN_WATCH;
      gMcCoyPalaceActorPlacements[ACTOR_SOLDIER_SHOWING_THE_REWARD -
                                  ACTOR_FIRST_PLACED]
          .talkFacing = TALK_FACING_PARTY_AND_BACK;
    }

    return placements;
  }
  return gMcCoyPalaceNoActorPlacements;
}

/// What the party can talk to and search in the palace.
const struct Trigger *Scene_GetTriggers(void) {
  if (gMainRoutineWork.next == SCENE_MC_COY_PALACE) {
    return gMcCoyPalaceTriggers;
  }
  return gMcCoyPalaceNoTriggers;
}

/// Before the reward, the woman with fair hair in the hall asks "Oh, are you
/// willing to go to Kolima Forest?" On yes she has lost count of the warriors
/// McCoy has hired to end the curse, and not one has come back; on no, the
/// party does not look too strong, but that may be for the best.
static void McCoyPalace_AskAboutGoingToKolima(void) {
  Event_Start();
  Event_SetMessageId(MsgBilibinYouWillingGoKolimaForest);
  // "Oh, are you willing to go to Kolima Forest?"
  // Yes: "I forget how many people Lord McCoy has hired to end the..."
  // No: "You don't look too strong, but that may be for the best."
  Event_AskYesNo(ACTOR_VILLAGER_ASKING_ABOUT_KOLIMA_FOREST, 0);
  Event_End();
}

/// After the reward, the woman in Lady McCoy's nightgown in the east bedroom
/// winces and screams "AAAAH!!!" when spoken to, then turns: "Oh! For
/// a second, I thought you were milady. You really startled me!"
static void McCoyPalace_TalkToWomanInMiladysNightgown(void) {
  Event_Start();
  Actor_ShowEmote(ACTOR_VILLAGER_IN_MILADYS_NIGHTGOWN,
                  EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Actor_RunRepeatedMotion(ACTOR_VILLAGER_IN_MILADYS_NIGHTGOWN, 2);
  Event_Wait(40);
  Event_SetMessageId(MsgBilibinAaaah);
  // "AAAAH!!!"
  Event_ShowMessageThenWait(ACTOR_VILLAGER_IN_MILADYS_NIGHTGOWN, 0, 20);
  Actor_FaceActor(ACTOR_VILLAGER_IN_MILADYS_NIGHTGOWN, ACTOR_PARTY_LEADER, 0);
  Event_Wait(20);
  // "Oh! For a second, I thought you were milady. You really..."
  Event_ShowMessageThenWait(ACTOR_VILLAGER_IN_MILADYS_NIGHTGOWN, 0, 10);
  Actor_FaceDirection(ACTOR_VILLAGER_IN_MILADYS_NIGHTGOWN,
                      FACING_NORTH_NORTHWEST, 10);
  Event_End();
}

/// After the reward, the old man with a cane in the north-east tower room tells
/// Isaac "You are a fine warrior." Spoken to again before the party leaves
/// McCoy's Palace, he worries instead: "It's very reckless for such young
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

/// After the reward, the soldier in the south-east tower room asks "I was
/// watching from here. After all that fuss, you still went to Kolima?" On yes
/// he does not know how they lifted the curse, but they are the best; on no he
/// calls Isaac a liar, for he saw them walk through the barricade.
static void McCoyPalace_TalkToSoldierWatchingKolima(void) {
  Event_Start();
  Event_SetMessageId(MsgBilibinWasWatchingFromHereAfter);
  // "I was watching from here. After all that fuss, you still..."
  // Yes: "I don't know how you removed the curse, but you're the best!"
  // No: "You lie! I saw you walking through the barricade! And then I..."
  Event_AskYesNo(ACTOR_SOLDIER_WATCHING_KOLIMA, 0);
  Event_End();
}

/// The soldier at the treasure room's barred gate shows off what lies behind
/// it: "What do you think? These are Bilibin's great treasures. Whoever ends
/// the curse can claim these as his own." While the party is choosing its
/// reward he says "You may choose only one item, but I trust that all of them
/// will prove useful," and once it has one, that it was indeed the greatest of
/// treasures.
///
/// Unused: a line the game never shows, "Pick any one of the four
/// treasures.", would follow his boast if FLAG_MCCOY_PALACE_UNUSED_PICK_OFFERED
/// were set, and nothing sets it.
static void McCoyPalace_TalkToSoldierShowingTheReward(void) {
  Event_Start();

  if (GameFlag_Get(FLAG_CHOOSING_REWARD) != 0) {
    Event_SetMessageId(MsgBilibinMayChooseOnlyOneItem);
  } else if (GameFlag_Get(FLAG_MCCOY_PALACE_REWARD_TAKEN) != 0) {
    Event_SetMessageId(MsgBilibinRewardReceivedWasIndeedGreatest);
  } else {
    // "What do you think? These are Bilibin's great treasures."
    Event_SetMessageId(MsgBilibinDoThinkTheseBilibinsGreat);
    if (GameFlag_Get(FLAG_MCCOY_PALACE_UNUSED_PICK_OFFERED) != 0) {
      // "Pick any one of the four treasures."
      gFieldWork->message++;
    }
  }

  Event_ShowMessage(ACTOR_SOLDIER_SHOWING_THE_REWARD, 0);
  Event_End();
}

/// After the reward, Mind Read on the old man with a cane in the north-east
/// tower room hears "Sometimes, we need children to remind us of our own
/// responsibilities." Read again before the party leaves McCoy's Palace, he
/// thinks "You looked very courageous walking toward Kolima."
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

/// Mind Read on the soldier at the treasure room's barred gate. Before the
/// curse is lifted he thinks "What good is treasure if you get turned into a
/// tree?" Once it is lifted: "Are we really giving our treasure to these
/// wimps?" While the party chooses: "Nonetheless...if your luck is sour, you
/// may end up an item you do not need." After the reward he thinks they got a
/// pretty nice reward but were just unlucky.
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

  Event_ShowMessage(ACTOR_SOLDIER_SHOWING_THE_REWARD, 0);
  Event_End();
}

/// Walking north into the door at the top of the hall's red carpet (cells
/// (15, 39) and (16, 39)) opens it, and Isaac walks through into McCoy's room.
/// The two doors at the back of Lady McCoy's room, cells (6, 23) and (19, 23),
/// open the same way into the west and east bedrooms.
///
/// McCoy's door opens with `SE_THUMP` and the bedroom doors with
/// `SE_DOOR_OPEN`, the door's opening being the map's tile animations 1 and 2,
/// let run for the walk. After the captain has come out of McCoy's door
/// (McCoyPalace_CaptainTalksAboutBarricade) it stands open, so walking back in
/// makes no sound and plays no opening.
static void McCoyPalace_WalkThroughDoor(void) {
  struct FieldWork *work;
  s32 hideDoorLayers;

  work = gFieldWork;
  hideDoorLayers = 0;
  if (work->stepTrigger == MCCOY_PALACE_ENTRANCE_ROOM_TO_MCCOYS_ROOM) {
    if (GameFlag_Get(FLAG_MCCOYS_DOOR_OPEN) == 0) {
      Sound_Play(SE_THUMP);
      hideDoorLayers = 1;
    }
  } else {
    Sound_Play(SE_DOOR_OPEN);
    hideDoorLayers = 1;
  }
  if (hideDoorLayers != 0) {
    TilesetAnim_Resume(1);
    TilesetAnim_Resume(2);
  }
  Event_Start();
  Event_Wait(10);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.5);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  if (work->stepTrigger == MCCOY_PALACE_ENTRANCE_ROOM_TO_MCCOYS_ROOM)
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -16);
  else
    Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 3, -16);
  Event_Wait(16);
  Import_EventSetExitRequest(work->stepTrigger);
  Event_End();
  TilesetAnim_Pause(1);
  TilesetAnim_Pause(2);
}

/// "Isaac checked the chest... but it was empty."
static void McCoyPalace_ShowChestEmpty(void) {
  Event_Start();
  FieldMessage_Show(MsgBilibinIsaacCheckedChestButWas, 1);
  Event_End();
}

/// "The treasure chest is locked."
static void McCoyPalace_ShowChestLocked(void) {
  Event_Start();
  FieldMessage_Show(MsgBilibinTreasureChestLocked, 1);
  Event_End();
}

/// After the reward, the chest the party opened searches as empty and the other
/// three as locked.
static void McCoyPalace_ShowUnavailableChest(s32 chestFlag) {
  if (GameFlag_Get(chestFlag) != 0) {
    McCoyPalace_ShowChestEmpty();
  } else {
    McCoyPalace_ShowChestLocked();
  }
}

/// Searching the westmost of the four reward chests in the treasure room
/// (cell (38, 45)) gives a Vial, if the party has not yet chosen its reward.
static void McCoyPalace_SearchVialChest(void) {
  if (GameFlag_Get(FLAG_MCCOY_PALACE_REWARD_TAKEN) != 0) {
    McCoyPalace_ShowUnavailableChest(FLAG_TOOK_VIAL);
  } else {
    McCoyPalace_ClaimReward(ACTOR_VIAL_CHEST, ITEM_VIAL, FLAG_TOOK_VIAL);
  }
}

/// Searching the second reward chest from the west in the treasure room (cell
/// (39, 45)) gives a Potion, if the party has not yet chosen its reward.
static void McCoyPalace_SearchPotionChest(void) {
  if (GameFlag_Get(FLAG_MCCOY_PALACE_REWARD_TAKEN) != 0) {
    McCoyPalace_ShowUnavailableChest(FLAG_TOOK_POTION);
  } else {
    McCoyPalace_ClaimReward(ACTOR_POTION_CHEST, ITEM_POTION, FLAG_TOOK_POTION);
  }
}

/// Searching the third reward chest from the west in the treasure room (cell
/// (40, 45)) gives a Psy Crystal, if the party has not yet chosen its reward.
static void McCoyPalace_SearchPsyCrystalChest(void) {
  if (GameFlag_Get(FLAG_MCCOY_PALACE_REWARD_TAKEN) != 0) {
    McCoyPalace_ShowUnavailableChest(FLAG_TOOK_PSY_CRYSTAL);
  } else {
    McCoyPalace_ClaimReward(ACTOR_PSY_CRYSTAL_CHEST, ITEM_PSY_CRYSTAL,
                            FLAG_TOOK_PSY_CRYSTAL);
  }
}

/// Searching the eastmost of the four reward chests in the treasure room (cell
/// (41, 45)) gives a Water of Life, if the party has not yet chosen its reward.
static void McCoyPalace_SearchWaterOfLifeChest(void) {
  if (GameFlag_Get(FLAG_MCCOY_PALACE_REWARD_TAKEN) != 0) {
    McCoyPalace_ShowUnavailableChest(FLAG_TOOK_WATER_OF_LIFE);
  } else {
    McCoyPalace_ClaimReward(ACTOR_WATER_OF_LIFE_CHEST, ITEM_WATER_OF_LIFE,
                            FLAG_TOOK_WATER_OF_LIFE);
  }
}

/// The chest opens and gives its item, and the other three lock. If no one in
/// the party has room for it, the lid peeks open and shuts again, and the
/// party can still choose.
static void McCoyPalace_ClaimReward(s32 chestActorId, s32 itemId,
                                    s32 chestFlag) {
  s32 emitterHandle;

  Event_Start();

  emitterHandle =
      FieldEffect_PlayCueAndStartEmitterOnTarget(0, chestActorId, itemId);

  if (Party_GiveItem(itemId, 0) != -1) {
    Actor_SetAnimation(chestActorId, CHEST_ANIMATION_OPEN_EMPTY);
    GameFlag_Set(FLAG_MCCOY_PALACE_REWARD_TAKEN);
    GameFlag_Set(chestFlag);
    GameFlag_Clear(FLAG_REWARD_OFFERED);
    GameFlag_Clear(FLAG_CHOOSING_REWARD);
  } else {
    Sound_Play(SE_CHEST_CLOSE);
    Actor_SetAnimation(chestActorId, CHEST_ANIMATION_PEEK_AND_SHUT);
  }

  Actor_Release((struct Actor *)emitterHandle);
  Event_End();
}

/// Walking to the foot of the treasure room's stairs (cell (36, 47)) without
/// a reward makes the captain call out "Please take your reward before your
/// depart!", and Isaac walks back toward the chests.
static void McCoyPalace_CaptainAsksPartyToChoose(void) {
  if (GameFlag_Get(FLAG_MCCOY_PALACE_REWARD_TAKEN) == 0) {
    if (GameFlag_Get(FLAG_REWARD_OFFERED) != 0) {
      Event_Start();
      Actor_ShowEmote(ACTOR_CAPTAIN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
      Actor_FaceDirection(ACTOR_CAPTAIN, FACING_WEST_SOUTHWEST, 10);
      Actor_RunRepeatedMotion(ACTOR_CAPTAIN, 2);
      Event_Wait(20);
      Event_SetMessageId(MsgBilibinPleaseTakeYourRewardBefore);
      // "Please take your reward before your depart!"
      Event_ShowMessage(ACTOR_CAPTAIN, 0);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, 1);
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 616, 762);
      Actor_FaceDirection(ACTOR_CAPTAIN, FACING_NORTH_NORTHEAST, 10);
      Event_End();
    }
  }
}

/// Once Isaac has his reward, walking along the row in front of the chests
/// (cells (38, 46) to (41, 46)) makes the captain step up beside the soldier
/// at the gate. Both nod, and the captain tells Isaac "You are always welcome
/// in the palace of Lord McCoy." before walking off up the stairs.
///
/// It can play only in the visit the reward is taken in: the row takes the
/// cells its trigger answers only while the chests stand out before the gate
/// (McCoyPalace_OpenRewardChestRow), and once a reward is taken a later visit
/// does not bring them out again.
static void McCoyPalace_CaptainSeesPartyOff(void) {
  if (GameFlag_Get(FLAG_MCCOY_PALACE_REWARD_TAKEN) != 0) {
    Event_Start();
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_CAPTAIN, 0);
    Actor_SetSpeed(ACTOR_CAPTAIN, 0.6);
    Actor_WalkToAndWait(ACTOR_CAPTAIN, 622, 764);
    Actor_FaceDirection(ACTOR_CAPTAIN, FACING_EAST_NORTHEAST, 20);
    Actor_SetAnimationAndWait(ACTOR_CAPTAIN, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_SOLDIER_SHOWING_THE_REWARD, ANIM_NOD);
    Event_Wait(20);
    Actor_FaceActor(ACTOR_CAPTAIN, ACTOR_PARTY_LEADER, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_CAPTAIN, ANIM_NOD);
    Event_SetMessageId(MsgBilibinAlwaysWelcomeInPalaceLord);
    // "You are always welcome in the palace of Lord McCoy."
    Event_ShowMessageThenWait(ACTOR_CAPTAIN, 0, 10);
    Actor_SetSpeed(ACTOR_CAPTAIN, 0.8);
    Actor_WalkToAndWait(ACTOR_CAPTAIN, 570, 758);
    Actor_SetPosition(ACTOR_CAPTAIN, 0, 0);
    gFieldWork->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
    GameFlag_Set(FLAG_MCCOY_PALACE_PARTY_REWARDED);
    GameFlag_Set(FLAG_PARTY_SEEN_OFF);
    Event_End();
  }
}

/// The four reward chests stand in a row before the barred gate, from west to
/// east the Vial, the Potion, the Psy Crystal and the Water of Life.
static inline void McCoyPalace_PositionRewardChests(void) {
  Actor_SetPosition(ACTOR_VIAL_CHEST, 616, 728);
  Actor_SetPosition(ACTOR_POTION_CHEST, 632, 728);
  Actor_SetPosition(ACTOR_PSY_CRYSTAL_CHEST, 648, 728);
  Actor_SetPosition(ACTOR_WATER_OF_LIFE_CHEST, 664, 728);
}

/// The chests' cells and the row in front of them take the behaviors the
/// triggers answer, copied from the spare cells at (38, 55) to (45, 55).
static inline void McCoyPalace_OpenRewardChestRow(void) {
  MapGrid_CopyCellAttributes(38, 55, 4, 1, 38, 45);
  MapGrid_CopyCellAttributes(42, 55, 4, 1, 38, 46);
}

/// Sets McCoy's Palace up and plays the story's scenes: the first audience on
/// entering McCoy's room, the captain's hint outside his door after it, the
/// reward once the curse is lifted, and the chests in the treasure room. A
/// return from a battle while the party is choosing puts the
/// chests straight back in their row.
s32 Scene_Initialize(void) {
  s32 rewardTaken;
  s32 curseLifted;

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
    if (gMainRoutineWork.entrance ==
        MCCOY_PALACE_FROM_MCCOY_PALACE_GROUNDS_EAST) {
      McCoyPalace_AudienceWithMcCoy();
    } else if (gMainRoutineWork.entrance ==
                   MCCOY_PALACE_ENTRANCE_ROOM_FROM_MCCOYS_ROOM &&
               GameFlag_Get(FLAG_CAPTAIN_SEEING_PARTY_OUT) != 0) {
      McCoyPalace_CaptainTalksAboutBarricade();
    }
  } else {
    rewardTaken = GameFlag_Get(FLAG_MCCOY_PALACE_REWARD_TAKEN);
    if (rewardTaken == 0) {
      if (gMainRoutineWork.entrance ==
          MCCOY_PALACE_FROM_MCCOY_PALACE_GROUNDS_EAST) {
        if (GameFlag_Get(FLAG_MCCOY_PALACE_PARTY_REWARDED) == 0) {
          curseLifted = GameFlag_Get(FLAG_KOLIMA_CURSE_LIFTED);
          if (curseLifted != 0) {
            McCoyPalace_McCoyRewardsParty();
          }
        }
      } else {
        if (gMainRoutineWork.entrance ==
            MCCOY_PALACE_TREASURE_ROOM_FROM_ENTRANCE_ROOM) {
          if (GameFlag_Get(FLAG_REWARD_OFFERED) != 0) {
            if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) != 0) {
              McCoyPalace_OpenRewardChestRow();
              McCoyPalace_PositionRewardChests();
              Actor_SetSpriteFlags(Actor_Get(ACTOR_VIAL_CHEST), 0);
              Actor_SetSpriteFlags(Actor_Get(ACTOR_POTION_CHEST), 0);
              Actor_SetSpriteFlags(Actor_Get(ACTOR_PSY_CRYSTAL_CHEST), 0);
              Actor_SetSpriteFlags(Actor_Get(ACTOR_WATER_OF_LIFE_CHEST), 0);
              Actor_Get(ACTOR_VIAL_CHEST)->motionFlags = rewardTaken;
              Actor_Get(ACTOR_POTION_CHEST)->motionFlags = rewardTaken;
              Actor_Get(ACTOR_PSY_CRYSTAL_CHEST)->motionFlags = rewardTaken;
              Actor_Get(ACTOR_WATER_OF_LIFE_CHEST)->motionFlags = rewardTaken;
              Actor_Get(ACTOR_VIAL_CHEST)->y.fixed = -toFIXED(4);
              Actor_Get(ACTOR_POTION_CHEST)->y.fixed = -toFIXED(4);
              Actor_Get(ACTOR_PSY_CRYSTAL_CHEST)->y.fixed = -toFIXED(4);
              Actor_Get(ACTOR_WATER_OF_LIFE_CHEST)->y.fixed = -toFIXED(4);
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
static inline void McCoyPalace_PrepareCompanionApproach(void) {
  Actor_SetSpeed(ACTOR_GARET, 0.6);
  Actor_SetSpeed(ACTOR_IVAN, 0.6);
  Actor_SetSpeed(ACTOR_MIA, 1);
  Actor_SetAnimation(ACTOR_GARET, ANIM_WALK);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_WALK);
  Actor_SetAnimation(ACTOR_MIA, ANIM_WALK);
}

/// The first audience, on walking through McCoy's door before Kolima's curse
/// is lifted: the captain announces the party, McCoy calls Isaac closer and
/// asks whether they will go to Kolima Forest. On no he sends them away as
/// too young, and a return lets Isaac change his mind. On yes he tosses Isaac
/// the key to the barricades, then, seeing how young the party is, takes it
/// back and has his captain see them out.
static void McCoyPalace_AudienceWithMcCoy(void) {
  struct Actor *actor;

  Event_Start();
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  wait(1);
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_MoveTo(894, CAMERA_KEEP, 664, 0);
  wait(1);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  if (Event_GetFlag(FLAG_MCCOY_PALACE_JOB_REFUSED) != 0) {
    FieldCamera_MoveTo(894, CAMERA_KEEP, 698, 0);
    Actor_SetPosition(ACTOR_CAPTAIN, 876, 634);
    Actor_FaceDirection(ACTOR_CAPTAIN, FACING_SOUTH_SOUTHEAST, 0);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 894, 798);
  }
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  gFieldWork->transitionFrames = 40;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  if (GameFlag_Get(FLAG_MCCOY_PALACE_JOB_REFUSED) == 0) {
    Event_Wait(80);
    Actor_SetPosition(ACTOR_CAPTAIN, 894, 798);
    FieldCamera_SetSpeed(0.6);
    FieldCamera_MoveTo(894, CAMERA_KEEP, 698, 1);
    Actor_SetSpeed(ACTOR_CAPTAIN, 0.8);
    Actor_WalkTo(ACTOR_CAPTAIN, 894, 696);
    Event_Wait(80);
    FieldCamera_MoveTo(894, CAMERA_KEEP, 664, 1);
    Actor_WaitForMoveById(ACTOR_CAPTAIN);
    Actor_WalkToAndWait(ACTOR_CAPTAIN, 842, 696);
    Actor_WalkToAndWait(ACTOR_CAPTAIN, 842, 636);
    Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_WEST_SOUTHWEST, 20);
    Actor_WalkToAndWait(ACTOR_CAPTAIN, 876, 634);
    Actor_SetAnimationAndWait(ACTOR_CAPTAIN, ANIM_NOD);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_LORD_MCCOY, ANIM_NOD);
    Event_Wait(10);
    // The captain announces the warriors before they enter the audience.
    Event_SetMessageId(MsgBilibinMatter);
    // "What's the matter, then?"
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0,
                                   10);
    Actor_RunRepeatedMotion(ACTOR_CAPTAIN, 2);
    // "Sir! Some new warriors have just arrived in the palace!"
    Event_ShowMessageThenWait(ACTOR_CAPTAIN, 0, 20);
    Actor_RunRepeatedMotion(ACTOR_LORD_MCCOY, 1);
    // "New warriors, yeh say?"
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0,
                                   10);
    Actor_SetAnimationAndWait(ACTOR_CAPTAIN, ANIM_NOD);
    Event_Wait(40);
    Actor_ShowEmote(ACTOR_LORD_MCCOY, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
    // "We've sent many a man intae Kolima Forest, but not a one has..."
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0);
    Actor_RunRepeatedMotion(ACTOR_LORD_MCCOY, 1);
    // "These warriors... Did yeh get a good look at 'em?"
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0,
                                   10);
    Actor_SetAttachedEffect(ACTOR_CAPTAIN,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Event_Wait(60);
    Actor_FaceDirection(ACTOR_CAPTAIN, FACING_SOUTH_SOUTHEAST, 10);
    Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTH_SOUTHWEST, 10);
    FieldCamera_MoveTo(894, CAMERA_KEEP, 698, 1);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 894, 798);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.6);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 894, 726);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_LORD_MCCOY, 1);
    // "Come closer, laddy."
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0);
    FieldCamera_MoveTo(894, CAMERA_KEEP, 664, 1);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 894, 684);
    actor = Actor_Get(ACTOR_PARTY_LEADER);
    if (actor != NULL) {
      Actor_SetPositionFX(ACTOR_GARET, actor->x.fixed, actor->z.fixed);
    }
    actor = Actor_Get(ACTOR_PARTY_LEADER);
    if (actor != NULL) {
      Actor_SetPositionFX(ACTOR_IVAN, actor->x.fixed, actor->z.fixed);
    }
    if (GameFlag_Get(FLAG_IN_PARTY(MIA)) != 0) {
      actor = Actor_Get(ACTOR_PARTY_LEADER);
      if (actor != NULL) {
        Actor_SetPositionFX(ACTOR_MIA, actor->x.fixed, actor->z.fixed);
      }
    }
    McCoyPalace_PrepareCompanionApproach();
    Actor_SetDestinationOffset(ACTOR_GARET, -16, 16);
    Actor_SetDestinationOffset(ACTOR_IVAN, 16, 16);
    if (GameFlag_Get(FLAG_IN_PARTY(MIA)) != 0) {
      Actor_SetDestinationOffset(ACTOR_MIA, 32, 16);
    }
    Actor_WaitForMoveById(ACTOR_IVAN);
    Actor_SetAnimation(ACTOR_GARET, ANIM_STAND);
    Actor_SetAnimation(ACTOR_IVAN, ANIM_STAND);
    Actor_SetAnimation(ACTOR_MIA, ANIM_STAND);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 20);
    Actor_Jump(ACTOR_LORD_MCCOY, 2, 20);
    Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_WEST_SOUTHWEST, 10);
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0,
                                   10);
    Actor_FaceDirection(ACTOR_CAPTAIN, FACING_EAST_SOUTHEAST, 10);
    Actor_SetAnimationAndWait(ACTOR_CAPTAIN, ANIM_NOD);
    Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTH_SOUTHWEST, 40);
    Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_WEST_SOUTHWEST, 10);
    Actor_SetAnimationAndWait(ACTOR_LORD_MCCOY, ANIM_SHAKE_HEAD);
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0,
                                   10);
    Actor_SetAttachedEffect(ACTOR_CAPTAIN,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Event_Wait(40);
    Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTH_SOUTHWEST, 20);
    Actor_ShowEmote(ACTOR_LORD_MCCOY, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
    Event_OpenMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0);
    Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
    if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
      // The initial offer and reconsideration share their accept/refuse paths.
      goto accepted;
    }
  declined:
    Event_SetMessageId(MsgBilibinTooYoungForTheJob);
    // "Bah! Yeh're too young. Yeh cannae handle the job."
    FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0,
                                   10);
    Actor_FaceDirection(ACTOR_CAPTAIN, FACING_SOUTH_SOUTHEAST, 0);
    Actor_SetAnimationAndWait(ACTOR_LORD_MCCOY, ANIM_SHAKE_HEAD);
    // "Yeh can leave now..."
    Event_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
    Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Actor_SetAnimation(ACTOR_GARET, ANIM_WALK);
    actor = Actor_Get(ACTOR_PARTY_LEADER);
    if (actor != NULL) {
      Actor_SetDestination(ACTOR_GARET, actor->x.part[0].integer,
                           actor->z.part[0].integer);
    }
    Actor_SetAnimation(ACTOR_IVAN, ANIM_WALK);
    actor = Actor_Get(ACTOR_PARTY_LEADER);
    if (actor != NULL) {
      Actor_SetDestination(ACTOR_IVAN, actor->x.part[0].integer,
                           actor->z.part[0].integer);
    }
    if (GameFlag_Get(FLAG_IN_PARTY(MIA)) != 0) {
      Actor_SetAnimation(ACTOR_MIA, ANIM_WALK);
      actor = Actor_Get(ACTOR_PARTY_LEADER);
      if (actor != NULL) {
        Actor_SetDestination(ACTOR_MIA, actor->x.part[0].integer,
                             actor->z.part[0].integer);
      }
    }
    Actor_WaitForMoveById(ACTOR_IVAN);
    Actor_SetPosition(ACTOR_GARET, 0, 0);
    Actor_SetPosition(ACTOR_IVAN, 0, 0);
    Actor_SetPosition(ACTOR_MIA, 0, 0);
    GameFlag_Set(FLAG_MCCOY_PALACE_JOB_REFUSED);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 1);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 894, 752);
    gFieldWork->transitionFrames = MAP_TRANSITION_FRAMES;
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    goto leave;
  }
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.6);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 894, 684);
  Event_Wait(80);
  FieldCamera_SetSpeed(0.6);
  FieldCamera_MoveTo(894, CAMERA_KEEP, 664, 1);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetPositionFX(ACTOR_GARET, actor->x.fixed, actor->z.fixed);
  }
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetPositionFX(ACTOR_IVAN, actor->x.fixed, actor->z.fixed);
  }
  if (GameFlag_Get(FLAG_IN_PARTY(MIA)) != 0) {
    actor = Actor_Get(ACTOR_PARTY_LEADER);
    if (actor != NULL) {
      Actor_SetPositionFX(ACTOR_MIA, actor->x.fixed, actor->z.fixed);
    }
  }
  McCoyPalace_PrepareCompanionApproach();
  Actor_SetDestinationOffset(ACTOR_GARET, -16, 16);
  Actor_SetDestinationOffset(ACTOR_IVAN, 16, 16);
  if (GameFlag_Get(FLAG_IN_PARTY(MIA)) != 0) {
    Actor_SetDestinationOffset(ACTOR_MIA, 32, 16);
  }
  Actor_WaitForMoveById(ACTOR_IVAN);
  Actor_SetAnimation(ACTOR_GARET, ANIM_STAND);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_STAND);
  Actor_SetAnimation(ACTOR_MIA, ANIM_STAND);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 20);
  Actor_ShowEmote(ACTOR_LORD_MCCOY, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  // A returning party can reconsider the assignment without replaying the
  // captain's announcement.
  Event_SetMessageId(MsgBilibinYehveChangeHeart);
  // "What is it? Yeh've had a change of heart? Yeh want tae help..."
  Event_OpenMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 1) {
    goto declined;
  }
accepted:
  // Both ways of accepting join the same explanation and departure.
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 20);
  Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Actor_ShowEmote(ACTOR_LORD_MCCOY, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Event_SetMessageId(MsgBilibinHmmmWellGrant);
  // "Hmmm. Well, I'll grant yeh're a brave lot."
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_BARRICADE_KEY), 0);
  actor = Actor_Get(ACTOR_BARRICADE_KEY);
  actor->scaleX = FIXED_ONE / 2;
  actor->scaleY = FIXED_ONE / 2;
  actor = Actor_Get(ACTOR_LORD_MCCOY);
  if (actor != NULL) {
    Actor_SetPositionFX(ACTOR_BARRICADE_KEY, actor->x.fixed, actor->z.fixed);
  }
  wait(1);
  Actor_Jump(ACTOR_BARRICADE_KEY, 6, 0);
  Actor_SetSpeed(ACTOR_BARRICADE_KEY, 2);
  Actor_MoveToAndWait(ACTOR_BARRICADE_KEY, 894, 668);
  Event_Wait(40);
  // "Here. This key'll get yeh past the barricades tae the east..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0,
                                 10);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Actor_SetAnimationAndWait(ACTOR_LORD_MCCOY, ANIM_SHAKE_HEAD);
  // "Even with the key, such wee lads as yerselves'll never reach..."
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 60);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 10);
  // "He's making fun of us! Calling us children! That's just..."
  Event_OpenMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 1) {
    do {
      Actor_StartRepeatedMotion(ACTOR_GARET, 2);
      Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
      Event_SetMessageId(MsgBilibinSeriousDoesntBother);
      // "Are you serious, Isaac? It doesn't bother you?"
      Event_OpenMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0);
    } while (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) != 1);
  }
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  // Resolve Garet\'s objections before McCoy dismisses the party.
  Event_SetMessageId(MsgBilibinWellDontLet);
  // "Well, don't let him get to you. We'll just have to prove we..."
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0, 10);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 10);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_LORD_MCCOY, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  // "So, yeh've decided to accept our little job?"
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 20);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTH_SOUTHEAST, 10);
  Actor_ShowEmote(ACTOR_LORD_MCCOY, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Actor_FaceDirection(ACTOR_GARET, FACING_EAST, 20);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  // "Hey, Ivan! You... You don't look too enthusiastic."
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0, 10);
  Actor_FaceDirection(ACTOR_MIA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 20);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 10);
  // "That curse can turn a man into a tree! You think we stand a..."
  Event_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_IVAN, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_GARET, 1);
  Event_Wait(40);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  // "What? Yeah, of course we do!"
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0, 10);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 10);
  Actor_StartRepeatedMotion(ACTOR_GARET, 1);
  // "You know we do, right, Isaac?"
  Event_OpenMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  while (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) != 0) {
    Event_SetMessageId(MsgBilibinComeSaidYoud);
    // "Come on, Isaac! You said you'd do it! We're still going, right?"
    Event_OpenMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0);
  }
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_MIA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_EAST, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 10);
  Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 10);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
  Event_SetMessageId(MsgBilibinWellSGoing);
  // "Well, if Isaac's going, I guess I'm in, too."
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_IVAN, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_LORD_MCCOY, 1);
  Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTH_SOUTHWEST, 10);
  Actor_SetAnimationAndWait(ACTOR_LORD_MCCOY, ANIM_SHAKE_HEAD);
  // "At least we all agree on something!"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0,
                                 10);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 20);
  Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_Wait(40);
  Actor_ShowEmote(ACTOR_LORD_MCCOY, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
  Actor_FaceDirection(ACTOR_CAPTAIN, FACING_EAST_SOUTHEAST, 10);
  Actor_SetAttachedEffect(ACTOR_CAPTAIN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  // "What troubles you, Milord? They seem willing enough..."
  Event_ShowMessageThenWait(ACTOR_CAPTAIN, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_LORD_MCCOY, 1);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_WEST_SOUTHWEST, 20);
  // "They call themselves warriors...but they're naught but wee..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0,
                                 10);
  Actor_SetAnimationAndWait(ACTOR_CAPTAIN, ANIM_NOD);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_LORD_MCCOY, ANIM_SHAKE_HEAD);
  Actor_SetAnimation(ACTOR_LORD_MCCOY, ANIM_SHAKE_HEAD);
  // "Aye, these are desperate times, but I cannae send such wee..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0,
                                 10);
  Actor_Jump(ACTOR_BARRICADE_KEY, 6, 0);
  actor = Actor_Get(ACTOR_LORD_MCCOY);
  if (actor != NULL) {
    Actor_SetDestination(ACTOR_BARRICADE_KEY, actor->x.part[0].integer,
                         actor->z.part[0].integer);
  }
  Actor_WaitForMoveById(ACTOR_BARRICADE_KEY);
  Actor_SetPosition(ACTOR_BARRICADE_KEY, 0, 0);
  Event_Wait(20);
  Actor_SetAttachedEffect(ACTOR_MIA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_GARET,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_IVAN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  Actor_RunRepeatedMotion(ACTOR_CAPTAIN, 2);
  // "Then what will happen to Kolima Forest?"
  Event_ShowMessageThenWait(ACTOR_CAPTAIN, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_LORD_MCCOY, ANIM_NOD);
  // "We cannae give up hope on our other champions!"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0,
                                 20);
  Actor_SetAnimationAndWait(ACTOR_CAPTAIN, ANIM_NOD);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  Event_Wait(20);
  // "But... But what about us?"
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_IVAN, 0, 10);
  Actor_FaceDirection(ACTOR_CAPTAIN, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTH_SOUTHEAST, 10);
  Actor_SetAnimationAndWait(ACTOR_LORD_MCCOY, ANIM_SHAKE_HEAD);
  // "We cannae send yeh intae certain doom."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0,
                                 10);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 60);
  // "What if we said that we were going east anyway?"
  Event_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0, 10);
  Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTH_SOUTHWEST, 10);
  Actor_SetAnimation(ACTOR_LORD_MCCOY, ANIM_SHAKE_HEAD);
  // "Yeh can go once things have settled down."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0,
                                 10);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_SHAKE_HEAD);
  // "But that means... you're not going to let us go?"
  Event_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_IVAN, 0, 10);
  Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTH_SOUTHEAST, 10);
  Actor_SetAnimationAndWait(ACTOR_LORD_MCCOY, ANIM_NOD);
  // "Ach, it's fer yer own good, lad."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0,
                                 10);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_SCOWL, 60);
  Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_WEST_SOUTHWEST, 10);
  Actor_SetAnimationAndWait(ACTOR_LORD_MCCOY, ANIM_NOD);
  Event_Wait(10);
  // "I dinnae want tae talk about it anymore. Captain?"
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_CAPTAIN, 2);
  Actor_FaceDirection(ACTOR_CAPTAIN, FACING_EAST_SOUTHEAST, 10);
  Actor_SetAnimationAndWait(ACTOR_CAPTAIN, ANIM_NOD);
  Event_Wait(20);
  FieldActor_WatchAndStartScript(ACTOR_PARTY_LEADER,
                                 ACTOR_WATCH_KEEP_MOTION | ACTOR_CAPTAIN,
                                 gPalace_FacingActions);
  FieldActor_WatchAndStartScript(ACTOR_GARET,
                                 ACTOR_WATCH_KEEP_MOTION | ACTOR_CAPTAIN,
                                 gPalace_FacingActions);
  FieldActor_WatchAndStartScript(ACTOR_IVAN,
                                 ACTOR_WATCH_KEEP_MOTION | ACTOR_CAPTAIN,
                                 gPalace_FacingActions);
  FieldActor_WatchAndStartScript(ACTOR_MIA,
                                 ACTOR_WATCH_KEEP_MOTION | ACTOR_CAPTAIN,
                                 gPalace_FacingActions);
  Actor_SetSpeed(ACTOR_CAPTAIN, 0.6);
  Actor_WalkToAndWait(ACTOR_CAPTAIN, 852, 646);
  Actor_WalkToAndWait(ACTOR_CAPTAIN, 852, 666);
  Actor_WalkToAndWait(ACTOR_CAPTAIN, 864, 672);
  Actor_FaceDirection(ACTOR_CAPTAIN, FACING_EAST_SOUTHEAST, 10);
  // "I'm sorry, but you should leave now."
  Event_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_CAPTAIN, 0, 20);
  Actor_Stop(ACTOR_PARTY_LEADER);
  Actor_Stop(ACTOR_GARET);
  Actor_Stop(ACTOR_IVAN);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  FieldActor_WatchAndStartScript(ACTOR_CAPTAIN,
                                 ACTOR_WATCH_KEEP_MOTION | ACTOR_PARTY_LEADER,
                                 gPalace_FacingActions);
  Actor_SetAnimation(ACTOR_GARET, ANIM_WALK);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetDestination(ACTOR_GARET, actor->x.part[0].integer,
                         actor->z.part[0].integer);
  }
  Actor_SetAnimation(ACTOR_IVAN, ANIM_WALK);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetDestination(ACTOR_IVAN, actor->x.part[0].integer,
                         actor->z.part[0].integer);
  }
  if (GameFlag_Get(FLAG_IN_PARTY(MIA)) != 0) {
    Actor_SetAnimation(ACTOR_MIA, ANIM_WALK);
    actor = Actor_Get(ACTOR_PARTY_LEADER);
    if (actor != NULL) {
      Actor_SetDestination(ACTOR_MIA, actor->x.part[0].integer,
                           actor->z.part[0].integer);
    }
  }
  Actor_WaitForMoveById(ACTOR_IVAN);
  Actor_SetPosition(ACTOR_GARET, 0, 0);
  Actor_SetPosition(ACTOR_IVAN, 0, 0);
  Actor_SetPosition(ACTOR_MIA, 0, 0);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 1);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 894, 752);
  gFieldWork->transitionFrames = MAP_TRANSITION_FRAMES;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  GameFlag_Set(FLAG_CAPTAIN_SEEING_PARTY_OUT);
leave:
  Import_EventSetExitRequest(MCCOY_PALACE_MCCOYS_ROOM_TO_ENTRANCE_ROOM);
  Event_End();
}

/// Back in the hall after the first audience, McCoy's door opens behind the
/// party and the captain comes out. He is sorry Isaac was rejected, asks him to
/// promise not to go to Kolima, lets slip "That barricade... You may not need
/// a key.", and leaves down the red carpet.
static void McCoyPalace_CaptainTalksAboutBarricade(void) {
  Event_Start();
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_FaceDirection(ACTOR_CAPTAIN, FACING_SOUTH_SOUTHEAST, 0);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.6);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 256, 660);
  Event_Wait(20);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  GameFlag_Set(FLAG_MCCOYS_DOOR_OPEN);
  Sound_Play(SE_THUMP);
  TilesetAnim_Resume(1);
  TilesetAnim_Resume(2);
  Actor_SetPosition(ACTOR_CAPTAIN, 256, 632);
  wait(1);
  Actor_SetSpeed(ACTOR_CAPTAIN, 0.6);
  Actor_WalkToAndWait(ACTOR_CAPTAIN, 256, 644);
  TilesetAnim_Pause(1);
  TilesetAnim_Pause(2);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_CAPTAIN, 2);
  Event_SetMessageId(MsgBilibinNameSorryRejected);
  // "Your name was...Isaac? I'm sorry you were rejected."
  Event_ShowMessageThenWait(ACTOR_CAPTAIN, 0, 10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 264, 660);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_WalkToAndWait(ACTOR_CAPTAIN, 248, 660);
  Actor_FaceDirection(ACTOR_CAPTAIN, FACING_EAST_SOUTHEAST, 40);
  Actor_SetAnimationAndWait(ACTOR_CAPTAIN, ANIM_SHAKE_HEAD);
  // "I never, ever, expected anything like this to happen to Bilibin."
  Event_ShowMessage(ACTOR_CAPTAIN, 0);
  Actor_SetAnimationAndWait(ACTOR_CAPTAIN, ANIM_NOD);
  // "Will you promise me you won't go to Kolima?"
  // Yes: "That's good."
  // No: "You are persistent, aren't you?"
  Event_AskYesNo(ACTOR_CAPTAIN, 0);
  Actor_RunRepeatedMotion(ACTOR_CAPTAIN, 2);
  // "That barricade... You may not need a key."
  Event_ShowMessageThenWait(ACTOR_CAPTAIN, 0, 10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  Actor_SetAttachedEffect(ACTOR_CAPTAIN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(60);
  Actor_RunRepeatedMotion(ACTOR_CAPTAIN, 1);
  // "Oops... I shouldn't have said anything."
  Event_ShowMessageThenWait(ACTOR_CAPTAIN, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_CAPTAIN, ANIM_NOD);
  // "Well, I'm going to go now."
  Event_ShowMessage(ACTOR_CAPTAIN, 0);
  Actor_SetSpeed(ACTOR_CAPTAIN, 0.8);
  Actor_WalkToAndWait(ACTOR_CAPTAIN, 248, 772);
  Actor_SetPosition(ACTOR_CAPTAIN, 0, 0);
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

/// Once Kolima's curse is lifted, the guards march the party into McCoy's room
/// as if under arrest. McCoy thanks Isaac, explains he only feared they would
/// not stop by on the way back, and has the captain take them down to the
/// treasure room to choose a reward.
static void McCoyPalace_McCoyRewardsParty(void) {
  struct Actor *viewCenter;
  struct Actor *actor;
  s32 advanceAlternateReply;

  Event_Start();
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  wait(1);
  viewCenter = FieldCamera_GetActor();
  advanceAlternateReply = 0;
  viewCenter->motionFlags = advanceAlternateReply;
  FieldCamera_MoveTo(894, CAMERA_KEEP, 664, 0);
  wait(1);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  MapGrid_CenterOnFollowedPoint();
  wait(1);

  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  gFieldWork->transitionFrames = MAP_TRANSITION_FRAMES;

  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(40);
  Actor_SetPosition(ACTOR_CAPTAIN, 888, 798);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 904, 798);
  FieldCamera_SetSpeed(0.6);
  FieldCamera_MoveTo(894, CAMERA_KEEP, 698, 1);
  Actor_SetSpeed(ACTOR_CAPTAIN, 0.8);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.6);
  Actor_WalkTo(ACTOR_CAPTAIN, 888, 720);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 904, 736);
  Event_Wait(60);
  Actor_WaitForMoveById(ACTOR_CAPTAIN);
  Actor_SetAnimation(ACTOR_CAPTAIN, ANIM_STAND);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_CAPTAIN, 2);
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

  FieldCamera_MoveTo(894, CAMERA_KEEP, 664, 1);
  Actor_SetScript(ACTOR_CAPTAIN, gPalace_PacingActions);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 894, 684);

  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetPositionFX(ACTOR_GARET, actor->x.fixed, actor->z.fixed);
  }
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetPositionFX(ACTOR_IVAN, actor->x.fixed, actor->z.fixed);
  }
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetPositionFX(ACTOR_MIA, actor->x.fixed, actor->z.fixed);
  }

  McCoyPalace_PrepareCompanionApproach();
  Actor_SetDestinationOffset(ACTOR_GARET, -16, 16);
  Actor_SetDestinationOffset(ACTOR_IVAN, 16, 16);
  Actor_SetDestinationOffset(ACTOR_MIA, 32, 16);
  Actor_WaitForMoveById(ACTOR_IVAN);
  Actor_SetAnimation(ACTOR_GARET, ANIM_STAND);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_STAND);
  Actor_SetAnimation(ACTOR_MIA, ANIM_STAND);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
  Actor_WaitForMoveById(ACTOR_MIA);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHWEST, 0);
  Actor_WaitForScriptEndById(ACTOR_CAPTAIN);
  Event_Wait(20);

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
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
  if (advanceAlternateReply != 0) {
    gFieldWork->message++;
  }

  McCoyPalace_FaceCompanionsForDiscussion();
  Event_Wait(20);
  if (GameFlag_Get(FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY) != 0) {
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  } else {
    Event_Wait(40);
  }

  Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 10);
  Event_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0, 10);
  Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH, 10);
  Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_IVAN, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTHWEST, 10);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_MIA, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_LORD_MCCOY, ANIM_NOD);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0,
                                 20);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);

  if (GameFlag_Get(FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY) != 0) {
    Actor_RunRepeatedMotion(ACTOR_LORD_MCCOY, 1);
    Actor_SetAnimationAndWait(ACTOR_LORD_MCCOY, ANIM_SHAKE_HEAD);
    Event_OpenMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0);
    McCoyPalace_FaceCompanionsForDiscussion();
    advanceAlternateReply = 1;
    if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) != 0) {
      gFieldWork->message++;
      advanceAlternateReply = 0;
    }
    Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTH_SOUTHWEST, 0);
    McCoyPalace_FaceCompanionsForAudience();
    Event_Wait(10);
    Event_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
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
  // "I was but worried yeh might nae stop here on yer way back."
  Event_ShowMessageThenWait(ACTOR_CAPTAIN, 0, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_MIA, FACING_WEST, 40);
  Actor_RunRepeatedMotion(ACTOR_LORD_MCCOY, 2);
  // "I want tae show my appreciation for yer help..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0,
                                 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  McCoyPalace_FaceCompanionsForAudience();
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_LORD_MCCOY, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0,
                                 10);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_LORD_MCCOY, ANIM_NOD);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0,
                                 10);
  Actor_ShowEmote(ACTOR_LORD_MCCOY, EMOTE_IN_FRONT | EMOTE_SMILE, 60);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0,
                                 10);
  Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTH_SOUTHEAST, 10);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0,
                                 10);
  Actor_SetAnimationAndWait(ACTOR_LORD_MCCOY, ANIM_NOD);
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

  Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_WEST_SOUTHWEST, 10);
  Actor_RunRepeatedMotion(ACTOR_CAPTAIN, 1);
  Actor_FaceDirection(ACTOR_CAPTAIN, FACING_EAST_SOUTHEAST, 20);
  Actor_SetAnimationAndWait(ACTOR_LORD_MCCOY, ANIM_NOD);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_CAPTAIN, ANIM_NOD);
  Event_ShowMessageThenWait(ACTOR_CAPTAIN, 0, 10);
  Actor_FaceDirection(ACTOR_CAPTAIN, FACING_SOUTH_SOUTHEAST, 10);
  Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTH_SOUTHEAST, 20);
  Actor_RunRepeatedMotion(ACTOR_LORD_MCCOY, 1);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0,
                                 10);
  Actor_SetAnimationAndWait(ACTOR_LORD_MCCOY, ANIM_NOD);
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_Wait(20);

  Actor_SetAnimation(ACTOR_GARET, ANIM_WALK);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetDestination(ACTOR_GARET, actor->x.part[0].integer,
                         actor->z.part[0].integer);
  }
  Actor_SetAnimation(ACTOR_IVAN, ANIM_WALK);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetDestination(ACTOR_IVAN, actor->x.part[0].integer,
                         actor->z.part[0].integer);
  }
  Actor_SetAnimation(ACTOR_MIA, ANIM_WALK);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetDestination(ACTOR_MIA, actor->x.part[0].integer,
                         actor->z.part[0].integer);
  }

  Actor_WaitForMoveById(ACTOR_GARET);
  Actor_SetPosition(ACTOR_GARET, 0, 0);
  Actor_SetPosition(ACTOR_IVAN, 0, 0);
  Actor_WaitForMoveById(ACTOR_MIA);
  Actor_SetPosition(ACTOR_MIA, 0, 0);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTH_SOUTHWEST, 0);
  FieldActor_WatchAndStartScript(ACTOR_PARTY_LEADER,
                                 ACTOR_WATCH_KEEP_MOTION | ACTOR_CAPTAIN,
                                 gPalace_FacingActions);
  Actor_WalkToAndWait(ACTOR_CAPTAIN, 852, 646);
  Actor_WalkToAndWait(ACTOR_CAPTAIN, 852, 666);
  Actor_WalkToAndWait(ACTOR_CAPTAIN, 864, 672);
  Actor_FaceDirection(ACTOR_CAPTAIN, FACING_EAST_SOUTHEAST, 10);
  Actor_RunRepeatedMotion(ACTOR_CAPTAIN, 1);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_CAPTAIN, 0, 10);
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

/// In the treasure room, the captain walks the party up to four chests in a
/// row before the barred gate. He tells the soldier at the gate that Lord
/// McCoy ordered Isaac and his companions brought here, and the soldier says
/// "You may select a single item from among these four chests."
static void McCoyPalace_PresentRewardChests(void) {
  Event_Start();
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_MoveTo(628, CAMERA_KEEP, 748, 0);
  McCoyPalace_OpenRewardChestRow();
  Actor_Get(ACTOR_PARTY_LEADER)->facing = FACING_EAST;
  Actor_SetPosition(ACTOR_PARTY_LEADER, 577, 760);
  Actor_Get(ACTOR_CAPTAIN)->facing = FACING_EAST;
  Actor_SetPosition(ACTOR_CAPTAIN, 592, 760);
  Actor_Get(ACTOR_SOLDIER_SHOWING_THE_REWARD)->facing = FACING_WEST_NORTHWEST;
  Actor_SetPosition(ACTOR_SOLDIER_SHOWING_THE_REWARD, 662, 764);
  McCoyPalace_PositionRewardChests();
  Actor_SetSpriteFlags(Actor_Get(ACTOR_VIAL_CHEST), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_POTION_CHEST), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_PSY_CRYSTAL_CHEST), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_WATER_OF_LIFE_CHEST), 0);
  Actor_Get(ACTOR_VIAL_CHEST)->motionFlags = 0;
  Actor_Get(ACTOR_POTION_CHEST)->motionFlags = 0;
  Actor_Get(ACTOR_PSY_CRYSTAL_CHEST)->motionFlags = 0;
  Actor_Get(ACTOR_WATER_OF_LIFE_CHEST)->motionFlags = 0;
  Actor_Get(ACTOR_VIAL_CHEST)->y.fixed = -toFIXED(4);
  Actor_Get(ACTOR_POTION_CHEST)->y.fixed = -toFIXED(4);
  Actor_Get(ACTOR_PSY_CRYSTAL_CHEST)->y.fixed = -toFIXED(4);
  Actor_Get(ACTOR_WATER_OF_LIFE_CHEST)->y.fixed = -toFIXED(4);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  gFieldWork->transitionFrames = MAP_TRANSITION_FRAMES;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_SetSpeed(ACTOR_CAPTAIN, 0.6);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.6);
  Actor_WalkTo(ACTOR_CAPTAIN, 628, 764);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 612, 764);
  Actor_SetAnimation(ACTOR_CAPTAIN, ANIM_STAND);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_CAPTAIN, 1);
  // The captain and the keeper explain that only one chest may be chosen.
  Event_SetMessageId(MsgBilibinLordMccoyOrdered);
  // "Lord McCoy ordered that Isaac and his companions be brought..."
  Event_ShowMessageThenWait(ACTOR_CAPTAIN, 0, 10);
  Actor_WalkToAndWait(ACTOR_CAPTAIN, 622, 780);
  Actor_FaceDirection(ACTOR_CAPTAIN, FACING_NORTH, 10);
  Actor_RunRepeatedMotion(ACTOR_SOLDIER_SHOWING_THE_REWARD, 2);
  // "You may select a single item from among these four chests."
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
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(842, 0, 696),
    SC_SYNC,
    SC_MOVE(842, 0, 636),
    SC_SYNC,
    SC_MOVE(876, 0, 634),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_SET(SW_FACING, FACING_SOUTH_SOUTHEAST),
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

/// Unused: the getters return it for any scene but SceneId_McCoyPalace, and the
/// overlay serves no other.
struct ActorPlacement gMcCoyPalaceNoActorPlacements[] = {
    {SCENE_TABLE_END},
};

// clang-format off
struct ActorPlacement gMcCoyPalaceActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        265, 0, 702, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        342, 0, 632, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        360, 0, 632, FACING_EAST_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_WANDER,
        166, 0, 728, FACING_EAST_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        326, 0, 378, FACING_WEST_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_WANDER,
        362, 0, 142, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        296, 0, 87, FACING_NORTH_NORTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        952, 0, 289, FACING_EAST_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        951, 0, 438, FACING_SOUTH_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        616, 0, 728, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MCCOY, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        894, 0, 634, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_KEY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_IN_VIEW, MOVEMENT_TYPE_RESET_ACTION,
        616, 0, 616, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_IN_VIEW, MOVEMENT_TYPE_RESET_ACTION,
        632, 0, 616, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_IN_VIEW, MOVEMENT_TYPE_RESET_ACTION,
        648, 0, 616, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_IN_VIEW, MOVEMENT_TYPE_RESET_ACTION,
        664, 0, 616, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_IN_VIEW, MOVEMENT_TYPE_RESET_ACTION,
        616, 0, 648, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_IN_VIEW, MOVEMENT_TYPE_RESET_ACTION,
        648, 0, 648, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_IN_VIEW, MOVEMENT_TYPE_RESET_ACTION,
        664, 0, 648, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_IN_VIEW, MOVEMENT_TYPE_RESET_ACTION,
        616, 0, 664, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_IN_VIEW, MOVEMENT_TYPE_RESET_ACTION,
        648, 0, 664, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_CHEST, CONDITION_IN_VIEW, MOVEMENT_TYPE_RESET_ACTION,
        664, 0, 664, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    { SCENE_TABLE_END },
};

/// Unused, as gMcCoyPalaceNoActorPlacements is.
struct Trigger gMcCoyPalaceNoTriggers[] = {
    { TRIGGER_TABLE_END },
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
    { STEP_TRIGGER_FACING(FACING_NORTH), MCCOY_PALACE_ENTRANCE_ROOM_TO_MCCOYS_ROOM, CONDITION_ALWAYS, EVENT(McCoyPalace_WalkThroughDoor) },
    { TRIGGER_EXIT, 10, CONDITION_ALWAYS, MCCOY_PALACE_LADY_MCCOYS_ROOM_TO_ENTRANCE_ROOM_WEST },
    { TRIGGER_EXIT, 11, CONDITION_ALWAYS, MCCOY_PALACE_LADY_MCCOYS_ROOM_TO_ENTRANCE_ROOM_EAST },
    { STEP_TRIGGER_FACING(FACING_NORTH), MCCOY_PALACE_LADY_MCCOYS_ROOM_TO_WEST_BEDROOM, CONDITION_ALWAYS, EVENT(McCoyPalace_WalkThroughDoor) },
    { STEP_TRIGGER_FACING(FACING_NORTH), MCCOY_PALACE_LADY_MCCOYS_ROOM_TO_EAST_BEDROOM, CONDITION_ALWAYS, EVENT(McCoyPalace_WalkThroughDoor) },
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
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_MCCOY_PALACE_VIAL_CHEST, CONDITION_ALWAYS, EVENT(McCoyPalace_SearchVialChest) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_MCCOY_PALACE_POTION_CHEST, CONDITION_ALWAYS, EVENT(McCoyPalace_SearchPotionChest) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_MCCOY_PALACE_PSY_CRYSTAL_CHEST, CONDITION_ALWAYS, EVENT(McCoyPalace_SearchPsyCrystalChest) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_MCCOY_PALACE_WATER_OF_LIFE_CHEST, CONDITION_ALWAYS, EVENT(McCoyPalace_SearchWaterOfLifeChest) },
    { TRIGGER_STEP, CELL_BEHAVIOR_MCCOY_PALACE_BEFORE_REWARD_CHESTS, FLAG_PARTY_SEEN_OFF, EVENT(McCoyPalace_CaptainSeesPartyOff) },
    { TRIGGER_STEP, CELL_BEHAVIOR_MCCOY_PALACE_FOOT_OF_TREASURE_STAIRS, FLAG_PARTY_SEEN_OFF, EVENT(McCoyPalace_CaptainAsksPartyToChoose) },
    { EXAMINE_TRIGGER(EXAMINE_BARREL), CELL_BEHAVIOR_MCCOY_PALACE_ELIXIR_BARREL, FLAG_MCCOY_PALACE_BARREL_ELIXIR, FIND_ITEM(ITEM_ELIXIR) },
    { EXAMINE_TRIGGER(EXAMINE_BARREL), CELL_BEHAVIOR_MCCOY_PALACE_SMOKE_BOMB_BARREL, FLAG_MCCOY_PALACE_BARREL_SMOKE_BOMB, FIND_ITEM(ITEM_SMOKE_BOMB) },
    { EXAMINE_TRIGGER(EXAMINE_WARDROBE), CELL_BEHAVIOR_MCCOY_PALACE_WARDROBE, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBilibinTheresASilkNegligeeIsaacPut)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), CELL_BEHAVIOR_MCCOY_PALACE_BOOKCASE, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBilibinTheresASheetOfPaperTitled)) },
    { TRIGGER_TABLE_END },
};
// clang-format on
