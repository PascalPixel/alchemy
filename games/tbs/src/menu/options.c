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
#include "edition.h"
#include "types.h"
#include "game_state.h"
#include "dma.h"
#include "io_reg.h"
#include "render_input.h"
#include "workspace_options.h"
#include "tbs_edition.h"
#include "uiflow.h"
#include "text/msg_ids.h"

#define Resource_FixedBlockBTiles ((const u8 *)&Resource_FixedBlockBTiles)
#define UiWindow_Create ((struct RenderInput * (*)(s32, s32, s32, s32, s32))UiWindow_Create)
#define UiText_DrawCharacterAtOffset ((void (*)(s32, struct RenderInput *, s32, s32))UiText_DrawCharacterAtOffset)

#define RenderResource_CreateFrame ((void * (*)(s32,s32,struct RenderInput*,s32,s32))RenderResource_CreateFrame)
#define ShopCursor_SetPositionImmediateFar ((void (*)(void*,s32,s32))ShopCursor_SetPositionImmediateFar)
#define Shop_SetCursorFar ((void (*)(void*,s32,s32,s32))Shop_SetCursorFar)
#define UiWindow_ClearInteriorTiles ((void (*)(struct RenderInput*,s32,s32,s32,s32))UiWindow_ClearInteriorTiles)
#define Ui_ApplyTableOffsetToPair ((void (*)(void*))Ui_ApplyTableOffsetToPair)

#define SLIDER_PALETTE 14
#define RESOURCE_SLOTS 96
void UiWindow_DrawDividerLine(struct RenderInput *, s32, s32, s32, s32);

extern u8 gSleepDisabled;
extern u8 gOptionMirror;
void WaitFrames(s32);
void PaletteGlow_Update(s32, s32);
void UiText_DrawResource(s32, struct RenderInput *, s32, s32);
void AudioCommand_PlayFar(s32);

struct RenderInput *Menu_OpenWorkspaceOptions(void)
{
    /* FAKEMATCH: the shared cursor union keeps pointer stores before window reads; an equal-size struct reorders these moves in all six editions. */
    struct WorkspaceWork *work;
    struct RenderInput *win;
    struct RenderOutput *out;
    s32 x;
    s32 y;
    s32 msg;

    work = gSelectionWork;
    win = UiWindow_Create(OPTION_WINDOW_X, 5, OPTION_WINDOW_WIDTH, 14, 2);
    UiWindow_DrawDividerLine(win, 0, 2, OPTION_WINDOW_WIDTH - 1, 2);
    UiWindow_DrawDividerLine(win, 0, 4, OPTION_WINDOW_WIDTH - 1, 4);
    UiWindow_DrawDividerLine(win, 0, 7, OPTION_WINDOW_WIDTH - 1, 7);
    UiWindow_DrawDividerLine(win, 0, 10, OPTION_WINDOW_WIDTH - 1, 10);
    msg = (s32)MsgWindowColorLabel;
    UiText_DrawCharacterAtOffset(msg, win, 8, 0);
    msg++;
    UiText_DrawCharacterAtOffset(msg, win, 8, 16);
#if EDITION_INTERNATIONAL
    msg = (s32)MsgMessageSpeedLabel;
    UiText_DrawCharacterAtOffset(msg, win, 8, 32);
    msg++;
    UiText_DrawCharacterAtOffset(msg, win, 32, 40);
#else
    UiText_DrawCharacterAtOffset((s32)MsgMessageSpeedLabel, win, 8, 40);
#endif
    UiText_DrawCharacterAtOffset((s32)MsgSpeechLabel, win, 8, 64);
    UiText_DrawCharacterAtOffset((s32)MsgAutoSleepLabel, win, 8, 88);

    x = Resource_FindFreeEntry();
    if (x < RESOURCE_SLOTS) {
        VramBlock_LoadCached(x, 128, (const void *)Resource_FixedBlockBTiles);
        out = RenderOutput_Create(x, 0x40000000, win, 0, 0);
        work->cursor.output = out;
        x = win->x * 8;
        y = win->y * 8 + 12;
        ShopCursor_SetPositionImmediateFar(&work->cursor, x, y);
    }

