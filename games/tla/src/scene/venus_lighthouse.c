/* @file */
/* The Venus Lighthouse, as the prologue opens on it: Felix moves a statue onto
 */
/* the floor switch with Psynergy and goes on alone, and Jenna and Kraden, */
/* doubting they should leave, meet Alex and set off for Idejima */
/* (SceneId_VenusLighthouse). */
#include "version.h"
#include "field/actor_script.h"
#include "field/event_inline.h"
#include "field/field_sequence.h"
#include "game/game.h"
#include "scene/scene.h"
#include "text/msg_ids.h"

#include "../field/actor_set_sprite_priority.c"
#include "shared/tables.h"

/* Set once the statue Felix moved onto the floor switch has sunk into it and */
/* holds it down; the barrier no longer throws the leader back. */
#define FLAG_VENUS_LIGHTHOUSE_SWITCH_HELD FLAG_SCENE_TEMP_00
/* Set while the leader stands on the floor switch, pressed down. */
#define FLAG_VENUS_LIGHTHOUSE_ON_GROUND_FLOOR_SWITCH FLAG_SCENE_TEMP_01

/* The cell behaviors the scene's events answer, as The Broken Seal numbers */
/* them on this floor. */
typedef enum VENUS_LIGHTHOUSE_CELL_BEHAVIOR {
	/* The first of the five reliefs scVenusLighthouseReadRelief shows the words
	 */
	/* of. */
	CELL_BEHAVIOR_VENUS_LIGHTHOUSE_FIRST_RELIEF = 35,
	/* The relief by the stairs down from the floor switch's room */
	/* (scVenusLighthouseReadTrueHeart). */
	CELL_BEHAVIOR_VENUS_LIGHTHOUSE_TRUE_HEART_RELIEF = 40,
	/* The floor of yellow lightning south of the floor switch, which throws the
	 */
	/* leader back until the switch is held (scVenusLighthouseRepelLeader). */
	CELL_BEHAVIOR_VENUS_LIGHTHOUSE_BARRIER = 45,
	CELL_BEHAVIOR_VENUS_LIGHTHOUSE_FLOOR_SWITCH = 46
} VENUS_LIGHTHOUSE_CELL_BEHAVIOR;

/* The ways between the rooms of this floor, The Broken Seal's ground floor, */
/* by its names: each exit leads to the entrance of the same rooms the other */
/* way round. The stairs down at the south of the floor switch's room, where */
/* the prologue opens, lead to the room of the spiral pillars: Jenna, Kraden */
/* and Alex leave by VENUS_LIGHTHOUSE_FLOOR_SWITCH_TO_PILLARS, and from patch */
/* 1 the game restarts at VENUS_LIGHTHOUSE_FLOOR_SWITCH_FROM_PILLARS. */
typedef enum VENUS_LIGHTHOUSE_WAY {
	VENUS_LIGHTHOUSE_FRONT_ROOM_TO_RELIEFS = 2,
	VENUS_LIGHTHOUSE_RELIEFS_TO_FRONT_ROOM = 3,
	VENUS_LIGHTHOUSE_RELIEFS_TO_CARRY_STONE_EAST = 4,
	VENUS_LIGHTHOUSE_RELIEFS_TO_CARRY_STONE_WEST = 5,
	VENUS_LIGHTHOUSE_RELIEFS_TO_PILLARS = 6,
	VENUS_LIGHTHOUSE_PILLARS_TO_RELIEFS = 7,
	VENUS_LIGHTHOUSE_PILLARS_TO_CARRY_STONE = 8,
	VENUS_LIGHTHOUSE_PILLARS_TO_FLOOR_SWITCH = 9,
	VENUS_LIGHTHOUSE_PILLARS_TO_STATUE_ROW_NORTH = 10,
	VENUS_LIGHTHOUSE_PILLARS_TO_STATUE_ROW_WEST = 11,
	VENUS_LIGHTHOUSE_STATUE_ROW_TO_PILLARS_NORTH = 12,
	VENUS_LIGHTHOUSE_STATUE_ROW_TO_PILLARS_WEST = 13,
	VENUS_LIGHTHOUSE_STATUE_ROW_TO_SMALL_ROOM = 14,
	VENUS_LIGHTHOUSE_GREAT_STATUE_TO_FLOOR_SWITCH = 20,
	VENUS_LIGHTHOUSE_GREAT_STATUE_TO_SMALL_ROOM = 21,
	VENUS_LIGHTHOUSE_FLOOR_SWITCH_TO_PILLARS = 22,
	VENUS_LIGHTHOUSE_SMALL_ROOM_TO_STATUE_ROW = 24,
	VENUS_LIGHTHOUSE_SMALL_ROOM_TO_GREAT_STATUE = 25,
	VENUS_LIGHTHOUSE_FLOOR_SWITCH_TO_GREAT_STATUE = 26,
	VENUS_LIGHTHOUSE_CARRY_STONE_TO_RELIEFS_WEST = 30,
	VENUS_LIGHTHOUSE_CARRY_STONE_TO_RELIEFS_EAST = 31,
	VENUS_LIGHTHOUSE_CARRY_STONE_TO_PILLARS = 32,
	/* The floor switch room's other stairs, The Broken Seal's way on to its */
	/* first floor; with no row in SceneExits, scTakeExit sends the */
	/* party to ENTRANCE_DEFAULT. */
	VENUS_LIGHTHOUSE_FLOOR_SWITCH_STAIRS = 23,
	VENUS_LIGHTHOUSE_FRONT_ROOM_FROM_RELIEFS = 2,
	VENUS_LIGHTHOUSE_RELIEFS_FROM_FRONT_ROOM = 3,
	VENUS_LIGHTHOUSE_RELIEFS_FROM_CARRY_STONE_EAST = 4,
	VENUS_LIGHTHOUSE_RELIEFS_FROM_CARRY_STONE_WEST = 5,
	VENUS_LIGHTHOUSE_RELIEFS_FROM_PILLARS = 6,
	VENUS_LIGHTHOUSE_PILLARS_FROM_RELIEFS = 7,
	VENUS_LIGHTHOUSE_PILLARS_FROM_CARRY_STONE = 8,
	VENUS_LIGHTHOUSE_PILLARS_FROM_FLOOR_SWITCH = 9,
	VENUS_LIGHTHOUSE_PILLARS_FROM_STATUE_ROW_NORTH = 10,
	VENUS_LIGHTHOUSE_PILLARS_FROM_STATUE_ROW_WEST = 11,
	VENUS_LIGHTHOUSE_STATUE_ROW_FROM_PILLARS_NORTH = 12,
	VENUS_LIGHTHOUSE_STATUE_ROW_FROM_PILLARS_WEST = 13,
	VENUS_LIGHTHOUSE_STATUE_ROW_FROM_SMALL_ROOM = 14,
	VENUS_LIGHTHOUSE_GREAT_STATUE_FROM_FLOOR_SWITCH = 20,
	VENUS_LIGHTHOUSE_GREAT_STATUE_FROM_SMALL_ROOM = 21,
	VENUS_LIGHTHOUSE_FLOOR_SWITCH_FROM_PILLARS = 22,
	VENUS_LIGHTHOUSE_SMALL_ROOM_FROM_STATUE_ROW = 24,
	VENUS_LIGHTHOUSE_SMALL_ROOM_FROM_GREAT_STATUE = 25,
	VENUS_LIGHTHOUSE_FLOOR_SWITCH_FROM_GREAT_STATUE = 26,
	VENUS_LIGHTHOUSE_CARRY_STONE_FROM_RELIEFS_WEST = 30,
	VENUS_LIGHTHOUSE_CARRY_STONE_FROM_RELIEFS_EAST = 31,
	VENUS_LIGHTHOUSE_CARRY_STONE_FROM_PILLARS = 32
} VENUS_LIGHTHOUSE_WAY;

/* The scene's actors, numbering the rows of ActorPlacements: The Broken */
/* Seal's ground floor's, with Kraden and Alex in place of Garet and its three
 */
/* scholars set aside in the corner. At VENUS_LIGHTHOUSE_FROM_TITLE Felix and */
/* Jenna are placed as the first two rows of */
/* VenusLighthouseOpeningActorPlacements and keep their own ids. */
typedef enum VENUS_LIGHTHOUSE_ACTOR_ID {
	ACTOR_KRADEN = ACTOR_FIRST_PLACED,
	ACTOR_ALEX,
	/* The lady statue Felix moves onto the floor switch with Move; it sinks in
	 */
	/* and holds the switch down. */
	ACTOR_FLOOR_SWITCH_STATUE,
	/* The Broken Seal's three scholars of this floor
	   (SPRITE_OLD_MAN_IN_BLUE_CAP */
	/* and SPRITE_MAN_WITH_GLASSES), under its names, kept at (8, 8) out of */
	/* sight; they never speak here. */
	/* @{ */
	ACTOR_SCHOLAR_ATTACKED_ANYWAY,
	ACTOR_SCHOLAR_NOT_THE_ENTRANCE,
	ACTOR_DOOR_SCHOLAR,
	/* @} */
	/* The two spiral pillars below the stairs of */
	/* VENUS_LIGHTHOUSE_PILLARS_FROM_FLOOR_SWITCH. */
	/* @{ */
	ACTOR_SPIRAL_PILLAR_A,
	ACTOR_SPIRAL_PILLAR_B,
	/* @} */
	/* The small model of the lighthouse (VenusMiniatureLighthouseScript). */
	ACTOR_MINIATURE_LIGHTHOUSE,
	/* The first of the nine lady statues drawn grey (VenusGreyStatueScript), */
	/* in the two rooms by the entrance. */
	ACTOR_FIRST_GREY_STATUE
} VENUS_LIGHTHOUSE_ACTOR_ID;

/* An actor's timer as VenusGreyStatueScript sets it. It holds a tag in the */
/* upper bits and a colour mode (SPR_COL_MODE) in the low four. */
#define VENUS_COLOR_MARK(mode) (VENUS_COLOR_MARK_TAG << 4 | (mode))
enum { VENUS_COLOR_MARK_TAG = 0x212 }; /* the tag that marks the timer */

/* The scene's name for the byte Actor keeps for scene code. A statue with */
/* SW_VENUS_IMMOVABLE set cannot be pushed. */
enum { SW_VENUS_IMMOVABLE = SW_SCENE_COUNTER };

