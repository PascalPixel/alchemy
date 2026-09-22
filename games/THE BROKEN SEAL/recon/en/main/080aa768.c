#include "types.h"

extern u8 Data_03001f2c[];

void Func_080030f8(s32 frames);
s32 Func_08002304(s32 value, s32 scale);
void Func_08015278(s32 value);
void Func_08015120(s32 value, s32 mode);
void Func_080151c8(s32 value);
void Func_08077010(s32 value);
s32 Func_080771b0(s32 a0, s32 a1, s32 a2);
s32 Func_080771b8(s32 a0, s32 a1, s32 a2);
s32 Func_080771c0(s32 a0, s32 a1, s32 a2);
s32 Func_080771c8(s32 a0, s32 a1, s32 a2);
void Func_080aa544(s32 value);
s32 Func_080ab314(void);
s32 Func_080ab5e4(s32 value);
void Func_080aad10(void);
void Func_080ad5b4(s32 a0, s32 a1, s32 a2, s32 a3);
s32 Func_080ad6d4(s32 value);
void Func_080ae2f4(void);
void Func_080aafb8(s32 value);
void Func_080aaf58(s32 value);
void Func_080f9010(s32 value);

static void UpdateShopSelection(s32 *ctx, s32 result, s32 message)
{
    s32 i;
    s32 count;
    s32 row;
    u8 *table;

    Func_080f9010(message);
    Func_08015278(*(s32 *)(ctx + 0x30));
    Func_080aaf58(*(s32 *)(ctx + 0x188));
    row = Func_08002304(*(s32 *)(ctx + 0x174), 10);
    table = *(u8 **)(ctx + 0x188);
    count = *(s8 *)(ctx + 0x188);
    for (i = 0; i < count; i++) {
        if (table[row * 10 + i] == *(u8 *)(ctx + 0x21a)) {
            *(u16 *)(ctx + 0x174) = (u16)(row * 10 + i);
            break;
        }
    }
    *(u8 *)(*(s32 **)(ctx + 20) + 5) = 1;
}

