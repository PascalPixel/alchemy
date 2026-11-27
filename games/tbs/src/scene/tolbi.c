/* @file */
/* Tolbi, the town Babi rules, where Colosso, a festival of warriors, is held */
/* each year in the colosseum. */
/* */
/* The party comes to Tolbi across the Karagol Sea from Kalay while the */
/* festival is on, and walks in from the south. Stairs lead up to the */
/* square, where Tolbi Spring plays in the middle; the road north passes */
/* under an archway to the Colosseum, stairs in the north-west climb to */
/* Babi's Palace, and the inn stands in the north-east. The town's talk */
/* follows the Colosso story in three stages. */
/* */
/* scGetTriggers gives each stage its own trigger table. One actor */
/* placement table serves all three, and scInit moves the */
/* actors whose places change. */
/* */
/* - The festival (neither story flag set). The trials have begun at the */
/*   colosseum, but Babi is not there, and his soldiers are searching the */
/*   town. The first time the party heads up the path toward Babi's Palace, */
/*   two of them stop it to show his picture, `Have you seen anyone who looks */
/*   like this?`, and go their way whatever it answers */
/*   (scTolbiMeetSearchingSoldiers, FLAG_TOLBI_SOLDIERS_ASKED_ABOUT_BABI). */
/* - Finals day (FLAG_BABI_SAVED), once the party has saved Babi in */
/*   Altmiller Cave. He has shown himself at the finals, and the town hurries */
/*   to the colosseum. The scalper who slept under the archway has moved, */
/*   sending people to his buddy for seats. */
/* - After Colosso (FLAG_COLOSSO_OVER). The visitors have left, so the family */
/*   seen at the festival and four others are moved off the map. Those who */
/*   stay talk of the quiet and of the ships that no longer sail the */
/*   Karagol. The snack vendor gives away his leftover corn, once */
/*   (FLAG_TOLBI_CORN_GIVEN). */
/* */
/* In every stage two children run a small loop south-east of the fountain */
/* (TolbiOlderBrotherRun, TolbiYoungerSisterRun) and quarrel over their */
/* allowance. Each stops when it runs up behind the other or finds the leader */
/* in its way (scTolbiUpdateKid). */
/* */
/* A visitor testing his luck stands at the gate in the fountain's railing, */
/* between its two signs. Once the party agrees to go first he steps aside for
 */
/* good (scTolbiTalkToSpringVisitor, FLAG_TOLBI_SPRING_OPENED), and the gate */
/* leads up to Tolbi Spring, where a coin or Lucky Medal is tossed in for a */
/* prize (TOLBI_TO_TOLBI_SPRING). */
/* */
/* Ember, a Mars Djinni, waits on the high ground east of the inn and joins the
 */
/* party when spoken to. The way there: Growth on the sprout on the town's east
 */
/* side grows a vine to climb, Frost on the puddle above it makes an ice pillar
 */
/* to hop across. Each is a scene flag, kept only through a return that keeps */
/* the party's position. Reveal on the gravestones in the south-west uncovers */
/* a chest with Power Bread. */
/* */
/* A barrel near the stairs to Babi's Palace holds 9 coins, and a jar behind */
/* the easternmost house a Lucky Medal. During the festival, the editions */
/* from TBS_PATCH_1 on find the jar only when the party faces north. */
/* */
/* The doors, whose cells carry their exits' numbers, lead into Tolbi's */
/* houses, the inn, the weapon and armor shop and Tolbi Sanctum (all */
/* SceneId_StuccoRoomsB), and into the Lucky Dice and Lucky Wheels halls */
/* (SceneId_StuccoRoomsC, whose area name is also Tolbi). The stairs in the */
/* north-west lead to Babi's Palace, the road north to the Colosseum, and the */
/* town's south and east edges out onto the World Map. */

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "sounds.h"
#include "field/event_inline.h"
#include "field/field_effect.h"
#include "field/map_grid.h"
#include "scene/vault_range.h"
#include "text/msg_ids.h"

extern s32 TolbiOlderBrotherRun[];
extern s32 TolbiYoungerSisterRun[];
extern SCENE_ENTRANCE TolbiEntrances[];
extern u32 TolbiExits[];
extern ACTOR_PLACE TolbiActorPlacements[];
extern TRIGGER TolbiTriggersFestival[];
extern TRIGGER TolbiTriggersFinalsDay[];
extern TRIGGER TolbiTriggersAfterColosso[];
extern MAP_GRID_CELL_ANIMATION TolbiDoors[];

/* Growth has raised the sprout on the town's east side into a vine the leader
 */
/* can climb. */
#define FLAG_TOLBI_SPROUT_GROWN FLAG_SCENE_TEMP_00
/* Frost has frozen the puddle north of the sprout into an ice pillar. */
#define FLAG_TOLBI_PUDDLE_FROZEN FLAG_SCENE_TEMP_01

/* Actors, numbered in the order of the actor placements from */
/* ACTOR_FIRST_PLACED. The townspeople the text gives no name are named by */
/* what their sprites show and what they say or, under Mind Read, think */
/* during the festival. */
typedef enum TOLBI_ACTOR_ID {
	/* The sprout on the town's east side, which Growth grows into a vine */
	/* (scTolbiGrowSprout). */
	ACTOR_TOLBI_SPROUT = ACTOR_FIRST_PLACED,
	/* The puddle just north of the sprout, which Frost freezes into an ice */
	/* pillar (scTolbiFreezePuddle). */
	ACTOR_TOLBI_PUDDLE,
	/* A young man with brown hair (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR). */
	ACTOR_TOLBI_EXCITED_YOUNG_MAN,
	/* A woman with fair hair (SPRITE_WOMAN_WITH_FAIR_HAIR). */
	ACTOR_TOLBI_HURRYING_WOMAN,
	/* A burly grey-haired man (SPRITE_BURLY_MAN_WITH_GREY_HAIR) by the market
	 */
	/* stalls north-east of the fountain; after Colosso he asks whether the */
	/* party was in the finals (scTolbiAskWhetherPartyWasInFinals). */
	ACTOR_TOLBI_FINALS_FAN,
	/* A woman in a green dress (SPRITE_WOMAN_IN_GREEN_DRESS). */
	ACTOR_TOLBI_WORRIED_WOMAN,
	/* An old man with a cane (SPRITE_OLD_MAN_WITH_CANE), who says he is 65 and
	 */
	/* later talks of the Lucky Guess. He copies the leader's sprite priority */
	/* every frame (scTolbiCopyLeaderPriority). */
	ACTOR_TOLBI_OLD_MAN,
	/* An old woman (SPRITE_OLD_WOMAN) who asks whether someone can live past */
	/* a hundred (scTolbiAskAboutLivingPastHundred). */
	ACTOR_TOLBI_SUSPICIOUS_OLD_WOMAN,
	/* A small boy with fair hair (SPRITE_BOY_WITH_FAIR_HAIR), who runs after */
	/* his sister. */
	ACTOR_TOLBI_OLDER_BROTHER,
	/* A little girl with pigtails (SPRITE_GIRL_WITH_PIGTAILS). */
	ACTOR_TOLBI_YOUNGER_SISTER,
	/* A strongman (SPRITE_STRONGMAN). */
	ACTOR_TOLBI_SOUVENIR_SELLER,
	/* A stout man (SPRITE_STOUT_MAN). */
	ACTOR_TOLBI_SNACK_VENDOR,
	/* A young man with a headband (SPRITE_YOUTH_WITH_HEADBAND). */
	ACTOR_TOLBI_WOULD_BE_FIGHTER,
	/* A young man with a headband (SPRITE_YOUTH_WITH_HEADBAND), whom Mind Read
	 */
	/* finds thinking of his home. */
	ACTOR_TOLBI_XIAN_VISITOR,
	/* Two people with swords (SPRITE_PERSON_WITH_SWORD), the first of whom */
	/* says "We overslept"; the second has no trigger row. */
	ACTOR_TOLBI_OVERSLEEPER,
	ACTOR_TOLBI_SECOND_OVERSLEEPER,
	/* A family at the festival: the father, a burly grey-haired man */
	/* (SPRITE_BURLY_MAN_WITH_GREY_HAIR), the mother, a woman in a green dress
	 */
	/* (SPRITE_WOMAN_IN_GREEN_DRESS), and their small boy */
	/* (SPRITE_BOY_WITH_FAIR_HAIR), who cries for sweets. */
	ACTOR_TOLBI_FATHER,
	ACTOR_TOLBI_MOTHER,
	ACTOR_TOLBI_CRYING_BOY,
	/* A strongman (SPRITE_STRONGMAN) who sleeps under the archway on the road
	 */
	/* to the Colosseum during the festival; on finals day he stands by it and
	 */
	/* sends people to his buddy for seats. */
	ACTOR_TOLBI_SCALPER,
	/* A man carrying a pack (SPRITE_MAN_CARRYING_A_PACK) at the gate in the */
	/* fountain's railing, who lets the party go first at Tolbi Spring. */
	ACTOR_TOLBI_SPRING_VISITOR,
	/* Babi's soldiers (SPRITE_SOLDIER), searching the town for him and */
	/* showing his picture (scTolbiMeetSearchingSoldiers). */
	ACTOR_TOLBI_SOLDIER_A,
	/* The second of the two soldiers. */
	ACTOR_TOLBI_SOLDIER_B,
	/* Ember, a Mars Djinni, on the high ground east of the inn until it joins
	 */
	/* the party (scTolbiJoinEmber). */
	ACTOR_TOLBI_EMBER,
	/* The "Missing Person" the soldiers ask about, placed with Babi's sprite */
	/* (SPRITE_BABI) and kept hidden: the speaker of their portrait window. */
	ACTOR_TOLBI_MISSING_PERSON,
} TOLBI_ACTOR_ID;

