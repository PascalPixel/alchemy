#include "TYPES.H"
#include "SCENE.H"
#include "IWRAM_CALL.H"
#include "DMA.H"
#include "SYSTEM.H"
#include "IO_REG.H"
#include "OWNER_STATE.H"
#include "GLOBAL_CELLS.H"
#include "TBS_EDITION.H"
#include "UI.H"
#include "DJINN_MENU.H"

/* The tile in character block 1 that holds the window frame. */
#define FRAME_TILE 150
extern u8 gMenuWork[];

/* The frame tile the backdrop screen loads into BG character block 1. */
extern const u8 Menu_BackdropFrameTile[];
typedef s32 (*WordCopyFn)(void *dst, const void *src, s32 size);
typedef s32 (*WordFillFn)(void *dst, s32 size, u32 value);

static __inline__ s32 CopyWords(WordCopyFn copy, void *dst, const void *src, s32 size)
{
    /* FAKEMATCH: direct calls add r9, sl and fp saves and change the backdrop stack layout. */
    return copy(dst, src, size);
}

static __inline__ s32 FillWords(WordFillFn fill, void *dst, s32 size, u32 value)
{
    /* FAKEMATCH: a direct call loads the fill value before constructing the size. */
    return fill(dst, size, value);
}

s32 UiWindow_UpdateOrCreate(s32 *window, s32 x, s32 y, s32 width, s32 height, s32 style);
void Func_080153d8(void *);
void *Runtime_GetLowTableAddress(void);
void Graphics_AdjustPaletteBank(s32);
void UiWork_SetParamNibbleFar(s32 value);
void UiWindow_SetTilemapEntryFar(s32 window, s32 tile, s32 x, s32 y, u32 mode);
void UiWindow_DrawDividerLineFar(s32 window, u32 x1, u32 y1, u32 x2, u32 y2);
void RenderOutput_RedrawSavedRectFar(s32 window);
s32 Trade_CanOfferDjinnFar(s32 owner, s32 element, s32 djinn);
s32 Djinn_IsActiveFar(s32 owner, s32 element, s32 djinn);
extern u8 MsgUnleashEffect[];
extern u8 MsgDjinnName[];
#define ACTION_MASK 0x3fff
#define FLAG_FIRST 0x8000
#define FLAG_SECOND 0x4000
extern u8 Data_03001f2c[];
s16 Djinn_ListOwnerEntries(void *, s32, s32);
void DjinnMenu_DrawElementList(struct DjinnListTable *tbl);

/* menu/core/compute_entry_values.c */
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void Graphics_AdjustPaletteBank(s32 arg0)
{
    s32 iter;
    s32 bank;
    s32 col;
    s32 mask;
    s32 idx;

    bank = 15;
    iter = 0;
    mask = 31;
    do
    {
        for (col = 0; col <= 15; col++)
        {
            u32 color;
            s32 blue;
            s32 green;
            s32 red;
            s32 value;

            idx = bank * 16 + col;
            color = ((u16 *)0x05000000)[idx];
            blue = (color >> 10) & mask;
            green = (color >> 5) & mask;
            red = color & mask;
            blue += arg0;
            green += arg0;
            red += arg0;
            if (blue > 31)
                blue = 31;
            if (green > 31)
                green = 31;
            if (red > 31)
                red = 31;
            if (blue < 0)
                blue = 0;
            if (green < 0)
                green = 0;
            if (red < 0)
                red = 0;
            value = blue << 10;
            value |= green << 5;
            value |= red;
            ((s16 *)0x04FFFFE0)[idx] = (s16)value;
        }
        if (iter == 0)
        {
            bank = 5;
        } else
        {
            bank = 7;
            arg0 = -12;
        }
        iter++;
    } while (iter <= 2);
}

/* Opens the full-width menu window: saves BG character block 1 and
   palettes 4-7 into the backdrop buffer, blanks them with fill patterns,
   loads the frame tile and palettes, then runs the backdrop screen. */
s32 Menu_OpenBackdropScreen(void)
{
    struct DjinnMenuWork *work = *(struct DjinnMenuWork **)gMenuWork;
    struct DjinnMenuBackdrop *backdrop = work->backdrop;

    UiWindow_UpdateOrCreate(&work->list_window, 0, 5, 30, 15, 2);
    WaitFrames(1);
    CopyWords(Iwram_CopyWords, backdrop->tiles, BG_CHAR_BLOCK(1), 0x2000);
    CopyWords(Iwram_CopyWords, backdrop->palette, (void *)&BG_PLTT_COLOR(4, 0), 128);
    FillWords(Iwram_FillWords, BG_CHAR_BLOCK(1), 0x2000, 0x33333333);
    FillWords(Iwram_FillWords, (void *)&BG_PLTT_COLOR(4, 0), 128, 0x55555555);
    Func_080153d8(BG_CHAR_BLOCK(1) + 0x1000);
    CopyWords(Iwram_CopyWords, BG_CHAR_BLOCK(1) + FRAME_TILE * TILE_SIZE_4BPP,
              Menu_BackdropFrameTile, TILE_SIZE_4BPP);
    Dma_Set(Runtime_GetLowTableAddress(), (void *)&BG_PLTT_COLOR(5, 0), 0x80000010, REG_DMA3);
    BG_PLTT_COLOR(5, 14) = BG_PLTT_COLOR(15, 4);
    Dma_Set((void *)&BG_PLTT_COLOR(15, 0), (void *)&BG_PLTT_COLOR(7, 0), 0x80000010, REG_DMA3);
    Graphics_AdjustPaletteBank(8);
    BG_PLTT_COLOR(7, 4) = BG_PLTT_COLOR(15, 4);
    BG_PLTT_COLOR(6, 4) = BG_PLTT_COLOR(15, 4);
    /* FAKEMATCH: the list drawing returns nothing, but called as a statement
       its argument is loaded before the last palette store instead of after. */
    return ((s32 (*)(struct DjinnListTable *))DjinnMenu_DrawElementList)(&backdrop->list);
}

