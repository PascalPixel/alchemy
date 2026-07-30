typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16; typedef signed int s32; typedef unsigned int u32;
#define F(base, type, off) (*(type *)((u8 *)(base) + (off)))

extern s32 Func_02006f58(s32);
extern u8 *Func_02006fb2();
extern u8 *Func_02006fbc(s32);
extern void Func_02006f90(s32);
extern void Func_02006fb6(s32);
extern u8 *Func_02006fe4(s32);
extern u8 *Func_02006ff2(s32);
extern u8 *Func_02006ffa(s32);
extern void Func_02006f40(s32, s32, s32, s32, s32, s32);
extern void Func_020071a6(s32);
extern void Func_02006f48(s32, s32, s32);
extern void Func_0200700c(void);
extern u8 Value_00000256;
extern u8 Data_0200e010[];

void Func_020014cc(void)
{
    s32 a;
    s32 b;
    u8 *p;

    if (Func_02006f58(0x256) != 0) {
        return;
    }
    a = F(Func_02006fb2(0), s16, 10);
    b = F(Func_02006fbc(0), s16, 18);
    a -= 0x54;
    if ((u32)a <= 7 && b > 0xd3 && b <= 0xdb) {
        Func_02006fb2();
        Func_02006f90(0x256);
        Func_02006fb6(5);
        F(Func_02006fe4(0), s32, 12) += 0xfffe0000;
        p = Func_02006ff2(0);
        F(p, s32, 0x3c) = F(Func_02006ffa(0), s32, 12);
        Func_02006f40(5, 2, 5, 11, 1, 1);
        Func_020071a6(0xd9);
        Func_02006f48((s32)Data_0200e010, 9, 7);
        Func_0200700c();
    }
}
