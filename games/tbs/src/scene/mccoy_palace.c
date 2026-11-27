/* @file */
/* McCoy's Palace in Bilibin, the home of the town's chief. */
/* */
/* The palace is one scene, SceneId_McCoyPalace, sharing the area name McCoy's
 */
/* Palace with its grounds (mccoy_palace_grounds.c). Its rooms are side by */
/* side on one map, each in its own camera box: the entrance hall with its red
 */
/* carpet, McCoy's room at the carpet's top, Lady McCoy's room with a bedroom */
/* through each of its two back doors, the landings and small tower rooms */
/* at the hall's four corners, and the treasure room below, reached from the */
/* west of the hall. */
/* */
/* The party first comes in by the front door once the east guard on the */
/* grounds lets it in. A man in the hall says to follow the red carpet, and */
/* walking through the door at its top (scMcCoyPalaceWalkThroughDoor) brings */
/* Isaac before McCoy, a stout red-haired man who speaks with a heavy accent */
/* and a portrait. Before Kolima's curse is lifted this is the first audience */
/* (scMcCoyPalaceAudienceWithMcCoy): McCoy asks whether the party will save */
/* Kolima. If Isaac says no, he is sent away and can come back to change his */
/* mind. If he says yes, McCoy tosses him the key to the barricades, then */
/* takes it back, deciding they are too young, and his captain sees the party */
/* out. Outside his door the captain is sorry, and lets slip that */
/* the barricade on the way to Kolima may not need a key */
/* (scMcCoyPalaceCaptainTalksAboutBarricade); from then on the people speak as
 */
/* if the party had been turned away */
/* (FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY). */
/* */
/* Once the curse is lifted, the guards on the grounds march the party */
/* straight into McCoy's room, where McCoy thanks it */
/* (scMcCoyPalaceMcCoyRewardsParty). His lines differ as the party has met him
 */
/* before or not. The captain then leads it down to the treasure room, where */
/* four chests stand in a row before the barred gate */
/* (scMcCoyPalacePresentRewardChests): from west to east a Vial, a Potion, a */
/* Psy Crystal and a Water of Life. Only one may be taken */
/* (scMcCoyPalaceClaimReward); the others are then locked. Heading back up the
 */
/* stairs without a reward, the captain stops Isaac and sends him back to the */
/* chests; once he has one, the captain bids the party farewell. When the */
/* party comes back later, the four chests are gone, and which one it opened */
/* was only ever kept in the scene's temporary flags (FLAG_TOOK_VIAL and the */
/* others). */
/* */
/* The palace's people follow three story flags, */
/* FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY, FLAG_KOLIMA_CURSE_LIFTED and */
/* FLAG_MCCOY_PALACE_REWARD_TAKEN, which decide what they say and how they */
/* turn when talked to (scGetActorPlacements). */
/* */
/* A barrel by the chimney in the hall holds an Elixir and one in the */
/* small room at the south-west corner a Smoke Bomb. The wardrobe in the east */
/* bedroom holds Lady McCoy's silk negligee, and the bookcase in the west */
/* bedroom the long list of gifts McCoy has bought her. */

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/actor_script.h"
#include "field/event_inline.h"
#include "graphics/graphics.h"
#include "text/msg_ids.h"

extern s32 McCoyPalaceTurnTowardWatched[];
extern s32 McCoyPalaceCaptainStepsBesideMcCoy[];
extern SCENE_ENTRANCE McCoyPalaceEntrances[];
extern u32 McCoyPalaceExits[];
extern ACTOR_PLACE McCoyPalaceNoActorPlacements[];
extern ACTOR_PLACE McCoyPalaceActorPlacements[];
extern TRIGGER McCoyPalaceNoTriggers[];
extern TRIGGER McCoyPalaceTriggers[];

/* The palace's people and the treasure room's chests, numbering */
/* McCoyPalaceActorPlacements' rows from ACTOR_FIRST_PLACED. */
/* Each note gives what they say before the reward, then after it. */
typedef enum MC_COY_PALACE_ACTOR_ID {
	/* A burly grey-haired man who points the way to McCoy. "If you want to see
	 */
	/* Lord McCoy, follow the red carpet." Later: "The people of Bilibin will */
	/* never forget your bravery." */
	ACTOR_VILLAGER_FOLLOW_RED_CARPET = ACTOR_FIRST_PLACED,
	/* A woman in a green dress who warns the party about McCoy. "Make sure you
	 */
	/* don't offend Lord McCoy!" Later: "I heard you received a spectacular */
	/* reward." */
	ACTOR_VILLAGER_DO_NOT_OFFEND_MCCOY,
	/* A woman with fair hair in the hall who asks whether the party is willing
	 */
	/* to go to Kolima Forest. Later: "Lady McCoy is quite upset that the palace
	 */
	/* construction was halted." */
	ACTOR_VILLAGER_ASKING_ABOUT_KOLIMA_FOREST,
	/* A young man with brown hair who speaks of the old tree in Kolima Forest.
	 */
	/* Later: "Things are looking grim south of Goma Range..." */
	ACTOR_YOUNG_MAN_PROTECT_OLD_TREE,
	/* A woman with fair hair standing in Lady McCoy's room. "This is Lady */
	/* McCoy's room, so don't touch anything." Later: "Milady was very upset
	   when */
	/* she left the room." */
	ACTOR_VILLAGER_LADY_MCCOYS_ROOM,
	/* A woman with fair hair who admires Lady McCoy. "Milady was renowned far
	 */
	/* and wide for her beauty!" Later: "For once, milord didn't let Lady McCoy
	 */
	/* push him around!" */
	ACTOR_VILLAGER_MILADYS_BEAUTY,
	/* A woman with fair hair who screams when spoken to ("AAAAH!!!"). Mind
	   Read: */
	/* "Oh, no! This nightgown has a hole... Milady will be furious!" */
	ACTOR_VILLAGER_IN_MILADYS_NIGHTGOWN,
	/* An old man with a cane in the north-east tower room. "You can see Kolima
	 */
	/* pretty well from here." Later he praises the party. */
	ACTOR_OLD_MAN_PRAISING_THE_WARRIORS,
	/* A soldier in a spiked helmet in the south-east tower room. "Hmm. It seems
	 */
	/* the curse of the tree has not yet come to Bilibin." Later he says he */
	/* watched the party go to Kolima. */
	ACTOR_SOLDIER_WATCHING_KOLIMA,
	/* A soldier in a spiked helmet who guards the treasure room's barred gate,
	 */
	/* and shows the party the four chests when the captain brings it down. */
	ACTOR_SOLDIER_SHOWING_THE_REWARD,
	/* McCoy, the chief of Bilibin, a stout man with red hair in a pink tunic.
	 */
	ACTOR_LORD_MCCOY,
	/* The captain of McCoy's soldiers, drawn as they are, in a spiked helmet.
	 */
	ACTOR_CAPTAIN,
	/* The key McCoy tosses to the party. */
	ACTOR_BARRICADE_KEY,
	ACTOR_VIAL_CHEST,
	ACTOR_POTION_CHEST,
	ACTOR_PSY_CRYSTAL_CHEST,
	ACTOR_WATER_OF_LIFE_CHEST
} MC_COY_PALACE_ACTOR_ID;

/* The cells the triggers answer besides the exits, whose cell behaviors are */
/* their exits' numbers. */
typedef enum MC_COY_PALACE_CELL_BEHAVIOR {
	/* The row just in front of the four reward chests, cells (38, 46) to */
	/* (41, 46), copied there from (42, 55) to (45, 55) when the chests are */
	/* brought out. */
	CELL_BEHAVIOR_MCCOY_PALACE_BEFORE_REWARD_CHESTS = 50,
	/* The treasure room's floor at the foot of its stairs, cell (36, 47), one
	 */
	/* step before the way out. */
	CELL_BEHAVIOR_MCCOY_PALACE_FOOT_OF_TREASURE_STAIRS = 51,
	/* The four reward chests' cells, (38, 45) to (41, 45) from west to east, */
	/* copied there from (38, 55) to (41, 55) when the chests are brought out.
	 */
	CELL_BEHAVIOR_MCCOY_PALACE_VIAL_CHEST = 100,
	CELL_BEHAVIOR_MCCOY_PALACE_POTION_CHEST = 101,
	CELL_BEHAVIOR_MCCOY_PALACE_PSY_CRYSTAL_CHEST = 102,
	CELL_BEHAVIOR_MCCOY_PALACE_WATER_OF_LIFE_CHEST = 103,
	/* The barrel by the chimney in the hall, cell (23, 40), with an Elixir. */
	CELL_BEHAVIOR_MCCOY_PALACE_ELIXIR_BARREL = 120,
	/* The barrel in the small room at the south-west corner, cell (45, 29),
	   with */
	/* a Smoke Bomb. */
	CELL_BEHAVIOR_MCCOY_PALACE_SMOKE_BOMB_BARREL = 121,
	/* The east bedroom's wardrobe, cells (17, 3) to (18, 4), with Lady McCoy's
	 */
	/* silk negligee. */
	CELL_BEHAVIOR_MCCOY_PALACE_WARDROBE = 200,
	/* The west bedroom's bookcase, cells (9, 3) to (10, 4), with the list of */
	/* McCoy's gifts to his wife. */
	CELL_BEHAVIOR_MCCOY_PALACE_BOOKCASE = 201
} MC_COY_PALACE_CELL_BEHAVIOR;

/* The palace's doors swing open by the map's tileset animations, which each */
/* scene entry pauses so every door is drawn shut; walking through any door */
/* runs both a moment. */
typedef enum MC_COY_PALACE_DOOR_ANIMATION {
	/* McCoy's door at the top of the red carpet, four tiles across. */
	MCCOY_PALACE_MCCOYS_DOOR_ANIMATION = 1,
	/* The two bedroom doors in Lady McCoy's room, two tiles across. */
	MCCOY_PALACE_BEDROOM_DOORS_ANIMATION = 2
} MC_COY_PALACE_DOOR_ANIMATION;

/* The palace's temporary flags. */
/* The scene temp flags last until the party leaves the palace's scene, the */
/* area temp flags until it leaves McCoy's Palace. */
enum {
	/* The captain has opened McCoy's door, so walking through it is silent. */
	FLAG_MCCOYS_DOOR_OPEN = FLAG_SCENE_TEMP_00,
	/* The party is choosing its reward among the four chests. */
	FLAG_CHOOSING_REWARD = FLAG_SCENE_TEMP_02,
	/* The chest the reward was taken from, which then searches as empty. */
	FLAG_TOOK_VIAL = FLAG_SCENE_TEMP_10,
	FLAG_TOOK_POTION = FLAG_SCENE_TEMP_11,
	FLAG_TOOK_PSY_CRYSTAL = FLAG_SCENE_TEMP_12,
	FLAG_TOOK_WATER_OF_LIFE = FLAG_SCENE_TEMP_13,
	/* The old man has praised the party once. He then warns of the holy tree.
	 */
	FLAG_OLD_MAN_HAS_PRAISED = FLAG_AREA_TEMP_02,
	/* Mind Read has heard the old man's first thought. */
	FLAG_OLD_MAN_THOUGHTS_READ = FLAG_AREA_TEMP_03,
	/* McCoy has had the captain see the party out after the first audience. */
	FLAG_CAPTAIN_SEEING_PARTY_OUT = FLAG_AREA_TEMP_21,
	/* McCoy has had the captain take the party to the treasure room, until it
	 */
	/* takes a reward. */
	FLAG_REWARD_OFFERED = FLAG_AREA_TEMP_22,
	/* The captain has seen the party off with its reward. */
	FLAG_PARTY_SEEN_OFF = FLAG_AREA_TEMP_33
};

