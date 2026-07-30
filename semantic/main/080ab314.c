#include "types.h"

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

void Func_080030f8(s32);
void Func_08003f3c(u16);
void Func_080041d8(u32, s32);
u8 *Func_08015010(s32, s32, s32, s32, s32);
void Func_08015018(u8 *, s32);
s32 Func_08015048(void);
void Func_08015060(u8 *);
void Func_08015078(s32, u8 *, s32, s32);
void Func_08015080(s32, u8 *, s32, s32);
void Func_08015278(u8 *);
void Func_080152a8(void);
u8 **Func_080153f8(u8 *, s32);
void Func_080a1a40(s32, s32);
s32 Func_080aa538(s32, s32);
void Func_080ab1f4(u8 *, s32, s32, s32, s32, s32);
void Func_080ab21c(s32, s32, s32, s32, s32);
void Func_080ab2ec(u8 *, s32, s32, s32, s32, s32);
void Func_080f9010(s32);

/*
 * Run the seven-entry selection screen.  Return -1 for cancel, -2 for the
 * alternate exit, and keep rebuilding the preview until an exit is chosen.
 */
s32 Func_080ab314(void)
{
    u8 *ui = *(u8 **)0x03001f2c;
    u8 *state = *(u8 **)0x03001e8c;
    u8 *row;
    u8 *preview;
    u8 *cursor;
    u8 *window;
    s32 result = 0;
    s32 selection = 0;
    s32 previous = 0;
    s32 i;

    Func_08015278(FIELD(ui, u8 *, 0x30));
    Func_080030f8(1);
    Func_08015060(FIELD(ui, u8 *, 0x10c));
    Func_08015080(0x0c30, FIELD(ui, u8 *, 0x10c), 0, 0);
    Func_08015080(0x0c31, FIELD(ui, u8 *, 0x10c), 0, 0x10);
    Func_080ab21c(1, 1, 0x0b, 3, 6);
    Func_080ab2ec(FIELD(ui, u8 *, 0x30), 0, 0, 0x1c, 0x0a, 6);

    row = Func_08015010(0, 9, 8, 0x0a, 6);
    preview = Func_08015010(8, 0x0c, 0x16, 7, 2);
    cursor = Func_08015010(8, 9, 0x16, 3, 2);
    Func_080152a8();

    for (i = 0; i < 7; i++)
        Func_08015080(0x0c32 + i, row, 0, i * 8);

    do {
        u8 **preview_slot;

        Func_08015060(cursor);
        Func_08015078(0x0c32 + selection, cursor, 0, 0);
        preview_slot = Func_080153f8(preview, 0x0c39 + selection);
        Func_080ab1f4(row, 0, previous, 6, 1, 0x0f);
        Func_080ab1f4(row, 0, selection, 6, 1, 0x0e);
        previous = selection;

        for (;;) {
            Func_080a1a40(
                -0x0c,
                (FIELD(row, u16, 0x0e) + selection) * 8 + 8);
            Func_080030f8(1);

            if ((*(u32 *)0x03001b04 & 0x90) != 0) {
                selection = Func_080aa538(selection + 1, 7);
                Func_080f9010(0x6f);
                break;
            }
            if ((*(u32 *)0x03001b04 & 0x60) != 0) {
                selection = Func_080aa538(selection - 1, 7);
                Func_080f9010(0x6f);
                break;
            }
            if ((*(u32 *)0x03001c94 & 8) != 0) {
                Func_080f9010(0x71);
                result = -2;
                break;
            }
            if ((*(u32 *)0x03001c94 & 6) != 0) {
                Func_080f9010(0x71);
                result = -1;
                break;
            }
            if ((*(u32 *)0x03001c94 & 1) != 0) {
                if (Func_08015048() != 0) {
                    selection = Func_080aa538(selection + 1, 7);
                    Func_080f9010(0x70);
                    break;
                }
                Func_080f9010(0x6f);
            }
        }

        if (FIELD(state, u16, 0x12b6) != 0x63) {
            Func_08003f3c(FIELD(state, u16, 0x12b6));
            FIELD(state, u16, 0x12b6) = 0x63;
        }
        FIELD(state, s8, 0x12f8) = 0;
        Func_08015060(preview);

        window = *preview_slot;
        FIELD(window, s16, 0x1a) = 0;
        FIELD(window, s16, 0x18) = 0;
        FIELD(window, s16, 0x14) = 0;
        *preview_slot = 0;
    } while (result == 0);

    FIELD(state, s8, 0x0ea6) = 1;
    Func_08015278(cursor);
    Func_08015278(preview);
    Func_080030f8(1);
    Func_08015018(cursor, 1);
    Func_08015018(row, 1);
    Func_08015018(preview, 1);
    Func_080152a8();

    if (result == -2) {
        Func_08015060(FIELD(ui, u8 *, 0x10c));
        Func_08015060(FIELD(ui, u8 *, 0x30));
        Func_08015060(FIELD(ui, u8 *, 0x10));
        FIELD(state, s8, 0x0ea6) = 0;
    }

    Func_080041d8(0x080a19a1, 0x0c80);
    return result;
}
