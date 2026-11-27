/* @file */
/* SceneId_Xian, the village of Xian. */
/* */
/* Its houses, shops and inn are lacquer_rooms_b.c. Master Feh's school is a */
/* room of the Fuchin Temple overlay, fuchin_temple.c. That overlay is */
/* LacquerRoomsA, which XIAN_TO_LACQUER_ROOMS_A_FEHS_SCHOOL leads into. */
/* */
/* Xian is the village of silk and kung fu beyond Mogall Forest. The party */
/* comes in from the World Map at the south end and may pass straight through */
/* to Altin, but there is a Djinni to win here first. */
/* */
/* What the villagers say turns on the news that Hsu is trapped */
/* (FLAG_XIAN_HEARD_HSU_TRAPPED), which Master Hama's Reveal lesson at Lama */
/* Temple brings. Every villager's talk and Mind Read has a row for each side */
/* of it. After the news the small boy looks west, the kung fu student stands */
/* where her strike leaves her, and the muscular man turns to face the party. */
/* */
/* Once Feizhi has left Master Feh's school to look for Hsu */
/* (FLAG_FEIZHI_WENT_TO_HSU, fuchin_temple.c) she waits by the village's south
 */
/* way out, looking for someone to take her west. She is gone once she has */
/* found the road blocked at Alpine Crossing (FLAG_FEIZHI_SAW_ROCKSLIDE) or */
/* Xian has heard the news. */
/* */
/* A woman carries water along the street below the waterfall, from the pool */
/* at its west end, cell (17, 14), to the door of the east house at its east */
/* end, cell (26, 13), and back. Spoken to on the eastern part of her round she
 */
/* spills her water, and a puddle spreads at her feet that Isaac must hop over.
 */
/* Spilled at the east end, by the wall, Frost freezes it into an ice pillar */
/* that Isaac climbs onto the ledge where Mist, a Mercury Djinni, waits at */
/* cell (28, 20); Mist joins when spoken to. Standing in her way at the west */
/* end for two seconds makes her march Isaac back to the pool. */
/* */
/* The kung fu student, a little girl by the bridge north of the mulberry */
/* orchard, shows her strike on the post before her once, and Force on the */
/* post makes it jump on its own, to her surprise. */
/* */
/* The spilled water and the student's strike last until the party next */
/* enters a scene other than by returning from a battle. */
/* */
/* A jar by the mulberry orchard, at cell (10, 22), holds a Lucky Medal. */
/* */
/* The village's doors and where they lead: */
/* - Five doors lead into the houses, shops and inn (SceneId_LacquerRoomsB): */
/*   the house at cell (21, 20), the north and east houses on the water */
/*   carrier's street at cells (23, 13) and (26, 13), the inn at (25, 26) and */
/*   the weapon and armor shop at (13, 11). */
/* - The Sanctum's sliding door at cells (10, 13) and (11, 13) leads into Xian
 */
/*   Sanctum. */
/* - The door at cell (24, 11) leads into Master Feh's school. */
/* - The road south at cells (23, 32) and (24, 32) leads out to the World Map.
 */
/* */
/* Coming back from the school (XIAN_FROM_LACQUER_ROOMS_A_FEHS_SCHOOL) or the */
/* Sanctum (XIAN_FROM_SANCTUM_WEST) clears FLAG_SHOW_AREA_NAME, so the name is
 */
/* not shown again. */

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "macros.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/event_inline.h"
#include "graphics/graphics.h"
#include "text/msg_ids.h"

/* The water carrier has spilled her water. */
#define FLAG_XIAN_WATER_SPILLED FLAG_SCENE_TEMP_00
/* Frost has frozen the spilled water into ice that can be stood on. */
#define FLAG_XIAN_SPILLED_WATER_FROZEN FLAG_SCENE_TEMP_01
/* The kung fu student has shown her strike on the tree. */
#define FLAG_XIAN_STRIKE_SHOWN FLAG_SCENE_TEMP_02
/* Force has made the kung fu student's tree jump once, which picks her second
 */
/* line. */
#define FLAG_XIAN_TREE_JUMPED FLAG_SCENE_TEMP_03

s32 scInit(void);
extern s32 Xian_WaterCarrierRoute[];
extern s32 Xian_TrainingTreeJump[];
extern s32 Xian_TreeImpactScript[];
extern s32 Xian_TrainingParticleScript[];
extern u16 Xian_HouseDoorOpens[];
extern u16 Xian_NorthHouseDoorOpens[];
extern u16 Xian_InnDoorOpens[];
extern u16 Xian_SanctumDoorOpens[];
extern u16 Xian_SchoolDoorOpens[];
extern SCENE_ENTRANCE Xian_Entrances[];
extern u32 Xian_Exits[];
extern ACTOR_PLACE Xian_ActorPlacements[];
extern TRIGGER Xian_Triggers[];

#include "shared/field_effect_create.c"
#include "shared/field_effect_spawn.c"

/* The actors, numbering the rows of Xian_ActorPlacements. Each */
/* person is named by what their sprite shows and what they talk of before Xian
 */
/* hears of Hsu. */
typedef enum XIAN_ACTOR_ID {
	/* A young man in a blue jacket. */
	ACTOR_YOUNG_MAN_WELCOMING_TRAVELERS = ACTOR_FIRST_PLACED,
	/* A young woman (scXianAskAboutYoungWarriors). */
	ACTOR_WOMAN_ON_YOUNG_WARRIORS,
	/* A man with a topknot. */
	ACTOR_MAN_ON_KINU,
	/* An old man. */
	ACTOR_OLD_MAN_ON_MARTIAL_ARTS,
	/* An old woman with a cane. */
	ACTOR_OLD_WOMAN_ON_FEHS_SCHOOL,
	/* A small boy. */
	ACTOR_BOY_ON_SILK_CLOTHES,
	/* A little girl who strikes the tree before her with a shout. */
	ACTOR_KUNG_FU_STUDENT,
	/* A bare-chested, muscular man. */
	ACTOR_MUSCULAR_MAN_ON_DISASTERS,
	/* A young woman. */
	ACTOR_WOMAN_IN_THE_MULBERRY_ORCHARD,
	/* Feizhi, as at Alpine Crossing: looking for someone to take her west. */
	ACTOR_FEIZHI,
	/* A young woman who carries water along the street. "You made me spill my
	 */
	/* water!" */
	ACTOR_WOMAN_CARRYING_WATER,
	/* The tree the student strikes; it jumps when hit. */
	ACTOR_TRAINING_TREE,
	/* The spilled water, which Frost freezes. */
	ACTOR_SPILLED_WATER,
	/* Force's mark on the training tree. */
	ACTOR_FORCE_TARGET,
	/* Mist, a Mercury Djinni. */
	ACTOR_MIST,
	/* The first of three crates placed with SPRITE_CRATE, which scInit */
	/* hides (SPRITE_COLOR_HIDDEN): the player never sees them. */
	ACTOR_FIRST_CRATE
} XIAN_ACTOR_ID;

/* The event the water carrier raises when the leader has stood in her way */
/* for XIAN_CARRIER_BLOCKED_FRAMES. */
typedef enum XIAN_RAISED_TRIGGER {
	RAISED_CARRIER_BLOCKED = 200
} XIAN_RAISED_TRIGGER;

/* Xian's own cell behaviors. The doors' cells carry the numbers of their */
/* exits, so their rows of Xian_Triggers name them by the exit. */
typedef enum XIAN_CELL_BEHAVIOR {
	/* The water carrier's street, cells (19, 14) to (27, 14), and the cells */
	/* beside it, where Isaac hops over the spilled water. */
	CELL_BEHAVIOR_XIAN_WATER_STREET = 20,
	/* The cell north of the street between the north and east houses' doors, */
	/* cell (25, 13), from which Isaac hops south over the spilled water. */
	CELL_BEHAVIOR_XIAN_WATER_STREET_NORTH = 21,
	/* The jar by the mulberry orchard, cell (10, 22), with the Lucky Medal. */
	CELL_BEHAVIOR_XIAN_JAR = 101
} XIAN_CELL_BEHAVIOR;