static void scMcCoyPalaceClaimReward(s32 chestActorId, s32 itemId,
									 s32 chestFlag);
static void scMcCoyPalaceAudienceWithMcCoy(void);
static void scMcCoyPalaceCaptainTalksAboutBarricade(void);
static void scMcCoyPalaceMcCoyRewardsParty(void);
static void scMcCoyPalacePresentRewardChests(void);

/* scTurnTowardTarget, the call McCoyPalaceTurnTowardWatched makes. */
#include "shared/actor_facing.c"

const SCENE_ENTRANCE *scGetEntrances(void) { return McCoyPalaceEntrances; }

const SCENE_EXIT_REGION *scGetExitRegions(void) { return NULL; }

const u32 *scGetExits(void) { return McCoyPalaceExits; }

/* The people of McCoy's Palace. Once the party has its reward, the woman in */
/* Lady McCoy's nightgown no longer turns when spoken to, and the old man and */
/* the soldiers in the tower rooms and the treasure room turn to the party. */
const ACTOR_PLACE *scGetActorPlacements(void) {
	ACTOR_PLACE *placements;

	if(MainRoutineStateWork.next == SCENE_MC_COY_PALACE) {
		placements = McCoyPalaceActorPlacements;
		scAssignViewFlags(placements);

		if(gmFlagGet(FLAG_MCCOY_PALACE_REWARD_TAKEN) != 0) {
			McCoyPalaceActorPlacements[ACTOR_VILLAGER_IN_MILADYS_NIGHTGOWN -
									   ACTOR_FIRST_PLACED]
				.talkFacing = TALK_FACING_KEEP;
			McCoyPalaceActorPlacements[ACTOR_OLD_MAN_PRAISING_THE_WARRIORS -
									   ACTOR_FIRST_PLACED]
				.talkFacing = TALK_FACING_PARTY;
			McCoyPalaceActorPlacements[ACTOR_SOLDIER_WATCHING_KOLIMA -
									   ACTOR_FIRST_PLACED]
				.talkFacing = TALK_FACING_PARTY_THEN_WATCH;
			McCoyPalaceActorPlacements[ACTOR_SOLDIER_SHOWING_THE_REWARD -
									   ACTOR_FIRST_PLACED]
				.talkFacing = TALK_FACING_PARTY_AND_BACK;
		}

		return placements;
	}
	return McCoyPalaceNoActorPlacements;
}

/* What the party can talk to and search in the palace. */
const TRIGGER *scGetTriggers(void) {
	if(MainRoutineStateWork.next == SCENE_MC_COY_PALACE) {
		return McCoyPalaceTriggers;
	}
	return McCoyPalaceNoTriggers;
}

/* Before the reward, the woman with fair hair in the hall asks "Oh, are you */
/* willing to go to Kolima Forest?" On yes she has lost count of the warriors */
/* McCoy has hired to end the curse, and not one has come back; on no, the */
/* party does not look too strong, but that may be for the best. */
static void scMcCoyPalaceAskAboutGoingToKolima(void) {
	fdEventStart();
	fdEventSetMessageId(MsgBilibinYouWillingGoKolimaForest);
	/* "Oh, are you willing to go to Kolima Forest?" */
	/* Yes: "I forget how many people Lord McCoy has hired to end the..." */
	/* No: "You don't look too strong, but that may be for the best." */
	fdEventAskYesNo(ACTOR_VILLAGER_ASKING_ABOUT_KOLIMA_FOREST, 0);
	fdEventEnd();
}

/* After the reward, the woman in Lady McCoy's nightgown in the east bedroom */
/* winces and screams "AAAAH!!!" when spoken to, then turns: "Oh! For */
/* a second, I thought you were milady. You really startled me!" */
static void scMcCoyPalaceTalkToWomanInMiladysNightgown(void) {
	fdEventStart();
	fdActorShowEmote(ACTOR_VILLAGER_IN_MILADYS_NIGHTGOWN,
					 EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorRunRepeatedMotion(ACTOR_VILLAGER_IN_MILADYS_NIGHTGOWN, 2);
	fdEventWait(40);
	fdEventSetMessageId(MsgBilibinAaaah);
	/* "AAAAH!!!" */
	fdEventShowMessageThenWait(ACTOR_VILLAGER_IN_MILADYS_NIGHTGOWN, 0, 20);
	fdActorFaceActor(ACTOR_VILLAGER_IN_MILADYS_NIGHTGOWN, ACTOR_PARTY_LEADER,
					 0);
	fdEventWait(20);
	/* "Oh! For a second, I thought you were milady. You really..." */
	fdEventShowMessageThenWait(ACTOR_VILLAGER_IN_MILADYS_NIGHTGOWN, 0, 10);
	fdActorFaceDirection(ACTOR_VILLAGER_IN_MILADYS_NIGHTGOWN,
						 FACING_NORTH_NORTHWEST, 10);
	fdEventEnd();
}

/* After the reward, the old man with a cane in the north-east tower room tells
 */
/* Isaac "You are a fine warrior." Spoken to again before the party leaves */
/* McCoy's Palace, he worries instead: "It's very reckless for such young */
/* warriors to face the holy tree..." */
static void scMcCoyPalaceTalkToOldManPraisingTheWarriors(void) {
	fdEventStart();
	fdEventSetMessageId(MsgBilibinFineWarrior);
	if(fdEventGetFlag(FLAG_OLD_MAN_HAS_PRAISED) != 0) {
		fdEventSetMessageId(MsgBilibinItsVeryRecklessForSuch);
	}
	fdEventShowMessage(ACTOR_OLD_MAN_PRAISING_THE_WARRIORS, 0);
	gmFlagSet(FLAG_OLD_MAN_HAS_PRAISED);
	fdEventEnd();
}

/* After the reward, the soldier in the south-east tower room asks "I was */
/* watching from here. After all that fuss, you still went to Kolima?" On yes */
/* he does not know how they lifted the curse, but they are the best; on no he
 */
/* calls Isaac a liar, for he saw them walk through the barricade. */
static void scMcCoyPalaceTalkToSoldierWatchingKolima(void) {
	fdEventStart();
	fdEventSetMessageId(MsgBilibinWasWatchingFromHereAfter);
	/* "I was watching from here. After all that fuss, you still..." */
	/* Yes: "I don't know how you removed the curse, but you're the best!" */
	/* No: "You lie! I saw you walking through the barricade! And then I..." */
	fdEventAskYesNo(ACTOR_SOLDIER_WATCHING_KOLIMA, 0);
	fdEventEnd();
}

/* The soldier at the treasure room's barred gate shows off what lies behind */
/* it: "What do you think? These are Bilibin's great treasures. Whoever ends */
/* the curse can claim these as his own." While the party is choosing its */
/* reward he says "You may choose only one item, but I trust that all of them */
/* will prove useful," and once it has one, that it was indeed the greatest of
 */
/* treasures. */
/* */
/* Unused: a line the game never shows, "Pick any one of the four */
/* treasures.", would follow his boast if FLAG_MCCOY_PALACE_UNUSED_PICK_OFFERED
 */
/* were set, and nothing sets it. */
static void scMcCoyPalaceTalkToSoldierShowingTheReward(void) {
	fdEventStart();

	if(gmFlagGet(FLAG_CHOOSING_REWARD) != 0) {
		fdEventSetMessageId(MsgBilibinMayChooseOnlyOneItem);
	}
	else if(gmFlagGet(FLAG_MCCOY_PALACE_REWARD_TAKEN) != 0) {
		fdEventSetMessageId(MsgBilibinRewardReceivedWasIndeedGreatest);
	}
	else {
		/* "What do you think? These are Bilibin's great treasures." */
		fdEventSetMessageId(MsgBilibinDoThinkTheseBilibinsGreat);
		if(gmFlagGet(FLAG_MCCOY_PALACE_UNUSED_PICK_OFFERED) != 0) {
			/* "Pick any one of the four treasures." */
			FieldStateWork->msg++;
		}
	}

	fdEventShowMessage(ACTOR_SOLDIER_SHOWING_THE_REWARD, 0);
	fdEventEnd();
}

/* After the reward, Mind Read on the old man with a cane in the north-east */
/* tower room hears "Sometimes, we need children to remind us of our own */
/* responsibilities." Read again before the party leaves McCoy's Palace, he */
/* thinks "You looked very courageous walking toward Kolima." */
static void scMcCoyPalaceReadOldMansThoughtsOnTheWarriors(void) {
	fdEventStart();
	fdEventSetMessageId(MsgBilibinSometimesWeNeedChildrenRemind);
	if(fdEventGetFlag(FLAG_OLD_MAN_THOUGHTS_READ) != 0) {
		fdEventSetMessageId(MsgBilibinLookedVeryCourageousWalkingToward);
	}
	fdEventShowMessage(ACTOR_OLD_MAN_PRAISING_THE_WARRIORS, 0);
	gmFlagSet(FLAG_OLD_MAN_THOUGHTS_READ);
	fdEventEnd();
}

/* Mind Read on the soldier at the treasure room's barred gate. Before the */
/* curse is lifted he thinks "What good is treasure if you get turned into a */
/* tree?" Once it is lifted: "Are we really giving our treasure to these */
/* wimps?" While the party chooses: "Nonetheless...if your luck is sour, you */
/* may end up an item you do not need." After the reward he thinks they got a */
/* pretty nice reward but were just unlucky. */
static void scMcCoyPalaceReadRewardSoldiersThoughts(void) {
	fdEventStart();

	if(gmFlagGet(FLAG_CHOOSING_REWARD) != 0) {
		fdEventSetMessageId(MsgBilibinNonethelessIfYourLuckSour);
	}
	else if(gmFlagGet(FLAG_KOLIMA_CURSE_LIFTED) == 0) {
		fdEventSetMessageId(MsgBilibinGoodTreasureIfGetTurned);
	}
	else {
		fdEventSetMessageId(MsgBilibinWeReallyGivingOurTreasure);
		if(gmFlagGet(FLAG_MCCOY_PALACE_REWARD_TAKEN) != 0) {
			fdEventSetMessageId(MsgBilibinGotPrettyNiceRewardBut);
		}
	}

	fdEventShowMessage(ACTOR_SOLDIER_SHOWING_THE_REWARD, 0);
	fdEventEnd();
}

/* Walking north into the door at the top of the hall's red carpet (cells */
/* (15, 39) and (16, 39)) opens it, and Isaac walks through into McCoy's room.
 */
/* The two doors at the back of Lady McCoy's room, cells (6, 23) and (19, 23),
 */
/* open the same way into the west and east bedrooms. */
/* */
/* McCoy's door opens with `SE_THUMP` and the bedroom doors with */
/* `SE_DOOR_OPEN`; the doors' opening is the map's tileset animations 1 and 2,
 */
/* let run for the walk. After the captain has come out of McCoy's door */
/* (scMcCoyPalaceCaptainTalksAboutBarricade) it stands open, so walking back in
 */
/* makes no sound and plays no opening. */
static void scMcCoyPalaceWalkThroughDoor(void) {
	FIELD_STATE *state;
	s32 playDoorOpening;

	state = FieldStateWork;
	playDoorOpening = 0;
	if(state->stepTrigger == MCCOY_PALACE_ENTRANCE_ROOM_TO_MCCOYS_ROOM) {
		if(gmFlagGet(FLAG_MCCOYS_DOOR_OPEN) == 0) {
			sySndPlay(SE_THUMP);
			playDoorOpening = 1;
		}
	}
	else {
		sySndPlay(SE_DOOR_OPEN);
		playDoorOpening = 1;
	}
	if(playDoorOpening != 0) {
		fdTilesetAnimResume(MCCOY_PALACE_MCCOYS_DOOR_ANIMATION);
		fdTilesetAnimResume(MCCOY_PALACE_BEDROOM_DOORS_ANIMATION);
	}
	fdEventStart();
	fdEventWait(10);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.5);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
	if(state->stepTrigger == MCCOY_PALACE_ENTRANCE_ROOM_TO_MCCOYS_ROOM)
		fdActorSetDestinationOffset(ACTOR_PARTY_LEADER, 0, -16);
	else
		fdActorCenterAndWalk(ACTOR_PARTY_LEADER, 3, -16);
	fdEventWait(16);
	syImportEventSetExitRequest(state->stepTrigger);
	fdEventEnd();
	fdTilesetAnimPause(MCCOY_PALACE_MCCOYS_DOOR_ANIMATION);
	fdTilesetAnimPause(MCCOY_PALACE_BEDROOM_DOORS_ANIMATION);
}

