/* Bilibin, the Bilibin Treasury and the Bilibin Barricade, three scenes that
   share one overlay; the getters pick each scene's tables by
   MainRoutineStateWork.next.

   Bilibin is the town past Goma Cave, whose chief, Lord McCoy, has closed the
   road to Kolima. Almost everything here turns on FLAG_KOLIMA_CURSE_LIFTED, set
   in Kolima Forest when the party heals Tret: until then a man turned into a
   tree stands at the entrance (Mind Read hears `Help me... Somebody...`), and
   each villager has a line and a thought for either time. Seven doors open as
   the party walks into them: the inn, three houses, the item shop, the weapon
   and armor shop, and the Sanctum's double door.

   Two ways lead into the Treasury, `McCoy's Hidden Warehouse` by its sign,
   where Gust waits: the cave mouth at the town's west edge, behind ivy that
   Whirlwind blows away, and a hole under the golden statue in front of the
   weapon and armor shop, which Move or a push slides aside. Inside, a second
   golden statue slides along a walkway between two pools.

   The Bilibin Barricade is the road east to Kolima, shut by McCoy's wall of
   crates; the stack at its left end slides out and opens the way. By the river
   stand three trees, one on another's shoulders: the Gang of Three, children
   turned into trees by the curse. Once Saturos is beaten at Mercury Lighthouse
   the travelers from Imil have pushed them over to cross, and Jill's tree
   drifts in the river until Isaac pulls her ashore with Move. With the curse
   lifted they are children again, and Jill, if she was saved, gives Isaac a
   Hard Nut. */

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/actor_script.h"
#include "field/script_inline.h"
#include "field/pushed_actor.h"
#include "graphics/graphics.h"
#include "text/msg_ids.h"

extern Sint32		  BilibinFaceLeaderScript[];
extern SCENE_ENTRANCE BilibinEntrancesOther[];
extern SCENE_ENTRANCE BilibinEntrances[];
extern SCENE_ENTRANCE BilibinBarricadeEntrances[];
extern SCENE_ENTRANCE BilibinTreasuryEntrances[];
extern Sint16		  BilibinTreasuryRegions[];
extern Uint32		  BilibinExits[];
extern ACTOR_PLACE	  BilibinActorPlacementsOther[];
extern ACTOR_PLACE	  BilibinActorPlacements[];
extern ACTOR_PLACE	  BilibinBarricadeActorPlacements[];
extern ACTOR_PLACE	  BilibinTreasuryActorPlacements[];
extern CUE			  BilibinCuesOther[];
extern CUE			  BilibinCues[];
extern CUE			  BilibinBarricadeCues[];
extern CUE			  BilibinTreasuryCues[];
extern Uint16		 *BilibinDoorSteps[];
extern Sint16		  BilibinDoorMetatiles[][2];
/* The metatiles round the crates' place that push them, as column and row
   pairs. */
extern Uint8  BilibinCratesPushMetatiles[];
extern Sint32 BilibinRippleScript[];

static void scBilibinRestoreVillageActors(void);
/* Move on the golden statue in front of Bilibin's weapon and armor shop slides
   it a metatile. Moved off metatile (16, 13), it uncovers the hole into the
   Treasury; moved back on, it covers it again.

   The same work follows every push and every arrival in Bilibin. The metatiles
   of the statue's dotted square, (15..17, 12..14), are made ground that pushes
   the statue again, the statue's own metatile is walled off, and the hole's
   metatile is given its exit unless the statue stands on it. If the statue
   lands on the hole while Isaac is standing on it, he starts with a "!" and
   hops clear, to the north or the east. */
static void scBilibinUpdateVillageStatueMetatiles(void);
static void scBilibinRestoreRiverActors(void);
/* Move on the stack of two crates at the left end of the Bilibin Barricade
   drags it out of the wall of crates, opening a gap through the barricade to
   the road to Kolima.

   The same work follows every push and every arrival at the barricade: the ten
   metatiles round the crate's place, listed in BilibinCratesPushMetatiles, are
   made ground that pushes the crate, and the crate's own metatile is walled
   off. */
static void scBilibinUpdateCratesMetatiles(void);
static void scBilibinSetUpTreasury(void);
static void scBilibinUpdateRiverRipples(void);
static void scBilibinSpawnRipple(ACTOR *actor);
/* Move on the golden statue in the Treasury slides it up or down the walkway
   between the two pools, where Gust waits on the ledge to the east.

   The statue rests either in its alcove at the top, metatile (14, 6), or at the
   walkway's foot, metatile (14, 9). The same work follows every push and every
   arrival: the metatile the statue stands on is walled off, the alcove when
   empty lets Isaac hop east or west across the pools, and the foot when empty
   lets him push the statue back up or hop south off the walkway. */
static void scBilibinUpdateTreasuryStatueMetatiles(void);

/* Actors of Bilibin, numbering its actor placements from ACTOR_FIRST_PLACED.
   The ten villagers are named by what they say before the curse is lifted. */
typedef enum BILIBIN_ACTOR_ID {
	/* The bush of ivy over the cave mouth at the west edge, the way into the
	   Treasury, drawn half as tall again. Whirlwind blows it away. */
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
	/* The golden statue in front of the weapon and armor shop, over the hole
	   into the Treasury. It slides a metatile under Move or a push. */
	ACTOR_VILLAGE_STATUE,
	/* The same man once the curse is lifted, a stocky, muscular man with black
	   hair. */
	ACTOR_FORMER_TREE_MAN
} BILIBIN_ACTOR_ID;

/* Actors of the Bilibin Barricade (BilibinBarricade), numbering its actor
   placements from ACTOR_FIRST_PLACED. */
typedef enum BILIBIN_BARRICADE_ACTOR_ID {
	/* The stack of two crates at the barricade's left end, which slides a
	   metatile under Move or a push. */
	ACTOR_BARRICADE_CRATES = ACTOR_FIRST_PLACED,
	/* The bottom of the three trees. */
	ACTOR_BOTTOM_TREE,
	/* The tree 32 pixels up, which the travelers from Imil push down onto its
	   side by the river. */
	ACTOR_MIDDLE_TREE,
	/* Jill as a tree, the top of the three. */
	ACTOR_JILL_TREE,
	/* Jill's two friends, once the curse is lifted, east and west of her. They
	   are two boys with brown hair. With her they were the "Gang of Three". */
	ACTOR_GANG_MEMBER_EAST,
	ACTOR_GANG_MEMBER_WEST,
	/* Jill herself, once the curse is lifted, drawn as the woman with long fair
	   hair the villages use (SPRITE_WOMAN_WITH_FAIR_HAIR), though the text
	   calls the three children. */
	ACTOR_JILL,
	/* Unused: a blank actor (SPRITE_WORLD_MAP_FOREST_COVER) placed at the map's
	   origin. No code or cue addresses it, so it never shows or answers
	   anything. */
	ACTOR_INVISIBLE_AT_ORIGIN,
	/* The invisible actor (SPRITE_BLANK) that stands where the middle tree lies
	   pushed down and answers for it. */
	ACTOR_FALLEN_TREE_STAND_IN
} BILIBIN_BARRICADE_ACTOR_ID;

/* Actors of the Bilibin Treasury (BilibinTreasury), numbering its actor
   placements from ACTOR_FIRST_PLACED. */
typedef enum BILIBIN_TREASURY_ACTOR_ID {
	/* The golden statue at the top of the walkway between the pools, which
	   slides up and down it under Move or a push. */
	ACTOR_TREASURY_STATUE = ACTOR_FIRST_PLACED,
	/* Gust, a Jupiter Djinni, placed until he has joined. */
	ACTOR_GUST
} BILIBIN_TREASURY_ACTOR_ID;

/* Counts ticks between the ripples, one every 30 ticks. */
typedef struct BILIBIN_RIPPLE_TIMER {
	Sint16 ticks;
} BILIBIN_RIPPLE_TIMER;

BILIBIN_RIPPLE_TIMER BilibinRippleTimer;

#include "shared/actor_facing.c"

/* The party has looked once at the tree at Bilibin's entrance, so a second look
   says it looks like a living person. */
#define FLAG_BILIBIN_TREE_EXAMINED		   FLAG_AREA_TEMP_01
/* McCoy's gate guards have let the party in but it has not yet entered the
   palace. The garden guard who reads it is in another area, so it is cleared
   before he can. */
