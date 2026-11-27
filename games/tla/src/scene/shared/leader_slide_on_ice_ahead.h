/* The leader slides metatile by metatile over ice the way they face, turned to
   the nearest cardinal, until a wall, ground more than 8 pixels up, a solid
   actor or the end of the ice stops them. */
void fdLeaderSlideOnIceAhead(void)
{
	VECTOR3 probe;
	Sint32	heading;
	Sint32	startBehavior;
	FIXED	x;
	FIXED	z;
	ACTOR  *actor;
	Sint32	nextBehavior;
	Uint8  *layer;
	ACTOR  *other;
	Uint8  *flg;
	Sint32	i;
	FIXED	startX;
	FIXED	startZ;

	actor = fdActorLookup(MainRoutineStateWork.leaderActorId);
	heading = (Sint16)FACING_NEAREST_CARDINAL(actor->facing);
	for(;;) {
		fdLeaderStopIfPending();
		actor->speedLimit = toFIXED(1);
		actor->acceleration = toFIXED(1) / 2;
		actor->tgtX = ACTOR_NO_TARGET;
		actor->tgtZ = ACTOR_NO_TARGET;
		actor->velX = 0;
		actor->velZ = 0;
		probe.x = startX = (actor->x.fixed & FIELD_METATILE_MASK) + toFIXED(8);
		probe.y = actor->y.fixed;
		probe.z = startZ = (actor->z.fixed & FIELD_METATILE_MASK) + toFIXED(8);
		x = startX;
		z = startZ;
		layer = &actor->layer;
		startBehavior =
			syFieldMapGridGetMetatileBehaviorAt(*layer, startX, startZ);
		syFieldVectorAddPolarOffset(16, heading, &probe);
		nextBehavior =
			syFieldMapGridGetMetatileBehaviorAt(*layer, probe.x, probe.z);
		if(nextBehavior == METATILE_BEHAVIOR_WALL ||
			fdMapGetTerrainHeight(*layer, probe.x, probe.z) - actor->y.fixed >
				toFIXED(8)) {
			actor->facing = heading;
			actor->velX = 0;
			actor->velZ = 0;
			actor->x.fixed = startX;
			actor->z.fixed = startZ;
			goto done;
		}
		probe.x = x;
		probe.z = z;
		actor->speedLimit = toFIXED(2);
		actor->acceleration = toFIXED(1) / 10;
		actor->tmr = 0;
		fdActorSelectAnimation(actor, ANIM_WALK);
		fdActorSetAnimationSpeed(actor, ANIM_STEP_NORMAL * 3);
		actor->update = fdActorSteerByPad;
		for(;;) {
			syFieldVectorAddPolarOffset(16, heading, &probe);
			nextBehavior =
				syFieldMapGridGetMetatileBehaviorAt(*layer, probe.x, probe.z);
			if(nextBehavior == METATILE_BEHAVIOR_WALL) {
				break;
			}
			if(fdMapGetTerrainHeight(*layer, probe.x, probe.z) -
					actor->y.fixed >
				toFIXED(8)) {
				break;
			}
			{
				Sint32 radius = actor->radius - 2;
				other = RamHeapSlots->actors;
				flg = &other->collisionFlg;
				for(i = 0; i < ACTOR_COUNT;
					i++, flg += sizeof(ACTOR), other++) {
					if(other->script != NULL && (*flg & ACTOR_SOLID) &&
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
	}

blocked:
	syWait(10);
done:
	actor->update = NULL;
	actor->steeringFlg |= ACTOR_FACES_MOTION;
	actor->acceleration = toFIXED(1) / 4;
	fdActorSelectAnimation(actor, ANIM_STAND);
}
