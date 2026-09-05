#include "low_runtime.h"
#include "global_cells.h"

#define Runtime_WriteDebugTextTiles Func_080046c4

void Runtime_WriteDebugTextTiles(const u8 *src)
{
    if (*(u8 *)ADDR_03001AC4 != 0) {
        u32 addr = ADDR_03001CBC;
        u32 c = *src;
        u16 *dst = *(u16 **)addr;
        u32 cnt = 0;
        src++;

        if (c != 0) {
            u32 mask = 0xf000;
            addr = 0x06002500;
            do {
                *dst++ = c | mask;
                if (dst == (u16 *)addr)
                    dst = (u16 *)0x06002000;
                cnt++;
                if (cnt > 31)
                    break;
                c = *src++;
            } while (c != 0);
            addr = ADDR_03001CBC;
        }
        *(u16 **)addr = dst;
    }
}
