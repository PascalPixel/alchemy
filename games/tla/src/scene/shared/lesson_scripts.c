// The lesson at the heart of each Rock, which Air's Rock, Aqua Rock, Gaia Rock and Magma Rock keep
// a copy of: as the Adept reads the tablet, the tablet lifts off its stand and the Adept rises as
// the sparks circle in, and both settle back as the lesson ends.

// The tablet lifts 24 pixels.
Uint32 LessonTabletRiseScript[] = {
	SC_MOVE_BY(0, 24, 0),
	SC_SYNC,
	SC_SET(SW_MOTION, 4),
	SC_END,
};

// The Adept rises 16 pixels.
Uint32 LessonAdeptRiseScript[] = {
	SC_MOVE_BY(0, 16, 0),
	SC_SYNC,
	SC_SET(SW_MOTION, 4),
	SC_END,
};

// The tablet settles back on its stand.
Sint32 LessonTabletSettleScript[] = {
	SC_SET(SW_MOTION, 0),
	SC_MOVE_BY(0, -24, 0),
	SC_SYNC,
	SC_SET(SW_MOTION, 0),
	SC_END,
};

// The Adept settles back to the ground.
Sint32 LessonAdeptSettleScript[] = {
	SC_SET(SW_MOTION, 0),
	SC_MOVE_BY(0, -16, 0),
	SC_SYNC,
	SC_SET(SW_MOTION, 3),
	SC_END,
};
