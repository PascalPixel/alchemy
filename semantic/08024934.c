#include "layout_guard.h"
#include "types.h"

struct Window_08024934 {
    u8 pad00[8];
    u16 width;
    u16 height;
    u16 x;
    u16 y;
};

struct SpriteSlot_08024934 {
    u8 padding00[4];
    u8 oam_y;
    u8 oam_mode;
    u16 oam_attr1;
    u16 oam_attr2;
    u16 padding0a;
};

struct EntryRecord_08024934 {
    u16 label;
    u8 padding02[2];
    u8 requirements[4];
};

struct MenuRuntime_08024934 {
    u8 padding000[0x30];
    s32 column;
    s32 row;
    s32 saved_column;
    u8 padding03c[0x10];
    s32 allow_cancel;
    u8 padding050[0x88];
    s32 automatic_input;
    s32 automatic_timer;
};

struct FieldUiState_08024934 {
    u8 padding000[0xea3];
    u8 window_flags;
    u8 paddingea4[2];
    u8 redraw_busy;
};

struct Work_80190000;

LAYOUT_OFFSET_GUARD(
    Window08024934_Width,
    struct Window_08024934,
    width,
    0x08);
LAYOUT_OFFSET_GUARD(
    Window08024934_X,
    struct Window_08024934,
    x,
    0x0c);
LAYOUT_SIZE_GUARD(
    SpriteSlot08024934_Size,
    struct SpriteSlot_08024934,
    0x0c);
LAYOUT_OFFSET_GUARD(
    SpriteSlot08024934_Y,
    struct SpriteSlot_08024934,
    oam_y,
    0x04);
LAYOUT_OFFSET_GUARD(
    SpriteSlot08024934_Attr1,
    struct SpriteSlot_08024934,
    oam_attr1,
    0x06);
LAYOUT_OFFSET_GUARD(
    EntryRecord08024934_Requirements,
    struct EntryRecord_08024934,
    requirements,
    0x04);
LAYOUT_OFFSET_GUARD(
    MenuRuntime08024934_Column,
    struct MenuRuntime_08024934,
    column,
    0x30);
LAYOUT_OFFSET_GUARD(
    MenuRuntime08024934_AllowCancel,
    struct MenuRuntime_08024934,
    allow_cancel,
    0x4c);
LAYOUT_OFFSET_GUARD(
    MenuRuntime08024934_AutomaticInput,
    struct MenuRuntime_08024934,
    automatic_input,
    0xd8);
LAYOUT_OFFSET_GUARD(
    FieldUiState08024934_WindowFlags,
    struct FieldUiState_08024934,
    window_flags,
    0xea3);
LAYOUT_OFFSET_GUARD(
    FieldUiState08024934_RedrawBusy,
    struct FieldUiState_08024934,
    redraw_busy,
    0xea6);

extern struct FieldUiState_08024934 *Data_03001e8c;
extern struct MenuRuntime_08024934 *Data_03001f34;
extern u32 Data_03001e40;
extern u32 Data_03001c94;
extern u32 Data_03001b04;

void Func_080030f8(s32);
void Func_0800352c(void);
void Func_08003dec(void *, s32);
void Func_08003f3c(u32);
void *Func_080040b4(s32);
s32 Func_080040d0(s32, s32);
struct Window_08024934 *Func_080162d4(s32, s32, s32, s32, s32);
void Func_08016418(void *, s32);
void Func_08016498(void *);
void Func_08016738(void);
void Func_08017aa4(void *, void *, s32, s32);
void Func_08018efc(void *, s32, s32, s32, s32);
void Func_08019000(struct Work_80190000 *, s32, s32, s32, s32);
s32 Func_0801965c(s32, s16 *, s32);
void Func_0801a3d0(s32, s32, s32 *, s32 *, s32);
void Func_0801e71c(s32);
void Func_0801e7c0(s32, void *, s32, s32);
void Func_08022768(s32, s32, s32, s32, s32);
s32 Func_080771d8(u8 *);
struct EntryRecord_08024934 *Func_080771e0(u8);
void Func_080f9010(s32);

static s32 Div4TowardZero(s32 value)
{
    if (value < 0)
        value += 3;
    return value >> 2;
}

