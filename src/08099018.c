#include "types.h"

s32 Func_08009240(s32, s32);

void Func_08099018(s32 arg0) {
    s32 mode = *(s32 *)0x03001E40 & 7;
    if (mode == 0) {
        Func_08009240(arg0, 2);
    } else if (mode == 2) {
        Func_08009240(arg0, 0);
    }
}
