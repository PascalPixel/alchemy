/* @file */
/* The Spring of Lemuria, up in the middle of the town past the man who offers
 */
/* the test of luck. The party drinks from it to restore its HP and PP, and */
/* from the step between its two signs tosses coins or Lucky Medals into it, */
/* past the turtles and crabs that swim there, as in Tolbi's spring game: what
 */
/* lands in its rings wins coins or items, and the first medal in the middle */
/* wins the summon Eclipse. */

#include "item_ids.h"
#include "game/game.h"
#include "macros.h"
#include "field/event_inline.h"
#include "field/field_sequence_event.h"
#include "scene/scene.h"
#include "window/window.h"
#include "version.h"
#include "field/call.h"
#include "field/field_sequence.h"
#include "text/msg_ids.h"

/* The spring's actors, as The Broken Seal numbers Tolbi's spring, by the */
/* rows of ActorPlacements. */
/* */
/* The toss draws the coin or the medal with its shadow and three trailing */
/* copies, which LEMURIA_SPRING_COIN_TRAIL and LEMURIA_SPRING_MEDAL_TRAIL */
/* number. */
typedef enum LEMURIA_SPRING_ACTOR_ID {
	ACTOR_LEMURIA_SPRING_COIN_SHADOW = ACTOR_FIRST_PLACED + 3,
	/* The coin the party tosses (SPRITE_SPRING_COIN). */
	ACTOR_LEMURIA_SPRING_COIN,
	ACTOR_LEMURIA_SPRING_MEDAL_SHADOW = ACTOR_LEMURIA_SPRING_COIN + 4,
	/* The Lucky Medal the party tosses (SPRITE_SPRING_LUCKY_MEDAL). */
	ACTOR_LEMURIA_SPRING_MEDAL,
	/* The four obstacles that move in the spring during a toss: two crabs, */
	/* then two turtles. */
	ACTOR_LEMURIA_SPRING_CRAB_A,
	ACTOR_LEMURIA_SPRING_CRAB_B,
	ACTOR_LEMURIA_SPRING_TURTLE_A,
	ACTOR_LEMURIA_SPRING_TURTLE_B,
	ACTOR_LEMURIA_SPRING_MEDAL_SIGN, /* east of the spring */
	ACTOR_LEMURIA_SPRING_COIN_SIGN,	 /* west of the spring */
	/* The grey slabs at the foot of each sign: the signs' sprite in its second
	 */
	/* picture, drawn underfoot (scInit). */
	ACTOR_LEMURIA_SPRING_EAST_SIGN_SLAB,
	ACTOR_LEMURIA_SPRING_WEST_SIGN_SLAB,
	/* "If you face away from the spring, toss in a coin..." */
	ACTOR_LEMURIA_SPRING_VISITOR_FACING_AWAY,
	/* Unused: a man carrying a pack, placed at the map's corner, where no */
	/* trigger answers him; Tolbi's spring places him to ask for a medal. */
	ACTOR_LEMURIA_SPRING_MAN_WITH_PACK,
	/* "I remember an outsider who came..." */
	ACTOR_LEMURIA_SPRING_VISITOR_REMEMBERING_OUTSIDER,
	/* The stone tablet that brings Eclipse to the leader the first time a */
	/* medal lands in the middle of the target (scLemuriaSpringGivePrize). */
	ACTOR_LEMURIA_SPRING_SUMMON_TABLET
} LEMURIA_SPRING_ACTOR_ID;

/* The actor that draws toss->trail[n] of a tossed coin or medal, n from 1. */
#define LEMURIA_SPRING_COIN_TRAIL(n)  (ACTOR_LEMURIA_SPRING_COIN_SHADOW - (n))
#define LEMURIA_SPRING_MEDAL_TRAIL(n) (ACTOR_LEMURIA_SPRING_MEDAL_SHADOW - (n))

/* What the party tosses, as the menu lists them. */
typedef enum SPRING_TOSS_KIND {
	SPRING_TOSS_COIN,
	SPRING_TOSS_MEDAL
} SPRING_TOSS_KIND;

/* Animations of the spring's sprites, as The Broken Seal names Tolbi's. The */
/* coin's and the Lucky Medal's sprite sheets (spring_coin.png, */
/* spring_lucky_medal.png) are drawn alike. */
typedef enum LEMURIA_SPRING_ANIMATION {
	ANIM_TOKEN_TUMBLE = 0, /* turning over once as it is tossed */
	ANIM_TOKEN_FLOAT = 1,  /* turning on the water as it drifts */
	ANIM_TOKEN_REST = 2,   /* lying still */
	ANIM_TOKEN_SHADOW = 3, /* a grey spot on the ground below it */
	/* A twinkle, as each of the three marks trailing a flying token. */
	ANIM_TOKEN_GLINT = 4,
	ANIM_CRAB_SCUTTLE = 1,	   /* stepping sideways (crab.png) */
	ANIM_CRAB_RAISE_CLAWS = 3, /* snapping its raised claws (crab.png) */
	ANIM_TURTLE_SWIM = 2,	   /* paddling (turtle.png) */
	ANIM_TURTLE_HIDE = 3,	   /* head and flippers drawn in (turtle.png) */
	/* The signpost sprite sheet's second picture, a grey slab (signpost.png).
	 */
	ANIM_SIGN_SLAB = 2
} LEMURIA_SPRING_ANIMATION;

/* What searching answers at Lemuria's spring. */
typedef enum LEMURIA_SPRING_CELL_BEHAVIOR {
	/* The spring's rim: the party drinks from it while its target is hidden. */
	CELL_BEHAVIOR_LEMURIA_SPRING_RIM = 25,
	/* The water before the spring's steps: searched to toss a coin or medal. */
	CELL_BEHAVIOR_LEMURIA_SPRING_WATER = 80
} LEMURIA_SPRING_CELL_BEHAVIOR;

/* Set once the spring's water has faded to show the target at its bottom, */
/* for the rest of the visit. */
#define FLAG_LEMURIA_SPRING_TARGET_SHOWN FLAG_SCENE_TEMP_00

/* The middle of the target, which the target pulls the tossed token toward. */
#define LEMURIA_SPRING_CENTER_X toFIXED(120)
#define LEMURIA_SPRING_CENTER_Z toFIXED(71)

/* Where a tossed token comes to rest: the target's four rings, the middle */
/* first, then none. */
typedef enum SPRING_TOSS_RESULT {
	TOSS_RING_MIDDLE,
	TOSS_RING_SECOND,
	TOSS_RING_THIRD,
	TOSS_RING_OUTER,
	TOSS_RING_MISSED
} SPRING_TOSS_RESULT;

/* How much of its speed a token on the water keeps each frame, in 256ths: */
/* while the target pulls it, and once it drifts free. */
#define SPEED_KEPT_SCALE  256
#define PULLED_SPEED_KEPT 253
#define DRIFT_SPEED_KEPT  220

/* The crabs and turtles in the spring, by their slots in */
/* LemuriaSpring_Obstacles. */
typedef enum LEMURIA_SPRING_OBSTACLE {
	OBSTACLE_CRAB_A,
	OBSTACLE_CRAB_B,
	OBSTACLE_TURTLE_A,
	OBSTACLE_TURTLE_B,
	OBSTACLE_COUNT
} LEMURIA_SPRING_OBSTACLE;

/* Which way a crab scuttles (its SPRING_OBSTACLE.heading). */
enum { CRAB_HEADING_EAST, CRAB_HEADING_WEST };

/* The frames the second crab waits at each end of its way. */
#define CRAB_TURN_PAUSE_FRAMES 30
/* A turtle's slowest swim round its loop, a 1024th of a turn a frame. */
#define TURTLE_SWIM_TURN (TRIG_TURN / 1024)

/* scLemuriaSpringGivePrize's item for the summon Eclipse. */
#define PRIZE_ECLIPSE (-1)

