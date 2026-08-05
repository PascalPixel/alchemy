#include "layout_guard.h"
#include "types.h"

typedef struct Selection_080a93a4 {
    u8 padding00[8];
    s32 row;
    u8 padding0c[4];
    s32 column;
    u8 padding14[4];
    s32 absolute;
} Selection_080a93a4;

typedef struct MenuState_080a93a4 {
    u8 padding000[0x2c];
    s32 description_window;
    s32 baseline_window;
    u8 padding034[0x194];
    u16 item_ids[1];
} MenuState_080a93a4;

typedef struct ItemDefinition_080a93a4 {
    u8 padding00[2];
    u8 kind;
    u8 padding03[0x11];
    s32 display_type;
} ItemDefinition_080a93a4;

LAYOUT_OFFSET_GUARD(
    Selection080a93a4_Row,
    Selection_080a93a4,
    row,
    0x08);
LAYOUT_OFFSET_GUARD(
    Selection080a93a4_Column,
    Selection_080a93a4,
    column,
    0x10);
LAYOUT_OFFSET_GUARD(
    Selection080a93a4_Absolute,
    Selection_080a93a4,
    absolute,
    0x18);
LAYOUT_OFFSET_GUARD(
    MenuState080a93a4_ItemIds,
    MenuState_080a93a4,
    item_ids,
    0x1c8);
LAYOUT_OFFSET_GUARD(
    ItemDefinition080a93a4_DisplayType,
    ItemDefinition_080a93a4,
    display_type,
    0x14);

void Func_080030f8(s32);
void Func_08015080(s32, s32, s32, s32);
void Func_08015270(s32);
void Func_08015280(s32, s32, s32, s32, s32);
ItemDefinition_080a93a4 *Func_08077018(s32);
void Func_080a2268(s32, s32, s32, s32, s32, s32);

/*
 * Render five equipment entries and the secondary four-row category marker
 * for the currently selected item.
 */
s32 Func_080a93a4(
    s32 window,
    s32 unused,
    Selection_080a93a4 *selection)
{
    MenuState_080a93a4 *state =
        *(MenuState_080a93a4 **)0x03001f2c;
    u16 selected_id;
    u16 *row_items;
    u32 i;

    (void)unused;
    selection->absolute = selection->row * 5 + selection->column;
    Func_08015270(state->description_window);
    Func_080030f8(1);

    selected_id = state->item_ids[selection->absolute];
    if (selected_id != 0) {
        Func_08015080(
            (selected_id & 0x01ff) + 0x75,
            state->description_window,
            0,
            0);
    }

    row_items = &state->item_ids[selection->row * 5];
    for (i = 0; i < 5; i++) {
        s32 y = 1 + i * 2;

        if (i == selection->column) {
            ItemDefinition_080a93a4 *definition =
                Func_08077018(row_items[i] & 0x01ff);

            if (definition->display_type != 4) {
                Func_08015280(
                    window,
                    definition->display_type + 1,
                    27,
                    y,
                    0);
                Func_080a2268(window, 14, y, 13, 1, 14);
            } else {
                Func_080a2268(window, 14, y, 14, 1, 14);
            }
        } else {
            ItemDefinition_080a93a4 *definition =
                Func_08077018(row_items[i] & 0x01ff);

            if (definition->display_type != 4) {
                Func_08015280(
                    window,
                    definition->display_type + 1,
                    27,
                    y,
                    4);
                Func_080a2268(window, 14, y, 13, 1, 15);
            } else {
                Func_080a2268(window, 14, y, 14, 1, 15);
            }
        }
    }

    for (i = 0; i < 4; i++)
        Func_080a2268(state->baseline_window, 1, 1 + i * 2, 12, 1, 15);

    if ((selected_id & 0x0200) != 0) {
        ItemDefinition_080a93a4 *definition =
            Func_08077018(selected_id & 0x01ff);
        s32 marker_row = -1;

        switch (definition->kind) {
        case 1:
            marker_row = 1;
            break;
        case 4:
            marker_row = 3;
            break;
        case 3:
            marker_row = 5;
            break;
        case 2:
            marker_row = 7;
            break;
        }
        if (marker_row >= 0) {
            Func_080a2268(
                state->baseline_window,
                1,
                marker_row,
                12,
                1,
                14);
        }
    }

    Func_080030f8(1);
    return 1;
}
