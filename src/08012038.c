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


u8 Func_08012038(s32 arg0, s32 arg1, s32 arg2) {
    s32 temp_r0;
    s32 var_r2;
    s32 idx;

    temp_r0 = *(s32 *)0x03001E70;
    arg1 >>= 0x14;
    arg2 >>= 0x14;
    var_r2 = 0x02010000;
    if (temp_r0 != 0) {
        idx = ((arg0 & 3) * 0x30) + 0x130;
        var_r2 = *(s32 *)(temp_r0 + idx);
    }
    var_r2 += (arg1 + (arg2 << 7)) * 4;
    return M2C_FIELD(var_r2, u8 *, 2);
}
