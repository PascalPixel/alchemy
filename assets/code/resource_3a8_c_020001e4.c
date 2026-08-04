#include "types.h"

extern void Func_02003ddc(void);
extern void Func_02003dac(s32, s32);
extern void Func_02003db4(s32, s32);
extern void Func_02003df8(void);

void Func_020001e4(void) {
    Func_02003ddc();
    Func_02003dac(0x947, 1);
    Func_02003db4(0x29df, 1);
    Func_02003df8();
}
