#ifndef ALCHEMY_MESSAGE_SPEED_H
#define ALCHEMY_MESSAGE_SPEED_H

/* The message speed setting and how fast each speed types a message. */

#include "types.h"

/* MAIN_ROUTINE_STATE.msgSpeed, as the settings menu names it
   (MsgMessageSpeedSetting + speed): it indexes the speed tables below. */
typedef enum MESSAGE_SPEED {
	MESSAGE_SPEED_SLOW = 0,
	MESSAGE_SPEED_NORMAL = 1, /* a new game's */
	MESSAGE_SPEED_FAST = 2
} MESSAGE_SPEED;

extern const Uint8 MessageSpeed_Delays[3];
extern const Uint8 MessageSpeed_Steps[3];
extern const Uint8 MessageSpeed_VoiceGaps[3];

/* The German counted nouns that follow these tables are a file of their own,
   window/counted_nouns.c: their table of pointers is word-aligned, and in one
   file with it these byte tables would be too. */

#endif
