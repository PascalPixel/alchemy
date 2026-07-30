typedef signed int s32;

extern void Func_02007d7e(void);
extern s32 Func_02003bb6(void);
extern void Func_02007d3a(s32, s32, s32, s32, s32, s32);
extern void Func_02007d4e(s32, s32, s32, s32, s32, s32);
extern void Func_02003292(void);
extern void Func_02007dbe(void);
extern void Func_02005a3a(void);

void Func_02002ee4(void)
{
    Func_02007d7e();
    if (Func_02003bb6() == 0) {
        { s32 k5 = 5, k6 = 48; Func_02007d3a(69, 48, 4, 2, k5, k6); }
        { s32 j5 = 9, j6 = 37; Func_02007d4e(73, 37, 9, 13, j5, j6); }
        Func_02003292();
    }
    Func_02007dbe();
    Func_02005a3a();
}
