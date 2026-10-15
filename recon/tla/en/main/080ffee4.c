/* Draft, differ 2 of 237 (2026-10-15), ported from The Broken Seal's ItemMenu_DrawEquipPage:
 * the native loads MsgItemPlainName from the pool between the first two instructions of the
 * 0x1ff mask (movs r0, #128; ldr r3; lsls r0, r0, #1); here sched2 gives the mask's chain
 * (priority 5) the slot before the load (priority 4). The sum written either way round, as an
 * index, through a local or with an int cast measured the same; three minutes of alchemy permute
 * found nothing better. The plain twin is differ 6: the two loop loads of the item also come after
 * their mask, which the barriers below fix. */
#include "types.h"
#include "item.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"

struct InventoryMenuState {
    u8 unk000[0x30];
    s32 infoWindow;
    s32 equipWindow;
    u8 unk038[0x18c];
    u16 items[32];
};

struct MenuResult {
    s32 ownerState;
    s32 unused04;
    s32 page;
    s32 pageCount;
    s32 row;
    s32 entryCount;
    s32 selectedIndex;
};

#define PAGE_ROWS 5
#define EQUIP_SLOT_X 2
#define EQUIP_SLOT_WIDTH 9

void RenderOutput_RedrawSavedRectFar(s32 window);
void WaitFrames(s32 frames);
void UiText_DrawCharacterAtOffsetFar(u8 *message, s32 window, s32 x, s32 y);
void UiWindow_SetTilemapEntryFar(s32 window, s32 tile, s32 x, s32 y, s32 palette);
void Render_SetTilemapFlagRect(s32 window, s32 x, s32 y, s32 width, s32 height, s32 flag);

s32 ItemMenu_DrawEquipPage(s32 window, s32 unused, struct MenuResult *state)
{
    struct InventoryMenuState *menu;
    struct ItemDefinition *item;
    s32 base;
    s32 row;
    u32 entry;
    u32 mask;

    menu = Ram_HeapSlots->menuRuntime;
    base = state->page * PAGE_ROWS;
    state->selectedIndex = base + state->row;
    RenderOutput_RedrawSavedRectFar(menu->infoWindow);
    WaitFrames(1);
    if (menu->items[state->selectedIndex] != 0) {
        UiText_DrawCharacterAtOffsetFar(
            (menu->items[state->selectedIndex] & ITEM_ID_MASK) + MsgItemPlainName,
            menu->infoWindow, 0, 0);
    }

    for (row = 0; row < PAGE_ROWS; row++) {
        if (row == state->row) {
            entry = menu->items[state->selectedIndex];
            asm volatile("");
            mask = ITEM_ID_MASK;
            mask &= entry;
            item = Item_Get(mask);
            if (item->element != 4) {
                UiWindow_SetTilemapEntryFar(window, item->element + 1, 27, row * 2 + 1, 0);
                Render_SetTilemapFlagRect(window, 14, row * 2 + 1, 13, 1, 14);
            } else {
                Render_SetTilemapFlagRect(window, 14, row * 2 + 1, 14, 1, 14);
            }
        } else {
            entry = menu->items[base + row];
            asm volatile("");
            mask = ITEM_ID_MASK;
            mask &= entry;
            item = Item_Get(mask);
            if (item->element != 4) {
                UiWindow_SetTilemapEntryFar(window, item->element + 1, 27, row * 2 + 1, 4);
                Render_SetTilemapFlagRect(window, 14, row * 2 + 1, 13, 1, 15);
            } else {
                Render_SetTilemapFlagRect(window, 14, row * 2 + 1, 14, 1, 15);
            }
        }
    }

    for (row = 0; row <= 3; row++)
        Render_SetTilemapFlagRect(menu->equipWindow, EQUIP_SLOT_X, row * 2 + 1, EQUIP_SLOT_WIDTH, 1, 15);

    if (menu->items[state->selectedIndex] & INVENTORY_EQUIPPED) {
        item = Item_Get(menu->items[state->selectedIndex] & ITEM_ID_MASK);
        switch (item->type) {
        case 1:
            Render_SetTilemapFlagRect(menu->equipWindow, EQUIP_SLOT_X, 1, EQUIP_SLOT_WIDTH, 1, 14);
            break;
        case 4:
            Render_SetTilemapFlagRect(menu->equipWindow, EQUIP_SLOT_X, 3, EQUIP_SLOT_WIDTH, 1, 14);
            break;
        case 3:
            Render_SetTilemapFlagRect(menu->equipWindow, EQUIP_SLOT_X, 5, EQUIP_SLOT_WIDTH, 1, 14);
            break;
        case 2:
            Render_SetTilemapFlagRect(menu->equipWindow, EQUIP_SLOT_X, 7, EQUIP_SLOT_WIDTH, 1, 14);
            break;
        }
    }
    WaitFrames(1);
    return 1;
}
