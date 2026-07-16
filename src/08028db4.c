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

s32 Func_080284dc();
s32 Func_0802851c();
s32 Func_08028574(s32);
s32 Func_080287a8(s32);
s32 Func_08028808(s32, s32, s32);

s32 Func_08028db4(s32 arg0) {
    s32 temp_r5;

    Func_080284dc();
    Func_080287a8(0x19);
    Func_080287a8(0x1A);
    Func_080287a8(0x1B);
    Func_080287a8(0x1C);
    Func_08028808(0x11, 0xA, 0);
    temp_r5 = Func_08028574(arg0);
    Func_0802851c();
    return temp_r5;
}
