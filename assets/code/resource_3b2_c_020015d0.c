typedef signed int s32;
typedef unsigned char u8;

extern u8 *Func_02004660(s32 arg0);
extern s32 Func_02004672(s32 arg0);
extern void Func_02004628(s32 arg0, s32 arg1);
extern void Func_02004648(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_02004658(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_02004676(s32 arg0);

void Func_020015d0(s32 arg0) {
    s32 a = 0x900000;
    s32 b = 0x1400000;
    s32 c = 0x2f00000;
    s32 d = 0x1400000;
    u8 *p = Func_02004660(14);

    if (p != 0) {
        p[89] = 0;
    }
    Func_02004628(Func_02004672(arg0), 0);
    Func_02004648(0, a, b, 253);
    Func_02004658(0, c, d, 253);
    Func_02004676(579);
}
