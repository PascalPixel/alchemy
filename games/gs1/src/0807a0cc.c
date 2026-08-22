#include "types.h"

extern const u16 Data_0808926c[];

const u16 *Func_0807a0cc(u32 group, u32 index)
{
    s32 entry;

    entry = 0;
    if ((group <= 3U) && (index <= 0x13U)) {
        entry = (group * 0x14) + index;
    }
    return (const u16 *)((u8 *)Data_0808926c + entry * 0xC);
}
