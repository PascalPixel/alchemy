typedef int s32;

extern void Func_020059b2(s32, s32, s32, s32, s32, s32);
extern void Func_020059c4(s32, s32, s32, s32, s32, s32);
extern void Func_020059d6(s32, s32, s32, s32, s32, s32);
extern void Func_020059e8(s32, s32, s32, s32, s32, s32);

void Func_020012f0(void)
{
    s32 a = 0x2a;
    s32 b;

    Func_020059b2(29, 22, 1, 1, 3, a);
    b = 2;
    Func_020059c4(29, 21, 1, 1, b, a);
    Func_020059d6(29, 21, 1, 1, 4, a);
    Func_020059e8(23, 20, 3, 1, b, 0x2b);
}
