typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

s32 Func_0801bc34(s32, s32);
s32 Func_0801eadc(s32, s32, s32, s32, s32);

s32 Func_0801eb64(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 result;

    result = Func_0801bc34(arg0, arg1);
    if (result < 0) {
        return 0;
    }
    return Func_0801eadc(result, 0x40000000, arg2, arg3, arg4);
}
