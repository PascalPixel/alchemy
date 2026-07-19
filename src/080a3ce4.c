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

s32 Func_080a3ce4(s32 arg0) {
    s32 ret;
    s32 lo;

    if (arg0 > 0xC4) {
        goto L0;
    }
    lo = 0xC1;
    if (arg0 < lo) {
        goto L0;
    }
    ret = 1;
    return ret;
L0:
    ret = 0;
    return ret;
}
