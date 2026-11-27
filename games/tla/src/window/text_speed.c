// The message speed tables, indexed by the player's message speed setting
// (MainRoutineStateWork.msgSpeed, MESSAGE_SPEED).

#include "types.h"

// Ticks to wait after each glyph while no button is held.
const Uint8 MessageSpeedDelays[3]         = {4, 0, 0};
// Ticks a page or a wait for a button lasts while B is held before the text goes on by itself; The
// Broken Seal has no such table.
const Uint8 MessageSpeedHeldPageDelays[3] = {30, 30, 10};
// Glyphs a message draws each tick.
const Uint8 MessageSpeedSteps[3]          = {1, 1, 10};
// Glyphs between two of the speaker's voice blips.
const Uint8 MessageSpeedVoiceGaps[3]      = {1, 3, 10};
