/* Player control: the configured keys and moving actors by the pad. Part of
   field.c. */

#include "field_internal.h"

/* On the World Map, the keys whose R and L open the map screen and the zoomed
   map (newly pressed in every edition but the Japanese, held before), and how
   far ahead of the leader each step is probed, in pixels. */
#define FIELD_SHORTCUT_KEYS \
	WHEN(WORLD_MAP_SHORTCUT_FOLLOWS_PRESSED_KEY, PerPush, PerData)
#define WORLD_MAP_PROBE_RADIUS \
	WHEN(WORLD_MAP_SHORTCUT_FOLLOWS_PRESSED_KEY, toFIXED(7), toFIXED(8))

/* The leader's speeds and accelerations, in pixels a tick as 16.16 fixed
   point: the top 16 bits are whole pixels, so toFIXED(1) is one pixel. */
#define FIELD_WALK_SPEED toFIXED(1)		  /* 1 pixel a tick */
#define FIELD_RUN_SPEED	 (toFIXED(3) / 2) /* 1.5, with a dash key held */
#define FIELD_ACCELERATION \
	(toFIXED(1) / 4) /* a quarter pixel faster each tick */
#define FIELD_FAST_WALK_SPEED		 toFIXED(4) /* 4, B held with FLAG_FAST_WALK */
#define FIELD_FAST_WALK_ACCELERATION toFIXED(1)
#define WORLD_MAP_WALK_SPEED		 (toFIXED(1) / 2) /* half a pixel a tick */
#define WORLD_MAP_RUN_SPEED			 toFIXED(1) /* 1, with a dash key held */
#define WORLD_MAP_RUN_ACCELERATION	 (toFIXED(5) / 4)
#define WORLD_MAP_ACCELERATION		 (toFIXED(1) / 4)
#define WORLD_MAP_FAST_SPEED		 toFIXED(4) /* 4, while L repeats */

/* 22.5 degrees: a step is probed this far either side of its heading, and a
   blocked step tries headings one, two and three times this far off. */
#define FIELD_PROBE_ANGLE		(TRIG_TURN / 16)
/* A blocked leader turns toward the pad at most 22.5 degrees a tick. */
#define FIELD_BLOCKED_TURN_MAX	(TRIG_TURN / 16)
/* The World Map camera turns toward the pad's yaw by at most about 2.8 degrees
   a tick. */
#define WORLD_MAP_YAW_STEP_MAX	(TRIG_TURN / 128)
/* The ticks between footprints, running and walking. */
#define FOOTPRINT_RUN_INTERVAL	12
#define FOOTPRINT_WALK_INTERVAL 18

/* Puts position distance along heading from actor: where a step is probed. */
#define PROBE_FROM(actor, distance, heading, position) \
	((position).x = (actor)->x.fixed, (position).y = (actor)->y.fixed, \
		(position).z = (actor)->z.fixed, \
		syVectorAddPolarOffset(distance, heading, (FIXED *)&(position)))

/* What a shortcut key's assignment requests: its top two bits. */
typedef enum FIELD_SHORTCUT_KIND {
	FIELD_SHORTCUT_PSYNERGY = 0, /* fieldState->psynergy, an ability */
	FIELD_SHORTCUT_ITEM = 1		 /* fieldState->item */
} FIELD_SHORTCUT_KIND;

/* A shortcut key's assignment: its kind in the top two bits, from this one, and
   its request id in the bits below. */
#define FIELD_SHORTCUT_KIND_SHIFT 14
#define FIELD_SHORTCUT_ID_MASK	  ((1 << FIELD_SHORTCUT_KIND_SHIFT) - 1)

/* The debug beep sounds while fewer resource blocks than this are free. */
#define DEBUG_RESOURCE_BEEP_FREE_BLOCKS 136

/* The World Map camera's turn is finished at once when its step is smaller than
   this. */
#define WORLD_MAP_YAW_STEP_MIN 16

/* What stopped the leader's step this tick (fdActorUpdatePlayerCtrl,
   fdActorMoveOnWorldMap). */
typedef enum PLAYER_STEP_BLOCK {
	PLAYER_STEP_BLOCKED_BY_MAP = 1, /* every heading tried runs into the map */
	/* A solid actor ahead, or one with no room to be pushed. */
	PLAYER_STEP_BLOCKED_BY_ACTOR = 2,
	PLAYER_STEP_BLOCKED = 3, /* either of the two */
	/* No step, and none counted as blocked: the pad is not held, or a climb
	   meets a wall. */
	PLAYER_STEP_STOPPED = 4
} PLAYER_STEP_BLOCK;

/* The footprints left in snow such as Imil's (FIELD_SPRITE_FOOTPRINT,
   footprints.png), and the leader's userValue that asks for the pair next. */
typedef enum FOOTPRINT_ANIMATION {
	FOOTPRINT_ONE_FOOT = 1, /* one print, fading */
	/* Two prints side by side, fading: laid first after a stop. */
	FOOTPRINT_BOTH_FEET = 2
} FOOTPRINT_ANIMATION;

/* Unused: stores value as a word in the actor's update function. */
UNUSED void fdActorSetUpdateValue(void *rawActor, Sint32 value)
{
	ACTOR *actor = rawActor;

	if(actor != NULL)
		*(Sint32 *)&actor->update = value;
}