/* The items a Lucky Medal can win, and how many more of them each ring out */
/* from the middle allows. */
#define MEDAL_PRIZE_COUNT	  15
#define MEDAL_PRIZES_PER_RING 3

/* How far the water sinks before it fades. */
#define WATER_SHIFT_PIXELS 16

s32 scLemuriaSpringRunSpringGame(void);

extern u16 LemuriaSpring_CoinPrizes[];
extern u32 LemuriaSpring_MedalPrizes[MEDAL_PRIZE_COUNT];

s32 scLemuriaSpringRunToss();

void scLemuriaSpringGivePrize(s32 item);

extern u8 LemuriaSpring_CrabColorModes[];
extern u8 LemuriaSpring_TurtleColorModes[];
extern u8 LemuriaSpring_ObstacleStartX[];
extern u8 LemuriaSpring_ObstacleStartZ[];

extern u16 LemuriaSpring_ObstacleHeadings[];

void scLemuriaSpringSetSprStorageKind(s32 actor, s32 active);

SPRING_TOSS_MOTION LemuriaSpring_Toss BSS_DATA ALIGNED(16);
s32 LemuriaSpring_TossKind BSS_DATA;
/* The crabs and turtles in the toss's way, as the toss moves them. */
SPRING_OBSTACLE LemuriaSpring_Obstacles[OBSTACLE_COUNT] BSS_DATA ALIGNED(16);

s32 LemuriaSpring_TossFrame BSS_DATA;
s32 LemuriaSpring_TossEnded BSS_DATA;
s32 LemuriaSpring_TossResult BSS_DATA;

const SCENE_ENTRANCE *scGetEntrances(void) { return SceneEntrances; }

const SCENE_REGION *scGetExitRegions(void) { return 0; }

const u32 *scGetExits(void) { return SceneExits; }

const ACTOR_PLACE *scGetActorPlacements(void) { return ActorPlacements; }

/* Felix checks the rim of the spring, any of its edges round the pool, before
 */
/* the target has been shown this visit: "Felix and the others drank of the */
/* spring. Their HP and PP were fully replenished!" */
void scLemuriaSpringDrinkFromSpring(void) {
	/* "Felix and the others drank of the spring. Their HP and PP were fully..."
	 */
	mnMessageShowCentered((s32)MsgAndTheOthers, 1);
	fdEventPlaySnd(SE_RECOVERY);
	gmPartyRestoreHpPp(1);
}

extern u16 LemuriaSpring_AlphaSteps[];

/* The spring's water, the overhead layer, sinks 16 pixels, a pixel every */
/* four frames, then fades away in eight steps to show the target at the */
/* bottom of the spring, as Tolbi's does. */
void scLemuriaSpringDrainSpring(void) {
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	MAP_LAYER *layer;
	s32 i;

	fdEventPlaySnd(SE_BURST);
	layer = &map->layers[MAPGRID_LAYER_OVERHEAD];
	for(i = 0; i < WATER_SHIFT_PIXELS; i++) {
		layer->ofsY -= toFIXED(1);
		sySynch(4);
	}
	for(i = 0; i < 8; i++) {
		syIoWrite16(BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL,
					&REG_BLDCNT);
		syIoWrite16(LemuriaSpring_AlphaSteps[i], &REG_BLDALPHA);
		sySynch(8);
	}
}

/* Felix checks the water from the step at the south of the spring, between the
 */
/* two signs, cell (7, 8) or (7, 10): he turns his back to the spring to toss a
 */
/* coin or a Lucky Medal into it. The first time each visit, the */
/* spring's water first fades to show the target of rings at its bottom. */
void scLemuriaSpringTossCoinOrMedal(void) {
	s32 actor = MainRoutineStateWork.leaderActorId;

	if(!fdEventGetFlag(FLAG_LEMURIA_SPRING_TARGET_SHOWN)) {
		fdEventSetFlag(FLAG_LEMURIA_SPRING_TARGET_SHOWN);
		scLemuriaSpringDrainSpring();
	}
	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	fdActorWalkToAndWait(actor, 120, 152);
	fdActorFaceDirection(actor, FACING_SOUTH, 0);
	scLemuriaSpringRunSpringGame();
	fdEventEnd();
}

const TRIGGER *scGetTriggers(void) { return SceneEvents; }

void scLemuriaSpringInitTossSimulation(void);

/* Starts the scene: the water blended over the target, the slabs at the */
/* signs' feet drawn underfoot; arriving from Lemuria, the toss is set up, and
 */
/* the water stays faded once the target has been shown. */
s32 scInit(void) {
	ACTOR *leader;
	s32 blend;

	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	leader->priFlg = ACTOR_PRIORITY_IGNORES_RISES | leader->priFlg;
	blend = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
	REG_BLDCNT = blend;
	syIoWrite16(BLEND_ALPHA(12, 8), &REG_BLDALPHA);
	fdActorSetAnimation(ACTOR_LEMURIA_SPRING_EAST_SIGN_SLAB, ANIM_SIGN_SLAB);
	fdActorSetAnimation(ACTOR_LEMURIA_SPRING_WEST_SIGN_SLAB, ANIM_SIGN_SLAB);
	fdActorGet(ACTOR_LEMURIA_SPRING_EAST_SIGN_SLAB)->priFlg =
		ACTOR_PRIORITY_UNDERFOOT;
	fdActorGet(ACTOR_LEMURIA_SPRING_WEST_SIGN_SLAB)->priFlg =
		ACTOR_PRIORITY_UNDERFOOT;
	fdEventStartScrTransition();
	if(MainRoutineStateWork.entrance == LEMURIA_SPRING_FROM_LEMURIA) {
		scLemuriaSpringInitTossSimulation();
		if(fdEventGetFlag(FLAG_LEMURIA_SPRING_TARGET_SHOWN)) {
			syIoWrite16(blend, &REG_BLDCNT);
			syIoWrite16(BLEND_ALPHA(0, BLEND_WEIGHT_MAX), &REG_BLDALPHA);
		}
	}
	return 0;
}

s32 scPrepareMap(void) { return 0; }

/* The Japanese catalog does not name these yet. */
extern u8 MsgLemuriaSpringWhatWillYouThrow[], MsgLemuriaSpringCoins[],
	MsgLemuriaSpringToss[], MsgLemuriaSpringYouWonCoin[];

/* French draws the coin window two tiles wider, its numbers 16 pixels further
 */
/* right; Japanese draws them 16 pixels further left, each followed by its */
/* counter word. */
#if LANGUAGE == LANGUAGE_FRENCH
#define COIN_WINDOW_WIDTH 19
#define COIN_NUMBER_X	  88
#elif LANGUAGE == LANGUAGE_JAPANESE
#define COIN_WINDOW_WIDTH 17
#define COIN_NUMBER_X	  56
#else
#define COIN_WINDOW_WIDTH 17
#define COIN_NUMBER_X	  72
#endif

/* The toss prompt's window: two tiles narrower in Japanese, against the right
 */
/* edge. */
#if LANGUAGE == LANGUAGE_JAPANESE
#define TOSS_WINDOW_X	  22
#define TOSS_WINDOW_WIDTH 7
#else
#define TOSS_WINDOW_X	  20
#define TOSS_WINDOW_WIDTH 9
#endif

/* The weight of prize i in the draw of what a toss wins. */
#define WEIGHT(i) ((s8)MainRoutineStateWork.prizeWeights[i])

