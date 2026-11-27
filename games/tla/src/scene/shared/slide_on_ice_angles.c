// The headings the direction pad gives on ice, which the scenes with ice to slide on keep a copy
// of, both tables indexed by the pad's four bits ((PerWork.data >> 4) & 15: right, left, up, down);
// -1 where the keys held give no heading.

// The heading fdActorSteerByPad turns toward, diagonals included; a second row of sixteen keeps
// only the four straight headings.
Sint16 SlideOnIceSteerAngles[] = {
	-1,
	FACING_EAST,
	FACING_WEST,
	-1,
	FACING_NORTH,
	FACING_NORTHEAST,
	FACING_NORTHWEST,
	FACING_NORTH,
	FACING_SOUTH,
	FACING_SOUTHEAST,
	FACING_SOUTHWEST,
	FACING_SOUTH,
	-1,
	FACING_EAST,
	FACING_WEST,
	-1,
	-1,
	FACING_EAST,
	FACING_WEST,
	-1,
	FACING_NORTH,
	-1,
	-1,
	FACING_NORTH,
	FACING_SOUTH,
	-1,
	-1,
	FACING_SOUTH,
	-1,
	FACING_EAST,
	FACING_WEST,
	-1,
};

// The heading a slide sets off in: a diagonal goes the way of its left or right key.
Sint16 SlideOnIceAnglesByKey[] = {
	-1,
	FACING_EAST,
	FACING_WEST,
	-1,
	FACING_NORTH,
	FACING_EAST,
	FACING_WEST,
	FACING_NORTH,
	FACING_SOUTH,
	FACING_EAST,
	FACING_WEST,
	FACING_SOUTH,
	-1,
	FACING_EAST,
	FACING_WEST,
	-1,
};