/* Handles a shortcut key's assignment: its top two bits choose what it requests
   (FIELD_SHORTCUT_KIND) and the low 14 bits which one. */
Uint32 fdRequestShortcut(Uint32 encodedAssignment)
{
	Uint32 assignmentKind = encodedAssignment >> FIELD_SHORTCUT_KIND_SHIFT;
	Uint32 requestId = FIELD_SHORTCUT_ID_MASK & encodedAssignment;
	FIELD_STATE *state = FieldStateWork;

	if(gmFlagGetStub(FLAG_MENUS_RAISE_CUE) != 0) {
		state->raisedCue = FIELD_RAISED_CUE_MENUS;
	} else if(state->mode == ACTOR_SYSTEM_WORLD_MAP) {
		if(FIELD_SHORTCUT_KEYS & KEY_R) {
			state->raisedCue = FIELD_RAISED_CUE_MAP_SCREEN;
		} else if(FIELD_SHORTCUT_KEYS & KEY_L) {
			state->raisedCue = FIELD_RAISED_CUE_WORLD_MAP_ZOOM;
		}
	} else {
		switch(assignmentKind) {
		case FIELD_SHORTCUT_PSYNERGY:
			state->psynergy = requestId;
			break;
		case FIELD_SHORTCUT_ITEM:
			state->item = requestId;
			break;
		}
	}

	return requestId;
}

/* Checks the keys the player has configured: Select, A and Start set their flag
   in the field state and return 1; the two shortcut keys pass on their
   assignment (fdRequestShortcut) and return its request id. */
Sint32 fdCheckConfiguredKeys(void)
{
	FIELD_STATE *state = FieldStateWork;
	Sint32		 result = 0;

	if(state == NULL)
		return 0;
	if(PerPush & MainRoutineStateWork.selectKeys) {
		state->fieldMenu = 1;
		result = 1;
	} else if(PerPush & MainRoutineStateWork.aKeys) {
		state->aButton = 1;
		result = 1;
	} else if(PerPush & MainRoutineStateWork.startKeys) {
		state->pauseMenu = 1;
		result = 1;
	} else if(PerPush & MainRoutineStateWork.firstShortcutKeys) {
		result = fdRequestShortcut(MainRoutineStateWork.firstShortcut);
	} else if(PerPush & MainRoutineStateWork.secondShortcutKeys) {
		result = fdRequestShortcut(MainRoutineStateWork.secondShortcut);
	}
	return result;
}

/* Returns 0 when two spheres overlap: at positions a and b (16.16 pixels) with
   radii radiusA and radiusB (pixels); -1 otherwise. */
Sint32 fdCheckSpheresOverlap(
	FIXED *firstPos, Sint32 radiusA, FIXED *secondPos, Sint32 radiusB)
{
	const FIELD_POS *firstPoint = (const FIELD_POS *)firstPos;
	const FIELD_POS *secondPoint = (const FIELD_POS *)secondPos;
	const Sint32	*firstCoordinate = &firstPoint->x;
	const Sint32	*secondCoordinate = &secondPoint->x;
	Sint32			 dx = FIXEDtoINT(*firstCoordinate++ - *secondCoordinate++);
	Sint32			 dy = FIXEDtoINT(*firstCoordinate++ - *secondCoordinate++);
	Sint32			 dz = FIXEDtoINT(*firstCoordinate - *secondCoordinate);
	Sint32			 radius = radiusA + radiusB;

	/* The bound is 64 pixels in 16.16, the units before the shift, but dx and
	   dz are already whole pixels (at most 32768), so these tests always pass
	   and only the distance decides. */
	if(!(dx > toFIXED(64)) && !(dz > toFIXED(64)) &&
		dx * dx + dy * dy + dz * dz < radius * radius)
		return 0;
	return -1;
}

/* SC_PLAYER: moves the player on the field by the direction pad: walking or
   running, sliding around walls, pushing movable actors, turning, playing the
   matching animation and leaving footprints. */
