/* The tables of mercury_lighthouse_pillar.c, which Mercury Lighthouse and its
   entrance both hold. */

/* The column of light over the leader follows the leader and flickers. */
Sint32 MercuryLighthouse_PillarScript[] = {
	SC_CALL(scMercuryLighthouseFollowLeader),
	SC_END,
};

/* The heading a hop into the pool takes for each combination of held directions
   (DPAD_INDEX): only the four straight ones hop, and KEY_HEADING_NONE stays. */
/* clang-format off */
Uint16 MercuryLighthouse_MoveAngles[] = {
	KEY_HEADING_NONE, FACING_EAST, FACING_WEST, KEY_HEADING_NONE, FACING_NORTH, KEY_HEADING_NONE, KEY_HEADING_NONE, FACING_NORTH,
	FACING_SOUTH, KEY_HEADING_NONE, KEY_HEADING_NONE, FACING_SOUTH, KEY_HEADING_NONE, FACING_EAST, FACING_WEST, KEY_HEADING_NONE,
};
/* clang-format on */
