/* 1 when the first actor stands just above the second. */
s32 fdActorIsAbove(ACTOR *a, ACTOR *b) {
	FIXED dy;

	if(ABS(a->x.fixed - b->x.fixed) < toFIXED(12)) {
		dy = a->y.fixed - b->y.fixed;
		if(dy > 0 && dy < toFIXED(16) &&
		   ABS(a->z.fixed - b->z.fixed) < toFIXED(8))
			return 1;
	}
	return 0;
}
