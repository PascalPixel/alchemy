typedef signed int s32;
typedef unsigned int u32;
typedef signed short s16;
typedef unsigned short u16;
typedef signed char s8;
typedef unsigned char u8;

extern u8 Value_000022a8;
extern void Func_020049c6(s32);
extern s32 Func_020049d6(s32, s32);
extern s32 Func_02004936(s32, s32);
extern void Func_020049e0(s32);
extern void Func_020049e8(s32);
extern s32 Func_02004a00(s32, s32);

void Func_02000524(s32 a)
{
    s32 k = (s32)&Value_000022a8;

    Func_020049c6(k);
    Func_020049d6(a, 0);
    if (Func_02004936(0, 0) == 0)
        Func_020049e0(k + 1);
    else
        Func_020049e8(k + 2);
    Func_02004a00(a, 0);
}
