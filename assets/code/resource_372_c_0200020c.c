#include "types.h"

extern void Func_02004ac6(s32);
extern s32 Func_020048f4(s32);
extern s32 Func_020048fe(s32);
extern void Func_020041da(void);
extern void Func_0200038c(s32);
extern u8 Value_00000841;
extern u8 Value_00000842;

void Func_0200020c(void)
{
    Func_02004ac6(0x7B);
    if (Func_020048f4((s32)&Value_00000841) != 0
        && Func_020048fe((s32)&Value_00000842) == 0) {
        Func_020041da();
    }
    Func_0200038c(2);
}
