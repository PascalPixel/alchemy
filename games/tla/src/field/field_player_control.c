/* Player control: the configured keys, and SC_PLAYER, the leader moved by the
   pad (fdActorUpdatePlayerControl). Part of field.c. */

#include "edition.h"
#include "scene/ground_effects.h"

/* What a shortcut key's assignment requests, bits 15..14 (fdRequestShortcut),
   as The Broken Seal's. */
typedef enum FIELD_SHORTCUT_KIND {
	FIELD_SHORTCUT_PSYNERGY = 0, /* FIELD_STATE.psynergy, an ability */
	FIELD_SHORTCUT_ITEM = 1		 /* FIELD_STATE.item */
} FIELD_SHORTCUT_KIND;

/* A shortcut assignment's request, bits 13..0, and its kind above them. */
#define FIELD_SHORTCUT_ID_MASK		   0x3fff
#define FIELD_SHORTCUT_KIND_SHIFT	   14
/* MAP_STATE.groundEffects' footprints, its bits 1..0: whether the leader leaves
   them. */
#define GROUND_EFFECTS_FOOTPRINTS_MASK 0x03

/* How far ahead of the leader the world map tests the ground, 16.16 pixels. */
#define WORLD_MAP_PROBE_RADIUS toFIXED(8)

/* Sets position radius out from actor along heading and tests it with check
   (fdMapGridCheckCollision, fdWorldMapIsSailable and the like, 0 where the way
   is open); true when it is open. */
#define PROBE_CLEAR(check, actor, radius, heading, position) \
	(SET_POSITION( \
		 position, (actor)->x.fixed, (actor)->y.fixed, (actor)->z.fixed), \
		syVectorAddPolarOffset(radius, heading, position), \
		check(actor, position) == 0)
/* The four probes beside heading, FIELD_PROBE_ANGLE and 45 degrees to each
   side, made in probe: true when all four are open, so the actor moving along
   heading grazes nothing it passes. */
#define SIDE_PROBES_CLEAR(check, actor, radius, heading, probe) \
	(PROBE_CLEAR( \
		 check, actor, radius, (heading) + FIELD_PROBE_ANGLE, probe) && \
		PROBE_CLEAR( \
			check, actor, radius, (heading) - FIELD_PROBE_ANGLE, probe) && \
		PROBE_CLEAR(check, actor, radius, (heading) + DEGtoANG(45), probe) && \
		PROBE_CLEAR(check, actor, radius, (heading) - DEGtoANG(45), probe))

/* The leader's speeds and accelerations, in pixels a tick as 16.16 fixed
   point, as The Broken Seal's; on the world map this game runs with twice the
   walk's acceleration. */
#define FIELD_WALK_SPEED toFIXED(1)		  /* 1 pixel a tick */
#define FIELD_RUN_SPEED	 (toFIXED(3) / 2) /* 1.5, with a dash key held */
#define FIELD_ACCELERATION \
	(toFIXED(1) / 4) /* a quarter pixel faster each tick */
#define FIELD_FAST_WALK_SPEED		 toFIXED(4) /* 4, B held with FLAG_FAST_WALK */
#define FIELD_FAST_WALK_ACCELERATION toFIXED(1)
#define WORLD_MAP_WALK_SPEED		 (toFIXED(1) / 2) /* half a pixel a tick */
#define WORLD_MAP_RUN_SPEED			 toFIXED(1) /* 1, with a dash key held */
#define WORLD_MAP_RUN_ACCELERATION	 (toFIXED(1) / 2)
#define WORLD_MAP_ACCELERATION		 (toFIXED(1) / 4)
/* The world map camera turns toward the pad's yaw by at most about 2.8 degrees
   a tick. */
#define WORLD_MAP_YAW_STEP_MAX		 (TRIG_TURN / 128)

/* SC_WALK (LEADER_MOVEMENT_WALKING): the leader's control at one pace, a pixel
   a tick whatever the keys. Unused: nothing sets LEADER_MOVEMENT_WALKING. It
   is SC_PLAYER (fdActorUpdatePlayerControl) without the run, the wading, the
   footprints or the steered velocity, a script command of its own, so both
   stay. */
