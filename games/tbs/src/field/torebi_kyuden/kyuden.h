#ifndef TOREBI_KYUDEN_KYUDEN_H
#define TOREBI_KYUDEN_KYUDEN_H

#include "keystate.h"
extern struct EventWork *gEventWork;

struct OwnerState;

void			   Engine_EventOpenScreen(void);
void			   Engine_EventWaitForScreen(void);
void			   Engine_InventorySnapshotRestore(void);
void			   Engine_ActorSetDestination(s32 first, s32 second, s32 third);
void			   Engine_CameraFollowActor(s32 id, s32 flag);
void			   Engine_OwnerRecalculateRatios(s32 owner);
s32				   Engine_PartyListActiveOwners(s16 *owners);
void			   Engine_UiTextDrawQuantity(s32 first, s32 second);

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
