#include "types.h"

u8 *Func_08077008(s32);

s32 Func_080b7514(void) {
    s32 i = 0;
    s32 id;
    goto test;
again:
    i++;
test:
    if (i > 5) {
        goto done;
    }
    id = i + 0x80;
    if (Func_08077008(id)[0x12A] != 0) {
        goto again;
    }
done:
    if (i == 6) {
        return -1;
    }
    return id;
}
