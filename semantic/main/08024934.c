#include "types.h"

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

struct Window {
    u8 pad00[8];
    u16 width;
    u16 height;
    u16 x;
    u16 y;
};

struct Sprite {
    u8 y;
    u8 pad01[3];
    u8 x;
    u8 pad05;
    u16 attr1;
    u16 attr2;
    u16 pad0a;
};

struct Work_80190000;

void Func_080030f8(s32);
void Func_0800352c(void);
void Func_08003dec(void *, s32);
void Func_08003f3c(u32);
void *Func_080040b4(s32);
s32 Func_080040d0(s32, s32);
struct Window *Func_080162d4(s32, s32, s32, s32, s32);
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
u16 *Func_080771e0(u8);
void Func_080f9010(s32);

static s32 Div4TowardZero(s32 value)
{
    if (value < 0)
        value += 3;
    return value >> 2;
}

s32 Func_08024934(s32 arg0, s32 arg1, u8 *key)
{
    u8 *engine = *(u8 **)0x03001e8c;
    struct Window *heading;
    struct Window *keyWindow;
    struct Window *listWindow;
    struct Sprite rowSprites[4];
    struct Sprite cursor;
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
    u8 *state;

    (void)arg0;
    (void)arg1;

    heading = Func_080162d4(0, 4, 30, 4, 42);
    keyWindow = Func_080162d4(20, 8, 10, 3, 6);
    listWindow = Func_080162d4(13, 11, 17, 9, 6);

    state = *(u8 **)0x03001f34;
    row = *(s32 *)(state + 0x34);
    column = *(s32 *)(state + 0x30);
    savedColumn = *(s32 *)(state + 0x38);

    for (i = 0; i < 4; i++) {
        *(u32 *)((u8 *)&rowSprites[i] + 4) = 0x40000000;
        *(u32 *)((u8 *)&rowSprites[i] + 8) = 0;
        rowSprites[i].x = (u8)(((i * 2 + listWindow->y) * 8) + 4);
        rowSprites[i].attr1 =
            (rowSprites[i].attr1 & 0xfe00) | ((listWindow->x * 8 + 8) & 0x1ff);
        rowSprites[i].attr2 = 0;
        graphics[i] = (u32)Func_080040b4(0x80);
        rowSprites[i].attr2 =
            (rowSprites[i].attr2 & 0xfc00) |
            (Func_080040d0(graphics[i], -1) & 0x3ff);
    }

    count = Func_080771d8(ordered);
    last = count - 1;
    j = 0;
    for (i = last; i >= 0; i--) {
        u8 entry = ordered[i];
        u8 *record = (u8 *)Func_080771e0(entry);
        for (result = 0; result < 4 && record[4 + result] <= key[result]; result++)
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
        u16 *record;

        if (row != -1 || column != -1) {
            engine[0xea6] = 1;
            Func_08022768(listWindow->x + 1, listWindow->y + column * 2 + 1,
                          listWindow->width - 2, 1, 15);
            Func_08016738();

            entry = visible[row + column];
            record = Func_080771e0(entry);
            Func_0801965c(record[0] + 0x53a, text, 52);
            Func_08017aa4(text, heading, 0, 4);

            selectionMask = 0;
            for (i = 0; i < 4; i++) {
                if (((u8 *)record)[4 + i] != 0)
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
                         match < 4 && ((u8 *)record)[4 + match] <= key[match];
                         match++)
                        ;

                    value = 0;
                    rendered = 0;
                    Func_0801a3d0(record[0] & 0x3fff, 0, &graphics[i],
                                  &rendered, match == 4);
                    tile = rendered & 0x3ff;
                    rowSprites[i].attr2 =
                        (rowSprites[i].attr2 & 0xfc00) | tile;
                    if (match != 4)
                        Func_0801e71c(2);
                    record = Func_080771e0(entry);
                    Func_0801e7c0(record[0] + 0x333, listWindow, 16, i * 16);

                    for (j = 0, value = 13; j < 4; j++) {
                        if (((u8 *)record)[4 + j] != 0) {
                            Func_08019000((struct Work_80190000 *)listWindow,
                                          0x5001 + j, value, i * 2, 0);
                            Func_08018efc(listWindow,
                                          ((u8 *)record)[4 + j] + 0x30,
                                          value + 1, i * 2, 0);
                            value += 2;
                        }
                    }
                    Func_0801e71c(15);
                    active[i] = 1;
                }
                while (i < 4)
                    active[i++] = 0;
                row = *(s32 *)(state + 0x34);
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
            engine[0xea3] = 1;
            engine[0xea6] = 0;
        }

        for (i = 0; i < 4; i++) {
            if (active[i])
                Func_08003dec(&rowSprites[i], 0xf0);
        }

        *(u32 *)((u8 *)&cursor + 4) = 0x40000000;
        *(u32 *)((u8 *)&cursor + 8) = 0;
        cursor.x = (u8)((listWindow->y + pageWidth) * 8 + 20);
        cursor.attr2 = (cursor.attr2 & 0xfc00) |
                       (Func_080040d0((u16)initialTile, 0x080310a4) & 0x3ff);
        cursor.attr1 =
            (cursor.attr1 & 0xfe00) |
            (((listWindow->x * 8 - 2) +
              ((*(u32 *)0x03001e40 & 4) >> 1) - 4) & 0x1ff);
        cursor.x =
            (u8)(cursor.x - ((*(u32 *)0x03001e40 & 4) >> 2) + 248);
        Func_08003dec(&cursor, 0xf2);

        for (i = 0; i < 4; i++) {
            s32 palette = (*(u32 *)0x03001e40 & 8) ? 14 : 15;
            if (!(selectionMask & (1 << i)))
                palette = 15;
            Func_08022768(keyWindow->x + i * 2 + 1, keyWindow->y + 1,
                          2, 1, palette);
        }

        if (count > 4) {
            s32 rows = Div4TowardZero(count + 3);
            for (i = 0; i < rows; i++) {
                s32 glyph = 0xf301 + i;
                if ((*(u32 *)0x03001e40 & 15) <= 11 &&
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
            engine[0xea3] |= 2 << ((listWindow->y - 1) >> 2);
        }

        state = *(u8 **)0x03001f34;
        *(s32 *)(state + 0x34) = row;
        *(s32 *)(state + 0x30) = column;
        *(s32 *)(state + 0x38) = savedColumn;

        {
            s32 repeat = *(s32 *)0x03001c94;
            s32 pressed = *(s32 *)0x03001b04;
            if (*(s32 *)(state + 0xd8) != 0) {
                pressed = repeat = 0;
                if (*(s32 *)(state + 0xdc) == 0) {
                    *(s32 *)(state + 0xdc) = 120;
                    pressed = repeat = 1;
                } else {
                    --*(s32 *)(state + 0xdc);
                }
            }

            if (repeat & 1) {
                result = visible[row + column];
                break;
            }
            if (*(s32 *)(state + 0x4c) == 0 || (repeat & 2)) {
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
