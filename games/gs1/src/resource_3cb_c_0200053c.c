#include "types.h"
extern s16 Data_02000240[];
extern s32 Func_02001d76(void);
s32 Func_0200053c(u16 *dest)
{
    s32 count = Func_02001d76();
    if (count > 3) count = 3;
    if (count > 0) {
        s16 *p = Data_02000240;
        const u8 *src;
        s32 remaining;
        p += 252;
        src = (const u8 *)p;
        remaining = count;
        do {
            u8 entry = *src++;
            if (dest != 0) { *dest = (u16)entry; dest++; }
            remaining--;
        } while (remaining != 0);
    }
    if (dest != 0) *dest = 0x00ff;
    return count;
}
