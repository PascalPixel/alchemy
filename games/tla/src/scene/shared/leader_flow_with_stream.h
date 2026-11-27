/* Felix, walking into one of Aqua Rock's streams, is carried down it two pixels
   a tick, splashing, until something blocks the way; where it pours off a
   ledge he drops with the water and lands sitting down. It runs every tick he
   stands on the stream's metatiles; on the Moai room's dry ground it does
   nothing. */
void scLeaderFlowWithStream(void)
{
	ACTOR *camera;
	Sint32 id;
	ACTOR *leader;
	FIXED  pos[3];
	Sint32 hit;

	{
		FIELD_STATE *state = RamHeapSlots->fieldState;

		id = MainRoutineStateWork.leaderActorId;
		camera = state->camera;
	}
	leader = fdActorGet(id);
	if(MainRoutineStateWork.next == SCENE_AQUA_ROCK_MOAI &&
		leader->groundHeight == 0)
		return;
	pos[0] = leader->x.fixed;
	pos[1] = leader->y.fixed;
	pos[2] = leader->z.fixed + toFIXED(2);
	hit = fdMapGridCheckCollision(leader, pos);
	if((TickCount & 4) == 0)
		scActorSpawnWadingSplash(leader);
	if(hit < 0) {
		fdActorSetAttachedEffect(
			id, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
		scFieldActorSetMoveTargetFX(leader, leader->x.fixed, leader->y.fixed,
			leader->z.fixed + toFIXED(8));
		fdActorSelectAnimation(leader, ANIM_LEADER_SIT);
		scFieldActorWaitForMove(leader);
		do {
			syWait(1);
		} while(leader->y.fixed != leader->groundHeight);
		scActorSpawnWadingSplash(leader);
		fdActorSelectAnimation(leader, ANIM_LEADER_SIT);
		syWait(3);
		return;
	}
	pos[0] = leader->x.fixed;
	pos[1] = leader->y.fixed;
	pos[2] = leader->z.fixed + toFIXED(8);
	hit = fdMapGridCheckCollision(leader, pos);
	if(hit > 0)
		return;
	pos[0] = leader->x.fixed + (toFIXED(57) / 10);
	pos[1] = leader->y.fixed;
	pos[2] = leader->z.fixed + (toFIXED(57) / 10);
	hit = fdMapGridCheckCollision(leader, pos);
	if(hit > 0)
		return;
	pos[0] = leader->x.fixed - (toFIXED(57) / 10);
	pos[1] = leader->y.fixed;
	pos[2] = leader->z.fixed + (toFIXED(57) / 10);
	hit = fdMapGridCheckCollision(leader, pos);
	if(hit > 0)
		return;
	camera->z.fixed += toFIXED(2);
	leader->z.fixed += toFIXED(2);
	leader->facing = FACING_SOUTH;
}