/* The frames the leader must stand in the water carrier's way before she */
/* raises RAISED_CARRIER_BLOCKED. */
enum { XIAN_CARRIER_BLOCKED_FRAMES = 120 };

/* The water carrier's round (Xian_WaterCarrierRoute) runs along map row 14 */
/* between columns 17 and 26, where she turns north and back. */
enum {
	CARRIER_ROUTE_ROW = 14,
	CARRIER_ROUTE_WEST_COLUMN = 17,
	CARRIER_ROUTE_EAST_COLUMN = 26
};

/* Talking to Mist in Xian: Mist, a Mercury Djinni, joins the party. */
static void scXianMistJoins(void) {
	fdDjinnJoinParty(ACTOR_MIST, ELEMENT_MERCURY, DJINNI_INDEX(DJINNI_MIST));
}

/* Keeps the ice pillar drawn under Isaac while he stands on top of it. */
static void scXianUpdateFrozenWaterLayer(void) {
	ACTOR *leader = fdActorGet(ACTOR_PARTY_LEADER);
	if(leader->y.part[0].integer > 31) {
		fdActorGet(ACTOR_SPILLED_WATER)->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
	}
	else {
		fdActorGet(ACTOR_SPILLED_WATER)->priFlg &= ~ACTOR_PRIORITY_UNDERFOOT;
	}
}

/* Spins the post as it jumps, under the student's strike or Force. */
static void scXianRotateTrainingTree(void) {
	ACTOR *tree = fdActorGet(ACTOR_TRAINING_TREE);
	SPR *sprite = tree->spr;
	sprite->rotation += TRIG_TURN * 5 / 64;
}

/* Leaves a trail of blue after-images behind the student as she leaps. */
static void scXianSpawnTrainingParticles(void) {
	ACTOR *student = fdActorGet(ACTOR_KUNG_FU_STUDENT);

	if((FrameCount & 3) == 0) {
		EFFECT_OPTIONS options;

		options.pri = 1;
		options.pal = SPRITE_COLOR_BLUE_TINT;
		options.type = SPRITE_GIRL_IN_PINK;
		options.script = Xian_TrainingParticleScript;
		fdEffectSpawn(student->x.fixed, student->y.fixed,
					  student->z.fixed - toFIXED(1), 0, -toFIXED(1),
					  -toFIXED(1),
					  EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE |
						  EFFECT_USE_TYPE | EFFECT_USE_SCRIPT,
					  &options);
	}
}

#include "shared/actor_distance.c"

/* Stops the water carrier where she stands while `target` is within */
/* `distanceLimit` pixels of her and no more than a sixteenth of a turn off her
 */
/* heading, or whenever `alwaysStop` is set, and sets her walking on */
/* otherwise. Returns whether she is stopped. */
/* */
/* `paused` holds her still, and `timer` is 1 while this function holds her; */
/* paused with `timer` 0, she stays put whatever the leader does. */
static s32 scXianStopCarrierBeforeLeader(ACTOR *actor, ACTOR *target,
										 s32 distanceLimit, s32 alwaysStop) {
	s32 stopped = 0;

	if(actor->paused == 1 && actor->tmr == 0) {
		fdActorSelectAnimation(actor, ANIM_STAND);
		return 1;
	}

	if(fdActorDistance(target, actor) < distanceLimit || alwaysStop != 0) {
		u16 angle = syAtan(target->z.fixed - actor->z.fixed,
						   target->x.fixed - actor->x.fixed);
		s32 previousHeading = FACING_SIXTEENTH(angle - FACING_STEP);
		s32 nextHeading = FACING_SIXTEENTH(angle + FACING_STEP);
		s32 targetHeading = FACING_SIXTEENTH(angle);
		s32 currentHeading = FACING_SIXTEENTH(actor->facing);

		if(targetHeading == currentHeading || nextHeading == currentHeading ||
		   previousHeading == currentHeading || alwaysStop != 0) {
			actor->paused = 1;
			fdActorSelectAnimation(actor, ANIM_STAND);
			stopped = 1;
			actor->tmr = stopped;
		}
		else {
			actor->paused = 0;
			fdActorSelectAnimation(actor, ANIM_WALK);
			actor->tmr = 0;
		}
	}
	else {
		actor->paused = 0;
		fdActorSelectAnimation(actor, ANIM_WALK);
		actor->tmr = 0;
	}

	return stopped;
}

/* The water carrier's update: she stops and looks at Isaac when he stands in */
/* front of her, and when he has stood in her way at the west end of her */
/* round, cells (17, 14) and (18, 14), for two seconds on her way back, she */
/* raises RAISED_CARRIER_BLOCKED. */
static UNREAD_RESULT scXianWatchWaterCarrierPath(ACTOR *carrier) {
	ACTOR *leader;
	s32 near = 0;

	if(carrier->tgtX == ACTOR_NO_TARGET && carrier->tgtZ == ACTOR_NO_TARGET)
		RETURN_UNREAD(0);
	leader = fdActorGet(ACTOR_PARTY_LEADER);
	if(FIELD_CELL(leader->x.fixed) >= CARRIER_ROUTE_WEST_COLUMN &&
	   FIELD_CELL(leader->x.fixed) <= CARRIER_ROUTE_WEST_COLUMN + 1 &&
	   FIELD_CELL(leader->z.fixed) == CARRIER_ROUTE_ROW &&
	   FIELD_CELL(carrier->x.fixed) <= CARRIER_ROUTE_WEST_COLUMN + 2 &&
	   carrier->velX <= 0) {
		if(leader->x.fixed <= carrier->x.fixed) {
			carrier->sceneCnt++;
			near = 1;
		}
	}
	else {
		carrier->sceneCnt = 0;
	}
	/* sceneCnt counts the frames he has stood there. */
	if(near && carrier->sceneCnt > XIAN_CARRIER_BLOCKED_FRAMES - 1) {
		FieldStateWork->raisedTrigger = RAISED_CARRIER_BLOCKED;
		carrier->sceneCnt = 0;
	}
	scXianStopCarrierBeforeLeader(carrier, leader, 18, near);
	RETURN_UNREAD(0);
}

/* scXianWatchWaterCarrierPath, as an actor's update takes it. */
#ifdef UBFIX
#define XIAN_WATER_CARRIER_UPDATE scXianWatchWaterCarrierPath
#else
#define XIAN_WATER_CARRIER_UPDATE ((void *)scXianWatchWaterCarrierPath)
#endif

const SCENE_ENTRANCE *scGetEntrances(void) { return Xian_Entrances; }

const SCENE_EXIT_REGION *scGetExitRegions(void) { return NULL; }

const u32 *scGetExits(void) { return Xian_Exits; }

/* Once Xian has heard Hsu is trapped, the muscular man turns to face the party
 */
/* when spoken to. */
const ACTOR_PLACE *scGetActorPlacements(void) {
	if(gmFlagGet(FLAG_XIAN_HEARD_HSU_TRAPPED) != 0)
		Xian_ActorPlacements[ACTOR_MUSCULAR_MAN_ON_DISASTERS -
							 ACTOR_FIRST_PLACED]
			.talkFacing = TALK_FACING_PARTY;
	return Xian_ActorPlacements;
}

/* A villager in Xian asks whether the young warriors came through Mogall */
/* Forest. */
static void scXianAskAboutYoungWarriors(void) {
	fdEventStart();
	fdEventSetMessageId(MsgXianYoungWarriorsVeryGallantCame);
	/* "Young warriors, very gallant! You came through Mogall Forest?" */
	/* Yes: "Incredible. I respect your deeds." */
	/* No: "Came from Altin? No. That is a lie. Silk Road is blocked. No..." */
	fdEventAskYesNo(ACTOR_WOMAN_ON_YOUNG_WARRIORS, 0);
	fdEventEnd();
}

/* Talking to the water carrier on the eastern part of her round makes her */
/* spill her water: a puddle spreads at her feet, she cries "What are you */
/* doing! You made me spill my water!" and walks back to the pool for more. */
/* */
/* Near the pool, or once her water is spilled, she only says "Do not speak to
 */
