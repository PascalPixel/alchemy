#include "far_runtime.h"
#include "inventory_menu.h"

s32 Func_08015080(s32, s32, s32, s32);

void InventoryMenu_DrawMessage(s32 unused, s32 message) {
    struct InventoryMenuState *menu;

    menu = Data_03001f2c;
    Func_08015270(menu->message_window);
    Func_08015080(message, menu->message_window, 0, 0);
}
