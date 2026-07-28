typedef signed int s32;
typedef unsigned int u32;

extern void Func_02005dd2(void);
extern void Func_02005e5a(s32, s32);
extern void Func_02005e90(s32);
extern void Func_02005eae(s32, s32, s32);
extern void Func_02005e96(s32, s32);
extern void Func_02005d0c(s32);
extern void Func_02005ea4(s32, s32);
extern void Func_02005d1a(s32);
extern void Func_02005e04(s32);
extern void Func_02005e86(s32, s32, s32);
extern void Func_02005dda(s32, s32, s32, s32, s32, s32);
extern void Func_02005e46(void);

void Func_02000f38(void)
{
    u32 n;
    u32 w;
    s32 a;
    s32 b;

    Func_02005dd2();
    Func_02005e5a(8, 3);
    Func_02005e90(0x266d);
    n = 10;
    w = 8;
    Func_02005eae(8, 0, 20);
    do {
        Func_02005e96(8, 15);
        Func_02005d0c(2);
        Func_02005ea4(8, 0);
        Func_02005d1a(w);
        if (w > 3) {
            w--;
        }
        n--;
    } while (n != 0);
    Func_02005e04(0x981);
    Func_02005e86(8, 0, 0);
    a = 7;
    b = 16;
    Func_02005dda(7, 17, 2, 1, a, b);
    Func_02005e46();
}
