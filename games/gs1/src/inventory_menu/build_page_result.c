#include "inventory_menu.h"
#include "global_cells.h"

s32 Func_08077008(s32);
s32 Func_080022ec(s32, s32);
s32 Func_080022fc(s32, s32);

static __inline__ u8 LoadByte(s32 base, s32 offset)
{
    return *(u8 *)(base + offset);
}

static __inline__ s8 LoadSignedByte(s32 base, s32 offset)
{
    return *(s8 *)(base + offset);
}

s32 InventoryMenu_BuildPageResult(struct MenuResult *result, s32 index)
{
    s32 encoded;
    s32 base = *(s32 *)ADDR_03001F2C;
    s32 offset = index + 0x218;
    s32 entries = base + 2;
    s32 limit;
    s32 remainder;
    s32 quotient;
    s32 groups;
    s32 value;

    limit = InventoryMenu_CountItems(LoadByte(entries, offset));
    encoded = Func_08077008(LoadByte(entries, offset));
    value = LoadSignedByte(base, LoadByte(entries, offset) + 0x260);
    if ((s32)(value + 1) > limit) {
        value = limit - 1;
    }
    quotient = Func_080022ec(value, 5);
    remainder = Func_080022fc(value, 5);
    groups = Func_080022ec(limit, 5);
    if (Func_080022fc(limit, 5) != 0) {
        groups++;
    }
    result->owner_state = encoded;
    result->page = quotient;
    result->page_count = groups;
    result->row = remainder;
    result->entry_count = limit;
    result->selected_index = value;
    return 1;
}
