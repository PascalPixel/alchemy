typedef signed int s32;
typedef unsigned int u32;
typedef signed short s16;
typedef unsigned short u16;
typedef signed char s8;
typedef unsigned char u8;

extern s32 Func_02004b0e(void);

s32 Func_0200089c(u8 *a)
{
    if ((((u32)(Func_02004b0e() * 40)) >> 16) == 0)
        *(s32 *)(a + 0x28) = 0x40000;
    return 1;
}