s32 Func_080aa768(void)
{
    s32 *ctx;
    s32 mode;
    s32 done;
    s32 result;
    s32 value;
    s32 index;
    s32 count;
    s32 i;
    u8 *table;

    ctx = *(s32 **)Data_03001f2c;
    *(u8 *)(*(s32 **)(ctx + 20) + 5) = 13;
    *(u16 *)(*(s32 **)(ctx + 20) + 12) = 0;
    Func_080aad10();
    Func_080030f8(1);

    mode = 2;
    done = 0;
    result = 0;
    while (!done) {
        if ((u32)mode > 15) {
            result = -1;
            done = 1;
            continue;
        }
        switch (mode) {
        case 0:
            result = -1;
            done = 1;
            break;
        case 2:
            Func_080aa544(0);
            result = Func_080ab5e4(0);
            mode = 15;
            if (result == 10) {
                done = 1;
                break;
            }
            mode = 0;
            if (result < 0) {
                done = 1;
                break;
            }
            *(s16 *)(ctx + 0xbb) = *(s8 *)(ctx + 28);
            if (result == 7) {
                mode = 10;
            } else {
                mode = 3;
            }
            break;
        case 3:
            index = *(s8 *)(ctx + 28);
            value = *(u16 *)(ctx + 0x208 + index * 2);
            *(s32 *)(ctx + 8) = value;
            *(u8 *)(ctx + 0x21a) = value;
            Func_080aafb8(*(s32 *)(ctx + 0x188));
            Func_080aa544(-8);
            Func_080ad5b4(0, index * 56 + 48, 54, 0);
            result = Func_080ab5e4(1);
            mode = 4;
            if (result == -2 || result < 0) {
                done = 1;
                break;
            }
            count = *(u8 *)(ctx + 0x219);
            for (i = 0; i < count; i++) {
                *(u16 *)(ctx + 0x144 + i * 2) += 8;
            }
            if (result == 1) {
                mode = 5;
            } else if (result == 2) {
                mode = 6;
            } else if (result == 3) {
                *(u16 *)(ctx + 0x220) = 2;
                mode = 7;
            } else if (result == 4) {
                *(u16 *)(ctx + 0x220) = 2;
                mode = 9;
            } else if (result == 5) {
                mode = 11;
            } else if (result == 6) {
                mode = 12;
            } else if (result == 8) {
                *(u16 *)(ctx + 0x220) = 2;
                mode = 13;
            } else if (result == 9) {
                *(u16 *)(ctx + 0x220) = 2;
                mode = 14;
            }
            break;
        case 4:
            if (result == -1) {
                done = 1;
                break;
            }
            value = *(u16 *)(ctx + 0x220);
            if (value & 1) {
                mode = 8;
            } else if (value & 2) {
                mode = 7;
            } else {
                mode = 3;
            }
            break;
        case 5:
            result = Func_080ad6d4(3);
            mode = 3;
            if (result == -2 || result < 0) {
                done = 1;
                break;
            }
            Func_080f9010(139);
            table = *(u8 **)(ctx + 0x188);
            Func_080771b0(table[0], table[1], table[2]);
            Func_080771c0(table[0], table[1], table[2]);
            Func_080151c8(*(s32 *)(ctx + 0x30));
            *(u8 *)(*(s32 **)(ctx + 20) + 5) = 1;
            mode = 2;
            break;
        case 6:
            result = Func_080ad6d4(2);
            mode = 3;
            if (result == -2 || result < 0) {
                done = 1;
                break;
            }
            UpdateShopSelection(ctx, result, 126);
            mode = 0;
            break;
        case 7:
            result = Func_080ad6d4(1);
            mode = 3;
            if (result == -2 || result < 0) {
                done = 1;
                break;
            }
            UpdateShopSelection(ctx, result, 126);
            mode = 0;
            break;
        case 8:
            if (*(u8 *)(ctx + 0x218) == 0) {
                mode = 0;
                break;
            }
            result = Func_080ab5e4(1);
            mode = 4;
            if (result == -2 || result < 0) {
                done = 1;
            } else {
                mode = 9;
            }
            break;
        case 9:
            result = Func_080ad6d4(0);
            mode = 3;
            if (result == -2 || result < 0) {
                done = 1;
                break;
            }
            UpdateShopSelection(ctx, result, 126);
            mode = 0;
            break;
        case 10:
            index = *(s8 *)(ctx + 28);
            value = *(u16 *)(ctx + 0x208 + index * 2);
            *(s32 *)(ctx + 8) = value;
            *(u8 *)(ctx + 0x21a) = value;
            Func_080ae2f4();
            result = Func_080ab5e4(1);
            mode = 4;
            if (result == -2 || result < 0) {
                done = 1;
            } else {
                mode = 9;
            }
            break;
        case 11:
            result = Func_080ad6d4(3);
            mode = 3;
            if (result == -2 || result < 0) {
                done = 1;
                break;
            }
            Func_080f9010(139);
            Func_080151c8(*(s32 *)(ctx + 0x30));
            mode = 2;
            break;
        case 12:
            Func_080f9010(175);
            table = *(u8 **)(ctx + 0x188);
            Func_080771b8(table[0], table[1], table[2]);
            Func_080771c8(table[0], table[1], table[2]);
            Func_080151c8(*(s32 *)(ctx + 0x30));
            mode = 2;
            break;
        case 13:
            result = Func_080ad6d4(1);
            mode = 3;
            if (result == -2 || result < 0) {
                done = 1;
                break;
            }
            UpdateShopSelection(ctx, result, 126);
            mode = 0;
            break;
        case 14:
            result = Func_080ad6d4(0);
            mode = 3;
            if (result == -2 || result < 0) {
                done = 1;
                break;
            }
            UpdateShopSelection(ctx, result, 126);
            mode = 0;
            break;
        case 15:
            result = Func_080ab314();
            done = 1;
            break;
        default:
            result = -1;
            done = 1;
            break;
        }
    }
    return result;
}
