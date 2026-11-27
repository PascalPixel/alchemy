// McCoy's Palace in Bilibin, the home of the town's chief.
//
// The palace is one scene, SceneIdMcCoyPalace, sharing the area name McCoy's Palace with its
// grounds (mccoy_palace_grounds.c). Its rooms are side by side on one map, each in its own camera
// box: the entrance hall with its red carpet, McCoy's room at the carpet's top, Lady McCoy's room
// with a bedroom through each of its two back doors, the landings and small tower rooms at the
// hall's four corners, and the treasure room below, reached from the west of the hall.
//
// The party first comes in by the front door once the east guard on the grounds lets it in. A man
// in the hall says to follow the red carpet, and walking through the door at its top
// (scMcCoyPalaceWalkThroughDoor) brings Isaac before McCoy, a stout red-haired man who speaks with
// a heavy accent and a portrait. Before Kolima's curse is lifted this is the first audience
// (scMcCoyPalaceAudienceWithMcCoy): McCoy asks whether the party will save Kolima. If Isaac says
// no, he is sent away and can come back to change his mind. If he says yes, McCoy tosses him the
// key to the barricades, then takes it back, deciding they are too young, and his captain sees the
// party out. Outside his door the captain is sorry, and lets slip that the barricade on the way to
// Kolima may not need a key (scMcCoyPalaceCaptainTalksAboutBarricade); from then on the people
// speak as if the party had been turned away (FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY).
//
// Once the curse is lifted, the guards on the grounds march the party straight into McCoy's room,
// where McCoy thanks it (scMcCoyPalaceMcCoyRewardsParty). His lines differ as the party has met him
// before or not. The captain then leads it down to the treasure room, where four chests stand in a
// row before the barred gate (scMcCoyPalacePresentRewardChests): from west to east a Vial, a
// Potion, a Psy Crystal and a Water of Life. Only one may be taken (scMcCoyPalaceClaimReward); the
// others are then locked. Heading back up the stairs without a reward, the captain stops Isaac and
// sends him back to the chests; once he has one, the captain bids the party farewell. When the
// party comes back later, the four chests are gone, and which one it opened was only ever kept in
// the scene's temporary flags (FLAG_TOOK_VIAL and the others).
//
// The palace's people follow three story flags, FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY,
// FLAG_KOLIMA_CURSE_LIFTED and FLAG_MCCOY_PALACE_REWARD_TAKEN, which decide what they say and how
// they turn when talked to (scGetActorPlacements).
//
// A barrel by the chimney in the hall holds an Elixir and one in the small room at the south-west
// corner a Smoke Bomb. The wardrobe in the east bedroom holds Lady McCoy's silk negligee, and the
// bookcase in the west bedroom the long list of gifts McCoy has bought her.

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/actor_script.h"
#include "field/script_inline.h"
#include "graphics/graphics.h"
#include "text/msg_ids.h"

extern Sint32         McCoyPalaceTurnTowardWatched[];
extern Sint32         McCoyPalaceCaptainStepsBesideMcCoy[];
extern SCENE_ENTRANCE McCoyPalaceEntrances[];
extern Uint32         McCoyPalaceExits[];
extern ACTOR_PLACE    McCoyPalaceNoActorPlacements[];
extern ACTOR_PLACE    McCoyPalaceActorPlacements[];
extern CUE            McCoyPalaceNoCues[];
extern CUE            McCoyPalaceCues[];

// The palace's people and the treasure room's chests, numbering McCoyPalaceActorPlacements' rows
// from ACTOR_FIRST_PLACED. Each note gives what they say before the reward, then after it.
typedef enum MC_COY_PALACE_ACTOR_ID
{
	// A burly grey-haired man who points the way to McCoy. "If you want to see Lord McCoy, follow
	// the red carpet." Later: "The people of Bilibin will never forget your bravery."
	ACTOR_VILLAGER_FOLLOW_RED_CARPET = ACTOR_FIRST_PLACED,
	// A woman in a green dress who warns the party about McCoy. "Make sure you don't offend Lord
	// McCoy!" Later: "I heard you received a spectacular reward."
	ACTOR_VILLAGER_DO_NOT_OFFEND_MCCOY,
	// A woman with fair hair in the hall who asks whether the party is willing to go to Kolima
	// Forest. Later: "Lady McCoy is quite upset that the palace construction was halted."
	ACTOR_VILLAGER_ASKING_ABOUT_KOLIMA_FOREST,
	// A young man with brown hair who speaks of the old tree in Kolima Forest. Later: "Things are
	// looking grim south of Goma Range..."
	ACTOR_YOUNG_MAN_PROTECT_OLD_TREE,
	// A woman with fair hair standing in Lady McCoy's room. "This is Lady McCoy's room, so don't
	// touch anything." Later: "Milady was very upset when she left the room."
	ACTOR_VILLAGER_LADY_MCCOYS_ROOM,
	// A woman with fair hair who admires Lady McCoy. "Milady was renowned far and wide for her
	// beauty!" Later: "For once, milord didn't let Lady McCoy push him around!"
	ACTOR_VILLAGER_MILADYS_BEAUTY,
	// A woman with fair hair who screams when spoken to ("AAAAH!!!"). Mind Read: "Oh, no! This
	// nightgown has a hole... Milady will be furious!"
	ACTOR_VILLAGER_IN_MILADYS_NIGHTGOWN,
	// An old man with a cane in the north-east tower room. "You can see Kolima pretty well from
	// here." Later he praises the party.
	ACTOR_OLD_MAN_PRAISING_THE_WARRIORS,
	// A soldier in a spiked helmet in the south-east tower room. "Hmm. It seems the curse of the
	// tree has not yet come to Bilibin." Later he says he watched the party go to Kolima.
	ACTOR_SOLDIER_WATCHING_KOLIMA,
	// A soldier in a spiked helmet who guards the treasure room's barred gate, and shows the party
	// the four chests when the captain brings it down.
	ACTOR_SOLDIER_SHOWING_THE_REWARD,
	// McCoy, the chief of Bilibin, a stout man with red hair in a pink tunic.
	ACTOR_LORD_MCCOY,
	// The captain of McCoy's soldiers, drawn as they are, in a spiked helmet.
	ACTOR_CAPTAIN,
	// The key McCoy tosses to the party.
	ACTOR_BARRICADE_KEY,
	ACTOR_VIAL_CHEST,
	ACTOR_POTION_CHEST,
	ACTOR_PSY_CRYSTAL_CHEST,
	ACTOR_WATER_OF_LIFE_CHEST
} MC_COY_PALACE_ACTOR_ID;

// The metatiles the cues answer besides the exits, whose metatile behaviors are their exits'
// numbers.
typedef enum MC_COY_PALACE_METATILE_BEHAVIOR
{
	// The row just in front of the four reward chests, metatiles (38, 46) to (41, 46), copied there
	// from (42, 55) to (45, 55) when the chests are brought out.
	METATILE_BEHAVIOR_MCCOY_PALACE_BEFORE_REWARD_CHESTS    = 50,
	// The treasure room's floor at the foot of its stairs, metatile (36, 47), one step before the
	// way out.
	METATILE_BEHAVIOR_MCCOY_PALACE_FOOT_OF_TREASURE_STAIRS = 51,
	// The four reward chests' metatiles, (38, 45) to (41, 45) from west to east, copied there from
	// (38, 55) to (41, 55) when the chests are brought out.
	METATILE_BEHAVIOR_MCCOY_PALACE_VIAL_CHEST              = 100,
	METATILE_BEHAVIOR_MCCOY_PALACE_POTION_CHEST            = 101,
	METATILE_BEHAVIOR_MCCOY_PALACE_PSY_CRYSTAL_CHEST       = 102,
	METATILE_BEHAVIOR_MCCOY_PALACE_WATER_OF_LIFE_CHEST     = 103,
	// The barrel by the chimney in the hall, metatile (23, 40), with an Elixir.
	METATILE_BEHAVIOR_MCCOY_PALACE_ELIXIR_BARREL           = 120,
	// The barrel in the small room at the south-west corner, metatile (45, 29), with a Smoke Bomb.
	METATILE_BEHAVIOR_MCCOY_PALACE_SMOKE_BOMB_BARREL       = 121,
	// The east bedroom's wardrobe, metatiles (17, 3) to (18, 4), with Lady McCoy's silk negligee.
	METATILE_BEHAVIOR_MCCOY_PALACE_WARDROBE                = 200,
	// The west bedroom's bookcase, metatiles (9, 3) to (10, 4), with the list of McCoy's gifts to
	// his wife.
	METATILE_BEHAVIOR_MCCOY_PALACE_BOOKCASE                = 201
} MC_COY_PALACE_METATILE_BEHAVIOR;

// The palace's doors swing open by the map's tileset animations, which each scene entry pauses so
// every door is drawn shut; walking through any door runs both a moment.
typedef enum MC_COY_PALACE_DOOR_ANIMATION
{
	// McCoy's door at the top of the red carpet, four tiles across.
	MCCOY_PALACE_MCCOYS_DOOR_ANIMATION   = 1,
	// The two bedroom doors in Lady McCoy's room, two tiles across.
	MCCOY_PALACE_BEDROOM_DOORS_ANIMATION = 2
} MC_COY_PALACE_DOOR_ANIMATION;

// The palace's temporary flags. The scene temp flags last until the party leaves the palace's
// scene, the area temp flags until it leaves McCoy's Palace.
enum
{
	// The captain has opened McCoy's door, so walking through it is silent.
	FLAG_MCCOYS_DOOR_OPEN         = FLAG_SCENE_TEMP_00,
	// The party is choosing its reward among the four chests.
	FLAG_CHOOSING_REWARD          = FLAG_SCENE_TEMP_02,
	// The chest the reward was taken from, which then searches as empty.
	FLAG_TOOK_VIAL                = FLAG_SCENE_TEMP_10,
	FLAG_TOOK_POTION              = FLAG_SCENE_TEMP_11,
	FLAG_TOOK_PSY_CRYSTAL         = FLAG_SCENE_TEMP_12,
	FLAG_TOOK_WATER_OF_LIFE       = FLAG_SCENE_TEMP_13,
	// The old man has praised the party once. He then warns of the holy tree.
	FLAG_OLD_MAN_HAS_PRAISED      = FLAG_AREA_TEMP_02,
	// Mind Read has heard the old man's first thought.
	FLAG_OLD_MAN_THOUGHTS_READ    = FLAG_AREA_TEMP_03,
	// McCoy has had the captain see the party out after the first audience.
	FLAG_CAPTAIN_SEEING_PARTY_OUT = FLAG_AREA_TEMP_21,
	// McCoy has had the captain take the party to the treasure room, until it takes a reward.
	FLAG_REWARD_OFFERED           = FLAG_AREA_TEMP_22,
	// The captain has seen the party off with its reward.
	FLAG_PARTY_SEEN_OFF           = FLAG_AREA_TEMP_33
};

static void scMcCoyPalaceClaimReward(Sint32 chestActorId, Sint32 itemId, Sint32 chestFlag);
static void scMcCoyPalaceAudienceWithMcCoy(void);
static void scMcCoyPalaceCaptainTalksAboutBarricade(void);
static void scMcCoyPalaceMcCoyRewardsParty(void);
static void scMcCoyPalacePresentRewardChests(void);

// scTurnTowardTarget, the call McCoyPalaceTurnTowardWatched makes.
#include "shared/actor_facing.c"

const SCENE_ENTRANCE *scGetEntrances(void)
{
	return McCoyPalaceEntrances;
}

const SCENE_EXIT_REGION *scGetExitRegions(void)
{
	return NULL;
}

const Uint32 *scGetExits(void)
{
	return McCoyPalaceExits;
}

// The people of McCoy's Palace. Once the party has its reward, the woman in Lady McCoy's nightgown
// no longer turns when spoken to, and the old man and the soldiers in the tower rooms and the
// treasure room turn to the party.
const ACTOR_PLACE *scGetActorPlacements(void)
{
	ACTOR_PLACE *placements;

	if (MainRoutineStateWork.next == SCENE_MC_COY_PALACE)
	{
		placements = McCoyPalaceActorPlacements;
		fdAssignViewFlags(placements);

		if (gmFlagGet(FLAG_MCCOY_PALACE_REWARD_TAKEN) != 0)
		{
			McCoyPalaceActorPlacements[ACTOR_VILLAGER_IN_MILADYS_NIGHTGOWN - ACTOR_FIRST_PLACED]
				.talkFacing = TALK_FACING_KEEP;
			McCoyPalaceActorPlacements[ACTOR_OLD_MAN_PRAISING_THE_WARRIORS - ACTOR_FIRST_PLACED]
				.talkFacing = TALK_FACING_PARTY;
			McCoyPalaceActorPlacements[ACTOR_SOLDIER_WATCHING_KOLIMA - ACTOR_FIRST_PLACED]
				.talkFacing = TALK_FACING_PARTY_THEN_WATCH;
			McCoyPalaceActorPlacements[ACTOR_SOLDIER_SHOWING_THE_REWARD - ACTOR_FIRST_PLACED]
				.talkFacing = TALK_FACING_PARTY_AND_BACK;
		}

		return placements;
	}
	return McCoyPalaceNoActorPlacements;
}

// What the party can talk to and search in the palace.
const CUE *scGetCues(void)
{
	if (MainRoutineStateWork.next == SCENE_MC_COY_PALACE)
	{
		return McCoyPalaceCues;
	}
	return McCoyPalaceNoCues;
}

