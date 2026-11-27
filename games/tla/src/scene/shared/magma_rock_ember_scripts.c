/* The scripts of the embers that fly up in Magma Rock, which its outside and
   its inside keep a copy of. */

/* An ember's actor script. */
Uint32 MagmaRockEmberScript[] = {
	SC_ANIMATION(2),
	SC_WAIT(18),
	SC_POSITION_ORIGIN,
	SC_WAIT(1),
	SC_RELEASE,
	SC_END,
};

/* A flame ember's actor script. */
Sint32 MagmaRockFlameEmberScript[] = {
	SC_ANIMATION(3),
	SC_WAIT(23),
	SC_POSITION_ORIGIN,
	SC_WAIT(1),
	SC_RELEASE,
	SC_END,
};
