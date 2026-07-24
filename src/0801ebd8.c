typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

s32 Func_08004080(void);
void Func_08019ee4(s32, s32, s32 *, s32 *, s32);
s32 Func_0801eadc(s32, s32, s32, s32, s32);

s32 Func_0801ebd8(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 count;
    s32 unused;
    s32 result;

    count = Func_08004080();
    if (count == 0x60) {
        return 0;
    }
    Func_08019ee4(arg0, 1, &count, &unused, 1);
    result = Func_0801eadc(count, 0x40000000, arg1, arg2, arg3);
    *(u8 *)(result + 15) = 251;
    return result;
}
