#include "types.h"

s16 Func_080b9a44(s32 arg0) {
    u8 *base = *(u8 **)0x03001E74;
    s32 offset;

    if ((arg0 & 0x80) != 0) {
        offset = (arg0 & 0xF) * 2 + 0x64;
        base += 2;
    } else {
        offset = (arg0 & 0xF) * 2 + 0x58;
    }
    return *(s16 *)(base + offset);
}
