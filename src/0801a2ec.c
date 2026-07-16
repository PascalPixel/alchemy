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

s32 Func_08002dd8(s32);
s32 Func_08003fa4(s32, s32, s32);
s32 Func_080048b0(s32, s32);
s32 Func_0801a4c0(s32);

s32 Func_0801a2ec(s32 arg0, s32 arg1, s32 arg2) {
    s32 temp_r5;

    temp_r5 = Func_080048b0(0x11, 0x608);
    Func_0801a4c0(arg0);
    Func_08003fa4(arg2, 0x80, temp_r5 + 0x400);
    Func_08002dd8(0x11);
    return 1;
}
