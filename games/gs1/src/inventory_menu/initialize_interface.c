#include "inventory_menu.h"

s32 UiMenu_CreateCursor(void *menu);
void InitializeEntryObjects(s32 source, s32 x, s32 y, s32 spacing, s32 style);
s32 UiWindow_CreateFar(s32 x, s32 y, s32 width, s32 height, s32 style);

void InventoryMenu_InitializeInterface(void)
{
    struct InventoryMenuState *menu = Data_03001f2c;
    s32 index;

    InitializeEntryObjects(UiMenu_CreateCursor(menu), 2, 2, 8, 0);
    for (index = 3; index >= 0; index--)
        *(u16 *)((u8 *)menu + 324 + index * 2) = 30;

    {
        s32 zero = 0;
        s32 style;
        *(s32 *)((u8 *)menu + 40) = zero;
        *(s32 *)((u8 *)menu + 36) = zero;
        style = 2;
        *(s32 *)((u8 *)menu + 44) =
            UiWindow_CreateFar(0, 17, 30, 3, style);
        *(s32 *)((u8 *)menu + 32) = zero;
        *(u8 *)((u8 *)menu + 272) = zero;
        *(u8 *)((u8 *)menu + 273) = zero;
        *(u8 *)((u8 *)menu + 274) = 8;
        *(u8 *)((u8 *)menu + 275) = style;
    }
}
