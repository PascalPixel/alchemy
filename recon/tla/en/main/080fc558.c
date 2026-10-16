/* Draft, differ 10 of 79: The Broken Seal's ItemMenu_DrawItemDetailPage over The Lost Age's menu
 * offsets. Three rows are scheduled the other way round, each around a split constant: the native
 * saves the state pointer before it starts 0x03000000 (mov r8, r2; movs r3, #192), loads the info
 * window before it opens the frame (ldr r0, [r7, #48]; ... sub sp, #8) and loads the item name
 * base before it finishes 0x1ff (ldr r3, =MsgItemPlainName; lsls r0, r0, #1). MsgItemPlainName is
 * message 146 in English and is not named in the catalogs yet. */
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

extern char MsgItemPlainName[];
void RenderOutput_RedrawSavedRectFar(s32 window);
s32 Render_SetTilemapFlagRect(s32 window, s32 x, s32 y, s32 width, s32 height, s32 flag);
void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
void WaitFrames(s32 frames);

// Draws the selected item-list row's name and highlights only that row.
s32 ItemMenu_DrawItemDetailPage(s32 arg0, void *arg1, struct MenuResult *state)
{
    struct InventoryMenuState *menu;
    s32 page;
    s32 combined;
    s32 row;

    page = state->page;
    menu = Ram_HeapSlots->menuRuntime;
    combined = page * 5;
    combined += state->row;
    state->selectedIndex = combined;

    RenderOutput_RedrawSavedRectFar(menu->infoWindow);
    WaitFrames(1);

    combined = state->selectedIndex;
    if (menu->items[combined] != 0) {
        s32 masked = (menu->items[combined] & 0x1ff) + (s32)MsgItemPlainName;

        UiText_DrawCharacterAtOffsetFar(masked, menu->infoWindow, 0, 0);
    }

    row = 0;
    do {
        if (row == state->row) {
            Render_SetTilemapFlagRect(menu->itemWindow, 1, row * 2 + 1, 14, 1, 14);
        } else {
            Render_SetTilemapFlagRect(menu->itemWindow, 1, row * 2 + 1, 14, 1, 15);
        }
        row++;
    } while (row <= 4);

    WaitFrames(1);
    return 1;
}
