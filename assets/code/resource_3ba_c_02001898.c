typedef signed int s32;

extern s32 Func_02005508(s32);
extern s32 Func_02005512(s32);
extern void Func_020055b8(s32, s32, s32);
extern s32 Func_02005530(s32);
extern s32 Func_0200553a(s32);
extern void Func_020055dc(s32, s32, s32);
extern s32 Func_02005554(s32);
extern s32 Func_0200555e(s32);
extern void Func_02005600(s32, s32, s32);

void Func_02001898(void)
{
    {
        s32 x = Func_02005508(896);
        s32 y = Func_02005512(904);

        x <<= 20;
        x += 0x80000;
        y <<= 20;
        y += 0x80000;
        Func_020055b8(1, x, y);
    }
    {
        s32 x = Func_02005530(912);
        s32 y = Func_0200553a(920);

        x <<= 20;
        x += 0x80000;
        y <<= 20;
        y += 0x80000;
        Func_020055dc(2, x, y);
    }
    {
        s32 x = Func_02005554(928);
        s32 y = Func_0200555e(936);

        x <<= 20;
        x += 0x80000;
        y <<= 20;
        y += 0x80000;
        Func_02005600(3, x, y);
    }
}
