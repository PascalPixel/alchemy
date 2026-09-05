#include "types.h"

typedef s32 (*IwramIntegerSquareRoot)(s32);

s32 MeasureFixedPointPositionDistance(s32 *first_position, s32 *second_position)
{
    s32 delta_x = (*first_position++ - *second_position++) >> 16;
    s32 delta_y = (*first_position++ - *second_position++) >> 16;
    s32 delta_z = (*first_position - *second_position) >> 16;
    s32 delta_x_squared = delta_x * delta_x;
    s32 delta_y_squared = delta_y * delta_y;
    s32 delta_z_squared = delta_z * delta_z;

    return ((IwramIntegerSquareRoot)0x030001D8)(delta_x_squared + delta_y_squared + delta_z_squared);
}
