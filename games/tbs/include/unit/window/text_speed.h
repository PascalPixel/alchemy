#ifndef ALCHEMY_UNIT_WINDOW_TEXT_SPEED_H
#define ALCHEMY_UNIT_WINDOW_TEXT_SPEED_H

#include "types.h"

/// GameState.messageSpeed, as the settings name it (MsgMessageSpeedSetting
/// + speed): it indexes the speed tables below.
enum MessageSpeed {
  MESSAGE_SPEED_SLOW = 0,
  MESSAGE_SPEED_NORMAL = 1, ///< a new game's
  MESSAGE_SPEED_FAST = 2
};

extern const u8 gUiText_SpeedDelays[3];
extern const u8 gUiText_SpeedSteps[3];
extern const u8 gUiText_SpeedCueGaps[3];

// The German counted nouns of graphics/text/numwords.c come right after these
// tables and stay their own object: in tbs-de the speed tables start at an odd
// address, right after genders.o, and in one file with gUiText_NumberWords, a
// word-aligned pointer table, the object would start at the next word, 3 bytes
// later. Split evidence for text_speed|numwords.

#endif
