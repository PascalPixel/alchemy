/* @file */
/* The actor a field Psynergy or item aims at, and the helpers that follow it.
 */
/* Part of field_sequence.c. */

/* Picks the actor nearest actor id, under 32 pixels away and 48 pixels in */
/* height. Distances to treasure spots count double and to actors with */
/* ACTOR_NEAR_TALK ten thirteenths; beyond 11 pixels the actor must also lie */
/* within three sixteenths of a turn of id's facing. Returns -1 when none */
/* qualifies. */
s32 fdFindNearbyTalkTarget(s32 id) {
	ACTOR *self;
	ACTOR *candidate;
	s32 best;
	s32 bestDistance;
	s32 i;
	s32 dx;
	s32 dy;
	s32 dz;
	s32 xx;
	s32 yy;
	s32 zz;
	s32 distanceSquared;
	s32 distance;
	s32 angle;

	best = -1;
	bestDistance = 32;
	self = fdActorLookup(id);
	if(self != 0) {
		for(i = 0; i <= FIELD_ACTOR_COUNT; i++) {
			if(i == id)
				continue;

			candidate = fdActorLookup(i);
			if(candidate == 0 ||
			   (candidate->collisionFlg & ACTOR_NOT_TARGETED) != 0)
				continue;

			if(candidate->y.fixed - self->y.fixed >= 0) {
				if(candidate->y.fixed - self->y.fixed >= toFIXED(48))
					continue;
			}
			else if(self->y.fixed - candidate->y.fixed >= toFIXED(48)) {
				continue;
			}

			dx = (candidate->x.fixed - self->x.fixed) / FIXED_ONE;
			dy = (candidate->y.fixed - self->y.fixed) / FIXED_ONE;
			dz = (candidate->z.fixed - self->z.fixed) / FIXED_ONE;

			xx = dx * dx;
			yy = dy * dy;
			zz = dz * dz;
			distanceSquared = xx + yy + zz;
			distance = sySquart(distanceSquared);
			if(i > FIELD_TREASURE_ACTOR - 1)
				distance <<= 1;
			if((candidate->collisionFlg & ACTOR_NEAR_TALK) != 0)
				distance = distance * 10 / 13;
			if(distance >= bestDistance)
				continue;

			angle = (u16)syAtan(candidate->z.fixed - self->z.fixed,
								candidate->x.fixed - self->x.fixed);
			if(distance > 11) {
				s32 diff = (s16)(angle - self->facing);

				if(diff < -(TRIG_TURN * 3 / 16 - 1))
					continue;
				if(diff > TRIG_TURN * 3 / 16 - 1)
					continue;
			}

			best = i;
			bestDistance = distance;
		}
	}
	return best;
}

/* Returns the row of FieldPsynergy_Reach for a field Psynergy kind: its */
/* range, height and facing width. A kind not found returns the -1 terminator.
 */
const FIELD_PSYNERGY_REACH *fdGetPsynergyReach(s32 id) {
	const FIELD_PSYNERGY_REACH *entry = FieldPsynergy_Reach;

	while(entry->fieldPsynergy != -1 && id != entry->fieldPsynergy)
		entry++;
	return entry;
}

/* Returns how far the ground at position lies above or below the actor's, */
/* made positive, when that is within the kind's reach in height, else -1. */
s32 fdMeasurePsynergyHeight(s32 id, ACTOR *actor, s32 *position) {
	const FIELD_PSYNERGY_REACH *entry = fdGetPsynergyReach(id);
	s32 height = fdMapGridGetTerrainHeightAboveStub(
		position[0], position[2], actor->layer, actor->groundHeight);

	if(ABS(height) <= toFIXED(entry->height))
		return ABS(height);
	return -1;
}

/* A facing's sixteenths of a turn (FACING_STEP), as a shift. */
#define FACING_STEP_SHIFT 12

/* How far actor otherId stands from actor selfId, in pixels over the ground, */
/* when it lies within the reach of field Psynergy kind in front of selfId; -1
 */
