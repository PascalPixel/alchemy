typedef signed int s32; typedef unsigned char u8;

extern s32 *Func_020076cc(s32);
extern s32 *Func_020076de(s32);
extern void Func_020076ce(void);
extern s32 *Func_020076fc(s32);
extern s32 *Func_0200770a(s32);
extern s32 *Func_02007712(s32);
extern void Func_02007658(s32, s32, s32, s32, s32, s32);
extern void Func_020078be(s32);
extern void Func_02007660(s32, s32, s32);
extern void Func_02007692(s32, s32, s32, s32, s32, s32);
extern void Func_02007708(s32);
extern void Func_0200773c(void);
extern u8 Data_0200e064[];

void Func_02001bf0(void)
{
    s32 x = Func_020076cc(8)[2] / 0x100000;
    s32 y = Func_020076de(8)[4] / 0x100000;

    Func_020076ce();
    if (x == 10 && y == 23) {
        s32 *p;
        Func_020076fc(8)[3] += 0xfffe0000;
        p = Func_0200770a(8);
        p[15] = Func_02007712(8)[3];
        Func_02007658(6, 29, 10, 23, 1, 1);
        Func_020078be(0xd9);
        Func_02007660((s32)Data_0200e064, 10, 18);
        Func_02007692(10, 16, 1, 1, x, 19);
        Func_02007708(0x878);
    }
    Func_0200773c();
}
