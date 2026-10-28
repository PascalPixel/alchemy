#ifndef ALCHEMY_UNIT_WINDOW_MESSAGE_SPEED_H
#define ALCHEMY_UNIT_WINDOW_MESSAGE_SPEED_H

#include "types.h"

/// Adventure.messageSpeed, as the settings name it
/// (MsgMessageSpeedSetting
/// + speed): it indexes the speed tables below.
enum MessageSpeed {
  MESSAGE_SPEED_SLOW = 0,
  MESSAGE_SPEED_NORMAL = 1, ///< a new game's
  MESSAGE_SPEED_FAST = 2
};

extern const u8 gMessageSpeed_Delays[3];
extern const u8 gMessageSpeed_Steps[3];
extern const u8 gMessageSpeed_VoiceGaps[3];

// The German counted nouns of graphics/text/numwords.c come right after these
// tables and stay their own object: in tbs-de the speed tables start at an odd
// address, right after genders.o, and in one file with
// gLocalizedWord_CountedNouns, a word-aligned pointer table, the object would
// start at the next word, 3 bytes later. Split evidence for
// message_speed|numwords.

#endif
