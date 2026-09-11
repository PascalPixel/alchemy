#ifndef ALCHEMY_INN_H
#define ALCHEMY_INN_H

#include "layout_guard.h"
#include "types.h"

struct InnState {
    u8 padding_000[0x0c];
    s32 window;
    u8 padding_010[0x380];
    u16 resource_entries[6];
    u8 padding_39c[8];
    u16 resource_id;
    u8 padding_3a6[3];
    u8 active;
    u8 padding_3aa[2];
    u8 special_active;
};

LAYOUT_OFFSET_GUARD(
    InnState_Window, struct InnState, window, 0x0c);
LAYOUT_OFFSET_GUARD(
    InnState_ResourceEntries,
    struct InnState,
    resource_entries,
    0x390);
LAYOUT_OFFSET_GUARD(
    InnState_ResourceId,
    struct InnState,
    resource_id,
    0x3a4);
LAYOUT_OFFSET_GUARD(
    InnState_Active, struct InnState, active, 0x3a9);
LAYOUT_OFFSET_GUARD(
    InnState_SpecialActive,
    struct InnState,
    special_active,
    0x3ac);

#endif
