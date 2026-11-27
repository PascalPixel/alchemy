/* The message speed tables, indexed by the player's message speed setting
   (MainRoutineStateWork.msgSpeed, MESSAGE_SPEED). */

#include "types.h"

/* Ticks to wait after each glyph while no button is held. */
const Uint8 MessageSpeedDelays[3] = {4, 0, 0};
/* Glyphs a message draws each tick. */
const Uint8 MessageSpeedSteps[3] = {1, 1, 10};
/* Glyphs between two of the speaker's voice blips. */
const Uint8 MessageSpeedVoiceGaps[3] = {1, 3, 10};
