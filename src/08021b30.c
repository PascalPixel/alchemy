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

s32 Func_08002dd8(s32);
s32 Func_080040d0(s32, s32);
s32 Func_080048f4(s32, s32);
s32 Func_0801a3d0(s32, s32, s32 *, s32 *, s32);

s32 Func_08021b30(s32 arg0, s32 arg1) {
    s32 sp4;
    s32 sp8;
    s32 temp_r5;
    s32 temp_r5_2;

    temp_r5_2 = Func_080048f4(0x11, 0x608);
    sp8 = arg1;
    Func_0801a3d0(arg0, 0, &sp8, &sp4, 1);
    temp_r5 = Func_080040d0(arg1, temp_r5_2 + 0x400);
    Func_08002dd8(0x11);
    return temp_r5;
}