// Before the reward, the woman with fair hair in the hall asks "Oh, are you willing to go to Kolima
// Forest?" On yes she has lost count of the warriors McCoy has hired to end the curse, and not one
// has come back; on no, the party does not look too strong, but that may be for the best.
static void scMcCoyPalaceAskAboutGoingToKolima(void)
{
	fdScriptStart();
	fdScriptSetMessageId(MsgBilibinYouWillingGoKolimaForest);
	// "Oh, are you willing to go to Kolima Forest?" Yes: "I forget how many people Lord McCoy has
	// hired to end the..." No: "You don't look too strong, but that may be for the best."
	fdScriptAskYesNo(ACTOR_VILLAGER_ASKING_ABOUT_KOLIMA_FOREST, 0);
	fdScriptEnd();
}

// After the reward, the woman in Lady McCoy's nightgown in the east bedroom winces and screams
// "AAAAH!!!" when spoken to, then turns: "Oh! For a second, I thought you were milady. You really
// startled me!"
static void scMcCoyPalaceTalkToWomanInMiladysNightgown(void)
{
	fdScriptStart();
	fdActorShowEmote(ACTOR_VILLAGER_IN_MILADYS_NIGHTGOWN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorRunRepeatedMotion(ACTOR_VILLAGER_IN_MILADYS_NIGHTGOWN, 2);
	fdScriptWait(40);
	fdScriptSetMessageId(MsgBilibinAaaah);
	// "AAAAH!!!"
	fdScriptShowMessageThenWait(ACTOR_VILLAGER_IN_MILADYS_NIGHTGOWN, 0, 20);
	fdActorFaceActor(ACTOR_VILLAGER_IN_MILADYS_NIGHTGOWN, ACTOR_PARTY_LEADER, 0);
	fdScriptWait(20);
	// "Oh! For a second, I thought you were milady. You really..."
	fdScriptShowMessageThenWait(ACTOR_VILLAGER_IN_MILADYS_NIGHTGOWN, 0, 10);
	fdActorFaceDirection(ACTOR_VILLAGER_IN_MILADYS_NIGHTGOWN, FACING_NORTH_NORTHWEST, 10);
	fdScriptEnd();
}

// After the reward, the old man with a cane in the north-east tower room tells Isaac "You are a
// fine warrior." Spoken to again before the party leaves McCoy's Palace, he worries instead: "It's
// very reckless for such young warriors to face the holy tree..."
static void scMcCoyPalaceTalkToOldManPraisingTheWarriors(void)
{
	fdScriptStart();
	fdScriptSetMessageId(MsgBilibinFineWarrior);
	if (fdScriptGetFlag(FLAG_OLD_MAN_HAS_PRAISED) != 0)
	{
		fdScriptSetMessageId(MsgBilibinItsVeryRecklessForSuch);
	}
	fdScriptShowMessage(ACTOR_OLD_MAN_PRAISING_THE_WARRIORS, 0);
	gmFlagSet(FLAG_OLD_MAN_HAS_PRAISED);
	fdScriptEnd();
}

// After the reward, the soldier in the south-east tower room asks "I was watching from here. After
// all that fuss, you still went to Kolima?" On yes he does not know how they lifted the curse, but
// they are the best; on no he calls Isaac a liar, for he saw them walk through the barricade.
static void scMcCoyPalaceTalkToSoldierWatchingKolima(void)
{
	fdScriptStart();
	fdScriptSetMessageId(MsgBilibinWasWatchingFromHereAfter);
	// "I was watching from here. After all that fuss, you still..." Yes: "I don't know how you
	// removed the curse, but you're the best!" No: "You lie! I saw you walking through the
	// barricade! And then I..."
	fdScriptAskYesNo(ACTOR_SOLDIER_WATCHING_KOLIMA, 0);
	fdScriptEnd();
}

// The soldier at the treasure room's barred gate shows off what lies behind it: "What do you think?
// These are Bilibin's great treasures. Whoever ends the curse can claim these as his own." While
// the party is choosing its reward he says "You may choose only one item, but I trust that all of
// them will prove useful," and once it has one, that it was indeed the greatest of treasures.
//
// Unused: a line the game never shows, "Pick any one of the four treasures.", would follow his
// boast if FLAG_MCCOY_PALACE_UNUSED_PICK_OFFERED were set, and nothing sets it.
static void scMcCoyPalaceTalkToSoldierShowingTheReward(void)
{
	fdScriptStart();

	if (gmFlagGet(FLAG_CHOOSING_REWARD) != 0)
	{
		fdScriptSetMessageId(MsgBilibinMayChooseOnlyOneItem);
	}
	else if (gmFlagGet(FLAG_MCCOY_PALACE_REWARD_TAKEN) != 0)
	{
		fdScriptSetMessageId(MsgBilibinRewardReceivedWasIndeedGreatest);
	}
	else
	{
		// "What do you think? These are Bilibin's great treasures."
		fdScriptSetMessageId(MsgBilibinDoThinkTheseBilibinsGreat);
		if (gmFlagGet(FLAG_MCCOY_PALACE_UNUSED_PICK_OFFERED) != 0)
		{
			// "Pick any one of the four treasures."
			FieldStateWork->msg++;
		}
	}

	fdScriptShowMessage(ACTOR_SOLDIER_SHOWING_THE_REWARD, 0);
	fdScriptEnd();
}

// After the reward, Mind Read on the old man with a cane in the north-east tower room hears
// "Sometimes, we need children to remind us of our own responsibilities." Read again before the
// party leaves McCoy's Palace, he thinks "You looked very courageous walking toward Kolima."
static void scMcCoyPalaceReadOldMansThoughtsOnTheWarriors(void)
{
	fdScriptStart();
	fdScriptSetMessageId(MsgBilibinSometimesWeNeedChildrenRemind);
	if (fdScriptGetFlag(FLAG_OLD_MAN_THOUGHTS_READ) != 0)
	{
		fdScriptSetMessageId(MsgBilibinLookedVeryCourageousWalkingToward);
	}
	fdScriptShowMessage(ACTOR_OLD_MAN_PRAISING_THE_WARRIORS, 0);
	gmFlagSet(FLAG_OLD_MAN_THOUGHTS_READ);
	fdScriptEnd();
}

// Mind Read on the soldier at the treasure room's barred gate. Before the curse is lifted he thinks
// "What good is treasure if you get turned into a tree?" Once it is lifted: "Are we really giving
// our treasure to these wimps?" While the party chooses: "Nonetheless...if your luck is sour, you
// may end up an item you do not need." After the reward he thinks they got a pretty nice reward but
// were just unlucky.
static void scMcCoyPalaceReadRewardSoldiersThoughts(void)
{
	fdScriptStart();

	if (gmFlagGet(FLAG_CHOOSING_REWARD) != 0)
	{
		fdScriptSetMessageId(MsgBilibinNonethelessIfYourLuckSour);
	}
	else if (gmFlagGet(FLAG_KOLIMA_CURSE_LIFTED) == 0)
	{
		fdScriptSetMessageId(MsgBilibinGoodTreasureIfGetTurned);
	}
	else
	{
		fdScriptSetMessageId(MsgBilibinWeReallyGivingOurTreasure);
		if (gmFlagGet(FLAG_MCCOY_PALACE_REWARD_TAKEN) != 0)
		{
			fdScriptSetMessageId(MsgBilibinGotPrettyNiceRewardBut);
		}
	}

	fdScriptShowMessage(ACTOR_SOLDIER_SHOWING_THE_REWARD, 0);
	fdScriptEnd();
}

// Walking north into the door at the top of the hall's red carpet (metatiles (15, 39) and (16, 39))
// opens it, and Isaac walks through into McCoy's room. The two doors at the back of Lady McCoy's
// room, metatiles (6, 23) and (19, 23), open the same way into the west and east bedrooms.
//
// McCoy's door opens with `SE_THUMP` and the bedroom doors with `SE_DOOR_OPEN`; the doors' opening
// is the map's tileset animations 1 and 2, let run for the walk. After the captain has come out of
// McCoy's door (scMcCoyPalaceCaptainTalksAboutBarricade) it stands open, so walking back in makes
// no sound and plays no opening.
static void scMcCoyPalaceWalkThroughDoor(void)
{
	FIELD_STATE *state;
	Sint32       playDoorOpening;

	state           = FieldStateWork;
	playDoorOpening = 0;
	if (state->stepCue == MCCOY_PALACE_ENTRANCE_ROOM_TO_MCCOYS_ROOM)
	{
		if (gmFlagGet(FLAG_MCCOYS_DOOR_OPEN) == 0)
		{
			sySoundPlay(SE_THUMP);
			playDoorOpening = 1;
		}
	}
	else
	{
		sySoundPlay(SE_DOOR_OPEN);
		playDoorOpening = 1;
	}
	if (playDoorOpening != 0)
	{
		fdTilesetAnimationResume(MCCOY_PALACE_MCCOYS_DOOR_ANIMATION);
		fdTilesetAnimationResume(MCCOY_PALACE_BEDROOM_DOORS_ANIMATION);
	}
	fdScriptStart();
	fdScriptWait(10);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.5);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
	if (state->stepCue == MCCOY_PALACE_ENTRANCE_ROOM_TO_MCCOYS_ROOM)
		fdActorSetDestinationOffset(ACTOR_PARTY_LEADER, 0, -16);
	else
		fdActorCenterAndWalk(ACTOR_PARTY_LEADER, 3, -16);
	fdScriptWait(16);
	syImportScriptSetExitRequest(state->stepCue);
	fdScriptEnd();
	fdTilesetAnimationPause(MCCOY_PALACE_MCCOYS_DOOR_ANIMATION);
	fdTilesetAnimationPause(MCCOY_PALACE_BEDROOM_DOORS_ANIMATION);
}

// "Isaac checked the chest... but it was empty."
static void scMcCoyPalaceShowChestEmpty(void)
{
	fdScriptStart();
	mnFieldMessageShow(MsgBilibinIsaacCheckedChestButWas, 1);
	fdScriptEnd();
}

// "The treasure chest is locked."
static void scMcCoyPalaceShowChestLocked(void)
{
	fdScriptStart();
	mnFieldMessageShow(MsgBilibinTreasureChestLocked, 1);
	fdScriptEnd();
}

// After the reward, the chest the party opened searches as empty and the other three as locked.
static void scMcCoyPalaceShowUnavailableChest(Sint32 chestFlag)
{
	if (gmFlagGet(chestFlag) != 0)
	{
		scMcCoyPalaceShowChestEmpty();
	}
	else
	{
		scMcCoyPalaceShowChestLocked();
	}
}

// Searching the westmost of the four reward chests in the treasure room (metatile (38, 45)) gives a
// Vial, if the party has not yet chosen its reward.
static void scMcCoyPalaceSearchVialChest(void)
{
	if (gmFlagGet(FLAG_MCCOY_PALACE_REWARD_TAKEN) != 0)
	{
		scMcCoyPalaceShowUnavailableChest(FLAG_TOOK_VIAL);
	}
	else
	{
		scMcCoyPalaceClaimReward(ACTOR_VIAL_CHEST, ITEM_VIAL, FLAG_TOOK_VIAL);
	}
}

// Searching the second reward chest from the west in the treasure room (metatile (39, 45)) gives a
// Potion, if the party has not yet chosen its reward.
static void scMcCoyPalaceSearchPotionChest(void)
{
	if (gmFlagGet(FLAG_MCCOY_PALACE_REWARD_TAKEN) != 0)
	{
		scMcCoyPalaceShowUnavailableChest(FLAG_TOOK_POTION);
	}
	else
	{
		scMcCoyPalaceClaimReward(ACTOR_POTION_CHEST, ITEM_POTION, FLAG_TOOK_POTION);
	}
}

// Searching the third reward chest from the west in the treasure room (metatile (40, 45)) gives a
// Psy Crystal, if the party has not yet chosen its reward.
static void scMcCoyPalaceSearchPsyCrystalChest(void)
{
	if (gmFlagGet(FLAG_MCCOY_PALACE_REWARD_TAKEN) != 0)
	{
		scMcCoyPalaceShowUnavailableChest(FLAG_TOOK_PSY_CRYSTAL);
	}
	else
	{
		scMcCoyPalaceClaimReward(ACTOR_PSY_CRYSTAL_CHEST, ITEM_PSY_CRYSTAL, FLAG_TOOK_PSY_CRYSTAL);
	}
}

// Searching the eastmost of the four reward chests in the treasure room (metatile (41, 45)) gives a
// Water of Life, if the party has not yet chosen its reward.
static void scMcCoyPalaceSearchWaterOfLifeChest(void)
{
	if (gmFlagGet(FLAG_MCCOY_PALACE_REWARD_TAKEN) != 0)
	{
		scMcCoyPalaceShowUnavailableChest(FLAG_TOOK_WATER_OF_LIFE);
	}
	else
	{
		scMcCoyPalaceClaimReward(
			ACTOR_WATER_OF_LIFE_CHEST, ITEM_WATER_OF_LIFE, FLAG_TOOK_WATER_OF_LIFE);
	}
}