    x = Resource_FindFreeEntry();
    if (x < RESOURCE_SLOTS) {
        Dma_Set(WorkspaceOptions_SliderPalette, (void *)&OBJ_PLTT_COLOR(SLIDER_PALETTE, 0), 0x80000020, REG_DMA3);
        VramBlock_LoadCached(x, 256, WorkspaceOptions_SliderTiles);
        y = 0;
        out = RenderOutput_Create(x, 0x40004000, win, OPTION_SLIDER_X - 6, y);
        out->oam.palette = SLIDER_PALETTE;
        out = RenderOutput_Create(x, 0x40004000, win, OPTION_SLIDER_X + 26, y);
        out->oam.index += 4;
        out->oam.palette = SLIDER_PALETTE;
        y = 16;
        out = RenderOutput_Create(x, 0x40004000, win, OPTION_SLIDER_X - 6, y);
        out->oam.palette = 15;
        out = RenderOutput_Create(x, 0x40004000, win, OPTION_SLIDER_X + 26, y);
        out->oam.index += 4;
        out->oam.palette = 15;
    }

    x = Resource_FindFreeEntry();
    if (x < RESOURCE_SLOTS) {
        VramBlock_LoadCached(x, 256, 0);
        out = RenderOutput_Create(x, 0x40000000, win, 0, 0);
        out->oam.palette_256 = 1;
        work->marker[0].output = out;
        x = win->x * 8 + OPTION_SLIDER_X;
        x += work->option[0] * 60 / work->option_count[0];
        y = win->y * 8 + 4;
        ShopCursor_SetPositionImmediateFar(&work->marker[0], x, y);
    }

    x = Resource_FindFreeEntry();
    if (x < RESOURCE_SLOTS) {
        VramBlock_LoadCached(x, 256, 0);
        out = RenderOutput_Create(x, 0x40000000, win, 0, 0);
        out->oam.palette_256 = 1;
        work->marker[1].output = out;
        x = win->x * 8 + OPTION_SLIDER_X;
        x += work->option[1] * 60 / work->option_count[1];
        y = win->y * 8 + 20;
        ShopCursor_SetPositionImmediateFar(&work->marker[1], x, y);
    }

    y = 28;
    work->frame[0][0] = RenderResource_CreateFrame(OptionMenu_SpeedFrames[0], 0, win, OPTION_FRAME_X, y);
    work->frame[0][1] = RenderResource_CreateFrame(OptionMenu_SpeedFrames[1], 0, win, OPTION_FRAME_X + 24, y);
    work->frame[0][2] = RenderResource_CreateFrame(OptionMenu_SpeedFrames[2], 0, win, OPTION_FRAME_X + 48, y);
    y = 52;
    work->frame[1][0] = RenderResource_CreateFrame(OptionMenu_SpeechFrames[0], 0, win, 100, y);
    work->frame[1][1] = RenderResource_CreateFrame(OptionMenu_SpeechFrames[1], 0, win, 124, y);
    y = 76;
    work->frame[2][0] = RenderResource_CreateFrame(OptionMenu_SleepFrames[0], 0, win, 100, y);
    work->frame[2][1] = RenderResource_CreateFrame(OptionMenu_SleepFrames[1], 0, win, 124, y);
    return win;
}

