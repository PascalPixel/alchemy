typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;

s32 Func_0800eba0(s32 *a, s32 arg1, s32 *b, s32 arg3) {
    register s32 r0 asm("r0");
    s32 dx = (*a++ - *b++) >> 16;
    s32 dy = (*a++ - *b++) >> 16;
    s32 r1;
    r0 = (*a - *b) >> 16;
    r1 = arg1 + arg3;
    if (!(dx > 0x400000) && !(r0 > 0x400000) && (dx*dx + dy*dy + r0*r0) < r1*r1)
        r0 = 0;
    else
        r0 = -1;
    return r0;
}
