typedef signed int s32;
typedef unsigned char u8;

extern u8 *Func_02004598(s32 arg0);
extern s32 Func_020045aa(s32 arg0);
extern void Func_02004560(s32 arg0, s32 arg1);
extern void Func_02004580(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_020045a0(s32 arg0);

void Func_02001508(s32 arg0) {
    s32 a = 0x1300000;
    s32 b = 0x1700000;
    u8 *p = Func_02004598(11);

    if (p != 0) {
        p[89] = 0;
    }
    Func_02004560(Func_020045aa(arg0), 0);
    Func_02004580(0, a, b, 253);
    Func_020045a0(576);
}
