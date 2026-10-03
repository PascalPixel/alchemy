#include "CHARACTER_MENU.H"
#include "EDITION.H"
#include "INVENTORY_MENU.H"
#include "OBJECT_FACTORY.H"
#include "TYPES.H"
#include "GLOBAL_CELLS.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "UI.H"

s32 UiMenu_CreateCursor(void *menu);
struct RenderOutput *RenderOutput_CreateFromResourceFar(
    s32 kind, s32 index, struct RenderInput *window, s32 x, s32 y);
/* The four-word void helper keeps the caller's extra legacy word. */
void PsynergyMenu_InitializeEntryObjects();

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
        menu->selected_column = zero;
        menu->selected_row = zero;
        menu->column_count = 8;
        menu->row_count = style;
    }
}

void Menu_SpawnIconEntries(struct InventoryMenuState *state, s32 arg1)
{
    struct RenderOutput **output0;
    s32 index0;
    s32 fifth0;
    struct RenderOutput **output1;
    s32 index1;
    s32 fifth1;
    struct RenderOutput **output2;
    s32 index2;
    s32 fifth2;

    index0 = 0;
    fifth0 = 0xA8;
    output0 = &state->entry_icons[0];
    do {
        *output0++ = RenderOutput_CreateFromResourceFar(2, index0, (struct RenderInput *)arg1, 0xF8, fifth0);
        index0++;
    } while (index0 <= 7);

    index1 = 8;
    fifth1 = 0xA8;
    output1 = &state->entry_icons[8];
    do {
        *output1++ = RenderOutput_CreateFromResourceFar(2, index1, (struct RenderInput *)arg1, 0x100, fifth1);
        index1++;
    } while (index1 <= 15);

    index2 = 16;
    fifth2 = 0xA8;
    output2 = &state->entry_icons[16];
    do {
        *output2++ = RenderOutput_CreateFromResourceFar(2, index2, (struct RenderInput *)arg1, 0x100, fifth2);
        index2++;
    } while (index2 <= 31);
}

void ItemMenu_HideAllIcons(void)
{
    s32 hidden_state = 13;
    struct RenderOutput **icons = gMenuWork->entry_icons;
    s32 slot;

    for (slot = 31; slot >= 0; slot--) {
        struct RenderOutput *icon = *icons++;
        if (icon != 0) {
            icon->active = hidden_state;
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
    struct RenderOutput **icon_slot = menu->entry_icons;

    do {
        struct RenderOutput *icon = *icon_slot++;

        if (icon != 0 && slot % 5 == 0) {
            icon->active = hidden_state;
        }
        slot++;
    } while (slot <= 31);
}
#endif

/* The pointer-cell closer ignores the extra legacy caller word. */
void UiWindow_CloseIfOpen();
void Menu_ReleaseEntryObjects(void);

void ItemMenu_Close(void)
{
    struct InventoryMenuState *menu;
    struct RenderOutput *cursor;

    menu = gMenuWork;
    Menu_ReleaseEntryObjects();
    ItemMenu_HideAllIcons();
    WaitFrames(1);
    cursor = menu->cursor;
    cursor->active = 0xD;
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
