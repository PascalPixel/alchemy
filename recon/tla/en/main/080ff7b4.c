#include "TYPES.H"
#include "RAM_BUFFER.H"

struct MenuOwnerWork {
    u8 unknown_000[0x214];
    u8 count;
    u8 unknown_215;
    u8 owners[0x264 - 0x216];
    s8 selected[8];
};

struct MenuPageResult {
    void *owner;
    s32 unknown_04;
    s32 page;
    s32 page_count;
    s32 row;
    s32 count;
    s32 selected;
};

void *Owner_GetState(s32 owner);

s32 Menu_BuildPageResult(struct MenuPageResult *result, s32 index)
{
    struct MenuOwnerWork *menu = Ram_HeapSlots->menu_runtime;
    void *owner;
    s32 count;
    s32 value;
    s32 page;
    s32 row;
    s32 pages;

    owner = Owner_GetState(menu->owners[index]);
    count = menu->count;
    value = menu->selected[menu->owners[index]];

    if (value + 1 > count)
        value = count - 1;
    if (count == 0)
        value = 0;
    page = value / 5;
    row = value % 5;
    pages = count / 5;
    if (count % 5 != 0)
        pages++;
    result->owner = owner;
    result->page = page;
    result->page_count = pages;
    result->row = row;
    result->count = count;
    result->selected = value;
    return 1;
}
