#include "far_runtime.h"
#include "inventory_menu.h"

s32 UiText_DrawAt(s32, s32, s32, s32);

void InventoryMenu_DrawMessage(s32 unused, s32 message) {
    struct InventoryMenuState *menu;

    menu = Data_03001f2c;
    UiWindow_Commit(menu->message_window);
    UiText_DrawAt(message, menu->message_window, 0, 0);
}
