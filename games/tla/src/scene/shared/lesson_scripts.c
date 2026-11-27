/* @file */
/* The lesson at the heart of each Rock, which Air's Rock, Aqua Rock, Gaia */
/* Rock and Magma Rock keep a copy of: as the Adept reads the tablet, the */
/* tablet lifts off its stand and the Adept rises as the sparks circle in, */
/* and both settle back as the lesson ends. */

/* The tablet lifts 24 pixels. */
u32 Lesson_TabletRiseScript[] = {
	SC_MOVE_BY(0, 24, 0),
	SC_SYNC,
	SC_SET(SW_MOTION, 4),
	SC_END,
};

/* The Adept rises 16 pixels. */
u32 Lesson_AdeptRiseScript[] = {
	SC_MOVE_BY(0, 16, 0),
	SC_SYNC,
	SC_SET(SW_MOTION, 4),
	SC_END,
};

/* The tablet settles back on its stand. */
s32 Lesson_TabletSettleScript[] = {
	SC_SET(SW_MOTION, 0),
	SC_MOVE_BY(0, -24, 0),
	SC_SYNC,
	SC_SET(SW_MOTION, 0),
	SC_END,
};

/* The Adept settles back to the ground. */
s32 Lesson_AdeptSettleScript[] = {
	SC_SET(SW_MOTION, 0),
	SC_MOVE_BY(0, -16, 0),
	SC_SYNC,
	SC_SET(SW_MOTION, 3),
	SC_END,
};
