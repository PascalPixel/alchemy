#include "types.h"

extern s32 Func_02002d1c(s32);
extern void Func_02002d14(s32, s32, s32, s32, s32, s32);
extern s32 Func_02002d3a(s32);
extern void Func_02002d32(s32, s32, s32, s32, s32, s32);
extern s32 Func_02002d58(s32);
extern void Func_02002d50(s32, s32, s32, s32, s32, s32);
extern s32 Func_02002d76(s32);
extern void Func_02002d6e(s32, s32, s32, s32, s32, s32);

void Func_02000360(void)
{
    if (Func_02002d1c(0x80b) != 0) {
        s32 a = 2;
        s32 b = 1;
        Func_02002d14(0x2d, 30, 0x22, 10, a, b);
    }
    if (Func_02002d3a(0x80c) != 0) {
        s32 a = 2;
        s32 b = 1;
        Func_02002d32(0x2f, 30, 0x24, 10, a, b);
    }
    if (Func_02002d58(0x80d) != 0) {
        s32 a = 2;
        s32 b = 1;
        Func_02002d50(0x2d, 31, 0x22, 11, a, b);
    }
    if (Func_02002d76(0x80e) != 0) {
        s32 a = 2;
        s32 b = 1;
        Func_02002d6e(0x2f, 31, 0x24, 11, a, b);
    }
}
