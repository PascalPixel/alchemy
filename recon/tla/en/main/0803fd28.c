#include "RESOURCE.H"
#include "TYPES.H"
#include "RENDER_INPUT.H"

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
struct RenderOutput *RenderOutput_Create(s32 no, s32 flags, struct RenderInput *input, s32 offset_x, s32 offset_y);
void *RenderResource_CreateFrame(s32 frame, s32 flags, struct RenderInput *input, s32 offset_x, s32 offset_y);
s32 __divsi3(s32 numerator, s32 denominator);
void Func_080b0038(void *object, s32 x, s32 y);

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
    win = UiWindow_Create(1, 5, 28, 14, 2);
    UiWindow_DrawDividerLine(win, 0, 2, 27, 2);
    UiWindow_DrawDividerLine(win, 0, 4, 27, 4);
    UiWindow_DrawDividerLine(win, 0, 7, 27, 7);
    UiWindow_DrawDividerLine(win, 0, 10, 27, 10);
    msg = (s32)&MsgWindowColorLabel;
    UiText_DrawCharacterAtOffset(msg, win, 8, 0);
    msg++;
    UiText_DrawCharacterAtOffset(msg, win, 8, 16);
    msg = (s32)&MsgMessageSpeedLabel;
    UiText_DrawCharacterAtOffset(msg, win, 8, 32);
    msg++;
    UiText_DrawCharacterAtOffset(msg, win, 32, 40);
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
        out = RenderOutput_Create(x, 0x40004000, win, 134, y);
        ((u8 *)&out->table)[1] = (((u8 *)&out->table)[1] & 15) | 224;
        out = RenderOutput_Create(x, 0x40004000, win, 166, y);
        out->table.bits.index += 4;
        ((u8 *)&out->table)[1] = (((u8 *)&out->table)[1] & 15) | 224;
        y = 16;
        out = RenderOutput_Create(x, 0x40004000, win, 134, y);
        ((u8 *)&out->table)[1] |= 240;
        out = RenderOutput_Create(x, 0x40004000, win, 166, y);
        out->table.bits.index += 4;
        ((u8 *)&out->table)[1] |= 240;
    }

    x = Resource_FindFreeEntry();
    if (x < RESOURCE_SLOTS) {
        VramBlock_LoadCached(x, 256, 0);
        out = RenderOutput_Create(x, 0x40000000, win, 0, 0);
        ((u8 *)&out->packed)[1] |= 32;
        work->marker[0].output = out;
        x = win->x * 8 + 140;
        x += __divsi3(work->option[0] * 60, work->option_count[0]);
        y = win->y * 8 + 4;
        Func_080b0038(&work->marker[0], x, y);
    }

    x = Resource_FindFreeEntry();
    if (x < RESOURCE_SLOTS) {
        VramBlock_LoadCached(x, 256, 0);
        out = RenderOutput_Create(x, 0x40000000, win, 0, 0);
        ((u8 *)&out->packed)[1] |= 32;
        work->marker[1].output = out;
        x = win->x * 8 + 140;
        x += __divsi3(work->option[1] * 60, work->option_count[1]);
        y = win->y * 8 + 20;
        Func_080b0038(&work->marker[1], x, y);
    }

    y = 28;
    work->frame[0][0] = RenderResource_CreateFrame(Data_080367c9[0], 0, win, 84, y);
    work->frame[0][1] = RenderResource_CreateFrame(Data_080367c9[1], 0, win, 108, y);
    work->frame[0][2] = RenderResource_CreateFrame(Data_080367c9[2], 0, win, 132, y);
    y = 52;
    work->frame[1][0] = RenderResource_CreateFrame(Data_080367cc[0], 0, win, 100, y);
    work->frame[1][1] = RenderResource_CreateFrame(Data_080367cc[1], 0, win, 124, y);
    y = 76;
    work->frame[2][0] = RenderResource_CreateFrame(Data_080367ce[0], 0, win, 100, y);
    work->frame[2][1] = RenderResource_CreateFrame(Data_080367ce[1], 0, win, 124, y);
    return win;
}
