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

s32 Func_08011d10(void *arg0, s32 arg1, s32 arg2) {
    register s32 difference asm("r3");
    s32 temp_r1;
    register s32 var_r0 asm("r0");

    temp_r1 = M2C_FIELD(arg0, s8 *, 0) << 0x13;
    difference = (M2C_FIELD(arg0, s8 *, 1) << 0x13) - temp_r1;
    var_r0 = arg2;
    asm volatile("" : "+r" (var_r0) : "r" (difference));
    var_r0 *= difference;
    if (var_r0 < 0) {
        var_r0 += 0xF;
    }
    return temp_r1 + (var_r0 >> 4);
}
