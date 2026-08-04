#include "types.h"

void Func_0800615c(s32 value);
void Func_080060e8(s32 value);

u32 Func_08005ee0(s32 value, s32 other)
{
    u8 *state = (u8 *)0x02002240;
    volatile u8 *serial = (volatile u8 *)0x04000128;
    u32 status = *(volatile u32 *)serial;
    u32 result;

    switch (state[1]) {
    case 0:
        if ((status & 0x30) == 0) {
            u8 mode = status & 0x88;

            if (mode == 8) {
                u8 error = status & 4;

                if (error == 0 && *(s32 *)(state + 0x14) == -1) {
                    volatile u16 *biosFlag = (volatile u16 *)0x000000C0;
                    volatile u16 *ime = (volatile u16 *)0x04000208;

                    *biosFlag = error;
                    *ime = (*ime & -129) | 0x40;
                    *biosFlag = 1;
                    serial[1] &= -65;
                    *(volatile u16 *)0x04000202 = 0xC963;
                    *(volatile u32 *)0x0400010C = (u32)state;
                    state[0] = mode;
                }
                goto activate;
            }
            break;
        }

activate:
        state[1] = 1;
        /* Fall through. */
    case 1:
        Func_0800615c(other);
        Func_080060e8(value);
        state = (u8 *)0x02002240;
        break;
    }

    state[0x0B]++;
    result = state[3] | (state[2] << 8);
    if (state[0] == 8)
        result |= 0x80;
    if (state[9] != 0)
        result |= 0x1000;
    if (((status << 26) >> 30) > 1)
        result |= 0x2000;
    return result;
}
