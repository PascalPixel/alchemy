/* Puts `actor` where `tgt` stands, at rest, and `tgt` away at the
   origin. */
void scActorTakePlaceOf(ACTOR *actor, ACTOR *tgt)
{
	scFieldActorSetPosAndResetMotion(
		actor, tgt->x.fixed, tgt->y.fixed, tgt->z.fixed);
	scFieldActorSetPosAndResetMotion(tgt, 0, 0, 0);
}
