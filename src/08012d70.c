typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;
#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void *Func_08185000(s32 arg0);

void Func_08012d70(s32 arg0, s32 arg1) {
    s32 sp0;
    s32 sp4;
    s32 var_r1;
    s32 var_r4;
    s32 var_r7;
    s32 var_r8;
    void *temp_r0;
    void *temp_r5;
    void *var_r6;

    var_r1 = arg1;
    var_r6 = *(void **)0x03001E60;
    var_r7 = 0;
    var_r8 = ((3 & arg0) * 4) + 0x28;
    var_r4 = 0;
    do {
        temp_r5 = *(void **)((u8 *)var_r6 + var_r8);
        if (M2C_FIELD(temp_r5, s32, 0xC) != 0) {
            sp4 = var_r1;
            sp0 = var_r4;
            temp_r0 = Func_08185000((s32) M2C_FIELD(temp_r5, s16, 0));
            if (var_r1 < (s32) M2C_FIELD(temp_r0, u8, 5)) {
                M2C_FIELD(temp_r5, u8, 4) = (u8) M2C_FIELD(temp_r0, u8, 4);
                M2C_FIELD(temp_r5, s32, 0x10) = *(s32 *)((var_r1 * 4) + M2C_FIELD(temp_r5, s32, 0xC));
                M2C_FIELD(temp_r5, s16, 2) = (s16) (var_r7 * 0x10);
                M2C_FIELD(temp_r5, s8, 0x15) = 0x10;
                M2C_FIELD(temp_r5, s8, 0x14) = (s8) var_r4;
                M2C_FIELD(temp_r5, s8, 0x17) = (s8) var_r4;
                M2C_FIELD(temp_r5, u8, 0x16) = 0xFF;
            }
            M2C_FIELD(var_r6, u8, 0x23) = (u8) M2C_FIELD(temp_r0, u8, 7);
            M2C_FIELD(var_r6, s16, 0x1E) = (s16) var_r4;
        }
        var_r7 += 1;
        var_r6 = (u8 *)var_r6 + 0x38;
    } while (var_r7 <= 9);
}