/* me. I might spill my water." or "Now I must get water again." If Isaac is */
/* on the street with her, he first steps up beside her, and he steps back out
 */
/* of her way before she leaves. The puddle's cell takes a blocking cell */
/* behavior copied from the map's store at cell (16, 16). */
static void scXianTalkToWaterCarrier(void) {
	ACTOR *puddle;
	ACTOR *carrier;
	ACTOR *leader;
	s32 leaderMustStepAside;
	s32 savedFacing;
	FIXED carrierX;
	s32 carrierCellZ;
	s32 carrierCellX;
	s32 scaleY;
	FIXED scaleX;

	puddle = fdActorGet(ACTOR_SPILLED_WATER);
	fdEventStart();
	leaderMustStepAside = 0;
	fdActorGet(ACTOR_WOMAN_CARRYING_WATER)->update = NULL;
	if(fdEventGetFlag(FLAG_XIAN_WATER_SPILLED) != 0 ||
	   (FIELD_CELL(fdActorGet(ACTOR_WOMAN_CARRYING_WATER)->x.fixed)) <=
		   CARRIER_ROUTE_WEST_COLUMN + 2) {
		savedFacing = fdActorGet(ACTOR_WOMAN_CARRYING_WATER)->facing;
		fdActorFaceActor(ACTOR_WOMAN_CARRYING_WATER, ACTOR_PARTY_LEADER, 0);
		fdEventWait(10);
		fdEventSetMessageId(MsgXianNowMustGetWaterAgain);
		if(gmFlagGet(FLAG_XIAN_WATER_SPILLED) == 0) {
			FieldStateWork->msg++;
			/* "Do not speak to me. I might spill my water." */
			fdEventShowMessage(ACTOR_WOMAN_CARRYING_WATER, 0);
			fdActorGet(ACTOR_WOMAN_CARRYING_WATER)->tmr = 0;
			fdActorGet(ACTOR_WOMAN_CARRYING_WATER)->facing = savedFacing;
		}
		else {
			/* "Now I must get water again. This work is hard on my back." */
			fdEventShowMessage(ACTOR_WOMAN_CARRYING_WATER, 0);
			fdActorFaceDirection(ACTOR_WOMAN_CARRYING_WATER, FACING_WEST, 20);
		}
		fdActorGet(ACTOR_WOMAN_CARRYING_WATER)->update =
			XIAN_WATER_CARRIER_UPDATE;
		fdEventEnd();
		return;
	}
	if(FIELD_HALF_CELL(fdActorGet(ACTOR_PARTY_LEADER)->z.fixed) >=
		   2 * CARRIER_ROUTE_ROW &&
	   FIELD_HALF_CELL(fdActorGet(ACTOR_PARTY_LEADER)->z.fixed) <=
		   2 * CARRIER_ROUTE_ROW + 1 &&
	   (FIELD_CELL(fdActorGet(ACTOR_PARTY_LEADER)->x.fixed)) !=
		   CARRIER_ROUTE_EAST_COLUMN) {
		fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.5);
		fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_WOMAN_CARRYING_WATER, 0);
		fdEventWait(5);
		/* He steps up to the middle of the cell beside hers on the street, on
		 */
		/* the side he stands (row 14's middle is 232 pixels down). */
		leader = fdActorGet(ACTOR_PARTY_LEADER);
		if(leader->x.fixed < fdActorGet(ACTOR_WOMAN_CARRYING_WATER)->x.fixed) {
			fdActorGet(ACTOR_PARTY_LEADER)->steeringFlg &= ~ACTOR_FACES_MOTION;
			fdActorWalkTo(
				ACTOR_PARTY_LEADER,
				(FIELD_CELL(fdActorGet(ACTOR_WOMAN_CARRYING_WATER)->x.fixed)
				 << 4) -
					8,
				232);
			leaderMustStepAside = 1;
		}
		else {
			fdActorGet(ACTOR_PARTY_LEADER)->steeringFlg &= ~ACTOR_FACES_MOTION;
			fdActorWalkTo(
				ACTOR_PARTY_LEADER,
				(FIELD_CELL(fdActorGet(ACTOR_WOMAN_CARRYING_WATER)->x.fixed)
				 << 4) +
					24,
				232);
		}
		fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
	}
	fdActorGet(ACTOR_WOMAN_CARRYING_WATER)->tgtX = ACTOR_NO_TARGET;
	fdActorGet(ACTOR_WOMAN_CARRYING_WATER)->tgtY = ACTOR_NO_TARGET;
	fdActorGet(ACTOR_WOMAN_CARRYING_WATER)->tgtZ = ACTOR_NO_TARGET;
	fdActorSetMovementTypeById(ACTOR_WOMAN_CARRYING_WATER, MOVEMENT_TYPE_STAND);
	fdActorSetAnimation(ACTOR_WOMAN_CARRYING_WATER, ANIM_STAND);
	fdActorRunRepeatedMotion(ACTOR_WOMAN_CARRYING_WATER, 2);
	fdEventWait(10);
	sySndPlay(SE_SPARKLING_PASSAGE);
	puddle->scaleX = 3 * FIXED_ONE / 10;
	puddle->scaleY = 3 * FIXED_ONE / 10;
	carrierCellX = FIELD_CELL(fdActorGet(ACTOR_WOMAN_CARRYING_WATER)->x.fixed);
	carrierCellZ = FIELD_CELL(fdActorGet(ACTOR_WOMAN_CARRYING_WATER)->z.fixed);
	fdActorSetPosFX(ACTOR_SPILLED_WATER, FIELD_CELL_CENTER(carrierCellX),
					FIELD_CELL_CENTER(carrierCellZ));
	carrierX = fdActorGet(ACTOR_WOMAN_CARRYING_WATER)->x.fixed;
	carrier = fdActorGet(ACTOR_WOMAN_CARRYING_WATER);
	fdMapGridCopyCellAttributes(16, 16, 1, 1, FIELD_CELL(carrierX),
								FIELD_CELL(carrier->z.fixed));
	fdActorSetSprPriority(ACTOR_SPILLED_WATER, 2);
	puddle->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
	do {
		sySynch(3);
		scaleY = puddle->scaleY;
		scaleX = puddle->scaleX;
		puddle->scaleY = scaleY + FIXED_ONE / 10;
		scaleX += FIXED_ONE / 10;
		puddle->scaleX = scaleX;
	} while(scaleX <= FIXED_ONE - 1);
	fdActorShowEmote(ACTOR_WOMAN_CARRYING_WATER,
					 EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 70);
	fdActorFaceActor(ACTOR_WOMAN_CARRYING_WATER, ACTOR_PARTY_LEADER, 0);
	fdEventWait(20);
	fdActorShowEmote(ACTOR_WOMAN_CARRYING_WATER,
					 EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
	fdActorStartRepeatedMotion(ACTOR_WOMAN_CARRYING_WATER, 2);
	fdEventWait(70);
	fdEventSetMessageId(MsgXianDoingMadeMeSpillMy);
	/* "What are you doing! You made me spill my water!" */
	fdEventShowMessageThenWait(ACTOR_WOMAN_CARRYING_WATER, 0, 20);
	scPlayQueuedMusic();
	if(FIELD_CELL(fdActorGet(ACTOR_PARTY_LEADER)->x.fixed) ==
		   CARRIER_ROUTE_EAST_COLUMN &&
	   (FIELD_CELL(fdActorGet(ACTOR_PARTY_LEADER)->z.fixed)) >=
		   CARRIER_ROUTE_ROW) {
		leaderMustStepAside = 1;
	}
	if(leaderMustStepAside != 0) {
		fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.8);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 10);
		fdActorGet(ACTOR_PARTY_LEADER)->steeringFlg &= ~ACTOR_FACES_MOTION;
		fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
		fdActorSetDestinationOffset(ACTOR_PARTY_LEADER, 0, 16);
		fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
		fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	}
	fdActorSetSpeed(ACTOR_WOMAN_CARRYING_WATER, 0.8);
	if(FIELD_CELL(fdActorGet(ACTOR_WOMAN_CARRYING_WATER)->z.fixed) !=
	   CARRIER_ROUTE_ROW) {
		fdActorWalkToAndWait(
			ACTOR_WOMAN_CARRYING_WATER,
			fdActorGet(ACTOR_WOMAN_CARRYING_WATER)->x.part[0].integer, 232);
	}
	fdActorWalkToAndWait(ACTOR_WOMAN_CARRYING_WATER, 280, 232);
	gmFlagSet(FLAG_XIAN_WATER_SPILLED);
	fdActorGet(ACTOR_PARTY_LEADER)->steeringFlg |= ACTOR_FACES_MOTION;
	fdEventEnd();
}