Sint32 fdActorUpdatePlayerCtrl(ACTOR *actor)
{
	FIELD_POS pos;
	FIELD_POS testPos;
	FIELD_POS contact;
	Sint16	  angOffsets[22];
	Sint32	  collision;
	Sint32	  pushed;
	Sint16	  heading;
	Sint32	  ang;
	Sint32	  movementAnm;
	Uint16	  direction;
	Sint32	  footprintAng;
	Sint32	  angIdx;
	Sint32	  headingCnt;
	ACTOR	 *pushedActor;

	collision = 0;
	pushed = 0;
	/* Debug (FLAG_DEBUG_RESOURCE_BEEP): beep when few resource blocks are
	   free. */
	if(DebugMode != 0 && gmFlagGetStub(FLAG_DEBUG_RESOURCE_BEEP) != 0) {
		Sint32 freeBlocks;
		Sint32 blockIdx;

		freeBlocks = 0;
		for(blockIdx = 0; blockIdx < VRAM_BLOCK_COUNT; blockIdx++) {
			if(VramCacheBlockOwners[blockIdx] == VRAM_BLOCK_OWNER_FREE)
				freeBlocks++;
		}
		if(freeBlocks - DEBUG_RESOURCE_BEEP_FREE_BLOCKS < 0)
			sySndPlayStub(SE_CLICK);
	}
	/* Debug: holding L stalls each tick in four empty loops, the last three
	   counting in a halfword. */
	if(DebugMode != 0) {
		if(PerData & KEY_L) {
			Sint32 i;
			Sint16 j;

			for(i = 0; i < 512; i++) {
			}
			for(j = 0; j < 96; j++) {
			}
			for(j = 0; j < 64; j++) {
			}
			for(j = 0; j < 64; j++) {
			}
		}
	}
	/* Walk, or run with the dash keys; with FLAG_FAST_WALK, B moves faster
	   still. */
	if(PerData & MainRoutineStateWork.dashKeys) {
		actor->speedLimit = FIELD_RUN_SPEED;
		actor->acceleration = FIELD_ACCELERATION;
		movementAnm = ANIM_LEADER_RUN;
	} else {
		actor->speedLimit = FIELD_WALK_SPEED;
		actor->acceleration = FIELD_ACCELERATION;
		movementAnm = ANIM_WALK;
	}
	if(gmFlagGetStub(FLAG_FAST_WALK) != 0 && (PerData & KEY_B)) {
		actor->speedLimit = FIELD_FAST_WALK_SPEED;
		actor->acceleration = FIELD_FAST_WALK_ACCELERATION;
		movementAnm = ANIM_LEADER_RUN;
	}

	/* Step 8 pixels the way the direction pad points, when that step and the
	   ones 22.5 and 45 degrees either side are clear (debug L walks through
	   walls); otherwise try headings up to 67.5 degrees off. */
	ang = WorldMapKeyHeadings[DPAD_INDEX(PerData)];
	if((Uint16)ang == WORLD_MAP_KEY_HEADING_NONE) {
		collision |= PLAYER_STEP_STOPPED;
	} else {
		collision = 0;
		PROBE_FROM(actor, toFIXED(8), (Uint16)ang, pos);
		if(DebugMode != 0 && (PerData & KEY_L)) {
			heading = ang;
		} else {
			if(fdMapGridCheckCollision(actor, &pos) == COLLISION_NONE) {
				PROBE_FROM(actor, toFIXED(8), (Uint16)ang + FIELD_PROBE_ANGLE,
					testPos);
				if(fdMapGridCheckCollision(actor, &testPos) == COLLISION_NONE) {
					PROBE_FROM(actor, toFIXED(8),
						(Uint16)ang - FIELD_PROBE_ANGLE, testPos);
					if(fdMapGridCheckCollision(actor, &testPos) ==
						COLLISION_NONE) {
						PROBE_FROM(actor, toFIXED(8),
							(Uint16)ang + DEGtoANG(45), testPos);
						if(fdMapGridCheckCollision(actor, &testPos) ==
							COLLISION_NONE) {
							PROBE_FROM(actor, toFIXED(8),
								(Uint16)ang - DEGtoANG(45), testPos);
							if(fdMapGridCheckCollision(actor, &testPos) ==
								COLLISION_NONE) {
								heading = ang;
								goto push_actors;
							}
						}
					}
				}
			}
			/* The pad's way is blocked: try 22.5, 45 and 67.5 degrees off,
			   nearest first, each needing the same five clear probes. */
			direction = ang;
			headingCnt = 0;
			angOffsets[headingCnt++] = direction + FIELD_PROBE_ANGLE;
			angOffsets[headingCnt++] = direction - FIELD_PROBE_ANGLE;
			angOffsets[headingCnt++] = direction + DEGtoANG(45);
			angOffsets[headingCnt++] = direction - DEGtoANG(45);
			angOffsets[headingCnt++] = direction + FIELD_PROBE_ANGLE * 3;
			angOffsets[headingCnt++] = direction - FIELD_PROBE_ANGLE * 3;
			for(angIdx = 0; angIdx < headingCnt; angIdx++) {
				heading = angOffsets[angIdx];
				PROBE_FROM(actor, toFIXED(8), (Uint16)heading, pos);
				if(fdMapGridCheckCollision(actor, &pos) != COLLISION_NONE)
					continue;
				PROBE_FROM(actor, toFIXED(8),
					(Uint16)heading + FIELD_PROBE_ANGLE, testPos);
				if(fdMapGridCheckCollision(actor, &testPos) != COLLISION_NONE)
					continue;
				PROBE_FROM(actor, toFIXED(8),
					(Uint16)heading - FIELD_PROBE_ANGLE, testPos);
				if(fdMapGridCheckCollision(actor, &testPos) != COLLISION_NONE)
					continue;
				PROBE_FROM(
					actor, toFIXED(8), (Uint16)heading + DEGtoANG(45), testPos);
				if(fdMapGridCheckCollision(actor, &testPos) != COLLISION_NONE)
					continue;
				PROBE_FROM(
					actor, toFIXED(8), (Uint16)heading - DEGtoANG(45), testPos);
				if(fdMapGridCheckCollision(actor, &testPos) == COLLISION_NONE)
					goto push_actors;
			}
			/* Nothing passed: the step is the actor's own position. */
			pos.x = actor->x.fixed;
			pos.y = actor->y.fixed;
			pos.z = actor->z.fixed;
			collision |= PLAYER_STEP_BLOCKED_BY_MAP;

		push_actors:
			/* Push the movable actors (ACTOR_PUSHABLE) just ahead 4 pixels
			   further, when they have room; any other solid actor there blocks
			   the step (bit 1 of collision). Pushing slows the player down. */
			PROBE_FROM(actor, toFIXED(4), (Uint16)heading, contact);
			pushedActor = ActorsWork;
			for(angIdx = 0; angIdx < ACTOR_COUNT; angIdx++, pushedActor++) {
				Sint32 radius;

				radius = actor->radius - 2;
				if(pushedActor->script == 0)
					continue;
				if((pushedActor->collisionFlg & ACTOR_SOLID) == 0)
					continue;
				if(pushedActor == actor)
					continue;
				if(fdCheckSpheresOverlap(&pushedActor->x.fixed, radius,
					   &contact.x, pushedActor->radius - 2) < 0)
					continue;
				/* A movable actor that is not paused slides a quarter pixel
				   away from the player, if no solid actor is there and the
				   ground 10 pixels on, straight and 22.5 degrees either side,
				   is clear. */
				if((pushedActor->collisionFlg & ACTOR_PUSHABLE) != 0 &&
					pushedActor->paused == 0) {
					direction = syAtan(pushedActor->z.fixed - actor->z.fixed,
						pushedActor->x.fixed - actor->x.fixed);
					testPos.x = pushedActor->x.fixed;
					testPos.y = pushedActor->y.fixed;
					testPos.z = pushedActor->z.fixed;
					heading = direction;
					syVectorAddPolarOffset(
						toFIXED(1) / 4, direction, (FIXED *)&testPos);
					if(!fdActorScriptCheckOverlap(
						   pushedActor, (Sint32 *)&testPos)) {
						PROBE_FROM(
							pushedActor, toFIXED(10), direction, testPos);
						if(fdMapGridCheckCollision(pushedActor, &testPos) ==
							COLLISION_NONE) {
							PROBE_FROM(pushedActor, toFIXED(10),
								direction + FIELD_PROBE_ANGLE, testPos);
							/* The same probe is tested twice. */
							if(fdMapGridCheckCollision(pushedActor, &testPos) ==
									COLLISION_NONE &&
								fdMapGridCheckCollision(
									pushedActor, &testPos) == COLLISION_NONE) {
								PROBE_FROM(pushedActor, toFIXED(10),
									direction - FIELD_PROBE_ANGLE, testPos);
								if(fdMapGridCheckCollision(pushedActor,
									   &testPos) == COLLISION_NONE) {
									syVectorAddPolarOffset(toFIXED(1) / 4,
										direction,
										(FIXED *)&pushedActor->x.fixed);
									pushedActor->tgtX = ACTOR_NO_TARGET;
									pushedActor->tgtY = ACTOR_NO_TARGET;
									pushedActor->tgtZ = ACTOR_NO_TARGET;
									pushed |= 1;
									continue;
								}
							}
						}
					}
				}
				collision |= PLAYER_STEP_BLOCKED_BY_ACTOR;
			}
			/* Pushing: a quarter pixel a tick. */
			if(collision == 0 && pushed != 0) {
				actor->speedLimit = toFIXED(1) / 4;
				actor->acceleration = toFIXED(1) / 8;
			}
		}
	}
	/* Count the blocked steps, and pick the animation: pushing; stopped,
	   standing and in time glancing about (leaning back when the leader has no
	   HP); or walking or running. */
	if(FieldStateWork != NULL) {
		if(collision & PLAYER_STEP_BLOCKED)
			FieldStateWork->blockedSteps++;
		else
			FieldStateWork->blockedSteps = 0;
	}
	if(pushed != 0) {
		fdActorSelectAnimation(actor, ANIM_PUSH);
	} else if(collision != 0) {
		Sint32 action;

		action = ANIM_LEADER_LOOK_AROUND;
		if(gmUnitGetStub(MainRoutineStateWork.leaderActorId)->hp == 0)
			action = ANIM_LEADER_LEAN_BACK;
		fdActorSelectAnimation(actor, action);
	} else {
		fdActorSelectAnimation(actor, movementAnm);
	}

	/* Blocked: stop and turn towards the pad, at most 22.5 degrees a tick; the
	   next footprint is the pair. */
	if(collision != 0) {
		actor->tgtX = ACTOR_NO_TARGET;
		actor->tgtY = ACTOR_NO_TARGET;
		actor->tgtZ = ACTOR_NO_TARGET;
		actor->velX = 0;
		actor->velZ = 0;
		if(collision & PLAYER_STEP_BLOCKED) {
			Sint32 difference;

			difference = (Uint16)ang - actor->facing;
			difference = (Sint16)difference;
			if(difference > FIELD_BLOCKED_TURN_MAX)
				difference = FIELD_BLOCKED_TURN_MAX;
			if(difference < -FIELD_BLOCKED_TURN_MAX)
				difference = -FIELD_BLOCKED_TURN_MAX;
			actor->facing += difference;
		}
		actor->tmr = 0;
		actor->userValue = FOOTPRINT_BOTH_FEET;
	} else {
		FIXED speed;

		fdActorSetMoveTarget(actor, pos.x, pos.y, pos.z);
		speed = sySquartFX(syMulFX(actor->velX, actor->velX) +
			syMulFX(actor->velZ, actor->velZ));
		actor->velX = 0;
		actor->velZ = 0;
		syVectorAddPolarOffset(speed, (Uint16)heading, (FIXED *)&actor->velX);
		if(actor->tmr != 0)
			actor->tmr--;
	}

	/* Leave a footprint every 18 walking or 12 running ticks on maps that show
	   them, alternating left and right; timer counts down the ticks to the
	   next. */
	footprintAng = (Uint16)ang;
	if(((MAP_GRID_SCROLL_STATE *)MapGridState[0])->footprints != 0 &&
		actor->tmr == 0 && collision == 0) {
		ACTOR *effect;

		effect = fdActorCreate(FIELD_SPRITE_FOOTPRINT, actor->x.fixed,
			actor->y.fixed, actor->z.fixed);
		if(effect != 0) {
			SPR *anm;

			/* collision is 0 here: the footprint takes no motion flags (it
			   stays where it is laid) and its sprite no shadow. */
			effect->groundHeight = actor->groundHeight;
			anm = effect->spr;
			fdActorStartScript(effect, WorldMapFootprintActions);
			effect->priFlg = ACTOR_PRIORITY_UNDERFOOT;
			effect->motionFlg = collision;
			if(anm != 0) {
				fdSprStartAnimation(anm, FOOTPRINT_ONE_FOOT);
				anm->flg = collision;
				anm->rotation = footprintAng + DEGtoANG(90);
				anm->image.pri = 3;
			}
			/* The first footprint after a stop is the pair and restarts the
			   sides; each second one faces a half turn round. */
			if(actor->userValue == FOOTPRINT_BOTH_FEET) {
				fdSprStartAnimation(anm, FOOTPRINT_BOTH_FEET);
				actor->userValue = collision;
			}
			if(actor->userValue != 0)
				effect->facing = DEGtoANG(180);
			if(movementAnm == ANIM_LEADER_RUN)
				actor->tmr = FOOTPRINT_RUN_INTERVAL;
			else
				actor->tmr = FOOTPRINT_WALK_INTERVAL;
			actor->userValue ^= 1;
		}
	}
	fdCheckConfiguredKeys();
	actor->scriptCursor++;
	return 1;
}

