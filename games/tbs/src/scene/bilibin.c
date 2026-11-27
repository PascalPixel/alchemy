/* @file */
/* Bilibin, the Bilibin Treasury and the Bilibin Barricade, three scenes that */
/* share one overlay; the getters pick each scene's tables by */
/* MainRoutineStateWork.next. */
/* */
/* Bilibin is the town past Goma Cave, whose chief, Lord McCoy, has closed the
 */
/* road to Kolima. Almost everything here turns on FLAG_KOLIMA_CURSE_LIFTED, */
/* set in Kolima Forest when the party heals Tret: until then a man turned */
/* into a tree stands at the entrance (Mind Read hears `Help me... */
/* Somebody...`), and each villager has a line and a thought for either time. */
/* Seven doors open as the party walks into them: the inn, three houses, the */
/* item shop, the weapon and armor shop, and the Sanctum's double door. */
/* */
/* Two ways lead into the Treasury, `McCoy's Hidden Warehouse` by its sign, */
/* where Gust waits: the cave mouth at the town's west edge, behind ivy that */
/* Whirlwind blows away, and a hole under the golden statue in front of the */
/* weapon and armor shop, which Move or a push slides aside. Inside, a second */
/* golden statue slides along a walkway between two pools. */
/* */
/* The Bilibin Barricade is the road east to Kolima, shut by McCoy's wall of */
/* crates; the stack at its left end slides out and opens the way. By the */
/* river stand three trees, one on another's shoulders: the Gang of Three, */
/* children turned into trees by the curse. Once Saturos is beaten at Mercury */
/* Lighthouse the travelers from Imil have pushed them over to cross, and */
/* Jill's tree drifts in the river until Isaac pulls her ashore with Move. */
/* With the curse lifted they are children again, and Jill, if she was saved, */
/* gives Isaac a Hard Nut. */

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/actor_script.h"
#include "field/event_inline.h"
#include "graphics/graphics.h"
#include "text/msg_ids.h"

extern s32 Bilibin_FaceLeaderScript[];
extern SCENE_ENTRANCE BilibinEntrancesOther[];
extern SCENE_ENTRANCE BilibinEntrances[];
extern SCENE_ENTRANCE BilibinBarricadeEntrances[];
extern SCENE_ENTRANCE BilibinTreasuryEntrances[];
extern s16 BilibinTreasuryRegions[];
extern u32 BilibinExits[];
extern ACTOR_PLACE BilibinActorPlacementsOther[];
extern ACTOR_PLACE BilibinActorPlacements[];
extern ACTOR_PLACE BilibinBarricadeActorPlacements[];
extern ACTOR_PLACE BilibinTreasuryActorPlacements[];
extern TRIGGER BilibinTriggersOther[];
extern TRIGGER BilibinTriggers[];
extern TRIGGER BilibinBarricadeTriggers[];
extern TRIGGER BilibinTreasuryTriggers[];
extern u16 *Bilibin_DoorSteps[];
extern s16 Bilibin_DoorCells[][2];
/* The cells round the crates' place that push them, as column and row pairs. */
extern u8 Bilibin_CratesPushCells[];
extern s32 PushedActor_DirectionSteps[];
extern s32 Bilibin_RippleScript[];

static void scBilibinRestoreVillageActors(void);
/* Move on the golden statue in front of Bilibin's weapon and armor shop slides
 */
/* it a cell. Moved off cell (16, 13), it uncovers the hole into the Treasury;
 */
/* moved back on, it covers it again. */
/* */
/* The same work follows every push and every arrival in Bilibin. The cells of
 */
/* the statue's dotted square, (15..17, 12..14), are made ground that pushes */
/* the statue again, the statue's own cell is walled off, and the hole's cell */
/* is given its exit unless the statue stands on it. If the statue lands on */
/* the hole while Isaac is standing on it, he starts with a "!" and hops clear,
 */
/* to the north or the east. */
static void scBilibinUpdateVillageStatueCells(void);
static void scBilibinRestoreRiverActors(void);
/* Move on the stack of two crates at the left end of the Bilibin Barricade */
/* drags it out of the wall of crates, opening a gap through the barricade to */
/* the road to Kolima. */
/* */
/* The same work follows every push and every arrival at the barricade: the */
/* ten cells round the crate's place, listed in Bilibin_CratesPushCells, are */
/* made ground that pushes the crate, and the crate's own cell is walled off. */
static void scBilibinUpdateCratesCells(void);
static void scBilibinSetUpTreasury(void);
static void scBilibinUpdateRiverRipples(void);
static void scBilibinSpawnRipple(ACTOR *actor);
/* Move on the golden statue in the Treasury slides it up or down the walkway */
/* between the two pools, where Gust waits on the ledge to the east. */
/* */
/* The statue rests either in its alcove at the top, cell (14, 6), or at the */
/* walkway's foot, cell (14, 9). The same work follows every push and every */
/* arrival: the cell the statue stands on is walled off, the alcove when empty
 */
/* lets Isaac hop east or west across the pools, and the foot when empty lets */
/* him push the statue back up or hop south off the walkway. */
static void scBilibinUpdateTreasuryStatueCells(void);

/* Actors of Bilibin, numbering its actor placements from */
/* ACTOR_FIRST_PLACED. The ten villagers are named by what they say before */
/* the curse is lifted. */
typedef enum BILIBIN_ACTOR_ID {
	/* The bush of ivy over the cave mouth at the west edge, the way into the */
	/* Treasury, drawn half as tall again. Whirlwind blows it away. */
	ACTOR_IVY = ACTOR_FIRST_PLACED,
	/* The man turned into a tree at the village entrance, drawn as a tree. */
	ACTOR_TREE_MAN,
	/* A young man with brown hair. */
	ACTOR_YOUNG_MAN_WELCOME,
	/* A woman with fair hair. */
	ACTOR_VILLAGER_GROUP_TO_IMIL,
	/* A burly grey-haired man (scBilibinAskAboutEntranceTree). */
	ACTOR_VILLAGER_ASKS_ABOUT_TREE,
	/* A woman in a green dress. */
	ACTOR_VILLAGER_MCCOY_FRIGHTENED,
	/* An old man with a cane (scBilibinAskAboutSoutheastRoute). */
	ACTOR_OLD_MAN_ASKS_ABOUT_SOUTHEAST,
	/* An old woman. */
	ACTOR_OLD_WOMAN_ROAD_BLOCKED,
	/* A small boy. */
	ACTOR_BOY_SCARED_OF_ENTRANCE,
	/* A little girl. */
	ACTOR_GIRL_RED_SKY,
	/* A strongman, stocky and muscular. */
	ACTOR_STRONGMAN_IMIL_TOWER,
	/* A man with a pickaxe. */
	ACTOR_VILLAGER_STRANGE_CREATURES,
	/* The golden statue in front of the weapon and armor shop, over the hole */
	/* into the Treasury. It slides a cell under Move or a push. */
	ACTOR_VILLAGE_STATUE,
	/* The same man once the curse is lifted, a stocky, muscular man with black
	 */
	/* hair. */
	ACTOR_FORMER_TREE_MAN
} BILIBIN_ACTOR_ID;

/* Actors of the Bilibin Barricade (BilibinBarricade), numbering its */
/* actor placements from ACTOR_FIRST_PLACED. */
typedef enum BILIBIN_BARRICADE_ACTOR_ID {
	/* The stack of two crates at the barricade's left end, which slides a cell
	 */
	/* under Move or a push. */
	ACTOR_BARRICADE_CRATES = ACTOR_FIRST_PLACED,
	/* The bottom of the three trees. */
	ACTOR_BOTTOM_TREE,
	/* The tree 32 pixels up, which the travelers from Imil push down onto its
	 */
	/* side by the river. */
	ACTOR_MIDDLE_TREE,
	/* Jill as a tree, the top of the three. */
	ACTOR_JILL_TREE,
	/* Jill's two friends, once the curse is lifted, east and west of her. They
	 */
	/* are two boys with brown hair. With her they were the "Gang of Three". */
	ACTOR_GANG_MEMBER_EAST,
	ACTOR_GANG_MEMBER_WEST,
	/* Jill herself, once the curse is lifted, drawn as the woman with long fair
	 */
	/* hair the villages use (SPRITE_WOMAN_WITH_FAIR_HAIR), though the text
	   calls */
	/* the three children. */
	ACTOR_JILL,
	/* Unused: a blank actor (SPRITE_WORLD_MAP_FOREST_COVER) placed at the map's
	 */
	/* origin. No code or trigger addresses it, so it never shows or answers */
	/* anything. */
	ACTOR_INVISIBLE_AT_ORIGIN,
	/* The invisible actor (SPRITE_BLANK) that stands where the middle tree lies
	 */
	/* pushed down and answers for it. */
	ACTOR_FALLEN_TREE_STAND_IN
} BILIBIN_BARRICADE_ACTOR_ID;

/* Actors of the Bilibin Treasury (BilibinTreasury), numbering its */
/* actor placements from ACTOR_FIRST_PLACED. */
typedef enum BILIBIN_TREASURY_ACTOR_ID {
	/* The golden statue at the top of the walkway between the pools, which */
	/* slides up and down it under Move or a push. */
	ACTOR_TREASURY_STATUE = ACTOR_FIRST_PLACED,
	/* Gust, a Jupiter Djinni, placed until he has joined. */
	ACTOR_GUST
} BILIBIN_TREASURY_ACTOR_ID;

/* Counts frames between the ripples, one every 30 frames. */
typedef struct BILIBIN_RIPPLE_TIMER {
	s16 frames;
} BILIBIN_RIPPLE_TIMER;

BILIBIN_RIPPLE_TIMER Bilibin_RippleTimer;

#include "shared/actor_facing.c"

