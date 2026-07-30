#include "types.h"

#define U8_AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define U16_AT(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define S16_AT(p, o) (*(s16 *)((u8 *)(p) + (o)))

void *Func_08004938(s32);
void Func_08002df0(void *);
void Func_080072f0(void *, const void *, s32, void *);
void Func_08015070(void *, s32, s32, s32, s32);
void Func_08015080(s32, void *, s32, s32);
void Func_08015090(const void *, void *, s32, s32);
void Func_080150a8(s32, s32, void *, s32, s32);
void Func_080150b0(s32, s32, void *, s32, s32);
void Func_080150b8(s32);
void Func_080150d8(s32, s32, s32, void *, s32, s32);
void Func_08015280(void *, s32, s32, s32, s32);
void *Func_08077008(s32);
void Func_08077010(s32);
void *Func_08077080(s32);
void Func_080771a8(s32, s32, s32);
void Func_080771b0(s32, s32, s32);
void Func_080771b8(s32, s32, s32);
s32 Func_080aae14(u16 *, u16 *, s16 *, s32 *, s32 *);
void Func_080ae958(void *, s32, s32, s32);
void Func_080ae9f0(void *, s32, s32, s32);

s32 Func_080acab8(
    void *window, s32 base_x, s32 base_y, u8 character_id,
    s32 first_index, s32 second_index, s32 operation,
    s32 comparison_row, s32 draw_portrait)
{
    u8 *character;
    u8 *work;
    u8 *party = *(u8 **)0x03001f2c;
    s32 first_class;
    s32 first_value;
    s32 first_flag;
    s32 second_class;
    s32 second_value;
    s32 second_flag;
    s32 x = base_x * 8;
    s32 y = base_y * 8;
    s32 i;

    character = Func_08077008(character_id);
    first_class = party[2 + first_index + 596] & 31;
    first_value = party[first_index + 596];
    first_flag = U16_AT(party, 376 + first_index * 2) & 0x8000;
    second_class = party[2 + second_index + 596] & 31;
    second_value = party[second_index + 596];
    second_flag = U16_AT(party, 376 + second_index * 2) & 0x8000;

    work = Func_08004938(332);
    Func_080072f0(work, character, 332, (void *)0x03001388);

    if (comparison_row == 0) {
        if (operation == 3) {
            Func_080150b0(S16_AT(character, 52), 3, window, x + 80, y + 56);
            Func_080150b0(S16_AT(character, 54), 3, window, x + 80, y + 64);
            Func_080150b0(S16_AT(character, 56), 3, window, x + 48, y + 56);
            Func_080150b0(S16_AT(character, 58), 3, window, x + 48, y + 64);
            Func_08015090((const void *)0x080af290, window, x + 72, y + 56);
            Func_08015090((const void *)0x080af290, window, x + 72, y + 64);
        } else {
            Func_080150b0(S16_AT(character, 56), 3, window, x + 48, y + 56);
            Func_080150b0(S16_AT(character, 58), 3, window, x + 48, y + 64);
        }
        Func_080150b0(U16_AT(character, 60), 3, window, x + 48, y + 72);
        Func_080150b0(U16_AT(character, 62), 3, window, x + 48, y + 80);
        Func_080150b0(U16_AT(character, 64), 3, window, x + 48, y + 88);
        Func_080150b0(U8_AT(character, 66), 2, window, x + 56, y + 96);
    }

    switch (operation) {
    case 0:
        Func_080771a8(character_id, second_value, second_class);
        Func_080771b0(character_id, second_value, second_class);
        break;
    case 1:
        Func_080771b8(character_id, first_value, first_class);
        break;
    case 2:
        if (first_flag) {
            Func_080771b8(character_id, first_value, first_class);
        }
        Func_080771a8(character_id, second_value, second_class);
        if (second_flag)
            Func_080771b0(character_id, second_value, second_class);
        break;
    case 4:
        Func_080771a8(character_id, second_value, second_class);
        if (second_flag)
            Func_080771b0(character_id, second_value, second_class);
        break;
    }

    Func_08077010(character_id);
    character = Func_08077008(character_id);

    if (comparison_row == 0) {
        Func_08015090(character, window, x + 40, y);
        Func_08015090((const void *)0x080af28c, window, x + 40, y + 16);
        Func_080150b0(character[15], 2, window, x + 88, y + 16);
        for (i = 0; i < 6; i++)
            Func_08015080(0x8ae + i, window, x, y + 56 + i * 8);
        Func_08015080(
            0x129 + work[0x741], window, x, y + 32);
    }

    if (comparison_row == 0) {
        s32 left = work[0x741];
        s32 right = character[0x741];
        s32 row = base_x;

        if (left != right) {
            Func_08015080(0xf296 + right, window, x, y + 48);
            Func_08015280(
                window, 0x333, base_x + 2, 5, comparison_row);
            left = work[0x741];
            right = character[0x741];
        }
        if (left != right)
            row += 5;

        for (i = 0; i < 4; i++) {
            Func_08015280(window, 0x5001 + i, row + i * 2, base_y + 5, 0);
            Func_08015280(
                window, 0xf030 + character[284 + i],
                row + 1 + i * 2, base_y + 5, 0);
        }
    }

    {
        s32 marker_x = x + 70;
        s32 marker_y;

        if (S16_AT(character, 56) != S16_AT(work, 56)) {
            marker_y = y + 56;
            Func_080150b0(S16_AT(character, 56), 4, window, x + 72, marker_y);
            Func_080ae9f0(
                window, marker_x, marker_y,
                S16_AT(character, 56) <= S16_AT(work, 56));
        }
        if (S16_AT(character, 58) != S16_AT(work, 58)) {
            marker_y = y + 64;
            Func_080150b0(S16_AT(character, 58), 4, window, x + 72, marker_y);
            Func_080ae9f0(
                window, marker_x, marker_y,
                S16_AT(character, 58) <= S16_AT(work, 58));
        }
        if (U16_AT(character, 60) != U16_AT(work, 60)) {
            marker_y = y + 72;
            Func_080150b0(U16_AT(character, 60), 4, window, x + 72, marker_y);
            Func_080ae9f0(
                window, marker_x, marker_y,
                U16_AT(character, 60) <= U16_AT(work, 60));
        }
        if (U16_AT(character, 62) != U16_AT(work, 62)) {
            marker_y = y + 80;
            Func_080150b0(U16_AT(character, 62), 4, window, x + 72, marker_y);
            Func_080ae9f0(
                window, marker_x, marker_y,
                U16_AT(character, 62) <= U16_AT(work, 62));
        }
        if (U16_AT(character, 64) != U16_AT(work, 64)) {
            marker_y = y + 88;
            Func_080150b0(U16_AT(character, 64), 4, window, x + 72, marker_y);
            Func_080ae9f0(
                window, marker_x, marker_y,
                U16_AT(character, 64) <= U16_AT(work, 64));
        }
        if (U8_AT(character, 66) != U8_AT(work, 66)) {
            marker_y = y + 96;
            Func_080150b0(U8_AT(character, 66), 2, window, x + 88, marker_y);
            Func_080ae9f0(
                window, marker_x, marker_y,
                U8_AT(character, 66) <= U8_AT(work, 66));
        }
    }

    if (comparison_row > 0) {
        s16 entries[48];
        s32 removed;
        s32 added;
        s32 count;
        s32 columns = 6 - (operation != 3);
        s32 start = columns * (comparison_row - 1);

        count = (s8)Func_080aae14(
            (u16 *)(work + 88), (u16 *)(character + 88),
            entries, &removed, &added);

        for (i = start; i < count && i - start < columns; i++) {
            s32 column = i - start;
            u16 entry = entries[i];
            s32 row = (base_y + column * 2) * 8;
            s32 tile = entry & 0x3fff;

            Func_080ae958(window, x, row + 4, tile);
            if (entry & 0x8000)
                Func_080150b8(4);
            else if (entry & 0x4000)
                Func_080150b8(2);
            else
                Func_080150b8(15);
            Func_08015080(
                0x333 + tile, window, x + 16, row + 8);
            Func_080150a8(
                U8_AT(Func_08077080(entry), 9), 2,
                window, x + 88, row + 8);
        }

        Func_080150b8(15);
        Func_08015080(0xaed, window, x + 88, y);
        if (operation != 3) {
            s32 row = base_y;
            if (removed) {
                Func_080150b8(4);
                Func_08015080(0xba2, window, x, y + 88);
                row++;
            }
            if (added) {
                Func_080150b8(2);
                Func_08015080(0xba3, window, x, row * 8 + 88);
                row++;
            }
            if (row == base_y)
                Func_08015080(0xba8, window, x, y + 88);
            Func_080150b8(15);
            Func_08015070(window, 0, 11, 13, 11);
        }
        (*(u8 **)0x03001e8c)[0xea3] = 1;
    }

    if (comparison_row == 0)
        Func_080150d8(character_id, 0, draw_portrait, window, 0, 0);

    Func_080072f0(character, work, 332, (void *)0x03001388);
    Func_08002df0(work);
    return 1;
}
