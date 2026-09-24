/*
 * Menu_OpenWorkspaceOptions (main:0801d108, 964 bytes)
 *
 * Opens the workspace options window: four divider lines, six captions,
 * the selection cursor, the two value markers placed along their sliders
 * (value * 60 / step pixels in), and seven frame icons in three rows.
 * Returns the window for Menu_RunWorkspaceOptions.
 *
 * Draft, not exact (2026-09-24): candidate=964 reference=964
 * differing_halfwords=6.  Everything matches except the second and third
 * frame rows, where the reference loads the row table address before the
 * previous frame pointer is stored.  Findings: the work area is accessed
 * through a union view, so its stores alias the window loads; the caption
 * ids are link-time Value_ symbols with a post-incremented local; the
 * marker x is (x * 8 + 140) + quotient.  With const row tables the address
 * load is hoisted as in the reference, but mov r2, fp then wins the tie
 * against the first ldrsb of each row (it has one more dependent, the
 * epilogue's fp restore); with non-const tables (this draft) the ldrsb
 * order is right but the address load waits for the store.
 */
#include "TYPES.H"
#include "DMA.H"
#include "RENDER_INPUT.H"

union WorkspaceWork {
    s8 s8[0x610];
    u16 u16[0x308];
    struct RenderOutput *out[0x184];
    void *ptr[0x184];
};

extern union WorkspaceWork *Data_03001ea0;
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

struct RenderInput *Menu_OpenWorkspaceOptions(void)
{
    union WorkspaceWork *work;
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
        work->out[0x5a4 / 4] = out;
        x = win->x * 8;
        y = win->y * 8 + 12;
        Func_080b0038(&work->out[0x5a4 / 4], x, y);
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
        work->out[0x5b4 / 4] = out;
        x = win->x * 8 + 140;
        x += Math_Div(work->s8[0x594] * 60, work->s8[0x599]);
        y = win->y * 8 + 4;
        Func_080b0038(&work->out[0x5b4 / 4], x, y);
    }

    x = Resource_FindFreeEntry();
    if (x < 96) {
        VramBlock_LoadCached(x, 256, 0);
        out = RenderOutput_Create(x, 0x40000000, win, 0, 0);
        ((u8 *)&out->packed)[1] |= 32;
        work->out[0x5c4 / 4] = out;
        x = win->x * 8 + 140;
        x += Math_Div(work->s8[0x595] * 60, work->s8[0x59a]);
        y = win->y * 8 + 20;
        Func_080b0038(&work->out[0x5c4 / 4], x, y);
    }

    y = 28;
    work->ptr[0x5ec / 4 + 0] = RenderOutput_CreateFrame(Data_080367c9[0], 0, win, 84, y);
    work->ptr[0x5ec / 4 + 1] = RenderOutput_CreateFrame(Data_080367c9[1], 0, win, 108, y);
    work->ptr[0x5ec / 4 + 2] = RenderOutput_CreateFrame(Data_080367c9[2], 0, win, 132, y);
    y = 52;
    work->ptr[0x5ec / 4 + 3] = RenderOutput_CreateFrame(Data_080367cc[0], 0, win, 100, y);
    work->ptr[0x5ec / 4 + 4] = RenderOutput_CreateFrame(Data_080367cc[1], 0, win, 124, y);
    y = 76;
    work->ptr[0x5ec / 4 + 6] = RenderOutput_CreateFrame(Data_080367ce[0], 0, win, 100, y);
    work->ptr[0x5ec / 4 + 7] = RenderOutput_CreateFrame(Data_080367ce[1], 0, win, 124, y);
    return win;
}
