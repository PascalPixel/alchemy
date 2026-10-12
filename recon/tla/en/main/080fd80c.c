/* Near miss, differ 2 of 69, one reordered pair: the native loads the menu before it starts the count's offset
 * 0x214 (ldr r6, [r3]; movs r2, #133); this form starts the constant first (sched2 gives movs priority 38, the
 * load 37). An empty asm or a one-pass block after the load pulls the two parameter copies before it (differ 4).
 * The permuter found nothing in five minutes (4,701 candidates). */
#include "types.h"
#include "ram_buffer.h"

// エナジーメニューの作業領域（わかっている欄だけ）
struct PsynergyMenuState {
    u8 unknown_000[0x24];
    s32 list_window;
    u8 unknown_028[8];
    s32 info_window;
    u8 unknown_034[0x190];
    u16 psynergies[32];
    u8 unknown_204[0x10];
    u8 psynergy_count;
    u8 unknown_215;
    u8 pane_owner[2];
    u8 unknown_218[0x4c];
    s8 selected_index_by_owner[8];
};

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

// index 番目の欄の持ち主のエナジーを 5 行ずつのページに割る
s32 PsynergyMenu_BuildPageResult(struct MenuResult *result, s32 index)
{
    s32 owner_state;
    struct PsynergyMenuState *menu = Ram_HeapSlots->menu_runtime;
    s32 entry_count;
    s32 row;
    s32 page;
    s32 page_count;
    s32 selected_index;
    s32 owner;

    owner_state = UnitGet(menu->pane_owner[index]);
    entry_count = menu->psynergy_count;
    owner = menu->pane_owner[index];
    selected_index = menu->selected_index_by_owner[owner];
    if ((selected_index + 1) > entry_count) {
        selected_index = entry_count - 1;
    }
    page = selected_index / 5;
    row = selected_index % 5;
    page_count = entry_count / 5;
    if (entry_count % 5 != 0) {
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
