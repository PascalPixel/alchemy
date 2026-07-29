typedef signed int s32;
typedef unsigned char u8;

extern s32 *Func_020040fc(s32);
extern void Func_020040a2(s32, s32, s32, s32, s32, s32);
extern void Func_020040b6(s32 *, s32);
extern void Func_020040f6(s32);

void Func_0200055c(void)
{
    s32 *p;
    s32 s0;
    s32 s1;

    p = Func_020040fc(9);
    s0 = 25;
    s1 = 13;
    Func_020040a2(23, 13, 1, 1, s0, s1);
    if (p != 0) {
        Func_020040b6(p, 0);
        p[3] += 0xffe00000;
        ((u8 *)p)[0x23] = 2;
    }
    Func_020040f6(0x200);
}
