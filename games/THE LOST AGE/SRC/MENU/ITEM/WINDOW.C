#include "M7_INTERFACES.H"
#include "RAM_BUFFER.H"

extern u8 Menu_PlusSignString;
extern u8 Menu_MinusSignString;

void UiText_DrawNumberInWindowFar(s32, s32, s32, s32, s32);
void UiText_DrawStringInWindowFar(u8 *, s32, s32, s32);

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
    u8 unknown_00[0x24];
    struct RenderInput *item_window;
    u8 unknown_28[0xc8];
    struct RenderInput *message_window;
};

#define ITEM_MENU ((struct ItemMenuWindows *)Ram_HeapSlots->menu_runtime)

void ItemMenu_SetItemWin5(void)
{
    UiWindow_SetBounds(ITEM_MENU->item_window, 13, 5, 17, 10);
}

void ItemMenu_SetItemWin3(void)
{
    UiWindow_SetBounds(ITEM_MENU->item_window, 13, 3, 17, 10);
}

void ItemMenu_SetMsgWin5(void)
{
    UiWindow_SetBounds(ITEM_MENU->message_window, 13, 0, 17, 5);
}

void ItemMenu_SetMsgWin6(void)
{
    UiWindow_SetBounds(ITEM_MENU->message_window, 13, 0, 17, 6);
}

void ItemMenu_SetMsgWin7(void)
{
    UiWindow_SetBounds(ITEM_MENU->message_window, 13, 0, 17, 7);
}

void ItemMenu_SetMsgWin3(void)
{
    UiWindow_SetBounds(ITEM_MENU->message_window, 13, 0, 17, 3);
}
