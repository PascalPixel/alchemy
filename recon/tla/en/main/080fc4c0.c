#include "TYPES.H"
#include "FIXED_MATH.H"
extern u8 Data_03001f2c[];

/* menu/item_menu/page_result.c */
s32 Owner_GetStateFar(s32);
s32 ItemMenu_Count(s32 owner);

s32 ItemMenu_PageResult(struct MenuResult *result, s32 index)
{
    s32 encoded;
    s32 base = *(s32 *)((u32)&Data_03001f2c);
    s32 offset = index + 0x218;
    s32 entries = base + 2;
    s32 limit;
    s32 remainder;
    s32 quotient;
    s32 groups;
    s32 value;

    limit = ItemMenu_Count(LoadByte(entries, offset));
    encoded = Owner_GetStateFar(LoadByte(entries, offset));
    value = LoadSignedByte(base, LoadByte(entries, offset) + 0x260);
    if ((s32)(value + 1) > limit) {
        value = limit - 1;
    }
    quotient = __divsi3(value, 5);
    remainder = __modsi3(value, 5);
    groups = __divsi3(limit, 5);
    if (__modsi3(limit, 5) != 0) {
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
