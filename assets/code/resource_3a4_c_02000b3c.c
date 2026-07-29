typedef signed int s32;

extern void Func_020046b2(void);
extern void Func_02004692(s32, s32);
extern void Func_02004870(s32);
extern s32 Func_020046a6(s32);
extern void Func_02004696(s32, s32, s32, s32, s32, s32);
extern void Func_0200468a(s32, s32, s32, s32, s32, s32);
extern void Func_020046e8(s32);
extern void Func_020046c6(s32, s32, s32, s32, s32, s32);
extern void Func_020046ba(s32, s32, s32, s32, s32, s32);
extern void Func_02004710(s32);
extern void Func_02004636(s32);
extern void Func_020048da(void);
extern void Func_0200473e(void);

void Func_02000b3c(void)
{
    s32 s0;
    s32 s1;

    Func_020046b2();
    Func_02004692(0x1528, 1);
    Func_02004870(125);
    if (Func_020046a6(0x326) != 0) {
        s0 = 16;
        s1 = 92;
        Func_02004696(15, 93, 1, 1, s0, s1);
        s0 = 1;
        s1 = 2;
        Func_0200468a(47, 29, 16, 28, s0, s1);
        Func_020046e8(0x326);
    } else {
        s0 = 16;
        s1 = 92;
        Func_020046c6(17, 93, 1, 1, s0, s1);
        s0 = 1;
        s1 = 2;
        Func_020046ba(46, 29, 16, 28, s0, s1);
        Func_02004710(0x326);
    }
    Func_02004636(20);
    Func_020048da();
    Func_0200473e();
}
