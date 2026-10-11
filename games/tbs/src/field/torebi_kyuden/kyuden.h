#ifndef TOREBI_KYUDEN_KYUDEN_H
#define TOREBI_KYUDEN_KYUDEN_H

#include "keystate.h"
extern struct EventWork *gEventWork;

struct OwnerState;

union WorkCell
{
	s32 w;
	u16 h;
};

static __inline__ void SetHalf(u16 *at, s32 value)
{
	((union WorkCell *)at)->h = value;
}

extern s16 TorebiKyuden_EmptyTable[];
extern u32 TorebiKyuden_MessageTable[];

extern s32 TorebiKyuden_MiddleActionScript[];

enum
{
	ITEM_CLOAK_BALL = 205
};

#endif
