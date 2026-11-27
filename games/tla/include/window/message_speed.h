#ifndef ALCHEMY_MESSAGE_SPEED_H
#define ALCHEMY_MESSAGE_SPEED_H

/* @file */
/* The message speed setting and the typing tables it picks a row of */
/* (text_speed.c). */

#include "types.h"

/* MAIN_ROUTINE_STATE.msgSpeed, as the settings name it */
/* (MsgMessageSpeedSetting + speed): it indexes the speed tables below. */
typedef enum MESSAGE_SPEED {
	MESSAGE_SPEED_SLOW = 0,
	MESSAGE_SPEED_NORMAL = 1, /* a new game's */
	MESSAGE_SPEED_FAST = 2
} MESSAGE_SPEED;

extern const u8 MessageSpeed_Delays[3];
extern const u8 MessageSpeed_HeldPageDelays[3];
extern const u8 MessageSpeed_Steps[3];
extern const u8 MessageSpeed_VoiceGaps[3];

#endif