/* The party has looked once at the tree at Bilibin's entrance, so a second */
/* look says it looks like a living person. */
#define FLAG_BILIBIN_TREE_EXAMINED FLAG_AREA_TEMP_01
/* McCoy's gate guards have let the party in but it has not yet entered the */
/* palace. The garden guard who reads it is in another area, so it is cleared */
/* before he can. */
#define FLAG_BILIBIN_KEEPING_MCCOY_WAITING FLAG_AREA_TEMP_04

/* Cell behaviors the three maps' triggers answer besides the exits and the */
/* doors, whose cell behaviors are their exits' numbers. */
typedef enum BILIBIN_CELL_BEHAVIOR {
	/* Bilibin's well, cells (20..21, 13..14). */
	CELL_BEHAVIOR_BILIBIN_WELL = 90,
	/* The jar at cell (22, 10), with a Herb in it. */
	CELL_BEHAVIOR_BILIBIN_HERB_JAR = 100,
	/* The jar at cell (9, 19), with 5 coins in it. */
	CELL_BEHAVIOR_BILIBIN_COINS_JAR = 101,
	/* The cells round the village statue's dotted square, (15..17, 12..14), */
	/* where walking into the statue pushes it. */
	CELL_BEHAVIOR_BILIBIN_STATUE_SQUARE = 20,
	/* At the barricade, cell (14, 10), from which Isaac hops north. */
	CELL_BEHAVIOR_BILIBIN_BARRICADE_HOP_NORTH = 9,
	/* At the barricade, cell (8, 17) beside the crates: Isaac hops west from it
	 */
	/* or pushes the crates south. */
	CELL_BEHAVIOR_BILIBIN_BARRICADE_BESIDE_CRATES = 10,
	/* The barricade's sign, cell (5, 22). */
	CELL_BEHAVIOR_BILIBIN_BARRICADE_SIGN = 11,
	/* The cells round the crates' place, where walking into them pushes them.
	 */
	CELL_BEHAVIOR_BILIBIN_BARRICADE_CRATES_PUSH = 12,
	/* Beside the Gang of Three, cells (13, 10..12) to their west and */
	/* (18, 10..12) to their east. */
	CELL_BEHAVIOR_BILIBIN_BARRICADE_BESIDE_GANG = 13,
	/* The barricade's door, cell (10, 17), locked until the curse is lifted. */
	CELL_BEHAVIOR_BILIBIN_BARRICADE_DOOR = 30,
	/* The Treasury's sign, cell (16, 14). */
	CELL_BEHAVIOR_BILIBIN_TREASURY_SIGN = 10,
	/* The statue's alcove at the top of the walkway, cell (14, 6), when empty:
	 */
	/* Isaac hops east or west from it across the pools, or pushes the statue */
	/* south. */
	CELL_BEHAVIOR_BILIBIN_TREASURY_ALCOVE = 11,
	/* The walkway's foot, cell (14, 9), when empty: Isaac pushes the statue */
	/* north from it, or hops south off the walkway. */
	CELL_BEHAVIOR_BILIBIN_TREASURY_WALKWAY_FOOT = 12,
	/* The walkway between them, cells (14, 7) and (14, 8). */
	CELL_BEHAVIOR_BILIBIN_TREASURY_WALKWAY = 13
} BILIBIN_CELL_BEHAVIOR;

/* Spare cells in row 0 of each map, whose behaviors are copied where a statue
 */
/* or the crates leave or land. */
typedef enum BILIBIN_SPARE_CELL {
	SPARE_CELL_ROW = 0,
	VILLAGE_HOLE_CELL_COLUMN =
		0, /* Bilibin's: the exit down into the Treasury */
	VILLAGE_WALL_CELL_COLUMN = 1,
	TREASURY_ALCOVE_CELL_COLUMN = 0, /* the empty alcove's */
	TREASURY_FOOT_CELL_COLUMN = 1,	 /* the empty walkway foot's */
	TREASURY_WALL_CELL_COLUMN = 2,
	BARRICADE_WALL_CELL_COLUMN = 0,
	BARRICADE_PUSH_CELL_COLUMN =
		1 /* CELL_BEHAVIOR_BILIBIN_BARRICADE_CRATES_PUSH */
} BILIBIN_SPARE_CELL;

const SCENE_ENTRANCE *scGetEntrances(void) {
	s16 sceneId = MainRoutineStateWork.next;

	if(sceneId == SCENE_BILIBIN) {
		return BilibinEntrances;
	}
	if(sceneId == SCENE_BILIBIN_BARRICADE) {
		return BilibinBarricadeEntrances;
	}
	if(sceneId == SCENE_BILIBIN_TREASURY) {
		return BilibinTreasuryEntrances;
	}
	return BilibinEntrancesOther;
}

/* Only the Bilibin Treasury has exit regions. */
const SCENE_EXIT_REGION *scGetExitRegions(void) {
	if(MainRoutineStateWork.next == SCENE_BILIBIN_TREASURY) {
		return (const SCENE_EXIT_REGION *)BilibinTreasuryRegions;
	}
	return 0;
}

/* Returns the packed exits shared by the three scenes. */
const u32 *scGetExits(void) { return BilibinExits; }

/* Talking to Gust in the Bilibin Treasury makes Gust, a Jupiter Djinni, join */
/* the party. */
static void scBilibinGustJoins(void) {
	fdDjinnJoinParty(ACTOR_GUST, ELEMENT_JUPITER, DJINNI_INDEX(DJINNI_GUST));
}

/* Selects the actors of Bilibin, the Bilibin Barricade or the Bilibin */
/* Treasury. */
const ACTOR_PLACE *scGetActorPlacements(void) {
	s16 sceneId = MainRoutineStateWork.next;

	if(sceneId == SCENE_BILIBIN) {
		return BilibinActorPlacements;
	}
	if(sceneId == SCENE_BILIBIN_BARRICADE) {
		return BilibinBarricadeActorPlacements;
	}
	if(sceneId == SCENE_BILIBIN_TREASURY) {
		return BilibinTreasuryActorPlacements;
	}
	return BilibinActorPlacementsOther;
}

/* Searching the well in Bilibin, the leader peers in, and its bottom cannot be
 */
/* seen. */
static void scBilibinInspectWell(void) {
	fdEventStart();
	mnFieldMessageShow(MsgFieldPeeredWell, 1);
	mnFieldMessageShow(MsgBilibinBottomNotVisibleLooksVery, 1);
	fdEventEnd();
}

const TRIGGER *scGetTriggers(void) {
	s16 sceneId = MainRoutineStateWork.next;

	if(sceneId == SCENE_BILIBIN) {
		return BilibinTriggers;
	}
	if(sceneId == SCENE_BILIBIN_BARRICADE) {
		return BilibinBarricadeTriggers;
	}
	if(sceneId == SCENE_BILIBIN_TREASURY) {
		return BilibinTreasuryTriggers;
	}
	return BilibinTriggersOther;
}

/* Searching the sign at the Bilibin Barricade: "This area is off-limits to all
 */
/* those without a permit. —Lord McCoy" */
static void scBilibinReadOffLimitsSign(void) {
	fdEventStart();
	mnFieldMessageShow(MsgBilibinAreaOffLimitsThoseWithout, 1);
	fdEventEnd();
}

/* Searching the sign in the Bilibin Treasury: "McCoy's Hidden Warehouse / Do */
/* Not Enter!" */
static void scBilibinReadWarehouseSign(void) {
	fdEventStart();
	mnFieldMessageShow(MsgBilibinMccoysHiddenWarehouseDoNot, 1);
	fdEventEnd();
}

/* The Gang of Three's trees at the Bilibin Barricade look like people. */
static void scBilibinInspectHumanShapedTree(void) {
	fdEventStart();
	mnFieldMessageShow(MsgBilibinThereTreeLooksLikePerson, 1);
	fdEventEnd();
}

/* The tree at Bilibin's entrance almost looks like a man and, looked at again,
 */
/* like a living person. */
static void scBilibinExamineTreeMan(void) {
	fdEventStart();
	fdEventSetMessageId(MsgBilibinItsTreeButAlmostLooks);
	if(fdEventGetFlag(FLAG_BILIBIN_TREE_EXAMINED) != 0) {
		FieldStateWork->msg++;
	}
	fdEventShowMessage(ACTOR_TREE_MAN, 0);
	gmFlagSet(FLAG_BILIBIN_TREE_EXAMINED);
	fdEventEnd();
}

/* A burly man asks about the tree at Bilibin's entrance. */
static void scBilibinAskAboutEntranceTree(void) {
	fdEventStart();
	fdEventSetMessageId(MsgBilibinDidSeeTreeAtEntrance);
	/* "Did you see the tree at the entrance?" */
	/* Yes: "That tree was once a man... He came to our village under a..." */
	/* No: "I can't believe you missed the tree... You should look..." */
	fdEventAskYesNo(ACTOR_VILLAGER_ASKS_ABOUT_TREE, 0);
	fdEventEnd();
}

/* An old man asks whether the party has tried heading southeast. */
static void scBilibinAskAboutSoutheastRoute(void) {
	fdEventStart();
	fdEventSetMessageId(MsgBilibinHaveTriedHeadingSoutheastFrom);
	/* "Have you tried heading southeast from here?" */
	/* Yes: "That barricade was built because there's been trouble in Kolima."
	 */
	/* No: "The road is blocked, so you might as well not even bother. I..." */
	fdEventAskYesNo(ACTOR_OLD_MAN_ASKS_ABOUT_SOUTHEAST, 0);
	fdEventEnd();
}