/* When Isaac casts Frost on the spilled water from north of the street, at */
/* the foot of the cliff, the puddle is brought to the front drawing layer */
/* before it freezes. All this changes on screen is brief: as the ice rises */
/* it is drawn over the left of the Psynergy ring around Isaac rather than */
/* behind it, and a moment later the ice stands as it would without this. */
static void scXianUpdatePuddlePriority(void) {
	ACTOR *leader = fdActorGet(ACTOR_PARTY_LEADER);
	if(FIELD_CELL(leader->z.fixed) < CARRIER_ROUTE_ROW)
		fdActorSetSprPriority(ACTOR_SPILLED_WATER, 1);
}

/* Frost on the spilled water in Xian freezes the puddle into an ice pillar. */
/* Isaac can climb onto it, and from the one at the street's east end onto the
 */
/* ledge where Mist waits; there is nothing left to hop over. */
/* */
/* The pillar's cell takes the raised cell stored at cell (3, 17). */
static void scXianFreezeSpilledWater(void) {
	ACTOR *puddle;
	FIXED puddleX;

	fdEventStart();
	fdActorGet(ACTOR_SPILLED_WATER)->priFlg &= ~ACTOR_PRIORITY_UNDERFOOT;
	fdActorGet(ACTOR_SPILLED_WATER)->motionFlg = 0;
	puddleX = fdActorGet(ACTOR_SPILLED_WATER)->x.fixed;
	puddle = fdActorGet(ACTOR_SPILLED_WATER);
	fdMapGridCopyCellAttributes(3, 17, 1, 1, FIELD_CELL(puddleX),
								(FIELD_CELL(puddle->z.fixed)));
	syTaskAddFrame(scXianUpdateFrozenWaterLayer, TASK_PASS_FRAME);
	gmFlagSet(FLAG_XIAN_SPILLED_WATER_FROZEN);
	fdActorSetSprPriority(ACTOR_SPILLED_WATER, 2);
	fdEventEnd();
}

/* Talking to the kung fu student by the bridge the first time, she cries */
/* "Hoo-whaaa... Hachaaa!", Isaac steps back, and she leaps west in a blur of */
/* after-images and strikes the post, which jumps spinning into the air and */
/* drops back. "Heh heh heh..." After that she only boasts of her kung fu. */
static void scXianTalkToKungFuStudent(void) {
	FIXED reboundVelocity;
	s32 stepBackOffset[3];
	EFFECT_OPTIONS options;

	fdEventStart();
	if(gmFlagGet(FLAG_XIAN_STRIKE_SHOWN) != 0) {
		fdActorFaceActor(ACTOR_KUNG_FU_STUDENT, ACTOR_PARTY_LEADER, 0);
		fdEventWait(10);
		fdEventSetMessageId(MsgXianKungFuStrong);
		/* "My kung fu is strong. If you go to Master Feh's school every..." */
		fdEventShowMessage(ACTOR_KUNG_FU_STUDENT, 0);
		fdActorFaceDirection(ACTOR_KUNG_FU_STUDENT, FACING_EAST, 10);
		fdEventEnd();
	}
	else {
		fdEventSetMessageId(MsgXianHooWhaaaHachaaa);
		/* "Hoo-whaaa... Hachaaa!" */
		fdEventShowMessage(ACTOR_KUNG_FU_STUDENT, 0);
		fdActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
		fdActorGet(ACTOR_PARTY_LEADER)->steeringFlg &= ~ACTOR_FACES_MOTION;
		SET_VECTOR(stepBackOffset, 0, 0, 0);
		syFieldVectorAddPolarOffset(-8, fdActorGet(ACTOR_PARTY_LEADER)->facing,
									(FIELD_POS *)stepBackOffset);
		fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
		fdActorSetDestinationOffset(ACTOR_PARTY_LEADER,
									stepBackOffset[0] / toFIXED(1),
									stepBackOffset[2] / toFIXED(1));
		fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
		fdActorGet(ACTOR_PARTY_LEADER)->steeringFlg |= ACTOR_FACES_MOTION;
		fdEventWait(30);
		fdActorRunRepeatedMotion(ACTOR_KUNG_FU_STUDENT, 2);
		options.pri = 1;
		fdEffectSpawn(toFIXED(192), 0, toFIXED(312), toFIXED(1) / 10,
					  toFIXED(1) / 5, 0,
					  EFFECT_USE_PRIORITY | EFFECT_SCRIPT_MEDIUM, 0);
		fdEffectSpawn(toFIXED(192), 0, toFIXED(312), toFIXED(1) / 5,
					  toFIXED(1) / 10, 0,
					  EFFECT_USE_PRIORITY | EFFECT_SCRIPT_MEDIUM, 0);
		sySndPlay(SE_WATER);
		syTaskAddFrame(scXianSpawnTrainingParticles, TASK_PASS_FRAME);
		fdActorGet(ACTOR_KUNG_FU_STUDENT)->velY = toFIXED(6);
		fdActorGet(ACTOR_KUNG_FU_STUDENT)->gravity = toFIXED(1);
		fdActorGet(ACTOR_KUNG_FU_STUDENT)->bounce = 0;
		fdActorSetSpeed(ACTOR_KUNG_FU_STUDENT, 3);
		fdActorSetDestination(ACTOR_KUNG_FU_STUDENT, 168, 312);
		fdActorWaitForMoveById(ACTOR_KUNG_FU_STUDENT);
		sySndPlay(SE_HEAVY_IMPACT);
		fdActorSetScript(ACTOR_TRAINING_TREE, Xian_TrainingTreeJump);
		syTaskAddFrame(scXianRotateTrainingTree, TASK_PASS_FRAME);
		options.type = SPRITE_STAR_BURST;
		options.script = Xian_TreeImpactScript;
		options.rotation = DEGtoANG(90);
		fdEffectSpawn(toFIXED(168), 0, toFIXED(332), 0, 0, 0,
					  EFFECT_USE_PRIORITY | EFFECT_USE_TYPE |
						  EFFECT_USE_SCRIPT | EFFECT_USE_ROTATION,
					  &options);
		fdActorSetDestination(ACTOR_KUNG_FU_STUDENT, 146, 312);
		fdActorWaitForMoveById(ACTOR_KUNG_FU_STUDENT);
		syCloseEvent(scXianSpawnTrainingParticles);
		fdEffectSpawn(toFIXED(144), 0, toFIXED(312), 0, 0, 0,
					  EFFECT_USE_PRIORITY | EFFECT_SCRIPT_MEDIUM, 0);
		fdEffectSpawn(toFIXED(144), 0, toFIXED(312), -toFIXED(1) / 5,
					  toFIXED(1) / 10, 0,
					  EFFECT_USE_PRIORITY | EFFECT_SCRIPT_MEDIUM, 0);
		/* Half a pixel a frame back east, and below as a rotation: half a turn,
		 */
		/* which stands the post upright again. */
		reboundVelocity = -toFIXED(1) / 2;
		fdEffectSpawn(toFIXED(144), 0, toFIXED(312), reboundVelocity, 0, 0,
					  EFFECT_USE_PRIORITY | EFFECT_SCRIPT_MEDIUM, 0);
		fdActorWaitForScriptEndById(ACTOR_TRAINING_TREE);
		sySndPlay(SE_CHEST_OPEN);
		fdEffectSpawn(toFIXED(168), toFIXED(8), toFIXED(312), 0, 0, 0,
					  EFFECT_USE_PRIORITY | EFFECT_SCRIPT_MEDIUM, 0);
		fdEffectSpawn(toFIXED(168), toFIXED(8), toFIXED(312), toFIXED(1) / 5, 0,
					  0, EFFECT_USE_PRIORITY | EFFECT_SCRIPT_MEDIUM, 0);
		fdEffectSpawn(toFIXED(168), toFIXED(8), toFIXED(312), -toFIXED(1) / 5,
					  0, 0, EFFECT_USE_PRIORITY | EFFECT_SCRIPT_MEDIUM, 0);
		syCloseEvent(scXianRotateTrainingTree);
		fdActorGet(ACTOR_TRAINING_TREE)->spr->rotation = reboundVelocity;
		fdActorGet(ACTOR_KUNG_FU_STUDENT)->bounce = FIXED_ONE / 4;
		fdActorGet(ACTOR_KUNG_FU_STUDENT)->gravity = toFIXED(1);
		fdEventWait(30);
		fdActorFaceDirection(ACTOR_KUNG_FU_STUDENT, FACING_EAST, 20);
		fdActorRunRepeatedMotion(ACTOR_KUNG_FU_STUDENT, 2);
		fdEventWait(20);
		/* "Heh heh heh..." */
		fdEventShowMessageThenWait(ACTOR_KUNG_FU_STUDENT, 0, 20);
		gmFlagSet(FLAG_XIAN_STRIKE_SHOWN);
		scPlayQueuedMusic();
		fdEventEnd();
	}
}

