typedef int s32;
typedef unsigned char u8;
extern u8 Value_00000ed0;

extern s32 Func_02005dae(s32);
extern void Func_02005dd6(void);
extern void Func_02005ea0(s32, s32, s32);
extern void Func_02005dde(s32);
extern void Func_02005ec4(s32);
extern void Func_02005edc(s32, s32);
extern void Func_02005f00(s32, s32, s32);
extern void Func_02005e0c(void);
extern s32 Func_02005dec(s32);
extern void Func_02005e14(void);
extern void Func_02005ef2(s32);
extern void Func_02002e5e(void);
extern void Func_02005e2a(void);

void Func_020016cc(void)
{
    if (Func_02005dae(0x841) != 0) {
        Func_02005dd6();
        Func_02005ea0(22, 0, 0);
        Func_02005dde(20);
        Func_02005ec4((s32)&Value_00000ed0);
        Func_02005edc(22, 0);
        Func_02005f00(22, 0xe000, 10);
        Func_02005e0c();
    } else if (Func_02005dec(0x837) == 0) {
        Func_02005e14();
        Func_02005ef2(0xe6e);
        Func_02002e5e();
        Func_02005e2a();
    }
}
