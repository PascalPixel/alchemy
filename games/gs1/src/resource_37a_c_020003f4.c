#include "types.h"

extern s32 Func_02002db0(s32);
extern void Func_02002da8(s32, s32, s32, s32, s32, s32);
extern s32 Func_02002dce(s32);
extern void Func_02002dc6(s32, s32, s32, s32, s32, s32);
extern s32 Func_02002dec(s32);
extern void Func_02002de4(s32, s32, s32, s32, s32, s32);
extern s32 Func_02002e0a(s32);
extern void Func_02002e02(s32, s32, s32, s32, s32, s32);

void Func_020003f4(void)
{
    if (Func_02002db0(0x826) != 0) {
        s32 a = 2;
        s32 b = 1;
        Func_02002da8(0x2d, 30, 0x22, 10, a, b);
    }
    if (Func_02002dce(0x827) != 0) {
        s32 a = 2;
        s32 b = 1;
        Func_02002dc6(0x2f, 30, 0x24, 10, a, b);
    }
    if (Func_02002dec(0x828) != 0) {
        s32 a = 2;
        s32 b = 1;
        Func_02002de4(0x2d, 31, 0x22, 11, a, b);
    }
    if (Func_02002e0a(0x829) != 0) {
        s32 a = 2;
        s32 b = 1;
        Func_02002e02(0x2f, 31, 0x24, 11, a, b);
    }
}