/* Feizhi asks, in Xian, whether the party are strong warriors. */
static void scXianTalkToFeizhi(void) {
	fdEventStart();
	fdEventSetMessageId(MsgXianWarriorsFromSchoolStrongWarriors);
	/* "You are the warriors from the school... Are you strong warriors?" */
	/* Yes: "I am happy to see strong warriors. I hope you will go west." */
	/* No: "You are weak warriors? Very few warriors would call..." */
	fdEventAskYesNo(ACTOR_FEIZHI, 0);
	fdEventEnd();
}

/* Walking into the spilled water in Xian before it is frozen, along the */
/* street or south from the doorsteps above it, makes Isaac hop over it, */
/* landing two cells on if nothing stands there. */
static void scXianJumpOverSpilledWater(void) {
	s32 savedMotionFlags;
	ACTOR *leader;
	FIELD_POS landingPosition;

	leader = fdActorGet(ACTOR_PARTY_LEADER);
	savedMotionFlags = leader->motionFlg;
	if(gmFlagGet(FLAG_XIAN_WATER_SPILLED) != 0) {
		landingPosition.x = (leader->x.fixed & FIELD_CELL_MASK) + toFIXED(8);
		landingPosition.y = leader->y.fixed;
		landingPosition.z = (leader->z.fixed & FIELD_CELL_MASK) + toFIXED(8);
		syFieldVectorAddPolarOffset(32, FACING_NEAREST_CARDINAL(leader->facing),
									&landingPosition);
		if(syFieldActorCheckMovementCollision(leader, &landingPosition) == 0) {
			fdEventStart();
			fdActorSelectAnimation(leader, ANIM_LEADER_SPRING);
			sySynch(6);
			sySndPlay(SE_JUMP);
			fdActorSelectAnimation(leader, ANIM_LEADER_LEAP);
			leader->speedLimit = toFIXED(3);
			leader->acceleration = toFIXED(2);
			leader->velY = toFIXED(4);
			leader->motionFlg &= ACTOR_JUMP_KEEPS_MOTION;
			fdActorSetSprFlags(leader, 0);
			fdActorMoveToAndWait(ACTOR_PARTY_LEADER,
								 FIXEDtoINT(landingPosition.x),
								 FIXEDtoINT(landingPosition.z));
			fdActorSelectAnimation(leader, ANIM_LEADER_SPRING);
			fdActorSetSprFlags(leader, SPRITE_FLAG_SHADOW);
			leader->motionFlg = savedMotionFlags;
			fdEventEnd();
		}
	}
}

/* Walks the leader through a doorway and leaves by its exit. Through the */
/* Sanctum's door the leader walks straight in, without centering on it. */
static void scXianWalkThroughExit(s32 exitNumber) {
	fdActorGet(ACTOR_PARTY_LEADER)->motionFlg = 0;
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.5);
	if(exitNumber == XIAN_TO_SANCTUM) {
		fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
		fdActorSetDestinationOffset(ACTOR_PARTY_LEADER, 0, -16);
	}
	else {
		fdActorCenterAndWalk(ACTOR_PARTY_LEADER, 2, -16);
	}
	FieldStateWork->transitionFrm = MAP_TRANSITION_FRAMES;
	syImportEventSetExitRequest(exitNumber);
}

/* Walking north into one of Xian's doors opens it with a creak, or the */
/* Sanctum's sliding door with a thump, and the party goes in. */
static void scXianOpenDoorAndLeave(void) {
	FIELD_STATE *state = FieldStateWork;

	fdEventStart();

	switch(state->stepTrigger) {
	case XIAN_TO_HOUSE:
		sySndPlay(SE_DOOR_OPEN);
		fdMapGridPlayMapEdit(Xian_HouseDoorOpens, 81, 18);
		break;
	case XIAN_TO_HOUSE_NORTH:
		sySndPlay(SE_DOOR_OPEN);
		fdMapGridPlayMapEdit(Xian_NorthHouseDoorOpens, 83, 11);
		break;
	case XIAN_TO_LACQUER_ROOMS_B_EAST:
		sySndPlay(SE_DOOR_OPEN);
		fdMapGridPlayMapEdit(Xian_NorthHouseDoorOpens, 86, 11);
		break;
	case XIAN_TO_INN:
		sySndPlay(SE_DOOR_OPEN);
		fdMapGridPlayMapEdit(Xian_InnDoorOpens, 84, 24);
		break;
	case XIAN_TO_WEAPON_AND_ARMOR_SHOP:
		sySndPlay(SE_DOOR_OPEN);
		fdMapGridPlayMapEdit(Xian_InnDoorOpens, 72, 7);
		break;
	case XIAN_TO_SANCTUM:
		sySndPlay(SE_THUMP);
		fdMapGridPlayMapEdit(Xian_SanctumDoorOpens, 69, 11);
		break;
	case XIAN_TO_LACQUER_ROOMS_A_FEHS_SCHOOL:
		sySndPlay(SE_DOOR_OPEN);
		fdMapGridPlayMapEdit(Xian_SchoolDoorOpens, 83, 7);
		break;
	default:
		break;
	}

	scXianWalkThroughExit(state->stepTrigger);
	fdEventEnd();
}

/* Force on the post the kung fu student strikes makes it jump into the air */
/* spinning and drop back, and she cries "Wha-Wha-What happened? The tree */
/* jumped on its own!" Each time after that: "...It jumped again... I am too */
/* tired..." */
static void scXianStrikeTrainingTreeWithForce(void) {
	TASK_FUNC rotate;

	fdEventStart();
	fdActorSetScript(ACTOR_TRAINING_TREE, Xian_TrainingTreeJump);
	rotate = scXianRotateTrainingTree;
	syTaskAddFrame(rotate, TASK_PASS_FRAME);
	fdActorWaitForScriptEndById(ACTOR_TRAINING_TREE);
	sySndPlay(SE_CHEST_OPEN);
	fdEffectSpawn(toFIXED(168), toFIXED(8), toFIXED(312), 0, 0, 0,
				  EFFECT_USE_PRIORITY | EFFECT_SCRIPT_MEDIUM, 0);
	fdEffectSpawn(toFIXED(168), toFIXED(8), toFIXED(312), toFIXED(1) / 5, 0, 0,
				  EFFECT_USE_PRIORITY | EFFECT_SCRIPT_MEDIUM, 0);
	fdEffectSpawn(toFIXED(168), toFIXED(8), toFIXED(312), -toFIXED(1) / 5, 0, 0,
				  EFFECT_USE_PRIORITY | EFFECT_SCRIPT_MEDIUM, 0);
	syCloseEvent(rotate);
	fdActorGet(ACTOR_TRAINING_TREE)->spr->rotation = DEGtoANG(180);
	fdActorSetPos(ACTOR_FORCE_TARGET, 168, 312);
	fdEventWait(20);
	fdActorFaceActor(ACTOR_KUNG_FU_STUDENT, ACTOR_TRAINING_TREE, 0);
	fdActorRunRepeatedMotion(ACTOR_KUNG_FU_STUDENT, 2);
	fdEventWait(10);
	fdEventSetMessageId(MsgXianWhaWhaHappened);
	if(fdEventGetFlag(FLAG_XIAN_TREE_JUMPED)) {
		FieldStateWork->msg++;
	}
	fdEventShowMessage(ACTOR_KUNG_FU_STUDENT, 0);
	gmFlagSet(FLAG_XIAN_TREE_JUMPED);
	scPlayQueuedMusic();
	fdEventEnd();
}

