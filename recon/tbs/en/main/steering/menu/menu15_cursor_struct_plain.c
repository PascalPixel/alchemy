/* NONMATCHING: 2026-10-01 equal-size menu cursor struct attempt.
 * Menu_OpenWorkspaceOptions: mov r4, r0 becomes mov r1, fp (428/428 assembly lines in EN).
 * All six editions change; the production union preserves the measured order.
 * Existing approved TBS options; complete function and literal pool remain to match.
 */
#include "TYPES.H"
#include "DMA.H"
#include "IO_REG.H"
#include "RENDER_INPUT.H"
#ifndef ALCHEMY_WORKSPACE_OPTIONS_H
#define ALCHEMY_WORKSPACE_OPTIONS_H

#include "TYPES.H"
#include "RENDER_INPUT.H"

/* A menu cursor record (object pointer and cursor state). The union keeps
   pointer stores ordered against the window reads that follow them;
   the two affected callers record their measured steering reason. */
struct MenuCursor {
    struct RenderOutput *output;
    u8 data[0xc];
};

/* The options page of the workspace: five settings, each a value in
   0..count-1, and the menu objects that show them. */
struct WorkspaceWork {
    u8 unknown_000[0x574];
    u16 page;
    u8 unknown_576[8];
    u16 preset;
    u8 unknown_580[0x14];
    s8 option[5];
    s8 option_count[5];
    u8 unknown_59e[6];
    struct MenuCursor cursor;
    struct MenuCursor marker[2];
    u8 unknown_5d4[0x18];
    struct RenderOutput *frame[3][3];
};

extern struct WorkspaceWork *gSelectionWork;

struct RenderInput *Menu_OpenWorkspaceOptions(void);

#endif

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
#if defined(TBS_EDITION_JA)
    /* The Japanese speed caption is one line, level with its frames. */
    UiText_DrawCharacterAtOffset((s32)&MsgMessageSpeedLabel, win, 8, 40);
#else
    msg = (s32)&MsgMessageSpeedLabel;
    UiText_DrawCharacterAtOffset(msg, win, 8, 32);
    msg++;
    UiText_DrawCharacterAtOffset(msg, win, 32, 40);
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
