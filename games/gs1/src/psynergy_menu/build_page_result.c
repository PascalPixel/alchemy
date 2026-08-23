#include "psynergy_menu.h"
#include "global_cells.h"

s32 Func_08077008(s32);
s32 Func_080022ec(s32, s32);
s32 Func_080022fc(s32, s32);

s32 PsynergyMenu_BuildPageResult(struct MenuResult *result, s32 index)
{
    s32 owner_state;
    u8 *base = *(u8 **)ADDR_03001F2C;
    s32 offset = index + 0x218;
    u8 *owners = base + 2;
    s32 entry_count;
    s32 row;
    s32 page;
    s32 page_count;
    s32 selected_index;
    s32 owner_slot;

    owner_state = Func_08077008(owners[offset]);
    entry_count = base[0x218];
    owner_slot = owners[offset] + 0x260;
    selected_index = ((s8 *)base)[owner_slot];
    if ((s32)(selected_index + 1) > entry_count) {
        selected_index = entry_count - 1;
    }
    page = Func_080022ec(selected_index, 5);
    row = Func_080022fc(selected_index, 5);
    page_count = Func_080022ec(entry_count, 5);
    if (Func_080022fc(entry_count, 5) != 0) {
        page_count++;
    }
    result->owner_state = owner_state;
    result->page = page;
    result->page_count = page_count;
    result->row = row;
    result->entry_count = entry_count;
    result->selected_index = selected_index;
    return 1;
}
