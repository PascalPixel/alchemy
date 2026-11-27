/* The leader slides metatile by metatile over ice until something stops them: a
   wall, ground more than 8 pixels up, a solid actor, or the end of the ice. */
void fdLeaderSlideOnIce(void)
{
	VECTOR3 probe;
	Sint32	startBehavior;
	FIXED	x;
	FIXED	z;
	FIXED	startX;
	FIXED	startZ;
	Uint8  *layer;
	ACTOR  *actor;
	Sint32	heading;
	Sint32	nextBehavior;
	FIXED	distance;
	ACTOR  *other;
	Uint8  *flags;
	Sint32	i;
	FIXED	dx;
	FIXED	dz;

	actor = fdActorLookup(MainRoutineStateWork.leaderActorId);

top:
	fdLeaderStopIfPending();
	actor->speedLimit = toFIXED(1);
	actor->acceleration = toFIXED(1) / 2;
	actor->targetX = ACTOR_NO_TARGET;
	actor->targetZ = ACTOR_NO_TARGET;
	actor->velocityX = 0;
	actor->velocityZ = 0;
	probe.x = startX = (actor->x.fixed & FIELD_METATILE_MASK) + toFIXED(8);
	probe.y = actor->y.fixed;
	probe.z = startZ = (actor->z.fixed & FIELD_METATILE_MASK) + toFIXED(8);
	x = startX;
	z = startZ;
	layer = &actor->layer;
	startBehavior = syFieldMapGridGetMetatileBehaviorAt(*layer, x, z);
	dz = z - actor->z.fixed;
	dx = x - actor->x.fixed;
	distance = syMulFX(dx, dx) + syMulFX(dz, dz);
	if(distance < toFIXED(4)) {
		heading = SlideOnIceAnglesByKey[DPAD_INDEX(PerWork.data)];
		/* -1 for no direction, tested as its halfword. */
		if((Uint16)heading == DPAD_ANGLE_NONE) {
			return;
		}
	} else {
		heading = (Sint16)syAtan(z - actor->z.fixed, x - actor->x.fixed);
		heading = (Sint16)FACING_NEAREST_CARDINAL(heading);
	}
	syFieldVectorAddPolarOffset(16, heading, &probe);
	nextBehavior =
		syFieldMapGridGetMetatileBehaviorAt(*layer, probe.x, probe.z);
	if(nextBehavior == METATILE_BEHAVIOR_WALL ||
		fdMapGetTerrainHeight(*layer, probe.x, probe.z) - actor->y.fixed >
			toFIXED(8)) {
		actor->facing = heading;
		actor->velocityX = 0;
		actor->velocityZ = 0;
		actor->x.fixed = startX;
		actor->z.fixed = startZ;
		goto done;
	}
	probe.x = startX;
	probe.z = startZ;
	actor->speedLimit = toFIXED(2);
	actor->acceleration = toFIXED(1) / 10;
	actor->timer = 0;
	scFieldActorSetMoveTargetFX(actor, startX, actor->y.fixed, startZ);
	fdActorSelectAnimation(actor, ANIM_WALK);
	fdActorSetAnimationSpeed(actor, ANIM_STEP_NORMAL * 3);
	scFieldActorWaitForMove(actor);
	actor->update = fdActorSteerByPad;
	for(;;) {
		syFieldVectorAddPolarOffset(16, heading, &probe);
		nextBehavior =
			syFieldMapGridGetMetatileBehaviorAt(*layer, probe.x, probe.z);
		if(nextBehavior == METATILE_BEHAVIOR_WALL) {
			break;
		}
		if(fdMapGetTerrainHeight(*layer, probe.x, probe.z) - actor->y.fixed >
			toFIXED(8)) {
			break;
		}
		{
			Sint32 radius = actor->radius - 2;

			other = RamHeapSlots->actors;
			flags = &other->collisionFlags;
			for(i = 0; i < ACTOR_COUNT; i++, flags += sizeof(ACTOR), other++) {
				if(other->script != NULL && (*flags & ACTOR_SOLID) &&
					other != actor) {
					if(scScriptCheckSpheresOverlap(&other->x.fixed, radius,
						   &probe.x, other->radius - 2) >= 0) {
						goto stop;
					}
				}
			}
		}
		x = probe.x;
		z = probe.z;
		actor->speedLimit = toFIXED(2);
		actor->acceleration = toFIXED(1) / 10;
		scFieldActorSetMoveTargetFX(actor, probe.x, probe.y, probe.z);
		scFieldActorWaitForMove(actor);
		if(nextBehavior != startBehavior) {
			goto blocked;
		}
	}
stop:
	actor->speedLimit = toFIXED(2);
	actor->acceleration = toFIXED(1);
	scFieldActorSetMoveTargetFX(actor, x, actor->y.fixed, z);
	scFieldActorWaitForMove(actor);
	syWait(2);
	goto top;

blocked:
	syWait(10);
done:
	actor->update = NULL;
	actor->steeringFlags |= ACTOR_FACES_MOTION;
	actor->acceleration = toFIXED(1) / 4;
	fdActorSelectAnimation(actor, ANIM_STAND);
}
