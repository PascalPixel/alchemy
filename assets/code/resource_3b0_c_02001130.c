typedef int s32;

extern s32 Func_02002292(s32);
extern s32 Func_0200229a(s32);
extern s32 Func_020022a2(s32);
extern s32 Func_020022aa(s32);

s32 Func_02001130(void)
{
    s32 a;
    s32 b;

    a = Func_02002292(0);
    a += Func_0200229a(2);
    b = Func_020022a2(1);
    b += Func_020022aa(3);
    return a - b;
}
