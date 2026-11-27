/* Tolbi-bound Ship (SceneId_TolbiShipMast) is the top of the ship's mast, where
   the ship's lookout (ACTOR_LOOKOUT) keeps watch.

   It is one of the ship's three scenes on its voyage from Kalay to Tolbi, which
   tolbi_ship_deck.c describes.

   The party climbs up by the deck's ladder
   (TOLBI_SHIP_MAST_FROM_TOLBI_SHIP_DECK). It climbs down again by
   METATILE_BEHAVIOR_TOLBI_SHIP_MAST_LADDER and the region of
   TOLBI_SHIP_MAST_TO_TOLBI_SHIP_DECK.

   The entrances from TOLBI_SHIP_MAST_FROM_TOLBI_SHIP_DECK_COURSE_CLEAR on are
   the voyage's scripts here, each leading on into the next. During the attacks
   the restart point (MainRoutineStateWork.restartScene) is set to
   SHIP_ROOMS_PASSENGER_HOLD_AFTER_DEFEAT, which starts the voyage over.

   While the ship is under way, the setup rocks the view. The ship is under way
   when it is rowing, or at sea after the first attack, and neither at the
   island nor at Tolbi.

   In story order:

   - At Kalay, while the ship waits for its replacement, the captain's anchor
	 charm, gone from his quarters, lies up here. Searching for it
	 (METATILE_BEHAVIOR_TOLBI_SHIP_MAST_ANCHOR_CHARM) finds the Anchor Charm
	 (FLAG_TOLBI_SHIP_ANCHOR_CHARM_FOUND). It does so only once the party has
	 confronted the crewman on deck or the warriors have gone to the captain
	 (FLAG_TOLBI_SHIP_CREWMAN_CAUGHT or FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN).
	 The lookout is the crewman who hid it. Until the first attack, Mind Read
	 on him has him wonder whether the party has worked out that he was the
	 one. Once the charm is back the setup moves him to his lookout spot.
   - Under sail, at TOLBI_SHIP_MAST_FROM_TOLBI_SHIP_DECK_COURSE_CLEAR, he scans
	 both sides and calls the course clear, which leads below to the cast-off.
	 The first time, the leader is moved out of view. On later departures he
	 stands by.
   - The attacks. TOLBI_SHIP_MAST_FROM_WORLD_MAP_FIRST_ATTACK is "Avast!
	 Monsters sighted!", as three monsters appear where a sparkle twinkles and
	 close in (FLAG_TOLBI_SHIP_MONSTERS_SIGHTED, which plays
	 MUS_THE_SLEEPING_TREASURE_OF_ALTMILLER_CAVE). Then comes Sean's
	 "Monsters!" after a bird swoops at the lookout
	 (TOLBI_SHIP_MAST_FROM_WORLD_MAP_SECOND_ATTACK). Last is a monster dropping
	 in beside the lookout (TOLBI_SHIP_MAST_FROM_WORLD_MAP_THIRD_ATTACK). Each
	 leads below or to the deck.
   - Land. At TOLBI_SHIP_MAST_FROM_WORLD_MAP_LAND_HO the seven birds
	 (ACTOR_FIRST_BIRD) fly away before "Land ho!". The rowing values of the
	 passengers chosen to row (TolbiShipPassengerRowValues) decide where
	 the ship comes in. The balance is those at the first and third seats less
	 those at the second and fourth. A balance of ROWING_BALANCE_OFF_COURSE
	 takes TOLBI_SHIP_MAST_TO_TOLBI_SHIP_DECK_OFF_COURSE, off course to the
	 island.
	 Any other takes TOLBI_SHIP_MAST_TO_TOLBI_SHIP_DECK_LAND_SIGHTED and Tolbi.
	 Once the rowers have switched sides at the island the balance is the
	 opposite, so the ship comes in to Tolbi.

   At sea the lookout says he is the lookout, not an oarsman. At the island he
   warns of monsters and wonders at its treasures. */

#include "ability_ids.h"
#include "item_ids.h"
#include "macros.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/actor_script.h"
#include "field/script_inline.h"
#include "field/map_grid_scroll.h"
#include "graphics/graphics.h"
#include "scene/tolbi_ship.h"
#include "text/msg_ids.h"

extern Sint32			 TolbiShipMastSparkleActions[];
extern Sint32			 TolbiShipMastLookoutActions[];
extern Sint32			 TolbiShipMastDriftActions[];
extern Sint32			 ActorTrembleScript[];
extern Sint32			 TolbiShipMastBirdDepartureA[];
extern Sint32			 TolbiShipMastBirdDepartureB[];
extern Sint32			 TolbiShipMastBirdDepartureC[];
extern Sint32			 TolbiShipMastBirdDepartureD[];
extern Sint32			 TolbiShipMastBirdDepartureE[];
extern Sint32			 TolbiShipMastBirdDepartureF[];
extern Uint32			 TolbiShipMastBirdDepartureG[];
extern SCENE_ENTRANCE	 TolbiShipMastEntrances[];
extern SCENE_EXIT_REGION TolbiShipMastRegions[];
extern Uint32			 TolbiShipMastExits[];
extern ACTOR_PLACE		 TolbiShipMastActorPlacements[];
extern ACTOR_PLACE		 TolbiShipMastFirstAttackActorPlacements[];
extern ACTOR_PLACE		 TolbiShipMastBirdActorPlacements[];
extern ACTOR_PLACE		 TolbiShipMastLandingActorPlacements[];
extern CUE				 TolbiShipMastCues[];

static void	  scTolbiShipMastWatchCourseFirstTime(void);
static void	  scTolbiShipMastWatchCourseAgain(void);
static void	  scTolbiShipMastAnnounceClearCourse(void);
static void	  scTolbiShipMastPlayFirstAttack(void);
static void	  scTolbiShipMastPlaySecondAttack(void);
static void	  scTolbiShipMastStartBirdDrifting(Sint32 actorId);
static void	  scTolbiShipMastPlayThirdAttack(void);
static void	  scTolbiShipMastAnnounceLand(void);
static void	  scTolbiShipMastRockView(void);
static Sint32 scTolbiShipMastGetRowingBalance(void);
static Sint32 scTolbiShipMastGetSeatRowValue(Uint32 seat);

/* How far the rocking has moved the view in all (scTolbiShipMastRockView),
   which nothing reads back, and the angles of its swing up and down and
   across. */
Sint32 TolbiShipMastRockOffsetY BSS_DATA ALIGNED(16);
Sint32 TolbiShipMastRockOffsetX			 BSS_DATA;
Sint32 TolbiShipMastRockAngleY			 BSS_DATA;
/* Unused: nothing reads or writes it. */
Sint32 TolbiShipMastRockUnused BSS_DATA;
/* Where the sparkle appears, and where the camera was then: the sparkle moves
   with the rocking camera across, and half as far up and down
   (scTolbiShipMastTurnSparkle). */
Sint32						   TolbiShipMastCameraCenter[2] BSS_DATA;
Sint32						   TolbiShipMastCameraOrigin[2] BSS_DATA;
Sint32 TolbiShipMastRockAngleX BSS_DATA;

