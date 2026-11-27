/* The Spring of Lemuria, up in the middle of the town past the man who offers
   the test of luck. The party drinks from it to restore its HP and PP, and from
   the step between its two signs tosses coins or Lucky Medals into it, past the
   turtles and crabs that swim there, as in Tolbi's spring game: what lands in
   its rings wins coins or items, and the first medal in the middle wins the
   summon Eclipse. */

#include "item_ids.h"
#include "game/game.h"
#include "macros.h"
#include "field/script_inline.h"
#include "field/field_mode_script.h"
#include "scene/scene.h"
#include "window/window.h"
#include "edition.h"
#include "field/call.h"
#include "field/field_mode.h"
#include "text/msg_ids.h"

/* The spring's actors, as The Broken Seal numbers Tolbi's spring, by the rows
   of ActorPlacements.

   The toss draws the coin or the medal with its shadow and three trailing
   copies, which LEMURIA_SPRING_COIN_TRAIL and LEMURIA_SPRING_MEDAL_TRAIL
   number. */
typedef enum LEMURIA_SPRING_ACTOR_ID {
	ACTOR_LEMURIA_SPRING_COIN_SHADOW = ACTOR_FIRST_PLACED + 3,
	/* The coin the party tosses (SPRITE_SPRING_COIN). */
	ACTOR_LEMURIA_SPRING_COIN,
	ACTOR_LEMURIA_SPRING_MEDAL_SHADOW = ACTOR_LEMURIA_SPRING_COIN + 4,
	/* The Lucky Medal the party tosses (SPRITE_SPRING_LUCKY_MEDAL). */
	ACTOR_LEMURIA_SPRING_MEDAL,
	/* The four obstacles that move in the spring during a toss: two crabs, then
	   two turtles. */
	ACTOR_LEMURIA_SPRING_CRAB_A,
	ACTOR_LEMURIA_SPRING_CRAB_B,
	ACTOR_LEMURIA_SPRING_TURTLE_A,
	ACTOR_LEMURIA_SPRING_TURTLE_B,
	ACTOR_LEMURIA_SPRING_MEDAL_SIGN, /* east of the spring */
	ACTOR_LEMURIA_SPRING_COIN_SIGN,	 /* west of the spring */
	/* The grey slabs at the foot of each sign: the signs' sprite in its second
	   picture, drawn underfoot (scInit). */
	ACTOR_LEMURIA_SPRING_EAST_SIGN_SLAB,
	ACTOR_LEMURIA_SPRING_WEST_SIGN_SLAB,
	/* "If you face away from the spring, toss in a coin..." */
	ACTOR_LEMURIA_SPRING_VISITOR_FACING_AWAY,
	/* Unused: a man carrying a pack, placed at the map's corner, where no cue
	   answers him; Tolbi's spring places him to ask for a medal. */
	ACTOR_LEMURIA_SPRING_MAN_WITH_PACK,
	/* "I remember an outsider who came..." */
	ACTOR_LEMURIA_SPRING_VISITOR_REMEMBERING_OUTSIDER,
	/* The stone tablet that brings Eclipse to the leader the first time a medal
	   lands in the middle of the target (scLemuriaSpringGivePrize). */
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

/* Animations of the spring's sprites, as The Broken Seal names Tolbi's. The
   coin's and the Lucky Medal's sprite sheets (spring_coin.png,
   spring_lucky_medal.png) are drawn alike. */
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
	/* The signpost sprite sheet's second picture, a grey slab
	   (signpost.png). */
	ANIM_SIGN_SLAB = 2
} LEMURIA_SPRING_ANIMATION;

/* What searching answers at Lemuria's spring. */
typedef enum LEMURIA_SPRING_METATILE_BEHAVIOR {
	/* The spring's rim: the party drinks from it while its target is hidden. */
	METATILE_BEHAVIOR_LEMURIA_SPRING_RIM = 25,
	/* The water before the spring's steps: searched to toss a coin or medal. */
	METATILE_BEHAVIOR_LEMURIA_SPRING_WATER = 80
} LEMURIA_SPRING_METATILE_BEHAVIOR;

/* Set once the spring's water has faded to show the target at its bottom, for
   the rest of the visit. */
#define FLAG_LEMURIA_SPRING_TARGET_SHOWN FLAG_SCENE_TEMP_00

/* The middle of the target, which the target pulls the tossed token toward. */
#define LEMURIA_SPRING_CENTER_X toFIXED(120)
#define LEMURIA_SPRING_CENTER_Z toFIXED(71)

/* Where a tossed token comes to rest: the target's four rings, the middle
   first, then none. */
typedef enum SPRING_TOSS_RESULT {
	TOSS_RING_MIDDLE,
	TOSS_RING_SECOND,
	TOSS_RING_THIRD,
	TOSS_RING_OUTER,
	TOSS_RING_MISSED
} SPRING_TOSS_RESULT;

/* How much of its speed a token on the water keeps each tick, in 256ths: while
   the target pulls it, and once it drifts free. */
#define SPEED_KEPT_SCALE  256
#define PULLED_SPEED_KEPT 253
#define DRIFT_SPEED_KEPT  220

/* The crabs and turtles in the spring, by their slots in
   LemuriaSpringObstacles. */
typedef enum LEMURIA_SPRING_OBSTACLE {
	OBSTACLE_CRAB_A,
	OBSTACLE_CRAB_B,
	OBSTACLE_TURTLE_A,
	OBSTACLE_TURTLE_B,
	OBSTACLE_COUNT
} LEMURIA_SPRING_OBSTACLE;

/* Which way a crab scuttles (its SPRING_OBSTACLE.heading). */
enum { CRAB_HEADING_EAST, CRAB_HEADING_WEST };

/* The ticks the second crab waits at each end of its way. */
#define CRAB_TURN_PAUSE_TICKS 30
/* A turtle's slowest swim round its loop, a 1024th of a turn a tick. */
#define TURTLE_SWIM_TURN	  (TRIG_TURN / 1024)

/* scLemuriaSpringGivePrize's item for the summon Eclipse. */
#define PRIZE_ECLIPSE (-1)

/* The items a Lucky Medal can win, and how many more of them each ring out from
   the middle allows. */
#define MEDAL_PRIZE_COUNT	  15
#define MEDAL_PRIZES_PER_RING 3

/* How far the water sinks before it fades. */
#define WATER_SHIFT_PIXELS 16

Sint32 scLemuriaSpringRunSpringGame(void);

