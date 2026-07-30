typedef signed int s32;
typedef unsigned int u32;
typedef signed short s16;
typedef unsigned short u16;
typedef signed char s8;
typedef unsigned char u8;

extern u8 Value_00002006;
extern u8 Value_00000105;

extern void Func_020047ea(s32);
extern s32 Func_020047fa(s32, s32);
extern s32 Func_0200475a(s32, s32);
extern void Func_02004744(s32);
extern void Func_02004830(s32, s32, s32);
extern void Func_02004816(s32);
extern void Func_0200475e(s32);
extern void Func_02004848(s32, s32, s32);
extern void Func_0200482e(s32);
extern void Func_02004846(s32, s32);

void Func_02000348(s32 a)
{
    s32 k = (s32)&Value_00002006;

    Func_020047ea(k);
    Func_020047fa(a, 0);
    if (Func_0200475a(0, 0) == 0) {
        Func_02004744(10);
        Func_02004830(a, 0x102, 0x28);
        Func_02004816(k + 1);
    } else {
        Func_0200475e(10);
        Func_02004848(a, (s32)&Value_00000105, 0x28);
        Func_0200482e(k + 2);
    }
    Func_02004846(a, 0);
}
