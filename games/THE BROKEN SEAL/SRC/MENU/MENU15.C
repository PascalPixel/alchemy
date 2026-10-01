#include "EDITION.H"
#include "TYPES.H"
#include "DMA.H"
#include "IO_REG.H"
#include "RENDER_INPUT.H"
#include "WORKSPACE_OPTIONS.H"
#include "TBS_EDITION.H"

/* The object palette the slider graphics use. */
#define SLIDER_PALETTE 14
#define RESOURCE_SLOTS 96
extern const u8 Resource_FixedBlockBTiles[];
extern const u8 WorkspaceOptions_SliderTiles[];
extern const u16 WorkspaceOptions_SliderPalette[];
extern u8 MsgWindowColorLabel;
extern u8 MsgMessageSpeedLabel;
extern s8 Data_080367c9[];
extern s8 Data_080367cc[];
extern s8 Data_080367ce[];
extern u8 MsgSpeechLabel[];
extern u8 MsgAutoSleepLabel[];
struct RenderInput *UiWindow_Create(s32, s32, s32, s32, s32);
void UiWindow_DrawDividerLine(struct RenderInput *, s32, s32, s32, s32);
void UiText_DrawCharacterAtOffset(s32 message, struct RenderInput *win, s32 x, s32 y);
s32 Resource_FindFreeEntry(void);
void VramBlock_LoadCached(s32 slot, s32 size, const void *source);
struct RenderOutput *RenderOutput_Create(s32 no, s32 flags, struct RenderInput *input, s32 offset_x, s32 offset_y);
void *RenderResource_CreateFrame(s32 frame, s32 flags, struct RenderInput *input, s32 offset_x, s32 offset_y);
void Func_080b0038(void *object, s32 x, s32 y);

extern volatile u32 Data_03001c94;
extern volatile u32 gKeysRepeat;
extern u8 Data_03001ca0;
extern u8 gOptionMirror;
extern u8 MsgMessageSpeedSetting;
extern u8 MsgSpeechSetting;
extern u8 MsgAutoSleepSetting;
extern u8 MsgOptionHelp;
extern u8 Data_02000240[];
extern u8 Data_080367d0[];
extern u8 Data_080367d6[];
void OptionMenu_InitializeWork(void);
void *RenderResource_CreatePair(s32, struct RenderInput *, s32, s32);
void WaitFrames(s32);
void UiIcon_PrepareObjectFar(struct RenderOutput *);
void RenderResource_LoadFrame(s32 frame, s32 index, s32 dim);
void Shop_SetCursorFar(void *cursor, s32 x, s32 y, s32 mode);
void UiWindow_ClearInteriorTiles(struct RenderInput *, s32, s32, s32, s32);
void PaletteGlow_Update(s32, s32);
void RenderOutput_ClearList(struct RenderInput *);
void UiText_DrawResource(s32, struct RenderInput *, s32, s32);
void Ui_ApplyTableOffsetToPair(void *);
void Audio_PlayCue(s32);
void UiWork_Finalize(struct RenderInput *, s32);
void Runtime_ScheduleCallbackAndReleaseBlock20B(void);

/* Opens the workspace options window: four divider lines, six captions,
   the selection cursor, the two value markers placed along their sliders
   (value * 60 / step pixels in) and seven frame icons in three rows.
   Caption ids come from the literal pool as link-time values. */
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
    msg = (s32)&MsgWindowColorLabel;
    UiText_DrawCharacterAtOffset(msg, win, 8, 0);
    msg++;
    UiText_DrawCharacterAtOffset(msg, win, 8, 16);
#if EDITION_INTERNATIONAL
    msg = (s32)&MsgMessageSpeedLabel;
    UiText_DrawCharacterAtOffset(msg, win, 8, 32);
    msg++;
    UiText_DrawCharacterAtOffset(msg, win, 32, 40);
