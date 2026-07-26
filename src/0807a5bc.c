#include "types.h"

struct State_0807a5bc {
    u8 padding[280];
    u8 values[4];
};

s32 Func_080796c4(s16 *output);
struct State_0807a5bc *Func_08077394(s32);

s32 Func_0807a5bc(s32 index)
{
    u16 owners[16];
    s32 result = 0;
    s32 count = Func_080796c4(owners);

    if (result < count) {
        u16 *owner = owners;
        s32 remaining = count;

        do {
            struct State_0807a5bc *state = Func_08077394(*owner++);

            if (index == -1) {
                result += state->values[0];
                result += state->values[1];
                result += state->values[2];
                result += state->values[3];
            } else {
                result += state->values[index];
            }
            remaining--;
        } while (remaining != 0);
    }
    return result;
}