#define FLAG_BILIBIN_KEEPING_MCCOY_WAITING FLAG_AREA_TEMP_04

/* Metatile behaviors the three maps' cues answer besides the exits and the
   doors, whose metatile behaviors are their exits' numbers. */
typedef enum BILIBIN_METATILE_BEHAVIOR {
	/* Bilibin's well, metatiles (20..21, 13..14). */
	METATILE_BEHAVIOR_BILIBIN_WELL = 90,
	/* The jar at metatile (22, 10), with a Herb in it. */
	METATILE_BEHAVIOR_BILIBIN_HERB_JAR = 100,
	/* The jar at metatile (9, 19), with 5 coins in it. */
	METATILE_BEHAVIOR_BILIBIN_COINS_JAR = 101,
	/* The metatiles round the village statue's dotted square, (15..17, 12..14),
	   where walking into the statue pushes it. */
	METATILE_BEHAVIOR_BILIBIN_STATUE_SQUARE = 20,
	/* At the barricade, metatile (14, 10), from which Isaac hops north. */
	METATILE_BEHAVIOR_BILIBIN_BARRICADE_HOP_NORTH = 9,
	/* At the barricade, metatile (8, 17) beside the crates: Isaac hops west
	   from it or pushes the crates south. */
	METATILE_BEHAVIOR_BILIBIN_BARRICADE_BESIDE_CRATES = 10,
	/* The barricade's sign, metatile (5, 22). */
	METATILE_BEHAVIOR_BILIBIN_BARRICADE_SIGN = 11,
	/* The metatiles round the crates' place, where walking into them pushes
	   them. */
	METATILE_BEHAVIOR_BILIBIN_BARRICADE_CRATES_PUSH = 12,
	/* Beside the Gang of Three, metatiles (13, 10..12) to their west and (18,
	   10..12) to their east. */
	METATILE_BEHAVIOR_BILIBIN_BARRICADE_BESIDE_GANG = 13,
	/* The barricade's door, metatile (10, 17), locked until the curse is
	   lifted. */
	METATILE_BEHAVIOR_BILIBIN_BARRICADE_DOOR = 30,
	/* The Treasury's sign, metatile (16, 14). */
	METATILE_BEHAVIOR_BILIBIN_TREASURY_SIGN = 10,
	/* The statue's alcove at the top of the walkway, metatile (14, 6), when
	   empty:
	   Isaac hops east or west from it across the pools, or pushes the statue
	   south. */
	METATILE_BEHAVIOR_BILIBIN_TREASURY_ALCOVE = 11,
	/* The walkway's foot, metatile (14, 9), when empty: Isaac pushes the statue
	   north from it, or hops south off the walkway. */
	METATILE_BEHAVIOR_BILIBIN_TREASURY_WALKWAY_FOOT = 12,
	/* The walkway between them, metatiles (14, 7) and (14, 8). */
	METATILE_BEHAVIOR_BILIBIN_TREASURY_WALKWAY = 13
} BILIBIN_METATILE_BEHAVIOR;

/* Spare metatiles in row 0 of each map, whose behaviors are copied where a
   statue or the crates leave or land. */
typedef enum BILIBIN_SPARE_METATILE {
	SPARE_METATILE_ROW = 0,
	/* Bilibin's: the exit down into the Treasury */
	VILLAGE_HOLE_METATILE_COLUMN = 0,
	VILLAGE_WALL_METATILE_COLUMN = 1,
	TREASURY_ALCOVE_METATILE_COLUMN = 0, /* the empty alcove's */
	TREASURY_FOOT_METATILE_COLUMN = 1,	 /* the empty walkway foot's */
	TREASURY_WALL_METATILE_COLUMN = 2,
	BARRICADE_WALL_METATILE_COLUMN = 0,
	/* METATILE_BEHAVIOR_BILIBIN_BARRICADE_CRATES_PUSH */
	BARRICADE_PUSH_METATILE_COLUMN = 1
} BILIBIN_SPARE_METATILE;

