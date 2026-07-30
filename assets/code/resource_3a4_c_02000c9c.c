typedef signed int s32;
typedef unsigned char u8;

extern void Func_02004812(void);
extern void Func_020047de(s32, s32, s32, s32, s32, s32);
extern void Func_020049dc(s32);
extern void Func_02004866(s32, s32, s32);
extern void Func_02004870(s32, s32, s32);
extern u8 *Func_0200486e(s32);
extern void Func_020048d8(s32, s32);
extern void Func_020048b6(s32, s32, s32);
extern void Func_020048c4(s32, s32, s32);
extern void Func_020048ea(s32);
extern void Func_02004902(s32, s32);
extern void Func_02001122(void);
extern void Func_02004892(void);

void Func_02000c9c(void)
{
    s32 s0;
    s32 s1;
    u8 *p;

    Func_02004812();
    s0 = 24;
    s1 = 26;
    Func_020047de(24, 27, 2, 1, s0, s1);
    Func_020049dc(185);
    Func_02004866(10, 0x3333, 0x1999);
    Func_02004870(0, 0x3333, 0x1999);
    p = Func_0200486e(10);
    p[0x5a] &= 0xfe;
    Func_020048d8(0, 8);
    Func_020048b6(0, 400, 424);
    Func_020048c4(10, 408, 424);
    Func_020048ea(10);
    Func_02004902(0, 1);
    Func_02001122();
    Func_02004892();
}
