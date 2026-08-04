#include "types.h"

extern void Func_080030f8(u32);
extern void Func_080f9010(u32);
extern void Func_0801b664(void *);
extern void Func_0801b810(void *);

u32 Func_0801b424(u32 value)
{
    u8 *state = *(u8 **)0x03001e98;
    volatile u32 *input;
    u32 result;

again:
    Func_080030f8(1);
    if (*(u16 *)(state + 0x3a0) != 0)
        goto again;

    if (value != 999) {
        input = (u32 *)0x03001b04;
        if (*input & 0x10) {
            Func_080f9010(111);
            Func_0801b664(state);
        } else if (*input & 0x20) {
            Func_080f9010(111);
            Func_0801b810(state);
        }

        if (*(u32 *)0x03001c94 & 1) {
            result = *(u16 *)(state + 0x39c)
                   + *(u16 *)(state + 0x39e);
            if (*(u16 *)(*(u8 **)(state + 0x348) + 10) == 6) {
                if (result == 0)
                    Func_080f9010(112);
                else
                    Func_080f9010(113);
            } else {
                Func_080f9010(112);
            }
            return result;
        }
    }

    if (value != 0 && (*(u32 *)0x03001c94 & 2)) {
        Func_080f9010(113);
        return -1;
    }
    goto again;
}
