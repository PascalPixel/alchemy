typedef signed int s32; typedef unsigned char u8;

extern s32 Func_020077c8(s32);
extern void Func_02007800(void);
extern void Func_020077e6(s32);
extern s32 *Func_0200782c(s32);
extern s32 *Func_0200783c(s32);
extern s32 *Func_02007844(s32);
extern void Func_02007826(s32);
extern void Func_02007790(s32, s32, s32, s32, s32, s32);
extern void Func_020079f6(s32);
extern void Func_02007798(s32, s32, s32);
extern void Func_0200785c(void);
extern u8 Data_0200e08e[];

void Func_02001d3c(void)
{
    if (Func_020077c8(0x256) == 0)
        return;
    Func_02007800();
    Func_020077e6(0x256);
    Func_0200782c(0)[3] += 0x20000;
    {
        s32 *p = Func_0200783c(0);
        p[15] = Func_02007844(0)[3];
    }
    Func_02007826(5);
    Func_02007790(8, 29, 10, 23, 1, 1);
    Func_020079f6(0xd9);
    Func_02007798((s32)Data_0200e08e, 10, 18);
    Func_0200785c();
}
