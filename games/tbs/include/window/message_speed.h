#ifndef ALCHEMY_MESSAGE_SPEED_H
#define ALCHEMY_MESSAGE_SPEED_H

/// @file
/// The message speed setting and how fast each speed types a message.

#include "types.h"

/// MainRoutineState.messageSpeed, as the settings menu names it
/// (MsgMessageSpeedSetting + speed): it indexes the speed tables below.
enum MessageSpeed {
  MESSAGE_SPEED_SLOW = 0,
  MESSAGE_SPEED_NORMAL = 1, ///< a new game's
  MESSAGE_SPEED_FAST = 2
};

extern const u8 gMessageSpeed_Delays[3];
extern const u8 gMessageSpeed_Steps[3];
extern const u8 gMessageSpeed_VoiceGaps[3];

// The German counted nouns that follow these tables are a file of their own,
// window/counted_nouns.c: their table of pointers is word-aligned, and in one
// file with it these byte tables would be too.

#endif
