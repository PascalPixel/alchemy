typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef int bool;
#define NULL ((void *)0)

s32 *Func_0200405c(s32);

s32 Func_02001be8(s32 arg0, s32 arg1, s32 arg2) {
    s32 *p = Func_0200405c(arg0);
    if (p == NULL || (p[2] >> 20) != arg1) {
        return 0;
    }
    if ((p[4] >> 20) != arg2) {
        return 0;
    }
    return 1;
}
