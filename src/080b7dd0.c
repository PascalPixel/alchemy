#include "types.h"

void *Func_080b7dd0(s32 arg0) {
    u8 *base = *(u8 **)0x03001E74;
    u8 *result_base = base + 0x74;
    s32 offset;
    if (arg0 > 7) {
        arg0 -= 0x78;
    }
    offset = arg0 + 0x2DC;
    if (base[offset] == 0xFF) {
        return NULL;
    }
    return result_base + base[offset] * 0x2C;
}
