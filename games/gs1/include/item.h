#ifndef ALCHEMY_ITEM_H
#define ALCHEMY_ITEM_H

#include "layout_guard.h"

struct ItemDefinition {
    u8 unknown_00[2];
    u8 type;                        /* 0x02 */
    u8 flags;                       /* 0x03 */
    u16 properties;                 /* 0x04 */
    u8 unknown_06[6];
    u8 usability;                   /* 0x0c */
    u8 unknown_0d[0x1b];
    u16 action_id;                  /* 0x28 */
    u8 unknown_2a[2];
};

LAYOUT_SIZE_GUARD(ItemDefinition_Size, struct ItemDefinition, 0x2c);
LAYOUT_OFFSET_GUARD(
    ItemDefinition_Usability,
    struct ItemDefinition,
    usability,
    0x0c);
LAYOUT_OFFSET_GUARD(
    ItemDefinition_ActionId,
    struct ItemDefinition,
    action_id,
    0x28);

struct ItemDefinition *Func_08078414(s32 item);

#endif
