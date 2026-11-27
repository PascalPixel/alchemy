/* The leader pushes the object to its resting place: the metatiles it leaves
   are floor again and the metatiles it comes to are walls. */
void scPushTargetPush(PUSH_TARGET tgt)
{
	MAP_STATE *map = RamHeapSlots->mapGrid;
	ACTOR	  *leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	ACTOR	  *actor = fdActorGet(tgt.id);
	Sint32	   metatile[3];
	FIXED	   from[3];
	Sint32	   rectangle;
	Sint32	   width;
	Sint32	   height;
	Sint32	   originX;
	Sint32	   originZ;
	Sint32	   direction;
	Sint32	   speed;
	Sint32	   acceleration;

	/* BUG: the leader's facing is taken as a 16.16 number, whose whole part is
	   always 0 for this 16-bit angle, where its sixteenth (FACING_INDEX) was
	   meant: every push takes direction 0 (east), so the pushed thing rolls
	   with animation 2 whichever way it goes and the leader's step after it is
	   none. */
#if FIX_PUSH_FACING_AND_LEADER_STEP
	direction = FACING_INDEX(leader->facing);
#else
	direction = FIXEDtoINT(leader->facing);
#endif
	rectangle = tgt.rectangle;
	speed = leader->speedLimit;
	acceleration = leader->acceleration;
	from[0] = actor->x.fixed;
	from[2] = actor->z.fixed;
	metatile[0] =
		FIELD_METATILE(actor->x.fixed + toFIXED(PushRectangles[rectangle][0]));
	metatile[2] =
		FIELD_METATILE(actor->z.fixed + toFIXED(PushRectangles[rectangle][1]));
	width = (ABS(PushRectangles[rectangle][0]) +
				ABS(PushRectangles[rectangle][2])) >>
		4;
	height = (ABS(PushRectangles[rectangle][1]) +
				 ABS(PushRectangles[rectangle][3])) >>
		4;
	fdMapGridFillCollisionRectangle(MAPGRID_LAYER_GROUND, metatile[0],
		metatile[2], width, height, METATILE_BEHAVIOR_NORMAL);
	fdActorSetSpeedAndAcceleration(
		MainRoutineStateWork.leaderActorId, 0.5, 0.1);
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_PUSH);
	syWait(15);
	/* The leader follows the object half its way. */
	{
		Sint32 deltaX = FIXEDtoINT(tgt.x - from[0]) >> 1;
		Sint32 deltaZ = FIXEDtoINT(tgt.z - from[2]) >> 1;

		fdActorSetDestinationOffset(
			MainRoutineStateWork.leaderActorId, deltaX, deltaZ);
	}
	fdActorGet(MainRoutineStateWork.leaderActorId)->update =
		scActorStopAtPushTarget;
	syWait(4);
	if(direction >= FACING_INDEX(FACING_SOUTHWEST) &&
		direction <= FACING_INDEX(FACING_NORTH_NORTHEAST))
		fdActorSelectAnimation(actor, 3);
	else
		fdActorSelectAnimation(actor, 2);
	fdScriptPlaySnd(SE_HEAVY_MOVE);
	fdActorSetSpeedAndAcceleration(tgt.id, 0.5, 0.1);
	scFieldActorSetMoveTargetFX(actor, tgt.x, tgt.y, tgt.z);
	scFieldActorWaitForMoveById(MainRoutineStateWork.leaderActorId);
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_WALK);
	fdActorSetSpeedAndAcceleration(
		MainRoutineStateWork.leaderActorId, 0.3, 0.1);
	/* BUG: the leader's step after the push reads both halves from the step's
	   z: x is its sign and z its half (half of PUSH_STEP_X and PUSH_STEP_Z was
	   meant). With direction 0 both are 0. */
	{
		Sint32 step = PushLeaderSteps[direction];

#if FIX_PUSH_FACING_AND_LEADER_STEP
		fdActorSetDestinationOffset(MainRoutineStateWork.leaderActorId,
			PUSH_STEP_X(step) / 2, PUSH_STEP_Z(step) / 2);
#else
		fdActorSetDestinationOffset(MainRoutineStateWork.leaderActorId,
			toFIXED(step) >> 31, FIXEDtoINT(toFIXED(step)) >> 1);
#endif
	}
	if(tgt.func != 0)
		((void (*)(void))tgt.func)();
	scFieldActorWaitForMoveById(MainRoutineStateWork.leaderActorId);
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_STAND);
	leader->update = NULL;
	leader->speedLimit = speed;
	leader->acceleration = acceleration;
	scFieldActorWaitForMove(actor);
	fdScriptPlaySnd(SE_STOP_EXIT_SE);
	fdScriptPlaySnd(SE_HEAVY_STOP);
	scFieldActorSetPosAndResetMotion(actor, tgt.x, actor->y.fixed, tgt.z);
	fdActorSelectAnimation(actor, 1);
	tgt.x = tgt.x + toFIXED(PushRectangles[rectangle][0]);
	tgt.z = tgt.z + toFIXED(PushRectangles[rectangle][1]);
	tgt.x >>= 20;
	tgt.z >>= 20;
	originX = FIELD_METATILE(map->layers[MAPGRID_LAYER_MIDDLE].ofsX);
	originZ = FIELD_METATILE(map->layers[MAPGRID_LAYER_MIDDLE].ofsY);
	fdMapGridCopyMetatileAttributes(
		tgt.x, tgt.z, width, height, originX + tgt.x, originZ + tgt.z);
	fdMapGridFillCollisionRectangle(MAPGRID_LAYER_GROUND, tgt.x, tgt.z, width,
		height, METATILE_BEHAVIOR_WALL);
	fdMapGridFillCollisionRectangle(MAPGRID_LAYER_OVERHEAD, tgt.x, tgt.z, width,
		height, METATILE_BEHAVIOR_WALL);
	metatile[0] =
		FIELD_METATILE(from[0] + toFIXED(PushRectangles[rectangle][0]));
	metatile[2] =
		FIELD_METATILE(from[2] + toFIXED(PushRectangles[rectangle][1]));
	originX += metatile[0];
	originZ += metatile[2];
	fdMapGridCopyMetatileAttributes(
		originX, originZ, width, height, metatile[0], metatile[2]);
	fdMapGridFillCollisionRectangle(MAPGRID_LAYER_OVERHEAD, metatile[0],
		metatile[2], width, height, METATILE_BEHAVIOR_NORMAL);
	fdScriptNoOperation();
}