// The chest opens and gives its item, and the other three lock. If no one in the party has room for
// it, the lid peeks open and shuts again, and the party can still choose.
static void scMcCoyPalaceClaimReward(Sint32 chestActorId, Sint32 itemId, Sint32 chestFlag)
{
	Sint32 emitterHandle;

	fdScriptStart();

	emitterHandle = fdChestOpenAndShowItem(0, chestActorId, itemId);

	if (gmPartyGiveItem(itemId, 0) != -1)
	{
		fdActorSetAnimation(chestActorId, CHEST_ANIMATION_OPEN_EMPTY);
		gmFlagSet(FLAG_MCCOY_PALACE_REWARD_TAKEN);
		gmFlagSet(chestFlag);
		gmFlagClear(FLAG_REWARD_OFFERED);
		gmFlagClear(FLAG_CHOOSING_REWARD);
	}
	else
	{
		sySoundPlay(SE_CHEST_CLOSE);
		fdActorSetAnimation(chestActorId, CHEST_ANIMATION_PEEK_AND_SHUT);
	}

	fdActorRelease((ACTOR *)emitterHandle);
	fdScriptEnd();
}

// Walking to the foot of the treasure room's stairs (metatile (36, 47)) without a reward makes the
// captain call out "Please take your reward before your depart!", and Isaac walks back toward the
// chests.
static void scMcCoyPalaceCaptainAsksPartyToChoose(void)
{
	if (gmFlagGet(FLAG_MCCOY_PALACE_REWARD_TAKEN) == 0)
	{
		if (gmFlagGet(FLAG_REWARD_OFFERED) != 0)
		{
			fdScriptStart();
			fdActorShowEmote(ACTOR_CAPTAIN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
			fdActorFaceDirection(ACTOR_CAPTAIN, FACING_WEST_SOUTHWEST, 10);
			fdActorRunRepeatedMotion(ACTOR_CAPTAIN, 2);
			fdScriptWait(20);
			fdScriptSetMessageId(MsgBilibinPleaseTakeYourRewardBefore);
			// "Please take your reward before your depart!"
			fdScriptShowMessage(ACTOR_CAPTAIN, 0);
			fdActorSetSpeed(ACTOR_PARTY_LEADER, 1);
			fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 616, 762);
			fdActorFaceDirection(ACTOR_CAPTAIN, FACING_NORTH_NORTHEAST, 10);
			fdScriptEnd();
		}
	}
}

// Once Isaac has his reward, walking along the row in front of the chests (metatiles (38, 46) to
// (41, 46)) makes the captain step up beside the soldier at the gate. Both nod, and the captain
// tells Isaac "You are always welcome in the palace of Lord McCoy." before walking off up the
// stairs.
//
// It can play only in the visit the reward is taken in: the row takes the metatiles its cue answers
// only while the chests stand out before the gate (scMcCoyPalaceOpenRewardChestRow), and once a
// reward is taken a later visit does not bring them out again.
static void scMcCoyPalaceCaptainSeesPartyOff(void)
{
	if (gmFlagGet(FLAG_MCCOY_PALACE_REWARD_TAKEN) != 0)
	{
		fdScriptStart();
		fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_CAPTAIN, 0);
		fdActorSetSpeed(ACTOR_CAPTAIN, 0.6);
		fdActorWalkToAndWait(ACTOR_CAPTAIN, 622, 764);
		fdActorFaceDirection(ACTOR_CAPTAIN, FACING_EAST_NORTHEAST, 20);
		fdActorSetAnimationAndWait(ACTOR_CAPTAIN, ANIM_NOD);
		fdActorSetAnimationAndWait(ACTOR_SOLDIER_SHOWING_THE_REWARD, ANIM_NOD);
		fdScriptWait(20);
		fdActorFaceActor(ACTOR_CAPTAIN, ACTOR_PARTY_LEADER, 0);
		fdScriptWait(20);
		fdActorSetAnimationAndWait(ACTOR_CAPTAIN, ANIM_NOD);
		fdScriptSetMessageId(MsgBilibinAlwaysWelcomeInPalaceLord);
		// "You are always welcome in the palace of Lord McCoy."
		fdScriptShowMessageThenWait(ACTOR_CAPTAIN, 0, 10);
		fdActorSetSpeed(ACTOR_CAPTAIN, 0.8);
		fdActorWalkToAndWait(ACTOR_CAPTAIN, 570, 758);
		fdActorSetPos(ACTOR_CAPTAIN, 0, 0);
		FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
		gmFlagSet(FLAG_MCCOY_PALACE_PARTY_REWARDED);
		gmFlagSet(FLAG_PARTY_SEEN_OFF);
		fdScriptEnd();
	}
}

// The four reward chests stand in a row before the barred gate, from west to east the Vial, the
// Potion, the Psy Crystal and the Water of Life.
static inline void scMcCoyPalacePosRewardChests(void)
{
	fdActorSetPos(ACTOR_VIAL_CHEST, 616, 728);
	fdActorSetPos(ACTOR_POTION_CHEST, 632, 728);
	fdActorSetPos(ACTOR_PSY_CRYSTAL_CHEST, 648, 728);
	fdActorSetPos(ACTOR_WATER_OF_LIFE_CHEST, 664, 728);
}

// The chests' metatiles and the row in front of them take the behaviors the cues answer, copied
// from the spare metatiles at (38, 55) to (45, 55).
static inline void scMcCoyPalaceOpenRewardChestRow(void)
{
	fdMapGridCopyMetatileAttributes(38, 55, 4, 1, 38, 45);
	fdMapGridCopyMetatileAttributes(42, 55, 4, 1, 38, 46);
}

// Sets McCoy's Palace up and plays the story's scenes: the first audience on entering McCoy's room,
// the captain's hint outside his door after it, the reward once the curse is lifted, and the chests
// in the treasure room. A return from a battle while the party is choosing puts the chests straight
// back in their row.
Sint32 scInit(void)
{
	Sint32 rewardTaken;
	Sint32 curseLifted;

	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
	fdTilesetAnimationPause(MCCOY_PALACE_MCCOYS_DOOR_ANIMATION);
	fdTilesetAnimationPause(MCCOY_PALACE_BEDROOM_DOORS_ANIMATION);
	gmFlagSet(FLAG_MCCOY_PALACE_VISITED);
	if (gmFlagGet(FLAG_KEEP_PARTY_POSITION) != 0)
	{
		gmFlagClear(FLAG_MCCOYS_DOOR_OPEN);
	}
	// A first audience and a return visit have different greetings.
	if (gmFlagGet(FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY) == 0
	    && gmFlagGet(FLAG_KOLIMA_CURSE_LIFTED) == 0)
	{
		if (MainRoutineStateWork.entrance == MCCOY_PALACE_FROM_MCCOY_PALACE_GROUNDS_EAST)
		{
			scMcCoyPalaceAudienceWithMcCoy();
		}
		else if (MainRoutineStateWork.entrance == MCCOY_PALACE_ENTRANCE_ROOM_FROM_MCCOYS_ROOM
		         && gmFlagGet(FLAG_CAPTAIN_SEEING_PARTY_OUT) != 0)
		{
			scMcCoyPalaceCaptainTalksAboutBarricade();
		}
	}
	else
	{
		rewardTaken = gmFlagGet(FLAG_MCCOY_PALACE_REWARD_TAKEN);
		if (rewardTaken == 0)
		{
			if (MainRoutineStateWork.entrance == MCCOY_PALACE_FROM_MCCOY_PALACE_GROUNDS_EAST)
			{
				if (gmFlagGet(FLAG_MCCOY_PALACE_PARTY_REWARDED) == 0)
				{
					curseLifted = gmFlagGet(FLAG_KOLIMA_CURSE_LIFTED);
					if (curseLifted != 0)
					{
						scMcCoyPalaceMcCoyRewardsParty();
					}
				}
			}
			else
			{
				if (MainRoutineStateWork.entrance == MCCOY_PALACE_TREASURE_ROOM_FROM_ENTRANCE_ROOM)
				{
					if (gmFlagGet(FLAG_REWARD_OFFERED) != 0)
					{
						if (gmFlagGet(FLAG_KEEP_PARTY_POSITION) != 0)
						{
							scMcCoyPalaceOpenRewardChestRow();
							scMcCoyPalacePosRewardChests();
							fdActorSetSpriteFlags(fdActorGet(ACTOR_VIAL_CHEST), 0);
							fdActorSetSpriteFlags(fdActorGet(ACTOR_POTION_CHEST), 0);
							fdActorSetSpriteFlags(fdActorGet(ACTOR_PSY_CRYSTAL_CHEST), 0);
							fdActorSetSpriteFlags(fdActorGet(ACTOR_WATER_OF_LIFE_CHEST), 0);
							fdActorGet(ACTOR_VIAL_CHEST)->motionFlags          = rewardTaken;
							fdActorGet(ACTOR_POTION_CHEST)->motionFlags        = rewardTaken;
							fdActorGet(ACTOR_PSY_CRYSTAL_CHEST)->motionFlags   = rewardTaken;
							fdActorGet(ACTOR_WATER_OF_LIFE_CHEST)->motionFlags = rewardTaken;
							fdActorGet(ACTOR_VIAL_CHEST)->y.fixed              = -toFIXED(4);
							fdActorGet(ACTOR_POTION_CHEST)->y.fixed            = -toFIXED(4);
							fdActorGet(ACTOR_PSY_CRYSTAL_CHEST)->y.fixed       = -toFIXED(4);
							fdActorGet(ACTOR_WATER_OF_LIFE_CHEST)->y.fixed     = -toFIXED(4);
						}
						else
						{
							scMcCoyPalacePresentRewardChests();
						}
					}
				}
			}
		}
	}
	return 0;
}

// Garet, Ivan and Mia get ready to walk up to McCoy.
static inline void scMcCoyPalacePrepareCompanionApproach(void)
{
	fdActorSetSpeed(ACTOR_GARET, 0.6);
	fdActorSetSpeed(ACTOR_IVAN, 0.6);
	fdActorSetSpeed(ACTOR_MIA, 1);
	fdActorSetAnimation(ACTOR_GARET, ANIM_WALK);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_WALK);
	fdActorSetAnimation(ACTOR_MIA, ANIM_WALK);
}

