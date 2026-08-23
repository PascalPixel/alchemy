#include "types.h"

s32 GameFlag_IsSet();

s32 Func_0808d428(s32 arg0) {
    s32 temp_r0;

    if (arg0 == -1) {
        return 1;
    }
    if (0x1000 & arg0) {
        return GameFlag_IsSet();
    }
    temp_r0 = GameFlag_IsSet();
    return 1 - ((u32) ((0 - temp_r0) | temp_r0) >> 0x1F);
}