/* The scene's one actor placement row. */
typedef enum TOLBI_SHIP_MAST_ACTOR_ID {
	/* The ship's lookout is a man in a blue bandana
	   (SPRITE_MAN_IN_BLUE_BANDANA).

	   He is the crewman who hid the captain's anchor charm (ACTOR_KALAY_CREWMAN
	   on deck, ACTOR_QUARTERS_CREWMAN below), in the same sprite. Mind Read on
	   him at Kalay hears "I was the one who hid the anchor"
	   (MsgTolbiShipHiddenAnchorDiscovered). */
	ACTOR_LOOKOUT = ACTOR_FIRST_PLACED
} TOLBI_SHIP_MAST_ACTOR_ID;

/* The first attack's actors (TOLBI_SHIP_MAST_FROM_WORLD_MAP_FIRST_ATTACK). They
   are placed after the lookout from
   TolbiShipMastFirstAttackActorPlacements. */
typedef enum TOLBI_SHIP_MAST_FIRST_ATTACK_ACTOR_ID {
	/* A sparkle, placed nowhere else. Its picture is graphics/sprite/glint.png,
	   a four-pointed sparkle in eight shrinking ticks.

	   It turns, swells for 10 ticks and shrinks for 24
	   (TolbiShipMastSparkleActions) where the monsters then appear. */
	ACTOR_SPARKLE = ACTOR_LOOKOUT + 1,
	/* The first of three monsters, all with the sprite of the first attack's
	   monsters below decks (ShipRoomsFirstAttackMonsters). They grow from half
	   size as they close in. */
	ACTOR_FIRST_SIGHTED_MONSTER
} TOLBI_SHIP_MAST_FIRST_ATTACK_ACTOR_ID;

/* The actors of the later scripts, from
   TOLBI_SHIP_MAST_FROM_WORLD_MAP_SECOND_ATTACK on.

   They are placed after the lookout from TolbiShipMastBirdActorPlacements and,
   at TOLBI_SHIP_MAST_FROM_WORLD_MAP_THIRD_ATTACK, then from
   TolbiShipMastLandingActorPlacements. */
typedef enum TOLBI_SHIP_MAST_ENSEMBLE_ACTOR_ID {
	/* The first of seven birds, all of one sprite (SPRITE_BIRD, a white bird in
	   three wing poses). They flutter and bob about the mast
	   (scTolbiShipMastDriftBird).

	   One swoops at the lookout at the second attack. They scatter as the third
	   attack's monster comes, and they fly off just before "Land ho!". */
	ACTOR_FIRST_BIRD = ACTOR_LOOKOUT + 1,
	/* Sean, by the sprite he has in the hold (ACTOR_HOLD_SEAN, ship_rooms.c)
	   and on deck (ACTOR_SEA_SEAN, tolbi_ship_deck.c). He cries "Monsters!" at
	   the second attack. */
	ACTOR_SEAN = ACTOR_FIRST_BIRD + 7,
	/* The sprite of the third attack's monster below decks
	   (ShipRoomsThirdAttackMonsters). It drops in beside the lookout. */
	ACTOR_LANDING_MONSTER,
	/* The landing monster's shadow. A blank sprite (SPRITE_BLANK) draws only
	   its shadow, which grows from a tenth of its size where
	   ACTOR_LANDING_MONSTER drops, then follows the monster away.
	   tolbi_ship_deck.c's ACTOR_SEA_FALLING_MONSTER_SHADOW does the same. */
	ACTOR_LANDING_MONSTER_SHADOW
} TOLBI_SHIP_MAST_ENSEMBLE_ACTOR_ID;

/* The mast's step cue and examine cue metatiles. */
typedef enum TOLBI_SHIP_MAST_METATILE_BEHAVIOR {
	/* the top of the ladder down to the deck */
	METATILE_BEHAVIOR_TOLBI_SHIP_MAST_LADDER = 10,
	/* Where the crewman hid the captain's anchor charm. */
	METATILE_BEHAVIOR_TOLBI_SHIP_MAST_ANCHOR_CHARM = 11
} TOLBI_SHIP_MAST_METATILE_BEHAVIOR;

/* Each tick of the sparkle on the sea: it keeps its place against the rocking
   view and turns. */
static Sint32 scTolbiShipMastTurnSparkle(ACTOR *actor)
{
	Sint32 *camera = ((MAP_GRID_SCROLL_STATE *)MapGridState[0])->origin;
	Sint32	cameraX = *camera++;
	Sint32	cameraY = *camera;

	actor->x.fixed =
		TolbiShipMastCameraCenter[0] + (cameraX - TolbiShipMastCameraOrigin[0]);
	actor->y.fixed = TolbiShipMastCameraCenter[1] +
		(cameraY - TolbiShipMastCameraOrigin[1]) / 2;
	actor->spr->rotation += TRIG_TURN * 3 / 128;
	return 0;
}

/* The lookout's watch: now and then (two chances in 64 a tick) he turns to
   look a little east or a little west of north. */
static Sint32 scTolbiShipMastTurnLookoutAtRandom(ACTOR *actor)
{
	Uint32 sample = syRandomBelow(64);

	if(sample == 6) {
		actor->facing = FACING_NORTH_NORTHEAST;
	} else if(sample == 9) {
		actor->facing = FACING_NORTH_NORTHWEST;
	}

	return 1;
}

/* A sighted monster grows as it closes in, until it is full size. */
static void scTolbiShipMastGrowActorScale(ACTOR *actor)
{
	if(actor->scaleX < toFIXED(1)) {
		actor->scaleX += toFIXED(5) / 2048;
		actor->scaleY += toFIXED(5) / 2048;
	}
}

/* Which way scTolbiShipMastDriftBird moves a bird along each axis. It is kept
   in the actor's timer (the drift) and its userValue (the bob). */
typedef enum TOLBI_SHIP_MAST_DRIFT {
	DRIFT_INCREASING = 0, /* the drift speeding up, or the bob rising */
	DRIFT_DECREASING = 1, /* the drift slowing, or the bob sinking */
	DRIFT_HELD = 9		  /* held at zero; nothing here sets it */
} TOLBI_SHIP_MAST_DRIFT;

/* Each tick of a bird about the mast: it drifts east and west, speeding up and
   slowing by random amounts, and bobs up and down. */
static Sint32 scTolbiShipMastDriftBird(ACTOR *actor)
{
	/* The drift: sceneWord is the speed added to x each tick. */
	if(actor->tmr == DRIFT_HELD) {
		actor->sceneWord = 0;
	} else if(actor->tmr != DRIFT_INCREASING) {
		actor->sceneWord -= syRandomBelow(2048);
		if(actor->sceneWord < -toFIXED(3) / 4) {
			actor->tmr = DRIFT_INCREASING;
		}
	} else {
		actor->sceneWord += syRandomBelow(2048);
		if(actor->sceneWord > toFIXED(3) / 4) {
			actor->tmr = DRIFT_DECREASING;
		}
	}
	if(actor->x.fixed > toFIXED(40) && actor->x.fixed < toFIXED(320)) {
		actor->x.fixed += actor->sceneWord;
	}

	/* The bob, between the deck and 16 pixels up. */
	if(actor->userValue == DRIFT_HELD) {
		actor->y.fixed = 0;
	} else if(actor->userValue != DRIFT_INCREASING) {
		actor->y.fixed -= syRandomBelow(toFIXED(3) / 4);
		if(actor->y.fixed < 0) {
			actor->userValue = DRIFT_INCREASING;
		}
	} else {
		actor->y.fixed += syRandomBelow(toFIXED(3) / 4);
		if(actor->y.fixed > toFIXED(16)) {
			actor->userValue = DRIFT_DECREASING;
		}
	}
	return 1;
}

