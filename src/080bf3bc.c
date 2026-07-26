#include "types.h"

u8 *Func_08077008(void);
s32 Func_080bf208(s32 arg0, s32 arg1, s32 arg2);

s32 Func_080bf3bc(s32 arg0) {
    u8 *value = Func_08077008() + 0x139;
    if (*value != 0) {
        (*value)--;
        if (*value == 0) {
            return 1;
        }
        if (Func_080bf208(arg0, *value, 60) != 0) {
            *value = 0;
            return 1;
        }
    }
    return 0;
}
