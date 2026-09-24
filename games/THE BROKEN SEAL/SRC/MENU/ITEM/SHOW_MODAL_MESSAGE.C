#include "TYPES.H"
#include "SYSTEM.H"

struct MenuIcon {
    u8 padding0[5];
    u8 state;
};

struct InventoryMenuWork {
    u8 padding000[0x14];
    struct MenuIcon *icon;
    u8 padding018[0x14];
    s32 message_window;
    u8 padding030[0x0c];
    s32 window;
    u8 padding040[0x1e2];
    u16 refresh;
};

void Func_08015108(s32 message, s32 *x, s32 *y, s32 *width, s32 *height);
s32 UiWindow_UpdateOrCreate(s32 *window, s32 x, s32 y, s32 width, s32 height, s32 style);
void UiWindow_SetBounds(s32 window, s32 x, s32 y, s32 width, s32 height);
void RenderOutput_RedrawSavedRectFar(s32 window);
void RenderOutput_ClearListFar(s32 window);
void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
void Func_08015078(s32 message, s32 window, s32 x, s32 y);
void GameFlag_SetBitFar(s32 flag);
void UiWindow_CloseIfOpen(s32 *window, s32 mode);

/* Show a message in the item menu: in the menu message window when y is -1,
   otherwise in a window sized to the message at (x, y). Unless x is -1, wait
   for A, B or Start and clear it; otherwise set flag 0x151. */
void InventoryMenu_ShowModalMessage(s32 message, s32 x, s32 y)
{
    struct InventoryMenuWork *menu = *(struct InventoryMenuWork **)0x03001f2c;
    s32 window;
    s32 height;
    s32 width;
    s32 top;
    s32 left;

    menu->icon->state = 13;
    if (y != -1) {
        Func_08015108(message, &left, &top, &width, &height);
        if (UiWindow_UpdateOrCreate(&menu->window, x, y, width, height, 0x102) == 0)
            UiWindow_SetBounds(menu->window, x, y, width, height);
        window = menu->window;
    } else {
        window = menu->message_window;
    }
    RenderOutput_RedrawSavedRectFar(window);
    RenderOutput_ClearListFar(window);
    if (y == -1)
        UiText_DrawCharacterAtOffsetFar(message, window, 0, 0);
    else
        Func_08015078(message, window, 0, 0);
    if (x != -1) {
        WaitFrames(1);
        do {
            WaitFrames(1);
        } while (!(*(volatile u32 *)0x03001c94 & 1) && !(*(volatile u32 *)0x03001c94 & 2) && !(*(volatile u32 *)0x03001c94 & 8));
        if (y == -1)
            RenderOutput_RedrawSavedRectFar(window);
        RenderOutput_ClearListFar(window);
    } else {
        GameFlag_SetBitFar(0x151);
    }
    menu->refresh = 1;
    menu->icon->state = 1;
    if (y != -1)
        UiWindow_CloseIfOpen(&menu->window, 1);
}
