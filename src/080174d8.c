#include "types.h"

s32 Func_08016418(s32, s32);

void Func_080174d8(void) {
    s32 *temp_r5;
    s32 temp_r0;

    temp_r5 = *(s32 **)0x03001EE4;
    temp_r0 = *temp_r5;
    if (temp_r0 != 0) {
        Func_08016418(temp_r0, 1);
        *temp_r5 = 0;
    }
}