/* "Isaac checked the chest... but it was empty." */
static void scMcCoyPalaceShowChestEmpty(void) {
	fdEventStart();
	mnFieldMessageShow(MsgBilibinIsaacCheckedChestButWas, 1);
	fdEventEnd();
}

/* "The treasure chest is locked." */
static void scMcCoyPalaceShowChestLocked(void) {
	fdEventStart();
	mnFieldMessageShow(MsgBilibinTreasureChestLocked, 1);
	fdEventEnd();
}

/* After the reward, the chest the party opened searches as empty and the other
 */
/* three as locked. */
static void scMcCoyPalaceShowUnavailableChest(s32 chestFlag) {
	if(gmFlagGet(chestFlag) != 0) {
		scMcCoyPalaceShowChestEmpty();
	}
	else {
		scMcCoyPalaceShowChestLocked();
	}
}

/* Searching the westmost of the four reward chests in the treasure room */
/* (cell (38, 45)) gives a Vial, if the party has not yet chosen its reward. */
static void scMcCoyPalaceSearchVialChest(void) {
	if(gmFlagGet(FLAG_MCCOY_PALACE_REWARD_TAKEN) != 0) {
		scMcCoyPalaceShowUnavailableChest(FLAG_TOOK_VIAL);
	}
	else {
		scMcCoyPalaceClaimReward(ACTOR_VIAL_CHEST, ITEM_VIAL, FLAG_TOOK_VIAL);
	}
}

/* Searching the second reward chest from the west in the treasure room (cell */
/* (39, 45)) gives a Potion, if the party has not yet chosen its reward. */
static void scMcCoyPalaceSearchPotionChest(void) {
	if(gmFlagGet(FLAG_MCCOY_PALACE_REWARD_TAKEN) != 0) {
		scMcCoyPalaceShowUnavailableChest(FLAG_TOOK_POTION);
	}
	else {
		scMcCoyPalaceClaimReward(ACTOR_POTION_CHEST, ITEM_POTION,
								 FLAG_TOOK_POTION);
	}
}

/* Searching the third reward chest from the west in the treasure room (cell */
/* (40, 45)) gives a Psy Crystal, if the party has not yet chosen its reward. */
static void scMcCoyPalaceSearchPsyCrystalChest(void) {
	if(gmFlagGet(FLAG_MCCOY_PALACE_REWARD_TAKEN) != 0) {
		scMcCoyPalaceShowUnavailableChest(FLAG_TOOK_PSY_CRYSTAL);
	}
	else {
		scMcCoyPalaceClaimReward(ACTOR_PSY_CRYSTAL_CHEST, ITEM_PSY_CRYSTAL,
								 FLAG_TOOK_PSY_CRYSTAL);
	}
}

/* Searching the eastmost of the four reward chests in the treasure room (cell
 */
/* (41, 45)) gives a Water of Life, if the party has not yet chosen its reward.
 */
static void scMcCoyPalaceSearchWaterOfLifeChest(void) {
	if(gmFlagGet(FLAG_MCCOY_PALACE_REWARD_TAKEN) != 0) {
		scMcCoyPalaceShowUnavailableChest(FLAG_TOOK_WATER_OF_LIFE);
	}
	else {
		scMcCoyPalaceClaimReward(ACTOR_WATER_OF_LIFE_CHEST, ITEM_WATER_OF_LIFE,
								 FLAG_TOOK_WATER_OF_LIFE);
	}
}

/* The chest opens and gives its item, and the other three lock. If no one in */
/* the party has room for it, the lid peeks open and shuts again, and the */
/* party can still choose. */
static void scMcCoyPalaceClaimReward(s32 chestActorId, s32 itemId,
									 s32 chestFlag) {
	s32 emitterHandle;

	fdEventStart();

	emitterHandle = fdChestOpenAndShowItem(0, chestActorId, itemId);

	if(gmPartyGiveItem(itemId, 0) != -1) {
		fdActorSetAnimation(chestActorId, CHEST_ANIMATION_OPEN_EMPTY);
		gmFlagSet(FLAG_MCCOY_PALACE_REWARD_TAKEN);
		gmFlagSet(chestFlag);
		gmFlagClear(FLAG_REWARD_OFFERED);
		gmFlagClear(FLAG_CHOOSING_REWARD);
	}
	else {
		sySndPlay(SE_CHEST_CLOSE);
		fdActorSetAnimation(chestActorId, CHEST_ANIMATION_PEEK_AND_SHUT);
	}

	fdActorRelease((ACTOR *)emitterHandle);
	fdEventEnd();
}

/* Walking to the foot of the treasure room's stairs (cell (36, 47)) without */
/* a reward makes the captain call out "Please take your reward before your */
/* depart!", and Isaac walks back toward the chests. */
static void scMcCoyPalaceCaptainAsksPartyToChoose(void) {
	if(gmFlagGet(FLAG_MCCOY_PALACE_REWARD_TAKEN) == 0) {
		if(gmFlagGet(FLAG_REWARD_OFFERED) != 0) {
			fdEventStart();
			fdActorShowEmote(ACTOR_CAPTAIN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
							 0);
			fdActorFaceDirection(ACTOR_CAPTAIN, FACING_WEST_SOUTHWEST, 10);
			fdActorRunRepeatedMotion(ACTOR_CAPTAIN, 2);
			fdEventWait(20);
			fdEventSetMessageId(MsgBilibinPleaseTakeYourRewardBefore);
			/* "Please take your reward before your depart!" */
			fdEventShowMessage(ACTOR_CAPTAIN, 0);
			fdActorSetSpeed(ACTOR_PARTY_LEADER, 1);
			fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 616, 762);
			fdActorFaceDirection(ACTOR_CAPTAIN, FACING_NORTH_NORTHEAST, 10);
			fdEventEnd();
		}
	}
}

/* Once Isaac has his reward, walking along the row in front of the chests */
/* (cells (38, 46) to (41, 46)) makes the captain step up beside the soldier */
/* at the gate. Both nod, and the captain tells Isaac "You are always welcome */
/* in the palace of Lord McCoy." before walking off up the stairs. */
/* */
/* It can play only in the visit the reward is taken in: the row takes the */
/* cells its trigger answers only while the chests stand out before the gate */
/* (scMcCoyPalaceOpenRewardChestRow), and once a reward is taken a later visit
 */
/* does not bring them out again. */
static void scMcCoyPalaceCaptainSeesPartyOff(void) {
	if(gmFlagGet(FLAG_MCCOY_PALACE_REWARD_TAKEN) != 0) {
		fdEventStart();
		fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_CAPTAIN, 0);
		fdActorSetSpeed(ACTOR_CAPTAIN, 0.6);
		fdActorWalkToAndWait(ACTOR_CAPTAIN, 622, 764);
		fdActorFaceDirection(ACTOR_CAPTAIN, FACING_EAST_NORTHEAST, 20);
		fdActorSetAnimationAndWait(ACTOR_CAPTAIN, ANIM_NOD);
		fdActorSetAnimationAndWait(ACTOR_SOLDIER_SHOWING_THE_REWARD, ANIM_NOD);
		fdEventWait(20);
		fdActorFaceActor(ACTOR_CAPTAIN, ACTOR_PARTY_LEADER, 0);
		fdEventWait(20);
		fdActorSetAnimationAndWait(ACTOR_CAPTAIN, ANIM_NOD);
		fdEventSetMessageId(MsgBilibinAlwaysWelcomeInPalaceLord);
		/* "You are always welcome in the palace of Lord McCoy." */
		fdEventShowMessageThenWait(ACTOR_CAPTAIN, 0, 10);
		fdActorSetSpeed(ACTOR_CAPTAIN, 0.8);
		fdActorWalkToAndWait(ACTOR_CAPTAIN, 570, 758);
		fdActorSetPos(ACTOR_CAPTAIN, 0, 0);
		FieldStateWork->transition =
			SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
		gmFlagSet(FLAG_MCCOY_PALACE_PARTY_REWARDED);
		gmFlagSet(FLAG_PARTY_SEEN_OFF);
		fdEventEnd();
	}
}

/* The four reward chests stand in a row before the barred gate, from west to */
/* east the Vial, the Potion, the Psy Crystal and the Water of Life. */
static inline void scMcCoyPalacePosRewardChests(void) {
	fdActorSetPos(ACTOR_VIAL_CHEST, 616, 728);
	fdActorSetPos(ACTOR_POTION_CHEST, 632, 728);
	fdActorSetPos(ACTOR_PSY_CRYSTAL_CHEST, 648, 728);
	fdActorSetPos(ACTOR_WATER_OF_LIFE_CHEST, 664, 728);
}

/* The chests' cells and the row in front of them take the behaviors the */
/* triggers answer, copied from the spare cells at (38, 55) to (45, 55). */
static inline void scMcCoyPalaceOpenRewardChestRow(void) {
	fdMapGridCopyCellAttributes(38, 55, 4, 1, 38, 45);
	fdMapGridCopyCellAttributes(42, 55, 4, 1, 38, 46);
}

