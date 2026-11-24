/// @file
/// The message speed tables, indexed by the player's message speed setting
/// (gMainRoutineState.messageSpeed, enum MessageSpeed).

#include "window/message_speed.h"
#include "types.h"

/// Frames to wait after each glyph while no button is held.
const u8 gMessageSpeed_Delays[3] = {4, 0, 0};
/// Frames a page or a wait for a button lasts while B is held before the text
/// goes on by itself; The Broken Seal has no such table.
const u8 gMessageSpeed_HeldPageDelays[3] = {30, 30, 10};
/// Glyphs a message draws each frame.
const u8 gMessageSpeed_Steps[3] = {1, 1, 10};
/// Glyphs between two of the speaker's voice blips.
const u8 gMessageSpeed_VoiceGaps[3] = {1, 3, 10};