const SCENE_ENTRANCE *scGetEntrances(void)
{
	Sint16 sceneId = MainRoutineStateWork.next;

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
const SCENE_EXIT_REGION *scGetExitRegions(void)
{
	if(MainRoutineStateWork.next == SCENE_BILIBIN_TREASURY) {
		return (const SCENE_EXIT_REGION *)BilibinTreasuryRegions;
	}
	return 0;
}

/* Returns the packed exits shared by the three scenes. */
const Uint32 *scGetExits(void)
{
	return BilibinExits;
}

/* Talking to Gust in the Bilibin Treasury makes Gust, a Jupiter Djinni, join
   the party. */
static void scBilibinGustJoins(void)
{
	fdDjinnJoinParty(ACTOR_GUST, ELEMENT_JUPITER, DJINNI_INDEX(DJINNI_GUST));
}

/* Selects the actors of Bilibin, the Bilibin Barricade or the Bilibin
   Treasury. */
const ACTOR_PLACE *scGetActorPlacements(void)
{
	Sint16 sceneId = MainRoutineStateWork.next;

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
   seen. */
static void scBilibinInspectWell(void)
{
	fdScriptStart();
	mnFieldMessageShow(MsgFieldPeeredWell, 1);
	mnFieldMessageShow(MsgBilibinBottomNotVisibleLooksVery, 1);
	fdScriptEnd();
}

const CUE *scGetCues(void)
{
	Sint16 sceneId = MainRoutineStateWork.next;

	if(sceneId == SCENE_BILIBIN) {
		return BilibinCues;
	}
	if(sceneId == SCENE_BILIBIN_BARRICADE) {
		return BilibinBarricadeCues;
	}
	if(sceneId == SCENE_BILIBIN_TREASURY) {
		return BilibinTreasuryCues;
	}
	return BilibinCuesOther;
}

/* Searching the sign at the Bilibin Barricade: "This area is off-limits to all
   those without a permit. —Lord McCoy" */
static void scBilibinReadOffLimitsSign(void)
{
	fdScriptStart();
	mnFieldMessageShow(MsgBilibinAreaOffLimitsThoseWithout, 1);
	fdScriptEnd();
}

/* Searching the sign in the Bilibin Treasury: "McCoy's Hidden Warehouse / Do
   Not Enter!" */
static void scBilibinReadWarehouseSign(void)
{
	fdScriptStart();
	mnFieldMessageShow(MsgBilibinMccoysHiddenWarehouseDoNot, 1);
	fdScriptEnd();
}

/* The Gang of Three's trees at the Bilibin Barricade look like people. */
static void scBilibinInspectHumanShapedTree(void)
{
	fdScriptStart();
	mnFieldMessageShow(MsgBilibinThereTreeLooksLikePerson, 1);
	fdScriptEnd();
}

/* The tree at Bilibin's entrance almost looks like a man and, looked at again,
   like a living person. */
static void scBilibinExamineTreeMan(void)
{
	fdScriptStart();
	fdScriptSetMessageId(MsgBilibinItsTreeButAlmostLooks);
	if(fdScriptGetFlag(FLAG_BILIBIN_TREE_EXAMINED) != 0) {
		FieldStateWork->msg++;
	}
	fdScriptShowMessage(ACTOR_TREE_MAN, 0);
	gmFlagSet(FLAG_BILIBIN_TREE_EXAMINED);
	fdScriptEnd();
}

/* A burly man asks about the tree at Bilibin's entrance. */
static void scBilibinAskAboutEntranceTree(void)
{
	fdScriptStart();
	fdScriptSetMessageId(MsgBilibinDidSeeTreeAtEntrance);
	/* "Did you see the tree at the entrance?" Yes: "That tree was once a man...
	   He came to our village under a..." No: "I can't believe you missed the
	   tree... You should look..." */
	fdScriptAskYesNo(ACTOR_VILLAGER_ASKS_ABOUT_TREE, 0);
	fdScriptEnd();
}

/* An old man asks whether the party has tried heading southeast. */
static void scBilibinAskAboutSoutheastRoute(void)
{
	fdScriptStart();
	fdScriptSetMessageId(MsgBilibinHaveTriedHeadingSoutheastFrom);
	/* "Have you tried heading southeast from here?" Yes: "That barricade was
	   built because there's been trouble in Kolima." No: "The road is blocked,
	   so you might as well not even bother. I..." */
	fdScriptAskYesNo(ACTOR_OLD_MAN_ASKS_ABOUT_SOUTHEAST, 0);
	fdScriptEnd();
}

/* The man who was the tree at the entrance talks about it. */
static void scBilibinAskAboutBeingATree(void)
{
	fdScriptStart();
	fdScriptSetMessageId(MsgBilibinWasTurnedIntoTreeFor);
	/* "I was turned into a tree for a while... Can you believe it?" Yes: "I
	   think I now know what it means to sink your roots in one..." No: "Yeah, I
	   can't even believe it myself." */
	fdScriptAskYesNo(ACTOR_FORMER_TREE_MAN, 0);
	fdScriptEnd();
}

/* A small boy asks whether the party broke the curse. */
static void scBilibinAskAboutCurseBreakers(void)
{
	fdScriptStart();
	fdScriptSetMessageId(MsgBilibinCurseWasBrokenThanksEfforts);
	/* "The curse was broken all thanks to the efforts of three..." Yes: "That's
	   amazing! You broke the so-called curse!" No: "I didn't think so. You
	   don't look very tough at all." */
	fdScriptAskYesNo(ACTOR_BOY_SCARED_OF_ENTRANCE, 0);
	fdScriptEnd();
}

/* A strongman asks whether the party has been to Imil. */
static void scBilibinAskAboutImil(void)
{
	fdScriptStart();
	fdScriptSetMessageId(MsgBilibinHaveEverBeenVillageImil);
	/* "Have you ever been to the village of Imil, up north?" Yes: "Did you see
	   the great lighthouse over there? I bet it was..." No: "If you ever go
	   there, be sure to visit their famous lighthouse." */
	fdScriptAskYesNo(ACTOR_STRONGMAN_IMIL_TOWER, 0);
	fdScriptEnd();
}

/* Walking up to a door in Bilibin opens it, and Isaac walks in: the inn
   (metatile (23, 22)), the houses east (12, 20), west (6, 21) and north (10,
   13), the item shop (4, 13), the weapon and armor shop (18, 11) or the
   Sanctum's double door (24..25, 10).

   Everyone in town stops where they are while the door opens. The Sanctum's
   door opens with a thump instead of the sound of the others, and the screen
   fades as Isaac walks through. */
static void scBilibinOpenDoorAndLeave(void)
{
	Uint32		 actorSlot;
	ACTOR		*actor;
	Sint32		 exitIndex;
	Sint32		 column;
	Sint32		 row;
	FIELD_STATE *state;

	state = FieldStateWork;
	fdScriptStart();
	/* Freeze placed actors before the exit metatiles start animating. */
	for(actorSlot = ACTOR_FIRST_PLACED; actorSlot < ACTOR_ID_COUNT;
		actorSlot++) {
		actor = fdActorGet(actorSlot);
		if(actor != NULL) {
			actor->motionFlags = 0;
		}
	}
	exitIndex = (Sint16)(state->stepCue - BILIBIN_TO_INN);
	if(exitIndex == BILIBIN_TO_SANCTUM - BILIBIN_TO_INN) {
		sySoundPlay(SE_THUMP);
	} else {
		sySoundPlay(SE_DOOR_OPEN);
	}
	/* Each door has its metatile and the steps that draw it opening. */
	column = BilibinDoorMetatiles[exitIndex][0];
	row = BilibinDoorMetatiles[exitIndex][1];
	fdMapGridPlayMapEdit(BilibinDoorSteps[exitIndex], column, row);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.5);
	fdActorGet(ACTOR_PARTY_LEADER)->motionFlags = 0;
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	if(exitIndex == BILIBIN_TO_SANCTUM - BILIBIN_TO_INN) {
		fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
		fdActorSetDestinationOffset(ACTOR_PARTY_LEADER, 0, -4);
	} else {
		fdActorCenterAndWalk(ACTOR_PARTY_LEADER, 3, -16);
	}
	if(exitIndex == BILIBIN_TO_ITEM_SHOP - BILIBIN_TO_INN) {
		fdActorSetSpritePriority(ACTOR_PARTY_LEADER, 3);
	} else {
		fdActorSetSpritePriority(ACTOR_PARTY_LEADER, 2);
	}
	fdScriptWait(16);
	syImportScriptSetExitRequest(exitIndex + BILIBIN_TO_INN);
	fdScriptEnd();
}

/* Move on Jill's tree, adrift in the river at the Bilibin Barricade, lets Isaac
   pull her ashore. Once Move has brought her one metatile east, into metatile
   column 6, he hauls her out in three tugs, walks back up the bank, and her
   thought comes through: she thanks him for saving her from being washed
   downstream. */
static void scBilibinPullJillFromRiver(void)
{
	ACTOR *leader;
	ACTOR *jillTree;

	leader = fdActorGet(ACTOR_PARTY_LEADER);
	jillTree = fdActorGet(ACTOR_JILL_TREE);
	if(FIELD_METATILE(jillTree->x.fixed) == 6) {
		fdScriptStart();
		fdActorSetSpritePriority(ACTOR_JILL_TREE, 1);
		fdActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
		fdScriptWait(20);
		fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.2);
		fdActorSetSpeed(ACTOR_JILL_TREE, 0.2);
		fdActorGet(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
		jillTree->motionFlags = 0;
		/* Each tug: the leader faces about, strains and draws her in, then
		   steps on. */
		leader->scaleX = -toFIXED(1);
		fdActorSetAttachedEffect(ACTOR_PARTY_LEADER,
			ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
		fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_STRIDE);
		fdActorMoveToAndWait(ACTOR_JILL_TREE, 111, 196);
		leader->scaleX = toFIXED(1);
		fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 128, 185);
		fdScriptWait(20);
		leader->scaleX = -toFIXED(1);
		fdActorSetAttachedEffect(ACTOR_PARTY_LEADER,
			ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
		fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_STRIDE);
		fdActorMoveToAndWait(ACTOR_JILL_TREE, 121, 190);
		leader->scaleX = toFIXED(1);
		fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 141, 189);
		fdScriptWait(20);
		leader->scaleX = -toFIXED(1);
		fdActorSetAttachedEffect(ACTOR_PARTY_LEADER,
			ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
		fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_STRIDE);
		fdActorMoveToAndWait(ACTOR_JILL_TREE, 132, 186);
		leader->scaleX = toFIXED(1);
		fdActorGet(ACTOR_PARTY_LEADER)->steeringFlags |= ACTOR_FACES_MOTION;
		fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.6);
		fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 166, 185);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 20);
		fdActorSetSpritePriority(ACTOR_JILL_TREE, 2);
		fdPsynergyStartMindReadWave(0, ACTOR_JILL_TREE);
		sySynch(10);
		fdScriptSetMessageId(MsgBilibinThankSavedMeFromBeing);
		/* "Thank you! You saved me from being washed downstream!" */
		fdScriptShowMessage(ACTOR_JILL_TREE, 0);
		fdPsynergyStopMindReadWave();
		sySynch(10);
		gmFlagSet(FLAG_BILIBIN_JILL_RESCUED);
		fdScriptEnd();
	}
}

/* Walking on into a gap from one of a few ledges, Isaac hops across it: in the
   Treasury east or west across the pools from the statue's empty alcove, or
   south off the foot of the walkway; at the Bilibin Barricade north from
   metatile (14, 10), or west from metatile (8, 17) beside the crates. */
static void scBilibinHopAcross(void)
{
	fdTryJumpForward();
}

