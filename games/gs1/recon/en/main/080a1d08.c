#include "inventory_menu.h"

#define INPUT_NEW_KEYS (*(volatile u32 *)0x03001c94)

void Func_08015108(s32 message, s32 *, s32 *, s32 *, s32 *);
s32 UiWindow_UpdateOrCreate(s32 *, s32, s32, s32, s32, s32);
void WindowBounds_Set(s32, s32, s32, s32, s32);
void UiWindow_Commit(s32 window);
void Func_08015278(s32 window);
void UiText_DrawAt(s32 message, s32 window, s32 x, s32 y);
void UiText_DrawMessageAt(s32 message, s32 window, s32 x, s32 y);
void WaitFrames(s32 frames);
void Func_080770c8(s32 flag);
void UiWindow_CloseIfOpen(s32 *, s32);

#define InventoryMenu_ShowModalMessage Func_080a1d08

/* Show an inventory message and optionally wait for an acknowledgement. */
void InventoryMenu_ShowModalMessage(
    s32 message,
    s32 acknowledgement_mode,
    s32 window_mode)
{
    u8 *menu = *(u8 **)0x03001f2c;
    s32 window;
    s32 left;
    s32 top;
    s32 width;
    s32 height;

    (*(struct InventoryMenuIcon **)(menu + 0x14))->state = 13;
    if (window_mode != -1) {
        Func_08015108(message, &left, &top, &width, &height);
        if (UiWindow_UpdateOrCreate(
                (s32 *)(menu + 0x3c),
                acknowledgement_mode,
                window_mode,
                width,
                height,
                0x102) == 0) {
            WindowBounds_Set(
                *(s32 *)(menu + 0x3c),
                acknowledgement_mode,
                window_mode,
                width,
                height);
        }
        window = *(s32 *)(menu + 0x3c);
    } else {
        window = *(s32 *)(menu + 0x2c);
    }

    UiWindow_Commit(window);
    Func_08015278(window);
    if (window_mode == -1)
        UiText_DrawAt(message, window, 0, 0);
    else
        UiText_DrawMessageAt(message, window, 0, 0);

    if (acknowledgement_mode != -1) {
        WaitFrames(1);
        do {
            WaitFrames(1);
        } while ((INPUT_NEW_KEYS & (1 | 2 | 8)) == 0);

        if (window_mode == -1)
            UiWindow_Commit(window);
        Func_08015278(window);
    } else {
        Func_080770c8(0x151);
    }

    *(s16 *)(menu + 0x222) = 1;
    (*(struct InventoryMenuIcon **)(menu + 0x14))->state = 1;
    if (window_mode != -1)
        UiWindow_CloseIfOpen((s32 *)(menu + 0x3c), 1);
}
