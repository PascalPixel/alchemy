/// @file
/// The things the leader can push and the cells each covers, which the scenes
/// with pushed things keep a copy of (PUSH_KINDS, shared/tables.h).

/// The sprites of the things pushed: a log, a pipe and a fallen column, each
/// lying west to east and north to south.
u16 gPushKinds[PUSH_KINDS] = {
    SPRITE_LOG_WEST_EAST,           SPRITE_LOG_NORTH_SOUTH,
    SPRITE_PIPE_WEST_EAST,          SPRITE_PIPE_NORTH_SOUTH,
    SPRITE_FALLEN_COLUMN_WEST_EAST, SPRITE_FALLEN_COLUMN_NORTH_SOUTH,
};

/// Each kind's footprint: its corners from the actor, x and z in pixels
/// (PushTarget_FillFootprint).
s8 gPushFootprints[][4] = {
    {-32, -8, 32, 8}, {-8, -32, 8, 32}, {-32, -16, 32, 0},
    {-8, -32, 8, 32}, {-32, -8, 32, 8}, {-8, -32, 8, 32},
};