/* Standing in the water carrier's way at the west end of her round for two */
/* seconds makes her angry: she marches back to the pool with Isaac before */
/* her, he winces, and she starts her round again. */
static void scXianWaterCarrierStartsRoundAgain(void) {
	fdEventStart();
	fdActorSetMovementTypeById(ACTOR_WOMAN_CARRYING_WATER, MOVEMENT_TYPE_STAND);
	fdActorGet(ACTOR_WOMAN_CARRYING_WATER)->update = 0;
	fdActorGet(ACTOR_WOMAN_CARRYING_WATER)->tgtX = ACTOR_NO_TARGET;
	fdActorGet(ACTOR_WOMAN_CARRYING_WATER)->tgtZ = ACTOR_NO_TARGET;
	fdActorGet(ACTOR_WOMAN_CARRYING_WATER)->velX = 0;
	fdActorGet(ACTOR_WOMAN_CARRYING_WATER)->velZ = 0;
	fdActorGet(ACTOR_WOMAN_CARRYING_WATER)->speedLimit = 0;
	fdActorGet(ACTOR_WOMAN_CARRYING_WATER)->acceleration = 0;
	fdActorShowEmote(ACTOR_WOMAN_CARRYING_WATER,
					 EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
	fdActorStartRepeatedMotion(ACTOR_WOMAN_CARRYING_WATER, 2);
	fdEventWait(60);
	fdActorSetSpeed(ACTOR_WOMAN_CARRYING_WATER, 1.5);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 1.5);
	fdActorWalkTo(ACTOR_WOMAN_CARRYING_WATER, 280, 232);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 296, 232);
	fdActorWaitForMoveById(ACTOR_WOMAN_CARRYING_WATER);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 20);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
	fdActorSetScript(ACTOR_WOMAN_CARRYING_WATER, Xian_WaterCarrierRoute);
	fdActorGet(ACTOR_WOMAN_CARRYING_WATER)->update = XIAN_WATER_CARRIER_UPDATE;
	fdEventEnd();
}

const TRIGGER *scGetTriggers(void) { return Xian_Triggers; }

/* Plays the village's wind and sets up the village as the story and this */
/* visit left it. */
s32 scInit(void) {
	ACTOR *actor;
	u32 crateIndex;

	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	scQueueMusic(SE_AMBIENT_WIND);
	/* Back from the school (XIAN_FROM_LACQUER_ROOMS_A_FEHS_SCHOOL) or the */
	/* Sanctum (XIAN_FROM_SANCTUM_WEST), the last two entrances, the name */
	/* is not shown. */
	if(MainRoutineStateWork.entrance >= XIAN_FROM_LACQUER_ROOMS_A_FEHS_SCHOOL) {
		gmFlagClear(FLAG_SHOW_AREA_NAME);
	}
	if(gmFlagGet(FLAG_XIAN_HEARD_HSU_TRAPPED)) {
		fdActorFaceDirection(ACTOR_BOY_ON_SILK_CLOTHES, FACING_WEST, 0);
		fdActorSetPos(ACTOR_KUNG_FU_STUDENT, 146, 312);
		fdActorFaceDirection(ACTOR_KUNG_FU_STUDENT, FACING_EAST, 0);
		if(gmFlagGet(FLAG_FEIZHI_WENT_TO_HSU)) {
			fdActorSetPos(ACTOR_FEIZHI, 0, 0);
		}
	}
	/* Gone once she has found the road blocked (Alpine Crossing's */
	/* FLAG_FEIZHI_SAW_ROCKSLIDE). */
	if(gmFlagGet(FLAG_FEIZHI_SAW_ROCKSLIDE)) {
		fdActorSetPos(ACTOR_FEIZHI, 0, 0);
	}
	/* Set the three crates up as fdActorCreateConfigured sets up */
	/* its actors: still, not solid, never a target and hidden. */
	/* XXX (no evidence): what the crates are for; nothing reads them again. */
	for(crateIndex = 0; crateIndex <= 2; crateIndex++) {
		ACTOR *crate = fdActorGet(crateIndex + ACTOR_FIRST_CRATE);
		crate->spr->image.pri = 1;
		crate->motionFlg = 0;
		crate->collisionFlg = ACTOR_NOT_TARGETED;
		fdActorSetSprFlags(crate, 0);
		fdActorSetSprCol(crate, SPRITE_COLOR_HIDDEN);
		crate->priFlg = (crate->priFlg & ~ACTOR_PRIORITY_AUTOMATIC) |
						ACTOR_PRIORITY_UNDERFOOT;
	}
	if(gmFlagGet(FLAG_XIAN_STRIKE_SHOWN)) {
		fdActorSetPos(ACTOR_KUNG_FU_STUDENT, 146, 312);
		fdActorFaceDirection(ACTOR_KUNG_FU_STUDENT, FACING_EAST, 0);
	}
	if(gmFlagGet(FLAG_XIAN_SPILLED_WATER_FROZEN)) {
		fdActorSetAnimation(ACTOR_SPILLED_WATER, PUDDLE_ANIMATION_PILLAR);
		{
			FIXED puddleX = fdActorGet(ACTOR_SPILLED_WATER)->x.fixed;

			fdMapGridCopyCellAttributeRect(
				3, 17, 1, 1, FIELD_CELL(puddleX),
				FIELD_CELL(fdActorGet(ACTOR_SPILLED_WATER)->z.fixed));
		}
		syTaskAddFrame(scXianUpdateFrozenWaterLayer, TASK_PASS_FRAME);
	}
	/* The water carrier's sprite is drawn with its green and blue swapped. */
	fdActorSetSprColById(ACTOR_WOMAN_CARRYING_WATER, SPRITE_COLOR_RBG);
	fdActorGet(ACTOR_WOMAN_CARRYING_WATER)->update = XIAN_WATER_CARRIER_UPDATE;
	actor = fdActorGet(ACTOR_TRAINING_TREE);
	actor->motionFlg = 0;
	actor->y.fixed = toFIXED(16);
	actor->tgtY = toFIXED(16);
	actor->scaleX = 11 * FIXED_ONE / 20;
	actor->scaleY = 2 * FIXED_ONE / 5;
	actor->spr->rotation = DEGtoANG(180);
	fdActorSetSprFlags(fdActorGet(ACTOR_FORCE_TARGET), 0);
	fdActorGet(ACTOR_FORCE_TARGET)->motionFlg = 0;
	fdActorGet(ACTOR_FORCE_TARGET)->y.fixed = 0;
	fdActorGet(ACTOR_FORCE_TARGET)->tgtY = ACTOR_NO_TARGET;
	return 0;
}

#include "shared/field_effect_duration_scripts.c"

const s32 *FieldEffect_DurationScripts[] = {
	FieldEffect_ShortScript,
	FieldEffect_MediumScript,
	FieldEffect_LongScript,
};