/* The man who was the tree at the entrance talks about it. */
static void scBilibinAskAboutBeingATree(void) {
	fdEventStart();
	fdEventSetMessageId(MsgBilibinWasTurnedIntoTreeFor);
	/* "I was turned into a tree for a while... Can you believe it?" */
	/* Yes: "I think I now know what it means to sink your roots in one..." */
	/* No: "Yeah, I can't even believe it myself." */
	fdEventAskYesNo(ACTOR_FORMER_TREE_MAN, 0);
	fdEventEnd();
}

/* A small boy asks whether the party broke the curse. */
static void scBilibinAskAboutCurseBreakers(void) {
	fdEventStart();
	fdEventSetMessageId(MsgBilibinCurseWasBrokenThanksEfforts);
	/* "The curse was broken all thanks to the efforts of three..." */
	/* Yes: "That's amazing! You broke the so-called curse!" */
	/* No: "I didn't think so. You don't look very tough at all." */
	fdEventAskYesNo(ACTOR_BOY_SCARED_OF_ENTRANCE, 0);
	fdEventEnd();
}

/* A strongman asks whether the party has been to Imil. */
static void scBilibinAskAboutImil(void) {
	fdEventStart();
	fdEventSetMessageId(MsgBilibinHaveEverBeenVillageImil);
	/* "Have you ever been to the village of Imil, up north?" */
	/* Yes: "Did you see the great lighthouse over there? I bet it was..." */
	/* No: "If you ever go there, be sure to visit their famous lighthouse." */
	fdEventAskYesNo(ACTOR_STRONGMAN_IMIL_TOWER, 0);
	fdEventEnd();
}

/* Walking up to a door in Bilibin opens it, and Isaac walks in: the inn */
/* (cell (23, 22)), the houses east (12, 20), west (6, 21) and north (10, 13),
 */
/* the item shop (4, 13), the weapon and armor shop (18, 11) or the Sanctum's */
/* double door (24..25, 10). */
/* */
/* Everyone in town stops where they are while the door opens. The Sanctum's */
/* door opens with a thump instead of the sound of the others, and the screen */
/* fades as Isaac walks through. */
static void scBilibinOpenDoorAndLeave(void) {
	u32 actorSlot;
	ACTOR *actor;
	s32 exitIndex;
	s32 column;
	s32 row;
	FIELD_STATE *state;

	state = FieldStateWork;
	fdEventStart();
	/* Freeze placed actors before the exit cells start animating. */
	for(actorSlot = ACTOR_FIRST_PLACED; actorSlot < ACTOR_ID_COUNT;
		actorSlot++) {
		actor = fdActorGet(actorSlot);
		if(actor != NULL) {
			actor->motionFlg = 0;
		}
	}
	exitIndex = (s16)(state->stepTrigger - BILIBIN_TO_INN);
	if(exitIndex == BILIBIN_TO_SANCTUM - BILIBIN_TO_INN) {
		sySndPlay(SE_THUMP);
	}
	else {
		sySndPlay(SE_DOOR_OPEN);
	}
	/* Each door has its cell and the steps that draw it opening. */
	column = Bilibin_DoorCells[exitIndex][0];
	row = Bilibin_DoorCells[exitIndex][1];
	fdMapGridPlayMapEdit(Bilibin_DoorSteps[exitIndex], column, row);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.5);
	fdActorGet(ACTOR_PARTY_LEADER)->motionFlg = 0;
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	if(exitIndex == BILIBIN_TO_SANCTUM - BILIBIN_TO_INN) {
		fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
		fdActorSetDestinationOffset(ACTOR_PARTY_LEADER, 0, -4);
	}
	else {
		fdActorCenterAndWalk(ACTOR_PARTY_LEADER, 3, -16);
	}
	if(exitIndex == BILIBIN_TO_ITEM_SHOP - BILIBIN_TO_INN) {
		fdActorSetSprPriority(ACTOR_PARTY_LEADER, 3);
	}
	else {
		fdActorSetSprPriority(ACTOR_PARTY_LEADER, 2);
	}
	fdEventWait(16);
	syImportEventSetExitRequest(exitIndex + BILIBIN_TO_INN);
	fdEventEnd();
}

/* Move on Jill's tree, adrift in the river at the Bilibin Barricade, lets */
/* Isaac pull her ashore. Once Move has brought her one cell east, into cell */
/* column 6, he hauls her out in three tugs, walks back up the bank, and her */
/* thought comes through: she thanks him for saving her from being washed */
/* downstream. */
static void scBilibinPullJillFromRiver(void) {
	ACTOR *leader;
	ACTOR *jillTree;

	leader = fdActorGet(ACTOR_PARTY_LEADER);
	jillTree = fdActorGet(ACTOR_JILL_TREE);
	if(FIELD_CELL(jillTree->x.fixed) == 6) {
		fdEventStart();
		fdActorSetSprPriority(ACTOR_JILL_TREE, 1);
		fdActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
		fdEventWait(20);
		fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.2);
		fdActorSetSpeed(ACTOR_JILL_TREE, 0.2);
		fdActorGet(ACTOR_PARTY_LEADER)->steeringFlg &= ~ACTOR_FACES_MOTION;
		jillTree->motionFlg = 0;
		/* Each tug: the leader faces about, strains and draws her in, then
		 * steps */
		/* on. */
		leader->scaleX = -FIXED_ONE;
		fdActorSetAttachedEffect(ACTOR_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT |
														 ATTACHED_EFFECT_ONCE);
		fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_STRIDE);
		fdActorMoveToAndWait(ACTOR_JILL_TREE, 111, 196);
		leader->scaleX = FIXED_ONE;
		fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 128, 185);
		fdEventWait(20);
		leader->scaleX = -FIXED_ONE;
		fdActorSetAttachedEffect(ACTOR_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT |
														 ATTACHED_EFFECT_ONCE);
		fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_STRIDE);
		fdActorMoveToAndWait(ACTOR_JILL_TREE, 121, 190);
		leader->scaleX = FIXED_ONE;
		fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 141, 189);
		fdEventWait(20);
		leader->scaleX = -FIXED_ONE;
		fdActorSetAttachedEffect(ACTOR_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT |
														 ATTACHED_EFFECT_ONCE);
		fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_STRIDE);
		fdActorMoveToAndWait(ACTOR_JILL_TREE, 132, 186);
		leader->scaleX = FIXED_ONE;
		fdActorGet(ACTOR_PARTY_LEADER)->steeringFlg |= ACTOR_FACES_MOTION;
		fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.6);
		fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 166, 185);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 20);
		fdActorSetSprPriority(ACTOR_JILL_TREE, 2);
		fdPsynergyStartMindReadWave(0, ACTOR_JILL_TREE);
		sySynch(10);
		fdEventSetMessageId(MsgBilibinThankSavedMeFromBeing);
		/* "Thank you! You saved me from being washed downstream!" */
		fdEventShowMessage(ACTOR_JILL_TREE, 0);
		fdPsynergyStopMindReadWave();
		sySynch(10);
		gmFlagSet(FLAG_BILIBIN_JILL_RESCUED);
		fdEventEnd();
	}
}

/* Walking on into a gap from one of a few ledges, Isaac hops across it: in the
 */
/* Treasury east or west across the pools from the statue's empty alcove, or */
/* south off the foot of the walkway; at the Bilibin Barricade north from cell
 */
/* (14, 10), or west from cell (8, 17) beside the crates. */
static void scBilibinHopAcross(void) { fdTryJumpForward(); }

