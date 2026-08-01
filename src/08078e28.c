#include "types.h"

struct Slot08078e28 {
    u16 value;
    u16 padding;
};

struct State08078e28 {
    u8 padding00[0x58];
    struct Slot08078e28 slots[32];
};

struct State08078e28 *Func_08077394(s32);
void Func_08078bf0(s32);

s32 Func_08078e28(s32 state_index, s32 value)
{
    struct State08078e28 *state = Func_08077394(state_index);
    s32 key = value & 0x3fff;
    s32 found = -1;
    s32 index;

    for (index = 0; index <= 30; index++) {
        s32 masked = state->slots[index].value & 0x3fff;

        if ((masked ^ key) == 0) {
            state->slots[index].value = masked;
            found = index;
            break;
        }
    }

    if (found < 0) {
        for (index = 0; index <= 30; index++) {
            s32 offset = (index * 4) + 0x58;
            if (*(u16 *)((u8 *)state + offset) == 0) {
                *(u16 *)((u8 *)state + offset) = key;
                found = index;
                break;
            }
        }
        if (found < 0) {
            return -1;
        }
    }

    Func_08078bf0(state_index);
    for (index = 0; index <= 31; index++) {
        if (state->slots[index].value == key) {
            break;
        }
    }
    return index;
}
