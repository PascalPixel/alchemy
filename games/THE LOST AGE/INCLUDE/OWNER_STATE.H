#ifndef ALCHEMY_OWNER_STATE_H
#define ALCHEMY_OWNER_STATE_H

#include "TYPES.H"

struct OwnerInventoryState {
    u8 unknown_000[0xd8];
    u16 inventory[15];
};

struct OwnerLearnedState {
    u8 unknown_000[0x108];
    u32 learned[4];
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
    u32 active[4];
    u8 owned_counts[4];
    u8 active_counts[4];
};

struct OwnerValueState {
    u8 unknown_000[0x118];
    u8 values[4];
};

void *OwnerState_Get(s32 owner);

#endif
