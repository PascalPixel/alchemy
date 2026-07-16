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

s32 Func_08021bc8(s32 arg0) {
    register s32 *base asm("r3");
    register s32 index asm("r2");
    s32 var_r0;

    var_r0 = arg0;
    if (var_r0 != 0) {
        var_r0 = 0;
    }
    base = (s32 *) 0x08073968;
    index = var_r0 << 2;
    return *(s32 *) ((u8 *) base + index);
}