/* when it does not. */
s32 fdMeasurePsynergyReach(s32 kind, s32 selfId, s32 otherId) {
	ACTOR *self;
	ACTOR *other;
	const FIELD_PSYNERGY_REACH *rule;
	s32 height;
	s32 range;
	s32 dx;
	s32 dz;
	s32 distance;
	s32 width;
	s32 facing;
	s32 diff;
	s32 angle;

	self = fdActorLookup(selfId);
	other = fdActorLookup(otherId);
	if(self == 0 || other == 0 ||
	   (other->collisionFlg & ACTOR_NOT_TARGETED) != 0)
		return -1;

	/* Height: within the rule's pixels above or below; Pound compares the */
	/* ground the two stand on, not their own heights. */
	rule = fdGetPsynergyReach(kind);
	height = toFIXED(rule->height);
	if(kind == FIELD_PSYNERGY_POUND) {
		if(ABS(other->groundHeight - self->groundHeight) > height)
			return -1;
	}
	else {
		if(ABS(other->y.fixed - self->y.fixed) > height)
			return -1;
	}

	/* Distance over the ground; actors with ACTOR_NEAR_PSYNERGY count from half
	 */
	/* as far again. */
	range = rule->range + 1;
	if((other->collisionFlg & ACTOR_NEAR_PSYNERGY) != 0)
		range = range * 3 / 2;
	dx = (other->x.fixed - self->x.fixed) / FIXED_ONE;
	dz = (other->z.fixed - self->z.fixed) / FIXED_ONE;
	distance = sySquart(dx * dx + dz * dz);
	if(distance > range)
		return -1;

	/* Beyond 11 pixels it must also lie in front: within the rule's width of */
	/* the caster's facing, rounded to a quarter turn (two thirds as wide */
	/* beyond 19 pixels). */
	if(distance > 11) {
		width = rule->width;
		if(distance > 19)
			width = width * 2 / 3;
		facing = (self->facing + DEGtoANG(45)) & TRIG_TURN * 3 / 4;
		angle = (u16)syAtan(other->z.fixed - self->z.fixed,
							other->x.fixed - self->x.fixed);
		diff = (s16)(angle - facing);
		if(diff < 0)
			diff = -diff;
		if(diff >= width)
			return -1;
	}
	return distance;
}

/* Returns the actor nearest in front of actor self that field Psynergy kind */
/* can reach, or -1. */
s32 fdFindNearbyPsynergyTarget(s32 self, s32 kind) {
	s32 nearest = BEYOND_REACH;
	s32 found = -1;
	s32 i;

	for(i = 0; i <= FIELD_ACTOR_COUNT; i++) {
		if(i != self) {
			s32 distance = fdMeasurePsynergyReach(kind, self, i);

			if(distance != -1 && nearest > distance) {
				found = i;
				nearest = distance;
			}
		}
	}
	return found;
}

/* Unused: only the stub table refers to it. Opens a chest: if actor target */
/* exists, plays SE_CHEST_OPEN, opens its lid and, 12 frames later, */
/* shows itemId in front of actor actorId with sparkles. */
s32 fdChestOpenAndShowItem(s32 actorId, s32 target, s32 itemId) {
	void *targetActor;
	s32 emitter;

	targetActor = fdActorGetById(target);
	emitter = 0;
	if(targetActor != 0) {
		sySndPlayStub(SE_CHEST_OPEN);
		fdActorSelectAnimationStub(targetActor, TREASURE_SPOT_CHEST_OPENING);
		sySynch(12);
		emitter = (s32)fdItemDispShowItemInFront(actorId, itemId);
	}
	return emitter;
}

/* Whether the actor 16 pixels in front of the leader, along its facing rounded
 */
/* to a quarter turn, can be pushed on by 16 pixels: 1 when nothing solid */
/* stands on the cell beyond it or above it and its terrain test passes there,
 */
/* else 0. fdLeaderPushObjectAhead, after it, makes the push. */
/* Since patch 1 the actor Halt has stopped (MainRoutineStateWork.haltedActor)
 */
