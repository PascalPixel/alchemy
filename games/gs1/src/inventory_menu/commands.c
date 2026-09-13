#include "types.h"
#include "inventory_menu.h"
#include "item.h"

#define COMMAND_DISABLED (-1)
#define COMMAND_AVAILABLE 1

s32 Func_080a46b4(s32 owner, s32 item);
s32 Func_0808a490(s32 item);

void ItemMenu_BuildCmd(s8 *command_states)
{
    struct InventoryMenuState *menu;
    struct ItemDefinition *item;

    menu = Data_03001f2c;
    item = Item_Get(0x1ff & menu->selected_item);

    if (item->type == 0) {
        command_states[0] = COMMAND_AVAILABLE;
        command_states[1] = COMMAND_DISABLED;
    } else {
        command_states[0] = COMMAND_DISABLED;
        command_states[1] = COMMAND_AVAILABLE;
    }

    if (Func_080a46b4(menu->item_owner, menu->selected_item) != -1)
        command_states[0] = COMMAND_AVAILABLE;
    else
        command_states[0] = COMMAND_DISABLED;

    if (menu->selected_item & 0x400)
        command_states[0] = COMMAND_DISABLED;

    if (Item_CanOwnerEquip(
            menu->item_owner,
            menu->selected_item & 0x1ff) == 0) {
        command_states[1] = COMMAND_DISABLED;
    }

    command_states[3] = COMMAND_AVAILABLE;
    command_states[5] = COMMAND_AVAILABLE;
    command_states[2] = COMMAND_AVAILABLE;

    if (menu->selected_item & 0x200) {
        command_states[4] = COMMAND_AVAILABLE;
        command_states[1] = COMMAND_DISABLED;
    } else {
        command_states[4] = COMMAND_DISABLED;
    }

    if (item->flags & 2) {
        command_states[4] = COMMAND_DISABLED;
        if (menu->selected_item & 0x200) {
            command_states[3] = COMMAND_DISABLED;
            command_states[5] = COMMAND_DISABLED;
        }
    }

    if (Func_0808a490(menu->selected_item & 0x1ff) != 0)
        command_states[0] = COMMAND_AVAILABLE;

    if (menu->party_count <= 1)
        command_states[3] = COMMAND_DISABLED;

    if (item->flags & 8)
        command_states[5] = COMMAND_DISABLED;
}
#include "gs1_edition.h"

#if defined(GS1_EDITION_JA)
#define ITEM_TEXT_X 0x28
#else
#define ITEM_TEXT_X 0x20
#endif

#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void UiText_DrawAt(s32, s32, s32, s32);
extern char Value_00000b33;
#define InventoryMenu_CommandUseMessage Value_00000b33

void ItemMenu_DrawCmd(void *command_states, s32 window)
{
    s32 disabled;
    s32 value;
    u32 message;

    UiPalette_SetColor(0xf);
    value = FIELD(command_states, s8 *, 0);
    disabled = -1;
    if (value == disabled)
        UiPalette_SetColor(0xe);

    message = (u32)&InventoryMenu_CommandUseMessage;
    UiText_DrawAt(message, window, 0, 0x18);
    UiPalette_SetColor(0xf);
    if (FIELD(command_states, s8 *, 1) == disabled)
        UiPalette_SetColor(0xe);

    UiText_DrawAt(message + 1, window, ITEM_TEXT_X, 0x18);
    UiPalette_SetColor(0xf);
    if (FIELD(command_states, s8 *, 3) == disabled)
        UiPalette_SetColor(0xe);

    UiText_DrawAt(message + 2, window, 0, 0x20);
    UiPalette_SetColor(0xf);
    if (FIELD(command_states, s8 *, 5) == disabled)
        UiPalette_SetColor(0xe);

    UiText_DrawAt(message + 3, window, 0x50, 0x20);
    UiPalette_SetColor(0xf);
    if (FIELD(command_states, s8 *, 2) == disabled)
        UiPalette_SetColor(0xe);

    UiText_DrawAt(message + 4, window, 0x50, 0x18);
    UiPalette_SetColor(0xf);
    if (FIELD(command_states, s8 *, 4) == disabled)
        UiPalette_SetColor(0xe);

    UiText_DrawAt(message + 5, window, ITEM_TEXT_X, 0x20);
    UiPalette_SetColor(0xf);
}