Sint32 fdActorUpdateWalkingControl(ACTOR *actor)
{
	POS			 pos;
	POS			 testPos;
	POS			 contact;
	Sint16		 angleOffsets[22];
	Sint32		 collision;
	Sint32		 pushed;
	Sint32		 movementAnimation;
	Uint16		 angle;
	Sint16		 heading;
	Sint32		 angleIndex;
	Sint32		 headingCount;
	Bool		 onPlatform;
	ACTOR		*other;
	FIELD_STATE *state;

	movementAnimation = ANIM_WALK;
	collision = 0;
	pushed = 0;
	headingCount = 0;
	if(PerWork.data & MainRoutineStateWork.dashKeys) {
		actor->speedLimit = toFIXED(1);
		actor->acceleration = toFIXED(1) / 20;
		movementAnimation = ANIM_LEADER_RUN;
	} else {
		actor->speedLimit = toFIXED(1);
		actor->acceleration = toFIXED(1) / 20;
	}

	angle = WorldMapKeyHeadings[DPAD_INDEX(PerWork.data)];
	if(angle == WORLD_MAP_KEY_HEADING_NONE) {
		collision |= PLAYER_STEP_STOPPED;
		goto counted;
	}
	/* DEAD: collision is already 0 here; the ROM keeps its clearing. */
	collision = 0;
	SET_POSITION(&pos, actor->x.fixed, actor->y.fixed, actor->z.fixed);
	syVectorAddPolarOffset(toFIXED(8), angle, &pos);
	if(actor->priorityFlags & ACTOR_PRIORITY_WALKS_ON_PLATFORMS) {
		onPlatform = FALSE;
		other = (ACTOR *)RamHeapSlots->actors;
		for(angleIndex = 0; angleIndex < ACTOR_COUNT; angleIndex++, other++) {
			FIXED dy;

			if(other->script == NULL)
				continue;
			if(!(other->priorityFlags & ACTOR_PRIORITY_PLATFORM))
				continue;
			if(other == actor)
				continue;
			dy = actor->y.fixed - other->y.fixed;
			if(ABS(dy - toFIXED(16)) > toFIXED(8) - 1)
				continue;
			if(SAME_METATILE(actor->x.fixed, other->x.fixed) &&
				SAME_METATILE(actor->z.fixed, other->z.fixed))
				onPlatform = TRUE;
			if(SAME_METATILE(pos.x, other->x.fixed) &&
				SAME_METATILE(pos.z, other->z.fixed))
				goto clear;
		}
		if(onPlatform && !fdMapGridCheckCollision(actor, &pos)) {
			/* DEAD: collision is already 0 here; the ROM keeps its clearing. */
			collision = 0;
			goto counted;
		}
	}
	if(DebugMode != 0 && (PerWork.data & KEY_L)) {
		/* DEAD: collision is already 0 here; the ROM keeps its clearing, which
		   cross-jumping shares with clear's. */
		collision = 0;
		goto counted;
	}
	if(!fdMapGridCheckCollision(actor, &pos) &&
		SIDE_PROBES_CLEAR(
			fdMapGridCheckCollision, actor, toFIXED(8), angle, &testPos)) {
		heading = angle;
		goto push_objects;
	}
	angleOffsets[headingCount++] = angle + FIELD_PROBE_ANGLE;
	angleOffsets[headingCount++] = angle - FIELD_PROBE_ANGLE;
	angleOffsets[headingCount++] = angle + DEGtoANG(45);
	angleOffsets[headingCount++] = angle - DEGtoANG(45);
	angleOffsets[headingCount++] = angle + FIELD_PROBE_ANGLE * 3;
	angleOffsets[headingCount++] = angle - FIELD_PROBE_ANGLE * 3;
	for(angleIndex = 0; angleIndex < headingCount; angleIndex++) {
		heading = angleOffsets[angleIndex];
		if(PROBE_CLEAR(fdMapGridCheckCollision, actor, toFIXED(8),
			   (Uint16)heading, &pos) &&
			SIDE_PROBES_CLEAR(fdMapGridCheckCollision, actor, toFIXED(8),
				(Uint16)heading, &testPos))
			goto push_objects;
	}

	goto blocked;

clear:
	/* DEAD: collision is already 0 here; the ROM keeps its clearing. */
	collision = 0;
	goto counted;

blocked:
	SET_POSITION(&pos, actor->x.fixed, actor->y.fixed, actor->z.fixed);
	collision |= PLAYER_STEP_BLOCKED_BY_MAP;

push_objects:
	SET_POSITION(&contact, actor->x.fixed, actor->y.fixed, actor->z.fixed);
	syVectorAddPolarOffset(toFIXED(4), (Uint16)heading, &contact);
	other = (ACTOR *)RamHeapSlots->actors;
	for(angleIndex = ACTOR_COUNT - 1; angleIndex >= 0; angleIndex--, other++) {
		Sint32 radius;

		radius = actor->radius - 2;
		if(other->script == NULL)
			continue;
		if(!(other->collisionFlags & ACTOR_SOLID))
			continue;
		if(other == actor)
			continue;
		if(fdCheckSpheresOverlap(
			   &other->x.fixed, radius, &contact.x, other->radius - 2) < 0)
			continue;
		if((other->collisionFlags & ACTOR_PUSHABLE) && other->paused == 0) {
			Uint16 direction;

			direction = syAtan(other->z.fixed - actor->z.fixed,
				other->x.fixed - actor->x.fixed);
			SET_POSITION(
				&testPos, other->x.fixed, other->y.fixed, other->z.fixed);
			syVectorAddPolarOffset(toFIXED(1) / 4, direction, &testPos);
			if(!fdActorScriptCheckOverlap(other, &testPos.x)) {
				SET_POSITION(
					&testPos, other->x.fixed, other->y.fixed, other->z.fixed);
				syVectorAddPolarOffset(toFIXED(10), direction, &testPos);
				if(!fdMapGridCheckCollision(other, &testPos)) {
					SET_POSITION(&testPos, other->x.fixed, other->y.fixed,
						other->z.fixed);
					syVectorAddPolarOffset(
						toFIXED(10), direction + FIELD_PROBE_ANGLE, &testPos);
					/* The same probe is tested twice. */
					if(!fdMapGridCheckCollision(other, &testPos) &&
						!fdMapGridCheckCollision(other, &testPos)) {
						SET_POSITION(&testPos, other->x.fixed, other->y.fixed,
							other->z.fixed);
						syVectorAddPolarOffset(toFIXED(10),
							direction - FIELD_PROBE_ANGLE, &testPos);
						if(!fdMapGridCheckCollision(other, &testPos)) {
							syVectorAddPolarOffset(toFIXED(1) / 4, direction,
								(POS *)&other->x.fixed);
							other->targetX = ACTOR_NO_TARGET;
							other->targetY = ACTOR_NO_TARGET;
							other->targetZ = ACTOR_NO_TARGET;
							pushed |= 1;
							continue;
						}
					}
				}
			}
		}
		collision |= PLAYER_STEP_BLOCKED_BY_ACTOR;
	}
	if(collision == 0 && pushed != 0) {
		actor->speedLimit = toFIXED(1) / 4;
		actor->acceleration = toFIXED(1) / 8;
	}

counted:
	state = RamHeapSlots->fieldState;
	if(state != NULL) {
		if(collision & PLAYER_STEP_BLOCKED)
			state->blockedSteps++;
		else
			state->blockedSteps = 0;
	}
	if(pushed != 0) {
		fdActorSelectAnimation(actor, ANIM_PUSH);
	} else if(collision != 0) {
		Sint32 action;

		action = ANIM_LEADER_LOOK_AROUND;
		if(syUnitGet(MainRoutineStateWork.leaderActorId)->hp == 0)
			action = ANIM_LEADER_LEAN_BACK;
		fdActorSelectAnimation(actor, action);
	} else {
		fdActorSelectAnimation(actor, movementAnimation);
	}

	if(collision != 0) {
		actor->targetX = ACTOR_NO_TARGET;
		actor->targetY = ACTOR_NO_TARGET;
		actor->targetZ = ACTOR_NO_TARGET;
		if(collision & PLAYER_STEP_BLOCKED) {
			Sint32 difference;

			difference = (Sint16)(angle - actor->facing);
			if(difference > FIELD_BLOCKED_TURN_MAX)
				difference = FIELD_BLOCKED_TURN_MAX;
			if(difference < -FIELD_BLOCKED_TURN_MAX)
				difference = -FIELD_BLOCKED_TURN_MAX;
			actor->facing += difference;
		}
		actor->timer = 0;
		actor->userValue = FOOTPRINT_BOTH_FEET;
	} else {
		fdActorSetMoveTarget(actor, pos.x, pos.y, pos.z);
		if(actor->timer != 0)
			actor->timer--;
	}
	fdCheckConfiguredKeys();
	actor->scriptCursor++;
	return 1;
}

