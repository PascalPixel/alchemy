/* The balances of the Tunnel Ruins and Venus Lighthouse, two platforms that
   weigh what stands on them against each other. */

/* A balance platform's height by its weight (its timer, from 0 to 6). Each
   carved stone or pillar on it takes it 16 pixels further down, and the other
   platform up as far. */
Uint32 Balance_PlatformHeights[] = {
	-toFIXED(16),
	-toFIXED(16),
	-toFIXED(32),
	-toFIXED(48),
	-toFIXED(64),
	-toFIXED(80),
	-toFIXED(80),
};