extern Uint16 LemuriaSpringCoinPrizes[];
extern Uint32 LemuriaSpringMedalPrizes[MEDAL_PRIZE_COUNT];

Sint32 scLemuriaSpringRunToss();

void scLemuriaSpringGivePrize(Sint32 item);

extern Uint8 LemuriaSpringCrabColorModes[];
extern Uint8 LemuriaSpringTurtleColorModes[];
extern Uint8 LemuriaSpringObstacleStartX[];
extern Uint8 LemuriaSpringObstacleStartZ[];

extern Uint16 LemuriaSpringObstacleHeadings[];

void scLemuriaSpringSetSprStorageKind(Sint32 actor, Sint32 active);

SPRING_TOSS_MOTION LemuriaSpringToss BSS_DATA ALIGNED(16);
Sint32 LemuriaSpringTossKind				  BSS_DATA;
/* The crabs and turtles in the toss's way, as the toss moves them. */
SPRING_OBSTACLE LemuriaSpringObstacles[OBSTACLE_COUNT] BSS_DATA ALIGNED(16);

Sint32 LemuriaSpringTossTick   BSS_DATA;
Sint32 LemuriaSpringTossEnded  BSS_DATA;
Sint32 LemuriaSpringTossResult BSS_DATA;

const SCENE_ENTRANCE *scGetEntrances(void)
{
	return SceneEntrances;
}

const SCENE_REGION *scGetExitRegions(void)
{
	return 0;
}

const Uint32 *scGetExits(void)
{
	return SceneExits;
}

const ACTOR_PLACE *scGetActorPlacements(void)
{
	return ActorPlacements;
}

/* Felix checks the rim of the spring, any of its edges round the pool, before
   the target has been shown this visit: "Felix and the others drank of the
   spring. Their HP and PP were fully replenished!" */
void scLemuriaSpringDrinkFromSpring(void)
{
	/* "Felix and the others drank of the spring. Their HP and PP were
	   fully..." */
	mnMessageShowCentered((Sint32)MsgAndTheOthers, 1);
	fdScriptPlaySnd(SE_RECOVERY);
	gmPartyRestoreHpPp(1);
}

extern Uint16 LemuriaSpringAlphaSteps[];

/* The spring's water, the overhead layer, sinks 16 pixels, a pixel every four
   ticks, then fades away in eight steps to show the target at the bottom of
   the spring, as Tolbi's does. */
void scLemuriaSpringDrainSpring(void)
{
	MAP_STATE *map = Ram_HeapSlots->mapGrid;
	MAP_LAYER *layer;
	Sint32	   i;

	fdScriptPlaySnd(SE_BURST);
	layer = &map->layers[MAPGRID_LAYER_OVERHEAD];
	for(i = 0; i < WATER_SHIFT_PIXELS; i++) {
		layer->ofsY -= toFIXED(1);
		sySynch(4);
	}
	for(i = 0; i < 8; i++) {
		syIOWrite16(BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL,
			&REG_BLDCNT);
		syIOWrite16(LemuriaSpringAlphaSteps[i], &REG_BLDALPHA);
		sySynch(8);
	}
}

/* Felix checks the water from the step at the south of the spring, between the
   two signs, metatile (7, 8) or (7, 10): he turns his back to the spring to
   toss a coin or a Lucky Medal into it. The first time each visit, the spring's
   water first fades to show the target of rings at its bottom. */
void scLemuriaSpringTossCoinOrMedal(void)
{
	Sint32 actor = MainRoutineStateWork.leaderActorId;

	if(!fdScriptGetFlag(FLAG_LEMURIA_SPRING_TARGET_SHOWN)) {
		fdScriptSetFlag(FLAG_LEMURIA_SPRING_TARGET_SHOWN);
		scLemuriaSpringDrainSpring();
	}
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdActorWalkToAndWait(actor, 120, 152);
	fdActorFaceDirection(actor, FACING_SOUTH, 0);
	scLemuriaSpringRunSpringGame();
	fdScriptEnd();
}

const CUE *scGetCues(void)
{
	return SceneCues;
}

void scLemuriaSpringInitTossSimulation(void);

/* Starts the scene: the water blended over the target, the slabs at the signs'
   feet drawn underfoot; arriving from Lemuria, the toss is set up, and the
   water stays faded once the target has been shown. */
Sint32 scInit(void)
{
	ACTOR *leader;
	Sint32 blend;

	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	leader->priFlg = ACTOR_PRIORITY_IGNORES_RISES | leader->priFlg;
	blend = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
	REG_BLDCNT = blend;
	syIOWrite16(BLEND_ALPHA(12, 8), &REG_BLDALPHA);
	fdActorSetAnimation(ACTOR_LEMURIA_SPRING_EAST_SIGN_SLAB, ANIM_SIGN_SLAB);
	fdActorSetAnimation(ACTOR_LEMURIA_SPRING_WEST_SIGN_SLAB, ANIM_SIGN_SLAB);
	fdActorGet(ACTOR_LEMURIA_SPRING_EAST_SIGN_SLAB)->priFlg =
		ACTOR_PRIORITY_UNDERFOOT;
	fdActorGet(ACTOR_LEMURIA_SPRING_WEST_SIGN_SLAB)->priFlg =
		ACTOR_PRIORITY_UNDERFOOT;
	fdScriptStartScreenTransition();
	if(MainRoutineStateWork.entrance == LEMURIA_SPRING_FROM_LEMURIA) {
		scLemuriaSpringInitTossSimulation();
		if(fdScriptGetFlag(FLAG_LEMURIA_SPRING_TARGET_SHOWN)) {
			syIOWrite16(blend, &REG_BLDCNT);
			syIOWrite16(BLEND_ALPHA(0, BLEND_WEIGHT_MAX), &REG_BLDALPHA);
		}
	}
	return 0;
}

Sint32 scPrepareMap(void)
{
	return 0;
}

/* The Japanese catalog does not name these yet. */
extern Uint8 MsgLemuriaSpringWhatWillYouThrow[], MsgLemuriaSpringCoins[],
	MsgLemuriaSpringToss[], MsgLemuriaSpringYouWonCoin[];

/* French draws the coin window two tiles wider, its numbers 16 pixels further
   right; Japanese draws them 16 pixels further left, each followed by its
   counter word. */
#define COIN_WINDOW_WIDTH FRENCH_OR(19, 17)
#define COIN_NUMBER_X	  BY_LANGUAGE(56, 72, 72, 72, 88, 72)