// The first audience, on walking through McCoy's door before Kolima's curse is lifted: the captain
// announces the party, McCoy calls Isaac closer and asks whether they will go to Kolima Forest. On
// no he sends them away as too young, and a return lets Isaac change his mind. On yes he tosses
// Isaac the key to the barricades, then, seeing how young the party is, takes it back and has his
// captain see them out.
static void scMcCoyPalaceAudienceWithMcCoy(void)
{
	ACTOR *actor;

	fdScriptStart();
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	sySynch(1);
	fdCameraGetActor()->motionFlags = 0;
	fdCameraMoveTo(894, CAMERA_KEEP, 664, 0);
	sySynch(1);
	fdActorSetPos(ACTOR_PARTY_LEADER, 0, 0);
	if (fdScriptGetFlag(FLAG_MCCOY_PALACE_JOB_REFUSED) != 0)
	{
		fdCameraMoveTo(894, CAMERA_KEEP, 698, 0);
		fdActorSetPos(ACTOR_CAPTAIN, 876, 634);
		fdActorFaceDirection(ACTOR_CAPTAIN, FACING_SOUTH_SOUTHEAST, 0);
		fdActorSetPos(ACTOR_PARTY_LEADER, 894, 798);
	}
	fdMapGridCenterOnFollowedPoint();
	sySynch(1);
	FieldStateWork->transition      = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	FieldStateWork->transitionFrame = 40;
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	if (gmFlagGet(FLAG_MCCOY_PALACE_JOB_REFUSED) == 0)
	{
		fdScriptWait(80);
		fdActorSetPos(ACTOR_CAPTAIN, 894, 798);
		fdCameraSetSpeed(0.6);
		fdCameraMoveTo(894, CAMERA_KEEP, 698, 1);
		fdActorSetSpeed(ACTOR_CAPTAIN, 0.8);
		fdActorWalkTo(ACTOR_CAPTAIN, 894, 696);
		fdScriptWait(80);
		fdCameraMoveTo(894, CAMERA_KEEP, 664, 1);
		fdActorWaitForMoveById(ACTOR_CAPTAIN);
		fdActorWalkToAndWait(ACTOR_CAPTAIN, 842, 696);
		fdActorWalkToAndWait(ACTOR_CAPTAIN, 842, 636);
		fdActorFaceDirection(ACTOR_LORD_MCCOY, FACING_WEST_SOUTHWEST, 20);
		fdActorWalkToAndWait(ACTOR_CAPTAIN, 876, 634);
		fdActorSetAnimationAndWait(ACTOR_CAPTAIN, ANIM_NOD);
		fdScriptWait(20);
		fdActorSetAnimationAndWait(ACTOR_LORD_MCCOY, ANIM_NOD);
		fdScriptWait(10);
		// The captain announces the warriors before they enter the audience.
		fdScriptSetMessageId(MsgBilibinMatter);
		// "What's the matter, then?"
		syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
		fdActorRunRepeatedMotion(ACTOR_CAPTAIN, 2);
		// "Sir! Some new warriors have just arrived in the palace!"
		fdScriptShowMessageThenWait(ACTOR_CAPTAIN, 0, 20);
		fdActorRunRepeatedMotion(ACTOR_LORD_MCCOY, 1);
		// "New warriors, yeh say?"
		syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
		fdActorSetAnimationAndWait(ACTOR_CAPTAIN, ANIM_NOD);
		fdScriptWait(40);
		fdActorShowEmote(ACTOR_LORD_MCCOY, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
		// "We've sent many a man intae Kolima Forest, but not a one has..."
		fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0);
		fdActorRunRepeatedMotion(ACTOR_LORD_MCCOY, 1);
		// "These warriors... Did yeh get a good look at 'em?"
		syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
		fdActorSetAttachedEffect(ACTOR_CAPTAIN, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
		fdScriptWait(60);
		fdActorFaceDirection(ACTOR_CAPTAIN, FACING_SOUTH_SOUTHEAST, 10);
		fdActorFaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTH_SOUTHWEST, 10);
		fdCameraMoveTo(894, CAMERA_KEEP, 698, 1);
		fdActorSetPos(ACTOR_PARTY_LEADER, 894, 798);
		fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.6);
		fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 894, 726);
		fdScriptWait(20);
		fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
		fdScriptWait(10);
		fdActorRunRepeatedMotion(ACTOR_LORD_MCCOY, 1);
		// "Come closer, laddy."
		fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0);
		fdCameraMoveTo(894, CAMERA_KEEP, 664, 1);
		fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 894, 684);
		actor = fdActorGet(ACTOR_PARTY_LEADER);
		if (actor != NULL)
		{
			fdActorSetPosFX(ACTOR_GARET, actor->x.fixed, actor->z.fixed);
		}
		actor = fdActorGet(ACTOR_PARTY_LEADER);
		if (actor != NULL)
		{
			fdActorSetPosFX(ACTOR_IVAN, actor->x.fixed, actor->z.fixed);
		}
		if (gmFlagGet(FLAG_IN_PARTY(MIA)) != 0)
		{
			actor = fdActorGet(ACTOR_PARTY_LEADER);
			if (actor != NULL)
			{
				fdActorSetPosFX(ACTOR_MIA, actor->x.fixed, actor->z.fixed);
			}
		}
		scMcCoyPalacePrepareCompanionApproach();
		fdActorSetDestinationOffset(ACTOR_GARET, -16, 16);
		fdActorSetDestinationOffset(ACTOR_IVAN, 16, 16);
		if (gmFlagGet(FLAG_IN_PARTY(MIA)) != 0)
		{
			fdActorSetDestinationOffset(ACTOR_MIA, 32, 16);
		}
		fdActorWaitForMoveById(ACTOR_IVAN);
		fdActorSetAnimation(ACTOR_GARET, ANIM_STAND);
		fdActorSetAnimation(ACTOR_IVAN, ANIM_STAND);
		fdActorSetAnimation(ACTOR_MIA, ANIM_STAND);
		fdScriptWait(10);
		fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
		fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
		fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 20);
		fdActorJump(ACTOR_LORD_MCCOY, 2, 20);
		fdActorFaceDirection(ACTOR_LORD_MCCOY, FACING_WEST_SOUTHWEST, 10);
		syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
		fdActorFaceDirection(ACTOR_CAPTAIN, FACING_EAST_SOUTHEAST, 10);
		fdActorSetAnimationAndWait(ACTOR_CAPTAIN, ANIM_NOD);
		fdActorFaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTH_SOUTHWEST, 40);
		fdActorFaceDirection(ACTOR_LORD_MCCOY, FACING_WEST_SOUTHWEST, 10);
		fdActorSetAnimationAndWait(ACTOR_LORD_MCCOY, ANIM_SHAKE_HEAD);
		syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
		fdActorSetAttachedEffect(ACTOR_CAPTAIN, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
		fdScriptWait(40);
		fdActorFaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTH_SOUTHWEST, 20);
		fdActorShowEmote(ACTOR_LORD_MCCOY, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
		fdScriptOpenMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0);
		fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
		fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
		if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0)
		{
			// The initial offer and reconsideration share their accept/refuse paths.
			goto accepted;
		}
	declined:
		fdScriptSetMessageId(MsgBilibinTooYoungForTheJob);
		// "Bah! Yeh're too young. Yeh cannae handle the job."
		syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
		fdActorFaceDirection(ACTOR_CAPTAIN, FACING_SOUTH_SOUTHEAST, 0);
		fdActorSetAnimationAndWait(ACTOR_LORD_MCCOY, ANIM_SHAKE_HEAD);
		// "Yeh can leave now..."
		fdScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
		fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
		fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
		fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
		fdActorSetAnimation(ACTOR_GARET, ANIM_WALK);
		actor = fdActorGet(ACTOR_PARTY_LEADER);
		if (actor != NULL)
		{
			fdActorSetDestination(ACTOR_GARET, actor->x.part[0].integer, actor->z.part[0].integer);
		}
		fdActorSetAnimation(ACTOR_IVAN, ANIM_WALK);
		actor = fdActorGet(ACTOR_PARTY_LEADER);
		if (actor != NULL)
		{
			fdActorSetDestination(ACTOR_IVAN, actor->x.part[0].integer, actor->z.part[0].integer);
		}
		if (gmFlagGet(FLAG_IN_PARTY(MIA)) != 0)
		{
			fdActorSetAnimation(ACTOR_MIA, ANIM_WALK);
			actor = fdActorGet(ACTOR_PARTY_LEADER);
			if (actor != NULL)
			{
				fdActorSetDestination(
					ACTOR_MIA, actor->x.part[0].integer, actor->z.part[0].integer);
			}
		}
		fdActorWaitForMoveById(ACTOR_IVAN);
		fdActorSetPos(ACTOR_GARET, 0, 0);
		fdActorSetPos(ACTOR_IVAN, 0, 0);
		fdActorSetPos(ACTOR_MIA, 0, 0);
		gmFlagSet(FLAG_MCCOY_PALACE_JOB_REFUSED);
		fdActorSetSpeed(ACTOR_PARTY_LEADER, 1);
		fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 894, 752);
		FieldStateWork->transitionFrame = MAP_TRANSITION_TICKS;
		fdScriptFinishScreenTransition();
		fdScriptWaitTransitionTicks();
		goto leave;
	}
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.6);
	fdActorWalkTo(ACTOR_PARTY_LEADER, 894, 684);
	fdScriptWait(80);
	fdCameraSetSpeed(0.6);
	fdCameraMoveTo(894, CAMERA_KEEP, 664, 1);
	fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	actor = fdActorGet(ACTOR_PARTY_LEADER);
	if (actor != NULL)
	{
		fdActorSetPosFX(ACTOR_GARET, actor->x.fixed, actor->z.fixed);
	}
	actor = fdActorGet(ACTOR_PARTY_LEADER);
	if (actor != NULL)
	{
		fdActorSetPosFX(ACTOR_IVAN, actor->x.fixed, actor->z.fixed);
	}
	if (gmFlagGet(FLAG_IN_PARTY(MIA)) != 0)
	{
		actor = fdActorGet(ACTOR_PARTY_LEADER);
		if (actor != NULL)
		{
			fdActorSetPosFX(ACTOR_MIA, actor->x.fixed, actor->z.fixed);
		}
	}
	scMcCoyPalacePrepareCompanionApproach();
	fdActorSetDestinationOffset(ACTOR_GARET, -16, 16);
	fdActorSetDestinationOffset(ACTOR_IVAN, 16, 16);
	if (gmFlagGet(FLAG_IN_PARTY(MIA)) != 0)
	{
		fdActorSetDestinationOffset(ACTOR_MIA, 32, 16);
	}
	fdActorWaitForMoveById(ACTOR_IVAN);
	fdActorSetAnimation(ACTOR_GARET, ANIM_STAND);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_STAND);
	fdActorSetAnimation(ACTOR_MIA, ANIM_STAND);
	fdScriptWait(10);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 20);
	fdActorShowEmote(ACTOR_LORD_MCCOY, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	// A returning party can reconsider the assignment without replaying the captain's announcement.
	fdScriptSetMessageId(MsgBilibinYehveChangeHeart);
	// "What is it? Yeh've had a change of heart? Yeh want tae help..."
	fdScriptOpenMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0);
	if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 1)
	{
		goto declined;
	}
accepted:
	// Both ways of accepting join the same explanation and departure.
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 20);
	fdActorSetAnimation(ACTOR_MIA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	fdActorShowEmote(ACTOR_LORD_MCCOY, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
	fdScriptSetMessageId(MsgBilibinHmmmWellGrant);
	// "Hmmm. Well, I'll grant yeh're a brave lot."
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_BARRICADE_KEY), 0);
	actor         = fdActorGet(ACTOR_BARRICADE_KEY);
	actor->scaleX = toFIXED(1) / 2;
	actor->scaleY = toFIXED(1) / 2;
	actor         = fdActorGet(ACTOR_LORD_MCCOY);
	if (actor != NULL)
	{
		fdActorSetPosFX(ACTOR_BARRICADE_KEY, actor->x.fixed, actor->z.fixed);
	}
	sySynch(1);
	fdActorJump(ACTOR_BARRICADE_KEY, 6, 0);
	fdActorSetSpeed(ACTOR_BARRICADE_KEY, 2);
	fdActorMoveToAndWait(ACTOR_BARRICADE_KEY, 894, 668);
	fdScriptWait(40);
	// "Here. This key'll get yeh past the barricades tae the east..."
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	fdActorSetAnimationAndWait(ACTOR_LORD_MCCOY, ANIM_SHAKE_HEAD);
	// "Even with the key, such wee lads as yerselves'll never reach..."
	fdScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 60);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 10);
	// "He's making fun of us! Calling us children! That's just..."
	fdScriptOpenMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
	if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 1)
	{
		do
		{
			fdActorStartRepeatedMotion(ACTOR_GARET, 2);
			fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
			fdScriptSetMessageId(MsgBilibinSeriousDoesntBother);
			// "Are you serious, Isaac? It doesn't bother you?"
			fdScriptOpenMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0);
		} while (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) != 1);
	}
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
	// Garet gets over his anger before McCoy dismisses the party.
	fdScriptSetMessageId(MsgBilibinWellDontLet);
	// "Well, don't let him get to you. We'll just have to prove we..."
	syFieldScriptShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0, 10);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 10);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdScriptWait(20);
	fdActorShowEmote(ACTOR_LORD_MCCOY, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
	// "So, yeh've decided to accept our little job?"
	fdScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 20);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
	fdActorFaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTH_SOUTHEAST, 10);
	fdActorShowEmote(ACTOR_LORD_MCCOY, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	fdActorFaceDirection(ACTOR_GARET, FACING_EAST, 20);
	fdActorRunRepeatedMotion(ACTOR_GARET, 1);
	// "Hey, Ivan! You... You don't look too enthusiastic."
	syFieldScriptShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0, 10);
	fdActorFaceDirection(ACTOR_MIA, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 20);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
	fdActorFaceDirection(ACTOR_IVAN, FACING_WEST, 10);
	// "That curse can turn a man into a tree! You think we stand a..."
	fdScriptShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_IVAN, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_GARET, 1);
	fdScriptWait(40);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
	// "What? Yeah, of course we do!"
	syFieldScriptShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0, 10);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 10);
	fdActorStartRepeatedMotion(ACTOR_GARET, 1);
	// "You know we do, right, Isaac?"
	fdScriptOpenMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
	while (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) != 0)
	{
		fdScriptSetMessageId(MsgBilibinComeSaidYoud);
		// "Come on, Isaac! You said you'd do it! We're still going, right?"
		fdScriptOpenMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0);
	}
	fdScriptWait(10);
	fdActorFaceDirection(ACTOR_MIA, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_EAST, 10);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 10);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
	fdActorFaceDirection(ACTOR_IVAN, FACING_WEST, 10);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
	fdScriptSetMessageId(MsgBilibinWellSGoing);
	// "Well, if Isaac's going, I guess I'm in, too."
	syFieldScriptShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_IVAN, 0, 20);
	fdActorRunRepeatedMotion(ACTOR_LORD_MCCOY, 1);
	fdActorFaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTH_SOUTHWEST, 10);
	fdActorSetAnimationAndWait(ACTOR_LORD_MCCOY, ANIM_SHAKE_HEAD);
	// "At least we all agree on something!"
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 20);
	fdActorSetAnimation(ACTOR_MIA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	fdScriptWait(40);
	fdActorShowEmote(ACTOR_LORD_MCCOY, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
	fdActorFaceDirection(ACTOR_CAPTAIN, FACING_EAST_SOUTHEAST, 10);
	fdActorSetAttachedEffect(ACTOR_CAPTAIN, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(40);
	// "What troubles you, Milord? They seem willing enough..."
	fdScriptShowMessageThenWait(ACTOR_CAPTAIN, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_LORD_MCCOY, 1);
	fdScriptWait(40);
	fdActorFaceDirection(ACTOR_LORD_MCCOY, FACING_WEST_SOUTHWEST, 20);
	// "They call themselves warriors...but they're naught but wee..."
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_CAPTAIN, ANIM_NOD);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_LORD_MCCOY, ANIM_SHAKE_HEAD);
	fdActorSetAnimation(ACTOR_LORD_MCCOY, ANIM_SHAKE_HEAD);
	// "Aye, these are desperate times, but I cannae send such wee..."
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
	fdActorJump(ACTOR_BARRICADE_KEY, 6, 0);
	actor = fdActorGet(ACTOR_LORD_MCCOY);
	if (actor != NULL)
	{
		fdActorSetDestination(
			ACTOR_BARRICADE_KEY, actor->x.part[0].integer, actor->z.part[0].integer);
	}
	fdActorWaitForMoveById(ACTOR_BARRICADE_KEY);
	fdActorSetPos(ACTOR_BARRICADE_KEY, 0, 0);
	fdScriptWait(20);
	fdActorSetAttachedEffect(ACTOR_MIA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(ACTOR_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(ACTOR_GARET, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(ACTOR_IVAN, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(40);
	fdActorRunRepeatedMotion(ACTOR_CAPTAIN, 2);
	// "Then what will happen to Kolima Forest?"
	fdScriptShowMessageThenWait(ACTOR_CAPTAIN, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_LORD_MCCOY, ANIM_NOD);
	// "We cannae give up hope on our other champions!"
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 20);
	fdActorSetAnimationAndWait(ACTOR_CAPTAIN, ANIM_NOD);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
	fdScriptWait(20);
	// "But... But what about us?"
	syFieldScriptShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_IVAN, 0, 10);
	fdActorFaceDirection(ACTOR_CAPTAIN, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTH_SOUTHEAST, 10);
	fdActorSetAnimationAndWait(ACTOR_LORD_MCCOY, ANIM_SHAKE_HEAD);
	// "We cannae send yeh intae certain doom."
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 60);
	// "What if we said that we were going east anyway?"
	fdScriptShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0, 10);
	fdActorFaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTH_SOUTHWEST, 10);
	fdActorSetAnimation(ACTOR_LORD_MCCOY, ANIM_SHAKE_HEAD);
	// "Yeh can go once things have settled down."
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_SHAKE_HEAD);
	// "But that means... you're not going to let us go?"
	fdScriptShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_IVAN, 0, 10);
	fdActorFaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTH_SOUTHEAST, 10);
	fdActorSetAnimationAndWait(ACTOR_LORD_MCCOY, ANIM_NOD);
	// "Ach, it's fer yer own good, lad."
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_SCOWL, 60);
	fdActorFaceDirection(ACTOR_LORD_MCCOY, FACING_WEST_SOUTHWEST, 10);
	fdActorSetAnimationAndWait(ACTOR_LORD_MCCOY, ANIM_NOD);
	fdScriptWait(10);
	// "I dinnae want tae talk about it anymore. Captain?"
	fdScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_CAPTAIN, 2);
	fdActorFaceDirection(ACTOR_CAPTAIN, FACING_EAST_SOUTHEAST, 10);
	fdActorSetAnimationAndWait(ACTOR_CAPTAIN, ANIM_NOD);
	fdScriptWait(20);
	syFieldActorWatchAndStartScript(
		ACTOR_PARTY_LEADER, ACTOR_WATCH_KEEP_MOTION | ACTOR_CAPTAIN, McCoyPalaceTurnTowardWatched);
	syFieldActorWatchAndStartScript(
		ACTOR_GARET, ACTOR_WATCH_KEEP_MOTION | ACTOR_CAPTAIN, McCoyPalaceTurnTowardWatched);
	syFieldActorWatchAndStartScript(
		ACTOR_IVAN, ACTOR_WATCH_KEEP_MOTION | ACTOR_CAPTAIN, McCoyPalaceTurnTowardWatched);
	syFieldActorWatchAndStartScript(
		ACTOR_MIA, ACTOR_WATCH_KEEP_MOTION | ACTOR_CAPTAIN, McCoyPalaceTurnTowardWatched);
	fdActorSetSpeed(ACTOR_CAPTAIN, 0.6);
	fdActorWalkToAndWait(ACTOR_CAPTAIN, 852, 646);
	fdActorWalkToAndWait(ACTOR_CAPTAIN, 852, 666);
	fdActorWalkToAndWait(ACTOR_CAPTAIN, 864, 672);
	fdActorFaceDirection(ACTOR_CAPTAIN, FACING_EAST_SOUTHEAST, 10);
	// "I'm sorry, but you should leave now."
	fdScriptShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_CAPTAIN, 0, 20);
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
		ACTOR_CAPTAIN, ACTOR_WATCH_KEEP_MOTION | ACTOR_PARTY_LEADER, McCoyPalaceTurnTowardWatched);
	fdActorSetAnimation(ACTOR_GARET, ANIM_WALK);
	actor = fdActorGet(ACTOR_PARTY_LEADER);
	if (actor != NULL)
	{
		fdActorSetDestination(ACTOR_GARET, actor->x.part[0].integer, actor->z.part[0].integer);
	}
	fdActorSetAnimation(ACTOR_IVAN, ANIM_WALK);
	actor = fdActorGet(ACTOR_PARTY_LEADER);
	if (actor != NULL)
	{
		fdActorSetDestination(ACTOR_IVAN, actor->x.part[0].integer, actor->z.part[0].integer);
	}
	if (gmFlagGet(FLAG_IN_PARTY(MIA)) != 0)
	{
		fdActorSetAnimation(ACTOR_MIA, ANIM_WALK);
		actor = fdActorGet(ACTOR_PARTY_LEADER);
		if (actor != NULL)
		{
			fdActorSetDestination(ACTOR_MIA, actor->x.part[0].integer, actor->z.part[0].integer);
		}
	}
	fdActorWaitForMoveById(ACTOR_IVAN);
	fdActorSetPos(ACTOR_GARET, 0, 0);
	fdActorSetPos(ACTOR_IVAN, 0, 0);
	fdActorSetPos(ACTOR_MIA, 0, 0);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 1);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 894, 752);
	FieldStateWork->transitionFrame = MAP_TRANSITION_TICKS;
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	gmFlagSet(FLAG_CAPTAIN_SEEING_PARTY_OUT);
leave:
	syImportScriptSetExitRequest(MCCOY_PALACE_MCCOYS_ROOM_TO_ENTRANCE_ROOM);
	fdScriptEnd();
}