#else
    /* The Japanese speed caption is one line, level with its frames. */
    UiText_DrawCharacterAtOffset((s32)&MsgMessageSpeedLabel, win, 8, 40);
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
        Func_080b0038(&work->cursor, x, y);
    }

    x = Resource_FindFreeEntry();
    if (x < RESOURCE_SLOTS) {
        Dma_Set(WorkspaceOptions_SliderPalette, (void *)&OBJ_PLTT_COLOR(SLIDER_PALETTE, 0), 0x80000020, REG_DMA3);
        VramBlock_LoadCached(x, 256, WorkspaceOptions_SliderTiles);
        y = 0;
        out = RenderOutput_Create(x, 0x40004000, win, OPTION_SLIDER_X - 6, y);
        ((u8 *)&out->table)[1] = (((u8 *)&out->table)[1] & 15) | 224;
        out = RenderOutput_Create(x, 0x40004000, win, OPTION_SLIDER_X + 26, y);
        out->table.bits.index += 4;
        ((u8 *)&out->table)[1] = (((u8 *)&out->table)[1] & 15) | 224;
        y = 16;
        out = RenderOutput_Create(x, 0x40004000, win, OPTION_SLIDER_X - 6, y);
        ((u8 *)&out->table)[1] |= 240;
        out = RenderOutput_Create(x, 0x40004000, win, OPTION_SLIDER_X + 26, y);
        out->table.bits.index += 4;
        ((u8 *)&out->table)[1] |= 240;
    }

    x = Resource_FindFreeEntry();
    if (x < RESOURCE_SLOTS) {
        VramBlock_LoadCached(x, 256, 0);
        out = RenderOutput_Create(x, 0x40000000, win, 0, 0);
        ((u8 *)&out->packed)[1] |= 32;
        work->marker[0].output = out;
        x = win->x * 8 + OPTION_SLIDER_X;
        x += work->option[0] * 60 / work->option_count[0];
        y = win->y * 8 + 4;
        Func_080b0038(&work->marker[0], x, y);
    }

    x = Resource_FindFreeEntry();
    if (x < RESOURCE_SLOTS) {
        VramBlock_LoadCached(x, 256, 0);
        out = RenderOutput_Create(x, 0x40000000, win, 0, 0);
        ((u8 *)&out->packed)[1] |= 32;
        work->marker[1].output = out;
        x = win->x * 8 + OPTION_SLIDER_X;
        x += work->option[1] * 60 / work->option_count[1];
        y = win->y * 8 + 20;
        Func_080b0038(&work->marker[1], x, y);
    }

    y = 28;
    work->frame[0][0] = RenderResource_CreateFrame(Data_080367c9[0], 0, win, OPTION_FRAME_X, y);
    work->frame[0][1] = RenderResource_CreateFrame(Data_080367c9[1], 0, win, OPTION_FRAME_X + 24, y);
    work->frame[0][2] = RenderResource_CreateFrame(Data_080367c9[2], 0, win, OPTION_FRAME_X + 48, y);
    y = 52;
    work->frame[1][0] = RenderResource_CreateFrame(Data_080367cc[0], 0, win, 100, y);
    work->frame[1][1] = RenderResource_CreateFrame(Data_080367cc[1], 0, win, 124, y);
    y = 76;
    work->frame[2][0] = RenderResource_CreateFrame(Data_080367ce[0], 0, win, 100, y);
    work->frame[2][1] = RenderResource_CreateFrame(Data_080367ce[1], 0, win, 124, y);
    return win;
}

/* Runs the workspace options page until A/START (0) or B (-1).  L and R
   step through the presets, up and down move between the five settings and
   left and right change the selected one; every change redraws the page.
   On A the settings are copied into the game state. */
