typedef signed int s32;
typedef unsigned char u8;

extern s32 *Func_0200401c(s32);
extern void Func_02003fc2(s32, s32, s32, s32, s32, s32);
extern void Func_02003fd6(s32 *, s32);
extern void Func_02004016(s32);

void Func_0200047c(void)
{
    s32 *p;
    s32 s0;
    s32 s1;

    p = Func_0200401c(8);
    s0 = 9;
    s1 = 13;
    Func_02003fc2(7, 13, 1, 1, s0, s1);
    if (p != 0) {
        Func_02003fd6(p, 0);
        p[3] += 0xffe00000;
        ((u8 *)p)[0x23] = 2;
    }
    Func_02004016(0x200);
}