// Back in the hall after the first audience, McCoy's door opens behind the party and the captain
// comes out. He is sorry Isaac was rejected, asks him to promise not to go to Kolima, lets slip
// "That barricade... You may not need a key.", and leaves down the red carpet.
static void scMcCoyPalaceCaptainTalksAboutBarricade(void)
{
	fdScriptStart();
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdActorFaceDirection(ACTOR_CAPTAIN, FACING_SOUTH_SOUTHEAST, 0);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.6);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 256, 660);
	fdScriptWait(20);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	gmFlagSet(FLAG_MCCOYS_DOOR_OPEN);
	sySoundPlay(SE_THUMP);
	fdTilesetAnimationResume(MCCOY_PALACE_MCCOYS_DOOR_ANIMATION);
	fdTilesetAnimationResume(MCCOY_PALACE_BEDROOM_DOORS_ANIMATION);
	fdActorSetPos(ACTOR_CAPTAIN, 256, 632);
	sySynch(1);
	fdActorSetSpeed(ACTOR_CAPTAIN, 0.6);
	fdActorWalkToAndWait(ACTOR_CAPTAIN, 256, 644);
	fdTilesetAnimationPause(MCCOY_PALACE_MCCOYS_DOOR_ANIMATION);
	fdTilesetAnimationPause(MCCOY_PALACE_BEDROOM_DOORS_ANIMATION);
	fdScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_CAPTAIN, 2);
	fdScriptSetMessageId(MsgBilibinNameSorryRejected);
	// "Your name was...Isaac? I'm sorry you were rejected."
	fdScriptShowMessageThenWait(ACTOR_CAPTAIN, 0, 10);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 264, 660);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
	fdActorWalkToAndWait(ACTOR_CAPTAIN, 248, 660);
	fdActorFaceDirection(ACTOR_CAPTAIN, FACING_EAST_SOUTHEAST, 40);
	fdActorSetAnimationAndWait(ACTOR_CAPTAIN, ANIM_SHAKE_HEAD);
	// "I never, ever, expected anything like this to happen to Bilibin."
	fdScriptShowMessage(ACTOR_CAPTAIN, 0);
	fdActorSetAnimationAndWait(ACTOR_CAPTAIN, ANIM_NOD);
	// "Will you promise me you won't go to Kolima?" Yes: "That's good." No: "You are persistent,
	// aren't you?"
	fdScriptAskYesNo(ACTOR_CAPTAIN, 0);
	fdActorRunRepeatedMotion(ACTOR_CAPTAIN, 2);
	// "That barricade... You may not need a key."
	fdScriptShowMessageThenWait(ACTOR_CAPTAIN, 0, 10);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	fdActorSetAttachedEffect(ACTOR_CAPTAIN, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(60);
	fdActorRunRepeatedMotion(ACTOR_CAPTAIN, 1);
	// "Oops... I shouldn't have said anything."
	fdScriptShowMessageThenWait(ACTOR_CAPTAIN, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_CAPTAIN, ANIM_NOD);
	// "Well, I'm going to go now."
	fdScriptShowMessage(ACTOR_CAPTAIN, 0);
	fdActorSetSpeed(ACTOR_CAPTAIN, 0.8);
	fdActorWalkToAndWait(ACTOR_CAPTAIN, 248, 772);
	fdActorSetPos(ACTOR_CAPTAIN, 0, 0);
	gmFlagClear(FLAG_SHOW_AREA_NAME);
	gmFlagSet(FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY);
	fdScriptEnd();
}

// Garet, Ivan and Mia turn to talk among themselves.
static void scMcCoyPalaceFaceCompanionsForDiscussion(void)
{
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_WEST, 0);
}

// Garet, Ivan and Mia face McCoy again.
static void scMcCoyPalaceFaceCompanionsForAudience(void)
{
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTHWEST, 0);
}

