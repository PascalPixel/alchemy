#include "types.h"

void Func_020051ac(s32, s32);
void Func_020051b4(s32, s32);

s32 Func_020026a4(s32 arg0) {
    if ((*(u32 *)0x03001e40 >> 1) & 1) {
        Func_020051ac(arg0, 10);
    } else {
        Func_020051b4(arg0, 7);
    }
    return 0;
}
#include "resource_38f.h"