/* Passes on a shortcut key's assignment, as The Broken Seal's: bits 15..14 say
   whether the request is a field Psynergy or an item, bits 13..0 which. On the
   world map R and L raise the map screen and the zoom instead, and while
   FLAG_MENUS_RAISE_CUE is set the scene's raised cue. Returns the request. */
Sint32 fdRequestShortcut(Uint32 assignment)
{
	FIELD_STATE *state = RamHeapSlots->fieldState;
	Uint32		 kind = assignment >> FIELD_SHORTCUT_KIND_SHIFT;
	Sint32		 request = assignment & FIELD_SHORTCUT_ID_MASK;

	if(syGameFlagGet(FLAG_MENUS_RAISE_CUE) != 0) {
		state->raisedCue = FIELD_RAISED_CUE_MENUS;
	} else if(state->mode == ACTOR_SYSTEM_WORLD_MAP) {
		if(PerWork.data & KEY_R)
			state->raisedCue = FIELD_RAISED_CUE_MAP_SCREEN;
		else if(PerWork.data & KEY_L)
			state->raisedCue = FIELD_RAISED_CUE_WORLD_MAP_ZOOM;
	} else {
		switch(kind) {
		case FIELD_SHORTCUT_PSYNERGY:
			state->psynergy = request;
			break;
		case FIELD_SHORTCUT_ITEM:
			state->item = request;
			break;
		}
	}
	return request;
}

