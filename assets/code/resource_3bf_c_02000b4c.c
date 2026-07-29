typedef signed int s32;
typedef unsigned char u8;

void Func_0200613a(s32, s32, s32, s32, s32, s32);
void Func_0200614c(s32, s32, s32, s32, s32, s32);
u8 *Func_020061ca(s32);
void Func_0200616e(u8 *, s32);

void Func_02000b4c(void)
{
    s32 f = 15;
    u8 *p;

    Func_0200613a(15, 20, 1, 1, f, 22);
    Func_0200614c(17, 23, 1, 3, f, 23);
    p = Func_020061ca(12);
    if (p != 0) {
        Func_0200616e(p, 0);
        p[0x55] = 0;
        p[0x23] = 2;
    }
}
