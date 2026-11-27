/* Puts `actor` where `target` stands, at rest, and `target` away at the
   origin. */
void scActorTakePlaceOf(ACTOR *actor, ACTOR *target)
{
	scFieldActorSetPosAndResetMotion(
		actor, target->x.fixed, target->y.fixed, target->z.fixed);
	scFieldActorSetPosAndResetMotion(target, 0, 0, 0);
}