/* Checks the keys the player has configured: the menu, examine and start keys
   set their request and return 1, the two shortcut keys pass on their
   assignment (fdRequestShortcut). While the Ring is out, any of the five asks
   for the Ring again (ABILITY_MAGNET, packed), which ends it. Returns 0 when
   none is pressed. */
Sint32 fdCheckConfiguredKeys(void)
{
	FIELD_STATE *state = FieldStateWork;
	Sint32		 result = 0;

	if(state == NULL)
		return 0;
	if(state->ringActive != 0) {
		if((PerWork.push & MainRoutineStateWork.selectKeys) ||
			(PerWork.push & MainRoutineStateWork.aKeys) ||
			(PerWork.push & MainRoutineStateWork.startKeys) ||
			(PerWork.push & MainRoutineStateWork.firstShortcutKeys) ||
			(PerWork.push & MainRoutineStateWork.secondShortcutKeys)) {
			state->psynergy = ABILITY_MAGNET | ACT_PACKED;
			return 1;
		}
		return 0;
	} else {
		if(PerWork.push & MainRoutineStateWork.selectKeys) {
			state->fieldMenu = 1;
			result = 1;
		} else if(PerWork.push & MainRoutineStateWork.aKeys) {
			state->aButton = 1;
			result = 1;
		} else if(PerWork.push & MainRoutineStateWork.startKeys) {
			state->pauseMenu = 1;
			result = 1;
		} else if(PerWork.push & MainRoutineStateWork.firstShortcutKeys) {
			result = fdRequestShortcut(MainRoutineStateWork.firstShortcut);
		} else if(PerWork.push & MainRoutineStateWork.secondShortcutKeys) {
			result = fdRequestShortcut(MainRoutineStateWork.secondShortcut);
		}
	}
	return result;
}

/* Returns 0 when two spheres overlap: at firstPos and secondPos
   (16.16 pixels) with radii radiusA and radiusB (pixels); -1 otherwise. */
