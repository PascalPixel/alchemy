#include "types.h"

typedef void (*Callback_080f9ef8)(s32);

struct Root_080f9ef8 {
    u8 padding[44];
    Callback_080f9ef8 callback;
};

extern struct Root_080f9ef8 *Data_03007ff0;

void Func_080f9ef8(s32 mask, u8 *state)
{
    u8 flags = state[0];
    u8 *entry;

    mask = 0x80;
    if ((mask & flags) != 0) {
        entry = *(u8 **)(state + 0x20);
        if (entry != 0) {
            s32 zero = 0;

            do {
                if (entry[0] != 0) {
                    s32 kind = entry[1];
                    s32 mask = 7;

                    kind &= mask;
                    if (kind != 0)
                        Data_03007ff0->callback(kind);
                    entry[0] = zero;
                }
                *(s32 *)(entry + 0x2C) = zero;
                entry = *(u8 **)(entry + 0x34);
            } while (entry != 0);
        }
        *(u8 **)(state + 0x20) = entry;
    }
}
