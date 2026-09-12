#ifndef ALCHEMY_INN_RUNTIME_H
#define ALCHEMY_INN_RUNTIME_H

#include "layout_guard.h"
#include "types.h"

struct InnRuntimeState {
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
    InnRuntimeState_Window, struct InnRuntimeState, window, 0x0c);
LAYOUT_OFFSET_GUARD(
    InnRuntimeState_ResourceEntries,
    struct InnRuntimeState,
    resource_entries,
    0x390);
LAYOUT_OFFSET_GUARD(
    InnRuntimeState_ResourceId,
    struct InnRuntimeState,
    resource_id,
    0x3a4);
LAYOUT_OFFSET_GUARD(
    InnRuntimeState_Active, struct InnRuntimeState, active, 0x3a9);
LAYOUT_OFFSET_GUARD(
    InnRuntimeState_SpecialActive,
    struct InnRuntimeState,
    special_active,
    0x3ac);

extern struct InnRuntimeState *Data_03001f2c;

#endif
