#ifndef ALCHEMY_INN_RUNTIME_H
#define ALCHEMY_INN_RUNTIME_H

#include "global_cells.h"
#include "layout_guard.h"
#include "types.h"

struct InnRuntimeState
{
	u8	padding000[0x0c];
	s32 window;
	u8	padding010[0x380];
	u16 resourceEntries[6];
	u8	padding39c[8];
	u16 resourceId;
	u8	padding3a6[3];
	u8	active;
	u8	padding3aa[2];
	u8	specialActive;
};

LAYOUT_OFFSET_GUARD(InnRuntimeState_Window, struct InnRuntimeState, window, 0x0c);
LAYOUT_OFFSET_GUARD(
	InnRuntimeState_ResourceEntries, struct InnRuntimeState, resourceEntries, 0x390);
LAYOUT_OFFSET_GUARD(InnRuntimeState_ResourceId, struct InnRuntimeState, resourceId, 0x3a4);
LAYOUT_OFFSET_GUARD(InnRuntimeState_Active, struct InnRuntimeState, active, 0x3a9);
LAYOUT_OFFSET_GUARD(InnRuntimeState_SpecialActive, struct InnRuntimeState, specialActive, 0x3ac);

#endif