/* The cell behaviors Tolbi's coord and examine triggers answer besides the */
/* doors, whose own cells carry the numbers of their exits (scTolbiEnterDoor, */
/* scTolbiRequestTouchedExit). */
typedef enum TOLBI_CELL_BEHAVIOR {
	/* On the path up to Babi's Palace, cells (5, 15) and (5, 16), where Babi's
	 */
	/* soldiers stop the party during the festival. */
	CELL_BEHAVIOR_TOLBI_SOLDIERS = 20,
	/* The signs either side of the gate in the fountain's railing: the west */
	/* one (17, 17) on tossing a coin, the east one (20, 17) on Lucky Medals. */
	CELL_BEHAVIOR_TOLBI_SPRING_SIGN = 30,
	CELL_BEHAVIOR_TOLBI_LUCKY_MEDAL_SIGN = 31,
	/* The barrel near the stairs to Babi's Palace (8, 9). */
	CELL_BEHAVIOR_TOLBI_BARREL = 100,
	/* The jar behind the easternmost house (30, 18). */
	CELL_BEHAVIOR_TOLBI_JAR = 101,
	/* The chest among the gravestones in the south-west, kept off the drawn */
	/* map at (3, 32) until Reveal copies it to (3, 26). */
	CELL_BEHAVIOR_TOLBI_HIDDEN_CHEST = 102,
	/* The gate in the fountain's railing (18, 18) to (19, 18), open once the */
	/* visitor there has stepped aside. */
	CELL_BEHAVIOR_TOLBI_GATE_TO_SPRING = 239,
	/* The town's south and east edges, out onto the World Map. */
	CELL_BEHAVIOR_TOLBI_EDGE = 12
} TOLBI_CELL_BEHAVIOR;

/* The running children keep these flags in ACTOR.tmr, their scene */
/* scratch halfword. Bits: 1 talking | 0 chases the younger sister. */
enum {
	/* Set on the older brother, who runs after ACTOR_TOLBI_YOUNGER_SISTER. */
	KID_CHASES_YOUNGER = 1,
	KID_TALKING = 2, /* set while the party talks to this child */
};

/* How the town's jar is searched: from patch 1 only facing north. */
#if TBS_PATCH_1
#define TOLBI_JAR_SEARCH_KIND                                                  \
	(EXAMINE_TRIGGER(EXAMINE_JAR) | TRIGGER_FLAG_FACING | FACING_NORTH)
#else
#define TOLBI_JAR_SEARCH_KIND EXAMINE_TRIGGER(EXAMINE_JAR)
#endif

/* Frost on the puddle on Tolbi's east side, just north of the sprout (cell */
/* (34, 16)), freezes it into an ice pillar to hop across. */
static void scTolbiFreezePuddle(void);

/* Talking to Ember, the Mars Djinni on the high ground east of Tolbi's inn */
/* (cell (36, 8)), makes it join the party. */
static void scTolbiJoinEmber(void) {
	fdDjinnJoinParty(ACTOR_TOLBI_EMBER, ELEMENT_MARS,
					 DJINNI_INDEX(DJINNI_EMBER));
}

#include "shared/actor_distance.c"

/* Stops a running child when the target is close by and ahead of it, or */
/* always when force is set. */
/* */
/* The child stops when the target is within one facing step of where it */
/* looks. For the party leader, that is within two steps either side. */
/* */
/* Returns 1 when the child stopped. */
static s32 scTolbiKidStopForTarget(ACTOR *kid, ACTOR *target, s32 range,
								   s32 force) {
	s32 stopped = 0;

	if(fdActorDistance(target, kid) < range || force != 0) {
		/* The heading to the target, and the eight-way directions around it. */
		u32 heading = (u16)syAtan(target->z.fixed - kid->z.fixed,
								  target->x.fixed - kid->x.fixed);
		u32 farLeft = FACING_SIXTEENTH(heading - 2 * FACING_STEP);
		u32 farRight = FACING_SIXTEENTH(heading + 2 * FACING_STEP);
		u32 left = FACING_SIXTEENTH(heading - FACING_STEP);
		u32 right = FACING_SIXTEENTH(heading + FACING_STEP);
		u32 ahead = FACING_SIXTEENTH(heading);
		u32 facing = FACING_SIXTEENTH(kid->facing);

		if(ahead == facing || right == facing || left == facing || force != 0) {
			kid->paused = 1;
			fdActorSelectAnimation(kid, ANIM_STAND);
			stopped = 1;
		}
		if(target == fdActorGet(ACTOR_PARTY_LEADER) &&
		   (farRight == facing || farLeft == facing)) {
			kid->paused = 1;
			fdActorSelectAnimation(kid, ANIM_STAND);
			stopped = 1;
		}
	}
	else {
		kid->paused = 0;
		fdActorSelectAnimation(kid, ANIM_WALK);
	}
	return stopped;
}

/* The update of the two children running their loop. Each stops when it runs */
/* up behind the other, and otherwise when the party leader is in its way. */
/* While the party talks to someone or Mind Read shows thoughts (the range */
/* widens to TALK_STOP_RANGE), the child being talked to stops whatever the */
/* direction. */
static UNREAD_RESULT scTolbiUpdateKid(ACTOR *updated) {
	WIN_STATE *window = (WIN_STATE *)WindowStateWork[0];
	FIELD_STATE *state = FieldStateWork;
	s32 force = 0;
	s32 range = STOP_RANGE;
	ACTOR *other;
	ACTOR *leader;

	if((updated->tmr & KID_CHASES_YOUNGER) != 0) {
		other = fdActorGet(ACTOR_TOLBI_YOUNGER_SISTER);
	}
	else {
		other = fdActorGet(ACTOR_TOLBI_OLDER_BROTHER);
	}
	if(scTolbiKidStopForTarget(updated, other, PARTNER_STOP_RANGE, 0) != 0) {
		RETURN_UNREAD(0);
	}

	leader = fdActorGet(ACTOR_PARTY_LEADER);

	if((s16)state->talk != 0 || window->mindRead != 0) {
		range = TALK_STOP_RANGE;
		if((updated->tmr & KID_TALKING) != 0) {
			force = 1;
		}
	}

	scTolbiKidStopForTarget(updated, leader, range, force);
	RETURN_UNREAD(0);
}

const SCENE_ENTRANCE *scGetEntrances(void) { return TolbiEntrances; }

const SCENE_EXIT_REGION *scGetExitRegions(void) { return NULL; }

const u32 *scGetExits(void) { return TolbiExits; }

const ACTOR_PLACE *scGetActorPlacements(void) { return TolbiActorPlacements; }

/* Growth on the sprout on Tolbi's east side (cell (34, 18)) grows a vine the */
/* party can climb, the way up toward the puddle and Ember. */
static void scTolbiGrowSprout(void) {
	ACTOR *sprout;

	sprout = fdActorGet(ACTOR_TOLBI_SPROUT);
	if(sprout != NULL) {
		sprout->collisionFlg = 0;
	}
	fdActorSetSprFlags(fdActorGet(ACTOR_TOLBI_SPROUT), 0);
	fdSetCellBehaviorAt(0, 544, 288, CELL_BEHAVIOR_CLIMB);
	gmFlagSet(FLAG_TOLBI_SPROUT_GROWN);
}

/* Reveal among the gravestones in the south-west of Tolbi uncovers a chest */
/* (cell (3, 26)) holding Power Bread. */
static void scTolbiRevealHiddenChest(void) {
	fdMapGridCopyCellAttributes(3, 32, 1, 1, 3, 26);
	fdTreasureSpotSetPos(CELL_BEHAVIOR_TOLBI_HIDDEN_CHEST, toFIXED(56),
						 toFIXED(424));
}

/* When Reveal wears off, the chest among the gravestones in the south-west of
 */
/* Tolbi (cell (3, 26)) disappears again. */
static void scTolbiHideRevealedChest(void) {
	fdMapGridCopyCellAttributes(2, 25, 1, 1, 3, 26);
	fdTreasureSpotSetPos(CELL_BEHAVIOR_TOLBI_HIDDEN_CHEST, -1, -1);
}

/* The sign west of the gate in the fountain's railing says to face away from */
/* Tolbi Spring and toss a coin into it. */
static void scTolbiReadSpringSign(void) {
	fdEventSetMessageId(MsgTolbiFaceAwayTolbi);
	/* "Face away from Tolbi Spring and toss a coin into it. If you..." */
	fdEventShowMessage(-1, 0);
}

/* The sign east of the gate in the fountain's railing says a Lucky Medal */
/* tossed in can win better prizes. */
static void scTolbiReadLuckyMedalSign(void) {
	fdEventSetMessageId(MsgTolbiTossLuckyMedal);
	/* "If you toss a Lucky Medal and you get really lucky, you'll..." */
	fdEventShowMessage(-1, 0);
}

/* Talking to the boy running round the loop south-east of the fountain stops */
/* him to say what he will buy with the money Mama gave him; Mind Read finds */
/* him thinking the oldest should not have to share the allowance. */
/* */
/* On finals day and after Colosso he says and thinks other things. */
static void scTolbiTalkToOlderBrother(void) {
	FIELD_STATE *state = FieldStateWork;
	ACTOR *kid;
	s16 facing;
	s32 msg;

	kid = fdActorGet(ACTOR_TOLBI_OLDER_BROTHER);
	facing = kid->facing;
	fdEventStart();
	kid->tmr |= KID_TALKING;
	if(state->psynergy == 0) {
		if(gmFlagGet(FLAG_COLOSSO_OVER) != 0) {
			msg = MsgTolbiYayEasyRun;
		}
		else if(gmFlagGet(FLAG_BABI_SAVED) != 0) {
			msg = MsgTolbiHoorayFinalsSeven;
		}
		else {
			msg = MsgTolbiWheeFestivalColosso;
		}
	}
	else {
		if(gmFlagGet(FLAG_COLOSSO_OVER) != 0) {
			msg = MsgTolbiMainStreetTolbi;
		}
		else if(gmFlagGet(FLAG_BABI_SAVED) != 0) {
			msg = MsgTolbiWinBigUsed;
		}
		else {
			msg = MsgTolbiOldestShouldntShare;
		}
	}
	fdEventSetMessageId(msg);
	fdActorSetAnimation(ACTOR_TOLBI_OLDER_BROTHER, ANIM_STILL);
	fdActorFaceEachOther(ACTOR_TOLBI_OLDER_BROTHER, ACTOR_PARTY_LEADER, 2);
	fdEventShowMessageThenWait(ACTOR_TOLBI_OLDER_BROTHER, 0, 10);
	kid->facing = facing;
	sySynch(1);
	kid->tmr &= KID_CHASES_YOUNGER;
	fdEventEnd();
}