// Once Kolima's curse is lifted, the guards march the party into McCoy's room as if under arrest.
// McCoy thanks Isaac, explains he only feared they would not stop by on the way back, and has the
// captain take them down to the treasure room to choose a reward.
static void scMcCoyPalaceMcCoyRewardsParty(void)
{
	ACTOR *viewCenter;
	ACTOR *actor;
	Sint32 motionFlags = 0;
	Sint32 advanceAlternateReply;

	fdScriptStart();
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	sySynch(1);
	viewCenter              = fdCameraGetActor();
	viewCenter->motionFlags = motionFlags;
	fdCameraMoveTo(894, CAMERA_KEEP, 664, 0);
	sySynch(1);
	fdActorSetPos(ACTOR_PARTY_LEADER, 0, 0);
	fdMapGridCenterOnFollowedPoint();
	sySynch(1);

	FieldStateWork->transition      = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
	FieldStateWork->transitionFrame = MAP_TRANSITION_TICKS;

	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptWait(40);
	fdActorSetPos(ACTOR_CAPTAIN, 888, 798);
	fdActorSetPos(ACTOR_PARTY_LEADER, 904, 798);
	fdCameraSetSpeed(0.6);
	fdCameraMoveTo(894, CAMERA_KEEP, 698, 1);
	fdActorSetSpeed(ACTOR_CAPTAIN, 0.8);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.6);
	fdActorWalkTo(ACTOR_CAPTAIN, 888, 720);
	fdActorWalkTo(ACTOR_PARTY_LEADER, 904, 736);
	fdScriptWait(60);
	fdActorWaitForMoveById(ACTOR_CAPTAIN);
	fdActorSetAnimation(ACTOR_CAPTAIN, ANIM_STAND);
	fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	fdScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_CAPTAIN, 2);
	// Each line of this audience is written twice, the first for a party McCoy turned away before
	// (FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY), the second for one he meets for the first time;
	// the script steps over the other. Here "We've brought the warriors, milord." or "We brought
	// the warriors, as you ordered, milord."
	fdScriptSetMessageId(MsgBilibinWeveBroughtWarriorsMilord);
	advanceAlternateReply = 1;
	if (fdScriptGetFlag(FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY) == 0)
	{
		FieldStateWork->msg++;
		advanceAlternateReply = 0;
	}
	fdScriptShowMessage(ACTOR_CAPTAIN, 0);
	if (advanceAlternateReply != 0)
	{
		FieldStateWork->msg++;
	}

	fdCameraMoveTo(894, CAMERA_KEEP, 664, 1);
	fdActorSetScript(ACTOR_CAPTAIN, McCoyPalaceCaptainStepsBesideMcCoy);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 894, 684);

	actor = fdActorGet(ACTOR_PARTY_LEADER);
	if (actor != NULL)
	{
		fdActorSetPosFX(ACTOR_GARET, actor->x.fixed, actor->z.fixed);
	}
	actor = fdActorGet(ACTOR_PARTY_LEADER);
	if (actor != NULL)
	{
		fdActorSetPosFX(ACTOR_IVAN, actor->x.fixed, actor->z.fixed);
	}
	actor = fdActorGet(ACTOR_PARTY_LEADER);
	if (actor != NULL)
	{
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
	fdScriptWait(10);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	fdActorWaitForMoveById(ACTOR_MIA);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTHWEST, 0);
	fdActorWaitForScriptEndById(ACTOR_CAPTAIN);
	fdScriptWait(20);

	advanceAlternateReply = 1;
	if (fdScriptGetFlag(FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY) == 0)
	{
		FieldStateWork->msg++;
		advanceAlternateReply = 0;
	}
	fdActorRunRepeatedMotion(ACTOR_LORD_MCCOY, 3);
	// "So, yeh're the wee--I mean, brave--lads I met before!" or "...lads who saved Kolima..."
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 20);
	if (advanceAlternateReply != 0)
	{
		FieldStateWork->msg++;
	}

	advanceAlternateReply = 1;
	if (fdScriptGetFlag(FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY) == 0)
	{
		FieldStateWork->msg++;
		advanceAlternateReply = 0;
	}
	fdActorRunRepeatedMotion(ACTOR_LORD_MCCOY, 1);
	// "What were yer names, again?" or "What are yer names, then?"
	fdScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
	if (advanceAlternateReply != 0)
	{
		FieldStateWork->msg++;
	}

	scMcCoyPalaceFaceCompanionsForDiscussion();
	fdScriptWait(20);
	if (gmFlagGet(FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY) != 0)
	{
		fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
		fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
	}
	else
	{
		fdScriptWait(40);
	}

	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 10);
	// "He's Isaac, sir, and I'm Garet..."
	fdScriptShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0, 10);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 10);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	// "I am called Ivan."
	fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_IVAN, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTHWEST, 10);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
	// "And I am Mia, from the village of Imil."
	fdScriptShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_MIA, 0, 20);
	fdActorSetAnimationAndWait(ACTOR_LORD_MCCOY, ANIM_NOD);
	fdScriptWait(20);
	// "Isaac and company..."
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 20);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);

	if (gmFlagGet(FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY) != 0)
	{
		fdActorRunRepeatedMotion(ACTOR_LORD_MCCOY, 1);
		fdActorSetAnimationAndWait(ACTOR_LORD_MCCOY, ANIM_SHAKE_HEAD);
		// "And did I not decline yer offer when we first met?" Yes: "I did nae give it enough
		// consideration back then." No: "Are yeh no' just sayin' that tae appease me?"
		fdScriptOpenMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0);
		scMcCoyPalaceFaceCompanionsForDiscussion();
		advanceAlternateReply = 1;
		if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) != 0)
		{
			FieldStateWork->msg++;
			advanceAlternateReply = 0;
		}
		fdActorFaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTH_SOUTHWEST, 0);
		scMcCoyPalaceFaceCompanionsForAudience();
		fdScriptWait(10);
		fdScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
		if (advanceAlternateReply != 0)
		{
			FieldStateWork->msg++;
		}
		fdActorShowEmote(ACTOR_LORD_MCCOY, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
	}
	else
	{
		FieldStateWork->msg += 4;
	}

	// "And yet yeh went oan tae break the curse despite my words, did yeh not?" for a party he
	// turned away, else "Are yeh no' the ones who ended the curse of the holy tree?" Yes: "I humbly
	// thank you." No: "Nae, yeh dinnae need tae hide it. My men told me aught yeh did."
	fdScriptOpenMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0);
	scMcCoyPalaceFaceCompanionsForDiscussion();
	if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0)
	{
		fdScriptSetMessageId(MsgBilibinHumblyThank);
	}
	else
	{
		fdScriptSetMessageId(MsgBilibinNaeYehDinnaeNeedTae);
	}

	scMcCoyPalaceFaceCompanionsForAudience();
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 20);
	fdActorRunRepeatedMotion(ACTOR_CAPTAIN, 1);
	// McCoy explains his caution and offers a choice from the treasury. His first line names the
	// captain as its speaker, so it shows without McCoy's portrait.
	fdScriptSetMessageId(MsgBilibinWasButWorriedYehMight);
	// "I was but worried yeh might nae stop here on yer way back."
	fdScriptShowMessageThenWait(ACTOR_CAPTAIN, 0, 10);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_WEST, 40);
	fdActorRunRepeatedMotion(ACTOR_LORD_MCCOY, 2);
	// "I want tae show my appreciation for yer help..."
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	scMcCoyPalaceFaceCompanionsForAudience();
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_LORD_MCCOY, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
	// "I relied oan my money, but I forgot about my people's needs."
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_LORD_MCCOY, ANIM_NOD);
	// "Yeh reminded me of my duties."
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
	fdActorShowEmote(ACTOR_LORD_MCCOY, EMOTE_IN_FRONT | EMOTE_SMILE, 60);
	// "As lord of these lands, I will do my best for everyone's behalf."
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
	fdActorFaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTH_SOUTHEAST, 10);
	// "Isaac, it would honor me greatly if yeh'd accept a wee token of my gratitude."
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_LORD_MCCOY, ANIM_NOD);
	// "I could give it tae yeh here, but would yeh no' rather choose it yerself?" Yes: "Very well.
	// Yeh can choose somethin' that'll help yeh along yer way." No: "Words are nae enough tae
	// express my regret in doubting yeh. Please, choose a token in thanks..."
	fdScriptOpenMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0);

	scMcCoyPalaceFaceCompanionsForDiscussion();
	advanceAlternateReply = 1;
	if (fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 1)
	{
		FieldStateWork->msg++;
		advanceAlternateReply = 0;
	}
	scMcCoyPalaceFaceCompanionsForAudience();
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
	if (advanceAlternateReply != 0)
	{
		FieldStateWork->msg++;
	}

	fdActorFaceDirection(ACTOR_LORD_MCCOY, FACING_WEST_SOUTHWEST, 10);
	fdActorRunRepeatedMotion(ACTOR_CAPTAIN, 1);
	fdActorFaceDirection(ACTOR_CAPTAIN, FACING_EAST_SOUTHEAST, 20);
	fdActorSetAnimationAndWait(ACTOR_LORD_MCCOY, ANIM_NOD);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_CAPTAIN, ANIM_NOD);
	// "I will show Isaac and his friends the way, milord."
	fdScriptShowMessageThenWait(ACTOR_CAPTAIN, 0, 10);
	fdActorFaceDirection(ACTOR_CAPTAIN, FACING_SOUTH_SOUTHEAST, 10);
	fdActorFaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTH_SOUTHEAST, 20);
	fdActorRunRepeatedMotion(ACTOR_LORD_MCCOY, 1);
	// "Many thanks tae yeh, and tae yer companions, as well!!"
	syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_LORD_MCCOY, ANIM_NOD);
	// "Visit me anytime! Yeh'll always find a warm hearth here!"
	fdScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_LORD_MCCOY, 0, 10);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
	fdScriptWait(20);

	fdActorSetAnimation(ACTOR_GARET, ANIM_WALK);
	actor = fdActorGet(ACTOR_PARTY_LEADER);
	if (actor != NULL)
	{
		fdActorSetDestination(ACTOR_GARET, actor->x.part[0].integer, actor->z.part[0].integer);
	}
	fdActorSetAnimation(ACTOR_IVAN, ANIM_WALK);
	actor = fdActorGet(ACTOR_PARTY_LEADER);
	if (actor != NULL)
	{
		fdActorSetDestination(ACTOR_IVAN, actor->x.part[0].integer, actor->z.part[0].integer);
	}
	fdActorSetAnimation(ACTOR_MIA, ANIM_WALK);
	actor = fdActorGet(ACTOR_PARTY_LEADER);
	if (actor != NULL)
	{
		fdActorSetDestination(ACTOR_MIA, actor->x.part[0].integer, actor->z.part[0].integer);
	}

	fdActorWaitForMoveById(ACTOR_GARET);
	fdActorSetPos(ACTOR_GARET, 0, 0);
	fdActorSetPos(ACTOR_IVAN, 0, 0);
	fdActorWaitForMoveById(ACTOR_MIA);
	fdActorSetPos(ACTOR_MIA, 0, 0);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_LORD_MCCOY, FACING_SOUTH_SOUTHWEST, 0);
	syFieldActorWatchAndStartScript(
		ACTOR_PARTY_LEADER, ACTOR_WATCH_KEEP_MOTION | ACTOR_CAPTAIN, McCoyPalaceTurnTowardWatched);
	fdActorWalkToAndWait(ACTOR_CAPTAIN, 852, 646);
	fdActorWalkToAndWait(ACTOR_CAPTAIN, 852, 666);
	fdActorWalkToAndWait(ACTOR_CAPTAIN, 864, 672);
	fdActorFaceDirection(ACTOR_CAPTAIN, FACING_EAST_SOUTHEAST, 10);
	fdActorRunRepeatedMotion(ACTOR_CAPTAIN, 1);
	fdScriptWait(10);
	// "Follow me, and I'll take you to the treasure room."
	fdScriptShowMessageThenWait(ACTOR_CAPTAIN, 0, 10);
	fdActorWalkToAndWait(ACTOR_CAPTAIN, 886, 708);
	fdActorWalkTo(ACTOR_CAPTAIN, 894, 764);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 894, 764);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	gmFlagSet(FLAG_REWARD_OFFERED);
	if (fdScriptGetFlag(FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY) == 0)
	{
		gmFlagSet(FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY);
		gmFlagSet(FLAG_MCCOY_PALACE_ADMITTED);
	}
	syImportScriptSetExitRequest(MCCOY_PALACE_ENTRANCE_ROOM_TO_TREASURE_ROOM);
	fdScriptEnd();
}

// In the treasure room, the captain walks the party up to four chests in a row before the barred
// gate. He tells the soldier at the gate that Lord McCoy ordered Isaac and his companions brought
// here, and the soldier says "You may select a single item from among these four chests."
static void scMcCoyPalacePresentRewardChests(void)
{
	fdScriptStart();
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdCameraGetActor()->motionFlags = 0;
	fdCameraMoveTo(628, CAMERA_KEEP, 748, 0);
	scMcCoyPalaceOpenRewardChestRow();
	fdActorGet(ACTOR_PARTY_LEADER)->facing = FACING_EAST;
	fdActorSetPos(ACTOR_PARTY_LEADER, 577, 760);
	fdActorGet(ACTOR_CAPTAIN)->facing = FACING_EAST;
	fdActorSetPos(ACTOR_CAPTAIN, 592, 760);
	fdActorGet(ACTOR_SOLDIER_SHOWING_THE_REWARD)->facing = FACING_WEST_NORTHWEST;
	fdActorSetPos(ACTOR_SOLDIER_SHOWING_THE_REWARD, 662, 764);
	scMcCoyPalacePosRewardChests();
	fdActorSetSpriteFlags(fdActorGet(ACTOR_VIAL_CHEST), 0);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_POTION_CHEST), 0);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_PSY_CRYSTAL_CHEST), 0);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_WATER_OF_LIFE_CHEST), 0);
	fdActorGet(ACTOR_VIAL_CHEST)->motionFlags          = 0;
	fdActorGet(ACTOR_POTION_CHEST)->motionFlags        = 0;
	fdActorGet(ACTOR_PSY_CRYSTAL_CHEST)->motionFlags   = 0;
	fdActorGet(ACTOR_WATER_OF_LIFE_CHEST)->motionFlags = 0;
	fdActorGet(ACTOR_VIAL_CHEST)->y.fixed              = -toFIXED(4);
	fdActorGet(ACTOR_POTION_CHEST)->y.fixed            = -toFIXED(4);
	fdActorGet(ACTOR_PSY_CRYSTAL_CHEST)->y.fixed       = -toFIXED(4);
	fdActorGet(ACTOR_WATER_OF_LIFE_CHEST)->y.fixed     = -toFIXED(4);
	fdMapGridCenterOnFollowedPoint();
	sySynch(1);
	FieldStateWork->transition      = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
	FieldStateWork->transitionFrame = MAP_TRANSITION_TICKS;
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdActorSetSpeed(ACTOR_CAPTAIN, 0.6);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.6);
	fdActorWalkTo(ACTOR_CAPTAIN, 628, 764);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 612, 764);
	fdActorSetAnimation(ACTOR_CAPTAIN, ANIM_STAND);
	fdScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_CAPTAIN, 1);
	// The captain and the soldier at the gate explain that only one chest may be chosen.
	fdScriptSetMessageId(MsgBilibinLordMccoyOrdered);
	// "Lord McCoy ordered that Isaac and his companions be brought..."
	fdScriptShowMessageThenWait(ACTOR_CAPTAIN, 0, 10);
	fdActorWalkToAndWait(ACTOR_CAPTAIN, 622, 780);
	fdActorFaceDirection(ACTOR_CAPTAIN, FACING_NORTH, 10);
	fdActorRunRepeatedMotion(ACTOR_SOLDIER_SHOWING_THE_REWARD, 2);
	// "You may select a single item from among these four chests."
	fdScriptShowMessageThenWait(ACTOR_SOLDIER_SHOWING_THE_REWARD, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	gmFlagClear(FLAG_SHOW_AREA_NAME);
	gmFlagSet(FLAG_CHOOSING_REWARD);
	fdScriptEnd();
}

