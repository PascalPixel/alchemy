/* fdActorIsTargetUnset, the same in both games. Both games'
   field/field_actor_motion.c include it where the function stands. */

/* Returns 1 when the actor has no movement target: none of its target
   coordinates set, or for an actor moved by the ground (any motion flag)
   neither its target x nor z. */
Bool fdActorIsTargetUnset(ACTOR *actor)
{
	Sint32 first;
	Sint32 second;

	/* One compare of three values: all must equal ACTOR_NO_TARGET, or with
	   motion flags only x and z. */
	if(actor->motionFlg == 0) {
		second = actor->tgtX;
		if(second != ACTOR_NO_TARGET)
			return FALSE;
		first = actor->tgtY;
	} else {
		first = actor->tgtX;
		second = ACTOR_NO_TARGET;
	}
	if(first != second || actor->tgtZ != first)
		return FALSE;
	return TRUE;
}