/* A bird flies off into the distance, growing smaller each tick; once small
   enough it is put away at the origin, standing still. */
static Sint32 scTolbiShipMastFlyBirdAway(ACTOR *actor)
{
	SPR *sprite = actor->spr;

	sprite->image.pri = 3;
	actor->speedLimit = toFIXED(2);
	actor->acceleration = toFIXED(1);
	if(actor->scaleX > toFIXED(1) / 16) {
		actor->scaleX -= toFIXED(1) / 64;
		actor->scaleY -= toFIXED(1) / 64;
	} else {
		actor->x.fixed = 0;
		actor->y.fixed = 0;
		actor->z.fixed = 0;
		SET_VELOCITY(actor, 0, 0, 0);
	}
	return 1;
}

/* Returns the entrances, which are the one from the deck's ladder and the
   voyage's scripts. */
const SCENE_ENTRANCE *scGetEntrances(void)
{
	return TolbiShipMastEntrances;
}

/* Returns the top of the ladder, where climbing down takes the party to the
   deck. */
const SCENE_EXIT_REGION *scGetExitRegions(void)
{
	return TolbiShipMastRegions;
}

/* Returns exits to the deck and the continuing ship scenes. */
const Uint32 *scGetExits(void)
{
	return TolbiShipMastExits;
}

/* Returns the lookout, the only actor placed. */
const ACTOR_PLACE *scGetActorPlacements(void)
{
	return TolbiShipMastActorPlacements;
}

/* Returns the ladder, lookout dialogue and Anchor Charm examine cues. */
const CUE *scGetCues(void)
{
	return TolbiShipMastCues;
}

/* Walking south onto the ladder at the mast's top
   (METATILE_BEHAVIOR_TOLBI_SHIP_MAST_LADDER) has the leader climb down. */
static void scTolbiShipMastClimbDownMast(void)
{
	fdClimbDown();
}

/* Searching the mast's top for the captain's anchor charm
   (METATILE_BEHAVIOR_TOLBI_SHIP_MAST_ANCHOR_CHARM) gives the Anchor Charm once
   the crewman has been confronted or the warriors have gone to the captain. */
static void scTolbiShipMastFindAnchorCharm(void)
{
	if(gmFlagGet(FLAG_TOLBI_SHIP_CREWMAN_CAUGHT) != 0 ||
		gmFlagGet(FLAG_TOLBI_SHIP_WARRIORS_SEEK_CAPTAIN) != 0) {
		fdScriptStart();
		gmItemShowFound(ITEM_ANCHOR_CHARM, 3);
		gmPartyGiveItem(ITEM_ANCHOR_CHARM, 0);
		gmFlagSet(FLAG_TOLBI_SHIP_ANCHOR_CHARM_FOUND);
		fdScriptEnd();
	}
}

/* The mast's setup rocks the view while the ship is under way, puts the lookout
   to his spot once the charm is back, and plays what the entrance calls for. */
Sint32 scInit(void)
{
	ACTOR *leader;

	gmFlagSet(FLAG_RETREAT_BLOCKED);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
	if((gmFlagGet(FLAG_TOLBI_SHIP_ROWING) != 0 ||
		   gmFlagGet(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) != 0) &&
		gmFlagGet(FLAG_TOLBI_SHIP_REACHED_TOLBI) == 0 &&
		gmFlagGet(FLAG_TOLBI_SHIP_AT_ISLAND) == 0) {
		TolbiShipMastRockAngleX = (Uint16)syRandomNext();
		TolbiShipMastRockAngleY = (Uint16)syRandomNext();
		sySetTickEvent(scTolbiShipMastRockView, EVENT_PASS_TICK);
	}
	if(gmFlagGet(FLAG_TOLBI_SHIP_ANCHOR_CHARM_RETURNED) != 0 &&
		gmFlagGet(FLAG_TOLBI_SHIP_REACHED_TOLBI) == 0) {
		fdActorSetPos(ACTOR_LOOKOUT, 164, 328);
	}
	/* Each of the voyage's entrances plays its script. */
	switch(MainRoutineStateWork.entrance) {
	case TOLBI_SHIP_MAST_FROM_TOLBI_SHIP_DECK:
		if(gmFlagGet(FLAG_KEEP_PARTY_POSITION) == 0) {
			leader = fdActorGet(ACTOR_PARTY_LEADER);
			fdScriptStart();
			fdClimbUp();
			leader->y.fixed = toFIXED(56);
			fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
			sySynch(1);
			fdCameraFollowActor(ACTOR_PARTY_LEADER, 0);
			fdMapGridCenterOnFollowedPoint();
			sySynch(1);
			fdScriptEnd();
		}
		break;
	case TOLBI_SHIP_MAST_FROM_TOLBI_SHIP_DECK_COURSE_CLEAR:
		if(gmFlagGet(FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF) != 0) {
			scTolbiShipMastWatchCourseAgain();
		} else {
			scTolbiShipMastWatchCourseFirstTime();
		}
		break;
	case TOLBI_SHIP_MAST_FROM_WORLD_MAP_FIRST_ATTACK:
		MainRoutineStateWork.restartScene = SCENE_SHIP_ROOMS;
		MainRoutineStateWork.restartEntrance =
			SHIP_ROOMS_PASSENGER_HOLD_AFTER_DEFEAT;
		scTolbiShipMastPlayFirstAttack();
		break;
	case TOLBI_SHIP_MAST_FROM_WORLD_MAP_SECOND_ATTACK:
		MainRoutineStateWork.restartScene = SCENE_SHIP_ROOMS;
		MainRoutineStateWork.restartEntrance =
			SHIP_ROOMS_PASSENGER_HOLD_AFTER_DEFEAT;
		scTolbiShipMastPlaySecondAttack();
		break;
	case TOLBI_SHIP_MAST_FROM_WORLD_MAP_THIRD_ATTACK:
		MainRoutineStateWork.restartScene = SCENE_SHIP_ROOMS;
		MainRoutineStateWork.restartEntrance =
			SHIP_ROOMS_PASSENGER_HOLD_AFTER_DEFEAT;
		scTolbiShipMastPlayThirdAttack();
		break;
	case TOLBI_SHIP_MAST_FROM_WORLD_MAP_LAND_HO:
		scTolbiShipMastAnnounceLand();
		break;
	}
	return 0;
}

/* The first time the ship sails, the view is set on the lookout alone, with the
   leader taken off the mast, before he calls the course clear. */
static void scTolbiShipMastWatchCourseFirstTime(void)
{
	fdScriptStart();
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	sySynch(1);
	fdCameraGetActor()->motionFlg = 0;
	fdCameraMoveTo(164, 64, 321, 0);
	fdMapGridCenterOnFollowedPoint();
	sySynch(1);
	fdActorSetPos(ACTOR_PARTY_LEADER, 0, 0);
	scTolbiShipMastAnnounceClearCourse();
	fdScriptEnd();
}

