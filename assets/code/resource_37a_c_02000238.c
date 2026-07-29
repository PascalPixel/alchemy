typedef signed int s32;

extern s32 Func_02002bf4(s32);
extern void Func_02002bec(s32, s32, s32, s32, s32, s32);
extern s32 Func_02002c12(s32);
extern void Func_02002c0a(s32, s32, s32, s32, s32, s32);
extern s32 Func_02002c30(s32);
extern void Func_02002c28(s32, s32, s32, s32, s32, s32);
extern s32 Func_02002c4e(s32);
extern void Func_02002c46(s32, s32, s32, s32, s32, s32);

void Func_02000238(void)
{
    if (Func_02002bf4(0x80b) != 0) {
        s32 a = 2;
        s32 b = 1;
        Func_02002bec(0x2d, 28, 0x22, 10, a, b);
    }
    if (Func_02002c12(0x80c) != 0) {
        s32 a = 2;
        s32 b = 1;
        Func_02002c0a(0x2f, 28, 0x24, 10, a, b);
    }
    if (Func_02002c30(0x80d) != 0) {
        s32 a = 2;
        s32 b = 1;
        Func_02002c28(0x2d, 29, 0x22, 11, a, b);
    }
    if (Func_02002c4e(0x80e) != 0) {
        s32 a = 2;
        s32 b = 1;
        Func_02002c46(0x2f, 29, 0x24, 11, a, b);
    }
}
