typedef signed int s32;

void Func_02006678(void);
s32 Func_020067d2(s32, s32, s32);
s32 Func_0200669c(s32, s32);
void Func_02006714(s32, s32);
void Func_02006820(s32);
void Func_02006728(s32, s32);
void Func_02006636(s32);
void Func_020066c2(void);

s32 Func_02001038(s32 a, s32 b)
{
    s32 r = 0;
    s32 p;

    Func_02006678();
    p = Func_020067d2(0, a, b);
    if (Func_0200669c(b, 0) != -1) {
        Func_02006714(a, 2);
        r = 1;
    } else {
        Func_02006820(0x7d);
        Func_02006728(a, 5);
    }
    Func_02006636(p);
    Func_020066c2();
    return r;
}
