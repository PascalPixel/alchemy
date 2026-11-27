#ifndef ALCHEMY_MESSAGE_SPEED_H
#define ALCHEMY_MESSAGE_SPEED_H

// The message speed setting and the typing tables it picks a row of (text_speed.c).

#include "types.h"

// MAIN_ROUTINE_STATE.msgSpeed, as the settings name it (MsgMessageSpeedSetting
// + speed): it indexes the speed tables below.
typedef enum MESSAGE_SPEED
{
    MESSAGE_SPEED_SLOW   = 0,
    MESSAGE_SPEED_NORMAL = 1, // a new game's
    MESSAGE_SPEED_FAST   = 2
} MESSAGE_SPEED;

extern const Uint8 MessageSpeedDelays[3];
extern const Uint8 MessageSpeedHeldPageDelays[3];
extern const Uint8 MessageSpeedSteps[3];
extern const Uint8 MessageSpeedVoiceGaps[3];

#endif
