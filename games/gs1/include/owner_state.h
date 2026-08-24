#ifndef ALCHEMY_OWNER_STATE_H
#define ALCHEMY_OWNER_STATE_H

#include "layout_guard.h"

struct OwnerInventoryState {
    u8 unknown_000[0xd8];
    u16 inventory[15];
    u8 unknown_0f6[0x32];
    volatile u8 class_id;           /* 0x128 */
};

struct OwnerActionSlot {
    u16 encoded_action;
    u16 unknown_02;
};

struct OwnerActionState {
    u8 unknown_000[0x58];
    struct OwnerActionSlot action_slots[32];
};

struct OwnerLearnedState {
    u8 unknown_000[0x108];
    u32 learned[5];
};

struct OwnerBitState {
    u8 unknown_000[0xf8];
    u32 bits[8];
    u8 bit_counts[8];
};

struct OwnerTransferState {
    u8 unknown_000[0xf8];
    u32 available[4];
    u32 owned[4];
    u8 owned_counts[4];
};

struct OwnerDjinnState {
    u8 unknown_000[0xf8];
    u32 available[4];
    u32 active[5];
    u8 active_counts[5];
};

struct OwnerValueState {
    u8 unknown_000[0x118];
    u8 values[4];
};

LAYOUT_OFFSET_GUARD(
    OwnerInventoryState_Inventory,
    struct OwnerInventoryState,
    inventory,
    0x0d8);
LAYOUT_OFFSET_GUARD(
    OwnerInventoryState_ClassId,
    struct OwnerInventoryState,
    class_id,
    0x128);
LAYOUT_SIZE_GUARD(
    OwnerActionSlot_Size,
    struct OwnerActionSlot,
    0x04);
LAYOUT_OFFSET_GUARD(
    OwnerActionState_ActionSlots,
    struct OwnerActionState,
    action_slots,
    0x58);
LAYOUT_OFFSET_GUARD(
    OwnerDjinnState_Available,
    struct OwnerDjinnState,
    available,
    0x0f8);
LAYOUT_OFFSET_GUARD(
    OwnerDjinnState_Active,
    struct OwnerDjinnState,
    active,
    0x108);
LAYOUT_OFFSET_GUARD(
    OwnerDjinnState_ActiveCounts,
    struct OwnerDjinnState,
    active_counts,
    0x11c);

/*
 * Some exact callers deliberately leave the owner id in r0 instead of
 * spelling it as a C argument.  Keep the legacy declaration non-prototyped
 * so those compiler-shaped calls remain representable.
 */
void *Func_08077394();
struct OwnerInventoryState *Func_08077008(s32 owner);

#define OwnerState_Get Func_08077394
#define OwnerState_GetFar Func_08077008

#endif
