#include "types.h"

s32 Func_08006408(s32 value)
{
    s32 *busy = (s32 *)0x020023AC;
    u8 *state = (u8 *)0x02002220;
    s32 current = *busy;

    if (current != 0)
        return -1;

    {
        volatile u16 *ime = (volatile u16 *)0x04000208;
        u32 saved;
        saved = *ime;
        *ime = (u16)(u32)ime;
        state[1] = 129;
        *busy = value;
        *(u16 *)0x02002238 = (u16)current;
        state[0] = 1;
        *(u8 *)0x020023A4 = 0;
        *ime = saved;
    }
    return 0;
}