/* SC_WORLD: moves the player on the World Map by the direction pad, as
   SC_PLAYER does on the field, and turns the World Map camera with the pad. */
Sint32 fdActorMoveOnWorldMap(ACTOR *actor)
{
	FIELD_POS position;
#if WORLD_MAP_FIVE_PROBE_STEPS
	FIELD_POS testPosition;
	Sint16	  angleOffsets[28];
#else
	FIELD_POS leftPosition;
	FIELD_POS rightPosition;
	Sint16	  angleOffsets[22];
#endif
	Sint32			collision;
	Sint32			angle;
	Uint16			direction;
	Sint32			angleIndex;
	Sint32			movementAnimation;
	Sint32			count;
	Sint32			difference;
	Sint32			terrainCollision;
	FIXED			speed;
	Sint32			targetCameraYaw;
	WORLD_MAP_VIEW *perspective;
	Sint32			yawStep;
	SPR			   *playerSprite;
	ACTOR		   *ripple;
	SPR			   *rippleSprite;

	movementAnimation = ANIM_WALK;
	collision = 0;
#if !WORLD_MAP_FIVE_PROBE_STEPS
	count = 0;
#endif
	/* Walk, or run with the dash keys. */
	if(PerData & MainRoutineStateWork.dashKeys) {
		actor->speedLimit = WORLD_MAP_RUN_SPEED;
		actor->acceleration = WORLD_MAP_RUN_ACCELERATION;
		movementAnimation = ANIM_LEADER_RUN;
	} else {
		actor->speedLimit = WORLD_MAP_WALK_SPEED;
		actor->acceleration = WORLD_MAP_ACCELERATION;
	}
	if((PerRepeat & KEY_L) != 0)
		actor->speedLimit = WORLD_MAP_FAST_SPEED;

	angle = WorldMapKeyHeadings[DPAD_INDEX(PerData)];
	direction = angle;
	if(direction == WORLD_MAP_KEY_HEADING_NONE) {
		collision |= PLAYER_STEP_STOPPED;
	} else {
		collision = 0;
		PROBE_FROM(actor, WORLD_MAP_PROBE_RADIUS, direction, position);
		/* Debug L walks anywhere. */
		if(DebugMode == 0 || (PerData & KEY_L) == 0) {
#if WORLD_MAP_FIVE_PROBE_STEPS
			if(fdWorldMapIsWalkable((Sint32)actor, (WORLD_POS *)&position) ==
				0) {
				PROBE_FROM(actor, WORLD_MAP_PROBE_RADIUS,
					direction + FIELD_PROBE_ANGLE, testPosition);
				if(fdWorldMapIsWalkable(
					   (Sint32)actor, (WORLD_POS *)&testPosition) == 0) {
					PROBE_FROM(actor, WORLD_MAP_PROBE_RADIUS,
						direction - FIELD_PROBE_ANGLE, testPosition);
					if(fdWorldMapIsWalkable(
						   (Sint32)actor, (WORLD_POS *)&testPosition) == 0) {
						PROBE_FROM(actor, WORLD_MAP_PROBE_RADIUS,
							direction + DEGtoANG(45), testPosition);
						if(fdWorldMapIsWalkable((Sint32)actor,
							   (WORLD_POS *)&testPosition) == 0) {
							PROBE_FROM(actor, WORLD_MAP_PROBE_RADIUS,
								direction - DEGtoANG(45), testPosition);
							if(fdWorldMapIsWalkable((Sint32)actor,
								   (WORLD_POS *)&testPosition) == 0)
								goto update_actor;
						}
					}
				}
			}
			count = 0;
			angleOffsets[count++] = direction + FIELD_PROBE_ANGLE;
			angleOffsets[count++] = direction - FIELD_PROBE_ANGLE;
			angleOffsets[count++] = direction + DEGtoANG(45);
			angleOffsets[count++] = direction - DEGtoANG(45);
			angleOffsets[count++] = direction + FIELD_PROBE_ANGLE * 3;
			angleOffsets[count++] = direction - FIELD_PROBE_ANGLE * 3;
			angleOffsets[count++] = direction + DEGtoANG(90);
			angleOffsets[count++] = direction - DEGtoANG(90);
			for(angleIndex = 0; angleIndex < count; angleIndex++) {
				direction = angleOffsets[angleIndex];
				PROBE_FROM(actor, WORLD_MAP_PROBE_RADIUS, direction, position);
				if(fdWorldMapIsWalkable(
					   (Sint32)actor, (WORLD_POS *)&position) != 0)
					continue;
				PROBE_FROM(actor, WORLD_MAP_PROBE_RADIUS,
					direction + FIELD_PROBE_ANGLE, testPosition);
				if(fdWorldMapIsWalkable(
					   (Sint32)actor, (WORLD_POS *)&testPosition) != 0)
					continue;
				PROBE_FROM(actor, WORLD_MAP_PROBE_RADIUS,
					direction - FIELD_PROBE_ANGLE, testPosition);
				if(fdWorldMapIsWalkable(
					   (Sint32)actor, (WORLD_POS *)&testPosition) != 0)
					continue;
				PROBE_FROM(actor, WORLD_MAP_PROBE_RADIUS,
					direction + DEGtoANG(45), testPosition);
				if(fdWorldMapIsWalkable(
					   (Sint32)actor, (WORLD_POS *)&testPosition) != 0)
					continue;
				PROBE_FROM(actor, WORLD_MAP_PROBE_RADIUS,
					direction - DEGtoANG(45), testPosition);
				if(fdWorldMapIsWalkable(
					   (Sint32)actor, (WORLD_POS *)&testPosition) != 0)
					continue;
				angle = (Sint16)direction;
				goto update_actor;
			}
			collision |= PLAYER_STEP_BLOCKED_BY_MAP;

#else
			PROBE_FROM(actor, WORLD_MAP_PROBE_RADIUS, direction + DEGtoANG(30),
				leftPosition);
			PROBE_FROM(actor, WORLD_MAP_PROBE_RADIUS, direction - DEGtoANG(30),
				rightPosition);
			if((fdWorldMapIsWalkable((Sint32)actor, (WORLD_POS *)&position) |
				   fdWorldMapIsWalkable(
					   (Sint32)actor, (WORLD_POS *)&leftPosition) |
				   fdWorldMapIsWalkable(
					   (Sint32)actor, (WORLD_POS *)&rightPosition)) != 0) {
				angleOffsets[count++] = direction + FIELD_PROBE_ANGLE;
				angleOffsets[count++] = direction - FIELD_PROBE_ANGLE;
				angleOffsets[count++] = direction + DEGtoANG(45);
				angleOffsets[count++] = direction - DEGtoANG(45);
				angleOffsets[count++] = direction + FIELD_PROBE_ANGLE * 3;
				angleOffsets[count++] = direction - FIELD_PROBE_ANGLE * 3;
				for(angleIndex = 0; angleIndex < count; angleIndex++) {
					direction = angleOffsets[angleIndex];
					PROBE_FROM(
						actor, WORLD_MAP_PROBE_RADIUS, direction, position);
					PROBE_FROM(actor, WORLD_MAP_PROBE_RADIUS,
						direction + DEGtoANG(30), leftPosition);
					PROBE_FROM(actor, WORLD_MAP_PROBE_RADIUS,
						direction - DEGtoANG(30), rightPosition);
					if((fdWorldMapIsWalkable(
							(Sint32)actor, (WORLD_POS *)&position) |
						   fdWorldMapIsWalkable(
							   (Sint32)actor, (WORLD_POS *)&leftPosition) |
						   fdWorldMapIsWalkable((Sint32)actor,
							   (WORLD_POS *)&rightPosition)) != 0)
						continue;
					angle = (Sint16)direction;
					goto update_actor;
				}
				collision |= PLAYER_STEP_BLOCKED_BY_MAP;
			}
#endif
		}
	}

update_actor:
	if(FieldStateWork != NULL) {
		if((collision & PLAYER_STEP_BLOCKED) != 0)
			FieldStateWork->blockedSteps++;
		else
			FieldStateWork->blockedSteps = 0;
	}
	if(collision != 0)
		fdActorSelectAnimation(actor, ANIM_LEADER_LOOK_AROUND);
	else
		fdActorSelectAnimation(actor, movementAnimation);

	if(collision != 0) {
		actor->tgtX = ACTOR_NO_TARGET;
		actor->tgtY = ACTOR_NO_TARGET;
		actor->tgtZ = ACTOR_NO_TARGET;
		actor->velX = 0;
		actor->velZ = 0;
		if((collision & PLAYER_STEP_BLOCKED) != 0) {
			direction = angle;
			difference = (Sint16)(direction - actor->facing);
			if(difference > FIELD_BLOCKED_TURN_MAX)
				difference = FIELD_BLOCKED_TURN_MAX;
			if(difference < -FIELD_BLOCKED_TURN_MAX)
				difference = -FIELD_BLOCKED_TURN_MAX;
			actor->facing += difference;
		}
		actor->tmr = 0;
		actor->userValue = FOOTPRINT_BOTH_FEET;
	} else {
		fdActorSetMoveTarget(actor, position.x, position.y, position.z);
		speed = sySquartFX(syMulFX(actor->velX, actor->velX) +
			syMulFX(actor->velZ, actor->velZ));
		/* collision is 0 here: keep the speed, along the chosen heading. */
		actor->velX = collision;
		actor->velZ = collision;
		syVectorAddPolarOffset(speed, (Uint16)angle, (FIXED *)&actor->velX);
		if(actor->tmr != 0)
			actor->tmr--;
	}

	/* Turn the camera towards the pad's yaw by an eighth of the difference a
	   tick, at most WORLD_MAP_YAW_STEP_MAX, finishing the turn once the step
	   falls under WORLD_MAP_YAW_STEP_MIN. */
	perspective = MapGridState[0];
	targetCameraYaw = WorldMapKeyYaws[DPAD_INDEX(PerData)];
	yawStep = (Sint16)(targetCameraYaw - perspective->yaw) / 8;
	if(yawStep > WORLD_MAP_YAW_STEP_MAX)
		yawStep = WORLD_MAP_YAW_STEP_MAX;
	if(yawStep < -WORLD_MAP_YAW_STEP_MAX)
		yawStep = -WORLD_MAP_YAW_STEP_MAX;
	if(yawStep > -WORLD_MAP_YAW_STEP_MIN && yawStep < WORLD_MAP_YAW_STEP_MIN)
		yawStep = targetCameraYaw - perspective->yaw;
	perspective->yaw += yawStep;

	if(actor->sprStorageKind == ACTOR_SPRITE_ONE) {
		playerSprite = actor->spr;
		terrainCollision = fdWorldMapGetCollision((WORLD_POS *)&actor->x.fixed);
		/* In a forest the sprite's second sprite animation is drawn (priority
		   1) and its shadow is not; elsewhere that sprite animation takes
		   priority 9, and a sprite animation above 3 is not drawn. */
		if(terrainCollision == WORLD_MAP_COLLISION_FOREST) {
			playerSprite->entries[1]->pri = 1;
			playerSprite->flg = 0;
		} else {
			playerSprite->entries[1]->pri = 9;
			playerSprite->flg = SPRITE_FLAG_SHADOW;
		}
		/* In shallow water a ripple follows every 10 ticks of walking: it
		   lasts 30 ticks, takes no motion flags, and its sprite, with no
		   shadow, is drawn semi-transparent at priority 2. */
		if(terrainCollision == WORLD_MAP_COLLISION_SHALLOW_WATER &&
			actor->tmr == 0 && collision == 0) {
			ripple = fdActorCreate(FIELD_SPRITE_RIPPLE, actor->x.fixed,
				actor->y.fixed, actor->z.fixed);
			if(ripple != 0) {
				rippleSprite = ripple->spr;
				fdActorStartScript(ripple, WorldMapFootprintActions + 3);
				ripple->motionFlg = collision;
				ripple->layer = 1;
				if(rippleSprite != 0) {
					fdSprStartAnimation(rippleSprite, 1);
					rippleSprite->flg = collision;
					rippleSprite->image.objMode = OBJ_MODE_SEMI_TRANSPARENT;
					rippleSprite->image.pri = 2;
				}
				actor->tmr = 10;
			}
		}
	}
	fdCheckConfiguredKeys();
	actor->scriptCursor++;
	return 1;
}

