#include "types.h"

s32 Func_080072f0(s32, s32, s32, s32);

void Func_08003d04(void) {
    *(s8 *)0x03001D00 = 0;
    Func_080072f0(0x03001400, 0x400, 0x03001D00, 0x03000164);
}
