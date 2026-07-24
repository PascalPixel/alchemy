typedef signed int s32;

s32 Func_080284dc(void);
s32 Func_080287a8(s32);
s32 Func_08028808(s32, s32, s32);
s32 Func_08028574(s32);
s32 Func_0802851c(void);

s32 Func_08028df4(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    s32 flag;

    flag = 0;

    Func_080284dc();

    if (arg2 == 0) {
        arg2 = 3;
    }
    if (arg0 != 0) {
        flag = 17;
    }

    Func_080287a8(5);
    Func_080287a8(6);
    Func_08028808(flag, arg2, arg1);

    arg3 = Func_08028574(arg3);
    Func_0802851c();
    if (arg3 == -1) {
        arg3 = 1;
    }
    return arg3;
}
