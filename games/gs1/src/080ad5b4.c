#include "types.h"
#include "four_object_motion.h"

#define NEGATIVE_POSITION_MASK ((s32)0xffff8000)
#define POSITIONS_Y_OFFSET 0x23c

/*
 * The binary preserves r0 at return: the null-object path leaves the index,
 * while the write path leaves the selected Y-field byte offset. Callers use
 * only the state update, but returning both observed values keeps the recovered
 * scalar ABI fully defined.
 */
s32 Func_080ad5b4(s32 index, s32 position_x, s32 position_y, s32 negative)
{
    struct FourObjectMotionState *state = Data_03001f2c;

    if (state->objects[index] != 0) {
        state->positions_x[index] = position_x;
        state->positions_y[index] =
            negative != 0 ? position_y | NEGATIVE_POSITION_MASK : position_y;
        return POSITIONS_Y_OFFSET + index * 2;
    }
    return index;
}
