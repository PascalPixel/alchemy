#ifndef ALCHEMY_ITEM_H
#define ALCHEMY_ITEM_H

#include "types.h"

struct ItemDefinition {
    u8 unknown_00[2];
    u8 type;
    u8 flags;
    u16 properties;
    u8 unknown_06[0x26];
};

struct ItemDefinition *Func_08078414(s32 item);

#endif