/* The water carrier's round: from the pool at the street's west end, cell */
/* (17, 14), east to cell (26, 14), up to the east house's door and back, */
/* pausing at each end. */
s32 Xian_WaterCarrierRoute[] = {
	SC_SPEED(0.4),
	SC_ACCELERATION(0.1),
	SC_WAIT(60),
	SC_LABEL(1),
	SC_ANIMATION(ANIM_NOD),
	SC_WAIT(60),
	SC_ANIMATION(ANIM_NOD),
	SC_WAIT(180),
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(424, 0, 232),
	SC_SYNC,
	SC_MOVE(424, 0, 216),
	SC_SYNC,
	SC_WAIT(10),
	SC_SET(SW_FACING, FACING_EAST_SOUTHEAST),
	SC_WAIT(10),
	SC_ANIMATION(ANIM_NOD),
	SC_WAIT(60),
	SC_ANIMATION(ANIM_NOD),
	SC_WAIT(120),
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(424, 0, 232),
	SC_SYNC,
	SC_MOVE(280, 0, 232),
	SC_SYNC,
	SC_LOOP(SC_FOREVER, SC_TO(1)),
	SC_END,
};

/* The struck post jumps 32 pixels into the air and drops back. */
s32 Xian_TrainingTreeJump[] = {
	SC_SET(SW_SNAP, 0),
	SC_SPEED(3),
	SC_ACCELERATION(1.5),
	SC_MOVE(168, 48, 312),
	SC_SYNC,
	SC_SPEED(3),
	SC_ACCELERATION(0.1),
	SC_SET(SW_SNAP, 1),
	SC_MOVE(168, 16, 312),
	SC_SYNC,
	SC_END,
};

/* The effect at the tree's foot when the student strikes it: 18 frames. */
s32 Xian_TreeImpactScript[] = {
	SC_WAIT(18), SC_WAIT(1), SC_POSITION(0, 0, 0), SC_WAIT(1), SC_RELEASE,
};

/* Each particle beside the leaping student, facing west: 16 frames. */
s32 Xian_TrainingParticleScript[] = {
	SC_SET(SW_FACING, FACING_WEST), SC_WAIT(16), SC_WAIT(1),
	SC_POSITION(0, 0, 0),			SC_WAIT(1),	 SC_RELEASE,
};

/* The doors' opening frames, copied from map cells. */
u16 Xian_HouseDoorOpens[] = {
	MAPGRID_ANIMATION_STEP(64, 48, 1, 2, 4),
	MAPGRID_ANIMATION_STEP(63, 48, 1, 2, 4),
	MAPGRID_ANIMATION_STEPS_END,
};
u16 Xian_NorthHouseDoorOpens[] = {
	MAPGRID_ANIMATION_STEP(69, 50, 2, 2, 4),
	MAPGRID_ANIMATION_STEP(65, 50, 2, 2, 4),
	MAPGRID_ANIMATION_STEPS_END,
};
u16 Xian_InnDoorOpens[] = {
	MAPGRID_ANIMATION_STEP(66, 46, 3, 2, 4),
	MAPGRID_ANIMATION_STEP(63, 46, 3, 2, 4),
	MAPGRID_ANIMATION_STEPS_END,
};
u16 Xian_SanctumDoorOpens[] = {
	MAPGRID_ANIMATION_STEP(67, 50, 4, 2, 4),
	MAPGRID_ANIMATION_STEP(63, 50, 4, 2, 4),
	MAPGRID_ANIMATION_STEPS_END,
};
u16 Xian_SchoolDoorOpens[] = {
	MAPGRID_ANIMATION_STEP(66, 44, 3, 2, 4),
	MAPGRID_ANIMATION_STEP(63, 44, 3, 2, 4),
	MAPGRID_ANIMATION_STEPS_END,
};

