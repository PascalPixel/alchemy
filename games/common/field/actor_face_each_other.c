/* fdActorFaceActorById, fdActorTurnToFaceEachOther and
   fdActorTurnPairToFaceEachOther, the same in both games. The Broken Seal's
   field/script_commands_actor_commands.c and The Lost Age's
   field/field_mode_actor_commands.c include it where the functions stand. */

/* The script command "actor face actor": turns the first actor at once to face
   the second, then waits the given number of ticks. */
void fdActorFaceActorById(Sint32 firstId, Sint32 secondId, Sint32 ticks)
{
	ACTOR *first;
	ACTOR *second;

	first = fdActorLookup(firstId);
	second = fdActorLookup(secondId);
	if(first != 0 && second != 0) {
		first->facing = syAtan(
			second->z.fixed - first->z.fixed, second->x.fixed - first->x.fixed);
		fdScriptWait(ticks);
	}
}

/* The script command "actors face each other": turns two actors toward each
   other, then waits the given number of ticks. */
void fdActorTurnToFaceEachOther(Sint32 firstId, Sint32 secondId, Sint32 ticks)
{
	void *first = fdActorLookup(firstId);
	void *second = fdActorLookup(secondId);

	if(first != NULL && second != NULL) {
		fdActorTurnPairToFaceEachOther(first, second);
		fdScriptWait(ticks);
	}
}

/* Turns two actors to face each other, at most 1/16 turn a tick each, until
   both do (at most 60 ticks). */
void fdActorTurnPairToFaceEachOther(ACTOR *first, ACTOR *second)
{
	Sint32 firstTgtAng;
	Sint32 secondTgtAng;
	Sint32 frm;
	Sint32 actorsTurning;
	Sint32 angDifference;

	if(first == NULL || second == NULL)
		return;
	firstTgtAng = (Uint16)syAtan(
		second->z.fixed - first->z.fixed, second->x.fixed - first->x.fixed);
	/* The second actor faces the opposite way, half a turn round. */
	secondTgtAng = firstTgtAng + DEGtoANG(180);
	for(frm = 0; frm < 60; frm++) {
		/* Each actor that already faces its way takes one off the count; both
		   facing ends the turn early. */
		actorsTurning = 2;
		angDifference = (Sint16)(firstTgtAng - first->facing);
		if(angDifference != 0) {
			if(angDifference > FACING_STEP)
				angDifference = FACING_STEP;
			if(angDifference < -FACING_STEP)
				angDifference = -FACING_STEP;
			first->facing += angDifference;
		} else {
			actorsTurning = 1;
		}
		angDifference = (Sint16)(secondTgtAng - second->facing);
		if(angDifference != 0) {
			if(angDifference > FACING_STEP)
				angDifference = FACING_STEP;
			if(angDifference < -FACING_STEP)
				angDifference = -FACING_STEP;
			second->facing += angDifference;
		} else {
			actorsTurning--;
		}
		if(actorsTurning == 0)
			break;
		sySynch(1);
	}
}