/* SC_KEY_COUNT: checks the configured keys (fdCheckConfiguredKeys). Returns
   1. */
Sint32 fdCheckConfiguredKeysAndCount(ACTOR *actor)
{
	fdCheckConfiguredKeys();
	actor->scriptCursor++;
	return 1;
}

/* SC_KEYS: moves an actor 8 pixels at a time by the direction pad, with up and
   down changing its height (y) rather than its depth. */
Sint32 fdActorMoveByKeys(ACTOR *actor)
{
	FIELD_POS position;
	/* LEFTOVER(frame): an unused local the ROM's frame reserves and never
	 * touches. */
	UNUSED Uint8	   unused[68];
	MAP_GRID_METATILE *sourceMetatile;
	MAP_GRID_METATILE *destinationMetatile;
	Uint16			   angle;
	Sint32			   animation;
	Sint32			   collisionFlags;
	Sint32			   deltaZ;
	FIXED			   groundHeight;

	actor->speedLimit = toFIXED(1) / 2;
	actor->acceleration = toFIXED(1) / 4;
	animation = ANIM_LEADER_CLING;
	angle = WorldMapKeyHeadings[DPAD_INDEX(PerData)];
	if(angle == WORLD_MAP_KEY_HEADING_NONE) {
		collisionFlags = PLAYER_STEP_STOPPED;
	} else {
		/* The animation by the pad's heading: climbing east for headings below
		   a sixteenth of a turn, west for the sixteenth from a half turn on, up
		   or down for the rest. */
		animation = ANIM_LEADER_CLIMB_EAST;
		if(FACING_SIXTEENTH(angle) != 0) {
			animation = ANIM_LEADER_CLIMB_WEST;
			if(FACING_SIXTEENTH(angle) != FACING_WEST)
				animation = ANIM_LEADER_CLIMB;
		}
		collisionFlags = 0;
		position.x = 0;
		position.y = 0;
		position.z = 0;
		syVectorAddPolarOffset(toFIXED(8), angle, (FIXED *)&position);
		/* The step's depth becomes height: pad up climbs, pad down descends,
		   and the actor's angle becomes FACING_NORTH going up the map and
		   FACING_SOUTH going down. */
		position.x += actor->x.fixed;
		deltaZ = position.z;
		if(deltaZ < 0)
			actor->facing = FACING_NORTH;
		if(deltaZ > 0)
			actor->facing = FACING_SOUTH;
		position.y = actor->y.fixed - position.z;
		position.z = actor->z.fixed;
		/* The metatiles (16 pixels) under the actor and the step; a step into a
		   metatile with another metatile behavior is refused. */
		sourceMetatile =
			&((MAP_GRID_METATILE *)RamMapGridMetatileBuffer)[actor->x.fixed /
					toFIXED(MAPGRID_METATILE_PIXELS) +
				(position.z / toFIXED(MAPGRID_METATILE_PIXELS)) * MAPGRID_SIZE];
		destinationMetatile =
			&((MAP_GRID_METATILE *)RamMapGridMetatileBuffer)[position.x /
					toFIXED(MAPGRID_METATILE_PIXELS) +
				(position.z / toFIXED(MAPGRID_METATILE_PIXELS)) * MAPGRID_SIZE];
		if(fdMapGridIsWallAt((WORLD_POS *)&position) != 0 ||
			sourceMetatile->behavior != destinationMetatile->behavior) {
			collisionFlags = PLAYER_STEP_STOPPED;
			animation = ANIM_LEADER_CLING;
		} else if(PerData & KEY_UP) {
			/* Climbing needs the ground a metatile further up to stand at least
			   16 pixels above the actor; descending needs the ground at the
			   step at least 8 pixels below it. */
			groundHeight = fdMapGridGetTerrainHeight(
				actor->layer, position.x, position.z - toFIXED(16));
			if(groundHeight - actor->y.fixed < toFIXED(16))
				goto refused;
		} else if(PerData & KEY_DOWN) {
			groundHeight =
				fdMapGridGetTerrainHeight(actor->layer, position.x, position.z);
			if(groundHeight - actor->y.fixed > -toFIXED(8)) {
			refused:
				collisionFlags = PLAYER_STEP_BLOCKED_BY_MAP;
				animation = ANIM_LEADER_CLING;
			}
		}
	}
	if(FieldStateWork != NULL) {
		if(collisionFlags & PLAYER_STEP_BLOCKED)
			FieldStateWork->blockedSteps++;
		else
			FieldStateWork->blockedSteps = 0;
	}
	fdActorSelectAnimation(actor, animation);
	if(collisionFlags != 0) {
		actor->tgtX = ACTOR_NO_TARGET;
		actor->tgtY = ACTOR_NO_TARGET;
		actor->tgtZ = ACTOR_NO_TARGET;
		SET_VELOCITY(actor, 0, 0, 0);
	} else {
		fdActorSetMoveTarget(actor, position.x, position.y, position.z);
	}
	actor->scriptCursor++;
	return 1;
}

#undef FIELD_SHORTCUT_KEYS
#undef WORLD_MAP_PROBE_RADIUS
#undef FIELD_PROBE_ANGLE
#undef FIELD_BLOCKED_TURN_MAX
#undef WORLD_MAP_YAW_STEP_MAX
#undef FOOTPRINT_RUN_INTERVAL
#undef FOOTPRINT_WALK_INTERVAL
#undef FIELD_WALK_SPEED
#undef FIELD_RUN_SPEED
#undef FIELD_ACCELERATION
#undef FIELD_FAST_WALK_SPEED
#undef FIELD_FAST_WALK_ACCELERATION
#undef WORLD_MAP_WALK_SPEED
#undef WORLD_MAP_RUN_SPEED
#undef WORLD_MAP_RUN_ACCELERATION
#undef WORLD_MAP_ACCELERATION
#undef WORLD_MAP_FAST_SPEED
#undef FIELD_SHORTCUT_ID_MASK
#undef PROBE_FROM