SCENE_ENTRANCE Xian_Entrances[] = {
	{0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{XIAN_FROM_HOUSE, CONDITION_ALWAYS, 376, 0, 504, FACING_NORTH, 0, -1, -1,
	 -1, -1, 0},
	{XIAN_FROM_WEAPON_AND_ARMOR_SHOP, CONDITION_ALWAYS, 344, 0, 328,
	 FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{XIAN_FROM_HOUSE_NORTH, CONDITION_ALWAYS, 376, 0, 216, FACING_SOUTH, 0, -1,
	 -1, -1, -1, 0},
	{XIAN_FROM_LACQUER_ROOMS_B_EAST, CONDITION_ALWAYS, 424, 0, 216,
	 FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{XIAN_FROM_LACQUER_ROOMS_B_SOUTH, CONDITION_ALWAYS, 408, 0, 424,
	 FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{XIAN_FROM_LACQUER_ROOMS_B_WEST, CONDITION_ALWAYS, 216, 0, 184,
	 FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{XIAN_FROM_LACQUER_ROOMS_A_FEHS_SCHOOL, CONDITION_ALWAYS, 392, 0, 184,
	 FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{XIAN_FROM_SANCTUM_WEST, CONDITION_ALWAYS, 176, 0, 216, FACING_SOUTH, 0, -1,
	 -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

u32 Xian_Exits[] = {
	SCENE_EXITS(Xian),
	SCENE_EXIT(XIAN_TO_HOUSE, LacquerRoomsB, LACQUER_ROOMS_B_HOUSE),
	SCENE_EXIT(XIAN_TO_HOUSE_NORTH, LacquerRoomsB, LACQUER_ROOMS_B_HOUSE_NORTH),
	SCENE_EXIT(XIAN_TO_LACQUER_ROOMS_B_EAST, LacquerRoomsB,
			   LACQUER_ROOMS_B_FROM_XIAN_EAST),
	SCENE_EXIT(XIAN_TO_INN, LacquerRoomsB, LACQUER_ROOMS_B_INN_SOUTH),
	SCENE_EXIT(XIAN_TO_WEAPON_AND_ARMOR_SHOP, LacquerRoomsB,
			   LACQUER_ROOMS_B_WEAPON_AND_ARMOR_SHOP),
	SCENE_EXIT(XIAN_TO_SANCTUM, Sanctum, SANCTUM_FROM_XIAN),
	SCENE_EXIT(XIAN_TO_LACQUER_ROOMS_A_FEHS_SCHOOL, LacquerRoomsA,
			   LACQUER_ROOMS_A_FEHS_SCHOOL_FROM_XIAN),
	SCENE_EXIT(XIAN_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_XIAN),
	SCENE_EXITS_END,
};

/* clang-format off */
ACTOR_PLACE Xian_ActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_IN_BLUE_JACKET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        328, 0, 448, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_BLACK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        152, 0, 184, FACING_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_TOPKNOT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        420, 0, 304, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_BLUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        144, 0, 232, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        312, 0, 200, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        264, 0, 184, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_IN_PINK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        192, 0, 312, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MUSCULAR_MAN_WITH_TOPKNOT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        428, 0, 432, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_BLACK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        200, 0, 440, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_FEIZHI, IF_FLAG_SET(FLAG_FEIZHI_WENT_TO_HSU), MOVEMENT_TYPE_STAND,
        368, 0, 496, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_BLACK_HAIR, CONDITION_ALWAYS, (s32)Xian_WaterCarrierRoute,
        280, 0, 232, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        168, 0, 312, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        0, 0, 0, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        168, 0, 312, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MERCURY_DJINNI, FLAG_DJINNI(DJINNI_MIST), MOVEMENT_TYPE_STAND,
        448, 0, 328, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_CRATE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        426, 0, 428, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_CRATE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        299, 0, 222, FACING_WEST, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_CRATE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        168, 0, 280, FACING_WEST, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    { SCENE_TABLE_END },
};

TRIGGER Xian_Triggers[] = {
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), XIAN_TO_HOUSE, CONDITION_ALWAYS, EVENT(scXianOpenDoorAndLeave) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), XIAN_TO_HOUSE_NORTH, CONDITION_ALWAYS, EVENT(scXianOpenDoorAndLeave) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), XIAN_TO_LACQUER_ROOMS_B_EAST, CONDITION_ALWAYS, EVENT(scXianOpenDoorAndLeave) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), XIAN_TO_INN, CONDITION_ALWAYS, EVENT(scXianOpenDoorAndLeave) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), XIAN_TO_WEAPON_AND_ARMOR_SHOP, CONDITION_ALWAYS, EVENT(scXianOpenDoorAndLeave) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), XIAN_TO_SANCTUM, CONDITION_ALWAYS, EVENT(scXianOpenDoorAndLeave) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), XIAN_TO_LACQUER_ROOMS_A_FEHS_SCHOOL, CONDITION_ALWAYS, EVENT(scXianOpenDoorAndLeave) },
    { TRIGGER_EXIT, XIAN_TO_WORLD_MAP, CONDITION_ALWAYS, XIAN_TO_WORLD_MAP },
    { TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, CELL_BEHAVIOR_XIAN_WATER_STREET, FLAG_XIAN_SPILLED_WATER_FROZEN, EVENT(scXianJumpOverSpilledWater) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_XIAN_WATER_STREET_NORTH, FLAG_XIAN_SPILLED_WATER_FROZEN, EVENT(scXianJumpOverSpilledWater) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), XIAN_TO_HOUSE_NORTH, FLAG_XIAN_SPILLED_WATER_FROZEN, EVENT(scXianJumpOverSpilledWater) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), XIAN_TO_LACQUER_ROOMS_B_EAST, FLAG_XIAN_SPILLED_WATER_FROZEN, EVENT(scXianJumpOverSpilledWater) },
    { TRIGGER_TALK, ACTOR_YOUNG_MAN_WELCOMING_TRAVELERS, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianThisIsXianTravelersAreWelcome) },
    { TRIGGER_TALK, ACTOR_WOMAN_ON_YOUNG_WARRIORS, FLAG_XIAN_HEARD_HSU_TRAPPED, EVENT(scXianAskAboutYoungWarriors) },
    { TRIGGER_TALK, ACTOR_MAN_ON_KINU, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianXianIsTheVillageOfKinu) },
    { TRIGGER_TALK, ACTOR_OLD_MAN_ON_MARTIAL_ARTS, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianMartialArtsAreVeryPopularIn) },
    { TRIGGER_TALK, ACTOR_OLD_WOMAN_ON_FEHS_SCHOOL, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianYouWarriorsShouldVisitMasterFehs) },
    { TRIGGER_TALK, ACTOR_BOY_ON_SILK_CLOTHES, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianMyClothesAreNotSilkSilk) },
    { TRIGGER_TALK, ACTOR_KUNG_FU_STUDENT, FLAG_XIAN_HEARD_HSU_TRAPPED, EVENT(scXianTalkToKungFuStudent) },
    { TRIGGER_TALK, ACTOR_MUSCULAR_MAN_ON_DISASTERS, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianDisastersFromTheSkyManyMany) },
    { TRIGGER_TALK, ACTOR_WOMAN_IN_THE_MULBERRY_ORCHARD, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianThisIsTheMulberryOrchardWhen) },
    { TRIGGER_TALK, ACTOR_FEIZHI, FLAG_XIAN_HEARD_HSU_TRAPPED, EVENT(scXianTalkToFeizhi) },
    { TRIGGER_TALK, ACTOR_WOMAN_CARRYING_WATER, CONDITION_ALWAYS, EVENT(scXianTalkToWaterCarrier) },
    { TRIGGER_TALK, ACTOR_YOUNG_MAN_WELCOMING_TRAVELERS, CONDITION_ALWAYS, MSGNO(MsgXianXianIsAPeacefulVillageBut) },
    { TRIGGER_TALK, ACTOR_WOMAN_ON_YOUNG_WARRIORS, CONDITION_ALWAYS, MSGNO(MsgXianThereWasARockslideOnSilk) },
    { TRIGGER_TALK, ACTOR_MAN_ON_KINU, CONDITION_ALWAYS, MSGNO(MsgXianIWentToHelpMoveBoulders) },
    { TRIGGER_TALK, ACTOR_OLD_MAN_ON_MARTIAL_ARTS, CONDITION_ALWAYS, MSGNO(MsgXianThePeopleWhoClearedTheRockslide) },
    { TRIGGER_TALK, ACTOR_OLD_WOMAN_ON_FEHS_SCHOOL, CONDITION_ALWAYS, MSGNO(MsgXianHsuIsVeryGoodAtKung) },
    { TRIGGER_TALK, ACTOR_BOY_ON_SILK_CLOTHES, CONDITION_ALWAYS, MSGNO(MsgXianThePeopleOfXianLoveTheir) },
    { TRIGGER_TALK, ACTOR_KUNG_FU_STUDENT, CONDITION_ALWAYS, MSGNO(MsgXianAltinWasFloodedWithWaterBut) },
    { TRIGGER_TALK, ACTOR_MUSCULAR_MAN_ON_DISASTERS, CONDITION_ALWAYS, MSGNO(MsgXianWeClearedTheRocksFromSilk) },
    { TRIGGER_TALK, ACTOR_WOMAN_IN_THE_MULBERRY_ORCHARD, CONDITION_ALWAYS, MSGNO(MsgXianManyStrangeThingsHappenedAllAt) },
    { PSYNERGY_PHASE_ON_ACTOR(ABILITY_FROST, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_SPILLED_WATER, FLAG_XIAN_SPILLED_WATER_FROZEN, EVENT(scXianUpdatePuddlePriority) },
    { PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SPILLED_WATER, FLAG_XIAN_SPILLED_WATER_FROZEN, EVENT(scXianFreezeSpilledWater) },
    { PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_FORCE_TARGET, CONDITION_ALWAYS, EVENT(scXianStrikeTrainingTreeWithForce) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_WELCOMING_TRAVELERS, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianTheseChildrenMustHaveComeTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_YOUNG_WARRIORS, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianYouYoungWarriorsLookStrongBut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_KINU, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianIHopeManyPeopleComeTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_MARTIAL_ARTS, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianFightingWithKicksAndPunchesIs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_FEHS_SCHOOL, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianMasterFehsSchoolIsTrainingIt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_ON_SILK_CLOTHES, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianSilkIsShinyAndSmoothIt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_KUNG_FU_STUDENT, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianBreakingTreesWithMyHandHurts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MUSCULAR_MAN_ON_DISASTERS, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianMasterFehSaidYahAndThen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_IN_THE_MULBERRY_ORCHARD, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianTheOrchardGivesTheVillageLife) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FEIZHI, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianGoingWestAloneIsVeryDangerous) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_CARRYING_WATER, CONDITION_ALWAYS, MSGNO(MsgXianIMustCarryWaterEveryDay) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_WELCOMING_TRAVELERS, CONDITION_ALWAYS, MSGNO(MsgXianHsuWasCaughtUnderABoulder) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_YOUNG_WARRIORS, CONDITION_ALWAYS, MSGNO(MsgXianTheRocksThereWereVeryHard) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_KINU, CONDITION_ALWAYS, MSGNO(MsgXianSomePeopleDidNotCarryRocks) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_MARTIAL_ARTS, CONDITION_ALWAYS, MSGNO(MsgXianSandIsTheColorOfThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_FEHS_SCHOOL, CONDITION_ALWAYS, MSGNO(MsgXianHsuIsStrongHeadedHeWould) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_ON_SILK_CLOTHES, CONDITION_ALWAYS, MSGNO(MsgXianEveryoneStaysInXianTheyDo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_KUNG_FU_STUDENT, CONDITION_ALWAYS, MSGNO(MsgXianThisWarriorSavedAltinVillageNo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MUSCULAR_MAN_ON_DISASTERS, CONDITION_ALWAYS, MSGNO(MsgXianItIsBadForXianIf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_IN_THE_MULBERRY_ORCHARD, CONDITION_ALWAYS, MSGNO(MsgXianIWasHappyToSeeGems) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FEIZHI, CONDITION_ALWAYS, MSGNO(MsgXianWhereAreTheMerchantsNoOne) },
    { EXAMINE_TRIGGER(EXAMINE_JAR), CELL_BEHAVIOR_XIAN_JAR, FLAG_XIAN_LUCKY_MEDAL_JAR, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { TRIGGER_TALK, ACTOR_MIST, CONDITION_ALWAYS, EVENT(scXianMistJoins) },
    { TRIGGER_RAISED, RAISED_CARRIER_BLOCKED, CONDITION_ALWAYS, EVENT(scXianWaterCarrierStartsRoundAgain) },
    { TRIGGER_TABLE_END },
};
/* clang-format on */
