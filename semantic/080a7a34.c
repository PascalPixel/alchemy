#include "layout_guard.h"
#include "types.h"

typedef struct DmaTransfer_080a7a34 {
    const void *source;
    void *destination;
    u32 control;
} DmaTransfer_080a7a34;

typedef struct EntryMenu_080a7a34 {
    u8 padding000[8];
    s32 selected_id;
    u8 padding00c[4];
    void *selection_window;
    u8 padding014[8];
    s8 selection;
    u8 padding01d;
    s8 entry_count;
    u8 padding01f[0xed];
    void *menu_window;
    u8 padding110[0x34];
    u16 row_styles[8];
    u8 padding154[0xb4];
    u16 entries[9];
    s8 selected_actor;
    u8 padding21b[5];
    u16 cursor_phase;
    u8 padding222[0x12];
    s16 cursor_x[4];
    s16 cursor_y[4];
} EntryMenu_080a7a34;

LAYOUT_OFFSET_GUARD(
    EntryMenu080a7a34_SelectedId,
    EntryMenu_080a7a34,
    selected_id,
    8);
LAYOUT_OFFSET_GUARD(
    EntryMenu080a7a34_SelectionWindow,
    EntryMenu_080a7a34,
    selection_window,
    0x10);
LAYOUT_OFFSET_GUARD(
    EntryMenu080a7a34_Selection,
    EntryMenu_080a7a34,
    selection,
    0x1c);
LAYOUT_OFFSET_GUARD(
    EntryMenu080a7a34_EntryCount,
    EntryMenu_080a7a34,
    entry_count,
    0x1e);
LAYOUT_OFFSET_GUARD(
    EntryMenu080a7a34_MenuWindow,
    EntryMenu_080a7a34,
    menu_window,
    0x10c);
LAYOUT_OFFSET_GUARD(
    EntryMenu080a7a34_RowStyles,
    EntryMenu_080a7a34,
    row_styles,
    0x144);
LAYOUT_OFFSET_GUARD(
    EntryMenu080a7a34_Entries,
    EntryMenu_080a7a34,
    entries,
    0x208);
LAYOUT_OFFSET_GUARD(
    EntryMenu080a7a34_SelectedActor,
    EntryMenu_080a7a34,
    selected_actor,
    0x21a);
LAYOUT_OFFSET_GUARD(
    EntryMenu080a7a34_CursorPhase,
    EntryMenu_080a7a34,
    cursor_phase,
    0x220);
LAYOUT_OFFSET_GUARD(
    EntryMenu080a7a34_CursorX,
    EntryMenu_080a7a34,
    cursor_x,
    0x234);

extern EntryMenu_080a7a34 *Data_03001f2c;
extern volatile u32 Data_03001c94;
extern volatile u32 Data_03001b04;

s8 Func_080022fc(s32, s32);
void Func_080030f8(s32);
void Func_08015080(s32, void *, s32, s32);
void Func_08015270(void *);
void Func_08077008(u16);
s32 Func_080770c0(s32);
void Func_080a1804(EntryMenu_080a7a34 *, u16);
void Func_080a1870(void *, s32, s32, s32, s32);
void Func_080a195c(void);
void Func_080a1a40(s32, s32);
void Func_080a2144(s32);
void Func_080a7850(void);
s32 Func_080a7f44(s32, s32);
void Func_080a8088(u16, s32);
void Func_080f9010(s32);

static void ResetRowStyles_080a7a34(
    EntryMenu_080a7a34 *menu,
    s32 selection)
{
    s32 row;

    for (row = 0; row < 8; row++)
        menu->row_styles[row] = 0x1e;
    menu->row_styles[selection] = 0x1a;
}

static void InitializeCursorPositions_080a7a34(
    EntryMenu_080a7a34 *menu)
{
    s32 index;

    for (index = 0; index < 4; index++) {
        menu->cursor_x[index] = 0x82 + index * 0x20;
        menu->cursor_y[index] = 0x80;
    }
}

