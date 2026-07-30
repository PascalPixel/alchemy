typedef signed int s32; typedef signed short s16; typedef unsigned char u8;

extern s32 Func_02007728(s32);
extern s16 *Func_02007782(s32);
extern s16 *Func_0200778c(s32);
extern void Func_02007788(void);
extern void Func_02007766(s32);
extern s32 *Func_020077ba(s32);
extern s32 *Func_020077c8(s32);
extern s32 *Func_020077d0(s32);
extern void Func_02007716(s32, s32, s32, s32, s32, s32);
extern void Func_0200797c(s32);
extern void Func_0200771e(s32, s32, s32);
extern void Func_020077e2(void);
extern u8 Data_0200e064[];

void Func_02001c9c(void)
{
    s32 a, b;

    if (Func_02007728(0x256) != 0)
        return;
    a = Func_02007782(0)[5];
    b = Func_0200778c(0)[9];
    if (a < 164 || a > 171)
        return;
    if (b < 372)
        return;
    if (b >= 380)
        return;
    Func_02007788();
    Func_02007766(0x256);
    Func_0200778c(5);
    Func_020077ba(0)[3] += 0xfffe0000;
    {
        s32 *p = Func_020077c8(0);
        p[15] = Func_020077d0(0)[3];
    }
    Func_02007716(6, 29, 10, 23, 1, 1);
    Func_0200797c(0xd9);
    Func_0200771e((s32)Data_0200e064, 10, 18);
    Func_020077e2();
}
