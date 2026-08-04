#include "types.h"

struct Window_08025200 {
    u8 unknown_00[8];
    u16 width;
    u8 unknown_0a[2];
    u16 x;
    u16 y;
};

struct Sprite_08025200 {
    u32 unknown_00;
    u8 y;
    u8 unknown_05;
    u16 x;
    u16 tile;
};

struct MenuState_08025200 {
    u8 unknown_00[0x30];
    s32 row;
    s32 column;
    s32 saved_row;
    u8 unknown_3c[0x10];
    s32 active;
};

struct UiGlobals_08025200 {
    u8 unknown_0000[0xea3];
    u8 page_bits;
    u8 unknown_ea4[2];
    u8 drawing;
};

s32 Func_080022ec(s32, s32);
void Func_080030f8(s32);
void Func_0800352c(void);
void Func_08003dec(void *, s32);
void Func_08003f3c(s32);
s32 Func_080040b4(s32);
s32 Func_080040d0(s32, const void *);
void Func_08016418(void *, s32);
void Func_08016498(void *);
void Func_08016738(void);
void *Func_080162d4(s32, s32, s32, s32, s32);
void Func_08017aa4(void *, void *, s32, s32);
void Func_08019000(
    struct Window_08025200 *, s32, s32, s32, s32);
void Func_0801965c(s32, void *, s32);
void Func_0801e71c(s32);
void Func_0801e7c0(s32, void *, s32, s32);
s32 Func_08021af0(s32, s32);
void Func_08022768(s32, s32, s32, s32, s32);
s32 Func_08025180(s32, s32);
void Func_080251d4(s32, s32);
void Func_08077018(s32);
void Func_080f9010(s32);