/* The toss: the party picks a coin or a Lucky Medal, throws it, and what */
/* lands in the spring's rings wins its prize, as in Tolbi's spring game. */
s32 scLemuriaSpringRunSpringGame(void) {
	s32 choice;
#if !TLA_PATCH_1
	s32 text;
#endif
	s32 message;
	s32 coins;
	s32 medals;
	WINDOW *window;
	s32 result;
	s32 prizeRoll;
	s32 total;
	s32 prize;
	s32 cancelled = -1; /* the toss's result when B puts it off */

#if TLA_PATCH_1
	fdEventStart();
	choice = 0;
	fdEventEndLastingPsynergy(0);
	for(;;) {
		coins = MainRoutineStateWork.coins;
		medals = gmPartyInventoryCountItem(ITEM_LUCKY_MEDAL);
		message = (s32)MsgLemuriaSpringWhatWillYouThrow;
		fdEventSetMessageId(message);
		/* "What will you throw?" */
		fdEventOpenMessage(-1, 0);
		window = mnWindowCreate(0, 0, COIN_WINDOW_WIDTH, 4, 2);
		result = (s32)MsgLemuriaSpringCoins;
		mnTextDrawMessageAsTiles(result, window, 0, 0);
		mnTextDrawNumInWindow(coins, 6, window, COIN_NUMBER_X, 0);
		mnTextDrawMessageAsTiles(result + 1, window, 0, 8);
		mnTextDrawNumInWindow(medals, 6, window, COIN_NUMBER_X, 8);
		choice = mnSelectCoinsOrMedals(choice);
		mnWindowClose(window, 2);
		mnMessageCloseFinishedWindows();
		if(choice == -1) {
			fdEventEnd();
			return 0;
		}
		if(choice == SPRING_TOSS_COIN) {
			if(coins == 0) {
				fdEventSetMessageId(message + 1);
				/* "You don't have any coins!" */
				fdEventShowMessage(-1, 0);
				sySynch(1);
				continue;
			}
		}
		else if(choice == SPRING_TOSS_MEDAL) {
			if(medals == 0) {
				fdEventSetMessageId(message + 2);
				/* "You have no Lucky Medals!" */
				fdEventShowMessage(-1, 0);
				sySynch(1);
				continue;
			}
			if(gmPartyInventoryCountFreeSlots() == 0) {
				fdEventSetMessageId(message + 4);
				/* "You can't carry more items. Do you still want to play?" */
				fdEventOpenMessage(-1, 0);
				if(fdEventAskYesNoWithActorAnimation(0, 0) != 0)
					goto end;
			}
		}
		window = mnWindowCreate(TOSS_WINDOW_X, 15, TOSS_WINDOW_WIDTH, 4, 2);
		result = (s32)MsgLemuriaSpringToss;
		mnTextDrawMessageAsTiles(result, window, 0, 0);
		mnTextDrawMessageAsTiles(result + 1, window, 0, 8);
		sySynch(5);
		fdEventPlaySnd(SE_TOLBI_SPRING_TOSS_PROMPT);
		for(;;) {
			if(InputWork.pressed & KEY_A) {
				fdEventPlaySnd(SE_MENU_CONFIRM);
				result = 0;
				break;
			}
			if(InputWork.pressed & KEY_B) {
				fdEventPlaySnd(SE_MENU_CANCEL);
				result = cancelled;
				break;
			}
			sySynch(1);
		}
		mnWindowClose(window, 2);
		break;
	}
#else
	/* The Japanese release keeps The Broken Seal's Japanese layout: a counter
	 */
	/* word after each count, and the coin or medal taken as soon as it is */
	/* chosen, so cancelling the toss loses it. From patch 1 it is taken once
	 * the */
	/* toss is confirmed. */
	choice = 0;
	fdEventStart();
	fdEventEndLastingPsynergy(0);
	for(;;) {
		coins = MainRoutineStateWork.coins;
		medals = gmPartyInventoryCountItem(ITEM_LUCKY_MEDAL);
		message = (s32)MsgLemuriaSpringWhatWillYouThrow;
		fdEventSetMessageId(message);
		/* "What will you throw?" */
		fdEventOpenMessage(-1, 0);
		window = mnWindowCreate(0, 0, COIN_WINDOW_WIDTH, 4, 2);
		text = (s32)MsgLemuriaSpringCoins;
		mnTextDrawMessageAsTiles(text, window, 0, 0);
		mnTextDrawNumInWindow(coins, 6, window, COIN_NUMBER_X, 0);
		mnTextDrawMessageAsTiles(text + 2, window, 104, 0);
		mnTextDrawMessageAsTiles(text + 1, window, 0, 8);
		mnTextDrawNumInWindow(medals, 6, window, COIN_NUMBER_X, 8);
		mnTextDrawMessageAsTiles(text + 2, window, 104, 8);
		choice = mnSelectCoinsOrMedals(choice);
		mnWindowClose(window, 2);
		mnMessageCloseFinishedWindows();
		if(choice == -1) {
			fdEventEnd();
			return 0;
		}
		if(choice == SPRING_TOSS_COIN) {
			if(coins == 0) {
				fdEventSetMessageId(message + 1);
				/* "You don't have any coins!" */
				fdEventShowMessage(-1, 0);
				sySynch(1);
				continue;
			}
			gmPartyAdjustCoins(-1);
		}
		else if(choice == SPRING_TOSS_MEDAL) {
			if(medals == 0) {
				fdEventSetMessageId(message + 2);
				/* "You have no Lucky Medals!" */
				fdEventShowMessage(-1, 0);
				sySynch(1);
				continue;
			}
			gmPartyInventoryRemove(ITEM_LUCKY_MEDAL);
			if(gmPartyInventoryCountFreeSlots() == 0) {
				fdEventSetMessageId(message + 4);
				/* "You can't carry more items. Do you still want to play?" */
				fdEventOpenMessage(-1, 0);
				if(fdEventAskYesNoWithActorAnimation(0, 0) != 0)
					goto end;
			}
		}
		window = mnWindowCreate(TOSS_WINDOW_X, 15, TOSS_WINDOW_WIDTH, 4, 2);
		result = (s32)MsgLemuriaSpringToss;
		mnTextDrawMessageAsTiles(result, window, 0, 0);
		mnTextDrawMessageAsTiles(result + 1, window, 0, 8);
		sySynch(5);
		fdEventPlaySnd(SE_TOLBI_SPRING_TOSS_PROMPT);
		for(;;) {
			if(InputWork.pressed & KEY_A) {
				fdEventPlaySnd(SE_MENU_CONFIRM);
				result = 0;
				break;
			}
			if(InputWork.pressed & KEY_B) {
				fdEventPlaySnd(SE_MENU_CANCEL);
				result = cancelled;
				break;
			}
			sySynch(1);
		}
		mnWindowClose(window, 2);
		break;
	}
#endif
	if(result != cancelled) {
#if TLA_PATCH_1
		if(choice == SPRING_TOSS_COIN) {
			gmPartyAdjustCoins(-1);
		}
		else if(choice == SPRING_TOSS_MEDAL) {
			gmPartyInventoryRemove(ITEM_LUCKY_MEDAL);
		}
#endif
		result = scLemuriaSpringRunToss(choice);
		if(choice == SPRING_TOSS_COIN) {
			if(result != TOSS_RING_MISSED) {
				gmPartyAdjustCoins(LemuriaSpring_CoinPrizes[result]);
				fdEventPlaySnd(MUS_LUCKY);
				mnMessageArgumentPush(LemuriaSpring_CoinPrizes[result],
									  MESSAGE_ARGUMENT_NUMBER);
				fdEventSetMessageId((s32)MsgLemuriaSpringYouWonCoin);
				/* "You won ... coin!" */
				fdEventShowMessage(-1, 0);
			}
			else {
				fdEventPlaySnd(SE_MENU_CANCEL);
				syFieldEventWait(10);
			}
		}
		else {
			total = 0;
			if(result == TOSS_RING_MIDDLE &&
			   fdEventGetFlag(FLAG_LEMURIA_SPRING_ECLIPSE_WON) == 0) {
				fdEventSetFlag(FLAG_LEMURIA_SPRING_ECLIPSE_WON);
				scLemuriaSpringGivePrize(PRIZE_ECLIPSE);
			}
			else {
				for(prize = 0; prize < result * MEDAL_PRIZES_PER_RING +
										   MEDAL_PRIZES_PER_RING;
					prize++)
					total += WEIGHT(prize);
				prizeRoll = syRandomBelow(total);
				for(prize = 0; prize < MEDAL_PRIZE_COUNT; prize++) {
					prizeRoll -= WEIGHT(prize);
					if(prizeRoll < 0)
						break;
				}
				if(prize == MEDAL_PRIZE_COUNT)
					prize = MEDAL_PRIZE_COUNT - 1;
				scLemuriaSpringGivePrize(LemuriaSpring_MedalPrizes[prize]);
				if(WEIGHT(prize) > 1)
					MainRoutineStateWork.prizeWeights[prize] =
						WEIGHT(prize) / 2;
			}
		}
	}
end:
	fdEventEnd();
	return 0;
}

