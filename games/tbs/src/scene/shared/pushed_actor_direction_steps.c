/* The metatile ahead of a pushed thing for each of sixteen headings, one
   metatile (16 pixels) along x or z, which every scene with pushed things
   holds. */

/* An includer that keeps the pushed-actor tables with its read-only data
   defines PUSHED_ACTOR_TABLES_READ_ONLY first. */
#ifdef PUSHED_ACTOR_TABLES_READ_ONLY
#define PUSHED_ACTOR_TABLE_SECTION __attribute__((section(".rodata")))
#else
#define PUSHED_ACTOR_TABLE_SECTION
#endif

Sint32 PushedActorDirectionSteps[] PUSHED_ACTOR_TABLE_SECTION = {
	DIRECTION_STEP(16, 0),
	DIRECTION_STEP(16, 0),
	DIRECTION_STEP(16, 0),
	DIRECTION_STEP(0, 16),
	DIRECTION_STEP(0, 16),
	DIRECTION_STEP(0, 16),
	DIRECTION_STEP(0, 16),
	DIRECTION_STEP(-16, 0),
	DIRECTION_STEP(-16, 0),
	DIRECTION_STEP(-16, 0),
	DIRECTION_STEP(-16, 0),
	DIRECTION_STEP(0, -16),
	DIRECTION_STEP(0, -16),
	DIRECTION_STEP(0, -16),
	DIRECTION_STEP(0, -16),
	DIRECTION_STEP(16, 0),
};
