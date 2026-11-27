/* 1 when `other` is nearer `self` than `range` pixels and lies in the */
/* sixteenth of a turn `self` faces or in one either side of it. */
s32 fdActorIsFacingNear(ACTOR *self, ACTOR *other, s32 range) {
	s32 result = 0;
	s32 *to;
	s32 *from;

	to = &other->x.fixed;
	from = &self->x.fixed;
	if(scPosDistance(to, from) < range) {
		u32 angle = (u16)syAtan(other->z.fixed - self->z.fixed, *to - *from);
		u32 left = FACING_SIXTEENTH(angle - FACING_STEP);
		u32 right = FACING_SIXTEENTH(angle + FACING_STEP);
		u32 ahead = FACING_SIXTEENTH(angle);
		u32 facing = FACING_SIXTEENTH(self->facing);

		if(ahead == facing || right == facing || left == facing)
			result = 1;
	}
	return result;
}
