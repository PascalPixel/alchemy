#include "TYPES.H"

extern u8 gDebugTextEnabled;
extern u16 *gDebugTextCursor;

void Runtime_WriteDebugTextTiles(const u8 *src)
{
    if (gDebugTextEnabled != 0) {
        u32 addr = ((u32)&gDebugTextCursor);
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
            addr = ((u32)&gDebugTextCursor);
        }
        *(u16 **)addr = dst;
    }
}