Sint32 fdCheckSpheresOverlap(
	FIXED *firstPos, Sint32 radiusA, FIXED *secondPos, Sint32 radiusB)
{
	const POS	 *firstPoint = (const POS *)firstPos;
	const POS	 *secondPoint = (const POS *)secondPos;
	const Sint32 *firstCoordinate = &firstPoint->x;
	const Sint32 *secondCoordinate = &secondPoint->x;
	Sint32		  dx = FIXEDtoINT(*firstCoordinate++ - *secondCoordinate++);
	Sint32		  dy = FIXEDtoINT(*firstCoordinate++ - *secondCoordinate++);
	Sint32		  dz = FIXEDtoINT(*firstCoordinate - *secondCoordinate);
	Sint32		  radius = radiusA + radiusB;

	/* The bound is 64 pixels in 16.16, the units before the shift, but dx and
	   dz are already whole pixels (at most 32768), so these tests always pass
	   and only the distance decides. */
	if(!(dx > toFIXED(64)) && !(dz > toFIXED(64)) &&
		dx * dx + dy * dy + dz * dz < radius * radius)
		return 0;
	return -1;
}

/* A splash at the feet of an actor wading through water (MAP_METATILE_WATER):
   SPRITE_WADING_SPLASH started on its splash script, its sound every other
   step; mode is the map's kind of wading (GROUND_EFFECTS_WADING). In Taopo
   Swamp's mud the splash is drawn in the mud's colour, at half speed. */
void fdActorSpawnWaterSplash(ACTOR *actor, Sint32 mode)
{
	ACTOR		   *created;
	SPR_LAYER_VIEW *sprite;

	created = fdActorCreate(SPRITE_WADING_SPLASH, actor->x.fixed,
		actor->y.fixed + toFIXED(4), actor->z.fixed);
	sprite = actor->sprite;
	if(!(actor->userValue & 1))
		sySoundPlayStub(SE_WADE);
	if(created != NULL) {
		SPR_LAYER_VIEW *child = created->sprite;

		created->groundHeight = actor->groundHeight;
		fdActorStartScript(created, ActorSplashScript);
		created->motionFlags = 0;
		if(child != NULL) {
			fdSpriteStartAnimation((SPR *)child, 1);
			child->flags = 0;
			child->layer = sprite->layer;
			if(mode == MAP_WADING_MUD) {
				fdSpriteSetAnimationColorModes((SPR *)child, SPRITE_COLOR_FILL);
				fdSpriteSetAnimationSpeed((SPR *)child, ANIM_STEP_NORMAL / 2);
			}
		}
	}
}

extern MAP_METATILE MapGridMetatileBuffer[];

/* SC_PLAYER: moves the player on the field by the direction pad: walking or
   running, sliding around walls, pushing movable actors, turning, playing the
   matching animation and leaving footprints. */
