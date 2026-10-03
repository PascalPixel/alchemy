#include "EDITION.H"
#include "TYPES.H"
#include "IO_REG.H"
#include "INVENTORY_MENU.H"
#include "M7_INTERFACES.H"
#include "SYSTEM.H"

extern volatile u32 gKeyState;

void Func_08015108(s32 message, s32 *x, s32 *y, s32 *width, s32 *height);
s32 UiWindow_UpdateOrCreate(s32 *window, s32 x, s32 y, s32 width, s32 height, s32 style);
void RenderOutput_RedrawSavedRectFar(struct UiWindow *window);
void RenderOutput_ClearListFar(void *window);
void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
void Func_08015078(s32 message, s32 window, s32 x, s32 y);
void GameFlag_SetBitFar(s32 flag);
/* The pointer-cell closer ignores the extra legacy caller word. */
void UiWindow_CloseIfOpen();

/* Show a message in the item menu: in the menu message window when y is -1,
   otherwise in a window sized to the message at (x, y). Unless x is -1, wait
   for A, B or Start and clear it; otherwise set flag 0x151. */
void InventoryMenu_ShowModalMessage(s32 message, s32 x, s32 y)
{
    struct InventoryMenuState *menu = gMenuWork;
    s32 window;
    s32 height;
    s32 width;
    s32 top;
    s32 left;

    menu->pane_icons[0]->active = 13;
    if (y != -1) {
        Func_08015108(message, &left, &top, &width, &height);
        if (UiWindow_UpdateOrCreate((s32 *)&menu->modal_window, x, y, width, height, 0x102) == 0)
            UiWindow_SetBounds((struct RenderInput *)menu->modal_window, x, y, width, height);
        window = (s32)menu->modal_window;
    } else {
        window = (s32)menu->info_window;
    }
    RenderOutput_RedrawSavedRectFar((struct UiWindow *)window);
    RenderOutput_ClearListFar((void *)window);
#if EDITION_INTERNATIONAL
    if (y == -1)
        UiText_DrawCharacterAtOffsetFar(message, window, 0, 0);
    else
        Func_08015078(message, window, 0, 0);
#else
    /* The Japanese menu draws every message the same way and leaves the
       sized window open. */
    Func_08015078(message, window, 0, 0);
#endif
    if (x != -1) {
        WaitFrames(1);
        do {
            WaitFrames(1);
        } while (!(gKeyState & KEY_A) && !(gKeyState & KEY_B) && !(gKeyState & KEY_START));
#if EDITION_INTERNATIONAL
        if (y == -1)
            RenderOutput_RedrawSavedRectFar((struct UiWindow *)window);
#endif
        RenderOutput_ClearListFar((void *)window);
    } else {
        GameFlag_SetBitFar(0x151);
    }
    menu->completion_flag = 1;
    menu->pane_icons[0]->active = 1;
#if EDITION_INTERNATIONAL
    if (y != -1)
        UiWindow_CloseIfOpen((s32 *)&menu->modal_window, 1);
#endif
}
