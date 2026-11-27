/* fdActorTurnTowardWatchedActor, the same in both games. Both games'
   field/script_commands_movement_types.c include it where the function
   stands. */

/* The SC_CALL function of MOVEMENT_TYPE_FACE_LEADER: turns the actor toward its
   watched actor, at most 1/16 turn a tick. */
Sint32 fdActorTurnTowardWatchedActor(ACTOR *actor)
{
	Sint16 turnStep;
	Uint16 curAng;
	Sint32 tgtAng;
	ACTOR *tgt;

	tgt = (ACTOR *)actor->watchedActor;
	if(tgt != NULL) {
		/* Clear ACTOR_FACES_MOTION, so moving does not turn it away again. */
		actor->steeringFlg &= ~ACTOR_FACES_MOTION;
		tgtAng = (Uint16)syAtan(
			tgt->z.fixed - actor->z.fixed, tgt->x.fixed - actor->x.fixed);
		curAng = actor->facing;
		turnStep = tgtAng - curAng;
		if(turnStep != 0) {
			if(turnStep > FACING_STEP)
				turnStep = FACING_STEP;
			if(turnStep < -FACING_STEP)
				turnStep = -FACING_STEP;
			actor->facing = (curAng + turnStep);
		}
	}
	return 1;
}
