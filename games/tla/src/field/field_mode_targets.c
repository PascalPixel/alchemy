/* The actor a field Psynergy or item aims at, and the helpers that follow it.
   Part of field_mode.c. */

/* Picks the actor nearest actor id, under 32 pixels away and 48 pixels in
   height. Distances to treasure spots count double and to actors with
   ACTOR_NEAR_TALK ten thirteenths; beyond 11 pixels the actor must also lie
   within three sixteenths of a turn of id's facing. Returns -1 when none
   qualifies. */
Sint32 fdFindNearbyTalkTarget(Sint32 id)
{
	ACTOR *self;
	ACTOR *candidate;
	Sint32 best;
	Sint32 bestDistance;
	Sint32 i;
	Sint32 dx;
	Sint32 dy;
	Sint32 dz;
	Sint32 xx;
	Sint32 yy;
	Sint32 zz;
	Sint32 distanceSquared;
	Sint32 distance;
	Sint32 angle;

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
			} else if(self->y.fixed - candidate->y.fixed >= toFIXED(48)) {
				continue;
			}

			dx = (candidate->x.fixed - self->x.fixed) / toFIXED(1);
			dy = (candidate->y.fixed - self->y.fixed) / toFIXED(1);
			dz = (candidate->z.fixed - self->z.fixed) / toFIXED(1);

			xx = dx * dx;
			yy = dy * dy;
			zz = dz * dz;
			distanceSquared = xx + yy + zz;
			distance = sySquartIWRAM(distanceSquared);
			if(i > FIELD_TREASURE_ACTOR - 1)
				distance <<= 1;
			if((candidate->collisionFlg & ACTOR_NEAR_TALK) != 0)
				distance = distance * 10 / 13;
			if(distance >= bestDistance)
				continue;

			angle = (Uint16)syAtan(candidate->z.fixed - self->z.fixed,
				candidate->x.fixed - self->x.fixed);
			if(distance > 11) {
				Sint32 diff = (Sint16)(angle - self->facing);

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

/* Returns the row of FieldPsynergy_Reach for a field Psynergy kind: its range,
   height and facing width. A kind not found returns the -1 terminator. */
const FIELD_PSYNERGY_REACH *fdGetPsynergyReach(Sint32 id)
{
	const FIELD_PSYNERGY_REACH *entry = FieldPsynergy_Reach;

	while(entry->fieldPsynergy != -1 && id != entry->fieldPsynergy)
		entry++;
	return entry;
}

/* Returns how far the ground at position lies above or below the actor's, made
   positive, when that is within the kind's reach in height, else -1. */
Sint32 fdMeasurePsynergyHeight(Sint32 id, ACTOR *actor, Sint32 *position)
{
	const FIELD_PSYNERGY_REACH *entry = fdGetPsynergyReach(id);
	Sint32						height = fdMapGridGetTerrainHeightAboveStub(
		 position[0], position[2], actor->layer, actor->groundHeight);

	if(ABS(height) <= toFIXED(entry->height))
		return ABS(height);
	return -1;
}

/* A facing's sixteenths of a turn (FACING_STEP), as a shift. */
#define FACING_STEP_SHIFT 12

/* How far actor otherId stands from actor selfId, in pixels over the ground,
   when it lies within the reach of field Psynergy kind in front of selfId; -1
   when it does not. */
Sint32 fdMeasurePsynergyReach(Sint32 kind, Sint32 selfId, Sint32 otherId)
{
	ACTOR					   *self;
	ACTOR					   *other;
	const FIELD_PSYNERGY_REACH *rule;
	Sint32						height;
	Sint32						range;
	Sint32						dx;
	Sint32						dz;
	Sint32						distance;
	Sint32						width;
	Sint32						facing;
	Sint32						diff;
	Sint32						angle;

	self = fdActorLookup(selfId);
	other = fdActorLookup(otherId);
	if(self == 0 || other == 0 ||
		(other->collisionFlg & ACTOR_NOT_TARGETED) != 0)
		return -1;

	/* Height: within the rule's pixels above or below; Pound compares the
	   ground the two stand on, not their own heights. */
	rule = fdGetPsynergyReach(kind);
	height = toFIXED(rule->height);
	if(kind == FIELD_PSYNERGY_POUND) {
		if(ABS(other->groundHeight - self->groundHeight) > height)
			return -1;
	} else {
		if(ABS(other->y.fixed - self->y.fixed) > height)
			return -1;
	}

	/* Distance over the ground; actors with ACTOR_NEAR_PSYNERGY count from half
	   as far again. */
	range = rule->range + 1;
	if((other->collisionFlg & ACTOR_NEAR_PSYNERGY) != 0)
		range = range * 3 / 2;
	dx = (other->x.fixed - self->x.fixed) / toFIXED(1);
	dz = (other->z.fixed - self->z.fixed) / toFIXED(1);
	distance = sySquartIWRAM(dx * dx + dz * dz);
	if(distance > range)
		return -1;

	/* Beyond 11 pixels it must also lie in front: within the rule's width of
	   the caster's facing, rounded to a quarter turn (two thirds as wide beyond
	   19 pixels). */
	if(distance > 11) {
		width = rule->width;
		if(distance > 19)
			width = width * 2 / 3;
		facing = (self->facing + DEGtoANG(45)) & TRIG_TURN * 3 / 4;
		angle = (Uint16)syAtan(
			other->z.fixed - self->z.fixed, other->x.fixed - self->x.fixed);
		diff = (Sint16)(angle - facing);
		if(diff < 0)
			diff = -diff;
		if(diff >= width)
			return -1;
	}
	return distance;
}

/* Returns the actor nearest in front of actor self that field Psynergy kind can
   reach, or -1. */
Sint32 fdFindNearbyPsynergyTarget(Sint32 self, Sint32 kind)
{
	Sint32 nearest = BEYOND_REACH;
	Sint32 found = -1;
	Sint32 i;

	for(i = 0; i <= FIELD_ACTOR_COUNT; i++) {
		if(i != self) {
			Sint32 distance = fdMeasurePsynergyReach(kind, self, i);

			if(distance != -1 && nearest > distance) {
				found = i;
				nearest = distance;
			}
		}
	}
	return found;
}

/* Unused: only the stub table refers to it. Opens a chest: if actor target
   exists, plays SE_CHEST_OPEN, opens its lid and, 12 frames later, shows itemId
   in front of actor actorId with sparkles. */
Sint32 fdChestOpenAndShowItem(Sint32 actorId, Sint32 target, Sint32 itemId)
{
	void  *targetActor;
	Sint32 emitter;

	targetActor = fdActorGetById(target);
	emitter = 0;
	if(targetActor != 0) {
		sySndPlayStub(SE_CHEST_OPEN);
		fdActorSelectAnimationStub(targetActor, TREASURE_SPOT_CHEST_OPENING);
		sySynch(12);
		emitter = (Sint32)fdItemDispShowItemInFront(actorId, itemId);
	}
	return emitter;
}

/* Whether the actor 16 pixels in front of the leader, along its facing rounded
   to a quarter turn, can be pushed on by 16 pixels: 1 when nothing solid stands
   on the metatile beyond it or above it and its terrain test passes there, else
   0. fdLeaderPushObjectAhead, after it, makes the push. In every edition but
   the Japanese the actor Halt has stopped (MainRoutineStateWork.haltedActor)
   cannot be pushed; the Japanese release has no such test. */
Bool fdLeaderCanPushObjectAhead(void)
{
	Sint32 position[3];
	FIXED  above[3];
	ACTOR *owner = fdActorGetById(MainRoutineStateWork.leaderActorId);
	ACTOR *target;
	ACTOR *hit;
	Sint32 id;
	Sint32 terrain;
	Sint32 result;
	Sint32 angle = owner->facing / FACING_STEP;

	/* The facing rounded to the nearest quarter turn, in sixteenths; the probe
	   starts at the leader's height rounded to the nearest 8 pixels. */
	angle = ((angle + 2) & 0xc) << FACING_STEP_SHIFT;
	position[0] = owner->x.fixed;
	position[1] = (owner->y.fixed + toFIXED(4)) & ~(toFIXED(8) - 1);
	position[2] = owner->z.fixed;
	syVectorAddPolarOffset(toFIXED(16), angle, position);
	target = fdActorFindAtMetatile(position, TRUE);
	if(target == 0)
		return FALSE;
	if(target->sceneCnt != 0)
		return FALSE;
#if HALTED_ACTOR_CANNOT_BE_PUSHED
	/* With nothing halted the id is -1, for which fdActorGetById returns NULL,
	   so the test against 1 (where -1 looks meant) changes the result only when
	   actor 1 is the halted one, which it then lets be pushed. */
	id = MainRoutineStateWork.haltedActor;
	if(id != 1 && target == fdActorGetById(id))
		return FALSE;
#endif
	position[0] = target->x.fixed;
	position[1] = target->y.fixed;
	position[2] = target->z.fixed;
	syVectorAddPolarOffset(toFIXED(16), angle, position);
	hit = fdActorFindAtMetatile(position, TRUE);
	if(hit != 0 && (hit->collisionFlg & ACTOR_SOLID))
		return FALSE;
	above[0] = target->x.fixed;
	above[1] = target->y.fixed + toFIXED(16);
	above[2] = target->z.fixed;
	hit = fdActorFindAtMetatile(above, TRUE);
	if(hit != 0 && (hit->collisionFlg & ACTOR_SOLID))
		return FALSE;
	/* The metatile beyond is tested on the map's overhead layer. */
	terrain = target->layer;
	target->layer = MAPGRID_LAYER_OVERHEAD;
	result = fdMapGridCheckCollisionStub(target, position);
	target->layer = terrain;
	if(result > 0)
		return FALSE;
	return TRUE;
}

/* Pushes the actor ahead of the leader on by 16 pixels, as
   fdLeaderCanPushObjectAhead allowed. */
void fdLeaderPushObjectAhead(void)
{
	Sint32 position[3];
	ACTOR *owner = fdActorGetById(MainRoutineStateWork.leaderActorId);
	ACTOR *target;
	Sint32 terrain;
	Sint32 result;
	Sint32 speed;
	Sint32 acceleration;
	Sint32 angle = owner->facing / FACING_STEP;

	angle = ((angle + 2) & 0xc) << FACING_STEP_SHIFT;
	position[0] = owner->x.fixed;
	position[1] = (owner->y.fixed + toFIXED(4)) & ~(toFIXED(8) - 1);
	position[2] = owner->z.fixed;
	syVectorAddPolarOffset(toFIXED(16), angle, position);
	target = fdActorFindAtMetatile(position, TRUE);
	position[0] = target->x.fixed;
	position[1] = target->y.fixed;
	position[2] = target->z.fixed;
	syVectorAddPolarOffset(toFIXED(16), angle, position);
	/* The metatile beyond is tested on the map's overhead layer. */
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
		fdScriptNoOperation();
		fdActorSetPosAndResetMotionStub(
			target, position[0], position[1], position[2]);
		fdActorSetPosAndResetMotionStub(owner,
			toFIXED(owner->x.part[0].integer), owner->y.fixed,
			toFIXED(owner->z.part[0].integer));
		fdActorSelectAnimationStub(owner, ANIM_STAND);
		owner->speedLimit = speed;
		owner->acceleration = acceleration;
	}
}

#undef FACING_STEP_SHIFT