/* The toss prompt's window: two tiles narrower in Japanese, against the right
   edge. */
#define TOSS_WINDOW_X	  JAPANESE_OR(22, 20)
#define TOSS_WINDOW_WIDTH JAPANESE_OR(7, 9)

/* The weight of prize i in the draw of what a toss wins. */
#define WEIGHT(i) ((Sint8)MainRoutineStateWork.prizeWeights[i])

/* The toss: the party picks a coin or a Lucky Medal, throws it, and what lands
   in the spring's rings wins its prize, as in Tolbi's spring game. */
Sint32 scLemuriaSpringRunSpringGame(void)
{
	Sint32 choice;
#if LANGUAGE == LANGUAGE_JAPANESE
	Sint32 text;
#endif
	Sint32	msg;
	Sint32	coins;
	Sint32	medals;
	WINDOW *win;
	Sint32	result;
	Sint32	prizeRoll;
	Sint32	total;
	Sint32	prize;
	Sint32	cancelled = -1; /* the toss's result when B puts it off */

#if LEMURIA_SPRING_PAYS_ON_CONFIRM
	fdScriptStart();
	choice = 0;
	fdScriptEndLastingPsynergy(0);
	for(;;) {
		coins = MainRoutineStateWork.coins;
		medals = gmPartyInventoryCountItem(ITEM_LUCKY_MEDAL);
		msg = (Sint32)MsgLemuriaSpringWhatWillYouThrow;
		fdScriptSetMessageId(msg);
		/* "What will you throw?" */
		fdScriptOpenMessage(-1, 0);
		win = mnWindowCreate(0, 0, COIN_WINDOW_WIDTH, 4, 2);
		result = (Sint32)MsgLemuriaSpringCoins;
		mnTextDrawMessageAsTiles(result, win, 0, 0);
		mnTextDrawNumInWindow(coins, 6, win, COIN_NUMBER_X, 0);
		mnTextDrawMessageAsTiles(result + 1, win, 0, 8);
		mnTextDrawNumInWindow(medals, 6, win, COIN_NUMBER_X, 8);
		choice = mnSelectCoinsOrMedals(choice);
		mnWindowClose(win, 2);
		mnMessageCloseFinishedWindows();
		if(choice == -1) {
			fdScriptEnd();
			return 0;
		}
		if(choice == SPRING_TOSS_COIN) {
			if(coins == 0) {
				fdScriptSetMessageId(msg + 1);
				/* "You don't have any coins!" */
				fdScriptShowMessage(-1, 0);
				sySynch(1);
				continue;
			}
		} else if(choice == SPRING_TOSS_MEDAL) {
			if(medals == 0) {
				fdScriptSetMessageId(msg + 2);
				/* "You have no Lucky Medals!" */
				fdScriptShowMessage(-1, 0);
				sySynch(1);
				continue;
			}
			if(gmPartyInventoryCountFreeSlots() == 0) {
				fdScriptSetMessageId(msg + 4);
				/* "You can't carry more items. Do you still want to play?" */
				fdScriptOpenMessage(-1, 0);
				if(fdScriptAskYesNoWithActorAnimation(0, 0) != 0)
					goto end;
			}
		}
		win = mnWindowCreate(TOSS_WINDOW_X, 15, TOSS_WINDOW_WIDTH, 4, 2);
		result = (Sint32)MsgLemuriaSpringToss;
		mnTextDrawMessageAsTiles(result, win, 0, 0);
		mnTextDrawMessageAsTiles(result + 1, win, 0, 8);
		sySynch(5);
		fdScriptPlaySnd(SE_TOLBI_SPRING_TOSS_PROMPT);
		for(;;) {
			if(PerWork.push & KEY_A) {
				fdScriptPlaySnd(SE_MENU_CONFIRM);
				result = 0;
				break;
			}
			if(PerWork.push & KEY_B) {
				fdScriptPlaySnd(SE_MENU_CANCEL);
				result = cancelled;
				break;
			}
			sySynch(1);
		}
		mnWindowClose(win, 2);
		break;
	}
#else
	/* The Japanese release keeps The Broken Seal's Japanese layout: a counter
	   word after each count, and the coin or medal taken as soon as it is
	   chosen, so cancelling the toss loses it. In every edition but the
	   Japanese it is taken once the toss is confirmed. */
	choice = 0;
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	for(;;) {
		coins = MainRoutineStateWork.coins;
		medals = gmPartyInventoryCountItem(ITEM_LUCKY_MEDAL);
		msg = (Sint32)MsgLemuriaSpringWhatWillYouThrow;
		fdScriptSetMessageId(msg);
		/* "What will you throw?" */
		fdScriptOpenMessage(-1, 0);
		win = mnWindowCreate(0, 0, COIN_WINDOW_WIDTH, 4, 2);
		text = (Sint32)MsgLemuriaSpringCoins;
		mnTextDrawMessageAsTiles(text, win, 0, 0);
		mnTextDrawNumInWindow(coins, 6, win, COIN_NUMBER_X, 0);
		mnTextDrawMessageAsTiles(text + 2, win, 104, 0);
		mnTextDrawMessageAsTiles(text + 1, win, 0, 8);
		mnTextDrawNumInWindow(medals, 6, win, COIN_NUMBER_X, 8);
		mnTextDrawMessageAsTiles(text + 2, win, 104, 8);
		choice = mnSelectCoinsOrMedals(choice);
		mnWindowClose(win, 2);
		mnMessageCloseFinishedWindows();
		if(choice == -1) {
			fdScriptEnd();
			return 0;
		}
		if(choice == SPRING_TOSS_COIN) {
			if(coins == 0) {
				fdScriptSetMessageId(msg + 1);
				/* "You don't have any coins!" */
				fdScriptShowMessage(-1, 0);
				sySynch(1);
				continue;
			}
			gmPartyAdjustCoins(-1);
		} else if(choice == SPRING_TOSS_MEDAL) {
			if(medals == 0) {
				fdScriptSetMessageId(msg + 2);
				/* "You have no Lucky Medals!" */
				fdScriptShowMessage(-1, 0);
				sySynch(1);
				continue;
			}
			gmPartyInventoryRemove(ITEM_LUCKY_MEDAL);
			if(gmPartyInventoryCountFreeSlots() == 0) {
				fdScriptSetMessageId(msg + 4);
				/* "You can't carry more items. Do you still want to play?" */
				fdScriptOpenMessage(-1, 0);
				if(fdScriptAskYesNoWithActorAnimation(0, 0) != 0)
					goto end;
			}
		}
		win = mnWindowCreate(TOSS_WINDOW_X, 15, TOSS_WINDOW_WIDTH, 4, 2);
		result = (Sint32)MsgLemuriaSpringToss;
		mnTextDrawMessageAsTiles(result, win, 0, 0);
		mnTextDrawMessageAsTiles(result + 1, win, 0, 8);
		sySynch(5);
		fdScriptPlaySnd(SE_TOLBI_SPRING_TOSS_PROMPT);
		for(;;) {
			if(PerWork.push & KEY_A) {
				fdScriptPlaySnd(SE_MENU_CONFIRM);
				result = 0;
				break;
			}
			if(PerWork.push & KEY_B) {
				fdScriptPlaySnd(SE_MENU_CANCEL);
				result = cancelled;
				break;
			}
			sySynch(1);
		}
		mnWindowClose(win, 2);
		break;
	}
#endif
	if(result != cancelled) {
#if LEMURIA_SPRING_PAYS_ON_CONFIRM
		if(choice == SPRING_TOSS_COIN) {
			gmPartyAdjustCoins(-1);
		} else if(choice == SPRING_TOSS_MEDAL) {
			gmPartyInventoryRemove(ITEM_LUCKY_MEDAL);
		}
#endif
		result = scLemuriaSpringRunToss(choice);
		if(choice == SPRING_TOSS_COIN) {
			if(result != TOSS_RING_MISSED) {
				gmPartyAdjustCoins(LemuriaSpringCoinPrizes[result]);
				fdScriptPlaySnd(MUS_LUCKY);
				mnMessageArgumentPush(
					LemuriaSpringCoinPrizes[result], MESSAGE_ARGUMENT_NUMBER);
				fdScriptSetMessageId((Sint32)MsgLemuriaSpringYouWonCoin);
				/* "You won ... coin!" */
				fdScriptShowMessage(-1, 0);
			} else {
				fdScriptPlaySnd(SE_MENU_CANCEL);
				syFieldScriptWait(10);
			}
		} else {
			total = 0;
			if(result == TOSS_RING_MIDDLE &&
				fdScriptGetFlag(FLAG_LEMURIA_SPRING_ECLIPSE_WON) == 0) {
				fdScriptSetFlag(FLAG_LEMURIA_SPRING_ECLIPSE_WON);
				scLemuriaSpringGivePrize(PRIZE_ECLIPSE);
			} else {
				for(prize = 0; prize <
					result * MEDAL_PRIZES_PER_RING + MEDAL_PRIZES_PER_RING;
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
				scLemuriaSpringGivePrize(LemuriaSpringMedalPrizes[prize]);
				if(WEIGHT(prize) > 1)
					MainRoutineStateWork.prizeWeights[prize] =
						WEIGHT(prize) / 2;
			}
		}
	}
end:
	fdScriptEnd();
	return 0;
}

/* Draws one picture of the prize being brought up in the middle of the spring,
   with SE_CAST, and holds it 8 ticks. */
static inline void scLemuriaSpringShowPrizePicture(
	Sint32 srcColumn, Sint32 srcRow)
{
	fdMapGridCopyMetatilesTo(srcColumn, srcRow, 70, 0, 3, 8);
	fdScriptPlaySnd(SE_CAST);
	syFieldScriptWait(8);
}

/* A medal's prize comes up out of the spring as at Tolbi's: a roar, the leader
   turns to the spring, and the prize is brought up to him, an item, or, for the
   first medal to land in the middle (FLAG_LEMURIA_SPRING_ECLIPSE_WON), the
   summon Eclipse, read from a tablet.

   What rises is the pictures: each is 3 by 8 metatiles copied over the spring's
   middle from spare metatiles of the map, one every 8 ticks, in order as it
   comes and back in reverse as it goes. */
void scLemuriaSpringGivePrize(Sint32 item)
{
	ACTOR *prize;

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	syFieldScriptWait(30);
	fdScriptPlaySnd(SE_BOSS_ROAR);
	syFieldScriptWait(100);
	fdActorFaceDirection(ACTOR_ISAAC, FACING_NORTH, 0);
	syFieldScriptWait(40);
	fdMapGridCopyMetatilesTo(82, 20, 70, 0, 3, 8);
	syFieldScriptWait(3);
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
	fdMapGridCopyMetatilesTo(100, 29, 70, 0, 3, 8);
	fdScriptPlaySnd(SE_CAST);
	syFieldScriptWait(70);
	fdScriptPlaySnd(SE_RECOVERY);
	if(item == PRIZE_ECLIPSE) {
		prize = fdActorGet(ACTOR_LEMURIA_SPRING_SUMMON_TABLET);
		fdActorPlaceOnActor(ACTOR_LEMURIA_SPRING_SUMMON_TABLET,
			MainRoutineStateWork.leaderActorId);
		prize->motionFlg = 0;
		prize->y.fixed = toFIXED(32);
		scFieldActorSetAnimation(
			MainRoutineStateWork.leaderActorId, ANIM_LEADER_ARMS_RAISED);
		fdSummonTabletPlayRunes(ACTOR_LEMURIA_SPRING_SUMMON_TABLET);
		mnSummonShowLearnedMessage(SUMMON_ECLIPSE);
		gmSummonLearn(SUMMON_ECLIPSE);
		fdActorSetPos(ACTOR_LEMURIA_SPRING_SUMMON_TABLET, 0, 0);
		scFieldActorSetAnimation(
			MainRoutineStateWork.leaderActorId, ANIM_STAND);
	} else {
		Call2(fdItemDispShowItem, item,
			ITEM_DISPLAY_SPARKLES | ITEM_DISPLAY_RAISED);
		gmPartyGiveItem(item, 0);
		syFieldScriptWait(20);
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
	fdScriptEnd();
}

/* Sets an actor of the toss where the simulation has it: its position, facing,
   colour mode and animation step. */
void scLemuriaSpringSetActorAppearance(
	Sint32 id, Sint32 *pos, Sint32 facing, Sint32 colMode, Sint32 step)
{
	ACTOR		   *actor = fdActorGet(id);
	SPR			   *spr;
	SPR_ANIMATION **entry;
	Sint32			i;

	if(actor != NULL) {
		actor->x.fixed = *pos++;
		actor->y.fixed = *pos++;
		actor->z.fixed = *pos;
		actor->facing = facing;
		actor->motionFlg = 0;
		((SPR *)actor->spr)->flg = 0;
		fdActorSetAnimationSpeed(actor, step);
	}
	spr = actor->spr;
	i = 0;
	if(i < spr->cnt) {
		entry = spr->entries;
		for(; i < spr->cnt; i++) {
			SPR_ANIMATION *anm = *entry++;

			if(anm->colMode != colMode) {
				anm->colMode = colMode;
				anm->picture = SPRITE_PICTURE_NONE;
			}
		}
	}
}

/* Sets how actor `id` keeps its sprites (ACTOR.sprStorageKind). */
void scLemuriaSpringSetSprStorageKind(Sint32 id, Sint32 kind)
{
	ACTOR *actor;

	actor = fdActorGetById(id);
	if(actor != 0) {
		actor->sprStorageKind = kind;
	}
}

/* An event: moves the tossed coin or medal a tick, its trail and shadow behind
   it, and the crabs and turtles that swim in the spring. */
void scLemuriaSpringUpdateTossSimulation(void)
{
	SPRING_TOSS_MOTION *toss;
	SPRING_OBSTACLE	   *obstacle;
	Sint32				idx;
	Sint32				height;
	FIXED				edgeInset;
	Sint32				motionStep;
	Sint32				speedMode;
	Sint32				cycleFrm;
	FIXED				minX;
	FIXED				maxX;
	FIXED				minZ;
	FIXED				maxZ;
	/* LEFTOVER(frame): an unused local the ROM's frame reserves and never
	 * touches. */
	VECTOR3 spare;

	toss = &LemuriaSpringToss;
	idx = 3;
	do {
		toss->trail[idx].x = toss->trail[idx - 1].x;
		toss->trail[idx].y = toss->trail[idx - 1].y;
		toss->trail[idx].z = toss->trail[idx - 1].z;
		idx--;
	} while(idx != 0);

	/* The token leaves the leader's hand 32 ticks into the throw. */
	if(toss->launchFrm > 31) {
		toss->trail[0].x += toss->vel.x;
		height = toss->trail[0].y + toss->vel.y;
		toss->trail[0].y = height;
		toss->trail[0].z += toss->vel.z;

		if(height <= 0) {
			toss->trail[0].y = 0;
			if(toss->vel.y != 0) {
				toss->vel.y = 0;
				if(LemuriaSpringTossKind == SPRING_TOSS_MEDAL) {
					fdActorSelectAnimation(
						fdActorGetById(ACTOR_LEMURIA_SPRING_MEDAL),
						ANIM_TOKEN_FLOAT);
				} else {
					fdActorSelectAnimation(
						fdActorGetById(ACTOR_LEMURIA_SPRING_COIN),
						ANIM_TOKEN_FLOAT);
				}
			}

			if(toss->attractionFrm > 0) {
				Sint32 deltaX;
				Sint32 deltaZ;
				Sint32 distance;

				deltaX = LEMURIA_SPRING_CENTER_X - toss->trail[0].x;
				deltaZ = LEMURIA_SPRING_CENTER_Z - toss->trail[0].z;
				deltaX >>= 8;
				deltaZ >>= 8;
				/* BUG: an exact center position leaves a zero divisor below. */
				distance = sySquartIWRAM(deltaX * deltaX + deltaZ * deltaZ);
				toss->vel.x += (toFIXED(0.1) * deltaX) / distance;
				toss->vel.z += (toFIXED(0.1) * deltaZ) / distance;
				toss->vel.x =
					toss->vel.x * PULLED_SPEED_KEPT / SPEED_KEPT_SCALE;
				toss->vel.z =
					toss->vel.z * PULLED_SPEED_KEPT / SPEED_KEPT_SCALE;
				toss->attractionFrm = toss->attractionFrm - 1;
			} else {
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
					if(LemuriaSpringTossKind == SPRING_TOSS_MEDAL) {
						fdActorSelectAnimation(
							fdActorGetById(ACTOR_LEMURIA_SPRING_MEDAL),
							ANIM_TOKEN_REST);
						scLemuriaSpringSetSprStorageKind(
							LEMURIA_SPRING_MEDAL_TRAIL(1), 0);
						scLemuriaSpringSetSprStorageKind(
							LEMURIA_SPRING_MEDAL_TRAIL(2), 0);
						scLemuriaSpringSetSprStorageKind(
							LEMURIA_SPRING_MEDAL_TRAIL(3), 0);
					} else {
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
						Sint32 deltaX = FIXEDtoINT(
							LEMURIA_SPRING_CENTER_X - toss->trail[0].x);
						Sint32 deltaZ = FIXEDtoINT(
							LEMURIA_SPRING_CENTER_Z - toss->trail[0].z);
						Sint32 distanceSquared =
							deltaX * deltaX + deltaZ * deltaZ;

						/* The target's rings end at these squared distances, in
						   square pixels, as the rings painted in the spring
						   do. */
						LemuriaSpringTossEnded = 1;
						if(distanceSquared <= 224) {
							LemuriaSpringTossResult = TOSS_RING_MIDDLE;
						} else if(distanceSquared <= 624) {
							LemuriaSpringTossResult = TOSS_RING_SECOND;
						} else if(distanceSquared <= 1088) {
							LemuriaSpringTossResult = TOSS_RING_THIRD;
						} else if(distanceSquared <= 1680) {
							LemuriaSpringTossResult = TOSS_RING_OUTER;
						} else {
							LemuriaSpringTossResult = TOSS_RING_MISSED;
						}
					}
				}
			}

			/* The basin's walls: 48 to 192 pixels across and 24 to 120 down,
			   its corners cut along slopes of 42 across to 18 down. */
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
		} else {
			toss->vel.y -= toFIXED(1) / 4;
		}
	}

	/* The two crabs scuttle east and west; the two turtles swim round ellipses,
	   the second hiding in its shell for the last quarter of every 512
	   ticks. */
	idx = 0;
	do {
		obstacle = &LemuriaSpringObstacles[idx];

		if(obstacle->reactionFrm > 0) {
			obstacle->reactionFrm = obstacle->reactionFrm - 1;
		}
		if(obstacle->hitCooldownFrm > 0) {
			obstacle->hitCooldownFrm = obstacle->hitCooldownFrm - 1;
		}
		if(idx <= OBSTACLE_CRAB_B) {
			speedMode = obstacle->speedMode;
			motionStep = toFIXED(1);
			if(speedMode == 1) {
				motionStep = motionStep * 2;
			}
			if(speedMode == 2) {
				motionStep = motionStep * 3;
			}
			if(obstacle->reactionFrm > 0) {
				if(idx == OBSTACLE_CRAB_A) {
					fdActorSelectAnimation(
						fdActorGetById(ACTOR_LEMURIA_SPRING_CRAB_A),
						ANIM_CRAB_RAISE_CLAWS);
				} else {
					fdActorSelectAnimation(
						fdActorGetById(ACTOR_LEMURIA_SPRING_CRAB_B),
						ANIM_CRAB_RAISE_CLAWS);
				}
			} else {
				if(idx == OBSTACLE_CRAB_A) {
					fdActorSelectAnimation(
						fdActorGetById(ACTOR_LEMURIA_SPRING_CRAB_A),
						ANIM_CRAB_SCUTTLE);
				} else {
					fdActorSelectAnimation(
						fdActorGetById(ACTOR_LEMURIA_SPRING_CRAB_B),
						ANIM_CRAB_SCUTTLE);
				}
				if(obstacle->pauseOrPhase == 0) {
					Sint32 pos;

					pos = obstacle->pos.x;
					if(obstacle->heading == CRAB_HEADING_EAST) {
						obstacle->pos.x = pos + motionStep;
					} else {
						obstacle->pos.x = pos - motionStep;
					}
					if(obstacle->pos.x <= toFIXED(64)) {
						obstacle->heading = CRAB_HEADING_EAST;
						if(idx == OBSTACLE_CRAB_B) {
							obstacle->pauseOrPhase = CRAB_TURN_PAUSE_TICKS;
						}
					}
					if(obstacle->pos.x > toFIXED(176) - 1) {
						obstacle->heading = CRAB_HEADING_WEST;
						if(idx == OBSTACLE_CRAB_B) {
							obstacle->pauseOrPhase = CRAB_TURN_PAUSE_TICKS;
						}
					}
				} else {
					obstacle->pauseOrPhase = obstacle->pauseOrPhase - 1;
				}
			}
		} else if(idx == OBSTACLE_TURTLE_A) {
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
			} else {
				fdActorSelectAnimation(
					fdActorGetById(ACTOR_LEMURIA_SPRING_TURTLE_A),
					ANIM_TURTLE_SWIM);
				obstacle->pos.x = sySin(obstacle->heading) * 48 + toFIXED(112);
				obstacle->pos.z = syCos(obstacle->heading) * 40 + toFIXED(72);
				cycleFrm = (Uint16)obstacle->heading + motionStep;
				obstacle->heading = cycleFrm;
				obstacle->pauseOrPhase = obstacle->pauseOrPhase + 1;
			}
		} else {
			cycleFrm = obstacle->pauseOrPhase & 511;
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
			} else if(cycleFrm <= 383) {
				obstacle->pos.x = sySin(obstacle->heading) * 52 + toFIXED(112);
				obstacle->pos.z = syCos(obstacle->heading) * 24 + toFIXED(72);
				obstacle->heading = obstacle->heading + motionStep;
				fdActorSelectAnimation(
					fdActorGetById(ACTOR_LEMURIA_SPRING_TURTLE_B),
					ANIM_TURTLE_SWIM);
			} else {
				fdActorSelectAnimation(
					fdActorGetById(ACTOR_LEMURIA_SPRING_TURTLE_B),
					ANIM_TURTLE_HIDE);
			}
			obstacle->pauseOrPhase = obstacle->pauseOrPhase + 1;
		}

		/* A creature the token drifts against knocks it away at 3 pixels a
		   tick and shortens the rings' pull; it flinches and speeds up a
		   step. */
		if(obstacle->hitCooldownFrm == 0 && toss->trail[0].y == 0) {
			Sint32 distanceSquared;
			Sint32 deltaX;
			Sint32 deltaZ;

			deltaX = FIXEDtoINT(obstacle->pos.x - toss->trail[0].x);
			deltaZ = FIXEDtoINT(obstacle->pos.z - toss->trail[0].z);
			distanceSquared = deltaX * deltaX + deltaZ * deltaZ;
			if(distanceSquared <= 119 && toss->attractionFrm > 30) {
				Sint32 nextMode;
				FIXED  speed;

				speed = toFIXED(3);
				if(idx <= OBSTACLE_CRAB_B) {
					if(obstacle->heading == CRAB_HEADING_EAST) {
						if(toss->vel.x < speed) {
							toss->vel.x = speed;
							toss->attractionFrm = toss->attractionFrm - 100;
						}
					} else {
						if(toss->vel.x > -speed) {
							toss->vel.x = -speed;
							toss->attractionFrm = toss->attractionFrm - 100;
						}
					}
				} else {
					Sint32 distance;

					/* BUG: coincident positions leave a zero divisor below. */
					distance = sySquartIWRAM(distanceSquared);
					toss->vel.x = -deltaX * speed / distance;
					toss->vel.z = -deltaZ * speed / distance;
					toss->attractionFrm = toss->attractionFrm - 100;
				}
				fdScriptPlaySnd(SE_TOLBI_SPRING_OBSTACLE_HIT);
				nextMode = (obstacle->speedMode + 1) % 3;
				obstacle->reactionFrm = 36;
				obstacle->speedMode = nextMode;
				obstacle->hitCooldownFrm = 30;
			}
		}

		switch(idx) {
		case OBSTACLE_CRAB_A:
			scLemuriaSpringSetActorAppearance(ACTOR_LEMURIA_SPRING_CRAB_A,
				&obstacle->pos.x, 0,
				LemuriaSpringCrabColorModes[obstacle->speedMode],
				ANIM_STEP_NORMAL * (obstacle->speedMode + 1));
			break;
		case OBSTACLE_CRAB_B:
			scLemuriaSpringSetActorAppearance(ACTOR_LEMURIA_SPRING_CRAB_B,
				&obstacle->pos.x, 0,
				LemuriaSpringCrabColorModes[obstacle->speedMode],
				ANIM_STEP_NORMAL * (obstacle->speedMode + 1));
			break;
		case OBSTACLE_TURTLE_A:
			scLemuriaSpringSetActorAppearance(ACTOR_LEMURIA_SPRING_TURTLE_A,
				&obstacle->pos.x, DEGtoANG(180) - obstacle->heading,
				LemuriaSpringTurtleColorModes[obstacle->speedMode],
				ANIM_STEP_NORMAL * (obstacle->speedMode + 1));
			break;
		case OBSTACLE_TURTLE_B:
			scLemuriaSpringSetActorAppearance(ACTOR_LEMURIA_SPRING_TURTLE_B,
				&obstacle->pos.x, TRIG_TURN - 1 - obstacle->heading,
				LemuriaSpringTurtleColorModes[obstacle->speedMode],
				ANIM_STEP_NORMAL * (obstacle->speedMode + 1));
			break;
		}
		idx++;
	} while(idx != OBSTACLE_COUNT);

	toss->shadow.x = toss->trail[0].x;
	toss->shadow.y = 0;
	toss->shadow.z = toss->trail[0].z;

	if(LemuriaSpringTossKind == SPRING_TOSS_MEDAL) {
		scLemuriaSpringSetActorAppearance(ACTOR_LEMURIA_SPRING_MEDAL,
			&toss->trail[0].x, 0, 0, ANIM_STEP_NORMAL);
		scLemuriaSpringSetActorAppearance(ACTOR_LEMURIA_SPRING_MEDAL_SHADOW,
			&toss->shadow.x, 0, 0, ANIM_STEP_NORMAL);
		scLemuriaSpringSetActorAppearance(LEMURIA_SPRING_MEDAL_TRAIL(1),
			&toss->trail[1].x, 0, 0, ANIM_STEP_NORMAL);
		scLemuriaSpringSetActorAppearance(LEMURIA_SPRING_MEDAL_TRAIL(2),
			&toss->trail[2].x, 0, 0, ANIM_STEP_NORMAL);
		scLemuriaSpringSetActorAppearance(LEMURIA_SPRING_MEDAL_TRAIL(3),
			&toss->trail[3].x, 0, 0, ANIM_STEP_NORMAL);
		fdActorSelectAnimation(
			fdActorGetById(LEMURIA_SPRING_MEDAL_TRAIL(1)), ANIM_TOKEN_GLINT);
		fdActorSelectAnimation(
			fdActorGetById(LEMURIA_SPRING_MEDAL_TRAIL(2)), ANIM_TOKEN_GLINT);
		fdActorSelectAnimation(
			fdActorGetById(LEMURIA_SPRING_MEDAL_TRAIL(3)), ANIM_TOKEN_GLINT);
	} else {
		scLemuriaSpringSetActorAppearance(ACTOR_LEMURIA_SPRING_COIN,
			&toss->trail[0].x, 0, 0, ANIM_STEP_NORMAL);
		scLemuriaSpringSetActorAppearance(ACTOR_LEMURIA_SPRING_COIN_SHADOW,
			&toss->shadow.x, 0, 0, ANIM_STEP_NORMAL);
		scLemuriaSpringSetActorAppearance(LEMURIA_SPRING_COIN_TRAIL(1),
			&toss->trail[1].x, 0, 0, ANIM_STEP_NORMAL);
		scLemuriaSpringSetActorAppearance(LEMURIA_SPRING_COIN_TRAIL(2),
			&toss->trail[2].x, 0, 0, ANIM_STEP_NORMAL);
		scLemuriaSpringSetActorAppearance(LEMURIA_SPRING_COIN_TRAIL(3),
			&toss->trail[3].x, 0, 0, ANIM_STEP_NORMAL);
		fdActorSelectAnimation(
			fdActorGetById(LEMURIA_SPRING_COIN_TRAIL(1)), ANIM_TOKEN_GLINT);
		fdActorSelectAnimation(
			fdActorGetById(LEMURIA_SPRING_COIN_TRAIL(2)), ANIM_TOKEN_GLINT);
		fdActorSelectAnimation(
			fdActorGetById(LEMURIA_SPRING_COIN_TRAIL(3)), ANIM_TOKEN_GLINT);
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
   the spring, and starts the event that moves them. */
void scLemuriaSpringInitTossSimulation(void)
{
	SPRING_TOSS_MOTION *toss = &LemuriaSpringToss;
	SPRING_OBSTACLE	   *obstacle;
	Sint32				i;

	for(i = 0; i != OBSTACLE_COUNT; i++) {
		obstacle = &LemuriaSpringObstacles[i];
		obstacle->pos.x = toFIXED(LemuriaSpringObstacleStartX[i]);
		obstacle->pos.y = 0;
		obstacle->pos.z = toFIXED(LemuriaSpringObstacleStartZ[i]);
		obstacle->heading = LemuriaSpringObstacleHeadings[i];
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

	fdActorSelectAnimation(
		fdActorGet(ACTOR_LEMURIA_SPRING_TURTLE_A), ANIM_TURTLE_SWIM);
	fdActorSelectAnimation(
		fdActorGet(ACTOR_LEMURIA_SPRING_TURTLE_B), ANIM_TURTLE_SWIM);

	sySetTickEvent(
		(EVENT_FUNC)scLemuriaSpringUpdateTossSimulation, EVENT_PASS_TICK + 3);
}

/* Tosses the chosen coin or medal and waits for it to come to rest; returns the
   ring it rests in. */
Sint32 scLemuriaSpringRunToss(Sint32 tossKind)
{
	SPRING_TOSS_MOTION *toss;

	toss = &LemuriaSpringToss;
	toss->trail[0].y = 0;
	toss->trail[1].y = 0;
	toss->trail[2].y = 0;
	toss->trail[3].y = 0;
	LemuriaSpringTossKind = tossKind;
	LemuriaSpringTossEnded = 0;
	toss->launchFrm = -1;
	for(LemuriaSpringTossTick = 0;; LemuriaSpringTossTick++) {
		if(LemuriaSpringTossTick == 50)
			fdScriptPlaySnd(SE_TOLBI_SPRING_TOSS);
		if(LemuriaSpringTossTick == 16) {
			fdActorSetAnimation(
				MainRoutineStateWork.leaderActorId, ANIM_LEADER_TOSS);
			toss->launchFrm = 0;
			toss->vel.x = toFIXED(13) / 10;
			toss->vel.y = toFIXED(4);
			toss->vel.z = -toFIXED(2);
			toss->trail[0].x = toFIXED(120);
			toss->trail[0].y = toFIXED(16);
			toss->trail[0].z = toFIXED(152);
			toss->attractionFrm = 300;
			if(LemuriaSpringTossKind == SPRING_TOSS_MEDAL) {
				fdActorSelectAnimation(
					fdActorGet(ACTOR_LEMURIA_SPRING_MEDAL_SHADOW),
					ANIM_TOKEN_SHADOW);
				fdActorSelectAnimation(
					fdActorGet(ACTOR_LEMURIA_SPRING_MEDAL), ANIM_TOKEN_TUMBLE);
				scLemuriaSpringSetSprStorageKind(
					LEMURIA_SPRING_MEDAL_TRAIL(1), 1);
				scLemuriaSpringSetSprStorageKind(
					LEMURIA_SPRING_MEDAL_TRAIL(2), 1);
				scLemuriaSpringSetSprStorageKind(
					LEMURIA_SPRING_MEDAL_TRAIL(3), 1);
			} else {
				fdActorSelectAnimation(
					fdActorGet(ACTOR_LEMURIA_SPRING_COIN_SHADOW),
					ANIM_TOKEN_SHADOW);
				fdActorSelectAnimation(
					fdActorGet(ACTOR_LEMURIA_SPRING_COIN), ANIM_TOKEN_TUMBLE);
				scLemuriaSpringSetSprStorageKind(
					LEMURIA_SPRING_COIN_TRAIL(1), 1);
				scLemuriaSpringSetSprStorageKind(
					LEMURIA_SPRING_COIN_TRAIL(2), 1);
				scLemuriaSpringSetSprStorageKind(
					LEMURIA_SPRING_COIN_TRAIL(3), 1);
			}
		}
		syWait(1);
		if(LemuriaSpringTossEnded == 1)
			break;
	}
	return LemuriaSpringTossResult;
}

SCENE_ENTRANCE SceneEntrances[] = {
	{0, CONDITION_ALWAYS, 288, 0, 454, FACING_NORTH, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

Uint32 SceneExits[] = {
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
		MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_SPRING_LUCKY_MEDAL, CONDITION_ALWAYS,
		MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_SPRING_LUCKY_MEDAL, CONDITION_ALWAYS,
		MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_SPRING_LUCKY_MEDAL, CONDITION_ALWAYS,
		MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_SPRING_LUCKY_MEDAL, CONDITION_ALWAYS,
		MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY, 0),
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
		MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
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
Uint16 LemuriaSpringAlphaSteps[] = {
	BLEND_ALPHA(10, 9), BLEND_ALPHA(8, 10), BLEND_ALPHA(6, 11), BLEND_ALPHA(4, 12),
	BLEND_ALPHA(3, 13), BLEND_ALPHA(2, 14), BLEND_ALPHA(1, 15),
	BLEND_ALPHA(0, BLEND_WEIGHT_MAX),
};

CUE SceneCues[] = {
	{CUE_EXIT, LEMURIA_SPRING_TO_LEMURIA, CONDITION_ALWAYS, LEMURIA_SPRING_TO_LEMURIA},
	{CUE_TALK, ACTOR_LEMURIA_SPRING_MEDAL_SIGN, CONDITION_ALWAYS, MSGNO(MsgReadTheSign)},
	{CUE_TALK, ACTOR_LEMURIA_SPRING_COIN_SIGN, CONDITION_ALWAYS, MSGNO(MsgReadTheSignThe)},
	{CUE_TALK, ACTOR_LEMURIA_SPRING_VISITOR_FACING_AWAY, CONDITION_ALWAYS, MSGNO(MsgLemuriaSpringIfYouFaceAway)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LEMURIA_SPRING_VISITOR_FACING_AWAY, CONDITION_ALWAYS, MSGNO(MsgLemuriaSpringIdLoveToTest)},
	{CUE_TALK, ACTOR_LEMURIA_SPRING_VISITOR_REMEMBERING_OUTSIDER, CONDITION_ALWAYS, MSGNO(MsgLemuriaSpringIRememberOutsiderWhoCame)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LEMURIA_SPRING_VISITOR_REMEMBERING_OUTSIDER, CONDITION_ALWAYS, MSGNO(MsgLemuriaSpringHeHadSuchGoodTime)},
	{CUE_EXAMINE, METATILE_BEHAVIOR_LEMURIA_SPRING_RIM, FLAG_LEMURIA_SPRING_TARGET_SHOWN, SCRIPT_REF(scLemuriaSpringDrinkFromSpring)},
	{CUE_EXAMINE, METATILE_BEHAVIOR_LEMURIA_SPRING_WATER, CONDITION_ALWAYS, SCRIPT_REF(scLemuriaSpringTossCoinOrMedal)},
	{CUE_TABLE_END},
};

/* The items a medal can win, drawn by the weights in MainRoutineStateWork: */
/* the middle ring draws from the first three, each ring further out from */
/* three more. */
Uint32 LemuriaSpringMedalPrizes[MEDAL_PRIZE_COUNT] = {
	ITEM_HESTIA_BLADE, ITEM_AEGIS_SHIELD, ITEM_EREBUS_ARMOR,
	ITEM_MINERVA_HELM, ITEM_MIGHTY_AXE, ITEM_FLORAL_DRESS,
	ITEM_CROWN_OF_GLORY, ITEM_LEDAS_BRACELET, ITEM_FIREMANS_POLE,
	ITEM_BRILLIANT_CIRCLET, ITEM_CRAFTED_GLOVES, ITEM_WILD_COAT,
	ITEM_POTION, ITEM_PSY_CRYSTAL, ITEM_WATER_OF_LIFE,
};

/* The coins a coin wins by where it lands, the middle first; 0 for a miss. */
Uint16 LemuriaSpringCoinPrizes[] = {
	20, 10, 2, 1, 0,
};

/* The crabs' colour modes, by speed. */
Uint8 LemuriaSpringCrabColorModes[] = {
	SPRITE_COLOR_NORMAL, SPRITE_COLOR_GRB, SPRITE_COLOR_BLUE_TINT,
};

/* The turtles' colour modes, by speed. */
Uint8 LemuriaSpringTurtleColorModes[] = {
	SPRITE_COLOR_NORMAL, SPRITE_COLOR_BGR, SPRITE_COLOR_GBR,
};

/* Where each obstacle starts, across. */
Uint8 LemuriaSpringObstacleStartX[] = {
	80, 160, 80, 72,
};

/* Where each obstacle starts, down. */
Uint8 LemuriaSpringObstacleStartZ[] = {
	32, 104, 68, 72,
};

/* The way each obstacle starts heading: the crabs east or west, the turtles */
/* by their angle round their loops. */
Uint16 LemuriaSpringObstacleHeadings[] = {
	CRAB_HEADING_EAST, CRAB_HEADING_WEST, 0, DEGtoANG(180),
};
/* clang-format on */
