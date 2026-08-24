#include "global_cells.h"
#include "inventory_menu.h"

void WaitFrames(s32);
void Func_080a1114(void *, s32);
void Menu_ReleaseEntryObjects(void);

void InventoryMenu_CloseWindows(void) {
    u8 *menu;
    s8 *cursor;

    menu = *(u8 **)ADDR_03001F2C;
    Menu_ReleaseEntryObjects();
    InventoryMenu_HideAllItemIcons();
    WaitFrames(1);
    cursor = *(s8 **)(menu + 0x17C);
    cursor[5] = 0xD;
    Func_080a1114(menu + 0x10, 1);
    Func_080a1114(menu + 0x20, 1);
    Func_080a1114(menu + 0x10C, 1);
    Func_080a1114(menu + 0x24, 1);
    Func_080a1114(menu + 0x28, 1);
    Func_080a1114(menu + 0x2C, 1);
    Func_080a1114(menu + 0x30, 1);
    Func_080a1114(menu + 0x34, 1);
    Func_080a1114(menu + 0x38, 1);
    Func_080a1114(menu + 0x3C, 1);
    Func_080a1114(menu + 0x40, 1);
}
