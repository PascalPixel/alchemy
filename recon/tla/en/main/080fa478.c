#include "TYPES.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
extern u8 Data_03001f2c[];

s32 UiMenu_CreateCursor(void *menu);
void PsynergyMenu_InitializeEntryObjects(s32 source, s32 x, s32 y, s32 spacing, s32 style);

void ItemMenu_Close(void)
{
    u8 *menu;
    s8 *cursor;

    menu = *(u8 **)((u32)&Data_03001f2c);
    Menu_ReleaseEntryObjects();
    ItemMenu_HideAllIcons();
    WaitFrames(1);
    cursor = *(s8 **)(menu + 0x17C);
    cursor[5] = 0xD;
    UiWindow_CloseIfOpen(menu + 0x10, 1);
    UiWindow_CloseIfOpen(menu + 0x20, 1);
    UiWindow_CloseIfOpen(menu + 0x10C, 1);
    UiWindow_CloseIfOpen(menu + 0x24, 1);
    UiWindow_CloseIfOpen(menu + 0x28, 1);
    UiWindow_CloseIfOpen(menu + 0x2C, 1);
    UiWindow_CloseIfOpen(menu + 0x30, 1);
    UiWindow_CloseIfOpen(menu + 0x34, 1);
    UiWindow_CloseIfOpen(menu + 0x38, 1);
    UiWindow_CloseIfOpen(menu + 0x3C, 1);
    UiWindow_CloseIfOpen(menu + 0x40, 1);
}
