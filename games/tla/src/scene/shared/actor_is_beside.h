/* 1 when two actors stand side by side. */
s32 fdActorIsBeside(ACTOR *a, ACTOR *b) {
	FIXED dy;

	if(ABS(a->x.fixed - b->x.fixed) < toFIXED(12)) {
		dy = a->y.fixed - b->y.fixed;
		if(dy > -toFIXED(8) && dy < toFIXED(24) &&
		   ABS(a->z.fixed - b->z.fixed) < toFIXED(12))
			return 1;
	}
	return 0;
}
