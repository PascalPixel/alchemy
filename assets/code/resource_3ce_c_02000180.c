#include "types.h"

extern u8 Value_00000d21;
extern u8 Value_00000d4c;
extern s32 Func_02000206();

void Func_02000180(void) {
    Func_02000206((s32)&Value_00000d21, (s32)&Value_00000d4c - (s32)&Value_00000d21);
}
