typedef signed int s32; typedef unsigned int u32;

extern s32 *Func_02007d9e(s32);
extern u32 Func_02007c64(void);
extern void Func_02005e06(s32, s32, s32);

void Func_020022c4(void)
{
    s32 *p = Func_02007d9e(9);
    u32 v = Func_02007c64();

    s32 b = p[3] + (((v << 2) >> 16) << 16);
    s32 c = p[4];

    Func_02005e06(p[2], b, c);
}