/* Sets McCoy's Palace up and plays the story's scenes: the first audience on */
/* entering McCoy's room, the captain's hint outside his door after it, the */
/* reward once the curse is lifted, and the chests in the treasure room. A */
/* return from a battle while the party is choosing puts the */
/* chests straight back in their row. */
s32 scInit(void) {
	s32 rewardTaken;
	s32 curseLifted;

	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
	fdTilesetAnimPause(MCCOY_PALACE_MCCOYS_DOOR_ANIMATION);
	fdTilesetAnimPause(MCCOY_PALACE_BEDROOM_DOORS_ANIMATION);
	gmFlagSet(FLAG_MCCOY_PALACE_VISITED);
	if(gmFlagGet(FLAG_KEEP_PARTY_POSITION) != 0) {
		gmFlagClear(FLAG_MCCOYS_DOOR_OPEN);
	}
	/* A first audience and a return visit have different greetings. */
	if(gmFlagGet(FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY) == 0 &&
	   gmFlagGet(FLAG_KOLIMA_CURSE_LIFTED) == 0) {
		if(MainRoutineStateWork.entrance ==
		   MCCOY_PALACE_FROM_MCCOY_PALACE_GROUNDS_EAST) {
			scMcCoyPalaceAudienceWithMcCoy();
		}
		else if(MainRoutineStateWork.entrance ==
					MCCOY_PALACE_ENTRANCE_ROOM_FROM_MCCOYS_ROOM &&
				gmFlagGet(FLAG_CAPTAIN_SEEING_PARTY_OUT) != 0) {
			scMcCoyPalaceCaptainTalksAboutBarricade();
		}
	}
	else {
		rewardTaken = gmFlagGet(FLAG_MCCOY_PALACE_REWARD_TAKEN);
		if(rewardTaken == 0) {
			if(MainRoutineStateWork.entrance ==
			   MCCOY_PALACE_FROM_MCCOY_PALACE_GROUNDS_EAST) {
				if(gmFlagGet(FLAG_MCCOY_PALACE_PARTY_REWARDED) == 0) {
					curseLifted = gmFlagGet(FLAG_KOLIMA_CURSE_LIFTED);
					if(curseLifted != 0) {
						scMcCoyPalaceMcCoyRewardsParty();
					}
				}
			}
			else {
				if(MainRoutineStateWork.entrance ==
				   MCCOY_PALACE_TREASURE_ROOM_FROM_ENTRANCE_ROOM) {
					if(gmFlagGet(FLAG_REWARD_OFFERED) != 0) {
						if(gmFlagGet(FLAG_KEEP_PARTY_POSITION) != 0) {
							scMcCoyPalaceOpenRewardChestRow();
							scMcCoyPalacePosRewardChests();
							fdActorSetSprFlags(fdActorGet(ACTOR_VIAL_CHEST), 0);
							fdActorSetSprFlags(fdActorGet(ACTOR_POTION_CHEST),
											   0);
							fdActorSetSprFlags(
								fdActorGet(ACTOR_PSY_CRYSTAL_CHEST), 0);
							fdActorSetSprFlags(
								fdActorGet(ACTOR_WATER_OF_LIFE_CHEST), 0);
							fdActorGet(ACTOR_VIAL_CHEST)->motionFlg =
								rewardTaken;
							fdActorGet(ACTOR_POTION_CHEST)->motionFlg =
								rewardTaken;
							fdActorGet(ACTOR_PSY_CRYSTAL_CHEST)->motionFlg =
								rewardTaken;
							fdActorGet(ACTOR_WATER_OF_LIFE_CHEST)->motionFlg =
								rewardTaken;
							fdActorGet(ACTOR_VIAL_CHEST)->y.fixed = -toFIXED(4);
							fdActorGet(ACTOR_POTION_CHEST)->y.fixed =
								-toFIXED(4);
							fdActorGet(ACTOR_PSY_CRYSTAL_CHEST)->y.fixed =
								-toFIXED(4);
							fdActorGet(ACTOR_WATER_OF_LIFE_CHEST)->y.fixed =
								-toFIXED(4);
						}
						else {
							scMcCoyPalacePresentRewardChests();
						}
					}
				}
			}
		}
	}
	return 0;
}

/* Garet, Ivan and Mia get ready to walk up to McCoy. */
static inline void scMcCoyPalacePrepareCompanionApproach(void) {
	fdActorSetSpeed(ACTOR_GARET, 0.6);
	fdActorSetSpeed(ACTOR_IVAN, 0.6);
	fdActorSetSpeed(ACTOR_MIA, 1);
	fdActorSetAnimation(ACTOR_GARET, ANIM_WALK);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_WALK);
	fdActorSetAnimation(ACTOR_MIA, ANIM_WALK);
}

/* The first audience, on walking through McCoy's door before Kolima's curse */
/* is lifted: the captain announces the party, McCoy calls Isaac closer and */
/* asks whether they will go to Kolima Forest. On no he sends them away as */
/* too young, and a return lets Isaac change his mind. On yes he tosses Isaac */
/* the key to the barricades, then, seeing how young the party is, takes it */
/* back and has his captain see them out. */
static void scMcCoyPalaceAudienceWithMcCoy(void) {
	ACTOR *actor;

	fdEventStart();
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	sySynch(1);
	fdCameraGetActor()->motionFlg = 0;
	fdCameraMoveTo(894, CAMERA_KEEP, 664, 0);
	sySynch(1);
	fdActorSetPos(ACTOR_PARTY_LEADER, 0, 0);
	if(fdEventGetFlag(FLAG_MCCOY_PALACE_JOB_REFUSED) != 0) {
		fdCameraMoveTo(894, CAMERA_KEEP, 698, 0);
		fdActorSetPos(ACTOR_CAPTAIN, 876, 634);
		fdActorFaceDirection(ACTOR_CAPTAIN, FACING_SOUTH_SOUTHEAST, 0);
		fdActorSetPos(ACTOR_PARTY_LEADER, 894, 798);
	}
	fdMapGridCenterOnFollowedPoint();
	sySynch(1);
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	FieldStateWork->transitionFrm = 40;
	fdEventStartScrTransition();
	fdEventWaitTransitionFrames();
	if(gmFlagGet(FLAG_MCCOY_PALACE_JOB_REFUSED) == 0) {
		fdEventWait(80);
		fdActorSetPos(ACTOR_CAPTAIN, 894, 798);
		fdCameraSetSpeed(0.6);
		fdCameraMoveTo(894, CAMERA_KEEP, 698, 1);
		fdActorSetSpeed(ACTOR_CAPTAIN, 0.8);
		fdActorWalkTo(ACTOR_CAPTAIN, 894, 696);
		fdEventWait(80);
		fdCameraMoveTo(894, CAMERA_KEEP, 664, 1);
		fdActorWaitForMoveById(ACTOR_CAPTAIN);
		fdActorWalkToAndWait(ACTOR_CAPTAIN, 842, 696);
		fdActorWalkToAndWait(ACTOR_CAPTAIN, 842, 636);
		fdActorFaceDirection(ACTOR_LORD_MCCOY, FACING_WEST_SOUTHWEST, 20);
		fdActorWalkToAndWait(ACTOR_CAPTAIN, 876, 634);
		fdActorSetAnimationAndWait(ACTOR_CAPTAIN, ANIM_NOD);
		fdEventWait(20);
		fdActorSetAnimationAndWait(ACTOR_LORD_MCCOY, ANIM_NOD);
		fdEventWait(10);
		/* The captain announces the warriors before they enter the audience. */
		fdEventSetMessageId(MsgBilibinMatter);
		/* "What's the matter, then?" */
		syFieldEventShowMessageThenWait(
			SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
		fdActorRunRepeatedMotion(ACTOR_CAPTAIN, 2);
		/* "Sir! Some new warriors have just arrived in the palace!" */
		fdEventShowMessageThenWait(ACTOR_CAPTAIN, 0, 20);
		fdActorRunRepeatedMotion(ACTOR_LORD_MCCOY, 1);
		/* "New warriors, yeh say?" */
		syFieldEventShowMessageThenWait(
			SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
		fdActorSetAnimationAndWait(ACTOR_CAPTAIN, ANIM_NOD);
		fdEventWait(40);
		fdActorShowEmote(ACTOR_LORD_MCCOY, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
		/* "We've sent many a man intae Kolima Forest, but not a one has..." */
		fdEventShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0);
		fdActorRunRepeatedMotion(ACTOR_LORD_MCCOY, 1);
		/* "These warriors... Did yeh get a good look at 'em?" */
		syFieldEventShowMessageThenWait(
			SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
		fdActorSetAttachedEffect(ACTOR_CAPTAIN, ATTACHED_EFFECT_IN_FRONT |
													ATTACHED_EFFECT_ONCE);
		fdEventWait(60);
		fdActorFaceDirection(ACTOR_CAPTAIN, FACING_SOUTH_SOUTHEAST, 10);
		fdActorFaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTH_SOUTHWEST, 10);
		fdCameraMoveTo(894, CAMERA_KEEP, 698, 1);
		fdActorSetPos(ACTOR_PARTY_LEADER, 894, 798);
		fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.6);
		fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 894, 726);
		fdEventWait(20);
		fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
		fdEventWait(10);
		fdActorRunRepeatedMotion(ACTOR_LORD_MCCOY, 1);
		/* "Come closer, laddy." */
		fdEventShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0);
		fdCameraMoveTo(894, CAMERA_KEEP, 664, 1);
		fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 894, 684);
		actor = fdActorGet(ACTOR_PARTY_LEADER);
		if(actor != NULL) {
			fdActorSetPosFX(ACTOR_GARET, actor->x.fixed, actor->z.fixed);
		}
		actor = fdActorGet(ACTOR_PARTY_LEADER);
		if(actor != NULL) {
			fdActorSetPosFX(ACTOR_IVAN, actor->x.fixed, actor->z.fixed);
		}
		if(gmFlagGet(FLAG_IN_PARTY(MIA)) != 0) {
			actor = fdActorGet(ACTOR_PARTY_LEADER);
			if(actor != NULL) {
				fdActorSetPosFX(ACTOR_MIA, actor->x.fixed, actor->z.fixed);
			}
		}
		scMcCoyPalacePrepareCompanionApproach();
		fdActorSetDestinationOffset(ACTOR_GARET, -16, 16);
		fdActorSetDestinationOffset(ACTOR_IVAN, 16, 16);
		if(gmFlagGet(FLAG_IN_PARTY(MIA)) != 0) {
			fdActorSetDestinationOffset(ACTOR_MIA, 32, 16);
		}
		fdActorWaitForMoveById(ACTOR_IVAN);
		fdActorSetAnimation(ACTOR_GARET, ANIM_STAND);
		fdActorSetAnimation(ACTOR_IVAN, ANIM_STAND);
		fdActorSetAnimation(ACTOR_MIA, ANIM_STAND);
		fdEventWait(10);
		fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
		fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
		fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 20);
		fdActorJump(ACTOR_LORD_MCCOY, 2, 20);
		fdActorFaceDirection(ACTOR_LORD_MCCOY, FACING_WEST_SOUTHWEST, 10);
		syFieldEventShowMessageThenWait(
			SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
		fdActorFaceDirection(ACTOR_CAPTAIN, FACING_EAST_SOUTHEAST, 10);
		fdActorSetAnimationAndWait(ACTOR_CAPTAIN, ANIM_NOD);
		fdActorFaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTH_SOUTHWEST, 40);
		fdActorFaceDirection(ACTOR_LORD_MCCOY, FACING_WEST_SOUTHWEST, 10);
		fdActorSetAnimationAndWait(ACTOR_LORD_MCCOY, ANIM_SHAKE_HEAD);
		syFieldEventShowMessageThenWait(
			SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
		fdActorSetAttachedEffect(ACTOR_CAPTAIN, ATTACHED_EFFECT_IN_FRONT |
													ATTACHED_EFFECT_ONCE);
		fdEventWait(40);
		fdActorFaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTH_SOUTHWEST, 20);
		fdActorShowEmote(ACTOR_LORD_MCCOY, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
		fdEventOpenMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0);
		fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
		fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
		if(fdEventAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
			/* The initial offer and reconsideration share their accept/refuse
			 * paths. */
			goto accepted;
		}
	declined:
		fdEventSetMessageId(MsgBilibinTooYoungForTheJob);
		/* "Bah! Yeh're too young. Yeh cannae handle the job." */
		syFieldEventShowMessageThenWait(
			SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
		fdActorFaceDirection(ACTOR_CAPTAIN, FACING_SOUTH_SOUTHEAST, 0);
		fdActorSetAnimationAndWait(ACTOR_LORD_MCCOY, ANIM_SHAKE_HEAD);
		/* "Yeh can leave now..." */
		fdEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0,
								   10);
		fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
		fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
		fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
		fdActorSetAnimation(ACTOR_GARET, ANIM_WALK);
		actor = fdActorGet(ACTOR_PARTY_LEADER);
		if(actor != NULL) {
			fdActorSetDestination(ACTOR_GARET, actor->x.part[0].integer,
								  actor->z.part[0].integer);
		}
		fdActorSetAnimation(ACTOR_IVAN, ANIM_WALK);
		actor = fdActorGet(ACTOR_PARTY_LEADER);
		if(actor != NULL) {
			fdActorSetDestination(ACTOR_IVAN, actor->x.part[0].integer,
								  actor->z.part[0].integer);
		}
		if(gmFlagGet(FLAG_IN_PARTY(MIA)) != 0) {
			fdActorSetAnimation(ACTOR_MIA, ANIM_WALK);
			actor = fdActorGet(ACTOR_PARTY_LEADER);
			if(actor != NULL) {
				fdActorSetDestination(ACTOR_MIA, actor->x.part[0].integer,
									  actor->z.part[0].integer);
			}
		}
		fdActorWaitForMoveById(ACTOR_IVAN);
		fdActorSetPos(ACTOR_GARET, 0, 0);
		fdActorSetPos(ACTOR_IVAN, 0, 0);
		fdActorSetPos(ACTOR_MIA, 0, 0);
		gmFlagSet(FLAG_MCCOY_PALACE_JOB_REFUSED);
		fdActorSetSpeed(ACTOR_PARTY_LEADER, 1);
		fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 894, 752);
		FieldStateWork->transitionFrm = MAP_TRANSITION_FRAMES;
		fdEventFinishScrTransition();
		fdEventWaitTransitionFrames();
		goto leave;
	}
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.6);
	fdActorWalkTo(ACTOR_PARTY_LEADER, 894, 684);
	fdEventWait(80);
	fdCameraSetSpeed(0.6);
	fdCameraMoveTo(894, CAMERA_KEEP, 664, 1);
	fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	actor = fdActorGet(ACTOR_PARTY_LEADER);
	if(actor != NULL) {
		fdActorSetPosFX(ACTOR_GARET, actor->x.fixed, actor->z.fixed);
	}
	actor = fdActorGet(ACTOR_PARTY_LEADER);
	if(actor != NULL) {
		fdActorSetPosFX(ACTOR_IVAN, actor->x.fixed, actor->z.fixed);
	}
	if(gmFlagGet(FLAG_IN_PARTY(MIA)) != 0) {
		actor = fdActorGet(ACTOR_PARTY_LEADER);
		if(actor != NULL) {
			fdActorSetPosFX(ACTOR_MIA, actor->x.fixed, actor->z.fixed);
		}
	}
	scMcCoyPalacePrepareCompanionApproach();
	fdActorSetDestinationOffset(ACTOR_GARET, -16, 16);
	fdActorSetDestinationOffset(ACTOR_IVAN, 16, 16);
	if(gmFlagGet(FLAG_IN_PARTY(MIA)) != 0) {
		fdActorSetDestinationOffset(ACTOR_MIA, 32, 16);
	}
	fdActorWaitForMoveById(ACTOR_IVAN);
	fdActorSetAnimation(ACTOR_GARET, ANIM_STAND);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_STAND);
	fdActorSetAnimation(ACTOR_MIA, ANIM_STAND);
	fdEventWait(10);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 20);
	fdActorShowEmote(ACTOR_LORD_MCCOY, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	/* A returning party can reconsider the assignment without replaying the */
	/* captain's announcement. */
	fdEventSetMessageId(MsgBilibinYehveChangeHeart);
	/* "What is it? Yeh've had a change of heart? Yeh want tae help..." */
	fdEventOpenMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0);
	if(fdEventAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 1) {
		goto declined;
	}
