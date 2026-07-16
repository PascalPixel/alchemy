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

s32 Func_080030f8(s32);
s32 Func_08009080(s32, s32);
s32 Func_0808ef70(s32, s32);
s32 Func_08092054(s32);
s32 Func_080f9010(s32);

s32 Func_0808e078(s32 arg0, s32 arg1, s32 arg2) {
    s32 temp_r0;
    s32 var_r0;

    temp_r0 = Func_08092054(arg1);
    var_r0 = 0;
    if (temp_r0 != 0) {
        Func_080f9010(0x7C);
        Func_08009080(temp_r0, 4);
        Func_080030f8(0xC);
        var_r0 = Func_0808ef70(arg0, arg2);
    }
    return var_r0;
}
