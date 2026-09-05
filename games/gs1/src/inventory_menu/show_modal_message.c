#include "inventory_menu.h"
#include "global_cells.h"

void Func_08015108(s32 message, s32 *, s32 *, s32 *, s32 *);
s32 UiWindow_UpdateOrCreate(s32 *, s32, s32, s32, s32, s32);
void WindowBounds_Set(s32, s32, s32, s32, s32);
void UiWindow_Commit(s32 window);
void Func_08015278(s32 window);
void UiText_DrawAt(s32 message, s32 window, s32 x, s32 y);
void UiText_DrawMessageAt(s32 message, s32 window, s32 x, s32 y);
void WaitFrames(s32 frames);
void GameFlag_Set(s32 flag);
void UiWindow_CloseIfOpen(s32 *, s32);

/* Show an inventory message and optionally wait for an acknowledgement. */
void Func_080a1d08(
    s32 message,
    s32 acknowledgement_mode,
    s32 window_mode)
{
    struct InventoryMenuState *menu;
    s32 window;
    s32 left;
    s32 top;
    s32 width;
    s32 height;

    menu = Data_03001f2c;
    (*(struct InventoryMenuIcon **)((u8 *)menu + 0x14))->state = 13;
    if (window_mode != -1) {
        Func_08015108(message, &left, &top, &width, &height);
        if (UiWindow_UpdateOrCreate(
                (s32 *)((u8 *)menu + 0x3c),
                acknowledgement_mode,
                window_mode,
                width,
                height,
                0x102) == 0) {
            WindowBounds_Set(
                *(s32 *)((u8 *)menu + 0x3c),
                acknowledgement_mode,
                window_mode,
                width,
                height);
        }
        window = *(s32 *)((u8 *)menu + 0x3c);
    } else {
        window = menu->info_window;
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
            if ((*(volatile u32 *)ADDR_03001C94 & 1) != 0)
                break;
            if ((*(volatile u32 *)ADDR_03001C94 & 2) != 0)
                break;
        } while ((*(volatile u32 *)ADDR_03001C94 & 8) == 0);

        if (window_mode == -1)
            UiWindow_Commit(window);
        Func_08015278(window);
    } else {
        GameFlag_Set(0x151);
    }

    menu->completion_flag = 1;
    (*(struct InventoryMenuIcon **)((u8 *)menu + 0x14))->state = 1;
    if (window_mode != -1)
        UiWindow_CloseIfOpen((s32 *)((u8 *)menu + 0x3c), 1);
}
