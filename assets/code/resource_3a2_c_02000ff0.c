typedef signed int s32;

extern void Func_020024b6(s32, s32, s32, s32, s32, s32);
extern void Func_020024d2(s32, s32, s32, s32, s32, s32);
extern void Func_02002508(s32);

void Func_02000ff0(void)
{
    {
        s32 fifth = 1;
        s32 sixth = 2;

        Func_020024b6(5, 28, 5, 13, fifth, sixth);
    }
    {
        s32 fifth = 5;
        s32 sixth = 13;

        Func_020024d2(5, 28, 1, 2, fifth, sixth);
    }
    Func_02002508(1);
}
