#ifndef ALCHEMY_OWNER_STATE_H
#define ALCHEMY_OWNER_STATE_H

#include "types.h"

struct OwnerInventoryState {
    u8 unknown_000[0xd8];
    u16 inventory[15];
};

struct OwnerEquipmentEntry {
    u16 value;
    u16 unknown_02;
};

struct OwnerEquipmentState {
    u8 unknown_000[0x58];
    struct OwnerEquipmentEntry equipment[32];
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

struct OwnerValueState {
    u8 unknown_000[0x118];
    u8 values[4];
};

void *Func_08077394(s32 owner);
void *Func_0807882c(struct OwnerInventoryState *owner, s32 type);

#endif
