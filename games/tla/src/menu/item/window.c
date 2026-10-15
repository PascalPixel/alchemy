#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "m7_interfaces.h"
#include "ram_buffer.h"

#define UiText_DrawStringInWindowFar ((void (*)(u8*,s32,s32,s32))UiText_DrawStringInWindowFar)



void ItemMenu_DrawStat(
    s32 delta, s32 unused, s32 window, s32 x, s32 y)
{
    s32 digits;
    s32 magnitude;

    UiText_DrawNumberInWindowFar(delta, 3, window, x, y);
    digits = 1;
    magnitude = delta;
    if (delta < 0) {
        magnitude = -delta;
    }
    if (magnitude > 9) {
        digits = 2;
    }
    magnitude = delta;
    if (delta < 0) {
        magnitude = -delta;
    }
    if (magnitude > 99) {
        digits = 3;
    }
    if (delta > 0) {
        UiText_DrawStringInWindowFar(
            &Menu_PlusSignString, window, x - digits * 8 + 16, y);
    } else {
        UiText_DrawStringInWindowFar(
            &Menu_MinusSignString, window, x - digits * 8 + 16, y);
    }
}

struct ItemMenuWindows {
    u8 unk00[0x24];
    struct RenderInput *itemWindow;
    u8 unk28[0xc8];
    struct RenderInput *messageWindow;
};

#define ITEM_MENU ((struct ItemMenuWindows *)Ram_HeapSlots->menuRuntime)

void ItemMenu_SetItemWin5(void)
{
    UiWindow_SetBounds(ITEM_MENU->itemWindow, 13, 5, 17, 10);
}

void ItemMenu_SetItemWin3(void)
{
    UiWindow_SetBounds(ITEM_MENU->itemWindow, 13, 3, 17, 10);
}

void ItemMenu_SetMsgWin5(void)
{
    UiWindow_SetBounds(ITEM_MENU->messageWindow, 13, 0, 17, 5);
}

void ItemMenu_SetMsgWin6(void)
{
    UiWindow_SetBounds(ITEM_MENU->messageWindow, 13, 0, 17, 6);
}

void ItemMenu_SetMsgWin7(void)
{
    UiWindow_SetBounds(ITEM_MENU->messageWindow, 13, 0, 17, 7);
}

void ItemMenu_SetMsgWin3(void)
{
    UiWindow_SetBounds(ITEM_MENU->messageWindow, 13, 0, 17, 3);
}