#define fdActorSetSprFlags ((void (*)(ACTOR *, s32))fdActorSetSprFlags)

#define fdActorSetSprCol ((void (*)(ACTOR *, s32))fdActorSetSprCol)

/* Sets up a passage effect's motion, collision and colour mode. */
void scVenusLighthouseConfigurePassageEffect(ACTOR *actor, s32 colorMode) {
	actor->motionFlg = 0;
	actor->collisionFlg = ACTOR_NOT_TARGETED;
	fdActorSetSprFlags(actor, 0);
	scFieldActorSetSprCol(actor, colorMode);
}

/* Unused: creates a mask over a hole, an actor of sprite type drawn solid */
/* black and translucent, as The Broken Seal's Venus Lighthouse drops the */
/* leader or a pillar through its floor; nothing here calls it. */
UNUSED ACTOR *scVenusLighthouseCreateBlendedPassageEffect(s32 x, s32 y, s32 z,
														  s32 type) {
	ACTOR *effect = scFieldActorCreate(type, x, y, z);

	if(effect != 0) {
		scSetActorSprPriority(effect, 0);
		scVenusLighthouseConfigurePassageEffect(effect, SPRITE_COLOR_BLACK);
		fdActorSetBlendMode(effect, 1);
		return effect;
	}
	return 0;
}

/* Unused: creates an actor drawn hidden that keeps the leader on screen */
/* while a pillar sinks by him, as The Broken Seal's Venus Lighthouse does; */
/* nothing here calls it. */
UNUSED ACTOR *scVenusLighthouseCreateOccludingPassageEffect(s32 x, s32 y, s32 z,
															s32 type) {
	ACTOR *actor = scFieldActorCreate(type, x, y, z);

	if(actor != 0) {
		scSetActorSprPriority(actor, 1);
		scVenusLighthouseConfigurePassageEffect(actor, SPRITE_COLOR_HIDDEN);
		actor->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
		return actor;
	}
	return 0;
}

#include "../field/field_effect_move.c"

#undef fdActorSetSprFlags

extern const s32 *FieldEffect_DurationScripts[];

/* Puts a short-lived effect sprite, dust unless `options` names another, at */
/* a point, moving off at the velocity given and drawn at the leader's */
/* priority: this overlay's own copy of field_effect_spawn.c. */
void scFieldEffectSpawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
						s32 velocityZ, u32 flags,
						const EFFECT_OPTIONS *options) {
	FIELD_EFFECT *party =
		(FIELD_EFFECT *)fdActorGetById(MainRoutineStateWork.leaderActorId);
	FIELD_EFFECT *effect;
	FIELD_SPR *sprite;
	const s32 *script;
	u32 start;

	if((flags & EFFECT_USE_TYPE) != 0 && options != 0)
		effect = (FIELD_EFFECT *)syImportActorCreate(options->type, x, y, z);
	else
		effect =
			(FIELD_EFFECT *)syImportActorCreate(SPRITE_DUST_CLOUD, x, y, z);
	if(effect == 0)
		return;

	sprite = effect->spr;
	fdActorSelectAnimation((void *)effect, (flags + 1) & EFFECT_SCRIPT_MASK);
	fdActorStartScript(effect,
					   FieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK]);
	effect->motionFlg = 0;
	sprite->flg = 0;
	effect->update = fdEffectUpdateMotionScaleAndSpin;
	effect->vel.x = velocityX;
	effect->vel.y = velocityY;
	effect->vel.z = velocityZ;
	scSetActorSprPriority(effect, party->spr->pri);
	effect->scaleRateX = 0;
	effect->scaleRateY = 0;
	effect->spin = 0;

	if((flags & EFFECT_OPTION_MASK) == 0 || options == 0)
		return;
	if((flags & EFFECT_USE_PALETTE) != 0)
		scFieldActorSetSprCol(effect, options->pal);
	if((flags & EFFECT_USE_PRIORITY) != 0) {
		effect->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC;
		scSetActorSprPriority(effect, options->pri);
	}
	start = flags & EFFECT_USE_START_SCALE;
	if(start != 0) {
		effect->scaleX = options->startScaleX;
		effect->scaleY = options->startScaleY;
	}
	if((flags & EFFECT_SCALE_TO_TARGET) != 0) {
		FIXED targetX;
		s32 count;

		script = FieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK];
		targetX = options->tgtScaleX;
		count = script[3];
		if(start != 0) {
			effect->scaleRateX = (targetX - effect->scaleX) / count;
			effect->scaleRateY =
				(options->tgtScaleY - effect->scaleY) / script[3];
		}
		else {
			effect->scaleRateX = (targetX - FIXED_ONE) / count;
			effect->scaleRateY = (options->tgtScaleY - FIXED_ONE) / script[3];
		}
	}
	if((flags & EFFECT_USE_SCRIPT) != 0) {
		fdActorSelectAnimation((void *)effect, 1);
		fdActorStartScript(effect, options->script);
	}
	if((flags & EFFECT_USE_ROTATION) != 0)
		sprite->rotation = options->rotation;
	if((flags & EFFECT_USE_SPIN) != 0)
		effect->spin = options->spin;
	if((flags & EFFECT_USE_UPDATE) != 0)
		effect->update = options->update;
}

#include "shared/position_distance.h"

#include "../field/actor_find_in_cell.c"

extern s32 PushLeaderSteps[];

/* Unused: the leader pushes what stands in front of him a cell on, unless */
/* something solid is beyond it or on top of it, the ground beyond refuses it */
/* or it is still moving, as The Broken Seal's pushing scenes do; nothing here
 */
/* calls it. */
UNUSED void scPushedActorAdvancePair(void) {
	POS targetPosition;
	ACTOR *leader;
	ACTOR *pillar;
	ACTOR *blockingActor;
	s32 directionIndex;
	u32 step;
	FIXED moveRate;
	s32 transitionBusy;

	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	directionIndex = FACING_INDEX(leader->facing);
	step = PushLeaderSteps[directionIndex];
	targetPosition.x = leader->x.fixed + (step & FIELD_PIXEL_MASK);
	targetPosition.y = leader->y.fixed;
	step = INTtoFIXED(step);
	targetPosition.z = leader->z.fixed + step;
	pillar = fdActorFindInCell((s32 *)&targetPosition, leader);
	if(pillar == 0)
		return;

	step = PushLeaderSteps[directionIndex];
	targetPosition.x = pillar->x.fixed + (step & FIELD_PIXEL_MASK);
	targetPosition.y = pillar->y.fixed;
	step = INTtoFIXED(step);
	targetPosition.z = pillar->z.fixed + step;
	blockingActor = fdActorFindInCell((s32 *)&targetPosition, pillar);
	if(blockingActor != 0 && (blockingActor->collisionFlg & ACTOR_SOLID) != 0)
		return;

	targetPosition.x = pillar->x.fixed;
	targetPosition.y = pillar->y.fixed + toFIXED(16);
	targetPosition.z = pillar->z.fixed;
	blockingActor = fdActorFindInCell((s32 *)&targetPosition, pillar);
	if(blockingActor != 0 && (blockingActor->collisionFlg & ACTOR_SOLID) != 0)
		return;

	pillar->layer = 2;
	step = PushLeaderSteps[directionIndex];
	targetPosition.x = pillar->x.fixed + (step & FIELD_PIXEL_MASK);
	targetPosition.y = pillar->y.fixed;
	step = INTtoFIXED(step);
	targetPosition.z = pillar->z.fixed + step;
	if(fdMapGridCheckCollision(pillar, &targetPosition) > 0)
		return;

	transitionBusy = pillar->sceneCnt;
	if(transitionBusy != 0)
		return;

	fdActorSelectAnimation(leader, ANIM_PUSH);
	moveRate = toFIXED(1) / 5;
	syWait(15);
	fdEventPlaySnd(SE_PUSH);
	pillar->speedLimit = moveRate;
	pillar->acceleration = moveRate;
	scFieldActorSetMoveTargetFX(pillar, targetPosition.x, targetPosition.y,
								targetPosition.z);
	leader->speedLimit = moveRate;
	leader->acceleration = moveRate;
	scFieldActorSetMoveTargetFX(leader, targetPosition.x, targetPosition.y,
								targetPosition.z);
	scFieldActorWaitForMove(pillar);
	fdEventNoOp();
	pillar->x.fixed = targetPosition.x;
	pillar->z.fixed = targetPosition.z;
	pillar->velX = transitionBusy;
	pillar->velZ = transitionBusy;
	leader->tgtX = ACTOR_NO_TARGET;
	leader->tgtZ = ACTOR_NO_TARGET;
	leader->velX = transitionBusy;
	leader->velZ = transitionBusy;
	leader->x.fixed = INTtoFIXED(leader->x.part[0].integer);
	leader->z.fixed = INTtoFIXED(leader->z.part[0].integer);
	fdActorSelectAnimation(leader, ANIM_STAND);
}

#include "../field/map_cell_fill_byte.c"

/* Unused: does nothing; nothing calls it. */
UNUSED void scVenusLighthouseNothing(void) {}

#define scFieldActorSetSprCol ((void (*)(void *, s32))fdActorSetSprCol)

/* Flashes the leader white every fourth frame, with the sound of a breaking */
/* item every eighth. */
s32 scVenusLighthouseFlashStruckLeader(void *actor) {
	if((FrameCount & 3) == 0)
		scFieldActorSetSprCol(actor, SPRITE_COLOR_WHITE);
	else
		scFieldActorSetSprCol(actor, SPRITE_COLOR_NORMAL);

	if((FrameCount & 7) == 0)
		fdEventPlaySnd(SE_ITEM_BREAK);
	return 0;
}