/* Draws one picture of the prize being brought up in the middle of the */
/* spring, with SE_CAST, and holds it 8 frames. */
static inline void scLemuriaSpringShowPrizePicture(s32 sourceColumn,
												   s32 sourceRow) {
	fdMapGridCopyCellsTo(sourceColumn, sourceRow, 70, 0, 3, 8);
	fdEventPlaySnd(SE_CAST);
	syFieldEventWait(8);
}

/* A medal's prize comes up out of the spring as at Tolbi's: a roar, the */
/* leader turns to the spring, and the prize is brought up to him, an item, */
/* or, for the first medal to land in the middle */
/* (FLAG_LEMURIA_SPRING_ECLIPSE_WON), the summon Eclipse, read from a tablet. */
/* */
/* What rises is the pictures: each is 3 by 8 cells copied over the spring's */
/* middle from spare cells of the map, one every 8 frames, in order as it */
/* comes and back in reverse as it goes. */
void scLemuriaSpringGivePrize(s32 item) {
	ACTOR *prize;

	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	syFieldEventWait(30);
	fdEventPlaySnd(SE_BOSS_ROAR);
	syFieldEventWait(100);
	fdActorFaceDirection(ACTOR_ISAAC, FACING_NORTH, 0);
	syFieldEventWait(40);
	fdMapGridCopyCellsTo(82, 20, 70, 0, 3, 8);
	syFieldEventWait(3);
	scLemuriaSpringShowPrizePicture(85, 20);
	scLemuriaSpringShowPrizePicture(88, 20);
	scLemuriaSpringShowPrizePicture(91, 20);
	scLemuriaSpringShowPrizePicture(94, 20);
	scLemuriaSpringShowPrizePicture(97, 20);
	scLemuriaSpringShowPrizePicture(100, 20);
	scLemuriaSpringShowPrizePicture(79, 29);
	scLemuriaSpringShowPrizePicture(82, 29);
	scLemuriaSpringShowPrizePicture(85, 29);
	scLemuriaSpringShowPrizePicture(88, 29);
	scLemuriaSpringShowPrizePicture(91, 29);
	scLemuriaSpringShowPrizePicture(94, 29);
	scLemuriaSpringShowPrizePicture(97, 29);
	fdMapGridCopyCellsTo(100, 29, 70, 0, 3, 8);
	fdEventPlaySnd(SE_CAST);
	syFieldEventWait(70);
	fdEventPlaySnd(SE_RECOVERY);
	if(item == PRIZE_ECLIPSE) {
		prize = fdActorGet(ACTOR_LEMURIA_SPRING_SUMMON_TABLET);
		fdActorPlaceOnActor(ACTOR_LEMURIA_SPRING_SUMMON_TABLET,
							MainRoutineStateWork.leaderActorId);
		prize->motionFlg = 0;
		prize->y.fixed = toFIXED(32);
		scFieldActorSetAnimation(MainRoutineStateWork.leaderActorId,
								 ANIM_LEADER_ARMS_RAISED);
		fdSummonTabletPlayRunes(ACTOR_LEMURIA_SPRING_SUMMON_TABLET);
		mnSummonShowLearnedMessage(SUMMON_ECLIPSE);
		gmSummonLearn(SUMMON_ECLIPSE);
		fdActorSetPos(ACTOR_LEMURIA_SPRING_SUMMON_TABLET, 0, 0);
		scFieldActorSetAnimation(MainRoutineStateWork.leaderActorId,
								 ANIM_STAND);
	}
	else {
		Call2(fdItemDispShowItem, item,
			  ITEM_DISPLAY_SPARKLES | ITEM_DISPLAY_RAISED);
		gmPartyGiveItem(item, 0);
		syFieldEventWait(20);
	}
	scLemuriaSpringShowPrizePicture(97, 29);
	scLemuriaSpringShowPrizePicture(94, 29);
	scLemuriaSpringShowPrizePicture(91, 29);
	scLemuriaSpringShowPrizePicture(88, 29);
	scLemuriaSpringShowPrizePicture(85, 29);
	scLemuriaSpringShowPrizePicture(82, 29);
	scLemuriaSpringShowPrizePicture(100, 20);
	scLemuriaSpringShowPrizePicture(97, 20);
	scLemuriaSpringShowPrizePicture(94, 20);
	scLemuriaSpringShowPrizePicture(91, 20);
	scLemuriaSpringShowPrizePicture(88, 20);
	scLemuriaSpringShowPrizePicture(85, 20);
	scLemuriaSpringShowPrizePicture(82, 20);
	scLemuriaSpringShowPrizePicture(79, 20);
	fdEventEnd();
}

/* Sets an actor of the toss where the simulation has it: its position, */
/* facing, colour mode and animation step. */
void scLemuriaSpringSetActorAppearance(s32 id, s32 *position, s32 facing,
									   s32 colorMode, s32 step) {
	ACTOR *actor = fdActorGet(id);
	SPR *sprite;
	SPR_ANIMATION **entry;
	s32 i;

	if(actor != NULL) {
		actor->x.fixed = *position++;
		actor->y.fixed = *position++;
		actor->z.fixed = *position;
		actor->facing = facing;
		actor->motionFlg = 0;
		((SPR *)actor->spr)->flg = 0;
		fdActorSetAnimationSpeed(actor, step);
	}
	sprite = actor->spr;
	i = 0;
	if(i < sprite->cnt) {
		entry = sprite->entries;
		for(; i < sprite->cnt; i++) {
			SPR_ANIMATION *animation = *entry++;

			if(animation->colMode != colorMode) {
				animation->colMode = colorMode;
				animation->picture = SPRITE_PICTURE_NONE;
			}
		}
	}
}

/* Sets how actor `id` keeps its sprites (ACTOR.sprStorageKind). */
void scLemuriaSpringSetSprStorageKind(s32 id, s32 kind) {
	ACTOR *actor;

	actor = fdActorGetById(id);
	if(actor != 0) {
		actor->sprStorageKind = kind;
	}
}

