#include "types.h"
#include "four_object_motion.h"

#define NEGATIVE_POSITION_MASK ((s32)0xffff8000)
#define POSITIONS_Y_OFFSET 0x23c

s32 FourObjectMotion_SetSlotPosition(s32 index, s32 position_x, s32 position_y, s32 negative)
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
