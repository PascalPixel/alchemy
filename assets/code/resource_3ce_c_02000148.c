#include "types.h"

extern u8 Value_00000c9b;
extern u8 Value_00000cc6;
extern s32 Func_020001ce();

void Func_02000148(void) {
    Func_020001ce((s32)&Value_00000cc6, (s32)&Value_00000cc6 - (s32)&Value_00000c9b);
}