/* A task: moves the tossed coin or medal a frame, its trail and shadow */
/* behind it, and the crabs and turtles that swim in the spring. */
void scLemuriaSpringUpdateTossSimulation(void) {
	SPRING_TOSS_MOTION *toss;
	SPRING_OBSTACLE *obstacle;
	s32 index;
	s32 height;
	FIXED edgeInset;
	s32 motionStep;
	s32 speedMode;
	s32 cycleFrame;
	FIXED minX;
	FIXED maxX;
	FIXED minZ;
	FIXED maxZ;
	/* LEFTOVER(frame): an unused local the ROM's frame reserves and never
	 * touches. */
	VECTOR3 spare;

	toss = &LemuriaSpring_Toss;
	index = 3;
	do {
		toss->trail[index].x = toss->trail[index - 1].x;
		toss->trail[index].y = toss->trail[index - 1].y;
		toss->trail[index].z = toss->trail[index - 1].z;
		index--;
	} while(index != 0);

	/* The token leaves the leader's hand 32 frames into the throw. */
	if(toss->launchFrm > 31) {
		toss->trail[0].x += toss->vel.x;
		height = toss->trail[0].y + toss->vel.y;
		toss->trail[0].y = height;
		toss->trail[0].z += toss->vel.z;

		if(height <= 0) {
			toss->trail[0].y = 0;
			if(toss->vel.y != 0) {
				toss->vel.y = 0;
				if(LemuriaSpring_TossKind == SPRING_TOSS_MEDAL) {
					fdActorSelectAnimation(
						fdActorGetById(ACTOR_LEMURIA_SPRING_MEDAL),
						ANIM_TOKEN_FLOAT);
				}
				else {
					fdActorSelectAnimation(
						fdActorGetById(ACTOR_LEMURIA_SPRING_COIN),
						ANIM_TOKEN_FLOAT);
				}
			}

			if(toss->attractionFrm > 0) {
				s32 deltaX;
				s32 deltaZ;
				s32 distance;

				deltaX = LEMURIA_SPRING_CENTER_X - toss->trail[0].x;
				deltaZ = LEMURIA_SPRING_CENTER_Z - toss->trail[0].z;
				deltaX >>= 8;
				deltaZ >>= 8;
				/* BUG: an exact center position leaves a zero divisor below. */
				distance = sySquart(deltaX * deltaX + deltaZ * deltaZ);
				toss->vel.x += (toFIXED(0.1) * deltaX) / distance;
				toss->vel.z += (toFIXED(0.1) * deltaZ) / distance;
				toss->vel.x =
					toss->vel.x * PULLED_SPEED_KEPT / SPEED_KEPT_SCALE;
				toss->vel.z =
					toss->vel.z * PULLED_SPEED_KEPT / SPEED_KEPT_SCALE;
				toss->attractionFrm = toss->attractionFrm - 1;
			}
			else {
				toss->vel.x = toss->vel.x * DRIFT_SPEED_KEPT / SPEED_KEPT_SCALE;
				toss->vel.z = toss->vel.z * DRIFT_SPEED_KEPT / SPEED_KEPT_SCALE;
				if(toss->vel.x > -toFIXED(1) / 64 &&
				   toss->vel.x < toFIXED(1) / 64) {
					toss->vel.x = 0;
				}
				if(toss->vel.z > -toFIXED(1) / 64 &&
				   toss->vel.z < toFIXED(1) / 64) {
					toss->vel.z = 0;
				}
				if(toss->vel.x == 0 && toss->vel.z == 0) {
					if(LemuriaSpring_TossKind == SPRING_TOSS_MEDAL) {
						fdActorSelectAnimation(
							fdActorGetById(ACTOR_LEMURIA_SPRING_MEDAL),
							ANIM_TOKEN_REST);
						scLemuriaSpringSetSprStorageKind(
							LEMURIA_SPRING_MEDAL_TRAIL(1), 0);
						scLemuriaSpringSetSprStorageKind(
							LEMURIA_SPRING_MEDAL_TRAIL(2), 0);
						scLemuriaSpringSetSprStorageKind(
							LEMURIA_SPRING_MEDAL_TRAIL(3), 0);
					}
					else {
						fdActorSelectAnimation(
							fdActorGetById(ACTOR_LEMURIA_SPRING_COIN),
							ANIM_TOKEN_REST);
						scLemuriaSpringSetSprStorageKind(
							LEMURIA_SPRING_COIN_TRAIL(1), 0);
						scLemuriaSpringSetSprStorageKind(
							LEMURIA_SPRING_COIN_TRAIL(2), 0);
						scLemuriaSpringSetSprStorageKind(
							LEMURIA_SPRING_COIN_TRAIL(3), 0);
					}
					{
						s32 deltaX = FIXEDtoINT(LEMURIA_SPRING_CENTER_X -
												toss->trail[0].x);
						s32 deltaZ = FIXEDtoINT(LEMURIA_SPRING_CENTER_Z -
												toss->trail[0].z);
						s32 distanceSquared = deltaX * deltaX + deltaZ * deltaZ;

						/* The target's rings end at these squared distances, in
						 * square */
						/* pixels, as the rings painted in the spring do. */
						LemuriaSpring_TossEnded = 1;
						if(distanceSquared <= 224) {
							LemuriaSpring_TossResult = TOSS_RING_MIDDLE;
						}
						else if(distanceSquared <= 624) {
							LemuriaSpring_TossResult = TOSS_RING_SECOND;
						}
						else if(distanceSquared <= 1088) {
							LemuriaSpring_TossResult = TOSS_RING_THIRD;
						}
						else if(distanceSquared <= 1680) {
							LemuriaSpring_TossResult = TOSS_RING_OUTER;
						}
						else {
							LemuriaSpring_TossResult = TOSS_RING_MISSED;
						}
					}
				}
			}

			/* The basin's walls: 48 to 192 pixels across and 24 to 120 down,
			 * its */
			/* corners cut along slopes of 42 across to 18 down. */
			minX = toFIXED(48);
			maxX = toFIXED(192);
			minZ = toFIXED(24);
			maxZ = toFIXED(120);
			if(toss->trail[0].z < toFIXED(42)) {
				edgeInset = (toFIXED(42) - toss->trail[0].z) * 42 / 18;
				minX += edgeInset;
				if(minX > toFIXED(90)) {
					minX = toFIXED(90);
				}
				maxX -= edgeInset;
				if(maxX < toFIXED(150)) {
					maxX = toFIXED(150);
				}
			}
			if(toss->trail[0].z > toFIXED(102)) {
				edgeInset = (toss->trail[0].z * 42 - toFIXED(102) * 42) / 18;
				minX = toFIXED(48) + edgeInset;
				if(minX > toFIXED(90)) {
					minX = toFIXED(90);
				}
				maxX = toFIXED(192) - edgeInset;
				if(maxX < toFIXED(150)) {
					maxX = toFIXED(150);
				}
			}
			if(toss->trail[0].x < toFIXED(90)) {
				edgeInset = (toFIXED(90) - toss->trail[0].x) * 18 / 42;
				minZ = toFIXED(24) + edgeInset;
				if(minZ > toFIXED(42)) {
					minZ = toFIXED(42);
				}
				maxZ = toFIXED(120) - edgeInset;
				if(maxZ < toFIXED(102)) {
					maxZ = toFIXED(102);
				}
			}
			if(toss->trail[0].x > toFIXED(150)) {
				edgeInset = (toss->trail[0].x * 18 - toFIXED(150) * 18) / 42;
				minZ = toFIXED(24) + edgeInset;
				if(minZ > toFIXED(42)) {
					minZ = toFIXED(42);
				}
				maxZ = toFIXED(120) - edgeInset;
				if(maxZ < toFIXED(102)) {
					maxZ = toFIXED(102);
				}
			}

			/* The token bounces off a wall at half its speed. */
			if(toss->trail[0].x < minX) {
				toss->trail[0].x = minX;
				if(toss->vel.x < 0) {
					toss->vel.x = -toss->vel.x / 2;
				}
			}
			if(toss->trail[0].x > maxX) {
				toss->trail[0].x = maxX;
				if(toss->vel.x > 0) {
					toss->vel.x = -toss->vel.x / 2;
				}
			}
			if(toss->trail[0].z < minZ) {
				toss->trail[0].z = minZ;
				if(toss->vel.z < 0) {
					toss->vel.z = -toss->vel.z / 2;
				}
			}
			if(toss->trail[0].z > maxZ) {
				toss->trail[0].z = maxZ;
				if(toss->vel.z > 0) {
					toss->vel.z = -toss->vel.z / 2;
				}
			}
		}
		else {
			toss->vel.y -= toFIXED(1) / 4;
		}
	}

	/* The two crabs scuttle east and west; the two turtles swim round ellipses,
	 */
	/* the second hiding in its shell for the last quarter of every 512 frames.
	 */
	index = 0;
	do {
		obstacle = &LemuriaSpring_Obstacles[index];

		if(obstacle->reactionFrm > 0) {
			obstacle->reactionFrm = obstacle->reactionFrm - 1;
		}
		if(obstacle->hitCooldownFrm > 0) {
			obstacle->hitCooldownFrm = obstacle->hitCooldownFrm - 1;
		}
		if(index <= OBSTACLE_CRAB_B) {
			speedMode = obstacle->speedMode;
			motionStep = toFIXED(1);
			if(speedMode == 1) {
				motionStep = motionStep * 2;
			}
			if(speedMode == 2) {
				motionStep = motionStep * 3;
			}
			if(obstacle->reactionFrm > 0) {
				if(index == OBSTACLE_CRAB_A) {
					fdActorSelectAnimation(
						fdActorGetById(ACTOR_LEMURIA_SPRING_CRAB_A),
						ANIM_CRAB_RAISE_CLAWS);
				}
				else {
					fdActorSelectAnimation(
						fdActorGetById(ACTOR_LEMURIA_SPRING_CRAB_B),
						ANIM_CRAB_RAISE_CLAWS);
				}
			}
			else {
				if(index == OBSTACLE_CRAB_A) {
					fdActorSelectAnimation(
						fdActorGetById(ACTOR_LEMURIA_SPRING_CRAB_A),
						ANIM_CRAB_SCUTTLE);
				}
				else {
					fdActorSelectAnimation(
						fdActorGetById(ACTOR_LEMURIA_SPRING_CRAB_B),
						ANIM_CRAB_SCUTTLE);
				}
				if(obstacle->pauseOrPhase == 0) {
					s32 pos;

					pos = obstacle->pos.x;
					if(obstacle->heading == CRAB_HEADING_EAST) {
						obstacle->pos.x = pos + motionStep;
					}
					else {
						obstacle->pos.x = pos - motionStep;
					}
					if(obstacle->pos.x <= toFIXED(64)) {
						obstacle->heading = CRAB_HEADING_EAST;
						if(index == OBSTACLE_CRAB_B) {
							obstacle->pauseOrPhase = CRAB_TURN_PAUSE_FRAMES;
						}
					}
					if(obstacle->pos.x > toFIXED(176) - 1) {
						obstacle->heading = CRAB_HEADING_WEST;
						if(index == OBSTACLE_CRAB_B) {
							obstacle->pauseOrPhase = CRAB_TURN_PAUSE_FRAMES;
						}
					}
				}
				else {
					obstacle->pauseOrPhase = obstacle->pauseOrPhase - 1;
				}
			}
		}
		else if(index == OBSTACLE_TURTLE_A) {
			speedMode = obstacle->speedMode;
			motionStep = -TURTLE_SWIM_TURN;
			if(speedMode == 1) {
				motionStep = motionStep * 2;
			}
			if(speedMode == 2) {
				motionStep = motionStep * 3;
			}
			if(obstacle->reactionFrm > 0) {
				fdActorSelectAnimation(
					fdActorGetById(ACTOR_LEMURIA_SPRING_TURTLE_A),
					ANIM_TURTLE_HIDE);
			}
			else {
				fdActorSelectAnimation(
					fdActorGetById(ACTOR_LEMURIA_SPRING_TURTLE_A),
					ANIM_TURTLE_SWIM);
				obstacle->pos.x = sySin(obstacle->heading) * 48 + toFIXED(112);
				obstacle->pos.z = syCos(obstacle->heading) * 40 + toFIXED(72);
				cycleFrame = (u16)obstacle->heading + motionStep;
				obstacle->heading = cycleFrame;
				obstacle->pauseOrPhase = obstacle->pauseOrPhase + 1;
			}
		}
		else {
			cycleFrame = obstacle->pauseOrPhase & 511;
			speedMode = obstacle->speedMode;
			motionStep = TURTLE_SWIM_TURN;
			if(speedMode == 1) {
				motionStep = motionStep * 2;
			}
			if(speedMode == 2) {
				motionStep = motionStep * 3;
			}
			if(obstacle->reactionFrm > 0) {
				fdActorSelectAnimation(
					fdActorGetById(ACTOR_LEMURIA_SPRING_TURTLE_B),
					ANIM_TURTLE_HIDE);
			}
			else if(cycleFrame <= 383) {
				obstacle->pos.x = sySin(obstacle->heading) * 52 + toFIXED(112);
				obstacle->pos.z = syCos(obstacle->heading) * 24 + toFIXED(72);
				obstacle->heading = obstacle->heading + motionStep;
				fdActorSelectAnimation(
					fdActorGetById(ACTOR_LEMURIA_SPRING_TURTLE_B),
					ANIM_TURTLE_SWIM);
			}
			else {
				fdActorSelectAnimation(
					fdActorGetById(ACTOR_LEMURIA_SPRING_TURTLE_B),
					ANIM_TURTLE_HIDE);
			}
			obstacle->pauseOrPhase = obstacle->pauseOrPhase + 1;
		}

		/* A creature the token drifts against knocks it away at 3 pixels a
		 * frame */
		/* and shortens the rings' pull; it flinches and speeds up a step. */
		if(obstacle->hitCooldownFrm == 0 && toss->trail[0].y == 0) {
			s32 distanceSquared;
			s32 deltaX;
			s32 deltaZ;

			deltaX = FIXEDtoINT(obstacle->pos.x - toss->trail[0].x);
			deltaZ = FIXEDtoINT(obstacle->pos.z - toss->trail[0].z);
			distanceSquared = deltaX * deltaX + deltaZ * deltaZ;
			if(distanceSquared <= 119 && toss->attractionFrm > 30) {
				s32 nextMode;
				FIXED speed;

				speed = toFIXED(3);
				if(index <= OBSTACLE_CRAB_B) {
					if(obstacle->heading == CRAB_HEADING_EAST) {
						if(toss->vel.x < speed) {
							toss->vel.x = speed;
							toss->attractionFrm = toss->attractionFrm - 100;
						}
					}
					else {
						if(toss->vel.x > -speed) {
							toss->vel.x = -speed;
							toss->attractionFrm = toss->attractionFrm - 100;
						}
					}
				}
				else {
					s32 distance;

					/* BUG: coincident positions leave a zero divisor below. */
					distance = sySquart(distanceSquared);
					toss->vel.x = -deltaX * speed / distance;
					toss->vel.z = -deltaZ * speed / distance;
					toss->attractionFrm = toss->attractionFrm - 100;
				}
				fdEventPlaySnd(SE_TOLBI_SPRING_OBSTACLE_HIT);
				nextMode = (obstacle->speedMode + 1) % 3;
				obstacle->reactionFrm = 36;
				obstacle->speedMode = nextMode;
				obstacle->hitCooldownFrm = 30;
			}
		}

		switch(index) {
		case OBSTACLE_CRAB_A:
			scLemuriaSpringSetActorAppearance(
				ACTOR_LEMURIA_SPRING_CRAB_A, &obstacle->pos.x, 0,
				LemuriaSpring_CrabColorModes[obstacle->speedMode],
				ANIM_STEP_NORMAL * (obstacle->speedMode + 1));
			break;
		case OBSTACLE_CRAB_B:
			scLemuriaSpringSetActorAppearance(
				ACTOR_LEMURIA_SPRING_CRAB_B, &obstacle->pos.x, 0,
				LemuriaSpring_CrabColorModes[obstacle->speedMode],
				ANIM_STEP_NORMAL * (obstacle->speedMode + 1));
			break;
		case OBSTACLE_TURTLE_A:
			scLemuriaSpringSetActorAppearance(
				ACTOR_LEMURIA_SPRING_TURTLE_A, &obstacle->pos.x,
				DEGtoANG(180) - obstacle->heading,
				LemuriaSpring_TurtleColorModes[obstacle->speedMode],
				ANIM_STEP_NORMAL * (obstacle->speedMode + 1));
			break;
		case OBSTACLE_TURTLE_B:
			scLemuriaSpringSetActorAppearance(
				ACTOR_LEMURIA_SPRING_TURTLE_B, &obstacle->pos.x,
				TRIG_TURN - 1 - obstacle->heading,
				LemuriaSpring_TurtleColorModes[obstacle->speedMode],
				ANIM_STEP_NORMAL * (obstacle->speedMode + 1));
			break;
		}
		index++;
	} while(index != OBSTACLE_COUNT);

	toss->shadow.x = toss->trail[0].x;
	toss->shadow.y = 0;
	toss->shadow.z = toss->trail[0].z;

	if(LemuriaSpring_TossKind == SPRING_TOSS_MEDAL) {
		scLemuriaSpringSetActorAppearance(ACTOR_LEMURIA_SPRING_MEDAL,
										  &toss->trail[0].x, 0, 0,
										  ANIM_STEP_NORMAL);
		scLemuriaSpringSetActorAppearance(ACTOR_LEMURIA_SPRING_MEDAL_SHADOW,
										  &toss->shadow.x, 0, 0,
										  ANIM_STEP_NORMAL);
		scLemuriaSpringSetActorAppearance(LEMURIA_SPRING_MEDAL_TRAIL(1),
										  &toss->trail[1].x, 0, 0,
										  ANIM_STEP_NORMAL);
		scLemuriaSpringSetActorAppearance(LEMURIA_SPRING_MEDAL_TRAIL(2),
										  &toss->trail[2].x, 0, 0,
										  ANIM_STEP_NORMAL);
		scLemuriaSpringSetActorAppearance(LEMURIA_SPRING_MEDAL_TRAIL(3),
										  &toss->trail[3].x, 0, 0,
										  ANIM_STEP_NORMAL);
		fdActorSelectAnimation(fdActorGetById(LEMURIA_SPRING_MEDAL_TRAIL(1)),
							   ANIM_TOKEN_GLINT);
		fdActorSelectAnimation(fdActorGetById(LEMURIA_SPRING_MEDAL_TRAIL(2)),
							   ANIM_TOKEN_GLINT);
		fdActorSelectAnimation(fdActorGetById(LEMURIA_SPRING_MEDAL_TRAIL(3)),
							   ANIM_TOKEN_GLINT);
	}
	else {
		scLemuriaSpringSetActorAppearance(ACTOR_LEMURIA_SPRING_COIN,
										  &toss->trail[0].x, 0, 0,
										  ANIM_STEP_NORMAL);
		scLemuriaSpringSetActorAppearance(ACTOR_LEMURIA_SPRING_COIN_SHADOW,
										  &toss->shadow.x, 0, 0,
										  ANIM_STEP_NORMAL);
		scLemuriaSpringSetActorAppearance(LEMURIA_SPRING_COIN_TRAIL(1),
										  &toss->trail[1].x, 0, 0,
										  ANIM_STEP_NORMAL);
		scLemuriaSpringSetActorAppearance(LEMURIA_SPRING_COIN_TRAIL(2),
										  &toss->trail[2].x, 0, 0,
										  ANIM_STEP_NORMAL);
		scLemuriaSpringSetActorAppearance(LEMURIA_SPRING_COIN_TRAIL(3),
										  &toss->trail[3].x, 0, 0,
										  ANIM_STEP_NORMAL);
		fdActorSelectAnimation(fdActorGetById(LEMURIA_SPRING_COIN_TRAIL(1)),
							   ANIM_TOKEN_GLINT);
		fdActorSelectAnimation(fdActorGetById(LEMURIA_SPRING_COIN_TRAIL(2)),
							   ANIM_TOKEN_GLINT);
		fdActorSelectAnimation(fdActorGetById(LEMURIA_SPRING_COIN_TRAIL(3)),
							   ANIM_TOKEN_GLINT);
	}

	if(toss->launchFrm != -1) {
		toss->launchFrm = toss->launchFrm + 1;
	}
}
#undef COIN_WINDOW_WIDTH
#undef COIN_NUMBER_X
#undef TOSS_WINDOW_X
#undef TOSS_WINDOW_WIDTH
#undef WEIGHT

