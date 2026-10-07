#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "EDITION.H"
#include "TYPES.H"
#include "IO_REG.H"
#include "INVENTORY_MENU.H"
#include "M7_INTERFACES.H"
#include "SYSTEM.H"
#include "UI.H"

#define gKeyState (*(volatile u32 *)&gKeyState)
#define RenderOutput_ClearListFar ((void (*)(void *))RenderOutput_ClearListFar)

#define UiText_GetResourceDimensionsFar ((void (*)(s32,s32*,s32*,s32*,s32*))UiText_GetResourceDimensionsFar)

void RenderOutput_RedrawSavedRectFar(struct UiWindow *window);
void UiWindow_CloseIfOpen();

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
        UiText_GetResourceDimensionsFar(message, &left, &top, &width, &height);
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
        UiText_DrawResourceFar(message, window, 0, 0);
#else
    UiText_DrawResourceFar(message, window, 0, 0);
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
        UiWindow_CloseIfOpen(&menu->modal_window, 1);
#endif
}
