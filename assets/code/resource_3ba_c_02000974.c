typedef signed int s32;

extern void Func_0200463c(s32);
extern void Func_02004642(s32);
extern void Func_02004618(s32);
extern void Func_0200462c(void);
extern void Func_02004692(s32, s32, s32);
extern void Func_020046a0(s32, s32, s32);
extern void Func_020046d4();
extern void Func_020046e0(s32, s32, s32);
extern void Func_020046cc(s32, s32);
extern void Func_020046de(s32, s32);
extern void Func_020046e6(s32, s32);
extern void Func_020044f4(s32);
extern void Func_02004774(s32, s32, s32, s32);
extern void Func_02003246(s32);
extern void Func_020046ae(void);

void Func_02000974(s32 a)
{
    Func_0200463c(24);
    Func_02004642(25);
    Func_02004618(1);
    Func_0200462c();
    Func_02004692(8, 0x5280000, 0xC00000);
    Func_020046a0(0, 0x5080000, 0xC00000);
    Func_020046d4(8, 0x4000, 0);
    Func_020046e0(0, 0x4000, 0);
    if (a < 0) {
        Func_020046cc(8, 10);
        Func_020046d4(0, 35);
    } else {
        Func_020046de(8, 8);
        Func_020046e6(0, 28);
    }
    Func_020044f4(1);
    Func_02004774(0x5180000, 0, 0x800000, 0);
    Func_02003246(a);
    Func_020046ae();
}