s32 Menu_RunWorkspaceOptions(void)
{
    struct WorkspaceWork *work;
    struct RenderInput *icon;
    struct RenderInput *win;
    struct RenderOutput *pair;
    s8 *pA;
    s8 *pB;
    s32 redraw;
    s32 page;
    s32 i;
    s32 dim;
    s32 index;
    s32 x;
    s32 y;
    s32 result;
    struct RenderOutput *frame;

    redraw = 1;
    page = 0;
    OptionMenu_InitializeWork();
    work = gSelectionWork;
    win = UiWindow_Create(OPTION_WINDOW_X, 2, OPTION_WINDOW_WIDTH, 3, 2);
    icon = Menu_OpenWorkspaceOptions();
    pair = RenderResource_CreatePair(7, icon, 64, -48);
    WaitFrames(1);
    pA = &work->option[0];
    pB = &work->option[1];

    for (;;) {
        if (redraw != 0) {
            redraw = 0;
            page = (page + 5) % 5;
            work->option[page] = (work->option[page] + work->option_count[page]) % work->option_count[page];
            work->page = page;
            if (gSleepDisabled != 0)
                work->option[4] = 0;

            for (i = 0; i <= 2; i++) {
                frame = work->frame[0][i];
                frame->sentinel = 0xfb;
                UiIcon_PrepareObjectFar(frame);
                index = (u8)work->frame[0][i]->index;
                dim = 0;
                if (i != work->option[2])
                    dim = 1;
                RenderResource_LoadFrame(OptionMenu_SpeedFrames[i], index, dim);
            }
            for (i = 0; i <= 1; i++) {
                frame = work->frame[1][i];
                frame->sentinel = 0xfb;
                UiIcon_PrepareObjectFar(frame);
                index = (u8)work->frame[1][i]->index;
                dim = 0;
                if (i != work->option[3])
                    dim = 1;
                RenderResource_LoadFrame(OptionMenu_SpeechFrames[i], index, dim);
            }
            for (i = 0; i <= 1; i++) {
                frame = work->frame[2][i];
                frame->sentinel = 0xfb;
                UiIcon_PrepareObjectFar(frame);
                index = (u8)work->frame[2][i]->index;
                dim = 0;
                if (i != work->option[4])
                    dim = 1;
                RenderResource_LoadFrame(OptionMenu_SleepFrames[i], index, dim);
            }

            x = icon->x * 8 + OPTION_SLIDER_X;
            x += (*pA * 60) / work->option_count[0];
            y = icon->y * 8 + 4;
            Shop_SetCursorFar(&work->marker[0], x, y, 1);
            x = icon->x * 8 + OPTION_SLIDER_X;
            x += (*pB * 60) / work->option_count[1];
            y = icon->y * 8 + 20;
            Shop_SetCursorFar(&work->marker[1], x, y, 1);

            x = work->option[2] + (s32)MsgMessageSpeedSetting;
            UiWindow_ClearInteriorTiles(icon, OPTION_SETTING_X, 40, OPTION_SPEED_END, 48);
            UiText_DrawCharacterAtOffset(x, icon, OPTION_SETTING_X, 40);
            x = work->option[3] + (s32)MsgSpeechSetting;
            UiWindow_ClearInteriorTiles(icon, OPTION_SETTING_X, 64, OPTION_SETTING_END, 72);
            UiText_DrawCharacterAtOffset(x, icon, OPTION_SETTING_X, 64);
            x = work->option[4] + (s32)MsgAutoSleepSetting;
            UiWindow_ClearInteriorTiles(icon, OPTION_SETTING_X, 88, OPTION_SETTING_END, 96);
            UiText_DrawCharacterAtOffset(x, icon, OPTION_SETTING_X, 88);
            PaletteGlow_Update(*pA, *pB);

            x = icon->x * 8;
            y = (page * 3 + icon->y) * 8 + 4;
            if (page == 0)
                y += 8;
            Shop_SetCursorFar(&work->cursor, x, y, 3);
            RenderOutput_ClearList(win);
            UiText_DrawResource(page + (s32)MsgOptionHelp, win, 0, 0);
        }
        Ui_ApplyTableOffsetToPair(pair);
        WaitFrames(1);

        if (gKeyState & KEY_SELECT) {
            AudioCommand_PlayFar(112);
            work->preset++;
            redraw = 1;
            if (work->preset > 5)
                work->preset = 0;
            *pA = OptionMenu_FirstColorPresets[work->preset];
            *pB = OptionMenu_SecondColorPresets[work->preset];
            continue;
        }
        if (gKeyState & (KEY_A | KEY_START)) {
            result = 0;
            AudioCommand_PlayFar(112);
            break;
        }
        if (gKeyState & KEY_B) {
            result = -1;
            AudioCommand_PlayFar(113);
            break;
        }
        if (gKeysRepeat & KEY_UP) {
            AudioCommand_PlayFar(111);
            page--;
            redraw = 1;
        } else if (gKeysRepeat & KEY_DOWN) {
            AudioCommand_PlayFar(111);
            page++;
            redraw = 1;
        } else {
            if (gKeysRepeat & KEY_LEFT) {
                AudioCommand_PlayFar(111);
                work->option[page]--;
                redraw = 1;
            }
            if (gKeysRepeat & KEY_RIGHT) {
                AudioCommand_PlayFar(111);
                work->option[page]++;
                redraw = 1;
            }
        }
    }

    UiWork_Finalize(win, 2);
    UiWork_Finalize(icon, 2);
    if (result == 0) {
        gGameState.palette_glow[0] = work->option[0];
        gGameState.palette_glow[1] = work->option[1];
        gGameState.message_speed = work->option[2];
        gGameState.speech = work->option[3];
        gGameState.saved_options = work->option[4];
        gOptionMirror = gGameState.saved_options;
    } else {
        PaletteGlow_Update(gGameState.palette_glow[0], gGameState.palette_glow[1]);
    }
    Runtime_ScheduleCallbackAndReleaseBlock20B();
    WaitFrames(1);
    return result;
}