// Keeps turning the actor toward the one it watches.
Sint32 McCoyPalaceTurnTowardWatched[] = {
	SC_CALL(scTurnTowardTarget),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

// The captain walks up beside McCoy and turns south.
Sint32 McCoyPalaceCaptainStepsBesideMcCoy[] = {
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
	{ENTRANCE_DEFAULT, CONDITION_ALWAYS, 256, 0, 808, FACING_NORTH, 0, -1, -1, -1, -1, 0},
	{MCCOY_PALACE_FROM_MCCOY_PALACE_GROUNDS_WEST,
     CONDITION_ALWAYS, 256,
     0, 808,
     FACING_NORTH, 0,
     32, 496,
     480, 856,
     0},
	{MCCOY_PALACE_ENTRANCE_ROOM_FROM_SOUTHWEST_LANDING,
     CONDITION_ALWAYS, 120,
     0, 792,
     FACING_EAST, 0,
     32, 496,
     480, 856,
     0},
	{MCCOY_PALACE_ENTRANCE_ROOM_FROM_SOUTHEAST_LANDING,
     CONDITION_ALWAYS, 392,
     0, 792,
     FACING_WEST, 0,
     32, 496,
     480, 856,
     0},
	{MCCOY_PALACE_ENTRANCE_ROOM_FROM_LADY_MCCOYS_ROOM_WEST,
     CONDITION_ALWAYS, 232,
     0, 712,
     FACING_EAST, 0,
     32, 496,
     480, 856,
     0},
	{MCCOY_PALACE_ENTRANCE_ROOM_FROM_LADY_MCCOYS_ROOM_EAST,
     CONDITION_ALWAYS, 280,
     0, 712,
     FACING_WEST, 0,
     32, 496,
     480, 856,
     0},
	{MCCOY_PALACE_ENTRANCE_ROOM_FROM_TREASURE_ROOM,
     CONDITION_ALWAYS, 152,
     0, 632,
     FACING_WEST, 0,
     32, 496,
     480, 856,
     0},
	{MCCOY_PALACE_ENTRANCE_ROOM_FROM_NORTHWEST_LANDING,
     CONDITION_ALWAYS, 104,
     0, 552,
     FACING_EAST, 0,
     32, 496,
     480, 856,
     0},
	{MCCOY_PALACE_ENTRANCE_ROOM_FROM_NORTHEAST_LANDING,
     CONDITION_ALWAYS, 408,
     0, 552,
     FACING_WEST, 0,
     32, 496,
     480, 856,
     0},
	{MCCOY_PALACE_ENTRANCE_ROOM_FROM_MCCOYS_ROOM,
     CONDITION_ALWAYS, 256,
     0, 632,
     FACING_SOUTH, 0,
     32, 496,
     480, 856,
     0},
	{MCCOY_PALACE_LADY_MCCOYS_ROOM_FROM_ENTRANCE_ROOM_WEST,
     CONDITION_ALWAYS, 152,
     0, 376,
     FACING_WEST, 0,
     56, 304,
     360, 464,
     0},
	{MCCOY_PALACE_LADY_MCCOYS_ROOM_FROM_ENTRANCE_ROOM_EAST,
     CONDITION_ALWAYS, 264,
     0, 376,
     FACING_EAST, 0,
     56, 304,
     360, 464,
     0},
	{MCCOY_PALACE_LADY_MCCOYS_ROOM_FROM_WEST_BEDROOM,
     CONDITION_ALWAYS, 104,
     0, 376,
     FACING_SOUTH, 0,
     56, 304,
     360, 464,
     0},
	{MCCOY_PALACE_LADY_MCCOYS_ROOM_FROM_EAST_BEDROOM,
     CONDITION_ALWAYS, 312,
     0, 376,
     FACING_SOUTH, 0,
     56, 304,
     360, 464,
     0},
	{MCCOY_PALACE_SOUTHWEST_LANDING_FROM_ENTRANCE_ROOM,
     CONDITION_ALWAYS, 696,
     0, 96,
     FACING_WEST, 0,
     592, 0,
     832, 160,
     0},
	{MCCOY_PALACE_SOUTHWEST_LANDING_FROM_SOUTHWEST_TOWER,
     CONDITION_ALWAYS, 680,
     0, 72,
     FACING_WEST, 0,
     592, 0,
     832, 160,
     0},
	{MCCOY_PALACE_SOUTHEAST_LANDING_FROM_SOUTHEAST_TOWER,
     CONDITION_ALWAYS, 952,
     0, 72,
     FACING_EAST, 0,
     800, 0,
     1040, 160,
     0},
	{MCCOY_PALACE_SOUTHEAST_LANDING_FROM_ENTRANCE_ROOM,
     CONDITION_ALWAYS, 936,
     0, 96,
     FACING_EAST, 0,
     800, 0,
     1040, 160,
     0},
	{MCCOY_PALACE_WEST_BEDROOM_FROM_LADY_MCCOYS_ROOM,
     CONDITION_ALWAYS, 88,
     0, 200,
     FACING_NORTH, 0,
     0, 16,
     240, 240,
     0},
	{MCCOY_PALACE_EAST_BEDROOM_FROM_LADY_MCCOYS_ROOM,
     CONDITION_ALWAYS, 360,
     0, 200,
     FACING_NORTH, 0,
     208, 16,
     448, 240,
     0},
	{MCCOY_PALACE_NORTHEAST_LANDING_FROM_ENTRANCE_ROOM,
     CONDITION_ALWAYS, 536,
     0, 112,
     FACING_EAST, 0,
     400, 0,
     640, 160,
     0},
	{MCCOY_PALACE_NORTHEAST_LANDING_FROM_NORTHEAST_TOWER,
     CONDITION_ALWAYS, 536,
     0, 88,
     FACING_EAST, 0,
     400, 0,
     640, 160,
     0},
	{MCCOY_PALACE_NORTHWEST_LANDING_FROM_ENTRANCE_ROOM,
     CONDITION_ALWAYS, 520,
     0, 312,
     FACING_WEST, 0,
     400, 208,
     640, 368,
     0},
	{MCCOY_PALACE_NORTHWEST_LANDING_FROM_NORTHWEST_TOWER,
     CONDITION_ALWAYS, 520,
     0, 296,
     FACING_WEST, 0,
     400, 208,
     640, 368,
     0},
	{MCCOY_PALACE_NORTHWEST_TOWER_FROM_NORTHWEST_LANDING,
     CONDITION_ALWAYS, 728,
     0, 264,
     FACING_EAST, 0,
     592, 192,
     832, 352,
     0},
	{MCCOY_PALACE_NORTHEAST_TOWER_FROM_NORTHEAST_LANDING,
     CONDITION_ALWAYS, 904,
     0, 264,
     FACING_WEST, 0,
     792, 192,
     1032, 352,
     0},
	{MCCOY_PALACE_SOUTHWEST_TOWER_FROM_SOUTHWEST_LANDING,
     CONDITION_ALWAYS, 744,
     0, 424,
     FACING_EAST, 0,
     592, 352,
     832, 512,
     0},
	{MCCOY_PALACE_SOUTHEAST_TOWER_FROM_SOUTHEAST_LANDING,
     CONDITION_ALWAYS, 888,
     0, 424,
     FACING_WEST, 0,
     792, 352,
     1032, 512,
     0},
	{MCCOY_PALACE_TREASURE_ROOM_FROM_ENTRANCE_ROOM,
     CONDITION_ALWAYS, 600,
     0, 760,
     FACING_EAST, 0,
     512, 560,
     752, 816,
     0},
	{MCCOY_PALACE_FROM_MCCOY_PALACE_GROUNDS_EAST,
     CONDITION_ALWAYS, 896,
     0, 728,
     FACING_NORTH, 0,
     776, 560,
     1016, 768,
     0},
	{SCENE_TABLE_END},
};

Uint32 McCoyPalaceExits[] = {
	SCENE_EXITS(McCoyPalace),
	SCENE_EXIT(MCCOY_PALACE_TO_MCCOY_PALACE_GROUNDS_WEST,
               McCoyPalaceGrounds,
               MCCOY_PALACE_GROUNDS_FROM_MCCOY_PALACE),
	SCENE_EXIT(MCCOY_PALACE_ENTRANCE_ROOM_TO_SOUTHWEST_LANDING,
               McCoyPalace,
               MCCOY_PALACE_SOUTHWEST_LANDING_FROM_ENTRANCE_ROOM),
	SCENE_EXIT(MCCOY_PALACE_ENTRANCE_ROOM_TO_SOUTHEAST_LANDING,
               McCoyPalace,
               MCCOY_PALACE_SOUTHEAST_LANDING_FROM_ENTRANCE_ROOM),
	SCENE_EXIT(MCCOY_PALACE_ENTRANCE_ROOM_TO_LADY_MCCOYS_ROOM_WEST,
               McCoyPalace,
               MCCOY_PALACE_LADY_MCCOYS_ROOM_FROM_ENTRANCE_ROOM_WEST),
	SCENE_EXIT(MCCOY_PALACE_ENTRANCE_ROOM_TO_LADY_MCCOYS_ROOM_EAST,
               McCoyPalace,
               MCCOY_PALACE_LADY_MCCOYS_ROOM_FROM_ENTRANCE_ROOM_EAST),
	SCENE_EXIT(MCCOY_PALACE_ENTRANCE_ROOM_TO_TREASURE_ROOM,
               McCoyPalace,
               MCCOY_PALACE_TREASURE_ROOM_FROM_ENTRANCE_ROOM),
	SCENE_EXIT(MCCOY_PALACE_ENTRANCE_ROOM_TO_NORTHWEST_LANDING,
               McCoyPalace,
               MCCOY_PALACE_NORTHWEST_LANDING_FROM_ENTRANCE_ROOM),
	SCENE_EXIT(MCCOY_PALACE_ENTRANCE_ROOM_TO_NORTHEAST_LANDING,
               McCoyPalace,
               MCCOY_PALACE_NORTHEAST_LANDING_FROM_ENTRANCE_ROOM),
	SCENE_EXIT(MCCOY_PALACE_ENTRANCE_ROOM_TO_MCCOYS_ROOM,
               McCoyPalace,
               MCCOY_PALACE_FROM_MCCOY_PALACE_GROUNDS_EAST),
	SCENE_EXIT(MCCOY_PALACE_LADY_MCCOYS_ROOM_TO_ENTRANCE_ROOM_WEST,
               McCoyPalace,
               MCCOY_PALACE_ENTRANCE_ROOM_FROM_LADY_MCCOYS_ROOM_WEST),
	SCENE_EXIT(MCCOY_PALACE_LADY_MCCOYS_ROOM_TO_ENTRANCE_ROOM_EAST,
               McCoyPalace,
               MCCOY_PALACE_ENTRANCE_ROOM_FROM_LADY_MCCOYS_ROOM_EAST),
	SCENE_EXIT(MCCOY_PALACE_LADY_MCCOYS_ROOM_TO_WEST_BEDROOM,
               McCoyPalace,
               MCCOY_PALACE_WEST_BEDROOM_FROM_LADY_MCCOYS_ROOM),
	SCENE_EXIT(MCCOY_PALACE_LADY_MCCOYS_ROOM_TO_EAST_BEDROOM,
               McCoyPalace,
               MCCOY_PALACE_EAST_BEDROOM_FROM_LADY_MCCOYS_ROOM),
	SCENE_EXIT(MCCOY_PALACE_SOUTHWEST_LANDING_TO_ENTRANCE_ROOM,
               McCoyPalace,
               MCCOY_PALACE_ENTRANCE_ROOM_FROM_SOUTHWEST_LANDING),
	SCENE_EXIT(MCCOY_PALACE_SOUTHWEST_LANDING_TO_SOUTHWEST_TOWER,
               McCoyPalace,
               MCCOY_PALACE_SOUTHWEST_TOWER_FROM_SOUTHWEST_LANDING),
	SCENE_EXIT(MCCOY_PALACE_SOUTHEAST_LANDING_TO_SOUTHEAST_TOWER,
               McCoyPalace,
               MCCOY_PALACE_SOUTHEAST_TOWER_FROM_SOUTHEAST_LANDING),
	SCENE_EXIT(MCCOY_PALACE_SOUTHEAST_LANDING_TO_ENTRANCE_ROOM,
               McCoyPalace,
               MCCOY_PALACE_ENTRANCE_ROOM_FROM_SOUTHEAST_LANDING),
	SCENE_EXIT(MCCOY_PALACE_WEST_BEDROOM_TO_LADY_MCCOYS_ROOM,
               McCoyPalace,
               MCCOY_PALACE_LADY_MCCOYS_ROOM_FROM_WEST_BEDROOM),
	SCENE_EXIT(MCCOY_PALACE_EAST_BEDROOM_TO_LADY_MCCOYS_ROOM,
               McCoyPalace,
               MCCOY_PALACE_LADY_MCCOYS_ROOM_FROM_EAST_BEDROOM),
	SCENE_EXIT(MCCOY_PALACE_NORTHEAST_LANDING_TO_ENTRANCE_ROOM,
               McCoyPalace,
               MCCOY_PALACE_ENTRANCE_ROOM_FROM_NORTHEAST_LANDING),
	SCENE_EXIT(MCCOY_PALACE_NORTHEAST_LANDING_TO_NORTHEAST_TOWER,
               McCoyPalace,
               MCCOY_PALACE_NORTHEAST_TOWER_FROM_NORTHEAST_LANDING),
	SCENE_EXIT(MCCOY_PALACE_NORTHWEST_LANDING_TO_ENTRANCE_ROOM,
               McCoyPalace,
               MCCOY_PALACE_ENTRANCE_ROOM_FROM_NORTHWEST_LANDING),
	SCENE_EXIT(MCCOY_PALACE_NORTHWEST_LANDING_TO_NORTHWEST_TOWER,
               McCoyPalace,
               MCCOY_PALACE_NORTHWEST_TOWER_FROM_NORTHWEST_LANDING),
	SCENE_EXIT(MCCOY_PALACE_NORTHWEST_TOWER_TO_NORTHWEST_LANDING,
               McCoyPalace,
               MCCOY_PALACE_NORTHWEST_LANDING_FROM_NORTHWEST_TOWER),
	SCENE_EXIT(MCCOY_PALACE_NORTHEAST_TOWER_TO_NORTHEAST_LANDING,
               McCoyPalace,
               MCCOY_PALACE_NORTHEAST_LANDING_FROM_NORTHEAST_TOWER),
	SCENE_EXIT(MCCOY_PALACE_SOUTHWEST_TOWER_TO_SOUTHWEST_LANDING,
               McCoyPalace,
               MCCOY_PALACE_SOUTHWEST_LANDING_FROM_SOUTHWEST_TOWER),
	SCENE_EXIT(MCCOY_PALACE_SOUTHEAST_TOWER_TO_SOUTHEAST_LANDING,
               McCoyPalace,
               MCCOY_PALACE_SOUTHEAST_LANDING_FROM_SOUTHEAST_TOWER),
	SCENE_EXIT(MCCOY_PALACE_TREASURE_ROOM_TO_ENTRANCE_ROOM,
               McCoyPalace,
               MCCOY_PALACE_ENTRANCE_ROOM_FROM_TREASURE_ROOM),
	SCENE_EXIT(MCCOY_PALACE_MCCOYS_ROOM_TO_ENTRANCE_ROOM,
               McCoyPalace,
               MCCOY_PALACE_ENTRANCE_ROOM_FROM_MCCOYS_ROOM),
	SCENE_EXITS_END,
};

// Unused: the getters return it for any scene but SceneIdMcCoyPalace, and the overlay serves no
// other.
ACTOR_PLACE McCoyPalaceNoActorPlacements[] = {
	{SCENE_TABLE_END},
};

// clang-format off
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

// Unused, as McCoyPalaceNoActorPlacements is.
CUE McCoyPalaceNoCues[] = {
	{ CUE_TABLE_END },
};

CUE McCoyPalaceCues[] = {
	{ CUE_EXIT, 1, CONDITION_ALWAYS, MCCOY_PALACE_TO_MCCOY_PALACE_GROUNDS_WEST },
	{ CUE_EXIT, 2, CONDITION_ALWAYS, MCCOY_PALACE_ENTRANCE_ROOM_TO_SOUTHWEST_LANDING },
	{ CUE_EXIT, 3, CONDITION_ALWAYS, MCCOY_PALACE_ENTRANCE_ROOM_TO_SOUTHEAST_LANDING },
	{ CUE_EXIT, 4, CONDITION_ALWAYS, MCCOY_PALACE_ENTRANCE_ROOM_TO_LADY_MCCOYS_ROOM_WEST },
	{ CUE_EXIT, 5, CONDITION_ALWAYS, MCCOY_PALACE_ENTRANCE_ROOM_TO_LADY_MCCOYS_ROOM_EAST },
	{ CUE_EXIT, 6, CONDITION_ALWAYS, MCCOY_PALACE_ENTRANCE_ROOM_TO_TREASURE_ROOM },
	{ CUE_EXIT, 7, CONDITION_ALWAYS, MCCOY_PALACE_ENTRANCE_ROOM_TO_NORTHWEST_LANDING },
	{ CUE_EXIT, 8, CONDITION_ALWAYS, MCCOY_PALACE_ENTRANCE_ROOM_TO_NORTHEAST_LANDING },
	{ STEP_CUE_FACING(FACING_NORTH), MCCOY_PALACE_ENTRANCE_ROOM_TO_MCCOYS_ROOM, CONDITION_ALWAYS, SCRIPT_REF(scMcCoyPalaceWalkThroughDoor) },
	{ CUE_EXIT, 10, CONDITION_ALWAYS, MCCOY_PALACE_LADY_MCCOYS_ROOM_TO_ENTRANCE_ROOM_WEST },
	{ CUE_EXIT, 11, CONDITION_ALWAYS, MCCOY_PALACE_LADY_MCCOYS_ROOM_TO_ENTRANCE_ROOM_EAST },
	{ STEP_CUE_FACING(FACING_NORTH), MCCOY_PALACE_LADY_MCCOYS_ROOM_TO_WEST_BEDROOM, CONDITION_ALWAYS, SCRIPT_REF(scMcCoyPalaceWalkThroughDoor) },
	{ STEP_CUE_FACING(FACING_NORTH), MCCOY_PALACE_LADY_MCCOYS_ROOM_TO_EAST_BEDROOM, CONDITION_ALWAYS, SCRIPT_REF(scMcCoyPalaceWalkThroughDoor) },
	{ CUE_EXIT, 14, CONDITION_ALWAYS, MCCOY_PALACE_SOUTHWEST_LANDING_TO_ENTRANCE_ROOM },
	{ CUE_EXIT, 15, CONDITION_ALWAYS, MCCOY_PALACE_SOUTHWEST_LANDING_TO_SOUTHWEST_TOWER },
	{ CUE_EXIT, 16, CONDITION_ALWAYS, MCCOY_PALACE_SOUTHEAST_LANDING_TO_SOUTHEAST_TOWER },
	{ CUE_EXIT, 17, CONDITION_ALWAYS, MCCOY_PALACE_SOUTHEAST_LANDING_TO_ENTRANCE_ROOM },
	{ CUE_EXIT, 18, CONDITION_ALWAYS, MCCOY_PALACE_WEST_BEDROOM_TO_LADY_MCCOYS_ROOM },
	{ CUE_EXIT, 19, CONDITION_ALWAYS, MCCOY_PALACE_EAST_BEDROOM_TO_LADY_MCCOYS_ROOM },
	{ CUE_EXIT, 20, CONDITION_ALWAYS, MCCOY_PALACE_NORTHEAST_LANDING_TO_ENTRANCE_ROOM },
	{ CUE_EXIT, 21, CONDITION_ALWAYS, MCCOY_PALACE_NORTHEAST_LANDING_TO_NORTHEAST_TOWER },
	{ CUE_EXIT, 22, CONDITION_ALWAYS, MCCOY_PALACE_NORTHWEST_LANDING_TO_ENTRANCE_ROOM },
	{ CUE_EXIT, 23, CONDITION_ALWAYS, MCCOY_PALACE_NORTHWEST_LANDING_TO_NORTHWEST_TOWER },
	{ CUE_EXIT, 24, CONDITION_ALWAYS, MCCOY_PALACE_NORTHWEST_TOWER_TO_NORTHWEST_LANDING },
	{ CUE_EXIT, 25, CONDITION_ALWAYS, MCCOY_PALACE_NORTHEAST_TOWER_TO_NORTHEAST_LANDING },
	{ CUE_EXIT, 26, CONDITION_ALWAYS, MCCOY_PALACE_SOUTHWEST_TOWER_TO_SOUTHWEST_LANDING },
	{ CUE_EXIT, 27, CONDITION_ALWAYS, MCCOY_PALACE_SOUTHEAST_TOWER_TO_SOUTHEAST_LANDING },
	{ CUE_EXIT, 28, CONDITION_ALWAYS, MCCOY_PALACE_TREASURE_ROOM_TO_ENTRANCE_ROOM },
	{ CUE_EXIT, 29, CONDITION_ALWAYS, MCCOY_PALACE_MCCOYS_ROOM_TO_ENTRANCE_ROOM },
	{ CUE_TALK, ACTOR_VILLAGER_FOLLOW_RED_CARPET, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinIfYouWantToSeeLord) },
	{ CUE_TALK, ACTOR_VILLAGER_FOLLOW_RED_CARPET, CONDITION_ALWAYS, MSGNO(MsgBilibinThePeopleOfBilibinWillNever) },
	{ CUE_TALK, ACTOR_VILLAGER_DO_NOT_OFFEND_MCCOY, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinMakeSureYouDontOffendLord) },
	{ CUE_TALK, ACTOR_VILLAGER_DO_NOT_OFFEND_MCCOY, CONDITION_ALWAYS, MSGNO(MsgBilibinIHeardYouReceivedASpectacular) },
	{ CUE_TALK, ACTOR_VILLAGER_ASKING_ABOUT_KOLIMA_FOREST, FLAG_MCCOY_PALACE_REWARD_TAKEN, SCRIPT_REF(scMcCoyPalaceAskAboutGoingToKolima) },
	{ CUE_TALK, ACTOR_VILLAGER_ASKING_ABOUT_KOLIMA_FOREST, CONDITION_ALWAYS, MSGNO(MsgBilibinLadyMccoyIsQuiteUpsetThat) },
	{ CUE_TALK, ACTOR_YOUNG_MAN_PROTECT_OLD_TREE, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinIAlwaysBelievedThatTheOld) },
	{ CUE_TALK, ACTOR_YOUNG_MAN_PROTECT_OLD_TREE, CONDITION_ALWAYS, MSGNO(MsgBilibinThingsAreLookingGrimSouthOf) },
	{ CUE_TALK, ACTOR_VILLAGER_LADY_MCCOYS_ROOM, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinThisIsLadyMccoysRoomSo) },
	{ CUE_TALK, ACTOR_VILLAGER_LADY_MCCOYS_ROOM, CONDITION_ALWAYS, MSGNO(MsgBilibinMiladyWasVeryUpsetWhenShe) },
	{ CUE_TALK, ACTOR_VILLAGER_MILADYS_BEAUTY, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinMiladyWasRenownedFarAndWide) },
	{ CUE_TALK, ACTOR_VILLAGER_MILADYS_BEAUTY, CONDITION_ALWAYS, MSGNO(MsgBilibinForOnceMilordDidntLetLady) },
	{ CUE_TALK, ACTOR_VILLAGER_IN_MILADYS_NIGHTGOWN, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinOhSomedayIWantToSleep) },
	{ CUE_TALK, ACTOR_VILLAGER_IN_MILADYS_NIGHTGOWN, CONDITION_ALWAYS, SCRIPT_REF(scMcCoyPalaceTalkToWomanInMiladysNightgown) },
	{ CUE_TALK, ACTOR_OLD_MAN_PRAISING_THE_WARRIORS, FLAG_MCCOY_PALACE_BARRICADE_NEEDS_NO_KEY, MSGNO(MsgBilibinYouCanSeeKolimaPrettyWell) },
	{ CUE_TALK, ACTOR_OLD_MAN_PRAISING_THE_WARRIORS, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinHeyIfYouCantDoThe) },
	{ CUE_TALK, ACTOR_OLD_MAN_PRAISING_THE_WARRIORS, CONDITION_ALWAYS, SCRIPT_REF(scMcCoyPalaceTalkToOldManPraisingTheWarriors) },
	{ CUE_TALK, ACTOR_SOLDIER_WATCHING_KOLIMA, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinHmmItSeemsTheCurseOf) },
	{ CUE_TALK, ACTOR_SOLDIER_WATCHING_KOLIMA, CONDITION_ALWAYS, SCRIPT_REF(scMcCoyPalaceTalkToSoldierWatchingKolima) },
	{ CUE_TALK, ACTOR_CAPTAIN, CONDITION_ALWAYS, MSGNO(MsgBilibinYouMaySelectASingleItem) },
	{ CUE_TALK, ACTOR_SOLDIER_SHOWING_THE_REWARD, CONDITION_ALWAYS, SCRIPT_REF(scMcCoyPalaceTalkToSoldierShowingTheReward) },
	{ CUE_TALK, ACTOR_LORD_MCCOY, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinSorryLadsButKolimasFutureIs) },
	{ CUE_TALK, ACTOR_LORD_MCCOY, CONDITION_ALWAYS, MSGNO(MsgBilibinYehveTaughtMeThatICannae) },
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
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_PRAISING_THE_WARRIORS, CONDITION_ALWAYS, SCRIPT_REF(scMcCoyPalaceReadOldMansThoughtsOnTheWarriors) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SOLDIER_WATCHING_KOLIMA, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinKolimaForestUsedToBeSo) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SOLDIER_WATCHING_KOLIMA, CONDITION_ALWAYS, MSGNO(MsgBilibinACourageousWarriorWouldntThinkTwice) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CAPTAIN, CONDITION_ALWAYS, MSGNO(MsgBilibinTheseCourageousWarriorsDeserveToReceive) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SOLDIER_SHOWING_THE_REWARD, CONDITION_ALWAYS, SCRIPT_REF(scMcCoyPalaceReadRewardSoldiersThoughts) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LORD_MCCOY, FLAG_MCCOY_PALACE_REWARD_TAKEN, MSGNO(MsgBilibinThereIsNothingMoreToTalk) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LORD_MCCOY, CONDITION_ALWAYS, MSGNO(MsgBilibinButICannaeFiggerOutHow) },
	{ EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_MCCOY_PALACE_VIAL_CHEST, CONDITION_ALWAYS, SCRIPT_REF(scMcCoyPalaceSearchVialChest) },
	{ EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_MCCOY_PALACE_POTION_CHEST, CONDITION_ALWAYS, SCRIPT_REF(scMcCoyPalaceSearchPotionChest) },
	{ EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_MCCOY_PALACE_PSY_CRYSTAL_CHEST, CONDITION_ALWAYS, SCRIPT_REF(scMcCoyPalaceSearchPsyCrystalChest) },
	{ EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_MCCOY_PALACE_WATER_OF_LIFE_CHEST, CONDITION_ALWAYS, SCRIPT_REF(scMcCoyPalaceSearchWaterOfLifeChest) },
	{ CUE_STEP, METATILE_BEHAVIOR_MCCOY_PALACE_BEFORE_REWARD_CHESTS, FLAG_PARTY_SEEN_OFF, SCRIPT_REF(scMcCoyPalaceCaptainSeesPartyOff) },
	{ CUE_STEP, METATILE_BEHAVIOR_MCCOY_PALACE_FOOT_OF_TREASURE_STAIRS, FLAG_PARTY_SEEN_OFF, SCRIPT_REF(scMcCoyPalaceCaptainAsksPartyToChoose) },
	{ EXAMINE_CUE(EXAMINE_BARREL), METATILE_BEHAVIOR_MCCOY_PALACE_ELIXIR_BARREL, FLAG_MCCOY_PALACE_BARREL_ELIXIR, FIND_ITEM(ITEM_ELIXIR) },
	{ EXAMINE_CUE(EXAMINE_BARREL), METATILE_BEHAVIOR_MCCOY_PALACE_SMOKE_BOMB_BARREL, FLAG_MCCOY_PALACE_BARREL_SMOKE_BOMB, FIND_ITEM(ITEM_SMOKE_BOMB) },
	{ EXAMINE_CUE(EXAMINE_WARDROBE), METATILE_BEHAVIOR_MCCOY_PALACE_WARDROBE, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBilibinTheresASilkNegligeeIsaacPut)) },
	{ EXAMINE_CUE(EXAMINE_BOOKCASE), METATILE_BEHAVIOR_MCCOY_PALACE_BOOKCASE, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBilibinTheresASheetOfPaperTitled)) },
	{ CUE_TABLE_END },
};
// clang-format on
