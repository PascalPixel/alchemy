#include "scene.h"
#include "abi/item_menu/close.h"
#include "global_cells.h"
#include "item_menu.h"

void WaitFrames(s32);

void Menu_ReleaseEntryObjects(void);

void ItemMenu_Close(void)
{
    u8 *menu;
    s8 *cursor;

    menu = *(u8 **)ADDR_03001F2C;
    Menu_ReleaseEntryObjects();
    ItemMenu_HideAllIcons();
    WaitFrames(1);
    cursor = *(s8 **)(menu + 0x17C);
    cursor[5] = 0xD;
    Sys_Apply(menu + 0x10, 1);
    Sys_Apply(menu + 0x20, 1);
    Sys_Apply(menu + 0x10C, 1);
    Sys_Apply(menu + 0x24, 1);
    Sys_Apply(menu + 0x28, 1);
    Sys_Apply(menu + 0x2C, 1);
    Sys_Apply(menu + 0x30, 1);
    Sys_Apply(menu + 0x34, 1);
    Sys_Apply(menu + 0x38, 1);
    Sys_Apply(menu + 0x3C, 1);
    Sys_Apply(menu + 0x40, 1);
}
