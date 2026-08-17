#include "types.h"

extern s32 Func_020028bc();
extern s32 Func_020028a2();
extern s32 Func_0200299c();
extern s32 Func_020029a4();
extern s32 Func_020029b4();
extern s32 Func_020028e8();

void Func_02000688(void) {
    Func_020028bc();
    if (Func_020028a2(3) != 0) {
        Func_0200299c(0x1573);
    } else {
        Func_020029a4(0x155a);
    }
    Func_020029b4(19, 0);
    Func_020028e8();
}