/* Jill and her friends thank the leader with a Hard Nut. */
static void scBilibinJillGivesHardNut(void)
{
	ACTOR *leader;

	leader = fdActorGet(ACTOR_PARTY_LEADER);
	if(gmFlagGet(FLAG_KOLIMA_CURSE_LIFTED) == 0 ||
		gmFlagGet(FLAG_BILIBIN_JILL_RESCUED) == 0)
		return;

	fdScriptStart();
	/* The camera moves over to the three; the friend nearer the leader speaks
	   first. */
	fdCameraSetSpeed(2.4);
	fdCameraMoveTo(263, CAMERA_KEEP, 173, 1);
	fdCameraWaitForMove();
	if(fdActorGet(ACTOR_GANG_MEMBER_EAST)->x.fixed > leader->x.fixed) {
		fdActorFaceDirection(
			ACTOR_GANG_MEMBER_WEST, FACING_SOUTH_SOUTHWEST, 20);
		fdActorShowEmote(
			ACTOR_GANG_MEMBER_WEST, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
		fdScriptSetMessageId(MsgBilibinYoureGuy);
		/* "Hey! You're that guy!" */
		fdScriptShowMessageThenWait(ACTOR_GANG_MEMBER_WEST, 0, 10);
		fdActorShowEmote(
			ACTOR_GANG_MEMBER_EAST, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	} else {
		fdActorFaceDirection(
			ACTOR_GANG_MEMBER_EAST, FACING_SOUTH_SOUTHEAST, 20);
		fdActorShowEmote(
			ACTOR_GANG_MEMBER_EAST, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
		fdScriptSetMessageId(MsgBilibinYoureGuy);
		/* "Hey! You're that guy!" */
		fdScriptShowMessageThenWait(ACTOR_GANG_MEMBER_EAST, 0, 10);
		fdActorShowEmote(
			ACTOR_GANG_MEMBER_WEST, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	}
	fdActorShowEmote(ACTOR_JILL, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorFaceDirection(ACTOR_JILL, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_GANG_MEMBER_EAST, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_GANG_MEMBER_WEST, FACING_SOUTH_SOUTHEAST, 0);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 268, 184);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 40);
	fdActorRunRepeatedMotion(ACTOR_GANG_MEMBER_WEST, 2);
	/* "Jill, are these the people who saved you?" */
	fdScriptShowMessageThenWait(ACTOR_GANG_MEMBER_WEST, 0, 10);
	fdActorFaceDirection(ACTOR_GANG_MEMBER_WEST, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_JILL, FACING_SOUTH_SOUTHEAST, 20);
	fdActorFaceDirection(ACTOR_GANG_MEMBER_EAST, FACING_WEST, 20);
	fdActorSetAnimationAndWait(ACTOR_GANG_MEMBER_EAST, ANIM_NOD);
	fdActorSetAttachedEffect(
		ACTOR_JILL, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(40);
	fdActorFaceDirection(ACTOR_JILL, FACING_SOUTH_SOUTHEAST, 10);
	fdActorFaceDirection(ACTOR_GANG_MEMBER_EAST, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_GANG_MEMBER_WEST, FACING_SOUTH_SOUTHEAST, 10);
	fdActorRunRepeatedMotion(ACTOR_JILL, 1);
	/* "Remember me? I was that tree!" */
	fdScriptShowMessageThenWait(ACTOR_JILL, 0, 10);
	fdActorSetAnimation(ACTOR_GANG_MEMBER_EAST, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_GANG_MEMBER_WEST, ANIM_NOD);
	fdScriptWait(20);
	/* "I would have been swept down the river if it wasn't for you." */
	fdScriptShowMessage(ACTOR_JILL, 0);
	fdActorSetSpeed(ACTOR_JILL, 0.6);
	fdActorGet(ACTOR_JILL)->steeringFlags &= ~ACTOR_FACES_MOTION;
	fdActorWalkToAndWait(ACTOR_JILL, 266, 172);
	fdScriptWait(1);
	fdActorGet(ACTOR_JILL)->steeringFlags |= ACTOR_FACES_MOTION;
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_JILL, ANIM_NOD);
	/* "Please, take this. It is a small token of my appreciation." */
	fdScriptShowMessageThenWait(ACTOR_JILL, 0, 10);
	mnFieldMessageShow(MsgBilibinJillGaveIsaacSpecialGift, 1);
	FieldStateWork->msg++;
	gmItemShowFound(ITEM_HARD_NUT, 3);
	gmPartyGiveItem(ITEM_HARD_NUT, 0);
	fdActorSetAnimationAndWait(ACTOR_JILL, ANIM_NOD);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorSetSpeed(ACTOR_JILL, 1);
	fdActorGet(ACTOR_JILL)->steeringFlags &= ~ACTOR_FACES_MOTION;
	fdActorWalkToAndWait(ACTOR_JILL, 262, 156);
	fdScriptWait(1);
	{
		ACTOR *jill = fdActorGet(ACTOR_JILL);
		Uint8  steeringFlags = jill->steeringFlags | ACTOR_FACES_MOTION;

		jill->steeringFlags = steeringFlags;
	}
	fdScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_GANG_MEMBER_EAST, 2);
	fdScriptShowMessageThenWait(ACTOR_GANG_MEMBER_EAST, 0, 10);
	fdActorSetAnimation(ACTOR_GANG_MEMBER_EAST, ANIM_NOD);
	fdActorSetAnimation(ACTOR_GANG_MEMBER_WEST, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_JILL, ANIM_NOD);
	/* From now on the three turn to face the leader wherever he goes. */
	syFieldActorWatchAndStartScript(ACTOR_GANG_MEMBER_EAST,
		ACTOR_WATCH_KEEP_MOTION | ACTOR_PARTY_LEADER, BilibinFaceLeaderScript);
	syFieldActorWatchAndStartScript(ACTOR_GANG_MEMBER_WEST,
		ACTOR_WATCH_KEEP_MOTION | ACTOR_PARTY_LEADER, BilibinFaceLeaderScript);
	syFieldActorWatchAndStartScript(ACTOR_JILL,
		ACTOR_WATCH_KEEP_MOTION | ACTOR_PARTY_LEADER, BilibinFaceLeaderScript);
	gmFlagSet(FLAG_BILIBIN_JILL_GIFT_RECEIVED);
	fdScriptEnd();
}

/* Sets up the scene being entered, Bilibin, the Bilibin Barricade or the
   Bilibin Treasury. */
Sint32 scInit(void)
{
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	if(MainRoutineStateWork.next == SCENE_BILIBIN) {
		scBilibinRestoreVillageActors();
	} else if(MainRoutineStateWork.next == SCENE_BILIBIN_BARRICADE) {
		scBilibinRestoreRiverActors();
		sySetTickEvent(scBilibinUpdateRiverRipples, EVENT_PASS_TICK);
	} else if(MainRoutineStateWork.next == SCENE_BILIBIN_TREASURY) {
		scBilibinSetUpTreasury();
	}
	return 0;
}

/* Sets up Bilibin. */
static void scBilibinRestoreVillageActors(void)
{
	Sint16 entrance;

	if(gmFlagGet(FLAG_KOLIMA_CURSE_LIFTED) != 0) {
		fdActorSetPos(ACTOR_TREE_MAN, 0, 0);
		fdActorFaceDirection(
			ACTOR_OLD_MAN_ASKS_ABOUT_SOUTHEAST, FACING_SOUTH_SOUTHEAST, 0);
		fdActorFaceDirection(
			ACTOR_OLD_WOMAN_ROAD_BLOCKED, FACING_SOUTH_SOUTHWEST, 0);
	} else {
		fdActorSetSpriteFlags(fdActorGet(ACTOR_TREE_MAN), 0);
		fdActorSetPos(ACTOR_FORMER_TREE_MAN, 0, 0);
	}
	fdActorGet(ACTOR_IVY)->scaleY = toFIXED(3) / 2;
	/* Whirlwind's row runs no script, so nothing remembers the ivy blown away:
	   it is back unless the party has just come out of the cave behind it. */
	entrance = MainRoutineStateWork.entrance;
	if(entrance == BILIBIN_FROM_BILIBIN_TREASURY_WEST) {
		fdActorSetPos(ACTOR_IVY, 0, 0);
	} else if(entrance == BILIBIN_FROM_SANCTUM) {
		gmFlagClear(FLAG_SHOW_AREA_NAME);
	}
	if(gmFlagGet(FLAG_KEEP_PARTY_POSITION) == 0) {
		entrance = MainRoutineStateWork.entrance;
		if(entrance == BILIBIN_FROM_BILIBIN_TREASURY_EAST) {
			fdActorSetPos(ACTOR_VILLAGE_STATUE, 248, 216);
		}
	}
	scBilibinUpdateVillageStatueMetatiles();
	if(gmFlagGet(FLAG_MCCOY_PALACE_ADMITTED) != 0 &&
		gmFlagGet(FLAG_MCCOY_PALACE_VISITED) == 0) {
		gmFlagSet(FLAG_BILIBIN_KEEPING_MCCOY_WAITING);
	}
}

/* The metatile the village statue first stands on, over the way down to the
   Bilibin Treasury. */
enum { TREASURY_WAY_COLUMN = 16, TREASURY_WAY_ROW = 13 };

static void scBilibinUpdateVillageStatueMetatiles(void)
{
	Sint32 leaderRow;
	Sint32 leaderColumn;
	ACTOR *leader;
	ACTOR *statue;
	FIXED  statueX;
	Sint32 statueRow;

	leader = fdActorGet(ACTOR_PARTY_LEADER);
	statue = fdActorGet(ACTOR_VILLAGE_STATUE);
	statueRow = FIELD_METATILE(statue->z.fixed);
	leaderColumn = FIELD_METATILE(leader->x.fixed);
	leaderRow = FIELD_METATILE(leader->z.fixed);
	statueX = statue->x.fixed;
	/* Make the statue's square plain pushing ground, then wall off its
	   metatile. */
	fdMapGridCopyMetatileAttributes(15, 11, 3, 1, 15, 12);
	fdMapGridCopyMetatileAttributes(15, 11, 3, 1, 15, 13);
	fdMapGridCopyMetatileAttributes(15, 11, 3, 1, 15, 14);
	fdMapGridCopyMetatileAttributes(VILLAGE_WALL_METATILE_COLUMN,
		SPARE_METATILE_ROW, 1, 1, FIELD_METATILE(statueX), statueRow);
	if(FIELD_METATILE(statueX) != TREASURY_WAY_COLUMN ||
		statueRow != TREASURY_WAY_ROW) {
		fdMapGridCopyMetatileAttributes(VILLAGE_HOLE_METATILE_COLUMN,
			SPARE_METATILE_ROW, 1, 1, TREASURY_WAY_COLUMN, TREASURY_WAY_ROW);
	}
	/* The statue may have moved onto the leader while this script was
	   running. */
	if(leaderColumn == TREASURY_WAY_COLUMN) {
		if(leaderRow == TREASURY_WAY_ROW) {
			fdScriptStart();
			fdActorShowEmote(
				ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
			fdActorSetSpeed(ACTOR_PARTY_LEADER, 2);
			fdActorJump(ACTOR_PARTY_LEADER, 6, 0);
			if(statueRow == TREASURY_WAY_ROW) {
				fdActorMoveToAndWait(ACTOR_PARTY_LEADER, 262, 196);
				fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 20);
			} else {
				fdActorMoveToAndWait(ACTOR_PARTY_LEADER, 286, 218);
				fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 20);
			}
			fdScriptEnd();
		}
	}
}

/* Sets up the Bilibin Barricade. */
static void scBilibinRestoreRiverActors(void)
{
	ACTOR *jillTree;
	Sint32 populationState;
	ACTOR *fallenTree;
	Sint32 crossingState;

	fallenTree = fdActorGet(ACTOR_MIDDLE_TREE);
	jillTree = fdActorGet(ACTOR_JILL_TREE);
	fdActorSetSpriteFlags(fdActorGet(ACTOR_BARRICADE_CRATES), 0);
	populationState = gmFlagGet(FLAG_KOLIMA_CURSE_LIFTED);
	if(populationState != 0) {
		/* The curse is lifted: the trees are gone. */
		fdActorSetPos(ACTOR_BOTTOM_TREE, 0, 0);
		fdActorSetPos(ACTOR_MIDDLE_TREE, 0, 0);
		fdActorSetPos(ACTOR_JILL_TREE, 0, 0);
		/* The door in the barricade stands open, and the party can walk
		   through. */
		fdMapGridCopyMetatilesTo(56, 15, 40, 15, 1, 2);
		fdMapGridCopyMetatileAttributes(26, 15, 1, 3, 10, 15);
		if(gmFlagGet(FLAG_BILIBIN_JILL_GIFT_RECEIVED) == 0) {
			if(gmFlagGet(FLAG_BILIBIN_JILL_RESCUED) == 0) {
				fdActorSetPos(ACTOR_JILL, 0, 0);
				fdActorFaceDirection(
					ACTOR_GANG_MEMBER_EAST, FACING_NORTH_NORTHEAST, 0);
				fdActorFaceDirection(
					ACTOR_GANG_MEMBER_WEST, FACING_NORTH_NORTHWEST, 0);
			}
		} else {
			fdActorFaceDirection(
				ACTOR_GANG_MEMBER_EAST, FACING_NORTH_NORTHEAST, 0);
			fdActorFaceDirection(
				ACTOR_GANG_MEMBER_WEST, FACING_NORTH_NORTHWEST, 0);
		}
	} else {
		fdActorSetPos(ACTOR_GANG_MEMBER_EAST, 0, 0);
		fdActorSetPos(ACTOR_GANG_MEMBER_WEST, 0, 0);
		fdActorSetPos(ACTOR_JILL, 0, 0);
		fdActorSetSpriteFlags(fdActorGet(ACTOR_BOTTOM_TREE), 0);
		fdActorSetSpriteFlags(fdActorGet(ACTOR_MIDDLE_TREE), 0);
		fdActorSetSpriteFlags(fdActorGet(ACTOR_JILL_TREE), 0);
		fallenTree->motionFlags = populationState;
		/* Once FLAG_SATUROS_BEATEN_AT_MERCURY is set ("They crossed the river
		   after pushing us down"), the middle tree lies on its side and Jill is
		   in the water, or on the bank where she was pulled ashore. */
		crossingState = gmFlagGet(FLAG_SATUROS_BEATEN_AT_MERCURY);
		if(crossingState != 0) {
			fdActorGet(ACTOR_BOTTOM_TREE)->collisionFlags |=
				ACTOR_NEAR_PSYNERGY;
			fdActorGet(ACTOR_FALLEN_TREE_STAND_IN)->collisionFlags |=
				ACTOR_NEAR_PSYNERGY;
			fdActorGet(ACTOR_JILL_TREE)->collisionFlags |= ACTOR_NEAR_PSYNERGY;
			fdActorSetPos(ACTOR_FALLEN_TREE_STAND_IN, 142, 156);
			fdActorSetSpriteFlags(fdActorGet(ACTOR_FALLEN_TREE_STAND_IN), 0);
			fdActorSetPos(ACTOR_MIDDLE_TREE, 142, 156);
			fallenTree->sprite->rotation = DEGtoANG(90);
			fallenTree->y.fixed -= toFIXED(8);
			if(gmFlagGet(FLAG_BILIBIN_JILL_RESCUED) != 0) {
				fdActorSetPos(ACTOR_JILL_TREE, 132, 186);
			} else {
				fdActorSetPos(ACTOR_JILL_TREE, 88, 196);
				fdActorSetSpritePriority(ACTOR_JILL_TREE, 3);
				jillTree->collisionFlags |= ACTOR_NEAR_TALK;
			}
		} else {
			fallenTree->y.fixed = toFIXED(32);
			jillTree->motionFlags = crossingState;
			jillTree->y.fixed = toFIXED(48);
		}
	}
	scBilibinUpdateCratesMetatiles();
}

/* The metatiles round the crate's place, as BilibinCratesPushMetatiles lists
   them. */
#define CRATES_LANE_METATILES 10

static void scBilibinUpdateCratesMetatiles(void)
{
	ACTOR *actor;
	Sint32 cratesColumn;
	Sint32 cratesRow;
	Uint32 i;

	actor = fdActorGet(ACTOR_BARRICADE_CRATES);
	cratesColumn = FIELD_METATILE(actor->x.fixed);
	cratesRow = FIELD_METATILE(actor->z.fixed);

	for(i = 0; i < 2 * CRATES_LANE_METATILES; i += 2) {
		fdMapGridCopyMetatileAttributes(BARRICADE_PUSH_METATILE_COLUMN,
			SPARE_METATILE_ROW, 1, 1, BilibinCratesPushMetatiles[i],
			BilibinCratesPushMetatiles[i + 1]);
	}

	fdMapGridCopyMetatileAttributes(BARRICADE_WALL_METATILE_COLUMN,
		SPARE_METATILE_ROW, 1, 1, cratesColumn, cratesRow);
}

/* Sets up the Bilibin Treasury. */
static void scBilibinSetUpTreasury(void)
{
	ACTOR *leader;

	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	scBilibinUpdateTreasuryStatueMetatiles();
	if(MainRoutineStateWork.entrance == BILIBIN_TREASURY_UPPER_FROM_LOWER ||
		MainRoutineStateWork.entrance == BILIBIN_TREASURY_FROM_BILIBIN_NORTH) {
		if(gmFlagGet(FLAG_KEEP_PARTY_POSITION) == 0) {
			leader = fdActorGet(ACTOR_PARTY_LEADER);
			fdScriptStart();
			leader->y.fixed = toFIXED(16);
			fdCameraMoveWithinMapBounds(
				leader->x.fixed, leader->y.fixed, leader->z.fixed, 0);
			fdMapGridCenterOnFollowedPoint();
			fdScriptEnd();
			sySynch(1);
		}
	}
}

#include "shared/actor_find_at_metatile.c"

/* Pushing a statue in Bilibin or the Bilibin Treasury, or the crates at the
   Bilibin Barricade, slides it one metatile ahead with the leader behind it, if
   the metatile is free. */
static void scBilibinPushStatueOrCrates(void)
{
	ACTOR	 *leader;
	ACTOR	 *pushed;
	Uint32	  direction;
	FIELD_POS dest;

	leader = fdActorGet(ACTOR_PARTY_LEADER);
	direction = FACING_INDEX(leader->facing);
	pushed = scActorFindAtMetatile(
		(leader->x.part[0].integer +
			(FIXEDtoINT(PushedActorDirectionSteps[direction]))) >>
			4,
		(leader->z.part[0].integer +
			(Sint16)PushedActorDirectionSteps[direction]) >>
			4);
	if(pushed != NULL) {
		pushed->layer = 2;
		fdActorStepFrom(&dest, pushed, PushedActorDirectionSteps[direction]);
		if(syFieldActorCheckMovementCollision(pushed, &dest) <= 0) {
			/* The leader leans on it, then both move together. */
			fdActorSelectAnimation(leader, ANIM_PUSH);
			sySynch(15);
			sySoundPlay(SE_PUSH);
			pushed->speedLimit = toFIXED(1) / 5;
			pushed->acceleration = toFIXED(1) / 5;
			syFieldActorSetMoveTargetFX(pushed, dest.x, dest.y, dest.z);
			leader->speedLimit = toFIXED(1) / 5;
			leader->acceleration = toFIXED(1) / 5;
			syFieldActorSetMoveTargetFX(leader, dest.x, dest.y, dest.z);
			fdActorWaitForMove(pushed);
			pushed->x.fixed = dest.x;
			pushed->z.fixed = dest.z;
			pushed->velocityX = 0;
			pushed->velocityZ = 0;
			fdActorSelectAnimation(leader, ANIM_STAND);
			/* Each scene derives its metatile behaviors from this same moved
			   actor. */
			if(MainRoutineStateWork.next == SCENE_BILIBIN_BARRICADE)
				scBilibinUpdateCratesMetatiles();
			else if(MainRoutineStateWork.next == SCENE_BILIBIN)
				scBilibinUpdateVillageStatueMetatiles();
			else if(MainRoutineStateWork.next == SCENE_BILIBIN_TREASURY)
				scBilibinUpdateTreasuryStatueMetatiles();
		}
	}
}

/* While Isaac wades in the river at the Bilibin Barricade, west of the fallen
   tree, a ripple spreads from his feet every 30 ticks. It runs every tick. */
static void scBilibinUpdateRiverRipples(void)
{
	ACTOR *actor;

	actor = fdActorGet(MainRoutineStateWork.leaderActorId);
	if(actor->x.fixed < toFIXED(142)) {
		if(actor->y.fixed < toFIXED(8)) {
			if(BilibinRippleTimer.ticks == 0)
				scBilibinSpawnRipple(actor);
			if(++BilibinRippleTimer.ticks == 30)
				BilibinRippleTimer.ticks = 0;
		} else {
			BilibinRippleTimer.ticks = 0;
		}
	}
}

/* Spawns a semi-transparent ripple (SPRITE_RIPPLE's spreading rings) at the
   wader, which releases itself after 30 ticks. */
static void scBilibinSpawnRipple(ACTOR *actor)
{
	ACTOR *effect;
	SPR	  *sprite;

	effect = syFieldActorCreate(
		SPRITE_RIPPLE, actor->x.fixed, actor->y.fixed, actor->z.fixed);
	if(effect == NULL)
		return;

	sprite = effect->sprite;
	fdActorStartScript(effect, BilibinRippleScript);
	effect->motionFlags = 0;
	effect->layer = 1;
	effect->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
	if(sprite == NULL)
		return;

	fdSpriteStartAnimation(sprite, 2);
	sprite->flags = 0;
	sprite->image.objectMode = OBJ_MODE_SEMI_TRANSPARENT;
	sprite->image.priority = 3;
}

static void scBilibinUpdateTreasuryStatueMetatiles(void)
{
	ACTOR *statue;

	statue = fdActorGet(ACTOR_TREASURY_STATUE);
	if(statue == NULL)
		return;

	if(FIELD_METATILE(statue->z.fixed) == 6)
		fdMapGridCopyMetatileAttributes(
			TREASURY_WALL_METATILE_COLUMN, SPARE_METATILE_ROW, 1, 1, 14, 6);
	else
		fdMapGridCopyMetatileAttributes(
			TREASURY_ALCOVE_METATILE_COLUMN, SPARE_METATILE_ROW, 1, 1, 14, 6);

	if(FIELD_METATILE(statue->z.fixed) == 9)
		fdMapGridCopyMetatileAttributes(
			TREASURY_WALL_METATILE_COLUMN, SPARE_METATILE_ROW, 1, 1, 14, 9);
	else
		fdMapGridCopyMetatileAttributes(
			TREASURY_FOOT_METATILE_COLUMN, SPARE_METATILE_ROW, 1, 1, 14, 9);
}

Sint32 BilibinFaceLeaderScript[] = {
	SC_CALL(scTurnTowardTarget),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

/* Unused: the entrances for a scene other than the three, which never loads
   this overlay. Its one row has no SCENE_TABLE_END after it. */
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
Sint16 BilibinTreasuryRegions[] = {
	66, 26, 128, 78, 38, 140, CONDITION_ALWAYS, BILIBIN_TREASURY_UPPER_TO_LOWER,
	290, 26, 224, 302, 38, 236, CONDITION_ALWAYS, BILIBIN_TREASURY_TO_BILIBIN_NORTH,
	-1, 0, 0, 0, 0, 0, 0, 0,
};

Uint32 BilibinExits[] = {
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

CUE BilibinCuesOther[] = {
	{ CUE_TABLE_END },
};

CUE BilibinCues[] = {
	{ CUE_EXIT, 1, CONDITION_ALWAYS, BILIBIN_TO_WORLD_MAP },
	{ CUE_EXIT, 2, CONDITION_ALWAYS, BILIBIN_TO_MCCOY_PALACE_GROUNDS },
	{ CUE_EXIT, 10, CONDITION_ALWAYS, BILIBIN_TO_BILIBIN_TREASURY_WEST },
	{ CUE_EXIT, 11, CONDITION_ALWAYS, BILIBIN_TO_BILIBIN_TREASURY_EAST },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), BILIBIN_TO_INN, CONDITION_ALWAYS, SCRIPT_REF(scBilibinOpenDoorAndLeave) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), BILIBIN_TO_HOUSE_EAST, CONDITION_ALWAYS, SCRIPT_REF(scBilibinOpenDoorAndLeave) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), BILIBIN_TO_HOUSE_WEST, CONDITION_ALWAYS, SCRIPT_REF(scBilibinOpenDoorAndLeave) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), BILIBIN_TO_HOUSE_NORTH, CONDITION_ALWAYS, SCRIPT_REF(scBilibinOpenDoorAndLeave) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), BILIBIN_TO_ITEM_SHOP, CONDITION_ALWAYS, SCRIPT_REF(scBilibinOpenDoorAndLeave) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), BILIBIN_TO_WEAPON_AND_ARMOR_SHOP, CONDITION_ALWAYS, SCRIPT_REF(scBilibinOpenDoorAndLeave) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), BILIBIN_TO_SANCTUM, CONDITION_ALWAYS, SCRIPT_REF(scBilibinOpenDoorAndLeave) },
	{ CUE_TALK, ACTOR_TREE_MAN, CONDITION_ALWAYS, SCRIPT_REF(scBilibinExamineTreeMan) },
	{ CUE_TALK, ACTOR_FORMER_TREE_MAN, CONDITION_ALWAYS, SCRIPT_REF(scBilibinAskAboutBeingATree) },
	{ CUE_TALK, ACTOR_YOUNG_MAN_WELCOME, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinWelcomeToBilibinPleaseJustIgnore) },
	{ CUE_TALK, ACTOR_YOUNG_MAN_WELCOME, CONDITION_ALWAYS, MSGNO(MsgBilibinWelcomeToBilibinWellForgetThat) },
	{ CUE_TALK, ACTOR_VILLAGER_GROUP_TO_IMIL, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinIHearALargeGroupCrossed) },
	{ CUE_TALK, ACTOR_VILLAGER_GROUP_TO_IMIL, CONDITION_ALWAYS, MSGNO(MsgBilibinIfYoureTalkingAboutTheTravelers) },
	{ CUE_TALK, ACTOR_VILLAGER_ASKS_ABOUT_TREE, FLAG_KOLIMA_CURSE_LIFTED, SCRIPT_REF(scBilibinAskAboutEntranceTree) },
	{ CUE_TALK, ACTOR_VILLAGER_ASKS_ABOUT_TREE, CONDITION_ALWAYS, MSGNO(MsgBilibinYoureWonderingAboutTheTreeNear) },
	{ CUE_TALK, ACTOR_VILLAGER_MCCOY_FRIGHTENED, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinLordMccoyHasBeenQuiteFrightened) },
	{ CUE_TALK, ACTOR_VILLAGER_MCCOY_FRIGHTENED, CONDITION_ALWAYS, MSGNO(MsgBilibinISawLordMccoyTheChief) },
	{ CUE_TALK, ACTOR_OLD_MAN_ASKS_ABOUT_SOUTHEAST, FLAG_KOLIMA_CURSE_LIFTED, SCRIPT_REF(scBilibinAskAboutSoutheastRoute) },
	{ CUE_TALK, ACTOR_OLD_MAN_ASKS_ABOUT_SOUTHEAST, CONDITION_ALWAYS, MSGNO(MsgBilibinIHearTheBarricadeGateThat) },
	{ CUE_TALK, ACTOR_OLD_WOMAN_ROAD_BLOCKED, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinTheRoadToKolimaIsBlocked) },
	{ CUE_TALK, ACTOR_OLD_WOMAN_ROAD_BLOCKED, CONDITION_ALWAYS, MSGNO(MsgBilibinThoseThreeKidsVanishedDuringAll) },
	{ CUE_TALK, ACTOR_BOY_SCARED_OF_ENTRANCE, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinEverSinceThatManTurnedInto) },
	{ CUE_TALK, ACTOR_BOY_SCARED_OF_ENTRANCE, CONDITION_ALWAYS, SCRIPT_REF(scBilibinAskAboutCurseBreakers) },
	{ CUE_TALK, ACTOR_GIRL_RED_SKY, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinIsItTrueThatAMountain) },
	{ CUE_TALK, ACTOR_GIRL_RED_SKY, CONDITION_ALWAYS, MSGNO(MsgBilibinLordMccoysMenSaidTheySaw) },
	{ CUE_TALK, ACTOR_STRONGMAN_IMIL_TOWER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinTheresATowerAtTheTip) },
	{ CUE_TALK, ACTOR_STRONGMAN_IMIL_TOWER, CONDITION_ALWAYS, SCRIPT_REF(scBilibinAskAboutImil) },
	{ CUE_TALK, ACTOR_VILLAGER_STRANGE_CREATURES, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinSomethingChangedInTheEruptionNow) },
	{ CUE_TALK, ACTOR_VILLAGER_STRANGE_CREATURES, CONDITION_ALWAYS, MSGNO(MsgBilibinTheEruptionOnTheOtherSide) },
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
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_VILLAGE_STATUE, CONDITION_ALWAYS, SCRIPT_REF(scBilibinUpdateVillageStatueMetatiles) },
	{ STEP_CUE_WALKING, METATILE_BEHAVIOR_BILIBIN_STATUE_SQUARE, CONDITION_ALWAYS, SCRIPT_REF(scBilibinPushStatueOrCrates) },
	{ PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_IVY, CONDITION_ALWAYS, 0 },
	{ EXAMINE_CUE(EXAMINE_JAR), METATILE_BEHAVIOR_BILIBIN_HERB_JAR, FLAG_BILIBIN_HERB, FIND_ITEM(ITEM_HERB) },
	{ EXAMINE_CUE(EXAMINE_JAR), METATILE_BEHAVIOR_BILIBIN_COINS_JAR, FLAG_BILIBIN_COINS, FIND_COINS(5) },
	{ EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_BILIBIN_WELL, CONDITION_ALWAYS, SCRIPT_REF(scBilibinInspectWell) },
	{ CUE_TABLE_END },
};

