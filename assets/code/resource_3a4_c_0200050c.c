typedef signed int s32;
typedef unsigned char u8;

extern s32 *Func_020040ac(s32);
extern void Func_02004050(s32, s32, s32, s32, s32, s32);
extern void Func_02004064(s32 *, s32);
extern void Func_020040a4(s32);

void Func_0200050c(void)
{
    s32 *p;
    s32 s0;

    p = Func_020040ac(9);
    s0 = 26;
    Func_02004050(0, 0, 1, 1, s0, s0);
    if (p != 0) {
        Func_02004064(p, 0);
        p[3] += 0xffe00000;
        ((u8 *)p)[0x23] = 2;
    }
    Func_020040a4(0x200);
}