/* On later departures the leader stands by, unseen, while the lookout calls the
   course clear. */
static void scTolbiShipMastWatchCourseAgain(void)
{
	fdScriptStart();
	fdActorSetPos(ACTOR_PARTY_LEADER, 164, 321);
	fdActorSetSprColById(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
	fdActorSetSprFlags(fdActorGet(ACTOR_PARTY_LEADER), 0);
	sySynch(1);
	fdMapGridCenterOnFollowedPoint();
	sySynch(1);
	scTolbiShipMastAnnounceClearCourse();
	fdScriptEnd();
}

/* On the mast at TOLBI_SHIP_MAST_FROM_TOLBI_SHIP_DECK_COURSE_CLEAR, the lookout
   scans both sides and announces a clear course. */
static void scTolbiShipMastAnnounceClearCourse(void)
{
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptWait(20);
	fdActorSetSpeed(ACTOR_LOOKOUT, 1);
	fdActorWalkToAndWait(ACTOR_LOOKOUT, 164, 321);
	fdActorFaceDirection(ACTOR_LOOKOUT, FACING_NORTH_NORTHEAST, 40);
	fdActorFaceDirection(ACTOR_LOOKOUT, FACING_NORTH_NORTHWEST, 40);
	fdActorFaceDirection(ACTOR_LOOKOUT, FACING_NORTH_NORTHEAST, 40);
	fdActorFaceDirection(ACTOR_LOOKOUT, FACING_SOUTH_SOUTHEAST, 10);
	fdActorWalkToAndWait(ACTOR_LOOKOUT, 164, 334);
	fdActorJump(ACTOR_LOOKOUT, 4, 40);
	fdActorStartRepeatedMotion(ACTOR_LOOKOUT, 2);
	fdScriptSetMessageId(MsgTolbiShipShipsCourseClear);
	/* "Ship's course... Clear!" */
	fdScriptShowMessageThenWait(ACTOR_LOOKOUT, 0, 20);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	syImportScriptSetExitRequest(TOLBI_SHIP_MAST_TO_SHIP_ROOMS_CAST_OFF);
}

/* On the mast at TOLBI_SHIP_MAST_FROM_WORLD_MAP_FIRST_ATTACK, the first attack,
   three monsters close in and the lookout cries out. "Avast!" */
static void scTolbiShipMastPlayFirstAttack(void)
{
	Sint32 *cameraOrigin = ((MAP_GRID_SCROLL_STATE *)MapGridState[0])->origin;
	ACTOR  *actor;
	FIXED	sparkleX;
	Sint32	sparkleY = 0;

	fdScriptStart();
	fdScriptAppendActorPlacements(TolbiShipMastFirstAttackActorPlacements);
	sySynch(1);
	fdActorSetSprColById(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
	fdActorSetSprFlags(fdActorGet(ACTOR_PARTY_LEADER), 0);
	fdActorSetScript(ACTOR_LOOKOUT, TolbiShipMastLookoutActions);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_CIRCLE);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptWait(20);
	TolbiShipMastCameraOrigin[0] = *cameraOrigin++;
	TolbiShipMastCameraOrigin[1] = *cameraOrigin;
	fdActorSetPos(ACTOR_SPARKLE, 80, 210);
	sparkleX = toFIXED(80);
	fdActorGet(ACTOR_SPARKLE)->motionFlg = 0;
	TolbiShipMastCameraCenter[0] = sparkleX;
	TolbiShipMastCameraCenter[1] = sparkleY;
	fdActorSetScript(ACTOR_SPARKLE, TolbiShipMastSparkleActions);
	fdScriptWait(20);
	sySndPlay(MUS_THE_SLEEPING_TREASURE_OF_ALTMILLER_CAVE);
	gmFlagSet(FLAG_TOLBI_SHIP_MONSTERS_SIGHTED);
	fdActorStop(ACTOR_LOOKOUT);
	sySynch(1);
	fdActorShowEmote(ACTOR_LOOKOUT, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorFaceDirection(ACTOR_LOOKOUT, FACING_NORTH_NORTHWEST, 0);
	fdScriptSetMessageId(MsgTolbiShipAvast);
	/* "Avast!" */
	fdScriptShowMessageThenWait(ACTOR_LOOKOUT, 0, 10);
	/* The three monsters appear where the sparkle turned. */
	fdActorSetPosFX(ACTOR_FIRST_SIGHTED_MONSTER, sparkleX, toFIXED(210));
	fdActorSetPosFX(ACTOR_FIRST_SIGHTED_MONSTER + 1, sparkleX, toFIXED(210));
	fdActorSetPosFX(ACTOR_FIRST_SIGHTED_MONSTER + 2, sparkleX, toFIXED(210));
	fdActorSetSprPriority(ACTOR_FIRST_SIGHTED_MONSTER, 3);
	fdActorSetSprPriority(ACTOR_FIRST_SIGHTED_MONSTER + 1, 3);
	fdActorSetSprPriority(ACTOR_FIRST_SIGHTED_MONSTER + 2, 3);
	fdActorSetSprColById(ACTOR_FIRST_SIGHTED_MONSTER, SPRITE_COLOR_BGR);
	fdActorSetSprColById(ACTOR_FIRST_SIGHTED_MONSTER + 1, SPRITE_COLOR_BGR);
	fdActorSetSprColById(ACTOR_FIRST_SIGHTED_MONSTER + 2, SPRITE_COLOR_BGR);
	actor = fdActorGet(ACTOR_FIRST_SIGHTED_MONSTER);
	actor->scaleY = toFIXED(1) / 2;
	actor->scaleX = toFIXED(1) / 2;
	actor->update = scTolbiShipMastGrowActorScale;
	actor = fdActorGet(ACTOR_FIRST_SIGHTED_MONSTER + 1);
	actor->scaleY = toFIXED(1) / 2;
	actor->scaleX = toFIXED(1) / 2;
	actor->update = scTolbiShipMastGrowActorScale;
	actor = fdActorGet(ACTOR_FIRST_SIGHTED_MONSTER + 2);
	actor->scaleY = toFIXED(1) / 2;
	actor->scaleX = toFIXED(1) / 2;
	actor->update = scTolbiShipMastGrowActorScale;
	sySynch(1);
	fdActorSetSpeed(ACTOR_FIRST_SIGHTED_MONSTER, 0.52);
	fdActorSetSpeed(ACTOR_FIRST_SIGHTED_MONSTER + 1, 0.45);
	fdActorSetSpeed(ACTOR_FIRST_SIGHTED_MONSTER + 2, 0.6);
	fdActorSetDestination(ACTOR_FIRST_SIGHTED_MONSTER, 128, 345);
	fdActorSetDestination(ACTOR_FIRST_SIGHTED_MONSTER + 1, 136, 330);
	fdActorSetDestination(ACTOR_FIRST_SIGHTED_MONSTER + 2, 156, 340);
	fdScriptWait(60);
	fdActorRunRepeatedMotion(ACTOR_LOOKOUT, 2);
	fdActorWalkToAndWait(ACTOR_LOOKOUT, 164, 344);
	fdActorJump(ACTOR_LOOKOUT, 4, 10);
	fdActorJump(ACTOR_LOOKOUT, 6, 40);
	fdActorStartRepeatedMotion(ACTOR_LOOKOUT, 3);
	/* "Monsters sighted!" */
	fdScriptShowMessageThenWait(ACTOR_LOOKOUT, 0, 20);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	syImportScriptSetExitRequest(TOLBI_SHIP_MAST_TO_SHIP_ROOMS_MONSTERS);
	fdScriptEnd();
}

/* Stops a bird's drift before it swoops. */
static inline void scTolbiShipMastStopBirdDrifting(ACTOR *actor)
{
	actor->tgtX = ACTOR_NO_TARGET;
	actor->tgtY = ACTOR_NO_TARGET;
	actor->tgtZ = ACTOR_NO_TARGET;
	SET_VELOCITY(actor, 0, 0, 0);
	actor->sceneWord = 0;
}

/* Sets each of the seven birds drifting about the mast in its own way. */
static inline void scTolbiShipMastStartBirdsDrifting(void)
{
	scTolbiShipMastStartBirdDrifting(ACTOR_FIRST_BIRD);
	scTolbiShipMastStartBirdDrifting(ACTOR_FIRST_BIRD + 1);
	scTolbiShipMastStartBirdDrifting(ACTOR_FIRST_BIRD + 2);
	scTolbiShipMastStartBirdDrifting(ACTOR_FIRST_BIRD + 3);
	scTolbiShipMastStartBirdDrifting(ACTOR_FIRST_BIRD + 4);
	scTolbiShipMastStartBirdDrifting(ACTOR_FIRST_BIRD + 5);
	scTolbiShipMastStartBirdDrifting(ACTOR_FIRST_BIRD + 6);
}

/* Stops the birds drifting and gives them the speed to fly off. */
static inline void scTolbiShipMastStartBirdsFlyingOff(void)
{
	fdActorStop(ACTOR_FIRST_BIRD);
	fdActorStop(ACTOR_FIRST_BIRD + 1);
	fdActorStop(ACTOR_FIRST_BIRD + 2);
	fdActorStop(ACTOR_FIRST_BIRD + 3);
	fdActorStop(ACTOR_FIRST_BIRD + 4);
	fdActorStop(ACTOR_FIRST_BIRD + 5);
	fdActorStop(ACTOR_FIRST_BIRD + 6);
	fdActorSetSpeed(ACTOR_FIRST_BIRD, 3);
	fdActorSetSpeed(ACTOR_FIRST_BIRD + 1, 3);
	fdActorSetSpeed(ACTOR_FIRST_BIRD + 2, 3);
	fdActorSetSpeed(ACTOR_FIRST_BIRD + 3, 3);
	fdActorSetSpeed(ACTOR_FIRST_BIRD + 4, 3);
	fdActorSetSpeed(ACTOR_FIRST_BIRD + 5, 3);
	fdActorSetSpeed(ACTOR_FIRST_BIRD + 6, 3);
}

/* On the mast at TOLBI_SHIP_MAST_FROM_WORLD_MAP_SECOND_ATTACK, the second
   attack, a bird swoops at the lookout twice and Sean cries out. "Monsters!" */
static void scTolbiShipMastPlaySecondAttack(void)
{
	ACTOR *approachingActor;

	fdScriptStart();
	fdActorSetSprColById(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
	fdActorSetSprFlags(fdActorGet(ACTOR_PARTY_LEADER), 0);
	sySynch(1);
	fdScriptAppendActorPlacements(TolbiShipMastBirdActorPlacements);
	sySynch(1);
	scTolbiShipMastStartBirdsDrifting();
	fdActorSetScript(ACTOR_LOOKOUT, TolbiShipMastLookoutActions);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_CIRCLE);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptWait(120);
	/* The first bird swoops at the lookout twice while the rest keep
	   drifting. */
	approachingActor = fdActorGet(ACTOR_FIRST_BIRD);
	fdActorStop(ACTOR_FIRST_BIRD);
	scTolbiShipMastStopBirdDrifting(approachingActor);
	fdScriptWait(20);
	fdActorSetSpeed(ACTOR_FIRST_BIRD, 8);
	syFieldActorSetMoveTarget(approachingActor, 164, 144, 321);
	fdActorWaitForMove(approachingActor);
	syFieldActorSetMoveTarget(approachingActor, 164, 104, 321);
	fdActorWaitForMove(approachingActor);
	syFieldActorSetMoveTarget(approachingActor, 204, 124, 321);
	fdActorWaitForMove(approachingActor);
	syFieldActorSetMoveTarget(approachingActor, 144, 0, 169);
	fdActorStop(ACTOR_LOOKOUT);
	sySynch(1);
	fdActorFaceDirection(ACTOR_LOOKOUT, FACING_WEST, 0);
	fdActorShowEmote(ACTOR_LOOKOUT, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 60);
	fdActorSetSpeed(ACTOR_FIRST_BIRD, 2);
	scTolbiShipMastStartBirdDrifting(ACTOR_FIRST_BIRD);
	fdScriptWait(20);
	fdActorSetScript(ACTOR_LOOKOUT, TolbiShipMastLookoutActions);
	fdScriptWait(120);
	fdActorStop(ACTOR_FIRST_BIRD);
	scTolbiShipMastStopBirdDrifting(approachingActor);
	fdScriptWait(20);
	fdActorSetSpeed(ACTOR_FIRST_BIRD, 8);
	syFieldActorSetMoveTarget(approachingActor, 164, 144, 321);
	fdActorWaitForMove(approachingActor);
	fdActorSetSpeed(ACTOR_FIRST_BIRD, 5);
	syFieldActorSetMoveTarget(approachingActor, 164, 104, 321);
	fdActorWaitForMove(approachingActor);
	syFieldActorSetMoveTarget(approachingActor, 164, 114, 321);
	fdActorWaitForMove(approachingActor);
	syFieldActorSetMoveTarget(approachingActor, 164, 104, 321);
	fdActorWaitForMove(approachingActor);
	syFieldActorSetMoveTarget(approachingActor, 204, 124, 321);
	fdActorWaitForMove(approachingActor);
	syFieldActorSetMoveTarget(approachingActor, 144, 0, 169);
	fdActorStop(ACTOR_LOOKOUT);
	sySynch(1);
	fdActorFaceDirection(ACTOR_LOOKOUT, FACING_WEST, 0);
	fdActorShowEmote(ACTOR_LOOKOUT, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 60);
	fdActorSetSpeed(ACTOR_FIRST_BIRD, 2);
	scTolbiShipMastStartBirdDrifting(ACTOR_FIRST_BIRD);
	/* The lookout jumps, the monsters' theme plays, and Sean cries out. */
	fdActorJump(ACTOR_LOOKOUT, 4, 20);
	fdActorJump(ACTOR_LOOKOUT, 6, 40);
	sySndPlay(MUS_THE_SLEEPING_TREASURE_OF_ALTMILLER_CAVE);
	gmFlagSet(FLAG_TOLBI_SHIP_MONSTERS_SIGHTED);
	fdScriptSetMessageId(MsgTolbiShipMonstersExclaimed);
	/* "Monsters!" */
	fdScriptShowMessageThenWait(ACTOR_SEAN, 0, 20);
	fdActorShowEmote(ACTOR_LOOKOUT, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorWalkToAndWait(ACTOR_LOOKOUT, 164, 344);
	fdScriptWait(40);
	fdActorRunRepeatedMotion(ACTOR_LOOKOUT, 2);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	syImportScriptSetExitRequest(
		TOLBI_SHIP_MAST_TO_TOLBI_SHIP_DECK_SECOND_ATTACK);
	fdScriptEnd();
}

/* Sets a bird drifting from a random start: heading east or west, rising or
   sinking, 6 to 9 pixels up and a quarter larger than its sheet. */
static void scTolbiShipMastStartBirdDrifting(Sint32 actorId)
{
	ACTOR *actor;

	actor = fdActorGet(actorId);
	fdActorSetSprPriority(actorId, 1);
	actor->motionFlg = 0;
	actor->tmr = syRandomBelow(2);
	actor->userValue = syRandomBelow(2);
	actor->y.fixed = toFIXED(syRandomBelow(4)) + toFIXED(6);
	actor->sceneWord = syRandomBelow(toFIXED(3) / 8) - toFIXED(3) / 16;
	actor->scaleX = toFIXED(5) / 4;
	actor->scaleY = toFIXED(5) / 4;
	fdActorSetScript(actorId, TolbiShipMastDriftActions);
}

/* On the mast at TOLBI_SHIP_MAST_FROM_WORLD_MAP_THIRD_ATTACK, the third attack,
   the birds scatter, then ACTOR_LANDING_MONSTER drops in beside the lookout. */
static void scTolbiShipMastPlayThirdAttack(void)
{
	Uint32 frm;
	ACTOR *actor;

	fdScriptStart();
	fdActorSetSprColById(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
	fdActorSetSprFlags(fdActorGet(ACTOR_PARTY_LEADER), 0);
	sySynch(1);
	fdScriptAppendActorPlacements(TolbiShipMastBirdActorPlacements);
	sySynch(1);
	fdScriptAppendActorPlacements(TolbiShipMastLandingActorPlacements);
	sySynch(1);
	scTolbiShipMastStartBirdsDrifting();
	fdActorSetScript(ACTOR_LOOKOUT, TolbiShipMastLookoutActions);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_CIRCLE);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptWait(300);
	sySndPlay(SE_BEAST_CRY);
	fdScriptWait(100);
	/* The birds fly off before the monster lands. */
	scTolbiShipMastStartBirdsFlyingOff();
	fdActorSetDestination(ACTOR_FIRST_BIRD, 0, 100);
	fdActorSetDestination(ACTOR_FIRST_BIRD + 1, 60, 100);
	fdActorSetDestination(ACTOR_FIRST_BIRD + 2, 120, 100);
	fdActorSetDestination(ACTOR_FIRST_BIRD + 3, 180, 100);
	fdActorSetDestination(ACTOR_FIRST_BIRD + 4, 240, 100);
	fdActorSetDestination(ACTOR_FIRST_BIRD + 5, 320, 100);
	fdActorSetDestination(ACTOR_FIRST_BIRD + 6, 380, 100);
	fdScriptWait(40);
	fdActorShowEmote(ACTOR_LOOKOUT, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdScriptWait(20);
	fdActorSetPos(ACTOR_FIRST_BIRD, 0, 0);
	fdActorSetPos(ACTOR_FIRST_BIRD + 1, 0, 0);
	fdActorSetPos(ACTOR_FIRST_BIRD + 2, 0, 0);
	fdActorSetPos(ACTOR_FIRST_BIRD + 3, 0, 0);
	fdActorSetPos(ACTOR_FIRST_BIRD + 4, 0, 0);
	fdActorSetPos(ACTOR_FIRST_BIRD + 5, 0, 0);
	fdActorSetPos(ACTOR_FIRST_BIRD + 6, 0, 0);
	fdScriptWait(100);
	/* ACTOR_LANDING_MONSTER_SHADOW grows where the monster will land, then
	   ACTOR_LANDING_MONSTER drops onto it. */
	actor = fdActorGet(ACTOR_LANDING_MONSTER_SHADOW);
	actor->scaleX = toFIXED(1) / 10;
	actor->scaleY = toFIXED(1) / 10;
	fdActorSetPos(ACTOR_LANDING_MONSTER_SHADOW, 172, 340);
	fdActorStop(ACTOR_LOOKOUT);
	sySynch(1);
	fdActorRunRepeatedMotion(ACTOR_LOOKOUT, 1);
	fdActorFaceDirection(ACTOR_LOOKOUT, FACING_SOUTH_SOUTHEAST, 0);
	sySndPlay(MUS_THE_SLEEPING_TREASURE_OF_ALTMILLER_CAVE);
	gmFlagSet(FLAG_TOLBI_SHIP_MONSTERS_SIGHTED);
	/* ACTOR_LANDING_MONSTER_SHADOW grows for 32 ticks before
	   ACTOR_LANDING_MONSTER is placed above it. */
	for(frm = 0; frm < 32; frm++) {
		actor->scaleX += toFIXED(1) / 20;
		actor->scaleY += toFIXED(1) / 20;
		sySynch(1);
	}
	fdActorShowEmote(ACTOR_LOOKOUT, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	fdActorRunRepeatedMotion(ACTOR_LOOKOUT, 2);
	fdActorWalkToAndWait(ACTOR_LOOKOUT, 168, 340);
	fdActorWalkToAndWait(ACTOR_LOOKOUT, 200, 340);
	fdActorFaceDirection(ACTOR_LOOKOUT, FACING_WEST, 0);
	actor = fdActorGet(ACTOR_LANDING_MONSTER);
	actor->scaleX = toFIXED(23) / 20;
	actor->scaleY = toFIXED(23) / 20;
	actor->x.fixed = toFIXED(172);
	actor->y.fixed = toFIXED(160);
	actor->z.fixed = toFIXED(340);
	actor->facing = FACING_EAST;
	actor->bounce = toFIXED(2) / 5;
	actor->gravity = toFIXED(3);
	fdScriptWait(20);
	fdActorJump(ACTOR_LOOKOUT, 6, 20);
	sySndPlay(SE_BEAST_CRY);
	fdScriptWait(20);
	fdActorSetScript(ACTOR_LOOKOUT, ActorTrembleScript);
	fdScriptWait(80);
	fdActorSetSprPriority(ACTOR_LANDING_MONSTER, 1);
	fdActorSetSpeed(ACTOR_LANDING_MONSTER, 1);
	actor->bounce = toFIXED(1) / 10;
	actor->gravity = toFIXED(7) / 10;
	sySndPlay(SE_HIGH_JUMP);
	actor->velY = toFIXED(8);
	fdActorSetDestination(ACTOR_LANDING_MONSTER, 132, 360);
	fdActorSetDestination(ACTOR_LANDING_MONSTER_SHADOW, 132, 360);
	fdScriptWait(40);
	fdActorSetPos(ACTOR_LANDING_MONSTER, 0, 0);
	actor = fdActorGet(ACTOR_LOOKOUT);
	actor->scaleX = toFIXED(1);
	actor->scaleY = toFIXED(1);
	actor->facing = FACING_SOUTH_SOUTHWEST;
	fdScriptWait(40);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	syImportScriptSetExitRequest(
		TOLBI_SHIP_MAST_TO_TOLBI_SHIP_DECK_THIRD_ATTACK);
	fdScriptEnd();
}

/* On the mast at TOLBI_SHIP_MAST_FROM_WORLD_MAP_LAND_HO, the seven birds fly
   off, then the lookout announces land, and the rowing balance chooses the
   exit. */
static void scTolbiShipMastAnnounceLand(void)
{
	fdScriptStart();
	fdActorSetSprColById(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
	fdActorSetSprFlags(fdActorGet(ACTOR_PARTY_LEADER), 0);
	fdScriptAppendActorPlacements(TolbiShipMastBirdActorPlacements);
	sySynch(1);
	scTolbiShipMastStartBirdsDrifting();
	fdActorSetScript(ACTOR_LOOKOUT, TolbiShipMastLookoutActions);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_CIRCLE);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptWait(400);
	/* Each bird takes its own way off while shrinking out of view. */
	scTolbiShipMastStartBirdsFlyingOff();
	fdActorSetScript(ACTOR_FIRST_BIRD, TolbiShipMastBirdDepartureA);
	fdActorSetScript(ACTOR_FIRST_BIRD + 1, TolbiShipMastBirdDepartureB);
	fdActorSetScript(ACTOR_FIRST_BIRD + 2, TolbiShipMastBirdDepartureC);
	fdActorSetScript(ACTOR_FIRST_BIRD + 3, TolbiShipMastBirdDepartureD);
	fdActorSetScript(ACTOR_FIRST_BIRD + 4, TolbiShipMastBirdDepartureE);
	fdActorSetScript(ACTOR_FIRST_BIRD + 5, TolbiShipMastBirdDepartureF);
	fdActorSetScript(ACTOR_FIRST_BIRD + 6, TolbiShipMastBirdDepartureG);
	fdScriptWait(40);
	fdActorStartRepeatedMotion(ACTOR_LOOKOUT, 3);
	fdActorSetAttachedEffect(
		ACTOR_LOOKOUT, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(120);
	fdActorStartRepeatedMotion(ACTOR_LOOKOUT, 1);
	fdActorShowEmote(ACTOR_LOOKOUT, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
	fdActorSetSpeed(ACTOR_LOOKOUT, 1);
	fdActorWalkToAndWait(ACTOR_LOOKOUT, 164, 344);
	fdActorJump(ACTOR_LOOKOUT, 4, 10);
	fdActorJump(ACTOR_LOOKOUT, 6, 20);
	fdScriptSetMessageId(MsgTolbiShipLandHo);
	/* "Land ho!" */
	fdScriptShowMessageThenWait(ACTOR_LOOKOUT, 0, 20);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	/* The rowing balance picks where the ship comes in:
	   ROWING_BALANCE_OFF_COURSE takes
	   TOLBI_SHIP_MAST_TO_TOLBI_SHIP_DECK_OFF_COURSE, off course to the
	   island. */
	MainRoutineStateWork.restartScene = SCENE_SHIP_ROOMS;
	MainRoutineStateWork.restartEntrance =
		SHIP_ROOMS_PASSENGER_HOLD_FROM_ROWING_DECK;
	if(scTolbiShipMastGetRowingBalance() == ROWING_BALANCE_OFF_COURSE) {
		syImportScriptSetExitRequest(
			TOLBI_SHIP_MAST_TO_TOLBI_SHIP_DECK_OFF_COURSE);
	} else {
		syImportScriptSetExitRequest(
			TOLBI_SHIP_MAST_TO_TOLBI_SHIP_DECK_LAND_SIGHTED);
	}
	fdScriptEnd();
}

/* Rocks the view with the ship: adds a random swing across and up and down to
   the camera's origin each tick. */
static void scTolbiShipMastRockView(void)
{
	Sint32 *camera = ((MAP_GRID_SCROLL_STATE *)MapGridState[0])->origin;
	FIXED	rockX = syCos(TolbiShipMastRockAngleX);
	Sint32	rockY = sySin(TolbiShipMastRockAngleY);

	*camera++ += rockX;
	rockY *= 4;
	*camera += rockY;
	TolbiShipMastRockOffsetX += rockX;
	TolbiShipMastRockOffsetY += rockY;
	TolbiShipMastRockAngleX += syRandomBelow(384);
	TolbiShipMastRockAngleY += syRandomBelow(512);
	TolbiShipMastRockAngleX &= TRIG_ANGLE_MASK;
	TolbiShipMastRockAngleY &= TRIG_ANGLE_MASK;
}

/* The rowing balance of the chosen passengers. It is the values of those at the
   west seats (the first and third filled) less those at the east seats. */
static Sint32 scTolbiShipMastGetRowingBalance(void)
{
	Sint32 westSeats;
	Sint32 eastSeats;

	westSeats =
		scTolbiShipMastGetSeatRowValue(0) + scTolbiShipMastGetSeatRowValue(2);
	eastSeats =
		scTolbiShipMastGetSeatRowValue(1) + scTolbiShipMastGetSeatRowValue(3);
	return westSeats - eastSeats;
}

/* The rowing value of the passenger chosen for a seat, or 0 while it is
   empty. */
static Sint32 scTolbiShipMastGetSeatRowValue(Uint32 seat)
{
	Sint32 firstFlg = 0;
	Uint32 passenger;

	switch(seat) {
	case 0:
		firstFlg = FLAG_TOLBI_SHIP_FIRST_SEAT_ROWERS;
		break;
	case 1:
		firstFlg = FLAG_TOLBI_SHIP_SECOND_SEAT_ROWERS;
		break;
	case 2:
		firstFlg = FLAG_TOLBI_SHIP_THIRD_SEAT_ROWERS;
		break;
	case 3:
		firstFlg = FLAG_TOLBI_SHIP_FOURTH_SEAT_ROWERS;
		break;
	}
	for(passenger = 0; passenger <= 8; passenger++) {
		if(gmFlagGet(firstFlg + passenger) != 0)
			return TolbiShipPassengerRowValues[passenger];
	}
	return 0;
}

Sint32 TolbiShipMastSparkleActions[] = {
	SC_ADD(SW_SCALE_X, toFIXED(1) / 32),
	SC_ADD(SW_SCALE_Y, toFIXED(1) / 32),
	SC_CALL(scTolbiShipMastTurnSparkle),
	SC_WAIT(1),
	SC_LOOP(10, SC_TOP),
	SC_LABEL(1),
	SC_ADD(SW_SCALE_X, -toFIXED(1) / 32),
	SC_ADD(SW_SCALE_Y, -toFIXED(1) / 32),
	SC_CALL(scTolbiShipMastTurnSparkle),
	SC_WAIT(1),
	SC_LOOP(24, SC_TO(1)),
	SC_POSITION(0, 0, 0),
	SC_WAIT(1),
	SC_END,
};

Sint32 TolbiShipMastLookoutActions[] = {
	SC_CALL(scTolbiShipMastTurnLookoutAtRandom),
};

Sint32 TolbiShipMastDriftActions[] = {
	SC_CALL(scTolbiShipMastDriftBird),
};

#include "shared/tremble_script.c"

Sint32 TolbiShipMastBirdDepartureA[] = {
	SC_MOVE(72, 0, 216),
	SC_SYNC,
	SC_MOVE(152, 0, 196),
	SC_CALL(scTolbiShipMastFlyBirdAway),
	SC_END,
};

Sint32 TolbiShipMastBirdDepartureB[] = {
	SC_MOVE(132, 0, 242),
	SC_SYNC,
	SC_MOVE(152, 0, 196),
	SC_CALL(scTolbiShipMastFlyBirdAway),
	SC_END,
};

Sint32 TolbiShipMastBirdDepartureC[] = {
	SC_MOVE(88, 0, 166),
	SC_SYNC,
	SC_MOVE(152, 0, 196),
	SC_CALL(scTolbiShipMastFlyBirdAway),
	SC_END,
};

Sint32 TolbiShipMastBirdDepartureD[] = {
	SC_MOVE(220, 0, 134),
	SC_SYNC,
	SC_MOVE(152, 0, 196),
	SC_CALL(scTolbiShipMastFlyBirdAway),
	SC_END,
};

Sint32 TolbiShipMastBirdDepartureE[] = {
	SC_MOVE(264, 0, 228),
	SC_SYNC,
	SC_MOVE(152, 0, 196),
	SC_CALL(scTolbiShipMastFlyBirdAway),
	SC_END,
};

Sint32 TolbiShipMastBirdDepartureF[] = {
	SC_MOVE(282, 0, 168),
	SC_SYNC,
	SC_MOVE(152, 0, 196),
	SC_CALL(scTolbiShipMastFlyBirdAway),
	SC_END,
};

/* clang-format off */
Uint32 TolbiShipMastBirdDepartureG[] = {
	SC_MOVE(154, 0, 135),
	SC_SYNC,
	SC_MOVE(152, 0, 196),
	SC_CALL(scTolbiShipMastFlyBirdAway),
	SC_END,
	/* Unused: eight words after the script's end, which nothing reads. */
	/* XXX (no evidence): what they were; as sixteen halfwords they form no */
	/* table any code here indexes. */
	0x9090000, 0xd70090d, 0xdb315cb, 0x15f5164c,
	0x1f0e1aad, 0x1f6f227a, 0x23f223b1, 0x2bf727f4,
};

SCENE_ENTRANCE TolbiShipMastEntrances[] = {
	{ ENTRANCE_DEFAULT, CONDITION_ALWAYS, 165, 0, 235, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ TOLBI_SHIP_MAST_FROM_TOLBI_SHIP_DECK, CONDITION_ALWAYS, 184, 56, 352, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ TOLBI_SHIP_MAST_FROM_TOLBI_SHIP_DECK_COURSE_CLEAR, CONDITION_ALWAYS, 164, 96, 321, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ TOLBI_SHIP_MAST_FROM_WORLD_MAP_FIRST_ATTACK, CONDITION_ALWAYS, 164, 96, 321, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ TOLBI_SHIP_MAST_FROM_WORLD_MAP_SECOND_ATTACK, CONDITION_ALWAYS, 164, 96, 321, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ TOLBI_SHIP_MAST_FROM_WORLD_MAP_THIRD_ATTACK, CONDITION_ALWAYS, 164, 96, 321, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ TOLBI_SHIP_MAST_FROM_WORLD_MAP_LAND_HO, CONDITION_ALWAYS, 164, 96, 321, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

SCENE_EXIT_REGION TolbiShipMastRegions[] = {
	{176, 16, 348, 192, 32, 364, -1, TOLBI_SHIP_MAST_TO_TOLBI_SHIP_DECK},
	{-1, 0, 0, 0, 0, 0, 0, 0},
};

Uint32 TolbiShipMastExits[] = {
	SCENE_EXITS(TolbiShipMast),
	SCENE_EXIT(TOLBI_SHIP_MAST_TO_TOLBI_SHIP_DECK, TolbiShipDeck, TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST),
	SCENE_EXIT(TOLBI_SHIP_MAST_TO_SHIP_ROOMS_CAST_OFF, ShipRooms, SHIP_ROOMS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_MAST_CAST_OFF),
	SCENE_EXIT(TOLBI_SHIP_MAST_TO_SHIP_ROOMS_MONSTERS, ShipRooms, SHIP_ROOMS_CAPTAINS_ROOM_FROM_TOLBI_SHIP_MAST_MONSTERS),
	SCENE_EXIT(TOLBI_SHIP_MAST_TO_TOLBI_SHIP_DECK_SECOND_ATTACK, TolbiShipDeck, TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_SECOND_ATTACK),
	SCENE_EXIT(TOLBI_SHIP_MAST_TO_TOLBI_SHIP_DECK_THIRD_ATTACK, TolbiShipDeck, TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_THIRD_ATTACK),
	SCENE_EXIT(TOLBI_SHIP_MAST_TO_TOLBI_SHIP_DECK_LAND_SIGHTED, TolbiShipDeck, TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_LAND_SIGHTED),
	SCENE_EXIT(TOLBI_SHIP_MAST_TO_TOLBI_SHIP_DECK_OFF_COURSE, TolbiShipDeck, TOLBI_SHIP_DECK_FROM_TOLBI_SHIP_MAST_OFF_COURSE),
	SCENE_EXITS_END,
};

ACTOR_PLACE TolbiShipMastActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_MAN_IN_BLUE_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE TolbiShipMastFirstAttackActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_GLINT, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_JELLYFISH_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE TolbiShipMastBirdActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_BIRD, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		144, 0, 169, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BIRD, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		134, 0, 189, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BIRD, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		124, 0, 209, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BIRD, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		129, 0, 174, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BIRD, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		139, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BIRD, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		149, 0, 204, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BIRD, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		154, 0, 194, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BLUE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		182, 0, 420, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE TolbiShipMastLandingActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_BLUE_WINGED_MONSTER, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
		0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{ SCENE_TABLE_END },
};

CUE TolbiShipMastCues[] = {
	{ STEP_CUE_WALKING_INTO(FACING_SOUTH), METATILE_BEHAVIOR_TOLBI_SHIP_MAST_LADDER, CONDITION_ALWAYS, SCRIPT_REF(scTolbiShipMastClimbDownMast) },
	{ CUE_TALK, ACTOR_LOOKOUT, FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF, MSGNO(MsgTolbiShipWatchSeasHelpKaja) },
	{ CUE_TALK, ACTOR_LOOKOUT, FLAG_TOLBI_SHIP_AT_ISLAND, MSGNO(MsgTolbiShipLookoutCannotRow) },
	{ CUE_TALK, ACTOR_LOOKOUT, CONDITION_ALWAYS, MSGNO(MsgTolbiShipMonstersAnywhereAroundHere) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LOOKOUT, FLAG_TOLBI_SHIP_FIRST_ATTACK_FOUGHT_OFF, MSGNO(MsgTolbiShipHiddenAnchorDiscovered) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LOOKOUT, FLAG_TOLBI_SHIP_AT_ISLAND, MSGNO(MsgTolbiShipSeaTooVast) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LOOKOUT, CONDITION_ALWAYS, MSGNO(MsgTolbiShipIslandTreasures) },
	{ EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_TOLBI_SHIP_MAST_ANCHOR_CHARM, FLAG_TOLBI_SHIP_ANCHOR_CHARM_FOUND, SCRIPT_REF(scTolbiShipMastFindAnchorCharm) },
	{ CUE_TABLE_END },
};

#include "shared/tolbi_ship_rowing.c"
/* clang-format on */