CUE BilibinBarricadeCues[] = {
	{ CUE_EXIT, 1, CONDITION_ALWAYS, BILIBIN_BARRICADE_TO_WORLD_MAP_EAST },
	{ CUE_EXIT, 2, CONDITION_ALWAYS, BILIBIN_BARRICADE_TO_WORLD_MAP_WEST },
	{ CUE_TALK, ACTOR_BOTTOM_TREE, CONDITION_ALWAYS, SCRIPT_REF(scBilibinInspectHumanShapedTree) },
	{ CUE_TALK, ACTOR_MIDDLE_TREE, CONDITION_ALWAYS, SCRIPT_REF(scBilibinInspectHumanShapedTree) },
	{ CUE_TALK, ACTOR_JILL_TREE, CONDITION_ALWAYS, SCRIPT_REF(scBilibinInspectHumanShapedTree) },
	{ CUE_TALK, ACTOR_FALLEN_TREE_STAND_IN, CONDITION_ALWAYS, SCRIPT_REF(scBilibinInspectHumanShapedTree) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOTTOM_TREE, IF_FLAG_SET(FLAG_SATUROS_BEATEN_AT_MERCURY), MSGNO(MsgBilibinTheyCrossedTheRiverAfterPushing) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOTTOM_TREE, CONDITION_ALWAYS, MSGNO(MsgBilibinTheyreSoHeavy) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FALLEN_TREE_STAND_IN, FLAG_BILIBIN_JILL_RESCUED, MSGNO(MsgBilibinShellBeSweptDownstreamIfShe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FALLEN_TREE_STAND_IN, CONDITION_ALWAYS, MSGNO(MsgBilibinOhItLooksLikeSheIs) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MIDDLE_TREE, FLAG_BILIBIN_JILL_RESCUED, MSGNO(MsgBilibinShellBeSweptDownstreamIfShe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MIDDLE_TREE, CONDITION_ALWAYS, MSGNO(MsgBilibinOhItLooksLikeSheIs) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_JILL_TREE, FLAG_BILIBIN_JILL_RESCUED, MSGNO(MsgBilibinAhhhhPleaseAnyonePleaseHelp) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_JILL_TREE, CONDITION_ALWAYS, MSGNO(MsgBilibinThankSavedMeFromBeing) },
	{ CUE_TALK, ACTOR_GANG_MEMBER_EAST, FLAG_BILIBIN_JILL_RESCUED, MSGNO(MsgBilibinIHeardWeWereOnceCalled) },
	{ CUE_TALK, ACTOR_GANG_MEMBER_EAST, CONDITION_ALWAYS, MSGNO(MsgBilibinItWasAwfulLuckBeingTurned) },
	{ CUE_TALK, ACTOR_GANG_MEMBER_WEST, FLAG_BILIBIN_JILL_RESCUED, MSGNO(MsgBilibinIfOnlyWeHadntTriedGetting) },
	{ CUE_TALK, ACTOR_GANG_MEMBER_WEST, CONDITION_ALWAYS, MSGNO(MsgBilibinIReallyLikeThisSpotBy) },
	{ CUE_TALK, ACTOR_JILL, CONDITION_ALWAYS, MSGNO(MsgBilibinIWillThinkOfYouWhenever) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GANG_MEMBER_EAST, FLAG_BILIBIN_JILL_RESCUED, MSGNO(MsgBilibinWeWorkedSoHardIfOnly) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GANG_MEMBER_EAST, CONDITION_ALWAYS, MSGNO(MsgBilibinWhoseIdeaWasItToPick) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GANG_MEMBER_WEST, FLAG_BILIBIN_JILL_RESCUED, MSGNO(MsgBilibinWhatACruelWorldWeJust) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GANG_MEMBER_WEST, CONDITION_ALWAYS, MSGNO(MsgBilibinWellItWasntMyIdeaTo) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_JILL, CONDITION_ALWAYS, MSGNO(MsgBilibinThesePersimmonsAreJustLikeMy) },
	{ EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_BILIBIN_BARRICADE_SIGN, CONDITION_ALWAYS, SCRIPT_REF(scBilibinReadOffLimitsSign) },
	{ EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_BILIBIN_BARRICADE_DOOR, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgFieldDoorTightlyLocked)) },
	{ CUE_STEP, METATILE_BEHAVIOR_BILIBIN_BARRICADE_BESIDE_GANG, FLAG_BILIBIN_JILL_GIFT_RECEIVED, SCRIPT_REF(scBilibinJillGivesHardNut) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_JILL_TREE, FLAG_BILIBIN_JILL_RESCUED, SCRIPT_REF(scBilibinPullJillFromRiver) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_BILIBIN_BARRICADE_HOP_NORTH, CONDITION_ALWAYS, SCRIPT_REF(scBilibinHopAcross) },
	{ STEP_CUE_WALKING_INTO(FACING_WEST), METATILE_BEHAVIOR_BILIBIN_BARRICADE_BESIDE_CRATES, CONDITION_ALWAYS, SCRIPT_REF(scBilibinHopAcross) },
	{ STEP_CUE_WALKING_INTO(FACING_SOUTH), METATILE_BEHAVIOR_BILIBIN_BARRICADE_BESIDE_CRATES, CONDITION_ALWAYS, SCRIPT_REF(scBilibinPushStatueOrCrates) },
	{ STEP_CUE_WALKING, METATILE_BEHAVIOR_BILIBIN_BARRICADE_CRATES_PUSH, CONDITION_ALWAYS, SCRIPT_REF(scBilibinPushStatueOrCrates) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_BARRICADE_CRATES, CONDITION_ALWAYS, SCRIPT_REF(scBilibinUpdateCratesMetatiles) },
	{ CUE_TABLE_END },
};

