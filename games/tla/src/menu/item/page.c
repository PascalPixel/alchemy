#include "types.h"
#include "ram_buffer.h"

// どうぐメニューの作業領域（わかっている欄だけ）
struct InventoryMenuState {
    u8 unknown_000[0x24];
    s32 item_window;
    u8 unknown_028[8];
    s32 info_window;
    u8 unknown_034[0x190];
    u16 items[32];
    u8 unknown_204[0x12];
    u8 pane_owner[2];
    u8 unknown_218[0x4c];
    s8 selected_index_by_owner[8];
    u8 unknown_26c[0x282];
    s16 resource_slots[2];
};

extern char ResourceId_MenuArrowTiles;
extern char ResourceId_MenuSmallDownArrowTiles;

s32 Resource_FindFreeEntry(void);
void VramBlock_LoadResourceFar(s32 slot, s32 size, s32 resource);

// ページ送りの矢印 2 枚を空いているタイルの枠に読む
void Resource_LoadPairedBlocks(void)
{
    struct InventoryMenuState *state = Ram_HeapSlots->menu_runtime;
    s32 value = Resource_FindFreeEntry();

    state->resource_slots[0] = value;
    VramBlock_LoadResourceFar(value, 128, (s32)&ResourceId_MenuArrowTiles);
    value = Resource_FindFreeEntry();
    state->resource_slots[1] = value;
    VramBlock_LoadResourceFar(value, 128, (s32)&ResourceId_MenuSmallDownArrowTiles);
}

struct MenuResult {
    s32 owner_state;
    s32 unknown_04;
    s32 page;
    s32 page_count;
    s32 row;
    s32 entry_count;
    s32 selected_index;
};

s32 UnitGet(s32 owner);
s32 Func_080fad1c(s32 owner);

// index 番目の欄の持ち主のどうぐを 5 行ずつのページに割る
s32 ItemMenu_PageResult(struct MenuResult *result, s32 index)
{
    s32 encoded;
    struct InventoryMenuState *menu = Ram_HeapSlots->menu_runtime;
    s32 limit;
    s32 remainder;
    s32 quotient;
    s32 groups;
    s32 value;

    limit = Func_080fad1c(menu->pane_owner[index]);
    encoded = UnitGet(menu->pane_owner[index]);
    value = menu->selected_index_by_owner[menu->pane_owner[index]];
    if ((value + 1) > limit) {
        value = limit - 1;
    }
    quotient = value / 5;
    remainder = value % 5;
    groups = limit / 5;
    if (limit % 5 != 0) {
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
