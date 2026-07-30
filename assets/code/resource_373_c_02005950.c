typedef unsigned char u8;
typedef signed int s32;

extern void Func_0200b8a2(s32, s32, s32, s32, s32, s32);
extern s32 Func_0200b908(s32);
extern s32 *Func_0200b95e(s32);
extern void Func_0200b934(s32);
extern void Func_0200b93a(s32);
extern void Func_0200b940(s32);
extern void Func_0200b8ea(s32, s32, s32, s32, s32, s32);
extern void Func_0200b95a(s32);
extern void Func_0200b90a(s32, s32, s32, s32, s32, s32);
extern void Func_0200b978(s32);
extern void Func_0200b926(s32, s32, s32, s32, s32, s32);
extern void Func_0200b994(s32);
extern u8 Value_0000087a;
extern u8 Value_00000315;
extern u8 Value_00000316;

void Func_02005950(void)
{
    s32 *p;
    s32 v;
    {
        s32 e = 22;
        s32 f = 36;
        Func_0200b8a2(17, 0, 3, 1, e, f);
    }
    if (Func_0200b908((s32)&Value_0000087a) != 0)
        p = Func_0200b95e(21);
    else
        p = Func_0200b95e(20);
    if (p == 0)
        return;
    Func_0200b934(0x314);
    Func_0200b93a(0x315);
    Func_0200b940(0x316);
    v = p[2] >> 20;
    if (v == 22) {
        Func_0200b8ea(17, 1, 1, 1, v, 36);
        Func_0200b95a(0x314);
    } else if (v == 23) {
        Func_0200b90a(17, 1, 1, 1, v, 36);
        Func_0200b978(0x315);
    } else {
        s32 g = 24;
        s32 h = 36;
        Func_0200b926(17, 1, 1, 1, g, h);
        Func_0200b994(0x316);
    }
}
