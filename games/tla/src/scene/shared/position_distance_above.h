// The distance in whole pixels between two positions in x and height, the first raised 6 pixels,
// each given by its x and read on through its y. UB: an actor's x, y and z are separate members,
// read here as an array.
Sint32 scPosDistanceAbove(FIXED *a, FIXED *b)
{
	Sint32 dx = FIXEDtoINT(*a++ - *b++);
	Sint32 dy = FIXEDtoINT(*a - *b + toFIXED(6));

	return sySquartIWRAM(dx * dx + dy * dy);
}