s32 Func_08025200(s32 category, const u16 *entries, s32 entry_count)
{
    struct UiGlobals_08025200 *ui =
        *(struct UiGlobals_08025200 **)0x03001e8c;
    struct MenuState_08025200 *menu =
        *(struct MenuState_08025200 **)0x03001f34;
    void *heading;
    struct Window_08025200 *window;
    s32 cursor_tile;
    struct Sprite_08025200 item_sprites[5];
    s32 item_tiles[5];
    struct Sprite_08025200 cursor;
    u8 label[52];
    s32 column = menu->column;
    s32 row = menu->row;
    s32 saved_row = menu->saved_row;
    s32 previous_column = -1;
    s32 previous_row = -1;
    s32 visible_count = 0;
    s32 row_offset = row * 2;
    s32 selected;
    s32 i;

    cursor_tile = Func_080040b4(0x80);
    heading = Func_080162d4(0, 5, 30, 4, 42);
    window = Func_080162d4(15, 9, 15, 11, 6);

    for (i = 0; i < 5; i++) {
        struct Sprite_08025200 *sprite = &item_sprites[i];

        sprite->unknown_00 = 0x40000000;
        sprite->tile = 0;
        sprite->x =
            (sprite->x & 0xfe00) |
            ((window->x * 8 + 8) & 0x1ff);
        sprite->y = (u8)((window->y + i * 2) * 8 + 4);
    }

    for (i = 0; i < 5; i++) {
        item_tiles[i] = Func_080040b4(0x80);
        item_sprites[i].tile =
            (item_sprites[i].tile & 0xfc00) |
            (Func_080040d0(item_tiles[i], (const void *)-1) &
             0x3ff);
    }

    Func_080251d4(0xf018, 0x200);
    Func_080251d4(0xf018, 0x201);
    Func_080251d4(0xf019, 0x210);
    Func_080251d4(0xf019, 0x211);

    for (;;) {
        if (column != previous_column || row != previous_row) {
            const u16 *row_entries;

            ui->drawing = 1;
            Func_08022768(window->x + 1,
                window->y + previous_row * 2 + 1,
                window->width - 2, 1, 15);
            Func_08016738();

            if (entry_count != 0) {
                u16 entry = entries[column + row];
                s32 kind = Func_08025180(category, entry);
                s32 message;

                if (kind == 2)
                    message = 0x8ee;
                else
                    message = (entry & 0x1ff) + 0x75;
                Func_0801965c(message, label, 52);
            } else {
                Func_0801965c(0x8e5, label, 52);
            }
            Func_08017aa4(label, heading, 0, 4);
            previous_row = row;

            if (column != previous_column) {
                const u16 *entry = &entries[column];

                Func_08016498(window);
                visible_count = 0;
                while (visible_count < 5 && *entry != 0) {
                    u16 value = *entry;
                    s32 kind;

                    Func_08077018(value);
                    Func_0801e71c(15);
                    kind = Func_08025180(category, value);
                    if (kind != 0)
                        Func_0801e71c(4);
                    else if ((value & 0x400) != 0)
                        Func_0801e71c(2);
                    Func_0801e7c0(
                        (value & 0x1ff) + 0x182,
                        window, 16, visible_count * 16);
                    Func_0801e71c(15);
                    item_sprites[visible_count].tile =
                        (item_sprites[visible_count].tile &
                         0xfc00) |
                        (Func_08021af0(
                             value, item_tiles[visible_count]) &
                         0x3ff);
                    visible_count++;
                    entry++;
                }
                previous_column = column;
            }

            if (entry_count > 5) {
                s32 page_count = Func_080022ec(entry_count + 4, 5);

                for (i = 0; i < page_count; i++) {
                    s32 tile = i + 0xf301;

                    if (i == Func_080022ec(column, 5))
                        tile = i + 0xf30b;
                    Func_08019000(window, tile,
                        window->width - page_count + i - 2,
                        -1, 0);
                }
            }

            Func_08022768(window->x + 1,
                window->y + row_offset + 1,
                window->width - 2, 1, 14);
            ui->page_bits = 1;
            ui->drawing = 0;
        }

        if (entry_count > 5) {
            s32 page_count = Func_080022ec(entry_count + 4, 5);

            for (i = 0; i < page_count; i++) {
                s32 tile = i + 0xf301;

                if ((u32)(*(s32 *)0x03001e40 & 15) <= 11 &&
                    i == Func_080022ec(column, 5))
                    tile = i + 0xf30b;
                Func_08019000(window, tile,
                    window->width -
                        Func_080022ec(entry_count + 4, 5) +
                        i - 2,
                    -1, 0);
            }
            Func_08019000(window, 0xf334,
                window->width - page_count - 3, -1, 0);
            Func_08019000(
                window, 0xf335, window->width - 2, -1, 0);
            ui->page_bits |=
                2 << ((u32)(window->y - 1) >> 2);
        }

        for (i = 0; i < visible_count; i++)
            Func_08003dec(&item_sprites[i], 0xf0);

        cursor.unknown_00 = 0x40000000;
        cursor.tile = 0;
        cursor.tile =
            (cursor.tile & 0xfc00) |
            (Func_080040d0(cursor_tile,
                 (const void *)0x080310a4) &
             0x3ff);
        cursor.x =
            (cursor.x & 0xfe00) |
            ((window->x * 8 - 2 +
                 ((*(u32 *)0x03001e40 & 4) >> 1) -
                 4) &
             0x1ff);
        cursor.y =
            (u8)(((window->y + row_offset) * 8 + 20) -
                 ((*(u32 *)0x03001e40 & 4) >> 2) + 248);
        if (entry_count != 0)
            Func_08003dec(&cursor, 0xf2);

        menu->column = column;
        menu->row = row;
        menu->saved_row = saved_row;

        if ((*(u32 *)0x03001c94 & 1) != 0) {
            if (entry_count != 0) {
                u16 entry = entries[column + row];
                s32 kind;

                selected = column + row;
                Func_08077018(entry);
                kind = 0;
                if ((entry & 0x400) == 0) {
                    kind = Func_08025180(category, entry);
                    if (kind == 0)
                        goto selection_done;
                }

                Func_080f9010(0x72);
                if (kind == 2)
                    Func_0801965c(0x8ee, label, 52);
                else if ((entry & 0x400) != 0)
                    Func_0801965c(0x8ec, label, 52);
                else
                    Func_0801965c(0x8eb, label, 52);
                Func_08016738();
                Func_08017aa4(label, heading, 0, 4);
                goto wait_for_input;
            } else {
                selected = -1;
            }
        } else if (menu->active == 0 ||
                   (*(u32 *)0x03001c94 & 2) != 0) {
            Func_080f9010(0x71);
            selected = -1;
        } else {
wait_for_input:
            if (entry_count != 0) {
                u32 keys = *(u32 *)0x03001b04;

                if ((keys & 0x80) != 0) {
                    Func_080f9010(0x6f);
                    row++;
                    if (row == 5 || column + row == entry_count)
                        row = 0;
                    saved_row = row;
                    row_offset = row * 2;
                } else if ((keys & 0x40) != 0) {
                    Func_080f9010(0x6f);
                    row--;
                    if (row < 0) {
                        if (column ==
                            Func_080022ec(entry_count - 1, 5) *
                                5)
                            row = entry_count - column - 1;
                        else
                            row = 4;
                    }
                    saved_row = row;
                    row_offset = row * 2;
                } else if ((keys & 0x10) != 0) {
                    s32 next_column;

                    Func_080f9010(0x6f);
                    Func_0800352c();
                    next_column = column + 5;
                    if (next_column >= entry_count) {
                        if (column != 0) {
                            row = saved_row;
                            column = 0;
                        }
                    } else {
                        column = next_column;
                        row = saved_row;
                        if (column ==
                            Func_080022ec(entry_count - 1, 5) *
                                5) {
                            s32 final_row =
                                entry_count - column - 1;

                            if (final_row < row)
                                row = final_row;
                        }
                    }
                    row_offset = row * 2;
                } else if ((keys & 0x20) != 0) {
                    Func_080f9010(0x6f);
                    Func_0800352c();
                    if (column != 0) {
                        row = saved_row;
                        column -= 5;
                    } else {
                        column =
                            Func_080022ec(entry_count - 1, 5) *
                            5;
                        row = saved_row;
                        if (column != 0) {
                            s32 final_row =
                                entry_count - column - 1;

                            if (final_row < row)
                                row = final_row;
                        }
                    }
                    row_offset = row * 2;
                }
            }
            Func_080030f8(1);
            continue;
        }

selection_done:
        Func_08016418(heading, 1);
        Func_08016418(window, 1);
        Func_080030f8(1);
        Func_08003f3c(cursor_tile);
        for (i = 0; i < 5; i++)
            Func_08003f3c(item_tiles[i]);
        Func_080030f8(1);
        return selected;
    }
}