CUE BilibinTreasuryCues[] = {
	{ CUE_EXIT, 1, CONDITION_ALWAYS, BILIBIN_TREASURY_TO_BILIBIN_SOUTH },
	{ CUE_EXIT, 2, CONDITION_ALWAYS, BILIBIN_TREASURY_LOWER_TO_UPPER },
	{ EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_BILIBIN_TREASURY_SIGN, CONDITION_ALWAYS, SCRIPT_REF(scBilibinReadWarehouseSign) },
	{ CUE_TALK, ACTOR_GUST, CONDITION_ALWAYS, SCRIPT_REF(scBilibinGustJoins) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_TREASURY_STATUE, CONDITION_ALWAYS, SCRIPT_REF(scBilibinUpdateTreasuryStatueMetatiles) },
	{ STEP_CUE_WALKING_INTO(FACING_SOUTH), METATILE_BEHAVIOR_BILIBIN_TREASURY_ALCOVE, CONDITION_ALWAYS, SCRIPT_REF(scBilibinPushStatueOrCrates) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_BILIBIN_TREASURY_WALKWAY_FOOT, CONDITION_ALWAYS, SCRIPT_REF(scBilibinPushStatueOrCrates) },
	{ STEP_CUE_WALKING, METATILE_BEHAVIOR_BILIBIN_TREASURY_WALKWAY, CONDITION_ALWAYS, SCRIPT_REF(scBilibinPushStatueOrCrates) },
	{ STEP_CUE_WALKING_INTO(FACING_EAST), METATILE_BEHAVIOR_BILIBIN_TREASURY_ALCOVE, CONDITION_ALWAYS, SCRIPT_REF(scBilibinHopAcross) },
	{ STEP_CUE_WALKING_INTO(FACING_WEST), METATILE_BEHAVIOR_BILIBIN_TREASURY_ALCOVE, CONDITION_ALWAYS, SCRIPT_REF(scBilibinHopAcross) },
	{ STEP_CUE_WALKING_INTO(FACING_SOUTH), METATILE_BEHAVIOR_BILIBIN_TREASURY_WALKWAY_FOOT, CONDITION_ALWAYS, SCRIPT_REF(scBilibinHopAcross) },
	{ CUE_TABLE_END },
};

