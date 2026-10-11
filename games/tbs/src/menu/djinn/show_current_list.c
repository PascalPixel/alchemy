#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "io_reg.h"
#include "system.h"
#include "ui.h"
#include "djinn_menu.h"
#include "btlsys.h"

#include "tbs_edition.h"
#include "text/msg_ids.h"

#define gWindowWork (*(struct UiWork * *)&gWindowWork)
#define gKeyState (*(volatile u32 *)&gKeyState)
#define RenderOutput_RedrawSavedRectFar ((void (*)(s32))RenderOutput_RedrawSavedRectFar)
#define RenderOutput_ClearListFar ((void (*)(s32))RenderOutput_ClearListFar)


void AudioCommand_PlayFar(s32 cue);
void UiWindow_DrawDividerLineFar(s32 window, u32 x1, u32 y1, u32 x2, u32 y2);
void UiWindow_SetTilemapEntryFar(s32 window, s32 tile, s32 x, s32 y, u32 mode);

s32 DjinnMenu_ShowCurrentList(void)
{
    struct DjinnMenuWork *menu;
    s32 window;
    s32 row;
    s32 col;

    menu = gMenuWork;
    ItemMenu_ResetCategory();
    AudioCommand_PlayFar(112);
    RenderOutput_RedrawSavedRectFar(menu->help_window);
    UiText_DrawCharacterAtOffsetFar((s32)MsgReturnHelp, menu->help_window, 0, 16);
    menu->cursor->active = 13;
    menu->second_cursor->active = 13;
    WaitFrames(1);
    window = menu->window;
    {
        s32 i;

        for (i = 0; i < 4; i++) {
            menu->slot_x[i] = 32 + i * 56;
            menu->slot_y[i] = 70;
            menu->owner_y[i] = 30;
        }
    }
    RenderOutput_RedrawSavedRectFar(window);
    UiWindow_DrawDividerLineFar(window, 0, 11, 28, 11);
    UiText_DrawResourceFar((s32)MsgCurrentDjinn, menu->help_window, -96, 132);
    for (row = 0; row < 4; row++) {
        for (col = 0; col < 7; col++) {
            s32 id = row * 20 + col;

            if (GameFlag_TestFar(48 + id)) {
                UiWindow_SetTilemapEntryFar(window, 0x1001 + row,
                    row * 7 + 1, col + 3, 0);
                UiText_DrawCharacterAtOffsetFar((s32)MsgDjinnName + id,
                    window, row * 56 + 16, col * 8 + 24);
            }
        }
    }
    gWindowWork->dirty = 1;
    for (;;) {
        if (GameFlag_TestFar(0x150))
            break;
        WaitFrames(1);
        if (gKeyState & (KEY_A | KEY_B | KEY_SELECT))
            break;
    }
    RenderOutput_RedrawSavedRectFar(menu->window);
    RenderOutput_ClearListFar(menu->help_window);
    {
        s32 i;

        for (i = 0; i < 4; i++) {
            menu->slot_x[i] = 130 + i * 32;
            menu->slot_y[i] = 128;
        }
    }
    menu->cursor->active = 1;
    menu->second_cursor->active = 1;
    AudioCommand_PlayFar(113);
    /* Returns no value: the ROM has the s32 epilogue (pop {r1}; bx r1) after this void call. */
}
