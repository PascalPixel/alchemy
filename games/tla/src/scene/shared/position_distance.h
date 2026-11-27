// The distance in whole pixels between two positions, each given by its x and read on through the y
// and z that follow it, as an actor holds them. UB: an actor's x, y and z are separate members,
// read here as an array.
Sint32 scPosDistance(FIXED *first, FIXED *second)
{
	Sint32 deltaX = FIXEDtoINT(*first++ - *second++);
	Sint32 deltaY = FIXEDtoINT(*first++ - *second++);
	Sint32 deltaZ = FIXEDtoINT(*first - *second);

	return sySquartIWRAM(deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ);
}