/* Lists all slots in b, marks those absent from a, then appends slots only in a. */
s32 OwnerAction_DiffSlots(struct OwnerActionSlot *a, struct OwnerActionSlot *b,
                          u16 *out, s32 *first_count, s32 *second_count)
{
    s32 i;
    s32 j;
    s32 total;
    s32 first;
    s32 second;

    total = 0;
    first = 0;
    second = 0;
    for (i = 0; i < 32 && b[i].encoded_action != 0; i++) {
        out[total] = b[i].encoded_action & ACTION_MASK;
        total++;
        for (j = 0; j < 32; j++) {
            if (((b[i].encoded_action ^ a[j].encoded_action) & ACTION_MASK) == 0)
                break;
        }
        if (j == 32) {
            first++;
            out[total - 1] |= FLAG_FIRST;
        }
    }
    for (i = 0; i < 32 && a[i].encoded_action != 0; i++) {
        for (j = 0; j < 32; j++) {
            if (((a[i].encoded_action ^ b[j].encoded_action) & ACTION_MASK) == 0)
                break;
        }
        if (j == 32) {
            second++;
            out[total] = (a[i].encoded_action & ACTION_MASK) | FLAG_SECOND;
            total++;
        }
    }
    *first_count = first;
    *second_count = second;
    return total;
}

s32 Menu_ComputeEntryValues(void *tbl)
{
    void *state;
    s32 i;
    void *p;
    u16 *src;
    s8 *dst;
    s16 v;
    s32 cnt;

    state = *(void **)((u32)&Data_03001f2c);
    i = 0;
    if (i < FIELD_AT_OFFSET(state, u8, 0x219)) {
        dst = (s8 *)tbl + 0xA0;
        src = (u16 *)((u8 *)state + 0x208);
        p = tbl;
        do {
            v = Djinn_ListOwnerEntries(p, *src, -1);
            cnt = FIELD_AT_OFFSET(state, u8, 0x219);
            i += 1;
            *dst = v;
            src += 1;
            dst += 1;
            p = (u8 *)p + 0x14;
        } while (i < cnt);
    }
}

/* Tilemap entry: the element tile in palette 5. */
#define ELEMENT_TILE 0x5001
#define ENTRY list[i]

/* Lists every party member's Djinn in the full-width window, a column per
   member and grouped by element: set Djinn in the second text colour, Djinn
   that can be neither offered nor used in the fourth. */
void DjinnMenu_DrawElementList(struct DjinnListTable *tbl)
{
    struct DjinnMenuWork *menu;
    s32 i;
    s32 row;
    s32 element;
    struct UiWork *ui;
    s32 line;
    s32 usable;
    u16 *list;

    menu = *(struct DjinnMenuWork **)gMenuWork;
    ui = *(struct UiWork **)(gMenuWork - 160);
    ui->menu_busy = 1;
    i = 0;
    if (menu->owner_count != 0) {
        do {
            tbl->counts[i] = Djinn_ListOwnerEntries(tbl->ids[i], menu->owners[i], -1);
            i++;
        } while (i < menu->owner_count);
    }
    RenderOutput_RedrawSavedRectFar(menu->list_window);
    UiText_DrawCharacterAtOffsetFar((s32)MsgUnleashEffect, menu->list_window, 0, 80);
    for (row = 0; row < menu->owner_count; row++) {
        list = tbl->ids[row];
        line = 0;
        for (element = 0; element < DJINN_ELEMENTS; element++) {
            for (i = 0; i < tbl->counts[row]; i++) {
                if (element != (ENTRY & DJINN_ENTRY_ELEMENT) >> 5)
                    continue;
                if ((ENTRY & DJINN_ENTRY_IS_SET) == 0)
                    UiWork_SetParamNibbleFar(2);
                usable = 0;
                if (Trade_CanOfferDjinnFar((ENTRY & DJINN_ENTRY_OWNER) >> 8,
                        (ENTRY & DJINN_ENTRY_ELEMENT) >> 5, ENTRY & DJINN_ENTRY_INDEX)
                    || Djinn_IsActiveFar((ENTRY & DJINN_ENTRY_OWNER) >> 8,
                        (ENTRY & DJINN_ENTRY_ELEMENT) >> 5, ENTRY & DJINN_ENTRY_INDEX))
                    usable = 1;
                if (!usable)
                    UiWork_SetParamNibbleFar(4);
                UiWindow_SetTilemapEntryFar(menu->list_window,
                    ((ENTRY & DJINN_ENTRY_ELEMENT) >> 5) + ELEMENT_TILE,
                    row * 7 + 1, line + 2, 0);
                UiText_DrawCharacterAtOffsetFar((s32)MsgDjinnName
                    + ((ENTRY & DJINN_ENTRY_ELEMENT) >> 5) * DJINN_PER_ELEMENT
                    + (ENTRY & DJINN_ENTRY_INDEX),
                    menu->list_window, row * 56 + 16, line * 8 + 16);
                line++;
                UiWork_SetParamNibbleFar(15);
            }
        }
    }
    UiWindow_DrawDividerLineFar(menu->list_window, 0, 10, 28, 10);
    (*(struct UiWork **)(gMenuWork - 160))->dirty = 1;
    ui->menu_busy = 0;
}