Sint32 fdActorUpdatePlayerControl(ACTOR *actor)
{
	POS			 pos;
	POS			 testPos;
	POS			 contact;
	Sint16		 angleOffsets[22];
	Sint32		 collision;
	Sint32		 pushed;
	PLAYER_SPR	*sprite;
	Sint16		 heading;
	Sint32		 angleIndex;
	Sint32		 movementAnimation;
	Sint32		 headingCount;
	Sint32		 footprintAngle;
	Bool		 onPlatform;
	FIELD_STATE *state;
	MAP_STATE	*map;
	Uint16		 angle;
	Uint16		 direction;
	ACTOR		*other;
	Sint32		 oldFlags;
	Uint32		 attribute;

	sprite = actor->sprite;
	movementAnimation = ANIM_WALK;
	onPlatform = FALSE;
	state = RamHeapSlots->fieldState;
	map = RamHeapSlots->mapGrid;
	collision = 0;
	pushed = 0;
	if(PerWork.data & MainRoutineStateWork.dashKeys) {
		actor->speedLimit = FIELD_RUN_SPEED;
		actor->acceleration = FIELD_ACCELERATION;
		movementAnimation = ANIM_LEADER_RUN;
	} else {
		actor->speedLimit = FIELD_WALK_SPEED;
		actor->acceleration = FIELD_ACCELERATION;
	}

	angle = WorldMapKeyHeadings[DPAD_INDEX(PerWork.data)];
	if((Uint16)angle == WORLD_MAP_KEY_HEADING_NONE) {
		collision |= PLAYER_STEP_STOPPED;
		goto counted;
	}
	collision = 0;
	SET_POSITION(&pos, actor->x.fixed, actor->y.fixed, actor->z.fixed);
	syVectorAddPolarOffset(toFIXED(8), (Uint16)angle, &pos);
	if(actor->priorityFlags & ACTOR_PRIORITY_WALKS_ON_PLATFORMS) {
		/* Standing on another actor a metatile below: the step is clear when
		   the actor under the step (and, after Japanese, both its sides) is a
		   still one. */
#if PLATFORM_STEP_TESTS_BOTH_SIDES
		SET_POSITION(&testPos, pos.x, pos.y, pos.z);
		syVectorAddPolarOffset(
			toFIXED(2), (Uint16)angle + DEGtoANG(90), &testPos);
		SET_POSITION(&contact, pos.x, pos.y, pos.z);
		syVectorAddPolarOffset(
			toFIXED(2), (Uint16)angle - DEGtoANG(90), &contact);
#endif
		angleIndex = 0;
		other = (ACTOR *)RamHeapSlots->actors;
		for(; angleIndex < ACTOR_COUNT; angleIndex++, other++) {
			FIXED dy;

			if(other->script == NULL)
				continue;
			if(!(other->priorityFlags & ACTOR_PRIORITY_PLATFORM))
				continue;
			if(other == actor)
				continue;
			dy = actor->y.fixed - other->y.fixed;
			if((dy - toFIXED(16) < 0 ? -(dy - toFIXED(16)) : dy - toFIXED(16)) >
				toFIXED(8) - 1)
				continue;
			if(SAME_METATILE(actor->x.fixed, other->x.fixed) &&
				SAME_METATILE(actor->z.fixed, other->z.fixed))
				onPlatform = TRUE;
			if(SAME_METATILE(pos.x, other->x.fixed) &&
				SAME_METATILE(pos.z, other->z.fixed) &&
#if PLATFORM_STEP_TESTS_BOTH_SIDES
				SAME_METATILE(testPos.x, other->x.fixed) &&
				SAME_METATILE(testPos.z, other->z.fixed) &&
				SAME_METATILE(contact.x, other->x.fixed) &&
				SAME_METATILE(contact.z, other->z.fixed) &&
#endif
				other->targetX == ACTOR_NO_TARGET &&
				other->targetZ == other->targetX)
				goto clear;
		}
		if(onPlatform && !fdMapGridCheckCollision(actor, &pos)) {
			collision = 0;
			heading = angle;
			goto push_objects;
		}
	}
	if(DebugMode != 0 && (PerWork.data & KEY_L)) {
		collision = 0;
		heading = angle;
		goto counted;
	}
	if(!fdMapGridCheckCollision(actor, &pos) &&
		SIDE_PROBES_CLEAR(fdMapGridCheckCollision, actor, toFIXED(8),
			(Uint16)angle, &testPos)) {
		heading = angle;
		goto push_objects;
	}
	direction = angle;
	headingCount = 0;
	angleOffsets[headingCount++] = direction + FIELD_PROBE_ANGLE;
	angleOffsets[headingCount++] = direction - FIELD_PROBE_ANGLE;
	angleOffsets[headingCount++] = direction + DEGtoANG(45);
	angleOffsets[headingCount++] = direction - DEGtoANG(45);
	angleOffsets[headingCount++] = direction + FIELD_PROBE_ANGLE * 3;
	angleOffsets[headingCount++] = direction - FIELD_PROBE_ANGLE * 3;
	for(angleIndex = 0; angleIndex < headingCount; angleIndex++) {
		heading = angleOffsets[angleIndex];
		if(PROBE_CLEAR(fdMapGridCheckCollision, actor, toFIXED(8),
			   (Uint16)heading, &pos) &&
			SIDE_PROBES_CLEAR(fdMapGridCheckCollision, actor, toFIXED(8),
				(Uint16)heading, &testPos))
			goto push_objects;
	}
	SET_POSITION(&pos, actor->x.fixed, actor->y.fixed, actor->z.fixed);
	collision |= PLAYER_STEP_BLOCKED_BY_MAP;
	goto push_objects;

clear:
	collision = 0;
	heading = angle;

push_objects:
	/* Push the movable actors just ahead a quarter pixel further, when they
	   have room; any other solid actor there blocks the step. */
	SET_POSITION(&contact, actor->x.fixed, actor->y.fixed, actor->z.fixed);
	syVectorAddPolarOffset(toFIXED(4), (Uint16)heading, &contact);
	other = (ACTOR *)RamHeapSlots->actors;
	for(angleIndex = 0; angleIndex < ACTOR_COUNT; angleIndex++, other++) {
		Sint32 radius;

		radius = actor->radius - 2;
		if(other->script == NULL)
			continue;
		if(!(other->collisionFlags & ACTOR_SOLID))
			continue;
		if(other == actor)
			continue;
		if(fdCheckSpheresOverlap(
			   &other->x.fixed, radius, &contact.x, other->radius - 2) < 0)
			continue;
		if((other->collisionFlags & ACTOR_PUSHABLE) && other->paused == 0) {
			direction = syAtan(other->z.fixed - actor->z.fixed,
				other->x.fixed - actor->x.fixed);
			SET_POSITION(
				&testPos, other->x.fixed, other->y.fixed, other->z.fixed);
			heading = direction;
			syVectorAddPolarOffset(toFIXED(1) / 4, direction, &testPos);
			if(!fdActorScriptCheckOverlap(other, &testPos.x)) {
				SET_POSITION(
					&testPos, other->x.fixed, other->y.fixed, other->z.fixed);
				syVectorAddPolarOffset(toFIXED(10), direction, &testPos);
				if(!fdMapGridCheckCollision(other, &testPos)) {
					SET_POSITION(&testPos, other->x.fixed, other->y.fixed,
						other->z.fixed);
					syVectorAddPolarOffset(
						toFIXED(10), direction + FIELD_PROBE_ANGLE, &testPos);
					/* The same probe is tested twice. */
					if(!fdMapGridCheckCollision(other, &testPos) &&
						!fdMapGridCheckCollision(other, &testPos)) {
						SET_POSITION(&testPos, other->x.fixed, other->y.fixed,
							other->z.fixed);
						syVectorAddPolarOffset(toFIXED(10),
							direction - FIELD_PROBE_ANGLE, &testPos);
						if(!fdMapGridCheckCollision(other, &testPos)) {
							syVectorAddPolarOffset(toFIXED(1) / 4, direction,
								(POS *)&other->x.fixed);
							other->targetX = ACTOR_NO_TARGET;
							other->targetY = ACTOR_NO_TARGET;
							other->targetZ = ACTOR_NO_TARGET;
							pushed |= 1;
							continue;
						}
					}
				}
			}
		}
		collision |= PLAYER_STEP_BLOCKED_BY_ACTOR;
	}
	if(collision == 0 && pushed != 0) {
		actor->speedLimit = toFIXED(1) / 4;
		actor->acceleration = toFIXED(1) / 8;
	}

counted:
	if(state != NULL) {
		if(collision & PLAYER_STEP_BLOCKED)
			state->blockedSteps++;
		else
			state->blockedSteps = 0;
		state->heldKeys = PerWork.data;
	}
	if(pushed != 0) {
		fdActorSelectAnimation(actor, ANIM_PUSH);
	} else if(collision != 0) {
		Sint32 action;

		action = ANIM_LEADER_LOOK_AROUND;
		if(syUnitGet(MainRoutineStateWork.leaderActorId)->hp == 0)
			action = ANIM_LEADER_LEAN_BACK;
		fdActorSelectAnimation(actor, action);
	} else {
		fdActorSelectAnimation(actor, movementAnimation);
	}

	if(collision != 0) {
		actor->targetX = ACTOR_NO_TARGET;
		actor->targetY = ACTOR_NO_TARGET;
		actor->targetZ = ACTOR_NO_TARGET;
		actor->velocityX = 0;
		actor->velocityZ = 0;
		if(collision & PLAYER_STEP_BLOCKED) {
			Sint32 difference;

			difference = (Sint16)((Uint16)angle - actor->facing);
			if(difference > FIELD_BLOCKED_TURN_MAX)
				difference = FIELD_BLOCKED_TURN_MAX;
			if(difference < -FIELD_BLOCKED_TURN_MAX)
				difference = -FIELD_BLOCKED_TURN_MAX;
			actor->facing += difference;
		}
		actor->timer = 0;
		actor->userValue = FOOTPRINT_BOTH_FEET;
	} else {
		FIXED speed;

		fdActorSetMoveTarget(actor, pos.x, pos.y, pos.z);
		speed = sySquartFX(syMulFX(actor->velocityX, actor->velocityX) +
			syMulFX(actor->velocityZ, actor->velocityZ));
		actor->velocityX = collision;
		actor->velocityZ = collision;
		syVectorAddPolarOffset(
			speed, (Uint16)heading, (POS *)&actor->velocityX);
		if(actor->timer != 0)
			actor->timer--;
	}

	/* Ground that wades: hide the lower half of the sprite while on it, and
	   leave the wading effect as the leader moves. */
	if(map->groundEffects & GROUND_EFFECTS_WADING_MASK) {
		MAP_METATILE *metatiles;

		if(actor->layer <= 2)
			metatiles = ((MAP_STATE *)map)->layers[actor->layer].metatiles;
		else
			metatiles = MapGridMetatileBuffer;
		metatiles += actor->x.fixed / toFIXED(16) +
			(actor->z.fixed / toFIXED(16)) * MAPGRID_SIZE;
		attribute = metatiles->attributeB;
		oldFlags = sprite->flags;
		if(attribute & MAP_METATILE_WATER) {
			if(!(oldFlags & SPRITE_FLAG_SUNK)) {
				sprite->sunkRows = 4;
				sprite->flags = ((Uint8)oldFlags | SPRITE_FLAG_SHADED) &
					~SPRITE_FLAG_SHADOW;
			}
			if(actor->timer == 0 && collision == 0 && !onPlatform) {
				fdActorSpawnWaterSplash(
					actor, GROUND_EFFECTS_WADING(map->groundEffects));
				if(movementAnimation == ANIM_LEADER_RUN)
					actor->timer = 8;
				else
					actor->timer = 12;
				actor->userValue ^= 1;
			}
		} else if(!(oldFlags & SPRITE_FLAG_SUNK)) {
			sprite->flags =
				((Uint8)oldFlags | SPRITE_FLAG_SHADOW) & ~SPRITE_FLAG_SHADED;
		}
		if(oldFlags != sprite->flags)
			sprite->dirty = 1;
	}

	/* Footprints on maps that show them, alternating left and right. */
	footprintAngle = (Uint16)angle;
	if((map->groundEffects & GROUND_EFFECTS_FOOTPRINTS_MASK) &&
		actor->timer == 0 && collision == 0) {
		ACTOR *effect;

		effect = fdActorCreate(
			SPRITE_FOOTPRINTS, actor->x.fixed, actor->y.fixed, actor->z.fixed);
		if(effect != 0) {
			PLAYER_SPR *effectSprite;

			effect->groundHeight = actor->groundHeight;
			effectSprite = effect->sprite;
			fdActorStartScript(effect, ActorFootprintScript);
			effect->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
			effect->motionFlags = collision;
			if(effectSprite != NULL) {
				fdSpriteStartAnimation(effectSprite, FOOTPRINT_ONE_FOOT);
				effectSprite->flags = collision;
				effectSprite->rotation = footprintAngle + DEGtoANG(90);
				effectSprite->priority = 3;
			}
			if(actor->userValue == FOOTPRINT_BOTH_FEET) {
				fdSpriteStartAnimation(effectSprite, FOOTPRINT_BOTH_FEET);
				actor->userValue = collision;
			}
			if(actor->userValue != 0)
				effect->facing = DEGtoANG(180);
			if(movementAnimation == ANIM_LEADER_RUN)
				actor->timer = 12;
			else
				actor->timer = 18;
			actor->userValue ^= 1;
		}
	}
	fdCheckConfiguredKeys();
	actor->scriptCursor++;
	return 1;
}
