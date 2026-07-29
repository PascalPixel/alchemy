typedef signed int s32;

extern s32 Func_02002c88(s32);
extern void Func_02002c80(s32, s32, s32, s32, s32, s32);
extern s32 Func_02002ca6(s32);
extern void Func_02002c9e(s32, s32, s32, s32, s32, s32);
extern s32 Func_02002cc4(s32);
extern void Func_02002cbc(s32, s32, s32, s32, s32, s32);
extern s32 Func_02002ce2(s32);
extern void Func_02002cda(s32, s32, s32, s32, s32, s32);

void Func_020002cc(void)
{
    if (Func_02002c88(0x826) != 0) {
        s32 a = 2;
        s32 b = 1;
        Func_02002c80(0x2d, 28, 0x22, 10, a, b);
    }
    if (Func_02002ca6(0x827) != 0) {
        s32 a = 2;
        s32 b = 1;
        Func_02002c9e(0x2f, 28, 0x24, 10, a, b);
    }
    if (Func_02002cc4(0x828) != 0) {
        s32 a = 2;
        s32 b = 1;
        Func_02002cbc(0x2d, 29, 0x22, 11, a, b);
    }
    if (Func_02002ce2(0x829) != 0) {
        s32 a = 2;
        s32 b = 1;
        Func_02002cda(0x2f, 29, 0x24, 11, a, b);
    }
}