/* Talking to the girl running round the loop south-east of the fountain stops
 */
/* her to wail that the money was for both of them; Mind Read finds her */
/* thinking Mama told them to share. */
/* */
/* After Colosso she laughs that her big brother is being nice again. */
static void scTolbiTalkToYoungerSister(void) {
	FIELD_STATE *state = FieldStateWork;
	ACTOR *kid;
	s16 facing;
	s32 msg;

	kid = fdActorGet(ACTOR_TOLBI_YOUNGER_SISTER);
	facing = kid->facing;
	fdEventStart();
	kid->tmr |= KID_TALKING;
	if(state->psynergy == 0) {
		if(gmFlagGet(FLAG_COLOSSO_OVER) != 0) {
			msg = MsgTolbiHaHaHa;
		}
		else if(gmFlagGet(FLAG_BABI_SAVED) != 0) {
			msg = MsgTolbiWaahBigBrother;
		}
		else {
			msg = MsgTolbiWaahSaidMoney;
		}
	}
	else {
		if(gmFlagGet(FLAG_COLOSSO_OVER) != 0) {
			msg = MsgTolbiBrotherMeanDuring;
		}
		else if(gmFlagGet(FLAG_BABI_SAVED) != 0) {
			msg = MsgTolbiMamaWhyListen;
		}
		else {
			msg = MsgTolbiMamaToldShare;
		}
	}
	fdEventSetMessageId(msg);
	fdActorSetAnimation(ACTOR_TOLBI_YOUNGER_SISTER, ANIM_STILL);
	fdActorFaceEachOther(ACTOR_TOLBI_YOUNGER_SISTER, ACTOR_PARTY_LEADER, 2);
	fdEventShowMessageThenWait(ACTOR_TOLBI_YOUNGER_SISTER, 0, 10);
	kid->facing = facing;
	sySynch(1);
	kid->tmr &= KID_CHASES_YOUNGER;
	fdEventEnd();
}

/* Tolbi's events for the stage of the Colosso story: the festival, finals day
 */
/* or after Colosso. */
const TRIGGER *scGetTriggers(void) {
	if(gmFlagGet(FLAG_COLOSSO_OVER) != 0) {
		return TolbiTriggersAfterColosso;
	}
	if(gmFlagGet(FLAG_BABI_SAVED) != 0) {
		return TolbiTriggersFinalsDay;
	}
	return TolbiTriggersFestival;
}

/* The old woman in the south-west corner of Tolbi asks whether anyone can */
/* live past 100 years old, suspecting Babi is a lot older than that. */
static void scTolbiAskAboutLivingPastHundred(void) {
	fdEventStart();
	fdEventSetMessageId(MsgTolbiSomeoneLiveMore);
	/* "Can someone live to be more than 100 years old?" */
	/* Yes: "Hmmm. Still, I'm pretty sure Babi is a lot older than 100..." */
	/* No: "You could be right. But then, what about Tolbi's ruler,..." */
	fdEventAskYesNo(ACTOR_TOLBI_SUSPICIOUS_OLD_WOMAN, 0);
	fdEventEnd();
}

/* During the festival, the father of the family on the road north of the */
/* fountain asks whether children really do love souvenirs, and owns up to */
/* loving them himself. */
static void scTolbiTalkToFather(void) {
	fdEventStart();
	fdEventSetMessageId(MsgTolbiChildrenLoveSouvenirs);
	/* "Children really do love souvenirs, don't they?" */
	/* Yes: "I'll bet you like them, too. Don't try to hide it! I used to..." */
	/* No: "Hmph! You're only saying that because your parents never..." */
	fdEventAskYesNo(ACTOR_TOLBI_FATHER, 0);
	fdEventEnd();
}

/* During the festival, the mother of the family on the road north of the */
/* fountain turns to her crying boy, then back to promise better sweets later */
/* on. */
static void scTolbiTalkToMother(void) {
	fdEventStart();
	fdEventSetMessageId(MsgTolbiPatientLittleGuy);
	fdActorFaceDirection(ACTOR_TOLBI_MOTHER, FACING_NORTH, 0);
	/* "Be patient, little guy." */
	fdEventShowMessage(ACTOR_TOLBI_MOTHER, 0);
	fdActorFaceDirection(ACTOR_TOLBI_MOTHER, FACING_WEST, 0);
	/* "I'll buy you better sweets than those later on." */
	fdEventShowMessage(ACTOR_TOLBI_MOTHER, 0);
	fdEventEnd();
}

/* During the festival, the family's small boy turns, stamps twice and wails */
/* for sweets. */
static void scTolbiTalkToCryingBoy(void) {
	fdEventStart();
	fdActorFaceDirection(ACTOR_TOLBI_CRYING_BOY, FACING_SOUTH, 0);
	fdActorStartRepeatedMotion(ACTOR_TOLBI_CRYING_BOY, 2);
	fdEventSetMessageId(MsgTolbiWaahBuySweets);
	/* "Waah! Buy me some sweets! I said buy me some sweets!" */
	fdEventShowMessage(ACTOR_TOLBI_CRYING_BOY, 0);
	fdEventEnd();
}

/* During the festival, the strongman sleeping under the archway on the road to
 */
/* the Colosseum says the inns are all full, and the archway keeps the rain */
/* off. */
static void scTolbiTalkToSleepingScalper(void) {
	fdEventStart();
	fdEventSetMessageId(MsgTolbiInnsFullStaying);
	/* "The inns are all full, so I'm staying here. This archway..." */
	fdEventShowMessage(ACTOR_TOLBI_SCALPER, 0);
	fdEventEnd();
}

/* After Colosso, the burly grey-haired man by the market stalls north-east of
 */
/* the fountain asks whether the party was in the finals: he was moved by how */
/* determined they were, or says they look just like that warrior. */
static void scTolbiAskWhetherPartyWasInFinals(void) {
	fdEventStart();
	fdEventSetMessageId(MsgTolbiFinalsWerent);
	/* The question opens beside the family's father, whom the town has moved */
	/* off the map by now, so its window sits low on the screen, away from the
	 */
	/* man asking. */
	/* "You were in the finals, weren't you?" */
	/* Yes: "I was moved by how you were so determined, despite your size." */
	/* No: "What? You weren't? You know, you look just like that warrior." */
	fdEventAskYesNo(ACTOR_TOLBI_FATHER, 0);
	fdEventEnd();
}

/* After Colosso, the stout snack vendor at the stalls north-east of the */
/* fountain gives the party his leftover corn, once, and afterwards says the */
/* festival ran long this year. */
static void scTolbiTalkToSnackVendor(void) {
	fdEventStart();
	if(fdEventGetFlag(FLAG_TOLBI_CORN_GIVEN) == 0) {
		gmFlagSet(FLAG_TOLBI_CORN_GIVEN);
		fdEventSetMessageId(MsgTolbiLeftovers);
		/* "Hey there, these are leftovers, but have some!" */
		fdEventShowMessage(ACTOR_TOLBI_SNACK_VENDOR, 0);
		gmItemShowFound(ITEM_CORN, 3);
		/* "You got some corn." */
		fdEventShowMessage(ACTOR_TOLBI_SNACK_VENDOR, 0);
		fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
		gmPartyGiveItem(ITEM_CORN, 0);
	}
	else {
		fdEventSetMessageId(MsgTolbiFestivalLongerUsual);
		/* "The festival was longer than usual this year, so I made more..." */
		fdEventShowMessage(ACTOR_TOLBI_SNACK_VENDOR, 0);
	}
	fdEventEnd();
}

/* Walking north into one of Tolbi's doors opens it and takes the party inside
 */
/* a house, the inn, the shop, the Sanctum or a Lucky Dice or Lucky Wheels */
/* hall. */
/* */
/* Everyone in town stops while the door opens, and the leader walks a step in
 */
/* (except at the weapon and armor shop) before the screen closes. */
static void scTolbiEnterDoor(void) {
	FIELD_STATE *state = FieldStateWork;
	u32 actorId;
	s32 door;

	fdEventStart();
	for(actorId = ACTOR_FIRST_PLACED; actorId <= ACTOR_LAST_ID; actorId++) {
		ACTOR *actor = fdActorGet(actorId);
		if(actor != NULL) {
			actor->motionFlg = 0;
		}
	}
	door = state->stepTrigger - 1;
	sySndPlay(SE_DOOR_OPEN);
	fdMapGridAnimateCells(TolbiDoors[door].steps, TolbiDoors[door].x,
						  TolbiDoors[door].y);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.5);
	fdActorGet(ACTOR_PARTY_LEADER)->motionFlg = 0;
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
	if(door != TOLBI_TO_WEAPON_AND_ARMOR_SHOP - 1) {
		fdActorCenterAndWalk(ACTOR_PARTY_LEADER, 2, -8);
		fdEventWait(10);
	}
	syImportEventSetExitRequest(state->stepTrigger);
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	fdEventEnd();
}

/* The old man with a cane, who wanders west of the fountain, is drawn in */
/* front of or behind things as the leader is, each frame. */
/* */
/* During the festival he says he is 65 and that Babi was already old when he */
/* was born. */
static void scTolbiCopyLeaderPriority(ACTOR *updated) {
	s32 priority;

	if(updated != NULL) {
		priority = fdActorGet(ACTOR_PARTY_LEADER)->spr->image.pri;
		updated->priFlg = 0;
		updated->spr->image.pri = priority;
		updated->spr->shadow.pri = priority;
	}
}