void scLemuriaSpringUpdateTossSimulation(void);

/* Puts the crabs and turtles where they start, the token out of sight west of
 */
/* the spring, and starts the task that moves them. */
void scLemuriaSpringInitTossSimulation(void) {
	SPRING_TOSS_MOTION *toss = &LemuriaSpring_Toss;
	SPRING_OBSTACLE *obstacle;
	s32 i;

	for(i = 0; i != OBSTACLE_COUNT; i++) {
		obstacle = &LemuriaSpring_Obstacles[i];
		obstacle->pos.x = INTtoFIXED(LemuriaSpring_ObstacleStartX[i]);
		obstacle->pos.y = 0;
		obstacle->pos.z = INTtoFIXED(LemuriaSpring_ObstacleStartZ[i]);
		obstacle->heading = LemuriaSpring_ObstacleHeadings[i];
		obstacle->pauseOrPhase = 0;
		obstacle->speedMode = 0;
		obstacle->reactionFrm = 0;
		obstacle->hitCooldownFrm = 0;
	}

	toss->trail[0].x = -toFIXED(30);
	toss->trail[0].y = 0;
	toss->trail[0].z = toFIXED(100);
	toss->vel.x = 0;
	toss->vel.y = 0;
	toss->vel.z = 0;
	toss->attractionFrm = 0;

	fdActorSelectAnimation(fdActorGet(ACTOR_LEMURIA_SPRING_TURTLE_A),
						   ANIM_TURTLE_SWIM);
	fdActorSelectAnimation(fdActorGet(ACTOR_LEMURIA_SPRING_TURTLE_B),
						   ANIM_TURTLE_SWIM);

	syTaskAddFrame((TASK_FUNC)scLemuriaSpringUpdateTossSimulation,
				   TASK_PASS_FRAME + 3);
}

