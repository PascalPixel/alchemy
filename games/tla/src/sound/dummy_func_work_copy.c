/// @file
/// An older copy of the sound driver's empty callback, which the ROM keeps
/// beside the driver with the player routines of player_work_copy.c.

#include "types.h"

/// Unused: does nothing, as DummyFunc (empty_callback.c); nothing refers to
/// it.
void DummyFuncWorkCopy(void) {}
