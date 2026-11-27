/* fdActorSetPosAndResetMotion, the same in both games. Both games'
   field/field_actor_motion.c include it where the function stands. */

/* Places the actor at (x, y, z), in 16.16 pixels, clears its movement target
   and stops it. */
void fdActorSetPosAndResetMotion(ACTOR *actor, FIXED x, FIXED y, FIXED z)
{
	actor->z.fixed = z;
	actor->tgtX = ACTOR_NO_TARGET;
	actor->tgtY = ACTOR_NO_TARGET;
	actor->tgtZ = ACTOR_NO_TARGET;
	actor->x.fixed = x;
	actor->y.fixed = y;
	SET_VELOCITY(actor, 0, 0, 0);
}
