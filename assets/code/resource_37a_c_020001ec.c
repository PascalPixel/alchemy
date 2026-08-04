#include "types.h"

extern s32 Func_02002ba6(s32);
extern void Func_02002cba(s32, s32);
extern void Func_02002cd0(s32);
extern void Func_02002bc6(s32);
extern void Func_02002bd6(s32);
extern void Func_02002bdc(s32);

void Func_020001ec(void)
{
    if (Func_02002ba6(0x202) == 0) {
        Func_02002cba(0x202db1, 1);
        Func_02002cd0(20);
        Func_02002bc6(0x202);
        Func_02002bd6(0x200);
        Func_02002bdc(0x201);
    }
}