accepted:
	/* Both ways of accepting join the same explanation and departure. */
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 20);
	fdActorSetAnimation(ACTOR_MIA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	fdActorShowEmote(ACTOR_LORD_MCCOY, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
	fdEventSetMessageId(MsgBilibinHmmmWellGrant);
	/* "Hmmm. Well, I'll grant yeh're a brave lot." */
	fdEventShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0);
	fdActorSetSprFlags(fdActorGet(ACTOR_BARRICADE_KEY), 0);
	actor = fdActorGet(ACTOR_BARRICADE_KEY);
	actor->scaleX = FIXED_ONE / 2;
	actor->scaleY = FIXED_ONE / 2;
	actor = fdActorGet(ACTOR_LORD_MCCOY);
	if(actor != NULL) {
		fdActorSetPosFX(ACTOR_BARRICADE_KEY, actor->x.fixed, actor->z.fixed);
	}
	sySynch(1);
	fdActorJump(ACTOR_BARRICADE_KEY, 6, 0);
	fdActorSetSpeed(ACTOR_BARRICADE_KEY, 2);
	fdActorMoveToAndWait(ACTOR_BARRICADE_KEY, 894, 668);
	fdEventWait(40);
	/* "Here. This key'll get yeh past the barricades tae the east..." */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY,
									0, 10);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	fdActorSetAnimationAndWait(ACTOR_LORD_MCCOY, ANIM_SHAKE_HEAD);
	/* "Even with the key, such wee lads as yerselves'll never reach..." */
	fdEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0,
							   10);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 60);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 10);
	/* "He's making fun of us! Calling us children! That's just..." */
	fdEventOpenMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
	if(fdEventAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 1) {
		do {
			fdActorStartRepeatedMotion(ACTOR_GARET, 2);
			fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
			fdEventSetMessageId(MsgBilibinSeriousDoesntBother);
			/* "Are you serious, Isaac? It doesn't bother you?" */
			fdEventOpenMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0);
		} while(fdEventAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) != 1);
	}
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
	/* Garet gets over his anger before McCoy dismisses the party. */
	fdEventSetMessageId(MsgBilibinWellDontLet);
	/* "Well, don't let him get to you. We'll just have to prove we..." */
	syFieldEventShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0, 10);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 10);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdEventWait(20);
	fdActorShowEmote(ACTOR_LORD_MCCOY, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
	/* "So, yeh've decided to accept our little job?" */
	fdEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0,
							   20);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
	fdActorFaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTH_SOUTHEAST, 10);
	fdActorShowEmote(ACTOR_LORD_MCCOY, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	fdActorFaceDirection(ACTOR_GARET, FACING_EAST, 20);
	fdActorRunRepeatedMotion(ACTOR_GARET, 1);
	/* "Hey, Ivan! You... You don't look too enthusiastic." */
	syFieldEventShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0, 10);
	fdActorFaceDirection(ACTOR_MIA, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 20);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
	fdActorFaceDirection(ACTOR_IVAN, FACING_WEST, 10);
	/* "That curse can turn a man into a tree! You think we stand a..." */
	fdEventShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_IVAN, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_GARET, 1);
	fdEventWait(40);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
	/* "What? Yeah, of course we do!" */
	syFieldEventShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0, 10);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 10);
	fdActorStartRepeatedMotion(ACTOR_GARET, 1);
	/* "You know we do, right, Isaac?" */
	fdEventOpenMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
	while(fdEventAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) != 0) {
		fdEventSetMessageId(MsgBilibinComeSaidYoud);
		/* "Come on, Isaac! You said you'd do it! We're still going, right?" */
		fdEventOpenMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0);
	}
	fdEventWait(10);
	fdActorFaceDirection(ACTOR_MIA, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_EAST, 10);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 10);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
	fdActorFaceDirection(ACTOR_IVAN, FACING_WEST, 10);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
	fdEventSetMessageId(MsgBilibinWellSGoing);
	/* "Well, if Isaac's going, I guess I'm in, too." */
	syFieldEventShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_IVAN, 0, 20);
	fdActorRunRepeatedMotion(ACTOR_LORD_MCCOY, 1);
	fdActorFaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTH_SOUTHWEST, 10);
	fdActorSetAnimationAndWait(ACTOR_LORD_MCCOY, ANIM_SHAKE_HEAD);
	/* "At least we all agree on something!" */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY,
									0, 10);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 20);
	fdActorSetAnimation(ACTOR_MIA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	fdEventWait(40);
	fdActorShowEmote(ACTOR_LORD_MCCOY, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
	fdActorFaceDirection(ACTOR_CAPTAIN, FACING_EAST_SOUTHEAST, 10);
	fdActorSetAttachedEffect(ACTOR_CAPTAIN,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdEventWait(40);
	/* "What troubles you, Milord? They seem willing enough..." */
	fdEventShowMessageThenWait(ACTOR_CAPTAIN, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_LORD_MCCOY, 1);
	fdEventWait(40);
	fdActorFaceDirection(ACTOR_LORD_MCCOY, FACING_WEST_SOUTHWEST, 20);
	/* "They call themselves warriors...but they're naught but wee..." */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY,
									0, 10);
	fdActorSetAnimationAndWait(ACTOR_CAPTAIN, ANIM_NOD);
	fdEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_LORD_MCCOY, ANIM_SHAKE_HEAD);
	fdActorSetAnimation(ACTOR_LORD_MCCOY, ANIM_SHAKE_HEAD);
	/* "Aye, these are desperate times, but I cannae send such wee..." */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY,
									0, 10);
	fdActorJump(ACTOR_BARRICADE_KEY, 6, 0);
	actor = fdActorGet(ACTOR_LORD_MCCOY);
	if(actor != NULL) {
		fdActorSetDestination(ACTOR_BARRICADE_KEY, actor->x.part[0].integer,
							  actor->z.part[0].integer);
	}
	fdActorWaitForMoveById(ACTOR_BARRICADE_KEY);
	fdActorSetPos(ACTOR_BARRICADE_KEY, 0, 0);
	fdEventWait(20);
	fdActorSetAttachedEffect(ACTOR_MIA,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(ACTOR_PARTY_LEADER,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(ACTOR_GARET,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(ACTOR_IVAN,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdEventWait(40);
	fdActorRunRepeatedMotion(ACTOR_CAPTAIN, 2);
	/* "Then what will happen to Kolima Forest?" */
	fdEventShowMessageThenWait(ACTOR_CAPTAIN, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_LORD_MCCOY, ANIM_NOD);
	/* "We cannae give up hope on our other champions!" */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY,
									0, 20);
	fdActorSetAnimationAndWait(ACTOR_CAPTAIN, ANIM_NOD);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
	fdEventWait(20);
	/* "But... But what about us?" */
	syFieldEventShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_IVAN, 0, 10);
	fdActorFaceDirection(ACTOR_CAPTAIN, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTH_SOUTHEAST, 10);
	fdActorSetAnimationAndWait(ACTOR_LORD_MCCOY, ANIM_SHAKE_HEAD);
	/* "We cannae send yeh intae certain doom." */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY,
									0, 10);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 60);
	/* "What if we said that we were going east anyway?" */
	fdEventShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0, 10);
	fdActorFaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTH_SOUTHWEST, 10);
	fdActorSetAnimation(ACTOR_LORD_MCCOY, ANIM_SHAKE_HEAD);
	/* "Yeh can go once things have settled down." */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY,
									0, 10);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_SHAKE_HEAD);
	/* "But that means... you're not going to let us go?" */
	fdEventShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_IVAN, 0, 10);
	fdActorFaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTH_SOUTHEAST, 10);
	fdActorSetAnimationAndWait(ACTOR_LORD_MCCOY, ANIM_NOD);
	/* "Ach, it's fer yer own good, lad." */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY,
									0, 10);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_SCOWL, 60);
	fdActorFaceDirection(ACTOR_LORD_MCCOY, FACING_WEST_SOUTHWEST, 10);
	fdActorSetAnimationAndWait(ACTOR_LORD_MCCOY, ANIM_NOD);
	fdEventWait(10);
	/* "I dinnae want tae talk about it anymore. Captain?" */
	fdEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0,
							   10);
	fdActorRunRepeatedMotion(ACTOR_CAPTAIN, 2);
	fdActorFaceDirection(ACTOR_CAPTAIN, FACING_EAST_SOUTHEAST, 10);
	fdActorSetAnimationAndWait(ACTOR_CAPTAIN, ANIM_NOD);
	fdEventWait(20);
	syFieldActorWatchAndStartScript(ACTOR_PARTY_LEADER,
									ACTOR_WATCH_KEEP_MOTION | ACTOR_CAPTAIN,
									McCoyPalaceTurnTowardWatched);
	syFieldActorWatchAndStartScript(ACTOR_GARET,
									ACTOR_WATCH_KEEP_MOTION | ACTOR_CAPTAIN,
									McCoyPalaceTurnTowardWatched);
	syFieldActorWatchAndStartScript(ACTOR_IVAN,
									ACTOR_WATCH_KEEP_MOTION | ACTOR_CAPTAIN,
									McCoyPalaceTurnTowardWatched);
	syFieldActorWatchAndStartScript(ACTOR_MIA,
									ACTOR_WATCH_KEEP_MOTION | ACTOR_CAPTAIN,
									McCoyPalaceTurnTowardWatched);
	fdActorSetSpeed(ACTOR_CAPTAIN, 0.6);
	fdActorWalkToAndWait(ACTOR_CAPTAIN, 852, 646);
	fdActorWalkToAndWait(ACTOR_CAPTAIN, 852, 666);
	fdActorWalkToAndWait(ACTOR_CAPTAIN, 864, 672);
	fdActorFaceDirection(ACTOR_CAPTAIN, FACING_EAST_SOUTHEAST, 10);
	/* "I'm sorry, but you should leave now." */
	fdEventShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_CAPTAIN, 0, 20);
	fdActorStop(ACTOR_PARTY_LEADER);
	fdActorStop(ACTOR_GARET);
	fdActorStop(ACTOR_IVAN);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	syFieldActorWatchAndStartScript(
		ACTOR_CAPTAIN, ACTOR_WATCH_KEEP_MOTION | ACTOR_PARTY_LEADER,
		McCoyPalaceTurnTowardWatched);
	fdActorSetAnimation(ACTOR_GARET, ANIM_WALK);
	actor = fdActorGet(ACTOR_PARTY_LEADER);
	if(actor != NULL) {
		fdActorSetDestination(ACTOR_GARET, actor->x.part[0].integer,
							  actor->z.part[0].integer);
	}
	fdActorSetAnimation(ACTOR_IVAN, ANIM_WALK);
	actor = fdActorGet(ACTOR_PARTY_LEADER);
	if(actor != NULL) {
		fdActorSetDestination(ACTOR_IVAN, actor->x.part[0].integer,
							  actor->z.part[0].integer);
	}
	if(gmFlagGet(FLAG_IN_PARTY(MIA)) != 0) {
		fdActorSetAnimation(ACTOR_MIA, ANIM_WALK);
		actor = fdActorGet(ACTOR_PARTY_LEADER);
		if(actor != NULL) {
			fdActorSetDestination(ACTOR_MIA, actor->x.part[0].integer,
								  actor->z.part[0].integer);
		}
	}
	fdActorWaitForMoveById(ACTOR_IVAN);
	fdActorSetPos(ACTOR_GARET, 0, 0);
	fdActorSetPos(ACTOR_IVAN, 0, 0);
	fdActorSetPos(ACTOR_MIA, 0, 0);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 1);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 894, 752);
	FieldStateWork->transitionFrm = MAP_TRANSITION_FRAMES;
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	gmFlagSet(FLAG_CAPTAIN_SEEING_PARTY_OUT);
leave:
	syImportEventSetExitRequest(MCCOY_PALACE_MCCOYS_ROOM_TO_ENTRANCE_ROOM);
	fdEventEnd();
}

