#include "TYPES.H"
#include "DMA.H"
#include "RENDER_INPUT.H"

/* A menu cursor record (object pointer and cursor state).
   FAKEMATCH: declared as a union so the stores of its object pointer stay
   ordered against the window reads that follow them. */
union MenuCursor {
    struct RenderOutput *output;
    u8 data[0x10];
};

struct WorkspaceWork {
    u8 unknown_000[0x594];
    s8 value[5];
    s8 step[5];
    u8 unknown_59e[6];
    union MenuCursor cursor;
    union MenuCursor marker[2];
    u8 unknown_5d4[0x18];
    void *frame[3][3];
};

extern struct WorkspaceWork *Data_03001ea0;
extern u8 Value_00000c07;
extern u8 Value_00000c0d;
extern s8 Data_080367c9[];
extern s8 Data_080367cc[];
extern s8 Data_080367ce[];

struct RenderInput *UiWindow_Create(s32, s32, s32, s32, s32);
void UiWindow_DrawDividerLine(struct RenderInput *, s32, s32, s32, s32);
void UiText_DrawCharacterAtOffset(s32 message, struct RenderInput *win, s32 x, s32 y);
s32 Resource_FindFreeEntry(void);
void VramBlock_LoadCached(s32 slot, s32 size, const void *source);
struct RenderOutput *RenderOutput_Create(s32 no, s32 flags, struct RenderInput *input, s32 offset_x, s32 offset_y);
void *RenderOutput_CreateFrame(s32 frame, s32 flags, struct RenderInput *input, s32 offset_x, s32 offset_y);
s32 Math_Div(s32 numerator, s32 denominator);
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

    work = Data_03001ea0;
    win = UiWindow_Create(1, 5, 28, 14, 2);
    UiWindow_DrawDividerLine(win, 0, 2, 27, 2);
    UiWindow_DrawDividerLine(win, 0, 4, 27, 4);
    UiWindow_DrawDividerLine(win, 0, 7, 27, 7);
    UiWindow_DrawDividerLine(win, 0, 10, 27, 10);
    msg = (s32)&Value_00000c07;
    UiText_DrawCharacterAtOffset(msg, win, 8, 0);
    msg++;
    UiText_DrawCharacterAtOffset(msg, win, 8, 16);
    msg = (s32)&Value_00000c0d;
    UiText_DrawCharacterAtOffset(msg, win, 8, 32);
    msg++;
    UiText_DrawCharacterAtOffset(msg, win, 32, 40);
    UiText_DrawCharacterAtOffset(0xc0f, win, 8, 64);
    UiText_DrawCharacterAtOffset(0xc12, win, 8, 88);

    x = Resource_FindFreeEntry();
    if (x < 96) {
        VramBlock_LoadCached(x, 128, (const void *)0x080310a4);
        out = RenderOutput_Create(x, 0x40000000, win, 0, 0);
        work->cursor.output = out;
        x = win->x * 8;
        y = win->y * 8 + 12;
        Func_080b0038(&work->cursor, x, y);
    }

    x = Resource_FindFreeEntry();
    if (x < 96) {
        Dma_Set((const void *)0x08073812, (void *)0x050003c0, 0x80000020, (volatile u32 *)0x040000d4);
        VramBlock_LoadCached(x, 256, (const void *)0x08029910);
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
    if (x < 96) {
        VramBlock_LoadCached(x, 256, 0);
        out = RenderOutput_Create(x, 0x40000000, win, 0, 0);
        ((u8 *)&out->packed)[1] |= 32;
        work->marker[0].output = out;
        x = win->x * 8 + 140;
        x += Math_Div(work->value[0] * 60, work->step[0]);
        y = win->y * 8 + 4;
        Func_080b0038(&work->marker[0], x, y);
    }

    x = Resource_FindFreeEntry();
    if (x < 96) {
        VramBlock_LoadCached(x, 256, 0);
        out = RenderOutput_Create(x, 0x40000000, win, 0, 0);
        ((u8 *)&out->packed)[1] |= 32;
        work->marker[1].output = out;
        x = win->x * 8 + 140;
        x += Math_Div(work->value[1] * 60, work->step[1]);
        y = win->y * 8 + 20;
        Func_080b0038(&work->marker[1], x, y);
    }

    y = 28;
    work->frame[0][0] = RenderOutput_CreateFrame(Data_080367c9[0], 0, win, 84, y);
    work->frame[0][1] = RenderOutput_CreateFrame(Data_080367c9[1], 0, win, 108, y);
    work->frame[0][2] = RenderOutput_CreateFrame(Data_080367c9[2], 0, win, 132, y);
    y = 52;
    work->frame[1][0] = RenderOutput_CreateFrame(Data_080367cc[0], 0, win, 100, y);
    work->frame[1][1] = RenderOutput_CreateFrame(Data_080367cc[1], 0, win, 124, y);
    y = 76;
    work->frame[2][0] = RenderOutput_CreateFrame(Data_080367ce[0], 0, win, 100, y);
    work->frame[2][1] = RenderOutput_CreateFrame(Data_080367ce[1], 0, win, 124, y);
    return win;
}
