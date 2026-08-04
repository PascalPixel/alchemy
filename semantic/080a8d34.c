#include "layout_guard.h"
#include "types.h"

typedef struct Selection_080a8d34 {
    u8 padding00[8];
    s32 row;
    u8 padding0c[4];
    s32 column;
    u8 padding14[4];
    s32 absolute;
} Selection_080a8d34;

typedef struct MenuState_080a8d34 {
    u8 padding000[0x2c];
    s32 description_window;
    u8 padding030[0x198];
    u16 item_ids[1];
} MenuState_080a8d34;

typedef struct ItemMetadata_080a8d34 {
    u8 padding00;
    u8 flags;
    u8 type;
    u8 padding03[9];
    u8 category;
} ItemMetadata_080a8d34;

LAYOUT_OFFSET_GUARD(
    Selection080a8d34_Row,
    Selection_080a8d34,
    row,
    0x08);
LAYOUT_OFFSET_GUARD(
    Selection080a8d34_Column,
    Selection_080a8d34,
    column,
    0x10);
LAYOUT_OFFSET_GUARD(
    Selection080a8d34_Absolute,
    Selection_080a8d34,
    absolute,
    0x18);
LAYOUT_OFFSET_GUARD(
    MenuState080a8d34_ItemIds,
    MenuState_080a8d34,
    item_ids,
    0x1c8);

void Func_080030f8(s32);
void Func_08015068(s32, s32, s32, s32, s32);
void Func_08015080(s32, s32, s32, s32);
void Func_08015270(s32);
void Func_08015280(s32, s32, s32, s32, s32);
ItemMetadata_080a8d34 *Func_08077080(s32);
void Func_080a2268(s32, s32, s32, s32, s32, s32);

/*
 * Render the five inventory entries on the selected row, including the
 * selected entry's description and status markers.
 */
s32 Func_080a8d34(
    s32 window,
    s32 unused,
    Selection_080a8d34 *selection)
{
    MenuState_080a8d34 *state =
        *(MenuState_080a8d34 **)0x03001f2c;
    u16 *row_items;
    u16 selected_id;
    s32 i;

    (void)unused;
    selection->absolute = selection->row * 5 + selection->column;
    Func_08015270(state->description_window);
    Func_080030f8(1);

    selected_id = state->item_ids[selection->absolute];
    if (selected_id != 0) {
        ItemMetadata_080a8d34 *metadata;
        s32 status = 0;
        s32 status_resource;

        Func_08015080(
            (selected_id & 0x3fff) + 0x53a,
            state->description_window,
            0,
            0);
        metadata = Func_08077080(selected_id & 0x3fff);
        Func_08015068(window, 0, 96, 224, 104);

        if (metadata->category != 0 || (metadata->flags & 0x40) != 0)
            status = 2;
        if ((metadata->flags & 0x80) != 0)
            status |= 1;

        if (status == 2 || status == 3) {
            status_resource = status == 3 ? 0x0b15 : 0x0b14;
            Func_08015080(status_resource, window, 0, 96);
        } else if (status == 1) {
            Func_08015080(0x0b13, window, 0, 96);
        }
    }

    row_items = &state->item_ids[selection->row * 5];
    for (i = 0; i < 5; i++) {
        s32 x = 2 + i * 2;
        s32 palette;
        s32 needs_strip = 0;

        if (i == selection->column) {
            ItemMetadata_080a8d34 *metadata =
                Func_08077080(row_items[i] & 0x3fff);

            palette = 14;
            if (metadata->type != 4) {
                Func_08015280(window, metadata->type + 1, 24, x, 0);
                Func_080a2268(window, 9, x, 15, 1, 14);
                needs_strip = 1;
            } else {
                Func_080a2268(window, 9, x, 19, 1, 14);
            }
        } else {
            ItemMetadata_080a8d34 *metadata =
                Func_08077080(row_items[i] & 0x3fff);

            palette = 15;
            if (metadata->type != 4) {
                Func_08015280(window, metadata->type + 1, 24, x, 4);
                Func_080a2268(window, 9, x, 15, 1, 15);
                needs_strip = 1;
            } else {
                Func_080a2268(window, 9, x, 19, 1, 15);
            }
        }

        if (needs_strip != 0)
            Func_080a2268(window, 25, x, 3, 1, palette);
    }

    Func_080030f8(1);
    return 1;
}