static void CopyMenuPalettes_080a7a34(void)
{
    volatile DmaTransfer_080a7a34 *dma =
        (volatile DmaTransfer_080a7a34 *)0x040000d4;

    dma->source = (const void *)0x05000200;
    dma->destination = (void *)0x05000000;
    dma->control = 0x80000010;
    dma->source = (const void *)0x050001c8;
    dma->destination = (void *)0x0500001c;
    dma->control = 0x80000001;
    dma->source = (const void *)0x05000200;
    dma->destination = (void *)0x05000020;
    dma->control = 0x80000010;
    dma->source = (const void *)0x050001e8;
    dma->destination = (void *)0x0500003c;
    dma->control = 0x80000001;
}

/* Run the cyclic entry-selection menu and commit the selected entry. */
s32 Func_080a7a34(void)
{
    EntryMenu_080a7a34 *menu = Data_03001f2c;
    s32 selection = menu->selection;
    s32 entry_count = menu->entry_count;
    s32 redraw = 1;
    s32 result = 0;
    s32 cursor_phase = menu->cursor_phase;

    Func_08077008(menu->entries[selection]);
    InitializeCursorPositions_080a7a34(menu);
    Func_080a2144(0x0e);
    CopyMenuPalettes_080a7a34();

    while (Func_080770c0(0x150) == 0) {
        if (redraw != 0) {
            u16 entry;

            redraw = 0;
            Func_08015270(menu->menu_window);
            Func_08015080(0x0b0d, menu->menu_window, 0, 0);
            if (Func_080770c0(0x30) != 0)
                Func_08015080(0x0b16, menu->menu_window, 0, 0x10);
            Func_08015080(0x0b0a, menu->menu_window, 0, 8);

            selection = Func_080022fc(
                selection + entry_count, entry_count);
            entry = menu->entries[selection];
            Func_08077008(entry);
            cursor_phase = Func_080022fc(cursor_phase + 3, 3);
            Func_080a8088(entry, cursor_phase);
            Func_080a1804(menu, entry);
            ResetRowStyles_080a7a34(menu, selection);
        }

        Func_080a1a40(selection * 24 - 10, 0x10);
        Func_080030f8(1);

        if ((Data_03001c94 & 1) != 0) {
            Func_080f9010(0x70);
            result = 1;
            break;
        }
        if ((Data_03001c94 & 2) != 0) {
            Func_080f9010(0x71);
            result = -1;
            break;
        }

        if ((Data_03001b04 & 0x100) != 0) {
            if (Func_080a7f44(selection, 1) != 0) {
                Func_080f9010(0x70);
                selection++;
                Func_080a195c();
                Func_080a1870(menu->selection_window, 2, 2, 8, 0);
                ResetRowStyles_080a7a34(menu, selection);
            } else {
                Func_080f9010(0x72);
            }
            Func_080030f8(1);
        } else if ((Data_03001b04 & 0x200) != 0) {
            if (Func_080a7f44(selection, 0) != 0) {
                Func_080f9010(0x70);
                selection--;
                Func_080a195c();
                Func_080a1870(menu->selection_window, 2, 2, 8, 0);
                ResetRowStyles_080a7a34(menu, selection);
            } else {
                Func_080f9010(0x72);
            }
            Func_080030f8(1);
        } else if ((Data_03001c94 & 4) != 0 &&
                   Func_080770c0(0x30) != 0) {
            Func_080a7850();
            redraw = 1;
        } else {
            if ((Data_03001b04 & 0x20) != 0) {
                Func_080f9010(0x6f);
                if (entry_count > 1) {
                    redraw = 1;
                    selection--;
                }
            }
            if ((Data_03001b04 & 0x10) != 0) {
                Func_080f9010(0x6f);
                if (entry_count > 1) {
                    redraw = 1;
                    selection++;
                }
            }
        }
    }

    menu->selection = (s8)selection;
    menu->selected_id = menu->entries[selection];
    menu->selected_actor = (s8)menu->entries[selection];
    return result;
}
