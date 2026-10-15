/* Draft, differ 2 of 80: The Broken Seal's ItemMenu_DrawNamePage over The Lost Age's menu offsets.
 * Before the row loop the native loads the 0x1ff mask from the pool one row earlier (movs r2,
 * #226; ldr r7, =0x1ff; add r3, sl; lsls r2, r2, #1), this form after the shift. A for loop with
 * an index, with the pointer as a second induction variable or with the pointer stepped in the
 * body measured 45 to 63. MsgItemName is message 607 in English and is not named in the catalogs
 * yet. */
#include "types.h"
#include "ram_buffer.h"

struct MenuResult {
    s32 ownerState;
    s32 unk04;
    s32 page;
    s32 pageCount;
    s32 row;
    s32 entryCount;
    s32 selectedIndex;
};

struct InventoryMenuState {
    u8 unk000[0x24];
    s32 itemWindow;
    u8 unk028[8];
    s32 infoWindow;
    u8 unk034[0x190];
    u16 items[32];
};

#define PAGE_X  116
#define ENTRY_X 24

extern char MsgItemName[];

void UiWindow_DrawDividerLineFar(s32 window, s32 x, s32 width, s32 height, s32 style);
void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
void RenderOutput_RedrawSavedRectFar(s32 window);
void Menu_DrawPageIndicator(s32 window, s32 count, s32 page_size, s32 page, s32 style);
void Menu_SetPageIcons(s32 page_size, s32 first_entry, s32 window, s32 x, s32 y);

// どうぐ一覧の 1 ページ（5 行）の名前を描く
s32 ItemMenu_DrawNamePage(s32 window, s32 unused, const struct MenuResult *state)
{
    struct InventoryMenuState *menu = Ram_HeapSlots->menuRuntime;
    u32 page;
    u32 first_entry;
    u32 visible_count;
    u8 row;
    const u16 *item_id;

    RenderOutput_RedrawSavedRectFar(window);
    UiWindow_DrawDividerLineFar(window, 0, 11, 16, 11);

    page = state->page;
    first_entry = page * 5;
    visible_count = (u8)(state->entryCount - first_entry);
    if (visible_count > 5) {
        visible_count = 5;
    }

    Menu_SetPageIcons(5, first_entry, window, PAGE_X, 34);
    Menu_DrawPageIndicator(window, state->entryCount, 5, state->page, 15);

    row = 0;
    if (visible_count > row) {
        item_id = &menu->items[first_entry];
        do {
            UiText_DrawCharacterAtOffsetFar(
                (item_id[0] & 0x1ff) + (s32)MsgItemName,
                menu->itemWindow,
                ENTRY_X,
                row * 16 + 8);
            row++;
            item_id++;
        } while (visible_count > row);
    }

    return 1;
}