s32 Menu_RunWorkspaceOptions(void)
{
    struct WorkspaceWork *work;
    struct RenderInput *icon;
    struct RenderInput *win;
    void *pair;
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
    u16 *preset;
    s32 n;

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
            if (Data_03001ca0 != 0)
                work->option[4] = 0;

            for (i = 0; i <= 2; i++) {
                work->frame[0][i]->sentinel = 0xfb;
                UiIcon_PrepareObjectFar(work->frame[0][i]);
                index = (u8)work->frame[0][i]->index;
                dim = 0;
                if (i != work->option[2])
                    dim = 1;
                RenderResource_LoadFrame(Data_080367c9[i], index, dim);
            }
            for (i = 0; i <= 1; i++) {
                work->frame[1][i]->sentinel = 0xfb;
                UiIcon_PrepareObjectFar(work->frame[1][i]);
                index = (u8)work->frame[1][i]->index;
                dim = 0;
                if (i != work->option[3])
                    dim = 1;
                RenderResource_LoadFrame(Data_080367cc[i], index, dim);
            }
            for (i = 0; i <= 1; i++) {
                work->frame[2][i]->sentinel = 0xfb;
                UiIcon_PrepareObjectFar(work->frame[2][i]);
                index = (u8)work->frame[2][i]->index;
                dim = 0;
                if (i != work->option[4])
                    dim = 1;
                RenderResource_LoadFrame(Data_080367ce[i], index, dim);
            }

            x = icon->x * 8 + OPTION_SLIDER_X;
            x += (*pA * 60) / work->option_count[0];
            y = icon->y * 8 + 4;
            Shop_SetCursorFar(&work->marker[0], x, y, 1);
            x = icon->x * 8 + OPTION_SLIDER_X;
            x += (*pB * 60) / work->option_count[1];
            y = icon->y * 8 + 20;
            Shop_SetCursorFar(&work->marker[1], x, y, 1);

            x = work->option[2] + (s32)&MsgMessageSpeedSetting;
            UiWindow_ClearInteriorTiles(icon, OPTION_SETTING_X, 40, OPTION_SPEED_END, 48);
            UiText_DrawCharacterAtOffset(x, icon, OPTION_SETTING_X, 40);
            x = work->option[3] + (s32)&MsgSpeechSetting;
            UiWindow_ClearInteriorTiles(icon, OPTION_SETTING_X, 64, OPTION_SETTING_END, 72);
            UiText_DrawCharacterAtOffset(x, icon, OPTION_SETTING_X, 64);
            x = work->option[4] + (s32)&MsgAutoSleepSetting;
            UiWindow_ClearInteriorTiles(icon, OPTION_SETTING_X, 88, OPTION_SETTING_END, 96);
            UiText_DrawCharacterAtOffset(x, icon, OPTION_SETTING_X, 88);
            PaletteGlow_Update(*pA, *pB);

            x = icon->x * 8;
            y = (page * 3 + icon->y) * 8 + 4;
            if (page == 0)
                y += 8;
            Shop_SetCursorFar(&work->cursor, x, y, 3);
            RenderOutput_ClearList(win);
            UiText_DrawResource(page + (s32)&MsgOptionHelp, win, 0, 0);
        }
        Ui_ApplyTableOffsetToPair(pair);
        WaitFrames(1);

        if (Data_03001c94 & 4) {
            Audio_PlayCue(112);
            preset = &work->preset;
            n = *preset + 1;
            *preset = n;
            /* FAKEMATCH: the preset wraps to the zero held in dim. */
            dim = 0;
            redraw = 1;
            if ((u32)(n << 16) > 0x50000)
                work->preset = dim;
            *pA = Data_080367d0[*preset];
            *pB = Data_080367d6[*preset];
            continue;
        }
        if (Data_03001c94 & 9) {
            result = 0;
            Audio_PlayCue(112);
            break;
        }
        if (Data_03001c94 & 2) {
            result = -1;
            Audio_PlayCue(113);
            break;
        }
        if (gKeysRepeat & 64) {
            Audio_PlayCue(111);
            page--;
            redraw = 1;
        } else if (gKeysRepeat & 128) {
            Audio_PlayCue(111);
            page++;
            redraw = 1;
        } else {
            if (gKeysRepeat & 32) {
                Audio_PlayCue(111);
                work->option[page]--;
                redraw = 1;
            }
            if (gKeysRepeat & 16) {
                Audio_PlayCue(111);
                work->option[page]++;
                redraw = 1;
            }
        }
    }

    UiWork_Finalize(win, 2);
    UiWork_Finalize(icon, 2);
    if (result == 0) {
        Data_02000240[0x205] = work->option[0];
        Data_02000240[0x206] = work->option[1];
        Data_02000240[0x20c] = work->option[2];
        Data_02000240[0x20a] = work->option[3];
        Data_02000240[0x22a] = work->option[4];
        gOptionMirror = Data_02000240[0x22a];
    } else {
        PaletteGlow_Update(Data_02000240[0x205], Data_02000240[0x206]);
    }
    Runtime_ScheduleCallbackAndReleaseBlock20B();
    WaitFrames(1);
    return result;
}
