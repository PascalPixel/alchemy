#include "types.h"

u8 *Runtime_GetObject(void);

s32 Func_080bf524(void) {
    u8 *value = Runtime_GetObject() + 0x13E;
    if (*value != 0) {
        (*value)--;
        if (*value == 0) {
            return 1;
        }
    }
    return 0;
}