/* Emits a rising particle near the actor every eight frames. */
s32 scVenusLighthouseEmitRisingParticles(ACTOR *actor) {
	EFFECT_OPTIONS options;
	s32 framePhase;
	FIXED particleX, particleY, riseSpeed;
	framePhase = FrameCount & 7;
	if(framePhase == 0) {
		options.pri = 3 - (s32)(syRandomBelow(2));
		options.startScaleX = 2 * FIXED_ONE / 5;
		options.startScaleY = 2 * FIXED_ONE / 5;
		options.pal = SPRITE_COLOR_BLACK;
		particleX = actor->x.fixed + INTtoFIXED((s32)(syRandomBelow(9)) - 4);
		particleY = actor->y.fixed + INTtoFIXED(32 - (s32)(syRandomBelow(32)));
		riseSpeed = (INTtoFIXED((s32)(syRandomBelow(5))) + toFIXED(5)) / 10;
		scFieldEffectSpawn(
			particleX, particleY, actor->z.fixed, 0, riseSpeed, framePhase,
			EFFECT_USE_PALETTE | EFFECT_USE_PRIORITY | EFFECT_USE_START_SCALE,
			&options);
	}
	return 0;
}

#define fdActorSetSprFlags ((void (*)(void *, s32))fdActorSetSprFlags)

extern ACTOR_PLACE VenusLighthouseOpeningActorPlacements[];

const SCENE_ENTRANCE *scGetEntrances(void) { return SceneEntrances; }

const SCENE_REGION *scGetExitRegions(void) { return 0; }

const u32 *scGetExits(void) { return SceneExits; }

/* Draws the actor in the colour mode of its VENUS_COLOR_MARK. */
s32 scVenusLighthouseApplyMarkedCol(ACTOR *actor) {
	scFieldActorSetSprCol(actor, actor->tmr & 15);
	return 0;
}

/* Clears the scripted actor's sprite flags and advances its script. */
s32 scVenusLighthouseClearScriptedSprFlags(void *actor) {
	fdActorSetSprFlags(actor, 0);
	return 0;
}

const ACTOR_PLACE *scGetActorPlacements(void) {
	if(MainRoutineStateWork.entrance == VENUS_LIGHTHOUSE_FROM_TITLE)
		return VenusLighthouseOpeningActorPlacements;
	return ActorPlacements;
}

#undef scFieldActorSetSprCol

/* The Venus Lighthouse's floor events and data: Felix moving the statue and */
/* leaving Jenna and Kraden, their talk of leaving, the floor switch, the */
/* statue that repels the leader, the reliefs and the paths Reveal shows. */

void scVenusLighthouseFelixLeaves(void);

void scVenusLighthouseRepelLeader(void);

void scVenusLighthouseStepOnFloorSwitch(void);

extern u32 FieldEffect_ShortScript[];
extern u32 FieldEffect_MediumScript[];
extern s32 VenusGreyStatueScript[];
extern s32 VenusMiniatureLighthouseScript[];

extern s32 VenusLeaderDazedScript[];

/* Until the statue holds the floor switch down, stepping onto the barrier */
/* throws the leader back, dazed and flashing. */
void scVenusLighthouseRepelLeader(void) {
	s32 leader = fdPartyGetLeaderActor();
	ACTOR *actor = fdActorGetById(leader);
	s32 z;

	fdEventStart();
	fdEventEndLastingPsynergy(0);
	fdActorSetMovementTypeById(leader, VenusLeaderDazedScript);
	fdActorWaitForScriptEndById(leader);
	fdActorSetSprCol(fdActorGetById(leader), SPRITE_COLOR_GREY);
	actor->velY = toFIXED(4);
	fdActorSetSpeed(leader, 4);
	/* Thrown back off the barrier the way he came, north to z 840 or south to
	 */
	/* z 952. */
	if(FIELD_CELL(actor->z.fixed) <= 54) {
		fdActorGetById(leader)->steeringFlg &= ~ACTOR_FACES_MOTION;
		z = 210;
	}
	else {
		fdActorGetById(leader)->steeringFlg &= ~ACTOR_FACES_MOTION;
		z = 238;
	}
	fdActorWalkToAndWaitById(leader, actor->x.part[0].integer, z * 4);
	fdEventWait(1);
	syActorFlagsSet(&fdActorGetById(leader)->steeringFlg, ACTOR_FACES_MOTION);
	fdEventWait(20);
	fdActorSetSprCol(actor, SPRITE_COLOR_NORMAL);
	actor->update = scVenusLighthouseEmitRisingParticles;
	scFieldActorShowEmote(leader, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
	fdActorSetAnimationAndWait(leader, ANIM_SHAKE_HEAD);
	fdActorSetSprCol(fdActorGetById(leader), SPRITE_COLOR_NORMAL);
	fdActorSetAnimationAndWait(leader, ANIM_SHAKE_HEAD);

	actor->update = 0;
	fdEventEnd();
}

/* Standing on the floor switch presses it */
/* (FLAG_VENUS_LIGHTHOUSE_ON_GROUND_FLOOR_SWITCH) and lowers the leader into */
/* it; stepping off releases it. */
void scVenusLighthouseStepOnFloorSwitch(void) {
	ACTOR *leader = fdActorGetById(fdPartyGetLeaderActor());
	s32 z = leader->z.part[0].integer;

	/* Off the switch's middle, 8 pixels square from (676, 788). */
	if((u32)(leader->x.part[0].integer - 676) > 7 || z < 788 || z >= 796) {
		fdMapGridCopyCellTilesIndicesRect(53, 50, 42, 49, 1, 1);
		fdMapGridCopyCellsTo(55, 117, 41, 117, 3, 5);
		syGameFlagClear(FLAG_VENUS_LIGHTHOUSE_ON_GROUND_FLOOR_SWITCH);
		leader->motionFlg |= ACTOR_FOLLOWS_TERRAIN;
		leader->groundHeight = 0;
		leader->y.fixed = 0;
	}
	else if(!fdEventGetFlag(FLAG_VENUS_LIGHTHOUSE_ON_GROUND_FLOOR_SWITCH)) {
		fdEventStart();
		fdEventEndLastingPsynergy(0);
		fdEventWait(5);
		fdMapGridCopyCellTilesIndicesRect(52, 50, 42, 49, 1, 1);
		fdMapGridCopyCellsTo(52, 117, 41, 117, 3, 5);
		syGameFlagSet(FLAG_VENUS_LIGHTHOUSE_ON_GROUND_FLOOR_SWITCH);
		sySndPlay(SE_RATTLE);
		leader->motionFlg &= ~ACTOR_FOLLOWS_TERRAIN;
		leader->groundHeight = -toFIXED(2);
		leader->y.fixed = -toFIXED(2);
		fdEventEnd();
	}
}

/* The statue Felix moved onto the floor switch, in column 42, sinks in and */
/* holds it down, and the barrier lets the leader by. */
void scVenusLighthouseSinkStatue(void) {
	ACTOR *statue;
	FIXED sunk;
	s32 width;
	s32 height;

	statue = fdActorGet(ACTOR_FLOOR_SWITCH_STATUE);
	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	if(FIELD_CELL(statue->x.fixed) == 42) {
		syFieldEventWait(30);
		fdEventPlaySnd(SE_THUMP);
		statue->motionFlg = 0;
		sunk = -toFIXED(2);
		statue->groundHeight = sunk;
		statue->y.fixed = sunk;
		fdEventSetFlag(FLAG_VENUS_LIGHTHOUSE_SWITCH_HELD);
		width = 3;
		height = 5;
		fdMapGridCopyCellsTo(44, 117, 41, 117, width, height);
	}
	fdEventEnd();
}

/* Searching the relief shows its words. */
void scVenusLighthouseReadRelief(void) {
	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	fdActorSetAnimation(syFieldPartyGetLeaderActor(), ANIM_STAND);
	/* "There are words carved into the relief... If ye seek to climb Venus..."
	 */
	mnMessageShowCentered((s32)MsgVenusLighthouseReliefWords, 1);
	fdEventEnd();
}

/* Searching the other relief shows that only a true heart knows the road. */
void scVenusLighthouseReadTrueHeart(void) {
	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	fdActorSetAnimation(syFieldPartyGetLeaderActor(), ANIM_STAND);
	/* "The true road can only be known by he who bears a true heart." */
	mnMessageShowCentered((s32)MsgVenusLighthouseTrueHeart, 1);
	fdEventEnd();
}

/* Reveal fades: the paths it showed hide again. */
void scVenusLighthouseHidePaths(void) {
	s32 x;
	s32 y;
	s32 wide;
	s32 narrow;

	x = 23;
	y = 8;
	fdMapGridCopyCellAttributes(35, 8, 1, 3, x, y);
	wide = 3;
	narrow = 1;
	fdMapGridCopyCellsTo(35, 8, 23, 8, narrow, wide);
	fdMapGridCopyCellsTo(99, 8, 87, 8, narrow, wide);
	x = 46;
	y = 55;
	fdMapGridCopyCellAttributes(57, 55, 3, 3, x, y);
	fdMapGridCopyCellsTo(57, 55, 46, 55, wide, wide);
	fdMapGridCopyCellsTo(121, 55, 110, 55, wide, wide);
}

/* Reveal shows the hidden paths. */
void scVenusLighthouseRevealPaths(void) {
	s32 x;
	s32 y;
	s32 wide;
	s32 narrow;

	x = 23;
	y = 8;
	fdMapGridCopyCellAttributes(36, 8, 1, 3, x, y);
	wide = 3;
	narrow = 1;
	fdMapGridCopyCellsTo(36, 8, 23, 8, narrow, wide);
	fdMapGridCopyCellsTo(100, 8, 87, 8, narrow, wide);
	x = 46;
	y = 55;
	fdMapGridCopyCellAttributes(53, 55, 3, 3, x, y);
	fdMapGridCopyCellsTo(53, 55, 46, 55, wide, wide);
	fdMapGridCopyCellsTo(117, 55, 110, 55, wide, wide);
}

#define fdActorSetSprCol ((void (*)(ACTOR *, s32))fdActorSetSprCol)

const TRIGGER *scGetTriggers(void) { return SceneEvents; }

/* Does nothing: the end of Felix's Move, which leaves the rest to the event. */
s32 scVenusLighthouseIdle(void) { return 0; }

/* Felix casts Move on the statue. */
void scVenusLighthouseFelixMovesStatue(void) {
	fdPsynergyLoadResources(ABILITY_MOVE, 1);
	fdPsynergySetupActorPair(ACTOR_FELIX, ACTOR_FLOOR_SWITCH_STATUE);
	((FIELD_EFFECT_STATE *)Ram_HeapSlots->fieldEffState)->finishFunc =
		(void (*)(void))scVenusLighthouseIdle;
	fdPsynergyStartCast();
	syFieldEventWait(10);
	fdPsynergyRunHandler(1);
	fdBattleEffectStopMainActor();
	fdPsynergyEnd();
}

/* Draws the lady statues grey: the floor switch's, and the nine of the two */
/* rooms by the entrance. */
static inline void scVenusLighthouseGreyStatues(void) {
	scFieldActorSetSprCol(fdActorGet(ACTOR_FLOOR_SWITCH_STATUE),
						  SPRITE_COLOR_GREY);
	scFieldActorSetSprCol(fdActorGet(ACTOR_FIRST_GREY_STATUE),
						  SPRITE_COLOR_GREY);
	scFieldActorSetSprCol(fdActorGet(ACTOR_FIRST_GREY_STATUE + 1),
						  SPRITE_COLOR_GREY);
	scFieldActorSetSprCol(fdActorGet(ACTOR_FIRST_GREY_STATUE + 2),
						  SPRITE_COLOR_GREY);
	scFieldActorSetSprCol(fdActorGet(ACTOR_FIRST_GREY_STATUE + 3),
						  SPRITE_COLOR_GREY);
	scFieldActorSetSprCol(fdActorGet(ACTOR_FIRST_GREY_STATUE + 4),
						  SPRITE_COLOR_GREY);
	scFieldActorSetSprCol(fdActorGet(ACTOR_FIRST_GREY_STATUE + 5),
						  SPRITE_COLOR_GREY);
	scFieldActorSetSprCol(fdActorGet(ACTOR_FIRST_GREY_STATUE + 6),
						  SPRITE_COLOR_GREY);
	scFieldActorSetSprCol(fdActorGet(ACTOR_FIRST_GREY_STATUE + 7),
						  SPRITE_COLOR_GREY);
	scFieldActorSetSprCol(fdActorGet(ACTOR_FIRST_GREY_STATUE + 8),
						  SPRITE_COLOR_GREY);
}

/* The Japanese form; from patch 1, the form below also saves this scene and */
/* VENUS_LIGHTHOUSE_FLOOR_SWITCH_FROM_PILLARS as where the game restarts. */
#if !TLA_PATCH_1
s32 scInit(void) {
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	if(MainRoutineStateWork.entrance == VENUS_LIGHTHOUSE_FROM_TITLE) {
		fdEventStart();
		syFieldEventEndLastingPsynergy(0);
		scVenusLighthouseFelixLeaves();
		scVenusLighthouseDoubtLeaving();
		fdEventEnd();
	}
	else {
		scVenusLighthouseGreyStatues();
	}
	return 0;
}
#endif

void scVenusLighthouseDoubtLeaving(void);

/* The Japanese edition has no form of this function here. */
#if TLA_PATCH_1
s32 scInit(void) {
	MAIN_ROUTINE_STATE *state;

	state = &MainRoutineStateWork;
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	if(state->entrance == VENUS_LIGHTHOUSE_FROM_TITLE) {
		state->restartScene = SCENE_VENUS_LIGHTHOUSE;
		state->restartEntrance = VENUS_LIGHTHOUSE_FLOOR_SWITCH_FROM_PILLARS;
		fdEventStart();
		syFieldEventEndLastingPsynergy(0);
		scVenusLighthouseFelixLeaves();
		scVenusLighthouseDoubtLeaving();
		fdEventEnd();
	}
	else {
		scVenusLighthouseGreyStatues();
	}
	return 0;
}
#endif

s32 scPrepareMap(void) { return 0; }

/* Has the actor go down the stairs (fdExitPrepareActorAnimation). */
static inline void scVenusLighthouseGoDownStairs(s32 actor, s32 side) {
	fdExitPrepareActorAnimation(actor, side);
}

/* The prologue's opening: Felix moves the statue aside with Psynergy and */
/* climbs off alone, leaving Jenna and Kraden. */
void scVenusLighthouseFelixLeaves(void) {
	FIELD_STATE *state;

	fdActorSetPos(ACTOR_FELIX, 712, 816);
	fdActorSetPos(ACTOR_JENNA, 696, 840);
	fdActorSetPos(ACTOR_KRADEN, 728, 840);
	fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
	fdActorSetSpeed(ACTOR_FELIX, 1);
	fdActorSetSpeed(ACTOR_JENNA, 1);
	fdActorSetSpeed(ACTOR_KRADEN, 1);
	fdActorSetSpeed(ACTOR_ALEX, 1);
	fdCameraMoveTo(664, CAMERA_KEEP, 824, 0);
	state = Ram_HeapSlots->fieldState;
	state->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	state->transitionFrm = 60;
	fdEventStartScrTransition();
	fdEventWaitTransitionFrames();
	syFieldEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
	syFieldEventWait(20);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	syFieldEventWait(10);
	fdActorWalkToAndWait(ACTOR_FELIX, 664, 824);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
	syFieldEventWait(10);
	fdActorWalkToAndWait(ACTOR_KRADEN, 712, 824);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
	syFieldEventWait(20);
	scVenusLighthouseFelixMovesStatue();
	scVenusLighthouseSinkStatue();
	syFieldEventWait(20);
	fdActorWalkToAndWait(ACTOR_KRADEN, 728, 840);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
	fdActorWalkToAndWait(ACTOR_FELIX, 712, 816);
	fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
	syFieldEventWait(30);
	fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
	syFieldEventWait(20);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	syFieldEventWait(10);
	scFieldActorWatchAndSetScript(ACTOR_JENNA, ACTOR_FELIX);
	scFieldActorWatchAndSetScript(ACTOR_KRADEN, ACTOR_FELIX);
	fdActorWalkToAndWait(ACTOR_FELIX, 744, 816);
	fdActorWalkToAndWait(ACTOR_FELIX, 744, 780);
	fdEventPlaySnd(SE_STAIRS_DOWN);
	fdExitPrepareActorAnimation(ACTOR_FELIX, -1);
	syFieldEventWait(50);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTH, 0);
	fdActorSetPos(ACTOR_FELIX, 0, 0);
}

