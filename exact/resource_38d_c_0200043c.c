#include "types.h"

extern s32 Func_02002892(s32);
extern void Func_02000872(s32);
extern s32 Func_02000948(s32, s32, s32);

void Func_0200043c(void)
{
    if (Func_02002892(0x84E) != 0) {
        Func_02000872(0x210);
    } else {
        Func_02000948(21, 182, 0x210);
    }
}
