#include "types.h"

#define NEGATIVE_POSITION_MASK ((s32)0xffff8000)
#define POSITIONS_Y_OFFSET 0x23c

struct State_080ad5b4 {
    u8 padding_000[0x224];
    void *objects[4];
    s16 positions_x[4];
    s16 positions_y[4];
};

extern struct State_080ad5b4 *Data_03001f2c;

/*
 * The binary preserves r0 at return: the null-object path leaves the index,
 * while the write path leaves the selected Y-field byte offset. Callers use
 * only the state update, but returning both observed values keeps the recovered
 * scalar ABI fully defined.
 */
s32 Func_080ad5b4(s32 index, s32 position_x, s32 position_y, s32 negative)
{
    struct State_080ad5b4 *state = Data_03001f2c;

    if (state->objects[index] != 0) {
        state->positions_x[index] = position_x;
        state->positions_y[index] =
            negative != 0 ? position_y | NEGATIVE_POSITION_MASK : position_y;
        return POSITIONS_Y_OFFSET + index * 2;
    }
    return index;
}