/* Sets Tolbi up as the party enters: the children start running, the */
/* visitor stays aside once he has let the party go first, the vine and the */
/* ice pillar stand again after a battle, and the crowd and the scalper move */
/* for finals day or leave after Colosso. */
s32 scInit(s32 unused) {
	ACTOR *actor;
	void (*kidUpdate)(ACTOR *);

	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);

	/* The two children run their loop south-east of the fountain. */
	fdActorSetPos(ACTOR_TOLBI_OLDER_BROTHER, 352, 352);
	fdActorSetScript(ACTOR_TOLBI_OLDER_BROTHER, TolbiOlderBrotherRun);
	actor = fdActorGet(ACTOR_TOLBI_OLDER_BROTHER);
	kidUpdate = (void (*)(ACTOR *))scTolbiUpdateKid;
	actor->tmr = KID_CHASES_YOUNGER;
	actor->update = kidUpdate;
	fdActorSetPos(ACTOR_TOLBI_YOUNGER_SISTER, 368, 320);
	fdActorSetScript(ACTOR_TOLBI_YOUNGER_SISTER, TolbiYoungerSisterRun);
	actor = fdActorGet(ACTOR_TOLBI_YOUNGER_SISTER);
	actor->tmr = 0;
	actor->update = kidUpdate;
	fdActorGet(ACTOR_TOLBI_OLD_MAN)->update = scTolbiCopyLeaderPriority;

	/* What the party has already done here. */
	if(gmFlagGet(FLAG_TOLBI_SPRING_OPENED) != 0) {
		fdActorSetPos(ACTOR_TOLBI_SPRING_VISITOR, 316, 328);
	}
	if(gmFlagGet(FLAG_TOLBI_PUDDLE_FROZEN) != 0) {
		scTolbiFreezePuddle();
	}
	if(gmFlagGet(FLAG_TOLBI_SPROUT_GROWN) != 0) {
		scTolbiGrowSprout();
		fdActorSetAnimation(ACTOR_TOLBI_SPROUT, SPROUT_ANIMATION_GROWN);
	}

	/* After Colosso the crowd has gone home and is moved off the map. On finals
	 */
	/* day the scalper waits by the gate. */
	if(gmFlagGet(FLAG_COLOSSO_OVER) != 0) {
		fdActorSetPos(ACTOR_TOLBI_WOULD_BE_FIGHTER, 520, 560);
		fdActorSetPos(ACTOR_TOLBI_XIAN_VISITOR, 520, 560);
		fdActorSetPos(ACTOR_TOLBI_OVERSLEEPER, 520, 560);
		fdActorSetPos(ACTOR_TOLBI_FATHER, 520, 560);
		fdActorSetPos(ACTOR_TOLBI_MOTHER, 520, 560);
		fdActorSetPos(ACTOR_TOLBI_CRYING_BOY, 520, 560);
		fdActorSetPos(ACTOR_TOLBI_SCALPER, 520, 560);
	}
	else {
		if(gmFlagGet(FLAG_BABI_SAVED) != 0) {
			fdActorSetPos(ACTOR_TOLBI_SCALPER, 280, 80);
			fdActorFaceDirection(ACTOR_TOLBI_SCALPER, FACING_SOUTHEAST, 0);
			fdActorSetAnimation(ACTOR_TOLBI_SCALPER, ANIM_STAND);
		}
	}
	return 0;
}

/* Talking to the visitor at the gate in the fountain's railing: he asks */
/* whether the party wants to test its luck at Tolbi Spring. Yes, and he steps
 */
/* aside for good to let the party go first; no, and he keeps trying. */
static void scTolbiTalkToSpringVisitor(void) {
	fdEventStart();
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 304, 312);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_TOLBI_SPRING_VISITOR, FACING_SOUTH, 0);
	fdEventWait(20);
	fdEventSetMessageId(MsgTolbiWantTestLuck);
	/* "Hmm? Do you want to test your luck at Tolbi Spring?" */
	fdEventOpenMessage(ACTOR_TOLBI_SPRING_VISITOR, 0);
	if(fdEventAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
		/* Past the No answer's line. */
		FieldStateWork->msg++;
		/* Yes: "Really? Well then, I'll give you the chance to go first." */
		fdEventShowMessage(ACTOR_TOLBI_SPRING_VISITOR, 0);
		fdActorSetSpeed(ACTOR_TOLBI_SPRING_VISITOR, 1);
		fdActorWalkToAndWait(ACTOR_TOLBI_SPRING_VISITOR, 320, 304);
		fdActorWalkToAndWait(ACTOR_TOLBI_SPRING_VISITOR, 316, 328);
		fdActorFaceDirection(ACTOR_TOLBI_SPRING_VISITOR, FACING_NORTHWEST, 0);
		gmFlagSet(FLAG_TOLBI_SPRING_OPENED);
	}
	else {
		/* No: "Oh, you don't? Well, I'm just going to keep trying for a while."
		 */
		fdEventShowMessage(ACTOR_TOLBI_SPRING_VISITOR, 0);
	}
	fdEventEnd();
}

/* Walking north through the gate in the fountain's railing (cells (18, 18) */
/* and (19, 18)), once the visitor there has stepped aside, takes the party up
 */
/* to Tolbi Spring. */
static void scTolbiEnterSpring(void) {
	syImportEventSetExitRequest(TOLBI_TO_TOLBI_SPRING);
	fdEventEnd();
}

/* Walking north up the stairs to Babi's Palace in the north-west, up the */
/* road to the Colosseum in the north, or into the Lucky Dice hall leaves */
/* Tolbi that way. */
/* */
/* The exit taken is the one numbered as the cell's behavior. */
static void scTolbiRequestTouchedExit(void) {
	syImportEventSetExitRequest(FieldStateWork->stepTrigger);
}

/* During the festival, the first time the party heads up the path toward */
/* Babi's Palace (cells (5, 15) and (5, 16)), two of Babi's soldiers come down
 */