s32 Func_08024934(s32 arg0, s32 arg1, u8 *key)
{
    struct FieldUiState_08024934 *engine = Data_03001e8c;
    struct Window_08024934 *heading;
    struct Window_08024934 *keyWindow;
    struct Window_08024934 *listWindow;
    struct SpriteSlot_08024934 rowSprites[4];
    struct SpriteSlot_08024934 cursor;
    u32 graphics[4];
    s16 text[26];
    u8 visible[96];
    u8 ordered[96];
    u8 active[4] = {0, 0, 0, 0};
    s32 selectionMask = 0;
    s32 initialTile = (s16)(s32)Func_080040b4(0x80);
    s32 row = -1;
    s32 column = -1;
    s32 savedColumn;
    s32 count;
    s32 last;
    s32 result;
    s32 i;
    s32 j;
    s32 pageWidth;
    struct MenuRuntime_08024934 *state;

    (void)arg0;
    (void)arg1;

    heading = Func_080162d4(0, 4, 30, 4, 42);
    keyWindow = Func_080162d4(20, 8, 10, 3, 6);
    listWindow = Func_080162d4(13, 11, 17, 9, 6);

    state = Data_03001f34;
    row = state->row;
    column = state->column;
    savedColumn = state->saved_column;

    for (i = 0; i < 4; i++) {
        rowSprites[i].oam_y = 0;
        rowSprites[i].oam_mode = 0;
        rowSprites[i].oam_attr1 = 0x4000;
        rowSprites[i].oam_attr2 = 0;
        rowSprites[i].padding0a = 0;
        rowSprites[i].oam_y = (u8)(((i * 2 + listWindow->y) * 8) + 4);
        rowSprites[i].oam_attr1 =
            (rowSprites[i].oam_attr1 & 0xfe00) |
            ((listWindow->x * 8 + 8) & 0x1ff);
        graphics[i] = (u32)Func_080040b4(0x80);
        rowSprites[i].oam_attr2 =
            (rowSprites[i].oam_attr2 & 0xfc00) |
            (Func_080040d0(graphics[i], -1) & 0x3ff);
    }

    count = Func_080771d8(ordered);
    last = count - 1;
    j = 0;
    for (i = last; i >= 0; i--) {
        u8 entry = ordered[i];
        struct EntryRecord_08024934 *record = Func_080771e0(entry);
        for (result = 0;
             result < 4 && record->requirements[result] <= key[result];
             result++)
            ;
        if (result == 4) {
            visible[j++] = entry;
            ordered[i] = 0x20;
        }
    }
    for (i = 0; i < count; i++) {
        if (ordered[i] != 0x20)
            visible[j++] = ordered[i];
    }
    visible[j] = 0x20;

    pageWidth = column * 2;
    for (;;) {
        u8 entry;
        struct EntryRecord_08024934 *record;

        if (row != -1 || column != -1) {
            engine->redraw_busy = 1;
            Func_08022768(listWindow->x + 1, listWindow->y + column * 2 + 1,
                          listWindow->width - 2, 1, 15);
            Func_08016738();

            entry = visible[row + column];
            record = Func_080771e0(entry);
            Func_0801965c(record->label + 0x53a, text, 52);
            Func_08017aa4(text, heading, 0, 4);

            selectionMask = 0;
            for (i = 0; i < 4; i++) {
                if (record->requirements[i] != 0)
                    selectionMask |= 1 << i;
            }

            if (row != -1) {
                Func_08016498(listWindow);
                for (i = 0; i < 4; i++) {
                    Func_08019000((struct Work_80190000 *)keyWindow,
                                  0x5001 + i, i * 2, 0, 0);
                    Func_08018efc(keyWindow, key[i] + 0x30, i * 2 + 1, 0, 0);
                }

                for (i = 0; i < 4; i++) {
                    s32 match;
                    s32 tile;
                    s32 rendered;
                    s32 value;

                    entry = visible[row + i];
                    if (entry == 0x20)
                        break;
                    record = Func_080771e0(entry);
                    for (match = 0;
                         match < 4 && record->requirements[match] <= key[match];
                         match++)
                        ;

                    value = 0;
                    rendered = 0;
                    Func_0801a3d0(record->label & 0x3fff, 0, &graphics[i],
                                  &rendered, match == 4);
                    tile = rendered & 0x3ff;
                    rowSprites[i].oam_attr2 =
                        (rowSprites[i].oam_attr2 & 0xfc00) | tile;
                    if (match != 4)
                        Func_0801e71c(2);
                    record = Func_080771e0(entry);
                    Func_0801e7c0(
                        record->label + 0x333, listWindow, 16, i * 16);

                    for (j = 0, value = 13; j < 4; j++) {
                        if (record->requirements[j] != 0) {
                            Func_08019000((struct Work_80190000 *)listWindow,
                                          0x5001 + j, value, i * 2, 0);
                            Func_08018efc(listWindow,
                                          record->requirements[j] + 0x30,
                                          value + 1, i * 2, 0);
                            value += 2;
                        }
                    }
                    Func_0801e71c(15);
                    active[i] = 1;
                }
                while (i < 4)
                    active[i++] = 0;
                row = state->row;
            }

            if (count > 4) {
                s32 rows = Div4TowardZero(count + 3);
                for (i = 0; i < rows; i++) {
                    s32 glyph = 0xf301 + i;
                    if (i == Div4TowardZero(row))
                        glyph = 0xf30b + i;
                    Func_08019000((struct Work_80190000 *)listWindow, glyph,
                                  listWindow->width - rows + i - 2, -1, 0);
                }
            }

            Func_08022768(listWindow->x + 1, listWindow->y + pageWidth + 1,
                          listWindow->width - 2, 1, 14);
            engine->window_flags = 1;
            engine->redraw_busy = 0;
        }

        for (i = 0; i < 4; i++) {
            if (active[i])
                Func_08003dec(&rowSprites[i], 0xf0);
        }

        cursor.oam_y = 0;
        cursor.oam_mode = 0;
        cursor.oam_attr1 = 0x4000;
        cursor.oam_attr2 = 0;
        cursor.padding0a = 0;
        cursor.oam_y = (u8)((listWindow->y + pageWidth) * 8 + 20);
        cursor.oam_attr2 = (cursor.oam_attr2 & 0xfc00) |
                       (Func_080040d0((u16)initialTile, 0x080310a4) & 0x3ff);
        cursor.oam_attr1 =
            (cursor.oam_attr1 & 0xfe00) |
            (((listWindow->x * 8 - 2) +
              ((Data_03001e40 & 4) >> 1) - 4) & 0x1ff);
        cursor.oam_y =
            (u8)(cursor.oam_y - ((Data_03001e40 & 4) >> 2) + 248);
        Func_08003dec(&cursor, 0xf2);

        for (i = 0; i < 4; i++) {
            s32 palette = (Data_03001e40 & 8) ? 14 : 15;
            if (!(selectionMask & (1 << i)))
                palette = 15;
            Func_08022768(keyWindow->x + i * 2 + 1, keyWindow->y + 1,
                          2, 1, palette);
        }

        if (count > 4) {
            s32 rows = Div4TowardZero(count + 3);
            for (i = 0; i < rows; i++) {
                s32 glyph = 0xf301 + i;
                if ((Data_03001e40 & 15) <= 11 &&
                    i == Div4TowardZero(row))
                    glyph = 0xf30b + i;
                Func_08019000((struct Work_80190000 *)listWindow, glyph,
                                  listWindow->width - rows + i - 2, -1, 0);
            }
            Func_08019000((struct Work_80190000 *)listWindow, 0xf334,
                              listWindow->width - Div4TowardZero(count + 3) - 3,
                              -1, 0);
            Func_08019000((struct Work_80190000 *)listWindow, 0xf335,
                              listWindow->width - 2, -1, 0);
            engine->window_flags |= 2 << ((listWindow->y - 1) >> 2);
        }

        state = Data_03001f34;
        state->row = row;
        state->column = column;
        state->saved_column = savedColumn;

        {
            s32 repeat = Data_03001c94;
            s32 pressed = Data_03001b04;
            if (state->automatic_input != 0) {
                pressed = repeat = 0;
                if (state->automatic_timer == 0) {
                    state->automatic_timer = 120;
                    pressed = repeat = 1;
                } else {
                    state->automatic_timer--;
                }
            }

            if (repeat & 1) {
                result = visible[row + column];
                break;
            }
            if (state->allow_cancel == 0 || (repeat & 2)) {
                Func_080f9010(113);
                result = -1;
                break;
            }
            if (pressed & 0x80) {
                Func_080f9010(111);
                column++;
                if (column == 4 || row + column == count)
                    column = 0;
                savedColumn = column;
                pageWidth = column * 2;
            } else if (pressed & 0x40) {
                Func_080f9010(111);
                column--;
                if (column < 0) {
                    if (row == Div4TowardZero(last) * 4)
                        column = count - row - 1;
                    else
                        column = 3;
                }
                savedColumn = column;
                pageWidth = column * 2;
            } else if (pressed & 0x10) {
                Func_080f9010(111);
                Func_0800352c();
                if (row + 4 >= count) {
                    if (row != 0) {
                        column = savedColumn;
                        row = 0;
                    }
                } else {
                    row += 4;
                    column = savedColumn;
                    if (row == Div4TowardZero(last) * 4) {
                        column = count - row - 1;
                        if (column > savedColumn)
                            column = savedColumn;
                    }
                }
                pageWidth = column * 2;
            } else if (pressed & 0x20) {
                Func_080f9010(111);
                Func_0800352c();
                if (row != 0) {
                    column = savedColumn;
                    row -= 4;
                } else {
                    row = Div4TowardZero(last) * 4;
                    column = savedColumn;
                    if (row != 0) {
                        column = count - row - 1;
                        if (column > savedColumn)
                            column = savedColumn;
                    }
                }
                pageWidth = column * 2;
            }
        }
        Func_080030f8(1);
    }

    Func_080030f8(1);
    for (i = 0; i < 4; i++)
        Func_08003f3c(graphics[i]);
    Func_08003f3c((u16)initialTile);
    Func_08016418(keyWindow, 1);
    Func_08016418(heading, 1);
    Func_08016418(listWindow, 1);
    Func_080030f8(1);
    return result;
}