/* Back in the hall after the first audience, McCoy's door opens behind the */
/* party and the captain comes out. He is sorry Isaac was rejected, asks him to
 */
/* promise not to go to Kolima, lets slip "That barricade... You may not need */
/* a key.", and leaves down the red carpet. */
static void scMcCoyPalaceCaptainTalksAboutBarricade(void) {
	fdEventStart();
	fdEventStartScrTransition();
	fdEventWaitTransitionFrames();
	fdActorFaceDirection(ACTOR_CAPTAIN, FACING_SOUTH_SOUTHEAST, 0);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.6);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 256, 660);
	fdEventWait(20);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	gmFlagSet(FLAG_MCCOYS_DOOR_OPEN);
	sySndPlay(SE_THUMP);
	fdTilesetAnimResume(MCCOY_PALACE_MCCOYS_DOOR_ANIMATION);
	fdTilesetAnimResume(MCCOY_PALACE_BEDROOM_DOORS_ANIMATION);
	fdActorSetPos(ACTOR_CAPTAIN, 256, 632);
	sySynch(1);
	fdActorSetSpeed(ACTOR_CAPTAIN, 0.6);
	fdActorWalkToAndWait(ACTOR_CAPTAIN, 256, 644);
	fdTilesetAnimPause(MCCOY_PALACE_MCCOYS_DOOR_ANIMATION);
	fdTilesetAnimPause(MCCOY_PALACE_BEDROOM_DOORS_ANIMATION);
	fdEventWait(20);
	fdActorRunRepeatedMotion(ACTOR_CAPTAIN, 2);
	fdEventSetMessageId(MsgBilibinNameSorryRejected);
	/* "Your name was...Isaac? I'm sorry you were rejected." */
	fdEventShowMessageThenWait(ACTOR_CAPTAIN, 0, 10);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
					 40);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 264, 660);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
	fdActorWalkToAndWait(ACTOR_CAPTAIN, 248, 660);
	fdActorFaceDirection(ACTOR_CAPTAIN, FACING_EAST_SOUTHEAST, 40);
	fdActorSetAnimationAndWait(ACTOR_CAPTAIN, ANIM_SHAKE_HEAD);
	/* "I never, ever, expected anything like this to happen to Bilibin." */
	fdEventShowMessage(ACTOR_CAPTAIN, 0);
	fdActorSetAnimationAndWait(ACTOR_CAPTAIN, ANIM_NOD);
	/* "Will you promise me you won't go to Kolima?" */
	/* Yes: "That's good." */
	/* No: "You are persistent, aren't you?" */
	fdEventAskYesNo(ACTOR_CAPTAIN, 0);
	fdActorRunRepeatedMotion(ACTOR_CAPTAIN, 2);
	/* "That barricade... You may not need a key." */
	fdEventShowMessageThenWait(ACTOR_CAPTAIN, 0, 10);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	fdActorSetAttachedEffect(ACTOR_CAPTAIN,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdEventWait(60);
	fdActorRunRepeatedMotion(ACTOR_CAPTAIN, 1);
	/* "Oops... I shouldn't have said anything." */
	fdEventShowMessageThenWait(ACTOR_CAPTAIN, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_CAPTAIN, ANIM_NOD);
	/* "Well, I'm going to go now." */
	fdEventShowMessage(ACTOR_CAPTAIN, 0);
	fdActorSetSpeed(ACTOR_CAPTAIN, 0.8);
	fdActorWalkToAndWait(ACTOR_CAPTAIN, 248, 772);
	fdActorSetPos(ACTOR_CAPTAIN, 0, 0);
	gmFlagClear(FLAG_SHOW_AREA_NAME);
	gmFlagSet(FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY);
	fdEventEnd();
}

/* Garet, Ivan and Mia turn to talk among themselves. */
static void scMcCoyPalaceFaceCompanionsForDiscussion(void) {
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_WEST, 0);
}

/* Garet, Ivan and Mia face McCoy again. */
static void scMcCoyPalaceFaceCompanionsForAudience(void) {
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTHWEST, 0);
}

/* Once Kolima's curse is lifted, the guards march the party into McCoy's room
 */
