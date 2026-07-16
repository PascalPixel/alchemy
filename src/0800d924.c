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
#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_0800eba0(void *, s32, s32, s32);

s32 Func_0800d924(void *arg0, s32 arg1) {
    s32 sp0;
    s32 var_r4;
    u8 *var_r6;
    void *var_r5;

    var_r5 = *(void **)0x03001E64;
    var_r4 = 0;
    var_r6 = var_r5 + 0x59;
loop_1:
    if ((M2C_FIELD(var_r5, s32 *, 0) != 0) && (1 & *var_r6) && (var_r5 != arg0)) {
        sp0 = var_r4;
        if (Func_0800eba0(var_r5 + 8, M2C_FIELD(var_r5, u16 *, 0x20) - 2, arg1, M2C_FIELD(arg0, u16 *, 0x20) - 2) >= 0) {
            return -1;
        }
    }
    var_r4 += 1;
    var_r6 += 0x70;
    var_r5 += 0x70;
    if (var_r4 > 0x3F) {
        return 0;
    }
    goto loop_1;
}
