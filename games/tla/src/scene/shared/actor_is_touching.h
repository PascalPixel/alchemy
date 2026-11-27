/* 1 when two actors touch. */
Bool fdActorIsTouching(ACTOR *a, ACTOR *b)
{
	if(ABS(a->x.fixed - b->x.fixed) < toFIXED(8) &&
		ABS(a->y.fixed - b->y.fixed) < toFIXED(8) &&
		ABS(a->z.fixed - b->z.fixed) < toFIXED(8))
		return TRUE;
	return FALSE;
}
