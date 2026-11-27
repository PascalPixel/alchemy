/* The distance in whole pixels between two positions in x and height, the */
/* first raised 6 pixels, each given by its x and read on through its y. */
/* UB: an actor's x, y and z are separate members, read here as an array. */
s32 scPosDistanceAbove(FIXED *a, FIXED *b) {
	s32 dx = FIXEDtoINT(*a++ - *b++);
	s32 dy = FIXEDtoINT(*a - *b + toFIXED(6));

	return sySquart(dx * dx + dy * dy);
}
