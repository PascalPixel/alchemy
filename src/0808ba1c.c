#include "types.h"

extern u8 *Data_03001ebc;

s32 Func_0808ba1c(u32 arg0) {
    u8 *base = Data_03001ebc;
    u32 offset;
    if (arg0 > 0xbf)
        return 0;
    offset = (arg0 * 4) + 0x14;
    return *(s32 *)(base + offset);
}
