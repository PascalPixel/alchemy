#ifndef ALCHEMY_FIELD_SEQUENCE_EVENT_H
#define ALCHEMY_FIELD_SEQUENCE_EVENT_H

/* @file */
/* The field sequence's event functions (field_sequence/event.c and items.c). */

#include "field/field_state.h"

/* TRIGGER (scene_tables.h), a row of the scene's trigger table, as */
/* the Psynergy and item triggers run it: its control word as key, its */
/* subject as flags, its condition unsigned, and its value as action (a */
/* message below 0x10000, else a function). */
typedef struct PENDING_EVENT {
	u32 key;
	u16 flg;
	u16 condition;
	s32 action;
} PENDING_EVENT;

void fdEventRunMessage(s32 actor, s32 message);
s32 fdRunPsynergyEndEvents(u32 action);
s32 fdRunPsynergyEvent(PENDING_EVENT *event, s32 argument, s32 target);
void fdEventUpdateDebugSkip(void);
void fdEventEmptyCallback(void);

#endif
