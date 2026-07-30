typedef int s32;

extern s32 Func_02005ce2(s32);
extern s32 Func_02005cec(s32);
extern void Func_02005d14(void);
extern void Func_02005df2(s32);
extern void Func_02005e14(s32, s32, s32);
extern void Func_02005e3e(s32, s32, s32);
extern void Func_02005dba(s32, s32, s32);
extern void Func_02005e04(s32, s32, s32);
extern void Func_02005e04_b(s32, s32);
extern void Func_02005d4a(s32);
extern void Func_02005e42(s32, s32);
extern void Func_02005d48(s32);
extern void Func_02005d6c(void);

void Func_02001600(void)
{
    s32 p1;
    s32 p2;
    if (Func_02005ce2(0x836) == 0 && Func_02005cec(0x837) == 0) {
        Func_02005d14();
        Func_02005df2(0xe6c);
        Func_02005e14(22, 0, 20);
        p1 = 0x101;
        Func_02005e3e(0, p1, 40);
        p2 = 0x26b;
        Func_02005dba(0, 0x17e, p2);
        Func_02005e04(0, 22, 0);
        Func_02005e04_b(0, 2);
        Func_02005d4a(30);
        Func_02005e42(22, 0);
        Func_02005d48(0x836);
        Func_02005d6c();
    }
}
