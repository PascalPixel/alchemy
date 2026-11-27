/* The distance in whole pixels between two positions across the ground, in x */
/* and z, each given by its x and read on through the y and z that follow it. */
/* UB: an actor's x, y and z are separate members, read here as an array. */
s32 scPosFlatDistance(FIXED *a, FIXED *b) {
	s32 dx = FIXEDtoINT(a[0] - b[0]);
	s32 dz = FIXEDtoINT(a[2] - b[2]);

	return sySquart(dx * dx + dz * dz);
}