/* Jill and her friends thank the leader with a Hard Nut. */
static void scBilibinJillGivesHardNut(void) {
	ACTOR *leader;

	leader = fdActorGet(ACTOR_PARTY_LEADER);
	if(gmFlagGet(FLAG_KOLIMA_CURSE_LIFTED) == 0 ||
	   gmFlagGet(FLAG_BILIBIN_JILL_RESCUED) == 0)
		return;

	fdEventStart();
	/* The camera moves over to the three; the friend nearer the leader speaks
	 */
	/* first. */
	fdCameraSetSpeed(2.4);
	fdCameraMoveTo(263, CAMERA_KEEP, 173, 1);
	fdCameraWaitForMove();
	if(fdActorGet(ACTOR_GANG_MEMBER_EAST)->x.fixed > leader->x.fixed) {
		fdActorFaceDirection(ACTOR_GANG_MEMBER_WEST, FACING_SOUTH_SOUTHWEST,
							 20);
		fdActorShowEmote(ACTOR_GANG_MEMBER_WEST,
						 EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
		fdEventSetMessageId(MsgBilibinYoureGuy);
		/* "Hey! You're that guy!" */
		fdEventShowMessageThenWait(ACTOR_GANG_MEMBER_WEST, 0, 10);
		fdActorShowEmote(ACTOR_GANG_MEMBER_EAST,
						 EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	}
	else {
		fdActorFaceDirection(ACTOR_GANG_MEMBER_EAST, FACING_SOUTH_SOUTHEAST,
							 20);
		fdActorShowEmote(ACTOR_GANG_MEMBER_EAST,
						 EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
		fdEventSetMessageId(MsgBilibinYoureGuy);
		/* "Hey! You're that guy!" */
		fdEventShowMessageThenWait(ACTOR_GANG_MEMBER_EAST, 0, 10);
		fdActorShowEmote(ACTOR_GANG_MEMBER_WEST,
						 EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	}
	fdActorShowEmote(ACTOR_JILL, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorFaceDirection(ACTOR_JILL, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_GANG_MEMBER_EAST, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_GANG_MEMBER_WEST, FACING_SOUTH_SOUTHEAST, 0);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 268, 184);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 40);
	fdActorRunRepeatedMotion(ACTOR_GANG_MEMBER_WEST, 2);
	/* "Jill, are these the people who saved you?" */
	fdEventShowMessageThenWait(ACTOR_GANG_MEMBER_WEST, 0, 10);
	fdActorFaceDirection(ACTOR_GANG_MEMBER_WEST, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_JILL, FACING_SOUTH_SOUTHEAST, 20);
	fdActorFaceDirection(ACTOR_GANG_MEMBER_EAST, FACING_WEST, 20);
	fdActorSetAnimationAndWait(ACTOR_GANG_MEMBER_EAST, ANIM_NOD);
	fdActorSetAttachedEffect(ACTOR_JILL,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdEventWait(40);
	fdActorFaceDirection(ACTOR_JILL, FACING_SOUTH_SOUTHEAST, 10);
	fdActorFaceDirection(ACTOR_GANG_MEMBER_EAST, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_GANG_MEMBER_WEST, FACING_SOUTH_SOUTHEAST, 10);
	fdActorRunRepeatedMotion(ACTOR_JILL, 1);
	/* "Remember me? I was that tree!" */
	fdEventShowMessageThenWait(ACTOR_JILL, 0, 10);
	fdActorSetAnimation(ACTOR_GANG_MEMBER_EAST, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_GANG_MEMBER_WEST, ANIM_NOD);
	fdEventWait(20);
	/* "I would have been swept down the river if it wasn't for you." */
	fdEventShowMessage(ACTOR_JILL, 0);
	fdActorSetSpeed(ACTOR_JILL, 0.6);
	fdActorGet(ACTOR_JILL)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkToAndWait(ACTOR_JILL, 266, 172);
	fdEventWait(1);
	fdActorGet(ACTOR_JILL)->steeringFlg |= ACTOR_FACES_MOTION;
	fdEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_JILL, ANIM_NOD);
	/* "Please, take this. It is a small token of my appreciation." */
	fdEventShowMessageThenWait(ACTOR_JILL, 0, 10);
	mnFieldMessageShow(MsgBilibinJillGaveIsaacSpecialGift, 1);
	FieldStateWork->msg++;
	gmItemShowFound(ITEM_HARD_NUT, 3);
	gmPartyGiveItem(ITEM_HARD_NUT, 0);
	fdActorSetAnimationAndWait(ACTOR_JILL, ANIM_NOD);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorSetSpeed(ACTOR_JILL, 1);
	fdActorGet(ACTOR_JILL)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkToAndWait(ACTOR_JILL, 262, 156);
	fdEventWait(1);
	{
		ACTOR *jill = fdActorGet(ACTOR_JILL);
		u8 steeringFlags = jill->steeringFlg | ACTOR_FACES_MOTION;

		jill->steeringFlg = steeringFlags;
	}
	fdEventWait(20);
	fdActorRunRepeatedMotion(ACTOR_GANG_MEMBER_EAST, 2);
	fdEventShowMessageThenWait(ACTOR_GANG_MEMBER_EAST, 0, 10);
	fdActorSetAnimation(ACTOR_GANG_MEMBER_EAST, ANIM_NOD);
	fdActorSetAnimation(ACTOR_GANG_MEMBER_WEST, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_JILL, ANIM_NOD);
	/* From now on the three turn to face the leader wherever he goes. */
	syFieldActorWatchAndStartScript(
		ACTOR_GANG_MEMBER_EAST, ACTOR_WATCH_KEEP_MOTION | ACTOR_PARTY_LEADER,
		Bilibin_FaceLeaderScript);
	syFieldActorWatchAndStartScript(
		ACTOR_GANG_MEMBER_WEST, ACTOR_WATCH_KEEP_MOTION | ACTOR_PARTY_LEADER,
		Bilibin_FaceLeaderScript);
	syFieldActorWatchAndStartScript(
		ACTOR_JILL, ACTOR_WATCH_KEEP_MOTION | ACTOR_PARTY_LEADER,
		Bilibin_FaceLeaderScript);
	gmFlagSet(FLAG_BILIBIN_JILL_GIFT_RECEIVED);
	fdEventEnd();
}

/* Sets up the scene being entered, Bilibin, the Bilibin Barricade or the */
/* Bilibin Treasury. */
s32 scInit(void) {
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	if(MainRoutineStateWork.next == SCENE_BILIBIN) {
		scBilibinRestoreVillageActors();
	}
	else if(MainRoutineStateWork.next == SCENE_BILIBIN_BARRICADE) {
		scBilibinRestoreRiverActors();
		syTaskAddFrame(scBilibinUpdateRiverRipples, TASK_PASS_FRAME);
	}
	else if(MainRoutineStateWork.next == SCENE_BILIBIN_TREASURY) {
		scBilibinSetUpTreasury();
	}
	return 0;
}

/* Sets up Bilibin. */
static void scBilibinRestoreVillageActors(void) {
	s16 entrance;

	if(gmFlagGet(FLAG_KOLIMA_CURSE_LIFTED) != 0) {
		fdActorSetPos(ACTOR_TREE_MAN, 0, 0);
		fdActorFaceDirection(ACTOR_OLD_MAN_ASKS_ABOUT_SOUTHEAST,
							 FACING_SOUTH_SOUTHEAST, 0);
		fdActorFaceDirection(ACTOR_OLD_WOMAN_ROAD_BLOCKED,
							 FACING_SOUTH_SOUTHWEST, 0);
	}
	else {
		fdActorSetSprFlags(fdActorGet(ACTOR_TREE_MAN), 0);
		fdActorSetPos(ACTOR_FORMER_TREE_MAN, 0, 0);
	}
	fdActorGet(ACTOR_IVY)->scaleY = 3 * FIXED_ONE / 2;
	/* Whirlwind's row runs no event, so nothing remembers the ivy blown away:
	 */
	/* it is back unless the party has just come out of the cave behind it. */
	entrance = MainRoutineStateWork.entrance;
	if(entrance == BILIBIN_FROM_BILIBIN_TREASURY_WEST) {
		fdActorSetPos(ACTOR_IVY, 0, 0);
	}
	else if(entrance == BILIBIN_FROM_SANCTUM) {
		gmFlagClear(FLAG_SHOW_AREA_NAME);
	}
	if(gmFlagGet(FLAG_KEEP_PARTY_POSITION) == 0) {
		entrance = MainRoutineStateWork.entrance;
		if(entrance == BILIBIN_FROM_BILIBIN_TREASURY_EAST) {
			fdActorSetPos(ACTOR_VILLAGE_STATUE, 248, 216);
		}
	}
	scBilibinUpdateVillageStatueCells();
	if(gmFlagGet(FLAG_MCCOY_PALACE_ADMITTED) != 0 &&
	   gmFlagGet(FLAG_MCCOY_PALACE_VISITED) == 0) {
		gmFlagSet(FLAG_BILIBIN_KEEPING_MCCOY_WAITING);
	}
}

/* The cell the village statue first stands on, over the way down to the */
/* Bilibin Treasury. */
enum { TREASURY_WAY_COLUMN = 16, TREASURY_WAY_ROW = 13 };

static void scBilibinUpdateVillageStatueCells(void) {
	s32 leaderRow;
	s32 leaderColumn;
	ACTOR *leader;
	ACTOR *statue;
	FIXED statueX;
	s32 statueRow;

	leader = fdActorGet(ACTOR_PARTY_LEADER);
	statue = fdActorGet(ACTOR_VILLAGE_STATUE);
	statueRow = FIELD_CELL(statue->z.fixed);
	leaderColumn = FIELD_CELL(leader->x.fixed);
	leaderRow = FIELD_CELL(leader->z.fixed);
	statueX = statue->x.fixed;
	/* Make the statue's square plain pushing ground, then wall off its cell. */
	fdMapGridCopyCellAttributes(15, 11, 3, 1, 15, 12);
	fdMapGridCopyCellAttributes(15, 11, 3, 1, 15, 13);
	fdMapGridCopyCellAttributes(15, 11, 3, 1, 15, 14);
	fdMapGridCopyCellAttributes(VILLAGE_WALL_CELL_COLUMN, SPARE_CELL_ROW, 1, 1,
								FIELD_CELL(statueX), statueRow);
	if(FIELD_CELL(statueX) != TREASURY_WAY_COLUMN ||
	   statueRow != TREASURY_WAY_ROW) {
		fdMapGridCopyCellAttributes(VILLAGE_HOLE_CELL_COLUMN, SPARE_CELL_ROW, 1,
									1, TREASURY_WAY_COLUMN, TREASURY_WAY_ROW);
	}
	/* The statue may have moved onto the leader while this event was running.
	 */
	if(leaderColumn == TREASURY_WAY_COLUMN) {
		if(leaderRow == TREASURY_WAY_ROW) {
			fdEventStart();
			fdActorShowEmote(ACTOR_PARTY_LEADER,
							 EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
			fdActorSetSpeed(ACTOR_PARTY_LEADER, 2);
			fdActorJump(ACTOR_PARTY_LEADER, 6, 0);
			if(statueRow == TREASURY_WAY_ROW) {
				fdActorMoveToAndWait(ACTOR_PARTY_LEADER, 262, 196);
				fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 20);
			}
			else {
				fdActorMoveToAndWait(ACTOR_PARTY_LEADER, 286, 218);
				fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 20);
			}
			fdEventEnd();
		}
	}
}

/* Sets up the Bilibin Barricade. */
static void scBilibinRestoreRiverActors(void) {
	ACTOR *jillTree;
	s32 populationState;
	ACTOR *fallenTree;
	s32 crossingState;

	fallenTree = fdActorGet(ACTOR_MIDDLE_TREE);
	jillTree = fdActorGet(ACTOR_JILL_TREE);
	fdActorSetSprFlags(fdActorGet(ACTOR_BARRICADE_CRATES), 0);
	populationState = gmFlagGet(FLAG_KOLIMA_CURSE_LIFTED);
	if(populationState != 0) {
		/* The curse is lifted: the trees are gone. */
		fdActorSetPos(ACTOR_BOTTOM_TREE, 0, 0);
		fdActorSetPos(ACTOR_MIDDLE_TREE, 0, 0);
		fdActorSetPos(ACTOR_JILL_TREE, 0, 0);
		/* The door in the barricade stands open, and the party can walk
		 * through. */
		fdMapGridCopyCellsTo(56, 15, 40, 15, 1, 2);
		fdMapGridCopyCellAttributes(26, 15, 1, 3, 10, 15);
		if(gmFlagGet(FLAG_BILIBIN_JILL_GIFT_RECEIVED) == 0) {
			if(gmFlagGet(FLAG_BILIBIN_JILL_RESCUED) == 0) {
				fdActorSetPos(ACTOR_JILL, 0, 0);
				fdActorFaceDirection(ACTOR_GANG_MEMBER_EAST,
									 FACING_NORTH_NORTHEAST, 0);
				fdActorFaceDirection(ACTOR_GANG_MEMBER_WEST,
									 FACING_NORTH_NORTHWEST, 0);
			}
		}
		else {
			fdActorFaceDirection(ACTOR_GANG_MEMBER_EAST, FACING_NORTH_NORTHEAST,
								 0);
			fdActorFaceDirection(ACTOR_GANG_MEMBER_WEST, FACING_NORTH_NORTHWEST,
								 0);
		}
	}
	else {
		fdActorSetPos(ACTOR_GANG_MEMBER_EAST, 0, 0);
		fdActorSetPos(ACTOR_GANG_MEMBER_WEST, 0, 0);
		fdActorSetPos(ACTOR_JILL, 0, 0);
		fdActorSetSprFlags(fdActorGet(ACTOR_BOTTOM_TREE), 0);
		fdActorSetSprFlags(fdActorGet(ACTOR_MIDDLE_TREE), 0);
		fdActorSetSprFlags(fdActorGet(ACTOR_JILL_TREE), 0);
		fallenTree->motionFlg = populationState;
		/* Once FLAG_SATUROS_BEATEN_AT_MERCURY is set ("They crossed the river
		 */
		/* after pushing us down"), the middle tree lies on its side and Jill is
		 * in */
		/* the water, or on the bank where she was pulled ashore. */
		crossingState = gmFlagGet(FLAG_SATUROS_BEATEN_AT_MERCURY);
		if(crossingState != 0) {
			fdActorGet(ACTOR_BOTTOM_TREE)->collisionFlg |= ACTOR_NEAR_PSYNERGY;
			fdActorGet(ACTOR_FALLEN_TREE_STAND_IN)->collisionFlg |=
				ACTOR_NEAR_PSYNERGY;
			fdActorGet(ACTOR_JILL_TREE)->collisionFlg |= ACTOR_NEAR_PSYNERGY;
			fdActorSetPos(ACTOR_FALLEN_TREE_STAND_IN, 142, 156);
			fdActorSetSprFlags(fdActorGet(ACTOR_FALLEN_TREE_STAND_IN), 0);
			fdActorSetPos(ACTOR_MIDDLE_TREE, 142, 156);
			fallenTree->spr->rotation = DEGtoANG(90);
			fallenTree->y.fixed -= toFIXED(8);
			if(gmFlagGet(FLAG_BILIBIN_JILL_RESCUED) != 0) {
				fdActorSetPos(ACTOR_JILL_TREE, 132, 186);
			}
			else {
				fdActorSetPos(ACTOR_JILL_TREE, 88, 196);
				fdActorSetSprPriority(ACTOR_JILL_TREE, 3);
				jillTree->collisionFlg |= ACTOR_NEAR_TALK;
			}
		}
		else {
			fallenTree->y.fixed = toFIXED(32);
			jillTree->motionFlg = crossingState;
			jillTree->y.fixed = toFIXED(48);
		}
	}
	scBilibinUpdateCratesCells();
}

/* The cells round the crate's place, as Bilibin_CratesPushCells lists them. */
#define CRATES_LANE_CELLS 10

static void scBilibinUpdateCratesCells(void) {
	ACTOR *actor;
	s32 cratesColumn;
	s32 cratesRow;
	u32 i;

	actor = fdActorGet(ACTOR_BARRICADE_CRATES);
	cratesColumn = FIELD_CELL(actor->x.fixed);
	cratesRow = FIELD_CELL(actor->z.fixed);

	for(i = 0; i < 2 * CRATES_LANE_CELLS; i += 2) {
		fdMapGridCopyCellAttributes(BARRICADE_PUSH_CELL_COLUMN, SPARE_CELL_ROW,
									1, 1, Bilibin_CratesPushCells[i],
									Bilibin_CratesPushCells[i + 1]);
	}

	fdMapGridCopyCellAttributes(BARRICADE_WALL_CELL_COLUMN, SPARE_CELL_ROW, 1,
								1, cratesColumn, cratesRow);
}

/* Sets up the Bilibin Treasury. */
static void scBilibinSetUpTreasury(void) {
	ACTOR *leader;

	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	scBilibinUpdateTreasuryStatueCells();
	if(MainRoutineStateWork.entrance == BILIBIN_TREASURY_UPPER_FROM_LOWER ||
	   MainRoutineStateWork.entrance == BILIBIN_TREASURY_FROM_BILIBIN_NORTH) {
		if(gmFlagGet(FLAG_KEEP_PARTY_POSITION) == 0) {
			leader = fdActorGet(ACTOR_PARTY_LEADER);
			fdEventStart();
			leader->y.fixed = toFIXED(16);
			fdCameraMoveWithinMapBounds(leader->x.fixed, leader->y.fixed,
										leader->z.fixed, 0);
			fdMapGridCenterOnFollowedPoint();
			fdEventEnd();
			sySynch(1);
		}
	}
}

#include "shared/actor_find_at_cell.c"

/* Pushing a statue in Bilibin or the Bilibin Treasury, or the crates at the */
/* Bilibin Barricade, slides it one cell ahead with the leader behind it, if */
/* the cell is free. */
static void scBilibinPushStatueOrCrates(void) {
	ACTOR *leader;
	ACTOR *pushed;
	u32 direction;
	FIELD_POS destination;

	leader = fdActorGet(ACTOR_PARTY_LEADER);
	direction = FACING_INDEX(leader->facing);
	pushed = scActorFindAtCell(
		(leader->x.part[0].integer +
		 (FIXEDtoINT(PushedActor_DirectionSteps[direction]))) >>
			4,
		(leader->z.part[0].integer +
		 (s16)PushedActor_DirectionSteps[direction]) >>
			4);
	if(pushed != NULL) {
		pushed->layer = 2;
		fdActorStepFrom(&destination, pushed,
						PushedActor_DirectionSteps[direction]);
		if(syFieldActorCheckMovementCollision(pushed, &destination) <= 0) {
			/* The leader leans on it, then both move together. */
			fdActorSelectAnimation(leader, ANIM_PUSH);
			sySynch(15);
			sySndPlay(SE_PUSH);
			pushed->speedLimit = toFIXED(1) / 5;
			pushed->acceleration = toFIXED(1) / 5;
			syFieldActorSetMoveTargetFX(pushed, destination.x, destination.y,
										destination.z);
			leader->speedLimit = toFIXED(1) / 5;
			leader->acceleration = toFIXED(1) / 5;
			syFieldActorSetMoveTargetFX(leader, destination.x, destination.y,
										destination.z);
			fdActorWaitForMove(pushed);
			pushed->x.fixed = destination.x;
			pushed->z.fixed = destination.z;
			pushed->velX = 0;
			pushed->velZ = 0;
			fdActorSelectAnimation(leader, ANIM_STAND);
			/* Each scene derives its cell behaviors from this same moved */
			/* actor. */
			if(MainRoutineStateWork.next == SCENE_BILIBIN_BARRICADE)
				scBilibinUpdateCratesCells();
			else if(MainRoutineStateWork.next == SCENE_BILIBIN)
				scBilibinUpdateVillageStatueCells();
			else if(MainRoutineStateWork.next == SCENE_BILIBIN_TREASURY)
				scBilibinUpdateTreasuryStatueCells();
		}
	}
}

/* While Isaac wades in the river at the Bilibin Barricade, west of the fallen
 */
/* tree, a ripple spreads from his feet every 30 frames. It runs every frame. */
static void scBilibinUpdateRiverRipples(void) {
	ACTOR *actor;

	actor = fdActorGet(MainRoutineStateWork.leaderActorId);
	if(actor->x.fixed < toFIXED(142)) {
		if(actor->y.fixed < toFIXED(8)) {
			if(Bilibin_RippleTimer.frames == 0)
				scBilibinSpawnRipple(actor);
			if(++Bilibin_RippleTimer.frames == 30)
				Bilibin_RippleTimer.frames = 0;
		}
		else {
			Bilibin_RippleTimer.frames = 0;
		}
	}
}

/* Spawns a semi-transparent ripple (SPRITE_RIPPLE's spreading rings) at the */
/* wader, which releases itself after 30 frames. */
static void scBilibinSpawnRipple(ACTOR *actor) {
	ACTOR *effect;
	SPR *sprite;

	effect = syFieldActorCreate(SPRITE_RIPPLE, actor->x.fixed, actor->y.fixed,
								actor->z.fixed);
	if(effect == NULL)
		return;

	sprite = effect->spr;
	fdActorStartScript(effect, Bilibin_RippleScript);
	effect->motionFlg = 0;
	effect->layer = 1;
	effect->priFlg = ACTOR_PRIORITY_UNDERFOOT;
	if(sprite == NULL)
		return;

	fdSprStartAnimation(sprite, 2);
	sprite->flg = 0;
	sprite->image.objMode = OBJ_MODE_SEMI_TRANSPARENT;
	sprite->image.pri = 3;
}

static void scBilibinUpdateTreasuryStatueCells(void) {
	ACTOR *statue;

	statue = fdActorGet(ACTOR_TREASURY_STATUE);
	if(statue == NULL)
		return;

	if(FIELD_CELL(statue->z.fixed) == 6)
		fdMapGridCopyCellAttributes(TREASURY_WALL_CELL_COLUMN, SPARE_CELL_ROW,
									1, 1, 14, 6);
	else
		fdMapGridCopyCellAttributes(TREASURY_ALCOVE_CELL_COLUMN, SPARE_CELL_ROW,
									1, 1, 14, 6);

	if(FIELD_CELL(statue->z.fixed) == 9)
		fdMapGridCopyCellAttributes(TREASURY_WALL_CELL_COLUMN, SPARE_CELL_ROW,
									1, 1, 14, 9);
	else
		fdMapGridCopyCellAttributes(TREASURY_FOOT_CELL_COLUMN, SPARE_CELL_ROW,
									1, 1, 14, 9);
}

s32 Bilibin_FaceLeaderScript[] = {
	SC_CALL(scTurnTowardTarget),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

/* Unused: the entrances for a scene other than the three, which never loads */
/* this overlay. Its one row has no SCENE_TABLE_END after it. */
SCENE_ENTRANCE BilibinEntrancesOther[] = {
	{ENTRANCE_DEFAULT, CONDITION_ALWAYS, 168, 0, 168, FACING_SOUTH, 0, -1, -1,
	 -1, -1, 0},
};

SCENE_ENTRANCE BilibinEntrances[] = {
	{BILIBIN_FROM_WORLD_MAP, CONDITION_ALWAYS, 240, 0, 472, FACING_NORTH, 0, -1,
	 -1, -1, -1, 0},
	{BILIBIN_FROM_MCCOY_PALACE_GROUNDS, CONDITION_ALWAYS, 224, 0, 88,
	 FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{BILIBIN_FROM_INN, CONDITION_ALWAYS, 376, 0, 360, FACING_SOUTH, 0, -1, -1,
	 -1, -1, 0},
	{BILIBIN_FROM_HOUSE_EAST, CONDITION_ALWAYS, 200, 0, 328, FACING_SOUTH, 0,
	 -1, -1, -1, -1, 0},
	{BILIBIN_FROM_HOUSE_WEST, CONDITION_ALWAYS, 104, 0, 344, FACING_SOUTH, 0,
	 -1, -1, -1, -1, 0},
	{BILIBIN_FROM_HOUSE_NORTH, CONDITION_ALWAYS, 168, 0, 216, FACING_SOUTH, 0,
	 -1, -1, -1, -1, 0},
	{BILIBIN_FROM_ITEM_SHOP, CONDITION_ALWAYS, 72, 0, 216, FACING_SOUTH, 0, -1,
	 -1, -1, -1, 0},
	{BILIBIN_FROM_WEAPON_AND_ARMOR_SHOP, CONDITION_ALWAYS, 296, 0, 184,
	 FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{BILIBIN_FROM_SANCTUM, CONDITION_ALWAYS, 400, 0, 168, FACING_SOUTH, 0, -1,
	 -1, -1, -1, 0},
	{BILIBIN_FROM_BILIBIN_TREASURY_WEST, CONDITION_ALWAYS, 40, 0, 152,
	 FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{BILIBIN_FROM_BILIBIN_TREASURY_EAST, CONDITION_ALWAYS, 264, 0, 200,
	 FACING_NORTH, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

SCENE_ENTRANCE BilibinBarricadeEntrances[] = {
	{ENTRANCE_DEFAULT, CONDITION_ALWAYS, 168, 0, 168, FACING_SOUTH, 0, -1, -1,
	 -1, -1, 0},
	{BILIBIN_BARRICADE_FROM_WORLD_MAP_WEST, CONDITION_ALWAYS, 56, 0, 376,
	 FACING_EAST, 0, 16, 16, 400, 400, 0},
	{BILIBIN_BARRICADE_FROM_WORLD_MAP_EAST, CONDITION_ALWAYS, 376, 0, 184,
	 FACING_WEST, 0, 16, 16, 400, 400, 0},
	{SCENE_TABLE_END},
};

SCENE_ENTRANCE BilibinTreasuryEntrances[] = {
	{ENTRANCE_DEFAULT, CONDITION_ALWAYS, 232, 0, 144, FACING_SOUTH, 0, -1, -1,
	 -1, -1, 0},
	{BILIBIN_TREASURY_FROM_BILIBIN_SOUTH, CONDITION_ALWAYS, 120, 0, 408,
	 FACING_NORTH, 0, 0, 288, 240, 448, 0},
	{BILIBIN_TREASURY_LOWER_FROM_UPPER, CONDITION_ALWAYS, 120, 0, 384,
	 FACING_SOUTH, 0, 0, 288, 240, 448, 0},
	{BILIBIN_TREASURY_UPPER_FROM_LOWER, CONDITION_ALWAYS, 72, 4, 128,
	 FACING_NORTH, 0, 32, 32, 384, 304, 0},
	{BILIBIN_TREASURY_FROM_BILIBIN_NORTH, CONDITION_ALWAYS, 296, 4, 224,
	 FACING_NORTH, 0, 32, 32, 384, 304, 0},
	{SCENE_TABLE_END},
};

/* clang-format off */
s16 BilibinTreasuryRegions[] = {
    66, 26, 128, 78, 38, 140, CONDITION_ALWAYS, BILIBIN_TREASURY_UPPER_TO_LOWER,
    290, 26, 224, 302, 38, 236, CONDITION_ALWAYS, BILIBIN_TREASURY_TO_BILIBIN_NORTH,
    -1, 0, 0, 0, 0, 0, 0, 0,
};

u32 BilibinExits[] = {
    SCENE_EXITS(Bilibin),
    SCENE_EXIT(BILIBIN_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_BILIBIN),
    SCENE_EXIT(BILIBIN_TO_MCCOY_PALACE_GROUNDS, McCoyPalaceGrounds, MCCOY_PALACE_GROUNDS_FROM_BILIBIN),
    SCENE_EXIT(BILIBIN_TO_INN, CottageRoomsC, COTTAGE_ROOMS_C_INN),
    SCENE_EXIT(BILIBIN_TO_HOUSE_EAST, CottageRoomsC, COTTAGE_ROOMS_C_HOUSE_EAST),
    SCENE_EXIT(BILIBIN_TO_HOUSE_WEST, CottageRoomsC, COTTAGE_ROOMS_C_HOUSE_WEST),
    SCENE_EXIT(BILIBIN_TO_HOUSE_NORTH, CottageRoomsC, COTTAGE_ROOMS_C_HOUSE_NORTH),
    SCENE_EXIT(BILIBIN_TO_ITEM_SHOP, CottageRoomsC, COTTAGE_ROOMS_C_ITEM_SHOP),
    SCENE_EXIT(BILIBIN_TO_WEAPON_AND_ARMOR_SHOP, CottageRoomsC, COTTAGE_ROOMS_C_WEAPON_AND_ARMOR_SHOP),
    SCENE_EXIT(BILIBIN_TO_SANCTUM, Sanctum, SANCTUM_FROM_BILIBIN),
    SCENE_EXIT(BILIBIN_TO_BILIBIN_TREASURY_WEST, BilibinTreasury, BILIBIN_TREASURY_FROM_BILIBIN_SOUTH),
    SCENE_EXIT(BILIBIN_TO_BILIBIN_TREASURY_EAST, BilibinTreasury, BILIBIN_TREASURY_FROM_BILIBIN_NORTH),
    SCENE_EXITS(BilibinTreasury),
    SCENE_EXIT(BILIBIN_TREASURY_TO_BILIBIN_SOUTH, Bilibin, BILIBIN_FROM_BILIBIN_TREASURY_WEST),
    SCENE_EXIT(BILIBIN_TREASURY_LOWER_TO_UPPER, BilibinTreasury, BILIBIN_TREASURY_UPPER_FROM_LOWER),
    SCENE_EXIT(BILIBIN_TREASURY_UPPER_TO_LOWER, BilibinTreasury, BILIBIN_TREASURY_LOWER_FROM_UPPER),
    SCENE_EXIT(BILIBIN_TREASURY_TO_BILIBIN_NORTH, Bilibin, BILIBIN_FROM_BILIBIN_TREASURY_EAST),
    SCENE_EXITS(BilibinBarricade),
    SCENE_EXIT(BILIBIN_BARRICADE_TO_WORLD_MAP_EAST, WorldMap, WORLD_MAP_FROM_BILIBIN_BARRICADE_EAST),
    SCENE_EXIT(BILIBIN_BARRICADE_TO_WORLD_MAP_WEST, WorldMap, WORLD_MAP_FROM_BILIBIN_BARRICADE_WEST),
    SCENE_EXITS_END,
};

ACTOR_PLACE BilibinActorPlacementsOther[] = {
    { SCENE_TABLE_END },
};

ACTOR_PLACE BilibinActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        40, 0, 140, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TREE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        256, 0, 440, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        264, 0, 328, FACING_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        312, 0, 240, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        176, 0, 328, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        128, 0, 224, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        176, 0, 96, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        192, 0, 96, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        208, 0, 200, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        192, 0, 208, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        232, 0, 296, FACING_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_PICKAXE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        392, 0, 360, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_GOLDEN_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        264, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        264, 0, 432, FACING_SOUTH, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};

ACTOR_PLACE BilibinBarricadeActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_CRATES, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        136, 0, 300, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TREE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        272, 0, 154, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TREE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        272, 0, 154, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TREE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        272, 0, 154, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        282, 0, 170, FACING_WEST_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        246, 0, 170, FACING_WEST_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        262, 0, 156, FACING_WEST_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WORLD_MAP_FOREST_COVER, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

ACTOR_PLACE BilibinTreasuryActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_GOLDEN_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        232, 0, 100, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JUPITER_DJINNI, FLAG_DJINNI(DJINNI_GUST), MOVEMENT_TYPE_STAND,
        328, 0, 152, FACING_SOUTH, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};

TRIGGER BilibinTriggersOther[] = {
    { TRIGGER_TABLE_END },
};

TRIGGER BilibinTriggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, BILIBIN_TO_WORLD_MAP },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, BILIBIN_TO_MCCOY_PALACE_GROUNDS },
    { TRIGGER_EXIT, 10, CONDITION_ALWAYS, BILIBIN_TO_BILIBIN_TREASURY_WEST },
    { TRIGGER_EXIT, 11, CONDITION_ALWAYS, BILIBIN_TO_BILIBIN_TREASURY_EAST },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), BILIBIN_TO_INN, CONDITION_ALWAYS, EVENT(scBilibinOpenDoorAndLeave) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), BILIBIN_TO_HOUSE_EAST, CONDITION_ALWAYS, EVENT(scBilibinOpenDoorAndLeave) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), BILIBIN_TO_HOUSE_WEST, CONDITION_ALWAYS, EVENT(scBilibinOpenDoorAndLeave) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), BILIBIN_TO_HOUSE_NORTH, CONDITION_ALWAYS, EVENT(scBilibinOpenDoorAndLeave) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), BILIBIN_TO_ITEM_SHOP, CONDITION_ALWAYS, EVENT(scBilibinOpenDoorAndLeave) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), BILIBIN_TO_WEAPON_AND_ARMOR_SHOP, CONDITION_ALWAYS, EVENT(scBilibinOpenDoorAndLeave) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), BILIBIN_TO_SANCTUM, CONDITION_ALWAYS, EVENT(scBilibinOpenDoorAndLeave) },
    { TRIGGER_TALK, ACTOR_TREE_MAN, CONDITION_ALWAYS, EVENT(scBilibinExamineTreeMan) },
    { TRIGGER_TALK, ACTOR_FORMER_TREE_MAN, CONDITION_ALWAYS, EVENT(scBilibinAskAboutBeingATree) },
    { TRIGGER_TALK, ACTOR_YOUNG_MAN_WELCOME, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinWelcomeToBilibinPleaseJustIgnore) },
    { TRIGGER_TALK, ACTOR_YOUNG_MAN_WELCOME, CONDITION_ALWAYS, MSGNO(MsgBilibinWelcomeToBilibinWellForgetThat) },
    { TRIGGER_TALK, ACTOR_VILLAGER_GROUP_TO_IMIL, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinIHearALargeGroupCrossed) },
    { TRIGGER_TALK, ACTOR_VILLAGER_GROUP_TO_IMIL, CONDITION_ALWAYS, MSGNO(MsgBilibinIfYoureTalkingAboutTheTravelers) },
    { TRIGGER_TALK, ACTOR_VILLAGER_ASKS_ABOUT_TREE, FLAG_KOLIMA_CURSE_LIFTED, EVENT(scBilibinAskAboutEntranceTree) },
    { TRIGGER_TALK, ACTOR_VILLAGER_ASKS_ABOUT_TREE, CONDITION_ALWAYS, MSGNO(MsgBilibinYoureWonderingAboutTheTreeNear) },
    { TRIGGER_TALK, ACTOR_VILLAGER_MCCOY_FRIGHTENED, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinLordMccoyHasBeenQuiteFrightened) },
    { TRIGGER_TALK, ACTOR_VILLAGER_MCCOY_FRIGHTENED, CONDITION_ALWAYS, MSGNO(MsgBilibinISawLordMccoyTheChief) },
    { TRIGGER_TALK, ACTOR_OLD_MAN_ASKS_ABOUT_SOUTHEAST, FLAG_KOLIMA_CURSE_LIFTED, EVENT(scBilibinAskAboutSoutheastRoute) },
    { TRIGGER_TALK, ACTOR_OLD_MAN_ASKS_ABOUT_SOUTHEAST, CONDITION_ALWAYS, MSGNO(MsgBilibinIHearTheBarricadeGateThat) },
    { TRIGGER_TALK, ACTOR_OLD_WOMAN_ROAD_BLOCKED, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinTheRoadToKolimaIsBlocked) },
    { TRIGGER_TALK, ACTOR_OLD_WOMAN_ROAD_BLOCKED, CONDITION_ALWAYS, MSGNO(MsgBilibinThoseThreeKidsVanishedDuringAll) },
    { TRIGGER_TALK, ACTOR_BOY_SCARED_OF_ENTRANCE, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinEverSinceThatManTurnedInto) },
    { TRIGGER_TALK, ACTOR_BOY_SCARED_OF_ENTRANCE, CONDITION_ALWAYS, EVENT(scBilibinAskAboutCurseBreakers) },
    { TRIGGER_TALK, ACTOR_GIRL_RED_SKY, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinIsItTrueThatAMountain) },
    { TRIGGER_TALK, ACTOR_GIRL_RED_SKY, CONDITION_ALWAYS, MSGNO(MsgBilibinLordMccoysMenSaidTheySaw) },
    { TRIGGER_TALK, ACTOR_STRONGMAN_IMIL_TOWER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinTheresATowerAtTheTip) },
    { TRIGGER_TALK, ACTOR_STRONGMAN_IMIL_TOWER, CONDITION_ALWAYS, EVENT(scBilibinAskAboutImil) },
    { TRIGGER_TALK, ACTOR_VILLAGER_STRANGE_CREATURES, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinSomethingChangedInTheEruptionNow) },
    { TRIGGER_TALK, ACTOR_VILLAGER_STRANGE_CREATURES, CONDITION_ALWAYS, MSGNO(MsgBilibinTheEruptionOnTheOtherSide) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TREE_MAN, CONDITION_ALWAYS, MSGNO(MsgBilibinHelpMeSomebody) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FORMER_TREE_MAN, CONDITION_ALWAYS, MSGNO(MsgBilibinINeverWantToBeTurned) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_WELCOME, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinYouGuysAreSoUnluckyArriving) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_WELCOME, CONDITION_ALWAYS, MSGNO(MsgBilibinNotManyLumberjacksAreWillingTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_GROUP_TO_IMIL, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinYouMustBeMadItsAlmost) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_GROUP_TO_IMIL, CONDITION_ALWAYS, MSGNO(MsgBilibinOneOfTheTwoPeopleThat) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ASKS_ABOUT_TREE, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinWeEndedUpLikeThisBecause) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ASKS_ABOUT_TREE, CONDITION_ALWAYS, MSGNO(MsgBilibinThatGuyMightTurnIntoA) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_MCCOY_FRIGHTENED, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinEveryoneGetsWorriedWhenTownLeaders) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_MCCOY_FRIGHTENED, CONDITION_ALWAYS, MSGNO(MsgBilibinLordMccoySeemsToBeHaving) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ASKS_ABOUT_SOUTHEAST, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinWhereDidTheseKidsComeFrom) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ASKS_ABOUT_SOUTHEAST, CONDITION_ALWAYS, MSGNO(MsgBilibinApparentlyJustAboutAnyoneCouldJust) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ROAD_BLOCKED, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinNoOneWouldDareGoAfter) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ROAD_BLOCKED, CONDITION_ALWAYS, MSGNO(MsgBilibinTheGangOfThreeHaTheyre) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_SCARED_OF_ENTRANCE, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinHeBeggedForHelpAsHe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_SCARED_OF_ENTRANCE, CONDITION_ALWAYS, MSGNO(MsgBilibinImSureLordMccoyWillGive) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_RED_SKY, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinIWantToSeeThatPretty) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_RED_SKY, CONDITION_ALWAYS, MSGNO(MsgBilibinTheyMustBeReportingYourDeeds) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRONGMAN_IMIL_TOWER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinOnlyGreatAdeptsCanEnterThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRONGMAN_IMIL_TOWER, CONDITION_ALWAYS, MSGNO(MsgBilibinIWantToShareWithYou) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_STRANGE_CREATURES, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinThatExplosionMustHaveBeenA) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_STRANGE_CREATURES, CONDITION_ALWAYS, MSGNO(MsgBilibinAsLongAsImOkayI) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_VILLAGE_STATUE, CONDITION_ALWAYS, EVENT(scBilibinUpdateVillageStatueCells) },
    { STEP_TRIGGER_WALKING, CELL_BEHAVIOR_BILIBIN_STATUE_SQUARE, CONDITION_ALWAYS, EVENT(scBilibinPushStatueOrCrates) },
    { PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_IVY, CONDITION_ALWAYS, 0 },
    { EXAMINE_TRIGGER(EXAMINE_JAR), CELL_BEHAVIOR_BILIBIN_HERB_JAR, FLAG_BILIBIN_HERB, FIND_ITEM(ITEM_HERB) },
    { EXAMINE_TRIGGER(EXAMINE_JAR), CELL_BEHAVIOR_BILIBIN_COINS_JAR, FLAG_BILIBIN_COINS, FIND_COINS(5) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_BILIBIN_WELL, CONDITION_ALWAYS, EVENT(scBilibinInspectWell) },
    { TRIGGER_TABLE_END },
};

