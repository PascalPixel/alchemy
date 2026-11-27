/* Stops a pushed thing where it is when an actor stands in the metatile ahead
   of it, unless that is another pushable thing, or when the map blocks it. */
void scActorStopAtPushTarget(ACTOR *actor)
{
	FIXED  pos[3];
	ACTOR *hit;
	Uint32 i;
	Sint32 angle = FACING_INDEX(actor->facing);

	/* The facing rounded to the nearest of east, south, west and north: two
	   sixteenths on, whole quarters kept, and back from sixteenths to a
	   facing. */
	angle = FACING_FROM_INDEX((angle + 2) & 0xc);
	pos[0] = actor->x.fixed;
	pos[1] = actor->y.fixed;
	pos[2] = actor->z.fixed;
	syFieldVectorAddPolarOffset(16, angle, pos);
	hit = fdActorFindAtMetatile(pos, 1);
	if(hit != 0) {
		for(i = 0; i <= PUSH_KINDS - 1; i++) {
			if(((SPR *)hit->spr)->entries[0]->anmId == PushKinds[i])
				return;
		}
		scFieldActorSetPosAndResetMotion(
			actor, actor->x.fixed, actor->y.fixed, actor->z.fixed);
	}
	pos[0] = actor->x.fixed;
	pos[1] = actor->y.fixed;
	pos[2] = actor->z.fixed;
	if(fdMapGridCheckCollision(actor, pos) > 0)
		scFieldActorSetPosAndResetMotion(
			actor, actor->x.fixed, actor->y.fixed, actor->z.fixed);
}
