#include "types.h"
#define M2C_FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

void Func_080f24a0(void) {
    s16 *var_r1;
    s16 *var_r1_2;
    s16 var_r3;
    s16 var_r3_2;
    s16 var_r3_3;
    s16 var_r3_4;
    s32 temp_r0;
    s32 temp_r0_2;
    s32 var_r0;
    s32 var_r0_2;
    s32 var_r0_3;
    s32 var_r0_4;
    s32 var_r4;
    s32 var_r4_2;
    s32 var_r4_3;
    s32 var_r4_4;
    s32 var_r4_5;
    void *temp_r3;
    void *temp_r3_2;
    void *temp_r7;
    void *var_r3_5;

    temp_r7 = *(void **)0x03001EFC;
    *(s16 *)0x04000000 = 0;
    temp_r0 = Func_08002f40(0x15);
    M2C_FIELD((void *)0x040000D4, s32 *, 0) = temp_r0;
    M2C_FIELD((void *)0x040000D4, s32 *, 4) = 0x05000200;
    M2C_FIELD((void *)0x040000D4, s32 *, 8) = 0x84000080;
    *(s16 *)0x05000200 = 0;
    Func_080053e8(temp_r0 + 0x200, (void *)0x02010000);
    M2C_FIELD((void *)0x040000D4, s32 *, 0) = 0x02010000;
    M2C_FIELD((void *)0x040000D4, s32 *, 4) = 0x06010000;
    M2C_FIELD((void *)0x040000D4, s32 *, 8) = 0x80000F00;
    temp_r0_2 = Func_08002f40(0x17);
    M2C_FIELD((void *)0x040000D4, s32 *, 0) = temp_r0_2;
    M2C_FIELD((void *)0x040000D4, s32 *, 4) = 0x05000000;
    M2C_FIELD((void *)0x040000D4, s32 *, 8) = 0x84000080;
    *(s16 *)0x05000000 = 0;
    Func_080053e8(temp_r0_2 + 0x200, (void *)0x02010000);
    M2C_FIELD((void *)0x040000D4, s32 *, 0) = 0x02012940;
    M2C_FIELD((void *)0x040000D4, s32 *, 4) = 0x06000000;
    M2C_FIELD((void *)0x040000D4, s32 *, 8) = 0x80002760;
    temp_r3 = (0x040000D4 + 0xC) - 0xC;
    M2C_FIELD(temp_r3, s32 *, 0) = 0x0201A140;
    M2C_FIELD(temp_r3, s32 *, 4) = 0x06004EC0;
    M2C_FIELD(temp_r3, s32 *, 8) = 0x80004EC0;
    var_r1 = (s16 *)0x0600F000;
    var_r3 = 0x267;
    var_r4 = 0;
    do {
        var_r0 = 0x1D;
loop_3:
        var_r0 -= 1;
        *var_r1 = var_r3;
        var_r3 = (s16) ((s32) ((var_r3 << 0x10) + 0x10000) >> 0x10);
        var_r1 += 2;
        if (var_r0 >= 0) {
            goto loop_3;
        }
        M2C_FIELD(var_r1, s16 *, 0) = 0x1FF;
        var_r4 += 1;
        M2C_FIELD(var_r1, s16 *, 2) = 0x1FF;
        var_r1 = var_r1 + 2 + 2;
    } while (var_r4 <= 0xA);
    var_r3_2 = 0x13B;
    var_r4_2 = 0xB;
    do {
        var_r0_2 = 0x1D;
loop_7:
        var_r0_2 -= 1;
        *var_r1 = var_r3_2;
        var_r3_2 = (s16) ((s32) ((var_r3_2 << 0x10) + 0x10000) >> 0x10);
        var_r1 += 2;
        if (var_r0_2 >= 0) {
            goto loop_7;
        }
        M2C_FIELD(var_r1, s16 *, 0) = 0x1FF;
        var_r4_2 += 1;
        M2C_FIELD(var_r1, s16 *, 2) = 0x1FF;
        var_r1 = var_r1 + 2 + 2;
    } while (var_r4_2 <= 0x1F);
    var_r1_2 = (s16 *)0x0600F800;
    var_r3_3 = 0x12C;
    var_r4_3 = 0;
    do {
        var_r0_3 = 0x1D;
loop_11:
        var_r0_3 -= 1;
        *var_r1_2 = var_r3_3;
        var_r3_3 = (s16) ((s32) ((var_r3_3 << 0x10) + 0x10000) >> 0x10);
        var_r1_2 += 2;
        if (var_r0_3 >= 0) {
            goto loop_11;
        }
        M2C_FIELD(var_r1_2, s16 *, 0) = 0x1FF;
        var_r4_3 += 1;
        M2C_FIELD(var_r1_2, s16 *, 2) = 0x1FF;
        var_r1_2 = var_r1_2 + 2 + 2;
    } while (var_r4_3 <= 0xA);
    var_r3_4 = 0;
    var_r4_4 = 0xB;
    do {
        var_r0_4 = 0x1D;
loop_15:
        var_r0_4 -= 1;
        *var_r1_2 = var_r3_4;
        var_r3_4 = (s16) ((s32) ((var_r3_4 << 0x10) + 0x10000) >> 0x10);
        var_r1_2 += 2;
        if (var_r0_4 >= 0) {
            goto loop_15;
        }
        M2C_FIELD(var_r1_2, s16 *, 0) = 0x1FF;
        var_r4_4 += 1;
        M2C_FIELD(var_r1_2, s16 *, 2) = 0x1FF;
        var_r1_2 = var_r1_2 + 2 + 2;
    } while (var_r4_4 <= 0x1F);
    M2C_FIELD((void *)0x0400000A, s16 *, 0) = 0x1F43;
    M2C_FIELD((void *)0x0400000A, s16 *, 2) = 0x1E81;
    M2C_FIELD((void *)0x04000040, s16 *, 0) = 0xF0;
    M2C_FIELD((void *)0x04000040, s16 *, 4) = 0x9F;
    temp_r3_2 = ((void *)0x04000040 + 4) - 2;
    M2C_FIELD(temp_r3_2, s16 *, 0) = 0xF0;
    M2C_FIELD(temp_r3_2, s16 *, 4) = 0x9F;
    *(s16 *)0x04000048 = 0x1616;
    var_r3_5 = (void *)0x03001AD0;
    var_r4_5 = 3;
    do {
        var_r4_5 -= 1;
        M2C_FIELD(var_r3_5, s16 *, 2) = 0;
        M2C_FIELD(var_r3_5, s16 *, 0) = 0;
        var_r3_5 += 4;
    } while (var_r4_5 >= 0);
    M2C_FIELD((void *)0x03001AD0, s16 *, 6) = 0x60;
    M2C_FIELD((void *)0x03001AD0, s16 *, 0xA) = 0x60;
    M2C_FIELD(temp_r7, s32 *, 8) = 0;
    M2C_FIELD(temp_r7, s32 *, 0) = 0;
    M2C_FIELD(temp_r7, s32 *, 4) = 0;
    M2C_FIELD(temp_r7, s32 *, 0xC) = 0;
    M2C_FIELD(temp_r7, s32 *, 0x14) = 0;
    M2C_FIELD(temp_r7, s32 *, 0x10) = 0;
    M2C_FIELD((void *)0x040000D4, s32 *, 0) = 0x03001AD0;
    M2C_FIELD((void *)0x040000D4, s32 *, 4) = 0x04000010;
    M2C_FIELD((void *)0x040000D4, s32 *, 8) = 0x84000004;
    *(s16 *)0x04000050 = 0x3FBF;
    *(s16 *)0x04000052 = 0x1010;
    *(s16 *)0x04000050 = 0x3F44;
}
