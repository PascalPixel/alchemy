#include "types.h"

extern u8 Value_00000c9b;
extern u8 Value_00000cc6;
extern u8 Value_00000d4c;
extern s32 Func_02000220();

void Func_02000198(void) {
    Func_02000220((s32)&Value_00000d4c, (s32)&Value_00000cc6 - (s32)&Value_00000c9b);
}