/* Jenna doubts they should have left Felix; Alex appears in sparks and asks */
/* after him, and the three go down the stairs to leave the lighthouse. */
void scVenusLighthouseDoubtLeaving(void) {
	ACTOR *actor;
	FIELD_STATE *state;

	syFieldEventSetMessageId(
		(s32)MsgVenusLighthouseAreYouSureWeShouldBeLeaving);
	syEventCameraFollowActor(ACTOR_KRADEN, 1);
	fdCameraWaitForArrival();
	syFieldEventWait(30);
	fdActorWalkToAndWait(ACTOR_JENNA, 664, 840);
	fdActorFaceDirection(ACTOR_JENNA, FACING_EAST, 0);
	fdActorWalkToAndWait(ACTOR_KRADEN, 696, 840);
	syFieldEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	syFieldEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
	syFieldEventWait(10);
	fdActorWalkTo(ACTOR_KRADEN, 696, 920);
	syFieldEventWait(10);
	fdActorWalkToAndWait(ACTOR_JENNA, 664, 888);
	scFieldActorWaitForMoveById(ACTOR_KRADEN);
	scFieldActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTHWEST, 0);
	syFieldEventWait(20);
	fdActorWalkToAndWait(ACTOR_KRADEN, 696, 888);
	fdActorFaceDirection(ACTOR_JENNA, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
	syFieldEventWait(20);
	fdCameraMoveTo(696, CAMERA_KEEP, 920, 1);
	fdCameraWaitForArrival();
	syFieldEventWait(20);
	fdActorRunRepeatedMotion(ACTOR_JENNA, 2);
	syFieldEventWait(10);
	/* "Are you sure we should be leaving like this?" */
	fdEventShowMessage(ACTOR_JENNA, 0);
	syFieldEventWait(10);
	scFieldActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
	/* "Once we go down those stairs, we can't get back into the lighthouse." */
	fdEventShowMessage(ACTOR_JENNA, 0);
	syFieldEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	syFieldEventWait(20);
	fdActorRunRepeatedMotion(ACTOR_JENNA, 2);
	syFieldEventWait(10);
	/* "Maybe I should have stopped my brother..." */
	fdEventShowMessage(ACTOR_JENNA, 0);
	syFieldEventWait(10);
	scFieldActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	/* "Jenna, is something troubling you?" */
	fdEventShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
	syFieldEventWait(10);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 0);
	syFieldEventWait(30);
	/* "I just..." */
	fdEventShowMessage(ACTOR_JENNA, 0);
	syFieldEventWait(10);
	scFieldActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "What is it? What's wrong?" */
	fdEventShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
	syFieldEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
	syFieldEventWait(10);
	/* "I have a bad feeling... Like something terrible is going to happen." */
	fdEventShowMessage(ACTOR_JENNA, 0);
	syFieldEventWait(30);
	fdEventPlaySnd(SE_KOLIMA_BRIDGE_POST);
	fdActorSetPos(ACTOR_ALEX, 680, 920);
	fdActorFaceDirection(ACTOR_ALEX, FACING_NORTH_NORTHWEST, 0);
	fdActorAppearInSparks(ACTOR_ALEX);
	fdActorFaceDirection(ACTOR_ALEX, FACING_NORTH_NORTHWEST, 0);
	syFieldEventWait(20);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH, 0);
	syFieldEventWait(10);
	syFieldEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
	/* "How unlike you, Jenna." */
	fdEventShowMessage(ACTOR_ALEX, 0);
	syFieldEventWait(10);
	scFieldActorStartRepeatedMotion(ACTOR_JENNA, 2);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	syFieldEventWait(20);
	fdActorFaceEachOther(ACTOR_JENNA, ACTOR_KRADEN, 40);
	scFieldActorShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	/* "Surprised to see me?" */
	fdEventShowMessage(ACTOR_ALEX, 0);
	syFieldEventWait(10);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH, 0);
	syFieldEventWait(20);
	scFieldActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "A-Alex... Were you listening?" */
	fdEventShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
	syFieldEventWait(10);
	scFieldActorShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	syFieldEventWait(10);
	fdActorFaceDirection(ACTOR_ALEX, FACING_EAST, 0);
	syFieldEventWait(30);
	fdActorFaceDirection(ACTOR_ALEX, FACING_WEST, 0);
	syFieldEventWait(30);
	fdActorFaceDirection(ACTOR_ALEX, FACING_NORTH, 0);
	syFieldEventWait(30);
	/* "Where's Felix? Why isn't he here with you?" */
	fdEventShowMessage(ACTOR_ALEX, 0);
	syFieldEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_JENNA, 2);
	syFieldEventWait(20);
	fdActorSetSpeed(ACTOR_JENNA, 0.5);
	scFieldActorWatchAndSetScript(ACTOR_KRADEN, ACTOR_JENNA);
	fdActorWalkByAndWait(ACTOR_JENNA, 0, -32);
	syFieldEventWait(10);
	/* "My brother? He left us." */
	fdEventShowMessage(ACTOR_JENNA, 0);
	syFieldEventWait(10);
	scFieldActorShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30);
	fdActorWalkByAndWait(ACTOR_ALEX, -8, -8);
	/* "What can Felix possibly be thinking?" */
	fdEventShowMessage(ACTOR_ALEX, 0);
	syFieldEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	syFieldEventWait(10);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
	syFieldEventWait(20);
	/* "He was worried about Sheba. He went to check on her." */
	fdEventShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
	syFieldEventWait(10);
	scFieldActorShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	fdActorFaceDirection(ACTOR_ALEX, FACING_NORTHEAST, 0);
	syFieldEventWait(30);
	/* "He was supposed to be leading you out of here! I thought you would..."
	 */
	fdEventShowMessage(ACTOR_ALEX, 0);
	syFieldEventWait(10);
	scFieldActorShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	/* "Why are you still here?" */
	fdEventShowMessage(ACTOR_ALEX, 0);
	syFieldEventWait(10);
	scFieldActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	scFieldActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "We tried to stop Felix from going back up to the aerie." */
	fdEventShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
	syFieldEventWait(10);
	scFieldActorShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	fdActorFaceDirection(ACTOR_ALEX, FACING_EAST_NORTHEAST, 0);
	syFieldEventWait(30);
	/* "How like him. Once Felix gets an idea into his head, he rarely..." */
	fdEventShowMessage(ACTOR_ALEX, 0);
	syFieldEventWait(20);
	scFieldActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 40);
	/* "Alex, what were you talking about just now?" */
	fdEventShowMessage(ACTOR_JENNA, 0);
	syFieldEventWait(10);
	fdActorFaceDirection(ACTOR_ALEX, FACING_NORTH_NORTHWEST, 0);
	syFieldEventWait(20);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH_SOUTHEAST, 0);
	syFieldEventWait(30);
	/* "What did you mean when you said... "How unlike you"?" */
	fdEventShowMessage(ACTOR_JENNA, 0);
	syFieldEventWait(10);
	scFieldActorShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	syFieldEventWait(20);
	fdActorRunRepeatedMotion(ACTOR_ALEX, 2);
	syFieldEventWait(10);
	/* "I was merely surprised to hear you expressing such concern, Jenna." */
	fdEventShowMessage(ACTOR_ALEX, 0);
	syFieldEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
	/* "Nevertheless, I'm impressed Felix went back..." */
	fdEventShowMessage(ACTOR_ALEX, 0);
	syFieldEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
	syFieldEventWait(10);
	fdActorSetSpeed(ACTOR_JENNA, 1);
	fdActorWalkByAndWait(ACTOR_JENNA, 0, 16);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH_SOUTHEAST, 0);
	syFieldEventWait(10);
	/* "Don't change the subject, Alex! What did you mean!?" */
	fdEventShowMessage(ACTOR_JENNA, 0);
	syFieldEventWait(10);
	scFieldActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
	/* "Are you saying that I'm insensitive!?" */
	fdEventShowMessage(ACTOR_JENNA, 0);
	syFieldEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_ALEX, 2);
	syFieldEventWait(10);
	/* "If that's what you heard, then I must have misspoken. Accept my..." */
	fdEventShowMessage(ACTOR_ALEX, 0);
	syFieldEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_ALEX, 7);
	syFieldEventWait(30);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST_NORTHWEST, 0);
	syFieldEventWait(20);
	scFieldActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "Well, Jenna, I think Alex has said his piece on the matter..." */
	fdEventShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
	syFieldEventWait(10);
	fdActorFaceDirection(ACTOR_JENNA, FACING_EAST_SOUTHEAST, 0);
	syFieldEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
	/* "I'm not sure he has, Kraden." */
	fdEventShowMessage(ACTOR_JENNA, 0);
	syFieldEventWait(10);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
	syFieldEventWait(20);
	fdActorWalkByAndWait(ACTOR_JENNA, 0, 16);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH_SOUTHEAST, 0);
	syFieldEventWait(10);
	/* "Who do you think you are, talking to me like that!?" */
	fdEventShowMessage(ACTOR_JENNA, 0);
	syFieldEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
	/* "I retract my statement, my dear. Forget all about it, Jenna." */
	fdEventShowMessage(ACTOR_ALEX, 0);
	syFieldEventWait(10);
	scFieldActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 50);
	fdActorRunRepeatedMotion(ACTOR_ALEX, 2);
	syFieldEventWait(10);
	/* "So tell me about this "feeling," Jenna..." */
	fdEventShowMessage(ACTOR_ALEX, 0);
	syFieldEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
	syFieldEventWait(10);
	/* "I'm just... I'm not sure we should be leaving Venus Lighthouse yet..."
	 */
	fdEventShowMessage(ACTOR_JENNA, 0);
	syFieldEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	syFieldEventWait(10);
	/* "What are you saying?" */
	fdEventShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
	syFieldEventWait(10);
	fdActorFaceDirection(ACTOR_JENNA, FACING_EAST, 0);
	syFieldEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
	syFieldEventWait(20);
	fdActorRunRepeatedMotion(ACTOR_ALEX, 2);
	syFieldEventWait(10);
	/* "You can't really mean to remain here..." */
	fdEventShowMessage(ACTOR_ALEX, 0);
	syFieldEventWait(10);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH_SOUTHEAST, 0);
	syFieldEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
	syFieldEventWait(30);
	scFieldActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "Alex is right. It's far too dangerous, Jenna. We would only hinder..."
	 */
	fdEventShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
	syFieldEventWait(10);
	fdActorFaceDirection(ACTOR_JENNA, FACING_EAST, 0);
	syFieldEventWait(20);
	/* "The others? Did Isaac follow us to the lighthouse?" */
	fdEventShowMessage(ACTOR_JENNA, 0);
	syFieldEventWait(10);
	scFieldActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	/* "It would seem so." */
	fdEventShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
	syFieldEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_JENNA, 2);
	syFieldEventWait(10);
	/* "He must be trying to stop Saturos and Menardi!" */
	fdEventShowMessage(ACTOR_JENNA, 0);
	syFieldEventWait(10);
	scFieldActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "If my brother goes back and finds Isaac, they're going to end up..." */
	fdEventShowMessage(ACTOR_JENNA, 0);
	syFieldEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
	/* "If the Venus Lighthouse has not yet been lit, yes. They will fight." */
	fdEventShowMessage(ACTOR_ALEX, 0);
	syFieldEventWait(10);
	scFieldActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30);
	/* "Felix is a terribly rash young man, is he not?" */
	fdEventShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
	syFieldEventWait(10);
	scFieldActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
	/* "Why are boys such fools?" */
	fdEventShowMessage(ACTOR_JENNA, 0);
	syFieldEventWait(10);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_NORTHEAST, 0);
	syFieldEventWait(30);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	syFieldEventWait(10);
	/* "He may be rash, but Felix is no fool. His good qualities outweigh..." */
	fdEventShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
	syFieldEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_JENNA, 2);
	syFieldEventWait(10);
	/* "That's why I think Isaac would understand if we just talked to him." */
	fdEventShowMessage(ACTOR_JENNA, 0);
	syFieldEventWait(10);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
	syFieldEventWait(30);
	fdActorWalkByAndWait(ACTOR_JENNA, 0, -16);
	syFieldEventWait(10);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 0);
	syFieldEventWait(30);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
	syFieldEventWait(20);
	/* "Why didn't I think of this before now?" */
	fdEventShowMessage(ACTOR_JENNA, 0);
	syFieldEventWait(10);
	fdActorFaceDirection(ACTOR_ALEX, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
	syFieldEventWait(50);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_ALEX, FACING_NORTH_NORTHEAST, 0);
	syFieldEventWait(20);
	fdActorFaceDirection(ACTOR_JENNA, FACING_EAST_SOUTHEAST, 0);
	syFieldEventWait(30);
	/* "Please... Can't we go back up and talk to Isaac?" */
	fdEventShowMessage(ACTOR_JENNA, 0);
	syFieldEventWait(10);
	fdActorFaceDirection(ACTOR_ALEX, FACING_NORTH_NORTHWEST, 0);
	syFieldEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
	/* "I'm afraid that's not possible." */
	fdEventShowMessage(ACTOR_ALEX, 0);
	syFieldEventWait(10);
	scFieldActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH_SOUTHEAST, 0);
	syFieldEventWait(20);
	/* "Alex, why?" */
	fdEventShowMessage(ACTOR_JENNA, 0);
	syFieldEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
	/* "He is an enemy." */
	fdEventShowMessage(ACTOR_ALEX, 0);
	fdActorRunRepeatedMotion(ACTOR_JENNA, 2);
	syFieldEventWait(20);
	/* "Isaac? An enemy?" */
	fdEventShowMessage(ACTOR_JENNA, 0);
	syFieldEventWait(10);
	fdActorFaceDirection(ACTOR_ALEX, FACING_EAST_NORTHEAST, 0);
	syFieldEventWait(25);
	/* "Our methods may differ, but you and I ultimately want the same thing..."
	 */
	fdEventShowMessage(ACTOR_ALEX, 0);
	syFieldEventWait(10);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH, 0);
	syFieldEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	syFieldEventWait(10);
	/* "To light the elemental lighthouses..." */
	fdEventShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
	syFieldEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_ALEX, 2);
	syFieldEventWait(10);
	/* "Isaac and his friends would prevent this from happening." */
	fdEventShowMessage(ACTOR_ALEX, 0);
	syFieldEventWait(10);
	scFieldActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "So that makes them enemies?" */
	fdEventShowMessage(ACTOR_JENNA, 0);
	syFieldEventWait(10);
	fdActorFaceDirection(ACTOR_ALEX, FACING_NORTHWEST, 0);
	syFieldEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_NOD);
	syFieldEventWait(10);
	/* "But fear not... They won't be able to defeat Saturos and Menardi." */
	fdEventShowMessage(ACTOR_ALEX, 0);
	syFieldEventWait(10);
	fdActorFaceEachOther(ACTOR_JENNA, ACTOR_KRADEN, 50);
	fdActorFaceDirection(ACTOR_ALEX, FACING_SOUTH, 0);
	syFieldEventWait(30);
	scFieldActorShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "So... Felix went back to the top of the lighthouse?" */
	fdEventShowMessage(ACTOR_ALEX, 0);
	syFieldEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_ALEX, 6);
	syFieldEventWait(30);
	scFieldActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	syFieldEventWait(10);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
	syFieldEventWait(20);
	/* "Why do you seem so pleased, Alex?" */
	fdEventShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
	syFieldEventWait(10);
	scFieldActorShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	fdActorFaceDirection(ACTOR_ALEX, FACING_EAST_NORTHEAST, 0);
	syFieldEventWait(20);
	/* "Why shouldn't I be? After all, the lighthouse will soon shine..." */
	fdEventShowMessage(ACTOR_ALEX, 0);
	syFieldEventWait(10);
	scFieldActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
	fdActorFaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
	syFieldEventWait(20);
	/* "Alex, why do you want to see the beacon lit so badly?" */
	fdEventShowMessage(ACTOR_JENNA, 0);
	syFieldEventWait(10);
	scFieldActorShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	fdActorFaceDirection(ACTOR_ALEX, FACING_NORTH_NORTHWEST, 0);
	syFieldEventWait(30);
	/* "Oh, so it's my turn to answer questions, is it?" */
	fdEventShowMessage(ACTOR_ALEX, 0);
	syFieldEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_ALEX, 2);
	syFieldEventWait(10);
	/* "Once, Alchemy was commonplace throughout this world..." */
	fdEventShowMessage(ACTOR_ALEX, 0);
	syFieldEventWait(10);
	fdActorFaceDirection(ACTOR_ALEX, FACING_EAST_NORTHEAST, 0);
	syFieldEventWait(10);
	/* "With its power, mankind worked wonders across the land..." */
	fdEventShowMessage(ACTOR_ALEX, 0);
	syFieldEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	syFieldEventWait(10);
	/* "The lost age of man..." */
	fdEventShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
	syFieldEventWait(10);
	fdActorFaceDirection(ACTOR_ALEX, FACING_NORTH_NORTHWEST, 0);
	syFieldEventWait(20);
	/* "I want to see that world restored once again, and..." */
	fdEventShowMessage(ACTOR_ALEX, 0);
	syFieldEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
	syFieldEventWait(10);
	/* "And what, Alex?" */
	fdEventShowMessage(ACTOR_JENNA, 0);
	syFieldEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_ALEX, ANIM_SHAKE_HEAD);
	/* "We've spoken long enough already. Let us continue this another time." */
	fdEventShowMessage(ACTOR_ALEX, 0);
	syFieldEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_ALEX, 2);
	syFieldEventWait(10);
	/* "We should leave the lighthouse now, before the beacon is fired..." */
	fdEventShowMessage(ACTOR_ALEX, 0);
	syFieldEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	syFieldEventWait(20);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
	syFieldEventWait(20);
	/* "I concur... Let's meet at the location we agreed on and wait for Felix."
	 */
	fdEventShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
	syFieldEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_JENNA, 2);
	syFieldEventWait(10);
	fdActorFaceDirection(ACTOR_JENNA, FACING_EAST_SOUTHEAST, 0);
	syFieldEventWait(30);
	scFieldActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30);
	/* "All right, Kraden, let's go." */
	fdEventShowMessage(ACTOR_JENNA, 0);
	syFieldEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
	syFieldEventWait(20);
	fdActorSetAnimation(ACTOR_ALEX, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	syFieldEventWait(20);
	fdActorSetSpeed(ACTOR_KRADEN, 1.2);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_WALK);
	actor = (ACTOR *)fdActorGet(ACTOR_JENNA);
	if(actor != NULL)
		fdActorSetDestination(ACTOR_KRADEN, actor->x.part[0].integer,
							  actor->z.part[0].integer);
	scFieldActorWaitForMoveById(ACTOR_KRADEN);
	fdActorSetPos(ACTOR_KRADEN, 0, 0);
	fdActorSetSpeed(ACTOR_ALEX, 1.2);
	fdActorSetAnimation(ACTOR_ALEX, ANIM_WALK);
	actor = (ACTOR *)fdActorGet(ACTOR_JENNA);
	if(actor != NULL)
		fdActorSetDestination(ACTOR_ALEX, actor->x.part[0].integer,
							  actor->z.part[0].integer);
	scFieldActorWaitForMoveById(ACTOR_ALEX);
	fdActorSetPos(ACTOR_ALEX, 0, 0);
	syFieldEventWait(10);
	fdActorWalkByAndWait(ACTOR_JENNA, 16, 0);
	fdActorWalkToAndWait(ACTOR_JENNA, 680, 944);
	syFieldEventWait(15);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH_NORTHEAST, 0);
	syFieldEventWait(50);
	fdActorFaceDirection(ACTOR_JENNA, FACING_EAST, 0);
	syFieldEventWait(15);
	fdActorWalkToAndWait(ACTOR_JENNA, 728, 944);
	fdActorWalkToAndWait(ACTOR_JENNA, 728, 928);
	fdActorSetPos(ACTOR_JENNA, 728, 912);
	fdEventPlaySnd(SE_STAIRS_DOWN);
	scVenusLighthouseGoDownStairs(ACTOR_JENNA, -1);
	syFieldEventWait(40);
	/* Behind them the floor switch springs back up and the statue stands where
	 */
	/* it was. */
	fdMapGridCopyCellsTo(53, 50, 42, 49, 1, 1);
	fdMapGridCopyCellsTo(55, 117, 41, 117, 3, 5);
	fdEventClearFlag(FLAG_VENUS_LIGHTHOUSE_ON_GROUND_FLOOR_SWITCH);
	fdActorSetPos(ACTOR_FLOOR_SWITCH_STATUE, 664, 792);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	MainRoutineStateWork.leaderActorId = ACTOR_JENNA;
	gmPartyRemoveMember(FELIX);
	syFieldEventWait(50);
	state = Ram_HeapSlots->fieldState;
	state->transitionFrm = 60;
	state->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	syFieldEventSetExitRequest(VENUS_LIGHTHOUSE_FLOOR_SWITCH_TO_PILLARS);
}

