#include "types.h"

u8 *Func_08077008(s32);

s32 Func_080b7514(void) {
    s32 i;
    s32 id;

    for (i = 0; i <= 5; i++) {
        id = i + 0x80;
        if (Func_08077008(id)[0x12A] == 0)
            break;
    }
    if (i == 6) {
        return -1;
    }
    return id;
}