TRIGGER BilibinBarricadeTriggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, BILIBIN_BARRICADE_TO_WORLD_MAP_EAST },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, BILIBIN_BARRICADE_TO_WORLD_MAP_WEST },
    { TRIGGER_TALK, ACTOR_BOTTOM_TREE, CONDITION_ALWAYS, EVENT(scBilibinInspectHumanShapedTree) },
    { TRIGGER_TALK, ACTOR_MIDDLE_TREE, CONDITION_ALWAYS, EVENT(scBilibinInspectHumanShapedTree) },
    { TRIGGER_TALK, ACTOR_JILL_TREE, CONDITION_ALWAYS, EVENT(scBilibinInspectHumanShapedTree) },
    { TRIGGER_TALK, ACTOR_FALLEN_TREE_STAND_IN, CONDITION_ALWAYS, EVENT(scBilibinInspectHumanShapedTree) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOTTOM_TREE, IF_FLAG_SET(FLAG_SATUROS_BEATEN_AT_MERCURY), MSGNO(MsgBilibinTheyCrossedTheRiverAfterPushing) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOTTOM_TREE, CONDITION_ALWAYS, MSGNO(MsgBilibinTheyreSoHeavy) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FALLEN_TREE_STAND_IN, FLAG_BILIBIN_JILL_RESCUED, MSGNO(MsgBilibinShellBeSweptDownstreamIfShe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FALLEN_TREE_STAND_IN, CONDITION_ALWAYS, MSGNO(MsgBilibinOhItLooksLikeSheIs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MIDDLE_TREE, FLAG_BILIBIN_JILL_RESCUED, MSGNO(MsgBilibinShellBeSweptDownstreamIfShe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MIDDLE_TREE, CONDITION_ALWAYS, MSGNO(MsgBilibinOhItLooksLikeSheIs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_JILL_TREE, FLAG_BILIBIN_JILL_RESCUED, MSGNO(MsgBilibinAhhhhPleaseAnyonePleaseHelp) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_JILL_TREE, CONDITION_ALWAYS, MSGNO(MsgBilibinThankSavedMeFromBeing) },
    { TRIGGER_TALK, ACTOR_GANG_MEMBER_EAST, FLAG_BILIBIN_JILL_RESCUED, MSGNO(MsgBilibinIHeardWeWereOnceCalled) },
    { TRIGGER_TALK, ACTOR_GANG_MEMBER_EAST, CONDITION_ALWAYS, MSGNO(MsgBilibinItWasAwfulLuckBeingTurned) },
    { TRIGGER_TALK, ACTOR_GANG_MEMBER_WEST, FLAG_BILIBIN_JILL_RESCUED, MSGNO(MsgBilibinIfOnlyWeHadntTriedGetting) },
    { TRIGGER_TALK, ACTOR_GANG_MEMBER_WEST, CONDITION_ALWAYS, MSGNO(MsgBilibinIReallyLikeThisSpotBy) },
    { TRIGGER_TALK, ACTOR_JILL, CONDITION_ALWAYS, MSGNO(MsgBilibinIWillThinkOfYouWhenever) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GANG_MEMBER_EAST, FLAG_BILIBIN_JILL_RESCUED, MSGNO(MsgBilibinWeWorkedSoHardIfOnly) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GANG_MEMBER_EAST, CONDITION_ALWAYS, MSGNO(MsgBilibinWhoseIdeaWasItToPick) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GANG_MEMBER_WEST, FLAG_BILIBIN_JILL_RESCUED, MSGNO(MsgBilibinWhatACruelWorldWeJust) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GANG_MEMBER_WEST, CONDITION_ALWAYS, MSGNO(MsgBilibinWellItWasntMyIdeaTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_JILL, CONDITION_ALWAYS, MSGNO(MsgBilibinThesePersimmonsAreJustLikeMy) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_BILIBIN_BARRICADE_SIGN, CONDITION_ALWAYS, EVENT(scBilibinReadOffLimitsSign) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_BILIBIN_BARRICADE_DOOR, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgFieldDoorTightlyLocked)) },
    { TRIGGER_STEP, CELL_BEHAVIOR_BILIBIN_BARRICADE_BESIDE_GANG, FLAG_BILIBIN_JILL_GIFT_RECEIVED, EVENT(scBilibinJillGivesHardNut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_JILL_TREE, FLAG_BILIBIN_JILL_RESCUED, EVENT(scBilibinPullJillFromRiver) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_BILIBIN_BARRICADE_HOP_NORTH, CONDITION_ALWAYS, EVENT(scBilibinHopAcross) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_BILIBIN_BARRICADE_BESIDE_CRATES, CONDITION_ALWAYS, EVENT(scBilibinHopAcross) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_BILIBIN_BARRICADE_BESIDE_CRATES, CONDITION_ALWAYS, EVENT(scBilibinPushStatueOrCrates) },
    { STEP_TRIGGER_WALKING, CELL_BEHAVIOR_BILIBIN_BARRICADE_CRATES_PUSH, CONDITION_ALWAYS, EVENT(scBilibinPushStatueOrCrates) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_BARRICADE_CRATES, CONDITION_ALWAYS, EVENT(scBilibinUpdateCratesCells) },
    { TRIGGER_TABLE_END },
};

TRIGGER BilibinTreasuryTriggers[] = {
    { TRIGGER_EXIT, 1, CONDITION_ALWAYS, BILIBIN_TREASURY_TO_BILIBIN_SOUTH },
    { TRIGGER_EXIT, 2, CONDITION_ALWAYS, BILIBIN_TREASURY_LOWER_TO_UPPER },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_BILIBIN_TREASURY_SIGN, CONDITION_ALWAYS, EVENT(scBilibinReadWarehouseSign) },
    { TRIGGER_TALK, ACTOR_GUST, CONDITION_ALWAYS, EVENT(scBilibinGustJoins) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_TREASURY_STATUE, CONDITION_ALWAYS, EVENT(scBilibinUpdateTreasuryStatueCells) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_BILIBIN_TREASURY_ALCOVE, CONDITION_ALWAYS, EVENT(scBilibinPushStatueOrCrates) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_BILIBIN_TREASURY_WALKWAY_FOOT, CONDITION_ALWAYS, EVENT(scBilibinPushStatueOrCrates) },
    { STEP_TRIGGER_WALKING, CELL_BEHAVIOR_BILIBIN_TREASURY_WALKWAY, CONDITION_ALWAYS, EVENT(scBilibinPushStatueOrCrates) },
    { STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_BILIBIN_TREASURY_ALCOVE, CONDITION_ALWAYS, EVENT(scBilibinHopAcross) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_BILIBIN_TREASURY_ALCOVE, CONDITION_ALWAYS, EVENT(scBilibinHopAcross) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_BILIBIN_TREASURY_WALKWAY_FOOT, CONDITION_ALWAYS, EVENT(scBilibinHopAcross) },
    { TRIGGER_TABLE_END },
};

/* Each door's opening, as map grid animations, and the doors in turn. */
u16 Bilibin_SanctumDoorSteps[] = {
    MAPGRID_ANIMATION_STEP(37, 33, 4, 2, 5), MAPGRID_ANIMATION_STEP(33, 33, 4, 2, 5), MAPGRID_ANIMATION_STEPS_END,
};
u16 Bilibin_ItemShopDoorSteps[] = {
    MAPGRID_ANIMATION_STEP(35, 43, 2, 2, 5), MAPGRID_ANIMATION_STEP(33, 43, 2, 2, 5), MAPGRID_ANIMATION_STEPS_END,
};
u16 Bilibin_WeaponAndArmorShopDoorSteps[] = {
    MAPGRID_ANIMATION_STEP(35, 52, 2, 2, 5), MAPGRID_ANIMATION_STEP(33, 52, 2, 2, 5), MAPGRID_ANIMATION_STEPS_END,
};
u16 Bilibin_HouseEastDoorSteps[] = {
    MAPGRID_ANIMATION_STEP(35, 54, 2, 2, 5), MAPGRID_ANIMATION_STEP(33, 54, 2, 2, 5), MAPGRID_ANIMATION_STEPS_END,
};
u16 Bilibin_InnDoorSteps[] = {
    MAPGRID_ANIMATION_STEP(35, 48, 2, 2, 5), MAPGRID_ANIMATION_STEP(33, 48, 2, 2, 5), MAPGRID_ANIMATION_STEPS_END,
};
u16 Bilibin_HouseWestDoorSteps[] = {
    MAPGRID_ANIMATION_STEP(35, 46, 2, 2, 5), MAPGRID_ANIMATION_STEP(33, 46, 2, 2, 5), MAPGRID_ANIMATION_STEPS_END,
};
u16 Bilibin_HouseNorthDoorSteps[] = {
    MAPGRID_ANIMATION_STEP(35, 50, 2, 2, 5), MAPGRID_ANIMATION_STEP(33, 50, 2, 2, 5), MAPGRID_ANIMATION_STEPS_END,
};
u16 *Bilibin_DoorSteps[] = {
    Bilibin_InnDoorSteps,
    Bilibin_HouseEastDoorSteps,
    Bilibin_HouseWestDoorSteps,
    Bilibin_HouseNorthDoorSteps,
    Bilibin_ItemShopDoorSteps,
    Bilibin_WeaponAndArmorShopDoorSteps,
    Bilibin_SanctumDoorSteps,
};

/* Where each door's opening is drawn on the map grid, in the order of */
/* Bilibin_DoorSteps. The doors are drawn on map layer 1, whose cells begin */
/* at the map's column 32: the inn's (54, 20) is that layer's (22, 20), the */
/* door just above the ground cell (23, 22) Isaac walks into (render probe */
/* Bilibin --entrance 3 --mark-cells 22,20,23,21,1). */
s16 Bilibin_DoorCells[][2] = {
    {54, 20}, {43, 18}, {37, 19}, {41, 11}, {35, 11}, {49, 9}, {55, 6},
};

/* The ten cells round the crates' place at the Bilibin Barricade that push */
/* them, as column and row. */
u8 Bilibin_CratesPushCells[] = {
    8,  18, 9,  18, 10, 18, 8,  19, 9,  19,
    10, 19, 11, 19, 9,  20, 10, 20, 11, 20,
};
/* clang-format on */

#include "shared/pushed_actor_direction_steps.c"

s32 Bilibin_RippleScript[] = {
	SC_WAIT(30),
	SC_RELEASE,
};
