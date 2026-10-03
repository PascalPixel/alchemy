#include "EDITION.H"
#include "INVENTORY_MENU.H"
#include "OBJECT_FACTORY.H"
#include "TYPES.H"
#include "GLOBAL_CELLS.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "UI.H"

s32 UiMenu_CreateCursor(void *menu);
void PsynergyMenu_InitializeEntryObjects(s32 source, s32 x, s32 y, s32 spacing, s32 style);

void ItemMenu_Init(s32 x, s32 y, s32 mode, s32 columns)
{
    struct InventoryMenuState *menu = gMenuWork;
    s32 index;

    PsynergyMenu_InitializeEntryObjects(UiMenu_CreateCursor(menu), 2, 2, 8, 0);
    for (index = 3; index >= 0; index--)
        menu->owner_y[index] = 30;

    {
        s32 zero = 0;
        s32 style;
        menu->help_window = NULL;
        menu->status_window = NULL;
        style = 2;
        menu->info_window =
            (struct UiWindow *)UiWindow_CreateFar(0, 17, 30, 3, style);
        menu->item_window = NULL;
        menu->unknown_110[0] = zero;
        menu->unknown_110[1] = zero;
        menu->unknown_110[2] = 8;
        menu->unknown_110[3] = style;
    }
}

void Menu_SpawnIconEntries(struct InventoryMenuState *state, s32 arg1)
{
    struct InventoryMenuIcon **output0;
    s32 index0;
    s32 fifth0;
    struct InventoryMenuIcon **output1;
    s32 index1;
    s32 fifth1;
    struct InventoryMenuIcon **output2;
    s32 index2;
    s32 fifth2;

    index0 = 0;
    fifth0 = 0xA8;
    output0 = &state->entry_icons[0];
    do {
        *output0++ = (struct InventoryMenuIcon *)RenderOutput_CreateFromResourceFar(2, index0, arg1, 0xF8, fifth0);
        index0++;
    } while (index0 <= 7);

    index1 = 8;
    fifth1 = 0xA8;
    output1 = &state->entry_icons[8];
    do {
        *output1++ = (struct InventoryMenuIcon *)RenderOutput_CreateFromResourceFar(2, index1, arg1, 0x100, fifth1);
        index1++;
    } while (index1 <= 15);

    index2 = 16;
    fifth2 = 0xA8;
    output2 = &state->entry_icons[16];
    do {
        *output2++ = (struct InventoryMenuIcon *)RenderOutput_CreateFromResourceFar(2, index2, arg1, 0x100, fifth2);
        index2++;
    } while (index2 <= 31);
}

void ItemMenu_HideAllIcons(void)
{
    s32 hidden_state = 13;
    struct InventoryMenuIcon **icons = gMenuWork->entry_icons;
    s32 slot;

    for (slot = 31; slot >= 0; slot--) {
        struct InventoryMenuIcon *icon = *icons++;
        if (icon != 0) {
            icon->state = hidden_state;
        }
    }
}

#if EDITION_INTERNATIONAL
/* Only the localised item menus hide the first icon of each page; the
   Japanese edition has no such routine. */
void ItemMenu_HidePageIcons(void)
{
    struct InventoryMenuState *menu = gMenuWork;
    s32 slot = 0;
    s32 hidden_state = 13;
    struct InventoryMenuIcon **icon_slot = menu->entry_icons;

    do {
        struct InventoryMenuIcon *icon = *icon_slot++;

        if (icon != 0 && slot % 5 == 0) {
            icon->state = hidden_state;
        }
        slot++;
    } while (slot <= 31);
}
#endif

void UiWindow_CloseIfOpen(void *, s32);
void Menu_ReleaseEntryObjects(void);

void ItemMenu_Close(void)
{
    struct InventoryMenuState *menu;
    struct InventoryMenuIcon *cursor;

    menu = gMenuWork;
    Menu_ReleaseEntryObjects();
    ItemMenu_HideAllIcons();
    WaitFrames(1);
    cursor = menu->cursor;
    cursor->state = 0xD;
    UiWindow_CloseIfOpen(&menu->main_window, 1);
    UiWindow_CloseIfOpen(&menu->item_window, 1);
    UiWindow_CloseIfOpen(&menu->message_window, 1);
    UiWindow_CloseIfOpen(&menu->status_window, 1);
    UiWindow_CloseIfOpen(&menu->help_window, 1);
    UiWindow_CloseIfOpen(&menu->info_window, 1);
    UiWindow_CloseIfOpen(&menu->equip_window, 1);
    UiWindow_CloseIfOpen(&menu->list_window, 1);
    UiWindow_CloseIfOpen(&menu->unknown_window_38, 1);
    UiWindow_CloseIfOpen(&menu->modal_window, 1);
    UiWindow_CloseIfOpen(&menu->unknown_window_40, 1);
}
