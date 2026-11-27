// The tables of pushed_actor.c, which every scene with pushed things holds: the metatile ahead for
// each of sixteen headings, and the six rectangles.

#include "pushed_actor_direction_steps.c"

// The large things that roll, by sprite: a rectangle's row in PushedActorRectangles. Each lies west
// to east or north to south, the odd rows north to south.
Uint32 PushedActorRectangleKinds[] PUSHED_ACTOR_TABLE_SECTION = {
	SPRITE_LOG_WEST_EAST,
	SPRITE_LOG_NORTH_SOUTH,
	SPRITE_PIPE_WEST_EAST,
	SPRITE_PIPE_NORTH_SOUTH,
	SPRITE_FALLEN_COLUMN_WEST_EAST,
	SPRITE_FALLEN_COLUMN_NORTH_SOUTH,
};

// Each rectangle's left, top, right and bottom edges, in pixels from the thing's position: four
// metatiles by one, or a pipe's four by one north of it.
PUSHED_ACTOR_RECTANGLE
PushedActorRectangles[] PUSHED_ACTOR_TABLE_SECTION = {
	{-32, -8,  32, 8 }, // log, west to east
	{-8,  -32, 8,  32}, // log, north to south
	{-32, -16, 32, 0 }, // pipe, west to east
	{-8,  -32, 8,  32}, // pipe, north to south
	{-32, -8,  32, 8 }, // fallen column, west to east
	{-8,  -32, 8,  32}, // fallen column, north to south
};