/* as if under arrest. McCoy thanks Isaac, explains he only feared they would */
/* not stop by on the way back, and has the captain take them down to the */
/* treasure room to choose a reward. */
static void scMcCoyPalaceMcCoyRewardsParty(void) {
	ACTOR *viewCenter;
	ACTOR *actor;
	s32 motionFlags = 0;
	s32 advanceAlternateReply;

	fdEventStart();
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	sySynch(1);
	viewCenter = fdCameraGetActor();
	viewCenter->motionFlg = motionFlags;
	fdCameraMoveTo(894, CAMERA_KEEP, 664, 0);
	sySynch(1);
	fdActorSetPos(ACTOR_PARTY_LEADER, 0, 0);
	fdMapGridCenterOnFollowedPoint();
	sySynch(1);

	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
	FieldStateWork->transitionFrm = MAP_TRANSITION_FRAMES;

	fdEventStartScrTransition();
	fdEventWaitTransitionFrames();
	fdEventWait(40);
	fdActorSetPos(ACTOR_CAPTAIN, 888, 798);
	fdActorSetPos(ACTOR_PARTY_LEADER, 904, 798);
	fdCameraSetSpeed(0.6);
	fdCameraMoveTo(894, CAMERA_KEEP, 698, 1);
	fdActorSetSpeed(ACTOR_CAPTAIN, 0.8);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.6);
	fdActorWalkTo(ACTOR_CAPTAIN, 888, 720);
	fdActorWalkTo(ACTOR_PARTY_LEADER, 904, 736);
	fdEventWait(60);
	fdActorWaitForMoveById(ACTOR_CAPTAIN);
	fdActorSetAnimation(ACTOR_CAPTAIN, ANIM_STAND);
	fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	fdEventWait(20);
	fdActorRunRepeatedMotion(ACTOR_CAPTAIN, 2);
	/* Each line of this audience is written twice, the first for a party McCoy
	 */
	/* turned away before (FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY), the second
	 */
	/* for one he meets for the first time; the event steps over the other. Here
	 */
	/* "We've brought the warriors, milord." or "We brought the warriors, as you
	 */
	/* ordered, milord." */
	fdEventSetMessageId(MsgBilibinWeveBroughtWarriorsMilord);
	advanceAlternateReply = 1;
	if(fdEventGetFlag(FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY) == 0) {
		FieldStateWork->msg++;
		advanceAlternateReply = 0;
	}
	fdEventShowMessage(ACTOR_CAPTAIN, 0);
	if(advanceAlternateReply != 0) {
		FieldStateWork->msg++;
	}

	fdCameraMoveTo(894, CAMERA_KEEP, 664, 1);
	fdActorSetScript(ACTOR_CAPTAIN, McCoyPalaceCaptainStepsBesideMcCoy);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 894, 684);

	actor = fdActorGet(ACTOR_PARTY_LEADER);
	if(actor != NULL) {
		fdActorSetPosFX(ACTOR_GARET, actor->x.fixed, actor->z.fixed);
	}
	actor = fdActorGet(ACTOR_PARTY_LEADER);
	if(actor != NULL) {
		fdActorSetPosFX(ACTOR_IVAN, actor->x.fixed, actor->z.fixed);
	}
	actor = fdActorGet(ACTOR_PARTY_LEADER);
	if(actor != NULL) {
		fdActorSetPosFX(ACTOR_MIA, actor->x.fixed, actor->z.fixed);
	}

	scMcCoyPalacePrepareCompanionApproach();
	fdActorSetDestinationOffset(ACTOR_GARET, -16, 16);
	fdActorSetDestinationOffset(ACTOR_IVAN, 16, 16);
	fdActorSetDestinationOffset(ACTOR_MIA, 32, 16);
	fdActorWaitForMoveById(ACTOR_IVAN);
	fdActorSetAnimation(ACTOR_GARET, ANIM_STAND);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_STAND);
	fdActorSetAnimation(ACTOR_MIA, ANIM_STAND);
	fdEventWait(10);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	fdActorWaitForMoveById(ACTOR_MIA);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTHWEST, 0);
	fdActorWaitForScriptEndById(ACTOR_CAPTAIN);
	fdEventWait(20);

	advanceAlternateReply = 1;
	if(fdEventGetFlag(FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY) == 0) {
		FieldStateWork->msg++;
		advanceAlternateReply = 0;
	}
	fdActorRunRepeatedMotion(ACTOR_LORD_MCCOY, 3);
	/* "So, yeh're the wee--I mean, brave--lads I met before!" or "...lads who
	 */
	/* saved Kolima..." */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY,
									0, 20);
	if(advanceAlternateReply != 0) {
		FieldStateWork->msg++;
	}

	advanceAlternateReply = 1;
	if(fdEventGetFlag(FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY) == 0) {
		FieldStateWork->msg++;
		advanceAlternateReply = 0;
	}
	fdActorRunRepeatedMotion(ACTOR_LORD_MCCOY, 1);
	/* "What were yer names, again?" or "What are yer names, then?" */
	fdEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0,
							   10);
	if(advanceAlternateReply != 0) {
		FieldStateWork->msg++;
	}

	scMcCoyPalaceFaceCompanionsForDiscussion();
	fdEventWait(20);
	if(gmFlagGet(FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY) != 0) {
		fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
		fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
	}
	else {
		fdEventWait(40);
	}

	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 10);
	/* "He's Isaac, sir, and I'm Garet..." */
	fdEventShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0, 10);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 10);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	/* "I am called Ivan." */
	fdEventShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_IVAN, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTHWEST, 10);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
	/* "And I am Mia, from the village of Imil." */
	fdEventShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_MIA, 0, 20);
	fdActorSetAnimationAndWait(ACTOR_LORD_MCCOY, ANIM_NOD);
	fdEventWait(20);
	/* "Isaac and company..." */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY,
									0, 20);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);

	if(gmFlagGet(FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY) != 0) {
		fdActorRunRepeatedMotion(ACTOR_LORD_MCCOY, 1);
		fdActorSetAnimationAndWait(ACTOR_LORD_MCCOY, ANIM_SHAKE_HEAD);
		/* "And did I not decline yer offer when we first met?" */
		/* Yes: "I did nae give it enough consideration back then." */
		/* No: "Are yeh no' just sayin' that tae appease me?" */
		fdEventOpenMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0);
		scMcCoyPalaceFaceCompanionsForDiscussion();
		advanceAlternateReply = 1;
		if(fdEventAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) != 0) {
			FieldStateWork->msg++;
			advanceAlternateReply = 0;
		}
		fdActorFaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTH_SOUTHWEST, 0);
		scMcCoyPalaceFaceCompanionsForAudience();
		fdEventWait(10);
		fdEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0,
								   10);
		if(advanceAlternateReply != 0) {
			FieldStateWork->msg++;
		}
		fdActorShowEmote(ACTOR_LORD_MCCOY, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
	}
	else {
		FieldStateWork->msg += 4;
	}

	/* "And yet yeh went oan tae break the curse despite my words, did yeh not?"
	 */
	/* for a party he turned away, else "Are yeh no' the ones who ended the
	 * curse */
	/* of the holy tree?" */
	/* Yes: "I humbly thank you." */
	/* No: "Nae, yeh dinnae need tae hide it. My men told me aught yeh did." */
	fdEventOpenMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0);
	scMcCoyPalaceFaceCompanionsForDiscussion();
	if(fdEventAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
		fdEventSetMessageId(MsgBilibinHumblyThank);
	}
	else {
		fdEventSetMessageId(MsgBilibinNaeYehDinnaeNeedTae);
	}

	scMcCoyPalaceFaceCompanionsForAudience();
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY,
									0, 20);
	fdActorRunRepeatedMotion(ACTOR_CAPTAIN, 1);
	/* McCoy explains his caution and offers a choice from the treasury. His */
	/* first line names the captain as its speaker, so it shows without McCoy's
	 */
	/* portrait. */
	fdEventSetMessageId(MsgBilibinWasButWorriedYehMight);
	/* "I was but worried yeh might nae stop here on yer way back." */
	fdEventShowMessageThenWait(ACTOR_CAPTAIN, 0, 10);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_WEST, 40);
	fdActorRunRepeatedMotion(ACTOR_LORD_MCCOY, 2);
	/* "I want tae show my appreciation for yer help..." */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY,
									0, 10);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	scMcCoyPalaceFaceCompanionsForAudience();
	fdEventWait(10);
	fdActorShowEmote(ACTOR_LORD_MCCOY, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
	/* "I relied oan my money, but I forgot about my people's needs." */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY,
									0, 10);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
	fdEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_LORD_MCCOY, ANIM_NOD);
	/* "Yeh reminded me of my duties." */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY,
									0, 10);
	fdActorShowEmote(ACTOR_LORD_MCCOY, EMOTE_IN_FRONT | EMOTE_SMILE, 60);
	/* "As lord of these lands, I will do my best for everyone's behalf." */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY,
									0, 10);
	fdActorFaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTH_SOUTHEAST, 10);
	/* "Isaac, it would honor me greatly if yeh'd accept a wee token of my */
	/* gratitude." */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY,
									0, 10);
	fdActorSetAnimationAndWait(ACTOR_LORD_MCCOY, ANIM_NOD);
	/* "I could give it tae yeh here, but would yeh no' rather choose it */
	/* yerself?" */
	/* Yes: "Very well. Yeh can choose somethin' that'll help yeh along yer
	 * way." */
	/* No: "Words are nae enough tae express my regret in doubting yeh. Please,
	 */
	/* choose a token in thanks..." */
	fdEventOpenMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0);

	scMcCoyPalaceFaceCompanionsForDiscussion();
	advanceAlternateReply = 1;
	if(fdEventAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 1) {
		FieldStateWork->msg++;
		advanceAlternateReply = 0;
	}
	scMcCoyPalaceFaceCompanionsForAudience();
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY,
									0, 10);
	if(advanceAlternateReply != 0) {
		FieldStateWork->msg++;
	}

	fdActorFaceDirection(ACTOR_LORD_MCCOY, FACING_WEST_SOUTHWEST, 10);
	fdActorRunRepeatedMotion(ACTOR_CAPTAIN, 1);
	fdActorFaceDirection(ACTOR_CAPTAIN, FACING_EAST_SOUTHEAST, 20);
	fdActorSetAnimationAndWait(ACTOR_LORD_MCCOY, ANIM_NOD);
	fdEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_CAPTAIN, ANIM_NOD);
	/* "I will show Isaac and his friends the way, milord." */
	fdEventShowMessageThenWait(ACTOR_CAPTAIN, 0, 10);
	fdActorFaceDirection(ACTOR_CAPTAIN, FACING_SOUTH_SOUTHEAST, 10);
	fdActorFaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTH_SOUTHEAST, 20);
	fdActorRunRepeatedMotion(ACTOR_LORD_MCCOY, 1);
	/* "Many thanks tae yeh, and tae yer companions, as well!!" */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY,
									0, 10);
	fdActorSetAnimationAndWait(ACTOR_LORD_MCCOY, ANIM_NOD);
	/* "Visit me anytime! Yeh'll always find a warm hearth here!" */
	fdEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0,
							   10);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
	fdEventWait(20);

	fdActorSetAnimation(ACTOR_GARET, ANIM_WALK);
	actor = fdActorGet(ACTOR_PARTY_LEADER);
	if(actor != NULL) {
		fdActorSetDestination(ACTOR_GARET, actor->x.part[0].integer,
							  actor->z.part[0].integer);
	}
	fdActorSetAnimation(ACTOR_IVAN, ANIM_WALK);
	actor = fdActorGet(ACTOR_PARTY_LEADER);
	if(actor != NULL) {
		fdActorSetDestination(ACTOR_IVAN, actor->x.part[0].integer,
							  actor->z.part[0].integer);
	}
	fdActorSetAnimation(ACTOR_MIA, ANIM_WALK);
	actor = fdActorGet(ACTOR_PARTY_LEADER);
	if(actor != NULL) {
		fdActorSetDestination(ACTOR_MIA, actor->x.part[0].integer,
							  actor->z.part[0].integer);
	}

	fdActorWaitForMoveById(ACTOR_GARET);
	fdActorSetPos(ACTOR_GARET, 0, 0);
	fdActorSetPos(ACTOR_IVAN, 0, 0);
	fdActorWaitForMoveById(ACTOR_MIA);
	fdActorSetPos(ACTOR_MIA, 0, 0);
	fdEventWait(20);
	fdActorFaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTH_SOUTHWEST, 0);
	syFieldActorWatchAndStartScript(ACTOR_PARTY_LEADER,
									ACTOR_WATCH_KEEP_MOTION | ACTOR_CAPTAIN,
									McCoyPalaceTurnTowardWatched);
	fdActorWalkToAndWait(ACTOR_CAPTAIN, 852, 646);
	fdActorWalkToAndWait(ACTOR_CAPTAIN, 852, 666);
	fdActorWalkToAndWait(ACTOR_CAPTAIN, 864, 672);
	fdActorFaceDirection(ACTOR_CAPTAIN, FACING_EAST_SOUTHEAST, 10);
	fdActorRunRepeatedMotion(ACTOR_CAPTAIN, 1);
	fdEventWait(10);
	/* "Follow me, and I'll take you to the treasure room." */
	fdEventShowMessageThenWait(ACTOR_CAPTAIN, 0, 10);
	fdActorWalkToAndWait(ACTOR_CAPTAIN, 886, 708);
	fdActorWalkTo(ACTOR_CAPTAIN, 894, 764);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 894, 764);
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	gmFlagSet(FLAG_REWARD_OFFERED);
	if(fdEventGetFlag(FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY) == 0) {
		gmFlagSet(FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY);
		gmFlagSet(FLAG_MCCOY_PALACE_ADMITTED);
	}
	syImportEventSetExitRequest(MCCOY_PALACE_ENTRANCE_ROOM_TO_TREASURE_ROOM);
	fdEventEnd();
}

/* In the treasure room, the captain walks the party up to four chests in a */
/* row before the barred gate. He tells the soldier at the gate that Lord */
/* McCoy ordered Isaac and his companions brought here, and the soldier says */
/* "You may select a single item from among these four chests." */
static void scMcCoyPalacePresentRewardChests(void) {
	fdEventStart();
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdCameraGetActor()->motionFlg = 0;
	fdCameraMoveTo(628, CAMERA_KEEP, 748, 0);
	scMcCoyPalaceOpenRewardChestRow();
	fdActorGet(ACTOR_PARTY_LEADER)->facing = FACING_EAST;
	fdActorSetPos(ACTOR_PARTY_LEADER, 577, 760);
	fdActorGet(ACTOR_CAPTAIN)->facing = FACING_EAST;
	fdActorSetPos(ACTOR_CAPTAIN, 592, 760);
	fdActorGet(ACTOR_SOLDIER_SHOWING_THE_REWARD)->facing =
		FACING_WEST_NORTHWEST;
	fdActorSetPos(ACTOR_SOLDIER_SHOWING_THE_REWARD, 662, 764);
	scMcCoyPalacePosRewardChests();
	fdActorSetSprFlags(fdActorGet(ACTOR_VIAL_CHEST), 0);
	fdActorSetSprFlags(fdActorGet(ACTOR_POTION_CHEST), 0);
	fdActorSetSprFlags(fdActorGet(ACTOR_PSY_CRYSTAL_CHEST), 0);
	fdActorSetSprFlags(fdActorGet(ACTOR_WATER_OF_LIFE_CHEST), 0);
	fdActorGet(ACTOR_VIAL_CHEST)->motionFlg = 0;
	fdActorGet(ACTOR_POTION_CHEST)->motionFlg = 0;
	fdActorGet(ACTOR_PSY_CRYSTAL_CHEST)->motionFlg = 0;
	fdActorGet(ACTOR_WATER_OF_LIFE_CHEST)->motionFlg = 0;
	fdActorGet(ACTOR_VIAL_CHEST)->y.fixed = -toFIXED(4);
	fdActorGet(ACTOR_POTION_CHEST)->y.fixed = -toFIXED(4);
	fdActorGet(ACTOR_PSY_CRYSTAL_CHEST)->y.fixed = -toFIXED(4);
	fdActorGet(ACTOR_WATER_OF_LIFE_CHEST)->y.fixed = -toFIXED(4);
	fdMapGridCenterOnFollowedPoint();
	sySynch(1);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
	FieldStateWork->transitionFrm = MAP_TRANSITION_FRAMES;
	fdEventStartScrTransition();
	fdEventWaitTransitionFrames();
	fdActorSetSpeed(ACTOR_CAPTAIN, 0.6);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.6);
	fdActorWalkTo(ACTOR_CAPTAIN, 628, 764);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 612, 764);
	fdActorSetAnimation(ACTOR_CAPTAIN, ANIM_STAND);
	fdEventWait(20);
	fdActorRunRepeatedMotion(ACTOR_CAPTAIN, 1);
	/* The captain and the soldier at the gate explain that only one chest may
	 */
	/* be chosen. */
	fdEventSetMessageId(MsgBilibinLordMccoyOrdered);
	/* "Lord McCoy ordered that Isaac and his companions be brought..." */
	fdEventShowMessageThenWait(ACTOR_CAPTAIN, 0, 10);
	fdActorWalkToAndWait(ACTOR_CAPTAIN, 622, 780);
	fdActorFaceDirection(ACTOR_CAPTAIN, FACING_NORTH, 10);
	fdActorRunRepeatedMotion(ACTOR_SOLDIER_SHOWING_THE_REWARD, 2);
	/* "You may select a single item from among these four chests." */
	fdEventShowMessageThenWait(ACTOR_SOLDIER_SHOWING_THE_REWARD, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	gmFlagClear(FLAG_SHOW_AREA_NAME);
	gmFlagSet(FLAG_CHOOSING_REWARD);
	fdEventEnd();
}