extern u32 FieldEffect_LongScript[];

/* clang-format off */
u32 FieldEffect_ShortScript[] = {
    SC_WAIT(1),
    SC_LOOP(22, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
};

u32 FieldEffect_MediumScript[] = {
    SC_WAIT(1),
    SC_LOOP(44, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
};

u32 FieldEffect_LongScript[] = {
    SC_WAIT(1),
    SC_LOOP(126, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
};

#include "shared/push_leader_steps.c"
/* clang-format on */

const s32 *FieldEffect_DurationScripts[] = {
	FieldEffect_ShortScript,
	FieldEffect_MediumScript,
	FieldEffect_LongScript,
};

/* The leader's actor script as the statue throws him back */
/* (scVenusLighthouseRepelLeader). He swells and shrinks ten times, flashing */
/* (scVenusLighthouseFlashStruckLeader). */
s32 VenusLeaderDazedScript[] = {
	SC_ANIMATION(12),
	SC_SET(SW_HOOK, (s32)scVenusLighthouseFlashStruckLeader),
	SC_LABEL(1),
	SC_SET(SW_SCALE_X, 11 * FIXED_ONE / 10),
	SC_SET(SW_SCALE_Y, 11 * FIXED_ONE / 10),
	SC_WAIT(2),
	SC_SET(SW_SCALE_X, 9 * FIXED_ONE / 10),
	SC_SET(SW_SCALE_Y, 9 * FIXED_ONE / 10),
	SC_WAIT(2),
	SC_LOOP(10, SC_TO(1)),
	SC_SET(SW_SCALE_X, FIXED_ONE),
	SC_SET(SW_SCALE_Y, FIXED_ONE),
	SC_END,
};

SCENE_ENTRANCE SceneEntrances[] = {
	{0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{1, CONDITION_ALWAYS, 120, 0, 200, FACING_NORTH, 0, 0, 56, 240, 216, 0},
	{2, CONDITION_ALWAYS, 120, 0, 104, FACING_SOUTH, 0, 0, 56, 240, 216, 0},
	{3, CONDITION_ALWAYS, 344, 0, 232, FACING_NORTH, 0, 256, 48, 512, 248, 0},
	{4, CONDITION_ALWAYS, 472, 0, 184, FACING_SOUTH, 0, 256, 48, 512, 248, 0},
	{5, CONDITION_ALWAYS, 376, 0, 184, FACING_SOUTH, 0, 256, 48, 512, 248, 0},
	{6, CONDITION_ALWAYS, 344, 0, 104, FACING_SOUTH, 0, 256, 48, 512, 248, 0},
	{7, CONDITION_ALWAYS, 840, 0, 248, FACING_NORTH, 0, 672, 32, 1008, 256, 0},
	{8, CONDITION_ALWAYS, 968, 0, 168, FACING_SOUTH, 0, 672, 32, 1008, 256, 0},
	{9, CONDITION_ALWAYS, 856, 0, 88, FACING_SOUTH, 0, 672, 32, 1008, 256, 0},
	{10, CONDITION_ALWAYS, 808, 0, 88, FACING_SOUTH, 0, 672, 32, 1008, 256, 0},
	{11, CONDITION_ALWAYS, 712, 0, 168, FACING_SOUTH, 0, 672, 32, 1008, 256, 0},
	{12, CONDITION_ALWAYS, 200, 0, 408, FACING_NORTH, 0, 16, 288, 256, 520, 0},
	{13, CONDITION_ALWAYS, 104, 0, 504, FACING_NORTH, 0, 16, 288, 256, 520, 0},
	{14, CONDITION_ALWAYS, 104, 0, 472, FACING_SOUTH, 0, 16, 288, 256, 520, 0},
	{20, CONDITION_ALWAYS, 520, 0, 696, FACING_SOUTH, 0, 304, 344, 576, 728, 0},
	{21, CONDITION_ALWAYS, 344, 0, 568, FACING_SOUTH, 0, 304, 344, 576, 728, 0},
	{22, CONDITION_ALWAYS, 728, 0, 936, FACING_SOUTH, 0, 600, 736, 840, 952, 0},
	{23, CONDITION_ALWAYS, 744, 0, 792, FACING_SOUTH, 0, 600, 736, 840, 952, 0},
	{24, CONDITION_ALWAYS, 392, 0, 952, FACING_SOUTH, 0, 272, 808, 512, 984, 0},
	{25, CONDITION_ALWAYS, 392, 0, 856, FACING_SOUTH, 0, 272, 808, 512, 984, 0},
	{26, CONDITION_ALWAYS, 760, 0, 936, FACING_SOUTH, 0, 600, 736, 840, 952, 0},
	{30, CONDITION_ALWAYS, 88, 0, 872, FACING_SOUTH, 0, 16, 600, 256, 920, 0},
	{31, CONDITION_ALWAYS, 184, 0, 872, FACING_SOUTH, 0, 16, 600, 256, 920, 0},
	{32, CONDITION_ALWAYS, 184, 0, 648, FACING_SOUTH, 0, 16, 600, 256, 920, 0},
	{98, CONDITION_ALWAYS, 744, 0, 792, FACING_SOUTH, 0, 600, 736, 840, 952, 0},
	{99, CONDITION_ALWAYS, 744, 0, 792, FACING_SOUTH, 0, 576, 736, -1, 968, 0},
	{SCENE_TABLE_END},
};

u32 SceneExits[] = {
	SCENE_EXITS(VenusLighthouse),
	SCENE_EXIT(VENUS_LIGHTHOUSE_TO_VENUS_LIGHTHOUSE_ENTRANCE,
			   VenusLighthouseEntrance,
			   VENUS_LIGHTHOUSE_ENTRANCE_FROM_VENUS_LIGHTHOUSE),
	SCENE_EXIT(VENUS_LIGHTHOUSE_FRONT_ROOM_TO_RELIEFS, VenusLighthouse,
			   VENUS_LIGHTHOUSE_RELIEFS_FROM_FRONT_ROOM),
	SCENE_EXIT(VENUS_LIGHTHOUSE_RELIEFS_TO_FRONT_ROOM, VenusLighthouse,
			   VENUS_LIGHTHOUSE_FRONT_ROOM_FROM_RELIEFS),
	SCENE_EXIT(VENUS_LIGHTHOUSE_RELIEFS_TO_CARRY_STONE_EAST, VenusLighthouse,
			   VENUS_LIGHTHOUSE_CARRY_STONE_FROM_RELIEFS_EAST),
	SCENE_EXIT(VENUS_LIGHTHOUSE_RELIEFS_TO_CARRY_STONE_WEST, VenusLighthouse,
			   VENUS_LIGHTHOUSE_CARRY_STONE_FROM_RELIEFS_WEST),
	SCENE_EXIT(VENUS_LIGHTHOUSE_RELIEFS_TO_PILLARS, VenusLighthouse,
			   VENUS_LIGHTHOUSE_PILLARS_FROM_RELIEFS),
	SCENE_EXIT(VENUS_LIGHTHOUSE_PILLARS_TO_RELIEFS, VenusLighthouse,
			   VENUS_LIGHTHOUSE_RELIEFS_FROM_PILLARS),
	SCENE_EXIT(VENUS_LIGHTHOUSE_PILLARS_TO_CARRY_STONE, VenusLighthouse,
			   VENUS_LIGHTHOUSE_CARRY_STONE_FROM_PILLARS),
	SCENE_EXIT(VENUS_LIGHTHOUSE_PILLARS_TO_FLOOR_SWITCH, VenusLighthouse,
			   VENUS_LIGHTHOUSE_FLOOR_SWITCH_FROM_PILLARS),
	SCENE_EXIT(VENUS_LIGHTHOUSE_PILLARS_TO_STATUE_ROW_NORTH, VenusLighthouse,
			   VENUS_LIGHTHOUSE_STATUE_ROW_FROM_PILLARS_NORTH),
	SCENE_EXIT(VENUS_LIGHTHOUSE_PILLARS_TO_STATUE_ROW_WEST, VenusLighthouse,
			   VENUS_LIGHTHOUSE_STATUE_ROW_FROM_PILLARS_WEST),
	SCENE_EXIT(VENUS_LIGHTHOUSE_STATUE_ROW_TO_PILLARS_NORTH, VenusLighthouse,
			   VENUS_LIGHTHOUSE_PILLARS_FROM_STATUE_ROW_NORTH),
	SCENE_EXIT(VENUS_LIGHTHOUSE_STATUE_ROW_TO_PILLARS_WEST, VenusLighthouse,
			   VENUS_LIGHTHOUSE_PILLARS_FROM_STATUE_ROW_WEST),
	SCENE_EXIT(VENUS_LIGHTHOUSE_STATUE_ROW_TO_SMALL_ROOM, VenusLighthouse,
			   VENUS_LIGHTHOUSE_SMALL_ROOM_FROM_STATUE_ROW),
	SCENE_EXIT(VENUS_LIGHTHOUSE_GREAT_STATUE_TO_FLOOR_SWITCH, VenusLighthouse,
			   VENUS_LIGHTHOUSE_FLOOR_SWITCH_FROM_GREAT_STATUE),
	SCENE_EXIT(VENUS_LIGHTHOUSE_GREAT_STATUE_TO_SMALL_ROOM, VenusLighthouse,
			   VENUS_LIGHTHOUSE_SMALL_ROOM_FROM_GREAT_STATUE),
	SCENE_EXIT(VENUS_LIGHTHOUSE_FLOOR_SWITCH_TO_PILLARS, VenusLighthouse,
			   VENUS_LIGHTHOUSE_PILLARS_FROM_FLOOR_SWITCH),
	SCENE_EXIT(VENUS_LIGHTHOUSE_SMALL_ROOM_TO_STATUE_ROW, VenusLighthouse,
			   VENUS_LIGHTHOUSE_STATUE_ROW_FROM_SMALL_ROOM),
	SCENE_EXIT(VENUS_LIGHTHOUSE_SMALL_ROOM_TO_GREAT_STATUE, VenusLighthouse,
			   VENUS_LIGHTHOUSE_GREAT_STATUE_FROM_SMALL_ROOM),
	SCENE_EXIT(VENUS_LIGHTHOUSE_FLOOR_SWITCH_TO_GREAT_STATUE, VenusLighthouse,
			   VENUS_LIGHTHOUSE_GREAT_STATUE_FROM_FLOOR_SWITCH),
	SCENE_EXIT(VENUS_LIGHTHOUSE_CARRY_STONE_TO_RELIEFS_WEST, VenusLighthouse,
			   VENUS_LIGHTHOUSE_RELIEFS_FROM_CARRY_STONE_WEST),
	SCENE_EXIT(VENUS_LIGHTHOUSE_CARRY_STONE_TO_RELIEFS_EAST, VenusLighthouse,
			   VENUS_LIGHTHOUSE_RELIEFS_FROM_CARRY_STONE_EAST),
	SCENE_EXIT(VENUS_LIGHTHOUSE_CARRY_STONE_TO_PILLARS, VenusLighthouse,
			   VENUS_LIGHTHOUSE_PILLARS_FROM_CARRY_STONE),
	SCENE_EXITS_END,
};

/* The statues' actor script. They are drawn grey (VENUS_COLOR_MARK), and */
/* never pushed (SW_VENUS_IMMOVABLE). */
s32 VenusGreyStatueScript[] = {
	SC_SET(SW_TIMER, VENUS_COLOR_MARK(SPRITE_COLOR_GREY)),
	SC_SET(SW_VENUS_IMMOVABLE, 1),
	SC_CALL(scVenusLighthouseApplyMarkedCol),
	SC_END,
};

/* The actor script of the World Map's picture of Venus Lighthouse, placed */
/* at three fifths of its size, with no collision. */
s32 VenusMiniatureLighthouseScript[] = {
	SC_SET(SW_SCALE_X, 3 * FIXED_ONE / 5),
	SC_SET(SW_SCALE_Y, 3 * FIXED_ONE / 5),
	SC_SET(SW_COLLISION, 0),
	SC_CALL(scVenusLighthouseClearScriptedSprFlags),
	SC_END,
};

ACTOR_PLACE ActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0,
					8, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_ALEX, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8,
					FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_LADY_STATUE, CONDITION_ALWAYS,
					(s32)VenusGreyStatueScript, 664, 0, 792, FACING_SOUTH,
					TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_BLUE_CAP, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 8, 0, 12, FACING_SOUTH,
					TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_WITH_GLASSES, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 8, 0, 12, FACING_SOUTH,
					TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_BLUE_CAP, CONDITION_ALWAYS,
					MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_EAST,
					TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_SPIRAL_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
					872, 0, 120, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SPIRAL_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
					840, 0, 104, FACING_NORTH, TALK_FACING_KEEP, 1),
	ACTOR_PLACEMENT(SPRITE_WORLD_MAP_VENUS_LIGHTHOUSE, CONDITION_ALWAYS,
					(s32)VenusMiniatureLighthouseScript, 488, 0, 536,
					FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_LADY_STATUE, CONDITION_ALWAYS,
					(s32)VenusGreyStatueScript, 72, 0, 104, FACING_SOUTH,
					TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_LADY_STATUE, CONDITION_ALWAYS,
					(s32)VenusGreyStatueScript, 72, 0, 168, FACING_SOUTH,
					TALK_FACING_KEEP, 1),
	ACTOR_PLACEMENT(SPRITE_LADY_STATUE, CONDITION_ALWAYS,
					(s32)VenusGreyStatueScript, 168, 0, 104, FACING_SOUTH,
					TALK_FACING_KEEP, 1),
	ACTOR_PLACEMENT(SPRITE_LADY_STATUE, CONDITION_ALWAYS,
					(s32)VenusGreyStatueScript, 168, 0, 168, FACING_SOUTH,
					TALK_FACING_KEEP, 1),
	ACTOR_PLACEMENT(SPRITE_LADY_STATUE, CONDITION_ALWAYS,
					(s32)VenusGreyStatueScript, 72, 0, 344, FACING_SOUTH,
					TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_LADY_STATUE, CONDITION_ALWAYS,
					(s32)VenusGreyStatueScript, 104, 0, 344, FACING_SOUTH,
					TALK_FACING_KEEP, 1),
	ACTOR_PLACEMENT(SPRITE_LADY_STATUE, CONDITION_ALWAYS,
					(s32)VenusGreyStatueScript, 136, 0, 344, FACING_SOUTH,
					TALK_FACING_KEEP, 1),
	ACTOR_PLACEMENT(SPRITE_LADY_STATUE, CONDITION_ALWAYS,
					(s32)VenusGreyStatueScript, 168, 0, 344, FACING_SOUTH,
					TALK_FACING_KEEP, 1),
	ACTOR_PLACEMENT(SPRITE_LADY_STATUE, CONDITION_ALWAYS,
					(s32)VenusGreyStatueScript, 200, 0, 344, FACING_SOUTH,
					TALK_FACING_KEEP, 1),
	{SCENE_TABLE_END},
};

ACTOR_PLACE VenusLighthouseOpeningActorPlacements[] = {
	ACTOR_PLACEMENT(ACTOR_FELIX, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8,
					FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(ACTOR_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8,
					FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0,
					8, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_ALEX, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8,
					FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_LADY_STATUE, CONDITION_ALWAYS,
					(s32)VenusGreyStatueScript, 664, 0, 792, FACING_SOUTH,
					TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

TRIGGER SceneEvents[] = {
	{TRIGGER_EXIT, VENUS_LIGHTHOUSE_TO_VENUS_LIGHTHOUSE_ENTRANCE,
	 CONDITION_ALWAYS, VENUS_LIGHTHOUSE_TO_VENUS_LIGHTHOUSE_ENTRANCE},
	{TRIGGER_EXIT, VENUS_LIGHTHOUSE_FRONT_ROOM_TO_RELIEFS, CONDITION_ALWAYS,
	 VENUS_LIGHTHOUSE_FRONT_ROOM_TO_RELIEFS},
	{TRIGGER_EXIT, VENUS_LIGHTHOUSE_RELIEFS_TO_FRONT_ROOM, CONDITION_ALWAYS,
	 VENUS_LIGHTHOUSE_RELIEFS_TO_FRONT_ROOM},
	{TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN,
	 VENUS_LIGHTHOUSE_RELIEFS_TO_CARRY_STONE_EAST, CONDITION_ALWAYS,
	 VENUS_LIGHTHOUSE_RELIEFS_TO_CARRY_STONE_EAST},
	{TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN,
	 VENUS_LIGHTHOUSE_RELIEFS_TO_CARRY_STONE_WEST, CONDITION_ALWAYS,
	 VENUS_LIGHTHOUSE_RELIEFS_TO_CARRY_STONE_WEST},
	{TRIGGER_EXIT, VENUS_LIGHTHOUSE_RELIEFS_TO_PILLARS, CONDITION_ALWAYS,
	 VENUS_LIGHTHOUSE_RELIEFS_TO_PILLARS},
	{TRIGGER_EXIT, VENUS_LIGHTHOUSE_PILLARS_TO_RELIEFS, CONDITION_ALWAYS,
	 VENUS_LIGHTHOUSE_PILLARS_TO_RELIEFS},
	{TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN,
	 VENUS_LIGHTHOUSE_PILLARS_TO_CARRY_STONE, CONDITION_ALWAYS,
	 VENUS_LIGHTHOUSE_PILLARS_TO_CARRY_STONE},
	{TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP,
	 VENUS_LIGHTHOUSE_PILLARS_TO_FLOOR_SWITCH, CONDITION_ALWAYS,
	 VENUS_LIGHTHOUSE_PILLARS_TO_FLOOR_SWITCH},
	{TRIGGER_EXIT, VENUS_LIGHTHOUSE_PILLARS_TO_STATUE_ROW_NORTH,
	 CONDITION_ALWAYS, VENUS_LIGHTHOUSE_PILLARS_TO_STATUE_ROW_NORTH},
	{TRIGGER_EXIT, VENUS_LIGHTHOUSE_PILLARS_TO_STATUE_ROW_WEST,
	 CONDITION_ALWAYS, VENUS_LIGHTHOUSE_PILLARS_TO_STATUE_ROW_WEST},
	{TRIGGER_EXIT, VENUS_LIGHTHOUSE_STATUE_ROW_TO_PILLARS_NORTH,
	 CONDITION_ALWAYS, VENUS_LIGHTHOUSE_STATUE_ROW_TO_PILLARS_NORTH},
	{TRIGGER_EXIT, VENUS_LIGHTHOUSE_STATUE_ROW_TO_PILLARS_WEST,
	 CONDITION_ALWAYS, VENUS_LIGHTHOUSE_STATUE_ROW_TO_PILLARS_WEST},
	{TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP,
	 VENUS_LIGHTHOUSE_STATUE_ROW_TO_SMALL_ROOM, CONDITION_ALWAYS,
	 VENUS_LIGHTHOUSE_STATUE_ROW_TO_SMALL_ROOM},
	{TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN,
	 VENUS_LIGHTHOUSE_GREAT_STATUE_TO_FLOOR_SWITCH, CONDITION_ALWAYS,
	 VENUS_LIGHTHOUSE_GREAT_STATUE_TO_FLOOR_SWITCH},
	{TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN,
	 VENUS_LIGHTHOUSE_GREAT_STATUE_TO_SMALL_ROOM, CONDITION_ALWAYS,
	 VENUS_LIGHTHOUSE_GREAT_STATUE_TO_SMALL_ROOM},
	{TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN,
	 VENUS_LIGHTHOUSE_FLOOR_SWITCH_TO_PILLARS, CONDITION_ALWAYS,
	 VENUS_LIGHTHOUSE_FLOOR_SWITCH_TO_PILLARS},
	{TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN,
	 VENUS_LIGHTHOUSE_FLOOR_SWITCH_STAIRS, CONDITION_ALWAYS,
	 VENUS_LIGHTHOUSE_FLOOR_SWITCH_STAIRS},
	{TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN,
	 VENUS_LIGHTHOUSE_SMALL_ROOM_TO_STATUE_ROW, CONDITION_ALWAYS,
	 VENUS_LIGHTHOUSE_SMALL_ROOM_TO_STATUE_ROW},
	{TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP,
	 VENUS_LIGHTHOUSE_SMALL_ROOM_TO_GREAT_STATUE, CONDITION_ALWAYS,
	 VENUS_LIGHTHOUSE_SMALL_ROOM_TO_GREAT_STATUE},
	{TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP,
	 VENUS_LIGHTHOUSE_FLOOR_SWITCH_TO_GREAT_STATUE, CONDITION_ALWAYS,
	 VENUS_LIGHTHOUSE_FLOOR_SWITCH_TO_GREAT_STATUE},
	{TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP,
	 VENUS_LIGHTHOUSE_CARRY_STONE_TO_RELIEFS_WEST, CONDITION_ALWAYS,
	 VENUS_LIGHTHOUSE_CARRY_STONE_TO_RELIEFS_WEST},
	{TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP,
	 VENUS_LIGHTHOUSE_CARRY_STONE_TO_RELIEFS_EAST, CONDITION_ALWAYS,
	 VENUS_LIGHTHOUSE_CARRY_STONE_TO_RELIEFS_EAST},
	{TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP,
	 VENUS_LIGHTHOUSE_CARRY_STONE_TO_PILLARS, CONDITION_ALWAYS,
	 VENUS_LIGHTHOUSE_CARRY_STONE_TO_PILLARS},
	{TRIGGER_STEP, CELL_BEHAVIOR_VENUS_LIGHTHOUSE_BARRIER,
	 FLAG_VENUS_LIGHTHOUSE_SWITCH_HELD, EVENT(scVenusLighthouseRepelLeader)},
	{TRIGGER_STEP, CELL_BEHAVIOR_VENUS_LIGHTHOUSE_FLOOR_SWITCH,
	 CONDITION_ALWAYS, EVENT(scVenusLighthouseStepOnFloorSwitch)},
	{TRIGGER_EXAMINE, CELL_BEHAVIOR_VENUS_LIGHTHOUSE_FIRST_RELIEF,
	 CONDITION_ALWAYS, EVENT(scVenusLighthouseReadRelief)},
	{TRIGGER_EXAMINE, CELL_BEHAVIOR_VENUS_LIGHTHOUSE_FIRST_RELIEF + 1,
	 CONDITION_ALWAYS, EVENT(scVenusLighthouseReadRelief)},
	{TRIGGER_EXAMINE, CELL_BEHAVIOR_VENUS_LIGHTHOUSE_FIRST_RELIEF + 2,
	 CONDITION_ALWAYS, EVENT(scVenusLighthouseReadRelief)},
	{TRIGGER_EXAMINE, CELL_BEHAVIOR_VENUS_LIGHTHOUSE_FIRST_RELIEF + 3,
	 CONDITION_ALWAYS, EVENT(scVenusLighthouseReadRelief)},
	{TRIGGER_EXAMINE, CELL_BEHAVIOR_VENUS_LIGHTHOUSE_FIRST_RELIEF + 4,
	 CONDITION_ALWAYS, EVENT(scVenusLighthouseReadRelief)},
	{TRIGGER_EXAMINE, CELL_BEHAVIOR_VENUS_LIGHTHOUSE_TRUE_HEART_RELIEF,
	 CONDITION_ALWAYS, EVENT(scVenusLighthouseReadTrueHeart)},
	{PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
	 CONDITION_ALWAYS, EVENT(scVenusLighthouseRevealPaths)},
	{PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
	 CONDITION_ALWAYS, EVENT(scVenusLighthouseHidePaths)},
	{TRIGGER_TABLE_END},
};
#undef scFieldActorSetSprCol
#undef fdActorSetSprFlags
