#include "types.h"

u8 *Func_08077008(void);
s32 Func_080bf208(s32, u8, s32);

s32 Func_080bf440(s32 arg0) {
    u8 *value = Func_08077008() + 0x13B;
    if (*value != 0) {
        (*value)--;
        if (*value == 0) {
            return 1;
        }
        if (Func_080bf208(arg0, *value, 40) != 0) {
            *value = 0;
            return 1;
        }
    }
    return 0;
}