/* Keeps turning the actor toward the one it watches. */
s32 McCoyPalaceTurnTowardWatched[] = {
	SC_CALL(scTurnTowardTarget),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

/* The captain walks up beside McCoy and turns south. */
s32 McCoyPalaceCaptainStepsBesideMcCoy[] = {
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

SCENE_ENTRANCE McCoyPalaceEntrances[] = {
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

u32 McCoyPalaceExits[] = {
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

/* Unused: the getters return it for any scene but SceneId_McCoyPalace, and the
 */
/* overlay serves no other. */
ACTOR_PLACE McCoyPalaceNoActorPlacements[] = {
	{SCENE_TABLE_END},
};

/* clang-format off */
ACTOR_PLACE McCoyPalaceActorPlacements[] = {
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

/* Unused, as McCoyPalaceNoActorPlacements is. */
TRIGGER McCoyPalaceNoTriggers[] = {
    { TRIGGER_TABLE_END },
};

TRIGGER McCoyPalaceTriggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, MCCOY_PALACE_TO_MCCOY_PALACE_GROUNDS_WEST },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, MCCOY_PALACE_ENTRANCE_ROOM_TO_SOUTHWEST_LANDING },
    { TRIGGER_EXIT, 3, CONDITION_ALWAYS, MCCOY_PALACE_ENTRANCE_ROOM_TO_SOUTHEAST_LANDING },
    { TRIGGER_EXIT, 4, CONDITION_ALWAYS, MCCOY_PALACE_ENTRANCE_ROOM_TO_LADY_MCCOYS_ROOM_WEST },
    { TRIGGER_EXIT, 5, CONDITION_ALWAYS, MCCOY_PALACE_ENTRANCE_ROOM_TO_LADY_MCCOYS_ROOM_EAST },
    { TRIGGER_EXIT, 6, CONDITION_ALWAYS, MCCOY_PALACE_ENTRANCE_ROOM_TO_TREASURE_ROOM },
    { TRIGGER_EXIT, 7, CONDITION_ALWAYS, MCCOY_PALACE_ENTRANCE_ROOM_TO_NORTHWEST_LANDING },
    { TRIGGER_EXIT, 8, CONDITION_ALWAYS, MCCOY_PALACE_ENTRANCE_ROOM_TO_NORTHEAST_LANDING },
    { STEP_TRIGGER_FACING(FACING_NORTH), MCCOY_PALACE_ENTRANCE_ROOM_TO_MCCOYS_ROOM, CONDITION_ALWAYS, EVENT(scMcCoyPalaceWalkThroughDoor) },
    { TRIGGER_EXIT, 10, CONDITION_ALWAYS, MCCOY_PALACE_LADY_MCCOYS_ROOM_TO_ENTRANCE_ROOM_WEST },
    { TRIGGER_EXIT, 11, CONDITION_ALWAYS, MCCOY_PALACE_LADY_MCCOYS_ROOM_TO_ENTRANCE_ROOM_EAST },
    { STEP_TRIGGER_FACING(FACING_NORTH), MCCOY_PALACE_LADY_MCCOYS_ROOM_TO_WEST_BEDROOM, CONDITION_ALWAYS, EVENT(scMcCoyPalaceWalkThroughDoor) },
    { STEP_TRIGGER_FACING(FACING_NORTH), MCCOY_PALACE_LADY_MCCOYS_ROOM_TO_EAST_BEDROOM, CONDITION_ALWAYS, EVENT(scMcCoyPalaceWalkThroughDoor) },
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
    { TRIGGER_TALK, ACTOR_VILLAGER_ASKING_ABOUT_KOLIMA_FOREST, FLAG_MCCOY_PALACE_REWARD_TAKEN, EVENT(scMcCoyPalaceAskAboutGoingToKolima) },
    { TRIGGER_TALK, ACTOR_VILLAGER_ASKING_ABOUT_KOLIMA_FOREST, CONDITION_ALWAYS, MSGNO(MsgBilibinLadyMccoyIsQuiteUpsetThat) },
    { TRIGGER_TALK, ACTOR_YOUNG_MAN_PROTECT_OLD_TREE, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinIAlwaysBelievedThatTheOld) },
    { TRIGGER_TALK, ACTOR_YOUNG_MAN_PROTECT_OLD_TREE, CONDITION_ALWAYS, MSGNO(MsgBilibinThingsAreLookingGrimSouthOf) },
    { TRIGGER_TALK, ACTOR_VILLAGER_LADY_MCCOYS_ROOM, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinThisIsLadyMccoysRoomSo) },
    { TRIGGER_TALK, ACTOR_VILLAGER_LADY_MCCOYS_ROOM, CONDITION_ALWAYS, MSGNO(MsgBilibinMiladyWasVeryUpsetWhenShe) },
    { TRIGGER_TALK, ACTOR_VILLAGER_MILADYS_BEAUTY, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinMiladyWasRenownedFarAndWide) },
    { TRIGGER_TALK, ACTOR_VILLAGER_MILADYS_BEAUTY, CONDITION_ALWAYS, MSGNO(MsgBilibinForOnceMilordDidntLetLady) },
    { TRIGGER_TALK, ACTOR_VILLAGER_IN_MILADYS_NIGHTGOWN, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinOhSomedayIWantToSleep) },
    { TRIGGER_TALK, ACTOR_VILLAGER_IN_MILADYS_NIGHTGOWN, CONDITION_ALWAYS, EVENT(scMcCoyPalaceTalkToWomanInMiladysNightgown) },
    { TRIGGER_TALK, ACTOR_OLD_MAN_PRAISING_THE_WARRIORS, FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY, MSGNO(MsgBilibinYouCanSeeKolimaPrettyWell) },
    { TRIGGER_TALK, ACTOR_OLD_MAN_PRAISING_THE_WARRIORS, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinHeyIfYouCantDoThe) },
    { TRIGGER_TALK, ACTOR_OLD_MAN_PRAISING_THE_WARRIORS, CONDITION_ALWAYS, EVENT(scMcCoyPalaceTalkToOldManPraisingTheWarriors) },
    { TRIGGER_TALK, ACTOR_SOLDIER_WATCHING_KOLIMA, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinHmmItSeemsTheCurseOf) },
    { TRIGGER_TALK, ACTOR_SOLDIER_WATCHING_KOLIMA, CONDITION_ALWAYS, EVENT(scMcCoyPalaceTalkToSoldierWatchingKolima) },
    { TRIGGER_TALK, ACTOR_CAPTAIN, CONDITION_ALWAYS, MSGNO(MsgBilibinYouMaySelectASingleItem) },
    { TRIGGER_TALK, ACTOR_SOLDIER_SHOWING_THE_REWARD, CONDITION_ALWAYS, EVENT(scMcCoyPalaceTalkToSoldierShowingTheReward) },
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
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_PRAISING_THE_WARRIORS, CONDITION_ALWAYS, EVENT(scMcCoyPalaceReadOldMansThoughtsOnTheWarriors) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SOLDIER_WATCHING_KOLIMA, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinKolimaForestUsedToBeSo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SOLDIER_WATCHING_KOLIMA, CONDITION_ALWAYS, MSGNO(MsgBilibinACourageousWarriorWouldntThinkTwice) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CAPTAIN, CONDITION_ALWAYS, MSGNO(MsgBilibinTheseCourageousWarriorsDeserveToReceive) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SOLDIER_SHOWING_THE_REWARD, CONDITION_ALWAYS, EVENT(scMcCoyPalaceReadRewardSoldiersThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LORD_MCCOY, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinThereIsNothingMoreToTalk) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LORD_MCCOY, CONDITION_ALWAYS, MSGNO(MsgBilibinButICannaeFiggerOutHow) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_MCCOY_PALACE_VIAL_CHEST, CONDITION_ALWAYS, EVENT(scMcCoyPalaceSearchVialChest) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_MCCOY_PALACE_POTION_CHEST, CONDITION_ALWAYS, EVENT(scMcCoyPalaceSearchPotionChest) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_MCCOY_PALACE_PSY_CRYSTAL_CHEST, CONDITION_ALWAYS, EVENT(scMcCoyPalaceSearchPsyCrystalChest) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_MCCOY_PALACE_WATER_OF_LIFE_CHEST, CONDITION_ALWAYS, EVENT(scMcCoyPalaceSearchWaterOfLifeChest) },
    { TRIGGER_STEP, CELL_BEHAVIOR_MCCOY_PALACE_BEFORE_REWARD_CHESTS, FLAG_PARTY_SEEN_OFF, EVENT(scMcCoyPalaceCaptainSeesPartyOff) },
    { TRIGGER_STEP, CELL_BEHAVIOR_MCCOY_PALACE_FOOT_OF_TREASURE_STAIRS, FLAG_PARTY_SEEN_OFF, EVENT(scMcCoyPalaceCaptainAsksPartyToChoose) },
    { EXAMINE_TRIGGER(EXAMINE_BARREL), CELL_BEHAVIOR_MCCOY_PALACE_ELIXIR_BARREL, FLAG_MCCOY_PALACE_BARREL_ELIXIR, FIND_ITEM(ITEM_ELIXIR) },
    { EXAMINE_TRIGGER(EXAMINE_BARREL), CELL_BEHAVIOR_MCCOY_PALACE_SMOKE_BOMB_BARREL, FLAG_MCCOY_PALACE_BARREL_SMOKE_BOMB, FIND_ITEM(ITEM_SMOKE_BOMB) },
    { EXAMINE_TRIGGER(EXAMINE_WARDROBE), CELL_BEHAVIOR_MCCOY_PALACE_WARDROBE, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBilibinTheresASilkNegligeeIsaacPut)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), CELL_BEHAVIOR_MCCOY_PALACE_BOOKCASE, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBilibinTheresASheetOfPaperTitled)) },
    { TRIGGER_TABLE_END },
};
/* clang-format on */
