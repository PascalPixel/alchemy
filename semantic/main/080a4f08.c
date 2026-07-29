#include "types.h"

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

s32 Func_080022fc(s32, s32);
void Func_08002dd8(s32);
void Func_080030f8(s32);
void Func_08003fa4(s32, s32, void *);
s32 Func_08004080(void);
u8 *Func_080048f4(s32, s32);
void Func_08015080(s32, u8 *, s32, s32);
void Func_08015090(u8 *, u8 *, s32, s32);
void Func_080150b0(s32, s32, u8 *, s32, s32);
u8 *Func_080150c8(s32, u32, u8 *, s32, s32);
void Func_08015270(u8 *);
void Func_08015278(u8 *);
u8 *Func_08077008(u8);
s32 Func_080770c0(s32);
void Func_080a1a40(s32, s32);
void Func_080a1ac0(s32, s32);
s32 Func_080a3d9c(u8, s32);
void Func_080a4eb8(void);
void Func_080b0040(s32, s32, u8 *);
void Func_080f9010(s32);

/*
 * Present the bounded selection window and return the selected index, or -1
 * when cancelled or interrupted by status flag 0x150.
 */
s32 Func_080a4f08(s32 selection, s32 count, s32 alternate_flag)
{
    u8 *ui = *(u8 **)0x03001f2c;
    u8 *buffer = Func_080048f4(0x0e, 0x400);
    u8 *window = FIELD(ui, u8 *, 0x10c);
    s32 initial_selection = selection;
    s32 alternate_total = 0;
    s32 primary_total;
    s32 tile;
    s32 dirty = 1;

    Func_080a4eb8();
    Func_08015270(window);

    if (alternate_flag == 0) {
        alternate_total = Func_080a3d9c(
            FIELD(ui, u8, 0x21b),
            FIELD(ui, u16, 0x178) & 0x1ff);
    }
    primary_total = Func_080a3d9c(
        FIELD(ui, u8, 0x21a),
        FIELD(ui, u16, 0x178) & 0x1ff);

    tile = Func_08004080();
    if (tile != 0x60) {
        u8 *object;
        u16 attributes;

        Func_08003fa4(tile, 0x100, 0);
        Func_080150c8(tile, 0x40004000, window, 0x30, 0x20);
        object = Func_080150c8(
            tile, 0x40004000, window, 0x50, 0x20);
        attributes = FIELD(object, u16, 0x18);
        FIELD(object, u16, 0x18) =
            (attributes & 0xfc00) | ((attributes + 4) & 0x03ff);
        Func_080a1ac0(0x80, 0x28);

        for (;;) {
            if (Func_080770c0(0x150) != 0)
                break;

            if (dirty != 0) {
                volatile u32 *dma3 = (volatile u32 *)0x040000d4;

                dirty = 0;
                selection = Func_080022fc(selection + count, count);
                Func_08015270(window);
                Func_08015080(0x0ade, window, 0x20, 0);

                dma3[0] = 0x080af08c;
                dma3[1] = (u32)buffer;
                dma3[2] = 0x84000040;

                Func_080b0040(0x1e, 0x0e, buffer);
                Func_080b0040(count + selection, 0, buffer);
                Func_080b0040(
                    initial_selection + selection + 1, 0x0a, buffer);
                Func_080b0040(initial_selection, 2, buffer);
                Func_08003fa4(tile, 0x100, buffer);

                Func_080150b0(selection + 1, 2, window, 0x20, 0x20);
                Func_08015080(
                    (FIELD(ui, u16, 0x178) & 0x1ff) + 0x182,
                    window, 0x10, 8);
                Func_080150b0(
                    primary_total - selection - 1,
                    2, window, 0x10, 0x18);
                if (alternate_flag == 0) {
                    Func_080150b0(
                        alternate_total + selection + 1,
                        2, window, 0x50, 0x18);
                }

                Func_08015090(
                    Func_08077008(FIELD(ui, u8, 0x21a)),
                    window, 0x10, 0x10);
                if (alternate_flag == 0) {
                    Func_08015090(
                        Func_08077008(FIELD(ui, u8, 0x21b)),
                        window, 0x50, 0x10);
                }
            }

            if ((*(u32 *)0x03001c94 & 1) != 0) {
                Func_080f9010(0x70);
                break;
            }
            if ((*(u32 *)0x03001c94 & 2) != 0) {
                selection = -1;
                Func_080f9010(0x71);
                break;
            }

            Func_080a1a40(0x80, 0x28);
            if ((*(u32 *)0x03001b04 & 0x20) != 0) {
                selection--;
                dirty = 1;
                Func_080f9010(0x6f);
            }
            if ((*(u32 *)0x03001b04 & 0x10) != 0) {
                selection++;
                dirty = 1;
                Func_080f9010(0x6f);
            }
            Func_080030f8(1);
        }
    }

    Func_08015270(window);
    Func_08015278(window);
    Func_08002dd8(0x0e);
    FIELD(FIELD(ui, u8 *, 0x21c), s8, 5) = 0x0d;
    if (Func_080770c0(0x150) != 0)
        selection = -1;
    return selection;
}
