#include "types.h"

typedef int bool;
#define NULL ((void *)0)

u8 *Func_08077008(void);

s32 Func_080bf524(void) {
    u8 *value = Func_08077008() + 0x13E;
    if (*value != 0) {
        (*value)--;
        if (*value == 0) {
            return 1;
        }
    }
    return 0;
}
