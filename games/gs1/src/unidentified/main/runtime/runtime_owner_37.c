#include "a8_state.h"
#include "object_factory.h"
#include "types.h"

void Func_080a33d4(struct State080a8088 *state, s32 arg1)
{
    void **output0;
    s32 index0;
    s32 fifth0;
    void **output1;
    s32 index1;
    s32 fifth1;
    void **output2;
    s32 index2;
    s32 fifth2;

    index0 = 0;
    fifth0 = 0xA8;
    output0 = &state->entries[0];
    do {
        *output0++ = Func_080150d0(2, index0, arg1, 0xF8, fifth0);
        index0++;
    } while (index0 <= 7);

    index1 = 8;
    fifth1 = 0xA8;
    output1 = &state->entries[8];
    do {
        *output1++ = Func_080150d0(2, index1, arg1, 0x100, fifth1);
        index1++;
    } while (index1 <= 15);

    index2 = 16;
    fifth2 = 0xA8;
    output2 = &state->entries[16];
    do {
        *output2++ = Func_080150d0(2, index2, arg1, 0x100, fifth2);
        index2++;
    } while (index2 <= 31);
}
