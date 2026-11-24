#ifndef ALCHEMY_MESSAGE_SPEED_H
#define ALCHEMY_MESSAGE_SPEED_H

/// @file
/// The message speed setting and the typing tables it picks a row of
/// (text_speed.c).

#include "types.h"

/// MainRoutineState.messageSpeed, as the settings name it
/// (MsgMessageSpeedSetting + speed): it indexes the speed tables below.
enum MessageSpeed {
  MESSAGE_SPEED_SLOW = 0,
  MESSAGE_SPEED_NORMAL = 1, ///< a new game's
  MESSAGE_SPEED_FAST = 2
};

extern const u8 gMessageSpeed_Delays[3];
extern const u8 gMessageSpeed_HeldPageDelays[3];
extern const u8 gMessageSpeed_Steps[3];
extern const u8 gMessageSpeed_VoiceGaps[3];

#endif