/* Each door's opening, as map grid animations, and the doors in turn. */
Uint16 BilibinSanctumDoorSteps[] = {
	MAPGRID_ANIMATION_STEP(37, 33, 4, 2, 5), MAPGRID_ANIMATION_STEP(33, 33, 4, 2, 5), MAPGRID_ANIMATION_STEPS_END,
};
Uint16 BilibinItemShopDoorSteps[] = {
	MAPGRID_ANIMATION_STEP(35, 43, 2, 2, 5), MAPGRID_ANIMATION_STEP(33, 43, 2, 2, 5), MAPGRID_ANIMATION_STEPS_END,
};
Uint16 BilibinWeaponAndArmorShopDoorSteps[] = {
	MAPGRID_ANIMATION_STEP(35, 52, 2, 2, 5), MAPGRID_ANIMATION_STEP(33, 52, 2, 2, 5), MAPGRID_ANIMATION_STEPS_END,
};
Uint16 BilibinHouseEastDoorSteps[] = {
	MAPGRID_ANIMATION_STEP(35, 54, 2, 2, 5), MAPGRID_ANIMATION_STEP(33, 54, 2, 2, 5), MAPGRID_ANIMATION_STEPS_END,
};
Uint16 BilibinInnDoorSteps[] = {
	MAPGRID_ANIMATION_STEP(35, 48, 2, 2, 5), MAPGRID_ANIMATION_STEP(33, 48, 2, 2, 5), MAPGRID_ANIMATION_STEPS_END,
};
Uint16 BilibinHouseWestDoorSteps[] = {
	MAPGRID_ANIMATION_STEP(35, 46, 2, 2, 5), MAPGRID_ANIMATION_STEP(33, 46, 2, 2, 5), MAPGRID_ANIMATION_STEPS_END,
};
Uint16 BilibinHouseNorthDoorSteps[] = {
	MAPGRID_ANIMATION_STEP(35, 50, 2, 2, 5), MAPGRID_ANIMATION_STEP(33, 50, 2, 2, 5), MAPGRID_ANIMATION_STEPS_END,
};
Uint16 *BilibinDoorSteps[] = {
	BilibinInnDoorSteps,
	BilibinHouseEastDoorSteps,
	BilibinHouseWestDoorSteps,
	BilibinHouseNorthDoorSteps,
	BilibinItemShopDoorSteps,
	BilibinWeaponAndArmorShopDoorSteps,
	BilibinSanctumDoorSteps,
};

/* Where each door's opening is drawn on the map grid, in the order of */
/* BilibinDoorSteps. The doors are drawn on map layer 1, whose metatiles */
/* begin at the map's column 32: the inn's (54, 20) is that layer's (22, 20), */
/* the door just above the ground metatile (23, 22) Isaac walks into (render */
/* probe Bilibin --entrance 3 --mark-metatiles 22,20,23,21,1). */
Sint16 BilibinDoorMetatiles[][2] = {
	{54, 20}, {43, 18}, {37, 19}, {41, 11}, {35, 11}, {49, 9}, {55, 6},
};

/* The ten metatiles round the crates' place at the Bilibin Barricade that */
/* push them, as column and row. */
Uint8 BilibinCratesPushMetatiles[] = {
	8,  18, 9,  18, 10, 18, 8,  19, 9,  19,
	10, 19, 11, 19, 9,  20, 10, 20, 11, 20,
};
/* clang-format on */

#include "shared/pushed_actor_direction_steps.c"

Sint32 BilibinRippleScript[] = {
	SC_WAIT(30),
	SC_RELEASE,
};
