/*
 * Draft: ItemMenu_DrawItemDetailPage, ported from its ☀️ twin (the
 * European path) with the menu work from its heap slot. Remaining
 * difference: scheduling at the start and around the item name, where the
 * listing issues its loads before the independent moves; the message is
 * still listed as the number 0x92 (☀️'s MsgItemPlainName).
 */
#include "TYPES.H"
#include "RAM_BUFFER.H"

extern u8 MsgItemPlainName;
void RenderOutput_RedrawSavedRectFar(s32 window);
s32 Render_SetTilemapFlagRect(s32, s32, s32, s32, s32, s32);
void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
void WaitFrames(s32);

/* ⚓️ keeps the detail window at 0x30 and the list window at 0x24 of the menu
   work, and the items at 0x1c4, each four bytes on from ☀️'s. */
s32 ItemMenu_DrawItemDetailPage(s32 arg0, s32 arg1, void *state)
{
    void *menu;
    s32 page;
    s32 combined;
    s32 off;
    s32 row;

    page = *(s32 *)(state + 8);
    menu = Ram_HeapSlots->menu_runtime;
    combined = page * 5;
    combined += *(s32 *)(state + 16);
    *(s32 *)(state + 24) = combined;

    RenderOutput_RedrawSavedRectFar(*(s32 *)(menu + 48));
    WaitFrames(1);

    combined = *(s32 *)(state + 24);
    off = combined * 2 + 452;
    if (*(u16 *)((char *)menu + off) != 0) {
        s32 masked = (*(u16 *)((char *)menu + off) & 0x1ff) + (s32)&MsgItemPlainName;
        UiText_DrawCharacterAtOffsetFar(masked, *(s32 *)(menu + 48), 0, 0);
    }

    row = 0;
    do {
        if (row == *(s32 *)(state + 16)) {
            Render_SetTilemapFlagRect(*(s32 *)(menu + 36), 1, row * 2 + 1, 14, 1, 14);
        } else {
            Render_SetTilemapFlagRect(*(s32 *)(menu + 36), 1, row * 2 + 1, 14, 1, 15);
        }
        row++;
    } while (row <= 4);

    WaitFrames(1);
    return 1;
}
