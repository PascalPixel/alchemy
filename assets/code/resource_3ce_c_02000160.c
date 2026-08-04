#include "types.h"

extern u8 Value_00000c9b;
extern u8 Value_00000cc6;
extern u8 Value_00000cf1;
extern s32 Func_020001e8();

void Func_02000160(void) {
    Func_020001e8((s32)&Value_00000cf1, (s32)&Value_00000cc6 - (s32)&Value_00000c9b);
}
