#ifndef ALCHEMY_RESOURCE_MENU_H
#define ALCHEMY_RESOURCE_MENU_H

#include "types.h"
#include "layout_guard.h"
#include "window.h"

struct ResourceMenuEntry
{
	u32 link;
	u32 attr01;
	u32 attr2;
	s16 x;
	s16 y;
	u16 unk10;
	u16 slot;
};

struct ResourceMenuWork
{
	struct ResourceMenuEntry entries[6];
	struct UiWindow			*window;
	struct UiWindow			*lowerWindow;
	struct UiWindow			*upperWindow;
	u8						 resourceIds[8];
	s16						 selection;
	s16						 count;
	s16						 width;
	s16						 resourceBase;
	s16						 row;
	s16						 previousMode;
};

LAYOUT_SIZE_GUARD(ResourceMenuEntry_Size, struct ResourceMenuEntry, 0x14);
LAYOUT_OFFSET_GUARD(ResourceMenuEntry_Slot, struct ResourceMenuEntry, slot, 0x12);
LAYOUT_SIZE_GUARD(ResourceMenuWork_Size, struct ResourceMenuWork, 0x98);
LAYOUT_OFFSET_GUARD(ResourceMenuWork_Window, struct ResourceMenuWork, window, 0x78);
LAYOUT_OFFSET_GUARD(ResourceMenuWork_Selection, struct ResourceMenuWork, selection, 0x8c);
LAYOUT_OFFSET_GUARD(ResourceMenuWork_ResourceBase, struct ResourceMenuWork, resourceBase, 0x92);
LAYOUT_OFFSET_GUARD(ResourceMenuWork_PreviousMode, struct ResourceMenuWork, previousMode, 0x96);

extern struct ResourceMenuWork *gMenuSelectWork;

#endif
