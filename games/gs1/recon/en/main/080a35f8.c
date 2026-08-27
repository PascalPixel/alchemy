#include "types.h"

#define M2C_FIELD(expr, type_ptr, offset) (*(type_ptr)((s8 *)(expr) + (offset)))

extern void *Data_03001f2c;

void *Func_08077008(s32);
s32 Func_080a10d0(s32 *, s32, s32, s32, s32, s32);
void Func_080a33d4(void *, s32);
void *Func_080150d0(s32, s32, s32, s32, s32);
void Func_08015080(s32, s32, s32, s32);
void Func_080030f8(s32);
s32 Func_080770c0(s32);
s32 Func_080022fc(s32, s32);
void Func_080a1a40(s32, s32);
s32 Func_080a3ddc(void *, void *, s32);
void Func_080a38a8(s32);
void Func_080a112c(s32, s32, s32, s32);
void Func_080a3e88(s32, s32);
s32 Func_080a3d6c(s32);
void Func_080a1e38(s32, s32);
void Func_080f9010(s32);

s32 Func_080a35f8(u16 *positions)
{
    void *state;
    s32 selection;
    s32 count;
    s32 redraw;
    s32 result;
    s32 pending;
    s32 window;
    s32 pair_off;
    s32 offset;
    s16 *cursor_ptr;
    s32 i;
    u16 *entry;
    s32 counter;
    u8 tmp_byte;
    s32 tmp2;
    void *icon;
    s32 sound_row;

    state = Data_03001f2c;
    selection = M2C_FIELD(state, s8 *, 0x1C);
    count = (s32)M2C_FIELD(state, s8 *, 0x1E);
    pending = 0;
    counter = 0;
    result = 0;
    redraw = 1;
    window = (s32)Func_08077008(positions[selection]);
    if (Func_080a10d0((s32 *)((u8 *)state + 0x20), 0xD, 3, 0x11, 2, 10) != 0) {
        Func_080a33d4(state, M2C_FIELD(state, s32 *, 0x20));
    }
    if (Func_080a10d0((s32 *)((u8 *)state + 0x28), 0xD, 0xD, 0x11, 2, 4) != 0) {
        icon = Func_080150d0(2, 0, M2C_FIELD(state, s32 *, 0x28), 0, pending);
        M2C_FIELD(state, void **, 0x21C) = icon;
        M2C_FIELD(icon, s8 *, 5) = 0xD;
    }
    Func_08015080(0xB87, M2C_FIELD(state, s32 *, 0x28), 0, 0);
    Func_08015080(0xB88, M2C_FIELD(state, s32 *, 0x28), 0, 8);
    M2C_FIELD(M2C_FIELD(state, void **, 0x14), s8 *, 5) = 1;

loop_32:
    if (Func_080770c0(0x150) == 0) {
        selection = Func_080022fc(count + selection, count);
        pair_off = selection * 2;
        Func_080a1a40((selection * 0x18) - 0xA, 0x10);
        if (redraw != 0) {
            counter = 0;
            entry = positions + selection;
            redraw = 0;
            sound_row = M2C_FIELD(state, s32 *, 0x24);
            window = (s32)Func_08077008(*entry);
            if (pending != 0) {
                M2C_FIELD(state, s8 *, 0x218) = (s8)Func_080a3ddc(
                    Func_08077008(*entry), (u8 *)state + 0x1C8, 0);
                Func_080a38a8(*entry);
                Func_080a112c(sound_row, *entry, 0, 8);
            } else {
                Func_080a3e88(*entry, 0);
                Func_080a112c(sound_row, *entry, 0, 0);
            }
            i = 3;
            cursor_ptr = (s16 *)((u8 *)state + 0x14A);
            do {
                i -= 1;
                *cursor_ptr = 0x1E;
                cursor_ptr -= 2;
            } while (i >= 0);
            M2C_FIELD(state, s16 *, 0x144 + pair_off) = 0x1A;
        }
        Func_080030f8(1);
        if (*(volatile u32 *)0x03001C94 & 1) {
            if (*(volatile u32 *)0x03001AE8 & 0x200) {
                counter = (counter + 4) % 4;
                tmp_byte = (u8)counter;
                Func_080a1e38(window + 0xD8, tmp_byte);
                counter = (u8)(tmp_byte + 1);
                Func_080a3e88(positions[selection], 0);
                Func_080f9010(0x70);
                goto block_21;
            }
            entry = positions + selection;
            if (Func_080a3d6c(*entry) != 0) {
                Func_080f9010(0x70);
                result = *entry;
            } else {
                Func_080f9010(0x72);
                goto block_21;
            }
        } else {
        block_21:
            if (*(volatile u32 *)0x03001C94 & 2) {
                Func_080f9010(0x71);
                result = -1;
            } else {
                if (*(volatile u32 *)0x03001C94 & 0x100) {
                    pending = 1;
                    redraw = 1;
                }
                if (!(*(volatile u32 *)0x03001AE8 & 0x100) && pending == 1) {
                    pending = 0;
                    redraw = 1;
                }
                if (*(volatile u32 *)0x03001B04 & 0x20) {
                    Func_080f9010(0x6F);
                    selection -= 1;
                    redraw = 1;
                }
                if (*(volatile u32 *)0x03001B04 & 0x10) {
                    Func_080f9010(0x6F);
                    selection += 1;
                    redraw = 1;
                }
                goto loop_32;
            }
        }
    } else {
        pair_off = selection * 2;
    }
    M2C_FIELD(state, s8 *, 0x1C) = selection;
    tmp2 = positions[selection];
    M2C_FIELD(state, s32 *, 8) = tmp2;
    M2C_FIELD(state, s8 *, 0x21A) = (s8)tmp2;
    return result;
}