/* Tosses the chosen coin or medal and waits for it to come to rest; returns */
/* the ring it rests in. */
s32 scLemuriaSpringRunToss(s32 tossKind) {
	SPRING_TOSS_MOTION *toss;

	toss = &LemuriaSpring_Toss;
	toss->trail[0].y = 0;
	toss->trail[1].y = 0;
	toss->trail[2].y = 0;
	toss->trail[3].y = 0;
	LemuriaSpring_TossKind = tossKind;
	LemuriaSpring_TossEnded = 0;
	toss->launchFrm = -1;
	for(LemuriaSpring_TossFrame = 0;; LemuriaSpring_TossFrame++) {
		if(LemuriaSpring_TossFrame == 50)
			fdEventPlaySnd(SE_TOLBI_SPRING_TOSS);
		if(LemuriaSpring_TossFrame == 16) {
			fdActorSetAnimation(MainRoutineStateWork.leaderActorId,
								ANIM_LEADER_TOSS);
			toss->launchFrm = 0;
			toss->vel.x = toFIXED(13) / 10;
			toss->vel.y = toFIXED(4);
			toss->vel.z = -toFIXED(2);
			toss->trail[0].x = toFIXED(120);
			toss->trail[0].y = toFIXED(16);
			toss->trail[0].z = toFIXED(152);
			toss->attractionFrm = 300;
			if(LemuriaSpring_TossKind == SPRING_TOSS_MEDAL) {
				fdActorSelectAnimation(
					fdActorGet(ACTOR_LEMURIA_SPRING_MEDAL_SHADOW),
					ANIM_TOKEN_SHADOW);
				fdActorSelectAnimation(fdActorGet(ACTOR_LEMURIA_SPRING_MEDAL),
									   ANIM_TOKEN_TUMBLE);
				scLemuriaSpringSetSprStorageKind(LEMURIA_SPRING_MEDAL_TRAIL(1),
												 1);
				scLemuriaSpringSetSprStorageKind(LEMURIA_SPRING_MEDAL_TRAIL(2),
												 1);
				scLemuriaSpringSetSprStorageKind(LEMURIA_SPRING_MEDAL_TRAIL(3),
												 1);
			}
			else {
				fdActorSelectAnimation(
					fdActorGet(ACTOR_LEMURIA_SPRING_COIN_SHADOW),
					ANIM_TOKEN_SHADOW);
				fdActorSelectAnimation(fdActorGet(ACTOR_LEMURIA_SPRING_COIN),
									   ANIM_TOKEN_TUMBLE);
				scLemuriaSpringSetSprStorageKind(LEMURIA_SPRING_COIN_TRAIL(1),
												 1);
				scLemuriaSpringSetSprStorageKind(LEMURIA_SPRING_COIN_TRAIL(2),
												 1);
				scLemuriaSpringSetSprStorageKind(LEMURIA_SPRING_COIN_TRAIL(3),
												 1);
			}
		}
		syWait(1);
		if(LemuriaSpring_TossEnded == 1)
			break;
	}
	return LemuriaSpring_TossResult;
}

