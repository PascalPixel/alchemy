#ifndef ALCHEMY_RESMENU_H
#define ALCHEMY_RESMENU_H

#include "TYPES.H"
#include "LAYOUT_GUARD.H"
#include "WINDOW.H"

/* One queued icon, followed by its position and tile-slot handle. */
struct ResourceMenuEntry {
    u32 link;
    u32 attr01;
    u32 attr2;
    s16 x;
    s16 y;
    u16 unknown_10;
    u16 slot;
};

/* AffineEffect_InitializeWork allocates this block. At most six icons
   precede the text window and the two transfer-help windows. */
struct ResourceMenuWork {
    struct ResourceMenuEntry entries[6];
    struct UiWindow *window;
    struct UiWindow *lower_window;
    struct UiWindow *upper_window;
    u8 resource_ids[8];
    s16 selection;
    s16 count;
    s16 width;
    s16 resource_base;
    s16 row;
    s16 previous_mode;
};

LAYOUT_SIZE_GUARD(ResourceMenuEntry_Size, struct ResourceMenuEntry, 0x14);
LAYOUT_OFFSET_GUARD(ResourceMenuEntry_Slot, struct ResourceMenuEntry, slot, 0x12);
LAYOUT_SIZE_GUARD(ResourceMenuWork_Size, struct ResourceMenuWork, 0x98);
LAYOUT_OFFSET_GUARD(ResourceMenuWork_Window, struct ResourceMenuWork, window, 0x78);
LAYOUT_OFFSET_GUARD(ResourceMenuWork_Selection, struct ResourceMenuWork, selection, 0x8c);
LAYOUT_OFFSET_GUARD(ResourceMenuWork_ResourceBase, struct ResourceMenuWork, resource_base, 0x92);
LAYOUT_OFFSET_GUARD(ResourceMenuWork_PreviousMode, struct ResourceMenuWork, previous_mode, 0x96);

extern struct ResourceMenuWork *gMenuSelectWork;

#endif