/* the stairs, show a "Missing Person" picture of Babi and ask whether the */
/* party has seen him. */
/* */
/* Yes, and they cry that Babi must be nearby and run east; no, and they go */
/* back north to look elsewhere. */
static void scTolbiMeetSearchingSoldiers(void) {
	s32 firstMsg;

	fdEventStart();
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdActorSetSpeed(ACTOR_TOLBI_SOLDIER_A, 1);
	fdActorSetSpeed(ACTOR_TOLBI_SOLDIER_B, 1);
	firstMsg = MsgTolbiSeenAnyoneWho;
	fdEventSetMessageId(firstMsg);

	/* The soldiers come up to the party; the missing person waits off to the */
	/* side. */
	fdActorSetPos(ACTOR_TOLBI_SOLDIER_A, 72, 208);
	fdActorSetPos(ACTOR_TOLBI_SOLDIER_B, 56, 208);
	fdActorSetSprColById(ACTOR_TOLBI_MISSING_PERSON, SPRITE_COLOR_HIDDEN);
	fdActorSetSprFlags(fdActorGet(ACTOR_TOLBI_MISSING_PERSON), 0);
	fdActorSetPos(ACTOR_TOLBI_MISSING_PERSON, 95, 40);
	fdActorWalkTo(ACTOR_TOLBI_SOLDIER_A, 72, 248);
	fdActorWalkTo(ACTOR_TOLBI_SOLDIER_B, 56, 248);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 64, 264);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorWaitForMoveById(ACTOR_TOLBI_SOLDIER_A);
	fdActorSetAnimation(ACTOR_TOLBI_SOLDIER_A, ANIM_STAND);
	fdActorSetAnimation(ACTOR_TOLBI_SOLDIER_B, ANIM_STAND);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	fdActorFaceActor(ACTOR_TOLBI_SOLDIER_A, ACTOR_PARTY_LEADER, 0);
	fdActorFaceActor(ACTOR_TOLBI_SOLDIER_B, ACTOR_PARTY_LEADER, 0);
	fdEventWait(20);
	fdActorSetAttachedEffect(ACTOR_TOLBI_SOLDIER_A,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(ACTOR_TOLBI_SOLDIER_B,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorStartRepeatedMotion(ACTOR_TOLBI_SOLDIER_A, 2);
	fdActorRunRepeatedMotion(ACTOR_TOLBI_SOLDIER_B, 2);
	fdEventWait(20);

	/* "Have you seen anyone who looks like this?", under a "Missing Person" */
	/* window showing Babi's portrait. The missing person, placed with Babi's */
	/* sprite, becomes the speaker so that answering closes that portrait
	 * window. */
	/* "Hey, have you seen anyone who looks like this?" */
	fdEventOpenMessage(ACTOR_TOLBI_SOLDIER_A, 0);
	fdEventWait(25);
	mnPortraitOpenWindow(SPRITE_BABI, 0, 12, 7);
	mnMessageOpenWindow((firstMsg + 3), 11, 12, 2);
	FieldStateWork->speaker = ACTOR_TOLBI_MISSING_PERSON;
	if(fdEventAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
		/* Yes: "Babi must be nearby!!!" They nod and run east. */
		fdEventWait(20);
		fdActorRunRepeatedMotion(ACTOR_TOLBI_SOLDIER_B, 2);
		fdEventWait(30);
		fdActorFaceDirection(ACTOR_TOLBI_SOLDIER_B, FACING_EAST, 0);
		fdEventWait(30);
		fdEventWait(10);
		fdActorSetAnimationAndWait(ACTOR_TOLBI_SOLDIER_A, ANIM_NOD);
		fdEventWait(20);
		fdActorFaceDirection(ACTOR_TOLBI_SOLDIER_A, FACING_EAST, 0);
		fdEventWait(30);
		/* "Babi must be nearby!!!" */
		fdEventShowMessage(ACTOR_TOLBI_SOLDIER_A, 0);
		fdEventWait(20);
		fdActorFaceDirection(ACTOR_TOLBI_SOLDIER_A, FACING_SOUTH, 0);
		fdActorFaceDirection(ACTOR_TOLBI_SOLDIER_B, FACING_SOUTH, 0);
		fdEventWait(30);
		fdActorSetAnimation(ACTOR_TOLBI_SOLDIER_A, ANIM_NOD);
		fdActorSetAnimationAndWait(ACTOR_TOLBI_SOLDIER_B, ANIM_NOD);
		fdEventWait(20);
		fdActorSetSpeed(ACTOR_TOLBI_SOLDIER_A, 1.8);
		fdActorSetSpeed(ACTOR_TOLBI_SOLDIER_B, 1.8);
		fdActorWalkTo(ACTOR_TOLBI_SOLDIER_A, 232, 248);
		fdEventWait(2);
		fdActorWalkTo(ACTOR_TOLBI_SOLDIER_B, 232, 248);
		fdActorWaitForMoveById(ACTOR_TOLBI_SOLDIER_A);
		fdActorWalkTo(ACTOR_TOLBI_SOLDIER_A, 248, 248);
		fdActorWalkToAndWait(ACTOR_TOLBI_SOLDIER_B, 248, 248);
	}
	else {
		/* No: "He doesn't seem to be here. Let's look over there." They shake
		 */
		/* their heads and go back north. */
		fdEventWait(20);
		fdActorRunRepeatedMotion(ACTOR_TOLBI_SOLDIER_B, 2);
		fdEventWait(30);
		fdActorFaceDirection(ACTOR_TOLBI_SOLDIER_B, FACING_EAST, 0);
		fdEventWait(30);
		fdEventWait(10);
		fdActorSetAnimationAndWait(ACTOR_TOLBI_SOLDIER_A, ANIM_SHAKE_HEAD);
		fdEventWait(20);
		fdActorFaceDirection(ACTOR_TOLBI_SOLDIER_A, FACING_EAST, 0);
		fdEventWait(30);
		FieldStateWork->msg += 1;
		/* "Where's Babi!? He doesn't seem to be here. Let's look over..." */
		fdEventShowMessage(ACTOR_TOLBI_SOLDIER_A, 0);
		fdEventWait(20);
		fdActorFaceDirection(ACTOR_TOLBI_SOLDIER_A, FACING_SOUTH, 0);
		fdActorFaceDirection(ACTOR_TOLBI_SOLDIER_B, FACING_SOUTH, 0);
		fdEventWait(30);
		fdActorSetAnimation(ACTOR_TOLBI_SOLDIER_A, ANIM_NOD);
		fdActorSetAnimationAndWait(ACTOR_TOLBI_SOLDIER_B, ANIM_NOD);
		fdEventWait(20);
		fdActorSetSpeed(ACTOR_TOLBI_SOLDIER_A, 1.6);
		fdActorSetSpeed(ACTOR_TOLBI_SOLDIER_B, 1.6);
		fdActorWalkTo(ACTOR_TOLBI_SOLDIER_A, 72, 184);
		fdActorWalkToAndWait(ACTOR_TOLBI_SOLDIER_B, 56, 184);
	}
	fdActorSetPos(ACTOR_TOLBI_SOLDIER_A, 0, 0);
	fdActorSetPos(ACTOR_TOLBI_SOLDIER_B, 0, 0);
	fdActorSetPos(ACTOR_TOLBI_MISSING_PERSON, 0, 0);
	gmFlagSet(FLAG_TOLBI_SOLDIERS_ASKED_ABOUT_BABI);
	fdEventEnd();
}

/* Frost on the puddle on Tolbi's east side, just north of the sprout (cell */
/* (34, 16)), freezes it into an ice pillar to hop across. */
/* */
/* The puddle's cell takes the attributes of the cell two to its east. */
static void scTolbiFreezePuddle(void) {
	ACTOR *puddle = fdActorGet(ACTOR_TOLBI_PUDDLE);
	if(puddle != NULL) {
		fdActorSetSprFlags(puddle, 0);
		puddle->priFlg = ACTOR_PRIORITY_UNDERFOOT;
		puddle->motionFlg = 0;
	}
	fdActorSetAnimation(ACTOR_TOLBI_PUDDLE, PUDDLE_ANIMATION_PILLAR);
	fdMapGridCopyCellAttributes(36, 16, 1, 1, 34, 16);
	gmFlagSet(FLAG_TOLBI_PUDDLE_FROZEN);
}

/* The scalper asleep under the archway during the festival: propped up on one
 */
/* hand, mirrored. */
s32 TolbiSleepingScalper[] = {
	SC_SET(SW_SCALE_X, -FIXED_ONE),
	SC_ANIMATION(STRONGMAN_ANIMATION_PROPPED_UP),
	SC_END,
};

/* The older brother runs a loop of eight points, 32 pixels across, south-east
 */
/* of the fountain (around cell (22, 21)). */
s32 TolbiOlderBrotherRun[] = {
	SC_ANIMATION(ANIM_WALK),	   SC_SPEED(0.8),
	SC_ACCELERATION(0.2),		   SC_LABEL(1),
	SC_MOVE(352, 0, 352),		   SC_SYNC,
	SC_MOVE(368, 0, 352),		   SC_SYNC,
	SC_MOVE(376, 0, 344),		   SC_SYNC,
	SC_MOVE(376, 0, 328),		   SC_SYNC,
	SC_MOVE(368, 0, 320),		   SC_SYNC,
	SC_MOVE(352, 0, 320),		   SC_SYNC,
	SC_MOVE(344, 0, 328),		   SC_SYNC,
	SC_MOVE(344, 0, 344),		   SC_SYNC,
	SC_LOOP(SC_FOREVER, SC_TO(1)),
};

/* The younger sister runs the same loop half a lap ahead. */
s32 TolbiYoungerSisterRun[] = {
	SC_ANIMATION(ANIM_WALK),	   SC_SPEED(0.8),
	SC_ACCELERATION(0.2),		   SC_LABEL(1),
	SC_MOVE(368, 0, 320),		   SC_SYNC,
	SC_MOVE(352, 0, 320),		   SC_SYNC,
	SC_MOVE(344, 0, 328),		   SC_SYNC,
	SC_MOVE(344, 0, 344),		   SC_SYNC,
	SC_MOVE(352, 0, 352),		   SC_SYNC,
	SC_MOVE(368, 0, 352),		   SC_SYNC,
	SC_MOVE(376, 0, 344),		   SC_SYNC,
	SC_MOVE(376, 0, 328),		   SC_SYNC,
	SC_LOOP(SC_FOREVER, SC_TO(1)),
};

SCENE_ENTRANCE TolbiEntrances[] = {
	{ENTRANCE_DEFAULT, CONDITION_ALWAYS, 296, 0, 232, FACING_SOUTH, 0, -1, -1,
	 -1, -1, 0},
	{TOLBI_FROM_WORLD_MAP, CONDITION_ALWAYS, 304, 0, 448, FACING_NORTH, 0, -1,
	 -1, -1, -1, 0},
	{TOLBI_FROM_HOUSE_NORTHWEST, CONDITION_ALWAYS, 168, 0, 200, FACING_SOUTH, 0,
	 -1, -1, -1, -1, 0},
	{TOLBI_FROM_HOUSE_NORTH, CONDITION_ALWAYS, 408, 0, 160, FACING_SOUTH, 0, -1,
	 -1, -1, -1, 0},
	{TOLBI_FROM_INN, CONDITION_ALWAYS, 536, 0, 232, FACING_SOUTH, 0, -1, -1, -1,
	 -1, 0},
	{TOLBI_FROM_LUCKY_WHEELS, CONDITION_ALWAYS, 536, 0, 200, FACING_SOUTH, 0,
	 -1, -1, -1, -1, 0},
	{TOLBI_FROM_HOUSE_EAST, CONDITION_ALWAYS, 456, 0, 376, FACING_SOUTH, 0, -1,
	 -1, -1, -1, 0},
	{TOLBI_FROM_HOUSE_SOUTHEAST, CONDITION_ALWAYS, 368, 0, 416, FACING_SOUTH, 0,
	 -1, -1, -1, -1, 0},
	{TOLBI_FROM_WEAPON_AND_ARMOR_SHOP, CONDITION_ALWAYS, 200, 0, 456,
	 FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{TOLBI_FROM_HOUSE_SOUTHWEST, CONDITION_ALWAYS, 200, 0, 424, FACING_SOUTH, 0,
	 -1, -1, -1, -1, 0},
	{TOLBI_FROM_SANCTUM, CONDITION_ALWAYS, 88, 0, 392, FACING_SOUTH, 0, -1, -1,
	 -1, -1, 0},
	{TOLBI_FROM_BABI_PALACE_ENTRANCE, CONDITION_ALWAYS, 64, 0, 112,
	 FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{TOLBI_FROM_COLOSSEUM_ENTRANCE, CONDITION_ALWAYS, 304, 0, 64, FACING_SOUTH,
	 0, -1, -1, -1, -1, 0},
	{TOLBI_FROM_LUCKY_DICE, CONDITION_ALWAYS, 424, 0, 224, FACING_SOUTH, 0, -1,
	 -1, -1, -1, 0},
	{TOLBI_FROM_TOLBI_SPRING, CONDITION_ALWAYS, 304, 0, 304, FACING_SOUTH, 0,
	 -1, -1, -1, -1, 0},
	{TOLBI_DEBUG_BY_SPROUT, CONDITION_ALWAYS, 568, 0, 296, FACING_NORTH, 0, -1,
	 -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

u32 TolbiExits[] = {
	SCENE_EXITS(Tolbi),
	SCENE_EXIT(TOLBI_TO_HOUSE_NORTHWEST, StuccoRoomsB,
			   STUCCO_ROOMS_B_HOUSE_NORTHWEST),
	SCENE_EXIT(TOLBI_TO_HOUSE_NORTH, StuccoRoomsB, STUCCO_ROOMS_B_HOUSE_NORTH),
	SCENE_EXIT(TOLBI_TO_INN, StuccoRoomsB, STUCCO_ROOMS_B_INN),
	SCENE_EXIT(TOLBI_TO_LUCKY_WHEELS, StuccoRoomsC,
			   STUCCO_ROOMS_C_LUCKY_WHEELS_FROM_TOLBI),
	SCENE_EXIT(TOLBI_TO_HOUSE_EAST, StuccoRoomsB, STUCCO_ROOMS_B_HOUSE_EAST),
	SCENE_EXIT(TOLBI_TO_HOUSE_SOUTHEAST, StuccoRoomsB,
			   STUCCO_ROOMS_B_HOUSE_SOUTHEAST),
	SCENE_EXIT(TOLBI_TO_WEAPON_AND_ARMOR_SHOP, StuccoRoomsB,
			   STUCCO_ROOMS_B_WEAPON_AND_ARMOR_SHOP),
	SCENE_EXIT(TOLBI_TO_HOUSE_SOUTHWEST, StuccoRoomsB,
			   STUCCO_ROOMS_B_HOUSE_SOUTHWEST),
	SCENE_EXIT(TOLBI_TO_SANCTUM, StuccoRoomsB, STUCCO_ROOMS_B_SANCTUM),
	SCENE_EXIT(TOLBI_TO_BABI_PALACE_ENTRANCE, BabiPalaceEntrance,
			   BABI_PALACE_ENTRANCE_FROM_TOLBI),
	SCENE_EXIT(TOLBI_TO_COLOSSEUM_ENTRANCE, ColosseumEntrance,
			   COLOSSEUM_ENTRANCE_FROM_TOLBI),
	SCENE_EXIT(TOLBI_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_TOLBI),
	SCENE_EXIT(TOLBI_TO_LUCKY_DICE, StuccoRoomsC,
			   STUCCO_ROOMS_C_LUCKY_DICE_FROM_TOLBI),
	SCENE_EXIT(TOLBI_TO_TOLBI_SPRING, TolbiSpring, TOLBI_SPRING_FROM_TOLBI),
	SCENE_EXITS_END,
};

/* clang-format off */
ACTOR_PLACE TolbiActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        552, 0, 288, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        552, 0, 264, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        264, 0, 264, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        208, 0, 312, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        360, 0, 224, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        136, 0, 216, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        152, 0, 304, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        24, 0, 456, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, (s32)TolbiOlderBrotherRun,
        344, 0, 328, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, (s32)TolbiYoungerSisterRun,
        376, 0, 328, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        248, 0, 200, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_STOUT_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        360, 0, 200, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUTH_WITH_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        400, 0, 432, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUTH_WITH_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        344, 0, 120, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_PERSON_WITH_SWORD, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        184, 0, 472, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PERSON_WITH_SWORD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        -64, 0, -64, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        256, 0, 168, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        280, 0, 184, FACING_NORTH_NORTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        272, 0, 168, FACING_SOUTH_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, (s32)TolbiSleepingScalper,
        312, 0, 120, FACING_NORTH_NORTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        302, 0, 296, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        -9, 0, 488, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        -9, 0, 488, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    /* Ember, until its Djinni flag is set: twenty flags to an element. */
    ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, FLAG_DJINNI(DJINNI_EMBER), MOVEMENT_TYPE_STAND,
        584, 0, 136, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BABI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        -9, 0, 488, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    { SCENE_TABLE_END },
};

/* During the festival, before the finals. */
TRIGGER TolbiTriggersFestival[] = {
    { PSYNERGY_ON_ACTOR(ABILITY_GROWTH), ACTOR_TOLBI_SPROUT, FLAG_TOLBI_SPROUT_GROWN, EVENT(scTolbiGrowSprout) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TOLBI_TO_HOUSE_NORTHWEST, CONDITION_ALWAYS, EVENT(scTolbiEnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TOLBI_TO_HOUSE_NORTH, CONDITION_ALWAYS, EVENT(scTolbiEnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TOLBI_TO_INN, CONDITION_ALWAYS, EVENT(scTolbiEnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TOLBI_TO_LUCKY_WHEELS, CONDITION_ALWAYS, EVENT(scTolbiEnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TOLBI_TO_HOUSE_EAST, CONDITION_ALWAYS, EVENT(scTolbiEnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TOLBI_TO_HOUSE_SOUTHEAST, CONDITION_ALWAYS, EVENT(scTolbiEnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TOLBI_TO_WEAPON_AND_ARMOR_SHOP, CONDITION_ALWAYS, EVENT(scTolbiEnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TOLBI_TO_HOUSE_SOUTHWEST, CONDITION_ALWAYS, EVENT(scTolbiEnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TOLBI_TO_SANCTUM, CONDITION_ALWAYS, EVENT(scTolbiEnterDoor) },
    { TRIGGER_STEP | TRIGGER_FLAG_FACING | FACING_NORTH, TOLBI_TO_BABI_PALACE_ENTRANCE, CONDITION_ALWAYS, EVENT(scTolbiRequestTouchedExit) },
    { TRIGGER_STEP | TRIGGER_FLAG_FACING | FACING_NORTH, TOLBI_TO_COLOSSEUM_ENTRANCE, CONDITION_ALWAYS, EVENT(scTolbiRequestTouchedExit) },
    { TRIGGER_EXIT, CELL_BEHAVIOR_TOLBI_EDGE, CONDITION_ALWAYS, TOLBI_TO_WORLD_MAP },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), TOLBI_TO_WORLD_MAP, CONDITION_ALWAYS, EVENT(scTolbiRequestTouchedExit) },
    { TRIGGER_STEP | TRIGGER_FLAG_FACING | FACING_NORTH, TOLBI_TO_LUCKY_DICE, CONDITION_ALWAYS, EVENT(scTolbiRequestTouchedExit) },
    { TRIGGER_STEP, CELL_BEHAVIOR_TOLBI_SOLDIERS, FLAG_TOLBI_SOLDIERS_ASKED_ABOUT_BABI, EVENT(scTolbiMeetSearchingSoldiers) },
    { TRIGGER_STEP | TRIGGER_FLAG_FACING | FACING_NORTH, CELL_BEHAVIOR_TOLBI_GATE_TO_SPRING, IF_FLAG_SET(FLAG_TOLBI_SPRING_OPENED), EVENT(scTolbiEnterSpring) },
    { TRIGGER_TALK, ACTOR_TOLBI_EXCITED_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiTolbisAlwaysReallyBustlingDuringColosso) },
    { TRIGGER_TALK, ACTOR_TOLBI_HURRYING_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiColossoIsHeldInTheColosseum) },
    { TRIGGER_TALK, ACTOR_TOLBI_FINALS_FAN, CONDITION_ALWAYS, MSGNO(MsgTolbiTryoutsStartedAtTheColosseumToday) },
    { TRIGGER_TALK, ACTOR_TOLBI_WORRIED_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiColossoShouldHaveStartedTodayWhat) },
    { TRIGGER_TALK, ACTOR_TOLBI_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiBabiWasAlreadyPrettyOldBack) },
    { TRIGGER_TALK, ACTOR_TOLBI_SUSPICIOUS_OLD_WOMAN, CONDITION_ALWAYS, EVENT(scTolbiAskAboutLivingPastHundred) },
    { TRIGGER_TALK, ACTOR_TOLBI_OLDER_BROTHER, CONDITION_ALWAYS, EVENT(scTolbiTalkToOlderBrother) },
    { TRIGGER_TALK, ACTOR_TOLBI_YOUNGER_SISTER, CONDITION_ALWAYS, EVENT(scTolbiTalkToYoungerSister) },
    { TRIGGER_TALK, ACTOR_TOLBI_SOUVENIR_SELLER, CONDITION_ALWAYS, MSGNO(MsgTolbiGetYourColossoSouvenirsTheyMake) },
    { TRIGGER_TALK, ACTOR_TOLBI_SNACK_VENDOR, CONDITION_ALWAYS, MSGNO(MsgTolbiGetYourSnacksAndDrinksBefore) },
    { TRIGGER_TALK, ACTOR_TOLBI_WOULD_BE_FIGHTER, CONDITION_ALWAYS, MSGNO(MsgTolbiIveDecidedNotToFightAt) },
    { TRIGGER_TALK, ACTOR_TOLBI_XIAN_VISITOR, CONDITION_ALWAYS, MSGNO(MsgTolbiWowTolbisFestivalIsHumongousJust) },
    { TRIGGER_TALK, ACTOR_TOLBI_OVERSLEEPER, CONDITION_ALWAYS, MSGNO(MsgTolbiRatsWeOversleptAndNowThe) },
    { TRIGGER_TALK, ACTOR_TOLBI_FATHER, CONDITION_ALWAYS, EVENT(scTolbiTalkToFather) },
    { TRIGGER_TALK, ACTOR_TOLBI_MOTHER, CONDITION_ALWAYS, EVENT(scTolbiTalkToMother) },
    { TRIGGER_TALK, ACTOR_TOLBI_CRYING_BOY, CONDITION_ALWAYS, EVENT(scTolbiTalkToCryingBoy) },
    { TRIGGER_TALK, ACTOR_TOLBI_SCALPER, CONDITION_ALWAYS, EVENT(scTolbiTalkToSleepingScalper) },
    { TRIGGER_TALK, ACTOR_TOLBI_SPRING_VISITOR, FLAG_TOLBI_SPRING_OPENED, EVENT(scTolbiTalkToSpringVisitor) },
    { TRIGGER_TALK, ACTOR_TOLBI_SPRING_VISITOR, CONDITION_ALWAYS, MSGNO(MsgTolbiAhIGuessIJustDont) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_EXCITED_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiTouristsArentTheOnlyOnesWho) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_HURRYING_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiTheTrialsAreStartingAtThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_FINALS_FAN, CONDITION_ALWAYS, MSGNO(MsgTolbiTheTrialsAreOkayButThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_WORRIED_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiItsNotLikeThereHaveBeen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiEveryYearIGetOlderAnd) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_SUSPICIOUS_OLD_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiWhatIfBabiIsActuallyA) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_OLDER_BROTHER, CONDITION_ALWAYS, EVENT(scTolbiTalkToOlderBrother) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_YOUNGER_SISTER, CONDITION_ALWAYS, EVENT(scTolbiTalkToYoungerSister) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_SOUVENIR_SELLER, CONDITION_ALWAYS, MSGNO(MsgTolbiWeHaveToSellAllOf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_SNACK_VENDOR, CONDITION_ALWAYS, MSGNO(MsgTolbiTheyreSellingTheseAtTheColosseum) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_WOULD_BE_FIGHTER, CONDITION_ALWAYS, MSGNO(MsgTolbiISawThePeopleWhoAre) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_XIAN_VISITOR, CONDITION_ALWAYS, MSGNO(MsgTolbiTolbiIsNotLikeXianIt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_OVERSLEEPER, CONDITION_ALWAYS, MSGNO(MsgTolbiICantSayAnythingCauseIt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_FATHER, CONDITION_ALWAYS, MSGNO(MsgTolbiTheStuffTheyreSellingAtThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_MOTHER, CONDITION_ALWAYS, MSGNO(MsgTolbiWhyDoKidsWantSuchAwful) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_CRYING_BOY, CONDITION_ALWAYS, MSGNO(MsgTolbiThoseSweetsSmellGreatImNot) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_SCALPER, CONDITION_ALWAYS, MSGNO(MsgTolbiIveBeenGuardingOurSpotBut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_SPRING_VISITOR, CONDITION_ALWAYS, MSGNO(MsgTolbiIKeepLosingCoinsButI) },
    { TRIGGER_TALK, ACTOR_TOLBI_EMBER, CONDITION_ALWAYS, EVENT(scTolbiJoinEmber) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_TOLBI_SPRING_SIGN, CONDITION_ALWAYS, EVENT(scTolbiReadSpringSign) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_TOLBI_LUCKY_MEDAL_SIGN, CONDITION_ALWAYS, EVENT(scTolbiReadLuckyMedalSign) },
    { EXAMINE_TRIGGER(EXAMINE_BARREL), CELL_BEHAVIOR_TOLBI_BARREL, FLAG_TOLBI_BARREL_COINS, FIND_COINS(9) },
    /* From TBS_PATCH_1 on, the jar is found only facing north. */
    { TOLBI_JAR_SEARCH_KIND, CELL_BEHAVIOR_TOLBI_JAR, FLAG_TOLBI_JAR_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), CELL_BEHAVIOR_TOLBI_HIDDEN_CHEST, FLAG_TOLBI_CHEST_POWER_BREAD, FIND_ITEM(ITEM_POWER_BREAD) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_TOLBI_PUDDLE, FLAG_TOLBI_PUDDLE_FROZEN, EVENT(scTolbiFreezePuddle) },
    { PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(scTolbiRevealHiddenChest) },
    { PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(scTolbiHideRevealedChest) },
    { TRIGGER_TABLE_END },
};

TRIGGER TolbiTriggersFinalsDay[] = {
    { PSYNERGY_ON_ACTOR(ABILITY_GROWTH), ACTOR_TOLBI_SPROUT, FLAG_TOLBI_SPROUT_GROWN, EVENT(scTolbiGrowSprout) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TOLBI_TO_HOUSE_NORTHWEST, CONDITION_ALWAYS, EVENT(scTolbiEnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TOLBI_TO_HOUSE_NORTH, CONDITION_ALWAYS, EVENT(scTolbiEnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TOLBI_TO_INN, CONDITION_ALWAYS, EVENT(scTolbiEnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TOLBI_TO_LUCKY_WHEELS, CONDITION_ALWAYS, EVENT(scTolbiEnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TOLBI_TO_HOUSE_EAST, CONDITION_ALWAYS, EVENT(scTolbiEnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TOLBI_TO_HOUSE_SOUTHEAST, CONDITION_ALWAYS, EVENT(scTolbiEnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TOLBI_TO_WEAPON_AND_ARMOR_SHOP, CONDITION_ALWAYS, EVENT(scTolbiEnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TOLBI_TO_HOUSE_SOUTHWEST, CONDITION_ALWAYS, EVENT(scTolbiEnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TOLBI_TO_SANCTUM, CONDITION_ALWAYS, EVENT(scTolbiEnterDoor) },
    { TRIGGER_STEP | TRIGGER_FLAG_FACING | FACING_NORTH, TOLBI_TO_BABI_PALACE_ENTRANCE, CONDITION_ALWAYS, EVENT(scTolbiRequestTouchedExit) },
    { TRIGGER_STEP | TRIGGER_FLAG_FACING | FACING_NORTH, TOLBI_TO_COLOSSEUM_ENTRANCE, CONDITION_ALWAYS, EVENT(scTolbiRequestTouchedExit) },
    { TRIGGER_EXIT, CELL_BEHAVIOR_TOLBI_EDGE, CONDITION_ALWAYS, TOLBI_TO_WORLD_MAP },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), TOLBI_TO_WORLD_MAP, CONDITION_ALWAYS, EVENT(scTolbiRequestTouchedExit) },
    { TRIGGER_STEP | TRIGGER_FLAG_FACING | FACING_NORTH, TOLBI_TO_LUCKY_DICE, CONDITION_ALWAYS, EVENT(scTolbiRequestTouchedExit) },
    { TRIGGER_STEP | TRIGGER_FLAG_FACING | FACING_NORTH, CELL_BEHAVIOR_TOLBI_GATE_TO_SPRING, IF_FLAG_SET(FLAG_TOLBI_SPRING_OPENED), EVENT(scTolbiEnterSpring) },
    { TRIGGER_TALK, ACTOR_TOLBI_EXCITED_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiColossoWillReachItsApexAny) },
    { TRIGGER_TALK, ACTOR_TOLBI_HURRYING_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiAreYouPlanningToWatchThe) },
    { TRIGGER_TALK, ACTOR_TOLBI_FINALS_FAN, CONDITION_ALWAYS, MSGNO(MsgTolbiMyFamilySavedMeASeat) },
    { TRIGGER_TALK, ACTOR_TOLBI_WORRIED_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiIThoughtSomethingHappenedToBabi) },
    { TRIGGER_TALK, ACTOR_TOLBI_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiTheColosseumWasSoFullThat) },
    { TRIGGER_TALK, ACTOR_TOLBI_SUSPICIOUS_OLD_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiNobodyElseHasNoticedButThis) },
    { TRIGGER_TALK, ACTOR_TOLBI_OLDER_BROTHER, CONDITION_ALWAYS, EVENT(scTolbiTalkToOlderBrother) },
    { TRIGGER_TALK, ACTOR_TOLBI_YOUNGER_SISTER, CONDITION_ALWAYS, EVENT(scTolbiTalkToYoungerSister) },
    { TRIGGER_TALK, ACTOR_TOLBI_SOUVENIR_SELLER, CONDITION_ALWAYS, MSGNO(MsgTolbiHowAboutSomeColossoSouvenirsThis) },
    { TRIGGER_TALK, ACTOR_TOLBI_SNACK_VENDOR, CONDITION_ALWAYS, MSGNO(MsgTolbiTheFinalsAreSoLongIts) },
    { TRIGGER_TALK, ACTOR_TOLBI_WOULD_BE_FIGHTER, CONDITION_ALWAYS, MSGNO(MsgTolbiTheTrialsWereSoRivetingThe) },
    { TRIGGER_TALK, ACTOR_TOLBI_XIAN_VISITOR, CONDITION_ALWAYS, MSGNO(MsgTolbiIHaveSeatsToTheFinals) },
    { TRIGGER_TALK, ACTOR_TOLBI_OVERSLEEPER, CONDITION_ALWAYS, MSGNO(MsgTolbiIOversleptAgainSoICant) },
    /* The family repeats its festival lines on finals day. XXX (no evidence): */
    /* whether the four messages after MsgTolbiSpringOhThereAreAlwaysScalpersAt */
    /* (`The finals are about to start.` and the rest) were meant for it; no row */
    /* or code shows them. */
    { TRIGGER_TALK, ACTOR_TOLBI_FATHER, CONDITION_ALWAYS, EVENT(scTolbiTalkToFather) },
    { TRIGGER_TALK, ACTOR_TOLBI_MOTHER, CONDITION_ALWAYS, EVENT(scTolbiTalkToMother) },
    { TRIGGER_TALK, ACTOR_TOLBI_CRYING_BOY, CONDITION_ALWAYS, EVENT(scTolbiTalkToCryingBoy) },
    { TRIGGER_TALK, ACTOR_TOLBI_SCALPER, CONDITION_ALWAYS, MSGNO(MsgTolbiTheFinalsStartTodayIWonder) },
    { TRIGGER_TALK, ACTOR_TOLBI_SPRING_VISITOR, FLAG_TOLBI_SPRING_OPENED, EVENT(scTolbiTalkToSpringVisitor) },
    { TRIGGER_TALK, ACTOR_TOLBI_SPRING_VISITOR, CONDITION_ALWAYS, MSGNO(MsgTolbiAhIGuessIJustDont) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_EXCITED_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiIveGotToGetToThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_HURRYING_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiABattleToTheDeathWith) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_FINALS_FAN, CONDITION_ALWAYS, MSGNO(MsgTolbiTheFinalsStartOnBabisSignal) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_WORRIED_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiSoWhyWereTheSoldiersAll) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiICouldStillBuySeatsFrom) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_SUSPICIOUS_OLD_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiNoMatterHowIAddIt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_OLDER_BROTHER, CONDITION_ALWAYS, EVENT(scTolbiTalkToOlderBrother) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_YOUNGER_SISTER, CONDITION_ALWAYS, EVENT(scTolbiTalkToYoungerSister) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_SOUVENIR_SELLER, CONDITION_ALWAYS, MSGNO(MsgTolbiWeCantSellAnythingOnceThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_SNACK_VENDOR, CONDITION_ALWAYS, MSGNO(MsgTolbiThisIsTheFinalPushAnything) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_WOULD_BE_FIGHTER, CONDITION_ALWAYS, MSGNO(MsgTolbiFightingInColossoRequiresALot) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_XIAN_VISITOR, CONDITION_ALWAYS, MSGNO(MsgTolbiTheyreSellingFinalsSeatsRightAt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_OVERSLEEPER, CONDITION_ALWAYS, MSGNO(MsgTolbiItsSupposedToBeFreeHaving) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_FATHER, CONDITION_ALWAYS, MSGNO(MsgTolbiOnceColossoEndsOurLivesCan) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_MOTHER, CONDITION_ALWAYS, MSGNO(MsgTolbiFestivalsAreSoExpensiveMyWallets) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_CRYING_BOY, CONDITION_ALWAYS, MSGNO(MsgTolbiMomYouSaidYouWereAll) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_SCALPER, CONDITION_ALWAYS, MSGNO(MsgTolbiTodayIsABigDayFor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_SPRING_VISITOR, CONDITION_ALWAYS, MSGNO(MsgTolbiIKeepLosingCoinsButI) },
    { TRIGGER_TALK, ACTOR_TOLBI_EMBER, CONDITION_ALWAYS, EVENT(scTolbiJoinEmber) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_TOLBI_SPRING_SIGN, CONDITION_ALWAYS, EVENT(scTolbiReadSpringSign) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_TOLBI_LUCKY_MEDAL_SIGN, CONDITION_ALWAYS, EVENT(scTolbiReadLuckyMedalSign) },
    { EXAMINE_TRIGGER(EXAMINE_BARREL), CELL_BEHAVIOR_TOLBI_BARREL, FLAG_TOLBI_BARREL_COINS, FIND_COINS(9) },
    { EXAMINE_TRIGGER(EXAMINE_JAR), CELL_BEHAVIOR_TOLBI_JAR, FLAG_TOLBI_JAR_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), CELL_BEHAVIOR_TOLBI_HIDDEN_CHEST, FLAG_TOLBI_CHEST_POWER_BREAD, FIND_ITEM(ITEM_POWER_BREAD) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_TOLBI_PUDDLE, FLAG_TOLBI_PUDDLE_FROZEN, EVENT(scTolbiFreezePuddle) },
    { PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(scTolbiRevealHiddenChest) },
    { PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(scTolbiHideRevealedChest) },
    { TRIGGER_TABLE_END },
};

/* After Colosso has ended. */
TRIGGER TolbiTriggersAfterColosso[] = {
    { PSYNERGY_ON_ACTOR(ABILITY_GROWTH), ACTOR_TOLBI_SPROUT, FLAG_TOLBI_SPROUT_GROWN, EVENT(scTolbiGrowSprout) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TOLBI_TO_HOUSE_NORTHWEST, CONDITION_ALWAYS, EVENT(scTolbiEnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TOLBI_TO_HOUSE_NORTH, CONDITION_ALWAYS, EVENT(scTolbiEnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TOLBI_TO_INN, CONDITION_ALWAYS, EVENT(scTolbiEnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TOLBI_TO_LUCKY_WHEELS, CONDITION_ALWAYS, EVENT(scTolbiEnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TOLBI_TO_HOUSE_EAST, CONDITION_ALWAYS, EVENT(scTolbiEnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TOLBI_TO_HOUSE_SOUTHEAST, CONDITION_ALWAYS, EVENT(scTolbiEnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TOLBI_TO_WEAPON_AND_ARMOR_SHOP, CONDITION_ALWAYS, EVENT(scTolbiEnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TOLBI_TO_HOUSE_SOUTHWEST, CONDITION_ALWAYS, EVENT(scTolbiEnterDoor) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), TOLBI_TO_SANCTUM, CONDITION_ALWAYS, EVENT(scTolbiEnterDoor) },
    { TRIGGER_STEP | TRIGGER_FLAG_FACING | FACING_NORTH, TOLBI_TO_BABI_PALACE_ENTRANCE, CONDITION_ALWAYS, EVENT(scTolbiRequestTouchedExit) },
    { TRIGGER_STEP | TRIGGER_FLAG_FACING | FACING_NORTH, TOLBI_TO_COLOSSEUM_ENTRANCE, CONDITION_ALWAYS, EVENT(scTolbiRequestTouchedExit) },
    { TRIGGER_EXIT, CELL_BEHAVIOR_TOLBI_EDGE, CONDITION_ALWAYS, TOLBI_TO_WORLD_MAP },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), TOLBI_TO_WORLD_MAP, CONDITION_ALWAYS, EVENT(scTolbiRequestTouchedExit) },
    { TRIGGER_STEP | TRIGGER_FLAG_FACING | FACING_NORTH, TOLBI_TO_LUCKY_DICE, CONDITION_ALWAYS, EVENT(scTolbiRequestTouchedExit) },
    { TRIGGER_STEP | TRIGGER_FLAG_FACING | FACING_NORTH, CELL_BEHAVIOR_TOLBI_GATE_TO_SPRING, IF_FLAG_SET(FLAG_TOLBI_SPRING_OPENED), EVENT(scTolbiEnterSpring) },
    { TRIGGER_TALK, ACTOR_TOLBI_EXCITED_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiWellThisYearsColossoHasEnded) },
    { TRIGGER_TALK, ACTOR_TOLBI_HURRYING_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiItsBackToOurNormalQuiet) },
    { TRIGGER_TALK, ACTOR_TOLBI_FINALS_FAN, CONDITION_ALWAYS, EVENT(scTolbiAskWhetherPartyWasInFinals) },
    { TRIGGER_TALK, ACTOR_TOLBI_WORRIED_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiWithAllThoseRevelersGoneThe) },
    { TRIGGER_TALK, ACTOR_TOLBI_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiIHadNoLuckInThe) },
    { TRIGGER_TALK, ACTOR_TOLBI_SUSPICIOUS_OLD_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiItsSoQuietTheHustleAnd) },
    { TRIGGER_TALK, ACTOR_TOLBI_OLDER_BROTHER, CONDITION_ALWAYS, EVENT(scTolbiTalkToOlderBrother) },
    { TRIGGER_TALK, ACTOR_TOLBI_YOUNGER_SISTER, CONDITION_ALWAYS, EVENT(scTolbiTalkToYoungerSister) },
    { TRIGGER_TALK, ACTOR_TOLBI_SOUVENIR_SELLER, CONDITION_ALWAYS, MSGNO(MsgTolbiIllBeBackToDoBusiness) },
    { TRIGGER_TALK, ACTOR_TOLBI_SNACK_VENDOR, CONDITION_ALWAYS, EVENT(scTolbiTalkToSnackVendor) },
    { TRIGGER_TALK, ACTOR_TOLBI_SPRING_VISITOR, FLAG_TOLBI_SPRING_OPENED, EVENT(scTolbiTalkToSpringVisitor) },
    { TRIGGER_TALK, ACTOR_TOLBI_SPRING_VISITOR, CONDITION_ALWAYS, MSGNO(MsgTolbiAhIGuessIJustDont) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_EXCITED_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiTheSpectatorsHaveAllLeftSo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_HURRYING_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiThinkHowFunItWouldBe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_FINALS_FAN, CONDITION_ALWAYS, MSGNO(MsgTolbiAllTheWarriorsWhoGainFame) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_WORRIED_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiComeToThinkOfItNo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgTolbiHmmSinceColossoEndedSoDramatically) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_SUSPICIOUS_OLD_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiTheCalmAfterTheStormIs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_OLDER_BROTHER, CONDITION_ALWAYS, EVENT(scTolbiTalkToOlderBrother) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_YOUNGER_SISTER, CONDITION_ALWAYS, EVENT(scTolbiTalkToYoungerSister) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_SOUVENIR_SELLER, CONDITION_ALWAYS, MSGNO(MsgTolbiEveryYearITryToSell) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_SNACK_VENDOR, CONDITION_ALWAYS, MSGNO(MsgTolbiHowCanWeGetHomeIf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOLBI_SPRING_VISITOR, CONDITION_ALWAYS, MSGNO(MsgTolbiIKeepLosingCoinsButI) },
    { TRIGGER_TALK, ACTOR_TOLBI_EMBER, CONDITION_ALWAYS, EVENT(scTolbiJoinEmber) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_TOLBI_SPRING_SIGN, CONDITION_ALWAYS, EVENT(scTolbiReadSpringSign) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_TOLBI_LUCKY_MEDAL_SIGN, CONDITION_ALWAYS, EVENT(scTolbiReadLuckyMedalSign) },
    { EXAMINE_TRIGGER(EXAMINE_BARREL), CELL_BEHAVIOR_TOLBI_BARREL, FLAG_TOLBI_BARREL_COINS, FIND_COINS(9) },
    { EXAMINE_TRIGGER(EXAMINE_JAR), CELL_BEHAVIOR_TOLBI_JAR, FLAG_TOLBI_JAR_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), CELL_BEHAVIOR_TOLBI_HIDDEN_CHEST, FLAG_TOLBI_CHEST_POWER_BREAD, FIND_ITEM(ITEM_POWER_BREAD) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_TOLBI_PUDDLE, FLAG_TOLBI_PUDDLE_FROZEN, EVENT(scTolbiFreezePuddle) },
    { PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(scTolbiRevealHiddenChest) },
    { PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(scTolbiHideRevealedChest) },
    { TRIGGER_TABLE_END },
};
/* clang-format on */

/* How a door opens, as two frames of cells copied over the closed door. */
u16 TolbiDoorOpening[] = {
	MAPGRID_ANIMATION_STEP(39, 32, 1, 2, 6),
	MAPGRID_ANIMATION_STEP(40, 32, 1, 2, 6),
	MAPGRID_ANIMATION_STEPS_END,
};

/* The doors, in the order of their cell behaviors 1 to 9, by map cell. */
MAP_GRID_CELL_ANIMATION TolbiDoors[] = {
	{TolbiDoorOpening, 48, 6},	{TolbiDoorOpening, 63, 4},
	{TolbiDoorOpening, 71, 8},	{TolbiDoorOpening, 71, 4},
	{TolbiDoorOpening, 66, 21}, {TolbiDoorOpening, 61, 24},
	{TolbiDoorOpening, 50, 26}, {TolbiDoorOpening, 50, 22},
	{TolbiDoorOpening, 43, 20},
};
