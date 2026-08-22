#include "types.h"

extern s32 Func_02002ac4(s32);
extern void Func_02002aec();
extern void Func_02002bde(s32, s32);
extern void Func_02002bf4(s32);
extern void Func_02002afa(s32);
extern void Func_02002b00(s32);
extern void Func_02002b1c(void);

void Func_02000108(void)
{
    if (Func_02002ac4(0x200) == 0) {
        Func_02002aec();
        Func_02002bde(0x10000, 1);
        Func_02002bf4(20);
        Func_02002aec(0x200);
        Func_02002afa(0x201);
        Func_02002b00(0x202);
        Func_02002b1c();
    }
}
