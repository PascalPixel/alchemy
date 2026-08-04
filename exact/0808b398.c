#include "types.h"

s32 Func_080770c0(s32);

s32 Func_0808b398(s32 arg0) {
    s32 result;

    result = arg0;
    if (result <= 8) {
        if (Func_080770c0(0x20) != 0) {
            if (result == 0) {
                result = 0x12;
            }
            if (result == 1) {
                result = 0x13;
            }
        } else if ((Func_080770c0(0x21) != 0) && (result == 0)) {
            result = 0x11;
        }
    }
    return result;
}
