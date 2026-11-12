/// @file
/// The message speed tables, indexed by the player's message speed setting
/// (gMainRoutineWork.messageSpeed, enum MessageSpeed).

#include "module/window/message_speed.h"
#include "types.h"

/// Frames to wait after each glyph while no button is held.
const u8 gMessageSpeed_Delays[3] = {4, 0, 0};
/// Glyphs a message draws each frame.
const u8 gMessageSpeed_Steps[3] = {1, 1, 10};
/// Glyphs between two of the speaker's voice blips.
const u8 gMessageSpeed_VoiceGaps[3] = {1, 3, 10};
