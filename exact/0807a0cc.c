#include "types.h"

const u16 *Func_0807a0cc(u32 group, u32 index)
{
    s32 entry;

    entry = 0;
    if ((group <= 3U) && (index <= 0x13U)) {
        entry = (group * 0x14) + index;
    }
    return (const u16 *)((entry * 0xC) + 0x0808926C);
}