SCENE_ENTRANCE SceneEntrances[] = {
	{0, CONDITION_ALWAYS, 288, 0, 454, FACING_NORTH, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

u32 SceneExits[] = {
	SCENE_EXITS(LemuriaSpring),
	SCENE_EXIT_UNLESS(LEMURIA_SPRING_TO_LEMURIA, Lemuria,
					  LEMURIA_FROM_LEMURIA_SPRING, EXIT_FLAG_NONE),
	SCENE_EXITS_END,
};

ACTOR_PLACE ActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_SPRING_COIN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
					0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_SPRING_COIN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
					0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_SPRING_COIN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
					0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_SPRING_COIN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
					0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_SPRING_COIN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
					0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_SPRING_LUCKY_MEDAL, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH,
					TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_SPRING_LUCKY_MEDAL, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH,
					TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_SPRING_LUCKY_MEDAL, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH,
					TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_SPRING_LUCKY_MEDAL, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH,
					TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_SPRING_LUCKY_MEDAL, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH,
					TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_CRAB, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0,
					FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_CRAB, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0,
					FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_TURTLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0,
					0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_TURTLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0,
					0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_SIGN, CONDITION_ALWAYS, 7, 176, 0, 144, FACING_SOUTH,
					TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_SIGN, CONDITION_ALWAYS, 7, 72, 0, 144, FACING_SOUTH,
					TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_SIGN, CONDITION_ALWAYS, 7, 176, 0, 168, FACING_SOUTH,
					TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_SIGN, CONDITION_ALWAYS, 7, 72, 0, 168, FACING_SOUTH,
					TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_LEMURIAN_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
					48, 0, 136, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST,
					TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_LEMURIAN_IN_TEAL, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 216, 0, 96, FACING_WEST,
					TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_TABLET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
					-56, 0, -56, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
	{SCENE_TABLE_END},
};

/* clang-format off */
/* The blend weights the spring fades in through, eight steps from 10/9 to */
/* 0/16 (first target over second, in sixteenths). */
u16 LemuriaSpring_AlphaSteps[] = {
    BLEND_ALPHA(10, 9), BLEND_ALPHA(8, 10), BLEND_ALPHA(6, 11), BLEND_ALPHA(4, 12),
    BLEND_ALPHA(3, 13), BLEND_ALPHA(2, 14), BLEND_ALPHA(1, 15),
    BLEND_ALPHA(0, BLEND_WEIGHT_MAX),
};

TRIGGER SceneEvents[] = {
    {TRIGGER_EXIT, LEMURIA_SPRING_TO_LEMURIA, CONDITION_ALWAYS, LEMURIA_SPRING_TO_LEMURIA},
    {TRIGGER_TALK, ACTOR_LEMURIA_SPRING_MEDAL_SIGN, CONDITION_ALWAYS, MSGNO(MsgReadTheSign)},
    {TRIGGER_TALK, ACTOR_LEMURIA_SPRING_COIN_SIGN, CONDITION_ALWAYS, MSGNO(MsgReadTheSignThe)},
    {TRIGGER_TALK, ACTOR_LEMURIA_SPRING_VISITOR_FACING_AWAY, CONDITION_ALWAYS, MSGNO(MsgLemuriaSpringIfYouFaceAway)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LEMURIA_SPRING_VISITOR_FACING_AWAY, CONDITION_ALWAYS, MSGNO(MsgLemuriaSpringIdLoveToTest)},
    {TRIGGER_TALK, ACTOR_LEMURIA_SPRING_VISITOR_REMEMBERING_OUTSIDER, CONDITION_ALWAYS, MSGNO(MsgLemuriaSpringIRememberOutsiderWhoCame)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LEMURIA_SPRING_VISITOR_REMEMBERING_OUTSIDER, CONDITION_ALWAYS, MSGNO(MsgLemuriaSpringHeHadSuchGoodTime)},
    {TRIGGER_EXAMINE, CELL_BEHAVIOR_LEMURIA_SPRING_RIM, FLAG_LEMURIA_SPRING_TARGET_SHOWN, EVENT(scLemuriaSpringDrinkFromSpring)},
    {TRIGGER_EXAMINE, CELL_BEHAVIOR_LEMURIA_SPRING_WATER, CONDITION_ALWAYS, EVENT(scLemuriaSpringTossCoinOrMedal)},
    {TRIGGER_TABLE_END},
};

/* The items a medal can win, drawn by the weights in MainRoutineStateWork: the */
/* middle ring draws from the first three, each ring further out from three */
/* more. */
u32 LemuriaSpring_MedalPrizes[MEDAL_PRIZE_COUNT] = {
    ITEM_HESTIA_BLADE, ITEM_AEGIS_SHIELD, ITEM_EREBUS_ARMOR,
    ITEM_MINERVA_HELM, ITEM_MIGHTY_AXE, ITEM_FLORAL_DRESS,
    ITEM_CROWN_OF_GLORY, ITEM_LEDAS_BRACELET, ITEM_FIREMANS_POLE,
    ITEM_BRILLIANT_CIRCLET, ITEM_CRAFTED_GLOVES, ITEM_WILD_COAT,
    ITEM_POTION, ITEM_PSY_CRYSTAL, ITEM_WATER_OF_LIFE,
};

/* The coins a coin wins by where it lands, the middle first; 0 for a miss. */
u16 LemuriaSpring_CoinPrizes[] = {
    20, 10, 2, 1, 0,
};

/* The crabs' colour modes, by speed. */
u8 LemuriaSpring_CrabColorModes[] = {
    SPRITE_COLOR_NORMAL, SPRITE_COLOR_GRB, SPRITE_COLOR_BLUE_TINT,
};

/* The turtles' colour modes, by speed. */
u8 LemuriaSpring_TurtleColorModes[] = {
    SPRITE_COLOR_NORMAL, SPRITE_COLOR_BGR, SPRITE_COLOR_GBR,
};

/* Where each obstacle starts, across. */
u8 LemuriaSpring_ObstacleStartX[] = {
    80, 160, 80, 72,
};

/* Where each obstacle starts, down. */
u8 LemuriaSpring_ObstacleStartZ[] = {
    32, 104, 68, 72,
};

/* The way each obstacle starts heading: the crabs east or west, the turtles */
/* by their angle round their loops. */
u16 LemuriaSpring_ObstacleHeadings[] = {
    CRAB_HEADING_EAST, CRAB_HEADING_WEST, 0, DEGtoANG(180),
};
/* clang-format on */
