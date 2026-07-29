typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16; typedef signed int s32; typedef unsigned int u32;
#define F(base, type, off) (*(type *)((u8 *)(base) + (off)))

extern u8 *Func_02006efc(s32);
extern u8 *Func_02006f0e(s32);
extern void Func_02006efe(void);
extern u8 *Func_02006f2c(s32);
extern u8 *Func_02006f3a(s32);
extern u8 *Func_02006f42(s32);
extern void Func_02006e88(s32, s32, s32, s32, s32, s32);
extern void Func_020070ee(s32);
extern void Func_02006e90(s32, s32, s32);
extern void Func_02006ec4(s32, s32, s32, s32, s32, s32);
extern void Func_02006f6e(void);
extern u8 Data_0200e010[];
extern u8 Value_00000874;

void Func_02001420(void)
{
    s32 x;
    s32 y;
    u8 *p;

    x = F(Func_02006efc(11), s32, 8) / 0x100000;
    y = F(Func_02006f0e(11), s32, 16) / 0x100000;
    Func_02006efe();
    if (x == 5 && y == 13) {
        F(Func_02006f2c(11), s32, 12) += 0xfffe0000;
        p = Func_02006f3a(11);
        F(p, s32, 0x3c) = F(Func_02006f42(11), s32, 12);
        Func_02006e88(5, 2, 5, 11, 1, 1);
        Func_020070ee(0xd9);
        Func_02006e90((s32)Data_0200e010, 9, 7);
        {
            s32 s0 = 9;
            s32 s1 = 10;
            Func_02006ec4(9, 5, 1, 1, s0, s1);
        }
        Func_02006f3a((s32)&Value_00000874);
    }
    Func_02006f6e();
}
