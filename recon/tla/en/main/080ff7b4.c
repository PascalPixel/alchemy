/* Near miss, differ 2 of 72: the native loads the menu (ldr r6, [r3]) before it starts the
 * offset 0x214 (movs r2, #133); here the offset starts first. Queue 10: a loop note after
 * the menu load gives that order, but the two parameter copies (mov r9, r0; adds r5, r1, #0)
 * then come before the offset where the native has them after it (differ 4); copying the
 * parameters into locals after the note leaves them there. */
#include "types.h"
#include "ram_buffer.h"

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

void *UnitGet(s32 owner);

s32 Menu_BuildPageResult(struct MenuPageResult *result, s32 index)
{
    struct MenuOwnerWork *menu = Ram_HeapSlots->menu_runtime;
    void *owner;
    s32 count;
    s32 value;
    s32 page;
    s32 row;
    s32 pages;

    owner = UnitGet(menu->owners[index]);
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
