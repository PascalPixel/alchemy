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
#include "scene.h"
#include "iwram_call.h"
#include "dma.h"
#include "system.h"
#include "io_reg.h"
#include "owner_state.h"
#include "global_cells.h"
#include "tbs_edition.h"
#include "ui.h"
#include "djinn_menu.h"
#include "heap_state.h"
#include "text/msg_ids.h"

#define RenderOutput_RedrawSavedRectFar ((void (*)(s32))RenderOutput_RedrawSavedRectFar)

#define Runtime_GetLowTableAddress ((void * (*)(void))Runtime_GetLowTableAddress)
#define Djinn_ListOwnerEntries ((s16 (*)(void*,s32,s32))Djinn_ListOwnerEntries)

#define FRAME_TILE 150

typedef s32 (*WordCopyFn)(void *dst, const void *src, s32 size);
typedef s32 (*WordFillFn)(void *dst, s32 size, u32 value);

#define ACTION_MASK 0x3fff
#define FLAG_FIRST 0x8000
#define FLAG_SECOND 0x4000

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 Menu_OpenBackdropScreen(void)
{
    struct DjinnMenuWork *work = gMenuWork;
    struct DjinnMenuBackdrop *backdrop = work->backdrop;

    UiWindow_UpdateOrCreate(&work->list_window, 0, 5, 30, 15, 2);
    WaitFrames(1);
    Iwram_CopyWords(backdrop->tiles, BG_CHAR_BLOCK(1), 0x2000);
    Iwram_CopyWords(backdrop->palette, (void *)&BG_PLTT_COLOR(4, 0), 128);
    Iwram_FillWords(BG_CHAR_BLOCK(1), 0x2000, 0x33333333);
    Iwram_FillWords((void *)&BG_PLTT_COLOR(4, 0), 128, 0x55555555);
    Graphics_ExpandVramTilesByColorTableFar(BG_CHAR_BLOCK(1) + 0x1000);
    Iwram_CopyWords(BG_CHAR_BLOCK(1) + FRAME_TILE * TILE_SIZE_4BPP,
              Menu_BackdropFrameTile, TILE_SIZE_4BPP);
    Dma_Set(Runtime_GetLowTableAddress(), (void *)&BG_PLTT_COLOR(5, 0), 0x80000010, REG_DMA3);
    BG_PLTT_COLOR(5, 14) = BG_PLTT_COLOR(15, 4);
    Dma_Set((void *)&BG_PLTT_COLOR(15, 0), (void *)&BG_PLTT_COLOR(7, 0), 0x80000010, REG_DMA3);
    Graphics_AdjustPaletteBank(8);
    BG_PLTT_COLOR(7, 4) = BG_PLTT_COLOR(15, 4);
    BG_PLTT_COLOR(6, 4) = BG_PLTT_COLOR(15, 4);
    /* FAKEMATCH: the list drawing returns nothing, but called as a statement its argument is loaded before the last palette store instead of after. */
    return ((s32 (*)(struct DjinnListTable *))DjinnMenu_DrawElementList)(&backdrop->list);
}

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

    state = gMenuWork;
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

#define ELEMENT_TILE 0x5001
#define ENTRY list[i]

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

    menu = gMenuWork;
    ui = *(struct UiWork **)((u8 *)&gMenuWork + (HEAP_SLOT_WINDOW - HEAP_SLOT_MENU) * sizeof(void *));
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
    (*(struct UiWork **)((u8 *)&gMenuWork + (HEAP_SLOT_WINDOW - HEAP_SLOT_MENU) * sizeof(void *)))->dirty = 1;
    ui->menu_busy = 0;
}
