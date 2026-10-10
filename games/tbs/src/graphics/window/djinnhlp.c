#include "battle.h"
#include "keystate.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "resource.h"
#include "types.h"
#include "io_reg.h"
#include "window.h"
#include "callback_scheduler.h"
#include "tbs_edition.h"
#include "system.h"
#include "ui.h"
#include "djinn_menu.h"
#include "heap_state.h"
#include "text/msg_ids.h"
void RenderOutput_PrepareForRedrawFar(s32 window);

#define RenderOutput_ClearListFar ((void (*)(s32))RenderOutput_ClearListFar)


void AudioCommand_PlayFar(s32 cue);
void UiWork_FinalizeFar(s32 window, s32 mode);

s32 Menu_DrawAtWindowOffset(void *win, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5)
{
    UiWindow_SetPaletteBitRectFar(((struct UiWindow *)win)->x + arg1 + 1, ((struct UiWindow *)win)->y + arg2 + 1, arg3, arg4, arg5);
}

void UiWindow_SetRectPalette(s32 x, s32 y, s32 width, s32 height, s32 palette)
{
    u8 *base;
    u16 *p;
    s32 col;
    u32 tile;
    u32 masked;

    base = gWindowWork[0];
    palette <<= 12;
    if (x < 0) {
        width += x;
        x = 0;
    }
    if (x + width > 29)
        width = 30 - x;
    if (y < 0) {
        height += y;
        y = 0;
    }
    if (y + height > 29)
        height = 20 - y;

    if (width > 0 && height > 0) {
        do {
            p = &((u16 (*)[32])base)[y][x];
            for (col = width; col != 0; col--) {
                tile = *p;
                if (((tile >> 12) & 15) == 15) {
                    masked = tile & 0xffff0fff;
                    *p = masked | palette;
                }
                p++;
            }
            ((struct UiRenderWork *)base)->dirty |= 2 << ((u32)y >> 2);
            /* FAKEMATCH: needed to match: a do-while(0) around the row count decrement keeps the reference register order */
            do {
                height--;
            } while (0);
            y++;
        } while (height != 0);
    }
}

void UiWindow_ApplyRectAtObjectOrigin(void *obj, s32 x, s32 y, s32 width, s32 height, s32 palette)
{
    UiWindow_SetRectPalette(((struct UiWindow *)obj)->x + x + 1,
        ((struct UiWindow *)obj)->y + y + 1, width, height, palette);
}

s32 DjinnMenu_ShowHelp(void)
{
    s32 win_b;
    s32 win_a;
    struct DjinnMenuWork *menu;
    void **slot_cells;
    struct UiRenderWork *work;
    s32 result;
    s32 previous;
    s32 selection;
    s32 list;
    struct UiChannelSlot *channel;
    s32 message;
    s32 cnt;
    s32 list_message;

    /* FAKEMATCH: needed to match: the menu/render heap-cell slice avoids four bytes and changed literal order from separate named cells. */
    slot_cells = (void **)&gMenuWork;
    menu = slot_cells[0];
    work = slot_cells[HEAP_SLOT_WINDOW - HEAP_SLOT_MENU];
    result = 0;
    previous = 0;
    selection = 0;
    RenderOutput_ClearListFar(menu->list_window);
    WaitFrames(1);
    RenderOutput_PrepareForRedrawFar(menu->help_window);
    message = (s32)MsgDjinnInfoPrompt;
    UiText_DrawCharacterAtOffsetFar(message, menu->help_window, 0, 0);
    message++;
    UiText_DrawCharacterAtOffsetFar(message, menu->help_window, 0, 16);
    UiWindow_SetRectPalette(1, 1, 11, 3, 6);
    UiWindow_ApplyRectAtObjectOrigin((void *)menu->list_window, 0, 0, 28, 10, 6);
    list = UiWindow_CreateFar(0, 9, 8, 10, 6);
    win_b = UiWindow_CreateFar(8, 12, 22, 7, 2);
    win_a = UiWindow_CreateFar(8, 9, 22, 3, 2);
    UiWindow_MarkVisibleTileAttributesFar();
    cnt = 0;
    list_message = (s32)MsgDjinnInfoTopic;
    do {
        UiText_DrawCharacterAtOffsetFar(cnt + list_message, list, 0, cnt * 8);
        cnt++;
    } while (cnt <= 6);
    do {
        RenderOutput_PrepareForRedrawFar(win_a);
        UiText_DrawResourceFar(selection + (s32)MsgDjinnInfoTopic, win_a, 0, 0);
        channel = (struct UiChannelSlot *)UiText_OpenEntryMessageFar(win_b, selection + (s32)MsgDjinnInfoText);
        Menu_DrawAtWindowOffset((void *)list, 0, previous, 6, 1, 15);
        Menu_DrawAtWindowOffset((void *)list, 0, selection, 6, 1, 14);
        previous = selection;
        for (;;) {
            UiMenu_PositionCursor(-12, (((struct UiWindow *)list)->y + selection) * 8 + 8);
            WaitFrames(1);
            if (gKeysRepeat & (KEY_RIGHT | KEY_DOWN)) {
                selection++;
                selection = Menu_GetModuloOfSum(selection, 7);
                AudioCommand_PlayFar(111);
                break;
            } else if (gKeysRepeat & (KEY_LEFT | KEY_UP)) {
                selection--;
                selection = Menu_GetModuloOfSum(selection, 7);
                AudioCommand_PlayFar(111);
                break;
            } else if (gKeyState & KEY_START) {
                AudioCommand_PlayFar(113);
                result = -2;
                break;
            } else if (gKeyState & (KEY_B | KEY_SELECT)) {
                AudioCommand_PlayFar(113);
                result = -1;
                break;
            } else if (gKeyState & KEY_A) {
                if (UiWork_IsCompleteFar()) {
                    selection++;
                    selection = Menu_GetModuloOfSum(selection, 7);
                    AudioCommand_PlayFar(112);
                    break;
                } else {
                    AudioCommand_PlayFar(111);
                }
            }
        }
        if (work->glyph_resource != 99) {
            Resource_ResetEntry(work->glyph_resource);
            work->glyph_resource = 99;
        }
        ((struct UiRenderWork *)gWindowWork[0])->page_arrow_shown = 0;
        RenderOutput_PrepareForRedrawFar(win_b);
        {
            struct UiWindow *window = channel->work;

            window->frame = 0;
            window->duration = 0;
            window->state = 0;
        }
        channel->work = NULL;
    } while (result == 0);
    ((struct UiRenderWork *)gWindowWork[0])->menu_busy = 1;
    RenderOutput_ClearListFar(win_a);
    RenderOutput_ClearListFar(win_b);
    WaitFrames(1);
    UiWork_FinalizeFar(win_a, 1);
    UiWork_FinalizeFar(list, 1);
    UiWork_FinalizeFar(win_b, 1);
    UiWindow_MarkVisibleTileAttributesFar();
    if (result == -2) {
        RenderOutput_PrepareForRedrawFar(menu->help_window);
        RenderOutput_PrepareForRedrawFar(menu->list_window);
        RenderOutput_PrepareForRedrawFar(*(s32 *)(menu->unknown_000 + 0x10));
        ((struct UiRenderWork *)gWindowWork[0])->menu_busy = 0;
    }
    Scheduler_AddOrUpdateCallback(Menu_UpdateEntryObjectTransforms, 0xc80);
    return result;
}
