typedef signed int s32;
typedef unsigned char u8;

extern s32 *Func_0200406c(s32);
extern void Func_02004012(s32, s32, s32, s32, s32, s32);
extern void Func_02004054(s32);

void Func_020004cc(void)
{
    s32 *p;
    s32 s0;
    s32 s1;

    p = Func_0200406c(9);
    s0 = 17;
    s1 = 13;
    Func_02004012(29, 1, 3, 1, s0, s1);
    if (p != 0) {
        ((u8 *)p)[0x55] = 2;
    }
    Func_02004054(0x201);
}