/* cannot be pushed; the Japanese release has no such test. */
s32 fdLeaderCanPushObjectAhead(void) {
	s32 position[3];
	FIXED above[3];
	ACTOR *owner = fdActorGetById(MainRoutineStateWork.leaderActorId);
	ACTOR *target;
	ACTOR *hit;
	s32 id;
	s32 terrain;
	s32 result;
	s32 angle = owner->facing / FACING_STEP;

	/* The facing rounded to the nearest quarter turn, in sixteenths; the probe
	 */
	/* starts at the leader's height rounded to the nearest 8 pixels. */
	angle = ((angle + 2) & 0xc) << FACING_STEP_SHIFT;
	position[0] = owner->x.fixed;
	position[1] = (owner->y.fixed + toFIXED(4)) & ~(toFIXED(8) - 1);
	position[2] = owner->z.fixed;
	syVectorAddPolarOffset(toFIXED(16), angle, position);
	target = fdActorFindAtCell(position, TRUE);
	if(target == 0)
		return 0;
	if(target->sceneCnt != 0)
		return 0;
#if TLA_PATCH_1
	/* With nothing halted the id is -1, for which fdActorGetById returns NULL,
	 */
	/* so the test against 1 (where -1 looks meant) changes the result only */
	/* when actor 1 is the halted one, which it then lets be pushed. */
	id = MainRoutineStateWork.haltedActor;
	if(id != 1 && target == fdActorGetById(id))
		return 0;
#endif
	position[0] = target->x.fixed;
	position[1] = target->y.fixed;
	position[2] = target->z.fixed;
	syVectorAddPolarOffset(toFIXED(16), angle, position);
	hit = fdActorFindAtCell(position, TRUE);
	if(hit != 0 && (hit->collisionFlg & ACTOR_SOLID))
		return 0;
	above[0] = target->x.fixed;
	above[1] = target->y.fixed + toFIXED(16);
	above[2] = target->z.fixed;
	hit = fdActorFindAtCell(above, TRUE);
	if(hit != 0 && (hit->collisionFlg & ACTOR_SOLID))
		return 0;
	/* The cell beyond is tested on the map's overhead layer. */
	terrain = target->layer;
	target->layer = MAPGRID_LAYER_OVERHEAD;
	result = fdMapGridCheckCollisionStub(target, position);
	target->layer = terrain;
	if(result > 0)
		return 0;
	return 1;
}

/* Pushes the actor ahead of the leader on by 16 pixels, as */
/* fdLeaderCanPushObjectAhead allowed. */
void fdLeaderPushObjectAhead(void) {
	s32 position[3];
	ACTOR *owner = fdActorGetById(MainRoutineStateWork.leaderActorId);
	ACTOR *target;
	s32 terrain;
	s32 result;
	s32 speed;
	s32 acceleration;
	s32 angle = owner->facing / FACING_STEP;

	angle = ((angle + 2) & 0xc) << FACING_STEP_SHIFT;
	position[0] = owner->x.fixed;
	position[1] = (owner->y.fixed + toFIXED(4)) & ~(toFIXED(8) - 1);
	position[2] = owner->z.fixed;
	syVectorAddPolarOffset(toFIXED(16), angle, position);
	target = fdActorFindAtCell(position, TRUE);
	position[0] = target->x.fixed;
	position[1] = target->y.fixed;
	position[2] = target->z.fixed;
	syVectorAddPolarOffset(toFIXED(16), angle, position);
	/* The cell beyond is tested on the map's overhead layer. */
	terrain = target->layer;
	target->layer = MAPGRID_LAYER_OVERHEAD;
	result = fdMapGridCheckCollisionStub(target, position);
	target->layer = terrain;
	if(result <= 0) {
		fdActorSelectAnimationStub(owner, ANIM_PUSH);
		sySynch(15);
		sySndPlayStub(SE_PUSH);
		target->speedLimit = toFIXED(1) / 5;
		target->acceleration = toFIXED(1) / 5;
		fdActorSetMoveTargetStub(target, position[0], position[1], position[2]);
		sySynch(2);
		acceleration = owner->acceleration;
		owner->acceleration = toFIXED(1) / 5;
		speed = owner->speedLimit;
		owner->speedLimit = toFIXED(1) / 5;
		fdActorSetMoveTargetStub(owner, position[0], position[1], position[2]);
		fdActorWaitForMoveStub(target);
		sySynch(1);
		fdEventNoOp();
		fdActorSetPosAndResetMotionStub(target, position[0], position[1],
										position[2]);
		fdActorSetPosAndResetMotionStub(
			owner, toFIXED(owner->x.part[0].integer), owner->y.fixed,
			toFIXED(owner->z.part[0].integer));
		fdActorSelectAnimationStub(owner, ANIM_STAND);
		owner->speedLimit = speed;
		owner->acceleration = acceleration;
	}
}

#undef FACING_STEP_SHIFT
