typedef signed int s32;
typedef unsigned char u8;

extern u8 Data_00000024[];

s32 Func_080284dc(void);
s32 Func_080287a8(s32);
s32 Func_080288a8(s32, s32, s32, u8 *);
s32 Func_08028574(s32);
s32 Func_0802851c(void);

s32 Func_08028e54(s32 arg0, s32 arg1, s32 arg2)
{
    s32 var_r7;
    u8 *p;

    var_r7 = arg2;
    p = Data_00000024;
    Func_080284dc();
    Func_080287a8(5);
    Func_080287a8(6);
    Func_080288a8(arg0, arg1, 3, p);
    var_r7 = Func_08028574(var_r7);
    Func_0802851c();
    if (var_r7 == -1) {
        var_r7 = 1;
    }
    return var_r7;
}
