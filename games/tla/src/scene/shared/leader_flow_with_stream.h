/* Felix, walking into one of Aqua Rock's streams, is carried down it two pixels
   a frame, splashing, until something blocks the way; where it pours off a
   ledge he drops with the water and lands sitting down. It runs every frame he
   stands on the stream's metatiles; on the Moai room's dry ground it does
   nothing. */
void scLeaderFlowWithStream(void)
{
	ACTOR *camera;
	Sint32 id;
	ACTOR *leader;
	FIXED  position[3];
	Sint32 hit;

	{
		FIELD_STATE *state = Ram_HeapSlots->fieldState;

		id = MainRoutineStateWork.leaderActorId;
		camera = state->camera;
	}
	leader = fdActorGet(id);
	if(MainRoutineStateWork.next == SCENE_AQUA_ROCK_MOAI &&
		leader->groundHeight == 0)
		return;
	position[0] = leader->x.fixed;
	position[1] = leader->y.fixed;
	position[2] = leader->z.fixed + toFIXED(2);
	hit = fdMapGridCheckCollision(leader, position);
	if((FrameCount & 4) == 0)
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
	position[0] = leader->x.fixed;
	position[1] = leader->y.fixed;
	position[2] = leader->z.fixed + toFIXED(8);
	hit = fdMapGridCheckCollision(leader, position);
	if(hit > 0)
		return;
	position[0] = leader->x.fixed + (toFIXED(57) / 10);
	position[1] = leader->y.fixed;
	position[2] = leader->z.fixed + (toFIXED(57) / 10);
	hit = fdMapGridCheckCollision(leader, position);
	if(hit > 0)
		return;
	position[0] = leader->x.fixed - (toFIXED(57) / 10);
	position[1] = leader->y.fixed;
	position[2] = leader->z.fixed + (toFIXED(57) / 10);
	hit = fdMapGridCheckCollision(leader, position);
	if(hit > 0)
		return;
	camera->z.fixed += toFIXED(2);
	leader->z.fixed += toFIXED(2);
	leader->facing = FACING_SOUTH;
}
