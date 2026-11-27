/* The leader slides cell by cell over ice the way they face, turned to the */
/* nearest cardinal, until a wall, ground more than 8 pixels up, a solid actor
 */
/* or the end of the ice stops them. */
void fdLeaderSlideOnIceAhead(void) {
	VECTOR3 probe;
	s32 heading;
	s32 startBehavior;
	FIXED x;
	FIXED z;
	ACTOR *actor;
	s32 nextBehavior;
	u8 *layer;
	ACTOR *other;
	u8 *flags;
	s32 i;
	FIXED startX;
	FIXED startZ;

	actor = fdActorLookup(MainRoutineStateWork.leaderActorId);
	heading = (s16)FACING_NEAREST_CARDINAL(actor->facing);
	for(;;) {
		fdLeaderStopIfPending();
		actor->speedLimit = toFIXED(1);
		actor->acceleration = toFIXED(1) / 2;
		actor->tgtX = ACTOR_NO_TARGET;
		actor->tgtZ = ACTOR_NO_TARGET;
		actor->velX = 0;
		actor->velZ = 0;
		probe.x = startX = (actor->x.fixed & FIELD_CELL_MASK) + toFIXED(8);
		probe.y = actor->y.fixed;
		probe.z = startZ = (actor->z.fixed & FIELD_CELL_MASK) + toFIXED(8);
		x = startX;
		z = startZ;
		layer = &actor->layer;
		startBehavior = syFieldMapGridGetCellBehaviorAt(*layer, startX, startZ);
		syFieldVectorAddPolarOffset(16, heading, &probe);
		nextBehavior =
			syFieldMapGridGetCellBehaviorAt(*layer, probe.x, probe.z);
		if(nextBehavior == CELL_BEHAVIOR_WALL ||
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
				syFieldMapGridGetCellBehaviorAt(*layer, probe.x, probe.z);
			if(nextBehavior == CELL_BEHAVIOR_WALL) {
				break;
			}
			if(fdMapGetTerrainHeight(*layer, probe.x, probe.z) -
				   actor->y.fixed >
			   toFIXED(8)) {
				break;
			}
			{
				s32 radius = actor->radius - 2;
				other = Ram_HeapSlots->actors;
				flags = &other->collisionFlg;
				for(i = 0; i < ACTOR_COUNT;
					i++, flags += sizeof(ACTOR), other++) {
					if(other->script != NULL && (*flags & ACTOR_SOLID) &&
					   other != actor) {
						if(scEventCheckSpheresOverlap(&other->x.fixed, radius,
													  &probe.x,
													  other->radius - 2) >= 0) {
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
