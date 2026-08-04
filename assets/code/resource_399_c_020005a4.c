#include "types.h"

extern void Func_020027d8(void);
extern s32 Func_020027be(s32);
extern void Func_020028b8(s32);
extern void Func_020028c0(s32);
extern void Func_020028d0(s32, s32);
extern void Func_02002804(void);

void Func_020005a4(void)
{
    Func_020027d8();
    if (Func_020027be(3) != 0) {
        Func_020028b8(0x1575);
    } else {
        Func_020028c0(0x155B);
    }
    Func_020028d0(20, 0);
    Func_02002804();
}
