typedef signed int s32;
typedef unsigned int u32;
typedef signed short s16;
typedef unsigned short u16;
typedef signed char s8;
typedef unsigned char u8;

extern u8 Value_00001ff1;
extern void Func_0200456a(s32);
extern s32 Func_0200457a(s32, s32);
extern s32 Func_020044da(s32, s32);
extern void Func_02004584(s32);
extern void Func_0200458c(s32);
extern s32 Func_020045a4(s32, s32);

void Func_020000c8(s32 a)
{
    s32 k = (s32)&Value_00001ff1;

    Func_0200456a(k);
    Func_0200457a(a, 0);
    if (Func_020044da(0, 0) == 0)
        Func_02004584(k + 1);
    else
        Func_0200458c(k + 2);
    Func_020045a4(a, 0);
}
