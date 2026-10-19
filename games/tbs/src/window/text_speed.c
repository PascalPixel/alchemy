#include "unit/window/text_speed.h"
#include "types.h"

// Message speed tables, indexed by the player's text speed setting
// (gGameState.messageSpeed, 0 to 2: slowest to fastest).

/// Frames to wait after each glyph while no button is held.
const u8 gUiText_SpeedDelays[3] = {4, 0, 0};
/// Glyphs a message channel draws each frame.
const u8 gUiText_SpeedSteps[3] = {1, 1, 10};
/// Glyphs between two of the speaker's voice blips.
const u8 gUiText_SpeedCueGaps[3] = {1, 3, 10};
